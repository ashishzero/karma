#include "build_selector.h"
#include "gfx_renderer.h"
#include "imgui/imconfig.h"
#include "imgui/imgui.h"
#include "maths.h"
#include "string.h"
#include "systems.h"
#include "utility.h"

#include "stb_image.h"

#include "opentype.h"

#include "stream.h"
#include "string.h"
#include "systems.h"

#define timed_begin(x) u64 timed_counter_##x = system_get_counter()
#define timed_end(x) ((1000000.0f * (r32)(system_get_counter() - timed_counter_##x)) / (r32)system_get_frequency()) / 1000.0f

int system_main() {
	do_pre_build_steps(u8"../res", u8"./data");

	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_OPENGL, 1, 4, 800, 600);
	ImGui::Initialize();

	Rng rng = random_init(0, 0);

	Handle quad_shader = gfx_create_shader("./data/quad.fx");

	Texture2d circle = {};
	int		  w, h, n;
	u8 *	  pixels = stbi_load("../res/misc/circle.png", &w, &h, &n, 4);
	if (pixels) {
		Sampler_Params params = {};
		params.srgb			  = true;
		defer { stbi_image_free(pixels); };
		circle.handle = gfx_create_texture2d(w, h, n, pixels, &params);
		circle.rect	  = mm_rect(0, 0, 1, 1);
	}
	assert(circle.handle);

	Render_Region region = {};

	Event event;
	bool  running = true;

	const float dt = 1.0f / 60.0f;

	float  t			 = 0.0f;
	float  accumulator	 = 0.0f;
	float  frame_time	 = 0.0f;
	float  event_time	 = 0.0f;
	float  simulate_time = 0.0f;
	float  render_time	 = 0.0f;
	float  gpu_time		 = 0.0f;
	Handle gpu_query	 = gfx_create_query();

	u64 frequency = system_get_frequency();
	u64 counter	  = system_get_counter();

	r32 window_w = 0, window_h = 0;

	auto tex_rect = mm_rect(0, 1, 1, 0);

	String content = system_read_entire_file("c:/windows/fonts/mangal.ttf");
	assert(content);

#if 0
	stbtt_fontinfo font;
	stbtt_InitFont(&font, content.data, stbtt_GetFontOffsetForIndex(content.data, 0));
	const String string = u8"देहांते तव +-*/ सान्निध्यं देहि मे परमेश्"; //श्वर ";
	auto textures = convert_string_into_textures(font, string);
#endif

	Dynamic_Font font;
	font_create(content, 0, &font);

	utf32 codepoint = (utf32)'र';

	u8	 buffer[5] = {};
	auto bytes	   = utf32_to_utf8(codepoint, buffer);

	//const String string = u8"र्ज्ञ श्वरश्;न्निध्यंर";
	const String string = u8"र्क;र्र्;र्ज्ञ";
	//const String string = u8"देहांते तव +-*/ सान्निध्यं देहि मे परमेश्"; //श्वर ";
	//const String string = u8"The cake is a lie";

	Font_Shape font_shape = font_shape_string(&font, string);

#if 0
	struct Render_Glyph {
		Handle texture;
		u32 id;
		int w, h;
		int xoff, yoff;
	};

	float font_scale_factor = stbtt_ScaleForPixelHeight(&font.info, 100);
	Array<Render_Glyph> glyphs;
	{
		array_resize(&glyphs, font_shape.glyph_count);

		int w = 0, h = 0, xoff = 0, yoff = 0;
		for (u32 i = 0; i < font_shape.glyph_count; ++i) {
			auto id = font_shape.glyph_ids[i];
			auto pixels		= stbtt_GetGlyphBitmap(&font.info, 0, font_scale_factor, id, &w, &h, &xoff, &yoff);
			auto tex_handle = gfx_create_texture2d(w, h, 1, pixels);
			stbtt_FreeBitmap(pixels, 0);

			array_add(&glyphs,  { tex_handle, id, w, h, xoff, yoff });
		}
	}
