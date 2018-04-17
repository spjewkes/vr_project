/************************************************
* ####  ##### ## ## ##  ##  ####  ####     #### *
* ## ## ##    ## ## ### ## ##    ##       ##    *
* ####  ####  ## ## ###### ##    #####    ##    *
* ##    ##    ## ## ## ### ##       ##    ##    *
* ##    ##     ###  ##  ##  #### ####  ##  #### *
************************************************/
/****************
* include files *
****************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"

/*************************
* prototype declarations *
*************************/
int getline(void);
void skip_garbage(void);
int check(char *ptr);
void getword(char *word);
int getnum(void);
float fgetnum(void);
int get_point(float *pntx, float *pnty, float *pntz);

/*********************************************
* forward declaration of external prototypes *
*********************************************/
extern int error(char *errno, char *message, int line_no);
extern void debug(char *string, int level);

/*********************
* external variables *
*********************/
extern char LINE[];
extern int lincnt, lineptr, masterdef_processed;
extern void *fp;
extern struct master *masterptr;
extern struct instance *instanceptr;

/****************************************************************************
* getline() - function takes the next line from the file pointed to by '*fp'*
*             and stores it in the char array 'LINE'                        *
*             A return code tells if the line is a comment, blank,          *
*             end-of-file or something else.                                *
*             returns: EOF if the file is at the end                        *
*                      COMMENT if the line is a comment                     *
*                      BLANK if the line contains nothing                   *
*                      OTHER if the line contains something other than a    *
*                      comment                                              *
****************************************************************************/
int getline(void)
{
        int retcode;

        debug("getline()", 1);

        if ((fgets(LINE, MAXLINE, (FILE *)fp)) == NULL)
                return (EOF);

        lincnt++;
        lineptr = O;
        retcode = check("#");
        lineptr = 0;

        debug(LINE, 2);

        if (retcode == OKAY)
                return (COMMENT);
        else if (retcode == BLANK)
                return (BLANK);
        else
                return (OTHER);
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

        while ((retcode == COMMENT) || (retcode == BLANK))
                retcode = getline();

        if (retcode == EOF)
        {
                error("0038", "Unexpected end of file", lincnt);
                fclose((FILE *)fp);
                exit(ERROR);
        }
}

