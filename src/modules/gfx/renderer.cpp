#include "renderer.h"
#include "modules/core/gfx_platform.h"
#include "modules/core/systems.h"
#include "shaders/gfx_shaders.h"

#pragma pack(push, 4)

struct alignas(16) Im_Uniform {
	Mat4 transform;
};

#pragma pack(pop)

struct Im_Vertex2d {
	Vec3   position;
	Vec2   tex_coord;
	Color4 color;
};

struct Im_Vertex3d {
	Vec3   position;
	Vec2   tex_coord;
	Vec3   normal;
	Color4 color;
};

typedef u16 Im_Index;

struct Im_Draw_Cmd {
	Texture_View texture;
	u32          vertex;
	u32          index;
	u32          count;
};

static constexpr u32 IM_CONTEXT_MAX_VERTICES        = 4096 * 4;
static constexpr u32 IM_CONTEXT_MAX_INDICES         = IM_CONTEXT_MAX_VERTICES * 6;
static constexpr u32 IM_CONTEXT_MAX_TRANSFORMATIONS = 100;
static r32           im_unit_circle_cos[IM_MAX_CIRCLE_SEGMENTS];
static r32           im_unit_circle_sin[IM_MAX_CIRCLE_SEGMENTS];

struct Im_Context2d {
	Render_Pipeline pipeline;
	Uniform_Buffer  uniform_buffer;
	Vertex_Buffer   vertex_buffer;
	Index_Buffer    index_buffer;
	Sampler         sampler;

	Im_Draw_Cmd draw_cmds[IM_CONTEXT_MAX_VERTICES];
	u32         draw_cmd;
	Im_Vertex2d * vertex_ptr;
	u32         vertex;
	Im_Index *  index_ptr;
	Im_Index    last_index;
	u32         index;
	Im_Index    counter;

	Mat4 transformations[IM_CONTEXT_MAX_TRANSFORMATIONS];
	u32  transformation;
};

struct Im_Context3d {
	Render_Pipeline pipeline;
	Uniform_Buffer  uniform_buffer;
	Vertex_Buffer   vertex_buffer;
	Index_Buffer    index_buffer;
	Sampler         sampler;

	Im_Draw_Cmd		draw_cmds[IM_CONTEXT_MAX_VERTICES];
	u32				draw_cmd;
	Im_Vertex3d		* vertex_ptr;
	u32				vertex;
	Im_Index *		index_ptr;
	Im_Index		last_index;
	u32				index;
	Im_Index		counter;

	Mat4 transformations[IM_CONTEXT_MAX_TRANSFORMATIONS];
	u32  transformation;
};

struct Hdr_Data {
	Texture2d          hdr_color_buffer[2];
	Texture2d          hdr_depth_buffer;
	Texture_View       hdr_color_view[2];
	Depth_Stencil_View hdr_depth_view;
	Framebuffer        hdr_framebuffer;

	Texture2d          bloom_color_buffer[2];
	Texture_View       bloom_color_view[2];
	Framebuffer        bloom_framebuffer[2];
};

static Im_Context2d     im_context2d;
static Im_Context3d     im_context3d;
Render_Pipeline         debug_pipeline;
static Hdr_Data         hdr_data;
static Render_Pipeline  blur_pipelines[2];
static Texture2d_Handle white_texture;
static Render_Pipeline  hdr_pipeline;
static Sampler          postprocess_sampler;
static u32              framebuffer_w;
static u32              framebuffer_h;
static u32              bloom_texture_w;
static u32              bloom_texture_h;
static Gfx_Platform *   gfx;
static u32              shader_lang;

//
//
//

r32 im2d_stroke_weight = 1.0f;

//
//
//

static String igfx_find_shader(String content, u32 shader_tag, const char *name = 0) {
	if (name == 0) name = "-unknown-";
	shader_tag |= shader_lang;

	u32 magic_number = bswap32p_le(content.data);
	if (magic_number != SHADER_MAGIC_NUMBER) {
		system_log(LOG_ERROR, "Gfx", "Shader: %s is invalid or corrupted", name);
		return {};
	}

	String                result     = {};
	u32                   elem_count = bswap32p_le(content.data + 4);
	Shader_Table_Element *elem       = (Shader_Table_Element *)(content.data + 8);
	u8 *                  ptr        = content.data + 8 + sizeof(Shader_Table_Element) * elem_count;

	for (u32 index = 0; index < elem_count; ++elem_count) {
		if (elem->flag == shader_tag) {
			result.data  = ptr + elem->offset;
			result.count = elem->size;
			break;
		}
		elem += 1;
	}

	if (result.count == 0) {
		system_log(LOG_ERROR, "Gfx", "Shader %s not found", name);
	}

	return result;
}

