/* Chinese (Simplified) language strings for btop4win */

#include "lang.hpp"

// ============================================================================
// Help text (key-description pairs) - matches help_text in btop_menu.cpp
// ============================================================================
static const Lang::HelpEntry zh_help[] = {
	{"Mouse 1",          "点击按钮和选择进程列表。"},
	{"Mouse scroll",     "滚动光标处的列表/文本。"},
	{"Esc, m",           "切换主菜单。"},
	{"p",                "向前切换视图预设。"},
	{"shift + p",        "向后切换视图预设。"},
	{"1",                "切换 CPU 框。"},
	{"2",                "切换内存框。"},
	{"3",                "切换网络框。"},
	{"4",                "切换进程框。"},
	{"d",                "在内存框中切换磁盘视图。"},
	{"F2, o",            "显示选项。"},
	{"F1, h",            "显示此帮助窗口。"},
	{"q, ctrl + c",      "退出程序。"},
	{"+, -",             "增加/减少 100ms 更新间隔。"},
	{"Up, Down",         "在进程列表中选择。"},
	{"Enter",            "显示所选进程的详细信息。"},
	{"Spacebar",         "在树形视图中展开/折叠进程。"},
	{"Pg Up, Pg Down",   "在进程列表中翻页。"},
	{"Home, End",        "跳转到进程列表首页/末页。"},
	{"Left, Right",      "选择上一/下一排序列。"},
	{"b, n",             "选择上一/下一网络设备。"},
	{"i",                "切换磁盘 IO 大图模式。"},
	{"z",                "切换当前网络设备的总计归零。"},
	{"a",                "切换网络图表自动缩放。"},
	{"y",                "切换网络图表同步缩放。"},
	{"f, /",             "输入进程筛选器。"},
	{"delete",           "清除已输入的筛选器。"},
	{"c",                "切换进程的每核 CPU 使用率。"},
	{"r",                "反转进程框排序顺序。"},
	{"e",                "切换进程树形视图。"},
	{"s",                "切换服务/进程。"},
	{"Selected +, -",    "在树形视图中展开/折叠进程。"},
	{"Selected t",       "终止所选进程"},
	{"", " "},
	{"",                 "报告 Bug 或获取项目更新，请访问："},
	{"",                 "https://github.com/aristocratos/btop4win"},
};

// ============================================================================
// Category option descriptions
// Each entry: pointer to array of const char* where [0]=key, [1..n]=description lines
// ============================================================================

