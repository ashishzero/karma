#include "shared.h"

#define STBI_ASSERT(x) assert(x)
#define STBI_MALLOC(sz)	mallocate(sz)
#define STBI_REALLOC(p, newsz) mreallocate(p, newsz)
#define STBI_FREE(p) mfree(p)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#include "parser.cpp"

#include "systems.h"
#include "string.h"
#include "stream.h"

void print_lexer_errors(Lexer* lexer) {
	for (int i = 0; i < lexer_errors(lexer); ++i) {
		auto& err = lexer->error[i];
		if (err.kind == Lexer_Error_Kind_COMMENT_BLOCK_NO_END && err.content) {
			system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Expected: %s, Got: %s",
					   err.got.row, err.got.column, tto_cstring(err.content),
					   tto_cstring(err.got.content));
		} else {
			system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Expected: %s, Got: %s",
					   err.got.row, err.got.column, token_kind_string(err.expected),
					   tto_cstring(err.got.content));
		}
	}
	lexer_clear_errors(lexer);
}

int get_glsl_var_type(const String type) {
	if (string_match(type, "float"))
		return 1;
	else if (string_match(type, "vec2"))
		return 2;
	else if (string_match(type, "vec3"))
		return 3;
	else if (string_match(type, "vec4"))
		return 4;
	else if (string_match(type, "mat4"))
		return 16;
	else
		return 0;
}

int get_glsl_texture_type(const String type) {
	if (string_match(type, "sampler2D"))
		return 2;
	else
		return 0;
}

