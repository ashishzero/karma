#pragma once
#include "lin_maths.h"

struct Texture2d {
	Handle id;
};

struct Texture_View {
	Handle id;
};

struct Render_Target_View {
	Handle id;
};

struct Depth_Stencil_View {
	Handle id;
};

struct Sampler {
	Handle id;
};

struct Vertex_Buffer {
	Handle id;
};

struct Index_Buffer {
	Handle id;
};

struct Uniform_Buffer {
	Handle id;
};

struct Render_Pipeline {
	Handle id;
};

enum Render_Backend {
	Render_Backend_NONE,
	Render_Backend_OPENGL,
	Render_Backend_DIRECTX11,

	Render_Backend_COUNT
};

enum Data_Format {
	Data_Format_UNKNOWN,
	Data_Format_RGBA32_FLOAT,
	Data_Format_RGBA32_UINT,
	Data_Format_RGBA32_SINT,
	Data_Format_RGB32_FLOAT,
	Data_Format_RGB32_UINT,
	Data_Format_RGB32_SINT,
	Data_Format_RGBA16_FLOAT,
	Data_Format_RGBA16_UNORM,
	Data_Format_RGBA16_UINT,
	Data_Format_RGBA16_SNORM,
	Data_Format_RGBA16_SINT,
	Data_Format_RG32_FLOAT,
	Data_Format_RG32_UINT,
	Data_Format_RG32_SINT,
	Data_Format_D32_FLOAT_S8X24_UINT,
	Data_Format_R32_FLOAT_X8X24_TYPELESS,
	Data_Format_X32_TYPELESS_G8X24_UINT,
	Data_Format_R10G10B10A2_TYPELESS,
	Data_Format_R10G10B10A2_UNORM,
	Data_Format_R10G10B10A2_UINT,
	Data_Format_R11G11B10_FLOAT,
	Data_Format_RGBA8_UNORM,
	Data_Format_RGBA8_UNORM_SRGB,
	Data_Format_RGBA8_UINT,
	Data_Format_RGBA8_SNORM,
	Data_Format_RGBA8_SINT,
	Data_Format_RG16_FLOAT,
	Data_Format_RG16_UNORM,
	Data_Format_RG16_UINT,
	Data_Format_RG16_SNORM,
	Data_Format_RG16_SINT,
	Data_Format_D32_FLOAT,
	Data_Format_R32_FLOAT,
	Data_Format_R32_UINT,
	Data_Format_R32_SINT,
	Data_Format_D24_UNORM_S8_UINT,
	Data_Format_RG8_UNORM,
	Data_Format_RG8_UINT,
	Data_Format_RG8_SNORM,
	Data_Format_RG8_SINT,
	Data_Format_R16_FLOAT,
	Data_Format_D16_UNORM,
	Data_Format_R16_UNORM,
	Data_Format_R16_UINT,
	Data_Format_R16_SNORM,
	Data_Format_R16_SINT,
	Data_Format_R8_UNORM,
	Data_Format_R8_UINT,
	Data_Format_R8_SNORM,
	Data_Format_R8_SINT,
	Data_Format_A8_UNORM,

	Data_Format_COUNT
};

enum Buffer_Usage {
	Buffer_Usage_DEFAULT,
	Buffer_Usage_IMMUTABLE,
	Buffer_Usage_DYNAMIC,
	Buffer_Usage_STAGING,

	Buffer_Usage_COUNT,
};

enum Texture_Bind_Flag_Bit : u32 {
	Texture_Bind_SHADER_RESOURCE = bit(1),
	Texture_Bind_RENDER_TARGET   = bit(2),
	Texture_Bind_DEPTH_STENCIL   = bit(3)
};
typedef u32 Texture_Bind_Flags;

enum Cpu_Access_Flag_Bit : u32 {
	Cpu_Access_READ  = bit(1),
	Cpu_Access_WRITE = bit(2),
};
typedef u32 Cpu_Access_Flags;

enum Map_Type {
	Map_Type_READ,
	Map_Type_WRITE,
	Map_Type_READ_WRITE,
	Map_Type_WRITE_DISCARD,

