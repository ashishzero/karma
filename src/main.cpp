#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "debug_service.h"
#include "particle_system.h"
#include "stream.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <time.h>
#include <stdlib.h>
#define NUM_RIGID_BODIES 20

int initial = 0;
int xy      = 2000;

typedef struct {
	float width;
	float height;
	float mass;
	float momentofInertia;
} BoxShape;

//=========two dimensional rigid body=========#

typedef struct {
	Vec2     position;
	Vec2     velocity;
	Vec2     linear_velocity;
	Vec2     linear_acceleration;
	float    angular_velocity;
	float    angle;
	Vec2     force;
	float    torque;
	BoxShape shape;
	Mm_Rect  min_max_points;
	bool     collided;
} RigidBody;

RigidBody rigidBodies[NUM_RIGID_BODIES];

int random_number_generator(int n) {
	//srand(int(time(NULL)));
	return (rand() % n + 1);
}

int neg_random_number_generator(int n) {
	//srand(int(time(NULL)));
	return (rand() % n - n + 1);
}

void CalculateBoxInertia(BoxShape *boxShape) {
	float m                   = boxShape->mass;
	float h                   = boxShape->height;
	float w                   = boxShape->width;
	boxShape->momentofInertia = m * (w * w + h * h) / 2;
}

void ComputeForceandTorque(RigidBody *rigidBody) {
	Vec2 f;
	//f = { r32(2000), r32(2000) };
	f = vec2(r32(neg_random_number_generator(2 + xy)), r32(neg_random_number_generator(1 + xy)));
	//if(initial==1)
	//	 f = { r32(1500), r32(2000) };
	//else
	//	 f = { r32(1500), r32(-2000) };

	rigidBody->force  = f;
	Vec2 r            = vec2(rigidBody->shape.width / 2, rigidBody->shape.height / 2);
	rigidBody->torque = r.x * f.y - r.y * f.x;
	xy += 1;
	initial += 1;
}

void InitializeRigidBodies() {
	int x = 0;
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		ComputeForceandTorque(rigidBody);
		rigidBody->position = vec2(r32(random_number_generator(350 + x)), r32(random_number_generator(650 + x)));
		//if (i == 0)
		//	rigidBody->position = { 500,100 };
		//if (i==1)
		//	rigidBody->position = { 500,500 };
		rigidBody->angle            = float(random_number_generator(360) / 360.0f * MATH_PI * 2);
		rigidBody->linear_velocity  = vec2(0);
		rigidBody->angular_velocity = 0;
		rigidBody->collided         = false;

		BoxShape shape;
		shape.mass = float(random_number_generator(5000));

		printf("%.2f\t", shape.mass);
		shape.width  = 20 * float(random_number_generator(2));
		shape.height = 20 * float(random_number_generator(4));
		CalculateBoxInertia(&shape);
		rigidBody->shape = shape;

		//rigidBody->linear_acceleration = { rigidBody->force.x / rigidBody->shape.mass, rigidBody->force.y / rigidBody->shape.mass };
		rigidBody->linear_acceleration.x = rigidBody->force.x / rigidBody->shape.mass;
		rigidBody->linear_acceleration.y = rigidBody->force.y / rigidBody->shape.mass;

		rigidBody->min_max_points.min = vec2(rigidBody->position.x, rigidBody->position.y);
		rigidBody->min_max_points.max = vec2(rigidBody->position.x + rigidBody->shape.width, rigidBody->position.y + rigidBody->shape.height);
	}
}

void update_min_max(RigidBody *rigidBody) {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody          = &rigidBodies[i];
		rigidBody->min_max_points.min = vec2(rigidBody->position.x, rigidBody->position.y);
		rigidBody->min_max_points.max = vec2(rigidBody->position.x + rigidBody->shape.width, rigidBody->position.y + rigidBody->shape.height);
	}
}

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

void PrintRigidBodies() {
	for (int i = 0; i < NUM_RIGID_BODIES; i++) {
		RigidBody *rigidBody = &rigidBodies[i];
		im_rect(vec2(rigidBody->position.x, rigidBody->position.y), vec2(rigidBody->shape.width, rigidBody->shape.height), vec4(vec3(1.1f, 1.2f, 1.5f), 1.0f));
	}
}

