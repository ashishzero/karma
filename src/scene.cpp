#include "scene.h"
#include <time.h>

#include "modules/core/systems.h"
#include ".generated/entity.typeinfo"

Scene *scene_create() {
	Scene *scene = new Scene;
	
	scene->camera.position = vec2(0);
	scene->camera.distance = 0.0f;

	scene->rigid_bodies.allocator = context.allocator;
	scene->rigid_bodies.node.next = &scene->rigid_bodies.node;
	scene->rigid_bodies.node.prev = &scene->rigid_bodies.node;
	
	scene->colliders.allocator = context.allocator;
	scene->colliders.node.next = &scene->colliders.node;
	scene->colliders.node.prev = &scene->colliders.node;

	scene->null_collider.type = Collider_Null;
	scene->null_collider.handle = nullptr;

	scene->colliders.node.data.collider	= &scene->null_collider;
	scene->colliders.node.data.count		= 0;
	scene->colliders.node.data.transform	= mat3_identity();

	scene->collider_shape_allocator = context.allocator;

	scene->id_series = random_init(context.id, system_get_counter());

	scene->pool_allocator = context.allocator;

	// Add a Null Collider
	Raw_Collider_Group null_collider;
	null_collider.id = 0;
	null_collider.colliders_count = 1;
	null_collider.colliders = scene->colliders.node.data.collider;
	array_add(&scene->raw_colliders, null_collider);

	return scene;
}

void scene_destroy(Scene *scene) {
	// TODO: Distroy other objects as well
	memory_free(scene);
}

u64 iscene_generate_unique_id(Scene *scene) {
	return random_get(&scene->id_series) | ((time(0) & 0xffffffff) << 32);
}

Raw_Collider_Group *scene_find_raw_collider_group(Scene *scene, Raw_Collider_Id id) {
	auto res = array_find(&scene->raw_colliders, [](const Raw_Collider_Group &col, Raw_Collider_Id id) {
		return col.id == id;
		}, id);
	if (res >= 0)
		return scene->raw_colliders.data + res;
	return nullptr;
}

Raw_Collider_Id scene_add_raw_collider_group(Scene *scene, Array_View<Collider> &colliders) {
	Raw_Collider_Group *raw = array_add(&scene->raw_colliders);

	raw->id = iscene_generate_unique_id(scene);
	raw->colliders_count = (u32)colliders.count;
	raw->colliders = (Collider *)memory_allocate(sizeof(Collider) * raw->colliders_count, scene->pool_allocator);
	
	Collider *dst = raw->colliders;
	ptrsize size = 0;
	for (auto &src : colliders) {
		switch (src.type) {
		case Collider_Null: invalid_code_path(); break;
		case Collider_Circle: size = sizeof(Circle); break;
		case Collider_Mm_Rect: size = sizeof(Mm_Rect); break;
		case Collider_Capsule: size = sizeof(Capsule); break;
		case Collider_Polygon: size = sizeof(Polygon) + sizeof(Vec2) * ((Polygon *)src.handle)->vertex_count; break;

		invalid_default_case();
		}

		dst->type = src.type;
		dst->handle = memory_allocate(size, scene->pool_allocator);
		memcpy(dst->handle, src.handle, size);

		dst += 1;
	}

	return raw->id;
}

