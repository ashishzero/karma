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

r32 vec2_determinant(Vec2 a, Vec2 b) {
	return (a.x * b.y) - (a.y * b.x);
}

Vec3 vec2_cross(Vec2 a, Vec2 b) {
	Vec3 res;
	res.x = 0;
	res.y = 0;
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
	Vec3 res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}

Vec2 vec2_triple_product(Vec2 a, Vec2 b, Vec2 c) {
	r32 det = vec2_determinant(a, b);
	Vec2 res;
	res.x = -c.y * det;
	res.y = c.x * det;
	return res;
}

Vec3 vec3_triple_product(Vec3 a, Vec3 b, Vec3 c) {
	return vec3_cross(vec3_cross(a, b), c);
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
	r32 dot = mmclamp(-1.0f, 1.0f, vec2_dot(a, b));
	return acosf(dot);
}
r32 vec2_angle_between_normalize(Vec2 a, Vec2 b) {
	a = vec2_normalize(a);
	b = vec2_normalize(b);
	return vec2_angle_between(a, b);
}
r32 vec3_angle_between(Vec3 a, Vec3 b) {
	r32 dot = mmclamp(-1.0f, 1.0f, vec3_dot(a, b));
	return acosf(dot);
}
r32 vec3_angle_between_normalize(Vec3 a, Vec3 b) {
	a = vec3_normalize(a);
	b = vec3_normalize(b);
	return vec3_angle_between(a, b);
}

