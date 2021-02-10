#pragma once

#include "modules/core/lin_maths.h"
#include "modules/core/gfx_types.h"
#include "modules/core/reflection.h"
#include "modules/core/random.h"

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
	attribute(no_serialize, no_display)			u32				level;
};

enum Entity_Type : u32 {
	Entity_Type_Camera,
	Entity_Type_Character,
	Entity_Type_Obstacle,
	Entity_Type_Bullet,
	Entity_Type_Particle_Emitter,

	Entity_Type_Count
};

struct Audio;

struct Camera;
struct Character;
struct Obstacle;
struct Bullet;
struct Particle_Emitter;

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
											assert(type == Entity_Type_Obstacle);
											return (Obstacle *)this;
										}

										template <>
										Bullet *as() {
											assert(type == Entity_Type_Bullet);
											return (Bullet *)this;
										}

										template <>
										Particle_Emitter *as() {
											assert(type == Entity_Type_Particle_Emitter);
											return (Particle_Emitter *)this;
										}
};

struct Entity_Controller {
	Vec2 axis;
	Vec2 pointer;
	booli attack;
	r32 cool_down;
};

struct Particle_Emitter_Property {
	enum Kind {
		POINT,
		LINE,
		ELLIPSE,
		TRIANGLE,
		RECT,
	};
	/*
	 * If kind is POINT, a is used
	 * if kind is LINE, a and b are used
	 * if kind is ELLIPSE, a and b are used where b.x and b.y gives x-radius and y-radius of ellipse
	 * if kind is TRIANGLE, a, b, c are used as 3 corner of the triangle around the origin
	 * if kind is RECT, then a is used as width and height of the rectangle
	*/
	Kind                 kind;
	Distribution_Control control;
	Vec2                 a, b, c; // TODO: Perhaps we can remove TRIANGLE, that way we only have to store 'a'

						Random_Distribution density;
	attribute(min:0)	r32                 volume_factor;

	Random_Distribution scale_a;
	Random_Distribution scale_b;

	Random_Distribution spin;
	Random_Distribution rotation;

	Random_Distribution initial_velocity_x;
	Random_Distribution initial_velocity_y;
	Random_Distribution force_x;
	Random_Distribution force_y;

	Random_Distribution drag;

	attribute(color, hsv)	Color4 color_a;
	attribute(color, hsv)	Color4 color_b;
	attribute(min:0, max:1)	r32    opacity;
	attribute(min:0, max:1)	r32    intensity; // TODO: Make this per particle!

	Random_Distribution life_span;
	
	attribute(min:0)		u32                 emission_rate; // in particles per 60-milli secs
	attribute(min:0, max:1)	r32                 fade_in;       // in secs
	attribute(min:0, max:1)	r32                 fade_out;      // in secs
};

struct Particle {
	r32    density;
	Vec2   position;
	r32    scale_a;
	r32    scale_b;
	r32    rotation;
	Vec2   velocity;
	r32    drag;
	r32    spin;
	Color4 color_a;
	Color4 color_b;
	r32    life_span; // in secs
	r32    life;      // in secs
	Vec2   external_force;
};

struct Particle_System {
										Vec2						position;
	attribute(no_display, no_serialize) Texture_Id					texture;
										Particle_Emitter_Property	properties;
	attribute(no_display, no_serialize)	Particle *					particles;
										u32							particles_count;
										s32							loop;
	attribute(no_display, no_serialize)	u32							emit_count; // counts per sec
	attribute(no_display, no_serialize)	r32							time_elapsed; // in milli-secs
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

struct attribute(no_serialize_base, v:2) Character : public Entity {
											r32 rotation;
											Vec2 size;
	attribute(color)						Vec4 color;
	attribute(no_serialize)					Texture_Id texture;
											Particle_System particle_system;
	attribute(no_display, no_serialize)		Entity_Controller controller;
	attribute(no_display, no_serialize)		Audio *boost;
	attribute(no_display, no_serialize)		Audio *fall;
	attribute(no_serialize)					Rigid_Body *rigid_body;
};

struct attribute(no_serialize_base, v:1) Obstacle : public Entity {
	attribute(v:1)					r32  rotation;
	attribute(v:1)					Vec2 size;
	attribute(color)				Vec4 color;
	attribute(no_serialize)			Texture_Id texture;
	attribute(no_serialize)			Rigid_Body *rigid_body;
};

struct attribute(no_serialize_base) Bullet : public Entity {
	attribute(min:0)						r32 radius;
	attribute(min:0)						r32 intensity;
	attribute(color)						Vec4 color;
	attribute(min:0)						r32 age;
	attribute(min:0)						r32 life_span;
	attribute(no_display, no_serialize)		Audio *audio;
	attribute(no_serialize)					Rigid_Body *rigid_body;
};

struct attribute(no_serialize_base) Particle_Emitter : public Entity {
	Particle_System particle_system;
	booli			remove_on_finish;
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
