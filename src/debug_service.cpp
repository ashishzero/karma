#include "debug_service.h"
#include "systems.h"
#include "intrinsics.h"
#include "random.h"
#include "utility.h"
#include "stream.h"

static constexpr u32 MAX_RECORDS_LOGS                    = 5000000; // around 190 MB
static constexpr u32 RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD = 2;
static constexpr u32 MAX_COLLATION_RECORDS               = MAX_RECORDS_LOGS / 2;

static constexpr r32 DEBUG_PRESENTATION_X_OFFSET		= 5.0f;
static constexpr r32 DEBUG_PRESENTATION_Y_OFFSET		= 2.0f;
static constexpr r32 DEBUG_PRESENTATION_BLOCK_Y_OFFSET  = 5.0f;

static constexpr int FRAME_TIME_MAX_LOGS = 150;

static constexpr int PROFILER_MAX_PRESENTATION_COLORS = 50;

enum Menu_Icon {
	Menu_Icon_FRAME_TIME,
	Menu_Icon_PROFILER,
	Menu_Icon_AUDIO,

	Menu_Icon_COUNT,
};

struct Record {
	String name;
	String id;
	r32    ms;
	u32    begin_cycle;
	u32    end_cycle;

	Record *parent;
	Record *next;
	Record *child;
};

struct Debug_Collation {
	Record *root_record;
	Record *leaf_record;
};

static void * debug_memory;

static Timed_Frame  timed_frames[RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD];
static Timed_Frame *timed_frame_writer_ptr;
static Timed_Frame *timed_frame_reader_ptr;

static Color3 presentation_colors[PROFILER_MAX_PRESENTATION_COLORS];

static r32 stablilized_frame_time = 0.0f;
static r32 frame_time_history[FRAME_TIME_MAX_LOGS];

constexpr r32 PROFILER_RESIZER_SIZE      = 10.0f;
constexpr r32 PROFILER_MIN_WIDTH		 = 450.0f;
constexpr r32 PROFILER_MIN_HEIGHT		 = 100.0f;
static r32 profiler_width	= 450.0f;
static r32 profiler_height	= 100.0f;
static Vec2 profiler_resizer_position;
static bool profiler_resizing		   = false;
static bool left_button_pressed		   = false;
static bool debug_presentation_hovered = false;
static Vec2 mouse_cursor;

static Debug_Collation debug_collation;
static Record *        record_collation_ptr;
static Record *        record_collation_trav;

static bool debug_service_present		= true;
static bool frame_recording             = true;
static bool user_frame_recording_option = true;

static Monospaced_Font		debug_font;
static Texture2d_Handle		debug_menu_icons;
static Mm_Rect				debug_menu_icons_rect[Menu_Icon_COUNT];
static Color4				debug_menu_icons_color[Menu_Icon_COUNT];
static bool					debug_menu_icons_value[Menu_Icon_COUNT];

bool frame_recording_get_state() {
	return user_frame_recording_option;
}

void frame_recording_set_state(bool state) {
	user_frame_recording_option = state;
}

bool debug_presentation_get_display() {
	return debug_service_present;
}

void debug_presentation_set_display(bool state) {
	debug_service_present = state;
}

void reset_collation_record() {
	record_collation_trav = record_collation_ptr;
}

Record *push_collation_record() {
	assert(record_collation_trav < record_collation_ptr + MAX_COLLATION_RECORDS);
	auto ptr = record_collation_trav;
	record_collation_trav += 1;
	return ptr;
}

