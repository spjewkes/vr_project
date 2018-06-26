#ifndef __PFUNCS_HPP__
#define __PFUNCS_HPP__

#include "defs.hpp"

void rotate(float *pntx, float *pnty, float *pntz, float angx, float angy, float angz);
void scale(float *pntx, float *pnty, float *pntz, float sclx, float scly, float sclz);
void translate(float *pntx, float *pnty, float *pntz, float trnx, float trny, float trnz);
void translation(instance &inst, float locx, float locy, float locz);

#endif // __PFUNCS_HPP__
