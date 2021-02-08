#include "scene.h"
#include "scene_debug.h"
#include "modules/imgui/imgui.h"
#include "modules/imgui/editor.h"
#include "modules/gfx/renderer.h"
#include "modules/imgui/dev.h"
#include "modules/core/stb_image.h"
#include "scene.h"

#include ".generated/entity.typeinfo"

const r32  EDITOR_VERTEX_RADIUS = 0.02f;
const r32  EDITOR_VERTEX_SELECTOR_RADIUS = 0.04f;
const r32  EDITOR_VERTEX_SELECTOR_THICKNESS = 0.02f;
const Vec4 EDITOR_VERTEX_COLOR = vec4(1, 0, 0);
const Vec4 EDITOR_SELECTED_VERTEX_COLOR = vec4(0, 3, 3);
const Vec4 EDITOR_INVALID_SELECTED_VERTEX_COLOR = vec4(6, 0, 0);

const r32 GIZMO_LINE_THICKNESS = 0.022f;
const r32 GIZMO_LINE_HALF_THICKNESS = 0.5f * GIZMO_LINE_THICKNESS;
const r32 GIZMO_LINE_LENGTH = 0.3f;
const r32 GIZMO_POINTER_THICKNESS = 2.0f * GIZMO_LINE_THICKNESS;
const r32 GIZMO_POINTER_HALF_THICKNESS = 0.5f * GIZMO_POINTER_THICKNESS;
const Vec3 GIZMO_SQUARE_COLOR = vec3(1);
const r32 GIZMO_ROTOR_MAX_RADIUS = GIZMO_LINE_LENGTH;
const r32 GIZMO_ROTOR_MIN_RADIUS = GIZMO_ROTOR_MAX_RADIUS - 0.5f * GIZMO_POINTER_THICKNESS;
const Vec3 GIZMO_X_COLOR = vec3(0.8f, 0.1f, 0.1f);
const Vec3 GIZMO_Y_COLOR = vec3(0.1f, 0.8f, 0.1f);
const Vec3 GIZMO_ROTOR_COLOR = vec3(0.1f, 0.1f, 0.9f);
const Vec3 GIZMO_ROTOR_INDICATOR_COLOR = vec3(1, 1, 0.1f);
const r32 GIZMO_POINTER_OFFSET = GIZMO_LINE_LENGTH + GIZMO_LINE_HALF_THICKNESS;

const Mm_Rect GIZMO_CENTER_RECT = mm_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(-GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_THICKNESS));

const Mm_Rect GIZMO_X_LINE_RECT = mm_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS),
										  vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_LENGTH, GIZMO_LINE_THICKNESS));
const Mm_Rect GIZMO_Y_LINE_RECT = mm_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS),
										  vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS) + vec2(GIZMO_LINE_THICKNESS, GIZMO_LINE_LENGTH));

const Polygon GIZMO_X_POINTER_POLY = { 3, {
	vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
	vec2(GIZMO_POINTER_OFFSET, GIZMO_POINTER_HALF_THICKNESS),
	vec2(GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS, 0)
	}
};
const Polygon GIZMO_Y_POINTER_POLY = { 3, {
	vec2(GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
	vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
	vec2(0, GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS)
} };

const Mm_Rect GIZMO_X_POINTER_RECT = mm_rect(vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
											 vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS) + vec2(GIZMO_POINTER_THICKNESS));
const Mm_Rect GIZMO_Y_POINTER_RECT = mm_rect(vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
											 vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET) + vec2(GIZMO_POINTER_THICKNESS));

const Circle GIZMO_OUTER = { vec2(0), GIZMO_ROTOR_MAX_RADIUS };
const Circle GIZMO_INNER = { vec2(0), GIZMO_ROTOR_MIN_RADIUS };

//
//
//

Editor editor_create(Scene *scene) {
	Editor editor;
	editor.gizmo.type = Gizmo_Type_NONE;
	editor.gizmo.render_type = Gizmo_Render_Type_NONE;
	editor.gizmo.out = vec2(0);
	editor.gizmo.intensity[0] = 1;
	editor.gizmo.intensity[1] = 1;
	editor.gizmo.intensity[2] = 1;
	editor.gizmo.intensity[3] = 1;
	editor.gizmo.values[0] = 0;
	editor.gizmo.values[1] = 0;

	editor.level_camera.id = { 0 };
	editor.level_camera.type = Entity_Type_Camera;
	editor.level_camera.position = vec2(0);
	editor.level_camera.distance = 0;
	editor.level_camera.target_position = vec2(0);
	editor.level_camera.target_distance = 0;
	editor.level_camera.follow_factor = 0.99f;
	editor.level_camera.zoom_factor = 0.99f;
	editor.level_camera.behaviour = 0;

	editor.entity_camera = editor.level_camera;

	editor.entity.fixture_count = 0;
	editor.entity.selected_index = -1;
	editor.entity.hovered_index = -1;

	editor.entity.hovered_vertex = nullptr;
	editor.entity.selected_vertex = nullptr;

	editor.entity.mode = Editor_Entity::UNSELECTED;
	editor.entity.added_vertex_index = -1;
	editor.entity.vertex_is_valid = false;
	editor.entity.vertex_pointer_angle = 0;
	editor.entity.selected_texture = -1;
	memset(&editor.entity.texture, 0, sizeof(editor.entity.texture));

	editor.flags = Editor_Flag_Bit_RENDER_WORLD | Editor_Flag_Bit_RENDER_FIXTURE;

	editor.level.hovered_body = nullptr;
	editor.level.selected_body = nullptr;
	editor.level.selected_camera_index = -1;
	memset(editor.level.name_storage, 0, sizeof(Level_Name));
	editor.level.name_is_valid = false;

	editor.level.new_entity_type = Entity_Type_Character;
	editor.level.selected_resource_index = 0;
	editor.level.preview_shape_scale = 1;
	editor.level.preview_shapes = true;

	editor.mode = Editor_Mode_LEVEL_EDITOR;

	return editor;
}

bool editor_handle_event(const Event &event, Scene *scene, Editor *editor) {
	if (event.type & Event_Type_KEY_UP) {
		switch (event.key.symbol) {
			case Key_F1: {
				Dev_TogglePresentationState();
				return true;
			} break;

			case Key_F2: {
				if (editor->mode == Editor_Mode_GAME)
					editor_set_mode_level_editor(scene, &scene->editor);
				else
					editor_set_mode_game(scene, &scene->editor);
				return true;
			} break;
		}
	}

	if (editor->mode == Editor_Mode_GAME || editor->mode == Editor_Mode_GAME_DEVELOPER)
		return false;

	auto mode = editor->mode;
	auto &gizmo = editor->gizmo;

	if ((event.type & Event_Type_KEY_UP) && gizmo.render_type != Gizmo_Render_Type_NONE) {
		if (mode == Editor_Mode_LEVEL_EDITOR) {
			switch (event.key.symbol) {
				case Key_T: {
					gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
					gizmo.type = Gizmo_Type_NONE;
					return true;
				} break;
				case Key_S: {
					gizmo.render_type = Gizmo_Render_Type_SCALE;
					gizmo.type = Gizmo_Type_NONE;
					return true;
				} break;
				case Key_R: {
					gizmo.render_type = Gizmo_Render_Type_ROTATE;
					gizmo.type = Gizmo_Type_NONE;
					return true;
				} break;
			}
		} else if (mode == Editor_Mode_ENTITY_EDITOR) {
			switch (event.key.symbol) {
				case Key_T: {
					gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
					gizmo.type = Gizmo_Type_NONE;
					return true;
				} break;
				case Key_S: {
					assert(editor->entity.selected_index >= 0);
					Fixture *fixture = editor->entity.fixtures + editor->entity.selected_index;
					if (fixture->shape == Fixture_Shape_Circle || fixture->shape == Fixture_Shape_Capsule) {
						gizmo.render_type = Gizmo_Render_Type_SCALE;
						gizmo.type = Gizmo_Type_NONE;
					}
					return true;
				} break;
			}
		}
	}

	return true;
}

void ieditor_save_resource(Scene *scene, Editor *editor) {
	Fixture_Group fixture;
	fixture.fixtures = editor->entity.fixtures;
	fixture.count = editor->entity.fixture_count;

	Texture_Group texture;
	texture.handle.buffer.id.hptr = nullptr;
	texture.handle.view.id.hptr = nullptr;
	texture.uv = editor->entity.texture_uv;

	Resource_Header header;
	header.id = editor->entity.id;
	memcpy(header.name, editor->entity.name, sizeof(Resource_Name));
	memcpy(header.texture, editor->entity.texture_name, sizeof(Resource_Name));

	scene_save_resource(scene, header, texture, fixture, true);
}

inline void ieditor_reset(Scene *scene, Editor *editor, Editor_Mode new_mode) {
	editor->level.hovered_body = nullptr;
	editor->level.selected_body = nullptr;
	editor->level.selected_camera_index = -1;
	editor->gizmo.render_type = Gizmo_Render_Type_NONE;
	editor->gizmo.type = Gizmo_Type_NONE;
	editor->level_camera.behaviour = 0;
	editor->entity_camera.behaviour = 0;

	if (editor->mode == Editor_Mode_ENTITY_EDITOR) {
		if (editor->entity.texture.view.id) {
			gfx_destroy_texture2d(editor->entity.texture);
			memset(&editor->entity.texture, 0, sizeof(editor->entity.texture));
		}

		ieditor_save_resource(scene, editor);

		scene_reload_resource(scene, editor->entity.id);
		editor->entity.id = { 0 };
	}

	if (editor->mode != Editor_Mode_ENTITY_EDITOR && new_mode != Editor_Mode_ENTITY_EDITOR) {
		scene_reload_level(scene);
	}

	editor->mode = new_mode;
}

