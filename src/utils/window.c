#include "../defines.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

SDL_Window *main_window = NULL;
SDL_Renderer *main_renderer = NULL;
SDL_Surface *main_surface = NULL;
Mix_Music *main_music = NULL;

int init_window()
{
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] running SDL_Init\n");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fprintf(stderr, "[ERR] SDL_Init failure\n%s\n", SDL_GetError());
        return 1;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] initializing main window\n");
    main_window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, 0);
    if (main_window == NULL)
    {
        fprintf(stderr, "[ERR] main window initialization error\n%s\n", SDL_GetError());
        return 1;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] creating main renderer\n");
    main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (main_renderer == NULL)
    {
        fprintf(stderr, "[ERR] couldn't create main renderer\n%s\n", SDL_GetError());
        SDL_DestroyWindow(main_window);
        main_window = NULL;
        return 1;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] getting main window surface\n");
    main_surface = SDL_GetWindowSurface(main_window);
    if (main_surface == NULL)
    {
        fprintf(stderr, "[ERR] couldn't get main window surface\n%s\n", SDL_GetError());
        SDL_DestroyRenderer(main_renderer);
        SDL_DestroyWindow(main_window);
        main_window = NULL;
        return 1;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] updating main window surface\n");
    if (SDL_UpdateWindowSurface(main_window) < 0)
    {
        fprintf(stderr, "[ERR] couldn't update main window surface\n%s\n", SDL_GetError());
        SDL_DestroyRenderer(main_renderer);
        SDL_DestroyWindow(main_window);
        main_window = NULL;
        return 1;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] initializing image\n");
    int imgflags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgflags) & imgflags))
    {
        fprintf(stderr, "[ERR] failed to initialize image\n%s\n", IMG_GetError());
        return 1;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] initializing fonts\n");
    if (TTF_Init())
    {
        fprintf(stderr, "[ERR] failed to initialize fonts\n%s\n", TTF_GetError());
        return 1;
    }

    return 0;
}

int free_window()
{
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] destroying main renderer\n");
    SDL_DestroyRenderer(main_renderer);
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] destroying main window\n");
    SDL_DestroyWindow(main_window);
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] calling SDL_Quit\n");
    SDL_Quit();

    return 0;
}

SDL_Texture *load_image(char *filepath, SDL_Renderer *renderer)
{
    SDL_Texture *ret = NULL;
    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] reading \"%s\" image input\n", filepath);
    SDL_Surface *surf = IMG_Load(filepath);
    if (surf == NULL)
    {
        fprintf(stderr, "[ERR] failed to read image file\n%s\n", SDL_GetError());
        return ret;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] making texture from image surface\n");
    ret = SDL_CreateTextureFromSurface(renderer, surf);
    if (ret == NULL)
    {
        fprintf(stderr, "[ERR] failed to convert surface to texture\n%s\n", SDL_GetError());
        SDL_FreeSurface(surf);
        return ret;
    }

    SDL_FreeSurface(surf);

    return ret;
}

SDL_Window *get_window()
{
    return main_window;
}

SDL_Renderer *get_renderer()
{
    return main_renderer;
}

SDL_Surface *get_surface()
{
    return main_surface;
}

int start_music(char *filepath)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "[ERR] couldn't initialize Mix_OpenAudio\n%s\n", Mix_GetError());
        return 1;
    }
    else
    {
        main_music = Mix_LoadMUS(filepath);
        if (main_music == NULL)
        {
            fprintf(stderr, "[ERR] couldn't load music\n%s\n", Mix_GetError());
            return 1;
        }
        else
        {
            Mix_PlayMusic(main_music, -1);
            return 0;
        }
    }
}

int is_music_playing()
{
    if (Mix_PlayingMusic() == 0)
    {
        return 0;
    }
    else
    {
        if (Mix_PausedMusic() == 1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int free_music()
{
    Mix_FreeMusic(main_music);
    main_music = NULL;
    return 0;
}

void render_button(button bt, SDL_Renderer *renderer, TTF_Font *button_font, SDL_Color fg_color, SDL_Color bg_color, SDL_Color rim_color)
{
    SDL_Rect bt_rect = {bt.x * WINDOW_SCALE, bt.y * WINDOW_SCALE, bt.w * WINDOW_SCALE, bt.h * WINDOW_SCALE};
    SDL_SetRenderDrawColor(renderer, rim_color.r, rim_color.g, rim_color.b, rim_color.a);
    SDL_RenderFillRect(renderer, &bt_rect);

    bt_rect = (SDL_Rect){(bt.x + BUTTON_RIM_WIDTH) * WINDOW_SCALE, (bt.y + BUTTON_RIM_WIDTH) * WINDOW_SCALE, (bt.w - 2 * BUTTON_RIM_WIDTH) * WINDOW_SCALE, (bt.h - 2 * BUTTON_RIM_WIDTH) * WINDOW_SCALE};
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderFillRect(renderer, &bt_rect);

    render_text_by_center(renderer, button_font, bt.x + bt.w / 2, bt.y + bt.h / 2, bt.text, fg_color);
}

void render_text_by_center(SDL_Renderer *renderer, TTF_Font *font, int x, int y, char *str, SDL_Color color)
{
    SDL_Surface *surface_message = TTF_RenderText_Solid(font, str, color);
    SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surface_message);

    int text_width, text_height;
    SDL_QueryTexture(message, NULL, NULL, &text_width, &text_height);

    SDL_Rect textpos_rect = {(x - text_width / 2) * WINDOW_SCALE, (y - text_height / 2) * WINDOW_SCALE, text_width * WINDOW_SCALE, text_height * WINDOW_SCALE};
    SDL_RenderCopy(renderer, message, NULL, &textpos_rect);

    SDL_FreeSurface(surface_message);
    SDL_DestroyTexture(message);
}

int button_hover(button bt, int x, int y)
{
    return (x / WINDOW_SCALE >= bt.x && x / WINDOW_SCALE <= bt.x + bt.w && y / WINDOW_SCALE >= bt.y && y / WINDOW_SCALE <= bt.y + bt.h);
}