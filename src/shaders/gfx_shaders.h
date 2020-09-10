#pragma once
#include "modules/core/karma.h"

typedef u32            Shader_Flags;
constexpr Shader_Flags SHADER_TYPE_VERTEX  = 0x00000001;
constexpr Shader_Flags SHADER_TYPE_PIXEL   = 0x00000002;
constexpr Shader_Flags SHADER_LANG_HLSL    = 0x80000000;
constexpr Shader_Flags SHADER_LANG_GLSL    = 0x40000000;
constexpr u32          SHADER_MAGIC_NUMBER = 0x2078666b;

/*
 * Shader Spec
 * Shader_Magic_Number (u32)
 * Shader_Table_Element_Count (u32)
 * Shader_Table_Element[Shader_Table_Element_Count]
 *
 * Shader_Table_Element Spec
 * Shader_Flags (u32)
 * Shader_Offset (u32)
 * Shader_Size (u32)
*/

struct Shader_Table_Element {
	Shader_Flags flag;
	u32          offset;
	u32          size;
};

struct Shader_Table {
	u32 magic_number;
	u32 elem_count;
	Shader_Table_Element *elems;
};
