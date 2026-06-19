// ============================================================================
// broker_client.hpp — SysMonBroker COM Client for btop4win
// ============================================================================
// 通过 COM Local Server 连接 SysMonBroker，获取完整进程列表和传感器数据。
// Broker 以管理员运行，btop4win 无需提权即可获得完整数据。
//
// 用法:
//   #include "broker_client.hpp"
//   BrokerClient broker;
//   if (broker.connect()) {
//       auto procs = broker.getProcesses();
//       double cpuTemp = broker.getCpuTemperature();
//   }
//
// 依赖: ole32.lib, oleaut32.lib
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
#include <optional>

// ---- CLSID / IID (必须与 Broker C# 端匹配) ----

// {7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C3}
DEFINE_GUID(CLSID_BrokerService,
    0x7B3F8A1C, 0x9D2E, 0x4F50, 0xB6, 0xC7, 0xD8, 0xE9, 0xF0, 0xA1, 0xB2, 0xC3);

// {7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C4}
DEFINE_GUID(IID_IBrokerService,
    0x7B3F8A1C, 0x9D2E, 0x4F50, 0xB6, 0xC7, 0xD8, 0xE9, 0xF0, 0xA1, 0xB2, 0xC4);

// {7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C5}
DEFINE_GUID(IID_IBrokerProcessService,
    0x7B3F8A1C, 0x9D2E, 0x4F50, 0xB6, 0xC7, 0xD8, 0xE9, 0xF0, 0xA1, 0xB2, 0xC5);

// {7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C6}
DEFINE_GUID(IID_IBrokerSensorService,
    0x7B3F8A1C, 0x9D2E, 0x4F50, 0xB6, 0xC7, 0xD8, 0xE9, 0xF0, 0xA1, 0xB2, 0xC6);

// ---- COM 接口定义 (vtable 顺序必须与 C# 端完全一致) ----

// Forward declarations for interface parameters
struct IBrokerProcessService;
struct IBrokerSensorService;

// IBrokerService: IUnknown + 4 methods
MIDL_INTERFACE("7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C4")
IBrokerService : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetProcessService(
        /* [out] */ IBrokerProcessService** ppService) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetSensorService(
        /* [out] */ IBrokerSensorService** ppService) = 0;

    virtual HRESULT STDMETHODCALLTYPE IsAlive(
        /* [out, retval] */ VARIANT_BOOL* pAlive) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVersion(
        /* [out, retval] */ BSTR* pVersion) = 0;
};

// IBrokerProcessService: IUnknown + 5 methods
MIDL_INTERFACE("7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C5")
IBrokerProcessService : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetProcesses(
        /* [out, retval] */ SAFEARRAY** ppProcesses) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetProcessCount(
        /* [out, retval] */ LONG* pCount) = 0;

    virtual HRESULT STDMETHODCALLTYPE Refresh(void) = 0;

    virtual HRESULT STDMETHODCALLTYPE KillProcess(
        /* [in] */ ULONG pid,
        /* [in] */ ULONG exitCode,
        /* [out, retval] */ LONG* pResult) = 0;

    virtual HRESULT STDMETHODCALLTYPE Authenticate(
        /* [in] */ ULONG clientPid,
        /* [in] */ BSTR exeHashHex,
        /* [out, retval] */ LONG* pResult) = 0;
};

// IBrokerSensorService: IUnknown + 7 methods
MIDL_INTERFACE("7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C6")
IBrokerSensorService : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetCpuTemperature(
        /* [out, retval] */ DOUBLE* pTemp) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCpuSource(
        /* [out, retval] */ BSTR* pSource) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetGpuCount(
        /* [out, retval] */ LONG* pCount) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetGpuName(
        /* [in] */ LONG index,
        /* [out, retval] */ BSTR* pName) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetGpuTemperature(
        /* [in] */ LONG index,
        /* [out, retval] */ DOUBLE* pTemp) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetGpuUsage(
        /* [in] */ LONG index,
        /* [out, retval] */ DOUBLE* pUsage) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetAllSensors(
        /* [out, retval] */ SAFEARRAY** ppSensors) = 0;
};

// ---- 数据结构 (必须与 C# BrokerProcessEntry / BrokerSensorEntry 内存布局一致) ----

