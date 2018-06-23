#ifndef __PARSE_HPP__
#define __PARSE_HPP__

#include "defs.hpp"

class Parser
{
public:
	Parser();
	~Parser();

    Status parse(char *filename);

	int num_masters() const { return m_masters.size(); }
	int num_instances() const { return m_instances.size(); }

	std::vector<master> &masters() { return m_masters; }
	std::vector<instance> &instances() { return m_instances; }
	viewer& get_user() { return m_user; }

private:
	Status init_master(int no_masters);
	Status init_instance(int no_instances);
	void init_user();

	Status process(void);
	Status process_master(void);
	Status process_instance(void);
	Status process_user(void);
	Status process_objects(int no_objects);
	Status check_object_values(int object_no, int object_pos, int no_objects);
	Status process_object_definition(int object_no);
	Status process_object_instances(int no_instances, int no_objects);
	Status check_instance_values(bool &col_set, bool &spec_set, bool &style_set, int instance_pos, int no_instances, int master_no);

	// for telling whether the master objects have been defined yet
	bool masterdef_processed;

	std::vector<master> m_masters;
	std::vector<instance> m_instances;
	viewer m_user;
};

#endif // __PARSE_HPP__
