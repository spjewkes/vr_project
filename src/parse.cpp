/*****************************************
*                                        *
* ####   ###  ####   #### #####     #### *
* ## ## ## ## ## ## ##    ##       ##    *
* ####  ##### ####  ##### ####     ##    *
* ##    ## ## ## ##    ## ##       ##    *
* ##    ## ## ## ## ####  ##### ##  #### *
*                                        *
*****************************************/
/****************
* include files *
****************/
#include "defs.hpp"
#include "error.hpp"
#include "parse.hpp"
#include "vector3d.hpp"

/* Constructor */
Parser::Parser(const std::string &_filename, World &_world) : filename(_filename), world(_world)
{
}

/* Destructor */
Parser::~Parser()
{
}

/****************************************************************************
* parse() - main function to initiate the parsing of a text file for the    *
*           virtual reality program.                                        *
*           Returns a pointer to a datastructure that contains the virtual  *
*           world to be manipulated.                                        *
****************************************************************************/
Status Parser::parse()
{
	MatchResult retcode;
	debug("parse()", 1);

	/* lincnt stores currently processed line */
	lincnt = 0;

	file.open(filename, std::ios::in);

	if (!file.is_open())
	{
		/* error if can't open file */
		error("Error opening file", lincnt);
		return Error;
	}
	else
	{
		/* get the first line of the file */
		retcode = getline();

		/* getting lines from file until EOF */
		while (retcode != EoF)
		{
			if ((retcode != Comment) && (retcode != Blank))
				if (process() == Error)
				{
					/* if error then stop parsing file */
					error("Cannot parse file", lincnt);
					return Error;
				}
			/* get next line */
			retcode = getline();
		}
	}

	file.close();

	return Okay;
}

/****************************************************************************
* process() - function to process any commands that may occur in the file   *
*             at this level it searches for one of three headers:           *
*             ".masterdefs", ".instance defs" or ".userdefs"                *
*             any other strings will-result in a syntax error               *
****************************************************************************/
Status Parser::process(void)
{
	std::string word;

	debug("process()", 1);

	/* get the first word in LINE */
	getword(word);

	if (word.size() == 0)
		return Okay;
	else if (word == ".masterdefs")
		/* a master block */
		return process_master();
	else if (word == ".instancedefs")
		/* an instance block */
		return process_instance();
	else if (word == ".userdefs")
		/* a user block */
		return process_user();
	else
	{
		error("Syntax error", lincnt);
		/* unknown block! */
		return Error;
	}
}

/****************************************************************************
* process_master() - process a text block which contains definitions for    *
*                    the master objects                                     *
****************************************************************************/
Status Parser::process_master()
{
	Status result = Okay;
	int no_masters;

	debug("process_master()", 1);

	skip_garbage();

	if (check("no_objects") != Match)
		result = error("The no_objects definition is missing", lincnt);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	if ((no_masters = getnum()) == Error)
		result = error("Cannot parse number", lincnt);

	if (check("") != Blank)
		result = error("Syntax error with no_objects command", lincnt);

	if (no_masters > 0)
	{
		/* create the space required for the master objects */
		world.masters().resize(no_masters);

		skip_garbage();
		/* process the master objects */
		if (process_objects(no_masters) != Okay)
			result = error("Error with master object definitions", lincnt);
	}
	else if (no_masters < 0)
		result = error("The no_objects is less than zero", lincnt);
	else if (no_masters == 0)
	{
		warn("There are no master objects", lincnt);

		skip_garbage();

		std::string word;
		getword(word);

		/* check that the master definition block is ended */
		if (word == ".end_masterdefs")
			result = Okay;
		else
			result = error("Error with master object definitions", lincnt);
	}

	/* return the result of parsing the master block */
	return result;
}

