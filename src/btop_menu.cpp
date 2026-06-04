/* Copyright 2021 Aristocratos (jakob@qvantnet.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

indent = tab
tab-size = 4
*/

#include <deque>
#include <robin_hood.h>
#include <array>
#include <ranges>
#include <signal.h>
#include <errno.h>
#include <cmath>
#include <filesystem>
#define _WIN32_DCOM
#define _WIN32_WINNT 0x0600
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <processthreadsapi.h>

#include <btop_menu.hpp>
#include <btop_tools.hpp>
#include <btop_config.hpp>
#include <btop_theme.hpp>
#include <btop_draw.hpp>
#include <btop_shared.hpp>

using std::deque, robin_hood::unordered_flat_map, std::array, std::views::iota, std::ref, std::max, std::min, std::ceil, std::clamp;
using namespace Tools;
namespace fs = std::filesystem;
namespace rng = std::ranges;

namespace Menu {

   atomic<bool> active (false);
   string bg;
   bool redraw = true;
   int currentMenu = -1;
   msgBox messageBox;
   int signalKillRet = 0;

   const array<string, 32> P_Signals = {
	   	"0",
		"SIGHUP", "SIGINT",	"SIGQUIT",	"SIGILL",
		"SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE",
		"SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM", "SIGTERM", "16", "SIGCHLD",
		"SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN",
		"SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ",
		"SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO",
		"SIGPWR", "SIGSYS"
	};

  unordered_flat_map<string, Input::Mouse_loc> mouse_mappings;

   const array<array<string, 3>, 3> menu_normal = {
		array<string, 3>{
			"┌─┐┌─┐┌┬┐┬┌─┐┌┐┌┌─┐",
			"│ │├─┘ │ ││ ││││└─┐",
			"└─┘┴   ┴ ┴└─┘┘└┘└─┘"
		},
		{
			"┬ ┬┌─┐┬  ┌─┐",
			"├─┤├┤ │  ├─┘",
			"┴ ┴└─┘┴─┘┴  "
		},
		{
			"┌─┐ ┬ ┬ ┬┌┬┐",
			"│─┼┐│ │ │ │ ",
			"└─┘└└─┘ ┴ ┴ "
		}
	};

	const array<array<string, 3>, 3> menu_selected = {
		array<string, 3>{
			"╔═╗╔═╗╔╦╗╦╔═╗╔╗╔╔═╗",
			"║ ║╠═╝ ║ ║║ ║║║║╚═╗",
			"╚═╝╩   ╩ ╩╚═╝╝╚╝╚═╝"
		},
		{
			"╦ ╦╔═╗╦  ╔═╗",
			"╠═╣║╣ ║  ╠═╝",
			"╩ ╩╚═╝╩═╝╩  "
		},
		{
			"╔═╗ ╦ ╦ ╦╔╦╗ ",
			"║═╬╗║ ║ ║ ║  ",
			"╚═╝╚╚═╝ ╩ ╩  "
		}
	};

	const array<int, 3> menu_width = {19, 12, 12};

	// Build translated help_text from language data
	const vector<array<string, 2>>& get_help_text() {
		static vector<array<string, 2>> ht;
		static const Lang* cached = nullptr;
		if (cached != L) {
			cached = L;
			ht.clear();
			for (int i = 0; i < L->help_count; i++)
				ht.push_back({L->help[i].key, L->help[i].desc});
		}
		return ht;
	}

	// Build translated categories from language data
	const vector<vector<vector<string>>>& get_categories() {
		static vector<vector<vector<string>>> cats;
		static const Lang* cached = nullptr;
		if (cached != L) {
			cached = L;
			cats.clear();
			for (int ci = 0; ci < 5; ci++) {
				vector<vector<string>> cat;
				for (int oi = 0; oi < L->cats[ci].option_count; oi++) {
					auto& opt = L->cats[ci].options[oi];
					vector<string> entry = {opt.key};
					for (int li = 0; li < opt.line_count; li++)
						entry.push_back(opt.lines[li]);
					cat.push_back(std::move(entry));
				}
				cats.push_back(std::move(cat));
			}
		}
		return cats;
	}

	// categories: now provided by get_categories()

