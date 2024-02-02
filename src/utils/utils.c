#include "linkedList.h"
#include <SDL2/SDL.h>

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a > b)
        return b;
    return a;
}

int abs(int x)
{
    if (x < 0)
        return -x;
    return x;
}

int box_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    int x_check = (x2 >= x1 && x2 <= x1 + w1) || (x2 <= x1 && x1 <= x2 + w2);
    int y_check = (y2 >= y1 && y2 <= y1 + h1) || (y2 <= y1 && y1 <= y2 + h2);

    return x_check && y_check;
}

int hitbox_collision(int x1, int y1, list *h1, int x2, int y2, list *h2)
{
    for (list *i = h1; i; i = i->next)
    {
        for (list *j = h2; j; j = j->next)
        {
            if (box_collision(x1 + ((SDL_Rect *)i->val)->x, y1 + ((SDL_Rect *)i->val)->y,
                              ((SDL_Rect *)i->val)->w, ((SDL_Rect *)i->val)->h,
                              x2 + ((SDL_Rect *)j->val)->x, y2 + ((SDL_Rect *)j->val)->y,
                              ((SDL_Rect *)j->val)->w, ((SDL_Rect *)j->val)->h))
            {
                return 1;
            }
        }
    }

    return 0;
}