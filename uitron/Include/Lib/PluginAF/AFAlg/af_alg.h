/**
    Public header file for AF algorithm.

    This file is the header file that define the API and data type for AF algorithm.

    @file       af_alg.h
    @ingroup    mILibAFAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _AF_ALG_H_
#define _AF_ALG_H_

#include "Type.h"
#include "ipl_cmd.h"


/**
    constant define
*/
#define AF_PROC_CNT             2

#define AF_WIN_HW_X             16
#define AF_WIN_HW_Y             16
#define AF_WIN_HW_NUM           (AF_WIN_HW_X * AF_WIN_HW_Y)
#define AF_WIN_ALG_W            8
#define AF_WIN_ALG_H            8
#define AF_WIN_ALG_NUM          (AF_WIN_ALG_W * AF_WIN_ALG_H)
#define AF_DEFAULT_WIN          (((AF_WIN_ALG_H/2) * AF_WIN_ALG_W) + (AF_WIN_ALG_W/2))


/**
    macro definition
*/
#define AFALG_ABS(X)        (((X) < 0) ? -(X) : (X))
#define AFALG_MAX(A,B)      (((A) > (B)) ? (A) : (B))
#define AFALG_MIN(A,B)      (((A) < (B)) ? (A) : (B))
#define AFALG_DIFF(A,B)     (((A) > (B)) ? ((A)-(B)) : ((B)-(A)))
#define AFALG_CLAMP(X,L,H)  (((X) > (H)) ? (H) : (((X) < (L)) ? (L) : (X)))


typedef enum {
    AF_PRNT_DBG_INFO         = 0x00000001,
    AF_PRNT_DBG_VA           = 0x00000002,
    AF_PRNT_DBG_PERF_MEASURE = 0x00000004,
    ENUM_DUMMY4WORD(AF_PRNT_DBG_MSG)
} AF_PRNT_DBG_MSG;


/**
    @name AF algorithm operation result.

     Status code to indicate condition.
*/
typedef enum {
    AFALG_STS_SUCCESS      = 0x01,    ///< AF result: success
    AFALG_STS_MULTI_TARGET = 0x02,    ///< AF result: multi target
    AFALG_STS_NO_TARGET    = 0x04,    ///< AF result: no target
    AFALG_STS_LOW_VA       = 0x08,    ///< AF result: low VA
    AFALG_STS_FLAT_CURVE   = 0x10,    ///< AF result: too flat
    AFALG_STS_TIME_OUT     = 0x20,    ///< AF result: time out
    AFALG_STS_NO_MOTOR     = 0x40,    ///< AF result: no motor driver exist
    AFALG_STS_FAILURE      = 0x80,    ///< AF result: failure
    ENUM_DUMMY4WORD(AFALG_STATUS_RESULT)
} AFALG_STATUS_RESULT;

typedef enum {
    AFALG_WINDOW_CENTER = 0,    ///< AF single window
    AFALG_WINDOW_MULTI,         ///< AF multi windows
    AFALG_WINDOW_MANUAL,        ///< AF manual windows
    ENUM_DUMMY4WORD(AFALG_WINDOW_SEL)
} AFALG_WINDOW_SEL;

typedef enum {
    AFALG_STOP = 0,    ///< AF process status: Stop - initial state
    AFALG_RUN,         ///< AF process status: Run  - AF is running
    AFALG_END,         ///< AF process status: End  - AF is finished
    AFALG_QUIT,        ///< AF process status: Quit by UI
    ENUM_DUMMY4WORD(AFALG_STATUS)
} AFALG_STATUS;

typedef enum {
    AFALG_CFG_METHOD = 0,       ///< AF algorithm method
    AFALG_CFG_STEP_SIZE,        ///< AF algorithm search step size
    AFALG_CFG_THRS,             ///< AF algorithm threshold of switching to next flow
    AFALG_CFG_TIME_OUT_NUM,     ///< AF algorithm time out number
    AFALG_CFG_MAX_ITEM,         ///< total number of AF algorithm parameter
    ENUM_DUMMY4WORD(AFALG_CFG_ITEM)
} AFALG_CFG_ITEM;

typedef enum {
    AF_MODE_ONESHOT = 0,
    AF_MODE_CONTINUOUS,
    AF_MODE_UNKNOWN,
    ENUM_DUMMY4WORD(AF_MODE)
} AF_MODE;

