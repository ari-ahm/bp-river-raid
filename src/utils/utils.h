#ifndef UTILS_H
#define UTILS_H

#include "linkedList.h"

int min(int a, int b);
int max(int a, int b);
int abs(int x);
int box_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int hitbox_collision(int x1, int y1, list *h1, int x2, int y2, list *h2);

#endif