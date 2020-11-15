#include "scene.h"
#include <time.h>

#include "modules/core/systems.h"
#include "modules/gfx/renderer.h"
#include "modules/imgui/imgui.h"
#include "modules/imgui/editor.h"

#include ".generated/entity.typeinfo"

//
// Gizmo Values
//

const r32  EDITOR_VERTEX_RADIUS = 0.1f;
const r32  EDITOR_VERTEX_SELECTOR_RADIUS = 0.3f;
const r32  EDITOR_VERTEX_SELECTOR_THICKNESS = 0.1f;
const Vec4 EDITOR_VERTEX_COLOR = vec4(1, 1, 0);
const Vec4 EDITOR_SELECTED_VERTEX_COLOR = vec4(0, 3, 3);

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

const Mm_Rect GIZMO_X_POINTER_RECT = mm_rect(vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
											 vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS) + vec2(GIZMO_POINTER_THICKNESS));
const Mm_Rect GIZMO_Y_POINTER_RECT = mm_rect(vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
											 vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET) + vec2(GIZMO_POINTER_THICKNESS));

const Circle GIZMO_OUTER = { vec2(0), GIZMO_ROTOR_MAX_RADIUS };
const Circle GIZMO_INNER = { vec2(0), GIZMO_ROTOR_MIN_RADIUS };

//
// End Gizmo Values
//


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

template <typename Shape>
static bool test_shape_vs_point(const Shape &s, const Transform &t, Vec2 point, r32 size = 0) {
	Circle circle = { point, size };

	Transform tc;
	tc.p = vec2(0);
	tc.xform = mat2_identity();

	return gjk(s, circle, t, tc);
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

	#ifdef ENABLE_DEVELOPER_OPTIONS
	memset(&scene->debug, 0, sizeof(scene->debug));
	scene->debug.manifold.allocator = TEMPORARY_ALLOCATOR;
	#endif

	Entity_Info info;
	Camera_Info camera_info;
	camera_info.target_position = vec2(0);
	camera_info.target_distance = 0;
	camera_info.distance = .4f;
	camera_info.follow_factor = 0.977f;
	camera_info.zoom_factor = 0.9f;
	camera_info.behaviour = Camera_Behaviour_STILL;
	camera_info.lens.field_of_view = 5.0f;
	camera_info.lens.near = -1;
	camera_info.lens.far = 1;
	info.position = vec2(0);
	info.data = &camera_info;

	scene_create_new_entity(scene, Entity_Type_Camera, info);

	#ifdef ENABLE_DEVELOPER_OPTIONS
	camera_info.behaviour = Camera_Behaviour_STILL;
	camera_info.target_distance = 0;
	camera_info.target_position = vec2(0);

	scene_create_new_entity(scene, Entity_Type_Camera, info);
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

s64 iscene_find_resource_fixture_index_from_pointer(Scene *scene, Fixture *fixture) {
	s64 index = array_find(&scene->resource_fixtures, [](const Resource_Fixture &f, Fixture *ptr) { return f.fixtures == ptr; }, fixture);
	index = maximum(index, 0);
	return index;
}

Resource_Id scene_create_new_resource_fixture(Scene *scene, String name, Fixture *fixtures, u32 fixture_count) {
	Resource_Fixture *resource = array_add(&scene->resource_fixtures);

	assert(name.count < (sizeof(Resource_Name) - 1));

	resource->id = iscene_generate_unique_id(scene);

	memcpy(resource->name, name.data, name.count);
	resource->name[name.count] = 0;

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
	most_min_rect.min = vec2(MAX_FLOAT);
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
		Resource_Fixture *resource = scene_find_resource_fixture(scene, info->fixture_id);
		rigid_body->fixtures = resource->fixtures;
		rigid_body->fixture_count = resource->fixture_count;
	} else {
		rigid_body->fixture_count = 0;
		rigid_body->fixtures = 0;
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

Camera *scene_primary_camera(Scene *scene) {
	return &scene->by_type.camera[0];
}

Camera *scene_editor_camera(Scene *scene, Scene_Editor_Mode mode) {
	if (mode == Scene_Editor_Mode_MAP) {
		return &scene->by_type.camera[0];
	} else if (mode == Scene_Editor_Mode_FIXTURE) {
		return &scene->by_type.camera[1];
	}
	invalid_code_path();
	return &scene->by_type.camera[0];
}

//
//
//

void iscene_set_editor_mode_map(Scene *scene) {
	if (scene->debug.editor.mode == Scene_Editor_Mode_FIXTURE) {
		for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
			ptr->data.bounding_box = rigid_body_bounding_box(&ptr->data, 0);
		}
	}

	scene->debug.editor.mode = Scene_Editor_Mode_MAP;
	scene->debug.editor.map.hovered_body = nullptr;
	scene->debug.editor.map.selected_body = nullptr;
	scene->debug.editor.gizmo.render_type = Gizmo_Render_Type_NONE;
	scene->debug.editor.gizmo.type = Gizmo_Type_NONE;
}

void iscene_set_editor_mode_fixture(Scene *scene, s64 index) {
	scene->debug.editor.mode = Scene_Editor_Mode_FIXTURE;
	scene->debug.editor.fixture.index = index;
	scene->debug.editor.fixture.hovered_vertex_ptr = nullptr;
	scene->debug.editor.fixture.selected_vertex_ptr = nullptr;
	auto camera = scene_editor_camera(scene, Scene_Editor_Mode_FIXTURE);
	camera->position = vec2(0);
	camera->target_position = vec2(0);
	camera->distance = 0;
	camera->target_distance = 0;
	camera->behaviour = 0;
}

bool scene_handle_event(Scene *scene, const Event &event) {
	#ifdef ENABLE_DEVELOPER_OPTIONS
	auto &editor = scene->debug.editor;
	auto &gizmo = scene->debug.editor.gizmo;

	if ((event.type & Event_Type_KEY_UP) && editor.gizmo.render_type != Gizmo_Render_Type_NONE) {
		if (editor.mode == Scene_Editor_Mode_MAP) {
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
		} else if (editor.mode == Scene_Editor_Mode_FIXTURE) {
			switch (event.key.symbol) {
				case Key_T: {
					gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
					gizmo.type = Gizmo_Type_NONE;
					return true;
				} break;
				case Key_S: {
					Fixture &f = scene->resource_fixtures[editor.fixture.index].fixtures[editor.fixture.selected_fixture_index];
					if (f.shape == Fixture_Shape_Circle || f.shape == Fixture_Shape_Capsule) {
						gizmo.render_type = Gizmo_Render_Type_SCALE;
						gizmo.type = Gizmo_Type_NONE;
						return true;
					}
				} break;
			}
		}
	}
	#endif

	return false;
}

void scene_pre_simulate(Scene *scene) {
	#ifdef ENABLE_DEVELOPER_OPTIONS
	scene->debug.manifold.capacity = scene->debug.manifold.count = 0;
	scene->debug.manifold.data = nullptr;
	#endif
}

void scene_simulate(Scene *scene, r32 dt) {
	Contact_Manifold manifold;
	manifold.penetration = 0;

	auto count = scene->by_type.character.count;
	auto &characters = scene->by_type.character;
	for (auto index = 0; index < count; ++index) {
		auto &character = characters[index];
		character.color = vec4(1);
		character.rigid_body->transform.p = character.position;
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

							#ifdef ENABLE_DEVELOPER_OPTIONS
							array_add(&scene->debug.manifold, manifold);
							#endif
						}
					}
				}
			}

		}
	}

	for (auto index = 0; index < count; ++index) {
		auto &character = characters[index];
		character.position = character.rigid_body->transform.p;
	}

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

