#include "modules/core/intrinsics.h"
#include "modules/core/random.h"
#include "modules/core/utility.h"
#include "modules/core/stream.h"
#include "modules/core/stb_sprintf.h"
#include "modules/core/systems.h"
#include "modules/core/intrinsics.h"

#include "imgui.h"
#include "dev.h"

static constexpr u32 PROFILER_MAX_TIMED_RECORDS_LOGS				= 5000000; // around 190 MB
static constexpr u32 PROFILER_RECORD_CIRCULAR_BUFFER_FRAMES_AHEAD	= 2;
static constexpr u32 PROFILER_MAX_COLLATION_RECORDS					= PROFILER_MAX_TIMED_RECORDS_LOGS / 2;
static constexpr int PROFILER_MAX_PRESENTATION_COLORS				= 50;

static constexpr int FRAME_TIME_MAX_LOGS = 150;

static constexpr int AUDIO_VISUALIZER_MAX_SAMPLES = 48000;
static const char *AUDIO_VISUALIZER_CHANNEL_NAMES[] = {
	"Left", "Right"
};
static_assert(static_count(AUDIO_VISUALIZER_CHANNEL_NAMES) == Audio_Channel_COUNT, "Names for channel array not enough");

enum Menu_Icon {
	Menu_FRAME_TIME,
	Menu_AUDIO,
	Menu_PROFILER,

	Menu_COUNT,
};

static const char *MENU_ICON_NAMES[] = {
	"\xe9\x50",
	"\xe9\x11",
	"\xe9\x22",
};
static_assert(Menu_COUNT == static_count(MENU_ICON_NAMES), "Update this!!");

static const Color4 MENU_ITEMS_COLORS[] = {
	vec4(0.9f, 0.2f, 0.2f),
	vec4(0.3f, 0.2f, 0.8f),
	vec4(0.7f, 0.2f, 0.7f),
};
static_assert(static_count(MENU_ITEMS_COLORS) == Menu_COUNT, "Specify Color for Menu Icon");

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
};

struct Frame_Time_Recorder {
	r32 stablilized = 0.0f;
	r32 history[FRAME_TIME_MAX_LOGS];
};

struct Audio_Visualizer {
	r32	history[AUDIO_VISUALIZER_MAX_SAMPLES * Audio_Channel_COUNT];
	int	write_cursor;
};

static constexpr int NOTIFICATION_MAX_COUNT			= 10;
static constexpr r32 NOTIFICATION_FADE_STAY_TIME	= 5;

const Color4 NOTIFICATION_FONT_COLORS_IN[]= {
	vec4(1.0f, 1.0f, 1.0f),
	vec4(0.2f, 0.9f, 0.3f),
	vec4(1.0f, 1.0f, 0.1f),
	vec4(0.9f, 0.1f, 0.3f),
};
static_assert(static_count(NOTIFICATION_FONT_COLORS_IN) == Notification_Level_COUNT, "Notification color not specified for all notification levels");

struct Notification {
	char				msg[NOTIFICATION_MAX_LENGTH];
	int					msg_length;
	Notification_Level  level;
	r32					t;
};

struct Notification_Handler {
	Notification	notifications[NOTIFICATION_MAX_COUNT];
	s64				write_index;
	s64				read_index;
};

static Profiler				profiler;
static Frame_Time_Recorder	frame_time_recorder;
static Audio_Visualizer		audio_visualizer;
static bool					should_present = true;
static bool					menu_icons_value[Menu_COUNT];

static Notification_Handler notification_handler;

void dev_mode_enable() {
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

}

