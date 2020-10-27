#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "entity.h"

struct Entity_By_Type {
	Array<Player> player;
	Array<Static_Body> static_body;
};

struct Collider_Node {
	Collider_Node *prev;
	Collider_Node *next;
	Collider	   collider;
};

struct Collider_List {
	Collider_Node node;
	Allocator allocator;
	Null null_shape;
};

struct Scene {
	Entity_By_Type	by_type;
	Collider_List	colliders;
	Allocator		collider_shape_allocator;
	Random_Series	entity_id_series;
};

struct Collider_Attachment {
	u32 polygon_n;
};

//
//
//

Scene *scene_create();
void scene_destroy(Scene *scene);

Player *scene_add_player(Scene *scene);
Static_Body *scene_add_static_body(Scene *scene);

void scene_generate_new_entity(Scene *scene, Entity *entity, Vec2 position);

Collider scene_null_collider(Scene *scene);
Collider_Node *collider_node(Collider_Handle handle, u32 index);
Collider *collider_get(Scene *scene, Collider_Node *node);

Collider_Group scene_create_colliders(Scene *scene, u32 count);
void scene_destroy_colliders(Scene *scene, Collider_Group *_group);

void *scene_attach_collider_type(Scene *scene, Collider_Node *node, Collider_Type type, Collider_Attachment *attachment);
#define scene_attach_collider(scene, node, type, attachment) (type *)scene_attach_collider_type(scene, node, Collider_##type, attachment)

//
//
//

void serialize_entity(Scene *scene, Entity *entity, Ostream *out);
bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state);
