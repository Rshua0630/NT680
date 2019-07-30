/**
    Public header file for photo task information

    This file is the header file that define the API and data type for photo task.

    @file       PhotoTask.h
    @ingroup    mIAppPhoto
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _PHOTO_TASK_H_
#define _PHOTO_TASK_H_

#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mIAppCap */
//@{

#define PHOTO_EXIT_CB 1

/**
    max hook funcion number
*/
//@{
#define PHOTO_FUNC_MAX_NUM      10      ///<  max. process func for each task
//@}

/**
   photo process priority
*/
typedef enum {
	PHOTO_PRIORITY_H = 0,     ///< photo priority H:16
	PHOTO_PRIORITY_M,         ///< photo priority M:17
	PHOTO_PRIORITY_L,         ///< photo priority L:18
	PHOTO_PRIORITY_SH,        ///< photo priority super H:10
	PHOTO_PRIORITY_MAX_NUM,
	ENUM_DUMMY4WORD(PHOTO_PROC_PRIORITY)
} PHOTO_PROC_PRIORITY;


/**
   photo trigger event
*/
typedef enum {
	PHOTO_ISR_SIE_VD = 0,           ///< sie vd
	PHOTO_ISR_SIE_BP1,              ///< sie bp1
	PHOTO_ISR_SIE_BP2,              ///< sie bp2
	PHOTO_ISR_SIE_BP3,              ///< sie bp3
	PHOTO_ISR_SIE_ACTST,            ///< sie active start
	PHOTO_ISR_SIE_CRPST,            ///< sie crop start
	PHOTO_ISR_SIE_FLASH_TRG,        ///< sie flash trigger
	PHOTO_ISR_SIE_FLASH_END,        ///< sie flash end
	PHOTO_ISR_SIE_MSH_TRG,          ///< sie mshutter trigger
	PHOTO_ISR_SIE_MSH_CLOSE_END,    ///< sie mshutter close end
	PHOTO_ISR_SIE_MSH_OPEN_END,     ///< sie mshutter open end
	PHOTO_ISR_SIE_DRAM_OUT0_END,    ///< sie dram out 0
	PHOTO_ISR_SIE_DRAM_OUT1_END,    ///< sie dram out 1
	PHOTO_ISR_SIE_DRAM_OUT2_END,    ///< sie dram out 2
	PHOTO_ISR_SIE_DRAM_OUT3_END,    ///< sie dram out 3
	PHOTO_ISR_SIE_DRAM_OUT4_END,    ///< sie dram out 4
	PHOTO_ISR_SIE_VA_END,           ///< sie va end

	PHOTO_ISR_PRE_FRM_ST,           ///< pre frame start
	PHOTO_ISR_PRE_FRM_END,          ///< pre frame end

	PHOTO_ISR_IFE_FRM_END,          ///< ife end

	PHOTO_ISR_DCE_FRM_ST,           ///< dce frame start
	PHOTO_ISR_DCE_FRM_END,          ///< dce frame end

	PHOTO_ISR_IPE_FRM_END,          ///< ipe frame end
	PHOTO_ISR_IPE_YCOUTEND,         ///< ipe YCC out end
	PHOTO_ISR_IPE_CSUBOUTEND,       ///< ipe csub out end
	PHOTO_ISR_IPE_VSUBOUTEND,       ///< ipe vsub out end

	PHOTO_ISR_IME_FRM_END,          ///< ime frame end

	PHOTO_ISR_IFE2_FRM_END,         ///< ife2 frame end
	PHOTO_ISR_MAX_NUM,
	ENUM_DUMMY4WORD(PHOTO_ISR_TRIG_EVENT)
} PHOTO_ISR_TRIG_EVENT;


/**
   photo buffer
*/
typedef struct {
	UINT32 Addr;    ///< buffer address
	UINT32 Size;    ///< buffer size
} PHOTO_BUF;

/**
   photo process type
*/
typedef void (*PHOTO_PROC)(MEM_RANGE *buf, MEM_RANGE *cachebuf);

#if (PHOTO_EXIT_CB == 1)
/**
   photo exit type
*/
typedef void (*PHOTO_EXIT)(void);
#endif
/**
   photo get buffer type
*/
typedef UINT32(*PHOTO_GET_BUF)(void);

