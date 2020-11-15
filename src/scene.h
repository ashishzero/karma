#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "modules/core/data_structures.h"
#include "entity.h"

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#define SCENE_DEVELOPER_TOOLS
#endif

typedef bool(*Collision_Resolver)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold);
typedef Impact_Type(*Continuous_Collision_Resolver)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Impact_Time *impact);
typedef bool(*Nearest_Points_Finder)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Nearest_Points *nearest_points);
typedef bool(*Collision_Detector)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb);

typedef u64 Resource_Id;
struct Resource_Fixture {
	Resource_Id id;
	Fixture *	fixtures;
	u32			fixture_count;
};

using Rigid_Body_List = Circular_Linked_List<Rigid_Body>;

struct Entity_Ref {
	Entity_Id	id;
	Entity_Type type;
	u32			index;
};

struct Entity_By_Type {
	Array<Camera>		camera;
	Array<Character>	character;
	Array<Obstacle>		obstacle;
};

struct World {
	r32			gravity;
};

enum Gizmo_Type {
	Gizmo_Type_NONE,
	Gizmo_Type_CENTER,
	Gizmo_Type_TRANSLATE_X,
	Gizmo_Type_TRANSLATE_Y,
	Gizmo_Type_SCALE_X,
	Gizmo_Type_SCALE_Y,
	Gizmo_Type_ROTOR,

	Gizmo_Type_COUNT
};

enum Gizmo_Render_Type {
	Gizmo_Render_Type_NONE,
	Gizmo_Render_Type_TRANSLATE,
	Gizmo_Render_Type_SCALE,
	Gizmo_Render_Type_ROTATE,

	Gizmo_Render_Type_COUNT
};

struct Gizmo {
	Gizmo_Type type;
	Gizmo_Render_Type render_type;
	r32 intensity[4]; // center, x, y, rotor
	r32 values[2];
};

struct Scene_Editor {
	Rigid_Body *hovered_body;
	Rigid_Body *selected_body;
	Gizmo		gizmo;
	bool		open_fixture;
};

struct Scene_Debug {
	Array<Contact_Manifold> manifold;
	Scene_Editor			editor;
};

struct Scene {
	Array<Entity_Ref> entity;

	Entity_By_Type	by_type;
	Rigid_Body_List rigid_bodies;

	World			world;

	Array<Resource_Fixture>		resource_fixtures;
	Allocator					pool_allocator;

	Random_Series	id_series;

#ifdef SCENE_DEVELOPER_TOOLS
	Scene_Debug debug;
#endif
};

void scene_prepare();
Scene *scene_create();
void scene_destroy(Scene *scene);

Entity *scene_find_entity(Scene *scene, Entity_Id id);

Resource_Fixture *scene_find_resource_fixture(Scene *scene, Resource_Id id);
Resource_Id scene_create_new_resource_fixture(Scene *scene, Fixture *fixtures, u32 fixture_count);
bool scene_delete_resource_fixture(Scene *scene, Resource_Id id);

inline Fixture *rigid_body_get_fixture(Rigid_Body *rigid_body, u32 index) {
	assert(index < rigid_body->fixture_count);
	return rigid_body->fixtures + index;
}

Mm_Rect rigid_body_bounding_box(Rigid_Body *body, r32 dt);

struct Camera_Info {
	r32					distance;
	Vec2				target_position;
	r32					target_distance;
	r32					follow_factor;
	r32					zoom_factor;
	Camera_Behaviour	behaviour;

	Camera_Lens			lens;
};

struct Rigid_Body_Info {
	Rigid_Body_Type		type;
	booli				fixture;
	Resource_Id			fixture_id;
	Transform			transform;
};

struct Entity_Info {
	Vec2			position;
	void *			data;
};

Entity *scene_create_new_entity(Scene *scene, Entity_Type type, const Entity_Info &info);

//
//
//

bool scene_handle_event(Scene *scene, const Event &event);

void scene_pre_simulate(Scene *scene);
void scene_simulate(Scene *scene, r32 dt);
void scene_update(Scene *scene, r32 window_w, r32 window_h);

typedef u32 Scene_Render_Flags;
enum Scene_Render_Bit : Scene_Render_Flags {
	Scene_Render_WORLD = bit(0),
	Scene_Render_FIXTURE = bit(1),
	Scene_Render_COLLISION = bit(2),
	Scene_Render_EDITOR = bit(3),
};

void scene_render(Scene *scene, r32 alpha, r32 aspect_ratio, Scene_Render_Flags flags);

//
//
//

void serialize_entity(Scene *scene, Entity *entity, Ostream *out);
bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state);
