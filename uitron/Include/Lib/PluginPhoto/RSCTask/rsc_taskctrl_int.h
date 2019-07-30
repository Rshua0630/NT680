/**
    IPL_Ctrl module(Rolling shutter correction control, FW trig flow only)

    enable/disable/configure/trig control rolling shutter correction correction

    @file       IPL_Ctrl_RSC_Int.h
    @ingroup    mIIPL_Ctrl
    @note       none

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _IPL_RSC_INT_H_
#define _IPL_RSC_INT_H_
#define MAX_GD_CNT               32
//#include "IPL_Hal.h"
#include "rsc.h"
#include "ipl_utility.h"
#include "sensor.h"
#include "ipl_alg_infor.h"
#include "ipl_hal_dce_info.h"
#include "ipl_ctrl_infor.h"
#include "gyro_drv.h"
#include "ipl_hal_op.h"
#include "rsc_obj.h"

#define RSC_SENSOR_FRM_UNIT 1000
#define RSC_LASTBUF_NUM 2

/** \addtogroup mIIPL_Ctrl */
//@{
/**
     enum of RSC CTRL
*/
typedef enum {
	_RSC_OPEN,
	_RSC_CLOSE,
	_RSC_CTRL,
	ENUM_DUMMY4WORD(ENUM_RSCCTRL),
} ENUM_RSCCTRL;
/**
     enum of RSC STATUS
*/
typedef enum _RSC_CTRL_STATUS {
	RSC_FIRSTINIT      = 0x00000001,    ///< STATUS RSC FIRST INIT.
	RSC_FIRSTRUN       = 0x00000002,    ///< STATUS RSC FIRST RUN.
	RSC_UIFRAMECOUNT   = 0x00000004,    ///< STATUS RSC UIFRAMECOUNT .
	RSC_OPENED         = 0x00000008,    ///< STATUS RSC OPEN .
	RSC_ACT            = 0x00000010,    ///< STATUS RSC_ACT (STARTED OR NOT START YET).
	RSC_ACTDURATION    = 0x00000020,    ///< STATUS RSC_ACTDURATION  (ON ACT DURATION OR NOT).
	ENUM_DUMMY4WORD(RSC_CTRL_STATUS)
} RSC_CTRL_STATUS;
/**
     enum of RSC DBG STATUS
*/
typedef enum _RSC_DBGMSG_STATUS {
	RSC_DBGMSG_FLOW		= 0x00000001,   ///< STATUS RSC_DBGMSG(SHOW DEBUG MASSGE).
	RSC_DBGMSG_DEF_2D   = 0x00000002,   ///< STATUS RSC_DBGMSG_DEF_2D (USE DEFAULT 2D LUT).
	RSC_DBGMSG_PARA_CAP = 0x00000004,   ///< STATUS RSC_DBGMSG_PARA_CAP (SHOW CAP PARAMETERS).
	RSC_DBGMSG_PARA_PRV = 0x00000008,   ///< STATUS RSC_DBGMSG_PARA_PRV (SHOW PRV PARAMETERS).
	ENUM_DUMMY4WORD(RSC_DBGMSG_STATUS)
} RSC_DBGMSG_STATUS;
/**

    Gyro data format.
    temporal def, this should be from Gyro driver
*/
typedef enum {
	_INT32,    ///< as signed 32 bits integer
	_INT16,    ///< as signed 16 bits integer
	ENUM_DUMMY4WORD(RSCCTRL_GD_FMT)
} RSCCTRL_GD_FMT;


typedef enum {
	_AXIS_X,      ///< Yaw,   as view horizontal motion, with head-toe axis
	_AXIS_Y,      ///< Pitch, as view vertical   motion, with ear-ear axis
	_AXIS_Z,      ///< Roll,  as view rotation         , with front-back axis
	_AXIS_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(RSCCTRL_GD_AXIS)
} RSCCTRL_GD_AXIS;

typedef enum {
	_DIR_POS,      ///< positive direction, X: right, Y: up  , Z: clockwise
	_DIR_NEG,      ///< negative direction, X: left , Y: down, Z: counter-clockwise
	ENUM_DUMMY4WORD(RSCCTRL_GD_DIR)
} RSCCTRL_GD_DIR;

typedef enum {
	_RSC_EIS = 0,   ///< RSC on,  EIS on
	_RSC_ONLY,      ///< RSC on,  EIS off
	_EIS_ONLY,      ///< RSC off, EIS on
	_NULL,          ///< RSC off, EIS off
	_ADJ_TBL,       ///< function as previous adjust table
	_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(RSCCTRL_FUNC_SELEC)
} RSCCTRL_FUNC_SELEC;

