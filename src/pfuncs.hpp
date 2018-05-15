#ifndef __PFUNCS_HPP__
#define __PFUNCS_HPP__

void rotate(float *pntx, float *pnty, float *pntz, float angx, float angy, float angz);
void scale(float *pntx, float *pnty, float *pntz, float sclx, float scly, float sclz);
void translate(float *pntx, float *pnty, float *pntz, float trnx, float trny, float trnz);
void translation(struct instance *instanceptr, int instance_no, float locx, float locy, float locz);

int getline(void);
void skip_garbage(void);
int check(const char *ptr);

void getword(char *word);
int getnum(void);
float fgetnum(void);
int getstring(char *word);
int get_point(float *pntx, float *pnty, float *pntz);

int set_colour(struct master *masterptr, struct instance *instanceptr, int instance_no, int master_no, int colour, float specularity);
void set_bound(struct instance *instanceptr, int instance_no);

#endif // __PFUNCS_HPP__
