#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../defines.h"
#include "../utils/window.h"
#include "../utils/utils.h"
#include "../utils/linkedList.h"
#include "../utils/recordHandler.h"

int show_records(SDL_Renderer *renderer)
{
    const int RECORD_COUNT = 7, RECORD_HEIGHT = 60, RECORD_WIDTH = 190, RECORD_DIST = 5;
    const int MOUSE_WHEEL_SENSIVITY = 1;

    button button_back;
    button_back.x = (WINDOW_WIDTH - RECORD_WIDTH) / 2;
    button_back.y = (WINDOW_HEIGHT + RECORD_COUNT * (RECORD_HEIGHT + RECORD_DIST) + 40) / 2;
    button_back.w = RECORD_WIDTH;
    button_back.h = RECORD_HEIGHT;
    button_back.text = "Back";

    const int COSMIC_GLARE_COUNT = 5;
    const int COSMIC_GLARE_STEPS = 64;

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

    list *records = loadRecords();
    int records_cnt = 0;
    for (list *i = records; i; i = i->next)
        records_cnt++;

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

    int selected_button = -1;
    int selected_click = -1;
    int mouse_wheel_traversal = 0;

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
                while (records)
                    removeElement(&records, 0);
                return -1;
            }
            else if (ev.type == SDL_MOUSEMOTION)
            {
                selected_button = -1;
                if (button_hover(button_back, ev.motion.x, ev.motion.y))
                {
                    selected_button = 1;
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN)
            {
                selected_click = -1;
                if (button_hover(button_back, ev.motion.x, ev.motion.y))
                {
                    selected_click = 1;
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONUP)
            {
                if (button_hover(button_back, ev.motion.x, ev.motion.y))
                {
                    if (selected_click == 1)
                    {
                        SDL_DestroyTexture(cosmic_glare_texture);
                        SDL_DestroyTexture(bg_image);
                        TTF_CloseFont(button_font);
                        TTF_CloseFont(bottom_text_font);
                        while (records)
                            removeElement(&records, 0);
                        return 0;
                    }
                }
                selected_click = -1;
            }
            else if (ev.type == SDL_MOUSEWHEEL)
            {
                mouse_wheel_traversal -= ev.wheel.y;
                mouse_wheel_traversal = max(0, mouse_wheel_traversal);
                mouse_wheel_traversal = max(0, min(mouse_wheel_traversal, MOUSE_WHEEL_SENSIVITY * (records_cnt - RECORD_COUNT + 1) - 1));
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


        list *cur = getElement(records, mouse_wheel_traversal / MOUSE_WHEEL_SENSIVITY);
        for (int i = 0; i < RECORD_COUNT && cur; i++, cur = cur->next)
        {
            char buf[100];
            sprintf(buf, "%s - %s - %s - %d - %d", ((record *)cur->val)->time, ((char*[]){"EASY", "MEDIUM", "HARD"})[((record *)cur->val)->lvl], ((record *)cur->val)->name, ((record *)cur->val)->score, ((record *)cur->val)->dur);
            render_text_by_center(renderer, button_font, WINDOW_WIDTH / 2, (WINDOW_HEIGHT - RECORD_COUNT * (RECORD_HEIGHT + RECORD_DIST) - RECORD_DIST) / 2 + i * (RECORD_HEIGHT + RECORD_DIST), buf, (SDL_Color){255, 255, 255, 255});
        }

        if (selected_click == 1)
            render_button(button_back, renderer, button_font, button_black, button_white, button_white);
        else if (selected_button == 1)
            render_button(button_back, renderer, button_font, button_white, button_bg_selected_color, button_bg_selected_color);
        else
            render_button(button_back, renderer, button_font, (SDL_Color){255, 255, 255, 255}, button_bg_default_color, button_bg_default_color);

        render_text_by_center(renderer, bottom_text_font, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 10, "Mohammad Arian Ahmidi - FP Final Project \xA9", button_white);

        SDL_RenderPresent(renderer);
    }
}