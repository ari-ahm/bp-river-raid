#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "entities.h"
#include "../game_setup.h"
#include <stdlib.h>

void cargo_setup(SDL_Renderer *renderer, list *hitboxes[])
{
    calc_hitbox(renderer, textures[7], (SDL_Rect){0, 0, get_texture_width(7), get_texture_height(7)}, &hitboxes[7], 5, 1, -5);
}

void cargo_create(game_entity *self, list **visual_effects)
{
    self->type = 2;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(7));
    self->xspeed = 0;
    self->yspeed = 40 + rand() % 20;
    self->xacc = 0;
    self->yacc = 0;
    self->bullet_invisible = 0;
    self->texture = 7;
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
    addElement(entities, sizeof(game_entity), 0);
    heart_create((game_entity*)(*entities)->val, visual_effects);
    ((game_entity*)(*entities)->val)->x = self->x + (self->w - ((game_entity*)(*entities)->val)->w) / 2;
    ((game_entity*)(*entities)->val)->y = self->y + (self->h - ((game_entity*)(*entities)->val)->h) / 2;
}

void cargo_draw(game_entity *self, int tiks, list **visual_effects, player p)
{

    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect*)(*visual_effects)->val)->priority = 200;
    ((visual_effect*)(*visual_effects)->val)->texture = 7;
    ((visual_effect*)(*visual_effects)->val)->x = self->x;
    ((visual_effect*)(*visual_effects)->val)->y = self->y;
    ((visual_effect*)(*visual_effects)->val)->texture_w = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_h = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_i = 0;
    ((visual_effect*)(*visual_effects)->val)->texture_j = 0;
}