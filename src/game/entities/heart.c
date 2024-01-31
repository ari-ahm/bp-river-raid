#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "stdlib.h"

void heart_create(game_entity *self, list **visual_effects)
{
    self->type = 8;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(17));
    self->xspeed = 0;
    self->yspeed = 20;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = 0;
    self->attack_cooldown = 0;
    self->damage = -20;
    self->health = 200000;
    self->max_health = 200000;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(17);
    self->h = get_texture_height(17);
}


void heart_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void heart_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}