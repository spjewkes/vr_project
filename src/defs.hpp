#ifndef __DEFS_HPP__
#define __DEFS_HPP__

#include <vector>
#include <string>
#include "vector3d.hpp"

#define RADCONST (57.295779513082321)

#define MAXLINE (4096)

#define MSIZE (8)

#define MOVE_SPEED (25.0f)

//
// Define type for returning a status from functions
//
typedef enum status { Error, Okay } Status;

//
// Define type for checking string matches
//
typedef enum match_result { Match, NoMatch, Blank, Comment, Other, EoF } MatchResult;

//
// Define object's render style
//
enum RenderStyle
{
	SOLID,
	WIREFRAME
};

/*************************
* Define Keyboard events *
*************************/
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
