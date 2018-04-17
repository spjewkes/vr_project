/********************************************************
* ####  ####   ###   ##### ####   ###  ##   ##     #### *
* ## ## ## ## ## ## ##     ## ## ## ## ### ###    ##    *
* ####  ####  ## ## ## ### ####  ##### #######    ##    *
* ##    ## ## ## ## ##  ## ## ## ## ## ## # ##    ##    *
* ##    ## ##  ###   ####  ## ## ## ## ##   ## ##  #### *
********************************************************/
/****************
* include files *
****************/
#include <dos.h>
#include <process.h>
#include <string.h>
#include <stddef.h>
#include "defs.h"

/****************************************************************************
* program() - this function looks at the string for a particular instance   *
*             and checks whether it is possible to execute it using the     *
*             spawnvp() command                                             *
****************************************************************************/
void program(int instance, struct instance *ptr)
{
        char path[MAXLINE];
        char *array[MAXARG];
        char *chptr;
        int loop, flag, inparam;

        /* point to the start of the array */
        chptr = path;

        /* set the first pointer in array to be NULL */
        array[0] = NULL;

        /* copy the outcome string to a char array we can play with */
        strcpy(path, ptr[instance].outcome);

        /* if the string doesn't contain the string 'solid' then it must
           contain a program to execute */
        if (strcmpi(path, "solid") != EQUAL)
        {
                /* first skip any blanks */
                while ((*chptr == ' ') || (*chptr == '\t'))
                        chptr++;

                /* now get the first word - which we assume to be the executable */
                while ((*chptr == ' ') && (*chptr == '\t') && (*chptr != '\0'))
                        chptr++;

                if (chptr != '\0')
                {
                        /* then we may have some parameters to deal with */
                        /* first we want to put a null terminator after the executable's
                           name and move along the pointer */
                        /* *chptr++ = '\0'; */
                        /* reset the chptr because the first parameter has to be the
                           name of the executable */
                        chptr = path;

                        /* now we start filling the array of char pointers */
                        for (loop = 0; loop < MAXAKG-2; loop++)
                        {
                                /* skip any more blanks */
                                while ((*chptr == ' ') || (*chptr == '\t'))
                                        chptr++;

                                /* get the array to point to the next parameter */
                                array[loop] = chptr;
                                array[loop+1] = NULL;

                                /* set flag to zero and start processing string */
                                flag = 0;
                                /* now we want to loop to pick up more parameters */
                                while (flag == 0)
                                {
                                        switch (* chpt r)
                                        {
                                                case '\0':
                                                        /* the end of the array of characters */
                                                        if (inparam == FALSE)
                                                                array[loop] = '\0';

                                                        /* terminate string */
                                                        *chptr++ = '\0';

                                                        /* now leave the while loop */
                                                        goto exit;

                                        case ' ':
                                        case '\t':
                                                /* end of a parameter */
                                                inparam = FALSE;
                                                /* terminate the string */
                                                *chptr++ = '\0';
                                                /* end this for..loop */
                                                flag = 1;
                                                break;

                                        default:
                                                /* we're in a parameter */
                                                inparam = TRUE;
                                                /* increment pointer */
                                                chptr++;
                                                break;
                                        }
                                }
                        }
                }
                /* this is where we spawn the program */
                exit:
                spawnvp(P_WAIT, path, array);
        }
}
