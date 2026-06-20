#ifndef __MOUSE_HPP__
#define __MOUSE_HPP__

int mouse_info(int *x, int *y);

/** @brief Draw the software crosshair centred at a screen-space position. */
void draw_pointer(int x, int y);

/** @brief Clamp a screen-space pointer position so the crosshair remains visible. */
void check_pointer(int *x, int *y);

#endif // __MOUSE_HPP__
