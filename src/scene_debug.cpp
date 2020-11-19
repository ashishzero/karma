#include "scene.h"
#include "scene_debug.h"
#include "modules/imgui/imgui.h"
#include "modules/imgui/editor.h"
#include "modules/gfx/renderer.h"
#include "scene.h"

#include ".generated/entity.typeinfo"

const r32  EDITOR_VERTEX_RADIUS = 0.1f;
const r32  EDITOR_VERTEX_SELECTOR_RADIUS = 0.3f;
const r32  EDITOR_VERTEX_SELECTOR_THICKNESS = 0.1f;
const Vec4 EDITOR_VERTEX_COLOR = vec4(1, 1, 0);
const Vec4 EDITOR_SELECTED_VERTEX_COLOR = vec4(0, 3, 3);

const r32 GIZMO_LINE_THICKNESS = 0.1f;
const r32 GIZMO_LINE_HALF_THICKNESS = 0.5f * GIZMO_LINE_THICKNESS;
const r32 GIZMO_LINE_LENGTH = 1.3f;
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

	editor.camera.id = { 0 };
	editor.camera.type = Entity_Type_Camera;
	editor.camera.position = vec2(0);
	editor.camera.distance = 0;
	editor.camera.target_position = vec2(0);
	editor.camera.target_distance = 0;
	editor.camera.follow_factor = 0.99f;
	editor.camera.zoom_factor = 0.99f;
	editor.camera.behaviour = 0;

	editor_set_mode_map(scene, &editor);

	return editor;
}

bool editor_handle_event(const Event &event, Editor *editor) {
	auto mode = editor->mode;
	auto &gizmo = editor->gizmo;

	if ((event.type & Event_Type_KEY_UP) && gizmo.render_type != Gizmo_Render_Type_NONE) {
		if (mode == Editor_Mode_MAP) {
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
		} else if (mode == Editor_Mode_ENTITY) {

		}
	}

	return false;
}

void editor_set_mode_map(Scene *scene, Editor *editor) {
	scene_refresh_rigid_bodies(scene);
	editor->mode = Editor_Mode_MAP;
	editor->map.hovered_body = nullptr;
	editor->map.selected_body = nullptr;
	editor->map.select_camera_index = -1;
	editor->gizmo.render_type = Gizmo_Render_Type_NONE;
	editor->gizmo.type = Gizmo_Type_NONE;
}

void editor_set_mode_fixture(Scene *scene, Editor *editor) {
	editor->mode = Editor_Mode_ENTITY;
}

//
//
//

void ieditor_gizmo_action(Gizmo *gizmo, const Transform &gizmo_transform, Vec2 cursor, Vec2 delta) {
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
					if (test_shape_vs_point(GIZMO_CENTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[0] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_CENTER;
					} else if (test_shape_vs_point(GIZMO_X_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_X_POINTER_POLY, gizmo_transform, cursor)) {
						gizmo->intensity[1] = 4.5f;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_TRANSLATE_X;
					} else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_Y_POINTER_POLY, gizmo_transform, cursor)) {
						gizmo->intensity[2] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_TRANSLATE_Y;
					}

					if (gizmo->type != Gizmo_Type_NONE) {
						gizmo->out = vec2(0);
					}
				} break;

				case Gizmo_Render_Type_SCALE: {
					if (test_shape_vs_point(GIZMO_CENTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[0] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left])
							gizmo->type = Gizmo_Type_CENTER;
					} else if (test_shape_vs_point(GIZMO_X_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_X_POINTER_RECT, gizmo_transform, cursor)) {
						gizmo->intensity[1] = 4.5f;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_SCALE_X;
							gizmo->values[0] = 1;
							gizmo->values[1] = 1;
						}
					} else if (test_shape_vs_point(GIZMO_Y_LINE_RECT, gizmo_transform, cursor) ||
							   test_shape_vs_point(GIZMO_Y_POINTER_RECT, gizmo_transform, cursor)) {
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
					if (test_shape_vs_point(GIZMO_OUTER, gizmo_transform, cursor) &&
						!test_shape_vs_point(GIZMO_INNER, gizmo_transform, cursor)) {
						gizmo->intensity[3] = 2;
						if (io.MouseDown[ImGuiMouseButton_Left]) {
							gizmo->type = Gizmo_Type_ROTOR;
							gizmo->values[0] = cursor.x - gizmo_transform.p.x;
							gizmo->values[1] = cursor.y - gizmo_transform.p.y;
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
			Vec2 b = vec2_normalize_check(cursor - gizmo_transform.p);
			r32 angle = vec2_signed_angle_between(a, b);

			gizmo->out.x = angle;
			gizmo->values[0] = b.x;
			gizmo->values[1] = b.y;
		} break;
	}
}

