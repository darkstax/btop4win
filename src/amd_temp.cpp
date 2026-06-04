/* Copyright 2026 StarL
   CPU temperature reader: AMD ADL PMLOG (primary) + HWiNFO shared memory fallback */

#include "amd_temp.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define VC_EXTRALEAN
#include <windows.h>
#include <heapapi.h>

namespace {

constexpr int ADL_OK = 0;
constexpr int ADL_PMLOG_MAX_SENSORS = 256;

// ADL PMLOG sensor IDs (from adl_sdk.h)
constexpr int ADL_PMLOG_TEMPERATURE_EDGE    = 500;
constexpr int ADL_PMLOG_TEMPERATURE_MEM     = 501;
constexpr int ADL_PMLOG_TEMPERATURE_VRVDDC  = 502;
constexpr int ADL_PMLOG_TEMPERATURE_SOC     = 503;
constexpr int ADL_PMLOG_TEMPERATURE_CPU     = 504;

struct ADLPMLogDataOutput {
	int size;
	int version;
	int supported[ADL_PMLOG_MAX_SENSORS];
	int values[ADL_PMLOG_MAX_SENSORS];
};

// HWiNFO shared memory (fallback)
#define HWINFO_MAP_NAME   "Global\\HWiNFO_SENS_SM2"
#define HWINFO_STR_LEN    128
#define HWINFO_SIGNATURE  0x53695748  // "HWiS"
#define HWINFO_TYPE_TEMP  1

#pragma pack(push, 1)
struct HWiNFOReading {
	int tReading;
	unsigned long dwSensorIndex;
	unsigned long dwReadingID;
	char szLabelOrig[HWINFO_STR_LEN];
	char szLabelUser[HWINFO_STR_LEN];
	char szUnit[16];
	double Value;
	double ValueMin;
	double ValueMax;
	double ValueAvg;
};
#pragma pack(pop)

// Function pointer types
typedef void* (__stdcall* ADL_MALLOC_CB)(int);
typedef int (__stdcall* ADL_CREATE)(ADL_MALLOC_CB, int);
typedef int (__stdcall* ADL_DESTROY)();
typedef int (__stdcall* ADL_ADAPTER_COUNT)(int*);
typedef int (__stdcall* ADL2_PMLOG)(int, int, ADLPMLogDataOutput*);

// State
HMODULE hADL = nullptr;
ADL_DESTROY pfnDestroy = nullptr;
ADL_ADAPTER_COUNT pfnAdapters = nullptr;
ADL2_PMLOG pfnPmlog = nullptr;
bool adlOk = false;
int adlAdapter = 0;

HANDLE hHWMap = nullptr;
void* pHWMem = nullptr;
bool hwOk = false;

void* __stdcall ADL_Malloc(int size) {
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void adlCleanup() {
	if (pfnDestroy) pfnDestroy();
	if (hADL) { FreeLibrary(hADL); hADL = nullptr; }
	pfnDestroy = nullptr;
	pfnAdapters = nullptr;
	pfnPmlog = nullptr;
	adlOk = false;
}

void hwCleanup() {
	if (pHWMem) { UnmapViewOfFile(pHWMem); pHWMem = nullptr; }
	if (hHWMap) { CloseHandle(hHWMap); hHWMap = nullptr; }
	hwOk = false;
}

// ---- ADL ----

bool adlInit() {
	hADL = LoadLibraryW(L"atiadlxx.dll");
	if (!hADL) { hADL = LoadLibraryW(L"atiadlxy.dll"); if (!hADL) return false; }

	auto create = (ADL_CREATE)GetProcAddress(hADL, "ADL_Main_Control_Create");
	if (!create || create(ADL_Malloc, 1) != ADL_OK) { adlCleanup(); return false; }

	pfnDestroy  = (ADL_DESTROY)GetProcAddress(hADL, "ADL_Main_Control_Destroy");
	pfnAdapters = (ADL_ADAPTER_COUNT)GetProcAddress(hADL, "ADL_Adapter_NumberOfAdapters_Get");
	pfnPmlog    = (ADL2_PMLOG)GetProcAddress(hADL, "ADL2_New_QueryPMLogData_Get");
	if (!pfnPmlog || !pfnAdapters) { adlCleanup(); return false; }

	int count = 0;
	if (pfnAdapters(&count) != ADL_OK || count <= 0) { adlCleanup(); return false; }

	// Probe each adapter for CPU temp sensor (504)
	ADLPMLogDataOutput probe{};
	probe.size = sizeof(ADLPMLogDataOutput);
	probe.version = 1;

	for (int i = 0; i < count; i++) {
		if (pfnPmlog(0, i, &probe) == ADL_OK && probe.supported[ADL_PMLOG_TEMPERATURE_CPU]) {
			adlAdapter = i;
			adlOk = true;
			return true;
		}
	}
	adlCleanup();
	return false;
}

int adlReadTemp() {
	if (!adlOk || !pfnPmlog) return -1;

	ADLPMLogDataOutput data{};
	data.size = sizeof(ADLPMLogDataOutput);
	data.version = 1;
	if (pfnPmlog(0, adlAdapter, &data) != ADL_OK) return -1;

	// Priority: CPU(504) > SOC(503) > EDGE(500) > GFX/VRVDDC(502)
	const int ids[] = {
		ADL_PMLOG_TEMPERATURE_CPU,
		ADL_PMLOG_TEMPERATURE_SOC,
		ADL_PMLOG_TEMPERATURE_EDGE,
		ADL_PMLOG_TEMPERATURE_VRVDDC,
	};
	for (int j = 0; j < 4; j++) {
		int id = ids[j];
		if (data.supported[id]) {
			int v = data.values[id];
			if (v > 0 && v < 150000) return v / 1000;
		}
	}
	return -1;
}

// ---- HWiNFO fallback ----

bool hwInit() {
	hHWMap = OpenFileMappingA(FILE_MAP_READ, FALSE, HWINFO_MAP_NAME);
	if (!hHWMap) return false;
	pHWMem = MapViewOfFile(hHWMap, FILE_MAP_READ, 0, 0, 0);
	if (!pHWMem) { CloseHandle(hHWMap); hHWMap = nullptr; return false; }
	if (*(unsigned long*)pHWMem != HWINFO_SIGNATURE) {
		hwCleanup(); return false;
	}
	hwOk = true;
	return true;
}

int hwReadTemp() {
	if (!hwOk || !pHWMem) return -1;
	auto* h = (unsigned long*)pHWMem;
	char* base = (char*)pHWMem;
	unsigned long off = h[8], sz = h[9], n = h[10];
	if (sz < sizeof(HWiNFOReading)) return -1;

	for (unsigned long i = 0; i < n; i++) {
		auto* r = (HWiNFOReading*)(base + off + sz * i);
		if (r->tReading != HWINFO_TYPE_TEMP || r->Value <= 0) continue;
		if (strstr(r->szLabelOrig, "CPU Package") ||
			strstr(r->szLabelOrig, "Tctl/Tdie") ||
			strstr(r->szLabelOrig, "CPU Die") ||
			strstr(r->szLabelOrig, "CPU CCD") ||
			strstr(r->szLabelOrig, "CPU Tctl"))
			return (int)(r->Value + 0.5);
	}
	// Any CPU temp
	for (unsigned long i = 0; i < n; i++) {
		auto* r = (HWiNFOReading*)(base + off + sz * i);
		if (r->tReading != HWINFO_TYPE_TEMP || r->Value <= 0) continue;
		if (strstr(r->szLabelOrig, "CPU"))
			return (int)(r->Value + 0.5);
	}
	return -1;
}

} // anonymous namespace

// ========== Public API ==========

bool AmdTemp::init() {
	if (adlInit()) return true;
	return hwInit();
}

int AmdTemp::readCpuTemp() {
	if (adlOk) {
		int t = adlReadTemp();
		if (t > 0) return t;
	}
	if (hwOk) return hwReadTemp();
	return -1;
}

void AmdTemp::cleanup() {
	adlCleanup();
	hwCleanup();
}
