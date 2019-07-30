#ifndef __NvtOt_H__
#define __NvtOt_H__

#include "SxCmd.h"
#include "Type.h"
#include "NvtCv_lib.h"

#ifdef __cplusplus
extern "C" {
#endif


#define OT_SHOW_PROFILE        1   //profile switch 0 = OFF, 1 = ON


typedef enum
{
    OT_CFG_INTERP_FACTOR,      ///< config inter_factor 0~128, the more larger number,the more fast model update
    OT_CFG_PADDINGSTYLE,       ///< config padding style ,0 for simple,1 for replicate
    OT_CFG_FRAME_INTERVALS,    ///< config frame_intervals, 1~5 advised;
    OT_CFG_TARGET,             ///< config target
    OT_CFG_ALG_VERSION,        ///< config algorithm version, 0 for SingleScale, 1 for MultiScale
    OT_CFG_FEATURE_TYPE,       ///< config feature type, 0 or 1
    OT_CFG_SEARCH_RANGE,       ///< config search range, 10~30 advised;
    OT_CFG_FRAME_OFV,          ///< config frames out of view totally, the more larger number, the more longer target wait;
    ENUM_DUMMY4WORD(OT_CFG)
} OT_CFG;

typedef struct
{
    UINT32 width;                    ///< Input Img width
    UINT32 height;                   ///< Input Img height
    UINT32 lineoffset;               ///< Input Img lineoffset
    UINT32 addr;                     ///< Input Img addr
    UINT32 imgRatio;                 ///< Input Img ratio
}OT_UIMGSIZE;

typedef enum
{
    OT_VERSION_SINGLE_SCALE = 0,
    OT_VERSION_MULTI_SCALE = 1,
    ENUM_DUMMY4WORD(OT_ALG_VERSION)
} OT_ALG_VERSION;

typedef enum
{
    OT_CELL4_BIN9 = 0,
    OT_CELL4_BIN4 = 1,
    OT_CELL8_BIN9 = 2,
    OT_CELL8_BIN4 = 3,
    ENUM_DUMMY4WORD(OT_FEATURE_TYPE)
} OT_FEATURE_TYPE;

typedef enum
{
    OT_EFFECT_DEFAULT,
	OT_EFFECT_SMALL_SRANDIF,                    ///< Small search range and small inter factor
	OT_EFFECT_LARGE_IF,                         ///< Large inter factor
	OT_EFFECT_LARGE_SR,                         ///< Large search range
    ENUM_DUMMY4WORD(OT_EFFECT_SELECTION)
} OT_EFFECT_SELECTION;

extern BOOL NvtOt_Init(MEM_RANGE *buf);

extern void OT_SetTarget(IRECT Target);
extern BOOL OT_SetImgSize( OT_UIMGSIZE* pInputImgSize);
extern void OT_SetConfig(OT_CFG config_id, INT32 Value);
extern void OT_SetDSP(NVTCV_SENDTO DspSendTo);
extern void OT_SetEFFECT(OT_EFFECT_SELECTION EffectSelection);


extern UINT32 OT_GetTarget(URECT* Target, URECT* TargetCoord);
extern OT_UIMGSIZE OT_GetInputImgBuf(void);
extern UINT32 OT_GetConfig(OT_CFG config_id);
extern NVTCV_SENDTO OT_GetDSP(void);
extern OT_EFFECT_SELECTION OT_GetEFFECT(void);
extern BOOL OT_PROCESS_STA(VOID);  //TRUE = END, FALSE = DOING; Default = TRUE
extern BOOL OT_TRK_STA(VOID);      //TRUE = Good, FALSE = Error; Default = TRUE

extern UINT32 OT_START(VOID);
extern UINT32 OT_END(VOID);

extern UINT32 OT_CalcCacheBuffSize(void);
extern UINT32 OT_CalcBuffSize(void);


extern VOID NvtCv_ot_process(VOID);

VOID OT_SetShowProfileState(BOOL flag);
BOOL OT_GetShowProfileState(VOID);


#ifdef __cplusplus
}
#endif

#endif
