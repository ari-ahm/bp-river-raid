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
    "assets/asteroids/asteroid4.png",
    "assets/cargo_ship.png",
    "assets/main_ship_engines/base_engine.png",
    "assets/bullet.png"};

static SDL_Texture *textures[ARRAY_SIZE(GAME_TEXTURES_PATH)];
static TTF_Font *font;
static int texture_dim[ARRAY_SIZE(GAME_TEXTURES_PATH)][2];

int load_textures(SDL_Renderer *renderer)
{
    for (int i = 0; i < ARRAY_SIZE(GAME_TEXTURES_PATH); i++)
    {
        textures[i] = load_image(GAME_TEXTURES_PATH[i], renderer);
        if (textures[i] == NULL)
            return 1;
        SDL_QueryTexture(textures[i], NULL, NULL, &texture_dim[i][0], &texture_dim[i][1]);
    }
    font = TTF_OpenFont("assets/Minecraft.ttf", 24);
    return 0;
}

void destroy_textures()
{
    for (int i = 0; i < ARRAY_SIZE(GAME_TEXTURES_PATH); i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    TTF_CloseFont(font);
}

int get_texture_width(int ind)
{
    return texture_dim[ind][0];
}

int get_texture_height(int ind)
{
    return texture_dim[ind][1];
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

void draw(SDL_Renderer *renderer, int tiks, game_input_move gim, player p, list *entities, list *bullets)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (list *i = bullets; i; i = i->next)
    {
        __draw_anim(renderer, textures[9], (int)((bullet *)i->val)->x, (int)((bullet *)i->val)->y, 0, 1);
    }

    for (list *i = entities; i; i = i->next)
    {
        switch (((game_entity *)i->val)->type)
        {
        case 1:
            __draw_anim(renderer, textures[3 + ((game_entity *)i->val)->texture], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
            break;
        case 2:
            __draw_anim(renderer, textures[7], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
            break;
        }

        // if (((game_entity *)i->val)->health != ((game_entity *)i->val)->max_health)
        // {

        // }
    }

    if (p.invincible && (p.invincible / 500) % 2)
    {
        if (gim.u)
        {
            SDL_SetTextureAlphaMod(textures[2], 128);
            __draw_anim(renderer, textures[2], (int)p.x, (int)p.y, (tiks / 200) % 4, 4);
            SDL_SetTextureAlphaMod(textures[2], 255);
        }
        else
        {
            SDL_SetTextureAlphaMod(textures[1], 128);
            __draw_anim(renderer, textures[1], (int)p.x, (int)p.y, (tiks / 200) % 3, 3);
            SDL_SetTextureAlphaMod(textures[1], 255);
        }
        SDL_SetTextureAlphaMod(textures[0], 127);
        __draw_anim(renderer, textures[0], (int)p.x, (int)p.y, 0, 1);
        SDL_SetTextureAlphaMod(textures[0], 255);

        SDL_SetTextureAlphaMod(textures[8], 127);
        __draw_anim(renderer, textures[8], (int)p.x, (int)p.y, 0, 1);
        SDL_SetTextureAlphaMod(textures[8], 255);
    }
    else
    {
        if (gim.u)
            __draw_anim(renderer, textures[2], (int)p.x, (int)p.y, (tiks / 200) % 4, 4);
        else
            __draw_anim(renderer, textures[1], (int)p.x, (int)p.y, (tiks / 200) % 3, 3);
        __draw_anim(renderer, textures[0], (int)p.x, (int)p.y, 0, 1);
        __draw_anim(renderer, textures[8], (int)p.x, (int)p.y, 0, 1);
    }

    char health[20];
    sprintf(health, "Health : %d", p.health);
    render_text_by_center(renderer, font, 100, 40, health, (SDL_Color){255, 255, 255, 255});

    SDL_RenderPresent(renderer);
}