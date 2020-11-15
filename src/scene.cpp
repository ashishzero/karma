#include "scene.h"
#include <time.h>

#include "modules/core/systems.h"
#include "modules/gfx/renderer.h"
#include "modules/imgui/imgui.h"
#include "modules/imgui/editor.h"

#include ".generated/entity.typeinfo"

const r32 GIZMO_LINE_THICKNESS = 0.1f;
const r32 GIZMO_LINE_HALF_THICKNESS = 0.5f * GIZMO_LINE_THICKNESS;
const r32 GIZMO_LINE_LENGTH = 1.3f;
const r32 GIZMO_POINTER_THICKNESS = 2.0f * GIZMO_LINE_THICKNESS;
const r32 GIZMO_POINTER_HALF_THICKNESS = 0.5f * GIZMO_POINTER_THICKNESS;
const Vec3 GIZMO_SQUARE_COLOR = vec3(1);
const r32 GIZMO_ROTOR_MAX_RADIUS = GIZMO_LINE_LENGTH;
const r32 GIZMO_ROTOR_MIN_RADIUS = GIZMO_ROTOR_MAX_RADIUS - 0.5f * GIZMO_POINTER_THICKNESS;
const Vec3 GIZMO_X_COLOR = vec3(0.8f, 0.1f, 0.1f);
const Vec3 GIZMO_Y_COLOR = vec3(0.1f, 0.8f, 0.1f);
const Vec3 GIZMO_ROTOR_COLOR = vec3(0.1f, 0.1f, 0.9f);
const Vec3 GIZMO_ROTOR_INDICATOR_COLOR = vec3(1, 1, 0.1f);
const r32 GIZMO_POINTER_OFFSET = GIZMO_LINE_LENGTH + GIZMO_LINE_HALF_THICKNESS;

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
}

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

template <typename Shape>
static bool test_shape_vs_point(const Shape &s, const Transform &t, Vec2 point, r32 size = 0) {
	Circle circle = { point, size };

	Transform tc;
	tc.p = vec2(0);
	tc.xform = mat2_identity();

	return gjk(s, circle, t, tc);
}

//
//
//

Scene *scene_create() {
	Scene *scene = new Scene;
	
	circular_linked_list_init(&scene->rigid_bodies, context.allocator);

	scene->world.gravity = 0;

	scene->pool_allocator = context.allocator;

	scene->id_series = random_init(context.id, system_get_counter());

#ifdef SCENE_DEVELOPER_TOOLS
	memset(&scene->debug, 0, sizeof(scene->debug));
	scene->debug.manifold.allocator = TEMPORARY_ALLOCATOR;
#endif

	return scene;
}

void scene_destroy(Scene *scene) {
	// TODO: Distroy other objects as well
	memory_free(scene);
}

u64 iscene_generate_unique_id(Scene *scene) {
	u32 a = random_get(&scene->id_series);
	u32 b = (time(0) & 0xffffffff);
	u64 id = ((u64)a | ((u64)b << 32));
	return id;
}

Entity *scene_find_entity(Scene *scene, Entity_Id id) {
	auto res = array_find(&scene->entity, [](const Entity_Ref &a, Entity_Id id) { return a.id == id; }, id);
	if (res >= 0) {
		Entity_Ref &ref = scene->entity[res];

		switch (ref.type) {
			case Entity_Type_Camera: return &scene->by_type.camera[ref.index];
			case Entity_Type_Character: return &scene->by_type.character[ref.index];
			case Entity_Type_Obstacle: return &scene->by_type.obstacle[ref.index];

			invalid_default_case();
		}

	}

	return nullptr;
}

Resource_Fixture *scene_find_resource_fixture(Scene *scene, Resource_Id id) {
	auto res = array_find(&scene->resource_fixtures, [](const Resource_Fixture &f, Resource_Id id) { return f.id == id; }, id);
	if (res >= 0) return scene->resource_fixtures.data + res;
	return nullptr;
}

Resource_Id scene_create_new_resource_fixture(Scene *scene, Fixture *fixtures, u32 fixture_count) {
	Resource_Fixture *resource = array_add(&scene->resource_fixtures);

	resource->id = iscene_generate_unique_id(scene);
	resource->fixture_count = fixture_count;
	resource->fixtures = new (scene->pool_allocator) Fixture[resource->fixture_count];

	Fixture *dst = resource->fixtures;
	Fixture *src;
	ptrsize size = 0;
	for (u32 index = 0; index < fixture_count; ++index) {
		src = fixtures + index;

		switch (src->shape) {
			case Fixture_Shape_Circle:	size = sizeof(Circle); break;
			case Fixture_Shape_Mm_Rect: size = sizeof(Mm_Rect); break;
			case Fixture_Shape_Capsule: size = sizeof(Capsule); break;
			case Fixture_Shape_Polygon: size = sizeof(Polygon) + sizeof(Vec2) * ((Polygon *)src->handle)->vertex_count; break;

			invalid_default_case();
		}

		dst->shape = src->shape;
		dst->handle = memory_allocate(size, scene->pool_allocator);
		memcpy(dst->handle, src->handle, size);

		dst += 1;
	}

	return resource->id;
}

