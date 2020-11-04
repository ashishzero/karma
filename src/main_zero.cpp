#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/reflection.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"
#include "modules/imgui/imconfig.h"
#include "modules/imgui/imgui.h"
#include "modules/imgui/dev.h"
#include "modules/gfx/renderer.h"
#include "modules/core/thread_pool.h"

#include "scene.h"
#include "editor.h"

#include ".generated/entity.typeinfo"

struct Player_Controller {
	r32 x, y;
};

typedef bool(*Collision_Resolver)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold);
static Collision_Resolver COLLISION_RESOLVERS[Fixture_Shape_Count][Fixture_Shape_Count];

typedef bool(*Continuous_Collision_Resolver)(Fixture &a, Fixture &b, const Transform &a_from, const Transform &a_to, const Transform &b_from, const Transform &b_to, Impact_Time *impact);
static Continuous_Collision_Resolver CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Count][Fixture_Shape_Count];

typedef bool(*Nearest_Points_Finder)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Nearest_Points *nearest_points);
static Nearest_Points_Finder NEAREST_POINTS_FINDERS[Fixture_Shape_Count][Fixture_Shape_Count];

typedef bool(*Collision_Detector)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb);
static Collision_Detector COLLISION_DETECTORS[Fixture_Shape_Count][Fixture_Shape_Count];

// TODO: Do we really need null collider?? REMOVE THIS
static bool null_collision_resolver(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold) {
	return false;
}
static bool null_continuous_collision_resolver(Fixture &a, Fixture &b, const Transform &a_from, const Transform &a_to, const Transform &b_from, const Transform &b_to, Impact_Time *impact) {
	return false;
}

static bool null_nearest_points_finder(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Nearest_Points *nearest_points) {
	nearest_points->a = vec2(INFINITY, INFINITY);
	nearest_points->b = vec2(INFINITY, INFINITY);
	nearest_points->distance = INFINITY;
	return false;
}

static bool null_collision_detector(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb) {
	return false;
}

template <typename ShapeA, typename ShapeB>
static bool shapes_collision_resolver(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold) {
	return gjk_epa(*(ShapeA *)a.handle, *(ShapeB *)b.handle, manifold, ta, tb);
}

template <typename ShapeA, typename ShapeB>
static bool shapes_continuous_collision_resolver(Fixture &a, Fixture &b, const Transform &a_from, const Transform &a_to, const Transform &b_from, const Transform &b_to, Impact_Time *impact) {
	return gjk_continuous(*(ShapeA *)a.handle, *(ShapeB *)b.handle, a_from, a_to, b_from, b_to, impact);
}

template <typename ShapeA, typename ShapeB>
static bool shapes_nearest_points_finder(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Nearest_Points *nearest_points) {
	return gjk_nearest_points(*(ShapeA *)a.handle, *(ShapeB *)b.handle, nearest_points, ta, tb);
}

template <typename ShapeA, typename ShapeB>
static bool shapes_collision_detector(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb) {
	return gjk(*(ShapeA *)a.handle, *(ShapeB *)b.handle, ta, tb);
}