/**
    DCE distortion correction selection

    Select distortion correction mode
\n Used for dce_setDistortCorrSel().
*/
typedef enum {
	_GDC_ONLY              = 0,    ///< GDC/CAC only
	_2DLUT_ONLY            = 1,    ///< 2D lut only
	_GDC_2DLUT_DIFFSUM     = 2,    ///< GDC/CAC, 2D grid LUT difference addition
	_GDC_2DLUT_CASCADE     = 3,    ///< 2D grid LUT cascade with GDC/CAC
	ENUM_DUMMY4WORD(RSCCTRL_DCE_DC_SEL)
} RSCCTRL_DCE_DC_SEL;

/**
    DCE CAC method selection

    Select CAC method.
\n Used for dce_setCacSel().
*/
typedef enum {
	_RGB_GAIN = 0,         ///< Correct CA with G LUT and R, G, B LUTGAIN
	_RGB_LUT  = 1,         ///< Correct CA with R, G, B LUT
	ENUM_DUMMY4WORD(RSCCTRL_DCE_CAC_SEL)
} RSCCTRL_DCE_CAC_SEL;

/**
    DCE FOV boundary selection

    Select FOV boundary.
\n Used for dce_setFovBoundSel().
*/
typedef enum {
	_FOV_BOUND_DUPLICATE = 0,         ///< Replace out of boundary pixels with duplicate boundary pixels
	_FOV_BOUND_REG_RGB   = 1,         ///< Replace out of boundary pixels with GEO_BOUNDR, GEO_BOUNDG, GEO_BOUNDB
	ENUM_DUMMY4WORD(RSCCTRL_DCE_FOV_SEL)
} RSCCTRL_DCE_FOV_SEL;


typedef enum {
	RSCCTRL_MODE_NORMAL = 0,       ///< Video
	RSCCTRL_MODE_CAP,        ///< Still (for still-capture)
	RSCCTRL_MODE_MAX_CNT,
	ENUM_DUMMY4WORD(RSCCTRL_MODE_SELECTION)
} RSCCTRL_MODE_SELECTION;

typedef enum _RSC_CTRL_PROC_ID {
	RSC_CTRL_ID_1 = 0,                       ///< process id 0
	RSC_CTRL_ID_2,                           ///< process id 1
	RSC_CTRL_ID_3,                           ///< process id 2
	RSC_CTRL_ID_4,                           ///< process id 3
	RSC_CTRL_ID_5,                           ///< process id 5
	RSC_CTRL_ID_6,                           ///< process id 6
	RSC_CTRL_ID_7,                           ///< process id 7
	RSC_CTRL_ID_8,                           ///< process id 8
	RSC_CTRL_ID_MAX_NUM,
	ENUM_DUMMY4WORD(RSC_CTRL_PROC_ID)
} RSC_CTRL_PROC_ID;
/**
    @name DCE struct - Optical center
*/
//@{
typedef struct {
	INT32    iXCent;    ///< Horizontal center, unit : pix
	INT32    iYCent;    ///< Vertical center, uint : line
} RSCCTRL_DCE_IMG_CENT;
//@}

/**
    @name DCE struct - Image ratio
*/
//@{
typedef struct {
	UINT32    uiXDist;    ///< Horizontal distance ratio
	UINT32    uiYDist;    ///< Vertical distance ratio
} RSCCTRL_DCE_IMG_RAT;
//@}

/**
    @name DCE struct - Distance normalization
*/
//@{
typedef struct {
	UINT32    uiNFact;    ///< Normalization factor
	UINT32    uiNBit;     ///< Normalization bit
} RSCCTRL_DCE_DIST_NORM;
//@}

/**
    DCE structure - DCE GDC/CAC digital zoom parameters.
*/
//@{
typedef struct {
	RSCCTRL_DCE_IMG_RAT ImgRat;         ///< GDC/CAC ratio setting
	RSCCTRL_DCE_DIST_NORM DistNorm;     ///< GDC/CAC distance normalization
} RSCCTRL_DZOOM_PARAM;
//@}

/**
    @name DCE struct - CAC gain
*/
//@{
typedef struct {
	UINT32    uiRLutGain;    ///< R channel lut gain
	UINT32    uiGLutGain;    ///< G channel lut gain
	UINT32    uiBLutGain;    ///< B channel lut gain
} RSCCTRL_DCE_CAC_GAIN;
//@}

