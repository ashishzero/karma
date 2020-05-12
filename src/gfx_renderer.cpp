#include "gfx_renderer.h"
#include "gfx_platform.h"
#include "string.h"
#include "systems.h"
#include "gfx_shaders.h"

#pragma pack(push, 4)

struct alignas(16) Im_Uniform {
	Mat4 transform;
};

#pragma pack(pop)

struct Im_Vertex {
	Vec3   position;
	Vec2   tex_coord;
	Color4 color;
};
typedef u16 Im_Index;

struct Im_Draw_Cmd {
	Texture_View texture;
	u32          vertex;
	u32          index;
	u32          count;
};

static constexpr int IM_CONTEXT_MAX_VERTICES        = 2048 * 4;
static constexpr int IM_CONTEXT_MAX_INDICES         = IM_CONTEXT_MAX_VERTICES * 6;
static constexpr int IM_CONTEXT_MAX_TRANSFORMATIONS = 100;
static r32           im_unit_circle_cos[IM_MAX_CIRCLE_SEGMENTS];
static r32           im_unit_circle_sin[IM_MAX_CIRCLE_SEGMENTS];

struct Im_Context {
	Render_Pipeline pipeline;
	Uniform_Buffer  uniform_buffer;
	Vertex_Buffer   vertex_buffer;
	Index_Buffer    index_buffer;
	Sampler         sampler;

	Im_Draw_Cmd draw_cmds[IM_CONTEXT_MAX_VERTICES];
	int         draw_cmd;
	Im_Vertex * vertex_ptr;
	int         vertex;
	Im_Index *  index_ptr;
	Im_Index    last_index;
	int         index;
	Im_Index    counter;

	Mat4 transformations[IM_CONTEXT_MAX_TRANSFORMATIONS];
	int  transformation;
};

struct Hdr_Data {
	Texture2d          hdr_color_buffer[2];
	Texture2d          hdr_depth_buffer;
	Texture_View       hdr_color_view[2];
	Depth_Stencil_View hdr_depth_view;
	Framebuffer        hdr_framebuffer;
	// TODO: Currently we are using full window width and window height for bloom effect
	// which is very expensive and not worth it!!
	Texture2d    bloom_color_buffer[2];
	Texture_View bloom_color_view[2];
	Framebuffer  bloom_framebuffer[2];
};

static Im_Context       im_context;
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

	if (!result) {
		system_log(LOG_ERROR, "Gfx", "Shader %s not found", name);
	}

	return result;
}

