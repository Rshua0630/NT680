
#ifndef _SCENE_RENDER_LIB_H_
#define _SCENE_RENDER_LIB_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Type.h"
#include "ErrorNo.h"
#include "Debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    Scene Render enum - processing ID
*/
typedef enum
{
    SR_WDR_PROCID1 = 0,  ///< processing ID 1
    SR_WDR_PROCID2 = 1,  ///< processing ID 2
    SR_WDR_PROCID3 = 2,  ///< processing ID 3
    SR_WDR_PROCID4 = 3,  ///< processing ID 4
    ENUM_DUMMY4WORD(SR_WDR_PROCID)
} SR_WDR_PROCID;

/*
    Scene Render enum -
*/
typedef enum
{
    SR_WDR_OFF 		= 0,	///< WDR off mode
    SR_WDR_AUTO		= 1,	///< WDR auto mode
    SR_WDR_MANUAL	= 2,	///< WDR manual modes
    ENUM_DUMMY4WORD(SR_WDR_PROC_MODE)
} SR_WDR_PROC_MODE;


/*
    Scene Render enum - change parameter selection
*/
typedef enum
{
    SR_WDR_CHG_BLEND_REF_WET	= 0x00000001,  ///< change blending weighting map selection
    SR_WDR_CHG_WDR_STRENGTH 	= 0x00000002,  ///< change wdr strength
    SR_WDR_CHG_AE_TAG_Y 		= 0x00000004,  ///< change AE target Y (post gamma)
    SR_WDR_CHG_LCE_LEVEL	 	= 0x00000008,  ///< change local contrast level
    SR_WDR_CHG_GCE_LEVEL	 	= 0x00000010,  ///< change global contrast level
    ENUM_DUMMY4WORD(SR_WDR_CHG_SEL)
} SR_WDR_CHG_SEL;


/*
    Scene Render enum - local contrast enhancement reference source selection
*/
typedef enum
{
	SR_WDR_LCE_REF_MI = 0,
	SR_WDR_LCE_REF_SI = 1,
	ENUM_DUMMY4WORD(SR_WDR_LCE_REF_SRC)
} SR_WDR_LCE_REF_SRC;
//----------------------------------------------------------

/*
    Scene Render enum - global tone curve process selection
*/
typedef enum
{
	SR_WDR_GTC_PROC_Y = 0,
	SR_WDR_GTC_PROC_BAYER = 1,
	ENUM_DUMMY4WORD(SR_WDR_GTC_PROC_FMT)
} SR_WDR_GTC_PROC_FMT;
//----------------------------------------------------------



/*
    Scene Render structure - open WDR operation
*/
typedef struct _SR_WDR_OPEN_INFO_
{
    // do nothing...
} SR_WDR_OPEN_INFO;
//----------------------------------------------------------


/*
    Scene Render structure - input sub-image info
*/
typedef struct _SR_WDR_SUBIMG_INFO_
{
    UINT8 uiSubWidth;	///< input sub-image width
    UINT8 uiSubHeight;	///< input sub-image height
    UINT8 uiSubRatio;   ///< 0: direct input;  1: transfer by image w/h ratio
} SR_WDR_SUBIMG_INFO;
//----------------------------------------------------------



/*
    Scene Render structure - Y output image information
*/
typedef struct _SR_WDR_YOUT_INFO_
{
	UINT32 uiYoutAddr;	///< Yout dram buffer address
	UINT32 uiYoutLofs;	///< Yout dram buffer lineoffset
	UINT32 uiYoutSizeH;	///< Y horizontal output window number
	UINT32 uiYoutSizeV;	///< Y vertical output window number
	UINT32 uiOB;		///< if OB subtraction is completed in the front of RHE, set this value 0; if not, set OB value
} SR_WDR_YOUT_INFO;
//----------------------------------------------------------


/*
    Scene Render structure - image blending parameters
*/
typedef struct _SR_WDR_BLEND_PARAM_
{
	UINT32 *puiBlendCurve;	///< image blend curve, 65 elements
	UINT32 uiBlendRefWet;        ///< image blend reference map weight
} SR_WDR_BLEND_PARAM;
//----------------------------------------------------------


/*
    Scene Render structure - Image blending path curves
*/
typedef struct _SR_WDR_PATH_CURVE_PARAM_
{
	UINT32	*puiCurveA;      ///< curve of path A, 33 entries
	UINT32	*puiCurveB;      ///< curve of path B, 33 entries
	UINT32	*puiCurveC;      ///< curve of path C, 33 entries
	UINT32	*puiCurveD;      ///< curve of path D, 33 entries
	UINT32	*puiCurveE;      ///< curve of path E, 33 entries
} SR_WDR_PATH_CURVE_PARAM;
//----------------------------------------------------------


/*
    Scene Render structure - Local contrast gain parameters
*/
typedef struct _SR_WDR_LCE_PARAM_
{
	UINT32 *puiDifGain;     ///< local diference gain, 17 entries
	UINT32 *puiLumaGain;    ///< local luma gain, 17 entries
	SR_WDR_LCE_REF_SRC RefSrc;     ///< luma gain reference source
} SR_WDR_LCE_PARAM;
//----------------------------------------------------------

