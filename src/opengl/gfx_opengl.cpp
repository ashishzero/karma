#include "../gfx_platform.h"
#include "../glad/include/glad/glad.h"
#include "../stream.h"
#include "../length_string.h"
#include "../systems.h"

#if 1

static const GLint INT_FMT_KARMA_TO_OPENGL[] = {
	GL_RGBA,
	GL_RGBA32F,
	GL_RGBA32UI,
	GL_RGBA32I,
	GL_RGB32F,
	GL_RGB32UI,
	GL_RGB32I,
	GL_RGBA16F,
	GL_RGBA16,
	GL_RGBA16UI,
	GL_RGBA16_SNORM,
	GL_RGBA16I,
	GL_RG32F,
	GL_RG32UI,
	GL_RG32I,
	GL_DEPTH_STENCIL,
	GL_R32F,
	GL_R8,
	GL_RGB10_A2,
	GL_RGB10_A2,
	GL_RGB10_A2UI,
	GL_R11F_G11F_B10F,
	GL_RGBA8,
	GL_SRGB8_ALPHA8,
	GL_RGBA8UI,
	GL_RGBA8_SNORM,
	GL_RGBA8I,
	GL_RG16F,
	GL_RG16,
	GL_RG16UI,
	GL_RG16_SNORM,
	GL_RG16I,
	GL_DEPTH_COMPONENT32F,
	GL_R32F,
	GL_R32UI,
	GL_R32I,
	GL_DEPTH24_STENCIL8,
	GL_RG8,
	GL_RG8UI,
	GL_RG8_SNORM,
	GL_RG8I,
	GL_R16F,
	GL_DEPTH_COMPONENT16,
	GL_R16,
	GL_R16UI,
	GL_R16_SNORM,
	GL_R16I,
	GL_R8,
	GL_R8UI,
	GL_R8_SNORM,
	GL_R8I,
	GL_R8,
};
static_assert(static_count(INT_FMT_KARMA_TO_OPENGL) == Data_Format_COUNT, "The mapping from Data_Format to Internal Format is invalid");

static const GLenum TEX_ADD_KARMA_TO_OPENGL[] = {
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_CLAMP_TO_EDGE,
};
static_assert(static_count(TEX_ADD_KARMA_TO_OPENGL) == Texture_Address_Mode_COUNT, "The mapping from Texture_Address_Mode to GL_TEXTURE_WRAP is invalid");

static const GLenum BUFFER_USAGE_KARMA_TO_OPENGL[] = {
	GL_STATIC_DRAW,
	GL_STATIC_DRAW,
	GL_DYNAMIC_DRAW,
	GL_STREAM_DRAW,
};
static_assert(static_count(BUFFER_USAGE_KARMA_TO_OPENGL) == Buffer_Usage_COUNT, "The mapping from Buffer_Usage to OpenGL Buffer Usage is invalid");

static GLenum COMP_FUNC_KARMA_TO_OPENGL[] = {
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS,
};
static_assert(static_count(COMP_FUNC_KARMA_TO_OPENGL) == Comparison_Function_COUNT, "The mapping from Comparison_Function to GL_TEXTURE_COMPARE_FUNC is invaid");

GLenum karma_to_gl_texture_min_filter(Filter_Type filter) {
	switch (filter) {
		case Filter_Type_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_MIN_POINT_MAG_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MIN_LINEAR_MAG_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_ANISOTROPIC: return GL_LINEAR;
		case Filter_Type_COMPARISON_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_COMPARISON_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_COMPARISON_MIN_POINT_MAG_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_COMPARISON_MIN_LINEAR_MAG_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_COMPARISON_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_COMPARISON_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_COMPARISON_ANISOTROPIC: return GL_LINEAR;
		case Filter_Type_MINIMUM_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_MINIMUM_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_MINIMUM_MIN_POINT_MAG_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MINIMUM_MIN_LINEAR_MAG_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_MINIMUM_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MINIMUM_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_MINIMUM_ANISOTROPIC: return GL_LINEAR;
		case Filter_Type_MAXIMUM_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MAXIMUM_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_MAXIMUM_ANISOTROPIC:
			return GL_LINEAR;

			invalid_default_case();
	}

	return 0;
};

GLenum karma_to_gl_texture_mag_filter(Filter_Type filter) {
	switch (filter) {
		case Filter_Type_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MIN_POINT_MAG_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_MIN_LINEAR_MAG_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_ANISOTROPIC: return GL_LINEAR;
		case Filter_Type_COMPARISON_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_COMPARISON_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_COMPARISON_MIN_POINT_MAG_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_COMPARISON_MIN_LINEAR_MAG_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_COMPARISON_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_COMPARISON_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_COMPARISON_ANISOTROPIC: return GL_LINEAR;
		case Filter_Type_MINIMUM_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_MINIMUM_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MINIMUM_MIN_POINT_MAG_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_MINIMUM_MIN_LINEAR_MAG_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MINIMUM_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MINIMUM_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_MINIMUM_ANISOTROPIC: return GL_LINEAR;
		case Filter_Type_MAXIMUM_MIN_MAG_MIP_POINT: return GL_NEAREST;
		case Filter_Type_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		case Filter_Type_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT: return GL_NEAREST_MIPMAP_NEAREST;
		case Filter_Type_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
		case Filter_Type_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT: return GL_LINEAR_MIPMAP_NEAREST;
		case Filter_Type_MAXIMUM_MIN_MAG_MIP_LINEAR: return GL_LINEAR;
		case Filter_Type_MAXIMUM_ANISOTROPIC:
			return GL_LINEAR;

			invalid_default_case();
	}

	return 0;
};

