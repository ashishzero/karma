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

#include "entity.h"

#include ".generated/entity.typeinfo"
#include "editor.h"

struct Player_Controller {
	r32 x, y;
};

struct Sorted_Colliders {
	Entity_Handle handle;
	r32 distance;
};

struct Quad_Mesh {
	Quad quad;
	Vec2 vertices[4];
};

int karma_user_zero() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	ImGui_Initialize();
	Dev_ModeEnable();

	bool running = true;

	r32 aspect_ratio = framebuffer_w / framebuffer_h;
	const r32 speed_factor = 1;

	r32 const fixed_dt = 1.0f / 30.0f;
	r32       dt = fixed_dt * speed_factor;
	r32       game_dt = fixed_dt * speed_factor;
	r32       real_dt = fixed_dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = fixed_dt;

	r32 window_w = 0, window_h = 0;

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	Entity_Manager manager = manager_create();

	auto player = manager_add_entity(&manager, Player);

	player->position = vec2(-0.2f, 0);
	player->size = vec2(0.5f);
	player->color = vec4(1);
	player->velocity = vec2(0);
	Entity_Handle player_id = player->handle;

	Vec2 quad_position = vec2(1);
	Vec2 scale = vec2(1);

	Quad_Mesh quad_mesh = {};
	quad_mesh.vertices[0] = vec2(-0.5f, -0.5f);
	quad_mesh.vertices[1] = vec2(-0.5f + 1.0f,  0.5f);
	quad_mesh.vertices[2] = vec2( 0.5f + 1.0f,  0.5f);
	quad_mesh.vertices[3] = vec2( 0.5f, -0.5f);

#if 1
	Line *line = nullptr;
	line = manager_add_entity(&manager, Line);
	line->start = vec2(-4, -3);
	line->end = vec2(-4, 2);
	line->color = vec4(1, 0, 0, 1);

	line = manager_add_entity(&manager, Line);
	line->start = vec2(2, 4);
	line->end = vec2(-4, 2);
	line->color = vec4(1, 0, 0, 1);

	line = manager_add_entity(&manager, Line);
	line->start = vec2(5.1f, 4);
	line->end = vec2(2, 4);
	line->color = vec4(1, 0, 0, 1);

	line = manager_add_entity(&manager, Line);
	line->start = vec2(4, 1);
	line->end = vec2(5, 4);
	line->color = vec4(1, 0, 0, 1);

	line = manager_add_entity(&manager, Line);
	line->start = vec2(4, 4);
	line->end = vec2(-5, 0);
	line->color = vec4(1, 0, 0, 1);

	line = manager_add_entity(&manager, Line);
	line->start = vec2(3.5f, -4.1f);
	line->end = vec2(3.5f, 4);
	line->color = vec4(1, 0, 0, 1);

	line = manager_add_entity(&manager, Line);
	line->start = vec2(-4, 3);
	line->end = vec2(3.5f, -4);
	line->color = vec4(1, 0, 0, 1);
#endif

	Player_Controller controller = {};


	Vec3 p[3];
	p[0] = vec3(0.0f, 0.0f, 0.0f);
	p[1] = vec3(0.0f, 0.0f, 0.0f);
	p[2] = vec3(0.0f, 0.0f, 0.0f);

	Ray_Hit hit;
	memset(&hit, 0, sizeof(hit));

	while (running) {
		Dev_TimedFrameBegin();

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

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_SPACE) {
				Dev_NotifySuccess("Sent success message");
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

		}

		Dev_TimedBlockEnd(EventHandling);

		Dev_TimedBlockBegin(Simulation);

