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
	int no_polygons, loop;
	int poly_no[3];
	float ka, kd;

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

		Vector3d v1 = vert[poly_no[1]] - vert[poly_no[0]];
		Vector3d v2 = vert[poly_no[2]] - vert[poly_no[1]];

		Vector3d normal = v1.cross(v2);
		normal.normalize();

		// Calculate a light coming in at an angle for now
		Vector3d light(0.0f, -5.0f, -1.0f);
		light.normalize();

		float cos_theta = normal.dot(light);

		if (cos_theta < 0.0)
		{
			// Apply just ambient light to surfaces facing away
			poly_color[loop] = Color(ka, ka, ka);
		}
		else
		{
			/* calculate the diffuse-reflection coeffiecient + ambient */
			ka = (specularity / 100.0) * 0.25f;
			kd = (specularity / 100.0) * cos_theta;
			/* now calculate the color intensity */
			poly_color[loop] = (color * kd * 1.0f) + Color(ka, ka, ka);
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
	}

	// Calculate length of centre of object relative to viewer
	Vector3d v[2] = { min, max };
	for (auto &vertex : v)
	{
		vertex -= user.loc;
		vertex.rotate(-user.ang);

		// Normalize the points
		vertex.scale((2.0 * vrp) / (100.0 * (vrp+BACK)),
					 (-2.0 * vrp) / (75.0 * (vrp+BACK)),
					 -1.0 / (vrp+BACK));
	}

	// Use the furthest point of the object
	float l1 = v[0].length2();
	float l2 = v[1].length2();

	order = (l1 > l2 ? l1 : l2);

	assert(vert.size() == user_vert.size());
}
