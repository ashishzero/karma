#include "debug_service.h"
#include "systems.h"
#include "intrinsics.h"
#include "random.h"

static constexpr u32 MAX_RECORDS_LOGS                    = 5000000; // around 190 MB
static constexpr u32 RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD = 2;
static constexpr u32 MAX_COLLATION_THREADS               = 256;
static constexpr u32 MAX_COLLATION_RECORDS               = MAX_RECORDS_LOGS / 2;

static constexpr r32 PROFILER_PRESENTATION_RECORD_HEIGHT = 25.0f;
static constexpr r32 PROFILER_PRESENTATION_FONT_HEIGHT   = 20.0f;
static constexpr r32 PROFILER_PRESENTATION_X_OFFSET      = 5.0f;
static constexpr r32 PROFILER_PRESENTATION_Y_OFFSET      = 0.0f;
static constexpr r32 PROFILER_RESIZER_SIZE               = 10.0f;
static constexpr r32 PROFILER_RESIZER_MIN_X              = 50.0f;
static r32           profiler_resizer_x                  = 500.0f;
static bool          profiler_resize_with_mouse          = false;

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

struct Thread_Record {
	Record *root_record;
	Record *leaf_record;
	u32     thread_id;
};

struct Debug_Collation {
	Thread_Record thread_records[MAX_COLLATION_THREADS];
	u32           thread_count;
};

static Handle mutex_handle;
static void * debug_memory;

static Timed_Frame  timed_frames[RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD];
static Timed_Frame *timed_frame_writer;

constexpr int MAX_PRESENTATION_COLORS = 100;
static Color3 presentation_colors[MAX_PRESENTATION_COLORS];

static r32             stablilized_frame_time = 0.0f;
static Debug_Collation debug_collation;
static Record *        record_collation_ptr;
static Record *        record_collation_trav;

bool frame_recording             = true;
bool user_frame_recording_option = true;

bool frame_recording_is_on() {
	return user_frame_recording_option;
}

void frame_recording_turn_on() {
	user_frame_recording_option = true;
}

void frame_recording_turn_off() {
	user_frame_recording_option = false;
}

void frame_recording_toggle() {
	user_frame_recording_option = !user_frame_recording_option;
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
		timed_frames[i].begin_counter_value = 0;
		timed_frames[i].end_counter_value   = 0;
	}

	record_collation_ptr = (Record *)((u8 *)debug_memory + RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD * MAX_RECORDS_LOGS * sizeof(Timed_Record));

	timed_frame_writer = timed_frames;
	mutex_handle       = system_create_mutex();

	for (int color_index = 0; color_index < MAX_PRESENTATION_COLORS; ++color_index) {
		presentation_colors[color_index] = random_color3(0.9f, 0.3f);
	}
}

void debug_service_shutdown() {
	system_destory_mutex(mutex_handle);
	system_virtual_free(debug_memory, 0, Virtual_Memory_RELEASE);
}

Timed_Frame *timed_frame_get() {
	Timed_Frame *frame;
	if (timed_frame_writer == timed_frames) {
		frame = timed_frames + 1;
	} else {
		frame = timed_frames;
	}
	return frame;
}

void timed_frame_begin() {
	if (frame_recording) {
		timed_frame_writer->begin_cycle_value   = intrin__rdtsc();
		timed_frame_writer->begin_counter_value = system_get_counter();
	}
}

void timed_frame_end() {
	if (frame_recording) {
		timed_frame_writer->end_cycle_value   = intrin__rdtsc();
		timed_frame_writer->end_counter_value = system_get_counter();

		system_lock_mutex(mutex_handle, WAIT_INFINITE);
		if (timed_frame_writer == timed_frames) {
			timed_frame_writer = timed_frames + 1;
		} else {
			timed_frame_writer = timed_frames;
		}
		system_unlock_mutex(mutex_handle);

		timed_frame_writer->records_count       = 0;
		timed_frame_writer->begin_counter_value = 0;
		timed_frame_writer->end_counter_value   = 0;
	}

	frame_recording = user_frame_recording_option;
}

