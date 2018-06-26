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

