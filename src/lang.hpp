/* Language/localization system for btop4win */

#pragma once
#include <string>
#include <vector>
#include <array>

struct Lang {
	// ---- Help text (key-description pairs) ----
	struct HelpEntry { const char* key; const char* desc; };
	const HelpEntry* help;
	int help_count;

	// ---- Category option descriptions ----
	// First element of each option is the key name (NOT translated),
	// remaining elements are the translated description lines.
	struct OptionDesc {
		const char* key;
		const char** lines;
		int line_count;
	};
	struct CategoryDesc {
		const OptionDesc* options;
		int option_count;
	};

	// 5 categories: general, cpu, mem, net, proc
	CategoryDesc cats[5];

	// ---- Category tab names ----
	const char* cat_general;
	const char* cat_cpu;
	const char* cat_mem;
	const char* cat_net;
	const char* cat_proc;

	// ---- Help menu headers ----
	const char* help_key;
	const char* help_desc;

	// ---- Buttons ----
	const char* btn_ok;
	const char* btn_yes;
	const char* btn_no;

	// ---- Box titles ----
	const char* box_cpu;
	const char* box_mem;
	const char* box_net;
	const char* box_proc;
	const char* box_download;
	const char* box_upload;

	// ---- CPU box ----
	const char* cpu_label;         // "CPU " meter label
	const char* gpu_label;         // "GPU " meter label
	const char* up_prefix;         // "up" uptime prefix
	const char* load_avg;          // "Load AVG:"
	const char* lav;               // "LAV:"
	const char* bat;               // "BAT"
	const char* charging;
	const char* discharging;
	const char* full;
	const char* unknown;

	// ---- Memory box ----
	const char* mem_total;         // "Total:"
	const char* mem_used;          // "Used"
	const char* mem_available;     // "Avail"
	const char* mem_cached;        // "Cache"
	const char* mem_committed;     // "Commi"
	const char* mem_free;          // " Free:"
	const char* mem_free_short;    // "F"
	const char* mem_used_short;    // "U"
	const char* mem_pagevirt;      // "Page+Virt:"
	const char* mem_pagefiles;     // "Pagefiles:"
	const char* mem_gpu;           // "GPU"
	const char* mem_gpu_shared;    // " Shared"
	const char* mem_io_pct;        // " IO% "
	const char* mem_io;            // " IO   "
	const char* mem_rw;            // "RW"
	const char* mem_r;             // "R"
	const char* mem_w;             // "W"
	const char* btn_disks;         // "isks" (completes "d"+"isks")
	const char* btn_io;            // "io"

	// ---- Network box ----
	const char* net_top;           // "Top: "
	const char* net_total;         // "Total: "
	const char* btn_zero;          // "ero" (completes "z"+"ero")
	const char* btn_auto;          // "uto" (completes "a"+"uto")
	const char* btn_sync;          // "ync" (completes "s"+"ync")

	// ---- Process box ----
	const char* proc_pid;          // "Pid:"
	const char* proc_program;      // "Program:"
	const char* proc_command;      // "Command:"
	const char* proc_tree;         // "Tree:"
	const char* proc_threads;      // "Threads: "
	const char* proc_user;         // "User:"
	const char* proc_status;       // "Status:"
	const char* proc_memb;         // "MemB"
	const char* proc_mem_pct;      // "Mem%"
	const char* proc_cpu_pct;      // "Cpu%"
	const char* proc_service;      // "Service:"
	const char* proc_caption;      // "Caption:"
	const char* proc_parent;       // "Parent :"
	const char* proc_owner;        // "Owner:"
	const char* proc_start;        // "Start:"
	const char* proc_elapsed;      // "Elapsed:"
	const char* proc_ior;          // "IO/R:"
	const char* proc_iow;          // "IO/W:"
	const char* proc_memory;       // "Memory: "
	const char* proc_memory_short; // "M:"

