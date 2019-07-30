/**
    ADAS detection library.

    @file       adas_Apps.h
    @ingroup    mILibADAS

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _ADAS_APPS_H
#define _ADAS_APPS_H

#include "Type.h"
#include "ldws_lib.h"
#include "fcws_lib.h"
#include "ime_lib.h"
#include "ipl_cmd.h"
#include "SnG_lib.h"
#include "mIType.h"
#include "autoVP_alg.h"
#include "ipl_utility.h"
#include "GxImage.h"


/**
     @name ADASreturn status
*/
//@{
#define ADAS_STA_OK                  (0)              ///<  the status is ok, for normal case
#define ADAS_STA_ERROR               (-1)             ///<  some error occurred
//@}


/**
     ADAS detection callback message.
*/
typedef enum _ADAS_CBMSG
{
    ADAS_CBMSG_WARNNING = 0,       ///< user will receive this notification when the lane departure warning occur
    ADAS_CBMSG_SNG = 1,            ///< sng callback
    ADAS_CBMSG_MAX_CNT,            ///< total callback message numbers
    ENUM_DUMMY4WORD(ADAS_CBMSG)
} ADAS_CBMSG;

/**
    TASK ID
*/
typedef enum _ADAS_ID
{
    ADAS_HIGHPRIOAPPS_ID = 0,       ///< user will receive this notification when the lane departure warning occur
    ADAS_MIDPRIOAPPS_ID = 1,            ///< sng callback
    ADAS_LOWPRIOAPPS_ID = 2,
    ENUM_DUMMY4WORD(ADAS_ID)
} ADAS_ID;

/**
     ADAS event
*/
typedef enum _ADAS_CBEVENT
{
    ADAS_CBEVENT_UPDATE_CROP_WINDOW = 0,
    ADAS_CBEVENT_WRITE_LOG,
    ADAS_CBEVENT_FCLD_RLT,
    ADAS_CBEVENT_SNG_RLT,
    ADAS_CBEVENT_MAX_CNT,
    ENUM_DUMMY4WORD(ADAS_CBEVENT)
} ADAS_CBEVENT;

typedef enum _ADAS_FCWS_RSLT_FORMAT
{
	ADAS_FCWS_RSLT_LANE_WIDTH = 0,
	ADAS_FCWS_RSLT_CAR_WIDTH,
	ENUM_DUMMY4WORD(ADAS_FCWS_RSLT_FORMAT)
} ADAS_FCWS_RSLT_FORMAT;

// User data test, KCHong
typedef struct _ADAS_LOG
{
    UINT32 LogDataSize;     // Only the size of LogData!!!
    UINT32 LogData;
} ADAS_LOG, *PADAS_LOG;
/**
     ADAS Applications list
*/
typedef enum _ADAS_ALLPRIOAPPS
{
    ADAS_MIDPRIOAPPS_LDWS = 1,           ///< LDWS
    ADAS_MIDPRIOAPPS_FCWS,               ///< FCWS
    ADAS_HIGHPRIOAPPS_SNG,               ///< SNG
    ADAS_HIGHPRIOAPPS_STOPDECT,          ///< STOP DETECTION
    ADAS_LOWPRIOAPPS_AVP,                ///< AVP
    ENUM_DUMMY4WORD(ADAS_ALLPRIOAPPS)
} ADAS_ALLPRIOAPPS;

/**
     debug info for Low priority apps
*/
typedef struct _ADAS_LOWPRIOAPPS_LOG_INFO
{
    ADAS_ALLPRIOAPPS    AppsType;            ///< App type
    AUTO_VP_RESULT_INFO AutoVPRslt;          ///< auto vp result
    AUTO_VP_DEBUG_INFO  AutoVPDebugInfo;     ///< auto vp debug info
} ADAS_LOWPRIOAPPS_LOG_INFO;

typedef struct _ADAS_LOWPRIOAPPS_LOG
{
    UINT32                      LogDataSize;     ///< Only the size of LogData!!!
    ADAS_LOWPRIOAPPS_LOG_INFO LogData;         ///< log data
} ADAS_LOWPRIOAPPS_LOG;

