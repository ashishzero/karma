#include "reflection.h"

//karma.h for array and string
#include "karma.h"
//thread  controller position,systems.h is for window, audio and everything platform related
#include "systems.h"
#include "reflection.h"
//related to image ellipse mesh cube
#include "gfx_renderer.h"
#include "lin_maths.h"
#include "imgui/imgui.h"
#include "imgui/imconfig.h"
#include "debug.h"
//utility.h contains sorting and hashing, searching and such
#include "utility.h"
#include "atish.h"
#include ".generated/atish.typeinfo"
//unsigned int32
typedef u32 Entity_Handle;

#include "entity.h"
#include ".generated/entity.typeinfo"

#define string_cstr(string) ((char *)((string).data))

constexpr u32 EDITOR_FLAG_NO_DISPLAY = bit(1);
constexpr u32 EDITOR_FLAG_READ_ONLY  = bit(3);
constexpr u32 EDITOR_FLAG_COLOR		 = bit(4);

u32 editor_get_flags_from_attribute(const String* attrs, s64 count) {
	u32 flags = 0;
	for (s64 index = 0; index < count; ++index) {
		if (string_match(attrs[index], "no-display")) {
			flags |= EDITOR_FLAG_NO_DISPLAY;
		} else if (string_match(attrs[index], "read-only")) {
			flags |= EDITOR_FLAG_READ_ONLY;
		} else if (string_match(attrs[index], "color")) {
			flags |= EDITOR_FLAG_COLOR;
		}
	}
	return flags;
}

void imgui_draw_entity(Entity *entity) {
	char *data = (char *)entity;

	auto info = (Type_Info_Struct *)reflect_info<Entity>();
	auto mem_counts = info->member_count;

	ImGui::Begin(string_cstr(info->name));

	u32 flags = 0;

	for (ptrsize index = 0; index < mem_counts; ++index) {
		auto mem = info->members + index;

		flags = editor_get_flags_from_attribute(mem->attributes, mem->attribute_count);
		if (flags & EDITOR_FLAG_NO_DISPLAY) continue;

		if (flags & EDITOR_FLAG_READ_ONLY) {
			if (mem->info == reflect_info<r32>()) {
				r32 val = *(r32 *)(data + mem->offset);
				ImGui::Text("%s : %.3f", string_cstr(mem->name), val);
			} else if (mem->info == reflect_info<Vec2>()) {
				Vec2 *val = (Vec2 *)(data + mem->offset);
				ImGui::Text("%s : (%.3f, %.3f)", string_cstr(mem->name), val->x, val->y);
			} else if (mem->info == reflect_info<Vec3>()) {
				Vec3 *val = (Vec3 *)(data + mem->offset);
				ImGui::Text("%s : (%.3f, %.3f, %.3f)", string_cstr(mem->name), val->x, val->y, val->z);
			} else if (mem->info == reflect_info<Vec4>()) {
				Vec4 *val = (Vec4 *)(data + mem->offset);
				ImGui::Text("%s : (%.3f, %.3f, %.3f, %.3f)", string_cstr(mem->name), val->x, val->y, val->z, val->w);
			} else if (mem->info == reflect_info<u32>()) {
				u32 *val = (u32 *)(data + mem->offset);
				ImGui::Text("%s : %u", string_cstr(mem->name), *val);
			} else if (mem->info == reflect_info<Entity::Type>()) {
				Entity::Type *val = (Entity::Type *)(data + mem->offset);
				ImGui::Text("%s : %s", string_cstr(mem->name), string_cstr(enum_string(*val)));
			}
		} else {
			if (mem->info == reflect_info<r32>()) {
				ImGui::DragFloat(string_cstr(mem->name), (r32 *)(data + mem->offset));
			} else if (mem->info == reflect_info<Vec2>()) {
				ImGui::DragFloat2(string_cstr(mem->name), (r32 *)(data + mem->offset));
			} else if (mem->info == reflect_info<Vec3>()) {
				if (flags & EDITOR_FLAG_COLOR) {
					ImGui::ColorEdit3(string_cstr(mem->name), (r32 *)(data + mem->offset));
				} else {
					ImGui::DragFloat3(string_cstr(mem->name), (r32 *)(data + mem->offset));
				}
			} else if (mem->info == reflect_info<Vec4>()) {
				if (flags & EDITOR_FLAG_COLOR) {
					ImGui::ColorEdit4(string_cstr(mem->name), (r32 *)(data + mem->offset));
				} else {
					ImGui::DragFloat4(string_cstr(mem->name), (r32 *)(data + mem->offset));
				}
			} else if (mem->info == reflect_info<u32>()) {
				ImGui::DragScalar(string_cstr(mem->name), ImGuiDataType_U32, data + mem->offset, 1.0f);
			} else if (mem->info == reflect_info<Entity::Type>()) {
				auto items = enum_string_array<Entity::Type>();
				ImGui::Combo(string_cstr(mem->name), (int *)(data + mem->offset), items.data, (int)items.count);
			}
		}
	}

	ImGui::End();
}