	// Process buttons
	const char* btn_terminate;     // "terminate"
	const char* btn_stop;          // "stop"
	const char* btn_start;         // "start"
	const char* btn_pause;         // "pause"
	const char* btn_continue;      // "continue"
	const char* btn_starttype;     // "Start-type"
	const char* btn_hide;          // "hide "
	const char* btn_select;        // " select "
	const char* btn_info;          // "info "
	const char* btn_filter;        // "ilter" (completes "f"+"ilter")
	const char* btn_filter_del;    // " del"
	const char* btn_percore;       // "per-", "ore" around "c"
	const char* btn_reverse;       // "everse" (completes "r"+"everse")
	const char* btn_tree;          // "tre", "e"
	const char* btn_services;      // "ervices" (completes "s"+"ervices")
	const char* btn_menu;          // "enu"
	const char* btn_preset;        // "reset "
	const char* btn_startstop;     // "top" (for start/s+top)

	// ---- Dialogs ----
	const char* dlg_terminate_pid; // "Terminate PID: "
	const char* dlg_terminate;     // "terminate" (dialog title)
	const char* dlg_stop;          // "stop"
	const char* dlg_start;         // "start"
	const char* dlg_continue;      // "continue"
	const char* dlg_pause;         // "pause"
	const char* dlg_error;         // "Error:"
	const char* dlg_failure;       // "Failure:"
	const char* dlg_term_too_small;// "Terminal size too small to"
	const char* dlg_display_menu;  // "display menu or box!"
	const char* dlg_warning;       // "warning"
	const char* dlg_starttype;     // "start-type"
	const char* dlg_set_starttype; // "Set start-type for "
	const char* dlg_choose_type;   // "To choose type."
	const char* dlg_set_type;      // "To set type."
	const char* dlg_abort;         // "To abort."
	const char* dlg_help;          // "help"

	// Service start types
	const char* svc_auto;          // "Auto"
	const char* svc_manual;        // "Manual"
	const char* svc_disabled;      // "Disabled"
	const char* svc_system;        // "System"
	const char* svc_boot;          // "Boot"

	// Error messages
	const char* err_access_denied;
	const char* err_no_handle;
	const char* err_something_wrong;
	const char* err_invalid_service;
	const char* err_unknown;       // "Unknown error! (errno: "

	// ---- No boxes shown screen ----
	const char* no_boxes;
	const char* show_cpu_box;
	const char* show_mem_box;
	const char* show_net_box;
	const char* show_proc_box;
	const char* show_menu;
	const char* quit;

	// ---- Terminal size ----
	const char* term_too_small;
	const char* term_width;
	const char* term_height;
	const char* term_needed;

	// ---- CLI ----
	const char* cli_usage;
	const char* cli_args;
	const char* cli_help;
	const char* cli_version;
	const char* cli_low_color;
	const char* cli_tty_on;
	const char* cli_tty_off;
	const char* cli_preset;
	const char* cli_debug;
	const char* cli_preset_err;
	const char* cli_preset_range;
	const char* cli_unknown_arg;
	const char* cli_use_help;

	// ---- Init status ----
	const char* init_se_debug;
	const char* init_sysinfo;
	const char* init_lhm;
	const char* init_cpu;
	const char* init_mem;
	const char* init_wmi;
	const char* init_wmi_mon;
	const char* init_draw;

	// ---- Process states ----
	const char* state_running;
	const char* state_sleeping;
	const char* state_waiting;
	const char* state_zombie;
	const char* state_stopped;
	const char* state_tracing;
	const char* state_dead;
	const char* state_wakekill;
	const char* state_unknown;
	const char* state_parked;

	// ---- Config validation ----
	const char* val_too_many_presets;
	const char* val_too_many_boxes;
	const char* val_malformed_preset;
	const char* val_invalid_box;
	const char* val_invalid_pos;
	const char* val_invalid_graph;
	const char* val_invalid_num;
	const char* val_out_of_range;
	const char* val_update_ms_low;
	const char* val_update_ms_high;
	const char* val_invalid_loglevel;
	const char* val_invalid_symbol;
	const char* val_invalid_boxes;
	const char* val_invalid_procsort;
	const char* val_invalid_svcsort;
	const char* val_invalid_io;

	// ---- Config file ----
	const char* cfg_header;        // "#? Config file for btop4win v. "
};

extern const Lang LANG_EN;
extern const Lang LANG_ZH;
extern const Lang* L;

void langInit(const std::string& code);
int langDisplayWidth(const char* s);
