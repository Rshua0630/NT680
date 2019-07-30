/**
    IPH module (SIE common function type define)

    @file       IPL_Hal_SIE_Com_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE_COM_INFO_H
#define _IPL_Hal_SIE_COM_INFO_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    sie engine supported clk
*/


typedef enum _IPH_SIE_PXCLKSRC {
	IPH_SIEPXCLK_OFF = 0,       ///< SIE pixel-clock disabled
	IPH_SIEPXCLK_PAD,           ///< SIE pixel-clock enabled, source as pixel-clock-pad
	IPH_SIEPXCLK_PAD_AHD,       ///< SIE pixel-clock enabled, source as pixel-clock-pad(AHD)
	IPH_SIEPXCLK_MCLK,          ///< SIE pixel-clock enabled, source as MCLK (SIE_PXCLK from SIE_MCLK, SIE2_PXCLK from SIE_MCLK2, SIE3_PXCLK from SIE_MCLK2, SIE4_PXCLK from SIE_MCLK2)
	IPH_SIEPXCLK_VX1_1X,        ///< SIE pixel-clock enabled, source as Vx1 1x clock (only for SIE2/3)
	IPH_SIEPXCLK_VX1_2X,        ///< SIE pixel-clock enabled, source as Vx1 2x clock (only for SIE2/3)
	ENUM_DUMMY4WORD(IPH_SIE_PXCLKSRC),
} IPH_SIE_PXCLKSRC;


typedef enum _IPH_SIE_CLKSRC_SEL {
	IPH_SIECLKSRC_CURR = 0, ///< SIE clock source as current setting(no setting, no changing)
	IPH_SIECLKSRC_480,      ///< SIE clock source as 480MHz
	IPH_SIECLKSRC_PLL5,     ///< SIE clock source as PLL5
	IPH_SIECLKSRC_PLL6,     ///< SIE clock source as PLL6,  might be 300MHz
	IPH_SIECLKSRC_PLL7,     ///< SIE clock source as PLL7,  might be 344.064MHz
	IPH_SIECLKSRC_PLL12,    ///< SIE clock source as PLL12, might be 300MHz
	IPH_SIECLKSRC_PLL13,    ///< SIE clock source as PLL13, might be 320MHz
	IPH_SIECLKSRC_192,      ///< SIE clock source as 192MHz
	ENUM_DUMMY4WORD(IPH_SIE_CLKSRC_SEL),
} IPH_SIE_CLKSRC_SEL;

typedef enum _ECS_BitRatio {
	ECS_D2F8    = 0,    ///<2 bits decimal, 8bits fraction
	ECS_D3F7    = 1,    ///<3 bits decimal, 7bits fraction
	ENUM_DUMMY4WORD(ECS_BitRatio)
} ECS_BitRatio;         ///< bit filed of ecs compesation gain

/**
    signal phase
*/
typedef enum _SIG_PHASE {
	_RISING     = 0,  ///< rising edge trig
	_FALLING    = 1,  ///< falling edge trig
} SIG_PHASE;


/**
    normalization factor for Optical black
*/
typedef enum _OB_NormaFact {
	MSB_8BIT    = 0,    ///< output MSB 8-bit
	DIVIDE_2    = 1,    ///< -
	DIVIDE_4    = 2,    ///< -
	DIVIDE_8    = 3,    ///< -
	ENUM_DUMMY4WORD(OB_NormaFact)
} OB_NormaFact;

/**
    scaler lp weighting on bayer domain
*/
typedef struct _BS_QUALITY {
	UINT32  LpfWeight_H;    ///< range: 0~100, if autoLpf = 1 set LpfWeight_H = 50 (default value)
	UINT32  LpfWeight_V;    ///< range: 0~100, if autoLpf = 1 set LpfWeight_V = 50 (default value)
	UINT32  Binning_H;      ///< -
	UINT32  Binning_V;      ///< -
	UINT32  AutoLpf;    ///< 0: LPF is absolute value; 1: LPF is adaptive value (suggestion: 1)
} BS_QUALITY;

/**
    SIE channel 0 output selection
*/
typedef enum _CH0_SRC {
	CH0_NONSCALED_IMG   = 0,    ///< SIE Dram output source = cropped image
	CH0_SCALED_IMG      = 1,    ///< SIE Dram output source = cropped and scaled image
	ENUM_DUMMY4WORD(CH0_SRC)
} CH0_SRC;

/**
    SIE channel 3 output selection
*/
typedef enum _CH3_SRC {
	CH3_VACC            = 0,    ///< va accumulation
	CH3_SCALED_IMG      = 1,    ///< scaled down image
	CH3_YOUT_ACC        = 2,    ///< shdr sub-raw out
	ENUM_DUMMY4WORD(CH3_SRC)
} CH3_SRC;

/**
    SIE channel 4 output selection
*/
typedef enum _CH4_SRC {
	CH4_VACC = 0,    ///< va accumulation
	CH4_ETH  = 1,    ///< eth
	ENUM_DUMMY4WORD(CH4_SRC)
} CH4_SRC;

typedef struct {
	UINT32  SizeH; ///< horizontal size
	UINT32  SizeV; ///< vertical size
} SIE_WIN_INFO;

typedef enum {
	NO_FLIP             =  0,
	H_FLIP,                    ///< enable selected function bits
	V_FLIP,                    ///< enable selected function bits
	HV_FLIP,                   ///< enable selected function bits
	ENUM_DUMMY4WORD(SIE_FLIP_INFO)
} SIE_FLIP_TYPE_INFO;
/**
    SIE data bitdepth
*/
typedef IPL_RAW_BIT SIE_PACKBUS;

/**
    Bayer CFA
*/
typedef IPL_RAW_PIX SIE_CFA;