struct Entity_Manager {
	Array<Entity>	entities;
	u32				handle_counter;
};
//not getting properly
Entity* add_entity(Entity_Manager* manager, Entity::Type type) {
	auto entity = array_add(&manager->entities);
	entity->handle = manager->handle_counter;
	manager->handle_counter += 1;
	entity->type = type;
	return entity;
}

Entity* manager_find_entity(Entity_Manager &manager, Entity_Handle handle) {
	for (auto& entity : manager.entities) {
		if (entity.handle == handle) return &entity;
	}
	return nullptr;
}

Entity_Manager make_manager() {
	Entity_Manager manager = {};
	manager.handle_counter = 1;
	return manager;
}

struct Player_Controller {
	r32 x, y;
};

struct Sorted_Colliders {
	Entity_Handle handle;
	r32 distance;
};

void collision_point_vs_line(Array<Sorted_Colliders>& sorted_colliders, Entity_Manager& manager, Entity* player, Vec2& new_player_position, r32& dt)
{
	Ray_Hit hit;
	Array<r32> t_values;
	Entity_Handle handle_save = 0;
	for (auto& collider : sorted_colliders) {
		auto entity = manager_find_entity(manager, collider.handle);
		entity->color = vec4(1, 0, 0);
		if (ray_vs_line(player->position, new_player_position, entity->start, entity->end, &hit)) {
			r32 dir = vec2_dot(vec2_normalize_check(player->velocity), hit.normal);

			if (dir <= 0 && hit.t >= -0.001f && hit.t < 1.001f) {
				array_add(&t_values, hit.t);
				//array_add(&normals, hit.point);

				Vec2 reduc_vector = (1.0f - hit.t) * vec2_dot(player->velocity, hit.normal) * hit.normal;
				//array_add(&normals, -reduc_vector);

				player->velocity -= reduc_vector;
				new_player_position = player->position + dt * player->velocity;
				handle_save = entity->handle;

				entity->color = vec4(1, 0, 1);
				break;
			}
		}
	}

	for (auto& collider : sorted_colliders) {
		Entity* entity = manager_find_entity(manager, collider.handle);
		if (entity->handle != handle_save) {
			if (ray_vs_line(player->position, new_player_position, entity->start, entity->end, &hit)) {
				r32 dir = vec2_dot(vec2_normalize_check(player->velocity), hit.normal);
				if (dir <= 0 && hit.t >= -0.001f && hit.t < 1.001f) {
					Vec2 reduc_vector = (1.0f - hit.t) * player->velocity;
					player->velocity -= reduc_vector;
					new_player_position = player->position + dt * player->velocity;

					entity->color = vec4(1, 0, 1);
				}
			}
		}
	}
}

int system_main() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	ImGui_Initialize();
	Debug_ModeEnable();

	bool running = true;

	r32 aspect_ratio = framebuffer_w / framebuffer_h;
	const r32 speed_factor = 1;

	r32 const fixed_dt = 1.0f / 30.0f;
	r32       dt = fixed_dt * speed_factor;
	r32       game_dt = fixed_dt * speed_factor;
	r32       real_dt = fixed_dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = fixed_dt;

	r32 window_w = 0, window_h = 0;

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	Entity_Manager manager = make_manager();

	Entity *player = add_entity(&manager, Entity::PLAYER);
	player->position = vec2(-0.2f, 0);
	player->size = vec2(0.2f, 0.4f);
	player->color = vec4(1);
	player->velocity = vec2(0);
	Entity_Handle player_id = player->handle;

	Entity *line = nullptr;

	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(-4, -3);
	line->end = vec2(-4, 2);
	line->color = vec4(1, 0, 0, 1);

	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(2, 4);
	line->end = vec2(-4, 2);
	line->color = vec4(1, 0, 0, 1);

	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(5.1f, 4);
	line->end = vec2(2, 4);
	line->color = vec4(1, 0, 0, 1);

	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(4, 1);
	line->end = vec2(5, 4);
	line->color = vec4(1, 0, 0, 1);

	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(4, 4);
	line->end = vec2(-5, 0);
	line->color = vec4(1, 0, 0, 1);


	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(3.5f, -4.1f);
	line->end = vec2(3.5f, 4);
	line->color = vec4(1, 0, 0, 1);

	line = add_entity(&manager, Entity::LINE);
	line->start = vec2(-4, 3);
	line->end = vec2(3.5f, -4);
	line->color = vec4(1, 0, 0, 1);

	Player_Controller controller = {};

	Ray_Hit hit;
	memset(&hit, 0, sizeof(hit));

	Vec2 normal = vec2(0);
	r32 penetrate_t = 0;
	Array<Ray_Hit> ray_hits;


	while (running) {
		Debug_TimedFrameBegin();

		Debug_TimedBlockBegin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event& event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

			if (Debug_HandleEvent(event))
				continue;
			if (ImGui_HandleEvent(event))
				continue;

			if (event.type & Event_Type_WINDOW_RESIZE) {
				s32 w = event.window.dimension.x;
				s32 h = event.window.dimension.y;

				gfx_on_client_resize(w, h);
				window_w = (r32)w;
				window_h = (r32)h;

				if (window_w && window_h) {
					gfx_resize_framebuffer(w, h);
					aspect_ratio = window_w / window_h;
					framebuffer_w = window_w;
					framebuffer_h = window_h;
				}

				continue;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}

			if (event.type & Event_Type_KEYBOARD) {
				float value = (float)(event.key.state == Key_State_DOWN);
				switch (event.key.symbol) {
				case Key_D:
				case Key_RIGHT:
					controller.x = value;
					break;
				case Key_A:
				case Key_LEFT:
					controller.x = -value;
					break;

				case Key_W:
				case Key_UP:
					controller.y = value;
					break;
				case Key_S:
				case Key_DOWN:
					controller.y = -value;
					break;
				}
			}

		}

		Debug_TimedBlockEnd(EventHandling);

		Debug_TimedBlockBegin(Simulation);

