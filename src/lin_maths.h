#pragma once
#include "lin_maths_types.h"

#define to_radians(deg)       ((deg) * (MATH_PI / 180.0f))
#define to_degrees(rad)       ((rad) * (180.0f / MATH_PI_INVERSE))
#define sgn(n)                ((r32)(0 < (n)) - (r32)((n) < 0))
#define min_value(a, b)       (((a) < (b)) ? (a) : (b))
#define max_value(a, b)       (((a) > (b)) ? (a) : (b))
#define clamp(min, max, v)    min_value(max, max_value(min, v))
#define r32_equals(a, b, tol) (fabsf((a) - (b)) < (tol))

inline r32 clamp01(r32 v) {
	return clamp(0.0f, 1.0f, v);
}

inline r32 map(r32 from_x1, r32 from_x2, r32 to_x1, r32 to_x2, r32 x) {
	return (to_x2 - to_x1) / (from_x2 - from_x1) * (x - from_x1) + to_x1;
}

inline r32 map01(r32 x1, r32 x2, r32 x) {
	return map(x1, x2, 0, 1, x);
}

inline Vec2 vec2_min(Vec2 a, Vec2 b) {
	Vec2 r;
	r.x = min_value(a.x, b.x);
	r.y = min_value(a.y, b.y);
	return r;
}

inline Vec3 vec3_min(Vec3 a, Vec3 b) {
	Vec3 r;
	r.x = min_value(a.x, b.x);
	r.y = min_value(a.y, b.y);
	r.z = min_value(a.z, b.z);
	return r;
}

inline Vec4 vec4_min(Vec4 a, Vec4 b) {
	Vec4 r;
	r.x = min_value(a.x, b.x);
	r.y = min_value(a.y, b.y);
	r.z = min_value(a.z, b.z);
	r.w = min_value(a.w, b.w);
	return r;
}

const Vec3 WORLD_UP      = vec3(0, 0, 1);
const Vec3 WORLD_RIGHT   = vec3(1, 0, 0);
const Vec3 WORLD_FORWARD = vec3(0, 1, 0);

Vec2  vec2_add(Vec2 a, Vec2 b);
Vec3  vec3_add(Vec3 a, Vec3 b);
Vec4  vec4_add(Vec4 a, Vec4 b);
Vec2s vec2s_add(Vec2s a, Vec2s b);
Vec3s vec3s_add(Vec3s a, Vec3s b);
Vec4s vec4s_add(Vec4s a, Vec4s b);

//
//
//

Vec2  vec2_sub(Vec2 a, Vec2 b);
Vec3  vec3_sub(Vec3 a, Vec3 b);
Vec4  vec4_sub(Vec4 a, Vec4 b);
Vec2s vec2s_sub(Vec2s a, Vec2s b);
Vec3s vec3s_sub(Vec3s a, Vec3s b);
Vec4s vec4s_sub(Vec4s a, Vec4s b);

//
//
//

Vec2  vec2_mul(r32 s, Vec2 v);
Vec2  vec2_mul(Vec2 v, r32 s);
Vec3  vec3_mul(r32 s, Vec3 v);
Vec3  vec3_mul(Vec3 v, r32 s);
Vec4  vec4_mul(r32 s, Vec4 v);
Vec4  vec4_mul(Vec4 v, r32 s);
Vec2s vec2s_mul(s32 s, Vec2s v);
Vec2s vec2s_mul(Vec2s v, s32 s);
Vec3s vec3s_mul(s32 s, Vec3s v);
Vec3s vec3s_mul(Vec3s v, s32 s);
Vec4s vec4s_mul(s32 s, Vec4s v);
Vec4s vec4s_mul(Vec4s v, s32 s);

//
//
//

Vec2  vec2_div(Vec2 v, r32 s);
Vec3  vec3_div(Vec3 v, r32 s);
Vec4  vec4_div(Vec4 v, r32 s);
Vec2s vec2s_div(Vec2s v, s32 s);
Vec3s vec3s_div(Vec3s v, s32 s);
Vec4s vec4s_div(Vec4s v, s32 s);

