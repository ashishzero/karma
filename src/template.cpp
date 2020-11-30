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

struct Game {
	Tile_Type tiles[WORLD_SIZE_X * WORLD_SIZE_Y];
	int actor_x, actor_y;
};

struct Qtable {
	float scores[(WORLD_SIZE_X + 2) * (WORLD_SIZE_Y + 2) * ACTION_COUNT];
};

void qtable_init(Qtable *table) {
	for (int i = 0; i < (WORLD_SIZE_X + 2) * (WORLD_SIZE_Y + 2) * ACTION_COUNT; ++i) {
		table->scores[i] = random_r32_range_r32(-1, 1);
	}
}

Action get_action(Qtable *table, float epsilon, int x, int y, float **score) {
	float p = random_r32_range_r32(0, 1);

	int index = (y + 1) * WORLD_SIZE_X + (x + 1) * ACTION_COUNT;
	auto action_scores = &table->scores[index];

	if (p > epsilon) {

		int high_score_index = 0;
		float high_score = action_scores[0];
		for (int i = 1; i < ACTION_COUNT; ++i) {
			if (action_scores[i] > high_score) {
				high_score_index = i;
				high_score = action_scores[i];
			}
		}

		assert(high_score_index >= 0);

		*score = &action_scores[high_score_index];

		return (Action)high_score_index;
	}

	int n = (int)lroundf(p * (ACTION_COUNT - 1));
	*score = &action_scores[n];

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
}

Tile_Type get_tile(Game *game, int x, int y) {
	if (x == game->actor_x && y == game->actor_y) {
		return TILE_TYPE_COUNT;
	}

	int index = y * WORLD_SIZE_X + x;
	assert(index < WORLD_SIZE_X *WORLD_SIZE_Y);
	return game->tiles[index];
}

float get_reward(Game *game) {
	if (game->actor_x >= 0 && game->actor_x < WORLD_SIZE_X && game->actor_y >= 0 && game->actor_y < WORLD_SIZE_Y) {
		int index = game->actor_y * WORLD_SIZE_X + game->actor_x;
		Tile_Type tile = game->tiles[index];

		switch (tile) {
			case TILE_TYPE_BLOCK: return -1;
			case TILE_TYPE_EMPTY: return 0;
			case TILE_TYPE_WIN: return 1;

				invalid_default_case();
		}
	}

	return -1;
}

float get_max_reward_available(Qtable *table, int x, int y) {
	int index = (y + 1) * WORLD_SIZE_X + (x + 1) * ACTION_COUNT;
	auto action_scores = &table->scores[index];

	int high_score_index = 0;
	float high_score = action_scores[high_score_index];
	for (int i = 1; i < ACTION_COUNT; ++i) {
		if (action_scores[i] > high_score) {
			high_score_index = i;
			high_score = action_scores[i];
		}
	}

	return high_score;
}

bool game_is_over(Game *game) {
	if (game->actor_x >= 0 && game->actor_x < WORLD_SIZE_X && game->actor_y >= 0 && game->actor_y < WORLD_SIZE_Y) {
		auto tile = get_tile(game, game->actor_x, game->actor_y);
		return tile == TILE_TYPE_WIN || tile == TILE_TYPE_BLOCK;
	}
	return true;
}

int karma_main_template() {
	random_init_global(context.id, system_get_counter());

	float learning_rate = 0.1f;
	float discount_factor = 0.1f;
	float epsilon = 1;
	float depsilon = 0.01f;

	Game game;

	Qtable table;
	qtable_init(&table);

	for (int iter = 0; ; ++iter) {
		game_init(&game);

		system_trace("Iter: %d", iter);

		while (!game_is_over(&game)) {
			system_trace("[%d] [%d] [%d]", get_tile(&game, 0, 2), get_tile(&game, 1, 2), get_tile(&game, 2, 2));
			system_trace("[%d] [%d] [%d]", get_tile(&game, 0, 1), get_tile(&game, 1, 1), get_tile(&game, 2, 1));
			system_trace("[%d] [%d] [%d]", get_tile(&game, 0, 0), get_tile(&game, 1, 0), get_tile(&game, 2, 0));

			float *old_score;
			Action action = get_action(&table, epsilon, game.actor_x, game.actor_y, &old_score);

			switch (action) {
				case ACTION_EAST: {
					game.actor_x += 1;
				} break;

				case ACTION_WEST: {
					game.actor_x -= 1;
				} break;

				case ACTION_NORTH: {
					game.actor_y += 1;
				} break;

				case ACTION_SOUTH: {
					game.actor_y -= 1;
				} break;
			}

			float reward = get_reward(&game);
			float estimate_of_optimal_future_value = get_max_reward_available(&table, game.actor_x, game.actor_y);

			float dq = *old_score + learning_rate * (reward + discount_factor * estimate_of_optimal_future_value * *old_score);
			*old_score = *old_score + dq;

			system_trace("reward: %f", reward);
			system_trace("");
		}

		epsilon -= depsilon;
		epsilon = maximum(0, epsilon);
	}

	return 0;
}
