/**
    Public header file for ISE module.

    @file       ise_lib.h
    @ingroup    mIIPPISE

    @brief

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/


#ifndef _ISE_LIB_H_
#define _ISE_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "Type.h"


typedef void (*ISEIntCB)(UINT32);


//========================================================================
// extern data type

/**
    @addtogroup mIDrvIPP_ISE
*/
//@{


#define ISE_INTE_CLR         0x00000000  ///< disable interrupt
#define ISE_INTE_FRMEND      0x80000000  ///< frame-end interrupt enable
#define ISE_INT_FRMEND       0x80000000  ///< frame-end interrupt status


/**
    Enable and Disable selection
*/
typedef enum _ISE_FUNC_EN {
	ISE_FUNC_DISABLE = 0,        ///< disable
	ISE_FUNC_ENABLE  = 1,        ///< enable
	ENUM_DUMMY4WORD(ISE_FUNC_EN)
} ISE_FUNC_EN;
//------------------------------------------------------------------


/**
    Scaling Up or Down selection
*/
typedef enum _ISE_SCALE_UD {
	ISE_SCALE_DN = 0,         ///< scaling down operation
	ISE_SCALE_UP = 1,          ///< scaling up operation
	ENUM_DUMMY4WORD(ISE_SCALE_UD)
} ISE_SCALE_UD;
//------------------------------------------------------------------


/**
    Input and output packing format selection
*/
typedef enum _ISE_IO_PACK_SEL {
	ISE_Y8          = 0,    ///< Y-8bit packing
	ISE_Y4          = 1,    ///< Y-4bit packing
	ISE_Y1          = 2,    ///< Y-1bit packing
	ISE_UVP         = 3,    ///< UV-Packed
	ISE_RGB565      = 4,    ///< RGB565 packing
	ISE_ARGB8888    = 5,    ///< ARGB8888
	ISE_ARGB1555    = 6,    ///< ARGB1555
	ISE_ARGB4444    = 7,    ///< ARGB4444
	ISE_YUVP        = 8,    ///< YUV-Packed
	ENUM_DUMMY4WORD(ISE_IO_PACK_SEL)
} ISE_IO_PACK_SEL;
//------------------------------------------------------------------


/**
    ARGB8888 output mode selection
*/
typedef enum _ISE_ARGB_OUTMODE_SEL {
	ISE_OUTMODE_ARGB8888 = 0, ///< Output ARGB8888
	ISE_OUTMODE_RGB8888 = 1,  ///< Output RGB888
	ISE_OUTMODE_A8 = 2,       ///< Output A8
	ENUM_DUMMY4WORD(ISE_ARGB_OUTMODE_SEL)
} ISE_ARGB_OUTMODE_SEL;
//------------------------------------------------------------------


/**
    Image scaling method selection
*/
typedef enum _ISE_SCALE_METHOD {
	ISE_BILINEAR    = 1,        ///< bilinear interpolation
	ISE_NEAREST     = 2,        ///< nearest interpolation
	ISE_INTEGRATION = 3,        ///< integration
	ENUM_DUMMY4WORD(ISE_SCALE_METHOD)
} ISE_SCALE_METHOD;
//------------------------------------------------------------------


/**
    Burst length selection
*/
typedef enum _ISE_BURST_SEL {
	ISE_BURST_32W = 0,  ///< burst 32 word
	ISE_BURST_16W = 1,  ///< burst 16 word
	ENUM_DUMMY4WORD(ISE_BURST_SEL)
} ISE_BURST_SEL;
//------------------------------------------------------------------


/**
    Overlap handle selection
*/
typedef enum _ISE_OVLPHDL_SEL {
	ISE_OVLPHDL_HW_AUTO = 0,  ///< overlap width selection, HW auto
	ISE_OVLPHDL_FW_SET  = 1,  ///< overlap width selection, FW setting
	ENUM_DUMMY4WORD(ISE_OVLPHDL_SEL)
} ISE_OVLPHDL_SEL;
//------------------------------------------------------------------


/**
    Scale factor mode
*/
typedef enum _ISE_SCLFTR_MODE {
	ISE_SCLFTR_AUTO = 0,  ///< Scale factor auto mode, handled by driver
	ISE_SCLFTR_USER = 1,  ///< Scale factor auto mode, handled by user
	ENUM_DUMMY4WORD(ISE_SCLFTR_MODE)
} ISE_SCLFTR_MODE;
//------------------------------------------------------------------

/**
    Get engine information selection
*/
typedef enum _ISE_GET_INFO_SEL {
	ISE_GET_IN_H_SIZE       = 0,   ///< get input width size
	ISE_GET_IN_V_SIZE       = 1,   ///< get input height size
	ISE_GET_IN_OFS          = 2,   ///< get input lineoffset size
	ISE_GET_IN_ADDR         = 3,   ///< get input buffer address
	ISE_GET_OUT_H_SIZE      = 4,   ///< get output width size
	ISE_GET_OUT_V_SIZE      = 5,   ///< get output height size
	ISE_GET_OUT_OFS         = 6,   ///< get output lineoffset
	ISE_GET_OUT_ADDR        = 7,   ///< get output buffer address
	ISE_GET_INTERRUPT       = 8,   ///< get interrupt status
	ISE_GET_INTERRUPT_EN    = 9,   ///< get interrupt enable
	ENUM_DUMMY4WORD(ISE_GET_INFO_SEL)
} ISE_GET_INFO_SEL;
//------------------------------------------------------------------


