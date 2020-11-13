#include "editor.h"

#include "modules/imgui/imgui.h"
#include "modules/core/reflection.h"

#include "scene.h"
#include ".generated/entity.typeinfo"

static const char *MAP_DATA_FORMAT[ImGuiDataType_COUNT] = {
	"%hhd",
	"%hhu",
	"%hd",
	"%hu",
	"%d",
	"%u",
	"%zd",
	"%16zx",
	"%.4f",
	"%.4lf",
};

static const ptrsize TYPE_UID_BOOL = (ptrsize)reflect_info<bool>();
static const ptrsize TYPE_UID_S8 = (ptrsize)reflect_info<s8>();
static const ptrsize TYPE_UID_S16 = (ptrsize)reflect_info<s16>();
static const ptrsize TYPE_UID_S32 = (ptrsize)reflect_info<s32>();
static const ptrsize TYPE_UID_S64 = (ptrsize)reflect_info<s64>();
static const ptrsize TYPE_UID_U8 = (ptrsize)reflect_info<u8>();
static const ptrsize TYPE_UID_U16 = (ptrsize)reflect_info<u16>();
static const ptrsize TYPE_UID_U32 = (ptrsize)reflect_info<u32>();
static const ptrsize TYPE_UID_U64 = (ptrsize)reflect_info<u64>();
static const ptrsize TYPE_UID_R32 = (ptrsize)reflect_info<r32>();
static const ptrsize TYPE_UID_R64 = (ptrsize)reflect_info<r64>();
static const ptrsize TYPE_UID_CHAR = (ptrsize)reflect_info<char>();
static const ptrsize TYPE_UID_VPTR = (ptrsize)reflect_info<void>();

static const ptrsize TYPE_UID_VEC2 = (ptrsize)reflect_info<Vec2>();
static const ptrsize TYPE_UID_VEC3 = (ptrsize)reflect_info<Vec3>();
static const ptrsize TYPE_UID_VEC4 = (ptrsize)reflect_info<Vec4>();

void scan_element_attributes(const String *attrs, ptrsize count, Element_Attribute *out) {
	for (u32 index = 0; index < (u32)count; ++index) {
		String name = attrs[index];

		if (string_match(name, "no_display")) {
			out->flags |= Attr_NO_DISPLAY;
			break;
		} else if (string_match(name, "read_only")) {
			out->flags |= Attr_READ_ONLY;
		} else if (string_match(name, "color")) {
			out->flags |= Attr_COLOR;
		} else if (string_match(name, "use")) {
			out->flags |= Attr_COLOR;
		} else if (string_starts_with(name, "step")) {
			sscanf(string_cstr(name), "step:%d", &out->step);
		} else if (string_starts_with(name, "speed")) {
			sscanf(string_cstr(name), "speed:%f", &out->speed);
		} else if (string_starts_with(name, "min")) {
			sscanf(string_cstr(name), "min:%f", &out->min);
		} else if (string_starts_with(name, "max")) {
			sscanf(string_cstr(name), "max:%f", &out->max);
		}

	}
}

template <typename T>
static bool editor_widget_basic_slider(const char *label, ImGuiDataType type, T *data, int components, r32 speed, T min, T max, bool read_only) {
	int flags = read_only ? ImGuiSliderFlags_NoInput : 0;
	flags |= ImGuiSliderFlags_AlwaysClamp;
	return ImGui::DragScalarN(label, type, data, components, speed, &min, &max, MAP_DATA_FORMAT[type], flags);
}