static void collision_resover_init() {
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Null]		= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Circle]	= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Mm_Rect]	= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Capsule]	= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Polygon]	= null_collision_resolver;

	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Null]		= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Circle]		= shapes_collision_resolver<Circle, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect]	= shapes_collision_resolver<Circle, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Capsule]	= shapes_collision_resolver<Circle, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Polygon]	= shapes_collision_resolver<Circle, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Null]		= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle]	= shapes_collision_resolver<Mm_Rect, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect]	= shapes_collision_resolver<Mm_Rect, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule]	= shapes_collision_resolver<Mm_Rect, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon]	= shapes_collision_resolver<Mm_Rect, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Null]		= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Circle]	= shapes_collision_resolver<Capsule, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect]	= shapes_collision_resolver<Capsule, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule]	= shapes_collision_resolver<Capsule, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon]	= shapes_collision_resolver<Capsule, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Null]		= null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Circle]	= shapes_collision_resolver<Polygon, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect]	= shapes_collision_resolver<Polygon, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule]	= shapes_collision_resolver<Polygon, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon]	= shapes_collision_resolver<Polygon, Polygon>;

	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Null] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Circle] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Mm_Rect] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Capsule] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Polygon] = null_continuous_collision_resolver;
	
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Null] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Circle, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Circle, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Circle, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Circle, Polygon>;
	
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Null] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Mm_Rect, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Mm_Rect, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Mm_Rect, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Mm_Rect, Polygon>;
	
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Null] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Capsule, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Capsule, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Capsule, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Capsule, Polygon>;
	
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Null] = null_continuous_collision_resolver;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Circle] = shapes_continuous_collision_resolver<Polygon, Circle>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect] = shapes_continuous_collision_resolver<Polygon, Mm_Rect>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule] = shapes_continuous_collision_resolver<Polygon, Capsule>;
	CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon] = shapes_continuous_collision_resolver<Polygon, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Null][Fixture_Shape_Null]		= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Null][Fixture_Shape_Circle]	= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Null][Fixture_Shape_Mm_Rect]	= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Null][Fixture_Shape_Capsule]	= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Null][Fixture_Shape_Polygon]	= null_nearest_points_finder;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Null]	= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Circle]	= shapes_nearest_points_finder<Circle, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect]	= shapes_nearest_points_finder<Circle, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_nearest_points_finder<Circle, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_nearest_points_finder<Circle, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Null]		= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle]		= shapes_nearest_points_finder<Mm_Rect, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect]	= shapes_nearest_points_finder<Mm_Rect, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule]	= shapes_nearest_points_finder<Mm_Rect, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon]	= shapes_nearest_points_finder<Mm_Rect, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Null]		= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Circle]		= shapes_nearest_points_finder<Capsule, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect]	= shapes_nearest_points_finder<Capsule, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule]	= shapes_nearest_points_finder<Capsule, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon]	= shapes_nearest_points_finder<Capsule, Polygon>;

	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Null]		= null_nearest_points_finder;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Circle]		= shapes_nearest_points_finder<Polygon, Circle>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect]	= shapes_nearest_points_finder<Polygon, Mm_Rect>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule]	= shapes_nearest_points_finder<Polygon, Capsule>;
	NEAREST_POINTS_FINDERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon]	= shapes_nearest_points_finder<Polygon, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Null][Fixture_Shape_Null]		= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Null][Fixture_Shape_Circle]	= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Null][Fixture_Shape_Mm_Rect]	= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Null][Fixture_Shape_Capsule]	= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Null][Fixture_Shape_Polygon]	= null_collision_detector;

	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Null]		= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Circle]		= shapes_collision_detector<Circle, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect]	= shapes_collision_detector<Circle, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Capsule]	= shapes_collision_detector<Circle, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Circle][Fixture_Shape_Polygon]	= shapes_collision_detector<Circle, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Null]		= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle]	= shapes_collision_detector<Mm_Rect, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect]	= shapes_collision_detector<Mm_Rect, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule]	= shapes_collision_detector<Mm_Rect, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon]	= shapes_collision_detector<Mm_Rect, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Null]		= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Circle]	= shapes_collision_detector<Capsule, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect]	= shapes_collision_detector<Capsule, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Capsule]	= shapes_collision_detector<Capsule, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Capsule][Fixture_Shape_Polygon]	= shapes_collision_detector<Capsule, Polygon>;

	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Null]		= null_collision_detector;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Circle]	= shapes_collision_detector<Polygon, Circle>;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect]	= shapes_collision_detector<Polygon, Mm_Rect>;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Capsule]	= shapes_collision_detector<Polygon, Capsule>;
	COLLISION_DETECTORS[Fixture_Shape_Polygon][Fixture_Shape_Polygon]	= shapes_collision_detector<Polygon, Polygon>;
}

static bool fixture_vs_fixture(Fixture *a, Fixture *b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold) {
	return COLLISION_RESOLVERS[a->shape][b->shape](*a, *b, ta, tb, manifold);
}

static bool fixture_vs_fixture_continuous(Fixture *a, Fixture *b, const Transform &a_from, const Transform &a_to, const Transform &b_from, const Transform &b_to, Impact_Time *impact) {
	return CONTINUOUS_COLLISION_RESOLVERS[a->shape][b->shape](*a, *b, a_from, a_to, b_from, b_to, impact);
}