/**
    Clear ISE Flag
*/
typedef enum _ISE_FLAG_CLEAR_SEL {
	ISE_FLAG_NO_CLEAR = 0,    ///< No clear flag
	ISE_FLAG_CLEAR    = 1,    ///< Clear flag
	ENUM_DUMMY4WORD(ISE_FLAG_CLEAR_SEL)
} ISE_FLAG_CLEAR_SEL;
//------------------------------------------------------------------


/**
    ISE structure - Scaling factors
*/
typedef struct _ISE_SCALE_FACTOR_INFO {
	ISE_SCALE_UD  HScaleUD;             ///< scale up or down for horizontal direction
	UINT32        uiHScaleDR;           ///< scale down rate of horizontal direction
	UINT32        uiHScaleFtr;          ///< scale factor of horizontal direction
	ISE_SCALE_UD  VScaleUD;             ///< scale up or down for vertical direction
	UINT32        uiVScaleDR;           ///< scale down rate of vertical direction
	UINT32        uiVScaleFtr;          ///< scale down factor of vertical direction

	UINT32        uiIsdScaleBaseH;      ///< scale down base of integration for horizontal direction
	UINT32        uiIsdScaleFtrH[3];    ///< scale down factor of integration for horizontal direction
	UINT32        uiIsdScaleBaseV;      ///< scale down base of integration for vertical direction
	UINT32        uiIsdScaleFtrV[3];    ///< scale down factor of integration for vertical direction

	UINT32        uiHScaleOfs;          ///< horizonal scale offset
	UINT32        uiVScaleOfs;          ///< vertical scale offset
} ISE_SCALE_FACTOR_INFO;
//------------------------------------------------------------------


/**
    ISE structure - Scaling filter parameters
*/
typedef struct _ISE_SCALE_FILTER_INFO {
	ISE_FUNC_EN   HFilterEn;        ///< scale filter enable for horizontal direction
	UINT32        uiHFilterCoef;    ///< scale filter coefficient for horizontal direction
	ISE_FUNC_EN   VFilterEn;        ///< scale filter enable for vertical direction
	UINT32        uiVFilterCoef;    ///< scale filter coefficient for vertical direction
} ISE_SCALE_FILTER_INFO;
//------------------------------------------------------------------

/*
    ISE structure - Input and Output Burst length parameters
*/
typedef struct _ISE_BURST_LENGTH {
	ISE_BURST_SEL BurstLenInput;    ///< input burst length
	ISE_BURST_SEL BurstLenOutput;   ///< output burst length
} ISE_BURST_LENGTH;



