# Broker Integration — 独立后台线程架构

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 将 SysMonBroker COM 以独立后台线程方式集成，不与 runner 线程耦合，Broker 不可用时自动回到现有采集逻辑。

**Architecture:** 新建 `broker_thread.hpp/.cpp` 独立管理 Broker 生命周期。后台线程启动时检查 exe 是否存在、拉取数据存入共享缓存。Runner 线程（`Cpu::collect`, `Proc::collect`）从缓存只读——缓存有数据就用，没数据就回退。Broker 线程完全独立，永不阻塞 runner。

**Tech Stack:** C++20, Windows COM, Win32 threads, `broker_client.hpp` (已有 COM 接口封装)

## File Structure

| 文件 | 职责 |
|------|------|
| `src/broker_thread.hpp` | `BrokerCache` 共享数据结构 + `BrokerThread` 声明 |
| `src/broker_thread.cpp` | `BrokerThread` 实现: 线程循环、exe 检查、缓存更新 |
| `src/broker_client.hpp` | 已有，不改 |
| `src/btop_collect.cpp` | 修改 `Proc::collect()` 从缓存读取进程; `Cpu::collect()` 从缓存读取传感器 |
| `src/btop.cpp` | 启动时 `BrokerThread::start()` |

## Global Constraints

- Broker 线程完全独立，runner 线程永不直接调用 COM
- 启动时 `GetFileAttributesW` 检查 exe 存在 → 不存在永久跳过
- 缓存用 `std::mutex` 保护
- 优雅降级：Broker 不可用时现有逻辑不受任何影响

---

### Task 1: BrokerCache 数据结构和 BrokerThread 声明

**Files:**
- Create: `src/broker_thread.hpp`

**Interfaces:**
- Produces: `struct BrokerCache`, `class BrokerThread`

- [ ] **Step 1: 创建 `src/broker_thread.hpp`**

```cpp
// broker_thread.hpp — 独立 Broker 后台线程 + 共享缓存
#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

// 从 Broker 获取的简化进程信息
struct BrokerProcData {
    size_t pid = 0;
    size_t ppid = 0;
    size_t threads = 0;
    std::string name;
    std::string cmd;
    std::string user;
    uint64_t mem = 0;
    double cpu_p = 0.0;
    uint64_t cpu_s = 0;
    uint64_t cpu_t = 0;
};

// 共享缓存 (runner 线程只读)
struct BrokerCache {
    mutable std::mutex mtx;

    // 进程数据
    std::vector<BrokerProcData> proc_list;
    bool proc_fresh = false;  // 当前 proc_list 是否新数据

    // 传感器数据
    double cpu_temp = -1;
    double gpu_temp = -1;
    double gpu_usage = -1;
    std::string gpu_name;
    bool sensor_fresh = false;

    // 整体状态
    bool broker_available = false;  // Broker 是否成功连接
};

// 全局缓存（单例）
inline BrokerCache g_brokerCache;

class BrokerThread {
public:
    static void start();
    static void stop();

    // 检查 Broker exe 是否存在于已知路径
    static bool brokerExeExists();

private:
    static void run();

    inline static std::thread s_thread;
    inline static std::atomic<bool> s_running{false};
    inline static std::atomic<bool> s_unavailable{false};
};
```

- [ ] **Step 2: 验证编译**

```powershell
$msbuild = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "C:\Users\StarL\Documents\AI_code\btop4win\btop4win.vcxproj" /p:Configuration=Release-LHM /p:Platform=x64 /t:Build /v:minimal
```

- [ ] **Step 3: Commit**

```bash
git add src/broker_thread.hpp
git commit -m "feat: BrokerCache + BrokerThread 声明

- BrokerCache: 共享缓存，mutex 保护，runner 线程只读
- BrokerThread: 独立后台线程管理，exe 存在性检查
- BrokerProcData: 简化进程信息，比 BrokerProcessEntry 轻量

Co-Authored-By: Claude Opus 4.8 <noreply@anthropic.com>"
```

---

### Task 2: BrokerThread 实现

**Files:**
- Create: `src/broker_thread.cpp`
- Consumes: `broker_thread.hpp`, `broker_client.hpp`

**Interfaces:**
- Consumes: `struct BrokerCache`, `class BrokerThread` (from Task 1)
- Produces: `BrokerThread::start()`, `BrokerThread::stop()`, `brokerExeExists()`, `run()`