	Map_Type_COUNT,
};

enum Filter_Type {
	Filter_Type_MIN_MAG_MIP_POINT,
	Filter_Type_MIN_MAG_POINT_MIP_LINEAR,
	Filter_Type_MIN_POINT_MAG_LINEAR_MIP_POINT,
	Filter_Type_MIN_POINT_MAG_MIP_LINEAR,
	Filter_Type_MIN_LINEAR_MAG_MIP_POINT,
	Filter_Type_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	Filter_Type_MIN_MAG_LINEAR_MIP_POINT,
	Filter_Type_MIN_MAG_MIP_LINEAR,
	Filter_Type_ANISOTROPIC,
	Filter_Type_COMPARISON_MIN_MAG_MIP_POINT,
	Filter_Type_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
	Filter_Type_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
	Filter_Type_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
	Filter_Type_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
	Filter_Type_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	Filter_Type_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
	Filter_Type_COMPARISON_MIN_MAG_MIP_LINEAR,
	Filter_Type_COMPARISON_ANISOTROPIC,
	Filter_Type_MINIMUM_MIN_MAG_MIP_POINT,
	Filter_Type_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
	Filter_Type_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
	Filter_Type_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
	Filter_Type_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
	Filter_Type_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	Filter_Type_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
	Filter_Type_MINIMUM_MIN_MAG_MIP_LINEAR,
	Filter_Type_MINIMUM_ANISOTROPIC,
	Filter_Type_MAXIMUM_MIN_MAG_MIP_POINT,
	Filter_Type_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
	Filter_Type_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
	Filter_Type_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
	Filter_Type_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
	Filter_Type_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	Filter_Type_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
	Filter_Type_MAXIMUM_MIN_MAG_MIP_LINEAR,
	Filter_Type_MAXIMUM_ANISOTROPIC,

	Filter_Type_COUNT
};

enum Comparison_Function {
	Comparison_Function_NEVER,
	Comparison_Function_LESS,
	Comparison_Function_EQUAL,
	Comparison_Function_LESS_EQUAL,
	Comparison_Function_GREATER,
	Comparison_Function_NOT_EQUAL,
	Comparison_Function_GREATER_EQUAL,
	Comparison_Function_ALWAYS,

	Comparison_Function_COUNT,
};

struct Filter {
	Filter_Type         type;
	u32                 max_anisotropy;
	Comparison_Function comparison;
};

inline Filter filter_create(Filter_Type         type           = Filter_Type_MIN_MAG_MIP_LINEAR,
							u32                 max_anisotropy = 1,
							Comparison_Function comp           = Comparison_Function_NEVER) {
	Filter filter;
	filter.type           = type;
	filter.max_anisotropy = max_anisotropy;
	filter.comparison     = comp;
	return filter;
}

enum Texture_Address_Mode {
	Texture_Address_Mode_WRAP,
	Texture_Address_Mode_MIRROR,
	Texture_Address_Mode_CLAMP,
	Texture_Address_Mode_BORDER,
	Texture_Address_Mode_MIRROR_ONCE,

	Texture_Address_Mode_COUNT,
};

struct Texture_Address {
	Texture_Address_Mode u, v, w;
	Color4               border_color;
};

inline Texture_Address texture_address_create(Texture_Address_Mode u            = Texture_Address_Mode_CLAMP,
											  Texture_Address_Mode v            = Texture_Address_Mode_CLAMP,
											  Texture_Address_Mode w            = Texture_Address_Mode_CLAMP,
											  Color4               border_color = vec4(1)) {
	Texture_Address address;
	address.u            = u;
	address.v            = v;
	address.w            = w;
	address.border_color = border_color;
	return address;
}

struct Level_Of_Detail {
	r32 bias, min, max;
};

inline Level_Of_Detail level_of_detail_create(r32 bias = 0, r32 min = MIN_FLOAT, r32 max = MAX_FLOAT) {
	Level_Of_Detail lod;
	lod.bias = bias;
	lod.min  = min;
	lod.max  = max;
	return lod;
}