/**
     debug info for middle priority apps
*/
typedef struct _ADAS_MIDPRIOAPPS_LOG_INFO
{
    ADAS_ALLPRIOAPPS         AppsType;      ///< App type
    LDWS_INIT_PARMS          LdwsIniParms;
    APPS_LDWS_RESULT_INFO    LdwsRslt;      ///< LDWS result
    LDWS_DEBUG_INFO          LdwsDebugInfo; ///< LDWS debug info
    ADAS_ALLPRIOAPPS         AppsType2;     ///< App type
    APPS_FCWS_RESULT_INFO    FcwsRslt;      ///< FCWS result
    APPS_FCWS_DEBUG_INFO     FcwsDebugInfo; ///< FCWS debug info
    APPS_FCWS_ALG_INFO       FcwsAlgDebugInfo;
} ADAS_MIDPRIOAPPS_LOG_INFO;

/**
     debug struct for middle priority apps
*/
typedef struct _ADAS_MIDPRIOAPPS_LOG
{
    UINT32                    LogDataSize;     ///< Only the size of LogData
    ADAS_MIDPRIOAPPS_LOG_INFO LogData;         ///< log data
} ADAS_MIDPRIOAPPS_LOG;

/**
     debug info for high priority apps
*/
typedef struct _ADAS_HIGHPRIOAPPS_LOG_INFO
{
    ADAS_ALLPRIOAPPS      AppsType;           ///< App type
    STOPNGO_RSLTINFO      SnGRslt;            ///< SNG result
    STOPNGO_DEBUG_INFO    SnGDebugInfo;       ///< SNG debug info
    ADAS_ALLPRIOAPPS      AppsType2;
    STOP_RSLT             StopDectRslt;
    STOP_RSLT             StopDectFiltRslt;
    STOP_DEBUG_INFO       StopDectDebugInfo;
} ADAS_HIGHPRIOAPPS_LOG_INFO;

/**
     debug struct for high priority apps
*/
typedef struct _ADAS_HIGHPRIOAPPS_LOG
{
    UINT32                     LogDataSize;     ///< Only the size of LogData!!!
    ADAS_HIGHPRIOAPPS_LOG_INFO LogData;         ///< log data for high priority apps
} ADAS_HIGHPRIOAPPS_LOG;

typedef UINT32 (*ADASLIB_CALLBACK_PTR)(ADAS_CBEVENT EventID, void *Data);
//#NT#2016/02/17#KCHong -end

//@{
/**
    ADAS result
*/
typedef struct _ADAS_APPS_RESULT_INFO
{
    APPS_LDWS_RESULT_INFO    LdwsRsltInfo;  ///<  LDWS   result
    APPS_FCWS_RESULT_INFO    FcwsRsltInfo;  ///<  FCWS   result
    STOPNGO_RSLTINFO         SnGInfo;       ///<  SNG    result
    AUTO_VP_RESULT_INFO      AutoVPRsltInfo;///<  AutoVP result
} ADAS_APPS_RESULT_INFO;

/**
    SNG ROI Info
*/
typedef struct _ADAS_SNG_ROI_INFO
{
    UINT32     uiP1x;
    UINT32     uiP1y;
    UINT32     uiP2x;
    UINT32     uiP2y;
    UINT32     uiP3x;
    UINT32     uiP3y;
    UINT32     uiP4x;
    UINT32     uiP4y;
} ADAS_SNG_ROI_INFO;

/**
     input data for LDWS.
*/
typedef struct _LDWS_SRCDATA_CROP_INFO
{
    UINT32 uiLdwsCropSx;             ///<  start x coordinate of crop region
    UINT32 uiLdwsCropSy;             ///<  start y coordinate of crop region
} LDWS_SRCDATA_CROP_INFO;

typedef struct _ADAS_FUNC_EN
{
    BOOL bLdwsFuncEn;
    BOOL bFcwsFuncEn;
    BOOL bAutoVPFuncEn;
    BOOL bSnGFuncEn;
    BOOL bStopDectEn;
} ADAS_FUNC_EN;
/**
    source data for ADAS
*/
typedef struct _ADAS_SRCDATA_INFO
{
    LDWS_SRCDATA_INFO   LdwsSrcDataInfo;  ///< src data of LDWS
    FCWS_SRCDATA_INFO   FcwsSrcDataInfo;  ///< src data of FCWS
    AUTO_VP_SRCDATA_INFO AutoVPSrcDataInfo;   ///< src data of AutoVP
} ADAS_SRCDATA_INFO;


