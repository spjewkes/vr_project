#ifndef __COLLIDE_HPP__
#define __COLLIDE_HPP__

#include "defs.hpp"

int hit_object(int mpos_x, int mpos_y, viewer &user, std::vector<instance> &instances);
bool check_col(float locx, float locy, float locz, viewer &user, std::vector<instance> &instances);

#endif // __COLLIDE_HPP__
