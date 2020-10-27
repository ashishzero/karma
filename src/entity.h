#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"
#include "colliders.h"

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
	attribute(read_only, no_serialize)	Collider_Group transformed_collider;
};

struct Static_Body : public Entity {
	attribute(color)						Vec4 color;
	attribute(read_only, no_serialize)		Collider_Group colliders;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
