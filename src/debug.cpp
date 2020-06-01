#include "debug.h"
#include "intrinsics.h"
#include "random.h"
#include "utility.h"
#include "stream.h"
#include "gfx_renderer.h"
#include "systems.h"

static constexpr r32 DEBUG_PRESENTATION_X_OFFSET		= 5.0f;
static constexpr r32 DEBUG_PRESENTATION_Y_OFFSET		= 2.0f;
static constexpr r32 DEBUG_PRESENTATION_BLOCK_Y_OFFSET  = 5.0f;

static constexpr u32 PROFILER_MAX_TIMED_RECORDS_LOGS				= 5000000; // around 190 MB
static constexpr u32 PROFILER_RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD	= 2;
static constexpr u32 PROFILER_MAX_COLLATION_RECORDS					= PROFILER_MAX_TIMED_RECORDS_LOGS / 2;
static constexpr int PROFILER_MAX_PRESENTATION_COLORS				= 50;
static constexpr r32 PROFILER_BUTTON_SIZE							= 10.0f;
static constexpr r32 PROFILER_MIN_WIDTH								= 450.0f;
static constexpr r32 PROFILER_MIN_HEIGHT							= 100.0f;

static constexpr int FRAME_TIME_MAX_LOGS = 150;

enum Menu_Icon {
	Menu_Icon_FRAME_TIME,
	Menu_Icon_PROFILER,
	Menu_Icon_AUDIO,

	Menu_Icon_COUNT,
};

static const Color4 MENU_ITEM_COLOR_HOVERED		= vec4(1);
static const Color4 MENU_ITEM_COLOR_DISABLED	= vec4(0.2f, 0.2f, 0.2f);

static const Color4 MENU_ITEMS_COLORS[] = {
	vec4(0.9f, 0.2f, 0.2f),
	vec4(0.3f, 0.2f, 0.8f),
	vec4(0.7f, 0.2f, 0.7f),
};
static_assert(static_count(MENU_ITEMS_COLORS) == Menu_Icon_COUNT, "Specify Color for Menu Icon");

struct Debug_Io {
	Vec2 mouse_cursor;
	bool left_button_pressed	= false;
	bool hovered				= false;
	bool should_present			= true;

	Monospaced_Font		font;
	Texture2d_Handle	menu_icons;
	Mm_Rect				menu_icons_rect[Menu_Icon_COUNT];
	Color4				menu_icons_color[Menu_Icon_COUNT];
	bool				menu_icons_value[Menu_Icon_COUNT];
};

enum Timed_Record_Type {
	Timed_Record_Type_BEGIN,
	Timed_Record_Type_END
};

struct Timed_Record {
	String            id;
	String            block_name;
	s64               time_stamp;
	Timed_Record_Type type;
};

struct Timed_Frame {
	Timed_Record *records;
	u32           records_count;
	u32           records_capacity;
	u64           begin_counter_value;
	u64           end_counter_value;
	u64           begin_cycle_value;
	u64           end_cycle_value;
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

struct Profiler {
	Timed_Frame timed_frames[PROFILER_RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD];
	Timed_Frame *timed_frame_write_ptr;
	Timed_Frame *timed_frame_read_ptr;

	Record *root_record;
	Record *leaf_record;

	Record *        record_collation_ptr;
	Record *        record_collation_trav;

	bool recording			= true;
	bool next_recording		= true;

	Color3 block_colors[PROFILER_MAX_PRESENTATION_COLORS];

