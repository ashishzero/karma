#include "modules/core/reflection.h"
#include "modules/core/karma.h"
#include "modules/core/systems.h"
#include "modules/core/reflection.h"
#include "modules/core/lin_maths.h"
#include "modules/core/utility.h"
#include "modules/imgui/imconfig.h"
#include "modules/imgui/imgui.h"
#include "modules/imgui/dev.h"
#include "modules/gfx/renderer.h"
#include "modules/core/thread_pool.h"

#include "scene.h"
#include "editor.h"

#include ".generated/entity.typeinfo"

struct Player_Controller {
	r32 x, y;
};

typedef bool(*Collision_Resolver)(Collider &a, Collider &b, Vec2 dp, Vec2 *normal, r32 *penetration);
static Collision_Resolver COLLISION_RESOLVERS[Collider_Count][Collider_Count];

bool null_collision_resolver(Collider &a, Collider &b, Vec2 dp, Vec2 *normal, r32 *penetration) {
	return false;
}

template <typename ShapeA, typename ShapeB>
bool shapes_collision_resolver(Collider &a, Collider &b, Vec2 dp, Vec2 *normal, r32 *penetration) {
	return epa_dynamic(*(ShapeA *)a.handle, *(ShapeB *)b.handle, dp, normal, penetration);
}

void collision_resover_init() {
	COLLISION_RESOLVERS[Collider_Null][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Circle] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Mm_Rect] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Capsule] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Null][Collider_Polygon] = null_collision_resolver;

	COLLISION_RESOLVERS[Collider_Circle][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Circle] = shapes_collision_resolver<Circle, Circle>;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Mm_Rect] = shapes_collision_resolver<Circle, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Capsule] = shapes_collision_resolver<Circle, Capsule>;
	COLLISION_RESOLVERS[Collider_Circle][Collider_Polygon] = shapes_collision_resolver<Circle, Polygon>;

	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Circle] = shapes_collision_resolver<Mm_Rect, Circle>;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Mm_Rect] = shapes_collision_resolver<Mm_Rect, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Capsule] = shapes_collision_resolver<Mm_Rect, Capsule>;
	COLLISION_RESOLVERS[Collider_Mm_Rect][Collider_Polygon] = shapes_collision_resolver<Mm_Rect, Polygon>;

	COLLISION_RESOLVERS[Collider_Capsule][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Circle] = shapes_collision_resolver<Capsule, Circle>;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Mm_Rect] = shapes_collision_resolver<Capsule, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Capsule] = shapes_collision_resolver<Capsule, Capsule>;
	COLLISION_RESOLVERS[Collider_Capsule][Collider_Polygon] = shapes_collision_resolver<Capsule, Polygon>;

	COLLISION_RESOLVERS[Collider_Polygon][Collider_Null] = null_collision_resolver;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Circle] = shapes_collision_resolver<Polygon, Circle>;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Mm_Rect] = shapes_collision_resolver<Polygon, Mm_Rect>;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Capsule] = shapes_collision_resolver<Polygon, Capsule>;
	COLLISION_RESOLVERS[Collider_Polygon][Collider_Polygon] = shapes_collision_resolver<Polygon, Polygon>;
}

bool collider_vs_collider_dynamic(Collider &a, Collider &b, Vec2 dp, Vec2 *normal, r32 *penetration) {
	return COLLISION_RESOLVERS[a.type][b.type](a, b, dp, normal, penetration);
}

