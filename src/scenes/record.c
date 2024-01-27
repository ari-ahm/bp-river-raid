#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../defines.h"
#include "../utils/window.h"
#include "../utils/utils.h"
#include "../utils/recordHandler.h"

int show_records(SDL_Renderer *renderer)
{
    const int BUTTON_COUNT = 7, BUTTON_HEIGHT = 60, BUTTON_WIDTH = 190, BUTTON_DIST = 5;
    button buttons[BUTTON_COUNT + 1];
    for (int i = 0; i < BUTTON_COUNT; i++) {
        buttons[i].x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
        buttons[i].y = (WINDOW_HEIGHT - BUTTON_COUNT * (BUTTON_HEIGHT + BUTTON_DIST) + BUTTON_DIST) / 2 + i * (BUTTON_HEIGHT + BUTTON_DIST);
        buttons[i].w = BUTTON_WIDTH;
        buttons[i].h = BUTTON_HEIGHT;
    }
    buttons[BUTTON_COUNT].x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    buttons[BUTTON_COUNT].y = (WINDOW_HEIGHT + BUTTON_COUNT * (BUTTON_HEIGHT + BUTTON_DIST) + 40) / 2;
    buttons[BUTTON_COUNT].w = BUTTON_WIDTH;
    buttons[BUTTON_COUNT].h = BUTTON_HEIGHT;
    buttons[BUTTON_COUNT].text = "Back";


    const int COSMIC_GLARE_COUNT = 5;

    const SDL_Color button_bg_default_color = {0x02, 0x3b, 0x59, 255};
    const SDL_Color button_bg_selected_color = {0x75, 0x50, 0x7b, 255};
    const SDL_Color button_white = {255, 255, 255, 255};
    const SDL_Color button_black = {0, 0, 0, 255};

    TTF_Font *button_font = TTF_OpenFont("assets/Minecraft.ttf", 24);
    TTF_Font *bottom_text_font = TTF_OpenFont("assets/Minecraft.ttf", 15);
    SDL_Texture *bg_image = load_image("assets/menu_pixelated.jpg", renderer);
    if (bg_image == NULL)
    {
        return -1;
    }

    record *records = loadRecords();

    for (int i = 0; i < BUTTON_COUNT; i++)
        buttons[i].text = records[i].time;

    SDL_Texture *cosmic_glare_texture = load_image("assets/cosmic_glare.bmp", renderer);
    int glare_x[COSMIC_GLARE_COUNT], glare_y[COSMIC_GLARE_COUNT], glare_alpha[COSMIC_GLARE_COUNT];
    for (int i = 0; i < COSMIC_GLARE_COUNT; i++) {
        glare_alpha[i] = rand() % 500;
        glare_x[i] = rand() % WINDOW_WIDTH;
        glare_y[i] = rand() % WINDOW_HEIGHT;
    }
    SDL_SetTextureBlendMode(cosmic_glare_texture, SDL_BLENDMODE_BLEND);
    int cosmic_glare_width, cosmic_glare_height;
    SDL_QueryTexture(cosmic_glare_texture, NULL, NULL, &cosmic_glare_width, &cosmic_glare_height);

    int last_frame = SDL_GetTicks();

    int selected_button = -1;
    int selected_click = -1;

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
                freeRecords(records);
                return -1;
            }
            else if (ev.type == SDL_MOUSEMOTION)
            {
                selected_button = -1;
                for (int i = 0; i < ARRAY_SIZE(buttons); i++)
                {
                    if (button_hover(buttons[i], ev.motion.x, ev.motion.y))
                    {
                        selected_button = i;
                    }
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN)
            {
                selected_click = -1;
                for (int i = 0; i < ARRAY_SIZE(buttons); i++)
                {
                    if (button_hover(buttons[i], ev.motion.x, ev.motion.y))
                    {
                        selected_click = i;
                    }
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONUP)
            {
                for (int i = 0; i < ARRAY_SIZE(buttons); i++)
                {
                    if (button_hover(buttons[i], ev.motion.x, ev.motion.y))
                    {
                        if (selected_click == i)
                        {
                            SDL_DestroyTexture(cosmic_glare_texture);
                            SDL_DestroyTexture(bg_image);
                            TTF_CloseFont(button_font);
                            TTF_CloseFont(bottom_text_font);
                            freeRecords(records);
                            return i;
                        }
                    }
                }
                selected_click = -1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_image, NULL, NULL);

        
        for (int i = 0; i < COSMIC_GLARE_COUNT; i++) {
            glare_alpha[i] += time_delta / 10;
            if (255 - abs(255 - glare_alpha[i]) < 0) {
                glare_alpha[i] = rand() % 50;
                glare_x[i] = rand() % WINDOW_WIDTH;
                glare_y[i] = rand() % WINDOW_HEIGHT;
            }
            SDL_SetTextureAlphaMod(cosmic_glare_texture, 255 - abs(255 - glare_alpha[i]));
            SDL_Rect glare_dest = {glare_x[i] * WINDOW_SCALE, glare_y[i] * WINDOW_SCALE, cosmic_glare_width * WINDOW_SCALE, cosmic_glare_height * WINDOW_SCALE};
            SDL_RenderCopy(renderer, cosmic_glare_texture, NULL, &glare_dest);
        }

        for (int i = 0; i < ARRAY_SIZE(buttons); i++)
        {
            if (selected_click == i)
                render_button(buttons[i], renderer, button_font, button_black, button_white, button_white);
            else if (selected_button == i)
                render_button(buttons[i], renderer, button_font, button_white, button_bg_selected_color, button_bg_selected_color);
            else
                render_button(buttons[i], renderer, button_font, (SDL_Color){255, 255, 255, 255}, button_bg_default_color, button_bg_default_color);
        }

        render_text_by_center(renderer, bottom_text_font, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 10, "Mohammad Arian Ahmidi - FP Final Project \xA9", button_white);

        SDL_RenderPresent(renderer);
    }
}