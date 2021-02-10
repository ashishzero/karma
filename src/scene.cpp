#include "scene.h"
#include <time.h>

#include "modules/core/systems.h"
#include "modules/gfx/renderer.h"
#include "modules/core/stb_image.h"
#include "modules/imgui/dev.h"

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
	Entity_Reference *slot = table->references + index;
	return slot;
}

inline Resource_Id iscene_get_entity_resource(Entity_Hash_Table *table, Entity_Id id) {
	u32 index = iscene_search_entity_reference(table, id);
	Resource_Id *slot = table->resources + index;
	return *slot;
}

inline void iscene_insert_entity_reference(Entity_Hash_Table *table, Entity_Id id, Entity_Reference ref, Resource_Id resource) {
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
				table->references[index] = ref;
				table->resources[index] = resource;
				table->count += 1;
				return;
			}
			index = entity_hash_index_probe(index);
			key = keys + index;
			next_hash_index = entity_hash_index(key->id);
		}

		if (key->slot != Entity_Hash_Table::Slot::EMPTY) {
			Entity_Reference temp_ref;
			temp_ref = table->references[index];
			table->references[index] = ref;
			ref = temp_ref;

			Resource_Id temp_res;
			temp_res = table->resources[index];
			table->resources[index] = resource;
			resource = temp_res;

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
			table->references[index] = ref;
			table->resources[index] = resource;
			table->count += 1;
			return;
		}
	}
}

inline void iscene_delete_entity_reference(Entity_Hash_Table *table, Entity_Id id) {
	u32 search_index = iscene_search_if_entity_reference_is_present(table, id);
	assert(search_index < SCENE_MAX_ENTITY_COUNT);

	Entity_Hash_Table::Key *key = table->keys + search_index;
	Entity_Reference *ref = table->references + search_index;
	Resource_Id *res = table->resources + search_index;

	Entity_Hash_Table::Key *nxt_key;
	u32 nxt_hash_index;
	Entity_Reference *nxt_ref;
	Resource_Id *nxt_res;

	u32 put_index = search_index;

	for (u32 index = entity_hash_index_probe(search_index); index != search_index; index = entity_hash_index_probe(index)) {
		nxt_key = table->keys + index;

		if (nxt_key->slot == Entity_Hash_Table::Slot::EMPTY) {
			break;
		}

		nxt_hash_index = entity_hash_index(nxt_key->id);

		if ((index > nxt_hash_index && put_index >= nxt_hash_index && put_index < index) ||
			(index < nxt_hash_index && (put_index >= nxt_hash_index || put_index < index))) {
			nxt_ref = table->references + index;
			nxt_res = table->resources + index;

			*key = *nxt_key;
			*ref = *nxt_ref;
			*res = *nxt_res;

			key = nxt_key;
			ref = nxt_ref;
			res = nxt_res;
			put_index = index;
		}
	}

	key->slot = Entity_Hash_Table::Slot::EMPTY;
}

inline bool iscene_delete_entity_reference_if_present(Entity_Hash_Table *table, Entity_Id id, Entity_Reference *data) {
	u32 search_index = iscene_search_if_entity_reference_is_present(table, id);

	if (search_index < SCENE_MAX_ENTITY_COUNT) {
		Entity_Hash_Table::Key *key = table->keys + search_index;
		Entity_Reference *ref = table->references + search_index;
		Resource_Id *res = table->resources + search_index;

		*data = *ref;

		Entity_Hash_Table::Key *nxt_key;
		u32 nxt_hash_index;
		Entity_Reference *nxt_ref;
		Resource_Id *nxt_res;

		u32 put_index = search_index;

		for (u32 index = entity_hash_index_probe(search_index); index != search_index; index = entity_hash_index_probe(index)) {
			nxt_key = table->keys + index;

			if (nxt_key->slot == Entity_Hash_Table::Slot::EMPTY) {
				break;
			}

			nxt_hash_index = entity_hash_index(nxt_key->id);

			if ((index > nxt_hash_index && put_index >= nxt_hash_index && put_index < index) ||
				(index < nxt_hash_index && (put_index >= nxt_hash_index || put_index < index))) {
				nxt_ref = table->references + index;
				nxt_res = table->resources + index;

				*key = *nxt_key;
				*ref = *nxt_ref;
				*res = *nxt_res;

				key = nxt_key;
				ref = nxt_ref;
				res = nxt_res;
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
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Camera, index }, { 0 });
	return camera;
}

inline Character *iscene_add_character(Scene *scene, Entity_Id id, Vec2 p, Resource_Id resource) {
	u32 index = (u32)scene->by_type.character.count;
	Character *character = array_add(&scene->by_type.character);
	character->type = Entity_Type_Character;
	character->id = id;
	character->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Character, index }, resource);
	return character;
}

inline Particle_Emitter *iscene_add_particle_emitter(Scene *scene, Entity_Id id, Vec2 p) {
	u32 index = (u32)scene->by_type.emitter.count;
	Particle_Emitter *emitter = array_add(&scene->by_type.emitter);
	emitter->type = Entity_Type_Particle_Emitter;
	emitter->id = id;
	emitter->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Particle_Emitter, index }, { 0 });
	return emitter;
}

inline Obstacle *iscene_add_obstacle(Scene *scene, Entity_Id id, Vec2 p, Resource_Id resource) {
	u32 index = (u32)scene->by_type.obstacle.count;
	Obstacle *obstacle = array_add(&scene->by_type.obstacle);
	obstacle->type = Entity_Type_Obstacle;
	obstacle->id = id;
	obstacle->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Obstacle, index }, resource);
	return obstacle;
}

inline Bullet *iscene_add_bullet(Scene *scene, Entity_Id id, Vec2 p, Resource_Id resource) {
	u32 index = (u32)scene->by_type.bullet.count;
	Bullet *bullet = array_add(&scene->by_type.bullet);
	bullet->type = Entity_Type_Bullet;
	bullet->id = id;
	bullet->position = p;
	iscene_insert_entity_reference(&scene->entity_table, id, { Entity_Type_Bullet, index }, resource);
	return bullet;
}

inline Rigid_Body *iscene_create_new_rigid_body(Scene *scene, Entity *entity, const Rigid_Body *src) {
	auto node = circular_linked_list_add(&scene->rigid_bodies);
	Rigid_Body *body = &node->data;
	if (src) {
		memcpy(body, src, sizeof(Rigid_Body));
	} else {
		memset(body, 0, sizeof(Rigid_Body));
	}
	body->transform.p = entity->position;
	body->entity_id = entity->id;
	body->next = NULL;
	return body;
}

inline void iscene_destroy_rigid_body(Scene *scene, Rigid_Body *rigid_body) {
	hgrid_remove_body_from_grid(&scene->hgrid, rigid_body);
	auto node = circular_linked_list_node_from_data(rigid_body);
	circular_linked_list_remove(&scene->rigid_bodies, node);
}

//
//
//