int karma_user_zero() {

	collision_resover_init();

#ifdef INIT_THREAD_POOL
	if (!async_initialize(2, mega_bytes(32), context.allocator)) {
		system_fatal_error("Thread could not be created");
	}
#endif

	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);

	ImGui_Initialize();
	Dev_ModeEnable();

	Dev_SetPresentationState(false);

	bool running = true;

	r32 aspect_ratio = framebuffer_w / framebuffer_h;
	const r32 speed_factor = 1;

	r32 const fixed_dt = 1.0f / 60.0f;
	r32       dt = fixed_dt * speed_factor;
	r32       game_dt = fixed_dt * speed_factor;
	r32       real_dt = fixed_dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = fixed_dt;

	r32 window_w = 0, window_h = 0;

	Scene *scene= scene_create();

	Player *player = scene_add_player(scene);
	scene_generate_new_entity(scene, player, vec2(0));
	player->id = 6888115871281690331; // HACK: hardcoded
	player->transformed_collider = scene_create_colliders(scene, 1);
	scene_attach_collider(scene, collider_node(player->transformed_collider.handle, 0), Circle, 0);

	{
		Vec2 points[] = {
			vec2(-2.4f, 4.6f), vec2(3.6f, 4.6f), vec2(4.6f, -1.4f), vec2(1.6f, -5.4f), vec2(-7.4f, -2.4f)
		};

		assert(static_count(points) >= 3);

		Static_Body *object;

		object = scene_add_static_body(scene);
		scene_generate_new_entity(scene, object, vec2(-5.6f, 0.4f));
		object->id = 6888174093586976255; // HACK: hardcoded
		object->color = vec4(0, 1, 1);
		object->colliders = scene_create_colliders(scene, 1);

		Collider_Attachment attachment;
		attachment.polygon_n = static_count(points);

		Polygon *polygon = scene_attach_collider(scene, collider_node(object->colliders.handle, 0), Polygon, &attachment);
		polygon->vertex_count = static_count(points);
		memcpy(polygon->vertices, points, sizeof(points));
		collider_transform(polygon, mat3_translation(object->position));

		object = scene_add_static_body(scene);
		scene_generate_new_entity(scene, object, vec2(5));
		object->color = vec4(0, 1, 1);
		object->colliders = scene_create_colliders(scene, 1);

		Circle *circle = scene_attach_collider(scene, collider_node(object->colliders.handle, 0), Circle, 0);
		circle->center = vec2(0);
		circle->radius = 1.23f;
		collider_transform(circle, mat3_translation(object->position));

		object = scene_add_static_body(scene);
		scene_generate_new_entity(scene, object, vec2(6.5f, -0.5f));
		object->color = vec4(0, 1, 1);
		object->colliders = scene_create_colliders(scene, 1);

		Mm_Rect *rect = scene_attach_collider(scene, collider_node(object->colliders.handle, 0), Mm_Rect, 0);
		rect->min = vec2(-2.5f, -3.5f);
		rect->max = vec2(2.5f, 3.5f);
		collider_transform(rect, mat3_translation(object->position));

		object = scene_add_static_body(scene);
		scene_generate_new_entity(scene, object, vec2(-1, -5));
		object->color = vec4(0, 1, 1);
		object->colliders = scene_create_colliders(scene, 2);

		Capsule *capsule = scene_attach_collider(scene, collider_node(object->colliders.handle, 0), Capsule, 0);
		capsule->a = vec2(-2, -3);
		capsule->b = vec2(2, 3);
		capsule->radius = 1;
		collider_transform(capsule, mat3_translation(object->position));

		circle = scene_attach_collider(scene, collider_node(object->colliders.handle, 1), Circle, 0);
		circle->center = vec2(1, -1);
		circle->radius = 1;
		collider_transform(circle, mat3_translation(object->position));
	}

	Player_Controller controller = {};

	Camera camera;
	camera.position = vec2(0);
	camera.distance = 0.0f;

	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();

	while (running) {
		Dev_TimedFrameBegin();

		Dev_TimedBlockBegin(EventHandling);
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];

			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}

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

			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F1) {
				Dev_TogglePresentationState();
				continue;
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

			if (event.type & Event_Type_CONTROLLER_AXIS) {
				if (event.controller_axis.symbol == Controller_Axis_LTHUMB_X)
					controller.x = event.controller_axis.value;
				else if (event.controller_axis.symbol == Controller_Axis_LTHUMB_Y)
					controller.y = event.controller_axis.value;
			}

		}

		player = &scene->by_type.player[0]; // Only one player exists for now

		Dev_TimedBlockEnd(EventHandling);

		Dev_TimedBlockBegin(Simulation);

		struct Collision_Manifold {
			Vec2 normal;
			r32 penetration_depth;
		};

		Array<Collision_Manifold> manifolds;
		manifolds.allocator = TEMPORARY_ALLOCATOR;

		Vec2 player_force;
		while (accumulator_t >= fixed_dt) {
			Dev_TimedScope(SimulationFrame);

			const r32 gravity = 10;
			const r32 drag = 5;

			player_force = vec2(0);

			r32 len = sqrtf(controller.x * controller.x + controller.y * controller.y);
			Vec2 dir;
			if (len) {
				dir.x = controller.x / len;
				dir.y = controller.y / len;
			}
			else {
				dir = vec2(0);
			}

			const float force = 20;

			player_force = force * dir;
			//player->force.y -= gravity;

			player->velocity += dt * player_force;
			player->velocity *= powf(0.5f, drag * dt);

			//Vec2 velocity_t = dt * player_velocity;

			Vec2 norm; r32 dist, v_t;

			auto circle = collider_get_shape(collider_get(scene, collider_node(player->transformed_collider.handle, 0)), Circle);
			circle->center = player->position + player->collider.center;
			circle->radius = player->collider.radius;

			for (auto ptr = scene->colliders.node.next; ptr != &scene->colliders.node; ptr = ptr->next)
				ptr->collider.flags = 0;

			auto &player_collider = *collider_get(scene, collider_node(player->transformed_collider.handle, 0));
			player_collider.flags = 0;
			for (auto &o : scene->by_type.static_body) {
				for (u32 index = 0; index < o.colliders.count; ++index) {
					auto c = collider_get(scene, collider_node(o.colliders.handle, index));
					if (collider_vs_collider_dynamic(*c, player_collider, dt * player->velocity, &norm, &dist)) {
						array_add(&manifolds, Collision_Manifold{ norm, dist });
						v_t = dist / dt * sgn(vec2_dot(norm, player->velocity));
						player->velocity -= v_t * norm;
						o.color = vec4(0, 1, 1, 1);
						c->flags |= Collider_Flag_Bit_TOUCHED;
						player_collider.flags |= Collider_Flag_Bit_TOUCHED;
					}
				}
			}

			player->position += dt * player->velocity;

			r32 camera_follow_speed = 0.977f;
			camera.position = lerp(camera.position, player->position, 1.0f - powf(1.0f - camera_follow_speed, dt));

			accumulator_t -= fixed_dt;
		}

		ImGui_UpdateFrame(real_dt);

		r32 view_height = 5.0f;
		r32 view_width = aspect_ratio * view_height;

		auto view = orthographic_view(-view_width, view_width, view_height, -view_height);

