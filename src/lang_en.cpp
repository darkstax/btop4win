/* English language strings for btop4win */

#include "lang.hpp"

// ============================================================================
// Help text (key-description pairs) - matches help_text in btop_menu.cpp
// ============================================================================
static const Lang::HelpEntry en_help[] = {
	{"Mouse 1",          "Clicks buttons and selects in process list."},
	{"Mouse scroll",     "Scrolls any scrollable list/text under cursor."},
	{"Esc, m",           "Toggles main menu."},
	{"p",                "Cycle view presets forwards."},
	{"shift + p",        "Cycle view presets backwards."},
	{"1",                "Toggle CPU box."},
	{"2",                "Toggle MEM box."},
	{"3",                "Toggle NET box."},
	{"4",                "Toggle PROC box."},
	{"d",                "Toggle disks view in MEM box."},
	{"F2, o",            "Shows options."},
	{"F1, h",            "Shows this window."},
	{"q, ctrl + c",      "Quits program."},
	{"+, -",             "Add/Subtract 100ms to/from update timer."},
	{"Up, Down",         "Select in process list."},
	{"Enter",            "Show detailed information for selected process."},
	{"Spacebar",         "Expand/collapse the selected process in tree view."},
	{"Pg Up, Pg Down",   "Jump 1 page in process list."},
	{"Home, End",        "Jump to first or last page in process list."},
	{"Left, Right",      "Select previous/next sorting column."},
	{"b, n",             "Select previous/next network device."},
	{"i",                "Toggle disks io mode with big graphs."},
	{"z",                "Toggle totals reset for current network device"},
	{"a",                "Toggle auto scaling for the network graphs."},
	{"y",                "Toggle synced scaling mode for network graphs."},
	{"f, /",             "To enter a process filter."},
	{"delete",           "Clear any entered filter."},
	{"c",                "Toggle per-core cpu usage of processes."},
	{"r",                "Reverse sorting order in processes box."},
	{"e",                "Toggle processes tree view."},
	{"s",                "Toggle services/processes."},
	{"Selected +, -",    "Expand/collapse the selected process in tree view."},
	{"Selected t",       "Terminate selected process"},
	{"", " "},
	{"",                 "For bug reporting and project updates, visit:"},
	{"",                 "https://github.com/aristocratos/btop4win"},
};

// ============================================================================
// Category option descriptions
// Each entry: pointer to array of const char* where [0]=key, [1..n]=description lines
// ============================================================================

