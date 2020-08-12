#include "../systems.h"
#include "../gfx_platform.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#include <d3d11_1.h>
#include <dxgi1_5.h>
#include "dx_error.h"

static constexpr u32 DX_BUFFER_COUNT = 2;

static const DXGI_FORMAT FMT_KARMA_TO_D3D11[] = {
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_SINT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
	DXGI_FORMAT_R10G10B10A2_TYPELESS,
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R10G10B10A2_UINT,
	DXGI_FORMAT_R11G11B10_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R8G8B8A8_SINT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R8G8_UINT,
	DXGI_FORMAT_R8G8_SNORM,
	DXGI_FORMAT_R8G8_SINT,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16_SNORM,
	DXGI_FORMAT_R16_SINT,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R8_SNORM,
	DXGI_FORMAT_R8_SINT,
	DXGI_FORMAT_A8_UNORM,
};
static_assert(static_count(FMT_KARMA_TO_D3D11) == Data_Format_COUNT, "The mapping from Data_Format to DXGI_FORMAT is invalid");

static const D3D11_USAGE BUFF_USAGE_KARMA_TO_D3D11[] = {
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_IMMUTABLE,
	D3D11_USAGE_DYNAMIC,
	D3D11_USAGE_STAGING
};
static_assert(static_count(BUFF_USAGE_KARMA_TO_D3D11) == Buffer_Usage_COUNT, "The mapping from Buffer_Usage to D3D11_USAGE is invalid");

static const D3D11_MAP MAP_TYPE_KARMA_TO_D3D11[] = {
	D3D11_MAP_READ,
	D3D11_MAP_WRITE,
	D3D11_MAP_READ_WRITE,
	D3D11_MAP_WRITE_DISCARD
};
static_assert(static_count(MAP_TYPE_KARMA_TO_D3D11) == Map_Type_COUNT, "The mapping from Map_Type to D3D11_MAP is invalid");

static const D3D11_FILTER TEX_FILTER_TYPE_KARMA_TO_D3D11[] = {
	D3D11_FILTER_MIN_MAG_MIP_POINT,
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	D3D11_FILTER_ANISOTROPIC,
	D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
	D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
	D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
	D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
	D3D11_FILTER_COMPARISON_ANISOTROPIC,
	D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,
	D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
	D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
	D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR,
	D3D11_FILTER_MINIMUM_ANISOTROPIC,
	D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT,
	D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
	D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
	D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,
	D3D11_FILTER_MAXIMUM_ANISOTROPIC
};
static_assert(static_count(TEX_FILTER_TYPE_KARMA_TO_D3D11) == Filter_Type_COUNT, "The mapping from Filter_Type to D3D11_FILTER is invalid");

static const D3D11_TEXTURE_ADDRESS_MODE TEX_ADDRESS_MODE_KARMA_TO_D3D11[] = {
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_BORDER,
	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};
static_assert(static_count(TEX_ADDRESS_MODE_KARMA_TO_D3D11) == Texture_Address_Mode_COUNT, "The mapping from Texture_Address_Mode to D3D11_TEXTURE_ADDRESS_MODE is invalid");

static const D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_KARMA_TO_D3D11[] = {
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};
static_assert(static_count(PRIMITIVE_KARMA_TO_D3D11) == Primitive_COUNT, "The mapping from Primitive to D3D11_PRIMITIVE_TOPOLOGY is invalid");

static const D3D11_COMPARISON_FUNC COMPARISON_FUNC_KARMA_TO_D3D11[] = {
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};
static_assert(static_count(COMPARISON_FUNC_KARMA_TO_D3D11) == Comparison_Function_COUNT, "The mapping from Comparison_Function to D3D11_COMPARISON_FUNC is invalid");

static const D3D11_FILL_MODE FILL_MODE_KARMA_TO_D3D11[] = {
	D3D11_FILL_WIREFRAME,
	D3D11_FILL_SOLID
};
static_assert(static_count(FILL_MODE_KARMA_TO_D3D11) == Fill_Mode_COUNT, "The mapping from Fill_Mode to D3D11_FILL_MODE is invalid");

static const D3D11_CULL_MODE CULL_MODE_KARMA_TO_D3D11[] = {
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};
static_assert(static_count(CULL_MODE_KARMA_TO_D3D11) == Cull_Mode_COUNT, "The mapping from Cull_Mode to D3D11_CULL_MODE is invalid");

static const BOOL FRONT_FACE_KARMA_TO_D3D11[] = {
	FALSE,
	TRUE
};
static_assert(static_count(FRONT_FACE_KARMA_TO_D3D11) == Front_Face_COUNT, "The mapping from Front_Face to BOOL is invalid");

static const D3D11_BLEND BLEND_KARMA_TO_D3D11[] = {
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_SRC1_COLOR,
	D3D11_BLEND_INV_SRC1_COLOR,
	D3D11_BLEND_SRC1_ALPHA,
	D3D11_BLEND_INV_SRC1_ALPHA
};
static_assert(static_count(BLEND_KARMA_TO_D3D11) == Blend_COUNT, "The mapping from Blend to D3D11_BLEND is invalid");