/**
    Defect concealment weighting enum
*/
typedef enum _DEF_WEIGHT {
	_50F00_PERCENT  = 0,    ///< cross-channel weighting = 50% for defect concealment
	_25F00_PERCENT  = 1,    ///< cross-channel weighting = 25% for defect concealment
	_12F50_PERCENT  = 2,    ///< cross-channel weighting = 12.5% for defect concealment
	_6F25_PERCENT   = 3,    ///< cross-channel weighting = 6.25% for defect concealment
	ENUM_DUMMY4WORD(DEF_WEIGHT)
} DEF_WEIGHT;


typedef enum _BAYER_TYPE {
	BAYER_FORMAT = 0,   ///< Bayer format
	RGBIR_FORMAT,       ///< RGB-IR format
	RCCB_FORMAT,        ///< RCCB format
	ENUM_DUMMY4WORD(BAYER_TYPE)
} BAYER_TYPE;
/**
    sie dma input
*/
typedef struct _SIE_Dma_In {
	UINT32  Addr;           ///< -
	SIE_PACKBUS packBus;    ///< data bit depth
	UINT32  LineOffset;     ///< -
} SIE_Dma_In;


/**
    sie dma output
*/
typedef struct _SIE_Dma_Out {
	UINT32  PPB_Addr;       ///<
	UINT32  PPB_BufNum;     ///<
	UINT32  PPB_FrmOfs;     ///<
	UINT32  LineOffset;     ///< -
	BOOL    HFlip;          ///<
	BOOL    VFlip;          ///<
} SIE_Dma_Out;

/**
    sie interrupt enable
*/
typedef enum _SIE_INTE_EN {
	SIE__CLR                 = 0,
	SIE__INTE_FLDEND         = 0x00000001,   ///< enable interrupt: Field end
	SIE__INTE_BP1            = 0x00000002,   ///< enable interrupt: Break point 1
	SIE__INTE_BP2            = 0x00000004,   ///< enable interrupt: Break point 2
	SIE__INTE_BP3            = 0x00000008,   ///< enable interrupt: Break point 3
	SIE__INTE_ACTST          = 0x00000010,   ///< enable interrupt: Active window start
	SIE__INTE_CRPST          = 0x00000020,   ///< enable interrupt: Crop window start
	SIE__INTE_DRAMINOUT_ERR  = 0x00000040,   ///< enable interrupt: Dram input/output Error
	SIE__INTE_DRAM_OUT0_END  = 0x00000080,   ///< enable interrupt: Dram output channel 0 end
	SIE__INTE_DRAM_OUT1_END  = 0x00000100,   ///< enable interrupt: Dram output channel 1 end
	SIE__INTE_DRAM_OUT2_END  = 0x00000200,   ///< enable interrupt: Dram output channel 2 end
	SIE__INTE_DRAM_OUT3_END  = 0x00000400,   ///< enable interrupt: Dram output channel 3 end
	SIE__INTE_DRAM_OUT4_END  = 0x00000800,   ///< enable interrupt: Dram output channel 4 end
	SIE__INTE_DRAM_OUT5_END  = 0x00001000,   ///< enable interrupt: Dram output channel 5 end
	SIE__INTE_DPCF           = 0x00002000,   ///< enable interrupt: Defect Pixel/Column concealment failure.
	SIE__INTE_RAWENC_END     = 0x00004000,   ///< enable interrupt: raw encode end
	SIE__INTE_RAWENC_OUTOVFL = 0x00008000,   ///< enable interrupt: Raw encoder output buffer full
	SIE__INT_ACTEND          = 0x00010000,   ///< enable interrupt: Raw active end
	SIE__INT_CROPEND         = 0x00020000,   ///< enable interrupt: Raw crop end
	SIE__INTE_ALL            = 0xffffffff,   ///< enable interrupt: all
} SIE_INTE_EN;

/**
    SIE pattern gen type
*/
typedef enum _SIE_PAT_GEN_SEL {
	SIE_PAT_COLORBAR    = 1,    ///< Color bar
	SIE_PAT_RANDOM,             ///< Random
	SIE_PAT_FIXED,              ///< Fixed
	SIE_PAT_HINCREASE,          ///< 1D Increment
	SIE_PAT_HVINCREASE,         ///< 2D increment
	ENUM_DUMMY4WORD(SIE_PAT_GEN_SEL)
} SIE_PAT_GEN_SEL;

/**
    SIE Dram output burst length
*/
typedef enum _SIE_IN0_BURST_LENG {
	_BURST_IN0_48WORD   = 0,   ///< SIE1 Dram burst length  48 words
	_BURST_IN0_32WORD   = 1,   ///< SIE1 Dram burst length  32 words
	_BURST_IN0_24WORD   = 2,   ///< SIE1 Dram burst length  24 words
	_BURST_IN0_16WORD   = 3,   ///< SIE1 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_IN0_BURST_LENG)
} SIE_IN0_BURST_LENG;

typedef enum _SIE_IN1_BURST_LENG {
	_BURST_IN1_48WORD   = 0,   ///< SIE1/2 Dram burst length  48 words
	_BURST_IN1_32WORD   = 1,   ///< SIE1/2 Dram burst length  32 words
	_BURST_IN1_24WORD   = 2,   ///< SIE1/2 Dram burst length  24 words
	_BURST_IN1_16WORD   = 3,   ///< SIE1/2 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_IN1_BURST_LENG)
} SIE_IN1_BURST_LENG;

typedef enum _SIE_IN2_BURST_LENG {
	_BURST_IN2_48WORD   = 0,   ///< SIE1/2 Dram burst length  48 words
	_BURST_IN2_32WORD   = 1,   ///< SIE1/2 Dram burst length  32 words
	_BURST_IN2_24WORD   = 2,   ///< SIE1/2 Dram burst length  24 words
	_BURST_IN2_16WORD   = 3,   ///< SIE1/2 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_IN2_BURST_LENG)
} SIE_IN2_BURST_LENG;

