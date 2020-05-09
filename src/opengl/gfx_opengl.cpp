#include "../gfx_platform.h"
#include "../glad/include/glad/glad.h"
#include "../stream.h"
#include "../length_string.h"
#include "../systems.h"

#if 0
static const GLenum COLOR_INTERNAL_FORMATS[] = {
	0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F
};
static const GLenum COLOR_FORMATS[] = {
	0, GL_RED, GL_RG, GL_RGB, GL_RGBA
};
static const GLenum INDEX_TYPE[] = {
	0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT
};

enum Render_Option : u32 {
	Render_Option_DEPTH_TEST = 0x1,
	Render_Option_BLEND      = 0x2,
	Render_Option_CULL       = 0x4,
};
typedef u32 Render_Options;

struct Attribute_Layout {
	GLuint  index;
	GLint   count;
	ptrsize offset;
};

struct Uniform {
	GLint location;
	int   type;
};

constexpr int MAX_IN_ATTRIBUTE_LAYOUT = 10;
constexpr int MAX_UNIFORM             = 10;
constexpr int MAX_TEXTURES            = 5;

struct Shader {
	GLuint id;

	Render_Options option;

	Attribute_Layout in_attributes[MAX_IN_ATTRIBUTE_LAYOUT];
	u32              in_attribute_count;
	ptrsize          stride;

	Uniform uniforms[MAX_UNIFORM];
	u32     uniform_count;

	Uniform textures[MAX_TEXTURES];
	u32     texture_count;
};

static constexpr int HDR_TEXTURE_SLOT_INDEX = 5;

static const String hdr_vertex_shader_code = R"(
#version 330 core
const vec2 vertices[] = {
	vec2(-1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0,  1.0),
	vec2(-1.0, -1.0), vec2( 1.0,  1.0), vec2( 1.0, -1.0)
};
const vec2 texture_coords[] = {
	vec2(0, 0), vec2(0, 1), vec2(1, 1),
	vec2(0, 0), vec2(1, 1), vec2(1, 0)
};

out vec2 tex_coord;

void main() {
	gl_Position = vec4(vertices[gl_VertexID], 0, 1);
	tex_coord   = texture_coords[gl_VertexID];
}
)";

static const String hdr_fragment_shader_code = R"(
#version 330 core

layout (location = 0) out vec4 fragment_color;

in vec2 tex_coord;
uniform sampler2D hdr_texture;

void main() {
	vec4 hdr_color = texture(hdr_texture, tex_coord);
	fragment_color = hdr_color;
}
)";

struct Gfx_Platform_OpenGL : public Gfx_Platform {
	Handle platform_handle;
	void * gl_context;

	u32 multisamples;

	GLuint hdr_shader;

	//s32 framebuffer_w;
	//s32 framebuffer_h;
	u32 render_view_w;
	u32 render_view_h;

	GLuint vertex_array;
	//GLuint color_attachment;
	//GLuint depth_attachment;
	//GLuint framebuffer;

	Shader *current_shader            = 0;
	GLenum  current_render_index_type = 0;

	int (*set_swap_interval_func)(int interval);
	int (*get_swap_interval_func)();

	virtual void *get_backend_device() final override {
		return 0;
	}
	virtual void *get_backend_context() final override {
		return gl_context;
	};

	virtual void resize_render_view(u32 w, u32 h) final override;
	virtual void get_render_view_size(u32 *w, u32 *h) final override;

	virtual u32  get_maximum_supported_multisamples() final override;
	virtual u32  get_multisamples() final override;

	virtual void set_swap_interval(s32 interval) final override;
	virtual s32  get_swap_interval() final override;

	virtual void present() final override;

	virtual Handle create_texture2d(s32 w, s32 h, s32 n, const u8 *pixels, Sampler_Params *params = 0) final override;
	virtual void   update_texture2d(Handle texture, s32 xoffset, s32 yoffset, s32 w, s32 h, s32 n, u8 *pixels) final override;
	virtual void   destroy_texture2d(Handle texture) final override;