#if 1
		//Array<Vec2> normals;
		Array<r32> t_values;
		//ray_hits.allocator = TEMPORARY_ALLOCATOR;
		//normals.allocator = TEMPORARY_ALLOCATOR;
		t_values.allocator = TEMPORARY_ALLOCATOR;

		player = manager_find_player(manager, player_id);

		//Vec2 prev_velocity = player->velocity;
		//only physics code
		static int test_counter = 0;

		while (accumulator_t >= fixed_dt) {
			Dev_TimedScope(SimulationFrame);

			memset(&hit, 0, sizeof(hit));

			const r32 gravity = 10;
			const r32 drag = 5;

			player->force = vec2(0);

			r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
			Vec2 dir;
			if (len) {
				dir.x = controller.x / len;
				dir.y = controller.y / len;
			} else {
				dir = vec2(0);
			}

			const float force = 50;

			player->force = force * dir;
			//player->force.y -= gravity;

			player->velocity += dt * player->force;
			player->velocity *= powf(0.5f, drag * dt);

			auto new_player_velocity = player->velocity;
			auto new_player_position = player->position + dt * new_player_velocity;


			Array<Sorted_Colliders> sorted_colliders;
			sorted_colliders.allocator = TEMPORARY_ALLOCATOR;

			for (auto &entity : manager.by_type.lines) {
				auto collider = array_add(&sorted_colliders);
				r32 dx = entity.end.x - entity.start.x;
				r32 dy = entity.end.y - entity.start.x;
				r32 d = dx * dx + dy * dy;
				r32 n = dy * player->position.x - dx * player->position.y + entity.end.x * entity.start.y - entity.end.y * entity.start.x;
				n *= n;
				collider->handle = entity.handle;
				collider->distance = n / d;
			}

			sort(sorted_colliders.data, sorted_colliders.count, [](Sorted_Colliders &a, Sorted_Colliders &b) {
				return a.distance < b.distance;
			});

			//collision_point_vs_line(sorted_colliders, manager, player, new_player_position, dt);
			Vec2 get_corner[4] = { {player->size.x / 2,player->size.y / 2},
				{-player->size.x / 2,-player->size.y / 2},
				{player->size.x / 2,-player->size.y / 2},
				{-player->size.x / 2,player->size.y / 2} };
			//collided handle
			Entity_Handle handle_save = INVALID_ENTITY_HANDLE;
			for (auto &collider : sorted_colliders) {
				auto entity = manager_find_line(manager, collider.handle);
				//entity->color = vec4(1, 0, 0);
				bool collision_found = false;
				for (Vec2 temp : get_corner) {
					if (ray_vs_line(player->position + temp, new_player_position + temp, entity->start, entity->end, &hit)) {
						r32 dir = vec2_dot(vec2_normalize_check(player->velocity), hit.normal);

						if (dir <= 0 && hit.t >= -0.001f && hit.t < 1.001f) {
							array_add(&t_values, hit.t);
							//array_add(&normals, hit.point);

							Vec2 reduc_vector = (1.0f - hit.t) * vec2_dot(player->velocity, hit.normal) * hit.normal;
							//array_add(&normals, -reduc_vector);

							player->velocity -= reduc_vector;
							new_player_position = player->position + dt * player->velocity;
							handle_save = entity->handle;

							//entity->color = vec4(1, 0, 1);
							collision_found = true;
						}
					}
				}
				if (collision_found)
					break;
			}

			Line *line_collided = nullptr;
			if (handle_save != INVALID_ENTITY_HANDLE) {
				line_collided = manager_find_line(manager, handle_save);

				for (auto &collider : sorted_colliders) {
					auto *entity = manager_find_line(manager, collider.handle);
					if (entity->handle != handle_save) {
						for (Vec2 temp : get_corner) {
							if (ray_vs_line(player->position + temp, new_player_position + temp, entity->start, entity->end, &hit)) {
								r32 dir = vec2_dot(vec2_normalize_check(player->velocity), hit.normal);
								if (dir <= 0 && hit.t >= -0.001f && hit.t < 1.001f) {

									r32 dot = vec2_dot(vec2_normalize(line_collided->end - line_collided->start),
										vec2_normalize(entity->end - entity->start));

									if (dot <= 0.0f) {
										Vec2 reduc_vector = (1.0f - hit.t) * player->velocity;
										player->velocity -= reduc_vector;
										new_player_position = player->position + dt * player->velocity;
										break;
									} else {
										Vec2 reduc_vector = (1.0f - hit.t) * vec2_dot(player->velocity, hit.normal) * hit.normal;
										player->velocity -= reduc_vector;
										new_player_position = player->position + dt * player->velocity;
									}

									//entity->color = vec4(1, 0, 1);
								}
							}
						}
					}
				}
			}

#endif

			{
				auto &positions = quad_mesh.quad.positions;
				auto &vertices = quad_mesh.vertices;

				positions[0] = vec2_hadamard(vertices[0], scale) + quad_position;
				positions[1] = vec2_hadamard(vertices[1], scale) + quad_position;
				positions[2] = vec2_hadamard(vertices[2], scale) + quad_position;
				positions[3] = vec2_hadamard(vertices[3], scale) + quad_position;

				quad_mesh.quad.normals[0] = vec2(positions[0].y - positions[1].y, positions[1].x - positions[0].x);
				quad_mesh.quad.normals[1] = vec2(positions[1].y - positions[2].y, positions[2].x - positions[1].x);
				quad_mesh.quad.normals[2] = vec2(positions[2].y - positions[3].y, positions[3].x - positions[2].x);
				quad_mesh.quad.normals[3] = vec2(positions[3].y - positions[0].y, positions[0].x - positions[3].x);
			}

			Quad player_quad;
			player_quad.normals[0] = vec2(-1, 0);
			player_quad.normals[1] = vec2(0, 1);
			player_quad.normals[2] = vec2(1, 0);
			player_quad.normals[3] = vec2(0, -1);

			player_quad.positions[0] = player->position + 0.5f * vec2(-player->size.x, -player->size.y);
			player_quad.positions[1] = player->position + 0.5f * vec2(-player->size.x,  player->size.y);
			player_quad.positions[2] = player->position + 0.5f * vec2( player->size.x,  player->size.y);
			player_quad.positions[3] = player->position + 0.5f * vec2( player->size.x, -player->size.y);

			if (test_quad_vs_quad(player_quad, quad_mesh.quad)) {
				player->color = vec4(1, 0, 0);
			} else {
				player->color = vec4(1, 1, 1);
			}

			player->position += dt * player->velocity;

			accumulator_t -= fixed_dt;
		}

		ImGui_UpdateFrame(real_dt);