Timed_Block_Match timed_block_begin(String id, String block_name) {
	if (frame_recording) {
		assert(timed_frame_writer->records_count < MAX_RECORDS_LOGS);
		s64  index  = intrin_InterlockedIncrement64(&timed_frame_writer->records_count);
		auto record = timed_frame_writer->records + index - 1;

		record->id         = id;
		record->block_name = block_name;
		record->time_stamp = intrin__rdtsc();
		record->thread_id  = (u32)context.id;
		record->type       = Timed_Record_Type_BEGIN;

		return id;
	}

	return String("", 0);
}

void timed_block_end(Timed_Block_Match value, String block_name) {
	if (frame_recording) {
		assert(timed_frame_writer->records_count < MAX_RECORDS_LOGS);
		s64  index  = intrin_InterlockedIncrement64(&timed_frame_writer->records_count);
		auto record = timed_frame_writer->records + index - 1;

		record->id         = value;
		record->block_name = block_name;
		record->time_stamp = intrin__rdtsc();
		record->thread_id  = (u32)context.id;
		record->type       = Timed_Record_Type_END;
	}
}

int calculate_presetation_max_children(Record *record) {
	int children = 1;
	while (record) {
		if (record->child) {
			children += calculate_presetation_max_children(record->child);
		}
		record = record->next;
	}
	return children;
}

void draw_timelines_recursive(Record *record, Vec2 position, r32 width, r32 height, int color_index, r32 inv_cycles, Vec2 cursor_p, Record **hover_record) {
	Vec2 render_position;
	render_position.y = position.y;
	Vec2 dimension;
	dimension.y = height;

	while (record) {
		Vec4 color = vec4(presentation_colors[color_index], 1);
		color_index += 1;
		if (color_index >= MAX_PRESENTATION_COLORS) color_index = 0;

		render_position.x = position.x + width * (r32)record->begin_cycle * inv_cycles;
		dimension.x       = width * (r32)(record->end_cycle - record->begin_cycle) * inv_cycles;

		im_rect(render_position, dimension, color);
		im_rect_outline2d(render_position, dimension, vec4(1), 0.4f);

		if (point_inside_rect(cursor_p, mm_rect(render_position, render_position + dimension))) {
			*hover_record = record;
		}

		if (record->child) {
			draw_timelines_recursive(record->child, position - vec2(0, height), width, height, color_index, inv_cycles, cursor_p, hover_record);
		}

		record = record->next;
	}
}

void write_timelines_info_recursive(Record *record, Vec2 position, r32 width, r32 height, r32 font_height, r32 inv_cycles, Monospaced_Font &font) {
	Vec2 render_position;
	render_position.y = position.y;
	Vec2 dimension;
	dimension.y = height;

	while (record) {
		render_position.x = position.x + width * (r32)record->begin_cycle * inv_cycles;
		dimension.x       = width * (r32)(record->end_cycle - record->begin_cycle) * inv_cycles;

		Vec2 region = im_calculate_text_region(font_height, font.info, record->name);
		if (dimension.x > region.x) {
			// align font in center only if there is enough space for font to be displayed
			render_position.x += (dimension.x - region.x) * 0.5;
		}

		im_text_region(render_position, vec2(dimension.x, font_height), font.info, record->name, vec4(1));

		if (record->child) {
			write_timelines_info_recursive(record->child, position - vec2(0, height), width, height, font_height, inv_cycles, font);
		}

		record = record->next;
	}
}

Vec2 draw_profiler(Vec2 position, r32 width, r32 height, r32 font_height, r32 inv_cycles, Vec2 cursor_p, Monospaced_Font &font, Record **hovered_record) {
	for (u32 thread_index = 0; thread_index < debug_collation.thread_count; ++thread_index) {
		auto thread_record = debug_collation.thread_records + thread_index;
		auto record        = thread_record->root_record;

		r32        children = calculate_presetation_max_children(record);
		const Vec2 bg_off   = vec2(3.0f);

		position.x += bg_off.x;
		position.y -= bg_off.y;

		// Make baground for all the children records, using height and children count
		const Vec2 bg_pos = position - vec2(0, (children - 1) * height + font_height) - bg_off; // font_height for thread id
		const Vec2 bg_dim = vec2(width, children * height + font_height) + 2 * (bg_off);

		im_unbind_texture();
		im_rect(bg_pos, bg_dim, vec4(0, 0, 0, 0.8f));
		im_rect_outline2d(bg_pos, bg_dim, vec4(1), 0.5f);

		position.y -= font_height; // For displaying thread id
		draw_timelines_recursive(record, position, width, height, 20, inv_cycles, cursor_p, hovered_record);

		im_bind_texture(font.texture);
		// we print thread id here to decrease texture rebinding
		im_text_region(position + vec2(0, height), vec2(width, font_height), font.info, tprintf("Thread: %u", thread_record->thread_id), vec4(1));
		position.y += (height - font_height) * 0.5f; // align font to center vertically
		write_timelines_info_recursive(record, position, width, height, font_height, inv_cycles, font);

		// Adjust for next thread
		position = bg_pos;
		position.y -= height;
	}

	// return position to next draw
	return position;
}

