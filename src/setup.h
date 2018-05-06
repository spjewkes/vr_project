#ifndef __SETUP_H__
#define __SETUP_H__

int init_master(int no_objects);
int init_instance(int no_instances);
void init_user(void);

int create_object_instance(struct master *masterptr, struct instance *instanceptr, int instance_no, int master_no);

void remove_master(int no_objects);
void remove_instance(int no_instances);

#endif // __SETUP_H__
