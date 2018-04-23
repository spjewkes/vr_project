#include <SDL2/SDL.h>

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define XOR_PUT  (0)
#define COPY_PUT (1)
#define SOLID_FILL (2)

#define WHITE (1)

int create_graphics(int width, int height);
void close_graphics();

void update_graphics();

int getmaxx();
int getmaxy();

void setcolor(int index);
void line(int x0, int y0, int x1, int y1);
void setrgbpalette(int index, int r, int g, int b);
void bar(int x0, int y0, int x1, int y1);
void fillpoly(int num_points, int *points);

#endif // __GRAPHICS_H__