bool scene_delete_resource_fixture(Scene *scene, Resource_Id id) {
	auto index = array_find(&scene->resource_fixtures, [](const Resource_Fixture &r, Resource_Id id) { return r.id == id; }, id);
	if (index >= 0) {
		auto resource = &scene->resource_fixtures[index];
		for (u32 i = 0; i < resource->fixture_count; ++i)
			memory_free(resource->fixtures[i].handle);
		memory_free(resource->fixtures);
		array_remove(&scene->resource_fixtures, index);
		return true;
	}
	return false;
}

Camera *iscene_add_camera(Scene *scene) {
	Camera *camera = array_add(&scene->by_type.camera);
	camera->type = Entity_Type_Camera;
	return camera;
}

Character *iscene_add_character(Scene *scene) {
	Character *character = array_add(&scene->by_type.character);
	character->type = Entity_Type_Character;
	return character;
}

Obstacle *iscene_add_obstacle(Scene *scene) {
	Obstacle *obstacle = array_add(&scene->by_type.obstacle);
	obstacle->type = Entity_Type_Obstacle;
	return obstacle;
}

Mm_Rect rigid_body_bounding_box(Rigid_Body *body, r32 dt) {
	Mm_Rect most_min_rect;
	most_min_rect.min = vec2( MAX_FLOAT);
	most_min_rect.max = vec2(-MAX_FLOAT);

	const Transform &t = body->transform;

	for (u32 index = 0; index < body->fixture_count; ++index) {
		Fixture *fixture = rigid_body_get_fixture(body, index);
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

Rigid_Body *iscene_create_rigid_body(Scene *scene, Entity_Id entity_id, const Rigid_Body_Info *info) {
	auto node = circular_linked_list_add(&scene->rigid_bodies);
	Rigid_Body *rigid_body = &node->data;
	rigid_body->type = info->type;
	rigid_body->flags = 0;
	rigid_body->drag = 5;
	rigid_body->imass = (info->type == Rigid_Body_Type_Dynamic) ? 1.0f : 0.0f;
	rigid_body->velocity = vec2(0);
	rigid_body->force = vec2(0);
	rigid_body->transform = info->transform;
	rigid_body->restitution = 0;
	rigid_body->entity_id = entity_id;

	if (info->fixture) {
		Resource_Fixture *resource  = scene_find_resource_fixture(scene, info->fixture_id);
		rigid_body->fixtures		= resource->fixtures;
		rigid_body->fixture_count	= resource->fixture_count;
	} else {
		rigid_body->fixture_count	= 0;
		rigid_body->fixtures		= 0;
	}

	rigid_body->bounding_box = rigid_body_bounding_box(rigid_body, 0);

	return rigid_body;
}

void iscene_destroy_rigid_body(Scene *scene, Rigid_Body *rigid_body) {
	auto node = circular_linked_list_node_from_data(rigid_body);
	circular_linked_list_remove(&scene->rigid_bodies, node);
}

Entity *scene_create_new_entity(Scene *scene, Entity_Type type, const Entity_Info &info) {
	Entity *entity = nullptr;

	Entity_Id id = iscene_generate_unique_id(scene);
	u32 index;

	switch (type) {
		case Entity_Type_Camera: {
			index = (u32)scene->by_type.camera.count;
			auto camera = (Camera *)iscene_add_camera(scene);
			auto camera_info = (Camera_Info *)info.data;
			camera->distance = camera_info->distance;
			camera->target_position = camera_info->target_position;
			camera->target_distance = camera_info->target_distance;
			camera->follow_factor = camera_info->follow_factor;
			camera->zoom_factor = camera_info->zoom_factor;
			camera->behaviour = camera_info->behaviour;
			camera->lens = camera_info->lens;
			entity = camera;
		} break;

		case Entity_Type_Character: {
			index = (u32)scene->by_type.character.count;
			auto player = (Character *)iscene_add_character(scene);
			player->radius = 1;
			player->color = vec4(1);
			player->rigid_body = iscene_create_rigid_body(scene, id, (Rigid_Body_Info *)info.data);
			entity = player;
		} break;

		case Entity_Type_Obstacle: {
			index = (u32)scene->by_type.obstacle.count;
			auto obstacle = (Obstacle *)iscene_add_obstacle(scene);
			obstacle->color = vec4(1);
			obstacle->rigid_body = iscene_create_rigid_body(scene, id, (Rigid_Body_Info *)info.data);
			entity = obstacle;
		} break;
	}

	entity->type = type;
	entity->id = id;
	entity->position = info.position;

	auto ref = array_add(&scene->entity);
	ref->id = entity->id;
	ref->type = entity->type;
	ref->index = index;

	return entity;
}

//
//
//

bool scene_handle_event(Scene *scene, const Event &event) {

#ifdef SCENE_DEVELOPER_TOOLS
	auto &gizmo = scene->debug.editor.gizmo;
	if ((event.type & Event_Type_KEY_UP)) {
		switch (event.key.symbol) {
			case Key_T: {
				gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
				gizmo.type = Gizmo_Type_NONE;
				return true;
			} break;
			case Key_S: {
				gizmo.render_type = Gizmo_Render_Type_SCALE;
				gizmo.type = Gizmo_Type_NONE;
				return true;
			} break;
			case Key_R: {
				gizmo.render_type = Gizmo_Render_Type_ROTATE;
				gizmo.type = Gizmo_Type_NONE;
				return true;
			} break;
		}
	}
#endif

	return false;
}

void scene_pre_simulate(Scene *scene) {
#ifdef SCENE_DEVELOPER_TOOLS
	scene->debug.manifold.capacity = scene->debug.manifold.count = 0;
	scene->debug.manifold.data = nullptr;
#endif
}

void scene_simulate(Scene *scene, r32 dt) {
	Contact_Manifold manifold;
	manifold.penetration = 0;

	for (auto &player : scene->by_type.character) {
		player.color = vec4(1);
		player.rigid_body->transform.p = player.position;
	}

	for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
		if (ptr->data.type == Rigid_Body_Type_Dynamic) {
			ptr->data.velocity += dt * ptr->data.force + vec2(0, -scene->world.gravity);
			ptr->data.velocity *= powf(0.5f, ptr->data.drag * dt);
			ptr->data.transform.p += dt * ptr->data.velocity;
			ptr->data.bounding_box = rigid_body_bounding_box(&ptr->data, 0);
		}
		clear_bit(ptr->data.flags, Rigid_Body_COLLIDING);
		clear_bit(ptr->data.flags, Rigid_Body_BOUNDING_BOX_COLLIDING);
	}

	for (auto a_ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, a_ptr); a_ptr = iter_next<Rigid_Body>(a_ptr)) {
		for (auto b_ptr = a_ptr->next; iter_continue(&scene->rigid_bodies, b_ptr); b_ptr = iter_next<Rigid_Body>(b_ptr)) {

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
						Fixture *a_fixture = rigid_body_get_fixture(&a, a_index);
						Fixture *b_fixture = rigid_body_get_fixture(&b, b_index);

						if (fixture_vs_fixture(a_fixture, b_fixture, a.transform, b.transform, &manifold)) {
							set_bit(a.flags, Rigid_Body_COLLIDING);
							set_bit(b.flags, Rigid_Body_COLLIDING);

							// Resolve velocity
							r32 restitution = minimum(a.restitution, b.restitution);
							r32 j = -(1 + restitution) * vec2_dot(b.velocity - a.velocity, manifold.normal);
							j /= (a.imass + b.imass);
							j = maximum(0, j);

							a.velocity -= j * a.imass * manifold.normal;
							b.velocity += j * b.imass * manifold.normal;

							// Fix position
							Vec2 correction = maximum(manifold.penetration, 0.0f) / (a.imass + b.imass) * manifold.normal;
							a.transform.p -= a.imass * correction;
							b.transform.p += b.imass * correction;
							a.bounding_box = rigid_body_bounding_box(&a, 0);
							b.bounding_box = rigid_body_bounding_box(&b, 0);

							#ifdef SCENE_DEVELOPER_TOOLS
							array_add(&scene->debug.manifold, manifold);
							#endif
						}
					}
				}
			}

		}
	}

	for (auto &player : scene->by_type.character) {
		player.position = player.rigid_body->transform.p;
	}

	for (auto &camera : scene->by_type.camera) {
		if (camera.behaviour & Camera_Behaviour_ANIMATE_MOVEMENT) {
			camera.position = lerp(camera.position, camera.target_position, 1.0f - powf(1.0f - camera.follow_factor, dt));
		}
		if (camera.behaviour & Camera_Behaviour_ANIMATE_FOCUS) {
			camera.distance = lerp(camera.distance, camera.target_distance, 1.0f - powf(1.0f - camera.zoom_factor, dt));
		}
	}
}

