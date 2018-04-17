/******************************************
*  #### ##### ###### ## ## ####      #### *
* ##    ##      ##   ## ## ## ##    ##    *
* ##### ####    ##   ## ## ####     ##    *
*    ## ##      ##   ## ## ##       ##    *
* ####  #####   ##    ###  ##    ##  #### *
******************************************/
/****************
* include files *
****************/
#include "defs.h"
#include <alloc.h>

/*********************************************
* forward declaration of external prototypes *
*********************************************/
extern int error(char *errno, char *message, int line_no);
extern void warn(char *warnno, char *message, int line_no);
extern void debug(char *string, int level);
extern void rotate(float *pntx, float *pnty, float *pntz, float angx, float angy, float angz);
extern void scale(float *pntx, float *pnty, float *pntz, float sclx, float scly, float sclz);

/************************************
* declaration of external variables *
************************************/
extern struct master *masterptr;
extern struct instance *instanceptr;
extern struct viewer user;
extern int lincnt;

/*****************************************************************************
* init_user() - initialises user variables                                   *
*****************************************************************************/
void init_user(void)
{
        debug("init_user()", 1);

        user.locx = 0.0;
        user.locy = 0.0;
        user.locz = 0.0;

        user.angx = 0.0;
        user.angy = 0.0;
        user.angz = 0.0;

        user.radius = 1.0;

        user.sky = 0;
        user.ground = 0;
}

/*****************************************************************************
* init_master() - initialises the array which will store the master objects  *
*****************************************************************************/
int init_master(int no_objects)
{
        int loop;

        debug("init_master()", 1);

        /* create the space required for the master objects */
        masterptr = (struct master *) malloc(sizeof(struct master) *  no_objects);

        if (masterptr == NULL)
                return(error("O043", "Cannot allocate memory", 0));

        for (loop = 0; loop < no_objects; loop++)
        {
                /* set all the object's scales to 1.0 */
                masterptr[loop].scalex = 1.0;
                masterptr[loop].scaley = 1.0;
                masterptr[loop].scalez = 1.0;
                /* set all the obJect's angles to 0.0 */
                masterptr[loop].anglex = 0.0;
                masterptr[loop].angley = 0.0;
                masterptr[loop].anglez = 0.0;
                /* set all the object's vertices, edges and polygons to 0 */
                masterptr[loop].no_vertices = 0;
                masterptr[loop].no_edges = 0;
                masterptr[loop].no_polygons = 0;
        }

        return (OKAY);
}

/****************************************************************************
* init_instance() - initialises the array which will store the instances of *
*                   the master objects                                      *
****************************************************************************/
int init_instance(int no_instances)
{
        int loops;

        debug("init_instance()", 1);

        /* create the space required to store the instance objects */
        instanceptr = (struct instance *) malloc(sizeof(struct instance) * no_instances);

        if (instanceptr == NULL)
                return(error("0043", "Cannot allocate memory", 0));

        for (loop = 0; loop < no_instances; loop++)
        {
                /* set the initial location of all the objects to 0 */
                instanceptr[loop].posx = 0.0;
                instanceptr[loop].posy = 0.0;
                instanceptr[loop].posz = 0.0;
                /* set all the minimum values of all the object to 0 */
                instanceptr[loop].minx = 0.0;
                instanceptr[loop].miny = 0.0;
                instanceptr[loop].minz = 0.0;
                /* set all the maximum values of all the objects to 0 */
                instanceptr[loop].maxx = 0.0;
                instanceptr[loop].maxy = 0.0;
                instanceptr[loop].maxz = 0.0;
                /* set all the instance's scales to 1.0 */
                instanceptr[loop].scalex = 1.0;
                instanceptr[loop].scaley = 1.0;
                instanceptr[loop].scalez = 1.0;
                /* set all the instance's angles to 0.0 */
                instanceptr[loop].anglex = 0.0;
                instanceptr[loop].angley = 0.0;
                instanceptr[loop].anglez = 0.0;
                /* default master no should be set to 0 (this will be changed though */
                instanceptr[loop].master_no = 0;
                /* set all the object's vertice to 0 */
                instanceptr[loop].no_vertices = 0;
                /* set the default style of the object to wlreframe */
                instanceptr[loop].style = WFRAME;
                /* set the default solidity of the object to false */
                instanceptr[loop].solid = FALSE;
                /* set up the default outcome string */
                instanceptr[loop].outcome = (char *) malloc(sizeof(char));
                instanceptr[loop].outcome = '\0';
        }
        return (OKAY);
}

