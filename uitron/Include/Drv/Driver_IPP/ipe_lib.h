/**
    IPE module driver

    NT966600 IPE driver extern header file.

    @file       Ipe_lib.h
    @ingroup    mIDrvIPP_IPE
    @note
\n IPE Driver has 4 states (Idel, Ready, Pause, Run)
\n Idle state: No operation, and IPE semaphore is not occupied.
\n Ready state: No operation, IPE parameters are not configured yet, but IPE semaphore is occupied.
\n Pause state: No operation, IPE parameters are already configured for operation, IPE semaphore is occupied.
\n Run state: In operation, some parameters are allowed to be changed dynamically, IPE semaphore is occupied.
\n ipe_open    Get IPE semaphore before operation starts
\n
\n Function list:
\n ipe_open() Get IPE semaphore before operation starts
\n ipe_close()   Release IPE semaphore after operation ends
\n ipe_changeSize()  Set all Dzoom size related params
\n ipe_changeParam() Set IQ related params
\n ipe_changeInput() Set input address, sync selection, and PPB control
\n ipe_changeOutput()    Set output source, address, and line offset
\n ipe_setMode() Set all IPE params, including size, IQ param, I/O
\n ipe_start()   Trigger IPE start
\n ipe_pause()   Trigger IPE stop
\n ipe_reset()   SW reset IPE
\n ipe_enableDMAOut()    Turn on/off IPE output DMA channel
\n ipe_clrFrameEnd() Clear IPE frame end flag
\n ipe_waitFrameEnd()    Wait IPE frame end flag
\n ipe_clrDMAEnd()   Clear IPE DMA done flag
\n ipe_waitDMAEnd()  Wait IPE DMA done flag
\n
\n Dram to Dram usage example:
\n (1)ipe_open()
\n (2)ipe_setMode()
\n (3)ipe_start()
\n (4)ipe_waitFrameEnd()
\n (5)ipe_close()

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _IPE_LIB_H
#define _IPE_LIB_H

#define IPE_DEBUG   0
#define IPE_DEBUG_INFO   0

//#define GetIpeVACnt ipe_getVACnt

/**
    @addtogroup mIDrvIPP_IPE
*/
//@{

/**
    @name Define IPE function enable.
*/
//@{
#define IPE_RGBLPF_EN                  0x00000001  ///< RGB LPF function enable
#define IPE_RGBGAMMA_EN                0x00000002  ///< RGB Gamma function enable
#define IPE_YCURVE_EN                  0x00000004  ///< Y curve function enable
#define IPE_CCR_EN                     0x00000100  ///< Color correction function enable
#define IPE_CC3D_EN                    0x00000200  ///< 3D color correction function enable
#define IPE_CST_EN                     0x00000400  ///< Color space transform function enable, RGB->YCC
#define IPE_CTRL_EN                    0x00001000  ///< Color control function enable
#define IPE_HADJ_EN                    0x00002000  ///< Hue adjustment function enable
#define IPE_CADJ_EN                    0x00004000  ///< Color component adjust function enable
#define IPE_CADJ_YENH_EN               0x00008000  ///< Color component edge enhance function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_YCON_EN               0x00010000  ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJ_EN
#define IPE_CADJ_CCON_EN               0x00020000  ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJ_EN
#define IPE_YCTH_EN                    0x00040000  ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJ_EN
#define IPE_CSTP_EN                    0x00080000  ///< Color space transform protect function enable, RGB->YCC
//@}

/**
    @name Define IPE interrput enable.
*/
//@{
//#define IPE_INTE_IPESTART              0x00000001  ///< Interrupt enable for triggering IPE start
#define IPE_INTE_FRMEND                0x00000002  ///< Interrupt enable for frame done
#define IPE_INTE_ST                    0x00000004  ///< Interrupt enable for current stripe done
#define IPE_INTE_FRMSTART              0x00000008  ///< Interrupt enable for frame start
#define IPE_INTE_YCOUTEND              0x00000010  ///< Interrupt enable for YC DRAM output done
#define IPE_INTE_DMAIN0END             0x00000020  ///< Interrupt enable for loading Gamma LUT done
#define IPE_INTE_DMAIN1END             0x00000040  ///< Interrupt enable for loading 3DCC LUT done
//#define IPE_INTE_GAMERR                0x00000080  ///< Interrupt enable for CbCr subsample DRAM output done
//#define IPE_INTE_3DCCERR               0x00000100  ///< Interrupt enable for V subsample DRAM output done
#define IPE_INTE_ALL                   0x0000007e  ///< All interrupt enable
//@}

/**
    @name Define IPE interrput status.
*/
//@{
//#define IPE_INT_IPESTART              0x00000001  ///< Interrupt enable for triggering IPE start
#define IPE_INT_FRMEND                0x00000002  ///< Interrupt enable for frame done
#define IPE_INT_ST                    0x00000004  ///< Interrupt enable for current stripe done
#define IPE_INT_FRMSTART              0x00000008  ///< Interrupt enable for frame start
#define IPE_INT_YCOUTEND              0x00000010  ///< Interrupt enable for YC DRAM output done
#define IPE_INT_DMAIN0END             0x00000020  ///< Interrupt enable for DRAM input done
#define IPE_INT_DMAIN1END             0x00000040  ///< Interrupt enable for DRAM input done
//#define IPE_INT_GAMERR                0x00000080  ///< Interrupt enable for gamma loading error
//#define IPE_INT_3DCCERR               0x00000100  ///< Interrupt enable for gamma loading error
#define IPE_INT_ALL                   0x0000007e  ///< All interrupt enable
//@}

/**
    @name Define IPE engine value.
*/
//@{
//#define IPE_HSIZE_IODIFF     0

//#define VAC_WIN8                        8  ///< VA window number 8
//#define VAC_WIN16                       16  ///< VA window number 16
//#define IPE_VAC_HNF_MINIMUN             2  ///< Minimum VA hnf value
//#define IPE_VACC_VALUE_MSK              0x0003ffff  ///< VA DRAM data, contrast value mask
//#define IPE_VACC_NUM_MSK                0xfffc0000  ///< VA DRAM data, threshold number mask
//#define IPE_MAXVAWORD      512  ///< Maximum VA DRAM output word
//#define IPE_VA_MAXH_WIN8                    1030  ///< Maximum VA horizontal size when VA window number is 8x8
//#define IPE_VA_MAXH_WIN16                  2038  ///< Maximum VA horizontal size when VA window number is 16x16

//#define IPE_MAX_COLOR_OFFSET    255
//#define IPE_MAX_COLOR_GAIN      1023
//@}