	msgBox::msgBox() {}
	msgBox::msgBox(int width, int boxtype, vector<string> content, string title)
	: width(width), boxtype(boxtype) {
		const auto& tty_mode = Config::getB("tty_mode");
		const auto& rounded = Config::getB("rounded_corners");
		const auto& right_up = (tty_mode or not rounded ? Symbols::right_up : Symbols::round_right_up);
		const auto& left_up = (tty_mode or not rounded ? Symbols::left_up : Symbols::round_left_up);
		const auto& right_down = (tty_mode or not rounded ? Symbols::right_down : Symbols::round_right_down);
		const auto& left_down = (tty_mode or not rounded ? Symbols::left_down : Symbols::round_left_down);
		height = content.size() + 7;
		x = Term::width / 2 - width / 2;
		y = Term::height/2 - height/2;
		if (boxtype == 2) selected = 1;


		button_left = left_up + Symbols::h_line * 6 + Mv::l(7) + Mv::d(2) + left_down + Symbols::h_line * 6 + Mv::l(7) + Mv::u(1) + Symbols::v_line;
		button_right = Symbols::v_line + Mv::l(7) + Mv::u(1) + Symbols::h_line * 6 + right_up + Mv::l(7) + Mv::d(2) + Symbols::h_line * 6 + right_down + Mv::u(2);

		box_contents = Draw::createBox(x, y, width, height, Theme::c("hi_fg"), true, title) + Mv::d(1);
		for (const auto& line : content) {
			box_contents += Mv::save + Mv::r(max((size_t)0, (width / 2) - (Fx::uncolor(line).size() / 2) - 1)) + line + Mv::restore + Mv::d(1);
		}
	}

	string msgBox::operator()() {
		string out;
		int pos = width / 2 - (boxtype == 0 ? 6 : 14);
		auto& first_color = (selected == 0 ? Theme::c("hi_fg") : Theme::c("div_line"));
		out = Mv::d(1) + Mv::r(pos) + Fx::b + first_color + button_left + (selected == 0 ? Theme::c("title") : Theme::c("main_fg") + Fx::ub)
			+ (boxtype == 0 ? L->btn_ok : L->btn_yes) + first_color + button_right;
		mouse_mappings["button1"] = Input::Mouse_loc{y + height - 4, x + pos + 1, 3, 12 + (boxtype > 0 ? 1 : 0)};
		if (boxtype > 0) {
			auto& second_color = (selected == 1 ? Theme::c("hi_fg") : Theme::c("div_line"));
			out += Mv::r(2) + second_color + button_left + (selected == 1 ? Theme::c("title") : Theme::c("main_fg") + Fx::ub)
				+ L->btn_no + second_color + button_right;
			mouse_mappings["button2"] = Input::Mouse_loc{y + height - 4, x + pos + 15 + (boxtype > 0 ? 1 : 0), 3, 12};
		}
		return box_contents + out + Fx::reset;
	}

	//? Process input
	int msgBox::input(string key) {
		if (key.empty()) return Invalid;

		if (is_in(key, "escape", "backspace", "q") or key == "button2") {
			return No_Esc;
		}
		else if (key == "button1" or (boxtype == 0 and str_to_upper(key) == "O")) {
			return Ok_Yes;
		}
		else if (is_in(key, "enter", "space")) {
			return selected + 1;
		}
		else if (boxtype == 0) {
			return Invalid;
		}
		else if (str_to_upper(key) == "Y") {
			return Ok_Yes;
		}
		else if (str_to_upper(key) == "N") {
			return No_Esc;
		}
		else if (is_in(key, "right", "tab")) {
			if (++selected > 1) selected = 0;
			return Select;
		}
		else if (is_in(key, "left", "shift_tab")) {
			if (--selected < 0) selected = 1;
			return Select;
		}

		return Invalid;
	}

	void msgBox::clear() {
		box_contents.clear();
		box_contents.shrink_to_fit();
		button_left.clear();
		button_left.shrink_to_fit();
		button_right.clear();
		button_right.shrink_to_fit();
		if (mouse_mappings.contains("button1")) mouse_mappings.erase("button1");
		if (mouse_mappings.contains("button2")) mouse_mappings.erase("button2");
	}

	enum menuReturnCodes {
		NoChange,
		Changed,
		Closed,
		Switch
	};

