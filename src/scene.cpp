#include "scene.h"
#include <time.h>

#include "modules/core/systems.h"
#include ".generated/entity.typeinfo"

Scene *scene_create() {
	Scene *scene = new Scene;
	
	scene->colliders.allocator = context.allocator;
	scene->colliders.node.next = &scene->colliders.node;
	scene->colliders.node.prev = &scene->colliders.node;
	scene->colliders.null_shape._placeholder = 0;
	scene->colliders.node.collider.flags = 0;
	scene->colliders.node.collider.type = Collider_Null;
	scene->colliders.node.collider.handle = &scene->colliders.null_shape;

	scene->collider_shape_allocator = context.allocator;

	scene->entity_id_series = random_init(context.id, system_get_counter());

	return scene;
}

void scene_destroy(Scene *scene) {
	memory_free(scene);
}

Player *scene_add_player(Scene *scene) {
	Player *player = array_add(&scene->by_type.player);
	player->type = Entity_Player;
	return player;
}

Static_Body *scene_add_static_body(Scene *scene) {
	Static_Body *body = array_add(&scene->by_type.static_body);
	body->type = Entity_Static_Body;
	return body;
}

u64 iscene_generate_unique_entity_id(Scene *scene) {
	return random_get(&scene->entity_id_series) | ((time(0) & 0xffffffff) << 32);
}

void scene_generate_new_entity(Scene *scene, Entity *entity, Vec2 position) {
	entity->id = iscene_generate_unique_entity_id(scene);
	entity->position = position;

	switch (entity->type) {
	case Entity_Player: {
		auto player = (Player *)entity;
		player->rradius = 1;
		player->color = vec4(1);
		player->collider.center = vec2(0);
		player->collider.radius = 1;
		player->velocity = vec2(0);
		player->force = vec2(0);
		player->transformed_collider.count = 0;
		player->transformed_collider.handle = nullptr;
	} break;

	case Entity_Static_Body: {
		auto body = (Static_Body *)entity;
		body->color = vec4(1);
		body->colliders.count = 0;
		body->colliders.handle = nullptr;
	} break;
	}
}

Collider scene_null_collider(Scene *scene) {
	return scene->colliders.node.collider;
}

Collider_Node *collider_node(Collider_Handle handle, u32 index) {
	return ((Collider_Node *)handle) + index;
}

Collider *collider_get(Scene *scene, Collider_Node *node) {
	return &node->collider;
}

Collider_Group scene_create_colliders(Scene *scene, u32 count) {
	Collider_Node *node = new(scene->colliders.allocator) Collider_Node[count];
	node->collider = scene->colliders.node.collider;

	node[0].prev = scene->colliders.node.prev;
	for (u32 index = 1; index < count; ++index) {
		node[index].prev = node + index - 1;
	}

	node[count - 1].next = &scene->colliders.node;
	for (u32 index = 0; index < count - 1; ++index) {
		node[index].next = node + index + 1;
	}

	scene->colliders.node.prev->next = node;
	scene->colliders.node.prev = node + count - 1;

	for (u32 index = 0; index < count; ++index) {
		node[index].collider = scene_null_collider(scene);
	}

	Collider_Group group;
	group.count = count;
	group.handle = node;

	return group;
}

void scene_destroy_colliders(Scene *scene, Collider_Group *_group) {
	struct internal_Collider_Group {
		Collider_Node *nodes;
		u32 count;
	};
	auto group = (internal_Collider_Group *)_group;

	Collider_Node *new_next = group->nodes[group->count - 1].next;
	Collider_Node *new_prev = group->nodes[0].prev;
	new_next->prev = new_prev;
	new_prev->next = new_next;

	Collider_Node *ptr = group->nodes;
	for (u32 index = 0; index < group->count; ++index) {
		scene_attach_collider_type(scene, ptr, Collider_Null, 0);
		ptr += 1;
	}
	memory_free(group->nodes, scene->colliders.allocator);
	group->count = 0;
	group->nodes = nullptr;
}

void *scene_attach_collider_type(Scene *scene, Collider_Node *node, Collider_Type type, Collider_Attachment *attachment) {
	Collider &collider = node->collider;

	if (collider.type != Collider_Null) {
		memory_free(collider.handle);
	}

	collider.type = type;

	switch (type) {
	case Collider_Null:
		collider.handle = &scene->colliders.null_shape;
		break;

	case Collider_Circle:
		collider.handle = memory_allocate(sizeof(Circle), scene->collider_shape_allocator);
		break;

	case Collider_Mm_Rect:
		collider.handle = memory_allocate(sizeof(Mm_Rect), scene->collider_shape_allocator);
		break;

	case Collider_Capsule:
		collider.handle = memory_allocate(sizeof(Capsule), scene->collider_shape_allocator);
		break;

	case Collider_Polygon:
		collider.handle = memory_allocate(sizeof(Polygon) + sizeof(Vec2) * (attachment->polygon_n - 3), scene->collider_shape_allocator);
		break;

		invalid_default_case();
	};

	return collider.handle;
}

//
//
//