typedef enum {
    AF_TUNING_SET_ENABLE = 0,
    AF_TUNING_SET_METHOD,
    AF_TUNING_SET_SHOT_MODE,
    AF_TUNING_SET_SEARCH_DIR,
    AF_TUNING_SET_SKIP_FRAME,
    AF_TUNING_SET_THRES,
    AF_TUNING_SET_STEP_SIZE,
    AF_TUNING_SET_MAX_SEARCH_CNT,
    AF_TUNING_SET_RETRIGGER,
    AF_TUNING_SET_NULL,
    AF_TUNING_SET_WIN_WEIGHT,
    AF_TUNING_SET_DBG_MSG,
    AF_TUNING_SET_VA_PARAM,
    AF_TUNING_SET_ALL,
    AF_TUNING_SET_MAX,
    ENUM_DUMMY4WORD(AF_TUNING_SET_ITEM)
} AF_TUNING_SET_ITEM;

typedef enum {
    AF_TUNING_GET_ENABLE = 0,
    AF_TUNING_GET_METHOD,
    AF_TUNING_GET_SHOT_MODE,
    AF_TUNING_GET_SEARCH_DIR,
    AF_TUNING_GET_SKIP_FRAME,
    AF_TUNING_GET_THRES,
    AF_TUNING_GET_STEP_SIZE,
    AF_TUNING_GET_MAX_SEARCH_CNT,
    AF_TUNING_GET_VA,
    AF_TUNING_GET_STATUS,
    AF_TUNING_GET_WIN_WEIGHT,
    AF_TUNING_GET_DBG_MSG,
    AF_TUNING_GET_VA_PARAM,
    AF_TUNING_GET_ALL,
    AF_TUNING_GET_MAX,
    ENUM_DUMMY4WORD(AF_TUNING_GET_ITEM)
} AF_TUNING_GET_ITEM;

typedef enum {
    AF_TUNING_UPDATE_ENABLE         = FLGPTN_BIT(AF_TUNING_SET_ENABLE),
    AF_TUNING_UPDATE_METHOD         = FLGPTN_BIT(AF_TUNING_SET_METHOD),
    AF_TUNING_UPDATE_SHOT_MODE      = FLGPTN_BIT(AF_TUNING_SET_SHOT_MODE),
    AF_TUNING_UPDATE_SEARCH_DIR     = FLGPTN_BIT(AF_TUNING_SET_SEARCH_DIR),
    AF_TUNING_UPDATE_SKIP_FRAME     = FLGPTN_BIT(AF_TUNING_SET_SKIP_FRAME),
    AF_TUNING_UPDATE_THRES          = FLGPTN_BIT(AF_TUNING_SET_THRES),
    AF_TUNING_UPDATE_STEP_SIZE      = FLGPTN_BIT(AF_TUNING_SET_STEP_SIZE),
    AF_TUNING_UPDATE_MAX_SEARCH_CNT = FLGPTN_BIT(AF_TUNING_SET_MAX_SEARCH_CNT),
    AF_TUNING_UPDATE_WIN_WEIGHT     = FLGPTN_BIT(AF_TUNING_SET_WIN_WEIGHT),
    AF_TUNING_UPDATE_DBG_MSG        = FLGPTN_BIT(AF_TUNING_SET_DBG_MSG),
    AF_TUNING_UPDATE_VA_PARAM       = FLGPTN_BIT(AF_TUNING_SET_VA_PARAM),
    AF_TUNING_UPDATE_ALL            = FLGPTN_BIT_ALL
} AF_TUNING_UPDATE;

typedef enum {
    AF_TUNING_CMD_SUCCESS = 0,
    AF_TUNING_CMD_NOT_READY,
    AF_TUNING_CMD_UNKNOWN_ITEM,
    AF_TUNING_CMD_SIZE_MISMATCH,
    AF_TUNING_CMD_ID_ERROR,
    ENUM_DUMMY4WORD(AF_TUNING_CMD_STATUS)
} AF_TUNING_CMD_STATUS;


typedef struct _AF_THRES {
    int thres_rough;
    int thres_fine;
    int thres_final;
    int thres_restart;
} AF_THRES;

typedef struct _AF_TUNING_THRES {
    UINT32 id;
    UINT32 size;
    AF_THRES thres;
} AF_TUNING_THRES;

typedef struct _AF_STEP {
    int step_rough;
    int step_fine;
    int step_final;
} AF_STEP;

typedef struct _AF_TUNING_STEP {
    UINT32 id;
    UINT32 size;
    AF_STEP step;
} AF_TUNING_STEP;

typedef struct _AF_TUNING_VA {
    UINT32 id;
    UINT32 size;
    UINT32 va_data[AF_WIN_HW_Y][AF_WIN_HW_X];
} AF_TUNING_VA;

typedef struct _AF_VA {
    UINT32 va_acc[AF_WIN_HW_Y][AF_WIN_HW_X];
} AF_VA;

