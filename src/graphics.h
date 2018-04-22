#include <SDL2/SDL.h>

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define XOR_PUT  (0)
#define COPY_PUT (1)
#define SOLID_FILL (2)

#define WHITE (1)

int create_graphics(int width, int height);
void close_graphics();

int getmaxx();
int getmaxy();

void setcolor(int);
void line(int, int, int, int);
void setwritemode(int);
void setrgbpalette(int, int, int, int);
void setfillstyle(int, int);
void bar(int, int, int, int);
void fillpoly(int, int *);

#endif // __GRAPHICS_H__