	virtual Handle create_vertex_buffer(Buffer_Type type, u32 size, void *data) final override;
	virtual Handle create_index_buffer(Buffer_Type type, u32 size, void *data) final override;

	virtual void update_vertex_buffer(Handle handle, u32 offset, u32 size, void *data) final override;
	virtual void update_index_buffer(Handle handle, u32 offset, u32 size, void *data) final override;

	virtual void *map_vertex_buffer(Handle handle, u32 offset, u32 length) final override;
	virtual void  unmap_vertex_buffer(Handle handle) final override;
	virtual void *map_index_buffer(Handle handle, u32 offset, u32 length) final override;
	virtual void  unmap_index_buffer(Handle handle) final override;

	virtual void destroy_vertex_buffer(Handle handle) final override;
	virtual void destroy_index_buffer(Handle handle) final override;

	virtual Framebuffer create_framebuffer(s32 width, s32 height, Texture_Format color, Texture_Format depth) final override;
	virtual void        destroy_framebuffer(Framebuffer framebuffer) final override;

	virtual Handle create_shader(String src) final override;
	virtual void   destroy_shader(Handle shader) final override;

	virtual void bind_framebuffer(Framebuffer *framebuffer) final override;
	virtual void set_render_region(Render_Region &region) final override;
	virtual void clear(Clear_Flag flags, Color4 color) final override;

	virtual void begin(Handle shader, u8 *data, ptrsize size) final override;
	virtual void end() final override;

	virtual void bind_vertex_buffer(Handle buffer) final override;
	virtual void bind_index_buffer(Handle buffer, Render_Index_Type type) final override;
	virtual void bind_texture(Handle texture, u32 index) final override;
	virtual void draw(ptrsize count, ptrsize offset) final override;
	virtual void draw_indexed(ptrsize count, ptrsize offset, ptrsize base_vertex) final override;

	virtual void destroy() final override;

	GLuint create_shader(GLenum type, const String source);

	Gfx_Platform_Info get_info();

	bool load_library(s32 vsync);
	void swap_buffers();

	static void debug_output(GLenum        source,
							 GLenum        type,
							 GLuint        id,
							 GLenum        severity,
							 GLsizei       msgLength,
							 const GLchar *message,
							 const void *  user_param);
};

Gfx_Platform *create_opengl_context(Handle platform, s32 vsync, s32 multisamples) {
	static Gfx_Platform_OpenGL gfx;

	// NOTE: set these, they are used by *load_library* function
	gfx.platform_handle = platform;
	gfx.multisamples    = multisamples;

	if (!gfx.load_library(vsync)) {
		gfx.backend = Render_Backend_NONE;
		return &gfx;
	}

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
	{
		system_log(LOG_INFO, "Gfx:OpenGL", "GL_DEBUG_OUTPUT_SYNCHRONOUS enabled.");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB((GLDEBUGPROCKHR)Gfx_Platform_OpenGL::debug_output, 0);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		// Disable notification message callback
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, nullptr, GL_FALSE);
	}
#endif

	gfx.info = gfx.get_info();
	system_log(LOG_INFO, "Gfx:OpenGL", "gfx.info.vendor               = %s", gfx.info.vendor);
	system_log(LOG_INFO, "Gfx:OpenGL", "gfx.info.renderer             = %s", gfx.info.renderer);
	system_log(LOG_INFO, "Gfx:OpenGL", "gfx.info.version              = %s", gfx.info.version);
	system_log(LOG_INFO, "Gfx:OpenGL", "gfx.info.shading_lang_version = %s", gfx.info.shading_lang_version);

	glGenVertexArrays(1, &gfx.vertex_array);
	glBindVertexArray(gfx.vertex_array);

	// Defaults
	glEnable(GL_MULTISAMPLE);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glGenTextures(1, &gfx.color_attachment);
	//glGenRenderbuffers(1, &gfx.depth_attachment);
	//glGenFramebuffers(1, &gfx.framebuffer);

	auto size = system_get_client_size();
	gfx.resize_render_view(size.x, size.y);

	auto program         = glCreateProgram();
	auto vertex_shader   = gfx.create_shader(GL_VERTEX_SHADER, hdr_vertex_shader_code);
	auto fragment_shader = gfx.create_shader(GL_FRAGMENT_SHADER, hdr_fragment_shader_code);

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint program_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar  message[2048];
		glGetProgramInfoLog(program, 2048, &log_length, message);
		system_log(LOG_WARNING, "Gfx:OpenGL", "OpenGL Shader program link failed (%s):", message);
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	auto hdr_texture = glGetUniformLocation(program, "hdr_texture");
	assert(hdr_texture != -1);
	glUseProgram(program);
	glUniform1i(hdr_texture, HDR_TEXTURE_SLOT_INDEX);
	glUseProgram(0);

	gfx.hdr_shader = program;

	gfx.backend = Render_Backend_OPENGL;

	return &gfx;
}

