#ifndef __COLLIDE_H__
#define __COLLIDE_H__

int check_col(float locx, float locy, float locz, struct instance *instanceptr, int no_instances, struct viewer user);
int hit_object(int mpos_x, int mpos_y, struct viewer user, struct instance *instanceptr, int no_instances);

#endif // __COLLIDE_H__
