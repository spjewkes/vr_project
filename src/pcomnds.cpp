/********************************************************
*                                                       *
* ####   ####  ###  ##   ## ##  ## ####   ####     #### *
* ## ## ##    ## ## ### ### ### ## ## ## ##       ##    *
* ####  ##    ## ## ####### ###### ## ## #####    ##    *
* ##    ##    ## ## ## # ## ## ### ## ##    ##    ##    *
* ##     ####  ###  ##   ## ##  ## ####  ####  ##  #### *
*                                                       *
********************************************************/
/****************
* include files *
****************/
#include <string>
#include <stdlib.h>
#include "defs.hpp"
#include "error.hpp"
#include "pfuncs.hpp"

/*************************
* prototype declarations *
*************************/
Status process_radius(float *radius);
Status process_colour(int *colour);
Status process_specularity(float *specularity);
Status process_sky(int *colour);
Status process_ground(int *colour);

/****************************************************************************
* process_location() - process the command which defines the user obJect's  *
*                      initial location                                     *
****************************************************************************/
Status process_location(Vector3d &loc)
{
	Status result = Okay;

	debug("process_location()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	loc.x(fgetnum());

	if (check(",") != Match)
		result = error("0017", "Syntax error with location definition", lincnt);

	loc.y(fgetnum());

	if (check(",") != Match)
		result = error("0017", "Syntax error with location definition", lincnt);
        
	loc.z(fgetnum());

	if (check("") != Blank)
		result = error("0017", "Syntax error with location definition", lincnt);

	return result;
}

/****************************************************************************
* process_direction() - process the command which defines the user object's *                                                                                                    **
*                       initial direction                                   *
****************************************************************************/
Status process_direction(Vector3d &ang)
{
	Status result = Okay;

	debug("process_direction", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);
        
	ang.x(fgetnum());

	if (check(",") != Match)
		result = error("0018", "Syntax error with user direction definition", lincnt);
        
	ang.y(fgetnum());

	if (check(",") != Match)
		result = error("0018", "Syntax error with user direction definition", lincnt);
        
	ang.z(fgetnum());

	if (check("") != Blank)
		result = error("0018", "Syntax error with user direction definition", lincnt);

	return result;
}

/****************************************************************************
* process_radius - process the command which defines the user object's      *
*                  radius                                                   *
****************************************************************************/
Status process_radius(float *radius)
{
	Status result = Okay;

	debug("process_radius()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);
        
	*radius = fgetnum();

	if (check("") != Blank)
		result = error("0019", "Syntax error with user radius definition", lincnt);

	return result;
}

/****************************************************************************
* process_angle() - process the command which defines the master object's   *
*                   initial angle                                           *
****************************************************************************/
Status process_angle(Vector3d &ang)
{
	Status result = Okay;

	debug("process_angle()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	ang.x(fgetnum());

	if (check(",") != Match)
		result = error("0023", "Syntax error with angle command", lincnt);

	ang.y(fgetnum());

	if (check(", ") != Match)
		result = error("0023", "Syntax error with angle command", lincnt);

	ang.z(fgetnum());

	if (check("") != Blank)
		result = error("0023", "Syntax error with angle command", lincnt);

	return result;
}

/****************************************************************************
* process_scale() - process the command which defines the master object's   *
*                   initial scale                                           *
****************************************************************************/
Status process_scale(Vector3d &scl)
{
	Status result = Okay;

	debug("process_scale()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	scl.x(fgetnum());

	if (check(",") != Match)
		result = error("0024", "Syntax error with scale command", lincnt);

	scl.y(fgetnum());

	if (check(",") != Match)
		result = error("0024", "Syntax error with scale command", lincnt);

	scl.z(fgetnum());

	if (check("") != Blank)
		result = error("0024", "Syntax error with scale command", lincnt);

	return result;
}

/****************************************************************************
* process sky - process the command which defines the colour of the sky     *
****************************************************************************/
Status process_sky(int *colour)
{
	Status result = Okay;

	debug("process_sky()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((*colour = getnum()) == Error)
		result = error("0054", "Syntax error with sky colour definition", lincnt);

	if (check("") != Blank)
		result = error("0054", "Syntax error with sky colour definition", lincnt);

	return result;
}

/****************************************************************************
* process_ground - process the command which defines the colour of the      *
*                  ground                                                   *
****************************************************************************/
Status process_ground(int *colour)
{
	Status result = Okay;

	debug("process_ground()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((*colour = getnum()) == Error)
		result = error("0055", "Syntax error with ground colour definition", lincnt);

	if (check("") != Blank)
		result = error("0055", "Syntax error with ground colour definition", lincnt);

	return result;
}

/****************************************************************************
* process_colour() - process the command which defines the instance         *
*                    object' s colour                                       *
****************************************************************************/
Status process_colour(int *colour)
{
	Status result = Okay;
	char word[MAXLINE];

	debug("process_colour()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	getword(word);

	/* get the colour value from the colour command */
	if ((strcmp(word, "BLACK") == 0) || (strcmp(word, "0") == 0))
		*colour = 0;
	else if ((strcmp(word, "BLUE") == 0) || (strcmp(word, "1") == 0))
		*colour = 1;
	else if ((strcmp(word, "GREEN") == 0) || (strcmp(word, "2") == 0))
		*colour = 2;
	else if ((strcmp(word, "CYAN") == 0) || (strcmp(word, "3") == 0))
		*colour = 3;
	else if ((strcmp(word, "RED") == 0) || (strcmp(word, "4") == 0))
		*colour = 4;
	else if ((strcmp(word, "MAGENTA") == 0) || (strcmp(word, "5") == 0))
		*colour = 5;
	else if ((strcmp(word, "BROWN") == 0) || (strcmp(word, "6") == 0))
		*colour = 6;
	else if ((strcmp(word, "LIGHTGREY") == 0) || (strcmp(word, "7") == 0))
		*colour = 7;
	else if ((strcmp(word, "DARKGREY") == 0) || (strcmp(word, "8") == 0))
		*colour = 8;
	else if ((strcmp(word, "LIGHTBLUE") == 0) || (strcmp(word, "9") == 0))
		*colour = 9;
	else if ((strcmp(word, "LIGHTGREEN") == 0) || (strcmp(word, "10") == 0))
		*colour = 10;
	else if ((strcmp(word, "LIGHTCYAN") == 0) || (strcmp(word, "11") == 0))
		*colour = 11;
	else if ((strcmp(word, "LIGHTRED") == 0) || (strcmp(word, "12") == 0))
		*colour = 12;
	else if ((strcmp(word, "LIGHTMAGENTA") == 0) || (strcmp(word, "13") == 0))
		*colour = 13;
	else if ((strcmp(word, "YELLOW") == 0) || (strcmp(word, "14") == 0))
		*colour = 14;
	else if ((strcmp(word, "WHITE") == 0) || (strcmp(word, "15") == 0))
		*colour = 15;
	else
		result = error("0044", "Invalid colour value", lincnt);

	if (check("") != Blank)
		result = error("0033", "Syntax error with colour command", lincnt);

	if ((*colour < 0) || (*colour > 15))
		result = error("0044", "Invalid colour value", lincnt);

	return result;
}

/****************************************************************************
* process specularity() - process the command which defines the instance    *
*                         object' s specularity                             *
****************************************************************************/
Status process_specularity(float *specularity)
{
	Status result = Okay;
	MatchResult rest_of_line;

	debug("process_specularity()", 1);

	/* make sure that there is an equals sign */
	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	*specularity = fgetnum();

	/* check to see if there is a percent sign at the end of the line */
	rest_of_line = check("%");

	if ((rest_of_line != Blank) && (rest_of_line != Match))
		result = error("0034", "Syntax error with specularity command", lincnt);

	/* make sure the specularity value is a percentage */
	if ((*specularity < 0.0) || (*specularity > 100.0))
		result = error("0035", "Invalid specularity number", lincnt);

	return result;
}

/****************************************************************************
* process_outcome() - process the command which defines the instance        *
*                     object's outcome string                               *
*                     this can either make an object solid or not           *
*                     there is also the possibility to execute DOS commands *
*                     with this string                                      *
****************************************************************************/
bool process_outcome(std::string &outcome)
{
	char word[MAXLINE];

	debug("process_outcome()", 1);

	/* make sure that there is an equals sign */
	if (check("=") != Match)
	{
		error("0006", "Missing assignment symbol", lincnt);
		return false;
	}

	/* if the string is in error then return error */
	if (getstring(word) == Error)
	{
		error("0053", "Syntax error with outcome string", lincnt);
		return false;
	}

	/* copy array over to outcome string */
	outcome = word;

	return true;
}

/****************************************************************************
* process_style() - process the command which defines the instance object's *
*                   style                                                   *
****************************************************************************/
Status process_style(RenderStyle &style)
{
	Status result = Okay;
	char word[MAXLINE];

	debug("process_style()", 1);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	getword(word);

	if (strcmp(word, "SOLID") == 0)
		style = RenderStyle::SOLID;
	else if (strcmp(word, "WIREFRAME") == 0)
		style = RenderStyle::WIREFRAME;
	else
		result = error("0036", "Unknown style type", lincnt);

	return result;
}

/****************************************************************************
* process_vertices() - process the object's vertices                        *
****************************************************************************/
Status process_verts(master &mast, int no_vertices)
{
	Status result = Okay;
	int loop, vert_no;

	/* first we create the vertice data structures */
	mast.vert.resize(no_vertices);

	for (loop = 0; loop < no_vertices; loop++)
	{
		/* first skip past any garbage! */
		skip_garbage();

		/* get the number of the vertex to set */
		if ((vert_no = getnum()) == Error)
			result = error("0047", "Syntax error with vertex command", lincnt);

		if ((vert_no < 1) || (vert_no > no_vertices))
			result = error("0047", "Syntax error with vertex command", lincnt);

		if (check("=") != Match)
			result = error("0006", "Missing assignment symbol", lincnt);

		/* now we get the x, y and z values of the vertex */
		mast.vert[vert_no-1].x(fgetnum());

		if (check(",") != Match)
			result = error("0047", "Syntax error with vertex command", lincnt);

		mast.vert[vert_no-1].y(fgetnum());

		if (check(",") != Match)
			result = error("0047", "Syntax error with vertex command", lincnt);

		mast.vert[vert_no-1].z(fgetnum());

		/* make sure there is no more text on the end of the line */
		if (check("") != Blank)
			result = error("0047", "Syntax error with vertex command", lincnt);
	}

	return result;
}

/****************************************************************************
* process_edges() - process the object's edges                              *
****************************************************************************/
Status process_edges(master &mast, int no_edges)
{
	Status result = Okay;
	int loop, edge_no;

	/* first we create the edge data structures */
	mast.edge0.resize(no_edges);
	mast.edge1.resize(no_edges);

	for (loop = 0; loop < no_edges; loop++)
	{
		/* first skip past any garbage! */
		skip_garbage();

		/* get the number of the vertex to set */
		if ((edge_no = getnum()) == Error)
			result = error("0049", "Syntax error with edge command", lincnt);

		if ((edge_no < 1) || (edge_no > no_edges))
			result = error("0049", "Syntax error with edge command", lincnt);

		if (check("=") != Match)
			result = error("0006", "Missing assignment symbol", lincnt);

		/* now we get the start and end vertices of the edge */
		size_t edge_val = getnum();

		/* now make sure that it is a valid vertex reference */
		if ((edge_val < 1) || (edge_val > mast.vert.size()))
			result = error("0050", "Illegal edge value", lincnt);

		/* remember to take one from values to match array structure */
		mast.edge0[edge_no-1] = edge_val - 1;

		if (check(",") != Match)
			result = error("0049", "Syntax error with edge command", lincnt);

		edge_val = getnum();

		/* now make sure that it is a valid vertex reference */
		if ((edge_val < 1) || (edge_val > mast.vert.size()))
			result = error("0050", "Illegal edge value", lincnt);

		/* remember to take one from values to match array structure */
		mast.edge1[edge_no-1] = edge_val - 1;

		/* make sure there is no more text on the end of the line */
		if (check("") != Blank)
			result = error("0049", "Syntax error with edge command", lincnt);
	}

	return result;
}

/****************************************************************************
* process_polygons() - process the object's polygons                        *
****************************************************************************/
Status process_polys(master &mast, int no_polygons)
{
	Status result = Okay;
	int loop, poly_no;
	unsigned int tmp;

	/* first we create the polygon data structures */
	mast.poly0.resize(no_polygons);
	mast.poly1.resize(no_polygons);
	mast.poly2.resize(no_polygons);

	for (loop = 0; loop < no_polygons; loop++)
	{
		/* first skip past any garbage! */
		skip_garbage();

		/* get the number of the polygon to set */
		if ((poly_no = getnum()) == Error)
			result = error("0051", "Syntax error with polygon command", lincnt);

		if ((poly_no < 1) || (poly_no > no_polygons))
			result = error("0051", "Syntax error with polygon command", lincnt);

		if (check("=") != Match)
			result = error("0006", "MAssing assignment symbol", lincnt);

		/* now we get the three edges that make up the polygon */
		tmp = getnum();

		/* now make sure that it is a valid edge reference */
		if ((tmp < 1) || (tmp > mast.edge0.size()))
			result = error("0052", "Illegal polygon value", lincnt);

		/* remember to take one from values to match array structure */
		mast.poly0[poly_no-1] = tmp - 1;

		if (check(",") != Match)
			result = error("0049", "Syntax error with edge command", lincnt);

		/* next edge reference */
		tmp = getnum();

		/* now make sure that it is a valid edge reference */
		if ((tmp < 1) || (tmp > mast.edge0.size()))
			result = error("0052", "Illegal polygon value", lincnt);

		/* remember to take one from values to match array structure */
		mast.poly1[poly_no-1] = tmp - 1;
		
		if (check(",") != Match)
			result = error("0049", "Syntax error with edge command", lincnt);

		/* final edge reference */
		tmp = getnum();

		/* now make sure that it is a valid edge reference */
		if ((tmp < 1) || (tmp > mast.edge0.size()))
			result = error("0052", "Illegal polygon value", lincnt);

		/* remember to take one from values to match array structure */
		mast.poly2[poly_no-1] = tmp - 1;

		/* make sure there is no more text on the end of the line */
		if (check("") != Blank)
			result = error("0049", "Syntax error with edge command", lincnt);
	}

	return result;
}
