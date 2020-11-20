#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "modules/core/data_structures.h"
#include "entity.h"

constexpr r32 SCENE_VIEW_HEIGHT_HALF = 1;
constexpr int SCENE_SIMULATION_MAX_ITERATION = 5;
constexpr r32 SCENE_SIMULATION_CORRECTION_ALPHA = 0.8f;

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

const Array_View<Camera> scene_cameras(Scene *scene);

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

void scene_save_resources(Scene *scene);
void scene_load_resources(Scene *scene);
void scene_clean_resources(Scene *scene);
void scene_clean_entities(Scene *scene);

//
//
//

bool scene_save_level(Scene *scene);
bool scene_load_level(Scene *scene, const String name);
void scene_unload_current_level(Scene *scene);
Level *scene_current_level_pointer(Scene *scene);
const char *scene_current_level(Scene *scene);
