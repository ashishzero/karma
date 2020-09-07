#pragma once
#include "lin_maths.h"
#include "reflection.h"

enum Entity_Kind : u32 {
	Entity_Invalid,
	Entity_Player,
	Entity_Line, // TODO: TEMPORARY
};

union Entity_Handle {
	struct {
		u32 index;
		Entity_Kind kind;
	};
	u64 id;
};

static constexpr Entity_Handle INVALID_ENTITY_HANDLE = {};

inline bool operator ==(Entity_Handle a, Entity_Handle b) { return a.id == b.id; }
inline bool operator !=(Entity_Handle a, Entity_Handle b) { return a.id != b.id; }

struct Entity {
	attribute("read-only") Entity_Handle	handle;
	attribute("read-only") Entity_Kind		kind;

	attribute("speed:0.01") Vec2 position;
};

struct Player : public Entity {
	attribute("speed:0.01") Vec2 size;
	attribute("color") Vec4 color;
	attribute("read-only") Vec2 velocity;
	attribute("read-only") Vec2 force;
};

struct Line : public Entity {
	attribute("color") Vec4 color;
	attribute("speed:0.01") Vec2 start;
	attribute("speed:0.01") Vec2 end;
};
