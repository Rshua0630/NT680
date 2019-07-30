/**
    Public header file for ISE2 module.

    @file       ise2_lib.h
    @ingroup    mIIPPISE2

    @brief

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/


#ifndef _ISE2_LIB_H_
#define _ISE2_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "Type.h"


typedef void (*ISE2IntCB)(UINT32);


//========================================================================
// extern data type

/**
    @addtogroup mIDrvIPP_ISE2
*/
//@{


#define ISE2_INTE_CLR         0x00000000  ///< disable interrupt
#define ISE2_INTE_FRMEND      0x80000000  ///< frame-end interrupt enable
#define ISE2_INT_FRMEND       0x80000000  ///< frame-end interrupt status


/**
    Enable and Disable selection
*/
typedef enum _ISE2_FUNC_EN {
	ISE2_FUNC_DISABLE = 0,        ///< disable
	ISE2_FUNC_ENABLE  = 1,        ///< enable
	ENUM_DUMMY4WORD(ISE2_FUNC_EN)
} ISE2_FUNC_EN;
//------------------------------------------------------------------


/**
    Scaling Up or Down selection
*/
typedef enum _ISE2_SCALE_UD {
	ISE2_SCALE_DN = 0,         ///< scaling down operation
	ISE2_SCALE_UP = 1,          ///< scaling up operation
	ENUM_DUMMY4WORD(ISE2_SCALE_UD)
} ISE2_SCALE_UD;
//------------------------------------------------------------------


/**
    Input and output packing format selection
*/
typedef enum _ISE2_IO_PACK_SEL {
	ISE2_Y8          = 0,    ///< Y-8bit packing
	ISE2_Y4          = 1,    ///< Y-4bit packing
	ISE2_Y1          = 2,    ///< Y-1bit packing
	ISE2_UVP         = 3,    ///< UV-Packed
	ISE2_RGB565      = 4,    ///< RGB565 packing
	ISE2_ARGB8888    = 5,    ///< ARGB8888
	ISE2_ARGB1555    = 6,    ///< ARGB1555
	ISE2_ARGB4444    = 7,    ///< ARGB4444
	ISE2_YUVP        = 8,    ///< YUV-Packed
	ENUM_DUMMY4WORD(ISE2_IO_PACK_SEL)
} ISE2_IO_PACK_SEL;
//------------------------------------------------------------------


/**
    ARGB8888 output mode selection
*/
typedef enum _ISE2_ARGB_OUTMODE_SEL {
	ISE2_OUTMODE_ARGB8888 = 0, ///< Output ARGB8888
	ISE2_OUTMODE_RGB8888 = 1,  ///< Output RGB888
	ISE2_OUTMODE_A8 = 2,       ///< Output A8
	ENUM_DUMMY4WORD(ISE2_ARGB_OUTMODE_SEL)
} ISE2_ARGB_OUTMODE_SEL;
//------------------------------------------------------------------


/**
    Image scaling method selection
*/
typedef enum _ISE2_SCALE_METHOD {
	ISE2_BILINEAR    = 1,        ///< bilinear interpolation
	ISE2_NEAREST     = 2,        ///< nearest interpolation
	ISE2_INTEGRATION = 3,        ///< integration
	ENUM_DUMMY4WORD(ISE2_SCALE_METHOD)
} ISE2_SCALE_METHOD;
//------------------------------------------------------------------


/**
    Burst length selection
*/
typedef enum _ISE2_BURST_SEL {
	ISE2_BURST_32W = 0,  ///< burst 32 word
	ISE2_BURST_16W = 1,  ///< burst 16 word
	ENUM_DUMMY4WORD(ISE2_BURST_SEL)
} ISE2_BURST_SEL;
//------------------------------------------------------------------


