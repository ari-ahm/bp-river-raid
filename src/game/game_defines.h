#ifndef GAME_DEFINES_H
#define GAME_DEFINES_H

#include "../utils/linkedList.h"
#include <SDL2/SDL.h>

typedef struct
{
    int l;
    int r;
    int u;
    int d;
    int shoot;
} game_input_move;

typedef struct
{
    long double x;
    long double y;
    long double xspeed;
    long double yspeed;
    int w;
    int h;
    int health;
    int invincible;
    int shoot_cooldown;
    int bullet_cnt;
    int bullets_num;
    int bullet_damage;
} player;

typedef struct
{
    int type;
    int texture;
    long double x;
    long double y;
    int w, h;
    long double xspeed;
    long double yspeed;
    long double xacc;
    long double yacc;
    int damage;
    int health;
    int max_health;
    int rnd;
    int attack_cooldown;
    int cnt;
    int bullet_invisible;
} game_entity;

typedef struct
{
    long double x;
    long double y;
    int damage;
    int range;
    long double yspeed;
    int w, h;
} bullet;

typedef struct
{
    int type;
    int priority;
    long double x;
    long double y;
    long double xspeed;
    long double yspeed;
    int texture;
    int texture_w;
    int texture_h;
    int texture_i;
    int texture_j;
} visual_effect;

struct ENTITY_CALLBACK
{
    int prob[3];
    void (*setup)(SDL_Renderer *, list *[]);
    void (*create)(game_entity *, list **);
    void (*update)(game_entity *, int, list **, list **, player);
    void (*death)(game_entity *, list **, list **);
    void (*draw)(game_entity *, int, list **, player);
};

extern const long double player_acceleration;
extern const long double drone_acceleration;
extern const long double drag_coef;
extern const long double friction;

extern const struct ENTITY_CALLBACK ENTITIES_DEF[];
extern const int ENTITIES_DEF_SIZE;

extern const int GAME_TEXTURES_PATH_SIZE;

extern const long double pi;
extern const long double e;

extern long double tan_vals[];

extern SDL_Texture *textures[];

#endif