/****************************************************************************
* process_instance() - process a text block which contains definitions for  *
*                      the instance objects                                 *
****************************************************************************/
Status Parser::process_instance(void)
{
	Status result = Okay;
	int no_instances;

	debug("process_instance()", 1);

	/* error if the master objects haven't been defined */
	if (world.masters().empty())
		result = error("Master objects have not yet been defined", lincnt);

	skip_garbage();

	if (check("no_instances") != Match)
		result = error("The no_instances definition is missing", lincnt);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	if ((no_instances = getnum()) == Error)
		result = error("Cannot parse number", lincnt);

	if (check("") != Blank)
		result = error("Syntax error with no_instances command", lincnt);

	if (no_instances > 0)
	{
		/* create the space required to store the instance objects */
		world.instances().resize(no_instances);

		skip_garbage();
		/* process the instance objects */
		if (process_object_instances(world.instances().size(), world.masters().size()) != Okay)
			result = error("Error with instance object definitions", lincnt);
	}
	else if (no_instances < 0)
		result = error("The no instances is less than zero", lincnt);
	else if (no_instances == 0)
	{
		warn("There are no instance objects", lincnt);
		skip_garbage();

		std::string word;
		getword(word);

		/* make sure that the instance definition block is correctly ended */
		if (word == ".end_instancedefs")
			result = Okay;
		else
			result = error("Syntax error with no_instances command", lincnt);
	}

	/* return the result of parsing the instance block */
	return result;
}

/****************************************************************************
* process_user() - process a text block which contains definitions for the  *
*                  user object                                              *
****************************************************************************/
Status Parser::process_user(void)
{
	debug("process_user()", 1);

	/* error if the master objects have already been defined */
	if (!world.masters().empty())
		return (error("Master objects have already been defined", lincnt));

	while (1)
	{
		skip_garbage();

		std::string word;
		getword(word);

		if (word == "location")
		{
			/* location command */
			if (process_location(world.user().loc) == Error)
				return Error;
		}
		else if (word == "direction")
		{
			/* direction command */
			if (process_direction(world.user().ang) == Error)
				return Error;
		}
		else if (word == "radius")
		{
			/* radius command */
			if (process_radius(&world.user().radius) == Error)
				return Error;
		}
		else if (word == "sky")
		{
			/* sky command */
			if (process_sky(&world.user().sky) == Error)
				return Error;
		}
		else if (word == "ground")
		{
			/* ground command */
			if (process_ground(&world.user().ground) == Error)
				return Error;
		}
		else if (word ==".end_userdefs")
		{
			/* end of user block */
			return Okay;
		}
		else
			return(error("Syntax error in block .userdefs", lincnt));
	}
}

/****************************************************************************
* process_objects() - this function will parse through all the master       *
*                     definitions and create the master data structure      *
*                     this structure is pointed to by 'master array'        *
****************************************************************************/
Status Parser::process_objects(int no_objects)
{
	int loop, master_no;
	Status result = Okay;

	debug("process_objects()", 1);

	for (loop = 0; loop < no_objects; loop++)
	{
		if (check("master_no") != Match)
			result = error("Error with master no command", lincnt);

		if (check("=") != Match)
			result = error("Missing assignment symbol", lincnt);

		if ((master_no = getnum()) == Error)
			result = error("Cannot parse number", lincnt);

		if ((master_no < 0) || (master_no > no_objects))
			result = error("The-master no is incorrect", lincnt);

		// Set its id
		world.masters()[loop].id = master_no;

		skip_garbage();

		/* get the vertices, edges and polygons which define the object */
		if (process_object_definition(master_no-1) != Okay)
			result = Error;

		skip_garbage();

		/* get any scale and angle commands */
		if (check_object_values(master_no-1, loop, no_objects) != Okay)
			result = Error;
	}

	return result;
}

