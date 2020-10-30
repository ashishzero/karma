#pragma once
#include "karma.h"

constexpr u32 MAX_WORK_QUEUE = 2;
constexpr u32 WORK_QUEUE_MAX_ENTRY_COUNT = 128;
constexpr u32 MIN_THREAD_COUNT = 1;
constexpr u32 MAX_THREAD_COUNT = 8;

static_assert(MAX_WORK_QUEUE <= 32);

typedef void(*Work_Procedure)(void *param);

struct Work_Queue;

u32 async_initialize(u32 thread_count, u32 temp_buffer_size, Allocator temp_allocator);
Work_Queue *async_queue(u32 index);
bool async_add_work(Work_Queue *queue, Work_Procedure proc, void *param);
void async_flush_work(Work_Queue *queue);
