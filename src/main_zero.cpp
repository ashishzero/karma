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

typedef bool(*Collision_Resolver)(Fixture &a, Fixture &b, const Mat3 &ta, const Mat3 &tb, const Mat2 &tdira, const Mat2 &tdirb, Vec2 dp, Collision_Manifold *manifold);
static Collision_Resolver COLLISION_RESOLVERS[Fixture_Shape_Count][Fixture_Shape_Count];

bool null_collision_resolver(Fixture &a, Fixture &b, const Mat3 &ta, const Mat3 &tb, const Mat2 &tdira, const Mat2 &tdirb, Vec2 dp, Collision_Manifold *manifold) {
	return false;
}

template <typename ShapeA, typename ShapeB>
bool shapes_collision_resolver(Fixture &a, Fixture &b, const Mat3 &ta, const Mat3 &tb, const Mat2 &tdira, const Mat2 &tdirb, Vec2 dp, Collision_Manifold *manifold) {
	return gjk_epa(*(ShapeA *)a.handle, *(ShapeB *)b.handle, manifold, ta, tb, tdira, tdirb, dp);
}

void collision_resover_init() {
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Circle] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Mm_Rect] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Capsule] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Null][Fixture_Shape_Polygon] = null_collision_resolver;

	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Circle] = shapes_collision_resolver<Circle, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Circle, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Capsule] = shapes_collision_resolver<Circle, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Circle][Fixture_Shape_Polygon] = shapes_collision_resolver<Circle, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Circle] = shapes_collision_resolver<Mm_Rect, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Mm_Rect, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Capsule] = shapes_collision_resolver<Mm_Rect, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Mm_Rect][Fixture_Shape_Polygon] = shapes_collision_resolver<Mm_Rect, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Circle] = shapes_collision_resolver<Capsule, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Capsule, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Capsule] = shapes_collision_resolver<Capsule, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Capsule][Fixture_Shape_Polygon] = shapes_collision_resolver<Capsule, Polygon>;

	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Circle] = shapes_collision_resolver<Polygon, Circle>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Mm_Rect] = shapes_collision_resolver<Polygon, Mm_Rect>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Capsule] = shapes_collision_resolver<Polygon, Capsule>;
	COLLISION_RESOLVERS[Fixture_Shape_Polygon][Fixture_Shape_Polygon] = shapes_collision_resolver<Polygon, Polygon>;
}

bool collider_vs_collider_dynamic(Fixture &a, Fixture &b, const Mat3 &ta, const Mat3 &tb, Vec2 dp, Collision_Manifold *manifold) {
	Mat2 tdira, tdirb;
	tdira.rows[0] = vec2(ta.m2[0][0], ta.m2[1][0]);
	tdira.rows[1] = vec2(ta.m2[0][1], ta.m2[1][1]);

	tdirb.rows[0] = vec2(tb.m2[0][0], tb.m2[1][0]);
	tdirb.rows[1] = vec2(tb.m2[0][1], tb.m2[1][1]);
	return COLLISION_RESOLVERS[a.type][b.type](a, b, ta, tb, tdira, tdirb, dp, manifold);
}