void gizmo_action(Gizmo *gizmo, const Transform &gizmo_transform, Vec2 cursor, Vec2 delta) {
	auto &io = ImGui::GetIO();

	for (auto &inten : gizmo->intensity) {
		inten = 1;
	}

	if (!io.MouseDown[ImGuiMouseButton_Left]) {
		gizmo->type = Gizmo_Type_NONE;
	}

	switch (gizmo->type) {
		case Gizmo_Type_NONE: {
			// Check if we have hovered over or used gizmo

			switch (gizmo->render_type) {
				case Gizmo_Render_Type_TRANSLATE: {
					if (test_shape_vs_point(GIZMO_CENTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[0] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_CENTER;
					} else if (test_shape_vs_point(GIZMO_X_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_X_POINTER_POLY, gizmo_transform, cursor)) {
						gizmo->intensity[1] = 4.5f;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_TRANSLATE_X;
					} else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_Y_POINTER_POLY, gizmo_transform, cursor)) {
						gizmo->intensity[2] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_TRANSLATE_Y;
					}

					if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(0);
					}
				} break;

				case Gizmo_Render_Type_SCALE: {
					if (test_shape_vs_point(GIZMO_CENTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[0] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_CENTER;
					} else if (test_shape_vs_point(GIZMO_X_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_X_POINTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[1] = 4.5f;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_SCALE_X;
							gizmo->values[0] = 1;
							gizmo->values[1] = 1;
						}
					} else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_Y_POINTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[2] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_SCALE_Y;
							gizmo->values[0] = 1;
							gizmo->values[1] = 1;
						}
					}

					if (gizmo->type == Gizmo_Type_CENTER) {
						gizmo->out = vec2(0);
					} else if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(1);
					}
				} break;

				case Gizmo_Render_Type_ROTATE: {
					if (test_shape_vs_point(GIZMO_OUTER, gizmo_transform, cursor) &&
						!test_shape_vs_point(GIZMO_INNER, gizmo_transform, cursor)) {
						gizmo->intensity[3] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_ROTOR;
							gizmo->values[0] = cursor.x - gizmo_transform.p.x;
							gizmo->values[1] = cursor.y - gizmo_transform.p.y;
						}
					}

					if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(0);
					}
				} break;
			}
		} break;

		case Gizmo_Type_CENTER: {
			gizmo->intensity[0] = 2;
			gizmo->out = delta;
		} break;

		case Gizmo_Type_TRANSLATE_X: {
			gizmo->intensity[1] = 4.5f;
			gizmo->out.x = delta.x;
			gizmo->out.y = 0;
		} break;

		case Gizmo_Type_TRANSLATE_Y: {
			gizmo->intensity[2] = 2;
			gizmo->out.x = 0;
			gizmo->out.y = delta.y;
		} break;

		case Gizmo_Type_SCALE_X: {
			gizmo->intensity[1] = 4.5f;

			r32 scale_amount_x, scale_amount_y;
			scale_amount_x = powf(2.0f, delta.x);
			scale_amount_y = io.KeyShift ? scale_amount_x : 1;
			gizmo->out.x = scale_amount_x;
			gizmo->out.y = scale_amount_y;
			gizmo->values[0] *= scale_amount_x;
			gizmo->values[1] *= scale_amount_y;
		} break;

		case Gizmo_Type_SCALE_Y: {
			gizmo->intensity[2] = 2;

			r32 scale_amount_x, scale_amount_y;
			scale_amount_y = powf(2.0f, delta.y);
			scale_amount_x = io.KeyShift ? scale_amount_y : 1;
			gizmo->out.x = scale_amount_x;
			gizmo->out.y = scale_amount_y;
			gizmo->values[0] *= scale_amount_x;
			gizmo->values[1] *= scale_amount_y;
		} break;

		case Gizmo_Type_ROTOR: {
			gizmo->intensity[3] = 2;

			Vec2 a = vec2_normalize_check(vec2(gizmo->values[0], gizmo->values[1]));
			Vec2 b = vec2_normalize_check(cursor - gizmo_transform.p);
			r32 angle = vec2_signed_angle_between(a, b);

			gizmo->out.x = angle;
			gizmo->values[0] = b.x;
			gizmo->values[1] = b.y;
		} break;
	}
}