static const D3D11_BLEND_OP BLEND_OP_KARMA_TO_D3D11[] = {
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};
static_assert(static_count(BLEND_OP_KARMA_TO_D3D11) == Blend_Operation_COUNT, "The mapping from Blend_Operation to D3D11_BLEND_OP is invalid");

static const D3D11_DEPTH_WRITE_MASK DEPTH_WRITE_MASK_KARMA_TO_D3D11[] = {
	D3D11_DEPTH_WRITE_MASK_ZERO,
	D3D11_DEPTH_WRITE_MASK_ALL
};
static_assert(static_count(DEPTH_WRITE_MASK_KARMA_TO_D3D11) == Depth_Write_Mask_COUNT, "The mapping from Depth_Write_Mask to D3D11_DEPTH_WRITE_MASK is invalid");

static const D3D11_INPUT_CLASSIFICATION INPUT_TYPE_KARMA_TO_D3D11[] = {
	D3D11_INPUT_PER_VERTEX_DATA,
	D3D11_INPUT_PER_INSTANCE_DATA
};
static_assert(static_count(INPUT_TYPE_KARMA_TO_D3D11) == Input_Type_COUNT, "The mapping from Input_Type to D3D11_INPUT_CLASSIFICATION is invalid");

static const DXGI_FORMAT INDEX_FMT_KARMA_TO_D3D11[] = {
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT
};
static_assert(static_count(INDEX_FMT_KARMA_TO_D3D11) == Index_Type_COUNT, "The mapping from Index_Type to DXGI_FORMAT is invalid");

struct Internal_Framebuffer {
	u32                      count;
	ID3D11RenderTargetView **views;
	ID3D11DepthStencilView * depth_stencil;
};

struct Internal_Pipeline {
	ID3D11InputLayout *      input_layout        = 0;
	ID3D11VertexShader *     vertex_shader       = 0;
	ID3D11PixelShader *      pixel_shader        = 0;
	D3D11_PRIMITIVE_TOPOLOGY primitive_topology  = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	ID3D11RasterizerState *  rasterizer_state    = 0;
	ID3D11BlendState *       blend_state         = 0;
	ID3D11DepthStencilState *depth_stencil_state = 0;
};

struct Gfx_Platform_Directx11 : public Gfx_Platform {
	ID3D11Device *       device;
	ID3D11DeviceContext *device_context;
	IDXGISwapChain1 *    swap_chain;
	u32                  swap_chain_flags;

	u32  sync_interval;
	u32  sync_flags;
	bool tearing_capable;

	// Resources
	ID3D11Texture2D *       render_target_texture;
	ID3D11RenderTargetView *render_target_view;
	Internal_Framebuffer    default_framebuffer;

	Gfx_Platform_Info info;

