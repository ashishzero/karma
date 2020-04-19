#include "gfx_renderer.h"
#include "gfx_platform.h"
#include "string.h"
#include "systems.h"
#include "utility.h"

struct Quad_Vertex {
	Vec2   position;
	Vec2   tex_coord;
	Color4 color;
};

struct Quad_Buffer {
	Mat4 transform;
};

struct Quad {
	Vec2   v[4];
	Vec2   t[4];
	Color4 color[4];
	Handle texture;
};

struct Quad_Sort_Key {
	u32 key;
	u32 index;
};

struct Quad_Render_Range {
	Handle texture;
	u32	index;
	u32	count;
};

static Gfx_Platform *gfx;
static Handle		 white_texture;

static constexpr int	 MAX_QUADS_PER_CALL = 2048;
static Quad				 quads[MAX_QUADS_PER_CALL];
static Quad_Sort_Key	 quad_sort_keys[MAX_QUADS_PER_CALL];
static Quad_Render_Range quad_render_range[MAX_QUADS_PER_CALL];
static Mat3				 quad_transformation_stack[MAX_QUADS_PER_CALL];
static u32				 quad_top;
static u32				 quad_transformation_top;
static Color4			 quad_top_colors[4] = { vec4(1), vec4(1), vec4(1), vec4(1) };
static Handle			 quad_top_texture;
static Handle			 quad_vertex_buffer;
static Handle			 quad_index_buffer;

bool gfx_create_context(Handle platform, Render_Backend backend, s32 vsync, s32 multisamples, s32 framebuffer_w, s32 framebuffer_h) {
	if (backend == Render_Backend_OPENGL) {
		system_log(LOG_INFO, "Gfx", "gfx.backend = Render_Backend_OPENGL");
		gfx = create_opengl_context(platform, vsync, multisamples, framebuffer_w, framebuffer_h);
		if (gfx->backend == Render_Backend_NONE) {
			system_log(LOG_ERROR, "Gfx", "create_opengl_context() failed.");
			return false;
		}
	} else {
		return false;
	}

	Sampler_Params params;
	params.gen_mipmaps = false;
	params.mag_filter  = Texture_Filter_NEAREST;
	params.min_filter  = Texture_Filter_NEAREST;
	params.srgb		   = false;
	params.wrap_s	  = Texture_Wrap_REPEAT;
	params.wrap_t	  = Texture_Wrap_REPEAT;

	const u8 white_pixels[] = { 0xff, 0xff, 0xff, 0xff };
	white_texture			= gfx->create_texture2d(1, 1, 4, white_pixels, &params);

	u16  indices[MAX_QUADS_PER_CALL * 6];
	u16 *index = indices;

	for (int i = 0; i < MAX_QUADS_PER_CALL; ++i) {
		index[0] = i * 4 + 0;
		index[1] = i * 4 + 1;
		index[2] = i * 4 + 2;
		index[3] = i * 4 + 0;
		index[4] = i * 4 + 2;
		index[5] = i * 4 + 3;
		index += 6;
	}

	quad_vertex_buffer			 = gfx->create_vertex_buffer(Buffer_Type_DYNAMIC, sizeof(Quad_Vertex) * 4 * MAX_QUADS_PER_CALL, 0);
	quad_index_buffer			 = gfx->create_index_buffer(Buffer_Type_STATIC, sizeof(u16) * 6 * MAX_QUADS_PER_CALL, (void *)indices);
	quad_transformation_stack[0] = mat3_identity();
	quad_transformation_top		 = 1;

	return true;
}

void gfx_destroy_context() {
	gfx->destroy_vertex_buffer(quad_vertex_buffer);
	gfx->destroy_index_buffer(quad_index_buffer);
	gfx->destroy();
}

Render_Backend gfx_render_backend() {
	return gfx->backend;
}

Handle gfx_create_query() {
	return gfx->create_query();
}

