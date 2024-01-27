#ifndef GAME_DEFINES_H
#define GAME_DEFINES_H

typedef struct {
    int l;
    int r;
    int u;
    int d;
    int shoot;
} game_input_move;

typedef struct {
    long double x;
    long double y;
    long double xspeed;
    long double yspeed;
} player;

#endif