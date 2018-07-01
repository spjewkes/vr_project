#ifndef __PFUNCS_HPP__
#define __PFUNCS_HPP__

#include "defs.hpp"
#include "instance.hpp"

void do_rotate(float *pntx, float *pnty, float *pntz, float angx, float angy, float angz);
void do_scale(float *pntx, float *pnty, float *pntz, float sclx, float scly, float sclz);
void do_translate(float *pntx, float *pnty, float *pntz, float trnx, float trny, float trnz);
#endif // __PFUNCS_HPP__
