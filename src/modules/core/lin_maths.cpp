#include "lin_maths.h"
#include "lin_maths_types.h"

Vec2 vec2_add(Vec2 a, Vec2 b) {
	return vec2(a.x + b.x, a.y + b.y);
}
Vec3 vec3_add(Vec3 a, Vec3 b) {
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
Vec4 vec4_add(Vec4 a, Vec4 b) {
	return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
Vec2s vec2s_add(Vec2s a, Vec2s b) {
	return vec2s(a.x + b.x, a.y + b.y);
}
Vec3s vec3s_add(Vec3s a, Vec3s b) {
	return vec3s(a.x + b.x, a.y + b.y, a.z + b.z);
}
Vec4s vec4s_add(Vec4s a, Vec4s b) {
	return vec4s(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

//
//
//

Vec2 vec2_sub(Vec2 a, Vec2 b) {
	return vec2(a.x - b.x, a.y - b.y);
}
Vec3 vec3_sub(Vec3 a, Vec3 b) {
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
Vec4 vec4_sub(Vec4 a, Vec4 b) {
	return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
Vec2s vec2s_sub(Vec2s a, Vec2s b) {
	return vec2s(a.x - b.x, a.y - b.y);
}
Vec3s vec3s_sub(Vec3s a, Vec3s b) {
	return vec3s(a.x - b.x, a.y - b.y, a.z - b.z);
}
Vec4s vec4s_sub(Vec4s a, Vec4s b) {
	return vec4s(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

//
//
//

Vec2 vec2_mul(r32 s, Vec2 v) {
	return vec2(s * v.x, s * v.y);
}
Vec2 vec2_mul(Vec2 v, r32 s) {
	return vec2_mul(s, v);
}
Vec3 vec3_mul(r32 s, Vec3 v) {
	return vec3(s * v.x, s * v.y, s * v.z);
}
Vec3 vec3_mul(Vec3 v, r32 s) {
	return vec3_mul(s, v);
}
Vec4 vec4_mul(r32 s, Vec4 v) {
	return vec4(s * v.x, s * v.y, s * v.z, s * v.w);
}
Vec4 vec4_mul(Vec4 v, r32 s) {
	return vec4_mul(s, v);
}
Vec2s vec2s_mul(s32 s, Vec2s v) {
	return vec2s(s * v.x, s * v.y);
}
Vec2s vec2s_mul(Vec2s v, s32 s) {
	return vec2s_mul(s, v);
}
Vec3s vec3s_mul(s32 s, Vec3s v) {
	return vec3s(s * v.x, s * v.y, s * v.z);
}
Vec3s vec3s_mul(Vec3s v, s32 s) {
	return vec3s_mul(s, v);
}
Vec4s vec4s_mul(s32 s, Vec4s v) {
	return vec4s(s * v.x, s * v.y, s * v.z, s * v.w);
}
Vec4s vec4s_mul(Vec4s v, s32 s) {
	return vec4s_mul(s, v);
}

//
//
//

Vec2 vec2_div(Vec2 v, r32 s) {
	s = 1.0f / s;
	return vec2(v.x * s, v.y * s);
}
Vec3 vec3_div(Vec3 v, r32 s) {
	s = 1.0f / s;
	return vec3(v.x * s, v.y * s, v.z * s);
}
Vec4 vec4_div(Vec4 v, r32 s) {
	return vec4_mul(v, 1.0f / s);
}
Vec2s vec2s_div(Vec2s v, s32 s) {
	return vec2s(v.x / s, v.y / s);
}
Vec3s vec3s_div(Vec3s v, s32 s) {
	return vec3s(v.x / s, v.y / s, v.z / s);
}
Vec4s vec4s_div(Vec4s v, s32 s) {
	return vec4s(v.x / s, v.y / s, v.z / s, v.w / s);
}

//
//
//

Vec2 vec2_hadamard(Vec2 l, Vec2 r) {
	return vec2(l.x * r.x, l.y * r.y);
}
Vec3 vec3_hadamard(Vec3 l, Vec3 r) {
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}
Vec4 vec4_hadamard(Vec4 l, Vec4 r) {
	return vec4(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
}
Vec2s vec2s_hadamard(Vec2s l, Vec2s r) {
	return vec2s(l.x * r.x, l.y * r.y);
}
Vec3s vec3s_hadamard(Vec3s l, Vec3s r) {
	return vec3s(l.x * r.x, l.y * r.y, l.z * r.z);
}
Vec4s vec4s_hadamard(Vec4s l, Vec4s r) {
	return vec4s(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
}

//
//
//

r32 vec2_dot(Vec2 a, Vec2 b) {
	return a.x * b.x + a.y * b.y;
}
r32 vec3_dot(Vec3 a, Vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
r32 vec4_dot(Vec4 a, Vec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//
//
//

Vec3 vec3_cross(Vec3 a, Vec3 b) {
	Vec3 res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}

//
//
//

r32 vec2_length2(Vec2 v) {
	return vec2_dot(v, v);
}
r32 vec3_length2(Vec3 v) {
	return vec3_dot(v, v);
}
r32 vec4_length2(Vec4 v) {
	return vec4_dot(v, v);
}

r32 vec2_length(Vec2 v) {
	return sqrtf(vec2_length2(v));
}
r32 vec3_length(Vec3 v) {
	return sqrtf(vec3_length2(v));
}
r32 vec4_length(Vec4 v) {
	return sqrtf(vec4_length2(v));
}

r32 vec2_distance(Vec2 a, Vec2 b) {
	return vec2_length(b - a);
}
r32 vec3_distance(Vec3 a, Vec3 b) {
	return vec3_length(b - a);
}
r32 vec4_distance(Vec4 a, Vec4 b) {
	return vec4_length(b - a);
}

//
//
//

// these check for division by zero
// returns  {0} if vector length is zero
Vec2 vec2_normalize_check(Vec2 v) {
	Vec2 res = {};
	r32  len = vec2_length(v);
	if (len != 0.0f)
		res = v / len;
	return res;
}
Vec3 vec3_normalize_check(Vec3 v) {
	Vec3 res = {};
	r32  len = vec3_length(v);
	if (len != 0.0f)
		res = v / len;
	return res;
}
Vec4 vec4_normalize_check(Vec4 v) {
	Vec4 res = {};
	r32  len = vec4_length(v);
	if (len != 0.0f)
		res = v * (1.0f / len);
	return res;
}

// no checking for division by zero
Vec2 vec2_normalize(Vec2 v) {
	Vec2 res = {};
	r32  len = vec2_length(v);
	assert(len != 0);
	res = v / len;
	return res;
}
Vec3 vec3_normalize(Vec3 v) {
	Vec3 res = {};
	r32  len = vec3_length(v);
	assert(len != 0);
	res = v / len;
	return res;
}
Vec4 vec4_normalize(Vec4 v) {
	Vec4 res = {};
	r32  len = vec4_length(v);
	assert(len != 0);
	res = v * (1.0f / len);
	return res;
}

//
//
//

r32 vec2_angle_between(Vec2 a, Vec2 b) {
	r32 dot = Clamp(-1.0f, 1.0f, vec2_dot(a, b));
	return acosf(dot);
}
r32 vec2_angle_between_normalize(Vec2 a, Vec2 b) {
	a = vec2_normalize(a);
	b = vec2_normalize(b);
	return vec2_angle_between(a, b);
}
r32 vec3_angle_between(Vec3 a, Vec3 b) {
	r32 dot = Clamp(-1.0f, 1.0f, vec3_dot(a, b));
	return acosf(dot);
}
r32 vec3_angle_between_normalize(Vec3 a, Vec3 b) {
	a = vec3_normalize(a);
	b = vec3_normalize(b);
	return vec3_angle_between(a, b);
}

r32 vec2_signed_angle_between(Vec2 a, Vec2 b) {
	r32 dot = Clamp(-1.0f, 1.0f, vec2_dot(a, b));
	r32 angle = acosf(dot);
	r32 cross = a.x * b.y - a.y * b.x;
	if (cross < 0) {
		angle = -angle;
	}
	return angle;
}
r32 vec2_signed_angle_between_normalize(Vec2 a, Vec2 b) {
	a = vec2_normalize(a);
	b = vec2_normalize(b);
	return vec2_signed_angle_between(a, b);
}
r32 vec3_signed_angle_between(Vec3 a, Vec3 b, Vec3 n) {
	r32 dot = Clamp(-1.0f, 1.0f, vec3_dot(a, b));
	r32 angle = acosf(dot);
	Vec3 cross = vec3_cross(a, b);
	if (vec3_dot(n, cross) < 0) {
		angle = -angle;
	}
	return angle;
}
r32 vec3_signed_angle_between_normalize(Vec3 a, Vec3 b, Vec3 n) {
	a = vec3_normalize(a);
	b = vec3_normalize(b);
	n = vec3_normalize(n);
	return vec3_signed_angle_between(a, b, n);
}

//
//
//

bool vec2_equals(Vec2 a, Vec2 b, r32 tolerance) {
	return RealEquals(a.x, b.x, tolerance) && RealEquals(a.y, b.y, tolerance);
}
bool vec3_equals(Vec3 a, Vec3 b, r32 tolerance) {
	return RealEquals(a.x, b.x, tolerance) && RealEquals(a.y, b.y, tolerance) && RealEquals(a.z, b.z, tolerance);
}
bool vec4_equals(Vec4 a, Vec4 b, r32 tolerance) {
	return RealEquals(a.x, b.x, tolerance) && RealEquals(a.y, b.y, tolerance) && RealEquals(a.z, b.z, tolerance) && RealEquals(a.w, b.w, tolerance);
}

//
//
//

r32 mm_rect_width(Mm_Rect m) {
	return m.max.x - m.min.x;
}

r32 mm_height(Mm_Rect m) {
	return m.max.y - m.min.y;
}

//
//
//

Mat3 mat3_identity() {
	Mat3 m;
	m.rows[0] = vec3(1.0f, 0.0f, 0.0f);
	m.rows[1] = vec3(0.0f, 1.0f, 0.0f);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 mat3_inverse(const Mat3 &mat) {
	Mat3 result;
	result.m2[0][0] = mat.m2[1][1] * mat.m2[2][2] - mat.m2[2][1] * mat.m2[1][2];
	result.m2[0][1] = mat.m2[0][2] * mat.m2[2][1] - mat.m2[0][1] * mat.m2[2][2];
	result.m2[0][2] = mat.m2[0][1] * mat.m2[1][2] - mat.m2[0][2] * mat.m2[1][1];
	result.m2[1][0] = mat.m2[1][2] * mat.m2[2][0] - mat.m2[1][0] * mat.m2[2][2];
	result.m2[1][1] = mat.m2[0][0] * mat.m2[2][2] - mat.m2[0][2] * mat.m2[2][0];
	result.m2[1][2] = mat.m2[1][0] * mat.m2[0][2] - mat.m2[0][0] * mat.m2[1][2];
	result.m2[2][0] = mat.m2[1][0] * mat.m2[2][1] - mat.m2[2][0] * mat.m2[1][1];
	result.m2[2][1] = mat.m2[2][0] * mat.m2[0][1] - mat.m2[0][0] * mat.m2[2][1];
	result.m2[2][2] = mat.m2[0][0] * mat.m2[1][1] - mat.m2[1][0] * mat.m2[0][1];

	r32 det = mat.m2[0][0] * result.m2[0][0] + mat.m2[0][1] * result.m2[1][0] + mat.m2[0][2] * result.m2[2][0];
	det /= det;
	for (int i = 0; i < 3; i++)
		result.rows[i] = result.rows[i] * det;
	return result;
}

Mat3 mat3_transpose(const Mat3 &m) {
	Mat3 res;
	res.rows[0] = vec3(m.m2[0][0], m.m2[1][0], m.m2[2][0]);
	res.rows[1] = vec3(m.m2[0][1], m.m2[1][1], m.m2[2][1]);
	res.rows[2] = vec3(m.m2[0][2], m.m2[1][2], m.m2[2][2]);
	return res;
}

Mat4 mat4_identity() {
	Mat4 m;
	m.rows[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	m.rows[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	m.rows[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_inverse(const Mat4 &mat) {
	Mat4 result;

	result.m[0] = mat.m[5] * mat.m[10] * mat.m[15] -
		mat.m[5] * mat.m[11] * mat.m[14] -
		mat.m[9] * mat.m[6] * mat.m[15] +
		mat.m[9] * mat.m[7] * mat.m[14] +
		mat.m[13] * mat.m[6] * mat.m[11] -
		mat.m[13] * mat.m[7] * mat.m[10];

	result.m[4] = -mat.m[4] * mat.m[10] * mat.m[15] +
		mat.m[4] * mat.m[11] * mat.m[14] +
		mat.m[8] * mat.m[6] * mat.m[15] -
		mat.m[8] * mat.m[7] * mat.m[14] -
		mat.m[12] * mat.m[6] * mat.m[11] +
		mat.m[12] * mat.m[7] * mat.m[10];

	result.m[8] = mat.m[4] * mat.m[9] * mat.m[15] -
		mat.m[4] * mat.m[11] * mat.m[13] -
		mat.m[8] * mat.m[5] * mat.m[15] +
		mat.m[8] * mat.m[7] * mat.m[13] +
		mat.m[12] * mat.m[5] * mat.m[11] -
		mat.m[12] * mat.m[7] * mat.m[9];

	result.m[12] = -mat.m[4] * mat.m[9] * mat.m[14] +
		mat.m[4] * mat.m[10] * mat.m[13] +
		mat.m[8] * mat.m[5] * mat.m[14] -
		mat.m[8] * mat.m[6] * mat.m[13] -
		mat.m[12] * mat.m[5] * mat.m[10] +
		mat.m[12] * mat.m[6] * mat.m[9];

	result.m[1] = -mat.m[1] * mat.m[10] * mat.m[15] +
		mat.m[1] * mat.m[11] * mat.m[14] +
		mat.m[9] * mat.m[2] * mat.m[15] -
		mat.m[9] * mat.m[3] * mat.m[14] -
		mat.m[13] * mat.m[2] * mat.m[11] +
		mat.m[13] * mat.m[3] * mat.m[10];

	result.m[5] = mat.m[0] * mat.m[10] * mat.m[15] -
		mat.m[0] * mat.m[11] * mat.m[14] -
		mat.m[8] * mat.m[2] * mat.m[15] +
		mat.m[8] * mat.m[3] * mat.m[14] +
		mat.m[12] * mat.m[2] * mat.m[11] -
		mat.m[12] * mat.m[3] * mat.m[10];

	result.m[9] = -mat.m[0] * mat.m[9] * mat.m[15] +
		mat.m[0] * mat.m[11] * mat.m[13] +
		mat.m[8] * mat.m[1] * mat.m[15] -
		mat.m[8] * mat.m[3] * mat.m[13] -
		mat.m[12] * mat.m[1] * mat.m[11] +
		mat.m[12] * mat.m[3] * mat.m[9];

	result.m[13] = mat.m[0] * mat.m[9] * mat.m[14] -
		mat.m[0] * mat.m[10] * mat.m[13] -
		mat.m[8] * mat.m[1] * mat.m[14] +
		mat.m[8] * mat.m[2] * mat.m[13] +
		mat.m[12] * mat.m[1] * mat.m[10] -
		mat.m[12] * mat.m[2] * mat.m[9];

	result.m[2] = mat.m[1] * mat.m[6] * mat.m[15] -
		mat.m[1] * mat.m[7] * mat.m[14] -
		mat.m[5] * mat.m[2] * mat.m[15] +
		mat.m[5] * mat.m[3] * mat.m[14] +
		mat.m[13] * mat.m[2] * mat.m[7] -
		mat.m[13] * mat.m[3] * mat.m[6];

	result.m[6] = -mat.m[0] * mat.m[6] * mat.m[15] +
		mat.m[0] * mat.m[7] * mat.m[14] +
		mat.m[4] * mat.m[2] * mat.m[15] -
		mat.m[4] * mat.m[3] * mat.m[14] -
		mat.m[12] * mat.m[2] * mat.m[7] +
		mat.m[12] * mat.m[3] * mat.m[6];

	result.m[10] = mat.m[0] * mat.m[5] * mat.m[15] -
		mat.m[0] * mat.m[7] * mat.m[13] -
		mat.m[4] * mat.m[1] * mat.m[15] +
		mat.m[4] * mat.m[3] * mat.m[13] +
		mat.m[12] * mat.m[1] * mat.m[7] -
		mat.m[12] * mat.m[3] * mat.m[5];

	result.m[14] = -mat.m[0] * mat.m[5] * mat.m[14] +
		mat.m[0] * mat.m[6] * mat.m[13] +
		mat.m[4] * mat.m[1] * mat.m[14] -
		mat.m[4] * mat.m[2] * mat.m[13] -
		mat.m[12] * mat.m[1] * mat.m[6] +
		mat.m[12] * mat.m[2] * mat.m[5];

	result.m[3] = -mat.m[1] * mat.m[6] * mat.m[11] +
		mat.m[1] * mat.m[7] * mat.m[10] +
		mat.m[5] * mat.m[2] * mat.m[11] -
		mat.m[5] * mat.m[3] * mat.m[10] -
		mat.m[9] * mat.m[2] * mat.m[7] +
		mat.m[9] * mat.m[3] * mat.m[6];

	result.m[7] = mat.m[0] * mat.m[6] * mat.m[11] -
		mat.m[0] * mat.m[7] * mat.m[10] -
		mat.m[4] * mat.m[2] * mat.m[11] +
		mat.m[4] * mat.m[3] * mat.m[10] +
		mat.m[8] * mat.m[2] * mat.m[7] -
		mat.m[8] * mat.m[3] * mat.m[6];

	result.m[11] = -mat.m[0] * mat.m[5] * mat.m[11] +
		mat.m[0] * mat.m[7] * mat.m[9] +
		mat.m[4] * mat.m[1] * mat.m[11] -
		mat.m[4] * mat.m[3] * mat.m[9] -
		mat.m[8] * mat.m[1] * mat.m[7] +
		mat.m[8] * mat.m[3] * mat.m[5];

	result.m[15] = mat.m[0] * mat.m[5] * mat.m[10] -
		mat.m[0] * mat.m[6] * mat.m[9] -
		mat.m[4] * mat.m[1] * mat.m[10] +
		mat.m[4] * mat.m[2] * mat.m[9] +
		mat.m[8] * mat.m[1] * mat.m[6] -
		mat.m[8] * mat.m[2] * mat.m[5];

	r32 det = mat.m[0] * result.m[0] + mat.m[1] * result.m[4] + mat.m[2] * result.m[8] + mat.m[3] * result.m[12];
	det     = 1.0f / det;
	for (int i = 0; i < 4; i++)
		result.rows[i] = result.rows[i] * det;
	return result;
}

Mat4 mat4_transpose(const Mat4 &m) {
	Mat4 res;
	res.rows[0] = vec4(m.m2[0][0], m.m2[1][0], m.m2[2][0], m.m2[3][0]);
	res.rows[1] = vec4(m.m2[0][1], m.m2[1][1], m.m2[2][1], m.m2[3][1]);
	res.rows[2] = vec4(m.m2[0][2], m.m2[1][2], m.m2[2][2], m.m2[3][2]);
	res.rows[3] = vec4(m.m2[0][3], m.m2[1][3], m.m2[2][3], m.m2[3][3]);
	return res;
}

//
//
//

Mat3 mat3_mul(const Mat3 &left, const Mat3 &right) {
	Mat3 res;
	Mat3 tras = mat3_transpose(right);

	res.m2[0][0] = vec3_dot(left.rows[0], tras.rows[0]);
	res.m2[0][1] = vec3_dot(left.rows[0], tras.rows[1]);
	res.m2[0][2] = vec3_dot(left.rows[0], tras.rows[2]);

	res.m2[1][0] = vec3_dot(left.rows[1], tras.rows[0]);
	res.m2[1][1] = vec3_dot(left.rows[1], tras.rows[1]);
	res.m2[1][2] = vec3_dot(left.rows[1], tras.rows[2]);

	res.m2[2][0] = vec3_dot(left.rows[2], tras.rows[0]);
	res.m2[2][1] = vec3_dot(left.rows[2], tras.rows[1]);
	res.m2[2][2] = vec3_dot(left.rows[2], tras.rows[2]);

	return res;
}

Vec3 mat3_vec3_mul(const Mat3 &mat, Vec3 vec) {
	Vec3 res;
	res.m[0] = vec3_dot(vec, mat.rows[0]);
	res.m[1] = vec3_dot(vec, mat.rows[1]);
	res.m[2] = vec3_dot(vec, mat.rows[2]);
	return res;
}

Mat4 mat4_mul(const Mat4 &left, const Mat4 &right) {
	Mat4 res;
	Mat4 tras = mat4_transpose(right);

	res.m2[0][0] = vec4_dot(left.rows[0], tras.rows[0]);
	res.m2[0][1] = vec4_dot(left.rows[0], tras.rows[1]);
	res.m2[0][2] = vec4_dot(left.rows[0], tras.rows[2]);
	res.m2[0][3] = vec4_dot(left.rows[0], tras.rows[3]);

	res.m2[1][0] = vec4_dot(left.rows[1], tras.rows[0]);
	res.m2[1][1] = vec4_dot(left.rows[1], tras.rows[1]);
	res.m2[1][2] = vec4_dot(left.rows[1], tras.rows[2]);
	res.m2[1][3] = vec4_dot(left.rows[1], tras.rows[3]);

	res.m2[2][0] = vec4_dot(left.rows[2], tras.rows[0]);
	res.m2[2][1] = vec4_dot(left.rows[2], tras.rows[1]);
	res.m2[2][2] = vec4_dot(left.rows[2], tras.rows[2]);
	res.m2[2][3] = vec4_dot(left.rows[2], tras.rows[3]);

	res.m2[3][0] = vec4_dot(left.rows[3], tras.rows[0]);
	res.m2[3][1] = vec4_dot(left.rows[3], tras.rows[1]);
	res.m2[3][2] = vec4_dot(left.rows[3], tras.rows[2]);
	res.m2[3][3] = vec4_dot(left.rows[3], tras.rows[3]);

	return res;
}

Vec4 mat4_vec4_mul(const Mat4 &mat, Vec4 vec) {
	Vec4 res;
	res.m[0] = vec4_dot(vec, mat.rows[0]);
	res.m[1] = vec4_dot(vec, mat.rows[1]);
	res.m[2] = vec4_dot(vec, mat.rows[2]);
	res.m[3] = vec4_dot(vec, mat.rows[3]);
	return res;
}

//
//
//

Mat3 mat3_scalar(r32 S_1, r32 S_2) {
	Mat3 m;
	m.rows[0] = vec3(S_1, 0.0f, 0.0f);
	m.rows[1] = vec3(0.0f, S_2, 0.0f);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 mat3_scalar(Vec2 s) {
	return mat3_scalar(s.x, s.y);
}

Mat3 mat3_translation(r32 T_x, r32 T_y) {
	Mat3 m;
	m.rows[0] = vec3(1.0f, 0.0f, T_x);
	m.rows[1] = vec3(0.0f, 1.0f, T_y);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 mat3_translation(Vec2 t) {
	return mat3_translation(t.x, t.y);
}

Mat3 mat3_rotation(r32 angle) {
	Mat3 m;
	r32  c    = cosf(angle);
	r32  s    = sinf(angle);
	m.rows[0] = vec3(c, -s, 0.0f);
	m.rows[1] = vec3(s, c, 0.0f);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 mat3_lookat(Vec2 from, Vec2 to, Vec2 forward) {
	Vec2 dir       = vec2_normalize(to - from);
	r32  cos_theta = vec2_dot(dir, forward);
	r32  sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

	Mat3 m;
	m.rows[0] = vec3(cos_theta, -sin_theta, from.x);
	m.rows[1] = vec3(sin_theta, cos_theta, from.y);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_scalar(r32 S_1, r32 S_2, r32 S_3) {
	Mat4 m;
	m.rows[0] = vec4(S_1, 0.0f, 0.0f, 0.0f);
	m.rows[1] = vec4(0.0f, S_2, 0.0f, 0.0f);
	m.rows[2] = vec4(0.0f, 0.0f, S_3, 0.0f);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_scalar(Vec3 s) {
	return mat4_scalar(s.x, s.y, s.z);
}

Mat4 mat4_translation(r32 T_x, r32 T_y, r32 T_z) {
	Mat4 m;
	m.rows[0] = vec4(1.0f, 0.0f, 0.0f, T_x);
	m.rows[1] = vec4(0.0f, 1.0f, 0.0f, T_y);
	m.rows[2] = vec4(0.0f, 0.0f, 1.0f, T_z);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_translation(Vec3 t) {
	return mat4_translation(t.x, t.y, t.z);
}

Mat4 mat4_rotation_x(r32 angle) {
	Mat4 m;
	m.rows[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	m.rows[1] = vec4(0.0f, cosf(angle), -sinf(angle), 0.0f);
	m.rows[2] = vec4(0.0f, sinf(angle), cosf(angle), 0.0f);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_rotation_y(r32 angle) {
	Mat4 m;
	m.rows[0] = vec4(cosf(angle), 0.0f, -sinf(angle), 0.0f);
	m.rows[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	m.rows[2] = vec4(sinf(angle), 0.0f, cosf(angle), 0.0f);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_rotation_z(r32 angle) {
	Mat4 m;
	m.rows[0] = vec4(cosf(angle), -sinf(angle), 0.0f, 0.0f);
	m.rows[1] = vec4(sinf(angle), cosf(angle), 0.0f, 0.0f);
	m.rows[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_rotation(r32 x, r32 y, r32 z, r32 angle) {
	r32 c = cosf(angle);
	r32 s = sinf(angle);

	r32 x2 = x * x;
	r32 xy = x * y;
	r32 zx = x * z;
	r32 y2 = y * y;
	r32 yz = y * z;
	r32 z2 = z * z;

	Mat4 m;
	m.rows[0] = vec4(c + x2 * (1 - c), xy * (1 - c) - z * s, zx * (1 - c) + y * s, 0.0f);
	m.rows[1] = vec4(xy * (1 - c) + z * s, c + y2 * (1 - c), yz * (1 - c) - x * s, 0.0f);
	m.rows[2] = vec4(zx * (1 - c) - y * s, yz * (1 - c) + x * s, c + z2 * (1 - c), 0.0f);
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_rotation(Vec3 axis, r32 angle) {
	return mat4_rotation(axis.x, axis.y, axis.z, angle);
}

Mat4 mat4_lookat(Vec3 from, Vec3 to, Vec3 world_up) {
	Vec3 n = vec3_normalize(to - from);
	Vec3 u = vec3_normalize(vec3_cross(world_up, n));
	Vec3 v = vec3_cross(n, u);

	Mat4 result;
	result.rows[0] = vec4(u.x, n.x, v.x, from.x);
	result.rows[1] = vec4(u.y, n.y, v.y, from.y);
	result.rows[2] = vec4(u.z, n.z, v.z, from.z);
	result.rows[3] = vec4(0, 0, 0, 1);

	return result;
}

Mat4 mat4_ortho_gl(r32 l, r32 r, r32 t, r32 b, r32 n, r32 f) {
	Mat4 m;
	m.rows[0] = vec4(2 / (r - l), 0.0f, 0.0f, -(l + r) / (r - l));
	m.rows[1] = vec4(0.0f, 2 / (t - b), 0.0f, -(t + b) / (t - b));
	m.rows[2] = vec4(0.0f, 0.0f, -2 / (f - n), -(f + n) / (f - n));
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_perspective_gl(r32 fov, r32 aspect_ratio, r32 n, r32 f) {
	r32  cot = 1.0f / tanf(fov * 0.5f);
	r32  fpn = f + n;
	r32  fmn = f - n;
	Mat4 m;
	m.rows[0] = vec4(cot / aspect_ratio, 0.0f, 0.0f, 0.0f);
	m.rows[1] = vec4(0.0f, cot, 0.0f, 0.0f);
	m.rows[2] = vec4(0.0f, 0.0f, -fpn / fmn, -2.0f * f * n / fmn);
	m.rows[3] = vec4(0.0f, 0.0f, -1.0f, 0.0f);
	return m;
}

Mat4 mat4_ortho_dx(r32 l, r32 r, r32 t, r32 b, r32 n, r32 f) {
	Mat4 m;
	m.rows[0] = vec4(2 / (r - l), 0.0f, 0.0f, -(l + r) / (r - l));
	m.rows[1] = vec4(0.0f, 2 / (t - b), 0.0f, -(t + b) / (t - b));
	m.rows[2] = vec4(0.0f, 0.0f, -1 / (f - n), -n / (f - n));
	m.rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return m;
}

Mat4 mat4_perspective_dx(r32 fov, r32 aspect_ratio, r32 n, r32 f) {
	r32  cot = 1.0f / tanf(fov * 0.5f);
	r32  fmn = f - n;
	Mat4 m;
	m.rows[0] = vec4(cot / aspect_ratio, 0.0f, 0.0f, 0.0f);
	m.rows[1] = vec4(0.0f, cot, 0.0f, 0.0f);
	m.rows[2] = vec4(0.0f, 0.0f, -f / fmn, -f * n / fmn);
	m.rows[3] = vec4(0.0f, 0.0f, -1.0f, 0.0f);
	return m;
}

Vec3 mat4_right(const Mat4 &m) {
	Vec3 v;
	v.x = m.m2[0][0];
	v.y = m.m2[1][0];
	v.z = m.m2[2][0];
	return v;
}

Vec3 mat4_up(const Mat4 &m) {
	Vec3 v;
	v.x = m.m2[0][2];
	v.y = m.m2[1][2];
	v.z = m.m2[2][2];
	return v;
}

Vec3 mat4_forward(const Mat4 &m) {
	Vec3 v;
	v.x = m.m2[0][1];
	v.y = m.m2[1][1];
	v.z = m.m2[2][1];
	return v;
}

bool mat3_equals(const Mat3 &a, const Mat3 &b, r32 tolerance) {
	return vec3_equals(a.rows[0], b.rows[0], tolerance) &&
		vec3_equals(a.rows[1], b.rows[1], tolerance) &&
		vec3_equals(a.rows[2], b.rows[2], tolerance);
}

bool mat4_equals(const Mat4 &a, const Mat4 &b, r32 tolerance) {
	return vec4_equals(a.rows[0], b.rows[0], tolerance) &&
		vec4_equals(a.rows[1], b.rows[1], tolerance) &&
		vec4_equals(a.rows[2], b.rows[2], tolerance) &&
		vec4_equals(a.rows[3], b.rows[3], tolerance);
}

Mat3 mat4_to_mat3(const Mat4 &mat) {
	Mat3 result;
	result.rows[0] = mat.rows[0].xyz;
	result.rows[1] = mat.rows[1].xyz;
	result.rows[2] = mat.rows[2].xyz;
	return result;
}

Mat4 mat3_to_mat4(const Mat3 &mat) {
	Mat4 result;
	result.rows[0] = vec4(mat.rows[0], 0);
	result.rows[1] = vec4(mat.rows[1], 0);
	result.rows[2] = vec4(mat.rows[2], 0);
	result.rows[3] = vec4(0, 0, 0, 1);
	return result;
}

//
//
//

Quat quat_identity() {
	return quat(0, 0, 0, 1);
}

r32 quat_dot(Quat q1, Quat q2) {
	return q1.real * q2.real + q1.i * q2.i + q1.j * q2.j + q1.k * q2.k;
}

r32 quat_length(Quat q) {
	return sqrtf(quat_dot(q, q));
}

Quat quat_normalize(Quat q) {
	r32 len = quat_length(q);
	assert(len != 0);
	return q * (1.0f / len);
}

Quat quat_conjugate(Quat q) {
	return quat(-q.i, -q.j, -q.k, q.real);
}

Quat quat_mul(Quat q1, Quat q2) {
	r32 a = q1.w;
	r32 b = q1.x;
	r32 c = q1.y;
	r32 d = q1.z;

	r32 e = q2.w;
	r32 f = q2.x;
	r32 g = q2.y;
	r32 h = q2.z;

	Quat res;
	res.w = a * e - b * f - c * g - d * h;
	res.x = a * f + b * e + c * h - d * g;
	res.y = a * g - b * h + c * e + d * f;
	res.z = a * h + b * g - c * f + d * e;
	return res;
}

Vec3 quat_rotate(Quat q, Vec3 v) {
	Quat p   = quat(v.x, v.y, v.z, 0);
	Quat qc  = quat_conjugate(q);
	Quat res = (q * p * qc);
	return vec3(res.x, res.y, res.z);
}

Quat quat_angle_axis(Vec3 axis, r32 angle) {
	r32 r = cosf(angle * 0.5f);
	r32 s = sinf(angle * 0.5f);
	r32 i = s * axis.x;
	r32 j = s * axis.y;
	r32 k = s * axis.z;
	return quat(i, j, k, r);
}

Quat quat_angle_axis_normalize(Vec3 axis, r32 angle) {
	return quat_angle_axis(vec3_normalize(axis), angle);
}

void quat_get_angle_axis(Quat q, r32 *angle, Vec3 *axis) {
	r32 len = sqrtf(q.i * q.i + q.j * q.j + q.k * q.k);
	if (len) {
		*angle  = 2.0f * atan2f(len, q.real);
		len     = 1.0f / len;
		axis->x = q.i * len;
		axis->y = q.j * len;
		axis->z = q.k * len;
	} else {
		// degenerate case, unit quaternion
		*angle = 0;
		*axis  = WORLD_FORWARD;
	}
}

Vec3 quat_get_axis(Quat q) {
	Vec3 axis;
	r32  angle;
	quat_get_angle_axis(q, &angle, &axis);
	return axis;
}

r32 quat_get_angle(Quat q) {
	Vec3 axis;
	r32  angle;
	quat_get_angle_axis(q, &angle, &axis);
	return angle;
}

Quat quat_mat4(const Mat4 &m) {
	Quat q;
	r32  trace = m.m2[0][0] + m.m2[1][1] + m.m2[2][2];
	if (trace > 0.0f) {
		r32 s = 0.5f / sqrtf(trace + 1.0f);
		q.w   = 0.25f / s;
		q.x   = (m.m2[2][1] - m.m2[1][2]) * s;
		q.y   = (m.m2[0][2] - m.m2[2][0]) * s;
		q.z   = (m.m2[1][0] - m.m2[0][1]) * s;
	} else {
		if (m.m2[0][0] > m.m2[1][1] && m.m2[0][0] > m.m2[2][2]) {
			r32 s = 2.0f * sqrtf(1.0f + m.m2[0][0] - m.m2[1][1] - m.m2[2][2]);
			q.w   = (m.m2[2][1] - m.m2[1][2]) / s;
			q.x   = 0.25f * s;
			q.y   = (m.m2[0][1] + m.m2[1][0]) / s;
			q.z   = (m.m2[0][2] + m.m2[2][0]) / s;
		} else if (m.m2[1][1] > m.m2[2][2]) {
			r32 s = 2.0f * sqrtf(1.0f + m.m2[1][1] - m.m2[0][0] - m.m2[2][2]);
			q.w   = (m.m2[0][2] - m.m2[2][0]) / s;
			q.x   = (m.m2[0][1] + m.m2[1][0]) / s;
			q.y   = 0.25f * s;
			q.z   = (m.m2[1][2] + m.m2[2][1]) / s;
		} else {
			r32 s = 2.0f * sqrtf(1.0f + m.m2[2][2] - m.m2[0][0] - m.m2[1][1]);
			q.w   = (m.m2[1][0] - m.m2[0][1]) / s;
			q.x   = (m.m2[0][2] + m.m2[2][0]) / s;
			q.y   = (m.m2[1][2] + m.m2[2][1]) / s;
			q.z   = 0.25f * s;
		}
	}
	return quat_normalize(q);
}

Quat quat_mat4_normalize(const Mat4 &m) {
	Mat4 nm;
	nm.rows[0] = vec4(vec3_normalize(m.rows[0].xyz), m.rows[0].w);
	nm.rows[1] = vec4(vec3_normalize(m.rows[1].xyz), m.rows[1].w);
	nm.rows[2] = vec4(vec3_normalize(m.rows[2].xyz), m.rows[2].w);
	nm.rows[3] = vec4(vec3_normalize(m.rows[3].xyz), m.rows[3].w);
	return quat_mat4(nm);
}

Mat4 quat_get_mat4(Quat q) {
	r32 i = q.i;
	r32 j = q.j;
	r32 k = q.k;
	r32 r = q.real;

	r32 ii = i * i;
	r32 jj = j * j;
	r32 kk = k * k;

	r32 ij = i * j;
	r32 jk = j * k;
	r32 kr = k * r;
	r32 jr = j * r;
	r32 ir = i * r;
	r32 ik = i * k;

	Mat4 m;

	m.m2[0][0] = 1 - 2 * (jj + kk);
	m.m2[0][1] = 2 * (ij - kr);
	m.m2[0][2] = 2 * (ik + jr);
	m.m2[0][3] = 0;

	m.m2[1][0] = 2 * (ij + kr);
	m.m2[1][1] = 1 - 2 * (ii + kk);
	m.m2[1][2] = 2 * (jk - ir);
	m.m2[1][3] = 0;

	m.m2[2][0] = 2 * (ik - jr);
	m.m2[2][1] = 2 * (jk + ir);
	m.m2[2][2] = 1 - 2 * (ii + jj);
	m.m2[2][3] = 0;

	m.m2[3][0] = 0;
	m.m2[3][1] = 0;
	m.m2[3][2] = 0;
	m.m2[3][3] = 1;

	return m;
}

Vec3 quat_forward(Quat q) {
	Vec3 forward;
	forward.x = 2 * (q.i * q.j - q.k * q.real);
	forward.y = 1 - 2 * (q.i * q.i + q.k * q.k);
	forward.z = 2 * (q.j * q.k + q.i * q.real);
	return vec3_normalize(forward);
}

Vec3 quat_right(Quat q) {
	Vec3 right;
	right.x = 1 - 2 * (q.j * q.j + q.k * q.k);
	right.y = 2 * (q.i * q.j + q.k * q.real);
	right.z = 2 * (q.i * q.k - q.j * q.real);
	return vec3_normalize(right);
}

Vec3 quat_up(Quat q) {
	Vec3 up;
	up.x = 2 * (q.i * q.k + q.j * q.real);
	up.y = 2 * (q.j * q.k - q.i * q.real);
	up.z = 1 - 2 * (q.i * q.i + q.j * q.j);
	return vec3_normalize(up);
}

Quat quat_euler_angles(r32 pitch, r32 yaw, r32 roll) {
	r32 cy = cosf(roll * 0.5f);
	r32 sy = sinf(roll * 0.5f);
	r32 cp = cosf(yaw * 0.5f);
	r32 sp = sinf(yaw * 0.5f);
	r32 cr = cosf(pitch * 0.5f);
	r32 sr = sinf(pitch * 0.5f);

	Quat q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * cp * sr - sy * sp * cr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = sy * cp * cr - cy * sp * sr;
	return q;
}

Quat quat_euler_angles(Vec3 euler) {
	return quat_euler_angles(euler.x, euler.y, euler.z);
}

Vec3 quat_get_euler_angles(Quat q) {
	Vec3 angles;

	r32 sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
	r32 cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	angles.z      = atan2f(sinr_cosp, cosr_cosp);

	r32 sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (fabsf(sinp) >= 1.0f) {
		// use 90 degrees if out of range
		angles.x = copysignf(MATH_PI / 2, sinp);
	} else {
		angles.x = asinf(sinp);
	}

	r32 siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	r32 cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	angles.y      = atan2f(siny_cosp, cosy_cosp);

	return angles;
}

Quat quat_between(Vec3 from, Vec3 to) {
	Quat q;
	q.v4.w   = 1.0f + vec3_dot(from, to);

	if (q.real) {
		q.v4.xyz = vec3_cross(from, to);
	} else {
		q.v4.xyz = fabsf(from.x) > fabsf(from.z) ? vec3(-from.y, from.x, 0.f) : vec3(0.f, -from.z, from.y);
	}

	return quat_normalize(q);
}

Quat quat_between(Quat a, Quat b) {
	Quat t = quat_conjugate(a);
	t      = t * (1.0f / quat_dot(t, t));
	return quat_mul(t, b);
}

Quat quat_lookat(Vec3 from, Vec3 to, Vec3 world_forward) {
	Vec3 dir = to - from;
	return quat_between(world_forward, dir);
}

bool quat_equals(Quat a, Quat b, r32 tolerance) {
	bool x = RealEquals(a.x, b.x, tolerance);
	bool y = RealEquals(a.y, b.y, tolerance);
	bool z = RealEquals(a.z, b.z, tolerance);
	bool w = RealEquals(a.w, b.w, tolerance);
	return x && y && z && w;
}

//
//
//

Color3 linear_to_srgb(Color3 color) {
	Color3 res;
	res.x = sqrtf(color.x);
	res.y = sqrtf(color.y);
	res.z = sqrtf(color.z);
	return res;
}

Color4 linear_to_srgb(Color4 color) {
	Color4 res;
	res.xyz = linear_to_srgb(color.xyz);
	res.w   = color.w;
	return res;
}

Color3 linear_to_srgb(Color3 color, r32 gamma) {
	r32    igamma = 1.0f / gamma;
	Color3 res;
	res.x = powf(color.x, igamma);
	res.y = powf(color.y, igamma);
	res.z = powf(color.z, igamma);
	return res;
}

Color4 linear_to_srgb(Color4 color, r32 gamma) {
	Color4 res;
	res.xyz = linear_to_srgb(color.xyz, gamma);
	res.w   = color.w;
	return res;
}

Color3 srgb_to_linear(Color3 color) {
	Color3 res;
	res.x = color.x * color.x;
	res.y = color.y * color.y;
	res.z = color.z * color.z;
	return res;
}

Color4 srgb_to_linear(Color4 color) {
	Color4 res;
	res.xyz = srgb_to_linear(color.xyz);
	res.w   = color.w;
	return res;
}

Color3 srgb_to_linear(Color3 color, r32 gamma) {
	Color3 res;
	res.x = powf(color.x, gamma);
	res.y = powf(color.y, gamma);
	res.z = powf(color.z, gamma);
	return res;
}

Color4 srgb_to_linear(Color4 color, r32 gamma) {
	Color4 res;
	res.xyz = srgb_to_linear(color.xyz, gamma);
	res.w   = color.w;
	return res;
}

Colorh color4_to_hex(Color4 v) {
	u8 r = static_cast<u8>(255.0f * v.x);
	u8 g = static_cast<u8>(255.0f * v.y);
	u8 b = static_cast<u8>(255.0f * v.z);
	u8 a = static_cast<u8>(255.0f * v.w);
	return colorh(r, g, b, a);
}

Color4 hex_to_color4(Colorh c) {
	r32 div = 1.0f / 255.0f;
	r32 r   = static_cast<r32>(c.r) * div;
	r32 g   = static_cast<r32>(c.g) * div;
	r32 b   = static_cast<r32>(c.b) * div;
	r32 a   = static_cast<r32>(c.a) * div;
	return vec4(r, g, b, a);
}

Color3 hex_to_color3(Colorh c) {
	r32 div = 1.0f / 255.0f;
	r32 r   = static_cast<r32>(c.r) * div;
	r32 g   = static_cast<r32>(c.g) * div;
	r32 b   = static_cast<r32>(c.b) * div;
	return vec3(r, g, b);
}

// http://en.wikipedia.org/wiki/HSL_and_HSV
Color3 hsv_to_rgb(Color3 col) {
	Color3 res;

	r32 h = col.x;
	r32 s = col.y;
	r32 v = col.z;

	if (s == 0.0f) {
		// gray
		res.x = res.y = res.z = v;
		return res;
	}

	h       = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i) {
		case 0: res = vec3(v, t, p); break;
		case 1: res = vec3(q, v, p); break;
		case 2: res = vec3(p, v, t); break;
		case 3: res = vec3(p, q, v); break;
		case 4: res = vec3(t, p, v); break;
		case 5:
		default: res = vec3(v, p, q); break;
	}

	return res;
}

// http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
Color3 rgb_to_hsv(Color3 c) {
	r32 r = c.x;
	r32 g = c.y;
	r32 b = c.z;

	r32 k = 0.f;
	if (g < b) {
		auto t = b;
		b      = g;
		g      = t;
		k      = -1.f;
	}
	if (r < g) {
		auto t = g;
		g      = r;
		r      = t;
		k      = -2.f / 6.f - k;
	}

	Color3 res;
	r32    chroma = r - (g < b ? g : b);
	res.x         = fabsf(k + (g - b) / (6.f * chroma + 1e-20f));
	res.y         = chroma / (r + 1e-20f);
	res.z         = r;
	return res;
}

Color4 hsv_to_rgb(Color4 c) {
	return vec4(hsv_to_rgb(c.xyz), c.w);
}

Color4 rgb_to_hsv(Color4 c) {
	return vec4(rgb_to_hsv(c.xyz), c.w);
}

//
//
//

Vec2 vec2_clamp(Vec2 min, Vec2 max, Vec2 v) {
	v.x = Clamp(min.x, max.x, v.x);
	v.y = Clamp(min.y, max.y, v.y);
	return v;
}

Vec3 vec3_clamp(Vec3 min, Vec3 max, Vec3 v) {
	v.x = Clamp(min.x, max.x, v.x);
	v.y = Clamp(min.y, max.y, v.y);
	v.z = Clamp(min.z, max.z, v.z);
	return v;
}

Vec4 vec4_clamp(Vec4 min, Vec4 max, Vec4 v) {
	v.x = Clamp(min.x, max.x, v.x);
	v.y = Clamp(min.y, max.y, v.y);
	v.z = Clamp(min.z, max.z, v.z);
	v.w = Clamp(min.w, max.w, v.w);
	return v;
}

//
//
//

Mat4 lerp(Mat4 &from, Mat4 &to, r32 t) {
	Mat4 res;
	res.rows[0] = lerp(from.rows[0], to.rows[0], t);
	res.rows[1] = lerp(from.rows[1], to.rows[1], t);
	res.rows[2] = lerp(from.rows[2], to.rows[2], t);
	res.rows[3] = lerp(from.rows[3], to.rows[3], t);
	return res;
}

Vec2 slerp(Vec2 from, Vec2 to, r32 t) {
	return slerp(from, to, vec2_angle_between(from, to), t);
}

Vec3 slerp(Vec3 from, Vec3 to, r32 t) {
	return slerp(from, to, vec3_angle_between(from, to), t);
}

Quat slerp(Quat from, Quat to, r32 t) {
	r32 dot = Clamp(-1.0f, 1.0f, quat_dot(from, to));

	// use shorter path
	if (dot < 0.0f) {
		to  = -to;
		dot = -dot;
	}

	if (dot > 0.9999f) {
		Quat result = lerp(from, to, t);
		return result;
	}

	r32 theta_0     = acosf(dot);
	r32 theta       = theta_0 * t;
	r32 sin_theta   = sinf(theta);
	r32 sin_theta_0 = sinf(theta_0);

	r32 s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
	r32 s1 = sin_theta / sin_theta_0;

	return (s0 * from) + (s1 * to);
}

r32 step(r32 edge, r32 val) {
	return val < edge ? 0.0f : 1.0f;
}

Vec2 step(Vec2 edge, Vec2 val) {
	Vec2 res;
	res.x = step(edge.x, val.x);
	res.y = step(edge.y, val.y);
	return res;
}

Vec3 step(Vec3 edge, Vec3 val) {
	Vec3 res;
	res.x = step(edge.x, val.x);
	res.y = step(edge.y, val.y);
	res.z = step(edge.z, val.z);
	return res;
}

Vec4 step(Vec4 edge, Vec4 val) {
	Vec4 res;
	res.x = step(edge.x, val.x);
	res.y = step(edge.y, val.y);
	res.z = step(edge.z, val.z);
	res.w = step(edge.w, val.w);
	return res;
}

Quat step(Quat edge, Quat val) {
	Quat res;
	res.v4 = step(edge.v4, val.v4);
	return res;
}

r32 smoothstep(r32 a, r32 b, r32 x) {
	x = Clamp(0.0f, 1.0f, (x - a) / (b - a));
	return x * x * (3 - 2 * x);
}

r32 smoothstep(Vec2 a, Vec2 b, Vec2 v) {
	auto x = Clamp(0.0f, 1.0f, vec2_distance(a, v) / vec2_distance(a, b));
	return x * x * (3 - 2 * x);
}

r32 smoothstep(Vec3 a, Vec3 b, Vec3 v) {
	r32 div_distance = vec3_distance(a, b);
	if (div_distance) {
		auto x = Clamp(0.0f, 1.0f, vec3_distance(a, v) / div_distance);
		return x * x * (3 - 2 * x);
	}
	return 1;
}

r32 smoothstep(Vec4 a, Vec4 b, Vec4 v) {
	auto x = Clamp(0.0f, 1.0f, vec4_distance(a, v) / vec4_distance(a, b));
	return x * x * (3 - 2 * x);
}

r32 smoothstep(Quat a, Quat b, Quat v) {
	auto x = Clamp(0.0f, 1.0f, vec4_distance(a.v4, v.v4) / vec4_distance(a.v4, b.v4));
	return x * x * (3 - 2 * x);
}

r32 inverse_smoothstep(r32 x) {
	return 0.5f - sinf(asinf(1.0f - 2.0f * x) / 3.0f);
}

Vec2 move_toward(Vec2 from, Vec2 to, r32 factor) {
	if (factor) {
		Vec2 direction = to - from;
		r32 distance = vec2_length(direction);

		if (distance < factor) {
			return to;
		}

		r32 t = factor / distance;

		return lerp(to, from, t);
	}

	return from;
}

Vec3 move_toward(Vec3 from, Vec3 to, r32 factor) {
	if (factor) {
		Vec3 direction = to - from;
		r32 distance = vec3_length(direction);

		if (distance < factor) {
			return to;
		}

		r32 t = factor / distance;

		return lerp(from, to, t);
	}

	return from;
}

Vec4 move_toward(Vec4 from, Vec4 to, r32 factor) {
	if (factor) {
		Vec4 direction = to - from;
		r32 distance = vec4_length(direction);

		if (distance < factor) {
			return to;
		}

		r32 t = factor / distance;

		return lerp(to, from, t);
	}

	return from;
}

//
//
//

Vec2 rotate_around(Vec2 point, Vec2 center, r32 angle) {
	r32  c = cosf(angle);
	r32  s = sinf(angle);
	Vec2 res;
	Vec2 p = point - center;
	res.x  = p.x * c - p.y * s;
	res.y  = p.x * s + p.y * c;
	res += center;
	return res;
}

Quat rotate_toward(Quat from, Quat to, r32 max_angle) {
	if (max_angle) {
		r32 dot = Clamp(-1.0f, 1.0f, quat_dot(from, to));

		// use shorter path
		if (dot < 0.0f) {
			to  = -to;
			dot = -dot;
		}

		r32 theta_0 = acosf(dot);

		if (theta_0 < max_angle) {
			return to;
		}

		r32 t = max_angle / theta_0;

		theta_0			= max_angle;
		r32 theta       = theta_0 * t;
		r32 sin_theta   = sinf(theta);
		r32 sin_theta_0 = sinf(theta_0);

		r32 s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
		r32 s1 = sin_theta / sin_theta_0;

		return (s0 * from) + (s1 * to);
	} else {
		return from;
	}
}

//
//
//

bool point_inside_rect(Vec2 point, Mm_Rect rect) {
	if (point.x < rect.min.x || point.y < rect.min.y) return false;
	if (point.x > rect.max.x || point.y > rect.max.y) return false;
	return true;
}

bool aabb_vs_aabb(const Mm_Rect &a, const Mm_Rect &b) {
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	return true;
}

bool quad_vs_quad_sat(Quad &a, Quad &b) {
	auto quad_a = &a;
	auto quad_b = &b;

	for (int quad_index = 0; quad_index < 2; ++quad_index) {
		for (int edge_index = 0; edge_index < 4; ++edge_index) {
			r32 min_proj_a = INFINITY, max_proj_a = -INFINITY;
			r32 min_proj_b = INFINITY, max_proj_b = -INFINITY;

			r32 dot;
			Vec2 normal = quad_a->normals[edge_index];

			for (int p_index = 0; p_index < 4; ++p_index) {
				dot = vec2_dot(normal, quad_a->positions[p_index]);
				min_proj_a = GetMinValue(min_proj_a, dot);
				max_proj_a = GetMaxValue(max_proj_a, dot);
			}

			for (int p_index = 0; p_index < 4; ++p_index) {
				dot = vec2_dot(normal, quad_b->positions[p_index]);
				min_proj_b = GetMinValue(min_proj_b, dot);
				max_proj_b = GetMaxValue(max_proj_b, dot);
			}

			if (!(max_proj_b >= min_proj_a && max_proj_a >= min_proj_b))
				return false;
		}
		
		auto temp = quad_a;
		quad_a = quad_b;
		quad_b = temp;
	}

	return true;
}

bool ray_vs_aabb(Vec2 origin, Vec2 direction, const Mm_Rect &rect, Ray_Hit *hit) {
	Vec2 i_direction;
	i_direction.x = 1.0f / direction.x;
	i_direction.y = 1.0f / direction.y;
	Vec2 near_t = vec2_hadamard(rect.min - origin, i_direction);
	Vec2 far_t  = vec2_hadamard(rect.max - origin, i_direction);

	r32 temp;

	if (near_t.x > far_t.x) {
		temp = near_t.x;
		near_t.x = far_t.x;
		far_t.x = temp;
	}
	if (near_t.y > far_t.y) {
		temp = near_t.y;
		near_t.y = far_t.y;
		far_t.y = temp;
	}

	if (near_t.x > far_t.y || near_t.y > far_t.x) {
		// No intersection
		return false;
	}

	r32 hit_near_t = GetMaxValue(near_t.x, near_t.y);
	r32 hit_far_t  = GetMinValue(far_t.x, far_t.y);

	if (hit_far_t < 0) {
		// Collision in negative direction
		return false;
	}

	hit->point = origin + hit_near_t * direction;

	if (near_t.x > near_t.y) {
		hit->normal = vec2(-Sgn(direction.x), 0);
	} else if (near_t.x < near_t.y) {
		hit->normal = vec2(0, -Sgn(direction.y));
	} else {
		constexpr r32 root_two = 1.4142135623730950488016887242097f;
		hit->normal = vec2(-Sgn(direction.x) * root_two, -Sgn(direction.y) * root_two);
	}

	hit->t = hit_near_t;

	return true;
}

bool ray_vs_line(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, Ray_Hit* hit) {
	r32 dx1 = p1.x - q1.x;
	r32 dy1 = p1.y - q1.y;
	r32 dx2 = p2.x - q2.x;
	r32 dy2 = p2.y - q2.y;

	r32 d = dx1 * dy2 - dy1 * dx2;

	if (d) {
		r32 n2 = -dx1 * (p1.y - p2.y) + dy1 * (p1.x - p2.x);
		r32 u = n2 / d;

		if (u >= 0 && u <= 1) {
			r32 n = (p1.x - p2.x) * dy2 - (p1.y - p2.y) * dx2;
			hit->t = n / d;

			hit->point.x = p2.x - u * dx2;
			hit->point.y = p2.y - u * dy2;

			Vec2 normal;
			normal.x = dy2;
			normal.y = -dx2;
			hit->normal = vec2_normalize(normal);
			return true;
		}
	}

	return false;
}