/**
    @name Define IPE function setting selection.
*/
//@{
#define IPE_SET_NONE                0x00000000  ///< Select don't update ANY parameters
#define IPE_SET_EDGEEXT             0x00000001  ///< Select to update edge related parameters
#define IPE_SET_EDGEPROC            0x00000002  ///< Select to update edge related parameters
#define IPE_SET_RGBLPF              0x00000004  ///< Select to update RGB LPF parameters
#define IPE_SET_COLORCORRECT        0x00000008  ///< Select to update color correction parameters
#define IPE_SET_CST                 0x00002000  ///< Select to update color space transform parameters
#define IPE_SET_CCTRL               0x00000010  ///< Select to update color control parameters
#define IPE_SET_CADJ_EENH           0x00000020  ///< Select to update cadj edge enhance parameters
#define IPE_SET_CADJ_EINV           0x00000040  ///< Select to update cadj edge inversion parameters
#define IPE_SET_CADJ_YCCON          0x00000080  ///< Select to update cadj YC contrast parameters
#define IPE_SET_CADJ_COFS           0x00000100  ///< Select to update cadj color offset parameters
#define IPE_SET_CADJ_RAND           0x00000200  ///< Select to update cadj YC random noise parameters
#define IPE_SET_CADJ_FIXTH          0x00000400  ///< Select to update cadj fixed threshold parameters
#define IPE_SET_CADJ_MASK           0x00000800  ///< Select to update cadj YC mask parameters
#define IPE_SET_CSTP                0x00001000  ///< Select to update color space transform protection parameters
#define IPE_SET_SUBOUT              0x00002000  ///< Select to update subsampled output to dram parameters
#define IPE_SET_DIRIFE2             0x00004000  ///< Select to update subsampled image direct to IFE2 parameters
#define IPE_SET_GAMMA               0x00008000  ///< Select to update gamma parameters
#define IPE_SET_CC3D                0x00010000  ///< Select to update 3dcc parameters
#define IPE_SET_YCURVE              0x00020000  ///< Select to update y curve parameters
#define IPE_SET_ETH                 0x00040000  ///< Select to update ETH parameters


#define IPE_SET_ALL                 0xffffffff  ///< Select to update ALL IQ parameters
//@}

/**
    @name Enum IPE input control.
*/
//@{

/**
    IPE option of setting registers

\n Used for ipe_changeParam().
*/
typedef enum {
	IPE_ALL_REG = 0,          ///< Set all registers
	IPE_FD_LATCHED = 1,        ///< Update FD-latched registers only
	IPE_NON_FD_LATCHED = 2,    ///< Updat Non-FD-latched registers only
	ENUM_DUMMY4WORD(IPE_REGTYPE)
} IPE_REGTYPE;

/**
    IPE load type selection

    Select ipe load type
*/
typedef enum {
	IPE_FRMEND_LOAD = 0,     ///< IPE frame end load
	IPE_START_LOAD = 1,      ///< IPE start load
	ENUM_DUMMY4WORD(IPE_LOAD_TYPE)
} IPE_LOAD_TYPE;


/**
    IPE DRAM input SRAM update selection

\n Used for ipe_changeParam().
*/
typedef enum {
	IPE_READ_LUT = 0,        ///< IPE read SRAM
	DMA_WRITE_LUT = 1,       ///< DMA write SRAM
	CPU_READ_LUT = 2,        ///< CPU read SRAM
	ENUM_DUMMY4WORD(IPE_RW_SRAM_ENUM)
} IPE_RW_SRAM_ENUM;

/**
    IPE Load from DRAM to Gamma SRAM update option

    Select Gamma load option
*/
typedef enum {
	LoadGam_R = 0,          ///< Load Gamma R
	LoadGam_RGB = 1,        ///< Load Gamma RGB
	LoadGam_Y = 2,          ///< Load Y curve
	LoadGam_R_Y = 3,        ///< Load Gamma R and then Y curve
	ENUM_DUMMY4WORD(IPE_RWGAM_OPT)
} IPE_RWGAM_OPT;

/**
    IPE mode selection

    Select ipe mode
*/
/*
typedef enum
{
    MODE_D2D = 0,        ///< IPE input from DRAM, output to DRAM
    MODE_DCE = 1,        ///< IPE input from DCE/Dram, output to IME
    MODE_ALLDIRECT = 2,  ///< IPE input from DCE/SIE, output to IME
    ENUM_DUMMY4WORD(IPE_OPMODE)
} IPE_OPMODE;
*/
typedef enum {
	IPE_OPMODE_D2D             = 0,    ///< D2D
	IPE_OPMODE_IPP             = 1,    ///< IFE->DCE->IPE->IME
	IPE_OPMODE_RDE_IPP         = 2,    ///< RDE + IFE->DCE->IPE->IME
	IPE_OPMODE_RHE_IPP         = 3,    ///< RDE + IFE->DCE->IPE->IME
	IPE_OPMODE_SIE_IPP         = 4,    ///< SIE + IFE->DCE->IPE->IME (all direct)
	IPE_OPMODE_UNKNOWN         = 5,    ///< Undefined mode
	ENUM_DUMMY4WORD(IPE_OPMODE)
} IPE_OPMODE;


/**
    @name Enum IPE output control.
*/
//@{

/**
    IPE DRAM output selection

    Select IPE output data type
\n Used for ipe_changeOutput().
*/
typedef enum {
	IPE_ORIGINAL = 0,           ///< original size, use both Y & C channels
	//IPE_SUBOUT = 1,             ///< subsampled out
	IPE_DIRECTION = 2,          ///< edge direction, use output C channel
	IPE_ETH = 3,                ///< edge thresholding, use output Y channel
	ENUM_DUMMY4WORD(IPE_DMAOSEL)
} IPE_DMAOSEL;

/**
    IPE Direct to IFE2 enable

    Enable IPE direct to IFE2
\n Used for ipe_changeOutput().
*/
typedef enum {
	B_DISABLE = 0,         ///< disable
	B_ENABLE = 1,          ///< enable
	ENUM_DUMMY4WORD(IPE_ENABLE)
} IPE_ENABLE;


/**
    IPE output YCC format selection

    Select IPE YCC output format
\n Used for ipe_changeOutput().
*/
typedef enum {
	IPE_YCC444 = 0,         ///< Select YCC 424 format
	IPE_YCC422 = 1,         ///< Select YCC 422 format
	IPE_YCC420 = 2,          ///< Select YCC 420 format
	IPE_YONLY = 3,
	ENUM_DUMMY4WORD(IPE_INOUT_FORMAT)
} IPE_INOUT_FORMAT;

/**
    IPE output YCC subsample selection

    Select IPE YCC 422 420 subsample method
\n Used for ipe_changeOutput().
*/
typedef enum {
	YCC_DROP_RIGHT = 0,         ///< When 422 or 420 format is chosen, drop right pixel for subsample
	YCC_DROP_LEFT = 1,         ///< When 422 or 420 format is chosen, drop left pixel for subsample
	ENUM_DUMMY4WORD(IPE_HSUB_SEL_ENUM)
} IPE_HSUB_SEL_ENUM;

/**
    IPE H overlap selection for MST

    Select IPE H overlap format

\n Used for ipe_changeInput().
*/
typedef enum {
	HOLAP_AUTO = 0,         ///< Auto calculation of H overlap pixels
	HOLAP_8PX  = 1,         ///< Keep H overlap to 8 pixels
	ENUM_DUMMY4WORD(IPE_HOVLP_SEL_ENUM)
} IPE_HOVLP_SEL_ENUM;

