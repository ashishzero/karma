#pragma once

#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if !defined(BUILD_DEBUG) && !defined(BUILD_DEVELOPER) && !defined(BUILD_RELEASE)
#	if defined(_DEBUG) || defined(DEBUG)
#		define BUILD_DEBUG
#	elif defined(NDEBUG) || defined(RELEASE)
#		define BUILD_RELEASE
#	endif
#endif

// Platform identification
#if defined(__ANDROID__) || defined(__ANDROID_API__)
#	define TARGET_ANDROID
#elif defined(__gnu_linux__) || defined(__linux__) || defined(linux) || defined(__linux)
#	define TARGET_LINUX
#elif defined(macintosh) || defined(Macintosh)
#	define TARGET_MAC
#elif defined(__APPLE__) && defined(__MACH__)
#	defined TARGET_MAC
#elif defined(__APPLE__)
#	define TARGET_IOS
#elif defined(_WIN64)
#	define TARGET_WINDOWS
#elif defined(__WIN32)
#	error x86 Platform not supported
#else
#	error Platform Identification failed or Unsupported Platform
#endif

#if defined(TARGET_ANDROID) || defined(TARGET_IOS)
#	define TARGET_MOBILE
#endif

// current function name
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#	define CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#	define CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#	define CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
 (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#	define CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#	define CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#	define CURRENT_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#	define CURRENT_FUNCTION __func__
#elif defined(_MSC_VER)
#	define CURRENT_FUNCTION __FUNCSIG__
#else
#	define CURRENT_FUNCTION "_unknown_"
#endif

struct Compile_Info {
	const char *file;
	const int   line;
	const char *current_function;
};

#define CURRENT_FILE __FILE__
#define CURRENT_LINE __LINE__

#define COMPILE_INFO \
	Compile_Info { CURRENT_FILE, CURRENT_LINE, CURRENT_FUNCTION }

#if defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
#	include <signal.h> // raise()
#endif

#ifdef TARGET_LINUX
#	include <endian.h>
#endif

#if defined(_MSC_VER)
#	define trigger_breakpoint() __debugbreak();
#elif ((!defined(__NACL__)) && ((defined(__GNUC__) || defined(__clang__)) && (defined(__i386__) || defined(__x86_64__))))
#	define trigger_breakpoint() __asm__ __volatile__("int $3\n\t")
#elif defined(__386__) && defined(__WATCOMC__)
#	define trigger_breakpoint() _asm { int 0x03 }
#elif defined(HAVE_SIGNAL_H) && !defined(__WATCOMC__)
#	define trigger_breakpoint() raise(SIGTRAP)
#else
#	define trigger_breakpoint() ((int *)0) = 0
#endif

#if defined(BUILD_DEBUG) || defined(BUILD_DEVELOPER)
#	define assert(expression)    \
		if (!(expression)) {      \
			trigger_breakpoint(); \
		}
#	define unimplemented() \
		{ trigger_breakpoint(); }
#	define trigger_if(x)         \
		if (x) {                  \
			trigger_breakpoint(); \
		}
#	define invalid_default_case() \
	default: {                     \
		trigger_breakpoint();      \
	} break
#	define invalid_code_path() trigger_breakpoint()
#else
#	define assert(expression)
#	define unimplemented()
#	define trigger_if(x)
#	define invalid_default_case()
#	define invalid_code_path()
#endif

inline void runtime_assert(bool exp) {
	if (!exp) {
		trigger_breakpoint();
	}
}

#define static_count(a) (s64)(sizeof(a) / sizeof((a)[0]))

// Analysis Macros
#if defined(_MSC_VER) && (_MSC_VER >= 1600) /* VS 2010 and above */
#	include <sal.h>
#	define ANALYSE_PRINTF_FMT _Printf_format_string_
#	define ANALYSE_SCANF_FMT _Scanf_format_string_impl_
#else
#	define ANALYSE_PRINTF_FMT
#	define ANALYSE_SCANF_FMT
#endif

#define bit(p) (1ull << (p))
#define set_bit(n, b) ((n) |= (b))
#define get_bit(n, b) ((n) & (b))
#define clear_bit(n, b) ((n) &= ~(b))
#define toggle_bit(n, b) ((n) ^= (b))
#define consume_bit(n, p, b) (((n) & (((1 << (b)) - 1) << (p))) >> (p))

#define kilo_bytes(n) ((n)*1024u)
#define mega_bytes(n) (kilo_bytes(n) * 1024u)
#define giga_bytes(n) (mega_bytes(n) * 1024u)

// ease of use
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

// int assumed as bool
typedef int booli;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t ptrsize;

typedef unsigned char utf8;
typedef char16_t	  utf16;
typedef char32_t	  utf32;

// for consistency
typedef float  r32;
typedef double r64;

constexpr s8  MIN_INT8  = INT8_MIN;
constexpr s8  MAX_INT8  = INT8_MAX;
constexpr s16 MIN_INT16 = INT16_MIN;
constexpr s16 MAX_INT16 = INT16_MAX;
constexpr s32 MIN_INT32 = INT32_MIN;
constexpr s32 MAX_INT32 = INT32_MAX;
constexpr s64 MIN_INT64 = INT64_MIN;
constexpr s64 MAX_INT64 = INT64_MAX;

constexpr u8  MAX_UINT8  = UINT8_MAX;
constexpr u16 MAX_UINT16 = UINT16_MAX;
constexpr u32 MAX_UINT32 = UINT32_MAX;
constexpr u64 MAX_UINT64 = UINT64_MAX;

constexpr r32 EPSILON_FLOAT = FLT_EPSILON;
constexpr r32 MAX_FLOAT		= FLT_MAX;
constexpr r32 MIN_FLOAT		= FLT_MIN;

constexpr ptrsize MAX_SIZE = SIZE_MAX;

//
//
//

#define ENDIAN_LIL 1234
#define ENDIAN_BIG 4321

#ifdef TARGET_LINUX
#	if __BYTE_ORDER == __BIG_ENDIAN
#		define ENDIAN ENDIAN_BIG
#	elif __BYTE_ORDER == __LITTLE_ENDIAN
#		define ENDIAN ENDIAN_LIL
#	endif
#else
#	if defined(__BYTE_ORDER__)
#		if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#			define ENDIAN ENDIAN_BIG
#		elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#			define ENDIAN ENDIAN_LIL
#		endif
#	elif defined(__FLOAT_WORD_ORDER__)
#		if __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__
#			define ENDIAN ENDIAN_BIG
#		elif __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
#			define ENDIAN ENDIAN_LIL
#		endif
#	else
#		if defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__ARMEB__) || \
		 defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__)
