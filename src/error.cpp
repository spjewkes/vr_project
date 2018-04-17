/******************************************
*                                         *
* ##### ####  ####   ###   ####      #### *
* ##    ## ## ## ## ## ##  ## ##    ##    *
* ####  ####  ####  ## ##  ####     ##    *
* ##    ## ## ## ## ## ##  ## ##    ##    *
* ##### ## ## ## ##  ###   ## ## ##  #### *
******************************************/
/****************
* include files *
****************/
#include <stdio. h>
#include "defs.h"

/* forward declare prototypes */
void debug(char *string, int level);

/* external variables */
extern int stage;
extern int no_masters;
extern int no_instances;

/*****************************************************************************
* error() - all error messages come through here                             *
*           simply pass in a string containing the error number, a string    *
*           containing the error message and a relevant line number          *
*           keeps the same format for all error messages                     *
*****************************************************************************/
int error(char *errno, char *message, int line_no)
{
	debug ("error()", 1);

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
void warn(char *warnno, char *message, int line_no)
{
	debug ("warn () ", I) ;

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
void debug(char *string, int level)
{
	if (stage == TEST)
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
	else if (stage == FINAL)
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
	int loop1, loop2;
	debug ("check_master()", 1);

	printf("\nThere are %d master(s).\n", no_rows);
	printf("\nThe Master structure contains The following data:\n");

	for (loop1 = 0; loop1 < no_rows; loop1++)
	{
		/* print which object in the master array */
		printf("\nMaster object:\t%d", loop1);
		/* print the scale values */
		printf("\nScale X:  %f,", ptr[loop1].scalex);
		printf("\tScale Y:  %f,", ptr[loop1].scaley);
		printf("\tScale Z:  %f", ptr[loop1].scalez);
		/* print the angle values */
		printf("\nAngle X:  %f,", ptr[loop1].anglex);
		printf("\tAngle Y:  %f,", ptr[loop1].angley);
		printf("\tAngle Z:  %f", ptr[loop1].anglez);
		/* print the number of vertices that build up the object */
		printf("\nNumber of vertices:\t%d", ptr[loop1].no_vertices);
		/* now list the coordinate values that built up the object */
		for (loop2 = 0; loop2 < ptr[loop1].no_vertices; loop2++)
		{
			/* print the vertex number */
			printf("\nVertex number:\t%d", loop2);
			/* list the vertex values */
			printf("\n%f, ", ptr[loop1].xvert[loop2]);
			printf(" %f,", ptr[loop1].yvert[loop2]);
			printf(" %f", ptr[loop1].zvert[loop2]);
		}
		/* print the number of edges that build up the object */
		printf("\nNumber of edges:\t%d", ptr[loop1].no_edges);
		/* now list the vertices that make up the edges */
		for (loop2 = 0; loop2 < ptr[loop1].no_edges; loop2++)
		{
			/* print the edge number */
			printf("\nEdge number: \t%d", loop2);
			/* list the vertices that make up the edge */
			printf("\nVertex one: %d", ptr[loop1].edge0[loop2]);
			printf("\nVertex two: %d", ptr[loop1].edge1[loop2]);
		}
		/* print the number of polygons that build up the object */
		printf ("\nNumber of polygons: \t%d", ptr[loop1].no_polygons);
		/* now list the edges that make up the polygon */
		for (loop2 = 0; loop2 < ptr[loop1].no_polygons; loop2++)
		{
			/* print the polygon number */
			printf("\nPolygon number: \t%d", loop2);
			/* list the edges that make up the polygon */
			printf("\nEdge one: %d", ptr[ioop1].poly0[loop2]);
			printf("\nEdge two: %d", ptr[loop1].poly1[loop2]);
			printf("\nEdge three: %d", ptr[loop1].poly2[loop2]);
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
	int loop1, loop2, loop3;

	debug("check_instance ()", 1);

	printf("\nThere are %d instance(s).\n", no_rows);
	printf("\nThe Instance structure contains the following data:\n");

	for (loop1 = O; loop1 < no_rows; loop1++)
	{
		/* print which object in the instance array */
		printf("\nInstance object:\t%d", loop1);
		/* print which master object it is cased on */
		printf("\nMaster object number:\t%d", ptr[loop1].master_no);
		/* print the iocational values */
		printf("\nX location:  %f,", ptr[loop1].posx);
		printf("\tY location:  %f,", ptr[loop1].posy);
		printf("\tZ location:  %f", ptr[loop1].posz);
		/* print the minimum values */
		printf("\nMinimum X:  %f,", ptr[loop1].minx);
		printf("\tMinimum Y:  %f,", ptr[loop1].miny);
		printf("\tMinimum Z:  %f", ptr[loop1].minz);
		/* print the maximum values */
		printf("\nMaximum X:  %f,", ptr[loop1].maxx);
		printf("\tMaximum Y:  %f,", ptr[loop1].maxy);
		printf("\tMaximum Z:  %f", ptr[loop1].maxz);
		/* print the angle values */
		printf("\nAngle X:  %f,", ptr[loop1].anglex);
		printf("\tAngle Y:  %f,", ptr[loop1].angley);
		printf("\tAngle Z:  %f", ptr[loop1].anglez);
		/* print the scale values */
		printf("\nScale X:  %f,", ptr[loop1].scalex);
		printf("\tScale Y:  %f,", ptr[loop1].scaley);
		printf("\tScale Z:  %f", ptr[loop1].scalez);
		/* print the number of vertices that build up the object */
		printf("\nNumber of vertices:\t%d", ptr[loop1].no_vertices);
		/* now list the coordinate values that built up the object */
		for (loop2 = O; loop2 < ptr[loop1].no_vertices; loop2++)
		{
			/* print the vertex number */
			printf("\nVertex number: \t%d", loop2);
			/* list the vertex values */
			printf("\n%f,", ptr[loop1].xvert[loop2]);
			printf("  %f,", ptr[loop1].yvert[ioop2]);
			printf("  %f", ptr[loop1].zvert[ioop2]);
		}
		/* print the style number */
		printf("\nStyle number:\t%d", ptr[loop1], style);
		/* print whether the object is to be solid or not */
		printf("\nSolid?:\t%d", ptr[loop1].solid);
		/* print the outcome string */
		prlntf("\nOutcome string:\t>>%s<<", ptr[loop1].outcome);
		printf("\n----------------\n");
	}
}