typedef enum _SIE_OUT0_BURST_LENG {
	_BURST_OUT0_48WORD   = 0,   ///< SIE1/2 Dram burst length  48 words
	_BURST_OUT0_64WORD   = 1,   ///< SIE1/2 Dram burst length  64 words
	_BURST_OUT0_80WORD   = 2,   ///< SIE1/2 Dram burst length  80 words
	_BURST_OUT0_96WORD   = 3,   ///< SIE1/2 Dram burst length  96 words
	_BURST_OUT0_112WORD  = 4,   ///< SIE1/2 Dram burst length  112 words
	_BURST_OUT0_128WORD  = 5,   ///< SIE1/2 Dram burst length  128 words
	ENUM_DUMMY4WORD(SIE_OUT0_BURST_LENG)
} SIE_OUT0_BURST_LENG;

typedef enum _SIE_OUT1_BURST_LENG {
	_BURST_OUT1_48WORD   = 0,   ///< SIE1/2/3/4 Dram burst length  48 words
	_BURST_OUT1_32WORD   = 1,   ///< SIE1/2 Dram burst length  32 words
	_BURST_OUT1_24WORD   = 2,   ///< SIE1/2 Dram burst length  24 words
	_BURST_OUT1_16WORD   = 3,   ///< SIE1/2 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_OUT1_BURST_LENG)
} SIE_OUT1_BURST_LENG;

typedef enum _SIE_OUT2_BURST_LENG {
	_BURST_OUT2_48WORD   = 0,   ///< SIE1/2/3/4 Dram burst length  48 words
	_BURST_OUT2_32WORD   = 1,   ///< SIE1/2 Dram burst length  32 words
	_BURST_OUT2_24WORD   = 2,   ///< SIE1/2 Dram burst length  24 words
	_BURST_OUT2_16WORD   = 3,   ///< SIE1/2 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_OUT2_BURST_LENG)
} SIE_OUT2_BURST_LENG;

typedef enum _SIE_OUT3_BURST_LENG {
	_BURST_OUT3_48WORD   = 0,   ///< SIE1 Dram burst length  48 words
	_BURST_OUT3_32WORD   = 1,   ///< SIE1 Dram burst length  32 words
	_BURST_OUT3_24WORD   = 2,   ///< SIE1 Dram burst length  24 words
	_BURST_OUT3_16WORD   = 3,   ///< SIE1 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_OUT3_BURST_LENG)
} SIE_OUT3_BURST_LENG;

typedef enum _SIE_OUT4_BURST_LENG {
	_BURST_OUT4_48WORD   = 0,   ///< SIE1 Dram burst length  48 words
	_BURST_OUT4_32WORD   = 1,   ///< SIE1 Dram burst length  32 words
	_BURST_OUT4_24WORD   = 2,   ///< SIE1 Dram burst length  24 words
	_BURST_OUT4_16WORD   = 3,   ///< SIE1 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_OUT4_BURST_LENG)
} SIE_OUT4_BURST_LENG;

typedef enum _SIE_OUT5_BURST_LENG {
	_BURST_OUT5_48WORD   = 0,   ///< SIE1 Dram burst length  48 words
	_BURST_OUT5_32WORD   = 1,   ///< SIE1 Dram burst length  32 words
	_BURST_OUT5_24WORD   = 2,   ///< SIE1 Dram burst length  24 words
	_BURST_OUT5_16WORD   = 3,   ///< SIE1 Dram burst length  16 words
	ENUM_DUMMY4WORD(SIE_OUT5_BURST_LENG)
} SIE_OUT5_BURST_LENG;

/**
    sie signal receive mode
*/
typedef enum _SIE_ACT_MODE {
	SIE_IN_PARA_MSTR_SNR = 0,   ///< Parallel Master Sensor
	SIE_IN_PARA_SLAV_SNR,       ///< Parallel Slave Sensor
	SIE_IN_SELF_PATGEN,         ///< Self Pattern-Generator
	SIE_IN_VX1_IF0_SNR,         ///< Vx1 Sensor
	SIE_IN_CSI_1,               ///< Serial Sensor from CSI-1
	SIE_IN_CSI_2,               ///< Serial Sensor from CSI-2
	SIE_IN_CSI_3,               ///< Serial Sensor from CSI-3
	SIE_IN_CSI_4,               ///< Serial Sensor from CSI-4
	SIE_IN_CSI_5,               ///< Serial Sensor from CSI-5
	SIE_IN_CSI_6,               ///< Serial Sensor from CSI-6
	SIE_IN_CSI_7,               ///< Serial Sensor from CSI-7
	SIE_IN_CSI_8,               ///< Serial Sensor from CSI-8
	SIE_IN_DRAM,                ///< Dram Input (channel 0)
	SIE_IN_AHD,
	SIE_IN_2PPATGEN,            ///< ??????
	SIE_IN_SLVS_EC,             ///< Serial Sensor from SLVS-EC
	SIE_IN_VX1_IF1_SNR,         ///< Vx1 Sensor, interface 1
	ENUM_DUMMY4WORD(SIE_ACT_MODE)
} SIE_ACT_MODE;

/**
    SIE input data format and signal
*/
typedef struct _IPP_SIE_SIGNAL {
	struct {
		SIG_PHASE   VD;     ///< -
		SIG_PHASE   HD;     ///< -
		SIG_PHASE   Data;   ///< -
	} SensorPhase;          ///< signal phase

	struct {
		BOOL    HD_ini;     ///< HD inverse
		BOOL    VD_ini;     ///< VD inverse
	} Sie_HDVD;             ///< HD/VD signal
} IPP_SIE_SIGNAL;

