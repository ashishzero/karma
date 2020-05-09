#pragma once

#include "gfx_types.h"

struct Gfx_Platform_Info {
	const u8 *vendor;
	const u8 *renderer;
	const u8 *version;
	const u8 *shading_lang_version;
};

struct Gfx_Platform {
	Render_Backend    backend;
	Gfx_Platform_Info info;

	virtual void *get_backend_device()  = 0;
	virtual void *get_backend_context() = 0;

	virtual void               resize_render_view(u32 w, u32 h)     = 0;
	virtual void               get_render_view_size(u32 *w, u32 *h) = 0;
	virtual Render_Target_View get_default_render_target_view()     = 0;

	virtual u32 get_maximum_supported_multisamples() = 0;
	virtual u32 get_multisamples()                   = 0;

	virtual void set_swap_interval(s32 interval) = 0;
	virtual s32  get_swap_interval()             = 0;

	virtual void present() = 0;

	virtual Texture2d create_texture2d(u32 w, u32 h, u32 channels, Data_Format format, const u8 **pixels, Buffer_Usage usage, Texture_Bind_Flags flags, u32 mip_levels) = 0;
	virtual void      update_texture2d(Texture2d texture, u32 xoffset, u32 yoffset, u32 w, u32 h, u32 n, u8 *pixels)                                                    = 0;
	virtual void      destroy_texture2d(Texture2d texture)                                                                                                              = 0;

	virtual void generate_mipmaps(Texture_View view) = 0;

	virtual Texture_View       create_texture_view(Texture2d texture)       = 0;
	virtual Render_Target_View create_render_target_view(Texture2d texture) = 0;
	virtual Depth_Stencil_View create_depth_stencil_view(Texture2d texture) = 0;

	virtual void destroy_texture_view(Texture_View view)             = 0;
	virtual void destroy_render_target_view(Render_Target_View view) = 0;
	virtual void destroy_depth_stencil_view(Depth_Stencil_View view) = 0;

	virtual Sampler create_sampler(const Filter &filer, const Texture_Address &address, const Level_Of_Detail &lod) = 0;
	virtual void    destory_sampler(Sampler sampler)                                                                = 0;

	virtual Vertex_Buffer  create_vertex_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data)  = 0;
	virtual Index_Buffer   create_index_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data)   = 0;
	virtual Uniform_Buffer create_uniform_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) = 0;

	virtual void destroy_vertex_buffer(Vertex_Buffer buffer)   = 0;
	virtual void destroy_index_buffer(Index_Buffer buffer)     = 0;
	virtual void destroy_uniform_buffer(Uniform_Buffer buffer) = 0;

	virtual void update_vertex_buffer(Vertex_Buffer buffer, u32 offset, u32 size, void *data) = 0;
	virtual void update_index_buffer(Index_Buffer buffer, u32 offset, u32 size, void *data)   = 0;
	virtual void update_uniform_buffer(Uniform_Buffer buffer, void *data)                     = 0;

	virtual void *map(Vertex_Buffer buffer, Map_Type type)  = 0;
	virtual void *map(Index_Buffer buffer, Map_Type type)   = 0;
	virtual void *map(Uniform_Buffer buffer, Map_Type type) = 0;
	virtual void  unmap(Vertex_Buffer buffer)               = 0;
	virtual void  unmap(Index_Buffer buffer)                = 0;
	virtual void  unmap(Uniform_Buffer buffer)              = 0;

	virtual Render_Pipeline create_render_pipeline(const Shader_Info &    shader,
												   const Rasterizer_Info &rasterizer,
												   const Blend_Info &     blend,
												   const Depth_Info &     depth,
												   const String           name)         = 0;
	virtual void            destory_render_pipeline(Render_Pipeline pipeline) = 0;

	virtual void begin_drawing(u32                       view_count,
							   Color_Clear_Info *        color_clear_info,
							   Render_Target_View *      render_views,
							   Depth_Stencil_Clear_Info *depth_stencil_clear_info = 0,
							   Depth_Stencil_View *      depth_stencil_view       = 0) = 0;
	virtual void end_drawing()                                             = 0;

	virtual void cmd_set_viewport(r32 x, r32 y, r32 w, r32 h) = 0;

	virtual void cmd_bind_pipeline(Render_Pipeline pipeline)                                    = 0;
	virtual void cmd_bind_vertex_buffer(Vertex_Buffer buffer, u32 offset, u32 stride)           = 0;
	virtual void cmd_bind_index_buffer(Index_Buffer buffer, Index_Type type, u32 offset)        = 0;
	virtual void cmd_bind_vs_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) = 0;
	virtual void cmd_bind_ps_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) = 0;
	virtual void cmd_bind_textures(u32 slot_index, u32 count, Texture_View *views)              = 0;
	virtual void cmd_bind_samplers(u32 slot_index, u32 count, Sampler *samplers)                = 0;

	virtual void cmd_draw(u32 vertex_count, u32 start_vertex)                        = 0;
	virtual void cmd_draw_indexed(u32 index_count, u32 start_index, u32 base_vertex) = 0;

	virtual void destroy() = 0;
};

//Gfx_Platform *create_opengl_context(Handle platform, s32 vsync, s32 multisamples);

Gfx_Platform *create_directx11_context(Handle platform, s32 vsync, s32 multisamples);
