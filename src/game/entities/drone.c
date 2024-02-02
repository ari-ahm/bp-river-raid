#include "../game_defines.h"
#include "../../utils/linkedList.h"
#include "../../defines.h"
#include "../game_draw.h"
#include "../game_setup.h"
#include <stdlib.h>

void drone_setup(SDL_Renderer *renderer, list *hitboxes[])
{
    calc_hitbox(renderer, textures[16], (SDL_Rect){0, 0, get_texture_width(16) / 6, get_texture_height(16) / 8}, &hitboxes[16], 5, 1, -5);
}

void drone_create(game_entity *self, list **visual_effects)
{
    self->type = 7;
    self->y = -100;
    self->x = rand() % (WINDOW_WIDTH - get_texture_width(16) / 6);
    self->xspeed = 0;
    self->yspeed = 20;
    self->xacc = 0;
    self->yacc = 0;
    self->cnt = 0;
    self->texture = 16;
    self->attack_cooldown = 0;
    self->bullet_invisible = 0;
    self->damage = 50;
    self->health = 20;
    self->max_health = 20;
    self->rnd = rand() % 1000000;
    self->w = get_texture_width(16) / 6;
    self->h = get_texture_height(16) / 8;
}

void drone_update(game_entity *self, int time_delta, list **entities, list **visual_effects, player p)
{
    long double xdrag = self->xspeed * self->xspeed * drag_coef + friction;
    long double ydrag = self->yspeed * self->yspeed * drag_coef + friction;

    self->xacc = (self->xspeed > 0 ? -1 : 1) * xdrag;
    self->yacc = (self->yspeed > 0 ? -1 : 1) * ydrag;

    long double dx = p.x - self->x;
    long double dy = p.y - self->y;
    self->xacc += (dx / sqrt(dx * dx + dy * dy)) * drone_acceleration;
    self->yacc += (dy / sqrt(dx * dx + dy * dy)) * drone_acceleration;

    if (self->x < 0 || self->x + self->w >= WINDOW_WIDTH)
        self->xacc += (self->x < 0 ? 1 : -1) * 300;
    if (self->y < 0 || self->y + self->h >= WINDOW_HEIGHT)
        self->yacc += (self->y < 0 ? 1 : -1) * 300;
}

void drone_death(game_entity *self, list **entities, list **visual_effects)
{
    return;
}

void drone_draw(game_entity *self, int tiks, list **visual_effects, player p)
{
    int dir;
    long double dx = p.x - self->x;
    long double dy = p.y - self->y;
    if (dx == 0)
    {
        if (dy < 0)
            dir = 0;
        else
            dir = 4;
    }
    else
    {
        long double nsb = -dy / dx;
        for (dir = 0; nsb < tan_vals[dir] && dir < 3; dir++)
            ;
        if (dx < 0)
            dir += 4;
        dir %= 8;
    }

    addElement(visual_effects, sizeof(visual_effect), 0);
    ((visual_effect *)(*visual_effects)->val)->priority = 600;
    ((visual_effect *)(*visual_effects)->val)->texture = 16;
    ((visual_effect *)(*visual_effects)->val)->x = self->x;
    ((visual_effect *)(*visual_effects)->val)->y = self->y;
    ((visual_effect *)(*visual_effects)->val)->texture_w = 6;
    ((visual_effect *)(*visual_effects)->val)->texture_h = 8;
    ((visual_effect *)(*visual_effects)->val)->texture_i = ((tiks + self->rnd) / 200) % 6;
    ((visual_effect *)(*visual_effects)->val)->texture_j = dir;
}