void scene_update(Scene *scene, r32 window_w, r32 window_h) {
	for (auto &player : scene->by_type.character) {
		player.rigid_body->force = vec2(0);
	}

#ifdef SCENE_DEVELOPER_TOOLS
	if (!ImGui_IsUsingCursor()) {
		Camera &camera = scene->by_type.camera[0];
		Scene_Editor &editor = scene->debug.editor;
		Gizmo &gizmo = editor.gizmo;

		r32 view_height = camera.lens.field_of_view;
		r32 view_width = (window_w / window_h) * view_height;

		r32 scale = powf(0.5f, camera.distance);
		r32 iscale = 1.0f / scale;

		Vec2 cursor = ImGui::GetIO().MousePos;
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor.y = 1.0f - cursor.y;
		cursor = 2.0f * cursor - vec2(1);

		cursor.x *= iscale * view_width;
		cursor.y *= iscale * view_height;
		cursor += camera.position;

		Vec2 delta = ImGui::GetIO().MouseDelta;
		delta.x /= window_w;
		delta.y /= (-window_h);

		delta.x *= 2 * iscale * view_width;
		delta.y *= 2 * iscale * view_height;

		if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Right]) {
			clear_bit(camera.behaviour, Camera_Behaviour_ANIMATE_MOVEMENT);
			camera.target_position = camera.position;
			camera.position -= delta;
		}

		if (ImGui::GetIO().MouseWheel) {
			clear_bit(camera.behaviour, Camera_Behaviour_ANIMATE_FOCUS);
			camera.target_distance = camera.distance;
			camera.distance -= ImGui::GetIO().DeltaTime * ImGui::GetIO().MouseWheel * 6;
		}

		editor.hovered_body = nullptr;
		for (auto ptr = iter_begin(&scene->rigid_bodies);
			iter_continue(&scene->rigid_bodies, ptr) && !editor.hovered_body;
			ptr = iter_next<Rigid_Body>(ptr)) {
			auto &body = ptr->data;
			for (u32 index = 0; index < body.fixture_count && !editor.hovered_body; ++index) {
				Fixture *fixture = rigid_body_get_fixture(&body, index);
				if (test_fixture_point(*fixture, body.transform, cursor)) {
					editor.hovered_body = &body;
				}
			}
		}

		bool camera_focus_on_body = false;

		if (editor.selected_body) {
			for (auto &inten : gizmo.intensity) {
				inten = 1;
			}

			if (!ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
				gizmo.type = Gizmo_Type_NONE;
			}

			switch (gizmo.type) {
				case Gizmo_Type_NONE: {
					Transform gizmo_transform;
					gizmo_transform.p = editor.selected_body->transform.p;
					gizmo_transform.xform = mat2_scalar(iscale, iscale);

					switch (gizmo.render_type) {
						case Gizmo_Render_Type_TRANSLATE: {
							const Mm_Rect GIZMO_CENTER_RECT = mm_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(-GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_THICKNESS));

							const Mm_Rect GIZMO_X_LINE_RECT = mm_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS),
								vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_LENGTH, GIZMO_LINE_THICKNESS));
							const Mm_Rect GIZMO_Y_LINE_RECT = mm_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS),
								vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_THICKNESS, GIZMO_LINE_LENGTH));

							const Polygon GIZMO_X_POINTER_POLY = { 3, {
								vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
								vec2(GIZMO_POINTER_OFFSET, GIZMO_POINTER_HALF_THICKNESS),
								vec2(GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS, 0)
								}
							};
							const Polygon GIZMO_Y_POINTER_POLY = { 3, {
								vec2(GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
								vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
								vec2(0, GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS)
							} };

							if (test_shape_vs_point(GIZMO_CENTER_RECT, gizmo_transform, cursor)) {
								gizmo.intensity[0] = 2;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left])
									gizmo.type = Gizmo_Type_CENTER;
							}
							else if (test_shape_vs_point(GIZMO_X_LINE_RECT, gizmo_transform, cursor) ||
								test_shape_vs_point(GIZMO_X_POINTER_POLY, gizmo_transform, cursor)) {
								gizmo.intensity[1] = 4.5f;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left])
									gizmo.type = Gizmo_Type_TRANSLATE_X;
							}
							else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, gizmo_transform, cursor) ||
								test_shape_vs_point(GIZMO_Y_POINTER_POLY, gizmo_transform, cursor)) {
								gizmo.intensity[2] = 2;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left])
									gizmo.type = Gizmo_Type_TRANSLATE_Y;
							}
						} break;

						case Gizmo_Render_Type_SCALE: {
							const Mm_Rect GIZMO_CENTER_RECT = mm_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(-GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_THICKNESS));

							const Mm_Rect GIZMO_X_LINE_RECT = mm_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS),
								vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_LENGTH, GIZMO_LINE_THICKNESS));
							const Mm_Rect GIZMO_Y_LINE_RECT = mm_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS),
								vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_THICKNESS, GIZMO_LINE_LENGTH));

							const Mm_Rect GIZMO_X_POINTER_RECT = mm_rect(vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
								vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS) + vec2(GIZMO_POINTER_THICKNESS));
							const Mm_Rect GIZMO_Y_POINTER_RECT = mm_rect(vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
								vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET) + vec2(GIZMO_POINTER_THICKNESS));

							if (test_shape_vs_point(GIZMO_CENTER_RECT, gizmo_transform, cursor)) {
								gizmo.intensity[0] = 2;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left])
									gizmo.type = Gizmo_Type_CENTER;
							}
							else if (test_shape_vs_point(GIZMO_X_LINE_RECT, gizmo_transform, cursor) ||
								test_shape_vs_point(GIZMO_X_POINTER_RECT, gizmo_transform, cursor)) {
								gizmo.intensity[1] = 4.5f;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
									gizmo.type = Gizmo_Type_SCALE_X;
									gizmo.values[0] = 1;
									gizmo.values[1] = 1;
								}
							}
							else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, gizmo_transform, cursor) ||
								test_shape_vs_point(GIZMO_Y_POINTER_RECT, gizmo_transform, cursor)) {
								gizmo.intensity[2] = 2;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
									gizmo.type = Gizmo_Type_SCALE_Y;
									gizmo.values[0] = 1;
									gizmo.values[1] = 1;
								}
							}
						} break;

						case Gizmo_Render_Type_ROTATE: {
							const Circle GIZMO_OUTER = { vec2(0), GIZMO_ROTOR_MAX_RADIUS };
							const Circle GIZMO_INNER = { vec2(0), GIZMO_ROTOR_MIN_RADIUS };

							if (test_shape_vs_point(GIZMO_OUTER, gizmo_transform, cursor) &&
								!test_shape_vs_point(GIZMO_INNER, gizmo_transform, cursor)) {
								gizmo.intensity[3] = 2;
								if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
									gizmo.type = Gizmo_Type_ROTOR;
									gizmo.values[0] = cursor.x - editor.selected_body->transform.p.x;
									gizmo.values[1] = cursor.y - editor.selected_body->transform.p.y;
								}
							}
						} break;
					}
				} break;

				case Gizmo_Type_CENTER: {
					gizmo.intensity[0] = 2;

					Entity *entity = scene_find_entity(scene, editor.selected_body->entity_id);
					entity->position += delta;
					editor.selected_body->transform.p = entity->position;
					editor.selected_body->bounding_box = rigid_body_bounding_box(editor.selected_body, 0);
				} break;

				case Gizmo_Type_TRANSLATE_X: {
					gizmo.intensity[1] = 4.5f;
					camera.behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT;
					clear_bit(camera.behaviour, Camera_Behaviour_ANIMATE_FOCUS);
					camera_focus_on_body = true;

					Entity *entity = scene_find_entity(scene, editor.selected_body->entity_id);
					entity->position.x += delta.x;
					editor.selected_body->transform.p = entity->position;
					editor.selected_body->bounding_box = rigid_body_bounding_box(editor.selected_body, 0);
				} break;

				case Gizmo_Type_TRANSLATE_Y: {
					gizmo.intensity[2] = 2;
					camera.behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT;
					clear_bit(camera.behaviour, Camera_Behaviour_ANIMATE_FOCUS);
					camera_focus_on_body = true;

					Entity *entity = scene_find_entity(scene, editor.selected_body->entity_id);
					entity->position.y += delta.y;
					editor.selected_body->transform.p = entity->position;
					editor.selected_body->bounding_box = rigid_body_bounding_box(editor.selected_body, 0);
				} break;

				case Gizmo_Type_SCALE_X: {
					gizmo.intensity[1] = 4.5f;
					camera.behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT | Camera_Behaviour_ANIMATE_FOCUS;
					camera_focus_on_body = true;

					r32 scale_amount_x, scale_amount_y;
					scale_amount_x = powf(2.0f, delta.x);
					scale_amount_y = ImGui::GetIO().KeyShift ? scale_amount_x : 1;
					gizmo.values[0] *= scale_amount_x;
					gizmo.values[1] *= scale_amount_y;
					editor.selected_body->transform.xform = mat2_scalar(scale_amount_x, scale_amount_y) * editor.selected_body->transform.xform;
					editor.selected_body->bounding_box = rigid_body_bounding_box(editor.selected_body, 0);
				} break;

				case Gizmo_Type_SCALE_Y: {
					gizmo.intensity[2] = 2;
					camera.behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT | Camera_Behaviour_ANIMATE_FOCUS;
					camera_focus_on_body = true;

					r32 scale_amount_x, scale_amount_y;
					scale_amount_y = powf(2.0f, delta.y);
					scale_amount_x = ImGui::GetIO().KeyShift ? scale_amount_y : 1;
					gizmo.values[0] *= scale_amount_x;
					gizmo.values[1] *= scale_amount_y;
					editor.selected_body->transform.xform = mat2_scalar(scale_amount_x, scale_amount_y) * editor.selected_body->transform.xform;
					editor.selected_body->bounding_box = rigid_body_bounding_box(editor.selected_body, 0);
				} break;

				case Gizmo_Type_ROTOR: {
					gizmo.intensity[3] = 2;
					camera.behaviour |= Camera_Behaviour_ANIMATE_MOVEMENT | Camera_Behaviour_ANIMATE_FOCUS;
					camera_focus_on_body = true;

					Vec2 a = vec2_normalize_check(vec2(gizmo.values[0], gizmo.values[1]));
					Vec2 b = vec2_normalize_check(cursor - editor.selected_body->transform.p);
					r32 angle = vec2_signed_angle_between(a, b);

					gizmo.values[0] = b.x;
					gizmo.values[1] = b.y;

					editor.selected_body->transform.xform = mat2_rotation(angle) * editor.selected_body->transform.xform;
					editor.selected_body->bounding_box = rigid_body_bounding_box(editor.selected_body, 0);
				} break;
			}

		}

		if (gizmo.type == Gizmo_Type_NONE) {
			if (ImGui::GetIO().MouseClicked[ImGuiMouseButton_Left]) {
				editor.selected_body = editor.hovered_body;
				if (gizmo.render_type == Gizmo_Render_Type_NONE)
					gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
			}

			if (editor.hovered_body && ImGui::GetIO().MouseDoubleClicked[ImGuiMouseButton_Left]) {
				editor.selected_body = editor.hovered_body;
				if (gizmo.render_type == Gizmo_Render_Type_NONE)
					gizmo.render_type = Gizmo_Render_Type_TRANSLATE;

				camera.behaviour |= Camera_Behaviour_ANIMATE_FOCUS | Camera_Behaviour_ANIMATE_MOVEMENT;
				camera_focus_on_body = true;
			}
		}

		if (camera_focus_on_body) {
			if (camera.behaviour & Camera_Behaviour_ANIMATE_MOVEMENT) {
				camera.target_position = editor.selected_body->transform.p;
			}

			if (camera.behaviour & Camera_Behaviour_ANIMATE_FOCUS) {
				r32 sx = editor.selected_body->bounding_box.max.x - editor.selected_body->bounding_box.min.x;
				r32 sy = editor.selected_body->bounding_box.max.y - editor.selected_body->bounding_box.min.y;
				sx /= view_width;
				sy /= view_height;
				r32 s = maximum(sx, sy);

				r32 new_distance = log2f(s);
				camera.target_distance = maximum(new_distance, camera.target_distance);
			}
		}

		if (editor.selected_body == nullptr) {
			gizmo.render_type = Gizmo_Render_Type_NONE;
		}
	}
