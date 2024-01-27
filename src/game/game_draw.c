#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_defines.h"
#include "../defines.h"
#include "../utils/window.h"


const char *GAME_TEXTURES_PATH[] = {
    "assets/main_ship_bases/full_health.png"
};

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

void __draw_anim(SDL_Renderer *renderer, SDL_Texture *txt, int x, int y, int frame, int frame_count)
{
    int w, h;
    SDL_QueryTexture(txt, NULL, NULL, &w, &h);

    SDL_Rect dest_rect = {
        x * WINDOW_SCALE,
        y * WINDOW_SCALE,
        w / frame_count * WINDOW_SCALE,
        h * WINDOW_SCALE
    };

    SDL_Rect src_rect = {
        w / frame_count * frame,
        0,
        w / frame_count,
        h
    };

    SDL_RenderCopy(renderer, txt, &src_rect, &dest_rect);
}

void draw(SDL_Renderer *renderer, player p)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    __draw_anim(renderer, textures[0], (int)p.x, (int)p.y, 0, 1);

    SDL_RenderPresent(renderer);
}