//
//
//

Vec2  vec2_hadamard(Vec2 l, Vec2 r);
Vec3  vec3_hadamard(Vec3 l, Vec3 r);
Vec4  vec4_hadamard(Vec4 l, Vec4 r);
Vec2s vec2s_hadamard(Vec2s l, Vec2s r);
Vec3s vec3s_hadamard(Vec3s l, Vec3s r);
Vec4s vec4s_hadamard(Vec4s l, Vec4s r);

//
//
//

inline Vec2 operator-(Vec2 &v) {
	return vec2(-v.x, -v.y);
}
inline Vec3 operator-(Vec3 &v) {
	return vec3(-v.x, -v.y, -v.z);
}
inline Vec4 operator-(Vec4 &v) {
	return vec4(-v.x, -v.y, -v.z, -v.w);
}
inline Vec2s operator-(Vec2s &v) {
	return vec2s(-v.x, -v.y);
}
inline Vec3s operator-(Vec3s &v) {
	return vec3s(-v.x, -v.y, -v.z);
}
inline Vec4s operator-(Vec4s &v) {
	return vec4s(-v.x, -v.y, -v.z, -v.w);
}

//
//
//

inline Vec2 operator+(Vec2 a, Vec2 b) {
	return vec2_add(a, b);
}
inline Vec3 operator+(Vec3 a, Vec3 b) {
	return vec3_add(a, b);
}
inline Vec4 operator+(Vec4 a, Vec4 b) {
	return vec4_add(a, b);
}
inline Vec2s operator+(Vec2s a, Vec2s b) {
	return vec2s_add(a, b);
}
inline Vec3s operator+(Vec3s a, Vec3s b) {
	return vec3s_add(a, b);
}
inline Vec4s operator+(Vec4s a, Vec4s b) {
	return vec4s_add(a, b);
}

//
//
//

inline Vec2 operator-(Vec2 a, Vec2 b) {
	return vec2_sub(a, b);
}
inline Vec3 operator-(Vec3 a, Vec3 b) {
	return vec3_sub(a, b);
}
inline Vec4 operator-(Vec4 a, Vec4 b) {
	return vec4_sub(a, b);
}
inline Vec2s operator-(Vec2s a, Vec2s b) {
	return vec2s_sub(a, b);
}
inline Vec3s operator-(Vec3s a, Vec3s b) {
	return vec3s_sub(a, b);
}
inline Vec4s operator-(Vec4s a, Vec4s b) {
	return vec4s_sub(a, b);
}

//
//
//

inline Vec2 &operator+=(Vec2 &a, Vec2 b) {
	a = vec2_add(a, b);
	return a;
}
inline Vec3 &operator+=(Vec3 &a, Vec3 b) {
	a = vec3_add(a, b);
	return a;
}
inline Vec4 &operator+=(Vec4 &a, Vec4 b) {
	a = vec4_add(a, b);
	return a;
}
inline Vec2s &operator+=(Vec2s &a, Vec2s b) {
	a = vec2s_add(a, b);
	return a;
}
inline Vec3s &operator+=(Vec3s &a, Vec3s b) {
	a = vec3s_add(a, b);
	return a;
}
inline Vec4s &operator+=(Vec4s &a, Vec4s b) {
	a = vec4s_add(a, b);
	return a;
}

//
//
//

inline Vec2 &operator-=(Vec2 &a, Vec2 b) {
	a = vec2_sub(a, b);
	return a;
}
inline Vec3 &operator-=(Vec3 &a, Vec3 b) {
	a = vec3_sub(a, b);
	return a;
}
inline Vec4 &operator-=(Vec4 &a, Vec4 b) {
	a = vec4_sub(a, b);
	return a;
}
inline Vec2s &operator-=(Vec2s &a, Vec2s b) {
	a = vec2s_sub(a, b);
	return a;
}
inline Vec3s &operator-=(Vec3s &a, Vec3s b) {
	a = vec3s_sub(a, b);
	return a;
}
inline Vec4s &operator-=(Vec4s &a, Vec4s b) {
	a = vec4s_sub(a, b);
	return a;
}