#			define ENDIAN ENDIAN_BIG
#		elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || \
		 defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
#			define ENDIAN ENDIAN_LIL
#		endif
#	endif
#endif

// Assume *ENDIAN_LIL* is not found
#ifndef ENDIAN
#	define ENDIAN ENDIAN_LIL
#endif

inline u16 bswap16(u16 a) {
	a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
	return a;
}

inline u32 bswap32(u32 a) {
	a = ((a & 0x000000FF) << 24) | ((a & 0x0000FF00) << 8) | ((a & 0x00FF0000) >> 8) |
		((a & 0xFF000000) >> 24);
	return a;
}

inline u64 bswap64(u64 a) {
	a = ((a & 0x00000000000000FFULL) << 56) | ((a & 0x000000000000FF00ULL) << 40) |
		((a & 0x0000000000FF0000ULL) << 24) | ((a & 0x00000000FF000000ULL) << 8) |
		((a & 0x000000FF00000000ULL) >> 8) | ((a & 0x0000FF0000000000ULL) >> 24) |
		((a & 0x00FF000000000000ULL) >> 40) | ((a & 0xFF00000000000000ULL) >> 56);
	return a;
}

inline u16 bswap16p(void *a) { return bswap16(*(u16 *)a); }

inline u32 bswap32p(void *a) { return bswap32(*(u32 *)a); }

inline u64 bswap64p(void *a) { return bswap64(*(u64 *)a); }