#if 1
		//Array<Vec2> normals;
		Array<r32> t_values;
		//ray_hits.allocator = TEMPORARY_ALLOCATOR;
		//normals.allocator = TEMPORARY_ALLOCATOR;
		t_values.allocator = TEMPORARY_ALLOCATOR;

		player = manager_find_entity(manager, player_id);

		//Vec2 prev_velocity = player->velocity;
		//only physics code
		static int test_counter = 0;

		while (accumulator_t >= fixed_dt /*&& test_counter < 2*/) {
			Debug_TimedScope(SimulationFrame);

			memset(&hit, 0, sizeof(hit));
			normal = vec2(0);
			penetrate_t = 0;
			array_clear(&ray_hits);

			const r32 gravity = 10;
			const r32 drag = 5;

			player->force = vec2(0);

			r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
			Vec2 dir;
			if (len) {
				dir.x = controller.x / len;
				dir.y = controller.y / len;
			}
			else {
				dir = vec2(0);
			}

			const float force = 50;

			player->force = force * dir;
			//player->force.y -= gravity;

			player->velocity += dt * player->force;
			player->velocity *= powf(0.5f, drag * dt);

			auto new_player_velocity = player->velocity;
			auto new_player_position = player->position + dt * new_player_velocity;


			Array<Sorted_Colliders> sorted_colliders;
			sorted_colliders.allocator = TEMPORARY_ALLOCATOR;

			for (auto& entity : manager.entities) {
				if (entity.type == Entity::LINE) {
					auto collider = array_add(&sorted_colliders);
					r32 dx = entity.end.x - entity.start.x;
					r32 dy = entity.end.y - entity.start.x;
					r32 d = dx * dx + dy * dy;
					r32 n = dy * player->position.x - dx * player->position.y + entity.end.x * entity.start.y - entity.end.y * entity.start.x;
					n *= n;
					collider->handle = entity.handle;
					collider->distance = n / d;
				}
			}

			sort(sorted_colliders.data, sorted_colliders.count, [](Sorted_Colliders & a, Sorted_Colliders &b) {
				return a.distance > b.distance;
			});

			//collision_point_vs_line(sorted_colliders, manager, player, new_player_position, dt);
			Vec2 get_corner[4] = { {player->size.x / 2,player->size.y / 2},
				{-player->size.x / 2,-player->size.y / 2},
				{player->size.x / 2,-player->size.y / 2},
				{-player->size.x / 2,player->size.y / 2} };
			//collided handle
			Entity_Handle handle_save = 0;
			for (auto& collider : sorted_colliders) {
				auto entity = manager_find_entity(manager, collider.handle);
				entity->color = vec4(1, 0, 0);
				bool collision_found = false;
				for (Vec2 temp : get_corner)
				{
					if (ray_vs_line(player->position + temp, new_player_position + temp, entity->start, entity->end, &hit)) {
						r32 dir = vec2_dot(vec2_normalize_check(player->velocity), hit.normal);

						if (dir <= 0 && hit.t >= -0.001f && hit.t < 1.001f) {
							array_add(&t_values, hit.t);
							//array_add(&normals, hit.point);

							Vec2 reduc_vector = (1.0f - hit.t) * vec2_dot(player->velocity, hit.normal) * hit.normal;
							//array_add(&normals, -reduc_vector);

							player->velocity -= reduc_vector;
							new_player_position = player->position + dt * player->velocity;
							handle_save = entity->handle;

							entity->color = vec4(1, 0, 1);
							collision_found = true;
						}
					}
				}
				if (collision_found)
					break;
			}

			for (auto& collider : sorted_colliders) {
				Entity* entity = manager_find_entity(manager, collider.handle);
				if (entity->handle != handle_save) {
					for (Vec2 temp : get_corner)
					{
						if (ray_vs_line(player->position + temp, new_player_position + temp, entity->start, entity->end, &hit)) {
							r32 dir = vec2_dot(vec2_normalize_check(player->velocity), hit.normal);
							if (dir <= 0 && hit.t >= -0.001f && hit.t < 1.001f) {
								Vec2 reduc_vector = (1.0f - hit.t) * player->velocity;
								player->velocity -= reduc_vector;
								new_player_position = player->position + dt * player->velocity;

								entity->color = vec4(1, 0, 1);
							}
						}
					}
				}
			}

			player->position += dt * player->velocity;

			accumulator_t -= fixed_dt;
		}

