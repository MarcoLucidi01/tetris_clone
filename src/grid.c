#include "grid.h"


grid *create_grid(int rows, int cols)
{
        grid *new_grid = safe_malloc(sizeof(*new_grid));
        grid_block *new_blocks = safe_malloc(sizeof(*new_blocks) * rows * cols);

        new_grid->blocks = new_blocks;
        new_grid->rows = rows;
        new_grid->cols = cols;

        flush_grid(new_grid);

        return new_grid;
}

bool is_valid_position(const grid *grid, int row, int col)
{
        return (row >= 0 && row < grid->rows) && (col >= 0 && col < grid->cols);
}

bool is_empty_position(const grid *grid, int row, int col)
{
        return get_block(grid, row, col).content == EMPTY_CONTENT;
}

bool is_full_row(const grid *grid, int row)
{
        for (int col = 0; col < grid->cols; col++) {
                if (is_empty_position(grid, row, col)) {
                        return false;
                }
        }
        return true;
}

grid_block get_block(const grid *grid, int row, int col)
{
        return grid->blocks[row * grid->cols + col];
}

void fill_block(grid *grid, char content, colors color, int row, int col)
{
        grid->blocks[row * grid->cols + col].content = content;
        grid->blocks[row * grid->cols + col].color = color;
}

void flush_block(grid *grid, int row, int col)
{
        grid->blocks[row * grid->cols + col].content = EMPTY_CONTENT;
}

void flush_row(grid *grid, int row)
{
        for (int col = 0; col < grid->cols; col++) {
                flush_block(grid, row, col);
        }
}

void flush_grid(grid *grid)
{
        for (int row = 0; row < grid->rows; row++) {
                flush_row(grid, row);
        }
}

void swap_rows(grid *grid, int row1, int row2)
{
        grid_block temp1;
        grid_block temp2;

        for (int col = 0; col < grid->cols; col++) {

                temp1 = get_block(grid, row1, col);
                temp2 = get_block(grid, row2, col);

                fill_block(grid, temp1.content, temp1.color, row2, col);
                fill_block(grid, temp2.content, temp2.color, row1, col);
        }
}

void free_grid(grid *grid)
{
        free(grid->blocks);
        free(grid);
}
