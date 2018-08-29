/************************************************
*                                               *
*  ####  #### ####  ##### ##### ##  ##     #### *
* ##    ##    ## ## ##    ##    ### ##    ##    *
* ##### ##    ####  ####  ####  ######    ##    *
*    ## ##    ## ## ##    ##    ## ###    ##    *
* ####   #### ## ## ##### ##### ##  ## ##  #### *
*                                               *
************************************************/
/****************
* include files *
****************/
#include <iostream>
#include <vector>
#include <forward_list>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "error.hpp"
#include "graphics.hpp"
#include "screen.hpp"

#define X  0
#define Y  1
#define Z  2

/************************************
* forward declaration of prototypes *
************************************/
bool clip3d(float *xs, float *ys, float *zs,
			float *xe, float *ye, float *ze, float minz);
int clip3dpara(float pre_array[][3], float post_array[][3], float minz);

/****************************************************************************
* screen open() - function to open a graphics screen in 256 vesa mode       *
*                 mode are: 0 - 320x200    1 - 640x400    2 - 640x480       *
*                           3 - 800x600    4 - 1024x768                     *
****************************************************************************/
Status screen_open(int mode)
{
	/* make sure that mode is of a legal value between 0 and 4 */
	if ((mode < 0) && (mode > 4))
	{
		printf("Try values:\n");
		printf("\t0 -  320x200\n");
		printf("\t1 -  640x400\n");
		printf("\t2 -  640x480\n");
		printf("\t3 -  800x600\n");
		printf("\t4 -  1024x768\n");
		return(error("Illegal graphics mode", 0));
	}

	int width = 0;
	int height = 0;

	switch (mode)
	{
	case 0:
		width = 320;
		height = 200;
		break;
	case 1:
		width = 640;
		height = 400;
		break;
	case 2:
		width = 640;
		height = 480;
		break;
	case 3:
		width = 800;
		height = 600;
		break;
	case 4:
		width = 1024;
		height = 768;
		break;
	}

	int retcode = create_graphics(width, height);
	if (retcode < 0)
	{
		return Error;
	}

	return Okay;
}

// Compare function for sorting instance
static bool compare_inst(const Instance *i1, const Instance *i2)
{
	return i1->order > i2->order;
}

// Structure used to list triangles for rendering
struct triangle
{
	int x0, y0;
	int x1, y1;
	int x2, y2;
	unsigned int color;
	float z;
};

// Compare function for sorting triangles
static bool compare_tri(const triangle &t1, const triangle &t2)
{
	return t1.z < t2.z;
}

