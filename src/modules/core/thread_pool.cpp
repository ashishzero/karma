#include "thread_pool.h"
#include "systems.h"
#include "intrinsics.h"

struct Work_Queue {
	struct Entry {
		Work_Procedure proc;
		void *param;
	};

	Entry entries[WORK_QUEUE_MAX_ENTRY_COUNT];
	s32 volatile read_index;
	s32 volatile write_index;
	s32 volatile work_count;
};

struct Async {
	Work_Queue work_queues[MAX_WORK_QUEUE];
	Handle semaphore;
};

static Async async;

int worker_thread();

u32 async_initialize(u32 thread_count, u32 temp_buffer_size, Allocator temp_allocator) {
	thread_count = mmclamp(MIN_THREAD_COUNT, MAX_THREAD_COUNT, thread_count);

	Builder builder;
	builder.allocator = NULL_ALLOCATOR;
	builder.data = nullptr;
	builder.entry = worker_thread;
	builder.flags = Builder_NONE;
	builder.temporary_buffer_size = temp_buffer_size;

	u32 launched_thread_count = 0;

	Thread_Context *thread = new Thread_Context[thread_count];
	for (u32 index = 0; index < thread_count; ++index) {
		if (!system_thread_create(builder, tprintf("Async %u", index), temp_allocator, thread + index)) {
			break;
		}
		system_thread_run(thread[index]);
		launched_thread_count += 1;
	}

	if (launched_thread_count == 0)
		return launched_thread_count;

	for (u32 index = 0; index < MAX_WORK_QUEUE; ++index) {
		Work_Queue *q = async.work_queues + index;
		q->read_index = 0;
		q->write_index = 0;
		q->work_count = 0;
	}

	async.semaphore = system_create_semaphore(0, thread_count);

	return launched_thread_count;
}

Work_Queue *async_queue(u32 index) {
	assert(index < MAX_WORK_QUEUE);
	return async.work_queues + index;
}

bool async_add_work(Work_Queue *queue, Work_Procedure proc, void *param) {
	s32 check_write_index, write_index;
	s32 next_write_index;
	do {
		check_write_index = queue->write_index;
		next_write_index = (check_write_index + 1) % WORK_QUEUE_MAX_ENTRY_COUNT;
		if (next_write_index == queue->read_index || queue->work_count >= (WORK_QUEUE_MAX_ENTRY_COUNT - 1)) {
			return false;
		}
		write_index = system_interlocked_compare_exchange(&queue->write_index, next_write_index, check_write_index);
	} while (write_index != check_write_index);

	Work_Queue::Entry *entry = queue->entries + write_index;
	entry->proc = proc;
	entry->param = param;

	system_interlocked_increment(&queue->work_count);
	system_signal_semaphore(async.semaphore);

	return true;
}

inline void iasync_do_work(Work_Queue *queue) {
	s32 check_read_index = queue->read_index;
	if (check_read_index == queue->write_index) return;
	s32 new_read_index = (check_read_index + 1) % WORK_QUEUE_MAX_ENTRY_COUNT;
	s32 read_index = system_interlocked_compare_exchange(&queue->read_index, new_read_index, check_read_index);
	if (read_index == check_read_index) {
		Work_Queue::Entry *entry = queue->entries + read_index;
		entry->proc(entry->param);
		system_interlocked_decrement(&queue->work_count);
	}
}

void async_flush_work(Work_Queue *queue) {
	while (queue->work_count) {
		iasync_do_work(queue);
	}
}

int worker_thread() {
	Work_Queue *queue, *q;
	u32 q_index = 0;

	auto work_queues = async.work_queues;

	while (true) {
		queue = nullptr;
		for (q_index = 0; q_index < MAX_WORK_QUEUE; ++q_index) {
			q = work_queues + q_index;
			if (q->work_count && q->read_index != q->write_index) {
				queue = q;
				break;
			}
		}

		if (queue && queue->work_count) {
			iasync_do_work(queue);
		} else {
			system_wait_semaphore(async.semaphore, WAIT_INFINITE);
		}
	}

	return 0;
}