// ---- General category ----
static const char* en_general_0[] = {
	"color_theme",
	"Set color theme.",
	"",
	"Choose from all theme files in (usually)",
	"\"/usr/[local/]share/btop/themes\" and",
	"\"~/.config/btop/themes\".",
	"",
	"\"Default\" for builtin default theme.",
	"\"TTY\" for builtin 16-color theme.",
	"",
	"For theme updates see:",
	"https://github.com/aristocratos/btop",
	nullptr
};
static const char* en_general_1[] = {
	"theme_background",
	"If the theme set background should be shown.",
	"",
	"Set to False if you want terminal background",
	"transparency.",
	nullptr
};
static const char* en_general_2[] = {
	"truecolor",
	"Sets if 24-bit truecolor should be used.",
	"",
	"Will convert 24-bit colors to 256 color",
	"(6x6x6 color cube) if False.",
	"",
	"Set to False if your terminal doesn't have",
	"truecolor support and can't convert to",
	"256-color.",
	nullptr
};
static const char* en_general_3[] = {
	"force_tty",
	"TTY mode.",
	"",
	"Set to true to force tty mode regardless",
	"if a real tty has been detected or not.",
	"",
	"Will force 16-color mode and TTY theme,",
	"set all graph symbols to \"tty\" and swap",
	"out other non tty friendly symbols.",
	nullptr
};
static const char* en_general_4[] = {
	"vim_keys",
	"Enable vim keys.",
	"Set to True to enable \"h,j,k,l\" keys for",
	"directional control in lists.",
	"",
	"Conflicting keys for",
	"h (help) and k (kill)",
	"is accessible while holding shift.",
	nullptr
};
static const char* en_general_5[] = {
	"enable_ohmr",
	"Enable Libre Hardware Monitor Report.",
	"",
	"Enables monitoring of CPU temps, CPU clock",
	"and GPU via PawnIO backend.",
	"Requires PawnIO driver (https://pawnio.eu),",
	"and LHM-CppExport DLL's in program folder.",
	"https://github.com/aristocratos/LHM-CppExport",
	"No admin needed after driver install.",
	"",
	"A restart is needed for this option to",
	"take effect.",
	nullptr
};
static const char* en_general_6[] = {
	"show_gpu",
	"Also show gpu stats in cpu and mem box.",
	"",
	"Only works if Libre Hardware Monitor",
	"is also enabled.",
	nullptr
};
static const char* en_general_7[] = {
	"selected_gpu",
	"Which GPU to display if multiple is present.",
	"",
	"Select GPU from list of detected devices.",
	"Defaults to \"Auto\" for first detected",
	"device.",
	nullptr
};
static const char* en_general_8[] = {
	"gpu_mem_override",
	"Manually set the GPU total memory shown.",
	"",
	"For use when not correctly detected.",
	"Value in MiB.",
	"Example: \"1024\" for 1 GiB.",
	nullptr
};
static const char* en_general_9[] = {
	"presets",
	"Define presets for the layout of the boxes.",
	"",
	"Preset 0 is always all boxes shown with",
	"default settings.",
	"Max 9 presets.",
	"",
	"Format: \"box_name:P:G,box_name:P:G\"",
	"P=(0 or 1) for alternate positions.",
	"G=graph symbol to use for box.",
	"",
	"Use withespace \" \" as separator between",
	"different presets.",
	"",
	"Example:",
	"\"mem:0:tty,proc:1:default cpu:0:braille\"",
	nullptr
};
static const char* en_general_10[] = {
	"shown_boxes",
	"Manually set which boxes to show.",
	"",
	"Available values are \"cpu mem net proc\".",
	"Separate values with whitespace.",
	"",
	"Toggle between presets with key \"p\".",
	nullptr
};
static const char* en_general_11[] = {
	"update_ms",
	"Update time in milliseconds.",
	"",
	"Recommended 2000 ms or above for better",
	"sample times for graphs.",
	"",
	"Min value: 100 ms",
	"Max value: 86400000 ms = 24 hours.",
	nullptr
};
static const char* en_general_12[] = {
	"rounded_corners",
	"Rounded corners on boxes.",
	"",
	"True or False",
	"",
	"Is always False if TTY mode is ON.",
	nullptr
};
static const char* en_general_13[] = {
	"graph_symbol",
	"Default symbols to use for graph creation.",
	"",
	"\"braille\", \"block\" or \"tty\".",
	"",
	"\"braille\" offers the highest resolution but",
	"might not be included in all fonts.",
	"",
	"\"block\" has half the resolution of braille",
	"but uses more common characters.",
	"",
	"\"tty\" uses only 3 different symbols but will",
	"work with most fonts.",
	"",
	"Note that \"tty\" only has half the horizontal",
	"resolution of the other two,",
	"so will show a shorter historical view.",
	nullptr
};
static const char* en_general_14[] = {
	"clock_format",
	"Draw a clock at top of screen.",
	"(Only visible if cpu box is enabled!)",
	"",
	"Formatting according to strftime, empty",
	"string to disable.",
	"",
	"Custom formatting options:",
	"\"/host\" = hostname",
	"\"/user\" = username",
	"\"/uptime\" = system uptime",
	"",
	"Examples of strftime formats:",
	"\"%X\" = locale HH:MM:SS",
	"\"%H\" = 24h hour, \"%I\" = 12h hour",
	"\"%M\" = minute, \"%S\" = second",
	"\"%d\" = day, \"%m\" = month, \"%y\" = year",
	nullptr
};
static const char* en_general_15[] = {
	"base_10_sizes",
	"Use base 10 for bits and bytes sizes.",
	"",
	"Uses KB = 1000 instead of KiB = 1024,",
	"MB = 1000KB instead of MiB = 1024KiB,",
	"and so on.",
	"",
	"True or False.",
	nullptr
};
static const char* en_general_16[] = {
	"background_update",
	"Update main ui when menus are showing.",
	"",
	"True or False.",
	"",
	"Set this to false if the menus is flickering",
	"too much for a comfortable experience.",
	nullptr
};
static const char* en_general_17[] = {
	"show_battery",
	"Show battery stats.",
	"(Only visible if cpu box is enabled!)",
	"",
	"Show battery stats in the top right corner",
	"if a battery is present.",
	nullptr
};
static const char* en_general_18[] = {
	"log_level",
	"Set loglevel for error.log",
	"",
	"\"ERROR\", \"WARNING\", \"INFO\" and \"DEBUG\".",
	"",
	"The level set includes all lower levels,",
	"i.e. \"DEBUG\" will show all logging info.",
	nullptr
};

