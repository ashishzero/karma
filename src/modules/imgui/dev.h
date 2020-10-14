#pragma once
#include "modules/core/karma.h"
#include "modules/core/systems.h"

#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST) || defined(BUILD_DEVELOPER)
#define BUILD_DEVELOPER_SERVICE
#endif

typedef String Timed_Block_Match;

void dev_mode_enable();
void dev_audio_feedback(r32 *samples, u32 size_in_bytes, u32 channel_count, u32 zeroed_size);

bool dev_get_presentation_state();
bool dev_set_presentation_state(bool state);

void dev_render_frame();

//
//
//

void              dev_profiler_timed_frame_begin();
void              dev_profiler_timed_frame_end(r32 frame_time);
Timed_Block_Match dev_profiler_timed_block_begin(String id, String block_name);
void              dev_profiler_timed_block_end(Timed_Block_Match match, String block_name);

struct Timed_Procedure {
	String            name;
	Timed_Block_Match match;
	inline Timed_Procedure(String id, String proc) {
		name  = proc;
		match = dev_profiler_timed_block_begin(id, name);
	}
	inline ~Timed_Procedure() {
		dev_profiler_timed_block_end(match, name);
	}
};

//
//
//

#if defined(BUILD_DEVELOPER_SERVICE)

#define Dev_ModeEnable						dev_mode_enable
#define Dev_AudioFeedback					dev_audio_feedback
#define Dev_GetPresentationState			dev_get_presentation_state
#define Dev_SetPresentationState			dev_set_presentation_state
#define Dev_TogglePresentationState()		dev_set_presentation_state(!debug_get_presentation_state())
#define Dev_RenderFrame						dev_render_frame

#define Dev_TimedFrameBegin			dev_profiler_timed_frame_begin
#define Dev_TimedFrameEnd			dev_profiler_timed_frame_end
#define Dev_TimedBlockBegin(name)	auto name = dev_profiler_timed_block_begin(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name)
#define Dev_TimedBlockEnd(name)		dev_profiler_timed_block_end(name, #name)
#define Dev_TimedSopedInfo(id, proc, line) Timed_Procedure CONCAT_INTERNAL(timed_block_, line)(id, proc)
#define Dev_TimedProcedure() Dev_TimedSopedInfo(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", CURRENT_PROCEDURE, CURRENT_LINE)
#define Dev_TimedScope(name) Dev_TimedSopedInfo(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name, CURRENT_LINE)

#else

#define Dev_ModeEnable()			
#define Dev_AudioFeedback(samples, size_in_bytes, channel_count, zeroed_size)
#define Dev_GetPresentationState()			(false)
#define Dev_SetPresentationState(state)		(false)
#define Dev_TogglePresentationState()	
#define Dev_RenderFrame(w, h)				

#define Dev_TimedFrameBegin()
#define Dev_TimedFrameEnd(frame_time)
#define Dev_TimedBlockBegin(name) 
#define Dev_TimedBlockEnd(name)   
#define Dev_TimedProcedure() 
#define Dev_TimedScope(name) 

#endif
