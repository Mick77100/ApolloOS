/*
    Copyright � 1995-2019, The AROS Development Team. All rights reserved.
    Copyright � 2001-2003, The MorphOS Development Team. All Rights Reserved.
    $Id$
*/

#include <string.h>
#include "intuition_intern.h"

/*****************************************************************************

    NAME */
#include <proto/intuition.h>

        AROS_LH1(struct Screen *, GetDefaultPubScreen,


/*  SYNOPSIS */
        AROS_LHA(UBYTE *, nameBuffer, A0),

/*  LOCATION */
        struct IntuitionBase *, IntuitionBase, 97, Intuition)

/*  FUNCTION
        Returns the name of the current default public screen. This will be
        "Workbench" if there is no default public screen.

    INPUTS
        nameBuffer - A buffer of length MAXPUBSCREENNAME

    RESULT
        Pointer to the default public screen or NULL, if there is none.

    NOTES
        Only Public Screen Manager utilities want to use this function
        since it is easy to open a window on the default public screen
        without specifying a name.

        The returned Screen pointer can become invalid any time, if the screen
        gets closed after the return of GetDefaultPubScreen(). This function
        does not lock the screen.

        Better use LockPubScreen(NULL).

    EXAMPLE

    BUGS

    SEE ALSO
        SetDefaultPubScreen(), OpenWindow(), LockPubScreen()

    INTERNALS

*****************************************************************************/
{
    AROS_LIBFUNC_INIT

    struct Screen *defscreen;
    STRPTR  	   name;

    DEBUG_GETDEFAULTPUBSCREEN(dprintf("GetDefaultPubScreen()\n"));

    LockPubScreenList();

    defscreen = GetPrivIBase(IntuitionBase)->DefaultPubScreen;

    if (defscreen)
    {
        name = GetPrivScreen(defscreen)->pubScrNode->psn_Node.ln_Name;
    }
    else
    {
        name = "Workbench";
    }

    if (nameBuffer)
    {
        strcpy(nameBuffer, name);
    }

    UnlockPubScreenList();

    return defscreen;

    AROS_LIBFUNC_EXIT
} /* GetDefaultPubScreen */
