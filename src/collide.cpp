/*****************************************************
*                                                    *
*  ####  ###  ##    ##    ####  ####  #####     #### *
* ##    ## ## ##    ##     ##   ## ## ##       ##    *
* ##    ## ## ##    ##     ##   ## ## ####     ##    *
* ##    ## ## ##    ##     ##   ## ## ##       ##    *
*  ####  ###  ##### ##### ####  ####  ##### ##  #### *
*                                                    *
*****************************************************/
/****************
* include files *
****************/
#include "defs.hpp"
#include "graphics.hpp"
#include "pfuncs.hpp"
#include "sound.hpp"

/************************************
* forward declaration of prototypes *
************************************/
int collision(float locx, float locy, float locz, struct instance *instanceptr, int no_instances);

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

	/* set the mouse value of z */
	mouse_z = vrp;

	/* set the floating point value of the mid values of the size of the screen in pixels */
	midx = (float) (getmaxx()/2);
	midy = (float) (getmaxy()/2);

	/* now normalize the x and y coordinates */
	pntx = ((float)mpos_x - midx) * (mouse_z / (-midx));
	/* reserve the polarity of y to make up when y is +re and down when y is -ve */
	pnty = ((float)mpos_y - midy) * (mouse_z / (midy));
	pntz = vrp;

	/* now make the final Jump into making the mouse coordinates into real world coordinates */
	/* now rotate the point around user */
	rotate(&pntx, &pnty, &pntz, user.ang.x(), user.ang.y(), user.ang.z());
	/* and translate it */
	translate(&pntx, &pnty, &pntz, user.loc.x(), user.loc.y(), user.loc.z());

	/* now create the vector from the user to the mouse point */
	dx = pntx - user.loc.x();
	dy = pnty - user.loc.y();
	dz = pntz - user.loc.z();

	/* now we have the vector start to examine the points on the line checking whether any of them collide with an object start at t = 0.0 and continue past t = 1.0 */
	for (t = 0.0; t < 150.0; t += 0.01)
	{
		/* calculate a point along the line */
		pntx = user.loc.x() + (t * dx);
		pnty = user.loc.y() + (t * dy);
		pntz = user.loc.z() + (t * dz);

		/* now see if any instance has collided with the point */
		which_instance = collision(pntx, pnty, pntz, instanceptr, no_instances);

		if (which_instance != ERROR)
		{
			/* we've clicked on an object! */
			return (which_instance);
		}
	}
	return (ERROR);
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

	for (loop = 0; loop < no_instances; loop++)
	{
		if (instanceptr[loop].solid == TRUE)
		{
			/* the object is solid so we check to make sure that the user's location is not within the boundary box */
			if (((locx+user.radius) >= instanceptr[loop].minx) &&
			    ((locy+user.radius) >= instanceptr[loop].miny) &&
			    ((locz+user.radius) >= instanceptr[loop].minz) &&
			    ((locx-user.radius) <= instanceptr[loop].maxx) &&
			    ((locy-user.radius) <= instanceptr[loop].maxy) &&
			    ((locz-user.radius) <= instanceptr[loop].maxz))
			{
				/* we have collided with the collision box */
				/* make a beep */
				start_blip();
				play_blip();
				stop_blip();
				return(FALSE);
			}
		}
	}
	return(RESULT);
}
