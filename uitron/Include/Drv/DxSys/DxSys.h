/**
    DMA/CPU Configuration and Driver Initail

    Configure DMA/CPU Speed.

    @file       DxSys.h
    @ingroup    mDXSYS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DXDSP_H_
#define _DXDSP_H_

#include "Type.h"
#include "SxCmd.h"

/**
    @addtogroup mDXSYS
*/
//@{

/**
     Preare initail.

     Here has to call by _main() at first line to prepare inital hardware.
*/
extern void DxSys_Entry(void);

/**
     Check DRAM size

     Check project DRAM setting match DRAM configuration.

     @note Here has to call by _main() before calling DxSys_Init().

     @param[in] dramTotal DRAM size by byte.

     @code
     {
        DxSys_Validate(MEM_DRAM_SIZE);
     }
     @endcode
*/
extern void DxSys_Validate(UINT32 dramTotal);

/**
     Initialize system

     Initialize essential HW.

     @note Here already call by GxSystem at system startup.
*/
extern void DxSys_Init(void);

/**
     Exit system

     At last step of power off flow, call it to power off.

     @note Here already call by GxSystem at system power off flow.
*/
extern void DxSys_Exit(void); //exit driver

/**
     Configure DMA/CPU and some Engine speed

     User needs to implement this function at ChipCfg.c. During system startup,
     GxSystem will call it to initialize DMA/CPU and some Engine speed.
*/
extern void DxSys_CfgClk(void);

/**
     Exception callback function table

     The structure is used by DxSys_RegExpHandler to notify the exception to user
*/
typedef struct _EXP_HANDLER {
	void (*fp_exceptionCB)(UINT32 *tbl); ///< exception inform callback
	void (*fp_stackoverflowCB)(UINT32 tcb);///< stack overflow callback
}
EXP_HANDLER, *PEXP_HANDLER;

/**
     exception information

     information during exception. used in fp_exceptionCB tbl.
*/
typedef enum {
	EXP_CAUSE = 0x0,                        ///< Exception cause
	EXP_PC,                                 ///< Exception program counter
	EXP_SRSCTL,                             ///< Shadow register number
	EXP_ISR_FLAG,                           ///< Program is in xxx_isr(module hook on driver ISR)
	EXP_INFO_NUM,
	ENUM_DUMMY4WORD(EXP_INFO)
} EXP_INFO;



/**
     Register exception handler

     Register exception callback function table

     @param[in] h clock id, refer to @ref EXP_HANDLER
*/
extern void DxSys_RegExpHandler(EXP_HANDLER *h);


///////////////////////////////////////////////////////////////////////////////

/**
     @name CPU speed id
     @note speed_id for DxSys_ChangeCPUSpeed()
*/
//@{
#define CPUSPEED_LOW        0 ///< speed low
#define CPUSPEED_NORMAL     1 ///< speed normal
#define CPUSPEED_HIGH       2 ///< speed high
//@}

/**
     Change CPU speed

     Change current the CPU speed

     @param[in] speed_id CPUSPEED_XXX
*/
extern void DxSys_ChangeCPUSpeed(UINT32 uiSpeedId);

/**
     Get CPU speed

     Get current the CPU speed

     @return speed_id CPUSPEED_XXX
*/
extern UINT32 DxSys_GetCPUSpeed(void);

/**
     Dump CPU clock

     Show CPU clock rate on UART
*/
extern void DxSys_DumpCPUClk(void);

/**
     Confirm CPU clock

     Confirm CPU clock rate is the same with user setting.

     @note use in DxSys_CfgClk user implement to confirm the clock you want.

     param[in] uiCPUClk_user CPU clock by MHz
*/
extern void DxSys_DumpAndVerifyCPUClk(UINT32 uiCPUClk_user);


///////////////////////////////////////////////////////////////////////////////

/**
     @name DMA speed id
     @note speed_id for DxSys_ChangeDMASpeed() and DxSys_DumpAndVerifyDMAClk()
*/
//@{
#define DMASPEED_LOW        0 ///< speed low
#define DMASPEED_NORMAL     1 ///< speed normal
#define DMASPEED_HIGH       2 ///< speed high
//@}

/**
     Change DMA speed

     Change current the DMA speed

     @param[in] speed_id DMASPEED_XXX
*/
extern void DxSys_ChangeDMASpeed(UINT32 uiSpeedId);

/**
     Get DMA speed

     Get current the DMA speed

     @return speed_id DMASPEED_XXX
*/
extern UINT32 DxSys_GetDMASpeed(void);

/**
     Dump DMA clock

     Show DMA clock rate on UART
*/
extern void DxSys_DumpDMAClk(void);

/**
     Confirm DMA clock

     Confirm DMA clock rate is the same with user setting.

     @note use in DxSys_CfgClk user implement to confirm the clock you want.

     param[in] uiSpeedId DMA Speed by DMASPEED_XXX, Only is support High and Low currently

     param[in] uiDMAClk_user CPU clock by MHz (if it is 0, don't care)
*/
extern void DxSys_DumpAndVerifyDMAClk(UINT32 uiDmaSpeedId, UINT32 uiDMAClk_user);

///////////////////////////////////////////////////////////////////////////////

/**
     @name Engine clock id
     @note eng_id for DxSys_GetCfgClk()
*/
//@{
#define CFG_GRPH_CLK  1 ///< graph clock
#define CFG_GRPH2_CLK 2 ///< graph-22 clock
#define CFG_ISE_CLK   3 ///< ise clock
#define CFG_IME_CLK   4 ///< ime clock
#define CFG_JPEG_CLK  5 ///< jpeg clock
#define CFG_FDE_CLK   6 ///< fde clock
#define CFG_H264_CLK  7 ///< h264 clock
#define CFG_ROT_CLK   8 ///< rotate engine clock
#define CFG_COPY_CLK   9 ///< copy engine clock
#define CFG_RHE_CLK   10 ///< rhe clock
#define CFG_IFE_CLK   11 ///< ife clock
#define CFG_IFE2_CLK  12 ///< ife2 clock
#define CFG_IPE_CLK   13 ///< ipe clock
#define CFG_DCE_CLK   14 ///< dce clock

//@}

/**
     Get engine speed clock

     User needs to implement this function at ChipCfg.c to provide a getting information.

     @param[in] uiEngId Engine clock id, refer to CFG_XXX_CLK
     @return clock speed in MHz
*/
extern UINT32 DxSys_GetCfgClk(UINT32 uiEngId);

/**
    Dump all engine clock

    Show current all engines clock on UART
*/
extern void DxSys_DumpEngineClk(void);

/**
    Dump all engine dma priority

    Show current all engines dma priority on UART
*/
extern void DxSys_DumpEngineDma(void);


///////////////////////////////////////////////////////////////////////////////

/**
    Dump engine info of display

    Show engine info of display

     param[in] id   device id
*/
extern void DxSys_DumpDispEngineInfo(UINT32 id);

/**
     Dump data status of display.

     Dump data status of display.
     @note Nothing
*/
extern UINT32 DxSys_DumpDispDataStatus(UINT32 *pData);


///////////////////////////////////////////////////////////////////////////////

/**
     Dump capture driver/h.w setting

     Project can always call it, it takes care dump information activitily.
*/
extern void DxSys_DumpCaptureDrvInfo(void);


//@}
#endif //_DXDSP_H_