void editor_set_mode_game(Scene *scene, Editor *editor) {
	ieditor_reset(scene, editor, Editor_Mode_GAME);
	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;
}

void editor_set_mode_game_developer(Scene *scene, Editor *editor) {
	ieditor_reset(scene, editor, Editor_Mode_GAME_DEVELOPER);
	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;
}

void editor_set_mode_level_editor(Scene *scene, Editor *editor) {
	ieditor_reset(scene, editor, Editor_Mode_LEVEL_EDITOR);
	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;
	editor->level.new_entity_type = Entity_Type_Character;
	editor->level.selected_resource_index = 0;
	editor->level.preview_shapes = true;
	editor->level.preview_shape_scale = 1;
	editor->entity.selected_texture = -1;
}

void editor_set_mode_entity_editor(Scene *scene, Editor *editor, Resource_Collection *r) {
	u32 fixture_count = 0;

	if (r->fixture) {
		fixture_count = r->fixture->count;
	}

	assert(fixture_count < MAXIMUM_FIXTURE_COUNT);
	ieditor_reset(scene, editor, Editor_Mode_ENTITY_EDITOR);

	editor->entity.id = r->header->id;
	memcpy(editor->entity.name, r->header->name, sizeof(Resource_Name));

	bool loaded = false;

	if (r->texture) {
		memcpy(editor->entity.texture_name, r->header->texture, sizeof(Resource_Name));
		editor->entity.texture_uv = r->texture->uv;

		String path = tprintf("resources/textures/%s", editor->entity.texture_name);

		int w, h, n;
		String content = system_read_entire_file(path);

		if (content.count) {
			u8 *pixels = stbi_load_from_memory(content.data, (int)content.count, &w, &h, &n, 4);
			if (pixels) {
				editor->entity.texture = gfx_create_texture2d((u32)w, (u32)h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
				stbi_image_free(pixels);
				loaded = true;
			}

			memory_free(content.data);
		}
	}

	if (!loaded) {
		memset(editor->entity.texture_name, 0, sizeof(Resource_Name));
		editor->entity.texture_uv = mm_rect(0, 0, 1, 1);
		memset(&editor->entity.texture, 0, sizeof(editor->entity.texture));
	}

	editor->entity.fixture_count = fixture_count;

	if (fixture_count) {
		Fixture *src = r->fixture->fixtures;
		Fixture *dst = editor->entity.fixtures;

		for (u32 index = 0; index < fixture_count; ++index, ++src, ++dst) {
			switch (src->shape) {
				case Fixture_Shape_Circle: {
					dst->shape = Fixture_Shape_Circle;
					dst->handle = &editor->entity.circle_storage[index];
					memcpy(dst->handle, src->handle, sizeof(Circle));
				} break;

				case Fixture_Shape_Mm_Rect: {
					dst->shape = Fixture_Shape_Mm_Rect;
					dst->handle = &editor->entity.mm_rect_storage[index];
					memcpy(dst->handle, src->handle, sizeof(Mm_Rect));
				} break;

				case Fixture_Shape_Capsule: {
					dst->shape = Fixture_Shape_Capsule;
					dst->handle = &editor->entity.capsule_storage[index];
					memcpy(dst->handle, src->handle, sizeof(Capsule));
				} break;

				case Fixture_Shape_Polygon: {
					Polygon *polygon_src = fixture_get_shape(src, Polygon);
					Polygon_Pt *polygon_dst = &editor->entity.polygon_storage[index];
					dst->shape = Fixture_Shape_Polygon;
					dst->handle = polygon_dst;
					polygon_dst->vertex_count = polygon_src->vertex_count;
					polygon_dst->vertices = editor->entity.polygon_vertices[index];
					memcpy(polygon_dst->vertices, polygon_src->vertices, sizeof(Vec2) * polygon_src->vertex_count);
				} break;
			}
		}
	}

	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;
	editor->entity.hovered_index = -1;
	editor->entity.hovered_vertex = nullptr;
	editor->entity.selected_index = -1;
	editor->entity.selected_vertex = nullptr;
	editor->entity_camera.position = vec2(0);
	editor->entity_camera.distance = 0;
	editor->entity_camera.target_position = vec2(0);
	editor->entity_camera.target_distance = 0;
}

Camera *ieditor_get_selected_camera(Scene *scene, Editor *editor) {
	int index = editor->level.selected_camera_index;
	auto cameras = scene_cameras(scene);
	if (index >= 0 && index < cameras.count) {
		return &cameras[index];
	} else {
		editor->level.selected_camera_index = -1;
	}
	return nullptr;
}

void ieditor_select_camera(Editor *editor, int index) {
	editor->level.selected_body = nullptr;
	editor->level.selected_camera_index = index;
}

void ieditor_deselect_camera(Scene *scene, Editor *editor) {
	auto src = ieditor_get_selected_camera(scene, editor);
	if (src) {
		editor->level_camera.behaviour = 0;
		editor->level_camera.position = src->position;
		editor->level_camera.distance = src->distance;
		editor->level.selected_camera_index = -1;
	}
}

Camera *editor_rendering_camera(Scene *scene, Editor *editor) {
	switch (editor->mode) {
		case Editor_Mode_GAME: {
			return scene_primary_camera(scene);
		} break;

		case Editor_Mode_GAME_DEVELOPER: {
			if (editor->level.selected_camera_index >= 0)
				return ieditor_get_selected_camera(scene, editor);
			return scene_primary_camera(scene);
		} break;

		case Editor_Mode_LEVEL_EDITOR: {
			if (editor->level.selected_camera_index >= 0)
				return ieditor_get_selected_camera(scene, editor);
			return &editor->level_camera;
		} break;

		case Editor_Mode_ENTITY_EDITOR: {
			return &editor->entity_camera;
		} break;

			invalid_default_case();
	}

	return nullptr;
}

//
//
//

void ieditor_select_fixture(Editor *editor, int index) {
	editor->entity.selected_index = index;
	editor->entity.hovered_vertex = nullptr;
	editor->entity.selected_vertex = nullptr;
	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;
	if (index < 0)
		editor->entity.mode = Editor_Entity::UNSELECTED;
	else
		editor->entity.mode = Editor_Entity::SELECTED;
}

void ieditor_select_vertex(Editor *editor, Vec2 *vertex) {
	assert(editor->entity.selected_index >= 0);
	editor->entity.selected_vertex = vertex;
	editor->entity.mode = Editor_Entity::EDITING;
}

void ieditor_deselect_vertex(Editor *editor) {
	assert(editor->entity.selected_index >= 0);
	editor->entity.selected_vertex = nullptr;
	editor->entity.mode = Editor_Entity::SELECTED;
}

void ieditor_deselect_fixture(Editor *editor) {
	editor->entity.selected_vertex = nullptr;
	editor->entity.hovered_vertex = nullptr;
	editor->entity.selected_index = -1;
	editor->entity.hovered_index = -1;
	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;
	editor->entity.mode = Editor_Entity::UNSELECTED;
}

void ieditor_add_fixture(Editor *editor, Fixture_Shape shape) {
	assert(editor->mode == Editor_Mode_ENTITY_EDITOR);

	u32 index = editor->entity.fixture_count;
	Fixture *f = &editor->entity.fixtures[index];
	editor->entity.fixture_count += 1;
	f->shape = shape;

	editor->entity.added_vertex_index = -1;
	editor->entity.vertex_is_valid = false;

	switch (f->shape) {
		case Fixture_Shape_Circle: {
			auto circle = &editor->entity.circle_storage[index];
			circle->center = vec2(0);
			circle->radius = 0.1f;
			f->handle = circle;
		} break;

		case Fixture_Shape_Mm_Rect: {
			auto rect = &editor->entity.mm_rect_storage[index];
			rect->min = vec2(-0.1f);
			rect->max = vec2(0.1f);
			f->handle = rect;
		} break;

		case Fixture_Shape_Capsule: {
			Capsule *capsule = &editor->entity.capsule_storage[index];
			capsule->a = vec2(-0.1f, 0);
			capsule->b = vec2(0.1f, 0);
			capsule->radius = 0.1f;
			f->handle = capsule;
		} break;

		case Fixture_Shape_Polygon: {
			Polygon_Pt *polygon = &editor->entity.polygon_storage[index];
			polygon->vertex_count = 3;
			polygon->vertices = editor->entity.polygon_vertices[index];
			polygon->vertices[0] = vec2(-0.1f, -0.1f);
			polygon->vertices[1] = vec2(0.0f, 0.1f);
			polygon->vertices[2] = vec2(0.1f, -0.1f);
			f->handle = polygon;
		} break;

			invalid_default_case();
	}
}

void ieditor_remove_fixture(Editor *editor, int index) {
	assert(index < editor->entity.fixture_count);

	editor->entity.fixtures[index] = editor->entity.fixtures[editor->entity.fixture_count - 1];
	editor->entity.fixture_count -= 1;
	ieditor_deselect_fixture(editor);
}

template <typename ...Args>
void ieditor_gizmo_action(Gizmo *gizmo, Vec2 cursor, Vec2 delta, Vec2 p, const Args & ...args) {
	auto &io = ImGui::GetIO();

	for (auto &inten : gizmo->intensity) {
		inten = 1;
	}

	if (!io.MouseDown[ImGuiMouseButton_Left]) {
		gizmo->type = Gizmo_Type_NONE;
	}

	switch (gizmo->type) {
		case Gizmo_Type_NONE: {
			// Check if we have hovered over or used gizmo

			switch (gizmo->render_type) {
				case Gizmo_Render_Type_TRANSLATE: {
					if (test_shape_vs_point(GIZMO_CENTER_RECT, cursor, 0, args...)) {
						gizmo->intensity[0] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_CENTER;
					} else if (test_shape_vs_point(GIZMO_X_LINE_RECT, cursor, 0, args...) ||
							   test_shape_vs_point(GIZMO_X_POINTER_POLY, cursor, 0, args...)) {
						gizmo->intensity[1] = 4.5f;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_TRANSLATE_X;
					} else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, cursor, 0, args...) ||
							   test_shape_vs_point(GIZMO_Y_POINTER_POLY, cursor, 0, args...)) {
						gizmo->intensity[2] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_TRANSLATE_Y;
					}

					if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(0);
					}
				} break;

				case Gizmo_Render_Type_SCALE: {
					if (test_shape_vs_point(GIZMO_CENTER_RECT, cursor, 0, args...)) {
						gizmo->intensity[0] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_CENTER;
					} else if (test_shape_vs_point(GIZMO_X_LINE_RECT, cursor, 0, args...) ||
							   test_shape_vs_point(GIZMO_X_POINTER_RECT, cursor, 0, args...)) {
						gizmo->intensity[1] = 4.5f;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_SCALE_X;
							gizmo->values[0] = 1;
							gizmo->values[1] = 1;
						}
					} else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, cursor, 0, args...) ||
							   test_shape_vs_point(GIZMO_Y_POINTER_RECT, cursor, 0, args...)) {
						gizmo->intensity[2] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_SCALE_Y;
							gizmo->values[0] = 1;
							gizmo->values[1] = 1;
						}
					}

					if (gizmo->type == Gizmo_Type_CENTER) {
						gizmo->out = vec2(0);
					} else if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(1);
					}
				} break;

				case Gizmo_Render_Type_ROTATE: {
					if (test_shape_vs_point(GIZMO_OUTER, cursor, 0, args...) &&
						!test_shape_vs_point(GIZMO_INNER, cursor, 0, args...)) {
						gizmo->intensity[3] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_ROTOR;
							gizmo->values[0] = cursor.x - p.x;
							gizmo->values[1] = cursor.y - p.y;
						}
					}

					if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(0);
					}
				} break;
			}
		} break;

		case Gizmo_Type_CENTER: {
			gizmo->intensity[0] = 2;
			gizmo->out = delta;
		} break;

		case Gizmo_Type_TRANSLATE_X: {
			gizmo->intensity[1] = 4.5f;
			gizmo->out.x = delta.x;
			gizmo->out.y = 0;
		} break;

		case Gizmo_Type_TRANSLATE_Y: {
			gizmo->intensity[2] = 2;
			gizmo->out.x = 0;
			gizmo->out.y = delta.y;
		} break;

		case Gizmo_Type_SCALE_X: {
			gizmo->intensity[1] = 4.5f;

			r32 scale_amount_x, scale_amount_y;
			scale_amount_x = powf(2.0f, delta.x);
			scale_amount_y = io.KeyShift ? scale_amount_x : 1;
			gizmo->out.x = scale_amount_x;
			gizmo->out.y = scale_amount_y;
			gizmo->values[0] *= scale_amount_x;
			gizmo->values[1] *= scale_amount_y;
		} break;

		case Gizmo_Type_SCALE_Y: {
			gizmo->intensity[2] = 2;

			r32 scale_amount_x, scale_amount_y;
			scale_amount_y = powf(2.0f, delta.y);
			scale_amount_x = io.KeyShift ? scale_amount_y : 1;
			gizmo->out.x = scale_amount_x;
			gizmo->out.y = scale_amount_y;
			gizmo->values[0] *= scale_amount_x;
			gizmo->values[1] *= scale_amount_y;
		} break;

		case Gizmo_Type_ROTOR: {
			gizmo->intensity[3] = 2;

			Vec2 a = vec2_normalize_check(vec2(gizmo->values[0], gizmo->values[1]));
			Vec2 b = vec2_normalize_check(cursor - p);
			r32 angle = vec2_signed_angle_between(a, b);

			gizmo->out.x = angle;
			gizmo->values[0] = b.x;
			gizmo->values[1] = b.y;
		} break;
	}
}

