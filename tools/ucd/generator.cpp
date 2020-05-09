#define _CRT_SECURE_NO_WARNINGS
#include "src/stream.cpp"
#include "src/karma_crt_impl.hpp"
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
	const String file    = tprintf("res/ucd/%s", file_name);
	String       content = read_entire_file(file);
	defer {
		mfree(content.data);
	};

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

struct Unicode_Data {
	String codepoint;
	String information;
	String category;
	String decomposed;
	String name;
};

bool parse_for_unicode_data(String line, Unicode_Data *data) {
	int terminators[14];

	String_Search_Result find_res;
	int                  search_index = 0;
	for (int put_index = 0; put_index < static_count(terminators); ++put_index) {
		find_res = string_isearch(line, ";", search_index);
		if (find_res.found) {
			terminators[put_index] = (int)find_res.start_index;
			search_index           = (int)find_res.start_index + 1;
		} else {
			return false;
		}
	}

	data->codepoint   = string_substring(line, 0, terminators[0]);
	data->information = string_substring(line, terminators[0] + 1, terminators[1] - terminators[0] - 1);
	data->category    = string_substring(line, terminators[1] + 1, terminators[2] - terminators[1] - 1);
	data->decomposed  = string_substring(line, terminators[4] + 1, terminators[5] - terminators[4] - 1);
	data->name        = string_substring(line, terminators[9] + 1, terminators[10] - terminators[9] - 1);

	return true;
}

void ucd_generate_unicode_data(const char *file_name) {
	const String file    = tprintf("res/ucd/%s", file_name);
	String       content = read_entire_file(file);
	defer {
		mfree(content.data);
	};

	if (!content) return;
	Array<Unicode_Data> unicode_data;
	defer {
		array_free(&unicode_data);
	};

	auto stream = istream(content);

	Unicode_Data data;
	while (!istream_eof(&stream)) {
		if (parse_for_unicode_data(istream_consume_line(&stream), &data)) {
			array_add(&unicode_data, data);
		}
	}

	// Normalization code generation
	{
		scoped_temporary_allocation();
		fprintf(header, "int unicode_normalize(uint32_t codepoint, uint32_t **normals);\n\n");

		fprintf(source, "int unicode_normalize(uint32_t codepoint, uint32_t **normals) {\n");

		Ostream switch_code;
		Ostream normals_code;

		for (auto &unicode : unicode_data) {
			if (unicode.decomposed) {
				char *name_of_decomposed = lower_caps(unicode.information);
				int   number_of_normals  = 1;
				ostream_write_formatted(&normals_code, "\tstatic uint32_t %s [] = { ", name_of_decomposed);
				String_Iter iter;
				while (string_iter_next(unicode.decomposed, &iter)) {
					if (iter.index == 0) ostream_write_formatted(&normals_code, "0x");
					if (iter.codepoint.code == ' ') {
						number_of_normals += 1;
						ostream_write_formatted(&normals_code, ", 0x");
					} else if (iter.codepoint.code == '<') {
						while (string_iter_next(unicode.decomposed, &iter)) {
							if (iter.codepoint.code == ' ') break;
						}
					} else {
						utf8 utf8_string[5];
						utf8_string[utf32_to_utf8(iter.codepoint.code, utf8_string)] = 0;
						ostream_write_formatted(&normals_code, "%s", utf8_string);
					}
				}
				ostream_write_formatted(&normals_code, " };\n");

				ostream_write_formatted(&switch_code, "\t\tcase 0x%s:\n\t\t\t*normals = %s;\n\t\t\treturn %d;\n",
										tto_cstring(unicode.codepoint), name_of_decomposed, number_of_normals);
			}
		}

		String switch_string  = ostream_build_string(&switch_code, true);
		String normals_string = ostream_build_string(&normals_code, true);
		ostream_free(&switch_code);
		ostream_free(&normals_code);

		fprintf(source, "%s", normals_string.data);
		fprintf(source, "\n\tswitch (codepoint) {\n");
		fprintf(source, "%s", switch_string.data);
		fprintf(source, "\t}\n\treturn 0;\n}\n\n");

		mfree(switch_string.data);
		mfree(normals_string.data);
	}

	// Category code generation
	{
		scoped_temporary_allocation();

		Array<String> category;
		defer {
			array_free(&category);
		};


		fprintf(source, "Unicode_Category unicode_category(uint32_t codepoint) {\n");
		fprintf(source, "\tswitch (codepoint) {\n");
		for (auto &unicode : unicode_data) {
			bool category_already_present = false;
			for (auto &cat : category) {
				if (string_match(cat, unicode.category)) {
					category_already_present = true;
					break;
				}
			}
			if (!category_already_present) array_add(&category, unicode.category);

			fprintf(source, "\t\tcase 0x%s: return Unicode_Category_%s;\n", tto_cstring(unicode.codepoint), upper_caps(unicode.category));
		}
		fprintf(source, "\t}\n");
		fprintf(source, "\treturn Unicode_Category_UNKNOWN;\n");
		fprintf(source, "}\n\n");

		fprintf(header, "enum Unicode_Category {\n");
		fprintf(header, "\tUnicode_Category_UNKNOWN,\n");
		for (auto &cat : category) {
			fprintf(header, "\tUnicode_Category_%s,\n", upper_caps(cat));
		}
		fprintf(header, "\tUnicode_Category_COUNT,\n");
		fprintf(header, "};\n");
		fprintf(header, "Unicode_Category unicode_category(uint32_t codepoint);\n\n");
	}
}

int system_main() {
	header = fopen("src/unicode.h", "wb");
	source = fopen("src/unicode.cpp", "wb");

	fprintf(header, "#pragma once\n");
	fprintf(header, "#include <stdint.h>\n\n");
	fprintf(source, "#include \"unicode.h\"\n\n");

	ucd_generate_fmt_range_type_category("Scripts.txt", "Unicode_Script");
	ucd_generate_fmt_range_type_category("ScriptExtensions.txt", "Unicode_Script_Ext");
	ucd_generate_fmt_range_type_category("Blocks.txt", "Unicode_Block");
	ucd_generate_fmt_range_type_category("PropList.txt", "Unicode_Prop");
	ucd_generate_fmt_range_type_category("IndicSyllabicCategory.txt", "Unicode_Indic_Syllable");
	ucd_generate_fmt_range_type_category("IndicPositionalCategory.txt", "Unicode_Indic_Position");
	ucd_generate_fmt_range_type_category("auxiliary/GraphemeBreakProperty.txt", "Unicode_Grapheme_Property");

	ucd_generate_unicode_data("UnicodeData.txt");

	fclose(header);
	fclose(source);

	return 0;
}
