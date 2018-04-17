/********************************************************
*                                                       *
* ####   ####  ###  ##   ## ##  ## ####   ####     #### *
* ## ## ##    ## ## ### ### ### ## ## ## ##       ##    *
* ####  ##    ## ## ####### ###### ## ## #####    ##    *
* ##    ##    ## ## ## # ## ## ### ## ##    ##    ##    *
* ##     ####  ###  ##   ## ##  ## ####  ####  ##  #### *
********************************************************/
/****************
* include files *
****************/
#include "string.h"
#include "alloc.h"
#include "defs.h"

/*************************
* prototype declarations *
*************************/
int process_location(float *locx, float *locy, float *locz);
int process_direction(float *angx, float *angy, float *angz);
int process_radius(float *radius);
int process_angle(float *angx, float *angy, float *angz);
int process_scale(float *angx, float *angy, float *angz);
int process_colour(int *colour);
int process_specularity(float *specularity);
char *process_outcome(void);
int process_style(int *style);
int process_sky(int *colour);
int process_ground(int *colour);

/**********************************
* external prototype declarations *
**********************************/
extern int error(char *errno, char *message, int line_no);
extern int check(char *ptr);
extern float fgetnum(void);
extern int getnum(void);
extern void getword(char *word);
extern void debug(char *string, int level);
extern void skip_garbage(void);
extern int getstring(char *word);

/*********************
* external variables *
*********************/
extern char LINE[MAXLINE];
extern int lincnt, lineptr, masterdef_processed;
extern struct master *masterptr;
extern struct instance *instanceptr;
extern struct viewer user;