#if 0
		Vec2 cursor = system_get_cursor_position();
		if (cursor.x < 0) cursor.x = 0;
		if (cursor.y < 0) cursor.y = 0;
		if (cursor.x > framebuffer_w) cursor.x = framebuffer_w;
		if (cursor.y > framebuffer_h) cursor.y = framebuffer_h;
		cursor.x /= framebuffer_w;
		cursor.y /= framebuffer_h;
		cursor.x *= 2;
		cursor.y *= 2;
		cursor.x -= 1;
		cursor.y -= 1;
#endif

		r32 view_height = 5.0f;
		r32 view_width = aspect_ratio * view_height;

		auto view = orthographic_view(-view_width, view_width, view_height, -view_height);

#if 1
		auto cursor = system_get_cursor_position();
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor = 2.0f * cursor - vec2(1);
		cursor.x *= view_width;
		cursor.y *= view_height;

		if (ImGui_IsUsingCursor()) {
			cursor.x = INFINITY;
			cursor.y = INFINITY;
		}

		static auto last_button_state = Key_State_UP;
		static Entity *selected_entity = nullptr;
		Entity *hovered_entity = nullptr;

		auto new_button_state = system_button(Button_LEFT);
		if (new_button_state == Key_State_DOWN && last_button_state == Key_State_UP) {
			for (auto &entity : manager.entities) {
				if (entity.kind == Entity_Player) {
					auto e = entity_down(manager, entity, Player);
					Mm_Rect rect;
					rect.min = e->position - e->size * 0.5f;
					rect.max = e->position + e->size * 0.5f;
					if (test_point_inside_rect(cursor, rect)) {
						selected_entity = e;
						break;
					}
				} else if (entity.kind == Entity_Line) {
					auto e = entity_down(manager, entity, Line);
					r32 dx = e->end.x - e->start.x;
					r32 dy = e->end.y - e->start.y;
					r32 v = dx * (cursor.y - e->start.y) - dy * (cursor.x - e->start.x);
					if (fabsf(v) < 0.5f) {
						selected_entity = e;
						break;
					}
				}
			}
		} else {
			for (auto &entity : manager.entities) {
				if (entity.kind == Entity_Player) {
					Mm_Rect rect;
					auto e = entity_down(manager, entity, Player);
					rect.min = e->position - e->size * 0.5f;
					rect.max = e->position + e->size * 0.5f;
					if (test_point_inside_rect(cursor, rect)) {
						hovered_entity = e;
						break;
					}
				} else if (entity.kind == Entity_Line) {
					auto e = entity_down(manager, entity, Line);
					r32 dx = e->end.x - e->start.x;
					r32 dy = e->end.y - e->start.y;
					r32 v = dx * (cursor.y - e->start.y) - dy * (cursor.x - e->start.x);
					if (fabsf(v) < 0.5f) {
						hovered_entity = e;
						break;
					}
				}
			}
		}

		last_button_state = new_button_state;

		//Mat4 view_inv = mat4_inverse(gfx_view_transform(view));
		//cursor = (view_inv * vec4(cursor, 0, 1)).xy;
		//cursor.x = roundf(cursor.x);
		//cursor.y = roundf(cursor.y);