static bool nearest_points_fixture_fixture(Fixture *a, Fixture *b, const Transform &ta, const Transform &tb, Nearest_Points *nearest_points) {
	return NEAREST_POINTS_FINDERS[a->shape][b->shape](*a, *b, ta, tb, nearest_points);
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

static bool nearest_points_fixture_point(Fixture &a, const Transform &ta, Vec2 point, r32 size, Nearest_Points *nearest_points) {
	Circle circle = { point, size };
	Fixture b;
	b.shape = Fixture_Shape_Circle;
	b.handle = &circle;

	Transform tb;
	tb.p = vec2(0);
	tb.xform = mat2_identity();

	return NEAREST_POINTS_FINDERS[a.shape][b.shape](a, b, ta, tb, nearest_points);
}

static bool test_fixture_point(Fixture &a, const Transform &ta, Vec2 point) {
	Circle circle = { point, 0.0f };
	Fixture b;
	b.shape = Fixture_Shape_Circle;
	b.handle = &circle;

	Transform tb;
	tb.p = vec2(0);
	tb.xform = mat2_identity();

	return COLLISION_DETECTORS[a.shape][b.shape](a, b, ta, tb);
}

enum Physics_State {
	Physics_State_RUNNING,
	Physics_State_PAUSED,
	Physics_State_RUN_SINGLE_STEP,
};

int karma_user_zero() {
	collision_resover_init();

#ifdef INIT_THREAD_POOL
	if (!async_initialize(2, mega_bytes(32), context.allocator)) {
		system_fatal_error("Thread could not be created");
	}
#endif

	auto render_api = Render_Backend_DIRECTX11;

	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	ImGui_Initialize();
	Dev_ModeEnable();

	Dev_SetPresentationState(false);

	bool running = true;

	r32 aspect_ratio = framebuffer_w / framebuffer_h;
	const r32 speed_factor = 1;

	r32 const fixed_dt = 1.0f / 60.0f;
	r32       dt = fixed_dt * speed_factor;
	r32       game_dt = fixed_dt * speed_factor;
	r32       real_dt = fixed_dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = fixed_dt;

	r32 window_w = 0, window_h = 0;

	r32 im_unit_circle_cos[IM_MAX_CIRCLE_SEGMENTS];
	r32 im_unit_circle_sin[IM_MAX_CIRCLE_SEGMENTS];

	for (int i = 0; i < IM_MAX_CIRCLE_SEGMENTS; ++i) {
		r32 theta = ((r32)i / (r32)IM_MAX_CIRCLE_SEGMENTS) * MATH_PI * 2;
		im_unit_circle_cos[i] = cosf(theta);
		im_unit_circle_sin[i] = sinf(theta);
	}

	Scene *scene = scene_create();

	scene->camera.id = 0;
	scene->camera.type = Entity_Type_Camera;

	Fixture fixture;
	Resource_Id id;

	Physics_State physics_state = Physics_State_RUNNING;

	Entity_Info info;
	
	{
		Circle circle;
		circle.center = vec2(0);
		circle.radius = 1;
		
		fixture.shape = Fixture_Shape_Circle;
		fixture.handle = &circle;
		id = scene_create_new_resource_fixture(scene, &fixture, 1);

		info.position = vec2(5);
		info.rigid_body.type = Rigid_Body_Type_Dynamic;
		info.rigid_body.fixture_id = id;
		info.rigid_body.transform.p = info.position;
		info.rigid_body.transform.xform = mat2_identity();

		((Player *)scene_create_new_entity(scene, Entity_Type_Player, info));

		info.position = vec2(-3, 7);
		info.rigid_body.transform.p = info.position;
		((Player *)scene_create_new_entity(scene, Entity_Type_Player, info));

		Mm_Rect rect;
		rect.min = vec2(-1);
		rect.max = vec2( 1);

		fixture.shape = Fixture_Shape_Mm_Rect;
		fixture.handle = &rect;
		id = scene_create_new_resource_fixture(scene, &fixture, 1);

		info.position = vec2(2, 5);
		info.rigid_body.transform.p = info.position;
		info.rigid_body.fixture_id = id;
		((Player *)scene_create_new_entity(scene, Entity_Type_Player, info));
	}

	{
		info.rigid_body.type = Rigid_Body_Type_Static;

		{
			Vec2 points[] = {
				vec2(-2.4f, 4.6f), vec2(3.6f, 4.6f), vec2(4.6f, -1.4f), vec2(1.6f, -5.4f), vec2(-7.4f, -2.4f)
			};
			assert(static_count(points) >= 3);

			auto polygon = (Polygon *)tallocate(sizeof(Polygon) + sizeof(Vec2) * (static_count(points) - 3));
			polygon->vertex_count = static_count(points);
			memcpy(polygon->vertices, points, sizeof(points));
			fixture.shape = Fixture_Shape_Polygon;
			fixture.handle = polygon;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(-5.6f, 0.4f);
			info.rigid_body.transform.xform = mat2_identity();
			info.rigid_body.transform.p = info.position;
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Type_Obstacle, info);
		}

		{
			Circle circle;
			circle.center = vec2(0);
			circle.radius = 0.6f;
			fixture.shape = Fixture_Shape_Circle;
			fixture.handle = &circle;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(1);
			info.rigid_body.transform.xform = mat2_identity();
			info.rigid_body.transform.p = info.position;
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Type_Obstacle, info);
		}

		{
			Mm_Rect rect;
			rect.min = vec2(-2.5f, -3.5f);
			rect.max = vec2(2.5f, 3.5f);
			fixture.shape = Fixture_Shape_Mm_Rect;
			fixture.handle = &rect;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(6.5f, -0.5f);
			info.rigid_body.transform.xform = mat2_rotation(to_radians(10));
			info.rigid_body.transform.p = info.position;
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Type_Obstacle, info);
		}

		{
			Mm_Rect rect;
			rect.min = vec2(-1);
			rect.max = vec2(1);
			fixture.shape = Fixture_Shape_Mm_Rect;
			fixture.handle = &rect;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(0, -8);
			info.rigid_body.transform.xform = mat2_scalar(15, 1);
			info.rigid_body.transform.p = info.position;
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Type_Obstacle, info);
		}

		{
			Circle circle;
			circle.center = vec2(1, -1);
			circle.radius = 1;

			Capsule capsule;
			capsule.a = vec2(-2, -3);
			capsule.b = vec2(2, 3);
			capsule.radius = 1;

			Fixture f[2];
			f[0].shape = Fixture_Shape_Circle;
			f[0].handle = &circle;
			f[1].shape = Fixture_Shape_Capsule;
			f[1].handle = &capsule;

			id = scene_create_new_resource_fixture(scene, f, static_count(f));

			info.position = vec2(-1, -5);
			info.rigid_body.transform.xform = mat2_identity();
			info.rigid_body.transform.p = info.position;
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Type_Obstacle, info);
		}
	}

	Player_Controller controller = {};

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	static r32 gravity = 0.0f;

	while (running) {
		Dev_TimedFrameBegin();

		static u32 primary_player_index = 0;
		auto primary_player = &scene->by_type.player[primary_player_index];

		Dev_TimedBlockBegin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

			if (ImGui_HandleEvent(event))
				continue;

			if (event.type & Event_Type_WINDOW_RESIZE) {
				s32 w = event.window.dimension.x;
				s32 h = event.window.dimension.y;

				gfx_on_client_resize(w, h);
				window_w = (r32)w;
				window_h = (r32)h;

				if (window_w && window_h) {
					aspect_ratio = window_w / window_h;
					gfx_resize_framebuffer(w, h, lroundf(512 * aspect_ratio), 512);
					framebuffer_w = window_w;
					framebuffer_h = window_h;
				}

				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F1) {
				Dev_TogglePresentationState();
				continue;
			}
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_TAB) {
				primary_player_index = (primary_player_index + 1) % (u32)(scene->by_type.player.count);
				continue;
			}

			if (event.type & Event_Type_KEYBOARD) {
				float value = (float)(event.key.state == Key_State_DOWN);
				switch (event.key.symbol) {
				case Key_D:
				case Key_RIGHT:
					controller.x = value;
					break;
				case Key_A:
				case Key_LEFT:
					controller.x = -value;
					break;

				case Key_W:
				case Key_UP:
					controller.y = value;
					break;
				case Key_S:
				case Key_DOWN:
					controller.y = -value;
					break;
				}
			}

			if (event.type & Event_Type_CONTROLLER_AXIS) {
				if (event.controller_axis.symbol == Controller_Axis_LTHUMB_X)
					controller.x = event.controller_axis.value;
				else if (event.controller_axis.symbol == Controller_Axis_LTHUMB_Y)
					controller.y = event.controller_axis.value;
			}

		}

		Dev_TimedBlockEnd(EventHandling);

		Dev_TimedBlockBegin(Simulation);

		static r32 movement_force = 20;

		Contact_Manifold manifold;
		manifold.normal = vec2(0);

		Impact_Time impact_time;
		impact_time.t = 0;

		Array<Impact_Time> impacts;
		impacts.allocator = TEMPORARY_ALLOCATOR;

		Array<Contact_Manifold> manifolds;
		manifolds.allocator = TEMPORARY_ALLOCATOR;

		Nearest_Points np;
		np.distance = 0;

		Array<Nearest_Points> nearest_points;
		nearest_points.allocator = TEMPORARY_ALLOCATOR;
		static int number_of_iterations = 4;

		while (accumulator_t >= fixed_dt) {
			if (physics_state == Physics_State_PAUSED)
				break;
			else if (physics_state == Physics_State_RUN_SINGLE_STEP)
				physics_state = Physics_State_PAUSED;

			Dev_TimedScope(SimulationFrame);

			r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
			Vec2 dir = vec2(0);
			if (len) {
				dir.x = controller.x / len;
				dir.y = controller.y / len;
			}

			if (len) {
				primary_player->rigid_body->force = movement_force * dir;
				//set_bit(primary_player->rigid_body->colliders->flags, Collision_Bit_MOTION);
			}

			primary_player->color = vec4(0, 1, 1);

			// TODO: Do broad phase collision detection
			for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
				if (ptr->data.type == Rigid_Body_Type_Dynamic) {
					ptr->data.velocity += dt * ptr->data.force + vec2(0, -gravity);
					ptr->data.velocity *= powf(0.5f, ptr->data.drag * dt);
					ptr->data.force = vec2(0);
				}
				clear_bit(ptr->data.flags, Rigid_Body_COLLIDING);
			}

			for (auto &player : scene->by_type.player) {
				player.color = vec4(1);
				player.rigid_body->final_transform.p = player.position + player.rigid_body->velocity * dt;
				player.rigid_body->final_transform.xform = mat2_scalar(player.radius, player.radius);
			}

			Vec2 dv, dp;
			//Contact_Manifold manifold;

			for (int iteration = 0; iteration < number_of_iterations; ++iteration) {
				for (auto a = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, a); a = iter_next<Rigid_Body>(a)) {
					Rigid_Body &a_body = a->data;
					for (auto b = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, b); b = iter_next<Rigid_Body>(b)) {
						if (a == b) continue;

						Rigid_Body &b_body = b->data;

						if (a_body.type == Rigid_Body_Type_Static && b_body.type == Rigid_Body_Type_Static)
							continue;

						dv = a_body.velocity - b_body.velocity;
						dp = dv * dt;

						for (u32 b_index = 0; b_index < b_body.fixture_count; ++b_index) {
							Fixture *b_collider = rigid_body_get_fixture(&b_body, b_index);
							for (u32 a_index = 0; a_index < a_body.fixture_count; ++a_index) {
								Fixture *a_collider = rigid_body_get_fixture(&a_body, a_index);

#if 1
#if 0
								if (fixture_vs_fixture(b_collider, a_collider, b_body.transform, a_body.transform, &manifold)) {
#else
								

								auto &b_from = b_body.transform;
								auto &b_to = b_body.final_transform;
								auto &a_from = a_body.transform;
								auto &a_to = a_body.final_transform;

								if (fixture_vs_fixture_continuous(b_collider, a_collider, b_from, b_to, a_from, a_to, &impact_time)) {
#endif
								
									//if (vec2_dot(dv, manifold.normal) >= 0.0f) continue;

#if 1
#if 1
									array_add(&impacts, impact_time);
									b_body.flags |= Rigid_Body_COLLIDING;
									a_body.flags |= Rigid_Body_COLLIDING;
#else
									r32 e = minimum(a_body.restitution, b_body.restitution);
									r32 j = -(1 + e) * vec2_dot(dv, manifold.normal) / (a_body.imass + b_body.imass);
									j = maximum(j, 0);

									a_body.velocity += j * a_body.imass * manifold.normal;
									b_body.velocity -= j * b_body.imass * manifold.normal;
#endif

#else
									r32 t = manifold.penetration / dt;
									t = clamp01(t);

									if (b_body.type == Rigid_Body_Type_Dynamic && a_body.type == Rigid_Body_Type_Dynamic) {
										a_body.velocity += manifold.normal * t;
										b_body.velocity -= manifold.normal * t;
									} else if (a_body.type == Rigid_Body_Type_Dynamic) {
										a_body.velocity += manifold.normal * t;
									} else {
										b_body.velocity -= manifold.normal * t;
									}
#endif
									//dv = a_body.velocity - b_body.velocity;
									//dp = dv * dt;
									//
									//array_add(&manifolds, manifold);
									//
									//b_body.flags |= Rigid_Body_COLLIDING;
									//a_body.flags |= Rigid_Body_COLLIDING;
								}
#else
								if (nearest_points_fixture_fixture(b_collider, a_collider, b_body.transform, a_body.transform, &np)) {
									array_add(&nearest_points, np);
								}
#endif

							}
						}
					}
				}
			}

			for (auto &player : scene->by_type.player) {
				player.position += player.rigid_body->velocity * dt;
				player.rigid_body->transform.p = player.rigid_body->final_transform.p;
				player.rigid_body->transform.xform = mat2_scalar(player.radius, player.radius);
			}

			r32 camera_follow_speed = 0.977f;
			scene->camera.position = lerp(scene->camera.position, primary_player->position, 1.0f - powf(1.0f - camera_follow_speed, dt));

			accumulator_t -= fixed_dt;
		}

		ImGui_UpdateFrame(real_dt);

		r32 view_height = 5.0f;
		r32 view_width = aspect_ratio * view_height;

		auto view = orthographic_view(-view_width, view_width, view_height, -view_height);

