#include "gfx_renderer.h"
#include "gfx_platform.h"
#include "string.h"
#include "systems.h"
#include "utility.h"

struct Im_Vertex {
	Vec3   position;
	Vec2   texture_coord;
	Color4 color;
};
typedef u16 Im_Index;

struct Im_Draw_Cmd {
	Handle texture;
	u32    vertex;
	u32    index;
	u32    count;
};

static constexpr int IM_CONTEXT_MAX_VERTICES        = 2048 * 4;
static constexpr int IM_CONTEXT_MAX_INDICES         = IM_CONTEXT_MAX_VERTICES * 6;
static constexpr int IM_CONTEXT_MAX_TRANSFORMATIONS = 100;
static r32           im_unit_circle_cos[IM_MAX_CIRCLE_SEGMENTS];
static r32           im_unit_circle_sin[IM_MAX_CIRCLE_SEGMENTS];

struct Im_Context {
	Handle vertex_buffer;
	Handle index_buffer;

	Im_Draw_Cmd draw_cmds[IM_CONTEXT_MAX_VERTICES];
	Im_Vertex   vertices[IM_CONTEXT_MAX_VERTICES];
	Im_Index    indices[IM_CONTEXT_MAX_INDICES];
	int         draw_cmd;
	int         vertex;
	int         index;
	Im_Index    counter;

	Mat4 transformations[IM_CONTEXT_MAX_TRANSFORMATIONS];
	int  transformation;
};

static Im_Context im_context;

static Gfx_Platform *gfx;
static Handle        white_texture;

