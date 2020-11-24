#include "scene.h"
#include <time.h>

#include "modules/core/systems.h"
#include "modules/gfx/renderer.h"
#include "modules/core/stb_image.h"

#include ".generated/entity.typeinfo"

//
// Collision
//

template <typename ShapeA, typename ShapeB>
static bool shapes_collision_resolver(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold) {
	return gjk_epa(*(ShapeA *)a.handle, *(ShapeB *)b.handle, manifold, ta, tb);
}

template <typename ShapeA, typename ShapeB>
static Impact_Type shapes_continuous_collision_resolver(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Impact_Time *impact) {
	return gjk_continuous(*(ShapeA *)a.handle, *(ShapeB *)b.handle, ta, tb, a_dp, b_dp, impact);
}

template <typename ShapeA, typename ShapeB>
static bool shapes_nearest_points_finder(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Nearest_Points *nearest_points) {
	return gjk_nearest_points(*(ShapeA *)a.handle, *(ShapeB *)b.handle, nearest_points, ta, tb, a_dp, b_dp);
}

template <typename ShapeA, typename ShapeB>
static bool shapes_collision_detector(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb) {
	return gjk(*(ShapeA *)a.handle, *(ShapeB *)b.handle, ta, tb);
}

static Continuous_Collision_Resolver	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Count][Fixture_Shape_Count];
static Nearest_Points_Finder			NEAREST_POINTS_FINDERS[Fixture_Shape_Count][Fixture_Shape_Count];
static Collision_Resolver				COLLISION_RESOLVERS[Fixture_Shape_Count][Fixture_Shape_Count];
static Collision_Detector				COLLISION_DETECTORS[Fixture_Shape_Count][Fixture_Shape_Count];

static bool fixture_vs_fixture(Fixture *a, Fixture *b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold) {
	return COLLISION_RESOLVERS[a->shape][b->shape](*a, *b, ta, tb, manifold);
}

static Impact_Type fixture_vs_fixture_continuous(Fixture *a, Fixture *b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Impact_Time *impact) {
	return CONTINUOUS_COLLISION_RESOLVERS[a->shape][b->shape](*a, *b, ta, tb, a_dp, b_dp, impact);
}

static bool nearest_points_fixture_fixture(Fixture *a, Fixture *b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Nearest_Points *nearest_points) {
	return NEAREST_POINTS_FINDERS[a->shape][b->shape](*a, *b, ta, tb, a_dp, b_dp, nearest_points);
}

static bool test_fixture_fixture(Fixture *a, Fixture *b, const Transform &ta, const Transform &tb) {
	return COLLISION_DETECTORS[a->shape][b->shape](*a, *b, ta, tb);
}

static bool fixture_vs_point(Fixture *a, const Transform &ta, Vec2 point, r32 size, Contact_Manifold *manifold) {
	Circle circle = { point, size };
	Fixture b;
	b.shape = Fixture_Shape_Circle;
	b.handle = &circle;

	Transform tb;
	tb.p = vec2(0);
	tb.xform = mat2_identity();

	return COLLISION_RESOLVERS[a->shape][b.shape](*a, b, ta, tb, manifold);
}

static bool nearest_points_fixture_point(Fixture &a, const Transform &ta, Vec2 dp, Vec2 point, r32 size, Nearest_Points *nearest_points) {
	Circle circle = { point, size };
	Fixture b;
	b.shape = Fixture_Shape_Circle;
	b.handle = &circle;

	Transform tb;
	tb.p = vec2(0);
	tb.xform = mat2_identity();

	return NEAREST_POINTS_FINDERS[a.shape][b.shape](a, b, ta, tb, dp, vec2(0), nearest_points);
}

static bool test_fixture_point(Fixture &a, const Transform &ta, Vec2 point, r32 size = 0) {
	Circle circle = { point, size };
	Fixture b;
	b.shape = Fixture_Shape_Circle;
	b.handle = &circle;

	Transform tb;
	tb.p = vec2(0);
	tb.xform = mat2_identity();

	return COLLISION_DETECTORS[a.shape][b.shape](a, b, ta, tb);
}

//
// End Collision
//

void scene_prepare() {
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Circle] = shapes_collision_resolver<Circle, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Circle, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_collision_resolver<Circle, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_collision_resolver<Circle, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle] = shapes_collision_resolver<Mm_Rect, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Mm_Rect, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule] = shapes_collision_resolver<Mm_Rect, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon] = shapes_collision_resolver<Mm_Rect, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Circle] = shapes_collision_resolver<Capsule, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Capsule, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule] = shapes_collision_resolver<Capsule, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon] = shapes_collision_resolver<Capsule, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Circle] = shapes_collision_resolver<Polygon, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Polygon, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule] = shapes_collision_resolver<Polygon, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon] = shapes_collision_resolver<Polygon, Polygon>;

	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Circle, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Circle, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Circle, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Circle, Polygon>;

	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Mm_Rect, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Mm_Rect, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Mm_Rect, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Mm_Rect, Polygon>;

	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Capsule, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Capsule, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Capsule, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Capsule, Polygon>;

	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Polygon, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Polygon, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Polygon, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Polygon, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Circle] = shapes_nearest_points_finder<Circle, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect] = shapes_nearest_points_finder<Circle, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_nearest_points_finder<Circle, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_nearest_points_finder<Circle, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle] = shapes_nearest_points_finder<Mm_Rect, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect] = shapes_nearest_points_finder<Mm_Rect, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule] = shapes_nearest_points_finder<Mm_Rect, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon] = shapes_nearest_points_finder<Mm_Rect, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Circle] = shapes_nearest_points_finder<Capsule, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect] = shapes_nearest_points_finder<Capsule, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule] = shapes_nearest_points_finder<Capsule, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon] = shapes_nearest_points_finder<Capsule, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Circle] = shapes_nearest_points_finder<Polygon, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect] = shapes_nearest_points_finder<Polygon, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule] = shapes_nearest_points_finder<Polygon, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon] = shapes_nearest_points_finder<Polygon, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Circle] = shapes_collision_detector<Circle, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect] = shapes_collision_detector<Circle, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_collision_detector<Circle, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_collision_detector<Circle, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle] = shapes_collision_detector<Mm_Rect, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect] = shapes_collision_detector<Mm_Rect, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule] = shapes_collision_detector<Mm_Rect, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon] = shapes_collision_detector<Mm_Rect, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Circle] = shapes_collision_detector<Capsule, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect] = shapes_collision_detector<Capsule, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Capsule] = shapes_collision_detector<Capsule, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Polygon] = shapes_collision_detector<Capsule, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Circle] = shapes_collision_detector<Polygon, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect] = shapes_collision_detector<Polygon, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Capsule] = shapes_collision_detector<Polygon, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Polygon] = shapes_collision_detector<Polygon, Polygon>;

	stbi_set_flip_vertically_on_load(1);
}

//
//
//

static u64 iscene_generate_unique_id(Scene *scene) {
	u32 a = random_get(&scene->id_series);
	u32 b = (time(0) & 0xffffffff);
	u64 id = ((u64)a | ((u64)b << 32));
	return id;
}

inline u32 entity_hash_index(Entity_Id id) {
	// same as this, but only works when SCENE_MAX_ENTITY_COUNT is the power of 2
	// return id.handle % SCENE_MAX_ENTITY_COUNT;
	return (u32)(id.handle & (SCENE_MAX_ENTITY_COUNT - 1));
}

inline u32 entity_hash_index_probe(u32 hash_index) {
	return (hash_index + 1) & (SCENE_MAX_ENTITY_COUNT - 1);
}

inline u32 iscene_search_if_entity_reference_is_present(Entity_Hash_Table *table, Entity_Id id) {
	u32 hash_index = entity_hash_index(id);

	Entity_Hash_Table::Key *keys = table->keys;

	u32 index = hash_index;
	Entity_Hash_Table::Key *key = keys + index;

	while (key->slot != Entity_Hash_Table::Slot::EMPTY && entity_hash_index(key->id) <= hash_index) {
		if (key->id.handle == id.handle)
			return index;
		index = entity_hash_index_probe(index);
		key = keys + index;
	}

	return SCENE_MAX_ENTITY_COUNT;
}

inline u32 iscene_search_entity_reference(Entity_Hash_Table *table, Entity_Id id) {
	u32 index = iscene_search_if_entity_reference_is_present(table, id);
	assert(index != SCENE_MAX_ENTITY_COUNT);
	return index;
}