#if ENDIAN == ENDIAN_LIL
constexpr u32 tag32le(const char (&string)[5]) { return ((((u8)string[0]) << 24) | (((u8)string[1]) << 16) | (((u8)string[2]) << 8) | ((u8)string[3])); }
constexpr u32 tag32be(const char (&string)[5]) { return ((((u8)string[3]) << 24) | (((u8)string[2]) << 16) | (((u8)string[1]) << 8) | ((u8)string[0])); }
#else
constexpr u32 tag32be(const char (&string)[5]) { return ((((u8)string[0]) << 24) | (((u8)string[1]) << 16) | (((u8)string[2]) << 8) | ((u8)string[3])); }
constexpr u32 tag32le(const char (&string)[5]) { return ((((u8)string[3]) << 24) | (((u8)string[2]) << 16) | (((u8)string[1]) << 8) | ((u8)string[0])); }
#endif

#if ENDIAN == ENDIAN_LIL
#	define bswap16_be(a) bswap16(a)
#	define bswap32_be(a) bswap32(a)
#	define bswap64_be(a) bswap64(a)
#	define bswap16p_be(a) bswap16p(a)
#	define bswap32p_be(a) bswap32p(a)
#	define bswap64p_be(a) bswap64p(a)
#	define bswap16_le(a) (a)
#	define bswap32_le(a) (a)
#	define bswap64_le(a) (a)
#	define bswap16p_le(a) (*(u16 *)(a))
#	define bswap32p_le(a) (*(u32 *)(a))
#	define bswap64p_le(a) (*(u64 *)(a))
#else
#	define bswap16_le(a) bswap16(a)
#	define bswap32_le(a) bswap32(a)
#	define bswap64_le(a) bswap64(a)
#	define bswap16p_le(a) bswap16p(a)
#	define bswap32p_le(a) bswap32p(a)
#	define bswap64p_le(a) bswap64p(a)
#	define bswap16_be(a) (a)
#	define bswap32_be(a) (a)
#	define bswap64_be(a) (a)
#	define bswap16p_be(a) (*(u16 *)(a))
#	define bswap32p_be(a) (*(u32 *)(a))
#	define bswap64p_be(a) (*(u64 *)(a))
#endif

#define dereference(ptr, type) (*((type *)(ptr)))

//
//
//

union Handle {
	u32   h32;
	u64   h64;
	void *hptr;

	inline operator bool() { return hptr != 0; }
};

//
// Defer
//
#define CONCAT_INTERNAL(x, y) x##y
#define CONCAT(x, y) CONCAT_INTERNAL(x, y)

template <typename T>
struct ExitScope {
	T lambda;
	ExitScope(T lambda) : lambda(lambda) {}
	~ExitScope() { lambda(); }
};

struct ExitScopeHelp {
	template <typename T>
	ExitScope<T> operator+(T t) {
		return t;
	}
};

#define defer const auto &CONCAT(defer__, CURRENT_LINE) = ExitScopeHelp() + [&]()

//
//
//

const ptrsize MEMORY_ALIGNMENT		= sizeof(ptrsize);
const ptrsize TEMPORARY_MEMORY_SIZE = mega_bytes(32);

enum Allocation_Type { Allocation_Type_NEW,
					   Allocation_Type_RESIZE,
					   Allocation_Type_FREE };

typedef void *(*Allocator_Proc)(Allocation_Type type, ptrsize size, const void *ptr, void *user_ptr);

struct Allocator {
	Allocator_Proc proc = 0;
	void *		   data = 0;
};

struct Temporary_Memory {
	u8 *	base	 = 0;
	u8 *	ptr		 = 0;
	ptrsize capacity = 0;

	inline Temporary_Memory() {}
	inline Temporary_Memory(void *ptr, ptrsize capacity) {
		this->base	 = (u8 *)ptr;
		this->ptr	  = this->base;
		this->capacity = capacity;
	}
};

struct Thread_Context {
	u64				 id;
	Allocator		 allocator;
	Temporary_Memory temp_memory;
	void *			 data;
};

struct Push_Allocator {
	Allocator allocator;
};

extern thread_local Thread_Context context;