bool gfx_create_context(Handle platform, Render_Backend backend, s32 vsync, s32 multisamples, s32 framebuffer_w, s32 framebuffer_h) {
	if (backend == Render_Backend_OPENGL) {
		auto window_size = system_get_client_size();
		if (framebuffer_w == 0) framebuffer_w = window_size.x;
		if (framebuffer_h == 0) framebuffer_h = window_size.y;

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
	params.srgb        = false;
	params.wrap_s      = Texture_Wrap_REPEAT;
	params.wrap_t      = Texture_Wrap_REPEAT;

	const u8 white_pixels[] = { 0xff, 0xff, 0xff, 0xff };
	white_texture           = gfx->create_texture2d(1, 1, 4, white_pixels, &params);

	im_context.vertex_buffer      = gfx->create_vertex_buffer(Buffer_Type_DYNAMIC, sizeof(*im_context.vertices) * IM_CONTEXT_MAX_VERTICES, 0);
	im_context.index_buffer       = gfx->create_index_buffer(Buffer_Type_DYNAMIC, sizeof(Im_Index) * IM_CONTEXT_MAX_INDICES, 0);
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

void gfx_destroy_context() {
	gfx->destroy_vertex_buffer(im_context.vertex_buffer);
	gfx->destroy_index_buffer(im_context.index_buffer);
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

inline void im_start_cmd_record(Handle texture) {
	auto draw_cmd     = im_context.draw_cmds + im_context.draw_cmd;
	draw_cmd->index   = im_context.index;
	draw_cmd->vertex  = im_context.vertex;
	draw_cmd->texture = texture;
	draw_cmd->count   = 0;
}

inline Im_Vertex *im_push_vertex_count(int count) {
	assert(im_context.vertex + count <= IM_CONTEXT_MAX_VERTICES);

	auto vertex = im_context.vertices + im_context.vertex;
	im_context.vertex += count;
	return vertex;
}

inline void im_push_vertex(Vec3 position, Vec2 texture_coord, Color4 color) {
	auto vertex = im_push_vertex_count(1);

	if (im_context.transformation) {
		position = (im_context.transformations[im_context.transformation - 1] * vec4(position, 0)).xyz;
	}

	vertex->position      = position;
	vertex->texture_coord = texture_coord;
	vertex->color         = color;
}

inline Im_Index *im_push_index_count(int count) {
	assert(im_context.index + count <= IM_CONTEXT_MAX_INDICES);
	auto index = im_context.indices + im_context.index;
	im_context.index += count;
	im_context.counter += count;
	return index;
}

#define im_get_next_index() (im_context.counter ? im_context.indices[im_context.index - 1] + 1 : 0)
#define im_get_draw_cmd()   (im_context.draw_cmds + im_context.draw_cmd)

inline void im_push_index(int count) {
	assert(im_context.index + count <= IM_CONTEXT_MAX_INDICES);
	auto index   = im_get_next_index();
	auto indices = im_push_index_count(count);
	for (int i = 0; i < count; ++i)
		indices[i] = index + i;
}

inline void im_push_draw_cmd() {
	auto last_cmd   = im_context.draw_cmds + im_context.draw_cmd;
	last_cmd->count = im_context.counter;
	im_context.draw_cmd += 1;
	im_context.counter = 0;
}

inline void im_ensure_size(int vertex_count, int index_count) {
	if (im_context.index + index_count - 1 >= IM_CONTEXT_MAX_INDICES ||
		im_context.vertex + vertex_count - 1 >= IM_CONTEXT_MAX_VERTICES) {
		im_push_draw_cmd();
		im_flush();
	}
}

void im_begin(Handle shader, Camera_View &view, Mat4 &transform) {
	im_context.draw_cmd       = 0;
	im_context.vertex         = 0;
	im_context.index          = 0;
	im_context.counter        = 0;
	im_context.transformation = 1;

	im_start_cmd_record(white_texture);

	Mat4 projection;
	if (gfx->backend == Render_Backend_OPENGL) {
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
	} else {
		projection = mat4_identity();
	}

	transform = projection * transform;

	gfx->begin(shader, (u8 *)&transform, sizeof(transform));
}

void im_flush() {
	if (im_context.draw_cmd) {
		gfx->update_vertex_buffer(im_context.vertex_buffer, 0, im_context.vertex * sizeof(*im_context.vertices), im_context.vertices);
		gfx->update_index_buffer(im_context.index_buffer, 0, im_context.index * sizeof(*im_context.indices), im_context.indices);

		gfx->bind_vertex_buffer(im_context.vertex_buffer);
		gfx->bind_index_buffer(im_context.index_buffer, Render_Index_Type_U16);

		for (int draw_cmd_index = 0; draw_cmd_index < im_context.draw_cmd; ++draw_cmd_index) {
			Im_Draw_Cmd *cmd = im_context.draw_cmds + draw_cmd_index;
			gfx->bind_texture(cmd->texture, 0);
			gfx->draw_indexed(cmd->count, cmd->index * sizeof(Im_Index), cmd->vertex);
		}

		auto bound_texture = im_context.draw_cmds[im_context.draw_cmd - 1].texture;

		im_context.draw_cmd = 0;
		im_context.vertex   = 0;
		im_context.index    = 0;
		im_context.counter  = 0;

		im_start_cmd_record(bound_texture);
	}
}

void im_end() {
	if (im_context.counter) {
		im_push_draw_cmd();
	}
	im_flush();
	gfx->end();
}

void im_bind_texture(Handle texture) {
	if (im_context.counter) {
		im_push_draw_cmd();
		im_start_cmd_record(texture);
	} else {
		auto draw_cmd     = im_get_draw_cmd();
		draw_cmd->texture = texture;
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
	im_push_index(3);
	im_push_vertex(a, uv_a, color);
	im_push_vertex(b, uv_b, color);
	im_push_vertex(c, uv_c, color);
}

void im_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color) {
	im_triangle(vec3(a, 0), vec3(b, 0), vec3(c, 0), uv_a, uv_b, uv_c, color);
}

void im_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color) {
	im_triangle(a, b, c, vec2(0), vec2(0), vec2(0), color);
}

void im_triangle(Vec2 a, Vec2 b, Vec2 c, Color4 color) {
	im_triangle(vec3(a, 0), vec3(b, 0), vec3(c, 0), color);
}

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_ensure_size(4, 6);
	auto count   = im_get_next_index();
	auto indices = im_push_index_count(6);
	indices[0]   = count + 0;
	indices[1]   = count + 1;
	indices[2]   = count + 2;
	indices[3]   = count + 0;
	indices[4]   = count + 2;
	indices[5]   = count + 3;

	im_push_vertex(a, uv_a, color);
	im_push_vertex(b, uv_b, color);
	im_push_vertex(c, uv_c, color);
	im_push_vertex(d, uv_d, color);
}

void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_quad(vec3(a, 0), vec3(b, 0), vec3(c, 0), vec3(d, 0), uv_a, uv_b, uv_c, uv_d, color);
}

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color) {
	im_quad(a, b, c, d, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color) {
	im_quad(vec3(a, 0), vec3(b, 0), vec3(c, 0), vec3(d, 0), color);
}

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Mm_Rect rect, Color4 color) {
	im_quad(vec3(a, 0), vec3(b, 0), vec3(c, 0), vec3(d, 0), rect, color);
}