inline Entity_Reference *iscene_get_entity_reference(Entity_Hash_Table *table, Entity_Id id) {
	u32 index = iscene_search_entity_reference(table, id);
	Entity_Reference *slot = table->slots + index;
	return slot;
}

inline void iscene_insert_entity_reference(Entity_Hash_Table *table, Entity_Id id, Entity_Reference ref) {
	assert(table->count < SCENE_MAX_ENTITY_COUNT);

	u32 hash_index = entity_hash_index(id);

	Entity_Hash_Table::Key *keys = table->keys;

	u32 index = hash_index;
	Entity_Hash_Table::Key *key = keys + index;
	u32 next_hash_index = entity_hash_index(key->id);

	while (true) {
		while (key->slot == Entity_Hash_Table::Slot::OCCUPIED && next_hash_index <= hash_index) {
			if (key->id.handle == id.handle) {
				key->id = id;
				key->slot = Entity_Hash_Table::Slot::OCCUPIED;
				table->slots[index] = ref;
				table->count += 1;
				return;
			}
			index = entity_hash_index_probe(index);
			key = keys + index;
			next_hash_index = entity_hash_index(key->id);
		}

		if (key->slot != Entity_Hash_Table::Slot::EMPTY) {
			Entity_Reference temp_ref;
			temp_ref = table->slots[index];
			table->slots[index] = ref;
			ref = temp_ref;

			Entity_Id temp_id;
			temp_id = key->id;
			key->id = id;
			id = temp_id;

			hash_index = next_hash_index;

			index = entity_hash_index_probe(index);
			key = keys + index;
			next_hash_index = entity_hash_index(key->id);
		} else {
			key->id = id;
			key->slot = Entity_Hash_Table::Slot::OCCUPIED;
			table->slots[index] = ref;
			table->count += 1;
			return;
		}
	}
}

inline void iscene_delete_entity_reference(Entity_Hash_Table *table, Entity_Id id) {
	u32 search_index = iscene_search_if_entity_reference_is_present(table, id);
	assert(search_index < SCENE_MAX_ENTITY_COUNT);

	Entity_Hash_Table::Key *key = table->keys + search_index;
	Entity_Reference *slot = table->slots + search_index;

	Entity_Hash_Table::Key *nxt_key;
	u32 nxt_hash_index;
	Entity_Reference *nxt_slot;

	u32 put_index = search_index;

	for (u32 index = entity_hash_index_probe(search_index); index != search_index; index = entity_hash_index_probe(index)) {
		nxt_key = table->keys + index;

		if (nxt_key->slot == Entity_Hash_Table::Slot::EMPTY) {
			break;
		}

		nxt_hash_index = entity_hash_index(nxt_key->id);

		if ((index > nxt_hash_index && put_index >= nxt_hash_index && put_index < index) ||
			(index < nxt_hash_index && (put_index >= nxt_hash_index || put_index < index))) {
			nxt_slot = table->slots + index;

			*key = *nxt_key;
			*slot = *nxt_slot;

			key = nxt_key;
			slot = nxt_slot;
			put_index = index;
		}
	}

	key->slot = Entity_Hash_Table::Slot::EMPTY;
}

inline bool iscene_delete_entity_reference_if_present(Entity_Hash_Table *table, Entity_Id id, Entity_Reference *data) {
	u32 search_index = iscene_search_if_entity_reference_is_present(table, id);

	if (search_index < SCENE_MAX_ENTITY_COUNT) {
		Entity_Hash_Table::Key *key = table->keys + search_index;
		Entity_Reference *slot = table->slots + search_index;

		*data = *slot;

		Entity_Hash_Table::Key *nxt_key;
		u32 nxt_hash_index;
		Entity_Reference *nxt_slot;

		u32 put_index = search_index;

		for (u32 index = entity_hash_index_probe(search_index); index != search_index; index = entity_hash_index_probe(index)) {
			nxt_key = table->keys + index;

			if (nxt_key->slot == Entity_Hash_Table::Slot::EMPTY) {
				break;
			}

			nxt_hash_index = entity_hash_index(nxt_key->id);

			if ((index > nxt_hash_index && put_index >= nxt_hash_index && put_index < index) ||
				(index < nxt_hash_index && (put_index >= nxt_hash_index || put_index < index))) {
				nxt_slot = table->slots + index;

				*key = *nxt_key;
				*slot = *nxt_slot;

				key = nxt_key;
				slot = nxt_slot;
				put_index = index;
			}
		}

		key->slot = Entity_Hash_Table::Slot::EMPTY;

		return true;
	}

	return false;
}

inline Camera *iscene_add_camera(Scene *scene, Entity_Id id, Vec2 p) {
	u32 index = (u32)scene->by_type.camera.count;
	Camera *camera = array_add(&scene->by_type.camera);
	camera->type = Entity_Type_Camera;
	camera->id = id;
	camera->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Camera, index });
	return camera;
}

inline Character *iscene_add_character(Scene *scene, Entity_Id id, Vec2 p) {
	u32 index = (u32)scene->by_type.character.count;
	Character *character = array_add(&scene->by_type.character);
	character->type = Entity_Type_Character;
	character->id = id;
	character->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Character, index });
	return character;
}

inline Obstacle *iscene_add_obstacle(Scene *scene, Entity_Id id, Vec2 p) {
	u32 index = (u32)scene->by_type.obstacle.count;
	Obstacle *obstacle = array_add(&scene->by_type.obstacle);
	obstacle->type = Entity_Type_Obstacle;
	obstacle->id = id;
	obstacle->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Obstacle, index });
	return obstacle;
}

inline Rigid_Body *iscene_create_new_rigid_body(Scene *scene, Entity *entity, const Rigid_Body *src) {
	auto node = circular_linked_list_add(&scene->rigid_bodies);
	Rigid_Body *body = &node->data;
	if (src) {
		memcpy(body, src, sizeof(Rigid_Body));
	}
	body->transform.p = entity->position;
	body->entity_id = entity->id;
	return body;
}

inline void iscene_destroy_rigid_body(Scene *scene, Rigid_Body *rigid_body) {
	auto node = circular_linked_list_node_from_data(rigid_body);
	circular_linked_list_remove(&scene->rigid_bodies, node);
}

//
//
//

Scene *scene_create() {
	Scene *scene = (Scene *)memory_allocate(sizeof(Scene));

	for (u32 index = 0; index < Entity_Type_Count; ++index) {
		auto &data = scene->by_type.data[index];
		data.count = 0;
		data.capacity = 0;
		data.data = nullptr;
		data.allocator = context.allocator;
	}

	array_resize(&scene->by_type.camera, 8);
	array_resize(&scene->by_type.character, 100);
	array_resize(&scene->by_type.obstacle, 1000);

	scene->entity_table.keys = (Entity_Hash_Table::Key *)memory_allocate(sizeof(Entity_Hash_Table::Key) * SCENE_MAX_ENTITY_COUNT);
	scene->entity_table.slots = (Entity_Reference *)memory_allocate(sizeof(Entity_Reference) * SCENE_MAX_ENTITY_COUNT);
	scene->entity_table.count = 0;

	memset(scene->entity_table.keys, 0, sizeof(Entity_Hash_Table::Key) * SCENE_MAX_ENTITY_COUNT);

	scene->resource_textures.count = 0;
	scene->resource_textures.capacity = 0;
	scene->resource_textures.data = nullptr;
	scene->resource_textures.allocator = context.allocator;

	scene->handle_textures.count = 0;
	scene->handle_textures.capacity = 0;
	scene->handle_textures.data = nullptr;
	scene->handle_textures.allocator = context.allocator;

	array_resize(&scene->resource_textures, 100);
	array_resize(&scene->handle_textures, 100);

	circular_linked_list_init(&scene->rigid_bodies, context.allocator);

	scene->resource_fixtures.count = scene->resource_fixtures.capacity = 0;
	scene->resource_fixtures.data = nullptr;
	scene->resource_fixtures.allocator = context.allocator;
	array_resize(&scene->resource_fixtures, 500);

	scene->pool_allocator = context.allocator;

	scene->loaded_level = -1;

	scene->levels.count = scene->levels.capacity = 0;
	scene->levels.data = nullptr;
	scene->levels.allocator = context.allocator;
	array_resize(&scene->levels, 5);

	scene->id_series = random_init(context.id, system_get_counter());

	for (s64 index = 0; index < Entity_Type_Count; ++index) {
		auto &rm = scene->removed_entity[index];
		rm.capacity = 0;
		rm.count = 0;
		rm.data = nullptr;
		rm.allocator = TEMPORARY_ALLOCATOR;
	}

	#ifdef ENABLE_DEVELOPER_OPTIONS
	scene->manifolds.count = scene->manifolds.capacity = 0;
	scene->manifolds.data = nullptr;
	scene->manifolds.allocator = TEMPORARY_ALLOCATOR;

	scene->editor = editor_create(scene);
	#endif

	return scene;
}

