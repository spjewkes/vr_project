#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include "vector3d.hpp"

#define RADCONST (57.295779513082321)

#define FALSE (0)
#define TRUE  (!FALSE)

#define TEST  (1)
#define FINAL (2)

#define MAXLINE (4096)
#define MAXARG  (260)

#define ERROR   (-1)
#define OKAY    (1)
#define BLANK   (2)
#define COMMENT (3)
#define OTHER   (4)

#define EQUAL (0)

#define MSIZE (8)

#define EVER (;;)

#define SOLID  (0)
#define WFRAME (1)

#define MOVE_SPEED (25.0f)

/* External variables */
extern int lincnt, lineptr;

/*************************
* Define Keyboard events *
*************************/
enum keyboard_state
{
	KEY_UP = 0,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_LSHIFT,
	KEY_LALT,
	KEY_QUIT,

	KEY_MAX
};

/************************
* Define user structure *
************************/
struct viewer
{
	Vector3d ang;
	Vector3d loc;

	float radius;

	int sky, ground;
};

/****************************
* Define instance structure *
****************************/
struct instance
{
	int master_no;

	Vector3d pos;
	Vector3d min;
	Vector3d max;
	float anglex, angley, anglez;
	float scalex, scaley, scalez;

	int no_vertices;
	float *xvert, *yvert, *zvert;

	int *edge_colour;
	int *poly_colour;

	int style;
	int solid;

	char *outcome;
};

/**************************
* Define master structure *
**************************/
struct master
{
	float scalex, scaley, scalez;
	float anglex, angley, anglez;

	int no_vertices;
	float *xvert, *yvert, *zvert;

	int no_edges;
	int *edge0, *edge1;

	int no_polygons;
	int *poly0, *poly1, *poly2;
};

#endif // __DEFS_HPP__