static const Lang::OptionDesc en_general_options[] = {
	{en_general_0[0],  en_general_0+1,  11},
	{en_general_1[0],  en_general_1+1,  4},
	{en_general_2[0],  en_general_2+1,  8},
	{en_general_3[0],  en_general_3+1,  8},
	{en_general_4[0],  en_general_4+1,  7},
	{en_general_5[0],  en_general_5+1,  11},
	{en_general_6[0],  en_general_6+1,  4},
	{en_general_7[0],  en_general_7+1,  5},
	{en_general_8[0],  en_general_8+1,  5},
	{en_general_9[0],  en_general_9+1,  15},
	{en_general_10[0], en_general_10+1, 6},
	{en_general_11[0], en_general_11+1, 7},
	{en_general_12[0], en_general_12+1, 5},
	{en_general_13[0], en_general_13+1, 16},
	{en_general_14[0], en_general_14+1, 16},
	{en_general_15[0], en_general_15+1, 7},
	{en_general_16[0], en_general_16+1, 6},
	{en_general_17[0], en_general_17+1, 5},
	{en_general_18[0], en_general_18+1, 6},
};

// ---- CPU category ----
static const char* en_cpu_0[] = {
	"cpu_bottom",
	"Cpu box location.",
	"",
	"Show cpu box at bottom of screen instead",
	"of top.",
	nullptr
};
static const char* en_cpu_1[] = {
	"cpu_wide",
	"Cpu box width.",
	"",
	"Show full-width cpu box.",
	nullptr
};
static const char* en_cpu_2[] = {
	"graph_symbol_cpu",
	"Graph symbol to use for graphs in cpu box.",
	"",
	"\"default\", \"braille\", \"block\" or \"tty\".",
	"",
	"\"default\" for the general default symbol.",
	nullptr
};
static const char* en_cpu_3[] = {
	"cpu_graph_upper",
	"Cpu upper graph.",
	"",
	"Sets the CPU stat shown in upper half of",
	"the CPU graph.",
	"",
	"\"total\" = Total cpu usage.",
	"\"user\" = User mode cpu usage.",
	"\"system\" = Kernel mode cpu usage.",
	"+ more depending on kernel.",
	nullptr
};
static const char* en_cpu_4[] = {
	"cpu_graph_lower",
	"Cpu lower graph.",
	"",
	"Sets the CPU stat shown in lower half of",
	"the CPU graph.",
	"",
	"\"total\" = Total cpu usage.",
	"\"user\" = User mode cpu usage.",
	"\"system\" = Kernel mode cpu usage.",
	"+ more depending on kernel.",
	nullptr
};
static const char* en_cpu_5[] = {
	"cpu_invert_lower",
	"Toggles orientation of the lower CPU graph.",
	"",
	"True or False.",
	nullptr
};
static const char* en_cpu_6[] = {
	"cpu_single_graph",
	"Completely disable the lower CPU graph.",
	"",
	"Shows only upper CPU graph and resizes it",
	"to fit to box height.",
	"",
	"True or False.",
	nullptr
};
static const char* en_cpu_7[] = {
	"check_temp",
	"Enable cpu temperature reporting.",
	"",
	"True or False.",
	nullptr
};
static const char* en_cpu_8[] = {
	"show_coretemp",
	"Show temperatures for cpu cores.",
	"",
	"Only works if check_temp is True and",
	"the system is reporting core temps.",
	nullptr
};
static const char* en_cpu_9[] = {
	"temp_scale",
	"Which temperature scale to use.",
	"",
	"Celsius, default scale.",
	"",
	"Fahrenheit, the american one.",
	"",
	"Kelvin, 0 = absolute zero, 1 degree change",
	"equals 1 degree change in Celsius.",
	"",
	"Rankine, 0 = abosulte zero, 1 degree change",
	"equals 1 degree change in Fahrenheit.",
	nullptr
};
static const char* en_cpu_10[] = {
	"custom_cpu_name",
	"Custom cpu model name in cpu percentage box.",
	"",
	"Empty string to disable.",
	nullptr
};
static const char* en_cpu_11[] = {
	"show_uptime",
	"Shows the system uptime in the CPU box.",
	"",
	"Can also be shown in the clock by using",
	"\"/uptime\" in the formatting.",
	"",
	"True or False.",
	nullptr
};

