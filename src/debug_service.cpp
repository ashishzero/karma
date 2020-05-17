#include "debug_service.h"
#include "systems.h"
#include "intrinsics.h"
#include "imgui/imgui.h"
#include "utility.h"
#include <stdlib.h>

struct Timed_Record {
	const char *file_name;
	const char *block_name;
	s64         time_stamp;
	u32         line_number;
	u32         thread_id;
	u32         record_id;
};

struct Timed_Record_List {
	Timed_Record *records;
	u32           count;
};

static constexpr u32 MAX_RECORDS_LOGS                    = 5000000; // around 190 MB
static constexpr u32 MAX_RECORDS_HISTORY_LIST            = 16;
static constexpr u32 RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD = 2;

static Handle mutex_handle;
static void * debug_memory;

static Timed_Record *records_writer;
static s64           record_top_index;

static Timed_Record_List records_history[MAX_RECORDS_HISTORY_LIST];
static s64               record_history_index;

void debug_service_initialize() {
	debug_memory = system_virtual_alloc(0, MAX_RECORDS_HISTORY_LIST * MAX_RECORDS_LOGS * sizeof(Timed_Record), Virtual_Memory_COMMIT | Virtual_Memory_RESERVE);
	for (u32 list_index = 0; list_index < MAX_RECORDS_HISTORY_LIST; ++list_index) {
		records_history[list_index].records = ((Timed_Record *)debug_memory) + list_index * MAX_RECORDS_LOGS;
	}

	records_writer = records_history[0].records;
	mutex_handle   = system_create_mutex();
}

void debug_service_shutdown() {
	system_virtual_free(debug_memory, 0, Virtual_Memory_RELEASE);
	system_destory_mutex(mutex_handle);
}

void debug_reset_timed_records() {
	system_lock_mutex(mutex_handle, WAIT_INFINITE);
	records_history[record_history_index].count = (u32)record_top_index;
	record_history_index                        = (record_history_index + 1) % MAX_RECORDS_HISTORY_LIST;
	records_writer                              = records_history[record_history_index].records;
	record_top_index                            = 0;
	system_unlock_mutex(mutex_handle);
}

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

Timed_Block timed_block_begin(const char *file_name, const char *block_name, u32 line_number) {
	assert(record_top_index < MAX_RECORDS_LOGS);
	s64 index = intrin_InterlockedIncrement64(&record_top_index);
	index -= 1;
	auto record         = records_writer + index;
	record->file_name   = file_name;
	record->block_name  = block_name;
	record->line_number = line_number;
	record->time_stamp  = intrin__rdtsc();
	record->thread_id   = (u32)context.id;
	record->record_id   = (u32)index;

	Timed_Block result;
	result.record_id = (u32)index;
	return result;
}

void timed_block_end(Timed_Block value, const char *file_name, const char *block_name, u32 line_number) {
	assert(record_top_index < MAX_RECORDS_LOGS);
	s64 index = intrin_InterlockedIncrement64(&record_top_index);
	index -= 1;
	auto record         = records_writer + index;
	record->file_name   = file_name;
	record->block_name  = block_name;
	record->line_number = line_number;
	record->time_stamp  = intrin__rdtsc();
	record->thread_id   = (u32)context.id;
	record->record_id   = value.record_id;
}