bool karma_to_gl_filter_comparison(Filter_Type filter) {
	switch (filter) {
		case Filter_Type_MIN_MAG_MIP_POINT:
		case Filter_Type_MIN_MAG_POINT_MIP_LINEAR:
		case Filter_Type_MIN_POINT_MAG_LINEAR_MIP_POINT:
		case Filter_Type_MIN_POINT_MAG_MIP_LINEAR:
		case Filter_Type_MIN_LINEAR_MAG_MIP_POINT:
		case Filter_Type_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		case Filter_Type_MIN_MAG_LINEAR_MIP_POINT:
		case Filter_Type_MIN_MAG_MIP_LINEAR:
		case Filter_Type_ANISOTROPIC:
		case Filter_Type_MINIMUM_MIN_MAG_MIP_POINT:
		case Filter_Type_MINIMUM_MIN_MAG_POINT_MIP_LINEAR:
		case Filter_Type_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT:
		case Filter_Type_MINIMUM_MIN_POINT_MAG_MIP_LINEAR:
		case Filter_Type_MINIMUM_MIN_LINEAR_MAG_MIP_POINT:
		case Filter_Type_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		case Filter_Type_MINIMUM_MIN_MAG_LINEAR_MIP_POINT:
		case Filter_Type_MINIMUM_MIN_MAG_MIP_LINEAR:
		case Filter_Type_MINIMUM_ANISOTROPIC:
		case Filter_Type_MAXIMUM_MIN_MAG_MIP_POINT:
		case Filter_Type_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR:
		case Filter_Type_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT:
		case Filter_Type_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR:
		case Filter_Type_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT:
		case Filter_Type_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		case Filter_Type_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT:
		case Filter_Type_MAXIMUM_MIN_MAG_MIP_LINEAR:
		case Filter_Type_MAXIMUM_ANISOTROPIC:
			return false;

		case Filter_Type_COMPARISON_MIN_MAG_MIP_POINT:
		case Filter_Type_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
		case Filter_Type_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
		case Filter_Type_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
		case Filter_Type_COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
		case Filter_Type_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		case Filter_Type_COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
		case Filter_Type_COMPARISON_MIN_MAG_MIP_LINEAR:
		case Filter_Type_COMPARISON_ANISOTROPIC:
			return true;
	}

	return false;
}

static const GLenum MAP_TYPE_KARMA_TO_OPENGL[] = {
	GL_READ_ONLY,
	GL_WRITE_ONLY,
	GL_READ_WRITE,
	GL_READ_WRITE,
};
static_assert(static_count(MAP_TYPE_KARMA_TO_OPENGL) == Map_Type_COUNT, "The mapping from Map_Type to OpenGL Buffer map type is invalid");

static const GLenum INDEX_TYPE_KARMA_TO_OPENGL[] {
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT,
};
static_assert(static_count(INDEX_TYPE_KARMA_TO_OPENGL) == Index_Type_COUNT, "The mapping from Index_Type to OpenGL index type is invalid");

static const GLenum INDEX_SIZE_KARMA_TO_OPENGL[] {
	sizeof(GLubyte),
	sizeof(GLushort),
	sizeof(GLuint),
};
static_assert(static_count(INDEX_SIZE_KARMA_TO_OPENGL) == Index_Type_COUNT, "The mapping from Index_Type to OpenGL index type is invalid");

static const GLenum PRIMITIVE_KARMA_TO_OPENGL[] = {
	GL_TRIANGLES,
	GL_POINTS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
};
static_assert(static_count(PRIMITIVE_KARMA_TO_OPENGL) == Primitive_COUNT, "The mapping from Primitive to OpenGL draw primitive is invalid");

static const GLenum FILL_MODE_KARMA_TO_OPENGL[] {
	GL_LINE,
	GL_FILL,
};
static_assert(static_count(FILL_MODE_KARMA_TO_OPENGL) == Fill_Mode_COUNT, "The mapping from Fill_Mode to OpenGL polygon mode is invalid");

static const GLenum CULL_MODE_KARMA_TO_OPENGL[] = {
	0,
	GL_FRONT,
	GL_BACK,
};
static_assert(static_count(CULL_MODE_KARMA_TO_OPENGL) == Cull_Mode_COUNT, "The mapping from Cull_Mode to OpenGL cull mode is invalid");

static const GLenum FRONT_FACE_KARMA_TO_OPENGL[] = {
	GL_CW,
	GL_CCW,
};
static_assert(static_count(FRONT_FACE_KARMA_TO_OPENGL) == Front_Face_COUNT, "The mapping from Front_Face to OpenGL front face is invalid");

static const GLenum BLEND_KARMA_TO_OPENGL[] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA_SATURATE,
	GL_SRC1_COLOR,
	GL_ONE_MINUS_SRC1_COLOR,
	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC1_ALPHA,
};
static_assert(static_count(BLEND_KARMA_TO_OPENGL) == Blend_COUNT, "The mapping from Blend to OpenGL blend is invalid");

static const GLenum BLEND_OP_KARMA_TO_OPENGL[] = {
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX,
};
static_assert(static_count(BLEND_OP_KARMA_TO_OPENGL) == Blend_Operation_COUNT, "The mapping from Blend_Operation to OpenGL blend operation is invalid");

GLenum type_karma_to_opengl(Data_Format fmt) {
	switch (fmt) {
		case Data_Format_RGBA32_FLOAT: return GL_FLOAT;
		case Data_Format_RGBA32_UINT: return GL_UNSIGNED_INT;
		case Data_Format_RGBA32_SINT: return GL_INT;
		case Data_Format_RGB32_FLOAT: return GL_FLOAT;
		case Data_Format_RGB32_SINT: return GL_INT;
		case Data_Format_RGBA16_SINT: return GL_SHORT;
		case Data_Format_RG32_FLOAT: return GL_FLOAT;
		case Data_Format_RG32_UINT: return GL_UNSIGNED_INT;
		case Data_Format_RG32_SINT: return GL_INT;
		case Data_Format_RGBA8_UINT: return GL_UNSIGNED_BYTE;
		case Data_Format_RGBA8_SINT: return GL_BYTE;
		case Data_Format_RG16_SINT: return GL_SHORT;
		case Data_Format_R32_FLOAT: return GL_FLOAT;
		case Data_Format_R32_UINT: return GL_UNSIGNED_INT;
		case Data_Format_R32_SINT: return GL_INT;
		case Data_Format_RG8_UINT: return GL_UNSIGNED_BYTE;
		case Data_Format_RG8_SINT: return GL_BYTE;
		case Data_Format_R16_SINT: return GL_SHORT;
		case Data_Format_R8_UINT: return GL_UNSIGNED_BYTE;
		case Data_Format_R8_SINT:
			return GL_BYTE;

			invalid_default_case();
	}

	return 0;
}

