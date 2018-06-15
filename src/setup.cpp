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
	int loop;
	float xpnt, ypnt, zpnt;

	/* pass the no_polygons value into the instance */
	int no_verts = masterptr[master_no].vert.size();

	/* create array for instance polygons */
	instanceptr[instance_no].vert.resize(no_verts);

	for (loop = 0; loop < no_verts; loop++)
	{
		/* the x, y, z coord from the master */
		xpnt = masterptr[master_no].vert[loop].x();
		ypnt = masterptr[master_no].vert[loop].y();
		zpnt = masterptr[master_no].vert[loop].z();
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
		instanceptr[instance_no].vert[loop].x(xpnt);
		instanceptr[instance_no].vert[loop].y(ypnt);
		instanceptr[instance_no].vert[loop].z(zpnt);
	}
	return (RESULT);
}

