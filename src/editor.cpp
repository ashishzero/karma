#include "imgui/imgui.h"
#include "entity.h"
#include ".generated/entity.typeinfo"

#include "editor.h"

constexpr u32 EDITOR_FLAG_NO_DISPLAY = bit(1);
constexpr u32 EDITOR_FLAG_READ_ONLY  = bit(2);
constexpr u32 EDITOR_FLAG_COLOR		 = bit(3);

struct Editor_Attribute {
	u32 flags;
	r32 speed;
	r32 min;
	r32 max;
};

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

#define struct_member(ptr, offset, type) (type *)(((char *)(ptr)) + (offset))

void editor_draw(Type_Info_Struct *info, void * data) {
	auto mem_counts = info->member_count;

	Editor_Attribute attr;

	for (ptrsize index = 0; index < mem_counts; ++index) {
		auto mem = info->members + index;

		editor_get_flags_from_attribute(mem->attributes, mem->attribute_count, &attr);
		if (attr.flags & EDITOR_FLAG_NO_DISPLAY) continue;
		
		ptrsize mem_type_uid = (ptrsize)mem->info;
		const char *name = string_cstr(mem->name);

		if (attr.flags & EDITOR_FLAG_READ_ONLY) {

			if (mem_type_uid == TYPE_UID_R32) {
				ImGui::Text("%s : %.3f", name, *struct_member(data, mem->offset, r32));
			} else if (mem_type_uid == TYPE_UID_VEC2) {
				Vec2 *val = struct_member(data, mem->offset, Vec2);
				ImGui::Text("%s : (%.3f, %.3f)", name, val->x, val->y);
			} else if (mem_type_uid == TYPE_UID_VEC3) {
				Vec3 *val = struct_member(data, mem->offset, Vec3);
				ImGui::Text("%s : (%.3f, %.3f, %.3f)", name, val->x, val->y, val->z);
			} else if (mem_type_uid == TYPE_UID_VEC4) {
				Vec4 *val = struct_member(data, mem->offset, Vec4);
				ImGui::Text("%s : (%.3f, %.3f, %.3f, %.3f)", name, val->x, val->y, val->z, val->w);
			} else if (mem_type_uid == TYPE_UID_U32) {
				u32 *val = struct_member(data, mem->offset, u32);
				ImGui::Text("%s : %u", name, *val);
			} else if (mem_type_uid == TYPE_UID_ENTITY_TYPE) {
				Entity::Type *val = struct_member(data, mem->offset, Entity::Type);
				ImGui::Text("%s : %s", name, string_cstr(enum_string(*val)));
			}
		} else {
			if (mem_type_uid == TYPE_UID_R32) {
				ImGui::DragFloat(name, struct_member(data, mem->offset, r32), attr.speed, attr.min, attr.max);
			} else if (mem_type_uid == TYPE_UID_VEC2) {
				ImGui::DragFloat2(name, struct_member(data, mem->offset, r32), attr.speed, attr.min, attr.max);
			} else if (mem_type_uid == TYPE_UID_VEC3) {
				if (attr.flags & EDITOR_FLAG_COLOR) {
					ImGui::ColorEdit3(name, struct_member(data, mem->offset, r32));
				} else {
					ImGui::DragFloat3(name, struct_member(data, mem->offset, r32), attr.speed, attr.min, attr.max);
				}
			} else if (mem_type_uid == TYPE_UID_VEC4) {
				if (attr.flags & EDITOR_FLAG_COLOR) {
					ImGui::ColorEdit4(name, struct_member(data, mem->offset, r32));
				} else {
					ImGui::DragFloat4(name, struct_member(data, mem->offset, r32), attr.speed, attr.min, attr.max);
				}
			} else if (mem_type_uid == TYPE_UID_U32) {
				ImGui::DragScalar(name, ImGuiDataType_U32, struct_member(data, mem->offset, u32), attr.speed);
			} else if (mem_type_uid == TYPE_UID_ENTITY_TYPE) {
				auto items = enum_string_array<Entity::Type>();
				ImGui::Combo(name, (int *)struct_member(data, mem->offset, Entity::Type), items.data, (int)items.count);
			}
		}
	}

}
