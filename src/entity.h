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

	attribute("speed:0.01") Vec2 position;
	attribute("speed:0.01") Vec2 size;
	attribute("color")		Vec4 color;

	attribute("read-only") Vec2 velocity;
	attribute("read-only") Vec2 force;

	attribute("speed:0.01") Vec2 start;
	attribute("speed:0.01") Vec2 end;
};