/****************************************************************************
* create_object_instance() - this function builds the instance object using *
*                            the master object and the angle and scale that *
*                            that needs to transform the instance           *
****************************************************************************/
int create_object_instance(struct master *masterptr, struct instance *instanceptr,
                        int instance_no, int master_no)
{
        int RESULT = OKAY;
        int loop, no_verts;
        float xpnt, ypnt, zpnt;

        /* pass the no_polygons value into the instance */
        no_verts = masterptr[master_no].no_vertices;
        instanceptr[instance_no].no_vertices = no_verts;

        /* create array for instance polygons */
        instanceptr[instance_no].xvert = (float *) malloc(sizeof(float) * no_verts);
        instanceptr[instance_no].yvert = (float *) malloc(sizeof(float) * no_verts);
        instanceptr[instance_no].zvert = (float *) malloc(sizeof(float) * no_verts);
        /* now check that these have been created */
        if ((instanceptr[instance_no].xvert == NULL) ||
            (instanceptr[instance_no].yvert == NULL) ||
            (instanceptr[instance_no].zvert == NULL))
                return(error("0048", "Error allocating memory", lincnt));

        for (loop = 0; loop < no_verts; loop++)
        {
                /* the x, y, z coord from the master */
                xpnt = masterptr[master_no].xvert[loop];
                ypnt = masterptr[master_no].yvert[loop];
                zpnt = masterptr[master_no].zvert[loop];
                /* scale by the master values */
                scale(&xpnt, &ypnt, &zpnt, masterptr[master_no].scalex,
                                        masterptr[master_no].scaley,
                                        masterptr[master_no].scalez);
                /* now scale the instance with the instance values */
                scale(&xpnt, &ypnt, &zpnt, instanceptr[instance_no].scalex,
                                        instanceptr[instance_no].scaley,
                                        instanceptr[instance_no].scalez);
                /* rotate by the master values */
                rotate(&xpnt, &ypnt, &zpnt, masterptr[master_no].anglex,
                                        masterptr[master_no].angley,
                                        masterptr[master_no].anglez);
                /* now rotate the instance with the instance values */
                rotate(&xpnt, &ypnt, &zpnt, instanceptr[instance_no].anglex,
                                        instanceptr[instance_no].angley,
                                        instanceptr[instance_no].anglez);
                /* place the coordinate into the instance structure */
                instanceptr[instance_no].xvert[loop] = xpnt;
                instanceptr[instance_no].yvert[loop] = ypnt;
                instanceptr[instance_no].zvert[loop] = zpnt;
        }
        return (RESULT);
}

/***************************************************************************
* remove master() - removes the array which stores the master objects      *
***************************************************************************/
void remove_master(int no_objects)
{
        int loop;

        debug("remove_master()", 1);

        if (masterptr == NULL)
                warn("0003", "Nothing in memory to free", 0);
        else
        {
                for (loop = 0; loop < no_objects; loop++)
                {
                        /* free the vertices array */
                        if (masterptr[loop].no_vertices > O)
                        {
                                free(masterptr[loop].xvert);
                                free(masterptr[loop].yvert);
                                free(masterptr[loop].zvert);
                        }

                        /* free the edges array */
                        if (masterptr[loop].no_edges > 0)
                        {
                                free(masterptr[loop].edge0);
                                free(masterptr[loop].edge1);
                        }

                        /* free the polygons array */
                        if (masterptr[loop].no_polygons > 0)
                        {
                                free(masterptr[loop].poly0);
                                free(masterptr[loop].poly1);
                                free(masterptr[loop].poly2);
                        }
                }

                /* now free the master array completely */
                free(masterptr);
        }
}

/****************************************************************************
* remove instance() - removes the array which stores the instances of the   *
*                     master objects                                        *
****************************************************************************/
void remove_instance(int no_instances)
{
        int loop;

        debug("remove_instance()", 1);

        if (instanceptr == NULL)
                warn("0003", "Nothing in memory to free", 0);
        else
        {
                for (loop = 0; loop < no_instancess loop++)
                {
                        /* free the arrays of vertices */
                        if (instanceptr[loop].xvert != NULL)
                                free(instanceptr[loop].xvert);

                        if (instanceptr[loop].yvert != NULL)
                                free(instanceptr[loop].yvert);

                        if (instanceptr[loop].zvert != NULL)
                                free(instanceptr[loop].zvert);

                        /* free the arrays of colour values for edges and polygons */
                        if (instanceptr[loop].edge_colour != NULL)
                                free(instanceptr[loop].edge_colour);
                        if (instanceptr[loop].poly_colour != NULL)
                                free(instanceptr[loop].poly_colour);

                        /* free the outcome string */
                        if (instanceptr[loop].outcome != "")
                                free(instanceptr[loop].outcome);
                }
                /* now free the instance array structure completely */
                free(instanceptr);
        }
}