/**
    SIE active and crop window
*/
typedef struct _SIE_WINDOW {
	struct {
		Coordinate  StartPix;///<crop start coordinate
		UINT32  SizeH;      ///< active window size
		UINT32  SizeV;      ///< active window size
		SIE_CFA CFA;        ///< CFA pattern of active window 1st pixel
	} Act;                          ///< active window

	struct {
		Coordinate  StartPix;///<crop start coordinate
		UINT32  SizeH;      ///< crop window size
		UINT32  SizeV;      ///< crop window size
		SIE_CFA CFA;    ///< CFA pattern of crop window 1st pixel
	} Crop;                         ///< crop window

} SIE_WINDOW;
/**
     SIE sub fucntion enable
*/
typedef enum _SieFuncEn {
	PatGenEn     = 0x00000001,   ///< enable pattern gen
	SHDRYOutEn   = 0x00000002,   ///< enable shdr Y out
	CGainEn      = 0x00000004,   ///< enable color gain
	RawEncEn     = 0x00000008,   ///< enable Raw encoder
	//RawEncGAMMAEn= 0x00000010,   ///< enable Raw encoder gamma function
	//SpiTrigEn    = 0x00000020,   ///< enable SPI trigger
	DviEn        = 0x00000040,   ///< enable DVI(CCIR)
	GridLineEn   = 0x00000080,   ///< enable grid line
	DpcEn        = 0x00000400,   ///< enable Defect pixel concealment
	HistoYEn    = 0x00002000,   ///< enable function statistics y historgram.
	OBAvgEn     = 0x00004000,   ///< enable OB average ( ob detection)
	OBByPassEn  = 0x00008000,   ///< enable function. OB by pass funtion.
	OBSubSelEn  = 0x00010000,   ///< enable OB subtraction selection
	OBFrameAvgEn = 0x00020000,  ///< enable OB-Frame average
	OBPlaneSubEn = 0x00040000,  ///< enable OB-Plane subtraction
	PFPCEn      = 0x00080000,   ///< enable PFPC
	PDAFEn      = 0x00100000,   ///< enable PDAF data
	ECSEn       = 0x00200000,   ///< enable Color shading compensation
	DGainEn     = 0x00400000,   ///< enable Digital Gain
	BSHEn       = 0x00800000,   ///< enable Horizontal bayer scaling
	BSVEn       = 0x01000000,   ///< enable Vertical bayer scaling
	CAEn        = 0x04000000,   ///< enable Color accumulation      (AWB)
	LAEn        = 0x08000000,   ///< enable Luminance accumulation  (AE)
	VAEn        = 0x10000000,   ///< enable Variation accumulation  (AF)
	ETHEn       = 0x20000000,   ///< enable Edge threshold
	DMAOutCh0En = 0x40000000,   ///< enable Dram output channel 0 (RAW)
	DMAOutCh3En = 0x80000000,   ///< enable Dram output channel 3 (VA)
} SieFuncEn;
/**
    DVI format
*/
typedef struct _SIE_DVI {
	enum {
		SIE_CCIR601 = 0,       ///< - CCIR 601
		SIE_CCIR656_EAV = 1,   ///< - CCIR 656 EAV
		SIE_CCIR656_ACT = 2,   ///< - CCIR 656 ACT
		ENUM_DUMMY4WORD(SIE_DVIFmt)
	} SIE_DVIFmt;        ///< -

	enum {
		SIE_SDMode = 0,    ///< SD mode (8 bits)
		SIE_HDMode = 1,    ///< HD mode (16bits)
		SIE_HDMode_Inv = 2,///< HD mode (16bits) with Byte Inverse
		ENUM_DUMMY4WORD(SIE_DVIMode)
	} SIE_DVIMode;          ///< -
	enum {
		DVIOUT_SWAPYUYV  = 0,   ///<
		DVIOUT_SWAPYVYU,        ///<
		DVIOUT_SWAPUYVY,        ///<
		DVIOUT_SWAPVYUY,        ///<
		ENUM_DUMMY4WORD(DVIOUT_SWAPSEL)
	} DVIOUT_SWAPSEL;
	BOOL    FldEn;          ///< start input from field with internal field signal=0
	BOOL    FldSelEn;       ///< inverse the polarity of internal field signal
	BOOL    OutSplit;       ///< output split
	BOOL    CCIR656VdSel;   ///< ccir 656 interlace/progressive select
	UINT8   DataPeriod;     ///< for multi frame
	UINT8   DataIdx;        ///< for multi frame
	BOOL    AutoAlign;      ///< for multi frame
} SIE_DVI;

/**
     data structure for OB
*/
typedef struct _OB_Para {
	UINT32 offset;  ///< -
	UINT32 Gain;    ///< -
} OB_Para;


/**
    Defect Pixel cocealment
*/
typedef struct _SIE_DPC {
	UINT32      DP_TblAddr;         ///< Ch0 input
	UINT32      TblSize;            ///< -
	DEF_WEIGHT  Weight;             ///< -
} SIE_DPC;


//sie dpc table size macro,excluding end string,0xFFFF. (unit:byte)
#define SIE_DPC_TBLSIZE(pixcnt,linecnt) ( ((pixcnt * 2) + (linecnt * 2) ) )

/**
    OB window average detection
*/
typedef struct _SIE_OBDET {
	Coordinate  StartPix; ///< window start pixel coordinate
	UINT32  SizeH;      ///< window horizontal size
	UINT32  SizeV;      ///< window vertical size
	UINT32  SubRatio_X; ///< window horizontal subsample
	UINT32  AvgActThres;///< threshold for ob avg activated counter
	UINT16  ObSubRatio;   ///< OB Detect Result Subtraction Ratio, replace to ob dtgain.
} SIE_OBDET;

/**
    OB window average detection
*/
typedef SIE_OBDET   SIE_OBAVG;

/**
    OB substration
*/
typedef struct _SIE_OBSUB {
	UINT16  Offset; ///< -
} SIE_OBSUB;

