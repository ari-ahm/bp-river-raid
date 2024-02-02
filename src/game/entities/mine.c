#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "../game_setup.h"
#include <stdlib.h>


void mine_setup(SDL_Renderer *renderer, list *hitboxes[])
{
    calc_hitbox(renderer, textures[3], (SDL_Rect){0, 0, get_texture_width(3), get_texture_height(3)}, &hitboxes[3], 5, 1, -5);
    calc_hitbox(renderer, textures[4], (SDL_Rect){0, 0, get_texture_width(4), get_texture_height(4)}, &hitboxes[4], 5, 1, -5);
    calc_hitbox(renderer, textures[5], (SDL_Rect){0, 0, get_texture_width(5), get_texture_height(5)}, &hitboxes[5], 5, 1, -5);
    calc_hitbox(renderer, textures[6], (SDL_Rect){0, 0, get_texture_width(6), get_texture_height(6)}, &hitboxes[6], 5, 1, -5);
}

void mine_create(game_entity *self, list **visual_effects)
{
    self->type = 1;
    self->xspeed = 0;
    self->yspeed = 20 + rand() % 10;
    self->xacc = 0;
    self->yacc = 0;
    self->texture = rand() % 4 + 3;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(self->texture));
    self->attack_cooldown = 1000;
    self->damage = 5;
    self->bullet_invisible = 0;
    self->health = 50;
    self->max_health = 50;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(self->texture);
    self->h = get_texture_height(self->texture);
}


void mine_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void mine_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}

void mine_draw(game_entity *self, int tiks, list **visual_effects, player p)
{
    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect*)(*visual_effects)->val)->priority = 100;
    ((visual_effect*)(*visual_effects)->val)->texture = self->texture;
    ((visual_effect*)(*visual_effects)->val)->x = self->x;
    ((visual_effect*)(*visual_effects)->val)->y = self->y;
    ((visual_effect*)(*visual_effects)->val)->texture_w = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_h = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_i = 0;
    ((visual_effect*)(*visual_effects)->val)->texture_j = 0;
}