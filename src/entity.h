#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/gfx_types.h"
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
	Vec2				velocity;
	Vec2				force;
	Transform			transform;
	r32					restitution;
	u32					fixture_count;
	Fixture *			fixtures;
	Mm_Rect				bounding_box;
	Entity_Id			entity_id;
};

enum Entity_Type {
	Entity_Type_Null,
	Entity_Type_Camera,
	Entity_Type_Character,
	Entity_Type_Obstacle,

	Entity_Type_Count
};

struct Entity {
	attribute(read_only)				Entity_Id	id;
	attribute(read_only, no_serialize)  Entity_Type type;
										Vec2 position;
};

enum Camera_Behaviour {
	Camera_Behaviour_STILL,
	Camera_Behaviour_ANIMATE
};

struct Camera_Lens {
										Camera_View_Kind	kind;
	attribute(slider, min:0, max:120)	r32					field_of_view;
										r32					near;
										r32					far;
};

struct Camera : public Entity {
	r32						distance;
	Vec2					target_position;
	r32						target_distance;
	r32						follow_factor;
	r32						zoom_factor;
	Camera_Behaviour		behaviour;

	Camera_Lens				lens;
};

struct Character : public Entity {
	attribute(min:0, max:5)				r32 radius;
	attribute(color)					Vec4 color;
										r32 intensity;
	attribute(no_serialize)				Rigid_Body *rigid_body;
};

struct Obstacle : public Entity {
	attribute(color)		Vec4 color;
	attribute(no_serialize) Rigid_Body *rigid_body;
};
