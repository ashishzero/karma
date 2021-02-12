#pragma once
#include "entity.h"

constexpr u16 SERVER_CONNECTION_PORT = 9999;
constexpr u16 MAX_CLIENTS_PER_ROOM   = 8;

constexpr int MAX_USERNAME_LENGTH = 255;

struct Username {
	u8 data[MAX_USERNAME_LENGTH];
	int length;
};

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

	u32 client_id;
	Username username;
};

struct Bullet_Spawn_Payload {
	Entity_Id id;
	Color_Id color_id;
	Vec2 position;
	Vec2 velocity;
	Transform transform;
};

struct Emitter_Spawn_Payload {
	Vec2 position;
	Color_Id color_id;
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

struct Input_Payload {
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

struct Character_Controller_Update_Payload {
	Entity_Id id;
	Input_Payload input;
};

struct Join_Request_Payload {
	u32			version;
	Username	username;
};

struct Join_Acknowledgement_Payload {
	u32			client_id;
	Entity_Id	player_id;
	Entity_Id	id[MAX_CLIENTS_PER_ROOM];
};

struct Get_Player_Payload {
	Entity_Id id;
};

struct Room_Member_Payload {
	Entity_Id	player_id[MAX_CLIENTS_PER_ROOM];
	bool		ready[MAX_CLIENTS_PER_ROOM];
};

struct Room_Update_Payload {
	enum Type : u32 {
		READY, UNREADY
	};

	Type type;
};

struct Start_Game_Payload {
	u32 player_count;
};

struct Finish_Game_Payload {
	Color_Id color_id;
};

struct Error_Payload {
	enum Type : u32 {
		INCORRECT_VERSION,
		ROOM_FULL,
		NOT_CAPTAIN
	};

	Type type;
};

//
//
//

struct Message {
	enum class Source : u32 {
		SERVER,
		CLIENT
	};

	struct Header {
		Source	source;
		u32		timestamp;
		u32		author_id;
	};

	enum class Type {
		JOIN_REQUEST,
		JOIN_ACKNOWLEDGEMENT,
		ROOM_MEMBER,
		GET_PLAYER,
		ROOM_UPDATE,
		START_GAME,
		FINISH_GAME,
		ERROR,

		REMOVE_ENTITY,
		CHARACTER_SPAWN,
		BULLET_SPAWN,
		EMITTER_SPAWN,
		CHARACTER_SPACIAL_UPDATE,
		CHARACTER_COLOR_UPDATE,
		BULLET_SPACIAL_UPDATE,
		INPUT,
		CHARACTER_CONTROLLER_UPDATE,
	};

	union Payload {
		Join_Request_Payload				join_request;
		Join_Acknowledgement_Payload		join_acknowledgement;
		Room_Member_Payload					room_member;
		Get_Player_Payload					get_player;
		Room_Update_Payload					room_update;
		Start_Game_Payload					start_game;
		Finish_Game_Payload					finish_game;
		Error_Payload						error;

		Remove_Entity_Payload				remove_entity;
		Character_Spawn_Payload				character_spawn;
		Bullet_Spawn_Payload				bullet_spawn;
		Emitter_Spawn_Payload				emitter_spawn;
		Character_Spacial_Update_Payload	character_spacial_update;
		Character_Color_Update_Payload		character_color_update;
		Bullet_Spacial_Update_Payload		bullet_spacial_update;
		Input_Payload						input;
		Character_Controller_Update_Payload	character_controller_update;
	};

	Header	header;
	Type	type;
	Payload payload;

	template <typename T>
	T *as(u32 id, u32 timestamp) {
		return nullptr;
	}

	template <> 
	Join_Request_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::CLIENT;
		header.author_id = id;
		header.timestamp = timestamp;
		type = Type::JOIN_REQUEST;
		return &payload.join_request;
	}

	template <> 
	Join_Acknowledgement_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::JOIN_ACKNOWLEDGEMENT;
		return &payload.join_acknowledgement;
	}

	template <> 
	Room_Member_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::ROOM_MEMBER;
		return &payload.room_member;
	}
	
	template <> 
	Get_Player_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::CLIENT;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::GET_PLAYER;
		return &payload.get_player;
	}

	template <> 
	Room_Update_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::CLIENT;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::ROOM_UPDATE;
		return &payload.room_update;
	}

	template <>
	Start_Game_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::START_GAME;
		return &payload.start_game;
	}

	template <>
	Finish_Game_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::FINISH_GAME;
		return &payload.finish_game;
	}

	template <> 
	Error_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::ERROR;
		return &payload.error;
	}
			
	template <>
	Remove_Entity_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::REMOVE_ENTITY;
		return &payload.remove_entity;
	}

	template <>
	Character_Spawn_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::CHARACTER_SPAWN;
		return &payload.character_spawn;
	}

	template <>
	Bullet_Spawn_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::BULLET_SPAWN;
		return &payload.bullet_spawn;
	}

	template <>
	Emitter_Spawn_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::EMITTER_SPAWN;
		return &payload.emitter_spawn;
	}

	template <>
	Character_Spacial_Update_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::CHARACTER_SPACIAL_UPDATE;
		return &payload.character_spacial_update;
	}

	template <>
	Character_Color_Update_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::CHARACTER_COLOR_UPDATE;
		return &payload.character_color_update;
	}

	template <>
	Bullet_Spacial_Update_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::BULLET_SPACIAL_UPDATE;
		return &payload.bullet_spacial_update;
	}

	template <>
	Input_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::CLIENT;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::INPUT;
		return &payload.input;
	}

	template <>
	Character_Controller_Update_Payload *as(u32 id, u32 timestamp) {
		header.source = Source::SERVER;
		header.author_id = id;
		header.timestamp = timestamp;

		type = Type::CHARACTER_CONTROLLER_UPDATE;
		return &payload.character_controller_update;
	}

	template <typename T>
	T *get() {
		return (T *)&payload;
	}

};