/**
    DCE structure - DCE GDC/CAC optical zoom parameters.
*/
//@{
typedef struct {
	RSCCTRL_DCE_CAC_SEL CacSel;         ///< CAC compensation method selection
	RSCCTRL_DCE_CAC_GAIN CacGain;       ///< CAC RGB lut gain
	UINT32 *puiGGeoLut;                 ///< GDC/CAC G geo lut
	INT32 *piRGeoLut;                   ///< GDC/CAC R geo lut
	INT32 *piBGeoLut;                   ///< GDC/CAC B geo lut
} RSCCTRL_OZOOM_PARAM;
//@}

/**
    @name DCE struct - FOV bound RGB
*/
//@{
typedef struct {
	UINT32    uiRBound;    ///< R boundary value
	UINT32    uiGBound;    ///< G boundary value
	UINT32    uiBBound;    ///< B boundary value
} RSCCTRL_DCE_FOV_RGB;
//@}


/**
    DCE structure - DCE FOV parameters.
*/
//@{
typedef struct {
	UINT32 uiFovGain;                   ///< FOV lut gain
	RSCCTRL_DCE_FOV_SEL FovSel;         ///< FOV boundary select
	RSCCTRL_DCE_FOV_RGB FovRGB;         ///< FOV boundary RGB definition
} RSCCTRL_DCE_FOV_PARAM;
//@}


typedef struct {
	RSCCTRL_DCE_DC_SEL DcSel;               ///< Distortion correction combination selection
	RSCCTRL_DCE_IMG_CENT ImgCent;           ///< GDC/CAC center setting
	RSCCTRL_DZOOM_PARAM GdcCacDzParam;      ///< GDC/CAC digital zoom parameters
	RSCCTRL_OZOOM_PARAM GdcCacOzParam;      ///< GDC/CAC optical zoom parameters
	RSCCTRL_DCE_FOV_PARAM FovParam;         ///< FOV parameters
} RSCCTRL_DCE_INFO;

/**
    RSC quality
*/
typedef struct {
	UINT32 CompRatioEIS;   ///< compensation ratio in %.(ratio=margin/body, while total-size=margin+body)
	UINT32 CompRatioRSC;   ///< compensation ratio in %.(ratio=margin/body, while total-size=margin+body)
	UINT32 Stability;      ///< the magnitude of stability, 100% max.
} RSCCTRL_Q;

typedef struct {
	FLOAT  uiSmallAng[3];   ///< small angle definition(degree)
	FLOAT  uiLargeAng[3];   ///< large angle definition(degree)
	UINT32 uiSmallStab[3]; ///< stability parameter for small angle 0~100
	UINT32 uiLargeStab[3]; ///< stability parameter for large angle 0~100
	UINT8  uiDimCtrl;       ///< stability dimension control seperate=0,together=1
	UINT8  uiCtrlType;      ///< stability dimension control fullu control=0,partial control=1
} RSCCTRL_STAB;
/**
    H/V Flip .
*/
typedef struct {
	BOOL uiHflipEn;              ///< Horizontal flip enable
	BOOL uiVflipEn;              ///< Vertical flip enable
} RSCCTRL_FLIP_PARAM;
/**
    AXIS COMPENSETION CONTROLL   .
*/
typedef struct {
	BOOL uiCompXEn;              ///< Image X axis compensation enable
	BOOL uiCompYEn;              ///< Image Y axis compensation enable
	BOOL uiCompZEn;              ///< Image Z axis compensation enable
} RSCCTRL_AXIS_SWITCH;

/**
     Bayer format image size info.
*/
typedef struct {
	USIZE SieCropSize;    ///< SIE crop size
	USIZE SieOutSize;     ///< SIE output size
	USIZE IfeInSize;      ///< IPP input(crop) size
	USIZE DceOutSize;     ///< DCE output size
} RSCCTRL_IPL_SIZE;

/**
     Lens info.
*/
typedef struct {
	UINT32 FocalLength;        ///< Focal length for current lens position, in um; for wide optical-zoom
	UINT32 DistortRate;        ///< Distortion Rate, in 1/65536; Example: 45875 for 30%-barrel, 72090 for 10%-pincushion
	//UINT16 *uiDistortTbl;    ///< Distortion table; project-changing; project-related
	//UINT32 uiDistortNorm;    ///< Distortion Table normalization factor; project-changing; project-related
} RSCCTRL_LENS_INFO;

