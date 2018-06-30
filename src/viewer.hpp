#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "defs.hpp"

class Viewer
{
public:
	Viewer() {}
	~Viewer() {}
	
	Vector3d ang;
	Vector3d loc;

	float radius = { 1.0f };

	int sky = { 0 };
	int ground = { 0 };
};

#endif // __VIEWER_HPP__
