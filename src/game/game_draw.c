#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "game_defines.h"
#include "../defines.h"
#include "../utils/window.h"
#include "../utils/linkedList.h"
#include "../utils/utils.h"

const char *GAME_TEXTURES_PATH[] = {
    "assets/main_ship/main_ship_bases/full_health.png",
    "assets/main_ship/main_ship_engine_effects/base_engine_idle.png",
    "assets/main_ship/main_ship_engine_effects/base_engine_powering.png",
    "assets/asteroids/asteroid1.png",
    "assets/asteroids/asteroid2.png",
    "assets/asteroids/asteroid3.png",
    "assets/asteroids/asteroid4.png",
    "assets/cargo_ship.png",
    "assets/main_ship/main_ship_engines/base_engine.png",
    "assets/bullet.png",
    "assets/bomber/bomber_base.png",
    "assets/bomber/bomber_bullet.png",
    "assets/bomber/bomber_destruction.png",
    "assets/bomber/bomber_engine_effect.png",
    "assets/fighter_jet/fighter_jet.png",
    "assets/fighter_jet/shot/shot5_asset.png",
    "assets/suicide_drone.png",
    "assets/heart.png"
};

long double pi = 3.141592653589793238462643383279502884197;
long double e = 2.7182818284590452353602874713526624977572;

long double tan_vals[4];

static SDL_Texture *textures[ARRAY_SIZE(GAME_TEXTURES_PATH)];
static SDL_Texture *effects[2];
static TTF_Font *font;
static int texture_dim[ARRAY_SIZE(GAME_TEXTURES_PATH)][2];

SDL_Texture *__gaussian_blur(SDL_Renderer *renderer, SDL_Texture *txt, int w, int h, long double sigma, long double mul);
list *background_stars;

int load_textures(SDL_Renderer *renderer)
{
    for (int i = 0; i < ARRAY_SIZE(GAME_TEXTURES_PATH); i++)
    {
        textures[i] = load_image(GAME_TEXTURES_PATH[i], renderer);
        if (textures[i] == NULL)
            return 1;
        SDL_QueryTexture(textures[i], NULL, NULL, &texture_dim[i][0], &texture_dim[i][1]);
    }
    effects[0] = __gaussian_blur(renderer, textures[9], 32, 32, 4, 3);
    effects[1] = __gaussian_blur(renderer, textures[15], 32, 32, 4, 3);
    font = TTF_OpenFont("assets/Minecraft.ttf", 24);

    tan_vals[0] = tan(3 * pi / 8);
    tan_vals[1] = tan(pi / 8);
    tan_vals[2] = tan(-pi / 8);
    tan_vals[3] = tan(-3 * pi / 8);

    for (int i = 0; i < 20; i++)
    {
        addElement(&background_stars, sizeof(visual_effect), 0);
        ((visual_effect *)background_stars->val)->type = rand() % 10000;
        ((visual_effect *)background_stars->val)->x = rand() % WINDOW_WIDTH;
        ((visual_effect *)background_stars->val)->y = rand() % WINDOW_HEIGHT;
        ((visual_effect *)background_stars->val)->xspeed = 0;
        ((visual_effect *)background_stars->val)->yspeed = 10;
        ((visual_effect *)background_stars->val)->texture = rand() % 3;
    }

    return 0;
}

