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
	attribute(use, read - only)			void *handle;
	attribute(read - only)				Collider_Type type;
	attribute(no - display, no - serialize) Collider_Flags flags;
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

//
//
//

enum Entity_Type {
	Entity_Player,
	Entity_Static_Body,

	Entity_Count
};

typedef u64 Entity_Id;

struct Entity {
	attribute(read_only)				Entity_Id id;
	attribute(read_only, no_serialize)  Entity_Type type;
	Vec2 position;
};

struct Player : public Entity {
	attribute(min:0, max:5)				r32 rradius;
	attribute(color)					Vec4 color;
	Circle collider;
	attribute(read_only)				Vec2 velocity;
	attribute(read_only)				Vec2 force;
	attribute(no_display, no_serialize)	Collider_Group transformed_collider;
};

struct Static_Body : public Entity {
	attribute(color)						Vec4 color;
	attribute(no_display, no_serialize)		Collider_Group colliders;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
