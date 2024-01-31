#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "stdlib.h"

void cargo_create(game_entity *self, list **visual_effects)
{
    self->type = 2;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(7));
    self->xspeed = 0;
    self->yspeed = 40 + rand() % 20;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = 0;
    self->attack_cooldown = 1000;
    self->damage = 10;
    self->health = 150;
    self->max_health = 150;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(7);
    self->h = get_texture_height(7);
}


void cargo_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void cargo_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}