#endif
}

static void iscene_render_shape(Fixture &fixture, const Transform &transform, Vec3 color) {
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
			im2d_circle(circle->center, circle->radius, shade);
			im2d_circle_outline(circle->center, circle->radius, outline);
		} break;

		case Fixture_Shape_Mm_Rect: {
			auto mm_rect = fixture_get_shape(&fixture, Mm_Rect);
			auto dim = mm_rect->max - mm_rect->min;
			im2d_rect(mm_rect->min, dim, shade);
			im2d_rect_outline(mm_rect->min, dim, outline);
		} break;

		case Fixture_Shape_Polygon: {
			auto polygon = fixture_get_shape(&fixture, Polygon);
			im2d_polygon(*polygon, shade);
			im2d_polygon_outline(*polygon, outline);
		} break;

		case Fixture_Shape_Capsule: {
			auto capsule = fixture_get_shape(&fixture, Capsule);
			Vec2 capsule_dir = capsule->b - capsule->a;
			Vec2 capsule_norm = vec2_normalize(vec2(-capsule_dir.y, capsule_dir.x)) * capsule->radius;

			Vec2 a, b, c, d;
			a = capsule->a - capsule_norm;
			b = capsule->a + capsule_norm;
			c = capsule->b + capsule_norm;
			d = capsule->b - capsule_norm;

			r32 theta_a = atan2f(capsule_norm.y, capsule_norm.x) + MATH_PI;
			r32 theta_b = theta_a + MATH_PI;
			while (theta_b > 2 * MATH_PI) theta_b -= 2 * MATH_PI;

			im2d_pie(capsule->a, capsule->radius, theta_b, theta_a, shade);
			im2d_pie(capsule->b, capsule->radius, theta_a, theta_b, shade);
			im2d_quad(a, b, c, d, shade);

			im2d_arc_outline(capsule->a, capsule->radius, theta_b, theta_a, outline);
			im2d_arc_outline(capsule->b, capsule->radius, theta_a, theta_b, outline);
			im2d_line(a, d, outline);
			im2d_line(b, c, outline);
		} break;

			invalid_default_case();
	}

	im2d_pop_matrix();
}