// Render triangles to the screen, sorted in depth order
void render(std::vector<Instance> &instances, Viewer &user)
{
	std::forward_list<Instance*> inst_list;

	// Calculate middle of screen
	int midx = getmaxx() / 2;
	int midy = getmaxy() / 2;

	// The front plane of the view screen - must be less than zero
	// float zmin = -1.0 * (vrp-1.0) / (vrp+BACK);
	float zmin = -0.1;

	// Clear screen with sky and ground
	setcolor(user.sky);
	bar(0, 0, getmaxx(), midy);
	setcolor(user.ground);
	bar(0, midy, getmaxx(), getmaxy());

	// Prepare objects in the scene so they are relative to user
	for (auto &inst : instances)
	{
		inst.prerender(user);
		inst_list.push_front(&inst);
	}

	// Sort instances
	inst_list.sort(compare_inst);

	// Now loop through all instances and collect triangles to render
	for (auto inst : inst_list)
	{
		std::forward_list<triangle> tri_list;
		Master *masterptr = inst->masterptr;

		// Depending on the style, clip and project slightly differently
		if (inst->style == RenderStyle::WIREFRAME)
		{
			for (size_t j=0; j<masterptr->poly0.size(); j++)
			{
				unsigned int edge0[3] = { masterptr->poly0[j], masterptr->poly1[j], masterptr->poly2[j] };
				unsigned int edge1[3] = { masterptr->poly1[j], masterptr->poly2[j], masterptr->poly0[j] };

				for (size_t e=0; e<3; e++)
				{
					// Get the first vertex
					float x1 = inst->user_vert[edge0[e]].x();
					float y1 = inst->user_vert[edge0[e]].y();
					float z1 = inst->user_vert[edge0[e]].z();

					// Get the second vertex
					float x2 = inst->user_vert[edge1[e]].x();
					float y2 = inst->user_vert[edge1[e]].y();
					float z2 = inst->user_vert[edge1[e]].z();

					// Clip the line
					if (clip3d(&x1, &y1, &z1, &x2, &y2, &z2, zmin))
					{
						// Project line
						int ix1 = static_cast<int>((((x1*-1.0)/z1) * midx) + midx);
						int iy1 = static_cast<int>((((y1*-1.0)/z1) * midy) + midy);
						int ix2 = static_cast<int>((((x2*-1.0)/z2) * midx) + midx);
						int iy2 = static_cast<int>((((y2*-1.0)/z2) * midy) + midy);
					
						// Store triangle for later
						tri_list.push_front({ix1,iy1, ix2,iy2, ix2,iy2, inst->poly_color[j], (z1+z2)/2.0f});
					}
				}
			}
		}
		else if (inst->style == RenderStyle::SOLID)
		{
			for (size_t j=0; j<masterptr->poly0.size(); j++)
			{
				// Get first vertex
				float x1 = inst->user_vert[masterptr->poly0[j]].x();
				float y1 = inst->user_vert[masterptr->poly0[j]].y();
				float z1 = inst->user_vert[masterptr->poly0[j]].z();
				
				// Get first vertex
				float x2 = inst->user_vert[masterptr->poly1[j]].x();
				float y2 = inst->user_vert[masterptr->poly1[j]].y();
				float z2 = inst->user_vert[masterptr->poly1[j]].z();
				
				// Get first vertex
				float x3 = inst->user_vert[masterptr->poly2[j]].x();
				float y3 = inst->user_vert[masterptr->poly2[j]].y();
				float z3 = inst->user_vert[masterptr->poly2[j]].z();

				// Do back-face culling
				Vector3d v1(x2-x1, y2-y1, z2-z1);
				Vector3d v2(x3-x2, y3-y2, z3-z2);
				v1.cross(v2);

				Vector3d viewer(x1,y1,z1);
				if (v1.cross(v2).dot(viewer) < 0.0f)
				{
					// Now we know that the surface is facing the viewer, so clip it
					float pre_array[8][3];
					float post_array[8][3];

					pre_array[0][X] = x1;
					pre_array[0][Y] = y1;
					pre_array[0][Z] = z1;

					pre_array[1][X] = x2;
					pre_array[1][Y] = y2;
					pre_array[1][Z] = z2;

					pre_array[2][X] = x2;
					pre_array[2][Y] = y2;
					pre_array[2][Z] = z2;

					pre_array[3][X] = x3;
					pre_array[3][Y] = y3;
					pre_array[3][Z] = z3;

					pre_array[4][X] = x3;
					pre_array[4][Y] = y3;
					pre_array[4][Z] = z3;

					pre_array[5][X] = x1;
					pre_array[5][Y] = y1;
					pre_array[5][Z] = z1;

					int no_points = clip3dpara(pre_array, post_array, zmin);
					if (no_points > 0)
					{
						// Project points
						for (auto k=0; k<no_points; k++)
						{
							float x = post_array[k][X];
							float y = post_array[k][Y];
							float z = post_array[k][Z];

							post_array[k][X] = (((x*-1.0)/z) * midx) + midx;
							post_array[k][Y] = (((y*-1.0)/z) * midy) + midy;
						}

						// Now create list of triangles
						for (auto k=2; k<no_points; k++)
						{
							tri_list.push_front(
								{
									static_cast<int>(post_array[0][X]), static_cast<int>(post_array[0][Y]),
									static_cast<int>(post_array[k-1][X]), static_cast<int>(post_array[k-1][Y]),
									static_cast<int>(post_array[k][X]), static_cast<int>(post_array[k][Y]),
									inst->poly_color[j],
									(post_array[0][Z] + post_array[k-1][Z] + post_array[k][Z]) / 3.0f
								}
								);
						}
					}
				}
			}
		}

		// Sort triangles according to their z-value and then render
		tri_list.sort(compare_tri);

		for (auto tri : tri_list)
		{
			setcolor(tri.color);
			drawtri(tri.x0, tri.y0, tri.x1, tri.y1, tri.x2, tri.y2);
		}
	}
}

/****************************************************************************
* clipt() - function is based on pseudo code in Foley and van Dam book page *
*           122                                                             *
****************************************************************************/
bool clipt(float denom, float num, float *tE, float *tL)
{
	float t;
	bool accept = true;

	/* PE intersection */
	if (denom > 0.0)
	{
		/* value of t at the intersection */
		t = num / denom;
		if (t > *tL)
		{
			/* if tE and tL crossover then prepare to reject line */
			accept = false;
		}
		else if (t > *tE)
		{
			/* else a new tE has been found */
			*tE = t;
		}
	}
	else if (denom < 0.0)
	{
		/* this part is for a PL intersection */
		/* value of t at intersection */
		t = num / denom;
		if (t < *tE)
		{
			/* if tE and tL crossover then prepare to reject the line */
			accept = false;
		}
		else if (t < *tL)
		{
			/* else a new tL has been found */
			*tL = t;
		}
	}
	else
	{
		if (num > 0.0)
		{
			/* line is outside of edge */
			accept = false;
		}
	}
	/* return value of accept */
	return accept;
}

