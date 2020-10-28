#pragma once
#include "modules/core/reflection.h"
#include "scene.h"

struct Editor_Attribute {
	u32 flags = 0;
	r32 speed = 0.01f;
	r32 min = 0.0f;
	r32 max = 0.0f;
};

void editor_draw(const Type_Info *base_info, char * data, Editor_Attribute attr, const char * name);

template <typename T>
inline void editor_draw(T& d) {
	Editor_Attribute attr = {};
	editor_draw(reflect_info<T>(), (char *)&d, attr, "Root");
}

void editor_entity(Entity *entity);