//
//
//

inline Vec2 operator*(Vec2 v, r32 s) {
	return vec2_mul(v, s);
}
inline Vec2 operator*(r32 s, Vec2 v) {
	return vec2_mul(v, s);
}
inline Vec3 operator*(Vec3 v, r32 s) {
	return vec3_mul(v, s);
}
inline Vec3 operator*(r32 s, Vec3 v) {
	return vec3_mul(v, s);
}
inline Vec4 operator*(Vec4 v, r32 s) {
	return vec4_mul(v, s);
}
inline Vec4 operator*(r32 s, Vec4 v) {
	return vec4_mul(v, s);
}
inline Vec2s operator*(Vec2s v, s32 s) {
	return vec2s_mul(v, s);
}
inline Vec2s operator*(s32 s, Vec2s v) {
	return vec2s_mul(v, s);
}
inline Vec3s operator*(Vec3s v, s32 s) {
	return vec3s_mul(v, s);
}
inline Vec3s operator*(s32 s, Vec3s v) {
	return vec3s_mul(v, s);
}
inline Vec4s operator*(Vec4s v, s32 s) {
	return vec4s_mul(v, s);
}
inline Vec4s operator*(s32 s, Vec4s v) {
	return vec4s_mul(v, s);
}

//
//
//

inline Vec2 operator/(Vec2 v, r32 s) {
	return vec2_div(v, s);
}
inline Vec3 operator/(Vec3 v, r32 s) {
	return vec3_div(v, s);
}
inline Vec4 operator/(Vec4 v, r32 s) {
	return vec4_div(v, s);
}
inline Vec2s operator/(Vec2s v, s32 s) {
	return vec2s_div(v, s);
}
inline Vec3s operator/(Vec3s v, s32 s) {
	return vec3s_div(v, s);
}
inline Vec4s operator/(Vec4s v, s32 s) {
	return vec4s_div(v, s);
}

//
//
//

inline Vec2 &operator*=(Vec2 &t, r32 s) {
	t = vec2_mul(t, s);
	return t;
}
inline Vec3 &operator*=(Vec3 &t, r32 s) {
	t = vec3_mul(t, s);
	return t;
}
inline Vec4 &operator*=(Vec4 &t, r32 s) {
	t = vec4_mul(t, s);
	return t;
}
inline Vec2s &operator*=(Vec2s &t, s32 s) {
	t = vec2s_mul(t, s);
	return t;
}
inline Vec3s &operator*=(Vec3s &t, s32 s) {
	t = vec3s_mul(t, s);
	return t;
}
inline Vec4s &operator*=(Vec4s &t, s32 s) {
	t = vec4s_mul(t, s);
	return t;
}

//
//
//

inline Vec2 &operator/=(Vec2 &t, r32 s) {
	t = vec2_div(t, s);
	return t;
}
inline Vec3 &operator/=(Vec3 &t, r32 s) {
	t = vec3_div(t, s);
	return t;
}
inline Vec4 &operator/=(Vec4 &t, r32 s) {
	t = vec4_div(t, s);
	return t;
}
inline Vec2s &operator/=(Vec2s &t, s32 s) {
	t = vec2s_div(t, s);
	return t;
}
inline Vec3s &operator/=(Vec3s &t, s32 s) {
	t = vec3s_div(t, s);
	return t;
}
inline Vec4s &operator/=(Vec4s &t, s32 s) {
	t = vec4s_div(t, s);
	return t;
}

//
//
//

r32 vec2_dot(Vec2 a, Vec2 b);
r32 vec3_dot(Vec3 a, Vec3 b);
r32 vec4_dot(Vec4 a, Vec4 b);

//
//
//

Vec3 vec3_cross(Vec3 a, Vec3 b);

//
//
//

r32 vec2_length2(Vec2 v);
r32 vec3_length2(Vec3 v);
r32 vec4_length2(Vec4 v);

r32 vec2_length(Vec2 v);
r32 vec3_length(Vec3 v);
r32 vec4_length(Vec4 v);