/****************************************************************************
* check() - function to check that the next string in the array 'LINE'      *
*           matches with a string passed into the function.                 *
*           returns: OKAY if there is a match                               *
*                    ERROR is there is a string but it does not match       *
*                    BLANK if there is nothing in the LINE to match         *
****************************************************************************/
int check(char *ptr)
{
        debug("check()", 1);

        while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
                lineptr++;

        if (LINE[lineptr] == '\n')
                return (BLANK);

        while ((*ptr != '\0') && (*ptr == LINE[lineptr]))
        {
                lineptr++;
                ptr++;
        }

        if (*ptr != '\0')
                return (ERROR);
        else
                return (OKAY);
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
int getstring(char *word)
{
        char *tptr;
        int RESULT = ERROR;

        debug("getstring()", 1);

        tptr = word;

        debug("before while loop", 2);

        /* skip any spaces or tabs before the string starts */
        while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
                lineptr++;

        debug("now check for double quote mark", 2);

        /* check that there's a double quote character there */
        if (LINE[lineptr++] != '\"')
                return (RESULT);

        debug("now get the string", 2);

        /* now put the rest of the line into char array pointed to by tptr */
        while (LINE[lineptr] != '\"')
        {
                /* check to make sure that we haven't reached the end of the
                   line retrieved from the file */
                if (lineptr == '\0')
                        return (RESULT);

                /* otherwise we put the value into the char array pointed to by tptr */
                *tptr++ = LINE[lineptr++];
        }

        debug("string has been fetched", 2);

        /* terminate the char array pointed to by tptr with a null value */
        *tptr = '\0';

        RESULT = OKAY;

        return (RESULT);
}

/****************************************************************************
* getnum() - function retrieves an integer number from the char array 'LINE'*
****************************************************************************/
int getnum(void)
{
        char numline[MAXLINE];
        int tptr = O;

        debug("getnum()", 1);

        while ((LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
                lineptr++

        if (LINE[lineptr] == '\n')
                return (ERROR);

        numline[tptr++] = LINE[lineptr++];

        while (LINE[lineptr] != '\0')
        {
                if ((LINE[lineptr] == '\n') || (LINE[lineptr] == ' ') || (LINE[lineptr] == '\t'))
                        break;
                numline[tptr++] = LINE[lineptr++];
        }

        numline[tptr] == '\0';

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
int get_point(float *pntx, float *pnty, float *pntz)
{
        int RESULT = OKAY;

        debug("get_point()", 1);

        *pntx = fgetnum();

        if (check(",") != OKAY)
                RESULT = ERROR;

        *pnty = fgetnum();

        if (check(",") != OKAY)
                RESULT = ERROR;

        *pntz = fgetnum();

        if (check("") != BLANK)
                RESULT = ERROR;

        return (RESULT);
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
        pntx *= sclx;
        pnty *= scly;
        pntz *= sclz;
}

/***************************************************************************
* translate() - function to translate a single point described by an x, y  *
*               and z value by three translation values for the x, y and   *
*               z planes                                                   *
***************************************************************************/
void translate(float *pntx, float *pnty, float *pntz,
                float trnx, float trny, float trnz)
{
        pntx += trnx;
        pnty += trny;
        pntz += trnz;
}

/***************************************************************************
* translation() - function to offset an entire instance by x, y and z      *
*                 values (i.e. a tranlation of the object)                 *
***************************************************************************/
void translation(struct instance *instanceptr, int instance_no,
                float locx, float locy, float locz)
{
        int loop;
        float x, y, z;

        /* loop to translate the whole of an instance object */
        for (loop = 0; loop < instanceptr[instance_no].no_vertices; loop++)
        {
                a = instanceptr[instance_no].xvert[loop];
                y = instanceptr[instance_no].yvert[loop];
                z = instanceptr[instance_no].zvert[loop];
                translate(&x, &y, &z, locx, locy, locz);
                instanceptr[instance_no].xvert[loop] = x;
                instanceptr[instance_no].yvert[loop] = y;
                instanceptr[instance_no].zvert[loop] = z;
        }
}

/****************************************************************************
* set_colour() - function to set the colours of each facet of the obejct    *
*                instance                                                   *
****************************************************************************/
int set_colour(struct master *masterptr, struct instance *instanceptr,
                int instance_no, int master_no, int colour, float specularity)
{
        int no_edges, no_polygons, loop, offset, RESULT = OKAY;
        int poly_no[2], edge0, edge1;
        float x1,y1,z1, x2,y2,z2, x3,y3,z3;
        float dx1,dy1,dz1, dx2,dy2,dz2;
        float A,B,C,D, normal, hyp, adj, cos_theta, kd;

        no_edges = masterptr[instanceptr[instance_no].master_no].no_edges;
        no_polygons = masterptr[instanceptr[instance_no].master_no].no_polygons;

        /* create the arrays that will hold the colour values */
        instanceptr[instance_no].edge_colour = (int *) malloc(sizeof(int) * no_edges);
        instanceptr[instance_no].poly_colour = (int *) malloc(sizeof(int) * no_polygons);

        /* check that the system did actually allocate some memory */
        if ((instanceptr[instance_no].edge_colour == NULL) ||
            (instanceptr[instance_no].poly_colour == NULL))
                return(error("0048", "Error allocating memory", lincnt));

        /* now fill the edge array with colour values */
        for (loop = 0; loop < no_edges; loop++)
        {
                /* let's get the end and start points of the edge */
                edge0 = masterptr[master_no].edge0[loop];
                edge1 = masterptr[master_no].edge1[loop];
                /* get the first vertex of that edge */
                x1 = instanceptr[instance_no].xvert[edge0];
                y1 = instanceptr[instance_no].yvert[edge0];
                z1 = instanceptr[instance_no].zvert[edge0];
                /* now get the second vertex of that edge */
                x2 = instanceptr[instance_no].xvert[edge1];
                y2 = instanceptr[instance_no].yvert[edge1];
                z2 = instanceptr[instance_no].zvert[edge1];
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
                instanceptr[instance_no].edge_colour[loop] = colour*16+(offset*0.5);
        }

        /* now fill the polygon array with colour values */
        for (loop = 0; loop < no_polygons; loop++)
        {
                /* we want to find the direction of the normal first */
                /* so we know which light intensity to apply to the polygon */
                /* get the two of the edges that build up the polygon */
                polyno[0] = masterptr[master_no].poly0[loop];
                polyno[1] = masterptr[master_no].poly1[loop];

                /* now let's deal with the first edge */
                edge0 = masterptr[master_no].edge0[poly_no[0]];
                edge1 = masterptr[master_no].edge1[poly_no[0]];
                /* get the first vertex of that edge */
                x1 = instanceptr[instance_no].xvert[edgeO];
                y1 = instanceptr[instance_no].yvert[edgeO];
                z1 = instanceptr[instance_no].zvert[edgeO];
                /* now get the second vertex of that edge */
                x2 = instanceptr[instance_no].xvert[edge1];
                y2 = instanceptr[instance_no].yvert[edge1];
                z2 = instanceptr[instance_no].zvert[edge1];
                /* we need a third point to find the normal to the plane */
                /* so we'll get the end point of the second edge */
                edge1 = masterptr[master_no].edge1[(poly_no[1])];
                x3 = instanceptr[instance_no].xvert[edge1];
                y3 = instanceptr[instance_no].yvert[edge1];
                z3 = instanceptr[instance_no].zvert[edge1];
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
                        instanceptr[instance_no].poly_colour[loop] = colour*16+(offset*0.25);
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
                        instanceptr[instance_no].poly_colour[loop] = colour*16+(offset*1.0);
                }
        }
        return (RESULT);
}

/****************************************************************************
* set_bound() - function to set the minimum and maximum values of an object *
*                  to allow collision detection                             *
****************************************************************************/
void set_bound(struct instance *instanceptr, int instance_no)
{
        int no_verts, loop;
        float xmin, ymin, zmin, xmax, ymax, zmax;

        /* get the number of vertices in the object */
        no_verts = instanceptr[instance_no].no_vertices;

        /* set the x,y and z min/max values to the values of the first vertex
           as an initial value */
        /* minimum value of collision box */
        xmin = instanceptr[instance_no].xvert[0];
        ymin = instanceptr[instance_no].yvert[0];
        zmin = instanceptr[instance_no].zvert[0];
        /* maximum value of collision box */
        xmax = instanceptr[instance_no].xvert[0];
        ymax = instanceptr[instance_no].yvert[0];
        zmax = instanceptr[instance_no].zvert[0];

        /* now look at other values in the instances vertex list and alter the
           minimum and maximum values accordingly */
        for (loop = 1; loop < no_verts; loop++)
        {
                /* looking for the minimum x value */
                if (instanceptr[instance_no].xvert[loop] < xmin)
                        xmin = instanceptr[instance_no].xvert[loop];

                /* looking for the minimum y value */
                if (instanceptr[instance_no].yvert[loop] < ymin)
                        ymin = instanceptr[instance_no].yvert[loop];

                /* looking for the minimum z value */
                if (instanceptr[instance_no].zvert[loop] < zmin)
                        zmin = instanceptr[instance_no].zvert[loop];

                /* looking for the maximum x value */
                if (instanceptr[instance_no].xvert[loop] > xmax)
                        xmax = instanceptr[instance_no].xvert[loop];

                /* looking for the maximum y value */
                if (instanceptr[instance_no].yvert[loop] > ymax)
                        ymax = instanceptr[instance_no].yvert[loop];

                /* looking for the maximum z value */
                if (instanceptr[instance_no].zvert[loop] > zmax)
                        zmax = instanceptr[instance_no].zvert[loop];
        }

        /* now set the instance values with the minimum x,y and z and the
           maximum x,y and z */
        /* the minimum values */
        instanceptr[instance_no].minx = xmin;
        instanceptr[instance_no].miny = ymin;
        instanceptr[instance_no].minz = zmin;
        /* the maximum values */
        instanceptr[instance_no].maxx = xmax;
        instanceptr[instance_no].maxy = ymax;
        instanceptr[instance_no].maxz = zmax;
}