enum Primitive {
	Primitive_UNDEFINED,
	Primitive_POINTLIST,
	Primitive_LINELIST,
	Primitive_LINESTRIP,
	Primitive_TRIANGLELIST,
	Primitive_TRIANGLESTRIP,

	Primitive_COUNT
};

enum Fill_Mode {
	Fill_Mode_WIREFRAME,
	Fill_Mode_SOLID,

	Fill_Mode_COUNT
};

enum Cull_Mode {
	Cull_Mode_NONE,
	Cull_Mode_FRONT,
	Cull_Mode_BACK,

	Cull_Mode_COUNT
};

enum Front_Face {
	Front_Face_CLOCKWISE,
	Front_Face_COUNTER_CLOCKWISE,

	Front_Face_COUNT
};

struct Rasterizer_Info {
	Primitive  primitive;
	Fill_Mode  fill_mode;
	Cull_Mode  cull_mode;
	Front_Face front_face;
	s32        depth_bias;
	r32        depth_bias_clamp;
	bool       depth_clip;
	bool       scissor_enable;
	bool       multisample;
};

inline Rasterizer_Info rasterizer_info_create(Primitive  primitive  = Primitive_TRIANGLELIST,
											  Fill_Mode  fill_mode  = Fill_Mode_SOLID,
											  Cull_Mode  cull_mode  = Cull_Mode_BACK,
											  Front_Face front_face = Front_Face_CLOCKWISE,
											  s32 depth_bias = 0, r32 depth_bias_clamp = 0,
											  bool depth_clip = true, bool scissor_enable = false,
											  bool multisample = false) {
	Rasterizer_Info rasterizer;
	rasterizer.primitive        = primitive;
	rasterizer.fill_mode        = fill_mode;
	rasterizer.cull_mode        = cull_mode;
	rasterizer.front_face       = front_face;
	rasterizer.depth_bias       = depth_bias;
	rasterizer.depth_bias_clamp = depth_bias_clamp;
	rasterizer.depth_clip       = depth_clip;
	rasterizer.scissor_enable   = scissor_enable;
	rasterizer.multisample      = multisample;
	return rasterizer;
}

enum Blend {
	Blend_ZERO,
	Blend_ONE,
	Blend_SRC_COLOR,
	Blend_INV_SRC_COLOR,
	Blend_SRC_ALPHA,
	Blend_INV_SRC_ALPHA,
	Blend_DEST_ALPHA,
	Blend_INV_DEST_ALPHA,
	Blend_DEST_COLOR,
	Blend_INV_DEST_COLOR,
	Blend_SRC_ALPHA_SAT,
	Blend_BLEND_FACTOR,
	Blend_INV_BLEND_FACTOR,
	Blend_SRC1_COLOR,
	Blend_INV_SRC1_COLOR,
	Blend_SRC1_ALPHA,
	Blend_INV_SRC1_ALPHA,

	Blend_COUNT
};

enum Blend_Operation {
	Blend_Operation_ADD,
	Blend_Operation_SUBTRACT,
	Blend_Operation_REV_SUBTRACT,
	Blend_Operation_MIN,
	Blend_Operation_MAX,

	Blend_Operation_COUNT
};

enum Blend_Mask_Bit : u8 {
	Blend_Mask_NONE      = 0,
	Blend_Mask_R         = bit(1),
	Blend_Mask_G         = bit(2),
	Blend_Mask_B         = bit(3),
	Blend_Mask_A         = bit(4),
	Blend_Mask_WRITE_ALL = Blend_Mask_R | Blend_Mask_G | Blend_Mask_B | Blend_Mask_A,
};
typedef u8 Blend_Mask;

struct Blend_Info {
	bool            enable;
	Blend           src;
	Blend           dst;
	Blend_Operation op;
	Blend           src_alpha;
	Blend           dst_alpha;
	Blend_Operation op_alpha;
	Blend_Mask      mask;
};

inline Blend_Info blend_info_disabled() {
	Blend_Info blend = {};
	return blend;
}

