/**
    Header file of background job.

    Background job export variables and function prototypes.

    @file       NvtBack.h
    @ingroup    mINvtUser
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef NVTBACK_H
#define NVTBACK_H

#include "Type.h"
#include "NVTEvent.h"
#include "NVTReturn.h"

extern void NvtBack_InstallID(void) _SECTION(".kercfg_text");


/**
    @addtogroup mINvtUser
*/

//@{

/**
    Background execution function prototype
*/
typedef UINT32(* BKG_FUNC_PTR)(void);

/**
    Background job execution table entry
*/
typedef struct _BKG_JOB_ENTRY {
	NVTEVT          event;         ///< The event to start the background job
	BKG_FUNC_PTR    pfn;           ///< The function pointer of background job
} BKG_JOB_ENTRY;

/**
    Background function table
*/
typedef struct _BKG_OBJ {
	BKG_JOB_ENTRY *pDefaultFuncTbl;    ///< Default background function table. set to NULL if not used
} BKG_OBJ;

/**
    The return parameter index while finish backgroun job.
    Background task would post event with parameter to User Task,when background done.
*/
//@{
#define ONDONE_PARAM_INDEX_CMD     0  ///<The event which user assigned in BKG_JOB_ENTRY.
#define ONDONE_PARAM_INDEX_RET     1  ///<The return value of background done.
//@}


/**
    Open Background.

    This API would set default background function table and start background task.

    @param[in] param       The pointer of background function table.

    @return
        - @b E_OK if success
        - @b E_SYS if backgroun opened.
*/
extern ER BKG_Open(BKG_OBJ *param);

/**
    Close Background.

    This API would wait background idle and then terminate background task.

    @return
        - @b E_OK if success
        - @b E_SYS if backgroun not opened.
*/
extern ER BKG_Close(void);

/**
    Close Background.

    This API would check background  task.

    @return
        - @b TRUE if task busy.
        - @b FALSE if task not busy.
*/
extern BOOL BKG_GetTskBusy(void);


/**
    Reset background task

    This API would flush all background event.Terminate and start task.
    If need hot swap storage card , need reset background task
*/
extern void BKG_ResetTsk(void);


/**
    Set execution table

    While Background task recesive flag,would look-up this execution table.
    If the matched event found,task would exect the event function.

    @param[in] pBackgroundFuncTbl The pointer of the execution table

*/
extern void BKG_SetExecFuncTable(BKG_JOB_ENTRY *pBackgroundFuncTbl);
/**
    Get execution table

    @return current execution table.

*/
extern BKG_JOB_ENTRY *BKG_GetExecFuncTable(void);

/**
    Get background task current execute event

    @return Current running task event

*/
extern NVTEVT BKG_GetExeuteCommand(void);

/**
    Post event to background task

    @param[in] evt Command

    @note The event has to be in one entry of the execution table.
*/
extern void BKG_PostEvent(NVTEVT evt);

/**
    Flush the message queue of Background Task
*/
extern void BKG_FlushEvent(void);

/**
     Set background done to background task.
     @note Afer user task dispatch the NVTEVT_BACKGROUND_DONE
     event which background task post to user task,user task need to set
     done flag to background.
*/
extern void BKG_Done(void);


//@}
#endif //NVTBACK_H