/**
    Overlap handle selection
*/
typedef enum _ISE2_OVLPHDL_SEL {
	ISE2_OVLPHDL_HW_AUTO = 0,  ///< overlap width selection, HW auto
	ISE2_OVLPHDL_FW_SET  = 1,  ///< overlap width selection, FW setting
	ENUM_DUMMY4WORD(ISE2_OVLPHDL_SEL)
} ISE2_OVLPHDL_SEL;
//------------------------------------------------------------------


/**
    Scale factor mode
*/
typedef enum _ISE2_SCLFTR_MODE {
	ISE2_SCLFTR_AUTO = 0,  ///< Scale factor auto mode, handled by driver
	ISE2_SCLFTR_USER = 1,  ///< Scale factor auto mode, handled by user
	ENUM_DUMMY4WORD(ISE2_SCLFTR_MODE)
} ISE2_SCLFTR_MODE;
//------------------------------------------------------------------

/**
    Get engine information selection
*/
typedef enum _ISE2_GET_INFO_SEL {
	ISE2_GET_IN_H_SIZE       = 0,   ///< get input width size
	ISE2_GET_IN_V_SIZE       = 1,   ///< get input height size
	ISE2_GET_IN_OFS          = 2,   ///< get input lineoffset size
	ISE2_GET_IN_ADDR         = 3,   ///< get input buffer address
	ISE2_GET_OUT_H_SIZE      = 4,   ///< get output width size
	ISE2_GET_OUT_V_SIZE      = 5,   ///< get output height size
	ISE2_GET_OUT_OFS         = 6,   ///< get output lineoffset
	ISE2_GET_OUT_ADDR        = 7,   ///< get output buffer address
	ISE2_GET_INTERRUPT       = 8,   ///< get interrupt status
	ISE2_GET_INTERRUPT_EN    = 9,   ///< get interrupt enable
	ENUM_DUMMY4WORD(ISE2_GET_INFO_SEL)
} ISE2_GET_INFO_SEL;
//------------------------------------------------------------------


/**
    Clear ISE2 Flag
*/
typedef enum _ISE2_FLAG_CLEAR_SEL {
	ISE2_FLAG_NO_CLEAR = 0,    ///< No clear flag
	ISE2_FLAG_CLEAR    = 1,    ///< Clear flag
	ENUM_DUMMY4WORD(ISE2_FLAG_CLEAR_SEL)
} ISE2_FLAG_CLEAR_SEL;
//------------------------------------------------------------------


/**
    ISE2 structure - Scaling factors
*/
typedef struct _ISE2_SCALE_FACTOR_INFO {
	ISE2_SCALE_UD  HScaleUD;             ///< scale up or down for horizontal direction
	UINT32        uiHScaleDR;           ///< scale down rate of horizontal direction
	UINT32        uiHScaleFtr;          ///< scale factor of horizontal direction
	ISE2_SCALE_UD  VScaleUD;             ///< scale up or down for vertical direction
	UINT32        uiVScaleDR;           ///< scale down rate of vertical direction
	UINT32        uiVScaleFtr;          ///< scale down factor of vertical direction

	UINT32        uiIsdScaleBaseH;      ///< scale down base of integration for horizontal direction
	UINT32        uiIsdScaleFtrH[3];    ///< scale down factor of integration for horizontal direction
	UINT32        uiIsdScaleBaseV;      ///< scale down base of integration for vertical direction
	UINT32        uiIsdScaleFtrV[3];    ///< scale down factor of integration for vertical direction

	UINT32        uiHScaleOfs;          ///< horizonal scale offset
	UINT32        uiVScaleOfs;          ///< vertical scale offset
} ISE2_SCALE_FACTOR_INFO;
//------------------------------------------------------------------


