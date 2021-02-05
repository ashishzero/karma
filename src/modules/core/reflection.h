#pragma once
#include "karma.h"

/*
 * Reflection
 * To use reflection feature, see src/.generated/readme.txt
*/

// Here's the example usage
#if 0

enum Number {
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
};

struct My_Handle {
	size_t size;
	union {
		void *ptr;
		uint32_t h32;
	};
};

struct My_Struct {
	float a, b, c, d;
	attribute("no-display") My_Handle handle;
};

void foo(const My_Struct &data) {

	for (int i = 0; i < enum_count<Number>(); ++i) {
		Number n = enum_index_value<Number>(i);
		String name = enum_string(n);
		printf("Enum String: %s\n", name.data);
	}

	const Type_Info *info = reflect_info(data); // reflect_info<My_Struct>() is also possible
	if (info->id == Type_Id_STRUCT) { // must be true because My_Struct is struct
		auto       struct_info = (Type_Info_Struct *)info; // because id is Type_Id_STRUCT
		String struct_name = info->name;
		s64 member_count = struct_info->member_count;
		const Struct_Member *members    = struct_info->members;
		// members[i].info gives Type_Info for members of the structs
	}

	// reflect_info works for enums, structs, unions and functions
	// by checking the Type_Id, Type_Info can be casted to Type_Info_Struct, Type_Info_Enum, Type_Info_Pointer,
	// Type_Info_Function and Type_Info_Union, for details of the data contained in these structures
	// check the code below
}

#endif

#ifdef RUN_REFLECTION
#	define attribute(...) __attribute__((annotate(#    __VA_ARGS__)))
#else
#	define attribute(...)
#endif

enum Type_Id {
	Type_Id_UNKNOWN,
	Type_Id_BOOL,
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
	Type_Id_STRING,
	Type_Id_DYNAMIC_ARRAY,
	Type_Id_ARRAY_VIEW,
};

template <typename T>
struct Enum_Info {
	static constexpr size_t get_count() {
		assert(!"File not added for reflection!");
		return 0;
	}
	static constexpr s64 get_min_value() {
		assert(!"File not added for reflection!");
		return 0;
	}
	static constexpr s64 get_max_value() {
		assert(!"File not added for reflection!");
		return 0;
	}
	static const T index_value(s64 index) {
		assert(!"File not added for reflection!");
		return (T)0;
	}
	static const String string(T value) {
		assert(!"File not added for reflection!");
		return "";
	}
	static const Array_View<const char *> string_array() {
		assert(!"File not added for reflection!");
		return Array_View<const char *>(nullptr, 0);
	}
};

template <typename T>
constexpr size_t enum_count(T v) {
	return Enum_Info<T>::get_count();
}
template <typename T>
constexpr size_t enum_count() {
	return Enum_Info<T>::get_count();
}

template <typename T>
constexpr s64 enum_min(T v) {
	return Enum_Info<T>::get_min_value();
}
template <typename T>
constexpr s64 enum_min() {
	return Enum_Info<T>::get_min_value();
}

template <typename T>
constexpr s64 enum_max(T v) {
	return Enum_Info<T>::get_max_value();
}
template <typename T>
constexpr s64 enum_max() {
	return Enum_Info<T>::get_max_value();
}

template <typename T>
constexpr T enum_index_value(s64 index) {
	return Enum_Info<T>::index_value();
}

template <typename T>
const String enum_string(T v) {
	return Enum_Info<T>::string(v);
}

template <typename T>
const Array_View<const char *> enum_string_array() {
	return Enum_Info<T>::string_array();
}
template <typename T>
const Array_View<const char *> enum_string_array(T v) {
	return Enum_Info<T>::string_array();
}

struct Type_Info {
	Type_Id id   = Type_Id_UNKNOWN;
	ptrsize size = 0;
	String  name;

	inline Type_Info() {
	}
	inline Type_Info(Type_Id tid, ptrsize sz, String n) {
		id   = tid;
		size = sz;
		name = n;
	}
};

inline bool type_are_same(const Type_Info *a, const Type_Info *b) {
	return a == b;
}

