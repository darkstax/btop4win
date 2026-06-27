// ============================================================================
// broker_client.hpp — SysMonBroker COM Client for btop4win
// ============================================================================
// 通过 IDispatch 连接 SysMonBroker（flat interface，所有方法在 IBrokerService 上）。
// IDispatch 是 COM 标准接口，无需自定义接口注册即可跨进程封送。
//
// 用法:
//   BrokerClient broker;
//   if (broker.connect()) {
//       auto procs = broker.getProcesses();
//       double cpuTemp = broker.getCpuTemperature();
//   }
// ============================================================================

#pragma once

#include <Windows.h>
#include <shellapi.h>
#include <initguid.h>
#include <comdef.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#include <string>
#include <vector>

// ---- CLSID ----
DEFINE_GUID(CLSID_BrokerService,
0x7B3F8A1C, 0x9D2E, 0x4F50, 0xB6, 0xC7, 0xD8, 0xE9, 0xF0, 0xA1, 0xB2, 0xC3);

// ---- 数据结构 ----
#pragma pack(push, 8)
struct BrokerProcessEntry {
    ULONG Pid, ParentPid, Threads;
    BSTR Name, CommandLine, UserName;
    LONGLONG Mem;
    DOUBLE CpuP;
    LONGLONG CpuS, CpuK, CpuU, IoR, IoW;
};
#pragma pack(pop)

struct BrokerProcInfo {
    size_t pid = 0, ppid = 0, threads = 0;
    std::string name, cmd, user;
    uint64_t mem = 0;
    double cpu_p = 0.0;
    uint64_t cpu_s = 0, cpu_t = 0;
};

// ---- IDispatch 调用助手 ----
class BrokerClient {
public:
    BrokerClient() = default;
    ~BrokerClient() { disconnect(); }

    bool connect() {
        if (_connected) return true;
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) { _comInitOk = false; return false; }
        _comInitOk = true;

        // 用 IDispatch 连接（永远可封送）
        IDispatch* pD = nullptr;
        hr = CoCreateInstance(CLSID_BrokerService, nullptr,
            CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pD);
        if (FAILED(hr) || !pD) { disconnect(); return false; }

        _svc = pD;
        _connected = true;
        return true;
    }

    void disconnect() {
        if (_svc) { _svc->Release(); _svc = nullptr; }
        if (_comInitOk) { CoUninitialize(); _comInitOk = false; }
        _connected = false; _authenticated = false;
    }

    bool isConnected() const { return _connected; }
    bool isAuthenticated() const { return _authenticated; }

    int authenticate() {
        if (!_svc) return -1;
        std::string hash = computeSelfHash();
        if (hash.empty()) return 3;

        BSTR bstrHash = SysAllocStringLen(nullptr, (UINT)hash.size() + 1);
        if (!bstrHash) return 3;
        MultiByteToWideChar(CP_UTF8, 0, hash.c_str(), (int)hash.size(), bstrHash, (int)hash.size() + 1);

        DISPID did = getDispId(L"Authenticate");
        if (did == -1) { SysFreeString(bstrHash); return -1; }

        VARIANTARG args[2];
        VariantInit(&args[0]); args[0].vt = VT_BSTR; args[0].bstrVal = bstrHash;
        VariantInit(&args[1]); args[1].vt = VT_I4; args[1].lVal = (LONG)GetCurrentProcessId();
        DISPPARAMS dp = {args, NULL, 2, 0};
        VARIANT result; VariantInit(&result);
        HRESULT hr = _svc->Invoke(did, _IID0(), LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, NULL, NULL);
        SysFreeString(bstrHash);

        if (FAILED(hr)) return -1;
        LONG authResult = (result.vt == VT_I4) ? result.lVal : -1;
        VariantClear(&result);
        if (authResult == 0) _authenticated = true;
        return (int)authResult;
    }

    std::vector<BrokerProcInfo> getProcesses() {
        std::vector<BrokerProcInfo> result;
        if (!_svc) return result;
        VARIANT v = invoke0(L"GetProcesses");
        // GetProcesses returns byte[] with fixed-size records: PID(4)+PPID(4)+Thr(4)+Name(260)+Cmd(1024)+User(256)+Mem(8)+CpuP(8)+Cs(8)+Ck(8)+Cu(8)+IoR(8)+IoW(8)=1608
        if ((v.vt & VT_ARRAY) && (v.vt & VT_UI1)) {
            SAFEARRAY* psa = v.parray;
            if (psa) {
                unsigned char* data = nullptr;
                if (SUCCEEDED(SafeArrayAccessData(psa, (void**)&data))) {
                    LONG lb, ub;
                    SafeArrayGetLBound(psa, 1, &lb);
                    SafeArrayGetUBound(psa, 1, &ub);
                    int totalBytes = (int)(ub - lb + 1);
                    const int REC_SIZE = 1608;
                    int count = totalBytes / REC_SIZE;
                    for (int i = 0; i < count; i++) {
                        int pos = i * REC_SIZE;
                        auto p = data + pos;
                        BrokerProcInfo info;
                        info.pid     = *(uint32_t*)(p); p += 4;
                        info.ppid    = *(uint32_t*)(p); p += 4;
                        info.threads = *(uint32_t*)(p); p += 4;
                        info.name    = std::string((char*)p, strnlen_s((char*)p, 260)); p += 260;
                        info.cmd     = std::string((char*)p, strnlen_s((char*)p, 1024)); p += 1024;
                        info.user    = std::string((char*)p, strnlen_s((char*)p, 256)); p += 256;
                        info.mem     = *(uint64_t*)(p); p += 8;
                        info.cpu_p   = *(double*)(p); p += 8;
                        info.cpu_s   = *(uint64_t*)(p); p += 8;
                        uint64_t ck  = *(uint64_t*)(p); p += 8;
                        uint64_t cu  = *(uint64_t*)(p); p += 8;
                        info.cpu_t   = ck + cu;
                        result.push_back(std::move(info));
                    }
                    SafeArrayUnaccessData(psa);
                }
            }
        }
        VariantClear(&v);
        return result;
    }

    double getCpuTemperature() { return invokeDouble(L"GetCpuTemperature"); }
    double getCpuClock() { return invokeDouble(L"GetCpuClock"); }
    int getGpuCount() {
        VARIANT v = invoke0(L"GetGpuCount");
        int r = -1;
        if (v.vt == VT_I4) r = v.lVal;
        else if (v.vt == VT_R8) r = (int)v.dblVal;
        VariantClear(&v);
        return r;
    }
    double getGpuTemperature(int i) { return invokeDouble1(L"GetGpuTemperature", i); }
    double getGpuUsage(int i) { return invokeDouble1(L"GetGpuUsage", i); }
    std::string getGpuName(int i) { return invokeString1(L"GetGpuName", i); }

    static std::string computeSelfHashPublic() { return computeSelfHash(); }