#endif

	u32	 region_index = 0;
	Vec2 rect_min = {}, rect_max = {};
	Vec2 rect_min_target = {}, rect_max_target = {};
	Vec2 line_beg = {}, line_end = {};
	Vec2 line_beg_target = {}, line_end_target = {};

	while (running) {
		gfx_begin_query(gpu_query);

		auto new_counter = system_get_counter();
		auto counts		 = new_counter - counter;
		counter			 = new_counter;

		frame_time = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		accumulator += frame_time;
		accumulator = min_value(accumulator, 0.2f);

		timed_begin(event);

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
				region.scissor	= region.viewport;
				window_w		= (r32)w;
				window_h		= (r32)h;
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

			if ((event.type & Event_Type_KEY_DOWN) && event.key.symbol == Key_DOWN) {
				region_index = (region_index + 1) % font_shape.glyph_count;
				break;
			}

			if ((event.type & Event_Type_KEY_DOWN) && event.key.symbol == Key_UP) {
				region_index = region_index > 0 ? region_index - 1 : font_shape.glyph_count - 1;
				break;
			}
		}

		event_time = timed_end(event);

		timed_begin(simulate);

		while (accumulator >= dt) {

			// simulate

			t += dt;
			accumulator -= dt;
		}

		simulate_time = timed_end(simulate);

		timed_begin(render);

		float alpha = accumulator / dt;

		Color4 picker[4] = {
			vec4(1, 1, 1, 1),
			vec4(1, 1, 1, 1),
			vec4(0, 0, 0, 1),
			vec4(0, 0, 0, 1),
		};

		gfx_frame(0, region, Clear_Flag_ALL, hex_to_color4(colorh(0x00, 0x45, 0x4f)));

		float h	   = 1;
		auto  view = orthographic_view(0, window_w, window_h, 0);

#if 0
		gfx2d_begin(Vec2(0), 1, view, quad_shader);

		gfx2d_color(Color4(1, 0, 1));
		Vec2 cursor = { 100, window_h - 75 };
		foreach (index, g, glyphs) {
			int advance, left_side_bearing;
			stbtt_GetGlyphHMetrics(&font.info, g->id, &advance, &left_side_bearing);

			float advance_scaled = font_scale_factor * advance;

			gfx2d_texture(g->texture);
			gfx2d_rect(Vec2(cursor.x + g->xoff, cursor.y - g->h - g->yoff), Vec2(g->w, g->h), tex_rect);
			cursor.x += advance_scaled;
			//cursor.x += font_height;
		}

		gfx2d_end();
