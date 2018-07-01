#include "instance.hpp"
#include "pfuncs.hpp"

void Instance::setup_vertices()
{
	/* pass the no_polygons value into the instance */
	int no_verts = masterptr->vert.size();

	/* create array for instance polygons */
	vert.resize(no_verts);

	for (int loop = 0; loop < no_verts; loop++)
	{
		/* the x, y, z coord from the master */
		float xpnt = masterptr->vert[loop].x();
		float ypnt = masterptr->vert[loop].y();
		float zpnt = masterptr->vert[loop].z();
		/* scale by the master values */
		do_scale(&xpnt, &ypnt, &zpnt,
				 masterptr->scale.x(),
				 masterptr->scale.y(),
				 masterptr->scale.z());
		/* now scale the instance with the instance values */
		do_scale(&xpnt, &ypnt, &zpnt,
				 scale.x(),
				 scale.y(),
				 scale.z());
		/* rotate by the master values */
		do_rotate(&xpnt, &ypnt, &zpnt,
				  masterptr->angle.x(),
				  masterptr->angle.y(),
				  masterptr->angle.z());
		/* now rotate the instance with the instance values */
		do_rotate(&xpnt, &ypnt, &zpnt,
				  angle.x(),
				  angle.y(),
				  angle.z());
		/* place the coordinate into the instance structure */
		vert[loop].x(xpnt);
		vert[loop].y(ypnt);
		vert[loop].z(zpnt);
	}
}