typedef struct {
	UINT16 *puiDistortTbl;    ///< Distortion table pointer of UINT16[32]; project-changing; project-related
	UINT32 uiDistortNorm;     ///< Distortion Table normalization factor; project-changing; project-related
} RSC_LENS_DISTORT_INFO;
/**
    Sensor array info
*/
typedef USIZE SenArraySize; ///<sensor array size, uint:um

/**
    Sensor output info
*/
typedef struct {
	UINT32 VdT;                 ///< length of VD signal, uint:us
	UINT32 StrLnT;              ///< length from VD start to 1st active line(including OB), uint:us
	UINT32 EndLnT;              ///< length from VD start to last active line(including OB), uint:us
	UINT32 TransDelyT;          ///< length from exposure end to start of data transmission, uint:us
	USIZE  OutSize;             ///< sensor array size(including OB),unit: pixel
	UINT32 Width1X;             ///< width (full scan pixel size)
	UINT32 Height1X;            ///< height(full scan pixel size)
	UINT32 SenBinnRatioH;
	UINT32 SenBinnRatioV;
	UINT32 SenCropRatioH;
	UINT32 SenCropRatioV;
} RSCCTRL_SEN_INFO;


/**
    Gyro reference
*/
typedef struct {
	RSCCTRL_GD_FMT  GDFmt;         ///< Gyro data format
	RSCCTRL_GD_AXIS GDAxis[3];     ///< Gyro data Direction
	RSCCTRL_GD_DIR  GDDir[3];      ///< Gyro data Direction
	UINT32          GDunit;        ///< Gyro data unit, unit: degree
	UINT32          GDCalGain[3];  ///< calibration gain.
	UINT32          GDCalOfs[3];   ///< calibration offset.
} RSCCTRL_Gyro_REF;


/**
     Gyro data table,Max length:8 ctrl points.
*/
typedef struct {
	INT32 *pGyroX;              ///< Gyro X data
	INT32 *pGyroY;              ///< Gyro Y data
	INT32 *pGyroZ;              ///< Gyro Z data
	UINT32 DataCnt;             ///< valid data number
} RSCCTRL_GYRO_TBL;
/**
    RSC Get CAL Information
*/

typedef struct {
	INT32 iCalGain;     ///< the over-all magnitude gain for calibration; sample-changing; project-related
	INT32 iCalDelay;    ///< the over-all delay for calibration; frame-changing; shot-related
} RSCCTRL_CAL_SETTING;
/**
    information for RSC inner operand calculation
*/
typedef struct {
	RSCCTRL_SEN_INFO        *pSenInfo;   ///< sensor timing information
	RSCCTRL_DCE_INFO        *pDceInfo;   ///< IPP-DCE information
	RSCCTRL_IPL_SIZE        *pIPLSize;   ///< IPP-Size information
	RSCCTRL_LENS_INFO       *pLensInfo;  ///< lens information
	UINT32                  *pExpT;      ///< sensor exposure-time information
	RSCCTRL_GYRO_TBL        *pGDTbl;     ///< gyro information: Gyro data
	RSC_LENS_DISTORT_INFO   *pDistorInfo;///< distortin information
	RSCCTRL_CAL_SETTING     *pCalInfo;   ///< cal information
	RSCCTRL_FLIP_PARAM      *pFlipInfor; ///< H/V Flip Information
} RSCCTRL_INFO;

typedef void (*FP_RSCCTRL_RunTimeChg)(IPL_PROC_ID, RSCCTRL_INFO *);
typedef void (*RSC_RSCCTRL_GetGyroData)(RSCCTRL_GYRO_TBL *);

