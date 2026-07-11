# CLAUDE.md — btop4win

## 概述

基于 btop (Linux) 的 Windows 移植版系统监控 TUI 工具。C++ 实现，使用 ADL (AMD Display Library) + LHM (LibreHardwareMonitor) + HWiNFO 共享内存采集硬件数据。可选通过 COM 连接 SysMonBroker（管理员）获取完整进程列表。

## 技术栈

| 层 | 技术 |
|----|------|
| 语言 | C++ (std:c++latest) |
| 构建 | MSBuild / VS Build Tools 2026 |
| UI | 终端 TUI (Windows Console) |
| 数据采集 | ADL PMLOG + LHM + HWiNFO shared memory |
| 进程采集 | SysMonBroker COM (管理员优先) → CreateToolhelp32Snapshot (本地回退) |
| COM 依赖 | ole32.lib + oleaut32.lib (CoCreateInstance + SAFEARRAY) |

## 构建

```powershell
$msbuild = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"

& $msbuild btop4win.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Rebuild /v:minimal
```

### 构建依赖

- **C++ ATL for latest v143 build tools (x86 & x64)** — 提供 `atlstr.h`（`btop_collect.cpp` 依赖 `CString`）
- Release|x64 配置已添加 `/utf-8` 编译选项（解决中文字符串 `lang_zh.cpp` 编码问题）

### Scoop 部署

scoop 包名: `btop-lhm`，安装路径: `scoop\apps\btop-lhm\current\btop.exe`

## SysMonBroker COM 集成 (2026-06-16)

btop4win 可选连接 SysMonBroker COM Local Server，以非管理员身份获取完整进程列表（管理员级 SE_DEBUG 采集）。

### 架构

```
btop4win (非管理员)
    ↓ BrokerClient::connect() → CoCreateInstance(CLSID_BrokerService)
    ↓ 连接失败 → tryLaunchBroker() (ShellExecute "runas")
SysMonBroker (管理员, COM Local Server)
    ↓ ProcessCollector: CreateToolhelp32Snapshot + OpenProcess + SE_DEBUG
    ↓ 2s 缓存, CPU% delta 计算
    ↓ 返回 SAFEARRAY<BrokerProcessEntry>
```

### 文件

| 文件 | 用途 |
|------|------|
| `src/broker_client.hpp` | COM 客户端: GUID 定义、接口声明、BrokerClient 封装类 |
| `src/btop_collect.cpp` | `Proc::collect()` — Broker 优先 + 本地回退 |

### 采集流程 (`Proc::collect()`)

1. 每 30 个采集周期尝试连接 Broker（`s_broker.connect()`）
2. 连接成功 → `broker.getProcesses()` 映射到 `proc_info` 结构
3. 连接失败 / 空结果 → 回退到本地 `CreateToolhelp32Snapshot` + WMI 补充
4. Broker 断连时自动检测（空结果触发 `disconnect()`），下次重试连接

### COM 接口契约

| 接口 | IID | 方法 |
|------|-----|------|
| `IBrokerService` | `...B2C4` | `GetProcessService()`, `GetSensorService()`, `IsAlive()`, `GetVersion()` |
| `IBrokerProcessService` | `...B2C5` | `GetProcesses()`, `GetProcessCount()`, `Refresh()`, `KillProcess()`, `Authenticate(pid, hash)` |
| `IBrokerSensorService` | `...B2C6` | CPU/GPU temp, `GetAllSensors()` |

CLSID: `{7B3F8A1C-9D2E-4F50-B6C7-D8E9F0A1B2C3}` — 修改接口时必须同步更新 C# 和 C++ 两端。

### CPU% 单位转换

Broker 返回 total system CPU% (0-100%)，btop4win 期望 per-core% (0-100% × coreCount)：
- `proc_per_core=true` → 直接使用 Broker 值
- `proc_per_core=false` → `cpu_p / coreCount`

### 构建依赖

x64 配置已在 `.vcxproj` 中添加 `ole32.lib` + `oleaut32.lib`。源码中通过 `#pragma comment(lib, ...)` 也做了声明。

### 进程结束代理 (Kill Proxy)

btop4win 以非管理员运行时，无法结束 SYSTEM/高权限进程。`btop_menu.cpp` 的进程结束逻辑：

