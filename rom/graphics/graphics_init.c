/*
    (C) 1995-96 AROS - The Amiga Replacement OS
    $Id$

    Desc: Graphics library
    Lang: english
*/
#include <exec/resident.h>
#include <proto/exec.h>
#include <aros/libcall.h>
#include <dos/dos.h>
#include <exec/execbase.h>
#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <graphics/text.h>
#include <proto/graphics.h>
#include <utility/utility.h>
#include "graphics_intern.h"
#include "libdefs.h"

#define INIT    AROS_SLIB_ENTRY(init,Graphics)

static const char name[];
static const char version[];
static const APTR inittabl[4];
static void *const LIBFUNCTABLE[];
struct LIBBASETYPE * INIT();
extern const char LIBEND;

extern int  driver_init (struct LIBBASETYPE *);
extern int  driver_open (struct LIBBASETYPE *);
extern void driver_close (struct LIBBASETYPE *);
extern void driver_expunge (struct LIBBASETYPE *);

int Graphics_entry(void)
{
    /* If the library was executed by accident return error code. */
    return -1;
}

const struct Resident Graphics_resident=
{
    RTC_MATCHWORD,
    (struct Resident *)&Graphics_resident,
    (APTR)&LIBEND,
    RTF_AUTOINIT|RTF_COLDSTART,
    VERSION_NUMBER,
    NT_LIBRARY,
    65,
    (char *)name,
    (char *)&version[6],
    (ULONG *)inittabl
};

static const char name[]=GRAPHICSNAME;

static const char version[]=VERSION_STRING;

static const APTR inittabl[4]=
{
    (APTR)sizeof(struct LIBBASETYPE),
    (APTR)LIBFUNCTABLE,
    NULL,
    &INIT
};

#ifndef SYSFONTNAME
#   define SYSFONTNAME  "topaz.font"
#endif

static struct TextAttr sysTA;

AROS_LH2(struct LIBBASETYPE *, init,
 AROS_LHA(struct LIBBASETYPE *, LIBBASE, D0),
 AROS_LHA(BPTR,               segList,   A0),
           struct ExecBase *, sysBase, 0, Graphics)
{
    AROS_LIBFUNC_INIT

    SysBase = sysBase;

    Disable();
    if (!driver_init (LIBBASE))
    {
        Enable();
        return NULL;
    }
    Enable();

    /* You would return NULL if the init failed */
    return LIBBASE;
    AROS_LIBFUNC_EXIT
}

AROS_LH1(struct LIBBASETYPE *, open,
 AROS_LHA(ULONG, version, D0),
           struct LIBBASETYPE *, LIBBASE, 1, Graphics)
{
    AROS_LIBFUNC_INIT
    struct TextFont * def;

    /* Keep compiler happy */
    version=0;

    if (!LIBBASE->DefaultFont)
    {
        sysTA.ta_Name  = (STRPTR)SYSFONTNAME;
        sysTA.ta_YSize = 8;
        sysTA.ta_Style = FS_NORMAL;
        sysTA.ta_Flags = 0;

        def = OpenFont (&sysTA);

        if (!def)
            return NULL;

        LIBBASE->DefaultFont = def;
        sysTA.ta_YSize = def->tf_YSize;
    }

    UtilityBase = OpenLibrary (UTILITYNAME,0L);

    if (!UtilityBase)
        return NULL;

    Disable();
    if (!driver_open (LIBBASE))
    {
        Enable();
        return NULL;
    }
    Enable();

    /* Allocate 8 IPTR's for a hash list needed by
       GfxAssociate(), GfxLookUp()                  */
    LIBBASE->hash_table = (LONG *)AllocMem(8*sizeof(LONG *), 
                                           MEMF_CLEAR|MEMF_PUBLIC);
    if (!LIBBASE->hash_table)
      return NULL;

    /* I have one more opener. */
    LIBBASE->LibNode.lib_OpenCnt++;
    LIBBASE->LibNode.lib_Flags&=~LIBF_DELEXP;

    /* You would return NULL if the open failed. */
    return LIBBASE;
    AROS_LIBFUNC_EXIT
}

AROS_LH0(BPTR, close,
           struct LIBBASETYPE *, LIBBASE, 2, Graphics)
{
    AROS_LIBFUNC_INIT

    /* I have one fewer opener. */
    if(!--LIBBASE->LibNode.lib_OpenCnt)
    {
        driver_close (LIBBASE);

        /* Delayed expunge pending? */
        if(LIBBASE->LibNode.lib_Flags&LIBF_DELEXP)
            /* Then expunge the library */
            return expunge();
    }
    return 0;
    AROS_LIBFUNC_EXIT
}

AROS_LH0(BPTR, expunge,
           struct LIBBASETYPE *, LIBBASE, 3, Graphics)
{
    AROS_LIBFUNC_INIT
#ifndef DISK_BASED
    if (!(LIBBASE->LibNode.lib_OpenCnt) )
    {
        if (LIBBASE->DefaultFont)
        {
            CloseFont (LIBBASE->DefaultFont);

            LIBBASE->DefaultFont = NULL;
        }

        /* Allow the driver to release uneccessary memory */
        driver_expunge (LIBBASE);
    }

    /* Don't delete this library. It's in ROM and therefore cannot be
       deleted */
    return 0L;
#else
    BPTR ret;

    /* Test for openers. */
    if (LIBBASE->LibNode.lib_OpenCnt)
    {
        /* Set the delayed expunge flag and return. */
        LIBBASE->LibNode.lib_Flags|=LIBF_DELEXP;
        return 0;
    }

    /* Get rid of the library. Remove it from the list. */
    Remove(&LIBBASE->LibNode.lib_Node);

    /* Get returncode here - FreeMem() will destroy the field. */
    ret=0L;

    /* Free the memory. */
    FreeMem((char *)LIBBASE-LIBBASE->LibNode.lib_NegSize,
            LIBBASE->LibNode.lib_NegSize+LIBBASE->LibNode.lib_PosSize);

    return ret;
#endif
    AROS_LIBFUNC_EXIT
}

AROS_LH0I(int, null,
            struct LIBBASETYPE *, LIBBASE, 4, Graphics)
{
    AROS_LIBFUNC_INIT
    return 0;
    AROS_LIBFUNC_EXIT
}