/**
    ISE2 structure - Scaling filter parameters
*/
typedef struct _ISE2_SCALE_FILTER_INFO {
	ISE2_FUNC_EN   HFilterEn;        ///< scale filter enable for horizontal direction
	UINT32        uiHFilterCoef;    ///< scale filter coefficient for horizontal direction
	ISE2_FUNC_EN   VFilterEn;        ///< scale filter enable for vertical direction
	UINT32        uiVFilterCoef;    ///< scale filter coefficient for vertical direction
} ISE2_SCALE_FILTER_INFO;
//------------------------------------------------------------------

/*
    ISE2 structure - Input and Output Burst length parameters
*/
typedef struct _ISE2_BURST_LENGTH {
	ISE2_BURST_SEL BurstLenInput;    ///< input burst length
	ISE2_BURST_SEL BurstLenOutput;   ///< output burst length
} ISE2_BURST_LENGTH;



/**
    ISE2 structure - Engine open object
*/
typedef struct _ISE2_OPENOBJ {
	void (*FP_ISE2ISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
	UINT32 uiIseClockSel;                      ///< Engine clock selection
} ISE2_OPENOBJ, *pISE2_OPENOBJ;
//------------------------------------------------------------------


/**
    ISE2 structure - Engine flow setting structure
*/
typedef struct _ISE2_MODE_PRAM {
	ISE2_IO_PACK_SEL       IOPackFormat;           ///< input and output packing format
	ISE2_ARGB_OUTMODE_SEL  ARGBOutMode;            ///< select output mode for IOPackFormat = ISE2_ARGB8888
	ISE2_SCALE_METHOD      ScaleMethod;            ///< scaling method selection
	ISE2_SCLFTR_MODE       SclFtrMode;             ///< scaling factor handling mode

	UINT32                uiInWidth;              ///< input image width
	UINT32                uiInHeight;             ///< input image height
	UINT32                uiInLineoffset;         ///< input lineoffset
	UINT32                uiInAddr;               ///< input buffer address

	UINT32                uiOutWidth;             ///< output image width
	UINT32                uiOutHeight;            ///< output image height
	UINT32                uiOutLineoffset;        ///< output lineoffset
	UINT32                uiOutAddr;              ///< output buffer address
} ISE2_MODE_PRAM, *pISE2_MODE_PRAM;
//------------------------------------------------------------------


/**
    ISE2 structure - Get output image buffer size
*/
typedef struct _ISE2_OUTPUT_BUFSIZE_INFO {
	UINT32 InSizeH;     ///< input image width
	UINT32 InSizeV;     ///< input image height
	UINT32 OutSizeH;    ///< output image width
	UINT32 OutSizeV;    ///< output image height
	UINT32 OutLineOfs;  ///< output image lineoffset
} ISE2_OUTPUT_BUFSIZE_INFO;



//---------------------------------------------------------------------------
/**
    ISE2 engine is opened or not

    To check ISE2 engine is opened or closed.

    @return BOOL  Status of performing this API.\n
    -@b TRUE  Engine is opened.\n
    -@b FALSE  Engine is closed.\n
*/
extern BOOL ise2_isOpen(VOID);
//------------------------------------------------------------------


/*
    Open ISE2 engine

    This function should be appiled before calling any other functions.

    @param pISE2_OpenInfo ISR callback function and clock selection

    @return ER
    -@b E_OK: Done without error.\n
    -@b Others: Error occured.\n
*/
extern ER ise2_open(ISE2_OPENOBJ *pISE2_OpenInfo);
//------------------------------------------------------------------


/**
    Set ISE2 engine mode and parameters

    Set ISE2 engine mode and parameters

    @param pEngineInfo mode information and parameters.\n Please refer to the data structure ISE2_MODE_PRAM for more details.

    @return ER \n
    -@b E_OK  Engine set mode and parameters without error.\n
    -@b E_PAR Egnine set parameters error.\n
    -@b other Engine set mode error.\n
*/
extern ER ise2_setMode(ISE2_MODE_PRAM *pEngineInfo);
//------------------------------------------------------------------


/**
    Start ISE2

    This function is used to start ISE2 engine after setting parameters.

    @return  ER  Error code to check the status of the desired operation.\n
    -@b E_OK  The desired operation is allowed.\n
    -@b E_SYS  The desired operation is performed error.\n
*/
extern ER ise2_start(VOID);
//------------------------------------------------------------------


/**
    Pause ISE2 engine

    If ones would like to alter those changeable parameters, \n
    this function must be called firstly to make engine pause from the running state.

    @return ER Error code to check the status of the desired interrupt. \n
    -@b E_OK  Execute function, success. \n
    -@b E_SYS Execute function, fail, or, the desired operation within state matchine is not allowed. \n
*/
extern ER ise2_pause(VOID);
//------------------------------------------------------------------


/**
    Close ISE2 engine

    Close ISE2 engine

    @return ER Error status to check engine closing status.\n
    -@b E_OK  Execute function, success.\n
    -@b E_SYS  Engine close, fail.\n
*/
extern ER ise2_close(VOID);
//------------------------------------------------------------------


/**
    Wait frame-end flag within system

    Wait system flag of frame-end of ISE2 engine.

    @param[in] isClrFlag  Clear flag selection.\n
                         -@b NO_CLEAR_FLAG  No clear flag.\n
                         -@b CLEAR_FLAG  Clear flag.\n

    @return ER Error code to check waiting status\n
    -@b E_OK  The flag is waited.\n
*/
extern ER ise2_waitFlagFrameEnd(ISE2_FLAG_CLEAR_SEL IsFlagClr);
//------------------------------------------------------------------


/**
    Clear System Flag of ISE2 Engine

    Clear System Flag of ISE2 Engine

    @return ER Error status of performing API. \n
    -@b E_OK  clear flag is done. \n
    -@b Others clear flag is fail. \n
*/
extern ER ise2_clrIntFlag(VOID);
//------------------------------------------------------------------


/**
  Get ISE2 Engine Information

  This function is used to get some information from ISE2

  @param[in] GetInfoSel select desired item\n

  @return UINT32 Value of desired item\n
*/
extern UINT32 ise2_getEngineInfo(ISE2_GET_INFO_SEL GetInfoSel);
//------------------------------------------------------------------


/**
  Get ISE2 Engine Clock Rate

  This function is used to get the in-used clock rate of ISE2
*/
extern UINT32 ise2_getClockRate(VOID);
//------------------------------------------------------------------


/**
  Set ISE2 Scale Factor by User

  This function is used to set user-define scale factor

  @param[in] pScaleFactorInfo  User-define scaling factors\n

  @return VOID\n
*/
extern VOID ise2_setUserScaleFactorParam(ISE2_SCALE_FACTOR_INFO *pScaleFactorInfo);
//------------------------------------------------------------------


/**
  Set ISE2 Scale Filter by User

  This function is used to set user-define scale filter parameters

  @param[in] pScaleInfo  User-define scaling filater parameters\n

  @return VOID\n
*/
extern VOID ise2_setUserScaleFilterParam(ISE2_SCALE_FILTER_INFO *pScaleFilterInfo);
//------------------------------------------------------------------

/**
  Get ISE2 Base Address of Register

  @param[in] VOID\n

  @return UINT32 Address\n
*/
extern UINT32 ise2_getRegBaseAddr(VOID);
//------------------------------------------------------------------

/**
    Set Burst Length

    @param[in] pBurstLen  burst length parameters

    @return VOID
*/
extern VOID ise2_chgBurstLength(ISE2_BURST_LENGTH *pBurstLen);
//------------------------------------------------------------------



/**
    Get Output Buffer Size

    @param[in] pBurstLen  burst length parameters

    @return VOID
*/
extern UINT32 ise2_getOutputBufSize(ISE2_OUTPUT_BUFSIZE_INFO *pGetBufInfo);
//------------------------------------------------------------------




#ifdef __cplusplus
}
#endif

#endif

//@}


