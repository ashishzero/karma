#include "karma.h"

void *operator new(ptrsize size, Allocator allocator) {
	return mallocate(size, allocator);
}

void *operator new(ptrsize size) {
	return mallocate(size);
}

void operator delete(void *ptr, Allocator allocator) {
	mfree(ptr, allocator);
}

void operator delete(void *ptr) noexcept {
	mfree(ptr);
}