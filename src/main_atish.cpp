#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"

#include "atish.h"
#include ".generated/atish.typeinfo"

//TODO : testing (proper testing is required)
void serialize_to_file(FILE *fp, String name, const Type_Info *info, char *data, s64 num_of_elements = 1, int tab_count = 0, bool is_array = false) {
	if (name.count) {
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : ", string_cstr(name));
	}
	switch (info->id) {
	case Type_Id_S8:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%d, ", (int)*(s8 *)(data + i * sizeof(s8)));
		}
		if (!is_array)
			fprintf(fp, "\n");
	}
	break;
	case Type_Id_S16:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%d, ", (int)*(s16 *)(data + i * sizeof(s16)));
		}
		if (!is_array)
			fprintf(fp, "\n");
	}
	break;
	case Type_Id_S32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%d, ", *(s32 *)(data + i * sizeof(s32)));
		}
		if (!is_array)
			fprintf(fp, "\n");
	}
	break;
	case Type_Id_S64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%zd, ", *(s64 *)(data + sizeof(s64)));
		}
		if (!is_array)
			fprintf(fp, "\n");
	}
	break;
	case Type_Id_U8:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%u, ", (u32) * (u8 *)(data + i * sizeof(u8)));
		}
		if (!is_array)
			fprintf(fp, "\n");
		break;
	case Type_Id_U16:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%u, ", (u32) * (u16 *)(data + i * sizeof(u16)));
		}
		if (!is_array)
			fprintf(fp, "\n");
		break;
	case Type_Id_U32:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%u, ", (u32) * (u32 *)(data + i * sizeof(u32)));
		}
		if (!is_array)
			fprintf(fp, "\n");
		break;
	case Type_Id_U64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%zu, ", *(u64 *)(data + i * sizeof(u64)));
		}
		if (!is_array)
			fprintf(fp, "\n");
		break;
	case Type_Id_R32:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%f, ", *(r32 *)(data + i * sizeof(r32)));
		}
		if (!is_array)
			fprintf(fp, "\n");
		break;
	case Type_Id_R64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%lf, ", *(r64 *)(data + i * sizeof(r64)));
		}
		if (!is_array)
			fprintf(fp, "\n");
		break;
	case Type_Id_CHAR:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "%c, ", *(char *)(data + i * sizeof(char)));
		}
		if (!is_array)
			fprintf(fp, "\n");
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
			serialize_to_file(fp, "", ptr_info->pointer_to, (char *)(*(ptrsize *)(data + i * sizeof(ptrsize))), 1, tab_count);
		}
	}
	break;
	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			serialize_to_file(fp, "", enum_info->item_type, data + i * enum_info->size, 1, tab_count);
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		fprintf(fp, "\n");
		auto struct_info = (Type_Info_Struct *)info;
		//TODO  : need optimization
		for (s64 j = 0; j < num_of_elements; ++j) {
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, "{\n");
			if (struct_info->base) {
				serialize_to_file(fp, ".base", struct_info->base, data + j * struct_info->size, 1, tab_count + 3);
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
					serialize_to_file(fp, mem->name, mem->info, data + j * struct_info->size + mem->offset, 1, tab_count + 3);
			}
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, "},\n");
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
					serialize_to_file(fp, mem->name, mem->info, data + k * union_info->size, 1, tab_count + 3);
					break;
				}
			}
			if (!write)
				serialize_to_file(fp, union_info->members->name, union_info->members->info, k * union_info->size + data, 1, tab_count + 3);
		}
	}
	break;
	case Type_Id_STRING:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			String *string_data = (String *)(data + i * sizeof(String));
			fprintf(fp, "\"%s\", ", tto_cstring(*string_data));
		};
		if (!is_array)
			fprintf(fp, "\n");
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		if (num_of_elements > 1)
			fprintf(fp, "{ %zd, [ ", num_of_elements);

		s64 count = ((Type_Info_Static_Array *)info)->count;
		const Type_Info *elem_type_info = ((Type_Info_Static_Array *)info)->type;

		for (s64 i = 0; i < num_of_elements; ++i) {
			fprintf(fp, "[N] : ");
			fprintf(fp, "{ %zd, [ ", count);
			serialize_to_file(fp, "", elem_type_info, data + i * elem_type_info->size * count, count, tab_count + 3, true);
			fprintf(fp, " ] }\n");
		}
		if (num_of_elements > 1) {
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		if (num_of_elements > 1)
			fprintf(fp, "{ %zd, [ ", num_of_elements);

		for (s64 i = 0; i < num_of_elements; ++i) {
			Array<char> *array = (Array<char> *)(data + i * sizeof(Array<char>));
			s64 count = array->count;

			fprintf(fp, "[..] : ");
			fprintf(fp, "{ %zd, [ ", count);
			serialize_to_file(fp, "", ((Type_Info_Dynamic_Array *)info)->type, array->data, count, tab_count + 3, true);
			fprintf(fp, " ] }\n");
		}
		if (num_of_elements > 1) {
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_ARRAY_VIEW:
	{
		if (num_of_elements > 1)
			fprintf(fp, "{ %zd, [ ", num_of_elements);

		const Type_Info *elem_type_info = ((Type_Info_Array_View *)info)->type;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array_View<char> *view = (Array_View<char> *)(data + i * sizeof(Array_View<char>));
			s64 count = view->count;

			fprintf(fp, "[] : ");
			fprintf(fp, "{ %zd, [ ", count);
			serialize_to_file(fp, "", elem_type_info, view->data, count, tab_count + 3, true);
			fprintf(fp, " ] }\n");
		}
		if (num_of_elements > 1) {
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, " ] }");
		}
		fprintf(fp, "\n");
	} break;
	}
}


