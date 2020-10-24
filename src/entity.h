#pragma once
#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

struct Entity {
	attribute("speed:0.01") Vec2 position;
};

struct Player : public Entity {
	attribute("color")		Vec4 color;
	attribute("read-only")	Vec2 velocity;
	attribute("read-only")	Vec2 force;
	attribute("speed:0.01", "min:0.0", "max:5.0") Circle collider;
};
