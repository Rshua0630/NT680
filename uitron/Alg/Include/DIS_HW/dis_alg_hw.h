/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2015.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Connie Yu                                                         *
****************************************************************************/

/**
    DIS lib

    Sample module detailed description.

    @file       Dis_alg_hw.h
    @ingroup    mILibDIS
    @note       DIS HW control.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_ALG_HW_H_
#define _DIS_ALG_HW_H_


//#define MDIS_PRV_MAX_SIZE    2304* 1296 * 2 * 2 ///< MAXSIZE*1.44*FMT_422*2
//#define DISLIB_VECTOR_NORM      12      ///< output vector norm factor(2^12)

typedef struct {
	UINT32  Hsz;
	UINT32  Vsz;
} DIS_SIZE;

/**
    DIS ALG Entire Configuration

    Structure of DIS ALG parameters when one wants to configure this module.
*/
//@{
typedef struct _DISHW_PARAM_STRUCT {
	//DIS_ENGINE_OPERATION DIS_operation; ///< DIS engine operation
	UINT32 InSizeH;                     ///< input horizontal pixels
	UINT32 InSizeV;                     ///< input vertical pixels
	UINT32 InLineOfs;                   ///< input line offset
	UINT32 InAdd0;                      ///< input starting address 0
	UINT32 InAdd1;                      ///< input starting address 1
	UINT32 InAdd2;                      ///< input starting address 2
	UINT16 frame_cnt;                   ///< frame count
} DISHW_PARAM;
//@}

extern void disHw_initialize(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disHw_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disHw_end(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern void disHw_setDisInfor(DISHW_PARAM *disInfo);

extern ER disHw_chgDisSizeConfig(void);
extern UINT32 disHw_getCurDisInH(void);
extern UINT32 disHw_getCurDisInV(void);

#endif