/**
    RSC initial configuration
*/
typedef struct {
	SenArraySize        SenSize;            ///< sensor dimension information
	RSCCTRL_FUNC_SELEC  FuncEn;             ///< true if EIS switch on.
	UINT32              GD_Num;             ///< the number of gyro-data each frame; 8 for RSC-ON, 1~8 for EIS-ON
	RSCCTRL_Gyro_REF    GyroRef;            ///< gyro information
	RSCCTRL_Q           *pRSC_Q;            ///< RSC margin
	FP_RSCCTRL_RunTimeChg   fpRunTimeChg;   ///< RSC sub function pointer: RunTimeChg
	RSC_RSCCTRL_GetGyroData fpGetGyroData;  ///< RSC sub function pointer: GetGyroData
	RSCCTRL_MODE_SELECTION  IPL_MODE;       ///< Current RSC IPL_MODE
	UINT32              TwoDLutSz;          ///< [invalid, no need to configure] DCE 2D LUT Size (H=V); selection = 65/33/17/9; frame-changing; shot-related
	RSCCTRL_STAB       *StabInfor;          ///< Stability Setting based on degree(x,y,z)
	RSCCTRL_FLIP_PARAM  Flip_Parm;          ///< V/H Flip Enable
	RSCCTRL_AXIS_SWITCH Axiset;             ///< axis x,y,z Enable
} RSCCTRL_INIT;
/**
     RSC frame count information
*/
typedef struct {
	UINT32             RSCFCnt;       ///< RSC frame count information
	UINT32             RSCCALCnt;       ///< RSC frame count information
	BOOL               RSCFCntFlag;   ///< RSC frame count flag
} RSCCTRL_FCntInfor;
/**
     RSC Last ready information
*/
typedef struct {
	_2DLUT_PARAM                RsCorrest_Dce2DLut;            ///< LAST FRAME RSC 2D INFORMATION(TO DCE ENGION)
	iCoordinate                 RsCorrest_InCrop;              ///< LAST FRAME RSC INCROP INFORMATION(TO DCE ENGION)
	iCoordinate                 RsCorrest_GdcCntr;             ///< LAST FRAME RSC GDC INFORMATION(TO DCE ENGION)
	UINT32                      RSC_addr_Dce2DLut;             ///< LAST FRAME RSC 2D ADDRESS					   ///< LAST FRAME Valid(TRUE or FALSE)
} RSCCTRL_LastReadyInfo;
/**

*/
typedef struct {
	RSCCTRL_SEN_INFO            uiSensorInfor[IPL_ID_MAX_NUM];      ///< sensor information
	RSCCTRL_FCntInfor           uiFrame;                            ///< frame information
	RSCCTRL_SEN_INFO            SenInfo;                            ///< sensor information
	RSCCTRL_DCE_INFO            DceInfo;                            ///< DCE parameters information
	RSCCTRL_IPL_SIZE            IPLSize;                            ///< IPL parameters information
	RSCCTRL_LENS_INFO           LensInfo;                           ///< Lens parameters information
	RSC_LENS_DISTORT_INFO       DistotInfo;                         ///< Distotion parameters information
	RSCCTRL_FLIP_PARAM          FlipInfor;                          ///< H/V FLIP Infor
	RSCCTRL_CAL_SETTING         CalInfo;                            ///< Cal parameters information
	UINT32                      ExpT;                               ///< exposure time information
	INT32                       GD[3][MAX_GD_CNT];                  ///< GYRO data
	RSCCTRL_GYRO_TBL            GDTbl ;                             ///< GYRO data table
	UINT32                      CapFrameCn[IPL_ID_MAX_NUM];         ///< RSC frameCount to be used on capture mode
	RSC_ACTION_SETTING          RSC_ACTMODE;                        ///< RSC ACTMODE (RSC_BEHAVIOR_NORMAL OR RSC_BEHAVIOR_ABORT)
	/*****parameter(output)*****/
	RSC_DCE_2D_LUT_PARAM        Dce2DLutRscInfo;                    ///< RSC 2D INFORMATION(FROM RSC LIB)
	RSC_IPP_INCROP_PARAM        InCropRscInfo;                      ///< RSC INCROP INFORMATION(FROM RSC LIB)
	RSC_DCE_GDCCNTR_PARAM       GdcCntrRscInfo;                     ///< RSC GDC INFORMATION(FROM RSC LIB)
	_2DLUT_PARAM                RsCorrest_Dce2DLut;                 ///< RSC 2D INFORMATION(TO DCE ENGION)
	iCoordinate                 RsCorrest_InCrop;                   ///< RSC INCROP INFORMATION(TO DCE ENGION)
	iCoordinate                 RsCorrest_GdcCntr;                  ///< RSC GDC INFORMATION(TO DCE ENGION)
	_2DLUT_PARAM                RSC_addr_DcecapDLut;                ///< CAPTURE RSC 2D INFORMATION
	UINT32                      RSC_addr_Dce2DLut;                  ///< RSC 2D ADDRESS
	UINT32                      RSC_DCE_2DLUT_Buf[6][65 * 65];      ///< RSC 2D BUFFER
	UINT32                      BufCnt;                             ///< RSC 2D BUFFER TABLE BUFCNT
	/*****copy outputdata*****/
	RSCCTRL_LastReadyInfo       LastReadyInfo[RSC_LASTBUF_NUM];		///< RSC LAST READY INFORMATION
    UINT32 						LastBufCnt;							///< RSC LAST READY INFORMATION BUFFER COUNT
} RSCCTRL_OBJ;