void destroy_textures()
{
    for (int i = 0; i < ARRAY_SIZE(GAME_TEXTURES_PATH); i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    TTF_CloseFont(font);
    while (background_stars)
        removeElement(&background_stars, 0);
}

int get_texture_width(int ind)
{
    return texture_dim[ind][0];
}

int get_texture_height(int ind)
{
    return texture_dim[ind][1];
}

long double __gaussian_dist(long double sigma, long double x, long double y)
{
    return pow(e, -(x * x + y * y) / (2 * sigma * sigma)) / (2 * pi * sigma * sigma);
}

SDL_Texture *__gaussian_blur(SDL_Renderer *renderer, SDL_Texture *txt, int w, int h, long double sigma, long double mul)
{

    SDL_Texture *ret = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, ret);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(ret, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int txt_w, txt_h;
    SDL_QueryTexture(txt, NULL, NULL, &txt_w, &txt_h);

    SDL_Rect rct = {
        (w - txt_w) / 2,
        (h - txt_h) / 2,
        txt_w,
        txt_h};

    SDL_RenderCopy(renderer, txt, NULL, &rct);
    SDL_Color txt_px[w][h];
    SDL_Surface *srf = SDL_CreateRGBSurface(0, w, h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, srf->pixels, srf->pitch);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            SDL_GetRGBA(((Uint32 *)srf->pixels)[i + j * h], srf->format, &txt_px[i][j].r, &txt_px[i][j].g, &txt_px[i][j].b, &txt_px[i][j].a);
    SDL_FreeSurface(srf);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            long double tmp_r = 0;
            long double tmp_g = 0;
            long double tmp_b = 0;
            long double tmp_a = 0;
            for (int i2 = 0; i2 < w; i2++)
            {
                for (int j2 = 0; j2 < h; j2++)
                {
                    long double coef = __gaussian_dist(sigma, i2 - i, j2 - j);
                    tmp_r += txt_px[i2][j2].r * coef;
                    tmp_g += txt_px[i2][j2].g * coef;
                    tmp_b += txt_px[i2][j2].b * coef;
                    tmp_a += txt_px[i2][j2].a * coef;
                }
            }
            SDL_SetRenderDrawColor(renderer, min(255, tmp_r * mul), min(255, tmp_g * mul), min(255, tmp_b * mul), min(255, tmp_a * mul));
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return ret;
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

void __draw_anim2d(SDL_Renderer *renderer, SDL_Texture *txt, int x, int y, int i, int j, int w, int h)
{
    int txt_w, txt_h;
    SDL_QueryTexture(txt, NULL, NULL, &txt_w, &txt_h);

    SDL_Rect dest_rect = {
        x * WINDOW_SCALE,
        y * WINDOW_SCALE,
        txt_w / w * WINDOW_SCALE,
        txt_h / h * WINDOW_SCALE};

    SDL_Rect src_rect = {
        txt_w / w * i,
        txt_h / h * j,
        txt_w / w,
        txt_h / h};

    SDL_RenderCopy(renderer, txt, &src_rect, &dest_rect);
}

void __draw_health(SDL_Renderer *renderer, int x, int y, int w, int h, long double hl, SDL_Color fg, SDL_Color bg)
{
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_Rect rct = {
        x * WINDOW_SCALE,
        y * WINDOW_SCALE,
        w * WINDOW_SCALE,
        h * WINDOW_SCALE};
    SDL_RenderFillRect(renderer, &rct);
    SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, fg.a);
    rct.w = (int)(w * hl * WINDOW_SCALE);
    SDL_RenderFillRect(renderer, &rct);
}

void __draw_star(SDL_Renderer *renderer, int mode, int tiks, int x, int y)
{
    int tik_variant = (abs(80 - ((tiks / 50) % 160)) + 19) / 20 * 20;
    SDL_SetRenderDrawColor(renderer, tik_variant + 150 + (mode == 0 ? 20 : 0), tik_variant + 150 + (mode == 1 ? 20 : 0), tik_variant + 150 + (mode == 2 ? 20 : 0), 255);
    SDL_Rect rct = {
        (x + 2) * WINDOW_SCALE,
        (y + 1) * WINDOW_SCALE,
        1 * WINDOW_SCALE,
        3 * WINDOW_SCALE};
    SDL_RenderFillRect(renderer, &rct);
    rct = (SDL_Rect){
        (x + 1) * WINDOW_SCALE,
        (y + 2) * WINDOW_SCALE,
        3 * WINDOW_SCALE,
        1 * WINDOW_SCALE};
    SDL_RenderFillRect(renderer, &rct);

    if ((tiks / 1000) % 2)
    {
        rct = (SDL_Rect){
            x * WINDOW_SCALE,
            (y + 2) * WINDOW_SCALE,
            5 * WINDOW_SCALE,
            1 * WINDOW_SCALE};
        SDL_RenderFillRect(renderer, &rct);
    }
    else
    {
        rct = (SDL_Rect){
            (x + 2) * WINDOW_SCALE,
            y * WINDOW_SCALE,
            1 * WINDOW_SCALE,
            5 * WINDOW_SCALE};
        SDL_RenderFillRect(renderer, &rct);
    }
}

void __draw_background(SDL_Renderer *renderer, int tiks, int time_delta)
{
    list *i = background_stars;
    for (int i_cnt = 0; i; i_cnt++)
    {
        __draw_star(renderer, ((visual_effect *)i->val)->texture, ((visual_effect *)i->val)->type + tiks, ((visual_effect *)i->val)->x, ((visual_effect *)i->val)->y);
        ((visual_effect *)i->val)->y += ((visual_effect *)i->val)->yspeed * time_delta / 1000;
        if (((visual_effect *)i->val)->y > WINDOW_HEIGHT)
        {
            i = i->next;
            removeElement(&background_stars, i_cnt--);
            continue;
        }
        i = i->next;
    }

    if (rand() % 1000000 < 250 * time_delta)
    {
        addElement(&background_stars, sizeof(visual_effect), 0);
        ((visual_effect *)background_stars->val)->type = rand() % 10000;
        ((visual_effect *)background_stars->val)->x = rand() % WINDOW_WIDTH;
        ((visual_effect *)background_stars->val)->y = -5;
        ((visual_effect *)background_stars->val)->xspeed = 0;
        ((visual_effect *)background_stars->val)->yspeed = 10;
        ((visual_effect *)background_stars->val)->texture = rand() % 3;
    }
}

void draw(SDL_Renderer *renderer, int tiks, int time_delta, game_input_move gim, player p, list *entities, list *bullets)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    __draw_background(renderer, tiks, time_delta);

    for (list *i = bullets; i; i = i->next)
    {
        __draw_anim(renderer, effects[0], (int)((bullet *)i->val)->x - (32 - get_texture_width(9)) / 2, (int)((bullet *)i->val)->y - (32 - get_texture_height(9)) / 2, 0, 1);
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
        case 3:
            __draw_anim(renderer, textures[13], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, ((tiks + ((game_entity *)i->val)->rnd) / 200) % 8, 8);
            __draw_anim(renderer, textures[10], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
            break;
        case 4:
            __draw_anim(renderer, textures[11], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, ((tiks + ((game_entity *)i->val)->rnd) / 200) % 8, 8);
            break;
        case 5:
            __draw_anim(renderer, textures[14], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
            break;
        case 6:
            __draw_anim(renderer, effects[1], (int)((game_entity *)i->val)->x - (32 - get_texture_width(15)) / 2, (int)((game_entity *)i->val)->y - (32 - get_texture_height(15)) / 2, 0, 1);
            __draw_anim(renderer, textures[15], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
            break;
        case 7:
            int dir;
            long double dx = p.x - ((game_entity *)i->val)->x;
            long double dy = p.y - ((game_entity *)i->val)->y;
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
                for (dir = 0; nsb < tan_vals[dir] && nsb < 3; dir++)
                    ;
                if (dx < 0)
                    dir += 4;
                dir %= 8;
            }
            __draw_anim2d(renderer, textures[16], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, ((tiks + ((game_entity *)i->val)->rnd) / 200) % 6, dir, 6, 8);
            break;
        case 8:
            __draw_anim(renderer, textures[17], (int)((game_entity *)i->val)->x, (int)((game_entity *)i->val)->y, 0, 1);
            break;
        }

        if (((game_entity *)i->val)->health != ((game_entity *)i->val)->max_health && ((game_entity *)i->val)->type != 4 && ((game_entity *)i->val)->type != 6 && ((game_entity *)i->val)->type != 8)
        {
            int w = ((game_entity *)i->val)->max_health;
            __draw_health(renderer, ((game_entity *)i->val)->x + (((game_entity *)i->val)->w - w) / 2, ((game_entity *)i->val)->y - 10, w, 4, (long double)max(0, ((game_entity *)i->val)->health) / ((game_entity *)i->val)->max_health, (SDL_Color){0, 255, 0, 255}, (SDL_Color){255, 0, 0, 255});
        }
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

    __draw_health(
        renderer, p.x + (p.w - 100) / 2,
        p.y + get_texture_height(0) + 10, 100, 4,
        (long double)p.health / 100,
        (SDL_Color){0, 128, 255, 255},
        (SDL_Color){0x02, 0x3b, 0x59, 255});

    SDL_RenderPresent(renderer);
}