#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include <string>
#include "defs.hpp"
#include "instance.hpp"
#include "master.hpp"
#include "viewer.hpp"
#include "light.hpp"

class World
{
public:
	World(std::string _filename);
	~World();

	bool is_ready() const { return is_parsed; }

	void dump_debug();

	int num_masters() const { return m_masters.size(); }
	int num_instances() const { return m_instances.size(); }

	std::vector<Master> &masters() { return m_masters; }
	std::vector<Instance> &instances() { return m_instances; }
	Viewer& user() { return m_user; }

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

