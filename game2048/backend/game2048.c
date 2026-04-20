#include "game2048.h"

#include <stdlib.h>

enum {
    DIR_UP = 0,
    DIR_DOWN = 1,
    DIR_LEFT = 2,
    DIR_RIGHT = 3
};

static void clear_state(Game2048State *state) {
    int r;
    int c;
    for (r = 0; r < G2048_SIZE; r++) {
        for (c = 0; c < G2048_SIZE; c++) {
            state->board[r][c] = 0;
        }
    }
    state->score = 0;
    state->game_over = 0;
    state->win = 0;
}

static int add_random_tile(Game2048State *state) {
    int empties[G2048_SIZE * G2048_SIZE][2];
    int empty_count = 0;
    int i;
    for (i = 0; i < G2048_SIZE * G2048_SIZE; i++) {
        int r = i / G2048_SIZE;
        int c = i % G2048_SIZE;
        if (state->board[r][c] == 0) {
            empties[empty_count][0] = r;
            empties[empty_count][1] = c;
            empty_count++;
        }
    }
    if (empty_count == 0) {
        return 0;
    }
    {
        int pick = rand() % empty_count;
        int value_roll = rand() % 10;
        int rr = empties[pick][0];
        int cc = empties[pick][1];
        state->board[rr][cc] = (value_roll == 0) ? 4 : 2;
    }
    return 1;
}

static int process_line(int line[G2048_SIZE], int *score_delta) {
    int tmp[G2048_SIZE] = {0, 0, 0, 0};
    int merged[G2048_SIZE] = {0, 0, 0, 0};
    int i;
    int pos = 0;
    int moved = 0;

    for (i = 0; i < G2048_SIZE; i++) {
        if (line[i] != 0) {
            tmp[pos++] = line[i];
        }
    }

    for (i = 0; i < G2048_SIZE - 1; i++) {
        if (tmp[i] != 0 && tmp[i] == tmp[i + 1] && !merged[i] && !merged[i + 1]) {
            tmp[i] *= 2;
            tmp[i + 1] = 0;
            merged[i] = 1;
            *score_delta += tmp[i];
        }
    }

    {
        int packed[G2048_SIZE] = {0, 0, 0, 0};
        int p = 0;
        for (i = 0; i < G2048_SIZE; i++) {
            if (tmp[i] != 0) {
                packed[p++] = tmp[i];
            }
        }
        for (i = 0; i < G2048_SIZE; i++) {
            if (line[i] != packed[i]) {
                moved = 1;
            }
            line[i] = packed[i];
        }
    }

    return moved;
}

int g2048_can_move(const Game2048State *state) {
    int r;
    int c;
    for (r = 0; r < G2048_SIZE; r++) {
        for (c = 0; c < G2048_SIZE; c++) {
            int v = state->board[r][c];
            if (v == 0) {
                return 1;
            }
            if (r + 1 < G2048_SIZE && state->board[r + 1][c] == v) {
                return 1;
            }
            if (c + 1 < G2048_SIZE && state->board[r][c + 1] == v) {
                return 1;
            }
        }
    }
    return 0;
}

int g2048_init(Game2048State *state, unsigned int seed) {
    if (state == NULL) {
        return -1;
    }
    state->seed = seed;
    srand(seed);
    clear_state(state);
    add_random_tile(state);
    add_random_tile(state);
    return 0;
}

int g2048_move(Game2048State *state, int direction) {
    int moved = 0;
    int score_delta = 0;
    int i;

    if (state == NULL) {
        return -1;
    }
    if (state->game_over || state->win) {
        return 0;
    }
    if (direction < DIR_UP || direction > DIR_RIGHT) {
        return -2;
    }

    for (i = 0; i < G2048_SIZE; i++) {
        int line[G2048_SIZE];
        int j;
        for (j = 0; j < G2048_SIZE; j++) {
            if (direction == DIR_LEFT) {
                line[j] = state->board[i][j];
            } else if (direction == DIR_RIGHT) {
                line[j] = state->board[i][G2048_SIZE - 1 - j];
            } else if (direction == DIR_UP) {
                line[j] = state->board[j][i];
            } else {
                line[j] = state->board[G2048_SIZE - 1 - j][i];
            }
        }

        moved |= process_line(line, &score_delta);

        for (j = 0; j < G2048_SIZE; j++) {
            if (direction == DIR_LEFT) {
                state->board[i][j] = line[j];
            } else if (direction == DIR_RIGHT) {
                state->board[i][G2048_SIZE - 1 - j] = line[j];
            } else if (direction == DIR_UP) {
                state->board[j][i] = line[j];
            } else {
                state->board[G2048_SIZE - 1 - j][i] = line[j];
            }
            if (line[j] == 2048) {
                state->win = 1;
            }
        }
    }

    if (moved) {
        state->score += score_delta;
        add_random_tile(state);
    }

    if (!g2048_can_move(state)) {
        state->game_over = 1;
    }
    return moved;
}
