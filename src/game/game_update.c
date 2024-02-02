#include <stdio.h>
#include <stdlib.h>
#include "../defines.h"
#include "game_defines.h"
#include "game_draw.h"
#include "../utils/linkedList.h"
#include "../utils/utils.h"

const long double player_acceleration = 150;
const long double drone_acceleration = 140;
const long double drag_coef = 0.05;
const long double friction = 1;

void __update_player_pos(int time_delta, game_input_move gim, player *p);
void __update_entities(int time_delta, list **entities, list **visual_effects, player p);
void __update_entities_pos(int time_delta, list **entities, list **visual_effects);
void __spawn_entities(int time_delta, list **entities, list **visual_effects, int lvl);
void __check_player_collision(player *p, list **entities, list *hitboxes[]);
void __update_bullets(int time_delta, game_input_move gim, player *p, list **bullets);
void __update_bullets_pos(int time_delta, list **bullets);
void __check_bullet_collision(list **entities, list **bullets, list **visual_effects, list *hitboxes[]);

void update(int time_delta, game_input_move gim, player *p, list **entities, list **bullets, list **visual_effects, list *hitboxes[], int lvl)
{
    __update_player_pos(time_delta, gim, p);
    __update_bullets_pos(time_delta, bullets);
    __update_bullets(time_delta, gim, p, bullets);
    __update_entities_pos(time_delta, entities, visual_effects);
    __spawn_entities(time_delta, entities, visual_effects, lvl);
    __check_bullet_collision(entities, bullets, visual_effects, hitboxes);
    __update_entities(time_delta, entities, visual_effects, *p);
    if (p->invincible - time_delta <= 0)
    {
        p->invincible = 0;
        __check_player_collision(p, entities, hitboxes);
    }
    else
    {
        p->invincible -= time_delta;
    }
}

void __update_player_pos(int time_delta, game_input_move gim, player *p)
{

    long double xdrag = p->xspeed * p->xspeed * drag_coef + friction;
    long double ydrag = p->yspeed * p->yspeed * drag_coef + friction;

    long double xacc = (gim.r * player_acceleration + gim.l * -player_acceleration + (p->xspeed > 0 ? -1 : 1) * xdrag);
    long double yacc = (gim.d * player_acceleration + gim.u * -player_acceleration + (p->yspeed > 0 ? -1 : 1) * ydrag);

    if (p->x < 0 || p->x + p->w >= WINDOW_WIDTH)
        xacc += (p->x < 0 ? 1 : -1) * 300;
    if (p->y < 0 || p->y + p->h >= WINDOW_HEIGHT)
        yacc += (p->y < 0 ? 1 : -1) * 300;

    p->xspeed += (xacc * time_delta / 1000);
    p->yspeed += (yacc * time_delta / 1000);

    p->x += (p->xspeed * time_delta / 1000);
    p->y += (p->yspeed * time_delta / 1000);
}

void __update_entities_pos(int time_delta, list **entities, list **visual_effects)
{
    for (list *i = *entities; i;)
    {
        ((game_entity *)i->val)->xspeed += ((game_entity *)i->val)->xacc * time_delta / 1000;
        ((game_entity *)i->val)->yspeed += ((game_entity *)i->val)->yacc * time_delta / 1000;

        ((game_entity *)i->val)->x += ((game_entity *)i->val)->xspeed * time_delta / 1000;
        ((game_entity *)i->val)->y += ((game_entity *)i->val)->yspeed * time_delta / 1000;

        if (((game_entity *)i->val)->y > WINDOW_HEIGHT + 100 || ((game_entity *)i->val)->y < -200 || ((game_entity *)i->val)->x < -100 || ((game_entity *)i->val)->x > WINDOW_WIDTH)
        {
            ENTITIES_DEF[((game_entity *)i->val)->type].death((game_entity *)i->val, entities, visual_effects);
            list *tmp = i;
            i = i->next;
            removeElementPtr(entities, tmp);
            continue;
        }
        i = i->next;
    }
}