GLint count_karma_to_opengl(Data_Format fmt) {
	switch (fmt) {
		case Data_Format_RGBA32_FLOAT: return 4;
		case Data_Format_RGBA32_UINT: return 4;
		case Data_Format_RGBA32_SINT: return 4;
		case Data_Format_RGB32_FLOAT: return 3;
		case Data_Format_RGB32_SINT: return 3;
		case Data_Format_RGBA16_SINT: return 4;
		case Data_Format_RG32_FLOAT: return 2;
		case Data_Format_RG32_UINT: return 2;
		case Data_Format_RG32_SINT: return 2;
		case Data_Format_RGBA8_UINT: return 4;
		case Data_Format_RGBA8_SINT: return 4;
		case Data_Format_RG16_SINT: return 2;
		case Data_Format_R32_FLOAT: return 1;
		case Data_Format_R32_UINT: return 1;
		case Data_Format_R32_SINT: return 1;
		case Data_Format_RG8_UINT: return 2;
		case Data_Format_RG8_SINT: return 2;
		case Data_Format_R16_SINT: return 1;
		case Data_Format_R8_UINT: return 1;
		case Data_Format_R8_SINT:
			return 1;

			invalid_default_case();
	}

	return 0;
}

struct Internal_Input_Element_Layout {
	GLint      count;
	GLenum     type;
	u32        offset;
	Input_Type input_type;
	u32        instance_data_step_rate;
};

struct Internal_Pipeline {
	GLuint                         program;
	Internal_Input_Element_Layout *input_layouts;
	u32                            input_layouts_count;
	u32                            uniform_block_index;

	GLenum primitive;
	GLenum fill_mode;
	GLenum cull_mode;
	GLenum front_face;
	bool   scissor_enable;

	bool      blend_enable;
	GLenum    blend_src;
	GLenum    blend_dst;
	GLenum    blend_op;
	GLenum    blend_src_alpha;
	GLenum    blend_dst_alpha;
	GLenum    blend_op_alpha;
	GLboolean blend_mask[4];

	bool      depth_test_enable;
	GLboolean depth_write_mask;
	GLenum    depth_compare;
};

struct Gfx_Platform_OpenGL : public Gfx_Platform {
	Handle platform_handle;
	void * gl_context;

	u32 multisamples;
	u32 framebuffer_w;
	u32 framebuffer_h;

	GLuint             vertex_array;
	Internal_Pipeline *current_pipeline = 0;
	GLenum             current_index_type;
	GLsizei            current_index_size;

	int (*set_swap_interval_func)(int interval);
	int (*get_swap_interval_func)();

	Gfx_Platform_Info info;

	bool load_library(Vsync vsync);
	void swap_buffers();

	virtual void *get_backend_device() final override {
		return 0;
	}

	virtual void *get_backend_context() final override {
		return gl_context;
	}

	virtual const Gfx_Platform_Info *get_info() final override {
		return &info;
	}

	virtual void on_client_resize(u32 w, u32 h) final override {
		framebuffer_w = w;
		framebuffer_h = h;
	}

	virtual void get_render_view_size(u32 *w, u32 *h) final override {
		*w = framebuffer_w;
		*h = framebuffer_h;
	}

	virtual Framebuffer get_default_framebuffer() final override {
		Framebuffer result;
		result.id.h32 = 0;
		return result;
	}

	virtual u32 get_maximum_supported_multisamples() final override {
		GLint multisamples;
		glGetIntegerv(GL_MAX_SAMPLES, &multisamples);
		return (u32)multisamples;
	}

	virtual u32 get_multisamples() final override {
		return multisamples;
	}

	virtual void set_sync_interval(Vsync vsync) final override {
		if (set_swap_interval_func) {
			if (vsync == Vsync_ADAPTIVE) {
				if (!set_swap_interval_func((int)vsync)) {
					system_log(LOG_WARNING, "OpenGL", "Negative swap interval not supported!");
					set_swap_interval_func(1);
				}
			} else {
				set_swap_interval_func((int)vsync);
			}
		}
	}

	virtual Vsync get_sync_interval() final override {
		if (get_swap_interval_func) {
			return (Vsync)get_swap_interval_func();
		} else {
			return Vsync_0;
		}
	}

	virtual void present() final override {
		swap_buffers();
	}

	virtual Texture2d create_texture2d(u32 w, u32 h, u32 channels, Data_Format format, const u8 **pixels, Buffer_Usage usage, Texture_Bind_Flags flags, u32 mip_levels) final override {
		GLenum gl_format;

		switch (channels) {
			case 1: gl_format = GL_RED;
			case 2: gl_format = GL_RG;
			case 3: gl_format = GL_RGB;
			case 4: gl_format = GL_RGBA;
		}

		GLint gl_internal_format = INT_FMT_KARMA_TO_OPENGL[format];

		GLuint texid;

		u32 mip_counts = mip_levels;
		if (mip_counts == 0) {
			mip_counts = (u32)min_value(log2(w), log2(h)) + 1;
		}

		GLenum target;

		// TODO: Multisampling
		if ((flags & Texture_Bind_DEPTH_STENCIL) == Texture_Bind_DEPTH_STENCIL) {
			glGenRenderbuffers(1, &texid);
			glBindRenderbuffer(GL_RENDERBUFFER, texid);
			glRenderbufferStorage(GL_RENDERBUFFER, gl_internal_format, w, h);
			target = GL_RENDERBUFFER;
		} else if ((flags & Texture_Bind_RENDER_TARGET) && (flags & Texture_Bind_SHADER_RESOURCE) != Texture_Bind_SHADER_RESOURCE) {
			glGenRenderbuffers(1, &texid);
			glBindRenderbuffer(GL_RENDERBUFFER, texid);
			glRenderbufferStorage(GL_RENDERBUFFER, gl_internal_format, w, h);
			target = GL_RENDERBUFFER;
		} else {
			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, texid);
			for (u32 i = 0; i < mip_counts; ++i) {
				glTexImage2D(GL_TEXTURE_2D, i, gl_internal_format, w >> i, h >> i, 0, gl_format, GL_UNSIGNED_BYTE, pixels ? pixels[i] : NULL);
			}
			target = GL_TEXTURE_2D;
		}

