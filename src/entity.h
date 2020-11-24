#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/gfx_types.h"
#include "modules/core/reflection.h"

struct Entity_Id {
	u64 handle;
};

struct Texture_Id {
	u32 index;
};

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

struct Resource_Id {
	u64 handle;
};
typedef char Resource_Name[125];

struct Resource_Fixture {
	attribute(read_only)				Resource_Id		id;
	attribute(text)						Resource_Name	name;
	attribute(no_serialize, no_display)	Fixture *		fixtures;
	attribute(no_serialize, no_display)	u32				fixture_count;
};

struct attribute(v:1) Resource_Entity {
					Entity_Id		id;
					Resource_Id		fixture_id;
	attribute(v:1)	Resource_Name	texture_name;
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
	attribute(read_only)						Rigid_Body_Type		type;
	attribute(no_serialize, no_display)			Rigid_Body_Flags	flags;
	attribute(min:0)							r32					imass;
	attribute(min:0)							r32					drag;
	attribute(slider, min:0, max:1)				r32					restitution;
	attribute(no_serialize, read_only)			Vec2				velocity;
	attribute(no_serialize, read_only)			Vec2				force;
												Transform			transform;
	attribute(no_serialize, no_display)			u32					fixture_count;
	attribute(no_serialize, no_display)			Fixture *			fixtures;
	attribute(no_serialize, read_only)			Mm_Rect				bounding_box;
	attribute(no_serialize, no_display)			Entity_Id			entity_id;
};

enum Entity_Type : u32 {
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

struct attribute(no_serialize_base) Camera : public Entity {
									r32					distance;
	attribute(no_display)			Vec2				target_position;
	attribute(no_display)			r32					target_distance;
	attribute(slider, min:0, max:1)	r32					follow_factor;
	attribute(slider, min:0, max:1)	r32					zoom_factor;
	attribute(no_display)			u32					behaviour;
};

#define camera_distance_to_scale(camera) powf(0.5f, camera->distance)

struct attribute(no_serialize_base, v:1) Character : public Entity {
	attribute(min:0)				r32 radius;
	attribute(color)				Vec4 color;
	attribute(v:1)					Mm_Rect	texture_rect;
	attribute(no_serialize, v:1)	Texture_Id texture;
	attribute(no_serialize)			Rigid_Body *rigid_body;
};

struct attribute(no_serialize_base, v:1) Obstacle : public Entity {
	attribute(color)				Vec4 color;
	attribute(v:1)					Mm_Rect	texture_rect;
	attribute(no_serialize, v:1)	Texture_Id texture;
	attribute(no_serialize)	Rigid_Body *rigid_body;
};

//
//
//

typedef char Level_Name[125];
struct Level {
	attribute(no_serialize, text)		Level_Name				name;
	attribute(no_serialize, no_display) u32						name_count;
	attribute(no_serialize, no_display) u32						key;
	attribute(no_display)				Array<Resource_Entity>	resources;
};
