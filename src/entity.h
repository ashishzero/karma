#pragma once
#include "modules/core/lin_maths.h"
#include "modules/core/reflection.h"

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
	attribute("display") u64 id;
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


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


struct Entity_Manager {
	struct Entity_By_Type {
		Array<Player>   players;
		Array<Line>		lines;
	};

	Array<Entity_Handle>	entities;
	Entity_By_Type			by_type;
};

template <u32>
inline Entity *entity_down_type(Entity_Manager &manager, Entity_Handle reference) {
	return nullptr;
}
#define entity_down(manager, reference, entity) (entity *)entity_down_type<Entity_##entity>(manager, reference)

template <>
inline Entity *entity_down_type<Entity_Player>(Entity_Manager &manager, Entity_Handle reference) {
	return manager.by_type.players.data + reference.index;
}

template <>
inline Entity *entity_down_type<Entity_Line>(Entity_Manager &manager, Entity_Handle reference) {
	return manager.by_type.lines.data + reference.index;
}

inline Entity *entity_get(Entity_Manager &manager, Entity_Handle reference) {
	switch (reference.kind) {
	case Entity_Player: return entity_down_type<Entity_Player>(manager, reference);
	case Entity_Line: return entity_down_type<Entity_Line>(manager, reference);
	}
	return nullptr;
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


Entity_Manager manager_create();

Entity *manager_add_entity_kind(Entity_Manager *manager, Entity_Kind kind);
#define manager_add_entity(manager, entity) (entity *)manager_add_entity_kind(manager, Entity_##entity)

template <typename T>
T *manager_find(Array_View<T> a, Entity_Handle handle) {
	T *result = nullptr;
	for (s64 index = 0; index < a.count; ++index) {
		auto en = a.data + index;
		if (handle == en->handle) {
			result = en;
			break;
		}
	}
	return result;
}
