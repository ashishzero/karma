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
	Editor_Flag_Bit_RENDER_BOUNDING_BOX	= bit(2),
	Editor_Flag_Bit_RENDER_COLLISION	= bit(3),
};

struct Editor_Level {
	Rigid_Body *hovered_body;
	Rigid_Body *selected_body;
	int			selected_camera_index;
	Level_Name	name_storage;
	bool		name_is_valid;

	Entity_Type		new_entity_type;
	int				selected_resource_index;
	r32				preview_shape_scale;
	bool			preview_shapes;
};

constexpr u32 MAXIMUM_POLYGON_VERTICES	= 100;
constexpr u32 MAXIMUM_FIXTURE_COUNT		= 25;

struct Editor_Entity {
	enum Mode {
		UNSELECTED,
		SELECTED,
		EDITING
	};

	Resource_Id		fixture_id;
	Resource_Name	fixture_name;

	Fixture fixtures[MAXIMUM_FIXTURE_COUNT];
	int		fixture_count;

	int			  selected_index;
	int			  hovered_index;

	Vec2 *		hovered_vertex;
	Vec2 *		selected_vertex;

	Mode			mode;
	int				added_vertex_index;
	bool			vertex_is_valid;
	r32				vertex_pointer_angle;

	Circle		circle_storage[MAXIMUM_FIXTURE_COUNT];
	Mm_Rect		mm_rect_storage[MAXIMUM_FIXTURE_COUNT];
	Capsule		capsule_storage[MAXIMUM_FIXTURE_COUNT];
	Polygon_Pt	polygon_storage[MAXIMUM_FIXTURE_COUNT];
	Vec2		polygon_vertices[MAXIMUM_FIXTURE_COUNT][MAXIMUM_POLYGON_VERTICES];
};

struct Editor {
	Gizmo		gizmo;

	Camera		camera;
	Editor_Mode	mode;
	u32			flags;

	Editor_Level	level;
	Editor_Entity	entity;
};

struct Scene;

Editor editor_create(Scene *scene);
bool editor_handle_event(const Event &event, Scene *scene, Editor *editor);
void editor_set_mode_game(Scene *scene, Editor *editor);
void editor_set_mode_game_developer(Scene *scene, Editor *editor);
void editor_set_mode_level_editor(Scene *scene, Editor *editor);
void editor_set_mode_entity_editor(Scene *scene, Editor *editor, Resource_Id id, const Resource_Name &name, Fixture *fixtures, u32 fixture_count);
Camera *editor_rendering_camera(Scene *scene, Editor *editor);

//
//
//

void editor_update(Scene *scene, Editor *editor);
void editor_render(Scene *scene, Editor *editor, r32 aspect_ratio);
