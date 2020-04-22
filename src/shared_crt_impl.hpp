#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "karma.h"
#include "length_string.h"
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#include "length_string.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <thread>

int system_main();

thread_local Thread_Context context;
static String __shared_impl_crt_main_cmd_line__;

void *operator new(ptrsize size, Allocator allocator) {
	return mallocate(size, allocator);
}

void *operator new(ptrsize size) {
	return mallocate(size);
}

void *malloc_allocator(Allocation_Type type, ptrsize size, const void *ptr, void *user_ptr) {
	if (type == Allocation_Type_NEW) {
		return malloc(size);
	} else if (type == Allocation_Type_RESIZE) {
		return realloc((void *)ptr, size);
	} else if (type == Allocation_Type_FREE) {
		free((void *)ptr);
		return 0;
	} else {
		invalid_code_path();
	}
	return 0;
}

inline String get_command_line() {
	return __shared_impl_crt_main_cmd_line__;
}

int main(int argc, char * argv[]) {
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

	Array<String> strings;
	array_resize(&strings, argc);
	for (int i = 0; i < argc; ++i) {
		array_add(&strings, String(argv[i], strlen(argv[i])));
		array_add(&strings, String(" "));
	}
	// we don't need to null terminate but we'll because it makes life a bit easier when working with certain things
	array_add(&strings, String("\0"));
	__shared_impl_crt_main_cmd_line__ = string_concat(strings);
	__shared_impl_crt_main_cmd_line__.count -= 1; // Don't count null terminator
	array_free(&strings);

	int result = system_main();

	free(ptr);
	context = {};

	return result;
}
