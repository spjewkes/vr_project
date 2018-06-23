/******************************************
*                                         *
* ##### ####  ####   ###   ####      #### *
* ##    ## ## ## ## ## ##  ## ##    ##    *
* ####  ####  ####  ## ##  ####     ##    *
* ##    ## ## ## ## ## ##  ## ##    ##    *
* ##### ## ## ## ##  ###   ## ## ##  #### *
*                                         *
******************************************/
/****************
* include files *
****************/
#include <stdio.h>
#include "defs.hpp"
#include "error.hpp"

/* global debug mode */
int debug_mode = TEST;

/* Set debug mode */
void set_debug_mode(int mode)
{
	debug_mode = mode;
}

/*****************************************************************************
* error() - all error messages come through here                             *
*           simply pass in a string containing the error number, a string    *
*           containing the error message and a relevant line number          *
*           keeps the same format for all error messages                     *
*****************************************************************************/
Status error(const char *errno, const char *message, int line_no)
{
	debug("error()", 1);

	/* print error number */
	printf("ERROR: %s - ", errno);
	/* print line number */
	printf("Line number - %d\n", line_no);
	/* and print error message */
	printf("\t%s.\n\n", message);

	/* return an error to caller */
	return Error;
}

/*****************************************************************************
* warn() - in principle the same as error() only it displays just warnings   *
*          and does not return anything                                      *
*****************************************************************************/
void warn(const char *warnno, const char *message, int line_no)
{
	debug("warn()", 1);

	/* print warning number */
	printf("WARNING: %s - ", warnno);
	/* print line number */
	printf("Line number: %d\n", line_no);
	/* and print warning message */
	printf("\t%s.inkn", message);
}

/*****************************************************************************
* debug() - function to be used to debug programs - global variable 'stage'  *
*           should be set to TEST for it to work                             *
*****************************************************************************/
void debug(const char *string, int level)
{
	if (debug_mode == TEST)
	{
		/* print the debug message at a particular level */
		switch (level)
		{
		case 1:
		{
			printf("Function: \"%s\"\n", string);
			break;
		}
		case 2:
		{
			printf("\t%s\n", string);
			break;
		}
		case 3:
		{
			printf("\t\t%s\n", string);
			break;
		}
		default:
		{
			printf("\t\t\t%s\n", string);
			break;
		}
		}
	}
	else if (debug_mode == FINAL)
	{
		/* we do nothing here because this is final stage of the program */
	}
}

/*****************************************************************************
* check_master() - another debugging tool - this one simply displays the     *
*                  contents of the master structure to the screen            *
*                  let's just make sure that it's doing its job correctly    *
*****************************************************************************/
void check_master(std::vector<master> &masters)
{
	debug ("check_master()", 1);

	printf("\nThere are %lu master(s).\n", masters.size());
	printf("\nThe Master structure contains The following data:\n");

	int i = 0;

	for (auto mast : masters)
	{
		/* print which object in the master array */
		printf("\nMaster object:\t%d", i++);
		/* print the scale values */
		printf("\nScale X:  %f,", mast.scale.x());
		printf("\tScale Y:  %f,", mast.scale.y());
		printf("\tScale Z:  %f", mast.scale.z());
		/* print the angle values */
		printf("\nAngle X:  %f,", mast.angle.x());
		printf("\tAngle Y:  %f,", mast.angle.y());
		printf("\tAngle Z:  %f", mast.angle.z());
		/* print the number of vertices that build up the object */
		printf("\nNumber of vertices:\t%lu", mast.vert.size());

		int i = 0;
		/* now list the coordinate values that built up the object */
		for (auto vertex : mast.vert)
		{
			/* print the vertex number */
			printf("\nVertex number:\t%d", i++);
			/* list the vertex values */
			printf("\n%f, ", vertex.x());
			printf(" %f,", vertex.y());
			printf(" %f", vertex.z());
		}
		/* print the number of edges that build up the object */
		printf("\nNumber of edges:\t%lu", mast.edge0.size());
		/* now list the vertices that make up the edges */
		for (size_t i = 0; i < mast.edge0.size(); i++)
		{
			/* print the edge number */
			printf("\nEdge number: \t%zu", i);
			/* list the vertices that make up the edge */
			printf("\nVertex one: %d", mast.edge0[i]);
			printf("\nVertex two: %d", mast.edge1[i]);
		}
		/* print the number of polygons that build up the object */
		printf ("\nNumber of polygons: \t%lu", mast.poly0.size());
		/* now list the edges that make up the polygon */
		for (size_t i = 0; i < mast.poly0.size(); i++)
		{
			/* print the polygon number */
			printf("\nPolygon number: \t%zu", i);
			/* list the edges that make up the polygon */
			printf("\nEdge one: %d", mast.poly0[i]);
			printf("\nEdge two: %d", mast.poly1[i]);
			printf("\nEdge three: %d", mast.poly2[i]);
		}
		printf("\n---------------------\n");
	}
}

/****************************************************************************
* check_instance() - another debugging tool - this one simply displays the  *
*                    contents of the instance structure to the screen       *
*                    let's just make sure that it's doing its job correctly *
****************************************************************************/
void check_instance(std::vector<instance> &instances)
{
	debug("check_instance ()", 1);

	printf("\nThere are %lu instance(s).\n", instances.size());
	printf("\nThe Instance structure contains the following data:\n");

	int i = 0;

	for (auto inst : instances)
	{
		/* print which object in the instance array */
		printf("\nInstance object:\t%d", i++);
		/* print which master object it is cased on */
		printf("\nMaster object number:\t%d", inst.master_no);
		/* print the iocational values */
		printf("\nX location:  %f,", inst.pos.x());
		printf("\tY location:  %f,", inst.pos.y());
		printf("\tZ location:  %f", inst.pos.z());
		/* print the minimum values */
		printf("\nMinimum X:  %f,", inst.min.x());
		printf("\tMinimum Y:  %f,", inst.min.y());
		printf("\tMinimum Z:  %f", inst.min.z());
		/* print the maximum values */
		printf("\nMaximum X:  %f,", inst.max.x());
		printf("\tMaximum Y:  %f,", inst.max.y());
		printf("\tMaximum Z:  %f", inst.max.z());
		/* print the angle values */
		printf("\nAngle X:  %f,", inst.angle.x());
		printf("\tAngle Y:  %f,", inst.angle.y());
		printf("\tAngle Z:  %f", inst.angle.z());
		/* print the scale values */
		printf("\nScale X:  %f,", inst.scale.x());
		printf("\tScale Y:  %f,", inst.scale.y());
		printf("\tScale Z:  %f", inst.scale.z());
		/* print the number of vertices that build up the object */
		printf("\nNumber of vertices:\t%lu", inst.vert.size());
		/* now list the coordinate values that built up the object */
		int i = 0;
		for (auto vertex : inst.vert)
		{
			/* print the vertex number */
			printf("\nVertex number: \t%d", i++);
			/* list the vertex values */
			printf("\n%f,", vertex.x());
			printf("  %f,", vertex.y());
			printf("  %f", vertex.z());
		}
		/* print the style number */
		printf("\nStyle number:\t%d", inst.style);
		/* print whether the object is to be solid or not */
		printf("\nSolid?:\t%d", inst.is_solid);
		/* print the outcome string */
		printf("\nOutcome string:\t>>%s<<", inst.outcome.c_str());
		printf("\n----------------\n");
	}
}
