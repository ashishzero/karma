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
	union {
		void *handle;
		Circle *circle;
		Mm_Rect *mm_rect;
		Polygon *polygon;
		Capsule *capsule;
	};

	Collider_Type type;
};

inline void *collider_get_handle(Collider &collider, Collider_Type type) {
	assert(type == collider.type);
	return collider.handle;
}
#define collider_get(collider, type) (type *)collider_get_handle(collider, Collider_##type)

struct Entity {
	attribute("speed:0.01") Vec2 position;
};

struct Player : public Entity {
	attribute("color")		Vec4 color;
	attribute("read-only")	Vec2 velocity;
	attribute("read-only")	Vec2 force;
	attribute("min:0.0", "max:5.0") Circle collider;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