#pragma pack(push, 8)  // C# 默认 Sequential 布局使用 8 字节对齐

struct BrokerProcessEntry
{
    ULONG Pid;                   // +0    (4 bytes)
    ULONG ParentPid;             // +4    (4 bytes)
    ULONG Threads;               // +8    (4 bytes)
    // 4 bytes padding to align BSTR pointer (8-byte alignment)
    BSTR  Name;                  // +16   (8 bytes, BSTR = OLECHAR*)
    BSTR  CommandLine;           // +24   (8 bytes)
    BSTR  UserName;              // +32   (8 bytes)
    LONGLONG PrivateMemoryBytes; // +40   (8 bytes)
    DOUBLE  CpuPercent;          // +48   (8 bytes)
    LONGLONG CreationTime;       // +56   (8 bytes)
    LONGLONG KernelTime;         // +64   (8 bytes)
    LONGLONG UserTime;           // +72   (8 bytes)
    LONGLONG IoReadBytes;        // +80   (8 bytes)
    LONGLONG IoWriteBytes;       // +88   (8 bytes)
    // Total: 96 bytes
};

struct BrokerSensorEntry
{
    LONG   CategoryTag;   // +0  (4 bytes)
    // 4 bytes padding
    BSTR   Name;          // +8  (8 bytes)
    DOUBLE Value;         // +16 (8 bytes)
    BSTR   Unit;          // +24 (8 bytes)
    // Total: 32 bytes
};

#pragma pack(pop)

// ---- 简化版进程信息 (btop4win 内部使用) ----

struct BrokerProcInfo
{
    size_t pid = 0;
    size_t ppid = 0;
    size_t threads = 0;
    std::string name;
    std::string cmd;
    std::string user;
    uint64_t mem = 0;
    double cpu_p = 0.0;
    uint64_t cpu_s = 0;     // creation time (FILETIME)
    uint64_t cpu_t = 0;     // kernel + user time
    uint64_t io_read = 0;
    uint64_t io_write = 0;
};

// ---- COM Client 封装 ----

class BrokerClient
{
public:
    BrokerClient() = default;
    ~BrokerClient() { disconnect(); }

    // 禁止拷贝
    BrokerClient(const BrokerClient&) = delete;
    BrokerClient& operator=(const BrokerClient&) = delete;

    /// <summary>连接到 SysMonBroker COM 服务器。
    /// 如果 Broker 未运行，尝试启动后重试。</summary>
    bool connect()
    {
        if (_service) return true; // 已连接

        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
        {
            _comInitOk = false;
            return false;
        }
        _comInitOk = true;

        // 第一次尝试连接
        hr = CoCreateInstance(CLSID_BrokerService, nullptr,
            CLSCTX_LOCAL_SERVER, IID_IBrokerService,
            reinterpret_cast<void**>(&_service));

        if (FAILED(hr))
        {
            // Broker 未运行 — 尝试启动
            if (tryLaunchBroker())
            {
                Sleep(2000); // 等待 Broker 初始化
                hr = CoCreateInstance(CLSID_BrokerService, nullptr,
                    CLSCTX_LOCAL_SERVER, IID_IBrokerService,
                    reinterpret_cast<void**>(&_service));
            }
        }

        if (FAILED(hr) || !_service) { disconnect(); return false; }

        // 获取子服务接口
        hr = _service->GetProcessService(&_procService);
        if (FAILED(hr)) { disconnect(); return false; }

        hr = _service->GetSensorService(&_sensorService);
        if (FAILED(hr)) { disconnect(); return false; }

        _connected = true;
        return true;
    }

    /// <summary>断开 COM 连接</summary>
    void disconnect()
    {
        if (_sensorService) { _sensorService->Release(); _sensorService = nullptr; }
        if (_procService) { _procService->Release(); _procService = nullptr; }
        if (_service) { _service->Release(); _service = nullptr; }
        if (_comInitOk) { CoUninitialize(); _comInitOk = false; }
        _connected = false;
        _authenticated = false;
    }

    /// <summary>Broker 是否已连接</summary>
    bool isConnected() const { return _connected; }

    // ====================================================================
    // 进程数据
    // ====================================================================