	void create_resources() {
		HRESULT hresult;

		hresult = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&render_target_texture));
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirectX11", "SwapChain::GetBuffer failed: %s", dx_error_string(hresult));
		}

		hresult = device->CreateRenderTargetView(render_target_texture, 0, &render_target_view);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirectX11", "Failed to create default Render Target View: %s", dx_error_string(hresult));
		}

		default_framebuffer.count         = 1;
		default_framebuffer.views         = &render_target_view;
		default_framebuffer.depth_stencil = NULL;
	}

	void destory_resources() {
		render_target_view->Release();
		render_target_texture->Release();
		device_context->ClearState();
		device_context->Flush();
	}

	virtual void *get_backend_device() final override {
		return device;
	}

	virtual void *get_backend_context() final override {
		return device_context;
	}

	virtual const Gfx_Platform_Info *get_info() final override {
		return &info;
	}

	virtual u32 get_maximum_supported_multisamples() final override {
		return D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	}

	virtual u32 get_multisamples() final override {
		DXGI_SWAP_CHAIN_DESC1 desc;
		swap_chain->GetDesc1(&desc);
		return desc.SampleDesc.Count;
	}

	virtual void set_sync_interval(Vsync vsync) final override {
		if (vsync == Vsync_ADAPTIVE) {
			if (tearing_capable)
				sync_flags = DXGI_PRESENT_ALLOW_TEARING;
			sync_interval = 1;
		} else {
			sync_interval = vsync;
		}
	}

	virtual Vsync get_sync_interval() final override {
		if (tearing_capable && (sync_flags & DXGI_PRESENT_ALLOW_TEARING) == DXGI_PRESENT_ALLOW_TEARING) {
			return Vsync_ADAPTIVE;
		}
		return (Vsync)sync_interval;
	}

	virtual void on_client_resize(u32 w, u32 h) final override {
		if (w > 0 && h > 0) {
			destory_resources();
			swap_chain->ResizeBuffers(DX_BUFFER_COUNT, 0, 0, DXGI_FORMAT_UNKNOWN, swap_chain_flags);
			create_resources();
		}
	}

	virtual void get_render_view_size(u32 *w, u32 *h) final override {
		DXGI_SWAP_CHAIN_DESC1 desc;
		swap_chain->GetDesc1(&desc);
		*w = desc.Width;
		*h = desc.Height;
	}

	virtual Framebuffer get_default_framebuffer() final override {
		Framebuffer result;
		result.id.hptr = &default_framebuffer;
		return result;
	}

	virtual void present() final override {
		swap_chain->Present(sync_interval, sync_flags);
	}

	virtual Texture2d create_texture2d(u32 w, u32 h, u32 channels, Data_Format format, const u8 **pixels, Buffer_Usage usage, Texture_Bind_Flags flags, u32 mip_levels) final override {
		D3D11_TEXTURE2D_DESC desc;
		desc.Width              = w;
		desc.Height             = h;
		desc.MipLevels          = mip_levels;
		desc.ArraySize          = 1;
		desc.Format             = FMT_KARMA_TO_D3D11[format];
		desc.SampleDesc.Count   = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage              = BUFF_USAGE_KARMA_TO_D3D11[usage];
		desc.BindFlags          = 0;

		if (flags & Texture_Bind_SHADER_RESOURCE) desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		if (flags & Texture_Bind_RENDER_TARGET) desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		if (flags & Texture_Bind_DEPTH_STENCIL) desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;

		u32 can_use_mipmaps = (flags & (Texture_Bind_SHADER_RESOURCE | Texture_Bind_RENDER_TARGET)) == (Texture_Bind_SHADER_RESOURCE | Texture_Bind_RENDER_TARGET);

		desc.CPUAccessFlags = 0;
		desc.MiscFlags      = (mip_levels != 1 && can_use_mipmaps) ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		u32 mip_counts = mip_levels;
		if (mip_counts == 0) {
			mip_counts = (u32)min_value(log2(w), log2(h)) + 1;
		}

		D3D11_SUBRESOURCE_DATA *initial_data = NULL;

		scoped_temporary_allocation();
		if (pixels) {
			initial_data = (D3D11_SUBRESOURCE_DATA *)tallocate(sizeof(D3D11_SUBRESOURCE_DATA) * mip_counts);
			for (u32 m = 0; m < mip_counts; ++m) {
				initial_data[m].pSysMem          = pixels[m];
				initial_data[m].SysMemPitch      = (w >> m) * channels;
				initial_data[m].SysMemSlicePitch = 0;
			}
		}

		ID3D11Texture2D *texture = 0;
		HRESULT          hresult = device->CreateTexture2D(&desc, initial_data, &texture);

		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirectX11", "Failed to create Texture2D: %s", dx_error_string(hresult));
		}

		Texture2d result;
		result.id.hptr = texture;
		return result;
	}

	virtual void update_texture2d(Texture2d texture, u32 xoffset, u32 yoffset, u32 w, u32 h, u32 n, u8 *pixels) final override {
		ID3D11Resource *resource = (ID3D11Resource *)texture.id.hptr;
		D3D11_BOX       box;
		box.left   = xoffset;
		box.right  = xoffset + w;
		box.top    = yoffset;
		box.bottom = yoffset + h;
		box.front  = 0;
		box.back   = 1;
		device_context->UpdateSubresource(resource, D3D11CalcSubresource(0, 0, 0), &box, pixels, w * n, 0);
	}

	virtual void destroy_texture2d(Texture2d texture) final override {
		ID3D11Texture2D *ptr = (ID3D11Texture2D *)texture.id.hptr;
		ptr->Release();
	}

	virtual void generate_mipmaps(Texture_View view) final override {
		ID3D11ShaderResourceView *ptr = (ID3D11ShaderResourceView *)view.id.hptr;
		device_context->GenerateMips(ptr);
	}

	virtual Texture_View create_texture_view(Texture2d texture) final override {
		ID3D11Resource *          resource = (ID3D11Resource *)texture.id.hptr;
		ID3D11ShaderResourceView *view     = 0;
		device->CreateShaderResourceView(resource, NULL, &view);
		Texture_View result;
		result.id.hptr = view;
		return result;
	}

	virtual Depth_Stencil_View create_depth_stencil_view(Texture2d texture) final override {
		ID3D11Resource *        resource = (ID3D11Resource *)texture.id.hptr;
		ID3D11DepthStencilView *view     = 0;
		device->CreateDepthStencilView(resource, NULL, &view);
		Depth_Stencil_View result;
		result.id.hptr = view;
		return result;
	}

	virtual Framebuffer create_framebuffer(u32 count, Texture2d *textures, Texture_View *views, Depth_Stencil_View *depth_view) final override {
		void *                mem         = memory_allocate(sizeof(Internal_Framebuffer) + (sizeof(ID3D11RenderTargetView *) * count));
		Internal_Framebuffer *framebuffer = (Internal_Framebuffer *)mem;
		framebuffer->views                = (ID3D11RenderTargetView **)((u8 *)mem + sizeof(Internal_Framebuffer));

		framebuffer->count         = count;
		framebuffer->depth_stencil = depth_view ? (ID3D11DepthStencilView *)depth_view->id.hptr : NULL;

		ID3D11RenderTargetView *render_target = nullptr;
		for (u32 index = 0; index < count; ++index) {
			ID3D11Resource *resource = (ID3D11Resource *)textures[index].id.hptr;
			device->CreateRenderTargetView(resource, NULL, &render_target);
			framebuffer->views[index] = render_target;
		}

		Framebuffer result;
		result.id.hptr = framebuffer;

		return result;
	}

	virtual void destroy_texture_view(Texture_View view) final override {
		ID3D11ShaderResourceView *ptr = (ID3D11ShaderResourceView *)view.id.hptr;
		ptr->Release();
	}

	virtual void destroy_depth_stencil_view(Depth_Stencil_View view) final override {
		ID3D11DepthStencilView *ptr = (ID3D11DepthStencilView *)view.id.hptr;
		ptr->Release();
	}

	virtual void destroy_framebuffer(Framebuffer handle) final override {
		Internal_Framebuffer *framebuffer = (Internal_Framebuffer *)handle.id.hptr;
		for (u32 index = 0; index < framebuffer->count; ++index) {
			framebuffer->views[index]->Release();
		}
		memory_free(framebuffer);
	}

	virtual Sampler create_sampler(const Filter &filter, const Texture_Address &address, const Level_Of_Detail &lod) final override {
		D3D11_SAMPLER_DESC desc;
		desc.Filter         = TEX_FILTER_TYPE_KARMA_TO_D3D11[filter.type];
		desc.AddressU       = TEX_ADDRESS_MODE_KARMA_TO_D3D11[address.u];
		desc.AddressV       = TEX_ADDRESS_MODE_KARMA_TO_D3D11[address.v];
		desc.AddressW       = TEX_ADDRESS_MODE_KARMA_TO_D3D11[address.w];
		desc.MipLODBias     = lod.bias;
		desc.MaxAnisotropy  = filter.max_anisotropy;
		desc.ComparisonFunc = COMPARISON_FUNC_KARMA_TO_D3D11[filter.comparison];
		desc.BorderColor[0] = address.border_color.x;
		desc.BorderColor[1] = address.border_color.y;
		desc.BorderColor[2] = address.border_color.z;
		desc.BorderColor[3] = address.border_color.w;
		desc.MinLOD         = lod.min;
		desc.MaxLOD         = lod.max;

		ID3D11SamplerState *sampler = 0;
		device->CreateSamplerState(&desc, &sampler);

		Sampler result;
		result.id.hptr = sampler;
		return result;
	}

	virtual void destory_sampler(Sampler sampler) final override {
		ID3D11SamplerState *ptr = (ID3D11SamplerState *)sampler.id.hptr;
		ptr->Release();
	}

	ID3D11Buffer *create_buffer(UINT bind_flags, Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) {
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth         = size;
		desc.Usage             = BUFF_USAGE_KARMA_TO_D3D11[usage];
		desc.BindFlags         = bind_flags;

		if (flags & Cpu_Access_READ) desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
		if (flags & Cpu_Access_WRITE) desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initial_data;
		initial_data.pSysMem          = data;
		initial_data.SysMemPitch      = 0;
		initial_data.SysMemSlicePitch = 0;

		ID3D11Buffer *buffer  = 0;
		HRESULT       hresult = device->CreateBuffer(&desc, data ? &initial_data : NULL, &buffer);

		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirectX11", "Failed to allocate buffer: %s", dx_error_string(hresult));
		}

		return buffer;
	}

	virtual Vertex_Buffer create_vertex_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) final override {
		Vertex_Buffer result;
		result.id.hptr = create_buffer(D3D11_BIND_VERTEX_BUFFER, usage, flags, size, data);
		return result;
	}

	virtual Index_Buffer create_index_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) final override {
		Index_Buffer result;
		result.id.hptr = create_buffer(D3D11_BIND_INDEX_BUFFER, usage, flags, size, data);
		return result;
	}

	virtual Uniform_Buffer create_uniform_buffer(Buffer_Usage usage, Cpu_Access_Flags flags, u32 size, void *data) final override {
		Uniform_Buffer result;
		result.id.hptr = create_buffer(D3D11_BIND_CONSTANT_BUFFER, usage, flags, size, data);
		return result;
	}

	virtual void destroy_vertex_buffer(Vertex_Buffer buffer) final override {
		ID3D11Buffer *ptr = (ID3D11Buffer *)buffer.id.hptr;
		ptr->Release();
	}

	virtual void destroy_index_buffer(Index_Buffer buffer) final override {
		ID3D11Buffer *ptr = (ID3D11Buffer *)buffer.id.hptr;
		ptr->Release();
	}

	virtual void destroy_uniform_buffer(Uniform_Buffer buffer) final override {
		ID3D11Buffer *ptr = (ID3D11Buffer *)buffer.id.hptr;
		ptr->Release();
	}

	virtual void update_vertex_buffer(Vertex_Buffer buffer, u32 offset, u32 size, void *data) final override {
		ID3D11Resource *resource = (ID3D11Resource *)buffer.id.hptr;
		D3D11_BOX       box;
		box.left   = offset;
		box.right  = offset + size;
		box.top    = 0;
		box.bottom = 1;
		box.front  = 0;
		box.back   = 1;
		device_context->UpdateSubresource(resource, 0, &box, data, 0, 0);
	}

	virtual void update_index_buffer(Index_Buffer buffer, u32 offset, u32 size, void *data) final override {
		ID3D11Resource *resource = (ID3D11Resource *)buffer.id.hptr;
		D3D11_BOX       box;
		box.left   = offset;
		box.right  = offset + size;
		box.top    = 0;
		box.bottom = 1;
		box.front  = 0;
		box.back   = 1;
		device_context->UpdateSubresource(resource, 0, &box, data, 0, 0);
	}

	virtual void update_uniform_buffer(Uniform_Buffer buffer, void *data) final override {
		ID3D11Resource *resource = (ID3D11Resource *)buffer.id.hptr;
		device_context->UpdateSubresource(resource, 0, NULL, data, 0, 0);
	}

	virtual void *map(Vertex_Buffer buffer, Map_Type type) final override {
		ID3D11Resource *         resource = (ID3D11Resource *)buffer.id.hptr;
		D3D11_MAPPED_SUBRESOURCE mapped   = {};
		device_context->Map(resource, 0, MAP_TYPE_KARMA_TO_D3D11[type], 0, &mapped);
		return mapped.pData;
	}

	virtual void *map(Index_Buffer buffer, Map_Type type) final override {
		ID3D11Resource *         resource = (ID3D11Resource *)buffer.id.hptr;
		D3D11_MAPPED_SUBRESOURCE mapped   = {};
		device_context->Map(resource, 0, MAP_TYPE_KARMA_TO_D3D11[type], 0, &mapped);
		return mapped.pData;
	}

	virtual void *map(Uniform_Buffer buffer, Map_Type type) final override {
		ID3D11Resource *         resource = (ID3D11Resource *)buffer.id.hptr;
		D3D11_MAPPED_SUBRESOURCE mapped   = {};
		device_context->Map(resource, 0, MAP_TYPE_KARMA_TO_D3D11[type], 0, &mapped);
		return mapped.pData;
	}

	virtual void unmap(Vertex_Buffer buffer) final override {
		ID3D11Resource *resource = (ID3D11Resource *)buffer.id.hptr;
		device_context->Unmap(resource, 0);
	}

	virtual void unmap(Index_Buffer buffer) final override {
		ID3D11Resource *resource = (ID3D11Resource *)buffer.id.hptr;
		device_context->Unmap(resource, 0);
	}

	virtual void unmap(Uniform_Buffer buffer) final override {
		ID3D11Resource *resource = (ID3D11Resource *)buffer.id.hptr;
		device_context->Unmap(resource, 0);
	}

	virtual Render_Pipeline create_render_pipeline(const Shader_Info &    shader_info,
												   const Rasterizer_Info &rasterizer_info,
												   const Blend_Info &     blend_info,
												   const Depth_Info &     depth_info,
												   const String           name) final override {
		Render_Pipeline    result   = {};
		Internal_Pipeline *pipeline = (Internal_Pipeline *)memory_allocate(sizeof(Internal_Pipeline));

		HRESULT hresult;

		pipeline->primitive_topology = PRIMITIVE_KARMA_TO_D3D11[rasterizer_info.primitive];

		D3D11_RASTERIZER_DESC rasterizer_desc;
		rasterizer_desc.FillMode              = FILL_MODE_KARMA_TO_D3D11[rasterizer_info.fill_mode];
		rasterizer_desc.CullMode              = CULL_MODE_KARMA_TO_D3D11[rasterizer_info.cull_mode];
		rasterizer_desc.FrontCounterClockwise = FRONT_FACE_KARMA_TO_D3D11[rasterizer_info.front_face];
		rasterizer_desc.DepthBias             = rasterizer_info.depth_bias;
		rasterizer_desc.DepthBiasClamp        = rasterizer_info.depth_bias_clamp;
		rasterizer_desc.SlopeScaledDepthBias  = 0;
		rasterizer_desc.DepthClipEnable       = rasterizer_info.depth_clip;
		rasterizer_desc.ScissorEnable         = rasterizer_info.scissor_enable;
		rasterizer_desc.MultisampleEnable     = rasterizer_info.multisample;
		rasterizer_desc.AntialiasedLineEnable = false;

		hresult = device->CreateRasterizerState(&rasterizer_desc, &pipeline->rasterizer_state);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirextX11", "Failed to create Rasterizer State for %s pipeline", tto_cstring(name));
			delete pipeline;
			return result;
		}

		D3D11_BLEND_DESC blend_desc                      = {};
		blend_desc.AlphaToCoverageEnable                 = FALSE;
		blend_desc.IndependentBlendEnable                = FALSE;
		blend_desc.RenderTarget[0].BlendEnable           = blend_info.enable;
		blend_desc.RenderTarget[0].SrcBlend              = BLEND_KARMA_TO_D3D11[blend_info.src];
		blend_desc.RenderTarget[0].DestBlend             = BLEND_KARMA_TO_D3D11[blend_info.dst];
		blend_desc.RenderTarget[0].BlendOp               = BLEND_OP_KARMA_TO_D3D11[blend_info.op];
		blend_desc.RenderTarget[0].SrcBlendAlpha         = BLEND_KARMA_TO_D3D11[blend_info.src_alpha];
		blend_desc.RenderTarget[0].DestBlendAlpha        = BLEND_KARMA_TO_D3D11[blend_info.dst_alpha];
		blend_desc.RenderTarget[0].BlendOpAlpha          = BLEND_OP_KARMA_TO_D3D11[blend_info.op_alpha];
		blend_desc.RenderTarget[0].RenderTargetWriteMask = 0;
		if (blend_info.mask == Blend_Mask_WRITE_ALL) {
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		} else {
			if (blend_info.mask & Blend_Mask_R) blend_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
			if (blend_info.mask & Blend_Mask_G) blend_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
			if (blend_info.mask & Blend_Mask_B) blend_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
			if (blend_info.mask & Blend_Mask_A) blend_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
		}

		hresult = device->CreateBlendState(&blend_desc, &pipeline->blend_state);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirextX11", "Failed to create Blend State for %s pipeline", tto_cstring(name));
			delete pipeline;
			return result;
		}

		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = {};
		depth_stencil_desc.DepthEnable              = depth_info.test_enable;
		depth_stencil_desc.DepthWriteMask           = DEPTH_WRITE_MASK_KARMA_TO_D3D11[depth_info.write_mask];
		depth_stencil_desc.DepthFunc                = COMPARISON_FUNC_KARMA_TO_D3D11[depth_info.compare];

		hresult = device->CreateDepthStencilState(&depth_stencil_desc, &pipeline->depth_stencil_state);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirextX11", "Failed to create Blend State for %s pipeline", tto_cstring(name));
			delete pipeline;
			return result;
		}

		D3D11_INPUT_ELEMENT_DESC *input_element_desc = (D3D11_INPUT_ELEMENT_DESC *)tallocate(sizeof(D3D11_INPUT_ELEMENT_DESC) * shader_info.input_layouts_count);
		for (u32 input_elem_index = 0; input_elem_index < shader_info.input_layouts_count; ++input_elem_index) {
			auto &src                = shader_info.input_layouts[input_elem_index];
			auto &dst                = input_element_desc[input_elem_index];
			dst.SemanticName         = src.name;
			dst.SemanticIndex        = 0;
			dst.Format               = FMT_KARMA_TO_D3D11[src.format];
			dst.InputSlot            = 0;
			dst.AlignedByteOffset    = src.offset;
			dst.InputSlotClass       = INPUT_TYPE_KARMA_TO_D3D11[src.input_type];
			dst.InstanceDataStepRate = src.instance_data_step_rate;
		}

		hresult = device->CreateInputLayout(input_element_desc, shader_info.input_layouts_count, shader_info.vertex.data, shader_info.vertex.count, &pipeline->input_layout);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirextX11", "Failed to create Input Layout for %s pipeline", tto_cstring(name));
			delete pipeline;
			return result;
		}

		hresult = device->CreateVertexShader(shader_info.vertex.data, shader_info.vertex.count, NULL, &pipeline->vertex_shader);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirextX11", "Failed to create Vertex Shader for %s pipeline", tto_cstring(name));
			delete pipeline;
			return result;
		}

		hresult = device->CreatePixelShader(shader_info.pixel.data, shader_info.pixel.count, NULL, &pipeline->pixel_shader);
		if (FAILED(hresult)) {
			system_log(LOG_ERROR, "DirextX11", "Failed to create Pixel Shader for %s pipeline", tto_cstring(name));
			delete pipeline;
			return result;
		}

		result.id.hptr = pipeline;
		return result;
	}

	virtual void destory_render_pipeline(Render_Pipeline pipeline) final override {
		Internal_Pipeline *ptr = (Internal_Pipeline *)pipeline.id.hptr;
		ptr->input_layout->Release();
		ptr->vertex_shader->Release();
		ptr->pixel_shader->Release();
		ptr->rasterizer_state->Release();
		ptr->blend_state->Release();
		ptr->depth_stencil_state->Release();

		memory_free(ptr);
	}

	virtual void begin_drawing(Framebuffer handle, Clear_Flags flags, Color4 color, r32 depth, u8 stencil) final override {
		Internal_Framebuffer *framebuffer = (Internal_Framebuffer *)handle.id.hptr;

		if ((flags & Clear_COLOR) == Clear_COLOR) {
			for (u32 view_index = 0; view_index < framebuffer->count; ++view_index) {
				device_context->ClearRenderTargetView(framebuffer->views[view_index], color.m);
			}
		}

		if (framebuffer->depth_stencil) {
			UINT d3d11_flags = 0;
			if ((flags & Clear_DEPTH) == Clear_DEPTH) d3d11_flags |= D3D11_CLEAR_DEPTH;
			if ((flags & Clear_STENCIL) == Clear_STENCIL) d3d11_flags |= D3D11_CLEAR_STENCIL;
			device_context->ClearDepthStencilView(framebuffer->depth_stencil, d3d11_flags, depth, stencil);
		}

		device_context->OMSetRenderTargets(framebuffer->count, framebuffer->views, framebuffer->depth_stencil);
	}

	virtual void end_drawing() final override {
		device_context->ClearState();
	}

	virtual void cmd_set_viewport(r32 x, r32 y, r32 w, r32 h) final override {
		D3D11_VIEWPORT d3d11_viewport;
		d3d11_viewport.TopLeftX = x;
		d3d11_viewport.TopLeftY = y;
		d3d11_viewport.Width    = w;
		d3d11_viewport.Height   = h;
		d3d11_viewport.MinDepth = 0;
		d3d11_viewport.MaxDepth = 1;

		device_context->RSSetViewports(1, &d3d11_viewport);
	}

	virtual void cmd_bind_pipeline(Render_Pipeline pipeline) final override {
		Internal_Pipeline *ptr = (Internal_Pipeline *)pipeline.id.hptr;

		device_context->IASetPrimitiveTopology(ptr->primitive_topology);
		device_context->IASetInputLayout(ptr->input_layout);

		device_context->VSSetShader(ptr->vertex_shader, NULL, 0);

		device_context->RSSetState(ptr->rasterizer_state);

		device_context->PSSetShader(ptr->pixel_shader, NULL, 0);

		device_context->OMSetDepthStencilState(ptr->depth_stencil_state, 0);
		device_context->OMSetBlendState(ptr->blend_state, 0, 0xffffffff);
	}

	virtual void cmd_bind_vertex_buffer(Vertex_Buffer buffer, u32 stride) final override {
		UINT          offset              = 0;
		ID3D11Buffer *d3d11_vertex_buffer = (ID3D11Buffer *)buffer.id.hptr;
		device_context->IASetVertexBuffers(0, 1, &d3d11_vertex_buffer, &stride, &offset);
	}

	virtual void cmd_bind_index_buffer(Index_Buffer buffer, Index_Type type) final override {
		UINT offset = 0;
		device_context->IASetIndexBuffer((ID3D11Buffer *)buffer.id.hptr, INDEX_FMT_KARMA_TO_D3D11[type], offset);
	}

	virtual void cmd_bind_vs_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) final override {
		device_context->VSSetConstantBuffers(slot_index, count, (ID3D11Buffer **)buffer);
	}

	virtual void cmd_bind_ps_uniform_buffers(u32 slot_index, u32 count, Uniform_Buffer *buffer) final override {
		device_context->PSSetConstantBuffers(slot_index, count, (ID3D11Buffer **)buffer);
	}

	virtual void cmd_bind_textures(u32 slot_index, u32 count, Texture_View *views) final override {
		device_context->PSSetShaderResources(slot_index, count, (ID3D11ShaderResourceView **)views);
	}

	virtual void cmd_bind_samplers(u32 slot_index, u32 count, Sampler *samplers) final override {
		device_context->PSSetSamplers(slot_index, count, (ID3D11SamplerState **)samplers);
	}

	virtual void cmd_draw(u32 vertex_count, u32 start_vertex) final override {
		device_context->Draw(vertex_count, start_vertex);
	}

	virtual void cmd_draw_indexed(u32 index_count, u32 start_index, u32 base_vertex) final override {
		device_context->DrawIndexed(index_count, start_index, base_vertex);
	}

	virtual void destroy() final override {
		destory_resources();
		swap_chain->Release();
		device_context->Release();
		device->Release();
	}
};

