#include <cassert>
#include <algorithm>
#include "graphics.hpp"

static int g_width = 0;
static int g_height = 0;
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_renderer = NULL;

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

void update_title(float fps)
{
	char title[256];
	sprintf(title, "FPS: %4.2f", fps);
	SDL_SetWindowTitle(g_window, title);
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

void setcolor(Color &col)
{
	int retcode = SDL_SetRenderDrawColor(g_renderer, col.r(), col.g(), col.b(), SDL_ALPHA_OPAQUE);
	assert(retcode == 0);
}

void line(int x0, int y0, int x1, int y1)
{
	int retcode = SDL_RenderDrawLine(g_renderer, x0, y0, x1, y1);
	assert(retcode == 0);
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

void filltrapezoid(int y_top, int y_bottom, int x0_top, int x1_top, int x0_bottom, int x1_bottom)
{
	float x0 = static_cast<float>(x0_top);
	float dx0 = static_cast<float>(x0_bottom-x0_top)/static_cast<float>(y_bottom-y_top);
	float x1 = static_cast<float>(x1_top);
	float dx1 = static_cast<float>(x1_bottom-x1_top)/static_cast<float>(y_bottom-y_top);

	for (int y=y_top; y<=y_bottom; y++)
	{
		line(static_cast<int>(x0), y, static_cast<int>(x1), y);
		x0 += dx0;
		x1 += dx1;
	}
}

void drawtri(int x0, int y0, int x1, int y1, int x2, int y2)
{
	int y[3];
	int x[3];

	if (y0 <= y1 && y0 <= y2)
	{
		x[0] = x0;
		y[0] = y0;
		
		if (y1 <= y2)
		{
			x[1] = x1; y[1] = y1; x[2] = x2; y[2] = y2;
		}
		else
		{
			x[1] = x2; y[1] = y2; x[2] = x1; y[2] = y1;
		}
	}
	else if (y1 <= y0 && y1 <= y2)
	{
		x[0] = x1;
		y[0] = y1;
		
		if (y0 <= y2)
		{
			x[1] = x0; y[1] = y0; x[2] = x2; y[2] = y2;
		}
		else
		{
			x[1] = x2; y[1] = y2; x[2] = x0; y[2] = y0;
		}
	}
	else
	{
		x[0] = x2;
		y[0] = y2;
		
		if (y0 <= y1)
		{
			x[1] = x0; y[1] = y0; x[2] = x1; y[2] = y1;
		}
		else
		{
			x[1] = x1; y[1] = y1; x[2] = x0; y[2] = y0;
		}
	}

	if (y[0] == y[1])
	{
		filltrapezoid(y[0], y[2], x[0], x[1], x[2], x[2]);
	}
	else if (y[1] == y[2])
	{
		filltrapezoid(y[0], y[1], x[0], x[0], x[1], x[2]);
	}
	else
	{
		float t = static_cast<float>(y[1] - y[0]) / static_cast<float>(y[2] - y[0]);
		int new_x = x[0] + static_cast<int>(static_cast<float>(x[2] - x[0]) * t);

		filltrapezoid(y[0], y[1], x[0], x[0], new_x, x[1]);
		filltrapezoid(y[1], y[2], new_x, x[1], x[2], x[2]);

	}
}
