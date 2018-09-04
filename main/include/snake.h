#ifndef SNAKE_H
#define SNAKE_H
#include "flipdot.h"

#define MAX_TAIL_LENGTH (DOT_COLUMNS * DOT_ROWS)
#define TOROIDAL 1

// A coordinate on the grid
typedef struct {
    int x;
    int y;
} coordinate_t;

//  Tail segments
typedef coordinate_t tail_t[MAX_TAIL_LENGTH];

// The direction of the snake
typedef enum {
    DIRECTION_UP = 1,
    DIRECTION_DOWN = 2,
    DIRECTION_LEFT = 4,
    DIRECTION_RIGHT = 8
} direction_t;

// The snake
typedef struct {
    coordinate_t head;
    tail_t tail;
    int tail_length;
    direction_t direction;
    bool is_dead;
} snake_t;

// Public subs
void snake_update();
void snake_init();

#endif