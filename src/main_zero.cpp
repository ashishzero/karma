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

typedef bool(*Collision_Resolver)(Collider &a, Collider &b, const Mat3 &ta, const Mat3 &tb, const Mat2 &tdira, const Mat2 &tdirb, Vec2 dp, Vec2 *normal, r32 *penetration);
static Collision_Resolver COLLISION_RESOLVERS[Collider_Count][Collider_Count];

bool null_collision_resolver(Collider &a, Collider &b, const Mat3 &ta, const Mat3 &tb, const Mat2 &tdira, const Mat2 &tdirb, Vec2 dp, Vec2 *normal, r32 *penetration) {
	return false;
}

template <typename ShapeA, typename ShapeB>
bool shapes_collision_resolver(Collider &a, Collider &b, const Mat3 &ta, const Mat3 &tb, const Mat2 &tdira, const Mat2 &tdirb, Vec2 dp, Vec2 *normal, r32 *penetration) {
	return gjk_epa(*(ShapeA *)a.handle, *(ShapeB *)b.handle, normal, penetration, ta, tb, tdira, tdirb, dp);
}

void collision_resover_init() {
	COLLISION_RESOLVERS[Collider_Null][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Circle] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Mm_Rect] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Capsule] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Polygon] = null_collision_resolver;

	COLLISION_RESOLVERS[Collider_Circle][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Circle] = shapes_collision_resolver<Circle, Circle>;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Mm_Rect] = shapes_collision_resolver<Circle, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Capsule] = shapes_collision_resolver<Circle, Capsule>;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Polygon] = shapes_collision_resolver<Circle, Polygon>;

	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Circle] = shapes_collision_resolver<Mm_Rect, Circle>;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Mm_Rect] = shapes_collision_resolver<Mm_Rect, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Capsule] = shapes_collision_resolver<Mm_Rect, Capsule>;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Polygon] = shapes_collision_resolver<Mm_Rect, Polygon>;

	COLLISION_RESOLVERS[Collider_Capsule][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Circle] = shapes_collision_resolver<Capsule, Circle>;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Mm_Rect] = shapes_collision_resolver<Capsule, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Capsule] = shapes_collision_resolver<Capsule, Capsule>;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Polygon] = shapes_collision_resolver<Capsule, Polygon>;

	COLLISION_RESOLVERS[Collider_Polygon][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Circle] = shapes_collision_resolver<Polygon, Circle>;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Mm_Rect] = shapes_collision_resolver<Polygon, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Capsule] = shapes_collision_resolver<Polygon, Capsule>;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Polygon] = shapes_collision_resolver<Polygon, Polygon>;
}

