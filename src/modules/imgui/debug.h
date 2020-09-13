#pragma once
#include "modules/core/karma.h"
#include "modules/core/systems.h"

#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST) || defined(BUILD_DEVELOPER)
#define BUILD_DEBUG_SERVICE
#endif

enum Notification_Level {
	Notification_Level_DEFAULT,
	Notification_Level_SUCCESS,
	Notification_Level_WARN,
	Notification_Level_ERROR,

	Notification_Level_COUNT,
};

constexpr int NOTIFICATION_MAX_LENGTH		= 128;

typedef String Timed_Block_Match;

void debug_mode_enable();
void debug_audio_feedback(r32 *samples, u32 size_in_bytes, u32 channel_count, u32 zeroed_size);

void debug_notify_level(Notification_Level level, ANALYSE_PRINTF_FORMAT_STRING(const char *fmt), ...) ANALYSE_PRINTF_FORMAT(2, 3);

bool debug_get_presentation_state();
bool debug_set_presentation_state(bool state);

void debug_render_frame();

//
//
//

void              debug_profiler_timed_frame_begin();
void              debug_profiler_timed_frame_end(r32 frame_time);
Timed_Block_Match debug_profiler_timed_block_begin(String id, String block_name);
void              debug_profiler_timed_block_end(Timed_Block_Match match, String block_name);

struct Timed_Procedure {
	String            name;
	Timed_Block_Match match;
	inline Timed_Procedure(String id, String proc) {
		name  = proc;
		match = debug_profiler_timed_block_begin(id, name);
	}
	inline ~Timed_Procedure() {
		debug_profiler_timed_block_end(match, name);
	}
};

//
//
//

#if defined(BUILD_DEBUG_SERVICE)

#define Debug_ModeEnable					debug_mode_enable
#define Debug_AudioFeedback					debug_audio_feedback
#define Debug_NotifyLevel(level, fmt, ...)	debug_notify_level(level, fmt, ##__VA_ARGS__)
#define Debug_Notify(fmt, ...)				debug_notify_level(Notification_Level_DEFAULT, fmt, ##__VA_ARGS__)
#define Debug_NotifySuccess(fmt, ...)		debug_notify_level(Notification_Level_SUCCESS, fmt, ##__VA_ARGS__)
#define Debug_NotifyWarn(fmt, ...)			debug_notify_level(Notification_Level_WARN, fmt, ##__VA_ARGS__)
#define Debug_NotifyError(fmt, ...)			debug_notify_level(Notification_Level_ERROR, fmt, ##__VA_ARGS__)
#define Debug_GetPresentationState			debug_get_presentation_state
#define Debug_SetPresentationState			debug_set_presentation_state
#define Debug_TogglePresentationState()		debug_set_presentation_state(!debug_get_presentation_state())
#define Debug_RenderFrame					debug_render_frame

#define Debug_TimedFrameBegin		debug_profiler_timed_frame_begin
#define Debug_TimedFrameEnd			debug_profiler_timed_frame_end
#define Debug_TimedBlockBegin(name) auto name = debug_profiler_timed_block_begin(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name)
#define Debug_TimedBlockEnd(name)   debug_profiler_timed_block_end(name, #name)
#define Debug_TimedSopedInfo(id, proc, line) Timed_Procedure CONCAT_INTERNAL(timed_block_, line)(id, proc)
#define Debug_TimedProcedure() Debug_TimedSopedInfo(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", CURRENT_PROCEDURE, CURRENT_LINE)
#define Debug_TimedScope(name) Debug_TimedSopedInfo(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name, CURRENT_LINE)

#else

#define Debug_ModeEnable()			
#define Debug_AudioFeedback(samples, size_in_bytes, channel_count, zeroed_size)
#define Debug_NotifyDefault(level, ...)		
#define Debug_Notify(...)				
#define Debug_NotifySuccess(...)		
#define Debug_NotifyWarn(...)			
#define Debug_NotifyError(...)			
#define Debug_GetPresentationState()			(false)
#define Debug_SetPresentationState(state)		(false)
#define Debug_TogglePresentationState()	
#define Debug_RenderFrame(w, h)				

#define Debug_TimedFrameBegin()
#define Debug_TimedFrameEnd(frame_time)
#define Debug_TimedBlockBegin(name) 
#define Debug_TimedBlockEnd(name)   
#define Debug_TimedProcedure() 
#define Debug_TimedScope(name) 

#endif