void serialize_collider(Scene *scene, Collider &collider, Ostream *out) {
	switch (collider.type) {
	case Collider_Null: {
		serialize_fmt_text(out, "collider_data", reflect_info<Null>(), (char *)collider.handle);
	} break;

	case Collider_Circle: {
		serialize_fmt_text(out, "collider_data", reflect_info<Circle>(), (char *)collider.handle);
	} break;

	case Collider_Mm_Rect: {
		serialize_fmt_text(out, "collider_data", reflect_info<Mm_Rect>(), (char *)collider.handle);
	} break;

	case Collider_Capsule: {
		serialize_fmt_text(out, "collider_data", reflect_info<Capsule>(), (char *)collider.handle);
	} break;

	case Collider_Polygon: {
		Polygon *polygon = collider_get_shape(&collider, Polygon);
		Array_View<Vec2> points;
		points.count = polygon->vertex_count;
		points.data = polygon->vertices;
		serialize_fmt_text(out, "collider_data", reflect_info(points), (char *)&points);
	} break;

		invalid_default_case();
	}
}

void serialize_collider_group(Scene *scene, Collider_Group &group, Ostream *out) {
	serialize_fmt_text(out, "collider_group_count", reflect_info(group.count), (char *)&group.count);

	for (u32 index = 0; index < group.count; ++index) {
		auto collider = collider_get(scene, collider_node(group.handle, index));
		serialize_fmt_text_next(out);
		serialize_fmt_text(out, "collider_type", reflect_info<Collider_Type>(), (char *)&collider->type);
		serialize_fmt_text_next(out);
		serialize_collider(scene, *collider, out);
	}
}

void serialize_entity(Scene *scene, Entity *entity, Ostream *out) {
	serialize_fmt_text(out, "type", reflect_info<Entity_Type>(), (char *)&entity->type);
	serialize_fmt_text_next(out);

	switch (entity->type) {
	case Entity_Player: {
		serialize_fmt_text(out, "player", reflect_info<Player>(), (char *)entity);
	} break;

	case Entity_Static_Body: {
		serialize_fmt_text(out, "static_body", reflect_info<Static_Body>(), (char *)entity);
		serialize_fmt_text_next(out);
		serialize_collider_group(scene, ((Static_Body *)entity)->colliders, out);
	} break;

		invalid_default_case();
	}

}

bool deserialize_collider(Scene *scene, Collider_Type type, Collider_Node *node, Deserialize_State *state) {
	bool result = false;

	switch (type) {
	case Collider_Null: {
		auto collider = scene_attach_collider(scene, node, Null, 0);
		Null temp; // we don't need to get value for null collider
		result = deserialize_fmt_text(state, "collider_data", reflect_info<Null>(), (char *)&temp);
	} break;

	case Collider_Circle: {
		auto collider = scene_attach_collider(scene, node, Circle, 0);
		result = deserialize_fmt_text(state, "collider_data", reflect_info<Circle>(), (char *)collider);
	} break;

	case Collider_Mm_Rect: {
		auto collider = scene_attach_collider(scene, node, Mm_Rect, 0);
		result = deserialize_fmt_text(state, "collider_data", reflect_info<Mm_Rect>(), (char *)collider);
	} break;

	case Collider_Capsule: {
		auto collider = scene_attach_collider(scene, node, Capsule, 0);
		result = deserialize_fmt_text(state, "collider_data", reflect_info<Capsule>(), (char *)collider);
	} break;

	case Collider_Polygon: {
		Array_View<Vec2> points;

		scoped_temporary_allocation();
		auto mark = push_temporary_allocator();
		result = deserialize_fmt_text(state, "collider_data", reflect_info(points), (char *)&points);
		pop_temporary_allocator(mark);

		if (!result) break;

		Collider_Attachment attachment;
		attachment.polygon_n = (u32)points.count;
		auto collider = scene_attach_collider(scene, node, Polygon, &attachment);
		collider->vertex_count = (u32)points.count;
		memcpy(collider->vertices, points.data, sizeof(Vec2) * points.count);
	} break;

		invalid_default_case();
	}

	return result;
}

bool deserialize_collider_group(Scene *scene, Collider_Group *group, Deserialize_State *state) {
	u32 count;
	if (!deserialize_fmt_text(state, "collider_group_count", reflect_info(count), (char *)&count))
		return false;

	*group = scene_create_colliders(scene, count);
	Collider_Type type;

	for (u32 index = 0; index < group->count; ++index) {
		if (!deserialize_fmt_text(state, "collider_type", reflect_info<Collider_Type>(), (char *)&type))
			return false;
		if (!deserialize_collider(scene, type, collider_node(group->handle, index), state))
			return false;
	}
	return true;
}

bool deserialize_entity(Scene *scene, Entity *entity, Deserialize_State *state) {
	bool result = deserialize_fmt_text(state, "type", reflect_info<Entity_Type>(), (char *)&entity->type);
	if (!result) return false;

	switch (entity->type) {
	case Entity_Player: {
		result = deserialize_fmt_text(state, "player", reflect_info<Player>(), (char *)entity);
		auto player = (Player *)entity;
		player->transformed_collider = scene_create_colliders(scene, 1);
		scene_attach_collider(scene, collider_node(player->transformed_collider.handle, 0), Circle, 0);
	} break;

	case Entity_Static_Body: {
		if (!deserialize_fmt_text(state, "static_body", reflect_info<Static_Body>(), (char *)entity))
			return false;
		Collider_Group *group = &((Static_Body *)entity)->colliders;
		result = deserialize_collider_group(scene, group, state);
	} break;

		invalid_default_case();
	}

	return result;
}
