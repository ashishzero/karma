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
#include "modules/core/random.h"

#include "entity.h"

#include ".generated/entity.typeinfo"
#include "editor.h"

struct Player_Controller {
	r32 x, y;
};

#define collider_combine_type(a, b) (((a) & 0xffff) | (((b) & 0xffff) << 16))

bool collider_vs_collider_dynamic(Collider &a, Collider &b, Vec2 dp, Vec2 *normal, r32 *penetration) {
	u32 type = collider_combine_type(a.type, b.type);

#define dispatch(type_a, type_b)										    \
		case collider_combine_type(Collider_##type_a, Collider_##type_b):   \
			return epa_dynamic(*collider_get(a, type_a), *collider_get(b, type_b), dp, normal, penetration)

	switch (type) {
		dispatch(Circle, Circle);
		dispatch(Circle, Mm_Rect);
		dispatch(Circle, Polygon);
		dispatch(Circle, Capsule);

		dispatch(Mm_Rect, Circle);
		dispatch(Mm_Rect, Mm_Rect);
		dispatch(Mm_Rect, Polygon);
		dispatch(Mm_Rect, Capsule);

		dispatch(Polygon, Circle);
		dispatch(Polygon, Mm_Rect);
		dispatch(Polygon, Polygon);
		dispatch(Polygon, Capsule);

		dispatch(Capsule, Circle);
		dispatch(Capsule, Mm_Rect);
		dispatch(Capsule, Polygon);
		dispatch(Capsule, Capsule);

		invalid_default_case();
	}

#undef dispatch

	return false;
}

#include <time.h>

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

	Random_Series entity_id_series = random_init(system_get_counter(), system_get_counter());

	Player player;
	player.id = random_get() | ((time(0) & 0xffffffff) << 32);
	player.position = vec2(0);
	player.color = vec4(1);
	player.velocity = vec2(0);
	player.force = vec2(0);
	player.collider.center = vec2(0);
	player.collider.radius = 0.55f;
	player.transformed_collider.type = Collider_Circle;
	player.transformed_collider.handle = new Circle;
	
	Collider colliders[4];
	Vec4 collider_colors[4];
	memset(collider_colors, 1, sizeof(collider_colors));
	{
		colliders[0].type = Collider_Mm_Rect;
		colliders[1].type = Collider_Circle;
		colliders[2].type = Collider_Capsule;
		colliders[3].type = Collider_Polygon;

		Vec2 points[] = {
			vec2(-8, 5), vec2(-2, 5), vec2(-1, -1), vec2(-4, -5), vec2(-13, -2)
		};

		assert(static_count(points) >= 3);

		colliders[0].handle = new Mm_Rect;
		colliders[1].handle = new Circle;
		colliders[2].handle = new Capsule;
		colliders[3].handle = memory_allocate(sizeof(Polygon) + (static_count(points) - 3) * sizeof(Vec2));

		Mm_Rect *rect = collider_get(colliders[0], Mm_Rect);
		rect->min = vec2(4, -4);
		rect->max = vec2(9, 3);

		Circle *circle = collider_get(colliders[1], Circle);
		circle->center = vec2(5);
		circle->radius = 1.23f;

		Capsule *capsule = collider_get(colliders[2], Capsule);
		capsule->a = vec2(-3, -8);
		capsule->b = vec2(1, -2);
		capsule->radius = 1;

		Polygon *polygon = collider_get(colliders[3], Polygon);
		polygon->vertex_count = static_count(points);
		memcpy(collider_get(colliders[3], Polygon)->vertices, points, sizeof(points));
	}

	Player_Controller controller = {};

	Camera camera;
	camera.position = vec2(0);
	camera.distance = 0.0f;

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

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

			const float force = 20;

			player_force = force * dir;
			//player->force.y -= gravity;

			player.velocity += dt * player_force;
			player.velocity *= powf(0.5f, drag * dt);

			//Vec2 velocity_t = dt * player_velocity;

			Vec2 norm; r32 dist, v_t;

			auto player_collider = collider_get(player.transformed_collider, Circle);
			player_collider->center = player.position + player.collider.center;
			player_collider->radius = player.collider.radius;

			int collider_index = 0;
			for (auto &c : colliders) {
				if (collider_vs_collider_dynamic(c, player.transformed_collider, dt * player.velocity, &norm, &dist)) {
					array_add(&manifolds, Collision_Manifold{ norm, dist });
					v_t = dist / dt * sgn(vec2_dot(norm, player.velocity));
					player.velocity -= v_t * norm;
					collider_colors[collider_index] = vec4(0, 1, 1, 1);
				} else {
					collider_colors[collider_index] = vec4(1, 0, 0, 1);
				}
				collider_index += 1;
			}

			player.position += dt * player.velocity;

			r32 camera_follow_speed = 0.977f;
			camera.position = lerp(camera.position, player.position, 1.0f - powf(1.0f - camera_follow_speed, dt));

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

		r32 scale = powf(0.5f, camera.distance);
		Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera.position, 0.0f));

		im2d_begin(view, transform);

		int collider_index = 0;
		for (auto &c : colliders) {
			auto color = collider_colors[collider_index];
			switch (c.type) {
			case Collider_Circle: {
				auto circle = collider_get(c, Circle);
				im2d_circle_outline(circle->center, circle->radius, color, 0.02f);
			} break;

			case Collider_Polygon: {
				auto polygon = collider_get(c, Polygon);
				for (u32 i = 0; i < polygon->vertex_count; ++i) {
					im2d_line(polygon->vertices[i], polygon->vertices[(i + 1) % polygon->vertex_count], color, 0.02f);
				}
			} break;

			case Collider_Mm_Rect: {
				auto rect = collider_get(c, Mm_Rect);
				im2d_rect_outline(rect->min, rect->max - rect->min, color, 0.02f);
			} break;

			case Collider_Capsule: {
				auto capsule = collider_get(c, Capsule);
				Vec2 capsule_dir = capsule->b - capsule->a;
				Vec2 capsule_norm = vec2_normalize(vec2(-capsule_dir.y, capsule_dir.x)) * capsule->radius;

				im2d_circle_outline(capsule->a, capsule->radius, color, 0.02f);
				im2d_circle_outline(capsule->b, capsule->radius, color, 0.02f);
				im2d_line(capsule->a + capsule_norm, capsule->b + capsule_norm, color, 0.02f);
				im2d_line(capsule->a - capsule_norm, capsule->b - capsule_norm, color, 0.02f);
			} break;

			invalid_default_case();

			}
			collider_index += 1;
		}

		im2d_circle(player.position, collider_get(player.transformed_collider, Circle)->radius, player.color);
		im2d_line(player.position, player.position + player.velocity, vec4(0, 1.5f, 0), 0.02f);

		for (auto &manifold : manifolds) {
			im2d_line(player.position, player.position + manifold.normal, 1.5f * vec4(1, 0, 0), 0.05f);
			im2d_line(player.position, player.position + manifold.penetration_depth * manifold.normal, 2 * vec4(1, 1, 0), 0.05f);
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
		editor_draw(player);
		
#if 1
		if (ImGui::Button("Save")) {
			Ostream out;
			serialize_fmt_text(&out, "Player", reflect_info(player), (char *)&player);
			System_File file;
			if (system_open_file("temp/player.karma", File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
			}
			system_close_file(&file);
			ostream_free(&out);
		}

		if (ImGui::Button("Load")) {
			Tokenization_Status status;
			auto content = system_read_entire_file("temp/player.karma");
			auto tokens = tokenize(content, &status);
			if (status.result == Tokenization_Result_SUCCESS) {
				Deserialize_Error_Info error;
				auto state = deserialize_begin(tokens);
				if (!deserialize_fmt_text(&state, "Player", reflect_info(player), (char *)&player))
					system_log(LOG_ERROR, "Load", "Failed to load player");
				deserialize_end(&state);
			}
			else {
				system_log(LOG_ERROR, "Load", "Failed to load player");
			}
			memory_free(tokens.data);
			memory_free(content.data);
		}
#endif

		ImGui::End();

		ImGui::Begin("Camera");
		editor_draw(camera);
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
