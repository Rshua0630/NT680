/**
    STOP&GO detection library.

    @file       SnG_lib.h
    @ingroup    mILibSNG

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _SNG_LIB_H
#define _SNG_LIB_H


#include "dis_lib.h"
/**
    @addtogroup mILibSNG
*/

/**
     @name Stop&go return status
*/
//@{
#define STOPNGO_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define STOPNGO_STA_ERROR               (-1)                  ///<  some error occurred
//@}

/**
     Stop&Go warning message.

*/
//@{
typedef enum _STOPNGO_WARNMSG
{
    STOPNGO_NONE = 0,          ///<
    STOPNGO_GO = 1,            ///<
    STOPNGO_MOVING = 2,
    STOPNGO_STOP = 3,
    STOPNGO_LD_OVERTHR = 4,
    STOPNGO_LD_LESSTHR = 5,
    STOPNGO_MV_VALID = 6,
    STOPNGO_MV_INVALID = 7
}STOPNGO_WARNMSG;
//@}

/**
    Stop result
*/
typedef enum _STOP_RSLT
{
    STOPDECT_STOP_FALSE = 0,  ///< not stop
    STOPDECT_STOP_TRUE = 1    ///< stop
}STOP_RSLT;

/**
     Default lane region for stop&go

*/
//@{
typedef struct _STOPNGO_ROI_STRUCT
{
    UINT32 uiLeftLdTopX;                       ///< lane region 1 on left side
    UINT32 uiLeftLdTopY;                       ///< lane region 1 on left side
    UINT32 uiLeftLdBlkWidth;                   ///< block width on left side
    UINT32 uiLeftLdBlkHeight;                  ///< block height 1 on left side
    UINT32 uiLeftLdBlkDur;                     ///< block duration on left side
    UINT32 uiRightLdTopX;                      ///< lane region 1 on Right side
    UINT32 uiRightLdTopY;                      ///< lane region 1 on Right side
    UINT32 uiRightLdBlkWidth;                  ///< block width on Right side
    UINT32 uiRightLdBlkHeight;                 ///< block height 1 on Right side
    UINT32 uiRightLdBlkDur;                    ///< block duration on Right side
} STOPNGO_ROI;
//@}

/**
    StopNGo input image and motion information

    Set StopNGo input image and motion information
*/
//@{
typedef struct _STOPNGO_IMGINFO
{
    UINT32 uiEdgImgWidth;             ///< width of edge plane
    UINT32 uiEdgImgHeight;            ///< height of edge plane
    UINT32 uiMvBlkNumH;                  ///< block number of horizontal direction
    UINT32 uiMvBlkNumV;                  ///< block number of vertical direction
} STOPNGO_IMGINFO;
//@}


/**
    StopNGo warning information

    Set StopNGo warning system
*/
//@{
typedef struct _STOPNGO_WARNSYS
{
    UINT32 uiStopStatus;             ///<
    UINT32 uiStopWarning;            ///<
    UINT32 uiStopCount;              ///<
    UINT32 uiGoWarning;              ///<
    UINT32 uiWarningPrepareCnt;      ///<
    UINT32 uiWarningConti;           ///<
    UINT32 uiStopFrame;              ///<
    UINT32 uiWarningPrepareFrame;    ///<
    UINT32 uiMovingStatus;           ///<
    UINT32 uiLdOverThr;              ///<
    UINT32 uiStopTimeCnt;
    UINT32 uiTimeDuration;
    UINT32 uiStop2GoConti;
} STOPNGO_WARNSYS;
//@}


/**
    StopNGo result information

    Set StopNGo result system
*/
//@{
typedef struct _STOPNGO_RSLTINFO
{
    STOPNGO_WARNMSG uiStopStatus;             ///<
    STOPNGO_WARNMSG uiGoStatus;               ///<
    STOPNGO_WARNMSG uiMovingStatus;           ///<
    STOPNGO_WARNMSG uiMvValid;
} STOPNGO_RSLTINFO;

typedef struct _STOPNGO_DEBUG_INFO
{
    STOPNGO_WARNMSG uiLdStatus;
    STOPNGO_ROI     StopNGoRoiInfo;               ///<
    UINT32          uiValidBlkNum;
    UINT32          uiStopCnt;
    UINT32          uiStopTimeCnt;
    UINT32          uiVoteNum;
    UINT32          uicadNum0;
    UINT32          uicadNum1;
    UINT32          uicadNum2;
    UINT32          uicadNum3;
    UINT32          uicadNum4;
    UINT32          uiTimeDuration;
} STOPNGO_DEBUG_INFO;

/*
typedef struct _STOPNGO_DEBUG_INFO
{
    UINT32          uiValidBlkNum;
    UINT32          uiStopCnt;
    UINT32          uiStopTimeCnt;
    UINT32          uiVoteNum;
    UINT32          uicadNum0;
    UINT32          uicadNum1;
    UINT32          uicadNum2;
    UINT32          uicadNum3;
    UINT32          uicadNum4;
    UINT32          uiTimeDuration;
} STOPNGO_DEBUG_INFO;
//@}
*/

/**
    Struct StopNGo global motion of each frame

    Set StopNGo global motion of each frame
*/
//@{
typedef struct _STOPNGO_MOTVECT
{
    INT32 iGx;
    INT32 iGy;
    UINT32 uiCandNum;
} STOPNGO_MOTVECT;
//@}


/**
    DIS global motion and whole motion info

    Select global motion and whole motion info.
*/
//@{
typedef struct _STOPNGO_DIS_SGM
{
    STOPNGO_MOTVECT* pCurMv;       ///< current motion vector
    STOPNGO_MOTVECT* pVoteMv;      ///< current motion vector decided by voting
    STOPNGO_MOTVECT* pMeanShiftMv; ///< current motion vector decided by mean-shift
    STOPNGO_MOTVECT* pPrevMv;      ///< motion vector of previous frame
    MOTION_INFOR* pBlkMv;          ///< motion vectors of each block
} STOPNGO_DIS_SGM;
//@}

/**
    DIS global motion and whole motion info

    Select global motion and whole motion info.
*/
//@{
typedef struct _STOP_DEBUG_INFO
{
    UINT32 uiStopRslt;
    UINT32 uicadNum0;
    UINT32 uicadNum1;
    UINT32 uicadNum2;
    UINT32 uicadNum3;
    UINT32 uicadNum4;
    UINT32 uiTotalValidBlk;
} STOP_DEBUG_INFO;
//@}

/**
     This function is used to initialize detection buffer and set the process rate.
*/
extern INT32 StopNGo_Init(void);
//extern INT32 StopNGo_Init(MEM_RANGE *buf);

/**
     Calculate STOPNGO detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 StopNGo_calcBuffSize(void);

extern void StopNGo_Detection(MOTION_INFOR *MotResult, STOPNGO_IMGINFO FrmInfo, UINT32 ValidBlkThr, UINT32 ValidBlkThr2, UINT32 EdgeLowPassThr, UINT32 EdgeLowPassThr2, UINT32 MvThr, UINT32 Frm, UINT32 uiStopPeriod);
extern STOPNGO_RSLTINFO StopNGo_getStopNGoStatus(void);
extern STOPNGO_DEBUG_INFO StopNGo_getStopNGoDebugInfo(void);
extern STOP_DEBUG_INFO StopNGo_getStopStatus(void);
extern void Stop_Detection(MOTION_INFOR *MotResult, MDS_DIM mdsDim, UINT32 StopThr);
extern STOP_RSLT StopNGo_getStopLowPassFiltRslt(void);
extern STOP_RSLT StopNGo_getStopRslt(void);
#endif