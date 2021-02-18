#include "modules/core/karma.h"
#include "modules/core/lin_maths.h"
#include "modules/core/systems.h"
#include "modules/imgui/imconfig.h"
#include "modules/core/random.h"
#include "modules/gfx/renderer.h"
#include "modules/imgui/imgui.h"
#include "modules/core/stb_image.h"
#include "modules/core/stream.h"
#include "modules/core/lin_maths.h"

enum Tile_Type {
	TILE_TYPE_EMPTY,
	TILE_TYPE_BLOCK,
	TILE_TYPE_WIN,
	
	TILE_TYPE_COUNT
};

enum Action {
	ACTION_EAST,
	ACTION_WEST,
	ACTION_NORTH,
	ACTION_SOUTH,
	
	ACTION_COUNT
};

constexpr u32 WORLD_SIZE_X = 9;
constexpr u32 WORLD_SIZE_Y = 9;
constexpr u32 WORLD_SIZE = WORLD_SIZE_X * WORLD_SIZE_Y;
constexpr u32 QTABLE_SIZE = WORLD_SIZE * ACTION_COUNT;

struct Game {
	Tile_Type tiles[WORLD_SIZE];
	int visited[WORLD_SIZE];
	int p_x, p_y;
	int win_x, win_y;
	bool goal_reached;
};

struct Qtable {
	float scores[QTABLE_SIZE];
	int generation[WORLD_SIZE];
};

void qtable_init(Qtable *table) {
	for (int i = 0; i < QTABLE_SIZE; ++i) {
		table->scores[i] = 0;
	}
	for (int i = 0; i < WORLD_SIZE; ++i) {
		table->generation[i] = 0;
	}
}

int hash_position(int x, int y) {
	int index = (y * WORLD_SIZE_X + x) * ACTION_COUNT;
	return index;
}

Action get_action(Qtable *table, float min_epsilon, float max_epsilon, float depsilon, int x, int y, int *out_index) {
	float p = random_get_zero_to_one();
	
	int index = hash_position(x, y);
	auto action_scores = &table->scores[index];
	
	float generation = (float)table->generation[y * WORLD_SIZE_X + x];
	table->generation[y * WORLD_SIZE_X + x] += 1;
	
	r32 epsilon = min_epsilon + (max_epsilon - min_epsilon) * expf(-depsilon * generation);
	
	if (p >= epsilon) {
		int high_score_index = 0;
		float high_score = action_scores[0];
		
		Action equals[ACTION_COUNT];
		equals[0] = (Action)0;
		u32 equal_count = 1;
		
		for (int i = 1; i < ACTION_COUNT; ++i) {
			if (action_scores[i] > high_score) {
				high_score_index = i;
				high_score = action_scores[i];
				equals[0] = (Action)high_score_index;
				equal_count = 1;
			} else if (action_scores[i] == high_score) {
				equals[equal_count++] = (Action)i;
			}
		}
		
		if (equal_count > 1) {
			u32 rand_index = (random_get() % equal_count);
			high_score_index = equals[rand_index];
		}
		
		assert(high_score_index >= 0);
		
		*out_index = index + high_score_index;
		
		return (Action)high_score_index;
	}
	
	int n = (int)lroundf(p * (ACTION_COUNT - 1));
	assert(n >= 0 && n < ACTION_COUNT);
	
	*out_index = index + n;
	
	return (Action)n;
}

void game_tile_put_rock(Game *game, int x, int y) {
	if (x >= 0 && x < WORLD_SIZE_X && y >= 0 && y < WORLD_SIZE_Y) {
		int index = y * WORLD_SIZE_X + x;
		game->tiles[index] = TILE_TYPE_BLOCK;
	}
}

