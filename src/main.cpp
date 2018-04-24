/**************************************
*                                     *
* ##   ##  ###  #### ##   ##     #### *
* ### ### ## ##  ##  ###  ##    ##    *
* ####### #####  ##  #######    ##    *
* ## # ## ## ##  ##  ##  ###    ##    *
* ##   ## ## ## #### ##   ## ##  #### *
*                                     *
**************************************/
/****************
* include files *
****************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <chrono>
#include "graphics.h"
#include "defs.h"
#include "hack.h"

/*************************
* prototype declarations *
*************************/
int main(int argc, char *argv[]);

/************************
* external declarations *
************************/
extern int parse(char *filename);
extern int error(const char *errno, const char *message, int line_no);
extern void remove_master(int no_objects);
extern void remove_instance(int no_instances);
extern void debug(const char *string, int level);
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
int main(int argc, char *argv[])
{
	float locx, locy, locz;
	float angle;
	int mode, instance, prev_inst;
	char c;

	/* variables for the mouse */
	int status;
	int x, y;
	int mpos_x, mpos_y;
	/* set phase of implementation */
	/* TEST: prints debug messages */
	/* FINAL: doesn't show any of the parsing */
	stage = FINAL;
	debug("START OF PROGRAM", 3);
	debug("main()", 1);
	/* set mode to a value of 0 - resolution 320x200 */
	mode = 4;
	/* if there are more than 2 arguments then error */
	if (argc > 2)
	{
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
	getchar();

	/* open up the graphics screen */
	if (screen_open(mode) == OKAY)
	{
		/* set the palette up */
		create_palette();
		/* draw the initial position of all the objects */
		draw_image(masterptr, instanceptr, no_instances);
		/* set the value of c to a null value to begin with */

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_time = tp2 - tp1;
		bool quit = false;
		
		while (!quit)
		{
			SDL_Event event;
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT)
			{
				quit = true;
				continue;
			}

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

			if (event.type == SDL_MOUSEMOTION)
			{
				/* get the current information from the mouse */
				x = event.motion.xrel;
				y = event.motion.yrel;

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
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				/* check to see if the left mouse button was pressed */
				if (event.button.button & SDL_BUTTON_LEFT)
				{
					/* see if we hit an objeot and act accordingly */
					instance = hit_object(mpos_x, mpos_y, user, instanceptr, no_instances);
					/* if this is the second click on the same object then
					   we run it */
					if (instance == prev_inst)
					{
						/* first let's clear the pointer */
						draw_pointer(mpos_x, mpos_y);
						/* now see if we can execute the program */
						if (instance != ERROR)
							program(instance, instanceptr);
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
			}
			else if (event.type == SDL_KEYDOWN)
			{
				/* now we see which key was pressed and act accordingly */
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_UP:
				{
					/* walk forward */
					/* angle 0 is facing forwards so must decrement by 90 */
					angle = user.angy + 90.0;
					if (angle > 360.0) angle -= 360.0;
					locz = locz - (25.0 * sin(angle / RADCONST) * elapsed_time.count());
					locx = locx - (25.0 * cos(angle / RADCONST) * elapsed_time.count());
					if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
					{
						/* if user hasn't collided with an object instance
						   then update the user's position */
						user.locx = locx;
						user.locz = locz;
					}
					break;
				}
				case SDL_SCANCODE_DOWN:
				{
					/* walk backwards */
					/* angle 0 is facing forwards so nust increment by 90 */
					angle = user.angy + 90.0;
					if (angle > 360.0) angle -= 360.0;
					locz = locz + (25.0 * sin(angle / RADCONST) * elapsed_time.count());
					locx = locx + (25.0 * cos(angle / RADCONST) * elapsed_time.count());
					if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
					{
						/* if user hasn't collided with an object instance
						   then update the user's posltion */
						user.locx = locx;
						user.locz = locz;
					}
					break;
				}
				case SDL_SCANCODE_Q:
				{
					/* slide left */
					locz = locz - (125.0 * sin(user.angy / RADCONST) * elapsed_time.count());
					locx = locx - (125.0 * cos(user.angy / RADCONST) * elapsed_time.count());
					if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
					{
						/* if user hasn't collided with an object instance
						   then update the user's position */
						user.locx = locx;
						user.locz = locz;
					}
					break;
				}
				case SDL_SCANCODE_E:
				{
					/* slide right */
					locz = locz + (125.0 * sin(user.angy / RADCONST) * elapsed_time.count());
					locx = locx + (125.0 * cos(user.angy / RADCONST) * elapsed_time.count());
					if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
					{
						/* if user hasn't colllded wlth an object instance
						   then update the user's position */
						user.locx = locx;
						user.locz = locz;
					}
					break;
				}
				case SDL_SCANCODE_LEFT:
				{
					/* turn to the left */
					user.angy -= 2.5 * elapsed_time.count();
					if (user.angy < 0.0) user.angy += 360.0;
					break;
				}
				case SDL_SCANCODE_RIGHT:
				{
					/* turn to the right */
					user.angy += 2.5 * elapsed_time.count();
					if (user.angy > 360.0) user.angy -= 360.0;
					break;
				}
				case SDL_SCANCODE_ESCAPE:
				{
					quit = true;
					break;
				}
				}
			}
			/* draw the new image */
			draw_image(masterptr, instanceptr, no_instances);

			/* check the contents of the palette */
			/* check_palette();                  */

			update_graphics();

			tp2 = std::chrono::system_clock::now();
			elapsed_time = tp2 - tp1;
			tp1 = tp2;
		}
		/* close the graphics screen */
		close_graphics();
	}

	/* free memory taken up by master objects */
	remove_master(no_masters);
	/* free memory taken up by instances */
	remove_instance(no_instances);

	debug("END OF PROGRAM", 3);

	return (OKAY);
}