r32 vec2_signed_angle_between(Vec2 a, Vec2 b) {
	r32 dot = mmclamp(-1.0f, 1.0f, vec2_dot(a, b));
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
	r32 dot = mmclamp(-1.0f, 1.0f, vec3_dot(a, b));
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
	return real_equals(a.x, b.x, tolerance) && real_equals(a.y, b.y, tolerance);
}
bool vec3_equals(Vec3 a, Vec3 b, r32 tolerance) {
	return real_equals(a.x, b.x, tolerance) && real_equals(a.y, b.y, tolerance) && real_equals(a.z, b.z, tolerance);
}
bool vec4_equals(Vec4 a, Vec4 b, r32 tolerance) {
	return real_equals(a.x, b.x, tolerance) && real_equals(a.y, b.y, tolerance) && real_equals(a.z, b.z, tolerance) && real_equals(a.w, b.w, tolerance);
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

Mat2 mat2_identity() {
	Mat2 m;
	m.rows[0] = vec2(1.0f, 0.0f);
	m.rows[1] = vec2(0.0f, 1.0f);
	return m;
}

r32 mat2_det(const Mat2 &mat) {
	return mat.m2[0][0] * mat.m2[1][1] - mat.m2[0][1] * mat.m2[1][0];
}

Mat2 mat2_inverse(const Mat2 &mat) {
	r32 det = mat.m2[0][0] * mat.m2[1][1] - mat.m2[0][1] * mat.m2[1][0];
	det /= det;
	Mat2 res;
	res.m2[0][0] = mat.m2[1][1];
	res.m2[0][1] = -mat.m2[0][1];
	res.m2[1][0] = -mat.m2[1][0];
	res.m2[1][1] = mat.m2[0][0];
	res.m[0] *= det;
	res.m[1] *= det;
	res.m[2] *= det;
	res.m[3] *= det;
	return res;
}

Mat2 mat2_transpose(const Mat2 &m) {
	Mat2 t;
	t.m2[0][0] = m.m2[0][0];
	t.m2[0][1] = m.m2[1][0];
	t.m2[1][0] = m.m2[0][1];
	t.m2[1][1] = m.m2[1][1];
	return t;
}

Mat3 mat3_identity() {
	Mat3 m;
	m.rows[0] = vec3(1.0f, 0.0f, 0.0f);
	m.rows[1] = vec3(0.0f, 1.0f, 0.0f);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

r32 mat3_det(const Mat3 &mat) {
	r32 det = mat.m2[0][0] * (mat.m2[1][1] * mat.m2[2][2] - mat.m2[2][1] * mat.m2[1][2]) +
		mat.m2[0][1] * (mat.m2[1][2] * mat.m2[2][0] - mat.m2[1][0] * mat.m2[2][2]) +
		mat.m2[0][2] * (mat.m2[1][0] * mat.m2[2][1] - mat.m2[2][0] * mat.m2[1][1]);
	return det;
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
	for (s32 i = 0; i < 3; i++)
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

r32 mat4_det(const Mat4 &mat) {
	r32 m0 = mat.m[5] * mat.m[10] * mat.m[15] -
		mat.m[5] * mat.m[11] * mat.m[14] -
		mat.m[9] * mat.m[6] * mat.m[15] +
		mat.m[9] * mat.m[7] * mat.m[14] +
		mat.m[13] * mat.m[6] * mat.m[11] -
		mat.m[13] * mat.m[7] * mat.m[10];

	r32 m4 = -mat.m[4] * mat.m[10] * mat.m[15] +
		mat.m[4] * mat.m[11] * mat.m[14] +
		mat.m[8] * mat.m[6] * mat.m[15] -
		mat.m[8] * mat.m[7] * mat.m[14] -
		mat.m[12] * mat.m[6] * mat.m[11] +
		mat.m[12] * mat.m[7] * mat.m[10];

	r32 m8 = mat.m[4] * mat.m[9] * mat.m[15] -
		mat.m[4] * mat.m[11] * mat.m[13] -
		mat.m[8] * mat.m[5] * mat.m[15] +
		mat.m[8] * mat.m[7] * mat.m[13] +
		mat.m[12] * mat.m[5] * mat.m[11] -
		mat.m[12] * mat.m[7] * mat.m[9];

	r32 m12 = -mat.m[4] * mat.m[9] * mat.m[14] +
		mat.m[4] * mat.m[10] * mat.m[13] +
		mat.m[8] * mat.m[5] * mat.m[14] -
		mat.m[8] * mat.m[6] * mat.m[13] -
		mat.m[12] * mat.m[5] * mat.m[10] +
		mat.m[12] * mat.m[6] * mat.m[9];

	r32 det = mat.m[0] * m0 + mat.m[1] * m4 + mat.m[2] * m8 + mat.m[3] * m12;
	return det;
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
	det = 1.0f / det;
	for (s32 i = 0; i < 4; i++)
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

Mat2 mat2_mul(const Mat2 &left, const Mat2 &right) {
	Mat2 res;
	Mat2 tras = mat2_transpose(right);

	res.m2[0][0] = vec2_dot(left.rows[0], tras.rows[0]);
	res.m2[0][1] = vec2_dot(left.rows[0], tras.rows[1]);
	res.m2[1][0] = vec2_dot(left.rows[1], tras.rows[0]);
	res.m2[1][1] = vec2_dot(left.rows[1], tras.rows[1]);

	return res;
}

Vec2 mat2_vec2_mul(const Mat2 &mat, Vec2 vec) {
	Vec2 res;
	res.m[0] = vec2_dot(vec, mat.rows[0]);
	res.m[1] = vec2_dot(vec, mat.rows[1]);
	return res;
}

Vec2 vec2_mat2_mul(Vec2 vec, const Mat2 &mat) {
	Vec2 res;
	res.m[0] = vec2_dot(vec, vec2(mat.m2[0][0], mat.m2[1][0]));
	res.m[1] = vec2_dot(vec, vec2(mat.m2[0][1], mat.m2[1][1]));
	return res;
}

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

Vec2 mat3_vec2_mul(const Mat3 &mat, Vec2 v, r32 h) {
	Vec3 vec = vec3(v, h);
	Vec2 res;
	res.m[0] = vec3_dot(vec, mat.rows[0]);
	res.m[1] = vec3_dot(vec, mat.rows[1]);
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

Mat2 mat2_scalar(r32 x, r32 y) {
	Mat2 m;
	m.rows[0] = vec2(x, 0.0f);
	m.rows[1] = vec2(0.0f, y);
	return m;

}

Mat2 mat2_scalar(Vec2 s) {
	Mat2 m;
	m.rows[0] = vec2(s.x, 0.0f);
	m.rows[1] = vec2(0.0f, s.y);
	return m;
}

Mat2 mat2_rotation(r32 angle) {
	r32 c = cosf(angle);
	r32 s = sinf(angle);

	Mat2 mat;
	mat.rows[0] = vec2(c, -s);
	mat.rows[1] = vec2(s, c);
	return mat;
}

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
	r32  c = cosf(angle);
	r32  s = sinf(angle);
	m.rows[0] = vec3(c, -s, 0.0f);
	m.rows[1] = vec3(s, c, 0.0f);
	m.rows[2] = vec3(0.0f, 0.0f, 1.0f);
	return m;
}

Mat3 mat3_lookat(Vec2 from, Vec2 to, Vec2 forward) {
	Vec2 dir = vec2_normalize(to - from);
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

Mat2 mat3_to_mat2(const Mat3 &mat) {
	Mat2 result;
	result.rows[0] = mat.rows[0].xy;
	result.rows[1] = mat.rows[1].xy;
	return result;
}

Mat3 mat2_to_mat3(const Mat2 &mat) {
	Mat3 result;
	result.rows[0] = vec3(mat.rows[0], 0);
	result.rows[1] = vec3(mat.rows[1], 0);
	result.rows[2] = vec3(0, 0, 1);
	return result;
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
	Quat p = quat(v.x, v.y, v.z, 0);
	Quat qc = quat_conjugate(q);
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
		*angle = 2.0f * atan2f(len, q.real);
		len = 1.0f / len;
		axis->x = q.i * len;
		axis->y = q.j * len;
		axis->z = q.k * len;
	}
	else {
		// degenerate case, unit quaternion
		*angle = 0;
		*axis = WORLD_FORWARD;
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
		q.w = 0.25f / s;
		q.x = (m.m2[2][1] - m.m2[1][2]) * s;
		q.y = (m.m2[0][2] - m.m2[2][0]) * s;
		q.z = (m.m2[1][0] - m.m2[0][1]) * s;
	}
	else {
		if (m.m2[0][0] > m.m2[1][1] && m.m2[0][0] > m.m2[2][2]) {
			r32 s = 2.0f * sqrtf(1.0f + m.m2[0][0] - m.m2[1][1] - m.m2[2][2]);
			q.w = (m.m2[2][1] - m.m2[1][2]) / s;
			q.x = 0.25f * s;
			q.y = (m.m2[0][1] + m.m2[1][0]) / s;
			q.z = (m.m2[0][2] + m.m2[2][0]) / s;
		}
		else if (m.m2[1][1] > m.m2[2][2]) {
			r32 s = 2.0f * sqrtf(1.0f + m.m2[1][1] - m.m2[0][0] - m.m2[2][2]);
			q.w = (m.m2[0][2] - m.m2[2][0]) / s;
			q.x = (m.m2[0][1] + m.m2[1][0]) / s;
			q.y = 0.25f * s;
			q.z = (m.m2[1][2] + m.m2[2][1]) / s;
		}
		else {
			r32 s = 2.0f * sqrtf(1.0f + m.m2[2][2] - m.m2[0][0] - m.m2[1][1]);
			q.w = (m.m2[1][0] - m.m2[0][1]) / s;
			q.x = (m.m2[0][2] + m.m2[2][0]) / s;
			q.y = (m.m2[1][2] + m.m2[2][1]) / s;
			q.z = 0.25f * s;
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
	angles.z = atan2f(sinr_cosp, cosr_cosp);

	r32 sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (fabsf(sinp) >= 1.0f) {
		// use 90 degrees if out of range
		angles.x = copysignf(MATH_PI / 2, sinp);
	}
	else {
		angles.x = asinf(sinp);
	}

	r32 siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
	r32 cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	angles.y = atan2f(siny_cosp, cosy_cosp);

	return angles;
}

Quat quat_between(Vec3 from, Vec3 to) {
	Quat q;
	q.v4.w = 1.0f + vec3_dot(from, to);

	if (q.real) {
		q.v4.xyz = vec3_cross(from, to);
	}
	else {
		q.v4.xyz = fabsf(from.x) > fabsf(from.z) ? vec3(-from.y, from.x, 0.f) : vec3(0.f, -from.z, from.y);
	}

	return quat_normalize(q);
}

Quat quat_between(Quat a, Quat b) {
	Quat t = quat_conjugate(a);
	t = t * (1.0f / quat_dot(t, t));
	return quat_mul(t, b);
}

Quat quat_lookat(Vec3 from, Vec3 to, Vec3 world_forward) {
	Vec3 dir = to - from;
	return quat_between(world_forward, dir);
}

bool quat_equals(Quat a, Quat b, r32 tolerance) {
	bool x = real_equals(a.x, b.x, tolerance);
	bool y = real_equals(a.y, b.y, tolerance);
	bool z = real_equals(a.z, b.z, tolerance);
	bool w = real_equals(a.w, b.w, tolerance);
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
	res.w = color.w;
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
	res.w = color.w;
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
	res.w = color.w;
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
	res.w = color.w;
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
	r32 r = static_cast<r32>(c.r) * div;
	r32 g = static_cast<r32>(c.g) * div;
	r32 b = static_cast<r32>(c.b) * div;
	r32 a = static_cast<r32>(c.a) * div;
	return vec4(r, g, b, a);
}

Color3 hex_to_color3(Colorh c) {
	r32 div = 1.0f / 255.0f;
	r32 r = static_cast<r32>(c.r) * div;
	r32 g = static_cast<r32>(c.g) * div;
	r32 b = static_cast<r32>(c.b) * div;
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

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	s32   i = (int)h;
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
		b = g;
		g = t;
		k = -1.f;
	}
	if (r < g) {
		auto t = g;
		g = r;
		r = t;
		k = -2.f / 6.f - k;
	}

	Color3 res;
	r32    chroma = r - (g < b ? g : b);
	res.x = fabsf(k + (g - b) / (6.f * chroma + 1e-20f));
	res.y = chroma / (r + 1e-20f);
	res.z = r;
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
	v.x = mmclamp(min.x, max.x, v.x);
	v.y = mmclamp(min.y, max.y, v.y);
	return v;
}

Vec3 vec3_clamp(Vec3 min, Vec3 max, Vec3 v) {
	v.x = mmclamp(min.x, max.x, v.x);
	v.y = mmclamp(min.y, max.y, v.y);
	v.z = mmclamp(min.z, max.z, v.z);
	return v;
}

Vec4 vec4_clamp(Vec4 min, Vec4 max, Vec4 v) {
	v.x = mmclamp(min.x, max.x, v.x);
	v.y = mmclamp(min.y, max.y, v.y);
	v.z = mmclamp(min.z, max.z, v.z);
	v.w = mmclamp(min.w, max.w, v.w);
	return v;
}

//
//
//

Mat2 lerp(const Mat2 &from, const Mat2 &to, r32 t) {
	Mat2 res;
	res.rows[0] = lerp(from.rows[0], to.rows[0], t);
	res.rows[1] = lerp(from.rows[1], to.rows[1], t);
	return res;
}

Mat3 lerp(const Mat3 &from, const Mat3 &to, r32 t) {
	Mat3 res;
	res.rows[0] = lerp(from.rows[0], to.rows[0], t);
	res.rows[1] = lerp(from.rows[1], to.rows[1], t);
	res.rows[2] = lerp(from.rows[2], to.rows[2], t);
	return res;
}

Mat4 lerp(const Mat4 &from, const Mat4 &to, r32 t) {
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
	r32 dot = mmclamp(-1.0f, 1.0f, quat_dot(from, to));

	// use shorter path
	if (dot < 0.0f) {
		to = -to;
		dot = -dot;
	}

	if (dot > 0.9999f) {
		Quat result = lerp(from, to, t);
		return result;
	}

	r32 theta_0 = acosf(dot);
	r32 theta = theta_0 * t;
	r32 sin_theta = sinf(theta);
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
	x = mmclamp(0.0f, 1.0f, (x - a) / (b - a));
	return x * x * (3 - 2 * x);
}

r32 smoothstep(Vec2 a, Vec2 b, Vec2 v) {
	auto x = mmclamp(0.0f, 1.0f, vec2_distance(a, v) / vec2_distance(a, b));
	return x * x * (3 - 2 * x);
}

r32 smoothstep(Vec3 a, Vec3 b, Vec3 v) {
	r32 div_distance = vec3_distance(a, b);
	if (div_distance) {
		auto x = mmclamp(0.0f, 1.0f, vec3_distance(a, v) / div_distance);
		return x * x * (3 - 2 * x);
	}
	return 1;
}

r32 smoothstep(Vec4 a, Vec4 b, Vec4 v) {
	auto x = mmclamp(0.0f, 1.0f, vec4_distance(a, v) / vec4_distance(a, b));
	return x * x * (3 - 2 * x);
}

r32 smoothstep(Quat a, Quat b, Quat v) {
	auto x = mmclamp(0.0f, 1.0f, vec4_distance(a.v4, v.v4) / vec4_distance(a.v4, b.v4));
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
	res.x = p.x * c - p.y * s;
	res.y = p.x * s + p.y * c;
	res += center;
	return res;
}

Quat rotate_toward(Quat from, Quat to, r32 max_angle) {
	if (max_angle) {
		r32 dot = mmclamp(-1.0f, 1.0f, quat_dot(from, to));

		// use shorter path
		if (dot < 0.0f) {
			to = -to;
			dot = -dot;
		}

		r32 theta_0 = acosf(dot);

		if (theta_0 < max_angle) {
			return to;
		}

		r32 t = max_angle / theta_0;

		theta_0 = max_angle;
		r32 theta = theta_0 * t;
		r32 sin_theta = sinf(theta);
		r32 sin_theta_0 = sinf(theta_0);

		r32 s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
		r32 s1 = sin_theta / sin_theta_0;

		return (s0 * from) + (s1 * to);
	}
	else {
		return from;
	}
}

//
//
//

inline float signed_area_double(r32 x1, r32 y1, r32 x2, r32 y2, r32 x3, r32 y3) {
	return (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
}

inline r32 signed_area_double(Vec2 a, Vec2 b, Vec2 c) {
	return ((a.x - b.x) * (b.y - c.y) - (b.x - c.x) * (a.y - b.y));
}

r32 signed_area(Vec2 a, Vec2 b, Vec2 c) {
	return 0.5f * signed_area_double(a, b, c);
}

r32 signed_area(Vec3 a, Vec3 b, Vec3 c) {
	r32 px = signed_area_double(a.y, a.z, b.y, b.z, c.y, c.z);
	r32 py = signed_area_double(a.z, a.x, b.z, b.x, c.z, c.x);
	r32 pz = signed_area_double(a.x, a.y, b.x, b.y, c.x, c.y);
	px *= px;
	py *= py;
	pz *= pz;
	return 0.5f * sqrtf(px + py + pz);
}

bool triangle_is_cw(Vec2 a, Vec2 b, Vec2 c) {
	return vec2_determinant(b - a, c - a) < 0.0f;
}

Vec2 corner_point(const Mm_Rect &b, u32 n) {
	Vec2 p;
	p.x = ((n & 1) ? b.max.x : b.min.x);
	p.y = ((n & 2) ? b.max.y : b.min.y);
	return p;
}

r32 point_to_segment_length2(Vec2 p, Vec2 a, Vec2 b) {
	Vec2 ab = b - a, ap = p - a, bp = p - b;
	r32 e = vec2_dot(ap, ab);
	// Handle cases where p projects outside ab
	if (e <= 0.0f) return vec2_dot(ap, ap);
	r32 f = vec2_dot(ab, ab);
	if (e >= f) return vec2_dot(bp, bp);
	// Handle cases where p projects onto ab
	return vec2_dot(ap, ap) - e * e / f;
}

r32 point_to_mm_rect_length2(Vec2 p, const Mm_Rect &rect) {
	r32 dist2 = 0;
	for (s32 i = 0; i < 2; i++) {
		r32 v = p.m[i];
		if (v < rect.min.m[i]) dist2 += (rect.min.m[i] - v) * (rect.min.m[i] - v);
		if (v > rect.max.m[i]) dist2 += (v - rect.max.m[i]) * (v - rect.max.m[i]);
	}
	return dist2;
}

r32 point_to_aabb2d_length2(Vec2 p, const Aabb2d &aabb) {
	Vec2 dim = vec2(aabb.radius[0], aabb.radius[1]);
	Mm_Rect rect;
	rect.min = aabb.center - dim;
	rect.max = aabb.center + dim;
	return point_to_mm_rect_length2(p, rect);
}

Vec2 nearest_point_point_segment(Vec2 p, Vec2 a, Vec2 b, r32 *t) {
	Vec2 ab = b - a;
	r32 len = vec2_dot(ab, ab);
	*t = vec2_dot(p - a, ab) / len;
	*t = clamp01(*t);
	return a + (*t * ab);
}

Vec2 nearest_point_point_segment(Vec2 p, Vec2 a, Vec2 b) {
	r32 t;
	return nearest_point_point_segment(p, a, b, &t);
}

Vec2 nearest_point_origin_segment(Vec2 a, Vec2 b, r32 *t) {
	Vec2 ab = b - a;
	r32 len = vec2_dot(ab, ab);
	*t = vec2_dot(-a, ab) / len;
	*t = clamp01(*t);
	return a + (*t * ab);
}

Vec2 nearest_point_origin_segment(Vec2 a, Vec2 b) {
	r32 t;
	return nearest_point_origin_segment(a, b, &t);
}

Vec2 nearest_point_point_mm_rect(Vec2 p, const Mm_Rect &rect) {
	Vec2 q;
	for (s32 i = 0; i < 2; i++) {
		r32 v = p.m[i];
		v = maximum(v, rect.min.m[i]);
		v = minimum(v, rect.max.m[i]);
		q.m[i] = v;
	}
	return q;
}

Vec2 nearest_point_origin_mm_rect(const Mm_Rect &rect) {
	return nearest_point_point_mm_rect(vec2(0), rect);
}

Vec2 nearest_point_point_aabb2d(Vec2 a, const Aabb2d &aabb) {
	Vec2 dim = vec2(aabb.radius[0], aabb.radius[1]);
	Mm_Rect rect;
	rect.min = aabb.center - dim;
	rect.max = aabb.center + dim;
	return nearest_point_point_mm_rect(a, rect);
}

Vec2 nearest_point_point_aabb2d(const Aabb2d &aabb) {
	return nearest_point_point_aabb2d(vec2(0), aabb);
}

Vec2 nearest_point_point_triangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c) {
	// Check if P in vertex region outside A
	Vec2 ab = b - a;
	Vec2 ac = c - a;
	Vec2 ap = p - a;
	r32 d1 = vec2_dot(ab, ap);
	r32 d2 = vec2_dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)

	// Check if P in vertex region outside B
	Vec2 bp = p - b;
	r32 d3 = vec2_dot(ab, bp);
	r32 d4 = vec2_dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)

	// Check if P in edge region of AB, if so return projection of P onto AB
	r32 vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		r32 v = d1 / (d1 - d3);
		return a + v * ab; // barycentric coordinates (1-v,v,0)
	}

	// Check if P in vertex region outside C
	Vec2 cp = p - c;
	r32 d5 = vec2_dot(ab, cp);
	r32 d6 = vec2_dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)

	// Check if P in edge region of AC, if so return projection of P onto AC
	r32 vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		r32 w = d2 / (d2 - d6);
		return a + w * ac; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	r32 va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		r32 w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b); // barycentric coordinates (0,1-w,w)
	}

	// P inside face region
	r32 denom = 1.0f / (va + vb + vc);
	r32 v = vb * denom;
	r32 w = vc * denom;
	return a + ab * v + ac * w;
}

