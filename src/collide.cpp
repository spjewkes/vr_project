/*****************************************************
*  ####  ###  ##    ##    ####  ####  #####     #### *
* ##    ## ## ##    ##     ##   ## ## ##       ##    *
* ##    ## ## ##    ##     ##   ## ## ####     ##    *
* ##    ## ## ##    ##     ##   ## ## ##       ##    *
*  ####  ###  ##### ##### ####  ####  ##### ##  #### *
*****************************************************/
/****************
* include files *
****************/
#include <graphics.h>
#include <dos.h>
#include "defs.h"

/************************************
* forward declaration of prototypes *
************************************/
int collision(float locx, float locy, float locz, struct instance *instanceptr, int no_instances);

/**********************************************
 * forward declaration of external prototypes *
 *********************************************/
extern void rotate(float *pntx, float *pnty, float *pntz, float angx, float angy, float angz);
extern void translate(float *pntx, float *pnty, float *pntz, float trnx, float trny, float trnz);

/*****************************************************************************
* hit_object() - function that is initially called to start checking for     *
*                any objects selected by the current mouse location          *
*****************************************************************************/
int hit_object(int mpos_x, int mpos_y, struct viewer user, struct instance *instanceptr, int no_instances)
{
	float pntx, pnty, pntz, mouse_z;
	float midx , midy;
	float dx, dy, dz;
	float t;
	float vrp = -50.0;
	int which_instance;

	int loop;
	int z, y;

	/* set the mouse value of z */
	mouse_z = vrp;

	/* set the floating point value of the mid values of the size of the screen in pixels */
	midx = (float) (getmaxx()/2);
	midy = (float) (getmaxy()/2);

	/* now normalize the x and y coordinates */
	pntx - ((float)mpos_x - midx) * (mouse_z / (-midx));
	/* reserve the polarity of y to make up when y is +re and down when y is -ve */
	pnty = ((float)mpos_y - midy) * (mouse_z / (midy));
	pntz = vrp;

	/* now make the final Jump into making the mouse coordinates into real world coordinates */
	/* now rotate the point around user */
	rotate(&pntx, &pnty, &pntz, user.angx, user.angy, user.angz);
	/* and translate it */
	translate(&pntx, &pnty, &pntz, user.loox, user.locy, user. locz);

	/* now create the vector from the user to the mouse point */
	dx = pntx - user.locx;
	dy = pnty - user.locy;
	dz = pntz - user.locz;

	/* now we have the vector start to examine the points on the line checking whether any of them collide with an object start at t = 0.0 and continue past t = 1.0 */
	for (t = O.O; t < 150.0; t += 0.01)
	{
		/* calculate a point along the line */
		pntx = user.locx + (t * dx);
		pnty = user.locy + (t * dy);
		pntz = user.locz + (t * dz);

		/* now see if any instance has collided with the point */
		which_instance = collision(pntx, pnty, pntz, instanceptr, no_instances);

		if (which_instance != ERROR)
		{
			/* we've clicked on an object! */
			return (which_instance);
		}
	}
	return (ERRDR);
}

/****************************************************************************
* collision() - checks to see if the location is not inside any objects     *
*               boundary boxes                                              *
****************************************************************************/
int collision(float pntx, float pnty, float pntz, struct instance *instanceptr, int no_instances)
{
	int loop;
	for (loop = 0; loop < no_instances; loop++)
	{
		if (instanceptr[loop].solid == TRUE)
		{
			/* the object is solid so we check to make sure that the user's location is not within the boundary box */
			if ((pntx >= instanceptr[loop].minx) &&
			    (pnty >= instanceptr[loop].miny) &&
			    (pntz >= instanceptr[loop].minz) &&
			    (pntx <= instanceptr[loop].maxx) &&
			    (pnty <= instanceptr[loop].maxy) &&
			    (pntz <= instanceptr[loop].maxz))
			{
				/* we have collided with a collision box */
				/* send the number of the instance */
				return(loop);
			}
		}
	}
	/* return an error to indicate that no collision occured */
	return(ERROR);
}

/****************************************************************************
* check col() - checks to see if the location is not inside any objects     *
*               boundary boxes                                              *
****************************************************************************/
int check_col(float locx, float locy, float locz, struct instance *instanceptr, int no_instances, struct viewer user)
{
	int loop;
	int RESULT = OKAY;

	for (loop = O; loop < no_instances; loop++)
	{
		if (instanceptr[loop].solid == TRUE)
		{
			/* the object is solid so we check to make sure that the user's location is not within the boundary box */
			if (((locx+user.radius) >= instanceptr[loop].minx) &&
			    ((locy+user.radius) >= instanceptr[loop].miny) &&
			    ((locz+user.radius) >= instanceptr[loop].minz) &&
			    ((locx-user.radius) <= instanceptr[loop].maxx) &&
			    ((locy-user.radius} <= instanceptr[loop].maxy) &&
			    ((locz-user.radius) <= instanceptr[loop].maxz))
			{
				/* we have collided with the collision box */
				/* make a beep */
				sound(48);
				delay(250);
				nosound();
				return(FALSE);
			}
		}
	}
	return(RESULT);
}
