/**
    Auto Exposure api.


    @file       ae_api.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_API_H_
#define _AE_API_H_
#include "ipl_alg_infor.h"
#include "iq_ui_info.h"
#include "ae_ui_info.h"
#include "ae_alg.h"

typedef enum _AE_APPMODE {
	AE_APPMODE_NORMAL = 0,
	AE_APPMODE_STITCH,
	AE_APPMODE_SHDR,
	AE_APPMODE_END = 0xffffffff,
	ENUM_DUMMY4WORD(AE_APPMODE)
} AE_APPMODE;

typedef struct _AE_LAINFO {
	UINT32 WinNumX;
	UINT32 WinNumY;
} AE_LAINFO;

typedef struct {
	UINT32 ExpT;
	UINT32 Gain;
    UINT32 GainRatio;
	UINT32 LumaAverage;
    UINT32 RawAverage;
	UINT16 *LumaValue;
	UINT16 *LumaHistogram;
} AEStatus;

/**
     AE process event.
     @note
*/
typedef struct _AE_EVENT_TAB {
	//AE_Process event
	UINT32(*AeProcessEnd)(UINT32 Id, UINT32 ISO, UINT32 ExposureTime);

} AE_EVENT_TAB;


typedef void (*AE_SETIRCUT_FP)(UINT32 Id, IQ_IRCUT Value);
void AE_SetIRCutFP(AE_SETIRCUT_FP fp);

/**

     load event table

     @note

     @param[in] event table(AE_EVENT_TAB)
     @return
         - @b void.
*/
extern void AE_LoadEventTAB(AE_EVENT_TAB *EventTab);

/**

     ae init function

     @note

     @param[in] process id
     @param[in] current status
     @return
         - @b void.
*/
extern void AE_Init(UINT32 Id, UINT32 CurrentStatus);


/**

     ae process function
     @note

     @param[in] process id
     @param[in] current status
     @return
         - @b void.
*/
extern void AE_Process(UINT32 Id, UINT32 CurrentStatus);

/**

     ae get preview ae arg.

     @note

     @param[in] process id
     @param[out] iso
     @param[out] exposure time
     @param[out] iris
*/
extern void AE_GetPrvAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);

/**

     ae get capture ae arg.

     @note

     @param[in] process id
     @param[out] iso
     @param[out] exposure time
     @param[out] iris
*/
extern void AE_GetCapAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);

/**

     ae set capture ae arg.

     @note

     @param[in] process id
*/
extern void AE_SetCapAEArg(UINT32 Id);

/**

     ae set capture ae arg.

     @note

     @param[in] process id
*/
extern void AE_SetCapAEArg_MFHDR(UINT32 Id, UINT32 MFHDR_Frm);

/**

     ae set preview ae arg.

     @note

     @param[in] process id
*/
extern void AE_SetPrvAEArg(UINT32 Id);

/**

     ae wait stable rough, for off2prv speed up using

     @note

     @param[in] process id
     @param[in] stable count
*/
extern void AE_WaitStableRF(UINT32 Id, UINT32 StableCnt);

/**

     ae write debug information

     @note

     @param[in] process id
     @param[in] buffer start address
     @return
         - @b buffer end address
*/
extern UINT32 AE_WriteDebugInfo(UINT32 Id, UINT32 Addr);

/**
    get preview ae lv value
    @note

    @param[in] process id
    @return
        - @b LV value : environment value*10(ex: LV5=50, LV6=60 ...)
*/
extern UINT32 AE_GetPrvLVValue(UINT32 Id);

/**
     get capture ae lv value
     @note

     @param[in] process id
     @return
        - @b LV value : environment value*10(ex: LV5=50, LV6=60 ...)
*/
extern UINT32 AE_GetCapLVValue(UINT32 Id);

/**
    get preview ae ev value
    @note

    @param[in] process id
    @return
        - @b Prv EV value
*/
extern UINT32 AE_GetPrvEVValue(UINT32 Id);

/**
     get capture ae ev value
     @note

     @param[in] process id
     @return
        - @b Cap EV value
*/
extern UINT32 AE_GetCapEVValue(UINT32 Id);

