#pragma once
#include "karma.h"

#ifdef RUN_REFLECTION
#define attribute(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define attribute(...)
#endif

enum Type_Id {
	Type_Id_UNKNOWN,
	Type_Id_S8,
	Type_Id_S16,
	Type_Id_S32,
	Type_Id_S64,
	Type_Id_U8,
	Type_Id_U16,
	Type_Id_U32,
	Type_Id_U64,
	Type_Id_R32,
	Type_Id_R64,
	Type_Id_CHAR,
	Type_Id_VOID,
	Type_Id_POINTER,
	Type_Id_FUNCTION,
	Type_Id_ENUM,
	Type_Id_STRUCT,
	Type_Id_UNION,
	Type_Id_STATIC_ARRAY,
};

inline uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
	size_t i = 0;
	uint32_t hash = 0;
	while (i != length) {
		hash += key[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

struct Type_Info {
	Type_Id id = Type_Id_UNKNOWN;
	ptrsize size = 0;
	String name;
	u32 hash = 0;

	inline Type_Info() {}
	inline Type_Info(Type_Id tid, ptrsize sz, String n) {
		id = tid;
		size = sz;
		name = n;
		hash = jenkins_one_at_a_time_hash((u8 *)this, sz);
	}
};

inline bool type_info_match(const Type_Info * a, const Type_Info * b) {
	return a->hash == b->hash;
}

struct Struct_Member {
	String name;
	ptrsize offset;
	ptrsize attribute_count;
	const String * attributes;
	const Type_Info * const info;
};

struct Type_Info_Struct : public Type_Info {
	ptrsize member_count;
	const Struct_Member * const members;
	const Type_Info * const base;

	inline Type_Info_Struct() : member_count(0), members(0), base(0) {}
	inline Type_Info_Struct(ptrsize sz, String n, ptrsize mem_n, const Struct_Member * const mems, const Type_Info * b = 0)
		: Type_Info(Type_Id_STRUCT, sz, n), member_count(mem_n), members(mems), base(b) {
	}
};

struct Union_Member {
	String name;
	ptrsize attribute_count;
	const String * attributes;
	const Type_Info * info;
};

struct Type_Info_Union : public Type_Info {
	ptrsize member_count;
	const Union_Member * members;

	inline Type_Info_Union() {}
	inline Type_Info_Union(ptrsize sz, String n, ptrsize mem_n, const Union_Member * mems)
		: Type_Info(Type_Id_UNION, sz, n) {
		member_count = mem_n;
		members = mems;
	}
};

struct Type_Info_Enum : public Type_Info {
	ptrsize count;

	inline Type_Info_Enum() {}
	inline Type_Info_Enum(ptrsize sz, String n, ptrsize num)
		: Type_Info(Type_Id_ENUM, sz, n) {
		count = num;
	}
};

struct Type_Info_Pointer : public Type_Info {
	const Type_Info * const pointer_to;

	inline Type_Info_Pointer() : pointer_to(0) {}
	inline Type_Info_Pointer(ptrsize sz, const Type_Info * const ptr) : Type_Info(Type_Id_POINTER, sz, "*"), pointer_to(ptr) {
	}
};

struct Type_Info_Static_Array : public Type_Info {
	const size_t count;
	const Type_Info * const type;

	inline Type_Info_Static_Array() : count(0), type(0) {}
	inline Type_Info_Static_Array(ptrsize sz, size_t n, const Type_Info * const ptr) : 
		Type_Info(Type_Id_STATIC_ARRAY, sz, "[]"), count(n), type(ptr) {
	}
};

struct Type_Info_Function : public Type_Info {
	const Type_Info * const return_type;
	ptrsize arguments_count;
	const Type_Info * const * arguments;

	inline Type_Info_Function() : return_type(0), arguments_count(0), arguments(0) {}
	inline Type_Info_Function(const Type_Info * const ptr, ptrsize args_count, const Type_Info * const args[]) : 
		Type_Info(Type_Id_FUNCTION, 0, "()"), return_type(ptr), arguments_count(args_count), arguments(args) {
	}
};

template <typename T>
struct Enum_Info {
	static constexpr size_t get_count() { return 0; }
	static constexpr s64 get_min_value() { return 0; }
	static constexpr s64 get_max_value() { return 0; }
	static const T index_value(s64 index) { return (T)0; }
	static const String string(T value) { return ""; }
};

template <typename T>
struct Reflect {
	static constexpr Type_Id id = Type_Id_UNKNOWN;
	static const Type_Info * const info() {
		static const Type_Info i(id, 0, "-unknown-");
		return &i;
	}
};

template <>
struct Reflect<u8> {
	static constexpr Type_Id id = Type_Id_U8;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(u8), "u8");
		return &i;
	}
};

