#pragma once
#include "karma.h"
#include <atomic>

#ifdef TARGET_WINDOWS
#	include <intrin.h>
#	define intrin__rdtsc()                             __rdtsc()
#	define intrin__rdtscp(ptr)                         __rdtscp(ptr)
#else
#error "Not implemented"
#endif

#define memory_barrier_aquire() std::atomic_thread_fence(std::memory_order_acquire)
#define memory_barrier_release() std::atomic_thread_fence(std::memory_order_release)
