#pragma once

#include "karma.h"
#include "reflection.h"
#include "stream.h"

inline void serialize_fmt_text(Ostream *out, String name, const Type_Info *info, char *data, s64 num_of_elements = 1, int tab_count = 0, bool is_array = false) {
	if (name.count) {
		ostream_write_formatted(out, "%*s", tab_count, "");
		ostream_write_buffer(out, name.data, name.count);
		ostream_write_formatted(out, " : ");
	}

	switch (info->id) {
	case Type_Id_S8: {
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", (int)*(s8 *)(data + i * sizeof(s8)));
		}
		if (!is_array) ostream_write_formatted(out, "\n");
	} break;

	case Type_Id_S16:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", (int)*(s16 *)(data + i * sizeof(s16)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_S32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", *(s32 *)(data + i * sizeof(s32)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_S64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%zd, ", *(s64 *)(data + sizeof(s64)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_U8:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%u, ", (u32) * (u8 *)(data + i * sizeof(u8)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_U16:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%u, ", (u32) * (u16 *)(data + i * sizeof(u16)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_U32:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%u, ", (u32) * (u32 *)(data + i * sizeof(u32)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_U64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%zu, ", *(u64 *)(data + i * sizeof(u64)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_R32:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%f, ", *(r32 *)(data + i * sizeof(r32)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_R64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%lf, ", *(r64 *)(data + i * sizeof(r64)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_CHAR:
		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "%d, ", (s32) * (char *)(data + i * sizeof(char)));
		}
		if (!is_array)
			ostream_write_formatted(out, "\n");
		break;
	case Type_Id_VOID:// invalid_path();
	{
		invalid_code_path();
	}
	break;
	case Type_Id_POINTER:// deference 
	{
		auto ptr_info = (Type_Info_Pointer *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			serialize_fmt_text(out, "", ptr_info->pointer_to, (char *)(*(ptrsize *)(data + i * sizeof(ptrsize))), 1, tab_count);
		}
	}
	break;
	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			serialize_fmt_text(out, "", enum_info->item_type, data + i * enum_info->size, 1, tab_count);
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		ostream_write_formatted(out, "\n");
		auto struct_info = (Type_Info_Struct *)info;
		//TODO  : need optimization
		for (s64 j = 0; j < num_of_elements; ++j) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, "{\n");
			if (struct_info->base) {
				serialize_fmt_text(out, ".base", struct_info->base, data + j * struct_info->size, 1, tab_count + 3);
			}
			for (int i = 0; i < struct_info->member_count; ++i) {
				auto mem = struct_info->members + i;
				bool no_serialize = false;
				for (u64 k = 0; k < mem->attribute_count; ++k) {
					if (string_match(mem->attributes[k], "no-serialize")) {
						no_serialize = true;
						break;
					}
				}
				if (!no_serialize)
					serialize_fmt_text(out, mem->name, mem->info, data + j * struct_info->size + mem->offset, 1, tab_count + 3);
			}
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, "},\n");
		}
	}
	break;
	case Type_Id_UNION:
	{
		auto union_info = (Type_Info_Union *)info;
		for (s64 k = 0; k < num_of_elements; ++k) {
			bool write = false;
			for (int j = 0; j < union_info->member_count; ++j) {
				auto mem = union_info->members + j;
				for (u64 i = 0; i < mem->attribute_count; ++i) {
					if (string_match(mem->attributes[i], "write")) {
						write = true;
						break;
					}
				}
				if (write) {
					ostream_write_formatted(out, "{\n");
					serialize_fmt_text(out, mem->name, mem->info, data + k * union_info->size, 1, tab_count + 3);
					ostream_write_formatted(out, "%*s", tab_count, "");
					ostream_write_formatted(out, "},\n");
					break;
				}
			}
			if (!write) {
				ostream_write_formatted(out, "{\n");
				serialize_fmt_text(out, union_info->members->name, union_info->members->info, k * union_info->size + data, 1, tab_count + 3);
				ostream_write_formatted(out, "%*s", tab_count, "");
				ostream_write_formatted(out, "},\n");
			}
		}
	}
	break;
	case Type_Id_STRING:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			String *string_data = (String *)(data + i * sizeof(String));
			ostream_write_formatted(out, "\"");
			ostream_write_buffer(out, string_data->data, string_data->count);
			ostream_write_formatted(out, "\"");
		};
		if (!is_array)
			ostream_write_formatted(out, "\n");
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		if (num_of_elements > 1)
			ostream_write_formatted(out, "{ %zd, [ ", num_of_elements);

		s64 count = ((Type_Info_Static_Array *)info)->count;
		const Type_Info *elem_type_info = ((Type_Info_Static_Array *)info)->type;

		for (s64 i = 0; i < num_of_elements; ++i) {
			ostream_write_formatted(out, "[N] : ");
			ostream_write_formatted(out, "{ %zd, [ ", count);
			serialize_fmt_text(out, "", elem_type_info, data + i * elem_type_info->size * count, count, tab_count + 3, true);
			ostream_write_formatted(out, " ] }\n");
		}
		if (num_of_elements > 1) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, " ] }\n");
		}
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		if (num_of_elements > 1)
			ostream_write_formatted(out, "{ %zd, [ ", num_of_elements);

		for (s64 i = 0; i < num_of_elements; ++i) {
			Array<char> *array = (Array<char> *)(data + i * sizeof(Array<char>));
			s64 count = array->count;

			ostream_write_formatted(out, "[..] : ");
			ostream_write_formatted(out, "{ %zd, [ ", count);
			serialize_fmt_text(out, "", ((Type_Info_Dynamic_Array *)info)->type, array->data, count, tab_count + 3, true);
			ostream_write_formatted(out, " ] }\n");
		}
		if (num_of_elements > 1) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, " ] }\n");
		}
	}
	break;
	case Type_Id_ARRAY_VIEW:
	{
		if (num_of_elements > 1)
			ostream_write_formatted(out, "{ %zd, [ ", num_of_elements);

		const Type_Info *elem_type_info = ((Type_Info_Array_View *)info)->type;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array_View<char> *view = (Array_View<char> *)(data + i * sizeof(Array_View<char>));
			s64 count = view->count;

			ostream_write_formatted(out, "[] : ");
			ostream_write_formatted(out, "{ %zd, [ ", count);
			serialize_fmt_text(out, "", elem_type_info, view->data, count, tab_count + 3, true);
			ostream_write_formatted(out, " ] }\n");
		}
		if (num_of_elements > 1) {
			ostream_write_formatted(out, "%*s", tab_count, "");
			ostream_write_formatted(out, " ] }");
		}
		ostream_write_formatted(out, "\n");
	} break;
	}
}

