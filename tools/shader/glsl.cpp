#include "shader.h"

#include "modules/core/utility.cpp"
#include "modules/core/reflection.h"
#include "modules/core/stream.h"
#include ".generated/utility.typeinfo"

Token parser_expect_token_kind(Array_View<Token> tokens, s64 index, Token_Kind kind, int *error_count, const char *file) {
	if (index < tokens.count) {
		const auto &token = tokens[index];
		if (token.kind != kind) {
			*error_count += 1;
			report_error("GLSL", "Parsing error: %s (%zu, %zu): Expected %s but found %s", file, token.row, token.column, enum_string(kind).data, enum_string(token.kind).data);
		}
		return token;
	} else {
		auto token = tokens[tokens.count - 1];
		report_error("GLSL", "Parsing error: %s (%zu, %zu): Expected %s but end of file reached", file, token.row, token.column, enum_string(kind).data);
		token.kind   = Token_Kind_NONE;
		token.row    = MAX_UINT64;
		token.column = MAX_UINT64;
		return token;
	}
}

bool glsl_compile_shader(const char *name, String src, int version, Shader_Code *out, const char *vs_define, const char *ps_define) {
	Tokenization_Status status;
	auto                tokens = tokenize(src, &status);
	defer {
		memory_free(tokens.data);
	};

	if (status.result != Tokenization_Result_SUCCESS) {
		const char *err_msg = "";
		if (status.result == Tokenization_Result_ERROR_COMMENT_BLOCK_NO_END)
			err_msg = "Comment Block not ended";
		else if (status.result == Tokenization_Result_ERROR_NO_MATCHING_PARENTHESIS)
			err_msg = "Matching parenthesis not found";

		report_error("GLSL", "Failed parsing %s (%zu, %zu): %s", name, status.row, status.column, err_msg);
		return false;
	}

	struct Shared {
		const char *type;
		const char *name;
	};

	Array<Shared> shared;

	String vertex_code   = {};
	String fragment_code = {};

	int double_colons = 0;

	defer {
		array_free(&shared);
	};

	Token token = {};
	for (s64 index = 0; index < tokens.count; ++index) {
		token = tokens[index];

		if (double_colons) {
			switch (token.kind) {
				case Token_Kind_DOUBLE_COLON: {
					if (double_colons == 2) break;

					double_colons += 1;
					vertex_code.count = token.content.data - vertex_code.data - 1;

					int error_count = 0;

					auto id = parser_expect_token_kind(tokens, ++index, Token_Kind_IDENTIFIER, &error_count, name);
					if (error_count == 0) {
						if (!string_match(id.content, "Fragment"))
							report_error("GLSL", "Parsing error: %zu:%zu Expected 'Fragment', Got '%s'", id.row, id.column, tto_cstring(id.content));
						fragment_code.data = id.content.data + id.content.count;
					}
				} break;
			}

		} else {
			switch (token.kind) {
				case Token_Kind_AT: {
					int error_count = 0;

					auto type_id = parser_expect_token_kind(tokens, ++index, Token_Kind_IDENTIFIER, &error_count, name);
					auto name_id = parser_expect_token_kind(tokens, ++index, Token_Kind_IDENTIFIER, &error_count, name);
					parser_expect_token_kind(tokens, ++index, Token_Kind_SEMICOLON, &error_count, name);

					if (error_count == 0) {
						Shared s;
						s.name = tto_cstring(name_id.content);
						s.type = tto_cstring(type_id.content);
						array_add(&shared, s);
					}
				} break;

				case Token_Kind_DOUBLE_COLON: {
					double_colons = 1;

					int error_count = 0;

					auto id = parser_expect_token_kind(tokens, ++index, Token_Kind_IDENTIFIER, &error_count, name);

					if (error_count == 0) {
						if (!string_match(id.content, "Vertex")) {
							report_error("GLSL", "Parsing error: %zu:%zu Expected 'Vertex', Got '%s'", id.row, id.column, tto_cstring(id.content));
						}
						vertex_code.data = id.content.data + id.content.count;
					}
				} break;
			}
		}
	}

	if (fragment_code.data) {
		fragment_code.count = token.content.data - fragment_code.data;
	}

	String header = tprintf("#version %d\n", version);

	Ostream stream;

	ostream_write_buffer(&stream, header.data, header.count);
	if (vs_define) {
		ostream_write_formatted(&stream, "\n#define %s\n", vs_define);
	}
	for (auto &s : shared) {
		ostream_write_formatted(&stream, "out %s %s;\n", s.type, s.name);
	}
	ostream_write_buffer(&stream, vertex_code.data, vertex_code.count);
	out->vertex = ostream_build_string(&stream);
	ostream_free(&stream);

	ostream_write_buffer(&stream, header.data, header.count);
	if (ps_define) {
		ostream_write_formatted(&stream, "\n#define %s\n", ps_define);
	}
	for (auto &s : shared) {
		ostream_write_formatted(&stream, "in %s %s;\n", s.type, s.name);
	}
	ostream_write_buffer(&stream, fragment_code.data, fragment_code.count);
	out->pixel = ostream_build_string(&stream);
	ostream_free(&stream);

	return true;
}