/**
    OB Frame calculation
    each window divide into 8 tabs(each tab length is "Interval")
*/
typedef struct _SIE_OBFAVG {
	struct {
		Coordinate  StartPix;   ///< -
		UINT32      Interval;   ///< -
	} TopWindow;                ///< -
	struct {
		Coordinate  StartPix;   ///< -
		UINT32      Interval;   ///< -
	} LeftWindow;               ///< -
	struct {
		Coordinate  StartPix;   ///< -
	} RightWindow;              ///< -
	struct {
		Coordinate  StartPix;   ///< -
	} BottomWindow;             ///< -
	OB_NormaFact OBF_NormFact;  ///< -

	UINT32 Thres;   ///< threshold for 4 windows
} SIE_OBFAVG;

/**
    OB Plane
    ob plane and scaling factor will be calcaulated automatically
*/
typedef struct _SIE_OBPSUB {
	//UINT32 TableAddr;  ///< tab num: 289 (17x17), Data type: UINT8
	//UINT32  Scale_H;///< Horizontal scaling factor
	//UINT32  Scale_V;///< Vertical scaling factor
	OB_NormaFact OBP_NormaFact; ///< -
} SIE_OBPSUB;
/**
    SIE Periodic fix pattern
*/
typedef struct _SIE_PFP {
	UINT32   Gain_TblAddr;  ///<tab num:32, Data type:UINT8
	UINT32   Ofs_TblAddr;   ///<tab num:32, Data type:UINT8
	enum {
		Mode_4x8    = 0,    ///< 4x8 period
		Mode_8x4    = 1,    ///< 8x4 period
		ENUM_DUMMY4WORD(PFPC_MODE)
	} PFPC_MODE;            ///< -
} SIE_PFP;

/**
    SIE color shading supported table size
*/
typedef enum _ECSMAP_ENUM {
	ecs65x65 = 0,   ///< 65x65
	ecs49x49 = 1,   ///< 49x49
	ecs33x33 = 2,   ///< 33x33
	ENUM_DUMMY4WORD(ECSMAP_ENUM)
} ECSMAP_ENUM;

#define ECS_MAP_SIZE(_enum,pSize) \
	{\
		if(_enum == ecs65x65)\
			*pSize = 65;\
		else if(_enum == ecs49x49)\
			*pSize = 49;\
		else if(_enum == ecs33x33)\
			*pSize = 33;\
	}

/**
     Sie Embedded Color shading
     @note (Optional)
*/
typedef struct _SIE_EmbeddedCS {
	ECS_BitRatio    BitSel;     ///< -
	BOOL    DthrEn;             ///< dithering enable
	BOOL    DthrRst;            ///< dithering reset
	enum {
		_BIT_0_1    = 0,    ///< add dithering at bit[0:1]
		_BIT_1_2    = 1,    ///< add dithering at bit[1:2]
		_BIT_2_3    = 2,    ///< -
		_BIT_3_4    = 3,    ///< -
		_BIT_4_5    = 4,    ///< -
		_BIT_5_6    = 5,    ///< -
		_BIT_6_7    = 6,    ///< -
		_BIT_7_8    = 7,    ///< -
		ENUM_DUMMY4WORD(ECSDthrLv)
	} ECSDthrLv;                ///< dithering level
} SIE_EmbeddedCS;

/**
    SIE VIG (for CA)
*/
typedef struct _SIE_CA_VIG {
	Coordinate  Center;         ///< center
	UINT32      T;              ///< disabled area
	UINT32      TabGain;        ///< gain factor
	UINT32      XDIV;           ///< scaling factor in x direction. unsigned 6.6
	UINT32      YDIV;           ///< scaling factor in y direction. unsigned 6.6
	UINT32      LUT_TblAddr;    ///< vignette LUT .(tab num:17,  Data type: UINT32)
} SIE_CA_VIG;
/**
    statistics information (path selection)
*/
typedef enum {
	STATS_VA_OUT_GP_1     = 0,   ///<
	STATS_VA_OUT_GP_1n2,         ///<
	ENUM_DUMMY4WORD(SIE_STATS_VA_OUT_SEL)
} SIE_STATS_VA_OUT_SEL;
typedef enum {
	STATS_HISTO_POST_GAMMA     = 0,   ///<
	STATS_HISTO_PRE_GAMMA,          ///<
	ENUM_DUMMY4WORD(SIE_STATS_HISTO_SRC_SEL)
} SIE_STATS_HISTO_SRC_SEL;
typedef enum {
	STATS_LA1_POST_CG  = 0,   ///< LA1 source from data right after  color-gain
	STATS_LA1_PRE_CG,         ///< LA1 source from data right before color-gain
	ENUM_DUMMY4WORD(SIE_STATS_LA1_SRC_SEL)
} SIE_STATS_LA1_SRC_SEL;
typedef struct {
	BOOL        bVig;           ///<
	BOOL        bLaCg;          ///<
	BOOL        bLaGama1;       ///<
	BOOL        bVaGama2;       ///<
	SIE_STATS_HISTO_SRC_SEL         bHistogramSel;    ///<
	BOOL        bCaThreshold;   ///<
	BOOL        bVaColorgain;   ///<
	SIE_STATS_LA1_SRC_SEL           bLa1SrcSel;      ///<
	UINT8       LaRGB2YWeight;  ///<
} SIE_STATS_PATH_INFO;
/**
    accumulation window
*/
typedef struct _ACC_WIN {
	UINT32  WinNum_X;   ///< horizontal window number
	UINT32  WinNum_Y;   ///< vertical window number
} ACC_WIN;
/**
    accumulation window
*/
typedef struct _VACC_WIN {
	UINT32  WinNum_X;  ///< horizontal window number
	UINT32  WinNum_Y;  ///< vertical window number
	UINT32  WinSizeX;    ///< color accumulation - horizontal window size
	UINT32  WinSizeY;    ///< color accumulation - vertical window size
	UINT32  WinSpaceX;    ///< color accumulation - horizontal window size
	UINT32  WinSpaceY;    ///< color accumulation - vertical window size
} VACC_WIN;
/**
    SIE color accumulation
*/
typedef struct _THRESHOL_SET {
	UINT16 Lower;
	UINT16 Upper;
} THRESHOL_SET;