struct Parse_State {
	Token *start;
	Token *end;
	Token *current;
};

inline bool parsing(Parse_State *w) {
	return w->current < w->end;
}

inline Token *parse_next(Parse_State *w) {
	if (parsing(w)) {
		w->current += 1;
		return w->current - 1;
	}
	return nullptr;
}

inline bool parse_require_token(Parse_State *w, String string) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_IDENTIFIER && string_match(string, t->content))
		return true;
	return false;
}

template <typename T>
inline bool parse_require_integer(Parse_State *w, T *d) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_INTEGER_LITERAL) {
		*d = (T)t->value.integer;
		return true;
	}
	return false;
}

template <typename T>
inline bool parse_require_real(Parse_State *w, T *d) {
	auto t = parse_next(w);
	if (t) {
		if (t->kind == Token_Kind_INTEGER_LITERAL) {
			*d = (T)t->value.integer;
			return true;
		}
		else if (t->kind == Token_Kind_REAL_LITERAL) {
			*d = (T)t->value.real;
			return true;
		}
	}
	return false;
}

inline bool parse_require_token(Parse_State *w, Token_Kind kind) {
	auto t = parse_next(w);
	if (t && t->kind == kind)
		return true;
	return false;
}

inline bool parse_require_string(Parse_State *w, String *d) {
	auto t = parse_next(w);
	if (t && t->kind == Token_Kind_DQ_STRING) {
		*d = t->value.string;
		return true;
	}
	return false;
}

template <typename T>
inline bool parse_integer_array(Parse_State *w, s64 num_of_elements, T *data) {
	for (s64 i = 0; i < num_of_elements; ++i) {
		if (!parse_require_integer<T>(w, data + i) ||
			!parse_require_token(w, Token_Kind_COMMA)) {
			return false;
		}
	}
	if (num_of_elements > 1) {
		if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
			!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
			return false;
		}
	}
	return true;
}

