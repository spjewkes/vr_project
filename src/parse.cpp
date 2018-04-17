/*****************************************
* ####   ###  ####   #### #####     #### *
* ## ## ## ## ## ## ##    ##       ##    *
* ####  ##### ####  ##### ####     ##    *
* ##    ## ## ## ##    ## ##       ##    *
* ##    ## ## ## ## ####  ##### ##  #### *
*****************************************/
/****************
* include files *
****************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

/*************************
* prototype declarations *
*************************/
int parse(char *filename);
int process(void);
int process_master(void);
int process_instance(void);
int process_user(void);
int process_objects(int no_objects);
int check_object_values(int object_no, int object_pos, int no_objects);
int process_obsect_definition(int object_no);
int process_object_instances(int no_instances, int no_objects);
int check_instance_values(int *col_set, int *spec_set, int *style_set,
                        int instance_pos, int no_instances, int master_no);

/**********************************
* external prototype declarations *
**********************************/
extern int process_location(float *locx, float *locy, float *locz);
extern int process_direction(float *angx, float *angy, float *angz);
extern int process_radius(float *radius);
extern int process_angle(float *angx, float *angy, float *angz);
extern int process_scale(float *sclx, float *scly, float *sclz);
extern int process_colour(int *colour);
extern int process_specularity(float *specularity);
extern char *process_outcome(void);
extern int process_style(int *style);
extern int getline(void);
extern void skip_garbage(void);
extern int check(char *ptr);
extern void getword(char *word);
extern int getnum(void);
extern float fgetnum(void);
extern int get_point(float *pntx, float *pnty, float *pntz);
extern int error(char *errno, char *message, int line no);
extern void warn(char *warnno, char *message, int line_no);
extern int init_master(int no objects);
extern int init_instance(int no_instances);
extern void init_user(void);
extern void debug(char *string, int level);
extern void rotate(float *pntx, float *pnty, float *pntz,
                float angx, float angy, float angz);
extern void scale(float *pntx, float *pnty, float *pntz,
                float sclx, float scly, float sclz);
extern void translation(struct instance *instanceptr, int instance_no,
                float locx, float 1ocy, float locz);
extern int set_colour(struct master *masterptr, struct instance *instanceptr,
                int instance_no, int master_no, int colour,
                float specularity);
extern int process_verts(int no_vertices, int object_no);
extern int process_edges(int no_edges, int object_no);
extern int process_polys(int no_polygons, int object_no);
extern void set_bound(struct instance *instanceptr, int instance_no);
extern int create_object_instance(struct master *masterptr,
                                struct instance *instanceptr,
                                int instance_no, int master_no);
extern int process_sky(int *colour);
extern int process_ground(int *colour)

/*********************
* external variables *
*********************/

extern char LINE[];
extern int lincnt, lineptr, masterdef_processed;
extern int no_masters, no_instances;
extern struct master *masterptr;
extern struct instance *instanceptr;
extern struct viewer user;
extern void *fp;

/****************************************************************************
* parse() - main function to initiate the parsing of a text file for the    *
*           virtual reality program.                                        *
*           Returns a pointer to a datastructure that contains the virtual  *
*           world to be manipulated.                                        *
****************************************************************************/
int parse(char *filename)
{
        int retcode;
        debug("parse()", 1);

        /* lincnt stores currently processed line */
        lincnt = 0;
        /* for telling whether the master objects have been defined yet */
        masterdef_processed = FALSE;

        /* set the number of masters to zero */
        no_masters = 0;
        /* set the number of instances to zero */
        no_instances = 0;
        /* initialise the pointers */
        masterptr = NULL;
        instanceptr = NULL;
        if (((FILE *)fp = fopen(filename, "r")) == NULL)
        {
                /* error if can't open file */
                error("0001", "Error opening file", lincnt);
                return (ERROR);
        }
        else
        {
                /* get the first line of the file */
                retcode = getline();
                /* getting lines from file until EOF */
                while (retcode != EOF)
                {
                        if ((retcode != COMMENT) && (retcode != BLANK))
                                if (process () == ERROR)
                                {
                                        /* if error then stop parsing file */
                                        error("0002", "Cannot parse file", lincnt);
                                        return (ERROR);
                                }
                        /* get next line */
                        retcode = getline();
                }
        }
        /* close the open file */
        fclose((FILE *)fp) ;
        return (OKAY);
}