r32 vec2_distance(Vec2 a, Vec2 b);
r32 vec3_distance(Vec3 a, Vec3 b);
r32 vec4_distance(Vec4 a, Vec4 b);

//
//
//

// these check for division by zero
// returns {0,0,0} if vector length is zero
Vec2 vec2_normalize_check(Vec2 v);
Vec3 vec3_normalize_check(Vec3 v);
Vec4 vec4_normalize_check(Vec4 v);

// no checking for division by zero
Vec2 vec2_normalize(Vec2 v);
Vec3 vec3_normalize(Vec3 v);
Vec4 vec4_normalize(Vec4 v);

//
//
//

r32 vec2_angle_between(Vec2 a, Vec2 b);
r32 vec2_angle_between_normalize(Vec2 a, Vec2 b);
r32 vec3_angle_between(Vec3 a, Vec3 b);
r32 vec3_angle_between_normalize(Vec3 a, Vec3 b);

r32 vec2_signed_angle_between(Vec2 a, Vec2 b);
r32 vec2_signed_angle_between_normalize(Vec2 a, Vec2 b);
r32 vec3_signed_angle_between(Vec3 a, Vec3 b, Vec3 n);
r32 vec3_signed_angle_between_normalize(Vec3 a, Vec3 b, Vec3 n);

//
//
//

bool vec2_equals(Vec2 a, Vec2 b, r32 tolerance = MATH_R32_EQUALS_DEFAULT_TOLERANCE);
bool vec3_equals(Vec3 a, Vec3 b, r32 tolerance = MATH_R32_EQUALS_DEFAULT_TOLERANCE);
bool vec4_equals(Vec4 a, Vec4 b, r32 tolerance = MATH_R32_EQUALS_DEFAULT_TOLERANCE);

//
//
//

Mat3 mat3_identity();
Mat3 mat3_inverse(const Mat3 &mat);
Mat3 mat3_transpose(const Mat3 &m);

Mat4 mat4_identity();
Mat4 mat4_inverse(const Mat4 &mat);
Mat4 mat4_transpose(const Mat4 &m);

//
//
//

Mat3 mat3_mul(const Mat3 &left, const Mat3 &right);
Vec3 mat3_vec3_mul(const Mat3 &mat, Vec3 vec);

Mat4 mat4_mul(const Mat4 &left, const Mat4 &right);
Vec4 mat4_vec4_mul(const Mat4 &mat, Vec4 vec);

inline Mat3 operator*(const Mat3 &a, const Mat3 &b) {
	return mat3_mul(a, b);
}
inline Mat3 &operator*=(Mat3 &t, Mat3 &o) {
	t = mat3_mul(t, o);
	return t;
}
inline Vec3 operator*(const Mat3 &m, Vec3 v) {
	return mat3_vec3_mul(m, v);
}

inline Mat4 operator*(const Mat4 &a, const Mat4 &b) {
	return mat4_mul(a, b);
}
inline Mat4 &operator*=(Mat4 &t, Mat4 &o) {
	t = mat4_mul(t, o);
	return t;
}
inline Vec4 operator*(const Mat4 &m, Vec4 v) {
	return mat4_vec4_mul(m, v);
}

//
//
//

Mat3 mat3_scalar(r32 S_1, r32 S_2);
Mat3 mat3_scalar(Vec2 s);

Mat3 mat3_translation(r32 T_x, r32 T_y);
Mat3 mat3_translation(Vec2 t);

Mat3 mat3_rotation(r32 angle);

Mat3 mat3_lookat(Vec2 from, Vec2 to, Vec2 forward = vec2(1, 0));

Mat4 mat4_scalar(r32 S_1, r32 S_2, r32 S_3);
Mat4 mat4_scalar(Vec3 s);

Mat4 mat4_translation(r32 T_x, r32 T_y, r32 T_z);
Mat4 mat4_translation(Vec3 t);

