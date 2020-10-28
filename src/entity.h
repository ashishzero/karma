#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"
#include "colliders.h"

enum Entity_Type {
	Entity_Null,
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

typedef void *Collider_Handle;

struct Collider_Group {
	Collider_Handle handle;
	Entity_Id		entity_id;
	u32 count;
};

typedef u32 Collision_Flags;

enum Collision_Flag_Bit : Collision_Flags {
	Collision_Bit_OCUURED = bit(0),
};

struct Rigid_Body {
	Vec2 velocity;
	Vec2 force;
	Collider_Group colliders;
	Collision_Flags flags;
};

struct Player : public Entity {
	attribute(min:0, max:5)				r32 radius;
	attribute(color)					Vec4 color;
										r32 intensity;
	attribute(read_only, no_serialize)	Rigid_Body *rigid_body;
};

struct Static_Body : public Entity {
	attribute(color)						Vec4 color;
	attribute(read_only, no_serialize)		Collider_Group colliders;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