// ---- General category ----
static const char* zh_general_0[] = {
	"color_theme",
	"设置颜色主题。",
	"",
	"从主题文件中选择（通常在）",
	"\"/usr/[local/]share/btop/themes\" 和",
	"\"~/.config/btop/themes\"。",
	"",
	"\"Default\" 为内置默认主题。",
	"\"TTY\" 为内置 16 色主题。",
	"",
	"主题更新请见：",
	"https://github.com/aristocratos/btop",
	nullptr
};
static const char* zh_general_1[] = {
	"theme_background",
	"是否显示主题背景。",
	"",
	"如需终端背景透明，",
	"请设为 False。",
	nullptr
};
static const char* zh_general_2[] = {
	"truecolor",
	"是否使用 24 位真彩色。",
	"",
	"设为 False 时将 24 位色转为 256 色",
	"(6x6x6 色立方体)。",
	"",
	"若终端不支持真彩色",
	"且无法自动转换 256 色，",
	"请设为 False。",
	nullptr
};
static const char* zh_general_3[] = {
	"force_tty",
	"TTY 模式。",
	"",
	"设为 true 强制启用 tty 模式，",
	"不论是否检测到真正的 tty。",
	"",
	"将强制 16 色模式和 TTY 主题，",
	"所有图表符号切换为 \"tty\"，",
	"并替换其他不兼容 tty 的符号。",
	nullptr
};
static const char* zh_general_4[] = {
	"vim_keys",
	"启用 vim 键位。",
	"设为 True 启用 \"h,j,k,l\" 键",
	"在列表中进行方向控制。",
	"",
	"冲突的 h (帮助) 和 k (终止)",
	"键可通过按住 shift 访问。",
	nullptr
};
static const char* zh_general_5[] = {
	"enable_ohmr",
	"启用 Libre Hardware Monitor Report。",
	"",
	"通过 PawnIO 后端监测 CPU 温度、CPU 频率",
	"和 GPU。",
	"需要 PawnIO 驱动 (https://pawnio.eu)，",
	"以及程序目录下的 LHM-CppExport DLL。",
	"https://github.com/aristocratos/LHM-CppExport",
	"安装驱动后无需管理员权限。",
	"",
	"修改此选项需要重启",
	"才能生效。",
	nullptr
};
static const char* zh_general_6[] = {
	"show_gpu",
	"在 CPU 和内存框中同时显示 GPU 状态。",
	"",
	"仅在启用 Libre Hardware Monitor 时有效。",
	nullptr
};
static const char* zh_general_7[] = {
	"selected_gpu",
	"多 GPU 时选择显示哪一个 GPU。",
	"",
	"从检测到的设备列表中选择 GPU。",
	"默认 \"Auto\" 为第一个",
	"检测到的设备。",
	nullptr
};
static const char* zh_general_8[] = {
	"gpu_mem_override",
	"手动设置 GPU 总显存。",
	"",
	"用于检测不正确时。",
	"单位 MiB。",
	"示例: \"1024\" 表示 1 GiB。",
	nullptr
};
static const char* zh_general_9[] = {
	"presets",
	"定义布局预设。",
	"",
	"预设 0 始终显示所有框，",
	"使用默认设置。",
	"最多 9 个预设。",
	"",
	"格式: \"box_name:P:G,box_name:P:G\"",
	"P=(0 或 1) 备选位置。",
	"G=图表符号类型。",
	"",
	"用空格 \" \" 分隔",
	"不同预设。",
	"",
	"示例:",
	"\"mem:0:tty,proc:1:default cpu:0:braille\"",
	nullptr
};
static const char* zh_general_10[] = {
	"shown_boxes",
	"手动设置显示哪些框。",
	"",
	"可用值: \"cpu mem net proc\"。",
	"用空格分隔。",
	"",
	"按 \"p\" 切换预设。",
	nullptr
};
static const char* zh_general_11[] = {
	"update_ms",
	"更新间隔（毫秒）。",
	"",
	"建议 2000ms 或以上",
	"以获得更好的图表采样。",
	"",
	"最小值: 100 ms",
	"最大值: 86400000 ms = 24 小时。",
	nullptr
};
static const char* zh_general_12[] = {
	"rounded_corners",
	"框的圆角。",
	"",
	"True 或 False",
	"",
	"TTY 模式开启时始终为 False。",
	nullptr
};
static const char* zh_general_13[] = {
	"graph_symbol",
	"默认图表符号类型。",
	"",
	"\"braille\"、\"block\" 或 \"tty\"。",
	"",
	"\"braille\" 分辨率最高，但",
	"可能未内置于所有字体中。",
	"",
	"\"block\" 分辨率为 braille 的一半",
	"但使用更常见的字符。",
	"",
	"\"tty\" 仅用 3 种符号，但能在",
	"大多数字体中使用。",
	"",
	"注意: \"tty\" 水平分辨率仅为",
	"其他两种的一半，",
	"显示的历史视图更短。",
	nullptr
};
static const char* zh_general_14[] = {
	"clock_format",
	"在屏幕顶部画时钟。",
	"(仅 CPU 框启用时可见!)",
	"",
	"按 strftime 格式化，空字符串",
	"禁用。",
	"",
	"自定义格式:",
	"\"/host\" = " "主机名",
	"\"/user\" = " "用户名",
	"\"/uptime\" = " "系统运行时间",
	"",
	"strftime 格式示例:",
	"\"%X\" = " "本地 HH:MM:SS",
	"\"%H\" = 24h " "时, \"%I\" = 12h " "时",
	"\"%M\" = " "分, \"%S\" = " "秒",
	"\"%d\" = " "日, \"%m\" = " "月, \"%y\" = " "年",
	nullptr
};
static const char* zh_general_15[] = {
	"base_10_sizes",
	"使用十进制计算比特/字节大小。",
	"",
	"使用 KB = 1000 而非 KiB = 1024，",
	"MB = 1000KB 而非 MiB = 1024KiB，",
	"依此类推。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_general_16[] = {
	"background_update",
	"菜单显示时更新主界面。",
	"",
	"True 或 False。",
	"",
	"如果菜单闪烁过严重，",
	"请设为 false。",
	nullptr
};
static const char* zh_general_17[] = {
	"show_battery",
	"显示电池状态。",
	"(仅 CPU 框启用时可见!)",
	"",
	"如有电池，在右上角",
	"显示电池状态。",
	nullptr
};
static const char* zh_general_18[] = {
	"log_level",
	"设置 error.log 的日志级别。",
	"",
	"\"ERROR\"、\"WARNING\"、\"INFO\" 和 \"DEBUG\"。",
	"",
	"设置的级别包含所有低级别，",
	"即 \"DEBUG\" 将显示所有日志信息。",
	nullptr
};
static const char* zh_general_19[] = {
	"lang",
	"语言。",
	"",
	"\"en\" 为英文。",
	"\"zh\" 为简体中文。",
	nullptr
};