inline void *temporary_allocator_proc(Allocation_Type type, ptrsize size, const void *ptr, void *user_ptr) {
	Temporary_Memory *temp = (Temporary_Memory *)&context.temp_memory;

	if (type == Allocation_Type_NEW) {
		ptrsize padding = (MEMORY_ALIGNMENT - ((ptrsize)temp->ptr % MEMORY_ALIGNMENT)) % MEMORY_ALIGNMENT;

		u8 *nxtptr = temp->ptr + size + padding + sizeof(ptrsize); // for storing allocated size
		u8 *endptr = temp->base + temp->capacity;

		if (nxtptr <= endptr) {
			ptrsize *result = (ptrsize *)temp->ptr;
			*result			= size;
			temp->ptr		= nxtptr;
			return (void *)(result + 1);
		}

		return 0;
	} else if (type == Allocation_Type_RESIZE) {
		ptrsize allocated_size = *((ptrsize *)ptr - 1);

		if (ptr == (temp->ptr - allocated_size)) { // if no allocation is done after the given *ptr* allocation
			u8 *beg_ptr = temp->ptr - allocated_size;
			u8 *nxtptr  = beg_ptr + size;
			u8 *endptr  = temp->base + temp->capacity;

			if (nxtptr <= endptr) {
				ptrsize *result = (ptrsize *)beg_ptr - 1;
				*result			= size;
				temp->ptr		= nxtptr;
				return (void *)(result + 1);
			}

			return 0;
		} else { // another allocation is done after *ptr*, so new allocation is required
			void *mem = temporary_allocator_proc(Allocation_Type_NEW, size, 0, user_ptr);

			if (mem) {
				ptrsize allocated_size = *((ptrsize *)ptr - 1);
				memmove(mem, ptr, allocated_size);
			}

			return 0;
		}

	} else if (type == Allocation_Type_FREE) {
		// do nothing
		return 0;
	} else {
		invalid_code_path();
	}

	return 0;
}

constexpr Allocator TEMPORARY_ALLOCATOR = { temporary_allocator_proc, 0 };

inline Push_Allocator push_temporary_allocator() {
	Push_Allocator result;
	result.allocator  = context.allocator;
	context.allocator = TEMPORARY_ALLOCATOR;
	return result;
}

inline void pop_temporary_allocator(Push_Allocator &mark) {
	context.allocator = mark.allocator;
}

#define push_scoped_temporary_allocator()                                     \
	auto CONCAT(push_allocator__, CURRENT_LINE) = push_temporary_allocator(); \
	defer { pop_temporary_allocator(CONCAT(push_allocator__, CURRENT_LINE)); }

inline u8 *get_temporary_allocator_point() { return context.temp_memory.ptr; }

inline void set_temporary_allocator_point(u8 *ptr) {
	assert(ptr >= context.temp_memory.base && ptr <= context.temp_memory.base + context.temp_memory.capacity);
	context.temp_memory.ptr = ptr;
}

#define scoped_temporary_allocation()                                                     \
	auto CONCAT(scope_temp_allocation__, CURRENT_LINE) = get_temporary_allocator_point(); \
	defer { set_temporary_allocator_point(CONCAT(scope_temp_allocation__, CURRENT_LINE)); }

inline void reset_temporary_memory() { context.temp_memory.ptr = context.temp_memory.base; }

inline void *tallocate(ptrsize size) {
	return temporary_allocator_proc(Allocation_Type_NEW, size, 0, 0);
}

inline void *mallocate(ptrsize size, Allocator allocator = context.allocator) {
	return allocator.proc(Allocation_Type_NEW, size, 0, allocator.data);
}

inline void *mreallocate(void *ptr, ptrsize size, Allocator allocator = context.allocator) {
	return allocator.proc(Allocation_Type_RESIZE, size, ptr, allocator.data);
}

inline void mfree(const void *ptr, Allocator allocator = context.allocator) {
	allocator.proc(Allocation_Type_FREE, 0, ptr, allocator.data);
}

void *operator new(ptrsize size, Allocator allocator);
void *operator new(ptrsize size);
void operator delete(void *ptr, Allocator allocator);
void operator delete(void *ptr) noexcept;

//
//
//

// inline void* operator new(ptrsize size, void* ptr) { return ptr; }

template <typename T>
struct Array_View {
	// Using singed type for count because it'll be easy to reverse loop
	s64 count = 0;
	T * data  = 0;

	inline Array_View() {}
	inline Array_View(T *p, s64 n) {
		data  = p;
		count = n;
	}

	inline operator bool() { return count; }

	inline T &operator[](s64 index) {
		assert(index < count);
		return data[index];
	}
};

template <typename T>
struct Array {
	// Using singed type for count because it'll be easy to reverse loop
	s64 count = 0;
	T * data  = 0;