#endif

#if 0
		Vec2 cursor = system_get_cursor_position();
		if (cursor.x < 0) cursor.x = 0;
		if (cursor.y < 0) cursor.y = 0;
		if (cursor.x > framebuffer_w) cursor.x = framebuffer_w;
		if (cursor.y > framebuffer_h) cursor.y = framebuffer_h;
		cursor.x /= framebuffer_w;
		cursor.y /= framebuffer_h;
		cursor.x *= 2;
		cursor.y *= 2;
		cursor.x -= 1;
		cursor.y -= 1;
#endif

		r32 view_height = 5.0f;
		r32 view_width = aspect_ratio * view_height;

		auto view = orthographic_view(-view_width, view_width, view_height, -view_height);

#if 0
		Mat4 view_inv = mat4_inverse(gfx_view_transform(view));
		cursor = (view_inv * vec4(cursor, 0, 0)).xy;
		cursor.x = roundf(cursor.x);
		cursor.y = roundf(cursor.y);

		position = lerp(position, cursor, 0.5f);
#endif

		Debug_TimedBlockEnd(Simulation);

		Debug_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		ImGui_UpdateFrame(real_dt);

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.0f));
		gfx_viewport(0, 0, window_w, window_h);

		im2d_begin(view);

		for (auto& entity : manager.entities) {
			// TODO: Render by type!!!
			switch (entity.type) {
			case Entity::PLAYER: {
				//im2d_circle(entity.position, entity.size.x, entity.color);
				im2d_rect_centered(entity.position, entity.size, entity.color);
				//im2d_circle(entity.position, 0.08f, vec4(0, 1, 0));
			} break;

			case Entity::LINE: {
				im2d_line(entity.start, entity.end, entity.color, 0.01f);
			} break;
			}
		}



		//for (auto& h : rayhits) {
		//	im2d_circle(h.point, 0.08f, vec4(0, 0, 1));
		//	im2d_line(h.point, h.point + h.normal, vec4(1), 0.01f);
		//}

#if 1
		//for (s64 index = 0; index < normals.count; index += 2) {
		//	im2d_circle(normals[index], 0.08f, vec4(1, 0, 0));
		//	im2d_line(normals[index], normals[index] + normals[index + 1], vec4(1, 1, 1), 0.01f);
		//}

		for (auto & hit : ray_hits) {
			im2d_line(player->position, player->position + (1.0f - hit.t) * hit.normal, vec4(0, 1, 1), 0.01f);
		}

		im2d_line(player->position, player->position + normal, vec4(1), 0.01f);
		im2d_line(player->position, player->position + player->velocity, vec4(0, 1, 0), 0.03f);
		//im2d_line(player->position, player->position + prev_velocity, vec4(1, 1, 0), 0.03f);
#endif

		im2d_end();

		gfx_end_drawing();

		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

		#if defined(BUILD_IMGUI)

		imgui_draw_entity(player);

		#endif

#if defined(BUILD_IMGUI)
		{
			Debug_TimedScope(ImGuiRender);
			ImGui_RenderFrame();
		}
#endif

#if defined(BUILD_DEBUG_SERVICE)
		{
			Debug_TimedScope(DebugRender);
			Debug_RenderFrame(window_w, window_h);
		}
#endif

		gfx_end_drawing();

		Debug_TimedBlockBegin(Present);
		gfx_present();
		Debug_TimedBlockEnd(Present);
		Debug_TimedBlockEnd(Rendering);

		reset_temporary_memory();

		auto new_counter = system_get_counter();
		auto counts = new_counter - counter;
		counter = new_counter;

		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;

		game_dt = real_dt * speed_factor;
		dt = fixed_dt * speed_factor;

		accumulator_t += real_dt;
		accumulator_t = GetMinValue(accumulator_t, 0.3f);

		Debug_TimedFrameEnd(real_dt);
	}

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}

