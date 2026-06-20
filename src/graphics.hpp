#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <SDL.h>
#include "color.hpp"

/**
 * @brief Initialize the process-wide SDL window and renderer.
 * @param width Drawing width in pixels.
 * @param height Drawing height in pixels.
 * @return Zero on success or a negative SDL error code on failure.
 */
int create_graphics(int width, int height);

/** @brief Destroy the active renderer and window, then shut down SDL. */
void close_graphics();

/**
 * @brief Set the active window title to the supplied frame rate.
 * @param fps Frames per second to display.
 */
void update_title(float fps);

/** @brief Present all drawing submitted since the previous presentation. */
void update_graphics();

/** @return Configured drawing width in pixels. */
int getmaxx();

/** @return Configured drawing height in pixels. */
int getmaxy();

/**
 * @brief Set the colour used by subsequent drawing operations.
 * @param col RGB drawing colour.
 * @pre create_graphics() succeeded and has not been closed.
 */
void setcolor(const Color &col);

/** @brief Draw a line in screen coordinates using the current colour. */
void line(int x0, int y0, int x1, int y1);

/** @brief Fill an axis-aligned screen-space rectangle. */
void bar(int x0, int y0, int x1, int y1);

/**
 * @brief Fill a screen-space polygon as a triangle fan.
 * @param num_points Number of polygon vertices; must be at least three.
 * @param points Interleaved X/Y integer coordinates with 2 * num_points entries.
 */
void fillpoly(int num_points, int *points);

/** @brief Fill a screen-space triangle with horizontal scanlines. */
void drawtri(int x0, int y0, int x1, int y1, int x2, int y2);

#endif // __GRAPHICS_HPP__
