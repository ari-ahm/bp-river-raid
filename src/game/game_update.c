#include <stdio.h>
#include <stdlib.h>
#include "../defines.h"
#include "game_defines.h"
#include "game_draw.h"
#include "../utils/linkedList.h"
#include "../utils/utils.h"

const long double player_acceleration = 150;
const long double player_drag_coef = 0.05;
const long double player_friction = 1;

const int mine_spawn_probability = 150;
const int cargo_spawn_probability = 100;

void update_player_pos(int time_delta, game_input_move gim, player *p);
void update_entities_pos(int time_delta, list **entities);
void spawn_entities(int time_delta, list **entities);
void check_player_collision(player *p, list **entities);
void update_bullets(int time_delta, game_input_move gim, player *p, list **bullets);
void update_bullets_pos(int time_delta, list **bullets);
void check_bullet_collision(list **entities, list **bullets);

void update(int time_delta, game_input_move gim, player *p, list **entities, list **bullets)
{
    update_player_pos(time_delta, gim, p);
    update_bullets(time_delta, gim, p, bullets);
    update_bullets_pos(time_delta, bullets);
    update_entities_pos(time_delta, entities);
    spawn_entities(time_delta, entities);
    check_bullet_collision(entities, bullets);
    if (p->invincible - time_delta <= 0)
    {
        p->invincible = 0;
        check_player_collision(p, entities);
    }
    else
    {
        p->invincible -= time_delta;
    }
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
        ((game_entity *)(*entities)->val)->yspeed = 20 + rand() % 10;
        ((game_entity *)(*entities)->val)->texture = rand() % 4;
        ((game_entity *)(*entities)->val)->damage = 5;
        ((game_entity *)(*entities)->val)->health = 100;
        ((game_entity *)(*entities)->val)->max_health = 100;
        ((game_entity *)(*entities)->val)->w = get_texture_width((((game_entity *)(*entities)->val)->texture % 4) + 3);
        ((game_entity *)(*entities)->val)->h = get_texture_height((((game_entity *)(*entities)->val)->texture % 4) + 3);
    }
    if (rand() % 1000000 < cargo_spawn_probability * time_delta)
    {
        addElement(entities, sizeof(game_entity), 0);
        ((game_entity *)(*entities)->val)->type = 2;
        ((game_entity *)(*entities)->val)->y = -100;
        ((game_entity *)(*entities)->val)->x = rand() % WINDOW_WIDTH;
        ((game_entity *)(*entities)->val)->xspeed = 0;
        ((game_entity *)(*entities)->val)->yspeed = 40 + rand() % 20;
        ((game_entity *)(*entities)->val)->texture = 0;
        ((game_entity *)(*entities)->val)->damage = 10;
        ((game_entity *)(*entities)->val)->health = 175;
        ((game_entity *)(*entities)->val)->max_health = 175;
        ((game_entity *)(*entities)->val)->w = get_texture_width(7);
        ((game_entity *)(*entities)->val)->h = get_texture_height(7);
    }
}

void check_player_collision(player *p, list **entities)
{
    for (list *i = *entities; i; i = i->next)
    {
        if (box_collision(((game_entity *)i->val)->x, ((game_entity *)i->val)->y, ((game_entity *)i->val)->w, ((game_entity *)i->val)->h, p->x, p->y, p->w, p->h))
        {
            p->invincible = 5000;
            p->health -= ((game_entity *)i->val)->damage;
            break;
        }
    }
}

void update_bullets(int time_delta, game_input_move gim, player *p, list **bullets)
{
    if (gim.shoot && p->shoot_cooldown - time_delta < 0)
    {
        addElement(bullets, sizeof(bullet), 0);

        ((bullet *)(*bullets)->val)->x = p->x + get_texture_width(0) / 3 + get_texture_width(0) / 3 * p->gun_parity;
        ((bullet *)(*bullets)->val)->y = p->y;
        ((bullet *)(*bullets)->val)->yspeed = -0.4;
        ((bullet *)(*bullets)->val)->damage = 1;
        ((bullet *)(*bullets)->val)->w = get_texture_width(9);
        ((bullet *)(*bullets)->val)->h = get_texture_height(9);

        p->shoot_cooldown = 50;
        p->gun_parity = 1 - p->gun_parity;
    }
    else if (p->shoot_cooldown > 0)
    {
        p->shoot_cooldown -= time_delta;
    }
}

void update_bullets_pos(int time_delta, list **bullets)
{
    list *i = *bullets;
    for (int j = 0; i; j++)
    {
        ((bullet *)i->val)->y += ((bullet *)i->val)->yspeed * time_delta;
        if (((bullet *)i->val)->y < -get_texture_height(9) || ((bullet *)i->val)->y > WINDOW_HEIGHT)
        {
            i = i->next;
            removeElement(bullets, j--);
            continue;
        }
        i = i->next;
    }
}

void check_bullet_collision(list **entities, list **bullets)
{
    list *i = *entities;
    for (int i_cnt = 0; i; i_cnt++)
    {
        list *j = *bullets;
        int removed = 0;
        for (int j_cnt = 0; j; j_cnt++)
        {
            if (box_collision(((game_entity *)i->val)->x, ((game_entity *)i->val)->y, ((game_entity *)i->val)->w, ((game_entity *)i->val)->h, ((bullet *)j->val)->x, ((bullet *)j->val)->y, ((bullet *)j->val)->w, ((bullet *)j->val)->h))
            {
                ((game_entity *)i->val)->health -= ((bullet *)j->val)->damage;
                if (((game_entity *)i->val)->health < 0)
                {
                    removed = 1;
                    i = i->next;
                    removeElement(entities, i_cnt--);
                }

                j = j->next;
                removeElement(bullets, j_cnt--);
                if (removed)
                    break;
                continue;
            }

            j = j->next;
        }

        if (removed)
            continue;

        i = i->next;
    }
}