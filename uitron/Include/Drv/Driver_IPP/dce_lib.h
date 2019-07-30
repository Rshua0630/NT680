/**
    This file is the public hearder file of DCE engine, \n
    including APIs' declaration and data type's definition

    @file              dce_lib.h

    @ingroup       mIDrvIPP_DCE

    @notes           None

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _DCE_LIB_H_
#define _DCE_LIB_H_

#include "Type.h"
#include "ErrorNo.h"

//*********************************************************************************************************
/**
    @addtogroup mIDrvIPP_DCE
*/
//@{

/**
    @name Define DCE function.
*/
//@{
#define DCE_FUNC_CFA                0x00000100  ///< CFA interpolation function
#define DCE_FUNC_DC                 0x00000200  ///< Distortion correction function
#define DCE_FUNC_CAC                0x00001000  ///< Chromatic aberration correction function
#define DCE_FUNC_CROP               0x00002000  ///< Output crop function
#define DCE_FUNC_D2D_RAND           0x04000000  ///< D2D random LSB function
#define DCE_FUNC_D2D_UVFILT         0x08000000  ///< D2D UV filter function
#define DCE_FUNC_D2D_IOSTOP         0x10000000  ///< D2D UV filter function
#define DCE_FUNC_ALL                0x1C003300  ///< Chromatic aberration correction function
//@}

/**
    @name Define DCE interrput.
*/
//@{
#define DCE_INT_FRMST               0x00000001  ///< Interrupt for DCE frame start
#define DCE_INT_FRMEND              0x00000002  ///< Interrupt for DCE frame end
#define DCE_INT_STEND               0x00000004  ///< Interrupt for DCE stripe end
#define DCE_INT_STPERR              0x00000008  ///< Interrupt for DCE stripe error
#define DCE_INT_LBOVF               0x00000010  ///< Interrupt for DCE line buffer overflow error
#define DCE_INT_CACOVF              0x00000020  ///< Interrupt for DCE cac buffer overflow error
#define DCE_INT_STPOB               0x00000040  ///< Interrupt for DCE stripe boundary overflow error
#define DCE_INT_YBACK               0x00000080  ///< Interrupt for DCE Y coordinate backward skip error
#define DCE_INT_ALL                 0x000000FF  ///< All DCE interrupt
//@}

/**
    @name Define constants
*/
//@{
#define IMG_MAX_STP_NUM             64  ///< maximal stripe number of DCE output image
//@}

/**
    @name DCE enum - Driver control
*/
//@{

/**
    DCE clear flag selection
*/
typedef enum {
	DCE_FLAG_NO_CLEAR = 0,    ///< No clear flag
	DCE_FLAG_CLEAR    = 1,    ///< Clear flag
	ENUM_DUMMY4WORD(DCE_FLAG_CLEAR_SEL)
} DCE_FLAG_CLEAR_SEL;

/**
    DCE setting selection
*/
typedef enum {
	DCE_ENABLE_SEL    = 0,    ///< Enable input 1's
	DCE_DISABLE_SEL   = 1,    ///< Disable input 1's
	DCE_SET_SEL       = 2,    ///< Enable input 1's, disable input 0's
	ENUM_DUMMY4WORD(DCE_SETTING_SEL)
} DCE_SETTING_SEL;

//@}

/**
    @name DCE enum - Engine control
*/
//@{

/**
    DCE operation selection

    Select operation mode
\n Used for dce_setOpMode().
*/
typedef enum {
	DCE_OPMODE_DCE_CTRL        = 0,    ///< DCE control IPP(RHE->IFE->DCE->IPE->IME)
	DCE_OPMODE_ALL_DIRT        = 2,    ///< SIE to
	DCE_OPMODE_DCE_D2D         = 3,    ///< DCE D2D
	ENUM_DUMMY4WORD(DCE_OPMODE)
} DCE_OPMODE;

/**
    DCE stripe mode selection

    Select stripe mode
\n Used for dce_setStpMode().
*/
typedef enum {
	DCE_STPMODE_SST     = 0,  ///< DCE single stripe mode
	DCE_STPMODE_MST     = 1,  ///< DCE horizontal multiple stripe mode
	DCE_STPMODE_2ST     = 2,  ///< DCE horizontal 2 stripe mode
	DCE_STPMODE_4ST     = 3,  ///< DCE horizontal 4 stripe mode
	ENUM_DUMMY4WORD(DCE_STP_MODE)
} DCE_STP_MODE;

