#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <SDL2/SDL.h>
#include "color.hpp"

int create_graphics(int width, int height);
void close_graphics();

void update_title(float fps);

void update_graphics();

int getmaxx();
int getmaxy();

void setcolor(Color &col);
void line(int x0, int y0, int x1, int y1);
void setrgbpalette(int index, int r, int g, int b);
void bar(int x0, int y0, int x1, int y1);
void fillpoly(int num_points, int *points);
void drawtri(int x0, int y0, int x1, int y1, int x2, int y2);

#endif // __GRAPHICS_HPP__