void debug_service_initialize() {
	debug_memory = system_virtual_alloc(0, RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD * MAX_RECORDS_LOGS * sizeof(Timed_Record) + MAX_COLLATION_RECORDS * sizeof(Record), Virtual_Memory_COMMIT | Virtual_Memory_RESERVE);

	for (u32 i = 0; i < RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD; ++i) {
		timed_frames[i].records             = ((Timed_Record *)debug_memory) + i * MAX_RECORDS_LOGS;
		timed_frames[i].records_count       = 0;
		timed_frames[i].records_capacity	= MAX_RECORDS_LOGS;
		timed_frames[i].begin_counter_value = 0;
		timed_frames[i].end_counter_value   = 0;
	}

	record_collation_ptr = (Record *)((u8 *)debug_memory + RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD * MAX_RECORDS_LOGS * sizeof(Timed_Record));

	timed_frame_writer_ptr = timed_frames + 0;
	timed_frame_reader_ptr = timed_frames + 1;

	for (int color_index = 0; color_index < PROFILER_MAX_PRESENTATION_COLORS; ++color_index) {
		presentation_colors[color_index] = random_color3(0.9f, 0.3f);
	}

	//
	//
	//

	{
		String content = system_read_entire_file("dev/debug.font");
		defer{ memory_free(content.data); };

		if (content.count) {
			Istream in = istream(content);

			u32 min_c = *istream_consume(&in, u32);
			u32 max_c = *istream_consume(&in, u32);
			r32 advance = *istream_consume(&in, r32);
			u32 size = *istream_consume(&in, u32);

			debug_font.info.range = (Monospaced_Font_Glyph_Range *)memory_allocate(size);
			memcpy(debug_font.info.range, istream_consume_size(&in, size), size);
			debug_font.info.first = (s32)min_c;
			debug_font.info.count = (s32)(max_c - min_c + 2);
			debug_font.info.advance = advance;

			int w = *istream_consume(&in, int);
			int h = *istream_consume(&in, int);
			int n = *istream_consume(&in, int);
			u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);

			debug_font.texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
		}
	}

	{
		String content = system_read_entire_file("dev/icons.img");
		defer{ memory_free(content.data); };

		if (content.count) {
			Istream in = istream(content);

			u32 w				= *istream_consume(&in, u32);
			u32 h				= *istream_consume(&in, u32);
			u32 x_icon_count	= *istream_consume(&in, u32);
			u32 y_icon_count	= *istream_consume(&in, u32);
			u32 sz				= *istream_consume(&in, u32);
			u8 *pixels			= (u8 *)istream_consume_size(&in, sz);

			assert(x_icon_count * y_icon_count >= Menu_Icon_COUNT);

			debug_menu_icons = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

			for (u32 y_index = 0; y_index < y_icon_count; ++y_index) {
				for (u32 x_index = 0; x_index < x_icon_count; ++x_index) {
					u32 index = x_index + y_index * x_icon_count;
					debug_menu_icons_rect[index].min.x = (r32)x_index / (r32)x_icon_count;
					debug_menu_icons_rect[index].max.y = (r32)y_index / (r32)y_icon_count;
					debug_menu_icons_rect[index].max.x = (r32)(x_index + 1) / (r32)x_icon_count;
					debug_menu_icons_rect[index].min.y = (r32)(y_index + 1) / (r32)y_icon_count;

					debug_menu_icons_color[index] = vec4(1);
				}
			}
		}
	}
}

void debug_service_shutdown() {
	system_virtual_free(debug_memory, 0, Virtual_Memory_RELEASE);
	memory_free(debug_font.info.range);
}

Timed_Frame *timed_frame_get() {
	return timed_frame_reader_ptr;
}

void timed_frame_begin() {
	if (frame_recording) {
		timed_frame_writer_ptr->begin_cycle_value   = intrin__rdtsc();
		timed_frame_writer_ptr->begin_counter_value = system_get_counter();
	}
}

void timed_frame_end(r32 frame_time) {
	if (frame_recording) {
		timed_frame_writer_ptr->end_cycle_value   = intrin__rdtsc();
		timed_frame_writer_ptr->end_counter_value = system_get_counter();

		swap(&timed_frame_writer_ptr, &timed_frame_reader_ptr);
	
		timed_frame_writer_ptr->records_count       = 0;
		timed_frame_writer_ptr->begin_counter_value = 0;
		timed_frame_writer_ptr->end_counter_value   = 0;
	}

	memmove(frame_time_history + 1, frame_time_history, sizeof(r32) * (FRAME_TIME_MAX_LOGS - 1));
	frame_time_history[0] = frame_time;

	frame_recording = user_frame_recording_option;
}

