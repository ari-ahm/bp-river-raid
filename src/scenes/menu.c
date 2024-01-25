#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../defines.h"
#include "../utils/window.h"
#include "../utils/utils.h"

typedef struct
{
    int x, y;
    int w, h;
    char *text;
} button;

void render_button(button bt, SDL_Renderer *renderer, TTF_Font *button_font, SDL_Color fg_color, SDL_Color bg_color, SDL_Color rim_color);
void render_text_by_center(SDL_Renderer *renderer, TTF_Font *font, int x, int y, char *str, SDL_Color color);
int button_hover(button bt, int x, int y);

int show_menu(SDL_Renderer *renderer)
{
    const button buttons[4] = {
        {WINDOW_WIDTH - 200, WINDOW_HEIGHT - 320, 190, 60, "Play!"},
        {WINDOW_WIDTH - 200, WINDOW_HEIGHT - 250, 190, 60, "Level"},
        {WINDOW_WIDTH - 200, WINDOW_HEIGHT - 180, 190, 60, "Records"},
        {WINDOW_WIDTH - 200, WINDOW_HEIGHT - 110, 190, 60, "Exit"}};
    
    const int COSMIC_GLARE_COUNT = 5;

    Mix_Music *music = NULL;    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        fprintf(stderr, "[ERR] couldn't initialize Mix_OpenAudio\n%s\n", Mix_GetError());
    else {
        music = Mix_LoadMUS("assets/intro.mp3");
        if (music == NULL) {
            fprintf(stderr, "[ERR] couldn't load music\n%s\n", Mix_GetError());
        }
        else
        {
            Mix_PlayMusic(music, -1);
        }
    }

    const SDL_Color button_bg_default_color = {0x02, 0x3b, 0x59, 255};
    const SDL_Color button_bg_selected_color = {0x75, 0x50, 0x7b, 255};
    const SDL_Color button_white = {255, 255, 255, 255};
    const SDL_Color button_black = {0, 0, 0, 255};

    TTF_Font *button_font = TTF_OpenFont("assets/Minecraft.ttf", 24);
    TTF_Font *bottom_text_font = TTF_OpenFont("assets/Minecraft.ttf", 15);
    SDL_Texture *bg_image = load_image("assets/menu_pixelated.jpg", renderer);
    if (bg_image == NULL)
    {
        return 3;
    }

    srand(time(NULL));

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
                Mix_FreeMusic(music);
                return 3;
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
                            Mix_FreeMusic(music);
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