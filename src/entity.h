#pragma once
#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

enum Collider_Type : u32 {
	Collider_Circle,
	Collider_Mm_Rect,
	Collider_Polygon,
	Collider_Capsule
};

struct Collider {
	attribute(use, read-only) void *handle;
	attribute(read-only) Collider_Type type;
};

inline void *collider_get_handle(Collider &collider, Collider_Type type) {
	assert(type == collider.type);
	return collider.handle;
}
#define collider_get(collider, type) ((type *)collider_get_handle(collider, Collider_##type))

struct Entity {
	attribute(speed:0.01) Vec2 position;
};

struct Player : public Entity {
	attribute(color)					Vec4 color;
	attribute(min:0, max:5)				Circle collider;
	attribute(read-only)				Vec2 velocity;
	attribute(read-only)				Vec2 force;
	attribute(no-display, no-serialize)	Collider transformed_collider;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