bool gfx_create_context(Handle platform, Render_Backend backend, s32 vsync, u32 multisamples, u32 framebuffer_w, u32 framebuffer_h) {
	if (backend == Render_Backend_OPENGL) {
		system_log(LOG_INFO, "Gfx", "gfx.backend = Render_Backend_OPENGL");
		gfx = create_opengl_context(platform, vsync, multisamples);
		if (gfx->backend == Render_Backend_NONE) {
			system_log(LOG_ERROR, "Gfx", "create_opengl_context() failed.");
			return false;
		}
		shader_lang = SHADER_LANG_GLSL;
	} else if (backend == Render_Backend_DIRECTX11) {
		system_log(LOG_INFO, "Gfx", "gfx.backend = Render_Backend_DIRECTX11");
		gfx = create_directx11_context(platform, vsync, multisamples);
		if (gfx->backend == Render_Backend_NONE) {
			system_log(LOG_ERROR, "Gfx", "create_directx11_context() failed.");
			return false;
		}
		shader_lang = SHADER_LANG_HLSL;
	} else {
		return false;
	}

	{
		auto info = gfx->get_info();
		system_log(LOG_INFO, "Gfx", "Renderer: %s, Version %s", info->renderer, info->version);
		system_log(LOG_INFO, "Gfx", "Vendor: %s", info->vendor);
		system_log(LOG_INFO, "Gfx", "Shading Language: %s", info->shading_lang);
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
		Depth_Info      depth      = depth_info_create(false);

		hdr_pipeline = gfx->create_render_pipeline(shader, rasterizer, blend, depth, "postprocess");

		mfree(hdr_shader.data);
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

		mfree(h_blur_shader.data);
		mfree(v_blur_shader.data);
	}

	postprocess_sampler = gfx->create_sampler(filter_create(), texture_address_create(), level_of_detail_create());

	{
		Input_Element_Layout layouts[] = {
			{ "POSITION", Data_Format_RGB32_FLOAT, offsetof(Im_Vertex, position), Input_Type_PER_VERTEX_DATA, 0 },
			{ "TEX_COORD", Data_Format_RG32_FLOAT, offsetof(Im_Vertex, tex_coord), Input_Type_PER_VERTEX_DATA, 0 },
			{ "COLOR", Data_Format_RGBA32_FLOAT, offsetof(Im_Vertex, color), Input_Type_PER_VERTEX_DATA, 0 }
		};

		String quad_shader_content = system_read_entire_file("data/shaders/quad.kfx");

		Shader_Info shader;
		shader.input_layouts       = layouts;
		shader.input_layouts_count = static_count(layouts);
		shader.vertex              = igfx_find_shader(quad_shader_content, SHADER_TYPE_VERTEX, "quad.kfx.vertex");
		shader.pixel               = igfx_find_shader(quad_shader_content, SHADER_TYPE_PIXEL, "quad.kfx.pixel");
		shader.stride              = sizeof(Im_Vertex);

		Rasterizer_Info rasterizer = rasterizer_info_create();
		rasterizer.cull_mode       = Cull_Mode_NONE;

		Blend_Info blend = blend_info_create(Blend_SRC_ALPHA, Blend_INV_SRC_ALPHA, Blend_Operation_ADD, Blend_SRC_ALPHA, Blend_INV_SRC_ALPHA, Blend_Operation_ADD);
		Depth_Info depth = depth_info_create(true, Depth_Write_Mask_ALL, Comparison_Function_LESS);

		im_context.pipeline = gfx->create_render_pipeline(shader, rasterizer, blend, depth, "Im_Pipeline");

		mfree(quad_shader_content.data);
	}

	im_context.uniform_buffer = gfx->create_uniform_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Mat4), 0);
	im_context.vertex_buffer  = gfx->create_vertex_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Im_Vertex) * IM_CONTEXT_MAX_VERTICES, 0);
	im_context.index_buffer   = gfx->create_index_buffer(Buffer_Usage_DYNAMIC, Cpu_Access_WRITE, sizeof(Im_Index) * IM_CONTEXT_MAX_INDICES, 0);

	im_context.sampler = gfx->create_sampler(filter_create(Filter_Type_MIN_MAG_MIP_POINT), texture_address_create(), level_of_detail_create());

	im_context.transformations[0] = mat4_identity();
	im_context.transformation     = 1;

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
		if (hdr_data.bloom_color_view[index].id) gfx->destroy_texture_view(hdr_data.bloom_color_view[index]);
		if (hdr_data.bloom_color_buffer[index].id) gfx->destroy_texture2d(hdr_data.bloom_color_buffer[index]);
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
		hdr_data.hdr_color_view[index]   = gfx->create_texture_view(hdr_data.hdr_color_buffer[index]);

		hdr_data.bloom_color_buffer[index] = gfx->create_texture2d(bloom_w, bloom_h, 4, Data_Format_RGBA16_FLOAT, 0, Buffer_Usage_DEFAULT, Texture_Bind_SHADER_RESOURCE | Texture_Bind_RENDER_TARGET, 1);
		hdr_data.bloom_color_view[index]   = gfx->create_texture_view(hdr_data.bloom_color_buffer[index]);
		hdr_data.bloom_framebuffer[index]  = gfx->create_framebuffer(1, &hdr_data.bloom_color_buffer[index], &hdr_data.bloom_color_view[index], NULL);
	}

	hdr_data.hdr_depth_buffer = gfx->create_texture2d(width, height, 1, Data_Format_D32_FLOAT, 0, Buffer_Usage_DEFAULT, Texture_Bind_DEPTH_STENCIL, 1);
	hdr_data.hdr_depth_view   = gfx->create_depth_stencil_view(hdr_data.hdr_depth_buffer);

	hdr_data.hdr_framebuffer = gfx->create_framebuffer(2, hdr_data.hdr_color_buffer, hdr_data.hdr_color_view, &hdr_data.hdr_depth_view);
}

