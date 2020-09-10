#pragma once
#include "karma.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600) /* VS 2010 and above */
#	include <intrin.h>

#	define intrin__rdtsc()                             __rdtsc()
#	define intrin__rdtscp(ptr)                         __rdtscp(ptr)
#	define intrin_InterlockedIncrement64(addend)       (s64) _InterlockedIncrement64((__int64 volatile *)addend)
#	define intrin_InterlockedExchange64(target, value) (s64) _InterlockedExchange64((__int64 volatile *)target, (__int64)value);

#else
#	error "Implement stuffs"
#endif