void iscene_editor_fixture_group(Scene_Editor *editor, Rigid_Body *body) {
	if (ImGui::CollapsingHeader("Fixtures")) {
		ImGui::Indent();
		ImGui::LabelText("id", "%016zx", (u64)body->fixtures);

		if (ImGui::CollapsingHeader("Shapes")) {
			ImGui::Indent();

			auto count = body->fixture_count;
			auto fixtures = body->fixtures;
			auto strings = enum_string_array<Fixture_Shape>();

			for (u32 index = 0; index < count; ++index) {
				auto f = fixtures + index;
				ImGui::BulletText(strings[f->shape]);
			}

			ImGui::Unindent();
		}

		if (ImGui::Button("Edit##Fixture", ImVec2(ImGui::CalcItemWidth(), 0))) {
			editor->open_fixture = true;
		}

		ImGui::Unindent();
	}
}

template <typename Shape>
bool editor_fixture_shape(Shape &s, const char *name) {
	bool result = false;
	ImGui::PushID((void *)&s);
	if (ImGui::CollapsingHeader(name)) {
		ImGui::Indent();
		result |= editor_widget<Shape>(s, name);
		ImGui::Unindent();
	}
	ImGui::PopID();
	return result;
}

bool editor_fixture(Fixture *fixture, u32 count) {
	bool result = false;

	for (u32 index = 0; index < count; ++index) {
		auto f = fixture + index;

		switch (f->shape) {
			case Fixture_Shape_Circle: {
				auto shape = fixture_get_shape(f, Circle);
				result |= editor_fixture_shape(*shape, "Circle");
			} break;

			case Fixture_Shape_Mm_Rect: {
				auto shape = fixture_get_shape(f, Mm_Rect);
				result |= editor_fixture_shape(*shape, "Mm_Rect");
			} break;

			case Fixture_Shape_Capsule: {
				auto shape = fixture_get_shape(f, Capsule);
				result |= editor_fixture_shape(*shape, "Capsule");
			} break;

			case Fixture_Shape_Polygon: {
				auto shape = fixture_get_shape(f, Polygon);
				result |= editor_fixture_shape(*shape, "Polygon");
			} break;

				invalid_default_case();
		}
	}

	return result;
}

