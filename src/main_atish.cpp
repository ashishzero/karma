#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"
#include "modules/core/serialize.h"
#include "atish.h"
#include ".generated/atish.typeinfo"
#include "modules/gfx/renderer.h"
#include "modules/core/stb_image.h"

//If any error occurs while updating files, delete two file asset_data.txt and asset_table.txt and run the program

static const String ASSET_EXTENSIONS[] = {
	".png",
	".wav"
};

u64 get_asset_data(Array_View<asset_info>& asset_table, FILE* fp, String file_name,void* &buffer)
{
	for (s64 i = 0; i < asset_table.count; ++i)
	{
		if (string_match(asset_table[i].name, file_name))
		{
			buffer = (char*)malloc(sizeof(char) * asset_table[i].size_of_file);
			fseek(fp, (long)asset_table[i].file_location, SEEK_SET);
			fread(buffer, asset_table[i].size_of_file, 1, fp);
			return asset_table[i].size_of_file;
		}
	}
	trigger_breakpoint();
	return 0;
	//TODO : Not found condition needs to be checked
}

Array_View<asset_info> prepare_asset()
{
	auto all_files = system_find_files("../res", "*", true);

	// load all files
	// Check if it is not .png or .wav then remove it from all files
	// remaining work same
	Array<asset_info> asset_table;
	FILE* fp_asset_table = fopen("temp/asset_table.txt", "rb+");
	if (fp_asset_table == NULL)
	{
		printf("file not opened");
		exit(1);
	}
	FILE* fp_asset_data = fopen("temp/asset_data.txt", "rb+");
	if (fp_asset_data == NULL)
	{
		printf("file not opened");
		exit(1);
	}

	//defer{ fclose(fp_asset_data); };

	fseek(fp_asset_data, 0, SEEK_END);
	deserialize_from_file(fp_asset_table, "asset_table", reflect_info<Array<asset_info>>(), (char*)&asset_table, 1, 0);
	FILE* source;
	asset_info single_asset;
	char* buffer;
	// TODO : need optimization
	for (s64 i = 0; i < all_files.count; ++i)
	{
		bool found = false;

		for (s64 ii = 0; ii < static_count(ASSET_EXTENSIONS); ++ii)
		{
			if (string_match(all_files[i].extension, ASSET_EXTENSIONS[ii]))
			{
				found = true;
				break;
			}
		}
		if (!found)
			continue;

		found = false;
		for (s64 j = 0; j < asset_table.count; ++j)
		{
			if (string_match(all_files[i].name, asset_table[j].name))
			{
				found = true;
				break;
			}
		}
		if (found)
			continue;
		else
		{
			const char* temp = (char*) all_files[i].path.data;
			source = fopen(temp, "rb");
			if (source == NULL)
			{
				printf("image not opened");
				exit(0);
			}
			buffer = (char*)malloc(sizeof(char) * all_files[i].size);
			fread(buffer, all_files[i].size, 1, source);
			single_asset.name = all_files[i].name;
			single_asset.file_location = ftell(fp_asset_data);
			single_asset.size_of_file = all_files[i].size;
			array_add(&asset_table, single_asset);
			fwrite(buffer, all_files[i].size, 1, fp_asset_data);
			fclose(source);
		}
	}
	fclose(fp_asset_table);
	fp_asset_table = fopen("temp/asset_table.txt", "wb");
	serialize_to_file(fp_asset_table, "asset_table", reflect_info<Array<asset_info>>(), (char*)&asset_table, 1, 0, true);
	fclose(fp_asset_table);
	fclose(fp_asset_data);
	return asset_table;
}

int karma_user_atish() {
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
	u64 size = get_asset_data(asset_table, fp_asset_data, "boing.wav", buffer);
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
