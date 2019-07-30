/**
    IPL_Ctrl module(D2D,dram to dram)

    This D2D module use 3 opeartion to model IPL D2D operation:
\n  IPL_D2D_Start   : start engine d2d behavior
\n  IPL_WaitFinish  : wait engine d2d behavior done
\n  IPL_D2D_End     : close engine.

    @file       IPL_Ctrl.h
    @ingroup    mILibIPH
    @note
\n  example of standard operation:
\n
\n          IPL_D2D_Start(...);
\n          IPL_WaitFinish(...);
\n          IPL_D2D_End(...);

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IPL_CTRL_H_
#define _IPL_CTRL_H_
#include "ipl_ctrl_par.h"
#include "ipl_ctrl_flow.h"
#include "ipl_hal_op.h"

#define IPL_SQUARE_PATCH_W_PADDING	16
#define IPL_SQUARE_PATCH_H_PADDING	8
#define IPL_CTRL_TIMEOUT_MS_DFT		(0xFFFFFFFF)
/** \addtogroup mILibIPH */
//@{

/**
     D2D result location.

     decribe current ready data is in InputAddress or OutputAddress.
*/
typedef enum _DataDest {
	_InputADDR  = 0,    ///< d2d output data in inputaddr
	_OutputADDR = 1,    ///< d2d output data in outputaddr
	ENUM_DUMMY4WORD(DataDest)
} DataDest;


/**
     SIE data structure for SIE_D2DWait

     SIE D2D output information.
*/
typedef struct _IPLC_SIE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_SIE_OUT_INFO;

/**
     IFE data structure for IFE_D2DWait

     IFE D2D output information.
*/
typedef struct _IPLC_IFE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IFE_OUT_INFO;

/**
     DCE data structure for DCE_D2DWait

     DCE D2D output information.
*/
typedef struct _IPLC_DCE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_DCE_OUT_INFO;

/**
     IPE data structure for IPE_D2DWait

     IPE D2D output information.
*/
typedef struct _IPLC_IPE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IPE_OUT_INFO;

/**
     IME data structure for IME_D2DWait

     IME D2D output information.
*/
typedef struct _IPLC_IME_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IME_OUT_INFO;


/**
     ISE data structure for ISE_D2DWait

     ISE D2D output information.
*/
typedef struct _IPLC_ISE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_ISE_OUT_INFO;

/**
     DRE data structure for DRE_D2DWait

     DRE D2D output information.
*/
typedef struct _IPLC_DRE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_DRE_OUT_INFO;

/**
     RHE data structure for RHE_D2DWait

     RHE D2D output information.
*/
typedef struct _IPLC_RHE_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_RHE_OUT_INFO;

/**
     IPLC_IFE2 data structure for IFE2_D2DWait

     IFE2 D2D output information.
*/
typedef struct _IPLC_IFE2_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
	IFE2_GRAY_AVG GrayAvg;          ///< information for IME LCA
} IPLC_IFE2_OUT_INFO;