/****************************************************************************
* check_object_values() - process the optional angle and scale commands for *
*                         the master object and see that it's definition is *
*                         correctly terminated                              *
****************************************************************************/
Status Parser::check_object_values(int object_no, int object_pos, int no_objects)
{
	Vector3d ang;
	Vector3d scl;

	debug("check_object_values()", 1);

	while (1)
	{
		std::string word;
		getword(word);

		if (word == "angle")
		{
			/* get object angle values from script */
			if (process_angle(ang) == Error)
				return Error;
			/* load master structure with angle values */
			world.masters()[object_no].angle = ang;
		}
		else if (word == "scale")
		{
			/* get object scale values from script */
			if (process_scale(scl) == Error)
				return Error;

			/* fill master structure with scale values */
			world.masters()[object_no].scale = scl;
		}
		else if (word == "master_no")
		{
			/* make sure that we need to process more master objects */
			if ((object_pos+1) < no_objects)
			{
				lineptr = 0;
				return Okay;
			}
			else
				return(error("Too many master no definitions", lincnt));
		}
		else if (word == ".end_masterdefs")
		{
			/* make sure that we have no more master objects to process */
			if ((object_pos+1) == no_objects)
				return Okay;
			else
				return (error("Not all master objects have been defined", lincnt));
		}
		else
			return(error("Syntax error in block .masterdefs", lincnt));
		skip_garbage();
	}
}

/****************************************************************************
* process_polygons() - this function reads in the data that describes the   *
*                      construction of an object with sets of polygons      *
*                      each polygon is three-sided                          *
****************************************************************************/
Status Parser::process_object_definition(int object_no)
{
	Status result = Okay;
	int no_vert, no_poly;

	debug("process_polygons()", 1);

	/* make sure that object block starts correctly */
	if (check(".objectdef") != Match)
		result = error("Error with block .objectdef", lincnt);

	skip_garbage () ;

	/* now retrieve the number of vertices that make up the object */
	if (check("no_vertices") != Match)
		result = error("Error with no_vertices command", lincnt);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	if ((no_vert = getnum()) == Error)
		result = error("Cannot parse number", lincnt);

	if (no_vert > 0)
	{
		/* get the values of the vertices */
		result = process_verts(world.masters()[object_no], no_vert);
	}
	else if (no_vert < 0)
	{
		/* generate an error if the no_vertices value is illegal */
		result = error("Error with the no vertices command", lincnt);
	}

	skip_garbage();

	/* now retrieve the number of polygons that make up the object */
	if (check("no_polygons") != Match)
		result = error("Error with the no_polygons command", lincnt);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	if ((no_poly = getnum() ) == Error)
		result = error("Cannot parse number", lincnt);

	if (no_poly > 0)
	{
		/* get the values of the polygons */
		result = process_polys(world.masters()[object_no], no_poly);
	}
	else if (no_poly < 0)
	{
		/* generate an error if the no polygons value is illegal */
		result = error("Error with the no_polygons command", lincnt);
	}

	skip_garbage();

	/* check that the object definition block is properly terminated */
	if (check(".objectend") != Match)
		result = error("Error terminating object definition", lincnt);

	return result;
}

/****************************************************************************
* process_object_instances () - function to process the list of instances   *
*                               of master objects described                 *
****************************************************************************/
Status Parser::process_object_instances(int no_instances, int no_objects)
{
	int loop, master_no;
	Status result = Okay;

	debug("process_object_instances()", 1);

	for (loop = 0; loop < no_instances; loop++)
	{
		bool col_set = false;
		bool spec_set = false;
		bool style_set = false;

		/* check for the master no command */
		if (check("master_no") != Match)
			result = error("The master no is incorrect", lincnt);

		if (check("=") != Match)
			result = error("Missing assignment symbol", lincnt);

		if ((master_no = getnum()) == Error)
			result = error("Cannot parse number", lincnt);

		if ((master_no < 0) || (master_no > no_objects))
			result = error("The master_no is incorrect", lincnt);

		/* store the master no value in the instance */
		/* take one away to-match internal format of master object references */
		world.instances()[loop].masterptr = &world.masters()[master_no - 1];

		skip_garbage();

		/* now get any other commands for that instance */
		/* take one away from master no because that's the way they are */
		/* stored in the master array */
		if (check_instance_values(col_set, spec_set, style_set,
								  loop, no_instances) != Okay)
			result = Error;
		/* make sure that color, specularity & style have been specified */
		if (!col_set || !spec_set || !style_set)
		{
			result = error("Either color, specularity or style has not been set", lincnt);
		}
	}

	return result;
}

