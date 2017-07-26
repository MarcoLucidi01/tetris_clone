#include "drawer.h"

/*
 * draw_edges:  draw the edges of scene->grid. Color used is BLUE.
 */
static void draw_edges(const scene *scene);

/*
 * draw_infos:  writes scene->infos strings starting from the top right corner
 *              of the grid.
 */
static void draw_infos(const scene *scene);

/*
 * clear_infos:  remove scene->infos strings from the screen.
 */
static void clear_infos(const scene *scene);

/*
 * draw_grid:  draw scene->grid. Only draw blocks which have changed.
 */
static void draw_grid(const scene *scene);

/*
 * draw_block_if_different:  gets the current block in row, col from ncurses and
 *                           the future row, col block from scene->grid and
 *                           compares them: if they are equal skip the draw,
 *                           else draws the future block.
 */
static void draw_block_if_different(const scene *scene, int row, int col);


scene *init_scene(grid *grid, bool full_blocks, bool edges,
                  char **infos, int infos_count)
{
        scene *new_scene = safe_malloc(sizeof(*new_scene));

        new_scene->grid = grid;
        new_scene->infos = infos;
        new_scene->infos_count = infos_count;
        new_scene->full_blocks = full_blocks;
        new_scene->edges = edges;

        initscr();
        curs_set(FALSE);
        keypad(stdscr, TRUE);
        noecho();
        cbreak();
        start_color();

        if (new_scene->full_blocks) {
                init_pair(1, RED,     RED);
                init_pair(2, GREEN,   GREEN);
                init_pair(3, YELLOW,  YELLOW);
                init_pair(4, BLUE,    BLUE);
                init_pair(5, MAGENTA, MAGENTA);
                init_pair(6, CYAN,    CYAN);
                init_pair(7, WHITE,   WHITE);
        } else {
                init_pair(1, RED,     BLACK);
                init_pair(2, GREEN,   BLACK);
                init_pair(3, YELLOW,  BLACK);
                init_pair(4, BLUE,    BLACK);
                init_pair(5, MAGENTA, BLACK);
                init_pair(6, CYAN,    BLACK);
                init_pair(7, WHITE,   BLACK);
        }

        return new_scene;
}

static void draw_edges(const scene *scene)
{
        attron(COLOR_PAIR(BLUE));
        for (int row = 0; row < scene->grid->rows + EDGE_OFFSET * 2; row++) {
                mvaddch(row,
                        0,
                        VERTICAL_EDGE);
                mvaddch(row,
                        scene->grid->cols + EDGE_OFFSET,
                        VERTICAL_EDGE);
        }
        for (int col = 0; col < scene->grid->cols; col++) {
                mvaddch(0,
                        col + EDGE_OFFSET,
                        HORIZONTAL_EDGE);
                mvaddch(scene->grid->rows + EDGE_OFFSET,
                        col + EDGE_OFFSET,
                        HORIZONTAL_EDGE);
        }
        attroff(COLOR_PAIR(BLUE));
}

void refresh_scene(const scene *scene)
{
        if (scene->edges) {
                draw_edges(scene);
        }

        /*
         * because the strings could change and it's easier erase and rewrite
         */
        clear_infos(scene);
        draw_infos(scene);

        draw_grid(scene);

        refresh();
}

static void draw_infos(const scene *scene)
{
        int col_offset = scene->grid->cols + 1;    /* + 1 for a space */
        col_offset += (scene->edges) ? EDGE_OFFSET * 2 : 0;

        for (int row = 0; row < scene->infos_count; row++) {
                mvaddstr(row,
                         col_offset,
                         scene->infos[row]);
        }
}

static void clear_infos(const scene *scene)
{
        int col_offset = scene->grid->cols + 1;    /* + 1 for a space */
        col_offset += (scene->edges) ? EDGE_OFFSET * 2 : 0;

        for (int row = 0; row < scene->infos_count; row++) {
                move(row, col_offset);
                clrtoeol();
        }
}

static void draw_grid(const scene *scene)
{
        for (int row = 0; row < scene->grid->rows; row++) {
                for (int col = 0; col < scene->grid->cols; col++) {

                        draw_block_if_different(scene, row, col);
                }
        }
}

static void draw_block_if_different(const scene *scene, int row, int col)
{
        int row_offset, col_offset;
        grid_block future_block;
        chtype current_block;

        row_offset = col_offset = (scene->edges) ? EDGE_OFFSET : 0;

        /* block from ncurses */
        current_block = mvinch(row + row_offset, col + col_offset);
        /* block from scene->grid */
        future_block = get_block(scene->grid, row, col);

        if ((future_block.content == (current_block & A_CHARTEXT)) &&
            (future_block.color == (current_block & A_COLOR))) {
                return;
        } else if (is_empty_position(scene->grid, row, col)) {
                mvaddch(row + row_offset,
                        col + col_offset,
                        ' ');
        } else {
                attron(COLOR_PAIR(future_block.color));
                mvaddch(row + row_offset,
                        col + col_offset,
                        future_block.content);
                attroff(COLOR_PAIR(future_block.color));
        }
}

void free_scene(scene *scene)
{
        free(scene);
        endwin();
}
