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

void imgui_distribution_control(Distribution_Control *dist) {
	int         item_current = *dist;
	const char *items[]      = {
        "Distribution_Control_UNIFORM",
        "Distribution_Control_SQUARED",
        "Distribution_Control_CUBED",
        "Distribution_Control_FOURTH",
        "Distribution_Control_ONE_MINUS_SQUARED",
        "Distribution_Control_ONE_MINUS_CUBED",
        "Distribution_Control_ONE_MINUS_FOURTH",
        "Distribution_Control_TWO_DICE",
        "Distribution_Control_THREE_DICE",
        "Distribution_Control_FOUR_DICE",
        "Distribution_Control_TWO_DICE_SQUARED",
        "Distribution_Control_THREE_DICE_SQUARED",
        "Distribution_Control_FOUR_DICE_SQUARED",
        "Distribution_Control_EXTREMES",
	};
	ImGui::Combo("Distribution", &item_current, items, IM_ARRAYSIZE(items));
	*dist = (Distribution_Control)item_current;
}

void imgui_particle_kind(Particle_Emitter_Property::Kind *kind, Vec2 *a, Vec2 *b, Vec2 *c) {
	int         item_current = *kind;
	const char *items[]      = {
        "POINT",
        "LINE",
        "ELLIPSE",
        "TRIANGLE",
        "RECT",
	};
	ImGui::Combo("Emitter Kind", &item_current, items, IM_ARRAYSIZE(items));
	*kind = (Particle_Emitter_Property::Kind)item_current;
	switch (*kind) {
		case Particle_Emitter_Property::POINT: {
		} break;
		case Particle_Emitter_Property::LINE: {
			ImGui::DragFloat2("Segement", a->m);
		} break;
		case Particle_Emitter_Property::ELLIPSE: {
			ImGui::DragFloat("Width", &a->x);
			ImGui::DragFloat("Height", &a->y);
		} break;
		case Particle_Emitter_Property::TRIANGLE: {
			ImGui::DragFloat2("Point 0", a->m);
			ImGui::DragFloat2("Point 1", b->m);
			ImGui::DragFloat2("Point 2", c->m);
		} break;
		case Particle_Emitter_Property::RECT: {
			ImGui::DragFloat("Width", &a->x);
			ImGui::DragFloat("Height", &a->y);
		} break;
	}
}

void imgui_random_distribution(Random_Distribution *dist, const char *min, const char *max) {
	imgui_distribution_control(&dist->control);
	ImGui::DragFloat(min, &dist->min);
	ImGui::DragFloat(max, &dist->max);
}

void imgui_particle_emitter(Particle_Emitter *emitter) {
	ImGui::DragFloat3("Position", emitter->position.m);
	ImGui::DragInt("Emit Count", &emitter->emit_count, 1, 1, emitter->particles_count);
	ImGui::DragInt("Emit Rate", &emitter->properties.emission_rate);
	ImGui::DragFloat("Volume Factor", &emitter->properties.volume_factor);

	auto &props = emitter->properties;

	if (ImGui::CollapsingHeader("Shape")) {
		imgui_particle_kind(&props.kind, &props.a, &props.b, &props.c);
		imgui_distribution_control(&props.control);
	}

	if (ImGui::CollapsingHeader("Density")) {
		imgui_random_distribution(&props.density, "Min##Density", "Max##Density");
	}

	if (ImGui::CollapsingHeader("Scale A")) {
		imgui_random_distribution(&props.scale_a, "Min##ScaleA", "Max##ScaleA");
	}

	if (ImGui::CollapsingHeader("Scale B")) {
		imgui_random_distribution(&props.scale_b, "Min##ScaleB", "Max##ScaleB");
	}

	if (ImGui::CollapsingHeader("Spin")) {
		imgui_random_distribution(&props.spin, "Min##Spin", "Max##Spin");
	}

	if (ImGui::CollapsingHeader("Rotation")) {
		imgui_random_distribution(&props.rotation, "Min##Rotation", "Max##Rotation");
	}

	if (ImGui::CollapsingHeader("Initial Velocity")) {
		imgui_random_distribution(&props.initial_velocity_x, "Min X##InitialVelocity", "Max X##InitialVelocity");
		imgui_random_distribution(&props.initial_velocity_y, "Min Y##InitialVelocity", "Max Y##InitialVelocity");
	}

	if (ImGui::CollapsingHeader("Force")) {
		imgui_random_distribution(&props.force_x, "Min X##Force", "Max X##Force");
		imgui_random_distribution(&props.force_y, "Min Y##Force", "Max Y##Force");
	}

	if (ImGui::CollapsingHeader("Drag")) {
		imgui_random_distribution(&props.drag, "Min##Drag", "Max##Drag");
	}

	if (ImGui::CollapsingHeader("Color")) {
		ImGui::ColorEdit4("Color A", props.color_a.m, ImGuiColorEditFlags_InputHSV);
		ImGui::ColorEdit4("Color B", props.color_b.m, ImGuiColorEditFlags_InputHSV);
		ImGui::DragFloat("Opacity", &props.opacity, 0.01f, 0, 1);
	}

	if (ImGui::CollapsingHeader("Life Span")) {
		imgui_random_distribution(&props.life_span, "Min##LifeSpan", "Max##LifeSpan");
		ImGui::DragFloat("Fade In", &props.fade_in, 0.001f);
		ImGui::DragFloat("Fade Out", &props.fade_out, 0.001f);
	}
}