/****************************************************************************
* check_instance_values() - process the optional and required commands for  *
*                                  an instance of an object                 *
****************************************************************************/
Status Parser::check_instance_values(bool &col_set, bool &spec_set, bool &style_set, int instance_pos, int no_instances)
{
	float specularity;
	int color;

	/* set the angles, scales and locations defaults */
	Vector3d loc;
	Vector3d ang;
	Vector3d scl(1.0, 1.0, 1.0);

	debug("check_instance_values()", 1);

	while (1)
	{
		std::string word;
		getword(word);

		if (word == "location")
		{
			/* get the instance location from the file */
			if (process_location(loc) == Error)
				return Error;
			/* fill instance location values into the instance structure */
			world.instances()[instance_pos].pos = loc;
		}
		else if (word == "angle")
		{
			if (process_angle(ang) == Error)
				return Error;
			world.instances()[instance_pos].angle = ang;
		}
		else if (word == "scale")
		{
			if (process_scale(scl) == Error)
				return Error;
			world.instances()[instance_pos].scale = scl;
		}
		else if (word == "color")
		{
			/* process the color value */
			if (process_color(&color) == Error)
				return Error;
			world.instances()[instance_pos].color = color;
			col_set = true;
		}
		else if (word == "specularity")
		{
			/* process the specularity value */
			if (process_specularity(&specularity) == Error)
				return Error;
			world.instances()[instance_pos].specularity = specularity;
			spec_set = true;
		}
		else if (word == "style")
		{
			/* get the instance style from the file */
			if (process_style(world.instances()[instance_pos].style) == Error)
				return Error;
			style_set = true;
		}
		else if (word == "outcome")
		{
			if (!process_outcome(world.instances()[instance_pos].outcome))
				return Error;
			/* now we should check whether we want
			   the object to be solid or not */
			if (world.instances()[instance_pos].outcome == "")
				world.instances()[instance_pos].is_solid = false;
			else if (world.instances()[instance_pos].outcome == "ignore")
				world.instances()[instance_pos].is_solid = false;
			else if (world.instances()[instance_pos].outcome == "solid")
				world.instances()[instance_pos].is_solid = true;
			else
				world.instances()[instance_pos].is_solid = true;
		}
		else if (word == "master_no")
		{
			if ((instance_pos+1) < no_instances)
			{
				lineptr = 0;

				/* set up the instance of the master object */
				world.instances()[instance_pos].setup_vertices();
				/* set the colors of the instances facets (polygons) */
				world.instances()[instance_pos].setup_color();
				/* finially we set up the collision box around the object */
				world.instances()[instance_pos].setup_bounds();
				return Okay;
			}
			else
				return (error("Too many instance definitions", lincnt));
		}
		else if (word == ".end_instancedefs")
		{
			if ((instance_pos+1) == no_instances)
			{
				/* set up the instance of the master object */
				world.instances()[instance_pos].setup_vertices();
				/* set the colors if the instances facets */
				world.instances()[instance_pos].setup_color();
				/* finially we set up the collision box around the object */
				world.instances()[instance_pos].setup_bounds();
				return Okay;
			}
			else
				return(error("Not all instances have been defined", lincnt));
		}
		else
			return (error("Syntax error in block .instancedefs", lincnt));

		skip_garbage();
	}
}