static const Lang::OptionDesc en_cpu_options[] = {
	{en_cpu_0[0],  en_cpu_0+1,  4},
	{en_cpu_1[0],  en_cpu_1+1,  3},
	{en_cpu_2[0],  en_cpu_2+1,  5},
	{en_cpu_3[0],  en_cpu_3+1,  9},
	{en_cpu_4[0],  en_cpu_4+1,  9},
	{en_cpu_5[0],  en_cpu_5+1,  3},
	{en_cpu_6[0],  en_cpu_6+1,  6},
	{en_cpu_7[0],  en_cpu_7+1,  3},
	{en_cpu_8[0],  en_cpu_8+1,  4},
	{en_cpu_9[0],  en_cpu_9+1,  11},
	{en_cpu_10[0], en_cpu_10+1, 3},
	{en_cpu_11[0], en_cpu_11+1, 6},
};

// ---- Memory category ----
static const char* en_mem_0[] = {
	"mem_below_net",
	"Mem box location.",
	"",
	"Show mem box below net box instead of above.",
	nullptr
};
static const char* en_mem_1[] = {
	"graph_symbol_mem",
	"Graph symbol to use for graphs in mem box.",
	"",
	"\"default\", \"braille\", \"block\" or \"tty\".",
	"",
	"\"default\" for the general default symbol.",
	nullptr
};
static const char* en_mem_2[] = {
	"mem_graphs",
	"Show graphs for memory values.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_3[] = {
	"show_disks",
	"Split memory box to also show disks.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_4[] = {
	"show_io_stat",
	"Toggle IO activity graphs.",
	"",
	"Show small IO graphs that for disk activity",
	"(disk busy time) when not in IO mode.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_5[] = {
	"io_mode",
	"Toggles io mode for disks.",
	"",
	"Shows big graphs for disk read/write speeds",
	"instead of used/free percentage meters.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_6[] = {
	"io_graph_combined",
	"Toggle combined read and write graphs.",
	"",
	"Only has effect if \"io mode\" is True.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_7[] = {
	"io_graph_speeds",
	"Set top speeds for the io graphs.",
	"",
	"Manually set which speed in MiB/s that",
	"equals 100 percent in the io graphs.",
	"(100 MiB/s by default).",
	"",
	"Format: \"device:\\speed\" separate disks with",
	"whitespace \" \".",
	"",
	"Example: \"C:\\100, D:\\20\".",
	nullptr
};
static const char* en_mem_8[] = {
	"show_page",
	"If page memory should be shown in memory box.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_9[] = {
	"only_physical",
	"Filter out non physical disks.",
	"",
	"Set this to False to include network disks,",
	"RAM disks and similar.",
	"",
	"True or False.",
	nullptr
};
static const char* en_mem_10[] = {
	"disk_free_priv",
	"Type of available disk space.",
	"",
	"Set to true to show how much disk space is",
	"available for privileged users.",
	"",
	"Set to false to show available for normal",
	"users.",
	nullptr
};
static const char* en_mem_11[] = {
	"disks_filter",
	"Optional filter for shown disks.",
	"",
	"Should be full path of a mountpoint.",
	"Separate multiple values with",
	"whitespace \" \".",
	"",
	"Begin line with \"exclude=\" to change to",
	"exclude filter.",
	"Oterwise defaults to \"most include\" filter.",
	"",
	"Example:",
	"\"exclude=D:\\ E:\\\"",
	nullptr
};

