#pragma once
#include "maths.h"

//
// Graphics rendering api
//
enum Render_Backend {
	Render_Backend_NONE,
	Render_Backend_OPENGL,

	Render_Backend_COUNT
};

enum Texture_Wrap {
	Texture_Wrap_REPEAT,
	Texture_Wrap_CLAMP
};

enum Texture_Filter {
	Texture_Filter_LINEAR,
	Texture_Filter_NEAREST
};

struct Sampler_Params {
	Texture_Wrap wrap_s; // default: TextureWrap_REPEAT
	Texture_Wrap wrap_t; // default: TextureWrap_REPEAT
	Texture_Filter min_filter; // default: TextureFilter_NEAREST
	Texture_Filter mag_filter; // default: TextureFilter_LINEAR
	bool gen_mipmaps; // default: false
	// NOTE : Unused if channels is 1
	bool srgb; // default: false
};

enum Buffer_Type {
	Buffer_Type_DYNAMIC, Buffer_Type_STATIC
};

enum Color_Format {
	Color_Format_NONE,
	Color_Format_R,
	Color_Format_RG,
	Color_Format_RGB,
	Color_Format_RGBA
};

enum Depth_Format {
	Depth_Format_NONE,
	Depth_Format_STORAGE,
	Depth_Format_TEXTURE,
};

struct Framebuffer {
	int width, height;
	Color_Format color_format;
	Depth_Format depth_format;
	Handle id;
	Handle color;
	Handle depth;
};

struct Texture2d {
	Handle handle = {};
	Mm_Rect rect = {};
};

struct Render_Region {
	Rects viewport;
	Rects scissor;
};

enum Clear_Flag_Bit : u32 {
	Clear_Flag_NONE		= bit(0),
	Clear_Flag_COLOR	= bit(1),
	Clear_Flag_DEPTH	= bit(2),
	Clear_Flag_STENCIL	= bit(3),
	Clear_Flag_ALL		= Clear_Flag_COLOR | Clear_Flag_DEPTH | Clear_Flag_STENCIL
};
typedef u32 Clear_Flag;

struct Camera_View {
	enum Kind {
		PERSPECTIVE, ORTHOGRAPHIC
	};

	Kind kind;

	union {
		struct {
			float field_of_view;
			float aspect_ratio;
			float far_plane;
			float near_plane;
		} perspective;
		struct {
			float left;
			float right;
			float top;
			float bottom;
			float near;
			float far;
		} orthographic;
	};

};

inline Camera_View perspective_view(float fov, float ar, float n, float f) {
	Camera_View view;
	view.kind = Camera_View::PERSPECTIVE;
	view.perspective.field_of_view = fov;
	view.perspective.aspect_ratio = ar;
	view.perspective.near_plane = n;
	view.perspective.far_plane = f;
	return view;
}

inline Camera_View orthographic_view(float left, float right, float top, float bottom, float near = -1, float far = 1) {
	Camera_View view;
	view.kind = Camera_View::ORTHOGRAPHIC;
	view.orthographic.left = left;
	view.orthographic.right = right;
	view.orthographic.top = top;
	view.orthographic.bottom = bottom;
	view.orthographic.near = near;
	view.orthographic.far = far;
	return view;
}