#if 0 
		auto cursor = system_get_cursor_position();
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor = 2.0f * cursor - vec2(1);
		cursor.x *= view_width;
		cursor.y *= view_height;
		cursor += scene->camera.position;

		if (ImGui_IsUsingCursor()) {
			cursor.x = 0;
			cursor.y = 0;
		}

		//Contact_Manifold manifold;
		Nearest_Points nearest_points;

		Array<Nearest_Points> all_nearest_points;
		all_nearest_points.allocator = TEMPORARY_ALLOCATOR;

		primary_player->rigid_body->xform = mat3_translation(primary_player->position);

		bool draw_cursor = false;

		for (auto a = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, a); a = iter_next<Rigid_Body>(a)) {
			Rigid_Body &a_body = a->data;
			for (u32 a_index = 0; a_index < a_body.fixture_count; ++a_index) {
				Fixture &fixture = *rigid_body_get_fixture(&a_body, a_index);
				if (collider_point_nearest_points(fixture, a_body.xform, cursor, 0, &nearest_points)) {
					draw_cursor = true;
					//array_add(&manifolds, manifold);
				}
				array_add(&all_nearest_points, nearest_points);
			}
		}

#endif

		Dev_TimedBlockEnd(Simulation);

		Dev_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.05f, 0.05f, 0.05f, 1.0f));
		gfx_viewport(0, 0, window_w, window_h);

		r32 scale = powf(0.5f, scene->camera.distance);
		Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-scene->camera.position, 0.0f));

		im2d_begin(view, transform);

		for (auto &player : scene->by_type.player) {
			im2d_circle(player.position, player.radius, player.color * player.intensity);
			im2d_line(player.position, player.position + player.rigid_body->velocity, vec4(0, 1.5f, 0), 0.02f);
		}

		im2d_circle(vec2(0), 0.05f, vec4(1.2f, 1.2f, 1.2f));

		for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
			auto &body = ptr->data;

			for (u32 index = 0; index < body.fixture_count; ++index) {
				Mat3 xform = mat2_to_mat3(body.transform.xform);
				xform.rows[0].z = body.transform.p.x;
				xform.rows[1].z = body.transform.p.y;

				im2d_push_matrix(mat3_to_mat4(xform));

				auto f = rigid_body_get_fixture(&body, index);
				auto color = (body.flags & Rigid_Body_COLLIDING) ? vec4(1, 0, 0) : vec4(0, 1, 1);

				switch (f->shape) {
				case Fixture_Shape_Null: {

				} break;

				case Fixture_Shape_Circle: {
					auto circle = fixture_get_shape(f, Circle);
					im2d_circle_outline(circle->center, circle->radius, color, 0.02f);
				} break;

				case Fixture_Shape_Polygon: {
					auto polygon = fixture_get_shape(f, Polygon);
					im2d_polygon_outline(*polygon, color, 0.02f);
				} break;

				case Fixture_Shape_Mm_Rect: {
					auto rect = fixture_get_shape(f, Mm_Rect);
					im2d_rect_outline(rect->min, rect->max - rect->min, color, 0.02f);
				} break;

				case Fixture_Shape_Capsule: {
					auto capsule = fixture_get_shape(f, Capsule);
					Vec2 capsule_dir = capsule->b - capsule->a;
					Vec2 capsule_norm = vec2_normalize(vec2(-capsule_dir.y, capsule_dir.x)) * capsule->radius;

					im2d_circle_outline(capsule->a, capsule->radius, color, 0.02f);
					im2d_circle_outline(capsule->b, capsule->radius, color, 0.02f);
					im2d_line(capsule->a + capsule_norm, capsule->b + capsule_norm, color, 0.02f);
					im2d_line(capsule->a - capsule_norm, capsule->b - capsule_norm, color, 0.02f);
				} break;

					invalid_default_case();

				}

				im2d_pop_matrix();
			}
		}

		for (auto &m : manifolds) {
			im2d_line(m.contacts[1], m.contacts[1] + m.penetration * m.normal, vec4(1, 0, 1), 0.02f);

			im2d_circle(m.contacts[0], 0.08f, vec4(1, 0, 1));
			im2d_circle(m.contacts[1], 0.08f, vec4(1, 0, 1));
		}

		for (auto &i : impacts) {
			im2d_line(i.contact, i.contact + i.t * i.normal, vec4(1, 0, 1), 0.02f);
			im2d_circle(i.contact, 0.08f, vec4(1, 0, 1));
		}
		//if (draw_cursor) {
		//	im2d_circle(cursor, 0.1f, 2 * vec4(1, 1, 0));
		//}

		for (auto &n : nearest_points) {
			im2d_line(n.a, n.b, vec4(1, 0, 1), 0.02f);
		}

		im2d_end();


		gfx_end_drawing();


		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0.0f));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