inline bool parse_char_array(Parse_State *w, s64 num_of_elements, char *data) {
	s32 temp;
	for (s64 i = 0; i < num_of_elements; ++i) {
		if (!parse_require_integer<s32>(w, &temp) ||
			!parse_require_token(w, Token_Kind_COMMA)) {
			return false;
		}
		*(data + i) = temp;
	}
	if (num_of_elements > 1) {
		if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
			!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
			return false;
		}
	}
	return true;
}

inline bool deserialize_fmt_text(Parse_State *w, String name, const Type_Info *info, char *data, s64 num_of_elements, int tab_count) {
	if (name.count) {
		if (!parse_require_token(w, name) || !parse_require_token(w, Token_Kind_COLON)) {
			return false;
		}
	}

	switch (info->id) {
	case Type_Id_S8:
	{
		if (!parse_integer_array<s8>(w, num_of_elements, (s8 *)data))
			return false;
	}
	break;
	case Type_Id_S16:
	{
		if (!parse_integer_array<s16>(w, num_of_elements, (s16 *)data))
			return false;
	}
	break;
	case Type_Id_S32:
	{
		if (!parse_integer_array<s32>(w, num_of_elements, (s32 *)data))
			return false;
	}
	break;
	case Type_Id_S64:
	{
		if (!parse_integer_array<s64>(w, num_of_elements, (s64 *)data))
			return false;
	}
	break;
	case Type_Id_U8:
	{
		if (!parse_integer_array<u8>(w, num_of_elements, (u8 *)data))
			return false;
	}
	break;
	case Type_Id_U16:
	{
		if (!parse_integer_array<u16>(w, num_of_elements, (u16 *)data))
			return false;
	}
	break;
	case Type_Id_U32:
	{
		if (!parse_integer_array<u32>(w, num_of_elements, (u32 *)data))
			return false;
	}
	break;
	case Type_Id_U64:
	{
		if (!parse_integer_array<u64>(w, num_of_elements, (u64 *)data))
			return false;
	}
	break;
	case Type_Id_R32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!parse_require_real<r32>(w, (r32 *)data + i) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_R64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!parse_require_real<r32>(w, (r32 *)data + i) ||
				!parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_CHAR:
	{
		if (!parse_char_array(w, num_of_elements, (char *)data))
			return false;
	}
	break;
	case Type_Id_VOID:// invalid_path();
	{
		invalid_code_path();
	}
	break;
	case Type_Id_POINTER:// deference 
	{
		auto ptr_info = (Type_Info_Pointer *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			(*(ptrsize *)(data + i * sizeof(ptrsize))) = (ptrsize)memory_allocate(ptr_info->pointer_to->size);
			if (!deserialize_fmt_text(w, "", ptr_info->pointer_to, (char *)(*(ptrsize *)(data + i * sizeof(ptrsize))), 1, tab_count))
				return false;

		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!deserialize_fmt_text(w, "", enum_info->item_type, data + i * enum_info->size, 1, tab_count))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		auto struct_info = (Type_Info_Struct *)info;
		//TODO  : need optimization
		for (s64 j = 0; j < num_of_elements; ++j) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET))
				return false;
			if (struct_info->base) {
				if (!parse_require_token(w, Token_Kind_PERIOD))
					return false;
				if (!deserialize_fmt_text(w, "base", struct_info->base, data + j * struct_info->size, 1, tab_count + 3))
					return false;
			}
			for (int i = 0; i < struct_info->member_count; ++i) {
				auto mem = struct_info->members + i;
				bool no_serialize = false;
				for (u64 k = 0; k < mem->attribute_count; ++k) {
					if (string_match(mem->attributes[k], "no-serialize")) {
						no_serialize = true;
						break;
					}
				}
				if (!no_serialize)
					if (!deserialize_fmt_text(w, mem->name, mem->info, data + j * struct_info->size + mem->offset, 1, tab_count + 3))
						return false;
			}
			if (!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) || !parse_require_token(w, Token_Kind_COMMA)) {
				return false;
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_UNION:
	{
		auto union_info = (Type_Info_Union *)info;
		for (s64 k = 0; k < num_of_elements; ++k) {
			bool write = false;
			for (int j = 0; j < union_info->member_count; ++j) {
				auto mem = union_info->members + j;
				for (u64 i = 0; i < mem->attribute_count; ++i) {
					if (string_match(mem->attributes[i], "write")) {
						write = true;
						break;
					}
				}
				if (write) {
					if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET))
						return false;
					if (!deserialize_fmt_text(w, mem->name, mem->info, data + k * union_info->size, 1, tab_count + 3))
						return false;
					if (!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) || !parse_require_token(w, Token_Kind_COMMA)) {
						return false;
					}
					break;
				}
			}
			if (!write) {
				if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET))
					return false;
				if (!deserialize_fmt_text(w, union_info->members->name, union_info->members->info, k * union_info->size + data, 1, tab_count + 3))
					return false;
				if (!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET) || !parse_require_token(w, Token_Kind_COMMA)) {
					return false;
				}
			}
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &num_of_elements) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;
		}

		const Type_Info *elem_type_info = ((Type_Info_Static_Array *)info)->type;
		s64 count = ((Type_Info_Static_Array *)info)->count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			if (!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET) ||
				!parse_require_token(w, "N") ||
				!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_COLON)) {
				return false;
			}

			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &count) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;

			if (!deserialize_fmt_text(w, "", elem_type_info, data + i * elem_type_info->size * count, count, tab_count + 3))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_STRING:
	{
		{
			for (s64 i = 0; i < num_of_elements; ++i) {
				String *string_data = (String *)(data + i * sizeof(String));
				String temp;
				if (!parse_require_string(w, &temp) || !parse_require_token(w, Token_Kind_COMMA))
					return false;
				string_data->count = temp.count;
				string_data->data = new u8[string_data->count];
				memcpy(string_data->data, temp.data, string_data->count);
			}
			if (num_of_elements > 1) {
				if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
					!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
					return false;
				}
			}
		}
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &num_of_elements) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;
		}

		s64 count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array<char> *array = (Array<char> *)(data + i * sizeof(Array<char>));
			*array = Array<char>();
			array->data = nullptr;

			if (!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_PERIOD) ||
				!parse_require_token(w, Token_Kind_PERIOD) ||
				!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_COLON)) {
				return false;
			}

			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &count) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;

			array->capacity = count;
			array->count = count;
			array->data = (char *)memory_reallocate(array->data, array->capacity * ((Type_Info_Dynamic_Array *)info)->type->size, array->allocator);
			if (!deserialize_fmt_text(w, "", ((Type_Info_Dynamic_Array *)info)->type, array->data, count, tab_count + 3))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	}
	break;
	case Type_Id_ARRAY_VIEW:
	{
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &num_of_elements) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;
		}

		s64 count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array_View<char> *view = (Array_View<char> *)(data + i * sizeof(Array_View<char>));

			if (!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_COLON))
				return false;

			if (!parse_require_token(w, Token_Kind_OPEN_CURLY_BRACKET) ||
				!parse_require_integer<s64>(w, &count) ||
				!parse_require_token(w, Token_Kind_COMMA) ||
				!parse_require_token(w, Token_Kind_OPEN_SQUARE_BRACKET))
				return false;

			view->count = count;
			view->data = (char *)memory_allocate(view->count * ((Type_Info_Array_View *)info)->type->size);
			if (!deserialize_fmt_text(w, "", ((Type_Info_Array_View *)info)->type, view->data, count, tab_count + 3))
				return false;
		}
		if (num_of_elements > 1) {
			if (!parse_require_token(w, Token_Kind_CLOSE_SQUARE_BRACKET) ||
				!parse_require_token(w, Token_Kind_CLOSE_CURLY_BRACKET)) {
				return false;
			}
		}
	} break;
	}

	return true;
}

inline bool deserialize_fmt_text(Array_View<Token> &tokens, String name, const Type_Info *info, char *data, s64 num_of_elements = 1, int tab_count = 0) {
	if (tokens.count == 0)
		return false;

	Parse_State w;
	w.start = tokens.data;
	w.current = tokens.data;
	w.end = tokens.data + tokens.count;

	return deserialize_fmt_text(&w, name, info, data, num_of_elements, tab_count);
}
