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
#include "sound.h"
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
	printf("\nENTERING ANOTHER WORLD...\n");

	/* open up the graphics screen */
	if (init_audio() && screen_open(mode) == OKAY)
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
		
		/* set the initial starting position of the mouse */
		mpos_x = getmaxx() / 2;
		mpos_y = getmaxy() / 2;

		SDL_Event event;

		/* get temporary values of position of user to check collision
		   with any object instances */
		locx = user.locx;
		locy = user.locy;
		locz = user.locz;

		bool key_state[keyboard_state::KEY_MAX] = { false };
		float move;

		while (!quit)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					quit = true;
					continue;
				}

				if (event.type == SDL_MOUSEMOTION)
				{
					/* get the current information from the mouse */
					mpos_x = event.motion.x;
					mpos_y = event.motion.y;

					/* now just check that the x and y values aren't
					   out of range of the screen */
					check_pointer(&mpos_x, &mpos_y);
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
						key_state[keyboard_state::KEY_UP] = true;
						break;
					}
					case SDL_SCANCODE_DOWN:
					{
						key_state[keyboard_state::KEY_DOWN] = true;
						break;
					}
					case SDL_SCANCODE_LEFT:
					{
						key_state[keyboard_state::KEY_LEFT] = true;
						break;
					}
					case SDL_SCANCODE_RIGHT:
					{
						key_state[keyboard_state::KEY_RIGHT] = true;
						break;
					}
					case SDL_SCANCODE_LALT:
					{
						key_state[keyboard_state::KEY_LALT] = true;
						break;
					}
					case SDL_SCANCODE_LSHIFT:
					{
						key_state[keyboard_state::KEY_LSHIFT] = true;
						break;
					}
					case SDL_SCANCODE_ESCAPE:
					{
						key_state[keyboard_state::KEY_QUIT] = true;
						break;
					}
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					/* now we see which key was pressed and act accordingly */
					switch (event.key.keysym.scancode)
					{
					case SDL_SCANCODE_UP:
					{
						key_state[keyboard_state::KEY_UP] = false;
						break;
					}
					case SDL_SCANCODE_DOWN:
					{
						key_state[keyboard_state::KEY_DOWN] = false;
						break;
					}
					case SDL_SCANCODE_LEFT:
					{
						key_state[keyboard_state::KEY_LEFT] = false;
						break;
					}
					case SDL_SCANCODE_RIGHT:
					{
						key_state[keyboard_state::KEY_RIGHT] = false;
						break;
					}
					case SDL_SCANCODE_LALT:
					{
						key_state[keyboard_state::KEY_LALT] = false;
						break;
					}
					case SDL_SCANCODE_LSHIFT:
					{
						key_state[keyboard_state::KEY_LSHIFT] = false;
						break;
					}
					case SDL_SCANCODE_ESCAPE:
					{
						key_state[keyboard_state::KEY_QUIT] = false;
						break;
					}
					}
				}
			}

			if (key_state[keyboard_state::KEY_LSHIFT])
			{
				move = MOVE_SPEED * 3.0f;
			}
			else
			{
				move = MOVE_SPEED;
			}

			if (key_state[keyboard_state::KEY_UP])
			{
				/* walk forward */
				/* angle 0 is facing forwards so must decrement by 90 */
				angle = user.angy + 90.0;
				if (angle > 360.0) angle -= 360.0;
				locz = locz - (move * sin(angle / RADCONST) * elapsed_time.count());
				locx = locx - (move * cos(angle / RADCONST) * elapsed_time.count());
				if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
				{
					/* if user hasn't collided with an object instance
					   then update the user's position */
					user.locx = locx;
					user.locz = locz;
				}
			}

			if (key_state[keyboard_state::KEY_DOWN])
			{
				/* walk backwards */
				/* angle 0 is facing forwards so nust increment by 90 */
				angle = user.angy + 90.0;
				if (angle > 360.0) angle -= 360.0;
				locz = locz + (move * sin(angle / RADCONST) * elapsed_time.count());
				locx = locx + (move * cos(angle / RADCONST) * elapsed_time.count());
				if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
				{
					/* if user hasn't collided with an object instance
					   then update the user's posltion */
					user.locx = locx;
					user.locz = locz;
				}
			}

			if (key_state[keyboard_state::KEY_LEFT] && key_state[keyboard_state::KEY_LALT])
			{
				/* slide left */
				locz = locz - (move * sin(user.angy / RADCONST) * elapsed_time.count());
				locx = locx - (move * cos(user.angy / RADCONST) * elapsed_time.count());
				if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
				{
					/* if user hasn't collided with an object instance
					   then update the user's position */
					user.locx = locx;
					user.locz = locz;
				}
			}
			else if (key_state[keyboard_state::KEY_LEFT])
			{
				/* turn to the left */
				user.angy -= move * elapsed_time.count();
				if (user.angy < 0.0) user.angy += 360.0;
			}

			if (key_state[keyboard_state::KEY_RIGHT] && key_state[keyboard_state::KEY_LALT])
			{
				/* slide right */
				locz = locz + (move * sin(user.angy / RADCONST) * elapsed_time.count());
				locx = locx + (move * cos(user.angy / RADCONST) * elapsed_time.count());
				if (check_col(locx, locy, locz, instanceptr, no_instances, user) == OKAY)
				{
					/* if user hasn't colllded wlth an object instance
					   then update the user's position */
					user.locx = locx;
					user.locz = locz;
				}
			}
			else if (key_state[keyboard_state::KEY_RIGHT])
			{
				/* turn to the right */
				user.angy += move * elapsed_time.count();
				if (user.angy > 360.0) user.angy -= 360.0;
			}

			if (key_state[keyboard_state::KEY_QUIT])
			{
				quit = true;
			}
			
			/* draw the new image */
			draw_image(masterptr, instanceptr, no_instances);

			/* now draw the pointer */
			draw_pointer(mpos_x, mpos_y);

			/* check the contents of the palette */
			/* check_palette();                  */

			update_graphics();

			tp2 = std::chrono::system_clock::now();
			elapsed_time = tp2 - tp1;
			tp1 = tp2;

			update_title(1.0f / elapsed_time.count());
		}
		close_graphics();
		term_audio();
	}

	/* free memory taken up by master objects */
	remove_master(no_masters);
	/* free memory taken up by instances */
	remove_instance(no_instances);

	debug("END OF PROGRAM", 3);

	return (OKAY);
}
