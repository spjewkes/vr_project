#include <iostream>
#include "options.hpp"

void Options::print_help()
{
	std::cout << "Run: " << m_argv[0] << " [--help] [--debug] [--dump] [--file <filename>] [filename]\n";
	std::cout << "\t--help            - displays this help\n";
	std::cout << "\t--file <filename> - run world with specified file\n";
	std::cout << "\tfilename          - run world with specified file\n";
	std::cout << "\t--debug           - switched on debug output\n";
	std::cout << "\t--dump            - dump world data\n";
}

void Options::process()
{
	std::string positional_file;
	bool parse_options = true;

	for (int index = 1; index < m_argc; index++)
	{
		if (m_argv[index] == nullptr)
		{
			continue;
		}

		std::string argument = m_argv[index];

		if (parse_options && argument == "--")
		{
			parse_options = false;
			continue;
		}

		if (!parse_options || argument.size() < 2 || argument[0] != '-' || argument == "-")
		{
			if (positional_file.empty())
			{
				positional_file = argument;
			}
			continue;
		}

		if (argument.compare(0, 2, "--") == 0)
		{
			if (argument == "--help")
			{
				help_mode = true;
				print_help();
			}
			else if (argument == "--debug")
			{
				debug_mode = true;
			}
			else if (argument == "--dump")
			{
				dump_mode = true;
			}
			else if (argument == "--file")
			{
				if ((index + 1 < m_argc) && (m_argv[index + 1] != nullptr))
				{
					file = m_argv[++index];
				}
			}
			else if (argument.compare(0, 7, "--file=") == 0)
			{
				file = argument.substr(7);
			}
			continue;
		}

		for (std::string::size_type offset = 1; offset < argument.size(); offset++)
		{
			switch (argument[offset])
			{
			case 'h':
				help_mode = true;
				print_help();
				break;
			case 'b':
				debug_mode = true;
				break;
			case 'd':
				dump_mode = true;
				break;
			case 'f':
				if (offset + 1 < argument.size())
				{
					file = argument.substr(offset + 1);
				}
				else if ((index + 1 < m_argc) && (m_argv[index + 1] != nullptr))
				{
					file = m_argv[++index];
				}
				offset = argument.size();
				break;
			default:
				break;
			}
		}
	}

	if (file.empty())
	{
		file = positional_file;
	}
}
