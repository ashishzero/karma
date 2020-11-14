#pragma once
#include "modules/core/karma.h"
#include "modules/core/lin_maths.h"
#include "modules/core/gfx_types.h"

static constexpr int IM_MIN_CIRCLE_SEGMENTS     = 12;
static constexpr int IM_MAX_CIRCLE_SEGMENTS     = 512;
static constexpr int IM_DEFAULT_CIRCLE_SEGMENTS = 48;
static constexpr int IM_DEFAULT_BEZIER_SEGMENTS = 48;

extern r32 im2d_stroke_weight;

struct Monospaced_Font_Glyph_Range {
	r32     width, height;
	Vec2    offset;
	Mm_Rect rect;
};

struct Monospaced_Font_Info {
	Monospaced_Font_Glyph_Range *range;
	s32                          first;
	s32                          count;
	r32                          advance;
};

struct Monospaced_Font {
	Texture2d_Handle     texture;
	Monospaced_Font_Info info;
};

struct Im_Mesh {
	struct Index {
		u32 v;
		u32 t;
		u32 n;
	};

	void *ptr;
	Index	*indices;
	Vec3	*vertices;
	Vec2	*texture_coords;
	Vec3	*normals;

	u32 index_count;
	u32 vertex_count;
	u32 texture_coord_count;
	u32 normal_count;
};

bool gfx_create_context(Handle platform, Render_Backend backend, Vsync vsync, u32 multisamples, u32 framebuffer_w = 1280, u32 framebuffer_h = 720);
void gfx_destroy_context();

void gfx_resize_framebuffer(u32 width, u32 height, u32 bloom_w = 512, u32 bloom_h = 512);

Render_Backend gfx_render_backend();
void *         gfx_render_device();
void *         gfx_render_context();

void gfx_on_client_resize(u32 w, u32 h);
void gfx_get_render_view_size(u32 *w, u32 *h);

Mat4 gfx_view_transform(Camera_View &view);

void  gfx_set_sync_interval(Vsync vsync);
Vsync gfx_get_sync_interval();

s32 gfx_get_multisamples();

Texture2d_Handle gfx_create_texture2d(u32 w, u32 h, u32 channels, Data_Format fmt, const u8 **pixels, Buffer_Usage usage, u32 mip_levels);
void             gfx_update_texture2d(Texture2d_Handle handle, u32 xoffset, u32 yoffset, u32 w, u32 h, u32 channels, u8 *pixels);
void             gfx_destroy_texture2d(Texture2d_Handle handle);

void gfx_present();

void gfx_apply_blur(Texture_View src, Framebuffer ping_pongs[2], Texture_View dsts[2], Rect rect, int amount);

void gfx_apply_bloom(int amount);
void gfx_blit_hdr(r32 x, r32 y, r32 w, r32 h);

void gfx_begin_drawing(Framebuffer_Type target, Clear_Flags flags, Color4 color = vec4(1), r32 depth = 1, u8 stencil = 0xff);
void gfx_end_drawing();

void gfx_viewport(r32 x, r32 y, r32 w, r32 h);

void im2d_begin(const Mat4 &transform = mat4_identity());
void im2d_begin(Camera_View &view, const Mat4 &transform = mat4_identity());
void im2d_end();

void im2d_bind_texture(Texture2d_Handle handle);
void im2d_unbind_texture();

void im2d_push_matrix(const Mat4 &matrix);
void im2d_pop_matrix();
void im2d_flush_transformations();

void im2d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color);
void im2d_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color);

void im2d_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color);
void im2d_triangle(Vec2 a, Vec2 b, Vec2 c, Color4 color);

void im2d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im2d_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im2d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color);
void im2d_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color);

void im2d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color);
void im2d_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Mm_Rect rect, Color4 color);

void im2d_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im2d_rect(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im2d_rect(Vec3 pos, Vec2 dim, Color4 color);
void im2d_rect(Vec2 pos, Vec2 dim, Color4 color);

void im2d_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color);
void im2d_rect(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color);

void im2d_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im2d_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im2d_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color);
void im2d_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color);

void im2d_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);
void im2d_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);

void im2d_rect_centered(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im2d_rect_centered(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im2d_rect_centered(Vec3 pos, Vec2 dim, Color4 color);
void im2d_rect_centered(Vec2 pos, Vec2 dim, Color4 color);

void im2d_rect_centered(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color);
void im2d_rect_centered(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color);

void im2d_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im2d_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im2d_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color);
void im2d_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color);

void im2d_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);
void im2d_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);

