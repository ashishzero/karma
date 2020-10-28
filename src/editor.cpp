#include "modules/imgui/imgui.h"
#include "modules/core/reflection.h"
#include "modules/core/lin_maths_types.h"

#include "editor.h"
#include ".generated/entity.typeinfo"

constexpr u32 EDITOR_FLAG_NO_DISPLAY = bit(1);
constexpr u32 EDITOR_FLAG_READ_ONLY = bit(2);
constexpr u32 EDITOR_FLAG_COLOR = bit(3);
constexpr u32 EDITOR_FLAG_UNION_DISPLAY = bit(4);

void editor_get_flags_from_attribute(const String *attrs, s64 count, Editor_Attribute *out) {
	out->flags = 0;

	for (s64 index = 0; index < count; ++index) {
		if (string_match(attrs[index], "no_display")) {
			out->flags |= EDITOR_FLAG_NO_DISPLAY;
			break; // If we are not displaying, than we don't need other attributes
		}
		else if (string_match(attrs[index], "read_only")) {
			out->flags |= EDITOR_FLAG_READ_ONLY;
		}
		else if (string_match(attrs[index], "color")) {
			out->flags |= EDITOR_FLAG_COLOR;
		}
		else if (string_starts_with(attrs[index], "speed:")) {
			sscanf(string_cstr(attrs[index]), "speed:%f", &out->speed);
		}
		else if (string_starts_with(attrs[index], "min:")) {
			sscanf(string_cstr(attrs[index]), "min:%f", &out->min);
		}
		else if (string_starts_with(attrs[index], "max:")) {
			sscanf(string_cstr(attrs[index]), "max:%f", &out->max);
		}
		else if (string_match(attrs[index], "use")) {
			out->flags |= EDITOR_FLAG_UNION_DISPLAY;
		}
	}
}

static const ptrsize TYPE_UID_R32 = (ptrsize)reflect_info<r32>();
static const ptrsize TYPE_UID_U32 = (ptrsize)reflect_info<u32>();
static const ptrsize TYPE_UID_U64 = (ptrsize)reflect_info<u64>();

static const ptrsize TYPE_UID_VEC2 = (ptrsize)reflect_info<Vec2>();
static const ptrsize TYPE_UID_VEC3 = (ptrsize)reflect_info<Vec3>();
static const ptrsize TYPE_UID_VEC4 = (ptrsize)reflect_info<Vec4>();
static const ptrsize TYPE_UID_VPTR = (ptrsize)reflect_info<void>();

static inline void editor_display_fundamentals(ptrsize mem_type_uid, const char *name, void *data) {
	if (mem_type_uid == TYPE_UID_R32) {
		ImGui::Text("%s : %.3f", name, *(r32 *)data);
	}
	else if (mem_type_uid == TYPE_UID_VEC2) {
		Vec2 *val = (Vec2 *)data;
		ImGui::Text("%s : (%.3f, %.3f)", name, val->x, val->y);
	}
	else if (mem_type_uid == TYPE_UID_VEC3) {
		Vec3 *val = (Vec3 *)data;
		ImGui::Text("%s : (%.3f, %.3f, %.3f)", name, val->x, val->y, val->z);
	}
	else if (mem_type_uid == TYPE_UID_VEC4) {
		Vec4 *val = (Vec4 *)data;
		ImGui::Text("%s : (%.3f, %.3f, %.3f, %.3f)", name, val->x, val->y, val->z, val->w);
	}
	else if (mem_type_uid == TYPE_UID_U32) {
		u32 *val = (u32 *)data;
		ImGui::Text("%s : %u", name, *val);
	}
	else if (mem_type_uid == TYPE_UID_U64) {
		u64 *val = (u64 *)data;
		ImGui::Text("%s : #%016zx", name, *val);
	}
	else if (mem_type_uid == TYPE_UID_VPTR) {
		u64 val = (u64 )data;
		ImGui::Text("%s : #%zx", name, val);
	}
}

static inline void editor_display_editables(ptrsize mem_type_uid, const char *name, void *data, Editor_Attribute &attr) {
	if (mem_type_uid == TYPE_UID_R32) {
		ImGui::DragFloat(name, (r32 *)data, attr.speed, attr.min, attr.max);
	}
	else if (mem_type_uid == TYPE_UID_VEC2) {
		ImGui::DragFloat2(name, (r32 *)data, attr.speed, attr.min, attr.max);
	}
	else if (mem_type_uid == TYPE_UID_VEC3) {
		if (attr.flags & EDITOR_FLAG_COLOR) {
			ImGui::ColorEdit3(name, (r32 *)data);
		}
		else {
			ImGui::DragFloat3(name, (r32 *)data, attr.speed, attr.min, attr.max);
		}
	}
	else if (mem_type_uid == TYPE_UID_VEC4) {
		if (attr.flags & EDITOR_FLAG_COLOR) {
			ImGui::ColorEdit4(name, (r32 *)data);
		}
		else {
			ImGui::DragFloat4(name, (r32 *)data, attr.speed, attr.min, attr.max);
		}
	}
	else if (mem_type_uid == TYPE_UID_U32) {
		ImGui::DragScalar(name, ImGuiDataType_U32, (u32 *)data, attr.speed);
	}
	else if (mem_type_uid == TYPE_UID_U64) {
		ImGui::DragScalar(name, ImGuiDataType_U64, (u64 *)data, attr.speed);
	}
}

