#pragma once
#include "shared.h"

constexpr r32 MATH_PI							= 3.1415926535f;
constexpr r32 MATH_PI_INVERSE					= 1.0f / MATH_PI;
constexpr r32 MATH_TAU							= MATH_PI / 2;
constexpr r32 MATH_R32_EQUALS_DEFAULT_TOLERANCE = 0.001f;

#define vec_expand2(v) (v).x, (v).y
#define vec_expand3(v) (v).x, (v).y, (v).z
#define vec_expand4(v) (v).x, (v).y, (v).z, (v).w

#include <math.h>

union Vec2 {
	r32 m[2];

	struct {
		r32 x, y;
	};

	inline Vec2() {}
	inline Vec2(r32 a) { x = y = a; }
	inline Vec2(r32 a, r32 b) {
		x = a;
		y = b;
	}
};

union Vec2s {
	s32 m[2];
	struct {
		s32 x, y;
	};

	inline Vec2s() {}
	inline Vec2s(int a) { x = y = a; }
	inline Vec2s(int a, int b) {
		x = a;
		y = b;
	}
};

union Vec3 {
	r32 m[3];

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

	inline Vec3() {}
	inline Vec3(r32 a) { x = y = z = a; }
	inline Vec3(r32 a, r32 b, r32 c) {
		x = a;
		y = b;
		z = c;
	}
	inline Vec3(Vec2 a, r32 b) {
		xy = a;
		z  = b;
	}
	inline Vec3(r32 a, Vec2 b) {
		x  = a;
		yz = b;
	}
};

union Vec3s {
	s32 m[3];
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

	inline Vec3s() {}
	inline Vec3s(int a) { x = y = z = a; }
	inline Vec3s(int a, int b, int c) {
		x = a;
		y = b;
		z = c;
	}
	inline Vec3s(Vec2s a, int b) {
		xy = a;
		z  = b;
	}
	inline Vec3s(int a, Vec2s b) {
		x  = a;
		yz = b;
	}
};

union Vec4 {
	r32 m[4];
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

	inline Vec4() {}
	inline Vec4(r32 a) { x = y = z = w = a; }
	inline Vec4(r32 a, r32 b, r32 c, r32 d = 1) {
		x = a;
		y = b;
		z = c;
		w = d;
	}
	inline Vec4(Vec2 a, Vec2 b) {
		xy = a;
		zw = b;
	}
	inline Vec4(Vec2 a, r32 b, r32 c = 1) {
		xy = a;
		z  = b;
		w  = c;
	}
	inline Vec4(Vec3 a, r32 b = 1) {
		xyz = a;
		w   = b;
	}
	inline Vec4(r32 a, Vec3 b) {
		x   = a;
		yzw = b;
	}
};

union Vec4s {
	s32 m[4];
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

	inline Vec4s() {}
	inline Vec4s(int a) { x = y = z = w = a; }
	inline Vec4s(int a, int b, int c, int d) {
		x = a;
		y = b;
		z = c;
		w = d;
	}
	inline Vec4s(Vec2s a, Vec2s b) {
		xy = a;
		zw = b;
	}
	inline Vec4s(Vec3s a, int b) {
		xyz = a;
		w   = b;
	}
	inline Vec4s(int a, Vec3s b) {
		x   = a;
		yzw = b;
	}
};

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

	inline Colorh() {}

	inline Colorh(u8 _r, u8 _g, u8 _b, u8 _a = 0xff) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline Colorh(u32 color) {
		hex = color;
	}
};

union Mat3 {
	Vec3  rows[3];
	r32 m[9];
	r32 m2[4][4];

	inline Mat3() {}

	inline Mat3(Vec3 r0, Vec3 r1, Vec3 r2) {
		rows[0] = r0;
		rows[1] = r1;
		rows[2] = r2;
	}

	inline Mat3(r32 x00, r32 x10, r32 x20,
				r32 x01, r32 x11, r32 x21,
				r32 x02, r32 x12, r32 x22) {
		rows[0] = Vec3(x00, x10, x20);
		rows[1] = Vec3(x01, x11, x21);
		rows[2] = Vec3(x02, x12, x22);
	}
};

union Mat4 {
	Vec4  rows[4];
	r32 m[16];
	r32 m2[4][4];

	inline Mat4() {}

	inline Mat4(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3) {
		rows[0] = r0;
		rows[1] = r1;
		rows[2] = r2;
		rows[3] = r3;
	}

	inline Mat4(r32 x00, r32 x10, r32 x20, r32 x30,
				r32 x01, r32 x11, r32 x21, r32 x31,
				r32 x02, r32 x12, r32 x22, r32 x32,
				r32 x03, r32 x13, r32 x23, r32 x33) {
		rows[0] = Vec4(x00, x10, x20, x30);
		rows[1] = Vec4(x01, x11, x21, x31);
		rows[2] = Vec4(x02, x12, x22, x32);
		rows[3] = Vec4(x03, x13, x23, x33);
	}
};

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

	inline Quat() {}
	inline Quat(r32 _b, r32 _c, r32 _d, r32 _a) {
		x = _b;
		y = _c;
		z = _d;
		w = _a;
	}
	inline Quat(Vec4 v) {
		v4 = v;
	}
};

struct Mm_Rect {
	Vec2 min;
	Vec2 max;

	inline Mm_Rect() {}
	inline Mm_Rect(r32 minx, r32 miny, r32 maxx, r32 maxy) {
		min.x = minx;
		min.y = miny;
		max.x = maxx;
		max.y = maxy;
	}
	inline Mm_Rect(Vec2 _min, Vec2 _max) {
		min = _min;
		max = _max;
	}
};

struct Rect {
	r32 x, y;
	r32 w, h;

	inline Rect() {}
	inline Rect(r32 _x, r32 _y, r32 _w, r32 _h) {
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}
};

struct Rects {
	s32 x, y;
	s32 w, h;

	inline Rects() {}
	inline Rects(s32 _x, s32 _y, s32 _w, s32 _h) {
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}
};