void Gfx_Platform_OpenGL::resize_render_view(u32 w, u32 h) {
	if (w > 0 && h > 0) {
		//glBindTexture(GL_TEXTURE_2D, color_attachment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//
		//glBindRenderbuffer(GL_RENDERBUFFER, depth_attachment);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, w, h);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_attachment);
		//
		//GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		//if (status != GL_FRAMEBUFFER_COMPLETE) {
		//	trigger_breakpoint();
		//	system_fatal_error("OpenGL: Framebuffer failed to resize!");
		//}
		//
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	render_view_w = w;
	render_view_h = h;
}

void Gfx_Platform_OpenGL::get_render_view_size(u32 *w, u32 *h) {
	*w = render_view_w;
	*h = render_view_h;
}

u32 Gfx_Platform_OpenGL::get_maximum_supported_multisamples() {
	GLint multisamples;
	glGetIntegerv(GL_MAX_SAMPLES, &multisamples);
	return (u32)multisamples;
}

u32 Gfx_Platform_OpenGL::get_multisamples() {
	return multisamples;
}

void Gfx_Platform_OpenGL::set_swap_interval(s32 interval) {
	if (set_swap_interval_func) {
		set_swap_interval_func(interval);
	}
}

s32 Gfx_Platform_OpenGL::get_swap_interval() {
	if (get_swap_interval_func) {
		return get_swap_interval_func();
	} else {
		return 0;
	}
}

void Gfx_Platform_OpenGL::present() {
	//if (framebuffer_w > 0 && framebuffer_h > 0) {
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//	glUseProgram(hdr_shader);
	//	glActiveTexture(GL_TEXTURE0 + HDR_TEXTURE_SLOT_INDEX);
	//	glBindTexture(GL_TEXTURE_2D, color_attachment);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//	current_shader = 0; // reset shader because we used glUseProgram
	//}

	swap_buffers();
}

Handle Gfx_Platform_OpenGL::create_texture2d(s32 w, s32 h, s32 n, const u8 *pixels, Sampler_Params *params) {
	assert(n == 1 || n == 3 || n == 4);

	GLenum min_filter  = GL_NEAREST;
	GLenum mag_filter  = GL_LINEAR;
	GLenum wrap_s      = GL_REPEAT;
	GLenum wrap_t      = GL_REPEAT;
	bool   gen_mipmaps = false;
	bool   srgb        = false;

	if (params) {
		switch (params->min_filter) {
			case Texture_Filter_LINEAR:
				min_filter = GL_LINEAR;
				break;
			case Texture_Filter_NEAREST:
				min_filter = GL_NEAREST;
				break;
				invalid_default_case();
		}
		switch (params->mag_filter) {
			case Texture_Filter_LINEAR:
				mag_filter = GL_LINEAR;
				break;
			case Texture_Filter_NEAREST:
				mag_filter = GL_NEAREST;
				break;
				invalid_default_case();
		}
		switch (params->wrap_s) {
			case Texture_Wrap_CLAMP:
				wrap_s = GL_CLAMP_TO_EDGE;
				break;
			case Texture_Wrap_REPEAT:
				wrap_s = GL_REPEAT;
				break;
				invalid_default_case();
		}
		switch (params->wrap_t) {
			case Texture_Wrap_CLAMP:
				wrap_t = GL_CLAMP_TO_EDGE;
				break;
			case Texture_Wrap_REPEAT:
				wrap_t = GL_REPEAT;
				break;
				invalid_default_case();
		}
		gen_mipmaps = params->gen_mipmaps;
		srgb        = params->srgb;
	}

	GLenum format, internal_format;
	if (n == 1) {
		format          = GL_RED;
		internal_format = GL_RED;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	} else if (n == 3) {
		format = GL_RGB;
		if (srgb) {
			internal_format = GL_SRGB8;
		} else {
			internal_format = GL_RGB8;
		}
	} else if (n == 4) {
		format = GL_RGBA;
		if (srgb) {
			internal_format = GL_SRGB8_ALPHA8;
		} else {
			internal_format = GL_RGBA8;
		}
	}

	GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, pixels);

	if (gen_mipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Handle hres = {};
	hres.h32    = texid;
	return hres;
}

void Gfx_Platform_OpenGL::update_texture2d(Handle texture, s32 xoffset, s32 yoffset, s32 w, s32 h, s32 n, u8 *pixels) {
	assert(n == 1 || n == 3 || n == 4);

	GLenum format;
	if (n == 1) {
		format = GL_RED;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	} else if (n == 3) {
		format = GL_RGB;
	} else if (n == 4) {
		format = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, texture.h32);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, w, h, format, GL_UNSIGNED_BYTE, pixels);
}

