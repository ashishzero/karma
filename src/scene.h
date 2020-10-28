#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "entity.h"

struct Entity_By_Type {
	Array<Player> player;
	Array<Static_Body> static_body;
};

template <typename Node_Data_Type>
struct Circular_Linked_List {
	struct Node {
		Node *prev;
		Node *next;
		Node_Data_Type data;
	};

	Node node;
	Allocator allocator;
};

template <typename T>
typename Circular_Linked_List<T>::Node *circular_linked_list_add(Circular_Linked_List<T> *list, u32 count = 1) {
	auto node = new(list->allocator)Circular_Linked_List<T>::Node[count];

	node[0].prev = list->node.prev;
	for (u32 index = 1; index < count; ++index) {
		node[index].prev = node + index - 1;
	}

	node[count - 1].next = &list->node;
	for (u32 index = 0; index < count - 1; ++index) {
		node[index].next = node + index + 1;
	}

	list->node.prev->next = node;
	list->node.prev = node + count - 1;

	return node;
}

template <typename T>
void circular_linked_list_remove(Circular_Linked_List<T> *list, typename Circular_Linked_List<T>::Node *nodes, u32 count = 1) {
	auto new_next = nodes[count - 1].next;
	auto new_prev = nodes[0].prev;
	new_next->prev = new_prev;
	new_prev->next = new_next;

	memory_free(nodes, list->allocator);
}

using Collider_List		= Circular_Linked_List<Collider>;
using Collider_Node		= Circular_Linked_List<Collider>::Node;
using Rigid_Body_List	= Circular_Linked_List<Rigid_Body>;

struct Scene {
	Camera			camera;
	Entity_By_Type	by_type;
	Rigid_Body_List rigid_bodies;
	Collider_List	colliders;
	Null			null_shape;
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

Collider_Group scene_create_colliders(Scene *scene, Entity *entity, u32 count);
void scene_destroy_colliders(Scene *scene, Collider_Group *group);

Rigid_Body *scene_create_rigid_body(Scene *scene, Entity *entity, u32 collider_count);
void scene_destroy_rigid_body(Scene *scene, Rigid_Body *rigid_body);

void *scene_attach_collider_type(Scene *scene, Collider_Node *node, Collider_Type type, Collider_Attachment *attachment);
#define scene_attach_collider(scene, node, type, attachment) (type *)scene_attach_collider_type(scene, node, Collider_##type, attachment)

//
//
//

void serialize_entity(Scene *scene, Entity *entity, Ostream *out);
bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state);
