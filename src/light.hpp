#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "vector3d.hpp"
#include "color.hpp"

class Light
{
public:
	Light(Vector3d pos, Color col);
	Light() {}
	virtual ~Light();

	Vector3d pos;
	Color col;
};

#endif
