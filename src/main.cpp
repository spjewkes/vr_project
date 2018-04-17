/**************************************
*                                     *
* ##   ##  ###  #### ##   ##     #### *
* ### ### ## ##  ##  ###  ##    ##    *
* ####### #####  ##  #######    ##    *
* ## # ## ## ##  ##  ##  ###    ##    *
* ##   ## ## ## #### ##   ## ##  #### *
**************************************/
/****************
* include files *
****************/
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include "defs.h"
/*************************
* prototype declarations *
*************************/
void main(int argc, char *argv[]);
/************************
* external declarations *
************************/
extern int parse(char *filename);
extern int error(char *errno, char *message, int line_no);
extern void remove_master(int no_objects);
extern void remove_lnstance(int no_instances);
extern void debug(char *string, int level);
extern void check_master(struct master *ptr, int no_rows);
extern void check_instance(struct instance *ptr, int no_rows);
extern int screen_open(int mode);
extern void draw_image(struct master *mptr, struct instance *iptr, int no_instances);
extern void create_palette(void);
extern void check_palette(void);
extern int check_col(float locx, float locy, float locz, struct instance *instanceptr, int no_instances, struct viewer user);
extern int mouse_info(int *x, int *y);
extern void draw_pointer(int x, int y);
extern void check_pointer(int *x, int *y);
extern int hit_object(int mpos_x, int mpos_y, struct viewer user, struct instance *instanceptr, int no_instances);
extern void program(int instance, struct instance *ptr);

