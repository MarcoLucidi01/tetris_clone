#include "game.h"

/*
 * reset_game:  sets game fields to their initial state.
 */
static void reset_game(game *game);

/*
 * spawn_next_on_top:  changes the shape for game->falling with game->next_shape
 *                     and tries to spawn game->falling on top of the grid. On
 *                     success, puts another random shape in game->next_shape
 *                     and returns true, on failure, returns false.
 */
static bool spawn_next_on_top(game *game);

/*
 * update_next_shape:  puts the next random shape in game->next_shape.
 *                     If all shapes in game->next_shapes have been used,
 *                     shuffles the array game->next_shapes before change
 *                     game->next_shape.
 *
 *                     http://tetris.wikia.com/wiki/Random_Generator
 */
static void update_next_shape(game *game);

/*
 * update_infos:  updates the strings in game->scene->infos to reflect the
 *                informations on the current state of the game.
 */
static void update_infos(game *game);

/*
 * get_action:  waits for a key to be pressed by the user for at most delay
 *              milliseconds and returns the next action to be performed
 *              according to the key pressed.
 *
 *              If the key is not valid or no key was pressed within delay
 *              milliseconds, MOVE_DOWN will be returned.
 *
 *              A negative delay will cause to wait indefinitely until a key
 *              will be pressed.
 */
static action get_action(int delay);

/*
 * push_in_history:  puts action in game->history_moves overwriting the "older"
 *                   move.
 */
static void push_in_history(action action, game *game);

/*
 * pause_game:  puts the game on pause waiting for the pause or end game key.
 */
static void pause_game(game *game);

/*
 * remove_full_lines:  scans the grid looking for rows without empty blocks and
 *                     removes them. Returns the number of full lines removed.
 */
static int remove_full_lines(game *game);

/*
 * increase_level:  increases game->level by 1 and reduces game->delay if
 *                  (game->lines / 10) + 1 is greater than game->level.
 */
static void increase_level(game *game);

/*
 * increase_score:  increases game->score according to lines (the number of
 *                  lines removed at the same time) and the current level.
 *
 *                  Applies the formula m * (n + 1) where n is level and m takes
 *                  the value of 40, 100, 300, 1200 depending on lines
 *                  (1 -> 40, 2 -> 100 ...).
 */
static void increase_score(game *game, int lines);


game *init_game(void)
{
        srand(time(NULL));

        game *new_game = safe_malloc(sizeof(*new_game));

        char **infos = safe_malloc(sizeof(*infos) * INFOS_COUNT);
        for (int i = 0; i < INFOS_COUNT; i++) {
                infos[i] = safe_malloc(sizeof(*infos[0]) * LEN_INFO_LINE);
        }

        new_game->grid = create_grid(GRID_ROWS, GRID_COLS);
        new_game->falling = create_tetramino();
        new_game->scene = init_scene(new_game->grid, true, true,
                                     infos, INFOS_COUNT);

        memcpy(new_game->next_shapes, shapes_letters, TOT_SHAPES);

        reset_game(new_game);

        return new_game;
}

static void reset_game(game *game)
{
        flush_grid(game->grid);
        game->falling->on_grid = false;

        game->next_index = 0;
        update_next_shape(game);

        game->level = 1;
        game->score = 0;
        game->lines = 0;
        game->delay = STARTING_DELAY;

        set_array(MOVE_DOWN, game->history_moves, HISTORY_SIZE);
        game->history_index = 0;

        game->is_paused = false;
        game->is_over = false;
}

void run_game(game *game)
{
        action action;
        int lines;

        while ( ! game->is_over) {

                if ( ! game->falling->on_grid) {
                        if ( ! spawn_next_on_top(game)) {
                                game->is_over = true;
                                continue;
                        }
                }

                update_infos(game);
                refresh_scene(game->scene);

                if ( ! is_in_array(MOVE_DOWN, game->history_moves, HISTORY_SIZE)) {
                        perform_action(game->grid, game->falling, MOVE_DOWN);
                        push_in_history(MOVE_DOWN, game);
                } else {
                        action = get_action(game->delay);
                        if (action == PAUSE) {
                                pause_game(game);
                                continue;
                        } else {
                                perform_action(game->grid, game->falling, action);
                                push_in_history(action, game);
                        }
                }

                if (game->falling->is_locked) {

                        lines = remove_full_lines(game);
                        if (lines) {
                                game->lines += lines;
                                increase_level(game);
                                increase_score(game, lines);
                        }
                }
        }
}