static const Lang::OptionDesc en_mem_options[] = {
	{en_mem_0[0],  en_mem_0+1,  3},
	{en_mem_1[0],  en_mem_1+1,  5},
	{en_mem_2[0],  en_mem_2+1,  3},
	{en_mem_3[0],  en_mem_3+1,  3},
	{en_mem_4[0],  en_mem_4+1,  6},
	{en_mem_5[0],  en_mem_5+1,  6},
	{en_mem_6[0],  en_mem_6+1,  5},
	{en_mem_7[0],  en_mem_7+1,  10},
	{en_mem_8[0],  en_mem_8+1,  3},
	{en_mem_9[0],  en_mem_9+1,  6},
	{en_mem_10[0], en_mem_10+1, 7},
	{en_mem_11[0], en_mem_11+1, 12},
};

// ---- Network category ----
static const char* en_net_0[] = {
	"graph_symbol_net",
	"Graph symbol to use for graphs in net box.",
	"",
	"\"default\", \"braille\", \"block\" or \"tty\".",
	"",
	"\"default\" for the general default symbol.",
	nullptr
};
static const char* en_net_1[] = {
	"net_download",
	"Fixed network graph download value.",
	"",
	"Value in Mebibits, default \"100\".",
	"",
	"Can be toggled with auto button.",
	nullptr
};
static const char* en_net_2[] = {
	"net_upload",
	"Fixed network graph upload value.",
	"",
	"Value in Mebibits, default \"100\".",
	"",
	"Can be toggled with auto button.",
	nullptr
};
static const char* en_net_3[] = {
	"net_auto",
	"Start in network graphs auto rescaling mode.",
	"",
	"Ignores any values set above at start and",
	"rescales down to 10Kibibytes at the lowest.",
	"",
	"True or False.",
	nullptr
};
static const char* en_net_4[] = {
	"net_sync",
	"Network scale sync.",
	"",
	"Syncs the scaling for download and upload to",
	"whichever currently has the highest scale.",
	"",
	"True or False.",
	nullptr
};
static const char* en_net_5[] = {
	"net_iface",
	"Network Interface.",
	"",
	"Manually set the starting Network Interface.",
	"",
	"Will otherwise automatically choose the NIC",
	"with the highest total download since boot.",
	nullptr
};

static const Lang::OptionDesc en_net_options[] = {
	{en_net_0[0], en_net_0+1, 5},
	{en_net_1[0], en_net_1+1, 5},
	{en_net_2[0], en_net_2+1, 5},
	{en_net_3[0], en_net_3+1, 6},
	{en_net_4[0], en_net_4+1, 6},
	{en_net_5[0], en_net_5+1, 6},
};

