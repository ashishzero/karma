#include "entity.h"

Entity_Manager manager_create() {
	Entity_Manager manager = {};
	return manager;
}

Entity *manager_add_entity_kind(Entity_Manager *manager, Entity_Kind kind) {
	Entity *result = nullptr;
	s64 index = -1;

	switch (kind) {
	case Entity_Player:
	{
		Player *player = array_add(&manager->by_type.players);
		result = player;
		index = player - manager->by_type.players.data;
	} break;

	case Entity_Line:
	{
		Line *line = array_add(&manager->by_type.lines);
		result = line;
		index = line - manager->by_type.lines.data;
	} break;
	}

	assert(index >=0 && index < MAX_UINT32);

	Entity_Handle handle;
	handle.kind = kind;
	handle.index = (u32)index;
	array_add(&manager->entities, handle);

	result->handle = handle;
	result->kind = kind;

	return result;
}

Player *manager_find_player(Entity_Manager &manager, Entity_Handle handle) {
	Player *result = nullptr;
	auto &players = manager.by_type.players;
	for (s64 index = 0; index < players.count; ++index) {
		auto player = players.data + index;
		if (handle == player->handle) {
			result = player;
			break;
		}
	}
	return result;
}

Line *manager_find_line(Entity_Manager &manager, Entity_Handle handle) {
	Line *result = nullptr;
	auto &lines = manager.by_type.lines;
	for (s64 index = 0; index < lines.count; ++index) {
		auto line = lines.data + index;
		if (handle == line->handle) {
			result = line;
			break;
		}
	}
	return result;
}

Entity *manager_find_entity(Entity_Manager &manager, Entity_Handle handle) {
	switch (handle.kind) {
		case Entity_Player: return manager_find_player(manager, handle);
		case Entity_Line:   return manager_find_line(manager, handle);
	}

	return nullptr;
}
