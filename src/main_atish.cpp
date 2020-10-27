#pragma once

#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"

#include "modules/gfx/renderer.h"
#include "modules/core/stb_image.h"
#include "asset_loader.h"

#include "modules/core/thread_pool.h"

Work_Queue* async_queue(u32 index);


bool async_add_work(Work_Queue* queue, Work_Procedure proc, void* param);
void async_flush_work(Work_Queue* queue);

void time_consume(void* x)
{
	const char* temp = (char*)x;

	system_log(LOG_INFO, "TEST", "%u, %s",(u32)context.id,temp);

}

int karma_user_atish() {

	if (!async_initialize(2, mega_bytes(32), context.allocator)) {
		system_fatal_error("Thread could not be created");
	}

	system_thread_sleep(2000);

	auto high = async_queue(0);
	auto low = async_queue(1);

	async_add_work(low, time_consume, "low1");
	async_add_work(low, time_consume, "low2");
	async_add_work(low, time_consume, "low3");
	async_add_work(low, time_consume, "low4");
	async_add_work(low, time_consume, "low5");

	async_flush_work(low);

	async_add_work(high, time_consume, "high1");
	async_add_work(high, time_consume, "high2");
	async_add_work(high, time_consume, "high3");
	async_add_work(high, time_consume, "high4");
	async_add_work(high, time_consume, "high5");


	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	bool running = true;

	r32 aspect_ratio = framebuffer_w / framebuffer_h;


	r32 window_w = 0, window_h = 0;

	Vec2 quad_position = vec2(1);
	Vec2 scale = vec2(1);


	Array_View<asset_info> asset_table = prepare_asset();

	FILE* fp_asset_data = fopen("temp/asset_data.txt", "rb+");
	if (fp_asset_data == NULL)
	{
		printf("file not opened");
		exit(1);
	}

	void* buffer = nullptr;
	u64 size = get_asset_data(asset_table, fp_asset_data, "love.png", buffer);
	int x, y, n;
	auto pixels = stbi_load_from_memory((u8*)buffer,(int) size, &x, &y, &n, 4);
	auto handle = gfx_create_texture2d(x, y, 4, Data_Format_RGBA8_UNORM, (const u8**)&pixels, Buffer_Usage_IMMUTABLE, 1);

	while (running) {

		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event& event = events[event_index];

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


		}
		r32 view_height = 5.0f;
		r32 view_width = aspect_ratio * view_height;

		auto view = orthographic_view(-view_width, view_width, view_height, -view_height);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.0f));
		gfx_viewport(0, 0, window_w, window_h);

		im2d_begin(view);
		im2d_bind_texture(handle);
		im2d_rect(vec2(0),vec2 (1), vec4 (1));

		im2d_end();

		gfx_end_drawing();

		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

		gfx_end_drawing();

		gfx_present();
		

		reset_temporary_memory();
	}

	gfx_destroy_context();
	return 0;
}
