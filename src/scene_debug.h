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
	Editor_Mode_MAP,
	Editor_Mode_ENTITY
};

struct Editor_Map {
	Rigid_Body *hovered_body;
	Rigid_Body *selected_body;
};

struct Editor_Fixture {
	r32		pointer_rotation;
};

struct Editor {
	Gizmo				gizmo;

	Camera		camera;
	Editor_Mode	mode;
	union {
		Editor_Map		map;
		Editor_Fixture	fixture;
	};
};

struct Scene;

Editor editor_create(Scene *scene);
bool editor_handle_event(const Event &event, Editor *editor);
void editor_set_mode_map(Scene *scene, Editor *editor);
void editor_set_mode_fixture(Scene *scene, Editor *editor);

//
//
//

void editor_update(Scene *scene, Editor *editor);
void editor_render(Scene *scene, Editor *editor, r32 aspect_ratio);
