#pragma once

#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"
#include "modules/core/serialize.h"
#include "atish.h"
#include ".generated/atish.typeinfo"
#include "modules/core/stb_image.h"
#include "modules/gfx/renderer.h"


void asset_write(FILE* destination,FILE* source)
{
	fseek(source, 0, SEEK_END);
	u64 size_of_file = ftell(source);
	rewind(source);
	fprintf(destination, "%s %zd\n", "circle.png", size_of_file);
	void* buffer = (char*)malloc(sizeof(char) * size_of_file);
	fread(buffer, size_of_file, 1, source);
	fwrite(buffer, size_of_file, 1, destination);
	fprintf(destination, "\n");
}


int karma_user_atish() {

	/*u64 size_of_file;
	char file_name[1024];
	fscanf(fp_destination, "%s %zd\n", file_name, &size_of_file);
	void* buffer = (char*)malloc(sizeof(char) * size_of_file);
	fread(buffer, size_of_file, 1, fp_destination);
	fclose(fp_destination);*/

	auto info = reflect_info<asset_info>();

	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);


	bool running = true;

	r32 aspect_ratio = framebuffer_w / framebuffer_h;


	r32 window_w = 0, window_h = 0;



	Vec2 quad_position = vec2(1);
	Vec2 scale = vec2(1);

	
#if 1
	FILE* fp_asset_table = fopen("temp/asset_table.txt", "wb+");
	if (fp_asset_table == NULL)
	{
		printf("file not opened");
		exit(1);
	}

	FILE* fp_asset_data = fopen("temp/fp_asset_data.txt", "wb+");
	if (fp_asset_data == NULL)
	{
		printf("file not opened");
		exit(1);
	}

	Array<asset_info> asset_table;
	asset_info single_asset;

	FILE* source = fopen("../res/misc/circle.png","rb+");

	fseek(source, 0, SEEK_END);
	u64 size_of_file = ftell(source);
	rewind(source);
	void* buffer = (char*)malloc(sizeof(char) * size_of_file);
	fread(buffer, size_of_file, 1, source);

	single_asset.name = "circle.png";
	single_asset.file_location = ftell(fp_asset_data);
	single_asset.size_of_file = size_of_file;
	array_add(&asset_table, single_asset);
	fwrite(buffer, size_of_file, 1, fp_asset_data);
	fclose(source);

	source = fopen("../res/misc/colors.png", "rb+");

	fseek(source, 0, SEEK_END);
	size_of_file = ftell(source);
	rewind(source);
	buffer = (char*)malloc(sizeof(char) * size_of_file);
	fread(buffer, size_of_file, 1, source);

	single_asset.name = "colors.png";
	single_asset.file_location = ftell(fp_asset_data);
	single_asset.size_of_file = size_of_file;
	array_add(&asset_table, single_asset);
	fwrite(buffer, size_of_file, 1, fp_asset_data);

	serialize_to_file(fp_asset_table, "asset_table", info, (char*)&asset_table,1, 0, true);
	fclose(source);
	fclose(fp_asset_data);
	fclose(fp_asset_table);
#endif

	


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


		gfx_present();
		

		reset_temporary_memory();

		
	}

	gfx_destroy_context();
	
	
	return 0;
}
