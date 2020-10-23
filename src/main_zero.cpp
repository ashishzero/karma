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

void test_print(void *param) {
	const char *string = (const char *)param;
	system_log(LOG_INFO, "Task", "ThreadId:%5zu %s. ", context.id, string);
}

struct Async_Serialize {
	const Type_Info *info;
	void *data;
	const char *file;
	Allocator allocator;
};

void test_serialize(void *param) {
	Async_Serialize *work = (Async_Serialize *)param;
	
	Ostream out;
	out.allocator = work->allocator;
	serialize_fmt_text(&out, "test", work->info, (char *)work->data);
	
	defer{
		ostream_free(&out);
	};

	System_File file;
	if (!system_open_file(String(work->file, strlen(work->file)), File_Operation_NEW, &file)) {
		system_log(LOG_ERROR, "Serialize", "File: %s could not be opened!", work->file);
		return;
	}
	ostream_build_out_file(&out, &file);
	system_close_file(&file);

	system_log(LOG_INFO, "Serialization", "Written file: %s", work->file);
}

void test_deserialize(void *param) {
	Async_Serialize *work = (Async_Serialize *)param;

	Player *player = (Player *)work->data;

	player->position = vec2(-0.2f, 0);
	player->size = vec2(0.5f);
	player->color = vec4(1);
	player->velocity = vec2(0);

	auto allocator = context.allocator;
	context.allocator = work->allocator;
	defer{
		context.allocator = allocator;
	};

	String content = system_read_entire_file(String(work->file, strlen(work->file)));
	if (!content.count) {
		system_log(LOG_ERROR, "Deserialize", "File: %s could not be opened!", work->file);
		return;
	}

	Tokenization_Status status;
	auto tokens = tokenize(content, &status);
	if (status.result != Tokenization_Result_SUCCESS) {
		system_log(LOG_ERROR, "Deserialize", "File: %s could not be tokenized [%zd, %zd]", status.row, status.column);
		return;
	}

	Deserialize_Error_Info error;
	if (!deserialize_fmt_text(tokens, "test", work->info, (char *)work->data, 1, &error)) {
		system_log(LOG_ERROR, "Deserialization", "Deserialization Failed: %s, Expected: %s (%zd, %zd)", 
			work->file, enum_string(error.expected).data, error.token.row, error.token.column);
		return;
	}

	memory_free(tokens.data);
	memory_free(content.data);
	system_log(LOG_INFO, "Deserialization", "Loaded from file: %s", work->file);
}

