#include "modules/core/karma.h"
#include "modules/core/lin_maths.h"
#include "modules/core/systems.h"
#include "modules/imgui/imconfig.h"

enum Tile_Type {
	TILE_TYPE_EMPTY,
	TILE_TYPE_BLOCK,
	TILE_TYPE_WIN
};

struct Game {
	Tile_Type board[3 * 3];
};

// [ ] [ ] [O]
// [ ] [X] [ ]
// [S] [ ] [ ]

void game_init(Game *game) {
	game->board[0 * 3 + 0] = TILE_TYPE_EMPTY;
	game->board[0 * 3 + 1] = TILE_TYPE_EMPTY;
	game->board[0 * 3 + 2] = TILE_TYPE_WIN;

	game->board[1 * 3 + 0] = TILE_TYPE_EMPTY;
	game->board[1 * 3 + 1] = TILE_TYPE_BLOCK;
	game->board[1 * 3 + 2] = TILE_TYPE_EMPTY;

	game->board[2 * 3 + 0] = TILE_TYPE_EMPTY;
	game->board[2 * 3 + 1] = TILE_TYPE_EMPTY;
	game->board[2 * 3 + 2] = TILE_TYPE_EMPTY;
}

int karma_main_template() {

}