void scene_destroy(Scene *scene) {
	// Entites are cleaned when level is unloaded
	scene_unload_current_level(scene);

	scene_clean_resources(scene);

	array_free(&scene->levels);

	array_free(&scene->resource_fixtures);

	circular_linked_list_clear(&scene->rigid_bodies);

	memory_free(scene->entity_table.keys);
	memory_free(scene->entity_table.slots);

	for (u32 index = 0; index < Entity_Type_Count - 1; ++index) {
		auto &data = scene->by_type.data[index];
		array_free(&data);
	}

	memory_free(scene);
}

//
//
//

Resource_Fixture *scene_create_new_resource_fixture(Scene *scene, String name, Fixture *fixtures, u32 fixture_count) {
	Resource_Fixture *resource = array_add(&scene->resource_fixtures);

	assert(name.count < (sizeof(Resource_Name) - 1));

	resource->id.handle = iscene_generate_unique_id(scene);

	memcpy(resource->name, name.data, name.count);
	resource->name[name.count] = 0;

	resource->fixture_count = fixture_count;
	if (fixture_count) {
		resource->fixtures = new (scene->pool_allocator) Fixture[resource->fixture_count];
	} else {
		resource->fixtures = nullptr;
	}

	Fixture *dst = resource->fixtures;
	Fixture *src;
	ptrsize size = 0;
	for (u32 index = 0; index < fixture_count; ++index) {
		src = fixtures + index;

		switch (src->shape) {
			case Fixture_Shape_Circle:	size = sizeof(Circle); break;
			case Fixture_Shape_Mm_Rect: size = sizeof(Mm_Rect); break;
			case Fixture_Shape_Capsule: size = sizeof(Capsule); break;
			case Fixture_Shape_Polygon: size = sizeof(Polygon) + sizeof(Vec2) * (((Polygon *)src->handle)->vertex_count - 3); break;

				invalid_default_case();
		}

		dst->shape = src->shape;
		dst->handle = memory_allocate(size, scene->pool_allocator);
		memcpy(dst->handle, src->handle, size);

		dst += 1;
	}

	return resource;
}

Resource_Fixture *scene_find_resource_fixture(Scene *scene, Resource_Id id) {
	auto res = array_find(&scene->resource_fixtures, [](const Resource_Fixture &f, Resource_Id id) { return f.id.handle == id.handle; }, id);
	if (res >= 0) return scene->resource_fixtures.data + res;
	return nullptr;
}

Resource_Fixture *scene_find_resource_fixture_from_fixture(Scene *scene, Fixture *fixture) {
	s64 index = array_find(&scene->resource_fixtures, [](const Resource_Fixture &f, Fixture *ptr) { return f.fixtures == ptr; }, fixture);
	assert(index >= 0);
	return &scene->resource_fixtures[index];
}

Resource_Texture *scene_find_resource_texture_from_index(Scene *scene, Texture_Id id) {
	return &scene->resource_textures[id.index];
}

bool scene_find_resource_texture(Scene *scene, const String name, Texture_Id *id) {
	u32 key;
	if (name.count) {
		key = murmur3_32(name.data, name.count, 5656);
	} else {
		key = 0;
	}

	s64 index = array_find(&scene->resource_textures, [](const Resource_Texture &t, const String name, u32 key) { 
		return t.key == key && string_match(name, String(t.name, t.name_count));
	}, name, key);
	if (index >= 0) {
		id->index = (u32)index;
		return true;
	} else {
		id->index = 0;
		return false;
	}
}

Entity *scene_clone_entity(Scene *scene, Entity *src, Vec2 p) {
	Entity_Id id = { iscene_generate_unique_id(scene) };

	Entity *result = nullptr;

	switch (src->type) {
		case Entity_Type_Camera: {
			auto camera = iscene_add_camera(scene, id, p);
			memcpy((u8 *)camera + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Camera) - sizeof(Entity));
			result = camera;
		} break;

		case Entity_Type_Character: {
			auto character = iscene_add_character(scene, id, p);
			memcpy((u8 *)character + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Character) - sizeof(Entity));

			character->rigid_body = iscene_create_new_rigid_body(scene, character, ((Character *)src)->rigid_body);

			Resource_Entity resource;
			resource.id = id;
			resource.fixture_id = scene_find_resource_fixture_from_fixture(scene, character->rigid_body->fixtures)->id;

			Resource_Texture *tex = scene_find_resource_texture_from_index(scene, character->texture);
			memcpy(resource.texture_name, tex->name, sizeof(Resource_Name));

			Level *level = scene_current_level_pointer(scene);
			array_add(&level->resources, resource);

			result = character;
		} break;

		case Entity_Type_Obstacle: {
			auto obstacle = iscene_add_obstacle(scene, id, p);
			memcpy((u8 *)obstacle + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Obstacle) - sizeof(Entity));

			obstacle->rigid_body = iscene_create_new_rigid_body(scene, obstacle, ((Obstacle *)src)->rigid_body);

			Resource_Entity resource;
			resource.id = id;
			resource.fixture_id = scene_find_resource_fixture_from_fixture(scene, obstacle->rigid_body->fixtures)->id;

			Resource_Texture *tex = scene_find_resource_texture_from_index(scene, obstacle->texture);
			memcpy(resource.texture_name, tex->name, sizeof(Resource_Name));

			Level *level = scene_current_level_pointer(scene);
			array_add(&level->resources, resource);

			result = obstacle;
		} break;

			invalid_code_path();
	}

	return result;
}

Entity_Reference scene_get_entity(Scene *scene, Entity_Id id) {
	return *iscene_get_entity_reference(&scene->entity_table, id);
}

bool scene_find_entity(Scene *scene, Entity_Id id, Entity_Reference *ref) {
	u32 index = iscene_search_if_entity_reference_is_present(&scene->entity_table, id);
	if (index != SCENE_MAX_ENTITY_COUNT) {
		*ref = scene->entity_table.slots[index];
		return true;
	}
	return false;
}

Entity *scene_entity_pointer(Scene *scene, Entity_Reference &ref) {
	static constexpr ptrsize ENTITY_TYPE_SIZE[] = {
		sizeof(Camera),
		sizeof(Character),
		sizeof(Obstacle)
	};
	static_assert(static_count(ENTITY_TYPE_SIZE) == Entity_Type_Count);

	return (Entity *)(scene->by_type.data[ref.type].data + ref.index * ENTITY_TYPE_SIZE[ref.type]);
}

const Array_View<Camera> scene_cameras(Scene *scene) {
	return scene->by_type.camera;
}

const Array_View<Resource_Entity> scene_resources(Scene *scene) {
	return scene_current_level_pointer(scene)->resources;
}

//
//
//

Fixture *scene_rigid_body_get_fixture(Rigid_Body *rigid_body, u32 index) {
	assert(index < rigid_body->fixture_count);
	return rigid_body->fixtures + index;
}

Mm_Rect scene_rigid_body_bounding_box(Rigid_Body *body, r32 dt) {
	Mm_Rect most_min_rect;
	most_min_rect.min = vec2(MAX_FLOAT);
	most_min_rect.max = vec2(-MAX_FLOAT);

	const Transform &t = body->transform;

	for (u32 index = 0; index < body->fixture_count; ++index) {
		Fixture *fixture = scene_rigid_body_get_fixture(body, index);
		switch (fixture->shape) {
			case Fixture_Shape_Circle: {
				auto circle = fixture_get_shape(fixture, Circle);
				auto rect = mm_rect_enclosing_circle(*circle, t);
				most_min_rect.min = vec2_min(most_min_rect.min, rect.min);
				most_min_rect.max = vec2_max(most_min_rect.max, rect.max);
			} break;

			case Fixture_Shape_Mm_Rect: {
				auto mm_rect = fixture_get_shape(fixture, Mm_Rect);
				auto rect = mm_rect_enclosing_mm_rect(*mm_rect, t);
				most_min_rect.min = vec2_min(most_min_rect.min, rect.min);
				most_min_rect.max = vec2_max(most_min_rect.max, rect.max);
			} break;

			case Fixture_Shape_Capsule: {
				auto capsule = fixture_get_shape(fixture, Capsule);
				auto rect = mm_rect_enclosing_capsule(*capsule, t);
				most_min_rect.min = vec2_min(most_min_rect.min, rect.min);
				most_min_rect.max = vec2_max(most_min_rect.max, rect.max);
			} break;

			case Fixture_Shape_Polygon: {
				auto polygon = fixture_get_shape(fixture, Polygon);
				auto rect = mm_rect_enclosing_polygon(*polygon, t);
				most_min_rect.min = vec2_min(most_min_rect.min, rect.min);
				most_min_rect.max = vec2_max(most_min_rect.max, rect.max);
			} break;

				invalid_default_case();
		}
	}

	Vec2 dp = dt * body->velocity;

	most_min_rect.min = vec2_min(most_min_rect.min, most_min_rect.min + dp);
	most_min_rect.max = vec2_max(most_min_rect.max, most_min_rect.max + dp);

	return most_min_rect;
}