void generate_glsl_shader(const String file, const String out) {
	auto content = system_read_entire_file(file);
	defer { mfree(content.data); };
	if (!content.count) return;

	struct Shared {
		const char* type;
		const char* name;
	};

	struct Attribute {
		int index;
		int count;
	};

	struct Uniform {
		String name;
		int type;
	};

	int version = 330;
	bool depth = 0;
	bool cull = 0;
	bool blend = 0;
	Array<Shared> shared;
	Array<Attribute> attribute;
	Array<Uniform> uniforms;
	Array<Uniform> textures;
	String vertex_code = {};
	String fragment_code = {};

	int double_colons = 0;

	defer {
		array_free(&shared); 
		array_free(&attribute);
		array_free(&uniforms);
		array_free(&textures);
	};

	auto lexer = lexer_init(content);

	Token token;
	for (token = lexer_next_token(&lexer); 
		 token.kind != Token_Kind_END_OF_STREAM;
		 token = lexer_next_token(&lexer)) {

		if (double_colons) {
			switch (token.kind) {
				case Token_Kind_IDENTIFIER:
				{
					if (string_match(token.content, "layout")) {
						
						if (double_colons == 1) {
							lexer_require_token(&lexer, Token_Kind_OPEN_BRACKET);
							lexer_require_identifier(&lexer, "location");
							lexer_require_token(&lexer, Token_Kind_EQUALS);
							auto index_id = lexer_require_token(&lexer, Token_Kind_INTEGER_LITERAL);
							lexer_require_token(&lexer, Token_Kind_CLOSE_BRACKET);
							lexer_require_identifier(&lexer, "in");
							auto type_id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
							lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
							lexer_require_token(&lexer, Token_Kind_SEMICOLON);

							if (lexer_pass(&lexer)) {
								int type = get_glsl_var_type(type_id.content);
								if (type != 0) {
									Attribute a;
									a.count = type;
									a.index = (int)index_id.value.integer;
									array_add(&attribute, a);
								} else {
									system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Unknown type '%s'",
											   tto_cstring(type_id.content));
								}
							}
						}

					} else if (string_match(token.content, "uniform")) {

						auto type_id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
						auto name_id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
						lexer_require_token(&lexer, Token_Kind_SEMICOLON);

						if (lexer_pass(&lexer)) {
							int type = get_glsl_var_type(type_id.content);
							if (type != 0) {
								array_add(&uniforms, Uniform{ name_id.content, type });
							} else {
								type = get_glsl_texture_type(type_id.content);
								if (type != 0) {
									array_add(&textures, Uniform{ name_id.content, type });
								} else {
									system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Unknown type '%s'",
											   tto_cstring(type_id.content));
								}
							}
						}

					}
				} break;

				case Token_Kind_DOUBLE_COLON:
				{
					if (double_colons == 2) break;

					double_colons += 1;
					vertex_code.count = token.content.data - vertex_code.data - 1;

					auto id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
					if (lexer_pass(&lexer)) {
						if (!string_match(id.content, "Fragment"))
							system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Expected 'Fragment', Got '%s'",
									   id.row, id.column, tto_cstring(id.content));

						fragment_code.data = id.content.data + id.content.count;
					}
				} break;
			}

		} else {

			switch (token.kind) {
				case Token_Kind_PERIOD:
				{
					auto id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
					lexer_require_token(&lexer, Token_Kind_EQUALS);
					auto val = lexer_require_token(&lexer, Token_Kind_INTEGER_LITERAL);
					lexer_require_token(&lexer, Token_Kind_SEMICOLON);

					if (lexer_pass(&lexer)) {
						String name = id.content;
						if (string_match(name, "version"))
							version = (int)val.value.integer;
						else if (string_match(name, "depth"))
							depth = val.value.integer;
						else if (string_match(name, "cull"))
							cull = val.value.integer;
						else if (string_match(name, "blend"))
							blend = val.value.integer;
						else
							system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Error unknown identifier '%s'",
									   id.row, id.column, tto_cstring(id.content));
					}
				} break;

				case Token_Kind_AT:
				{
					auto type_id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
					auto name_id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
					lexer_require_token(&lexer, Token_Kind_SEMICOLON);

					if (lexer_pass(&lexer)) {
						Shared s;
						s.name = tto_cstring(name_id.content);
						s.type = tto_cstring(type_id.content);
						array_add(&shared, s);
					}
				} break;

				case Token_Kind_DOUBLE_COLON:
				{
					double_colons = 1;

					auto id = lexer_require_token(&lexer, Token_Kind_IDENTIFIER);
					if (lexer_pass(&lexer)) {
						if (!string_match(id.content, "Vertex"))
							system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Expected 'Vertex', Got '%s'",
									   id.row, id.column, tto_cstring(id.content));

						vertex_code.data = id.content.data + id.content.count;
					}
				} break;

				default:
				{
					system_log(LOG_ERROR, "Prebuild:GLSL", "%zu:%zu Invalid identifier '%s'",
							   token.row, token.column, tto_cstring(token.content));
				} break;
			}
		}

		print_lexer_errors(&lexer);
	}


	if (fragment_code.data) {
		fragment_code.count = token.content.data - fragment_code.data;
	}

	System_File outf;
	if (system_open_file(out, File_Operation_NEW, &outf)) {
		String header = tprintf("#version %d\n", version);

		String vertex, fragment;
		Ostream stream;

		{
			ostream_write_buffer(&stream, header.data, header.count);
			foreach(index, s, shared) {
				ostream_write_formatted(&stream, "out %s %s;\n", s->type, s->name);
			}
			ostream_write_buffer(&stream, vertex_code.data, vertex_code.count);
			vertex = ostream_build_string(&stream);
			ostream_free(&stream);
		}

		{
			ostream_write_buffer(&stream, header.data, header.count);
			foreach(index, s, shared) {
				ostream_write_formatted(&stream, "in %s %s;\n", s->type, s->name);
			}
			ostream_write_buffer(&stream, fragment_code.data, fragment_code.count);
			fragment = ostream_build_string(&stream);
			ostream_free(&stream);
		}

		const u32 DEPTH = 0x1;
		const u32 BLEND = 0x2;
		const u32 CULL = 0x4;

		u32 flags = 0;
		if (depth) flags |= DEPTH;
		if (blend) flags |= BLEND;
		if (cull) flags |= CULL;

		outf.write(outf.handle, &flags, sizeof(flags));

		outf.write(outf.handle, &attribute.count, sizeof(attribute.count));
		foreach(index, a, attribute) {
			outf.write(outf.handle, a, sizeof(*a));
		}

		outf.write(outf.handle, &vertex.count, sizeof(vertex.count));
		outf.write(outf.handle, vertex.data, vertex.count);
		outf.write(outf.handle, &fragment.count, sizeof(fragment.count));
		outf.write(outf.handle, fragment.data, fragment.count);

		outf.write(outf.handle, &uniforms.count, sizeof(uniforms.count));
		foreach(index, u, uniforms) {
			outf.write(outf.handle, &u->name.count, sizeof(u->name.count));
			outf.write(outf.handle, u->name.data, u->name.count);
			outf.write(outf.handle, &u->type, sizeof(u->type));
		}

		outf.write(outf.handle, &textures.count, sizeof(textures.count));
		foreach(index, u, textures) {
			outf.write(outf.handle, &u->name.count, sizeof(u->name.count));
			outf.write(outf.handle, u->name.data, u->name.count);
			outf.write(outf.handle, &u->type, sizeof(u->type));
		}

		system_close_file(&outf);

		system_log(LOG_INFO, "Prebuild:GLSL", "Exported shader '%s'", tto_cstring(out));
	} else {
		system_log(LOG_ERROR, "Prebuild:GLSL", "Unable to write '%s'", tto_cstring(out));
	}

}

void generate_glsl_shaders(const char* dir, const char* out) {
	auto files = system_find_files(tprintf("%s/shaders/*.glsl", dir), false);

	foreach(index, file, files) {
		generate_glsl_shader(file->path, tprintf("%s/%s.fx.glsl", out, tto_cstring(file->name)));
	}

	foreach(index, file, files) {
		mfree(file->path.data);
	}

	mfree(files.data);
}

void do_pre_build_steps(const char* dir, const char* out) {
	generate_glsl_shaders(dir, out);
	reset_temporary_memory();
}

#endif