void gfx_destroy_context() {
	gfx->destroy_vertex_buffer(im_context.vertex_buffer);
	gfx->destroy_index_buffer(im_context.index_buffer);
	gfx->destroy_uniform_buffer(im_context.uniform_buffer);
	gfx->destory_render_pipeline(im_context.pipeline);
	gfx->destory_sampler(im_context.sampler);
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

void gfx_set_swap_interval(s32 interval) {
	gfx->set_swap_interval(interval);
}

s32 gfx_get_swap_interval() {
	return gfx->get_swap_interval();
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

inline void im_start_cmd_record(Texture_View texture) {
	auto draw_cmd     = im_context.draw_cmds + im_context.draw_cmd;
	draw_cmd->index   = im_context.index;
	draw_cmd->vertex  = im_context.vertex;
	draw_cmd->texture = texture;
	draw_cmd->count   = 0;
}

inline Im_Vertex *iim_push_vertex_count(int count) {
	assert(im_context.vertex + count <= IM_CONTEXT_MAX_VERTICES);

	auto vertex = im_context.vertex_ptr;
	im_context.vertex_ptr += count;
	im_context.vertex += count;
	return vertex;
}

inline void im_push_vertices(Im_Vertex *vertices, int count) {
	auto dst = iim_push_vertex_count(count);
	memcpy(dst, vertices, sizeof(Im_Vertex) * count);
}

inline void im_push_vertex(Vec3 position, Vec2 texture_coord, Color4 color) {
	Im_Vertex src = { position, texture_coord, color };
	im_push_vertices(&src, 1);
}

inline Im_Index *iim_push_index_count(int count) {
	assert(im_context.index + count <= IM_CONTEXT_MAX_INDICES);
	auto index = im_context.index_ptr;
	im_context.index_ptr += count;
	im_context.counter += count;
	return index;
}

#define im_get_last_index() (im_context.counter ? im_context.last_index + 1 : im_context.last_index)
#define im_get_draw_cmd()   (im_context.draw_cmds + im_context.draw_cmd)

inline void im_push_indices(Im_Index *indices, int count) {
	assert(im_context.index + count <= IM_CONTEXT_MAX_INDICES);
	auto prev_counter = im_context.counter;

	auto dst = iim_push_index_count(count);

	if (prev_counter) {
		auto next_index = im_context.last_index + 1;
		for (int i = 0; i < count; ++i) {
			indices[i] += next_index;
		}
	}

	im_context.last_index = indices[count - 1];
	memcpy(dst, indices, count * sizeof(Im_Vertex));
}

inline void im_push_draw_cmd() {
	auto last_cmd   = im_context.draw_cmds + im_context.draw_cmd;
	last_cmd->count = im_context.counter;
	im_context.draw_cmd += 1;
	im_context.counter = 0;
}

void iim_flush(bool restart = true);

inline void im_ensure_size(int vertex_count, int index_count) {
	if (im_context.index + index_count - 1 >= IM_CONTEXT_MAX_INDICES ||
		im_context.vertex + vertex_count - 1 >= IM_CONTEXT_MAX_VERTICES) {
		im_push_draw_cmd();
		iim_flush();
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
	auto viewport = rect(0, 0, (r32)bloom_texture_w, (r32)bloom_texture_h);
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

void im_begin(Mat4 &transform) {
	im_context.draw_cmd       = 0;
	im_context.vertex         = 0;
	im_context.index          = 0;
	im_context.counter        = 0;
	im_context.transformation = 1;

	im_context.vertex_ptr = (Im_Vertex *)gfx->map(im_context.vertex_buffer, Map_Type_WRITE_DISCARD);
	im_context.index_ptr  = (Im_Index *)gfx->map(im_context.index_buffer, Map_Type_WRITE_DISCARD);

	im_start_cmd_record(white_texture.view);

	Im_Uniform uniform;
	uniform.transform = transform;

	void *ptr = gfx->map(im_context.uniform_buffer, Map_Type_WRITE_DISCARD);
	memcpy(ptr, &uniform, sizeof(uniform));
	gfx->unmap(im_context.uniform_buffer);

	gfx->cmd_bind_pipeline(im_context.pipeline);
	gfx->cmd_bind_samplers(0, 1, &im_context.sampler);
	gfx->cmd_bind_vs_uniform_buffers(0, 1, &im_context.uniform_buffer);
}

void im_begin(Camera_View &view, Mat4 &transform) {
	Mat4 projection;
	switch (gfx->backend) {
		case Render_Backend_OPENGL: {
			if (view.kind == ORTHOGRAPHIC)
				projection = mat4_ortho_gl(view.orthographic.left,
										   view.orthographic.right,
										   view.orthographic.top,
										   view.orthographic.bottom,
										   view.orthographic.near,
										   view.orthographic.far);
			else
				projection = mat4_perspective_gl(view.perspective.field_of_view,
												 view.perspective.aspect_ratio,
												 view.perspective.near_plane,
												 view.perspective.far_plane);
		} break;

		case Render_Backend_DIRECTX11: {
			if (view.kind == ORTHOGRAPHIC)
				projection = mat4_ortho_dx(view.orthographic.left,
										   view.orthographic.right,
										   view.orthographic.top,
										   view.orthographic.bottom,
										   view.orthographic.near,
										   view.orthographic.far);
			else
				projection = mat4_perspective_dx(view.perspective.field_of_view,
												 view.perspective.aspect_ratio,
												 view.perspective.near_plane,
												 view.perspective.far_plane);
		} break;

			invalid_default_case();
	}

	im_begin(projection * transform);
}

void iim_flush(bool restart) {
	gfx->unmap(im_context.vertex_buffer);
	gfx->unmap(im_context.index_buffer);

	if (im_context.draw_cmd) {
		gfx->cmd_bind_vertex_buffer(im_context.vertex_buffer, sizeof(Im_Vertex));
		gfx->cmd_bind_index_buffer(im_context.index_buffer, Index_Type_U16);

		for (int draw_cmd_index = 0; draw_cmd_index < im_context.draw_cmd; ++draw_cmd_index) {
			Im_Draw_Cmd *cmd = im_context.draw_cmds + draw_cmd_index;

			gfx->cmd_bind_textures(0, 1, &cmd->texture);
			gfx->cmd_draw_indexed(cmd->count, cmd->index * sizeof(Im_Index), cmd->vertex);
		}

		auto bound_texture = im_context.draw_cmds[im_context.draw_cmd - 1].texture;

		im_context.draw_cmd = 0;
		im_context.vertex   = 0;
		im_context.index    = 0;
		im_context.counter  = 0;

		im_start_cmd_record(bound_texture);
	}

	if (restart) {
		im_context.vertex_ptr = (Im_Vertex *)gfx->map(im_context.vertex_buffer, Map_Type_WRITE_DISCARD);
		im_context.index_ptr  = (Im_Index *)gfx->map(im_context.index_buffer, Map_Type_WRITE_DISCARD);
	}
}

void im_end() {
	if (im_context.counter) {
		im_push_draw_cmd();
	}
	iim_flush(false);
}

void im_bind_texture(Texture2d_Handle handle) {
	if (im_context.counter) {
		im_push_draw_cmd();
		im_start_cmd_record(handle.view);
	} else {
		auto draw_cmd     = im_get_draw_cmd();
		draw_cmd->texture = handle.view;
	}
}

void im_unbind_texture() {
	im_bind_texture(white_texture);
}

void im_push_matrix(const Mat4 &matrix) {
	assert(im_context.transformation < IM_CONTEXT_MAX_TRANSFORMATIONS);
	im_context.transformations[im_context.transformation] = im_context.transformations[im_context.transformation - 1] * matrix;
	im_context.transformation += 1;
}

void im_pop_matrix() {
	assert(im_context.transformation > 1);
	im_context.transformation -= 1;
}

void im_flush_transformations() {
	im_context.transformation = 1;
}

void im_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color) {
	im_ensure_size(3, 3);

	Im_Index  indices[]  = { 0, 1, 2 };
	Im_Vertex vertices[] = {
		{ a, uv_a, color },
		{ b, uv_b, color },
		{ c, uv_c, color },
	};

	im_push_indices(indices, static_count(indices));
	im_push_vertices(vertices, static_count(vertices));
}

void im_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color) {
	im_triangle(vec3(a, 1), vec3(b, 1), vec3(c, 1), uv_a, uv_b, uv_c, color);
}

void im_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color) {
	im_triangle(a, b, c, vec2(0), vec2(0), vec2(0), color);
}

void im_triangle(Vec2 a, Vec2 b, Vec2 c, Color4 color) {
	im_triangle(vec3(a, 1), vec3(b, 1), vec3(c, 1), color);
}

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_ensure_size(4, 6);

	Im_Index  indices[]  = { 0, 1, 2, 0, 2, 3 };
	Im_Vertex vertices[] = {
		{ a, uv_a, color },
		{ b, uv_b, color },
		{ c, uv_c, color },
		{ d, uv_d, color },
	};

	im_push_indices(indices, static_count(indices));
	im_push_vertices(vertices, static_count(vertices));
}

