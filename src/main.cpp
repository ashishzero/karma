#include "karma.h"        // shared
#include "systems.h"      // windows
#include "gfx_renderer.h" // rendering
#include "length_string.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "prebuild.h"
#include "random.h"
#include "particle_system.h"

#ifdef BUILD_RELEASE
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

void imgui_distribution_control(Distribution_Control *dist) {
	int  item_current = *dist;
	const char *items[] = {
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

void imgui_random_distribution(Random_Distribution *dist, const char * min, const char *max) {
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
	}

}

int system_main() { // Entry point
	do_pre_build_steps(u8"../res", u8"./data");

	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_OPENGL, 1, 4);
	ImGui::Initialize();

	int  w, h, n;
	auto pixels      = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	auto circle      = gfx_create_texture2d(w, h, 4, pixels);
	auto rect_pixels = 0xffffffff;
	auto rectangle   = gfx_create_texture2d(1, 1, 4, (u8 *)&rect_pixels);

	Handle quad_shader = gfx_create_shader("./data/quad.fx");

	Random_Series rng = random_init(0, 0);

	Particle_Emitter emitter = particle_emitter_create(rectangle, mm_rect(0, 0, 1, 1), &rng, 1000, 750);

	Render_Region region = {};

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
				gfx_resize_framebuffer(w, h);
				region.viewport = { 0, 0, w, h };
				region.scissor  = region.viewport;
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

		gfx_frame(0, region, Clear_Flag_ALL, hex_to_color4(colorh(0x00, 0x45, 0x4f)));
		auto view = orthographic_view(0, window_w, window_h, 0);

		gfx2d_begin(vec2(0), 1, view, quad_shader);
		gfx2d_texture(emitter.texture);

		for (int i = 0; i < emitter.emit_count; ++i) {
			Particle *particle = emitter.particles + i;
			if (particle->life <= particle->life_span) {
				r32 t = particle->life / particle->life_span;
				assert(t >= 0 && t <= 1);
				auto particle_color = hsv_to_rgb(lerp(particle->color_a, particle->color_b, t)) * emitter.properties.opacity;
				gfx2d_color(particle_color);
				gfx2d_rotated_rect(particle->position, vec2(lerp(particle->scale_a, particle->scale_b, t)), particle->rotation);
			}
		}

		gfx2d_end();

		if (ImGui::Begin("Particle Emitter")) {
			imgui_particle_emitter(&emitter);
			ImGui::End();
		}

		#if 0
		//static Color4 color_a, color_b;
		emitter.properties.color_a = hsv_to_rgb(emitter.properties.color_a);
		emitter.properties.color_b = hsv_to_rgb(emitter.properties.color_b);

		// ImGui Rendering here
		ImGui::Begin("Edit");
		ImGui::DragFloat2("Position", emitter.position.m);
		ImGui::DragInt("Particle Count", &emitter.emit_count, 1, 1, emitter.particles_count);
		ImGui::DragFloat2("Min Initial Velocity", emitter.properties.initial_velocity.min.m);
		ImGui::DragFloat2("Max Initial Velocity", emitter.properties.initial_velocity.max.m);
		ImGui::ColorEdit3("Color A", emitter.properties.color_a.m);
		ImGui::ColorEdit3("Color B", emitter.properties.color_b.m);
		ImGui::DragFloat("Min Life Span", &emitter.properties.life_span.min);
		ImGui::DragFloat("Max Life Span", &emitter.properties.life_span.max);

		emitter.properties.color_a = rgb_to_hsv(emitter.properties.color_a);
		emitter.properties.color_b = rgb_to_hsv(emitter.properties.color_b);

		ImGui::End();
		#endif

		ImGui::RenderFrame();

		gfx_present();

		reset_temporary_memory();
	}

	ImGui::Shutdown();
	gfx_destroy_context();

	return 0;
}