int system_main() { 
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_OPENGL, 1, 2);
	ImGui::Initialize();

	int  w, h, n;
	auto pixels      = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	auto circle      = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM, (const u8**)&pixels, Buffer_Usage_IMMUTABLE, 1);

	Random_Series rng = random_init(0, 0);

	Particle_Emitter emitter = particle_emitter_create(&circle, mm_rect(0, 0, 1, 1), &rng, 1000, 800);

	Event event;
	bool  running = true;

	const float dt = 1.0f / 60.0f;

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
				window_w        = (r32)w;
				window_h        = (r32)h;
				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}
		}

		while (accumulator >= dt) {
			// simulate here

			t += dt;
			accumulator -= dt;
		}

		float alpha = accumulator / dt;

		particle_emitter_update_particles(&emitter, frame_time);

		ImGui::UpdateFrame(frame_time);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_COLOR | Clear_DEPTH, vec4(0.3f, 0.5f, 0.8f));
		gfx_viewport(0, 0, window_w, window_h);

		auto view = orthographic_view(0, window_w, window_h, 0);

		# if 1
		im_begin(view);

		im_triangle(vec3(50, 50, 0), vec3(150, 150, 0), vec3(300, 50, 0), vec4(0.5f, 0.5f, 0.8f));
		im_triangle(vec3(150, 150, 0), vec3(350, 350, 0), vec3(400, 150, 0), vec4(1, 0, 0));

		im_triangle_outline2d(vec3(50, 50, 0), vec3(150, 150, 0), vec3(300, 50, 0), vec4(1));
		im_triangle_outline2d(vec3(150, 150, 0), vec3(350, 350, 0), vec3(400, 150, 0), vec4(1));

		im_ellipse(vec3(400, 400, 0), 150, 150, vec4(0.3f, 0.5f, 0.9f));
		im_ellipse_outline(vec3(400, 400, 0), 150, 150, vec4(1));
		im_pie(vec3(600, 400, 0), 100, 100, MATH_TAU / 2, MATH_TAU + MATH_PI, vec4(0.9f, 0.63f, 0.2f));
		im_arc_outline(vec3(600, 400, 0), 100, 100, MATH_TAU / 2, MATH_TAU + MATH_PI, vec4(1), true);

		im_quad(vec3(200, 200, 0), vec3(200, 400, 0), vec3(400, 400, 0), vec3(400, 200, 0), vec4(0.8f, 0.7f, 0.3f));
		im_quad_outline2d(vec3(200, 200, 0), vec3(200, 400, 0), vec3(400, 400, 0), vec3(400, 200, 0), vec4(1));
		im_line2d(vec2(0), vec2(500, 500), vec4(1, 0, 1), 2);
		im_bezier_quadratic2d(vec2(0), vec2(300, 250), vec2(400, 500), vec4(0, 1, 1), 2);
		im_bezier_cubic2d(vec2(0), vec2(300, 250), vec2(350, 350), vec2(400, 500), vec4(1, 1, 0), 2);
		
		im_end();
		#endif

		#if 1

		im_begin(view);
		im_unbind_texture();
		im_bind_texture(*emitter.texture);

		for (int i = 0; i < emitter.emit_count; ++i) {
			Particle *particle = emitter.particles + i;
			if (particle->life <= particle->life_span) {
				r32 t      = particle->life / particle->life_span;

				r32 fade_t = 1;
				if (particle->life < emitter.properties.fade_in) {
					fade_t *= particle->life / emitter.properties.fade_in;
				} else if (particle->life_span - particle->life < emitter.properties.fade_out) {
					fade_t *= (particle->life_span - particle->life) / emitter.properties.fade_out;
				}

				auto particle_color = fade_t * hsv_to_rgb(lerp(particle->color_a, particle->color_b, t)) * emitter.properties.opacity;
				im_rect_rotated(particle->position, vec2(lerp(particle->scale_a, particle->scale_b, t)), particle->rotation, particle_color);
			}
		}
		im_end();

		#endif

		gfx_end_drawing();

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));

		gfx_blit_hdr(0, 0, window_w, window_h);

		if (ImGui::Begin("Particle Emitter")) {
			imgui_particle_emitter(&emitter);
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