void timed_frame_presentation(Monospaced_Font &font, r32 frame_time, r32 framebuffer_w, r32 framebuffer_h) {
	auto frame  = timed_frame_get();
	auto counts = frame->end_counter_value - frame->begin_counter_value;
	r32  dt     = ((1000000.0f * (r32)counts) / (r32)system_get_frequency()) / 1000000.0f;

	const r32 frame_rate_and_version_height = 24.0f;
	stablilized_frame_time                  = stablilized_frame_time * 0.8f + 0.2f * frame_time;
	String frame_time_string                = tprintf("FrameTime: %.3fms", stablilized_frame_time * 1000.0f);
	String version_string                   = "v" KARMA_VERSION_STRING;

	im_debug_begin(0, framebuffer_w, framebuffer_h, 0);
	defer {
		im_end();
	};

	im_bind_texture(font.texture);
	im_text(vec2(5.0f, framebuffer_h - frame_rate_and_version_height), frame_rate_and_version_height, font.info, frame_time_string, vec4(1, 1, 0));
	auto size = im_calculate_text_region(frame_rate_and_version_height, font.info, version_string);
	im_text(vec2(framebuffer_w - size.x - 8.0f, framebuffer_h - frame_rate_and_version_height), frame_rate_and_version_height, font.info, version_string, vec4(1, 1, 0));

	r32 prev_used_height_space = 2.0f * frame_rate_and_version_height;

	if (frame->records_count == 0) {
		return;
	}

	r32 inv_cycles_count = 1.0f / ((r32)(frame->end_cycle_value - frame->begin_cycle_value));
	{
		reset_collation_record();

		Record *record = NULL;

		for (s64 record_index = 0; record_index < frame->records_count; ++record_index) {
			auto frame_record = frame->records + record_index;

			Thread_Record *thread_record = NULL;
			for (u32 thread_index = 0; thread_index < debug_collation.thread_count; ++thread_index) {
				auto thread = debug_collation.thread_records + thread_index;
				if (thread->thread_id == frame_record->thread_id) {
					thread_record = thread;
					break;
				}
			}

			if (!thread_record) {
				assert(debug_collation.thread_count < MAX_COLLATION_THREADS);
				thread_record = debug_collation.thread_records + debug_collation.thread_count;
				debug_collation.thread_count += 1;

				record              = push_collation_record();
				record->child       = NULL;
				record->parent      = NULL;
				record->next        = NULL;
				record->id          = frame_record->id;
				record->name        = frame_record->block_name;
				record->ms          = -1; // we use negative to endicate that END has not been reached
				record->begin_cycle = frame_record->time_stamp - frame->begin_cycle_value;

				thread_record->thread_id   = frame_record->thread_id;
				thread_record->root_record = record;
				thread_record->leaf_record = record;
			} else if (frame_record->type == Timed_Record_Type_BEGIN) {
				if (thread_record->leaf_record->ms > 0) {
					record              = push_collation_record();
					record->child       = NULL;
					record->parent      = thread_record->leaf_record->parent;
					record->next        = NULL;
					record->id          = frame_record->id;
					record->name        = frame_record->block_name;
					record->ms          = -1;
					record->begin_cycle = frame_record->time_stamp - frame->begin_cycle_value;

					thread_record->leaf_record->next = record;
					thread_record->leaf_record       = record;
				} else {
					record              = push_collation_record();
					record->child       = NULL;
					record->parent      = thread_record->leaf_record;
					record->next        = NULL;
					record->id          = frame_record->id;
					record->name        = frame_record->block_name;
					record->ms          = -1;
					record->begin_cycle = frame_record->time_stamp - frame->begin_cycle_value;

					thread_record->leaf_record->child = record;
					thread_record->leaf_record        = record;
				}
			} else {
				auto parent = thread_record->leaf_record;

				while (parent) {
					if (parent->id.data == frame_record->id.data && parent->name.data == frame_record->block_name.data) {
						break;
					}
					parent = parent->parent;
				}

				assert(parent);         // block begin/end mismatch
				assert(parent->ms < 0); // block begin/end mismatch

				parent->end_cycle = frame_record->time_stamp - frame->begin_cycle_value;
				r32 cycles        = (r32)(parent->end_cycle - parent->begin_cycle);
				parent->ms        = 1000.0f * dt * cycles * inv_cycles_count;

				thread_record->leaf_record = parent;
			}
		}
	}

	r32 x_pos = PROFILER_PRESENTATION_X_OFFSET;
	r32 y_pos = framebuffer_h - prev_used_height_space - PROFILER_PRESENTATION_Y_OFFSET;

	r32 profiler_presentation_width = profiler_resizer_x - PROFILER_RESIZER_SIZE;

	auto cursor_p          = system_get_cursor_position();
	auto left_button_state = system_button(Button_LEFT);

	Record *hovered_record = NULL;
	Vec2 next_pos = draw_profiler(vec2(x_pos, y_pos), profiler_presentation_width, PROFILER_PRESENTATION_RECORD_HEIGHT, PROFILER_PRESENTATION_FONT_HEIGHT, inv_cycles_count, cursor_p, font, &hovered_record);

	Vec2 resize_dim   = vec2(PROFILER_RESIZER_SIZE);
	Vec4 resize_color = vec4(1);

	if (profiler_resize_with_mouse) {
		profiler_resizer_x = cursor_p.x - 0.5f * PROFILER_RESIZER_SIZE;
		profiler_resizer_x = max_value(PROFILER_RESIZER_MIN_X, profiler_resizer_x);
		resize_color       = vec4(1, 1, 0);
		resize_dim.x       = resize_dim.x * 1.5f;
	}

	next_pos.x = profiler_resizer_x;
	next_pos.y += PROFILER_PRESENTATION_RECORD_HEIGHT;

	if (point_inside_rect(cursor_p, mm_rect(next_pos, next_pos + resize_dim))) {
		if (left_button_state == State_DOWN) {
			profiler_resize_with_mouse = true;
		}
		resize_color = vec4(1, 1, 0);
		resize_dim.x = resize_dim.x * 1.5f;
	}

	if (left_button_state == State_UP) {
		profiler_resize_with_mouse = false;
	}

	im_unbind_texture();
	im_rect(next_pos, resize_dim, resize_color);

	if (hovered_record) {
		r32    cycles = (r32)(hovered_record->end_cycle - hovered_record->begin_cycle) / 1000.0f;
		String name   = hovered_record->name;
		String desc   = hovered_record->id;
		String time   = tprintf("%.3fms (%.3fkclocks)", hovered_record->ms, cycles);

		r32 max_w, max_h;
		max_w = im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, font.info, name).x;
		max_w = max_value(max_w, im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, font.info, desc).x);
		max_w = max_value(max_w, im_calculate_text_region(PROFILER_PRESENTATION_FONT_HEIGHT, font.info, time).x);
		max_h = PROFILER_PRESENTATION_FONT_HEIGHT * 3;

		const Vec2 overlay_offset = vec2(10.0f, -10.0f - max_h);
		Vec2       pos            = cursor_p + overlay_offset;

		im_unbind_texture();
		im_rect(pos, vec2(max_w, max_h), vec4(0.02f, 0.02f, 0.02f));

		im_bind_texture(font.texture);
		im_text(pos + vec2(0, 2 * PROFILER_PRESENTATION_FONT_HEIGHT), PROFILER_PRESENTATION_FONT_HEIGHT, font.info, name, vec4(1));
		im_text(pos + vec2(0, PROFILER_PRESENTATION_FONT_HEIGHT), PROFILER_PRESENTATION_FONT_HEIGHT, font.info, desc, vec4(1));
		im_text(pos, PROFILER_PRESENTATION_FONT_HEIGHT, font.info, time, vec4(1));
	}
}