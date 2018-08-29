#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "defs.hpp"
#include "instance.hpp"
#include "master.hpp"
#include "viewer.hpp"

Status screen_open(int mode);
void render(std::vector<Instance> &instances, Viewer &user);

#endif // __SCREEN_HPP__