//@}




/**
    @name Enum IPE color channel.
*/
//@{

/**
    IPE color gain range selection

    Select IPE color gain range
\n Used for ipe_changeParam().
*/
typedef enum {
	CGRANGE_2_8 = 0,
	CGRANGE_3_7 = 1,
	ENUM_DUMMY4WORD(IPE_CG_RANGE_ENUM)
} IPE_CG_RANGE_ENUM;

/**
    IPE Gamma color channel selection

    Select IPE Gamma color channel
\n Used for ipe_changeParam().
*/
typedef enum {
	IPE_R_CHANNEL = 0,      ///< Select R channle for setting
	IPE_G_CHANNEL = 1,     ///< Select G channle for setting
	IPE_B_CHANNEL = 2,      ///< Select B channle for setting
	ENUM_DUMMY4WORD(IPE_CHANNEL_ENUM)
} IPE_CHANNEL_ENUM;

//@}

/**
    @name Enum IPE edge param.
*/
//@{

/**
    IPE edge extract gamma selection

    Select pre or post gamma for edge extract
\n Used for ipe_changeParam().
*/
typedef enum {
	EEXT_PRE_GAM = 0,       ///< Select pre-gamma edge extraction
	EEXT_POST_GAM = 1,     ///< Select post-gamma edge extraction
	ENUM_DUMMY4WORD(IPE_EEXT_GAM_ENUM)
} IPE_EEXT_GAM_ENUM;

/**
    IPE edge extract channel selection

    Select G or Y for edge extract
\n Used for ipe_changeParam().
*/
typedef enum {
	EEXT_G_CHANNEL = 0,       ///< Select G edge extraction
	EEXT_Y_CHANNEL = 1,     ///< Select Y edge extraction
	ENUM_DUMMY4WORD(IPE_EEXT_CH_ENUM)
} IPE_EEXT_CH_ENUM;

/**
    IPE edge mapping input selection

    Select intput data for edge mapping
\n Used for ipe_changeParam().
*/
typedef enum {
	EIN_ESD = 0,       ///< Select EEXT2 for edge mapping input
	EIN_E57 = 1,       ///< Select EEXT1 for edge mapping input
	EIN_AVG = 2,       ///< Select average of EEXT & EEXT2 for edge mapping input
	ENUM_DUMMY4WORD(IPE_EMAP_IN_ENUM)
} IPE_EMAP_IN_ENUM;

/**
    IPE edge mapping output selection

    Select output data for edge mapping
\n Used for ipe_changeParam().
*/
typedef enum {
	EOUT_EEXT = 0,      ///< Select weighted output of 5x5 kernel A and 7x7 for edge mapping output
	EOUT_EEXT2 = 1,     ///< Select 5x5 kernal B for edge mapping output
	ENUM_DUMMY4WORD(IPE_EMAP_OUT_ENUM)
} IPE_EMAP_OUT_ENUM;

/**
    IPE directed edge: y diff range

    Select y diff range

*/
typedef enum {
	YDR_9x9 = 0,      ///< Select 9x9 for y diff calculation
	YDR_7x7 = 1,      ///<  Select 7x7 for y diff calculation
	YDR_5x5 = 2,      ///<  Select 5x5 for y diff calculation
	YDR_3x3 = 3,      ///<  Select 3x3 for y diff calculation
	ENUM_DUMMY4WORD(IPE_YDRANGE)
} IPE_YDRANGE;

/**
    IPE directed edge: eext kernel selection

    Select directed eext kernel

*/
typedef enum {
	KER_A = 0,      ///< kernel A
	KER_B = 1,      ///< kernel B
	ENUM_DUMMY4WORD(IPE_KERSEL)
} IPE_KERSEL;

/**
    IPE directed edge: connection score

    Select connection score of directed edge

*/
typedef enum {
	DE_ADD1 = 0,      ///< add 1
	DE_ADD2 = 1,      ///< add 2
	ENUM_DUMMY4WORD(IPE_DECONSCR)
} IPE_DECONSCR;

/**
    IPE directed edge: index selection in connection

    Select index selection in connection of directed edge

*/
typedef enum {
	DIR_COMPLEX = 0,      ///< as complex area
	DIR_FLAT = 1,         ///< as flat area
	ENUM_DUMMY4WORD(IPE_DECON_IDXSEL)
} IPE_DECON_IDXSEL;

/**
    IPE edge weighting: kernal B output selection

    Select kernal B input sources

*/
typedef enum {
	AVG_HV = 0,         ///< (ED0+ED1)>>1
	AVG_4D = 1,         ///< (ED0+ED1+ ED2+ED3)>>2
	ENUM_DUMMY4WORD(IPE_EW_KBOUTSEL)
} IPE_EW_KBOUTSEL;

//@}

/**
    IPE RGB LPF: kernal size selection

    Select kernel size of low-pass filter

*/
typedef enum {
	LPFSIZE_3X3 = 0,    ///< 3x3 filter size
	LPFSIZE_5X5 = 1,    ///< 5x5 filter size
	LPFSIZE_7X7 = 2,    ///< 7x7 filter size
	ENUM_DUMMY4WORD(IPE_LPF_SIZE_ENUM)
} IPE_LPF_SIZE_ENUM;

/**
    IPE CC range: precision selection

    Select precision 2.8 or 3.7 of color correction

*/
typedef enum {
	CCRANGE_2_9 = 0,    ///< Precision 2.9
	CCRANGE_3_8 = 1,    ///< Precision 3.8
	CCRANGE_4_7 = 2,    ///< Precision 4.7
	ENUM_DUMMY4WORD(IPE_CC_RANGE_ENUM)
} IPE_CC_RANGE_ENUM;

/**
    IPE CC output: weighting reference

    Select identity or gray for output weighting

*/
typedef enum {
	CC2_IDENTITY = 0,   ///< Identity matrix
	CC2_GRAY = 1,       ///< Gray matrix
	ENUM_DUMMY4WORD(IPE_CC2_SEL_ENUM)
} IPE_CC2_SEL_ENUM;

/**
    IPE color correction gamma selection

    Select pre or post gamma for color correction
*/
typedef enum {
	CC_PRE_GAM = 0,       ///< Select pre-gamma color correction
	CC_POST_GAM = 1,      ///< Select post-gamma color correction
	ENUM_DUMMY4WORD(IPE_CC_GAMSEL)
} IPE_CC_GAMSEL;

/**
    IPE color correction stab option

    Select input of color correction stab
*/
typedef enum {
	CC_MAX = 0,             ///< Select max
	CC_MIN = 1,             ///< Select min
	CC_MAX_MINUS_MIN = 2,   ///< Select max-min
	ENUM_DUMMY4WORD(IPE_CC_STABSEL)
} IPE_CC_STABSEL;

/**
    IPE color correction stab option

    Select input of color correction stab
*/
typedef enum {
	CST_NOOP = 0,             ///< No operation
	CST_MINUS128 = 1,         ///< out = in - 128
	ENUM_DUMMY4WORD(IPE_CST_OFFSEL)
} IPE_CST_OFFSEL;