void dev_audio_feedback(r32 *samples, u32 size_in_bytes, u32 channel_count, u32 zeroed_size) {
	u32 sample_count			= size_in_bytes / (sizeof(r32) * channel_count);
	u32 zeroed_size_in_sample	= zeroed_size / (sizeof(r32) * channel_count);

	assert(channel_count <= Audio_Channel_COUNT);

	r32 *write_ptr;
	for (u32 index = 0; index < sample_count; ++index) {
		write_ptr = audio_visualizer.history + audio_visualizer.write_cursor * Audio_Channel_COUNT;

		for (u32 channel_index = 0; channel_index < channel_count; ++channel_index, ++write_ptr, ++samples) {
			*write_ptr = *samples;
		}
		if (Audio_Channel_COUNT - channel_count) {
			memset(write_ptr, 0, (Audio_Channel_COUNT - channel_count) * sizeof(r32));
		}

		audio_visualizer.write_cursor = (audio_visualizer.write_cursor + 1) % AUDIO_VISUALIZER_MAX_SAMPLES;
	}

	if (zeroed_size_in_sample) {
		system_log(LOG_WARNING, "Audio", "Probably audio buffer size small, missed frame");

		if ((audio_visualizer.write_cursor + zeroed_size_in_sample) < AUDIO_VISUALIZER_MAX_SAMPLES) {
			memset(audio_visualizer.history + audio_visualizer.write_cursor * Audio_Channel_COUNT, 0, 
				zeroed_size_in_sample * sizeof(r32) * Audio_Channel_COUNT);
			audio_visualizer.write_cursor += zeroed_size_in_sample;
		} else {
			memset(audio_visualizer.history + audio_visualizer.write_cursor * Audio_Channel_COUNT, 0, 
				(AUDIO_VISUALIZER_MAX_SAMPLES - audio_visualizer.write_cursor) * sizeof(r32) * Audio_Channel_COUNT);
			audio_visualizer.write_cursor = zeroed_size_in_sample - (AUDIO_VISUALIZER_MAX_SAMPLES - audio_visualizer.write_cursor);
			memset(audio_visualizer.history, 0, audio_visualizer.write_cursor * sizeof(r32) * Audio_Channel_COUNT);
		}
	}
}

void dev_notify_level(Notification_Level level, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) {
	Notification *notification = notification_handler.notifications + (notification_handler.write_index % NOTIFICATION_MAX_COUNT);
	intrin_InterlockedIncrement64(&notification_handler.write_index);
	if ((notification_handler.write_index % NOTIFICATION_MAX_COUNT) == (notification_handler.read_index % NOTIFICATION_MAX_COUNT))
		intrin_InterlockedIncrement64(&notification_handler.read_index);
	
	va_list ap;
	va_start(ap, fmt);
	notification->msg_length = stbsp_vsnprintf(notification->msg, NOTIFICATION_MAX_LENGTH, fmt, ap);
	va_end(ap);

	notification->level = level;
	notification->t = NOTIFICATION_FADE_STAY_TIME;
}

bool dev_get_presentation_state() {
	return should_present;
}