void __spawn_entities(int time_delta, list **entities, list **visual_effects, int lvl)
{
    for (int i = 0; i < ENTITIES_DEF_SIZE; i++)
    {
        if (rand() % 1000000 < ENTITIES_DEF[i].prob[lvl] * time_delta)
        {
            addElement(entities, sizeof(game_entity), 0);
            ENTITIES_DEF[i].create((game_entity *)(*entities)->val, visual_effects);
        }
    }
}

void __check_player_collision(player *p, list **entities, list *hitboxes[])
{
    for (list *i = *entities; i; i = i->next)
    {
        if (hitbox_collision(((game_entity *)i->val)->x, ((game_entity *)i->val)->y, hitboxes[((game_entity *)i->val)->texture], p->x, p->y, hitboxes[0]))
        {
            p->invincible = 5000;
            p->health -= ((game_entity *)i->val)->damage;
            p->health = min(100, p->health);
            ((game_entity *)i->val)->health -= 20;
            if (((game_entity *)i->val)->type == 8)
                ((game_entity *)i->val)->health = -1;
            break;
        }
    }
}

void __update_bullets(int time_delta, game_input_move gim, player *p, list **bullets)
{
    if (gim.shoot && p->shoot_cooldown - time_delta < 0)
    {
        addElement(bullets, sizeof(bullet), 0);

        ((bullet *)(*bullets)->val)->x = p->x + get_texture_width(0) / (p->bullets_num + 1) + get_texture_width(0) / (p->bullets_num + 1) * p->bullet_cnt;
        ((bullet *)(*bullets)->val)->y = p->y;
        ((bullet *)(*bullets)->val)->yspeed = -0.4;
        ((bullet *)(*bullets)->val)->damage = p->bullet_damage;
        ((bullet *)(*bullets)->val)->w = get_texture_width(9);
        ((bullet *)(*bullets)->val)->h = get_texture_height(9);

        p->shoot_cooldown = 100 / (p->bullets_num) + p->shoot_cooldown - time_delta;
        p->bullet_cnt++;
        p->bullet_cnt %= p->bullets_num;
    }
    else if (p->shoot_cooldown > 0)
    {
        p->shoot_cooldown -= time_delta;
    }
}

void __update_bullets_pos(int time_delta, list **bullets)
{
    for (list *i = *bullets; i;)
    {
        ((bullet *)i->val)->y += ((bullet *)i->val)->yspeed * time_delta;
        if (((bullet *)i->val)->y < -get_texture_height(9) || ((bullet *)i->val)->y > WINDOW_HEIGHT)
        {
            list *tmp = i;
            i = i->next;
            removeElementPtr(bullets, tmp);
            continue;
        }
        i = i->next;
    }
}

void __check_bullet_collision(list **entities, list **bullets, list **visual_effects, list *hitboxes[])
{
    for (list *i = *entities; i;)
    {
        int removed = 0;
        for (list *j = *bullets; j;)
        {
            if (!((game_entity *)i->val)->bullet_invisible && hitbox_collision(((game_entity *)i->val)->x, ((game_entity *)i->val)->y, hitboxes[((game_entity *)i->val)->texture], ((bullet *)j->val)->x, ((bullet *)j->val)->y, hitboxes[9]))
            {
                ((game_entity *)i->val)->health -= ((bullet *)j->val)->damage;
                if (((game_entity *)i->val)->health <= 0)
                {
                    ENTITIES_DEF[((game_entity *)i->val)->type].death((game_entity *)i->val, entities, visual_effects);
                    removed = 1;
                    list *tmp = i;
                    i = i->next;
                    removeElementPtr(entities, tmp);
                }
                list *tmp = j;
                j = j->next;
                removeElementPtr(bullets, tmp);
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

void __update_entities(int time_delta, list **entities, list **visual_effects, player p)
{
    list *i = *entities;
    for (; i; i = i->next)
    {
        ENTITIES_DEF[((game_entity *)i->val)->type].update((game_entity *)i->val, time_delta, entities, visual_effects, p);
    }

    for (list *i = *entities; i;)
    {
        if (((game_entity *)i->val)->health <= 0)
        {
            ENTITIES_DEF[((game_entity *)i->val)->type].death((game_entity *)i->val, entities, visual_effects);
            list *tmp = i;
            i = i->next;
            removeElementPtr(entities, tmp);
            continue;
        }
        i = i->next;
    }
}