/**
    IPE color control source option

    Select edge reference of color control vdet
*/
typedef enum {
	CCTRL_1STORDER = 0,     ///< 5x5 kernel B output
	CCTRL_E5 = 1,           ///< 5x5 kernel A output
	CCTRL_E7 = 2,           ///< 7x7 kernel output
	ENUM_DUMMY4WORD(IPE_CCTRL_SEL_ENUM)
} IPE_CCTRL_SEL_ENUM;

/**
    IPE YC thresholding: replacement option

    Select replacement with the original or a value specified by the register
*/
typedef enum {
	YCTH_ORIGINAL_VALUE = 0,
	YCTH_REGISTER_VALUE = 1,
	ENUM_DUMMY4WORD(IPE_YCTH_SEL_ENUM)
} IPE_YCTH_SEL_ENUM;

/**
    IPE 3DCC rounding

    Select 3DCC rounding type
*/
typedef enum {
	ROUNDING = 0,             ///< rounding
	HALFTONE = 1,             ///< halftone rounding
	RANDOM = 2,               ///< random rounding
	ENUM_DUMMY4WORD(IPE_3DCC_ROUNDOPT)
} IPE_3DCC_ROUNDOPT;

/**
    IPE ETH ouput downsampling

    Select if ETH is downsampled at output
*/
typedef enum {
	ETH_OUT_ORIGINAL = 0,             ///< original size
	ETH_OUT_DOWNSAMPLED = 1,          ///< downsampled by 2x2
	ENUM_DUMMY4WORD(IPE_ETH_OUTSEL)
} IPE_ETH_OUTSEL;

/**
    IPE ETH ouput format

    Select ETH output bitdepth
*/
typedef enum {
	ETH_OUT_2BITS = 0,             ///< 2 bits/pixel
	ETH_OUT_8BITS = 1,             ///< 8 bits/pixel
	ENUM_DUMMY4WORD(IPE_ETH_OUTFMT)
} IPE_ETH_OUTFMT;

/**
    IPE ETH ouput position

    Select ETH output position when downsampling
*/
typedef enum {
	ETH_POS_EVEN = 0,             ///< even position
	ETH_POS_ODD = 1,              ///< odd position
	ENUM_DUMMY4WORD(IPE_ETH_POSSEL)
} IPE_ETH_POSSEL;


/**
    @name Enum IPE wait frame end selection.
*/
//@{

/**
    IPE wait frame-end selection

    Select clear flag or not before waiting for IPE frame-end
\n Used for ipe_waitFrameEnd().
*/
typedef enum {
	IPE_NOCLRFLG = 0,           ///< Select not to clear previous flag before waiting for frame-end
	IPE_CLRFLG = 1,             ///< Select to clear previous flag before waiting for frame-end
	ENUM_DUMMY4WORD(IPE_CLR_FLG_ENUM)
} IPE_CLR_FLG_ENUM;
//@}

/**
    @name Enum IPE function setting mode.
*/
//@{
/**
    IPE function set/enable/disable selection

    Select IPE function update mode
\n Used for ipe_changeParam().
*/
typedef enum {
	IPE_FUNC_ENABLE = 0,        ///< Enable functions, FuncEn |= FuncSel, 1s in FuncSel are enabled
	IPE_FUNC_DISABLE = 1,       ///< Disable functions, FuncEn &= (~FuncSel), 1s in FuncSel are disabled
	IPE_FUNC_SET = 2,           ///< Set functions, FuncEn = FuncSel, 1s in FuncSel are enabled while 0s in FuncSel are disabled
	IPE_FUNC_NOUPDATE = 3,      ///< Don't update functions, FuncEn = FuncEn, disregard FuncSel
	ENUM_DUMMY4WORD(IPE_FUNC_UPDATE_ENUM)
} IPE_FUNC_UPDATE_ENUM;

//@}

/**
    @name Enum IPE wait selection.
*/
//@{
/**
    IPE D2D wait frame-end selection

    Select wait frame-end or not for Dram2Dram operation
\n Used for ipe_Dram2Dram().
*/
typedef enum {
	IPE_WAIT = 0,       ///< Wait for frame-end before exit api
	IPE_NOWAIT = 1,     ///< Don't wait for frame-end in api, external ipe_Dram2DramWaitEnd() should be called to make sure frame operation ends
	ENUM_DUMMY4WORD(IPE_WAIT_ENUM)
} IPE_WAIT_ENUM;
//@}


/**
    @name Enum IPE wait selection.
*/
//@{
/**
    IPE burst length selection

    Select input/output burst length mode.
\n Used for ipe_Dram2Dram().
*/
typedef enum {
	IPE_BURST_32W    = 0,       ///< burst length 32 words
	IPE_BURST_16W    = 1,       ///< burst length 16 words
	ENUM_DUMMY4WORD(IPE_BURST_SEL)
} IPE_BURST_SEL;
//@}


//######################################################
// the following for structure define

/**
    IPE structure - IPE input/output burst length
*/
//@{
typedef struct _IPE_BURST_LENGTH {
	IPE_BURST_SEL   BurstLenInputY;  ///< Input burst length
	IPE_BURST_SEL   BurstLenInputC;  ///< Input burst length
	IPE_BURST_SEL   BurstLenOutputY; ///< Output burst length
	IPE_BURST_SEL   BurstLenOutputC; ///< Output burst length
} IPE_BURST_LENGTH;
//@}

/**
    IPE structure - IPE size.
*/
//@{
typedef struct {
	UINT32    uiHSize;    ///< Horizontal image size, unit : pix
	UINT32    uiVSize;    ///< Vertical image size, uint : line
} STR_IMG_SIZE;
//@}

/**
    IPE structure - IPE stripe.
*/
//@{
typedef struct {
	UINT32    uiN;        ///< Size of stripe except for last one, H unit : 16 pix, V unit : 1 line
	UINT32    uiL;        ///< Size of last stripe, H unit : 16 pix, V unit : 1 line
	UINT32    uiM;        ///< Number of total stripe - 1
	//IPE_OLAP_ENUM    Olap; ///< Overlap enable selection for multi-stripe mode
} STR_STRIPE_INFOR;
//@}

/**
    IPE structure - IPE DRAM input addresses.
*/
//@{
typedef struct {
	UINT32    uiSai0;     ///< input address for Y
	UINT32    uiSai1;     ///< input address for C
	UINT32    uiSai2;     ///< input address for Y curve
	UINT32    uiSai3;     ///< input address for Gamma
	UINT32    uiSai4;     ///< input address for 3DCC LUT
} IPE_DRAMSAI;
//@}



/**
    IPE structure - IPE directed edge eext
*/
//@{
typedef struct {
	IPE_YDRANGE YDRange;            ///< range of calculation y difference
	UINT32      uiYDMaxTh;          ///< threshold of max y difference
	IPE_ENABLE  PndAdjEn;           ///< enable of adjusting PD/ND edges
	UINT32      uiPndShft;          ///< PNDEdge = PNDedge - (PNDedge>>(uiPndShft+1))
	UINT32      uiES;               ///< Edge shift for all directions
	IPE_KERSEL  EdirSel;            ///< Select kernel of directed eext
} IPE_DIREDG_EEXT;
//@}