typedef struct _SIE_CACC {
	struct {
		Coordinate  StartPix;///<crop start coordinate
		UINT32  SizeH;      ///< crop window size
		UINT32  SizeV;      ///< crop window size
	} CACrop;                         ///< crop window
	struct {
		UINT16      uiFactorX; ///< horizontal scaling factor
		UINT16      uiFactorY; ///< vertical scaling output size
	} CA_SCAL_FACTOR;
	ACC_WIN CA_WIN;     ///< accumulation window

	struct {
		UINT32 R_Weight;
		UINT32 G_Weight;
		UINT32 B_Weight;
	} CA_IR_SUB;

	struct {
		THRESHOL_SET Gth;
		THRESHOL_SET Rth;
		THRESHOL_SET Bth;
		THRESHOL_SET Pth;
	} CA_TH_INFO;
} SIE_CACC;
/**
    SIE Luminance accumulation
*/
typedef struct _SIE_LACC {
	struct {
		Coordinate  StartPix;///<crop start coordinate
		UINT32  SizeH;      ///< crop window size
		UINT32  SizeV;      ///< crop window size
	} LACrop;                         ///< crop window
	ACC_WIN LA_WIN;     ///< accumulation window
	struct {
		UINT16  RGain;    ///<
		UINT16  GGain;    ///<
		UINT16  BGain;    ///<
		UINT16  IRGain;   ///<
	} LA_CG_INFO;
	UINT16     *GmaTbl;  ///<
} SIE_LACC;

/**
    SIE Variation accumulation
*/
typedef enum {
	STATS_VA_FLTR_MIRROR  = 0,   ///<
	STATS_VA_FLTR_INVERSE,       ///<
	ENUM_DUMMY4WORD(SIE_STATS_VA_FLTR_SYMM_SEL)
} SIE_STATS_VA_FLTR_SYMM_SEL;

typedef enum {
	STATS_VA_FLTR_SIZE_1  = 0,   ///<
	STATS_VA_FLTR_SIZE_3,        ///<
	STATS_VA_FLTR_SIZE_5,        ///<
	STATS_VA_FLTR_SIZE_7,        ///<
	ENUM_DUMMY4WORD(SIE_STATS_VA_FLTR_SIZE_SEL)
} SIE_STATS_VA_FLTR_SIZE_SEL;

typedef struct {
	struct {
		UINT8                   A;      ///<
		INT8                    B;      ///<
		INT8                    C;      ///<
		INT8                    D;      ///<
	} FILT_ITEMS;
	SIE_STATS_VA_FLTR_SYMM_SEL FillterrSymmetry;   ///<
	SIE_STATS_VA_FLTR_SIZE_SEL FilterSize;         ///<
	UINT8                   uiDiv;      ///<
	THRESHOL_SET            thres;
} VA_FLTR_INFO;
typedef struct {
	VA_FLTR_INFO        FilterH;
	VA_FLTR_INFO        FilterV;
	BOOL                HExtend;
	BOOL                LineMax;
} VA_FLTR_GROUP_INFO;
typedef struct {
	Coordinate  StartPix;///<crop start coordinate
	UINT32  SizeH;      ///< crop window size
	UINT32  SizeV;      ///< crop window size
} VACROP;
typedef struct _SIE_VACC {
	VACC_WIN VA_WIN;     ///< accumulation window
	SIE_STATS_VA_OUT_SEL va_outsel;
	VACROP VACrop;       ///< crop window
	struct {
		UINT16  Gain0;    ///<
		UINT16  Gain1;    ///<
		UINT16  Gain2;    ///<
		UINT16  Gain3;    ///<
	} VA_CG_INFO;
	UINT16          *GmaTbl;   ///<
	VA_FLTR_GROUP_INFO G1;
	VA_FLTR_GROUP_INFO G2;
	struct {
		BOOL                bVA_INDEP_En;
		VACROP              INDEP_WIN;
		BOOL                LineMaxG1;
		BOOL                LineMaxG2;
	} VA_INDEP_WIN_INFO[5];
} SIE_VACC;
/**
    SIE ETH
*/
typedef struct _SIE_ETH {
	enum {
		ETH_2BIT = 0,
		ETH_8BIT = 1,
		ENUM_DUMMY4WORD(ETH_OUT_FMT),
	} ETH_OUT_FMT;

	enum {
		ETH_OUT_ALL_PIXEL = 0,
		ETH_OUT_SEL_FORMAT = 1,
	} ETH_OUT_SEL;

	enum {
		ETH_H_EVEN = 0,
		ETH_H_ODD = 1,
	} ETH_HOUT_SEL;

	enum {
		ETH_V_EVEN = 0,
		ETH_V_ODD = 1,
	} ETH_VOUT_SEL;
} SIE_ETH;
/**
    SIE RAW Encoder
*/
typedef struct _SIE_RAWENC {
	enum {
		MULTI_STRIPE_MODE  = 0,  ///< multi-stripe mode
		SINGLE_STRIPE_MODE = 1,  ///< single stripe mode
		ENUM_DUMMY4WORD(BCC_OP_MODE),
	} BCC_OP_MODE;

} SIE_RAWENC;


/**
    Sie bayer scaling Quality,
    Horizontal is workable when crop_H != DmaOutH
    Vertical is workable when crop_V != DmaOutV
*/
typedef struct _SIE_SCAL_PAR {
	BOOL    bRst;           ///< TRUE if need to reset lp weighting
	BS_QUALITY  ScalingQuality; ///< -
} SIE_SCAL_PAR;

/**
    digital gain
*/
typedef struct _SIE__DGAIN {
	UINT32  DigitalGain;        ///< -
} SIE__DGAIN;


