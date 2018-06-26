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

	Status process_location(Vector3d &loc);
	Status process_direction(Vector3d &ang);
	Status process_radius(float *radius);
	Status process_angle(Vector3d &ang);
	Status process_scale(Vector3d &scl);
	Status process_colour(int *colour);
	Status process_specularity(float *specularity);
	bool process_outcome(std::string &outcome);
	Status process_style(RenderStyle &style);
	Status process_verts(master &mast, int no_vertices);
	Status process_edges(master &mast, int no_edges);
	Status process_polys(master &mast, int no_polygons);
	Status process_sky(int *colour);
	Status process_ground(int *colour);

	MatchResult getline(void);
	void skip_garbage(void);
	MatchResult check(const char *ptr);

	void getword(char *word);
	int getnum(void);
	float fgetnum(void);
	Status getstring(char *word);
	Status get_point(float *pntx, float *pnty, float *pntz);

	void set_colour(master &mast, instance &inst, int colour, float specularity);
	void set_bound(instance &inst);

	// Variables for managing reading the file to parse
	int lincnt;
	int lineptr;
	void *fp;
	char LINE[MAXLINE];

	// for telling whether the master objects have been defined yet
	bool masterdef_processed;

	std::vector<master> m_masters;
	std::vector<instance> m_instances;
	viewer m_user;
};

#endif // __PARSE_HPP__