Timed_Block_Match timed_block_begin(String id, String block_name) {
	if (frame_recording) {
		assert(timed_frame_writer_ptr->records_count < MAX_RECORDS_LOGS);
		auto record = timed_frame_writer_ptr->records + timed_frame_writer_ptr->records_count;
		timed_frame_writer_ptr->records_count += 1;

		record->id         = id;
		record->block_name = block_name;
		record->time_stamp = intrin__rdtsc();
		record->type       = Timed_Record_Type_BEGIN;

		return id;
	}

	return String("", 0);
}

void timed_block_end(Timed_Block_Match value, String block_name) {
	if (frame_recording) {
		assert(timed_frame_writer_ptr->records_count < MAX_RECORDS_LOGS);
		auto record = timed_frame_writer_ptr->records + timed_frame_writer_ptr->records_count;
		timed_frame_writer_ptr->records_count += 1;

		record->id         = value;
		record->block_name = block_name;
		record->time_stamp = intrin__rdtsc();
		record->type       = Timed_Record_Type_END;
	}
}

bool debug_service_handle_event(Event &event) {
	bool handled = false;

	switch (event.type) {
		case Event_Type_MOUSE_BUTTON_DOWN: {
			if (event.mouse_button.symbol == Button_LEFT) {
				if (point_inside_rect(mouse_cursor, mm_rect(profiler_resizer_position, profiler_resizer_position + vec2(PROFILER_RESIZER_SIZE)))) {
					profiler_resizing = true;
				}
			}
		} break;

		case Event_Type_MOUSE_BUTTON_UP: {
			if (event.mouse_button.symbol == Button_LEFT) {
				profiler_resizing	= false;
				left_button_pressed = true;
			}
		} break;

		case Event_Type_MOUSE_CURSOR: {
			mouse_cursor.x = (r32)event.mouse_cursor.x;
			mouse_cursor.y = (r32)event.mouse_cursor.y;
			handled = (profiler_resizing || debug_presentation_hovered);

			if (profiler_resizing) {
				profiler_width   = max_value(profiler_width + event.mouse_cursor.x_rel, PROFILER_MIN_WIDTH);
				profiler_height  = max_value(profiler_height + event.mouse_cursor.y_rel, PROFILER_MIN_HEIGHT);
			}
		} break;
	}

	return handled;
}

