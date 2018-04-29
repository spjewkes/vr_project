#include <cassert>
#include <algorithm>
#include "graphics.h"

static int g_width = 0;
static int g_height = 0;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_renderer = NULL;

static int g_palette[256][3];

int create_graphics(int width, int height)
{
	int retcode = SDL_Init(SDL_INIT_VIDEO);
	if (retcode < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return retcode;
	}

	retcode = SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_MAXIMIZED, &g_window, &g_renderer);
	if (retcode < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return retcode;
	}

	g_width = width;
	g_height = height;

	return retcode;
}

void close_graphics()
{
	SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);

    SDL_Quit();
}

void update_graphics()
{
	SDL_RenderPresent(g_renderer);
}

int getmaxx()
{
	return g_width;
}

int getmaxy()
{
	return g_height;
}

void setcolor(int index)
{
	assert(index >= 0 && index < 256);
	int retcode = SDL_SetRenderDrawColor(g_renderer, g_palette[index][0],
										 g_palette[index][1], g_palette[index][2],
										 SDL_ALPHA_OPAQUE);
	assert(retcode == 0);
}

void line(int x0, int y0, int x1, int y1)
{
	int retcode = SDL_RenderDrawLine(g_renderer, x0, y0, x1, y1);
	assert(retcode == 0);
}

void setrgbpalette(int index, int r, int g, int b)
{
	assert(index >= 0 && index < 256);
	assert(r >= 0 && r < 256);
	assert(g >= 0 && g < 256);
	assert(b >= 0 && b < 256);
	g_palette[index][0] = r;
	g_palette[index][1] = g;
	g_palette[index][2] = b;
}

void bar(int x0, int y0, int x1, int y1)
{
	SDL_Rect rect = { std::min(x0, x1), std::min(y0, y1), abs(x0 - x1), abs(y0 - y1) };
	int retcode = SDL_RenderFillRect(g_renderer, &rect);
	assert(retcode == 0);
}

void fillpoly(int num_points, int *points)
{
	assert(num_points > 2);
	int length = num_points * 2;
	for (int i = 2; i < length; i += 2)
	{
		drawtri(points[0], points[1],
				points[i-2], points[i-1],
				points[i], points[i+1]);
	}
}

void drawtri(int x0, int y0, int x1, int y1, int x2, int y2)
{
	line(x0, y0, x1, y1);
	line(x1, y1, x2, y2);
	line(x2, y2, x0, y0);
}