static const Lang::OptionDesc zh_general_options[] = {
	{zh_general_0[0],  zh_general_0+1,  11},
	{zh_general_1[0],  zh_general_1+1,  3},
	{zh_general_2[0],  zh_general_2+1,  7},
	{zh_general_3[0],  zh_general_3+1,  7},
	{zh_general_4[0],  zh_general_4+1,  5},
	{zh_general_5[0],  zh_general_5+1,  11},
	{zh_general_6[0],  zh_general_6+1,  2},
	{zh_general_7[0],  zh_general_7+1,  4},
	{zh_general_8[0],  zh_general_8+1,  4},
	{zh_general_9[0],  zh_general_9+1,  15},
	{zh_general_10[0], zh_general_10+1, 5},
	{zh_general_11[0], zh_general_11+1, 6},
	{zh_general_12[0], zh_general_12+1, 4},
	{zh_general_13[0], zh_general_13+1, 14},
	{zh_general_14[0], zh_general_14+1, 16},
	{zh_general_15[0], zh_general_15+1, 6},
	{zh_general_16[0], zh_general_16+1, 6},
	{zh_general_17[0], zh_general_17+1, 5},
	{zh_general_18[0], zh_general_18+1, 5},
	{zh_general_19[0], zh_general_19+1, 4},
};

// ---- CPU category ----
static const char* zh_cpu_0[] = {
	"cpu_bottom",
	"CPU 框位置。",
	"",
	"在屏幕底部而非顶部",
	"显示 CPU 框。",
	nullptr
};
static const char* zh_cpu_1[] = {
	"cpu_wide",
	"CPU 框宽度。",
	"",
	"显示全宽 CPU 框。",
	nullptr
};
static const char* zh_cpu_2[] = {
	"graph_symbol_cpu",
	"CPU 框图表符号类型。",
	"",
	"\"default\"、\"braille\"、\"block\" 或 \"tty\"。",
	"",
	"\"default\" 为全局默认符号。",
	nullptr
};
static const char* zh_cpu_3[] = {
	"cpu_graph_upper",
	"CPU 上半部图表。",
	"",
	"设置 CPU 图表上半部分",
	"显示的 CPU 状态。",
	"",
	"\"total\" = 总 CPU 使用率。",
	"\"user\" = 用户模式 CPU 使用率。",
	"\"system\" = 内核模式 CPU 使用率。",
	"+ 更多取决于内核。",
	nullptr
};
static const char* zh_cpu_4[] = {
	"cpu_graph_lower",
	"CPU 下半部图表。",
	"",
	"设置 CPU 图表下半部分",
	"显示的 CPU 状态。",
	"",
	"\"total\" = 总 CPU 使用率。",
	"\"user\" = 用户模式 CPU 使用率。",
	"\"system\" = 内核模式 CPU 使用率。",
	"+ 更多取决于内核。",
	nullptr
};
static const char* zh_cpu_5[] = {
	"cpu_invert_lower",
	"翻转 CPU 下半部图表方向。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_cpu_6[] = {
	"cpu_single_graph",
	"完全禁用 CPU 下半部图表。",
	"",
	"仅显示上半部分 CPU 图表",
	"并自动调整高度适配。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_cpu_7[] = {
	"check_temp",
	"显示 CPU 温度。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_cpu_8[] = {
	"show_coretemp",
	"显示 CPU 核心温度。",
	"",
	"仅在 check_temp 为 True 且",
	"系统报告核心温度时有效。",
	nullptr
};
static const char* zh_cpu_9[] = {
	"temp_scale",
	"温度单位。",
	"",
	"摄氏度 (celsius)，默认。",
	"",
	"华氏度 (fahrenheit)。",
	"",
	"开尔文 (kelvin)，0 = 绝对零度，",
	"1 度变化等于摄氏 1 度。",
	"",
	"兰氏度 (rankine)，0 = 绝对零度，",
	"1 度变化等于华氏 1 度。",
	nullptr
};
static const char* zh_cpu_10[] = {
	"custom_cpu_name",
	"自定义 CPU 名称（百分比框）。",
	"",
	"空字符串禁用。",
	nullptr
};
static const char* zh_cpu_11[] = {
	"show_uptime",
	"在 CPU 框中显示系统运行时间。",
	"",
	"也可在时钟中使用",
	"\"/uptime\" 显示。",
	"",
	"True 或 False。",
	nullptr
};