bool gfx_create_context(Handle platform, Render_Backend backend, Vsync vsync, u32 multisamples, u32 framebuffer_w, u32 framebuffer_h) {
	if (backend == Render_Backend_NONE) {
		system_log(LOG_INFO, "Gfx", "gfx.backend = Render_Backend_NONE");
		gfx = create_null_context(platform, vsync, multisamples);
		shader_lang = 0;
	} else if (backend == Render_Backend_OPENGL) {
		system_log(LOG_INFO, "Gfx", "gfx.backend = Render_Backend_OPENGL");
		gfx = create_opengl_context(platform, vsync, multisamples);
		shader_lang = SHADER_LANG_GLSL;
	} else if (backend == Render_Backend_DIRECTX11) {
		system_log(LOG_INFO, "Gfx", "gfx.backend = Render_Backend_DIRECTX11");
		gfx = create_directx11_context(platform, vsync, multisamples);
		shader_lang = SHADER_LANG_HLSL;
	} else {
		invalid_code_path();
	}

	{
		auto info = gfx->get_info();
		system_log(LOG_INFO, "Gfx", "Renderer: %s, Version %s", info->renderer, info->version);
		system_log(LOG_INFO, "Gfx", "Vendor: %s", info->vendor);
		system_log(LOG_INFO, "Gfx", "Shading Language: %s", info->shading_lang);

		if (gfx->backend != backend) {
			system_log(LOG_ERROR, "Gfx", "Context creation failed.");
			return false;
		}
	}

	gfx_resize_framebuffer(framebuffer_w, framebuffer_h);

	const u8  white_pixels[] = { 0xff, 0xff, 0xff, 0xff };
	const u8 *pixels[]       = { white_pixels };
	white_texture.buffer     = gfx->create_texture2d(1, 1, 4, Data_Format_RGBA8_UNORM, pixels, Buffer_Usage_IMMUTABLE, Texture_Bind_SHADER_RESOURCE, 1);
	white_texture.view       = gfx->create_texture_view(white_texture.buffer);

	{
		String hdr_shader = system_read_entire_file("data/shaders/hdr.kfx");

		Shader_Info shader;
		shader.input_layouts       = NULL;
		shader.input_layouts_count = 0;
		shader.vertex              = igfx_find_shader(hdr_shader, SHADER_TYPE_VERTEX, "hdr.kfx.vertex");
		shader.pixel               = igfx_find_shader(hdr_shader, SHADER_TYPE_PIXEL, "hdr.kfx.pixel");
		shader.stride              = 0;

		Rasterizer_Info rasterizer = rasterizer_info_create();
		Blend_Info      blend      = blend_info_disabled();
		Depth_Info      depth      = depth_info_create(false, Depth_Write_Mask_ZERO);

		hdr_pipeline = gfx->create_render_pipeline(shader, rasterizer, blend, depth, "postprocess");

		memory_free(hdr_shader.data);
	}

	{
		String h_blur_shader = system_read_entire_file("data/shaders/hblur.kfx");
		String v_blur_shader = system_read_entire_file("data/shaders/vblur.kfx");

		Shader_Info shader[2];
		shader[0].input_layouts       = NULL;
		shader[0].input_layouts_count = 0;
		shader[0].vertex              = igfx_find_shader(h_blur_shader, SHADER_TYPE_VERTEX, "hblur.kfx.vertex");
		shader[0].pixel               = igfx_find_shader(h_blur_shader, SHADER_TYPE_PIXEL, "hblur.kfx.pixel");
		shader[0].stride              = 0;

		shader[1].input_layouts       = NULL;
		shader[1].input_layouts_count = 0;
		shader[1].vertex              = igfx_find_shader(v_blur_shader, SHADER_TYPE_VERTEX, "vblur.kfx.vertex");
		shader[1].pixel               = igfx_find_shader(v_blur_shader, SHADER_TYPE_PIXEL, "vblur.kfx.pixel");
		shader[1].stride              = 0;

		Rasterizer_Info rasterizer = rasterizer_info_create();
		Blend_Info      blend      = blend_info_disabled();
		Depth_Info      depth      = depth_info_create(false);

		blur_pipelines[0] = gfx->create_render_pipeline(shader[0], rasterizer, blend, depth, "h_blur");
		blur_pipelines[1] = gfx->create_render_pipeline(shader[1], rasterizer, blend, depth, "v_blur");

		memory_free(h_blur_shader.data);
		memory_free(v_blur_shader.data);
	}

	postprocess_sampler = gfx->create_sampler(filter_create(), texture_address_create(), level_of_detail_create());

	{
		Input_Element_Layout layouts[] = {
			{ "POSITION", Data_Format_RGB32_FLOAT, offsetof(Im_Vertex2d, position), Input_Type_PER_VERTEX_DATA, 0 },
			{ "TEX_COORD", Data_Format_RG32_FLOAT, offsetof(Im_Vertex2d, tex_coord), Input_Type_PER_VERTEX_DATA, 0 },
			{ "COLOR", Data_Format_RGBA32_FLOAT, offsetof(Im_Vertex2d, color), Input_Type_PER_VERTEX_DATA, 0 }
		};

		String quad_shader_content = system_read_entire_file("data/shaders/quad.kfx");

		Shader_Info shader;
		shader.input_layouts       = layouts;
		shader.input_layouts_count = static_count(layouts);
		shader.vertex              = igfx_find_shader(quad_shader_content, SHADER_TYPE_VERTEX, "quad.kfx.vertex");
		shader.pixel               = igfx_find_shader(quad_shader_content, SHADER_TYPE_PIXEL, "quad.kfx.pixel");
		shader.stride              = sizeof(Im_Vertex2d);

		Rasterizer_Info rasterizer = rasterizer_info_create();
		rasterizer.cull_mode       = Cull_Mode_BACK;

		Blend_Info blend = blend_info_create(Blend_SRC_ALPHA, Blend_INV_SRC_ALPHA, Blend_Operation_ADD, Blend_SRC_ALPHA, Blend_INV_SRC_ALPHA, Blend_Operation_ADD);
		Depth_Info depth = depth_info_create(false, Depth_Write_Mask_ZERO, Comparison_Function_NEVER);

		im_context2d.pipeline = gfx->create_render_pipeline(shader, rasterizer, blend, depth, "Im2d_Pipeline");
		memory_free(quad_shader_content.data);

		String      debug_shader_content = system_read_entire_file("data/shaders/debug.kfx");
		Shader_Info debug_shader;
		debug_shader.input_layouts       = layouts;
		debug_shader.input_layouts_count = static_count(layouts);
		debug_shader.vertex              = igfx_find_shader(debug_shader_content, SHADER_TYPE_VERTEX, "debug.kfx.vertex");
		debug_shader.pixel               = igfx_find_shader(debug_shader_content, SHADER_TYPE_PIXEL, "debug.kfx.pixel");
		debug_shader.stride              = sizeof(Im_Vertex2d);
		rasterizer.cull_mode			 = Cull_Mode_NONE;
		depth                            = depth_info_create(false, Depth_Write_Mask_ZERO, Comparison_Function_NEVER);
		debug_pipeline                   = gfx->create_render_pipeline(debug_shader, rasterizer, blend, depth, "Debug_Pipeline");
		memory_free(debug_shader_content.data);
	}

	im_context2d.uniform_buffer = gfx->create_uniform_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Mat4), 0);
	im_context2d.vertex_buffer  = gfx->create_vertex_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Im_Vertex2d) * IM_CONTEXT_MAX_VERTICES, 0);
	im_context2d.index_buffer   = gfx->create_index_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Im_Index) * IM_CONTEXT_MAX_INDICES, 0);

	im_context2d.sampler = gfx->create_sampler(filter_create(Filter_Type_MIN_MAG_MIP_POINT), texture_address_create(), level_of_detail_create());

	im_context2d.transformations[0] = mat4_identity();
	im_context2d.transformation     = 1;

	{
		Input_Element_Layout layouts[] = {
		{ "POSITION", Data_Format_RGB32_FLOAT, offsetof(Im_Vertex3d, position), Input_Type_PER_VERTEX_DATA, 0 },
		{ "TEX_COORD", Data_Format_RG32_FLOAT, offsetof(Im_Vertex3d, tex_coord), Input_Type_PER_VERTEX_DATA, 0 },
		{ "NORMAL", Data_Format_RGB32_FLOAT, offsetof(Im_Vertex3d, normal), Input_Type_PER_VERTEX_DATA, 0 },
		{ "COLOR", Data_Format_RGBA32_FLOAT, offsetof(Im_Vertex3d, color), Input_Type_PER_VERTEX_DATA, 0 }
		};

		String im3d_shader_content = system_read_entire_file("data/shaders/im3d.kfx");

		Shader_Info shader;
		shader.input_layouts       = layouts;
		shader.input_layouts_count = static_count(layouts);
		shader.vertex              = igfx_find_shader(im3d_shader_content, SHADER_TYPE_VERTEX, "im3d.kfx.vertex");
		shader.pixel               = igfx_find_shader(im3d_shader_content, SHADER_TYPE_PIXEL, "im3d.kfx.pixel");
		shader.stride              = sizeof(Im_Vertex3d);

		Rasterizer_Info rasterizer = rasterizer_info_create();
		rasterizer.cull_mode = Cull_Mode_NONE;

		Blend_Info blend = blend_info_create(Blend_SRC_ALPHA, Blend_INV_SRC_ALPHA, Blend_Operation_ADD, Blend_SRC_ALPHA, Blend_INV_SRC_ALPHA, Blend_Operation_ADD);
		Depth_Info depth = depth_info_create(true, Depth_Write_Mask_ALL, Comparison_Function_LESS);

		im_context3d.pipeline = gfx->create_render_pipeline(shader, rasterizer, blend, depth, "Im3d_Pipeline");
		memory_free(im3d_shader_content.data);
	}

	im_context3d.uniform_buffer = gfx->create_uniform_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Mat4), 0);
	im_context3d.vertex_buffer  = gfx->create_vertex_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Im_Vertex3d) * IM_CONTEXT_MAX_VERTICES, 0);
	im_context3d.index_buffer   = gfx->create_index_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Im_Index) * IM_CONTEXT_MAX_INDICES, 0);

	im_context3d.sampler = gfx->create_sampler(filter_create(Filter_Type_MIN_MAG_MIP_POINT), texture_address_create(), level_of_detail_create());

	im_context3d.transformations[0] = mat4_identity();
	im_context3d.transformation     = 1;

	for (int i = 0; i < IM_MAX_CIRCLE_SEGMENTS; ++i) {
		r32 theta             = ((r32)i / (r32)IM_MAX_CIRCLE_SEGMENTS) * MATH_PI * 2;
		im_unit_circle_cos[i] = cosf(theta);
		im_unit_circle_sin[i] = sinf(theta);
	}

	im_unit_circle_cos[IM_MAX_CIRCLE_SEGMENTS - 1] = 1;
	im_unit_circle_sin[IM_MAX_CIRCLE_SEGMENTS - 1] = 0;

	return true;
}

void igfx_destory_global_data() {
	if (hdr_data.hdr_color_buffer[0].id) gfx->destroy_texture2d(hdr_data.hdr_color_buffer[0]);
	if (hdr_data.hdr_color_buffer[1].id) gfx->destroy_texture2d(hdr_data.hdr_color_buffer[1]);
	if (hdr_data.hdr_depth_buffer.id) gfx->destroy_texture2d(hdr_data.hdr_depth_buffer);
	if (hdr_data.hdr_color_view[0].id) gfx->destroy_texture_view(hdr_data.hdr_color_view[0]);
	if (hdr_data.hdr_color_view[1].id) gfx->destroy_texture_view(hdr_data.hdr_color_view[1]);
	if (hdr_data.hdr_depth_view.id) gfx->destroy_depth_stencil_view(hdr_data.hdr_depth_view);
	if (hdr_data.hdr_framebuffer.id) gfx->destroy_framebuffer(hdr_data.hdr_framebuffer);

	for (int index = 0; index < 2; ++index) {
		if (hdr_data.bloom_framebuffer[index].id) gfx->destroy_framebuffer(hdr_data.bloom_framebuffer[index]);
		if (hdr_data.bloom_color_buffer[index].id) gfx->destroy_texture2d(hdr_data.bloom_color_buffer[index]);
		if (hdr_data.bloom_color_view[index].id) gfx->destroy_texture_view(hdr_data.bloom_color_view[index]);
	}

	memset(&hdr_data, 0, sizeof(hdr_data));
}