void scene_update(Scene *scene, r32 window_w, r32 window_h) {
	auto count = scene->by_type.character.count;
	auto &characters = scene->by_type.character;
	for (auto index = 0; index < count; ++index) {
		auto &character = characters[index];
		character.rigid_body->force = vec2(0);
	}

	#ifdef ENABLE_DEVELOPER_OPTIONS
	ImGui::GetStyle().Alpha = 1.0f;

	if (!ImGui_IsUsingCursor()) {
		ImGui::GetStyle().Alpha = 0.4f;

		Camera *camera = scene_editor_camera(scene, scene->debug.editor.mode);
		Scene_Editor &editor = scene->debug.editor;
		Gizmo &gizmo = editor.gizmo;
		auto &io = ImGui::GetIO();

		r32 view_height = camera->lens.field_of_view;
		r32 view_width = (window_w / window_h) * view_height;

		r32 scale = powf(0.5f, camera->distance);
		r32 iscale = 1.0f / scale;

		Vec2 cursor = io.MousePos;
		Vec2 delta = io.MouseDelta;
		{
			// Convert cursor and delta value from window space into world space
			cursor.x /= window_w;
			cursor.y /= window_h;
			cursor.y = 1.0f - cursor.y;
			cursor = 2.0f * cursor - vec2(1);

			cursor.x *= iscale * view_width;
			cursor.y *= iscale * view_height;
			cursor += camera->position;

			delta.x /= window_w;
			delta.y /= (-window_h);

			delta.x *= 2 * iscale * view_width;
			delta.y *= 2 * iscale * view_height;
		}

		// Movement of the view of the world when editing
		if (io.MouseDown[ImGuiMouseButton_Right]) {
			clear_bit(camera->behaviour, Camera_Behaviour_ANIMATE_MOVEMENT);
			camera->position -= delta;
		}

		// Zoom in and out of the world when editing
		if (io.MouseWheel) {
			camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS;
			camera->target_distance -= io.DeltaTime * io.MouseWheel * 7;
		}

		switch (editor.mode) {
			case Scene_Editor_Mode_MAP: {
				// Get the Rigid_Body under the mouse cursor, nullptr if mouse doesn't hover over any Rigid_Body
				editor.map.hovered_body = nullptr;
				for (auto ptr = iter_begin(&scene->rigid_bodies);
					 iter_continue(&scene->rigid_bodies, ptr) && !editor.map.hovered_body;
					 ptr = iter_next<Rigid_Body>(ptr)) {
					auto &body = ptr->data;
					for (u32 index = 0; index < body.fixture_count && !editor.map.hovered_body; ++index) {
						Fixture *fixture = rigid_body_get_fixture(&body, index);
						if (test_fixture_point(*fixture, body.transform, cursor)) {
							editor.map.hovered_body = &body;
						}
					}
				}

				if (editor.map.selected_body) {
					Transform gizmo_transform;
					gizmo_transform.p = editor.map.selected_body->transform.p;
					gizmo_transform.xform = mat2_scalar(iscale, iscale);

					gizmo_action(&editor.gizmo, gizmo_transform, cursor, delta);

					switch (gizmo.type) {
						case Gizmo_Type_TRANSLATE_X:
						case Gizmo_Type_TRANSLATE_Y:
						case Gizmo_Type_CENTER: {
							Entity *entity = scene_find_entity(scene, editor.map.selected_body->entity_id);
							entity->position += gizmo.out;
							editor.map.selected_body->transform.p = entity->position;
							editor.map.selected_body->bounding_box = rigid_body_bounding_box(editor.map.selected_body, 0);
						} break;

						case Gizmo_Type_SCALE_X:
						case Gizmo_Type_SCALE_Y: {
							editor.map.selected_body->transform.xform = mat2_scalar(gizmo.out) * editor.map.selected_body->transform.xform;
							editor.map.selected_body->bounding_box = rigid_body_bounding_box(editor.map.selected_body, 0);
						} break;

						case Gizmo_Type_ROTOR: {
							editor.map.selected_body->bounding_box = rigid_body_bounding_box(editor.map.selected_body, 0);
							editor.map.selected_body->transform.xform = mat2_rotation(gizmo.out.x) * editor.map.selected_body->transform.xform;
							editor.map.selected_body->bounding_box = rigid_body_bounding_box(editor.map.selected_body, 0);
						} break;
					}
				}

				if (gizmo.type == Gizmo_Type_NONE) {
					// If we are not using Gizmo and Press left button,
					// then hovered body is selected
					if (io.MouseClicked[ImGuiMouseButton_Left]) {
						editor.map.selected_body = editor.map.hovered_body;
						if (gizmo.render_type == Gizmo_Render_Type_NONE)
							gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
					}

					// If we are not using Gizmo and Double click left button,
					// then hovered body is selected and focused
					if (editor.map.hovered_body && io.MouseDoubleClicked[ImGuiMouseButton_Left]) {
						editor.map.selected_body = editor.map.hovered_body;
						if (gizmo.render_type == Gizmo_Render_Type_NONE)
							gizmo.render_type = Gizmo_Render_Type_TRANSLATE;

						// Focus on Rigid_Body
						camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS | Camera_Behaviour_ANIMATE_MOVEMENT;
						camera->target_position = editor.map.selected_body->transform.p;

						r32 sx = editor.map.selected_body->bounding_box.max.x - editor.map.selected_body->bounding_box.min.x;
						r32 sy = editor.map.selected_body->bounding_box.max.y - editor.map.selected_body->bounding_box.min.y;
						sx /= view_width;
						sy /= view_height;
						r32 s = maximum(sx, sy);

						camera->target_distance = log2f(s);
					}
				}


				if (editor.map.selected_body == nullptr) {
					gizmo.render_type = Gizmo_Render_Type_NONE;
				}
			} break;

			case Scene_Editor_Mode_FIXTURE: {
				if (editor.fixture.index >= 0) {
					Transform p_transform;
					p_transform.p = vec2(0);
					p_transform.xform = mat2_identity();

					Circle p;
					p.radius = EDITOR_VERTEX_RADIUS;

					Resource_Fixture &resource = scene->resource_fixtures[editor.fixture.index];
					u32 count = resource.fixture_count;
					auto &fixtures = resource.fixtures;

					editor.fixture.hovered_vertex_ptr = nullptr;

					editor.fixture.vertex_pointer_angle += io.DeltaTime * 5;
					while (editor.fixture.vertex_pointer_angle >= 2 * MATH_PI) 
						editor.fixture.vertex_pointer_angle -= 2 * MATH_PI;

					for (u32 index = 0; index < count; ++index) {
						auto f = fixtures + index;
						switch (f->shape) {
							case Fixture_Shape_Circle: {
								auto shape = fixture_get_shape(f, Circle);
								p.center = shape->center;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor.fixture.hovered_vertex_ptr = &shape->center;
									editor.fixture.hovered_fixture_index = index;
								}
							} break;

							case Fixture_Shape_Mm_Rect: {
								auto shape = fixture_get_shape(f, Mm_Rect);
								p.center = shape->min;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor.fixture.hovered_vertex_ptr = &shape->min;
									editor.fixture.hovered_fixture_index = index;
								}

								p.center = shape->max;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor.fixture.hovered_vertex_ptr = &shape->max;
									editor.fixture.hovered_fixture_index = index;
								}
							} break;

							case Fixture_Shape_Capsule: {
								auto shape = fixture_get_shape(f, Capsule);
								p.center = shape->a;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor.fixture.hovered_vertex_ptr = &shape->a;
									editor.fixture.hovered_fixture_index = index;
								}

								p.center = shape->b;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor.fixture.hovered_vertex_ptr = &shape->b;
									editor.fixture.hovered_fixture_index = index;
								}
							} break;

							case Fixture_Shape_Polygon: {
								auto shape = fixture_get_shape(f, Polygon);
								u32 vcount = shape->vertex_count;
								auto v = shape->vertices;
								for (u32 vi = 0; vi < vcount; ++vi, ++v) {
									p.center = *v;
									if (test_shape_vs_point(p, p_transform, cursor)) {
										editor.fixture.hovered_vertex_ptr = v;
										editor.fixture.hovered_fixture_index = index;
										break;
									}
								}
							} break;
						}
					}

					if (editor.fixture.selected_vertex_ptr) {
						Transform gizmo_transform;
						gizmo_transform.p = *editor.fixture.selected_vertex_ptr;
						gizmo_transform.xform = mat2_scalar(iscale, iscale);

						gizmo_action(&editor.gizmo, gizmo_transform, cursor, delta);

						switch (gizmo.type) {
							case Gizmo_Type_TRANSLATE_X:
							case Gizmo_Type_TRANSLATE_Y:
							case Gizmo_Type_CENTER: {
								editor.fixture.selected_vertex_ptr->x += gizmo.out.x;
								editor.fixture.selected_vertex_ptr->y += gizmo.out.y;

								// Reject if the polygon is not convex
								Fixture &fixture = fixtures[editor.fixture.selected_fixture_index];
								if (fixture.shape == Fixture_Shape_Polygon) {
									if (!is_polygon_convex(*fixture_get_shape(&fixture, Polygon))) {
										editor.fixture.selected_vertex_ptr->x = gizmo_transform.p.x;
										editor.fixture.selected_vertex_ptr->y = gizmo_transform.p.y;
									}
								}
							} break;

							case Gizmo_Type_SCALE_X:
							case Gizmo_Type_SCALE_Y: {
								Fixture &fixture = fixtures[editor.fixture.selected_fixture_index];
								if (fixture.shape == Fixture_Shape_Circle) {
									auto circle = fixture_get_shape(&fixture, Circle);
									circle->radius *= ((gizmo.type == Gizmo_Type_SCALE_X) ? gizmo.out.x: gizmo.out.y);
								} else if (fixture.shape == Fixture_Shape_Capsule) {
									auto capsule = fixture_get_shape(&fixture, Capsule);
									capsule->radius *= ((gizmo.type == Gizmo_Type_SCALE_X) ? gizmo.out.x : gizmo.out.y);
								} else {
									invalid_code_path();
								}
							} break;

							case Gizmo_Type_ROTOR: {
								invalid_code_path();
							} break;
						}
					}

					if (gizmo.type == Gizmo_Type_NONE) {
						// If we are not using Gizmo and Press left button,
						// then hovered vertex is selected
						if (io.MouseClicked[ImGuiMouseButton_Left]) {
							editor.fixture.selected_vertex_ptr = editor.fixture.hovered_vertex_ptr;
							editor.fixture.selected_fixture_index = editor.fixture.hovered_fixture_index;

							Fixture &fixture = fixtures[editor.fixture.selected_fixture_index];

							if (gizmo.render_type == Gizmo_Render_Type_NONE) {
								gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
							} else if (editor.fixture.selected_vertex_ptr && 
									   (fixture.shape == Fixture_Shape_Mm_Rect || fixture.shape == Fixture_Shape_Polygon)) {
								gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
							}
						}
					}

					if (editor.fixture.selected_vertex_ptr == nullptr) {
						gizmo.render_type = Gizmo_Render_Type_NONE;
					}

				}
			} break;
		}

	}
	#endif
}