r32 draw_header_and_buttons(r32 render_height, r32 framebuffer_w, r32 framebuffer_h, Vec2 cursor, bool *set_on_hovered) {
	const Vec4    HEADER_FONT_COLOR  = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	constexpr r32 HEADER_FONT_HEIGHT = 24.0f;
	constexpr r32 VERSION_X_OFFSET   = 8.0f;

	//
	// Frame Time and Version print
	//

	stablilized_frame_time        = stablilized_frame_time * 0.8f + 0.2f * frame_time_history[0];
	String frame_time_string      = tprintf("FrameTime: %.3fms", stablilized_frame_time * 1000.0f);
	String version_string         = "v" KARMA_VERSION_STRING;

	r32 draw_y = render_height - HEADER_FONT_HEIGHT;

	im_bind_texture(debug_font.texture);
	Vec2 frame_time_draw_pos = vec2(DEBUG_PRESENTATION_X_OFFSET, draw_y);
	auto ft_size = im_calculate_text_region(HEADER_FONT_HEIGHT, debug_font.info, frame_time_string);
	im_text(frame_time_draw_pos, HEADER_FONT_HEIGHT, debug_font.info, frame_time_string, HEADER_FONT_COLOR);

	if (point_inside_rect(cursor, mm_rect(frame_time_draw_pos, frame_time_draw_pos + ft_size))) {
		*set_on_hovered = true;
	}

	auto v_size = im_calculate_text_region(HEADER_FONT_HEIGHT, debug_font.info, version_string);
	im_text(vec2(framebuffer_w - v_size.x - 8.0f, draw_y), HEADER_FONT_HEIGHT, debug_font.info, version_string, HEADER_FONT_COLOR);

	//
	// Menu Icons update and render
	//

	Vec2 icon_positions[Menu_Icon_COUNT];
	Vec2 icon_dimensions[Menu_Icon_COUNT];

	const r32  FRAME_TIME_AND_MENU_GAP	= 20.0f;
	const r32  MENU_ICON_X_OFFSET		= 5.0f;
	const auto MENU_ICON_WIDTH			= HEADER_FONT_HEIGHT;
	const auto MENU_ICON_X_POSITION		= MENU_ICON_WIDTH + MENU_ICON_X_OFFSET;

	r32 icon_draw_start_x = ft_size.x + FRAME_TIME_AND_MENU_GAP;
	for (int icon_index = 0; icon_index < Menu_Icon_COUNT; ++icon_index) {
		Vec2 pos = vec2(icon_draw_start_x + MENU_ICON_X_POSITION * icon_index, draw_y);
		Vec2 dim = vec2(HEADER_FONT_HEIGHT);

		icon_positions[icon_index]  = pos;
		icon_dimensions[icon_index] = dim;

		bool hovering = point_inside_rect(cursor, mm_rect(pos, pos + dim));

		if (hovering && left_button_pressed) {
			debug_menu_icons_value[icon_index] = !debug_menu_icons_value[icon_index];
		}

		if (debug_menu_icons_value[icon_index]) {
			debug_menu_icons_color[icon_index] = vec4(0.8f, 0.8f, 0, 1);
		} else if (hovering) {
			debug_menu_icons_color[icon_index] = vec4(1);
		} else {
			debug_menu_icons_color[icon_index] = vec4(0.2f, 0.2f, 0.2f);
		}

	}

	Vec2 min_icons_region = vec2(icon_draw_start_x, draw_y);
	Vec2 max_icons_region = vec2(icon_draw_start_x + MENU_ICON_X_POSITION * Menu_Icon_COUNT, draw_y + HEADER_FONT_HEIGHT);
	if (point_inside_rect(cursor, mm_rect(min_icons_region, max_icons_region))) {
		*set_on_hovered = true;
	}

	im_bind_texture(debug_menu_icons);

	for (int icon_index = 0; icon_index < Menu_Icon_COUNT; ++icon_index) {
		im_rect(icon_positions[icon_index], icon_dimensions[icon_index], debug_menu_icons_rect[icon_index], debug_menu_icons_color[icon_index]);
	}

	return render_height - HEADER_FONT_HEIGHT;
}

