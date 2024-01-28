#ifndef GAME_DRAW_H
#define GAME_DRAW_H

#include <SDL2/SDL.h>
#include "game_defines.h"
#include "../utils/linkedList.h"

int load_textures(SDL_Renderer *renderer);
void destroy_textures();
void draw(SDL_Renderer *renderer, int tiks, game_input_move gim, player p, list *entities);
int get_texture_width(int ind);
int get_texture_height(int ind);

#endif