bool load_debug_font(Monospaced_Font *font) {
	String content = system_read_entire_file("data/debug.font");
	defer {
		mfree(content.data);
	};

	if (content) {
		Istream in = istream(content);

		u32 min_c   = *istream_consume(&in, u32);
		u32 max_c   = *istream_consume(&in, u32);
		r32 advance = *istream_consume(&in, r32);
		u32 size    = *istream_consume(&in, u32);

		font->info.range = (Monospaced_Font_Glyph_Range *)mallocate(size);
		memcpy(font->info.range, istream_consume_size(&in, size), size);
		font->info.first   = (s32)min_c;
		font->info.count   = (s32)(max_c - min_c + 2);
		font->info.advance = advance;

		int w      = *istream_consume(&in, int);
		int h      = *istream_consume(&in, int);
		int n      = *istream_consume(&in, int);
		u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);

		font->texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

		return true;
	}

	return false;
}

constexpr s32 MAX_SPEED_FACTOR = 6;

struct Time_Speed_Factor {
	s32 numerator   = 1;
	s32 demonimator = 1;
	r32 ratio       = 1;
};

void increase_game_speed(Time_Speed_Factor *factor) {
	if (factor->demonimator == 1) {
		factor->numerator = min_value(factor->numerator + 1, MAX_SPEED_FACTOR);
	} else {
		factor->demonimator = max_value(factor->demonimator - 1, 1);
	}
	factor->ratio = (r32)factor->numerator / (r32)factor->demonimator;
}

void decrease_game_speed(Time_Speed_Factor *factor) {
	if (factor->numerator == 1) {
		factor->demonimator = min_value(factor->demonimator + 1, MAX_SPEED_FACTOR);
	} else {
		factor->numerator = max_value(factor->numerator - 1, 1);
	}
	factor->ratio = (r32)factor->numerator / (r32)factor->demonimator;
}

enum Time_State {
	Time_State_RESUME,
	Time_State_PAUSE,
};

