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
void create_object_instance(master &mast, instance &inst)
{
	int loop;
	float xpnt, ypnt, zpnt;

	/* pass the no_polygons value into the instance */
	int no_verts = mast.vert.size();

	/* create array for instance polygons */
	inst.vert.resize(no_verts);

	for (loop = 0; loop < no_verts; loop++)
	{
		/* the x, y, z coord from the master */
		xpnt = mast.vert[loop].x();
		ypnt = mast.vert[loop].y();
		zpnt = mast.vert[loop].z();
		/* scale by the master values */
		scale(&xpnt, &ypnt, &zpnt,
			  mast.scale.x(),
			  mast.scale.y(),
			  mast.scale.z());
		/* now scale the instance with the instance values */
		scale(&xpnt, &ypnt, &zpnt,
			  inst.scale.x(),
			  inst.scale.y(),
			  inst.scale.z());
		/* rotate by the master values */
		rotate(&xpnt, &ypnt, &zpnt,
			   mast.angle.x(),
			   mast.angle.y(),
			   mast.angle.z());
		/* now rotate the instance with the instance values */
		rotate(&xpnt, &ypnt, &zpnt,
			   inst.angle.x(),
			   inst.angle.y(),
			   inst.angle.z());
		/* place the coordinate into the instance structure */
		inst.vert[loop].x(xpnt);
		inst.vert[loop].y(ypnt);
		inst.vert[loop].z(zpnt);
	}
}

