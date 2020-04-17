#pragma once
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <thread>

int system_main();

thread_local Thread_Context context;

void *operator new(ptrsize size, Allocator allocator) {
	return mallocate(size, allocator);
}

void operator delete(void *ptr, Allocator allocator) {
	mfree(ptr, allocator);
}

void *operator new(ptrsize size) {
	return mallocate(size);
}

void operator delete(void *ptr) {
	mfree(ptr);
}

void *malloc_allocator(Allocation_Type type, ptrsize size, void *ptr, void *user_ptr) {
	if (type == Allocation_Type_NEW) {
		return malloc(size);
	} else if (type == Allocation_Type_RESIZE) {
		return realloc(ptr, size);
	} else if (type == Allocation_Type_FREE) {
		free(ptr);
		return 0;
	} else {
		invalid_code_path();
	}
	return 0;
}

int main() {
	auto thread_id				= std::this_thread::get_id();
	context.id					= *(u64 *)&thread_id;
	context.allocator.proc		= malloc_allocator;
	context.allocator.data		= 0;
	context.temp_memory			= {};

	void *ptr = malloc(TEMPORARY_MEMORY_SIZE);
	if (ptr == 0) {
		printf("Fatal error: Unable to allocate temporary memory of size: %zu bytes\n", TEMPORARY_MEMORY_SIZE);
		printf("Exiting...\n");
		return 0;
	}

	context.temp_memory = Temporary_Memory(ptr, static_cast<ptrsize>(TEMPORARY_MEMORY_SIZE));

	int result = system_main();

	free(ptr);
	context = {};

	return result;
}
