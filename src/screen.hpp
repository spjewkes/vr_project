#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "defs.hpp"

Status screen_open(int mode);
void draw_image(std::vector<master> &mptr, std::vector<instance> &iptr, viewer user);

#endif // __SCREEN_HPP__
