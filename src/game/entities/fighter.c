#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "../game_setup.h"
#include <stdlib.h>

void fighter_bullet_create(game_entity *self, list **visual_effects);

void fighter_setup(SDL_Renderer *renderer, list *hitboxes[])
{
    calc_hitbox(renderer, textures[14], (SDL_Rect){0, 0, get_texture_width(14), get_texture_height(14)}, &hitboxes[14], 5, 1, -5);
}

void fighter_create(game_entity *self, list **visual_effects)
{
    self->type = 5;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(14));
    self->xspeed = 0;
    self->yspeed = 35 + rand() % 20;
    self->xacc = 0;
    self->yacc = 0;
    self->cnt = 0;
    self->bullet_invisible = 0;
    self->score = 300;
    self->texture = 14;
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
        self->attack_cooldown = self->cnt ? 2500 : 500;
        self->cnt = 1 - self->cnt;
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


void fighter_death(game_entity *self, list **entities, list **visual_effects, player *p)
{
    return;
}

void fighter_draw(game_entity *self, int tiks, list **visual_effects, player p)
{
    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect*)(*visual_effects)->val)->priority = 500;
    ((visual_effect*)(*visual_effects)->val)->texture = 14;
    ((visual_effect*)(*visual_effects)->val)->x = self->x;
    ((visual_effect*)(*visual_effects)->val)->y = self->y;
    ((visual_effect*)(*visual_effects)->val)->texture_w = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_h = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_i = 0;
    ((visual_effect*)(*visual_effects)->val)->texture_j = 0;
}

void fighter_bullet_setup(SDL_Renderer *renderer, list *hitboxes[])
{
    calc_hitbox(renderer, textures[15], (SDL_Rect){0, 0, get_texture_width(15), get_texture_height(15)}, &hitboxes[15], 5, 1, -5);
}

void fighter_bullet_create(game_entity *self, list **visual_effects)
{
    self->type = 6;
    self->xspeed = 0;
    self->yspeed = 150;
    self->xacc = 0;
    self->yacc = 0;
    self->cnt = 0;
    self->texture = 15;
    self->attack_cooldown = 0;
    self->score = 0;
    self->damage = 10;
    self->bullet_invisible = 0;
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


void fighter_bullet_death(game_entity *self, list **entities, list **visual_effects, player *p)
{
    return;
}

void fighter_bullet_draw(game_entity *self, int tiks, list **visual_effects, player p)
{
    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect*)(*visual_effects)->val)->priority = 1100;
    ((visual_effect*)(*visual_effects)->val)->texture = GAME_TEXTURES_PATH_SIZE + 1;
    ((visual_effect*)(*visual_effects)->val)->x = self->x - (32 - get_texture_width(15)) / 2;
    ((visual_effect*)(*visual_effects)->val)->y = self->y - (32 - get_texture_height(15)) / 2;
    ((visual_effect*)(*visual_effects)->val)->texture_w = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_h = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_i = 0;
    ((visual_effect*)(*visual_effects)->val)->texture_j = 0;

    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect*)(*visual_effects)->val)->priority = 1200;
    ((visual_effect*)(*visual_effects)->val)->texture = 15;
    ((visual_effect*)(*visual_effects)->val)->x = self->x;
    ((visual_effect*)(*visual_effects)->val)->y = self->y;
    ((visual_effect*)(*visual_effects)->val)->texture_w = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_h = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_i = 0;
    ((visual_effect*)(*visual_effects)->val)->texture_j = 0;
}