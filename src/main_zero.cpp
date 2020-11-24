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

#include ".generated/entity.typeinfo"

struct Player_Controller {
	r32 x, y;
};

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

	Simulation_Speed sim_speed = simulation_speed(SIMULATION_SPEED_1X);

	r32 const dt = 1.0f / 60.0f;
	r32       game_dt = dt * sim_speed.factor;
	r32       real_dt = dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = dt;

	r32 window_w = 0, window_h = 0;

	scene_prepare();

	Scene *scene = scene_create();

	scene_load_resources(scene);

	bool loaded = scene_load_level(scene, "rokkenjima");
	assert(loaded);

	Physics_State physics_state = Physics_State_RUNNING;

	Player_Controller controller = {};

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	while (running) {
		Dev_TimedFrameBegin();

		// TODO: DEBUG CODE
		Character *primary_player = nullptr;
		if (scene->by_type.character.count)
			primary_player = &scene->by_type.character[0];

		Dev_TimedBlockBegin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

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

			if (ImGui_HandleEvent(event))
				continue;

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}

			if (scene_handle_event(scene, event))
				continue;

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

		scene_begin(scene);

		static r32 movement_force = 6;

		r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
		Vec2 dir = vec2(0);
		if (len) {
			dir.x = controller.x / len;
			dir.y = controller.y / len;
		}

		if (len && primary_player) {
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

			scene_simulate(scene, dt);

			accumulator_t -= dt;
		}

		scene_update(scene);

		if (primary_player)
			primary_player->color = vec4(0, 1, 1);

		ImGui_UpdateFrame(real_dt);

		Dev_TimedBlockEnd(Simulation);

		Dev_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / dt;

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.05f, 0.05f, 0.05f, 1.0f));
		gfx_viewport(0, 0, window_w, window_h);

		scene_render(scene, alpha, aspect_ratio);

		gfx_end_drawing();

		//
		//
		//

		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0.0f));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

		//ImGui::ShowDemoWindow();

		#if defined(ENABLE_DEVELOPER_OPTIONS)
		{
			Dev_TimedScope(DebugRender);
			Dev_RenderFrame();
		}
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

		scene_end(scene);

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
	}

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
