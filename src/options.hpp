#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include "defs.hpp"

class Options
{
public:
	Options(int argc, char **argv) : m_argc(argc), m_argv(argv) { process(); }
	~Options() {}

	std::string get_file() { return file; }
	bool get_debug_mode() { return debug_mode; }

private:
	Options() =delete;

	void print_help();
	void process();

	int m_argc;
	char **m_argv;

	std::string file = "";
	bool debug_mode = false;
};

#endif // __OPTIONS_HPP__
