/**
    IPL Ctrl module(streaming) data type

    Define all data type needed by IPL Ctrl module(streaming).

    @file       ipl_ctrl_flow_Int.h
    @ingroup    mIIPL_Ctrl
    @note       None

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _IPL_CTRL_FLOW_H_
#define _IPL_CTRL_FLOW_H_

#include "SysKer.h"
#include "ipl_utility.h"
#include "ipl_hal_op.h"
#include "ipl_header_infor_raw.h"

/** \addtogroup mIIPL_Ctrl */
//@{
/**
     IPL_CTRL Stop mode

     @note:inner status mechine(valid):
\n        run -> stop_all.

\n        run -> stop ycc ( -> ipc_pause_sie -> ipe_resume sie) -> ipc_stop_raw.
                                   |---<-----<----<----|

\n        run -> ipc_pause_sie -> ipc_resume_sie
                    |---<-----<----<----|

\n        run -> ipc_pause_ipl -> ipc_resume_ipl
                    |---<-----<----<----|
*/
typedef enum {
	IPC_IGNORE      = 0x00000000,   ///< ignore command
	IPC_START_RUN   = 0x00000001,   ///< RUN satus when each sie engine start run
	IPC_STOP_ALL    = 0x00000002,   ///< stop and close all engine
	IPC_STOP_YCC    = 0x00000004,   ///< stop and close all engine but sie, sie engine still in running
	IPC_PAUSE_SIE   = 0x00000008,   ///< stop sie (sie still open, not close)
	IPC_RESUME_SIE  = 0x00000010,   ///< start sie from status:IPC_PAUSE_SIE
	IPC_STOP_RAW    = 0x00000020,   ///< stop and close sie engine
	IPC_PAUSE_IPL   = 0x00000040,   ///< pause Image Pipeline, but all engine still in running.
	IPC_RESUME_IPL  = 0x00000080,   ///< resume Image Pipeline.
	ENUM_DUMMY4WORD(IPC_CTRL_CMD)
} IPC_CTRL_CMD;

/**
     IPL_Ctrl RunTime chang IPL configuration

     only valid when IPL_Ctrl in run_status ,
     excluding :"IPC_SIE",   "IPC_Chg_ImeP1Out", "IPC_Chg_ImeP2Out",
                "IPC_Chg_ImeP3Out", "IPC_Chg_Crop", "IPC_Chg_Load" !

     @note :
\n           "IPC_SIE" only valid when IPL stop at "IPC_PAUSE_SIE"
\n           "IPC_Chg_ImeP1Out","IPC_Chg_ImeP2Out","IPC_Chg_ImeP3Out","IPC_Chg_Crop"
             these four enum will be latched until chg == "IPC_Chg_Load".
\n              ex:
\n                  IPL_Ctrl_Runtime_Chg(IPC_Chg_ImeP2Out,ptr);
\n                  IPL_Ctrl_Runtime_Chg(IPC_Chg_DCE,ptr);               this cmd will fail.
\n                  IPL_Ctrl_Runtime_Chg(IPC_Chg_Load,NULL);

*/
typedef enum _IPC_RUNTIMECHG {
	IPC_UNKNOW          = 0x00000000,
	IPC_Dzoom           = 0x00000001,   ///<                                                ,data type: IPC_Dzoom_Info*, Array Size 8
	IPC_WaitDzoomIdle   = 0x00000002,   ///<                                                ,data type: none
	IPC_SIEx_           = 0x00000003,   ///< control sie switch.ex:prv2cap or cap2prv       ,data type :IPC_SIE_Info*, Array Size 8
	//IPC_SenChgMode      = 0x00000003,   ///< chg sensor mode when pipe line running       ,data type :IPC_SenChgMode_Info

	//IPC_Chg_ChromaA_Retinex = 0x00000004,   ///< chg chroma adaption/Retinex parameter,   ,data type :IPC_ChromaA_Retinex_Info

	IPC_Chg_IPL_INAddr  = 0x00000005,   ///< chg ipl input address                          ,data type :IPC_IPLInAddr
	//IPC_Chg_DCE         = 0x00000006,   ///< chg Distortion configuration                 ,data type :IPC_IQparam
	IPC_Chg_SubOut      = 0x00000007,   ///< chg SubImg size                                ,data type :IPE_SUB_OUT
	IPC_Flip            = 0x00000008,   ///< control H&V flip                               ,data type :IPC_Flip_Info*, Array Size 8

	IPC_Chg_ImeP1Out    = 0x00001000,   ///< chg path1 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info*, Array Size 8
	IPC_Chg_ImeP2Out    = 0x00002000,   ///< chg path2 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info*, Array Size 8
	IPC_Chg_ImeP3Out    = 0x00004000,   ///< chg path3 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info*, Array Size 8
	IPC_Chg_ImeP4Out    = 0x00008000,   ///< chg path4 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info*, Array Size 8
	IPC_Chg_ImeP5Out    = 0x00010000,   ///< chg path4 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info*, Array Size 8
	IPC_Chg_Crop        = 0x00020000,   ///< chg sie out and pre in size/lineofs            ,data type :IPC_FRONTEND*
	IPC_Chg_Load        = 0x00040000,   ///< load command 0x00001000~ 0x00008000 into reg.  ,data type :none
	IPC_Trig_IMEDMA		= 0x00080000,	///< trigger ime path dma output					,data type :IPL_IME_PATH_ID

    IPC_HDR_Mode        = 0x00200000,   ///< chg HDR setting in HDR mode                    ,data type :IPC_HDR_Info
	ENUM_DUMMY4WORD(IPC_RUNTIMECHG)
} IPC_RUNTIMECHG;