/**
    DCE line buffer mode selection

    Select line buffer mode
\n Used for dce_setLbufMode().
*/
typedef enum {
	DCE_LBUF_BAYER   = 0,      ///< DCE Bayer buffer mode
	DCE_LBUF_RGB     = 1,      ///< DCE RGB buffer mode
	ENUM_DUMMY4WORD(DCE_LBUF_MODE)
} DCE_LBUF_MODE;

/**
    DCE distortion correction selection

    Select distortion correction mode
\n Used for dce_setDistortCorrSel().
*/
typedef enum {
	DCE_DCSEL_GDC_ONLY              = 0,    ///< GDC/CAC only
	DCE_DCSEL_2DLUT_ONLY            = 1,    ///< 2D lut only
	DCE_DCSEL_GDC_2DLUT_DIFFSUM     = 2,    ///< GDC/CAC, 2D grid LUT difference addition
	DCE_DCSEL_GDC_2DLUT_CASCADE     = 3,    ///< 2D grid LUT cascade with GDC/CAC
	ENUM_DUMMY4WORD(DCE_DC_SEL)
} DCE_DC_SEL;

/**
    DCE D2D format selection

    Select D2D data format
\n Used for dce_setD2DFormat().
*/
typedef enum {
	DCE_D2D_YUV422IN_YUV422OUT    = 0,    ///< In/Out YUV 422 8 bit packed plane
	DCE_D2D_YUV420IN_YUV420OUT    = 1,    ///< In/Out YUV 420 8 bit packed plane
	DCE_D2D_BAYERIN_YUV422OUT     = 2,    ///< In Bayer 8 bit plane/Out YUV 422 8 bit packed plane
	ENUM_DUMMY4WORD(DCE_D2D_FMT)
} DCE_D2D_FMT;

/**
    DCE engine status

    Engine busy or idle status
\n Used for dce_setD2DFormat().
*/
typedef enum {
	DCE_BUSY    = 0,    ///< Engine is busy
	DCE_IDLE    = 1,    ///< Engine is idle
	ENUM_DUMMY4WORD(DCE_STATUS)
} DCE_STATUS;

/**
    DCE H stripe input start selection

    Select input start position control
\n Used for dce_setHstripeInStart().
*/
typedef enum {
	DCE_1ST_STP_POS_CALC    = 0,    ///< H,V Input starts from DCE calculated position
	DCE_1ST_STP_POS_ZERO    = 1,    ///< H,V Input starts from 0
	ENUM_DUMMY4WORD(DCE_STP_INST)
} DCE_STP_INST;

/**
    DCE control IME H stripe output overlap selection

    Select output overlap pixel number
\n Used for dce_setImeHStripeOutOvlp().
*/
typedef enum {
	DCE_HSTP_IMEOLAP_16    = 0,    ///< 16 pixels overlap
	DCE_HSTP_IMEOLAP_24    = 1,    ///< 24 pixels overlap
	DCE_HSTP_IMEOLAP_32    = 2,    ///< 32 pixels overlap
	DCE_HSTP_IMEOLAP_USER  = 3,    ///< user-defined value
	ENUM_DUMMY4WORD(DCE_HSTP_IMEOLAP_SEL)
} DCE_HSTP_IMEOLAP_SEL;

/**
    DCE control IPE H stripe output overlap selection

    Select output overlap pixel number
\n Used for dce_setIpeHstripeOutOvlp().
*/
typedef enum {
	DCE_HSTP_IPEOLAP_8     = 0,    ///< 4 pixels overlap
	DCE_HSTP_IPEOLAP_16    = 1,    ///< 8 pixels overlap
	ENUM_DUMMY4WORD(DCE_HSTP_IPEOLAP_SEL)
} DCE_HSTP_IPEOLAP_SEL;

//@}

/**
    @name DCE enum - Engine parameters
*/
//@{