static void iscene_render_shape(const Circle &circle, Vec4 shade, Vec4 outline) {
	im2d_circle(circle.center, circle.radius, shade);
	im2d_circle_outline(circle.center, circle.radius, outline);
}

static void iscene_render_shape(const Mm_Rect &mm_rect, Vec4 shade, Vec4 outline) {
	auto dim = mm_rect.max - mm_rect.min;
	im2d_rect(mm_rect.min, dim, shade);
	im2d_rect_outline(mm_rect.min, dim, outline);
}

static void iscene_render_shape(const Polygon &polygon, Vec4 shade, Vec4 outline) {
	im2d_polygon(polygon, shade);
	im2d_polygon_outline(polygon, outline);
}

static void iscene_render_shape(const Capsule &capsule, Vec4 shade, Vec4 outline) {
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
			iscene_render_shape(*circle, shade, outline);
		} break;

		case Fixture_Shape_Mm_Rect: {
			auto mm_rect = fixture_get_shape(&fixture, Mm_Rect);
			iscene_render_shape(*mm_rect, shade, outline);
		} break;

		case Fixture_Shape_Polygon: {
			auto polygon = fixture_get_shape(&fixture, Polygon);
			iscene_render_shape(*polygon, shade, outline);
		} break;

		case Fixture_Shape_Capsule: {
			auto capsule = fixture_get_shape(&fixture, Capsule);
			iscene_render_shape(*capsule, shade, outline);
		} break;

			invalid_default_case();
	}

	im2d_pop_matrix();
}

