/*
    Copyright � 1995-2017, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Allocate a signal
    Lang: english
*/

#define DEBUG 0

#include <exec/execbase.h>
#include <exec/tasks.h>
#include <aros/libcall.h>
#include <proto/exec.h>

#include "exec_util.h"
#if defined(__AROSEXEC_SMP__)
#include "etask.h"
#include "exec_locks.h"
#endif

/*****************************************************************************

    NAME */

	AROS_LH1(BYTE, AllocSignal,

/*  SYNOPSIS */
	AROS_LHA(BYTE, signalNum, D0),

/*  LOCATION */
	struct ExecBase *, SysBase, 55, Exec)

/*  FUNCTION
	Allocate a given signal out of the current task's pool of signals.
	Every task has a set of signals to communicate with other tasks.
	Half of them are reserved for the system and half of them are
	free for general use. Some of the reserved signals (e.g.
	SIGBREAKF_CTRL_C) have a defined behaviour and may be used by user
	code, however.

	You must not allocate or free signals from exception handlers.

    INPUTS
	signalNum - Number of the signal to allocate or -1 if any signal
		    will do.

    RESULT
	Number of the signal or -1 if the signal couldn't be allocated.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
	FreeSignal(), Signal(), Wait()

    INTERNALS

******************************************************************************/
{
    AROS_LIBFUNC_INIT

    /* Cast signalNum to BYTE for AOS/68k compatibility. Apps may set up only D0.b */
    return AllocTaskSignal(GET_THIS_TASK, (BYTE)signalNum, SysBase);

    AROS_LIBFUNC_EXIT
} /* AllocSignal() */

LONG AllocTaskSignal(struct Task *thisTask, BYTE signalNum, struct ExecBase *SysBase)
{
    ULONG mask;
    ULONG mask1;

    D(bug("[Exec] %s()\n", __func__));

    mask = thisTask->tc_SigAlloc;

    /* Will any signal do? */
    if(signalNum < 0)
    {
	/*
	 * To get the last nonzero bit in a number I use a&~a+1:
	 * Given a number that ends with a row of zeros  xxxx1000
	 * I first toggle all bits in that number	 XXXX0111
	 * then add 1 to toggle all but the last 0 again XXXX1000
	 * and AND this with the original number	 00001000
	 *
	 * And since ~a+1=-a I can use a&-a instead.
	 *
	 * And to get the last zero bit I finally use ~a&-~a.
	 */
	mask1 = ~mask & - ~mask;

	/* Is the bit already allocated? */
	if(mask1 == 0)
	    return -1;

	/* And get the bit number */
	signalNum=(mask1&0xffff0000?16:0)+(mask1&0xff00ff00?8:0)+
		  (mask1&0xf0f0f0f0? 4:0)+(mask1&0xcccccccc?2:0)+
		  (mask1&0xaaaaaaaa? 1:0);
    }
    else
    {
	mask1 = 1L << signalNum;

	/* If signal bit is already allocated, return. */
	if(thisTask->tc_SigAlloc & mask1)
	    return -1;
    }

    /*
     *	Exec shouldn't need to protect the changing of the task signal masks
     *	because the only thing allowed to change the mask of allocated,
     *	excepting and waiting signals is the task itself. 
     *  Received signals do need protected because they can be modified
     *  by interrupts, so atomicity cannot be relied upon.
     */

    thisTask->tc_SigAlloc  |=  mask1;
    thisTask->tc_SigExcept &= ~mask1;
    thisTask->tc_SigWait   &= ~mask1;

    Disable();
#if defined(__AROSEXEC_SMP__)
    if (thisTask->tc_UnionETask.tc_ETask)
    {
        EXEC_LOCK_WRITE(&IntETask(thisTask->tc_UnionETask.tc_ETask)->iet_TaskLock);
    }
#endif

    thisTask->tc_SigRecvd  &= ~mask1;

#if defined(__AROSEXEC_SMP__)
    if (thisTask->tc_UnionETask.tc_ETask)
    {
        EXEC_UNLOCK(&IntETask(thisTask->tc_UnionETask.tc_ETask)->iet_TaskLock);
    }
#endif
    Enable();

    return signalNum;
}