bool iscene_editor_entity(Scene_Editor *editor, Entity *entity) {
	if (entity == nullptr) {
		return false;
	}

	bool result = false;
	Rigid_Body *body = nullptr;

	ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	switch (entity->type) {
		case Entity_Type_Null: {
			result = editor_widget<Entity>(*entity, "Null Entity");
		} break;

		case Entity_Type_Camera: {
			result = editor_widget<Camera>(*(Camera *)entity, "Camera Entity");
		} break;

		case Entity_Type_Character: {
			Character *c = (Character *)entity;
			result = editor_widget<Character>(*c, "Character Entity");
			body = c->rigid_body;
		} break;

		case Entity_Type_Obstacle: {
			Obstacle *o = (Obstacle *)entity;
			result = editor_widget<Obstacle>(*o, "Obstacle Entity");
			if (result) {
				o->rigid_body->transform.p = entity->position;
				o->rigid_body->bounding_box = rigid_body_bounding_box(o->rigid_body, 0);
			}

			body = o->rigid_body;
		} break;

			invalid_default_case();
	}

	if (body) {
		iscene_editor_fixture_group(editor, body);
	}
	else {
		editor->open_fixture = false;
	}

	ImGui::End();

	if (editor->open_fixture) {
		if (ImGui::Begin("Fixture Editor", &editor->open_fixture)) {
			if (editor_fixture(body->fixtures, body->fixture_count)) {
				body->bounding_box = rigid_body_bounding_box(body, 0);
			}
		}
		ImGui::End();
	}

	return result;
}

