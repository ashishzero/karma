#pragma once
#include "karma.h"
#include "lin_maths.h"
#include "gfx_types.h"

static constexpr int IM_MIN_CIRCLE_SEGMENTS     = 12;
static constexpr int IM_MAX_CIRCLE_SEGMENTS     = 512;
static constexpr int IM_DEFAULT_CIRCLE_SEGMENTS = 48;
static constexpr int IM_DEFAULT_BEZIER_SEGMENTS = 48;

bool gfx_create_context(Handle platform, Render_Backend backend, s32 vsync, s32 multisamples, s32 framebuffer_w = 0, s32 framebuffer_h = 0);
void gfx_destroy_context();

Render_Backend gfx_render_backend();

Handle gfx_create_query();
void   gfx_destroy_query(Handle query);
void   gfx_begin_query(Handle query);
r32    gfx_end_query(Handle query);

void gfx_resize_framebuffer(s32 w, s32 h);
void gfx_get_framebuffer_size(s32 *w, s32 *h);

void gfx_set_swap_interval(s32 interval);
s32  gfx_get_swap_interval();

s32  gfx_get_multisamples();
void gfx_set_multisamples(s32 multisamples);

Handle gfx_create_shader(const String path);
void   gfx_destroy_shader(Handle shader);

Handle gfx_create_texture2d(s32 w, s32 h, s32 n, const u8 *pixels, Sampler_Params *params = 0);
void   gfx_update_texture2d(Handle texture, s32 xoffset, s32 yoffset, s32 w, s32 h, s32 n, u8 *pixels);
void   gfx_destroy_texture2d(Handle texture);

void gfx_present();

void gfx_frame(Framebuffer *framebuffer, Render_Region &region, Clear_Flag flags, Color4 color = vec4(0, 0, 0, 1));

void im_begin(Handle shader, Camera_View &view, Mat4 &transform = mat4_identity());
void im_flush();
void im_end();

void im_bind_texture(Handle texture);
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
void im_rect_outline2d(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Color4 color, r32 thickness = 1);
void im_rect_outline2d(Vec2 a, Vec2 b, Vec2 c, Vec2 d, Color4 color, r32 thickness = 1);

void im_ellipse_outline(Vec3 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_ellipse_outline(Vec2 position, r32 radius_a, r32 radius_b, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_circle_outline(Vec3 position, r32 radius, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_circle_outline(Vec2 position, r32 radius, Color4 color, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);

void im_arc_outline(Vec3 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_arc_outline(Vec2 position, r32 radius_a, r32 radius_b, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_arc_outline(Vec3 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
void im_arc_outline(Vec2 position, r32 radius, r32 theta_a, r32 theta_b, Color4 color, bool closed = false, r32 thickness = 1, int segments = IM_DEFAULT_CIRCLE_SEGMENTS);