typedef struct _AF_TUNING_ALL {
    AF_TUNING_THRES af_tuning_thres;
    AF_TUNING_STEP af_tuning_step;
    UINT8 af_win_weight[AF_WIN_ALG_NUM];
} AF_TUNING_ALL;

typedef struct {
    UINT32 uiTuneAfEnable;
    UINT32 uiTuneMethod;
    UINT32 uiTuneShotMode;
    UINT32 uiTuneSearchDir;
    INT32  iTuneSkipFrame;
    INT32  iTuneThres[4];
    INT32  iTuneStep[3];
    INT32  iTuneMaxSearchCnt;
    UINT32 uiTuneResultStatus;
    UINT8  ucTuneWinWeight[AF_WIN_ALG_NUM];
    UINT32 uiTuneDbgMsg;
    UINT32 uiTuneVaParam[2];
} AF_TUNING_PARAM_ALL;

typedef struct {
    INT32 iAfAlgParam[4];
} AFALG_SET_PARAM;

typedef struct {
    BOOL bAfEnable;

    AFALG_WINDOW_SEL eWindowCfg;
    AF_MODE eShotMode;
    AFALG_STATUS eExecuteStatus;

    BOOL bFaceFocusEnable;
    UINT32 uiFaceDetNum;
    UINT8 ucWindowWeight[AF_WIN_ALG_NUM];
    AFALG_STATUS_RESULT uiResultStatus;

    UINT32 uiBlkPixelNum;

    // move range
    INT32 iInitPos;
    INT32 iMinPos;
    INT32 iMaxPos;

    // local variables
    UINT32 uiPerfTime;

    // debug messages
    UINT32 uiDbgMsg;

    // VA related parameters
    UINT32 uiVaParam[2];
} AF_SUB_INFO;


typedef INT32  (*AF_FOCUS_DOACTION_CBFP)(UINT8 category, INT32 target_pos);
typedef INT32  (*AF_FOCUS_GETPOSITION_CBFP)(void);
typedef INT32  (*AF_FOCUS_GETRANGE_CBFP)(void);
typedef UINT8  (*AF_ZOOM_GETSECTION_CBFP)(UINT8 category);
typedef UINT32 (*AF_LENS_GETSTATE_CBFP)(void);
typedef UINT32 (*AF_AE_GETLUM_CBFP)(UINT32 id);
typedef void   (*AF_TUNE_SETVAPARAM_CBFP)(UINT16 thr_low, UINT16 thr_high);


extern void NVTAFALG_SetAfInfoPtr(UINT32 id, AF_SUB_INFO *paf_sub_info);

extern void NVTAFALG_RegFocusDoActionCB(AF_FOCUS_DOACTION_CBFP cb_func);
extern void NVTAFALG_RegFocusGetPositionCB(AF_FOCUS_GETPOSITION_CBFP cb_func);
extern void NVTAFALG_RegFocusGetRangeCB(AF_FOCUS_GETRANGE_CBFP cb_func);
extern void NVTAFALG_RegZoomGetSectionCB(AF_ZOOM_GETSECTION_CBFP cb_func);
extern void NVTAFALG_RegLensGetStateCB(AF_LENS_GETSTATE_CBFP cb_func);
extern void NVTAFALG_RegAeGetLumCB(AF_AE_GETLUM_CBFP cb_func);
extern void NVTAFALG_RegVaSetParamCB(AF_TUNE_SETVAPARAM_CBFP cb_func);

extern void NVTAFALG_SetAlgParam(UINT32 id, AFALG_CFG_ITEM item, AFALG_SET_PARAM set_param);
extern AFALG_SET_PARAM NVTAFALG_GetAlgParam(UINT32 id, AFALG_CFG_ITEM item);
extern void NVTAFALG_ResetInfo(UINT32 id);
extern void NVTAFALG_Init(UINT32 id);
extern void NVTAFALG_Process(UINT32 id);
extern void NVTAFALG_WriteDebugInfo(UINT32 id, UINT32 *addr);


// AF tuning API
extern UINT32 AFTuning_Set_Cmd(UINT32 id, AF_TUNING_SET_ITEM item, UINT32 addr, UINT32 size);
extern UINT32 AFTuning_Get_Cmd(UINT32 id, AF_TUNING_GET_ITEM item, UINT32 addr, UINT32 size);

void AFTuning_GetPtr(UINT32 id, UINT32 addr);
void AFTuning_SetInit(UINT32 id, UINT32 addr, UINT32 size);
void AFTuning_GetUpdate(UINT32 id, UINT32 addr);
void AFTuning_SetUpdate(UINT32 id, UINT32 up);


#endif    // _AF_ALG_H_


