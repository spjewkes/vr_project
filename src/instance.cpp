#include <iostream>
#include <cassert>
#include "instance.hpp"
#include "graphics.hpp"

void Instance::setup_vertices()
{
	/* pass the no_polygons value into the instance */
	int no_verts = masterptr->vert.size();

	/* create array for instance polygons */
	vert.resize(no_verts);
	user_vert.resize(no_verts);

	for (int loop = 0; loop < no_verts; loop++)
	{
		vert[loop] = masterptr->vert[loop];

		vert[loop] *= masterptr->scale;
		vert[loop] *= scale;
		vert[loop].rotate(masterptr->angle);
		vert[loop].rotate(angle);
		vert[loop] += pos;
	}
}

void Instance::setup_color()
{
	int no_polygons, loop, offset;
	int poly_no[3];
	float x1,y1,z1, x2,y2,z2, x3,y3,z3;
	float dx1,dy1,dz1, dx2,dy2,dz2;
	float A,B,C,D, normal, hyp, cos_theta, kd;

	no_polygons = masterptr->poly0.size();

	/* create the arrays that will hold the color values */
	poly_color.resize(no_polygons);

	/* Fill the polygon array with color values */
	for (loop = 0; loop < no_polygons; loop++)
	{
		/* we want to find the direction of the normal first */
		/* so we know which light intensity to apply to the polygon */
		/* get the two of the edges that build up the polygon */
		poly_no[0] = masterptr->poly0[loop];
		poly_no[1] = masterptr->poly1[loop];
		poly_no[2] = masterptr->poly2[loop];

		/* get the first vertex of that edge */
		x1 = vert[poly_no[0]].x();
		y1 = vert[poly_no[0]].y();
		z1 = vert[poly_no[0]].z();
		/* now get the second vertex of that edge */
		x2 = vert[poly_no[1]].x();
		y2 = vert[poly_no[1]].y();
		z2 = vert[poly_no[1]].z();
		/* we need a third point to find the normal to the plane */
		/* so we'll get the end point of the second edge */
		x3 = vert[poly_no[2]].x();
		y3 = vert[poly_no[2]].y();
		z3 = vert[poly_no[2]].z();
		/* now we calculate the changes between the first and */
		/* second vertex */
		dx1 = x2 - x1;
		dy1 = y2 - y1;
		dz1 = z2 - z1;
		/* now changes between third and second vertex */
		dx2 = x3 - x2;
		dy2 = y3 - y2;
		dz2 = z3 - z2;
		/* at last let's calculate the normal */
		/* using the cross product */
		A = (dy1 * dz2) - (dy2 * dz1);
		B = (dz1 * dx2) - (dz2 * dx1);
		C = (dx1 * dy2) - (dx2 * dy1);
		/* now calculate D using a point that is known to be */
		/* on the plane */
		D = (A * x1) + (B * y1) + (C * z1);

		/* now we want to find whether the surface is facing upwards or is */
		/* facing downwards - this way we apply a different intensity */
		/* of light source */
		/* to do this we simply add a value to the y component */
		normal = (A * x1) + (B * (y1 - 20.0)) + (C * z1) - D;

		if (normal < 0.0)
		{
			/* find the angle of incidence between the ray of light */
			/* (coming straight from below) with the surface */
			hyp = sqrt((A*A)+(B*B)+(C*C));
			/* now find the value of cos theta */
			if ((hyp == 0.0) || (B == 0.0))
				cos_theta = 0.0;
			else
				cos_theta = abs(B) / hyp;
			/* abs() is used in case the Y(B) value of the normal is negative */
			/* calculate the diffuse-reflection coeffiecient */
			kd = specularity / 100.0;
			/* now calculate the color offset from the base value */
			offset = 15*kd*cos_theta;
			/* make sure the offset is within the 15 color values */
			if (offset > 15) offset = 15;
			else if (offset < 0) offset = 0;
			/* this surface is facing downwards */
			poly_color[loop] = color*16+(offset*0.25);
		}
		else if (normal >= 0.0)
		{
			/* find the angle of incidence between the ray of light */
			/* (coming straight from above) with the surface */
			hyp = sqrt((A*A)+(B*B)+(C*C));
			/* now find the value of cos theta */
			if ((hyp == 0.0) || (B == 0.0))
				cos_theta = 0.0;
			else
				cos_theta = abs(B) / hyp;
			/* abs() is used in case the Y(B) value of the normal is negative */

			/* calculate the diffuse-reflection coeffiecient */
			kd = specularity / 100.0;
			/* now calculate the color offset from the base value */
			offset = 15*kd*cos_theta;
			/* make sure the offset is within the 15 color values */
			if (offset > 15) offset = 15;
			else if (offset < 0) offset = 0;
			/* this surface is facing upwards */
			/* the light intensity is half */
			poly_color[loop] = color*16+(offset*1.0);
		}
	}
}

void Instance::setup_bounds()
{
	/* set the x,y and z min/max values to the values of the first vertex
	   as an initial value */

	/* minimum value of collision box */
	float xmin = vert[0].x();
	float ymin = vert[0].y();
	float zmin = vert[0].z();

	/* maximum value of collision box */
	float xmax = vert[0].x();
	float ymax = vert[0].y();
	float zmax = vert[0].z();

	/* now look at other values in the instances vertex list and alter the
	   minimum and maximum values accordingly */
	for (auto vertex : vert)
	{
		xmin = std::fmin(xmin, vertex.x());
		ymin = std::fmin(ymin, vertex.y());
		zmin = std::fmin(zmin, vertex.z());

		xmax = std::fmax(xmax, vertex.x());
		ymax = std::fmax(ymax, vertex.y());
		zmax = std::fmax(zmax, vertex.z());
	}

	/* now set the instance values with the minimum x,y and z and the
	   maximum x,y and z */
	/* the minimum values */
	min.x(xmin);
	min.y(ymin);
	min.z(zmin);
	/* the maximum values */
	max.x(xmax);
	max.y(ymax);
	max.z(zmax);
}

void Instance::prerender(Viewer &user)
{
	// The viewing plance position
	float vrp = -50.0;

	// The back plane position
	float BACK = -75.0;

	user_vert.clear();
	order = 0.0f;

	for (auto vertex : vert)
	{
		vertex -= user.loc;
		vertex.rotate(-user.ang);

		// normalize the points ready for perspective projection
		// (-2.0 here to make the y value correct - i.e. -y down and +y up)
		vertex.scale((2.0 * vrp) / (100.0 * (vrp+BACK)),
					 (-2.0 * vrp) / (75.0 * (vrp+BACK)),
					 -1.0 / (vrp+BACK));

		user_vert.push_back(vertex);
		if (vertex.length2() > order)
		{
			order = vertex.length2();
		}
	}

	assert(vert.size() == user_vert.size());
}
