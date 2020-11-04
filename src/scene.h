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

struct Entity_By_Type {
	Array<Player>		player;
	Array<Obstacle>		obstacle;
};

struct Scene {
	Camera			camera;
	Entity_By_Type	by_type;
	Rigid_Body_List rigid_bodies;

	Array<Resource_Fixture>		resource_fixtures;
	Allocator					pool_allocator;

	Random_Series	id_series;
};


struct Scene;

Scene *scene_create();
void scene_destroy(Scene *scene);

Resource_Fixture *scene_find_resource_fixture(Scene *scene, Resource_Id id);
Resource_Id scene_create_new_resource_fixture(Scene *scene, Fixture *fixtures, u32 fixture_count);
bool scene_delete_resource_fixture(Scene *scene, Resource_Id id);

Player *scene_add_player(Scene *scene);
Obstacle *scene_add_obstacle(Scene *scene);

inline Fixture *rigid_body_get_fixture(Rigid_Body *rigid_body, u32 index) {
	assert(index < rigid_body->fixture_count);
	return rigid_body->fixtures + index;
}

struct Rigid_Body_Info {
	Rigid_Body_Type		type;
	Resource_Id			fixture_id;
	Transform			transform;
};

struct Entity_Info {
	Vec2			position;
	Rigid_Body_Info rigid_body;
};

Entity *scene_create_new_entity(Scene *scene, Entity_Type type, const Entity_Info &info);

//
//
//

void serialize_entity(Scene *scene, Entity *entity, Ostream *out);
bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state);