static bool editor_widget_slider(ptrsize uid, const char *label, void *data, u32 flags, s32 step, r32 speed, r32 min, r32 max) {
	bool read_only = (flags & Attr_READ_ONLY);
	if (uid == TYPE_UID_BOOL) {
		return ImGui::Checkbox(label, (bool *)data);
	} else if (uid == TYPE_UID_U8) {
		return editor_widget_basic_slider<u8>(label, ImGuiDataType_U8, (u8 *)data, 1, (r32)step, (u8)maximum(min, 0), (u8)minimum(max, MAX_UINT8), read_only);
	} else if (uid == TYPE_UID_U16) {
		return editor_widget_basic_slider<u16>(label, ImGuiDataType_U16, (u16 *)data, 1, (r32)step, (u16)maximum(min, 0), (u16)minimum(max, MAX_UINT16), read_only);
	} else if (uid == TYPE_UID_U32) {
		return editor_widget_basic_slider<u32>(label, ImGuiDataType_U32, (u32 *)data, 1, (r32)step, (u32)maximum(min, 0), (u32)minimum(max, MAX_UINT32), read_only);
	} else if (uid == TYPE_UID_U64) {
		return editor_widget_basic_slider<u64>(label, ImGuiDataType_U64, (u64 *)data, 1, (r32)step, (u64)maximum(min, 0), (u64)minimum(max, MAX_UINT64), read_only);
	} else if (uid == TYPE_UID_S8) {
		return editor_widget_basic_slider<s8>(label, ImGuiDataType_S8, (s8 *)data, 1, (r32)step, (s8)maximum(min, MIN_INT8), (s8)minimum(max, MAX_INT8), read_only);
	} else if (uid == TYPE_UID_S16) {
		return editor_widget_basic_slider<s16>(label, ImGuiDataType_S16, (s16 *)data, 1, (r32)step, (s16)maximum(min, MIN_INT16), (s16)minimum(max, MAX_INT16), read_only);
	} else if (uid == TYPE_UID_S32) {
		return editor_widget_basic_slider<s32>(label, ImGuiDataType_S32, (s32 *)data, 1, (r32)step, (s32)maximum(min, MIN_INT32), (s32)minimum(max, MAX_INT32), read_only);
	} else if (uid == TYPE_UID_S64) {
		return editor_widget_basic_slider<s64>(label, ImGuiDataType_S64, (s64 *)data, 1, (r32)step, (s64)maximum(min, MIN_INT64), (s64)minimum(max, MAX_INT64), read_only);
	} else if (uid == TYPE_UID_R32) {
		return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 1, speed, (r32)min, (r32)max, read_only);
	} else if (uid == TYPE_UID_R64) {
		return editor_widget_basic_slider<r64>(label, ImGuiDataType_Double, (r64 *)data, 1, speed, (r64)min, (r64)max, read_only);
	} else if (uid == TYPE_UID_CHAR) {
		return editor_widget_basic_slider<char>(label, ImGuiDataType_S8, (char *)data, 1, (r32)step, (char)maximum(min, MIN_INT8), (char)minimum(max, MAX_INT8), read_only);
	} else if (uid == TYPE_UID_VPTR) {
		u64 ptr = (u64)((void *)data);
		return editor_widget_basic_slider<u64>(label, ImGuiDataType_U64, &ptr, 1, 0.0f, (u64)min, (u64)max, read_only);
	} else if (uid == TYPE_UID_VEC2) {
		return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 2, speed, min, max, read_only);
	} else if (uid == TYPE_UID_VEC3) {
		if (flags & Attr_COLOR) {
			return ImGui::ColorEdit3(label, (r32 *)data, ImGuiColorEditFlags_DisplayHSV);
		} else {
			return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 3, speed, min, max, read_only);
		}
	} else if (uid == TYPE_UID_VEC4) {
		if (flags & Attr_COLOR) {
			return ImGui::ColorEdit4(label, (r32 *)data, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_AlphaBar);
		} else {
			return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 4, speed, min, max, read_only);
		}
	}

	return false;
}

int get_int_enum_value(const Type_Info_Enum *info, void *data) {
	switch (info->item_type->id) {
	case Type_Id_U8:	return (int)*(u8 *)data;
	case Type_Id_U16:	return (int)*(u16 *)data;
	case Type_Id_U32:	return (int)*(u32 *)data;
	case Type_Id_CHAR:	return (int)*(char *)data;
	case Type_Id_S8:	return (int)*(s8 *)data;
	case Type_Id_S16:	return (int)*(s16 *)data;
	case Type_Id_S32:	return (int)*(s32 *)data;

		invalid_default_case();
	}
	return 0;
}

void set_int_enum_value(const Type_Info_Enum *info, void *data, int index) {
	switch (info->item_type->id) {
	case Type_Id_U8:	*(u8 *)data = (u8)index; break;
	case Type_Id_U16:	*(u16 *)data = (u16)index; break;
	case Type_Id_U32:	*(u32 *)data = (u32)index; break;
	case Type_Id_CHAR:	*(char *)data = (char)index; break;
	case Type_Id_S8:	*(s8 *)data = (s8)index; break;
	case Type_Id_S16:	*(s16 *)data = (s16)index; break;
	case Type_Id_S32:	*(s32 *)data = (s32)index; break;

		invalid_default_case();
	}
}

