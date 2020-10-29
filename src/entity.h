#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"
#include "colliders.h"

typedef u64 Entity_Id;
typedef u64 Raw_Collider_Id;

enum Entity_Type {
	Entity_Null,
	Entity_Player,
	Entity_Static_Body,

	Entity_Count
};

struct Entity {
	attribute(read_only)				Entity_Id id;
	attribute(read_only, no_serialize)  Entity_Type type;
	Vec2 position;
};

typedef void *Collider_Handle;
typedef u32 Collision_Flags;

struct Collider_Group;
struct Rigid_Body;

enum Collision_Flag_Bit : Collision_Flags {
	Collision_Bit_OCUURED = bit(0),
	Collision_Bit_MOTION  = bit(1),
};

struct Collider_Group {
	Collider	*	collider;
	Mat3			transform;
	Rigid_Body	*	rigid_body;
	u32				count;
	Collision_Flags flags;
};

struct Rigid_Body {
	Vec2			velocity;
	Vec2			force;
	void			*colliders;
};

struct Player : public Entity {
	attribute(min:0, max:5)				r32 radius;
	attribute(color)					Vec4 color;
										r32 intensity;
	attribute(read_only)				Raw_Collider_Id collider_id;
	attribute(no_serialize)				Rigid_Body *rigid_body;
};

struct Static_Body : public Entity {
	attribute(color)						Vec4 color;
	attribute(read_only)					Raw_Collider_Id collider_id;
	attribute(read_only, no_serialize)		Collider_Group *colliders;
};

struct Camera {
	Vec2 position;
	r32 distance;
};