/****************************************************************************
* process() - function to process any commands that may occur in the file   *
*             at this level it searches for one of three headers:           *
*             ".masterdefs", ".instance defs" or ".userdefs"                *
*             any other strings will-result in a syntax error               *
****************************************************************************/
int process(void)
{
        char word[MAXLINE];

        debug("process()", 1);

        /* get the first word in LINE */
        getword(word);

        if (strcmp(word, ".masterdefs") == EQUAL)
                /* a master block */
                return (process_master());
        else if (strcmp(word, ".instancedefs") == EQUAL)
                /* an instance block */
                return (process_instance());
        else if (strcmp(word, ".userdefs") == EQUAL)
                /* a user block */
                return (process_user());
        else
        {
                error("0003", "Syntax error", lincnt);
                /* unknown block! */
                return (ERROR);
        }
}

/****************************************************************************
* process_master() - process a text block which contains definitions for    *
*                    the master objects                                     *
****************************************************************************/
int process_master(void)
{
        int RESULT = OKAY;
        char word[MAXLINE];

        debug("process_master()", 1);

        /* error if master objects have already been defined */
        if (masterptr != NULL)
                RESULT = error("0004", "Master objects have already been defined", lincnt);
        skip_garbage();

        if (check("no_objects") != OKAY)
                RESULT = error("0005", "The no_objects definition is missing", lincnt);
        masterdef_processed = TRUE;

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ((no_masters = getnum()) == ERROR)
                RESULT = error("0007", "Cannot parse number", lincnt);

        if (check("") != BLANK)
                RESULT = error("0041", "Syntax error with no_objects command", lincnt);

        if (no_masters > 0)
        {
                if (init_master(no_masters) == ERROR)
                        return (ERROR);
                skip_garbage();
                /* process the master objects */
                if (process_objects(no_masters) != OKAY)
                        RESULT = error("0008", "Error with master object definitions", lincnt);
        }
        else if (no_masters < 0)
                RESULT = error("0009", "The no_objects is less than zero", lincnt);
        else if (no_masters == 0)
        {
                warn("0001", "There are no master objects", lincnt);

                skip_garbage();

                getword(word);

                /* check that the master definition block is ended */
                if (strcmp(word, ".end_masterdefs") == EQUAL)
                        RESULT = OKAY;
                else
                        RESULT = error("0008", "Error with master object definitions", lincnt);
        }

        /* return the result of parsing the master block */
        return (RESULT);
}

/****************************************************************************
* process_instance() - process a text block which contains definitions for  *
*                      the instance objects                                 *
****************************************************************************/
int process_instance(void)
{
        int RESULT = OKAY;
        char word[MAXLINE];

        debug("process_instance()", 1);

        /* error if the master objects haven't been defined */
        if (masterdef_processed == FALSE)
                RESULT = error("0011", "Master objects have not yet been defined", lincnt);

        skip_garbage();

        if (check("no_instances") != OKAY)
                RESULT = error("0012", "The no_instances definition is missing", lincnt);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ((no_instances = getnum()) == ERROR)
                RESULT = error("0007", "Cannot parse number", lincnt);

        if (check("") != BLANK)
                RESULT = error("0042", "Syntax error with no_instances command", lincnt);

        if (no_instances > 0)
        {
                if (init_instance(no_instances) == ERROR)
                        return (ERROR);
                skip_garbage();
                /* process the instance objects */
                if (process_object_instances(no_instances, no_masters) != OKAY)
                        RESULT = error("0013", "Error with instance object definitions", lincnt);
        }
        else if (no instances < 0)
                RESULT = error("0014", "The no instances is less than zero", lincnt);
        else if (no_instances == 0)
        {
                warn("0002", "There are no instance objects", lincnt);
                skip_garbage();

                getword(word);

                /* make sure that the instance definition block is correctly ended */
                if (strcmp(word, ".end_instancedefs") == EQUAL)
                        RESULT = OKAY;
                else
                        RESULT = error("0042", "Syntax error with no_instances command", lincnt);
        }

        /* return the result of parsing the instance block */
        return (RESULT);
}

