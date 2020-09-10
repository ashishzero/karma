#pragma once
#include "modules/core/reflection.h"

struct Editor_Attribute {
	u32 flags;
	r32 speed;
	r32 min;
	r32 max;
};

void editor_draw(const Type_Info *base_info, char * data, Editor_Attribute &attr, const char * name);

template <typename T>
inline void editor_draw(T& d) {
	Editor_Attribute attr = {};
	editor_draw(reflect_info<T>(), (char *)&d, attr, "Root");
}
