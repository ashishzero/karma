#include "debug_service.h"
#include "systems.h"
#include "intrinsics.h"
#include "imgui/imgui.h"
#include "utility.h"

static constexpr u32 MAX_RECORDS_LOGS                    = 5000000; // around 190 MB
static constexpr u32 RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD = 2;
static constexpr u32 MAX_COLLATION_THREADS               = 256;
static constexpr u32 MAX_COLLATION_RECORDS               = MAX_RECORDS_LOGS / 2;

struct Record {
	const char *name;
	const char *id;
	r32         ms;
	u32         begin_cycle;
	u32         end_cycle;

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

static r32             stablilized_frame_time = 0.0f;
static Debug_Collation debug_collation;
static Record *        record_collation_ptr;
static Record *        record_collation_trav;

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
	timed_frame_writer->begin_cycle_value   = intrin__rdtsc();
	timed_frame_writer->begin_counter_value = system_get_counter();
}

void timed_frame_end() {
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

Timed_Block_Match timed_block_begin(const char *id, const char *block_name) {
	assert(timed_frame_writer->records_count < MAX_RECORDS_LOGS);
	s64  index  = intrin_InterlockedIncrement64(&timed_frame_writer->records_count);
	auto record = timed_frame_writer->records + index - 1;

	record->id         = id;
	record->block_name = block_name;
	record->time_stamp = intrin__rdtsc();
	record->thread_id  = (u32)context.id;
	record->type       = Timed_Record_Type_BEGIN;

	return (void *)id;
}

void timed_block_end(Timed_Block_Match value, const char *block_name) {
	assert(timed_frame_writer->records_count < MAX_RECORDS_LOGS);
	s64  index  = intrin_InterlockedIncrement64(&timed_frame_writer->records_count);
	auto record = timed_frame_writer->records + index - 1;

	record->id         = (const char *)value;
	record->block_name = block_name;
	record->time_stamp = intrin__rdtsc();
	record->thread_id  = (u32)context.id;
	record->type       = Timed_Record_Type_END;
}

void recursive_timed_frame_presentation(Record *record, Vec2 position, r32 height, Monospaced_Font &font, int tabs = 0) {
	while (record) {
		String string = tprintf("%*s%-15s : %3.3fms", tabs, "", record->name, record->ms);
		im_text(position, height, font.info, string, vec4(1));
		position.y -= height;

		if (record->child)
			recursive_timed_frame_presentation(record->child, position, height, font, tabs + 1);

		record = record->next;
	}
};

void timed_frame_presentation(Monospaced_Font &font, r32 frame_time, r32 framebuffer_w, r32 framebuffer_h) {
	auto frame  = timed_frame_get();
	auto counts = frame->end_counter_value - frame->begin_counter_value;
	r32  dt     = ((1000000.0f * (r32)counts) / (r32)system_get_frequency()) / 1000000.0f;

	const r32 frame_rate_and_version_height = 24.0f;
	stablilized_frame_time                  = stablilized_frame_time * 0.8f + 0.2f * frame_time;
	String frame_time_string                = tprintf("FrameTime: %.3fms", stablilized_frame_time * 1000.0f);
	String version_string                   = "v" KARMA_VERSION_STRING;

	im_debug_begin(0, framebuffer_w, framebuffer_h, 0);
	im_bind_texture(font.texture);
	im_text(vec2(5.0f, framebuffer_h - frame_rate_and_version_height), frame_rate_and_version_height, font.info, frame_time_string, vec4(1, 1, 0));
	auto size = im_calculate_text_region(frame_rate_and_version_height, font.info, version_string);
	im_text(vec2(framebuffer_w - size.x - 8.0f, framebuffer_h - frame_rate_and_version_height), frame_rate_and_version_height, font.info, version_string, vec4(1, 1, 0));

	const r32 height   = 18.0f;
	const r32 x_offset = 5.0f;
	const r32 y_offset = 0.0f;

	r32 x_pos = x_offset;
	r32 y_pos = framebuffer_h - 2 * frame_rate_and_version_height - y_offset;

	im_text(vec2(x_pos, y_pos), height, font.info, tprintf("FrameTime: %.3f", 1000.0f * dt), vec4(1));
	y_pos -= height;

	if (frame->records_count == 0) {
		im_end();
		return;
	}

	{
		reset_collation_record();

		r32 cycles_count = (r32)(frame->end_cycle_value - frame->begin_cycle_value);

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
					if (parent->id == frame_record->id && parent->name == frame_record->block_name) {
						break;
					}
					parent = parent->parent;
				}

				assert(parent);         // block begin/end mismatch
				assert(parent->ms < 0); // block begin/end mismatch

				parent->end_cycle = frame_record->time_stamp - frame->begin_cycle_value;
				r32 cycles        = (r32)(parent->end_cycle - parent->begin_cycle);
				parent->ms        = 1000.0f * dt * cycles / cycles_count;

				thread_record->leaf_record = parent;
			}
		}
	}

	for (u32 thread_index = 0; thread_index < debug_collation.thread_count; ++thread_index) {
		auto thread_record = debug_collation.thread_records + thread_index;
		auto record        = thread_record->root_record;
		recursive_timed_frame_presentation(record, vec2(x_pos, y_pos), height, font);
	}
	im_end();
}