inline Blend_Info blend_info_create(Blend           src       = Blend_ONE,
									Blend           dst       = Blend_ZERO,
									Blend_Operation op        = Blend_Operation_ADD,
									Blend           src_alpha = Blend_ONE,
									Blend           dst_alpha = Blend_ZERO,
									Blend_Operation op_alpha  = Blend_Operation_ADD,
									Blend_Mask      mask      = Blend_Mask_WRITE_ALL) {
	Blend_Info blend;
	blend.enable    = true;
	blend.src       = src;
	blend.dst       = dst;
	blend.op        = op;
	blend.src_alpha = src_alpha;
	blend.dst_alpha = dst_alpha;
	blend.op_alpha  = op_alpha;
	blend.mask      = mask;
	return blend;
}

enum Depth_Write_Mask {
	Depth_Write_Mask_ZERO,
	Depth_Write_Mask_ALL,

	Depth_Write_Mask_COUNT
};

struct Depth_Info {
	bool                test_enable;
	Depth_Write_Mask    write_mask;
	Comparison_Function compare;
};

inline Depth_Info depth_info_create(bool                enable_test = true,
									Depth_Write_Mask    write_mask  = Depth_Write_Mask_ALL,
									Comparison_Function compare     = Comparison_Function_LESS) {
	Depth_Info depth;
	depth.test_enable = enable_test;
	depth.write_mask  = write_mask;
	depth.compare     = compare;
	return depth;
}

enum Input_Type {
	Input_Type_PER_VERTEX_DATA,
	Input_Type_PER_INSTANCE_DATA,

	Input_Type_COUNT,
};

struct Input_Element_Layout {
	const char *name;
	Data_Format format;
	u32         offset;
	Input_Type  input_type;
	u32         instance_data_step_rate;
};

struct Shader_Info {
	String vertex;
	String pixel;

	Input_Element_Layout *input_layouts;
	u32                   input_layouts_count;
	u32                   stride;
};

//
//
//

struct Color_Clear_Info {
	Color4 color;
	bool   clear;
};

enum Depth_Stencil_Clear_Flag_Bit : u8 {
	Depth_Stencil_Clear_NONE    = 0,
	Depth_Stencil_Clear_DEPTH   = bit(1),
	Depth_Stencil_Clear_STENCIL = bit(2),
	Depth_Stencil_Clear_ALL     = Depth_Stencil_Clear_DEPTH | Depth_Stencil_Clear_STENCIL
};
typedef u8 Depth_Stencil_Clear_Flags;

struct Depth_Stencil_Clear_Info {
	r32                       depth_clear_value;
	u8                        stencil_clear_value;
	Depth_Stencil_Clear_Flags flags;
};

enum Index_Type {
	Index_Type_U8,
	Index_Type_U16,
	Index_Type_U32,

	Index_Type_COUNT,
};

//
//
//

struct Texture2d_Handle {
	Texture_View view;
	Texture2d    buffer;
};

enum Camera_View_Kind {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

struct Camera_View_Perspective {
	float field_of_view;
	float aspect_ratio;
	float far_plane;
	float near_plane;
};

struct Camera_View_Orthographic {
	float left;
	float right;
	float top;
	float bottom;
	float near;
	float far;
};

struct Camera_View {
	// TODO: Add Depth Of Field
	Camera_View_Kind kind;

	union {
		Camera_View_Perspective  perspective;
		Camera_View_Orthographic orthographic;
	};
};

inline Camera_View perspective_view(float fov, float ar, float n, float f) {
	Camera_View view;
	view.kind                      = PERSPECTIVE;
	view.perspective.field_of_view = fov;
	view.perspective.aspect_ratio  = ar;
	view.perspective.near_plane    = n;
	view.perspective.far_plane     = f;
	return view;
}

inline Camera_View orthographic_view(float left, float right, float top, float bottom, float near = -1, float far = 1) {
	Camera_View view;
	view.kind                = ORTHOGRAPHIC;
	view.orthographic.left   = left;
	view.orthographic.right  = right;
	view.orthographic.top    = top;
	view.orthographic.bottom = bottom;
	view.orthographic.near   = near;
	view.orthographic.far    = far;
	return view;
}