bool scene_remove_raw_collider_group(Scene *scene, Raw_Collider_Id id) {
	auto index = array_find(&scene->raw_colliders, [](const Raw_Collider_Group &col, Raw_Collider_Id id) {
		return col.id == id;
		}, id);
	if (index >= 0) {
		auto c = &scene->raw_colliders[index];
		for (u32 i = 0; i < c->colliders_count; ++i)
			memory_free(c->colliders[i].handle);
		memory_free(c->colliders);
		array_remove(&scene->raw_colliders, index);
		return true;
	}
	return false;
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

void scene_generate_new_entity(Scene *scene, Entity *entity, Vec2 position, Raw_Collider_Id collider_id) {
	entity->id = iscene_generate_unique_id(scene);
	entity->position = position;

	switch (entity->type) {
	case Entity_Player: {
		auto player = (Player *)entity;
		player->radius = 1;
		player->color = vec4(1);
		player->intensity = 1;
		player->collider_id = collider_id;
		player->rigid_body = scene_create_rigid_body(scene, entity, player->collider_id);
	} break;

	case Entity_Static_Body: {
		auto body = (Static_Body *)entity;
		body->color = vec4(1);
		body->collider_id = collider_id;
		body->colliders = scene_create_colliders(scene, entity, body->collider_id, mat3_translation(entity->position));
	} break;
	}
}

Collider *collider_get(Collider_Group *group, u32 index) {
	assert(index < group->count);
	return group->collider + index;
}

void iscene_allocate_colliders(Scene *scene, Entity *entity, Raw_Collider_Id id, const Mat3 &transform, Collider_Group *group) {
	auto raw = scene_find_raw_collider_group(scene, id);
	assert(raw); // TODO: Properly handle error!!

	u32 count = raw->colliders_count;
	group->collider = new(scene->collider_shape_allocator) Collider[count];
	group->transform = transform;
	group->entity_id = entity->id;
	group->count = count;
	group->flags = 0;

	Collider *src = raw->colliders;
	Collider *dst = group->collider;
	for (u32 index = 0; index < count; ++index, ++src, ++dst) {
		dst->type = src->type;

		switch (dst->type) {
		case Collider_Null: {
			dst->handle = nullptr;
		} break;

		case Collider_Circle: {
			dst->handle = memory_allocate(sizeof(Circle), scene->collider_shape_allocator);
			memcpy(dst->handle, src->handle, sizeof(Circle));
		} break;

		case Collider_Mm_Rect: {
			dst->handle = memory_allocate(sizeof(Mm_Rect), scene->collider_shape_allocator);
			memcpy(dst->handle, src->handle, sizeof(Mm_Rect));
		} break;

		case Collider_Capsule: {
			dst->handle = memory_allocate(sizeof(Capsule), scene->collider_shape_allocator);
			memcpy(dst->handle, src->handle, sizeof(Capsule));
		} break;

		case Collider_Polygon: {
			auto poly = (Polygon *)src->handle;
			auto total_size = sizeof(Polygon) + sizeof(Vec2) * (poly->vertex_count - 3);
			dst->handle = memory_allocate(total_size, scene->collider_shape_allocator);
			memcpy(dst->handle, poly, total_size);
		} break;

			invalid_default_case();
		};

	}

}

void iscene_free_colliders(Scene *scene, Collider_Group *group) {
	Collider *collider = group->collider;
	auto ptr = collider;
	for (u32 index = 0; index < group->count; ++index, ++ptr) {
		if (ptr->type != Collider_Null) {
			memory_free(ptr->handle, scene->collider_shape_allocator);
		}
	}
	memory_free(collider, scene->collider_shape_allocator);
}

Collider_Group *scene_create_colliders(Scene *scene, Entity *entity, Raw_Collider_Id id, const Mat3 &transform) {
	Collider_Node *node = circular_linked_list_add(&scene->colliders);
	iscene_allocate_colliders(scene, entity, id, transform, &node->data);
	return &node->data;
}

void scene_destroy_colliders(Scene *scene, Collider_Group *group) {
	iscene_free_colliders(scene, group);
	Collider_Node *node = (Collider_Node *)((char *)group - offsetof(Collider_Node, Collider_Node::data));
	circular_linked_list_remove(&scene->colliders, node);
}

Rigid_Body *scene_create_rigid_body(Scene *scene, Entity *entity, Raw_Collider_Id id) {
	auto node = circular_linked_list_add(&scene->rigid_bodies);
	Rigid_Body *rigid_body = &node->data;
	rigid_body->velocity = vec2(0);
	rigid_body->force = vec2(0);
	rigid_body->colliders = scene_create_colliders(scene, entity, id, mat3_identity());
	return rigid_body;
}

void scene_destroy_rigid_body(Scene *scene, Rigid_Body *rigid_body) {
	scene_destroy_colliders(scene, rigid_body->colliders);
	auto node = (Rigid_Body_List::Node *)((char *)rigid_body - offsetof(Rigid_Body_List::Node, Rigid_Body_List::Node::data));
	circular_linked_list_remove(&scene->rigid_bodies, node);
}

//
//
//

#if 0

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

bool deserialize_collider_group(Scene *scene, Entity *entity, Collider_Group *group, Deserialize_State *state) {
	u32 count;
	if (!deserialize_fmt_text(state, "collider_group_count", reflect_info(count), (char *)&count))
		return false;

	*group = scene_create_colliders(scene, entity, count);
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
		player->rigid_body = scene_create_rigid_body(scene, entity, 1);
		scene_attach_collider(scene, collider_node(player->rigid_body->colliders.handle, 0), Circle, 0);
	} break;

	case Entity_Static_Body: {
		if (!deserialize_fmt_text(state, "static_body", reflect_info<Static_Body>(), (char *)entity))
			return false;
		Collider_Group *group = &((Static_Body *)entity)->colliders;
		result = deserialize_collider_group(scene, entity, group, state);
	} break;

		invalid_default_case();
	}

	return result;
}

#endif