/**
    DCE input Bayer data start CFA selection

    Select intput Bayer raw data start channel - R, Gr, Gb, B.
\n Used for dce_setCFA().
*/
typedef enum {
	DCE_CFA_PATR  = 0,        ///< Start channel is R for input Bayer raw data
	DCE_CFA_PATGR = 1,        ///< Start channel is Gr for input Bayer raw data
	DCE_CFA_PATGB = 2,        ///< Start channel is Gb for input Bayer raw data
	DCE_CFA_PATB  = 3,        ///< Start channel is B for input Bayer raw data
	ENUM_DUMMY4WORD(DCE_CFA_PAT)
} DCE_CFA_PAT;

/**
    DCE CAC method selection

    Select CAC method.
\n Used for dce_setCacSel().
*/
typedef enum {
	DCE_CAC_RGB_GAIN = 0,         ///< Correct CA with G LUT and R, G, B LUTGAIN
	DCE_CAC_RGB_LUT  = 1,         ///< Correct CA with R, G, B LUT
	ENUM_DUMMY4WORD(DCE_CAC_SEL)
} DCE_CAC_SEL;

/**
    DCE FOV boundary selection

    Select FOV boundary.
\n Used for dce_setFovBoundSel().
*/
typedef enum {
	DCE_FOV_BOUND_DUPLICATE = 0,         ///< Replace out of boundary pixels with duplicate boundary pixels
	DCE_FOV_BOUND_REG_RGB   = 1,         ///< Replace out of boundary pixels with GEO_BOUNDR, GEO_BOUNDG, GEO_BOUNDB
	ENUM_DUMMY4WORD(DCE_FOV_SEL)
} DCE_FOV_SEL;

/**
    DCE 2D lut number selection

    Select 2D lut number.
\n Used for dce_set2DlutNum().
*/
typedef enum {
	DCE_2DLUT_65_65_GRID = 0,         ///< 65x65 grid 2D lut distortion correction
	DCE_2DLUT_33_33_GRID = 1,         ///< 33x33 grid 2D lut distortion correction
	DCE_2DLUT_17_17_GRID = 2,         ///< 17x17 grid 2D lut distortion correction
	DCE_2DLUT_9_9_GRID   = 3,         ///< 9x9 grid 2D lut distortion correction
	ENUM_DUMMY4WORD(DCE_2DLUT_NUM)
} DCE_2DLUT_NUM;

//@}

/**
    @name DCE enum - DMA burst length
*/
//@{
typedef enum {
	DCE_BURST_32W = 0,   ///< 32 word DMA burst
	DCE_BURST_16W = 1,   ///< 16 word DMA burst
	ENUM_DUMMY4WORD(DCE_DMA_BURST)
} DCE_DMA_BURST;

//@}

/**
    DCE structure - DMA burst length
*/
//@{
typedef struct {
	DCE_DMA_BURST   InBurstLen;  ///< Input burst length
	DCE_DMA_BURST   OutBurstLen; ///< Output burst length
} DCE_BURST_LENGTH;
//@}

/**
    DCE structure - Image size
*/
//@{
typedef struct {
	UINT32    uiHSize;    ///< Horizontal image size, unit : pix
	UINT32    uiVSize;    ///< Vertical image size, uint : line
} DCE_IMG_SIZE;
//@}

/**
    DCE structure - Image ratio
*/
//@{
typedef struct {
	UINT32    uiXDist;    ///< Horizontal distance ratio
	UINT32    uiYDist;    ///< Vertical distance ratio
} DCE_IMG_RAT;
//@}

/**
    DCE structure - Optical center
*/
//@{
typedef struct {
	INT32    iXCent;    ///< Horizontal center, unit : pix
	INT32    iYCent;    ///< Vertical center, uint : line
} DCE_IMG_CENT;
//@}

/**
    DCE structure - Distance normalization
*/
//@{
typedef struct {
	UINT32    uiNFact;    ///< Normalization factor
	UINT32    uiNBit;     ///< Normalization bit
} DCE_DIST_NORM;
//@}

/**
    DCE structure - CAC gain
*/
//@{
typedef struct {
	UINT32    uiRLutGain;    ///< R channel lut gain
	UINT32    uiGLutGain;    ///< G channel lut gain
	UINT32    uiBLutGain;    ///< B channel lut gain
} DCE_CAC_GAIN;
//@}