Mat4 mat4_rotation_x(r32 angle);
Mat4 mat4_rotation_y(r32 angle);
Mat4 mat4_rotation_z(r32 angle);
Mat4 mat4_rotation(r32 x, r32 y, r32 z, r32 angle);
Mat4 mat4_rotation(Vec3 axis, r32 angle);

Mat4 mat4_lookat(Vec3 from, Vec3 to, Vec3 world_up = WORLD_UP);

Mat4 mat4_ortho_gl(r32 l, r32 r, r32 t, r32 b, r32 n, r32 f);
Mat4 mat4_perspective_gl(r32 fov, r32 aspect_ratio, r32 n, r32 f);

Mat4 mat4_ortho_dx(r32 l, r32 r, r32 t, r32 b, r32 n, r32 f);
Mat4 mat4_perspective_dx(r32 fov, r32 aspect_ratio, r32 n, r32 f);

Vec3 mat4_right(const Mat4 &m);
Vec3 mat4_up(const Mat4 &m);
Vec3 mat4_forward(const Mat4 &m);

bool mat3_equals(const Mat3 &a, const Mat3 &b, r32 tolerance = MATH_R32_EQUALS_DEFAULT_TOLERANCE);
bool mat4_equals(const Mat4 &a, const Mat4 &b, r32 tolerance = MATH_R32_EQUALS_DEFAULT_TOLERANCE);

Mat3 mat4_to_mat3(const Mat4 &mat);
Mat4 mat3_to_mat4(const Mat3 &mat);

//
//
//

Quat quat_identity();

inline Quat operator-(Quat &q) {
	return quat(-q.v4);
}
inline Quat operator-(Quat r1, Quat r2) {
	return quat(r1.v4 - r2.v4);
}
inline Quat operator+(Quat r1, Quat r2) {
	return quat(r1.v4 + r2.v4);
}
inline Quat operator*(Quat q, r32 s) {
	return quat(q.v4 * s);
}
inline Quat operator*(r32 s, Quat q) {
	return quat(q.v4 * s);
}

r32 quat_dot(Quat q1, Quat q2);
r32 quat_length(Quat q);

Quat        quat_normalize(Quat q);
Quat        quat_conjugate(Quat q);
Quat        quat_mul(Quat q1, Quat q2);
inline Quat operator*(Quat a, Quat b) {
	return quat_mul(a, b);
}

Vec3 quat_rotate(Quat q, Vec3 v);
Quat quat_angle_axis(Vec3 axis, r32 angle);
Quat quat_angle_axis_normalize(Vec3 axis, r32 angle);

void quat_get_angle_axis(Quat q, r32 *angle, Vec3 *axis);
Vec3 quat_get_axis(Quat q);
r32  quat_get_angle(Quat q);

Quat quat_mat4(const Mat4 &m);
Quat quat_mat4_normalize(const Mat4 &m);
Mat4 quat_get_mat4(Quat q);

Vec3 quat_forward(Quat q);
Vec3 quat_right(Quat q);
Vec3 quat_up(Quat q);

Quat quat_euler_angles(r32 pitch, r32 yaw, r32 roll);
Quat quat_euler_angles(Vec3 euler);
Vec3 quat_get_euler_angles(Quat q);

Quat quat_between(Vec3 from, Vec3 to);
Quat quat_between(Quat a, Quat b);

Quat quat_lookat(Vec3 from, Vec3 to, Vec3 world_forward = WORLD_FORWARD);

bool quat_equals(Quat a, Quat b, r32 tolerance = MATH_R32_EQUALS_DEFAULT_TOLERANCE);

//
//
//

Color3 linear_to_srgb(Color3 color);
Color4 linear_to_srgb(Color4 color);
Color3 linear_to_srgb(Color3 color, r32 gamma);
Color4 linear_to_srgb(Color4 color, r32 gamma);

Color3 srgb_to_linear(Color3 color);
Color4 srgb_to_linear(Color4 color);
Color3 srgb_to_linear(Color3 color, r32 gamma);
Color4 srgb_to_linear(Color4 color, r32 gamma);

Colorh color4_to_hex(Color4 v);
Color4 hex_to_color4(Colorh c);
Color3 hex_to_color3(Colorh c);