void editor_update(Scene *scene, Editor *editor) {
	ImGui::GetStyle().Alpha = 1.0f;

	if (!ImGui_IsUsingCursor()) {
		ImGui::GetStyle().Alpha = 0.4f;

		Gizmo &gizmo = editor->gizmo;
		auto &io = ImGui::GetIO();

		switch (editor->mode) {
			case Editor_Mode_MAP: {
				Camera *camera = &editor->camera;

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

				// Get the Rigid_Body under the mouse cursor, nullptr if mouse doesn't hover over any Rigid_Body
				editor->map.hovered_body = scene_collide_point(scene, cursor);

				if (editor->map.selected_body) {
					Transform gizmo_transform;
					gizmo_transform.p = editor->map.selected_body->transform.p;
					gizmo_transform.xform = mat2_scalar(iscale, iscale);

					ieditor_gizmo_action(&editor->gizmo, gizmo_transform, cursor, delta);

					switch (gizmo.type) {
						case Gizmo_Type_TRANSLATE_X:
						case Gizmo_Type_TRANSLATE_Y:
						case Gizmo_Type_CENTER: {
							Entity *entity = scene_find_entity(scene, editor->map.selected_body->entity_id);
							entity->position += gizmo.out;
							editor->map.selected_body->transform.p = entity->position;
							scene_rigid_body_update_bounding_box(editor->map.selected_body, 0);
						} break;

						case Gizmo_Type_SCALE_X:
						case Gizmo_Type_SCALE_Y: {
							editor->map.selected_body->transform.xform = mat2_scalar(gizmo.out) * editor->map.selected_body->transform.xform;
							scene_rigid_body_update_bounding_box(editor->map.selected_body, 0);
						} break;

						case Gizmo_Type_ROTOR: {
							editor->map.selected_body->bounding_box = scene_rigid_body_bounding_box(editor->map.selected_body, 0);
							editor->map.selected_body->transform.xform = mat2_rotation(gizmo.out.x) * editor->map.selected_body->transform.xform;
							scene_rigid_body_update_bounding_box(editor->map.selected_body, 0);
						} break;
					}
				}

				if (gizmo.type == Gizmo_Type_NONE) {
					// If we are not using Gizmo and Press left button,
					// then hovered body is selected
					if (io.MouseClicked[ImGuiMouseButton_Left]) {
						editor->map.selected_body = editor->map.hovered_body;
						editor->map.select_camera_index = -1;
						if (gizmo.render_type == Gizmo_Render_Type_NONE)
							gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
					}

					// If we are not using Gizmo and Double click left button,
					// then hovered body is selected and focused
					if (editor->map.hovered_body && io.MouseDoubleClicked[ImGuiMouseButton_Left]) {
						editor->map.selected_body = editor->map.hovered_body;
						editor->map.select_camera_index = -1;
						if (gizmo.render_type == Gizmo_Render_Type_NONE)
							gizmo.render_type = Gizmo_Render_Type_TRANSLATE;

						// Focus on Rigid_Body
						camera->behaviour |= Camera_Behaviour_ANIMATE_FOCUS | Camera_Behaviour_ANIMATE_MOVEMENT;
						camera->target_position = editor->map.selected_body->transform.p;

						r32 sx = editor->map.selected_body->bounding_box.max.x - editor->map.selected_body->bounding_box.min.x;
						r32 sy = editor->map.selected_body->bounding_box.max.y - editor->map.selected_body->bounding_box.min.y;
						sx /= view_width;
						sy /= view_height;
						r32 s = maximum(sx, sy);

						camera->target_distance = log2f(s);
					}
				}

				if (editor->map.selected_body == nullptr) {
					gizmo.render_type = Gizmo_Render_Type_NONE;
				}

				if (editor->map.select_camera_index >= 0) {
					auto dst = scene_primary_camera(scene);
					dst->position = camera->position;
					dst->distance = camera->distance;
				}
			} break;

			case Editor_Mode_ENTITY: {
				#if 0
				if (editor->fixture.index >= 0) {
					Transform p_transform;
					p_transform.p = vec2(0);
					p_transform.xform = mat2_identity();

					Circle p;
					p.radius = EDITOR_VERTEX_RADIUS;

					Resource_Fixture &resource = scene->resource_fixtures[editor->fixture.index];
					u32 count = resource.fixture_count;
					auto &fixtures = resource.fixtures;

					editor->fixture.hovered_vertex_ptr = nullptr;

					editor->fixture.vertex_pointer_angle += io.DeltaTime * 5;
					while (editor->fixture.vertex_pointer_angle >= 2 * MATH_PI)
						editor->fixture.vertex_pointer_angle -= 2 * MATH_PI;

					for (u32 index = 0; index < count; ++index) {
						auto f = fixtures + index;
						switch (f->shape) {
							case Fixture_Shape_Circle: {
								auto shape = fixture_get_shape(f, Circle);
								p.center = shape->center;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor->fixture.hovered_vertex_ptr = &shape->center;
									editor->fixture.hovered_fixture_index = index;
								}
							} break;

							case Fixture_Shape_Mm_Rect: {
								auto shape = fixture_get_shape(f, Mm_Rect);
								p.center = shape->min;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor->fixture.hovered_vertex_ptr = &shape->min;
									editor->fixture.hovered_fixture_index = index;
								}

								p.center = shape->max;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor->fixture.hovered_vertex_ptr = &shape->max;
									editor->fixture.hovered_fixture_index = index;
								}
							} break;

							case Fixture_Shape_Capsule: {
								auto shape = fixture_get_shape(f, Capsule);
								p.center = shape->a;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor->fixture.hovered_vertex_ptr = &shape->a;
									editor->fixture.hovered_fixture_index = index;
								}

								p.center = shape->b;
								if (test_shape_vs_point(p, p_transform, cursor)) {
									editor->fixture.hovered_vertex_ptr = &shape->b;
									editor->fixture.hovered_fixture_index = index;
								}
							} break;

							case Fixture_Shape_Polygon: {
								auto shape = fixture_get_shape(f, Polygon);
								u32 vcount = shape->vertex_count;
								auto v = shape->vertices;
								for (u32 vi = 0; vi < vcount; ++vi, ++v) {
									p.center = *v;
									if (test_shape_vs_point(p, p_transform, cursor)) {
										editor->fixture.hovered_vertex_ptr = v;
										editor->fixture.hovered_fixture_index = index;
										break;
									}
								}
							} break;
						}
					}

					if (editor->fixture.selected_vertex_ptr) {
						Transform gizmo_transform;
						gizmo_transform.p = *editor->fixture.selected_vertex_ptr;
						gizmo_transform.xform = mat2_scalar(iscale, iscale);

						gizmo_action(&editor->gizmo, gizmo_transform, cursor, delta);

						switch (gizmo.type) {
							case Gizmo_Type_TRANSLATE_X:
							case Gizmo_Type_TRANSLATE_Y:
							case Gizmo_Type_CENTER: {
								editor->fixture.selected_vertex_ptr->x += gizmo.out.x;
								editor->fixture.selected_vertex_ptr->y += gizmo.out.y;

								// Reject if the polygon is not convex
								Fixture &fixture = fixtures[editor->fixture.selected_fixture_index];
								if (fixture.shape == Fixture_Shape_Polygon) {
									if (!is_polygon_convex(*fixture_get_shape(&fixture, Polygon))) {
										editor->fixture.selected_vertex_ptr->x = gizmo_transform.p.x;
										editor->fixture.selected_vertex_ptr->y = gizmo_transform.p.y;
									}
								}
							} break;

							case Gizmo_Type_SCALE_X:
							case Gizmo_Type_SCALE_Y: {
								Fixture &fixture = fixtures[editor->fixture.selected_fixture_index];
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
						}
					}

					if (gizmo.type == Gizmo_Type_NONE) {
						// If we are not using Gizmo and Press left button,
						// then hovered vertex is selected
						if (io.MouseClicked[ImGuiMouseButton_Left]) {
							editor->fixture.selected_vertex_ptr = editor->fixture.hovered_vertex_ptr;
							editor->fixture.selected_fixture_index = editor->fixture.hovered_fixture_index;

							Fixture &fixture = fixtures[editor->fixture.selected_fixture_index];

							if (gizmo.render_type == Gizmo_Render_Type_NONE) {
								gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
							} else if (editor->fixture.selected_vertex_ptr &&
									   (fixture.shape == Fixture_Shape_Mm_Rect || fixture.shape == Fixture_Shape_Polygon)) {
								gizmo.render_type = Gizmo_Render_Type_TRANSLATE;
							}
						}
					}

					if (editor->fixture.selected_vertex_ptr == nullptr) {
						gizmo.render_type = Gizmo_Render_Type_NONE;
					}

				}
				#endif
			} break;
		}

	}

	r32 dt = ImGui::GetIO().DeltaTime;
	Camera &camera = editor->camera;
	if (camera.behaviour & Camera_Behaviour_ANIMATE_MOVEMENT) {
		camera.position = lerp(camera.position, camera.target_position, 1.0f - powf(1.0f - camera.follow_factor, dt));
	}
	if (camera.behaviour & Camera_Behaviour_ANIMATE_FOCUS) {
		camera.distance = lerp(camera.distance, camera.target_distance, 1.0f - powf(1.0f - camera.zoom_factor, dt));
	}
}

