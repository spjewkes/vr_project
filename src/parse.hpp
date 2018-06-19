#ifndef __PARSE_HPP__
#define __PARSE_HPP__

#include "defs.hpp"

class Parser
{
public:
	Parser();
	~Parser();

	int parse(char *filename);

	int num_masters() const { return m_masters.size(); }
	int num_instances() const { return m_instances.size(); }

	std::vector<master> &masters() { return m_masters; }
	std::vector<instance> &instances() { return m_instances; }
	viewer& get_user() { return m_user; }

private:
	int init_master(int no_masters);
	int init_instance(int no_instances);
	void init_user();

	int process(void);
	int process_master(void);
	int process_instance(void);
	int process_user(void);
	int process_objects(int no_objects);
	int check_object_values(int object_no, int object_pos, int no_objects);
	int process_object_definition(int object_no);
	int process_object_instances(int no_instances, int no_objects);
	int check_instance_values(bool &col_set, bool &spec_set, bool &style_set, int instance_pos, int no_instances, int master_no);

	// for telling whether the master objects have been defined yet
	bool masterdef_processed;

	std::vector<master> m_masters;
	std::vector<instance> m_instances;
	viewer m_user;
};

#endif // __PARSE_HPP__
