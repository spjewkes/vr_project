/* This contains dummy functions and values to allow us to build the project.
   These functions must be properly implemented at some point but for now we
   want to iron out any other compiler faults from the initial scan of the
   listing. */

#include <stdlib.h>

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

#define P_WAIT (0)

static void spawnvp(int, char *, char *[])
{
}

