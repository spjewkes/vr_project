#include <iostream>
#include <getopt.h>
#include "options.hpp"

void Options::print_help()
{
	std::cout << "Run: " << m_argv[0] << "[--help] [--debug] [--dump] [--file <filename>]\n";
	std::cout << "\t--help            - displays this help\n";
	std::cout << "\t--file <filename> - run world with specified file\n";
	std::cout << "\t--debug           - switched on debug output\n";
	std::cout << "\t--dump            - dump world data\n";
}

void Options::process()
{
	static struct option long_options[] =
        {
			{ "help",     no_argument,       0, 'h' },
			{ "debug",    no_argument,       0, 'b' },
			{ "dump",     no_argument,       0, 'd' },
			{ "file",     required_argument, 0, 'f' },
			{ 0, 0, 0, 0 }
		};

	int c;

	while (1)
	{
		int option_index = 0;

		c = getopt_long(m_argc, m_argv, "bdf:", long_options, &option_index);

		if (c == -1)
		{
			break;
		}

		switch (c)
		{
		case 'h':
			print_help();
			break;
		case 'b':
			debug_mode = true;
			break;

		case 'd':
			dump_mode = true;
			break;
			
		case 'f':
			file = optarg;
			break;
		}
	}
}
