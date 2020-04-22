#pragma once
#include "karma.h"

constexpr r32 MATH_PI = 3.1415926535f;
constexpr r32 MATH_PI_INVERSE = 1.0f / MATH_PI;
constexpr r32 MATH_TAU = MATH_PI / 2;
constexpr r32 MATH_R32_EQUALS_DEFAULT_TOLERANCE = 0.001f;

#define vec_expand2(v) (v).x, (v).y
#define vec_expand3(v) (v).x, (v).y, (v).z
#define vec_expand4(v) (v).x, (v).y, (v).z, (v).w

#include <math.h>

union Vec2 {
	struct {
		r32 x, y;
	};
	r32 m[2];
};

inline Vec2 vec2(r32 a) { Vec2 v; v.x = v.y = a; return v; }
inline Vec2 vec2(r32 a, r32 b) { Vec2 v; v.x = a; v.y = b; return v; }

union Vec2s {
	struct {
		s32 x, y;
	};
	s32 m[2];
};

inline Vec2s vec2s(int a) { Vec2s v; v.x = v.y = a; return v; }
inline Vec2s vec2s(int a, int b) { Vec2s v; v.x = a; v.y = b; return v; }

union Vec3 {
	struct {
		r32 x, y, z;
	};
	struct {
		r32 r, g, b;
	};
	struct {
		Vec2  xy;
		r32 _ignorez;
	};
	struct {
		r32 _ignorex;
		Vec2  yz;
	};
	r32 m[3];
};

inline Vec3 vec3(r32 a) { Vec3 v; v.x = a; v.y = a; v.z = a; return v; }
inline Vec3 vec3(r32 a, r32 b, r32 c) { Vec3 v; v.x = a; v.y = b; v.z = c; return v; }
inline Vec3 vec3(Vec2 a, r32 b) { Vec3 v;  v.xy = a; v.z = b; return v; }
inline Vec3 vec3(r32 a, Vec2 b) { Vec3 v;  v.x = a; v.yz = b; return v; }

union Vec3s {
	struct {
		s32 x, y, z;
	};
	struct {
		s32 r, g, b;
	};
	struct {
		Vec2s xy;
		s32   _ignorez;
	};
	struct {
		s32   _ignorex;
		Vec2s yz;
	};
	s32 m[3];
};

inline Vec3s vec3s(int a) { Vec3s v; v.x = v.y = v.z = a; return v; }
inline Vec3s vec3s(int a, int b, int c) { Vec3s v; v.x = a; v.y = b; v.z = c; return v; }
inline Vec3s vec3s(Vec2s a, int b) { Vec3s v; v.xy = a; v.z = b; return v; }
inline Vec3s vec3s(int a, Vec2s b) { Vec3s v; v.x = a; v.yz = b; return v; }

union Vec4 {
	struct {
		r32 x, y, z, w;
	};
	struct {
		r32 r, g, b, a;
	};
	struct {
		Vec2 xy;
		Vec2 zw;
	};
	struct {
		Vec3  xyz;
		r32 _ignorew;
	};
	struct {
		r32 _ignorex;
		Vec3  yzw;
	};
	struct {
		Vec3  rgb;
		r32 alpha;
	};
	r32 m[4];
};

inline Vec4 vec4(r32 a) { Vec4 v; v.x = v.y = v.z = v.w = a; return v; }
inline Vec4 vec4(r32 a, r32 b, r32 c, r32 d = 1) { Vec4 v; v.x = a; v.y = b; v.z = c; v.w = d; return v; }
inline Vec4 vec4(Vec2 a, Vec2 b) { Vec4 v; v.xy = a; v.zw = b; return v; }
inline Vec4 vec4(Vec2 a, r32 b, r32 c = 1) { Vec4 v; v.xy = a; v.z = b; v.w = c; return v; }
inline Vec4 vec4(Vec3 a, r32 b = 1) { Vec4 v; v.xyz = a; v.w = b; return v; }
inline Vec4 vec4(r32 a, Vec3 b) { Vec4 v; v.x = a; v.yzw = b; return v; }

union Vec4s {
	struct {
		s32 x, y, z, w;
	};
	struct {
		s32 r, g, b, a;
	};
	struct {
		Vec2s xy;
		Vec2s zw;
	};
	struct {
		Vec3s xyz;
		s32   _ignorew;
	};
	struct {
		s32   _ignorex;
		Vec3s yzw;
	};
	struct {
		Vec3s rgb;
		s32   alpha;
	};
	s32 m[4];
};

