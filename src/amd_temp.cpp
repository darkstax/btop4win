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
constexpr int ADL_ERR = -1;

// ADL_PMLOG_SENSORS indices
constexpr int ADL_PMLOG_TEMPERATURE_CPU = 46;

// ADL PMLOG data structure
struct ADLPMLogDataOutput {
	int iSize;
	int iVersion;
	int iSensorCount;
};

struct ADLPMLogSensorData {
	int iSize;
	int iSensorIndex;
	int iSupported;
	int iMinValue;
	int iMaxValue;
	int iValue;  // millidegrees for temp
};

// Function pointer types
typedef void* (__stdcall* ADL_MAIN_MALLOC_CALLBACK)(int);
typedef int (__stdcall* ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (__stdcall* ADL_MAIN_CONTROL_DESTROY)();
typedef int (__stdcall* ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
typedef int (__stdcall* ADL2_NEW_QUERYPMLOGDATA_GET)(int, ADLPMLogDataOutput**);

HMODULE hADL = nullptr;
ADL_MAIN_CONTROL_DESTROY pfnDestroy = nullptr;
ADL_ADAPTER_NUMBEROFADAPTERS_GET pfnAdaptersGet = nullptr;
ADL2_NEW_QUERYPMLOGDATA_GET pfnPmlogGet = nullptr;
bool initialized = false;

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

	initialized = true;
	return true;
}

int AmdTemp::readCpuTemp() {
	if (!initialized || !pfnPmlogGet) return -1;

	int count = 0;
	if (pfnAdaptersGet(&count) != ADL_OK || count <= 0) return -1;

	ADLPMLogDataOutput* pmlog = nullptr;
	if (pfnPmlogGet(0, &pmlog) != ADL_OK || !pmlog) return -1;

	auto* sensors = (ADLPMLogSensorData*)((char*)pmlog + sizeof(ADLPMLogDataOutput));

	for (int i = 0; i < pmlog->iSensorCount; i++) {
		if (!sensors[i].iSupported) continue;
		int val = sensors[i].iValue;
		if (val < 10000 || val > 115000) continue;
		if (sensors[i].iSensorIndex == ADL_PMLOG_TEMPERATURE_CPU) {
			return val / 1000;
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
