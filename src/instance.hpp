#ifndef __INSTANCE_HPP__
#define __INSTANCE_HPP__

#include "defs.hpp"
#include "master.hpp"

class Instance
{
public:
	Instance() {}
	~Instance() {}

	Master *masterptr = { nullptr };

	Vector3d pos = { 0.0f, 0.0f, 0.0f };
	Vector3d min = { 0.0f, 0.0f, 0.0f };
	Vector3d max = { 0.0f, 0.0f, 0.0f };
	Vector3d angle = { 0.0f, 0.0f, 0.0f };
	Vector3d scale = { 1.0f, 1.0f, 1.0f };

	std::vector<Vector3d> vert;

	std::vector<unsigned int> edge_color;
	std::vector<unsigned int> poly_color;

	RenderStyle style = { RenderStyle::WIREFRAME };
	bool is_solid = { false };

	std::string outcome = { "" };
};

#endif // __INSTANCE_HPP__
