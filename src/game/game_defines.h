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
    int w;
    int h;
    int health;
    int invincible;
    int shoot_cooldown;
    int gun_parity;
} player;

typedef struct {
    int type;
    int texture;
    long double x;
    long double y;
    int w, h;
    long double xspeed;
    long double yspeed;
    int damage;
    int health;
    int max_health;
} game_entity;

typedef struct {
    long double x;
    long double y;
    int damage;
    int range;
    long double yspeed;
    int w, h;
} bullet;

#endif