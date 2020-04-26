#include "src/shared_crt_impl.hpp"
#include "src/stream.h"
#include "src/tokenizer.cpp"
#include <ctype.h>

static FILE *header;
static FILE *source;

String read_entire_file(const String file) {
	String content;

	FILE *handle = fopen(tto_cstring(file), "rb");
	if (!handle) return content;
	defer {
		fclose(handle);
	};

	fseek(handle, 0, SEEK_END);
	auto size = ftell(handle);
	fseek(handle, 0, SEEK_SET);
	content.data  = new u8[size];
	content.count = size;
	fread(content.data, size, 1, handle);

	return content;
}

char *upper_caps(const String string) {
	char *result = (char *)tallocate(string.count + 1);
	memcpy(result, string.data, string.count);
	result[string.count] = 0;

	for (s64 i = 0; i < string.count; ++i) {
		result[i] = toupper(result[i]);
		if (result[i] == ' ' || result[i] == '-') result[i] = '_';
	}

	return result;
}

char *lower_caps(const String string) {
	char *result = (char *)tallocate(string.count + 1);
	memcpy(result, string.data, string.count);
	result[string.count] = 0;

	for (s64 i = 0; i < string.count; ++i) {
		result[i] = tolower(result[i]);
		if (result[i] == ' ' || result[i] == '-') result[i] = '_';
	}

	return result;
}

struct Fmt_Range_Type_Category {
	bool   range;
	String first;
	String last;
	String type;
	bool   has_category;
	String category;
};

bool parse_for_fmt_range_type_category(const char *file, String line, Fmt_Range_Type_Category *parsed) {
	if (line.count < 15) return false;

	bool   range = false, has_category = false;
	String first, last;
	String type, category;

	auto period_find    = string_isearch(line, ".");
	auto space_find     = string_isearch(line, " ");
	auto semicolon_find = string_isearch(line, ";");
	auto hash_find      = string_isearch(line, "#");

	if (period_find.found) {
		first     = string_substring(line, 0, period_find.start_index);
		s64 count = space_find.start_index - period_find.start_index - 2;
		last      = string_substring(line, period_find.start_index + 2, count);
		if (last[last.count - 1] == ';') last.count -= 1;
		range = true;
	} else {
		first = string_substring(line, 0, space_find.start_index);
		last  = first;
	}

	if (!semicolon_find.found) return false;

	if (hash_find.found) {
		s64 type_count = hash_find.start_index - semicolon_find.start_index - 3;
		if (type_count < 0) return false;
		if (hash_find.start_index + 3 >= line.count) return false;

		has_category = true;
		type         = string_substring(line, semicolon_find.start_index + 2, type_count);
		category     = string_substring(line, hash_find.start_index + 1, 2);
	} else {
		s64 type_count = line.count - semicolon_find.start_index - 2;
		if (type_count < 0) return false;
		type = string_substring(line, semicolon_find.start_index + 2, type_count);
	}

	parsed->range        = range;
	parsed->first        = first;
	parsed->last         = last;
	parsed->type         = type;
	parsed->has_category = has_category;
	parsed->category     = category;

	return true;
}

void ucd_generate_fmt_range_type_category(const char *file_name, const char *name) {
	const String file    = tprintf("../../res/ucd/%s", file_name);
	String       content = read_entire_file(file);
	if (content) {
		auto stream = istream(content);

		auto lower_caps_name = lower_caps(String(name, strlen(name)));

		fprintf(header, "enum %s : uint32_t {\n", name);
		fprintf(header, "\t%s_UNKNOWN,\n", name);

		fprintf(source, "%s %s(uint32_t codepoint) {\n", name, lower_caps_name);

		Array<String> found_types;
		defer {
			array_free(&found_types);
		};

		while (!istream_eof(&stream)) {
			String line = istream_consume_line(&stream);
			if (line && line[0] != '#') {
				scoped_temporary_allocation();

				Fmt_Range_Type_Category parsed;
				if (parse_for_fmt_range_type_category(file_name, line, &parsed)) {
					char *type = upper_caps(parsed.type);

					s64 index = -1;
					for (s64 i = 0; i < found_types.count; ++i) {
						if (string_match(found_types[i], parsed.type)) {
							index = i;
							break;
						}
					}

					if (index == -1) {
						fprintf(header, "\t%s_%s,\n", name, type);
						array_add(&found_types, parsed.type);
					}

					if (parsed.range == false) {
						fprintf(source, "\tif (codepoint == 0x%s) return %s_%s;\n", tto_cstring(parsed.first), name, type);
					} else {
						fprintf(source, "\tif (codepoint >= 0x%s && codepoint <= 0x%s) return %s_%s;\n",
								tto_cstring(parsed.first), tto_cstring(parsed.last), name, type);
					}
				}
			}
		}

		fprintf(source, "\n\treturn %s_UNKNOWN;\n}\n\n", name);

		fprintf(header, "\t%s_COUNT,\n};\n\n", name);
		fprintf(header, "%s %s(uint32_t codepoint);\n\n", name, lower_caps_name);
	}
}

int system_main() {
	header = fopen("../../src/ucd.h", "wb");
	source = fopen("../../src/ucd.cpp", "wb");

	fprintf(header, "#pragma once\n");
	fprintf(header, "#include <stdint.h>\n\n");
	fprintf(source, "#include \"ucd.h\"\n\n");

	ucd_generate_fmt_range_type_category("Scripts.txt", "Ucd_Script");
	ucd_generate_fmt_range_type_category("ScriptExtensions.txt", "Ucd_Script_Ext");
	ucd_generate_fmt_range_type_category("Blocks.txt", "Ucd_Block");
	ucd_generate_fmt_range_type_category("PropList.txt", "Ucd_Prop");
	ucd_generate_fmt_range_type_category("IndicSyllabicCategory.txt", "Ucd_Indic_Syllable");
	ucd_generate_fmt_range_type_category("IndicPositionalCategory.txt", "Ucd_Indic_Position");
	ucd_generate_fmt_range_type_category("auxiliary/GraphemeBreakProperty.txt", "Ucd_Grapheme_Property");

	fclose(header);
	fclose(source);

	return 0;
}
