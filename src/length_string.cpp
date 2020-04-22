
#include "string.h"

s64 string_utf8_length(const String str) {
	s64 length = 0;
	for (auto index = 0; index < str.count; ++index)
		if ((str[index] & 0xc0) != 0x80) length += 1;
	return length;
}

Utf32_Codepoint utf8_to_utf32(const utf8 *character) {
	u32 codepoint;
	u8  size_in_bytes;

	if ((*character & 0x80) == 0) {
		codepoint	 = (int)character[0];
		size_in_bytes = 1;
	} else if ((*character & 0xe0) == 0xc0) {
		codepoint = ((int)(character[0] & 0x1f) << 6);
		codepoint |= (int)(character[1] & 0x3f);
		size_in_bytes = 2;
	} else if ((*character & 0xf0) == 0xe0) {
		codepoint = ((int)(character[0] & 0x0f) << 12);
		codepoint |= ((int)(character[1] & 0x3f) << 6);
		codepoint |= (int)(character[2] & 0x3f);
		size_in_bytes = 3;
	} else {
		codepoint = ((int)(character[0] & 0x07) << 18);
		codepoint |= ((int)(character[1] & 0x3f) << 12);
		codepoint |= ((int)(character[2] & 0x3f) << 6);
		codepoint |= (int)(character[3] & 0x3f);
		size_in_bytes = 4;
	}

	return Utf32_Codepoint{ codepoint, size_in_bytes };
}

u8 utf32_to_utf8(utf32 codepoint, utf8 buffer[4]) {
	u8 bytes = 0;

	if (codepoint < 0x0080) {
		buffer[0] = codepoint & 0x007f;
		bytes = 1;
	} else if (codepoint < 0x0800) {
		buffer[0] = ((codepoint >> 6) & 0x1f) | 0xc0;
		buffer[1] = (codepoint & 0x3f) | 0x80;
		bytes = 2;
	} else if (codepoint < 0x10000) {
		buffer[0] = ((codepoint >> 12) & 0x0f) | 0xe0;
		buffer[1] = ((codepoint >> 6) & 0x3f) | 0x80;
		buffer[2] = (codepoint & 0x3f) | 0x80;
		bytes = 3;
	} else {
		buffer[0] = ((codepoint >> 18) & 0xf7);
		buffer[1] = ((codepoint >> 12) & 0x3f) | 0x80;
		buffer[2] = ((codepoint >> 6) & 0x3f) | 0x80;
		buffer[3] = (codepoint & 0x3f) | 0x80;
		bytes = 4;
	}

	return bytes;
}

bool codepoint_is_surrogate(utf32 codepoint) {
	bool high = (codepoint >= HIGH_SURROGATE_CODEPOINT_FIRST && codepoint <= HIGH_SURROGATE_CODEPOINT_LAST);
	bool low  = (codepoint >= LOW_SURROGATE_CODEPOINT_FIRST && codepoint <= LOW_SURROGATE_CODEPOINT_LAST);
	return high || low;
}

bool string_iter_continue(const String string, String_Iter *iter) {
	return iter->index < string.count;
}

bool string_iter_next(const String string, String_Iter *iter) {
	iter->index += iter->codepoint.size_in_bytes;
	if (iter->index < string.count) {
		iter->codepoint = utf8_to_utf32(&string[iter->index]);
		return true;
	} else {
		iter->codepoint = {};
		return false;
	}
}

bool string_match(const String a, const String b) {
	if (a.count != b.count) return false;

	for (s64 i = 0; i < a.count; ++i) {
		if (a[i] != b[i]) return false;
	}

	return true;
}

int string_compare(const String a, const String b) {
	s64 len		= a.count < b.count ? a.count : b.count;
	int compare = 0;

	for (s64 i = 0; i < len; ++i) {
		compare = a[i] - b[i];
		if (compare) return compare;
	}

	return compare;
}

bool string_starts_with(const String a, const String b) {
	if (a.count < b.count) return false;

	for (s64 i = 0; i < b.count; ++i) {
		if (a[i] != b[i]) return false;
	}

	return true;
}

bool string_ends_with(const String a, const String b) {
	if (a.count < b.count) return false;

	s64 start = a.count - b.count;
	for (s64 i = 0; i < b.count; ++i) {
		if (a[i + start] != b[i]) return false;
	}

	return true;
}

String string_substring(const String string, s64 index, s64 count) {
	assert(index + count <= string.count);

	String sub;
	sub.data  = string.data + index;
	sub.count = count;
	return sub;
}

String_Search_Result string_isearch(const String string, const String item) {
	String_Search_Result result = {};

	if (string.count >= item.count) {
		s64 end = string.count - item.count;
		for (s64 i = 0; i < end; ++i) {
			String matcher;
			matcher.data  = string.data + i;
			matcher.count = item.count;
			if (string_match(matcher, item)) {
				result.found	   = true;
				result.start_index = i;
				return result;
			}
		}
	}

	return result;
}

