#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

typedef u64 Entity_Id;

enum Fixture_Shape : u32 {
	Fixture_Shape_Null,
	Fixture_Shape_Circle,
	Fixture_Shape_Mm_Rect,
	Fixture_Shape_Capsule,
	Fixture_Shape_Polygon,

	Fixture_Shape_Count
};

struct Fixture {
	Fixture_Shape		shape;
	void *				handle;
};

#define fixture_get_shape(fixture, type) ((type *)((fixture)->handle))

enum Rigid_Body_Type : u16 {
	Rigid_Body_Type_Static,
	Rigid_Body_Type_Dynamic
};

typedef u16 Rigid_Body_Flags;

enum Rigid_Body_Flag_Bit : u16 {
	Rigid_Body_COLLIDING = bit(0),
	Rigid_Body_BOUNDING_BOX_COLLIDING = bit(1)
};

struct Rigid_Body {
	Rigid_Body_Type		type;
	Rigid_Body_Flags	flags;
	r32					imass;
	r32					drag;
	u32					grid_index;
	Vec2				velocity;
	Vec2				force;
	Transform			transform;
	r32					stiffness;
	r32					restitution;
	u32					fixture_count;
	Fixture *			fixtures;
	Mm_Rect				bounding_box;
	Entity_Id			entity_id;
	Rigid_Body* next;
};

enum Entity_Type {
	Entity_Type_Null,
	Entity_Type_Camera,
	Entity_Type_Player,
	Entity_Type_Obstacle,

	Entity_Type_Count
};

struct Entity {
	attribute(read_only)				Entity_Id	id;
	attribute(read_only, no_serialize)  Entity_Type type;
										Vec2 position;
};

struct Camera : Entity {
	r32 distance;
};

struct Player : public Entity {
	attribute(min:0, max:5)				r32 radius;
	attribute(color)					Vec4 color;
										r32 intensity;
	attribute(no_serialize)				Rigid_Body *rigid_body;
};

struct Obstacle : public Entity {
	attribute(color)		Vec4 color;
	attribute(no_serialize) Rigid_Body *rigid_body;
};
