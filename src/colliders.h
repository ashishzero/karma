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

typedef u32 Collider_Flags;

enum Collider_Flag_Bit : Collider_Flags {
	Collider_Flag_Bit_TOUCHED = bit(0),
};

struct Collider {
	attribute(read_only)	void *handle;
	attribute(read_only)	Collider_Type type;
	attribute(no_display)	Collider_Flags flags;
};

inline void *collider_get_handle(Collider *collider, Collider_Type type) {
	assert(type == collider->type);
	return collider->handle;
}
#define collider_get_shape(collider, type) ((type *)collider_get_handle(collider, Collider_##type))

typedef void *Collider_Handle;

struct Collider_Group {
	Collider_Handle handle;
	u32 count;
};
