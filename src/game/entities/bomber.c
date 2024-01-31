#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "entities.h"
#include "stdlib.h"

void bomber_bullet_create(game_entity *self, list **visual_effects);

void bomber_create(game_entity *self, list **visual_effects)
{
    int dir = (rand() % 2);
    self->type = 3;
    self->y = -100;
    self->x = dir * (WINDOW_WIDTH - get_texture_width(10));
    self->xspeed = 0;
    self->yspeed = 25 + rand() % 20;
    self->xacc = dir ? -3 : 3;
    self->yacc = 0;
    self->texture = 0;
    self->attack_cooldown = 1000;
    self->damage = 10;
    self->health = 100;
    self->max_health = 100;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(10);
    self->h = get_texture_height(10);
}


void bomber_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    if (self->attack_cooldown - time_delta < 0)
    {
        self->attack_cooldown = 5000;
        addElement(entities, sizeof(game_entity), 0);
        bomber_bullet_create((game_entity*)(*entities)->val, visual_effects);
        ((game_entity*)(*entities)->val)->y = self->y + self->h;
        ((game_entity*)(*entities)->val)->x = self->x + (self->w - get_texture_width(11) / 8) / 2;
    }
    else
    {
        self->attack_cooldown -= time_delta;
    }
}


void bomber_death(game_entity *self, list **entities, list **visual_effects)
{
    // addElement(entities, sizeof(game_entity), 0);
    // heart_create((game_entity*)(*entities)->val, visual_effects);
    // ((game_entity*)(*entities)->val)->x = self->x + (self->w - ((game_entity*)(*entities)->val)->w) / 2;
    // ((game_entity*)(*entities)->val)->y = self->y + (self->h - ((game_entity*)(*entities)->val)->h) / 2;
}

void bomber_bullet_create(game_entity *self, list **visual_effects)
{
    self->type = 4;
    self->xspeed = 0;
    self->yspeed = 50 + rand() % 10;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = 0;
    self->attack_cooldown = 0;
    self->damage = 20;
    self->health = 1;
    self->max_health = 1;
    self->w = get_texture_width(11) / 8;
    self->h = get_texture_height(11);
    self->rnd = rand() % 1000000;
}


void bomber_bullet_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void bomber_bullet_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}