	int signalConfig(const string& key) {
		auto& p_name = Proc::detailed.entry.name;
		static string selected_signal = "";
		static int x = 0, y = 0, i_sel = 0;
		static const vector<string> start_index = { L->svc_auto, L->svc_manual, L->svc_disabled, L->svc_system, L->svc_boot };
		if (bg.empty()) selected_signal = Proc::detailed.start;
		i_sel = v_index(start_index, selected_signal);
		bool driver = (i_sel > 2 or Proc::detailed.service_type.ends_with("Driver"));
		auto& out = Global::overlay;
		int retval = Changed;

		if (redraw) {
			x = Term::width/2 - 40;
			y = Term::height/2 - 9;
			bg = Draw::createBox(x + 2, y, 78, 16, Theme::c("hi_fg"), true, L->dlg_starttype);
			bg += Mv::to(y+2, x+3) + Theme::c("title") + Fx::b + cjust(L->dlg_set_starttype + uresize(p_name, 30), 76);
		}
		else if (is_in(key, "escape", "q")) {
			return Closed;
		}
		else if (key.starts_with("button_")) {
			if (string new_select = key.substr(7); new_select == selected_signal)
				goto ChooseEntering;
			else {
				selected_signal = new_select;
				i_sel = v_index(start_index, selected_signal);
			}
		}
		else if (is_in(key, "enter", "space")) {
			ChooseEntering:
			signalKillRet = 0;
			if (auto ret = ServiceSetStart(p_name, ServiceStartTypes.at(selected_signal)); ret != ERROR_SUCCESS) {
				signalKillRet = ret;
				menuMask.set(SignalReturn);
			}
			
			if (signalKillRet == 0 and Proc::detailed.can_pause) {
				ServiceCommand(p_name, SCchange);
			}

			return Closed;
		}
		else if (is_in(key, "up", "k")) {
			if (--i_sel < 0) i_sel = (driver ? 4 : 2);
			selected_signal = start_index.at(i_sel);
		}
		else if (is_in(key, "down", "j")) {
			if (++i_sel > (driver ? 4 : 2)) i_sel = 0;
			selected_signal = start_index.at(i_sel);
		}
		else {
			retval = NoChange;
		}

		if (retval == Changed) {
			int cy = y+4, cx = x+30;
			out = bg;
			int i = 0;
			for (const auto& valstr : start_index) {
				out += Mv::to(cy, cx);
				if (i == i_sel) out += Theme::c("selected_bg") + Theme::c("selected_fg");
				else if (i > 2 and not driver) out += Theme::c("inactive_fg");
				else out += Theme::c("main_fg");
				
				out += "[" + cjust(valstr, 20) + "]" + Fx::reset;
				if (redraw and (driver or i <= 2)) mouse_mappings["button_" + valstr] = {cy, cx, 1, 20};
				cy++;
				i++;
			}

			cy++;
			out += Mv::to(++cy, x+3) + Fx::b + Theme::c("hi_fg") + rjust( "↑ ↓", 33, true) + Theme::c("main_fg") + Fx::ub + " | To choose type.";
			out += Mv::to(++cy, x+3) + Fx::b + Theme::c("hi_fg") + rjust("ENTER", 33) + Theme::c("main_fg") + Fx::ub + " | To set type.";
			mouse_mappings["enter"] = {cy, x, 1, 73};
			out += Mv::to(++cy, x+3) + Fx::b + Theme::c("hi_fg") + rjust("ESC or \"q\"", 33) + Theme::c("main_fg") + Fx::ub + " | To abort.";
			mouse_mappings["escape"] = {cy, x, 1, 73};

			out += Fx::reset;
		}

		return (redraw ? Changed : retval);
	}

	int sizeError(const string& key) {
		if (redraw) {
			vector<string> cont_vec;
			cont_vec.push_back(Fx::b + Theme::g("used")[100] + L->dlg_error + Theme::c("main_fg") + Fx::ub);
			cont_vec.push_back("Terminal size to small to" + Fx::reset);
			cont_vec.push_back(L->dlg_display_menu + Fx::reset);

			messageBox = Menu::msgBox{45, 0, cont_vec, "error"};
			Global::overlay = messageBox();
		}

		auto ret = messageBox.input(key);
		if (ret == msgBox::Ok_Yes or ret == msgBox::No_Esc) {
			messageBox.clear();
			return Closed;
		}
		else if (redraw) {
			return Changed;
		}
		return NoChange;
	}

	int signalSend(const string& key) {
		auto& s_pid = (Config::getB("show_detailed") and Config::getI("selected_pid") == 0 ? Config::getI("detailed_pid") : Config::getI("selected_pid"));
		const bool service = Config::getB("proc_services");
		auto& p_name = (Config::getB("show_detailed") ? Proc::detailed.entry.name : Config::getS("selected_name"));
		bool running = service and (Config::getB("show_detailed") ? Proc::detailed.status : Config::getS("selected_status")) != "Stopped";
		if (not service and s_pid == 0) return Closed;
		if (redraw) {
			atomic_wait(Runner::active);
			vector<string> cont_vec;
			if (service) {
				string action = running ? L->btn_stop : L->btn_start;
				cont_vec = {
					Fx::b + Theme::c("main_fg") + action + ' ' + Fx::ub + Theme::c("hi_fg") + uresize(p_name, 30) + Fx::reset,
				};
				messageBox = Menu::msgBox{ 50, 1, cont_vec, str_to_lower(action)};
			}
			else {
				cont_vec = {
					Fx::b + Theme::c("main_fg") + L->dlg_terminate_pid + Fx::ub + Theme::c("hi_fg") + to_string(s_pid) + Theme::c("main_fg") + " ("
					+ uresize(p_name, 16) + ')' + Fx::reset,
				};
				messageBox = Menu::msgBox{ 50, 1, cont_vec, "terminate" };
			}
			
			Global::overlay = messageBox();
		}
		auto ret = messageBox.input(key);
		if (ret == msgBox::Ok_Yes) {
			signalKillRet = 0;
			if (service) {
				if (auto ret = ServiceCommand(p_name, running ? Tools::SCstop : Tools::SCstart); ret != ERROR_SUCCESS) {
					signalKillRet = ret;
					menuMask.set(SignalReturn);
				}
			}
			else {
				HandleWrapper p(OpenProcess(PROCESS_TERMINATE, false, s_pid));
				if (not TerminateProcess(p(), 1)) {
					signalKillRet = GetLastError();
					menuMask.set(SignalReturn);
				}
			}
			messageBox.clear();
			return Closed;
		}
		else if (ret == msgBox::No_Esc) {
			messageBox.clear();
			return Closed;
		}
		else if (ret == msgBox::Select) {
			Global::overlay = messageBox();
			return Changed;
		}
		else if (redraw) {
			return Changed;
		}
		return NoChange;
	}

