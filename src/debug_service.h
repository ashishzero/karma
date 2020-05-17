#pragma once
#include "karma.h"

void debug_service_initialize();
void debug_service_shutdown();
void debug_reset_timed_records();

enum Profile_Overlay {
	Profile_Overlay_LIST,
	Profile_Overlay_LIST_SORT_HIGH,
	Profile_Overlay_LIST_SORT_LOW,
	Profile_Overlay_LIST_SORT_THREAD,
};

void debug_draw_profile_overlay(Profile_Overlay *overlay);

struct Timed_Block {
	u32 record_id;
};

Timed_Block timed_block_begin(const char *file_name, const char *block_name, u32 line_number);
void        timed_block_end(Timed_Block value, const char *file_name, const char *block_name, u32 line_number);

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#	define kdInitializeDebugService() debug_service_initialize()
#	define kdShutdownDebugService()   debug_service_shutdown()
#	define kdTimedBlockInfo(file, proc, line)                                          \
		auto CONCAT_INTERNAL(timed_block_, line) = timed_block_begin(file, proc, line); \
		defer {                                                                         \
			timed_block_end(CONCAT_INTERNAL(timed_block_, line), file, proc, line);     \
		}
#	define kdTimedProcedure()       kdTimedBlockInfo(CURRENT_FILE, CURRENT_PROCEDURE, CURRENT_LINE)
#	define kdTimedScope(block_name) kdTimedBlockInfo(CURRENT_FILE, #    block_name, CURRENT_LINE)
#	define kdBeginTimer(block_name) auto block_name = timed_block_begin(CURRENT_FILE, #    block_name, CURRENT_LINE)
#	define kdEndTimer(block_name)   timed_block_end(block_name, CURRENT_FILE, #    block_name, CURRENT_LINE)
#	define kdResetTimedRecords()    debug_reset_timed_records()
#else
#	define kdInitializeDebugService(...)
#	define kdShutdownDebugService(...)
#	define kdTimedProcedure(...)
#	define kdTimedScope(...)
#	define kdBeginTimer(...)
#	define kdEndTimer(...)
#	define kdResetTimedRecords(...)  
#endif
