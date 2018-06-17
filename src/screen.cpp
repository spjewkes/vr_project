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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.hpp"
#include "error.hpp"
#include "graphics.hpp"
#include "pfuncs.hpp"

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
int screen_open(int mode)
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
		return(error("0100", "Illegal graphics mode", 0));
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
		return (ERROR);
	}

	return (OKAY);
}

/****************************************************************************
* check_image() - a quick debugging tool to display the instance values in  *
*                 rough graphical form just a brief check to see if the     *
*                 values are being stored correctly                         *
****************************************************************************/
void draw_image(struct master *mptr, struct instance *iptr, int no_instances, struct viewer user)
{
	int loop1, loop2, loop3;
	int master_no;
	int edge0, edge1;
	float x, y, z;
	float x1,y1,z1, x2,y2,z2, x3,y3,z3;
	float dx1, dy1, dz1, dx2, dy2, dz2;
	float A, B, C, D;
	float zmin;
	int midx, midy;
	int polyarray[18];
	int poly_no[3];
	int polyptr, no_points;
	int tmp;
	/* the viewing plance position */
	float vrp = -50.0;
	/* the back plane position */
	float BACK = -75.0;

	/* make the arrays static - this seems to stop them conflicting with
	   the dynamically allocated arrays */
	/* try removing static on the depth array and watch the machine
	   crash during the build up of your complex models :-) */
	static long depth_array[256][2];
	static float pre_array[8][3];
	static float post_array[8][3];
	static float store[256][3];

	/* first depth sort all the objects */
	/* do we want to remove those objects which are behind us? */
	for (loop1 = 0; loop1 < no_instances; loop1++)
	{
		/* reference to which instance */
		depth_array[loop1][0] = loop1;
		/* the distance away from the viewer we use pythagoras */
		/* here we dont't need to find the square root as all distances */
		/* are still going to be relative */
		Vector3d hyp1 = iptr[loop1].min - user.loc;
		Vector3d hyp2 = iptr[loop1].max - user.loc;
		depth_array[loop1][1] = static_cast<long>(std::fmax(hyp1.length2(), hyp2.length2()));
	}

	/* smallest values come first */
	for (loop1 = 0; loop1 < no_instances; loop1++)
		for (loop2 = 0; loop2 < no_instances-1; loop2++)
		{
			if (depth_array[loop2][1] < depth_array[loop2+1][1])
			{
				/* swap the array values over */
				tmp = depth_array[loop2][0];
				depth_array[loop2][0] = depth_array[loop2+1][0];
				depth_array[loop2+1][0] = tmp;
				tmp = depth_array[loop2][1];
				depth_array[loop2][1] = depth_array[loop2+1][1];
				depth_array[loop2+1][1] = tmp;
			}
		}

	/* set up the values for the middle of the screen */
	midx = getmaxx() / 2;
	midy = getmaxy() / 2;
	/* the front plane of the view screen - must be less than zero */
	/* zmin = -1.0 * (vrp-1.0) / (vrp+BACK); */
	/* set zmin making sure that it's negative */
	zmin = -0.1;

	/* before we draw the scene we want to draw the sky and the ground */
	/* the sky */
	setcolor(user.sky);
	bar(0, 0, getmaxx(), midy);
	/* the ground */
	setcolor(user.ground);
	bar(0, midy, getmaxx(), getmaxy());

	for (loop1 = 0; loop1 < no_instances; loop1++)
	{
		/* make tmp hold the value of the current instance */
		tmp = depth_array[loop1][0];

		loop2 = 0;

		/* for every instance */
		/* rotate and translate all of the vertices so
		   it's relative to the viewer */
		for (auto vertex : iptr[tmp].vert)
		{
			/* get the vertex point */
			x = vertex.x();
			y = vertex.y();
			z = vertex.z();
			/* set coordinate so that viewer is at origin */
			translate(&x, &y, &z, 0-user.loc.x(), 0-user.loc.y(), 0-user.loc.z());
			rotate(&x, &y, &z, 0-user.ang.x(), 0-user.ang.y(), 0-user.ang.z());
			/* now normalise the points ready for the perspective
			   projection */
			x = x * ((2.0 * vrp) / (100.0 * (vrp+BACK)));
			/* minus 2.0 here to make the y value correct
			   i.e. -y - down and +y - up */
			y = y * ((-2.0 * vrp) / (75.0 * (vrp+BACK)));
			z = z * (-1.0 / (vrp+BACK));
			/* now store them in temporary instance space */
			store[loop2][X] = x;
			store[loop2][Y] = y;
			store[loop2][Z] = z;
			loop2++;
		}

		/* get the master no of the of master that stores
		   the instance's object */
		master_no = iptr[tmp].master_no;

		/* now we check whether to draw the image as a wireframe or solid */
		if (iptr[tmp].style == RenderStyle::WIREFRAME)
		{
			for (size_t i = 0; i < mptr[master_no].edge0.size(); i++)
			{
				/* get the value of the start and ends of the edges */
				edge0 = mptr[master_no].edge0[i];
				edge1 = mptr[master_no].edge1[i];
				/* get the first vertex of the edge */
				x1 = store[edge0][X];
				y1 = store[edge0][Y];
				z1 = store[edge0][Z];
				/* get the second vertex of the edge */
				x2 = store[edge1][X];
				y2 = store[edge1][Y];
				z2 = store[edge1][Z];
				/* set up the values ready for clipping */
				if (clip3d(&x1, &y1, &z1, &x2, &y2, &z2, zmin))
				{
					/* project onto a 2D monitor */
					x1 = (((x1*-1.0)/z1) * midx) + midx;
					y1 = (((y1*-1.0)/z1) * midy) + midy;
					x2 = (((x2*-1.0)/z2) * midx) + midx;
					y2 = (((y2*-1.0)/z2) * midy) + midy;
					/* set its colour */
					setcolor(iptr[tmp].edge_colour[i]);
					/* draw line if it's in the viewing volume */
					line(x1, y1, x2, y2);
				}
			}
		}
		else if (iptr[tmp].style == RenderStyle::SOLID)
		{
			/* now let's examine the three edges */
			for(size_t i = 0; i < mptr[master_no].poly0.size(); i++)
			{
				/* get the three edges that build up the polygon */
				poly_no[0] = mptr[master_no].poly0[i];
				poly_no[1] = mptr[master_no].poly1[i];
				poly_no[2] = mptr[master_no].poly2[i];

				/* before we go any further let's make
				   sure the polygon is visible */
				/* now let's deal with the first edge */
				edge0 = mptr[master_no].edge0[(poly_no[0])];
				edge1 = mptr[master_no].edge1[(poly_no[0])];
				/* get the first vertex of that edge */
				x1 = store[edge0][X];
				y1 = store[edge0][Y];
				z1 = store[edge0][Z];
				/* now get the second vertex of that edge */
				x2 = store[edge1][X];
				y2 = store[edge1][Y];
				z2 = store[edge1][Z];
				/* we need a third point to find the normal to the plane */
				/* so we'll get the end point of the second edge */
				edge1 = mptr[master_no].edge1[(poly_no[1])];
				x3 = store[edge1][X];
				y3 = store[edge1][Y];
				z3 = store[edge1][Z];
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
				A = (dy1 * dz2) - (dy2 * dz1);
				B = (dz1 * dx2) - (dz2 * dx1);
				C = (dx1 * dy2) - (dx2 * dy1);
				/* now calculate D using a point that is known to be */
				/* on the plane */
				D = (A * x1) + (B * y1) + (C * z1);

				/* so let's check if the surface is visble */
				if (D < 0)
				{
					/* now we know that part of the surface is facing us
					   we must now clip it */
					/* we already have the first edge so let's store it */
					/* start point */
					pre_array[0][X] = x1;
					pre_array[0][Y] = y1;
					pre_array[0][Z] = z1;
					/* end point */
					pre_array[1][X] = x2;
					pre_array[1][Y] = y2;
					pre_array[1][Z] = z2;
					/* now get the start point of the second edge */
					edge0 = mptr[master_no].edge0[(poly_no[1])];
					pre_array[2][X] = store[edge0][X];
					pre_array[2][Y] = store[edge0][Y];
					pre_array[2][Z] = store[edge0][Z];
					/* we've already got the end point of the second edge so
					   we store that also */
					pre_array[3][X] = x3;
					pre_array[3][Y] = y3;
					pre_array[3][Z] = z3;
					/* now move on to the third edge */
					/* the start point */
					edge0 = mptr[master_no].edge0[(poly_no[2])];
					pre_array[4][X] = store[edge0][X];
					pre_array[4][Y] = store[edge0][Y];
					pre_array[4][Z] = store[edge0][Z];
					/* and now the end point */
					edge1 = mptr[master_no].edge1[(poly_no[2])];
					pre_array[5][X] = store[edge1][X];
					pre_array[5][Y] = store[edge1][Y];
					pre_array[5][Z] = store[edge1][Z];
					/* finally fill in the last edge made from the start
					   of the first edge and the end of the last edge */
					/* the start point */
					pre_array[6][X] = pre_array[5][0];
					pre_array[6][Y] = pre_array[5][1];
					pre_array[6][Z] = pre_array[5][2];
					/* the end point */
					pre_array[7][X] = x1;
					pre_array[7][Y] = y1;
					pre_array[7][Z] = z1;
					/* call the polygon clipping routine */
					no_points = clip3dpara(pre_array, post_array, zmin);
					/* now it's been clipped let's check whether we've
					   got anything left to draw on the screen */
					if (no_points > 0)
					{
						/* set the polysize value to 0 */
						polyptr = 0;
						/* fill the polyarray with values in post_array */
						for (loop3 = 0; loop3 < no_points; loop3++)
						{
							x = post_array[loop3][X];
							y = post_array[loop3][Y];
							z = post_array[loop3][Z];
							/* project onto a 2D monitor */
							x = (((x*-1.0)/z) * midx) + midx;
							y = (((y*-1.0)/z) * midy) + midy;
							polyarray[polyptr++] = (int)(x);
							polyarray[polyptr++] = (int)(y);
						}
						/* now make the last two points of the polygon */
						/* equal to the fist two pointm */
						polyarray[polyptr++] = polyarray[X];
						polyarray[polyptr++] = polyarray[Y];

						no_points++;
						
						/* firstly set the colour */
						setcolor(iptr[tmp].poly_colour[i]);
						/* now draw the polygon */
						fillpoly(no_points, polyarray);
					}
				}
			}
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
	for (loop = 0; loop < 7; loop += 2)
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