/****************************************************************************
* process_user() - process a text block which contains definitions for the  *
*                  user object                                              *
****************************************************************************/
int process_user(void)
{
        char word[MAXLINE];

        debug("process_user()", 1);

        /* error if the master objects have already been defined */
        if (masterdef_processed == TRUE)
                return (error("0004", "Master objects have already been defined", lincnt));

        init_user();

        for EVER
        {
                skip_garbage();

        getword(word);

        if (strcmp(word, "location") == EQUAL)
        {
                /* location command */
                if (process_location(&user.locx, &user.locy, &user.locz) == ERROR)
                        return (ERROR);
        }
        else if (strcmp(word, "direction") == EQUAL)
        {
                /* direction command */
                if (process_direction(&user.angx, &user.angy, &user.angz) == ERROR)
                        return (ERROR);
        }
        else if (strcmp (word, "radius") == EQUAL)
        {
                /* radius command */
                if (process_radius(&user.radius) == ERROR)
                        return (ERROR);
        )
        else if (strcmp(word, "sky") == EQUAL)
        {
                /* sky command */
                if (process_sky(&user.sky) == ERROR)
                        return (ERROR);
        }
        else if (strcmp(word, "ground") == EQUAL)
        {
                /* ground command */
                if (process_ground(&user.ground) == ERROR)
                        return (ERROR);
        }
        else if (strcmp(word, ".end_userdefs") == EQUAL)
        {
                /* end of user block */
                return (OKAY);
        )
        else
                return(error("0016", "Syntax error in block .userdefs", lincnt));
        )
}

/****************************************************************************
* process_objects() - this function will parse through all the master       *
*                     definitions and create the master data structure      *
*                     this structure is pointed to by 'master array'        *
****************************************************************************/
int process_objects(int no_objects)
{
        int loop, master_no;
        int RESULT = OKAY;

        debug("process_objects()",1);

        for (loop = 0; loop < no_objects; loop++)
        {
                if (check("master_no") != OKAY)
                        RESULT = error("0020", "Error with master no command", lincnt);

                if (check("=") != OKAY)
                        RESULT = error("0006", "Missing assignment symbol", lincnt);

                if ((master_no = getnum()) == ERROR)
                        RESULT = error("0007", "Cannot parse number", lincnt);

                if ((master no < 0) || (master_no > no_objects))
                        RESULT = error("0021", "The-master no is incorrect", lincnt);

                skip_garbage();

                /* get the vertices, edges and polygons which define the object */
                if (process_object_definition(master_no-1) != OKAY)
                        RESULT = ERROR;

                skip_garbage();

                /* get any scale and angle commands */
                if (check_object_values(master_no-1, loop, no_objects) != OKAY)
                        RESULT = ERROR;
        }

        return (RESULT);
}