	s64		  capacity  = 0;
	Allocator allocator = { 0, 0 };

	inline Array() {
		allocator = context.allocator;
	}

	inline Array(Allocator_Proc proc, void *data = 0) {
		allocator.proc = proc;
		allocator.data = data;
	}

	inline operator bool() { return count; }

	inline operator Array_View<T>() { return Array_View<T>(data, count); }

	inline T &operator[](s64 index) {
		assert(index < count);
		return data[index];
	}
};

template <typename T>
void array_resize(Array<T> *a, s64 n) {
	if (a->data) {
		a->data		= (T *)a->allocator.proc(Allocation_Type_RESIZE, n * sizeof(T), a->data, a->allocator.data);
		a->capacity = n;
	} else {
		a->data		= (T *)a->allocator.proc(Allocation_Type_NEW, n * sizeof(T), 0, a->allocator.data);
		a->capacity = n;
	}
	if (a->count > a->capacity) a->count = a->capacity;
}

template <typename T>
void array_pack(Array<T> *a) {
	array_resize(a, a->count);
}

template <typename T>
void array_set_count(Array<T> *a, s64 count) {
	if (count > a->capacity) array_resize(a, count);
	a->count = count;
}

template <typename T>
void array_free(Array<T> *a) {
	a->allocator.proc(Allocation_Type_FREE, 0, a->data, a->allocator.data);
	a->count	= 0;
	a->data		= 0;
	a->capacity = 0;
}

template <typename T>
void array_clear(Array<T> *a) {
	a->count = 0;
}

inline s64 array_get_grow_capacity(s64 c, s64 n) {
	s64 geom		 = c + c / 2;
	s64 new_capacity = c + n;
	if (c < new_capacity) return (new_capacity);
	return geom;
}

template <typename T>
void array_add(Array<T> *a, const T &d) {
	if (a->count == a->capacity) {
		s64 c = array_get_grow_capacity(a->capacity, 1);
		array_resize(a, c);
	}
	a->data[a->count] = T{ d };
	a->count++;
}

template <typename T>
void array_append(Array<T> *a, const T *ptr, s64 count) {
	if (a->count == a->capacity) {
		s64 n = capacity_grow(a->capacity, count);
		array_resize(a, n);
	}
	for (s64 i = 0; i < count; ++i) a->data[i + a->count] = ptr[i];
	a->count += count;
}

template <typename T>
void array_append(Array<T> *a, const T *begin, const T *end) {
	array_append(a, begin, end - begin);
}

template <typename T>
void array_append(Array<T> *a, const Array_View<T> &v) {
	array_append(a, v->data, v->count);
}

template <typename T, typename... Args>
void array_append(Array<T> *a, const Args &... args) {
	array_append(a, args...);
}

template <typename T, typename... Args>
void array_emplace(Array<T> *a, const Args &... args) {
	if (a->count == a->capacity) {
		s64 n = array_get_grow_capacity(a->capacity, 1);
		array_resize(a, n);
	}
	a->data[a->count] = T(args...);
	a->count++;
}

#define foreach(index, item, arr) \
    for(s64 keep = 1, index = 0; keep && index < (arr).count; keep = !keep, ++index) \
      for(auto & item = ((arr)[index]); keep; keep = !keep)

//
// UTF-8 Strings
//
struct String {
	// Using singed type for count because it'll be easy to reverse loop
	s64   count = 0; // size in bytes of *data*
	utf8 *data  = 0;

	inline String() {}

	template <s64 n> // for string literals
	constexpr String(const char (&a)[n]) {
		count = n - 1;
		data  = (utf8 *)a;
	}

	inline String(const char *d, s64 length) {
		data  = (utf8 *)d;
		count = length;
	}

	inline String(const utf8 *d, s64 length) {
		data  = (utf8 *)d;
		count = length;
	}

	inline String(Array_View<utf8> view) {
		data  = view.data;
		count = view.count;
	}

	inline utf8 &operator[](s64 index) {
		assert(index < count);
		return data[index];
	}

	inline const utf8 &operator[](s64 index) const {
		assert(index < count);
		return data[index];
	}

	inline operator bool() const { return count; }

	inline operator Array_View<utf8>() { return Array_View<utf8>(data, count); }
};
