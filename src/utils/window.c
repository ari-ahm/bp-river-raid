#include "../defines.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

SDL_Window *main_window = NULL;
SDL_Renderer *main_renderer = NULL;
SDL_Surface *main_surface = NULL;

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
    if (surf == NULL) {
        fprintf(stderr, "[ERR] failed to read image file\n%s\n", SDL_GetError());
        return ret;
    }

    if (VERBOSE_ERR)
        fprintf(stderr, "[INF] making texture from image surface\n");
    ret = SDL_CreateTextureFromSurface(renderer, surf);
    if (ret == NULL) {
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