	Vec2 resizer_position;
	r32 width		= 450.0f;
	r32 height		= 100.0f;
	bool resizing	= false;
};

struct Frame_Time_Recorder {
	r32 stablilized = 0.0f;
	r32 history[FRAME_TIME_MAX_LOGS];
};

static Debug_Io				io;
static Profiler				profiler;
static Frame_Time_Recorder	frame_time_recorder;

void debug_mode_enable() {
	// NOTE: Allocated memory are not freed, because it doesn't matter, they will be freed by OS when app closes

	auto memory = system_virtual_alloc(0, PROFILER_RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD * PROFILER_MAX_TIMED_RECORDS_LOGS * sizeof(Timed_Record) + PROFILER_MAX_COLLATION_RECORDS * sizeof(Record), Virtual_Memory_COMMIT | Virtual_Memory_RESERVE);

	for (u32 i = 0; i < PROFILER_RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD; ++i) {
		profiler.timed_frames[i].records             = ((Timed_Record *)memory) + i * PROFILER_MAX_TIMED_RECORDS_LOGS;
		profiler.timed_frames[i].records_count       = 0;
		profiler.timed_frames[i].records_capacity	= PROFILER_MAX_TIMED_RECORDS_LOGS;
		profiler.timed_frames[i].begin_counter_value = 0;
		profiler.timed_frames[i].end_counter_value   = 0;
	}

	profiler.record_collation_ptr = (Record *)((u8 *)memory + PROFILER_RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD * PROFILER_MAX_TIMED_RECORDS_LOGS * sizeof(Timed_Record));

	profiler.timed_frame_write_ptr = profiler.timed_frames + 0;
	profiler.timed_frame_read_ptr  = profiler.timed_frames + 1;

	for (int color_index = 0; color_index < PROFILER_MAX_PRESENTATION_COLORS; ++color_index) {
		profiler.block_colors[color_index] = random_color3(0.9f, 0.3f);
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

			io.font.info.range = (Monospaced_Font_Glyph_Range *)memory_allocate(size);
			memcpy(io.font.info.range, istream_consume_size(&in, size), size);
			io.font.info.first = (s32)min_c;
			io.font.info.count = (s32)(max_c - min_c + 2);
			io.font.info.advance = advance;

			int w = *istream_consume(&in, int);
			int h = *istream_consume(&in, int);
			int n = *istream_consume(&in, int);
			u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);

			io.font.texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
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

			io.menu_icons = gfx_create_texture2d(w, h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);

			for (u32 y_index = 0; y_index < y_icon_count; ++y_index) {
				for (u32 x_index = 0; x_index < x_icon_count; ++x_index) {
					u32 index = x_index + y_index * x_icon_count;
					if (index >= Menu_Icon_COUNT) break;

					io.menu_icons_rect[index].min.x = (r32)x_index / (r32)x_icon_count;
					io.menu_icons_rect[index].max.y = (r32)y_index / (r32)y_icon_count;
					io.menu_icons_rect[index].max.x = (r32)(x_index + 1) / (r32)x_icon_count;
					io.menu_icons_rect[index].min.y = (r32)(y_index + 1) / (r32)y_icon_count;

					io.menu_icons_color[index] = vec4(1);
				}
			}
		}
	}
}

bool debug_handle_event(Event &event) {
	bool handled = false;

	switch (event.type) {
	case Event_Type_MOUSE_BUTTON_DOWN: {
		if (event.mouse_button.symbol == Button_LEFT) {
			if (point_inside_rect(io.mouse_cursor, mm_rect(profiler.resizer_position, profiler.resizer_position + vec2(PROFILER_BUTTON_SIZE)))) {
				profiler.resizing = true;
			}
		}
	} break;

	case Event_Type_MOUSE_BUTTON_UP: {
		if (event.mouse_button.symbol == Button_LEFT) {
			profiler.resizing	= false;
			io.left_button_pressed = true;
		}
	} break;

	case Event_Type_MOUSE_CURSOR: {
		io.mouse_cursor.x = (r32)event.mouse_cursor.x;
		io.mouse_cursor.y = (r32)event.mouse_cursor.y;
		handled = (profiler.resizing || io.hovered);

		if (profiler.resizing) {
			profiler.width   = max_value(profiler.width + event.mouse_cursor.x_rel, PROFILER_MIN_WIDTH);
			profiler.height  = max_value(profiler.height + event.mouse_cursor.y_rel, PROFILER_MIN_HEIGHT);
		}
	} break;
	}

	return handled;
}

bool debug_get_presentation_state() {
	return io.should_present;
}

void debug_set_presentation_state(bool state) {
	io.should_present = state;
}

