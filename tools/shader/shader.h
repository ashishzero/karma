#pragma once
#include "src/karma.h"

struct Shader_Code {
	String vertex;
	String pixel;
};

inline void free_shader_code(Shader_Code *code) {
	mfree(code->vertex.data);
	mfree(code->pixel.data);
}

inline void report_error(const char *category, const char *fmt, ...) {
	fprintf(stderr, "[%s] - ", category);
	va_list arg;
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
	fprintf(stderr, "\n");
}

#if defined(TARGET_WINDOWS)
bool hlsl_compile_shader(const char *name, String src, Shader_Code *out);
#endif
