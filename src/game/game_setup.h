#ifndef GAME_SETUP_H
#define GAME_SETUP_H

#include "../utils/linkedList.h"
#include <SDL2/SDL.h>

void calc_hitbox(SDL_Renderer *renderer, SDL_Texture *txt, SDL_Rect rct, list **ans, int n, long double positive_score, long double negative_score);

#endif