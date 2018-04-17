/*******************************************
* ##   ##  ###  ## ##  #### #####     #### *
* ### ### ## ## ## ## ##    ##       ##    *
* ####### ## ## ## ## ##### ####     ##    *
* ## # ## ## ## ## ##    ## ##       ##    *
* ##   ##  ###   ###  ####  ##### ##  #### *
********************************************
/****************
* include files *
****************/
#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include "defs.h"
/*****************************************************************************
* mouse_info() - this function sends the offsets of the mouse position in    *
*                relation to its last position and information on the        *
*                current status of the mouse buttons:                        *
*                1 = left; 2 = right; 3 = both                               *
*****************************************************************************/
int mouse_info(int *x, int *y)
{
        union REGS regs;
        /* we want to call the BIOS function that tells us the physical
           movement of the mouse pointer */
        regs.x.ax = 0x0b;
        /* interrupt call */
        int86(MOUSE, &regs, &regs);
        /* now load the returning values to the parameters */
        *x = regs.x.cx;
        *y = regs.x.dx;
        /* we want to call the BIOS function that tells us the button-press
           information of the mouse */
        regs.x.ax = 0x03;
        /* interrupt call */
        int86(MOUSE, &regs, &regs);
        /* now return the button status */
        return(regs.x.bx);
)

/*****************************************************************************
* draw_pointer() - function which simply draws the mouse pointer onto the    *
*                  screen - it can be anything at all as long as it fits     *
*                  inside the MSIZE defined boundary                         *
*****************************************************************************/
void draw_pointer(int x, int y)
{
        /* set the mouse colour to white */
        setcolor(WHITE);
        /* set the drawing mode to XOR mode */
        setwritemode(XOR_PUT);
        /* now draw the mouse pointer */
        line(x,y-MSIZE,x,y+MSIZE);
        line(x-MSIZE,y,x+MSIZE,y);
        /* now set the drawing mode back to normal */
        setwritemode(COPY_PUT);
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
