#include "modules/core/karma.h"
#include "modules/core/lin_maths.h"
#include "modules/core/systems.h"
#include "modules/imgui/imconfig.h"
#include "modules/core/random.h"
#include "modules/gfx/renderer.h"
#include "modules/imgui/imgui.h"
#include "modules/core/stb_image.h"
#include "modules/core/stream.h"
#include "modules/core/lin_maths.h"


static const s32 SIMULATION_SPEED_FACTORS[] = {
	-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 30, 35, 40, 45, 50
};
static const u32 SIMULATION_SPEED_1X = 11;

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

Texture2d_Handle load_texture(const String path) {
	String content = system_read_entire_file(path);
	
	if (content.count) {
		defer { memory_free(content.data); };
		
		int w, h, n;
		u8 *pixels = stbi_load_from_memory(content.data, (int)content.count, &w, &h, &n, 4);
		if (pixels) {
			auto handle = gfx_create_texture2d((u32)w, (u32)h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
			stbi_image_free(pixels);
			return handle;
		} else {
			system_log(LOG_ERROR, "Scene", "Failed to parse texture: %s", tto_cstring(path));
			return im_white_texture();
		}
	} else {
		system_log(LOG_ERROR, "Scene", "Failed to load file: %s", tto_cstring(path));
		return im_white_texture();
	}
}

bool iload_monospaced_font(const String path, Monospaced_Font *font) {
	String content = system_read_entire_file(path);
	defer{ memory_free(content.data); };
	
	if (content.count) {
		Istream in = istream(content);
		
		u32 min_c = *istream_consume(&in, u32);
		u32 max_c = *istream_consume(&in, u32);
		r32 advance = *istream_consume(&in, r32);
		u32 size = *istream_consume(&in, u32);
		
		font->info.range = (Monospaced_Font_Glyph_Range *)memory_allocate(size);
		memcpy(font->info.range, istream_consume_size(&in, size), size);
		font->info.first = (s32)min_c;
		font->info.count = (s32)(max_c - min_c + 2);
		font->info.advance = advance;
		
		int w = *istream_consume(&in, int);
		int h = *istream_consume(&in, int);
		int n = *istream_consume(&in, int);
		u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);
		
		font->texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
		
		return true;
	}
	
	return false;
}

int karma_main_template() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);
	
	ImGui_Initialize();
	
	random_init_global(context.id, system_get_counter());
	
	float learning_rate = 0.1f;
	float discount_factor = 0.88f;
	
	float min_epsilon = 0;
	float max_epsilon = 1;
	float depsilon = 0.1f;
	
	int generation = 0;
	int win_count = 0;
	int steps = 0;
	
	const int max_steps = 500;
	const int max_win_count = 15;
	
	//
	// Assets
	//
	
	auto stone_texture = load_texture("template/stone.png");
	auto grass_texture = load_texture("template/grass.png");
	auto star_texture  = load_texture("template/star.png");
	auto girlr_texture = load_texture("template/girl.png");
	auto girll_texture = load_texture("template/girl_flip.png");
	auto circle_texture = load_texture("template/circle.png");
	
	Monospaced_Font font;
	iload_monospaced_font("resources/font/mono.font", &font);
	
	//
	//
	//
	
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
	
	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();
	
	while (running) {
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
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F3) {
				if (sim_speed.index > 0) {
					sim_speed.index -= 1;
				}
				continue;
			}
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F4) {
				if (sim_speed.index < static_count(SIMULATION_SPEED_FACTORS) - 1) {
					sim_speed.index += 1;
				}
				continue;
			}
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}
		}
		
		while (accumulator_t >= dt) {
			
			accumulator_t -= dt;
		}
		
		//
		//
		//
		
		r32 view_height = 1;
		r32 view_width = aspect_ratio * view_height;
		
		ImGui_UpdateFrame(real_dt);
		
		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.05f, 0.05f, 0.05f, 1.0f));
		gfx_viewport(0, 0, window_w, window_h);
		
		Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height);
		
		im2d_begin(view);
		
		im2d_end();
		
		gfx_end_drawing();
		
		gfx_apply_bloom(2);
		
		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0.0f));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);
		
		ImGui_RenderFrame();
		
		gfx_end_drawing();
		
		gfx_present();
		
		reset_temporary_memory();

		sim_speed = simulation_speed(sim_speed.index);
		
		auto new_counter = system_get_counter();
		auto counts = new_counter - counter;
		counter = new_counter;
		
		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;
		
		game_dt = real_dt * sim_speed.factor;
		
		accumulator_t += game_dt;
		accumulator_t = minimum(accumulator_t, 0.2f);
	}
	
	ImGui_Shutdown();
	gfx_destroy_context();
	
	return 0;
}