/**
    DCE structure - FOV bound RGB
*/
//@{
typedef struct {
	UINT32    uiRBound;    ///< R boundary value
	UINT32    uiGBound;    ///< G boundary value
	UINT32    uiBBound;    ///< B boundary value
} DCE_FOV_RGB;
//@}

/**
    DCE structure - Interpolation enhance term
*/
//@{
typedef struct {
	UINT32    uiYEnhFact;   ///< Y enhance factor
	UINT32    uiYEnhBit;    ///< Y enhance norm bit
	UINT32    uiUVEnhFact;  ///< UV enhance factor
	UINT32    uiUVEnhBit;   ///< UV enhance norm bit
} DCE_INT_ENH;
//@}

/**
    DCE structure - 2D lut offset
*/
//@{
typedef struct {
	UINT32    uiXOfsInt;   ///< 2D lut x integer offset
	UINT32    uiXOfsFrac;  ///< 2D lut x fraction offset
	UINT32    uiYOfsInt;   ///< 2D lut y integer offset
	UINT32    uiYOfsFrac;  ///< 2D lut y fraction offset
} DCE_2DLUT_OFS;
//@}

/**
    DCE structure - 2D lut scaling factor
*/
//@{
typedef struct {
	UINT32    uiHScaleFact;  ///< 2D lut h scaling factor
	UINT32    uiVScaleFact;  ///< 2D lut v scaling factor
} DCE_2DLUT_SCALE;
//@}

/**
    DCE structure - CFA direction decision
*/
//@{
typedef struct {
	UINT32    uiDifNBit;    ///< Difference norm bit
	UINT32    uiNsMarEdge;  ///< Noise margin for edge
	UINT32    uiNsMarDiff;  ///< Noise margin for pixel difference
} DCE_CFA_DIR;
//@}

/**
    DCE structure - CFA correction term
*/
//@{
typedef struct {
	BOOL      bGCorrEn;     ///< Enable for G channel correction
	BOOL      bRBCorrEn;    ///< Enable for RB channel correction
	UINT32    uiGCorrNBit;  ///< G correction norm bit
	UINT32    uiGCorrTh;    ///< G correction noise threshold
	UINT32    uiRBCorrTh1;  ///< RB correction noise threshold1
	UINT32    uiRBCorrTh2;  ///< RB correction noise threshold2
} DCE_CFA_CORR;
//@}

/**
    DCE structure - CFA high frequency check term
*/
//@{
typedef struct {
	BOOL      bCLCheckEn;   ///< Enable cross line check
	BOOL      bHFCheckEn;   ///< Enable high frequency check
	BOOL      bAverageMode; ///< Interpolation method selection when flat region detect
	UINT32    uiHFGTh;      ///< G channel smoothness checking threshold
	UINT32    uiCLTh;       ///< Cross line checking threshold
	UINT32    uiHFDiff;     ///< Difference threshold value between Gr and Gb to determing high frequency pattern detected or not
	UINT32    uiHFEThr;     ///< Threshold to determine H or V direction high frequency pattern detected
} DCE_CFA_HFC;
//@}

/**
    DCE structure - H stripe mas increment, decrement
*/
//@{
typedef struct {
	UINT32    uiMaxInc;    ///< Max increment
	UINT32    uiMaxDec;    ///< Max decrement
} DCE_HSTP_INCDEC;
//@}

/**
    DCE structure - Output cropping
*/
//@{
typedef struct {
	UINT32    uiHSize;    ///< Horizontal image size, unit : pix
	UINT32    uiVSize;    ///< Vertical image size, uint : line
	UINT32    uiHStart;    ///< Horizontal image size, unit : pix
	UINT32    uiVStart;    ///< Vertical image size, uint : line
} DCE_OUT_CROP;
//@}

/**
    DCE structure - DCE operation parameters.
*/
//@{
typedef struct {
	DCE_OPMODE OpMode;          ///< Operation mode selection
	DCE_STP_MODE StpMode;       ///< Stripe mode selection
	DCE_LBUF_MODE LbufMode;     ///< Line buffer mode selection
} DCE_OP_PARAM;
//@}

