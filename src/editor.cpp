#include "imgui/imgui.h"
#include "entity.h"
#include ".generated/entity.typeinfo"

#include "editor.h"

constexpr u32 EDITOR_FLAG_NO_DISPLAY = bit(1);
constexpr u32 EDITOR_FLAG_READ_ONLY  = bit(2);
constexpr u32 EDITOR_FLAG_COLOR		 = bit(3);

void editor_get_flags_from_attribute(const String* attrs, s64 count, Editor_Attribute *out) {
	out->flags = 0;
	out->speed = 1;
	out->min = 0.0f;
	out->max = 0.0f;

	for (s64 index = 0; index < count; ++index) {
		if (string_match(attrs[index], "no-display")) {
			out->flags |= EDITOR_FLAG_NO_DISPLAY;
			break; // If we are not displaying, than we don't need other attributes
		} else if (string_match(attrs[index], "read-only")) {
			out->flags |= EDITOR_FLAG_READ_ONLY;
		} else if (string_match(attrs[index], "color")) {
			out->flags |= EDITOR_FLAG_COLOR;
		} else if (string_starts_with(attrs[index], "speed:")) {
			sscanf(string_cstr(attrs[index]), "speed:%f", &out->speed);
		} else if (string_starts_with(attrs[index], "min:")) {
			sscanf(string_cstr(attrs[index]), "min:%f", &out->min);
		} else if (string_starts_with(attrs[index], "max:")) {
			sscanf(string_cstr(attrs[index]), "max:%f", &out->min);
		}
	}
}

static const ptrsize TYPE_UID_R32		  = (ptrsize)reflect_info<r32>();
static const ptrsize TYPE_UID_VEC2		  = (ptrsize)reflect_info<Vec2>();
static const ptrsize TYPE_UID_VEC3		  = (ptrsize)reflect_info<Vec3>();
static const ptrsize TYPE_UID_VEC4		  = (ptrsize)reflect_info<Vec4>();
static const ptrsize TYPE_UID_U32		  = (ptrsize)reflect_info<u32>();
static const ptrsize TYPE_UID_ENTITY_TYPE  = (ptrsize)reflect_info<Entity::Type>();

static inline void editor_display_fundamentals(ptrsize mem_type_uid, const char *name, void * data) {
	if (mem_type_uid == TYPE_UID_R32) {
		ImGui::Text("%s : %.3f", name, *(r32 *)data);
	} else if (mem_type_uid == TYPE_UID_VEC2) {
		Vec2 *val = (Vec2 *)data;
		ImGui::Text("%s : (%.3f, %.3f)", name, val->x, val->y);
	} else if (mem_type_uid == TYPE_UID_VEC3) {
		Vec3 *val = (Vec3 *)data;
		ImGui::Text("%s : (%.3f, %.3f, %.3f)", name, val->x, val->y, val->z);
	} else if (mem_type_uid == TYPE_UID_VEC4) {
		Vec4 *val = (Vec4 *)data;
		ImGui::Text("%s : (%.3f, %.3f, %.3f, %.3f)", name, val->x, val->y, val->z, val->w);
	} else if (mem_type_uid == TYPE_UID_U32) {
		u32 *val = (u32 *)data;
		ImGui::Text("%s : %u", name, *val);
	} else if (mem_type_uid == TYPE_UID_ENTITY_TYPE) {
		Entity::Type *val = (Entity::Type *)data;
		ImGui::Text("%s : %s", name, string_cstr(enum_string(*val)));
	}
}