void scene_rigid_body_update_bounding_box(Rigid_Body *body, r32 dt) {
	body->bounding_box = scene_rigid_body_bounding_box(body, dt);
}

void scene_refresh_rigid_bodies(Scene *scene) {
	for_list(Rigid_Body, ptr, &scene->rigid_bodies) {
		auto &body = ptr->data;
		body.flags = 0;
		body.velocity = vec2(0);
		body.force = vec2(0);
		body.bounding_box = scene_rigid_body_bounding_box(&body, 0);
	}
}

Rigid_Body *scene_collide_point(Scene *scene, Vec2 point, r32 size) {
	Rigid_Body *collided = nullptr;
	for_list(Rigid_Body, ptr, &scene->rigid_bodies) {
		auto &body = ptr->data;
		for (u32 index = 0; index < body.fixture_count && !collided; ++index) {
			Fixture *fixture = scene_rigid_body_get_fixture(&body, index);
			if (test_fixture_point(*fixture, body.transform, point, size)) {
				collided = &body;
			}
		}
		if (collided) break;
	}
	return collided;
}

//
//
//

Camera *scene_primary_camera(Scene *scene) {
	#ifdef ENABLE_DEVELOPER_OPTIONS
	static Camera null_camera;
	if (scene->by_type.camera.count) {
		return &scene->by_type.camera[0];
	}
	return &null_camera;
	#else
	return &scene->by_type.camera[0];
	#endif
}

//
//
//

void scene_remove_entity(Scene *scene, Entity_Reference &ref, Entity_Id id) {
	array_add(&scene->removed_entity[ref.type], id);
}

void scene_remove_entity(Scene *scene, Entity_Id id) {
	Entity_Reference ref = scene_get_entity(scene, id);
	scene_remove_entity(scene, ref, id);
}

void scene_remove_entity_from_level(Scene *scene, Entity_Reference &ref, Entity_Id id) {
	scene_remove_entity(scene, ref, id);
	Level *level = scene_current_level_pointer(scene);
	s64 index = array_find(&level->resources, [](const Resource_Entity &r, Entity_Id id) { return r.id.handle == id.handle; }, id);
	if (index >= 0) {
		array_remove(&level->resources, index);
	}
	system_remove_file(tprintf("resources/levels/%s/%zu.ent", level->name, id.handle));
}

void scene_remove_entity_from_level(Scene *scene, Entity_Id id) {
	Entity_Reference ref = scene_get_entity(scene, id);
	scene_remove_entity_from_level(scene, ref, id);
}


//
//
//

bool scene_handle_event(Scene *scene, const Event &event) {
	#ifdef ENABLE_DEVELOPER_OPTIONS
	if (editor_handle_event(event, scene, &scene->editor))
		return true;
	#endif

	return false;
}

void scene_begin(Scene *scene) {
	for (s64 index = 0; index < Entity_Type_Count; ++index) {
		auto &rm = scene->removed_entity[index];
		rm.capacity = 0;
		rm.count = 0;
		rm.data = nullptr;
	}

	#ifdef ENABLE_DEVELOPER_OPTIONS
	scene->manifolds.capacity = 0;
	scene->manifolds.count = 0;
	scene->manifolds.data = nullptr;
	#endif
}

bool iscene_simulate_world_enabled(Scene *scene) {
	return scene->editor.mode == Editor_Mode_GAME || scene->editor.mode == Editor_Mode_GAME_DEVELOPER;
}

void scene_simulate(Scene *scene, r32 dt) {
	if (iscene_simulate_world_enabled(scene)) {
		// TODO: DEBUG CODE!!
		if (scene->by_type.character.count) {
			auto primary_player = &scene->by_type.character[0];

			auto camera = scene_primary_camera(scene);
			camera->behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT;
			camera->target_position = primary_player->position;
		}

		Contact_Manifold manifold;
		manifold.penetration = 0;

		auto count = scene->by_type.character.count;
		auto &characters = scene->by_type.character;
		for (auto index = 0; index < count; ++index) {
			auto &character = characters[index];
			character.color = vec4(1);
			character.rigid_body->transform.p = character.position;
		}

		for_list(Rigid_Body, ptr, &scene->rigid_bodies) {
			if (ptr->data.type == Rigid_Body_Type_Dynamic) {
				ptr->data.velocity += dt * ptr->data.force;
				ptr->data.velocity *= powf(0.5f, ptr->data.drag * dt);
				ptr->data.transform.p += dt * ptr->data.velocity;
				ptr->data.bounding_box = scene_rigid_body_bounding_box(&ptr->data, 0);
			}
			clear_bit(ptr->data.flags, Rigid_Body_COLLIDING);
			clear_bit(ptr->data.flags, Rigid_Body_BOUNDING_BOX_COLLIDING);
		}

		for (int iteration = 0; iteration < SCENE_SIMULATION_MAX_ITERATION; ++iteration) {
			for_list(Rigid_Body, a_ptr, &scene->rigid_bodies) {
				for_list_offset(Rigid_Body, b_ptr, a_ptr, &scene->rigid_bodies) {
					auto &a = a_ptr->data;
					auto &b = b_ptr->data;

					if (a.type == Rigid_Body_Type_Static && b.type == Rigid_Body_Type_Static) {
						continue;
					}

					if (test_mmrect_vs_mmrect(a.bounding_box, b.bounding_box)) {
						set_bit(a.flags, Rigid_Body_BOUNDING_BOX_COLLIDING);
						set_bit(b.flags, Rigid_Body_BOUNDING_BOX_COLLIDING);

						for (u32 a_index = 0; a_index < a.fixture_count; ++a_index) {
							for (u32 b_index = 0; b_index < b.fixture_count; ++b_index) {
								Fixture *a_fixture = scene_rigid_body_get_fixture(&a, a_index);
								Fixture *b_fixture = scene_rigid_body_get_fixture(&b, b_index);

								if (fixture_vs_fixture(a_fixture, b_fixture, a.transform, b.transform, &manifold)) {
									set_bit(a.flags, Rigid_Body_COLLIDING);
									set_bit(b.flags, Rigid_Body_COLLIDING);

									// Resolve velocity
									r32 restitution = minimum(a.restitution, b.restitution);
									r32 j = -(1 + restitution) * vec2_dot(b.velocity - a.velocity, manifold.normal);
									j /= (a.imass + b.imass);
									j = maximum(0, j);

									r32 alpha = SCENE_SIMULATION_CORRECTION_ALPHA;

									a.velocity -= alpha * j * a.imass * manifold.normal;
									b.velocity += alpha * j * b.imass * manifold.normal;

									// Fix position
									Vec2 correction = maximum(manifold.penetration, 0.0f) / (a.imass + b.imass) * manifold.normal;
									a.transform.p -= a.imass * correction;
									b.transform.p += b.imass * correction;
									a.bounding_box = scene_rigid_body_bounding_box(&a, 0);
									b.bounding_box = scene_rigid_body_bounding_box(&b, 0);

									#ifdef ENABLE_DEVELOPER_OPTIONS
									array_add(&scene->manifolds, manifold);
									#endif
								}
							}
						}
					}

				}
			}
		}

		// Update positions
		for (auto index = 0; index < count; ++index) {
			auto &character = characters[index];
			character.position = character.rigid_body->transform.p;
		}

		// Simulate camera
		count = scene->by_type.camera.count;
		auto &cameras = scene->by_type.camera;
		for (s64 index = 0; index < count; ++index) {
			Camera &camera = cameras[index];
			if (camera.behaviour & Camera_Behaviour_ANIMATE_MOVEMENT) {
				camera.position = lerp(camera.position, camera.target_position, 1.0f - powf(1.0f - camera.follow_factor, dt));
			}
			if (camera.behaviour & Camera_Behaviour_ANIMATE_FOCUS) {
				camera.distance = lerp(camera.distance, camera.target_distance, 1.0f - powf(1.0f - camera.zoom_factor, dt));
			}
		}
	}
}