bool dev_set_presentation_state(bool state) {
	should_present = state;
	return state;
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

void draw_header_and_buttons() {
	frame_time_recorder.stablilized		= frame_time_recorder.stablilized * 0.8f + 0.2f * frame_time_recorder.history[0];
	String frame_time_string			= tprintf("FrameTime: %.3fms", frame_time_recorder.stablilized * 1000.0f);
	String version_string				= "v" KARMA_VERSION_STRING;

	ImGui::TextColored(vec4(1, 1, 0), "FrameTime: %.3fms", frame_time_recorder.stablilized);

	ImGui::BeginIconFont();

	Vec4 color;
	for (int cindex = 0; cindex < Menu_COUNT; ++cindex) {
		color = menu_icons_value[cindex] ? MENU_ITEMS_COLORS[cindex] : vec4(1, 1, 1);
		ImGui::SameLine();
		ImGui::PushStyleColor(0, color);
		if (ImGui::Button((char *)MENU_ICON_NAMES[cindex])) {
			menu_icons_value[cindex] = !menu_icons_value[cindex];
		}
		ImGui::PopStyleColor();
	}

	ImGui::EndIconFont();
}

void draw_frame_time_graph() {
	// Find max frame time to display the graph in reference to the max frame time
	r32 max_frame_time = frame_time_recorder.history[0];
	r32 min_frame_time = frame_time_recorder.history[0];
	for (int frame_time_index = 1; frame_time_index < FRAME_TIME_MAX_LOGS; ++frame_time_index) {
		if (frame_time_recorder.history[frame_time_index] > max_frame_time)
			max_frame_time = frame_time_recorder.history[frame_time_index];
		if (frame_time_recorder.history[frame_time_index] < min_frame_time)
			min_frame_time = frame_time_recorder.history[frame_time_index];
	}

	ImGui::PlotLines("History", frame_time_recorder.history, static_count(frame_time_recorder.history),
		0, nullptr, min_frame_time, max_frame_time, vec2(0, 75));
}

void draw_profiler_timelines_rects(Record *record, int color_index, r32 inv_cycles, Vec2 cursor, Record **hovered_record, r32 *max_height) {
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetCursorScreenPos();
	r32 item_width = 0.95f * ImGui::GetWindowWidth();

	*max_height += 25.0f;

	while (record) {
		Vec4 color = vec4(profiler.block_colors[color_index], 1);
		color_index += 1;
		if (color_index >= PROFILER_MAX_PRESENTATION_COLORS) color_index = 0;

		auto render_pos = p;
		render_pos.x += item_width * (r32)record->begin_cycle * inv_cycles;

		Vec2 min_rect = render_pos;
		Vec2 dimension = vec2(item_width * (r32)(record->end_cycle - record->begin_cycle) * inv_cycles, 25.0f);
		Vec2 max_rect = render_pos + dimension;

		draw_list->AddRectFilled(min_rect, max_rect, ImGui::ColorConvertFloat4ToU32(color));
		draw_list->AddRect(min_rect, max_rect, 0xffffffff);

		ImVec2 text_draw_pos = min_rect;
		text_draw_pos.y += 0.5f * (dimension.y - ImGui::GetFontSize());

		s64 char_draw_count = record->name.count;
		ImVec2 text_region = ImGui::CalcTextSize((char *)record->name.data, (char *)record->name.data + record->name.count);
		if (dimension.x > text_region.x) {
			text_draw_pos.x += (dimension.x - text_region.x) * 0.5f;
		} else {
			char_draw_count -= lroundf(record->name.count * ((text_region.x - dimension.x) / text_region.x));
		}

		draw_list->AddText(text_draw_pos, 0xffffffff, (char *)record->name.data, (char *)record->name.data + char_draw_count);

		if (point_inside_rect(cursor, mm_rect(min_rect, max_rect))) {
			*hovered_record = record;
		}

		if (record->child) {
			ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + 25.0f));
			draw_profiler_timelines_rects(record->child, color_index, inv_cycles, cursor, hovered_record, max_height);
			ImGui::SetCursorScreenPos(p);
		}

		record = record->next;
	}
}

Record *draw_profiler(Vec2 cursor) {
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

	// Draw the profile data
	Record *hovered_record = NULL;
	auto root_record	= profiler.root_record;

	ImGui::NewLine();
	ImGui::TextColored(vec4(1, 1, 0), "Profiler");
	ImGui::SameLine();

	ImGui::BeginIconFont();
	if (profiler.next_recording) {
		if (ImGui::Button(u8"\xea\x16")) {
			profiler.next_recording = false;
		}
	} else {
		if (ImGui::Button(u8"\xea\x15")) {
			profiler.next_recording = true;
		}
	}
	ImGui::EndIconFont();

	auto p = ImGui::GetCursorScreenPos();

	r32 max_height = 0;
	draw_profiler_timelines_rects(root_record, 0, inv_cycles_count, cursor, &hovered_record, &max_height);

	auto pe = p;
	p.x += 0.95f * ImGui::GetWindowWidth();
	p.y += maximum(max_height, 25.0f * 3);

	ImGui::GetWindowDrawList()->AddRect(p, pe, 0xffffffff);

	p.y += max_height;
	ImGui::SetCursorScreenPos(p);

	return hovered_record;
}