static const Lang::OptionDesc zh_cpu_options[] = {
	{zh_cpu_0[0],  zh_cpu_0+1,  3},
	{zh_cpu_1[0],  zh_cpu_1+1,  2},
	{zh_cpu_2[0],  zh_cpu_2+1,  4},
	{zh_cpu_3[0],  zh_cpu_3+1,  8},
	{zh_cpu_4[0],  zh_cpu_4+1,  8},
	{zh_cpu_5[0],  zh_cpu_5+1,  2},
	{zh_cpu_6[0],  zh_cpu_6+1,  4},
	{zh_cpu_7[0],  zh_cpu_7+1,  2},
	{zh_cpu_8[0],  zh_cpu_8+1,  3},
	{zh_cpu_9[0],  zh_cpu_9+1,  8},
	{zh_cpu_10[0], zh_cpu_10+1, 2},
	{zh_cpu_11[0], zh_cpu_11+1, 4},
};

// ---- Memory category ----
static const char* zh_mem_0[] = {
	"mem_below_net",
	"内存框位置。",
	"",
	"在网络框下方而非上方",
	"显示内存框。",
	nullptr
};
static const char* zh_mem_1[] = {
	"graph_symbol_mem",
	"内存框图表符号类型。",
	"",
	"\"default\"、\"braille\"、\"block\" 或 \"tty\"。",
	"",
	"\"default\" 为全局默认符号。",
	nullptr
};
static const char* zh_mem_2[] = {
	"mem_graphs",
	"显示内存图表而非仪表。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_3[] = {
	"show_disks",
	"拆分内存框同时显示磁盘。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_4[] = {
	"show_io_stat",
	"显示 IO 活动图表。",
	"",
	"在非 IO 模式时显示",
	"磁盘活动 (IO%) 的小图表。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_5[] = {
	"io_mode",
	"切换磁盘 IO 模式。",
	"",
	"显示磁盘读写速度的大图表",
	"而非已用/可用百分比仪表。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_6[] = {
	"io_graph_combined",
	"合并读写 IO 图表。",
	"",
	"仅在 \"io mode\" 为 True 时有效。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_7[] = {
	"io_graph_speeds",
	"设置 IO 图表的最高速度。",
	"",
	"手动设置 MiB/s 的最高速度",
	"等于 IO 图表的 100%。",
	"(默认 100 MiB/s)。",
	"",
	"格式: \"设备:\\速度\" 用空格 \" \" 分隔。",
	"",
	"示例: \"C:\\100, D:\\20\"。",
	nullptr
};
static const char* zh_mem_8[] = {
	"show_page",
	"在内存框中显示页面内存。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_9[] = {
	"only_physical",
	"过滤非物理磁盘。",
	"",
	"设为 False 可包含网络磁盘、",
	"RAM 磁盘等。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_mem_10[] = {
	"disk_free_priv",
	"可用磁盘空间类型。",
	"",
	"设为 true 显示管理员可用空间。",
	"",
	"设为 false 显示普通用户可用空间。",
	nullptr
};
static const char* zh_mem_11[] = {
	"disks_filter",
	"磁盘显示筛选器（可选）。",
	"",
	"应为挂载点的完整路径。",
	"多个值用空格 \" \" 分隔。",
	"",
	"行首加 \"exclude=\" 切换为",
	"排除筛选器。",
	"否则默认为包含筛选器。",
	"",
	"示例:",
	"\"exclude=D:\\ E:\\\"",
	nullptr
};