void scene_update(Scene *scene) {
	auto count = scene->by_type.character.count;
	auto &characters = scene->by_type.character;
	for (auto index = 0; index < count; ++index) {
		auto &character = characters[index];
		character.rigid_body->force = vec2(0);
	}

	editor_update(scene, &scene->editor);
}

void scene_end(Scene *scene) {
	Array<Entity_Id> *rma;
	s64 count;

	Entity_Reference ref;
	Entity_Id id;

	{
		rma = scene->removed_entity + Entity_Type_Camera;
		count = rma->count;
		auto &cameras = scene->by_type.camera;
		for (s64 index = 0; index < count; ++index) {
			id = rma->data[index];
			if (iscene_delete_entity_reference_if_present(&scene->entity_table, id, &ref)) {
				cameras[ref.index] = cameras[cameras.count - 1];
				cameras.count -= 1;
				if (ref.index < cameras.count) {
					auto data = iscene_get_entity_reference(&scene->entity_table, cameras[ref.index].id);
					data->index = ref.index;
				}
			}
		}
	}

	{
		rma = scene->removed_entity + Entity_Type_Character;
		count = rma->count;
		auto &characters = scene->by_type.character;
		for (s64 index = 0; index < count; ++index) {
			id = rma->data[index];
			if (iscene_delete_entity_reference_if_present(&scene->entity_table, id, &ref)) {
				iscene_destroy_rigid_body(scene, characters[ref.index].rigid_body);
				characters[ref.index] = characters[characters.count - 1];
				characters.count -= 1;
				if (ref.index < characters.count) {
					auto data = iscene_get_entity_reference(&scene->entity_table, characters[ref.index].id);
					data->index = ref.index;
				}
			}
		}
	}

	{
		rma = scene->removed_entity + Entity_Type_Obstacle;
		count = rma->count;
		auto &obstacles = scene->by_type.obstacle;
		for (s64 index = 0; index < count; ++index) {
			id = rma->data[index];
			if (iscene_delete_entity_reference_if_present(&scene->entity_table, id, &ref)) {
				iscene_destroy_rigid_body(scene, obstacles[ref.index].rigid_body);
				obstacles[ref.index] = obstacles[obstacles.count - 1];
				obstacles.count -= 1;
				if (ref.index < obstacles.count) {
					auto data = iscene_get_entity_reference(&scene->entity_table, obstacles[ref.index].id);
					data->index = ref.index;
				}
			}
		}
	}
}

//
//
//

void scene_render_shape(const Circle &circle, Vec4 shade, Vec4 outline) {
	im2d_circle(circle.center, circle.radius, shade);
	im2d_circle_outline(circle.center, circle.radius, outline);
}

void scene_render_shape(const Mm_Rect &mm_rect, Vec4 shade, Vec4 outline) {
	auto dim = mm_rect.max - mm_rect.min;
	im2d_rect(mm_rect.min, dim, shade);
	im2d_rect_outline(mm_rect.min, dim, outline);
}

void scene_render_shape(const Polygon &polygon, Vec4 shade, Vec4 outline) {
	im2d_polygon(polygon, shade);
	im2d_polygon_outline(polygon, outline);
}

void scene_render_shape(const Polygon_Pt &polygon, Vec4 shade, Vec4 outline) {
	im2d_polygon(polygon, shade);
	im2d_polygon_outline(polygon, outline);
}

void scene_render_shape(const Capsule &capsule, Vec4 shade, Vec4 outline) {
	Vec2 capsule_dir = capsule.b - capsule.a;
	Vec2 capsule_norm = vec2_normalize(vec2(-capsule_dir.y, capsule_dir.x)) * capsule.radius;

	Vec2 a, b, c, d;
	a = capsule.a - capsule_norm;
	b = capsule.a + capsule_norm;
	c = capsule.b + capsule_norm;
	d = capsule.b - capsule_norm;

	r32 theta_a = atan2f(capsule_norm.y, capsule_norm.x) + MATH_PI;
	r32 theta_b = theta_a + MATH_PI;
	while (theta_b >= 2 * MATH_PI) theta_b -= 2 * MATH_PI;

	im2d_pie(capsule.a, capsule.radius, theta_b, theta_a, shade);
	im2d_pie(capsule.b, capsule.radius, theta_a, theta_b, shade);
	im2d_quad(a, b, c, d, shade);

	im2d_arc_outline(capsule.a, capsule.radius, theta_b, theta_a, outline);
	im2d_arc_outline(capsule.b, capsule.radius, theta_a, theta_b, outline);
	im2d_line(a, d, outline);
	im2d_line(b, c, outline);
}

static void iscene_render_shape_transformed(Fixture &fixture, const Transform &transform, Vec3 color) {
	const r32 alpha = 0.1f;
	auto shade = vec4(color, alpha);
	auto outline = vec4(color, 1);

	Mat3 xform = mat2_to_mat3(transform.xform);
	xform.rows[0].z = transform.p.x;
	xform.rows[1].z = transform.p.y;

	im2d_push_matrix(mat3_to_mat4(xform));

	switch (fixture.shape) {
		case Fixture_Shape_Circle: {
			auto circle = fixture_get_shape(&fixture, Circle);
			scene_render_shape(*circle, shade, outline);
		} break;

		case Fixture_Shape_Mm_Rect: {
			auto mm_rect = fixture_get_shape(&fixture, Mm_Rect);
			scene_render_shape(*mm_rect, shade, outline);
		} break;

		case Fixture_Shape_Polygon: {
			auto polygon = fixture_get_shape(&fixture, Polygon);
			scene_render_shape(*polygon, shade, outline);
		} break;

		case Fixture_Shape_Capsule: {
			auto capsule = fixture_get_shape(&fixture, Capsule);
			scene_render_shape(*capsule, shade, outline);
		} break;

			invalid_default_case();
	}

	im2d_pop_matrix();
}

inline Camera *iscene_get_rendering_camera(Scene *scene) {
	return editor_rendering_camera(scene, &scene->editor);
}

inline bool iscene_render_world_enabled(Scene *scene) {
	auto mode = scene->editor.mode;
	auto flags = scene->editor.flags;

	return (flags & Editor_Flag_Bit_RENDER_WORLD) && (
		mode == Editor_Mode_GAME || mode == Editor_Mode_GAME_DEVELOPER || mode == Editor_Mode_LEVEL_EDITOR
		);
}

inline bool iscene_render_fixture_enabled(Scene *scene, u32 *type) {
	auto mode = scene->editor.mode;
	auto flags = scene->editor.flags;
	if (mode == Editor_Mode_GAME) return false;
	*type = (flags & (Editor_Flag_Bit_RENDER_FIXTURE | Editor_Flag_Bit_RENDER_BOUNDING_BOX));
	return ((flags & Editor_Flag_Bit_RENDER_FIXTURE) || (flags & Editor_Flag_Bit_RENDER_BOUNDING_BOX)) &&
		(mode == Editor_Mode_GAME_DEVELOPER || mode == Editor_Mode_LEVEL_EDITOR);
}

inline bool iscene_render_collision_enabled(Scene *scene) {
	auto mode = scene->editor.mode;
	auto flags = scene->editor.flags;

	if (mode == Editor_Mode_GAME) return false;
	return (flags & Editor_Flag_Bit_RENDER_COLLISION) && (mode == Editor_Mode_GAME_DEVELOPER || mode == Editor_Mode_LEVEL_EDITOR);
}