void directx_error_cleanup(Gfx_Platform_Directx11 &gfx) {
	if (gfx.swap_chain) gfx.swap_chain->Release();
	if (gfx.device_context) gfx.device_context->Release();
	if (gfx.device) gfx.device->Release();
}

#define dx_error_check(res, gfx)                                                                       \
	assert(res != E_INVALIDARG);                                                                       \
	if (res != S_OK) {                                                                                 \
		system_log(LOG_WARNING, "DirectX11", "CreateDXGIFactory1() failed: %s", dx_error_string(res)); \
		directx_error_cleanup(gfx);                                                                    \
		return 0;                                                                                      \
	}

Gfx_Platform *create_directx11_context(Handle platform, Vsync vsync, s32 multisamples) {
	static Gfx_Platform_Directx11 gfx;

	gfx.backend = Render_Backend_NONE;

	IDXGIFactory2 *factory;

	u32 factory_flags = 0;
#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
	factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	dx_error_check(CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory)), gfx);
	defer {
		factory->Release();
	};

	u32 flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	IDXGIAdapter1 *adapter              = 0;
	ptrsize        max_dedicated_memory = 0;
	IDXGIAdapter1 *adapter_selection    = 0;

	for (u32 adapter_index = 0; factory->EnumAdapters1(adapter_index, &adapter_selection) != DXGI_ERROR_NOT_FOUND; ++adapter_index) {
		DXGI_ADAPTER_DESC1 desc;
		adapter_selection->GetDesc1(&desc);

		auto device_creation_result = D3D11CreateDevice(adapter_selection,
														D3D_DRIVER_TYPE_UNKNOWN,
														0, flags,
														feature_levels, static_count(feature_levels),
														D3D11_SDK_VERSION, NULL, NULL, NULL);

		if (SUCCEEDED(device_creation_result) && desc.DedicatedVideoMemory > max_dedicated_memory) {
			max_dedicated_memory = desc.DedicatedVideoMemory;
			adapter              = adapter_selection;
		} else {
			adapter_selection->Release();
		}
	}

	if (adapter == 0) {
		system_log(LOG_WARNING, "DirectX11", "Supported adapter not found!");
		return 0;
	}

	defer {
		adapter->Release();
	};

	HRESULT hresult;

	D3D_FEATURE_LEVEL feature;
	hresult = D3D11CreateDevice(adapter,
								D3D_DRIVER_TYPE_UNKNOWN,
								NULL, flags,
								feature_levels, static_count(feature_levels),
								D3D11_SDK_VERSION, &gfx.device, &feature, &gfx.device_context);
	dx_error_check(hresult, gfx);

	gfx.swap_chain_flags = 0;
	gfx.tearing_capable  = false;

	IDXGIFactory5 *factory5;
	if (SUCCEEDED(factory->GetParent(__uuidof(IDXGIFactory5), reinterpret_cast<void **>(&factory5)))) {
		BOOL tearing = FALSE;
		if (SUCCEEDED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &tearing, sizeof(tearing)))) {
			if (tearing == TRUE) {
				gfx.swap_chain_flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
				gfx.tearing_capable = true;
			}
		}
	}

	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
	swap_chain_desc.BufferCount           = DX_BUFFER_COUNT;
	swap_chain_desc.Width                 = 0;
	swap_chain_desc.Height                = 0;
	swap_chain_desc.Format                = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	swap_chain_desc.Stereo                = FALSE;
	swap_chain_desc.SampleDesc.Count      = multisamples;
	swap_chain_desc.SampleDesc.Quality    = 0; // ID3D11Device::CheckMultisampleQualityLevels
	swap_chain_desc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.Scaling               = DXGI_SCALING_STRETCH;
	swap_chain_desc.SwapEffect            = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags                 = gfx.swap_chain_flags;
	swap_chain_desc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;

	hresult = factory->CreateSwapChainForHwnd(gfx.device, (HWND)platform.hptr, &swap_chain_desc, NULL, NULL, &gfx.swap_chain);
	dx_error_check(hresult, gfx);

