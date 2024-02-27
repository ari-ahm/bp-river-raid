#ifndef DEFINES_H
#define DEFINES_H

extern int WINDOW_HEIGHT;
extern int WINDOW_WIDTH;
#define WINDOW_SCALE 2
#define WINDOW_TITLE "River Raid"

#define GAME_SAVE ".riverraiddata"

#define FPS 30
#define FRAME_TIME 1000 / FPS

#define VERBOSE_ERR 0
#define DEBUG_HITBOX 0

#define ARRAY_SIZE(a) (int)(sizeof(a) / sizeof(a[0]))

#define BUTTON_RIM_WIDTH 5

#define FULLSCREEN 0

#endif
