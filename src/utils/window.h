#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    int x, y;
    int w, h;
    char *text;
} button;

int init_window();
int free_window();
SDL_Window *get_window();
SDL_Renderer *get_renderer();
SDL_Surface *get_surface();
SDL_Texture *load_image(const char *filepath, SDL_Renderer *renderer);
int free_music();
int is_music_playing();
int start_music(char *filepath);
void render_button(button bt, SDL_Renderer *renderer, TTF_Font *button_font, SDL_Color fg_color, SDL_Color bg_color, SDL_Color rim_color);
void render_text_by_center(SDL_Renderer *renderer, TTF_Font *font, int x, int y, char *str, SDL_Color color);
void render_text_by_top_left(SDL_Renderer *renderer, TTF_Font *font, int x, int y, char *str, SDL_Color color);
int button_hover(button bt, int x, int y);

#endif