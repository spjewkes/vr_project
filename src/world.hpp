#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "defs.hpp"

class World
{
public:
	int num_masters() const { return m_masters.size(); }
	int num_instances() const { return m_instances.size(); }

	std::vector<master> &masters() { return m_masters; }
	std::vector<instance> &instances() { return m_instances; }
	viewer& user() { return m_user; }

private:
	std::vector<master> m_masters;
	std::vector<instance> m_instances;
	viewer m_user;
};

#endif // __WORLD_HPP__
