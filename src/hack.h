/* This contains dummy functions and values to allow us to build the project.
   These functions must be properly implemented at some point but for now we
   want to iron out any other compiler faults from the initial scan of the
   listing. */

#include <stdlib.h>

static int getmaxx()
{
	return 800;
}

static int getmaxy()
{
	return 600;
}

static void sound(int)
{
}

static void delay(int)
{
}

static void nosound()
{
}

static int getch()
{
	return 0;
}

static int kbhit()
{
	return 0;
}

#define XOR_PUT  (0)
#define COPY_PUT (1)
#define SOLID_FILL (2)

#define WHITE (1)

static void setcolor(int)
{
}

static void line(int, int, int, int)
{
}

static void setwritemode(int)
{
}

static void setrgbpalette(int, int, int, int)
{
}

static void setfillstyle(int, int)
{
}

static void bar(int, int, int, int)
{
}

static void fillpoly(int, int *)
{
}

#define P_WAIT (0)

static void spawnvp(int, char *, char *[])
{
}

static int installuserdriver(const char *, int)
{
	return 0;
}

static void initgraph(int *, int *, const char *)
{
}

static void closegraph()
{
}

#define grOk (0)

static int graphresult()
{
	return 0;
}

static const char *grapherrormsg(int)
{
	return NULL;
}