#if 0
/**
     RSC calculation mode
*/
typedef enum {
	RSCCTRL_Gyro_OK     = 0,    ///< all gyro data are ready
	RSCCTRL_Gyro_MISS   = 1,    ///< some of these gyro data miss.
	ENUM_DUMMY4WORD(RSCCTRL_Gyro_FB)
} RSCCTRL_Gyro_FB;
#endif

/**
     Check RSC calculation ready or not

     if RSC calculation is done, result will be configured into ime automatically.
     otherwise, ime will start with previous RSC result.
     @note (Optional)
     @param[out] pFuncEn:   function ctrl var. address, return 0 if no need to update
     @param[out] pRsc:      Rsc result var. address, return 0 if no need to update
     @return Description of data returned.
         - @b E_OK:     RSC calculation is done.
         - @b E_SYS:    RSC calculation still running.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
//extern ER RSCCTRL_ChkResult(UINT32 *pFuncEnAdr,UINT32 *pRscAdr);



/**
     Trig RSC start

     trig RSC task start to calcaulte, this command won't wait unitl calculation complete.
     @note:if Gyro data missd, Lib will start the mech. of Gyro data prediction.


     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void RSCCTRL_TRIG(GYRO_SENSOR_ID Id, GYRO_NOTIFY_TYPE trigtype);

/**
     Initialize RSC

     @param[in] pInit:    Initial configuration
     @return Description of data returned.
         - @b E_OK:     RSC open is done.
         - @b E_SYS:    RSC have been opened, or input is NULL.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER RSCCTRL_OPEN(IPL_PROC_ID Id, RSCCTRL_INIT *pInit);

/**
     Close RSC

     @param[in] void
     @return Description of data returned.
         - @b E_OK:     RSC close is done.
         - @b E_SYS:    RSC have been closed, or input is NULL.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER RSCCTRL_CLOSE(IPL_PROC_ID Id, UINT32 Msg);
extern UINT32 RSCCTRL_GETADR(IPL_PROC_ID Id);
extern BOOL RSCCTRL_GETFLAG(IPL_PROC_ID Id);
extern void RSCCTRL_SETFLAG(BOOL value);
extern void RSCCTRL_KeepInit(RSCCTRL_INIT *pInit);
extern ER RSCCTRL_Init(void);
extern void RSCCTRL_ClearInit(IPL_PROC_ID Id);
extern UINT32 RSCCTRL_GetIPLId(GYRO_SENSOR_ID Id);
extern UINT32 RSCCTRL_GetGYROId(IPL_PROC_ID Id);
extern void rsc_Msc_GenerateNull2DLut(UINT32 *pDramBuff, UINT32 uiTblSz, UINT32 uiSzH, UINT32 uiSzV);
extern UINT32 RSC_SetCapFrameCn(IPL_PROC_ID Id, UINT32 FrameCnt);
extern ER RSCCTRL_HandleGyroData(RSCCTRL_GYRO_TBL *gyrodata);
extern UINT32 RSCCTRL_FrameTimeChange(UINT32 id);
extern ER RSCCTRL_GetSensorInfor(IPL_PROC_ID Id, RSCCTRL_SEN_INFO *SensorInfor);
extern ER RSCCTRL_DBGStatus(IPL_PROC_ID Id, UINT32 ststus);
extern void RSCCTRL_DBGDumpInfo(IPL_PROC_ID Id);
extern ER RSCCTRL_SaveRaw(void);
extern ER RSCCTRL_CalNeutrlParm(RSC_DCE_2D_LUT_PARAM *LutInfo, RSC_IPP_INCROP_PARAM *CropInfo, RSC_DCE_GDCCNTR_PARAM *GdcCntrIfor, UINT32 sizeH, UINT32 sizeV, UINT32 EISRatio, UINT32 RSCRatio);
extern void RSC_GetNull2D(IPL_PROC_ID Id, UINT32 SieOutH, UINT32 SieOutV, UINT32 sizeH, UINT32 sizeV, DCE_SUBFUNC *pDce_sub, IPL_HAL_GROUP *group,SEL_RSC_STATUS enable,UINT32 eisration,UINT32 rscratio);
#endif
