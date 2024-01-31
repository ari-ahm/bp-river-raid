#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "stdlib.h"

void mine_create(game_entity *self, list **visual_effects)
{
    self->type = 1;
    self->xspeed = 0;
    self->yspeed = 20 + rand() % 10;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = rand() % 4;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(self->texture + 3));
    self->attack_cooldown = 1000;
    self->damage = 5;
    self->health = 50;
    self->max_health = 50;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(self->texture + 3);
    self->h = get_texture_height(self->texture + 3);
}


void mine_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void mine_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}