r32 draw_frame_time_graph(r32 render_height, Vec2 cursor, bool *set_on_hovered) {
	constexpr r32 FRAME_TIME_GRAPH_WIDTH			= 450.0f;
	constexpr r32 FRAME_TIME_GRAPH_HEIGHT			= 100.0f;
	constexpr r32 FRAME_TIME_GRAPH_PROGRESS			= 3.0f;
	constexpr r32 FRAME_TIME_MARK_FONT_HEIGHT		= 15.0f;
	constexpr r32 FRAME_TIME_OUTLINE_THICKNESS		= 0.5f;
	constexpr r32 FRAME_TIME_MAX_DT_HEIGHT_FACTOR	= 1.2f;
	constexpr r32 FRAME_TIME_MARK_LINE_THICKNESS	= 0.6f;
	constexpr r32 FRAME_TIME_GRAPH_LINE_THICKNESS	= 0.6f;
	constexpr int FRAME_TIME_LOGS_COUNT				= (int)(FRAME_TIME_GRAPH_WIDTH / FRAME_TIME_GRAPH_PROGRESS);
	constexpr r32 FRAME_TIME_MARK_CRITICAL_Y_OFFSET	= FRAME_TIME_GRAPH_HEIGHT * 0.2f;

	constexpr r32 FRAME_TIME_MARK_DT[] = {
		1.0f / 30.0f,
		1.0f / 60.0f
	};

	const Vec4 FRAME_TIME_MARK_COLOR = vec4(0, 1, 1);

	static_assert(FRAME_TIME_LOGS_COUNT <= FRAME_TIME_MAX_LOGS, "The number of frame time to be drawn should be less than or equal to the snapshots recorded");

	r32 draw_x = DEBUG_PRESENTATION_X_OFFSET;
	r32 draw_y = render_height - FRAME_TIME_GRAPH_HEIGHT - DEBUG_PRESENTATION_BLOCK_Y_OFFSET;
	
	// Find max frame time to display the graph in reference to the max frame time
	r32 max_frame_time = frame_time_history[0];
	for (int frame_time_index = 1; frame_time_index < FRAME_TIME_LOGS_COUNT; ++frame_time_index) {
		if (frame_time_history[frame_time_index] > max_frame_time)
			max_frame_time = frame_time_history[frame_time_index];
	}
	max_frame_time *= FRAME_TIME_MAX_DT_HEIGHT_FACTOR;
	r32 inv_max_frame_time = 1.0f / max_frame_time;

	Vec2 bg_draw_pos = vec2(draw_x, draw_y);
	if (point_inside_rect(cursor, mm_rect(bg_draw_pos, bg_draw_pos + vec2(FRAME_TIME_GRAPH_WIDTH, FRAME_TIME_GRAPH_HEIGHT)))) {
		*set_on_hovered = true;
	}

	im_unbind_texture();
	im_rect(bg_draw_pos, vec2(FRAME_TIME_GRAPH_WIDTH, FRAME_TIME_GRAPH_HEIGHT), vec4(0, 0, 0, 0.8f));

	// Draw the actual graph
	r32 next_y;
	r32 x = draw_x;
	r32 prev_y = frame_time_history[0] * inv_max_frame_time * FRAME_TIME_GRAPH_HEIGHT + draw_y;
	for (int frame_time_index = 1; frame_time_index < FRAME_TIME_LOGS_COUNT; ++frame_time_index) {
		next_y = frame_time_history[frame_time_index] * inv_max_frame_time * FRAME_TIME_GRAPH_HEIGHT + draw_y;

		im_line2d(vec2(x, prev_y), vec2(x + FRAME_TIME_GRAPH_PROGRESS, next_y), vec4(1), FRAME_TIME_GRAPH_LINE_THICKNESS);
		x += FRAME_TIME_GRAPH_PROGRESS;
		prev_y = next_y;
	}

	// Draw important frame-time markers (only if fits in reference with max frame time)
	r32 t;
	r32 marked_dt_y_offset[static_count(FRAME_TIME_MARK_DT)] = {};
	for (int mark_index = 0; mark_index < static_count(FRAME_TIME_MARK_DT); ++mark_index) {
		t = FRAME_TIME_MARK_DT[mark_index] * inv_max_frame_time;
		marked_dt_y_offset[mark_index] = t * FRAME_TIME_GRAPH_HEIGHT;

		if (t > 0 && t <= 1) {
			r32 mark_y = draw_y + marked_dt_y_offset[mark_index];
			im_line2d(vec2(draw_x, mark_y), vec2(FRAME_TIME_GRAPH_WIDTH, mark_y), FRAME_TIME_MARK_COLOR, FRAME_TIME_MARK_LINE_THICKNESS);
		}
	}

	// Draw the outline over the region
	im_rect_outline2d(bg_draw_pos, vec2(FRAME_TIME_GRAPH_WIDTH, FRAME_TIME_GRAPH_HEIGHT), vec4(1), FRAME_TIME_OUTLINE_THICKNESS);

	im_bind_texture(debug_font.texture);
	for (int mark_index = 0; mark_index < static_count(FRAME_TIME_MARK_DT); ++mark_index) {
		if (marked_dt_y_offset[mark_index] > FRAME_TIME_MARK_CRITICAL_Y_OFFSET) {
			String mark_string = tprintf("%.2fms", 1000.0f * FRAME_TIME_MARK_DT[mark_index]);
			im_text(vec2(draw_x, draw_y + marked_dt_y_offset[mark_index] - FRAME_TIME_MARK_FONT_HEIGHT), FRAME_TIME_MARK_FONT_HEIGHT, debug_font.info, mark_string, FRAME_TIME_MARK_COLOR);
		}
	}

	return draw_y;
}

