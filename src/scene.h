#pragma once
#include "modules/core/karma.h"
#include "modules/core/utility.h"
#include "modules/core/random.h"
#include "entity.h"

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
typename Circular_Linked_List<T>::Node *circular_linked_list_add(Circular_Linked_List<T> *list) {
	auto node = new(list->allocator)Circular_Linked_List<T>::Node;

	node[0].prev = list->node.prev;
	node[0].next = &list->node;

	list->node.prev->next = node;
	list->node.prev = node;

	return node;
}

template <typename T>
void circular_linked_list_remove(Circular_Linked_List<T> *list, typename Circular_Linked_List<T>::Node *nodes) {
	auto new_next = nodes[0].next;
	auto new_prev = nodes[0].prev;
	new_next->prev = new_prev;
	new_prev->next = new_next;

	memory_free(nodes, list->allocator);
}

struct Collider_Reference {
	Entity_Id	entity_id;
	Collider	collider;
};

struct Raw_Collider_Group {
	Raw_Collider_Id id;
	Collider *	colliders;
	u32			colliders_count;
};

using Collider_List		= Circular_Linked_List<Collider_Group>;
using Collider_Node		= Circular_Linked_List<Collider_Group>::Node;
using Rigid_Body_List	= Circular_Linked_List<Rigid_Body>;

struct Entity_By_Type {
	Array<Player>		player;
	Array<Static_Body>	static_body;
};

struct Scene {
	Camera			camera;
	Entity_By_Type	by_type;
	Rigid_Body_List rigid_bodies;
	Collider_List	colliders;
	Collider		null_collider;
	Allocator		collider_shape_allocator;

	Array<Raw_Collider_Group>	raw_colliders;
	Allocator					pool_allocator;

	Random_Series	id_series;
};

//
//
//

Scene *scene_create();
void scene_destroy(Scene *scene);

Raw_Collider_Group *scene_find_raw_collider_group(Scene *scene, Raw_Collider_Id id);
Raw_Collider_Id scene_add_raw_collider_group(Scene *scene, Array_View<Collider> &colliders);
bool scene_remove_raw_collider_group(Scene *scene, Raw_Collider_Id id);

Player *scene_add_player(Scene *scene);
Static_Body *scene_add_static_body(Scene *scene);

void scene_generate_new_entity(Scene *scene, Entity *entity, Vec2 position, Raw_Collider_Id collider_id);

Collider *collider_get(Collider_Group *group, u32 index);

Collider_Group *scene_create_colliders(Scene *scene, Rigid_Body *rigid_body, Raw_Collider_Id id, const Mat3 &transform);
void scene_destroy_colliders(Scene *scene, Collider_Group *group);

Rigid_Body *scene_create_rigid_body(Scene *scene, Entity *entity, Raw_Collider_Id id);
void scene_destroy_rigid_body(Scene *scene, Rigid_Body *rigid_body);

//
//
//

void serialize_entity(Scene *scene, Entity *entity, Ostream *out);
bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state);