inline Vec4s vec4s(int a) { Vec4s v; v.x = v.y = v.z = v.w = a; return v; }
inline Vec4s vec4s(int a, int b, int c, int d) { Vec4s v; v.x = a; v.y = b; v.z = c; v.w = d; return v; }
inline Vec4s vec4s(Vec2s a, Vec2s b) { Vec4s v; v.xy = a; v.zw = b; return v; }
inline Vec4s vec4s(Vec3s a, int b) { Vec4s v; v.xyz = a; v.w = b; return v; }
inline Vec4s vec4s(int a, Vec3s b) { Vec4s v; v.x = a; v.yzw = b; return v; }

typedef Vec4 Color4;
typedef Vec3 Color3;

struct Color_HSV {
	r32 h, s, v; // [0, 1]
};

union Colorh {
	struct {
		u8 r, g, b, a;
	};
	u32 hex;
};

inline Colorh colorh(u8 _r, u8 _g, u8 _b, u8 _a = 0xff) { Colorh c; c.r = _r; c.g = _g; c.b = _b; c.a = _a; return c; }
inline Colorh colorh(u32 color) { Colorh c; c.hex = color; return c; }

union Mat3 {
	Vec3  rows[3];
	r32 m[9];
	r32 m2[4][4];
};

inline Mat3 mat3(Vec3 r0, Vec3 r1, Vec3 r2) {
	Mat3 res;
	res.rows[0] = r0;
	res.rows[1] = r1;
	res.rows[2] = r2;
	return res;
}

inline Mat3 mat3(r32 x00, r32 x10, r32 x20,
	r32 x01, r32 x11, r32 x21,
	r32 x02, r32 x12, r32 x22) {
	Mat3 res;
	res.rows[0] = vec3(x00, x10, x20);
	res.rows[1] = vec3(x01, x11, x21);
	res.rows[2] = vec3(x02, x12, x22);
	return res;
}

union Mat4 {
	Vec4  rows[4];
	r32 m[16];
	r32 m2[4][4];
};

inline Mat4 mat4(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3) {
	Mat4 m;
	m.rows[0] = r0;
	m.rows[1] = r1;
	m.rows[2] = r2;
	m.rows[3] = r3;
	return m;
}

inline Mat4 mat4(r32 x00, r32 x10, r32 x20, r32 x30,
	r32 x01, r32 x11, r32 x21, r32 x31,
	r32 x02, r32 x12, r32 x22, r32 x32,
	r32 x03, r32 x13, r32 x23, r32 x33) {
	Mat4 m;
	m.rows[0] = vec4(x00, x10, x20, x30);
	m.rows[1] = vec4(x01, x11, x21, x31);
	m.rows[2] = vec4(x02, x12, x22, x32);
	m.rows[3] = vec4(x03, x13, x23, x33);
	return m;
}

union Quat {
	struct {
		r32 x, y, z, w;
	};
	struct {
		r32 i, j, k, real;
	};
	struct {
		Vec4 v4;
	};
	r32 m[4];
};

inline Quat quat(r32 _b, r32 _c, r32 _d, r32 _a) { Quat q; q.x = _b; q.y = _c; q.z = _d; q.w = _a; return q; }
inline Quat quat(Vec4 v) { Quat q; q.v4 = v; return q; }

struct Mm_Rect {
	Vec2 min;
	Vec2 max;
};

inline Mm_Rect mm_rect(r32 minx, r32 miny, r32 maxx, r32 maxy) {
	Mm_Rect rect;
	rect.min.x = minx;
	rect.min.y = miny;
	rect.max.x = maxx;
	rect.max.y = maxy;
	return rect;
}
inline Mm_Rect mm_rect(Vec2 _min, Vec2 _max) {
	Mm_Rect rect;
	rect.min = _min;
	rect.max = _max;
	return rect;
}

struct Rect {
	r32 x, y;
	r32 w, h;
};

inline Rect rect(r32 _x, r32 _y, r32 _w, r32 _h) {
	Rect r;
	r.x = _x;
	r.y = _y;
	r.w = _w;
	r.h = _h;
	return r;
}

struct Rects {
	s32 x, y;
	s32 w, h;
};

inline Rects rects(s32 _x, s32 _y, s32 _w, s32 _h) {
	Rects r;
	r.x = _x;
	r.y = _y;
	r.w = _w;
	r.h = _h;
	return r;
}
