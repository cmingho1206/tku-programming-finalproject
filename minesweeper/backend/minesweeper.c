#include "minesweeper.h"

#include <stddef.h>
#include <stdlib.h>

static int in_bounds(const GameState *state, int row, int col) {
    return row >= 0 && row < state->rows && col >= 0 && col < state->cols;
}

static int count_adjacent_mines(const GameState *state, int row, int col) {
    int count = 0;
    int dr;
    int dc;
    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            int nr = row + dr;
            int nc = col + dc;
            if (dr == 0 && dc == 0) {
                continue;
            }
            if (in_bounds(state, nr, nc) && state->board[nr][nc] == -1) {
                count++;
            }
        }
    }
    return count;
}

static void update_win_state(GameState *state) {
    int safe_cells = state->rows * state->cols - state->mine_count;
    if (!state->game_over && state->revealed_count >= safe_cells) {
        state->win = 1;
    }
}

static void flood_reveal(GameState *state, int row, int col) {
    int dr;
    int dc;
    if (!in_bounds(state, row, col)) {
        return;
    }
    if (state->revealed[row][col] || state->flagged[row][col]) {
        return;
    }
    if (state->board[row][col] == -1) {
        return;
    }

    state->revealed[row][col] = 1;
    state->revealed_count++;
    if (state->board[row][col] != 0) {
        return;
    }

    for (dr = -1; dr <= 1; dr++) {
        for (dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            flood_reveal(state, row + dr, col + dc);
        }
    }
}

int ms_init(GameState *state, int rows, int cols, int mines, unsigned int seed) {
    int r;
    int c;
    int placed = 0;
    if (state == NULL) {
        return -1;
    }
    if (rows < 2 || cols < 2 || rows > MS_MAX_ROWS || cols > MS_MAX_COLS) {
        return -2;
    }
    if (mines < 1 || mines >= rows * cols) {
        return -3;
    }

    state->rows = rows;
    state->cols = cols;
    state->mine_count = mines;
    state->game_over = 0;
    state->win = 0;
    state->revealed_count = 0;
    state->seed = seed;

    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            state->board[r][c] = 0;
            state->revealed[r][c] = 0;
            state->flagged[r][c] = 0;
        }
    }

    srand(seed);
    while (placed < mines) {
        int rr = rand() % rows;
        int cc = rand() % cols;
        if (state->board[rr][cc] == -1) {
            continue;
        }
        state->board[rr][cc] = -1;
        placed++;
    }

    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            if (state->board[r][c] == -1) {
                continue;
            }
            state->board[r][c] = count_adjacent_mines(state, r, c);
        }
    }

    return 0;
}

int ms_reveal(GameState *state, int row, int col) {
    if (state == NULL) {
        return -1;
    }
    if (!in_bounds(state, row, col)) {
        return -2;
    }
    if (state->game_over || state->win) {
        return 2;
    }
    if (state->flagged[row][col]) {
        return 3;
    }
    if (state->revealed[row][col]) {
        return 4;
    }

    if (state->board[row][col] == -1) {
        state->revealed[row][col] = 1;
        state->game_over = 1;
        return 1;
    }

    flood_reveal(state, row, col);
    update_win_state(state);
    return 0;
}

int ms_toggle_flag(GameState *state, int row, int col) {
    if (state == NULL) {
        return -1;
    }
    if (!in_bounds(state, row, col)) {
        return -2;
    }
    if (state->revealed[row][col]) {
        return 1;
    }
    if (state->game_over || state->win) {
        return 2;
    }
    state->flagged[row][col] = !state->flagged[row][col];
    return state->flagged[row][col];
}

int ms_get_cell_display(const GameState *state, int row, int col) {
    if (state == NULL || !in_bounds(state, row, col)) {
        return -99;
    }
    if (state->revealed[row][col]) {
        if (state->board[row][col] == -1) {
            return -1;
        }
        return state->board[row][col];
    }
    if (state->flagged[row][col]) {
        return -2;
    }
    if (state->game_over && state->board[row][col] == -1) {
        return -1;
    }
    return -3;
}