void iscene_editor_fixture_group(Scene *scene, Scene_Editor *editor, Rigid_Body *body) {
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
			iscene_set_editor_mode_fixture(scene, iscene_find_resource_fixture_index_from_pointer(scene, body->fixtures));
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
				u32 count = shape->vertex_count;
				Vec2 *vertices = shape->vertices;

				ImGui::PushID((void *)(vertices + index));

				char label[5];
				if (ImGui::CollapsingHeader("Vertices")) {
					for (u32 index = 0; index < count; ++index) {
						snprintf(label, sizeof(label), "%u", index);
						Vec2 v = vertices[index];
						if (ImGui::DragFloat2(label, vertices[index].m, 0.01f, 0.0f, 0.0f, "%.4f", 0)) {
							if (!is_polygon_convex(*shape)) {
								vertices[index] = v;
							}
							result = true;
						}
					}
				}

				ImGui::PopID();
			} break;

				invalid_default_case();
		}
	}

	return result;
}

bool iscene_make_editor(Scene *scene) {
	auto &editor = scene->debug.editor;

	bool result = false;

	switch (editor.mode) {
		case Scene_Editor_Mode_MAP: {
			ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

			Entity *entity = nullptr;
			if (editor.map.selected_body) {
				entity = scene_find_entity(scene, editor.map.selected_body->entity_id);
			}

			if (entity == nullptr) {
				ImGui::LabelText("type", "Entity_Type_Null");
				ImGui::End();
				return false;
			}

			Rigid_Body *body = nullptr;

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
				iscene_editor_fixture_group(scene, &editor, body);
			}

			ImGui::End();
		} break;

		case Scene_Editor_Mode_FIXTURE: {
			bool open_fixture = true;

			ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Fixture Editor", &open_fixture)) {
				int selected = (int)editor.fixture.index;
				
				// Left
				{
					ImGui::BeginChild("Fixtures", ImVec2(200, 0), true);
					
					int count = (int)scene->resource_fixtures.count;
					auto &fixtures = scene->resource_fixtures;
					for (int index = 0; index < count; index++) {
						auto &f = fixtures[index];
						ImGui::PushID((void *)&f);
						if (ImGui::Selectable(f.name, selected == index))
							selected = index;
						ImGui::PopID();
					}

					ImGui::EndChild();
				}
				ImGui::SameLine();

				editor.fixture.index = selected;

				// Right
				{
					ImGui::BeginGroup();
					ImGui::BeginChild("Resource View");

					auto &resource = scene->resource_fixtures[selected];
					editor_widget<Resource_Fixture>(resource, "Resource");
					editor_fixture(resource.fixtures, resource.fixture_count);

					ImGui::EndChild();
					ImGui::EndGroup();
				}
			}
			ImGui::End();

			if (!open_fixture) {
				iscene_set_editor_mode_map(scene);
			}
		} break;

			invalid_default_case();
	}

	return result;
}