#if 0 
		auto cursor = system_get_cursor_position();
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor = 2.0f * cursor - vec2(1);
		cursor.x *= view_width;
		cursor.y *= view_height;

		if (ImGui_IsUsingCursor()) {
			cursor.x = 0;
			cursor.y = 0;
		}
#endif

		Dev_TimedBlockEnd(Simulation);

		Dev_TimedBlockBegin(Rendering);

		r32 alpha = accumulator_t / fixed_dt; // TODO: Use this

		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.0f));
		gfx_viewport(0, 0, window_w, window_h);

		r32 scale = powf(0.5f, camera.distance);
		Mat4 transform = mat4_scalar(scale, scale, 1.0f) * mat4_translation(vec3(-camera.position, 0.0f));

		im2d_begin(view, transform);

		im2d_circle(player->position, player->rradius, player->color);
		im2d_line(player->position, player->position + player->velocity, vec4(0, 1.5f, 0), 0.02f);

		for (auto &manifold : manifolds) {
			im2d_line(player->position, player->position + manifold.normal, 1.5f * vec4(1, 0, 0), 0.05f);
			im2d_line(player->position, player->position + manifold.penetration_depth * manifold.normal, 2 * vec4(1, 1, 0), 0.05f);
		}

		for (auto ptr = scene->colliders.node.next; ptr != &scene->colliders.node; ptr = ptr->next) {
			auto &c = ptr->collider;
			auto color = (c.flags & Collider_Flag_Bit_TOUCHED) ? vec4(0, 1, 1) : vec4(1, 0, 0);
			switch (c.type) {
			case Collider_Null: {

			} break;

			case Collider_Circle: {
				auto circle = collider_get_shape(&c, Circle);
				im2d_circle_outline(circle->center, circle->radius, color, 0.02f);
			} break;

			case Collider_Polygon: {
				auto polygon = collider_get_shape(&c, Polygon);
				for (u32 i = 0; i < polygon->vertex_count; ++i) {
					im2d_line(polygon->vertices[i], polygon->vertices[(i + 1) % polygon->vertex_count], color, 0.02f);
				}
			} break;

			case Collider_Mm_Rect: {
				auto rect = collider_get_shape(&c, Mm_Rect);
				im2d_rect_outline(rect->min, rect->max - rect->min, color, 0.02f);
			} break;

			case Collider_Capsule: {
				auto capsule = collider_get_shape(&c, Capsule);
				Vec2 capsule_dir = capsule->b - capsule->a;
				Vec2 capsule_norm = vec2_normalize(vec2(-capsule_dir.y, capsule_dir.x)) * capsule->radius;

				im2d_circle_outline(capsule->a, capsule->radius, color, 0.02f);
				im2d_circle_outline(capsule->b, capsule->radius, color, 0.02f);
				im2d_line(capsule->a + capsule_norm, capsule->b + capsule_norm, color, 0.02f);
				im2d_line(capsule->a - capsule_norm, capsule->b - capsule_norm, color, 0.02f);
			} break;

				invalid_default_case();

			}
		}

		im2d_end();


		gfx_end_drawing();


		gfx_apply_bloom(2);

		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);