/**
    IPE structure - IPE directed edge score calculation
*/
//@{
typedef struct {
	INT8        iA0;              ///< a coefficient of score 0
	INT8        iB0;              ///< b coefficient of score 0
	INT8        iC0;              ///< c coefficient of score 0
	UINT8       uiD0;             ///< d coefficient of score 0
	INT8        iA1;              ///< a coefficient of score 1
	INT8        iB1;              ///< b coefficient of score 1
	INT8        iC1;              ///< c coefficient of score 1
	UINT8       uiD1;             ///< d coefficient of score 1
	INT8        iA2;              ///< a coefficient of score 2
	INT8        iB2;              ///< b coefficient of score 2
	INT8        iC2;              ///< c coefficient of score 2
	UINT8       uiD2;             ///< d coefficient of score 2
} IPE_DIREDG_SCORECAL;
//@}

/**
    IPE structure - IPE directed edge score calculation
*/
//@{
typedef struct {
	UINT8       uiScoreTh0;       ///< score threshold 0
	UINT8       uiScoreTh1;       ///< score threshold 1
	UINT8       uiScoreTh2;       ///< score threshold 2
	UINT8       uiScoreTh3;       ///< score threshold 3
} IPE_DIREDG_SCORETH;
//@}

/**
    IPE structure - IPE directed edge connection parameters
*/
//@{
typedef struct {
	IPE_DECONSCR ScrCenter;        ///< score of center
	IPE_DECONSCR ScrEdg;           ///< score of edges
	IPE_DECONSCR ScrDetail;        ///< score of details
	UINT8        uiCntNumTh;       ///< threshold of count number
	UINT8        uiMax1Th;         ///< threshold of max 1
	UINT8        uiMaxCntTh;       ///< threshold of max count
	UINT8        uiMax2Th;         ///< threshold of max 2
	IPE_DECON_IDXSEL IdxSel;       ///< index selection
} IPE_DIREDG_CONPARAM;
//@}

/**
    IPE structure - IPE edge weighting paramters of kernel A
*/
//@{
typedef struct {
	IPE_KERSEL   EWKer2DSel;       ///< 2D kernel selection of edge weighting
	UINT8        uiAdwt0;          ///< kernal A's dir weight 0
	UINT8        uiAdwt1;          ///< kernal A's dir weight 1
	UINT8        uiAdwt2;          ///< kernal A's dir weight 2
	UINT8        uiAdwt3;          ///< kernal A's dir weight 3
	UINT8        uiAdwt4;          ///< kernal A's dir weight 4
	UINT8        uiAdwt5;          ///< kernal A's dir weight 5
	INT8         iAds0;            ///< kernal A's dir shift 0
	INT8         iAds1;            ///< kernal A's dir shift 1
	INT8         iAds2;            ///< kernal A's dir shift 2
	INT8         iAds3;            ///< kernal A's dir shift 3
	INT8         iAds4;            ///< kernal A's dir shift 4
	INT8         iAds5;            ///< kernal A's dir shift 5
} IPE_EWKERA;
//@}

/**
    IPE structure - IPE edge weighting paramters of kernel B
*/
//@{
typedef struct {
	UINT8           uiBdwt0;          ///< kernal B's dir weight 0
	UINT8           uiBdwt1;          ///< kernal B's dir weight 1
	INT8            iBds0;            ///< kernal B's dir shift 0
	INT8            iBds1;            ///< kernal B's dir shift 1
	IPE_EW_KBOUTSEL EwKerBOutSel;     ///< Kernal B's output selection
} IPE_EWKERB;
//@}

/**
    IPE structure - IPE edge weighting paramters
*/
//@{
typedef struct {
	UINT8           uiEWDir0;         ///< weighting 0 with E7
	UINT8           uiEWDir1;         ///< weighting 1 with E7
	UINT8           uiEWDir2;         ///< weighting 2 with E7
	IPE_ENABLE      DynEMEn;          ///< enable of dynamic e-mapping
} IPE_EW_PARAM;
//@}


/**
    IPE structure - IPE edge map
*/
//@{
typedef struct {
	UINT32     uiEthrLow;                   ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=EDTabA[0]}, else if(EthrA <= Ein <EthrB){Eout apply EtabA and EDtabA}, else if(Ein >= EthrB){Eout apply EtabB and EDtabB}
	UINT32     uiEthrHigh;                  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=EDTabA[0]}, else if(EthrA <= Ein <EthrB){Eout apply EtabA and EDtabA}, else if(Ein >= EthrB){Eout apply EtabB and EDtabB}
	UINT32     uiEtabLow;                   ///< Legal range : 0~6, for edge mapping, EDtabA table has 8 sections, each section width is (1 << etaA)
	UINT32     uiEtabHigh;                  ///< Legal range : 0~6, for edge mapping, EDtabB table has 8 sections, each section width is (1 << etaB)
	IPE_EMAP_IN_ENUM EinSel;                ///< Edge mapping input selection
} IPE_EDGEMAP_INFOR;
//@}

/**
    IPE structure - IPE luminance map
*/
//@{
typedef struct {
	UINT32     uiEthrLow;                   ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=EDTabA[0]}, else if(EthrA <= Ein <EthrB){Eout apply EtabA and EDtabA}, else if(Ein >= EthrB){Eout apply EtabB and EDtabB}
	UINT32     uiEthrHigh;                  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=EDTabA[0]}, else if(EthrA <= Ein <EthrB){Eout apply EtabA and EDtabA}, else if(Ein >= EthrB){Eout apply EtabB and EDtabB}
	UINT32     uiEtabLow;                   ///< Legal range : 0~6, for edge mapping, EDtabA table has 8 sections, each section width is (1 << etaA)
	UINT32     uiEtabHigh;                  ///< Legal range : 0~6, for edge mapping, EDtabB table has 8 sections, each section width is (1 << etaB)
} IPE_ESMAP_INFOR;
//@}


/**
    IPE structure - IPE RGB LPF
*/
//@{
typedef struct {
	UINT32 uiLpfW;              ///< LPF weighting
	UINT32 uiSonlyW;            ///< S-Only weighting
	UINT32 uiRangeTh0;          ///< Range threshold 0
	UINT32 uiRangeTh1;          ///< Range threshold 1
	IPE_LPF_SIZE_ENUM FiltSize;
} STR_RGBLPF_INFOR;
//@}

/**
    IPE structure - IPE Y threshold 1
*/
//@{
typedef struct {
	UINT32 uiYth;               ///< Y threshold
	UINT32 uiEth;               ///< Edge threshold
	IPE_YCTH_SEL_ENUM HitSel;   ///< Value selection when hit
	IPE_YCTH_SEL_ENUM NHitSel;  ///< Value selection when not-hit
	UINT32 uiHitValue;          ///< The register value when hit
	UINT32 uiNHitValue;         ///< The register value when not-hit
} STR_YTH1_INFOR;
//@}

