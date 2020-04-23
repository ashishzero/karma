#pragma once

#include "gfx_types.h"

enum Render_Index_Type {
	Render_Index_Type_NONE,
	Render_Index_Type_U8,
	Render_Index_Type_U16,
	Render_Index_Type_U32,
};

struct Gfx_Platform_Info {
	const u8 *vendor;
	const u8 *renderer;
	const u8 *version;
	const u8 *shading_lang_version;
};

struct Gfx_Platform {
	Render_Backend    backend;
	Gfx_Platform_Info info;

	virtual s32  get_max_supported_framebuffer_size() = 0;
	virtual void resize_framebuffer(s32 w, s32 h)     = 0;
	virtual void get_framebuffer_size(s32 *w, s32 *h) = 0;

	virtual s32  get_maximum_supported_multisamples() = 0;
	virtual s32  get_multisamples()                   = 0;
	virtual void set_multisamples(s32 multisamples)   = 0;

	virtual void set_swap_interval(s32 interval) = 0;
	virtual s32  get_swap_interval()             = 0;

	virtual void present() = 0;

	virtual Handle create_query()              = 0;
	virtual void   destroy_query(Handle query) = 0;
	virtual void   begin_query(Handle query)   = 0;
	virtual r32    end_query(Handle query)     = 0;

	virtual Handle create_texture2d(s32 w, s32 h, s32 n, const u8 *pixels, Sampler_Params *params = 0)         = 0;
	virtual void   update_texture2d(Handle texture, s32 xoffset, s32 yoffset, s32 w, s32 h, s32 n, u8 *pixels) = 0;
	virtual void   destroy_texture2d(Handle texture)                                                           = 0;

	virtual Handle create_vertex_buffer(Buffer_Type type, u32 size, void *data) = 0;
	virtual Handle create_index_buffer(Buffer_Type type, u32 size, void *data)  = 0;

	virtual void update_vertex_buffer(Handle handle, u32 offset, u32 size, void *data) = 0;
	virtual void update_index_buffer(Handle handle, u32 offset, u32 size, void *data)  = 0;

	virtual void *map_vertex_buffer(Handle handle, u32 offset, u32 length) = 0;
	virtual void  unmap_vertex_buffer(Handle handle)                       = 0;
	virtual void *map_index_buffer(Handle handle, u32 offset, u32 length)  = 0;
	virtual void  unmap_index_buffer(Handle handle)                        = 0;

	virtual void destroy_vertex_buffer(Handle handle) = 0;
	virtual void destroy_index_buffer(Handle handle)  = 0;

	virtual Framebuffer create_framebuffer(s32 width, s32 height, Color_Format color, Depth_Format depth) = 0;
	virtual void        destroy_framebuffer(Framebuffer framebuffer)                                      = 0;

	virtual Handle create_shader(String src)     = 0;
	virtual void   destroy_shader(Handle shader) = 0;

	virtual void bind_framebuffer(Framebuffer *framebuffer) = 0;
	virtual void set_render_region(Render_Region &region)   = 0;
	virtual void clear(Clear_Flag flags, Color4 color)      = 0;

	virtual void begin(Handle shader, u8 *data, ptrsize size) = 0;
	virtual void end()                                        = 0;

	virtual void bind_vertex_buffer(Handle buffer)                        = 0;
	virtual void bind_index_buffer(Handle buffer, Render_Index_Type type) = 0;
	virtual void bind_texture(Handle texture, u32 index)                  = 0;
	virtual void draw(ptrsize count, ptrsize offset)                      = 0;
	virtual void draw_indexed(ptrsize count, ptrsize offset)              = 0;

	virtual void destroy() = 0;
};

Gfx_Platform *create_opengl_context(Handle platform, s32 vsync, s32 multisamples, s32 framebuffer_w, s32 framebuffer_h);