/**
    DCE structure - DCE CFA parameters.
*/
//@{
typedef struct {
	DCE_CFA_PAT CfaPat;         ///< CFA start pattern
	DCE_CFA_DIR CfaDir;         ///< CFA direction decision
	DCE_CFA_CORR CfaCorr;       ///< CFA correction term
	DCE_CFA_HFC CfaHfc;         ///< CFA high frequency check term
} DCE_CFA_PARAM;
//@}

/**
    DCE structure - DCE GDC/CAC digital zoom parameters.
*/
//@{
typedef struct {
	DCE_IMG_RAT ImgRat;         ///< GDC/CAC image ratio setting
	DCE_DIST_NORM DistNorm;     ///< GDC/CAC distance normalization
} DCE_GDC_CAC_DZOOM_PARAM;
//@}

/**
    DCE structure - DCE GDC/CAC optical zoom parameters.
*/
//@{
typedef struct {
	DCE_CAC_SEL CacSel;         ///< CAC compensation method selection
	DCE_CAC_GAIN CacGain;       ///< CAC RGB lut gain
	UINT32 *puiGGeoLut;         ///< GDC/CAC G geo lut
	INT32 *piRGeoLut;           ///< GDC/CAC R geo lut
	INT32 *piBGeoLut;           ///< GDC/CAC B geo lut
} DCE_GDC_CAC_OZOOM_PARAM;
//@}

/**
    DCE structure - DCE FOV parameters.
*/
//@{
typedef struct {
	UINT32 uiFovGain;           ///< FOV lut gain
	DCE_FOV_SEL FovSel;         ///< FOV boundary select
	DCE_FOV_RGB FovRGB;         ///< FOV boundary RGB definition
} DCE_FOV_PARAM;
//@}

/**
    DCE structure - DCE 2D lut parameters.
*/
//@{
typedef struct {
	UINT32 ui2DLutAddr;                 ///< 2D lut DRAM address
	DCE_2DLUT_OFS TwoDLutXYOfs;         ///< 2D lut x y offset
	DCE_2DLUT_NUM TwoDLutNum;           ///< 2D lut grid number
#if (_EMULATION_ == ENABLE)
	DCE_2DLUT_SCALE TwoDLutFact; 		///< 2D lut scaling factor
#endif
} DCE_2DLUT_PARAM;
//@}

/**
    DCE structure - DCE H stripe parameters.
*/
//@{
typedef struct {
	UINT32 *puiHStripe;                 ///< H stripe0~15 setting
	DCE_HSTP_INCDEC HStpIncDec;         ///< H stripe max increment/decrement
	DCE_STP_INST HStpInStart;           ///< First H stripe input selection
	DCE_HSTP_IMEOLAP_SEL ImeOHstpOvlap; ///< IME H stripe output overlap selection
	DCE_HSTP_IPEOLAP_SEL IpeOHstpOvlap; ///< IPE H stripe output overlap selection
	UINT32 ImeOHstpOvlapUserVal;        ///< user-defined overlap value
} DCE_HSTP_PARAM;
//@}

/**
    DCE structure - DCE V stripe parameters.
*/
//@{
typedef struct {
	DCE_STP_INST VStpInStart;           ///< First V stripe input selection
} DCE_VSTP_PARAM;
//@}

/**
    DCE structure - DCE H stripe status.
*/
//@{
typedef struct {
	UINT32 uiHstpBufMode;             ///< Last H stripe buffer mode used
	UINT32 uiHstpStx;                 ///< Last H stripe starting input X coordinate
	UINT32 uiHstpEdx;                 ///< Last H stripe ending input X coordinate
	UINT32 uiHstpClmStx;              ///< Last H stripe starting input X coordinate of the first column
	UINT32 uiHstpClmEdx;              ///< Last H stripe ending input X coordinate of the first column
	UINT32 uiBufHeightG;              ///< Last H stripe vertical buffer used by G
	UINT32 uiBufHeightRGB;            ///< Last H stripe vertical buffer used by RGB
	UINT32 uiBufHeightPix;            ///< Last H stripe vertical buffer used by RGB of single pixel
} DCE_HSTP_STATUS;
//@}