template <>
struct Reflect<u16> {
	static constexpr Type_Id id = Type_Id_U16;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(u16), "u16");
		return &i;
	}
};

template <>
struct Reflect<u32> {
	static constexpr Type_Id id = Type_Id_U32;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(u32), "u32");
		return &i;
	}
};

template <>
struct Reflect<u64> {
	static constexpr Type_Id id = Type_Id_U64;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(u64), "u64");
		return &i;
	}
};

template <>
struct Reflect<s8> {
	static constexpr Type_Id id = Type_Id_S8;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(s8), "s8");
		return &i;
	}
};

template <>
struct Reflect<s16> {
	static constexpr Type_Id id = Type_Id_S16;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(s16), "s16");
		return &i;
	}
};

template <>
struct Reflect<s32> {
	static constexpr Type_Id id = Type_Id_S32;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(s32), "s32");
		return &i;
	}
};

template <>
struct Reflect<s64> {
	static constexpr Type_Id id = Type_Id_S64;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(s64), "s64");
		return &i;
	}
};

template <>
struct Reflect<char> {
	static constexpr Type_Id id = Type_Id_CHAR;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(char), "char");
		return &i;
	}
};

template <>
struct Reflect<void> {
	static constexpr Type_Id id = Type_Id_VOID;
	static const Type_Info * const info() {
		static const Type_Info i(id, 0, "void");
		return &i;
	}
};

template <>
struct Reflect<r32> {
	static constexpr Type_Id id = Type_Id_R32;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(r32), "r32");
		return &i;
	}
};

template <>
struct Reflect<r64> {
	static constexpr Type_Id id = Type_Id_R64;
	static const Type_Info * const info() {
		static const Type_Info i(id, sizeof(r64), "r64");
		return &i;
	}
};

template<typename ReturnType, typename... ArgumentTypes>
struct Reflect<ReturnType(ArgumentTypes...)> {
	static constexpr Type_Id id = Type_Id_FUNCTION;
	static const Type_Info * info() {
		static const Type_Info * const args[] = {
			Reflect<ArgumentTypes>::info()..., 0
		};
		static const Type_Info_Function i(Reflect<ReturnType>::info(), static_count(args)-1, args);
		return &i;
	}
};

template <typename T>
struct Reflect<T *> {
	static constexpr Type_Id id = Type_Id_POINTER;
	static const Type_Info * info() {
		static const Type_Info_Pointer i(sizeof(T *), Reflect<T>::info());
		return &i;
	}
};

template <typename T>
struct Reflect<const T *> {
	static constexpr Type_Id id = Type_Id_POINTER;
	static const Type_Info * info() {
		static const Type_Info_Pointer i(sizeof(T *), Reflect<T>::info());
		return &i;
	}
};

template <typename T, size_t n>
struct Reflect<T[n]> {
	static constexpr Type_Id id = Type_Id_STATIC_ARRAY;
	static const Type_Info * info() {
		static const Type_Info_Static_Array i(sizeof(T) * n, n, Reflect<T>::info());
		return &i;
	}
};

template <typename T>
constexpr Type_Id reflect_id(const T & v) { return Reflect<T>::id; }
template <typename T>
constexpr Type_Id reflect_id() { return Reflect<T>::id; }

template <typename T>
constexpr const  Type_Info * const reflect_info(const T & v) { return Reflect<T>::info(); }
template <typename T>
constexpr const  Type_Info * const reflect_info() { return Reflect<T>::info(); }

template <typename T>
constexpr size_t enum_count(T v) { return Enum_Info<T>::get_count(); }
template <typename T>
constexpr size_t enum_count() { return Enum_Info<T>::get_count(); }

template <typename T>
constexpr s64 enum_min(T v) { return Enum_Info<T>::get_min_value(); }
template <typename T>
constexpr s64 enum_min() { return Enum_Info<T>::get_min_value(); }

template <typename T>
constexpr s64 enum_max(T v) { return Enum_Info<T>::get_max_value(); }
template <typename T>
constexpr s64 enum_max() { return Enum_Info<T>::get_max_value(); }

template <typename T>
constexpr T enum_index_value(s64 index) { return Enum_Info<T>::index_value(); }

template <typename T>
const String enum_string(T v) { return Enum_Info<T>::string(v); }