#endif

		rect_min = lerp(rect_min, rect_min_target, 0.2f);
		rect_max = lerp(rect_max, rect_max_target, 0.2f);
		line_beg = lerp(line_beg, line_beg_target, 0.2f);
		line_end = lerp(line_end, line_end_target, 0.2f);

		ImGui::UpdateFrame(frame_time, event_time, simulate_time, render_time, gpu_time);

		ImGui::Begin("Font");
		Vec2  p			= ImGui::GetCursorScreenPos();
		auto *draw_list = ImGui::GetWindowDrawList();

		static bool	 draw_points = false, draw_baseline = true, draw_regions = false;
		static float scale = 1.0f / 20.0f;

		p += vec2(170, 350);

		if (draw_baseline)
			draw_list->AddLine(vec2(0, p.y), vec2(window_w, p.y), 0xffffff00);
		//p.x -= 150;

		int x0, y0, x1, y1;
		u16 prev_id = 0;
		for (u32 id = 0; id < font_shape.glyph_count; ++id) {
			stbtt_vertex *vertices = 0;
			defer { STBTT_free(vertices, font.info.userdata); };
			int num_verts = stbtt_GetGlyphShape(&font.info, font_shape.glyph_infos[id].id, &vertices);

			int advance, left_side_bearing;
			stbtt_GetGlyphHMetrics(&font.info, font_shape.glyph_infos[id].id, &advance, &left_side_bearing);

			if (stbtt_GetGlyphBox(&font.info, font_shape.glyph_infos[id].id, &x0, &y0, &x1, &y1)) {
				Vec2 rmin = vec2(p.x + x0 * scale, p.y - y0 * scale);
				Vec2 rmax = vec2(p.x + x1 * scale, p.y - y1 * scale);
				if (draw_regions) {
					draw_list->AddRect(rmin, rmax, 0xffffffff);
				}
				if (region_index == id) {
					rect_min_target = rmin;
					rect_max_target = rmax;
					line_beg_target = vec2(p.x, p.y - 100);
					line_end_target = vec2(p.x + advance * scale, p.y - 100);
					draw_list->AddRect(rect_min, rect_max, 0xff00ffff);
					draw_list->AddLine(line_beg, line_end, 0xff0e00ff, 3);
				}
			}

			Vec2  pos		   = {};
			int	  num_segments = 12;
			float x = 0, y = 0, cx = 0, cy = 0, cx1, cy1;
			for (int i = 0; i < num_verts; ++i) {
				x	= (r32)vertices[i].x * scale;
				y	= -(r32)vertices[i].y * scale;
				cx	= (r32)vertices[i].cx * scale;
				cy	= -(r32)vertices[i].cy * scale;
				cx1 = (r32)vertices[i].cx1 * scale;
				cy1 = -(r32)vertices[i].cy1 * scale;

				if (draw_points)
					draw_list->AddRectFilled(p + vec2(x - 2, y - 2), p + vec2(x + 2, y + 2), 0xff0000ff);

				switch (vertices[i].type) {
				case STBTT_vmove:
					break;

				case STBTT_vline: {
					Vec2 a = p + pos, b = p + vec2(x, y);
					draw_list->AddLine(a, b, 0xffff00ff);
				} break;

				case STBTT_vcurve: {
					Vec2 p1 = p + pos, p2 = vec2(p.x + cx, p.y + cy), p3 = vec2(p.x + x, p.y + y);
					draw_list->PathLineTo(p1);
					p1			 = draw_list->_Path.back();
					float t_step = 1.0f / (float)num_segments;
					for (int i_step = 1; i_step <= num_segments; i_step++) {
						auto resp = bezier_quadratic(p1, p2, p3, t_step * i_step);
						draw_list->_Path.push_back(resp);
					}
					draw_list->PathStroke(0xffff00ff, false, 1);
				} break;

				case STBTT_vcubic: {
					draw_list->AddBezierCurve(p + pos, p + vec2(cx, cy), p + vec2(cx1, cy1), p + vec2(x, y), 0xffff00ff, 1, num_segments);
				} break;

					invalid_default_case();
				}

				pos = { x, y };
			}

			p.x += (r32)(advance)*scale;
		}

		utf8 buffer[50];
		ImGui::BeginChild("Child");
		for (u32 id = 0; id < font_shape.glyph_count; ++id) {
			auto count = font_shape.glyph_infos[id].codepoint_count;
			int	 idx   = 0;
			for (int i = 0; i < count; ++i) {
				idx += utf32_to_utf8(font_shape.glyph_infos[id].codepoints[i], buffer + idx);
				if (i != count - 1) {
					memcpy(buffer + idx, "  ", 2);
					idx += 2;
				}
			}
			buffer[idx] = 0;
			Colorh color	= (id == region_index) ? colorh(0xff00ffff) : colorh(0xffffffff);
			ImGui::TextColored(hex_to_color4(color), "[%d] %u (%d) : %s", (int)id, font_shape.glyph_infos[id].id, count, buffer);
			if (ImGui::IsItemClicked()) region_index = id;
		}
		ImGui::EndChild();
		ImGui::End();

		ImGui::Begin("Options");
		ImGui::Checkbox("Vertices", &draw_points);
		ImGui::Checkbox("Regions", &draw_regions);
		ImGui::Checkbox("Baseline", &draw_baseline);
		ImGui::DragFloat("Scale", &scale, 0.0001f, 0, 1);

		int codepoint_index = 0;
		for (u32 id = 0; id < font_shape.glyph_count; ++id) {
			auto count = font_shape.glyph_infos[id].codepoint_count;
			Colorh color = id == region_index ? colorh(0xff00ffff) : colorh(0xffffffff);
			for (int i = 0; i < count; ++i, ++codepoint_index) {
				auto &codepoint							 = font_shape.codepoints[codepoint_index];
				buffer[utf32_to_utf8(codepoint.codepoint, buffer)] = 0;
				auto syllable							 = ucd_indic_syllable(codepoint.codepoint);
				ImGui::TextColored(hex_to_color4(color), "[%d] U+%x  %s : %s, %s (%d)", 
					codepoint_index, codepoint, buffer, enum_string(syllable), enum_string(codepoint.prop), codepoint.value);

				if (ImGui::IsItemClicked()) region_index = id;
			}
		}

		ImGui::End();

		//ImGui::ShowDemoWindow();

		ImGui::RenderFrame();

		gfx_present();

		render_time = timed_end(render);

		reset_temporary_memory();

		gpu_time = gfx_end_query(gpu_query);
	}

	ImGui::Shutdown();
	gfx_destroy_context();

	return 0;
}
