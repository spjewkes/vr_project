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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.hpp"
#include "error.hpp"
#include "parse.hpp"
#include "pcomnds.hpp"
#include "pfuncs.hpp"
#include "setup.hpp"
#include "vector3d.hpp"

/* Global variables */
void *fp;

/* Constructor */
Parser::Parser() : masterdef_processed(false)
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
Status Parser::parse(char *filename)
{
	MatchResult retcode;
	debug("parse()", 1);

	/* lincnt stores currently processed line */
	lincnt = 0;

	if ((fp = (FILE *)fopen(filename, "r")) == NULL)
	{
		/* error if can't open file */
		error("0001", "Error opening file", lincnt);
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
					error("0002", "Cannot parse file", lincnt);
					return Error;
				}
			/* get next line */
			retcode = getline();
		}
	}
	/* close the open file */
	fclose((FILE *)fp) ;
	return Okay;
}

/*****************************************************************************
 * init_master() - initialises the array which will store the master objects *
 *****************************************************************************/
Status Parser::init_master(int no_masters)
{
	debug("init_master()", 1);

	/* create the space required for the master objects */
	m_masters.resize(no_masters);

	for (auto &mast : m_masters)
	{
		/* set all the object's scales to 1.0 */
		mast.scale.x(1.0);
		mast.scale.y(1.0);
		mast.scale.z(1.0);
		/* set all the obJect's angles to 0.0 */
		mast.angle.x(0.0);
		mast.angle.y(0.0);
		mast.angle.z(0.0);
	}

	return Okay;
}

/****************************************************************************
* init_instance() - initialises the array which will store the instances of *
*                   the master objects                                      *
****************************************************************************/
Status Parser::init_instance(int no_instances)
{
	debug("init_instance()", 1);

	/* create the space required to store the instance objects */
	m_instances.resize(no_instances);

	for (auto &inst : m_instances)
	{
		/* set the initial location of all the objects to 0 */
		inst.pos.x(0.0);
		inst.pos.y(0.0);
		inst.pos.z(0.0);
		/* set all the minimum values of all the object to 0 */
		inst.min.x(0.0);
		inst.min.y(0.0);
		inst.min.z(0.0);
		/* set all the maximum values of all the objects to 0 */
		inst.max.x(0.0);
		inst.max.y(0.0);
		inst.max.z(0.0);
		/* set all the instance's scales to 1.0 */
		inst.scale.x(1.0);
		inst.scale.y(1.0);
		inst.scale.z(1.0);
		/* set all the instance's angles to 0.0 */
		inst.angle.x(0.0);
		inst.angle.y(0.0);
		inst.angle.z(0.0);
		/* default master no should be set to 0 (this will be changed though */
		inst.master_no = 0;
		/* set the default style of the object to wlreframe */
		inst.style = RenderStyle::WIREFRAME;
		/* set the default solidity of the object to false */
		inst.is_solid = false;
		/* set up the default outcome string */
		inst.outcome = "";
	}
	return Okay;
}

/*****************************************************************************
* init_user() - initialises user variables                                   *
*****************************************************************************/
void Parser::init_user()
{
	debug("init_user()", 1);

	m_user.loc.x(0.0);
	m_user.loc.y(0.0);
	m_user.loc.z(0.0);

	m_user.ang.x(0.0);
	m_user.ang.y(0.0);
	m_user.ang.z(0.0);

	m_user.radius = 1.0;

	m_user.sky = 0;
	m_user.ground = 0;
}

