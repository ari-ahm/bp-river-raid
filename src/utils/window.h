#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

int init_window();
int free_window();
SDL_Window *get_window();
SDL_Renderer *get_renderer();
SDL_Surface *get_surface();
SDL_Texture *load_image(char *filepath, SDL_Renderer *renderer);

#endif