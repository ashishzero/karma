#pragma once
#include "reflection.h"
void editor_draw(Type_Info_Struct *info, void * data);

template <typename T>
inline void editor_draw(T& d) {
	editor_draw((Type_Info_Struct *)reflect_info<T>(), &d);
}
