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
#include "defs.hpp"
#include "error.hpp"
#include "pfuncs.hpp"

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
		scale(&xpnt, &ypnt, &zpnt,
			  masterptr[master_no].scale.x(),
			  masterptr[master_no].scale.y(),
			  masterptr[master_no].scale.z());
		/* now scale the instance with the instance values */
		scale(&xpnt, &ypnt, &zpnt,
			  instanceptr[instance_no].scale.x(),
			  instanceptr[instance_no].scale.y(),
			  instanceptr[instance_no].scale.z());
		/* rotate by the master values */
		rotate(&xpnt, &ypnt, &zpnt,
			   masterptr[master_no].angle.x(),
			   masterptr[master_no].angle.y(),
			   masterptr[master_no].angle.z());
		/* now rotate the instance with the instance values */
		rotate(&xpnt, &ypnt, &zpnt,
			   instanceptr[instance_no].angle.x(),
			   instanceptr[instance_no].angle.y(),
			   instanceptr[instance_no].angle.z());
		/* place the coordinate into the instance structure */
		instanceptr[instance_no].xvert[loop] = xpnt;
		instanceptr[instance_no].yvert[loop] = ypnt;
		instanceptr[instance_no].zvert[loop] = zpnt;
	}
	return (RESULT);
}