    /// <summary>获取完整进程列表（管理员权限，SE_DEBUG）</summary>
    std::vector<BrokerProcInfo> getProcesses()
    {
        std::vector<BrokerProcInfo> result;
        if (!_procService) return result;

        SAFEARRAY* psa = nullptr;
        HRESULT hr = _procService->GetProcesses(&psa);
        if (FAILED(hr) || !psa) return result;

        LONG lBound, uBound;
        SafeArrayGetLBound(psa, 1, &lBound);
        SafeArrayGetUBound(psa, 1, &uBound);

        BrokerProcessEntry* pData = nullptr;
        hr = SafeArrayAccessData(psa, reinterpret_cast<void**>(&pData));
        if (SUCCEEDED(hr) && pData)
        {
            for (LONG i = lBound; i <= uBound; i++)
            {
                auto& e = pData[i];
                BrokerProcInfo info;
                info.pid = e.Pid;
                info.ppid = e.ParentPid;
                info.threads = e.Threads;
                info.name = bstrToUtf8(e.Name);
                info.cmd = bstrToUtf8(e.CommandLine);
                info.user = bstrToUtf8(e.UserName);
                info.mem = e.PrivateMemoryBytes;
                info.cpu_p = e.CpuPercent;
                info.cpu_s = e.CreationTime;
                info.cpu_t = e.KernelTime + e.UserTime;
                info.io_read = e.IoReadBytes;
                info.io_write = e.IoWriteBytes;
                result.push_back(std::move(info));
            }
            SafeArrayUnaccessData(psa);
        }

        SafeArrayDestroy(psa);
        return result;
    }

    /// <summary>强制 Broker 立即刷新进程数据</summary>
    void refreshProcesses()
    {
        if (_procService) _procService->Refresh();
    }

    int getProcessCount()
    {
        LONG count = 0;
        if (_procService) _procService->GetProcessCount(&count);
        return count;
    }

    /// <summary>通过 Broker 结束指定进程（管理员级 SE_DEBUG）
    /// 返回 Win32 错误码，0=成功</summary>
    int killProcess(size_t pid, unsigned long exitCode = 1)
    {
        if (!_procService) return -1;
        LONG result = -1;
        HRESULT hr = _procService->KillProcess((ULONG)pid, (ULONG)exitCode, &result);
        if (FAILED(hr)) return -1;
        return (int)result;
    }

    /// <summary>向 Broker 认证自身身份（SHA256 白名单校验）
    /// 应在 connect() 成功后立即调用。
    /// 返回: 0=认证成功, 1=hash 不匹配, 2=进程不存在, 3=其他错误, -1=COM 失败</summary>
    int authenticate()
    {
        if (!_procService) return -1;

        std::string hash = computeSelfHash();
        if (hash.empty()) return 3;

        BSTR bstrHash = SysAllocStringLen(nullptr, (UINT)hash.size());
        if (!bstrHash) return 3;

        // UTF-8 → UTF-16 (SHA256 hex 是纯 ASCII，直接转即可)
        int wlen = MultiByteToWideChar(CP_UTF8, 0, hash.c_str(), (int)hash.size(), bstrHash, (int)hash.size());
        bstrHash[wlen] = L'\0';

        LONG result = -1;
        HRESULT hr = _procService->Authenticate((ULONG)GetCurrentProcessId(), bstrHash, &result);
        SysFreeString(bstrHash);

        if (FAILED(hr)) return -1;
        if (result == 0) _authenticated = true;
        return (int)result;
    }

    /// <summary>是否已完成认证</summary>
    bool isAuthenticated() const { return _authenticated; }

    // ====================================================================
    // 传感器数据
    // ====================================================================

    double getCpuTemperature()
    {
        DOUBLE temp = -1;
        if (_sensorService) _sensorService->GetCpuTemperature(&temp);
        return temp;
    }

    std::string getCpuSource()
    {
        BSTR source = nullptr;
        if (_sensorService) _sensorService->GetCpuSource(&source);
        std::string result = bstrToUtf8(source);
        if (source) SysFreeString(source);
        return result;
    }

    int getGpuCount()
    {
        LONG count = 0;
        if (_sensorService) _sensorService->GetGpuCount(&count);
        return count;
    }

    double getGpuTemperature(int index)
    {
        DOUBLE temp = -1;
        if (_sensorService) _sensorService->GetGpuTemperature(index, &temp);
        return temp;
    }

