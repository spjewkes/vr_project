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

