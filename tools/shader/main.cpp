#define _CRT_SECURE_NO_WARNINGS
#include "src/stream.cpp"
#include "src/karma_crt_impl.hpp"
#include "src/gfx_shaders.h"
#include "shader.h"
#include <ctype.h>

struct Shader_Batch {
	const char *name;
	const char *dx11;
};

String read_entire_file(const char *file) {
	String content;

	FILE *handle = fopen(file, "rb");
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

void write_code(Ostream *stream, Array<Shader_Table_Element> *elems, Shader_Code &code, u32 lang) {
	auto vertex    = array_add(elems);
	vertex->flag   = lang | SHADER_TYPE_VERTEX;
	vertex->offset = (u32)ostream_get_size(stream);
	vertex->size   = (u32)code.vertex.count;
	ostream_write_buffer(stream, code.vertex.data, code.vertex.count);

	auto pixel    = array_add(elems);
	pixel->flag   = lang | SHADER_TYPE_PIXEL;
	pixel->offset = (u32)ostream_get_size(stream);
	pixel->size   = (u32)code.pixel.count;
	ostream_write_buffer(stream, code.pixel.data, code.pixel.count);
}

void shader_process_batch(Shader_Batch &batch) {
	Array<Shader_Table_Element> elems;
	Ostream                     out;
	defer {
		array_free(&elems);
		ostream_free(&out);
	};

	Shader_Code code;

	if (batch.dx11) {
		String content = read_entire_file(batch.dx11);
		defer {
			mfree(content.data);
		};

		if (hlsl_compile_shader(batch.dx11, content, &code)) {
			write_code(&out, &elems, code, SHADER_LANG_HLSL);
			free_shader_code(&code);
		}
	}

	if (elems) {
		FILE * file         = fopen(batch.name, "wb");
		u32    magic_number = SHADER_MAGIC_NUMBER;
		u32    elems_count  = (u32)elems.count;
		String src_code     = ostream_build_string(&out, false);

		fwrite(&magic_number, sizeof(magic_number), 1, file);
		fwrite(&elems_count, sizeof(elems_count), 1, file);
		fwrite(elems.data, sizeof(*elems.data) * elems.count, 1, file);
		fwrite(src_code.data, src_code.count, 1, file);
		fclose(file);

		mfree(src_code.data);
	}
}

void build_batch(Array_View<Shader_Batch> batch) {
	for (auto &b : batch) {
		printf("Compiling Shader: %s to %s\n", b.dx11, b.name);
		shader_process_batch(b);
	}
}

void add_batch(Array<Shader_Batch> *batch, const char *name, const char *dx11) {
	auto b  = array_add(batch);
	b->name = name;
	b->dx11 = dx11;
}

int system_main() {
	Array<Shader_Batch> batch;

	add_batch(&batch, "run_tree/data/shaders/quad.kfx", "src/shaders/quad.hlsl");
	add_batch(&batch, "run_tree/data/shaders/hdr.kfx", "src/shaders/hdr.hlsl");

	build_batch(batch);

	return 0;
}