// TODO: Use the alpha
void scene_render(Scene *scene, r32 alpha, r32 aspect_ratio) {
	Camera *camera = iscene_get_rendering_camera(scene);

	r32 view_height = SCENE_VIEW_HEIGHT_HALF;
	r32 view_width = aspect_ratio * view_height;

	Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height);

	im2d_set_stroke_weight(0.008f);

	r32 scale = camera_distance_to_scale(camera);
	Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera->position, 0.0f));

	im2d_begin(view, transform);

	if (iscene_render_world_enabled(scene)) {
		auto &textures = scene->handle_textures;

		s64 count = scene->by_type.character.count;
		auto &characters = scene->by_type.character;
		for (s64 index = 0; index < count; ++index) {
			Character &c = characters[index];

			im2d_bind_texture(textures[c.texture.index]);

			auto v = c.rigid_body->velocity;
			r32 angle = atan2f(v.y, v.x) - MATH_TAU;

			im2d_rect_centered_rotated(c.position, vec2(c.radius), angle, c.color);
		}

		im2d_unbind_texture();
	}

	u32 type;
	if (iscene_render_fixture_enabled(scene, &type)) {
		Rigid_Body *body_hovered = scene->editor.level.hovered_body;
		Vec4 color;
		for_list(Rigid_Body, ptr, &scene->rigid_bodies) {
			auto &body = ptr->data;

			if (type & Editor_Flag_Bit_RENDER_FIXTURE) {
				color = (body.flags & Rigid_Body_COLLIDING) ? vec4(1, 0, 0) : vec4(0.7f, 0.45f, 0);

				if (&body == body_hovered) {
					color.xyz = vec3(1) - color.xyz;
				}

				for (u32 index = 0; index < body.fixture_count; ++index) {
					auto f = scene_rigid_body_get_fixture(&body, index);
					iscene_render_shape_transformed(*f, body.transform, color.xyz);
				}
			}

			if (type & Editor_Flag_Bit_RENDER_BOUNDING_BOX) {
				color = (body.flags & Rigid_Body_BOUNDING_BOX_COLLIDING) ? vec4(0.7f, 0.1f, 0.1f, 1) : vec4(0.1f, 0.7f, 0.1f, 1);

				im2d_rect_outline(body.bounding_box.min, body.bounding_box.max - body.bounding_box.min, color);
			}
		}
	}

	if (iscene_render_collision_enabled(scene)) {
		auto manifolds = scene->manifolds;
		for (auto &m : manifolds) {
			im2d_line(m.contacts[1], m.contacts[1] + m.penetration * m.normal, vec4(1, 0, 1), 0.02f);

			im2d_circle(m.contacts[0], 0.02f, vec4(1, 0, 1));
			im2d_circle(m.contacts[1], 0.02f, vec4(1, 0, 1));
		}
	}
	im2d_end();

	editor_render(scene, &scene->editor, aspect_ratio);
}

//
//
//

void iscene_serialize_fixture(Scene *scene, Fixture *fixture, Ostream *out, bool pt_polygon) {
	serialize_fmt_text(out, "shape", reflect_info<Fixture_Shape>(), (char *)&fixture->shape);
	serialize_fmt_text_next(out);

	switch (fixture->shape) {
		case Fixture_Shape_Circle: {
			serialize_fmt_text(out, "shape_data", reflect_info<Circle>(), (char *)fixture->handle);
		} break;

		case Fixture_Shape_Mm_Rect: {
			serialize_fmt_text(out, "shape_data", reflect_info<Mm_Rect>(), (char *)fixture->handle);
		} break;

		case Fixture_Shape_Capsule: {
			serialize_fmt_text(out, "shape_data", reflect_info<Capsule>(), (char *)fixture->handle);
		} break;

		case Fixture_Shape_Polygon: {
			Array_View<Vec2> points;
			if (pt_polygon) {
				Polygon_Pt *polygon = fixture_get_shape(fixture, Polygon_Pt);
				points.count = polygon->vertex_count;
				points.data = polygon->vertices;
			} else {
				Polygon *polygon = fixture_get_shape(fixture, Polygon);
				points.count = polygon->vertex_count;
				points.data = polygon->vertices;
			}
			serialize_fmt_text(out, "shape_data", reflect_info(points), (char *)&points);
		} break;

			invalid_default_case();
	}
}

void iscene_serialize_fixture_resource(Scene *scene, Resource_Fixture &resource, Ostream *out, bool pt_polygon) {
	serialize_fmt_text(out, "resource", reflect_info<Resource_Fixture>(), (char *)&resource);
	serialize_fmt_text_next(out);
	serialize_fmt_text(out, "fixture_count", reflect_info(resource.fixture_count), (char *)&resource.fixture_count);

	u32 count = resource.fixture_count;
	auto fixture = resource.fixtures;
	for (u32 index = 0; index < count; ++index, ++fixture) {
		serialize_fmt_text_next(out);
		iscene_serialize_fixture(scene, fixture, out, pt_polygon);
	}
}

bool iscene_deserialize_fixture(Fixture *fixture, Deserialize_State *state) {
	bool result = false;

	if (!deserialize_fmt_text(state, "shape", reflect_info<Fixture_Shape>(), (char *)&fixture->shape))
		return false;

	switch (fixture->shape) {
		case Fixture_Shape_Circle: {
			fixture->handle = memory_allocate(sizeof(Circle));
			result = deserialize_fmt_text(state, "shape_data", reflect_info<Circle>(), (char *)fixture->handle);
		} break;

		case Fixture_Shape_Mm_Rect: {
			fixture->handle = memory_allocate(sizeof(Mm_Rect));
			result = deserialize_fmt_text(state, "shape_data", reflect_info<Mm_Rect>(), (char *)fixture->handle);
		} break;

		case Fixture_Shape_Capsule: {
			fixture->handle = memory_allocate(sizeof(Capsule));
			result = deserialize_fmt_text(state, "shape_data", reflect_info<Capsule>(), (char *)fixture->handle);
		} break;

		case Fixture_Shape_Polygon: {
			Array_View<Vec2> points;

			scoped_temporary_allocation();
			auto mark = push_temporary_allocator();
			result = deserialize_fmt_text(state, "shape_data", reflect_info(points), (char *)&points);
			pop_temporary_allocator(mark);

			if (!result) break;

			fixture->handle = memory_allocate(sizeof(Polygon) + sizeof(Vec2) * (points.count - 3));
			auto polygon = (Polygon *)fixture->handle;
			polygon->vertex_count = (u32)points.count;
			memcpy(polygon->vertices, points.data, sizeof(Vec2) * points.count);
		} break;

			invalid_default_case();
	}

	return result;
}

bool iscene_deserialize_fixture_resource(Scene *scene, Resource_Fixture *resource, Deserialize_State *state) {
	auto mark = push_allocator(scene->pool_allocator);
	defer{ pop_allocator(mark); };

	if (!deserialize_fmt_text(state, "resource", reflect_info<Resource_Fixture>(), (char *)resource))
		return false;

	if (!deserialize_fmt_text(state, "fixture_count", reflect_info(resource->fixture_count), (char *)&resource->fixture_count))
		return false;

	u32 count = resource->fixture_count;
	resource->fixtures = (Fixture *)memory_allocate(count * sizeof(Fixture));

	auto fixture = resource->fixtures;
	for (u32 index = 0; index < count; ++index, ++fixture) {
		if (!iscene_deserialize_fixture(fixture, state))
			return false;
	}

	return true;
}

//
//
//

bool scene_save_resource(Scene *scene, Resource_Fixture &r, bool pt_polygon) {
	auto mark = push_temporary_allocator();
	defer{ pop_temporary_allocator(mark); };

	Ostream out;
	System_File file;
	iscene_serialize_fixture_resource(scene, r, &out, pt_polygon);

	String file_name = tprintf("resources/fixtures/%zu.fixture", r.id);
	if (system_open_file(file_name, File_Operation_NEW, &file)) {
		ostream_build_out_file(&out, &file);
		system_close_file(&file);
	} else {
		return false;
	}

	return true;
}

void scene_save_resources(Scene *scene) {
	auto &resources = scene->resource_fixtures;

	System_File file;
	Ostream out;
	for (auto &r : resources) {
		scoped_temporary_allocation();
		iscene_serialize_fixture_resource(scene, r, &out, false);
		String file_name = tprintf("resources/fixtures/%zu.fixture", r.id);
		if (system_open_file(file_name, File_Operation_NEW, &file)) {
			ostream_build_out_file(&out, &file);
			system_close_file(&file);
		} else {
			system_log(LOG_ERROR, "Scene", "Error saving: %s. File could not be created!", tto_cstring(file_name));
		}
		ostream_reset(&out);
	}

	ostream_free(&out);
}