void gizmo_render(const Gizmo &gizmo, const Mat2 &t) {
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
			r32 _00 = t.m2[0][0];
			r32 _01 = t.m2[0][1];
			r32 _10 = t.m2[1][0];
			r32 _11 = t.m2[1][1];

			r32 scale_x = sqrtf(_00 * _00 + _10 * _10);
			r32 scale_y = sqrtf(_01 * _01 + _11 * _11);

			r32 c = _00 / scale_x;
			r32 s = _10 / scale_x;

			if (c == 0 && s == 0) {
				c = 1;
			}

			r32 angle_a = atanf(s / c) + ((c < 0) ? MATH_PI : 0);
			while (angle_a < 0) angle_a += 2 * MATH_PI;
			r32 angle_b = angle_a + 0.6f * MATH_TAU;
			while (angle_b >= 2 * MATH_PI) angle_b -= 2 * MATH_PI;

			r32 intensity = gizmo.intensity[3];
			im2d_pie_part(vec2(0), GIZMO_ROTOR_MIN_RADIUS, GIZMO_ROTOR_MAX_RADIUS, 0, 2 * MATH_PI, vec4(GIZMO_ROTOR_COLOR, 1));
			im2d_pie_part(vec2(0), GIZMO_ROTOR_MIN_RADIUS, GIZMO_ROTOR_MAX_RADIUS, angle_a, angle_b, vec4(intensity * GIZMO_ROTOR_INDICATOR_COLOR, 1));
		} break;
	}
}