int calculate_max_children_count_in_profiler_records(Record *root_record) {
	int max_children = 1;

	while (root_record) {
		int children = 1;
		auto record = root_record;

		while (record->child) {
			children += 1;
			record = record->child;
		}
		max_children = max_value(max_children, children);

		root_record = root_record->next;
	}

	return max_children;
}

void draw_profiler_timelines_rects(Record *record, Vec2 top_position, r32 profiler_width, r32 child_height, int color_index, r32 inv_cycles, Vec2 cursor, Record **hovered_record) {
	Vec2 render_position;
	render_position.y = top_position.y - child_height;
	Vec2 dimension;
	dimension.y = child_height;

	while (record) {
		Vec4 color = vec4(presentation_colors[color_index], 1);
		color_index += 1;
		if (color_index >= PROFILER_MAX_PRESENTATION_COLORS) color_index = 0;

		render_position.x = top_position.x + profiler_width * (r32)record->begin_cycle * inv_cycles;
		dimension.x       = profiler_width * (r32)(record->end_cycle - record->begin_cycle) * inv_cycles;

		im_rect(render_position, dimension, color);
		im_rect_outline2d(render_position, dimension, vec4(1), 0.4f);

		if (point_inside_rect(cursor, mm_rect(render_position, render_position + dimension))) {
			*hovered_record = record;
		}

		if (record->child) {
			draw_profiler_timelines_rects(record->child, top_position - vec2(0, child_height), profiler_width, child_height, color_index, inv_cycles, cursor, hovered_record);
		}

		record = record->next;
	}
}

void draw_profiler_timelines_texts(Record *record, Vec2 top_position, r32 profiler_width, r32 child_height, r32 font_height, r32 inv_cycles) {
	Vec2 render_position;
	render_position.y = top_position.y - child_height;
	Vec2 dimension;
	dimension.y = child_height;

	while (record) {
		render_position.x = top_position.x + profiler_width * (r32)record->begin_cycle * inv_cycles;
		dimension.x       = profiler_width * (r32)(record->end_cycle - record->begin_cycle) * inv_cycles;

		Vec2 region = im_calculate_text_region(font_height, debug_font.info, record->name);
		if (dimension.x > region.x) {
			// align font in center only if there is enough space for font to be displayed
			render_position.x += (dimension.x - region.x) * 0.5f;
		}

		im_text_region(render_position, vec2(dimension.x, font_height), debug_font.info, record->name, vec4(1));

		if (record->child) {
			draw_profiler_timelines_texts(record->child, top_position- vec2(0, child_height), profiler_width, child_height, font_height, inv_cycles);
		}

		record = record->next;
	}
}

