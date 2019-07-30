#ifndef IPL_DZOOM_H_
#define IPL_DZOOM_H_

/**
    IPL_dzoom.h


    @file       IPL_dzoom.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Type.h"
#include "ipl_utility.h"
#include "ipl_ctrl_flow.h"

#define DZOOM_ITEM_MAX  8

typedef enum {
	IPL_DZOOM_EXT_FUNC_NONE = 0,
	IPL_DZOOM_EXT_FUNC_IME_SQUARE_PAD = 1,
} IPL_DZOOM_EXT_FUNC;

typedef struct {
	IPL_DZOOM_EXT_FUNC ext_func;
} IPL_DZOOM_EXT_INFO;

/**
     set digital zoom max index

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 idx, depend on IPL lib
*/
extern void IPL_DzoomSetMaxIndex(UINT32 Id, UINT32 Idx);

/**
     get digital zoom max index

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 idx, depend on IPL lib

     @return UINT32
         - @b max index

*/
extern UINT32 IPL_DzoomGetMaxIndex(UINT32 Id);

/**
     set digital zoom table

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 digital zoom table pointer
     @param[in] UINT32 digital zoom table max value
*/
extern void IPL_DZoomSetInfor(UINT32 Id, UINT32 *pdzoomtbl, UINT32 dzoomtbl_max);

/**
     get sie crop size

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 table index
     @param[in] BOOL still table flag
     @param[out] UINT32 sizeH
     @param[out] UINT32 sizeV
*/
extern void IPL_DzoomGetSIECropSize(UINT32 Id, UINT32 Idx, BOOL CapFlag, UINT32 *SizeH, UINT32 *SizeV);

/**
     get sie crop size

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 table index
     @param[out] UINT32 sizeH
     @param[out] UINT32 sizeV
*/

/**
     get sie out size

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 table index
     @param[out] UINT32 sizeH
     @param[out] UINT32 sizeV
*/
extern void IPL_DzoomGetSIEOutSize(UINT32 Id, UINT32 Idx, UINT32 *SizeH, UINT32 *SizeV);

/**
     get raw 2 yuv in size

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 table index
     @param[out] UINT32 sizeH
     @param[out] UINT32 sizeV
*/
extern void IPL_DzoomGetRaw2YUVInSize(UINT32 Id, UINT32 Idx, UINT32 *SizeH, UINT32 *SizeV);

/**
     set digital zoom current index

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 index
*/
extern void IPL_DzoomSetIndex(UINT32 Id, UINT32 Idx);

/**
     set digital zoom frame rate

     @param[in] UINT32 frame rate(it must be >= 1)
*/
extern void IPL_DzoomSetRate(UINT32 frame);

/**
     get digital zoom ratio

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 digital zoom index

     @return UINT32
         - @b ratio 1x = 100, 2.35x = 235
*/
extern UINT32 IPL_DzoomGetRatio(UINT32 Id, UINT32 DzoomIdx);

/**
     set digital zoom ratio

     @param[in] IPL_PROC_ID ipl process id
     @param[in] UINT32 digital ratio

*/
extern void IPL_DzoomSetRatio(UINT32 Id, UINT32 ratio);

/**
     get current digital zoom index

     @param[in] IPL_PROC_ID ipl process id

     @return UINT32
         - @b digital zoom index(0 ~ digital zoom max)
*/

extern UINT32 IPL_DzoomGetIndex(UINT32 Id);

/**
     set digital zoom

     @param[in] IPC_Dzoom_Info digital zoom information

*/
extern void IPL_DzoomSet(IPC_Dzoom_Info *info);
/**
     get auto digital zoom buffer

     @param[in] IPL_PROC_ID ipl process id

*/
extern UINT32 IPL_DzoomGetAutoAddr(UINT32 id);

/**
     set digital zoom Ext Info

     @param[in] IPL_PROC_ID ipl process id
     @param[in] IPL_DZOOM_EXT_INFO

*/
extern void IPL_DzoomSetExtInfo(UINT32 id, IPL_DZOOM_EXT_INFO* ext_ctrl);

#endif //IPL_DZOOM_INT_H_
