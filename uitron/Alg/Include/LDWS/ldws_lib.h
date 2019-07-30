 /**
    LDWS detection library.

    @file       ldws_lib.h
    @ingroup    mILibLDWS

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _LDWS_LIB_H
#define _LDWS_LIB_H

#include "autoVP_alg.h"

/**
    @addtogroup mILibLDWS
*/

/**
     LDWS return status
*/
//@{
#define LDWS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define LDWS_STA_ERROR               (-1)                  ///<  some error occurred
//@}

/**
*/
//@{
typedef enum
{
    LDWS_DEPARTURE_NONE  = 0,     ///<  no departure
    LDWS_DEPARTURE_LEFT  = 1,     ///<  left departure
    LDWS_DEPARTURE_RIGHT = 2      ///<  right departure
} LDWS_DEPARTURE_DIR;
//@}

/**
*/
//@{
typedef enum
{
    LDWS_FAILURE_TRUE   = 1,      ///<  in failure mode
    LDWS_FAILURE_FALSE = 0        ///<  Not in failure mode
} LDWS_FAILURE_STATUS;
//@}

/**
*/
//@{
typedef enum
{
    LDWS_DETECT_SINGLE = 0,       ///<  lane is not detected in previous frame
    LDWS_DETECT_CONTINUE = 1      ///<  lane is detected in previous frame
} LDWS_DETECT_STATUS;
//@}

/**
  LDWS initial parameters structure
*/
//@{
typedef struct _LDWS_INIT_PARMS
{
    INT32  iInitRow;                 ///<  initial row of detection, the (0,0) is on left right
    INT32  iLsensitivity;            ///<  warning sensitivity of left lane
    INT32  iRsensitivity;            ///<  warning sensitivity of right lane
    UINT32 uiHs;                     ///<  distance from camera to ground of car
    INT32  iFsizeDq2;                ///<  this value affect the warning timing, it can be adjusted according to the value of focal length, ex: focal length = 6mm, Fsize_DQ2 =100, focal length = 12mm, Fsize_DQ2 = 200
    UINT32 uiCamera2CarDistanceCm;   ///<  distance from camera to front of the car
} LDWS_INIT_PARMS;
//@}


//@{
typedef struct _LDWS_SRCDATA_INFO
{
    UINT32 uiLdwsSrcDataAddr;
    UINT32 uiLdwsWorkBufAddr;
    UINT32 uiLdwsSrcDataWidth;
    UINT32 uiLdwsSrcDataHeight;
    UINT32 uiLdwsSrcDataRoiWidth;
    UINT32 uiLdwsSrcDataRoiHeight;
} LDWS_SRCDATA_INFO;
//@}

/**
     Auto-Vanishing point adjustment parameters of LDWS.
*/
//@{
typedef struct _LDWS_AUTOVP_INFO
{
    AutoVP_point         AutoVPRslt;         ///< vanishing point predict by Auto-VP
    UINT32               uiVPYRslt;
    BOOL                 AutoVPUpdate;
} LDWS_AUTOVP_INFO;
//@}

/**
                 uiLdSrcWidth
 ----------------------------------------------------
 - - (uiLdRoiSx, uiLdRoiSy)                         -
 -   -                                              -
 -     ------------uiLdRoiWidth------------------   -
 -     -                                        -   -
 -     -   (uiLx1,uiLy1)*    * (uiRx1,uiRy1)    -   -
 -     -                          uiLdRoiHeight -   - uiLdSrcHeight
 -     -                                        -   -
 -     -(uiLx2,uiLy2)*           * (uiRx2,uiRy2)-   -
 -     ------------------------------------------   -
 -                                                  -
 -                                                  -
 ----------------------------------------------------
*/
//@{
typedef struct _APPS_LDWS_RESULT_INFO
{
    LDWS_FAILURE_STATUS      Failure;       ///<  Failure mode
    LDWS_DEPARTURE_DIR       DepartureDirVideo;  ///<  departure direction
    LDWS_DEPARTURE_DIR       DepartureDirSound;  ///<  departure direction
    LDWS_AUTOVP_INFO         AutoVpParms;   ///<  auto-vp adjustmnet info
    UINT32                   uiLx1;          ///<  pt1's x coordinate of left lane
    UINT32                   uiLy1;          ///<  pt1's y coordinate of left lane
    UINT32                   uiLx2;          ///<  pt2's x coordinate of left lane
    UINT32                   uiLy2;          ///<  pt2's y coordinate of left lane
    UINT32                   uiRx1;          ///<  pt1's x coordinate of right lane
    UINT32                   uiRy1;          ///<  pt1's y coordinate of right lane
    UINT32                   uiRx2;          ///<  pt2's x coordinate of right lane
    UINT32                   uiRy2;          ///<  pt2's y coordinate of right lane
} APPS_LDWS_RESULT_INFO;
//@}