	int signalReturn(const string& key) {
		if (redraw) {
			vector<string> cont_vec;
			cont_vec.push_back(Fx::b + Theme::g("used")[100] + L->dlg_failure + Theme::c("main_fg") + Fx::ub);
			if (signalKillRet == ERROR_ACCESS_DENIED) {
				cont_vec.push_back(L->err_access_denied + Fx::reset);
			}
			else if (signalKillRet == ERROR_INVALID_HANDLE) {
				cont_vec.push_back(L->err_no_handle + Fx::reset);
			}
			else if (signalKillRet == ERROR_INVALID_FUNCTION) {
				cont_vec.push_back(L->err_something_wrong + Fx::reset);
			}
			else if (signalKillRet == ERROR_INVALID_SERVICE_CONTROL) {
				cont_vec.push_back(L->err_invalid_service + Fx::reset);
			}
			else {
				cont_vec.push_back("Unknown error! (errno: " + to_string(signalKillRet) + ')' + Fx::reset);
			}

			messageBox = Menu::msgBox{50, 0, cont_vec, "error"};
			Global::overlay = messageBox();
		}

		auto ret = messageBox.input(key);
		if (ret == msgBox::Ok_Yes or ret == msgBox::No_Esc) {
			messageBox.clear();
			return Closed;
		}
		else if (redraw) {
			return Changed;
		}
		return NoChange;
	}

	int signalPause(const string& key) {
		auto& p_name = Proc::detailed.entry.name;
		bool paused = Proc::detailed.status == "Paused";
		if (redraw) {
			atomic_wait(Runner::active);
			vector<string> cont_vec;
			string action = paused ? L->btn_continue : L->btn_pause;
			cont_vec = {
				Fx::b + Theme::c("main_fg") + action + ' ' + Fx::ub + Theme::c("hi_fg") + uresize(p_name, 30) + Fx::reset,
			};
			messageBox = Menu::msgBox{ 50, 1, cont_vec, str_to_lower(action) };

			Global::overlay = messageBox();
		}
		auto ret = messageBox.input(key);
		if (ret == msgBox::Ok_Yes) {
			signalKillRet = 0;
			
			if (auto ret = ServiceCommand(p_name, paused ? Tools::SCcontinue : Tools::SCpause); ret != ERROR_SUCCESS) {
				signalKillRet = ret;
				menuMask.set(SignalReturn);
			}
			
			messageBox.clear();
			return Closed;
		}
		else if (ret == msgBox::No_Esc) {
			messageBox.clear();
			return Closed;
		}
		else if (ret == msgBox::Select) {
			Global::overlay = messageBox();
			return Changed;
		}
		else if (redraw) {
			return Changed;
		}
		return NoChange;
	}

	int mainMenu(const string& key) {
		enum MenuItems { Options, Help, Quit };
		static int y = 0, selected = 0;
		static vector<string> colors_selected;
		static vector<string> colors_normal;
		auto& tty_mode = Config::getB("tty_mode");
		if (bg.empty()) selected = 0;
		int retval = Changed;

		if (redraw) {
			y = Term::height/2 - 10;
			bg = Draw::banner_gen(y, 0, true);
			if (not tty_mode) {
				colors_selected = {
					Theme::hex_to_color(Global::Banner_src.at(0).at(0)),
					Theme::hex_to_color(Global::Banner_src.at(2).at(0)),
					Theme::hex_to_color(Global::Banner_src.at(4).at(0))
				};
				colors_normal = {
					Theme::hex_to_color("#CC"),
					Theme::hex_to_color("#AA"),
					Theme::hex_to_color("#80")
				};
			}
		}
		else if (is_in(key, "escape", "q", "m", "mouse_click")) {
			return Closed;
		}
		else if (key.starts_with("button_")) {
			if (int new_select = key.back() - '0'; new_select == selected)
				goto MainEntering;
			else
				selected = new_select;
		}
		else if (is_in(key, "enter", "space")) {
			MainEntering:
			switch (selected) {
				case Options:
					menuMask.set(Menus::Options);
					currentMenu = Menus::Options;
					return Switch;
				case Help:
					menuMask.set(Menus::Help);
					currentMenu = Menus::Help;
					return Switch;
				case Quit:
					clean_quit(0);
			}
		}
		else if (is_in(key, "down", "tab", "mouse_scroll_down", "j")) {
			if (++selected > 2) selected = 0;
		}
		else if (is_in(key, "up", "shift_tab", "mouse_scroll_up", "k")) {
			if (--selected < 0) selected = 2;
		}
		else {
			retval = NoChange;
		}


		if (retval == Changed) {
			auto& out = Global::overlay;
			out = bg + Fx::reset + Fx::b;
			auto cy = y + 7;
			for (const auto& i : iota(0, 3)) {
				if (tty_mode) out += (i == selected ? Theme::c("hi_fg") : Theme::c("main_fg"));
				const auto& menu = (not tty_mode and i == selected ? menu_selected[i] : menu_normal[i]);
				const auto& colors = (i == selected ? colors_selected : colors_normal);
				if (redraw) mouse_mappings["button_" + to_string(i)] = {cy, Term::width/2 - menu_width[i]/2, 3, menu_width[i]};
				for (int ic = 0; const auto& line : menu) {
					out += Mv::to(cy++, Term::width/2 - menu_width[i]/2) + (tty_mode ? "" : colors[ic++]) + line;
				}
			}
			out += Fx::reset;
		}


		return (redraw ? Changed : retval);
	}

