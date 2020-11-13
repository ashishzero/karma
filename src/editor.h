#pragma once
#include "modules/core/karma.h"
#include "modules/core/reflection.h"

typedef u32 Attr_Flags;
enum Attr_Flag_Bit : Attr_Flags {
	Attr_NO_DISPLAY = bit(0),
	Attr_READ_ONLY = bit(1),
	Attr_SLIDER = bit(2),
	Attr_COLOR = bit(3),
	Attr_UNION_DISPLAY = bit(4),
};

struct Element_Attribute {
	Attr_Flags flags = 0;
	s32 step = 1;
	r32 speed = 0.01f;
	r32 min = -MAX_FLOAT;
	r32 max = MAX_FLOAT;
};

bool editor_widget_draw(const Type_Info *base_info, char *data, const Element_Attribute &attr, const char *name);

template <typename T>
bool editor(const T &value, const char *name) {
	Element_Attribute attr = {};
	return editor_widget_draw(reflect_info<T>(), (char *)&value, attr, name);
}

struct Entity;
bool editor_entity(Entity *entity);