/**
    IPE structure - IPE Y threshold 2
*/
//@{
typedef struct {
	UINT32 uiYth;               ///< Y threshold
	IPE_YCTH_SEL_ENUM HitSel;   ///< Value selection when hit
	IPE_YCTH_SEL_ENUM NHitSel;  ///< Value selection when not-hit
	UINT32 uiHitValue;          ///< The register value when hit
	UINT32 uiNHitValue;         ///< The register value when not-hit
} STR_YTH2_INFOR;
//@}

/**
    IPE structure - IPE C threshold
*/
//@{
typedef struct {
	UINT32 uiEth;               ///< Edge threshold
	UINT32 uiYthL;              ///< Low threshold of Y
	UINT32 uiYthH;              ///< High threshold of Y
	UINT32 uiCBthL;             ///< Low threshold of CB
	UINT32 uiCBthH;             ///< High threshold of CB
	UINT32 uiCRthL;             ///< Low threshold of CR
	UINT32 uiCRthH;             ///< High threshold of CR
	IPE_YCTH_SEL_ENUM HitSel;   ///< Value selection when hit
	IPE_YCTH_SEL_ENUM NHitSel;  ///< Value selection when not-hit
	UINT32 uiCBHitValue;        ///< The register value of CB when hit
	UINT32 uiCBNHitValue;       ///< The register value of CB when not-hit
	UINT32 uiCRHitValue;        ///< The register value of CR when hit
	UINT32 uiCRNHitValue;       ///< The register value of CR when not-hit
} STR_CTH_INFOR;
//@}

/**
    IPE structure - IPE subsampling
*/
//@{
typedef struct {
	UINT32 uiHIntFact;          ///< Horizontal scaling factor: integer
	UINT32 uiHFracFact;         ///< Horizontal scaling factor: fraction
	UINT32 uiHsize;             ///< Output Horizontal size
	UINT32 uiVIntFact;          ///< Vertical scaling factor: integer
	UINT32 uiVFracFact;         ///< Vertical scaling factor: fraction
	UINT32 uiVsize;             ///< Output vertical size
} STR_SUBOUT_INFOR;
//@}

/**
    IPE structure - IPE random input of gamma and y curve
*/
//@{
typedef struct {
	IPE_ENABLE GamYRandEn;  ///< Enable of random input
	IPE_ENABLE GamYRandRst; ///< Reset of random input
	UINT8 uiGamYRandShft;   ///< Shift of random input
} IPE_GAMYRAND;
//@}

/**
    IPE structure - IPE 3DCC rounding
*/
//@{
typedef struct {
	IPE_3DCC_ROUNDOPT RndOpt;   ///< Rounding option
	IPE_ENABLE RndRst;          ///< Reset of random input
} IPE_3DCCROUND;
//@}

/**
    IPE structure - Output information of IPE Edge Thresholding
*/
//@{
typedef struct {
	IPE_ETH_OUTSEL EthOutSel;   ///< If ETH output is downsampled
	IPE_ETH_OUTFMT EthOutFmt;   ///< ETH output format
} IPE_ETH_SIZE;
//@}

/**
    IPE structure - Parameters of IPE Edge Thresholding
*/
//@{
typedef struct {
	UINT32 EthLow;              ///< Low edge threshold value
	UINT32 EthMid;              ///< Middle edge threshold value
	UINT32 EthHigh;             ///< High edge threshold value
	IPE_ETH_POSSEL EthHout;     ///< Horizontal position selection when downsampling
	IPE_ETH_POSSEL EthVout;     ///< Vertical position selection when downsampling
} IPE_ETH_PARAM;
//@}



//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

/**
    IPE structure - IPE edge extraction.
*/
//@{
typedef struct {
	IPE_EEXT_GAM_ENUM EextGamSel;       ///< Select pre- or post-gamma edge extraction
	IPE_EEXT_CH_ENUM EextCHSel;         ///< Edge input channel selection
	INT16 *piEdgeKerDiv;                ///< Edge extraction kernel setting
	//UINT32 uiEWDir;                   ///< Legal range : 0~16, 0 : 5x5 user defined edge , 16 : directed edge
	//IPE_EDIR_EEXT_ENUM EDirSel;       ///< Edge kernel selection for directed edge
	UINT32 uiEDirDiv;                   ///< Legal range : 0~3, 0: Normal division for directed edge kernel, 1: -1 division, 2: -2 division, 3: -3 division
	UINT8 *puiEDirTab;                  ///< Legal range : 0~8, wieghting for directed edge, 0 : no direction decision, 8 : sensitive edge decision
	UINT32 *puiToneMapLut;              ///< Tone remap LUT
	//STR_ESD_INFOR *EsdInfo;
	IPE_DIREDG_EEXT *pDirEdgEext;       ///< Directed edge extraction setting
	IPE_DIREDG_SCORECAL *pDirEdgScrCal; ///< Directed edge score calculation setting
	IPE_DIREDG_SCORETH *pDirEdgScrTh;   ///< Directed edge score thresholds
	IPE_ENABLE PowerSav;                ///< Enable of power-saving mode
	IPE_DIREDG_CONPARAM *pDirEdgCon;    ///< Directed edge connection setting
	IPE_EWKERA *pEWA;                   ///< Directed edge weighting of kernal A
	IPE_EWKERB *pEWB;                   ///< Directed edge weighting of kernal B
	IPE_EW_PARAM *pEWParam;             ///< Weighting of directed edge and 7x7
} IPE_EEXT_PARAM;
//@}

/**
    IPE structure - IPE edge process.
*/
//@{
typedef struct {
	IPE_EDGEMAP_INFOR *pEdgeMapTh;      ///< ED map thresholds
	UINT8 *puiEdgeMapLut;               ///< ED map LUT(16 entries). Legal range: 0~255. 1X = 255
	//STR_IPE_EDGEMAP_INFOR *pEsdMapTh;
	//UINT8 *puiEsdMapLut;
	IPE_ESMAP_INFOR *pEsMapTh;          ///< ES map thresholds
	UINT8 *puiESMapLut;                 ///< ES map LUT(16 entries). Legal range: 0~255. 1X = 64
	IPE_EMAP_OUT_ENUM EOutSel;          ///< Edge process output selection
} IPE_EPROC_PARAM;
//@}

/**
    IPE structure - IPE RGB LPF
*/
//@{
typedef struct {
	STR_RGBLPF_INFOR    RlpfInfor;  ///< LPF setting of R channel
	STR_RGBLPF_INFOR    GlpfInfor;  ///< LPF setting of G channel
	STR_RGBLPF_INFOR    BlpfInfor;  ///< LPF setting of B channel
} IPE_RGBLPF_PARAM;
//@}

/**
    IPE structure - IPE CC.
*/
//@{
typedef struct {
	IPE_CC_RANGE_ENUM CCRange;  ///< Range of matrix coefficients,0: 3.8 format,1: 4.7format
	IPE_CC2_SEL_ENUM CC2Sel;    ///< Select the other data of CC weighting
	IPE_CC_GAMSEL CCGamSel;     ///< Select pre- or post-gamma CC
	IPE_CC_STABSEL CCStabSel;   ///< Select CC stab input
	INT16 *piCoef;              ///< Color correction matrix [rr rg rb gr gg gb br bg bb]
	UINT8 *puiFstab;            ///< Color correction stab
	UINT8 *puiFdtab;            ///< Color correction dtab
} IPE_CC_PARAM;
//@}

