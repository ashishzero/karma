#pragma once
#include "karma.h"
#include "gfx_renderer.h"

typedef void *Timed_Block_Match;

enum Timed_Record_Type {
	Timed_Record_Type_BEGIN,
	Timed_Record_Type_END
};

struct Timed_Record {
	const char *      id;
	const char *      block_name;
	s64               time_stamp;
	u32               thread_id;
	Timed_Record_Type type;
};

struct Timed_Frame {
	Timed_Record *records;
	s64           records_count;
	u64           begin_counter_value;
	u64           end_counter_value;
	u64           begin_cycle_value;
	u64           end_cycle_value;
};

void debug_service_initialize();
void debug_service_shutdown();

Timed_Frame *timed_frame_get();

void              timed_frame_begin();
void              timed_frame_end();
Timed_Block_Match timed_block_begin(const char *id, const char *block_name);
void              timed_block_end(Timed_Block_Match match, const char *block_name);

void timed_frame_presentation(Monospaced_Font &font, r32 frame_time, r32 framebuffer_w, r32 framebuffer_h);

//
//
//

struct Timed_Procedure {
	const char *      name;
	Timed_Block_Match match;
	inline Timed_Procedure(const char *id, const char *proc) {
		name  = proc;
		match = timed_block_begin(id, name);
	}
	inline ~Timed_Procedure() {
		timed_block_end(match, name);
	}
};

#define karma_debug_service_initialize() debug_service_initialize()
#define karma_debug_service_shutdown()   debug_service_shutdown()

#define karma_timed_frame_get() timed_frame_get()

#define karma_timed_frame_begin() timed_frame_begin()
#define karma_timed_frame_end()   timed_frame_end()

#define karma_timed_block_begin(name) auto name = timed_block_begin(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name)
#define karma_timed_block_end(name)   timed_block_end(name, #name)

#define karma_timed_scoped_info(id, proc, line)                     \
	Timed_Procedure CONCAT_INTERNAL(timed_block_, line)(id, proc)   \

#define karma_timed_procedure() karma_timed_scoped_info(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", CURRENT_PROCEDURE, CURRENT_LINE)
#define karma_timed_scope(name) karma_timed_scoped_info(CURRENT_FILE "(" MACRO_NUMBER_TO_STRING(CURRENT_LINE) ")", #name, CURRENT_LINE)

#define karma_timed_frame_presentation(font, dt, w, h) timed_frame_presentation(font, dt, w, h)