	int optionsMenu(const string& key) {
		enum Predispositions { isBool, isInt, isString, is2D, isBrowseable, isEditable};
		static int y = 0, x = 0, height = 0, page = 0, pages = 0, selected = 0, select_max = 0, item_height = 0, selected_cat = 0, max_items = 0, last_sel = 0;
		static bool editing = false;
		static Draw::TextEdit editor;
		static string warnings;
		static bitset<8> selPred;
		static const unordered_flat_map<string, std::reference_wrapper<const vector<string>>> optionsList = {
			{"color_theme", std::cref(Theme::themes)},
			{"log_level", std::cref(Logger::log_levels)},
			{"temp_scale", std::cref(Config::temp_scales)},
			{"proc_sorting", std::cref(Proc::sort_vector)},
			{"services_sorting", std::cref(Proc::sort_vector_service)},
			{"graph_symbol", std::cref(Config::valid_graph_symbols)},
			{"graph_symbol_cpu", std::cref(Config::valid_graph_symbols_def)},
			{"graph_symbol_mem", std::cref(Config::valid_graph_symbols_def)},
			{"graph_symbol_net", std::cref(Config::valid_graph_symbols_def)},
			{"graph_symbol_proc", std::cref(Config::valid_graph_symbols_def)},
			{"cpu_graph_upper", std::cref(Cpu::available_fields)},
			{"cpu_graph_lower", std::cref(Cpu::available_fields)},
			{"cpu_sensor", std::cref(Cpu::available_sensors)},
			{"selected_gpu", std::cref(Config::available_gpus)}
		};
		auto& tty_mode = Config::getB("tty_mode");
		auto& vim_keys = Config::getB("vim_keys");
		if (max_items == 0) {
			for (const auto& cat : get_categories()) {
				if ((int)cat.size() > max_items) max_items = cat.size();
			}
		}
		if (bg.empty()) {
			page = selected = selected_cat = last_sel = 0;
			redraw = true;
			Theme::updateThemes();
		}
		int retval = Changed;
		bool recollect = false;
		bool screen_redraw = false;
		bool theme_refresh = false;

		//? Draw background if needed else process input
		if (redraw) {
			mouse_mappings.clear();
			selPred.reset();
			y = max(1, Term::height/2 - 3 - max_items);
			x = Term::width/2 - 39;
			height = min(Term::height - 7, max_items * 2 + 4);
			if (height % 2 != 0) height--;
			bg 	= Draw::banner_gen(y, 0, true)
				+ Draw::createBox(x, y + 6, 78, height, Theme::c("hi_fg"), true, "tab" + Symbols::right)
				+ Mv::to(y+8, x) + Theme::c("hi_fg") + Symbols::div_left + Theme::c("div_line") + Symbols::h_line * 29
				+ Symbols::div_up + Symbols::h_line * (78 - 32) + Theme::c("hi_fg") + Symbols::div_right
				+ Mv::to(y+6+height - 1, x+30) + Symbols::div_down + Theme::c("div_line");
			for (const auto& i : iota(0, height - 4)) {
				bg += Mv::to(y+9 + i, x + 30) + Symbols::v_line;
			}
		}
		else if (not warnings.empty() and not key.empty()) {
			auto ret = messageBox.input(key);
			if (ret == msgBox::msgReturn::Ok_Yes or ret == msgBox::msgReturn::No_Esc) {
				warnings.clear();
				messageBox.clear();
			}
		}
		else if (editing and not key.empty()) {
			if (is_in(key, "escape", "mouse_click")) {
				editor.clear();
				editing = false;
			}
			else if (key == "enter") {
				const auto& option = get_categories()[selected_cat][item_height * page + selected][0];
				if (selPred.test(isString) and Config::stringValid(option, editor.text)) {
					Config::set(option, editor.text);
					if (option == "custom_cpu_name") screen_redraw = true;
					else if (is_in(option, "shown_boxes", "presets")) {
						screen_redraw = true;
						Config::current_preset = -1;
					}
					else if (option == "clock_format") {
						Draw::update_clock(true);
						screen_redraw = true;
					}
				}
				else if (selPred.test(isInt) and Config::intValid(option, editor.text)) {
					Config::set(option, stoi(editor.text));
				}
				else
					warnings = Config::validError;

				editor.clear();
				editing = false;
			}
			else if (not editor.command(key))
				retval = NoChange;
		}
		else if (key == "mouse_click") {
			const auto [mouse_x, mouse_y] = Input::mouse_pos;
			if (mouse_x < x or mouse_x > x + 80 or mouse_y < y + 6 or mouse_y > y + 6 + height) {
				return Closed;
			}
			else if (mouse_x < x + 30 and mouse_y > y + 8) {
				auto m_select = ceil((double)(mouse_y - y - 8) / 2) - 1;
				if (selected != m_select)
					selected = m_select;
				else if (selPred.test(isEditable))
					goto mouseEnter;
				else retval = NoChange;
			}
		}
		else if (is_in(key, "enter", "e", "E") and selPred.test(isEditable)) {
			mouseEnter:
			const auto& option = get_categories()[selected_cat][item_height * page + selected][0];
			editor = Draw::TextEdit{Config::getAsString(option), selPred.test(isInt)};
			editing = true;
			mouse_mappings.clear();
		}
		else if (is_in(key, "escape", "q", "o", "backspace")) {
			return Closed;
		}
		else if (is_in(key, "down", "mouse_scroll_down") or (vim_keys and key == "j")) {
			if (++selected > select_max or selected >= item_height) {
				if (page < pages - 1) page++;
				else if (pages > 1) page = 0;
				selected = 0;
			}
		}
		else if (is_in(key, "up", "mouse_scroll_up") or (vim_keys and key == "k")) {
			if (--selected < 0) {
				if (page > 0) page--;
				else if (pages > 1) page = pages - 1;

				selected = item_height - 1;
			}
		}
		else if (pages > 1 and key == "page_down") {
			if (++page >= pages) page = 0;
			selected = 0;
		}
		else if (pages > 1 and key == "page_up") {
			if (--page < 0) page = pages - 1;
			selected = 0;
		}
		else if (key == "tab") {
			if (++selected_cat >= (int)get_categories().size()) selected_cat = 0;
			page = selected = 0;
		}
		else if (key == "shift_tab") {
			if (--selected_cat < 0) selected_cat = (int)get_categories().size() - 1;
			page = selected = 0;
		}
		else if (is_in(key, "1", "2", "3", "4", "5") or key.starts_with("select_cat_")) {
			selected_cat = key.back() - '0' - 1;
			page = selected = 0;
		}
		else if (is_in(key, "left", "right") or (vim_keys and is_in(key, "h", "l"))) {
			const auto& option = get_categories()[selected_cat][item_height * page + selected][0];
			if (selPred.test(isInt)) {
				const int mod = (option == "update_ms" ? 100 : 1);
				long value = Config::getI(option);
				if (key == "right" or (vim_keys and key == "l")) value += mod;
				else value -= mod;

				if (Config::intValid(option, to_string(value)))
					Config::set(option, static_cast<int>(value));
				else {
					warnings = Config::validError;
				}
			}
			else if (selPred.test(isBool)) {
				Config::flip(option);
				screen_redraw = true;
				if (option == "truecolor") {
					theme_refresh = true;
					Config::flip("lowcolor");
				}
				else if (option == "force_tty") {
					theme_refresh = true;
					Config::flip("tty_mode");
				}
				else if (is_in(option, "rounded_corners", "theme_background"))
					theme_refresh = true;
				else if (option == "background_update") {
					Runner::pause_output = false;
				}
				else if (option == "base_10_sizes") {
					recollect = true;
				}
			}
			else if (selPred.test(isBrowseable)) {
				auto& optList = optionsList.at(option).get();
				int i = v_index(optList, Config::getS(option));

				if ((key == "right" or (vim_keys and key == "l")) and ++i >= (int)optList.size()) i = 0;
				else if ((key == "left" or (vim_keys and key == "h")) and --i < 0) i = optList.size() - 1;
				Config::set(option, optList.at(i));

				if (option == "color_theme")
					theme_refresh = true;
				else if (option == "log_level") {
					Logger::set(optList.at(i));
					Logger::info("Logger set to " + optList.at(i));
				}
				else if (is_in(option, "proc_sorting", "services_sorting", "cpu_sensor") or option.starts_with("graph_symbol") or option.starts_with("cpu_graph_"))
					screen_redraw = true;
			}
			else
				retval = NoChange;
		}
		else {
			retval = NoChange;
		}

		//? Draw the menu
		if (retval == Changed) {
			Config::unlock();
			auto& out = Global::overlay;
			out = bg;
			item_height = min((int)get_categories()[selected_cat].size(), (int)floor((double)(height - 4) / 2));
			pages = ceil((double)get_categories()[selected_cat].size() / item_height);
			if (page > pages - 1) page = pages - 1;
			select_max = min(item_height - 1, (int)get_categories()[selected_cat].size() - 1 - item_height * page);
			if (selected > select_max) {
				selected = select_max;
			}

			//? Get variable properties for currently selected option
			if (selPred.none() or last_sel != (selected_cat << 8) + selected) {
				selPred.reset();
				last_sel = (selected_cat << 8) + selected;
				const auto& selOption = get_categories()[selected_cat][item_height * page + selected][0];
				if (Config::ints.contains(selOption))
					selPred.set(isInt);
				else if (Config::bools.contains(selOption))
					selPred.set(isBool);
				else
					selPred.set(isString);

				if (not selPred.test(isString))
					selPred.set(is2D);
				else if (optionsList.contains(selOption)) {
					selPred.set(isBrowseable);
				}
				if (not selPred.test(isBrowseable) and (selPred.test(isString) or selPred.test(isInt)))
					selPred.set(isEditable);
			}

			//? Category buttons
			out += Mv::to(y+7, x+4);
			for (int i = 0; const auto& m : {L->cat_general, L->cat_cpu, L->cat_mem, L->cat_net, L->cat_proc}) {
				out += Fx::b + (i == selected_cat
						? Theme::c("hi_fg") + '[' + Theme::c("title") + m + Theme::c("hi_fg") + ']'
						: Theme::c("hi_fg") + to_string(i + 1) + Theme::c("title") + m + ' ')
					+ Mv::r(10);
				if (string button_name = "select_cat_" + to_string(i + 1); not editing and not mouse_mappings.contains(button_name))
					mouse_mappings[button_name] = {y+6, x+2 + 15*i, 3, 15};
				i++;
			}
			if (pages > 1) {
				out += Mv::to(y+6 + height - 1, x+2) + Theme::c("hi_fg") + Symbols::title_left_down + Fx::b + Symbols::up + Theme::c("title") + " page "
					+ to_string(page+1) + '/' + to_string(pages) + ' ' + Theme::c("hi_fg") + Symbols::down + Fx::ub + Symbols::title_right_down;
			}
			//? Option name and value
			auto cy = y+9;
			for (int c = 0, i = max(0, item_height * page); c++ < item_height and i < (int)get_categories()[selected_cat].size(); i++) {
				const auto& option = get_categories()[selected_cat][i][0];
				const auto& value = (option == "color_theme" ? fs::path(Config::getS("color_theme")).stem().string() : Config::getAsString(option));

				out += Mv::to(cy++, x + 1) + (c-1 == selected ? Theme::c("selected_bg") + Theme::c("selected_fg") : Theme::c("title"))
					+ Fx::b + cjust(capitalize(s_replace(option, "_", " "))
						+ (c-1 == selected and selPred.test(isBrowseable)
							? ' ' + to_string(v_index(optionsList.at(option).get(), (option == "color_theme" ? Config::getS("color_theme") : value)) + 1) + '/' + to_string(optionsList.at(option).get().size())
							: ""), 29);
				out	+= Mv::to(cy++, x + 1) + (c-1 == selected ? "" : Theme::c("main_fg")) + Fx::ub + "  "
					+ (c-1 == selected and editing ? cjust(editor(24), 34, true) : cjust(value, 25, true)) + "  ";

				if (c-1 == selected) {
					if (not editing and (selPred.test(is2D) or selPred.test(isBrowseable))) {
						out += Fx::b + Mv::to(cy-1, x+2) + Symbols::left + Mv::to(cy-1, x+28) + Symbols::right;
						mouse_mappings["left"] = {cy-2, x, 2, 5};
						mouse_mappings["right"] = {cy-2, x+25, 2, 5};
					}
					if (selPred.test(isEditable)) {
						out += Fx::b + Mv::to(cy-1, x+28 - (not editing and selPred.test(isInt) ? 2 : 0)) + (tty_mode ? "E" : Symbols::enter);
					}
					//? Description of selected option
					out += Fx::reset + Theme::c("title") + Fx::b;
					for (int cyy = y+7; const auto& desc : get_categories()[selected_cat][i]) {
						if (cyy++ == y+7) continue;
						else if (cyy == y+10) out += Theme::c("main_fg") + Fx::ub;
						else if (cyy > y + height + 4) break;
						out += Mv::to(cyy, x+32) + desc;
					}
				}
			}

			if (not warnings.empty()) {
				messageBox = msgBox{min(78, (int)ulen(warnings) + 10), msgBox::BoxTypes::OK, {uresize(warnings, 74)}, L->dlg_warning};
				out += messageBox();
			}

			out += Fx::reset;
		}

		if (theme_refresh) {
			Theme::setTheme();
			Draw::banner_gen(0, 0, false, true);
			screen_redraw = true;
			redraw = true;
			optionsMenu("");
		}
		if (screen_redraw) {
			auto overlay_bkp = move(Global::overlay);
			auto clock_bkp = move(Global::clock);
			Draw::calcSizes();
			Global::overlay = move(overlay_bkp);
			Global::clock = move(clock_bkp);
			recollect = true;
		}
		if (recollect) {
			Runner::run("all", false, true);
			retval = NoChange;
		}

		return (redraw ? Changed : retval);
	}

