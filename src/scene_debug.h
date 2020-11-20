#pragma once
#include "entity.h"

enum Gizmo_Type {
	Gizmo_Type_NONE,
	Gizmo_Type_CENTER,
	Gizmo_Type_TRANSLATE_X,
	Gizmo_Type_TRANSLATE_Y,
	Gizmo_Type_SCALE_X,
	Gizmo_Type_SCALE_Y,
	Gizmo_Type_ROTOR,

	Gizmo_Type_COUNT
};

enum Gizmo_Render_Type {
	Gizmo_Render_Type_NONE,
	Gizmo_Render_Type_TRANSLATE,
	Gizmo_Render_Type_SCALE,
	Gizmo_Render_Type_ROTATE,

	Gizmo_Render_Type_COUNT
};

struct Gizmo {
	Gizmo_Type type;
	Gizmo_Render_Type render_type;
	Vec2 out;
	r32 intensity[4]; // center, x, y, rotor
	r32 values[2];
};

enum Editor_Mode {
	Editor_Mode_GAME, 
	Editor_Mode_GAME_DEVELOPER, 
	Editor_Mode_LEVEL_EDITOR, 
	Editor_Mode_ENTITY_EDITOR
};

enum Editor_Flags : u32 {
	Editor_Flag_Bit_RENDER_WORLD		= bit(0),
	Editor_Flag_Bit_RENDER_FIXTURE		= bit(1),
	Editor_Flag_Bit_RENDER_COLLISION	= bit(2),
};

struct Editor_Map {
	Rigid_Body *hovered_body;
	Rigid_Body *selected_body;
	int			select_camera_index;
};

struct Editor_Fixture {
	r32		pointer_rotation;
};

struct Editor {
	Gizmo				gizmo;

	Camera		camera;
	Editor_Mode	mode;
	u32			flags;

	union {
		Editor_Map		map;
		Editor_Fixture	fixture;
	};
};

struct Scene;

Editor editor_create(Scene *scene);
bool editor_handle_event(const Event &event, Scene *scene, Editor *editor);
void editor_set_mode_game(Scene *scene, Editor *editor);
void editor_set_mode_game_developer(Scene *scene, Editor *editor);
void editor_set_mode_level_editor(Scene *scene, Editor *editor);
void editor_set_mode_entity_editor(Scene *scene, Editor *editor);
Camera *editor_rendering_camera(Scene *scene, Editor *editor);

//
//
//

void editor_update(Scene *scene, Editor *editor);
void editor_render(Scene *scene, Editor *editor, r32 aspect_ratio);