r32 draw_profiler(r32 render_height, Vec2 cursor, bool *set_on_hovered, Record **out_hovered_record = nullptr) {
	auto frame				= timed_frame_get();
	auto counts				= frame->end_counter_value - frame->begin_counter_value;
	r32 inv_cycles_count	= 1.0f / ((r32)(frame->end_cycle_value - frame->begin_cycle_value));
	r32  dt					= ((1000000.0f * (r32)counts) / (r32)system_get_frequency()) / 1000000.0f;
	
	// Prepare render data
	{
		reset_collation_record();

		Record *record = NULL;
		for (s64 record_index = 0; record_index < frame->records_count; ++record_index) {
			auto frame_record = frame->records + record_index;

			if (debug_collation.leaf_record == nullptr) {
				record              = push_collation_record();
				record->child       = NULL;
				record->parent      = NULL;
				record->next        = NULL;
				record->id          = frame_record->id;
				record->name        = frame_record->block_name;
				record->ms          = -1; // we use negative to endicate that END has not been reached
				record->begin_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);

				debug_collation.root_record = record;
				debug_collation.leaf_record = record;
			} else if (frame_record->type == Timed_Record_Type_BEGIN) {
				if (debug_collation.leaf_record->ms > 0) {
					record              = push_collation_record();
					record->child       = NULL;
					record->parent      = debug_collation.leaf_record->parent;
					record->next        = NULL;
					record->id          = frame_record->id;
					record->name        = frame_record->block_name;
					record->ms          = -1;
					record->begin_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);

					debug_collation.leaf_record->next = record;
					debug_collation.leaf_record       = record;
				} else {
					record              = push_collation_record();
					record->child       = NULL;
					record->parent      = debug_collation.leaf_record;
					record->next        = NULL;
					record->id          = frame_record->id;
					record->name        = frame_record->block_name;
					record->ms          = -1;
					record->begin_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);

					debug_collation.leaf_record->child = record;
					debug_collation.leaf_record        = record;
				}
			} else {
				auto parent = debug_collation.leaf_record;

				while (parent) {
					if (parent->id.data == frame_record->id.data && parent->name.data == frame_record->block_name.data) {
						break;
					}
					parent = parent->parent;
				}

				assert(parent);         // block begin/end mismatch
				assert(parent->ms < 0); // block begin/end mismatch

				parent->end_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);
				r32 cycles        = (r32)(parent->end_cycle - parent->begin_cycle);
				parent->ms        = 1000.0f * dt * cycles * inv_cycles_count;

				debug_collation.leaf_record = parent;
			}
		}
	}

	constexpr r32 PROFILER_MAX_CHILD_HEIGHT  = 25.0f;
	constexpr r32 PROFILER_OUTLINE_THICKNESS = 0.5f;

	// Draw the profile data
	Record *hovered_record = NULL;

	r32 draw_x = DEBUG_PRESENTATION_X_OFFSET;
	r32 draw_y = render_height - DEBUG_PRESENTATION_BLOCK_Y_OFFSET - profiler_height;

	auto root_record	= debug_collation.root_record;
	r32  children		= (r32)calculate_max_children_count_in_profiler_records(root_record);
	r32  child_height	= min_value(profiler_height / children, PROFILER_MAX_CHILD_HEIGHT);
	r32  font_height	= child_height * 0.8f;

	Vec2 draw_region_position	= vec2(draw_x, draw_y);
	Vec2 draw_region_dimension	= vec2(profiler_width, profiler_height);
	Vec2 top_position			= vec2(draw_x, render_height - DEBUG_PRESENTATION_BLOCK_Y_OFFSET);

	profiler_resizer_position.x = draw_region_position.x + draw_region_dimension.x - PROFILER_RESIZER_SIZE;
	profiler_resizer_position.y = draw_region_position.y;

	if (point_inside_rect(cursor, mm_rect(draw_region_position, draw_region_position + draw_region_dimension))) {
		*set_on_hovered = true;
	}

	Vec4 resizer_color;
	if (point_inside_rect(cursor, mm_rect(profiler_resizer_position, profiler_resizer_position + vec2(PROFILER_RESIZER_SIZE)))) {
		resizer_color = vec4(1, 1, 0);
	} else {
		resizer_color = vec4(1);
	}

	im_unbind_texture();
	im_rect(draw_region_position, draw_region_dimension, vec4(0, 0, 0, 0.8f));

	draw_profiler_timelines_rects(root_record, top_position, profiler_width, child_height, 0, inv_cycles_count, cursor, &hovered_record);

	im_bind_texture(debug_font.texture);
	draw_profiler_timelines_texts(root_record, top_position, profiler_width, child_height, font_height, inv_cycles_count);

	im_unbind_texture();
	im_rect(profiler_resizer_position, vec2(PROFILER_RESIZER_SIZE), resizer_color);
	im_rect_outline2d(draw_region_position, draw_region_dimension, vec4(1), PROFILER_OUTLINE_THICKNESS);

	if (out_hovered_record) {
		*out_hovered_record = hovered_record;
	}

	return draw_y;
}

