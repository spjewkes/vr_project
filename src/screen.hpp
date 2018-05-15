#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

int screen_open(int mode);
void draw_image(struct master *mptr, struct instance *iptr, int no_instances, struct viewer user);

#endif // __SCREEN_HPP__