void Gfx_Platform_OpenGL::destroy_texture2d(Handle texture) {
	glDeleteTextures(1, &texture.h32);
}

Handle Gfx_Platform_OpenGL::create_vertex_buffer(Buffer_Type type, u32 size, void *data) {
	GLenum usage;
	if (type == Buffer_Type_DYNAMIC) {
		usage = GL_DYNAMIC_DRAW;
	} else if (type == Buffer_Type_STATIC) {
		usage = GL_STATIC_DRAW;
	} else {
		assert(!"Invalid code path");
	}

	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	Handle result = {};
	result.h32    = id;
	return result;
}

Handle Gfx_Platform_OpenGL::create_index_buffer(Buffer_Type type, u32 size, void *data) {
	GLenum usage;
	if (type == Buffer_Type_DYNAMIC) {
		usage = GL_DYNAMIC_DRAW;
	} else if (type == Buffer_Type_STATIC) {
		usage = GL_STATIC_DRAW;
	} else {
		assert(!"Invalid code path");
	}

	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

	Handle result = {};
	result.h32    = id;
	return result;
}

void Gfx_Platform_OpenGL::update_vertex_buffer(Handle handle, u32 offset, u32 size, void *data) {
	glBindBuffer(GL_ARRAY_BUFFER, handle.h32);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Gfx_Platform_OpenGL::update_index_buffer(Handle handle, u32 offset, u32 size, void *data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.h32);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void *Gfx_Platform_OpenGL::map_vertex_buffer(Handle handle, u32 offset, u32 length) {
	glBindBuffer(GL_ARRAY_BUFFER, handle.h32);
	return glMapBufferRange(GL_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT);
}

void Gfx_Platform_OpenGL::unmap_vertex_buffer(Handle handle) {
	(void)handle;
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void *Gfx_Platform_OpenGL::map_index_buffer(Handle handle, u32 offset, u32 length) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.h32);
	return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, length, GL_MAP_WRITE_BIT);
}

