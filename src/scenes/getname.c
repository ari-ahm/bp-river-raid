#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../defines.h"
#include "../utils/window.h"
#include "../utils/utils.h"
#include "../utils/linkedList.h"
#include "../utils/recordHandler.h"

int getname(SDL_Renderer *renderer, char *buf)
{
    const int COSMIC_GLARE_COUNT = 5;
    const int COSMIC_GLARE_STEPS = 64;

    TTF_Font *button_font = TTF_OpenFont("assets/Minecraft.ttf", 24);
    TTF_Font *bottom_text_font = TTF_OpenFont("assets/Minecraft.ttf", 15);
    SDL_Texture *bg_image = load_image("assets/menu_pixelated.jpg", renderer);
    if (bg_image == NULL)
    {
        return -1;
    }

    SDL_Texture *cosmic_glare_texture = load_image("assets/cosmic_glare.bmp", renderer);
    int glare_x[COSMIC_GLARE_COUNT], glare_y[COSMIC_GLARE_COUNT], glare_alpha[COSMIC_GLARE_COUNT];
    for (int i = 0; i < COSMIC_GLARE_COUNT; i++)
    {
        glare_alpha[i] = rand() % 500;
        glare_x[i] = rand() % WINDOW_WIDTH;
        glare_y[i] = rand() % WINDOW_HEIGHT;
    }
    SDL_SetTextureBlendMode(cosmic_glare_texture, SDL_BLENDMODE_BLEND);
    int cosmic_glare_width, cosmic_glare_height;
    SDL_QueryTexture(cosmic_glare_texture, NULL, NULL, &cosmic_glare_width, &cosmic_glare_height);

    int last_frame = SDL_GetTicks();

    int ptr = 0;
    buf[0] = '\0';
    while (1)
    {
        int current_frame = SDL_GetTicks();
        int time_delta = max(current_frame - last_frame, FRAME_TIME);
        if (FRAME_TIME - (current_frame - last_frame) > 0 && (current_frame - last_frame) >= 0)
            SDL_Delay(FRAME_TIME - (current_frame - last_frame));
        last_frame = current_frame;

        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                SDL_DestroyTexture(cosmic_glare_texture);
                SDL_DestroyTexture(bg_image);
                TTF_CloseFont(button_font);
                TTF_CloseFont(bottom_text_font);
                return -1;
            }
            else if (ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_KP_ENTER || ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_RETURN2)
                {
                    SDL_DestroyTexture(cosmic_glare_texture);
                    SDL_DestroyTexture(bg_image);
                    TTF_CloseFont(button_font);
                    TTF_CloseFont(bottom_text_font);

                    if (ptr == 0)
                        strcpy(buf, "NO_NAME");

                    return 0;
                }
                else if (ev.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (ptr > 0)
                    {
                        buf[--ptr] = '\0';
                    }
                }
                else
                {
                    buf[ptr++] = (char)ev.key.keysym.sym;
                    buf[ptr] = '\0';
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_image, NULL, NULL);

        for (int i = 0; i < COSMIC_GLARE_COUNT; i++)
        {
            glare_alpha[i] += time_delta / 10;
            if (255 - abs(255 - glare_alpha[i]) < 0)
            {
                glare_alpha[i] = rand() % 50;
                glare_x[i] = rand() % WINDOW_WIDTH;
                glare_y[i] = rand() % WINDOW_HEIGHT;
            }
            SDL_SetTextureAlphaMod(cosmic_glare_texture, min((((255 - abs(255 - glare_alpha[i])) + COSMIC_GLARE_STEPS - 1) / COSMIC_GLARE_STEPS) * COSMIC_GLARE_STEPS, 255));
            SDL_Rect glare_dest = {glare_x[i] * WINDOW_SCALE, glare_y[i] * WINDOW_SCALE, cosmic_glare_width * WINDOW_SCALE, cosmic_glare_height * WINDOW_SCALE};
            SDL_RenderCopy(renderer, cosmic_glare_texture, NULL, &glare_dest);
        }

        char render_buf[60];
        sprintf(render_buf, "Name : %s", buf);
        render_text_by_center(renderer, button_font, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, render_buf, (SDL_Color){255, 255, 255, 255});

        render_text_by_center(renderer, bottom_text_font, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 10, "Mohammad Arian Ahmidi - FP Final Project \xA9", (SDL_Color){255, 255, 255, 255});

        SDL_RenderPresent(renderer);
    }
}