Vec2 nearest_point_origin_triangle(Vec2 a, Vec2 b, Vec2 c) {
	// Check if origin in vertex region outside A
	Vec2 ab = b - a;
	Vec2 ac = c - a;
	Vec2 ap = -a;
	r32 d1 = vec2_dot(ab, ap);
	r32 d2 = vec2_dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)

	// Check if P in vertex region outside B
	Vec2 bp = -b;
	r32 d3 = vec2_dot(ab, bp);
	r32 d4 = vec2_dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)

	// Check if P in edge region of AB, if so return projection of P onto AB
	r32 vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		r32 v = d1 / (d1 - d3);
		return a + v * ab; // barycentric coordinates (1-v,v,0)
	}

	// Check if P in vertex region outside C
	Vec2 cp = -c;
	r32 d5 = vec2_dot(ab, cp);
	r32 d6 = vec2_dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)

	// Check if P in edge region of AC, if so return projection of P onto AC
	r32 vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		r32 w = d2 / (d2 - d6);
		return a + w * ac; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	r32 va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		r32 w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b); // barycentric coordinates (0,1-w,w)
	}

	// origin inside face region
	r32 denom = 1.0f / (va + vb + vc);
	r32 v = vb * denom;
	r32 w = vc * denom;
	return a + ab * v + ac * w;
}

r32 nearest_point_segment_segment(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, r32 *s, r32 *t, Vec2 *c1, Vec2 *c2) {
	Vec2 d1 = q1 - p1;
	Vec2 d2 = q2 - p2;
	Vec2 r = p1 - p2;
	r32 a = vec2_dot(d1, d1);
	r32 e = vec2_dot(d2, d2);
	r32 f = vec2_dot(d2, r);

	// Check if either or both segments degenerate into points
	if (a <= EPSILON_FLOAT && e <= EPSILON_FLOAT) {
		// Both segments degenerate into points
		*s = *t = 0.0f;
		*c1 = p1;
		*c2 = p2;
		return vec2_dot(*c1 - *c2, *c1 - *c2);
	}

	if (a <= EPSILON_FLOAT) {
		// First segment degenerates into a point
		*s = 0.0f;
		*t = f / e;
		*t = clamp01(*t);
	} else {
		r32 c = vec2_dot(d1, r);
		if (e <= EPSILON_FLOAT) {
			// Second segment degenerates into a point
			*t = 0.0f;
			*s = clamp01(-c / a);
		} else {
			// The general nondegenerate case starts here
			r32 b = vec2_dot(d1, d2);
			r32 denom = a * e - b * b;
			if (denom != 0.0f) {
				*s = clamp01((b * f - c * e) / denom);
			} else {
				*s = 0.0f;
			}

			r32 tnom = b * *s + f;
			if (tnom < 0.0f) {
				*t = 0.0f;
				*s = clamp01(-c / a);
			} else if (tnom > e) {
				*t = 1.0f;
				*s = clamp01((b - c) / a);
			} else {
				*t = tnom / e;
			}
		}
	}

	*c1 = p1 + d1 * *s;
	*c2 = p2 + d2 * *t;
	return vec2_dot(*c1 - *c2, *c1 - *c2);
}