void im2d_ellipse(Vec3 pos, r32 radius_a, r32 radius_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_ellipse(Vec2 pos, r32 radius_a, r32 radius_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_circle(Vec3 pos, r32 radius, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_circle(Vec2 pos, r32 radius, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_pie(Vec3 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_pie(Vec2 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_pie(Vec3 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_pie(Vec2 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_pie_part(Vec3 pos, r32 radius_a_min, r32 radius_b_min, r32 radius_a_max, r32 radius_b_max, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_pie_part(Vec2 pos, r32 radius_a_min, r32 radius_b_min, r32 radius_a_max, r32 radius_b_max, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_pie_part(Vec3 pos, r32 radius_min, r32 radius_max, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_pie_part(Vec2 pos, r32 radius_min, r32 radius_max, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_cube(Vec3 position, Quat rotation, Vec3 scale,
			 Mm_Rect rect0, Mm_Rect rect1, Mm_Rect rect2,
			 Mm_Rect rect3, Mm_Rect rect4, Mm_Rect rect5, Color4 color);
void im2d_cube(Vec3 position, Quat rotation, Vec3 scale, Mm_Rect rect, Color4 color);
void im2d_cube(Vec3 position, Quat rotation, Vec3 scale, Color4 color);

void im2d_set_stroke_weight(r32 weight);

void im2d_line(Vec3 a, Vec3 b, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_line(Vec2 a, Vec2 b, Color4 color, r32 thickness = im2d_stroke_weight);

void im2d_bezier_quadratic(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_BEZIER_SEGMENTS);
void im2d_bezier_quadratic(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_BEZIER_SEGMENTS);

void im2d_bezier_cubic(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_BEZIER_SEGMENTS);
void im2d_bezier_cubic(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_BEZIER_SEGMENTS);

void im2d_polygon(const Polygon &polygon, r32 z, Color4 color);
void im2d_polygon(const Polygon &polygon, Color4 color);

void im2d_triangle_outline(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_triangle_outline(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness = im2d_stroke_weight);

void im2d_quad_outline(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_quad_outline(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_rect_outline(Vec3 pos, Vec2 dim, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_rect_outline(Vec2 pos, Vec2 dim, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_rect_centered_outline(Vec3 pos, Vec2 dim, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_rect_centered_outline(Vec2 pos, Vec2 dim, Color4 color, r32 thickness = im2d_stroke_weight);

void im2d_ellipse_outline(Vec3 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_ellipse_outline(Vec2 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_circle_outline(Vec3 position, r32 radius, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_circle_outline(Vec2 position, r32 radius, Color4 color, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_arc_outline(Vec3 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_arc_outline(Vec2 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_arc_outline(Vec3 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im2d_arc_outline(Vec2 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = im2d_stroke_weight, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im2d_polygon_outline(const Polygon &polygon, r32 z, Color4 color, r32 thickness = im2d_stroke_weight);
void im2d_polygon_outline(const Polygon &polygon, Color4 color, r32 thickness = im2d_stroke_weight);

void im2d_text(Vec3 position, r32 scale, Monospaced_Font_Info &font, const String string, Color4 color);
void im2d_text(Vec2 position, r32 scale, Monospaced_Font_Info &font, const String string, Color4 color);
void im2d_text_region(Vec3 position, Vec2 region, Monospaced_Font_Info &font, const String string, Color4 color);
void im2d_text_region(Vec2 position, Vec2 region, Monospaced_Font_Info &font, const String string, Color4 color);
Vec2 im2d_calculate_text_region(r32 scale, Monospaced_Font_Info &font, const String string);

//
//
//

void im3d_begin(const Mat4 &transform);
void im3d_begin(Camera_View &view, const Mat4 &transform);
void im3d_end();

void im3d_bind_texture(Texture2d_Handle handle);
void im3d_unbind_texture();

void im3d_push_matrix(const Mat4 &matrix);
void im3d_pop_matrix();
void im3d_flush_transformations();

void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec3 na, Vec3 nb, Vec3 nc, Color4 color);
void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color);
void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Vec3 na, Vec3 nb, Vec3 nc, Color4 color);
void im3d_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color);
void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, Color4 color);
void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, Color4 color);
void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color);
void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Vec3 na, Vec3 nb, Vec3 nc, Vec3 nd, Color4 color);
void im3d_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color);
void im3d_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im3d_rect(Vec3 pos, Vec2 dim, Color4 color);
void im3d_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color);
void im3d_cube(Vec3 position, Quat rotation, Vec3 scale,
				Mm_Rect rect0, Mm_Rect rect1, Mm_Rect rect2,
				Mm_Rect rect3, Mm_Rect rect4, Mm_Rect rect5, Color4 color);
void im3d_cube(Vec3 position, Quat rotation, Vec3 scale, Mm_Rect rect, Color4 color);
void im3d_cube(Vec3 position, Quat rotation, Vec3 scale, Color4 color);
void im3d_mesh(Im_Mesh &mesh, Vec3 position, Quat rotation, Vec3 scale, Color4 color);