/**
     IPL_Ctrl RunTime chang IPL IQ configuration

*/
typedef enum _IPC_RUNTIMECHG_IQ {

	IPC_SIEx_IQparam    = 0x10000000,   ///< chg SIE iq parameter set                       ,data type :IPC_IQparam

	IPC_RHE_IQparam     = 0x20000000,   ///< chg RHE iq parameter set                       ,data type :IPC_IQparam
	IPC_IFE_IQparam     = 0x50000000,   ///< chg IFE iq parameter set                       ,data type :IPC_IQparam
	IPC_DCE_IQparam     = 0x60000000,   ///< chg DCE iq parameter set                       ,data type :IPC_IQparam
	IPC_IPE_IQparam     = 0x70000000,   ///< chg IPE iq parameter set                       ,data type :IPC_IQparam
	IPC_IME_IQparam     = 0x80000000,   ///< chg IME iq parameter set                       ,data type :IPC_IQparam
	IPC_IFE2_IQparam    = 0x90000000,   ///< chg IFE2 iq parameter set                      ,data type :IPC_IQparam

	IPC_IFE_IQLoad_ISR  = 0xa0000000,   ///< chg IFE iq parameter ISR load                  ,data type :IPC_IQparam
	IPC_IPE_IQLoad_ISR  = 0xb0000000,   ///< chg IPE iq parameter ISR load                  ,data type :IPC_IQparam
	IPC_RHE_IQLoad_ISR  = 0xc0000000,   ///< chg RHE iq parameter ISR load                  ,data type :IPC_IQparam
	ENUM_DUMMY4WORD(IPC_RUNTIMECHG_IQ)
} IPC_RUNTIMECHG_IQ;

/**
     IQ runTimeChg.

     definition for Engine Quality chg when IPL in "IPL_RUN" status.
*/
typedef struct {
	IPL_HAL_PARASET Idx;
	IPL_HAL_GROUP   *pGrp;
	ENG_SUBFUNC     EngSubF;
} IPC_IQparam;

/**
     PRE dma input address runTimeChg.

     definition for change pre dma input address when IPL in "IPL_RUN" status.
*/
typedef struct {
	IPL_HAL_PARASET Idx;
	UINT32 PPB0Addr;
	UINT32 PPB1Addr;
} IPC_IPLInAddr;