		Texture2d result = {};
		result.id.l32    = target;
		result.id.h32    = texid;
		return result;
	}

	virtual void update_texture2d(Texture2d texture, u32 xoffset, u32 yoffset, u32 w, u32 h, u32 n, u8 *pixels) final override {
		GLenum gl_format;

		switch (n) {
			case 1: gl_format = GL_RED;
			case 2: gl_format = GL_RG;
			case 3: gl_format = GL_RGB;
			case 4: gl_format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture.id.h32);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, w, h, gl_format, GL_UNSIGNED_BYTE, pixels);
	}

	virtual void destroy_texture2d(Texture2d texture) final override {
		auto id = texture.id.h32;

		GLenum target = texture.id.l32;

		switch (target) {
			case GL_RENDERBUFFER: glDeleteFramebuffers(1, &texture.id.h32); break;
			case GL_TEXTURE_2D: glDeleteTextures(1, &texture.id.h32); break;
			case GL_TEXTURE_2D_MULTISAMPLE:
				glDeleteTextures(1, &texture.id.h32);
				break;

				invalid_default_case();
		}
	}

	virtual void generate_mipmaps(Texture_View view) final override {
		glBindTexture(GL_TEXTURE_2D, view.id.h32);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	virtual Texture_View create_texture_view(Texture2d texture) final override {
		// we just copy the same id
		Texture_View view;
		view.id = texture.id;
		return view;
	}

	virtual Depth_Stencil_View create_depth_stencil_view(Texture2d texture) final override {
		// we just copy the same id
		Depth_Stencil_View view;
		view.id = texture.id;
		return view;
	}

	virtual Framebuffer create_framebuffer(u32 count, Texture2d *textures, Texture_View *views, Depth_Stencil_View *depth_view) final override {
		GLuint id;
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		for (u32 index = 0; index < count; ++index) {
			if (views[index].id.l32 != GL_RENDERBUFFER) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, views[index].id.l32, views[index].id.h32, 0);
			} else {
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, views[index].id.l32, views[index].id.h32);
			}
		}

		if (depth_view) {
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_view->id.h32);
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			system_log(LOG_WARNING, "OpenGL", "Incomplete framebuffer!");
			trigger_breakpoint();
		}

		GLuint *attachments = (GLuint *)tallocate(sizeof(GLuint) * count);
		for (GLuint index = 0; index < count; ++index)
			attachments[index] = GL_COLOR_ATTACHMENT0 + index;
		glDrawBuffers(count, attachments);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Framebuffer result;
		result.id.l32 = count;
		result.id.h32 = id;
		return result;
	}

	virtual void destroy_texture_view(Texture_View view) final override {
		// do nothing; we destory textures instead
	}

	virtual void destroy_depth_stencil_view(Depth_Stencil_View view) final override {
		// do nothing; we destory textures instead
	}

	virtual void destroy_framebuffer(Framebuffer handle) final override {
		glDeleteFramebuffers(1, &handle.id.h32);
	}

	virtual Sampler create_sampler(const Filter &filter, const Texture_Address &address, const Level_Of_Detail &lod) final override {
		GLuint id;
		glGenSamplers(1, &id);

		// glSamplerParameteri(id, GL_TEXTURE_MAX_ANISOTROPY_EXT, filter.max_anisotropy); TODO: Use this extension is available
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, karma_to_gl_texture_min_filter(filter.type));
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, karma_to_gl_texture_mag_filter(filter.type));

		if (karma_to_gl_filter_comparison(filter.type)) {
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_FUNC, COMP_FUNC_KARMA_TO_OPENGL[filter.comparison]);
		} else {
			glSamplerParameteri(id, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}

		glSamplerParameteri(id, GL_TEXTURE_WRAP_S, TEX_ADD_KARMA_TO_OPENGL[address.u]);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_T, TEX_ADD_KARMA_TO_OPENGL[address.v]);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_R, TEX_ADD_KARMA_TO_OPENGL[address.w]);
		glSamplerParameterfv(id, GL_TEXTURE_BORDER_COLOR, address.border_color.m);

		glSamplerParameteri(id, GL_TEXTURE_MIN_LOD, (GLint)lod.min);
		glSamplerParameteri(id, GL_TEXTURE_MAX_LOD, (GLint)lod.max);
		glSamplerParameteri(id, GL_TEXTURE_LOD_BIAS, (GLint)lod.bias);

		Sampler result;
		result.id.h32 = id;
		return result;
	}

	virtual void destory_sampler(Sampler sampler) final override {
		glDeleteSamplers(1, &sampler.id.h32);
	}

	virtual Vertex_Buffer create_vertex_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) final override {
		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, size, data, BUFFER_USAGE_KARMA_TO_OPENGL[usage]);

		Vertex_Buffer buffer;
		buffer.id.h32 = id;
		return buffer;
	}

	virtual Index_Buffer create_index_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) final override {
		GLuint id;

		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, BUFFER_USAGE_KARMA_TO_OPENGL[usage]);

		Index_Buffer buffer;
		buffer.id.h32 = id;
		return buffer;
	}

	virtual Uniform_Buffer create_uniform_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) final override {
		GLuint id;

		glGenBuffers(1, &id);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferData(GL_UNIFORM_BUFFER, size, data, BUFFER_USAGE_KARMA_TO_OPENGL[usage]);

		Uniform_Buffer buffer;
		buffer.id.h32 = id;
		buffer.id.l32 = size;
		return buffer;
	}

	virtual void destroy_vertex_buffer(Vertex_Buffer buffer) final override {
		glDeleteBuffers(1, &buffer.id.h32);
	}

	virtual void destroy_index_buffer(Index_Buffer buffer) final override {
		glDeleteBuffers(1, &buffer.id.h32);
	}

	virtual void destroy_uniform_buffer(Uniform_Buffer buffer) final override {
		glDeleteBuffers(1, &buffer.id.h32);
	}

	virtual void update_vertex_buffer(Vertex_Buffer buffer, u32 offset, u32 size, void *data) final override {
		glBindBuffer(GL_ARRAY_BUFFER, buffer.id.h32);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	virtual void update_index_buffer(Index_Buffer buffer, u32 offset, u32 size, void *data) final override {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id.h32);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	}

	virtual void update_uniform_buffer(Uniform_Buffer buffer, void *data) final override {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer.id.h32);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer.id.l32, data);
	}

	virtual void *map(Vertex_Buffer buffer, Map_Type type) final override {
		glBindBuffer(GL_ARRAY_BUFFER, buffer.id.h32);
		return glMapBuffer(GL_ARRAY_BUFFER, MAP_TYPE_KARMA_TO_OPENGL[type]);
	}

	virtual void *map(Index_Buffer buffer, Map_Type type) final override {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id.h32);
		return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, MAP_TYPE_KARMA_TO_OPENGL[type]);
	}

	virtual void *map(Uniform_Buffer buffer, Map_Type type) final override {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer.id.h32);
		return glMapBuffer(GL_UNIFORM_BUFFER, MAP_TYPE_KARMA_TO_OPENGL[type]);
	}

	virtual void unmap(Vertex_Buffer buffer) final override {
		glBindBuffer(GL_ARRAY_BUFFER, buffer.id.h32);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	virtual void unmap(Index_Buffer buffer) final override {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id.h32);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	virtual void unmap(Uniform_Buffer buffer) final override {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer.id.h32);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	GLuint create_shader(GLenum type, String source) {
		GLuint shaderid = glCreateShader(type);

		const GLchar *sources[] = {
			(GLchar *)source.data,
		};
		const GLint lens[] = {
			(GLint)source.count
		};

		glShaderSource(shaderid, static_count(sources), sources, lens);

		glCompileShader(shaderid);

		GLint compiled;
		glGetShaderiv(shaderid, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar  message[2048];
			glGetShaderInfoLog(shaderid, 2048, &log_length, message);
			const char *shader_type_name = "-unknown-";
			if (type == GL_VERTEX_SHADER)
				shader_type_name = "Vertex";
			else if (type == GL_FRAGMENT_SHADER)
				shader_type_name = "Fragment";
			else if (type == GL_GEOMETRY_SHADER)
				shader_type_name = "Geometry";
			system_log(LOG_WARNING, "OpenGL", "OpenGL %s Shader compilation failed: %s", shader_type_name, message);
		}

		return shaderid;
	}

	virtual Render_Pipeline create_render_pipeline(const Shader_Info &    shader,
												   const Rasterizer_Info &rasterizer,
												   const Blend_Info &     blend,
												   const Depth_Info &     depth,
												   const String           name) final override {
		void *             mem      = mallocate(sizeof(Internal_Pipeline) + sizeof(Input_Element_Layout) * shader.input_layouts_count);
		Internal_Pipeline *pipeline = (Internal_Pipeline *)mem;
		pipeline->input_layouts     = (Internal_Input_Element_Layout *)((u8 *)mem + sizeof(Internal_Pipeline));

		GLuint vertex   = create_shader(GL_VERTEX_SHADER, shader.vertex);
		GLuint fragment = create_shader(GL_FRAGMENT_SHADER, shader.pixel);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		GLint program_linked;
		glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar  message[2048];
			glGetProgramInfoLog(program, 2048, &log_length, message);
			system_log(LOG_WARNING, "OpenGL", "OpenGL Shader program link failed (%s) in pipeline %s", message, tto_cstring(name));
			glDeleteProgram(program);
			delete pipeline;
			return {};
		}

		pipeline->program             = program;
		pipeline->input_layouts_count = shader.input_layouts_count;
		pipeline->uniform_block_index = 0;

		auto dst = pipeline->input_layouts;
		auto src = shader.input_layouts;
		for (u32 index = 0; index < shader.input_layouts_count; ++index) {
			dst->type                    = type_karma_to_opengl(src->format);
			dst->count                   = count_karma_to_opengl(src->format);
			dst->offset                  = src->offset;
			dst->input_type              = src->input_type;
			dst->instance_data_step_rate = src->instance_data_step_rate;
			dst += 1;
			src += 1;
		}

		pipeline->primitive      = PRIMITIVE_KARMA_TO_OPENGL[rasterizer.primitive];
		pipeline->fill_mode      = FILL_MODE_KARMA_TO_OPENGL[rasterizer.fill_mode];
		pipeline->cull_mode      = CULL_MODE_KARMA_TO_OPENGL[rasterizer.cull_mode];
		pipeline->front_face     = FRONT_FACE_KARMA_TO_OPENGL[rasterizer.front_face];
		pipeline->scissor_enable = rasterizer.scissor_enable;

		pipeline->blend_enable    = blend.enable;
		pipeline->blend_src       = BLEND_KARMA_TO_OPENGL[blend.src];
		pipeline->blend_dst       = BLEND_KARMA_TO_OPENGL[blend.dst];
		pipeline->blend_op        = BLEND_OP_KARMA_TO_OPENGL[blend.op];
		pipeline->blend_src_alpha = BLEND_KARMA_TO_OPENGL[blend.src_alpha];
		pipeline->blend_dst_alpha = BLEND_KARMA_TO_OPENGL[blend.dst_alpha];
		pipeline->blend_op_alpha  = BLEND_OP_KARMA_TO_OPENGL[blend.op_alpha];
		pipeline->blend_mask[0]   = ((blend.mask & Blend_Mask_R) == Blend_Mask_R);
		pipeline->blend_mask[1]   = ((blend.mask & Blend_Mask_B) == Blend_Mask_B);
		pipeline->blend_mask[2]   = ((blend.mask & Blend_Mask_B) == Blend_Mask_B);
		pipeline->blend_mask[3]   = ((blend.mask & Blend_Mask_A) == Blend_Mask_A);

		pipeline->depth_test_enable = depth.test_enable;
		pipeline->depth_write_mask  = depth.write_mask == Depth_Write_Mask_ALL ? GL_TRUE : GL_FALSE;
		pipeline->depth_compare     = COMP_FUNC_KARMA_TO_OPENGL[depth.compare];

		Render_Pipeline result;
		result.id.hptr = pipeline;

		return result;
	}

	virtual void destory_render_pipeline(Render_Pipeline handle) final override {
		Internal_Pipeline *pipeline = (Internal_Pipeline *)handle.id.hptr;
		glDeleteProgram(pipeline->program);
		mfree(pipeline);
	}

	virtual void begin_drawing(Framebuffer framebuffer, Clear_Flags flags, Color4 color, r32 depth, u8 stencil) final override {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer.id.h32);

		u32 gl_flags = 0;
		if (flags & Clear_COLOR) gl_flags |= GL_COLOR_BUFFER_BIT;
		if (flags & Clear_DEPTH) gl_flags |= GL_DEPTH_BUFFER_BIT;
		if (flags & Clear_STENCIL) gl_flags |= GL_STENCIL_BUFFER_BIT;

		if (gl_flags) {
			glClearDepth(depth);
			glClearStencil(stencil);
			glClearColor(vec_expand4(color));

			glClear(gl_flags);
		}
	}

	virtual void end_drawing() final override {
		current_pipeline = NULL;
	}

	virtual void cmd_set_viewport(r32 x, r32 y, r32 w, r32 h) final override {
		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	}

	virtual void cmd_bind_pipeline(Render_Pipeline handle) final override {
		Internal_Pipeline *pipeline = (Internal_Pipeline *)handle.id.hptr;

		if (pipeline != current_pipeline) {
			current_pipeline = pipeline;

			glUseProgram(pipeline->program);

			glPolygonMode(GL_FRONT_AND_BACK, pipeline->fill_mode);
			if (pipeline->cull_mode != 0) {
				glEnable(GL_CULL_FACE);
				glCullFace(pipeline->cull_mode);
				glFrontFace(pipeline->front_face);
			} else {
				glDisable(GL_CULL_FACE);
			}

			if (pipeline->scissor_enable) {
				glEnable(GL_SCISSOR_TEST);
			} else {
				glDisable(GL_SCISSOR_TEST);
			}

			if (pipeline->blend_enable) {
				glEnable(GL_BLEND);
				glBlendFuncSeparate(pipeline->blend_src, pipeline->blend_dst, pipeline->blend_src_alpha, pipeline->blend_dst_alpha);
				glBlendEquationSeparate(pipeline->blend_op, pipeline->blend_op_alpha);
			} else {
				glDisable(GL_BLEND);
			}

			glColorMask(pipeline->blend_mask[0], pipeline->blend_mask[1], pipeline->blend_mask[2], pipeline->blend_mask[3]);

			if (pipeline->depth_test_enable) {
				glEnable(GL_DEPTH_TEST);
				glDepthMask(pipeline->depth_write_mask);
				glDepthFunc(pipeline->depth_compare);
			} else {
				glDisable(GL_DEPTH_TEST);
			}

			pipeline->uniform_block_index = 0;
		}
	}

	virtual void cmd_bind_vertex_buffer(Vertex_Buffer buffer, u32 stride) final override {
		glBindBuffer(GL_ARRAY_BUFFER, buffer.id.h32);

		assert(current_pipeline);

		auto count  = current_pipeline->input_layouts_count;
		auto layout = current_pipeline->input_layouts;

		for (u32 index = 0; index < count; ++index) {
			glEnableVertexAttribArray(index);
			size_t offset = layout->offset;
			glVertexAttribPointer(index, layout->count, layout->type, GL_FALSE, stride, (void *)offset);
			if (layout->input_type == Input_Type_PER_VERTEX_DATA) {
				glVertexAttribDivisor(index, 0);
			} else {
				glVertexAttribDivisor(index, layout->instance_data_step_rate);
			}
			layout += 1;
		}
	}

	virtual void cmd_bind_index_buffer(Index_Buffer buffer, Index_Type type) final override {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id.h32);
		current_index_type = INDEX_TYPE_KARMA_TO_OPENGL[type];
		current_index_size = INDEX_SIZE_KARMA_TO_OPENGL[type];
	}

	virtual void cmd_bind_vs_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) final override {
		assert(current_pipeline);
		auto ubo_index = current_pipeline->uniform_block_index;
		for (u32 index = 0; index < count; ++index) {
			glUniformBlockBinding(current_pipeline->program, slot_index + index, ubo_index + index);
			glBindBufferBase(GL_UNIFORM_BUFFER, ubo_index + index, buffer[index].id.h32);
		}
		current_pipeline->uniform_block_index += count;
	}

	virtual void cmd_bind_ps_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) final override {
		cmd_bind_vs_uniform_buffers(slot_index, count, buffer); // it's the same for opengl
	}

	virtual void cmd_bind_textures(u32 slot_index, u32 count, Texture_View *views) final override {
		for (u32 index = 0; index < count; ++index) {
			glActiveTexture(GL_TEXTURE0 + index + slot_index);
			glBindTexture(views[index].id.l32, views[index].id.h32);
		}
	}

	virtual void cmd_bind_samplers(u32 slot_index, u32 count, Sampler *samplers) final override {
		for (u32 index = 0; index < count; ++index) {
			glBindSampler(slot_index + index, samplers[index].id.h32);
		}
	}

	virtual void cmd_draw(u32 vertex_count, u32 start_vertex) final override {
		glDrawArrays(current_pipeline->primitive, start_vertex, vertex_count);
	}

	virtual void cmd_draw_indexed(u32 index_count, u32 start_index, u32 base_vertex) final override {
		size_t offset = start_index * current_index_size;
		glDrawElementsBaseVertex(current_pipeline->primitive, index_count, current_index_type, (void *)offset, base_vertex);
	}

	virtual void destroy() final override {
		glDeleteVertexArrays(1, &vertex_array);
	}

	static void debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei msgLength, const GLchar *message, const void *user_param) {
		// TODO: Ignoring these low severity messages during development
		// These messages probably need to be studied properly and resolved

		// MSG: API_ID_REDUNDANT_FBO performance warning has been generated.
		// Redundant state change in glBindFramebuffer API call, FBO 1, "", already bound.
		if (id == 8) return;

		const char *srcstr  = 0;
		const char *typestr = 0;
		const char *svrstr  = 0;
		int         log     = LOG_INFO;

		switch (source) {
			case GL_DEBUG_SOURCE_API_ARB:
				srcstr = "API";
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
				srcstr = "ARB";
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
				srcstr = "COMPILER";
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
				srcstr = "3RD PARTY";
				break;
			case GL_DEBUG_SOURCE_APPLICATION_ARB:
				srcstr = "APP";
				break;
			case GL_DEBUG_SOURCE_OTHER_ARB:
				srcstr = "OTHER";
				break;
			default:
				srcstr = "UNKNOWN";
		}

		switch (type) {
			case GL_DEBUG_TYPE_ERROR_ARB:
				typestr = "ERROR";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
				typestr = "DEPRECATED BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
				typestr = "UNDEFINED BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB:
				typestr = "PORTABILITY";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE_ARB:
				typestr = "PERFORMANCE";
				break;
			case GL_DEBUG_TYPE_OTHER_ARB:
				typestr = "OTHER";
				break;
			default:
				typestr = "UNKNOWN";
		}

		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH_ARB:
				svrstr = "HIGH";
				log    = LOG_ERROR;
				break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB:
				svrstr = "MEDIUM";
				log    = LOG_WARNING;
				break;
			case GL_DEBUG_SEVERITY_LOW_ARB:
				svrstr = "LOW";
				log    = LOG_WARNING;
				break;
			default:
				svrstr = "UNKNOWN";
		}

		system_log(LOG_WARNING, "OpenGL", "=======================================");
		system_log(LOG_WARNING, "OpenGL", "   Code: %u", id);
		system_log(LOG_WARNING, "OpenGL", "   Source: %s", srcstr);
		system_log(LOG_WARNING, "OpenGL", "   Type: %s", typestr);
		system_log(LOG_WARNING, "OpenGL", "   Severity: %s", svrstr);
		system_log(LOG_WARNING, "OpenGL", "   Message: %s", message);
		system_log(LOG_WARNING, "OpenGL", "=======================================");

		trigger_if(log == LOG_ERROR);
	}
};