void draw_audio_visualizer() {
	ImGui::NewLine();
	ImGui::TextColored(vec4(1, 1, 0), "Audio: %d channels", Audio_Channel_COUNT);
	for (int channel_index = 0; channel_index < Audio_Channel_COUNT; ++channel_index) {
		ImGui::PlotLines(AUDIO_VISUALIZER_CHANNEL_NAMES[channel_index], 
				audio_visualizer.history, 
				AUDIO_VISUALIZER_MAX_SAMPLES, 
				sizeof(r32) * channel_index, 
				nullptr, -1.0f, 1.0f, vec2(0, 75),
				sizeof(r32) * Audio_Channel_COUNT);
	}
}

void draw_notifications() {
	r32 dt = ImGui::GetIO().DeltaTime;

	constexpr r32 DISTANCE = 10.0f;

	if (notification_handler.read_index < notification_handler.write_index) {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_area_pos = viewport->GetWorkPos();
		ImVec2 work_area_size = viewport->GetWorkSize();
		ImVec2 window_pos = ImVec2(work_area_pos.x + work_area_size.x * 0.5f, work_area_pos.y + DISTANCE);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (ImGui::Begin("Notifications window", nullptr, flags)) {
			for (s64 index = notification_handler.read_index; index != notification_handler.write_index; ++index) {
				auto &notification = notification_handler.notifications[index % NOTIFICATION_MAX_COUNT];
				if (notification.t > 0.0f) {
					ImGui::TextColored(NOTIFICATION_FONT_COLORS_IN[notification.level], notification.msg);
					notification.t -= dt;
				} else {
					intrin_InterlockedIncrement64(&notification_handler.read_index);
				}
			}
			ImGui::End();
		}
	}
}

void dev_render_frame() {
	if (should_present) {
		ImGui::Begin("Debug Information");

		Vec2 cursor = ImGui::GetIO().MousePos;

		draw_header_and_buttons();

		if (menu_icons_value[Menu_FRAME_TIME]) {
			draw_frame_time_graph();
		}

		if (menu_icons_value[Menu_AUDIO]) {
			draw_audio_visualizer();
		}

		Record *hovered_record = nullptr;
		if (menu_icons_value[Menu_PROFILER]) {
			hovered_record = draw_profiler(cursor);
		}

		// Rendering Overlays at last
		if (hovered_record) {
			r32    cycles = (r32)(hovered_record->end_cycle - hovered_record->begin_cycle) / 1000.0f;
			String name   = hovered_record->name;
			String desc   = hovered_record->id;
			String time   = tprintf("%.3fms (%.3fkclocks)", hovered_record->ms, cycles);
			const char *cctime   = null_tprintf("%.3fms (%.3fkclocks)", hovered_record->ms, cycles);

			ImGui::BeginTooltip();
			ImGui::SetTooltip("%s\n%s\n%s", name.data, desc.data, cctime);
			ImGui::EndTooltip();
		}
		
		ImGui::End();
	}

	draw_notifications();
}


void dev_profiler_timed_frame_begin() {
	if (profiler.recording) {
		profiler.timed_frame_write_ptr->begin_cycle_value   = intrin__rdtsc();
		profiler.timed_frame_write_ptr->begin_counter_value = system_get_counter();
	}
}

void dev_profiler_timed_frame_end(r32 frame_time) {
	if (profiler.recording) {
		profiler.timed_frame_write_ptr->end_cycle_value   = intrin__rdtsc();
		profiler.timed_frame_write_ptr->end_counter_value = system_get_counter();

		swap(&profiler.timed_frame_write_ptr, &profiler.timed_frame_read_ptr);

		profiler.timed_frame_write_ptr->records_count       = 0;
		profiler.timed_frame_write_ptr->begin_counter_value = 0;
		profiler.timed_frame_write_ptr->end_counter_value   = 0;
	}

	memmove(frame_time_recorder.history + 1, frame_time_recorder.history, sizeof(r32) * (FRAME_TIME_MAX_LOGS - 1));
	frame_time_recorder.history[0] = frame_time * 1000.0f;

	profiler.recording = profiler.next_recording;
}

Timed_Block_Match dev_profiler_timed_block_begin(String id, String block_name) {
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

void dev_profiler_timed_block_end(Timed_Block_Match value, String block_name) {
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
