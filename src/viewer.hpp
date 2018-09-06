#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "defs.hpp"
#include "color.hpp"
#include "vector3d.hpp"

class Viewer
{
public:
	Viewer() { g_light_dir.normalize(); }
	~Viewer() {}
	
	Vector3d ang;
	Vector3d loc;

	float radius = { 1.0f };

	Color sky;
	Color ground;

	Color ambient = { 1.0f, 1.0f, 1.0f };
	float ambient_intensity = 0.1f;
	Color g_light = { 1.0f, 1.0f, 1.0f };
	Vector3d g_light_dir = { 1.0f, 5.0f, 1.0f };
};

#endif // __VIEWER_HPP__
