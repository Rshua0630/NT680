/**
    IPH module (type define)

    @file       IPL_Hal.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef IPL_HAL_OP_H_
#define IPL_HAL_OP_H_
#include "ipl_hal_ife2.h"
#include "ipl_hal_ife.h"
#include "ipl_hal_sie_com.h"
#include "ipl_hal_dce.h"
#include "ipl_hal_ipe.h"
#include "ipl_hal_ime.h"
#include "ipl_hal_ise.h"
#include "ipl_hal_rhe.h"
#include "ipl_hal_dre.h"


/** \addtogroup mILibIPH */
//@{

/**
     engine supported by IPL_Hal
*/
typedef enum _IPL_HAL_ENG {
	IPL_HAL_UNKNOW  = 0,
	IPL_HAL_SIE1    = 1,    ///< SIE1
	IPL_HAL_SIE2    = 2,    ///< SIE2
	IPL_HAL_SIE3    = 3,    ///< SIE3
	IPL_HAL_SIE4    = 4,    ///< SIE4
	IPL_HAL_SIE5    = 5,    ///< SIE5
	IPL_HAL_SIE6    = 6,    ///< SIE6
	IPL_HAL_SIE7    = 7,    ///< SIE7
	IPL_HAL_SIE8    = 8,    ///< SIE8
	IPL_HAL_IFE     = 9,    ///< IFE
	IPL_HAL_DCE     = 10,   ///< DCE
	IPL_HAL_IPE     = 11,   ///< IPE
	IPL_HAL_IME     = 12,   ///< IME
	IPL_HAL_IFE2    = 13,   ///< IFE2
	IPL_HAL_ISE     = 14,   ///< ISE
	IPL_HAL_RHE     = 15,   ///< RHE
	IPL_HAL_DRE     = 16,   ///< DRE
	IPL_HAL_MAX     = 17,
	ENUM_DUMMY4WORD(IPL_HAL_ENG)
} IPL_HAL_ENG;


/**
     isr status
*/
typedef enum _IPL_ISR_STATUS {
	//ISR_REV         = 0,
	SIE_FLDEND      = 0x00000001,   ///< Field end
	SIE_BP1         = 0x00000002,   ///< Break point 1
	SIE_BP2         = 0x00000004,   ///< Break point 2
	SIE_BP3         = 0x00000008,   ///< Break point 3
	SIE_ACTST       = 0x00000010,   ///< Active window start
	SIE_CRPST       = 0x00000020,   ///< Crop window start
	SIE_DRAM_IO_ERR = 0x00000040,   ///< Enable interrupt
	SIE_DRAM0_END   = 0x00000080,   ///< Dram output channel 0 end
	SIE_DRAM1_END   = 0X00000100,   ///< Dram output channel 1 end
	SIE_DRAM2_END   = 0X00000200,   ///< Dram output channel 2 end
	SIE_DRAM3_END   = 0X00000400,   ///< Dram output channel 3 end
	SIE_DRAM4_END   = 0X00000800,   ///< Dram output channel 4 end
	SIE_DPCF        = 0X00001000,   ///< Defect Pixel/Column concealment warning,happened at continus bp.(more than 2 bp)
	SIE_RAWENC_BUFF = 0X00002000,   ///< Raw encoder output buffer full

	IPE_FMD         = 0x00000002,   ///< frame end
	IPE_STPE        = 0X00000004,   ///< stripe end
	IPE_FMS         = 0X00000008,   ///< stripe end
	IPE_YCC_DMAE    = 0x00000010,   ///< ycc dma output end

	IFE_FMD         = 0x00000001,   ///< frame end interrupt
	IFE_ROWDEFW     = 0x00000004,   ///< row defect warning interrupt

	IFE2_FMD        = 0x80000000,   ///<Frame end

	IME_STPE        = 0X40000000,   ///< stripe end
	IME_FMD         = 0x80000000,   ///< frame end

	DCE_FMS         = 0x00000001,   ///< frame start
	DCE_FMD         = 0x00000002,   ///< frame end
	DCE_STRPE       = 0x00000004,   ///< current stripe end

	ISE_FMD         = 0x80000000,   ///< frame-end interrupt status

	RHE_FMD         = 0x00000001,   ///< frame end
	RHE_DPC_WARN    = 0x00000002,   ///< Defect Concealment warning
	RHE_IN0_DEC_ERR = 0x00000004,   ///< Decoder error for input-0
	RHE_IN1_DEC_ERR = 0x00000008,   ///< Decoder error for input-1
} IPL_ISR_STATUS;

