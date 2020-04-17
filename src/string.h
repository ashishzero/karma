#pragma once

#include "shared.h"

#define STB_SPRINTF_MIN 512
#include "stb_sprintf.h"

struct Utf32_Codepoint {
	utf32 code;
	u8	size_in_bytes;
};

struct String_Iter {
	Utf32_Codepoint codepoint = {};
	s64		  index		= 0;
};

struct String_Search_Result {
	bool found;
	s64  start_index;
};

s64					 string_utf8_length(const String str);
Utf32_Codepoint		 utf8_to_utf32(const utf8 *character);
u8					 utf32_to_utf8(utf32 codepoint, utf8 buffer[4]);
bool				 string_iter_continue(const String string, String_Iter *iter);
bool				 string_iter_next(const String string, String_Iter *iter);
bool				 string_match(const String a, const String b);
int					 string_compare(const String a, const String b);
bool				 string_starts_with(const String a, const String b);
bool				 string_ends_with(const String a, const String b);
String				 string_substring(const String string, s64 index, s64 count);
String_Search_Result string_isearch(const String string, const String item);
String_Search_Result string_isearch_reverse(const String string, const String item);

String sprintf(String string, const char *fmt, ...);
String mprintf(const char *fmt, ...);
String tprintf(const char *fmt, ...);
String vsprintf(String string, char const *fmt, va_list va);
String vmprintf(char const *fmt, va_list va);
String vtprintf(char const *fmt, va_list va);
char * tto_cstring(String string);
char * to_cstring(String string);
