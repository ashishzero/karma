#pragma once
#include "shared.h"
#include "maths.h"
#include "gfx_types.h"

bool gfx_create_context(Handle platform, Render_Backend backend, s32 vsync, s32 multisamples, s32 framebuffer_w, s32 framebuffer_h);
void gfx_destroy_context();

Render_Backend gfx_render_backend();

Handle gfx_create_query();
void gfx_destroy_query(Handle query);
void gfx_begin_query(Handle query);
r32 gfx_end_query(Handle query);

void gfx_resize_framebuffer(s32 w, s32 h);
void gfx_get_framebuffer_size(s32* w, s32* h);

void gfx_set_swap_interval(s32 interval);
s32 gfx_get_swap_interval();

s32 gfx_get_multisamples();
void gfx_set_multisamples(s32 multisamples);

Handle gfx_create_shader(const String path);
void gfx_destroy_shader(Handle shader);

Handle gfx_create_texture2d(s32 w, s32 h, s32 n, const u8* pixels, Sampler_Params* params = 0);
void gfx_update_texture2d(Handle texture, s32 xoffset, s32 yoffset, s32 w, s32 h, s32 n, u8* pixels);
void gfx_destroy_texture2d(Handle texture);

void gfx_present();

void gfx_frame(Framebuffer* framebuffer, Render_Region& region, Clear_Flag flags, Color4 color = vec4(0, 0, 0, 1));

void gfx2d_begin(Vec2 position, float zoom, Camera_View& view, Handle shader);
void gfx2d_end();
void gfx2d_push_matrix(const Mat3 &matrix);
void gfx2d_pop_matrix();
void gfx2d_flush_transformation_stack();
void gfx2d_texture(Handle texture);
void gfx2d_no_texture();
void gfx2d_color(Color4 color);
void gfx2d_color(Color4 a, Color4 b, Color4 c, Color4 d);
void gfx2d_quad(Vec2 v0, Vec2 v1, Vec2 v2, Vec2 v3, Mm_Rect texture_rect, r32 depth = 0);
void gfx2d_rect(Vec2 p, Vec2 d, Mm_Rect texture_rect, r32 depth = 0);
