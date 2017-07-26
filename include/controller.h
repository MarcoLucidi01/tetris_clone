#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include "actions.h"
#include "tetramino.h"

/*
 * perform_action:  performs action on tetramino and return true if action was
 *                  executed, false otherwise.
 */
bool perform_action(grid *grid, tetramino *tetramino, action action);

/*
 * rotate_tetramino:  rotates the shape of tetramino by +90. Return true if it
 *                    was possible, false otherwise.
 */
bool rotate_tetramino(grid *grid, tetramino *tetramino);

/*
 * move_tetramino:  change the position of tetramino according to action. Return
 *                  true if it was possible, false otherwise.
 *                  If tetramino is locked, sets true in tetramino->is_locked
 *                  and false in tetramino->on_grid.
 */
bool move_tetramino(grid *grid, tetramino *tetramino, action action);

/*
 * drop_tetramino:  performs an "hard drop" on tetramino and return true.
 */
bool drop_tetramino(grid *grid, tetramino *tetramino);

/*
 * check_position:  return true if tetramino can be put on the grid with the
 *                  current shape and rotation on position row x col as top left
 *                  corner, false otherwise.
 *                  In particular, checks if tetramino doesn't collide with
 *                  edges of grid or others blocks already on the grid.
 */
bool check_position(const grid *grid, const tetramino *tetramino,
                    int row, int col);

#endif