bool debug_presentation_is_hovered() {
	return io.hovered || profiler.resizing;
}

inline void reset_collation_record() {
	profiler.record_collation_trav = profiler.record_collation_ptr;
}

inline Record *push_collation_record() {
	assert(profiler.record_collation_trav < profiler.record_collation_ptr + PROFILER_MAX_COLLATION_RECORDS);
	auto ptr = profiler.record_collation_trav;
	profiler.record_collation_trav += 1;
	return ptr;
}

inline Timed_Frame *timed_frame_get() {
	return profiler.timed_frame_read_ptr;
}

r32 draw_header_and_buttons(r32 render_height, r32 framebuffer_w, r32 framebuffer_h, Vec2 cursor, bool *set_on_hovered) {
	const Vec4    HEADER_FONT_COLOR  = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	constexpr r32 HEADER_FONT_HEIGHT = 24.0f;
	constexpr r32 VERSION_X_OFFSET   = 8.0f;

	//
	// Frame Time and Version print
	//

	frame_time_recorder.stablilized		= frame_time_recorder.stablilized * 0.8f + 0.2f * frame_time_recorder.history[0];
	String frame_time_string			= tprintf("FrameTime: %.3fms", frame_time_recorder.stablilized * 1000.0f);
	String version_string				= "v" KARMA_VERSION_STRING;

	r32 draw_y = render_height - HEADER_FONT_HEIGHT;

	im_bind_texture(io.font.texture);
	Vec2 frame_time_draw_pos = vec2(DEBUG_PRESENTATION_X_OFFSET, draw_y);
	auto ft_size = im_calculate_text_region(HEADER_FONT_HEIGHT, io.font.info, frame_time_string);
	im_text(frame_time_draw_pos, HEADER_FONT_HEIGHT, io.font.info, frame_time_string, HEADER_FONT_COLOR);

	if (point_inside_rect(cursor, mm_rect(frame_time_draw_pos, frame_time_draw_pos + ft_size))) {
		*set_on_hovered = true;
	}

	auto v_size = im_calculate_text_region(HEADER_FONT_HEIGHT, io.font.info, version_string);
	im_text(vec2(framebuffer_w - v_size.x - 8.0f, draw_y), HEADER_FONT_HEIGHT, io.font.info, version_string, HEADER_FONT_COLOR);

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

		if (hovering && io.left_button_pressed) {
			io.menu_icons_value[icon_index] = !io.menu_icons_value[icon_index];
		}

		if (hovering) {
			io.menu_icons_color[icon_index] = MENU_ITEM_COLOR_HOVERED;
		} else if (io.menu_icons_value[icon_index]) {
			io.menu_icons_color[icon_index] = MENU_ITEMS_COLORS[icon_index];
		} else {
			io.menu_icons_color[icon_index] = MENU_ITEM_COLOR_DISABLED;
		}

	}

	Vec2 min_icons_region = vec2(icon_draw_start_x, draw_y);
	Vec2 max_icons_region = vec2(icon_draw_start_x + MENU_ICON_X_POSITION * Menu_Icon_COUNT, draw_y + HEADER_FONT_HEIGHT);
	if (point_inside_rect(cursor, mm_rect(min_icons_region, max_icons_region))) {
		*set_on_hovered = true;
	}

	im_bind_texture(io.menu_icons);

	for (int icon_index = 0; icon_index < Menu_Icon_COUNT; ++icon_index) {
		im_rect(icon_positions[icon_index], icon_dimensions[icon_index], io.menu_icons_rect[icon_index], io.menu_icons_color[icon_index]);
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
	r32 max_frame_time = frame_time_recorder.history[0];
	for (int frame_time_index = 1; frame_time_index < FRAME_TIME_LOGS_COUNT; ++frame_time_index) {
		if (frame_time_recorder.history[frame_time_index] > max_frame_time)
			max_frame_time = frame_time_recorder.history[frame_time_index];
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
	r32 prev_y = frame_time_recorder.history[0] * inv_max_frame_time * FRAME_TIME_GRAPH_HEIGHT + draw_y;
	for (int frame_time_index = 1; frame_time_index < FRAME_TIME_LOGS_COUNT; ++frame_time_index) {
		next_y = frame_time_recorder.history[frame_time_index] * inv_max_frame_time * FRAME_TIME_GRAPH_HEIGHT + draw_y;

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
	im_rect_outline2d(bg_draw_pos, vec2(FRAME_TIME_GRAPH_WIDTH, FRAME_TIME_GRAPH_HEIGHT), io.menu_icons_color[Menu_Icon_FRAME_TIME], FRAME_TIME_OUTLINE_THICKNESS);

	im_bind_texture(io.font.texture);
	for (int mark_index = 0; mark_index < static_count(FRAME_TIME_MARK_DT); ++mark_index) {
		if (marked_dt_y_offset[mark_index] > FRAME_TIME_MARK_CRITICAL_Y_OFFSET) {
			String mark_string = tprintf("%.2fms", 1000.0f * FRAME_TIME_MARK_DT[mark_index]);
			im_text(vec2(draw_x, draw_y + marked_dt_y_offset[mark_index] - FRAME_TIME_MARK_FONT_HEIGHT), FRAME_TIME_MARK_FONT_HEIGHT, io.font.info, mark_string, FRAME_TIME_MARK_COLOR);
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
		Vec4 color = vec4(profiler.block_colors[color_index], 1);
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

		Vec2 region = im_calculate_text_region(font_height, io.font.info, record->name);
		if (dimension.x > region.x) {
			// align font in center only if there is enough space for font to be displayed
			render_position.x += (dimension.x - region.x) * 0.5f;
		}

		im_text_region(render_position, vec2(dimension.x, font_height), io.font.info, record->name, vec4(1));

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

			if (profiler.leaf_record == nullptr) {
				record              = push_collation_record();
				record->child       = NULL;
				record->parent      = NULL;
				record->next        = NULL;
				record->id          = frame_record->id;
				record->name        = frame_record->block_name;
				record->ms          = -1; // we use negative to endicate that END has not been reached
				record->begin_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);

				profiler.root_record = record;
				profiler.leaf_record = record;
			} else if (frame_record->type == Timed_Record_Type_BEGIN) {
				if (profiler.leaf_record->ms > 0) {
					record              = push_collation_record();
					record->child       = NULL;
					record->parent      = profiler.leaf_record->parent;
					record->next        = NULL;
					record->id          = frame_record->id;
					record->name        = frame_record->block_name;
					record->ms          = -1;
					record->begin_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);

					profiler.leaf_record->next = record;
					profiler.leaf_record       = record;
				} else {
					record              = push_collation_record();
					record->child       = NULL;
					record->parent      = profiler.leaf_record;
					record->next        = NULL;
					record->id          = frame_record->id;
					record->name        = frame_record->block_name;
					record->ms          = -1;
					record->begin_cycle = (u32)(frame_record->time_stamp - frame->begin_cycle_value);

					profiler.leaf_record->child = record;
					profiler.leaf_record        = record;
				}
			} else {
				auto parent = profiler.leaf_record;

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

				profiler.leaf_record = parent;
			}
		}
	}

	constexpr r32 PROFILER_MAX_CHILD_HEIGHT  = 25.0f;
	constexpr r32 PROFILER_OUTLINE_THICKNESS = 0.5f;

	const Vec4 PROFILER_RECORD_BUTTON_RECORDING_COLOR			= vec4(0.1f, 0.8f, 0.2f);
	const Vec4 PROFILER_RECORD_BUTTON_NOT_RECORDING_COLOR		= vec4(0.8f, 0.2f, 0.1f);
	const Vec4 PROFILER_RECORD_BUTTON_HOVERING_COLOR_ADDITION	= vec4(-0.2f, -0.2f, -0.2f, 0);

	// Draw the profile data
	Record *hovered_record = NULL;

	r32 draw_x = DEBUG_PRESENTATION_X_OFFSET;
	r32 draw_y = render_height - DEBUG_PRESENTATION_BLOCK_Y_OFFSET - profiler.height;

	auto root_record	= profiler.root_record;
	r32  children		= (r32)calculate_max_children_count_in_profiler_records(root_record);
	r32  child_height	= min_value(profiler.height / children, PROFILER_MAX_CHILD_HEIGHT);
	r32  font_height	= child_height * 0.8f;

	Vec2 draw_region_position	= vec2(draw_x, draw_y);
	Vec2 draw_region_dimension	= vec2(profiler.width, profiler.height);
	Vec2 top_position			= vec2(draw_x, render_height - DEBUG_PRESENTATION_BLOCK_Y_OFFSET);

	profiler.resizer_position.x = draw_region_position.x + draw_region_dimension.x - PROFILER_BUTTON_SIZE;
	profiler.resizer_position.y = draw_region_position.y;

	Vec2 recording_state_position;
	recording_state_position.x = profiler.resizer_position.x - PROFILER_BUTTON_SIZE;
	recording_state_position.y = profiler.resizer_position.y;

	Vec4 recording_state_color = vec4(0);

	if (point_inside_rect(cursor, mm_rect(draw_region_position, draw_region_position + draw_region_dimension))) {
		*set_on_hovered = true;

		if (point_inside_rect(cursor, mm_rect(recording_state_position, recording_state_position + vec2(PROFILER_BUTTON_SIZE)))) {
			if (io.left_button_pressed) {
				profiler.next_recording = !profiler.next_recording;
			}
			recording_state_color += PROFILER_RECORD_BUTTON_HOVERING_COLOR_ADDITION;
		}
	}

	if (profiler.next_recording) {
		recording_state_color += PROFILER_RECORD_BUTTON_RECORDING_COLOR;
	} else {
		recording_state_color += PROFILER_RECORD_BUTTON_NOT_RECORDING_COLOR;
	}

	Vec4 resizer_color;
	if (profiler.resizing || point_inside_rect(cursor, mm_rect(profiler.resizer_position, profiler.resizer_position + vec2(PROFILER_BUTTON_SIZE)))) {
		resizer_color = vec4(1, 1, 0);
	} else {
		resizer_color = vec4(1);
	}

	im_unbind_texture();
	im_rect(draw_region_position, draw_region_dimension, vec4(0, 0, 0, 0.8f));

	draw_profiler_timelines_rects(root_record, top_position, profiler.width, child_height, 0, inv_cycles_count, cursor, &hovered_record);

	im_bind_texture(io.font.texture);
	draw_profiler_timelines_texts(root_record, top_position, profiler.width, child_height, font_height, inv_cycles_count);

	im_unbind_texture();
	im_rect(profiler.resizer_position, vec2(PROFILER_BUTTON_SIZE), resizer_color);
	im_rect(recording_state_position, vec2(PROFILER_BUTTON_SIZE), recording_state_color);
	im_rect_outline2d(draw_region_position, draw_region_dimension, io.menu_icons_color[Menu_Icon_PROFILER], PROFILER_OUTLINE_THICKNESS);

	if (out_hovered_record) {
		*out_hovered_record = hovered_record;
	}

	return draw_y;
}

