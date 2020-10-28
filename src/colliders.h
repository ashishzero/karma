#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

enum Collider_Type : u32 {
	Collider_Null,
	Collider_Circle,
	Collider_Mm_Rect,
	Collider_Capsule,
	Collider_Polygon,

	Collider_Count
};

struct Collider {
	attribute(read_only)	Collider_Type type;
	attribute(read_only)	void *handle;
};

inline void *collider_get_handle(Collider *collider, Collider_Type type) {
	assert(type == collider->type);
	return collider->handle;
}
#define collider_get_shape(collider, type) ((type *)collider_get_handle(collider, Collider_##type))