#if 0
template <typename Shape>
bool ieditor_fixture_shape(Shape &s, const char *name) {
	bool result = false;
	ImGui::PushID((void *)&s);
	if (ImGui::CollapsingHeader(name)) {
		ImGui::Indent();
		result |= editor_widget<Shape>(s, name);
		ImGui::Unindent();
	}
	ImGui::PopID();
	return result;
}

bool ieditor_fixture(Fixture *fixture, u32 count) {
	bool result = false;

	for (u32 index = 0; index < count; ++index) {
		auto f = fixture + index;

		switch (f->shape) {
			case Fixture_Shape_Circle: {
				auto shape = fixture_get_shape(f, Circle);
				result |= ieditor_fixture_shape(*shape, "Circle");
			} break;

			case Fixture_Shape_Mm_Rect: {
				auto shape = fixture_get_shape(f, Mm_Rect);
				result |= ieditor_fixture_shape(*shape, "Mm_Rect");
			} break;

			case Fixture_Shape_Capsule: {
				auto shape = fixture_get_shape(f, Capsule);
				result |= ieditor_fixture_shape(*shape, "Capsule");
			} break;

			case Fixture_Shape_Polygon: {
				auto shape = fixture_get_shape(f, Polygon);
				u32 count = shape->vertex_count;
				Vec2 *vertices = shape->vertices;

				ImGui::PushID((void *)(vertices + index));

				char label[5];
				if (ImGui::CollapsingHeader("Vertices")) {
					for (u32 index = 0; index < count; ++index) {
						snprintf(label, sizeof(label), "%u", index);
						Vec2 v = vertices[index];
						if (ImGui::DragFloat2(label, vertices[index].m, 0.01f, 0.0f, 0.0f, "%.4f", 0)) {
							if (!is_polygon_convex(*shape)) {
								vertices[index] = v;
							}
							result = true;
						}
					}
				}

				ImGui::PopID();
			} break;

				invalid_default_case();
		}
	}

	return result;
}
#endif

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

