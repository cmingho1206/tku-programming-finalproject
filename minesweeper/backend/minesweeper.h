#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#define MS_MAX_ROWS 30
#define MS_MAX_COLS 30

typedef struct {
    int rows;
    int cols;
    int mine_count;
    int game_over;
    int win;
    int revealed_count;
    unsigned int seed;
    int board[MS_MAX_ROWS][MS_MAX_COLS];
    int revealed[MS_MAX_ROWS][MS_MAX_COLS];
    int flagged[MS_MAX_ROWS][MS_MAX_COLS];
} GameState;

int ms_init(GameState *state, int rows, int cols, int mines, unsigned int seed);
int ms_reveal(GameState *state, int row, int col);
int ms_toggle_flag(GameState *state, int row, int col);
int ms_get_cell_display(const GameState *state, int row, int col);

#endif
