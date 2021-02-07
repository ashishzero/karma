#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "modules/core/data_structures.h"
#include "entity.h"
#include "broad_phase.h"

constexpr u32 SCENE_MAX_ENTITY_COUNT = 524288; // NOTE: This must be power of 2 (2^19)
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
	Entity_Type type;
	u32			index;
};

struct Entity_Hash_Table {
	enum class Slot : u32 {
		EMPTY,
		OCCUPIED
	};

	struct Key {
		Entity_Id	id;
		Slot		slot;
	};

	Key *				keys;
	Entity_Reference *	references;
	Resource_Id *		resources;
	u32				count;
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

	Physics physics;

	Entity_Hash_Table entity_table;

	Entity_By_Type	by_type;

	Entity_Id player_id;

	Rigid_Body_List rigid_bodies;
	HGrid hgrid;

	Array<Resource_Header>	resource_header;
	Array<Fixture_Group>	fixture_group;
	Array<Texture_Group>	texture_group;

	Allocator					pool_allocator;

	s32				loaded_level;
	Array<Level>	levels;

	Array<Entity_Id>	removed_entity[Entity_Type_Count];

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

const Resource_Header *scene_create_new_resource(Scene *scene, Resource_Name &name, Fixture *fixtures, u32 fixture_count, const Resource_Name &tex_name, const Mm_Rect &uv);

struct Resource_Collection {
	Resource_Header *	header;
	Fixture_Group *		fixture;
	Texture_Group *		texture;
	u32					index;
};

Resource_Id					scene_find_entity_resource_id(Scene *scene, Entity_Id id);
const Resource_Collection	scene_find_resource(Scene *scene, Resource_Id id);

Entity *scene_clone_entity(Scene *scene, Entity *entity, Vec2 p, Resource_Id *resource = nullptr);
Entity_Reference scene_get_entity(Scene *scene, Entity_Id id);
bool scene_find_entity(Scene *scene, Entity_Id id, Entity_Reference *ref);
Entity *scene_entity_pointer(Scene *scene, Entity_Reference &reference);

const Array_View<Camera>			scene_cameras(Scene *scene);

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
Character *scene_get_player(Scene *scene);

Character *scene_spawn_player(Scene *scene, Vec2 p = vec2(0), Vec4 color = vec4(1));

//
//
//

void scene_remove_entity(Scene *scene, Entity_Reference &ref, Entity_Id id);
void scene_remove_entity(Scene *scene, Entity_Id id);

void scene_remove_entity_from_level(Scene *scene, Entity_Reference &ref, Entity_Id id);
void scene_remove_entity_from_level(Scene *scene, Entity_Id id);

//
//
//

bool scene_handle_event(Scene *scene, const Event &event);

void scene_begin(Scene *scene);
void scene_simulate(Scene *scene, r32 dt);
void scene_update(Scene *scene);
void scene_end(Scene *scene);

//
//
//

void scene_render_shape(const Circle &circle, Vec4 shade, Vec4 outline);
void scene_render_shape(const Mm_Rect &mm_rect, Vec4 shade, Vec4 outline);
void scene_render_shape(const Capsule &capsule, Vec4 shade, Vec4 outline);
void scene_render_shape(const Polygon &polyon, Vec4 shade, Vec4 outline);
void scene_render_shape(const Polygon_Pt &polyon, Vec4 shade, Vec4 outline);

void scene_render(Scene *scene, r32 alpha, r32 aspect_ratio);

//
//
//

bool scene_save_resource(Scene *scene, Resource_Header &h, Texture_Group &texture, Fixture_Group &fixture, bool pt_polygon);
void scene_save_resources(Scene *scene);
void scene_load_resources(Scene *scene);
void scene_reload_resources(Scene *scene, Resource_Id id);
void scene_clean_resources(Scene *scene);
void scene_clean_entities(Scene *scene);

//
//
//

bool scene_create_new_level(Scene *scene, const String name);
bool scene_save_level(Scene *scene);
bool scene_load_level(Scene *scene, const String name);
bool scene_reload_level(Scene *scene);
void scene_unload_current_level(Scene *scene);
Level *scene_current_level_pointer(Scene *scene);
const String scene_current_level(Scene *scene);

//
//
//

inline void ent_rigid_body_init(Entity *entity, Rigid_Body *body, Rigid_Body_Type type, Fixture_Group &fixture) {
	body->type = type;
	body->flags = 0;
	body->imass = ((type == Rigid_Body_Type_Static) ? 0.0f : 1.0f);
	body->drag = 5;
	body->restitution = 0;
	body->velocity = vec2(0);
	body->force = vec2(0);
	body->transform.p = entity->position;
	body->transform.xform = mat2_identity();
	body->fixtures = fixture.fixtures;
	body->fixture_count = fixture.count;
	body->entity_id = entity->id;
	body->bounding_box = scene_rigid_body_bounding_box(body, 0);
}

inline void ent_init_camera(Camera *camera, Vec2 p, r32 distance) {
	camera->id.handle = 0;
	camera->type = Entity_Type_Camera;
	camera->position = p;
	camera->target_position = p;
	camera->distance = distance;
	camera->target_distance = distance;
	camera->follow_factor = 1;
	camera->zoom_factor = 1;
	camera->behaviour = 0;
}

// TODO: dont take in index
inline void ent_init_character(Character *character, Scene *scene, Vec2 p, Vec4 color, Rigid_Body *body, Fixture_Group &fixture, Texture_Group &texture, u32 index) {
	character->id.handle = 0;
	character->type = Entity_Type_Character;
	character->position = p;
	character->radius = 0.1f;
	character->color = color;
	character->texture.index = index;
	character->controller.boost = 0;
	character->controller.axis = 0;
	ent_rigid_body_init(character, body, Rigid_Body_Type_Dynamic, fixture);
	character->rigid_body = body;
}

// TODO: dont take in index
inline void ent_init_obstacle(Obstacle *obstable, Scene *scene, Vec2 p, Rigid_Body *body, Fixture_Group &fixture, Texture_Group &texture, u32 index) {
	obstable->id.handle = 0;
	obstable->type = Entity_Type_Obstacle;
	obstable->position = p;
	obstable->color = vec4(1);
	obstable->texture.index = index;
	ent_rigid_body_init(obstable, body, Rigid_Body_Type_Static, fixture);
	obstable->rigid_body = body;
}
