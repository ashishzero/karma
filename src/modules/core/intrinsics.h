#pragma once
#include "karma.h"
#include <atomic>

#define intrin_MemoryBarrierAquire() std::atomic_thread_fence(std::memory_order_acquire)
#define intrin_MemoryBarrierRelease() std::atomic_thread_fence(std::memory_order_release)

#if defined(_MSC_VER) && (_MSC_VER >= 1600) /* VS 2010 and above */
#	include <intrin.h>

#	define intrin__rdtsc()                             __rdtsc()
#	define intrin__rdtscp(ptr)                         __rdtscp(ptr)
#	define intrin_InterlockedIncrement64(addend)       (s64) _InterlockedIncrement64((__int64 volatile *)addend)
#	define intrin_InterlockedExchange64(target, value) (s64) _InterlockedExchange64((__int64 volatile *)target, (__int64)value);
#	define intrin_InterlockedCompareExchange64(dest, exchange, comperand) (s64)_InterlockedCompareExchange64((volatile long long*)dest, (long long)exchange, (long long)comperand)
#	define intrin_InterlockedIncrement(addend)			(s32)_InterlockedIncrement((volatile long *)addend)
#	define intrin_InterlockedExchange(target, value)	(s32)_InterlockedExchange((volatile long *)target, (long)value)
#	define intrin_InterlockedCompareExchange(dest, exchange, comperand) (s32)_InterlockedCompareExchange((volatile long *)dest, (long)exchange, (long)comperand)

#else
#	error "Implement stuffs"
#endif