int system_main() { // Entry point
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform      = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, 1, 2, (u32)framebuffer_w, (u32)framebuffer_h);
	ImGui::Initialize();

	karma_debug_service_initialize();

	Monospaced_Font font;
	load_debug_font(&font);

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

	InitializeRigidBodies();

	int  w, h, n;
	auto pixels = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	auto circle = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

	Particle_Emitter emitter = particle_emitter_create(&circle, mm_rect(0, 0, 1, 1), 1000, 250);

	//
	//
	//

	Event event;
	bool  running = true;

	const r32 aspect_ratio = framebuffer_w / framebuffer_h;

	Time_Speed_Factor factor;

	r32 const fixed_dt      = 1.0f / 60.0f;
	r32       dt            = fixed_dt * factor.ratio;
	r32       game_dt       = fixed_dt * factor.ratio;
	r32       real_dt       = fixed_dt;
	r32       game_t        = 0.0f;
	r32       real_t        = 0.0f;
	r32       accumulator_t = fixed_dt;

	Time_State state = Time_State_RESUME;

	r32 window_w = 0, window_h = 0;

	u64 frequency = system_get_frequency();
	u64 counter   = system_get_counter();

	while (running) {
		karma_timed_frame_begin();

		karma_timed_block_begin(EventHandling);

		while (system_poll_events(&event)) {
			if (ImGui::HandleEvent(event)) continue;

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

			//
			// Developer Commands
			//

			if (event.type == Event_Type_KEY_UP) {
				switch (event.key.symbol) {
					case Key_F1:
						if (state == Time_State_RESUME) {
							state = Time_State_PAUSE;
						} else {
							state   = Time_State_RESUME;
							game_dt = fixed_dt;
						}
						break;
					case Key_F2:
						decrease_game_speed(&factor);
						break;
					case Key_F3:
						increase_game_speed(&factor);
						break;
					case Key_F5:
						karma_frame_recording_toggle();
						break;
				}
			}

			//
			//
			//

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

		karma_timed_block_end(EventHandling);

		karma_timed_block_begin(Simulation);

		while (accumulator_t >= fixed_dt) {
			const r32 gravity = 10;
			const r32 drag    = 5;

			player.force = vec2(0);

			player.force.x += 600 * controller.horizontal * dt;
			player.force.y += 500 * controller.jump; // TODO: Make jump framerate independent!!!

			Vec2 acceleration = (player.force / player.mass) - vec2(0, gravity);
			player.velocity += dt * acceleration;
			player.velocity *= powf(0.5f, drag * dt);
			player.position.xy += dt * player.velocity;

			if (player.position.y <= 0) {
				player.position.y = 0;
				player.velocity.y = 0;
			}

			controller.jump = false;

			if (state == Time_State_RESUME) {
#if 1
				karma_timed_block_begin(RigidBodiesUpdate);

				for (int i = 0; i < NUM_RIGID_BODIES; i++) {
					RigidBody *rigidBody = &rigidBodies[i];

					if (rigidBody->collided == false) {
						rigidBody->linear_velocity.x += rigidBody->linear_acceleration.x * dt;
						rigidBody->linear_velocity.y += rigidBody->linear_acceleration.y * dt;
					}

					for (int j = i + 1; j < NUM_RIGID_BODIES; j++) {
						RigidBody *rigidBody2 = &rigidBodies[j];
						if (aabb_collision(rigidBody->min_max_points, rigidBody2->min_max_points)) {
							Vec2 prev_velocity_rigidBody  = rigidBody->linear_velocity;
							Vec2 prev_velocity_rigidBody2 = rigidBody2->linear_velocity;

							rigidBody2->linear_velocity.x = ((2 * rigidBody->shape.mass * prev_velocity_rigidBody.x) / (rigidBody->shape.mass + rigidBody2->shape.mass) + ((rigidBody2->shape.mass - rigidBody->shape.mass) / (rigidBody2->shape.mass + rigidBody->shape.mass)) * prev_velocity_rigidBody2.x);
							rigidBody2->linear_velocity.y = ((2 * rigidBody->shape.mass * prev_velocity_rigidBody.y) / (rigidBody->shape.mass + rigidBody2->shape.mass) + ((rigidBody2->shape.mass - rigidBody->shape.mass) / (rigidBody2->shape.mass + rigidBody->shape.mass)) * prev_velocity_rigidBody2.y);

							rigidBody->linear_velocity.x = (((rigidBody->shape.mass - rigidBody2->shape.mass) / (rigidBody->shape.mass + rigidBody2->shape.mass)) * prev_velocity_rigidBody.x + (2 * rigidBody2->shape.mass * prev_velocity_rigidBody2.x) / (rigidBody->shape.mass + rigidBody2->shape.mass));
							rigidBody->linear_velocity.y = (((rigidBody->shape.mass - rigidBody2->shape.mass) / (rigidBody->shape.mass + rigidBody2->shape.mass)) * prev_velocity_rigidBody.y + (2 * rigidBody2->shape.mass * prev_velocity_rigidBody2.y) / (rigidBody->shape.mass + rigidBody2->shape.mass));

							if (rigidBody->linear_velocity.x > 80.0f)
								rigidBody->linear_velocity.x = 80.0f;
							if (rigidBody->linear_velocity.x < -80.0f)
								rigidBody->linear_velocity.x = -80.0f;
							if (rigidBody->linear_velocity.y > 80.0f)
								rigidBody->linear_velocity.y = 80.0f;
							if (rigidBody->linear_velocity.y < -80.0f)
								rigidBody->linear_velocity.y = -80.0f;

							if (rigidBody2->linear_velocity.x > 80.0f)
								rigidBody2->linear_velocity.x = 80.0f;
							if (rigidBody2->linear_velocity.x < -80.0f)
								rigidBody2->linear_velocity.x = -80.0f;
							if (rigidBody2->linear_velocity.y > 80.0f)
								rigidBody2->linear_velocity.y = 80.0f;
							if (rigidBody2->linear_velocity.y < -80.0f)
								rigidBody2->linear_velocity.y = -80.0f;

							rigidBody->linear_acceleration.x = rigidBody->linear_velocity.x / dt;
							rigidBody->linear_acceleration.y = rigidBody->linear_velocity.y / dt;

							rigidBody->force.x = rigidBody->linear_acceleration.x * rigidBody->shape.mass;
							rigidBody->force.y = rigidBody->linear_acceleration.y * rigidBody->shape.mass;

							rigidBody2->linear_acceleration.x = rigidBody2->linear_velocity.x / dt;
							rigidBody2->linear_acceleration.y = rigidBody2->linear_velocity.y / dt;

							rigidBody2->force.x = rigidBody2->linear_acceleration.x * rigidBody2->shape.mass;
							rigidBody2->force.y = rigidBody2->linear_acceleration.y * rigidBody2->shape.mass;

							rigidBody2->position.x += rigidBody2->linear_velocity.x * dt;
							rigidBody2->position.y += rigidBody2->linear_velocity.y * dt;
							update_min_max(rigidBody2);

							rigidBody2->collided = true;
						}
					}

					//if (rigidBody->collided == false) {
					rigidBody->position.x += rigidBody->linear_velocity.x * dt;
					rigidBody->position.y += rigidBody->linear_velocity.y * dt;
					//}
					float angularAcceleration = rigidBody->torque / rigidBody->shape.momentofInertia;
					rigidBody->angular_velocity += angularAcceleration * dt;
					rigidBody->angle += rigidBody->angular_velocity * dt;
					if (rigidBody->linear_velocity.x > 80.0f)
						rigidBody->linear_velocity.x = 80.0f;
					if (rigidBody->linear_velocity.x < -80.0f)
						rigidBody->linear_velocity.x = -80.0f;
					if (rigidBody->linear_velocity.y > 80.0f)
						rigidBody->linear_velocity.y = 80.0f;
					if (rigidBody->linear_velocity.y < -80.0f)
						rigidBody->linear_velocity.y = -80.0f;

					if (rigidBody->position.x <= 0) {
						rigidBody->force.x *= -1;
						rigidBody->linear_acceleration.x *= -1;
						rigidBody->linear_velocity.x *= -1;
					}
					if (rigidBody->position.x >= window_w) {
						rigidBody->force.x *= -1;
						rigidBody->linear_acceleration.x *= -1;
						rigidBody->linear_velocity.x *= -1;
					}
					if (rigidBody->position.y <= 0) {
						rigidBody->force.y *= -1;
						rigidBody->linear_acceleration.y *= -1;
						rigidBody->linear_velocity.y *= -1;
					}
					if (rigidBody->position.y >= window_h) {
						rigidBody->force.y *= -1;
						rigidBody->linear_acceleration.y *= -1;
						rigidBody->linear_velocity.y *= -1;
					}
					update_min_max(rigidBody);
				}

				for (int i = 0; i < NUM_RIGID_BODIES; i++) {
					RigidBody *rigidBody = &rigidBodies[i];
					rigidBody->collided  = false;
				}

				karma_timed_block_end(RigidBodiesUpdate);
#endif

#if 1
				karma_timed_block_begin(ParticlesUpdate);
				particle_emitter_update_particles(&emitter, dt);
				karma_timed_block_end(ParticlesUpdate);
#endif
			}

			accumulator_t -= fixed_dt;
		}

		karma_timed_block_end(Simulation);

		karma_timed_block_begin(Rendering);
		camera.position.x = player.position.x;

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		ImGui::UpdateFrame(real_dt);

		r32 world_height_half = 4.5f;
		r32 world_width_half  = aspect_ratio * world_height_half;

		auto transform = mat4_inverse(mat4_scalar(vec3(4.5f, 4.5f, 1)));
		camera.view    = perspective_view(to_radians(90), aspect_ratio, 0.1f, 2);

		auto view = orthographic_view(0, framebuffer_w, framebuffer_h, 0);
		//auto view              = orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half, -2.0f, 2.0f);

#if 1
		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0.02f, 0.02f, 0.02f));
		gfx_viewport(0, 0, framebuffer_w, framebuffer_h);

		im_begin(camera.view, transform);

		static r32 angle = 0.0f;

		angle += game_dt;

		im_rect_rotated(vec2(0), vec2(1), angle, vec4(0.6f, 0.2f, 0.3f));
		im_rect(player.position, player.scale, vec4(1));

		im_end();

		//im_begin(orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half));
		{
			im_begin(view);

			if (emitter.texture)
				im_bind_texture(*emitter.texture);

			for (int i = emitter.emit_count; i >= 0; --i) {
				Particle *particle = emitter.particles + i;
				if (particle->life <= particle->life_span) {
					r32 t = particle->life / particle->life_span;

					r32 fade_t = 1;
					if (particle->life < emitter.properties.fade_in) {
						fade_t *= particle->life / emitter.properties.fade_in;
					} else if (particle->life_span - particle->life < emitter.properties.fade_out) {
						fade_t *= (particle->life_span - particle->life) / emitter.properties.fade_out;
					}

					auto particle_color = hsv_to_rgb(lerp(particle->color_a, particle->color_b, t));
					particle_color.xyz *= emitter.properties.intensity;
					particle_color.w *= (fade_t * emitter.properties.opacity);
					im_rect_rotated(particle->position, vec2(lerp(particle->scale_a, particle->scale_b, t)), particle->rotation, particle_color);
				}
			}
			im_unbind_texture();
			PrintRigidBodies();
			im_end();

			view = orthographic_view(-world_width_half, world_width_half, world_height_half, -world_height_half);

#	if 1
			{
				im_begin(view);
				const Vec4  line_color       = vec4(0.2f, 0.2f, 0.2f, 1.0f);
				const float x_line_thickness = world_width_half / framebuffer_w;
				const float y_line_thickness = world_height_half / framebuffer_h;
				for (float x = -world_width_half; x <= world_width_half; x += 1) {
					im_line2d(vec2(x, -world_height_half), vec2(x, world_height_half), line_color, x_line_thickness);
				}
				for (float y = -world_height_half; y <= world_height_half; y += 1) {
					im_line2d(vec2(-world_width_half, y), vec2(world_width_half, y), line_color, y_line_thickness);
				}
				im_end();
			}
#	endif
		}

		gfx_end_drawing();