static bool spawn_next_on_top(game *game)
{
        set_array(MOVE_DOWN, game->history_moves, HISTORY_SIZE);

        change_shape(game->falling, game->next_shape);

        game->falling->row = 0;
        game->falling->col = game->grid->cols / 2 - game->falling->size / 2;

        if ( ! check_position(game->grid, game->falling,
                              game->falling->row, game->falling->col)) {
                return false;
        }
        put_on_grid(game->grid, game->falling,
                    game->falling->row, game->falling->col);
        update_next_shape(game);
        return true;
}

static void update_next_shape(game *game)
{
        if (game->next_index == 0) {
                shuffle_array(game->next_shapes, TOT_SHAPES);
        }
        game->next_shape = game->next_shapes[game->next_index];
        game->next_index = (game->next_index + 1) % TOT_SHAPES;
}

static void update_infos(game *game)
{
        snprintf(game->scene->infos[0], LEN_INFO_LINE,
                 "Next : %c", game->next_shape);
        snprintf(game->scene->infos[1], LEN_INFO_LINE,
                 "\n");
        snprintf(game->scene->infos[2], LEN_INFO_LINE,
                 "Level: %u", game->level);
        snprintf(game->scene->infos[3], LEN_INFO_LINE,
                 "Score: %u", game->score);
        snprintf(game->scene->infos[4], LEN_INFO_LINE,
                 "Lines: %u", game->lines);
        snprintf(game->scene->infos[5], LEN_INFO_LINE,
                 "\n");

        if (game->is_paused || game->is_over) {

                if (game->is_paused) {
                        snprintf(game->scene->infos[6], LEN_INFO_LINE,
                                 "PAUSE");
                        snprintf(game->scene->infos[7], LEN_INFO_LINE,
                                 "Press P to Unpause, E to End Game");
                } else {
                        snprintf(game->scene->infos[6], LEN_INFO_LINE,
                                 "GAME OVER");
                        snprintf(game->scene->infos[7], LEN_INFO_LINE,
                                 "Press N to New Game, Q to Quit");
                }
        } else {
                snprintf(game->scene->infos[6], LEN_INFO_LINE,
                         "\n");
                snprintf(game->scene->infos[7], LEN_INFO_LINE,
                         "\n");
        }
}

static action get_action(int delay)
{
        timeout(delay);

        switch (getch()) {
                case 'k':
                case KEY_UP:
                        return ROTATE;
                case 'h':
                case KEY_LEFT:
                        return MOVE_LEFT;
                case 'l':
                case KEY_RIGHT:
                        return MOVE_RIGHT;
                case 'j':
                case KEY_DOWN:
                        return MOVE_DOWN;
                case ' ':
                        return DROP;
                case 'n':
                        return NEW_GAME;
                case 'e':
                        return END_GAME;
                case 'p':
                        return PAUSE;
                case 'q':
                        return QUIT;
                default:
                        return MOVE_DOWN;
        }
}

static void push_in_history(action action, game *game)
{
        game->history_moves[game->history_index] = action;
        game->history_index = (game->history_index + 1) % HISTORY_SIZE;
}

static void pause_game(game *game)
{
        action action;

        game->is_paused = true;
        update_infos(game);
        refresh_scene(game->scene);

        while ((action = get_action(-1)) != PAUSE && action != END_GAME) {
                ;
        }
        if (action == END_GAME) {
                game->is_over = true;
        }
        game->is_paused = false;
}

static int remove_full_lines(game *game)
{
        int lines = 0;

        for (int row = 0; row < game->grid->rows; row++) {

                if (is_full_row(game->grid, row)) {
                        lines++;
                        flush_row(game->grid, row);
                        for (int i = row; i > 0; i--) {
                                swap_rows(game->grid, i, i - 1);
                        }
                }
        }
        return lines;
}

static void increase_level(game *game)
{
        int level = (game->lines / 10) + 1;

        if (level <= game->level) {
                return;
        }

        game->level = level;

        if (game->delay > 200) {
                game->delay -= 100;
        } else if (game->delay > 150) {
                game->delay -= 5;
        }
}

static void increase_score(game *game, int lines)
{
        int m = 0;

        switch (lines) {
                case 1:
                        m = 40;
                        break;
                case 2:
                        m = 100;
                        break;
                case 3:
                        m = 300;
                        break;
                case 4:
                        m = 1200;
                        break;
                default:
                        break;
        }
        game->score += m * (game->level + 1);
}

action game_over(game *game)
{
        action action;

        game->is_over = true;

        update_infos(game);
        refresh_scene(game->scene);

        while ((action = get_action(-1)) != NEW_GAME && action != QUIT) {
                ;
        }
        if (action == NEW_GAME) {
                reset_game(game);
        }
        return action;
}

void free_game(game *game)
{
        free_grid(game->grid);
        free(game->falling);
        for (int i = 0; i < INFOS_COUNT; i++) {
                free(game->scene->infos[i]);
        }
        free(game->scene->infos);
        free_scene(game->scene);
        free(game);
}