/**
    IPE structure - IPE CST.
*/
//@{
typedef struct {
	INT16 *piCSTcoef;            ///< Color space transform matrix [yr yg yb ur ug ub vr vg vb]
	IPE_CST_OFFSEL CstOffSel;    ///< Operation when CST is off
} IPE_CST_PARAM;
//@}

/**
    IPE structure - IPE CCTRL.
*/
//@{
typedef struct {
	INT16 iIntOfs;                  ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
	INT16 iSatOfs;                  ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
	BOOL bHueRotate;                ///< Hue adjust rotation option, 0 : NO rotation, 1 : CHUEM[n] LSB 2 bit = 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation
	BOOL bHueC2G;                   ///< Select G ch for hue calculation, 0: input is G channel, 1: input is C channel
	IPE_CCTRL_SEL_ENUM CctrlSel;    ///< Color control: vdet source selection
	UINT8 uiVdetDiv;                ///< Vdet weighting. out =  in*vdetdiv>>8
	UINT8 *puiHueTab;               ///< Legal range : 0~255, color control hue adjust table, 128 : NO hue adjust
	INT32 *piSatTab;                ///< Legal range : -128~127, color control sat adjust table, 0 : NO sat adjust, -128 : Min sat suppress, 127 : Max sat enhance
	INT32 *piIntTab;                ///< Legal range : -128~127, color control int adjust table, 0 : NO int adjust, -128 : Min int suppress, 127 : Max int enhance
	UINT8 *puiEdgTab;               ///< Legal range : 0~255, color control edge adjust table, 128 : NO edge adjust
	UINT8 *puiDDSTab;               ///< Legal range : 0~255, color control weighting table for SatTab, IntTab, and EdgTab, 0 : NO adjust, 32 : 1X adjust weighting
} IPE_CCTRL_PARAM;
//@}

/**
    IPE structure - IPE CADJ Eenh.
*/
//@{
typedef struct {
	UINT32 uiEenhP;         ///< Positive edge enhance weighting. Legal range: 0~1023. 1X = 64.
	UINT32 uiEenhN;         ///< Negative edge enhance weighting. Legal range: 0~1023. 1X = 64.
} IPE_CADJ_EENH_PARAM;
//@}

/**
    IPE structure - IPE CADJ edge inverse.
*/
//@{
typedef struct {
	BOOL bEInvP; ///< Legal range : 0~1, if EInvP is 1, positive edge(overshoot) is inverted to negative edge(undershoot)
	BOOL bEInvN; ///< Legal range : 0~1, if EInvN is 1, negative edge(undershoot) is inverted to positive edge(overshoot)
} IPE_CADJ_EINV_PARAM;
//@}

/**
    IPE structure - IPE CADJ YC contrast.
*/
//@{
typedef struct {
	UINT8 uiYCon;    ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
	UINT8 uiCCon;    ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
} IPE_CADJ_YCCON_PARAM;
//@}

/**
    IPE structure - IPE CADJ CbCr offset.
*/
//@{
typedef struct {
	UINT8 uiCbOfs;    ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
	UINT8 uiCrOfs;    ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
} IPE_CADJ_COFS_PARAM;
//@}

/**
    IPE structure - IPE noise effect.
*/
//@{
typedef struct {
	IPE_ENABLE YRandEn;     ///< Enable of random noise on Y
	IPE_ENABLE CRandEn;     ///< Enable of random noise on C
	UINT32 uiYRandLevel;    ///< Legal range : 0~7, 0 : NO Y random noise, 7 : MAX Y random noise level
	UINT32 uiCRandLevel;    ///< Legal range : 0~7, 0 : NO CbCr random noise, 7 : MAX CbCr random noise level
	BOOL bRandReset;        ///< Legal range : 0~1, 0 : No reset for random pattern sequence, 1 : Frame start reset for random pattern seqeunce.
} IPE_CADJ_RAND_PARAM;
//@}

/**
    IPE structure - IPE YC fix effect.
*/
//@{
typedef struct {
	STR_YTH1_INFOR *pYth1;      ///< Y threshold setting 1
	STR_YTH2_INFOR *pYth2;      ///< Y threshold setting 2
	STR_CTH_INFOR *pCth;        ///< C threshold setting
} IPE_CADJ_FIXTH_PARAM;
//@}

/**
    IPE structure - IPE YC mask effect.
*/
//@{
typedef struct {
	UINT8 uiYmask;          ///< Mask of Y channel
	UINT8 uiCBmask;         ///< Mask of CB channel
	UINT8 uiCRmask;         ///< Mask of CR channel
} IPE_CADJ_MASK_PARAM;
//@}

/**
    IPE structure - IPE CSTP effect.
*/
//@{
typedef struct {
	UINT8 uiCSTPRatio;      ///< Ratio of color space transform protection
} IPE_CSTP_PARAM;
//@}

/**
    IPE structure - IPE 3DCC.
*/
//@{
typedef struct {
	UINT32 uiCC3DAddr;          ///< 3DCC DRAM input address
	IPE_3DCCROUND *pCC3dRnd;    ///< 3DCC rounding option
} IPE_3DCC_PARAM;
//@}







//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

/**
    IPE structure - IPE open object.
*/
//@{
typedef struct {
	void (*FP_IPEISR_CB)(UINT32 intstatus); ///< Call back function pointer to hook with IPE interrupt service routine
	UINT32 uiIpeClockSel;                   ///< IPE engine clock selection
} IPE_OPENOBJ;
//@}

/**
    IPE structure - IPE input info.
*/
//@{
typedef struct {
	IPE_OPMODE IpeMode;             ///< IPE operation mode
	STR_IMG_SIZE InSizeY;           ///< Input image H V size, effective if StripeMode is IPE_SSTP or IPE_AUTOSTP. If StripeMode is IPE_MSTP, HSTRIPE and VSTRIPE must be input
	UINT32 uiAdr0;                  ///< Legal range : multiples of 4(word aligned), IPE input address ID 0
	UINT32 uiAdr1;                  ///< Legal range : multiples of 4(word aligned), IPE input address ID 1
	IPE_INOUT_FORMAT InFmt;         ///< Input YCC format
	IPE_INOUT_FORMAT OutFmt;        ///< Output YCC format
	UINT32 uiLofsIn0;               ///< Legal range : multiples of 4(word aligned), IPE input image DRAM line offset 0
	UINT32 uiLofsIn1;               ///< Legal range : multiples of 4(word aligned), IPE input image DRAM line offset 1
	IPE_ENABLE InRandEn;            ///< Enable of YCC random input
	IPE_ENABLE InRandRst;           ///< Reset of YCC random input
	BOOL bOutYCToDRAM;              ///< Enable of YC data to DRAM
	IPE_DMAOSEL DmaOutSel;          ///< DMA output channel data selection
	IPE_ETH_SIZE EthOutInfo;      ///< ETH output information
	IPE_HOVLP_SEL_ENUM HovlpOpt;       ///< Choose IPE H overlap to fixed 8 pixels (when MST + SQUARE)
} IPE_INPUTINFO;
//@}

