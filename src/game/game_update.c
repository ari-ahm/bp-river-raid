#include "game_defines.h"
#include <stdio.h>

void update(int time_delta, game_input_move gim, player *p)
{
    const long double acceleration = 150;
    const long double drag_coef = 0.05;

    long double xdrag = p->xspeed * p->xspeed * drag_coef;
    long double ydrag = p->yspeed * p->yspeed * drag_coef;

    long double xacc = (gim.r * acceleration + gim.l * -acceleration + (p->xspeed > 0 ? -1 : 1) * xdrag);
    long double yacc = (gim.d * acceleration + gim.u * -acceleration + (p->yspeed > 0 ? -1 : 1) * ydrag);

    p->xspeed += (xacc * time_delta / 1000);
    p->yspeed += (yacc * time_delta / 1000);

    p->x += (p->xspeed * time_delta / 1000);
    p->y += (p->yspeed * time_delta / 1000);

}