void Gfx_Platform_OpenGL::unmap_index_buffer(Handle handle) {
	(void)handle;
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void Gfx_Platform_OpenGL::destroy_vertex_buffer(Handle handle) {
	glDeleteBuffers(1, &handle.h32);
}

void Gfx_Platform_OpenGL::destroy_index_buffer(Handle handle) {
	glDeleteBuffers(1, &handle.h32);
}

Framebuffer Gfx_Platform_OpenGL::create_framebuffer(int width, int height, Texture_Format color, Texture_Format depth) {
	assert(color || depth);

	GLuint color_map = 0;
	GLuint depth_map = 0;

	if (color != Texture_Format_UNKNOWN) {
		glGenTextures(1, &color_map);
		glBindTexture(GL_TEXTURE_2D, color_map);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, COLOR_INTERNAL_FORMATS[color], width, height, 0, COLOR_FORMATS[color], GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (depth != Texture_Format_UNKNOWN) {
		glGenRenderbuffers(1, &depth_map);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_map);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	GLuint id;
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	if (color != Texture_Format_UNKNOWN) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_map, 0);
	}
	if (depth != Texture_Format_UNKNOWN) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_map);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		system_log(LOG_WARNING, "Gfx:OpenGL", "Incomplete framebuffer!");
		trigger_breakpoint();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Framebuffer f;
	f.id.h32       = id;
	f.color_format = color;
	f.depth_format = depth;
	f.color.h32    = color_map;
	f.depth.h32    = depth_map;
	f.width        = width;
	f.height       = height;
	return f;
}

void Gfx_Platform_OpenGL::destroy_framebuffer(Framebuffer framebuffer) {
	glDeleteFramebuffers(1, &framebuffer.id.h32);
	glDeleteTextures(1, &framebuffer.color.h32);
	glDeleteTextures(1, &framebuffer.depth.h32);
}

void Gfx_Platform_OpenGL::destroy() {
	//glDeleteTextures(1, &color_attachment);
	//glDeleteRenderbuffers(1, &depth_attachment);
	//glDeleteFramebuffers(1, &framebuffer);

	glDeleteVertexArrays(1, &vertex_array);
}

Handle Gfx_Platform_OpenGL::create_shader(String src) {
	Shader *shader = new Shader;

	Istream stream  = istream(src.data, src.count);
	auto    options = *istream_consume(&stream, Render_Options);

	struct Attr_Info {
		int index;
		int count;
	};

	Array_View<Attr_Info> atts;
	atts.count = *istream_consume(&stream, s64);
	atts.data  = (Attr_Info *)istream_consume_size(&stream, sizeof(Attr_Info) * atts.count);

	shader->in_attribute_count = (u32)atts.count;
	shader->stride             = 0;
	assert(shader->in_attribute_count < MAX_IN_ATTRIBUTE_LAYOUT);

	for (s64 index = 0; index < atts.count; ++index) {
		auto &a                             = atts[index];
		shader->in_attributes[index].index  = a.index;
		shader->in_attributes[index].count  = a.count;
		shader->in_attributes[index].offset = shader->stride;
		shader->stride += a.count * sizeof(float);
	}

	String vertex_src, fragment_src;

	vertex_src.count   = *istream_consume(&stream, s64);
	vertex_src.data    = (u8 *)istream_consume_size(&stream, vertex_src.count);
	fragment_src.count = *istream_consume(&stream, s64);
	fragment_src.data  = (u8 *)istream_consume_size(&stream, fragment_src.count);

	GLuint vertex   = create_shader(GL_VERTEX_SHADER, vertex_src);
	GLuint fragment = create_shader(GL_FRAGMENT_SHADER, fragment_src);
	defer {
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	GLint program_linked;
	glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar  message[2048];
		glGetProgramInfoLog(program, 2048, &log_length, message);
		system_log(LOG_WARNING, "Gfx:OpenGL", "OpenGL Shader program link failed (%s):", message);
		glDeleteProgram(program);
		Handle result = {};
		return result;
	}

	shader->id     = program;
	shader->option = options;

	String string;

	shader->uniform_count = (u32)(*istream_consume(&stream, s64));
	assert(shader->uniform_count < MAX_UNIFORM);
	for (u32 i = 0; i < shader->uniform_count; ++i) {
		string.count                 = *istream_consume(&stream, s64);
		string.data                  = (u8 *)istream_consume_size(&stream, string.count);
		const char *name             = tto_cstring(string);
		shader->uniforms[i].location = glGetUniformLocation(shader->id, name);
		shader->uniforms[i].type     = *istream_consume(&stream, int);
	}

	shader->texture_count = (u32)(*istream_consume(&stream, s64));
	assert(shader->texture_count < MAX_UNIFORM);
	for (u32 i = 0; i < shader->texture_count; ++i) {
		string.count                 = *istream_consume(&stream, s64);
		string.data                  = (u8 *)istream_consume_size(&stream, string.count);
		const char *name             = tto_cstring(string);
		shader->textures[i].location = glGetUniformLocation(shader->id, name);
		shader->textures[i].type     = *istream_consume(&stream, int);
	}

	Handle handle = {};
	handle.hptr   = shader;
	return handle;
}

