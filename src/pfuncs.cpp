/************************************************
*                                               *
* ####  ##### ## ## ##  ##  ####  ####     #### *
* ## ## ##    ## ## ### ## ##    ##       ##    *
* ####  ####  ## ## ###### ##    #####    ##    *
* ##    ##    ## ## ## ### ##       ##    ##    *
* ##    ##     ###  ##  ##  #### ####  ##  #### *
*                                               *
************************************************/
/****************
* include files *
****************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.hpp"
#include "error.hpp"
#include "pfuncs.hpp"

/* Global variables */
static char LINE[MAXLINE];
extern void *fp;

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
MatchResult getline(void)
{
	MatchResult retcode;

	debug("getline()", 1);

	if ((fgets(LINE, MAXLINE, (FILE *)fp)) == NULL)
		return EoF;

	lincnt++;
	lineptr = 0;
	retcode = check("#");
	lineptr = 0;

	debug(LINE, 2);

	if (retcode == Match)
		return Comment;
	else if (retcode == Blank)
		return Blank;
	else
		return Other;
}

/****************************************************************************
* skip_garbage() - function to simply skip over those lines that are either *
*                  comments (i.e. lines starting with a '#') or blank       *
****************************************************************************/
void skip_garbage(void)
{
	int retcode;

	debug("skip_garbage()", 1);

	retcode = getline();

	while ((retcode == Comment) || (retcode == Blank))
		retcode = getline();

	if (retcode == EoF)
	{
		error("0038", "Unexpected end of file", lincnt);
		fclose((FILE *)fp);
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
MatchResult check(const char *ptr)
{
	debug("check()", 1);

	while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
		lineptr++;

	if (LINE[lineptr] == '\n')
		return Blank;

	while ((*ptr != '\0') && (*ptr == LINE[lineptr]))
	{
		lineptr++;
		ptr++;
	}

	if (*ptr != '\0')
		return NoMatch;
	else
		return Match;
}

/****************************************************************************
* getword() - function retrieves next word in the array 'LINE'              *
*             a word is defined as a string of characters surrounded by     *
*             white space characters and end-of-line characters             *
****************************************************************************/
void getword(char *word)
{
	char *tptr;

	debug("getword()", 1);

	tptr = word;

	while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
		lineptr++;

	while (LINE[lineptr] != '\0')
	{
		if ((LINE[lineptr] == '\n') || (LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
			break;
		*tptr++ = LINE[lineptr++];
	}

	*tptr = '\0';
}

/****************************************************************************
* getstring() - function retrieves a string - this is defined as a series   *
*               characters inside a pair of double quotes                   *
****************************************************************************/
Status getstring(char *word)
{
	char *tptr;
	Status result = Error;

	debug("getstring()", 1);

	tptr = word;

	debug("before while loop", 2);

	/* skip any spaces or tabs before the string starts */
	while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
		lineptr++;

	debug("now check for double quote mark", 2);

	/* check that there's a double quote character there */
	if (LINE[lineptr++] != '\"')
		return result;

	debug("now get the string", 2);

	/* now put the rest of the line into char array pointed to by tptr */
	while (LINE[lineptr] != '\"')
	{
		/* check to make sure that we haven't reached the end of the
		   line retrieved from the file */
		if (LINE[lineptr] == '\0')
			return result;

		/* otherwise we put the value into the char array pointed to by tptr */
		*tptr++ = LINE[lineptr++];
	}

	debug("string has been fetched", 2);

	/* terminate the char array pointed to by tptr with a null value */
	*tptr = '\0';

	result = Okay;

	return result;
}

/****************************************************************************
* getnum() - function retrieves an integer number from the char array 'LINE'*
****************************************************************************/
int getnum(void)
{
	char numline[MAXLINE];
	int tptr = 0;

	debug("getnum()", 1);

	while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
		lineptr++;

	if (LINE[lineptr] == '\n')
		return -1;

	numline[tptr++] = LINE[lineptr++];

	while (LINE[lineptr] != '\0')
	{
		if ((LINE[lineptr] == '\n') || (LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
			break;
		numline[tptr++] = LINE[lineptr++];
	}

	numline[tptr] = '\0';

	return (atoi(numline));
}

/****************************************************************************
* fgetnum() - function retrieves a floating point number from the char      *
*             array 'LINE'                                                  *
****************************************************************************/
float fgetnum(void)
{
	char fnumline[MAXLINE];
	int tptr = 0;

	debug("fgetnum()", 1);

	while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
		lineptr++;

	fnumline[tptr++] = LINE[lineptr++];

	while (LINE[lineptr] != '\0')
	{
		if ((LINE[lineptr] == '\n') || (LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
			break;
		fnumline[tptr++] = LINE[lineptr++];
	}

	fnumline[tptr] = '\0';

	return ((float)atof(fnumline));
}

/****************************************************************************
* get_point - function to retrieve point definition which is an X, Y and Z  *
*             coordinate - three of these describe a polygon in 3d space    *
****************************************************************************/
Status get_point(float *pntx, float *pnty, float *pntz)
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

/***************************************************************************
* rotate() - function to rotate a single point described by an x, y and z  *
*            value by three angles in the x,y and z planes                 *
***************************************************************************/
void rotate(float *pntx, float *pnty, float *pntz,
			float angx, float angy, float angz)
{
	float cax, cay, caz, sax, say, saz;
	float A, B, C, D;

	/* Convert x, y and z coords from degrees to radians */
	/* and apply them to both the cosine and the sine functions */
	cax = cos(angx/RADCONST);
	sax = sin(angx/RADCONST);
	cay = cos(angy/RADCONST);
	say = sin(angy/RADCONST);
	caz = cos(angz/RADCONST);
	saz = sin(angz/RADCONST);

	/* perform the rotation */
	A = *pntx;
	B = (*pnty * cax) + (*pntz * sax);
	C = (*pntz * cax) - (*pnty * sax);
	D = (cay * A) - (say * C);
	*pntz = (say * A) + (cay * C);
	*pnty = (B * caz) - (D * saz);
	*pntx = (D * caz) + (B * saz);
}

/***************************************************************************
* scale() - function to scale a single point described by an x, y and z    *
*           value by three scale values for the x,y and z planes           *
***************************************************************************/
void scale(float *pntx, float *pnty, float *pntz,
		   float sclx, float scly, float sclz)
{
	*pntx *= sclx;
	*pnty *= scly;
	*pntz *= sclz;
}

/***************************************************************************
* translate() - function to translate a single point described by an x, y  *
*               and z value by three translation values for the x, y and   *
*               z planes                                                   *
***************************************************************************/
void translate(float *pntx, float *pnty, float *pntz,
			   float trnx, float trny, float trnz)
{
	*pntx += trnx;
	*pnty += trny;
	*pntz += trnz;
}

/***************************************************************************
* translation() - function to offset an entire instance by x, y and z      *
*                 values (i.e. a tranlation of the object)                 *
***************************************************************************/
void translation(instance &inst, float locx, float locy, float locz)
{
	float x, y, z;

	/* loop to translate the whole of an instance object */
	for (auto &vertex : inst.vert)
	{
		x = vertex.x();
		y = vertex.y();
		z = vertex.z();
		translate(&x, &y, &z, locx, locy, locz);
		vertex.x(x);
		vertex.y(y);
		vertex.z(z);
	}
}

/****************************************************************************
* set_colour() - function to set the colours of each facet of the obejct    *
*                instance                                                   *
****************************************************************************/
void set_colour(master &mast, instance &inst, int colour, float specularity)
{
	int no_edges, no_polygons, loop, offset;
	int poly_no[2], edge0, edge1;
	float x1,y1,z1, x2,y2,z2, x3,y3,z3;
	float dx1,dy1,dz1, dx2,dy2,dz2;
	float A,B,C,D, normal, hyp, adj, cos_theta, kd;

	no_edges = mast.edge0.size();
	no_polygons = mast.poly0.size();

	/* create the arrays that will hold the colour values */
	inst.edge_colour.resize(no_edges);
	inst.poly_colour.resize(no_polygons);

	/* now fill the edge array with colour values */
	for (loop = 0; loop < no_edges; loop++)
	{
		/* let's get the end and start points of the edge */
		edge0 = mast.edge0[loop];
		edge1 = mast.edge1[loop];
		/* get the first vertex of that edge */
		x1 = inst.vert[edge0].x();
		y1 = inst.vert[edge0].y();
		z1 = inst.vert[edge0].z();
		/* now get the second vertex of that edge */
		x2 = inst.vert[edge1].x();
		y2 = inst.vert[edge1].y();
		z2 = inst.vert[edge1].z();
		/* now we calculate the changes between the first and */
		/* second vertex */
		dx1 = abs(x2 - x1);
		dy1 = abs(y2 - y1);
		dz1 = abs(z2 - z1);
		/* find the angle of incidence between the ray of light */
		/* (coming straight from above) with the edge */
		hyp = sqrt((dx1*dx1)+(dy1*dy1)+(dz1*dz1));
		adj = sqrt((dx1*dx1)+(dz1*dz1));
		if ((hyp == 0.0) || (adj == 0.0))
			cos_theta = 0.0;
		else
			cos_theta = adj / hyp;
		/* calculate the diffuse-reflection coeffiecient */
		kd = specularity / 100.0;
		/* now calculate the colour offset from the base value */
		offset = 15*kd*cos_theta;
		/* make sure the offset is within the 15 colour values */
		if (offset > 15) offset = 15;
		else if (offset < 0) offset = 0;
		/* now set the colour of the edge */
		inst.edge_colour[loop] = colour*16+(offset*0.5);
	}

	/* now fill the polygon array with colour values */
	for (loop = 0; loop < no_polygons; loop++)
	{
		/* we want to find the direction of the normal first */
		/* so we know which light intensity to apply to the polygon */
		/* get the two of the edges that build up the polygon */
		poly_no[0] = mast.poly0[loop];
		poly_no[1] = mast.poly1[loop];

		/* now let's deal with the first edge */
		edge0 = mast.edge0[poly_no[0]];
		edge1 = mast.edge1[poly_no[0]];
		/* get the first vertex of that edge */
		x1 = inst.vert[edge0].x();
		y1 = inst.vert[edge0].y();
		z1 = inst.vert[edge0].z();
		/* now get the second vertex of that edge */
		x2 = inst.vert[edge1].x();
		y2 = inst.vert[edge1].y();
		z2 = inst.vert[edge1].z();
		/* we need a third point to find the normal to the plane */
		/* so we'll get the end point of the second edge */
		edge1 = mast.edge1[(poly_no[1])];
		x3 = inst.vert[edge1].x();
		y3 = inst.vert[edge1].y();
		z3 = inst.vert[edge1].z();
		/* now we calculate the changes between the first and */
		/* second vertex */
		dx1 = x2 - x1;
		dy1 = y2 - y1;
		dz1 = z2 - z1;
		/* now changes between third and second vertex */
		dx2 = x3 - x2;
		dy2 = y3 - y2;
		dz2 = z3 - z2;
		/* at last let's calculate the normal */
		/* using the cross product */
		A = (dy1 * dz2) - (dy2 * dz1);
		B = (dz1 * dx2) - (dz2 * dx1);
		C = (dx1 * dy2) - (dx2 * dy1);
		/* now calculate D using a point that is known to be */
		/* on the plane */
		D = (A * x1) + (B * y1) + (C * z1);

		/* now we want to find whether the surface is facing upwards or is */
		/* facing downwards - this way we apply a different intensity */
		/* of light source */
		/* to do this we simply add a value to the y component */
		normal = (A * x1) + (B * (y1 - 20.0)) + (C * z1) - D;

		if (normal < 0.0)
		{
			/* find the angle of incidence between the ray of light */
			/* (coming straight from below) with the surface */
			hyp = sqrt((A*A)+(B*B)+(C*C));
			/* now find the value of cos theta */
			if ((hyp == 0.0) || (B == 0.0))
				cos_theta = 0.0;
			else
				cos_theta = abs(B) / hyp;
			/* abs() is used in case the Y(B) value of the normal is negative */
			/* calculate the diffuse-reflection coeffiecient */
			kd = specularity / 100.0;
			/* now calculate the colour offset from the base value */
			offset = 15*kd*cos_theta;
			/* make sure the offset is within the 15 colour values */
			if (offset > 15) offset = 15;
			else if (offset < 0) offset = 0;
			/* this surface is facing downwards */
			inst.poly_colour[loop] = colour*16+(offset*0.25);
		}
		else if (normal >= 0.0)
		{
			/* find the angle of incidence between the ray of light */
			/* (coming straight from above) with the surface */
			hyp = sqrt((A*A)+(B*B)+(C*C));
			/* now find the value of cos theta */
			if ((hyp == 0.0) || (B == 0.0))
				cos_theta = 0.0;
			else
				cos_theta = abs(B) / hyp;
			/* abs() is used in case the Y(B) value of the normal is negative */

			/* calculate the diffuse-reflection coeffiecient */
			kd = specularity / 100.0;
			/* now calculate the colour offset from the base value */
			offset = 15*kd*cos_theta;
			/* make sure the offset is within the 15 colour values */
			if (offset > 15) offset = 15;
			else if (offset < 0) offset = 0;
			/* this surface is facing upwards */
			/* the light intensity is half */
			inst.poly_colour[loop] = colour*16+(offset*1.0);
		}
	}
}

/****************************************************************************
* set_bound() - function to set the minimum and maximum values of an object *
*                  to allow collision detection                             *
****************************************************************************/
void set_bound(instance &inst)
{
	/* set the x,y and z min/max values to the values of the first vertex
	   as an initial value */

	/* minimum value of collision box */
	float xmin = inst.vert[0].x();
	float ymin = inst.vert[0].y();
	float zmin = inst.vert[0].z();

	/* maximum value of collision box */
	float xmax = inst.vert[0].x();
	float ymax = inst.vert[0].y();
	float zmax = inst.vert[0].z();

	/* now look at other values in the instances vertex list and alter the
	   minimum and maximum values accordingly */
	for (auto vertex : inst.vert)
	{
		/* looking for the minimum x value */
		xmin = std::fmin(xmin, vertex.x());

		/* looking for the minimum y value */
		ymin = std::fmin(ymin, vertex.y());

		/* looking for the minimum z value */
		zmin = std::fmin(zmin, vertex.z());

		/* looking for the maximum x value */
		xmax = std::fmax(xmax, vertex.x());

		/* looking for the maximum y value */
		ymax = std::fmax(ymax, vertex.y());

		/* looking for the maximum z value */
		zmax = std::fmax(zmax, vertex.z());
	}

	/* now set the instance values with the minimum x,y and z and the
	   maximum x,y and z */
	/* the minimum values */
	inst.min.x(xmin);
	inst.min.y(ymin);
	inst.min.z(zmin);
	/* the maximum values */
	inst.max.x(xmax);
	inst.max.y(ymax);
	inst.max.z(zmax);
}