static inline void editor_display_editables(ptrsize mem_type_uid, const char* name, void* data, Editor_Attribute &attr) {
	if (mem_type_uid == TYPE_UID_R32) {
		ImGui::DragFloat(name, (r32 *)data, attr.speed, attr.min, attr.max);
	} else if (mem_type_uid == TYPE_UID_VEC2) {
		ImGui::DragFloat2(name, (r32 *)data, attr.speed, attr.min, attr.max);
	} else if (mem_type_uid == TYPE_UID_VEC3) {
		if (attr.flags & EDITOR_FLAG_COLOR) {
			ImGui::ColorEdit3(name, (r32 *)data);
		} else {
			ImGui::DragFloat3(name, (r32 *)data, attr.speed, attr.min, attr.max);
		}
	} else if (mem_type_uid == TYPE_UID_VEC4) {
		if (attr.flags & EDITOR_FLAG_COLOR) {
			ImGui::ColorEdit4(name, (r32 *)data);
		} else {
			ImGui::DragFloat4(name, (r32 *)data, attr.speed, attr.min, attr.max);
		}
	} else if (mem_type_uid == TYPE_UID_U32) {
		ImGui::DragScalar(name, ImGuiDataType_U32, (u32 *)data, attr.speed);
	} else if (mem_type_uid == TYPE_UID_ENTITY_TYPE) {
		auto items = enum_string_array<Entity::Type>();
		ImGui::Combo(name, (int *)data, items.data, (int)items.count);
	}
}

void editor_draw(const Type_Info* base_info, char * data, Editor_Attribute &attr, const char * name) {
	u32 parent_flags = attr.flags;

	switch (base_info->id) {
		case Type_Id_POINTER: {
			auto info = (Type_Info_Pointer*)base_info;
			editor_draw(info->pointer_to, (char *)(*(ptrsize*)data), attr, name);
		} break;

			// ignored
		case Type_Id_FUNCTION:
			break;

		case Type_Id_STRUCT: {
			auto info = (Type_Info_Struct *)base_info;
			auto mem_counts = info->member_count;

			for (ptrsize index = 0; index < mem_counts; ++index) {
				auto mem = info->members + index;

				editor_get_flags_from_attribute(mem->attributes, mem->attribute_count, &attr);
				attr.flags |= parent_flags;
				if (attr.flags & EDITOR_FLAG_NO_DISPLAY) continue;

				if (mem->info->id == Type_Id_STRUCT) {
					if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
						editor_draw(mem->info, data + mem->offset, attr, string_cstr(mem->name));
					}
				} else {
					editor_draw(mem->info, data + mem->offset, attr, string_cstr(mem->name));
				}
			}
		} break;

		default: {
			ptrsize mem_type_uid = (ptrsize)base_info;
			if (parent_flags & EDITOR_FLAG_READ_ONLY) {
				editor_display_fundamentals(mem_type_uid, name, data);
			} else {
				editor_display_editables(mem_type_uid, name, data, attr);
			}
		} break;
	}
}

#if 0
void editor_draw(Type_Info_Struct *info, void * data, u32 parent_flags) {
	auto mem_counts = info->member_count;

	Editor_Attribute attr;

	for (ptrsize index = 0; index < mem_counts; ++index) {
		auto mem = info->members + index;

		editor_get_flags_from_attribute(mem->attributes, mem->attribute_count, &attr);
		attr.flags |= parent_flags;
		if (attr.flags & EDITOR_FLAG_NO_DISPLAY) continue;
		
		ptrsize mem_type_uid = (ptrsize)mem->info;
		const char *name = string_cstr(mem->name);

		if (attr.flags & EDITOR_FLAG_READ_ONLY) {
			if (mem->info->id == Type_Id_STRUCT) {
				if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
					editor_draw((Type_Info_Struct *)mem->info, struct_member(data, mem->offset, void), attr.flags);
				}
			} else {
				editor_display_fundamentals(mem_type_uid, name, data, mem->offset);
			}
		} else {
			if (mem->info->id == Type_Id_STRUCT) {
				if (ImGui::CollapsingHeader(string_cstr(mem->name))) {
					editor_draw((Type_Info_Struct *)mem->info, struct_member(data, mem->offset, void), attr.flags);
				}
			} else {
				editor_display_editables(mem_type_uid, name, data, mem->offset, attr);
			}
		}
	}

}
#endif