// ---- Process category ----
static const char* en_proc_0[] = {
	"proc_left",
	"Proc box location.",
	"",
	"Show proc box on left side of screen",
	"instead of right.",
	nullptr
};
static const char* en_proc_1[] = {
	"graph_symbol_proc",
	"Graph symbol to use for graphs in proc box.",
	"",
	"\"default\", \"braille\", \"block\" or \"tty\".",
	"",
	"\"default\" for the general default symbol.",
	nullptr
};
static const char* en_proc_2[] = {
	"proc_sorting",
	"Processes sorting option.",
	"",
	"Possible values:",
	"\"pid\", \"program\", \"arguments\", \"threads\",",
	"\"user\", \"memory\", \"cpu lazy\" and",
	"\"cpu direct\".",
	"",
	"\"cpu lazy\" updates top process over time.",
	"\"cpu direct\" updates top process",
	"directly.",
	nullptr
};
static const char* en_proc_3[] = {
	"proc_services",
	"Show services.",
	"",
	"Show services in the process box instead of",
	"processes.",
	"",
	"True or False.",
	nullptr
};
static const char* en_proc_4[] = {
	"services_sorting",
	"Services sorting option.",
	"",
	"Possible values:",
	"\"service\", \"caption\", \"status\", \"memory\",",
	"\"cpu lazy\" and \"cpu direct\".",
	"",
	"\"cpu lazy\" updates top service over time.",
	"\"cpu direct\" updates top service",
	"directly.",
	nullptr
};
static const char* en_proc_5[] = {
	"proc_reversed",
	"Reverse processes sorting order.",
	"",
	"True or False.",
	nullptr
};
static const char* en_proc_6[] = {
	"proc_tree",
	"Processes tree view.",
	"",
	"Set true to show processes grouped by",
	"parents with lines drawn between parent",
	"and child process.",
	nullptr
};
static const char* en_proc_7[] = {
	"proc_colors",
	"Enable colors in process view.",
	"",
	"True or False.",
	nullptr
};
static const char* en_proc_8[] = {
	"proc_gradient",
	"Enable process view gradient fade.",
	"",
	"Fades from top or current selection.",
	"Max fade value is equal to current themes",
	"\"inactive_fg\" color value.",
	nullptr
};
static const char* en_proc_9[] = {
	"proc_per_core",
	"Process usage per core.",
	"",
	"If process cpu usage should be of the core",
	"it's running on or usage of the total",
	"available cpu power.",
	"",
	"If true and process is multithreaded",
	"cpu usage can reach over 100%.",
	nullptr
};
static const char* en_proc_10[] = {
	"proc_mem_bytes",
	"Show memory as bytes in process list.",
	" ",
	"Will show percentage of total memory",
	"if False.",
	nullptr
};

static const Lang::OptionDesc en_proc_options[] = {
	{en_proc_0[0],  en_proc_0+1,  4},
	{en_proc_1[0],  en_proc_1+1,  5},
	{en_proc_2[0],  en_proc_2+1,  10},
	{en_proc_3[0],  en_proc_3+1,  6},
	{en_proc_4[0],  en_proc_4+1,  9},
	{en_proc_5[0],  en_proc_5+1,  3},
	{en_proc_6[0],  en_proc_6+1,  5},
	{en_proc_7[0],  en_proc_7+1,  3},
	{en_proc_8[0],  en_proc_8+1,  5},
	{en_proc_9[0],  en_proc_9+1,  8},
	{en_proc_10[0], en_proc_10+1, 4},
};

