#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include <vector>
#include "defs.hpp"
#include "vector3d.hpp"

/**
 * @brief Reusable triangle mesh stored in local coordinates.
 *
 * The polygon vectors are parallel arrays: entries at the same position form
 * one triangle and contain zero-based indices into vert.
 */
class Master
{
  public:
	Master() {}
	~Master() {}

	/** One-based identifier used by the world-file format. */
	unsigned int id = {0};

	/** Component-wise scale shared by every instance of this mesh. */
	Vector3d scale = {1.0f, 1.0f, 1.0f};

	/** Rotation in degrees shared by every instance of this mesh. */
	Vector3d angle = {0.0f, 0.0f, 0.0f};

	/** Local-space mesh vertices. */
	std::vector<Vector3d> vert;

	/** Zero-based first vertex index for each triangle. */
	std::vector<unsigned int> poly0;

	/** Zero-based second vertex index for each triangle. */
	std::vector<unsigned int> poly1;

	/** Zero-based third vertex index for each triangle. */
	std::vector<unsigned int> poly2;
};

#endif // __MASTER_HPP__
