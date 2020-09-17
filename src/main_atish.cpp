#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"

#include "atish.h"
#include ".generated/atish.typeinfo"

void serialize_to_file_array(FILE* fp, const Type_Info* info, char* data, int tab_count);

//TODO : testing 
void serialize_to_file(FILE* fp, String name, const Type_Info* info, char* data, int tab_count = 0)
{
	switch (info->id)
	{
	case Type_Id_S8:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %d,\n", string_cstr(name), (int)*(s8*)(data));
	}
	break;
	case Type_Id_S16:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %d,\n", string_cstr(name), (int)*(s16*)(data));
	}
	break;
	case Type_Id_S32:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %d,\n", string_cstr(name), *(s32*)(data));
	}
	break;
	case Type_Id_S64:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %zd,\n", string_cstr(name), *(s64*)(data));
	}
	break;
	case Type_Id_U8:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %u,\n", string_cstr(name), (u32) * (u8*)(data));
	}
	break;
	case Type_Id_U16:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %u,\n", string_cstr(name), (u32) * (u16*)(data));
	}
	break;
	case Type_Id_U32:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %u,\n", string_cstr(name), (u32) * (u32*)(data));
	}
	break;
	case Type_Id_U64:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %zu,\n", string_cstr(name), *(u64*)(data));
	}
	break;
	case Type_Id_R32:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %f,\n", string_cstr(name), *(r32*)(data));
	}
	break;
	case Type_Id_R64:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %f,\n", string_cstr(name), *(r64*)(data));
	}
	break;
	case Type_Id_CHAR:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "%s : %c,\n", string_cstr(name), *(char*)(data));
	}
	break;
	case Type_Id_VOID:// invalid_path();
	{
		invalid_code_path();
	}
	break;
	case Type_Id_POINTER:// deference 
	{
		fprintf(fp, "%*s", tab_count, "");
		auto ptr_info = (Type_Info_Pointer*)info;
		fprintf(fp, "?");
		serialize_to_file(fp, name, ptr_info->pointer_to, (char*)(*(ptrsize*)data), tab_count - 3);
	}
	break;

	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum*)info;
		serialize_to_file(fp, name, enum_info->item_type,data, tab_count);
	}
	break;
	case Type_Id_STRUCT:
	{
		fprintf(fp, "%*s", tab_count, "");
		auto struct_info = (Type_Info_Struct*)info;
		fprintf(fp, "%s : { \n", tto_cstring(name));
		if (struct_info->base)
		{
			serialize_to_file(fp, ".base", struct_info->base, data, tab_count + 3);
		}
		for (int j = 0; j < struct_info->member_count; ++j)
		{
			auto mem = struct_info->members + j;
			bool no_serialize = false;
			for (u64 i = 0; i < mem->attribute_count; ++i)
			{
				if (string_match(mem->attributes[i], "no-serialize"))
				{
					no_serialize = true;
					break;
				}
			}
			if (!no_serialize)
				serialize_to_file(fp, mem->name, mem->info, data + mem->offset, tab_count + 3);
		}
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "}\n");
	}
	break;
	case Type_Id_UNION:
	{
		fprintf(fp, "%*s", tab_count, "");
		auto union_info = (Type_Info_Union*)info;
		fprintf(fp, "%s : { \n", tto_cstring(name));
		bool write = false;
		for (int j = 0; j < union_info->member_count; ++j)
		{
			auto mem = union_info->members + j;
			for (u64 i = 0; i < mem->attribute_count; ++i)
			{
				if (string_match(mem->attributes[i], "write"))
				{
					write = true;
					break;
				}
			}
			if (write)
			{
				serialize_to_file(fp, mem->name, mem->info, data, tab_count + 3);
				break;
			}
		}
		if (!write)
			serialize_to_file(fp, union_info->members->name, union_info->members->info, data, tab_count + 3);
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "}\n");
	}
	break;
	case Type_Id_STATIC_ARRAY:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "[N] %s : ", string_cstr(name));
		serialize_to_file_array(fp, info, data, tab_count + 3);
	}
	break;
	case Type_Id_STRING:
	{
		fprintf(fp, "%*s", tab_count, "");
		String* string_data = (String*)data;
		fprintf(fp, "%s : \"%s\" ,\n", string_cstr(name), tto_cstring(*string_data));
	}
	break;
	case Type_Id_DYNAMIC_ARRAY:
	{
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "[..] %s : ", string_cstr(name));
		serialize_to_file_array(fp, info, data, tab_count + 3);
	}
	break;
	case Type_Id_ARRAY_VIEW: {
		fprintf(fp, "%*s", tab_count, "");
		fprintf(fp, "[] %s : ", string_cstr(name));
		serialize_to_file_array(fp, info, data, tab_count + 3);
	} break;
	}
}