/*******************
* global variables *
*******************/
char LINE[MAXLINE];
int lincnt;
int lineptr;
int masterdef_processed;
int no_masters;
int no_instances;
int stage;
struct master *masterptr;
struct instance *instanceptr;
struct viewer user;
void *fp;
/************
* functions *
************/
void main(int argc, char *argv[])
{
        float locx, locy, locz;
        float angle;
        int mode, instance, prev_inst;
        char c;

        /* variables for the mouse */
        int status;
        int x, y;
        int mpos_x, mpos_y;
        /* set phase of implementation ,/
        /* TEST: prints debug messages */
        /* FINAL: doesn't show any of the parsing */
        stage = FINAL;
        debug("START OF PROGRAM", 3);
        debug("main()", 1);
        /* set mode to a value of 0 - resolution 320x200 */
        mode = O;
        /* if there are more than 2 arguments then error */
        if (argc > 2)
        (
                error("0037", "Too many parameters", 0);
                exit(0);
        }

        /* if parsing script results in an error then leave program */
        if (parse(argv[1]) == ERROR)
                exit(0);

        /* display the contents of the master structure */
        /* check_master(masterptr, no masters); */
        /* display the contents of the instance structure */
        /* check_instance(instanceptr, no_instances); */
        /* now let's start the really interesting bit */
        printf("\nPRESS ANY KEY TO ENTER ANOTHER WORLD...\n");
        getch();

        /* open up the graphics screen */
        if (screen_open(mode) == OKAY)
        {
                /* set the palette up */
                create_palette();
                /* check the contents of the palette */
                /* check_palette();                  */
                /* draw the initial position of all the objects */
                draw_image(masterptr, instanceptr, no_instances);
                /* set the value of c to a null value to begin with */
                c = '\0';
                while (c != 'Q')
                (
                        /* get temporary values of position of user to check collision
                           with any object instances */
                        locx = user.locx;
                        locy = user.locy;
                        locz = user.locz;

                        /* set the initial starting position of the mouse */
                        mpos_x = getmaxx() / 2;
                        mpos_y = getmaxy() / 2;

                        /* now draw the pointer */
                        draw_pointer(mpos_x, mpos_y);

                        /* while the keyboard isn't touched we want to allow the user
                           to click on objects that they can see */
                        while (!kbhit())
                        {
                                /* get the current information from the mouse */
                                status = mouse_info(&x, &y);
                                /* check to see if the left mouse button was pressed */
                                if (status == 1)
                                {
                                        /* wait until mouse button is released before
                                           going any further */
                                        while ((status = mouse_info(&x, &y)) == 1);
                                        /* see if we hit an objeot and act accordingly */
                                        instance = hit_object(mpos_x, mpos_y, user, instanceptr, no_instances);
                                        /* if this is the second click on the same object then
                                           we run it */
                                        if (instance == prey inst)
                                        {
                                                /* first let's clear the pointer */
                                                draw_pointer(mpos_x, mpos_y);
                                                /* now see if we can execute the program */
                                                if (instance != ERROR)
                                                        program(instance, instanceptr);
                                                /* wait for the right mouse button to be pressed */
                                                while ((status = mouse_info(&x, &y)) != 2);
                                                /* draw the new image */
                                                draw_image(masterptr, instanceptr, no_instances);
                                                /* reset the palette - this takes care of any
                                                   programs that may alter it */
                                                create_palette();
                                                /* finally redraw the pointer */
                                                draw_pointer(mpos_x, mpos_y);
                                                /* so that another double click is needed */
                                                prev_inst = ERROR;
                                        }
                                        else
                                        {
                                                /* set previous instance to this one */
                                                prev_inst = instance;
                                        }
                                }
                                /* if the mouse has been moved then draw it's new position */
                                if ((x != 0) || (y != 0))
                                {
                                        /* first wipe the old pointer - we simply redraw it
                                           with its original position using XOR mode */
                                        draw_pointer(mpos_x, mpos_y) ;
                                        /* now modify the positions of x and y */
                                        mpos_x += x;
                                        mpos_y += y;
                                        /* now just check that the x and y values aren't
                                           out of range of the screen */
                                        check_pointer(&mpos_x, &mpos_y);
                                        /* now draw the pointer */
                                        draw_pointer(mpos_x, mpos_y);
                                }
                        )
                        /* retrieve the value from the keyboard */
                        c = getch();

                        /* now we see which key was pressed and act accordingly */
                        switch (c)
                        {
                                case '8':
                                {
                                        /* walk forward */
                                        /* angle 0 is facing forwards so must decrement by 90 */
                                        angle = user.angy + 90.0;
                                        if (angle > 360.0) angle -= 360.0;
                                        locz = locz - (2.5 * sin(angle / RADCONST));
                                        locx = locx - (2.5 * cos(angle / RADCONST));
                                        if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
                                        {
                                                /* if user hasn't collided with an object instance
                                                   then update the user's position */
                                                user.locx = locx;
                                                user.locz = locz;
                                        }
                                        break;
                                )
                                case '2':
                                {
                                        /* walk backwards */
                                        /* angle 0 is facing forwards so nust increment by 90 */
                                        angle = user.angy + 90.0;
                                        if (angle > 360.0) angle -= 360.0;
                                        locz = locz + (2.5 * sin(angle / RADCONST));
                                        locx = locx + (2.5 * cos(angle / RADCONST));
                                        if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
                                        (
                                                /* if user hasn't collided with an object instance
                                                   then update the user's posltion */
                                                user.locx = locx;
                                                user.locz = locz;
                                        }
                                        break;
                                )
                                case '6':
                                {
                                        /* slide left */
                                        locz = locz + (2.5 * sin(user.angy / RADCONST));
                                        locx = locx + (2.5 * cos(user.angy / RADCONST));
                                        if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
                                        {
                                                /* if user hasn't collided with an object instance
                                                   then update the user's position */
                                                user.locx = locx;
                                                user.locz = locz;
                                        )
                                        break;
                                )
                                case '4':
                                (
                                        /* slide right */
                                        locz = locz - (2.5 * sin(user.angy / RADCONST));
                                        locx = locx - (2.5 * cos(user.angy / RADCONST));
                                        if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
                                        (
                                                /* if user hasn't colllded wlth an object instance
                                                   then update the user's position */
                                                user.locx = locx;
                                                user.locz = locz;
                                        }
                                        break;
                                )
                                case '7':
                                (
                                        /* turn to the left */
                                        user.angy -= 2.5;
                                        if (user.angy < 0.0) user.angy = 360.0;
                                        break;
                                )
                                case '9':
                                (
                                        /* turn to the right */
                                        user.angy += 2.5;
                                        if (user.angy > 360.0) user.angy = 0.0;
                                        break;
                                }
                        )
                        /* draw the new image */
                        draw_image(masterptr, instanceptr, no_instances);
                }
                /* close the graphics screen */
                closegraph();
        }

        /* free memory taken up by master objects */
        remove_master(no_masters);
        /* free memory taken up by instances */
        remove_instance(no_instances);

        debug("END OF PROGRAM", 3);
}
