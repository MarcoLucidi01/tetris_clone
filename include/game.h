#ifndef GAME_HEADER
#define GAME_HEADER

#include <time.h>
#include "actions.h"
#include "grid.h"
#include "tetramino.h"
#include "controller.h"
#include "drawer.h"
#include "helpers.h"

#define STARTING_DELAY 700
#define GRID_ROWS      20
#define GRID_COLS      10
#define INFOS_COUNT    8
#define LEN_INFO_LINE  80
#define HISTORY_SIZE   4

/*
 * game:  represents the current "game state".
 *
 * grid         : grid of the game.
 * falling      : tetramino which is falling down. It's always the same, but the
 *                shape will be changed when it is locked down.
 * scene        : current game scene.
 * next_shape   : next shape letter.
 * next_shapes  : array of next shapes letters. It will be shuffled
 *                periodically.
 * next_index   : index for next_shapes array.
 * level        : current level.
 * score        : current score.
 * lines        : total number of full lines removed since the game started.
 * delay        : time to wait, in milliseconds, before the falling tetramino
 *                will come down by one step.
 * history_moves: last HISTORY_SIZE moves performed. If there is not a
 *                MOVE_DOWN, it will be performed without getting the action by
 *                the user.
 * history_index: index for history_moves array, used to overwrite the "older"
 *                move.
 * is_paused    : true if the game is in pause.
 * is_over      : true if the game is over.
 */
typedef struct
{
        grid *grid;
        tetramino *falling;
        scene *scene;
        char next_shape;
        char next_shapes[TOT_SHAPES];
        int next_index;
        unsigned int level;
        unsigned int score;
        unsigned int lines;
        int delay;
        int history_moves[HISTORY_SIZE];
        int history_index;
        bool is_paused;
        bool is_over;
} game;

/*
 * init_game:  allocates space and initialize a game, a grid and a scene.
 *             Allocates an array of strings to store infos for the scene.
 *             Returns a pointer to the new game.
 */
game *init_game(void);

/*
 * run_game:  "plays" game: refresh the scene, gets input from the user,
 *            performs actions, increment score and level, update the infos and
 *            the history of last moves and spawn pieces on top of the grid.
 */
void run_game(game *game);

/*
 * game_over:  sets game->is_over to true and waits for an input from the user
 *             to either start a new game (NEW_GAME) or quit (QUIT). If the user
 *             wants to start a new game, calls reset_game on game.
 *             Returns NEW_GAME or QUIT according to the user input.
 *
 *             Later run_game should be called to actually "start" a new game.
 */
action game_over(game *game);

/*
 * free_game:  frees all memory allocated by init_game() ending the game.
 */
void free_game(game *game);

#endif
