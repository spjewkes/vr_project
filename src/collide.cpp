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
#include "instance.hpp"
#include "pfuncs.hpp"
#include "sound.hpp"
#include "viewer.hpp"

/****************************************************************************
* collision() - checks to see if the location is not inside any objects     *
*               boundary boxes                                              *
****************************************************************************/
int collision(float pntx, float pnty, float pntz, std::vector<Instance> &instances)
{
	int idx = 0;

	for (auto inst : instances)
	{
		if (inst.is_solid)
		{
			/* the object is solid so we check to make sure that the user's location is not within the boundary box */
			if ((pntx >= inst.min.x()) &&
			    (pnty >= inst.min.y()) &&
			    (pntz >= inst.min.z()) &&
			    (pntx <= inst.max.x()) &&
			    (pnty <= inst.max.y()) &&
			    (pntz <= inst.max.z()))
			{
				/* we have collided with a collision box */
				/* send the number of the instance */
				return idx;
			}
		}
		idx++;
	}

	/* return an error to indicate that no collision occured */
	return -1;
}

/*****************************************************************************
* hit_object() - function that is initially called to start checking for     *
*                any objects selected by the current mouse location          *
*****************************************************************************/
int hit_object(int mpos_x, int mpos_y, Viewer &user, std::vector<Instance> &instances)
{
	float mouse_z;
	float midx , midy;
	float t;
	float vrp = -50.0;
	int which_instance;

	/* set the mouse value of z */
	mouse_z = vrp;

	/* set the floating point value of the mid values of the size of the screen in pixels */
	midx = (float) (getmaxx()/2);
	midy = (float) (getmaxy()/2);

	/* now normalize the x and y coordinates */
	/* reserve the polarity of y to make up when y is +re and down when y is -ve */
	Vector3d mousepnt((static_cast<float>(mpos_x) - midx) * (mouse_z / -midx),
					  (static_cast<float>(mpos_y) - midy) * (mouse_z / midy),
					  vrp);

	/* now make the final Jump into making the mouse coordinates into real world coordinates */
	/* now rotate the point around user */
	mousepnt.rotate(user.ang);
	/* and translate it */
	mousepnt += user.loc;

	/* now create the vector from the user to the mouse point */
	Vector3d d(mousepnt - user.loc);

	/* now we have the vector start to examine the points on the line checking whether any of
	   them collide with an object start at t = 0.0 and continue past t = 1.0 */
	for (t = 0.0; t < 150.0; t += 0.01)
	{
		/* calculate a point along the line */
		mousepnt = user.loc + (d * t);

		/* now see if any instance has collided with the point */
		which_instance = collision(mousepnt.x(), mousepnt.y(), mousepnt.z(), instances);

		if (which_instance != -1)
		{
			/* we've clicked on an object! */
			return which_instance;
		}
	}
	return -1;
}

/****************************************************************************
* check col() - checks to see if the location is not inside any objects     *
*               boundary boxes                                              *
****************************************************************************/
bool check_col(float locx, float locy, float locz, Viewer &user, std::vector<Instance> &instances)
{
	bool collide = false;

	for (auto inst : instances)
	{
		if (inst.is_solid)
		{
			/* the object is solid so we check to make sure that the user's location is not within the boundary box */
			if (((locx+user.radius) >= inst.min.x()) &&
			    ((locy+user.radius) >= inst.min.y()) &&
			    ((locz+user.radius) >= inst.min.z()) &&
			    ((locx-user.radius) <= inst.max.x()) &&
			    ((locy-user.radius) <= inst.max.y()) &&
			    ((locz-user.radius) <= inst.max.z()))
			{
				/* we have collided with the collision box */
				/* make a beep */
				start_blip();
				play_blip();
				stop_blip();
				collide = true;
				break;
			}
		}
	}
	return collide;
}