/****************************************************************************
* process() - function to process any commands that may occur in the file   *
*             at this level it searches for one of three headers:           *
*             ".masterdefs", ".instance defs" or ".userdefs"                *
*             any other strings will-result in a syntax error               *
****************************************************************************/
Status Parser::process(void)
{
	char word[MAXLINE];

	debug("process()", 1);

	/* get the first word in LINE */
	getword(word);

	if (strlen(word) == 0)
		return Okay;
	else if (strcmp(word, ".masterdefs") == 0)
		/* a master block */
		return process_master();
	else if (strcmp(word, ".instancedefs") == 0)
		/* an instance block */
		return process_instance();
	else if (strcmp(word, ".userdefs") == 0)
		/* a user block */
		return process_user();
	else
	{
		error("0003", "Syntax error", lincnt);
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
	char word[MAXLINE];
	int no_masters;

	debug("process_master()", 1);

	skip_garbage();

	if (check("no_objects") != Match)
		result = error("0005", "The no_objects definition is missing", lincnt);
	masterdef_processed = true;

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((no_masters = getnum()) == Error)
		result = error("0007", "Cannot parse number", lincnt);

	if (check("") != Blank)
		result = error("0041", "Syntax error with no_objects command", lincnt);

	if (no_masters > 0)
	{
		if (init_master(no_masters) == Error)
			return Error;
		skip_garbage();
		/* process the master objects */
		if (process_objects(no_masters) != Okay)
			result = error("0008", "Error with master object definitions", lincnt);
	}
	else if (no_masters < 0)
		result = error("0009", "The no_objects is less than zero", lincnt);
	else if (no_masters == 0)
	{
		warn("0001", "There are no master objects", lincnt);

		skip_garbage();

		getword(word);

		/* check that the master definition block is ended */
		if (strcmp(word, ".end_masterdefs") == 0)
			result = Okay;
		else
			result = error("0008", "Error with master object definitions", lincnt);
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
	char word[MAXLINE];
	int no_instances;

	debug("process_instance()", 1);

	/* error if the master objects haven't been defined */
	if (!masterdef_processed)
		result = error("0011", "Master objects have not yet been defined", lincnt);

	skip_garbage();

	if (check("no_instances") != Match)
		result = error("0012", "The no_instances definition is missing", lincnt);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((no_instances = getnum()) == Error)
		result = error("0007", "Cannot parse number", lincnt);

	if (check("") != Blank)
		result = error("0042", "Syntax error with no_instances command", lincnt);

	if (no_instances > 0)
	{
		if (init_instance(no_instances) == Error)
			return Error;
		skip_garbage();
		/* process the instance objects */
		if (process_object_instances(m_instances.size(), m_masters.size()) != Okay)
			result = error("0013", "Error with instance object definitions", lincnt);
	}
	else if (no_instances < 0)
		result = error("0014", "The no instances is less than zero", lincnt);
	else if (no_instances == 0)
	{
		warn("0002", "There are no instance objects", lincnt);
		skip_garbage();

		getword(word);

		/* make sure that the instance definition block is correctly ended */
		if (strcmp(word, ".end_instancedefs") == 0)
			result = Okay;
		else
			result = error("0042", "Syntax error with no_instances command", lincnt);
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
	char word[MAXLINE];

	debug("process_user()", 1);

	/* error if the master objects have already been defined */
	if (masterdef_processed)
		return (error("0004", "Master objects have already been defined", lincnt));

	init_user();

	for EVER
	{
		skip_garbage();

		getword(word);

		if (strcmp(word, "location") == 0)
		{
			/* location command */
			if (process_location(m_user.loc) == Error)
				return Error;
		}
		else if (strcmp(word, "direction") == 0)
		{
			/* direction command */
			if (process_direction(m_user.ang) == Error)
				return Error;
		}
		else if (strcmp (word, "radius") == 0)
		{
			/* radius command */
			if (process_radius(&m_user.radius) == Error)
				return Error;
		}
		else if (strcmp(word, "sky") == 0)
		{
			/* sky command */
			if (process_sky(&m_user.sky) == Error)
				return Error;
		}
		else if (strcmp(word, "ground") == 0)
		{
			/* ground command */
			if (process_ground(&m_user.ground) == Error)
				return Error;
		}
		else if (strcmp(word, ".end_userdefs") == 0)
		{
			/* end of user block */
			return Okay;
		}
		else
			return(error("0016", "Syntax error in block .userdefs", lincnt));
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
			result = error("0020", "Error with master no command", lincnt);

		if (check("=") != Match)
			result = error("0006", "Missing assignment symbol", lincnt);

		if ((master_no = getnum()) == Error)
			result = error("0007", "Cannot parse number", lincnt);

		if ((master_no < 0) || (master_no > no_objects))
			result = error("0021", "The-master no is incorrect", lincnt);

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
	char word[MAXLINE];

	Vector3d ang;
	Vector3d scl;

	debug("check_object_values()", 1);

	for EVER
	{
		getword(word);

		if (strcmp(word, "angle") == 0)
		{
			/* get object angle values from script */
			if (process_angle(ang) == Error)
				return Error;
			/* load master structure with angle values */
			m_masters[object_no].angle = ang;
		}
		else if (strcmp(word, "scale") == 0)
		{
			/* get object scale values from script */
			if (process_scale(scl) == Error)
				return Error;

			/* fill master structure with scale values */
			m_masters[object_no].scale = scl;
		}
		else if (strcmp(word, "master_no") == 0)
		{
			/* make sure that we need to process more master objects */
			if ((object_pos+1) < no_objects)
			{
				lineptr = 0;
				return Okay;
			}
			else
				return(error("0022", "Too many master no definitions", lincnt));
		}
		else if (strcmp (word, ".end_masterdefs") == 0)
		{
			/* make sure that we have no more master objects to process */
			if ((object_pos+1) == no_objects)
				return Okay;
			else
				return (error("0023", "Not all master objects have been defined", lincnt));
		}
		else
			return(error("0039", "Syntax error in block .masterdefs", lincnt));
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
	int no_vert, no_edge, no_poly;

	debug("process_polygons()", 1);

	/* make sure that object block starts correctly */
	if (check(".objectdef") != Match)
		result = error("0027", "Error with block .objectdef", lincnt);

	skip_garbage () ;

	/* now retrieve the number of vertices that make up the object */
	if (check("no_vertices") != Match)
		result = error("0045", "Error with no_vertices command", lincnt);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((no_vert = getnum()) == Error)
		result = error("0007", "Cannot parse number", lincnt);

	if (no_vert > 0)
	{
		/* get the values of the vertices */
		result = process_verts(m_masters[object_no], no_vert);
	}
	else if (no_vert < 0)
	{
		/* generate an error if the no_vertices value is illegal */
		result = error("0045", "Error with the no vertices command", lincnt);
	}

	skip_garbage();

	/* now retrieve the number of edges that make up the object */
	if (check("no_edges") != Match)
		result = error("0046", "Error with no_edges command", lincnt);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((no_edge = getnum() ) == Error)
		result = error("0007", "Cannot parse number", lincnt);

	if (no_edge > 0)
	{
		/* get the values of the edges */
		result = process_edges(m_masters[object_no], no_edge);
	}
	else if (no_edge < 0)
	{
		/* generate an error if the no_edges value is illegal */
		result = error("0046", "Error with the no_edges command", lincnt);
	}

	skip_garbage();

	/* now retrieve the number of polygons that make up the object */
	if (check("no_polygons") != Match)
		result = error("0025", "Error with the no_polygons command", lincnt);

	if (check("=") != Match)
		result = error("0006", "Missing assignment symbol", lincnt);

	if ((no_poly = getnum() ) == Error)
		result = error("0007", "Cannot parse number", lincnt);

	if (no_poly > 0)
	{
		/* get the values of the polygons */
		result = process_polys(m_masters[object_no], no_poly);
	}
	else if (no_poly < 0)
	{
		/* generate an error if the no polygons value is illegal */
		result = error("0025", "Error with the no_polygons command", lincnt);
	}

	skip_garbage();

	/* check that the object definition block is properly terminated */
	if (check(".objectend") != Match)
		result = error("0028", "Error terminating object definition", lincnt);

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
			result = error("0021", "The master no is incorrect", lincnt);

		if (check("=") != Match)
			result = error("0006", "Missing assignment symbol", lincnt);

		if ((master_no = getnum()) == Error)
			result = error("0007", "Cannot parse number", lincnt);

		if ((master_no < 0) || (master_no > no_objects))
			result = error("0021", "The master_no is incorrect", lincnt);

		/* store the master no value in the instance */
		/* take one away to-match internal format of master object references */
		m_instances[loop].master_no = master_no - 1;

		skip_garbage();

		/* now get any other commands for that instance */
		/* take one away from master no because that's the way they are */
		/* stored in the master array */
		if (check_instance_values(col_set, spec_set, style_set,
								  loop, no_instances, master_no-1) != Okay)
			result = Error;
		/* make sure that colour, specularity & style have been specified */
		if (!col_set || !spec_set || !style_set)
		{
			result = error("0030", "Either colour, specularity or style has not been set", lincnt);
		}
	}

	return result;
}

/****************************************************************************
* check_instance_values() - process the optional and required commands for  *
*                                  an instance of an object                 *
****************************************************************************/
Status Parser::check_instance_values(bool &col_set, bool &spec_set, bool &style_set, int instance_pos, int no_instances, int master_no)
{
	char word[MAXLINE] ;
	float specularity;
	int colour;

	/* set the angles, scales and locations defaults */
	Vector3d loc;
	Vector3d ang;
	Vector3d scl(1.0, 1.0, 1.0);

	debug("check_instance_values()", 1);

	for EVER
	{
		getword(word);

		if (strcmp(word, "location") == 0)
		{
			/* get the instance location from the file */
			if (process_location(loc) == Error)
				return Error;
			/* fill instance location values into the instance structure */
			m_instances[instance_pos].pos = loc;
		}
		else if (strcmp(word, "angle") == 0)
		{
			if (process_angle(ang) == Error)
				return Error;
			m_instances[instance_pos].angle = ang;
		}
		else if (strcmp(word, "scale") == 0)
		{
			if (process_scale(scl) == Error)
				return Error;
			m_instances[instance_pos].scale = scl;
		}
		else if (strcmp(word, "colour") == 0)
		{
			/* process the colour value */
			if (process_colour(&colour) == Error)
				return Error;
			col_set = true;
		}
		else if (strcmp(word, "specularity") == 0)
		{
			/* process the specularity value */
			if (process_specularity(&specularity) == Error)
				return Error;
			spec_set = true;
		}
		else if (strcmp(word, "style") == 0)
		{
			/* get the instance style from the file */
			if (process_style(m_instances[instance_pos].style) == Error)
				return Error;
			style_set = true;
		}
		else if (strcmp(word, "outcome") == 0)
		{
			if (!process_outcome(m_instances[instance_pos].outcome))
				return Error;
			/* now we should check whether we want
			   the object to be solid or not */
			if (m_instances[instance_pos].outcome == "")
				m_instances[instance_pos].is_solid = false;
			else if (m_instances[instance_pos].outcome == "ignore")
				m_instances[instance_pos].is_solid = false;
			else if (m_instances[instance_pos].outcome == "solid")
				m_instances[instance_pos].is_solid = true;
			else
				m_instances[instance_pos].is_solid = true;
		}
		else if (strcmp(word, "master_no") == 0)
		{
			if ((instance_pos+1) < no_instances)
			{
				lineptr = 0;
				/* set up the instance of the master object */
				create_object_instance(m_masters[master_no], m_instances[instance_pos]);
				/* translate the instance */
				translation(m_instances[instance_pos], loc.x(), loc.y(), loc.z());
				/* set the colours of the instances facets (polygons) */
				set_colour(m_masters[master_no], m_instances[instance_pos], colour, specularity);
				/* finially we set up the collision box around the object */
				set_bound(m_instances[instance_pos]);
				return Okay;
			}
			else
				return (error("0031", "Too many instance definitions", lincnt));
		}
		else if (strcmp (word, ".end_instancedefs") == 0)
		{
			if ((instance_pos+1) == no_instances)
			{
				/* set up the instance of the master object */
				create_object_instance(m_masters[master_no], m_instances[instance_pos]);
				/* translate the instance */
				translation(m_instances[instance_pos], loc.x(), loc.y(), loc.z());
				/* set the colours if the instances facets */
				set_colour(m_masters[master_no], m_instances[instance_pos], colour, specularity);
				/* finially we set up the collision box around the object */
				set_bound(m_instances[instance_pos]);
				return Okay;
			}
			else
				return(error("0032", "Not all instances have been defined", lincnt));
		}
		else
			return (error ("0040", "Syntax error in block .instancedefs", lincnt));

		skip_garbage();
	}
}