void deserialize_from_file(FILE *fp, String name, const Type_Info *info, char *data, s64 num_of_elements = 1, int tab_count = 0) {
	if (name.count) {
		fscanf(fp, null_tprintf("%*s", tab_count, ""));
		fscanf(fp, null_tprintf("%s : ", string_cstr(name)));
	}
	switch (info->id) {
	case Type_Id_S8:
	{
		{
			int temp;
			for (s64 i = 0; i < num_of_elements; ++i) {
				fscanf(fp, "%d, ", &temp);
				*(s8 *)(data + i * sizeof(s8)) = (s8)temp;
			}
			if (num_of_elements > 1)
				fscanf(fp, " ] }");
			fscanf(fp, "\n");
		}
	}
	break;
	case Type_Id_S16:
	{
		{
			int temp;
			for (s64 i = 0; i < num_of_elements; ++i) {
				fscanf(fp, "%d, ", &temp);
				*(s16 *)(data + i * sizeof(s16)) = (s16)temp;
			}
			if (num_of_elements > 1)
				fscanf(fp, " ] }");
			fscanf(fp, "\n");
		}
	}
	break;
	case Type_Id_S32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%d, ", (s32 *)(data + i * sizeof(s32)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_S64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%zd, ", (s64 *)(data + sizeof(s64)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_U8:
	{
		u32 temp;
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%u, ", &temp);
			*(u8 *)(data + i * sizeof(u8)) = (u8)temp;
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_U16:
	{
		u32 temp;
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%u, ", &temp);
			*(u16 *)(data + i * sizeof(u16)) = (u16)temp;
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_U32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%u, ", (u32 *)(data + i * sizeof(u32)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_U64:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%zu, ", (u64 *)(data + i * sizeof(u64)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_R32:
	{
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%f, ", (r32 *)(data + i * sizeof(r32)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
	}
	break;
	case Type_Id_R64:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%lf, ", (r64 *)(data + i * sizeof(r64)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
		break;
	case Type_Id_CHAR:
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "%c, ", (char *)(data + i * sizeof(char)));
		}
		if (num_of_elements > 1)
			fscanf(fp, " ] }");
		fscanf(fp, "\n");
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
			deserialize_from_file(fp, "", ptr_info->pointer_to, (char *)(*(ptrsize *)(data + i * sizeof(ptrsize))), 1, tab_count);
		}
		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum *)info;
		for (s64 i = 0; i < num_of_elements; ++i) {
			deserialize_from_file(fp, "", enum_info->item_type, data + i * enum_info->size, 1, tab_count);
		}
		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		fscanf(fp, "\n");
		auto struct_info = (Type_Info_Struct *)info;
		//TODO  : need optimization
		for (s64 j = 0; j < num_of_elements; ++j) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, "{\n");
			if (struct_info->base) {
				deserialize_from_file(fp, ".base", struct_info->base, data + j * struct_info->size, 1, tab_count + 3);
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
					deserialize_from_file(fp, mem->name, mem->info, data + j * struct_info->size + mem->offset, 1, tab_count + 3);
			}
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, "},\n");
		}
		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }\n");
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
					deserialize_from_file(fp, mem->name, mem->info, data + k * union_info->size, 1, tab_count + 3);
					break;
				}
			}
			if (!write)
				deserialize_from_file(fp, union_info->members->name, union_info->members->info, k * union_info->size + data, 1, tab_count + 3);
		}
		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		if (num_of_elements > 1)
			fscanf(fp, "{ %zd, [ ", &num_of_elements);

		const Type_Info *elem_type_info = ((Type_Info_Static_Array *)info)->type;
		s64 count = ((Type_Info_Static_Array *)info)->count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			fscanf(fp, "[N] : ");
			fscanf(fp, "{ %zd, [ ", &count);
			deserialize_from_file(fp, "", elem_type_info, data + i * elem_type_info->size * count, count, tab_count + 3);
		}

		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_STRING:
	{
		{
			char temp[1024] = {};
			for (s64 i = 0; i < num_of_elements; ++i) {
				String *string_data = (String *)(data + i * sizeof(String));
				fscanf(fp, "%s, ", temp);
				string_data->count = strlen(temp) - 3;
				string_data->data = new u8[string_data->count];
				memcpy(string_data->data, temp + 1, string_data->count);
			};
			if (num_of_elements > 1)
				fscanf(fp, " ] }");
			fscanf(fp, "\n");
		}
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		if (num_of_elements > 1)
			fscanf(fp, "{ %zd, [ ", &num_of_elements);

		s64 count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array<char> *array = (Array<char> *)(data + i * sizeof(Array<char>));
			*array = Array<char>();
			array->data = nullptr;

			fscanf(fp, "[..] : ");
			fscanf(fp, "{ %zd, [ ", &count);

			array->capacity = count;
			array->count = count;
			array->data = (char *)memory_reallocate(array->data, array->capacity * ((Type_Info_Dynamic_Array *)info)->type->size, array->allocator);
			deserialize_from_file(fp, "", ((Type_Info_Dynamic_Array *)info)->type, array->data, count, tab_count + 3);
		}
		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }\n");
		}
	}
	break;
	case Type_Id_ARRAY_VIEW:
	{
		if (num_of_elements > 1)
			fscanf(fp, "{ %zd, [ ", &num_of_elements);

		s64 count;
		for (s64 i = 0; i < num_of_elements; ++i) {
			Array_View<char> *view = (Array_View<char> *)(data + i * sizeof(Array_View<char>));

			fscanf(fp, "[] : ");
			fscanf(fp, "{ %zd, [ ", &count);

			view->count = count;
			view->data = (char *)memory_allocate(view->count * ((Type_Info_Array_View *)info)->type->size);
			deserialize_from_file(fp, "", ((Type_Info_Array_View *)info)->type, view->data, count, tab_count + 3);
		}

		if (num_of_elements > 1) {
			fscanf(fp, null_tprintf("%*s", tab_count, ""));
			fscanf(fp, " ] }");
		}
		fscanf(fp, "\n");
	} break;
	}
}

int karma_user_atish() {
	FILE *fp;
	fp = fopen("temp/savegame.txt", "w+");

	if (fp == NULL) {
		printf("file not opened");
		exit(1);
	}

	Array<Vector2> a1, a2;
	array_add(&a1, Vector2{ 1, 2 });
	array_add(&a1, Vector2{ 3, 3 });
	array_add(&a1, Vector2{ 4, 3 });

	array_add(&a2, Vector2{ 9, 9 });

	Hard_Block sample;
	sample.placements = a1;

	serialize_to_file(fp, "sample", reflect_info(sample), (char *)&sample);

	rewind(fp);

	Hard_Block *retrieve = (Hard_Block *)memory_allocate(sizeof(sample));
	deserialize_from_file(fp, "sample", reflect_info(sample), (char *)retrieve);

	fclose(fp);
	return 0;
}
