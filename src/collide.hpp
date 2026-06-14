#ifndef __COLLIDE_HPP__
#define __COLLIDE_HPP__

#include "defs.hpp"
#include "instance.hpp"
#include "viewer.hpp"

int hit_object(int mpos_x, int mpos_y, const Viewer &user, const std::vector<Instance> &instances);
bool check_col(float locx, float locy, float locz, const Viewer &user, const std::vector<Instance> &instances);

#endif // __COLLIDE_HPP__