bool editor_expanding_widget(const Type_Info *info) {
	return info->id == Type_Id_STRUCT;
}

bool editor_widget_draw(const Type_Info *base_info, char *data, const Element_Attribute &attr, const char *name) {
	switch (base_info->id) {
	case Type_Id_POINTER: {
		auto info = (Type_Info_Pointer *)base_info;

		if (editor_expanding_widget(info->pointer_to)) {
			if (ImGui::CollapsingHeader(string_cstr(info->pointer_to->name))) {
				return editor_widget_draw(info->pointer_to, (char *)(*(ptrsize *)data), attr, name);
			}
		} else {
			return editor_widget_draw(info->pointer_to, (char *)(*(ptrsize *)data), attr, name);
		}
	} break;

	case Type_Id_ENUM: {
		auto info = (Type_Info_Enum *)base_info;
		assert(info->is_sequential);

		int index = get_int_enum_value(info, data);

		if (attr.flags & Attr_READ_ONLY) {
			ImGui::Text("%s : %s", name, info->item_strings[index]);
			return false;
		} else {
			if (ImGui::Combo(name, &index, info->item_strings.data, (int)info->item_strings.count)) {
				set_int_enum_value(info, data, index);
				return true;
			}
			return false;
		}
	} break;

	case Type_Id_STRUCT: {
		auto info = (Type_Info_Struct *)base_info;
		auto mem_counts = info->member_count;

		bool result = false;

		if (info->base) {
			result |= editor_widget_draw(info->base, data, attr, string_cstr(info->base->name));
		}

		for (ptrsize index = 0; index < mem_counts; ++index) {
			auto mem = info->members + index;

			Element_Attribute mem_attr = {};
			scan_element_attributes(mem->attributes, mem->attribute_count, &mem_attr);
			mem_attr.flags |= attr.flags;
			
			if (mem_attr.flags & Attr_NO_DISPLAY) continue;

			if (editor_expanding_widget(mem->info)) {
				if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
					result |= editor_widget_draw(mem->info, data + mem->offset, mem_attr, string_cstr(mem->name));
				}
			} else {
				result |= editor_widget_draw(mem->info, data + mem->offset, mem_attr, string_cstr(mem->name));
			}
		}

		return result;
	} break;

	case Type_Id_UNION: {
		ptrsize uid = (ptrsize)base_info;
		if (uid == TYPE_UID_VEC2 || uid == TYPE_UID_VEC3 || uid == TYPE_UID_VEC4) {
			return editor_widget_slider(uid, name, data, attr.flags, attr.step, attr.speed, attr.min, attr.max);
		} else {

			auto info = (Type_Info_Union *)base_info;
			auto mem_counts = info->member_count;

			Element_Attribute mem_attr;

			if (mem_counts) {
				const Union_Member *mem = nullptr;
				for (ptrsize index = 0; index < mem_counts; ++index) {
					auto temp_mem = info->members + index;
					mem_attr = {};
					scan_element_attributes(temp_mem->attributes, temp_mem->attribute_count, &mem_attr);
					if (mem_attr.flags & Attr_UNION_DISPLAY) {
						mem = temp_mem;
						break;
					}
				}

				// If we don't find display attribute, we display the first member
				if (mem == nullptr) {
					mem = info->members;
					scan_element_attributes(mem->attributes, mem->attribute_count, &mem_attr);
				}

				mem_attr.flags |= attr.flags;
				if (editor_expanding_widget(mem->info)) {
					if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
						return editor_widget_draw(mem->info, data, mem_attr, string_cstr(mem->name));
					}
				} else {
					return editor_widget_draw(mem->info, data, mem_attr, string_cstr(mem->name));
				}
			}

		}
	} break;

	default: {
		ptrsize uid = (ptrsize)base_info;
		return editor_widget_slider(uid, name, data, attr.flags, attr.step, attr.speed, attr.min, attr.max);
	} break;
	}

	return false;
}