void im_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	Vec3 a = pos;
	Vec3 b = vec3(pos.x, pos.y + dim.y, pos.z);
	Vec3 c = vec3(pos.xy + dim, pos.z);
	Vec3 d = vec3(pos.x + dim.x, pos.y, pos.z);
	im_quad(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color) {
	im_rect(vec3(pos, 0), dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect(Vec3 pos, Vec2 dim, Color4 color) {
	im_rect(pos, dim, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im_rect(Vec2 pos, Vec2 dim, Color4 color) {
	im_rect(vec3(pos, 0), dim, color);
}

void im_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im_rect(pos, dim, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color) {
	im_rect(vec3(pos, 0), dim, rect, color);
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
	im_rect_rotated(vec3(pos, 0), dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color) {
	im_rect_rotated(pos, dim, angle, vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0), color);
}

void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color) {
	im_rect_rotated(vec3(pos, 0), dim, angle, color);
}

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	auto uv_a = rect.min;
	auto uv_b = vec2(rect.min.x, rect.max.y);
	auto uv_c = rect.max;
	auto uv_d = vec2(rect.max.x, rect.min.y);
	im_rect_rotated(pos, dim, angle, uv_a, uv_b, uv_c, uv_d, color);
}

void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color) {
	im_rect_rotated(vec3(pos, 0), dim, angle, rect, color);
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
	im_ellipse(vec3(pos, 0), radius_a, radius_b, color, segments);
}

void im_circle(Vec3 pos, r32 radius, Color4 color, int segments) {
	im_ellipse(pos, radius, radius, color, segments);
}

void im_circle(Vec2 pos, r32 radius, Color4 color, int segments) {
	im_ellipse(vec3(pos, 0), radius, radius, color, segments);
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
	im_pie(vec3(pos, 0), radius_a, radius_b, theta_a, theta_b, color, segments);
}

void im_pie(Vec3 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im_pie(pos, radius, radius, theta_a, theta_b, color, segments);
}

void im_pie(Vec2 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments) {
	im_pie(vec3(pos, 0), radius, radius, theta_a, theta_b, color, segments);
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
	im_line2d(vec3(a, 0), vec3(b, 0), color, thickness);
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
	im_bezier_quadratic2d(vec3(a, 0), vec3(b, 0), vec3(c, 0), color, thickness, segments);
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
	im_bezier_cubic2d(vec3(a, 0), vec3(b, 0), vec3(c, 0), vec3(d, 0), color, thickness, segments);
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
	im_ellipse_outline(vec3(position, 0), radius_a, radius_b, color, thickness, segments);
}

void im_circle_outline(Vec3 position, r32 radius, Color4 color, r32 thickness, int segments) {
	im_ellipse_outline(position, radius, radius, color, thickness, segments);
}

void im_circle_outline(Vec2 position, r32 radius, Color4 color, r32 thickness, int segments) {
	im_ellipse_outline(vec3(position, 0), radius, radius, color, thickness, segments);
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
	im_arc_outline(vec3(position, 0), radius_a, radius_b, theta_a, theta_b, color, closed, thickness, segments);
}

void im_arc_outline(Vec3 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im_arc_outline(position, radius, radius, theta_a, theta_b, color, closed, thickness, segments);
}

void im_arc_outline(Vec2 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed, r32 thickness, int segments) {
	im_arc_outline(vec3(position, 0), radius, radius, theta_a, theta_b, color, closed, thickness, segments);
}