void ieditor_try_select_fixture(Editor *editor, Vec2 cursor) {
	const auto &io = ImGui::GetIO();
	bool hovered = false;

	editor->entity.hovered_index = -1;

	int count = editor->entity.fixture_count;
	auto fixtures = editor->entity.fixtures;
	for (int index = 0; index < count; ++index) {
		Fixture *f = fixtures + index;

		switch (f->shape) {
			case Fixture_Shape_Circle: {
				auto shape = fixture_get_shape(f, Circle);
				if (test_shape_vs_point(*shape, cursor, 0)) {
					hovered = true;
					editor->entity.hovered_index = index;
				}
			} break;

			case Fixture_Shape_Mm_Rect: {
				auto shape = fixture_get_shape(f, Mm_Rect);
				if (test_shape_vs_point(*shape, cursor, 0)) {
					hovered = true;
					editor->entity.hovered_index = index;
				}
			} break;

			case Fixture_Shape_Capsule: {
				auto shape = fixture_get_shape(f, Capsule);
				if (test_shape_vs_point(*shape, cursor, 0)) {
					hovered = true;
					editor->entity.hovered_index = index;
				}
			} break;

			case Fixture_Shape_Polygon: {
				auto shape = fixture_get_shape(f, Polygon_Pt);
				if (test_shape_vs_point(*shape, cursor, 0)) {
					hovered = true;
					editor->entity.hovered_index = index;
				}
			} break;
		}
	}

	if (io.MouseClicked[ImGuiMouseButton_Left]) {
		ieditor_select_fixture(editor, editor->entity.hovered_index);
	}
}

void ieditor_try_select_vertex(Editor *editor, Vec2 cursor) {
	if (editor->entity.selected_index >= 0) {
		Fixture *f = editor->entity.fixtures + editor->entity.selected_index;

		editor->entity.hovered_vertex = nullptr;

		Circle p;
		p.radius = EDITOR_VERTEX_RADIUS;

		bool vertex_hovered = false;

		switch (f->shape) {
			case Fixture_Shape_Circle: {
				auto shape = fixture_get_shape(f, Circle);
				p.center = shape->center;
				if (test_shape_vs_point(p, cursor, 0)) {
					vertex_hovered = true;
					editor->entity.hovered_vertex = &shape->center;
				}
			} break;

			case Fixture_Shape_Mm_Rect: {
				auto shape = fixture_get_shape(f, Mm_Rect);
				p.center = shape->min;
				if (test_shape_vs_point(p, cursor, 0)) {
					vertex_hovered = true;
					editor->entity.hovered_vertex = &shape->min;
				} else {
					p.center = shape->max;
					if (test_shape_vs_point(p, cursor, 0)) {
						vertex_hovered = true;
						editor->entity.hovered_vertex = &shape->max;
					}
				}
			} break;

			case Fixture_Shape_Capsule: {
				auto shape = fixture_get_shape(f, Capsule);
				p.center = shape->a;
				if (test_shape_vs_point(p, cursor, 0)) {
					vertex_hovered = true;
					editor->entity.hovered_vertex = &shape->a;
				} else {
					p.center = shape->b;
					if (test_shape_vs_point(p, cursor, 0)) {
						vertex_hovered = true;
						editor->entity.hovered_vertex = &shape->b;
					}
				}
			} break;

			case Fixture_Shape_Polygon: {
				auto shape = fixture_get_shape(f, Polygon_Pt);
				u32 vcount = shape->vertex_count;
				auto v = shape->vertices;
				for (u32 vi = 0; vi < vcount; ++vi, ++v) {
					p.center = *v;
					if (test_shape_vs_point(p, cursor, 0)) {
						vertex_hovered = true;
						editor->entity.hovered_vertex = v;
						break;
					}
				}
			} break;
		}

		if (vertex_hovered && ImGui::GetIO().MouseClicked[ImGuiMouseButton_Left]) {
			ieditor_select_vertex(editor, editor->entity.hovered_vertex);

			Gizmo &gizmo = editor->gizmo;
			if (gizmo.render_type == Gizmo_Render_Type_NONE) {
				gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
			} else if (editor->entity.selected_vertex &&
					   (f->shape == Fixture_Shape_Mm_Rect || f->shape == Fixture_Shape_Polygon)) {
				gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
			}
		}

		if (!vertex_hovered && (editor->entity.mode == Editor_Entity::SELECTED || editor->entity.mode == Editor_Entity::EDITING)) {
			ieditor_try_select_fixture(editor, cursor);
		}
	}
}

void ieditor_select_body(Scene *scene, Editor *editor, Rigid_Body *body) {
	editor->level.selected_body = body;
	// Deselect Camera if pressed on empty world
	ieditor_deselect_camera(scene, editor);
}
#define ieditor_deselect_body(scene, editor) ieditor_select_body(scene, editor, nullptr);