/**
     start anti-flicker detection alg

     @note
     @param[in] process id
     @return
         - @b void.
*/
extern void AE_StartAFD(UINT32 Id);

/**
     ae wait preview stable
     @note

     @param[in] process id
     @param[in] StableCnt : wait stable count
     @param[in] TimeOutFrame : time out frame
     @return
         - @b TRUE: ae already stable
         - @b FALSE: ae not stable and time out
*/
extern BOOL AE_Wait_Stable(UINT32 Id, UINT32 StableCnt, UINT32 TimeOutFrame);

/**

     ae get capture ae arg.

     @note

     @param[in] process id
     @param[out] iso
     @param[out] exposure time (us)
     @param[out] Aperture : Current Fno (*10)
     @param[out] MaxApture : Max Fno (*10)
*/
void AE_GetExifInfo(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Aperture, UINT32 *MaxApture);


/**
     Get lum ratio form AEB steps
     @note

     @param[in] process id
     @param[in] AEB_Step : EV steps for adjusting capture lum
     @return
         - @b EV adjust ratio
*/
extern UINT32 AEB_GetRatioFromEV(UINT32 Id, AE_EV AEB_Step);

/**
     Set EV adjust ratio to capture
     @note

     @param[in] process id
     @param[in] Ratio : EV adjust ratio
     @return
         - @b none
*/
extern void AEB_SetRatio2Cap(UINT32 Id, UINT32 Ratio);


/**
     Get current raw luminmance
     @note

     @param[in] process id
     @param[in] LumBit : return data's bit
     @return
         - @b Luminmance : The average lum of raw image
*/
extern UINT32 AE_GetCurRawLum(UINT32 Id, UINT32 LumBit);


/**
     depend on frame cnt, to get ae setting
     @note

     @param[in] process id
     @param[in] frame cnt
     @param[out] ae data
     @return
         - @b E_OK, E_SYS
*/
extern ER AE_GetSyncInfo(UINT32 Id, UINT32 frame_cnt, AEALG_ARG *data);

/**

     get ae window

     @note
     @param[in] process id
     @return
         - @b none

*/
extern void AE_GetWin(UINT32 Id, UINT8 *win);

/**

     get LA window number

     @note
     @param[in] process id
     @param[out] LA info.
     @return
         - @b none

*/
extern void AE_GetLAInfo(UINT32 Id, AE_LAINFO *LAInfo);

/**
     ae get IR-Cut setting
     @note

     @param[in] process id

     @return
         - @b IR_CUT_ENABLE
*/
extern UINT32 AE_GetPrvOverExpAdjValue(UINT32 Id);

/**
     ae Set AE Mode
     @note

     @param[in] AE App mode, data type #AE_APPMODE

     @return
         - @b none
*/
extern BOOL AE_GetIRCutEnable(UINT32 Id);

/**
     ae get LV boundary
     @note

     @param[in] process id

     @return
         - @b LV H/L Boundary
*/
extern AE_BOUNDARY AE_GetLVBoundary(UINT32 Id);

/**
     ae Set p line information
     @note

     @param[in] process id
     @param[in] P line set information

     @return
         - @b none
*/
extern void AE_SetPLINEInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo);

/**
     ae Get Status
     @note

     @param[in] process id
     @param[out] AEStatus

     @return
         - @b none
*/
extern void AE_GetStatus(UINT32 Id, AEStatus *pAEStatus);

/**
     ae Set external parameters
     @note

     @param[in] process id
     @param[in] external setting parameters

     @return
         - @b none
*/
extern void AE_SetExtInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo);

/**
     ae Get external parameters
     @note

     @param[in] process id
     @param[out] external setting parameters

     @return
         - @b none
*/
extern void AE_GetExtInfo(UINT32 Id, AE_EXT_SET_INFO *SetInfo);

/**
     ae get shdr ev ratio
     @note
     @param[in] process id
     @param[out] SHDR_EV_RATIO_INFO

     @return
         - @b none
*/
SHDR_EV_RATIO_INFO ae_get_shdr_ev_ratio(UINT32 id);
void ae_param_init(UINT32 id, UINT32* init);
#endif //_AE_API_H_
