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