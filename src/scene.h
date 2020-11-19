#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "modules/core/data_structures.h"
#include "entity.h"

constexpr r32 SCENE_VIEW_HEIGHT_HALF = 5;

struct Scene;


#ifdef ENABLE_DEVELOPER_OPTIONS
#include "scene_debug.h"
#endif

typedef bool(*Collision_Resolver)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold);
typedef Impact_Type(*Continuous_Collision_Resolver)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Impact_Time *impact);
typedef bool(*Nearest_Points_Finder)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Nearest_Points *nearest_points);
typedef bool(*Collision_Detector)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb);

using Rigid_Body_List = Circular_Linked_List<Rigid_Body>;

struct Entity_Reference {
	Entity_Id	id;
	Entity_Type type;
	u32			offset;
};

struct Scene {
	union Entity_By_Type {
		struct {
			Array<Camera>		camera;
			Array<Character>	character;
			Array<Obstacle>		obstacle;
		};
		struct {
			Array<u8>	data[Entity_Type_Count];
		};
	};

	Array<Entity_Reference> entity;

	Entity_By_Type	by_type;
	Rigid_Body_List rigid_bodies;

	Array<Resource_Fixture>		resource_fixtures;
	Allocator					pool_allocator;

	s32				loaded_level;
	Array<Level>	levels;

	Random_Series	id_series;

	#ifdef ENABLE_DEVELOPER_OPTIONS
	struct State {
		enum Type {
			GAME, GAME_DEVELOPER, LEVEL_EDITOR, ENTITY_EDITOR
		};

		enum Flags : u32 {
			RENDER_WORLD = bit(0),
			RENDER_FIXTURE = bit(1),
			RENDER_COLLISION = bit(2),
			RENDER_EDITOR = bit(3),
		};

		Type type;
		u32 flags;
	};

	State state;
	Array<Contact_Manifold> manifolds;

	Editor editor;
	#endif
};

void scene_prepare();

//
//
//

Scene *scene_create();
void scene_destroy(Scene *scene);

//
//
//

Resource_Id			scene_create_new_resource_fixture(Scene *scene, String name, Fixture *fixtures, u32 fixture_count);
Resource_Fixture *	scene_find_resource_fixture(Scene *scene, Resource_Id id);
Resource_Id			scene_find_resource_fixture_from_fixture(Scene *scene, Fixture *fixture);

Entity *scene_create_new_entity(Scene *scene, Entity *entity, Vec2 p);
Entity *scene_find_entity(Scene *scene, Entity_Id id);

//
//
//

Fixture *	scene_rigid_body_get_fixture(Rigid_Body *body, u32 index);
Mm_Rect		scene_rigid_body_bounding_box(Rigid_Body *body, r32 dt);
void		scene_rigid_body_update_bounding_box(Rigid_Body *body, r32 dt);
void		scene_refresh_rigid_bodies(Scene *scene);
Rigid_Body *scene_collide_point(Scene *scene, Vec2 point, r32 size = 0);

//
//
//

Camera *scene_primary_camera(Scene *scene);

//
//
//

void scene_delete_all_resource_fixture(Scene *scene);

void scene_release_all_entity(Scene *scene);

// TODO
//
// -Make a specific place for allocation, deallocation, inserting and removing entities
// -Remove unnecessary indices and ids, and try to make a single handle for single entity
//	because it is confusing with indices, handles, pointers and references, too many 
//	things to worry about
// -Seperate debug code and runtime code, remove code that is added for debug optimization
// -Do not use scene structure for handling editor, use seperate structures for editor and debug things
//
// -Finish all of this tomorrow
//

//
//
//

bool scene_handle_event(Scene *scene, const Event &event);

void scene_pre_simulate(Scene *scene);
void scene_simulate(Scene *scene, r32 dt);
void scene_update(Scene *scene);

//
//
//

void scene_render(Scene *scene, r32 alpha, r32 aspect_ratio);

//
//
//

Level *scene_create_new_level(Scene *scene, const String name);
void scene_set_level(Scene *scene, s32 index);
Level *scene_current_level(Scene *scene);

//
//
//

void scene_save_resources(Scene *scene);
void scene_load_resources(Scene *scene);

//
//
//

bool scene_save_level(Scene *scene);
s32 scene_load_level(Scene *scene, const String name);
