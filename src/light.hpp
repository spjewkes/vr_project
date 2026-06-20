#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "vector3d.hpp"
#include "color.hpp"

/** @brief Positional light represented by a world-space position and colour. */
class Light
{
  public:
	Light(Vector3d pos, Color col);
	Light() {}
	virtual ~Light();

	Vector3d pos; /**< World-space light position. */
	Color col;    /**< RGB light colour and intensity. */
};

#endif