void editor_update(Scene *scene, Editor *editor) {
	if (editor->mode == Editor_Mode_GAME) return;

	ImGui::GetStyle().Alpha = 1.0f;

	if (!ImGui_IsUsingCursor()) {
		ImGui::GetStyle().Alpha = 0.4f;

		Gizmo &gizmo = editor->gizmo;
		auto &io = ImGui::GetIO();

		Camera *camera = editor_rendering_camera(scene, editor);

		Vec2s client_size = system_get_client_size();
		r32 window_w = (r32)client_size.x;
		r32 window_h = (r32)client_size.y;

		r32 view_height = SCENE_VIEW_HEIGHT_HALF;
		r32 view_width = (window_w / window_h) * view_height;

		r32 scale = powf(0.5f, camera->distance);
		r32 iscale = 1.0f / scale;

		Vec2 cursor = io.MousePos;
		Vec2 delta = io.MouseDelta;

		// Convert cursor and delta value from window space into world space
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor.y = 1.0f - cursor.y;
		cursor = 2.0f * cursor - vec2(1);

		cursor.x *= iscale * view_width;
		cursor.y *= iscale * view_height;
		cursor += camera->position;

		delta.x /= window_w;
		delta.y /= (-window_h);

		delta.x *= 2 * iscale * view_width;
		delta.y *= 2 * iscale * view_height;

		switch (editor->mode) {
			case Editor_Mode_GAME_DEVELOPER: {
				// Get the Rigid_Body under the mouse cursor, nullptr if mouse doesn't hover over any Rigid_Body
				editor->level.hovered_body = scene_collide_point(scene, cursor);

				if (io.MouseClicked[ImGuiMouseButton_Left]) {
					ieditor_select_body(scene, editor, editor->level.hovered_body);
				}
			} break;

			case Editor_Mode_LEVEL_EDITOR: {
				// Get the Rigid_Body under the mouse cursor, nullptr if mouse doesn't hover over any Rigid_Body
				editor->level.hovered_body = scene_collide_point(scene, cursor);

				// Movement of the view of the world when editing
				if (io.MouseDown[ImGuiMouseButton_Right]) {
					clear_bit(camera->behaviour, Camera_Behaviour_ANIMATE_MOVEMENT);
					camera->position -= delta;
				}

				// Zoom in and out of the world when editing
				if (io.MouseWheel) {
					camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS;
					camera->target_distance -= io.DeltaTime * io.MouseWheel * 7;
				}

				if (editor->level.selected_body) {
					Transform gizmo_transform;
					gizmo_transform.p = editor->level.selected_body->transform.p;
					gizmo_transform.xform = mat2_scalar(iscale, iscale);

					ieditor_gizmo_action(&editor->gizmo, cursor, delta, gizmo_transform.p, gizmo_transform);

					switch (gizmo.type) {
						case Gizmo_Type_TRANSLATE_X:
						case Gizmo_Type_TRANSLATE_Y:
						case Gizmo_Type_CENTER: {
							Entity *entity = scene_entity_pointer(scene, scene_get_entity(scene, editor->level.selected_body->entity_id));
							entity->position += gizmo.out;
							editor->level.selected_body->transform.p = entity->position;
							scene_rigid_body_update_bounding_box(editor->level.selected_body, 0);
						} break;

						case Gizmo_Type_SCALE_X:
						case Gizmo_Type_SCALE_Y: {
							editor->level.selected_body->transform.xform = mat2_scalar(gizmo.out) * editor->level.selected_body->transform.xform;
							scene_rigid_body_update_bounding_box(editor->level.selected_body, 0);

							Entity_Reference ref;
							if (scene_find_entity(scene, editor->level.selected_body->entity_id, &ref)) {
								switch (ref.type) {
									case Entity_Type_Character: {
										Character *ent = scene_entity_pointer(scene, ref)->as<Character>();
										ent->size = vec2_hadamard(ent->size, gizmo.out);
									} break;

									case Entity_Type_Obstacle: {
										Obstacle *ent = scene_entity_pointer(scene, ref)->as<Obstacle>();
										ent->size = vec2_hadamard(ent->size, gizmo.out);
									} break;
								}
							}
						} break;

						case Gizmo_Type_ROTOR: {
							editor->level.selected_body->bounding_box = scene_rigid_body_bounding_box(editor->level.selected_body, 0);
							editor->level.selected_body->transform.xform = mat2_rotation(gizmo.out.x) * editor->level.selected_body->transform.xform;
							scene_rigid_body_update_bounding_box(editor->level.selected_body, 0);

							Entity_Reference ref;
							if (scene_find_entity(scene, editor->level.selected_body->entity_id, &ref)) {
								switch (ref.type) {
									case Entity_Type_Character: {
										Character *ent = scene_entity_pointer(scene, ref)->as<Character>();
										ent->rotation += gizmo.out.x;
									} break;

									case Entity_Type_Obstacle: {
										Obstacle *ent = scene_entity_pointer(scene, ref)->as<Obstacle>();
										ent->rotation += gizmo.out.x;
									} break;
								}
							}
						} break;
					}
				}

				if (gizmo.type == Gizmo_Type_NONE) {
					// If we are not using Gizmo and Press left button,
					// then hovered body is selected
					if (io.MouseClicked[ImGuiMouseButton_Left]) {
						ieditor_select_body(scene, editor, editor->level.hovered_body);
						if (gizmo.render_type == Gizmo_Render_Type_NONE)
							gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
					}

					// If we are not using Gizmo and Double click left button,
					// then hovered body is selected and focused
					if (editor->level.hovered_body && io.MouseDoubleClicked[ImGuiMouseButton_Left]) {
						ieditor_select_body(scene, editor, editor->level.hovered_body);
						if (gizmo.render_type == Gizmo_Render_Type_NONE)
							gizmo.render_type = Gizmo_Render_Type_TRANSLATE;

						// Focus on Rigid_Body
						camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS | Camera_Behaviour_ANIMATE_MOVEMENT;
						camera->target_position = editor->level.selected_body->transform.p;

						r32 sx = editor->level.selected_body->bounding_box.max.x - editor->level.selected_body->bounding_box.min.x;
						r32 sy = editor->level.selected_body->bounding_box.max.y - editor->level.selected_body->bounding_box.min.y;
						sx /= view_width;
						sy /= view_height;
						r32 s = maximum(sx, sy);

						camera->target_distance = log2f(s);
					}
				}

				if (editor->level.selected_body == nullptr) {
					gizmo.render_type = Gizmo_Render_Type_NONE;
				}
			} break;

			case Editor_Mode_ENTITY_EDITOR: {
				cursor -= camera->position;
				cursor += iscale * camera->position;

				// Movement of the view of the world when editing
				if (io.MouseDown[ImGuiMouseButton_Right]) {
					clear_bit(camera->behaviour, Camera_Behaviour_ANIMATE_MOVEMENT);
					camera->position -= delta;
				}

				// Zoom in and out of the world when editing
				if (io.MouseWheel) {
					camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS;
					camera->target_distance -= io.DeltaTime * io.MouseWheel * 7;
				}

				switch (editor->entity.mode) {
					case Editor_Entity::UNSELECTED: {
						ieditor_try_select_fixture(editor, cursor);
					} break;

					case Editor_Entity::SELECTED: {
						Fixture *f = editor->entity.fixtures + editor->entity.selected_index;

						if (editor->entity.added_vertex_index >= 0) {
							assert(f->shape == Fixture_Shape_Polygon);
							Polygon_Pt *polygon = fixture_get_shape(f, Polygon_Pt);
							auto vertices = polygon->vertices;
							auto count = polygon->vertex_count;
							int index = editor->entity.added_vertex_index;
							memmove(vertices + index, vertices + index + 1, sizeof(Vec2) * (count - index));
							editor->entity.added_vertex_index = -1;
							editor->entity.vertex_is_valid = false;
						}

						if (io.KeyCtrl && f->shape == Fixture_Shape_Polygon) {
							Polygon_Pt *polygon = fixture_get_shape(f, Polygon_Pt);
							if (polygon->vertex_count < MAXIMUM_POLYGON_VERTICES - 1) {
								auto vertices = polygon->vertices;
								auto count = polygon->vertex_count;
								Nearest_Edge edge = nearest_edge_point_polygon(vertices, count, cursor);
								memmove(vertices + edge.b_index + 1, vertices + edge.b_index, sizeof(Vec2) * (count - edge.b_index));
								vertices[edge.b_index] = cursor;
								editor->entity.added_vertex_index = edge.b_index;
								editor->entity.vertex_is_valid = is_polygon_convex(vertices, count + 1);

								editor->entity.vertex_pointer_angle += io.DeltaTime * 5;
								while (editor->entity.vertex_pointer_angle >= 2 * MATH_PI)
									editor->entity.vertex_pointer_angle -= 2 * MATH_PI;
							}
						} else {
							ieditor_try_select_vertex(editor, cursor);
						}

						if (io.MouseClicked[ImGuiMouseButton_Left] && editor->entity.added_vertex_index >= 0 && editor->entity.vertex_is_valid) {
							assert(f->shape == Fixture_Shape_Polygon);
							Polygon_Pt *polygon = fixture_get_shape(f, Polygon_Pt);
							polygon->vertex_count += 1;
							editor->entity.added_vertex_index = -1;
							editor->entity.vertex_is_valid = false;
						}
					} break;

					case Editor_Entity::EDITING: {
						Transform gizmo_transform;
						gizmo_transform.p = vec2(0);
						gizmo_transform.xform = mat2_scalar(iscale, iscale);

						ieditor_gizmo_action(&editor->gizmo, cursor - *editor->entity.selected_vertex, delta, vec2(0), gizmo_transform);

						switch (gizmo.type) {
							case Gizmo_Type_TRANSLATE_X:
							case Gizmo_Type_TRANSLATE_Y:
							case Gizmo_Type_CENTER: {
								Vec2 p = *editor->entity.selected_vertex;
								editor->entity.selected_vertex->x += gizmo.out.x;
								editor->entity.selected_vertex->y += gizmo.out.y;

								// Reject if the polygon is not convex
								Fixture &fixture = editor->entity.fixtures[editor->entity.selected_index];
								if (fixture.shape == Fixture_Shape_Polygon) {
									if (!is_polygon_convex(*fixture_get_shape(&fixture, Polygon_Pt))) {
										editor->entity.selected_vertex->x = p.x;
										editor->entity.selected_vertex->y = p.y;
									}
								}
							} break;

							case Gizmo_Type_SCALE_X:
							case Gizmo_Type_SCALE_Y: {
								Fixture &fixture = editor->entity.fixtures[editor->entity.selected_index];
								if (fixture.shape == Fixture_Shape_Circle) {
									auto circle = fixture_get_shape(&fixture, Circle);
									circle->radius *= ((gizmo.type == Gizmo_Type_SCALE_X) ? gizmo.out.x : gizmo.out.y);
								} else if (fixture.shape == Fixture_Shape_Capsule) {
									auto capsule = fixture_get_shape(&fixture, Capsule);
									capsule->radius *= ((gizmo.type == Gizmo_Type_SCALE_X) ? gizmo.out.x : gizmo.out.y);
								} else {
									invalid_code_path();
								}
							} break;

							case Gizmo_Type_ROTOR: {
								invalid_code_path();
							} break;

							case Gizmo_Type_NONE: {
								ieditor_try_select_vertex(editor, cursor);
							} break;
						}
					} break;
				}

				if (editor->entity.selected_vertex == nullptr) {
					gizmo.render_type = Gizmo_Render_Type_NONE;
				}

			} break;

		}
	}

	r32 dt = ImGui::GetIO().DeltaTime;
	Camera *camera = editor_rendering_camera(scene, editor);
	if (camera->behaviour & Camera_Behaviour_ANIMATE_MOVEMENT) {
		camera->position = lerp(camera->position, camera->target_position, 1.0f - powf(1.0f - camera->follow_factor, dt));
	}
	if (camera->behaviour & Camera_Behaviour_ANIMATE_FOCUS) {
		camera->distance = lerp(camera->distance, camera->target_distance, 1.0f - powf(1.0f - camera->zoom_factor, dt));
	}
}

