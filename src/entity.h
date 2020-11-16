#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/gfx_types.h"
#include "modules/core/reflection.h"

typedef u64 Entity_Id;

enum Fixture_Shape : u32 {
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

typedef u64 Resource_Id;
typedef char Resource_Name[125];

struct Resource_Fixture {
	attribute(read_only)				Resource_Id		id;
	attribute(text)						Resource_Name	name;
	attribute(no_serialize, no_display)	Fixture *		fixtures;
	attribute(no_serialize, no_display)	u32				fixture_count;
};

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
	attribute(read_only)			Rigid_Body_Type		type;
	attribute(no_display)			Rigid_Body_Flags	flags;
	attribute(min:0)				r32					imass;
	attribute(min:0)				r32					drag;
	attribute(slider, min:0, max:1)	r32					restitution;
	attribute(read_only)			Vec2				velocity;
	attribute(read_only)			Vec2				force;
									Transform			transform;
	attribute(no_display)			u32					fixture_count;
	attribute(no_display)			Fixture *			fixtures;
	attribute(read_only)			Mm_Rect				bounding_box;
	attribute(no_display)			Entity_Id			entity_id;
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
	attribute(read_only)				Entity_Type type;
										Vec2 position;
};

enum Camera_Behaviour : u32 {
	Camera_Behaviour_STILL = 0,
	Camera_Behaviour_ANIMATE_MOVEMENT = bit(0),
	Camera_Behaviour_ANIMATE_FOCUS = bit(1),
};

struct Camera_Lens {
	attribute(min:0, max:120)	r32					field_of_view;
								r32					near;
								r32					far;
};

struct Camera : public Entity {
									r32					distance;
	attribute(no_display)			Vec2				target_position;
	attribute(no_display)			r32					target_distance;
	attribute(slider, min:0, max:1)	r32					follow_factor;
	attribute(slider, min:0, max:1)	r32					zoom_factor;
	attribute(no_display)			u32					behaviour;
									Camera_Lens			lens;
};

struct Character : public Entity {
	attribute(min:0)		r32 radius;
	attribute(color)		Vec4 color;
							Rigid_Body *rigid_body;
};

struct Obstacle : public Entity {
	attribute(color)		Vec4 color;
							Rigid_Body *rigid_body;
};
