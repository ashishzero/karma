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

typedef Impact_Type(*Continuous_Collision_Resolver)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Impact_Time *impact);
static Continuous_Collision_Resolver CONTINUOUS_COLLISION_RESOLVERS[Fixture_Shape_Count][Fixture_Shape_Count];

typedef bool(*Nearest_Points_Finder)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Nearest_Points *nearest_points);
static Nearest_Points_Finder NEAREST_POINTS_FINDERS[Fixture_Shape_Count][Fixture_Shape_Count];

typedef bool(*Collision_Detector)(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb);
static Collision_Detector COLLISION_DETECTORS[Fixture_Shape_Count][Fixture_Shape_Count];

// TODO: Do we really need null collider?? REMOVE THIS
static bool null_collision_resolver(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Contact_Manifold *manifold) {
	return false;
}
static Impact_Type null_continuous_collision_resolver(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Impact_Time *impact) {
	return Impact_Type_SEPERATED;
}

static bool null_nearest_points_finder(Fixture &a, Fixture &b, const Transform &ta, const Transform &tb, Vec2 a_dp, Vec2 b_dp, Nearest_Points *nearest_points) {
	nearest_points->a = vec2(INFINITY, INFINITY);
	nearest_points->b = vec2(INFINITY, INFINITY);
	nearest_points->distance2 = INFINITY;
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

static void render_shape(Fixture &fixture, const Transform &transform, Vec3 color) {
	const r32 alpha = 0.1f;
	auto shade = vec4(color, alpha);
	auto outline = vec4(color, 1);

	Mat3 xform = mat2_to_mat3(transform.xform);
	xform.rows[0].z = transform.p.x;
	xform.rows[1].z = transform.p.y;

	im2d_push_matrix(mat3_to_mat4(xform));

	switch (fixture.shape) {
	case Fixture_Shape_Null: break;

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

enum Physics_State {
	Physics_State_RUNNING,
	Physics_State_PAUSED,
	Physics_State_RUN_SINGLE_STEP,
};

static const s32 SIMULATION_SPEED_FACTORS[] = {
	-8, -7, -6, -5, -4, -3, -2, 1, 2, 3, 4, 5, 6, 7, 8
};
static const u32 SIMULATION_SPEED_1X = 7;

struct Simulation_Speed {
	u32 index;
	s32 x;
	r32 factor;
};

static Simulation_Speed simulation_speed(u32 index) {
	assert(index < static_count(SIMULATION_SPEED_FACTORS));
	Simulation_Speed speed;
	speed.index = index;
	speed.x = SIMULATION_SPEED_FACTORS[speed.index];
	if (speed.x < 0)
		speed.factor = -1.0f / (r32)speed.x;
	else
		speed.factor = (r32)speed.x;
	return speed;
}

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
	
	Simulation_Speed sim_speed = simulation_speed(SIMULATION_SPEED_1X);

	r32 const dt = 1.0f / 60.0f;
	r32       game_dt = dt * sim_speed.factor;
	r32       real_dt = dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = dt;

	r32 window_w = 0, window_h = 0;

	Scene *scene = scene_create();

	Entity_Info info;
	Camera_Info camera_info;
	camera_info.target_position = vec2(0);
	camera_info.target_distance = 0;
	camera_info.distance = .4f;
	camera_info.follow_factor = 0.977f;
	camera_info.zoom_factor = 0.78f;
	camera_info.behaviour = Camera_Behaviour_ANIMATE;
	camera_info.lens.kind = Camera_View_Kind::ORTHOGRAPHIC;
	camera_info.lens.field_of_view = 5.0f;
	camera_info.lens.near = -1;
	camera_info.lens.far = 1;
	info.position = vec2(0);
	info.data = &camera_info;

	scene_create_new_entity(scene, Entity_Type_Camera, info);

	Entity *entity_hovered, *entity_selected;
	entity_hovered = nullptr;
	entity_selected = nullptr;

	Fixture fixture;
	Resource_Id id;

	Physics_State physics_state = Physics_State_RUNNING;
	
	Rigid_Body_Info rigid_body;
	rigid_body.fixture = true;
	info.data = &rigid_body;
	{

		Circle circle;
		circle.center = vec2(0);
		circle.radius = 1;
		
		fixture.shape = Fixture_Shape_Circle;
		fixture.handle = &circle;
		id = scene_create_new_resource_fixture(scene, &fixture, 1);

		info.position = vec2(5);
		rigid_body.type = Rigid_Body_Type_Dynamic;
		rigid_body.fixture_id = id;
		rigid_body.transform.p = info.position;
		rigid_body.transform.xform = mat2_identity();

		scene_create_new_entity(scene, Entity_Type_Character, info);

		info.position = vec2(-3, 7);
		rigid_body.transform.p = info.position;
		scene_create_new_entity(scene, Entity_Type_Character, info);

		Mm_Rect rect;
		rect.min = vec2(-1);
		rect.max = vec2( 1);

		fixture.shape = Fixture_Shape_Mm_Rect;
		fixture.handle = &rect;
		id = scene_create_new_resource_fixture(scene, &fixture, 1);

		info.position = vec2(2, 5);
		rigid_body.transform.p = info.position;
		rigid_body.fixture_id = id;
		scene_create_new_entity(scene, Entity_Type_Character, info);
	}

	{
		rigid_body.type = Rigid_Body_Type_Static;

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
			rigid_body.transform.xform = mat2_identity();
			rigid_body.transform.p = info.position;
			rigid_body.fixture_id = id;
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
			rigid_body.transform.xform = mat2_identity();
			rigid_body.transform.p = info.position;
			rigid_body.fixture_id = id;
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
			rigid_body.transform.xform = mat2_rotation(to_radians(10));
			rigid_body.transform.p = info.position;
			rigid_body.fixture_id = id;
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
			rigid_body.transform.xform = mat2_scalar(15, 1);
			rigid_body.transform.p = info.position;
			rigid_body.fixture_id = id;
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
			rigid_body.transform.xform = mat2_identity();
			rigid_body.transform.p = info.position;
			rigid_body.fixture_id = id;
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
		auto primary_player = &scene->by_type.character[primary_player_index];

		Array<Contact_Manifold> manifolds;
		manifolds.allocator = TEMPORARY_ALLOCATOR;

		Array<Impact_Time> impact_times;
		impact_times.allocator = TEMPORARY_ALLOCATOR;

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
				primary_player_index = (primary_player_index + 1) % (u32)(scene->by_type.character.count);
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

		r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
		Vec2 dir = vec2(0);
		if (len) {
			dir.x = controller.x / len;
			dir.y = controller.y / len;
		}

		if (len) {
			primary_player->rigid_body->force += movement_force * dir;
			//set_bit(primary_player->rigid_body->colliders->flags, Collision_Bit_MOTION);
		}

		while (accumulator_t >= dt) {
			if (physics_state == Physics_State_PAUSED)
				break;
			else if (physics_state == Physics_State_RUN_SINGLE_STEP)
				physics_state = Physics_State_PAUSED;

			Dev_TimedScope(SimulationFrame);

			// TODO: Do broad phase collision detection and narrow collision detection
			// TODO: Continuous collision detection

			Contact_Manifold manifold;
			manifold.penetration = 0;

			for (auto &player : scene->by_type.character) {
				player.color = vec4(1);
				player.rigid_body->transform.p = player.position;
				player.rigid_body->transform.xform = mat2_scalar(player.radius, player.radius);
			}

			for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
				if (ptr->data.type == Rigid_Body_Type_Dynamic) {
					ptr->data.velocity += dt * ptr->data.force + vec2(0, -gravity);
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

									array_add(&manifolds, manifold);
								}
							}
						}
					}

				}		
			}

			for (auto &player : scene->by_type.character) {
				player.position = player.rigid_body->transform.p;
			}

			scene->by_type.camera[0].target_position = primary_player->position;

			for (auto &camera : scene->by_type.camera) {
				if (camera.behaviour == Camera_Behaviour_ANIMATE) {
					camera.position = lerp(camera.position, camera.target_position, 1.0f - powf(1.0f - camera.follow_factor, dt));
					camera.distance = lerp(camera.distance, camera.target_distance, 1.0f - powf(1.0f - camera.zoom_factor, dt));
				}
			}

			accumulator_t -= dt;
		}

		for (auto &player : scene->by_type.character) {
			player.rigid_body->force = vec2(0);
		}

		primary_player->color = vec4(0, 1, 1);

		ImGui_UpdateFrame(real_dt);

		Camera &camera = scene->by_type.camera[0];

		r32 view_height = camera.lens.field_of_view;
		r32 view_width = aspect_ratio * view_height;

		Camera_View view;

		if (camera.lens.kind == Camera_View_Kind::ORTHOGRAPHIC) {
			view = orthographic_view(-view_width, view_width, view_height, -view_height, camera.lens.near, camera.lens.far);
		} else {
			view = perspective_view(to_radians(camera.lens.field_of_view), aspect_ratio, camera.lens.near, camera.lens.far);
		}

		im2d_set_stroke_weight(0.02f);

		#if 1
		auto cursor = system_get_cursor_position();
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor = 2.0f * cursor - vec2(1);
		cursor.x *= view_width;
		cursor.y *= view_height;
		cursor += camera.position;

		if (!ImGui_IsUsingCursor()) {
			entity_hovered = nullptr;
			for (auto ptr = iter_begin(&scene->rigid_bodies); 
				iter_continue(&scene->rigid_bodies, ptr) && !entity_hovered; 
				ptr = iter_next<Rigid_Body>(ptr)) {
				auto &body = ptr->data;
				for (u32 index = 0; index < body.fixture_count && !entity_hovered; ++index) {
					Fixture *fixture = rigid_body_get_fixture(&body, index);
					if (test_fixture_point(*fixture, body.transform, cursor)) {
						entity_hovered = scene_find_entity(scene, body.entity_id);
					}
				}
			}

			if (system_button(Button_LEFT) == Key_State_DOWN) {
				entity_selected = entity_hovered;
			}
		}

		#endif

		if (!ImGui_IsUsingCursor()) {
			ImGui::GetStyle().Alpha = 0.4f;
		} else {
			ImGui::GetStyle().Alpha = 1.0f;
		}

		Dev_TimedBlockEnd(Simulation);

		Dev_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / dt; // TODO: Use this

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.05f, 0.05f, 0.05f, 1.0f));
		gfx_viewport(0, 0, window_w, window_h);

		r32 scale = powf(0.5f, camera.distance);

		Mat4 transform;
		if (camera.lens.kind == Camera_View_Kind::ORTHOGRAPHIC) {
			transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera.position, 0.0f));
		} else {
			transform = mat4_translation(vec3(-camera.position, -camera.distance));
		}

		im2d_begin(view, transform);

		for (auto &player : scene->by_type.character) {
			im2d_circle(player.position, player.radius, player.color);
			im2d_line(player.position, player.position + player.rigid_body->velocity, vec4(0, 1.5f, 0));
		}

		im2d_circle(vec2(0), 0.05f, vec4(1.2f, 1.2f, 1.2f));

		for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
			auto &body = ptr->data;

			for (u32 index = 0; index < body.fixture_count; ++index) {
				auto f = rigid_body_get_fixture(&body, index);
				auto color = (body.flags & Rigid_Body_COLLIDING) ? vec3(1, 0, 0) : vec3(0.7f, 0.45f, 0);
				render_shape(*f, body.transform, color);
			}

			Vec4 color = (body.flags & Rigid_Body_BOUNDING_BOX_COLLIDING) ? vec4(0.7f, 0.1f, 0.1f, 1) : vec4(0.1f, 0.7f, 0.1f, 1);

			im2d_rect_outline(body.bounding_box.min, body.bounding_box.max - body.bounding_box.min, color);
		}

		for (auto &m : manifolds) {
			im2d_line(m.contacts[1], m.contacts[1] + m.penetration * m.normal, vec4(1, 0, 1), 0.02f);

			im2d_circle(m.contacts[0], 0.08f, vec4(1, 0, 1));
			im2d_circle(m.contacts[1], 0.08f, vec4(1, 0, 1));
		}
		
		for (auto &m : impact_times) {
			im2d_line(m.contacts[1], m.contacts[1] + 0.5f * m.normal, vec4(1, 0, 1), 0.02f);

			im2d_circle(m.contacts[0], 0.08f, vec4(1, 0, 1));
			im2d_circle(m.contacts[1], 0.08f, vec4(1, 0, 1));
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

#if 1
		//ImGui::ShowDemoWindow();
		if (entity_selected) {
			ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
			editor_entity(entity_selected);
			ImGui::End();
		}
#else	
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

		int sim_index = (int)sim_speed.index;

		ImGui::DragFloat("Gravity", &gravity, 0.01f);
		ImGui::DragFloat("Movement Force", &movement_force, 0.01f);
		editor_draw(camera);
		ImGui::Text("Speed: x%d, Factor: %f", sim_speed.x, sim_speed.factor);
		if (ImGui::DragInt("Speed Index", &sim_index, 1.0f, 0, static_count(SIMULATION_SPEED_FACTORS))) {
			sim_speed = simulation_speed((u32)sim_index);
		}
		ImGui::End();
#endif

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

		game_dt = real_dt * sim_speed.factor;

		accumulator_t += game_dt;
		accumulator_t = minimum(accumulator_t, 0.2f);

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
