/* Language system implementation */

#include "lang.hpp"
#include "../include/widechar_width.hpp"
#include <cstring>

const Lang* L = &LANG_EN;

void langInit(const std::string& code) {
	if (code == "zh" || code == "zh_CN" || code == "chinese")
		L = &LANG_ZH;
	else
		L = &LANG_EN;
}

const char* langSortName(const std::string& key) {
	if (L == &LANG_ZH) {
		if (key == "pid") return "PID";
		if (key == "name" || key == "program") return "程序";
		if (key == "command" || key == "arguments") return "命令";
		if (key == "threads") return "线程";
		if (key == "user") return "用户";
		if (key == "memory") return "内存";
		if (key == "cpu direct") return "CPU直排";
		if (key == "cpu lazy") return "CPU缓排";
		if (key == "service") return "服务";
		if (key == "caption") return "标题";
		if (key == "status") return "状态";
	}
	return key.c_str();
}

int langDisplayWidth(const char* s) {
	if (!s) return 0;
	int w = 0;
	auto* p = (const unsigned char*)s;
	while (*p) {
		uint32_t cp;
		int len;
		if (*p < 0x80) {
			cp = *p;
			len = 1;
		} else if ((*p & 0xE0) == 0xC0) {
			cp = ((*p & 0x1Fu) << 6) | (*(p + 1) & 0x3Fu);
			len = 2;
		} else if ((*p & 0xF0) == 0xE0) {
			cp = ((*p & 0x0Fu) << 12) | ((*(p + 1) & 0x3Fu) << 6) | (*(p + 2) & 0x3Fu);
			len = 3;
		} else if ((*p & 0xF8) == 0xF0) {
			cp = ((*p & 0x07u) << 18) | ((*(p + 1) & 0x3Fu) << 12) | ((*(p + 2) & 0x3Fu) << 6) | (*(p + 3) & 0x3Fu);
			len = 4;
		} else {
			w += 1;
			p += 1;
			continue;
		}
		w += utf8::wcwidth(cp);
		p += len;
	}
	return w;
}
