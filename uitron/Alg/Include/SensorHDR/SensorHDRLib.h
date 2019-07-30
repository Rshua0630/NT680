
#ifndef _SENSOR_HDR_LIB_H_
#define _SENSOR_HDR_LIB_H_


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


#define SHDR_MAXFRAMES    4

//----------SHDR Ctrl parameters----------------------------
typedef struct {
	UINT32       uiLpf[10];   ///< low pass filter A/B/C/D/E/F/G/H/I/J
	UINT8       uiLpfSize;   ///< low pass filter size, 0:3x3; 1:5x5; 2:7x7
	UINT32      uiLpfNormM;  ///< low-pass-filter normalization factor M. No need to configure.
	UINT32      uiLpfNormS;  ///< low-pass-filter normalization factor S. No need to configure.
} SHDR_REF_LA_LPF_INFO;

typedef struct {
	UINT32 uiGblTnTbl[129];  ///< global tone table, 129taps x 12bits
} SHDR_GLOBAL_TONE_INFO;

typedef struct {
	UINT32 uiBldCvTbl[65];   ///< global tone table,  65taps x  8bits
} SHDR_BLEND_CURVE_INFO;

typedef struct {
	SHDR_BLEND_CURVE_INFO BlendCurve;
    UINT8    uiRefMapWeight; ///< reference map weight for blending, 8bits
} SHDR_BLEND_INFO;

typedef struct {
	UINT32 uiPathCvTbl[33];  ///< path curve table,   33taps x  12bits
} SHDR_CURVE_INFO;

typedef struct {
	UINT32 uiPathWeiTbl[17];  ///< path curve weight table,   17taps x  8bits
} SHDR_WEIGHT_INFO;

typedef struct {
	SHDR_CURVE_INFO Curve[5];      ///< path a, b, c, d, e
	SHDR_WEIGHT_INFO Weight[3];  ///< path b, c, d
} SHDR_PATH_CURVE_INFO;

//--- Local contrast gain parameters
typedef enum
{
	RHE_LCE_REF_MI = 0,
	RHE_LCE_REF_SI = 1,
	ENUM_DUMMY4WORD(RHE_LCE_REF_SRC)
} RHE_LCE_REF_SRC;
//----------------------------------------------------------
typedef struct _SHDR_LCE_PARAM_
{
	UINT32 puiDifGain[17];     ///< local diference gain, 17 entries
	UINT32 puiLumaGain[17];    ///< local luma gain, 17 entries
	RHE_LCE_REF_SRC RefSrc;     ///< luma gain reference source
} SHDR_LCE_PARAM;
//----------------------------------------------------------

typedef struct {
	SHDR_REF_LA_LPF_INFO     RefLaLpfInfo;    ///<
	SHDR_PATH_CURVE_INFO     PathCurveInfo;   ///<
	SHDR_GLOBAL_TONE_INFO    GblTnInfo;       ///<
	SHDR_BLEND_INFO          BlendInfo;       ///<
    SHDR_LCE_PARAM 			 LceParam;		///< local contrast enhancement parameters
    UINT32 					*pGetYCurveParam;	///< Y-curve parameters, 129 elements
} SHDR_PARAM;

typedef enum
{
	SHDR_CTRL_byIQPara = 0,
	SHDR_CTRL_byHDRLib = 1,
	ENUM_DUMMY4WORD(SHDR_CTRL_MODE)
} SHDR_CTRL_MODE;

typedef enum _SHDR_BLEND_MODE {
	SHDR_BLEND_MODE_BLEND  = 0,
	SHDR_BLEND_MODE_FRAME1 = 1,
	SHDR_BLEND_MODE_FRAME2 = 2,
	SHDR_BLEND_MODE_FRAME3 = 3,
	SHDR_BLEND_MODE_FRAME4 = 4,
	SHDR_BLEND_MODE_FRAME5 = 5,
	ENUM_DUMMY4WORD(SHDR_BLEND_MODE)
} SHDR_BLEND_MODE;

typedef struct  _SHDR_IQ_PARA{
    UINT16   uiBCTbl[10];   ///< global tone table,[0~4] X0~4, [5~9] Y0~4   X:0~255 Y:0~255
    UINT8    uiRefMapWeight;///< reference map weight for blending, 8bits
    UINT8    uiBCELevel;    // Bright Contrast Enance level 0~32, default 16
    SHDR_CTRL_MODE    uiCtrlMode;    // 0:HDR by IQ, 1:HDR by HDRLib
} SHDR_IQ_PARA;