int reflected_get_enum_value(const Type_Info_Enum *info, void *data) {
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

void reflected_set_enum_value(const Type_Info_Enum *info, void *data, int index) {
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

bool editor_item_expands(const Type_Info *info) {
	return info->id == Type_Id_STRUCT;
}

void editor_draw(const Type_Info *base_info, char *data, Editor_Attribute attr, const char *name) {
	u32 parent_flags = attr.flags;

	switch (base_info->id) {
	case Type_Id_POINTER: {
		auto info = (Type_Info_Pointer *)base_info;

		if (editor_item_expands(info->pointer_to)) {
			if (ImGui::CollapsingHeader(string_cstr(info->pointer_to->name))) {
				editor_draw(info->pointer_to, (char *)(*(ptrsize *)data), attr, name);
			}
		}
		else {
			editor_draw(info->pointer_to, (char *)(*(ptrsize *)data), attr, name);
		}
	} break;

		// ignored
	case Type_Id_FUNCTION:
		break;

	case Type_Id_STRUCT: {
		auto info = (Type_Info_Struct *)base_info;
		auto mem_counts = info->member_count;

		if (info->base) {
			editor_draw(info->base, data, attr, string_cstr(info->base->name));
		}

		for (ptrsize index = 0; index < mem_counts; ++index) {
			auto mem = info->members + index;

			editor_get_flags_from_attribute(mem->attributes, mem->attribute_count, &attr);
			attr.flags |= parent_flags;
			if (attr.flags & EDITOR_FLAG_NO_DISPLAY) continue;

			if (editor_item_expands(mem->info)) {
				if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
					editor_draw(mem->info, data + mem->offset, attr, string_cstr(mem->name));
				}
			}
			else {
				editor_draw(mem->info, data + mem->offset, attr, string_cstr(mem->name));
			}

		}

	} break;

	case Type_Id_UNION: {
		ptrsize uid = (ptrsize)base_info;
		if (uid == TYPE_UID_VEC2 || uid == TYPE_UID_VEC3 || uid == TYPE_UID_VEC4) {
			if (parent_flags & EDITOR_FLAG_READ_ONLY) {
				editor_display_fundamentals(uid, name, data);
			}
			else {
				editor_display_editables(uid, name, data, attr);
			}

		}
		else {

			auto info = (Type_Info_Union *)base_info;
			auto mem_counts = info->member_count;

			if (mem_counts) {
				const Union_Member *mem = nullptr;
				for (ptrsize index = 0; index < mem_counts; ++index) {
					auto temp_mem = info->members + index;
					editor_get_flags_from_attribute(temp_mem->attributes, temp_mem->attribute_count, &attr);
					if (attr.flags & EDITOR_FLAG_UNION_DISPLAY) {
						mem = temp_mem;
						break;
					}
				}

				// If we don't find display attribute, we display the first member
				if (mem == nullptr) {
					mem = info->members;
					editor_get_flags_from_attribute(mem->attributes, mem->attribute_count, &attr);
				}

				attr.flags |= parent_flags;
				if (editor_item_expands(mem->info)) {
					if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
						editor_draw(mem->info, data, attr, string_cstr(mem->name));
					}
				}
				else {
					editor_draw(mem->info, data, attr, string_cstr(mem->name));
				}
			}

		}
	} break;

	case Type_Id_ENUM: {
		auto info = (Type_Info_Enum *)base_info;
		assert(info->is_sequential);

		int index = reflected_get_enum_value(info, data);

		if (parent_flags & EDITOR_FLAG_READ_ONLY) {
			ImGui::Text("%s : %s", name, info->item_strings[index]);
		}
		else {
			ImGui::Combo(name, &index, info->item_strings.data, (int)info->item_strings.count);
			reflected_set_enum_value(info, data, index);
		}
	} break;

	default: {
		ptrsize mem_type_uid = (ptrsize)base_info;
		if (parent_flags & EDITOR_FLAG_READ_ONLY) {
			editor_display_fundamentals(mem_type_uid, name, data);
		}
		else {
			editor_display_editables(mem_type_uid, name, data, attr);
		}
	} break;
	}

}

//
//
//

void editor_entity(Entity *entity) {
	static Entity null;
	
	if (entity == nullptr) {
		entity = &null;
	}

	ImGui::Begin("Entity:", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	switch (entity->type) {
		case Entity_Null: {
			editor_draw(*entity);
		} break;

		case Entity_Player: {
			editor_draw(*(Player *)entity);
		} break;

		case Entity_Static_Body: {
			editor_draw(*(Static_Body *)entity);
		} break;
	}	

	ImGui::End();
}