Scene *scene_create() {
	Scene *scene = (Scene *)memory_allocate(sizeof(Scene));

	scene->physics = Physics{};
	scene->player_id = {};

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
	array_resize(&scene->by_type.bullet, 10000);
	array_resize(&scene->by_type.emitter, 10000);

	scene->entity_table.keys = (Entity_Hash_Table::Key *)memory_allocate(sizeof(Entity_Hash_Table::Key) * SCENE_MAX_ENTITY_COUNT);
	scene->entity_table.references = (Entity_Reference *)memory_allocate(sizeof(Entity_Reference) * SCENE_MAX_ENTITY_COUNT);
	scene->entity_table.resources = (Resource_Id *)memory_allocate(sizeof(Resource_Id) * SCENE_MAX_ENTITY_COUNT);
	scene->entity_table.count = 0;

	memset(scene->entity_table.keys, 0, sizeof(Entity_Hash_Table::Key) * SCENE_MAX_ENTITY_COUNT);

	scene->resource_header.count = 0;
	scene->resource_header.capacity = 0;
	scene->resource_header.data = nullptr;
	scene->resource_header.allocator = context.allocator;

	scene->fixture_group.count = 0;
	scene->fixture_group.capacity = 0;
	scene->fixture_group.data = nullptr;
	scene->fixture_group.allocator = context.allocator;

	scene->texture_group.count = 0;
	scene->texture_group.capacity = 0;
	scene->texture_group.data = nullptr;
	scene->texture_group.allocator = context.allocator;

	scene->audio_group.count = 0;
	scene->audio_group.capacity = 0;
	scene->audio_group.data = nullptr;
	scene->audio_group.allocator = context.allocator;

	array_resize(&scene->resource_header, 100);
	array_resize(&scene->fixture_group, 100);
	array_resize(&scene->texture_group, 100);
	array_resize(&scene->audio_group, 100);

	circular_linked_list_init(&scene->rigid_bodies, context.allocator);

	scene->hgrid = hgrid_create(8, 256);

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

	audio_mixer(&scene->audio_mixer);

	if (!system_audio(system_update_audio, system_refresh_audio_device, &scene->audio_mixer)) {
		system_display_critical_message("Failed to load audio!");
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

	array_free(&scene->resource_header);
	array_free(&scene->fixture_group);
	array_free(&scene->texture_group);

	circular_linked_list_clear(&scene->rigid_bodies);

	memory_free(scene->entity_table.keys);
	memory_free(scene->entity_table.references);

	for (u32 index = 0; index < Entity_Type_Count - 1; ++index) {
		auto &data = scene->by_type.data[index];
		array_free(&data);
	}

	memory_free(scene);
}

//
//
//

Texture2d_Handle iscene_load_texture(Scene *scene, const Resource_Name &name) {
	scoped_temporary_allocation();
	Texture2d_Handle handle;
	if (name[0]) {
		String path = tprintf("resources/textures/%s", name);

		int w, h, n;
		String content = system_read_entire_file(path);
		if (content.count) {
			u8 *pixels = stbi_load_from_memory(content.data, (int)content.count, &w, &h, &n, 4);
			if (pixels) {
				handle = gfx_create_texture2d((u32)w, (u32)h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
				stbi_image_free(pixels);
			} else {
				system_log(LOG_ERROR, "Scene", "Failed to parse texture: %s", tto_cstring(path));
				return im_white_texture();
			}
		} else {
			system_log(LOG_ERROR, "Scene", "Failed to load file: %s", tto_cstring(path));
			return im_white_texture();
		}

		return handle;
	}

	return im_white_texture();
}

const Resource_Header *scene_create_new_resource(Scene *scene, Resource_Name &name, Fixture *fixtures, u32 fixture_count, const Resource_Name &tex_name, const Mm_Rect &uv) {
	auto header = array_add(&scene->resource_header);
	auto fixture = array_add(&scene->fixture_group);
	auto texture = array_add(&scene->texture_group);

	header->id.handle = iscene_generate_unique_id(scene);
	memcpy(header->name, name, sizeof(Resource_Name));
	memcpy(header->texture, tex_name, sizeof(Resource_Name));

	texture->handle = iscene_load_texture(scene, tex_name);
	texture->uv = uv;

	fixture->count = fixture_count;
	if (fixture_count) {
		fixture->fixtures = new (scene->pool_allocator) Fixture[fixture->count];
	} else {
		fixture->fixtures = nullptr;
	}

	Fixture *dst = fixture->fixtures;
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

	return header;
}

const Array_View<Resource_Header> scene_resource_headers(Scene *scene) {
	return scene->resource_header;
}

Resource_Id	scene_find_entity_resource_id(Scene *scene, Entity_Id id) {
	return iscene_get_entity_resource(&scene->entity_table, id);
}

const Resource_Collection scene_find_resource(Scene *scene, Resource_Id id) {
	s64 index = array_find(&scene->resource_header, [](const Resource_Header &header, Resource_Id id) {
		return header.id.handle == id.handle; }, id);
	if (index >= 0) {
		return  { &scene->resource_header[index], &scene->fixture_group[index], &scene->texture_group[index], (u32)index };
	}
	return  { nullptr, nullptr, nullptr, MAX_UINT32 };
}

Audio_Stream *scene_find_audio_stream(Scene *scene, const char *name) {
	for (auto &a : scene->audio_group) {
		if (strcmp(a.name, name) == 0) {
			return a.stream;
		}
	}
	return nullptr;
}

Entity *scene_clone_entity(Scene *scene, Entity *src, Vec2 p, Resource_Id *resource) {
	Entity_Id id = { iscene_generate_unique_id(scene) };

	Entity *result = nullptr;

	switch (src->type) {
		case Entity_Type_Camera: {
			auto camera = iscene_add_camera(scene, id, p);
			memcpy((u8 *)camera + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Camera) - sizeof(Entity));
			result = camera;
		} break;

		case Entity_Type_Character: {
			Resource_Id resource_id;
			if (resource == nullptr) {
				resource_id = iscene_get_entity_resource(&scene->entity_table, src->id);
			} else {
				resource_id = *resource;
			}

			auto character = iscene_add_character(scene, id, p, resource_id);
			memcpy((u8 *)character + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Character) - sizeof(Entity));
			character->particle_system.particles = (Particle *)memory_allocate(sizeof(Particle) * character->particle_system.particles_count);
			character->boost = audio_mixer_add_audio(&scene->audio_mixer, scene_find_audio_stream(scene, "engine.wav"), false, true);
			character->fall = audio_mixer_add_audio(&scene->audio_mixer, scene_find_audio_stream(scene, "vacuum.wav"), false, true);
			character->rigid_body = iscene_create_new_rigid_body(scene, character, ((Character *)src)->rigid_body);
			hgrid_add_body_to_grid(&scene->hgrid, character->rigid_body);
			character->controller.axis = vec2(0);
			result = character;
		} break;

		case Entity_Type_Obstacle: {
			Resource_Id resource_id;
			if (resource == nullptr) {
				resource_id = iscene_get_entity_resource(&scene->entity_table, src->id);
			} else {
				resource_id = *resource;
			}

			auto obstacle = iscene_add_obstacle(scene, id, p, resource_id);
			memcpy((u8 *)obstacle + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Obstacle) - sizeof(Entity));
			obstacle->rigid_body = iscene_create_new_rigid_body(scene, obstacle, ((Obstacle *)src)->rigid_body);
			hgrid_add_body_to_grid(&scene->hgrid, obstacle->rigid_body);
			result = obstacle;
		} break;

		case Entity_Type_Bullet: {
			Resource_Id resource_id;
			if (resource == nullptr) {
				resource_id = iscene_get_entity_resource(&scene->entity_table, src->id);
			} else {
				resource_id = *resource;
			}

			auto bullet = iscene_add_bullet(scene, id, p, resource_id);
			memcpy((u8 *)bullet + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Bullet) - sizeof(Entity));
			bullet->audio = audio_mixer_add_audio(&scene->audio_mixer, scene->fire, false, false);
			bullet->rigid_body = iscene_create_new_rigid_body(scene, bullet, ((Bullet *)src)->rigid_body);
			hgrid_add_body_to_grid(&scene->hgrid, bullet->rigid_body);
			result = bullet;
		} break;

		case Entity_Type_Particle_Emitter: {
			auto emitter = iscene_add_particle_emitter(scene, id, p);
			memcpy((u8 *)emitter + sizeof(Entity), (u8 *)src + sizeof(Entity), sizeof(Particle_Emitter) - sizeof(Entity));
			emitter->particle_system.particles = (Particle *)memory_allocate(sizeof(Particle) * emitter->particle_system.particles_count);
			result = emitter;
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
		*ref = scene->entity_table.references[index];
		return true;
	}
	return false;
}

