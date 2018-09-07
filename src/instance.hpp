#ifndef __INSTANCE_HPP__
#define __INSTANCE_HPP__

#include <string>
#include <vector>
#include "defs.hpp"
#include "master.hpp"
#include "vector3d.hpp"
#include "viewer.hpp"
#include "color.hpp"
#include "light.hpp"

class Instance
{
public:
	Instance() {}
	~Instance() {}

	void setup_vertices();
	void setup_bounds();

	Master *masterptr = { nullptr };

	Vector3d pos = { 0.0f, 0.0f, 0.0f };
	Vector3d min = { 0.0f, 0.0f, 0.0f };
	Vector3d max = { 0.0f, 0.0f, 0.0f };
	Vector3d angle = { 0.0f, 0.0f, 0.0f };
	Vector3d scale = { 1.0f, 1.0f, 1.0f };

	Color color;
	
	float specularity = { 0.0f };

	std::vector<Vector3d> vert;

	std::vector<Color> poly_color;

	RenderStyle style = { RenderStyle::WIREFRAME };
	bool is_solid = { false };

	std::string outcome = { "" };

	void prerender(Viewer &user);
	void setup_color(Viewer &user, Light &light);

	std::vector<Vector3d> user_vert;
	float order;
};

#endif // __INSTANCE_HPP__