/**
    DCE structure - DCE D2D parameters.
*/
//@{
typedef struct {
	DCE_D2D_FMT D2DFormat;      ///< D2D format selection
	UINT32 YInAddr;             ///< D2D Y input address
	UINT32 YInLnOfs;            ///< D2D Y input line offset
	UINT32 UVInAddr;            ///< D2D UV input address
	UINT32 UVInLnOfs;           ///< D2D UV input line offset
	UINT32 YOutAddr;            ///< D2D Y output address
	UINT32 YOutLnOfs;           ///< D2D Y output line offset
	UINT32 UVOutAddr;           ///< D2D UV output address
	UINT32 UVOutLnOfs;          ///< D2D UV output line offset
} DCE_D2D_PARAM;
//@}

/**
    DCE structure - Engine open object
*/
//@{
typedef struct {
	void (*FP_DCEISR_CB)(UINT32 uiIntStatus);   ///< External ISR call-back function
	UINT32 uiDceClockSel;                       ///< Engine clock selection
} DCE_OPENOBJ;
//@}

/**
    DCE structure - DCE mode info.
*/
//@{
typedef struct {
	UINT32 uiIntEn;                         ///< Interrupt enable selection
	UINT32 uiFuncEn;                        ///< Function enable selection
	DCE_OP_PARAM OpParam;                   ///< Engine operation parameters
	DCE_IMG_SIZE ImgInSize;                 ///< Input image size
	DCE_OUT_CROP ImgOutCrop;                ///< Output image cropping
	DCE_CFA_PARAM CfaParam;                 ///< CFA interpolation parameters
	DCE_DC_SEL DcSel;                       ///< Distortion correction combination selection
	DCE_IMG_CENT ImgCent;                   ///< GDC/CAC center setting
	DCE_GDC_CAC_DZOOM_PARAM GdcCacDzParam;  ///< GDC/CAC digital zoom parameters
	DCE_GDC_CAC_OZOOM_PARAM GdcCacOzParam;  ///< GDC/CAC optical zoom parameters
	DCE_FOV_PARAM FovParam;                 ///< FOV parameters
	DCE_2DLUT_PARAM TwoDLutParam;           ///< 2D lut parameters
	DCE_INT_ENH IntEnhParam;                ///< Interpolation enhancement parameters
	DCE_HSTP_PARAM HstpParam;               ///< H stripe parameters
	DCE_VSTP_PARAM VstpParam;               ///< V stripe parameters
	DCE_D2D_PARAM D2DParam;                 ///< D2D parameters
} DCE_MODE_INFO;
//@}

/**
    DCE structure - DCE stripe info.
*/
//@{
typedef struct {
	UINT32 uiIpeHolap;              ///< IPE Output H overlap
	UINT32 uiImeHolap;              ///< IME Output H overlap
	BOOL bImeSsrEn;                 ///< IME SSR function enable
	BOOL bImeIsdEn;                 ///< IME ISD function enable
	BOOL bImeEncEn;                 ///< enable bit of IME encode
	BOOL bIme3dnrEn;                ///< enable bit of IME 3DNR
	BOOL bImeDecEn;                 ///< enable bit of IME decode
} DCE_NormStripeInfo_660;
//@}

/**
    DCE structure - DCE stripe result.
*/
//@{
typedef struct {
	UINT32 uiGdcHstp[16];                       ///< H stripe settings for DCE engine register
	UINT32 uiMaxInc;                            ///< Max increment
	UINT32 uiMaxDec;                            ///< Max decrement
	UINT32 ui_actual_hstp_num;                  ///< H stripe number of DCE output image
	UINT32 ui_actual_hstp[IMG_MAX_STP_NUM];     ///< H stripe settings of DCE output image
} DCE_NormStripeResult_660;
//@}

/**
    DCE structure - IPP HW stripe information.
*/
//@{
typedef struct {
	UINT32 *GdcHstp;                       		///< H stripe settings for DCE engine register
	UINT32 uiDceInHSize;						///< DCE input width
	UINT32 uiDceCropHStart;						///< DCE output crop start
	UINT32 uiDceCropHSize;						///< DCE output crop width
	UINT32 uiIpeHolap;							///< IPE overlap
	UINT32 uiImeHolap;							///< IME overlap
} IPP_HW_INFO;
//@}