typedef struct _LDWS_FAILURE_INFO
{
    UINT32                   uiFailureC;
    UINT32                   uiFailureS;
    UINT32                   uiFailureCounter;
    UINT32                   uiDelTime;
    UINT32                   uiBeginPT;
} LDWS_FAILURE_INFO;

typedef struct _LDWS_ALG_INFO
{
    UINT32                   Mode;
    UINT32                   IsFindIniPoint;
    UINT32                   uiFindIniPointLen;
    UINT32                   uiFindIniPointRow;
    UINT32                   uiFindIniPointX1L;
    UINT32                   uiFindIniPointX2R;
    UINT32                   uiFindBothIniDistanceIW;
    UINT32                   uiFindBothIniDistanceIE;
    UINT32                   uiBegRowIsNULL;
    UINT32                   uiLSize;
    UINT32                   uiRSize;
    UINT32                   uiLastCurL;
    UINT32                   uiLastCurR;
    UINT32                   uiLdwsPredVPx;
    UINT32                   uiLdwsPredVPy;
    UINT32                   uiEnerDrawLine;
    UINT32                   uiLineCoefLeftRow[20];
    UINT32                   uiLineCoefLeftX[20];
    UINT32                   uiLeftLineCnt;
    UINT32                   uiLineCoefRightRow[20];
    UINT32                   uiLineCoefRightX[20];
    UINT32                   uiRightLineCnt;
    UINT32                   uiLdwsOKStus;
    UINT32                   uiOKt;
    UINT32                   uiLDWwaringframe;
    UINT32                   uiImagePT;
    UINT32                   uiWarningBias;
    UINT32                   uiRoadI;
    UINT32                   uiCarI;
    UINT32                   uiCarBias;
    UINT32                   uiIniRowPixelL;
    UINT32                   uiIniRowPixelR;
    UINT32                   uiCarWid;
} LDWS_ALG_INFO;

typedef struct _LDWS_DEBUG_INFO
{
    LDWS_FAILURE_INFO        LdwsFailInfo;
    LDWS_ALG_INFO            LdwsAlgInfo;
} LDWS_DEBUG_INFO;




/**
    LDWS information for FCWS detection
*/
typedef struct _LDWS_INFOFORFCWS
{
    LDWS_DETECT_STATUS Detect;            ///<  detection status of LDWS
    UINT32             uiFailure;         ///<  enter failure mode
    INT32              iLeftLaneP1;       ///<  coefficient 1 of left lane
    INT32              iLeftLaneP2;       ///<  coefficient 2 of left lane
    INT32              iRightLaneP1;      ///<  coefficient 1 of right lane
    INT32              iRightLaneP2;      ///<  coefficient 2 of right lane
    INT32              iVanishingX;       ///<  x coordinate of current vanishing point
    INT32              iVanishingY;       ///<  y coordinate of current vanishing point
    INT32              iNewEuslope;       ///<  new euslope updated by Auto-VP alg.
} LDWS_INFOFORFCWS;

/**
    Set initial parameters into LDWS Lib.
*/
extern void LDWS_setInitParms(LDWS_INIT_PARMS *LDWSSysParamter);

/**
    LDWS Detection function
*/
extern UINT32 LDWS_Detection(LDWS_SRCDATA_INFO *LdwsSrcDataInfo, APPS_LDWS_RESULT_INFO *LDWS_INFO, LDWS_DEBUG_INFO *LdwsDebugInfo, UINT32 uiFailureTime);

extern UINT32 LDWS_checkInitFlag(void);
extern void LDWS_setInitFlag(UINT32 Flag);
extern LDWS_INFOFORFCWS LDWS_getLdwsInfoForFcws(void);
extern  LDWS_INFOFORFCWS LDWS_getldinfo2fcws(void);


#endif