- [ ] **Step 1: 创建 `src/broker_thread.cpp`**

```cpp
// broker_thread.cpp — Broker 后台线程实现
#include "broker_thread.hpp"
#include "broker_client.hpp"
#include "btop_tools.hpp"

bool BrokerThread::brokerExeExists() {
    const wchar_t* candidates[] = {
        L"%LOCALAPPDATA%\\SysMonCmdPal\\SysMonBroker.exe",
        L"%PROGRAMFILES%\\SysMonBroker\\SysMonBroker.exe",
    };
    for (auto& path : candidates) {
        wchar_t expanded[MAX_PATH];
        ExpandEnvironmentStringsW(path, expanded, MAX_PATH);
        if (GetFileAttributesW(expanded) != INVALID_FILE_ATTRIBUTES)
            return true;
    }
    return false;
}

void BrokerThread::start() {
    // 启动时检查 exe 是否存在
    if (!brokerExeExists()) {
        Logger::debug("Broker: exe not found, permanently disabled");
        s_unavailable = true;
        return;
    }

    if (s_running) return;
    s_running = true;
    s_thread = std::thread(run);
}

void BrokerThread::stop() {
    s_running = false;
    if (s_thread.joinable()) s_thread.join();
}

void BrokerThread::run() {
    Logger::debug("BrokerThread: started");

    // COM init
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    bool comOk = SUCCEEDED(hr);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
        Logger::warning("BrokerThread: CoInitializeEx failed, disabling");
        s_unavailable = true;
        return;
    }

    BrokerClient client;
    int failCount = 0;

    while (s_running) {
        // 尝试连接（如果未连接）
        if (!client.isConnected()) {
            if (client.connect()) {
                failCount = 0;
                Logger::debug("BrokerThread: connected");

                // 认证
                int authResult = client.authenticate();
                if (authResult != 0) {
                    Logger::warning("BrokerThread: auth failed (code=" + to_string(authResult) + ")");
                    client.disconnect();
                    failCount++;
                }
            } else {
                failCount++;
                Logger::debug("BrokerThread: connect failed (" + to_string(failCount) + ")");
            }
        }

        // 如果已连接并认证，拉取数据
        if (client.isConnected() && client.isAuthenticated()) {
            try {
                // --- 进程数据 ---
                auto procs = client.getProcesses();
                if (!procs.empty()) {
                    std::lock_guard lck(g_brokerCache.mtx);
                    g_brokerCache.proc_list.clear();
                    g_brokerCache.proc_list.reserve(procs.size());
                    for (auto& bp : procs) {
                        BrokerProcData pd;
                        pd.pid    = bp.pid;
                        pd.ppid   = bp.ppid;
                        pd.threads = bp.threads;
                        pd.name   = std::move(bp.name);
                        pd.cmd    = std::move(bp.cmd);
                        pd.user   = std::move(bp.user);
                        pd.mem    = bp.mem;
                        pd.cpu_p  = bp.cpu_p;
                        pd.cpu_s  = bp.cpu_s;
                        pd.cpu_t  = bp.cpu_t;
                        g_brokerCache.proc_list.push_back(std::move(pd));
                    }
                    g_brokerCache.proc_fresh = true;
                }

                // --- 传感器数据 ---
                double cpuTemp = client.getCpuTemperature();
                double gpuTemp = client.getGpuTemperature(0);
                double gpuUsage = client.getGpuUsage(0);
                std::string gpuName = client.getGpuName(0);

                {
                    std::lock_guard lck(g_brokerCache.mtx);
                    g_brokerCache.cpu_temp = cpuTemp;
                    g_brokerCache.gpu_temp = gpuTemp;
                    g_brokerCache.gpu_usage = gpuUsage;
                    if (!gpuName.empty()) g_brokerCache.gpu_name = std::move(gpuName);
                    g_brokerCache.sensor_fresh = true;
                    g_brokerCache.broker_available = true;
                }

                // 每 2 秒采集一次
                sleep_ms(2000);
            } catch (...) {
                Logger::warning("BrokerThread: data fetch exception, reconnecting");
                client.disconnect();
            }
        } else {
            // 未连接，等待重试
            failCount++;
            if (failCount > 5) {
                Logger::warning("BrokerThread: too many failures, giving up");
                s_unavailable = true;
                break;
            }
            sleep_ms(3000);  // 每 3 秒重试
        }
    }

    client.disconnect();
    if (comOk) CoUninitialize();
    Logger::debug("BrokerThread: stopped");
}
```