    double getGpuUsage(int index)
    {
        DOUBLE usage = -1;
        if (_sensorService) _sensorService->GetGpuUsage(index, &usage);
        return usage;
    }

    std::string getGpuName(int index)
    {
        BSTR name = nullptr;
        if (_sensorService) _sensorService->GetGpuName(index, &name);
        std::string result = bstrToUtf8(name);
        if (name) SysFreeString(name);
        return result;
    }

private:
    IBrokerService* _service = nullptr;
    IBrokerProcessService* _procService = nullptr;
    IBrokerSensorService* _sensorService = nullptr;
    bool _connected = false;
    bool _comInitOk = false;
    bool _authenticated = false;

    /// <summary>计算自身可执行文件的 SHA256 哈希（64 字符小写 hex）</summary>
    static std::string computeSelfHash()
    {
        wchar_t exePath[MAX_PATH];
        if (!GetModuleFileNameW(nullptr, exePath, MAX_PATH)) return "";

        HANDLE hFile = CreateFileW(exePath, GENERIC_READ, FILE_SHARE_READ,
            nullptr, OPEN_EXISTING, 0, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) return "";

        HANDLE hMap = CreateFileMappingW(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (!hMap) { CloseHandle(hFile); return ""; }

        void* pView = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
        LARGE_INTEGER fileSize;
        GetFileSizeEx(hFile, &fileSize);

        if (!pView) {
            CloseHandle(hMap);
            CloseHandle(hFile);
            return "";
        }

        // CNG SHA256
        BCRYPT_ALG_HANDLE hAlg = nullptr;
        BCRYPT_HASH_HANDLE hHash = nullptr;
        std::string result;

        if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, nullptr, 0) == 0)
        {
            if (BCryptCreateHash(hAlg, &hHash, nullptr, 0, nullptr, 0, 0) == 0)
            {
                if (BCryptHashData(hHash, (PUCHAR)pView, (ULONG)fileSize.QuadPart, 0) == 0)
                {
                    BYTE hash[32];
                    if (BCryptFinishHash(hHash, hash, sizeof(hash), 0) == 0)
                    {
                        // Convert to hex string
                        static const char hex[] = "0123456789abcdef";
                        result.reserve(64);
                        for (int i = 0; i < 32; i++) {
                            result += hex[hash[i] >> 4];
                            result += hex[hash[i] & 0x0F];
                        }
                    }
                }
                BCryptDestroyHash(hHash);
            }
            BCryptCloseAlgorithmProvider(hAlg, 0);
        }

        UnmapViewOfFile(pView);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return result;
    }

    /// <summary>尝试通过 ShellExecute 启动 Broker</summary>
    bool tryLaunchBroker()
    {
        // 查找 Broker 可执行文件
        const wchar_t* candidates[] = {
            L"%LOCALAPPDATA%\\SysMonCmdPal\\SysMonBroker.exe",
            L"%PROGRAMFILES%\\SysMonBroker\\SysMonBroker.exe",
        };

        for (auto& path : candidates)
        {
            wchar_t expanded[MAX_PATH];
            ExpandEnvironmentStringsW(path, expanded, MAX_PATH);

            if (GetFileAttributesW(expanded) != INVALID_FILE_ATTRIBUTES)
            {
                SHELLEXECUTEINFOW sei = {};
                sei.cbSize = sizeof(sei);
                sei.lpVerb = L"runas";  // 请求管理员
                sei.lpFile = expanded;
                sei.nShow = SW_HIDE;
                sei.fMask = SEE_MASK_NOCLOSEPROCESS;

                if (ShellExecuteExW(&sei))
                {
                    if (sei.hProcess)
                    {
                        WaitForSingleObject(sei.hProcess, 5000);
                        CloseHandle(sei.hProcess);
                    }
                    return true;
                }
            }
        }

        return false;
    }

    /// <summary>BSTR → std::string (UTF-8)</summary>
    static std::string bstrToUtf8(BSTR bstr)
    {
        if (!bstr) return "";
        int len = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, nullptr, 0, nullptr, nullptr);
        if (len <= 0) return "";
        std::string result(len - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, bstr, -1, result.data(), len, nullptr, nullptr);
        return result;
    }
};
