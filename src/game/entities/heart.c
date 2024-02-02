#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "../game_setup.h"
#include <stdlib.h>

void heart_setup(SDL_Renderer *renderer, list *hitboxes[])
{
    calc_hitbox(renderer, textures[17], (SDL_Rect){0, 0, get_texture_width(17), get_texture_height(17)}, &hitboxes[17], 5, 1, -5);
}

void heart_create(game_entity *self, list **visual_effects)
{
    self->type = 8;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(17));
    self->xspeed = 0;
    self->yspeed = 20;
    self->xacc = 0;
    self->yacc = 0;
    self->cnt = 0;
    self->bullet_invisible = 1;
    self->texture = 17;
    self->score = 500;
    self->attack_cooldown = 0;
    self->damage = -20;
    self->health = 1;
    self->max_health = 1;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(17);
    self->h = get_texture_height(17);
}


void heart_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    return;
}


void heart_death(game_entity *self, list **entities, list **visual_effects, player *p)
{
    return;
}

void heart_draw(game_entity *self, int tiks, list **visual_effects, player p)
{
    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect*)(*visual_effects)->val)->priority = 700;
    ((visual_effect*)(*visual_effects)->val)->texture = 17;
    ((visual_effect*)(*visual_effects)->val)->x = self->x;
    ((visual_effect*)(*visual_effects)->val)->y = self->y;
    ((visual_effect*)(*visual_effects)->val)->texture_w = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_h = 1;
    ((visual_effect*)(*visual_effects)->val)->texture_i = 0;
    ((visual_effect*)(*visual_effects)->val)->texture_j = 0;
}