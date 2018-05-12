/******************************************
*                                         *
*  #### ##### ###### ## ## ####      #### *
* ##    ##      ##   ## ## ## ##    ##    *
* ##### ####    ##   ## ## ####     ##    *
*    ## ##      ##   ## ## ##       ##    *
* ####  #####   ##    ###  ##    ##  #### *
*                                         *
******************************************/
/****************
* include files *
****************/
#include <stdlib.h>
#include "defs.h"
#include "error.h"
#include "pfuncs.h"

/*****************************************************************************
* init_user() - initialises user variables                                   *
*****************************************************************************/
void init_user(void)
{
	debug("init_user()", 1);

	user.locx = 0.0;
	user.locy = 0.0;
	user.locz = 0.0;

	user.angx = 0.0;
	user.angy = 0.0;
	user.angz = 0.0;

	user.radius = 1.0;

	user.sky = 0;
	user.ground = 0;
}

/****************************************************************************
* create_object_instance() - this function builds the instance object using *
*                            the master object and the angle and scale that *
*                            that needs to transform the instance           *
****************************************************************************/
int create_object_instance(struct master *masterptr, struct instance *instanceptr,
						   int instance_no, int master_no)
{
	int RESULT = OKAY;
	int loop, no_verts;
	float xpnt, ypnt, zpnt;

	/* pass the no_polygons value into the instance */
	no_verts = masterptr[master_no].no_vertices;
	instanceptr[instance_no].no_vertices = no_verts;

	/* create array for instance polygons */
	instanceptr[instance_no].xvert = (float *) malloc(sizeof(float) * no_verts);
	instanceptr[instance_no].yvert = (float *) malloc(sizeof(float) * no_verts);
	instanceptr[instance_no].zvert = (float *) malloc(sizeof(float) * no_verts);
	/* now check that these have been created */
	if ((instanceptr[instance_no].xvert == NULL) ||
		(instanceptr[instance_no].yvert == NULL) ||
		(instanceptr[instance_no].zvert == NULL))
		return(error("0048", "Error allocating memory", lincnt));

	for (loop = 0; loop < no_verts; loop++)
	{
		/* the x, y, z coord from the master */
		xpnt = masterptr[master_no].xvert[loop];
		ypnt = masterptr[master_no].yvert[loop];
		zpnt = masterptr[master_no].zvert[loop];
		/* scale by the master values */
		scale(&xpnt, &ypnt, &zpnt, masterptr[master_no].scalex,
			  masterptr[master_no].scaley,
			  masterptr[master_no].scalez);
		/* now scale the instance with the instance values */
		scale(&xpnt, &ypnt, &zpnt, instanceptr[instance_no].scalex,
			  instanceptr[instance_no].scaley,
			  instanceptr[instance_no].scalez);
		/* rotate by the master values */
		rotate(&xpnt, &ypnt, &zpnt, masterptr[master_no].anglex,
			   masterptr[master_no].angley,
			   masterptr[master_no].anglez);
		/* now rotate the instance with the instance values */
		rotate(&xpnt, &ypnt, &zpnt, instanceptr[instance_no].anglex,
			   instanceptr[instance_no].angley,
			   instanceptr[instance_no].anglez);
		/* place the coordinate into the instance structure */
		instanceptr[instance_no].xvert[loop] = xpnt;
		instanceptr[instance_no].yvert[loop] = ypnt;
		instanceptr[instance_no].zvert[loop] = zpnt;
	}
	return (RESULT);
}

