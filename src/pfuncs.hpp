#ifndef __PFUNCS_HPP__
#define __PFUNCS_HPP__

#include "defs.hpp"

void rotate(float *pntx, float *pnty, float *pntz, float angx, float angy, float angz);
void scale(float *pntx, float *pnty, float *pntz, float sclx, float scly, float sclz);
void translate(float *pntx, float *pnty, float *pntz, float trnx, float trny, float trnz);
void translation(instance &inst, float locx, float locy, float locz);

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

#endif // __PFUNCS_HPP__