/****************************************************************************
* process_location() - process the command which defines the user obJect's  *
*                      initial location                                     *
****************************************************************************/
Status Parser::process_location(Vector3d &loc)
{
	Status result = Okay;

	debug("process_location()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	loc.x(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with location definition", lincnt);

	loc.y(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with location definition", lincnt);
        
	loc.z(fgetnum());

	if (check("") != Blank)
		result = error("Syntax error with location definition", lincnt);

	return result;
}

/****************************************************************************
* process_direction() - process the command which defines the user object's *
*                       initial direction                                   *
****************************************************************************/
Status Parser::process_direction(Vector3d &ang)
{
	Status result = Okay;

	debug("process_direction", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);
        
	ang.x(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with user direction definition", lincnt);
        
	ang.y(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with user direction definition", lincnt);
        
	ang.z(fgetnum());

	if (check("") != Blank)
		result = error("Syntax error with user direction definition", lincnt);

	return result;
}

/****************************************************************************
* process_radius - process the command which defines the user object's      *
*                  radius                                                   *
****************************************************************************/
Status Parser::process_radius(float *radius)
{
	Status result = Okay;

	debug("process_radius()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);
        
	*radius = fgetnum();

	if (check("") != Blank)
		result = error("Syntax error with user radius definition", lincnt);

	return result;
}

/****************************************************************************
* process_angle() - process the command which defines the master object's   *
*                   initial angle                                           *
****************************************************************************/
Status Parser::process_angle(Vector3d &ang)
{
	Status result = Okay;

	debug("process_angle()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	ang.x(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with angle command", lincnt);

	ang.y(fgetnum());

	if (check(", ") != Match)
		result = error("Syntax error with angle command", lincnt);

	ang.z(fgetnum());

	if (check("") != Blank)
		result = error("Syntax error with angle command", lincnt);

	return result;
}

/****************************************************************************
* process_scale() - process the command which defines the master object's   *
*                   initial scale                                           *
****************************************************************************/
Status Parser::process_scale(Vector3d &scl)
{
	Status result = Okay;

	debug("process_scale()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	scl.x(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with scale command", lincnt);

	scl.y(fgetnum());

	if (check(",") != Match)
		result = error("Syntax error with scale command", lincnt);

	scl.z(fgetnum());

	if (check("") != Blank)
		result = error("Syntax error with scale command", lincnt);

	return result;
}

/****************************************************************************
* process sky - process the command which defines the color of the sky      *
****************************************************************************/
Status Parser::process_sky(int *color)
{
	Status result = Okay;

	debug("process_sky()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	if ((*color = getnum()) == Error)
		result = error("Syntax error with sky color definition", lincnt);

	if (check("") != Blank)
		result = error("Syntax error with sky color definition", lincnt);

	return result;
}

/****************************************************************************
* process_ground - process the command which defines the color of the       *
*                  ground                                                   *
****************************************************************************/
Status Parser::process_ground(int *color)
{
	Status result = Okay;

	debug("process_ground()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	if ((*color = getnum()) == Error)
		result = error("Syntax error with ground color definition", lincnt);

	if (check("") != Blank)
		result = error("Syntax error with ground color definition", lincnt);

	return result;
}

/****************************************************************************
* process_color() - process the command which defines the instance          *
*                    object' s color                                        *
****************************************************************************/
Status Parser::process_color(int *color)
{
	Status result = Okay;
	std::string word;

	debug("process_color()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	getword(word);

	/* get the color value from the color command */
	if ((word == "BLACK") || (word == "0"))
		*color = 0;
	else if ((word == "BLUE") || (word == "1"))
		*color = 1;
	else if ((word == "GREEN") || (word == "2"))
		*color = 2;
	else if ((word == "CYAN") || (word == "3"))
		*color = 3;
	else if ((word == "RED") || (word == "4"))
		*color = 4;
	else if ((word == "MAGENTA") || (word == "5"))
		*color = 5;
	else if ((word == "BROWN") || (word == "6"))
		*color = 6;
	else if ((word == "LIGHTGREY") || (word == "7"))
		*color = 7;
	else if ((word == "DARKGREY") || (word == "8"))
		*color = 8;
	else if ((word == "LIGHTBLUE") || (word == "9"))
		*color = 9;
	else if ((word == "LIGHTGREEN") || (word == "10"))
		*color = 10;
	else if ((word == "LIGHTCYAN") || (word == "11"))
		*color = 11;
	else if ((word == "LIGHTRED") || (word == "12"))
		*color = 12;
	else if ((word == "LIGHTMAGENTA") || (word == "13"))
		*color = 13;
	else if ((word == "YELLOW") || (word == "14"))
		*color = 14;
	else if ((word == "WHITE") || (word == "15"))
		*color = 15;
	else
		result = error("Invalid color value", lincnt);

	if (check("") != Blank)
		result = error("Syntax error with color command", lincnt);

	if ((*color < 0) || (*color > 15))
		result = error("Invalid color value", lincnt);

	return result;
}

/****************************************************************************
* process specularity() - process the command which defines the instance    *
*                         object' s specularity                             *
****************************************************************************/
Status Parser::process_specularity(float *specularity)
{
	Status result = Okay;
	MatchResult rest_of_line;

	debug("process_specularity()", 1);

	/* make sure that there is an equals sign */
	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	*specularity = fgetnum();

	/* check to see if there is a percent sign at the end of the line */
	rest_of_line = check("%");

	if ((rest_of_line != Blank) && (rest_of_line != Match))
		result = error("Syntax error with specularity command", lincnt);

	/* make sure the specularity value is a percentage */
	if ((*specularity < 0.0) || (*specularity > 100.0))
		result = error("Invalid specularity number", lincnt);

	return result;
}

/****************************************************************************
* process_outcome() - process the command which defines the instance        *
*                     object's outcome string                               *
*                     this can either make an object solid or not           *
*                     there is also the possibility to execute DOS commands *
*                     with this string                                      *
****************************************************************************/
bool Parser::process_outcome(std::string &outcome)
{
	debug("process_outcome()", 1);

	/* make sure that there is an equals sign */
	if (check("=") != Match)
	{
		error("Missing assignment symbol", lincnt);
		return false;
	}
	
	outcome.clear();

	/* if the string is in error then return error */
	if (getstring(outcome) == Error)
	{
		error("Syntax error with outcome string", lincnt);
		return false;
	}

	return true;
}

/****************************************************************************
* process_style() - process the command which defines the instance object's *
*                   style                                                   *
****************************************************************************/
Status Parser::process_style(RenderStyle &style)
{
	Status result = Okay;
	std::string word;

	debug("process_style()", 1);

	if (check("=") != Match)
		result = error("Missing assignment symbol", lincnt);

	getword(word);

	if (word == "SOLID")
		style = RenderStyle::SOLID;
	else if (word == "WIREFRAME")
		style = RenderStyle::WIREFRAME;
	else
		result = error("Unknown style type", lincnt);

	return result;
}

/****************************************************************************
* process_vertices() - process the object's vertices                        *
****************************************************************************/
Status Parser::process_verts(Master &mast, int no_vertices)
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
			result = error("Syntax error with vertex command", lincnt);

		if ((vert_no < 1) || (vert_no > no_vertices))
			result = error("Syntax error with vertex command", lincnt);

		if (check("=") != Match)
			result = error("Missing assignment symbol", lincnt);

		/* now we get the x, y and z values of the vertex */
		mast.vert[vert_no-1].x(fgetnum());

		if (check(",") != Match)
			result = error("Syntax error with vertex command", lincnt);

		mast.vert[vert_no-1].y(fgetnum());

		if (check(",") != Match)
			result = error("Syntax error with vertex command", lincnt);

		mast.vert[vert_no-1].z(fgetnum());

		/* make sure there is no more text on the end of the line */
		if (check("") != Blank)
			result = error("Syntax error with vertex command", lincnt);
	}

	return result;
}

/****************************************************************************
* process_polygons() - process the object's polygons                        *
****************************************************************************/
Status Parser::process_polys(Master &mast, int no_polygons)
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
			result = error("Syntax error with polygon command", lincnt);

		if ((poly_no < 1) || (poly_no > no_polygons))
			result = error("Syntax error with polygon command", lincnt);

		if (check("=") != Match)
			result = error("MAssing assignment symbol", lincnt);

		/* now we get the three edges that make up the polygon */
		tmp = getnum();

		/* now make sure that it is a valid vertex reference */
		if ((tmp < 1) || (tmp > mast.vert.size()))
			result = error("Illegal polygon value", lincnt);

		/* remember to take one from values to match array structure */
		mast.poly0[poly_no-1] = tmp - 1;

		if (check(",") != Match)
			result = error("Syntax error with edge command", lincnt);

		/* next edge reference */
		tmp = getnum();

		/* now make sure that it is a valid edge reference */
		if ((tmp < 1) || (tmp > mast.vert.size()))
			result = error("Illegal polygon value", lincnt);

		/* remember to take one from values to match array structure */
		mast.poly1[poly_no-1] = tmp - 1;
		
		if (check(",") != Match)
			result = error("Syntax error with edge command", lincnt);

		/* final edge reference */
		tmp = getnum();

		/* now make sure that it is a valid edge reference */
		if ((tmp < 1) || (tmp > mast.vert.size()))
			result = error("Illegal polygon value", lincnt);

		/* remember to take one from values to match array structure */
		mast.poly2[poly_no-1] = tmp - 1;

		/* make sure there is no more text on the end of the line */
		if (check("") != Blank)
			result = error("Syntax error with edge command", lincnt);
	}

	return result;
}

/****************************************************************************
* getline() - function takes the next line from the file pointed to by '*fp'*
*             and stores it in the char array 'LINE'                        *
*             A return code tells if the line is a comment, blank,          *
*             end-of-file or something else.                                *
*             returns: EoF if the file is at the end                        *
*                      Comment if the line is a comment                     *
*                      Blank if the line contains nothing                   *
*                      Other if the line contains something other than a    *
*                      comment                                              *
****************************************************************************/
MatchResult Parser::getline(void)
{
	debug("getline()", 1);

	std::getline(file, line);

	if (file.eof())
	{
		return EoF;
	}

	lincnt++;
	lineptr = 0;

	debug(line.c_str(), 2);

	if (line.find_first_of("#") == 0)
	{
		return Comment;
	}
	else if (line.find_first_not_of("\t\n ") == std::string::npos)
	{
		return Blank;
	}

	return Other;
}

/****************************************************************************
* skip_garbage() - function to simply skip over those lines that are either *
*                  comments (i.e. lines starting with a '#') or blank       *
****************************************************************************/
void Parser::skip_garbage(void)
{
	int retcode;

	debug("skip_garbage()", 1);

	retcode = getline();

	while ((retcode == Comment) || (retcode == Blank))
		retcode = getline();

	if (retcode == EoF)
	{
		error("Unexpected end of file", lincnt);
		file.close();
		exit(-1);
	}
}

/****************************************************************************
* check() - function to check that the next string in the array 'LINE'      *
*           matches with a string passed into the function.                 *
*           returns: Match if there is a match                              *
*                    NoMatch is there is a string but it does not match     *
*                    Blank if there is nothing in the LINE to match         *
****************************************************************************/
MatchResult Parser::check(const std::string &string)
{
	debug("check()", 1);

	while ((line[lineptr] == ' ') || (line[lineptr] == '\t'))
		lineptr++;

	if (static_cast<unsigned int>(lineptr) == line.size())
		return Blank;

	if (line.find_first_of(string, lineptr) == static_cast<unsigned int>(lineptr))
	{
		lineptr += string.size();
		return Match;
	}
	else
	{
		return NoMatch;
	}
}

/****************************************************************************
* getword() - function retrieves next word in the array 'LINE'              *
*             a word is defined as a string of characters surrounded by     *
*             white space characters and end-of-line characters             *
****************************************************************************/
void Parser::getword(std::string &word)
{
	debug("getword()", 1);

	while ((line[lineptr] == ' ') || (line[lineptr] == '\t'))
		lineptr++;

	while (line[lineptr] != '\0')
	{
		if ((line[lineptr] == '\n') || (line[lineptr] == ' ') || (line[lineptr] == '\t'))
			break;
		word.push_back(line[lineptr++]);
	}
}

/****************************************************************************
* getstring() - function retrieves a string - this is defined as a series   *
*               characters inside a pair of double quotes                   *
****************************************************************************/
Status Parser::getstring(std::string &word)
{
	Status result = Error;

	debug("getstring()", 1);
	debug("before while loop", 2);

	/* skip any spaces or tabs before the string starts */
	while ((line[lineptr] == ' ') || (line[lineptr] == '\t'))
		lineptr++;

	debug("now check for double quote mark", 2);

	/* check that there's a double quote character there */
	if (line[lineptr++] != '\"')
		return result;

	debug("now get the string", 2);

	/* now put the rest of the line into char array pointed to by tptr */
	while (line[lineptr] != '\"')
	{
		/* check to make sure that we haven't reached the end of the
		   line retrieved from the file */
		if (line[lineptr] == '\0')
			return result;

		/* otherwise we put the value into the char array pointed to by tptr */
		word.push_back(line[lineptr++]);
	}

	debug("string has been fetched", 2);

	result = Okay;

	return result;
}

/****************************************************************************
* getnum() - function retrieves an integer number from the char array 'LINE'*
****************************************************************************/
int Parser::getnum(void)
{
	char numline[MAXLINE];
	int tptr = 0;

	debug("getnum()", 1);

	while ((line[lineptr] == ' ') || (line[lineptr] == '\t'))
		lineptr++;

	if (line[lineptr] == '\n')
		return -1;

	numline[tptr++] = line[lineptr++];

	while (line[lineptr] != '\0')
	{
		if ((line[lineptr] == '\n') || (line[lineptr] == ' ') || (line[lineptr] == '\t'))
			break;
		numline[tptr++] = line[lineptr++];
	}

	numline[tptr] = '\0';

	return (atoi(numline));
}

/****************************************************************************
* fgetnum() - function retrieves a floating point number from the char      *
*             array 'LINE'                                                  *
****************************************************************************/
float Parser::fgetnum(void)
{
	char fnumline[MAXLINE];
	int tptr = 0;

	debug("fgetnum()", 1);

	while ((line[lineptr] == ' ') || (line[lineptr] == '\t'))
		lineptr++;

	fnumline[tptr++] = line[lineptr++];

	while (line[lineptr] != '\0')
	{
		if ((line[lineptr] == '\n') || (line[lineptr] == ' ') || (line[lineptr] == '\t'))
			break;
		fnumline[tptr++] = line[lineptr++];
	}

	fnumline[tptr] = '\0';

	return ((float)atof(fnumline));
}

/****************************************************************************
* get_point - function to retrieve point definition which is an X, Y and Z  *
*             coordinate - three of these describe a polygon in 3d space    *
****************************************************************************/
Status Parser::get_point(float *pntx, float *pnty, float *pntz)
{
	Status result = Okay;

	debug("get_point()", 1);

	*pntx = fgetnum();

	if (check(",") != Match)
		result = Error;

	*pnty = fgetnum();

	if (check(",") != Match)
		result = Error;

	*pntz = fgetnum();

	if (check("") != Blank)
		result = Error;

	return result;
}
