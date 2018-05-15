#ifndef __SCREEN_H__
#define __SCREEN_H__

int screen_open(int mode);
void draw_image(struct master *mptr, struct instance *iptr, int no_instances, struct viewer user);

#endif // __SCREEN_H__