void gfx_destroy_query(Handle query) {
	gfx->destroy_query(query);
}

void gfx_begin_query(Handle query) {
	gfx->begin_query(query);
}

r32 gfx_end_query(Handle query) {
	return gfx->end_query(query);
}

void gfx_resize_framebuffer(s32 w, s32 h) {
	gfx->resize_framebuffer(w, h);
}

void gfx_get_framebuffer_size(s32 *w, s32 *h) {
	gfx->get_framebuffer_size(w, h);
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

void gfx_set_multisamples(s32 multisamples) {
	gfx->set_multisamples(multisamples);
}

Handle gfx_create_shader(const String path) {
	String content = system_read_entire_file(tprintf("%s.glsl", tto_cstring(path)));
	Handle handle  = {};
	if (content) {
		handle = gfx->create_shader(content);
	}
	mfree(content.data);
	return handle;
}

void gfx_destroy_shader(Handle shader) {
	gfx->destroy_shader(shader);
}

void gfx_present() {
	gfx->present();
}

static inline void quad_to_vertex(Quad_Vertex *vertex, const Quad &quad) {
	for (int i = 0; i < 4; ++i) {
		vertex[i].position  = quad.v[i];
		vertex[i].tex_coord = quad.t[i];
		vertex[i].color		= quad.color[i];
	}
}

Handle gfx_create_texture2d(s32 w, s32 h, s32 n, const u8 *pixels, Sampler_Params *params) {
	return gfx->create_texture2d(w, h, n, pixels, params);
}

void gfx_update_texture2d(Handle texture, s32 xoffset, s32 yoffset, s32 w, s32 h, s32 n, u8 *pixels) {
	gfx->update_texture2d(texture, xoffset, yoffset, w, h, n, pixels);
}

void gfx_destroy_texture2d(Handle texture) {
	gfx->destroy_texture2d(texture);
}

void gfx_frame(Framebuffer *framebuffer, Render_Region &region, Clear_Flag flags, Color4 color) {
	gfx->bind_framebuffer(framebuffer);
	gfx->clear(flags, color);
	gfx->set_render_region(region);
}

void gfx2d_begin(Vec2 position, float zoom, Camera_View &view, Handle shader) {
	quad_top				= 0;
	quad_transformation_top = 1;

	 Mat4 projection;
	if (gfx->backend == Render_Backend_OPENGL) {
		if (view.kind == Camera_View::ORTHOGRAPHIC)
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
	} else {
		projection = mat4_identity();
	}

	Quad_Buffer buf;
	buf.transform = projection * mat4_translation(vec3(position, 0)) * mat4_scalar(vec3(zoom));

	gfx->begin(shader, (u8 *)&buf, sizeof(buf));
}

void gfx2d_end() {
	if (!quad_top) {
		gfx->end();
		return;
	}

	sort_insert(quad_sort_keys, quad_top, [](const Quad_Sort_Key *a, const Quad_Sort_Key *b) -> int { return a->key - b->key; });

	Quad_Vertex *vertex = (Quad_Vertex *)gfx->map_vertex_buffer(quad_vertex_buffer, 0, sizeof(Quad_Vertex) * 4 * quad_top);

	quad_render_range[0].index   = 0;
	quad_render_range[0].count   = 6;
	quad_render_range[0].texture = quads[quad_sort_keys[0].index].texture;

	u32 render_range_index = 0;

	auto prev_tex_handle = quads[quad_sort_keys[0].index].texture;

	quad_to_vertex(vertex, quads[quad_sort_keys[0].index]);
	vertex += 4;

	for (u32 index = 1; index < quad_top; index += 1) {
		const Quad &quad = quads[quad_sort_keys[index].index];

		quad_to_vertex(vertex, quad);
		vertex += 4;

		if (prev_tex_handle.hptr == quad.texture.hptr) {
			quad_render_range[render_range_index].count += 6;
		} else {
			render_range_index += 1;
			quad_render_range[render_range_index].index   = 4 * index;
			quad_render_range[render_range_index].count   = 6;
			quad_render_range[render_range_index].texture = quad.texture;
		}
	}

	gfx->unmap_vertex_buffer(quad_vertex_buffer);

	gfx->bind_vertex_buffer(quad_vertex_buffer);
	gfx->bind_index_buffer(quad_index_buffer, Render_Index_Type_U16);

	auto range_count = render_range_index + 1;

	auto *range = quad_render_range;
	for (u32 i = 0; i < range_count; ++i, ++range) {
		gfx->bind_texture(range->texture, 0);
		gfx->draw_indexed(range->count, range->index);
	}

	gfx->end();
}

void gfx2d_push_matrix(const Mat3 &matrix) {
	assert(quad_transformation_top < MAX_QUADS_PER_CALL);
	quad_transformation_stack[quad_transformation_top] = quad_transformation_stack[quad_transformation_top - 1] * matrix;
	quad_transformation_top += 1;
}

void gfx2d_pop_matrix() {
	assert(quad_transformation_top > 1);
	quad_transformation_top -= 1;
}

void gfx2d_flush_transformation_stack() {
	quad_transformation_top = 1;
}

void gfx2d_texture(Handle texture) {
	quad_top_texture = texture;
}

void gfx2d_no_texture() {
	quad_top_texture = white_texture;
}

void gfx2d_color(Color4 color) {
	gfx2d_color(color, color, color, color);
}

void gfx2d_color(Color4 a, Color4 b, Color4 c, Color4 d) {
	quad_top_colors[0] = a;
	quad_top_colors[1] = b;
	quad_top_colors[2] = c;
	quad_top_colors[3] = d;
}

void gfx2d_quad(Vec2 v0, Vec2 v1, Vec2 v2, Vec2 v3, Mm_Rect texture_rect, r32 depth) {
	if (quad_top == MAX_QUADS_PER_CALL) {
		gfx2d_end();
		quad_top = 0;
	}

	Quad &		   quad = quads[quad_top];
	Quad_Sort_Key &sort = quad_sort_keys[quad_top];

	if (quad_transformation_top > 1) {
		Mat3 &transformation = quad_transformation_stack[quad_transformation_top - 1];
		v0					 = (transformation * vec3(v0, 0)).xy;
		v1					 = (transformation * vec3(v1, 0)).xy;
		v2					 = (transformation * vec3(v2, 0)).xy;
		v3					 = (transformation * vec3(v3, 0)).xy;
	}

	quad.v[0]	 = v0;
	quad.t[0]	 = vec2(texture_rect.min.x, texture_rect.min.y);
	quad.color[0] = quad_top_colors[0];

	quad.v[1]	 = v1;
	quad.t[1]	 = vec2(texture_rect.min.x, texture_rect.max.y);
	quad.color[1] = quad_top_colors[1];

	quad.v[2]	 = v2;
	quad.t[2]	 = vec2(texture_rect.max.x, texture_rect.max.y);
	quad.color[2] = quad_top_colors[2];

	quad.v[3]	 = v3;
	quad.t[3]	 = vec2(texture_rect.max.x, texture_rect.min.y);
	quad.color[3] = quad_top_colors[3];

	quad.texture = quad_top_texture;

	u16 hl = MAX_UINT16 - (u32)((depth / MAX_FLOAT) * 65535.0f);

	sort.index = quad_top;
	sort.key   = (((u32)hl) << 16) | (0x0000ffff & (ptrsize)quad_top_texture.hptr);

	quad_top += 1;
}

void gfx2d_rect(Vec2 p, Vec2 d, Mm_Rect texture_rect, r32 depth) {
	auto v0 = p;
	auto v1 = vec2(p.x, p.y + d.y);
	auto v2 = p + d;
	auto v3 = vec2(p.x + d.x, p.y);
	gfx2d_quad(v0, v1, v2, v3, texture_rect, depth);
}
