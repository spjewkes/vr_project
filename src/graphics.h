#include <SDL2/SDL.h>

#define XOR_PUT  (0)
#define COPY_PUT (1)
#define SOLID_FILL (2)

#define WHITE (1)

#define grOk (0)

int installuserdriver(const char *, int);
void initgraph(int *, int *, const char *);
void closegraph();
int graphresult();
const char *grapherrormsg(int);

int getmaxx();
int getmaxy();

void setcolor(int);
void line(int, int, int, int);
void setwritemode(int);
void setrgbpalette(int, int, int, int);
void setfillstyle(int, int);
void bar(int, int, int, int);
void fillpoly(int, int *);
