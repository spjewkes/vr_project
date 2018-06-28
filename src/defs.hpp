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

/************************
* Define user structure *
************************/
struct viewer
{
	Vector3d ang;
	Vector3d loc;

	float radius = { 1.0f };

	int sky = { 0 };
	int ground = { 0 };
};

/****************************
* Define instance structure *
****************************/
struct instance
{
	int master_no = { 0 };

	Vector3d pos = { 0.0f, 0.0f, 0.0f };
	Vector3d min = { 0.0f, 0.0f, 0.0f };
	Vector3d max = { 0.0f, 0.0f, 0.0f };
	Vector3d angle = { 0.0f, 0.0f, 0.0f };
	Vector3d scale = { 1.0f, 1.0f, 1.0f };

	std::vector<Vector3d> vert;

	std::vector<unsigned int> edge_colour;
	std::vector<unsigned int> poly_colour;

	RenderStyle style = { RenderStyle::WIREFRAME };
	bool is_solid = { false };

	std::string outcome = { "" };
};

/**************************
* Define master structure *
**************************/
struct master
{
	Vector3d scale = { 1.0f, 1.0f, 1.0f };
	Vector3d angle = { 0.0f, 0.0f, 0.0f };

	std::vector<Vector3d> vert;

	std::vector<unsigned int> edge0;
	std::vector<unsigned int> edge1;

	std::vector<unsigned int> poly0;
	std::vector<unsigned int> poly1;
	std::vector<unsigned int> poly2;
};

#endif // __DEFS_HPP__
