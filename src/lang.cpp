/* Language system implementation */

#include "lang.hpp"
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
		if (*p < 0x80) {
			w += 1;
			p += 1;
		} else if ((*p & 0xE0) == 0xC0) {
			w += 2; // assume CJK
			p += 2;
		} else if ((*p & 0xF0) == 0xE0) {
			w += 2; // CJK wide
			p += 3;
		} else if ((*p & 0xF8) == 0xF0) {
			w += 2;
			p += 4;
		} else {
			w += 1;
			p += 1;
		}
	}
	return w;
}
