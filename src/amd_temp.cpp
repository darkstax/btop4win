/* Copyright 2026 StarL
   AMD ADL PMLOG wrapper - reads CPU temperature directly from AMD driver */

#include "amd_temp.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define VC_EXTRALEAN
#include <windows.h>
#include <heapapi.h>

namespace {

constexpr int ADL_OK = 0;

// ADL PMLOG max sensors
constexpr int ADL_PMLOG_MAX_SENSORS = 256;

// AMD CPU temperature sensor IDs (ADL2_PMLOG_*)
constexpr int ADL_PMLOG_TEMPERATURE_EDGE = 500;   // Edge/Tdie — primary CPU temp on AMD
constexpr int ADL_PMLOG_TEMPERATURE_HOTSPOT = 501; // Hot Spot
constexpr int ADL_PMLOG_TEMPERATURE_CPU = 503;     // CPU sensor (some APUs)

// Correct ADL PMLog data structure (matches ADL SDK)
struct ADLPMLogDataOutput {
	int size;
	int version;
	int supported[ADL_PMLOG_MAX_SENSORS];  // non-zero if sensor ID is supported
	int values[ADL_PMLOG_MAX_SENSORS];     // current readings (indexed by sensor ID)
};

// Function pointer types
typedef void* (__stdcall* ADL_MAIN_MALLOC_CALLBACK)(int);
typedef int (__stdcall* ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (__stdcall* ADL_MAIN_CONTROL_DESTROY)();
typedef int (__stdcall* ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
typedef int (__stdcall* ADL2_NEW_QUERYPMLOGDATA_GET)(int, int, ADLPMLogDataOutput*);

HMODULE hADL = nullptr;
ADL_MAIN_CONTROL_DESTROY pfnDestroy = nullptr;
ADL_ADAPTER_NUMBEROFADAPTERS_GET pfnAdaptersGet = nullptr;
ADL2_NEW_QUERYPMLOGDATA_GET pfnPmlogGet = nullptr;
bool initialized = false;
int adapterIndex = 0;  // adapter with valid PMLog data

void* __stdcall ADL_Main_Malloc(int size) {
	return HeapAlloc(GetProcessHeap(), 0, size);
}

} // anonymous namespace

bool AmdTemp::init() {
	if (initialized) return true;

	hADL = LoadLibraryW(L"atiadlxx.dll");
	if (!hADL) {
		hADL = LoadLibraryW(L"atiadlxy.dll");
		if (!hADL) return false;
	}

	auto pfnCreate = (ADL_MAIN_CONTROL_CREATE)GetProcAddress(hADL, "ADL_Main_Control_Create");
	if (!pfnCreate) { cleanup(); return false; }

	int res = pfnCreate(ADL_Main_Malloc, 1);
	if (res != ADL_OK) { cleanup(); return false; }

	pfnDestroy = (ADL_MAIN_CONTROL_DESTROY)GetProcAddress(hADL, "ADL_Main_Control_Destroy");
	pfnAdaptersGet = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(hADL, "ADL_Adapter_NumberOfAdapters_Get");
	pfnPmlogGet = (ADL2_NEW_QUERYPMLOGDATA_GET)GetProcAddress(hADL, "ADL2_New_QueryPMLogData_Get");

	if (!pfnPmlogGet || !pfnAdaptersGet) { cleanup(); return false; }

	// Find the first adapter that supports PMLog temperature sensors
	int count = 0;
	if (pfnAdaptersGet(&count) != ADL_OK || count <= 0) { cleanup(); return false; }

	ADLPMLogDataOutput probe{};
	probe.size = sizeof(ADLPMLogDataOutput);
	probe.version = 1;

	for (int i = 0; i < count; i++) {
		if (pfnPmlogGet(0, i, &probe) == ADL_OK) {
			// Check if this adapter has any CPU temp sensor
			if (probe.supported[ADL_PMLOG_TEMPERATURE_EDGE] ||
				probe.supported[ADL_PMLOG_TEMPERATURE_HOTSPOT] ||
				probe.supported[ADL_PMLOG_TEMPERATURE_CPU]) {
				adapterIndex = i;
				initialized = true;
				return true;
			}
		}
	}

	// No adapter with CPU temp sensor found
	cleanup();
	return false;
}

int AmdTemp::readCpuTemp() {
	if (!initialized || !pfnPmlogGet) return -1;

	ADLPMLogDataOutput data{};
	data.size = sizeof(ADLPMLogDataOutput);
	data.version = 1;

	if (pfnPmlogGet(0, adapterIndex, &data) != ADL_OK) return -1;

	// Try sensors in priority order: Edge (Tdie) > Hot Spot > CPU
	const int sensorIds[] = {ADL_PMLOG_TEMPERATURE_EDGE, ADL_PMLOG_TEMPERATURE_HOTSPOT, ADL_PMLOG_TEMPERATURE_CPU};
	for (int j = 0; j < 3; j++) {
		int sensorId = sensorIds[j];
		if (data.supported[sensorId]) {
			int val = data.values[sensorId];
			if (val > 0 && val < 150000) {  // valid range: 0-150°C in millidegrees
				return val / 1000;
			}
		}
	}
	return -1;
}

void AmdTemp::cleanup() {
	if (pfnDestroy) pfnDestroy();
	if (hADL) { FreeLibrary(hADL); hADL = nullptr; }
	pfnDestroy = nullptr;
	pfnAdaptersGet = nullptr;
	pfnPmlogGet = nullptr;
	initialized = false;
}
