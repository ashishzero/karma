#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/gfx_types.h"
#include "modules/core/reflection.h"

struct Physics {
	r32 gravity = 0.1f;
};

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

struct Resource_Header {
	Resource_Id		id;
	Resource_Name	name;
	Resource_Name	texture;
};

struct Fixture_Group {
	Fixture *fixtures;
	u32		 count;
};

struct Texture_Group {
	Texture2d_Handle handle;
	Mm_Rect			 uv;
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

struct attribute(v:1) Rigid_Body {
	attribute(read_only)						Rigid_Body_Type		type;
	attribute(no_serialize, no_display)			Rigid_Body_Flags	flags;
	attribute(min:0)							r32					imass;
	attribute(min:0)							r32					drag;
	attribute(slider, min:0, max:1)				r32					restitution;
	attribute(slider, min:0, max:1, v:1)		r32					gravity;
	attribute(no_serialize, read_only)			Vec2				velocity;
	attribute(no_serialize, read_only)			Vec2				force;
												Transform			transform;
	attribute(no_serialize, no_display)			u32					fixture_count;
	attribute(no_serialize, no_display)			Fixture *			fixtures;
	attribute(no_serialize, read_only)			Mm_Rect				bounding_box;
	attribute(no_serialize, no_display)			Entity_Id			entity_id;
	attribute(no_serialize, no_display)	        void*			next;
	attribute(no_serialize, no_display)			u32				grid_index;
};

enum Entity_Type : u32 {
	Entity_Type_Camera,
	Entity_Type_Character,
	Entity_Type_Obstacle,
	Entity_Type_Bullet,

	Entity_Type_Count
};

struct Camera;
struct Character;
struct Obstacle;
struct Bullet;

struct Entity {
	attribute(read_only)				Entity_Id	id;
	attribute(read_only)				Entity_Type type;
										Vec2 position;

										template <typename T>
										T *as() {
											return nullptr;
										}

										template <>
										Camera *as() {
											assert(type == Entity_Type_Camera);
											return (Camera *)this;
										}

										template <>
										Character *as() {
											assert(type == Entity_Type_Character);
											return (Character *)this;
										}

										template <>
										Obstacle *as() {
											assert(type == Entity_Type_Character);
											return (Obstacle *)this;
										}

										template <>
										Bullet *as() {
											assert(type == Entity_Type_Bullet);
											return (Bullet *)this;
										}
};

struct Entity_Controller {
	r32 boost;
	r32 axis;
	Vec2 pointer;
	booli attack;
	r32 cool_down;
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
	attribute(min:0)						r32 radius;
	attribute(color)						Vec4 color;
	attribute(no_serialize)					Texture_Id texture;
	attribute(no_display, no_serialize)		Entity_Controller controller;
	attribute(no_serialize)					Rigid_Body *rigid_body;
};

struct attribute(no_serialize_base) Obstacle : public Entity {
	attribute(color)				Vec4 color;
	attribute(no_serialize)			Texture_Id texture;
	attribute(no_serialize)			Rigid_Body *rigid_body;
};

struct attribute(no_serialize_base) Bullet : public Entity {
	attribute(min:0)				r32 radius;
	attribute(min:0)				r32 intensity;
	attribute(color)				Vec4 color;
	attribute(min:0)				r32 age;
	attribute(min:0)				r32 life_span;
	attribute(no_serialize)			Rigid_Body *rigid_body;
};

//
//
//

typedef char Level_Name[125];
struct Level {
	attribute(text)		Level_Name	name;
	attribute(no_display) u32		name_count;
	attribute(no_display) u32		key;
};