void gfx_resize_framebuffer(u32 width, u32 height, u32 bloom_w, u32 bloom_h) {
	igfx_destory_global_data();

	framebuffer_w = width;
	framebuffer_h = height;

	bloom_texture_w = bloom_w;
	bloom_texture_h = bloom_h;

	
	for (int index = 0; index < 2; ++index) {
		hdr_data.hdr_color_buffer[index] = gfx->create_texture2d(width, height, 4, Data_Format_RGBA16_FLOAT, 0, Buffer_Usage_DEFAULT, Texture_Bind_SHADER_RESOURCE | Texture_Bind_RENDER_TARGET, 1);
		hdr_data.hdr_color_view[index] = gfx->create_texture_view(hdr_data.hdr_color_buffer[index]);

		hdr_data.bloom_color_buffer[index] = gfx->create_texture2d(bloom_w, bloom_h, 4, Data_Format_RGBA16_FLOAT, 0, Buffer_Usage_DEFAULT, Texture_Bind_SHADER_RESOURCE | Texture_Bind_RENDER_TARGET, 1);
		hdr_data.bloom_color_view[index] = gfx->create_texture_view(hdr_data.bloom_color_buffer[index]);
		hdr_data.bloom_framebuffer[index] = gfx->create_framebuffer(1, &hdr_data.bloom_color_buffer[index], &hdr_data.bloom_color_view[index], NULL);
	}

	hdr_data.hdr_depth_buffer = gfx->create_texture2d(width, height, 1, Data_Format_D32_FLOAT, 0, Buffer_Usage_DEFAULT, Texture_Bind_DEPTH_STENCIL, 1);
	hdr_data.hdr_depth_view   = gfx->create_depth_stencil_view(hdr_data.hdr_depth_buffer);

	hdr_data.hdr_framebuffer = gfx->create_framebuffer(2, hdr_data.hdr_color_buffer, hdr_data.hdr_color_view, &hdr_data.hdr_depth_view);
}

void gfx_destroy_context() {
	gfx->destroy_vertex_buffer(im_context2d.vertex_buffer);
	gfx->destroy_index_buffer(im_context2d.index_buffer);
	gfx->destroy_uniform_buffer(im_context2d.uniform_buffer);
	gfx->destory_render_pipeline(im_context2d.pipeline);
	gfx->destory_sampler(im_context2d.sampler);

	gfx->destroy_vertex_buffer(im_context3d.vertex_buffer);
	gfx->destroy_index_buffer(im_context3d.index_buffer);
	gfx->destroy_uniform_buffer(im_context3d.uniform_buffer);
	gfx->destory_render_pipeline(im_context3d.pipeline);
	gfx->destory_sampler(im_context3d.sampler);

	gfx->destroy_texture_view(white_texture.view);
	gfx->destroy_texture2d(white_texture.buffer);
	igfx_destory_global_data();
	gfx->destroy();
}

Render_Backend gfx_render_backend() {
	return gfx->backend;
}

void *gfx_render_device() {
	return gfx->get_backend_device();
}

void *gfx_render_context() {
	return gfx->get_backend_context();
}

void gfx_on_client_resize(u32 w, u32 h) {
	gfx->on_client_resize(w, h);
}

void gfx_get_render_view_size(u32 *w, u32 *h) {
	gfx->get_render_view_size(w, h);
}

Mat4 gfx_view_transform(Camera_View &view) {
	auto backend = gfx_render_backend();
	if (view.kind == ORTHOGRAPHIC) {
		switch (backend) {
		case Render_Backend_NONE: return mat4_identity();
		case Render_Backend_OPENGL: return mat4_ortho_gl(view.orthographic.left, view.orthographic.right, 
			view.orthographic.top, view.orthographic.bottom, 
			view.orthographic.near, view.orthographic.far);
		case Render_Backend_DIRECTX11: return mat4_ortho_dx(view.orthographic.left, view.orthographic.right,
			view.orthographic.top, view.orthographic.bottom,
			view.orthographic.near, view.orthographic.far);
			invalid_default_case();
		}
	} else {
		switch (backend) {
		case Render_Backend_NONE: return mat4_identity();
		case Render_Backend_OPENGL: return mat4_perspective_gl(view.perspective.field_of_view,
			view.perspective.aspect_ratio, 
			view.perspective.near_plane, 
			view.perspective.far_plane);
		case Render_Backend_DIRECTX11: return mat4_perspective_dx(view.perspective.field_of_view,
			view.perspective.aspect_ratio, 
			view.perspective.near_plane, 
			view.perspective.far_plane);
			invalid_default_case();
		}
	}

	return mat4_identity();
}

void gfx_set_sync_interval(Vsync vsync) {
	gfx->set_sync_interval(vsync);
}

Vsync gfx_get_sync_interval() {
	return gfx->get_sync_interval();
}

s32 gfx_get_multisamples() {
	return gfx->get_multisamples();
}

void gfx_present() {
	gfx->present();
}

Texture2d_Handle gfx_create_texture2d(u32 w, u32 h, u32 channels, Data_Format fmt, const u8 **pixels, Buffer_Usage usage, u32 mip_levels) {
	Texture2d_Handle handle;
	handle.buffer = gfx->create_texture2d(w, h, channels, fmt, pixels, usage, Texture_Bind_SHADER_RESOURCE, mip_levels);
	handle.view   = gfx->create_texture_view(handle.buffer);
	return handle;
}

void gfx_update_texture2d(Texture2d_Handle handle, u32 xoffset, u32 yoffset, u32 w, u32 h, u32 channels, u8 *pixels) {
	gfx->update_texture2d(handle.buffer, xoffset, yoffset, w, h, channels, pixels);
}

void gfx_destroy_texture2d(Texture2d_Handle handle) {
	gfx->destroy_texture_view(handle.view);
	gfx->destroy_texture2d(handle.buffer);
}

inline void im2d_start_cmd_record(Texture_View texture) {
	auto draw_cmd     = im_context2d.draw_cmds + im_context2d.draw_cmd;
	draw_cmd->index   = im_context2d.index;
	draw_cmd->vertex  = im_context2d.vertex;
	draw_cmd->texture = texture;
	draw_cmd->count   = 0;
}

inline Im_Vertex2d *iim2d_push_vertex_count(int count) {
	assert(im_context2d.vertex + count <= IM_CONTEXT_MAX_VERTICES);

	auto vertex = im_context2d.vertex_ptr;
	im_context2d.vertex_ptr += count;
	im_context2d.vertex += count;
	return vertex;
}

inline void im2d_push_vertices(Im_Vertex2d *vertices, int count) {
	auto dst = iim2d_push_vertex_count(count);

	if (im_context2d.transformation > 1) {
		auto &trans = im_context2d.transformations[im_context2d.transformation - 1];
		for (int i = 0; i < count; ++i) {
			vertices[i].position = (trans * vec4(vertices[i].position, 1)).xyz;
		}
	}

	memcpy(dst, vertices, sizeof(Im_Vertex2d) * count);
}

inline void im2d_push_vertex(Vec3 position, Vec2 texture_coord, Color4 color) {
	Im_Vertex2d src = { position, texture_coord, color };
	im2d_push_vertices(&src, 1);
}

inline Im_Index *iim2d_push_index_count(int count) {
	assert(im_context2d.index + count <= IM_CONTEXT_MAX_INDICES);
	auto index = im_context2d.index_ptr;
	im_context2d.index_ptr += count;
	im_context2d.index += count;
	im_context2d.counter += count;
	return index;
}

#define im2d_get_last_index() (im_context2d.counter ? im_context2d.last_index + 1 : im_context2d.last_index)
#define im2d_get_draw_cmd()   (im_context2d.draw_cmds + im_context2d.draw_cmd)

inline void im2d_push_indices(Im_Index *indices, int count) {
	assert(im_context2d.index + count <= IM_CONTEXT_MAX_INDICES);
	auto prev_counter = im_context2d.counter;

	auto dst = iim2d_push_index_count(count);

	if (prev_counter) {
		auto next_index = im_context2d.last_index + 1;
		for (int i = 0; i < count; ++i) {
			indices[i] += next_index;
		}
	}

	im_context2d.last_index = indices[count - 1];
	memcpy(dst, indices, count * sizeof(Im_Vertex2d));
}

inline void im2d_push_draw_cmd() {
	auto last_cmd   = im_context2d.draw_cmds + im_context2d.draw_cmd;
	last_cmd->count = im_context2d.counter;
	im_context2d.draw_cmd += 1;
	im_context2d.counter = 0;
}

void iim2d_flush(bool restart = true);

inline void im2d_ensure_size(int vertex_count, int index_count) {
	if (im_context2d.index + index_count - 1 >= IM_CONTEXT_MAX_INDICES ||
		im_context2d.vertex + vertex_count - 1 >= IM_CONTEXT_MAX_VERTICES) {
		im2d_push_draw_cmd();
		iim2d_flush();
	}
}

void gfx_apply_blur(Texture_View src, Framebuffer ping_pongs[2], Texture_View dsts[2], Rect rect, int amount) {
	amount *= 2;

	gfx->begin_drawing(ping_pongs[0], Clear_COLOR, vec4(1), 1, 0xff);
	gfx->cmd_bind_pipeline(blur_pipelines[0]);
	gfx->cmd_set_viewport(rect.x, rect.y, rect.w, rect.h);
	gfx->cmd_bind_samplers(0, 1, &postprocess_sampler);
	gfx->cmd_bind_textures(0, 1, &src);
	gfx->cmd_draw(6, 0);
	gfx->end_drawing();

	for (int i = 1; i < amount; ++i) {
		int index = i & 1;
		gfx->begin_drawing(ping_pongs[index], Clear_NONE, vec4(1), 1, 0xff);
		gfx->cmd_bind_pipeline(blur_pipelines[index]);
		gfx->cmd_set_viewport(rect.x, rect.y, rect.w, rect.h);
		gfx->cmd_bind_samplers(0, 1, &postprocess_sampler);
		gfx->cmd_bind_textures(0, 1, &dsts[index != 1]);
		gfx->cmd_draw(6, 0);
		gfx->end_drawing();
	}
}

