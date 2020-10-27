#pragma once
#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

enum Collider_Type : u32 {
	Collider_Null,
	Collider_Circle,
	Collider_Mm_Rect,
	Collider_Polygon,
	Collider_Capsule,

	Collider_Count
};

#if defined(ENABLE_DEVELOPER_OPTIONS)
constexpr u32 COLLIDER_FLAG_BIT_TOUCHED = bit(0);
#endif

struct Collider {
	attribute(use, read-only) void *handle;
	attribute(read-only) Collider_Type type;

#if defined(ENABLE_DEVELOPER_OPTIONS)
	attribute(no-display, no-serialize) u32 flags;
#endif
};

inline void *collider_get_handle(Collider *collider, Collider_Type type) {
	assert(type == collider->type);
	return collider->handle;
}
#define collider_get_shape(collider, type) ((type *)collider_get_handle(collider, Collider_##type))

enum Entity_Type {
	Entity_Player,
	Entity_Static_Body,

	Entity_Count
};

typedef u64 Entity_Id;

struct Entity {
	attribute(read-only)				Entity_Id id;
	attribute(read-only, no-serialize)  Entity_Type type;
										Vec2 position;
};

typedef u32 Collider_Key;

struct Collider_Group {
	Collider_Key key;
	u32 count;
};

struct Player : public Entity {
	attribute(min:0, max : 5)			r32 rradius;
	attribute(color)					Vec4 color;
										Circle collider;
	attribute(read-only)				Vec2 velocity;
	attribute(read-only)				Vec2 force;
	attribute(no-display, no-serialize)	Collider_Key transformed_collider;
};

struct Static_Body : public Entity {
	attribute(color) Vec4 color;
	Collider_Group collider_group;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