void Gfx_Platform_OpenGL::destroy_shader(Handle handle) {
	Shader *shader = (Shader *)handle.hptr;
	glDeleteProgram(shader->id);
	delete shader;
}

void Gfx_Platform_OpenGL::bind_framebuffer(Framebuffer *pframebuffer) {
	if (pframebuffer)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pframebuffer->id);
	else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Gfx_Platform_OpenGL::set_render_region(Render_Region &region) {
	auto &v = region.viewport;
	auto &s = region.scissor;
	glViewport(v.x, v.y, v.w, v.h);
	glScissor(s.x, s.y, s.w, s.h);
}

void Gfx_Platform_OpenGL::clear(Clear_Flag flags, Color4 color) {
	GLenum f = 0;
	if (flags & Clear_Flag_COLOR) f |= GL_COLOR_BUFFER_BIT;
	if (flags & Clear_Flag_DEPTH) f |= GL_DEPTH_BUFFER_BIT;
	if (flags & Clear_Flag_STENCIL) f |= GL_STENCIL_BUFFER_BIT;
	if (f) {
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(f);
	}
}

void Gfx_Platform_OpenGL::begin(Handle hshader, u8 *data, ptrsize size) {
	Shader *shader = (Shader *)hshader.hptr;

	if (shader != current_shader) {
		current_shader = shader;

		glUseProgram(shader->id);

		if (shader->option & Render_Option_BLEND) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}

		if (shader->option & Render_Option_CULL)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		if (shader->option & Render_Option_DEPTH_TEST)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	Istream stream = istream(data, (s64)size);

	for (u32 i = 0; i < shader->uniform_count; ++i) {
		auto &u = shader->uniforms[i];

		r32 *data;

		switch (u.type) {
			case 1:
				data = istream_consume(&stream, r32);
				glUniform1fv(u.location, 1, data);
				break;

			case 2:
				data = (r32 *)(istream_consume(&stream, Vec2));
				glUniform2fv(u.location, 1, data);
				break;

			case 3:
				data = (r32 *)(istream_consume(&stream, Vec3));
				glUniform3fv(u.location, 1, data);
				break;

			case 4:
				data = (r32 *)(istream_consume(&stream, Vec4));
				glUniform4fv(u.location, 1, data);
				break;

			case 16:
				data = (r32 *)(istream_consume(&stream, Mat4));
				glUniformMatrix4fv(u.location, 1, GL_TRUE, data);
				break;

			default:
				invalid_code_path();
				break;
		}
	}

	assert(istream_eof(&stream));
}

void Gfx_Platform_OpenGL::end() {
	current_render_index_type = 0;
}

void Gfx_Platform_OpenGL::bind_vertex_buffer(Handle buffer) {
	glBindBuffer(GL_ARRAY_BUFFER, buffer.h32);

	for (u32 i = 0; i < current_shader->in_attribute_count; ++i) {
		auto &in = current_shader->in_attributes[i];
		glVertexAttribPointer(in.index, in.count, GL_FLOAT, GL_FALSE, (GLsizei)current_shader->stride, (void *)in.offset);
		glEnableVertexAttribArray(in.index);
	}
}