/**
    ISE structure - Engine open object
*/
typedef struct _ISE_OPENOBJ {
	void (*FP_ISEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
	UINT32 uiIseClockSel;                      ///< Engine clock selection
} ISE_OPENOBJ, *pISE_OPENOBJ;
//------------------------------------------------------------------


/**
    ISE structure - Engine flow setting structure
*/
typedef struct _ISE_MODE_PRAM {
	ISE_IO_PACK_SEL       IOPackFormat;           ///< input and output packing format
	ISE_ARGB_OUTMODE_SEL  ARGBOutMode;            ///< select output mode for IOPackFormat = ISE_ARGB8888
	ISE_SCALE_METHOD      ScaleMethod;            ///< scaling method selection
	ISE_SCLFTR_MODE       SclFtrMode;             ///< scaling factor handling mode

	UINT32                uiInWidth;              ///< input image width
	UINT32                uiInHeight;             ///< input image height
	UINT32                uiInLineoffset;         ///< input lineoffset
	UINT32                uiInAddr;               ///< input buffer address

	UINT32                uiOutWidth;             ///< output image width
	UINT32                uiOutHeight;            ///< output image height
	UINT32                uiOutLineoffset;        ///< output lineoffset
	UINT32                uiOutAddr;              ///< output buffer address
} ISE_MODE_PRAM, *pISE_MODE_PRAM;
//------------------------------------------------------------------


/**
    ISE structure - Get output image buffer size
*/
typedef struct _ISE_OUTPUT_BUFSIZE_INFO {
	UINT32 InSizeH;     ///< input image width
	UINT32 InSizeV;     ///< input image height
	UINT32 OutSizeH;    ///< output image width
	UINT32 OutSizeV;    ///< output image height
	UINT32 OutLineOfs;  ///< output image lineoffset
} ISE_OUTPUT_BUFSIZE_INFO;



//---------------------------------------------------------------------------
/**
    ISE engine is opened or not

    To check ISE engine is opened or closed.

    @return BOOL  Status of performing this API.\n
    -@b TRUE  Engine is opened.\n
    -@b FALSE  Engine is closed.\n
*/
extern BOOL ise_isOpen(VOID);
//------------------------------------------------------------------


/*
    Open ISE engine

    This function should be appiled before calling any other functions.

    @param pISE_OpenInfo ISR callback function and clock selection

    @return ER
    -@b E_OK: Done without error.\n
    -@b Others: Error occured.\n
*/
extern ER ise_open(ISE_OPENOBJ *pISE_OpenInfo);
//------------------------------------------------------------------


/**
    Set ISE engine mode and parameters

    Set ISE engine mode and parameters

    @param pEngineInfo mode information and parameters.\n Please refer to the data structure ISE_MODE_PRAM for more details.

    @return ER \n
    -@b E_OK  Engine set mode and parameters without error.\n
    -@b E_PAR Egnine set parameters error.\n
    -@b other Engine set mode error.\n
*/
extern ER ise_setMode(ISE_MODE_PRAM *pEngineInfo);
//------------------------------------------------------------------


/**
    Start ISE

    This function is used to start ISE engine after setting parameters.

    @return  ER  Error code to check the status of the desired operation.\n
    -@b E_OK  The desired operation is allowed.\n
    -@b E_SYS  The desired operation is performed error.\n
*/
extern ER ise_start(VOID);
//------------------------------------------------------------------


/**
    Pause ISE engine

    If ones would like to alter those changeable parameters, \n
    this function must be called firstly to make engine pause from the running state.

    @return ER Error code to check the status of the desired interrupt. \n
    -@b E_OK  Execute function, success. \n
    -@b E_SYS Execute function, fail, or, the desired operation within state matchine is not allowed. \n
*/
extern ER ise_pause(VOID);
//------------------------------------------------------------------


/**
    Close ISE engine

    Close ISE engine

    @return ER Error status to check engine closing status.\n
    -@b E_OK  Execute function, success.\n
    -@b E_SYS  Engine close, fail.\n
*/
extern ER ise_close(VOID);
//------------------------------------------------------------------


/**
    Wait frame-end flag within system

    Wait system flag of frame-end of ISE engine.

    @param[in] isClrFlag  Clear flag selection.\n
                         -@b NO_CLEAR_FLAG  No clear flag.\n
                         -@b CLEAR_FLAG  Clear flag.\n

    @return ER Error code to check waiting status\n
    -@b E_OK  The flag is waited.\n
*/
extern ER ise_waitFlagFrameEnd(ISE_FLAG_CLEAR_SEL IsFlagClr);
//------------------------------------------------------------------


/**
    Clear System Flag of ISE Engine

    Clear System Flag of ISE Engine

    @return ER Error status of performing API. \n
    -@b E_OK  clear flag is done. \n
    -@b Others clear flag is fail. \n
*/
extern ER ise_clrIntFlag(VOID);
//------------------------------------------------------------------


/**
  Get ISE Engine Information

  This function is used to get some information from ISE

  @param[in] GetInfoSel select desired item\n

  @return UINT32 Value of desired item\n
*/
extern UINT32 ise_getEngineInfo(ISE_GET_INFO_SEL GetInfoSel);
//------------------------------------------------------------------


/**
  Get ISE Engine Clock Rate

  This function is used to get the in-used clock rate of ISE
*/
extern UINT32 ise_getClockRate(VOID);
//------------------------------------------------------------------


/**
  Set ISE Scale Factor by User

  This function is used to set user-define scale factor

  @param[in] pScaleFactorInfo  User-define scaling factors\n

  @return VOID\n
*/
extern VOID ise_setUserScaleFactorParam(ISE_SCALE_FACTOR_INFO *pScaleFactorInfo);
//------------------------------------------------------------------


/**
  Set ISE Scale Filter by User

  This function is used to set user-define scale filter parameters

  @param[in] pScaleInfo  User-define scaling filater parameters\n

  @return VOID\n
*/
extern VOID ise_setUserScaleFilterParam(ISE_SCALE_FILTER_INFO *pScaleFilterInfo);
//------------------------------------------------------------------

/**
  Get ISE Base Address of Register

  @param[in] VOID\n

  @return UINT32 Address\n
*/
extern UINT32 ise_getRegBaseAddr(VOID);
//------------------------------------------------------------------

/**
    Set Burst Length

    @param[in] pBurstLen  burst length parameters

    @return VOID
*/
extern VOID ise_chgBurstLength(ISE_BURST_LENGTH *pBurstLen);
//------------------------------------------------------------------



/**
    Get Output Buffer Size

    @param[in] pBurstLen  burst length parameters

    @return VOID
*/
extern UINT32 ise_getOutputBufSize(ISE_OUTPUT_BUFSIZE_INFO *pGetBufInfo);
//------------------------------------------------------------------




#ifdef __cplusplus
}
#endif

#endif

//@}


