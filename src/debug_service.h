#pragma once
#include "karma.h"
#include "gfx_renderer.h"
#include "systems.h"

#if defined(BUILD_DEBUG) || defined(BUILD_DEBUG_FAST) || defined(BUILD_DEVELOPER)
#define BUILD_DEBUG_SERVICE
#endif

typedef String Timed_Block_Match;

enum Timed_Record_Type {
	Timed_Record_Type_BEGIN,
	Timed_Record_Type_END
};

struct Timed_Record {
	String            id;
	String            block_name;
	s64               time_stamp;
	u32               thread_id;
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

bool frame_recording_is_on();
void frame_recording_turn_on();
void frame_recording_turn_off();
void frame_recording_toggle();

void debug_service_initialize();
void debug_service_shutdown();

Timed_Frame *timed_frame_get();

void              timed_frame_begin();
void              timed_frame_end(r32 frame_time);
Timed_Block_Match timed_block_begin(String id, String block_name);
void              timed_block_end(Timed_Block_Match match, String block_name);

void debug_service_handle_event(Event &event);
void timed_frame_presentation(Monospaced_Font &font, r32 framebuffer_w, r32 framebuffer_h);

//
//
//

struct Timed_Procedure {
	String            name;
	Timed_Block_Match match;
	inline Timed_Procedure(String id, String proc) {
		name  = proc;
		match = timed_block_begin(id, name);
	}
	inline ~Timed_Procedure() {
		timed_block_end(match, name);
	}
};

#if defined(BUILD_DEBUG_SERVICE)

#define karma_debug_service_initialize() debug_service_initialize()
#define karma_debug_service_shutdown()   debug_service_shutdown()

#define karma_timed_frame_get() timed_frame_get()

#define karma_timed_frame_begin() timed_frame_begin()
#define karma_timed_frame_end(frame_time)   timed_frame_end(frame_time)

#define karma_timed_block_begin(name) auto name = timed_block_begin(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name)
#define karma_timed_block_end(name)   timed_block_end(name, #name)

#define karma_timed_scoped_info(id, proc, line) \
	Timed_Procedure CONCAT_INTERNAL(timed_block_, line)(id, proc)

#define karma_timed_procedure() karma_timed_scoped_info(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", CURRENT_PROCEDURE, CURRENT_LINE)
#define karma_timed_scope(name) karma_timed_scoped_info(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name, CURRENT_LINE)

#define karma_debug_service_handle_event debug_service_handle_event
#define karma_timed_frame_presentation   timed_frame_presentation

#define karma_frame_recording_is_on    frame_recording_is_on
#define karma_frame_recording_turn_on  frame_recording_turn_on
#define karma_frame_recording_turn_off frame_recording_turn_off
#define karma_frame_recording_toggle   frame_recording_toggle

#else

#define karma_debug_service_initialize() 
#define karma_debug_service_shutdown()   
#define karma_timed_frame_get() (nullptr)
#define karma_timed_frame_begin() 
#define karma_timed_frame_end(frame_time)   
#define karma_timed_block_begin(name) 
#define karma_timed_block_end(name)   
#define karma_timed_procedure() 
#define karma_timed_scope(name) 
#define karma_debug_service_handle_event(...) 
#define karma_timed_frame_presentation(...)   
#define karma_frame_recording_is_on(...)    
#define karma_frame_recording_turn_on(...)  
#define karma_frame_recording_turn_off(...) 
#define karma_frame_recording_toggle(...)   

#endif
