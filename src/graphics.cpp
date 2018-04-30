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

void drawtri_sorted(int x0, int y0, int x1, int y1, int x2, int y2)
{
	float dx0 = 0.0f;
	float dx1 = 0.0f;
	float dx2 = 0.0f;
	
	if ((y1-y0) > 0)
		dx0 = static_cast<float>(x1 - x0) / static_cast<float>(y1 - y0);
	if ((y2-y0) > 0)
		dx1 = static_cast<float>(x2 - x0) / static_cast<float>(y2 - y0);
	if ((y2-y1) > 0)
		dx2 = static_cast<float>(x2 - x1) / static_cast<float>(y2 - y1);

	float s_x = static_cast<float>(x0);
	int s_y = y0;
	float e_x = static_cast<float>(x0);
	int e_y = y0;

	if (dx0 > dx1)
	{
		for (; s_y < y1; s_y++, e_y++, s_x += dx1, e_x += dx0)
			line(static_cast<int>(s_x), s_y, static_cast<int>(e_x), s_y);
		e_x = static_cast<float>(x1);
		e_y = y1;
		for (; s_y <= y2; s_y++, e_y++, s_x += dx1, e_x += dx2)
			line(static_cast<int>(s_x), s_y, static_cast<int>(e_x), s_y);
	}
	else
	{
		for (; s_y < y1; s_y++, e_y++, s_x += dx0, e_x += dx1)
			line(static_cast<int>(s_x), s_y, static_cast<int>(e_x), s_y);
		s_x = static_cast<float>(x1);
		s_y = y1;
		for (; s_y <= y2; s_y++, e_y++, s_x += dx2, e_x += dx1)
			line(static_cast<int>(s_x), s_y, static_cast<int>(e_x), s_y);
	}
}

void drawtri(int x0, int y0, int x1, int y1, int x2, int y2)
{
	line(x0, y0, x1, y1);
	line(x1, y1, x2, y2);
	line(x2, y2, x0, y0);

	/*
	if (y0 > y1 && y0 > y2)
	{
		if (y1 > y2)
		{
			drawtri_sorted(x2, y2, x1, y1, x0, y0);
		}
		else
		{
			drawtri_sorted(x1, y1, x2, y2, x0, y0);
		}
	}
	else if (y1 > y0 && y1 > y2)
	{
		if (y0 > y2)
		{
			drawtri_sorted(x2, y2, x0, y0, x1, y1);
		}
		else
		{
			drawtri_sorted(x0, y0, x2, y2, x1, y1);
		}
	}
	else if (y2 > y0 && y2 > y1)
	{
		if (y0 > y1)
		{
			drawtri_sorted(x1, y1, x0, y0, x2, y2);
		}
		else
		{
			drawtri_sorted(x0, y0, x1, y1, x2, y2);
		}
	}
	*/
}
