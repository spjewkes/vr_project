#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "defs.hpp"
#include "color.hpp"

class Viewer
{
public:
	Viewer() {}
	~Viewer() {}
	
	Vector3d ang;
	Vector3d loc;

	float radius = { 1.0f };

	Color sky;
	Color ground;
};

#endif // __VIEWER_HPP__
