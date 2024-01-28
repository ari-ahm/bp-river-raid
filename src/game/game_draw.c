#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_defines.h"
#include "../defines.h"
#include "../utils/window.h"
#include "../utils/linkedList.h"

const char *GAME_TEXTURES_PATH[] = {
    "assets/main_ship_bases/full_health.png",
    "assets/main_ship_engine_effects/base_engine_idle.png",
    "assets/main_ship_engine_effects/base_engine_powering.png",
    "assets/asteroids/asteroid1.png",
    "assets/asteroids/asteroid2.png",
    "assets/asteroids/asteroid3.png",
    "assets/asteroids/asteroid4.png"};

static SDL_Texture *textures[ARRAY_SIZE(GAME_TEXTURES_PATH)];

int load_textures(SDL_Renderer *renderer)
{
    for (int i = 0; i < ARRAY_SIZE(GAME_TEXTURES_PATH); i++)
    {
        textures[i] = load_image(GAME_TEXTURES_PATH[i], renderer);
        if (textures[i] == NULL)
            return 1;
    }
    return 0;
}

void destroy_textures()
{
    for (int i = 0; i < ARRAY_SIZE(GAME_TEXTURES_PATH); i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
}

int get_texture_width(int ind)
{
    int w, h;
    SDL_QueryTexture(textures[ind], NULL, NULL, &w, &h);

    return w;
}

int get_texture_height(int ind)
{
    int w, h;
    SDL_QueryTexture(textures[ind], NULL, NULL, &w, &h);

    return h;
}

void __draw_anim(SDL_Renderer *renderer, SDL_Texture *txt, int x, int y, int frame, int frame_count)
{
    int w, h;
    SDL_QueryTexture(txt, NULL, NULL, &w, &h);

    SDL_Rect dest_rect = {
        x * WINDOW_SCALE,
        y * WINDOW_SCALE,
        w / frame_count * WINDOW_SCALE,
        h * WINDOW_SCALE};

    SDL_Rect src_rect = {
        w / frame_count * frame,
        0,
        w / frame_count,
        h};

    SDL_RenderCopy(renderer, txt, &src_rect, &dest_rect);
}

void draw(SDL_Renderer *renderer, int tiks, game_input_move gim, player p, list *entities)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (list *i = entities; i; i = i->next)
    {
        if (((game_entity *)i->val)->type == 1)
        {
            __draw_anim(renderer, textures[3 + ((game_entity *)i->val)->texture], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
        }
    }

    if (gim.u)
        __draw_anim(renderer, textures[2], (int)p.x, (int)p.y, (tiks / 200) % 4, 4);
    else
        __draw_anim(renderer, textures[1], (int)p.x, (int)p.y, (tiks / 200) % 3, 3);

    __draw_anim(renderer, textures[0], (int)p.x, (int)p.y, 0, 1);

    SDL_RenderPresent(renderer);
}