void scene_render(Scene *scene, r32 alpha, r32 aspect_ratio, Scene_Render_Flags flags) {
	Camera *camera = scene_primary_camera(scene);

	r32 view_height = camera->lens.field_of_view;
	r32 view_width = aspect_ratio * view_height;

	Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height, camera->lens.near, camera->lens.far);

	im2d_set_stroke_weight(0.02f);

	r32 scale = powf(0.5f, camera->distance);
	r32 iscale = 1.0f / scale;

	Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera->position, 0.0f));

	bool editor_map_mode = true;
	#ifdef ENABLE_DEVELOPER_OPTIONS
	editor_map_mode = (scene->debug.editor.mode == Scene_Editor_Mode_MAP);
	#endif

	im2d_begin(view, transform);

	if ((flags & Scene_Render_WORLD) && editor_map_mode) {
		s64 count = scene->by_type.character.count;
		auto &characters = scene->by_type.character;
		for (s64 index = 0; index < count; ++index) {
			Character &c = characters[index];
			im2d_circle(c.position, c.radius, c.color);
			im2d_line(c.position, c.position + c.rigid_body->velocity, vec4(0, 1.5f, 0));
		}
	}

	if ((flags & Scene_Render_FIXTURE) && editor_map_mode) {
		#ifdef ENABLE_DEVELOPER_OPTIONS
		Rigid_Body *body_hovered = scene->debug.editor.map.hovered_body;
		#endif

		for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
			auto &body = ptr->data;

			Vec4 color = (body.flags & Rigid_Body_COLLIDING) ? vec4(1, 0, 0) : vec4(0.7f, 0.45f, 0);

			#ifdef ENABLE_DEVELOPER_OPTIONS
			if (&body == body_hovered) {
				color.xyz = vec3(1) - color.xyz;
			}
			#endif

			for (u32 index = 0; index < body.fixture_count; ++index) {
				auto f = rigid_body_get_fixture(&body, index);
				iscene_render_shape_transformed(*f, body.transform, color.xyz);
			}

			color = (body.flags & Rigid_Body_BOUNDING_BOX_COLLIDING) ? vec4(0.7f, 0.1f, 0.1f, 1) : vec4(0.1f, 0.7f, 0.1f, 1);

			im2d_rect_outline(body.bounding_box.min, body.bounding_box.max - body.bounding_box.min, color);
		}
	}

	#ifdef ENABLE_DEVELOPER_OPTIONS
	if ((flags & Scene_Render_COLLISION) && editor_map_mode) {
		auto manifolds = scene->debug.manifold;
		for (auto &m : manifolds) {
			im2d_line(m.contacts[1], m.contacts[1] + m.penetration * m.normal, vec4(1, 0, 1), 0.02f);

			im2d_circle(m.contacts[0], 0.08f, vec4(1, 0, 1));
			im2d_circle(m.contacts[1], 0.08f, vec4(1, 0, 1));
		}
	}
	#endif

	im2d_end();

	#ifdef ENABLE_DEVELOPER_OPTIONS
	auto &editor = scene->debug.editor;
	auto &gizmo = scene->debug.editor.gizmo;
	auto body_selected = scene->debug.editor.map.selected_body;

	if (flags & Scene_Render_EDITOR) {

		switch (editor.mode) {
			case Scene_Editor_Mode_MAP: {
				if (gizmo.render_type != Gizmo_Render_Type_NONE) {
					// Using the same view as primary camera and updating the primary camera transform
					Vec2 p = body_selected->transform.p;
					transform = transform * mat4_translation(vec3(p, 0)) * mat4_scalar(iscale, iscale, 1);
					im2d_begin(view, transform);
					gizmo_render(gizmo, body_selected->transform.xform);
					im2d_end();
				}
			} break;

			case Scene_Editor_Mode_FIXTURE: {
				if (editor.fixture.index >= 0) {
					camera = scene_editor_camera(scene, Scene_Editor_Mode_FIXTURE);

					view_height = camera->lens.field_of_view;
					view_width = aspect_ratio * view_height;

					view = orthographic_view(-view_width, view_width, view_height, -view_height, camera->lens.near, camera->lens.far);

					scale = powf(0.5f, camera->distance);
					iscale = 1.0f / scale;

					transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera->position, 0.0f));

					im2d_begin(view, transform);

					Resource_Fixture &resource = scene->resource_fixtures[editor.fixture.index];

					u32 count = resource.fixture_count;
					auto &fixtures = resource.fixtures;

					for (u32 index = 0; index < count; ++index) {
						auto f = fixtures + index;
						switch (f->shape) {
							case Fixture_Shape_Circle: {
								auto shape = fixture_get_shape(f, Circle);
								im2d_circle(shape->center, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
								iscene_render_shape(*shape, vec4(0, 1, 0, 0.3f), vec4(0, 2, 0));
							} break;

							case Fixture_Shape_Mm_Rect: {
								auto shape = fixture_get_shape(f, Mm_Rect);
								im2d_circle(shape->min, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
								im2d_circle(shape->max, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
								iscene_render_shape(*shape, vec4(0, 1, 0, 0.1f), vec4(0, 2, 0));
							} break;

							case Fixture_Shape_Capsule: {
								auto shape = fixture_get_shape(f, Capsule);
								im2d_circle(shape->a, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
								im2d_circle(shape->b, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
								iscene_render_shape(*shape, vec4(0, 1, 0, 0.1f), vec4(0, 2, 0));
							} break;

							case Fixture_Shape_Polygon: {
								auto shape = fixture_get_shape(f, Polygon);
								u32 vcount = shape->vertex_count;
								auto v = shape->vertices;
								for (u32 vi = 0; vi < vcount; ++vi, ++v) {
									im2d_circle(*v, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
								}
								iscene_render_shape(*shape, vec4(0, 1, 0, 0.1f), vec4(0, 2, 0));
							} break;
						}
					}

					im2d_line(vec2(-0.5f, 0.0f), vec2(0.5f, 0.0f), vec4(1));
					im2d_line(vec2(0.0f, -0.5f), vec2(0.0f, 0.5f), vec4(1));
					
					if (editor.fixture.hovered_vertex_ptr) {
						Vec2 p = *editor.fixture.hovered_vertex_ptr;
						r32 a, b, c, d;
						a = editor.fixture.vertex_pointer_angle;
						b = a + 0.5f * MATH_TAU;
						while (b >= 2 * MATH_PI) b -= 2 * MATH_PI;
						c = a + MATH_PI;
						while (c >= 2 * MATH_PI) c -= 2 * MATH_PI;
						d = c + 0.5f * MATH_TAU;
						while (d >= 2 * MATH_PI) d -= 2 * MATH_PI;
						im2d_pie_part(p, EDITOR_VERTEX_SELECTOR_RADIUS, EDITOR_VERTEX_SELECTOR_RADIUS + EDITOR_VERTEX_SELECTOR_THICKNESS, a, b, EDITOR_SELECTED_VERTEX_COLOR);
						im2d_pie_part(p, EDITOR_VERTEX_SELECTOR_RADIUS, EDITOR_VERTEX_SELECTOR_RADIUS + EDITOR_VERTEX_SELECTOR_THICKNESS, c, d, EDITOR_SELECTED_VERTEX_COLOR);
					}

					if (editor.fixture.selected_vertex_ptr) {
						im2d_circle(*editor.fixture.selected_vertex_ptr, EDITOR_VERTEX_RADIUS, EDITOR_SELECTED_VERTEX_COLOR);
						
						Mat4 gt = mat4_translation(vec3(*editor.fixture.selected_vertex_ptr, 0)) *
							mat4_scalar(iscale, iscale, 1);
						im2d_push_matrix(gt);
						gizmo_render(gizmo, mat2_identity());
						im2d_pop_matrix();
					}

					im2d_end();
				}
			} break;

				invalid_default_case();
		}

	}

	iscene_make_editor(scene);

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
