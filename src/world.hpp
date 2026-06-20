#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include <string>
#include <vector>
#include "defs.hpp"
#include "instance.hpp"
#include "master.hpp"
#include "viewer.hpp"
#include "light.hpp"

/**
 * @brief Owns a parsed scene and coordinates rendering it.
 *
 * A world owns all masters, instances, viewer state, and scene lighting.
 * Construction parses the supplied world file immediately. Instance master
 * pointers refer into the owned master vector and require its element addresses
 * to remain stable.
 */
class World
{
  public:
	/**
	 * @brief Construct and parse a world file.
	 * @param _filename Path to the world file.
	 */
	World(std::string _filename);
	~World();

	/** @return `true` when construction parsed the complete world successfully. */
	bool is_ready() const
	{
		return is_parsed;
	}

	void dump_debug();

	int num_masters() const
	{
		return m_masters.size();
	}
	int num_instances() const
	{
		return m_instances.size();
	}

	/**
	 * @brief Access the owned reusable meshes.
	 * @warning Resizing this vector after instances have been linked can
	 * invalidate every Instance::masterptr.
	 */
	std::vector<Master> &masters()
	{
		return m_masters;
	}
	/** @brief Access the placed scene instances. */
	std::vector<Instance> &instances()
	{
		return m_instances;
	}
	/** @brief Access the viewer and environment state. */
	Viewer &user()
	{
		return m_user;
	}

	/**
	 * @brief Render the current scene through the active graphics context.
	 *
	 * Rebuilds viewer-space vertices and per-triangle colours, applies
	 * painter-style sorting, and submits 2D primitives to the graphics module.
	 *
	 * @pre Graphics has been initialized with create_graphics().
	 * @pre Every instance has a valid master pointer and transformed vertices.
	 */
	void render();

	void dump_masters();
	void dump_instances();

  private:
	std::vector<Master> m_masters;
	std::vector<Instance> m_instances;
	Viewer m_user;

	Light m_light;

	bool is_parsed = false;
};

#endif // __WORLD_HPP__
