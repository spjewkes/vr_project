/*******************************************
*                                          *
* ##   ##  ###  ## ##  #### #####     #### *
* ### ### ## ## ## ## ##    ##       ##    *
* ####### ## ## ## ## ##### ####     ##    *
* ## # ## ## ## ## ##    ## ##       ##    *
* ##   ##  ###   ###  ####  ##### ##  #### *
*                                          *
*******************************************/
/****************
* include files *
****************/
#include <stdio.h>
#include "graphics.hpp"
#include "defs.hpp"

/*****************************************************************************
* draw_pointer() - function which simply draws the mouse pointer onto the    *
*                  screen - it can be anything at all as long as it fits     *
*                  inside the MSIZE defined boundary                         *
*****************************************************************************/
void draw_pointer(int x, int y)
{
	/* set the mouse colour to white */
	setcolor(WHITE);
	/* now draw the mouse pointer */
	line(x,y-MSIZE,x,y+MSIZE);
	line(x-MSIZE,y,x+MSIZE,y);
}

/*****************************************************************************
* check_pointer() - this function makes sure that the pointer isn't going    *
*                   to be drawn off screen - the reason for this is that     *
*                   the functions getimage() and putimage() tend to crash    *
*                   the machine if we don't apply some boundary checking     *
*****************************************************************************/
void check_pointer(int *x, int *y)
{
	/* the minimum x can be */
	if (*x < 0+MSIZE) *x = 0+MSIZE;
	/* the maximum x can be */
	if (*x > getmaxx()-MSIZE) *x = getmaxx()-MSIZE;
	/* the minimum y can be */
	if (*y < 0+MSIZE) *y = 0+MSIZE;
	/* the maximum y can be */
	if (*y > getmaxy()-MSIZE) *y = getmaxy()-MSIZE;
}