Vec3 barycentric(Vec2 a, Vec2 b, Vec2 c, Vec2 p) {
	auto m = signed_area_double(a, b, c);

	r32 nu = signed_area_double(p, b, c);
	r32 nv = signed_area_double(a, p, c);
	r32 ood = 1.0f / m;

	Vec3 res;
	res.x = nu * ood;
	res.y = nv * ood;
	res.z = 1.0f - res.x - res.y;

	return res;
}

Vec3 barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p) {
	auto m = vec3_cross(b - a, c - a);

	r32 nu, nv, ood;

	// Absolute components for determining projection plane
	r32 x = fabsf(m.x), y = fabsf(m.y), z = fabsf(m.z);

	// Compute areas in plane of largest projection
	if (x >= y && x >= z) {
		// x is largest, project to the yz plane
		nu = signed_area_double(p.y, p.z, b.y, b.z, c.y, c.z);
		nv = signed_area_double(p.y, p.z, c.y, c.z, a.y, a.z);
		ood = 1.0f / m.x;
	}
	else if (y >= x && y >= z) {
		// y is largest, project to the xz plane
		nu = signed_area_double(p.x, p.z, b.x, b.z, c.x, c.z);
		nv = signed_area_double(p.x, p.z, c.x, c.z, a.x, a.z);
		ood = 1.0f / -m.y;
	}
	else {
		// z is largest, project to the xy plane
		nu = signed_area_double(p.x, p.y, b.x, b.y, c.x, c.y);
		nv = signed_area_double(p.x, p.y, c.x, c.y, a.x, a.y);
		ood = 1.0f / m.z;
	}

	Vec3 res;
	res.x = nu * ood;
	res.y = nv * ood;
	res.z = 1.0f - res.x - res.y;

	return res;
}

bool is_quad_convex(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
	auto bda = vec2_determinant(d - b, a - b);
	auto bdc = vec2_determinant(d - b, c - b);
	if ((bda * bdc) >= 0.0f) return false;
	auto acd = vec2_determinant(c - a, d - a);
	auto acb = vec2_determinant(c - a, b - a);
	return (acd * acb) < 0.0f;
}

bool is_quad_convex(Vec3 a, Vec3 b, Vec3 c, Vec3 d) {
	auto bda = vec3_cross(d - b, a - b);
	auto bdc = vec3_cross(d - b, c - b);
	if (vec3_dot(bda, bdc) >= 0.0f) return false;
	auto acd = vec3_cross(c - a, d - a);
	auto acb = vec3_cross(c - a, b - a);
	return vec3_dot(acd, acb) < 0.0f;
}

bool is_polygon_convex(const Vec2 *vertices, u32 count) {
	Vec2 a, b, c;

	for (u32 outer = 0; outer < count; ++outer) {
		a = vertices[outer];
		b = vertices[(outer + 1) % count];
		c = vertices[(outer + 2) % count];

		if (!triangle_is_cw(a, b, c))
			return false;
	}

	return true;
}

bool is_polygon_convex(const Polygon &polygon) {
	return is_polygon_convex(polygon.vertices, polygon.vertex_count);
}

bool is_polygon_convex(const Polygon_Pt &polygon) {
	return is_polygon_convex(polygon.vertices, polygon.vertex_count);
}

Mm_Rect enclosing_mm_rect_mm_rect(const Mm_Rect &a0, const Mm_Rect &a1) {
	Mm_Rect a;
	for (s32 i = 0; i < 2; i++) {
		a.min.m[i] = minimum(a0.min.m[i], a1.min.m[i]);
		a.max.m[i] = maximum(a0.max.m[i], a1.max.m[i]);
	}
	return a;
}

Circle enclosing_circle_circle(const Circle &c0, const Circle &c1) {
	Circle c;
	// Compute the squared distance between the circle centers
	Vec2 d = c1.center - c0.center;
	r32 dist2 = vec2_dot(d, d);
	r32 dr2 = c1.radius - c0.radius;
	dr2 *= dr2;
	if (dr2 >= dist2) {
		// The circle with the larger radius encloses the other
		// just set c to be the larger of the two spheres
		if (c1.radius >= c0.radius)
			c = c1;
		else
			c = c0;
	} else {
		// Spheres partially overlapping or disjoint
		r32 dist = sqrtf(dist2);
		c.radius = (dist + c0.radius + c1.radius) * 0.5f;
		c.center = c0.center;
		if (dist > EPSILON_FLOAT)
			c.center += ((c.radius - c0.radius) / dist) * d;
	}
	return c;
}

Mm_Rect enclosing_mm_rect_circle(const Mm_Rect &r, const Circle &c) {
	Mm_Rect b;
	b.min.x = c.center.x - c.radius;
	b.min.y = c.center.y - c.radius;
	b.max.x = c.center.x + c.radius;
	b.max.y = c.center.y + c.radius;
	return enclosing_mm_rect_mm_rect(r, b);
}

s32 point_farthest_from_edge(Vec2 a, Vec2 b, Vec2 *p, s32 n) {
	Vec2 e = b - a;
	Vec2 eperp = vec2(-e.y, e.x);

	s32 best_index = -1;
	r32 max_value = -FLT_MAX, right_most_value = -FLT_MAX;

	for (s32 i = 1; i < n; i++) {
		r32 d = vec2_dot(p[i] - a, eperp);
		r32 r = vec2_dot(p[i] - a, e);
		if (d > max_value || (d == max_value && r > right_most_value)) {
			best_index = i;
			max_value = d;
			right_most_value = r;
		}
	}

	return best_index;
}

void extreme_points_along_direction(Vec2 dir, Vec2 *pt, s32 n, s32 *min_index, s32 *max_index) {
	r32 min_proj = FLT_MAX, max_proj = -FLT_MAX;

	for (s32 i = 0; i < n; i++) {
		auto proj = vec2_dot(pt[i], dir);

		if (proj < min_proj) {
			min_proj = proj;
			*min_index = i;
		}

		if (proj > max_proj) {
			max_proj = proj;
			*max_index = i;
		}
	}
}

void most_seperated_points_on_aabb(Vec2 *pt, s32 n, s32 *min, s32 *max) {
	s32 minx = 0, maxx = 0, miny = 0, maxy = 0;

	for (s32 i = 1; i < n; i++) {
		if (pt[i].x < pt[minx].x) minx = i;
		if (pt[i].x > pt[maxx].x) maxx = i;
		if (pt[i].y < pt[miny].y) miny = i;
		if (pt[i].y > pt[maxy].y) maxy = i;
	}

	r32 dist2x = vec2_dot(pt[maxx] - pt[minx], pt[maxx] - pt[minx]);
	r32 dist2y = vec2_dot(pt[maxy] - pt[miny], pt[maxy] - pt[miny]);

	if (dist2y > dist2x) {
		*max = maxy;
		*min = miny;
	}
	else {
		*min = minx;
		*max = maxx;
	}
}

Circle circle_from_distant_points(Vec2 *pt, s32 n) {
	s32 min, max;
	most_seperated_points_on_aabb(pt, n, &min, &max);
	Circle c;
	c.center = (pt[min] + pt[max]) * 0.5f;
	c.radius = vec2_dot(pt[max] - c.center, pt[max] - c.center);
	c.radius = sqrtf(c.radius);
	return c;
}

r32 min_area_rect(Vec2 *pt, s32 num_pts, Vec2 *center, Vec2 u[2]) {
	r32 min_area = FLT_MAX;

	for (s32 i = 0, j = num_pts - 1; i < num_pts; j = i, i++) {
		auto e0 = pt[i] - pt[j];
		e0 /= vec2_length(e0);
		auto e1 = vec2(-e0.y, e0.x);

		r32 min0 = 0.0f, min1 = 0.0f, max0 = 0.0f, max1 = 0.0f;
		for (s32 k = 0; k < num_pts; k++) {
			Vec2 d = pt[k] - pt[j];
			r32 dot = vec2_dot(d, e0);
			if (dot < min0) min0 = dot;
			if (dot > max0) max0 = dot;
			dot = vec2_dot(d, e1);
			if (dot < min1) min1 = dot;
			if (dot > max1) max1 = dot;
		}
		r32 area = (max0 - min0) * (max1 - min1);

		if (area < min_area) {
			min_area = area;
			*center = pt[j] + 0.5f * ((min0 + max0) * e0 + (min1 + max1) * e1);
			u[0] = e0;
			u[1] = e1;
		}
	}

	return min_area;
}

Mm_Rect mm_rect_enclosing_quad(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
	Vec2 points[] = { a, b, c, d };

	Mm_Rect rect;
	rect.min = points[0];
	rect.max = points[0];

	for (u32 index = 1; index < static_count(points); ++index) {
		Vec2 *p = points + index;
		rect.min = vec2_min(rect.min, *p);
		rect.max = vec2_max(rect.max, *p);
	}

	return rect;
}

