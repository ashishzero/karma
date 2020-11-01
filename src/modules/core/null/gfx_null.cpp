#include "modules/core/gfx_platform.h"

struct Gfx_Platform_Null : public Gfx_Platform {
	Render_Backend backend;
	Gfx_Platform_Info info;
	u32 allocation_size;
	void *allocated_mem;

	virtual void *get_backend_device() { return nullptr; }
	virtual void *get_backend_context() { return nullptr; }

	virtual const Gfx_Platform_Info *get_info() { return &info; }

	virtual void        on_client_resize(u32 w, u32 h) {}
	virtual void        get_render_view_size(u32 *w, u32 *h) {}
	virtual Framebuffer get_default_framebuffer() { Framebuffer f; f.id.hptr = nullptr; return f; }

	virtual u32 get_maximum_supported_multisamples() { return 0; }
	virtual u32 get_multisamples() { return 0; }

	virtual void  set_sync_interval(Vsync vsync) {}
	virtual Vsync get_sync_interval() { return Vsync_0; }

	virtual void present() {}

	virtual Texture2d create_texture2d(u32 w, u32 h, u32 channels, Data_Format format, const u8 **pixels, Buffer_Usage usage, Texture_Bind_Flags flags, u32 mip_levels) { Texture2d t; t.id.hptr = nullptr; return t; }
	virtual void      update_texture2d(Texture2d texture, u32 xoffset, u32 yoffset, u32 w, u32 h, u32 n, u8 *pixels) {}
	virtual void      destroy_texture2d(Texture2d texture) {}

	virtual void generate_mipmaps(Texture_View view) {}

	virtual Texture_View       create_texture_view(Texture2d texture) { Texture_View t; t.id.hptr = nullptr; return t; }
	virtual Depth_Stencil_View create_depth_stencil_view(Texture2d texture) { Depth_Stencil_View t; t.id.hptr = nullptr; return t; }
	virtual Framebuffer        create_framebuffer(u32 count, Texture2d *textures, Texture_View *views, Depth_Stencil_View *depth_view) { Framebuffer f; f.id.hptr = nullptr; return f; }

	virtual void destroy_texture_view(Texture_View view) {}
	virtual void destroy_depth_stencil_view(Depth_Stencil_View view) {}
	virtual void destroy_framebuffer(Framebuffer framebuffer) {}

	virtual Sampler create_sampler(const Filter &filter, const Texture_Address &address, const Level_Of_Detail &lod) { Sampler t; t.id.hptr = nullptr; return t; }
	virtual void    destory_sampler(Sampler sampler) {}

	virtual Vertex_Buffer  create_vertex_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) { 
		if (size > allocation_size) {
			allocation_size = size;
			allocated_mem	= memory_allocate(allocation_size);
		}
		Vertex_Buffer t; 
		t.id.hptr = nullptr; 
		return t; 
	}
	virtual Index_Buffer   create_index_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) {
		if (size > allocation_size) {
			allocation_size = size;
			allocated_mem = memory_allocate(allocation_size);
		}
		Index_Buffer t; 
		t.id.hptr = nullptr;
		return t;
	}
	virtual Uniform_Buffer create_uniform_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) { 
		if (size > allocation_size) {
			allocation_size = size;
			allocated_mem = memory_allocate(allocation_size);
		}
		Uniform_Buffer t; 
		t.id.hptr = nullptr; 
		return t; 
	}

	virtual void destroy_vertex_buffer(Vertex_Buffer buffer) {}
	virtual void destroy_index_buffer(Index_Buffer buffer) {}
	virtual void destroy_uniform_buffer(Uniform_Buffer buffer) {}

	virtual void update_vertex_buffer(Vertex_Buffer buffer, u32 offset, u32 size, void *data) {}
	virtual void update_index_buffer(Index_Buffer buffer, u32 offset, u32 size, void *data) {}
	virtual void update_uniform_buffer(Uniform_Buffer buffer, void *data) {}

	virtual void *map(Vertex_Buffer buffer, Map_Type type) { return allocated_mem; }
	virtual void *map(Index_Buffer buffer, Map_Type type) { return allocated_mem; }
	virtual void *map(Uniform_Buffer buffer, Map_Type type) { return allocated_mem; }
	virtual void  unmap(Vertex_Buffer buffer) {}
	virtual void  unmap(Index_Buffer buffer) {}
	virtual void  unmap(Uniform_Buffer buffer) {}

	virtual Render_Pipeline create_render_pipeline(const Shader_Info &shader,
		const Rasterizer_Info &rasterizer,
		const Blend_Info &blend,
		const Depth_Info &depth,
		const String           name) {
		Render_Pipeline p;
		p.id.hptr = nullptr;
		return p;
	}
	virtual void            destory_render_pipeline(Render_Pipeline pipeline) {}

	virtual void begin_drawing(Framebuffer framebuffer, Clear_Flags flags, Color4 color, r32 depth, u8 stencil) {}
	virtual void end_drawing() {}

	virtual void cmd_set_viewport(r32 x, r32 y, r32 w, r32 h) {}

	virtual void cmd_bind_pipeline(Render_Pipeline pipeline) {}
	virtual void cmd_bind_vertex_buffer(Vertex_Buffer buffer, u32 stride) {}
	virtual void cmd_bind_index_buffer(Index_Buffer buffer, Index_Type type) {}
	virtual void cmd_bind_vs_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) {}
	virtual void cmd_bind_ps_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) {}
	virtual void cmd_bind_textures(u32 slot_index, u32 count, Texture_View *views) {}
	virtual void cmd_bind_samplers(u32 slot_index, u32 count, Sampler *samplers) {}

	virtual void cmd_draw(u32 vertex_count, u32 start_vertex) {}
	virtual void cmd_draw_indexed(u32 index_count, u32 start_index, u32 base_vertex) {}

	virtual void destroy() {
		memory_free(allocated_mem);
		allocation_size = 0;
	}
};

Gfx_Platform *create_null_context(Handle platform, Vsync vsync, s32 multisamples) {
	static Gfx_Platform_Null gfx;
	gfx.backend = Render_Backend_NONE;
	gfx.info.renderer = "Null";
	gfx.info.version = "1";
	gfx.info.vendor = "Unknown";
	gfx.info.shading_lang = "None";
	gfx.allocation_size = mega_bytes(32);
	gfx.allocated_mem = memory_allocate(gfx.allocation_size);
	return &gfx;
}