#if defined(BUILD_DEVELOPER_SERVICE)
		{
			Dev_TimedScope(DebugRender);
			Dev_RenderFrame();
		}
#endif

		int new_render_api = (int)render_api;

		const char *render_api_strings[] = {
			"None",
			"OpenGL",
			"DirectX 11"
		};

		editor_entity(primary_player);
		
		ImGui::Begin("World", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
		
		if (physics_state == Physics_State_RUNNING) {
			if (ImGui::Button("Pause")) {
				physics_state = Physics_State_PAUSED;
			}
		} else if (physics_state == Physics_State_PAUSED) {
			if (ImGui::Button("Run")) {
				physics_state = Physics_State_RUNNING;
			}
			ImGui::SameLine();
			if (ImGui::Button("Step")) {
				physics_state = Physics_State_RUN_SINGLE_STEP;
			}
		}

		ImGui::DragFloat("Gravity", &gravity, 0.01f);
		ImGui::DragInt("Iteration", &number_of_iterations, 1, 1, 10000);
		ImGui::DragFloat("Movement Force", &movement_force, 0.01f);
		editor_draw(scene->camera);
		ImGui::End();

		//ImGui::ShowDemoWindow();

#if defined(BUILD_IMGUI)
		{
			Dev_TimedScope(ImGuiRender);
			ImGui_RenderFrame();
		}
#endif

		gfx_end_drawing();

		Dev_TimedBlockBegin(Present);
		gfx_present();
		Dev_TimedBlockEnd(Present);
		Dev_TimedBlockEnd(Rendering);

		reset_temporary_memory();

		auto new_counter = system_get_counter();
		auto counts = new_counter - counter;
		counter = new_counter;

		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;

		game_dt = real_dt * speed_factor;
		dt = fixed_dt * speed_factor;

		accumulator_t += real_dt;
		accumulator_t = minimum(accumulator_t, 0.3f);

		Dev_TimedFrameEnd(real_dt);

		if ((Render_Backend)new_render_api != render_api) {
			gfx_destroy_context();
			render_api = (Render_Backend)new_render_api;
			gfx_create_context(platform, render_api, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);
			ImGui_RefreshRenderingContext();
		}

	}

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
