#ifndef TETRAMINO_HEADER
#define TETRAMINO_HEADER

#include <string.h>
#include "colors.h"
#include "grid.h"
#include "helpers.h"

#define TOT_SHAPES     7
#define TETRAMINO_ROWS 4
#define TETRAMINO_COLS 4
#define ROTATIONS      4
#define TEXTURE        '#'

extern const char shapes_letters[TOT_SHAPES];
extern const char shapes[TOT_SHAPES][ROTATIONS][TETRAMINO_ROWS][TETRAMINO_COLS];

/*
 * tetramino:  represents a tetramino (a piece).
 *
 * size     : number of blocks on the longer side of the tetramino.
 * shape    : a multidimensional array storing 1's and 0's maps representing the
 *            shape of the tetramino. There is a map for each rotation.
 * rotation : current rotation, used as first index in shape array.
 * color    : ncurses color of the tetramino.
 * on_grid  : true if the tetramino is on the grid, i.e. is not locked down or
 *            its shape has not just changed.
 * row      : row number of the top left corner of tetramino on the grid.
 * col      : column number of the top left corner of tetramino on the grid.
 * is_locked: true if the tetramino is locked down i.e. it touched the bottom of
 *            the grid or the top of a block already locked down. Implies
 *            on_grid = false.
 */
typedef struct
{
        int size;
        char shape[ROTATIONS][TETRAMINO_ROWS][TETRAMINO_COLS];
        int rotation;
        colors color;
        bool on_grid;
        int row;
        int col;
        bool is_locked;
} tetramino;

/*
 * create_tetramino:  creates and returns a new tetramino (pointer) with a
 *                    default shape ('I').
 */
tetramino *create_tetramino(void);

/*
 * change_shape:  changes the shape of tetramino setting the map corresponding
 *                to shape_letter, the color and size. Sets also on_grid and
 *                is_locked to false.
 */
void change_shape(tetramino *tetramino, char shape_letter);

/*
 * rotate_forward:  change tetramino->rotation to point to the next shape map.
 */
void rotate_forward(tetramino *tetramino);

/*
 * rotate_backwards:  change tetramino->rotation to point to the previous shape
 *                    map.
 */
void rotate_backwards(tetramino *tetramino);

/*
 * put_on_grid:  puts tetramino on grid at position row, col (upper left
 *               corner), stores row, col inside tetramino and sets
 *               tetramino->on_grid to true.
 */
void put_on_grid(grid *grid, tetramino *tetramino, int row, int col);

/*
 * remove_from_grid:  remove tetramino from the grid and sets tetramino->on_grid
 *                    to false.
 */
void remove_from_grid(grid *grid, tetramino *tetramino);

#endif
