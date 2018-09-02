/******************************************
*                                         *
* ##### ####  ####   ###   ####      #### *
* ##    ## ## ## ## ## ##  ## ##    ##    *
* ####  ####  ####  ## ##  ####     ##    *
* ##    ## ## ## ## ## ##  ## ##    ##    *
* ##### ## ## ## ##  ###   ## ## ##  #### *
*                                         *
******************************************/
/****************
* include files *
****************/
#include <iostream>
#include <stdio.h>
#include "defs.hpp"
#include "error.hpp"

/* global debug mode */
bool g_debug_mode = false;

/* Set debug mode */
void set_debug_mode(bool debug_mode)
{
	g_debug_mode = debug_mode;
}

/*****************************************************************************
* error() - all error messages come through here                             *
*           simply pass in a string containing the error number, a string    *
*           containing the error message and a relevant line number          *
*           keeps the same format for all error messages                     *
*****************************************************************************/
Status error(const std::string &message, int line_no)
{
	debug("error()", 1);

	std::cerr << "ERROR: " << message.c_str() << "   Line number: " << line_no << std::endl;

	return Error;
}

/*****************************************************************************
* warn() - in principle the same as error() only it displays just warnings   *
*          and does not return anything                                      *
*****************************************************************************/
void warn(const std::string &message, int line_no)
{
	debug("warn()", 1);

	std::cerr << "WARN: " << message.c_str() << "   Line number: " << line_no << std::endl;
}

/*****************************************************************************
* debug() - function to be used to debug programs - global variable 'stage'  *
*           should be set to TEST for it to work                             *
*****************************************************************************/
void debug(const std::string &string, int level)
{
	if (g_debug_mode)
	{
		/* print the debug message at a particular level */
		switch (level)
		{
		case 1:
		{
			std::cout << "Function: \"" << string.c_str() << "\"" << std::endl;
			break;
		}
		default:
		{
			for (auto i=0; i<level; i++)
			{
				std::cout << "\t";
			}
			std::cout << "\"" << string.c_str() << "\"" << std::endl;
		}
		}
	}
	else
	{
		/* we do nothing here because this is final stage of the program */
	}
}

