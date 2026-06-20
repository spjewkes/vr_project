#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include "defs.hpp"

/**
 * @brief Open one of the legacy numbered screen sizes.
 * @param mode Mode from 0 (320x200) through 4 (1024x768).
 * @return Okay when graphics initialization succeeds, otherwise Error.
 */
Status screen_open(int mode);

#endif // __SCREEN_HPP__