1. 先尝试本地 `OpenProcess(PROCESS_TERMINATE)` + `TerminateProcess()`
2. 若失败（ACCESS_DENIED / 无效句柄）→ 调用 `Proc::brokerKillPid(pid)` 代理给 Broker
3. Broker 以管理员 + SE_DEBUG 执行 `Process.Kill()` 或 `TerminateProcess()` 并返回 Win32 错误码

**实现路径**: `btop_menu.cpp` → `Proc::brokerKillPid()` → `BrokerClient::killProcess()` → COM `IBrokerProcessService::KillProcess()` → `BrokerComServer::KillProcess()`

### COM 安全认证 (SHA256 白名单)

Broker 以管理员运行，COM 接口暴露了 `KillProcess()` 和全进程枚举。为防止非授权客户端连接，采用 SHA256 文件哈希白名单机制。

**认证流程** (`BrokerClient::authenticate()`):
1. `GetModuleFileNameW` → 自身 exe 路径
2. CNG `BCryptHashData` → SHA256(btop.exe) → 64 字符 hex
3. COM `Authenticate(pid, hash)` → Broker 通过 PID 打开进程，重算磁盘文件 hash，比对白名单

**集成位置**:
- `Proc::collect()` — 连接后自动认证，认证成功才调用 `getProcesses()`
- `Proc::brokerKillPid()` — 检查 `isAuthenticated()`，未认证返回 -1
- `disconnect()` — 重置 `_authenticated = false`

**首次使用**: 需注册 btop.exe 的 hash：
```powershell
SysMonBroker.exe --register-hash "path\to\btop.exe"
```
白名单为空时信任所有（首次安装体验），注册后强制执行。更新 btop.exe 后需重新注册。

## 关键修复记录

### ADL PMLOG 结构体和传感器 ID（2026-06-12 修复）

**问题：** `src/amd_temp.cpp` 使用了错误的 PMLOG 结构体布局和传感器 ID，存在未定义行为（数组越界读取）。

**修复内容：**

1. **传感器 ID** — 从错误的 500-504 改为官方 SDK 定义：

| 传感器 | 旧值（错误） | 新值（正确） |
|--------|-------------|-------------|
| TEMPERATURE_EDGE | ~~500~~ | 8 |
| TEMPERATURE_VRVDDC | ~~502~~ | 10 |
| TEMPERATURE_HOTSPOT | — | 27 |
| TEMPERATURE_GFX | — | 28 |
| TEMPERATURE_SOC | ~~503~~ | 29 |
| TEMPERATURE_CPU | ~~504~~ | 32 |

2. **结构体布局** — 从两个独立数组改为配对数组：

```cpp
// 旧（错误）：两个独立数组，索引 500+ 越界
struct ADLPMLogDataOutput {
    int size;
    int version;
    int supported[256];   // offset 8
    int values[256];      // offset 1032
};

// 新（正确）：配对数组，total 2052 bytes
struct ADLSingleSensorData {
    int supported;   // offset +0
    int value;       // offset +4
};
struct ADLPMLogDataOutput {
    int size;                                // offset 0, 4 bytes
    ADLSingleSensorData sensors[256];        // offset 4, 256 × 8 = 2048 bytes
};
```

3. **值单位** — ADL PMLOG 温度值直接是摄氏度，不需要 `/ 1000`：

```cpp
// 旧（错误）：假设毫度
if (v > 0 && v < 150000) return v / 1000;

// 新（正确）：直接摄氏度
if (v > 0 && v < 150) return v;
```

**为什么旧代码能"碰巧工作"：**
- C++ `HeapAlloc` / stack 分配器给了比 `sizeof(ADLPMLogDataOutput)` (2056 bytes) 更大的内存块
- 索引 504 越界读到了 ADL 驱动写入的扩展数据区域
- 毫度假设对特定值范围碰巧给出了正确结果
- 这是未定义行为，在不同编译器/优化级别/运行时下可能崩溃

**来源：** 官方 AMD ADL SDK (`adl_defines.h`, `adl_structures.h`) 中 `ADL_PMLOG_SENSORS` 枚举和 `ADLPMLogDataOutput` 定义。