String_Search_Result string_isearch_reverse(const String string, const String item) {
	assert(string.count >= item.count);

	String_Search_Result result;
	result.found = false;

	String matcher;
	s64	start = string.count - item.count;
	for (s64 i = start; i >= 0; --i) {
		matcher.data  = string.data + i;
		matcher.count = item.count;
		if (string_match(matcher, item)) {
			result.found	   = true;
			result.start_index = i;
			return result;
		}
	}

	return result;
}

String sprintf(String string, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) {
	va_list ap;
	va_start(ap, fmt);
	int n = stbsp_vsnprintf((char *)string.data, (int)string.count, fmt, ap);
	va_end(ap);
	return String(string.data, n);
}

String mprintf(ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) {
	String string;

	va_list args1;
	va_start(args1, fmt);
	va_list args2;
	va_copy(args2, args1);
	string.count = stbsp_vsnprintf(0, 0, fmt, args1);
	string.data  = (u8 *)mallocate(string.count + 1);
	va_end(args1);
	stbsp_vsnprintf((char *)string.data, (int)string.count + 1, fmt, args2);
	va_end(args2);

	return string;
}

String tprintf(ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) {
	String string;

	va_list args1;
	va_start(args1, fmt);
	va_list args2;
	va_copy(args2, args1);
	string.count = stbsp_vsnprintf(0, 0, fmt, args1);
	string.data  = (u8 *)tallocate(string.count + 1);
	va_end(args1);
	stbsp_vsnprintf((char *)string.data, (int)string.count + 1, fmt, args2);
	va_end(args2);

	return string;
}

String vsprintf(String string, ANALYSE_PRINTF_FORMAT_STRING(char const *fmt), va_list va) {
	int n = stbsp_vsnprintf((char *)string.data, (int)string.count, fmt, va);
	return String(string.data, n);
}

String vmprintf(ANALYSE_PRINTF_FORMAT_STRING(char const *fmt), va_list va) {
	String string;

	va_list vaa;
	va_copy(vaa, va);
	string.count = stbsp_vsnprintf(0, 0, fmt, vaa);
	string.data  = (u8 *)mallocate(string.count + 1);
	va_end(vaa);
	stbsp_vsnprintf((char *)string.data, (int)string.count + 1, fmt, va);

	return string;
}

String vtprintf(ANALYSE_PRINTF_FORMAT_STRING(char const *fmt), va_list va) {
	String string;

	va_list vaa;
	va_copy(vaa, va);
	string.count = stbsp_vsnprintf(0, 0, fmt, vaa);
	string.data  = (u8 *)tallocate(string.count + 1);
	va_end(vaa);
	stbsp_vsnprintf((char *)string.data, (int)string.count + 1, fmt, va);

	return string;
}

char *tto_cstring(String string) {
	char *cstr = (char *)tallocate(string.count + 1);
	memcpy(cstr, string.data, string.count);
	cstr[string.count] = 0;
	return cstr;
}

char *to_cstring(String string) {
	char *cstr = (char *)mallocate(string.count + 1);
	memcpy(cstr, string.data, string.count);
	cstr[string.count] = 0;
	return cstr;
}

String string_copy(String string) {
	u8 * data = new u8[string.count];
	memcpy(data, string.data, string.count);
	return String(data, string.count);
}

String string_tcopy(String string) {
	u8 * data = (u8 *)tallocate(string.count);
	memcpy(data, string.data, string.count);
	return String(data, string.count);
}

String string_concat(String a, String b) {
	u8 * data = new u8[a.count + b.count];
	memcpy(data, a.data, a.count);
	memcpy(data + a.count, b.data, b.count);
	return String(data, a.count + b.count);
}

String string_concat(Array_View<String> strings) {
	s64 len = 0;
	foreach(index, string, strings) {
		len += string.count;
	}
	u8 * data = new u8[len];

	s64 pos = 0;
	foreach(index, string, strings) {
		memcpy(data + pos, string.data, string.count);
		pos += string.count;
	}
	return String(data, len);
}

String string_tconcat(String a, String b) {
	u8 * data = (u8 *)tallocate(a.count + b.count);
	memcpy(data, a.data, a.count);
	memcpy(data + a.count, b.data, b.count);
	return String(data, a.count + b.count);
}

String string_tconcat(Array_View<String> strings) {
	s64 len = 0;
	foreach(index, string, strings) {
		len += string.count;
	}
	u8 * data = (u8 *)tallocate(len);

	s64 pos = 0;
	foreach(index, string, strings) {
		memcpy(data + pos, string.data, string.count);
		pos += string.count;
	}
	return String(data, len);
}