#if defined(BUILD_DEVELOPER_SERVICE)
		{
			Dev_TimedScope(DebugRender);
			Dev_RenderFrame();
		}
#endif

		ImGui::Begin("Player");
		editor_draw(*player);

#if 1
		if (ImGui::Button("Save")) {
			Ostream out;
			serialize_entity(scene, player, &out);
			System_File file;
			if (system_open_file(tprintf("temp/%zu.entity", player->id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
			}
			system_close_file(&file);
			ostream_free(&out);
		}

		if (ImGui::Button("Load")) {
			scene_destroy_colliders(scene, &player->transformed_collider);

			Tokenization_Status status;
			auto content = system_read_entire_file(tprintf("temp/%zu.entity", player->id));
			auto tokens = tokenize(content, &status);
			if (status.result == Tokenization_Result_SUCCESS) {
				Deserialize_Error_Info error;
				auto state = deserialize_begin(tokens);

				if (!deserialize_entity(scene, player, &state)) {
					system_log(LOG_ERROR, "Load", "Failed to load player: %s [%zd:%zd]",
						state.error.string, state.error.token.row, state.error.token.column);
				}

				deserialize_end(&state);
			}
			else {
				system_log(LOG_ERROR, "Load", "Failed to load player");
			}
			memory_free(tokens.data);
			memory_free(content.data);
		}
#endif

		ImGui::End();

		ImGui::Begin("Static Body");
		editor_draw(scene->by_type.static_body[0]);

		if (ImGui::Button("Save Static Body")) {
			Ostream out;
			serialize_entity(scene, &scene->by_type.static_body[0], &out);
			System_File file;
			if (system_open_file(tprintf("temp/%zu.entity", scene->by_type.static_body[0].id), File_Operation_NEW, &file)) {
				ostream_build_out_file(&out, &file);
			}
			system_close_file(&file);
			ostream_free(&out);
		}

		if (ImGui::Button("Load Static Body")) {
			scene_destroy_colliders(scene, &scene->by_type.static_body[0].colliders);

			Tokenization_Status status;
			auto content = system_read_entire_file(tprintf("temp/%zu.entity", scene->by_type.static_body[0].id));
			auto tokens = tokenize(content, &status);
			if (status.result == Tokenization_Result_SUCCESS) {
				Deserialize_Error_Info error;
				auto state = deserialize_begin(tokens);

				if (!deserialize_entity(scene, &scene->by_type.static_body[0], &state)) {
					system_log(LOG_ERROR, "Load", "Failed to load static body: %s [%zd:%zd]",
						state.error.string, state.error.token.row, state.error.token.column);
				}

				deserialize_end(&state);
			}
			else {
				system_log(LOG_ERROR, "Load", "Failed to load static body");
			}
			memory_free(tokens.data);
			memory_free(content.data);
		}

		ImGui::End();

#if defined(BUILD_IMGUI)
		{
			Dev_TimedScope(ImGuiRender);
			ImGui_RenderFrame();
		}
#endif

		gfx_end_drawing();

		Dev_TimedBlockBegin(Present);
		gfx_present();
		Dev_TimedBlockEnd(Present);
		Dev_TimedBlockEnd(Rendering);

		reset_temporary_memory();

		auto new_counter = system_get_counter();
		auto counts = new_counter - counter;
		counter = new_counter;

		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;

		game_dt = real_dt * speed_factor;
		dt = fixed_dt * speed_factor;

		accumulator_t += real_dt;
		accumulator_t = minimum(accumulator_t, 0.3f);

		Dev_TimedFrameEnd(real_dt);
	}

	ImGui_Shutdown();
	gfx_destroy_context();

	return 0;
}
