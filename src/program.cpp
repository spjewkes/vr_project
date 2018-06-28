/********************************************************
*                                                       *
* ####  ####   ###   ##### ####   ###  ##   ##     #### *
* ## ## ## ## ## ## ##     ## ## ## ## ### ###    ##    *
* ####  ####  ## ## ## ### ####  ##### #######    ##    *
* ##    ## ## ## ## ##  ## ## ## ## ## ## # ##    ##    *
* ##    ## ##  ###   ####  ## ## ## ## ##   ## ##  #### *
*                                                       *
********************************************************/
/****************
* include files *
****************/
#include <vector>
#include <string>
#include <iostream>
#include "defs.hpp"

/****************************************************************************
* program() - this function looks at the string for a particular instance   *
*             and checks whether it is possible to execute it using the     *
*             spawnvp() command                                             *
****************************************************************************/
void program(instance &inst)
{
	if (inst.outcome != "solid")
	{
		std::string delims = " \t";
		std::vector<std::string> args;

		size_t start = inst.outcome.find_first_not_of(delims);
		size_t end = 0;

		while ((end = inst.outcome.find_first_of(delims, start)) != std::string::npos)
		{
			args.push_back(inst.outcome.substr(start, end - start));
			start = inst.outcome.find_first_not_of(delims, end);
		}

		if (start != std::string::npos)
		{
			args.push_back(inst.outcome.substr(start));
		}

		std::cout << "Running:\n";
		for (auto &arg : args)
		{
			std::cout << "\t" << arg.c_str() << std::endl;
		}
	}
}