/**
     ime dma output configuration runTimeChg.

     definition for change ime dma output configuration when IPL in "IPL_RUN" status.
     @note:
\n   can't change output address and scaling mehtod here
\n   scale IPL output image.
\n   control IPL dma output pathEn
*/
typedef struct {
	IPL_HAL_PARASET Idx;
	OUTCH           Path;               //output address and ScalMethod are useless here
	UINT32          sample_rate;        //ipl sample rate (dst_fps:bit[31:16], src_fps:bit[15:0])
	UINT32              p1_encode_en;   //path1 encode enable, only for path1
	IME_HAL_SINFO_FMT   p1_sinfo_fmt;   //path1 encode format, only for path1
} IPC_IMEOut_Info;

/**
    IPL flip information
*/
typedef enum _FLIP_TYPE {
	FLIP_NONE = 0x00000000,         ///< no flip
	FLIP_H    = 0x00000001,         ///< H flip
	FLIP_V    = 0x00000002,         ///< V flip
	FLIP_H_V  = 0x00000003,         ///<H & V flip(Flip_H | Flip_V) don't modify
	ENUM_DUMMY4WORD(FLIP_TYPE)
} FLIP_TYPE;

/**
     flip control information.
*/
typedef struct {
	IPL_PROC_ID Id;
	FLIP_TYPE type;
} IPC_Flip_Info;


/**
     IPL raw_image in/out size runTimeChg.

     definition for change sie output size/pre inout size when IPL in "IPL_RUN" status.
     @note:
\n   support digital zoom application.
\n   support cropping application.
*/
typedef struct _IPC_Dzoom_Info {
	IPL_HAL_PARASET Idx;
	IPL_PROC_ID     Id;
	UINT32 sieCropHini;
	UINT32 sieCropVini;
	UINT32 sieCropH;
	UINT32 sieCropV;
	UINT32 sieOutH;
	UINT32 sieOutV;
	UINT32 sieOutLineOfs;
	UINT32 iplCropX;
	UINT32 iplCropY;
	UINT32 iplInH;
	UINT32 iplInV;
	UINT32 iplInLineOfs;
	UINT32 sie_yout_winnum_x;
	UINT32 sie_yout_winnum_y;

	BOOL   bChgCAParam;
	BOOL   bChgLAParam;
	BOOL   bChgEthParam;
	BOOL   bChgYOutParam;

	FLIP_TYPE flip;
} IPC_Dzoom_Info;

typedef IPC_Dzoom_Info IPC_FRONTEND;

/**
     SIE control.

     reset SIE engine.
*/
typedef struct {
	IPL_HAL_PARASET Idx;
	SIE_UPDATE  sieUpdate;
	SIE *psie;
} IPC_SIE_Info;

/**
    HDR setting.

    Change IPL setting between HDR and WDR effect.
*/
typedef struct {
    RHE_HAL_FUNC_MODE rhe_func_mode;
    UINT32 win_num_x;
    UINT32 win_num_y;
} IPC_HDR_Info;

#if 0
- structure of supported flow
#endif

/**
     IPL_Ctrl flow.

     IPL_Ctrl module support following control flows.
*/
typedef enum _IPLCTRL_FLOW {
	ICF_REV1            = 0,
	ICF_FLOW_C          = 1,    ///< common flow, all engines control by FW handshake
	ICF_FLOW_CCIR       = 2,    ///< ccir/ccir interlace flow
	ICF_FLOW_VIRTUAL    = 3,    ///< virtual ipl flow, only raw2yuv, not support Function:Defog,LCA,Privacy Mask,Square,Raw encode,PHDR(WDR)
	ICF_MAX             = 4,
	ENUM_DUMMY4WORD(IPLCTRL_FLOW)
} IPLCTRL_FLOW;