Gfx_Platform *create_opengl_context(Handle platform, Vsync vsync, s32 multisamples) {
	static Gfx_Platform_OpenGL gfx;

	// NOTE: set these, they are used by *load_library* function
	gfx.platform_handle = platform;
	gfx.multisamples    = multisamples;

	if (!gfx.load_library(vsync)) {
		gfx.backend = Render_Backend_NONE;
		return &gfx;
	}

#	if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
	{
		system_log(LOG_INFO, "OpenGL", "GL_DEBUG_OUTPUT_SYNCHRONOUS enabled.");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB((GLDEBUGPROCKHR)Gfx_Platform_OpenGL::debug_output, 0);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		// Disable notification message callback
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, nullptr, GL_FALSE);
	}
#	endif

	gfx.info.vendor       = (const char *)glGetString(GL_VENDOR);
	gfx.info.renderer     = (const char *)glGetString(GL_RENDERER);
	gfx.info.version      = (const char *)glGetString(GL_VERSION);
	gfx.info.shading_lang = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

	glGenVertexArrays(1, &gfx.vertex_array);
	glBindVertexArray(gfx.vertex_array);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_FRAMEBUFFER_SRGB);

	gfx.backend = Render_Backend_OPENGL;

	return &gfx;
}

#	if defined(TARGET_WINDOWS)