static const Lang::OptionDesc zh_mem_options[] = {
	{zh_mem_0[0],  zh_mem_0+1,  3},
	{zh_mem_1[0],  zh_mem_1+1,  4},
	{zh_mem_2[0],  zh_mem_2+1,  2},
	{zh_mem_3[0],  zh_mem_3+1,  2},
	{zh_mem_4[0],  zh_mem_4+1,  4},
	{zh_mem_5[0],  zh_mem_5+1,  4},
	{zh_mem_6[0],  zh_mem_6+1,  3},
	{zh_mem_7[0],  zh_mem_7+1,  6},
	{zh_mem_8[0],  zh_mem_8+1,  2},
	{zh_mem_9[0],  zh_mem_9+1,  4},
	{zh_mem_10[0], zh_mem_10+1, 3},
	{zh_mem_11[0], zh_mem_11+1, 8},
};

// ---- Network category ----
static const char* zh_net_0[] = {
	"graph_symbol_net",
	"网络框图表符号类型。",
	"",
	"\"default\"、\"braille\"、\"block\" 或 \"tty\"。",
	"",
	"\"default\" 为全局默认符号。",
	nullptr
};
static const char* zh_net_1[] = {
	"net_download",
	"网络下载图表固定值。",
	"",
	"单位 Mebibits，默认 \"100\"。",
	"",
	"可通过 auto 按钮切换。",
	nullptr
};
static const char* zh_net_2[] = {
	"net_upload",
	"网络上传图表固定值。",
	"",
	"单位 Mebibits，默认 \"100\"。",
	"",
	"可通过 auto 按钮切换。",
	nullptr
};
static const char* zh_net_3[] = {
	"net_auto",
	"启动时网络图表自动缩放。",
	"",
	"启动时忽略上述设定值",
	"并自动缩放至最低 10KiB。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_net_4[] = {
	"net_sync",
	"网络缩放同步。",
	"",
	"同步下载和上传的缩放比例",
	"到当前最高的比例。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_net_5[] = {
	"net_iface",
	"网络接口。",
	"",
	"手动设置初始网络接口。",
	"",
	"否则自动选择启动以来",
	"总下载量最大的网卡。",
	nullptr
};

static const Lang::OptionDesc zh_net_options[] = {
	{zh_net_0[0], zh_net_0+1, 4},
	{zh_net_1[0], zh_net_1+1, 4},
	{zh_net_2[0], zh_net_2+1, 4},
	{zh_net_3[0], zh_net_3+1, 4},
	{zh_net_4[0], zh_net_4+1, 4},
	{zh_net_5[0], zh_net_5+1, 4},
};

// ---- Process category ----
static const char* zh_proc_0[] = {
	"proc_left",
	"进程框位置。",
	"",
	"在屏幕左侧而非右侧",
	"显示进程框。",
	nullptr
};
static const char* zh_proc_1[] = {
	"graph_symbol_proc",
	"进程框图表符号类型。",
	"",
	"\"default\"、\"braille\"、\"block\" 或 \"tty\"。",
	"",
	"\"default\" 为全局默认符号。",
	nullptr
};
static const char* zh_proc_2[] = {
	"proc_sorting",
	"进程排序选项。",
	"",
	"可用值:",
	"\"pid\"、\"program\"、\"arguments\"、\"threads\"、",
	"\"user\"、\"memory\"、\"cpu lazy\" 和",
	"\"cpu direct\"。",
	"",
	"\"cpu lazy\" 逐步更新顶部进程。",
	"\"cpu direct\" 直接更新顶部进程。",
	nullptr
};
static const char* zh_proc_3[] = {
	"proc_services",
	"显示服务。",
	"",
	"在进程框中显示服务",
	"而非进程。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_proc_4[] = {
	"services_sorting",
	"服务排序选项。",
	"",
	"可用值:",
	"\"service\"、\"caption\"、\"status\"、\"memory\"、",
	"\"cpu lazy\" 和 \"cpu direct\"。",
	"",
	"\"cpu lazy\" 逐步更新顶部服务。",
	"\"cpu direct\" 直接更新顶部服务。",
	nullptr
};
static const char* zh_proc_5[] = {
	"proc_reversed",
	"反转进程排序顺序。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_proc_6[] = {
	"proc_tree",
	"进程树形视图。",
	"",
	"设为 true 按父子关系分组显示进程，",
	"并在父子进程间画连接线。",
	nullptr
};
static const char* zh_proc_7[] = {
	"proc_colors",
	"启用进程视图颜色。",
	"",
	"True 或 False。",
	nullptr
};
static const char* zh_proc_8[] = {
	"proc_gradient",
	"启用进程视图渐变。",
	"",
	"从顶部或当前选中项渐变。",
	"最大渐变值等于当前主题",
	"的 \"inactive_fg\" 颜色值。",
	nullptr
};
static const char* zh_proc_9[] = {
	"proc_per_core",
	"进程每核 CPU 使用率。",
	"",
	"显示进程在其运行核心上的 CPU 使用率",
	"还是总 CPU 功率的使用率。",
	"",
	"设为 true 时，多线程进程",
	"CPU 使用率可能超过 100%。",
	nullptr
};
static const char* zh_proc_10[] = {
	"proc_mem_bytes",
	"在进程列表中以字节显示内存。",
	" ",
	"False 时显示总内存百分比。",
	nullptr
};

