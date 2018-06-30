#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include "defs.hpp"
#include "instance.hpp"
#include "master.hpp"

void set_debug_mode(bool debug_mode);

Status error(const std::string &message, int line_no);
void warn(const std::string &message, int line_no);
void debug(const std::string &string, int level);

void check_master(std::vector<Master> &masters);
void check_instance(std::vector<Instance> &instances);

#endif // __ERROR_HPP__
