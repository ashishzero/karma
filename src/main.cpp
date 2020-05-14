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

Mat4 make_transform(Vec3 p, Vec3 s, Quat q) {
	Mat4 m0 = mat4_scalar(s);
	Mat4 m1 = quat_get_mat4(q);
	Mat4 m2 = mat4_translation(p);
	return m2 * m1 * m0;
}

Mat4 make_camera_transform(Vec3 p, Vec3 s, Quat q) {
	s.x = 1.0f / s.x;
	s.y = 1.0f / s.y;
	s.z = 1.0f / s.z;
	q   = quat_conjugate(q);
	p   = -p;
	return make_transform(p, s, q);
}

struct Camera {
	Vec3 position;
	Vec3 scale;
	Quat rotation;

	Camera_View view;
};

void ImGui_ItemDisplay(Camera *camera) {
	ImGui::DragFloat3("Position", camera->position.m, 0.1f);
	//	ImGui::DragFloat("Scale", camera->scale.m, 0.1f);
	ImGui::DragFloat4("Rotation", camera->rotation.m, 0.1f);
	camera->scale.y = camera->scale.z = camera->scale.x;
}

struct Entity_Controller {
	r32 horizontal;
	r32 jump;
};

struct Entity {
	Vec3 position;
	Vec2 scale;

	r32  mass;
	Vec2 force;
	Vec2 velocity;
};

void ImGui_ItemDisplay(Entity *entity) {
	ImGui::DragFloat3("Position", entity->position.m, 0.1f);
	ImGui::DragFloat2("Scale", entity->scale.m, 0.1f);
	ImGui::DragFloat("Mass", &entity->mass, 0.1f);
	ImGui::DragFloat2("Force", entity->force.m, 0.1f);
	ImGui::DragFloat2("Velocity", entity->velocity.m, 0.1f);
}

int system_main() {
	const r32 framebuffer_w = 1280.0f;
	const r32 framebuffer_h = 720.0f;

	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, 1, 2, (u32)framebuffer_w, (u32)framebuffer_h);
	ImGui::Initialize();

	s32  w, h, n;
	auto pixels = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	auto circle = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

	Random_Series rng = random_init(0, 0);

	bool editor_on = false;

	Camera camera;
	camera.position = vec3(0);
	camera.scale    = vec3(1);
	camera.rotation = quat_identity();

	Entity_Controller controller;
	controller.horizontal = 0;
	controller.jump       = 0;

	Entity player;
	player.position = vec3(0, 0, 1);
	player.scale    = vec2(1);
	player.mass     = 1;
	player.force    = vec2(0);
	player.velocity = vec2(0);

	Event event;
	bool  running = true;

	const r32 aspect_ratio = framebuffer_w / framebuffer_h;
	const r32 dt           = 1.0f / 60.0f;

	r32 t           = 0.0f;
	r32 accumulator = 0.0f;
	r32 frame_time  = 0.0f;

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
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_TAB) {
				editor_on = !editor_on;
				continue;
			}

			if (event.type & Event_Type_KEYBOARD) {
				float value = (float)(event.key.state == State_DOWN);
				switch (event.key.symbol) {
					case Key_A:
					case Key_LEFT:
						controller.horizontal = -value;
						break;
					case Key_D:
					case Key_RIGHT:
						controller.horizontal = value;
						break;
				}

				if ((event.type & Event_Type_KEY_DOWN) && event.key.symbol == Key_SPACE && !event.key.repeat && player.position.y <= 0) {
					controller.jump = true;
				}
			}
		}

		while (accumulator >= dt) {
			const r32 gravity = 10;
			const r32 drag    = 5;

			player.force = vec2(0);

			player.force.x += 600 * controller.horizontal * dt;
			player.force.y += 500 * controller.jump;
			player.force.y -= gravity;

			Vec2 acceleration = player.force / player.mass;
			player.velocity += dt * acceleration;
			player.velocity *= powf(0.5f, drag * dt);
			player.position.xy += dt * player.velocity;

			if (player.position.y <= 0) {
				player.position.y = 0;
				player.velocity.y = 0;
			}

			controller.jump = false;

			t += dt;
			accumulator -= dt;
		}

		camera.position.x = player.position.x;

		r32 alpha = accumulator / dt; // TODO: Use this

		ImGui::UpdateFrame(frame_time);

		r32 world_height_half = 4.5f;
		r32 world_width_half  = aspect_ratio * world_height_half;

		auto transform = mat4_inverse(mat4_scalar(vec3(4.5f, 4.5f, 1)));
		camera.view      = perspective_view(to_radians(90), aspect_ratio, 0.1f, 2);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0.02f, 0.02f, 0.02f));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		im_begin(camera.view, transform);

		static r32 angle = 0.0f;
		angle += 0.01f;

		im_rect_rotated(vec2(0), vec2(1), angle, vec4(0.6f, 0.2f, 0.3f));
		im_rect(player.position, player.scale, vec4(1));

		//im_end();


		//im_begin(orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half));

		{
			const float x_off            = sgn(camera.position.x) * fmodf(fabsf(camera.position.x), 2.0f * world_width_half);
			const float y_off            = sgn(camera.position.y) * fmodf(fabsf(camera.position.y), 2.0f * world_height_half);
			const Vec4  line_color       = vec4(0.2f, 0.2f, 0.2f, 1.0f);
			const float x_line_thickness = world_width_half / framebuffer_w;
			const float y_line_thickness = world_height_half / framebuffer_h;
			for (float x = -world_width_half + 0.5f - x_off; x <= world_width_half; x += 1) {
				im_line2d(vec3(x, -world_height_half, 1), vec3(x, world_height_half, 1), line_color, x_line_thickness);
			}
			for (float y = -world_height_half - y_off; y <= world_height_half; y += 1) {
				im_line2d(vec3(-world_width_half, y, 1), vec3(world_width_half, y, 1), line_color, y_line_thickness);
			}
		}

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

		if (editor_on) {
			ImGui::Begin("Primary");
			ImGui::Text("Camera");
			ImGui_ItemDisplay(&camera);
			ImGui::End();

			ImGui::Begin("Player");
			ImGui_ItemDisplay(&player);
			ImGui::End();
		}

		ImGui::RenderFrame();
		gfx_end_drawing();

		gfx_present();

		reset_temporary_memory();
	}

	ImGui::Shutdown();
	gfx_destroy_context();

	return 0;
}