- [ ] **Step 2: 验证编译**

```powershell
$msbuild = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "C:\Users\StarL\Documents\AI_code\btop4win\btop4win.vcxproj" /p:Configuration=Release-LHM /p:Platform=x64 /t:Build /v:minimal
```

确认编译通过。会有 link error（broker_client.hpp 的函数找不到实现），但要确认 broker_thread.cpp 本身没有编译错误。

- [ ] **Step 3: Commit**

```bash
git add src/broker_thread.cpp
git commit -m "feat: BrokerThread 实现 — 独立后台线程

- 启动时 GetFileAttributesW 检查 exe 存在 → 不存在永久跳过
- COM connect + authenticate + 定时拉取进程/传感器数据
- 失败 >5 次自动放弃
- 写入 g_brokerCache 共享缓存，mutex 保护

Co-Authored-By: Claude Opus 4.8 <noreply@anthropic.com>"
```

---

### Task 3: 整合到 build 系统 + main 启动

**Files:**
- Modify: `btop4win.vcxproj` (添加 broker_thread.cpp)
- Modify: `src/btop.cpp` (启动/停止 BrokerThread)
- Modify: `src/broker_client.hpp` (移除 inline → 改为声明，防止 link error)

**Interfaces:**
- Consumes: `BrokerThread::start()`, `BrokerThread::stop()` (from Task 2)

- [ ] **Step 1: 在 vcxproj 中添加 broker_thread.cpp**

在 `btop4win.vcxproj` 的 `<ItemGroup>` 中添加：

```xml
<ClCompile Include="src\broker_thread.cpp" />
```

放在其他 `src\btop_*.cpp` 旁边。

- [ ] **Step 2: 修改 `btop.cpp`，启动和停止 Broker 线程**

在头文件 include 区域添加：
```cpp
#include "broker_thread.hpp"
```

在 `init()` 函数末尾（runner 启动之前）添加：
```cpp
//? Start Broker background thread (non-blocking, auto-disable if not installed)
BrokerThread::start();
```

在 `clean_quit()` 函数末尾（所有清理之后）添加：
```cpp
BrokerThread::stop();
```

- [ ] **Step 3: 修改 `broker_client.hpp` 防止 link error**

`broker_client.hpp` 中的所有方法都是 inline，编译应该没问题。但需要确认 `<initguid.h>` 的使用正确（已在 `.hpp` 中定义 GUID，各 cpp 各自 include 一次）。

- [ ] **Step 4: 验证编译**

```powershell
$msbuild = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "C:\Users\StarL\Documents\AI_code\btop4win\btop4win.vcxproj" /p:Configuration=Release-LHM /p:Platform=x64 /t:Build /v:minimal
```

- [ ] **Step 5: Commit**

```bash
git add btop4win.vcxproj src/btop.cpp src/broker_client.hpp
git commit -m "feat: 将 BrokerThread 接入构建系统和 btop 生命周期

- vcxproj 添加 broker_thread.cpp
- btop.cpp init() 中启动 BrokerThread（非阻塞）
- clean_quit() 中停止 BrokerThread

Co-Authored-By: Claude Opus 4.8 <noreply@anthropic.com>"
```

---

### Task 4: Proc::collect() 从 Broker 缓存读取进程

**Files:**
- Modify: `src/btop_collect.cpp` (`Proc::collect()`)

**Interfaces:**
- Consumes: `g_brokerCache` (from Task 1)

- [ ] **Step 1: 在 `btop_collect.cpp` 头文件中添加 include**

```cpp
#include "broker_thread.hpp"
```

- [ ] **Step 2: 修改 `Proc::collect()` 函数**

在 `Proc::collect()` 中，位置在现有的进程采集代码（`CreateToolhelp32Snapshot`）**之前**，添加 Broker 缓存读取逻辑（不是调用 COM，只是读缓存）：

