#include "modules/core/karma.h"
#include "modules/core/lin_maths.h"
#include "modules/core/systems.h"
#include "modules/imgui/imconfig.h"
#include "modules/core/random.h"

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

constexpr u32 WORLD_SIZE_X = 3;
constexpr u32 WORLD_SIZE_Y = 3;
constexpr u32 WORLD_SIZE = WORLD_SIZE_X * WORLD_SIZE_Y;
constexpr u32 QTABLE_SIZE = WORLD_SIZE * ACTION_COUNT;

struct Game {
	Tile_Type tiles[WORLD_SIZE];
	int actor_x, actor_y;
	int win_x, win_y;
};

struct Qtable {
	float scores[QTABLE_SIZE];
};

void qtable_init(Qtable *table) {
	for (int i = 0; i < QTABLE_SIZE; ++i) {
		table->scores[i] = 0;
	}
}

int hash_position(int x, int y) {
	int index = (y * WORLD_SIZE_X + x) * ACTION_COUNT;
	return index;
}

Action get_action(Qtable *table, float epsilon, int x, int y, int *out_index) {
	float p = random_get_zero_to_one();

	int index = hash_position(x, y);
	auto action_scores = &table->scores[index];

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

// [ ] [ ] [O]
// [ ] [X] [ ]
// [S] [ ] [ ]

void game_init(Game *game) {
	game->tiles[0 * WORLD_SIZE_X + 0] = TILE_TYPE_EMPTY;
	game->tiles[0 * WORLD_SIZE_X + 1] = TILE_TYPE_EMPTY;
	game->tiles[0 * WORLD_SIZE_X + 2] = TILE_TYPE_EMPTY;

	game->tiles[1 * WORLD_SIZE_X + 0] = TILE_TYPE_EMPTY;
	game->tiles[1 * WORLD_SIZE_X + 1] = TILE_TYPE_BLOCK;
	game->tiles[1 * WORLD_SIZE_X + 2] = TILE_TYPE_EMPTY;

	game->tiles[2 * WORLD_SIZE_X + 0] = TILE_TYPE_EMPTY;
	game->tiles[2 * WORLD_SIZE_X + 1] = TILE_TYPE_EMPTY;
	game->tiles[2 * WORLD_SIZE_X + 2] = TILE_TYPE_WIN;

	game->actor_x = 0;
	game->actor_y = 0;

	game->win_x = 2;
	game->win_y = 2;
}

Tile_Type get_tile_print(Game *game, int x, int y) {
	if (x == game->actor_x && y == game->actor_y) {
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

float get_reward(Game *game, int x, int y) {
	if (x >= 0 && x < WORLD_SIZE_X && y >= 0 && y < WORLD_SIZE_Y) {
		int index = y * WORLD_SIZE_X + x;

		Tile_Type tile = game->tiles[index];

		switch (tile) {
			case TILE_TYPE_BLOCK: {
				return -10;
			} break;

			case TILE_TYPE_EMPTY: {
				int d0 = (game->win_x - x) * (game->win_x - x) + (game->win_y - y) * (game->win_y - y);
				int d1 = (game->win_x - game->actor_x) * (game->win_x - game->actor_x) + (game->win_y - game->actor_y) * (game->win_y - game->actor_y);
				if (d0 < d1)
					return 1;
				else
					return -1;
			} break;

			case TILE_TYPE_WIN: {
				return 10;
			} break;

				invalid_default_case();
		}
	}

	return -10;
}

float get_max_optimal_future_reward_estimate(Qtable *table, int x, int y) {
	if (x >= 0 && x < WORLD_SIZE_X && y >= 0 && y < WORLD_SIZE_Y) {
		int index = hash_position(x, y);
		auto action_scores = &table->scores[index];

		float high_score = action_scores[0];
		for (int i = 1; i < ACTION_COUNT; ++i) {
			if (action_scores[i] > high_score) {
				high_score = action_scores[i];
			}
		}

		return high_score;
	}

	return -1;
}

bool game_is_over(Game *game) {
	if (game->actor_x >= 0 && game->actor_x < WORLD_SIZE_X && game->actor_y >= 0 && game->actor_y < WORLD_SIZE_Y) {
		auto tile = get_tile(game, game->actor_x, game->actor_y);
		return tile == TILE_TYPE_WIN || tile == TILE_TYPE_BLOCK;
	}
	return true;
}

bool game_win(Game *game) {
	if (game->actor_x >= 0 && game->actor_x < WORLD_SIZE_X && game->actor_y >= 0 && game->actor_y < WORLD_SIZE_Y) {
		auto tile = get_tile(game, game->actor_x, game->actor_y);
		return tile == TILE_TYPE_WIN;
	}
	return false;
}

int karma_main_template() {
	random_init_global(context.id, system_get_counter());

	float learning_rate = 0.1f;
	float discount_factor = 0;
	float epsilon = 0;
	float depsilon = 0.01f;

	Game game;

	Qtable table;
	qtable_init(&table);

	for (int iter = 0; ; ++iter) {
		game_init(&game);

		system_trace("Iter: %d", iter);

		while (!game_is_over(&game)) {
			system_trace("[%d] [%d] [%d]", get_tile_print(&game, 0, 2), get_tile_print(&game, 1, 2), get_tile_print(&game, 2, 2));
			system_trace("[%d] [%d] [%d]", get_tile_print(&game, 0, 1), get_tile_print(&game, 1, 1), get_tile_print(&game, 2, 1));
			system_trace("[%d] [%d] [%d]", get_tile_print(&game, 0, 0), get_tile_print(&game, 1, 0), get_tile_print(&game, 2, 0));

			int old_score_index;
			Action action = get_action(&table, epsilon, game.actor_x, game.actor_y, &old_score_index);

			int new_player_x = game.actor_x;
			int new_player_y = game.actor_y;

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

			float estimate_of_optimal_future_value = get_max_optimal_future_reward_estimate(&table, game.actor_x, game.actor_y);

			float old_score = table.scores[old_score_index];

			float dq = old_score + learning_rate * (reward + discount_factor * estimate_of_optimal_future_value * old_score);
			table.scores[old_score_index] = old_score + dq;

			system_trace("reward: %f", reward);
			system_trace("estimate: %f", estimate_of_optimal_future_value);
			system_trace("");

			game.actor_x = new_player_x;
			game.actor_y = new_player_y;
		}

		if (game_win(&game)) {
			break;
		}

		epsilon -= depsilon;
		epsilon = maximum(0, epsilon);
	}

	system_trace("[%d] [%d] [%d]", get_tile_print(&game, 0, 2), get_tile_print(&game, 1, 2), get_tile_print(&game, 2, 2));
	system_trace("[%d] [%d] [%d]", get_tile_print(&game, 0, 1), get_tile_print(&game, 1, 1), get_tile_print(&game, 2, 1));
	system_trace("[%d] [%d] [%d]", get_tile_print(&game, 0, 0), get_tile_print(&game, 1, 0), get_tile_print(&game, 2, 0));

	return 0;
}