	int helpMenu(const string& key) {
		static int y = 0, x = 0, height = 0, page = 0, pages = 0;
		if (bg.empty()) page = 0;
		int retval = Changed;

		if (redraw) {
			y = max(1, Term::height/2 - 4 - (int)(get_help_text().size() / 2));
			x = Term::width/2 - 39;
			height = min(Term::height - 6, (int)get_help_text().size() + 3);
			pages = ceil((double)get_help_text().size() / (height - 3));
			page = 0;
			bg = Draw::banner_gen(y, 0, true);
			bg += Draw::createBox(x, y + 6, 78, height, Theme::c("hi_fg"), true, L->dlg_help);
		}
		else if (is_in(key, "escape", "q", "h", "backspace", "space", "enter", "mouse_click")) {
			return Closed;
		}
		else if (pages > 1 and is_in(key, "down", "page_down", "tab", "mouse_scroll_down")) {
			if (++page >= pages) page = 0;
		}
		else if (pages > 1 and is_in(key, "up", "page_up", "shift_tab", "mouse_scroll_up")) {
			if (--page < 0) page = pages - 1;
		}
		else {
			retval = NoChange;
		}


		if (retval == Changed) {
			auto& out = Global::overlay;
			out = bg;
			if (pages > 1) {
				out += Mv::to(y+height+6, x + 2) + Theme::c("hi_fg") + Symbols::title_left_down + Fx::b + Symbols::up + Theme::c("title") + " page "
					+ to_string(page+1) + '/' + to_string(pages) + ' ' + Theme::c("hi_fg") + Symbols::down + Fx::ub + Symbols::title_right_down;
			}
			auto cy = y+7;
			out += Mv::to(cy++, x + 1) + Theme::c("title") + Fx::b + cjust(L->help_key, 20) + "Description:";
			for (int c = 0, i = max(0, (height - 3) * page); c++ < height - 3 and i < (int)get_help_text().size(); i++) {
				out += Mv::to(cy++, x + 1) + Theme::c("hi_fg") + Fx::b + cjust(get_help_text()[i][0], 20)
					+ Theme::c("main_fg") + Fx::ub + get_help_text()[i][1];
			}
			out += Fx::reset;
		}


		return (redraw ? Changed : retval);
	}