void debug_present(r32 framebuffer_w, r32 framebuffer_h) {
	io.hovered = false;

	if (io.should_present) {
		Vec2 cursor = system_get_cursor_position();
		r32 render_height = framebuffer_h - DEBUG_PRESENTATION_Y_OFFSET;

		im_debug_begin(0, framebuffer_w, framebuffer_h, 0);
		
		render_height = draw_header_and_buttons(render_height, framebuffer_w, framebuffer_h, cursor, &io.hovered);

		if (io.menu_icons_value[Menu_Icon_FRAME_TIME]) {
			render_height = draw_frame_time_graph(render_height, cursor, &io.hovered);
		}

		Record *hovered_record = nullptr; // overlay
		if (io.menu_icons_value[Menu_Icon_PROFILER]) {
			render_height = draw_profiler(render_height, cursor, &io.hovered, &hovered_record);
		}

		// Rendering Overlays at last
		if (hovered_record) {
			constexpr r32 PROFILER_PRESENTATION_FONT_HEIGHT = 16.0f;
			const Vec2 PROFILER_HOVERED_RECORD_OFFSET = vec2(10.0f, -10.0f);

			io.hovered = true;

			r32    cycles = (r32)(hovered_record->end_cycle - hovered_record->begin_cycle) / 1000.0f;
			String name   = hovered_record->name;
			String desc   = hovered_record->id;
			String time   = tprintf("%.3fms (%.3fkclocks)", hovered_record->ms, cycles);

			r32 max_w, max_h;
			max_w = im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, io.font.info, name).x;
			max_w = max_value(max_w, im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, io.font.info, desc).x);
			max_w = max_value(max_w, im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, io.font.info, time).x);
			max_h = PROFILER_PRESENTATION_FONT_HEIGHT * 3;

			Vec2 pos = cursor + PROFILER_HOVERED_RECORD_OFFSET;
			pos.y -= max_h;

			im_unbind_texture();
			im_rect(pos, vec2(max_w, max_h), vec4(0.02f, 0.02f, 0.02f, 0.8f));

			im_bind_texture(io.font.texture);
			im_text(pos + vec2(0, 2 * PROFILER_PRESENTATION_FONT_HEIGHT), PROFILER_PRESENTATION_FONT_HEIGHT, io.font.info, name, vec4(1));
			im_text(pos + vec2(0, PROFILER_PRESENTATION_FONT_HEIGHT), PROFILER_PRESENTATION_FONT_HEIGHT, io.font.info, desc, vec4(1));
			im_text(pos, PROFILER_PRESENTATION_FONT_HEIGHT, io.font.info, time, vec4(1));
		}
		
		im_end();
	}

	io.left_button_pressed = false;
}


