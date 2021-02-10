#pragma once
#include "entity.h"

constexpr u16 SERVER_CONNECTION_PORT = 9999;

struct Message_Input {
	enum Type : u32 {
		X_POINTER,
		Y_POINTER,
		X_AXIS,
		Y_AXIS,
		ATTACK
	};

	Type type;
	union {
		r32		real_value;
		s32		signed_value;
	};
};

//
//
//

struct Remove_Entity_Payload {
	Entity_Id id;
};

struct Character_Spawn_Payload {
	Entity_Id id;
	Color_Id color_id;
	Vec2 position;
	r32	 rotation;
	Vec2 velocity;
	Transform transform;
};

struct Bullet_Spawn_Payload {
	Entity_Id id;
	Color_Id color_id;
	Vec2 position;
	Vec2 velocity;
	Transform transform;
};

//
//
//

struct Character_Spacial_Update_Payload {
	Entity_Id id;
	Vec2 position;
	r32 rotation;
	u32 hit;
	Vec2 velocity;
	Transform transform;
};

struct Character_Color_Update_Payload {
	Entity_Id id;
	Color_Id color_id;
	r32 color_values[Color_Id_COUNT];
};

struct Bullet_Spacial_Update_Payload {
	Entity_Id id;
	Vec2 position;
	Vec2 velocity;
	Transform transform;
};
