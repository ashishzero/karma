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
