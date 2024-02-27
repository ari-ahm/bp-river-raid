#include <SDL2/SDL.h>
#include "../defines.h"
#include "../utils/utils.h"
#include "../utils/linkedList.h"
#include "game_defines.h"
#include "game_draw.h"
#include "entities/entities.h"
#include "game_setup.h"

/*

TODO

bullet 1UP
particle system
music
score visual effect(like 1UP)
final boss

deploy textures using enum
*/

const struct ENTITY_CALLBACK ENTITIES_DEF[] = {
    {{0, 0, 0}, NULL, NULL, NULL, NULL, NULL},
    {{150, 300, 400}, mine_setup, mine_create, mine_update, mine_death, mine_draw},
    {{100, 200, 300}, cargo_setup, cargo_create, cargo_update, cargo_death, cargo_draw},
    {{50, 150, 250}, bomber_setup, bomber_create, bomber_update, bomber_death, bomber_draw},
    {{0, 0, 0}, bomber_bullet_setup, bomber_bullet_create, bomber_bullet_update, bomber_bullet_death, bomber_bullet_draw},
    {{50, 150, 250}, fighter_setup, fighter_create, fighter_update, fighter_death, fighter_draw},
    {{0, 0, 0}, fighter_bullet_setup, fighter_bullet_create, fighter_bullet_update, fighter_bullet_death, fighter_bullet_draw},
    {{50, 150, 350}, drone_setup, drone_create, drone_update, drone_death, drone_draw},
    {{20, 15, 10}, heart_setup, heart_create, heart_update, heart_death, heart_draw},
    {{20, 15, 10}, powerup_setup, powerup_create, powerup_update, powerup_death, powerup_draw}};

const int ENTITIES_DEF_SIZE = ARRAY_SIZE(ENTITIES_DEF);

long double tan_vals[4];

void update(int time_delta, game_input_move gim, player *p, list **entities, list **bullets, list **visual_effects, list *hitboxes[], int lvl);

int run_game(SDL_Renderer *renderer, int lvl)
{
    int current_frame, last_frame = SDL_GetTicks();
    if (load_textures(renderer))
    {
        return 0;
    }

    list *hitboxes[GAME_TEXTURES_PATH_SIZE];
    for (int i = 0; i < GAME_TEXTURES_PATH_SIZE; i++)
        hitboxes[i] = NULL;

    for (int i = 0; i < ENTITIES_DEF_SIZE; i++)
    {
        if (ENTITIES_DEF[i].setup)
            ENTITIES_DEF[i].setup(renderer, hitboxes);
    }

    calc_hitbox(renderer, textures[0], (SDL_Rect){0, 0, get_texture_width(0), get_texture_height(0)}, &hitboxes[0], 5, 1, -5);
    calc_hitbox(renderer, textures[9], (SDL_Rect){0, 0, get_texture_width(9), get_texture_height(9)}, &hitboxes[9], 5, 1, -5);

    tan_vals[0] = tan(3 * pi / 8);
    tan_vals[1] = tan(pi / 8);
    tan_vals[2] = tan(-pi / 8);
    tan_vals[3] = tan(-3 * pi / 8);

    game_input_move gim = {0, 0, 0, 0, 0};
    player p = {(WINDOW_WIDTH - get_texture_width(0)) / 2, WINDOW_HEIGHT * 5 / 6, 0, 0, get_texture_width(0), get_texture_height(0), 100, 0, 0, 0, 1, 1, 0};
    list *entities = NULL;
    list *bullets = NULL;
    list *visual_effects = NULL;

    while (1)
    {
        current_frame = SDL_GetTicks();
        int time_delta = max(current_frame - last_frame, FRAME_TIME);
        if (FRAME_TIME - (current_frame - last_frame) > 0 && (current_frame - last_frame) >= 0)
            SDL_Delay(FRAME_TIME - (current_frame - last_frame));
        last_frame = current_frame;

        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT || (ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_ESCAPE) || (ev.type == SDL_MOUSEBUTTONUP && p.health <= 0))
            {
                destroy_textures();
                while (entities)
                    removeElement(&entities, 0);
                while (bullets)
                    removeElement(&bullets, 0);
                while (visual_effects)
                    removeElement(&visual_effects, 0);
                for (int i = 0; i < GAME_TEXTURES_PATH_SIZE; i++)
                    while (hitboxes[i])
                        removeElement(&hitboxes[i], 0);
                return p.score;
            }
            else if (ev.type == SDL_KEYDOWN)
            {
                SDL_Keycode key = ev.key.keysym.sym;
                gim.u |= (key == SDLK_w || key == SDLK_UP);
                gim.l |= (key == SDLK_a || key == SDLK_LEFT);
                gim.d |= (key == SDLK_s || key == SDLK_DOWN);
                gim.r |= (key == SDLK_d || key == SDLK_RIGHT);
                gim.shoot |= (key == SDLK_c || key == SDLK_f || key == SDLK_SPACE || key == SDLK_RETURN || key == SDLK_RETURN2 || key == SDLK_KP_ENTER);
            }
            else if (ev.type == SDL_KEYUP)
            {
                SDL_Keycode key = ev.key.keysym.sym;
                gim.u &= !(key == SDLK_w || key == SDLK_UP);
                gim.l &= !(key == SDLK_a || key == SDLK_LEFT);
                gim.d &= !(key == SDLK_s || key == SDLK_DOWN);
                gim.r &= !(key == SDLK_d || key == SDLK_RIGHT);
                gim.shoot &= !(key == SDLK_c || key == SDLK_f || key == SDLK_SPACE || key == SDLK_RETURN || key == SDLK_RETURN2 || key == SDLK_KP_ENTER);
            }
        }

        update(time_delta, gim, &p, &entities, &bullets, &visual_effects, hitboxes, lvl);

        draw(renderer, current_frame, time_delta, gim, p, entities, bullets, &visual_effects, hitboxes);
    }
}