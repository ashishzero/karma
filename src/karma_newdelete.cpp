#include "karma.h"

void *operator new(ptrsize size, Allocator allocator) {
	return memory_allocate(size, allocator);
}

void *operator new(ptrsize size) {
	return memory_allocate(size);
}

void operator delete(void *ptr, Allocator allocator) {
	memory_free(ptr, allocator);
}

void operator delete(void *ptr) noexcept {
	memory_free(ptr);
}