private:
    IDispatch* _svc = nullptr;
    bool _connected = false, _comInitOk = false, _authenticated = false;
    static const IID _IID0() { static const IID g = {0}; return g; }

    DISPID getDispId(const wchar_t* name) {
        if (!_svc) return -1;
        OLECHAR* n = const_cast<OLECHAR*>(name);
        DISPID d;
        if (SUCCEEDED(_svc->GetIDsOfNames(_IID0(), &n, 1, LOCALE_USER_DEFAULT, &d))) return d;
        return -1;
    }

    VARIANT invoke0(const wchar_t* name) {
        VARIANT v; VariantInit(&v); if (!_svc) return v;
        DISPID d = getDispId(name); if (d == -1) return v;
        DISPPARAMS p = {};
        _svc->Invoke(d, _IID0(), LOCALE_USER_DEFAULT, DISPATCH_METHOD, &p, &v, NULL, NULL);
        return v;
    }

    double invokeDouble(const wchar_t* name) {
        VARIANT v = invoke0(name); double r = (v.vt == VT_R8) ? v.dblVal : -1; VariantClear(&v); return r;
    }

    VARIANT invoke1(const wchar_t* name, LONG arg) {
        VARIANT v; VariantInit(&v); if (!_svc) return v;
        DISPID d = getDispId(name); if (d == -1) return v;
        VARIANTARG a; VariantInit(&a); a.vt = VT_I4; a.lVal = arg;
        DISPPARAMS p = {&a, NULL, 1, 0};
        _svc->Invoke(d, _IID0(), LOCALE_USER_DEFAULT, DISPATCH_METHOD, &p, &v, NULL, NULL);
        VariantClear(&a);
        return v;
    }

    double invokeDouble1(const wchar_t* name, LONG arg) {
        VARIANT v = invoke1(name, arg); double r = (v.vt == VT_R8) ? v.dblVal : -1; VariantClear(&v); return r;
    }

    std::string invokeString1(const wchar_t* name, LONG arg) {
        VARIANT v = invoke1(name, arg);
        std::string r;
        if (v.vt == VT_BSTR && v.bstrVal) r = b2s(v.bstrVal);
        VariantClear(&v);
        return r;
    }

    static std::string b2s(BSTR b) {
        if (!b) return "";
        int l = WideCharToMultiByte(CP_UTF8, 0, b, -1, nullptr, 0, nullptr, nullptr);
        if (l <= 0) return "";
        std::string r(l - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, b, -1, &r[0], l, nullptr, nullptr);
        return r;
    }

    static std::string computeSelfHash() {
        wchar_t exePath[MAX_PATH];
        if (!GetModuleFileNameW(nullptr, exePath, MAX_PATH)) return "";
        // FILE_SHARE_DELETE 让 btop.exe 运行时也能被读取 (scoop 更新场景)
        HANDLE hFile = CreateFileW(exePath, GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr, OPEN_EXISTING, 0, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) return "";
        HANDLE hMap = CreateFileMappingW(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (!hMap) { CloseHandle(hFile); return ""; }
        void* pView = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
        LARGE_INTEGER fs; GetFileSizeEx(hFile, &fs);
        if (!pView) { CloseHandle(hMap); CloseHandle(hFile); return ""; }
        BCRYPT_ALG_HANDLE hAlg = nullptr; BCRYPT_HASH_HANDLE hHash = nullptr;
        std::string result;
        if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, nullptr, 0) == 0) {
            if (BCryptCreateHash(hAlg, &hHash, nullptr, 0, nullptr, 0, 0) == 0) {
                if (BCryptHashData(hHash, (PUCHAR)pView, (ULONG)fs.QuadPart, 0) == 0) {
                    BYTE hash[32];
                    if (BCryptFinishHash(hHash, hash, sizeof(hash), 0) == 0) {
                        static const char hex[] = "0123456789abcdef";
                        result.reserve(64);
                        for (int i = 0; i < 32; i++) { result += hex[hash[i] >> 4]; result += hex[hash[i] & 0x0F]; }
                    }
                }
                BCryptDestroyHash(hHash);
            }
            BCryptCloseAlgorithmProvider(hAlg, 0);
        }
        UnmapViewOfFile(pView); CloseHandle(hMap); CloseHandle(hFile);
        return result;
    }
};