int karma_user_zero() {

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

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	Circle player;
	player.center = vec2(0);
	player.radius = 1;

	auto player_color = vec4(1);
	auto player_velocity = vec2(0);

	static Vec2 points[] = {
		vec2(-8, 5), vec2(-2, 5), vec2(-1, -1), vec2(-4, -5), vec2(-13, -2)
	};

	Polygon polygon;
	polygon.vertices = points;
	polygon.vertex_count = static_count(points);

	Mm_Rect rect;
	rect.min = vec2(4, -4);
	rect.max = vec2(9, 3);

	Circle circle;
	circle.center = vec2(5);
	circle.radius = 1.23f;

	Capsule2d capsule;
	capsule.a = vec2(-3, -8);
	capsule.b = vec2(1, -2);
	capsule.radius = 1;

	Vec4 rect_color = vec4(1, 0, 0 ), poly_color = vec4(1, 0, 0), circle_color = vec4(1, 0, 0), capsule_color = vec4(1, 0, 0);

	Player_Controller controller = {};

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

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F1) {
				Dev_TogglePresentationState();
				continue;
			}
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
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

		struct Collision_Manifold {
			Vec2 normal;
			r32 penetration_depth;
		};

		Array<Collision_Manifold> manifolds;
		manifolds.allocator = TEMPORARY_ALLOCATOR;

		Vec2 player_force;
		while (accumulator_t >= fixed_dt) {
			Dev_TimedScope(SimulationFrame);

			const r32 gravity = 10;
			const r32 drag = 5;

			player_force = vec2(0);

			r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
			Vec2 dir;
			if (len) {
				dir.x = controller.x / len;
				dir.y = controller.y / len;
			}
			else {
				dir = vec2(0);
			}

			const float force = 100;

			player_force = force * dir;
			//player->force.y -= gravity;

			player_velocity += dt * player_force;
			player_velocity *= powf(0.5f, drag * dt);

			//Vec2 velocity_t = dt * player_velocity;

			Capsule2d player_capsule;
			player_capsule.radius = player.radius;
			player_capsule.a = player.center;

			Vec2 norm; r32 dist, v_t;

			player_capsule.b = player.center + dt * player_velocity;
			if (epa(capsule, player_capsule, &norm, &dist)) {
				array_add(&manifolds, Collision_Manifold{ norm, dist });
				v_t = dist / dt * sgn(vec2_dot(norm, player_velocity));
				player_velocity -= v_t * norm;
				capsule_color = vec4(0, 1, 1, 1);
			} else {
				capsule_color = vec4(1, 0, 0);
			}

			player_capsule.b = player.center + dt * player_velocity;
			if (epa(rect, player_capsule, &norm, &dist)) {
				array_add(&manifolds, Collision_Manifold{ norm, dist });
				v_t = dist / dt * sgn(vec2_dot(norm, player_velocity));
				player_velocity -= v_t * norm;
				rect_color = vec4(0, 1, 1, 1);
			} else {
				rect_color = vec4(1, 0, 0);
			}

			player_capsule.b = player.center + dt * player_velocity;
			if (epa(polygon, player_capsule, &norm, &dist)) {
				array_add(&manifolds, Collision_Manifold{ norm, dist });
				v_t = dist / dt * sgn(vec2_dot(norm, player_velocity));
				player_velocity -= v_t * norm;
				poly_color = vec4(0, 1, 1, 1);
			} else {
				poly_color = vec4(1, 0, 0);
			}
			
			player_capsule.b = player.center + dt * player_velocity;
			if (epa(circle, player_capsule, &norm, &dist)) {
				array_add(&manifolds, Collision_Manifold{ norm, dist });
				v_t = dist / dt * sgn(vec2_dot(norm, player_velocity));
				player_velocity -= v_t * norm;
				circle_color = vec4(0, 1, 1, 1);
			} else {
				circle_color = vec4(1, 0, 0);
			}

			player.center += dt * player_velocity;

			accumulator_t -= fixed_dt;
		}

		ImGui_UpdateFrame(real_dt);

		r32 view_height = 10.0f;
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

		im2d_begin(view);

		for (int i = 0; i < polygon.vertex_count; ++i) {
			im2d_line(polygon.vertices[i], polygon.vertices[(i + 1) % polygon.vertex_count], poly_color, 0.02f);
		}

		im2d_rect_outline(rect.min, rect.max - rect.min, rect_color, 0.02f);
		im2d_circle_outline(circle.center, circle.radius, circle_color, 0.02f);

		Vec2 capsule_dir = capsule.b - capsule.a;
		Vec2 capsule_norm = vec2_normalize(vec2(-capsule_dir.y, capsule_dir.x)) * capsule.radius;

		im2d_circle_outline(capsule.a, capsule.radius, capsule_color, 0.02f);
		im2d_circle_outline(capsule.b, capsule.radius, capsule_color, 0.02f);
		im2d_line(capsule.a + capsule_norm, capsule.b + capsule_norm, capsule_color, 0.02f);
		im2d_line(capsule.a - capsule_norm, capsule.b - capsule_norm, capsule_color, 0.02f);

		im2d_circle(player.center, player.radius, player_color);
		im2d_line(player.center, player.center + player_velocity, vec4(0, 1.5f, 0), 0.02f);

		for (auto &manifold : manifolds) {
			im2d_line(player.center, player.center + manifold.normal, 1.5f * vec4(1, 0, 0), 0.05f);
			im2d_line(player.center, player.center + manifold.penetration_depth * manifold.normal, 2 * vec4(1, 1, 0), 0.05f);
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

		ImGui::Begin("Player");
		ImGui::DragFloat2("Position", player.center.m, 0.01f);
		ImGui::DragFloat("Radius", &player.radius, 0.01f);
		ImGui::DragFloat2("Velocity", player_velocity.m, 0.01f);
		ImGui::End();

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
