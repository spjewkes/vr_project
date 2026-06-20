#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include "defs.hpp"

/**
 * @brief Parsed command-line configuration for the application.
 *
 * Construction processes the argument vector immediately and prints usage when
 * help is requested. An explicit `--file` value takes precedence over a
 * positional world path.
 */
class Options
{
  public:
	Options(int argc, char **argv) : m_argc(argc), m_argv(argv)
	{
		process();
	}
	~Options() {}

	std::string get_file() const
	{
		return file;
	}
	bool get_debug_mode() const
	{
		return debug_mode;
	}
	bool get_dump_mode() const
	{
		return dump_mode;
	}
	bool get_help_mode() const
	{
		return help_mode;
	}

  private:
	Options() = delete;

	void print_help();
	void process();

	int m_argc;
	char **m_argv;

	std::string file = "";
	bool debug_mode = false;
	bool dump_mode = false;
	bool help_mode = false;
};

#endif // __OPTIONS_HPP__