Mm_Rect mm_rect_enclosing_mm_rect(const Mm_Rect &mm_rect, const Transform &t) {
	Vec2 a = mat2_vec2_mul(t.xform, mm_rect.min);
	Vec2 b = mat2_vec2_mul(t.xform, vec2(mm_rect.min.x, mm_rect.max.y));
	Vec2 c = mat2_vec2_mul(t.xform, mm_rect.max);
	Vec2 d = mat2_vec2_mul(t.xform, vec2(mm_rect.max.x, mm_rect.min.y));
	Mm_Rect result = mm_rect_enclosing_quad(a, b, c, d);
	result.min += t.p;
	result.max += t.p;
	return result;
}

Mm_Rect mm_rect_enclosing_circle(const Circle &circle) {
	Mm_Rect rect;
	rect.min = vec2(-circle.radius);
	rect.max = vec2( circle.radius);
	rect.min += circle.center;
	rect.max += circle.center;
	return rect;
}

Mm_Rect mm_rect_enclosing_circle(const Circle &circle, const Transform &t) {
	Mm_Rect rect;
	
	Vec2 a = support(circle, t, vec2(1, 0));
	Vec2 b = support(circle, t, vec2(0, 1));
	Vec2 c = support(circle, t, vec2(-1, 0));
	Vec2 d = support(circle, t, vec2(0, -1));

	rect.min = vec2(c.x, d.y);
	rect.max = vec2(a.x, b.y);
	
	return rect;
}

Mm_Rect mm_rect_enclosing_capsule(const Capsule &capsule) {
	Mm_Rect rect;
	
	if (capsule.a.x > capsule.b.x) {
		rect.min.x = capsule.b.x;
		rect.max.x = capsule.a.x;
	} else {
		rect.min.x = capsule.a.x;
		rect.max.x = capsule.b.x;
	}

	if (capsule.a.y > capsule.b.y) {
		rect.min.y = capsule.b.y;
		rect.max.y = capsule.a.y;
	} else {
		rect.min.y = capsule.a.y;
		rect.max.y = capsule.b.y;
	}

	rect.min -= vec2(capsule.radius);
	rect.max += vec2(capsule.radius);

	return rect;
}

Mm_Rect mm_rect_enclosing_capsule(const Capsule &capsule, const Transform &t) {
	Mm_Rect rect;

	Vec2 a = support(capsule, t, vec2(1, 0));
	Vec2 b = support(capsule, t, vec2(0, 1));
	Vec2 c = support(capsule, t, vec2(-1, 0));
	Vec2 d = support(capsule, t, vec2(0, -1));

	rect.min = vec2(c.x, d.y);
	rect.max = vec2(a.x, b.y);

	return rect;
}

Mm_Rect mm_rect_enclosing_polygon(const Polygon &polygon) {
	Mm_Rect rect;
	rect.min = polygon.vertices[0];
	rect.max = polygon.vertices[0];

	for (u32 index = 1; index < polygon.vertex_count; ++index) {
		const Vec2 *p = polygon.vertices + index;
		rect.min = vec2_min(rect.min, *p);
		rect.max = vec2_max(rect.max, *p);
	}

	return rect;
}

Mm_Rect mm_rect_enclosing_polygon(const Polygon &polygon, const Transform &t) {
	Mm_Rect rect;
	rect.min = mat2_vec2_mul(t.xform, polygon.vertices[0]);
	rect.max = mat2_vec2_mul(t.xform, polygon.vertices[0]);

	for (u32 index = 1; index < polygon.vertex_count; ++index) {
		const Vec2 *p = polygon.vertices + index;
		rect.min = vec2_min(rect.min, mat2_vec2_mul(t.xform, *p));
		rect.max = vec2_max(rect.max, mat2_vec2_mul(t.xform, *p));
	}

	rect.min += t.p;
	rect.max += t.p;

	return rect;
}

Mm_Rect transform_mmrect(const Mm_Rect &a, const Mat2 &mat, Vec2 t) {
	Mm_Rect b;
	for (s32 i = 0; i < 2; i++) {
		b.min.m[i] = b.max.m[i] = t.m[i];
		for (s32 j = 0; j < 2; j++) {
			r32 e = mat.m2[i][j] * a.min.m[j];
			r32 f = mat.m2[i][j] * a.max.m[j];
			if (e < f) {
				b.min.m[i] += e;
				b.max.m[i] += f;
			}
			else {
				b.min.m[i] += f;
				b.max.m[i] += e;
			}
		}
	}
	return b;
}

Mm_Rect transform_mmrect(const Mm_Rect &a, r32 rot, Vec2 t) {
	return transform_mmrect(a, mat2_rotation(rot), t);
}

Aabb2d transform_aabb(const Aabb2d &a, const Mat2 &mat, Vec2 t) {
	Aabb2d b;
	for (s32 i = 0; i < 2; i++) {
		b.center.m[i] = t.m[i];
		b.radius[i] = 0.0f;
		for (s32 j = 0; j < 2; j++) {
			b.center.m[i] += mat.m2[i][j] * a.center.m[j];
			b.radius[i] += fabsf(mat.m2[i][j]) * a.radius[j];
		}
	}
	return b;
}

Aabb2d transform_aabb(const Aabb2d &a, r32 rot, Vec2 t) {
	return transform_aabb(a, mat2_rotation(rot), t);
}

bool test_point_inside_rect(Vec2 point, const Mm_Rect &rect) {
	if (point.x < rect.min.x || point.y < rect.min.y) return false;
	if (point.x > rect.max.x || point.y > rect.max.y) return false;
	return true;
}

bool test_point_inside_aabb(Vec2 point, const Aabb2d &aabb) {
	if (point.x < (aabb.center.x - aabb.radius[0]) || point.y < (aabb.center.y - aabb.radius[1])) return false;
	if (point.x > (aabb.center.x + aabb.radius[0]) || point.y > (aabb.center.y + aabb.radius[1])) return false;
	return true;
}

bool test_point_inside_circle(Vec2 p, const Circle &c) {
	Vec2 d = c.center - p;
	r32 dist2 = vec2_dot(d, d);
	return dist2 < c.radius * c.radius;
}

bool test_point_inside_capsule(Vec2 p, const Capsule &c) {
	r32 dst2 = point_to_segment_length2(p, c.a, c.b);
	return dst2 <= c.radius * c.radius;
}

bool test_point_inside_triangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c) {
	a -= p; b -= p; c -= p;

	r32 u = vec2_determinant(b, c);
	r32 v = vec2_determinant(c, a);	
	if ((u * v) < 0.0f) return false;

	r32 w = vec2_determinant(a, b);
	if ((u * w) < 0.0f) return false;

	return true;
}

bool test_origin_inside_triangle(Vec2 a, Vec2 b, Vec2 c) {
	r32 u = vec2_determinant(b, c);
	r32 v = vec2_determinant(c, a);
	if ((u * v) < 0.0f) return false;

	r32 w = vec2_determinant(a, b);
	if ((u * w) < 0.0f) return false;

	return true;
}

bool test_point_inside_convex_polygon(Vec2 p, Vec2 *v, s32 n) {
	// Do binary search over polygon vertices to find the fan triangle
	// (v[0], v[low], v[high]) the pos32 p lies within the near sides of
	s32 low = 0, high = n;
	do {
		s32 mid = (low + high) / 2;
		if (triangle_is_cw(v[0], v[mid], p))
			low = mid;
		else
			high = mid;
	} while (low + 1 < high);
	// If pos32 outside last (or first) edge, then it is not inside the n-gon
	if (low == 0 || high == n) return 0;
	// p is inside the polygon if it is left of
	// the directed edge from v[low] to v[high]
	return triangle_is_cw(v[low], v[high], p);
}

bool test_mmrect_vs_mmrect(const Mm_Rect &a, const Mm_Rect &b) {
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	return true;
}

bool test_aabb_vs_aabb(const Aabb2d &a, const Aabb2d &b) {
	if (fabsf(a.center.x - b.center.x) > (a.radius[0] + b.radius[0])) return false;
	if (fabsf(a.center.y - b.center.y) > (a.radius[1] + b.radius[1])) return false;
	return true;
}

bool test_circle_vs_circle(const Circle &a, const Circle &b) {
	auto d = a.center - b.center;
	r32 dist2 = vec2_dot(d, d);
	r32 radius_sum = a.radius + b.radius;
	return dist2 <= radius_sum * radius_sum;
}

