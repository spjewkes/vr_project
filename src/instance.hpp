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

/**
 * @brief A transformed and styled occurrence of a Master mesh.
 *
 * Instances cache world-space vertices, viewer-space vertices, collision
 * bounds, and per-triangle colours. Callers that change mesh or instance
 * transforms must rebuild the affected caches before rendering or collision
 * checks.
 */
class Instance
{
  public:
	Instance() {}
	~Instance() {}

	/**
	 * @brief Recalculate the axis-aligned world-space collision bounds.
	 * @pre world_vert contains at least one vertex.
	 */
	void setup_bounds();

	/**
	 * Non-owning pointer to the source mesh.
	 *
	 * The pointed-to Master must outlive this instance and must not be relocated
	 * by resizing its owning vector.
	 */
	Master *masterptr = {nullptr};

	/** World-space translation. */
	Vector3d pos = {0.0f, 0.0f, 0.0f};

	/** Minimum corner of the axis-aligned world-space collision bounds. */
	Vector3d min = {0.0f, 0.0f, 0.0f};

	/** Maximum corner of the axis-aligned world-space collision bounds. */
	Vector3d max = {0.0f, 0.0f, 0.0f};

	/** Instance rotation around X, Y, and Z in degrees. */
	Vector3d angle = {0.0f, 0.0f, 0.0f};

	/** Component-wise scale applied after the master scale. */
	Vector3d scale = {1.0f, 1.0f, 1.0f};

	/** Base material colour used by per-triangle lighting. */
	Color color;

	/** Specular intensity expressed as a percentage from 0 to 100. */
	float specularity = {0.0f};

	/** Flat-lit colour for each source triangle. */
	std::vector<Color> poly_color;

	/** Render mode used by World::render(). */
	RenderStyle style = {RenderStyle::WIREFRAME};

	/** Whether broad-phase collision and pointer selection include this instance. */
	bool is_solid = {false};

	/** Legacy interaction text parsed from the world file. */
	std::string outcome = {""};

	/**
	 * @brief Rebuild world_vert from the source mesh and instance transforms.
	 * @pre masterptr points to a valid Master.
	 * @note Call setup_bounds() afterwards when collision bounds are needed.
	 */
	void local_to_world();

	/**
	 * @brief Transform cached world vertices into the current viewer space.
	 * @param user Viewer position and orientation used for the transform.
	 *
	 * Updates view_vert and the approximate painter-order value.
	 */
	void world_to_viewer(Viewer &user);

	/**
	 * @brief Calculate flat lighting for every source triangle.
	 * @param user Ambient, directional-light, and viewer state.
	 * @param light Positional scene light.
	 * @pre masterptr is valid and world_vert matches the source mesh.
	 */
	void setup_color(Viewer &user, Light &light);

	/** Cached world-space vertices produced by local_to_world(). */
	std::vector<Vector3d> world_vert;

	/** Cached normalized viewer-space vertices produced by world_to_viewer(). */
	std::vector<Vector3d> view_vert;

	/** Approximate squared distance used for painter-style instance sorting. */
	float order;
};

#endif // __INSTANCE_HPP__