void gfx_apply_bloom(int amount) {
	auto viewport = Rect{ 0, 0, (r32)bloom_texture_w, (r32)bloom_texture_h };
	gfx_apply_blur(hdr_data.hdr_color_view[1], hdr_data.bloom_framebuffer, hdr_data.bloom_color_view, viewport, 4);
}

void gfx_blit_hdr(r32 x, r32 y, r32 w, r32 h) {
	Sampler samplers[] = {
		postprocess_sampler,
		postprocess_sampler
	};

	Texture_View textures[] = {
		hdr_data.hdr_color_view[0],
		hdr_data.bloom_color_view[1]
	};

	gfx->cmd_bind_pipeline(hdr_pipeline);
	gfx->cmd_set_viewport(x, y, w, h);
	gfx->cmd_bind_samplers(0, static_count(samplers), samplers);
	gfx->cmd_bind_textures(0, static_count(textures), textures);
	gfx->cmd_draw(6, 0);
}

void gfx_begin_drawing(Framebuffer_Type type, Clear_Flags flags, Color4 color, r32 depth, u8 stencil) {
	switch (type) {
		case Framebuffer_Type_DEFAULT: {
			gfx->begin_drawing(gfx->get_default_framebuffer(), flags, color, depth, stencil);
		} break;

		case Framebuffer_Type_HDR: {
			gfx->begin_drawing(hdr_data.hdr_framebuffer, flags, color, depth, stencil);
		} break;

			invalid_default_case();
	}
}

void gfx_end_drawing() {
	gfx->end_drawing();
}

void gfx_viewport(r32 x, r32 y, r32 w, r32 h) {
	gfx->cmd_set_viewport(x, y, w, h);
}

void im2d_begin(const Mat4 &transform) {
	im_context2d.draw_cmd       = 0;
	im_context2d.vertex         = 0;
	im_context2d.index          = 0;
	im_context2d.counter        = 0;
	im_context2d.transformation = 1;

	im_context2d.vertex_ptr = (Im_Vertex2d *)gfx->map(im_context2d.vertex_buffer, Map_Type_WRITE_DISCARD);
	im_context2d.index_ptr  = (Im_Index *)gfx->map(im_context2d.index_buffer, Map_Type_WRITE_DISCARD);

	im2d_start_cmd_record(white_texture.view);

	Im_Uniform uniform;
	uniform.transform = transform;

	void *ptr = gfx->map(im_context2d.uniform_buffer, Map_Type_WRITE_DISCARD);
	memcpy(ptr, &uniform, sizeof(uniform));
	gfx->unmap(im_context2d.uniform_buffer);

	gfx->cmd_bind_pipeline(im_context2d.pipeline);
	gfx->cmd_bind_samplers(0, 1, &im_context2d.sampler);
	gfx->cmd_bind_vs_uniform_buffers(0, 1, &im_context2d.uniform_buffer);
}

void im2d_begin(Camera_View &view, const Mat4 &transform) {
	Mat4 projection = gfx_view_transform(view);
	im2d_begin(projection * transform);
}

void iim2d_flush(bool restart) {
	gfx->unmap(im_context2d.vertex_buffer);
	gfx->unmap(im_context2d.index_buffer);

	if (im_context2d.draw_cmd) {
		gfx->cmd_bind_vertex_buffer(im_context2d.vertex_buffer, sizeof(Im_Vertex2d));
		gfx->cmd_bind_index_buffer(im_context2d.index_buffer, Index_Type_U16);

		for (u32 draw_cmd_index = 0; draw_cmd_index < im_context2d.draw_cmd; ++draw_cmd_index) {
			Im_Draw_Cmd *cmd = im_context2d.draw_cmds + draw_cmd_index;

			gfx->cmd_bind_textures(0, 1, &cmd->texture);
			gfx->cmd_draw_indexed(cmd->count, cmd->index, cmd->vertex);
		}

		auto bound_texture = im_context2d.draw_cmds[im_context2d.draw_cmd - 1].texture;

		im_context2d.draw_cmd = 0;
		im_context2d.vertex   = 0;
		im_context2d.index    = 0;
		im_context2d.counter  = 0;

		im2d_start_cmd_record(bound_texture);
	}

	if (restart) {
		im_context2d.vertex_ptr = (Im_Vertex2d *)gfx->map(im_context2d.vertex_buffer, Map_Type_WRITE_DISCARD);
		im_context2d.index_ptr  = (Im_Index *)gfx->map(im_context2d.index_buffer, Map_Type_WRITE_DISCARD);
	}
}

void im2d_end() {
	if (im_context2d.counter) {
		im2d_push_draw_cmd();
	}
	iim2d_flush(false);
}

void im2d_bind_texture(Texture2d_Handle handle) {
	if (im_context2d.draw_cmds[im_context2d.draw_cmd].texture.id == handle.view.id)
		return;

	if (im_context2d.counter) {
		im2d_push_draw_cmd();
		im2d_start_cmd_record(handle.view);
	} else {
		auto draw_cmd     = im2d_get_draw_cmd();
		draw_cmd->texture = handle.view;
	}
}

void im2d_unbind_texture() {
	im2d_bind_texture(white_texture);
}

void im2d_push_matrix(const Mat4 &matrix) {
	assert(im_context2d.transformation < IM_CONTEXT_MAX_TRANSFORMATIONS);
	im_context2d.transformations[im_context2d.transformation] = im_context2d.transformations[im_context2d.transformation - 1] * matrix;
	im_context2d.transformation += 1;
}

void im2d_pop_matrix() {
	assert(im_context2d.transformation > 1);
	im_context2d.transformation -= 1;
}

void im2d_flush_transformations() {
	im_context2d.transformation = 1;
}

void im2d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color) {
	im2d_ensure_size(3, 3);

	Im_Index  indices[]  = { 0, 1, 2 };
	Im_Vertex2d vertices[] = {
		{ a, uv_a, color },
		{ b, uv_b, color },
		{ c, uv_c, color },
	};

	im2d_push_indices(indices, static_count(indices));
	im2d_push_vertices(vertices, static_count(vertices));
}

void im2d_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color) {
	im2d_triangle(vec3(a, 1), vec3(b, 1), vec3(c, 1), uv_a, uv_b, uv_c, color);
}

void im2d_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color) {
	im2d_triangle(a, b, c, vec2(0), vec2(0), vec2(0), color);
}

void im2d_triangle(Vec2 a, Vec2 b, Vec2 c, Color4 color) {
	im2d_triangle(vec3(a, 1), vec3(b, 1), vec3(c, 1), color);
}

void im2d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im2d_ensure_size(4, 6);

	Im_Index  indices[]  = { 0, 1, 2, 0, 2, 3 };
	Im_Vertex2d vertices[] = {
		{ a, uv_a, color },
		{ b, uv_b, color },
		{ c, uv_c, color },
		{ d, uv_d, color },
	};

	im2d_push_indices(indices, static_count(indices));
	im2d_push_vertices(vertices, static_count(vertices));
}

void im2d_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im2d_quad(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color) {
	im2d_quad(a, b, c, d, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color) {
	im2d_quad(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), color);
}

void im2d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im2d_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Mm_Rect rect, Color4 color) {
	im2d_quad(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), rect, color);
}