void serialize_to_file_array(FILE* fp, const Type_Info* info, char* data, int tab_count) {
	s64 count;
	const Type_Info* elem_type_info;
	if (info->id == Type_Id_STATIC_ARRAY) {
		count = ((Type_Info_Static_Array*)info)->count;
		elem_type_info = ((Type_Info_Static_Array*)info)->type;
	}
	else if (info->id == Type_Id_DYNAMIC_ARRAY) {
		Array<char>* array = (Array<char>*)data;
		count = array->count;
		elem_type_info = ((Type_Info_Dynamic_Array*)info)->type;
		data = array->data;
	}
	else if (info->id == Type_Id_ARRAY_VIEW) {
		Array_View<char>* view = (Array_View<char>*)data;
		count = view->count;
		elem_type_info = ((Type_Info_Array_View*)info)->type;
		data = view->data;
	}

	fprintf(fp, "{ %zd, [\n", count);
	switch (elem_type_info->id) {
	case Type_Id_S8:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%d, ", (int)*(s8*)(data + i * sizeof(s8)));
		};
		break;
	case Type_Id_S16:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%d, ", (int)*(s16*)(data + i * sizeof(s16)));
		}
		break;
	case Type_Id_S32:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%d, ", *(s32*)(data + i * sizeof(s32)));
		}
		break;
	case Type_Id_S64:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%zd, ", *(s64*)(data + sizeof(s64)));
		}
		break;
	case Type_Id_U8:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%u, ", (u32) * (u8*)(data + i * sizeof(u8)));
		}
		break;
	case Type_Id_U16:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%u, ", (u32) * (u16*)(data + i * sizeof(u16)));
		}
		break;
	case Type_Id_U32:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%u, ", (u32) * (u32*)(data + i * sizeof(u32)));
		}
		break;
	case Type_Id_U64:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%zu, ", *(u64*)(data + i * sizeof(u64)));
		}
		break;
	case Type_Id_R32:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%f, ", *(r32*)(data + i * sizeof(r32)));
		}
		break;
	case Type_Id_R64:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%f, ", *(r64*)(data + i * sizeof(r64)));
		}
		break;
	case Type_Id_CHAR:
		for (s64 i = 0; i < count; ++i) {
			fprintf(fp, "%c, ", *(char*)(data + i * sizeof(char)));
		}
		break;
	case Type_Id_VOID:// invalid_path();
	{
		invalid_code_path();
	}
	break;
	case Type_Id_POINTER:// deference 
	{
		auto ptr_info = (Type_Info_Pointer*)elem_type_info;
		for (s64 i = 0; i < count; ++i) {
			serialize_to_file(fp, "?", ptr_info->pointer_to, (char*)(*(ptrsize*)(data+i*sizeof(ptrsize))), tab_count - 3);
		}
	}
	break;

	case Type_Id_FUNCTION:// skip
		break;
	case Type_Id_ENUM:// s64 
	{
		auto enum_info = (Type_Info_Enum*)elem_type_info;
		for (s64 i = 0; i < count; ++i) {
			serialize_to_file(fp, ".",enum_info->item_type, data + i * enum_info->size, tab_count - 3);
		}
	}
	break;
	case Type_Id_STRUCT:
	{
		auto struct_info = (Type_Info_Struct*)elem_type_info;
		//TODO  : need optimization
		for (s64 j = 0; j < count; ++j) {
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, "{\n");
			if (struct_info->base)
			{
				serialize_to_file(fp, ".base", struct_info->base, data+j*struct_info->size, tab_count + 3);
			}
			for (int i = 0; i < struct_info->member_count; ++i)
			{
				auto mem = struct_info->members + i;
				bool no_serialize = false;
				for (u64 k = 0; k < mem->attribute_count; ++k)
				{
					if (string_match(mem->attributes[k], "no-serialize"))
					{
						no_serialize = true;
						break;
					}
				}
				if (!no_serialize)
					serialize_to_file(fp, mem->name, mem->info, data +j*struct_info->size+ mem->offset, tab_count + 3);
			}
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, "},\n");
		}
	}
	break;
	case Type_Id_UNION:
	{
		auto union_info = (Type_Info_Union*)elem_type_info;
		for (s64 k = 0; k < count; ++k)
		{
			bool write = false;
			for (int j = 0; j < union_info->member_count; ++j)
			{
				auto mem = union_info->members + j;
				for (u64 i = 0; i < mem->attribute_count; ++i)
				{
					if (string_match(mem->attributes[i], "write"))
					{
						write = true;
						break;
					}
				}
				if (write)
				{
					serialize_to_file(fp, mem->name, mem->info, data+k*union_info->size, tab_count + 3);
					break;
				}
			}
			if (!write)
				serialize_to_file(fp, union_info->members->name, union_info->members->info, k*union_info->size+data, tab_count + 3);
		}
	}
	break;
	case Type_Id_STRING: {
		for (s64 i = 0; i < count; ++i) {
			String* string_data = (String*)(data + i * sizeof(String));
			fprintf(fp, "%*s", tab_count, "");
			fprintf(fp, "\"%s\",\n",tto_cstring(*string_data));
		};
		break;
	} break;
	case Type_Id_STATIC_ARRAY: {
		for (s64 i = 0; i < count; ++i) {
			serialize_to_file_array(fp, elem_type_info, data + i * elem_type_info->size, tab_count + 3);
		}
	} break;
	case Type_Id_DYNAMIC_ARRAY: {
		for (s64 i = 0; i < count; ++i) {
			serialize_to_file_array(fp, elem_type_info, data + i * elem_type_info->size, tab_count + 3);
		}
	} break;
	case Type_Id_ARRAY_VIEW: {
		for (s64 i = 0; i < count; ++i) {
			serialize_to_file_array(fp, elem_type_info, data + i * elem_type_info->size, tab_count + 3);
		}
	} break;
	}
	fprintf(fp, "%*s", tab_count - 3, "");
	fprintf(fp, " ] },\n");
}