/****************************************************************************
* check_object_values() - process the optional angle and scale commands for *
*                         the master object and see that it's definition is *
*                         correctly terminated                              *
****************************************************************************/
int check_object_values(int object_no, int object_pos, int no_objects)
{
        char word[MAXLINE];

        float angx, angy, angz;
        float sclx, scly, sclz;

        debug("check_object_values()",l);

        for EVER
        {
                getword(word);

                if (strcmp(word, "angle") == EQUAL)
                {
                        /* get object angle values from script */
                        if (process_angle(&angx, &angy, &angz) == ERROR)
                                return(ERROR);
                        /* load master structure with angle values */
                        masterptr[object_no].anglex = angx;
                        masterptr[object_no].angley = angy;
                        masterptr[object_no].anglez = angz;
                }
                else if (strcmp(word, "scale") == EQUAL)
                {
                        /* get object scale values from script */
                        if (process_scale(&sclx, &scly, &sclz) == ERROR)
                                return(ERROR);

                        /* fill master structure with scale values */
                        masterptr[object_no].scalex = sclx;
                        masterptr[object_no].scaley = scly;
                        masterptr[object_no].scalez = sclz;
                }
                else if (strcmp(word, "master_no") == EQUAL)
                {
                        /* make sure that we need to process more master objects */
                        if ((object_pos+1) < no_objects)
                        {
                                lineptr = 0;
                                return (OKAY);
                        )
                        else
                                return(error("0022", "Too many master no definitions", lincnt));
                }
                else if (strcmp (word, ".end_masterdefs") == EQUAL)
                {
                        /* make sure that we have no more master objects to process */
                        if ((object_pos+1) == no_objects)
                                return (OKAY);
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
int process_object_definition(int object_no)
{
        int RESULT = OKAY;
        int no_vert, no_edge, no_poly;

        debug("process_polygons()", 1);

        /* make sure that object block starts correctly */
        if (check(".objectdef") != OKAY)
                RESULT = error("0027", "Error with block .objectdef", lincnt);

        skip_garbage () ;

        /* now retrieve the number of vertices that make up the object */
        if (check("no_vertices") != OKAY)
                RESULT = error("0045", "Error with no_vertices command", lincnt);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ((no vert = getnum()) == ERROR)
                RESULT = error("0007", "Cannot parse number", lincnt);

        if (no_vert > 0)
        {
                /* set the no_vertices value in the master structure */
                masterptr[object_no].no_vertices = no_vert;
                /* get the values of the vertices */
                RESULT = process_verts(no_vert, object_no);
        }
        else if (no_vert < 0)
        {
                /* generate an error if the no_vertices value is illegal */
                RESULT = error("0045", "Error with the no vertices command", lincnt);
        }

        skip_garbage();

        /* now retrieve the number of edges that make up the object */
        if (check("no edges") != OKAY)
                RESULT = error("0046", "Error with no_edges command", lincnt);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ((no_edge = getnum() ) == ERROR)
                RESULT = error("0007", "Cannot parse number", lincnt);

        if (no_edge > 0)
        {
                /* set the no_edges value in the master structure */
                masterptr[object_no].no_edges = no_edge;
                /* get the values of the edges */
                RESULT = process_edges(no_edge, object_no);
        }
        else if (no edge < 0)
        {
                /* generate an error if the no_edges value is illegal */
                RESULT = error("0046", "Error with the no_edges command", lincnt);
        }

        skip_garbage();

        /* now retrieve the number of polygons that make up the object */
        if (check("no_polygons") != OKAY)
                RESULT = error("0025", "Error with the no_polygons command", lincnt);

        if (check("=") != OKAY)
                RESULT = error("0006", "Missing assignment symbol", lincnt);

        if ( (no_poly = getnum() ) == ERROR)
                RESULT = error("0007", "Cannot parse number", lincnt);

        if (no_poly > 0)
        {
                /* set the no_polygons value in the master structure */
                masterptr[object_no].no_polygons = no_poly;
                /* get the values of the polygons */
                RESULT = process_polys(no_poly, object_no);
        }
        else if (no_poly < 0)
        {
                /* generate an error if the no polygons value is illegal */
                RESULT = error("0025", "Error with the no_polygons command", lincnt);
        }

        skip_garbage();

        /* check that the object definition block is properly terminated */
        if (check(".objectend") != OKAY)
                RESULT = error("0028", "Error terminating object definition", lincnt);

        return (RESULT);
}

/****************************************************************************
* process_object_instances () - function to process the list of instances   *
*                               of master objects described                 *
****************************************************************************/
int process_object_instances(int no_instances, int no_objects)
(
        int loop, master_no;
        int RESULT = OKAY;
        int col_set, spec_set, style_set;

        debug("process_object_instances()", 1);

        for (loop = 0; loop < no_instances; loop++)
        {
                col_set = FALSE;
                spec_set = FALSE;
                style_set = FALSE;

                /* check for the master no command */
                if (check("master_no") != OKAY)
                        RESULT = error("0021", "The master no is incorrect", lincnt);

                if (check("=") != OKAY)
                        RESULT = error("0006", "Missing assignment symbol", lincnt);

                if ((master_no = getnum()) == ERROR)
                        RESULT = error("0007", "Cannot parse number", lincnt);

                if ((master_no < 0) || (master_no > no_objects))
                        RESULT = error("0021", "The master_no is incorrect", lincnt);

                /* store the master no value in the instance */
                /* take one away to-match internal format of master object references */
                instanceptr[loop].master_no = master_no - 1;

                skip_garbage();

                /* now get any other commands for that instance */
                /* take one away from master no because that's the way they are */
                /* stored in the master array */
                if (check_instance_values(&col_set, &spec_set, &style_set,
                                        loop, no_instances, master_no-1) != OKAY)
                        RESULT = ERROR;
                /* make sure that colour, specularity & style have been specified */
                if ((col_set == FALSE) ||
                    (spec_set == FALSE) ||
                    (style_set == FALSE))
                        RESULT = error("0030", "Either colour, specularity or style has not been set", lincnt);
        }

        return (RESULT);
}

/****************************************************************************
* check_instance_values() - process the optional and required commands for  *
*                                  an instance of an object                 *
****************************************************************************/
int check_instance_values(int *col_set, int *spec_set, int *style_set, int instance_pos, int no_instances, int master_no)
{
        char word[MAXLINE] ;
        float locx, locy, locz;
        float angx, angy, angz;
        float sclx, scly, sclz;
        float specularity;
        int colour, style;
        char *string_ptr;

        /* set the angles, scales and locations defaults */
        locx = 0.0; locy = 0.0; locz = 0.0;
        angx = 0.0; angy = 0.0; angz = 0.0;
        sclx = 1.0; scly = 1.0; sclz = 1.0;

        debug("check_instance_values()", 1);

        for EVER
        {
                getword(word);

                if (strcmp(word, "location") == EQUAL)
                {
                        /* get the instance location from the file */
                        if (process_location(&locx, &locy, &locz) == ERROR)
                                return (ERROR);
                        /* fill instance location values into the instance structure */
                        instanceptr[instance_pos].posx = locx;
                        instanceptr[instance_pos].posy = locy;
                        instanceptr[instance_pos].posz = locz;
                }
                else if (strcmp(word, "angle") == EQUAL)
                {
                        if (process_angle(&angx, &angy, &angz) == ERROR)
                                return (ERROR);
                        instanceptr[instance_pos].anglex = angx;
                        instanceptr[instance_pos].angley = angy;
                        instanceptr[instance_pos].anglez = angz;
                }
                else if (strcmp(word, "scale") == EQUAL)
                {
                        if (process_scale(&sclx, &scly, &sclz) == ERROR)
                                return (ERROR);
                        instanceptr[instance_pos].scalex = sclx;
                        instanceptr[instance_pos].scaley = scly;
                        instanceptr[instance_pos].scalez = sclz;
                }
                else if (strcmp(word, "colour") == EQUAL)
                {
                        /* process the colour value */
                        if (process_colour(&colour) == ERROR)
                                return (ERROR);
                        *col_set = TRUE;
                }
                else if (strcmp(word, "specularity") == EQUAL)
                {
                        /* process the specularity value */
                        if (process_specularity(&specularity) == ERROR)
                                return (ERROR);
                        *spec_set = TRUE;
                }
                else if (strcmp(word, "style") == EQUAL)
                {
                        /* get the instance style from the file */
                        if (process_style(&style) == ERROR)
                                return (ERROR);
                        *style_set = TRUE;
                        /* fill the instance style value into the instance structure */
                        instanceptr[instance_pos].style = style;
                }
                else if (strcmp (word, "outcome") == EQUAL)
                {
                        if ((string_ptr = process_outcome()) == NULL)
                                return (ERROR);
                        instanceptr[instance_pos].outcome = string_ptr;
                        /* now we should check whether we want
                           the object to be solid or not */
                        if (strcmpi(string_ptr, "") == EQUAL)
                                instanceptr[instance_pos].solid = FALSE;
                        else if (strcmpi(string_ptr, "ignore") == EQUAL)
                                instanceptr[instance_pos].solid = FALSE;
                        else if (strcmpi(string_ptr, "solid") == EQUAL)
                                instanceptr[instance_pos].solid = TRUE;
                        else
                                instanceptr[instance_pos].solid = TRUE;
                }
                else if (strcmp(word, "master_no") == EQUAL)
                {
                        if ((instance_pos+1) < no_instances)
                        {
                                lineptr = 0;
                                /* set up the instance of the master object */
                                if (create_object_instance(masterptr, instanceptr, instance_pos, master_no) == ERROR)
                                        return (ERROR);
                                /* translate the instance */
                                translation(instanceptr, instance_pos, locx, locy, locz);
                                /* set the colours of the instances facets (polygons) */
                                if (set_colour(masterptr, instanceptr, instance_pos, master_no, colour, specularity) == ERROR)
                                        return (ERROR);
                                /* finially we set up the collision box around the object */
                                set_bound(instanceptr, instance_pos);
                                return (OKAY);
                        }
                        else
                                return (error("0031", "Too many instance definitions", lincnt));
                }
                else if (strcmp (word, ".end_instancedefs") == EQUAL)
                {
                        if ((instance pos+1) == no_instances)
                        {
                                /* set up the instance of the master object */
                                if (create_object_instance(masterptr, instanceptr, instance_pos, master_no) == ERROR)
                                        return (ERROR);
                                /* translate the instance */
                                translation(instanceptr, instance_pos, locx, locy, locz);
                                /* set the colours if the instances facets */
                                if (set_colour(masterptr, instanceptr, instance_pos, master_no, colour, specularity) == ERROR)
                                        return (ERROR);
                                /* finially we set up the collision box around the object */
                                set_bound(instanceptr, instance_pos);
                                return (OKAY);
                        }
                        else
                                return(error("0032", "Not all instances have been defined", lincnt));
                }
                else
                        return (error ("0040", "Syntax error in block .instancedefs", lincnt));

                skip_garbage();
        }
}