void debug_service_presentation(r32 framebuffer_w, r32 framebuffer_h) {
	debug_presentation_hovered = false;

	if (debug_service_present) {
		Vec2 cursor = system_get_cursor_position();
		r32 render_height = framebuffer_h - DEBUG_PRESENTATION_Y_OFFSET;

		im_debug_begin(0, framebuffer_w, framebuffer_h, 0);
		
		render_height = draw_header_and_buttons(render_height, framebuffer_w, framebuffer_h, cursor, &debug_presentation_hovered);

		if (debug_menu_icons_value[Menu_Icon_FRAME_TIME]) {
			render_height = draw_frame_time_graph(render_height, cursor, &debug_presentation_hovered);
		}

		Record *hovered_record = nullptr; // overlay
		if (debug_menu_icons_value[Menu_Icon_PROFILER]) {
			render_height = draw_profiler(render_height, cursor, &debug_presentation_hovered, &hovered_record);
		}

		// Rendering Overlays at last
		if (hovered_record) {
			constexpr r32 PROFILER_PRESENTATION_FONT_HEIGHT = 16.0f;
			const Vec2 PROFILER_HOVERED_RECORD_OFFSET = vec2(10.0f, -10.0f);

			debug_presentation_hovered = true;

			r32    cycles = (r32)(hovered_record->end_cycle - hovered_record->begin_cycle) / 1000.0f;
			String name   = hovered_record->name;
			String desc   = hovered_record->id;
			String time   = tprintf("%.3fms (%.3fkclocks)", hovered_record->ms, cycles);

			r32 max_w, max_h;
			max_w = im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, debug_font.info, name).x;
			max_w = max_value(max_w, im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, debug_font.info, desc).x);
			max_w = max_value(max_w, im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, debug_font.info, time).x);
			max_h = PROFILER_PRESENTATION_FONT_HEIGHT * 3;

			Vec2 pos = cursor + PROFILER_HOVERED_RECORD_OFFSET;
			pos.y -= max_h;

			im_unbind_texture();
			im_rect(pos, vec2(max_w, max_h), vec4(0.02f, 0.02f, 0.02f, 0.8f));

			im_bind_texture(debug_font.texture);
			im_text(pos + vec2(0, 2 * PROFILER_PRESENTATION_FONT_HEIGHT), PROFILER_PRESENTATION_FONT_HEIGHT, debug_font.info, name, vec4(1));
			im_text(pos + vec2(0, PROFILER_PRESENTATION_FONT_HEIGHT), PROFILER_PRESENTATION_FONT_HEIGHT, debug_font.info, desc, vec4(1));
			im_text(pos, PROFILER_PRESENTATION_FONT_HEIGHT, debug_font.info, time, vec4(1));
		}
		
		im_end();
	}

	left_button_pressed = false;
}

void timed_frame_presentation(Monospaced_Font &font, r32 framebuffer_w, r32 framebuffer_h) {
	debug_service_presentation(framebuffer_w, framebuffer_h);
	return;
}

bool debug_presentation_is_hovered() {
	return debug_presentation_hovered || profiler_resizing;
}