void debug_profiler_timed_frame_begin() {
	if (profiler.recording) {
		profiler.timed_frame_write_ptr->begin_cycle_value   = intrin__rdtsc();
		profiler.timed_frame_write_ptr->begin_counter_value = system_get_counter();
	}
}

void debug_profiler_timed_frame_end(r32 frame_time) {
	if (profiler.recording) {
		profiler.timed_frame_write_ptr->end_cycle_value   = intrin__rdtsc();
		profiler.timed_frame_write_ptr->end_counter_value = system_get_counter();

		swap(&profiler.timed_frame_write_ptr, &profiler.timed_frame_read_ptr);

		profiler.timed_frame_write_ptr->records_count       = 0;
		profiler.timed_frame_write_ptr->begin_counter_value = 0;
		profiler.timed_frame_write_ptr->end_counter_value   = 0;
	}

	memmove(frame_time_recorder.history + 1, frame_time_recorder.history, sizeof(r32) * (FRAME_TIME_MAX_LOGS - 1));
	frame_time_recorder.history[0] = frame_time;

	profiler.recording = profiler.next_recording;
}

Timed_Block_Match debug_profiler_timed_block_begin(String id, String block_name) {
	if (profiler.recording) {
		assert(profiler.timed_frame_write_ptr->records_count < PROFILER_MAX_TIMED_RECORDS_LOGS);
		auto record = profiler.timed_frame_write_ptr->records + profiler.timed_frame_write_ptr->records_count;
		profiler.timed_frame_write_ptr->records_count += 1;

		record->id         = id;
		record->block_name = block_name;
		record->time_stamp = intrin__rdtsc();
		record->type       = Timed_Record_Type_BEGIN;

		return id;
	}

	return String("", 0);
}

void debug_profiler_timed_block_end(Timed_Block_Match value, String block_name) {
	if (profiler.recording) {
		assert(profiler.timed_frame_write_ptr->records_count < PROFILER_MAX_TIMED_RECORDS_LOGS);
		auto record = profiler.timed_frame_write_ptr->records + profiler.timed_frame_write_ptr->records_count;
		profiler.timed_frame_write_ptr->records_count += 1;

		record->id         = value;
		record->block_name = block_name;
		record->time_stamp = intrin__rdtsc();
		record->type       = Timed_Record_Type_END;
	}
}