bool collider_vs_collider_dynamic(Collider &a, Collider &b, const Mat3 &ta, const Mat3 &tb, Vec2 dp, Vec2 *normal, r32 *penetration) {
	Mat2 tdira, tdirb;
	tdira.rows[0] = vec2(ta.m2[0][0], ta.m2[1][0]);
	tdira.rows[1] = vec2(ta.m2[0][1], ta.m2[1][1]);

	tdirb.rows[0] = vec2(tb.m2[0][0], tb.m2[1][0]);
	tdirb.rows[1] = vec2(tb.m2[0][1], tb.m2[1][1]);
	return COLLISION_RESOLVERS[a.type][b.type](a, b, ta, tb, tdira, tdirb, dp, normal, penetration);
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

	Collider collider;
	Raw_Collider_Id id;

	{
		Circle circle;
		circle.center = vec2(0);
		circle.radius = 1;
		
		collider.type = Collider_Circle;
		collider.handle = &circle;
		id = scene_add_raw_collider_group(scene, Array_View(&collider, 1));

		Player *player = scene_add_player(scene);
		scene_generate_new_entity(scene, player, vec2(5));
		player->collider_id = id;
		player->rigid_body->colliders = scene_create_colliders(scene, player, player->collider_id, mat3_identity());

		player = scene_add_player(scene);
		scene_generate_new_entity(scene, player, vec2(0, 5));
		player->collider_id = id;
		player->rigid_body->colliders = scene_create_colliders(scene, player, player->collider_id, mat3_identity());
	}

	{
		Static_Body *object;
		Mat3 trans;

		{
			Vec2 points[] = {
				vec2(-2.4f, 4.6f), vec2(3.6f, 4.6f), vec2(4.6f, -1.4f), vec2(1.6f, -5.4f), vec2(-7.4f, -2.4f)
			};
			assert(static_count(points) >= 3);

			auto polygon = (Polygon *)tallocate(sizeof(Polygon) + sizeof(Vec2) * (static_count(points) - 3));
			polygon->vertex_count = static_count(points);
			memcpy(polygon->vertices, points, sizeof(points));
			collider.type = Collider_Polygon;
			collider.handle = polygon;
			id = scene_add_raw_collider_group(scene, Array_View(&collider, 1));

			object = scene_add_static_body(scene);
			scene_generate_new_entity(scene, object, vec2(-5.6f, 0.4f));
			trans = mat3_translation(-5.6f, 0.4f);
			object->id = id;
			object->colliders = scene_create_colliders(scene, object, id, trans);
		}

		{
			Circle circle;
			circle.center = vec2(0);
			circle.radius = 0.6f;
			collider.type = Collider_Circle;
			collider.handle = &circle;
			id = scene_add_raw_collider_group(scene, Array_View(&collider, 1));

			object = scene_add_static_body(scene);
			scene_generate_new_entity(scene, object, vec2(1));
			trans = mat3_translation(1, 1);
			object->id = id; 
			object->colliders = scene_create_colliders(scene, object, id, trans);
		}

		{
			Mm_Rect rect;
			rect.min = vec2(-2.5f, -3.5f);
			rect.max = vec2(2.5f, 3.5f);
			collider.type = Collider_Mm_Rect;
			collider.handle = &rect;
			id = scene_add_raw_collider_group(scene, Array_View(&collider, 1));

			object = scene_add_static_body(scene);
			scene_generate_new_entity(scene, object, vec2(6.5f, -0.5f));
			trans = mat3_translation(6.5f, -0.5f) * mat3_rotation(to_radians(10));
			object->id = id; 
			object->colliders = scene_create_colliders(scene, object, id, trans);
		}

		{
			Circle circle;
			circle.center = vec2(1, -1);
			circle.radius = 1;

			Capsule capsule;
			capsule.a = vec2(-2, -3);
			capsule.b = vec2(2, 3);
			capsule.radius = 1;

			Collider c[2];
			c[0].type = Collider_Circle;
			c[0].handle = &circle;
			c[1].type = Collider_Capsule;
			c[1].handle = &capsule;

			id = scene_add_raw_collider_group(scene, Array_View(c, static_count(c)));

			object = scene_add_static_body(scene);
			scene_generate_new_entity(scene, object, vec2(-1, -5));
			trans = mat3_translation(-1, -5);
			object->id = id;
			object->colliders = scene_create_colliders(scene, object, id, trans);
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

			const float movement_force = 20;
			if (len) {
				primary_player->rigid_body->force = movement_force * dir;
				set_bit(primary_player->rigid_body->colliders->flags, Collision_Bit_MOTION);
			}

			for (auto &player : scene->by_type.player) {
				player.color = vec4(1);
				player.rigid_body->colliders->transform = mat3_translation(player.position) * mat3_scalar(player.radius, player.radius);
			}

			primary_player->color = vec4(0, 1, 1);

			Vec2 norm; r32 dist, v_t;

			for (auto collider_node = scene->colliders.node.next; collider_node != &scene->colliders.node; collider_node = collider_node->next) {
				clear_bit(collider_node->data.flags, Collision_Bit_OCUURED);
			}

			// TODO: Do broad phase collision detection

			// Dynamic collision resolution
			for (auto a = scene->rigid_bodies.node.next; a != &scene->rigid_bodies.node; a = a->next) {
				if (!get_bit(a->data.colliders->flags, Collision_Bit_MOTION)) continue;
				
				a->data.velocity += dt * a->data.force;
				a->data.velocity *= powf(0.5f, drag * dt);
				a->data.force = vec2(0);

				for (auto b = scene->rigid_bodies.node.next; b != &scene->rigid_bodies.node; b = b->next) {
					if (a == b) continue;

					Rigid_Body &a_body = a->data;
					Rigid_Body &b_body = b->data;

					Vec2 relative_velocity = a_body.velocity - b_body.velocity;
					Vec2 dp = dt * relative_velocity;

					for (u32 a_index = 0; a_index < a_body.colliders->count; ++a_index) {
						auto a_collider = collider_get(a_body.colliders, a_index);
						for (u32 b_index = 0; b_index < b_body.colliders->count; ++b_index) {
							auto b_collider = collider_get(b_body.colliders, b_index);

							if (collider_vs_collider_dynamic(*b_collider, *a_collider, 
															 b_body.colliders->transform, a_body.colliders->transform, 
															 dp, &norm, &dist)) {
								v_t = dist / dt * sgn(vec2_dot(norm, relative_velocity));
								v_t = 0.5f * v_t;
								a_body.velocity -= v_t * norm;
								b_body.velocity += v_t * norm;
								relative_velocity = a_body.velocity - b_body.velocity;
								dp = dt * relative_velocity;
								a_body.colliders->flags |= Collision_Bit_OCUURED;
								b_body.colliders->flags |= (Collision_Bit_OCUURED | Collision_Bit_MOTION);
							}
						}
					}

				}

				if (vec2_null(a->data.velocity)) {
					a->data.velocity = vec2(0);
					clear_bit(a->data.colliders->flags, Collision_Bit_MOTION);
				}

			}

			// Static collision resolution
			for (auto ptr = scene->rigid_bodies.node.next; ptr != &scene->rigid_bodies.node; ptr = ptr->next) {
				auto &body = ptr->data;
				if (!get_bit(body.colliders->flags, Collision_Bit_MOTION)) continue;

				Vec2 dp = body.velocity * dt;
				for (u32 b_collider_index = 0; b_collider_index < body.colliders->count; ++b_collider_index) {
					auto b_collider = collider_get(body.colliders, b_collider_index);

					for (auto collider_node = scene->colliders.node.next; collider_node != &scene->colliders.node; collider_node = collider_node->next) {
						if (body.colliders->entity_id == collider_node->data.entity_id) continue;

						for (u32 a_collider_index = 0; a_collider_index < collider_node->data.count; ++a_collider_index) {
							auto a_collider = collider_get(&collider_node->data, a_collider_index);

							if (collider_vs_collider_dynamic(*a_collider, *b_collider, collider_node->data.transform, body.colliders->transform, dp, &norm, &dist)) {
								v_t = dist / dt * sgn(vec2_dot(norm, body.velocity));
								body.velocity -= v_t * norm;
								dp = body.velocity * dt;
								body.colliders->flags |= Collision_Bit_OCUURED;
								collider_node->data.flags |= Collision_Bit_OCUURED;
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

		for (auto ptr = scene->colliders.node.next; ptr != &scene->colliders.node; ptr = ptr->next) {
			auto &group = ptr->data;

			for (u32 index = 0; index < group.count; ++index) {
				im2d_push_matrix(mat3_to_mat4(group.transform));

				auto c = collider_get(&group, index);
				auto color = (group.flags & Collision_Bit_OCUURED) ? vec4(1, 0, 0) : vec4(0, 1, 1);

				switch (c->type) {
				case Collider_Null: {

				} break;

				case Collider_Circle: {
					auto circle = collider_get_shape(c, Circle);
					im2d_circle_outline(circle->center, circle->radius, color, 0.02f);
				} break;

				case Collider_Polygon: {
					auto polygon = collider_get_shape(c, Polygon);
					for (u32 i = 0; i < polygon->vertex_count; ++i) {
						im2d_line(polygon->vertices[i], polygon->vertices[(i + 1) % polygon->vertex_count], color, 0.02f);
					}
				} break;

				case Collider_Mm_Rect: {
					auto rect = collider_get_shape(c, Mm_Rect);
					im2d_rect_outline(rect->min, rect->max - rect->min, color, 0.02f);
				} break;

				case Collider_Capsule: {
					auto capsule = collider_get_shape(c, Capsule);
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
