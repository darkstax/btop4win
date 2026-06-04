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