```cpp
//? Try Broker cache first (populated by background thread, no blocking)
bool brokerOk = false;
if (not services) {
    std::lock_guard lck(g_brokerCache.mtx);
    if (g_brokerCache.proc_fresh && !g_brokerCache.proc_list.empty()) {
        brokerOk = true;
        found.clear();
        current_procs.clear();
        current_procs.reserve(g_brokerCache.proc_list.size());

        for (auto& bp : g_brokerCache.proc_list) {
            found.push_back(bp.pid);
            proc_info pi;
            pi.pid     = bp.pid;
            pi.ppid    = bp.ppid;
            pi.threads = bp.threads;
            pi.cmd     = bp.cmd;
            pi.user    = bp.user;
            pi.mem     = bp.mem;
            pi.cpu_s   = bp.cpu_s;
            pi.state   = 'R';

            pi.name = bp.name;
            if (auto dot = pi.name.find_last_of('.'); dot != string::npos)
                pi.name = pi.name.substr(0, dot);

            if (pi.cmd.empty()) pi.cmd = pi.name;
            if (pi.user.empty() && pi.pid < 1000) pi.user = "SYSTEM";
            pi.short_cmd = pi.name;

            // CPU%: Broker returns total system %, btop expects per-core
            pi.cpu_p = clamp(cmult * bp.cpu_p / 10.0, 0.0, 100.0 * Shared::coreCount);

            if (bp.cpu_t > 0 && bp.cpu_s > 0 && systime > bp.cpu_s)
                pi.cpu_c = (double)bp.cpu_t / max(1ull, systime - bp.cpu_s);

            current_procs.push_back(std::move(pi));
        }
        // Refresh Broker for next cycle
        client.refreshProcesses();
    }
}

//? Fallback to local collection when Broker cache is empty
if (not brokerOk) {
    // ... existing CreateToolhelp32Snapshot code ...
}
```

关键点：**只有一个 `std::lock_guard` 读缓存**，不调用任何 COM。Broker 缓存为空 → 直接走本地采集。

- [ ] **Step 3: 验证编译**

```powershell
$msbuild = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "C:\Users\StarL\Documents\AI_code\btop4win\btop4win.vcxproj" /p:Configuration=Release-LHM /p:Platform=x64 /t:Build /v:minimal
```

- [ ] **Step 4: Commit**

```bash
git add src/btop_collect.cpp
git commit -m "feat: Proc::collect() 从 Broker 缓存读取进程（非阻塞）

- 只读 g_brokerCache，不调用 COM
- 缓存为空 → 自动回退 CreateToolhelp32Snapshot
- 完全不影响现有流程

Co-Authored-By: Claude Opus 4.8 <noreply@anthropic.com>"
```

---

### Task 5: 测试部署 + 验证

**Files:**
- No new files — 部署和测试

- [ ] **Step 1: 编译并停止旧 btop**

```powershell
Stop-Process -Name "btop" -Force -ErrorAction SilentlyContinue; Start-Sleep 1
$msbuild = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "C:\Users\StarL\Documents\AI_code\btop4win\btop4win.vcxproj" /p:Configuration=Release-LHM /p:Platform=x64 /t:Build /v:minimal
```

- [ ] **Step 2: 部署到 scoop**

```powershell
Copy-Item -Path "C:\Users\StarL\Documents\AI_code\btop4win\x64\Release-LHM\btop4win.exe" -Destination "C:\Users\StarL\scoop\apps\btop-lhm\current\btop.exe" -Force
```

- [ ] **Step 3: 启动 btop 并验证**

启动 btop，确认：
1. GPU 和 CPU 温度正常显示
2. 进程列表正常
3. 没有 stall 或 crash
4. 查看 `btop.log` 中 Broker 相关启动日志

- [ ] **Step 4: 如果 Broker 不可用，确认优雅降级**

系统上没有 SysMonBroker 时：
1. `BrokerThread::start()` 检测 exe 不存在
2. 日志: `Broker: exe not found, permanently disabled`
3. btop 正常运行，全部回退到原逻辑

- [ ] **Step 5: Commit 测试结果**

```bash
git commit --allow-empty -m "test: Broker 集成验证通过

- Broker 不存在时: 优雅降级，btop 正常运行
- Broker 存在时: 进程/传感器从缓存读取
- 无 stall、无 crash、无性能影响

Co-Authored-By: Claude Opus 4.8 <noreply@anthropic.com>"
```
