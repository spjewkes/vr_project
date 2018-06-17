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

/* global debug mode */
int debug_mode = TEST;

/* forward declare prototypes */
void debug(const char *string, int level);

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
int error(const char *errno, const char *message, int line_no)
{
	debug("error()", 1);

	/* print error number */
	printf("ERROR: %s - ", errno);
	/* print line number */
	printf("Line number - %d\n", line_no);
	/* and print error message */
	printf("\t%s.\n\n", message);

	/* return an error to caller */
	return(ERROR);
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
void check_master(struct master *ptr, int no_rows)
{
	int loop1;
	debug ("check_master()", 1);

	printf("\nThere are %d master(s).\n", no_rows);
	printf("\nThe Master structure contains The following data:\n");

	for (loop1 = 0; loop1 < no_rows; loop1++)
	{
		/* print which object in the master array */
		printf("\nMaster object:\t%d", loop1);
		/* print the scale values */
		printf("\nScale X:  %f,", ptr[loop1].scale.x());
		printf("\tScale Y:  %f,", ptr[loop1].scale.y());
		printf("\tScale Z:  %f", ptr[loop1].scale.z());
		/* print the angle values */
		printf("\nAngle X:  %f,", ptr[loop1].angle.x());
		printf("\tAngle Y:  %f,", ptr[loop1].angle.y());
		printf("\tAngle Z:  %f", ptr[loop1].angle.z());
		/* print the number of vertices that build up the object */
		printf("\nNumber of vertices:\t%lu", ptr[loop1].vert.size());

		int i = 0;
		/* now list the coordinate values that built up the object */
		for (auto vertex : ptr[loop1].vert)
		{
			/* print the vertex number */
			printf("\nVertex number:\t%d", i++);
			/* list the vertex values */
			printf("\n%f, ", vertex.x());
			printf(" %f,", vertex.y());
			printf(" %f", vertex.z());
		}
		/* print the number of edges that build up the object */
		printf("\nNumber of edges:\t%lu", ptr[loop1].edge0.size());
		/* now list the vertices that make up the edges */
		for (size_t i = 0; i < ptr[loop1].edge0.size(); i++)
		{
			/* print the edge number */
			printf("\nEdge number: \t%zu", i);
			/* list the vertices that make up the edge */
			printf("\nVertex one: %d", ptr[loop1].edge0[i]);
			printf("\nVertex two: %d", ptr[loop1].edge1[i]);
		}
		/* print the number of polygons that build up the object */
		printf ("\nNumber of polygons: \t%lu", ptr[loop1].poly0.size());
		/* now list the edges that make up the polygon */
		for (size_t i = 0; i < ptr[loop1].poly0.size(); i++)
		{
			/* print the polygon number */
			printf("\nPolygon number: \t%zu", i);
			/* list the edges that make up the polygon */
			printf("\nEdge one: %d", ptr[loop1].poly0[i]);
			printf("\nEdge two: %d", ptr[loop1].poly1[i]);
			printf("\nEdge three: %d", ptr[loop1].poly2[i]);
		}
		printf("\n---------------------\n");
	}
}

/****************************************************************************
* check_instance() - another debugging tool - this one simply displays the  *
*                    contents of the instance structure to the screen       *
*                    let's just make sure that it's doing its job correctly *
****************************************************************************/
void check_instance(struct instance *ptr, int no_rows)
{
	int loop1;

	debug("check_instance ()", 1);

	printf("\nThere are %d instance(s).\n", no_rows);
	printf("\nThe Instance structure contains the following data:\n");

	for (loop1 = 0; loop1 < no_rows; loop1++)
	{
		/* print which object in the instance array */
		printf("\nInstance object:\t%d", loop1);
		/* print which master object it is cased on */
		printf("\nMaster object number:\t%d", ptr[loop1].master_no);
		/* print the iocational values */
		printf("\nX location:  %f,", ptr[loop1].pos.x());
		printf("\tY location:  %f,", ptr[loop1].pos.y());
		printf("\tZ location:  %f", ptr[loop1].pos.z());
		/* print the minimum values */
		printf("\nMinimum X:  %f,", ptr[loop1].min.x());
		printf("\tMinimum Y:  %f,", ptr[loop1].min.y());
		printf("\tMinimum Z:  %f", ptr[loop1].min.z());
		/* print the maximum values */
		printf("\nMaximum X:  %f,", ptr[loop1].max.x());
		printf("\tMaximum Y:  %f,", ptr[loop1].max.y());
		printf("\tMaximum Z:  %f", ptr[loop1].max.z());
		/* print the angle values */
		printf("\nAngle X:  %f,", ptr[loop1].angle.x());
		printf("\tAngle Y:  %f,", ptr[loop1].angle.y());
		printf("\tAngle Z:  %f", ptr[loop1].angle.z());
		/* print the scale values */
		printf("\nScale X:  %f,", ptr[loop1].scale.x());
		printf("\tScale Y:  %f,", ptr[loop1].scale.y());
		printf("\tScale Z:  %f", ptr[loop1].scale.z());
		/* print the number of vertices that build up the object */
		printf("\nNumber of vertices:\t%lu", ptr[loop1].vert.size());
		/* now list the coordinate values that built up the object */
		int i = 0;
		for (auto vertex : ptr[loop1].vert)
		{
			/* print the vertex number */
			printf("\nVertex number: \t%d", i++);
			/* list the vertex values */
			printf("\n%f,", vertex.x());
			printf("  %f,", vertex.y());
			printf("  %f", vertex.z());
		}
		/* print the style number */
		printf("\nStyle number:\t%d", ptr[loop1].style);
		/* print whether the object is to be solid or not */
		printf("\nSolid?:\t%d", ptr[loop1].is_solid);
		/* print the outcome string */
		printf("\nOutcome string:\t>>%s<<", ptr[loop1].outcome.c_str());
		printf("\n----------------\n");
	}
}