/*
    Scene Render structure - local contrast enhancement parameters
*/
typedef struct _SR_WDR_LCE_CHG_PARAM_
{
	UINT32	uiLceLevel;		///< local contast enhancement strength level, range = 0 ~ 128
	UINT32	uiLceClamp;		///< local contast enhancement gain clamp, range = 0 ~ 255
} SR_WDR_LCE_CHG_PARAM;
//----------------------------------------------------------


/*
    Scene Render structure - global tone curve parameters
*/
typedef struct _SR_WDR_GTC_PARAM_
{
	SR_WDR_GTC_PROC_FMT	GtcProcSel;     ///< global tone curve process selection
	UINT32              *puiGtcCurve;    ///< global tone curve table, 129 entries
} SR_WDR_GTC_PARAM;
//----------------------------------------------------------


/*
    Scene Render structure - global contrast enhancement parameters
*/
typedef struct _SR_WDR_GCE_PARAM_
{
	UINT32	uiGceDarkSth;		///< global contast enhancement strength for dark region, range = 0 ~ 128
	UINT32	uiGceBrightSth;		///< global contast enhancement strength for bright region, range = 0 ~ 128
} SR_WDR_GCE_PARAM;
//----------------------------------------------------------



/*
    Scene Render structure - Input parameters of WDR computation
*/
typedef struct _SR_WDR_CAL_INFO
{
	SR_WDR_PROC_MODE	WdrProcMode;		///< WDR process mode selection
    SR_WDR_YOUT_INFO	*pYoutInfo;			///< Y output image data (pre-gamma)
    UINT32 				*puiGGammaLut;    ///< G gamma curve,  129 entries
    UINT32				uiWDRLevel;			///< WDR stength level, available only for WDR auto off, range: 0 ~ 128
} SR_WDR_CAL_INFO;
//----------------------------------------------------------


/*
    Scene Render structure - Change parameters for WDR effect
*/
typedef struct _SR_WDR_CHG_PARAMS_
{
	UINT32 uiChgSel;   ///< change item refer to "SR_WDR_CHG_SEL".  if changing mutiple items, "or" the selections

	UINT32 uiChgBlendRefWet;	///< change blending reference map weight
	UINT32 uiChgWDRUserSth;   	///< chagge WDR strength only for auto mode, range: 0 ~ 128, 64 = 1x
	UINT32 uiChgAeTagY;			///< change AE post gamma target Y,

	SR_WDR_LCE_CHG_PARAM	ChgLceParam;	///< change local contrast enhancement
	SR_WDR_GCE_PARAM		ChgGceParam;	///< change global contrast enhancement for dark and bright regions
} SR_WDR_CHG_PARAMS;
//----------------------------------------------------------


/*
    Scene Render structure - Get computed WDR parameters
*/
typedef struct _SR_WDR_PARAM
{
     SR_WDR_PATH_CURVE_PARAM	*pGetPathParam;		///< path parameters
     SR_WDR_BLEND_PARAM 		*pGetBlendParam;	///< blend parameters
     SR_WDR_LCE_PARAM 			*pGetLceParam;		///< local contrast enhancement parameters
     SR_WDR_GTC_PARAM			*pGetGtcParam;		///< global tone curve parameters

     UINT32 					*pGetYCurveParam;	///< Y-curve parameters, 129 elements
     UINT32						*pGetEdgeGamParam;	///< edge-gamma parameters, 65 elements
} SR_WDR_PARAM;

//----------------------------------------------------------

/**
    Parameter initialization

    @param[in] ProcId       process ID
    @param[in] pOpenInfo    initial parameter for opening WDR lib.

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_Init(SR_WDR_PROCID ProcId, SR_WDR_OPEN_INFO *pOpenInfo);
//----------------------------------------------------------

/**
    Parameter computation for WDR (for auto mode)

    @param[in] ProcId        process ID
    @param[in] pWdrCalInfo   parameters for computation

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_Proc(SR_WDR_PROCID ProcId, SR_WDR_CAL_INFO *pWdrCalInfo);
//----------------------------------------------------------


/**
    Get computed parameters of WDR (for auto mode)

    @param[in] ProcId          process ID
    @param[in] pGetWdrParams   get computed parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_GetParam(SR_WDR_PROCID ProcId, SR_WDR_PARAM *pGetWdrParams);
//----------------------------------------------------------


/**
    Change paremeters for WDR effects.

    @param[in] ProcId       process ID
    @param[in] pChgParams   parameters for adjustment

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_ChgParams(SR_WDR_PROCID ProcId, SR_WDR_CHG_PARAMS *pChgParams);
//----------------------------------------------------------

/**
    Get input sub-image size information

    @param[in] ProcId       process ID
    @param[in] pChgParams   parameters for adjustment

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_GetSubImgInfo(SR_WDR_PROCID ProcId, SR_WDR_SUBIMG_INFO *pGetInfo);
//----------------------------------------------------------

/**
    Get WDR Strength

    @param[in] ProcId       process ID

    @return UINT323      WDR strength,  128 = 1x\n
*/
extern UINT32 SceneRender_WDR_GetStrth(SR_WDR_PROCID ProcId);
//----------------------------------------------------------



#ifdef __cplusplus
}
#endif


#endif




