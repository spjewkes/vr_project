#include <iostream>
#include <cassert>
#include "instance.hpp"
#include "graphics.hpp"

void Instance::setup_bounds()
{
	/* set the x,y and z min/max values to the values of the first vertex
	   as an initial value */

	/* minimum value of collision box */
	float xmin = world_vert[0].x();
	float ymin = world_vert[0].y();
	float zmin = world_vert[0].z();

	/* maximum value of collision box */
	float xmax = world_vert[0].x();
	float ymax = world_vert[0].y();
	float zmax = world_vert[0].z();

	/* now look at other values in the instances vertex list and alter the
	   minimum and maximum values accordingly */
	for (auto vertex : world_vert)
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

void Instance::local_to_world()
{
	int no_verts = masterptr->vert.size();

	// Local coordinates are held by the master, convert and store the world
	// coordinates in this instance
	world_vert.resize(no_verts);

	for (int loop = 0; loop < no_verts; loop++)
	{
		world_vert[loop] = masterptr->vert[loop];

		world_vert[loop] *= masterptr->scale;
		world_vert[loop] *= scale;
		world_vert[loop].rotate(masterptr->angle);
		world_vert[loop].rotate(angle);
		world_vert[loop] += pos;
	}
}

void Instance::world_to_viewer(Viewer &user)
{
	// The viewing plance position
	float vrp = -50.0;

	// The back plane position
	float BACK = -75.0;

	view_vert.clear();

	for (auto vertex : world_vert)
	{
		vertex -= user.loc;
		vertex.rotate(-user.ang);

		// normalize the points ready for perspective projection
		// (-2.0 here to make the y value correct - i.e. -y down and +y up)
		vertex.scale((2.0 * vrp) / (100.0 * (vrp+BACK)),
					 (-2.0 * vrp) / (75.0 * (vrp+BACK)),
					 -1.0 / (vrp+BACK));

		view_vert.push_back(vertex);
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

	assert(world_vert.size() == view_vert.size());
}

void Instance::setup_color(Viewer &user, Light &light)
{
	int no_polygons, loop;
	int poly_no[3];

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

		Vector3d v1 = world_vert[poly_no[2]] - world_vert[poly_no[1]];
		Vector3d v2 = world_vert[poly_no[1]] - world_vert[poly_no[0]];

		Vector3d normal = v1.cross(v2);
		normal.normalize();

		// Apply just ambient light to start with
		poly_color[loop] = user.ambient * user.ambient_intensity * color;
		
		// Calulate specular highlight
		Vector3d view = world_vert[poly_no[0]] - user.loc;
		view.normalize();

		Vector3d ref = user.g_light_dir;
		ref.reflect(normal);

		float specular = (specularity / 100.0f) * view.dot(ref);
		
		// Now do global 'sun' light
		float cos_theta = normal.dot(user.g_light_dir);
		if (cos_theta > 0.0)
		{

			Color global = color * user.g_light * cos_theta;
			poly_color[loop] += global + Color(specular, specular, specular);
		}

		// Now do light
		Vector3d light_dir = light.pos - world_vert[poly_no[0]];
		light_dir.normalize();

		if (light_dir.dot(normal) > 0.0)
		{
			Color diffuse = color * light.col * light_dir.dot(normal);
			poly_color[loop] += diffuse + Color(specular, specular, specular);
		}
	}
}