bool ieditor_gui(Scene *scene, Editor *editor) {
	bool result = false;

	switch (editor->mode) {
		case Editor_Mode_MAP: {
			// Level
			Level *level = scene_current_level_pointer(scene);

			ImGui::Begin("Level");

			Level_Name level_name;
			u32 level_name_count = level->name_count;
			memcpy(level_name, level->name, sizeof(Level_Name));
			if (editor_widget<Level>(*level, "Level Editor")) {
				String old_path = tprintf("resources/levels/%s", level_name);
				String new_path = tprintf("resources/levels/%s", level->name);
				if (system_rename_directory(old_path, new_path)) {
					level->name_count = (u32)strlen(level->name);
				} else {
					// TODO: Failed to rename, log error somewhere
					memcpy(level->name, level_name, sizeof(Level_Name));
				}
			}

			if (ImGui::CollapsingHeader("Cameras")) {
				const Array_View<Camera> cameras = scene_cameras(scene);
				int count = (int)cameras.count;
				int selected = editor->map.select_camera_index;
				for (int index = 0; index < count; index++) {
					auto &c = cameras[index];
					ImGui::PushID((void *)&c);
					if (ImGui::Selectable(null_tprintf("%d Camera", index), selected == index)) {
						selected = (selected == index) ? -1 : index;
					}
					ImGui::PopID();
				}
				editor->map.select_camera_index = selected;
			}

			if (ImGui::Button("Save##Level")) {
				scene_save_level(scene);
			}

			ImGui::End();

			// Entity Properties
			ImGui::Begin("Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

			Entity *entity = nullptr;
			if (editor->map.selected_body) {
				entity = scene_find_entity(scene, editor->map.selected_body->entity_id);
			} else if (editor->map.select_camera_index >= 0) {
				entity = &scene_cameras(scene)[editor->map.select_camera_index];
			}

			if (entity == nullptr) {
				ImGui::LabelText("type", "Entity_Type_Null");
				ImGui::End();
				return false;
			}

			Rigid_Body *body = nullptr;

			switch (entity->type) {
				case Entity_Type_Camera: {
					result = editor_widget<Camera>(*(Camera *)entity, "Camera Entity");
					if (result) {
						Camera *src = (Camera *)entity;
						editor->camera.position = src->position;
						editor->camera.distance = src->distance;
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
			}

			ImGui::End();
		} break;

		case Editor_Mode_ENTITY: {
			#if 0
			bool open_fixture = true;

			ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Fixture Editor", &open_fixture)) {
				int selected = (int)editor->fixture.index;

				// Left
				{
					ImGui::BeginChild("Fixtures", ImVec2(200, 0), true);

					int count = (int)scene->resource_fixtures.count;
					auto &fixtures = scene->resource_fixtures;
					for (int index = 0; index < count; index++) {
						auto &f = fixtures[index];
						ImGui::PushID((void *)&f);
						if (ImGui::Selectable(f.name, selected == index)) {
							selected = index;
							editor->fixture.selected_vertex_ptr = nullptr;
						}
						ImGui::PopID();
					}

					ImGui::EndChild();
				}
				ImGui::SameLine();

				editor->fixture.index = selected;

				// Right
				{
					ImGui::BeginGroup();
					ImGui::BeginChild("Resource View");

					auto &resource = scene->resource_fixtures[selected];
					editor_widget<Resource_Fixture>(resource, "Resource");
					editor_fixture(resource.fixtures, resource.fixture_count);

					ImGui::EndChild();
					ImGui::EndGroup();
				}
			}
			ImGui::End();

			if (!open_fixture) {
				iscene_set_editor_mode_map(scene);
			}
			#endif
		} break;

			invalid_default_case();
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
	switch (editor->mode) {
		case Editor_Mode_MAP: {
			if (editor->gizmo.render_type != Gizmo_Render_Type_NONE) {
				Camera *camera = &editor->camera;
				auto selected_body = editor->map.selected_body;

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
		} break;

		case Editor_Mode_ENTITY: {

		} break;
	}

	ieditor_gui(scene, editor);
}
