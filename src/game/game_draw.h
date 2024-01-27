#ifndef GAME_DRAW_H
#define GAME_DRAW_H

#include <SDL2/SDL.h>
#include "game_defines.h"

int load_textures(SDL_Renderer *renderer);
void destroy_textures();
void draw(SDL_Renderer *renderer, player p);

#endif