/**
   photo hook function object
*/
typedef struct {
	PHOTO_PROC_PRIORITY Priority;         ///< priority.
	IPL_PROC_ID   IPLId;                ///< IPL Id.
	PHOTO_ISR_TRIG_EVENT TrigEvent;     ///< isr tigger event


	CHAR *ProcName;                     ///< process name
	PHOTO_PROC InitFP;                  ///< init fp, only execute at first process
	PHOTO_PROC ProcFP;                  ///< process fp
	PHOTO_PROC EndFP;                   ///< process end fp
#if (PHOTO_EXIT_CB == 1)
	PHOTO_EXIT ExitFp;					///< exit fp, only execute at photo close
#endif

	PHOTO_GET_BUF GetBufFP;             ///< get working buffer fp
	PHOTO_GET_BUF GetCacheBufFP;        ///< get working cache buffer fp
} PHOTO_FUNC;


/**
   photo cb event
*/
typedef enum {
	PHOTO_CB_GET_BUF = 0,               ///< get non-cache buffer, in_info data type: UINT32 (buffer size), out_info data type: UINT32 (buffer address)
	PHOTO_CB_GET_CACHE_BUF,             ///< get cache buffer, in_info data type: UINT32 (buffer size), out_info data type: UINT32 (buffer address)
	PHOTO_CB_REL_BUF,	                ///< release non-cache buffer, in_info data type: UINT32 (buffer size), in_info data type: UINT32 (buffer address)
	PHOTO_CB_REL_CACHE_BUF,             ///< release cache buffer, in_info data type: UINT32 (buffer size), in_info data type: UINT32 (buffer address)
	ENUM_DUMMY4WORD(PHOTO_CB_EVENT)
} PHOTO_CB_EVENT;
/**
   photo callback function
*/
typedef void (*PHOTO_CB_FUNC)(PHOTO_CB_EVENT event, CHAR *func_name, void *in_info, void *out_info);


/**
   photo hook function list
*/
typedef struct PHOTO_FUNC_INFO {
	PHOTO_FUNC Info;                       ///< function information
	struct PHOTO_FUNC_INFO *pNext;         ///< point next Function Obj, last Function must be set NULL
} PHOTO_FUNC_INFO;

/**
     sie trigger event

     @param[in] Id ipl process id
     @param[in] IntStatus interrupt status
     @param[in] idx frame index
*/
extern void Photo_ISR_EventSIE(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

/**
     ife trigger event

     @param[in] Id ipl process id
     @param[in] IntStatus interrupt status
     @param[in] idx frame index
*/
extern void Photo_ISR_EventIFE(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

/**
     dce trigger event

     @param[in] Id ipl process id
     @param[in] IntStatus interrupt status
     @param[in] idx frame index
*/
extern void Photo_ISR_EventDCE(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

/**
     ipe trigger event

     @param[in] Id ipl process id
     @param[in] IntStatus interrupt status
     @param[in] idx frame index
*/
extern void Photo_ISR_EventIPE(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

/**
     ime trigger event

     @param[in] Id ipl process id
     @param[in] IntStatus interrupt status
     @param[in] idx frame index
*/
extern void Photo_ISR_EventIME(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

/**
     ife2 trigger event

     @param[in] Id ipl process id
     @param[in] IntStatus interrupt status
     @param[in] idx frame index
*/
extern void Photo_ISR_EventIFE2(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

/**
     ife2 trigger event

     @param[in] FuncInfo hook function list
     @param[in] callback event

     @return
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER Photo_Open(PHOTO_FUNC_INFO *FuncInfo, PHOTO_CB_FUNC CBfp);

/**
     photo close

     @return
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER Photo_Close(void);

/**
     wait task enter idle mode

     @param[in] Id photo process id

     @return
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern void Photo_WaitIdle(PHOTO_PROC_PRIORITY priority);


extern void Photo_DumpInfo(void (*dump)(char *fmt, ...));

extern void Photo_InstallID(void) _SECTION(".kercfg_text");
//@}

#endif //_PHOTO_TASK_H_
