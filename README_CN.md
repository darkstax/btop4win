# ![btop++](Img/logo.png)

![Windows](https://img.shields.io/badge/-Windows-red?logo=windows)
![Usage](https://img.shields.io/badge/Usage-System%20resource%20monitor-yellow)
![c++20](https://img.shields.io/badge/cpp-c%2B%2B20-green)
![latest_release](https://img.shields.io/github/v/tag/darkstax/btop4win?label=release)

[English](README.md) | **中文**

## 目录

* [新闻](#新闻)
* [文档](#文档)
* [简介](#简介)
* [功能特性](#功能特性)
* [主题](#主题)
* [前置要求](#前置要求) （遇到问题请先阅读此节！）
* [截图](#截图)
* [快捷键](#帮助菜单)
* [安装](#安装)
* [编译](#编译)
* [配置项](#配置项)
* [许可证](#许可证)

## 新闻

##### 2026年6月7日

发布 v1.0.8 (darkstax fork)

开始本地化进程（优先中文），新增 CJK 感知对齐基础设施，修复中文字符截断乱码。

* 新增：中文 (zh) 本地化 — 按钮、内存标签、排序选项、GPU→显存
* 新增：`rjustW`/`ljustW`/`cjustW` CJK 感知对齐函数（基于 `langDisplayWidth()`）
* 修复：UTF-8 CJK 字符截断乱码（substr → uresize）

##### 2026年6月4日

发布 v1.0.7 (darkstax fork)

新增 AMD CPU 温度直读（ADL PMLOG sensor 504），绕过在 AMD APU 上返回错误值的 PawnIO/SMU。

* 新增：AMD ADL PMLOG 直接 CPU 温度读取（sensor 504）— AMD 平台优先使用
* 新增：HWiNFO 共享内存作为通用回退方案
* 新增：CPUID 厂商检测，自动选择温度来源优先级
* 变更：AMD 优先使用 ADL，Intel 优先使用 PawnIO/MSR
* 变更：UAC 级别从 RequireAdministrator 降为 AsInvoker（PawnIO 后端）
* 变更：更新配置/菜单帮助文本，说明 PawnIO 驱动需求（[https://pawnio.eu](https://pawnio.eu)）

##### 2026年6月2日

发布 v1.0.6 (darkstax fork)

温度检测后端从已弃用的 WinRing0 驱动切换至 PawnIO，一次安装驱动后无需管理员权限即可监控 CPU。

* 变更：LibreHardwareMonitor 后端从 WinRing0 迁移至 PawnIO（[https://pawnio.eu](https://pawnio.eu)）
* 变更：不再强制要求管理员权限（PawnIO 作为系统驱动运行）
* 新增：初始 AMD ADL 温度模块

##### 2025年10月12日

发布 v1.0.5

更新 LibreHardwareMonitor 至最新版 (0.9.4)。

##### 2022年8月29日

发布 v1.0.1

从 OpenHardwareMonitor 切换至 LibreHardwareMonitor，使用中介 DLL 直接导出函数到 btop4win。

##### 2022年8月28日

发布 btop4win v1.0.0

##### 2022年3月27日

开始开发 btop4win...

## 文档

**[CHANGELOG.md](CHANGELOG.md)**

**[CONTRIBUTING.md](CONTRIBUTING.md)**

**[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)**

## 简介

显示处理器、内存、磁盘、网络、进程和服务使用情况和统计信息的资源监视器。

[btop++](https://github.com/aristocratos/btop) 的 Windows 版本，btop++ 是 [bashtop](https://github.com/aristocratos/bashtop) 和 [bpytop](https://github.com/aristocratos/bpytop) 的 C++ 版本和延续。

需要 Windows 10 版本 1607（周年更新）才能在标准终端 (CMD) 中使用 ANSI 转义序列。可在 Windows 7 或更高版本上运行，但需要支持 ANSI 转义序列的第三方终端软件。

注意：默认图形符号设置为 TTY，以获得与 Windows CMD 字体的最佳兼容性。如需 braille 和 block 符号，请安装支持这些字符的自定义字体。

推荐字体：[Cascadia Code](https://github.com/microsoft/cascadia-code)（包含 braille 符号）。

推荐终端（Windows 11 已默认包含）：[Windows Terminal](https://github.com/microsoft/terminal)

## 功能特性

* 易于使用，游戏风格的菜单系统
* 完整鼠标支持，所有高亮按键均可点击，进程列表和菜单框支持鼠标滚轮
* 快速响应 UI，UP、DOWN 键选择进程
* 查看选定进程/服务的详细统计信息
* 进程/服务过滤
* 轻松切换排序选项
* 进程树视图
* 终止进程
* 启动/停止/暂停/继续服务
* 设置服务启动类型
* UI 菜单修改所有配置文件选项
* 网络使用量自动缩放图表
* 显示磁盘 IO 活动和速度
* 电池电量显示
* 可选择图表符号
* 自定义预设
* 更多...

## 主题

Btop4win++ 使用与 btop++、bpytop 和 bashtop 相同的主题文件。

参见 [themes](https://github.com/aristocratos/btop/tree/master/themes) 文件夹获取可用主题。

主题文件位于 btop4win.exe 同目录下的 themes 文件夹中。

## 前置要求

为获得最佳体验，终端需支持：

* 24 位真彩色
* 256 色终端通过 24 位到 256 色转换支持（在选项中设置 "truecolor" 为 False 或使用 "-lc/--low-color" 参数）
* 如果检测到真实 tty 设备，将激活 16 色 TTY 模式。可用 "-t/--tty_on" 参数强制启用
* 宽字符（在基于 Web 的终端中可能存在兼容性问题）

还需要 UTF-8 语言环境和覆盖以下字符集的字体：

* Unicode 区块「Braille Patterns」U+2800 - U+28FF（TTY 模式或图表类型设为 block/tty 时不需要）
* Unicode 区块「Geometric Shapes」U+25A0 - U+25FF
* Unicode 区块「Box Drawing」和「Block Elements」U+2500 - U+259F

* 推荐：[新版 Windows Terminal](https://github.com/microsoft/terminal)

### **注意（文本渲染问题）**

* 如果图表中的字符与截图不符，可能是字体缺少所需的特殊字符（如 braille 字符等）。
* 参见 [Cascadia Code](https://github.com/microsoft/cascadia-code) 或 [Terminess Powerline](https://github.com/ryanoasis/nerd-fonts/tree/master/patched-fonts/Terminus/terminus-ttf-4.40.1) 等包含所需符号的字体。

## 截图

#### 主界面 - 显示所选进程详情，字体：[Terminess Powerline](https://github.com/ryanoasis/nerd-fonts/tree/master/patched-fonts/Terminus/terminus-ttf-4.40.1)

![Screenshot 1](Img/normal.png)

#### 主界面 - 显示所选服务详情，字体：[Input Mono Compressed](https://input.djr.com)，主题 OneDark

![Screenshot 2](Img/services.png)

#### 主界面 - TTY 图形符号，CMD Raster 字体

![Screenshot 3](Img/tty.png)

#### 选项菜单

![Screenshot 4](Img/options-menu.png)

#### 帮助菜单

![Screenshot 5](Img/help-menu.png)

## 安装

1. 确保已安装最新 MSVC 运行时库：[https://aka.ms/vs/17/release/vc_redist.x64.exe](https://aka.ms/vs/17/release/vc_redist.x64.exe)

2. 从最新 [release](https://github.com/darkstax/btop4win/releases/latest) 下载所需的安装包

3. 将 btop4win 文件夹解压到任意位置，运行 btop4win.exe。

### PawnIO 驱动安装（必需）

btop4win v1.0.6 起使用 PawnIO 作为温度传感器后端，需要先安装 PawnIO 驱动才能正常读取 CPU 温度：

1. 访问 **[pawnio.eu](https://pawnio.eu)** 下载 PawnIO 安装包
2. 运行安装程序，按照提示完成驱动安装（仅需一次）
3. 安装后无需管理员权限即可运行 btop4win

> 未安装 PawnIO 将导致 CPU 温度显示异常或不可用。

## 编译

1. （可选）编译 LibreHardwareMonitor 支持：

    * 从 [https://github.com/aristocratos/LHM-CppExport](https://github.com/aristocratos/LHM-CppExport) 下载最新 release，将 "external" 文件夹放入 btop4win 顶层目录，或：

    * 1. 克隆 [https://github.com/aristocratos/LHM-CppExport](https://github.com/aristocratos/LHM-CppExport) 并打开解决方案

    * 2. 编译 CPPdll 的 Release x64 配置

    * 3. 将 "x64/Release" 中的所有 .dll 和 .lib 文件复制到 btop4win 顶层的 "external" 文件夹

2. btop4win

    * 1. 克隆仓库或从最新 release 下载源码包。

    * 2. 使用 Visual Studio 2022 打开解决方案。

    * 3. 选择 Debug、Release 或 Release-LHM 配置。

    * * （Release-LHM 会在二进制文件上设置管理员权限要求、LHM_Enabled 声明，并需要 "external" 文件夹中的 LHM-CppExport dll 和 .lib）。

    * 4. 编译解决方案。

## 配置项

所有选项均可在 UI 中修改。配置文件和日志文件存储在 btop4win.exe 所在目录。

#### btop.conf：（如未找到则自动生成）

```bash
#? Config file for btop4win v. 1.0.0

#* btop++/bpytop/bashtop 格式的 ".theme" 主题文件名，"Default" 和 "TTY" 为内置主题。
#* 主题应放在 btop4win.exe 同目录下的 "themes" 文件夹中。
color_theme = "Default"

#* 是否显示主题设置的背景色，如需终端背景透明则设为 False。
theme_background = True

#* 设置是否使用 24 位真彩色，设为 false 时将转换为 256 色（6x6x6 色立方）。
truecolor = True

#* 强制开启 tty 模式（无论是否检测到真实 tty）。
#* 将强制使用 16 色模式和 TTY 主题，所有图表符号设为 "tty" 并替换其他不兼容的符号。
force_tty = False

#* 定义盒子的布局预设。预设 0 始终显示所有盒子（默认设置）。最多 9 个预设。
#* 格式："box_name:P:G,box_name:P:G" P=(0 或 1) 为替代位置，G=盒子使用的图表符号。
#* 不同预设之间用空格 " " 分隔。
#* 示例："cpu:0:default,mem:0:tty,proc:1:default cpu:0:braille,proc:0:tty"
presets = "cpu:1:default,proc:0:default cpu:0:default,mem:0:default,net:0:default cpu:0:block,net:0:tty"

#* 设为 True 启用 "h,j,k,l,g,G" 键在列表中进行方向控制。
#* 冲突的按键 h:"help" 和 k:"kill" 可按住 shift 使用。
vim_keys = False

#* 通过 LibreHardwareMonitor (PawnIO 后端) 启用 CPU 温度监控、精确 CPU 频率和 GPU 监控。
#* 需要 LHM-CppExport 的 DLL 文件位于 btop4win.exe 同目录。
#* PawnIO 驱动: https://pawnio.eu 安装后无需管理员权限。
enable_ohmr = True

#* 在 CPU 和内存盒子中同时显示 GPU 统计信息。需要启用 Libre Hardware Monitor Report。
show_gpu = True

#* 检测到多个 GPU 时选择显示哪一个。
selected_gpu = "Auto"

#* 盒子圆角，TTY 模式开启时忽略此设置。
rounded_corners = False

#* 图表创建的默认符号，"braille"、"block" 或 "tty"。
#* "braille" 分辨率最高，但可能不是所有字体都包含。
#* "block" 分辨率为 braille 的一半，但使用更常见的字符。
#* "tty" 仅使用 3 种不同符号，适用于大多数字体和真实 TTY。
#* 注意 "tty" 的水平分辨率仅为其他两种的一半，历史视图较短。
graph_symbol = "tty"

# CPU 盒子图表的图表符号，"default"、"braille"、"block" 或 "tty"。
graph_symbol_cpu = "default"

# 内存盒子图表的图表符号，"default"、"braille"、"block" 或 "tty"。
graph_symbol_mem = "default"

# 网络盒子图表的图表符号，"default"、"braille"、"block" 或 "tty"。
graph_symbol_net = "default"

# 进程盒子图表的图表符号，"default"、"braille"、"block" 或 "tty"。
graph_symbol_proc = "default"

#* 手动设置显示哪些盒子。可用值："cpu mem net proc"，用空格分隔。
shown_boxes = "cpu mem net proc"

#* 更新间隔（毫秒），建议 2000 ms 以上以获得更好的图表采样时间。
update_ms = 1500

#* 进程排序，"pid" "program" "arguments" "threads" "user" "memory" "cpu lazy" "cpu direct"
#* "cpu lazy" 随时间排序（更易跟踪），"cpu direct" 直接更新顶部进程。
proc_sorting = "cpu lazy"

#* 在进程盒子中显示服务而非进程。
proc_services = False

#* 服务排序，"service" "caption" "status" "memory" "cpu lazy" "cpu direct"
services_sorting = "cpu lazy"

#* 反转排序顺序，True 或 False。
proc_reversed = False

#* 以树形结构显示进程。
proc_tree = False

#* 在进程列表中使用 CPU 图表颜色。
proc_colors = True

#* 在进程列表中使用渐变暗化效果。
proc_gradient = True

#* 进程 CPU 使用率按所在核心计算还是按总可用 CPU 计算。
proc_per_core = False

#* 进程内存以字节显示而非百分比。
proc_mem_bytes = True

#* 将进程盒子显示在屏幕左侧而非右侧。
proc_left = False

#* 设置 CPU 图表上半部分显示的统计项，"total" 始终可用。
#* 从选项菜单中检测到的属性列表中选择。
cpu_graph_upper = "total"

#* 设置 CPU 图表下半部分显示的统计项，"total" 始终可用。
#* 从选项菜单中检测到的属性列表中选择。
cpu_graph_lower = "gpu"

#* 切换是否反转下方 CPU 图表。
cpu_invert_lower = True

#* 设为 True 完全禁用下方 CPU 图表。
cpu_single_graph = False

#* 将 CPU 盒子显示在屏幕底部而非顶部。
cpu_bottom = False

#* 显示全宽 CPU 盒子。
cpu_wide = True

#* 在 CPU 盒子中显示系统运行时间。
show_uptime = True

#* 显示 CPU 温度。
check_temp = True

#* 用于 CPU 温度的传感器，从选项菜单的可用传感器列表中选择。
cpu_sensor = 

#* 如果 check_temp 为 True 且找到传感器，同时显示各核心温度。
show_coretemp = True

#* 温度单位，可用值："celsius"、"fahrenheit"、"kelvin" 和 "rankine"。
temp_scale = "celsius"

#* 使用十进制计数，KB = 1000 而非 KiB = 1024。
base_10_sizes = False

#* 在屏幕顶部绘制时钟，格式遵循 strftime，留空禁用。
#* 特殊格式：/host = 主机名 | /user = 用户名 | /uptime = 系统运行时间
clock_format = "%X"

#* 菜单显示时继续在后台更新主界面，如菜单闪烁过重可设为 false。
background_update = True

#* 自定义 CPU 型号名称，留空禁用。
custom_cpu_name = ""

#* 可选磁盘过滤器，应为挂载点的完整路径，多个值用空格 " " 分隔。
#* 行首加 "exclude=" 切换为排除过滤器，否则默认为"包含"过滤器。
#* 示例：disks_filter="exclude=D:\ E:\"
disks_filter = ""

#* 内存值显示图表而非仪表。
mem_graphs = True

#* 将内存盒子显示在网络盒子下方而非上方。
mem_below_net = False

#* 是否在内存盒子中显示页面内存。
show_page = True

#* 是否拆分内存盒子同时显示磁盘信息。
show_disks = True

#* 过滤非物理磁盘。设为 False 以包含网络磁盘、RAM 磁盘等。
only_physical = True

#* 设为 true 为特权用户显示可用磁盘空间。
disk_free_priv = False

#* 切换是否在常规磁盘使用视图中显示 IO 活动百分比（磁盘忙碌时间）。
show_io_stat = True

#* 切换磁盘 IO 模式，显示磁盘读写速度的大图表。
io_mode = False

#* 设为 True 在 IO 模式下显示合并的读写图表。
io_graph_combined = False

#* 设置 IO 图表的最大速度（MiB/s，默认 100），格式 "mountpoint:speed"，不同磁盘用空格 " " 分隔。
#* 示例："/mnt/media:100 /:20 /boot:1"
io_graph_speeds = ""

#* 设置网络图表的固定值（Mebibits）。仅在 net_auto 也设为 False 时使用。
net_download = 100

net_upload = 100

#* 使用网络图表自动缩放模式，忽略上述设定值，最低缩放到 10 Kibibytes。
net_auto = True

#* 同步下载和上传的自动缩放，以当前较高者为准。
net_sync = False

#* 启动时使用此处指定的网络接口。
net_iface = ""

#* 如检测到电池，在右上角显示电池状态。
show_battery = True

#* 设置日志级别 "~/.config/btop/btop.log"，可用级别："ERROR" "WARNING" "INFO" "DEBUG"。
#* 设置的级别包含所有更低级别，即 "DEBUG" 将显示所有日志信息。
log_level = "WARNING"
```

#### 命令行选项

```text
用法: btop4win.exe [-h] [-v] [-/+t] [-p <id>] [--debug]

可选参数:
  -h, --help            显示此帮助信息并退出
  -v, --version         显示版本信息并退出
  -lc, --low-color      禁用真彩色，将 24 位颜色转换为 256 色
  -t, --tty_on          强制开启 tty 模式，最多 16 色和 tty 友好图表符号
  +t, --tty_off         强制关闭 tty 模式
  -p, --preset <id>     以指定预设启动，整数值 0-9
  --debug               以 DEBUG 模式启动：显示信息收集和屏幕绘制的微秒计时器，日志级别设为 DEBUG
```

## 许可证

[Apache License 2.0](LICENSE)