/**
    LDWS callback funcion proto type
*/
typedef void (*ADAS_CBMSG_FP)(ADAS_CBMSG uiMsgID, void *Data);

/**
     Calculate ADAS detection (for for high priority apps) required buffer size.

     @return the required buffer size.
*/
extern UINT32 ADAS_CalcHighPrioAppsBuffSize(void);
/**
     Calculate ADAS detection (for middle priority apps) required buffer size.

     @return the required buffer size.
*/
extern UINT32 ADAS_CalcMidPrioAppsBuffSize(void);
/**
     Calculate ADAS detection (for low priority apps) required buffer size.

     @return the required buffer size.
*/
extern UINT32 ADAS_CalcLowPrioAppsBuffSize(void);
/**
     Calculate ADAS detection (for low priority apps) required cache buffer size.

     @return the required cache buffer size.
*/
extern UINT32 ADAS_CalcLowPrioAppsCacheBuffSize(void);
/**
     Init ADAS Detect engine (for High priority apps).
*/
extern INT32 ADAS_HighPrioAppsInit(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 uiProcessRate, UINT32 uiProcessPeriod);
/**
     Init ADAS Detect engine (for middle priority apps).
*/
extern INT32 ADAS_MidPrioAppsInit(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 uiProcessRate, UINT32 uiProcessPeriod);
/**
     Init ADAS Detect engine (for low priority apps).
*/
extern INT32 ADAS_LowPrioAppsInit(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 uiProcessRate, UINT32 uiProcessPeriod);
/**
     Process the ADAS detection (for high priority apps).
*/
extern INT32 ADAS_HighPrioAppsProcess(void);
/**
     Process the ADAS detection (for middle priority apps).
*/
extern INT32 ADAS_MidPrioAppsProcess(void);
/**
     Process the ADAS detection (for low priority apps).
*/
extern INT32 ADAS_LowPrioAppsProcess(void);
/**
    LDWS enable
*/
extern INT32 ADAS_AppsSetLdwsEanble(BOOL bkey);
/**
    FCWS enable
*/
extern INT32 ADAS_AppsSetFcwsEanble(BOOL bkey);
/**
    SNG enable
*/
extern INT32 ADAS_AppsSetSnGEanble(BOOL bkey);
/**
    Stop Detection enable
*/
extern INT32 ADAS_AppsSetStopDectEanble(BOOL bkey);
/**
    AutoVP enable
*/
extern INT32 ADAS_AppsSetAutoVPEanble(BOOL bkey);
/**
    Stop ADAS
*/
extern void ADAS_AppsStop(void);
/**
    Stop high priority apps
*/
extern void ADAS_HighPrioAppsStop(void);
/**
    Stop middle priority apps
*/
extern void ADAS_MidPrioAppsStop(void);
/**
    Stop low priority apps
*/
extern void ADAS_LowPrioAppsStop(void);
/**
    ADAS Enable
*/
extern void ADAS_AppsStart(void);
/*
    Get ADAS Status
*/
extern BOOL ADAS_GetAppsStatus(void);
/**
    Get IME path 4 information
*/
extern IPL_IME_PATH_INFO ADAS_getLdFcPath4Info(void);
/**
    Set save flag
*/
extern void SetSaveDisMvFlg(UINT32 uiflag);
/**
    ADAS detection for high prio apps
*/
extern INT32 ADAS_HighPrioAppsDetection(void);
/**
    ADAS detection for middle prio apps
*/
extern INT32 ADAS_MidPrioAppsDetection(void);
/**
    ADAS detection for low prio apps
*/
extern INT32 ADAS_LowPrioAppsDetection(void);