/**
    color gain
*/
typedef struct _SIE__CGAIN {
	UINT32  CGainCh0;        ///< -
	UINT32  CGainCh1;        ///< -
	UINT32  CGainCh2;        ///< -
	UINT32  CGainCh3;        ///< -
	BOOL CGain_SEL_3_7;      ///< - disable 2.8 format, enable 3.7 format
} SIE__CGAIN;
/**
    SIE breakpoint/inverse
*/
typedef struct _SIE_BP {
	UINT32  BP1;        ///< break point1 y-coordinate
	UINT32  BP2;        ///< -
	UINT32  BP3;        ///< -
} SIE_BP;

typedef struct {
	UINT32      uiSizeX; ///< horizontal size
	UINT32      uiSizeY; ///< vertical size
} SRC_WIN_INFO;

/**
    channel information
*/
typedef struct _SIE_CH0PARAM {
	UINT32 RdyAddr;         ///< ch0 ready output address
	UINT32 RdyIdx;          ///< ch0 ready output address index
} SIE_CH0PARAM;
/**
    color accumulation information
*/
typedef struct _SIE_CAPARAM {
	UINT32 Addr;         ///< output address
	UINT32 WinNumH;      ///< ca window number(hor.)
	UINT32 WinNumV;      ///< ca window number(ver.)
	UINT32 WinSizeH;     ///< ca window size(hor.)
	UINT32 WinSizeV;     ///< ca window size(ver.)
	UINT32 NormFactor;   ///< ca normal factor
} SIE_CAPARAM;
/**
    Luminance accumulation information
*/
typedef struct _SIE_LAPARAM {
	UINT32 Addr;         ///< output address
	UINT32 WinNumH;      ///< la window number(hor.)
	UINT32 WinNumV;      ///< la window number(ver.)
	UINT32 WinSizeH;     ///< la window size(hor.)
	UINT32 WinSizeV;     ///< la window size(ver.)
	UINT32 WinSum;       ///< la window sum(only valid at HDR mode)
} SIE_LAPARAM;

/**
    color accumulation result
*/
typedef struct _SIE_CARST {
	UINT16 *pR;         ///< output: assign R acc result to this addr
	UINT16 *pG;         ///< output: assign G acc result to this addr
	UINT16 *pB;         ///< output: assign B acc result to this addr
	UINT16 *pIR;
	UINT16 *pRth;
	UINT16 *pGth;
	UINT16 *pBth;
	UINT16 *pAccCnt;  ///< output: assign pixel count of block
	UINT32  Length;     ///< input:  window num, ex: 8x8->64.   16x16->256
	SIE_FLIP_TYPE_INFO type;    ///< input: input parameter
} SIE_CARST;
/**
    Luminance accumulation result
*/
typedef struct _SIE_LARST {
	UINT16 *pLum1;              ///< output: luminance buffer1
	UINT16 *pLum2;              ///< output: luminance buffer2
	UINT16 *pHisto;             ///< output: historgram
	SIE_LAPARAM Param;          ///< input: input parameter
	SIE_FLIP_TYPE_INFO type;    ///< input: input parameter
} SIE_LARST;

/**
    variation accumulation result
*/
typedef enum _SIE_VARST_CH_SEL {
	SIE_VARST_CH3_SEL,
	SIE_VARST_CH4_SEL,
	ENUM_DUMMY4WORD(SIE_VARST_CH_SEL),
} SIE_VARST_CH_SEL;

typedef struct _SIE_VARST {
	UINT32 *pG1H;        ///< table pointer
	UINT32 *pG1V;        ///< table pointer
	UINT32 *pG2H;        ///< table pointer
	UINT32 *pG2V;        ///< table pointer
	SIE_FLIP_TYPE_INFO type;    ///< input: input parameter
	SIE_VARST_CH_SEL ch_sel;    ///< input: input parameter, select which channel
} SIE_VARST;
typedef struct _SIE_VA_INDEP_GRP {
	UINT32 pG1H;        ///< table pointer
	UINT32 pG1V;        ///< table pointer
	UINT32 pG2H;        ///< table pointer
	UINT32 pG2V;        ///< table pointer
	SIE_VARST_CH_SEL ch_sel;    ///< input: input parameter, select which channel
} SIE_VA_INDEP_GRP;
/**
    variation accumulation Indepent window result
*/
typedef struct _SIE_VA_INDEPWIN_RST {
	SIE_VA_INDEP_GRP IndepWinRST[5];
} SIE_VA_INDEPWIN_RST;

/*
    IPE edge threshold output lineoffset calculate

    only 2 bit each pixel(H*2/8).
*/
#define ETH_LINEOFS(H)  ( IPL_ALIGN_ROUNDUP((H/4),4) )
/**
    Edge threshold (dma output)
*/
typedef struct _SIE_EDGETHRE {
	UINT32  low;    ///< Lower bound threshold
	UINT32  mid;    ///< Middle bound threshold
	UINT32  high;   ///< Higher bound threshold
} SIE_EDGETHRE;

/**
    Burst length for all channel
*/

typedef struct _SIE_BURSTLENGTH_SET {
	SIE_OUT0_BURST_LENG burstLeng_OutCh0;                     //@SIE1/SIE2
	SIE_OUT1_BURST_LENG burstLeng_OutCh1;                     //@SIE1/SIE2/SIE3/SIE4
	SIE_OUT2_BURST_LENG burstLeng_OutCh2;                     //@SIE1/SIE2/SIE3/SIE4
	SIE_OUT3_BURST_LENG burstLeng_OutCh3;                     //@SIE1/SIE2
	SIE_OUT4_BURST_LENG burstLeng_OutCh4;                     //@SIE1/SIE2
	SIE_OUT5_BURST_LENG burstLeng_OutCh5;                     //@SIE1/SIE2
	SIE_IN0_BURST_LENG burstLeng_InCh0;                       //@SIE1/SIE2
	SIE_IN1_BURST_LENG burstLeng_InCh1;                       //@SIE1/SIE2
	SIE_IN2_BURST_LENG burstLeng_InCh2;                       //@SIE1/SIE2
} SIE_BURSTLENGTH_SET;

