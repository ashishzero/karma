#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "prebuild.h"
#include "random.h"
#include "particle_system.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int system_main() {
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, 1, 2);
	ImGui::Initialize();

	int  w, h, n;
	auto pixels = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	auto circle = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

	Random_Series rng = random_init(0, 0);

	Particle_Emitter emitter = particle_emitter_create(&circle, mm_rect(0, 0, 1, 1), &rng, 1000, 800);

	Event event;
	bool  running = true;

	const r32 aspect_ratio = 16.0f / 9.0f;
	const r32 dt           = 1.0f / 60.0f;

	float t           = 0.0f;
	float accumulator = 0.0f;
	float frame_time  = 0.0f;

	u64 frequency = system_get_frequency();
	u64 counter   = system_get_counter();

	r32 window_w = 0, window_h = 0;

	while (running) {
		auto new_counter = system_get_counter();
		auto counts      = new_counter - counter;
		counter          = new_counter;

		frame_time = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		accumulator += frame_time;
		accumulator = min_value(accumulator, 0.2f);

		while (system_poll_events(&event)) {
			if (ImGui::HandleEvent(event)) continue;

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
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				break;
			}
		}

		while (accumulator >= dt) {
			// simulate here

			t += dt;
			accumulator -= dt;
		}

		float alpha = accumulator / dt;

		ImGui::UpdateFrame(frame_time);

		float world_height_half = 400;
		float world_width_half  = aspect_ratio * world_height_half;

		auto camera_transform = mat4_identity();
		auto  view              = orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half, -2.0f, 2.0f);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0.02f, 0.02f, 0.02f));
		gfx_viewport(0, 0, 1280, 720);

		//auto camera_transform = mat4_inverse(mat4_scalar(vec3(800, 800, 1)));
		//auto view             = perspective_view(to_radians(90), aspect_ratio, 0.1f, 10.0f);

		im_begin(view, camera_transform);
		//im_begin(camera_transform);

		static Vec3 position_a  = vec3(150, 0, 2);
		static Vec2 dimension_a = vec2(100);
		static Vec4 color_a     = vec4(1, 1, 0, 1);
		static Vec3 position_b  = vec3(0, 0, 2);
		static Vec2 dimension_b = vec2(100);
		static Vec4 color_b     = vec4(1, 0, 1, 1);

		im_rect(position_a, dimension_a, color_a);
		im_rect(position_b, dimension_b, color_b);

		//{
		//	const Vec4  line_color     = vec4(1.1f, 1.1f, 1.1f);
		//	const float line_thickness = 1;
		//	for (float x = -world_width_half; x < world_width_half; x += 1) {
		//		im_line2d(vec2(x, -world_height_half), vec2(x, world_height_half), line_color, line_thickness);
		//	}
		//}

		im_end();

		gfx_end_drawing();

		//
		//
		//

		gfx_apply_bloom(2);

		r32 render_w = window_w;
		r32 render_h = floorf(window_w / aspect_ratio);
		if (render_h > window_h) {
			render_h = window_h;
			render_w = floorf(window_h * aspect_ratio);
		}

		r32 render_x = floorf((window_w - render_w) * 0.5f);
		r32 render_y = floorf((window_h - render_h) * 0.5f);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0, 0, 0));

		gfx_blit_hdr(render_x, render_y, render_w, render_h);
		gfx_viewport(0, 0, window_w, window_h);

		ImGui::RenderFrame();
		gfx_end_drawing();

		gfx_present();

		reset_temporary_memory();
	}

	ImGui::Shutdown();
	gfx_destroy_context();

	return 0;
}