void scene_render(Scene *scene, r32 alpha, r32 aspect_ratio, Scene_Render_Flags flags) {
	Camera &camera = scene->by_type.camera[0];

	r32 view_height = camera.lens.field_of_view;
	r32 view_width = aspect_ratio * view_height;

	Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height, camera.lens.near, camera.lens.far);

	im2d_set_stroke_weight(0.02f);

	r32 scale = powf(0.5f, camera.distance);
	r32 iscale = 1.0f / scale;

	Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera.position, 0.0f));

	im2d_begin(view, transform);

	if (flags & Scene_Render_WORLD) {
		for (auto &player : scene->by_type.character) {
			im2d_circle(player.position, player.radius, player.color);
			im2d_line(player.position, player.position + player.rigid_body->velocity, vec4(0, 1.5f, 0));
		}
	}

	if (flags & Scene_Render_FIXTURE) {
		#ifdef SCENE_DEVELOPER_TOOLS
		Rigid_Body *body_hovered = scene->debug.editor.hovered_body;
		#endif

		for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
			auto &body = ptr->data;

			Vec4 color = (body.flags & Rigid_Body_COLLIDING) ? vec4(1, 0, 0) : vec4(0.7f, 0.45f, 0);

			#ifdef SCENE_DEVELOPER_TOOLS
			if (&body == body_hovered) {
				color.xyz = vec3(1) - color.xyz;
			}
			#endif

			for (u32 index = 0; index < body.fixture_count; ++index) {
				auto f = rigid_body_get_fixture(&body, index);
				iscene_render_shape(*f, body.transform, color.xyz);
			}

			color = (body.flags & Rigid_Body_BOUNDING_BOX_COLLIDING) ? vec4(0.7f, 0.1f, 0.1f, 1) : vec4(0.1f, 0.7f, 0.1f, 1);

			im2d_rect_outline(body.bounding_box.min, body.bounding_box.max - body.bounding_box.min, color);
		}
	}

	#ifdef SCENE_DEVELOPER_TOOLS
	if (flags & Scene_Render_COLLISION) {
		auto manifolds = scene->debug.manifold;
		for (auto &m : manifolds) {
			im2d_line(m.contacts[1], m.contacts[1] + m.penetration * m.normal, vec4(1, 0, 1), 0.02f);

			im2d_circle(m.contacts[0], 0.08f, vec4(1, 0, 1));
			im2d_circle(m.contacts[1], 0.08f, vec4(1, 0, 1));
		}
	}
	#endif

	im2d_end();

	#ifdef SCENE_DEVELOPER_TOOLS
	if (flags & Scene_Render_EDITOR) {
		auto &gizmo = scene->debug.editor.gizmo;
		auto body_selected = scene->debug.editor.selected_body;

		if (gizmo.render_type != Gizmo_Render_Type_NONE) {
			transform = transform * mat4_translation(vec3(body_selected->transform.p, 0)) * mat4_scalar(iscale, iscale, 1);
			im2d_begin(view, transform);

			switch (gizmo.render_type) {
				case Gizmo_Render_Type_TRANSLATE: {
					// square
					im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[0] * GIZMO_SQUARE_COLOR, 1));

					// x
					im2d_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_LENGTH, GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

					// y
					im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS, GIZMO_LINE_LENGTH), vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

					// x pointer arrow
					im2d_triangle(vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
						vec2(GIZMO_POINTER_OFFSET, GIZMO_POINTER_HALF_THICKNESS),
						vec2(GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS, 0),
						vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

					// y pointer arrow
					im2d_triangle(vec2(GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
						vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
						vec2(0, GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS),
						vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

				} break;

				case Gizmo_Render_Type_SCALE: {
					r32 offset_x = GIZMO_LINE_LENGTH, offset_y = GIZMO_LINE_LENGTH;

					if (gizmo.type == Gizmo_Type_SCALE_X || gizmo.type == Gizmo_Type_SCALE_Y) {
						offset_x *= gizmo.values[0];
						offset_y *= gizmo.values[1];
					}

					// square
					im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[0] * GIZMO_SQUARE_COLOR, 1));

					// x
					im2d_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS), vec2(offset_x, GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

					// y
					im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS, offset_y), vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

					// x pointer box
					im2d_rect(vec2(offset_x - GIZMO_LINE_LENGTH + GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS), vec2(GIZMO_POINTER_THICKNESS), vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

					// y pointer box
					im2d_rect(vec2(-GIZMO_POINTER_HALF_THICKNESS, offset_y - GIZMO_LINE_LENGTH + GIZMO_POINTER_OFFSET), vec2(GIZMO_POINTER_THICKNESS), vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

				} break;

				case Gizmo_Render_Type_ROTATE: {
					const auto &t = body_selected->transform.xform;
					r32 _00 = t.m2[0][0];
					r32 _01 = t.m2[0][1];
					r32 _10 = t.m2[1][0];
					r32 _11 = t.m2[1][1];

					r32 scale_x = sqrtf(_00 * _00 + _10 * _10);
					r32 scale_y = sqrtf(_01 * _01 + _11 * _11);

					r32 c = _00 / scale_x;
					r32 s = _10 / scale_x;

					r32 angle_a = atanf(s / c) + ((c < 0) ? MATH_PI : 0);
					while (angle_a < 0) angle_a += 2 * MATH_PI;
					r32 angle_b = angle_a + 0.6f * MATH_TAU;
					while (angle_b > 2 * MATH_PI) angle_b -= 2 * MATH_PI;

					r32 intensity = gizmo.intensity[3];
					im2d_pie_part(vec2(0), GIZMO_ROTOR_MIN_RADIUS, GIZMO_ROTOR_MAX_RADIUS, 0, 2 * MATH_PI, vec4(GIZMO_ROTOR_COLOR, 1));
					im2d_pie_part(vec2(0), GIZMO_ROTOR_MIN_RADIUS, GIZMO_ROTOR_MAX_RADIUS, angle_a, angle_b, vec4(intensity * GIZMO_ROTOR_INDICATOR_COLOR, 1));
				} break;
			}

			im2d_end();
		}
	}

	Entity *entity_selected = nullptr;
	Rigid_Body *body_selected = scene->debug.editor.selected_body;

	if (!body_selected) {
		entity_selected = &scene->by_type.camera[0];
	} else {
		entity_selected = scene_find_entity(scene, body_selected->entity_id);
	}

	iscene_editor_entity(&scene->debug.editor, entity_selected);

	#endif
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
