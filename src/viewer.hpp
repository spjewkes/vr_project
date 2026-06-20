#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__

#include "defs.hpp"
#include "color.hpp"
#include "vector3d.hpp"

/**
 * @brief Camera, environment, and global-light state for a World.
 *
 * With a zero rotation the viewer looks along negative Z. Angles are stored in
 * degrees, and radius is applied independently on each axis during collision
 * checks.
 */
class Viewer
{
  public:
	Viewer()
	{
		g_light_dir.normalize();
	}
	~Viewer() {}

	/** World-space orientation around X, Y, and Z in degrees. */
	Vector3d ang;

	/** World-space viewer position. */
	Vector3d loc;

	/** Half-extent used to expand the viewer position for collision tests. */
	float radius = {1.0f};

	/** Colour used to clear the upper half of each frame. */
	Color sky;

	/** Colour used to clear the lower half of each frame. */
	Color ground;

	/** Ambient light colour. */
	Color ambient = {1.0f, 1.0f, 1.0f};

	/** Ambient contribution applied to every triangle. */
	float ambient_intensity = 0.1f;

	/** Global directional-light colour. */
	Color g_light = {1.0f, 1.0f, 1.0f};

	/** Normalized global light direction. */
	Vector3d g_light_dir = {1.0f, 5.0f, 1.0f};
};

#endif // __VIEWER_HPP__