/**
    Enum for query IPL information
*/
typedef enum _IPLCTRL_QUERY {
	ICF_FLOWC           = ICF_FLOW_C,       ///< minimum buffer number needed by flow_C.
	ICF_FLOWCCIR        = ICF_FLOW_CCIR,    ///< minimum buffer number needed by flow_CCIR.
	ICF_FLOWVIRTUAL     = ICF_FLOW_VIRTUAL, ///< minimum buffer number needed by flow_VIRTUAL.
	/******* below must be bigger than ICF_MAX *********/
	ICF_ETHOUT_RDY      = 11,           ///< current ready edge threshold output
	ICF_IPLIO_RDY       = 12,           ///< current ready Pre input ch0 info.
	ICF_LAPARAM_RDY     = 13,           ///< special case for sync timing
	ICF_FRAME_CNT       = 14,
	ICF_GDC_NOR_FACTOR  = 15,           ///< special case for RSC
	ICF_FLIP_TYPE       = 16,           ///< get image pipe line flip type
	ICF_IME_STATS_INFO  = 17,           ///< get ime statistic information
	ICF_IPE_ETHOUT_RDY  = 18,           ///< get ipe eth out information
	ICF_IPL_SAMPLE_RATE = 19,           ///< get ipl sample rate, data type UIN32[5]
	ICF_SIE_PRV_INTE_EN	= 20,			///< get sie preview interrupt enable bit, data type SIE_INTE_EN
	ICF_SIE_DFT_INTE_EN	= 21,			///< get sie default interrupt enable bit, data type SIE_INTE_EN
	ICF_SIE_YOUT_RDY	= 22,			///< get sie ready yout information, data type SIE_YOUT_PARAM
	ICF_RHE_SUBOUT_RDY	= 23,			///< get rhe ready subout address(defog), data type UINT32
	ICF_FLOW_MODE		= 24,			///< get ipl ctrl flow mode
	ICF_FLOW_MODE_EXT	= 25,			///< get ipl ctrl flow mode extend
	ICF_SQUARE_MV_RDY	= 26,			///< get current ready mv out
	ICF_SQUARE_MO_RDY	= 27,			///< get current ready mo out
	ENUM_DUMMY4WORD(IPLCTRL_QUERY)
} IPLCTRL_QUERY;

/**
    Define for IPL Rate Control
    1 frame = 100 rate unit
*/
#define IPL_SAMPLE_RATE_ZERO    0x0000FFFF  ///< never trigger
#define IPL_SAMPLE_RATE_SINGLE  0x0001FFFF  ///< trigger once
#define IPL_SAMPLE_RATE_UNIT    100         ///< rate ctrl unit
#define IPL_SAMPLE_RATE_STREAM  0x00010001  ///< streaming mode, sample every frame
#define IPL_SAMPLE_RATE_LIMIT   240         ///< max fps 240 frame

/**
    Get/Set SIE Buffer Number
*/
UINT32 ICF_GetSIEPPBNum(IPL_PROC_ID id);
void ICF_SetSIEPPBNum(IPL_PROC_ID id, UINT32 num);

/**
	IPL Frontend image info
	for RAW postprocess used
*/
typedef struct _IPL_FRONTEND_IMG {
	BOOL b_yuv_img;

	// common
	UINT32 id;
	UINT32 addr[2];
	UINT32 width;
	UINT32 height;
	UINT32 line_offset;
	UINT64 vd_timestamp; ///< vd start

	// raw only
	IPL_RAW_BIT raw_bit;
	IPL_RAW_PIX raw_st_pix;

	// ccir only
	IPL_YUV_IMG_TYPE yuv_type;

	// RAW Header, this will effect how ipl process
	HEADER_RAW_DATA* p_raw_header;

	struct _IPL_FRONTEND_IMG* p_next_img;
} IPL_FRONTEND_IMG;


/**
	Define Error code for ICF_FC_TriggerIMEOut
*/
// IME_PATH1: 0x0001
// IME_PATH1: 0x0002
// IME_PATH1: 0x0004
// IME_PATH1: 0x0008
// ALL_IME_PATH1: 0x000F
#define ICF_TRIG_NG_LOG				0

#define ICF_TRIG_NG_PATH_DISABLE	(0x00010000)	///< Path not enable
#define ICF_TRIG_NG_MULTITRIG		(0x00020000)	///< Already trigger succes at same frame
#define ICF_TRIG_NG_NEWBUFFAIL		(0x00040000)	///< New buffer fail
#define ICF_TRIG_NG_TIMING_ISSUE	(0x00080000)	///< Trigger timing too close to ime frame end
#define ICF_TRIG_NG_SAMPLERATE		(0x00100000)	///< Sample rate not zero
#define ICF_TRIG_NG_PRVNOTEND		(0x00200000)	///< Previous trigger is not end

//@}
#endif
