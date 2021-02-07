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
static const ptrsize TYPE_UID_TRANSFORM = (ptrsize)reflect_info<Transform>();

void scan_element_attributes(const String *attrs, ptrsize count, Element_Attribute *out) {
	for (u32 index = 0; index < (u32)count; ++index) {
		String name = attrs[index];

		if (string_match(name, "no_display")) {
			out->flags |= Attr_NO_DISPLAY;
			break;
		} else if (string_match(name, "read_only")) {
			out->flags |= Attr_READ_ONLY;
		} else if (string_match(name, "slider")) {
			out->flags |= Attr_SLIDER;
		} else if (string_match(name, "color")) {
			out->flags |= Attr_COLOR;
		} else if (string_match(name, "hsv")) {
			out->flags |= Attr_COLOR_HSV;
		} else if (string_match(name, "use")) {
			out->flags |= Attr_UNION_DISPLAY;
		} else if (string_match(name, "text")) {
			out->flags |= Attr_TEXT;
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
static bool editor_widget_basic_slider(const char *label, ImGuiDataType type, T *data, int components, r32 speed, T min, T max, u32 flags) {
	if (!(flags & Attr_READ_ONLY)) {
		if (flags & Attr_SLIDER) {
			return ImGui::SliderScalarN(label, type, data, components, &min, &max, MAP_DATA_FORMAT[type], ImGuiSliderFlags_AlwaysClamp);
		} else {
			return ImGui::DragScalarN(label, type, data, components, speed, &min, &max, MAP_DATA_FORMAT[type], ImGuiSliderFlags_AlwaysClamp);
		}
	}

	if (components == 1) {
		ImGui::LabelText(label, MAP_DATA_FORMAT[type], *data);
	} else if (components == 2) {
		const char *fmt = MAP_DATA_FORMAT[type];
		ImGui::LabelText(label, null_tprintf("x: %s y: %s", fmt, fmt), data[0], data[1]);
	} else if (components == 3) {
		if (flags & Attr_COLOR) {
			int color_flags = ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_HDR;

			ImVec4 color;
			if (flags & Attr_COLOR_HSV) {
				auto c = hsv_to_rgb(vec3((r32)data[0], (r32)data[1], (r32)data[2]));
				color.x = c.x;
				color.y = c.y;
				color.z = c.z;
				color.w = 1;
			} else {
				color.x = (r32)data[0];
				color.y = (r32)data[1];
				color.z = (r32)data[2];
				color.w = 1;
			}

			ImGuiEx::LabelColor(label, color, color_flags);
		} else {
			const char *fmt = MAP_DATA_FORMAT[type];
			ImGui::LabelText(label, null_tprintf("x: %s y: %s z: %s", fmt, fmt, fmt), data[0], data[1], data[2]);
		}
	} else if (components == 4) {
		if (flags & Attr_COLOR) {
			int color_flags = ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview;

			ImVec4 color;
			if (flags & Attr_COLOR_HSV) {
				auto c = hsv_to_rgb(vec3((r32)data[0], (r32)data[1], (r32)data[2]));
				color.x = c.x;
				color.y = c.y;
				color.z = c.z;
				color.w = (r32)data[3];
			} else {
				color.x = (r32)data[0];
				color.y = (r32)data[1];
				color.z = (r32)data[2];
				color.w = (r32)data[3];
			}

			ImGuiEx::LabelColor(label, color, color_flags);
		} else {
			const char *fmt = MAP_DATA_FORMAT[type];
			ImGui::LabelText(label, null_tprintf("x: %s y: %s z: %s w: %s", fmt, fmt, fmt, fmt), data[0], data[1], data[2], data[3]);
		}
	}

	return false;
}

static bool editor_widget_slider(ptrsize uid, const char *label, void *data, u32 flags, s32 step, r32 speed, r32 min, r32 max) {
	if (uid == TYPE_UID_BOOL) {
		return ImGuiEx::LabelCheckbox(label, (bool *)data, !(flags & Attr_READ_ONLY));
	} else if (uid == TYPE_UID_U8) {
		return editor_widget_basic_slider<u8>(label, ImGuiDataType_U8, (u8 *)data, 1, (r32)step, (u8)maximum(min, 0), (u8)minimum(max, MAX_UINT8), flags);
	} else if (uid == TYPE_UID_U16) {
		return editor_widget_basic_slider<u16>(label, ImGuiDataType_U16, (u16 *)data, 1, (r32)step, (u16)maximum(min, 0), (u16)minimum(max, MAX_UINT16), flags);
	} else if (uid == TYPE_UID_U32) {
		return editor_widget_basic_slider<u32>(label, ImGuiDataType_U32, (u32 *)data, 1, (r32)step, (u32)maximum(min, 0), (u32)minimum(max, MAX_UINT32), flags);
	} else if (uid == TYPE_UID_U64) {
		return editor_widget_basic_slider<u64>(label, ImGuiDataType_U64, (u64 *)data, 1, (r32)step, (u64)maximum(min, 0), (u64)minimum(max, MAX_UINT64), flags);
	} else if (uid == TYPE_UID_S8) {
		return editor_widget_basic_slider<s8>(label, ImGuiDataType_S8, (s8 *)data, 1, (r32)step, (s8)maximum(min, MIN_INT8), (s8)minimum(max, MAX_INT8), flags);
	} else if (uid == TYPE_UID_S16) {
		return editor_widget_basic_slider<s16>(label, ImGuiDataType_S16, (s16 *)data, 1, (r32)step, (s16)maximum(min, MIN_INT16), (s16)minimum(max, MAX_INT16), flags);
	} else if (uid == TYPE_UID_S32) {
		return editor_widget_basic_slider<s32>(label, ImGuiDataType_S32, (s32 *)data, 1, (r32)step, (s32)maximum(min, MIN_INT32), (s32)minimum(max, MAX_INT32), flags);
	} else if (uid == TYPE_UID_S64) {
		return editor_widget_basic_slider<s64>(label, ImGuiDataType_S64, (s64 *)data, 1, (r32)step, (s64)maximum(min, MIN_INT64), (s64)minimum(max, MAX_INT64), flags);
	} else if (uid == TYPE_UID_R32) {
		return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 1, speed, (r32)min, (r32)max, flags);
	} else if (uid == TYPE_UID_R64) {
		return editor_widget_basic_slider<r64>(label, ImGuiDataType_Double, (r64 *)data, 1, speed, (r64)min, (r64)max, flags);
	} else if (uid == TYPE_UID_CHAR) {
		return editor_widget_basic_slider<char>(label, ImGuiDataType_S8, (char *)data, 1, (r32)step, (char)maximum(min, MIN_INT8), (char)minimum(max, MAX_INT8), flags);
	} else if (uid == TYPE_UID_VPTR) {
		u64 ptr = (u64)((void *)data);
		return editor_widget_basic_slider<u64>(label, ImGuiDataType_U64, &ptr, 1, 0.0f, (u64)min, (u64)max, flags);
	} else if (uid == TYPE_UID_VEC2) {
		return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 2, speed, min, max, flags);
	} else if (uid == TYPE_UID_VEC3) {
		if ((flags & Attr_COLOR) && !(flags & Attr_READ_ONLY)) {
			int col_flags = ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_HDR;
			if (flags & Attr_COLOR_HSV) col_flags |= ImGuiColorEditFlags_InputHSV;
			return ImGui::ColorEdit3(label, (r32 *)data, col_flags);
		} else {
			return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 3, speed, min, max, flags);
		}
	} else if (uid == TYPE_UID_VEC4) {
		if ((flags & Attr_COLOR) && !(flags & Attr_READ_ONLY)) {
			int col_flags = ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview;
			if (flags & Attr_COLOR_HSV) col_flags |= ImGuiColorEditFlags_InputHSV;
			return ImGui::ColorEdit4(label, (r32 *)data, col_flags);
		} else {
			return editor_widget_basic_slider<r32>(label, ImGuiDataType_Float, (r32 *)data, 4, speed, min, max, flags);
		}
	} else if (uid == TYPE_UID_TRANSFORM) {
		Transform *t = (Transform *)data;

		editor_widget_basic_slider<r32>("translation", ImGuiDataType_Float, t->p.m, 2, speed, min, max, flags | Attr_READ_ONLY);

		r32 angle;
		Vec2 scale;

		r32 _00 = t->xform.m2[0][0];
		r32 _01 = t->xform.m2[0][1];
		r32 _10 = t->xform.m2[1][0];
		r32 _11 = t->xform.m2[1][1];

		scale.x = sqrtf(_00 * _00 + _10 * _10);
		scale.y = sqrtf(_01 * _01 + _11 * _11);

		r32 c = _00 / scale.x;
		r32 s = _10 / scale.x;

		if (c == 0 && s == 0) {
			c = 1;
		}

		angle = atanf(s / c) + ((c < 0) ? MATH_PI : 0);

		bool changed = false;
		changed |= editor_widget_basic_slider<r32>("rotate", ImGuiDataType_Float, &angle, 1, speed, min, max, flags);
		changed |= editor_widget_basic_slider<r32>("scale", ImGuiDataType_Float, scale.m, 2, speed, 0.001f, max, flags);

		if (changed) {
			t->xform = mat2_rotation(angle) * mat2_scalar(scale);
		}

		return changed;
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
	ImGui::PushID((void *)data);
	defer{
		ImGui::PopID();
	};

	switch (base_info->id) {
	case Type_Id_POINTER: {
		auto info = (Type_Info_Pointer *)base_info;

		if (editor_expanding_widget(info->pointer_to)) {
			if (ImGui::CollapsingHeader(string_cstr(info->pointer_to->name))) {
				ImGui::Indent();
				bool result = editor_widget_draw(info->pointer_to, (char *)(*(ptrsize *)data), attr, name);
				ImGui::Unindent();
				return result;
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
			ImGui::LabelText(name, info->item_strings[index]);
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
		ptrsize uid = (ptrsize)base_info;

		if (uid == TYPE_UID_TRANSFORM) {
			return editor_widget_slider(uid, name, data, attr.flags, attr.step, attr.speed, attr.min, attr.max);
		}

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
					ImGui::Indent();
					result |= editor_widget_draw(mem->info, data + mem->offset, mem_attr, string_cstr(mem->name));
					ImGui::Unindent();
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

	case Type_Id_STATIC_ARRAY: {
		auto info = (Type_Info_Static_Array *)base_info;
		if (info->type->id == Type_Id_CHAR && (attr.flags & Attr_TEXT)) {
			ImGuiInputTextFlags input_flags = (attr.flags & Attr_READ_ONLY) ? ImGuiInputTextFlags_ReadOnly : 0;
			return ImGui::InputText(name, data, info->size, input_flags);
		}
	} break;

	default: {
		ptrsize uid = (ptrsize)base_info;
		return editor_widget_slider(uid, name, data, attr.flags, attr.step, attr.speed, attr.min, attr.max);
	} break;
	}

	return false;
}