/**
    engine clk
    @note : sie engine clk set to 0 when the protocal of sensor data transition is LVDS
*/
typedef union _IPH_CLK {
	UINT32      SIEclk;     ///< SIEx clock
	IPH_IPECLK  IPEclk;     ///< IPE  clock
	IPH_DCECLK  DCEclk;     ///< DCE  clock
	IPH_IFECLK  IFEclk;     ///< IFE  clock
	IPH_IFE2CLK IFE2clk;    ///< IFE2 clock
	IPH_IMECLK  IMEclk;     ///< IME  clock
	IPH_ISECLK  ISEclk;     ///< ISE  clock
	IPH_RHECLK  RHEclk;     ///< RHE  clock
	UINT32      DREclk;     ///< DRE  clock
	UINT32 EngineClk;
} IPH_CLK;

/**
     engine para. update selection
*/
typedef union _IPL_HAL_GROUP {
	SIE_UPDATE  sieUpdate;  ///< SIE update selection
	IFE_UPDATE  ifeUpdate;  ///< IFE update selection
	IPE_UPDATE  ipeUpdate;  ///< IPE update selection
	DCE_UPDATE  dceUpdate;  ///< DCE update selection
	IFE2_UPDATE ife2Update; ///< IFE2 update selection
	IME_UPDATE  imeUpdate;  ///< IME update selection
	ISE_UPDATE  iseUpdate;  ///< ISE update selection
	RHE_UPDATE  rheUpdate;  ///< RHE update selection
	DRE_UPDATE  dreUpdate;  ///< DRE update selection
	UINT64 data;
} IPL_HAL_GROUP;

/**
     engine sub-function
*/
typedef struct _ENG_SUBFUNC {
	SIE_SUBFUNC    *pSie_sub;   ///< -
	IFE_SUBFUNC    *pIfe_sub;   ///< -
	DCE_SUBFUNC    *pDce_sub;   ///< -
	IPE_SUBFUNC    *pIpe_sub;   ///< -
	IFE2_SUBFUNC   *pIfe2_sub;  ///< -
	IME_SUBFUNC    *pIme_sub;   ///< -
	ISE_SUBFUNC    *pIse_sub;   ///< -
	RHE_SUBFUNC    *pRhe_sub;   ///< -
	DRE_SUBFUNC    *pDre_sub;   ///< -
} ENG_SUBFUNC;

/**
    engine inner table info
*/
typedef union {
	SIE_READ sieRead;
	IFE_READ ifeRead;
	DCE_READ dceRead;
	IPE_READ ipeRead;
	IFE2_READ ife2Read;
	IME_READ imeRead;
	DRE_READ dreRead;
	RHE_READ rheRead;
	UINT32 data;
} IPL_HAL_READ;


/**
     Index of parameter set supported by IPL_Hal
*/
typedef enum _IPL_HAL_PARASET {
	IPL_HAL_PAR_PRV    = 0,        ///< preview hal parameter index
	IPL_HAL_PAR_CAP    = 1,        ///< capture hal parameter index
	IPL_HAL_PARMAX,             ///< -
	ENUM_DUMMY4WORD(IPL_HAL_PARASET)
} IPL_HAL_PARASET;

/**
    query operation supported by each engine
*/
typedef union {
	IPH_SIE_QUE SieQ;
	IPH_IFE_QUE IfeQ;
	IPH_DCE_QUE DceQ;
	IPH_IPE_QUE IpeQ;
	IPH_IFE2_QUE Ife2Q;
	IPH_IME_QUE ImeQ;
	IPH_ISE_QUE ISE;
	IPH_DRE_QUE DreQ;
	IPH_RHE_QUE RheQ;
	UINT32 data;
} IPH_QUE;

#if 0
#endif
/**
     read hw register

     @note none

     @param[in]  eng       indicated engine
     @param[in]  Que       which infor need to be queried
     @param[out] Rst       output register rst,data type plz reference "IPH_QUE" description
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional);
     @code
     {
        // Example for calling foo();
     }
     @endcode
*/

extern ER SIEx_ENG_ReadReg(IPL_PROC_ID id, IPH_QUE Que, void *Output);
extern ER IPE_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER DCE_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER IFE_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER IFE2_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER IME_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER ISE_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER DRE_ENG_ReadReg(IPH_QUE Que, void *Output);
extern ER RHE_ENG_ReadReg(IPH_QUE Que, void *Output);

//@}
#endif //IPL_HAL_OP_H_
