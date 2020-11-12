#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "modules/core/data_structures.h"
#include "entity.h"

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

struct Scene {
	Array<Entity_Ref> entity;

	Entity_By_Type	by_type;
	Rigid_Body_List rigid_bodies;

	Array<Resource_Fixture>		resource_fixtures;
	Allocator					pool_allocator;

	Random_Series	id_series;
};

Scene *scene_create();
void scene_destroy(Scene *scene);

Resource_Fixture *scene_find_resource_fixture(Scene *scene, Resource_Id id);
Resource_Id scene_create_new_resource_fixture(Scene *scene, Fixture *fixtures, u32 fixture_count);
bool scene_delete_resource_fixture(Scene *scene, Resource_Id id);

Character *scene_add_character(Scene *scene);
Obstacle *scene_add_obstacle(Scene *scene);

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

void serialize_entity(Scene *scene, Entity *entity, Ostream *out);
bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state);
