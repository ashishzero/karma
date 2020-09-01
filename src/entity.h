#pragma once
#include "lin_maths.h"
#include "reflection.h"

typedef u32 Entity_Handle;

struct Entity {
	enum Type : u32 {
		PLAYER,
		LINE,
	};

	attribute("read-only") Entity_Handle	handle;
	attribute("read-only") Type				type;

	Vec2 position;
	Vec2 size;
	attribute("color") Vec4 color;

	Vec2 velocity;
	Vec2 force;

	Vec2 start;
	Vec2 end;
};