struct Struct_Member {
	String                 name;
	ptrsize                offset;
	ptrsize                attribute_count;
	const String *         attributes;
	const Type_Info *const info;
};

struct Type_Info_Struct : public Type_Info {
	ptrsize					   attribute_count;
	const String *			   attributes;
	ptrsize                    member_count;
	const Struct_Member *const members;
	const Type_Info *const     base;

	inline Type_Info_Struct() :
		attribute_count(0), attributes(0), member_count(0), members(0), base(0) {
	}
	inline Type_Info_Struct(ptrsize sz, String n, ptrsize attr_n, const String *attrs, ptrsize mem_n, const Struct_Member *const mems, const Type_Info *b = 0) :
		Type_Info(Type_Id_STRUCT, sz, n), attribute_count(attr_n), attributes(attrs), member_count(mem_n), members(mems), base(b) {
	}
};

struct Union_Member {
	String           name;
	ptrsize          attribute_count;
	const String *   attributes;
	const Type_Info *info;
};

struct Type_Info_Union : public Type_Info {
	ptrsize				attribute_count;
	const String		*attributes;
	ptrsize             member_count;
	const Union_Member *members;

	inline Type_Info_Union() :
		attribute_count(0), attributes(0), member_count(0), members(0) {
	}
	inline Type_Info_Union(ptrsize sz, String n, ptrsize attr_n, const String *attrs, ptrsize mem_n, const Union_Member *mems) :
		Type_Info(Type_Id_UNION, sz, n), attribute_count(attr_n), attributes(attrs), member_count(mem_n), members(mems) {
	}
};

struct Type_Info_Enum : public Type_Info {
	const Type_Info *const item_type;
	bool is_sequential;
	const Array_View<const char*> item_strings;

	inline Type_Info_Enum() :
		item_type(nullptr), is_sequential(false) {
	}
	inline Type_Info_Enum(ptrsize sz, String n, const Type_Info *const elem_type, bool is_seq, const Array_View<const char*> strings) :
		Type_Info(Type_Id_ENUM, sz, n), item_type(elem_type), is_sequential(is_seq), item_strings(strings) {
	}
};

struct Type_Info_Pointer : public Type_Info {
	const Type_Info *const pointer_to;

	inline Type_Info_Pointer() :
		pointer_to(0) {
	}
	inline Type_Info_Pointer(ptrsize sz, const Type_Info *const ptr) :
		Type_Info(Type_Id_POINTER, sz, "*"), pointer_to(ptr) {
	}
};

struct Type_Info_Static_Array : public Type_Info {
	const size_t           count;
	const Type_Info *const type;

	inline Type_Info_Static_Array() :
		count(0), type(0) {
	}
	inline Type_Info_Static_Array(ptrsize sz, size_t n, const Type_Info *const ptr) :
		Type_Info(Type_Id_STATIC_ARRAY, sz, "[N]"), count(n), type(ptr) {
	}
};

struct Type_Info_Dynamic_Array : public Type_Info {
	const Type_Info *const type;
	inline Type_Info_Dynamic_Array() :
		type(0) {
	}
	inline Type_Info_Dynamic_Array(ptrsize size, const Type_Info* const _type) :
		Type_Info(Type_Id_DYNAMIC_ARRAY, size, "[..]"), type(_type) {
	}
};

struct Type_Info_Array_View : public Type_Info {
	const Type_Info *const type;
	inline Type_Info_Array_View() :
		type(0) {
	}
	inline Type_Info_Array_View(ptrsize size, const Type_Info* const _type) :
		Type_Info(Type_Id_ARRAY_VIEW, size, "[]"), type(_type) {
	}
};

struct Type_Info_Function : public Type_Info {
	const Type_Info *const  return_type;
	ptrsize                 arguments_count;
	const Type_Info *const *arguments;

	inline Type_Info_Function() :
		return_type(0), arguments_count(0), arguments(0) {
	}
	inline Type_Info_Function(const Type_Info *const ptr, ptrsize args_count, const Type_Info *const args[]) :
		Type_Info(Type_Id_FUNCTION, 0, "()"), return_type(ptr), arguments_count(args_count), arguments(args) {
	}
};