int karma_user_zero() {
	collision_resover_init();

#ifdef INIT_THREAD_POOL
	if (!async_initialize(2, mega_bytes(32), context.allocator)) {
		system_fatal_error("Thread could not be created");
	}
#endif

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
	scene->camera.type = Entity_Camera;

	Fixture fixture;
	Resource_Id id;

	Entity_Info info;
	
	{
		Circle circle;
		circle.center = vec2(0);
		circle.radius = 1;
		
		fixture.type = Fixture_Shape_Circle;
		fixture.handle = &circle;
		id = scene_create_new_resource_fixture(scene, &fixture, 1);

		info.position = vec2(5);
		info.rigid_body.type = Rigid_Body_Dynamic;
		info.rigid_body.fixture_id = id;
		info.rigid_body.xform = mat3_identity();

		scene_create_new_entity(scene, Entity_Player, info);

		info.position = vec2(-5, 8);
		scene_create_new_entity(scene, Entity_Player, info);

		Mm_Rect rect;
		rect.min = vec2(-1);
		rect.max = vec2( 1);

		fixture.type = Fixture_Shape_Mm_Rect;
		fixture.handle = &rect;
		id = scene_create_new_resource_fixture(scene, &fixture, 1);

		info.position = vec2(0, 5);
		info.rigid_body.fixture_id = id;
		scene_create_new_entity(scene, Entity_Player, info);
	}

	{
		info.rigid_body.type = Rigid_Body_Static;

		{
			Vec2 points[] = {
				vec2(-2.4f, 4.6f), vec2(3.6f, 4.6f), vec2(4.6f, -1.4f), vec2(1.6f, -5.4f), vec2(-7.4f, -2.4f)
			};
			assert(static_count(points) >= 3);

			auto polygon = (Polygon *)tallocate(sizeof(Polygon) + sizeof(Vec2) * (static_count(points) - 3));
			polygon->vertex_count = static_count(points);
			memcpy(polygon->vertices, points, sizeof(points));
			fixture.type = Fixture_Shape_Polygon;
			fixture.handle = polygon;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(-5.6f, 0.4f);
			info.rigid_body.xform = mat3_translation(info.position);
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Static_Body, info);
		}

		{
			Circle circle;
			circle.center = vec2(0);
			circle.radius = 0.6f;
			fixture.type = Fixture_Shape_Circle;
			fixture.handle = &circle;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(1);
			info.rigid_body.xform = mat3_translation(info.position);
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Static_Body, info);
		}

		{
			Mm_Rect rect;
			rect.min = vec2(-2.5f, -3.5f);
			rect.max = vec2(2.5f, 3.5f);
			fixture.type = Fixture_Shape_Mm_Rect;
			fixture.handle = &rect;
			id = scene_create_new_resource_fixture(scene, &fixture, 1);

			info.position = vec2(6.5f, -0.5f);
			info.rigid_body.xform = mat3_translation(info.position);
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Static_Body, info);
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
			f[0].type = Fixture_Shape_Circle;
			f[0].handle = &circle;
			f[1].type = Fixture_Shape_Capsule;
			f[1].handle = &capsule;

			id = scene_create_new_resource_fixture(scene, f, static_count(f));

			info.position = vec2(-1, -5);
			info.rigid_body.xform = mat3_translation(info.position);
			info.rigid_body.fixture_id = id;
			scene_create_new_entity(scene, Entity_Static_Body, info);
		}
	}

	Player_Controller controller = {};

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

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
					gfx_resize_framebuffer(w, h);
					aspect_ratio = window_w / window_h;
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

		while (accumulator_t >= fixed_dt) {
			Dev_TimedScope(SimulationFrame);

			const r32 gravity = 10;
			const r32 drag = 5;

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

			for (auto &player : scene->by_type.player) {
				player.color = vec4(1);
				player.rigid_body->xform = mat3_translation(player.position) * mat3_scalar(player.radius, player.radius);
			}

			primary_player->color = vec4(0, 1, 1);

			// TODO: Do broad phase collision detection
			for (auto ptr = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, ptr); ptr = iter_next<Rigid_Body>(ptr)) {
				ptr->data.velocity += dt * ptr->data.force;
				ptr->data.velocity *= powf(0.5f, drag * dt);
				ptr->data.force = vec2(0);
				clear_bit(ptr->data.flags, Rigid_Body_COLLIDING);
			}

			Vec2 dv, dp;
			Collision_Manifold manifold;

			for (auto a = iter_begin(&scene->rigid_bodies); iter_continue(&scene->rigid_bodies, a); a = iter_next<Rigid_Body>(a)) {
				Rigid_Body &a_body = a->data;
				if (a_body.type == Rigid_Body_Static) continue;
				for (auto b = a->next; iter_continue(&scene->rigid_bodies, b); b = iter_next<Rigid_Body>(b)) {
					if (a == b) continue;

					Rigid_Body &b_body = b->data;

					dv = a_body.velocity - b_body.velocity;
					dp = dv * dt;

					for (u32 b_index = 0; b_index < b_body.fixture_count; ++b_index) {
						Fixture &b_collider = *rigid_body_get_fixture(&b_body, b_index);
						for (u32 a_index = 0; a_index < a_body.fixture_count; ++a_index) {
							Fixture &a_collider = *rigid_body_get_fixture(&a_body, a_index);

							if (collider_vs_collider_dynamic(b_collider, a_collider, b_body.xform, a_body.xform, dp, &manifold)) {

								r32 collision_time = manifold.penetration / dt * sgn(vec2_dot(manifold.normal, dv));
								Vec2 vn, vt;

								if (b_body.type == Rigid_Body_Dynamic) {
									r32 a_collision_time =  0.5f * collision_time;
									r32 b_collision_time = -0.5f * collision_time;

									vn = a_collision_time * manifold.normal;
									vt = a_body.velocity - vn;
									a_body.velocity = vt - 0.5f * vn;

									vn = b_collision_time * manifold.normal;
									vt = b_body.velocity - vn;
									b_body.velocity = vt - 0.5f * vn;

									dv = a_body.velocity - b_body.velocity;
									dp = dv * dt;
								} else {
									vn = collision_time * manifold.normal;
									vt = a_body.velocity - vn;
									a_body.velocity = vt;

									dv = a_body.velocity;
									dp = dv * dt;
								}

								b_body.flags |= Rigid_Body_COLLIDING;
								a_body.flags |= Rigid_Body_COLLIDING;
							}
						}
					}
				}
			}

			for (auto &player : scene->by_type.player) {
				player.position += dt * player.rigid_body->velocity;
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

		if (ImGui_IsUsingCursor()) {
			cursor.x = 0;
			cursor.y = 0;
		}
#endif

		Dev_TimedBlockEnd(Simulation);

		Dev_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.0f));
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
				im2d_push_matrix(mat3_to_mat4(body.xform));

				auto f = rigid_body_get_fixture(&body, index);
				auto color = (body.flags & Rigid_Body_COLLIDING) ? vec4(1, 0, 0) : vec4(0, 1, 1);

				switch (f->type) {
				case Fixture_Shape_Null: {

				} break;

				case Fixture_Shape_Circle: {
					auto circle = fixture_get_shape(f, Circle);
					im2d_circle_outline(circle->center, circle->radius, color, 0.02f);
				} break;

				case Fixture_Shape_Polygon: {
					auto polygon = fixture_get_shape(f, Polygon);
					for (u32 i = 0; i < polygon->vertex_count; ++i) {
						im2d_line(polygon->vertices[i], polygon->vertices[(i + 1) % polygon->vertex_count], color, 0.02f);
					}
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

		im2d_end();


		gfx_end_drawing();


		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

#if defined(BUILD_DEVELOPER_SERVICE)
		{
			Dev_TimedScope(DebugRender);
			Dev_RenderFrame();
		}
#endif

		editor_entity(primary_player);
		
		ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::DragFloat("Movement Force", &movement_force, 0.01f);
		editor_draw(scene->camera);
		ImGui::End();

		ImGui::ShowDemoWindow();

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
	}

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