void game_init(Game *game) {
	memset(game->tiles, 0, sizeof(game->tiles));
	memset(game->visited, 0, sizeof(game->visited));
	
	game->tiles[WORLD_SIZE - 1] = TILE_TYPE_WIN;
	game->win_x = WORLD_SIZE_X - 1;
	game->win_y = WORLD_SIZE_Y - 1;
	
	game->p_x = 0;
	game->p_y = 0;
	game->visited[0] = 1;
	
	game->goal_reached = false;
	
	game_tile_put_rock(game, 4, 0);
	game_tile_put_rock(game, 6, 1);
	game_tile_put_rock(game, 6, 1);
	game_tile_put_rock(game, 6, 2);
	game_tile_put_rock(game, 7, 2);
	game_tile_put_rock(game, 8, 2);
	game_tile_put_rock(game, 2, 2);
	game_tile_put_rock(game, 3, 3);
	game_tile_put_rock(game, 3, 3);
	game_tile_put_rock(game, 4, 3);
	game_tile_put_rock(game, 4, 3);
	game_tile_put_rock(game, 0, 4);
	game_tile_put_rock(game, 4, 4);
	game_tile_put_rock(game, 5, 4);
	game_tile_put_rock(game, 8, 4);
	game_tile_put_rock(game, 2, 6);
	game_tile_put_rock(game, 3, 6);
	game_tile_put_rock(game, 3, 7);
	game_tile_put_rock(game, 3, 8);
	game_tile_put_rock(game, 4, 7);
	game_tile_put_rock(game, 8, 7);
	game_tile_put_rock(game, 7, 6);
	game_tile_put_rock(game, 6, 6);
	game_tile_put_rock(game, 4, 7);
}

void game_restart(Game *game) {
	memset(game->visited, 0, sizeof(game->visited));
	game->p_x = 0;
	game->p_y = 0;
	game->visited[0] = 1;
	game->goal_reached = false;
}

void move_player(Game *game, int x, int y) {
	if (x >= 0 && x < WORLD_SIZE_X && y >= 0 && y < WORLD_SIZE_Y) {
		int index = y * WORLD_SIZE_X + x;
		game->visited[index] = 1;
		game->p_x = x;
		game->p_y = y;
	}
}

Tile_Type get_tile_print(Game *game, int x, int y) {
	if (x == game->p_x && y == game->p_y) {
		return TILE_TYPE_COUNT;
	}
	
	int index = y * WORLD_SIZE_X + x;
	assert(index < WORLD_SIZE_X *WORLD_SIZE_Y);
	return game->tiles[index];
}

Tile_Type get_tile(Game *game, int x, int y) {
	assert(x >= 0 && x < WORLD_SIZE_X &&y >= 0 && y < WORLD_SIZE_Y);
	int index = y * WORLD_SIZE_X + x;
	return game->tiles[index];
}

void set_tile(Game *game, int x, int y, Tile_Type tile) {
	assert(x >= 0 && x < WORLD_SIZE_X &&y >= 0 && y < WORLD_SIZE_Y);
	int index = y * WORLD_SIZE_X + x;
	game->tiles[index] = tile;
}

float get_reward(Game *game, int x, int y) {
	if (x >= 0 && x < WORLD_SIZE_X && y >= 0 && y < WORLD_SIZE_Y) {
		int index = y * WORLD_SIZE_X + x;
		
		Tile_Type tile = game->tiles[index];
		
		switch (tile) {
			case TILE_TYPE_BLOCK: {
				return -1;
			} break;
			
			case TILE_TYPE_EMPTY: {
				if (game->visited[index]) return -0.5f;
				return 0;
			} break;
			
			case TILE_TYPE_WIN: {
				return 1;
			} break;
			
			invalid_default_case();
		}
	}
	
	return -1;
}

float get_max_optimal_future_reward_estimate(Qtable *table, int x, int y) {
	if (x >= 0 && x < WORLD_SIZE_X && y >= 0 && y < WORLD_SIZE_Y) {
		int index = hash_position(x, y);
		auto action_scores = &table->scores[index];
		
		int high_score_index = 0;
		float high_score = action_scores[0];
		for (int i = 1; i < ACTION_COUNT; ++i) {
			if (action_scores[i] > high_score) {
				high_score_index = i;
				high_score = action_scores[i];
			}
		}
		
		return high_score;
	}
	
	return -1;
}

bool game_is_over(Game *game) {
	if (game->p_x >= 0 && game->p_x < WORLD_SIZE_X && game->p_y >= 0 && game->p_y < WORLD_SIZE_Y) {
		auto tile = get_tile(game, game->p_x, game->p_y);
		game->goal_reached = (tile == TILE_TYPE_WIN);
		return tile == TILE_TYPE_WIN || tile == TILE_TYPE_BLOCK;
	}
	return true;
}