void scene_load_resources(Scene *scene) {
	scene_clean_resources(scene);

	auto mark = push_temporary_allocator();
	defer{ pop_temporary_allocator(mark); };

	{
		auto files = system_find_files("resources/fixtures", ".fixture", false);
		defer{ memory_free(files.data); };

		System_File file;
		array_resize(&scene->resource_fixtures, files.count);
		for (auto &f : files) {
			scoped_temporary_allocation();
			String content = system_read_entire_file(f.path);
			if (content.count) {
				Tokenization_Status status;
				auto tokens = tokenize(content, &status);
				defer{
					memory_free(content.data);
					memory_free(tokens.data);
				};

				if (status.result == Tokenization_Result_SUCCESS) {
					auto state = deserialize_begin(tokens);
					if (!iscene_deserialize_fixture_resource(scene, array_add(&scene->resource_fixtures), &state)) {
						system_log(LOG_ERROR, "Scene", "Failed loading %s. Invalid file: %s", tto_cstring(f.name), state.error.string);
						scene->resource_fixtures.count -= 1;
					}
					deserialize_end(&state);
				} else {
					system_log(LOG_ERROR, "Scene", "Tokenization of file %s failed at %zu:%zu.", tto_cstring(f.name), status.row, status.column);
				}
			} else {
				system_log(LOG_ERROR, "Scene", "Failed opening file %s.", tto_cstring(f.name));
			}
		}
	}

	{
		auto files = system_find_files("resources/textures", ".png", false);
		defer{ memory_free(files.data); };

		int w, h, n;

		System_File file;
		array_resize(&scene->resource_textures, files.count + 1);
		array_resize(&scene->handle_textures, files.count + 1);

		u8 white_pixels[] = { 0xff,0xff,0xff,0xff };
		const u8 *pixels[] = { white_pixels };
		auto handle_tex = array_add(&scene->handle_textures);
		auto resource_tex = array_add(&scene->resource_textures);
		*handle_tex = gfx_create_texture2d(1, 1, 1, Data_Format_RGBA8_UNORM_SRGB, pixels, Buffer_Usage_IMMUTABLE, 1);
		resource_tex->name[0] = 0;
		resource_tex->name_count = 0;
		resource_tex->key = 0;

		for (auto &f : files) {
			scoped_temporary_allocation();
			String content = system_read_entire_file(f.path);
			if (content.count) {
				u8 *pixels = stbi_load_from_memory(content.data, (int)content.count, &w, &h, &n, 4);
				if (pixels) {
					auto handle_tex = array_add(&scene->handle_textures);
					auto resource_tex = array_add(&scene->resource_textures);
					*handle_tex = gfx_create_texture2d((u32)w, (u32)h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
					u32 count = minimum((u32)f.name.count, (u32)sizeof(Resource_Name) - 1);
					memcpy(resource_tex->name, f.name.data, count);
					resource_tex->name[count] = 0;
					resource_tex->name_count = count;
					resource_tex->key = murmur3_32(resource_tex->name, resource_tex->name_count, 5656);
					stbi_image_free(pixels);
				} else {
					system_log(LOG_ERROR, "Scene", "Failed to parse PNG file %s.", stbi_failure_reason());
				}
			} else {
				system_log(LOG_ERROR, "Scene", "Failed opening file %s.", tto_cstring(f.name));
			}
		}
	}
}

Resource_Entity *iscene_level_find_resource(Level *level, Entity_Id id) {
	auto index = array_find(&level->resources, [](const Resource_Entity &e, Entity_Id id) { return  e.id.handle == id.handle; }, id);
	if (index >= 0)
		return level->resources.data + index;
	return nullptr;
}

void scene_reload_resources(Scene *scene) {
	scene_clean_resources(scene);
	scene_load_resources(scene);

	if (scene->loaded_level != -1) {
		Level *level = scene_current_level_pointer(scene);

		for_list(Rigid_Body, ptr, &scene->rigid_bodies) {
			Rigid_Body &body = ptr->data;
			Resource_Entity *r = iscene_level_find_resource(level, body.entity_id);
			Resource_Fixture *f = scene_find_resource_fixture(scene, r->fixture_id);
			body.fixtures = f->fixtures;
			body.fixture_count = f->fixture_count;
		}
	}
}

void scene_clean_resources(Scene *scene) {
	auto &resources = scene->resource_fixtures;
	for (auto &r : resources) {
		for (u32 i = 0; i < r.fixture_count; ++i)
			memory_free(r.fixtures[i].handle, scene->pool_allocator);
		memory_free(r.fixtures, scene->pool_allocator);
	}
	scene->resource_fixtures.count = 0;

	auto &textures = scene->handle_textures;
	for (auto &t : textures) {
		gfx_destroy_texture2d(t);
	}
	textures.count = 0;
	scene->resource_textures.count = 0;
}

void scene_clean_entities(Scene *scene) {
	for (auto &character : scene->by_type.character) {
		iscene_destroy_rigid_body(scene, character.rigid_body);
	}
	for (auto &obstable : scene->by_type.obstacle) {
		iscene_destroy_rigid_body(scene, obstable.rigid_body);
	}
	scene->by_type.camera.count = 0;
	scene->by_type.character.count = 0;
	scene->by_type.obstacle.count = 0;

	scene->entity_table.count = 0;
	memset(scene->entity_table.keys, 0, sizeof(sizeof(Entity_Hash_Table::Key) * SCENE_MAX_ENTITY_COUNT));
}

void iscene_serialize_entity(Entity *entity, Ostream *out) {
	serialize_fmt_text(out, "header", reflect_info<Entity>(), (char *)entity);
	serialize_fmt_text_next(out);

	switch (entity->type) {
		case Entity_Type_Camera: {
			serialize_fmt_text(out, "data", reflect_info<Camera>(), (char *)entity);
		} break;

		case Entity_Type_Character: {
			serialize_fmt_text(out, "data", reflect_info<Character>(), (char *)entity);
			serialize_fmt_text_next(out);
			serialize_fmt_text(out, "body", reflect_info<Rigid_Body>(), (char *)((Character *)entity)->rigid_body);
		} break;

		case Entity_Type_Obstacle: {
			serialize_fmt_text(out, "data", reflect_info<Obstacle>(), (char *)entity);
			serialize_fmt_text_next(out);
			serialize_fmt_text(out, "body", reflect_info<Rigid_Body>(), (char *)((Obstacle *)entity)->rigid_body);
		} break;

			invalid_default_case();
	}
}

bool iscene_deserialize_entity(Scene *scene, Level *level, Deserialize_State *state) {
	Entity entity;
	if (!deserialize_fmt_text(state, "header", reflect_info<Entity>(), (char *)&entity))
		return false;

	Entity *result = nullptr;
	switch (entity.type) {
		case Entity_Type_Camera: {
			Camera *camera = iscene_add_camera(scene, entity.id, entity.position);
			if (!deserialize_fmt_text(state, "data", reflect_info<Camera>(), (char *)camera)) {
				scene->by_type.camera.count -= 1;
				return false;
			}
			result = camera;
		} break;

		case Entity_Type_Character: {
			Character *character = iscene_add_character(scene, entity.id, entity.position);
			character->rigid_body = iscene_create_new_rigid_body(scene, &entity, nullptr);
			Resource_Entity *resource = iscene_level_find_resource(level, character->id);

			if (deserialize_fmt_text(state, "data", reflect_info<Character>(), (char *)character) &&
				deserialize_fmt_text(state, "body", reflect_info<Rigid_Body>(), (char *)(character->rigid_body))) {
				auto body = character->rigid_body;
				Resource_Fixture *res_fixture = scene_find_resource_fixture(scene, resource->fixture_id);
				if (res_fixture) {
					body->fixtures = res_fixture->fixtures;
					body->fixture_count = res_fixture->fixture_count;
				} else {
					system_log(LOG_ERROR, "Scene", "Unable to find fixture: %zu", resource->fixture_id);
					body->fixture_count = 0;
					body->fixtures = 0;
				}

				if (!scene_find_resource_texture(scene, String(resource->texture_name, strlen(resource->texture_name)), &character->texture)) {
					system_log(LOG_ERROR, "Scene", "Unable to find texture: %s", resource->texture_name);
				}

				body->entity_id = entity.id;

				result = character;
			} else {
				scene->by_type.character.count -= 1;
				iscene_destroy_rigid_body(scene, character->rigid_body);
				return false;
			}
		} break;

		case Entity_Type_Obstacle: {
			Obstacle *obstacle = iscene_add_obstacle(scene, entity.id, entity.position);
			obstacle->rigid_body = iscene_create_new_rigid_body(scene, &entity, nullptr);
			Resource_Entity *resource = iscene_level_find_resource(level, obstacle->id);

			if (deserialize_fmt_text(state, "data", reflect_info<Obstacle>(), (char *)obstacle) &&
				deserialize_fmt_text(state, "body", reflect_info<Rigid_Body>(), (char *)(obstacle->rigid_body))) {
				auto body = obstacle->rigid_body;
				Resource_Fixture *res_fixture = scene_find_resource_fixture(scene, resource->fixture_id);
				if (res_fixture) {
					body->fixtures = res_fixture->fixtures;
					body->fixture_count = res_fixture->fixture_count;
				} else {
					system_log(LOG_ERROR, "Scene", "Unable to find fixture: %zu", resource->fixture_id);
					body->fixture_count = 0;
					body->fixtures = 0;
				}

				if (!scene_find_resource_texture(scene, String(resource->texture_name, strlen(resource->texture_name)), &obstacle->texture)) {
					system_log(LOG_ERROR, "Scene", "Unable to find texture: %s", resource->texture_name);
				}

				body->entity_id = entity.id;

				result = obstacle;
			} else {
				scene->by_type.obstacle.count -= 1;
				iscene_destroy_rigid_body(scene, obstacle->rigid_body);
				return false;
			}
		} break;
	}

	return true;
}

//
//
//

Level *iscene_add_new_level(Scene *scene, const String name, s32 *index) {
	*index = (s32)scene->levels.count;
	auto level = array_add(&scene->levels);
	level->name_count = minimum((u32)name.count, (u32)sizeof(Level_Name) - 1);
	memcpy(level->name, name.data, level->name_count);
	level->name[level->name_count] = 0;
	level->key = murmur3_32(level->name, level->name_count, 5656);
	level->resources.count = level->resources.capacity = 0;
	level->resources.data = nullptr;
	level->resources.allocator = context.allocator;
	return level;
}

Level *iscene_create_new_level(Scene *scene, const String name, s32 *index) {
	String path = tprintf("resources/levels/%s", tto_cstring(name));
	if (system_create_directory(path) == Create_Directory_SUCCESS) {
		return iscene_add_new_level(scene, name, index);
	}
	return nullptr;
}

bool scene_create_new_level(Scene *scene, const String name) {
	s32 index;
	Level *level = iscene_create_new_level(scene, name, &index);
	if (level) {
		push_scoped_temporary_allocator();

		Ostream out;
		defer{ ostream_free(&out); };

		serialize_fmt_text(&out, "level", reflect_info<Level>(), (char *)level);

		System_File file;
		if (!system_open_file(tprintf("resources/levels/%s/level", level->name), File_Operation_NEW, &file)) {
			system_log(LOG_ERROR, "Scene", "%s level file could not be created. Failed to save level.", null_tprintf("resources/levels/%s", level->name));
			return false;
		}

		ostream_build_out_file(&out, &file);
		system_close_file(&file);

		return true;
	}
	return false;
}

bool scene_save_level(Scene *scene) {
	Level *level = scene_current_level_pointer(scene);

	String level_path = tprintf("resources/levels/%s", level->name);

	auto res = system_create_directory(level_path);
	if (res != Create_Directory_ALREADY_EXIST && res != Create_Directory_SUCCESS) {
		system_log(LOG_ERROR, "Scene", "%s level directory could not be created. Failed to save level.", tto_cstring(level_path));
		return false;
	}

	Ostream out;
	defer{ ostream_free(&out); };

	const char *level_dir = null_tprintf("resources/levels/%s", level->name);

	serialize_fmt_text(&out, "level", reflect_info<Level>(), (char *)level);
	System_File file;
	if (!system_open_file(tprintf("resources/levels/%s/level", level->name), File_Operation_NEW, &file)) {
		system_log(LOG_ERROR, "Scene", "%s level file could not be created. Failed to save level.", level_dir);
		return false;
	}

	ostream_build_out_file(&out, &file);
	system_close_file(&file);
	ostream_reset(&out);

	s64 count;

	{
		count = scene->by_type.camera.count;
		auto camera = scene->by_type.camera.data;
		for (s64 index = 0; index < count; ++index, ++camera) {
			iscene_serialize_entity(camera, &out);
			if (system_open_file(tprintf("%s/%zu.ent", level_dir, camera->id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
				system_close_file(&file);
			} else {
				system_log(LOG_ERROR, "Scene", "%s/%zu.ent file could not be opened. Failed to save entity.", level_dir, camera->id);
			}
			ostream_reset(&out);
		}
	}

	{
		count = scene->by_type.character.count;
		auto character = scene->by_type.character.data;
		for (s64 index = 0; index < count; ++index, ++character) {
			//auto resource = iscene_level_find_resource(level, character->id);
			//assert(resource);
			iscene_serialize_entity(character, &out);
			if (system_open_file(tprintf("%s/%zu.ent", level_dir, character->id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
				system_close_file(&file);
			} else {
				system_log(LOG_ERROR, "Scene", "%s/%zu.ent file could not be opened. Failed to save entity.", level_dir, character->id);
			}
			ostream_reset(&out);
		}
	}

	{
		count = scene->by_type.obstacle.count;
		auto obstacle = scene->by_type.obstacle.data;
		for (s64 index = 0; index < count; ++index, ++obstacle) {
			//auto resource = iscene_level_find_resource(level, obstacle->id);
			//assert(resource);
			iscene_serialize_entity(obstacle, &out);
			if (system_open_file(tprintf("%s/%zu.ent", level_dir, obstacle->id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
				system_close_file(&file);
			} else {
				system_log(LOG_ERROR, "Scene", "%s/%zu.ent file could not be opened. Failed to save entity.", level_dir, obstacle->id);
			}
			ostream_reset(&out);
		}
	}

	return true;
}

bool scene_load_level(Scene *scene, const String name) {
	scene_unload_current_level(scene);

	assert(name.count < sizeof(Level_Name) - 1);

	// First search if the level if already loaded
	u32 key = murmur3_32(name.data, name.count, 5656);
	s32 index = (s32)array_find(&scene->levels, [](const Level &l, u32 key, const String name) {
		return l.key == key && string_match(name, String(l.name, l.name_count));
								}, key, name);

	const char *cname = tto_cstring(name);

	// Load from file is the level is not already loaded
	if (index < 0) {
		Level *level = iscene_add_new_level(scene, name, &index);

		auto point = begin_temporary_allocation();

		auto mark = push_temporary_allocator();
		String content = system_read_entire_file(tprintf("resources/levels/%s/level", cname));

		if (!content.count) {
			system_log(LOG_ERROR, "Scene", "Failed to load level: %s", cname);
			scene->levels.count -= 1;
			return false;
		}

		Tokenization_Status status;
		auto tokens = tokenize(content, &status);
		pop_temporary_allocator(mark);

		if (status.result != Tokenization_Result_SUCCESS) {
			system_log(LOG_ERROR, "Scene", "Failed to parse file: resources/levels/%s/level. Invalid file", cname);
			scene->levels.count -= 1;
			return false;
		}

		auto state = deserialize_begin(tokens);
		if (!deserialize_fmt_text(&state, "level", reflect_info<Level>(), (char *)level)) {
			system_log(LOG_ERROR, "Scene", "Failed to parse file: resources/levels/%s/level", cname);
			scene->levels.count -= 1;
			return false;
		}
		deserialize_end(&state);

		end_temporary_allocation(point);
	}

	// Load entities

	auto files = system_find_files(tprintf("resources/levels/%s", cname), ".ent", false);
	defer{ memory_free(files.data); };

	String content;
	System_File file;

	Level *level = &scene->levels[index];

	for (auto &f : files) {
		scoped_temporary_allocation();
		content = system_read_entire_file(f.path);
		if (content.count) {
			Tokenization_Status status;
			auto tokens = tokenize(content, &status);
			defer{
				memory_free(content.data);
				memory_free(tokens.data);
			};

			if (status.result == Tokenization_Result_SUCCESS) {
				auto state = deserialize_begin(tokens);
				if (!iscene_deserialize_entity(scene, level, &state)) {
					system_log(LOG_ERROR, "Scene", "Failed loading %s. Invalid file: %s", tto_cstring(f.path), state.error.string);
				}
				deserialize_end(&state);
			} else {
				system_log(LOG_ERROR, "Scene", "Tokenization of file %s failed at %zu:%zu.", tto_cstring(f.path), status.row, status.column);
			}
		} else {
			system_log(LOG_ERROR, "Scene", "Failed opening file %s.", tto_cstring(f.path));
		}
	}

	scene_refresh_rigid_bodies(scene);

	scene->loaded_level = index;

	Camera *camera = scene_primary_camera(scene);
	camera->target_distance = camera->distance;
	camera->distance += 2;
	camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS;

	return true;
}

bool scene_reload_level(Scene *scene) {
	if (scene->loaded_level >= 0) {
		String name = scene_current_level(scene);
		scene_unload_current_level(scene);
		return scene_load_level(scene, name);
	}
	return false;
}

void scene_unload_current_level(Scene *scene) {
	if (scene->loaded_level >= 0) {
		scene_clean_entities(scene);
		scene->loaded_level = -1;
	}
}

Level *scene_current_level_pointer(Scene *scene) {
	assert(scene->loaded_level >= 0);
	return &scene->levels[scene->loaded_level];
}

const String scene_current_level(Scene *scene) {
	Level *level = scene_current_level_pointer(scene);
	return String(level->name, level->name_count);
}