void Gfx_Platform_OpenGL::bind_index_buffer(Handle buffer, Render_Index_Type type) {
	assert(type != Render_Index_Type_NONE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.h32);
	current_render_index_type = INDEX_TYPE[type];
}

void Gfx_Platform_OpenGL::bind_texture(Handle texture, u32 index) {
	assert(index < current_shader->texture_count);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture.h32);
	glUniform1i(current_shader->textures[index].location, (int)index);
}

void Gfx_Platform_OpenGL::draw(ptrsize count, ptrsize offset) {
	glDrawArrays(GL_TRIANGLES, (GLint)offset, (GLsizei)count);
}

void Gfx_Platform_OpenGL::draw_indexed(ptrsize count, ptrsize offset, ptrsize base_vertex) {
	assert(current_render_index_type != 0);
	glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)count, current_render_index_type, (void *)offset, (GLint)base_vertex);
}

Gfx_Platform_Info Gfx_Platform_OpenGL::get_info() {
	Gfx_Platform_Info info    = {};
	info.vendor               = glGetString(GL_VENDOR);
	info.renderer             = glGetString(GL_RENDERER);
	info.version              = glGetString(GL_VERSION);
	info.shading_lang_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	return info;
}

GLuint Gfx_Platform_OpenGL::create_shader(GLenum type, const String source) {
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
		system_log(LOG_WARNING, "Gfx:OpenGL", "OpenGL %s Shader compilation failed: %s", shader_type_name, message);
	}

	return shaderid;
}

void Gfx_Platform_OpenGL::debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei msgLength, const GLchar *message, const void *user_param) {
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

	system_log(LOG_WARNING, "Gfx:OpenGL", "=======================================");
	system_log(LOG_WARNING, "Gfx:OpenGL", "   Code: %u", id);
	system_log(LOG_WARNING, "Gfx:OpenGL", "   Source: %s", srcstr);
	system_log(LOG_WARNING, "Gfx:OpenGL", "   Type: %s", typestr);
	system_log(LOG_WARNING, "Gfx:OpenGL", "   Severity: %s", svrstr);
	system_log(LOG_WARNING, "Gfx:OpenGL", "   Message: %s", message);
	system_log(LOG_WARNING, "Gfx:OpenGL", "=======================================");

	trigger_if(log == LOG_ERROR);
}

#if defined(TARGET_WINDOWS)

//
// Windows OpenGL Context Creation
//

// windows uh...
#	ifdef APIENTRY
#		undef APIENTRY
#	endif

#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>

static HDC wnd_dc;

void Gfx_Platform_OpenGL::swap_buffers() {
	SwapBuffers(wnd_dc);
}

bool Gfx_Platform_OpenGL::load_library(s32 vsync) {
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
		0,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#	if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
		WGL_CONTEXT_FLAGS_ARB,
		WGL_CONTEXT_DEBUG_BIT_ARB,
#	endif
		0,
	};
	HGLRC glContext = wgl_create_context_attribs_arb(wnd_dc, 0, gl_version_attribs);
	wgl_make_current(0, 0);
	wgl_delete_context(dummy_gl_context);
	wgl_make_current(wnd_dc, glContext);

	if (set_swap_interval_func) {
		if (vsync == -1) {
			if (!set_swap_interval_func(vsync)) {
				system_log(0, "Gfx:OpenGL", "Tearing (-1 swap interval) not supported.");
				set_swap_interval_func(1);
			}
		} else {
			set_swap_interval_func(vsync);
		}
	} else {
		system_log(LOG_WARNING, "Gfx:OpenGL", "WGL_EXT_swap_control extension is not present, Vertical swapping disabled");
	}

	if (!gladLoadGL()) {
		system_log(LOG_ERROR, "Gfx:OpenGL", "gladLoadGLLoader() - Failed to Load Proc Address for OpenGL");

		// if error occurs do release resources and return
		wgl_make_current(0, 0);
		wgl_delete_context(glContext);

		return false;
	}
	gl_context = glContext;

	return true;
}

#endif

#endif