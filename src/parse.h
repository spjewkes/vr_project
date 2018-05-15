#ifndef __PARSE_H__
#define __PARSE_H__

class Parser
{
public:
	Parser();
	~Parser();

	int parse(char *filename);

	int num_masters() const { return no_masters; }
	int num_instances() const { return no_instances; }

	struct master* master_ptr() const { return masterptr; }
	struct instance* instance_ptr() const { return instanceptr; }
	struct viewer& get_user() { return user; }

private:
	int init_master();
	int init_instance();
	void init_user();
	void remove_master();
	void remove_instance();

	int process(void);
	int process_master(void);
	int process_instance(void);
	int process_user(void);
	int process_objects(int no_objects);
	int check_object_values(int object_no, int object_pos, int no_objects);
	int process_object_definition(int object_no);
	int process_object_instances(int no_instances, int no_objects);
	int check_instance_values(int *col_set, int *spec_set, int *style_set, int instance_pos, int no_instances, int master_no);

	// for telling whether the master objects have been defined yet
	bool masterdef_processed;

	int no_masters;
	int no_instances;

	struct master *masterptr;
	struct instance *instanceptr;
	struct viewer user;
};

#endif // __PARSE_H__