#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST)
	ID3D11Debug *debug = 0;

	if (SUCCEEDED(gfx.device->QueryInterface(__uuidof(ID3D11Debug), (void **)&debug))) {
		ID3D11InfoQueue *info_queue = nullptr;
		if (SUCCEEDED(gfx.device->QueryInterface(__uuidof(ID3D11InfoQueue), (void **)&info_queue))) {
			system_log(LOG_INFO, "DirectX", "GL_DEBUG_OUTPUT_SYNCHRONOUS enabled.");

			info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

			D3D11_MESSAGE_ID hide[] = {
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
			};

			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs         = static_count(hide);
			filter.DenyList.pIDList        = hide;
			info_queue->AddStorageFilterEntries(&filter);

			info_queue->Release();
		}
	}
	debug->Release();
#endif

	gfx.create_resources();

	gfx.sync_flags = 0;
	if (vsync == Vsync_ADAPTIVE) {
		gfx.sync_interval = 1;
		if (gfx.tearing_capable) {
			gfx.sync_flags |= DXGI_PRESENT_ALLOW_TEARING;
		}
	} else {
		gfx.sync_interval = vsync;
	}

	gfx.backend = Render_Backend_DIRECTX11;

	DXGI_ADAPTER_DESC adapter_desc = {};
	adapter->GetDesc(&adapter_desc);

	gfx.info.vendor = (char *)HeapAlloc(GetProcessHeap(), 0, 128);
	WideCharToMultiByte(CP_UTF8, 0, adapter_desc.Description, -1, (char *)gfx.info.vendor, 128, 0, 0);
	gfx.info.renderer = "Direct X";

	switch (feature) {
		case D3D_FEATURE_LEVEL_9_1:
			gfx.info.version      = "Feature Level 9.1";
			gfx.info.shading_lang = "HLSL Shader Model 2.x";
			break;
		case D3D_FEATURE_LEVEL_9_2:
			gfx.info.version      = "Feature Level 9.2";
			gfx.info.shading_lang = "HLSL Shader Model 2.x";
			break;
		case D3D_FEATURE_LEVEL_9_3:
			gfx.info.version      = "Feature Level 9.3";
			gfx.info.shading_lang = "HLSL Shader Model 2.x";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			gfx.info.version      = "Feature Level 10.0";
			gfx.info.shading_lang = "HLSL 4.0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			gfx.info.version      = "Feature Level 10.1";
			gfx.info.shading_lang = "HLSL 4.1";
			break;
		case D3D_FEATURE_LEVEL_11_0:
			gfx.info.version      = "Feature Level 11.0";
			gfx.info.shading_lang = "HLSL 5.0";
			break;
		case D3D_FEATURE_LEVEL_11_1:
			gfx.info.version      = "Feature Level 11.1";
			gfx.info.shading_lang = "HLSL 5.0";
			break;
		case D3D_FEATURE_LEVEL_12_0:
			gfx.info.version      = "Feature Level 12.0";
			gfx.info.shading_lang = "HLSL 5.0";
			break;
		case D3D_FEATURE_LEVEL_12_1:
			gfx.info.version      = "Feature Level 12.1";
			gfx.info.shading_lang = "HLSL 5.0";
			break;
		default: {
			gfx.info.version      = "Unknown";
			gfx.info.shading_lang = "HLSL";
			break;
		}
	}

	return &gfx;
}
