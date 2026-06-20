#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include "defs.hpp"
#include "instance.hpp"
#include "master.hpp"

/** @brief Enable or disable process-wide debug output. */
void set_debug_mode(bool debug_mode);

/** @brief Print a formatted error and return Error for direct propagation. */
Status error(const std::string &message, int line_no);

/** @brief Print a formatted warning with its source line number. */
void warn(const std::string &message, int line_no);

/** @brief Print a debug message at the requested indentation level when enabled. */
void debug(const std::string &string, int level);

void check_master(std::vector<Master> &masters);
void check_instance(std::vector<Instance> &instances);

#endif // __ERROR_HPP__
