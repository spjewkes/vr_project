#ifndef __COLLIDE_HPP__
#define __COLLIDE_HPP__

#include "defs.hpp"
#include "instance.hpp"
#include "viewer.hpp"

/**
 * @brief Find the first solid instance intersected by the pointer ray.
 * @param mpos_x Pointer X coordinate in screen pixels.
 * @param mpos_y Pointer Y coordinate in screen pixels.
 * @param user Viewer used to construct the world-space ray.
 * @param instances Instances tested through their axis-aligned bounds.
 * @return Zero-based instance index, or -1 when nothing is hit.
 * @pre Graphics is initialized so the screen dimensions are available.
 */
int hit_object(int mpos_x, int mpos_y, const Viewer &user, const std::vector<Instance> &instances);

/**
 * @brief Test a candidate viewer position against solid instance bounds.
 * @param locx Candidate world-space X coordinate.
 * @param locy Candidate world-space Y coordinate.
 * @param locz Candidate world-space Z coordinate.
 * @param user Supplies the collision half-extent through Viewer::radius.
 * @param instances Instances tested through their axis-aligned bounds.
 * @return `true` when the candidate position overlaps a solid instance.
 * @note A collision plays the blip sound synchronously before returning.
 */
bool check_col(float locx, float locy, float locz, const Viewer &user, const std::vector<Instance> &instances);

#endif // __COLLIDE_HPP__
