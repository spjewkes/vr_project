#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "defs.hpp"
#include "instance.hpp"
#include "master.hpp"
#include "viewer.hpp"

class World
{
public:
	int num_masters() const { return m_masters.size(); }
	int num_instances() const { return m_instances.size(); }

	std::vector<Master> &masters() { return m_masters; }
	std::vector<Instance> &instances() { return m_instances; }
	Viewer& user() { return m_user; }

	void render();

private:
	std::vector<Master> m_masters;
	std::vector<Instance> m_instances;
	Viewer m_user;
};

#endif // __WORLD_HPP__

