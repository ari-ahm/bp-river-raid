#include <SDL2/SDL.h>
#include "../defines.h"
#include "../utils/utils.h"
#include "game_defines.h"
#include "game_draw.h"

void update(int time_delta, game_input_move gim, player *p);

int run_game(SDL_Renderer *renderer, int lvl)
{
    int current_frame, last_frame = SDL_GetTicks();
    if (load_textures(renderer))
    {
        return 0;
    }
    game_input_move gim = {0, 0, 0, 0, 0};
    player p = {0, 0, 0, 0}; // fix
    while (1)
    {
        current_frame = SDL_GetTicks();
        int time_delta = max(current_frame - last_frame, FRAME_TIME);
        if (FRAME_TIME - (current_frame - last_frame) > 0 && (current_frame - last_frame) >= 0)
            SDL_Delay(FRAME_TIME - (current_frame - last_frame));
        last_frame = current_frame;

        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                destroy_textures();
                return 0;
            }
            else if (ev.type == SDL_KEYDOWN)
            {
                SDL_Keycode key = ev.key.keysym.sym;
                gim.u |= (key == SDLK_w || key == SDLK_UP);
                gim.l |= (key == SDLK_a || key == SDLK_LEFT);
                gim.d |= (key == SDLK_s || key == SDLK_DOWN);
                gim.r |= (key == SDLK_d || key == SDLK_RIGHT);
                gim.shoot |= (key == SDLK_c || key == SDLK_f || key == SDLK_SPACE || key == SDLK_RETURN || key == SDLK_RETURN2 || key == SDLK_KP_ENTER);
            }
            else if (ev.type == SDL_KEYUP)
            {
                SDL_Keycode key = ev.key.keysym.sym;
                gim.u &= !(key == SDLK_w || key == SDLK_UP);
                gim.l &= !(key == SDLK_a || key == SDLK_LEFT);
                gim.d &= !(key == SDLK_s || key == SDLK_DOWN);
                gim.r &= !(key == SDLK_d || key == SDLK_RIGHT);
                gim.shoot &= !(key == SDLK_c || key == SDLK_f || key == SDLK_SPACE || key == SDLK_RETURN || key == SDLK_RETURN2 || key == SDLK_KP_ENTER);
            }
        }

        update(time_delta, gim, &p);

        draw(renderer, p);
    }
}