#endif

		Dev_TimedBlockEnd(Simulation);

		Dev_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.0f));
		gfx_viewport(0, 0, window_w, window_h);

#if 1
		im2d_begin(view);

		im2d_quad(quad_mesh.quad.positions[0], 
			quad_mesh.quad.positions[1], 
			quad_mesh.quad.positions[2], 
			quad_mesh.quad.positions[3],
			vec4(1, 0, 1));

		for (auto &player : manager.by_type.players) {
			im2d_rect_centered(player.position, player.size, player.color);
		}

		im2d_line(player->position, player->position + player->velocity, vec4(0, 1, 0), 0.03f);

		for (auto &line : manager.by_type.lines) {
			im2d_line(line.start, line.end, line.color, 0.01f);
		}

		if (hovered_entity) {
			switch (hovered_entity->kind) {
			case Entity_Player:
			{
				Player *e = (Player *)hovered_entity;
				im2d_rect_centered_outline(e->position, e->size, 1.1f * vec4(1, 1, 0), 0.03f);
			} break;

			case Entity_Line:
			{
				Line *e = (Line *)hovered_entity;
				im2d_line(e->start, e->end, 1.1f * vec4(1, 1, 0), 0.03f);
			} break;
			}
		}

		//for (auto& h : rayhits) {
		//	im2d_circle(h.point, 0.08f, vec4(0, 0, 1));
		//	im2d_line(h.point, h.point + h.normal, vec4(1), 0.01f);
		//}

		im2d_end();
#endif

		im3d_begin(perspective_view(to_radians(30.0f), aspect_ratio, 0.01f, 100.0f), mat4_identity());

		im3d_triangle(p[0], p[1], p[2], vec4(1));

		im3d_end();

		gfx_end_drawing();

		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

#if defined(BUILD_IMGUI)

		ImGui::Begin("Editor");
		if (selected_entity) {
			switch (selected_entity->kind) {
			case Entity_Player:
				editor_draw(*(Player *)selected_entity);
				break;
			case Entity_Line:
				editor_draw(*(Line *)selected_entity);
				break;
			}
			
		}
		ImGui::End();

#endif

#if defined(BUILD_DEVELOPER_SERVICE)
		{
			Dev_TimedScope(DebugRender);
			Dev_RenderFrame();
		}
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