Entity *scene_entity_pointer(Scene *scene, Entity_Reference &ref) {
	static constexpr ptrsize ENTITY_TYPE_SIZE[] = {
		sizeof(Camera),
		sizeof(Character),
		sizeof(Obstacle),
		sizeof(Bullet),
		sizeof(Particle_Emitter),
	};
	static_assert(static_count(ENTITY_TYPE_SIZE) == Entity_Type_Count);

	return (Entity *)(scene->by_type.data[ref.type].data + ref.index * ENTITY_TYPE_SIZE[ref.type]);
}

const Array_View<Camera> scene_cameras(Scene *scene) {
	return scene->by_type.camera;
}

const Array_View<Particle_Emitter> scene_emitters(Scene *scene) {
	return scene->by_type.emitter;
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

Character *scene_get_player(Scene *scene) {
	if (scene->player_id.handle) {
		return scene_entity_pointer(scene, scene_get_entity(scene, scene->player_id))->as<Character>();
	}

	return nullptr;
}

Vec4 color_id_get_color(Color_Id color_id) {
	static const Vec4 COLORS[] = {
		vec4(0, 1, 1),
		vec4(1, 1, 0),
		vec4(0, 1, 0),
		vec4(0.6f, 0, 0)
	};
	static_assert(static_count(COLORS) == (int)Color_Id_COUNT);
	return COLORS[(int)color_id];
}

r32 color_id_get_intensity(Color_Id color_id) {
	static const r32 INTENS[] = {
		2.0f,
		2.0f,
		2.0f,
		10.0f
	};
	static_assert(static_count(INTENS) == (int)Color_Id_COUNT);
	return INTENS[(int)color_id];	
}

Character *scene_spawn_player(Scene *scene, Vec2 p, Color_Id color_id) {
	if (!scene->player_id.handle) {
		Resource_Id id = { 6895733819830550519 }; // TODO: hard coded resource id
		Resource_Collection resource = scene_find_resource(scene, id);

		Resource_Id particle_id = { 6926504382549284097 };
		Resource_Collection particle_resource = scene_find_resource(scene, particle_id);

		Rigid_Body body;
		Character src;
		ent_init_character(&src, scene, p, color_id, &body, resource.fixture, resource.texture, resource.index, particle_resource.texture, particle_resource.index);
		auto player = scene_clone_entity(scene, &src, p, &id)->as<Character>();
		scene->player_id = player->id;
		player->size = vec2(0.5f);
		player->rigid_body->transform.xform = mat2_scalar(0.5f, 0.5f);
		player->rigid_body->gravity = 1;

		return player;
	}

	return scene_get_player(scene);
}

Particle_Emitter *scene_spawn_emitter(Scene *scene, Vec2 p, Vec4 color, r32 intensity) {
	Resource_Id particle_id = { 6926504382549284097 };
	Resource_Collection particle_resource = scene_find_resource(scene, particle_id);

	Particle_Emitter src;
	ent_init_particle_emitter(&src, p, particle_resource.texture, particle_resource.index);

	auto emitter = scene_clone_entity(scene, &src, p)->as<Particle_Emitter>();
	emitter->particle_system.properties.density.min = 0.1f;
	emitter->particle_system.properties.density.max = 0.1f;
	emitter->particle_system.properties.initial_velocity_x.min = -0.5f;
	emitter->particle_system.properties.initial_velocity_x.max = +0.5f;
	emitter->particle_system.properties.initial_velocity_x.control = Distribution_Control_EXTREMES;
	emitter->particle_system.properties.initial_velocity_y.min = -0.5f;
	emitter->particle_system.properties.initial_velocity_y.max = +0.5f;
	emitter->particle_system.properties.initial_velocity_y.control = Distribution_Control_EXTREMES;
	emitter->particle_system.properties.life_span.min = 0.5f;
	emitter->particle_system.properties.life_span.max = 0.7f;
	emitter->particle_system.properties.emission_rate = 7;
	emitter->particle_system.properties.fade_in = 0.1f;
	emitter->particle_system.properties.fade_out = 0.1f;
	emitter->particle_system.properties.intensity = intensity;
	emitter->particle_system.properties.color_b = rgb_to_hsv(vec4(color.rgb, 0));
	emitter->particle_system.properties.color_a = rgb_to_hsv(color);
	emitter->particle_system.loop = 4;
	emitter->remove_on_finish = true;

	return emitter;
}

Bullet *scene_spawn_bullet(Scene *scene, Vec2 p, Color_Id color_id, Vec2 dir) {
	Resource_Id id = { 6926438366305714858 }; // TODO: hard coded resource id
	Resource_Collection resource = scene_find_resource(scene, id);

	Rigid_Body body;
	Bullet src;

	r32 radius = 0.05f * random_r32_range_r32(0.9f, 1.0f);
	r32 intensity = 2.0f * color_id_get_intensity(color_id);
	r32 life_span = 1.0f * random_r32_range_r32(0.6f, 1.0f);
	r32 initial_velocity = 5 * random_r32_range_r32(0.7f, 1.0f);

	ent_init_bullet(&src, scene, p, color_id, radius, intensity, color_id_get_color(color_id), life_span, &body, resource.fixture);
	auto bullet = scene_clone_entity(scene, &src, p, &id)->as<Bullet>();
	bullet->rigid_body->transform.xform = mat2_scalar(radius, radius);
	bullet->rigid_body->velocity = initial_velocity * dir;
	bullet->rigid_body->restitution = 1;
	bullet->rigid_body->drag = 0;

	audio_pitch_factor(bullet->audio, random_r32_range_r32(0.9f, 1.0f));
	audio_play(bullet->audio);

	return bullet;
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
	{
		constexpr r32 PLAYER_FORCE = 10;

		auto count = scene->by_type.character.count;
		auto &characters = scene->by_type.character;
		for (auto index = 0; index < count; ++index) {
			auto &character = characters[index];
			character.rigid_body->force = vec2(0);
			character.rigid_body->force += PLAYER_FORCE * vec2_normalize_check(character.controller.axis);

			if (character.hit) {
				character.hit -= 1;
			}
		}
	}

	{
		auto count = scene->by_type.bullet.count;
		auto &bullets = scene->by_type.bullet;
		for (auto index = 0; index < count; ++index) {
			auto &bullet = bullets[index];
			bullet.rigid_body->force = vec2(0);
		}
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

void particle_system_emit_particle(Particle_System *system) {
	// TODO: reallocate particles or use the last one??
	if (system->emit_count == system->particles_count) {
		system->emit_count -= 1;
	}

	Particle *particle = system->particles + system->emit_count;
	system->emit_count += 1;

	const Particle_Emitter_Property &properties = system->properties;

	switch (properties.kind) {
		case Particle_Emitter_Property::POINT: {
			particle->position = properties.a;
		} break;

		case Particle_Emitter_Property::LINE: {
			auto value = random_get_zero_to_one(properties.control);
			particle->position = value * (properties.a) + properties.b;
		} break;

		case Particle_Emitter_Property::ELLIPSE: {
			auto value_a = random_get_zero_to_one(properties.control);
			auto value_b = random_get_zero_to_one(properties.control) * MATH_PI * 2;
			particle->position.x = sqrtf(value_a) * properties.b.x * cosf(value_b) + properties.a.x;
			particle->position.y = sqrtf(value_a) * properties.b.y * sinf(value_b) + properties.a.y;
		} break;

		case Particle_Emitter_Property::TRIANGLE: {
			// https://www.cs.princeton.edu/~funk/tog02.pdf, Section 4.2
			auto value_a = sqrtf(random_get_zero_to_one(properties.control));
			auto value_b = random_get_zero_to_one(properties.control);
			particle->position = (1 - value_a) * properties.a + (value_a * (1 - value_b)) * properties.b + (value_a * value_b) * properties.c;
		} break;

		case Particle_Emitter_Property::RECT: {
			auto value_x = random_get_zero_to_one(properties.control);
			auto value_y = random_get_zero_to_one(properties.control);
			particle->position.x = value_x * properties.a.x;
			particle->position.y = value_y * properties.a.y;
		} break;

			invalid_default_case();
	}

	particle->density = random_get_range(properties.density);
	particle->scale_a = random_get_range(properties.scale_a);
	particle->scale_b = random_get_range(properties.scale_b);
	particle->rotation = random_get_range(properties.rotation);
	particle->velocity.x = random_get_range(properties.initial_velocity_x);
	particle->velocity.y = random_get_range(properties.initial_velocity_y);
	particle->drag = random_get_range(properties.drag);
	particle->spin = random_get_range(properties.spin);
	particle->color_a = properties.color_a;
	particle->color_b = properties.color_b;
	particle->life_span = random_get_range(properties.life_span);
	particle->life = 0;
	particle->external_force = vec2(0);

	assert(particle->life_span != 0);
}

void simulate_particle_system(Particle_System *system, r32 dt) {
	for (u32 particle_index = 0; particle_index < system->emit_count; ++particle_index) {
		auto particle = system->particles + particle_index;

		particle->life += dt;

		if (particle->life >= particle->life_span) {
			system->particles[particle_index] = system->particles[system->emit_count - 1];
			system->emit_count -= 1;
			particle_index -= 1;
		} else {
			r32 t = particle->life / particle->life_span;

			// TODO: Perhaps we should store the inverse of density and volume, instead of just density and volumne
			r32  scale = lerp(particle->scale_a, particle->scale_b, t) + 0.001f; // TODO: Should we do this to prevent division by 0?
			r32  volume = scale * system->properties.volume_factor;
			r32  mass = (particle->density * volume);
			r32  imass = 1.0f / mass;
			Vec2 force;
			force.x = random_get_range(system->properties.force_x);
			force.y = random_get_range(system->properties.force_y);
			force += particle->external_force;
			Vec2 acceleration = force * imass;
			particle->velocity += dt * acceleration;
			particle->velocity *= powf(0.5f, particle->drag * dt); // TODO: powf is expensive!
			particle->position += dt * particle->velocity;
			particle->rotation += dt * particle->spin;
		}
	}

	if (system->loop < 0) {
		auto dt_milli = dt * 1000.0f;
		system->time_elapsed += dt_milli;
		while (system->time_elapsed >= 60.0f) {
			for (u32 i = 0; i < system->properties.emission_rate; ++i) {
				particle_system_emit_particle(system);
			}
			system->time_elapsed -= 60.0f;
		}
	} else if (system->loop > 0) {
		auto dt_milli = dt * 1000.0f;
		system->time_elapsed += dt_milli;
		while (system->time_elapsed >= 60.0f && system->loop > 0) {
			for (u32 i = 0; i < system->properties.emission_rate; ++i) {
				particle_system_emit_particle(system);
			}
			system->time_elapsed -= 60.0f;
			system->loop -= 1;
		}
	}
}

void iscene_clean_particle_system(Particle_System *system) {
	memory_free(system->particles);
}

void iscene_post_simulate(Scene *scene) {
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
			if (id.handle == scene->player_id.handle) {
				scene->player_id = { 0 };
			}
			if (iscene_delete_entity_reference_if_present(&scene->entity_table, id, &ref)) {
				audio_mixer_remove_audio(&scene->audio_mixer, characters[ref.index].boost);
				audio_mixer_remove_audio(&scene->audio_mixer, characters[ref.index].fall);
				iscene_clean_particle_system(&characters[ref.index].particle_system);
				iscene_destroy_rigid_body(scene, characters[ref.index].rigid_body);
				memory_free(characters[ref.index].particle_system.particles);
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

	{
		rma = scene->removed_entity + Entity_Type_Bullet;
		count = rma->count;
		auto &bullets = scene->by_type.bullet;
		for (s64 index = 0; index < count; ++index) {
			id = rma->data[index];
			if (iscene_delete_entity_reference_if_present(&scene->entity_table, id, &ref)) {
				audio_mixer_remove_audio(&scene->audio_mixer, bullets[ref.index].audio);
				iscene_destroy_rigid_body(scene, bullets[ref.index].rigid_body);
				bullets[ref.index] = bullets[bullets.count - 1];
				bullets.count -= 1;
				if (ref.index < bullets.count) {
					auto data = iscene_get_entity_reference(&scene->entity_table, bullets[ref.index].id);
					data->index = ref.index;
				}
			}
		}
	}
	
	{
		rma = scene->removed_entity + Entity_Type_Particle_Emitter;
		count = rma->count;
		auto &emitters = scene->by_type.emitter;
		for (s64 index = 0; index < count; ++index) {
			id = rma->data[index];
			if (iscene_delete_entity_reference_if_present(&scene->entity_table, id, &ref)) {
				iscene_clean_particle_system(&emitters[ref.index].particle_system);
				emitters[ref.index] = emitters[emitters.count - 1];
				emitters.count -= 1;
				if (ref.index < emitters.count) {
					auto data = iscene_get_entity_reference(&scene->entity_table, emitters[ref.index].id);
					data->index = ref.index;
				}
			}
		}
	}
}

void scene_simulate(Scene *scene, r32 dt) {
	for (s64 index = 0; index < Entity_Type_Count; ++index) {
		auto &rm = scene->removed_entity[index];
		rm.capacity = 0;
		rm.count = 0;
		rm.data = nullptr;
	}

	if (iscene_simulate_world_enabled(scene)) {
		if (scene->by_type.character.count) {
			auto player = scene_get_player(scene);
			if (player) {
				auto camera = scene_primary_camera(scene);
				camera->behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT;
				camera->target_position = player->position;
			}
		}

		{
			auto count = scene->by_type.character.count;
			auto &characters = scene->by_type.character;
			for (auto index = 0; index < count; ++index) {
				auto &character = characters[index];
				character.rigid_body->transform.p = character.position;
				simulate_particle_system(&character.particle_system, dt);
			}
		}

		auto player = scene_get_player(scene);
		auto src = player->position;

		{
			auto count = scene->by_type.bullet.count;
			auto &bullets = scene->by_type.bullet;
			for (auto index = 0; index < count; ++index) {
				auto &bullet = bullets[index];
				bullet.rigid_body->transform.p = bullet.position;

				if (bullet.age >= bullet.life_span) {
					scene_remove_entity(scene, bullet.id);
				}
				bullet.age += dt;

				auto d = bullet.position - src;
				bullet.audio->attenuation = vec2_dot(d, d);
			}
		}

		{
			auto count = scene->by_type.emitter.count;
			auto &emitters = scene->by_type.emitter;
			for (auto index = 0; index < count; ++index) {
				auto &emitter = emitters[index];
				
				if (emitter.remove_on_finish && emitter.particle_system.loop == 0) {
					scene_remove_entity(scene, emitter.id);
				}
				
				simulate_particle_system(&emitter.particle_system, dt);
			}
		}

		for_list(Rigid_Body, ptr, &scene->rigid_bodies) {
			if (ptr->data.type == Rigid_Body_Type_Dynamic) {
				ptr->data.velocity += dt * ptr->data.force - vec2(0, scene->physics.gravity * ptr->data.gravity);
				ptr->data.velocity *= powf(0.5f, ptr->data.drag * dt);
				ptr->data.transform.p += dt * ptr->data.velocity;
				ptr->data.bounding_box = scene_rigid_body_bounding_box(&ptr->data, 0);
				hgrid_move_body(&scene->hgrid, &ptr->data);
			}
			clear_bit(ptr->data.flags, Rigid_Body_COLLIDING);
			clear_bit(ptr->data.flags, Rigid_Body_BOUNDING_BOX_COLLIDING);
		}

		{
			constexpr r32 BULLET_SPAWN = 0.15f;

			auto count = scene->by_type.character.count;
			auto &characters = scene->by_type.character;
			for (auto index = 0; index < count; ++index) {
				auto &character = characters[index];

				r32 radius = sqrtf(character.size.x * character.size.x + character.size.y * character.size.y);

				r32 v = character.rigid_body->velocity.x;
				character.rotation = atan2f(1, v) - MATH_TAU;

				auto dir = -vec2_normalize_check(vec2(v, 1));
				character.particle_system.position = character.position + 0.5f * dir * radius;

				character.particle_system.properties.initial_velocity_x.min = -0.8f + dir.x;
				character.particle_system.properties.initial_velocity_x.max = +0.8f + dir.x;

				if (character.controller.axis.x || character.controller.axis.y > 0) {
					character.particle_system.loop = 1;
					audio_play(character.boost, 0.8f);
				} else {
					character.particle_system.loop = 0;
					audio_vanish(character.boost, 0.8f);
				}

				if (character.controller.attack) {
					if (character.controller.cool_down <= 0) {
						auto dir = vec2_normalize_check(character.controller.pointer);
						scene_spawn_bullet(scene, character.position + 0.5f * radius * dir, character.color_id, dir);
						character.controller.cool_down = BULLET_SPAWN;
					} else {
						character.controller.cool_down -= dt;
					}
				} else {
					character.controller.cool_down = 0;
				}
			}
		}

		iscene_post_simulate(scene);

		Contact_Manifold manifold;
		manifold.penetration = 0;

		// TODO: Continuous collision detection
		for (int iteration = 0; iteration < SCENE_SIMULATION_MAX_ITERATION; ++iteration) {
			for (s64 index = 0; index < Entity_Type_Count; ++index) {
				auto &rm = scene->removed_entity[index];
				rm.capacity = 0;
				rm.count = 0;
				rm.data = nullptr;
			}

			for_list(Rigid_Body, a_ptr, &scene->rigid_bodies) {
				for_list_offset(Rigid_Body, b_ptr, a_ptr, &scene->rigid_bodies) {
					auto &a = a_ptr->data;
					auto &b = b_ptr->data;

					if (a.type == Rigid_Body_Type_Static && b.type == Rigid_Body_Type_Static) {
						continue;
					}

					if (hgrid_test_collision(&scene->hgrid, &a, &b)) {

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

										auto entt_a = scene_get_entity(scene, a.entity_id);
										auto entt_b = scene_get_entity(scene, b.entity_id);

										const r32 INCREASE_COLOR_EFFECT = 0.02f;
										const u32 HIT_COUNT = 15;

										if (entt_a.type == Entity_Type_Bullet && entt_b.type != Entity_Type_Bullet) {
											auto spawn_color_id = scene_entity_pointer(scene, entt_a)->as<Bullet>()->color_id;
											
											if (entt_b.type == Entity_Type_Character) {
												auto character = scene_entity_pointer(scene, entt_b)->as<Character>();
												if (character->color_id != spawn_color_id) {
													character->color_values[spawn_color_id] += INCREASE_COLOR_EFFECT;
													character->hit = HIT_COUNT;

													if (character->color_values[spawn_color_id] >= 1) {
														character->color_values[spawn_color_id] = 1;

														bool another_full_present = false;
														for (auto in = 0; in < Color_Id_COUNT; ++in) {
															if (in != spawn_color_id && character->color_values[in] == 1) {
																another_full_present = true;
																break;
															}
														}

														if (another_full_present) {
															for (auto in = 0; in < Color_Id_COUNT; ++in) {
																character->color_values[in] *= 0.5f;
															}
														} else {
															character->color_id = spawn_color_id;
														}
													} else {
														r32 max_value = character->color_values[character->color_id];

														for (auto in = 0; in < Color_Id_COUNT; ++in) {
															if (max_value < character->color_values[in]) {
																max_value = character->color_values[in];
																character->color_id = (Color_Id)in;
															}
														}
													}
												}
											}

											scene_spawn_emitter(scene, manifold.contacts[0], color_id_get_color(spawn_color_id), 2.0f * color_id_get_intensity(spawn_color_id));
											auto audio = audio_mixer_add_audio(&scene->audio_mixer, scene->hit, false, false);

											auto player = scene_get_player(scene);
											auto src = player->position;

											auto d = src - manifold.contacts[0];
											audio->attenuation = vec2_dot(d, d);
											audio_play(audio, 0.5f);
											audio_mixer_remove_audio(&scene->audio_mixer, audio);

											scene_remove_entity(scene, entt_a, a.entity_id);
										} else if (entt_b.type == Entity_Type_Bullet && entt_a.type != Entity_Type_Bullet) {
											auto spawn_color_id = scene_entity_pointer(scene, entt_b)->as<Bullet>()->color_id;

											if (entt_a.type == Entity_Type_Character) {
												auto character = scene_entity_pointer(scene, entt_a)->as<Character>();
												if (character->color_id != spawn_color_id) {
													character->color_values[spawn_color_id] += INCREASE_COLOR_EFFECT;
													character->hit = HIT_COUNT;

													if (character->color_values[spawn_color_id] >= 1) {
														character->color_values[spawn_color_id] = 1;

														bool another_full_present = false;
														for (auto in = 0; in < Color_Id_COUNT; ++in) {
															if (in != spawn_color_id && character->color_values[in] == 1) {
																another_full_present = true;
																break;
															}
														}
														
														if (another_full_present) {
															for (auto in = 0; in < Color_Id_COUNT; ++in) {
																character->color_values[in] *= 0.5f;
															}
														} else {
															character->color_id = spawn_color_id;
														}
													} else {
														r32 max_value = character->color_values[character->color_id];

														for (auto in = 0; in < Color_Id_COUNT; ++in) {
															if (max_value < character->color_values[in]) {
																max_value = character->color_values[in];
																character->color_id = (Color_Id)in;
															}
														}
													}
												}
											}

											scene_spawn_emitter(scene, manifold.contacts[0], color_id_get_color(spawn_color_id), 2.0f * color_id_get_intensity(spawn_color_id));
											auto audio = audio_mixer_add_audio(&scene->audio_mixer, scene->hit, false, false);

											auto player = scene_get_player(scene);
											auto src = player->position;

											auto d = src - manifold.contacts[0];
											audio->attenuation = vec2_dot(d, d);
											audio_play(audio, 0.5f);
											audio_mixer_remove_audio(&scene->audio_mixer, audio);

											scene_remove_entity(scene, entt_b, b.entity_id);
										} else {
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
											hgrid_move_body(&scene->hgrid, &a);
											hgrid_move_body(&scene->hgrid, &b);

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
			}


			iscene_post_simulate(scene);
		}

		// Update positions
		{
			auto count = scene->by_type.character.count;
			auto &characters = scene->by_type.character;
			for (auto index = 0; index < count; ++index) {
				auto &character = characters[index];

				auto new_p = character.rigid_body->transform.p;

				if ((new_p.y - character.position.y < -0.001f) && character.controller.axis.y < 0) {
					audio_play(character.fall, 0.3f);
				} else {
					audio_vanish(character.fall, 0.8f);
				}

				character.position = character.rigid_body->transform.p;
			}
		}
		{
			auto count = scene->by_type.bullet.count;
			auto &bullets = scene->by_type.bullet;
			for (auto index = 0; index < count; ++index) {
				auto &bullet = bullets[index];
				bullet.position = bullet.rigid_body->transform.p;
			}
		}

		// Simulate camera
		{
			auto count = scene->by_type.camera.count;
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
}

void scene_update(Scene *scene, r32 sim_factor) {
	Dev_TimedBlockBegin(AudioUpdate);
	scene->audio_mixer.pitch_factor = sim_factor;
	audio_mixer_update(&scene->audio_mixer);
	Dev_TimedBlockEnd(AudioUpdate);

	editor_update(scene, &scene->editor);
}

void scene_end(Scene *scene) {
	
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

inline bool iscene_render_broadphase_enabled(Scene *scene) {
	auto mode = scene->editor.mode;
	auto flags = scene->editor.flags;

	if (mode == Editor_Mode_GAME) return false;
	return (flags & Editor_Flag_Bit_RENDER_BROADPHASE) && (mode == Editor_Mode_GAME_DEVELOPER || mode == Editor_Mode_LEVEL_EDITOR);
}

void iscene_render_particles(Scene *scene, Particle_System *system) {
	auto text = scene->texture_group[system->texture.index];
	im2d_bind_texture(text.handle);

	Vec2 c = system->position;
	for (u32 index = 0; index < system->emit_count; ++index) {
		auto p = system->particles + index;
		r32 t = p->life / p->life_span;

		r32 fade_t = 1;
		if (p->life < system->properties.fade_in) {
			fade_t *= p->life / system->properties.fade_in;
		} else if (p->life_span - p->life < system->properties.fade_out) {
			fade_t *= (p->life_span - p->life) / system->properties.fade_out;
		}

		Vec4 color = hsv_to_rgb(lerp(p->color_a, p->color_b, t));
		color.rgb *= system->properties.intensity;
		color.alpha *= fade_t * system->properties.opacity;
		im2d_rect_rotated(c + p->position, vec2(p->scale_a, p->scale_b), p->rotation, text.uv, color);
	}
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
		{
			s64 count = scene->by_type.character.count;
			auto &characters = scene->by_type.character;
			for (s64 index = 0; index < count; ++index) {
				Character &c = characters[index];

				auto text = scene->texture_group[c.texture.index];

				im2d_bind_texture(text.handle);

				Vec4 color;
				if (c.hit) {
					color = vec4(1);
				} else {
					color = color_id_get_color(c.color_id);
				}

				im2d_rect_centered_rotated(c.position, c.size, c.rotation, text.uv, color);
			}

			for (s64 index = 0; index < count; ++index) {
				Character &c = characters[index];
				iscene_render_particles(scene, &c.particle_system);
			}

			im2d_unbind_texture();

			for (s64 index = 0; index < count; ++index) {
				Character &c = characters[index];
				Vec2 point = vec2_normalize_check(c.controller.pointer);
				if (!vec2_null(point)) {
					r32 intensity = color_id_get_intensity(c.color_id);
					r32 radius = sqrtf(c.size.x * c.size.x + c.size.y * c.size.y);
					auto color = color_id_get_color(c.color_id);
					im2d_circle(c.position + 0.50f * point * radius, 0.020f, intensity * 2.00f * color);
					im2d_circle(c.position + 0.65f * point * radius, 0.010f, intensity * 1.75f * color);
					im2d_circle(c.position + 0.75f * point * radius, 0.008f, intensity * 1.50f * color);
					im2d_circle(c.position + 0.85f * point * radius, 0.006f, intensity * 1.00f * color);
				}
			}
		}

		{
			s64 count = scene->by_type.bullet.count;
			auto &bullets = scene->by_type.bullet;
			for (s64 index = 0; index < count; ++index) {
				Bullet &b = bullets[index];
				im2d_circle(b.position, b.radius, b.color * map(0, b.life_span, 1, b.intensity, b.age));
			}
		}

		{
			s64 count = scene->by_type.emitter.count;
			auto &emitters = scene->by_type.emitter;
			for (s64 index = 0; index < count; ++index) {
				Particle_Emitter &e = emitters[index];
				iscene_render_particles(scene, &e.particle_system);
			}
		}

		{
			s64 count = scene->by_type.obstacle.count;
			auto &obs = scene->by_type.obstacle;
			for (s64 index = 0; index < count; ++index) {
				Obstacle &o = obs[index];

				auto text = scene->texture_group[o.texture.index];

				im2d_bind_texture(text.handle);

				im2d_rect_centered_rotated(o.position, o.size, o.rotation, text.uv, vec4(1));
			}
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

	if (iscene_render_broadphase_enabled(scene)) {
		r32 size = scene->hgrid.size / 2;
		r32 inc = scene->hgrid.size / (powf(2, (float)(scene->hgrid.level - 1)));
		for (r32 y = -size; y < size; y += inc) {
			for (r32 x = -size; x < size; x += inc) {
				im2d_rect_outline(vec2(x, y), vec2(inc), vec4(1));
			}
		}
	}
	im2d_end();

	if (iscene_render_world_enabled(scene)) {
		// Render Status

		const auto VIEW_HEIGHT = 10.0f;
		const auto VIEW_WIDTH = aspect_ratio * VIEW_HEIGHT;

		view = orthographic_view(0.0f, VIEW_WIDTH, VIEW_HEIGHT, 0.0f);

		im2d_begin(view);

		struct Color_Id_Value {
			Color_Id id;
			r32 value;
		};
		Color_Id_Value color_id_values[Color_Id_COUNT];

		auto player = scene_get_player(scene);

		for (auto i = 0; i < Color_Id_COUNT; ++i) {
			color_id_values[i].id = (Color_Id)i;
			color_id_values[i].value = player->color_values[i];
		}

		sort_insert(color_id_values, Color_Id_COUNT, [](Color_Id_Value &a, Color_Id_Value &b) {
			return sgn(b.value - a.value);
		});

		const r32 cx = 0.8f;
		const r32 cy = VIEW_HEIGHT - cx;
		const r32 color_potion_radius = 0.6f;
		const r32 color_rect_height = color_potion_radius / (r32)(Color_Id_COUNT);
		const r32 color_rect_max_width = 2.5f;
		
		const r32 color_width_ratio[] = {
			1.0f, 0.9f, 0.7f, 0.5f
		};
		const r32 color_start_offset[] = {
			0.0f, 0.3f, 0.6f, 0.9f
		};
		
		r32 value_offset;
		r32 y = cy + color_potion_radius - color_rect_height;
		for (auto i = 0; i < Color_Id_COUNT; ++i, y -= color_rect_height) {
			value_offset = color_start_offset[i] * color_potion_radius;
			im2d_rect(vec2(cx + value_offset, y), 
					  vec2((color_rect_max_width - value_offset) * color_width_ratio[i], color_rect_height),
					  vec4_hadamard(vec4(0.07f, 0.07f, 0.07f, 1.0f), color_id_get_color(color_id_values[i].id)));
		}

		y = cy + color_potion_radius - color_rect_height;
		for (auto i = 0; i < Color_Id_COUNT; ++i, y -= color_rect_height) {
			value_offset = color_start_offset[i] * color_potion_radius;
			im2d_rect(vec2(cx + value_offset, y), 
					  vec2(color_id_values[i].value * (color_rect_max_width - value_offset) * color_width_ratio[i], color_rect_height),
					  color_id_get_color(color_id_values[i].id));
		}

		im2d_circle(vec2(cx, cy), color_potion_radius, vec4(0, 0, 0));

		auto potion_color =color_id_get_color(player->color_id);
		potion_color.xyz *= color_id_get_intensity(player->color_id);
		im2d_circle(vec2(cx, cy), color_potion_radius * 0.8f, potion_color);

		im2d_end();
	}

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

void iscene_serialize_resource(Scene *scene, Resource_Header &header, Texture_Group &texture, Fixture_Group &fixture, Ostream *out, bool pt_polygon) {
	serialize_fmt_text(out, "header", reflect_info<Resource_Header>(), (char *)&header);
	serialize_fmt_text_next(out);
	serialize_fmt_text(out, "uv", reflect_info<Mm_Rect>(), (char *)&texture.uv);
	serialize_fmt_text_next(out);
	serialize_fmt_text(out, "fixture_count", reflect_info(fixture.count), (char *)&fixture.count);

	u32 count = fixture.count;
	auto f = fixture.fixtures;
	for (u32 index = 0; index < count; ++index, ++f) {
		serialize_fmt_text_next(out);
		iscene_serialize_fixture(scene, f, out, pt_polygon);
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

bool iscene_deserialize_resource(Scene *scene, Resource_Header *header, Texture_Group *texture, Fixture_Group *fixture, Deserialize_State *state) {
	auto mark = push_allocator(scene->pool_allocator);
	defer{ pop_allocator(mark); };

	if (!deserialize_fmt_text(state, "header", reflect_info<Resource_Header>(), (char *)header))
		return false;

	deserialize_fmt_text(state, "uv", reflect_info<Mm_Rect>(), (char *)&texture->uv);

	if (!deserialize_fmt_text(state, "fixture_count", reflect_info(fixture->count), (char *)&fixture->count))
		return false;

	u32 count = fixture->count;
	fixture->fixtures = (Fixture *)memory_allocate(count * sizeof(Fixture));

	auto f = fixture->fixtures;
	for (u32 index = 0; index < count; ++index, ++f) {
		if (!iscene_deserialize_fixture(f, state)) {
			memory_free(fixture->fixtures);
			return false;
		}
	}

	texture->handle = iscene_load_texture(scene, header->texture);

	return true;
}

//
//
//

bool scene_save_resource(Scene *scene, Resource_Header &header, Texture_Group &texture, Fixture_Group &fixture, bool pt_polygon) {
	auto mark = push_temporary_allocator();
	defer{ pop_temporary_allocator(mark); };

	Ostream out;
	System_File file;
	iscene_serialize_resource(scene, header, texture, fixture, &out, pt_polygon);

	String file_name = tprintf("resources/sprites/%zu.sprite", header.id);
	if (system_open_file(file_name, File_Operation_NEW, &file)) {
		ostream_build_out_file(&out, &file);
		system_close_file(&file);
	} else {
		return false;
	}

	return true;
}

void scene_save_resources(Scene *scene) {
	System_File file;
	Ostream out;

	s64 count = scene->resource_header.count;

	for (s64 index = 0; index < count; ++index) {
		scoped_temporary_allocation();
		iscene_serialize_resource(scene, scene->resource_header[index], scene->texture_group[index], scene->fixture_group[index], &out, false);
		String file_name = tprintf("resources/sprites/%zu.sprite", scene->resource_header[index].id);
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

bool iscene_load_resource(Scene *scene, const String path, Resource_Header *header, Texture_Group *texture, Fixture_Group *fixture) {
	scoped_temporary_allocation();
	String content = system_read_entire_file(path);
	if (content.count) {
		Tokenization_Status status;
		auto tokens = tokenize(content, &status);
		defer{
			memory_free(content.data);
			memory_free(tokens.data);
		};

		if (status.result == Tokenization_Result_SUCCESS) {
			auto state = deserialize_begin(tokens);
			if (!iscene_deserialize_resource(scene, header, texture, fixture, &state)) {
				system_log(LOG_ERROR, "Scene", "Failed loading %s. Invalid file: %s", tto_cstring(path), state.error.string);
				return false;
			}
			deserialize_end(&state);
		} else {
			system_log(LOG_ERROR, "Scene", "Tokenization of file %s failed at %zu:%zu.", tto_cstring(path), status.row, status.column);
			return false;
		}
	} else {
		system_log(LOG_ERROR, "Scene", "Failed opening file %s.", tto_cstring(path));
		return false;
	}

	return true;
}

void scene_load_resources(Scene *scene) {
	scene_clean_resources(scene);

	{
		auto mark = push_temporary_allocator();
		defer{ pop_temporary_allocator(mark); };

		auto files = system_find_files("resources/sprites", ".sprite", false);
		defer{ memory_free(files.data); };

		System_File file;
		array_resize(&scene->resource_header, files.count);
		array_resize(&scene->texture_group, files.count);
		array_resize(&scene->fixture_group, files.count);

		for (auto &f : files) {
			if (!iscene_load_resource(scene, f.path, array_add(&scene->resource_header), array_add(&scene->texture_group), array_add(&scene->fixture_group))) {
				scene->resource_header.count -= 1;
				scene->texture_group.count -= 1;
				scene->fixture_group.count -= 1;
			}
		}
	}

	{
		auto files = system_find_files("resources/audios", ".wav", false);
		defer{ memory_free(files.data); };

		array_resize(&scene->audio_group, files.count);

		for (auto &f : files) {
			auto content = system_read_entire_file(f.path);
			auto group = array_add(&scene->audio_group);
			group->stream = (Audio_Stream *)content.data;
			int length = (int)minimum(f.name.count, sizeof(Resource_Name) - 1);
			memcpy(group->name, f.name.data, length);
			group->name[length] = 0;
		}

		scene->fire = scene_find_audio_stream(scene, "bullet.wav");
		scene->hit = scene_find_audio_stream(scene, "hit.wav");
	}
}

void scene_clean_resources(Scene *scene) {
	auto &textures = scene->texture_group;
	for (auto &t : textures) {
		gfx_destroy_texture2d(t.handle);
	}
	textures.count = 0;

	auto &fixtures = scene->fixture_group;
	for (auto &f : fixtures) {
		for (u32 i = 0; i < f.count; ++i)
			memory_free(f.fixtures[i].handle, scene->pool_allocator);
		memory_free(f.fixtures, scene->pool_allocator);
	}
	scene->fixture_group.count = 0;

	scene->resource_header.count = 0;

	auto &audios = scene->audio_group;
	for (auto &a : audios) {
		memory_free(a.stream);
	}
	scene->audio_group.count = 0;
}

void scene_reload_resource(Scene *scene, Resource_Id id) {
	auto resource = scene_find_resource(scene, id);

	Resource_Header header;
	Texture_Group texture;
	Fixture_Group fixture;
	if (!iscene_load_resource(scene, tprintf("resources/sprites/%zu.sprite", id), &header, &texture, &fixture)) {
		return;
	}

	u32 count = resource.fixture->count;
	for (u32 i = 0; i < count; ++i)
		memory_free(resource.fixture->fixtures[i].handle, scene->pool_allocator);

	auto white_texture = im_white_texture();

	if (resource.texture->handle != white_texture) {
		gfx_destroy_texture2d(resource.texture->handle);
	}

	*resource.header = header;
	*resource.fixture = fixture;
	*resource.texture = texture;

	auto keys = scene->entity_table.keys;
	auto resources = scene->entity_table.resources;
	for (u32 index = 0; index < SCENE_MAX_ENTITY_COUNT; ++index) {
		auto k = keys + index;
		auto r = resources + index;
		if (k->slot == Entity_Hash_Table::Slot::OCCUPIED && r->handle == resource.header->id.handle) {
			auto ent_ref = scene->entity_table.references + index;
			switch (ent_ref->type) {
				case Entity_Type_Character: {
					Character *ent = &scene->by_type.character[ent_ref->index];
					ent->rigid_body->fixture_count = resource.fixture->count;
					ent->rigid_body->fixtures = resource.fixture->fixtures;
					ent->texture.index = resource.index;
				} break;

				case Entity_Type_Obstacle: {
					Obstacle *ent = &scene->by_type.obstacle[ent_ref->index];
					ent->rigid_body->fixture_count = resource.fixture->count;
					ent->rigid_body->fixtures = resource.fixture->fixtures;
					ent->texture.index = resource.index;
				} break;

				case Entity_Type_Bullet: {
					Bullet *ent = &scene->by_type.bullet[ent_ref->index];
					ent->rigid_body->fixture_count = resource.fixture->count;
					ent->rigid_body->fixtures = resource.fixture->fixtures;
				} break;

					invalid_default_case();
			}
		}
	}
}

void scene_clean_entities(Scene *scene) {
	for (auto &character : scene->by_type.character) {
		audio_mixer_remove_audio(&scene->audio_mixer, character.boost);
		audio_mixer_remove_audio(&scene->audio_mixer, character.fall);
		iscene_clean_particle_system(&character.particle_system);
		iscene_destroy_rigid_body(scene, character.rigid_body);
	}
	for (auto &obstable : scene->by_type.obstacle) {
		iscene_destroy_rigid_body(scene, obstable.rigid_body);
	}
	for (auto &bullet : scene->by_type.bullet) {
		audio_mixer_remove_audio(&scene->audio_mixer, bullet.audio);
		iscene_destroy_rigid_body(scene, bullet.rigid_body);
	}
	for (auto &emitter : scene->by_type.emitter) {
		iscene_clean_particle_system(&emitter.particle_system);
	}

	scene->by_type.camera.count = 0;
	scene->by_type.character.count = 0;
	scene->by_type.obstacle.count = 0;
	scene->by_type.bullet.count = 0;
	scene->by_type.emitter.count = 0;

	scene->entity_table.count = 0;
	memset(scene->entity_table.keys, 0, sizeof(sizeof(Entity_Hash_Table::Key) * SCENE_MAX_ENTITY_COUNT));
}

void iscene_serialize_entity(Scene *scene, Entity *entity, Ostream *out) {
	serialize_fmt_text(out, "header", reflect_info<Entity>(), (char *)entity);
	serialize_fmt_text_next(out);

	switch (entity->type) {
		case Entity_Type_Camera: {
			serialize_fmt_text(out, "data", reflect_info<Camera>(), (char *)entity);
		} break;

		case Entity_Type_Character: {
			Resource_Id id = scene_find_entity_resource_id(scene, entity->id);
			serialize_fmt_text(out, "resource", reflect_info<Resource_Id>(), (char *)&id);
			serialize_fmt_text_next(out);
			serialize_fmt_text(out, "data", reflect_info<Character>(), (char *)entity);
			serialize_fmt_text_next(out);
			serialize_fmt_text(out, "body", reflect_info<Rigid_Body>(), (char *)((Character *)entity)->rigid_body);
		} break;

		case Entity_Type_Obstacle: {
			Resource_Id id = scene_find_entity_resource_id(scene, entity->id);
			serialize_fmt_text(out, "resource", reflect_info<Resource_Id>(), (char *)&id);
			serialize_fmt_text_next(out);
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
			Resource_Id id;
			if (deserialize_fmt_text(state, "resource", reflect_info<Resource_Id>(), (char *)&id)) {
				Character *character = iscene_add_character(scene, entity.id, entity.position, id);
				character->rigid_body = iscene_create_new_rigid_body(scene, &entity, nullptr);

				if (deserialize_fmt_text(state, "data", reflect_info<Character>(), (char *)character) &&
					deserialize_fmt_text(state, "body", reflect_info<Rigid_Body>(), (char *)(character->rigid_body))) {
					auto r = scene_find_resource(scene, id);

					character->texture.index = r.index;

					auto body = character->rigid_body;
					body->fixture_count = r.fixture->count;
					body->fixtures = r.fixture->fixtures;
					body->entity_id = entity.id;
					result = character;
				} else {
					scene->by_type.character.count -= 1;
					iscene_destroy_rigid_body(scene, character->rigid_body);
					return false;
				}

				return true;
			} else {
				return false;
			}
		} break;

		case Entity_Type_Obstacle: {
			Resource_Id id;
			if (deserialize_fmt_text(state, "resource", reflect_info<Resource_Id>(), (char *)&id)) {
				Obstacle *obstacle = iscene_add_obstacle(scene, entity.id, entity.position, id);
				obstacle->rigid_body = iscene_create_new_rigid_body(scene, &entity, nullptr);

				if (deserialize_fmt_text(state, "data", reflect_info<Obstacle>(), (char *)obstacle) &&
					deserialize_fmt_text(state, "body", reflect_info<Rigid_Body>(), (char *)(obstacle->rigid_body))) {
					auto r = scene_find_resource(scene, id);

					obstacle->texture.index = r.index;

					auto body = obstacle->rigid_body;
					body->fixture_count = r.fixture->count;
					body->fixtures = r.fixture->fixtures;
					body->entity_id = entity.id;
					result = obstacle;
				} else {
					scene->by_type.obstacle.count -= 1;
					iscene_destroy_rigid_body(scene, obstacle->rigid_body);
					return false;
				}

				return true;
			} else {
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
	//level->resources.count = level->resources.capacity = 0;
	//level->resources.data = nullptr;
	//level->resources.allocator = context.allocator;
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
			iscene_serialize_entity(scene, camera, &out);
			if (system_open_file(tprintf("%s/%zu.ent", level_dir, camera->id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
				system_close_file(&file);
			} else {
				system_log(LOG_ERROR, "Scene", "%s/%zu.ent file could not be opened. Failed to save entity.", level_dir, camera->id);
			}
			ostream_reset(&out);
		}
	}

	#if 0 // no need to save players
	{
		count = scene->by_type.character.count;
		auto character = scene->by_type.character.data;
		for (s64 index = 0; index < count; ++index, ++character) {
			//auto resource = iscene_level_find_resource(level, character->id);
			//assert(resource);
			iscene_serialize_entity(scene, character, &out);
			if (system_open_file(tprintf("%s/%zu.ent", level_dir, character->id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
				system_close_file(&file);
			} else {
				system_log(LOG_ERROR, "Scene", "%s/%zu.ent file could not be opened. Failed to save entity.", level_dir, character->id);
			}
			ostream_reset(&out);
	}
}
	#endif

	{
		count = scene->by_type.obstacle.count;
		auto obstacle = scene->by_type.obstacle.data;
		for (s64 index = 0; index < count; ++index, ++obstacle) {
			//auto resource = iscene_level_find_resource(level, obstacle->id);
			//assert(resource);
			iscene_serialize_entity(scene, obstacle, &out);
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
		iscene_add_new_level(scene, name, &index);
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
	for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
		Rigid_Body *body = &ptr->data;
		hgrid_add_body_to_grid(&scene->hgrid, body);
	}

	scene->loaded_level = index;

	auto player = scene_spawn_player(scene, vec2(0), (Color_Id)random_get_range(0, (u32)Color_Id_COUNT));

	Camera *camera = scene_primary_camera(scene);
	camera->position = player->position;
	camera->target_distance = 0.75f;
	camera->distance = camera->target_distance + 1;
	camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS;

	return true;
}

bool scene_reload_level(Scene *scene) {
	scene->player_id = { 0 };

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