/**
     IPLC_IFE2IME data structure for IFE2IME_D2DWait

     IFE2IME D2D output information.
*/
typedef struct _IPLC_IFE2IME_OUT_INFO {
	DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IFE2IME_OUT_INFO;

typedef union _D2DResult {
	IPLC_SIE_OUT_INFO SIE;               ///< for SIE d2d
	IPLC_IFE_OUT_INFO IFE;               ///< for IFE d2d
	IPLC_DCE_OUT_INFO DCE;               ///< for DCE d2d
	IPLC_IPE_OUT_INFO IPE;               ///< for IPE d2d
	IPLC_IME_OUT_INFO IME;               ///< for IME d2d
	IPLC_IFE2_OUT_INFO IFE2;             ///< for IFE2 d2d
	IPLC_ISE_OUT_INFO ISE;               ///< for ISE d2d
	IPLC_IFE2IME_OUT_INFO IFE2IME;       ///< for IFE2IME d2d
	IPLC_DRE_OUT_INFO DRE;               ///< for DRE d2d
	IPLC_RHE_OUT_INFO RHE;               ///< for RHE d2d
} D2DResult;

/**
     Supported D2D operation
*/
typedef enum _IPLC_D2D {
	IPLC_IPE = 0,        ///< IPE D2D
	IPLC_DCE,        ///< DCE D2D
	IPLC_IFE,        ///< IFE D2D
	IPLC_IFE2,       ///< IFE2 D2D
	IPLC_IME,        ///< IME D2D
	IPLC_RHEIME,     ///< IFE IN, IME OUT(all engine direct path)
	IPLC_DRE,        ///< DRE D2D
	IPLC_SIE,        ///< SIE D2D
	IPLC_ISE,        ///< ISE D2D
	IPLC_RHE,        ///< RHE D2D
	IPLC_MAX,
	ENUM_DUMMY4WORD(IPLC_D2D)
} IPLC_D2D;

/**
     IPLC_DRE data structure

     DRE D2D configuration. (only reference input[0] image size & format)
*/
typedef struct _IPLC_DRE {
	IPL_HAL_PARASET ParIdx; ///< set configuration into indicated IPH inner table
	UINT32 eventID;         ///< map to DRE_D2D_CB eventID if DRE_D2D_CB exist

	DRE_IN_FMT fmt;         ///< input format
	BOOL HDRMode;           ///< Hdr mode
	DRE_IMG_INFO In[4];     ///< Y1, UV1, Y2, UV2
	///< input image information (HDRMode = 0 only 1 input image), (HDRMode = 1, 2 input image)
	DRE_IMG_INFO Out[2];       ///< output always 422 format
	UINT32 Workbuf_addr;     ///< working buffer adderss & size
	UINT32 Workbuf_size;     ///< working buffer adderss & size
	fpEngInit fp_Init;      ///< this CB will replace eventID's CB if existed.
} IPLC_DRE_INFO;

/**
     IPLC_SIE data structure

     SIE D2D configuration.
*/
typedef struct _IPLC_SIE {
	IPL_HAL_PARASET ParIdx; ///< set configuration into indicated IPH inner table
	UINT32 eventID;         ///< map to SIE_D2D_CB eventID if SIE_D2D_CB exist
	UINT32 clk;             ///< engine clk freq.

	IPL_RAW_IMG_INFO  In;   ///< dram input info.
	IPL_RAW_IMG_INFO Out;   ///< dram output info.

	fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
} IPLC_SIE_INFO;
/**
     IPLC_IPE data structure

     IPE D2D configuration.
*/
typedef struct _IPLC_IPE {
	IPL_HAL_PARASET ParIdx;     ///< -
	UINT32 eventID;             ///< map to IPE_D2D_CB eventID if IPE_D2D_CB exist
	IPH_IPECLK  clk;            ///< engine clk freq.

	//in
	IPE_SIZE InSizeY;         ///< in Y information
	UINT32 InLofsC;           ///< in C lineoffset
	IPEFmt InFmt;             ///< in format
	UINT32 InAddr[2];         ///< in addr (Image: idx0 = y, idx1 = uv, parameter: idx0 = RGB gamma, idx1 = 3DCC, idx2 = Y gamma)

	//out
	IPE_SIZE OutSizeY;         ///< out Y information
	UINT32 OutLofsC;           ///< out C lineoffset
	IPEFmt OutFmt;             ///< out format
	UINT32 OutAddr[2];         ///< out addr (Image: idx0 = y, idx1 = uv)

	fpEngInit   fp_Init;        ///< this CB will replace eventID's CB if existed.
} IPLC_IPE_INFO;

/**
     IPLC_DCE data structure

     DCE D2D configuration.
*/
typedef struct _IPLC_DCE {
	IPL_HAL_PARASET ParIdx; ///< -
	UINT32 eventID;         ///< map to DCE_D2D_CB eventID
	IPH_DCECLK  clk;        ///< engine clk freq.

	USIZE       Size;       ///< input/out size
	DCE_D2D_SETTING D2d_setting;    ///< D2D parameters

	MEM_RANGE  *pDceStpe;   ///< dce stripe table, NULL if no pre-calculation.
	fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
} IPLC_DCE_INFO;

/**
     IPLC_IFE data structure

     IFE D2D configuration.
*/
typedef struct _IPLC_IFE {
	IPL_HAL_PARASET ParIdx; ///< -
	UINT32 eventID;         ///< map to IFE_D2D_CB eventID if IFE_D2D_CB exist
	IPH_IFECLK  clk;        ///< engine clk freq.

	IFE_IN_DMA IFE_In;      ///< ife input information

	IFE_DMA_OUT IFE_Out;    ///< output info.

	IFEFMT IFEFmt;          ///< input data format
	fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
} IPLC_IFE_INFO;

/**
     IPLC_IFE2 data structure

     IFE2 D2D configuration.
*/
typedef struct _IPLC_IFE2_INFO {
	IPL_HAL_PARASET ParIdx; ///< -
	UINT32 eventID;         ///< map to IFE2_D2D_CB eventID if IFE2_D2D_CB exist
	IPH_IFE2CLK clk;        ///< engine clk freq.
	IFE2_ADDR InAddr;       ///< input data address
	IFE2_SIZE InSize;       ///< input size
	IFE2_LOFS InLofs;       ///< input line offset
	IFE2_FORMAT InFmt;         ///< input data format

	IFE2_ADDR OutAddr;       ///< output data address
	IFE2_LOFS OutLofs;       ///< output line offset
	IFE2_FORMAT OutFmt;      ///< output data format
	fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
} IPLC_IFE2_INFO;

/**
     IPLC_IME data structure

     IME D2D configuration.
*/
typedef struct _IPLC_IME {
	IPL_HAL_PARASET ParIdx; ///< -
	UINT32 eventID;         ///< map to IME_D2D_CB eventID if IME_D2D_CB exist
	IPH_IMECLK  clk;        ///< engine clk freq.

	struct {
		IMEADDR PPB0;       ///< -
		IMESIZE Size;       ///< -
		IMEFMT Fmt;         ///< -

		struct {
			IFE2_GRAY_AVG refval;
			IPL_YCC_CH_INFO Size;         ///< -
			UINT32 Addr;                  ///< -
			UINT32 FromIFE2;              ///< reference image source form 0:dram or 1:IFE2
		} Adaption;                       ///< chroma adaption dram input info
		UINT32 PmPxlDmaAddr;              ///< pixelation input image buffer address
	} IME_In;                             ///< input info.

	IME_OUT IME_Out;                      ///< ime output info

	struct {
		IME_PRI_MASK_INFO PmInfo;         ///< privacy mask information

	} IME_SubFunc;

	fpEngInit   fp_Init;        ///< this CB will replace eventID's CB if existed.
} IPLC_IME_INFO;

/**
     IPLC_IFEIME data structure(ife_ipe(dce)_ime)

     ife to ime in direct_path. (d2d frame mode)
*/
typedef struct _IPLC_RHEIME {
	IPL_HAL_PARASET ParIdx; ///< -
	UINT32 eventID;         ///< map to IFEIME_D2D_xxxCB eventID if IFEIME_D2D_xxxCB exist
	IPH_IFECLK  IFEclk;     ///< engine clk freq.
	IPH_IPECLK  IPEclk;     ///< engine clk freq.
	IPH_DCECLK  DCEclk;     ///< engine clk freq.
	IPH_IMECLK  IMEclk;     ///< engine clk freq.
	IPH_RHECLK  RHEclk;     ///< engine clk freq.
	struct {
		IPL_RAW_IMG  In;         ///< raw iamge input infor.
		BAYER_TYPE   BayerFmt;     ///< raw in bayer format(RGGB/RGBIr)
	} Raw_In;

	struct {
		IPL_RAW_IMG  In;         ///< raw iamge input infor.
		BAYER_TYPE   BayerFmt;     ///< raw in bayer format(RGGB/RGBIr)
	} Raw_ch3_In;


	struct {
		UINT32            proc_frame_cnt; ///< process frame number , from sensor
		UINT32            subout_addr;
		UINT32            defog_ref_addr;
		RHE_HAL_FUNC_MODE func_mode;
		RHE_Dma_In        dma_in;
		RHE_Dma_In        sub_dma_in;
		struct {
			UINT32 yout_x_winnum;         ///< yout x window number
			UINT32 yout_y_winnum;         ///< yout y window number
			UINT32 win_size_x;            ///< yout window size_x, calculated by sie driver
			UINT32 win_size_y;            ///< yout window size_y, calculated by sie driver
		} y_out_info;
		UINT32 enc_addr;                  ///< encode address from CH5
	} Rhe_Info;

	struct {
		IFE2_GRAY_AVG refval;
		IPL_YCC_CH_INFO Size;   ///< -
		UINT32 Addr;            ///< -
	} Adaption;                 ///< chroma adaption input info


	struct {
		BOOL FuncEn;            ///ime subout control
	} IME_Sub;                  ///<

	IME_OUT IME_Out;        ///< ime output info

	struct {
		UINT32 PmPxlDmaAddr;
		IME_PRI_MASK_INFO PmInfo;         ///< privacy mask information

	} IME_SubFunc;

	FLIP_TYPE   flip_info;      ///<rhe flip information
	MEM_RANGE  *pDceStpe;   ///< dce stripe table, NULL if no pre-calculation.

	fpEngInit   fp_Init[IPL_HAL_MAX];    ///< this CB will replace eventID's CB if existed. effeicient index: IPL_HAL_IFE, IPL_HAL_IPE, IPL_HAL_DCE, IPL_HAL_IME
} IPLC_RHEIME_INFO;

/**
     IPLC_ISE data structure

     ISE D2D configuration.
*/
typedef struct _IPLC_ISE {
	IPL_HAL_PARASET ParIdx;                 ///< set configuration into indicated IPH inner table
	UINT32 eventID;                         ///< map to ISE_D2D_CB eventID if ISE_D2D_CB exist
	IPH_ISECLK  clk;                        ///< engine clk freq.
	ISE_IN In;                              ///< dram input info.
	ISE_OUT Out;                            ///< ouput info.
	ISE_PACK_FMT InOutPackFormat;           ///< input and output packing format
	ISE_SCALE_METHOD_SEL ScaleMethod;       ///< scaling method selection
	fpEngInit   fp_Init;                    ///< this CB will replace eventID's CB if existed.
} IPLC_ISE_INFO;


/**
     IPLC_RHE data structure

     RHE D2D configuration.
*/
typedef struct _IPLC_RHE {
	IPL_HAL_PARASET ParIdx;             ///< -
	UINT32 eventID;                     ///< map to RHE_D2D_CB eventID if RHE_D2D_CB exist
	IPH_RHECLK  clk;                    ///< engine clk freq.

	struct {
		IPL_RAW_IMG_INFO Img_info;      ///< RHE input raw information
		BAYER_TYPE   BayerFmt;          ///< RHE input raw bayer format
	} Raw_in;

	RHE_SIZE RawCropSize;               ///< RHE input raw crop size
	RHE_CROP RawCropPos;                ///< RHE crop infomation
	RHE_Dma_In DmaInputInfo;            ///< RHE dma input information(1:image1 2:image2 3:LA
	RHE_Dma_Out DmaOutInfo;             ///<
	RHE_HAL_FUNC_MODE func_mode;
	RHE_RAW_DECODE_INFO decode_info;
	UINT32   in_path_sel;               ///< desired setting channel, ex: (RHE_HAL_INPATH_1 | RHE_HAL_INPATH_2)

	fpEngInit   fp_Init;                ///< this CB will replace eventID's CB if existed.
} IPLC_RHE_INFO;

/**
     IPLC_PREIME data structure

*/
typedef union _IPCL_D2D_STRUCT {
	IPLC_IPE_INFO IPE;
	IPLC_DCE_INFO DCE;          ///< for dce d2d
	IPLC_IFE_INFO IFE;          ///< for ife d2d
	IPLC_IFE2_INFO IFE2;        ///< for ife2 d2d
	IPLC_IME_INFO IME;          ///< for ime d2d
	IPLC_RHEIME_INFO RHEIME;    ///< for d2d frame mode
	IPLC_DRE_INFO DRE;          ///< for dre d2d
	IPLC_SIE_INFO SIE;          ///< for sie d2d
	IPLC_ISE_INFO ISE;          ///< for ise d2d
	IPLC_RHE_INFO RHE;          ///< for rhe d2d
} IPLC_D2D_STRUCT;

/**
     IPLC y_out data structure

*/
typedef struct _IPLC_D2D_YOUT_INFO {
	UINT32 yout_addr;	///< Yout dram buffer address
	UINT32 yout_lofs;	///< Yout dram buffer lineoffset
	UINT32 yout_sizeh;	///< Y horizontal output window number
	UINT32 yout_sizev;	///< Y vertical output window number
} IPLC_D2D_YOUT_INFO;

//@{
/**
     DCE d2d configuration

     a weak symbol for d2d opeartion, it will be called right before DCE d2d start, in order to get
     DCE configuration.
     @note
        if this symbol is undefined, DCE d2d will return error and drop start operation!(beccause of stripe hw constraint)

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    Ref_1x:     sensor output size
     @param[out]    Ref_1xCen   distortion center of sensor output
     @param[out]    subF:       dce configuration
     @param[out]    group:      IPH DCE update item

     @return Description of data returned.
         - @b return value is the repeat time of dce d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 DCE_D2D_CB(IPL_PROC_ID id, USIZE *Ref_1x, Coordinate *Ref_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     IPE d2d configuration

     a weak symbol for d2d operation. it will be called right before IPE d2d start, in order to get
     IPE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IPE configuration
     @param[out]    group:      IPH IPE update item

     @return Description of data returned.
         - @b return value is the repeat time of IPE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IPE_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     IFE d2d configuration

     a weak symbol for d2d operation. it will be called right before IFE d2d start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of IFE d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFE_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     IFE2 d2d configuration

     a weak symbol for d2d operation. it will be called right before IFE2 d2d start, in order to get
     IFE2 configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE2 configuration
     @param[out]    group:      IPH IFE2 update item

     @return Description of data returned.
         - @b return value is the repeat time of IFE2 d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFE2_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC *subF, IFE2_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     IME d2d configuration

     a weak symbol for d2d operation. it will be called right before IME d2d start, in order to get
     IME configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IME configuration
     @param[out]    group:      IPH IME update item

     @return Description of data returned.
         - @b return value is the repeat time of IME d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IME_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));
/**
     RHE configuration of rhe2ime frame mode

     a weak symbol for rhe2ime frame mode operation. it will be called right before frame mode start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RHE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RHEIME_D2D_RHECB(IPL_PROC_ID id, RHE_SUBFUNC *subF, RHE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));
/**
     IFE configuration of rhe2ime frame mode

     a weak symbol for rhe2ime frame mode operation. it will be called right before frame mode start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RHEIME_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     DCE configuration of rhe2ime frame mode

     rhe2ime frame mode operation. it will be called right before frame mode start, in order to get
     DCE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    Ref_1x:     sensor output size
     @param[out]    Ref_1xCen   distortion center of sensor output
     @param[out]    subF:       dce configuration
     @param[out]    group:      IPH DCE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RHEIME_D2D_DCECB(IPL_PROC_ID id, USIZE *Ref_1x, Coordinate *Ref_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     IPE configuration of rhe2ime frame mode

     a weak symbol for rhe2ime frame mode operation. it will be called right before frame mode start, in order to get
     IPE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IPE configuration
     @param[out]    group:      IPH IPE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RHEIME_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     IME configuration of rhee2ime frame mode

     a weak symbol for rhe2ime frame mode operation. it will be called right before frame mode start, in order to get
     IME configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fifegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IME configuration
     @param[out]    group:      IPH IME update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RHEIME_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     DRE d2d configuration

     a weak symbol for d2d operation. it will be called right before DRE d2d start, in order to get
     RDE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RDE configuration
     @param[out]    group:      IPH DRE update item

     @return Description of data returned.
         - @b return value is the repeat time of DRE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 DRE_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));

/**
     SIE d2d configuration

     a weak symbol for d2d operation. it will be called right before SIE d2d start, in order to get
     RDE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RDE configuration
     @param[out]    group:      IPH SIE update item

     @return Description of data returned.
         - @b return value is the repeat time of SIE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 SIE_D2D_CB(IPL_PROC_ID id, SIE_SUBFUNC *subF, SIE_UPDATE *group, UINT32 eventID, fpRegDump *fpRegD) __attribute__((weak));
//@}

/*
    IPL_D2D_Stripe callback function
*/

extern void _STRIPE_CB(IPL_PROC_ID id, USIZE *Ref_1x, Coordinate *Ref_1xCen, USIZE *SieInSize, USIZE *SieOutSize, UINT32 *SenHRatio, UINT32 *SenVRatio,
					   DCE_SUBFUNC *subF, IPL_HAL_GROUP *Update, UINT32 eventID) __attribute__((weak));
/**
     WDR_HDR d2d callback function

     a weak symbol for d2d operation. it will be called right before RHE2IME d2d start, in order to get
     WDR_HDR Parameters.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RHE configuration
     @param[out]    group:      IPH SIE update item

     @return Description of data returned.
         - @b return value is the repeat time of SIE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
UINT32 RHEIME_D2D_EXTFUNC_RHE_CB(IPL_PROC_ID id, RHE_SUBFUNC *psubF, RHE_UPDATE *Update, UINT32 eventID, void *extinfo)__attribute__((weak));;
/**
     start d2d operation.

     start d2d operation without wait operation finish.
     if previous d2d operation set not ready yet(under the same engine condition), current operation
     will wait until preview operation set done.
     @note
\n   this operation will trig multi-task protection("IPL_D2D_Start" and "IPL_D2D_End" are symmetrical.)

     @param[in] info         egnine configuration
     @param[in] d2dMode      supported d2d mode
     @return Description of data returned.
         - @b E_OK:    engine start.
         - @b E_SYS:   d2dMode not support.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPL_D2D_Start(IPL_PROC_ID id, IPLC_D2D_STRUCT *info, IPLC_D2D d2dMode);

/**
     wait d2d operation finish

     if d2d repeat more than one time, it will wait until last operation finish, and clear OS flag.

     @note:
\n     |--------|                         |--------|                            |--------|
\n     |  data  |   input addr            |        |   input addr               |  data  |   input addr
\n     |--------|                 1x      |--------|                    2x      |--------|
\n                                =>                                    =>
\n
\n     |--------|                         |--------|                            |--------|
\n     |        |   output addr           |  data  |   output addr              |        |   output addr
\n     |--------|                         |--------|                            |--------|
\n

     @param[in]     d2dMode         supported d2d mode
     @param[out]    DataInfo    return ready Data in InputAddr or OutputAddr or other data
     @return Description of data returned.
         - @b E_OK:     engine data output ready
         - @b E_NOSPT:  opeartion error.

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPL_D2D_WaitFinish(IPL_PROC_ID id, IPLC_D2D d2dMode, D2DResult *DataInfo);

/**
     close d2d operation.

     close engine directly.

     @note
\n   this operation will trig multi-task protection("IPL_D2D_Start" and "IPL_D2D_End" are symmetrical.)
\n   if end operation being calling during engine still running, it will cause engine crash.

     @param[in] d2dMode      supported d2d mode
     @return Description of data returned.
         - @b E_OK:     engine closed
         - @b E_NOSPT:  opeartion error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPL_D2D_End(IPL_PROC_ID id, IPLC_D2D d2dMode);


/**
     Control IPL start to run
     @note
     IPL can run in 3 modes,plz reference IPLCTRL_FLOW, this api will open and then start IPL engines, and configure
     inner status into "running_status"
\n   IPL must be in all_stop_status when call it.
\n   IPL engines are including : SIE,PRE,IFE,DCE,IPE,IFE2,IME.

     @param[in] group       set IPL group index.
     @param[in] id          set IPL id.
     @param[in] order       set order in group (0(min) ~ 8(max)).

     @param[in] flow       start IPL by indicatied flow.
     @param[in] info       info. for IPL,each flow have corresponding data structure
                           ex:
                           fulfill "pInfo->ICF_FA" if flow is "FLOW_A"
     @param[in] Obj        isr notification CB,call back at each engine isr occurred
     @param[in] fpImg      ime frame end notification CB.

     @return Description of data returned.
         - @b E_OK:        IPL start.
         - @b E_SYS:       ICF task not opened or some of IPL engines not close yet, or IPL not in stop_status.
     Example: (Optional)
     @code
     {
        IPC_INFO info;
        info.ICF_FA
        ...

        IPL_CTRL_OpenTsk();
        ...
        IPL_Ctrl_Cfg(ICF_FLOW_A,&info,NULL,NULL);
        ...
     }
     @endcode
*/
extern ER IPL_Ctrl_Cfg(UINT32 Group, IPL_PROC_ID Id, UINT32 Order, IPLCTRL_FLOW flow, IPC_INFO *pInfo, IPL_ISR_OBJ_INFO *pObj, fpImgRdy fpImg);
/**
     Control IPL mode switch
     @note
     pause IPL and close engine from running_status. and uninstall isr notification CB.
\n   detail state mechine plz reference enum:IPC_STOP

     @param[in] IPL_PROC_BIT_ID
     @param[in] IPL0~8 cmd

     @return Description of data returned.
         - @b E_OK:    IPL stop.
         - @b E_SYS:   IPL already stopped,or state mechine error.
     Example: (Optional)
     @code
     {
        IPL_Ctrl_Cfg(ICF_FLOW_A,&info,NULL,NULL);
        ...
        IPL_Ctrl_Run(bit_id, cmd);
        ...
        IPL_CTRL_CloseTsk();
     }
     @endcode
*/
ER IPL_Ctrl_Run(IPL_PROC_BIT_ID bit_id, IPC_CTRL_CMD *cmd);

/**
     Get IPL mode switch status
     @note
     pause IPL and close engine from running_status. and uninstall isr notification CB.
\n   detail state mechine plz reference enum:IPC_STOP

     @param[in] IPL_PROC_ID
     @param[out] ER
     @param[out] ER
*/

void IPL_Ctrl_Run_Get_Err_Code(ER *sts1, ER *sts2);
/**
     get IPL_Ctrl information
     @note
     get IPL_Ctrl information according input enum.
\n   (no association with IPL in run/stop status.)
\n   if flow == ICF_FLOWA, return information in IPC_QINFO->QUE_FA
\n   if flow == QUE_CSUB_Rdy, return information in IPC_QINFO->Rdy_Csub,
     if return address = 0, means that sub-image do not output to dram yet!

     @param[in] id          input which IPL_ID need to be queried
     @param[in] event       input which event need to be queried

     @return Description of data returned.
         - @b IPC_QINFO:

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern IPC_QINFO IPL_Ctrl_Query(IPL_PROC_ID id, IPLCTRL_QUERY event);

/**
     Change IPL setting when IPL in running_status
     @note
     change IPL engine during IPL in running status, all Chg will load into engine registor.
     this api will wait until all configuration is latched,excluding
     "IPC_Chg_ImeP1Out","IPC_Chg_ImeP2Out","IPC_Chg_ImeP3Out","IPC_Chg_Crop","IPC_SIE_".
     detail plz reference enum:IPC_RUNTIMECHG !

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!
     Example: (Optional)
     @code
     {
        IPL_CTRL_OpenTsk();
        ...
        IPL_Ctrl_Cfg(ICF_FLOW_A,&info,NULL,NULL);
        ...

        IPL_Ctrl_Runtime_Chg(IPC_Chg_ImeP2Out,(void*)&IPC_IMEOut_Info[0]);
        IPL_Ctrl_Runtime_Chg(IPC_Chg_Crop,(void*)&IPC_IMEOut_Info[0]);
        IPL_Ctrl_Runtime_Chg(IPC_Chg_Load,NULL);
        ...
        IPL_Ctrl_Runtime_Chg(IPC_SIE_IQparam,(void*)&IPC_IQparam);
        ...
        IPL_Ctrl_Run(IPC_STOP_ALL);
        ...
        IPL_CTRL_CloseTsk();
     }
     @endcode
*/
extern ER IPL_Ctrl_Runtime_Chg(IPL_PROC_BIT_ID bit_id, IPC_RUNTIMECHG chg, void *ptr);

/**
     Change IPL setting when IPL in running_status

	@note
	same function as IPL_Ctrl_Runtime_Chg, but disable wait flag ctrl for isr using
*/
extern ER IPL_Ctrl_Runtime_Chg_ISR(IPL_PROC_BIT_ID bit_id, IPC_RUNTIMECHG chg, void *ptr);

/**
     Change IPL IQ setting when IPL in running_status

     @note
     all enum:IPC_RUNTIMECHG_IQ have no timing control.

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG_IQ

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!

     @endcode
*/
extern ER IPL_Ctrl_Runtime_Chg_IQ(IPL_PROC_ID Id, IPC_RUNTIMECHG_IQ Chg, void *ptr);

/**
     Change IPL IQ setting when IPL in running_status

     @note
     all enum:IPC_RUNTIMECHG_IQ have no timing control.

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG_IQ

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!

     @endcode
*/
extern ER IPL_Ctrl_Runtime_Chg_IQ_ISR(IPL_PROC_ID Id, IPC_RUNTIMECHG_IQ Chg, void *ptr);

/**
     set timeout interval


     @param[in] ms       timeout interval ms

     @return Description of data returned.
         - @b E_OK:
         - @b E_SYS:

     @endcode
*/
extern ER IPL_Ctrl_Set_Timeout_MS(UINT32 ms);

/**
     get timeout interval

     @return Description of data returned.
         - @b timeout interval ms

     @endcode
*/
extern UINT32 IPL_Ctrl_Get_Timeout_MS(void);

/**
     wait ime frame end

     wait until ime frame end, and clear ime frame end.
     @note

     @param[in] id:       pipeline index
     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear frame end first , and then wait and clear ime frame end.
\n   if bClrFlg == FALSE, wait and clear ime frame end directly.
     @param[in] TimeOutMs: ms

     @return Description of data returned.
         - @b E_OK:    frame end ready.
         - @b E_SYS:   engine is not running.
         - @b E_TMOUT: timeout.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPL_Wait_Ime_Fmd(IPL_PROC_BIT_ID bit_id, BOOL bClrFlg, UINT32 TimeOutMs);
/**
     wait sie field end

     wait until sie field end, and clear sie field end.
     @note

     @param[in] id:       pipeline index
     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear field end first , and then wait and clear sie field end.
\n   if bClrFlg == FALSE, wait and clear ime frame end directly.
     @param[in] TimeOutMs: ms

     @return Description of data returned.
         - @b E_OK:    field end ready.
         - @b E_SYS:   engine is not running.
         - @b E_TMOUT: timeout.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPL_Wait_Vd(IPL_PROC_BIT_ID bit_id, BOOL bClrFlg, UINT32 TimeOutMs);

/**
    enable IPL_Ctrl module message

    print warning msg under pipeline in running status when enabled.
    @note :none

    @return :none
    Example: none
    @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_MSG(BOOL bEnable);

/**
    trigger IPL with input in dram

    @param[in] id:          IPL index
    @param[in] header_addr: Raw header buffer address
    @param[in] data:    	data for trigger(IPL_VIRTUAL_IN_RAW_INFO/IPL_VIRTUAL_IN_YCC_INFO)
*/
extern ER IPL_Ctrl_Virtual_Trigger(IPL_PROC_ID id, UINT32 header_addr, void* data);

/**
	read paramter at hal layer

	@param[in] id:          IPL index
	@param[in] eng:         Engine
	@param[in] hal_idx: 	Hal layer parameter index
	@param[in] read_item: 	Hal layer read item
	@param[in] data: 		pointer of return data, type is defined by each engine read item
*/
extern ER IPL_Ctrl_Read(IPL_PROC_ID id, IPL_HAL_ENG eng, IPL_HAL_PARASET hal_idx, IPL_HAL_READ read_item, void* data);
//@}
#endif