// ============================================================================
// Main LANG_EN definition
// ============================================================================
const Lang LANG_EN = {
	// ---- Help text ----
	.help           = en_help,
	.help_count     = sizeof(en_help) / sizeof(en_help[0]),

	// ---- Category option descriptions ----
	.cats = {
		{ en_general_options, sizeof(en_general_options) / sizeof(en_general_options[0]) },
		{ en_cpu_options,     sizeof(en_cpu_options)     / sizeof(en_cpu_options[0]) },
		{ en_mem_options,     sizeof(en_mem_options)     / sizeof(en_mem_options[0]) },
		{ en_net_options,     sizeof(en_net_options)     / sizeof(en_net_options[0]) },
		{ en_proc_options,    sizeof(en_proc_options)    / sizeof(en_proc_options[0]) },
	},

	// ---- Category tab names ----
	.cat_general    = "general",
	.cat_cpu        = "cpu",
	.cat_mem        = "mem",
	.cat_net        = "net",
	.cat_proc       = "proc",

	// ---- Help menu headers ----
	.help_key       = "Key:",
	.help_desc      = "Description:",

	// ---- Buttons ----
	.btn_ok         = "    Ok    ",
	.btn_yes        = "    Yes    ",
	.btn_no         = "    No    ",

	// ---- Box titles ----
	.box_cpu        = "cpu",
	.box_mem        = "mem",
	.box_net        = "net",
	.box_proc       = "proc",
	.box_download   = "Download",
	.box_upload     = "Upload",

	// ---- CPU box ----
	.cpu_label      = "CPU ",
	.gpu_label      = "GPU ",
	.up_prefix      = "up",
	.load_avg       = "Load AVG:",
	.lav            = "LAV:",
	.bat            = "BAT",
	.charging       = "Charging",
	.discharging    = "Discharging",
	.full           = "Full",
	.unknown        = "Unknown",

	// ---- Memory box ----
	.mem_total      = "Total:",
	.mem_used       = "Used",
	.mem_free       = " Free:",
	.mem_free_short = "F",
	.mem_used_short = "U",
	.mem_pagevirt   = "Page+Virt:",
	.mem_pagefiles  = "Pagefiles:",
	.mem_gpu        = "GPU",
	.mem_gpu_shared = " Shared",
	.mem_io_pct     = " IO% ",
	.mem_io         = " IO   ",
	.mem_rw         = "RW",
	.mem_r          = "R",
	.mem_w          = "W",
	.btn_disks      = "isks",
	.btn_io         = "io",

	// ---- Network box ----
	.net_top        = "Top: ",
	.net_total      = "Total: ",
	.btn_zero       = "ero",
	.btn_auto       = "uto",
	.btn_sync       = "ync",

	// ---- Process box ----
	.proc_pid       = "Pid:",
	.proc_program   = "Program:",
	.proc_command   = "Command:",
	.proc_tree      = "Tree:",
	.proc_threads   = "Threads: ",
	.proc_user      = "User:",
	.proc_status    = "Status:",
	.proc_memb      = "MemB",
	.proc_mem_pct   = "Mem%",
	.proc_cpu_pct   = "Cpu%",
	.proc_service   = "Service:",
	.proc_caption   = "Caption:",
	.proc_parent    = "Parent :",
	.proc_owner     = "Owner:",
	.proc_start     = "Start:",
	.proc_elapsed   = "Elapsed:",
	.proc_ior       = "IO/R:",
	.proc_iow       = "IO/W:",
	.proc_memory    = "Memory: ",
	.proc_memory_short = "M:",

	// Process buttons
	.btn_terminate  = "terminate",
	.btn_stop       = "stop",
	.btn_start      = "start",
	.btn_pause      = "pause",
	.btn_continue   = "continue",
	.btn_starttype  = "Start-type",
	.btn_hide       = "hide ",
	.btn_select     = " select ",
	.btn_info       = "info ",
	.btn_filter     = "ilter",
	.btn_filter_del = " del",
	.btn_percore    = "per-",
	.btn_reverse    = "everse",
	.btn_tree       = "tre",
	.btn_services   = "ervices",
	.btn_menu       = "enu",
	.btn_preset     = "reset ",
	.btn_startstop  = "top",

	// ---- Dialogs ----
	.dlg_terminate_pid = "Terminate PID: ",
	.dlg_terminate     = "terminate",
	.dlg_stop          = "stop",
	.dlg_start         = "start",
	.dlg_continue      = "continue",
	.dlg_pause         = "pause",
	.dlg_error         = "Error:",
	.dlg_failure       = "Failure:",
	.dlg_term_too_small = "Terminal size to small to",
	.dlg_display_menu  = "display menu or box!",
	.dlg_warning       = "warning",
	.dlg_starttype     = "start-type",
	.dlg_set_starttype = "Set start-type for ",
	.dlg_choose_type   = "To choose type.",
	.dlg_set_type      = "To set type.",
	.dlg_abort         = "To abort.",
	.dlg_help          = "help",

	// Service start types
	.svc_auto       = "Auto",
	.svc_manual     = "Manual",
	.svc_disabled   = "Disabled",
	.svc_system     = "System",
	.svc_boot       = "Boot",

	// Error messages
	.err_access_denied    = "Access denied!",
	.err_no_handle        = "Could not get handle to process or service!",
	.err_something_wrong  = "Something went wrong, check logfile for more info!",
	.err_invalid_service  = "Requested action is not valid for this service!",
	.err_unknown          = "Unknown error! (errno: ",

	// ---- No boxes shown screen ----
	.no_boxes       = "No boxes shown!",
	.show_cpu_box   = "Show CPU box",
	.show_mem_box   = "Show MEM box",
	.show_net_box   = "Show NET box",
	.show_proc_box  = "Show PROC box",
	.show_menu      = "Show menu",
	.quit           = "Quit",

	// ---- Terminal size ----
	.term_too_small = "Terminal size too small:",
	.term_width     = " Width = ",
	.term_height    = " Height = ",
	.term_needed    = "Needed for current config:",

	// ---- CLI ----
	.cli_usage      = "usage: btop [-h] [-v] [-/+t] [-p <id>] [--utf-force] [--debug]\n\n"
	                  "optional arguments:\n",
	.cli_args       = "  -h, --help            show this help message and exit\n"
	                  "  -v, --version         show version info and exit\n"
	                  "  -lc, --low-color      disable truecolor, converts 24-bit colors to 256-color\n"
	                  "  -t, --tty_on          force (ON) tty mode, max 16 colors and tty friendly graph symbols\n"
	                  "  +t, --tty_off         force (OFF) tty mode\n"
	                  "  -p, --preset <id>     start with preset, integer value between 0-9\n"
	                  "  --debug               start in DEBUG mode: shows microsecond timer for information collect\n"
	                  "                        and screen draw functions and sets loglevel to DEBUG\n",
	.cli_help       = "  -h, --help            show this help message and exit",
	.cli_version    = "btop4win version: ",
	.cli_low_color  = "  -lc, --low-color      disable truecolor",
	.cli_tty_on     = "  -t, --tty_on          force (ON) tty mode",
	.cli_tty_off    = "  +t, --tty_off         force (OFF) tty mode",
	.cli_preset     = "  -p, --preset <id>     start with preset (0-9)",
	.cli_debug      = "  --debug               start in DEBUG mode",
	.cli_preset_err = "ERROR: Preset option needs an argument.",
	.cli_preset_range = "ERROR: Preset option only accepts an integer value between 0-9.",
	.cli_unknown_arg = " Unknown argument: ",
	.cli_use_help   = " Use -h or --help for help.",

	// ---- Init status ----
	.init_se_debug  = "Setting SE Debug Mode",
	.init_sysinfo   = "Getting system info",
	.init_lhm       = "Libre Hardware Monitor Init",
	.init_cpu       = "CPU Init",
	.init_mem       = "MEM Init",
	.init_wmi       = "Connecting to WMI",
	.init_wmi_mon   = "Starting WMI monitor",
	.init_draw      = "Drawing to screen",

	// ---- Process states ----
	.state_running  = "Running",
	.state_sleeping = "Sleeping",
	.state_waiting  = "Waiting",
	.state_zombie   = "Zombie",
	.state_stopped  = "Stopped",
	.state_tracing  = "Tracing",
	.state_dead     = "Dead",
	.state_wakekill = "Wakekill",
	.state_unknown  = "Unknown",
	.state_parked   = "Parked",

	// ---- Config validation ----
	.val_too_many_presets  = "Too many presets entered!",
	.val_too_many_boxes    = "Too many boxes entered for preset!",
	.val_malformed_preset  = "Malformatted preset in config value presets!",
	.val_invalid_box       = "Invalid box name in config value presets!",
	.val_invalid_pos       = "Invalid position value in config value presets!",
	.val_invalid_graph     = "Invalid graph name in config value presets!",
	.val_invalid_num       = "Invalid numerical value!",
	.val_out_of_range      = "Value out of range!",
	.val_update_ms_low     = "Config value update_ms set too low (<100).",
	.val_update_ms_high    = "Config value update_ms set too high (>86400000).",
	.val_invalid_loglevel  = "Invalid log_level: ",
	.val_invalid_symbol    = "Invalid graph symbol identifier: ",
	.val_invalid_boxes     = "Invalid box name(s) in shown_boxes!",
	.val_invalid_procsort  = "Invalid process sorting option!",
	.val_invalid_svcsort   = "Invalid services sorting option!",
	.val_invalid_io        = "Invalid formatting of io_graph_speeds!",

	// ---- Config file ----
	.cfg_header     = "#? Config file for btop4win v. ",
};