static const Lang::OptionDesc zh_proc_options[] = {
	{zh_proc_0[0],  zh_proc_0+1,  3},
	{zh_proc_1[0],  zh_proc_1+1,  4},
	{zh_proc_2[0],  zh_proc_2+1,  8},
	{zh_proc_3[0],  zh_proc_3+1,  4},
	{zh_proc_4[0],  zh_proc_4+1,  8},
	{zh_proc_5[0],  zh_proc_5+1,  2},
	{zh_proc_6[0],  zh_proc_6+1,  3},
	{zh_proc_7[0],  zh_proc_7+1,  2},
	{zh_proc_8[0],  zh_proc_8+1,  4},
	{zh_proc_9[0],  zh_proc_9+1,  5},
	{zh_proc_10[0], zh_proc_10+1, 2},
};

// ============================================================================
// Main LANG_ZH definition
// ============================================================================
const Lang LANG_ZH = {
	// ---- Help text ----
	.help           = zh_help,
	.help_count     = sizeof(zh_help) / sizeof(zh_help[0]),

	// ---- Category option descriptions ----
	.cats = {
		{ zh_general_options, sizeof(zh_general_options) / sizeof(zh_general_options[0]) },
		{ zh_cpu_options,     sizeof(zh_cpu_options)     / sizeof(zh_cpu_options[0]) },
		{ zh_mem_options,     sizeof(zh_mem_options)     / sizeof(zh_mem_options[0]) },
		{ zh_net_options,     sizeof(zh_net_options)     / sizeof(zh_net_options[0]) },
		{ zh_proc_options,    sizeof(zh_proc_options)    / sizeof(zh_proc_options[0]) },
	},

	// ---- Category tab names ----
	.cat_general    = "常规",
	.cat_cpu        = "处理器",
	.cat_mem        = "内存",
	.cat_net        = "网络",
	.cat_proc       = "进程",

	// ---- Help menu headers ----
	.help_key       = "按键:",
	.help_desc      = "说明:",

	// ---- Buttons ----
	.btn_ok         = "    确定    ",
	.btn_yes        = "    是    ",
	.btn_no         = "    否    ",

	// ---- Box titles ----
	.box_cpu        = "处理器",
	.box_mem        = "内存",
	.box_net        = "网络",
	.box_proc       = "进程",
	.box_download   = "下载",
	.box_upload     = "上传",

	// ---- CPU box ----
	.cpu_label      = "CPU ",
	.gpu_label      = "GPU ",
	.up_prefix      = "运行",
	.load_avg       = "负载:",
	.lav            = "LAV:",
	.bat            = "BAT",
	.charging       = "充电中",
	.discharging    = "放电中",
	.full           = "已满",
	.unknown        = "未知",

	// ---- Memory box ----
	.mem_total      = "总计:",
	.mem_used       = "已用",
	.mem_available  = "可用",
	.mem_cached     = "缓存",
	.mem_committed  = "提交",
	.mem_free       = " 可用:",
	.mem_free_short = "F",
	.mem_used_short = "U",
	.mem_pagevirt   = "可提交",
	.mem_pagefiles  = "页面文件:",
	.mem_gpu        = "显存",
	.mem_gpu_shared = " 共享",
	.mem_io_pct     = " IO% ",
	.mem_io         = " IO   ",
	.mem_rw         = "RW",
	.mem_r          = "R",
	.mem_w          = "W",
	.btn_disks      = "磁盘",
	.btn_io         = "io",

	// ---- Network box ----
	.net_top        = "峰值: ",
	.net_total      = "总计: ",
	.btn_zero       = "归零",
	.btn_auto       = "自动",
	.btn_sync       = "同步",

	// ---- Process box ----
	.proc_pid       = "PID:",
	.proc_program   = "程序:",
	.proc_command   = "命令:",
	.proc_tree      = "树形:",
	.proc_threads   = "线程: ",
	.proc_user      = "用户:",
	.proc_status    = "状态:",
	.proc_memb      = "内存B",
	.proc_mem_pct   = "内存%",
	.proc_cpu_pct   = "CPU%",
	.proc_service   = "服务:",
	.proc_caption   = "标题:",
	.proc_parent    = "父进程:",
	.proc_owner     = "所有者:",
	.proc_start     = "启动:",
	.proc_elapsed   = "已运行:",
	.proc_ior       = "IO/R:",
	.proc_iow       = "IO/W:",
	.proc_memory    = "内存: ",
	.proc_memory_short = "M:",

	// Process buttons (padding preserves original button widths)
	.btn_terminate  = "终止     ",   // "terminate" = 9 cols
	.btn_stop       = "停止  ",       // "stop" = 4 cols
	.btn_start      = "启动   ",      // "start" = 5 cols
	.btn_pause      = "暂停    ",     // "pause" = 5 cols, hotkey 'u' after 4 cols
	.btn_continue   = "继续    ",     // "continue" = 8 cols
	.btn_starttype  = "启动类型",     // "Start-type" in detailed box (10 col area)
	.btn_hide       = "隐藏 ",        // "hide " = 5 cols
	.btn_select     = "  选择  ",     // " select " = 8 cols
	.btn_info       = "信息  ",       // "info " = 5 cols
	.btn_filter     = "筛选 ",        // "ilter" = 5 cols (hotkey 'f' + text)
	.btn_filter_del = " 删除",        // " del" = 4 cols
	.btn_percore    = "每核 ",        // "ore" around hotkey 'c' -> 4 cols with space
	.btn_reverse    = "反转 ",        // "everse" = 6 cols -> 3 CJK + space
	.btn_tree       = "树形",         // "tre" + hotkey 'e' -> 4 cols (2 CJK)
	.btn_services   = "服务 ",        // "ervices" = 7 cols -> 3 CJK + space
	.btn_menu       = "菜单",         // "enu" = 3 cols -> 2 CJK = 4 cols
	.btn_preset     = "预设 ",        // "reset " = 6 cols
	.btn_startstop  = "停止",         // "top" in bottom bar

	// ---- Dialogs ----
	.dlg_terminate_pid = "终止 PID: ",
	.dlg_terminate     = "终止",
	.dlg_stop          = "停止",
	.dlg_start         = "启动",
	.dlg_continue      = "继续",
	.dlg_pause         = "暂停",
	.dlg_error         = "错误:",
	.dlg_failure       = "失败:",
	.dlg_term_too_small = "终端尺寸太小，无法",
	.dlg_display_menu  = "显示菜单或框!",
	.dlg_warning       = "警告",
	.dlg_starttype     = "启动类型",
	.dlg_set_starttype = "设置启动类型: ",
	.dlg_choose_type   = "选择类型。",
	.dlg_set_type      = "设置类型。",
	.dlg_abort         = "放弃。",
	.dlg_help          = "帮助",

	// Service start types
	.svc_auto       = "自动",
	.svc_manual     = "手动",
	.svc_disabled   = "禁用",
	.svc_system     = "系统",
	.svc_boot       = "引导",

	// Error messages
	.err_access_denied    = "拒绝访问!",
	.err_no_handle        = "无法获取进程或服务的句柄!",
	.err_something_wrong  = "出现错误，请查看日志文件获取更多信息!",
	.err_invalid_service  = "请求的操作对此服务无效!",
	.err_unknown          = "未知错误! (errno: ",

	// ---- No boxes shown screen ----
	.no_boxes       = "未显示任何框!",
	.show_cpu_box   = "显示 CPU 框",
	.show_mem_box   = "显示内存框",
	.show_net_box   = "显示网络框",
	.show_proc_box  = "显示进程框",
	.show_menu      = "显示菜单",
	.quit           = "退出",

	// ---- Terminal size ----
	.term_too_small = "终端尺寸太小:",
	.term_width     = " 宽度 = ",
	.term_height    = " 高度 = ",
	.term_needed    = "当前配置所需:",

	// ---- CLI ----
	.cli_usage      = "用法: btop [-h] [-v] [-/+t] [-p <id>] [--utf-force] [--debug]\n\n"
	                  "可选参数:\n",
	.cli_args       = "  -h, --help            显示此帮助信息并退出\n"
	                  "  -v, --version         显示版本信息并退出\n"
	                  "  -lc, --low-color      禁用真彩色，24 位色转 256 色\n"
	                  "  -t, --tty_on          强制开启 tty 模式，最多 16 色和 tty 符号\n"
	                  "  +t, --tty_off         强制关闭 tty 模式\n"
	                  "  -p, --preset <id>     以预设启动，0-9 之间的整数\n"
	                  "  --debug               DEBUG 模式启动：显示微秒计时器\n"
	                  "                        并设置日志级别为 DEBUG\n",
	.cli_help       = "  -h, --help            显示此帮助信息并退出",
	.cli_version    = "btop4win 版本: ",
	.cli_low_color  = "  -lc, --low-color      禁用真彩色",
	.cli_tty_on     = "  -t, --tty_on          强制开启 tty 模式",
	.cli_tty_off    = "  +t, --tty_off         强制关闭 tty 模式",
	.cli_preset     = "  -p, --preset <id>     以预设启动 (0-9)",
	.cli_debug      = "  --debug               DEBUG 模式启动",
	.cli_preset_err = "错误: 预设选项需要一个参数。",
	.cli_preset_range = "错误: 预设选项仅接受 0-9 之间的整数值。",
	.cli_unknown_arg = "未知参数: ",
	.cli_use_help   = "使用 -h 或 --help 查看帮助。",

	// ---- Init status ----
	.init_se_debug  = "设置 SE Debug 模式",
	.init_sysinfo   = "获取系统信息",
	.init_lhm       = "Libre Hardware Monitor 初始化",
	.init_cpu       = "CPU 初始化",
	.init_mem       = "内存初始化",
	.init_wmi       = "连接 WMI",
	.init_wmi_mon   = "启动 WMI 监控",
	.init_draw      = "绘制屏幕",

	// ---- Process states ----
	.state_running  = "运行中",
	.state_sleeping = "休眠",
	.state_waiting  = "等待",
	.state_zombie   = "僵尸",
	.state_stopped  = "已停止",
	.state_tracing  = "跟踪",
	.state_dead     = "已终止",
	.state_wakekill = "唤醒终止",
	.state_unknown  = "未知",
	.state_parked   = "已驻留",

	// ---- Config validation ----
	.val_too_many_presets  = "输入了太多预设!",
	.val_too_many_boxes    = "预设中输入了太多框!",
	.val_malformed_preset  = "配置值 presets 格式错误!",
	.val_invalid_box       = "配置值 presets 中的框名无效!",
	.val_invalid_pos       = "配置值 presets 中的位置值无效!",
	.val_invalid_graph     = "配置值 presets 中的图表名无效!",
	.val_invalid_num       = "无效的数值!",
	.val_out_of_range      = "数值超出范围!",
	.val_update_ms_low     = "配置值 update_ms 设置过低 (<100)。",
	.val_update_ms_high    = "配置值 update_ms 设置过高 (>86400000)。",
	.val_invalid_loglevel  = "无效的 log_level: ",
	.val_invalid_symbol    = "无效的图表符号标识符: ",
	.val_invalid_boxes     = "shown_boxes 中的框名无效!",
	.val_invalid_procsort  = "无效的进程排序选项!",
	.val_invalid_svcsort   = "无效的服务排序选项!",
	.val_invalid_io        = "io_graph_speeds 格式无效!",

	// ---- Config file ----
	.cfg_header     = "#? Config file for btop4win v. ",
};