#endif

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

		// ImGui Rendering here
		ImGui::Begin("Edit");
		static int index = 0;
		RigidBody &rb    = rigidBodies[index];
		ImGui::Text("Speed: %f", factor.ratio);
		ImGui::Text("Speed N: %d", factor.numerator);
		ImGui::Text("Speed D: %d", factor.demonimator);
		ImGui::DragInt("Index", &index);
		ImGui::DragFloat3("Position", rb.position.m);
		ImGui::DragFloat2("Velocity", rb.velocity.m);
		ImGui::DragFloat2("Linear Velocity", rb.linear_velocity.m);
		ImGui::DragFloat2("Linear Acceleration", rb.linear_acceleration.m);
		ImGui::DragFloat("Angular Velocity", &rb.angular_velocity);
		ImGui::DragFloat("Angle", &rb.angle);
		ImGui::DragFloat2("Force", rb.force.m);
		ImGui::DragFloat("Torque", &rb.torque);

		ImGui::End();

		//ImGui::ShowDemoWindow();

		ImGui::RenderFrame();

		{
			karma_timed_scope(DebugCollation);
			karma_timed_frame_presentation(font, real_dt, window_w, window_h);
		}

		gfx_end_drawing();

		karma_timed_block_begin(Present);

		gfx_present();

		karma_timed_block_end(Present);

		karma_timed_block_end(Rendering);

		reset_temporary_memory();

		auto new_counter = system_get_counter();
		auto counts      = new_counter - counter;
		counter          = new_counter;

		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;

		game_dt = factor.ratio * real_dt;
		dt      = fixed_dt * factor.ratio;

		if (state == Time_State_RESUME) {
			game_t += game_dt;
		}

		accumulator_t += real_dt;
		accumulator_t = min_value(accumulator_t, 0.3f);

		karma_timed_frame_end();
	}

	karma_debug_service_shutdown();

	ImGui::Shutdown();
	gfx_destroy_context();

	return 0;
}