#if 0
struct Timed_Record_Paired {
	const char *file_name;
	const char *block_name;
	s64         counts;
	u32         line_number;
	u32         thread_id;
	u32         child;
};

struct Record_Id_Index {
	u32 id;
	u32 index;
};

void debug_draw_profile_overlay(Profile_Overlay *overlay) {
	scoped_temporary_allocation();

	auto first_index = (record_history_index + RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD) % MAX_RECORDS_HISTORY_LIST;
	auto last_index  = (record_history_index + MAX_RECORDS_HISTORY_LIST - 1) % MAX_RECORDS_HISTORY_LIST;

	auto &record = records_history[last_index];

	if (record.count) {
		auto                 pair_count      = record.count / 2;
		Timed_Record_Paired *pairs           = (Timed_Record_Paired *)tallocate(sizeof(Timed_Record_Paired) * pair_count);
		Record_Id_Index *    record_id_stack = (Record_Id_Index *)tallocate(sizeof(Record_Id_Index) * pair_count);

		pairs[0].file_name   = record.records[0].file_name;
		pairs[0].block_name  = record.records[0].block_name;
		pairs[0].counts      = -record.records[0].time_stamp;
		pairs[0].line_number = record.records[0].line_number;
		pairs[0].thread_id   = record.records[0].thread_id;
		pairs[0].child       = 0;

		record_id_stack[0] = { record.records[0].record_id, 0 };
		int record_id_top  = 1;

		auto record_ptr = record.records + 1;
		for (u32 read_index = 1, write_index = 1; read_index < record.count; read_index += 1, record_ptr += 1) {
			if (record_id_top && record_id_stack[record_id_top - 1].id == record_ptr->record_id) {
				pairs[record_id_stack[record_id_top - 1].index].counts += record_ptr->time_stamp;
				record_id_top -= 1;
			} else {
				if (record_id_top - 1 >= 0) {
					pairs[record_id_stack[record_id_top - 1].index].child += 1;
				}

				record_id_stack[record_id_top] = { record_ptr->record_id, write_index };
				record_id_top += 1;

				pairs[write_index].file_name   = record_ptr->file_name;
				pairs[write_index].block_name  = record_ptr->block_name;
				pairs[write_index].counts      = -record_ptr->time_stamp;
				pairs[write_index].line_number = record_ptr->line_number;
				pairs[write_index].thread_id   = record_ptr->thread_id;
				pairs[write_index].child       = 0;
				write_index += 1;
			}
		}

		switch (*overlay) {
			case Profile_Overlay_LIST: break;
			case Profile_Overlay_LIST_SORT_HIGH:
				sort(pairs, pair_count, [](Timed_Record_Paired &a, Timed_Record_Paired &b) -> int {
					return a.counts > b.counts;
				});
				break;
			case Profile_Overlay_LIST_SORT_LOW:
				sort(pairs, pair_count, [](Timed_Record_Paired &a, Timed_Record_Paired &b) -> int {
					return a.counts < b.counts;
				});
				break;
			case Profile_Overlay_LIST_SORT_THREAD:
				sort(pairs, pair_count, [](Timed_Record_Paired &a, Timed_Record_Paired &b) -> int {
					return a.thread_id > b.thread_id;
				});
				break;

				invalid_default_case();
		}

		ImGui::Columns(3, "Debug Profile No Sort");
		ImGui::Separator();

		ImGui::Text("Name");
		ImGui::NextColumn();
		if (ImGui::IsItemClicked()) {
			*overlay = Profile_Overlay_LIST;
		}

		auto sort_string = "";
		if (*overlay == Profile_Overlay_LIST_SORT_HIGH || *overlay == Profile_Overlay_LIST_SORT_LOW) {
			sort_string = "*";
		}
		ImGui::Text("kCycles/frame %s", sort_string);
		ImGui::NextColumn();
		if (ImGui::IsItemClicked()) {
			if (*overlay == Profile_Overlay_LIST_SORT_HIGH) {
				*overlay = Profile_Overlay_LIST_SORT_LOW;
			} else if (*overlay == Profile_Overlay_LIST_SORT_LOW) {
				*overlay = Profile_Overlay_LIST_SORT_HIGH;
			} else {
				*overlay = Profile_Overlay_LIST_SORT_HIGH;
			}
		}

		sort_string = "";
		if (*overlay == Profile_Overlay_LIST_SORT_THREAD) {
			sort_string = "*";
		}
		ImGui::Text("Thread Id %s", sort_string);
		ImGui::NextColumn();
		if (ImGui::IsItemClicked()) {
			*overlay = Profile_Overlay_LIST_SORT_THREAD;
		}
		ImGui::Separator();

		auto pair = pairs;
		for (u32 index = 0; index < pair_count; ++index, ++pair) {
			u32 counts = (u32)(pair->counts / 1000);

			ImGui::Selectable(pair->block_name, false, ImGuiSelectableFlags_SpanAllColumns);
			bool hovered = ImGui::IsItemHovered();
			ImGui::NextColumn();

			ImGui::Text("%u", counts);
			ImGui::NextColumn();

			ImGui::Text("%u", pair->thread_id);
			ImGui::NextColumn();

			if (hovered) {
				ImGui::SetTooltip("%s (%u)", pair->file_name, pair->line_number);
			}
		}

		ImGui::Columns(1);
		ImGui::Separator();
	}
}
#endif
