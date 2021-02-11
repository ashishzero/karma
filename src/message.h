#pragma once
#include "entity.h"

constexpr u16 SERVER_CONNECTION_PORT = 9999;
constexpr u16 MAX_CLIENTS_PER_ROOM   = 8;

struct Join_Request_Payload {
	u32 version;
};

struct Join_Acknowledgement_Payload {
	Entity_Id player_id;
	u32		  author_id;
};

struct Room_Member_Payload {
	Entity_Id id[MAX_CLIENTS_PER_ROOM];
};

struct Room_Update_Payload {
	enum Type : u32 {
		READY, START
	};

	Type type;
};

struct Error_Payload {
	enum Type : u32 {
		INCORRECT_VERSION,
		ROOM_FULL,
		NOT_CAPTAIN
	};

	Type type;
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
		ROOM_UPDATE,
		ERROR,

		REMOVE_ENTITY,
		CHARACTER_SPAWN,
		BULLET_SPAWN,
		CHARACTER_SPACIAL_UPDATE,
		CHARACTER_COLOR_UPDATE,
		BULLET_SPACIAL_UPDATE,
		INPUT
	};

	union Payload {
		Join_Request_Payload				join_request;
		Join_Acknowledgement_Payload		join_acknowledgement;
		Room_Member_Payload					room_member;
		Room_Update_Payload					room_update;
		Error_Payload						error;

		Remove_Entity_Payload				remove_entity;
		Character_Spawn_Payload				character_spawn;
		Bullet_Spawn_Payload				bullet_spawn;
		Character_Spacial_Update_Payload	character_spacial_update;
		Character_Color_Update_Payload		character_color_update;
		Bullet_Spacial_Update_Payload		bullet_spacial_update;
		Input_Payload						input;
	};

	Header	header;
	Type	type;
	Payload payload;

	template <typename T>
	T *as(u32 id) {
		return nullptr;
	}

	template <> 
	Join_Request_Payload *as(u32 id) {
		header.source = Source::CLIENT;
		header.author_id = id;
		type = Type::JOIN_REQUEST;
		return &payload.join_request;
	}

	template <> 
	Join_Acknowledgement_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::JOIN_ACKNOWLEDGEMENT;
		return &payload.join_acknowledgement;
	}

	template <> 
	Room_Member_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::ROOM_MEMBER;
		return &payload.room_member;
	}

	template <> 
	Room_Update_Payload *as(u32 id) {
		header.source = Source::CLIENT;
		header.author_id = id;
		type = Type::ROOM_UPDATE;
		return &payload.room_update;
	}

	template <> 
	Error_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::ERROR;
		return &payload.error;
	}
			
	template <>
	Remove_Entity_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::REMOVE_ENTITY;
		return &payload.remove_entity;
	}

	template <>
	Character_Spawn_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::CHARACTER_SPAWN;
		return &payload.character_spawn;
	}

	template <>
	Bullet_Spawn_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::BULLET_SPAWN;
		return &payload.bullet_spawn;
	}

	template <>
	Character_Spacial_Update_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::CHARACTER_SPACIAL_UPDATE;
		return &payload.character_spacial_update;
	}

	template <>
	Character_Color_Update_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::CHARACTER_COLOR_UPDATE;
		return &payload.character_color_update;
	}

	template <>
	Bullet_Spacial_Update_Payload *as(u32 id) {
		header.source = Source::SERVER;
		header.author_id = id;
		type = Type::BULLET_SPACIAL_UPDATE;
		return &payload.bullet_spacial_update;
	}

	template <>
	Input_Payload *as(u32 id) {
		header.source = Source::CLIENT;
		header.author_id = id;
		type = Type::INPUT;
		return &payload.input;
	}

};
