#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include "defs.hpp"

class Master
{
public:
	Master() {}
	~Master() {}
	
	Vector3d scale = { 1.0f, 1.0f, 1.0f };
	Vector3d angle = { 0.0f, 0.0f, 0.0f };

	std::vector<Vector3d> vert;

	std::vector<unsigned int> edge0;
	std::vector<unsigned int> edge1;

	std::vector<unsigned int> poly0;
	std::vector<unsigned int> poly1;
	std::vector<unsigned int> poly2;
};

#endif // __MASTER_HPP__
