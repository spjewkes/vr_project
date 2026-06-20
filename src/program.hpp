#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include "defs.hpp"
#include "instance.hpp"

/**
 * @brief Handle an instance's legacy outcome string.
 *
 * Non-`solid` strings are tokenized and printed. External process execution
 * from the DOS version is intentionally not performed.
 * @param inst Instance whose outcome is handled.
 */
void program(Instance &inst);

#endif // __PROGRAM_HPP__
