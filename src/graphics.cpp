#include "graphics.h"

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

int getmaxx()
{
	return g_width;
}

int getmaxy()
{
	return g_height;
}


const char *grapherrormsg(int)
{
	return NULL;
}

void setcolor(int)
{
}

void line(int, int, int, int)
{
}

void setwritemode(int)
{
}

void setrgbpalette(int, int, int, int)
{
}

void setfillstyle(int, int)
{
}

void bar(int, int, int, int)
{
}

void fillpoly(int, int *)
{
}