//
// Windows OpenGL Context Creation
//

// windows uh...
#		ifdef APIENTRY
#			undef APIENTRY
#		endif

#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>

static HDC wnd_dc;

void Gfx_Platform_OpenGL::swap_buffers() {
	SwapBuffers(wnd_dc);
}

bool Gfx_Platform_OpenGL::load_library(Vsync vsync) {
	HMODULE opengl = LoadLibraryW(L"opengl32.dll");
	if (!opengl) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform", "OpenGL module could not be loaded");
		return false;
	}
	defer {
		FreeLibrary(opengl);
	};

	WNDCLASSEXW wnd_class   = {};
	wnd_class.cbSize        = sizeof(wnd_class);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc   = DefWindowProcW;
	wnd_class.hInstance     = GetModuleHandleW(0);
	wnd_class.lpszClassName = L"OpenGLDummyWindow";
	RegisterClassExW(&wnd_class);
	defer {
		UnregisterClassW(L"OpenGLDummyWindow", GetModuleHandleW(0));
	};

	HWND dummy_wnd = CreateWindowExW(0, L"OpenGLDummyWindow", L"OpenGLDummyWindow", 0,
									 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									 0, 0, GetModuleHandleW(0), 0);
	if (!dummy_wnd) {
		system_log(LOG_WARNING, "Platform",
				   "OpenGL Context could not be created. Reason: Failed to create dummy window");
		return false;
	}
	HDC dummy_dc = GetDC(dummy_wnd);
	defer {
		ReleaseDC(dummy_wnd, dummy_dc);
		DestroyWindow(dummy_wnd);
	};

	PIXELFORMATDESCRIPTOR dummy_pixel_format = {};
	dummy_pixel_format.nSize                 = sizeof(dummy_pixel_format);
	dummy_pixel_format.nVersion              = 1;
	dummy_pixel_format.dwFlags               = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	dummy_pixel_format.iPixelType            = PFD_TYPE_RGBA;
	dummy_pixel_format.cColorBits            = 32;
	dummy_pixel_format.cDepthBits            = 24;
	dummy_pixel_format.cStencilBits          = 8;
	int pfi                                  = ChoosePixelFormat(dummy_dc, &dummy_pixel_format);
	if (pfi == 0) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform",
				   "Unsupported pixel format for dummy window. OpenGL Context creation failed");
	}

	PIXELFORMATDESCRIPTOR suggested_pixel_format;
	int                   num_pixel_fmts = DescribePixelFormat(dummy_dc, pfi, sizeof(suggested_pixel_format), &suggested_pixel_format);
	if (num_pixel_fmts == 0) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform", "OpenGL Pixel formats could not be described for dummy window");
		return false;
	}

	if (!SetPixelFormat(dummy_dc, pfi, &suggested_pixel_format)) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform", "OpenGL Pixel formats could not be set for dummy window");
		return false;
	}

	typedef HGLRC         WGLCreateContextFunc(HDC arg);
	WGLCreateContextFunc *wgl_create_context;
	typedef BOOL          WGLMakeCurrent(HDC, HGLRC);
	WGLMakeCurrent *      wgl_make_current;
	typedef PROC          WGLGetProcAddress(LPCSTR arg);
	WGLGetProcAddress *   wgl_get_proc_address;
	typedef BOOL          WGLDeleteContext(HGLRC arg);
	WGLDeleteContext *    wgl_delete_context;

	wgl_create_context = (WGLCreateContextFunc *)GetProcAddress(opengl, "wglCreateContext");
	if (!wgl_create_context) {
		system_log(LOG_WARNING, "Platform",
				   "wglCreateContext not found in OpenGL module");
		return false;
	}
	wgl_make_current = (WGLMakeCurrent *)GetProcAddress(opengl, "wglMakeCurrent");
	if (!wgl_make_current) {
		system_log(LOG_WARNING, "Platform",
				   "wglMakeCurrent not found in OpenGL module");
		return false;
	}
	wgl_get_proc_address = (WGLGetProcAddress *)GetProcAddress(opengl, "wglGetProcAddress");
	if (!wgl_get_proc_address) {
		system_log(LOG_WARNING, "Platform",
				   "wglGetProcAddress not found in OpenGL module");
		return false;
	}
	wgl_delete_context = (WGLDeleteContext *)GetProcAddress(opengl, "wglDeleteContext");
	if (!wgl_delete_context) {
		system_log(LOG_WARNING, "Platform",
				   "wglDeleteContext not found in OpenGL module");
		return false;
	}

	HGLRC dummy_gl_context = wgl_create_context(dummy_dc);
	if (!dummy_gl_context) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform",
				   "OpenGL dummy context could not be created");
		return false;
	}

	if (!wgl_make_current(dummy_dc, dummy_gl_context)) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform",
				   "OpenGL dummy context could not be used");
		return false;
	}

	typedef HGLRC WINAPI            WGLCreateContextAttribsARBFunc(HDC hdc, HGLRC hShareContext, const int *attribList);
	WGLCreateContextAttribsARBFunc *wgl_create_context_attribs_arb;
	typedef BOOL WINAPI             WGLChoosePixelFormatARBFunc(HDC hdc, const int *piAttribIList, const float *pfAttribFList, u32 nMaxFormats, int *piFormats, u32 *nNumFormats);
	WGLChoosePixelFormatARBFunc *   wgl_choose_pixel_format_arb;
	typedef BOOL                    WGLSwapIntervalEXTFunc(int interval);
	typedef int                     WGLGetSwapIntervalEXTFunc();

	wgl_create_context_attribs_arb = (WGLCreateContextAttribsARBFunc *)wgl_get_proc_address("wglCreateContextAttribsARB");
	if (!wgl_create_context_attribs_arb) {
		system_log(LOG_WARNING, "Platform",
				   "wglCreateContextAttribsARB not found in OpenGL module");
		return false;
	}
	wgl_choose_pixel_format_arb = (WGLChoosePixelFormatARBFunc *)wgl_get_proc_address("wglChoosePixelFormatARB");
	if (!wgl_choose_pixel_format_arb) {
		system_log(LOG_WARNING, "Platform",
				   "wglChoosePixelFormatARB not found in OpenGL module");
		return false;
	}
	set_swap_interval_func = (WGLSwapIntervalEXTFunc *)wgl_get_proc_address("wglSwapIntervalEXT");
	get_swap_interval_func = (WGLGetSwapIntervalEXTFunc *)wgl_get_proc_address("wglGetSwapIntervalEXT");

	const int WGL_DRAW_TO_WINDOW_ARB           = 0x2001;
	const int WGL_SUPPORT_OPENGL_ARB           = 0x2010;
	const int WGL_DOUBLE_BUFFER_ARB            = 0x2011;
	const int WGL_PIXEL_TYPE_ARB               = 0x2013;
	const int WGL_TYPE_RGBA_ARB                = 0x202B;
	const int WGL_COLOR_BITS_ARB               = 0x2014;
	const int WGL_DEPTH_BITS_ARB               = 0x2022;
	const int WGL_STENCIL_BITS_ARB             = 0x2023;
	const int WGL_CONTEXT_MAJOR_VERSION_ARB    = 0x2091;
	const int WGL_CONTEXT_MINOR_VERSION_ARB    = 0x2092;
	const int WGL_CONTEXT_LAYER_PLANE_ARB      = 0x2093;
	const int WGL_CONTEXT_FLAGS_ARB            = 0x2094;
	const int WGL_CONTEXT_PROFILE_MASK_ARB     = 0x9126;
	const int WGL_CONTEXT_DEBUG_BIT_ARB        = 0x0001;
	const int WGL_SAMPLE_BUFFERS_ARB           = 0x2041;
	const int WGL_SAMPLES_ARB                  = 0x2042;
	const int WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB = 0x20A9;
	const int WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x0001;

	int attrib_list[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, (int)multisamples,
		0
	};

	HWND wnd = (HWND)platform_handle.hptr;
	wnd_dc   = GetDC(wnd);

	int pixel_format;
	u32 num_formats;
	if (!wgl_choose_pixel_format_arb(wnd_dc, attrib_list, 0, 1, &pixel_format, &num_formats)) {
		system_log(LOG_WARNING, "Platform", "OpenGL Pixel format search failed!");
		wgl_make_current(0, 0);
		wgl_delete_context(dummy_gl_context);
	}

	if (num_formats == 0) {
		system_log(LOG_WARNING, "Platform", "OpenGL Compatible Pixel Format not found!");
		wgl_make_current(0, 0);
		wgl_delete_context(dummy_gl_context);
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	num_pixel_fmts = DescribePixelFormat(wnd_dc, pixel_format, sizeof(pfd), &pfd);
	if (num_pixel_fmts == 0) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform", "OpenGL Pixel formats could not be described");
		return false;
	}

	if (!SetPixelFormat(wnd_dc, pixel_format, &pfd)) {
		// TODO: Handle win32 error!!!
		system_log(LOG_WARNING, "Platform", "OpenGL Pixel formats could not be set");
		return false;
	}

	int gl_version_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB,
		4,
		WGL_CONTEXT_MINOR_VERSION_ARB,
		2,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#		if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
		WGL_CONTEXT_FLAGS_ARB,
		WGL_CONTEXT_DEBUG_BIT_ARB,
#		endif
		0,
	};
	HGLRC glContext = wgl_create_context_attribs_arb(wnd_dc, 0, gl_version_attribs);
	wgl_make_current(0, 0);
	wgl_delete_context(dummy_gl_context);
	wgl_make_current(wnd_dc, glContext);

	if (set_swap_interval_func) {
		if (vsync == Vsync_ADAPTIVE) {
			if (!set_swap_interval_func((int)vsync)) {
				system_log(0, "OpenGL", "Tearing (-1 swap interval) not supported.");
				set_swap_interval_func(1);
			}
		} else {
			set_swap_interval_func((int)vsync);
		}
	} else {
		system_log(LOG_WARNING, "OpenGL", "WGL_EXT_swap_control extension is not present, Vertical swapping disabled");
	}

	if (!gladLoadGL()) {
		system_log(LOG_ERROR, "OpenGL", "gladLoadGLLoader() - Failed to Load Proc Address for OpenGL");

		// if error occurs do release resources and return
		wgl_make_current(0, 0);
		wgl_delete_context(glContext);

		return false;
	}
	gl_context = glContext;

	return true;
}

#	endif

#endif