	//* Add menus here and update enum Menus in header
	const auto menuFunc = vector{
		ref(sizeError),
		ref(signalConfig),
		ref(signalSend),
		ref(signalPause),
		ref(signalReturn),
		ref(optionsMenu),
		ref(helpMenu),
		ref(mainMenu),
	};
	bitset<8> menuMask;

	void process(string key) {
		if (menuMask.none()) {
			Menu::active = false;
			Global::overlay.clear();
			Global::overlay.shrink_to_fit();
			Runner::pause_output = false;
			bg.clear();
			bg.shrink_to_fit();
			currentMenu = -1;
			Runner::run("all", true, true);
			mouse_mappings.clear();
			return;
		}

		if (currentMenu < 0 or not menuMask.test(currentMenu)) {
			Menu::active = true;
			redraw = true;
			if (((menuMask.test(Main) or menuMask.test(Options) or menuMask.test(Help) /*or menuMask.test(SignalChoose)*/)
			and (Term::width < 80 or Term::height < 24))
			or (Term::width < 50 or Term::height < 20)) {
				menuMask.reset();
				menuMask.set(SizeError);
			}

			for (const auto& i : iota(0, (int)menuMask.size())) {
				if (menuMask.test(i)) currentMenu = i;
			}

		}

		auto retCode = menuFunc.at(currentMenu)(key);
		if (retCode == Closed) {
			menuMask.reset(currentMenu);
			mouse_mappings.clear();
			bg.clear();
			Runner::pause_output = false;
			process();
		}
		else if (redraw) {
			redraw = false;
			Runner::run("all", true, true);
		}
		else if (retCode == Changed)
			Runner::run("overlay");
		else if (retCode == Switch) {
			Runner::pause_output = false;
			bg.clear();
			redraw = true;
			mouse_mappings.clear();
			process();
		}
	}

	void show(int menu) {
		menuMask.set(menu);
		process();
	}
}