Color3 hsv_to_rgb(Color3 c);
Color3 rgb_to_hsv(Color3 c);
Color4 hsv_to_rgb(Color4 c);
Color4 rgb_to_hsv(Color4 c);

//
//
//

Vec2 vec2_clamp(Vec2 min, Vec2 max, Vec2 v);
Vec3 vec3_clamp(Vec3 min, Vec3 max, Vec3 v);
Vec4 vec4_clamp(Vec4 min, Vec4 max, Vec4 v);

//
//
//

template <typename Type>
Type lerp(Type from, Type to, r32 t) {
	return (1 - t) * from + t * to;
}

template <typename Type>
Type bezier_quadratic(Type a, Type b, Type c, r32 t) {
	r32 mt = 1 - t;
	r32 w1 = mt * mt;
	r32 w2 = 2 * mt * t;
	r32 w3 = t * t;
	return w1 * a + w2 * b + w3 * c;
}

template <typename Type>
Type bezeir_cubic(Type a, Type b, Type c, Type d, r32 t) {
	r32 mt = 1.0f - t;
	r32 w1 = mt * mt * mt;
	r32 w2 = 3 * mt * mt * t;
	r32 w3 = 3 * mt * t * t;
	r32 w4 = t * t * t;
	return w1 * a + w2 * b + w3 * c + w4 * d;
}

template <typename Type>
Type slerp(Type from, Type to, r32 angle, r32 t) {
	r32 s   = sinf(angle);
	r32 ts  = sinf(angle * t);
	r32 mts = sinf(angle * (1 - t));
	return (mts * from + ts * to) * (1.0f / s);
}

Mat4 lerp(Mat4 &from, Mat4 &to, r32 t);
Vec2 slerp(Vec2 from, Vec2 to, r32 t);
Vec3 slerp(Vec3 from, Vec3 to, r32 t);
Vec4 slerp(Vec4 from, Vec4 to, r32 t);
Quat slerp(Quat from, Quat to, r32 t);

r32  step(r32 edge, r32 val);
Vec2 step(Vec2 edge, Vec2 val);
Vec3 step(Vec3 edge, Vec3 val);
Vec4 step(Vec4 edge, Vec4 val);
Quat step(Quat edge, Quat val);

r32 smoothstep(r32 a, r32 b, r32 x);
r32 smoothstep(Vec2 a, Vec2 b, Vec2 x);
r32 smoothstep(Vec3 a, Vec3 b, Vec3 x);
r32 smoothstep(Vec4 a, Vec4 b, Vec4 x);
r32 smoothstep(Quat a, Quat b, Quat x);
r32 inverse_smoothstep(r32 x);

//
//
//

r32  move_toward(r32 from, r32 to, r32 factor);
Vec2 move_toward(Vec2 from, Vec2 to, r32 factor);
Vec3 move_toward(Vec3 from, Vec3 to, r32 factor);
Vec4 move_toward(Vec4 from, Vec4 to, r32 factor);
Quat move_toward(Quat from, Quat to, r32 factor);

//
//
//

Vec2 rotate_around(Vec2 point, Vec2 center, r32 angle);

//
//
//

template <typename T>
inline T integrate(const T &x, r32 h, const T &f) {
	return x + h * f;
}

template <typename T, typename Function>
inline T integrate(const T &x, r32 t, r32 h, Function f) {
	return x + h * f(t, x);
}

template <typename T, typename Function>
inline T integrate_rk4(const T &x, r32 t, r32 h, Function f) {
	T k1 = h * f(t, x);
	T k2 = h * f(t + 0.5f * h, x + 0.5f * k1);
	T k3 = h * f(t + 0.5f * h, x + 0.5f * k2);
	T k4 = h * f(t + h, x + k3);
	return x + (k1 + 2.0f * (k2 + k3) + k4) / 6.0f;
}

//
//
//

bool point_inside_rect(Vec2 point, Mm_Rect rect);
bool aabb_collision(const Mm_Rect &a, const Mm_Rect &b);