/**
    IPE structure - IPE output info.
*/
//@{
typedef struct {
	BOOL bOutYCToIME;               ///< Enable of YC data to IME
	BOOL bOutYCToDRAM;              ///< Enable of YC data to DRAM
	IPE_INOUT_FORMAT YCFormat;      ///< IPE DRAM output source data selection
	IPE_HSUB_SEL_ENUM SubHSel;      ///< IPE output YCC subsample selection
	UINT32 uiAdrY0;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
	UINT32 uiAdrC0;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
	UINT32 uiLofsY;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
	UINT32 uiLofsC;                 ///< Legal range : multiples of 4(word aligned), DRAM output address, data type selected by OutSrcSel
	//UINT32 uiOutHeight;
	IPE_DMAOSEL DmaOutSel;          ///< DMA output channel data selection
	//UINT32 uiInYSizeH;              ///< Horizontal size of input Y
	//UINT32 uiInYSizeV;              ///< Vertical size of input Y
	//UINT32 uiOutYSizeH;             ///< Horizontal size of output Y
	//UINT32 uiOutYSizeV;             ///< Vertical size of output Y
	//IPE_ENABLE DirIfe2En;           ///< Enable of YC data direct to IFE2
	IPE_ETH_SIZE EthOutInfo;        ///< ETH output information
} IPE_OUTYCINFO;
//@}

/**
    IPE structure - IPE IQ info.
*/
//@{
typedef struct {
	UINT32 uiFuncSel;                           ///< IPE function set/enable/disable selection, combined usage with FuncUpdateSel
	IPE_FUNC_UPDATE_ENUM FuncUpdateSel;         ///< IPE function set/enable/disable selection
	UINT32 uiParamUpdateSel;                    ///< IPE parameters update selection, combined usage with definition IPE_SET_XXX
	//IPE_REGTYPE RegType;
	IPE_EEXT_PARAM *pEextParam;                 ///< Edge extraction parameters
	IPE_EPROC_PARAM *pEprocParam;               ///< Edge process parameters
	IPE_RGBLPF_PARAM *pRGBLpfParam;             ///< RGB LPF parameters
	IPE_CC_PARAM *pCCParam;                     ///< Color correction parameters
	IPE_CST_PARAM *pCSTParam;                   ///< Color space transform parameters
	IPE_CCTRL_PARAM *pCCtrlParam;               ///< Color control parameters
	IPE_CADJ_EENH_PARAM *pCAdjEenhParam;        ///< Color componenet adjustment: edge enhancement parameters
	IPE_CADJ_EINV_PARAM *pCAdjEinvParam;        ///< Color componenet adjustment: edge inversion parameters
	IPE_CADJ_YCCON_PARAM *pCAdjYCConParam;      ///< Color componenet adjustment: YC contrast parameters
	IPE_CADJ_COFS_PARAM *pCAdjCofsParam;        ///< Color componenet adjustment: color offset parameters
	IPE_CADJ_RAND_PARAM *pCAdjRandParam;        ///< Color componenet adjustment: random noise parameters
	IPE_CADJ_FIXTH_PARAM *pCAdjFixThParam;      ///< Color componenet adjustment: YC threshold parameters
	IPE_CADJ_MASK_PARAM *pCAdjMaskParam;        ///< Color componenet adjustment: YC mask parameters
	IPE_CSTP_PARAM *pCstpParam;                 ///< Color space transform protection parameters
	//IPE_SUB_PARAM *pSubParam;
	//IPE_DIRIFE2_PARAM *pDirIFE2;
	UINT32 uiGammaAddr;                         ///< Dram input address of Gamma
	IPE_GAMYRAND *pGamYcurRand;                 ///< Gamma and Y curve: random output parameters
	UINT32 uiYCurveAddr;                        ///< Dram input address of Y curve
	IPE_3DCC_PARAM *p3dccParam;                 ///< 3DCC parameters
	IPE_ETH_PARAM *pEth;                          ///< Edge thresholding parameters
} IPE_IQINFO;
//@}

/**
    IPE structure - IPE mode info.
*/
//@{
typedef struct {
	UINT32 uiIntEn;                 ///< Interrupt enable selection
	IPE_INPUTINFO InInfo;           ///< IPE DRAM input related parameters
	IPE_OUTYCINFO OutInfo;          ///< IPE DRAM output related parameters
	IPE_IQINFO IQInfo;              ///< IPE IQ related parameters
} IPE_MODEINFO;
//@}

extern void ipe_clrFrameEnd(void);
extern void ipe_waitFrameEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern void ipe_clrDmaEnd(void);
extern void ipe_waitDmaEnd(IPE_CLR_FLG_ENUM ClrFlgSel);
extern void ipe_clrGammaInEnd(void);
extern void ipe_waitGammaInEnd(void);
extern void ipe_clr3DLutInEnd(void);
extern void ipe_wait3DLutInEnd(void);
extern ER ipe_open(IPE_OPENOBJ *PIPE_OpenInfo);
extern ER ipe_close(void);
extern UINT32 ipe_getClockRate(void);
extern ER ipe_changeParam(IPE_IQINFO *IQinfo);
extern ER ipe_changeInterrupt(UINT32 IntEn);
extern ER ipe_changeInput(IPE_INPUTINFO *InInfo);
extern ER ipe_changeOutYC(IPE_OUTYCINFO *OutInfo);
extern ER ipe_setMode(IPE_MODEINFO *ModeInfo);
extern ER ipe_changeAll(IPE_MODEINFO *ModeInfo, IPE_REGTYPE RegType);
extern ER ipe_start(void);
extern ER ipe_pause(void);
extern ER ipe_reset(BOOL ResetEn);
extern ER ipe_loadGamma(IPE_RWGAM_OPT opt);
extern ER ipe_load3DLut(void);
extern ER ipe_readGamma(BOOL EnableCPURead);
extern ER ipe_read3DLut(BOOL EnableCPURead);

extern UINT32 ipe_getVersion(void);
extern UINT32 ipe_getIntStatus(void);
extern STR_STRIPE_INFOR ipe_getHstripe(void);
extern STR_STRIPE_INFOR ipe_getVstripe(void);
extern BOOL ipe_checkFunctionEnable(UINT32 ipe_Function);
extern UINT32 ipe_getDebugStatus(void);
extern void ipe_clearInt(unsigned long intr);
extern BOOL ipe_checkBusy(void);
extern UINT16 ipe_getFrmCnt(void);
extern UINT32 ipe_getIntEnable(void);
extern ER ipe_getBurstLength(IPE_BURST_LENGTH *pBurstLen);
//@}

#if (_EMULATION_ == ENABLE)
extern ER ipe_setMode_fix_state(VOID);
#endif

#endif
