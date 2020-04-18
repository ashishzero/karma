#include "shared.h"

// TODO: Change the name of this file to shared_new_impl.cpp

void* operator new(ptrsize size, Allocator allocator) {
	return mallocate(size, allocator);
}

void* operator new(ptrsize size) {
	return mallocate(size);
}
