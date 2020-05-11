#include "src/karma.h"
#include "shader.h"
#include "src/length_string.h"

#if defined(TARGET_WINDOWS)

#	pragma comment(lib, "d3dcompiler")
#	include <d3dcompiler.h>
#	include "src/directx/dx_error.h"

bool hlsl_compile_shader(const char *name, String src, Shader_Code *out, const char *vs_main, const char *ps_main) {
	ID3DBlob *vs_code = 0, *vs_error = 0;
	ID3DBlob *ps_code = 0, *ps_error = 0;

	HRESULT vs_result = D3DCompile2(src.data, src.count, name, NULL, NULL, vs_main, "vs_5_0", 0, 0, 0, NULL, 0, &vs_code, &vs_error);
	HRESULT ps_result = D3DCompile2(src.data, src.count, name, NULL, NULL, ps_main, "ps_5_0", 0, 0, 0, NULL, 0, &ps_code, &ps_error);

	bool success = true;
	if (FAILED(vs_result)) {
		report_error("HLSL", "Result: %s, Error: %s",
					 dx_error_string(vs_result),
					 tto_cstring(String((char *)vs_error->GetBufferPointer(), vs_error->GetBufferSize())));
		vs_error->Release();
		success = false;
	}

	if (FAILED(ps_result)) {
		report_error("HLSL", "Result: %s, Error: %s",
					 dx_error_string(ps_result),
					 tto_cstring(String((char *)ps_error->GetBufferPointer(), ps_error->GetBufferSize())));
		ps_error->Release();
		success = false;
	}

	if (success) {
		out->vertex.count = (s64)vs_code->GetBufferSize();
		out->vertex.data  = (utf8 *)mallocate(vs_code->GetBufferSize());
		memcpy(out->vertex.data, vs_code->GetBufferPointer(), vs_code->GetBufferSize());
		vs_code->Release();

		out->pixel.count = (s64)ps_code->GetBufferSize();
		out->pixel.data  = (utf8 *)mallocate(ps_code->GetBufferSize());
		memcpy(out->pixel.data, ps_code->GetBufferPointer(), ps_code->GetBufferSize());
		ps_code->Release();
	}

	return success;
}

#endif
