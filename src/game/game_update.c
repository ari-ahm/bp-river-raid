#include <stdio.h>
#include <stdlib.h>
#include "../defines.h"
#include "game_defines.h"
#include "../utils/linkedList.h"

const long double player_acceleration = 150;
const long double player_drag_coef = 0.05;
const long double player_friction = 1;

const int mine_spawn_probability = 150;

void update_player_pos(int time_delta, game_input_move gim, player *p);
void update_entities_pos(int time_delta, list **entities);
void spawn_entities(int time_delta, list **entities);

void update(int time_delta, game_input_move gim, player *p, list **entities)
{
    update_player_pos(time_delta, gim, p);
    update_entities_pos(time_delta, entities);
    spawn_entities(time_delta, entities);
}

void update_player_pos(int time_delta, game_input_move gim, player *p)
{

    long double xdrag = p->xspeed * p->xspeed * player_drag_coef + player_friction;
    long double ydrag = p->yspeed * p->yspeed * player_drag_coef + player_friction;

    long double xacc = (gim.r * player_acceleration + gim.l * -player_acceleration + (p->xspeed > 0 ? -1 : 1) * xdrag);
    long double yacc = (gim.d * player_acceleration + gim.u * -player_acceleration + (p->yspeed > 0 ? -1 : 1) * ydrag);

    p->xspeed += (xacc * time_delta / 1000);
    p->yspeed += (yacc * time_delta / 1000);

    p->x += (p->xspeed * time_delta / 1000);
    p->y += (p->yspeed * time_delta / 1000);
}

void update_entities_pos(int time_delta, list **entities)
{
    list *i = *entities;
    for (int j = 0; i; j++)
    {
        ((game_entity *)i->val)->x += ((game_entity *)i->val)->xspeed * time_delta / 1000;
        ((game_entity *)i->val)->y += ((game_entity *)i->val)->yspeed * time_delta / 1000;

        if (((game_entity *)i->val)->y > WINDOW_HEIGHT)
        {
            i = i->next;
            removeElement(entities, j--);
            continue;
        }
        i = i->next;
    }
}

void spawn_entities(int time_delta, list **entities)
{
    if (rand() % 1000000 < mine_spawn_probability * time_delta)
    {
        addElement(entities, sizeof(game_entity), 0);
        ((game_entity *)(*entities)->val)->type = 1;
        ((game_entity *)(*entities)->val)->y = -100;
        ((game_entity *)(*entities)->val)->x = rand() % WINDOW_WIDTH;
        ((game_entity *)(*entities)->val)->xspeed = 0;
        ((game_entity *)(*entities)->val)->yspeed = 40 + rand() % 20;
        ((game_entity *)(*entities)->val)->texture = rand() % 4;
    }
}