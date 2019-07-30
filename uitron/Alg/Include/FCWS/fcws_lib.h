/**
    FCWS detection library.

    @file       fcws_nvt_lib.h
    @ingroup    mILibFCWS_Nvt

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _FCWS_LIB_H
#define _FCWS_LIB_H

#include "ldws_lib.h"
#include "ime_lib.h"
/**
    @addtogroup mILibFCWS
*/

/**
     @name LDWSreturn status
*/
//@{
#define FCWS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define FCWS_STA_ERROR               (-1)                  ///<  some error occurred
//@}


//@{
typedef struct _APPS_FCWS_RESULT_INFO
{
    UINT32                 uiDist;            ///<  distance to front car
    BOOL                   FCVideoWarning;    ///<  FC warning (for video)
    BOOL                   FCSoundWarning;    ///<  FC warning (for sound)
    UINT32                 uiLx;              ///<  x coordinate of left point
    UINT32                 uiLy;              ///<  y coordinate of left point
    UINT32                 uiRx;              ///<  x coordinate of right point
    UINT32                 uiRy;              ///<  y coordinate of right point
    UINT32                 uiKelDist;
	UINT32                 Reserved;
} APPS_FCWS_RESULT_INFO;
//@}


//@{
typedef struct _FCWS_SRCDATA_INFO
{
    UINT32 uiFcwsSrcDataAddr;
    UINT32 uiFcwsWorkBufAddr;
    UINT32 uiFcwsEdgeDataAddr;
    UINT32 uiFcwsHistDataAddr;
    UINT32 uiFcwsSrcDataWidth;
    UINT32 uiFcwsSrcDataHeight;
    UINT32 uiFcwsSrcDataRoiWidth;
    UINT32 uiFcwsSrcDataRoiHeight;
} FCWS_SRCDATA_INFO;
//@}


typedef struct _FCWS_SEARCHRANG_INFO
{
    UINT32 uiIniRow;
    UINT32 uiFinalRow;
    UINT32 uiLeftBoundary;
    UINT32 uiRightBoundary;
} FCWS_SEARCHRANG_INFO;

/**
    FCWS histogram information for debug
    (0,0) is on left up
*/
//@{
typedef struct _FCWS_HISTO_INFO
{
    UINT32 uiVerTHRow;
    UINT32 uiVerHistV;
    UINT32 uiHoriThr;
    UINT32 uiVerThr;
    UINT32 uiHorThrReal;
    UINT32 uiVerThrReal;
    UINT32 uiStartDetDist;   ///<
    UINT32 uiEndDetDist;
} FCWS_HISTO_INFO;
//@}

typedef struct _APPS_FCWS_ALG_INFO
{
    UINT32 uiObStacleSingMode;
    UINT32 uiDect;
    UINT32 uiNeardetecton;
    UINT32 uiGetAccTagBin0;
    UINT32 uiGetAccTagBin1;
    UINT32 uiGetHistMax0;
    UINT32 uiGetHistMax1;
    UINT32 uiTimeStatus;
    UINT32 uiDistanceFP;
} APPS_FCWS_ALG_INFO;

typedef struct _APPS_FCWS_DEBUG_INFO
{
    FCWS_SEARCHRANG_INFO FcwsSearchRangInfo;
    FCWS_HISTO_INFO      FcwsHistoInfo;
} APPS_FCWS_DEBUG_INFO;

typedef struct _FCWS_DEBUG_INFO
{
    UINT32 uiIniRow;
    UINT32 uiFinalRow;
    UINT32 uiLeftBoundary;
    UINT32 uiRightBoundary;
    UINT32 uiVerTHRow;
    UINT32 uiVerHistV;
    UINT32 uiHoriThr;
    UINT32 uiVerThr;
    UINT32 uiHorThrReal;
    UINT32 uiVerThrReal;
    UINT32 uiStartDetDist;   ///<
    UINT32 uiEndDetDist;
} FCWS_DEBUG_INFO;
//@}

/**
    Set initial parameters into LDWS Lib.
*/
extern void FCWS_setInitParms(LDWS_INIT_PARMS *LDWSSysParamter);
/**
     Uninit FCWS engine.
 */
extern INT32 FCWS_UnInit(void);
/**
    Get IME parameters computing by FCWS
*/
extern IME_STL_INFO FCWS_getImeParms(UINT32 idx, APPS_FCWS_DEBUG_INFO *FcDebugInfo);
/**
    FCWS Detection
*/
extern UINT32 FCWS_Detection(FCWS_SRCDATA_INFO *FcwsSrcDataInfo, LDWS_INFOFORFCWS *LDWS_INFOFORFCWS, APPS_FCWS_RESULT_INFO *FCWS_RSLTINFO, IME_GET_STL_INFO *HistInfo, APPS_FCWS_ALG_INFO *FcwsAlgDebugInfo);

extern void FCWS_CarRsltSel(UINT32 uiSel);


#endif

