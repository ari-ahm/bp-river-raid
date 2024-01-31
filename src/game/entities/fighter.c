#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "stdlib.h"

void fighter_bullet_create(game_entity *self, list **visual_effects);

void fighter_create(game_entity *self, list **visual_effects)
{
    self->type = 5;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(14));
    self->xspeed = 0;
    self->yspeed = 35 + rand() % 20;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = 0;
    self->attack_cooldown = 1000;
    self->damage = 50;
    self->health = 50;
    self->max_health = 50;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(14);
    self->h = get_texture_height(14);
}


void fighter_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    if (self->attack_cooldown - time_delta < 0)
    {
        self->attack_cooldown = self->texture ? 2500 : 500;
        self->texture = 1 - self->texture;
        addElement(entities, sizeof(game_entity), 0);
        fighter_bullet_create((game_entity*)(*entities)->val, visual_effects);
        ((game_entity*)(*entities)->val)->y = self->y + self->h;
        ((game_entity*)(*entities)->val)->x = self->x + (self->w - get_texture_width(15)) / 2;
    }
    else
    {
        self->attack_cooldown -= time_delta;
    }
}


void fighter_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}



void fighter_bullet_create(game_entity *self, list **visual_effects)
{
    self->type = 6;
    self->xspeed = 0;
    self->yspeed = 150;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = 0;
    self->attack_cooldown = 0;
    self->damage = 10;
    self->health = 1;
    self->max_health = 1;
    self->w = get_texture_width(15);
    self->h = get_texture_height(15);
    self->rnd = rand() % 1000000;
    return;
}


void fighter_bullet_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void fighter_bullet_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}