bool test_quad_vs_quad(const Quad &a, const Quad &b) {
	auto quad_a = &a;
	auto quad_b = &b;

	for (s32 quad_index = 0; quad_index < 2; ++quad_index) {
		for (s32 edge_index = 0; edge_index < 4; ++edge_index) {
			r32 min_proj_a = INFINITY, max_proj_a = -INFINITY;
			r32 min_proj_b = INFINITY, max_proj_b = -INFINITY;

			r32 dot;
			Vec2 normal = quad_a->normals[edge_index];

			for (s32 p_index = 0; p_index < 4; ++p_index) {
				dot = vec2_dot(normal, quad_a->positions[p_index]);
				min_proj_a = minimum(min_proj_a, dot);
				max_proj_a = maximum(max_proj_a, dot);
			}

			for (s32 p_index = 0; p_index < 4; ++p_index) {
				dot = vec2_dot(normal, quad_b->positions[p_index]);
				min_proj_b = minimum(min_proj_b, dot);
				max_proj_b = maximum(max_proj_b, dot);
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

bool test_circle_vs_capsule(const Circle &circle, const Capsule &capsule) {
	r32 dst2 = point_to_segment_length2(circle.center, capsule.a, capsule.b);
	r32 radius = circle.radius + capsule.radius;
	return dst2 <= radius *radius;
}

bool test_segment_vs_circle(Vec2 a, Vec2 b, const Circle &c) {
	r32 dist2 = point_to_segment_length2(c.center, a, b);
	return dist2 <= c.radius * c.radius;
}

bool test_capsule_vs_capsule(const Capsule &capsule1, const Capsule &capsule2) {
	r32 s, t;
	Vec2 c1, c2;
	r32 dist2 = nearest_point_segment_segment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, &s, &t, &c1, &c2);
	r32 radius = capsule1.radius + capsule2.radius;
	return dist2 <= radius * radius;
}

bool test_circle_vs_aabb(const Circle &c, const Aabb2d &b) {
	r32 dist2 = point_to_aabb2d_length2(c.center, b);
	return dist2 <= c.radius * c.radius;
}

bool test_ray_vs_circle(const Ray2d &ray, const Circle &circle) {
	Vec2 m = ray.origin - circle.center;
	r32 c = vec2_dot(m, m) - circle.radius * circle.radius;
	if (c <= 0.0f) return true;

	r32 b = vec2_dot(m, ray.dir);
	// Early exit if ray origin outside sphere and ray pointing away from sphere
	if (b > 0.0f) return false;

	r32 disc = b * b - c;
	if (disc < 0.0f) return false;
	return true;
}

bool test_segment_vs_mm_rect(Vec2 p0, Vec2 p1, const Mm_Rect &rect) {
	Vec2 c = (rect.min + rect.max) * 0.5f;
	Vec2 e = rect.max - c;
	Vec2 m = (p0 + p1) * 0.5f;
	Vec2 d = p1 - m;

	m = m - c;

	r32 adx = fabsf(d.x); if (fabsf(m.x) > e.x + adx) return false;
	r32 ady = fabsf(d.y); if (fabsf(m.y) > e.y + ady) return false;

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis (see text for detail)
	adx += EPSILON_FLOAT; ady += EPSILON_FLOAT;
	if (fabsf(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;

	return true;
}

bool segment_vs_segment(Vec2 a, Vec2 b, Vec2 c, Vec2 d, r32 *t, Vec2 *p) {
	// Sign of areas correspond to which side of ab points c and d are
	r32 a1 = signed_area_double(a, b, d);
	r32 a2 = signed_area_double(a, b, c);

	// If c and d are on different sides of ab, areas have different signs
	if (a1 != 0.0f && a2 != 0.0f && a1 * a2 < 0.0f) {
		// Compute signs for a and b with respect to segment cd
		r32 a3 = signed_area_double(c, d, a);
		r32 a4 = a3 + a2 - a1;
		// Points a and b on different sides of cd if areas have different signs
		if (a3 != 0.0f && a4 != 0.0f && a3 * a4 < 0.0f) {
			*t = a3 / (a3 - a4);
			*p = a + *t * (b - a);
			return true;
		}
	}

	return false;
}

bool circle_vs_aabb(const Circle &c, Aabb2d &b, Vec2 *p) {
	*p = nearest_point_point_aabb2d(c.center, b);
	Vec2 v = *p - c.center;
	return vec2_dot(v, v) <= c.radius * c.radius;
}

bool circle_vs_triangle(const Circle &circle, Vec2 a, Vec2 b, Vec2 c, Vec2 *p) {
	*p = nearest_point_point_triangle(circle.center, a, b, c);
	Vec2 v = *p - circle.center;
	return vec2_dot(v, v) <= circle.radius * circle.radius;
}

bool intersect_segment_ray(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, r32 *t, Vec2 *p) {
	r32 dx1 = p1.x - q1.x;
	r32 dy1 = p1.y - q1.y;
	r32 dx2 = p2.x - q2.x;
	r32 dy2 = p2.y - q2.y;

	r32 d = dx1 * dy2 - dy1 * dx2;

	if (d) {
		r32 n2 = -dx1 * (p1.y - p2.y) + dy1 * (p1.x - p2.x);
		r32 u = n2 / d;

		r32 n = (p1.x - p2.x) * dy2 - (p1.y - p2.y) * dx2;
		*t = n / d;

		if ((*t >= 0.0f && *t <= 1.0f) && u >= 0.0f) {
			p->x = p1.x - (*t * dx1);
			p->y = p1.y - (*t * dy1);
			return true;
		}
	}

	return false;
}

bool intersect_segment_ray(Vec2 p1, Vec2 q1, const Ray2d &ray, r32 *t, Vec2 *p) {
	Vec2 p2 = ray.origin;
	Vec2 q2 = ray.origin + ray.dir;
	return intersect_segment_ray(p1, q1, p2, q2, t, p);
}

bool intersect_ray_ray(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, r32 *t, r32 *u, Vec2 *p) {
	r32 dx1 = p1.x - q1.x;
	r32 dy1 = p1.y - q1.y;
	r32 dx2 = p2.x - q2.x;
	r32 dy2 = p2.y - q2.y;

	r32 d = dx1 * dy2 - dy1 * dx2;

	if (d) {
		r32 n2 = -dx1 * (p1.y - p2.y) + dy1 * (p1.x - p2.x);
		*u = n2 / d;

		r32 n = (p1.x - p2.x) * dy2 - (p1.y - p2.y) * dx2;
		*t = n / d;

		p->x = p2.x - (*u * dx2);
		p->y = p2.y - (*u * dy2);
		return true;
	}

	return false;
}

bool intersect_ray_ray(Vec2 p1, Vec2 q1, const Ray2d &b, r32 *t, r32 *u, Vec2 *p) {
	Vec2 p2 = b.origin;
	Vec2 q2 = b.origin + b.dir;
	return intersect_ray_ray(p1, q1, p2, q2, t, u, p);
}

bool intersect_ray_ray(const Ray2d &a, const Ray2d &b, r32 *t, r32 *u, Vec2 *p) {
	Vec2 p1 = a.origin;
	Vec2 q1 = a.origin + a.dir;
	Vec2 p2 = b.origin;
	Vec2 q2 = b.origin + b.dir;
	return intersect_ray_ray(p1, q1, p2, q2, t, u, p);
}

bool intersect_segment_segment(Vec2 p1, Vec2 q1, Vec2 p2, Vec2 q2, r32 *t, r32 *u, Vec2 *p) {
	r32 dx1 = p1.x - q1.x;
	r32 dy1 = p1.y - q1.y;
	r32 dx2 = p2.x - q2.x;
	r32 dy2 = p2.y - q2.y;

	r32 d = dx1 * dy2 - dy1 * dx2;

	if (d) {
		r32 n2 = -dx1 * (p1.y - p2.y) + dy1 * (p1.x - p2.x);
		*u = n2 / d;

		if (*u >= 0.0f && *u <= 1.0f) {
			r32 n = (p1.x - p2.x) * dy2 - (p1.y - p2.y) * dx2;
			*t = n / d;
			if (*t >= 0.0f && *t <= 1.0f) {
				p->x = p2.x - (*u * dx2);
				p->y = p2.y - (*u * dy2);
				return true;
			}
		}
	}

	return false;
}

bool intersect_circle_ray(const Circle &circle, const Ray2d &ray, r32 *t, Vec2 *p) {
	Vec2 m = ray.origin - circle.center;

	r32 b = vec2_dot(m, ray.dir);
	r32 c = vec2_dot(m, m) - circle.radius * circle.radius;

	// Exit if ray’s origin outside circle (c > 0) and ray pointing away from circle (b > 0)
	if (c > 0.0f && b > 0.0f) return false;

	r32 discr = b * b - c;
	if (discr < 0.0f) return false;

	// Ray found to intersect circle, compute smallest t value of intersection
	*t = -b - sqrtf(discr);

	// If t is negative, ray started inside sphere so clamp t to zero
	if (*t < 0.0f) *t = 0.0f;
	*p = ray.origin + *t * ray.dir;
	return true;
}

bool intersect_circle_segment(const Circle &circle, Vec2 p, Vec2 q, r32 *t) {
	Vec2 m = p - circle.center;
	Vec2 d = q - p;

	r32 b = vec2_dot(m, d);
	r32 c = vec2_dot(m, m) - circle.radius * circle.radius;

	// Exit if ray’s origin outside circle (c > 0) and ray pointing away from circle (b > 0)
	if (c > 0.0f && b > 0.0f) return false;

	r32 a = vec2_dot(d, d);

	r32 discr = b * b - a * c;
	if (discr < 0.0f) return false;

	// Ray found to intersect circle, compute smallest t value of intersection
	*t = (-b - sqrtf(discr)) / a;
	if (*t <0.0f || *t > 1.0f) return false;

	return true;
}

bool intersect_mm_rect_ray(const Ray2d &ray, const Mm_Rect &rect, r32 *tmin, Vec2 *q) {
	*tmin = 0.0f;
	r32 tmax = FLT_MAX;

	for (s32 i = 0; i < 2; i++) {
		if (fabsf(ray.dir.m[i]) < EPSILON_FLOAT) {
			// Ray is parallel to slab. No hit if origin not within slab
			if (ray.origin.m[i] < rect.min.m[i] || ray.origin.m[i] > rect.max.m[i]) return false;
		} else {
			// Compute intersection t value of ray with near and far plane of slab
			r32 ood = 1.0f / ray.dir.m[i];
			r32 t1 = (rect.min.m[i] - ray.origin.m[i]) * ood;
			r32 t2 = (rect.max.m[i] - ray.origin.m[i]) * ood;
			// Make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2) {
				auto temp = t1;
				t1 = t2;
				t2 = temp;
			}
			// Compute the intersection of slab intersection intervals
			if (t1 > *tmin) *tmin = t1;
			if (t2 > tmax) tmax = t2;
			// Exit with no collision as soon as slab intersection becomes empty
			if (*tmin > tmax) return false;
		}
	}

	*q = ray.origin + ray.dir * *tmin;
	return true;
}

bool intersect_mm_rect_segment(Vec2 a, Vec2 b, const Mm_Rect &rect, r32 *tmin, Vec2 *q) {
	Ray2d ray = { a, b - a };
	
	*tmin = 0.0f;
	r32 tmax = 1.0f;

	for (s32 i = 0; i < 2; i++) {
		if (fabsf(ray.dir.m[i]) < EPSILON_FLOAT) {
			// Ray is parallel to slab. No hit if origin not within slab
			if (ray.origin.m[i] < rect.min.m[i] || ray.origin.m[i] > rect.max.m[i]) return false;
		}
		else {
			// Compute intersection t value of ray with near and far plane of slab
			r32 ood = 1.0f / ray.dir.m[i];
			r32 t1 = (rect.min.m[i] - ray.origin.m[i]) * ood;
			r32 t2 = (rect.max.m[i] - ray.origin.m[i]) * ood;
			// Make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2) {
				auto temp = t1;
				t1 = t2;
				t2 = temp;
			}
			// Compute the intersection of slab intersection intervals
			if (t1 > * tmin) *tmin = t1;
			if (t2 > tmax) tmax = t2;
			// Exit with no collision as soon as slab intersection becomes empty
			if (*tmin > tmax) return false;
		}
	}

	*q = ray.origin + ray.dir * *tmin;
	return true;
}

bool dynamic_circle_vs_circle(const Circle &c0, const Circle &c1, Vec2 v0, Vec2 v1, r32 *t) {
	Vec2 s = c1.center - c0.center;
	Vec2 v = v1 - v0; // Relative motion of c1 with respect to stationary c0
	r32 r = c1.radius + c0.radius;
	r32 c = vec2_dot(s, s) - r * r;

	if (c < 0.0f) {
		// Spheres initially overlapping so exit directly
		*t = 0.0f;
		return true;
	}

	r32 a = vec2_dot(v, v);
	if (a < EPSILON_FLOAT) return false; // Spheres not moving relative each other

	r32 b = vec2_dot(v, s);
	if (b >= 0.0f) return false; // Spheres not moving towards each other

	r32 d = b * b - a * c;
	if (d < 0.0f) return false; // No real-valued root, spheres do not intersect

	*t = (-b - sqrtf(d)) / a;
	return true;
}

bool dynamic_circle_vs_mm_rect(const Circle &c, Vec2 d, const Mm_Rect &b, r32 *t) {
	// Compute the AABB resulting from expanding b by circle radius r
	Mm_Rect e = b;
	e.min.x -= c.radius; e.min.y -= c.radius;
	e.max.x += c.radius; e.max.y += c.radius;

	// Define line segment [p0, p1] specified by the circle movement
	Vec2 p0 = c.center;
	Vec2 p1 = c.center + d;

	// Intersect ray against expanded AABB e. Exit with no intersection if ray
	// misses e, else get intersection pos32 p and time t as result
	Vec2 p;
	if (!intersect_mm_rect_segment(p0, p1, e, t, &p)) {
		return false;
	}

	// Compute which min and max faces of b the intersection pos32 p lies
	// outside of. Note, u and v cannot have the same bits set and
	// they must have at least one bit set among them
	u32 u = 0, v = 0;
	if (p.x < b.min.x) u |= 1;
	if (p.x > b.max.x) v |= 1;
	if (p.y < b.min.y) u |= 2;
	if (p.y > b.max.y) v |= 2;

	// ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
	s32 m = u + v;

	// If both 2 bits set (m == 3) then p is in a vertex region
	if (m == 3) {
		// Now check [p0, p1] against the circle in the vertex
		Vec2 vertex = corner_point(b, v);
		if (!intersect_circle_segment(Circle{ vertex, c.radius }, p0, p1, t)) return false;
	}

	return true;
}

bool dynamic_mm_rect_vs_mm_rect(const Mm_Rect &a, const Mm_Rect &b, Vec2 va, Vec2 vb, r32 *tfirst, r32 *tlast) {
	// Exit early if 'a' and 'b' initially overlapping
	if (test_mmrect_vs_mmrect(a, b)) {
		*tfirst = *tlast = 0.0f;
		return true;
	}

	// Use relative velocity; effectively treating 'a' as stationary
	Vec2 v = vb - va;

	// Initialize times of first and last contact
	*tfirst = 0.0f;
	*tlast  = 1.0f;

	// For each axis, determine times of first and last contact, if any
	for (s32 i = 0; i < 2; i++) {
		if (v.m[i] < 0.0f) {
			if (b.max.m[i] < a.min.m[i]) return false; // Nonintersecting and moving apart
			if (a.max.m[i] < b.min.m[i]) 
				*tfirst = maximum((a.max.m[i] - b.min.m[i]) / v.m[i], *tfirst);
			if (b.max.m[i] > a.min.m[i]) 
				*tlast = minimum((a.min.m[i] - b.max.m[i]) / v.m[i], *tlast);
		}

		if (v.m[i] > 0.0f) {
			if (b.min.m[i] > a.max.m[i]) return false; // Nonintersecting and moving apart
			if (b.max.m[i] < a.min.m[i]) 
				*tfirst = maximum((a.min.m[i] - b.max.m[i]) / v.m[i], *tfirst);
			if (a.max.m[i] > b.min.m[i]) 
				*tlast = minimum((a.max.m[i] - b.min.m[i]) / v.m[i], *tlast);
		}

		// No overlap possible if time of first contact occurs after time of last contact
		if (*tfirst > *tlast) return false;
	}

	return true;
}

Vec2 support(const Circle &c, Vec2 dir) {
	Vec2 n = vec2_normalize(dir);
	return c.center + n * c.radius;
}

Vec2 support(const Mm_Rect &m, Vec2 dir) {
	return vec2(dir.x >= 0.0f ? m.max.x : m.min.x, dir.y >= 0.0f ? m.max.y : m.min.y);
}

Vec2 support(const Capsule &c, Vec2 dir) {
	Vec2 n = vec2_normalize(dir);
	Vec2 s;

	r32 da = vec2_dot(c.a, dir);
	r32 db = vec2_dot(c.b, dir);
	if (da > db)
		s = c.a;
	else
		s = c.b;

	return s + c.radius * n;
}

inline Vec2 support_polygon(const Vec2 *vertices, u32 count, Vec2 dir) {
	s32 index = 0;
	r32 p = vec2_dot(dir, vertices[index]);

	s32 adj_index;
	r32 adj_p;
	while (true) {
		adj_index = (index + 1 == count ? 0 : index + 1);
		adj_p = vec2_dot(dir, vertices[adj_index]);
		if (adj_p > p) {
			p = adj_p;
			index = adj_index;
		} else {
			adj_index = (index - 1 == -1 ? count - 1 : index - 1);
			adj_p = vec2_dot(dir, vertices[adj_index]);
			if (adj_p > p) {
				p = adj_p;
				index = adj_index;
			} else {
				break;
			}
		}
	}

	return vertices[index];
}

Vec2 support(const Polygon &shape, Vec2 dir) {
	return support_polygon(shape.vertices, shape.vertex_count, dir);
}

Vec2 support(const Polygon_Pt &shape, Vec2 dir) {
	return support_polygon(shape.vertices, shape.vertex_count, dir);
}

Vec2 support(const Circle &a, const Circle &b, Vec2 dir) {
	Vec2 n = vec2_normalize(dir);
	return a.center - b.center + n * (a.radius + b.radius);
}

Vec2 support(const Circle &a, const Capsule &b, Vec2 dir) {
	Vec2 n = vec2_normalize(dir);
	Vec2 s = a.center;
	r32 da, db;

	da = vec2_dot(b.a, dir);
	db = vec2_dot(b.b, dir);
	if (da > db)
		s -= b.a;
	else
		s -= b.b;

	return s + n * (a.radius + b.radius);
}

Vec2 support(const Capsule &a, const Circle &b, Vec2 dir) {
	Vec2 n = vec2_normalize(dir);
	Vec2 s;
	r32 da, db;

	da = vec2_dot(a.a, dir);
	db = vec2_dot(a.b, dir);
	if (da > db)
		s = a.a;
	else
		s = a.b;

	return s - b.center + n * (a.radius + b.radius);
}

Vec2 support(const Capsule &a, const Capsule &b, Vec2 dir) {
	Vec2 n = vec2_normalize(dir);
	Vec2 s;
	r32 da, db;

	da = vec2_dot(a.a, dir);
	db = vec2_dot(a.b, dir);
	if (da > db)
		s = a.a;
	else
		s = a.b;
	
	da = vec2_dot(b.a, -dir);
	db = vec2_dot(b.b, -dir);
	if (da > db)
		s -= b.a;
	else
		s -= b.b;

	return s + n * (a.radius + b.radius);
}

Vec2 support(const Mm_Rect &a, const Mm_Rect &b, Vec2 dir) {
	Vec2 pa, pb;
	if (dir.x >= 0.0f) {
		pa.x = a.max.x;
		pb.x = b.min.x;
	} else {
		pa.x = a.min.x;
		pb.x = b.max.x;
	}
	if (dir.y >= 0.0f) {
		pa.y = a.max.y;
		pb.y = b.min.y;
	} else {
		pa.y = a.min.y;
		pb.y = b.max.y;
	}
	return pa - pb;
}

bool next_simplex(Vec2 *simplex, Vec2 *dir, u32 *n) {
	switch (*n) {
	case 2: {
		// a = simplex[1]
		// b = simplex[0]
		Vec2 ab = simplex[0] - simplex[1];
		Vec2 ao = -simplex[1];
		if (vec2_dot(ao, ab) > 0.0f) {
			r32 det = vec2_determinant(simplex[1], simplex[0]);
			dir->x = -ab.y * det;
			dir->y = ab.x * det;
		} else {
			*dir = ao;
			simplex[0] = simplex[1];
			*n = 1;
		}
	} break;

	case 3: {
		// a = simplex[2]
		// b = simplex[1]
		// c = simplex[0]
		Vec2 ab = simplex[1] - simplex[2];
		Vec2 ac = simplex[0] - simplex[2];
		Vec2 ao = -simplex[2];
		r32 abc = vec2_determinant(ab, ac);

		if (vec2_dot(vec2(-ac.y * abc, ac.x * abc), ao) > 0.0f) {
			if (vec2_dot(ac, ao) > 0.0f) {
				r32 det = vec2_determinant(simplex[2], simplex[0]);
				dir->x = -ac.y * det;
				dir->y = ac.x * det;

				simplex[1] = simplex[0];
				simplex[0] = simplex[2];

				*n = 2;
			} else {
				if (vec2_dot(ab, ao) > 0.0f) {
					r32 det = vec2_determinant(simplex[2], simplex[1]);
					dir->x = -ab.y * det;
					dir->y = ab.x * det;
					simplex[0] = simplex[2];
					*n = 2;
				} else {
					*dir = -simplex[2];
					simplex[0] = simplex[2];
					*n = 1;
				}
			}
		} else {
			if (vec2_dot(vec2(ab.y * abc, -ab.x * abc), ao) > 0.0f) {
				if (vec2_dot(ab, ao) > 0.0f) {
					r32 det = vec2_determinant(simplex[2], simplex[1]);
					dir->x = -ab.y * det;
					dir->y = ab.x * det;
					simplex[0] = simplex[2];
					*n = 2;
				} else {
					*dir = -simplex[2];
					simplex[0] = simplex[2];
					*n = 1;
				}
			} else {
				// point is inside the triangle
				return true;
			}
		}
	} break;

	invalid_default_case();
	}

	return false;
}

Nearest_Edge nearest_edge_point_polygon(const Vec2 *vertices, u32 vertex_count, Vec2 point) {
	Nearest_Edge edge;
	edge.distance = MAX_FLOAT;

	for (u32 p_index = 0; p_index < vertex_count; p_index += 1) {
		u32 q_index = ((p_index + 1) == vertex_count) ? 0 : p_index + 1;

		auto a = vertices[p_index];
		auto b = vertices[q_index];
		auto e = b - a;

		Vec2 n = vec2_normalize_check(vec2(-e.y, e.x));

		r32 d = vec2_dot(n, a - point);
		if (d < edge.distance) {
			edge.normal = n;
			edge.distance = d;
			edge.a_index = p_index;
			edge.b_index = q_index;
		}
	}

	return edge;
}

Nearest_Edge nearest_edge_origin_polygon(const Vec2 *vertices, u32 vertex_count) {
	Nearest_Edge edge;
	edge.distance = MAX_FLOAT;

	for (u32 p_index = 0; p_index < vertex_count; p_index += 1) {
		u32 q_index = ((p_index + 1) == vertex_count) ? 0 : p_index + 1;

		auto a = vertices[p_index];
		auto b = vertices[q_index];
		auto e = b - a;

		Vec2 n = vec2_normalize(vec2(-e.y, e.x));

		r32 d = vec2_dot(n, a);
		if (d < edge.distance) {
			edge.normal = n;
			edge.distance = d;
			edge.a_index = p_index;
			edge.b_index = q_index;
		}
	}

	return edge;
}

bool next_simplex_ex(Support_Ex *simplex, Vec2 *dir, u32 *n) {
	switch (*n) {
	case 2: {
		// a = simplex[1]
		// b = simplex[0]
		Vec2 ab = simplex[0].p - simplex[1].p;
		Vec2 ao = -simplex[1].p;
		if (vec2_dot(ao, ab) > 0.0f) {
			r32 det = vec2_determinant(simplex[1].p, simplex[0].p);
			dir->x = -ab.y * det;
			dir->y = ab.x * det;
		}
		else {
			*dir = ao;
			simplex[0] = simplex[1];
			*n = 1;
		}
	} break;

	case 3: {
		// a = simplex[2]
		// b = simplex[1]
		// c = simplex[0]
		Vec2 ab = simplex[1].p - simplex[2].p;
		Vec2 ac = simplex[0].p - simplex[2].p;
		Vec2 ao = -simplex[2].p;
		r32 abc = vec2_determinant(ab, ac);

		if (vec2_dot(vec2(-ac.y * abc, ac.x * abc), ao) > 0.0f) {
			if (vec2_dot(ac, ao) > 0.0f) {
				r32 det = vec2_determinant(simplex[2].p, simplex[0].p);
				dir->x = -ac.y * det;
				dir->y = ac.x * det;

				simplex[1] = simplex[0];
				simplex[0] = simplex[2];

				*n = 2;
			}
			else {
				if (vec2_dot(ab, ao) > 0.0f) {
					r32 det = vec2_determinant(simplex[2].p, simplex[1].p);
					dir->x = -ab.y * det;
					dir->y = ab.x * det;
					simplex[0] = simplex[2];
					*n = 2;
				}
				else {
					*dir = -simplex[2].p;
					simplex[0] = simplex[2];
					*n = 1;
				}
			}
		}
		else {
			if (vec2_dot(vec2(ab.y * abc, -ab.x * abc), ao) > 0.0f) {
				if (vec2_dot(ab, ao) > 0.0f) {
					r32 det = vec2_determinant(simplex[2].p, simplex[1].p);
					dir->x = -ab.y * det;
					dir->y = ab.x * det;
					simplex[0] = simplex[2];
					*n = 2;
				} else {
					*dir = -simplex[2].p;
					simplex[0] = simplex[2];
					*n = 1;
				}
			}
			else {
				// point is inside the triangle
				return true;
			}
		}
	} break;

		invalid_default_case();
	}

	return false;
}

Nearest_Edge nearest_edge_origin_polygon_ex(const Support_Ex *vertices, u32 vertex_count) {
	Nearest_Edge edge;
	edge.distance = MAX_FLOAT;

	for (u32 p_index = 0; p_index < vertex_count; p_index += 1) {
		u32 q_index = ((p_index + 1) == vertex_count) ? 0 : p_index + 1;

		auto a = vertices[p_index].p;
		auto b = vertices[q_index].p;
		auto e = b - a;

		Vec2 n = vec2_normalize(vec2(-e.y, e.x));

		r32 d = vec2_dot(n, a);
		if (d < edge.distance) {
			edge.normal = n;
			edge.distance = d;
			edge.a_index = p_index;
			edge.b_index = q_index;
		}
	}

	return edge;
}