void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_quad(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), uv_a, uv_b, uv_c, uv_d, color);
}

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color) {
	im_quad(a, b, c, d, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color) {
	im_quad(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), color);
}

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Mm_Rect rect, Color4 color) {
	im_quad(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), rect, color);
}

void im_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec3 a = pos;
	Vec3 b = vec3(pos.x, pos.y + dim.y, pos.z);
	Vec3 c = vec3(pos.xy + dim, pos.z);
	Vec3 d = vec3(pos.x + dim.x, pos.y, pos.z);
	im_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_rect(vec3(pos, 1), dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect(Vec3 pos, Vec2 dim, Color4 color) {
	im_rect(pos, dim, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im_rect(Vec2 pos, Vec2 dim, Color4 color) {
	im_rect(vec3(pos, 1), dim, color);
}

void im_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im_rect(pos, dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	im_rect(vec3(pos, 1), dim, rect, color);
}

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
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

	im_quad(vec3(a, pos.z), vec3(b, pos.z), vec3(c, pos.z), vec3(d, pos.z), uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_rect_rotated(vec3(pos, 1), dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color) {
	im_rect_rotated(pos, dim, angle, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color) {
	im_rect_rotated(vec3(pos, 1), dim, angle, color);
}

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im_rect_rotated(pos, dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	im_rect_rotated(vec3(pos, 1), dim, angle, rect, color);
}

void im_ellipse(Vec3 pos, r32 radius_a, r32 radius_b, Color4 color, int segments) {
	segments = clamp(IM_MIN_CIRCLE_SEGMENTS, IM_MAX_CIRCLE_SEGMENTS - 1, segments);

	r32 px = im_unit_circle_cos[0] * radius_a;
	r32 py = im_unit_circle_sin[0] * radius_b;

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		int lookup = (int)(((r32)index / (r32)segments) * (IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im_triangle(pos, pos + vec3(npx, npy, 0), pos + vec3(px, py, 0), color);

		px = npx;
		py = npy;
	}
}

void im_ellipse(Vec2 pos, r32 radius_a, r32 radius_b, Color4 color, int segments) {
	im_ellipse(vec3(pos, 1), radius_a, radius_b, color, segments);
}

void im_circle(Vec3 pos, r32 radius, Color4 color, int segments) {
	im_ellipse(pos, radius, radius, color, segments);
}

void im_circle(Vec2 pos, r32 radius, Color4 color, int segments) {
	im_ellipse(vec3(pos, 1), radius, radius, color, segments);
}

void im_pie(Vec3 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	assert(theta_a >= 0 && theta_a <= MATH_PI * 2 && theta_b >= 0 && theta_b <= MATH_PI * 2 && theta_b >= theta_a);

	int first_index = (int)((0.5f * theta_a * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);
	int last_index  = (int)((0.5f * theta_b * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);

	auto value_count = last_index - first_index;
	segments         = min_value(segments, value_count);

	r32 px = im_unit_circle_cos[first_index] * radius_a;
	r32 py = im_unit_circle_sin[first_index] * radius_b;

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		auto lookup = first_index + (int)((r32)index / (r32)segments * (r32)value_count + 0.5f);

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im_triangle(pos, pos + vec3(npx, npy, 0), pos + vec3(px, py, 0), color);

		px = npx;
		py = npy;
	}
}

void im_pie(Vec2 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im_pie(vec3(pos, 1), radius_a, radius_b, theta_a, theta_b, color, segments);
}

void im_pie(Vec3 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im_pie(pos, radius, radius, theta_a, theta_b, color, segments);
}

void im_pie(Vec2 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im_pie(vec3(pos, 1), radius, radius, theta_a, theta_b, color, segments);
}

void im_cube(Vec3 position, Quat rotation, Vec3 scale,
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

	im_quad(vertices[0], vertices[1], vertices[2], vertices[3], rect0, color);
	im_quad(vertices[3], vertices[2], vertices[5], vertices[4], rect1, color);
	im_quad(vertices[4], vertices[5], vertices[6], vertices[7], rect2, color);
	im_quad(vertices[7], vertices[6], vertices[1], vertices[0], rect3, color);
	im_quad(vertices[1], vertices[6], vertices[5], vertices[2], rect4, color);
	im_quad(vertices[7], vertices[0], vertices[3], vertices[4], rect5, color);
}

void im_cube(Vec3 position, Quat rotation, Vec3 scale, Mm_Rect rect, Color4 color) {
	im_cube(position, rotation, scale, rect, rect, rect, rect, rect, rect, color);
}

void im_cube(Vec3 position, Quat rotation, Vec3 scale, Color4 color) {
	im_cube(position, rotation, scale, mm_rect(0, 0, 1, 1), color);
}

void im_line2d(Vec3 a, Vec3 b, Color4 color, r32 thickness) {
	if (vec3_equals(a, b, 0)) return;

	r32 dx   = b.x - a.x;
	r32 dy   = b.y - a.y;
	r32 ilen = 1.0f / sqrtf(dx * dx + dy * dy);
	dx *= (thickness * ilen);
	dy *= (thickness * ilen);

	im_quad(vec3(a.x - dy, a.y + dx, a.z), vec3(b.x - dy, b.y + dx, b.z),
			vec3(b.x + dy, b.y - dx, b.z), vec3(a.x + dy, a.y - dx, a.z), color);
}

void im_line2d(Vec2 a, Vec2 b, Color4 color, r32 thickness) {
	im_line2d(vec3(a, 1), vec3(b, 1), color, thickness);
}

void im_bezier_quadratic2d(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness, int segments) {
	Vec3 p = a, np;
	for (int seg_index = 0; seg_index <= segments; ++seg_index) {
		r32 t = (r32)seg_index / (r32)segments;
		np    = bezier_quadratic(a, b, c, t);
		im_line2d(p, np, color, thickness);
		p = np;
	}
}

void im_bezier_quadratic2d(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness, int segments) {
	im_bezier_quadratic2d(vec3(a, 1), vec3(b, 1), vec3(c, 1), color, thickness, segments);
}

void im_bezier_cubic2d(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness, int segments) {
	Vec3 p = a, np;
	for (int seg_index = 0; seg_index <= segments; ++seg_index) {
		r32 t = (r32)seg_index / (r32)segments;
		np    = bezeir_cubic(a, b, c, d, t);
		im_line2d(p, np, color, thickness);
		p = np;
	}
}

void im_bezier_cubic2d(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness, int segments) {
	im_bezier_cubic2d(vec3(a, 1), vec3(b, 1), vec3(c, 1), vec3(d, 1), color, thickness, segments);
}

void im_triangle_outline2d(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness) {
	im_line2d(a, b, color, thickness);
	im_line2d(b, c, color, thickness);
	im_line2d(c, a, color, thickness);
}

void im_triangle_outline2d(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness) {
	im_line2d(a, b, color, thickness);
	im_line2d(b, c, color, thickness);
	im_line2d(c, a, color, thickness);
}

void im_quad_outline2d(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness) {
	im_line2d(a, b, color, thickness);
	im_line2d(b, c, color, thickness);
	im_line2d(c, d, color, thickness);
	im_line2d(a, d, color, thickness);
}

void im_quad_outline2d(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness) {
	im_line2d(a, b, color, thickness);
	im_line2d(b, c, color, thickness);
	im_line2d(c, d, color, thickness);
	im_line2d(a, d, color, thickness);
}

void im_rect_outline2d(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness) {
	im_line2d(a, b, color, thickness);
	im_line2d(b, c, color, thickness);
	im_line2d(c, d, color, thickness);
	im_line2d(a, d, color, thickness);
}

void im_rect_outline2d(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness) {
	im_line2d(a, b, color, thickness);
	im_line2d(b, c, color, thickness);
	im_line2d(c, d, color, thickness);
	im_line2d(a, d, color, thickness);
}

void im_ellipse_outline(Vec3 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness, int segments) {
	segments = clamp(IM_MIN_CIRCLE_SEGMENTS, IM_MAX_CIRCLE_SEGMENTS - 1, segments);

	r32 px = im_unit_circle_cos[0] * radius_a;
	r32 py = im_unit_circle_sin[0] * radius_b;

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		int lookup = (int)(((r32)index / (r32)segments) * (IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im_line2d(position + vec3(px, py, 0), position + vec3(npx, npy, 0), color, thickness);

		px = npx;
		py = npy;
	}
}

void im_ellipse_outline(Vec2 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness, int segments) {
	im_ellipse_outline(vec3(position, 1), radius_a, radius_b, color, thickness, segments);
}

void im_circle_outline(Vec3 position, r32 radius, Color4 color, r32 thickness, int segments) {
	im_ellipse_outline(position, radius, radius, color, thickness, segments);
}

void im_circle_outline(Vec2 position, r32 radius, Color4 color, r32 thickness, int segments) {
	im_ellipse_outline(vec3(position, 1), radius, radius, color, thickness, segments);
}

void im_arc_outline(Vec3 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	assert(theta_a >= 0 && theta_a <= MATH_PI * 2 && theta_b >= 0 && theta_b <= MATH_PI * 2 && theta_b >= theta_a);

	int first_index = (int)((0.5f * theta_a * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);
	int last_index  = (int)((0.5f * theta_b * MATH_PI_INVERSE) * (r32)(IM_MAX_CIRCLE_SEGMENTS - 1) + 0.5f);

	auto value_count = last_index - first_index;
	segments         = min_value(segments, value_count);

	r32 px = im_unit_circle_cos[first_index] * radius_a;
	r32 py = im_unit_circle_sin[first_index] * radius_b;

	if (closed) {
		im_line2d(position, position + vec3(px, py, 0), color, thickness);
	}

	r32 npx, npy;
	for (int index = 1; index <= segments; ++index) {
		auto lookup = first_index + (int)((r32)index / (r32)segments * (r32)value_count + 0.5f);

		npx = im_unit_circle_cos[lookup] * radius_a;
		npy = im_unit_circle_sin[lookup] * radius_b;

		im_line2d(position + vec3(px, py, 0), position + vec3(npx, npy, 0), color, thickness);

		px = npx;
		py = npy;
	}

	if (closed) {
		im_line2d(position, position + vec3(px, py, 0), color, thickness);
	}
}

void im_arc_outline(Vec2 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im_arc_outline(vec3(position, 1), radius_a, radius_b, theta_a, theta_b, color, closed, thickness, segments);
}

void im_arc_outline(Vec3 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im_arc_outline(position, radius, radius, theta_a, theta_b, color, closed, thickness, segments);
}

void im_arc_outline(Vec2 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im_arc_outline(vec3(position, 1), radius, radius, theta_a, theta_b, color, closed, thickness, segments);
}