void ieditor_fixture_group(Scene *scene, Editor *editor, Rigid_Body *body) {
	if (ImGui::CollapsingHeader("Fixtures")) {
		ImGui::Indent();
		ImGui::LabelText("id", "%016zx", (u64)body->fixtures);

		if (ImGui::CollapsingHeader("Shapes", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent();

			auto count = body->fixture_count;
			auto fixtures = body->fixtures;
			auto strings = enum_string_array<Fixture_Shape>();

			for (u32 index = 0; index < count; ++index) {
				auto f = fixtures + index;
				ImGui::BulletText(strings[f->shape]);
			}

			ImGui::Unindent();
		}

		ImGui::Unindent();
	}
}

inline void ieditor_create_new_entity(Scene *scene, Editor *editor, Entity_Type type, Resource_Collection *resource_collection, bool select) {
	Camera *camera = editor_rendering_camera(scene, editor);

	switch (type) {
		case Entity_Type_Camera: {
			Camera new_camera;
			ent_init_camera(&new_camera, camera->position, camera->distance);
			Camera *ent = scene_clone_entity(scene, &new_camera, camera->position, nullptr)->as<Camera>();
			if (select) {
				ieditor_select_camera(editor, scene_get_entity(scene, ent->id).index);
			}
		} break;

		case Entity_Type_Character: {
			Character character;
			Rigid_Body body;
			auto r0 = resource_collection[0];
			auto r1 = resource_collection[1];
			ent_init_character(&character, scene, camera->position, vec4(1), &body, r0.fixture, r0.texture, r0.index, r1.texture, r1.index);
			Character *ent = scene_clone_entity(scene, &character, camera->position, &r0.header->id)->as<Character>();
			if (select) {
				ieditor_select_body(scene, editor, ent->rigid_body);
			}
		} break;

		case Entity_Type_Obstacle: {
			Obstacle obstacle;
			Rigid_Body body;
			auto r0 = resource_collection[0];
			ent_init_obstacle(&obstacle, scene, camera->position, &body, r0.fixture, r0.texture, r0.index);
			Obstacle *ent = scene_clone_entity(scene, &obstacle, camera->position, &r0.header->id)->as<Obstacle>();
			if (select) {
				ieditor_select_body(scene, editor, ent->rigid_body);
			}
		} break;

		case Entity_Type_Bullet: {
			Bullet bullet;
			Rigid_Body body;
			auto r0 = resource_collection[0];
			ent_init_bullet(&bullet, scene, camera->position, 0.1f, 1, vec4(1), 0.1f, &body, r0.fixture);
			Bullet *ent = scene_clone_entity(scene, &bullet, camera->position, &r0.header->id)->as<Bullet>();
			if (select) {
				ieditor_select_body(scene, editor, ent->rigid_body);
			}
		} break;

			invalid_default_case();
	}
}

bool ieditor_gui_developer_editor(Scene *scene, Editor *editor) {
	bool result = false;

	// Level
	Level *level = scene_current_level_pointer(scene);

	ImGui::Begin("Level", nullptr, ImGuiWindowFlags_MenuBar);

	int new_entity_type_index = -1;
	bool create_new_level = false;
	bool open_another_level = false;

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			if (editor->mode == Editor_Mode_GAME_DEVELOPER) {
				if (ImGui::MenuItem("Edit##Level")) {
					editor_set_mode_level_editor(scene, editor);
				}
			} else {
				if (ImGui::MenuItem("New##Level")) {
					create_new_level = true;
				}

				if (ImGui::MenuItem("Open##Level")) {
					open_another_level = true;
				}

				if (ImGui::MenuItem("Save##Level")) {
					scene_save_level(scene);
				}

				if (ImGui::MenuItem("Close##Level")) {
					scene_save_level(scene);
					editor_set_mode_game_developer(scene, editor);
				}
			}

			ImGui::EndMenu();
		}

		if (editor->mode == Editor_Mode_LEVEL_EDITOR) {
			if (ImGui::BeginMenu("Add")) {
				auto type_strings = enum_string_array<Entity_Type>();

				const Entity_Type ADDABLE_ENTITY[] = {
					Entity_Type_Obstacle,
				};

				for (u32 counter = 0; counter < static_count(ADDABLE_ENTITY); ++counter) {
					auto index = ADDABLE_ENTITY[counter];
					if (ImGui::MenuItem(type_strings[index])) {
						new_entity_type_index = (int)index;
						break;
					}
				}

				ImGui::EndMenu();
			}
		}

		ImGui::EndMenuBar();
	}

	if (create_new_level) {
		editor->level.name_storage[0] = 0;
		editor->level.name_is_valid = true;
		ImGui::OpenPopup("Create New Level");
	}

	if (ImGui::BeginPopupModal("Create New Level", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::InputText("Name", editor->level.name_storage, sizeof(Level_Name))) {
			editor->level.name_is_valid = true;
		}

		if (!editor->level.name_is_valid) {
			ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "Level name already exist!");
		}

		if (editor->level.name_storage[0]) {
			if (ImGui::Button("Create##New Level", ImVec2(120, 0))) {
				String level_name = String(editor->level.name_storage, strlen(editor->level.name_storage));
				if (scene_create_new_level(scene, level_name)) {
					scene_load_level(scene, level_name);
					ieditor_deselect_body(scene, editor);
					ImGui::CloseCurrentPopup();
				} else {
					editor->level.name_is_valid = false;
				}
			}
		} else {
			ImGui::SetNextItemWidth(120);
			ImGui::Text("Create");
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (open_another_level) {
		editor->level.name_storage[0] = 0;
		editor->level.name_is_valid = true;
		ImGui::OpenPopup("Open Another Level");
	}

	if (ImGui::BeginPopupModal("Open Another Level", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Selected: %s", editor->level.name_storage);

		static ImGuiTextFilter filter;
		filter.Draw();

		auto mark = push_temporary_allocator();
		auto dirs = system_find_files("resources/levels", "*", false);
		pop_temporary_allocator(mark);

		int count = (int)dirs.count;
		for (int index = 0; index < count; ++index) {
			auto &d = dirs[index];
			if (string_match(d.name, ".") || string_match(d.name, "..") || d.extension.count)
				continue;

			ImGui::PushID((char *)d.name.data, (char *)d.name.data + d.name.count);

			const char *name = (char *)d.name.data;

			if (filter.PassFilter(name)) {
				if (ImGui::Selectable(name, strcmp(editor->level.name_storage, name) == 0, ImGuiSelectableFlags_DontClosePopups)) {
					strcpy(editor->level.name_storage, name);
					editor->level.name_is_valid = true;
				}
			}

			ImGui::PopID();
		}

		if (!editor->level.name_is_valid) {
			ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "Level could not be loaded!");
		}

		if (editor->level.name_storage[0]) {
			if (ImGui::Button("Open##Another Level", ImVec2(120, 0))) {
				if (scene_load_level(scene, String(editor->level.name_storage, strlen(editor->level.name_storage)))) {
					ieditor_deselect_body(scene, editor);
					ImGui::CloseCurrentPopup();
				} else {
					editor->level.name_is_valid = false;
				}
			}
		} else {
			ImGui::SetNextItemWidth(120);
			ImGui::Text("Open");
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (new_entity_type_index != -1) {
		editor->level.new_entity_type = (Entity_Type)new_entity_type_index;
		editor->level.selected_resource_index = 0;
		editor->level.preview_shape_scale = 1;
		editor->level.preview_shapes = true;
		ImGui::OpenPopup("Resource Selection");
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Resource Selection", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		Entity_Type new_entity_type = editor->level.new_entity_type;

		ImGui::Text("Entity: %s", enum_string(new_entity_type).data);

		if (new_entity_type == Entity_Type_Camera) {
			Camera camera;
			Camera *src = editor_rendering_camera(scene, editor);
			ent_init_camera(&camera, src->position, src->distance);
			scene_clone_entity(scene, &camera, src->position);
			ImGui::CloseCurrentPopup();
		}


		auto resource_headers = scene_resource_headers(scene);

		auto resources = resource_headers.data;
		int resource_count = (int)resource_headers.count;

		// Left
		{
			static ImGuiTextFilter filter;
			filter.Draw();

			ImGui::BeginChild("Resource", ImVec2(200, 500), true);

			int selected = editor->level.selected_resource_index;

			for (int index = 0; index < resource_count; ++index) {
				auto *header = resources + index;
				ImGui::PushID((void *)header);

				const char *name = header->name;

				if (filter.PassFilter(name)) {
					if (ImGui::Selectable(name, selected == index)) {
						selected = index;
					}
				}

				ImGui::PopID();
			}

			editor->level.selected_resource_index = selected;

			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginChild("Resource View", ImVec2(500, 500), true);

			int selected = editor->level.selected_resource_index;

			if (resource_count && selected >= 0) {
				auto header = resources + selected;

				auto resource_group = scene_find_resource(scene, header->id);

				ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
				ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
				if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
				if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
				ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

				ImGuiIO &io = ImGui::GetIO();

				if (io.MouseWheel) {
					editor->level.preview_shape_scale *= powf(0.5f, -io.DeltaTime * io.MouseWheel * 7);
				}

				ImDrawList *draw_list = ImGui::GetWindowDrawList();

				ImU32 grid_color = IM_COL32(200, 200, 200, 40);
				ImU32 shape_color = IM_COL32(255, 255, 0, 255);
				r32   scale_scalar = editor->level.preview_shape_scale;
				Vec2  shape_scale = vec2(scale_scalar, -scale_scalar);

				const ImVec2 origin(0.5f * (canvas_p0.x + canvas_p1.x), 0.5f * (canvas_p0.y + canvas_p1.y));

				if (resource_group.texture) {
					auto tex = resource_group.texture;
					auto scale = shape_scale;
					draw_list->AddImage(tex->handle.view.id.hptr,
										origin + vec2_hadamard(scale, canvas_p0 - origin), origin + vec2_hadamard(scale, canvas_p1 - origin),
										tex->uv.min, tex->uv.max);
				}

				if (editor->level.preview_shapes && resource_group.fixture) {
					Fixture *f = resource_group.fixture->fixtures;
					int fixture_count = resource_group.fixture->count;

					for (int index = 0; index < fixture_count; ++index, ++f) {
						switch (f->shape) {
							case Fixture_Shape_Circle: {
								auto shape = fixture_get_shape(f, Circle);
								draw_list->AddCircle(origin + vec2_hadamard(shape_scale, shape->center), scale_scalar * shape->radius, shape_color);
							} break;

							case Fixture_Shape_Mm_Rect: {
								auto shape = fixture_get_shape(f, Mm_Rect);
								draw_list->AddRect(origin + vec2_hadamard(shape_scale, shape->min), origin + vec2_hadamard(shape_scale, shape->max), shape_color);
							} break;

							case Fixture_Shape_Capsule: {
								auto shape = fixture_get_shape(f, Capsule);
								Vec2 capsule_dir = shape->b - shape->a;
								Vec2 capsule_norm = vec2_normalize(vec2(capsule_dir.y, capsule_dir.x)) * shape->radius * scale_scalar;

								Vec2 a, b, c, d;
								a = origin + vec2_hadamard(shape_scale, shape->a) - capsule_norm;
								b = origin + vec2_hadamard(shape_scale, shape->a) + capsule_norm;
								c = origin + vec2_hadamard(shape_scale, shape->b) + capsule_norm;
								d = origin + vec2_hadamard(shape_scale, shape->b) - capsule_norm;

								r32 radius = shape->radius * scale_scalar;
								draw_list->AddCircle(origin + vec2_hadamard(shape->a, shape_scale), radius, shape_color);
								draw_list->AddCircle(origin + vec2_hadamard(shape->b, shape_scale), radius, shape_color);

								draw_list->AddLine(a, d, shape_color);
								draw_list->AddLine(b, c, shape_color);
							} break;

							case Fixture_Shape_Polygon: {
								auto shape = fixture_get_shape(f, Polygon);
								const Vec2 *p, *q;
								p = shape->vertices;
								u32 vertex_count = shape->vertex_count;

								for (u32 index = 0; index < vertex_count - 1; ++index) {
									q = p + 1;
									draw_list->AddLine(origin + vec2_hadamard(*p, shape_scale), origin + vec2_hadamard(*q, shape_scale), shape_color);
									p = q;
								}
								draw_list->AddLine(origin + vec2_hadamard(*shape->vertices, shape_scale), origin + vec2_hadamard(*p, shape_scale), shape_color);
							} break;
						}
					}
				}

				ImGui::Checkbox("Shapes##Preview", &editor->level.preview_shapes);
			} else {
				ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.3f, 1.0f), "No resource selected!");
			}

			ImGui::EndChild();
		}

		ImGui::SetCursorPosX(250);

		if (ImGui::Button("Select", ImVec2(120, 0))) {
			int selected = editor->level.selected_resource_index;
			auto header = resources + selected;
			auto resource_group = scene_find_resource(scene, header->id);
			ieditor_create_new_entity(scene, editor, new_entity_type, &resource_group, true);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Create New...", ImVec2(120, 0))) {
			Resource_Name name, tex;
			tex[0] = 0;
			strncpy(name, "(unnamed)", sizeof(Resource_Name));

			Resource_Collection resource_collection;
			resource_collection.fixture = nullptr;
			resource_collection.texture = nullptr;
			resource_collection.index = MAX_UINT32;
			resource_collection.header = (Resource_Header *)scene_create_new_resource(scene, name, nullptr, 0, tex, mm_rect(0, 0, 1, 1));

			ieditor_create_new_entity(scene, editor, new_entity_type, &resource_collection, false);
			editor_set_mode_entity_editor(scene, editor, &resource_collection);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::Text(level->name);
	ImGui::SameLine();
	if (ImGui::Button(":##EditLevelName")) {
		ImGui::OpenPopup("Edit Level Name");
		memcpy(editor->level.name_storage, level->name, sizeof(Level_Name));
		editor->level.name_is_valid = true;
	}

	center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Level Name", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::InputText("Name", editor->level.name_storage, sizeof(Level_Name))) {
			editor->level.name_is_valid = true;
		}

		if (!editor->level.name_is_valid) {
			ImGui::TextColored(ImVec4(0.8f, 0.1f, 0.1f, 1.0f), "Level name already exist!");
		}

		if (ImGui::Button("Change", ImVec2(120, 0))) {
			String old_path = tprintf("resources/levels/%s", level->name);
			String new_path = tprintf("resources/levels/%s", editor->level.name_storage);
			if (system_rename_directory(old_path, new_path)) {
				memcpy(level->name, editor->level.name_storage, sizeof(Level_Name));
				level->name_count = (u32)strlen(level->name);
				ImGui::CloseCurrentPopup();
			} else {
				editor->level.name_is_valid = false;
			}
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::CollapsingHeader("Cameras")) {
		ImGui::Indent();

		const Array_View<Camera> cameras = scene_cameras(scene);
		int count = (int)cameras.count;
		int selected = editor->level.selected_camera_index;
		for (int index = 0; index < count; index++) {
			auto &c = cameras[index];
			ImGui::PushID((void *)&c);
			if (ImGui::Selectable(null_tprintf("%d Camera", index), selected == index)) {
				if (selected == index) {
					ieditor_deselect_camera(scene, editor);
				} else {
					ieditor_select_camera(editor, index);
				}
			}
			ImGui::PopID();
		}

		ImGui::Unindent();
	}

	u32 *flags = &editor->flags;
	ImGui::CheckboxFlags("Render World", flags, Editor_Flag_Bit_RENDER_WORLD);
	ImGui::CheckboxFlags("Render Fixture", flags, Editor_Flag_Bit_RENDER_FIXTURE);
	ImGui::CheckboxFlags("Render Bounding Box", flags, Editor_Flag_Bit_RENDER_BOUNDING_BOX);
	ImGui::CheckboxFlags("Render Collision", flags, Editor_Flag_Bit_RENDER_COLLISION);
	ImGui::CheckboxFlags("Render Broadphase", flags, Editor_Flag_Bit_RENDER_BROADPHASE);

	if (ImGui::CollapsingHeader("Physics")) {
		editor_widget(scene->physics, "Physics");
	}

	ImGui::End();

	// Entity Properties
	ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	Entity_Reference entity_ref;
	Entity *entity = nullptr;
	if (editor->level.selected_body) {
		entity_ref = scene_get_entity(scene, editor->level.selected_body->entity_id);
		entity = scene_entity_pointer(scene, entity_ref);
	} else if (editor->level.selected_camera_index >= 0) {
		entity = ieditor_get_selected_camera(scene, editor);
		entity_ref = scene_get_entity(scene, entity->id);
	}

	if (entity == nullptr) {
		ImGui::LabelText("type", "Entity_Type_Null");
		ImGui::End();
		return false;
	}

	Rigid_Body *body = nullptr;

	switch (entity->type) {
		case Entity_Type_Camera: {
			Camera *c = (Camera *)entity;
			result = editor_widget<Camera>(*c, "Camera Entity");
			if (result) {
				c->behaviour = 0;
			}
		} break;

		case Entity_Type_Character: {
			Character *c = (Character *)entity;
			result = editor_widget<Character>(*c, "Character Entity");
			body = c->rigid_body;
		} break;

		case Entity_Type_Obstacle: {
			Obstacle *o = (Obstacle *)entity;
			result = editor_widget<Obstacle>(*o, "Obstacle Entity");
			if (result) {
				o->rigid_body->transform.p = entity->position;
				scene_rigid_body_update_bounding_box(o->rigid_body, 0);
			}

			body = o->rigid_body;
		} break;

			invalid_default_case();
	}

	if (body) {
		ieditor_fixture_group(scene, editor, body);
		if (ImGui::Button("Edit##EntityEditor")) {
			auto res_id = scene_find_entity_resource_id(scene, body->entity_id);
			auto res_col = scene_find_resource(scene, res_id);
			editor_set_mode_entity_editor(scene, editor, &res_col);
		}
	}

	if (ImGui::Button("Clone##Entity")) {
		Camera *camera = editor_rendering_camera(scene, editor);
		Entity *new_ent = scene_clone_entity(scene, entity, camera->position);

		switch (new_ent->type) {
			case Entity_Type_Character: {
				Character *ent = (Character *)new_ent;
				ieditor_select_body(scene, editor, ent->rigid_body);
			} break;

			case Entity_Type_Obstacle: {
				Obstacle *ent = (Obstacle *)new_ent;
				ieditor_select_body(scene, editor, ent->rigid_body);
			} break;
		}
	}

	if (ImGui::Button("Remove##Entity")) {
		scene_remove_entity_from_level(scene, entity_ref, entity->id);
		ieditor_deselect_body(scene, editor);
	}

	ImGui::End();

	return result;
}

bool ieditor_fixture(Editor *editor, Fixture &fixture) {
	bool result = false;

	switch (fixture.shape) {
		case Fixture_Shape_Circle: {
			auto shape = fixture_get_shape(&fixture, Circle);
			result = editor_widget(*shape, "Circle");
		} break;

		case Fixture_Shape_Mm_Rect: {
			auto shape = fixture_get_shape(&fixture, Mm_Rect);
			result = editor_widget(*shape, "Mm_Rect");
		} break;

		case Fixture_Shape_Capsule: {
			auto shape = fixture_get_shape(&fixture, Capsule);
			result = editor_widget(*shape, "Capsule");
		} break;

		case Fixture_Shape_Polygon: {
			if (editor->entity.added_vertex_index == -1) {
				auto shape = fixture_get_shape(&fixture, Polygon_Pt);
				int count = (int)shape->vertex_count;
				Vec2 *vertices = shape->vertices;

				Array<int> to_be_removed;
				to_be_removed.allocator = TEMPORARY_ALLOCATOR;

				char label[5];
				for (int index = 0; index < count; ++index) {
					ImGui::PushID((void *)(vertices + index));
					snprintf(label, sizeof(label), "%u", index);
					Vec2 v = vertices[index];
					if (ImGui::DragFloat2(label, vertices[index].m, 0.01f, 0.0f, 0.0f, "%.4f", 0)) {
						if (!is_polygon_convex(*shape)) {
							vertices[index] = v;
						}
						result = true;
					}

					if (count > 3) {
						ImGui::SameLine();
						if (ImGui::Button("-")) {
							array_add(&to_be_removed, index);
						}
					}
					ImGui::PopID();
				}

				count = (int)to_be_removed.count;
				if (count) {
					ieditor_deselect_vertex(editor);
					for (int index = count - 1; index >= 0; --index) {
						int remove_index = to_be_removed[index];
						memmove(vertices + remove_index, vertices + remove_index + 1, sizeof(Vec2) * (shape->vertex_count - remove_index - 1));
						shape->vertex_count -= 1;
					}
				}

			}

		} break;

			invalid_default_case();
	}

	return result;
}

void ieditor_load_new_texture(Editor *editor) {
	if (editor->entity.texture.view.id) {
		gfx_destroy_texture2d(editor->entity.texture);
	}

	bool loaded = false;

	if (editor->entity.texture_name[0]) {
		String path = tprintf("resources/textures/%s", editor->entity.texture_name);

		int w, h, n;
		String content = system_read_entire_file(path);

		if (content.count) {
			u8 *pixels = stbi_load_from_memory(content.data, (int)content.count, &w, &h, &n, 4);
			if (pixels) {
				editor->entity.texture = gfx_create_texture2d((u32)w, (u32)h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
				stbi_image_free(pixels);
				loaded = true;
			}

			memory_free(content.data);
		}
	}

	if (!loaded) {
		memset(editor->entity.texture_name, 0, sizeof(Resource_Name));
		editor->entity.texture_uv = mm_rect(0, 0, 1, 1);
		memset(&editor->entity.texture, 0, sizeof(editor->entity.texture));
	}
}

bool ieditor_gui_entity_editor(Scene *scene, Editor *editor) {
	bool result = false;

	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	bool open = (editor->mode == Editor_Mode_ENTITY_EDITOR);
	if (ImGui::Begin("Properties", &open)) {

		// Left
		{
			ImGui::BeginChild("Fixtures", ImVec2(100, 0), true);

			int count = editor->entity.fixture_count;
			auto fixtures = editor->entity.fixtures;
			int selected = editor->entity.selected_index;

			for (int index = 0; index < count; ++index) {
				Fixture *f = fixtures + index;
				ImGui::PushID((void *)f);

				const char *name = (char *)enum_string(f->shape).data + sizeof("Fixture_Shape");
				if (ImGui::Selectable(name, selected == index)) {
					ieditor_select_fixture(editor, index);
				}

				ImGui::PopID();
			}

			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("Resource View", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

			ImGui::LabelText("id", "%zu", editor->entity.id);
			ImGui::InputText("name", editor->entity.name, sizeof(Resource_Name));

			ImGui::LabelText("texture", "%s", editor->entity.texture_name);
			ImGui::SameLine();
			if (ImGui::Button("X##Change Texture")) {
				ImGui::OpenPopup("Select Texture");
			}

			if (ImGui::BeginPopupModal("Select Texture", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

				int selected = editor->entity.selected_texture;

				auto files = system_find_files("resources/textures/", ".png", false);
				defer{ memory_free(files.data); };

				for (auto index = 0; index < files.count; ++index) {
					if (ImGui::Selectable((char *)files[index].name.data, selected == index, ImGuiSelectableFlags_DontClosePopups)) {
						selected = index;
					}
				}

				editor->entity.selected_texture = selected;

				if (selected >= 0) {
					if (ImGui::Button("Select")) {
						int length = (int)minimum(files[selected].name.count, sizeof(Resource_Name) - 1);
						memcpy(editor->entity.texture_name, files[selected].name.data, length);
						editor->entity.texture_name[length] = 0;
						ieditor_load_new_texture(editor);
						ImGui::CloseCurrentPopup();
					}
				} else {
					ImGui::TextDisabled("Select");
				}
				ImGui::SameLine();

				if (ImGui::Button("Remove")) {
					editor->entity.texture_name[0] = 0;
					ieditor_load_new_texture(editor);
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::Separator();

			ImGui::TextColored(ImVec4(0.2f, 0.3f, 0.7f, 1.0f), "Shape");

			if (editor->entity.selected_index >= 0) {
				Fixture &f = editor->entity.fixtures[editor->entity.selected_index];
				ieditor_fixture(editor, f);
				if (ImGui::Button("Remove##Shape")) {
					ieditor_remove_fixture(editor, editor->entity.selected_index);
				}
			}

			ImGui::EndChild();

			if (ImGui::Button("Save")) {
				ieditor_save_resource(scene, editor);
			}
			ImGui::SameLine();

			if (editor->entity.fixture_count < MAXIMUM_FIXTURE_COUNT) {
				if (ImGui::Button("New Shape##FixtureShape")) {
					ImGui::OpenPopup("Create New Fixture");
				}
			} else {
				ImGui::Text("New Shape");
			}

			if (ImGui::BeginPopup("Create New Fixture")) {
				auto strings = enum_string_array<Fixture_Shape>();
				for (u32 i = 0; i < Fixture_Shape_Count; ++i) {
					if (ImGui::Selectable(strings[i])) {
						ieditor_add_fixture(editor, (Fixture_Shape)i);
					}
				}

				ImGui::EndPopup();
			}

			ImGui::EndGroup();
		}
	}
	ImGui::End();


	if (!open) {
		editor_set_mode_level_editor(scene, editor);
	}

	return result;
}

void ieditor_gizmo_render(const Gizmo &gizmo, const Mat2 &t) {
	switch (gizmo.render_type) {
		case Gizmo_Render_Type_TRANSLATE: {
			// square
			im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[0] * GIZMO_SQUARE_COLOR, 1));

			// x
			im2d_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_LENGTH, GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

			// y
			im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS, GIZMO_LINE_LENGTH), vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

			// x pointer arrow
			im2d_triangle(vec2(GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS),
						  vec2(GIZMO_POINTER_OFFSET, GIZMO_POINTER_HALF_THICKNESS),
						  vec2(GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS, 0),
						  vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

			// y pointer arrow
			im2d_triangle(vec2(GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
						  vec2(-GIZMO_POINTER_HALF_THICKNESS, GIZMO_POINTER_OFFSET),
						  vec2(0, GIZMO_POINTER_OFFSET + GIZMO_POINTER_THICKNESS),
						  vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

		} break;

		case Gizmo_Render_Type_SCALE: {
			r32 offset_x = GIZMO_LINE_LENGTH, offset_y = GIZMO_LINE_LENGTH;

			if (gizmo.type == Gizmo_Type_SCALE_X || gizmo.type == Gizmo_Type_SCALE_Y) {
				offset_x *= gizmo.values[0];
				offset_y *= gizmo.values[1];
			}

			// square
			im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[0] * GIZMO_SQUARE_COLOR, 1));

			// x
			im2d_rect(vec2(GIZMO_LINE_HALF_THICKNESS, -GIZMO_LINE_HALF_THICKNESS), vec2(offset_x, GIZMO_LINE_THICKNESS), vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

			// y
			im2d_rect(vec2(-GIZMO_LINE_HALF_THICKNESS, GIZMO_LINE_HALF_THICKNESS), vec2(GIZMO_LINE_THICKNESS, offset_y), vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

			// x pointer box
			im2d_rect(vec2(offset_x - GIZMO_LINE_LENGTH + GIZMO_POINTER_OFFSET, -GIZMO_POINTER_HALF_THICKNESS), vec2(GIZMO_POINTER_THICKNESS), vec4(gizmo.intensity[1] * GIZMO_X_COLOR, 1));

			// y pointer box
			im2d_rect(vec2(-GIZMO_POINTER_HALF_THICKNESS, offset_y - GIZMO_LINE_LENGTH + GIZMO_POINTER_OFFSET), vec2(GIZMO_POINTER_THICKNESS), vec4(gizmo.intensity[2] * GIZMO_Y_COLOR, 1));

		} break;

		case Gizmo_Render_Type_ROTATE: {
			r32 _00 = t.m2[0][0];
			r32 _01 = t.m2[0][1];
			r32 _10 = t.m2[1][0];
			r32 _11 = t.m2[1][1];

			r32 scale_x = sqrtf(_00 * _00 + _10 * _10);
			r32 scale_y = sqrtf(_01 * _01 + _11 * _11);

			r32 c = _00 / scale_x;
			r32 s = _10 / scale_x;

			if (c == 0 && s == 0) {
				c = 1;
			}

			r32 angle_a = atanf(s / c) + ((c < 0) ? MATH_PI : 0);
			while (angle_a < 0) angle_a += 2 * MATH_PI;
			r32 angle_b = angle_a + 0.6f * MATH_TAU;
			while (angle_b >= 2 * MATH_PI) angle_b -= 2 * MATH_PI;

			r32 intensity = gizmo.intensity[3];
			im2d_pie_part(vec2(0), GIZMO_ROTOR_MIN_RADIUS, GIZMO_ROTOR_MAX_RADIUS, 0, 2 * MATH_PI, vec4(GIZMO_ROTOR_COLOR, 1));
			im2d_pie_part(vec2(0), GIZMO_ROTOR_MIN_RADIUS, GIZMO_ROTOR_MAX_RADIUS, angle_a, angle_b, vec4(intensity * GIZMO_ROTOR_INDICATOR_COLOR, 1));
		} break;
	}
}

void editor_render(Scene *scene, Editor *editor, r32 aspect_ratio) {
	if (editor->mode == Editor_Mode_GAME) return;

	switch (editor->mode) {
		case Editor_Mode_GAME_DEVELOPER: {
			ieditor_gui_developer_editor(scene, editor);
		} break;

		case Editor_Mode_LEVEL_EDITOR: {
			if (editor->gizmo.render_type != Gizmo_Render_Type_NONE) {
				Camera *camera = editor_rendering_camera(scene, editor);
				auto selected_body = editor->level.selected_body;

				if (selected_body) {
					Vec2 p = selected_body->transform.p - camera->position;
					r32 scale = camera_distance_to_scale(camera);
					r32 iscale = 1.0f / scale;

					Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(p, 0)) * mat4_scalar(iscale, iscale, 1.0f);

					r32 view_height = SCENE_VIEW_HEIGHT_HALF;
					r32 view_width = aspect_ratio * view_height;

					Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height);

					im2d_begin(view, transform);
					ieditor_gizmo_render(editor->gizmo, selected_body->transform.xform);
					im2d_end();
				}
			}

			ieditor_gui_developer_editor(scene, editor);
		} break;

		case Editor_Mode_ENTITY_EDITOR: {
			r32 view_height = SCENE_VIEW_HEIGHT_HALF;
			r32 view_width = aspect_ratio * view_height;

			Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height);

			Camera *camera = editor_rendering_camera(scene, editor);
			r32 scale = camera_distance_to_scale(camera);
			r32 iscale = 1.0f / scale;

			Mat4 transform = mat4_translation(vec3(-camera->position, 0)) * mat4_scalar(scale, scale, 1.0f);

			im2d_begin(view, transform);

			if (editor->entity.texture.view.id) {
				im2d_bind_texture(editor->entity.texture);
				im2d_rect_centered(vec2(0), vec2(1), editor->entity.texture_uv, vec4(1));
				im2d_unbind_texture();
			}

			r32 thickness = 0.003f;

			im2d_set_stroke_weight(thickness);

			auto shade = vec4(1.0f, 0.0f, 0.3f, 0.2f);
			auto outline = vec4(1);

			int count = editor->entity.fixture_count;
			auto fixtures = editor->entity.fixtures;
			int selected = editor->entity.selected_index;

			for (int index = 0; index < count; ++index) {
				auto f = fixtures + index;

				switch (f->shape) {
					case Fixture_Shape_Circle: {
						auto shape = fixture_get_shape(f, Circle);
						scene_render_shape(*shape, shade, outline);
						scene_render_shape(*shape, shade, outline);
					} break;

					case Fixture_Shape_Mm_Rect: {
						auto shape = fixture_get_shape(f, Mm_Rect);
						scene_render_shape(*shape, shade, outline);
					} break;

					case Fixture_Shape_Capsule: {
						auto shape = fixture_get_shape(f, Capsule);
						scene_render_shape(*shape, shade, outline);
					} break;

					case Fixture_Shape_Polygon: {
						auto shape = fixture_get_shape(f, Polygon_Pt);
						Polygon_Pt polygon;
						polygon.vertex_count = shape->vertex_count + (int)(index == selected && editor->entity.added_vertex_index != -1);
						polygon.vertices = shape->vertices;

						auto color = shade;
						if (index == selected && (editor->entity.added_vertex_index != -1) && !editor->entity.vertex_is_valid)
							color.w = 0;

						scene_render_shape(polygon, color, outline);
					} break;

						invalid_default_case();
				}
			}

			if (selected >= 0) {
				Fixture *f = editor->entity.fixtures + selected;

				switch (f->shape) {
					case Fixture_Shape_Circle: {
						auto shape = fixture_get_shape(f, Circle);
						im2d_circle(shape->center, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
					} break;

					case Fixture_Shape_Mm_Rect: {
						auto shape = fixture_get_shape(f, Mm_Rect);
						im2d_circle(shape->min, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
						im2d_circle(shape->max, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
					} break;

					case Fixture_Shape_Capsule: {
						auto shape = fixture_get_shape(f, Capsule);
						im2d_circle(shape->a, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
						im2d_circle(shape->b, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
					} break;

					case Fixture_Shape_Polygon: {
						auto shape = fixture_get_shape(f, Polygon_Pt);
						u32 count = shape->vertex_count + (int)(editor->entity.added_vertex_index != -1);
						auto v = shape->vertices;

						for (u32 index = 0; index < count; ++index, ++v) {
							im2d_circle(*v, EDITOR_VERTEX_RADIUS, EDITOR_VERTEX_COLOR);
						}

						if (editor->entity.added_vertex_index >= 0) {
							const auto color = editor->entity.vertex_is_valid ? EDITOR_SELECTED_VERTEX_COLOR : EDITOR_INVALID_SELECTED_VERTEX_COLOR;
							Vec2 p = shape->vertices[editor->entity.added_vertex_index];
							r32 a, b, c, d;
							a = editor->entity.vertex_pointer_angle;
							b = a + 0.5f * MATH_TAU;
							while (b >= 2 * MATH_PI) b -= 2 * MATH_PI;
							c = a + MATH_PI;
							while (c >= 2 * MATH_PI) c -= 2 * MATH_PI;
							d = c + 0.5f * MATH_TAU;
							while (d >= 2 * MATH_PI) d -= 2 * MATH_PI;
							im2d_pie_part(p, EDITOR_VERTEX_SELECTOR_RADIUS, EDITOR_VERTEX_SELECTOR_RADIUS + EDITOR_VERTEX_SELECTOR_THICKNESS, a, b, color);
							im2d_pie_part(p, EDITOR_VERTEX_SELECTOR_RADIUS, EDITOR_VERTEX_SELECTOR_RADIUS + EDITOR_VERTEX_SELECTOR_THICKNESS, c, d, color);
						}
					} break;
				}
			}

			if (editor->entity.hovered_vertex) {
				im2d_circle(*editor->entity.hovered_vertex, EDITOR_VERTEX_RADIUS, EDITOR_SELECTED_VERTEX_COLOR);
			}

			// Center
			im2d_line(vec2(0.0f, -0.1f), vec2(0.0f, 0.1f), vec4(1));
			im2d_line(vec2(-0.1f, 0.0f), vec2(0.1f, 0.0f), vec4(1));

			im2d_end();

			if (editor->entity.selected_vertex) {
				transform =
					mat4_translation(vec3(-camera->position, 0)) *
					mat4_scalar(scale, scale, 1.0f) *
					mat4_translation(vec3(*editor->entity.selected_vertex, 0)) *
					mat4_scalar(iscale, iscale, 1.0f);

				im2d_begin(view, transform);

				ieditor_gizmo_render(editor->gizmo, mat2_identity());

				im2d_end();
			}


			ieditor_gui_entity_editor(scene, editor);
		} break;
	}

}