/**
    DCE structure - IPP output stripe information.
*/
//@{
typedef struct {
	UINT32 *output_hstp_num;                  	///< H stripe number of IPP output
	UINT32 *output_hstp;     					///< H stripe settings of IPP output
} IPP_OUT_STRP;
//@}

//----------------------------------------------------------------------------------------//
extern ER dce_getNormStripe_660(DCE_NormStripeInfo_660 *NSInfo, DCE_NormStripeResult_660 *NSResult, DCE_MODE_INFO *DceModeInfo);
extern VOID dce_get_output_stp_setting(IPP_HW_INFO *IppHwInfo, IPP_OUT_STRP *IppOutStrp);

extern ER dce_open(DCE_OPENOBJ *pOpenInfo);
extern ER dce_close(void);
extern ER dce_start(void);
extern ER dce_pause(void);
extern ER dce_setMode(DCE_MODE_INFO *ModeInfo);
extern ER dce_changeAll(DCE_MODE_INFO *ModeInfo);
extern ER dce_changeIntEn(UINT32 IntSel, DCE_SETTING_SEL SettingSel);
extern ER dce_changeFuncEn(UINT32 FuncSel, DCE_SETTING_SEL SettingSel);
extern ER dce_changeOperation(DCE_OP_PARAM *pInParam);
extern ER dce_changeImgInSize(DCE_IMG_SIZE *pInParam);
extern ER dce_changeImgOutCrop(DCE_OUT_CROP *pInParam);
extern ER dce_changeCfaInt(DCE_CFA_PARAM *pInParam);
extern ER dce_changeDistortCorrSel(DCE_DC_SEL InParam);
extern ER dce_changeImgCenter(DCE_IMG_CENT *pInParam);
extern ER dce_changeGdcCacDigiZoom(DCE_GDC_CAC_DZOOM_PARAM *pInParam);
extern ER dce_changeGdcCacOptiZoom(DCE_GDC_CAC_OZOOM_PARAM *pInParam);
extern ER dce_changeFov(DCE_FOV_PARAM *pInParam);
extern ER dce_change2DLut(DCE_2DLUT_PARAM *pInParam, DCE_IMG_SIZE *in_img_sz);
extern ER dce_changeIntEnh(DCE_INT_ENH *pInParam);
extern ER dce_changeHStripe(DCE_HSTP_PARAM *pInParam);
extern ER dce_changeVStripe(DCE_VSTP_PARAM *pInParam);
extern ER dce_changeD2DInOut(DCE_D2D_PARAM *pInParam);

extern UINT32 dce_getClockRate(void);
extern UINT32 dce_getIntEn(void);
extern UINT32 dce_getFunctionEn(void);
extern void dce_getImgSize(DCE_IMG_SIZE *pSize);
extern DCE_CFA_PAT dce_getCfaPat(void);
extern DCE_STATUS dce_getBusyStatus(void);
extern UINT32 dce_getIntStatus(void);
extern UINT32 dce_getHStpCnt(void);
extern void dce_getHStripe(DCE_HSTP_PARAM *pHStp);
extern void dce_getHstpStatus(DCE_HSTP_STATUS *pHStpStatus);
extern void dce_getBurstLength(DCE_BURST_LENGTH *pBurstLen);

extern void dce_isr(void);
extern void dce_clearInt(UINT32 uiIntr);
extern void dce_waitFlagFrameEnd(DCE_FLAG_CLEAR_SEL FlagClear);
extern void dce_clearFlagFrameEnd(void);
extern void dce_waitFlagFrameStart(DCE_FLAG_CLEAR_SEL FlagClear);
extern void dce_clearFlagFrameStart(void);

extern UINT32 g_uiFrmStartCnt;
extern UINT32 g_uiFrmEndCnt;
extern UINT32 g_uiStrpEndCnt;
extern UINT32 g_uiStrpErrCnt;
extern UINT32 g_uiLbufOvflCnt;
extern UINT32 g_uiCacBufOvflCnt;
extern UINT32 g_uiStrpOfbCnt;
extern UINT32 g_uiYbackCnt;
//@}
#endif