/*****************************************************************************
* clip3dpara() - function to clip a line to a 3d viewing volume - based on   *
*                the pseudo code in the Foley and van Dam book page 274      *
*                this function is for a canonical parallel-projection        *
*                viewing volume                                              *
*****************************************************************************/
int clip3dpara(float pre_array[][3], float post_array[][3], float minz)
{
	float tmin, tmax;
	float dx, dy, dz;
	int loop;
	int no_pnts;

	/* assume initially that none of the line is visible */
	/* set the no_pnts that visible */
	no_pnts = 0;

	/* for every edge in the array */
	for (loop = 0; loop < 5; loop += 2)
	{
		/* first we set the values for dx, dy and dz */
		dx = pre_array[loop+1][X] - pre_array[loop][0];
		dy = pre_array[loop+1][Y] - pre_array[loop][1];
		dz = pre_array[loop+1][Z] - pre_array[loop][2];
		/* now we initialise the values for train and tmax */
		tmin = 0.0;
		tmax = 1.0;

		/* now clip edge with the front plane */
		if (clipt(0.0-dz, pre_array[loop][2]-minz, &tmin, &tmax))
		{
			/* edge is visible and has now been clipped */
			/* if endpoint 0 (t=0) is not in the region */
			/* then compute the intersection */
			if (tmin > 0.0)
			{
				/* store them in the post_array */
				post_array[no_pnts][X] = pre_array[loop][X] + (tmin*dx);
				post_array[no_pnts][Y] = pre_array[loop][Y] + (tmin*dy);
				post_array[no_pnts][Z] = pre_array[loop][Z] + (tmin*dz);
			}
			else
			{
				/* we don't need any recalculation just copy straight over */
				post_array[no_pnts][X] = pre_array[loop][X];
				post_array[no_pnts][Y] = pre_array[loop][Y];
				post_array[no_pnts][Z] = pre_array[loop][Z];
			}
			/* increment the number of points the final array has */
			no_pnts++;
			/* if endpoint 1 (t=1) is not in the region */
			/* then compute the intersection */
			if (tmax < 1.0)
			{
				/* store them in the post_array */
				post_array[no_pnts][X] = pre_array[loop][X] + (tmax*dx);
				post_array[no_pnts][Y] = pre_array[loop][Y] + (tmax*dy);
				post_array[no_pnts][Z] = pre_array[loop][Z] + (tmax*dz);
			}
			else
			{
				/* we don't need any recalculation just copy straight over */
				post_array[no_pnts][X] = pre_array[loop+1][X];
				post_array[no_pnts][Y] = pre_array[loop+1][Y];
				post_array[no_pnts][Z] = pre_array[loop+1][Z];
			}
			/* increment the number of points the final array has */
			no_pnts++;
		}
	}
	/* return the value of the number of points */
	return (no_pnts);
}

/****************************************************************************
* clip3d() - function to clip a line to a 3d viewing volume - based on the  *
*            pseudo code in the Foley and van Dam book page 274             *
*            this function is for a canonical perspective-projection        *
*            viewing volume                                                 *
****************************************************************************/
bool clip3d(float *xs, float *ys, float *zs,
			float *xe, float *ye, float *ze, float minz)
{
	float tmin, tmax;
	float dx, dy, dz;
	bool accept = false;

	/* assume initially that none of the line is visible */
	tmin = 0.0;
	tmax = 1.0;
	dx = *xe - *xs;
	dz = *ze - *zs;

	if (clipt(-dx-dz, *xs+*zs, &tmin, &tmax))
	{
		/* right side */
		if (clipt(dx-dz, -*xs+*zs, &tmin, &tmax))
		{
			/* left side */
			/* if we get this far then part of the line is in -z<=x<=z */
			dy = *ye - *ys;
			if (clipt(dy-dz, -*ys+*zs, &tmin, &tmax))
			{
				/* bottom part */
				if (clipt(-dy-dz, *ys+*zs, &tmin, &tmax))
				{
					/* top part */
					/* if we get this far then part of the line
					   is visible in: */
					/* -z<=x<=z, -z<=y<=z */
					if (clipt(-dz, *zs-minz, &tmin, &tmax))
					{
						/* front plane */
						if (clipt(dz, -*zs-400.0, &tmin, &tmax))
						{
							/* back plane */
							/* if we get this far then part of */
							/* the line is visible in: */
							/* -z<=x<=z, -z<=y<=z, -l<=z<=zmin */
							accept = true;
							/* part of the line is visible */
							/* if endpoint 1 (t=1) is not in the region */
							/* then compute the intersection */
							if (tmax < 1.0)
							{
								*xe = *xs + (tmax * dx);
								*ye = *ys + (tmax * dy);
								*ze = *zs + (tmax * dz);
							}
							/* if endpoint 0 (t=0) is not in the region */
							/* then compute the intersection */
							if (tmin > 0.0)
							{
								*xs = *xs + (tmin * dx);
								*ys = *ys + (tmin * dy);
								*zs = *zs + (tmin * dz);
							}
						}
					}
				}
			}
		}
	}
	/* return the value of accept */
	return (accept);
}