/****************************************************************************
* process_location() - process the command which defines the user obJect's  *
*                      initial location                                     *
****************************************************************************/
int process_location(float *locx, float *locy, float *locz)
{
        int RESULT = OKAY;

        debug("process_location()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        *locx = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0017", "Syntax error with location definition", lincnt);

        *locy = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0017", "Syntax error with location definition", lincnt);
        
        *locz = fgetnum();

        if (check("") != BLANK)
                RESULT = error("0017", "Syntax error with location definition", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_direction() - process the command which defines the user object's *                                                                                                    **
*                       initial direction                                   *
****************************************************************************/
int process_direction(float *angx, float *angy, float *angz)
{
        int RESULT = OKAY;

        debug("process_direction", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);
        
        *angx = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0018", "Syntax error with user direction definition", lincnt);
        
        *angy = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0018", "Syntax error with user direction definition", lincnt);
        
        *angz = fgetnum();

        if (check("") != BLANK)
                RESULT = error("0018", "Syntax error with user direction definition", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_radius - process the command which defines the user object's      *
*                  radius                                                   *
****************************************************************************/
int process_radius(float *radius)
{
        int RESULT = OKAY;

        debug("process_radius()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);
        
        *radius = fgetnum();

        if (check("") != BLANK)
                RESULT = error("0019", "Syntax error with user radius definition", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_angle() - process the command which defines the master object's   *
*                   initial angle                                           *
****************************************************************************/
int process_angle(float *angx, float *angy, float *angz)
(
        int RESULT = OKAY;

        debug("process_angle()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        *angx = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0023", "Syntax error with angle command", lincnt);

        *angy = fgetnum();

        if (check(", ") != OKAY)
                RESULT = error("0023", "Syntax error with angle command", lincnt);

        *angz = fgetnum();

        if (check("") != BLANK)
                RESULT = error("0023", "Syntax error with angle command", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_scale() - process the command which defines the master object's   *
*                   initial scale                                           *
****************************************************************************/
int process scale(float *sclx, float *scly, float *sclz)
{
        int RESULT = OKAY;

        debug("process_scale()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        *sclx = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0024", "Syntax error with scale command", lincnt);

        *scly = fgetnum();

        if (check(",") != OKAY)
                RESULT = error("0024", "Syntax error with scale command", lincnt);

        *sclz = fgetnum();

        if (check("") != BLANK)
                RESULT = error("0024", "Syntax error with scale command", lincnt);

        return (RESULT);
}

/****************************************************************************
* process sky - process the command which defines the colour of the sky     *
****************************************************************************/
int process_sky(int *colour)
{
        int RESULT = OKAY;

        debug("process_sky()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ((*colour = getnum()) == ERROR)
                RESULT = error("0054", "Syntax error with sky colour definition", lincnt);

        if (check("") != BLANK)
                RESULT = error("0054", "Syntax error with sky colour definition", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_ground - process the command which defines the colour of the      *
*                  ground                                                   *
****************************************************************************/
int process_ground(int *colour)
{
        int RESULT = OKAY;

        debug("process_ground()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ((*colour = getnum()) == ERROR)
                RESULT = error("0055", "Syntax error with ground colour definition", lincnt);

        if (check("") != BLANK)
                RESULT = error("0055", "Syntax error with ground colour definition", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_colour() - process the command which defines the instance         *
*                    object' s colour                                       *
****************************************************************************/
int process_colour (int *colour)
{
        int RESULT = OKAY;
        char word[MAXLINE];

        debug("process_colour()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        getword(word);

        /* get the colour value from the colour command */
        if ((strcmpi(word, "black") == EQUAL) || (strcmp(word, "0") == EQUAL))
                *colour = 0;
        else if ((strcmpi(word, "blue") == EQUAL) || (strcmp(word, "1") == EQUAL))
                *colour = 1;
        else if ((strcmpi(word, "green") == EQUAL) || (strcmp(word, "2") == EQUAL))
                *colour = 2;
        else if ((strcmpi(word, "cyan") == EQUAL) || (strcmp(word, "3") == EQUAL))
                *colour = 3;
        else if ((strcmpi(word, "red") == EQUAL) || (strcmp(word, "4") == EQUAL))
                *colour = 4;
        else if ((strcmpi(word, "magenta") == EQUAL) || (strcmp(word, "5") == EQUAL))
                *colour = 5;
        else if ((strcmpi(word, "brown") == EQUAL) || (strcmp(word, "6") == EQUAL))
                *colour = 6;
        else if ((strcmpi(word, "lightgrey") == EQUAL) || (strcmp(word, "7") == EQUAL))
                *colour = 7;
        else if ((strcmpi(word, "darkgrey") == EQUAL) || (strcmp(word, "8") == EQUAL))
                *colour = 8;
        else if ((strcmpi(word, "lightblue") == EQUAL) || (strcmp(word, "9") == EQUAL))
                *colour = 9;
        else if ((strcmpi(word, "lightgreen") == EQUAL) || (strcmp(word, "10") == EQUAL))
                *colour = 10;
        else if ((strcmpi(word, "lightcyan") == EQUAL) || (strcmp(word, "11") == EQUAL))
                *colour = 11;
        else if ((strcmpi(word, "lightred") == EQUAL) || (strcmp(word, "12") == EQUAL))
                *colour = 12;
        else if ((strcmpi(word, "lightmagenta") == EQUAL) || (strcmp(word, "13") == EQUAL))
                *colour = 13;
        else if ((strcmpi(word, "yellow") == EQUAL) || (strcmp(word, "14") == EQUAL))
                *colour = 14;
        else if ((strcmpi(word, "white") == EQUAL) || (strcmp(word, "15") == EQUAL))
                *colour = 15;
        else
                RESULT = error("0044", "Invalid colour value", lincnt);

        if (check("") != BLANK)
                RESULT = error("0033", "Syntax error with colour command", lincnt);

        if ((*colour < 0) || (*colour > 15))
                RESULT = error("0044", "Invalid colour value", lincnt);

        return (RESULT);
}

/****************************************************************************
* process specularity() - process the command which defines the instance    *
*                         object' s specularity                             *
****************************************************************************/
int process_specularity(float *specularity)
{
        int RESULT = OKAY, rest_of_line;

        debug("process_specularity()", 1);

        /* make sure that there is an equals sign */
        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        *specularity = fgetnum();

        /* check to see if there is a percent sign at the end of the line */
        rest_of_line = check("%");

        if ((rest_of_line != BLANK) && (rest_of_line != OKAY))
                RESULT = error("0034", "Syntax error with specularity command", lincnt);

        /* make sure the specularity value is a percentage */
        if ((*specularity < 0.0) || (*specularity > 100.0))
                RESULT = error("0035", "Invalid specularity number", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_outcome() - process the command which defines the instance        *
*                     object's outcome string                               *
*                     this can either make an object solid or not           *
*                     there is also the possibility to execute DOS commands *
*                     with this string                                      *
****************************************************************************/
char *process_outcome(void)
{
        char word[MAXLINE];
        char *string_ptr;

        debug("process_outcome()", 1);

        /* make sure that there is an equals sign */
        if (check("=") != OKAY)
        {
                error("0006", "Missing assignment symbol", lincnt);
                return (NULL);
        }

        /* if the string is in error then return error */
        if (getstring(word) == ERROR)
        {
                error("0053", "Syntax error with outcome string", lincnt);
                return (NULL);
        }

        /* so we have a valid string we must now copy the string into
           some allocated memory - i.e. place it within the instance
           data structure */
        string_ptr = (char *)malloc(sizeof(char) * strlen(word));

        /* check to make sure that we allocated the memory okay */
        if (string_ptr == NULL)
        {
                error("0043", "Cannot allocate memory", lincnt);
                return (NULL);
        }
        /* copy array over to malloced memory */
        strcpy(string_ptr, word);

        return (string_ptr);
}

/****************************************************************************
* process_style() - process the command which defines the instance object's *
*                   style                                                   *
****************************************************************************/
int process_style(int *style)
{
        int RESULT = OKAY;
        char word[MAXLINE];

        debug("process_style()", 1);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        getword(word);

        if (strcmpi(word, "SOLID") == EQUAL)
                *style = SOLID;
        else if (strcmpi(word, "WIREFRAME") == EQUAL)
                *style = WFRAME;
        else
                RESULT = error("0036", "Unknown style type", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_vertices() - process the object's vertices                        *
****************************************************************************/
int process_verts(int no_vertices, int object_no)
{
        int RESULT = OKAY;
        int loop, vert_no;
        float *xvert, *yvert, *zvert;

        /* first we create the vertice data structures */
        xvert = (float *) malloc(sizeof(float) * no_vertices);
        yvert = (float *) malloc(sizeof(float) * no_vertices);
        zvert = (float *) malloc(sizeof(float) * no_vertices);

        /* now check there was no problem in memory allocation */
        if ((xvert == NULL) || (yvert == NULL) || (zvert == NULL))
                return(error("0048", "Error allocating memory", lincnt));

        for (loop = 0; loop < no_vertices; loop++)
        {
                /* first skip past any garbage! */
                skip_garbage();

                /* get the number of the vertex to set */
                if ((vert_no = getnum()) == ERROR)
                        RESULT = error("0047", "Syntax error with vertex command", lincnt);

                if ((vert_no < 1) || (vert_no > no_vertices))
                        RESULT = error("0047", "Syntax error with vertex command", lincnt);

                if (check("=") != OKAY)
                        RESULT = error("0006", "Missing assignment symbol", lincnt);

                /* now we get the x, y and z values of the vertex */
                xvert[vert_no-1] = fgetnum();

                if (check(",") != OKAY)
                        RESULT = error("0047", "Syntax error with vertex command", lincnt);

                yvert[vert_no-1] = fgetnum();

                if (check(",") != OKAY)
                        RESULT = error("0047", "Syntax error with vertex command", lincnt);

                zvert[vert_no-1] = fgetnum();

                /* make sure there is no more text on the end of the line */
                if (check("") != BLANK)
                        RESULT = error("0047", "Syntax error with vertex command", lincnt);
        }

        /* set the memory holding the vertices into the master structure */
        masterptr[object_no].xvert = xvert;
        masterptr[object_no].yvert = yvert;
        masterptr[object_no].zvert = zvert;

        return (RESULT);
}

/****************************************************************************
* process_edges() - process the object's edges                              *
****************************************************************************/
int process_edges(int no_edges, int object_no)
{
        int RESULT = OKAY;
        int loop, edge_no, tmp;
        int *edge0, *edge1;

        /* first we create the edge data structures */
        edge0 = (int *) malloc(sizeof(int) * no_edges);
        edge1 = (int *) malloc(sizeof(int) * no_edges);
        /* now check there was no problem in memory allocation */
        if ((edge0 == NULL) || (edge1 == NULL))
                return(error("0048", "Error allocating memory", lincnt));

        for (loop = 0; loop < no_edges; loop++)
        {
                /* first skip past any garbage! */
                skip_garbage();

                /* get the number of the vertex to set */
                if ((edge_no = getnum()) == ERROR)
                        RESULT = error("0049", "Syntax error with edge command", lincnt);

                if ((edge_no < 1) || (edge_no > no_edges))
                        RESULT = error("0049", "Syntax error with edge command", lincnt);

                if (check("=") != OKAY)
                        RESULT = error("0006", "Missing assignment symbol", lincnt);

                /* now we get the start and end vertices of the edge */
                tmp = getnum();

                /* now make sure that it is a valid vertex reference */
                if ((tmp < 1) || (tmp > masterptr[object_no].no_vertices))
                        RESULT = error("0050", "Illegal edge value", lincnt);

                /* remember to take one from values to match array structure */
                edge0[edge_no-1] = tmp - 1;

                if (check(",") != OKAY)
                        RESULT = error("0049", "Syntax error with edge command", lincnt);

                tmp = getnum();

                /* now make sure that it is a valid vertex reference */
                if ((tmp < 1) || (tmp > masterptr[object_no].no_vertices))
                        RESULT = error("0050", "Illegal edge value", lincnt);

                /* remember to take one from values to match array structure */
                edge1[edge_no-1] = tmp - 1;

                /* make sure there is no more text on the end of the line */
                if (check("") != BLANK)
                        RESULT = error("0049", "Syntax error with edge command", lincnt);
        }

        /* set the memory holding the vertices into the master structure */
        masterptr[object_no].edge0 = edge0;
        masterptr[object_no].edge1 = edge1;

        return (RESULT);
}

/****************************************************************************
* process_polygons() - process the object's polygons                        *
****************************************************************************/
int process_polys(int no_polygons, int object_no)
{
        int RESULT = OKAY;
        int loop, poly_no, tmp;
        int *poly0, *poly1, *poly2;

        /* first we create the polygon data structures */
        poly0 = (int *) malloc(sizeof(int) * no_polygons);
        poly1 = (int *) malloo(sizeof(int) * no_polygons);
        poly2 = (int *) malloc(sizeof(int) * no_polygons);

        /* now check there was no problem in memory allocation */
        if ((poly0 == NULL) || (poly1 == NULL) || (poly2 == NULL))
                return (error("0048", "Error allocating memory", lincnt));

        for (loop = 0; loop < no_polygons; loop++)
        {
                /* first skip past any garbage! */
                skip_garbage();

                /* get the number of the polygon to set */
                if ((poly_no = getnum()) == ERROR)
                        RESULT = error("0051", "Syntax error with polygon command", lincnt);

                if ((poly_no < 1) || (poly_no > no_polygons))
                        RESULT = error("0051", "Syntax error with polygon command", lincnt);

                if (check("=") != OKAY)
                        RESULT = error("0006", "MAssing assignment symbol", lincnt);

                /* now we get the three edges that make up the polygon */
                tmp = getnum();

                /* now make sure that it is a valid edge reference */
                if ((tmp < 1) || (tmp > masterptr[object_no].no_edges))
                        RESULT = error("0052", "Illegal polygon value", lincnt);

                /* remember to take one from values to match array structure */
                poly0[poly_no-1] = tmp - 1;

                if (check(",") != OKAY)
                        RESULT = error("0049", "Syntax error with edge command", lincnt);

                /* next edge reference */
                tmp = getnum();

                /* now make sure that it is a valid edge reference */
                if ((tmp < 1) || (tmp > masterptr[object_no].no_edges))
                        RESULT = error("0052", "Illegal polygon value", lincnt);

                /* remember to take one from values to match array structure */
                poly1[poly_no-1] = tmp - 1;

                if (check(",") != OKAY)
                        RESULT = error("0049", "Syntax error with edge command", lincnt);

                /* final edge reference */
                tmp = getnum();

                /* now make sure that it is a valid edge reference */
                if ((tmp < 1) || (tmp > masterptr[object_no].no_edges))
                        RESULT = error("0052", "Illegal polygon value", lincnt);

                /* remember to take one from values to match array structure */
                poly2[poly_no-1] = tmp - 1;

                /* make sure there is no more text on the end of the line */
                if (check("") != BLANK)
                        RESULT = error("0049", "Syntax error with edge command", lincnt);
        }

        /* set the memory holding the vertices into the master structure */
        masterptr[object no].poly0 = poly0;
        masterptr[object_no].poly1 = poly1;
        masterptr[object_no].poly2 = poly2;

        return(RESULT);
}
