#include "entity.h"

Entity_Manager manager_create() {
	Entity_Manager manager = {};
	return manager;
}

Entity *manager_add_entity_kind(Entity_Manager *manager, Entity_Kind kind) {
	Entity *result = nullptr;

	switch (kind) {
	case Entity_Player:
	{
		Player *player = array_add(&manager->by_type.players);
		result = player;
	} break;

	case Entity_Line:
	{
		Line *line = array_add(&manager->by_type.lines);
		result = line;
	} break;
	}

	manager->_counter += 1;

	Entity_Handle handle;
	handle.kind = kind;
	handle.index = manager->_counter;
	array_add(&manager->entities, handle);

	result->handle = handle;
	result->kind = kind;

	return result;
}