void im2d_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec3 a = pos;
	Vec3 b = vec3(pos.x, pos.y + dim.y, pos.z);
	Vec3 c = vec3(pos.xy + dim, pos.z);
	Vec3 d = vec3(pos.x + dim.x, pos.y, pos.z);
	im2d_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im2d_rect(vec3(pos, 1), dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect(Vec3 pos, Vec2 dim, Color4 color) {
	im2d_rect(pos, dim, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_rect(Vec2 pos, Vec2 dim, Color4 color) {
	im2d_rect(vec3(pos, 1), dim, color);
}

void im2d_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im2d_rect(pos, dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	im2d_rect(vec3(pos, 1), dim, rect, color);
}

void im2d_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec2 center = 0.5f * (2 * pos.xy + dim);

	Vec2 a = pos.xy;
	Vec2 b = vec2(pos.x, pos.y + dim.y);
	Vec2 c = pos.xy + dim;
	Vec2 d = vec2(pos.x + dim.x, pos.y);

	auto r0 = a - center;
	auto r1 = b - center;
	auto r2 = c - center;
	auto r3 = d - center;

	r32 cv = cosf(angle);
	r32 sv = sinf(angle);

	a.x = r0.x * cv - r0.y * sv;
	a.y = r0.x * sv + r0.y * cv;
	b.x = r1.x * cv - r1.y * sv;
	b.y = r1.x * sv + r1.y * cv;
	c.x = r2.x * cv - r2.y * sv;
	c.y = r2.x * sv + r2.y * cv;
	d.x = r3.x * cv - r3.y * sv;
	d.y = r3.x * sv + r3.y * cv;

	a += center;
	b += center;
	c += center;
	d += center;

	im2d_quad(vec3(a, pos.z), vec3(b, pos.z), vec3(c, pos.z), vec3(d, pos.z), uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im2d_rect_rotated(vec3(pos, 1), dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color) {
	im2d_rect_rotated(pos, dim, angle, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color) {
	im2d_rect_rotated(vec3(pos, 1), dim, angle, color);
}

void im2d_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im2d_rect_rotated(pos, dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	im2d_rect_rotated(vec3(pos, 1), dim, angle, rect, color);
}

void im2d_rect_centered(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec2 half_dim = 0.5f * dim;

	Vec3 a, b, c, d;
	a.xy = pos.xy - half_dim;
	b.xy = vec2(pos.x - half_dim.x, pos.y + half_dim.y);
	c.xy = pos.xy + half_dim;
	d.xy = vec2(pos.x + half_dim.x, pos.y - half_dim.y);

	a.z = pos.z;
	b.z = pos.z;
	c.z = pos.z;
	d.z = pos.z;

	im2d_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_centered(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im2d_rect_centered(vec3(pos, 1), dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_centered(Vec3 pos, Vec2 dim, Color4 color) {
	im2d_rect_centered(pos, dim, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_rect_centered(Vec2 pos, Vec2 dim, Color4 color) {
	im2d_rect_centered(vec3(pos, 1), dim, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_rect_centered(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im2d_rect_centered(pos, dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_centered(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	im2d_rect_centered(vec3(pos, 1), dim, rect, color);
}

void im2d_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec2 center = pos.xy;

	Vec2 half_dim = 0.5f * dim;
	Vec2 a, b, c, d;
	a = pos.xy - half_dim;
	b = vec2(pos.x - half_dim.x, pos.y + half_dim.y);
	c = pos.xy + half_dim;
	d = vec2(pos.x + half_dim.x, pos.y - half_dim.y);

	auto r0 = a - center;
	auto r1 = b - center;
	auto r2 = c - center;
	auto r3 = d - center;

	r32 cv = cosf(angle);
	r32 sv = sinf(angle);

	a.x = r0.x * cv - r0.y * sv;
	a.y = r0.x * sv + r0.y * cv;
	b.x = r1.x * cv - r1.y * sv;
	b.y = r1.x * sv + r1.y * cv;
	c.x = r2.x * cv - r2.y * sv;
	c.y = r2.x * sv + r2.y * cv;
	d.x = r3.x * cv - r3.y * sv;
	d.y = r3.x * sv + r3.y * cv;

	a += center;
	b += center;
	c += center;
	d += center;

	im2d_quad(vec3(a, pos.z), vec3(b, pos.z), vec3(c, pos.z), vec3(d, pos.z), uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im2d_rect_centered_rotated(vec3(pos, 1), dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color) {
	im2d_rect_centered_rotated(pos, dim, angle, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color) {
	im2d_rect_centered_rotated(vec3(pos, 1), dim, angle, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im2d_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im2d_rect_centered_rotated(pos, dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im2d_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	im2d_rect_centered_rotated(vec3(pos, 1), dim, angle, rect, color);
}

void im2d_ellipse(Vec3 pos, r32 radius_a, r32 radius_b, Color4 color, int segments) {
	segments = mmclamp(IM_MIN_CIRCLE_SEGMENTS, IM_MAX_CIRCLE_SEGMENTS - 1, segments);

	r32 px = im_unit_circle_cos[0] * radius_a;
	r32 py = im_unit_circle_sin[0] * radius_b;

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		int lookup = (int)(((r32)index / (r32)segments) * (IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im2d_triangle(pos, pos + vec3(npx, npy, 0), pos + vec3(px, py, 0), color);

		px = npx;
		py = npy;
	}
}

void im2d_ellipse(Vec2 pos, r32 radius_a, r32 radius_b, Color4 color, int segments) {
	im2d_ellipse(vec3(pos, 1), radius_a, radius_b, color, segments);
}

void im2d_circle(Vec3 pos, r32 radius, Color4 color, int segments) {
	im2d_ellipse(pos, radius, radius, color, segments);
}

void im2d_circle(Vec2 pos, r32 radius, Color4 color, int segments) {
	im2d_ellipse(vec3(pos, 1), radius, radius, color, segments);
}

void im2d_pie(Vec3 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	assert(theta_a >= 0 && theta_a <= MATH_PI * 2 && theta_b >= 0 && theta_b <= MATH_PI * 2);

	int first_index = (int)((0.5f * theta_a * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS) + 0.5f);
	int last_index  = (int)((0.5f * theta_b * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS) + 0.5f);

	while (first_index >= last_index)
		last_index += IM_MAX_CIRCLE_SEGMENTS;

	auto value_count = last_index - first_index;
	segments         = minimum(segments, value_count);

	r32 px = im_unit_circle_cos[first_index] * radius_a;
	r32 py = im_unit_circle_sin[first_index] * radius_b;

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		auto lookup = first_index + (int)((r32)index / (r32)segments * (r32)value_count + 0.5f);
		lookup = lookup % IM_MAX_CIRCLE_SEGMENTS;

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im2d_triangle(pos, pos + vec3(npx, npy, 0), pos + vec3(px, py, 0), color);

		px = npx;
		py = npy;
	}
}

void im2d_pie(Vec2 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im2d_pie(vec3(pos, 1), radius_a, radius_b, theta_a, theta_b, color, segments);
}

void im2d_pie(Vec3 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im2d_pie(pos, radius, radius, theta_a, theta_b, color, segments);
}

void im2d_pie(Vec2 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im2d_pie(vec3(pos, 1), radius, radius, theta_a, theta_b, color, segments);
}

void im2d_pie_part(Vec3 pos, r32 radius_a_min, r32 radius_b_min, r32 radius_a_max, r32 radius_b_max, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	assert(theta_a >= 0 && theta_a <= MATH_PI * 2 && theta_b >= 0 && theta_b <= MATH_PI * 2);

	int first_index = (int)((0.5f * theta_a * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS)+0.5f);
	int last_index = (int)((0.5f * theta_b * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS)+0.5f);

	while (first_index >= last_index)
		last_index += IM_MAX_CIRCLE_SEGMENTS;

	auto value_count = last_index - first_index;
	segments = minimum(segments, value_count);

	r32 min_px = im_unit_circle_cos[first_index] * radius_a_min;
	r32 min_py = im_unit_circle_sin[first_index] * radius_b_min;
	r32 max_px = im_unit_circle_cos[first_index] * radius_a_max;
	r32 max_py = im_unit_circle_sin[first_index] * radius_b_max;

	r32 min_npx, min_npy;
	r32 max_npx, max_npy;
	for (int index = 1; index <= segments; ++index) {
		auto lookup = first_index + (int)((r32)index / (r32)segments * (r32)value_count + 0.5f);
		lookup = lookup % IM_MAX_CIRCLE_SEGMENTS;

		min_npx = im_unit_circle_cos[lookup] * radius_a_min;
		min_npy = im_unit_circle_sin[lookup] * radius_b_min;
		max_npx = im_unit_circle_cos[lookup] * radius_a_max;
		max_npy = im_unit_circle_sin[lookup] * radius_b_max;

		im2d_quad(pos + vec3(min_npx, min_npy, 0),
			pos + vec3(max_npx, max_npy, 0),
			pos + vec3(max_px, max_py, 0),
			pos + vec3(min_px, min_py, 0),
			color);

		min_px = min_npx;
		min_py = min_npy;
		max_px = max_npx;
		max_py = max_npy;
	}
}

void im2d_pie_part(Vec2 pos, r32 radius_a_min, r32 radius_b_min, r32 radius_a_max, r32 radius_b_max, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im2d_pie_part(vec3(pos, 1), radius_a_min, radius_b_min, radius_a_max, radius_b_max, theta_a, theta_b, color, segments);
}

void im2d_pie_part(Vec3 pos, r32 radius_min, r32 radius_max, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im2d_pie_part(pos, radius_min, radius_min, radius_max, radius_max, theta_a, theta_b, color, segments);
}

void im2d_pie_part(Vec2 pos, r32 radius_min, r32 radius_max, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im2d_pie_part(vec3(pos, 1), radius_min, radius_min, radius_max, radius_max, theta_a, theta_b, color, segments);
}

void im2d_cube(Vec3 position, Quat rotation, Vec3 scale,
			 Mm_Rect rect0, Mm_Rect rect1, Mm_Rect rect2,
			 Mm_Rect rect3, Mm_Rect rect4, Mm_Rect rect5, Color4 color) {
	Vec3 vertices[] = {
		vec3(-0.5f, -0.5f, -0.5f),
		vec3(-0.5f, +0.5f, -0.5f),
		vec3(+0.5f, +0.5f, -0.5f),
		vec3(+0.5f, -0.5f, -0.5f),

		vec3(+0.5f, -0.5f, +0.5f),
		vec3(+0.5f, +0.5f, +0.5f),
		vec3(-0.5f, +0.5f, +0.5f),
		vec3(-0.5f, -0.5f, +0.5f),
	};

	for (int i = 0; i < static_count(vertices); ++i) {
		vertices[i] = vec3_hadamard(vertices[i], scale);
		vertices[i] = quat_rotate(rotation, vertices[i]);
		vertices[i] += position;
	}

	im2d_quad(vertices[0], vertices[1], vertices[2], vertices[3], rect0, color);
	im2d_quad(vertices[3], vertices[2], vertices[5], vertices[4], rect1, color);
	im2d_quad(vertices[4], vertices[5], vertices[6], vertices[7], rect2, color);
	im2d_quad(vertices[7], vertices[6], vertices[1], vertices[0], rect3, color);
	im2d_quad(vertices[1], vertices[6], vertices[5], vertices[2], rect4, color);
	im2d_quad(vertices[7], vertices[0], vertices[3], vertices[4], rect5, color);
}

void im2d_cube(Vec3 position, Quat rotation, Vec3 scale, Mm_Rect rect, Color4 color) {
	im2d_cube(position, rotation, scale, rect, rect, rect, rect, rect, rect, color);
}

void im2d_cube(Vec3 position, Quat rotation, Vec3 scale, Color4 color) {
	im2d_cube(position, rotation, scale, mm_rect(0, 0, 1, 1), color);
}

void im2d_set_stroke_weight(r32 weight) {
	im2d_stroke_weight = weight;
}

void im2d_line(Vec3 a, Vec3 b, Color4 color, r32 thickness) {
	if (vec3_equals(a, b, 0)) return;

	im2d_ensure_size(4, 6);

	if (im_context2d.transformation > 1) {
		auto &trans = im_context2d.transformations[im_context2d.transformation - 1];
		a = (trans * vec4(a, 1)).xyz;
		b = (trans * vec4(b, 1)).xyz;
	}

	r32 dx = b.x - a.x;
	r32 dy = b.y - a.y;
	r32 ilen = 1.0f / sqrtf(dx * dx + dy * dy);
	dx *= (thickness * ilen);
	dy *= (thickness * ilen);

	Im_Index  indices[] = { 0, 1, 2, 0, 2, 3 };
	Im_Vertex2d vertices[] = {
		{ vec3(a.x - dy, a.y + dx, a.z), vec2(0, 0), color },
		{ vec3(b.x - dy, b.y + dx, b.z), vec2(0, 1), color },
		{ vec3(b.x + dy, b.y - dx, b.z), vec2(1, 1), color },
		{ vec3(a.x + dy, a.y - dx, a.z), vec2(1, 0), color },
	};

	im2d_push_indices(indices, static_count(indices));
	auto dst = iim2d_push_vertex_count(static_count(vertices));
	memcpy(dst, vertices, static_count(vertices) * sizeof(Im_Vertex2d));
}

void im2d_line(Vec2 a, Vec2 b, Color4 color, r32 thickness) {
	im2d_line(vec3(a, 1), vec3(b, 1), color, thickness);
}

void im2d_bezier_quadratic(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness, int segments) {
	Vec3 p = a, np;
	for (int seg_index = 0; seg_index <= segments; ++seg_index) {
		r32 t = (r32)seg_index / (r32)segments;
		np    = bezier_quadratic(a, b, c, t);
		im2d_line(p, np, color, thickness);
		p = np;
	}
}

void im2d_bezier_quadratic(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness, int segments) {
	im2d_bezier_quadratic(vec3(a, 1), vec3(b, 1), vec3(c, 1), color, thickness, segments);
}

void im2d_bezier_cubic(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness, int segments) {
	Vec3 p = a, np;
	for (int seg_index = 0; seg_index <= segments; ++seg_index) {
		r32 t = (r32)seg_index / (r32)segments;
		np    = bezeir_cubic(a, b, c, d, t);
		im2d_line(p, np, color, thickness);
		p = np;
	}
}

void im2d_bezier_cubic(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness, int segments) {
	im2d_bezier_cubic(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), color, thickness, segments);
}

void im2d_polygon(const Polygon &polygon, r32 z, Color4 color) {
	u32 triangle_count = polygon.vertex_count - 2;
	for (u32 triangle_index = 0; triangle_index < triangle_count; ++triangle_index) {
		im2d_triangle(vec3(polygon.vertices[0], z), vec3(polygon.vertices[triangle_index + 1], z), vec3(polygon.vertices[triangle_index + 2], z), color);
	}
}

void im2d_polygon(const Polygon &polygon, Color4 color) {
	im2d_polygon(polygon, 1, color);
}

void im2d_polygon(const Polygon_Pt &polygon, r32 z, Color4 color) {
	u32 triangle_count = polygon.vertex_count - 2;
	for (u32 triangle_index = 0; triangle_index < triangle_count; ++triangle_index) {
		im2d_triangle(vec3(polygon.vertices[0], z), vec3(polygon.vertices[triangle_index + 1], z), vec3(polygon.vertices[triangle_index + 2], z), color);
	}
}

void im2d_polygon(const Polygon_Pt &polygon, Color4 color) {
	im2d_polygon(polygon, 1, color);
}

void im2d_triangle_outline(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness) {
	im2d_line(a, b, color, thickness);
	im2d_line(b, c, color, thickness);
	im2d_line(c, a, color, thickness);
}

void im2d_triangle_outline(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness) {
	im2d_line(a, b, color, thickness);
	im2d_line(b, c, color, thickness);
	im2d_line(c, a, color, thickness);
}

void im2d_quad_outline(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness) {
	im2d_line(a, b, color, thickness);
	im2d_line(b, c, color, thickness);
	im2d_line(c, d, color, thickness);
	im2d_line(a, d, color, thickness);
}

void im2d_quad_outline(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness) {
	im2d_line(a, b, color, thickness);
	im2d_line(b, c, color, thickness);
	im2d_line(c, d, color, thickness);
	im2d_line(a, d, color, thickness);
}

void im2d_rect_outline(Vec3 pos, Vec2 dim, Color4 color, r32 thickness) {
	Vec3 a = pos;
	Vec3 b = pos + vec3(0, dim.y, 0);
	Vec3 c = pos + vec3(dim, 0);
	Vec3 d = pos + vec3(dim.x, 0, 0);
	im2d_quad_outline(a, b, c, d, color, thickness);
}

void im2d_rect_outline(Vec2 pos, Vec2 dim, Color4 color, r32 thickness) {
	im2d_rect_outline(vec3(pos, 1), dim, color, thickness);
}

void im2d_rect_centered_outline(Vec3 pos, Vec2 dim, Color4 color, r32 thickness) {
	Vec2 half_dim  = 0.5f * dim;

	Vec3 a, b, c, d;
	a.xy = pos.xy - half_dim;
	b.xy = vec2(pos.x - half_dim.x, pos.y + half_dim.y);
	c.xy = pos.xy + half_dim;
	d.xy = vec2(pos.x + half_dim.x, pos.y - half_dim.y);

	a.z = pos.z;
	b.z = pos.z;
	c.z = pos.z;
	d.z = pos.z;
	im2d_quad_outline(a, b, c, d, color, thickness);
}

void im2d_rect_centered_outline(Vec2 pos, Vec2 dim, Color4 color, r32 thickness) {
	im2d_rect_centered_outline(vec3(pos, 1), dim, color, thickness);
}

void im2d_ellipse_outline(Vec3 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness, int segments) {
	segments = mmclamp(IM_MIN_CIRCLE_SEGMENTS, IM_MAX_CIRCLE_SEGMENTS - 1, segments);

	r32 px = im_unit_circle_cos[0] * radius_a;
	r32 py = im_unit_circle_sin[0] * radius_b;

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		int lookup = (int)(((r32)index / (r32)segments) * (IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im2d_line(position + vec3(px, py, 0), position + vec3(npx, npy, 0), color, thickness);

		px = npx;
		py = npy;
	}
}

void im2d_ellipse_outline(Vec2 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness, int segments) {
	im2d_ellipse_outline(vec3(position, 1), radius_a, radius_b, color, thickness, segments);
}

void im2d_circle_outline(Vec3 position, r32 radius, Color4 color, r32 thickness, int segments) {
	im2d_ellipse_outline(position, radius, radius, color, thickness, segments);
}

void im2d_circle_outline(Vec2 position, r32 radius, Color4 color, r32 thickness, int segments) {
	im2d_ellipse_outline(vec3(position, 1), radius, radius, color, thickness, segments);
}

void im2d_arc_outline(Vec3 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	assert(theta_a >= 0 && theta_a <= MATH_PI * 2 && theta_b >= 0 && theta_b <= MATH_PI * 2);

	int first_index = (int)((0.5f * theta_a * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS) + 0.5f);
	int last_index  = (int)((0.5f * theta_b * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS) + 0.5f);

	while (first_index >= last_index)
		last_index += IM_MAX_CIRCLE_SEGMENTS;

	auto value_count = last_index - first_index;
	segments         = minimum(segments, value_count);

	r32 px = im_unit_circle_cos[first_index] * radius_a;
	r32 py = im_unit_circle_sin[first_index] * radius_b;

	if (closed) {
		im2d_line(position, position + vec3(px, py, 0), color, thickness);
	}

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		auto lookup = first_index + (int)((r32)index / (r32)segments * (r32)value_count + 0.5f);
		lookup = lookup % IM_MAX_CIRCLE_SEGMENTS;

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im2d_line(position + vec3(px, py, 0), position + vec3(npx, npy, 0), color, thickness);

		px = npx;
		py = npy;
	}

	if (closed) {
		im2d_line(position, position + vec3(px, py, 0), color, thickness);
	}
}

void im2d_arc_outline(Vec2 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im2d_arc_outline(vec3(position, 1), radius_a, radius_b, theta_a, theta_b, color, closed, thickness, segments);
}

void im2d_arc_outline(Vec3 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im2d_arc_outline(position, radius, radius, theta_a, theta_b, color, closed, thickness, segments);
}

void im2d_arc_outline(Vec2 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im2d_arc_outline(vec3(position, 1), radius, radius, theta_a, theta_b, color, closed, thickness, segments);
}

void im2d_polygon_outline(const Polygon &polygon, r32 z, Color4 color, r32 thickness) {
	const Vec2 *p, *q;
	p = polygon.vertices;
	for (u32 index = 0; index < polygon.vertex_count - 1; ++index) {
		q = p + 1;
		im2d_line(vec3(*p, z), vec3(*q, z), color, thickness);
		p = q;
	}
	im2d_line(vec3(*polygon.vertices, z), vec3(*p, z), color, thickness);
}

void im2d_polygon_outline(const Polygon &polygon, Color4 color, r32 thickness) {
	im2d_polygon_outline(polygon, 1, color, thickness);
}

void im2d_polygon_outline(const Polygon_Pt &polygon, r32 z, Color4 color, r32 thickness) {
	const Vec2 *p, *q;
	p = polygon.vertices;
	for (u32 index = 0; index < polygon.vertex_count - 1; ++index) {
		q = p + 1;
		im2d_line(vec3(*p, z), vec3(*q, z), color, thickness);
		p = q;
	}
	im2d_line(vec3(*polygon.vertices, z), vec3(*p, z), color, thickness);
}

void im2d_polygon_outline(const Polygon_Pt &polygon, Color4 color, r32 thickness) {
	im2d_polygon_outline(polygon, 1, color, thickness);
}

void im2d_text(Vec3 position, r32 scale, Monospaced_Font_Info &font, const String string, Color4 color) {
	Vec2 dimension;
	Vec3 render_pos;
	render_pos.z = position.z;
	r32 advance  = font.advance * scale;
	for (s64 c_index = 0; c_index < string.count; ++c_index) {
		s32 index = (s32)(string[c_index] - font.first) + 1;

		if (index >= font.count || index < 0) index = 0;

		auto &info    = font.range[index];
		render_pos.xy = position.xy + info.offset * scale;

		dimension.x = info.width * scale;
		dimension.y = info.height * scale;
		im2d_rect(render_pos, dimension, info.rect, color);

		position.x += advance;
	}
}

void im2d_text(Vec2 position, r32 scale, Monospaced_Font_Info &font, const String string, Color4 color) {
	im2d_text(vec3(position, 1), scale, font, string, color);
}

void im2d_text_region(Vec3 position, Vec2 region, Monospaced_Font_Info &font, const String string, Color4 color) {
	r32 scale   = region.y;
	r32 advance = font.advance * scale;
	r32 max_x   = position.x + region.x - advance;

	Vec2 dimension;
	Vec3 render_pos;
	render_pos.z = position.z;
	for (s64 c_index = 0; c_index < string.count && position.x <= max_x; ++c_index) {
		s32 index = (s32)(string[c_index] - font.first) + 1;

		if (index >= font.count || index < 0) index = 0;

		auto &info    = font.range[index];
		render_pos.xy = position.xy + info.offset * scale;

		dimension.x = info.width * scale;
		dimension.y = info.height * scale;
		im2d_rect(render_pos, dimension, info.rect, color);

		position.x += advance;
	}
}

void im2d_text_region(Vec2 position, Vec2 region, Monospaced_Font_Info &font, const String string, Color4 color) {
	im2d_text_region(vec3(position, 1), region, font, string, color);
}

Vec2 im2d_calculate_text_region(r32 scale, Monospaced_Font_Info &font, const String string) {
	r32 y = scale;
	r32 x = font.advance * string.count * scale;
	return vec2(x, y);
}

//
//
//

inline void im3d_start_cmd_record(Texture_View texture) {
	auto draw_cmd     = im_context3d.draw_cmds + im_context3d.draw_cmd;
	draw_cmd->index   = im_context3d.index;
	draw_cmd->vertex  = im_context3d.vertex;
	draw_cmd->texture = texture;
	draw_cmd->count   = 0;
}

inline Im_Vertex3d *iim3d_push_vertex_count(int count) {
	assert(im_context3d.vertex + count <= IM_CONTEXT_MAX_VERTICES);

	auto vertex = im_context3d.vertex_ptr;
	im_context3d.vertex_ptr += count;
	im_context3d.vertex += count;
	return vertex;
}

inline void im3d_push_vertices(Im_Vertex3d *vertices, int count) {
	auto dst = iim3d_push_vertex_count(count);

	if (im_context3d.transformation > 1) {
		auto &trans = im_context3d.transformations[im_context3d.transformation - 1];
		auto nmt = mat4_transpose(mat4_inverse(trans));
		nmt.rows[0].w = 0;
		nmt.rows[1].w = 0;
		nmt.rows[2].w = 0;
		nmt.rows[3] = vec4(0, 0, 0, 1);

		for (int i = 0; i < count; ++i) {
			vertices[i].position = (trans * vec4(vertices[i].position, 1)).xyz;
			vertices[i].normal = (nmt * vec4(vertices[i].normal, 0)).xyz;
		}
	}

	memcpy(dst, vertices, sizeof(Im_Vertex3d) * count);
}

inline void im3d_push_vertex(Vec3 position, Vec2 texture_coord, Vec3 normal, Color4 color) {
	Im_Vertex3d src = { position, texture_coord, normal, color };
	im3d_push_vertices(&src, 1);
}

inline Im_Index *iim3d_push_index_count(int count) {
	assert(im_context3d.index + count <= IM_CONTEXT_MAX_INDICES);
	auto index = im_context3d.index_ptr;
	im_context3d.index_ptr += count;
	im_context3d.index += count;
	im_context3d.counter += count;
	return index;
}

#define im3d_get_last_index() (im_context3d.counter ? im_context3d.last_index + 1 : im_context3d.last_index)
#define im3d_get_draw_cmd()   (im_context3d.draw_cmds + im_context3d.draw_cmd)

inline void im3d_push_indices(Im_Index *indices, int count) {
	assert(im_context3d.index + count <= IM_CONTEXT_MAX_INDICES);
	auto prev_counter = im_context3d.counter;

	auto dst = iim3d_push_index_count(count);

	if (prev_counter) {
		auto next_index = im_context3d.last_index + 1;
		for (int i = 0; i < count; ++i) {
			indices[i] += next_index;
		}
	}

	im_context3d.last_index = indices[count - 1];
	memcpy(dst, indices, count * sizeof(Im_Vertex3d));
}

inline void im3d_push_draw_cmd() {
	auto last_cmd   = im_context3d.draw_cmds + im_context3d.draw_cmd;
	last_cmd->count = im_context3d.counter;
	im_context3d.draw_cmd += 1;
	im_context3d.counter = 0;
}

void iim3d_flush(bool restart = true);

inline void im3d_ensure_size(int vertex_count, int index_count) {
	if (im_context3d.index + index_count - 1 >= IM_CONTEXT_MAX_INDICES ||
		im_context3d.vertex + vertex_count - 1 >= IM_CONTEXT_MAX_VERTICES) {
		im3d_push_draw_cmd();
		iim3d_flush();
	}
}

void im3d_begin(const Mat4 &transform) {
	im_context3d.draw_cmd       = 0;
	im_context3d.vertex         = 0;
	im_context3d.index          = 0;
	im_context3d.counter        = 0;
	im_context3d.transformation = 1;

	im_context3d.vertex_ptr = (Im_Vertex3d *)gfx->map(im_context3d.vertex_buffer, Map_Type_WRITE_DISCARD);
	im_context3d.index_ptr  = (Im_Index *)gfx->map(im_context3d.index_buffer, Map_Type_WRITE_DISCARD);

	im3d_start_cmd_record(white_texture.view);

	Im_Uniform uniform;
	uniform.transform = transform;

	void *ptr = gfx->map(im_context3d.uniform_buffer, Map_Type_WRITE_DISCARD);
	memcpy(ptr, &uniform, sizeof(uniform));
	gfx->unmap(im_context3d.uniform_buffer);

	gfx->cmd_bind_pipeline(im_context3d.pipeline);
	gfx->cmd_bind_samplers(0, 1, &im_context3d.sampler);
	gfx->cmd_bind_vs_uniform_buffers(0, 1, &im_context3d.uniform_buffer);
}

void im3d_begin(Camera_View &view, const Mat4 &transform) {
	Mat4 projection = gfx_view_transform(view);
	im3d_begin(projection * transform);
}

void iim3d_flush(bool restart) {
	gfx->unmap(im_context3d.vertex_buffer);
	gfx->unmap(im_context3d.index_buffer);

	if (im_context3d.draw_cmd) {
		gfx->cmd_bind_vertex_buffer(im_context3d.vertex_buffer, sizeof(Im_Vertex3d));
		gfx->cmd_bind_index_buffer(im_context3d.index_buffer, Index_Type_U16);

		for (u32 draw_cmd_index = 0; draw_cmd_index < im_context3d.draw_cmd; ++draw_cmd_index) {
			Im_Draw_Cmd *cmd = im_context3d.draw_cmds + draw_cmd_index;

			gfx->cmd_bind_textures(0, 1, &cmd->texture);
			gfx->cmd_draw_indexed(cmd->count, cmd->index, cmd->vertex);
		}

		auto bound_texture = im_context3d.draw_cmds[im_context3d.draw_cmd - 1].texture;

		im_context3d.draw_cmd = 0;
		im_context3d.vertex   = 0;
		im_context3d.index    = 0;
		im_context3d.counter  = 0;

		im3d_start_cmd_record(bound_texture);
	}

	if (restart) {
		im_context3d.vertex_ptr = (Im_Vertex3d *)gfx->map(im_context3d.vertex_buffer, Map_Type_WRITE_DISCARD);
		im_context3d.index_ptr  = (Im_Index *)gfx->map(im_context3d.index_buffer, Map_Type_WRITE_DISCARD);
	}
}

void im3d_end() {
	if (im_context3d.counter) {
		im3d_push_draw_cmd();
	}
	iim3d_flush(false);
}

void im3d_bind_texture(Texture2d_Handle handle) {
	if (im_context3d.draw_cmds[im_context3d.draw_cmd].texture.id == handle.view.id)
		return;

	if (im_context3d.counter) {
		im3d_push_draw_cmd();
		im3d_start_cmd_record(handle.view);
	} else {
		auto draw_cmd     = im3d_get_draw_cmd();
		draw_cmd->texture = handle.view;
	}
}

void im3d_unbind_texture() {
	im3d_bind_texture(white_texture);
}

void im3d_push_matrix(const Mat4 &matrix) {
	assert(im_context3d.transformation < IM_CONTEXT_MAX_TRANSFORMATIONS);
	im_context3d.transformations[im_context3d.transformation] = im_context3d.transformations[im_context3d.transformation - 1] * matrix;
	im_context3d.transformation += 1;
}

void im3d_pop_matrix() {
	assert(im_context3d.transformation > 1);
	im_context3d.transformation -= 1;
}

void im3d_flush_transformations() {
	im_context3d.transformation = 1;
}

void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec3 na, Vec3 nb, Vec3 nc, Color4 color) {
	im3d_ensure_size(3, 3);

	Im_Index  indices[]  = { 0, 1, 2 };
	Im_Vertex3d vertices[] = {
		{ a, uv_a, na, color },
		{ b, uv_b, nb, color },
		{ c, uv_c, nc, color },
	};

	im3d_push_indices(indices, static_count(indices));
	im3d_push_vertices(vertices, static_count(vertices));
}

void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color) {
	Vec3 dir_a = a - b;
	Vec3 dir_b = c - b;
	Vec3 n = vec3_cross(dir_b, dir_a);
	im3d_triangle(a, b, c, uv_a, uv_b, uv_c, n, n, n, color);
}

void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec3 na, Vec3 nb, Vec3 nc, Color4 color) {
	im3d_triangle(a, b, c, vec2(0), vec2(0), vec2(0), na, nb, nc, color);
}

void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color) {
	im3d_triangle(a, b, c, vec2(0), vec2(0), vec2(0), color);
}

void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, Color4 color) {
	im3d_triangle(a, b, c, uv_a, uv_b, uv_c, na, nb, nc, color);
	im3d_triangle(a, c, d, uv_a, uv_c, uv_d, na, nc, nd, color);
}

void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im3d_triangle(a, b, c, uv_a, uv_b, uv_c, color);
	im3d_triangle(a, c, d, uv_a, uv_c, uv_d, color);
}

void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, Color4 color) {
	im3d_quad(a, b, c, d, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), na, nb, nc, nd, color);
}

void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color) {
	im3d_quad(a, b, c, d, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im3d_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, na, nb, nc, nd, color);
}

void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im3d_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im3d_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec2 half_dim = 0.5f * dim;

	Vec3 a, b, c, d;
	a.xy = pos.xy - half_dim;
	b.xy = vec2(pos.x - half_dim.x, pos.y + half_dim.y);
	c.xy = pos.xy + half_dim;
	d.xy = vec2(pos.x + half_dim.x, pos.y - half_dim.y);

	a.z = pos.z;
	b.z = pos.z;
	c.z = pos.z;
	d.z = pos.z;

	Vec3 n = vec3(0, 0, -1);

	im3d_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, n, n, n, n, color);
}

void im3d_rect(Vec3 pos, Vec2 dim, Color4 color) {
	im3d_rect(pos, dim, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im3d_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im3d_rect(pos, dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im3d_cube(Vec3 position, Quat rotation, Vec3 scale,
	Mm_Rect rect0, Mm_Rect rect1, Mm_Rect rect2,
	Mm_Rect rect3, Mm_Rect rect4, Mm_Rect rect5, Color4 color) {
	Vec3 vertices[] = {
		vec3(-0.5f, -0.5f, -0.5f),
		vec3(-0.5f, +0.5f, -0.5f),
		vec3(+0.5f, +0.5f, -0.5f),
		vec3(+0.5f, -0.5f, -0.5f),

		vec3(+0.5f, -0.5f, +0.5f),
		vec3(+0.5f, +0.5f, +0.5f),
		vec3(-0.5f, +0.5f, +0.5f),
		vec3(-0.5f, -0.5f, +0.5f),
	};

	Vec3 normals[] = {
		vec3( 0,  0, -1),
		vec3( 1,  0,  0),
		vec3( 0,  0,  1),
		vec3(-1,  0,  0),
		vec3( 0,  1,  0),
		vec3( 0, -1,  0),
	};

	for (int i = 0; i < static_count(vertices); ++i) {
		vertices[i] = vec3_hadamard(vertices[i], scale);
		vertices[i] = quat_rotate(rotation, vertices[i]);
		vertices[i] += position;
	}

	im3d_quad(vertices[0], vertices[1], vertices[2], vertices[3], rect0, normals[0], normals[0], normals[0], normals[0], color);
	im3d_quad(vertices[3], vertices[2], vertices[5], vertices[4], rect1, normals[1], normals[1], normals[1], normals[1], color);
	im3d_quad(vertices[4], vertices[5], vertices[6], vertices[7], rect2, normals[2], normals[2], normals[2], normals[2], color);
	im3d_quad(vertices[7], vertices[6], vertices[1], vertices[0], rect3, normals[3], normals[3], normals[3], normals[3], color);
	im3d_quad(vertices[1], vertices[6], vertices[5], vertices[2], rect4, normals[4], normals[4], normals[4], normals[4], color);
	im3d_quad(vertices[7], vertices[0], vertices[3], vertices[4], rect5, normals[5], normals[5], normals[5], normals[5], color);
}

void im3d_cube(Vec3 position, Quat rotation, Vec3 scale, Mm_Rect rect, Color4 color) {
	im3d_cube(position, rotation, scale, rect, rect, rect, rect, rect, rect, color);
}

void im3d_cube(Vec3 position, Quat rotation, Vec3 scale, Color4 color) {
	im3d_cube(position, rotation, scale, mm_rect(0, 0, 1, 1), color);
}

void im3d_mesh(Im_Mesh &mesh, Vec3 position, Quat rotation, Vec3 scale, Color4 color) {
	Vec3 *v = mesh.vertices;
	Vec2 *t = mesh.texture_coords;
	Vec3 *n = mesh.normals;

	for (u32 face_index = 0; face_index < mesh.index_count; face_index += 3) {
		auto &i0 = mesh.indices[face_index + 0];
		auto &i1 = mesh.indices[face_index + 1];
		auto &i2 = mesh.indices[face_index + 2];

		Vec3 va = v[i0.v];
		va = vec3_hadamard(va, scale);
		va = quat_rotate(rotation, va);
		va += position;

		Vec3 vb = v[i1.v];
		vb = vec3_hadamard(vb, scale);
		vb = quat_rotate(rotation, vb);
		vb += position;		
		
		Vec3 vc = v[i2.v];
		vc = vec3_hadamard(vc, scale);
		vc = quat_rotate(rotation, vc);
		vc += position;

		im3d_triangle(va, vb, vc, t[i0.t], t[i1.t], t[i2.t], n[i0.n], n[i1.n], n[i2.n], color);
	}
}
