#ifndef DRAWER_HEADER
#define DRAWER_HEADER

#include "colors.h"
#include "grid.h"
#include "helpers.h"

#define VERTICAL_EDGE   '|'
#define HORIZONTAL_EDGE '-'
#define EDGE_OFFSET     1

/*
 * scene:  represents the current scene on the screen.
 *
 * grid       : grid to be drawn.
 * full_blocks: if true, fully colored blocks will be drawn, if false only the
 *              block content (i.e. a char) either for grid block and for the
 *              edges.
 * edges      : if true, edges of the grid will be drawn.
 * infos      : array of informations strings. They will be written after the
 *              grid, on the right.
 * infos_count: number of strings in infos, once set, should remain the same.
 */
typedef struct
{
        grid *grid;
        bool full_blocks;
        bool edges;
        char **infos;
        int infos_count;
} scene;

/*
 * init_scene:  creates a new scene with provided arguments, starts ncurses,
 *              returns the pointer to the scene.
 */
scene *init_scene(grid *grid, bool full_blocks, bool edges,
                  char **infos, int infos_count);

/*
 * refresh_scene:  draw scene in its current state.
 */
void refresh_scene(const scene *scene);

/*
 * free_scene:  frees memory occupied by scene (not scene->grid, not
 *              scene->infos) and stops ncurses.
 */
void free_scene(scene *scene);

#endif