bool game_win(Game *game) {
	if (game->p_x >= 0 && game->p_x < WORLD_SIZE_X && game->p_y >= 0 && game->p_y < WORLD_SIZE_Y) {
		auto tile = get_tile(game, game->p_x, game->p_y);
		return tile == TILE_TYPE_WIN;
	}
	return false;
}

static const s32 SIMULATION_SPEED_FACTORS[] = {
	-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 30, 35, 40, 45, 50
};
static const u32 SIMULATION_SPEED_1X = 11;

struct Simulation_Speed {
	u32 index;
	s32 x;
	r32 factor;
};

static Simulation_Speed simulation_speed(u32 index) {
	assert(index < static_count(SIMULATION_SPEED_FACTORS));
	Simulation_Speed speed;
	speed.index = index;
	speed.x = SIMULATION_SPEED_FACTORS[speed.index];
	if (speed.x < 0)
		speed.factor = -1.0f / (r32)speed.x;
	else
		speed.factor = (r32)speed.x;
	return speed;
}

enum Actor_State {
	ACTOR_STATE_MOVING,
	ACTOR_STATE_STOPPED,
	ACTOR_STATE_DEAD
};

struct Actor {
	Actor_State state;
	Vec2 p;
	Vec2 tp;
};

Texture2d_Handle load_texture(const String path) {
	String content = system_read_entire_file(path);
	
	if (content.count) {
		defer { memory_free(content.data); };
		
		int w, h, n;
		u8 *pixels = stbi_load_from_memory(content.data, (int)content.count, &w, &h, &n, 4);
		if (pixels) {
			auto handle = gfx_create_texture2d((u32)w, (u32)h, 4, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
			stbi_image_free(pixels);
			return handle;
		} else {
			system_log(LOG_ERROR, "Scene", "Failed to parse texture: %s", tto_cstring(path));
			return im_white_texture();
		}
	} else {
		system_log(LOG_ERROR, "Scene", "Failed to load file: %s", tto_cstring(path));
		return im_white_texture();
	}
}

bool iload_monospaced_font(const String path, Monospaced_Font *font) {
	String content = system_read_entire_file(path);
	defer{ memory_free(content.data); };
	
	if (content.count) {
		Istream in = istream(content);
		
		u32 min_c = *istream_consume(&in, u32);
		u32 max_c = *istream_consume(&in, u32);
		r32 advance = *istream_consume(&in, r32);
		u32 size = *istream_consume(&in, u32);
		
		font->info.range = (Monospaced_Font_Glyph_Range *)memory_allocate(size);
		memcpy(font->info.range, istream_consume_size(&in, size), size);
		font->info.first = (s32)min_c;
		font->info.count = (s32)(max_c - min_c + 2);
		font->info.advance = advance;
		
		int w = *istream_consume(&in, int);
		int h = *istream_consume(&in, int);
		int n = *istream_consume(&in, int);
		u8 *pixels = (u8 *)istream_consume_size(&in, w * h * n);
		
		font->texture = gfx_create_texture2d((u32)w, (u32)h, (u32)n, Data_Format_RGBA8_UNORM_SRGB, (const u8 **)&pixels, Buffer_Usage_IMMUTABLE, 1);
		
		return true;
	}
	
	return false;
}

int karma_main_template() {
	r32    framebuffer_w = 1280;
	r32    framebuffer_h = 720;
	Handle platform = system_create_window(u8"Karma", 1280, 720, System_Window_Show_NORMAL);
	gfx_create_context(platform, Render_Backend_DIRECTX11, Vsync_ADAPTIVE, 2, (u32)framebuffer_w, (u32)framebuffer_h);
	
	ImGui_Initialize();
	
	random_init_global(context.id, system_get_counter());
	
	float learning_rate = 0.1f;
	float discount_factor = 0.88f;
	
	float min_epsilon = 0;
	float max_epsilon = 1;
	float depsilon = 0.1f;
	
	int generation = 0;
	int win_count = 0;
	int steps = 0;
	
	const int max_steps = 500;
	const int max_win_count = 15;
	
	//
	// Assets
	//
	
	auto stone_texture = load_texture("template/stone.png");
	auto grass_texture = load_texture("template/grass.png");
	auto star_texture  = load_texture("template/star.png");
	auto girlr_texture = load_texture("template/girl.png");
	auto girll_texture = load_texture("template/girl_flip.png");
	auto circle_texture = load_texture("template/circle.png");
	
	Monospaced_Font font;
	iload_monospaced_font("resources/font/mono.font", &font);
	
	//
	//
	//
	
	float star_jump_t = 0.0f;
	float star_spin_t = 0.0f;
	
	const Mm_Rect girl_anim_frames[] = {
		mm_rect(vec2(0.0f / 6.0f, 2.0f / 3.0f), vec2(1.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(1.0f / 6.0f, 2.0f / 3.0f), vec2(2.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(2.0f / 6.0f, 2.0f / 3.0f), vec2(3.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(3.0f / 6.0f, 2.0f / 3.0f), vec2(4.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(4.0f / 6.0f, 2.0f / 3.0f), vec2(5.0f / 6.0f, 1.0f / 3.0f)),
		
		mm_rect(vec2(5.0f / 6.0f, 2.0f / 3.0f), vec2(6.0f / 6.0f, 1.0f / 3.0f)),
		
		mm_rect(vec2(4.0f / 6.0f, 2.0f / 3.0f), vec2(5.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(3.0f / 6.0f, 2.0f / 3.0f), vec2(4.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(2.0f / 6.0f, 2.0f / 3.0f), vec2(3.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(1.0f / 6.0f, 2.0f / 3.0f), vec2(2.0f / 6.0f, 1.0f / 3.0f)),
		mm_rect(vec2(0.0f / 6.0f, 2.0f / 3.0f), vec2(1.0f / 6.0f, 1.0f / 3.0f)),
	};
	
	const Mm_Rect girl_frame_down = mm_rect(vec2(2.0f / 5.0f, 3.0f / 3.0f), vec2(3.0f / 5.0f, 2.0f / 3.0f));
	const Mm_Rect girl_frame_up   = mm_rect(vec2(0.0f / 5.0f, 3.0f / 3.0f), vec2(1.0f / 5.0f, 2.0f / 3.0f));
	const Mm_Rect girl_dead       = mm_rect(vec2(3.9f / 5.0f, 3.0f / 3.0f), vec2(5.0f / 5.0f, 2.0f / 3.0f));
	const Mm_Rect girl_win        = mm_rect(vec2(4.2f / 6.0f, 1.0f / 3.0f), vec2(5.5f / 6.0f, 0.0f / 3.0f));
	
	//
	//
	//
	
	r32 game_over_t = 0;
	const r32 MAX_GAME_OVER_TIME = 1.5f;
	
	Game game;
	game_init(&game);
	
	Qtable table;
	qtable_init(&table);
	
	Actor actor;
	actor.state = ACTOR_STATE_STOPPED;
	actor.p = vec2((r32)game.p_x, (r32)game.p_y);
	actor.tp = actor.p;
	
	bool running = true;
	
	r32 aspect_ratio = framebuffer_w / framebuffer_h;
	
	Simulation_Speed sim_speed = simulation_speed(SIMULATION_SPEED_1X);
	
	r32 const dt = 1.0f / 60.0f;
	r32       game_dt = dt * sim_speed.factor;
	r32       real_dt = dt;
	r32       game_t = 0.0f;
	r32       real_t = 0.0f;
	r32       accumulator_t = dt;
	
	r32 window_w = 0, window_h = 0;
	
	u64 frequency = system_get_frequency();
	u64 counter = system_get_counter();
	
	while (running) {
		auto events = system_poll_events();
		for (s64 event_index = 0; event_index < events.count; ++event_index) {
			Event &event = events[event_index];
			
			if (event.type & Event_Type_EXIT) {
				running = false;
				break;
			}
			
			if (event.type & Event_Type_WINDOW_RESIZE) {
				s32 w = event.window.dimension.x;
				s32 h = event.window.dimension.y;
				
				gfx_on_client_resize(w, h);
				window_w = (r32)w;
				window_h = (r32)h;
				
				if (window_w && window_h) {
					aspect_ratio = window_w / window_h;
					gfx_resize_framebuffer(w, h, lroundf(512 * aspect_ratio), 512);
					framebuffer_w = window_w;
					framebuffer_h = window_h;
				}
				
				continue;
			}
			
			if (ImGui_HandleEvent(event))
				continue;
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_ESCAPE) {
				system_request_quit();
				break;
			}
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F3) {
				if (sim_speed.index > 0) {
					sim_speed.index -= 1;
				}
				continue;
			}
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F4) {
				if (sim_speed.index < static_count(SIMULATION_SPEED_FACTORS) - 1) {
					sim_speed.index += 1;
				}
				continue;
			}
			
			if ((event.type & Event_Type_KEY_UP) && event.key.symbol == Key_F11) {
				system_fullscreen_state(SYSTEM_TOGGLE);
				continue;
			}
		}
		
		while (accumulator_t >= dt) {
			star_spin_t += dt;
			star_jump_t += 3.0f * dt;
			
			while (star_spin_t >= 2.0f * MATH_PI) star_spin_t -= 2.0f * MATH_PI;
			while (star_jump_t >= 2.0f * MATH_PI) star_jump_t -= 2.0f * MATH_PI;
			
			switch (actor.state) {
				case ACTOR_STATE_MOVING: {
					actor.p = lerp(actor.p, actor.tp, 1.0f - powf(1.0f - 0.977f, dt));
					if (vec2_length2(actor.tp - actor.p) < 0.01f) {
						actor.state = ACTOR_STATE_STOPPED;
						
						if (game_is_over(&game) || steps == max_steps) {
							game_over_t = MAX_GAME_OVER_TIME;
							actor.state = ACTOR_STATE_DEAD;
						}
					}
				} break;
				
				case ACTOR_STATE_DEAD: {
					if (game_over_t <= 0.0f) {
						win_count += game_win(&game);
						generation += 1;
						
						steps = 0;
						
						game_restart(&game);
						actor.state = ACTOR_STATE_STOPPED;
						actor.p = vec2((r32)game.p_x, (r32)game.p_y);
						actor.tp = actor.p;
						
						game_over_t = 0;
					} else {
						game_over_t -= dt;
					}
				} break;
				
				case ACTOR_STATE_STOPPED: {
					int old_score_index;
					Action action = get_action(&table, min_epsilon, max_epsilon, depsilon, game.p_x, game.p_y, &old_score_index);
					
					int new_player_x = game.p_x;
					int new_player_y = game.p_y;
					
					switch (action) {
						case ACTION_EAST: {
							new_player_x += 1;
						} break;
						
						case ACTION_WEST: {
							new_player_x -= 1;
						} break;
						
						case ACTION_NORTH: {
							new_player_y += 1;
						} break;
						
						case ACTION_SOUTH: {
							new_player_y -= 1;
						} break;
						
						invalid_default_case();
					}
					
					float reward = get_reward(&game, new_player_x, new_player_y);
					
					float estimate_of_optimal_future_value = get_max_optimal_future_reward_estimate(&table, new_player_x, new_player_y);
					
					float old_score = table.scores[old_score_index];
					
					float new_score = old_score + learning_rate * (reward + discount_factor * estimate_of_optimal_future_value - old_score);
					table.scores[old_score_index] = new_score;
					
					move_player(&game, new_player_x, new_player_y);
					
					actor.tp = vec2((r32)game.p_x, (r32)game.p_y);
					actor.state = ACTOR_STATE_MOVING;
					
					steps += 1;
				} break;
			}
			
			accumulator_t -= dt;
		}
		
		//
		//
		//
		
		r32 view_height = 1;
		r32 view_width = aspect_ratio * view_height;
		
		r32 tile_size = 1;
		
		r32 sx = 0.6f * WORLD_SIZE_X * tile_size;
		r32 sy = 0.6f * WORLD_SIZE_Y * tile_size;
		sx /= view_width;
		sy /= view_height;
		r32 iscale = maximum(sx, sy);
		r32 scale = 1.0f / iscale;
		
		auto &io = ImGui::GetIO();
		
		Vec2 cursor = io.MousePos;
		
		// Convert cursor and delta value from window space into world space
		cursor.x /= window_w;
		cursor.y /= window_h;
		cursor.y = 1.0f - cursor.y;
		cursor = 2.0f * cursor - vec2(1);
		
		cursor.x *= iscale * view_width;
		cursor.y *= iscale * view_height;
		
		bool draw_hovering = false;
		int pos_x = (int)(cursor.x + 0.5f * (r32)WORLD_SIZE_X);
		int pos_y = (int)(cursor.y + 0.5f * (r32)WORLD_SIZE_Y);
		
		if (pos_x >= 0 && pos_x < WORLD_SIZE_X && pos_y >= 0 && pos_y < WORLD_SIZE_Y) {
			draw_hovering = true;
			
			if (io.MouseClicked[ImGuiMouseButton_Left] && (pos_x > 0 || pos_y > 0)) {
				auto tile = get_tile(&game, pos_x, pos_y);
				if (tile == TILE_TYPE_EMPTY) {
					set_tile(&game, pos_x, pos_y, TILE_TYPE_BLOCK);
				} else if (tile == TILE_TYPE_BLOCK) {
					set_tile(&game, pos_x, pos_y, TILE_TYPE_EMPTY);
				}
			}
		}
		
		ImGui_UpdateFrame(real_dt);
		
		gfx_begin_drawing(Framebuffer_Type_HDR, Clear_ALL, vec4(0.05f, 0.05f, 0.05f, 1.0f));
		gfx_viewport(0, 0, window_w, window_h);
		
		Camera_View view = orthographic_view(-view_width, view_width, view_height, -view_height);
		Mat4 transform = mat4_scalar(scale, scale, 1.0f);
		
		im2d_begin(view, transform);
		
		r32 draw_x, draw_y;
		
		draw_y = -0.5f * (r32)WORLD_SIZE_Y + 0.5f;
		for (int y = 0; y < WORLD_SIZE_Y; ++y) {
			draw_x = -0.5f * (r32)WORLD_SIZE_X + 0.5f;
			for (int x = 0; x < WORLD_SIZE_X; ++x) {
				im2d_bind_texture(grass_texture);
				im2d_rect_centered(vec2(draw_x, draw_y), vec2(tile_size), vec4(1));
				im2d_unbind_texture();
				
				auto tile = get_tile(&game, x, y);
				switch (tile) {
					case TILE_TYPE_BLOCK: {
						im2d_bind_texture(stone_texture);
						im2d_rect_centered(vec2(draw_x, draw_y), vec2(tile_size), vec4(1));
						im2d_unbind_texture();
					} break;
					
					case TILE_TYPE_EMPTY: {
						r32 inten = mmclamp(0.01f, 1.2f, (r32)table.generation[y * WORLD_SIZE_X + x] / 80.0f);
						
						int index = hash_position(x, y);
						auto action_scores = &table.scores[index];
						
						int high_score_index = 0;
						float high_score = action_scores[0];
						
						u32 equal_count = 1;
						
						for (int i = 1; i < ACTION_COUNT; ++i) {
							if (action_scores[i] > high_score) {
								high_score_index = i;
								high_score = action_scores[i];
								equal_count = 1;
							} else if (action_scores[i] == high_score) {
								equal_count++;
								break;
							}
						}
						
						im2d_bind_texture(circle_texture);
						
						if (equal_count == 1) {
							inten = mmclamp(0.01f, 1.2f, action_scores[high_score_index]);
							auto color = vec4(inten * vec3(1.3f, 0.2f, 0.2f), 1);
							const auto flat_size = 0.5f;
							const auto height_size = 0.05f;
							
							switch (high_score_index) {
								case ACTION_EAST: {
									im2d_rect_centered(vec2(draw_x + 0.4f, draw_y), vec2(height_size, flat_size * tile_size), color);
								} break;
								
								case ACTION_WEST: {
									im2d_rect_centered(vec2(draw_x - 0.4f, draw_y), vec2(height_size, flat_size * tile_size), color);
								} break;
								
								case ACTION_NORTH: {
									im2d_rect_centered(vec2(draw_x, draw_y + 0.4f), vec2(flat_size * tile_size, height_size), color);
								} break;
								
								case ACTION_SOUTH: {
									im2d_rect_centered(vec2(draw_x, draw_y - 0.4f), vec2(flat_size * tile_size, height_size), color);
								} break;
								
								invalid_default_case();
							}
						}
						
						im2d_unbind_texture();
						
					} break;
					
					case TILE_TYPE_WIN: {
						r32 rotation = map(-1.0f, 1.0f, -MATH_TAU * 0.3f, MATH_TAU * 0.3f, cosf(star_spin_t));
						r32 y_offset = map(-1.0f, 1.0f, -0.1f, 0.5f, sinf(star_jump_t));
						
						auto color = win_count == 0 ? vec4(4.2f, 4.2f, 0.1f) : vec4(0.1f, (r32)minimum(win_count, 4) * 4.2f, 0.1f);
						
						im2d_bind_texture(stone_texture);
						im2d_rect_centered_rotated(vec2(draw_x, draw_y + y_offset), vec2(0.6f * tile_size), rotation, color);
						im2d_unbind_texture();
					} break;
					
					invalid_default_case();
				}
				
				draw_x += 1;
			}
			draw_y += 1;
		}
		
		auto girl_pos = vec2(actor.p.x - 0.5f * (r32)WORLD_SIZE_X + 0.5f, (r32)actor.p.y - 0.5f * (r32)WORLD_SIZE_Y + 0.5f);
		
		if (actor.state != ACTOR_STATE_DEAD) {
			Vec2 girl_offset;
			
			Mm_Rect girl_anim_rect;
			
#define sng(val) ((float)((0.0f) < val) - (float)(val < (0.0f)))
			
			r32 move_t_x = actor.tp.x - actor.p.x;
			r32 move_t_y = actor.tp.y - actor.p.y;
			
			if (fabsf(move_t_x) > fabsf(move_t_y)) {
				auto move_t = move_t_x;
				r32 move_scale = sng(move_t);
				move_t = clamp01(fabsf(move_t));
				int move_index = (int)roundf(move_t * (float)static_count(girl_anim_frames));
				move_index = mmclamp(0, static_count(girl_anim_frames), move_index);
				
				girl_anim_rect = girl_anim_frames[move_index];
				
				if (move_scale > 0.0f) {
					im2d_bind_texture(girlr_texture);
				} else {
					im2d_bind_texture(girll_texture);
				}
				
				girl_offset = vec2(0.2f, 0.4f);
			} else {
				im2d_bind_texture(girlr_texture);
				
				if (move_t_y > 0.0f) {
					girl_anim_rect = girl_frame_up;
				} else {
					girl_anim_rect = girl_frame_down;
				}
				
				girl_offset = vec2(0.0f, 0.4f);
			}
			
			im2d_rect_centered(girl_pos + girl_offset, vec2(tile_size, 1.5f * tile_size), girl_anim_rect, vec4(1));
			im2d_unbind_texture();
		} else {
			auto girl_rect = game.goal_reached ? girl_win : girl_dead;
			im2d_bind_texture(girlr_texture);
			im2d_rect_centered(girl_pos + vec2(0, 0.5f), vec2(tile_size,1.5f *  tile_size), girl_rect, vec4(1));
			im2d_unbind_texture();
		}
		
		
		if (draw_hovering) {
			im2d_rect_centered_outline(vec2((r32)pos_x - 0.5f * (r32)WORLD_SIZE_X + 0.5f,
											(r32)pos_y - 0.5f * (r32)WORLD_SIZE_Y + 0.5f),
									   vec2(1), vec4(1.2f, 1.2f, 1.2f), 0.02f);
			
		}
		
		auto button_pos = vec2(5, 1.0f);
		auto button_size = vec2(1.2f, 0.4f);
		auto button_color = vec4(0, 0.6f, 0.6f);
		
		if (test_point_inside_rect(cursor, mm_rect(button_pos, button_pos + button_size))) {
			button_color.rgb *= 1.6f;
			
			if (io.MouseClicked[ImGuiMouseButton_Left]) {
				qtable_init(&table);
				game_restart(&game);
				generation = 0;
				win_count = 0;
				steps = 0;
				
				actor.state = ACTOR_STATE_STOPPED;
				actor.p = vec2((r32)game.p_x, (r32)game.p_y);
				actor.tp = actor.p;
			}
		}
		
		im2d_rect(button_pos, button_size, button_color);
		
		auto progress = tprintf("Progress (%.3f)", sim_speed.factor);
		auto num_of_tries = tprintf("Tries: %d", generation);
		auto num_of_steps_left = tprintf("Steps Left: %d", max_steps - steps);
		auto slearning_rate = tprintf("Learning Rate: %.4f", learning_rate);
		auto sdiscount_factor = tprintf("Discount Factor: %.4f", discount_factor);
		auto sdecay_rate = tprintf("Decay Rate: %.4f", depsilon);
		
		im2d_bind_texture(font.texture);
		im2d_text(vec2(5, 4), 0.5f, font.info, progress, vec4(1));
		im2d_text(vec2(5, 3.5f), 0.35f, font.info, num_of_tries, vec4(1));
		im2d_text(vec2(5, 3.0f), 0.35f, font.info, num_of_steps_left, vec4(1));
		im2d_text(vec2(5, 2.5f), 0.35f, font.info, slearning_rate, vec4(1));
		im2d_text(vec2(5, 2.0f), 0.35f, font.info, sdiscount_factor, vec4(1));
		im2d_text(vec2(5, 1.5f), 0.35f, font.info, sdecay_rate, vec4(1));
		im2d_text(vec2(5, 1.0f), 0.35f, font.info, "Reset", vec4(1));
		im2d_unbind_texture();
		
		im2d_end();
		
		gfx_end_drawing();
		
		gfx_apply_bloom(2);
		
		gfx_begin_drawing(Framebuffer_Type_DEFAULT, Clear_COLOR, vec4(0.0f));
		gfx_blit_hdr(0, 0, window_w, window_h);
		gfx_viewport(0, 0, window_w, window_h);
		
#if 0
		ImGui::Begin("Configuration");
		ImGui::Text("Generation: %d", generation);
		ImGui::Text("Win Count: %d", win_count);
		ImGui::Text("Steps: %d", steps);
		
		ImGui::SliderFloat("Min Epsilon", &min_epsilon, 0, 1, "%.6f");
		ImGui::SliderFloat("Max Epsilon", &max_epsilon, 0, 1, "%.6f");
		ImGui::SliderFloat("Learning Rate", &learning_rate, 0, 1, "%.6f");
		ImGui::SliderFloat("Discount Factor", &discount_factor, 0, 1, "%.6f");
		ImGui::DragFloat("Decay Rate", &depsilon, 0.001f, 0, 1, "%.6f");
		
		if (ImGui::Button("Reset")) {
			qtable_init(&table);
			game_restart(&game);
			generation = 0;
			win_count = 0;
			steps = 0;
			
			actor.state = ACTOR_STATE_STOPPED;
			actor.p = vec2((r32)game.p_x, (r32)game.p_y);
			actor.tp = actor.p;
		}
		
		ImGui::Text("Speed: x%d", SIMULATION_SPEED_FACTORS[sim_speed.index]);
		ImGui::SameLine();
		if (ImGui::Button("-")) {
			if (sim_speed.index > 0) {
				sim_speed.index -= 1;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("+")) {
			if (sim_speed.index < static_count(SIMULATION_SPEED_FACTORS) - 1) {
				sim_speed.index += 1;
			}
		}
		
		
		ImGui::End();
#endif
		
		sim_speed = simulation_speed(sim_speed.index);
		
		ImGui_RenderFrame();
		
		gfx_end_drawing();
		
		gfx_present();
		
		reset_temporary_memory();
		
		auto new_counter = system_get_counter();
		auto counts = new_counter - counter;
		counter = new_counter;
		
		real_dt = ((1000000.0f * (r32)counts) / (r32)frequency) / 1000000.0f;
		real_t += real_dt;
		
		game_dt = real_dt * sim_speed.factor;
		
		accumulator_t += game_dt;
		accumulator_t = minimum(accumulator_t, 0.2f);
	}
	
	ImGui_Shutdown();
	gfx_destroy_context();
	
	return 0;
}