//////////////////////////// ADAS APIs for user  ///////////////////////////////////////
/**
    Get High priority apps processing rate (10 times average)
*/
extern UINT32 ADAS_GetHighAvgProcessTime(void);
/**
    Get Middle priority apps processing rate (10 times average)
*/
extern UINT32 ADAS_GetMidAvgProcessTime(void);
/**
    Get Low priority apps processing rate (10 times average)
*/
extern UINT32 ADAS_GetLowAvgProcessTime(void);
/**
    Get High priority apps processing period
*/
extern UINT32 ADAS_GetHighPrioAppsProcesPeriod(void);
/**
    Get Middle priority apps processing period
*/
extern UINT32 ADAS_GetMidPrioAppsProcesPeriod(void);
/**
    Get Low priority apps processing period
*/
extern UINT32 ADAS_GetLowPrioAppsProcesPeriod(void);
/**
    Get ADAS Apps status
*/
extern ADAS_FUNC_EN ADAS_GetAdasAppsStatus(void);
/**
    Get OSD debug flag status
*/
extern UINT32 ADAS_IsAdasDebug(void);
/**
    Set ADAS Vanishing point
*/
extern void ADAS_SetAdasVPInfo(UPOINT uPoint);
/**
    Get ADAS Current Vanishing point
*/
extern UPOINT ADAS_GetCurrVP(void);
/**
    Set High priority apps warning time (resource check)
*/
extern void ADAS_SetHighPrioAppsWargTime(UINT32 uiTime);
/**
    Set High priority apps stop time (resource check)
*/
extern void ADAS_SetHighPrioAppsStopTime(UINT32 uiTime);
/**
    Set Middle priority apps warning time (resource check)
*/
extern void ADAS_SetMidPrioAppsWargTime(UINT32 uiTime);
/**
    Set Middle priority apps stop time (resource check)
*/
extern void ADAS_SetMidPrioAppsStopTime(UINT32 uiTime);
/**
    Set Low priority apps warning time (resource check)
*/
extern void ADAS_SetLowPrioAppsWargTime(UINT32 uiTime);
/**
    Set Low priority apps stop time (resource check)
*/
extern void ADAS_SetLowPrioAppsStopTime(UINT32 uiTime);
/*
    Sel Data Src for High Priority Apps
*/
extern INT32 ADAS_selHighPrioAppsDataSrc(IPL_PROC_ID IplSrc);
/*
    Sel Data Src for Middle Priority Apps
*/
extern INT32 ADAS_selMidPrioAppsDataSrc(IPL_PROC_ID IplSrc);
/*
	Sel Data Src for Low Priority Apps
*/
extern INT32 ADAS_selLowPrioAppsDataSrc(IPL_PROC_ID IplSrc);
/**
    Set flag for dump debug data
*/
extern void ADAS_SetDumpDebugDataFlag(BOOL bLdFckey, BOOL bSnGkey, BOOL bStopDectkey, BOOL bAvpkey);

/////////////// LDWS ///////////////////////
/**
    Set Left Sensitivity
*/
extern void ADAS_SetLdwsLeftSensitivity(UINT32 uiVal);
/**
    Get Left Sensitivity
*/
extern UINT32 ADAS_GetLdwsLeftSensitivity(void);
/**
    Set Right Sensitivity
*/
extern void ADAS_SetLdwsRightSensitivity(UINT32 uiVal);
/**
    Get Right Sensitivity
*/
extern UINT32 ADAS_GetLdwsRightSensitivity(void);
/**
    Set Height (cm) parameter
*/
extern void ADAS_SetLdwsHs(UINT32 uiVal);
/**
    Get Height (cm) parameter
*/
extern UINT32 ADAS_GetLdwsHs(void);
/**
    Set LDWS failure time
*/
extern void ADAS_SetFailureTime(UINT32 uiVal);
/**
    Get LDWS failure time
*/
extern UINT32 ADAS_GetFailureTime(void);
/**
    Set LDWS debounce time
*/
extern void ADAS_SetLdwsDebounceTime(UINT32 uiVal);
/**
    Get LDWS debounce time
*/
extern UINT32 ADAS_GetLdwsDebounceTime(void);

