#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include <vector>
#include <string>
#include "vector3d.hpp"

#define RADCONST (57.295779513082321)

#define MAXLINE (4096)

#define MSIZE (8)

#define MOVE_SPEED (25.0f)

/** Result used by legacy-style operations that report success or failure. */
typedef enum status
{
	Error,
	Okay
} Status;

/** Classification returned by the line-oriented parser helpers. */
typedef enum match_result
{
	Match,
	NoMatch,
	Blank,
	Comment,
	Other,
	EoF
} MatchResult;

/** Rendering path selected for an Instance. */
enum RenderStyle
{
	SOLID,
	WIREFRAME
};

/** Indices used by the main loop's pressed-key state array. */
enum keyboard_state
{
	KEY_UP = 0,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_LSHIFT,
	KEY_LALT,
	KEY_QUIT,

	KEY_MAX
};

#endif // __DEFS_HPP__