/**
    SIE YOUT Info
*/

typedef struct _SIE_HAL_YOUT_INFO {
	UINT32  crp_size_x;         ///< crop size
	UINT32  crp_size_y;         ///< crop size
	UINT32  win_num_x;          ///< yout window number_x
	UINT32  win_num_y;          ///< yout window number_y
	UINT32  win_size_x;         ///< yout window size_x, calculated by sie driver
	UINT32  win_size_y;         ///< yout window size_y, calculated by sie driver

	UINT32  cgain_en;           ///< enable cgain, disable will automaticaly set to 256(1x)
	UINT32  cgain_r;            ///< yout r gain
	UINT32  cgain_g;            ///< yout g gain
	UINT32  cgain_b;            ///< yout b gain
	BOOL    CGain_SEL_3_7;      ///< - disable 2.8 format, enable 3.7 format
} SIE_HAL_YOUT_INFO;

typedef struct _SIE_YOUT_PARAM {
	UINT32  addr;
	UINT32  lofs;				///< yout lineoffset
	UINT32  win_num_x;          ///< yout window number_x
	UINT32  win_num_y;          ///< yout window number_y
	UINT32  win_size_x;         ///< yout window size_x, calculated by sie driver
	UINT32  win_size_y;         ///< yout window size_y, calculated by sie driver
} SIE_YOUT_PARAM;

/**
    SIE edge threshold output

    current dma ready info
*/
typedef struct _SIE_ETH_DramRst {
	UINT32  addr;       ///< -
	UINT32  lineofs;    ///< -
	UINT32  width;      ///< -
	UINT32  height;     ///< -
	UINT32  fmt_8bit;
	UINT32  downsampled;
	SIE_EDGETHRE threshold;
	UINT32  frm_cnt;
} SIE_ETH_DramRst;


typedef struct _PAT_GEN_INFO {
	SIE_PAT_GEN_SEL type;       ///< pattern gen type (must be set DramIn_Win)  @SIE1/SIE2/SIE3/SIE4/SIE5/SIE6/SIE7/SIE8
	UINT32 val;                 ///< pattern gen value                          @SIE1/SIE2/SIE3/SIE4/SIE5/SIE6/SIE7/SIE8
} PAT_GEN_INFO;


/**
	SIE Grid line info
*/
typedef struct {
	UINT32              uiGridVal0;		///< Grid line Value0
	UINT32              uiGridVal1;		///< Grid line Value1
	UINT32              uiGridVal2;		///< Grid line Value2
	UINT32              uiGridVal3;		///< Grid line Value3
	UINT32              uiShftBs;		///< Period = 8 << uiShftBs
} SIE_HAL_GRIDLINE_INFO;

/**
     SIE sub function configuration.
*/
typedef struct _SIE_SUBFUNC {
	SIE_INTE_EN InteEn;     ///< interrupt enable                               @SIE1/SIE2/SIE3/SIE4
	SieFuncEn   sieFuncEn;  ///< function enable                                @SIE1/SIE2/SIE3/SIE4

	SIE_BP      BP;         ///< break point configuration                      @SIE1/SIE2/SIE3/SIE4

	SIE_DPC     Dpc;        ///< defect pixel configuration   @SIE1/SIE2
	SIE_OBDET   OBAvg;          ///< OB average dectection configuration        @SIE1/SIE2
	SIE_OBSUB   OBSub;          ///< OB substration configuration               @SIE1/SIE2
	SIE_OBFAVG  OBFAvg;         ///< OB frame configuration                     @SIE1
	SIE_OBPSUB  OBPSub;         ///< OB plane configuration                     @SIE1
	SIE__DGAIN  DGain;      ///< digital gain in 3.7 bits format                @SIE1/SIE2
	SIE__CGAIN  CGain;      ///< color gain in 2.8 or 3.7 format                @SIE1

	SIE_STATS_PATH_INFO pStatPathInfo;
	SIE_CACC    Ca;         ///< color acc. configuration                       @SIE1/SIE2
	SIE_LACC    La;         ///< luminanc acc. configuration                    @SIE1/SIE2
	SIE_VACC    Va;         ///< variation acc. configuration                   @SIE1/SIE2
	SIE_ETH     Eth;        ///< eth acc. configuration                         @SIE1/SIE2
	SIE_RAWENC  RawEnc;     ///< raw encode configuration                       @SIE1/SIE2
	SIE_SCAL_PAR *pScale;   ///< bayer scaler configuration                      @SIE1/SIE2

	struct {
		UINT32  Inaddr;         ///< ecs table input addr(ch1)                  @SIE1/SIE2
		ECSMAP_ENUM     EcsMap; ///< ecs table size                             @SIE1/SIE2
		SIE_EmbeddedCS *pecs;   ///<                                            @SIE1/SIE2
	} Ecs;                  ///< embed color shading configuration

	SIE_CA_VIG  *pVig;      ///< CACC vignette configuration                    @SIE1/SIE2
	SIE_PFP     *pPfpc;     ///< periodic fix pattern configuration             @SIE1
	SIE_DVI     *pDvi;      ///< DVI format configuration                       @SIE1/SIE2/SIE3/SIE4
	SIE_EDGETHRE *pEdgeThreshold;///< edge threshold                            @SIE1

	SIE_HAL_YOUT_INFO yout_info;    ///< SIE YOUT info                          @SIE1/SIE2/SIE3/SIE4

	PAT_GEN_INFO PatGen;		///< pattern gen

	SIE_HAL_GRIDLINE_INFO gridline_info;	///< gridline information(debug)
} SIE_SUBFUNC;

//@}
#endif