int karma_user_atish() {
	//saving from here

	FILE* fp;
	fp = fopen("temp/savegame.txt", "w+");
	if (fp == NULL)
	{
		//cout << "file not opened";
		exit(1);
	}

	Hard_Block sample[4];
	serialize_to_file(fp, "sample", reflect_info(sample), (char*)sample, 0);
	int a = 0;

#if 0
	if (info->id == Type_Id_STRUCT) {
		auto       struct_info = (Type_Info_Struct*)info;
		fprintf(fp, "struct %s { \n", string_cstr(struct_info->name));
		for (int i = 0; i < struct_info->member_count; ++i)
		{
			auto mem = struct_info->members + i;
			switch (mem->info->id)
			{
			case Type_Id_S8:
				fprintf(fp, "%s : %d\n", string_cstr(struct_info->members[i].info->name), (int)*(s8*)(data + mem->offset));
				break;
			case	Type_Id_S16:
			case		Type_Id_S32:
				fprintf(fp, "%s : %d\n", string_cstr(struct_info->members[i].info->name), *(s32*)(data + mem->offset));
				break;
				Type_Id_S64,
					Type_Id_U8,
					Type_Id_U16,
					Type_Id_U32,
					Type_Id_U64,
					Type_Id_R32,
					Type_Id_R64,
					Type_Id_CHAR,
					Type_Id_VOID,// invalid_path();
					Type_Id_POINTER,// deference 
					Type_Id_FUNCTION,// skip
					Type_Id_ENUM,// s64 
					Type_Id_STRUCT,
					Type_Id_UNION,// invalid_path();
					Type_Id_STATIC_ARRAY,// 
			}
			fprintf(fp, "%s : %d\n", struct_info->members[i].info->name.data, (int)(struct_info->members[i].info->name.count));
		}
		fprintf(fp, "}\n");
	}

	//auto info = reflect_info<Atish>();

	//Atish retrieve;
	//fseek(fp, 0, SEEK_SET);
	//int count;
	//fscanf(fp,null_tprintf( "struct %s { \n",info->name.data));
	//fscanf(fp, "struct Atish { \n");
	//const char* fmt = null_tprintf("%s : %s\n", string_cstr(info->members[i].info->name), "%d");
	//fscanf(fp, fmt,(s32*)(data + mem->offset)));
	//fscanf(fp, "a : %d\n", (s32*)(data + mem->offset)));

	//fscanf(fp, "members_count : %d \n",count);
	//String name_r;
	//int data_r;
	//for (int i = 0; i < count; ++i)
	//{
	//	fscanf(fp, "%s : %d\n", name_r, );
	//}
	//fscanf(fp, "}\n");
#endif
	fclose(fp);
	return 0;
}