template <typename T>
struct Reflect {
	static constexpr Type_Id      id = Type_Id_UNKNOWN;
	static const Type_Info *const info() {
		static const Type_Info i(id, 0, "-unknown-");
		assert(!"File not added for reflection!");
		return &i;
	}
};

template <>
struct Reflect<bool> {
	static constexpr Type_Id      id = Type_Id_BOOL;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(bool), "bool");
		return &i;
	}
};

template <>
struct Reflect<u8> {
	static constexpr Type_Id      id = Type_Id_U8;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(u8), "u8");
		return &i;
	}
};

template <>
struct Reflect<u16> {
	static constexpr Type_Id      id = Type_Id_U16;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(u16), "u16");
		return &i;
	}
};

template <>
struct Reflect<u32> {
	static constexpr Type_Id      id = Type_Id_U32;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(u32), "u32");
		return &i;
	}
};

template <>
struct Reflect<u64> {
	static constexpr Type_Id      id = Type_Id_U64;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(u64), "u64");
		return &i;
	}
};

template <>
struct Reflect<s8> {
	static constexpr Type_Id      id = Type_Id_S8;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(s8), "s8");
		return &i;
	}
};

template <>
struct Reflect<s16> {
	static constexpr Type_Id      id = Type_Id_S16;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(s16), "s16");
		return &i;
	}
};

template <>
struct Reflect<s32> {
	static constexpr Type_Id      id = Type_Id_S32;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(s32), "s32");
		return &i;
	}
};

template <>
struct Reflect<s64> {
	static constexpr Type_Id      id = Type_Id_S64;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(s64), "s64");
		return &i;
	}
};

template <>
struct Reflect<char> {
	static constexpr Type_Id      id = Type_Id_CHAR;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(char), "char");
		return &i;
	}
};

template <>
struct Reflect<void> {
	static constexpr Type_Id      id = Type_Id_VOID;
	static const Type_Info *const info() {
		static const Type_Info i(id, 0, "void");
		return &i;
	}
};

template <>
struct Reflect<r32> {
	static constexpr Type_Id      id = Type_Id_R32;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(r32), "r32");
		return &i;
	}
};

template <>
struct Reflect<r64> {
	static constexpr Type_Id      id = Type_Id_R64;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(r64), "r64");
		return &i;
	}
};

template <>
struct Reflect<String> {
	static constexpr Type_Id id = Type_Id_STRING;
	static const Type_Info *const info() {
		static const Type_Info i(id, sizeof(String), "String");
		return &i;
	}
};

template <typename Type>
struct Reflect<Array<Type>> {
	static constexpr Type_Id id = Type_Id_DYNAMIC_ARRAY;
	static const Type_Info* const info() {
		static const Type_Info_Dynamic_Array i(sizeof(Array<Type>), Reflect<Type>::info());
		return &i;
	}
};

template <typename Type>
struct Reflect<Array_View<Type>> {
	static constexpr Type_Id id = Type_Id_ARRAY_VIEW;
	static const Type_Info* const info() {
		static const Type_Info_Array_View i(sizeof(Array_View<Type>), Reflect<Type>::info());
		return &i;
	}
};

template <typename ReturnType, typename... ArgumentTypes>
struct Reflect<ReturnType(ArgumentTypes...)> {
	static constexpr Type_Id id = Type_Id_FUNCTION;
	static const Type_Info * info() {
        static const Type_Info *const args[] = {
            Reflect<ArgumentTypes>::info()..., 0
        };
        static const Type_Info_Function i(Reflect<ReturnType>::info(), static_count(args) - 1, args);
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
constexpr Type_Id reflect_id(const T &v) {
	return Reflect<T>::id;
}
template <typename T>
constexpr Type_Id reflect_id() {
	return Reflect<T>::id;
}

template <typename T>
constexpr const Type_Info *const reflect_info(const T &v) {
	return Reflect<T>::info();
}
template <typename T>
constexpr const Type_Info *const reflect_info() {
	return Reflect<T>::info();
}
