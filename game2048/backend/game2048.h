#ifndef GAME2048_H
#define GAME2048_H

#define G2048_SIZE 4

typedef struct {
    int board[G2048_SIZE][G2048_SIZE];
    int score;
    int game_over;
    int win;
    unsigned int seed;
} Game2048State;

int g2048_init(Game2048State *state, unsigned int seed);
int g2048_move(Game2048State *state, int direction);
int g2048_can_move(const Game2048State *state);

#endif