/////////////// FCWS ///////////////////////
/**
    Set Fsize Value
*/
extern void ADAS_SetLdFcFsizeValue(UINT32 uiVal);
/**
    Get Fsize Value
*/
extern UINT32 ADAS_GetLdFcFsizeValue(void);
/**
    Set Fsize Value
*/
extern void ADAS_SetLdFcFsizeValueByPixel(UINT32 uiVal);
/**
    Get Fsize Value
*/
extern UINT32 ADAS_GetLdFcFsizeValueByPixel(void);
/**
    Set FCWS warning dist
*/
extern void ADAS_SetFcwsUIWarningDist(UINT32 uiVal);
/**
    Get FCWS warning dist
*/
extern UINT32 ADAS_GetFcwsUIWarningDist(void);
/**
    Set FCWS debounce time
*/
extern void ADAS_SetFcwsDebounceTime(UINT32 uiVal);
/**
    Get FCWS debounce time
*/
extern UINT32 ADAS_GetFcwsDebounceTime(void);
/**
    Get FCWS histogram size
*/
extern UINT32 ADAS_GetFcwsHistSz(void);
/**
    Get FCWS edge map size
*/
extern UINT32 ADAS_GetEdgeMapSz(void);
/**
    Get FCWS current distance
*/
extern UINT32 ADAS_GetFcwsCurrentDist(void);
/**
    Set FCWS result output format
*/
extern void ADAS_SetFcwsCarRsltFmt(ADAS_FCWS_RSLT_FORMAT uiSel);
/**
    Get FCWS result output format
*/
extern UINT32 ADAS_GetFcwsCarRsltFmt(void);

/////////////// SNG ///////////////////////
/**
    Set SNG stop period
*/
extern void ADAS_SetSnGStopPeriod(UINT32 uiVal);
/**
    Get SNG stop period
*/
extern UINT32 ADAS_GetSnGStopPeriod(void);
/**
    Get Stop detection result (Filtered)
*/
extern STOP_RSLT ADAS_GetStopDectRslt(void);

extern void ADAS_InstallID(void) _SECTION(".kercfg_text");

///////////////////////////////////////////
/**
    ADAS callback function
*/

extern BOOL ADAS_GetNewCropWindow(PURECT CropWin);

extern INT32 ADAS_GetLdFcRslt(ADAS_APPS_RESULT_INFO *pADASResult, URECT* pTargetCoord);

extern INT32 ADAS_GetSnGRslt(ADAS_APPS_RESULT_INFO *pADASResult, URECT* pTargetCoord);

extern INT32 ADAS_GetSnGRoi(ADAS_SNG_ROI_INFO *pSnGRoiInfo, URECT* pTargetCoord);

extern UINT32 ADAS_GetSnGLog(UINT32 *puiRslt);

extern UINT32 ADAS_GetLdFcLog(UINT32 *puiRslt);

extern UINT32 ADAS_GetAutoVPLog(UINT32 *puiRslt);

///////////////////////////////////////////
extern INT32 ADAS_MidPrioAppsCopySrcDataInt(IMG_BUF *pInputImg);
extern INT32 ADAS_LowPrioAppsCopySrcDataInt(IMG_BUF *pInputImg);
///////////////////////////////////////////
/**
    ADAS callback
*/
extern void ADASLIB_RegCB(ADASLIB_CALLBACK_PTR fpADASLIBCB);
extern void ADAS_InstallCmd(void);         // Uart Cmd
extern void   ADAS_AppsRegisterCB(ADAS_CBMSG_FP CB);
typedef void (*ADAS_MOVIELOG_FP)(UINT32 uiId, char* Buf, UINT32 uiBufLen);
void ADAS_SetAppsMovieLogFP(ADAS_MOVIELOG_FP fp);
void ADAS_GetAppsLogString(UINT32 uiId, char* Buf, UINT32 uiBufLen);


#define FLGADAS_IPPDONE            FLGPTN_BIT(0)       //0x00000001
#define FLGADAS_SHOWRSLTDONE       FLGPTN_BIT(1)       //0x00000002
#define FLGADAS_SAVEDEBUGDATADONE  FLGPTN_BIT(2)       //0x00000004
#define FLGADAS_ADASSTOPADONE      FLGPTN_BIT(3)       //0x00000008
#define FLGADAS_AVPUNLOCKGRAPH     FLGPTN_BIT(4)       //0x00000010
#define FLGADAS_AVPIPCMSG          FLGPTN_BIT(5)       //0x00000020
extern UINT32 _SECTION(".kercfg_data") g_uiFLG_ID_ADAS;

#endif