typedef struct  _SHDR_TEST_PARA{
    UINT8    SHDR_TestMode;          // Test Mode 0: Disable, 1:Enable
    SHDR_BLEND_MODE   uiFrameMode;   // 0:blending, 1:Long, 3:Mid, 5:short frame
} SHDR_TEST_PARA;


//----------SHDR UI info     ----------------------------
typedef enum {
	SHDR_HDR_MODE       = 0, ///< Staggered Mode
	SHDR_LINEAR_MODE    = 1, ///< Linear in SHDR Mode, show long expo frame only
	ENUM_DUMMY4WORD(SHDR_MODE_SELECTION)
} SHDR_MODE_SELECTION;


typedef struct _SHDR_UI_INFO {
	SHDR_MODE_SELECTION  shdrMode;  ///<
	UINT8   uiHDRLevel;             ///< 8bits: 0 = linear; 255: high; 128: default
} SHDR_UI_INFO;

//----------SHDR Cal info     ----------------------------
typedef struct {
	UINT16 uiEVratio[SHDR_MAXFRAMES];    ///< SHDR EV ratio, 16=1x, 4 frames, 0 = no frame
} SHDR_EV_RATIO_INFO;

//----------- Y output image information subImage from SIE
typedef struct _SHDR_YOUT_INFO_
{
	UINT32 uiYoutAddr;	///< Yout dram buffer address
	UINT32 uiYoutLofs;	///< Yout dram buffer lineoffset
	UINT32 uiYoutSizeH;	///< Y horizontal output window number
	UINT32 uiYoutSizeV;	///< Y vertical output window number
	UINT32 uiOB;		///< if OB subtraction is completed in the front of RHE, set this value 0; if not, set OB value
} SHDR_YOUT_INFO;

typedef struct _SHDR_CAL_INFO {
	SHDR_EV_RATIO_INFO  evRatioR;   ///< Real Expect EV ratio, 4 frames
	UINT32  uiAELV;                 ///< AE current LV
	UINT32  uiAEExpectLumRaw;       ///< AE.ExpectLum.Raw
	UINT32  uiAECurrentLumRaw;      ///< AE.CurrentLum.Raw
	SHDR_IQ_PARA  shdrIQpara;
    SHDR_YOUT_INFO	*pYoutInfo[SHDR_MAXFRAMES];		///< Y output image data (from SIE)
} SHDR_CAL_INFO;


//----------SHDR Open info     ----------------------------
typedef struct _SHDR_OPEN_INFO {
	UINT8 uiHDRframeNo;                  ///< 0: 1frames; 1: 2 frames; 2: 3 frames; 3: 4 frames
	SHDR_EV_RATIO_INFO  evRatioExpect;   ///< expect evRatio
	SHDR_IQ_PARA  shdrIQpara;
} SHDR_OPEN_INFO;

typedef struct {
    UINT8    uiSubWidth;
    UINT8    uiSubHeight;
    UINT8    uiSubRatio; // 0: direct input;  1: transfer by image w/h ratio
} SHDR_SUBIMG_INFO;



//----------------------------------------------------------
extern  VOID SensorHDR_SetAeRatio(SHDR_EV_RATIO_INFO evRatio); ///< Only for AE init set


extern SHDR_EV_RATIO_INFO SensorHDR_GetAeRatio(VOID);

extern VOID SensorHDR_SetTestMode(SHDR_TEST_PARA testPara);

extern SHDR_TEST_PARA SensorHDR_GetTestMode(VOID);

//----------------------------------------------------------


extern ER SensorHDR_Init(SHDR_OPEN_INFO *pOpenInfo);
//----------------------------------------------------------


extern ER SensorHDR_SetUI(SHDR_UI_INFO *pHdrUI);
//----------------------------------------------------------


extern ER SensorHDR_Proc(SHDR_CAL_INFO *pHdrCalInfo);
//----------------------------------------------------------


extern VOID SensorHDR_GetParam(SHDR_PARAM *pHdrParams);
//----------------------------------------------------------

extern SHDR_SUBIMG_INFO SensorHDR_GetSubImgInfo(VOID);
//----------------------------------------------------------


//----------------------------------------------------------

#ifdef __cplusplus
}
#endif


#endif




