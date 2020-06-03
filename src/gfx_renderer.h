#pragma once
#include "karma.h"
#include "lin_maths.h"
#include "gfx_types.h"

static constexpr int IM_MIN_CIRCLE_SEGMENTS     = 12;
static constexpr int IM_MAX_CIRCLE_SEGMENTS     = 512;
static constexpr int IM_DEFAULT_CIRCLE_SEGMENTS = 48;
static constexpr int IM_DEFAULT_BEZIER_SEGMENTS = 48;

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

bool gfx_create_context(Handle platform, Render_Backend backend, Vsync vsync, u32 multisamples, u32 framebuffer_w = 1280, u32 framebuffer_h = 720);
void gfx_destroy_context();

void gfx_resize_framebuffer(u32 width, u32 height, u32 bloom_w = 512, u32 bloom_h = 512);

Render_Backend gfx_render_backend();
void *         gfx_render_device();
void *         gfx_render_context();

void gfx_on_client_resize(u32 w, u32 h);
void gfx_get_render_view_size(u32 *w, u32 *h);

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

void im_debug_begin(r32 left, r32 right, r32 top, r32 bottom, r32 near = -1, r32 far = 1);
void im_begin(const Mat4 &transform = mat4_identity());
void im_begin(Camera_View &view, const Mat4 &transform = mat4_identity());
void im_end();

void im_bind_texture(Texture2d_Handle handle);
void im_unbind_texture();

void im_push_matrix(const Mat4 &matrix);
void im_pop_matrix();
void im_flush_transformations();

void im_triangle(Vec3 a, Vec3 b, Vec3 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color);
void im_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Color4 color);

void im_triangle(Vec3 a, Vec3 b, Vec3 c, Color4 color);
void im_triangle(Vec2 a, Vec2 b, Vec2 c, Color4 color);

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color);
void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color);

void im_quad(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Mm_Rect rect, Color4 color);
void im_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Mm_Rect rect, Color4 color);

void im_rect(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im_rect(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im_rect(Vec3 pos, Vec2 dim, Color4 color);
void im_rect(Vec2 pos, Vec2 dim, Color4 color);

void im_rect(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color);
void im_rect(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color);

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color);
void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color);

void im_rect_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);
void im_rect_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);

void im_rect_centered(Vec3 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im_rect_centered(Vec2 pos, Vec2 dim, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im_rect_centered(Vec3 pos, Vec2 dim, Color4 color);
void im_rect_centered(Vec2 pos, Vec2 dim, Color4 color);

void im_rect_centered(Vec3 pos, Vec2 dim, Mm_Rect rect, Color4 color);
void im_rect_centered(Vec2 pos, Vec2 dim, Mm_Rect rect, Color4 color);

void im_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);
void im_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Vec2 uv_a, Vec2 uv_b, Vec2 uv_c, Vec2 uv_d, Color4 color);

void im_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Color4 color);
void im_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Color4 color);

void im_rect_centered_rotated(Vec3 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);
void im_rect_centered_rotated(Vec2 pos, Vec2 dim, r32 angle, Mm_Rect rect, Color4 color);

void im_ellipse(Vec3 pos, r32 radius_a, r32 radius_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_ellipse(Vec2 pos, r32 radius_a, r32 radius_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_circle(Vec3 pos, r32 radius, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_circle(Vec2 pos, r32 radius, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_pie(Vec3 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_pie(Vec2 pos, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_pie(Vec3 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_pie(Vec2 pos, r32 radius, r32 theta_a, r32 theta_b, Color4 color, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_cube(Vec3 position, Quat rotation, Vec3 scale,
			 Mm_Rect rect0, Mm_Rect rect1, Mm_Rect rect2,
			 Mm_Rect rect3, Mm_Rect rect4, Mm_Rect rect5, Color4 color);
void im_cube(Vec3 position, Quat rotation, Vec3 scale, Mm_Rect rect, Color4 color);
void im_cube(Vec3 position, Quat rotation, Vec3 scale, Color4 color);

void im_line2d(Vec3 a, Vec3 b, Color4 color, r32 thickness = 1);
void im_line2d(Vec2 a, Vec2 b, Color4 color, r32 thickness = 1);

void im_bezier_quadratic2d(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_BEZIER_SEGMENTS);
void im_bezier_quadratic2d(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_BEZIER_SEGMENTS);

void im_bezier_cubic2d(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_BEZIER_SEGMENTS);
void im_bezier_cubic2d(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_BEZIER_SEGMENTS);

void im_triangle_outline2d(Vec3 a, Vec3 b, Vec3 c, Color4 color, r32 thickness = 1);
void im_triangle_outline2d(Vec2 a, Vec2 b, Vec2 c, Color4 color, r32 thickness = 1);

void im_quad_outline2d(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness = 1);
void im_quad_outline2d(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness = 1);
void im_rect_outline2d(Vec3 pos, Vec2 dim, Color4 color, r32 thickness = 1);
void im_rect_outline2d(Vec2 pos, Vec2 dim, Color4 color, r32 thickness = 1);

void im_ellipse_outline(Vec3 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_ellipse_outline(Vec2 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_circle_outline(Vec3 position, r32 radius, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_circle_outline(Vec2 position, r32 radius, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_arc_outline(Vec3 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_arc_outline(Vec2 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_arc_outline(Vec3 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_arc_outline(Vec2 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_text(Vec3 position, r32 scale, Monospaced_Font_Info &font, const String string, Color4 color);
void im_text(Vec2 position, r32 scale, Monospaced_Font_Info &font, const String string, Color4 color);
void im_text_region(Vec3 position, Vec2 region, Monospaced_Font_Info &font, const String string, Color4 color);
void im_text_region(Vec2 position, Vec2 region, Monospaced_Font_Info &font, const String string, Color4 color);
Vec2 im_calculate_text_region(r32 scale, Monospaced_Font_Info &font, const String string);
