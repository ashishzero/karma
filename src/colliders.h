#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

struct Null {
	u32 _placeholder;
};

enum Collider_Type : u32 {
	Collider_Null,
	Collider_Circle,
	Collider_Mm_Rect,
	Collider_Polygon,
	Collider_Capsule,

	Collider_Count
};

struct Collider {
	attribute(read_only)	void *handle;
	attribute(read_only)	Collider_Type type;
};

inline void *collider_get_handle(Collider *collider, Collider_Type type) {
	assert(type == collider->type);
	return collider->handle;
}
#define collider_get_shape(collider, type) ((type *)collider_get_handle(collider, Collider_##type))

//
//
//

inline void collider_transform(Circle *circle, const Mat3 &m) {
	circle->center = mat3_vec2_mul(m, circle->center);
}

inline void collider_transform(Polygon *polygon, const Mat3 &m) {
	Vec2 *vertex = polygon->vertices;
	for (u32 index = 0; index < polygon->vertex_count; ++index, ++vertex)
		*vertex = mat3_vec2_mul(m, *vertex);
}

inline void collider_transform(Mm_Rect *mm_rect, const Mat3 &m) {
	mm_rect->min = mat3_vec2_mul(m, mm_rect->min);
	mm_rect->max = mat3_vec2_mul(m, mm_rect->max);
}

inline void collider_transform(Capsule *capsule, const Mat3 &m) {
	capsule->a = mat3_vec2_mul(m, capsule->a);
	capsule->b = mat3_vec2_mul(m, capsule->b);
}
