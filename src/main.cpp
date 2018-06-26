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
#include "collide.hpp"
#include "defs.hpp"
#include "error.hpp"
#include "graphics.hpp"
#include "mouse.hpp"
#include "parse.hpp"
#include "program.hpp"
#include "screen.hpp"
#include "setup.hpp"
#include "sound.hpp"

/************
* functions *
************/
int main(int argc, char *argv[])
{
	float locx, locy, locz;
	float angle;
	int mode, instance;
	int prev_inst = -1;

	/* variables for the mouse */
	int mpos_x, mpos_y;
	/* set phase of implementation */
	/* TEST: prints debug messages */
	/* FINAL: doesn't show any of the parsing */
	set_debug_mode(FINAL);
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

	Parser script;

	/* if parsing script results in an error then leave program */
	if (script.parse(argv[1]) == Error)
	{
		exit(0);
	}

	/* display the contents of the master structure */
	// check_master(script.masters()); 
	/* display the contents of the instance structure */
	// check_instance(script.instances());
	/* now let's start the really interesting bit */
	printf("\nENTERING ANOTHER WORLD...\n");

	/* open up the graphics screen */
	if ((init_audio() == Okay) && (screen_open(mode) == Okay))
	{
		/* draw the initial position of all the objects */
		draw_image(script.masters(), script.instances(), script.get_user());
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
		locx = script.get_user().loc.x();
		locy = script.get_user().loc.y();
		locz = script.get_user().loc.z();

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
						instance = hit_object(mpos_x, mpos_y, script.get_user(), script.instances());
						/* if this is the second click on the same object then
						   we run it */
						if (instance == prev_inst)
						{
							/* first let's clear the pointer */
							draw_pointer(mpos_x, mpos_y);
							/* now see if we can execute the program */
							if (instance != -1)
							{
								program(script.instances()[instance]);
							}
							/* draw the new image */
							draw_image(script.masters(), script.instances(), script.get_user());
							/* finally redraw the pointer */
							draw_pointer(mpos_x, mpos_y);
							/* so that another double click is needed */
							prev_inst = -1;
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
					default:
					{
						/* Do nothing */
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
					default:
					{
						/* Do nothing */
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
				angle = script.get_user().ang.y() + 90.0;
				if (angle > 360.0) angle -= 360.0;
				locz = locz - (move * sin(angle / RADCONST) * elapsed_time.count());
				locx = locx - (move * cos(angle / RADCONST) * elapsed_time.count());
				if (!check_col(locx, locy, locz, script.get_user(), script.instances()))
				{
					/* if user hasn't collided with an object instance
					   then update the user's position */
					script.get_user().loc.x(locx);
					script.get_user().loc.z(locz);
				}
			}

			if (key_state[keyboard_state::KEY_DOWN])
			{
				/* walk backwards */
				/* angle 0 is facing forwards so nust increment by 90 */
				angle = script.get_user().ang.y() + 90.0;
				if (angle > 360.0) angle -= 360.0;
				locz = locz + (move * sin(angle / RADCONST) * elapsed_time.count());
				locx = locx + (move * cos(angle / RADCONST) * elapsed_time.count());
				if (!check_col(locx, locy, locz, script.get_user(), script.instances()))
				{
					/* if user hasn't collided with an object instance
					   then update the user's posltion */
					script.get_user().loc.x(locx);
					script.get_user().loc.z(locz);
				}
			}

			if (key_state[keyboard_state::KEY_LEFT] && key_state[keyboard_state::KEY_LALT])
			{
				/* slide left */
				locz = locz - (move * sin(script.get_user().ang.y() / RADCONST) * elapsed_time.count());
				locx = locx - (move * cos(script.get_user().ang.y() / RADCONST) * elapsed_time.count());
				if (!check_col(locx, locy, locz, script.get_user(), script.instances()))
				{
					/* if user hasn't collided with an object instance
					   then update the user's position */
					script.get_user().loc.x(locx);
					script.get_user().loc.z(locz);
				}
			}
			else if (key_state[keyboard_state::KEY_LEFT])
			{
				/* turn to the left */
				script.get_user().ang.y(script.get_user().ang.y() - (move * elapsed_time.count()));
				if (script.get_user().ang.y() < 0.0)
					script.get_user().ang.y(script.get_user().ang.y() + 360.0);
			}

			if (key_state[keyboard_state::KEY_RIGHT] && key_state[keyboard_state::KEY_LALT])
			{
				/* slide right */
				locz = locz + (move * sin(script.get_user().ang.y() / RADCONST) * elapsed_time.count());
				locx = locx + (move * cos(script.get_user().ang.y() / RADCONST) * elapsed_time.count());
				if (!check_col(locx, locy, locz, script.get_user(), script.instances()))
				{
					/* if user hasn't colllded wlth an object instance
					   then update the user's position */
					script.get_user().loc.x(locx);
					script.get_user().loc.z(locz);
				}
			}
			else if (key_state[keyboard_state::KEY_RIGHT])
			{
				/* turn to the right */
				script.get_user().ang.y(script.get_user().ang.y() + (move * elapsed_time.count()));
				if (script.get_user().ang.y() > 360.0)
					script.get_user().ang.y(script.get_user().ang.y() - 360.0);
			}

			if (key_state[keyboard_state::KEY_QUIT])
			{
				quit = true;
			}
			
			/* draw the new image */
			draw_image(script.masters(), script.instances(), script.get_user());

			/* now draw the pointer */
			draw_pointer(mpos_x, mpos_y);

			update_graphics();

			tp2 = std::chrono::system_clock::now();
			elapsed_time = tp2 - tp1;
			tp1 = tp2;

			update_title(1.0f / elapsed_time.count());
		}
		close_graphics();
		term_audio();
	}

	debug("END OF PROGRAM", 3);

	return 0;
}
