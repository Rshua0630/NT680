/*
    PLL Configuration module

    Set the clock frequency and clock enable/disable of each module

    @file       pll.c
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include <common.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include "../cc/cc.h"
#include <asm/arch/nvt_common.h>
#include "pll.h"
//#include "pll_protected.h"
#include "pll_int.h"
#include "pll_reg.h"
//#include "clock.h"
//#include "Debug.h"
//#include "stdio.h"
//#include "cc_protected.h"

//extern CC_ER cc_hwLockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID);
//extern CC_ER cc_hwUnlockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID);

#define loc_multiCores()    {loc_cpu();cc_hwLockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_CKG);}
#define unl_multiCores()    {cc_hwUnlockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_CKG);unl_cpu();}

/**
    @addtogroup mIDrvSys_CG
*/
//@{
#if 0
static const PLLCLKCHECK Pll2ChkItem[] ={

    //{MIPI_DSI_CLK,  PLL_CLKSEL_DSI_CLKSRC,  PLL_CLKSEL_DSI_CLKSRC_PLL2,     "MIPI DSI"},
    {IDE1_CLK,      PLL_CLKSEL_IDE_CLKSRC,  PLL_CLKSEL_IDE_CLKSRC_PLL2,     "IDE"},
    {IDE2_CLK,      PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL2,    "IDE2"},
	{SDIO_CLK,		PLL_CLKSEL_SDIO,		PLL_CLKSEL_SDIO_PLL2,			"SDIO"},
	{SDIO2_CLK, 	PLL_CLKSEL_SDIO2,		PLL_CLKSEL_SDIO2_PLL2,			"SDIO2"},
	{SDIO3_CLK, 	PLL_CLKSEL_SDIO3,		PLL_CLKSEL_SDIO3_PLL2,			"SDIO3"},
	{SIE_CLK,		PLL_CLKSEL_SIE_CLKSRC,	PLL_CLKSEL_SIE_CLKSRC_PLL2,	    "SIE"},
	{SIE2_CLK,		PLL_CLKSEL_SIE2_CLKSRC, PLL_CLKSEL_SIE2_CLKSRC_PLL2,	"SIE2"},
	{SIE3_CLK,		PLL_CLKSEL_SIE3_CLKSRC, PLL_CLKSEL_SIE3_CLKSRC_PLL2,	"SIE3"},
	{SIE4_CLK,		PLL_CLKSEL_SIE4_CLKSRC, PLL_CLKSEL_SIE4_CLKSRC_PLL2,	"SIE4"},
    //{SP_CLK,        PLL_CLKSEL_SP,          PLL_CLKSEL_SP_PLL2,             "SP"},
    //{SP2_CLK,       PLL_CLKSEL_SP2,         PLL_CLKSEL_SP2_PLL2,            "SP2"},

};

static const PLLCLKCHECK Pll3ChkItem[] ={   // PLL3 only output to DMA
	//{DMA_CLK,       PLL_CLKSEL_DMA,	        PLL_CLKSEL_DMA_PLL3,		    "DMA"},
    //{DMA2_CLK,      PLL_CLKSEL_DMA2,        PLL_CLKSEL_DMA2_PLL3,           "DMA2"},
    //{DMA_CLK,       PLL_CLKSEL_DMA_ARBT,    PLL_CLKSEL_DMA_ARBT_PLL14,      "DMAARBT"},

};

static const PLLCLKCHECK Pll4ChkItem[] ={

    //{NAND_CLK,      PLL_CLKSEL_NAND,        PLL_CLKSEL_NAND_PLL4,           "NAND"},
    {SDIO_CLK,      PLL_CLKSEL_SDIO,        PLL_CLKSEL_SDIO_PLL4,           "SDIO"},
    {SDIO2_CLK,     PLL_CLKSEL_SDIO2,       PLL_CLKSEL_SDIO2_PLL4,          "SDIO2"},
    {SDIO3_CLK,     PLL_CLKSEL_SDIO3,       PLL_CLKSEL_SDIO3_PLL4,          "SDIO3"},
    //{MIPI_DSI_CLK,  PLL_CLKSEL_DSI_CLKSRC,  PLL_CLKSEL_DSI_CLKSRC_PLL4,     "MIPI DSI"},
    {IDE1_CLK,      PLL_CLKSEL_IDE_CLKSRC,  PLL_CLKSEL_IDE_CLKSRC_PLL4,     "IDE"},
    {IDE2_CLK,      PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL4,    "IDE2"},
	{SP_CLK,		PLL_CLKSEL_SP,		    PLL_CLKSEL_SP_PLL4,			    "SP"},
	{SP2_CLK, 	    PLL_CLKSEL_SP2,		    PLL_CLKSEL_SP2_PLL4,			"SP2"},
	{SIE_MCLK,		PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL4,	"SIE MCLK"},
	{SIE_MCLK2, 	PLL_CLKSEL_SIE_MCLK2SRC,PLL_CLKSEL_SIE_MCLK2SRC_PLL4,	"SIE MCLK2"},

};

static const PLLCLKCHECK Pll5ChkItem[] ={

    {SIE_MCLK,      PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL5,    "SIE MCLK"},
    {SIE_MCLK2,     PLL_CLKSEL_SIE_MCLK2SRC,PLL_CLKSEL_SIE_MCLK2SRC_PLL5,   "SIE MCLK2"},
    {SIE_CLK,       PLL_CLKSEL_SIE_CLKSRC,  PLL_CLKSEL_SIE_CLKSRC_PLL5,     "SIE"},
    {SIE2_CLK,      PLL_CLKSEL_SIE2_CLKSRC, PLL_CLKSEL_SIE2_CLKSRC_PLL5,    "SIE2"},
	{SIE3_CLK,		PLL_CLKSEL_SIE3_CLKSRC, PLL_CLKSEL_SIE3_CLKSRC_PLL5,	"SIE3"},
	{SIE4_CLK,		PLL_CLKSEL_SIE4_CLKSRC, PLL_CLKSEL_SIE4_CLKSRC_PLL5,	"SIE4"},
    //{SP_CLK,        PLL_CLKSEL_SP,          PLL_CLKSEL_SP_PLL5,             "SP"},
    //{SP2_CLK,       PLL_CLKSEL_SP2,         PLL_CLKSEL_SP2_PLL5,            "SP2"},

};

static const PLLCLKCHECK Pll6ChkItem[] ={

    {IPE_CLK,       PLL_CLKSEL_IPE,         PLL_CLKSEL_IPE_PLL6,            "IPE"},
    {DIS_CLK,       PLL_CLKSEL_DIS,         PLL_CLKSEL_DIS_PLL6,            "DIS"},
    {IME_CLK,       PLL_CLKSEL_IME,         PLL_CLKSEL_IME_PLL6,            "IME"},
    {FDE_CLK,       PLL_CLKSEL_FDE,         PLL_CLKSEL_FDE_PLL6,            "FDE"},
    {ISE_CLK,       PLL_CLKSEL_ISE,         PLL_CLKSEL_ISE_PLL6,            "ISE"},
    {DCE_CLK,       PLL_CLKSEL_DCE,         PLL_CLKSEL_DCE_PLL6,            "DCE"},
    {IFE_CLK,       PLL_CLKSEL_IFE,         PLL_CLKSEL_IFE_PLL6,            "IFE"},
    {IFE2_CLK,      PLL_CLKSEL_IFE2,        PLL_CLKSEL_IFE2_PLL6,           "IFE2"},
    {RHE_CLK,       PLL_CLKSEL_RHE,         PLL_CLKSEL_RHE_PLL6,            "RHE"},
    {DRE_CLK,       PLL_CLKSEL_DRE,         PLL_CLKSEL_DRE_PLL6,            "DRE"},
    {SVM_CLK,       PLL_CLKSEL_SVM,         PLL_CLKSEL_SVM_PLL6,            "SVM"},
    {CNN_CLK,       PLL_CLKSEL_CNN,         PLL_CLKSEL_CNN_PLL6,            "CNN"},
    {IVE_CLK,       PLL_CLKSEL_IVE,         PLL_CLKSEL_IVE_PLL6,            "IVE"},
    {SDE_CLK,       PLL_CLKSEL_SDE,         PLL_CLKSEL_SDE_PLL6,            "SDE"},
    {GRAPH_CLK,     PLL_CLKSEL_GRAPHIC,     PLL_CLKSEL_GRAPHIC_PLL6,        "GRAPHIC"},
    {GRAPH2_CLK,    PLL_CLKSEL_GRAPHIC2,    PLL_CLKSEL_GRAPHIC2_PLL6,       "GRAPHIC2"},
    {AFFINE_CLK,    PLL_CLKSEL_AFFINE,      PLL_CLKSEL_AFFINE_PLL6,         "AFFINE"},
    {HWCPY_CLK,     PLL_CLKSEL_HWCPY,       PLL_CLKSEL_HWCPY_PLL6,          "HWCPY"},
    {RORATE_CLK,    PLL_CLKSEL_ROTATE,      PLL_CLKSEL_ROTATE_PLL6,         "ROTATE"},
    {H26X_CLK,      PLL_CLKSEL_H26X,        PLL_CLKSEL_H26X_PLL6,           "H.264"},
    {JPG_CLK,       PLL_CLKSEL_JPEG,        PLL_CLKSEL_JPEG_PLL6,           "JPEG"},
};

static const PLLCLKCHECK Pll7ChkItem[] ={

    //{MIPI_DSI_CLK,  PLL_CLKSEL_DSI_CLKSRC,  PLL_CLKSEL_DSI_CLKSRC_PLL7,     "MIPI DSI"},
    //{IDE1_CLK,      PLL_CLKSEL_IDE_CLKSRC,  PLL_CLKSEL_IDE_CLKSRC_PLL7,     "IDE"},
    //{IDE2_CLK,      PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL7,    "IDE2"},
    //{DAI_CLK,       PLL_CLKSEL_ADO_CLKSEL,  PLL_CLKSEL_ADO_CLKSEL_CLKDIV,   "DAI"},
    //{HDMI_CLK,      PLL_CLKSEL_HDMI_ADO_CLKSEL,PLL_CLKSEL_HDMI_ADO_CLKSEL_CLKDIV,"HDMI audio"},
    //{SIE_CLK,       PLL_CLKSEL_SIE_CLKSRC,  PLL_CLKSEL_SIE_CLKSRC_PLL7,     "SIE"},
    //{SIE2_CLK,      PLL_CLKSEL_SIE2_CLKSRC, PLL_CLKSEL_SIE2_CLKSRC_PLL7,    "SIE2"},

};

static const PLLCLKCHECK Pll8ChkItem[] ={

    {0,             PLL_CLKSEL_CPU,         PLL_CLKSEL_CPU_PLL8,            "CPU"},

};

static const PLLCLKCHECK Pll9ChkItem[] ={

    {DSP_CLK,       PLL_CLKSEL_DSP,        PLL_CLKSEL_DSP_PLL9,             "DSP"},
	{DSP2_CLK,      PLL_CLKSEL_DSP2,       PLL_CLKSEL_DSP2_PLL9,            "DSP2"},

};

static const PLLCLKCHECK Pll10ChkItem[] ={

	{DSP2_CLK,      PLL_CLKSEL_DSP2,       PLL_CLKSEL_DSP2_PLL10,           "DSP2"},
	{SP_CLK,		PLL_CLKSEL_SP,		   PLL_CLKSEL_SP_PLL10,			    "SP"},
	{SP2_CLK, 	    PLL_CLKSEL_SP2,		   PLL_CLKSEL_SP2_PLL10,			"SP2"},
	{SIE_MCLK,		PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL10,	"SIE MCLK"},
	{SIE_MCLK2, 	PLL_CLKSEL_SIE_MCLK2SRC,PLL_CLKSEL_SIE_MCLK2SRC_PLL10,	"SIE MCLK2"},

};

static const PLLCLKCHECK Pll11ChkItem[] ={  // PLL11 only output to DSI

};

static const PLLCLKCHECK Pll12ChkItem[] ={
    //{IDE1_CLK,      PLL_CLKSEL_IDE_CLKSRC,  PLL_CLKSEL_IDE_CLKSRC_PLL12,    "IDE"},
    //{IDE2_CLK,      PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL12,   "IDE2"},
    //{TSMUX_CLK,     PLL_CLKSEL_TSMUX,       PLL_CLKSEL_TSMUX_PLL12,         "TSMUX"},
    //{SIE_CLK,       PLL_CLKSEL_SIE_CLKSRC,  PLL_CLKSEL_SIE_CLKSRC_PLL12,    "SIE"},
    //{SIE2_CLK,      PLL_CLKSEL_SIE2_CLKSRC, PLL_CLKSEL_SIE2_CLKSRC_PLL12,   "SIE2"},
    {ETH_CLK,       0,                      0,                              "ETH"},
};

static const PLLCLKCHECK Pll13ChkItem[] ={
    {DSP_CLK,       PLL_CLKSEL_DSP,        PLL_CLKSEL_DSP_PLL13,            "DSP"},
	{DSP2_CLK,      PLL_CLKSEL_DSP2,       PLL_CLKSEL_DSP2_PLL13,           "DSP2"},

    {IPE_CLK,       PLL_CLKSEL_IPE,         PLL_CLKSEL_IPE_PLL13,           "IPE"},
    {DIS_CLK,       PLL_CLKSEL_DIS,         PLL_CLKSEL_DIS_PLL13,           "DIS"},
    {IME_CLK,       PLL_CLKSEL_IME,         PLL_CLKSEL_IME_PLL13,           "IME"},
    {FDE_CLK,       PLL_CLKSEL_FDE,         PLL_CLKSEL_FDE_PLL13,           "FDE"},
    {ISE_CLK,       PLL_CLKSEL_ISE,         PLL_CLKSEL_ISE_PLL13,           "ISE"},
    {DCE_CLK,       PLL_CLKSEL_DCE,         PLL_CLKSEL_DCE_PLL13,           "DCE"},
    {IFE_CLK,       PLL_CLKSEL_IFE,         PLL_CLKSEL_IFE_PLL13,           "IFE"},
    {IFE2_CLK,      PLL_CLKSEL_IFE2,        PLL_CLKSEL_IFE2_PLL13,          "IFE2"},
    {RHE_CLK,       PLL_CLKSEL_RHE,         PLL_CLKSEL_RHE_PLL13,           "RHE"},
    {DRE_CLK,       PLL_CLKSEL_DRE,         PLL_CLKSEL_DRE_PLL13,           "DRE"},
    {SVM_CLK,       PLL_CLKSEL_SVM,         PLL_CLKSEL_SVM_PLL13,           "SVM"},
    {CNN_CLK,       PLL_CLKSEL_CNN,         PLL_CLKSEL_CNN_PLL13,           "CNN"},
    {IVE_CLK,       PLL_CLKSEL_IVE,         PLL_CLKSEL_IVE_PLL13,           "IVE"},
    {SDE_CLK,       PLL_CLKSEL_SDE,         PLL_CLKSEL_SDE_PLL13,           "SDE"},
    {GRAPH_CLK,     PLL_CLKSEL_GRAPHIC,     PLL_CLKSEL_GRAPHIC_PLL13,       "GRAPHIC"},
    {GRAPH2_CLK,    PLL_CLKSEL_GRAPHIC2,    PLL_CLKSEL_GRAPHIC2_PLL13,      "GRAPHIC2"},
    {AFFINE_CLK,    PLL_CLKSEL_AFFINE,      PLL_CLKSEL_AFFINE_PLL13,        "AFFINE"},
    {HWCPY_CLK,     PLL_CLKSEL_HWCPY,       PLL_CLKSEL_HWCPY_PLL13,         "HWCPY"},
    {RORATE_CLK,    PLL_CLKSEL_ROTATE,      PLL_CLKSEL_ROTATE_PLL13,        "ROTATE"},
    {H26X_CLK,      PLL_CLKSEL_H26X,        PLL_CLKSEL_H26X_PLL13,          "H.264"},
    {JPG_CLK,       PLL_CLKSEL_JPEG,        PLL_CLKSEL_JPEG_PLL13,          "JPEG"},



};

static const PLLCLKCHECK Pll14ChkItem[] ={
    {DMA2_CLK,      PLL_CLKSEL_DMA2,        PLL_CLKSEL_DMA2_PLL14,          "DMA2"},
    //{DMA_CLK,       PLL_CLKSEL_DMA_ARBT,    PLL_CLKSEL_DMA_ARBT_PLL14,      "DMAARBT"},
    {IDE1_CLK,      PLL_CLKSEL_IDE_CLKSRC,  PLL_CLKSEL_IDE_CLKSRC_PLL14,    "IDE"},
    {IDE2_CLK,      PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL14,   "IDE2"},
	{SP_CLK,		PLL_CLKSEL_SP,			PLL_CLKSEL_SP_PLL14, 			"SP"},
	{SP2_CLK,		PLL_CLKSEL_SP2, 		PLL_CLKSEL_SP2_PLL14,			"SP2"},
	{SIE_CLK,		PLL_CLKSEL_SIE_CLKSRC,	PLL_CLKSEL_SIE_CLKSRC_PLL14,	"SIE"},
	{SIE2_CLK,		PLL_CLKSEL_SIE2_CLKSRC, PLL_CLKSEL_SIE2_CLKSRC_PLL14,	"SIE2"},
	{SIE3_CLK,		PLL_CLKSEL_SIE3_CLKSRC, PLL_CLKSEL_SIE3_CLKSRC_PLL14,	"SIE3"},
	{SIE4_CLK,		PLL_CLKSEL_SIE4_CLKSRC, PLL_CLKSEL_SIE4_CLKSRC_PLL14,	"SIE4"},
};


/*
    Check PLL Usage

    @param[in] id           PLL ID

    @return
        - @b TRUE: id is currently ALLOCATED/USED by some module
        - @b FALSE: id is free
*/
static BOOL pll_checkPllUsage(PLL_ID id)
{
    UINT32  i;
    UINT32  Number;
    BOOL    ClkEnable;
    const char* pPllName;
    PPLLCLKCHECK pCheckItem;
    const PPLLCLKCHECK vPllItems[] = {(PPLLCLKCHECK)Pll2ChkItem, (PPLLCLKCHECK)Pll3ChkItem,
                                (PPLLCLKCHECK)Pll4ChkItem, (PPLLCLKCHECK)Pll5ChkItem,
                                (PPLLCLKCHECK)Pll6ChkItem, (PPLLCLKCHECK)Pll7ChkItem,
                                (PPLLCLKCHECK)Pll8ChkItem, (PPLLCLKCHECK)Pll9ChkItem,
                                (PPLLCLKCHECK)Pll10ChkItem,(PPLLCLKCHECK)Pll11ChkItem,
                                (PPLLCLKCHECK)Pll12ChkItem,(PPLLCLKCHECK)Pll13ChkItem,
                                (PPLLCLKCHECK)Pll14ChkItem};
    const UINT32 vPllItemCnt[] = {sizeof(Pll2ChkItem)/sizeof(Pll2ChkItem[0]),
                                sizeof(Pll3ChkItem)/sizeof(Pll3ChkItem[0]),
                                sizeof(Pll4ChkItem)/sizeof(Pll4ChkItem[0]),
                                sizeof(Pll5ChkItem)/sizeof(Pll5ChkItem[0]),
                                sizeof(Pll6ChkItem)/sizeof(Pll6ChkItem[0]),
                                sizeof(Pll7ChkItem)/sizeof(Pll7ChkItem[0]),
                                sizeof(Pll8ChkItem)/sizeof(Pll8ChkItem[0]),
                                sizeof(Pll9ChkItem)/sizeof(Pll9ChkItem[0]),
                                sizeof(Pll10ChkItem)/sizeof(Pll10ChkItem[0]),
                                sizeof(Pll11ChkItem)/sizeof(Pll11ChkItem[0]),
                                sizeof(Pll12ChkItem)/sizeof(Pll12ChkItem[0]),
                                sizeof(Pll13ChkItem)/sizeof(Pll13ChkItem[0]),
	                            sizeof(Pll14ChkItem)/sizeof(Pll14ChkItem[0])};
    const char* vPllName[] = {"PLL2", "PLL3", "PLL4", "PLL5", "PLL6", "PLL7", "PLL8", "PLL9", "PLL10", "PLL11", "PLL12", "PLL13", "PLL14"};

    if ((id < PLL_ID_2) || (id > PLL_ID_14))
    {
        return TRUE;
    }

    pCheckItem = vPllItems[id - PLL_ID_2];
    Number = vPllItemCnt[id - PLL_ID_2];
    pPllName = vPllName[id - PLL_ID_2];

    // Check all Clock Select related to id (instance of PLL_ID)
    for (i=0;i<Number;i++)
    {
        if (pCheckItem[i].uiSrcModule == PLL_CLKSEL_CPU)
        {
            // CPU's clock is always ON.
            ClkEnable = TRUE;
        }
        else
        {
            ClkEnable = pll_isClockEnabled(pCheckItem[i].uiSrcEnabled);
        }


        if (ClkEnable)
        {
            // Module Clock is enabled, Check the clock source
            if(pll_getClockRate(pCheckItem[i].uiSrcModule) == pCheckItem[i].uiCheckItem)
            {
                DBG_ERR("Change (%s) clock freq fail! (%s) is using %s. Don't change clock when module is active\r\n", pPllName, pCheckItem[i].ModuleName, pPllName);
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*
    Set (Driver scope) PLL frequency

    (This API should only be invoked by driver)

    @param[in] id           PLL ID
    @param[in] uiSetting    PLL frequency setting. PLL frequency will be 12MHz * uiSetting / 131072.
                            (Valid value: 0 ~ 0xFFFFFF)

    @return
        - @b E_OK: success
        - @b E_ID: PLL ID is out of range
        - @b E_PAR: uiSetting is out of range
        - @b E_MACV: Any module use this PLL and is enabled.
*/
ER pll_setDrvPLL(PLL_ID id, UINT32 uiSetting)
{
    BOOL bEn;
    T_PLL_PLL2_CR0_REG reg0 = {0};
    T_PLL_PLL2_CR1_REG reg1 = {0};
    T_PLL_PLL2_CR2_REG reg2 = {0};
    const UINT32 vPllAddr[] = {PLL_PLL2_CR0_REG_OFS, PLL_PLL3_CR0_REG_OFS, PLL_PLL4_CR0_REG_OFS,
                               PLL_PLL5_CR0_REG_OFS, PLL_PLL6_CR0_REG_OFS, PLL_PLL7_CR0_REG_OFS,
                               PLL_PLL8_CR0_REG_OFS, PLL_PLL9_CR0_REG_OFS, PLL_PLL10_CR0_REG_OFS,
                               PLL_PLL11_CR0_REG_OFS,PLL_PLL12_CR0_REG_OFS,PLL_PLL13_CR0_REG_OFS,
                               PLL_PLL14_CR0_REG_OFS};

    if (uiSetting > 0xFFFFFF)
    {
        DBG_ERR("uiSetting out of range: 0x%lx\r\n", uiSetting);
        return E_PAR;
    }

    if ((id < PLL_ID_2) || (id > PLL_ID_14))
    {
        DBG_ERR("id out of range: 0x%lx\r\n", id);
        return E_ID;
    }

    if (pll_checkPllUsage(id))
    {
        return E_MACV;
    }

    reg0.Bit.PLL_RATIO0 = uiSetting & 0xFF;
    reg1.Bit.PLL_RATIO1 = (uiSetting>>8) & 0xFF;
    reg2.Bit.PLL_RATIO2 = (uiSetting>>16) & 0xFF;

    bEn = pll_getPLLEn(id);
    // PLL8 should be modified when it is enabled
    if ((id != PLL_ID_7) && (id != PLL_ID_8))
    {
        pll_setPLLEn(id, FALSE);
    }

    //race condition protect. enter critical section
    loc_multiCores();

    switch (id)
    {
    /*se PLL_ID_3:
        {
            T_PLL_PLL3_CTRL0_REG pll3Ctrl0Reg;
            pll3Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL3_CTRL0_REG_OFS);
            pll3Ctrl0Reg.Bit.SSC_EN = 0;
            PLL_SETREG(PLL_PLL3_CTRL0_REG_OFS, pll3Ctrl0Reg.Reg);
        }
        break;*/
    case PLL_ID_4:
        {
            T_PLL_PLL4_SSPLL1_REG pll4Ctrl0Reg;
            pll4Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL1_REG_OFS);
            pll4Ctrl0Reg.Bit.SSC_EN = 0;
            PLL_SETREG(PLL_PLL4_SSPLL1_REG_OFS, pll4Ctrl0Reg.Reg);
        }
        break;
    case PLL_ID_2:
        {
            T_PLL_PLL2_SSPLL1_REG pll2Ctrl0Reg;
            pll2Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL1_REG_OFS);
            pll2Ctrl0Reg.Bit.SSC_EN = 0;
            PLL_SETREG(PLL_PLL2_SSPLL1_REG_OFS, pll2Ctrl0Reg.Reg);
        }
        break;

    default:
        break;
    }

    PLL_SETREG(vPllAddr[id-PLL_ID_2], reg0.Reg);
    PLL_SETREG(vPllAddr[id-PLL_ID_2] + 0x04, reg1.Reg);
    PLL_SETREG(vPllAddr[id-PLL_ID_2] + 0x08, reg2.Reg);

    //race condition protect. leave critical section
    unl_multiCores();

    if (bEn && ((id != PLL_ID_7) && (id != PLL_ID_8)))
    {
        DBG_WRN("PLL%d should be disabled before set\r\n", id+1);
    }

    return E_OK;
}

/**
    Set PLL frequency

    @param[in] id           PLL ID
                            - @b PLL_ID_2
                            - @b PLL_ID_3
                            - @b PLL_ID_5
                            - @b others: NOT support
    @param[in] uiSetting    PLL frequency setting. PLL frequency will be 12MHz * uiSetting / 131072.
                            (Valid value: 0 ~ 0xFFFFFF)

    @return
        - @b E_OK: success
        - @b E_ID: PLL ID is out of range
        - @b E_PAR: uiSetting is out of range
        - @b E_MACV: Any module use this PLL and is enabled.
*/
ER pll_setPLL(PLL_ID id, UINT32 uiSetting)
{
    if ((id < PLL_ID_2) || (id > PLL_ID_14))
    {
        DBG_ERR("id out of range: 0x%lx\r\n", id);
        return E_ID;
    }
    switch (id)
    {
    case PLL_ID_2:
    case PLL_ID_4:
    case PLL_ID_5:
    case PLL_ID_11:
    case PLL_ID_12:
        return pll_setDrvPLL(id, uiSetting);
    default:
        return E_ID;
    }
}

/**
    Get PLL Enable

    @param[in] id           PLL ID

    @return
        - @b TRUE: PLL is enabled
        - @b FALSE: PLL is disabled or id is out of range
*/
BOOL pll_getPLLEn(PLL_ID id)
{
    T_PLL_PLL_PWREN_REG pllEnReg;

    if (id == PLL_ID_1)
    {
        return TRUE;
    }
    if ((id < PLL_ID_2) || (id > PLL_ID_14))
    {
        DBG_ERR("id out of range: 0x%lx\r\n", id);
        return FALSE;
    }

    pllEnReg.Reg = PLL_GETREG(PLL_PLL_PWREN_REG_OFS);
    if (pllEnReg.Reg & (1 << (id+1)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
/**
    Set PLL Enable

    @param[in] id           PLL ID
    @param[in] bEnable      enable/disable PLL
                            - @b TRUE: enable PLL
                            - @b FALSE: disable PLL

    @return
        - @b E_OK: success
        - @b E_ID: PLL ID is out of range
*/
ER pll_setPLLEn(PLL_ID id, BOOL bEnable)
{
    union T_PLL_PLL_PWREN_REG pllEnReg;
    union T_PLL_PLL_STATUS_REG pllStsReg;

    if ((id < PLL_ID_2) || (id > PLL_ID_14))
    {
        printf("id out of range: 0x%x\r\n", id);
        return E_ID;
    }

    pllEnReg.Reg = PLL_GETREG(PLL_PLL_PWREN_REG_OFS);
    if (bEnable)
    {
        pllEnReg.Reg |= 1<<(id+1);
    }
    else
    {
        pllEnReg.Reg &= ~(1<<(id+1));
    }
    PLL_SETREG(PLL_PLL_PWREN_REG_OFS, pllEnReg.Reg);

    if (bEnable)
    {
        // Wait PLL power is powered on
        while (1)
        {
            pllStsReg.Reg = PLL_GETREG(PLL_PLL_STATUS_REG_OFS);
            if (pllStsReg.Reg & (1<<(id+1)))
            {
                break;
            }
        }
    }

    return E_OK;
}
#if 0
/**
    Get PLL frequency

    Get PLL frequency (unit:Hz)
    When spread spectrum is enabled (by pll_setPLLSpreadSpectrum()), this API will return lower bound frequency of spread spectrum.

    @param[in] id           PLL ID

    @return PLL frequency (unit:Hz)
*/
UINT32 pll_getPLLFreq(PLL_ID id)
{
#if _FPGA_EMULATION_
    if (id == PLL_ID_7)
    {
        return (PLL_FPGA_PLL7SRC);
    }
    else if ((id == PLL_ID_2) || (id == PLL_ID_5) || (id == PLL_ID_11))
    {
        return (PLL_FPGA_PLL2RC * 2);
    }
    else if (id == PLL_ID_3)
    {
        return (PLL_FPGA_PLL3SRC *2);
    }
    else if (id == PLL_ID_6)
    {
        return (PLL_FPGA_PLL2RC * 2)/2;
    }
    else if ((id == PLL_ID_9) || (id == PLL_ID_10))
    {
        return (PLL_FPGA_PLL2RC * 2)/4;
    }
    else if (id == PLL_ID_12)
    {
        return (PLL_FPGA_PLL12SRC * 2);
    }
    else if (id == PLL_ID_13)
    {
        return (PLL_FPGA_PLL2RC * 2)/3;
    }
    else if (id == PLL_ID_14)
    {
        return (PLL_FPGA_PLL2RC * 2)/3;
    }

    else
    {
        return (PLL_FPGA_480SRC*2);
    }
#else
    UINT64 pllRatio;
    T_PLL_PLL2_CR0_REG reg0 = {0};
    T_PLL_PLL2_CR1_REG reg1 = {0};
    T_PLL_PLL2_CR2_REG reg2 = {0};
    const UINT32 vPllAddr[] = {PLL_PLL2_CR0_REG_OFS, PLL_PLL3_CR0_REG_OFS, PLL_PLL4_CR0_REG_OFS,
                               PLL_PLL5_CR0_REG_OFS, PLL_PLL6_CR0_REG_OFS, PLL_PLL7_CR0_REG_OFS,
                               PLL_PLL8_CR0_REG_OFS, PLL_PLL9_CR0_REG_OFS, PLL_PLL10_CR0_REG_OFS,
                               PLL_PLL11_CR0_REG_OFS,PLL_PLL12_CR0_REG_OFS,PLL_PLL13_CR0_REG_OFS,
                               PLL_PLL14_CR0_REG_OFS};

    if ((id < PLL_ID_1) || (id > PLL_ID_14))
    {
        DBG_ERR("id out of range: 0x%lx\r\n", id);
        return 0;
    }

    if (id == PLL_ID_1)
    {
        return 480000000;
    }
    else if (id == PLL_ID_2)
    {
        UINT32 uiLower, uiUpper;
        T_PLL_PLL2_SSPLL1_REG pll2Ctrl0Reg;

        pll_getPLLSpreadSpectrum(PLL_ID_2, &uiLower, &uiUpper);

        // Get SSC_EN
        pll2Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL1_REG_OFS);
        if (pll2Ctrl0Reg.Bit.SSC_EN)
        {
            return uiLower;
        }
        else
        {
            return uiUpper;
        }
    }
    /*else if (id == PLL_ID_3)
    {
        UINT32 uiLower, uiUpper;
        T_PLL_PLL3_CTRL0_REG pll3Ctrl0Reg;

        pll_getPLLSpreadSpectrum(PLL_ID_3, &uiLower, &uiUpper);

        // Get SSC_EN
        pll3Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL3_CTRL0_REG_OFS);
        if (pll3Ctrl0Reg.Bit.SSC_EN)
        {
            return uiLower;
        }
        else
        {
            return uiUpper;
        }

    }*/
    else if (id == PLL_ID_4)
    {
        UINT32 uiLower, uiUpper;
        T_PLL_PLL4_SSPLL1_REG pll4Ctrl0Reg;

        pll_getPLLSpreadSpectrum(PLL_ID_4, &uiLower, &uiUpper);

        // Get SSC_EN
        pll4Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL1_REG_OFS);
        if (pll4Ctrl0Reg.Bit.SSC_EN)
        {
            return uiLower;
        }
        else
        {
            return uiUpper;
        }
    }
    else if (id == PLL_ID_12)
    {
        UINT32 uiLower, uiUpper;
        T_PLL_PLL12_SSPLL1_REG pll12Ctrl0Reg;

        pll_getPLLSpreadSpectrum(PLL_ID_12, &uiLower, &uiUpper);

        // Get SSC_EN
        pll12Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL1_REG_OFS);
        if (pll12Ctrl0Reg.Bit.SSC_EN)
        {
            return uiLower;
        }
        else
        {
            return uiUpper;
        }
    }
    reg0.Reg =PLL_GETREG(vPllAddr[id-PLL_ID_2]);
    reg1.Reg =PLL_GETREG(vPllAddr[id-PLL_ID_2] + 0x04);
    reg2.Reg =PLL_GETREG(vPllAddr[id-PLL_ID_2] + 0x08);
    pllRatio = (reg2.Bit.PLL_RATIO2 << 16) | (reg1.Bit.PLL_RATIO1 << 8) | (reg0.Bit.PLL_RATIO0 << 0);

    return 12000000*pllRatio / 131072;
#endif
}

/**
    Set PLL to Spread Spectrum mode

    Set PLL to Spread Spectrum mode
    SD driver and display object will use real lower bound frequency to calculate their target frequency.
    Please aware that the real frequency is still from uiLowerFreq to uiUpperFreq.

    @note Only PLL_ID_2 and PLL_ID_4 support spread spectrum

    @param[in] id           PLL ID
    @param[in] uiLowerFreq  lower bound frequency for spread spectrum (valid range: 426000000 ~ 480000000)
    @param[in] uiUpperFreq  upper bound frequency for spread spectrum (valid range: 426000000 ~ 480000000)

    @return
        - @b E_OK: success
        - @b E_NOSPT: PLL id not support spread spectrum
        - @b E_MACV: PLL id is NOT disabled before invoke this function
*/
ER pll_setPLLSpreadSpectrum(PLL_ID id, UINT32 uiLowerFreq, UINT32 uiUpperFreq)
{
#if _FPGA_EMULATION_
    DBG_ERR("NOT support in FPGA phase\r\n");
    return E_NOSPT;
#else
    FLOAT fPercent;
    UINT32 uiRealLow, uiRealUpper, uiMiddle;
    UINT32 uiRatio;
    UINT32 uiSSP;                           // Store spread spectrum percentage (Q1.11 fixed point format)
    UINT32 uiSscMulfac;
    const UINT32 uiSscStep = 0x02;          // 0: 2^17step, 1: 2^18step, 2: 2^19step, 3: 2^20step
    const UINT32 uiSscPeriodValue = 1<<7;   // Fix period value at 2^7

    // Only PLL3(DMA) and PLL4(SSPLL) support spread spectrum
    // But we only allow user to change PLL4 due to DMA frequency can not be easily changed.
    if ((id != PLL_ID_4) && (id != PLL_ID_2) && (id != PLL_ID_12))
    {
        DBG_ERR("PLL%d not support spread spectrum\r\n", id+1);
        return E_NOSPT;
    }

    // Ensure upper bound > lower bound
    if (uiUpperFreq < uiLowerFreq)
    {
        UINT32 t;

        t = uiUpperFreq;
        uiUpperFreq = uiLowerFreq;
        uiLowerFreq = t;
    }

    // Check if PLL is already disabled
    if (pll_getPLLEn(id) == TRUE)
    {
        DBG_ERR("PLL%d must be disabled\r\n", id+1);
        return E_MACV;
    }

    // Target Freq = RATIO * 12M / 131072
    uiRatio = ((UINT64)uiUpperFreq) * 131072 / 12000000;
    pll_setDrvPLL(id, uiRatio);

    // SSP% = SSC_MULFAC[7..0] * SSC_PERIOD_VALUE[7..0] / 2^(17+SSC_STEP_SEL[2..0])
    // SSP% = (uiUpperFreq - uiLowerFreq) / uiUpperFreq
    // Assume:
    //          1. SSC_STEP_SEL[2..0] = 3
    //          2. SSC_PERIOD_VALUE[7..0] = 2^7
    // ==> SSP_N / 2^M = SSC_MULFAC[7..0] * 2^7 / 2^(17+3)
    // ==> SSP_N / 2^M = SSC_MULFAC[7..0] * / 2^13
    //
    // We can treat:
    //          1. SSP_N = SSC_MULFAC[7..0]
    //          2. M = 13 (i.e. SSP% can be presented by Q1.13 format)
    // Translate SSP from float to Q1.[10+SSC_STEP_SEL]
    uiSSP = (((FLOAT)(uiUpperFreq - uiLowerFreq)) / uiUpperFreq) * (1<<(17+uiSscStep-7));
    uiSscMulfac = uiSSP;

    // Enter critical section
    loc_multiCores();

    /*if (id == PLL_ID_3)
    {
        T_PLL_PLL3_CTRL0_REG pll3Ctrl0Reg;
        T_PLL_PLL3_CFG0_REG pll3Cfg0Reg;
        T_PLL_PLL3_CFG1_REG pll3Cfg1Reg = {0};
        T_PLL_PLL3_CFG2_REG pll3Cfg2Reg = {0};

        pll3Cfg1Reg.Bit.SSC_MULFAC = uiSscMulfac;
        pll3Cfg2Reg.Bit.SSC_PERIOD_VALUE = uiSscPeriodValue;

        // 1. Setup period value
        PLL_SETREG(PLL_PLL3_CFG2_REG_OFS, pll3Cfg2Reg.Reg);
        // 2. Setup MULFAC
        PLL_SETREG(PLL_PLL3_CFG1_REG_OFS, pll3Cfg1Reg.Reg);
        // 3. Setup STEP_SEL
        pll3Cfg0Reg.Reg = PLL_GETREG(PLL_PLL3_CFG0_REG_OFS);
        pll3Cfg0Reg.Bit.SSC_STEP_SEL = uiSscStep;
        PLL_SETREG(PLL_PLL3_CFG0_REG_OFS, pll3Cfg0Reg.Reg);
        // 4. Write 1 to SSC_NEW_MODE, DSSC
        pll3Cfg0Reg.Bit.SSC_NEW_MODE = 1;
        pll3Cfg0Reg.Bit.DSSC = 1;
        PLL_SETREG(PLL_PLL3_CFG0_REG_OFS, pll3Cfg0Reg.Reg);
        // 5. Write 1 to SSC_EN
        pll3Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL3_CTRL0_REG_OFS);
        pll3Ctrl0Reg.Bit.SSC_EN = 1;
        PLL_SETREG(PLL_PLL3_CTRL0_REG_OFS, pll3Ctrl0Reg.Reg);
    }*/
    if (id == PLL_ID_2)
    {
        T_PLL_PLL2_SSPLL1_REG pll2sspl1Reg;
        T_PLL_PLL2_SSPLL2_REG pll2sspl2Reg = {0};
        T_PLL_PLL2_SSPLL3_REG pll2sspl3Reg = {0};
        T_PLL_PLL2_SSPLL0_REG pll2sspl0Reg;

        pll2sspl2Reg.Bit.SSC_MULFAC = uiSscMulfac;
        pll2sspl3Reg.Bit.SSC_PERIOD_VALUE = uiSscPeriodValue;

        // 1. Setup period value
        PLL_SETREG(PLL_PLL2_SSPLL3_REG_OFS, pll2sspl3Reg.Reg);
        // 2. Setup MULFAC
        PLL_SETREG(PLL_PLL2_SSPLL2_REG_OFS, pll2sspl2Reg.Reg);
        // 3. Setup STEP_SEL
        pll2sspl0Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL0_REG_OFS);
        pll2sspl0Reg.Bit.SSC_STEP_SEL = uiSscStep;
        PLL_SETREG(PLL_PLL2_SSPLL0_REG_OFS, pll2sspl0Reg.Reg);
        // 4. Write 1 to SSC_NEW_MODE, DSSC
        pll2sspl0Reg.Bit.SSC_NEW_MODE = 1;
        pll2sspl0Reg.Bit.DSSC = 1;
        PLL_SETREG(PLL_PLL2_SSPLL0_REG_OFS, pll2sspl0Reg.Reg);
        // 5. Write 1 to SSC_EN
        pll2sspl1Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL1_REG_OFS);
        pll2sspl1Reg.Bit.SSC_EN = 1;
        PLL_SETREG(PLL_PLL2_SSPLL1_REG_OFS, pll2sspl1Reg.Reg);

    }
    else if (id == PLL_ID_4)
    {
        T_PLL_PLL4_SSPLL1_REG pll4sspl1Reg;
        T_PLL_PLL4_SSPLL2_REG pll4sspl2Reg = {0};
        T_PLL_PLL4_SSPLL3_REG pll4sspl3Reg = {0};
        T_PLL_PLL4_SSPLL0_REG pll4sspl0Reg;

        pll4sspl2Reg.Bit.SSC_MULFAC = uiSscMulfac;
        pll4sspl3Reg.Bit.SSC_PERIOD_VALUE = uiSscPeriodValue;

        // 1. Setup period value
        PLL_SETREG(PLL_PLL4_SSPLL3_REG_OFS, pll4sspl3Reg.Reg);
        // 2. Setup MULFAC
        PLL_SETREG(PLL_PLL4_SSPLL2_REG_OFS, pll4sspl2Reg.Reg);
        // 3. Setup STEP_SEL
        pll4sspl0Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL0_REG_OFS);
        pll4sspl0Reg.Bit.SSC_STEP_SEL = uiSscStep;
        PLL_SETREG(PLL_PLL4_SSPLL0_REG_OFS, pll4sspl0Reg.Reg);
        // 4. Write 1 to SSC_NEW_MODE, DSSC
        pll4sspl0Reg.Bit.SSC_NEW_MODE = 1;
        pll4sspl0Reg.Bit.DSSC = 1;
        PLL_SETREG(PLL_PLL4_SSPLL0_REG_OFS, pll4sspl0Reg.Reg);
        // 5. Write 1 to SSC_EN
        pll4sspl1Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL1_REG_OFS);
        pll4sspl1Reg.Bit.SSC_EN = 1;
        PLL_SETREG(PLL_PLL4_SSPLL1_REG_OFS, pll4sspl1Reg.Reg);
    }
	else
	{
        T_PLL_PLL12_SSPLL1_REG pll12sspl1Reg;
        T_PLL_PLL12_SSPLL2_REG pll12sspl2Reg = {0};
        T_PLL_PLL12_SSPLL3_REG pll12sspl3Reg = {0};
        T_PLL_PLL12_SSPLL0_REG pll12sspl0Reg;

        pll12sspl2Reg.Bit.SSC_MULFAC = uiSscMulfac;
        pll12sspl3Reg.Bit.SSC_PERIOD_VALUE = uiSscPeriodValue;

        // 1. Setup period value
        PLL_SETREG(PLL_PLL12_SSPLL3_REG_OFS, pll12sspl3Reg.Reg);
        // 2. Setup MULFAC
        PLL_SETREG(PLL_PLL12_SSPLL2_REG_OFS, pll12sspl2Reg.Reg);
        // 3. Setup STEP_SEL
        pll12sspl0Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL0_REG_OFS);
        pll12sspl0Reg.Bit.SSC_STEP_SEL = uiSscStep;
        PLL_SETREG(PLL_PLL12_SSPLL0_REG_OFS, pll12sspl0Reg.Reg);
        // 4. Write 1 to SSC_NEW_MODE, DSSC
        pll12sspl0Reg.Bit.SSC_NEW_MODE = 1;
        pll12sspl0Reg.Bit.DSSC = 1;
        PLL_SETREG(PLL_PLL12_SSPLL0_REG_OFS, pll12sspl0Reg.Reg);
        // 5. Write 1 to SSC_EN
        pll12sspl1Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL1_REG_OFS);
        pll12sspl1Reg.Bit.SSC_EN = 1;
        PLL_SETREG(PLL_PLL12_SSPLL1_REG_OFS, pll12sspl1Reg.Reg);

	}

    // Leave critical section
    unl_multiCores();

    pll_getPLLSpreadSpectrum(id, &uiRealLow, &uiRealUpper);
    if (uiRealLow != uiLowerFreq)
    {
        DBG_WRN("PLL%d expect lower bound %d Hz, but real lower bound is %d Hz\r\n", id+1, uiLowerFreq, uiRealLow);
    }
    if (uiRealUpper != uiUpperFreq)
    {
        DBG_WRN("PLL%d expect upper bound %d Hz, but real upper bound is %d Hz\r\n", id+1, uiUpperFreq, uiRealUpper);
    }
    {
        uiMiddle = (uiRealLow+uiRealUpper) / 2;

        fPercent = uiRealUpper - uiRealLow;
        fPercent /= 2 * uiMiddle;
        if (fPercent > 0.03)
        {
            DBG_WRN("PLL%d percentage %f exceed 0.03\r\n", id+1, fPercent);
        }
    }

    return E_OK;
#endif
}

/**
    Get PLL Spread Spectrum mode range

    Get PLL to Spread Spectrum mode range

    @note Only PLL_ID_2 and PLL_ID_4 support spread spectrum

    @param[in] id               PLL ID
    @param[out] puiLowerFreq    lower bound frequency for spread spectrum (valid range: 426000000 ~ 480000000)
    @param[out] puiUpperFreq    upper bound frequency for spread spectrum (valid range: 426000000 ~ 480000000)

    @return
        - @b E_OK: success
        - @b E_NOSPT: PLL id not support spread spectrum
        - @b E_MACV: PLL id is NOT disabled before invoke this function
        - @b E_CTX: puiLowerFreq or puiUpperFreq is NULL
*/
ER pll_getPLLSpreadSpectrum(PLL_ID id, UINT32* puiLowerFreq, UINT32* puiUpperFreq)
{
#if _FPGA_EMULATION_
    DBG_ERR("NOT support in FPGA phase\r\n");
    return E_NOSPT;
#else
    UINT64 uiLowerFreq, uiUpperFreq;
    UINT32 uiPeriod, uiMulfac, uiStepSel;
    UINT32 uiSscEn = 0;
    UINT64 pllRatio;
    T_PLL_PLL2_CR0_REG reg0 = {0};
    T_PLL_PLL2_CR1_REG reg1 = {0};
    T_PLL_PLL2_CR2_REG reg2 = {0};

    if ((puiLowerFreq==NULL) || (puiUpperFreq==NULL))
    {
        return E_CTX;
    }

    // SSP% = SSC_MULFAC[7..0] * SSC_PERIOD_VALUE[7..0] / 2^(17+SSC_STEP_SEL[2..0])
    /*if (id == PLL_ID_3)
    {
        T_PLL_PLL3_CTRL0_REG pll3Ctrl0Reg;
        T_PLL_PLL3_CFG0_REG pll3Cfg0Reg;
        T_PLL_PLL3_CFG1_REG pll3Cfg1Reg = {0};
        T_PLL_PLL3_CFG2_REG pll3Cfg2Reg = {0};

        // 1. Get period value
        pll3Cfg2Reg.Reg = PLL_GETREG(PLL_PLL3_CFG2_REG_OFS);
        uiPeriod = pll3Cfg2Reg.Bit.SSC_PERIOD_VALUE;
        // 2. Get MULFAC
        pll3Cfg1Reg.Reg = PLL_GETREG(PLL_PLL3_CFG1_REG_OFS);
        uiMulfac = pll3Cfg1Reg.Bit.SSC_MULFAC;
        // 3. Get STEP_SEL
        pll3Cfg0Reg.Reg = PLL_GETREG(PLL_PLL3_CFG0_REG_OFS);
        uiStepSel = pll3Cfg0Reg.Bit.SSC_STEP_SEL;
        // 5. Get SSC_EN
        pll3Ctrl0Reg.Reg = PLL_GETREG(PLL_PLL3_CTRL0_REG_OFS);
        uiSscEn = pll3Ctrl0Reg.Bit.SSC_EN;

        reg0.Reg =PLL_GETREG(PLL_PLL3_CR0_REG_OFS);
        reg1.Reg =PLL_GETREG(PLL_PLL3_CR0_REG_OFS + 0x04);
        reg2.Reg =PLL_GETREG(PLL_PLL3_CR0_REG_OFS + 0x08);
        pllRatio = (reg2.Bit.PLL_RATIO2 << 16) | (reg1.Bit.PLL_RATIO1 << 8) | (reg0.Bit.PLL_RATIO0 << 0);

        uiUpperFreq = 12000000*pllRatio / 131072;
    }*/
    if (id == PLL_ID_2)
    {
        T_PLL_PLL2_SSPLL1_REG pll2sspl1Reg;
        T_PLL_PLL2_SSPLL2_REG pll2sspl2Reg = {0};
        T_PLL_PLL2_SSPLL3_REG pll2sspl3Reg = {0};
        T_PLL_PLL2_SSPLL0_REG pll2sspl0Reg;

        // 1. Get period value
        pll2sspl3Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL3_REG_OFS);
        uiPeriod = pll2sspl3Reg.Bit.SSC_PERIOD_VALUE;
        // 2. Setup MULFAC
        pll2sspl2Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL2_REG_OFS);
        uiMulfac = pll2sspl2Reg.Bit.SSC_MULFAC;
        // 3. Setup STEP_SEL
        pll2sspl0Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL0_REG_OFS);
        uiStepSel = pll2sspl0Reg.Bit.SSC_STEP_SEL;
        // 5. Write 1 to SSC_EN
        pll2sspl1Reg.Reg = PLL_GETREG(PLL_PLL2_SSPLL1_REG_OFS);
        uiSscEn = pll2sspl1Reg.Bit.SSC_EN;

        reg0.Reg =PLL_GETREG(PLL_PLL2_CR0_REG_OFS);
        reg1.Reg =PLL_GETREG(PLL_PLL2_CR0_REG_OFS + 0x04);
        reg2.Reg =PLL_GETREG(PLL_PLL2_CR0_REG_OFS + 0x08);
        pllRatio = (reg2.Bit.PLL_RATIO2 << 16) | (reg1.Bit.PLL_RATIO1 << 8) | (reg0.Bit.PLL_RATIO0 << 0);

        uiUpperFreq = 12000000*pllRatio / 131072;

    }
    else if (id == PLL_ID_4)
    {
        T_PLL_PLL4_SSPLL1_REG pll4sspl1Reg;
        T_PLL_PLL4_SSPLL2_REG pll4sspl2Reg = {0};
        T_PLL_PLL4_SSPLL3_REG pll4sspl3Reg = {0};
        T_PLL_PLL4_SSPLL0_REG pll4sspl0Reg;

        // 1. Get period value
        pll4sspl3Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL3_REG_OFS);
        uiPeriod = pll4sspl3Reg.Bit.SSC_PERIOD_VALUE;
        // 2. Setup MULFAC
        pll4sspl2Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL2_REG_OFS);
        uiMulfac = pll4sspl2Reg.Bit.SSC_MULFAC;
        // 3. Setup STEP_SEL
        pll4sspl0Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL0_REG_OFS);
        uiStepSel = pll4sspl0Reg.Bit.SSC_STEP_SEL;
        // 5. Write 1 to SSC_EN
        pll4sspl1Reg.Reg = PLL_GETREG(PLL_PLL4_SSPLL1_REG_OFS);
        uiSscEn = pll4sspl1Reg.Bit.SSC_EN;

        reg0.Reg =PLL_GETREG(PLL_PLL4_CR0_REG_OFS);
        reg1.Reg =PLL_GETREG(PLL_PLL4_CR0_REG_OFS + 0x04);
        reg2.Reg =PLL_GETREG(PLL_PLL4_CR0_REG_OFS + 0x08);
        pllRatio = (reg2.Bit.PLL_RATIO2 << 16) | (reg1.Bit.PLL_RATIO1 << 8) | (reg0.Bit.PLL_RATIO0 << 0);

        uiUpperFreq = 12000000*pllRatio / 131072;
    }
    else if (id == PLL_ID_12)
    {
        T_PLL_PLL12_SSPLL1_REG pll12sspl1Reg;
        T_PLL_PLL12_SSPLL2_REG pll12sspl2Reg = {0};
        T_PLL_PLL12_SSPLL3_REG pll12sspl3Reg = {0};
        T_PLL_PLL12_SSPLL0_REG pll12sspl0Reg;

        // 1. Get period value
        pll12sspl3Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL3_REG_OFS);
        uiPeriod = pll12sspl3Reg.Bit.SSC_PERIOD_VALUE;
        // 2. Setup MULFAC
        pll12sspl2Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL2_REG_OFS);
        uiMulfac = pll12sspl2Reg.Bit.SSC_MULFAC;
        // 3. Setup STEP_SEL
        pll12sspl0Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL0_REG_OFS);
        uiStepSel = pll12sspl0Reg.Bit.SSC_STEP_SEL;
        // 5. Write 1 to SSC_EN
        pll12sspl1Reg.Reg = PLL_GETREG(PLL_PLL12_SSPLL1_REG_OFS);
        uiSscEn = pll12sspl1Reg.Bit.SSC_EN;

        reg0.Reg =PLL_GETREG(PLL_PLL12_CR0_REG_OFS);
        reg1.Reg =PLL_GETREG(PLL_PLL12_CR0_REG_OFS + 0x04);
        reg2.Reg =PLL_GETREG(PLL_PLL12_CR0_REG_OFS + 0x08);
        pllRatio = (reg2.Bit.PLL_RATIO2 << 16) | (reg1.Bit.PLL_RATIO1 << 8) | (reg0.Bit.PLL_RATIO0 << 0);

        uiUpperFreq = 12000000*pllRatio / 131072;
    }
    else
    {
        return E_NOSPT;
    }

//    uiUpperFreq = pll_getPLLFreq(id);

    if (uiSscEn == FALSE)
    {
        uiLowerFreq = uiUpperFreq;
    }
    else
    {
        uiLowerFreq = ((uiUpperFreq << (17+uiStepSel)) - (uiUpperFreq*uiMulfac*uiPeriod)) >> (17+uiStepSel);
    }

    *puiLowerFreq = (UINT32)uiLowerFreq;
    *puiUpperFreq = (UINT32)uiUpperFreq;

    return E_OK;
#endif
}

/*
    Get bit mask for specific module clock setting

    Get bit mask for specific module clock setting

    @param[in] uiNum    Specific module

    @return bit mask for specific module clock
*/
static UINT32 pll_getClockMask(UINT32 uiNum)
{
    switch (uiNum)
    {
    case PLL_CLKSEL_CPU:
        return PLL_CLKSEL_CPU_MASK;

    /*case PLL_CLKSEL_CPU2:
        return PLL_CLKSEL_CPU2_MASK;*/

    /*case PLL_CLKSEL_DSP:
        return PLL_CLKSEL_DSP_MASK;*/
    /*case PLL_CLKSEL_OCP:
        return PLL_CLKSEL_OCP_MASK;*/

    case PLL_CLKSEL_DSP:
        return PLL_CLKSEL_DSP_MASK;

    case PLL_CLKSEL_DSP_IOP:
        return PLL_CLKSEL_DSP_IOP_MASK;

    case PLL_CLKSEL_DSP_EDP:
        return PLL_CLKSEL_DSP_EDP_MASK;

    case PLL_CLKSEL_DSP_EPP:
        return PLL_CLKSEL_DSP_EPP_MASK;

    case PLL_CLKSEL_DSP_EDAP:
        return PLL_CLKSEL_DSP_EDAP_MASK;

    case PLL_CLKSEL_DSP2:
        return PLL_CLKSEL_DSP2_MASK;

    case PLL_CLKSEL_DSP2_IOP:
        return PLL_CLKSEL_DSP2_IOP_MASK;

    case PLL_CLKSEL_DSP2_EDP:
        return PLL_CLKSEL_DSP2_EDP_MASK;

    case PLL_CLKSEL_DSP2_EPP:
        return PLL_CLKSEL_DSP2_EPP_MASK;

    case PLL_CLKSEL_DSP2_EDAP:
        return PLL_CLKSEL_DSP2_EDAP_MASK;


    case PLL_CLKSEL_APB:
        return PLL_CLKSEL_APB_MASK;

    /*case PLL_CLKSEL_PRE:
        return PLL_CLKSEL_PRE_MASK;*/

    case PLL_CLKSEL_IPE:
        return PLL_CLKSEL_IPE_MASK;

    case PLL_CLKSEL_DIS:
        return PLL_CLKSEL_DIS_MASK;

    case PLL_CLKSEL_IME:
        return PLL_CLKSEL_IME_MASK;

    case PLL_CLKSEL_FDE:
        return PLL_CLKSEL_FDE_MASK;

    case PLL_CLKSEL_IVE:
        return PLL_CLKSEL_IVE_MASK;

    case PLL_CLKSEL_ISE:
        return PLL_CLKSEL_ISE_MASK;

    case PLL_CLKSEL_DCE:
        return PLL_CLKSEL_DCE_MASK;

    case PLL_CLKSEL_IFE:
        return PLL_CLKSEL_IFE_MASK;

    case PLL_CLKSEL_IFE2:
        return PLL_CLKSEL_IFE2_MASK;

    case PLL_CLKSEL_SVM:
        return PLL_CLKSEL_SVM_MASK;

    case PLL_CLKSEL_SIE_MCLKSRC:
        return PLL_CLKSEL_SIE_MCLKSRC_MASK;

    case PLL_CLKSEL_RHE:
        return PLL_CLKSEL_RHE_MASK;

    case PLL_CLKSEL_CNN:
        return PLL_CLKSEL_CNN_MASK;

    case PLL_CLKSEL_ISE2:
        return PLL_CLKSEL_ISE2_MASK;

    case PLL_CLKSEL_SIE3_PXCLKSRC:
        return PLL_CLKSEL_SIE3_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE_MCLK2SRC:
        return PLL_CLKSEL_SIE_MCLK2SRC_MASK;

    case PLL_CLKSEL_SIE_PXCLKSRC:
        return PLL_CLKSEL_SIE_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE2_PXCLKSRC:
        return PLL_CLKSEL_SIE2_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE_CLKSRC:
        return PLL_CLKSEL_SIE_CLKSRC_MASK;

    case PLL_CLKSEL_SIE2_CLKSRC:
        return PLL_CLKSEL_SIE2_CLKSRC_MASK;

    case PLL_CLKSEL_SIE3_CLKSRC:
        return PLL_CLKSEL_SIE3_CLKSRC_MASK;

    case PLL_CLKSEL_SIE4_CLKSRC:
        return PLL_CLKSEL_SIE4_CLKSRC_MASK;

    case PLL_CLKSEL_SIE_MCLKINV:
        return PLL_CLKSEL_SIE_MCLKINV_MASK;

    case PLL_CLKSEL_RDE:
        return PLL_CLKSEL_RDE_MASK;

    case PLL_CLKSEL_TGE:
        return PLL_CLKSEL_TGE_MASK;

    case PLL_CLKSEL_TGE2:
        return PLL_CLKSEL_TGE2_MASK;

    case PLL_CLKSEL_SIE2_MCLKINV:
        return PLL_CLKSEL_SIE2_MCLKINV_MASK;

    case PLL_CLKSEL_RHE_CLK2:
        return PLL_CLKSEL_RHE_CLK2_MASK;

    case PLL_CLKSEL_DRE:
        return PLL_CLKSEL_DRE_MASK;

    case PLL_CLKSEL_SIE4_PXCLKSRC:
        return PLL_CLKSEL_SIE4_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE5_PXCLKSRC:
        return PLL_CLKSEL_SIE5_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE6_PXCLKSRC:
        return PLL_CLKSEL_SIE6_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE7_PXCLKSRC:
        return PLL_CLKSEL_SIE7_PXCLKSRC_MASK;

    case PLL_CLKSEL_SIE8_PXCLKSRC:
        return PLL_CLKSEL_SIE8_PXCLKSRC_MASK;

    case PLL_CLKSEL_JPEG:
        return PLL_CLKSEL_JPEG_MASK;

    case PLL_CLKSEL_H26X:
        return PLL_CLKSEL_H26X_MASK;

    case PLL_CLKSEL_GRAPHIC:
        return PLL_CLKSEL_GRAPHIC_MASK;

    case PLL_CLKSEL_GRAPHIC2:
        return PLL_CLKSEL_GRAPHIC2_MASK;

    case PLL_CLKSEL_AFFINE:
        return PLL_CLKSEL_AFFINE_MASK;

    case PLL_CLKSEL_HWCPY:
        return PLL_CLKSEL_HWCPY_MASK;

    case PLL_CLKSEL_ROTATE:
        return PLL_CLKSEL_ROTATE_MASK;

    case PLL_CLKSEL_CRYPTO:
        return PLL_CLKSEL_CRYPTO_MASK;

    /*case PLL_CLKSEL_VX1SL:
        return PLL_CLKSEL_VX1SL_MASK;*/

    case PLL_CLKSEL_NAND_CLKDIV:
        return PLL_CLKSEL_NAND_MASK;

    case PLL_CLKSEL_SDIO:
        return PLL_CLKSEL_SDIO_MASK;

    case PLL_CLKSEL_SDIO2:
        return PLL_CLKSEL_SDIO2_MASK;

    case PLL_CLKSEL_SDIO3:
        return PLL_CLKSEL_SDIO3_MASK;

    case PLL_CLKSEL_TSMUX:
        return PLL_CLKSEL_TSMUX_MASK;

    case PLL_CLKSEL_TSDEMUX:
        return PLL_CLKSEL_TSDEMUX_MASK;

    case PLL_CLKSEL_MIPI_LVDS:
        return PLL_CLKSEL_MIPI_LVDS_MASK;

    case PLL_CLKSEL_MIPI_LVDS2:
        return PLL_CLKSEL_MIPI_LVDS2_MASK;

    case PLL_CLKSEL_MIPI_LVDS3:
        return PLL_CLKSEL_MIPI_LVDS3_MASK;

    case PLL_CLKSEL_MIPI_LVDS4:
        return PLL_CLKSEL_MIPI_LVDS4_MASK;

    case PLL_CLKSEL_MIPI_LVDS5:
        return PLL_CLKSEL_MIPI_LVDS5_MASK;

    case PLL_CLKSEL_MIPI_LVDS6:
        return PLL_CLKSEL_MIPI_LVDS6_MASK;

    case PLL_CLKSEL_MIPI_LVDS7:
        return PLL_CLKSEL_MIPI_LVDS7_MASK;

    case PLL_CLKSEL_MIPI_LVDS8:
        return PLL_CLKSEL_MIPI_LVDS8_MASK;

    case PLL_CLKSEL_LVDS_CLKPHASE:
        return PLL_CLKSEL_LVDS_CLKPHASE_MASK;

    case PLL_CLKSEL_LVDS2_CLKPHASE:
        return PLL_CLKSEL_LVDS2_CLKPHASE_MASK;

    /*case PLL_CLKSEL_DSI_CLKSRC:
        return PLL_CLKSEL_DSI_CLKSRC_MASK;*/

    case PLL_CLKSEL_IDE_CLKSRC:
        return PLL_CLKSEL_IDE_CLKSRC_MASK;

    case PLL_CLKSEL_IDE2_CLKSRC:
        return PLL_CLKSEL_IDE2_CLKSRC_MASK;

    case PLL_CLKSEL_DSI_LPSRC:
        return PLL_CLKSEL_DSI_LPSRC_MASK;

    case PLL_CLKSEL_MI_CLKSRC:
        return PLL_CLKSEL_MI_CLKSRC_MASK;

    /*case PLL_CLKSEL_ADO_CLKMUX:
        return PLL_CLKSEL_ADO_CLKMUX_MASK;*/

    case PLL_CLKSEL_HDMI_ADO_CLKMUX:
        return PLL_CLKSEL_HDMI_ADO_CLKMUX_MASK;

    case PLL_CLKSEL_ADO_CLKSEL:
        return PLL_CLKSEL_ADO_CLKSEL_MASK;

    case PLL_CLKSEL_HDMI_ADO_CLKSEL:
        return PLL_CLKSEL_HDMI_ADO_CLKSEL_MASK;

    case PLL_CLKSEL_SIE_MCLKDIV:
        return PLL_CLKSEL_SIE_MCLKDIV_MASK;

    case PLL_CLKSEL_SIE_MCLK2DIV:
        return PLL_CLKSEL_SIE_MCLK2DIV_MASK;

    case PLL_CLKSEL_SIE_CLKDIV:
        return PLL_CLKSEL_SIE_CLKDIV_MASK;

    case PLL_CLKSEL_SIE2_CLKDIV:
        return PLL_CLKSEL_SIE2_CLKDIV_MASK;

    case PLL_CLKSEL_SIE3_CLKDIV:
        return PLL_CLKSEL_SIE3_CLKDIV_MASK;

    case PLL_CLKSEL_SIE4_CLKDIV:
        return PLL_CLKSEL_SIE4_CLKDIV_MASK;

    case PLL_CLKSEL_SIE5_CLKDIV:
        return PLL_CLKSEL_SIE5_CLKDIV_MASK;

    case PLL_CLKSEL_SIE6_CLKDIV:
        return PLL_CLKSEL_SIE6_CLKDIV_MASK;

    case PLL_CLKSEL_SIE7_CLKDIV:
        return PLL_CLKSEL_SIE7_CLKDIV_MASK;

    case PLL_CLKSEL_SIE8_CLKDIV:
        return PLL_CLKSEL_SIE8_CLKDIV_MASK;

    case PLL_CLKSEL_IDE_CLKDIV:
        return PLL_CLKSEL_IDE_CLKDIV_MASK;

    case PLL_CLKSEL_IDE2_CLKDIV:
        return PLL_CLKSEL_IDE2_CLKDIV_MASK;
    case PLL_CLKSEL_IDE_OUTIF_CLKDIV:
        return PLL_CLKSEL_IDE_OUTIF_CLKDIV_MASK;

    case PLL_CLKSEL_IDE2_OUTIF_CLKDIV:
        return PLL_CLKSEL_IDE2_OUTIF_CLKDIV_MASK;

    case PLL_CLKSEL_MI_CLKDIV:
        return PLL_CLKSEL_MI_CLKDIV_MASK;

    /*case PLL_CLKSEL_DSI_CLKDIV:
        return PLL_CLKSEL_DSI_CLKDIV_MASK;*/

    case PLL_CLKSEL_SP_CLKDIV:
        return PLL_CLKSEL_SP_CLKDIV_MASK;

    case PLL_CLKSEL_CEC_CLKDIV:
        return PLL_CLKSEL_CEC_CLKDIV_MASK;

    case PLL_CLKSEL_ADO_CLKDIV:
        return PLL_CLKSEL_ADO_CLKDIV_MASK;

    case PLL_CLKSEL_HDMI_ADO_CLKDIV:
        return PLL_CLKSEL_HDMI_ADO_CLKDIV_MASK;

    case PLL_CLKSEL_SDIO_CLKDIV:
        return PLL_CLKSEL_SDIO_CLKDIV_MASK;

    case PLL_CLKSEL_SDIO2_CLKDIV:
        return PLL_CLKSEL_SDIO2_CLKDIV_MASK;

    case PLL_CLKSEL_SDIO3_CLKDIV:
        return PLL_CLKSEL_SDIO3_CLKDIV_MASK;

    case PLL_CLKSEL_SPI_CLKDIV:
        return PLL_CLKSEL_SPI_CLKDIV_MASK;

    case PLL_CLKSEL_SPI2_CLKDIV:
        return PLL_CLKSEL_SPI2_CLKDIV_MASK;

    case PLL_CLKSEL_SPI3_CLKDIV:
        return PLL_CLKSEL_SPI3_CLKDIV_MASK;

    case PLL_CLKSEL_SPI4_CLKDIV:
        return PLL_CLKSEL_SPI4_CLKDIV_MASK;

    /*case PLL_CLKSEL_SPI5_CLKDIV:
        return PLL_CLKSEL_SPI5_CLKDIV_MASK;*/

    case PLL_CLKSEL_SP:
        return PLL_CLKSEL_SP_MASK;

    case PLL_CLKSEL_SP2:
        return PLL_CLKSEL_SP2_MASK;

    case PLL_CLKSEL_ETH_REFCLK_INV:
        return PLL_CLKSEL_ETH_REFCLK_INV_MASK;

    case PLL_CLKSEL_SLVSEC_D0CK_INV:
        return PLL_CLKSEL_SLVSEC_D0CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D1CK_INV:
        return PLL_CLKSEL_SLVSEC_D1CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D2CK_INV:
        return PLL_CLKSEL_SLVSEC_D2CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D3CK_INV:
        return PLL_CLKSEL_SLVSEC_D3CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D4CK_INV:
        return PLL_CLKSEL_SLVSEC_D4CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D5CK_INV:
        return PLL_CLKSEL_SLVSEC_D5CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D6CK_INV:
        return PLL_CLKSEL_SLVSEC_D6CK_INV_MASK;
    case PLL_CLKSEL_SLVSEC_D7CK_INV:
        return PLL_CLKSEL_SLVSEC_D7CK_INV_MASK;
    case PLL_CLKSEL_U3PCLK_INV:
        return PLL_CLKSEL_U3PCLK_INV_MASK;
    case PLL_CLKSEL_U3UCLK_INV:
        return PLL_CLKSEL_U3UCLK_INV_MASK;
    case PLL_CLKSEL_U2UCLK_INV:
        return PLL_CLKSEL_U2UCLK_INV_MASK;
    case PLL_CLKSEL_U3REFCLK_SEL:
        return PLL_CLKSEL_U3REFCLK_SEL_MASK;

    case PLL_CLKSEL_SIE_IO_PXCLK:
        return PLL_CLKSEL_SIE_IO_PXCLK_MASK;

    case PLL_CLKSEL_SIE2_IO_PXCLK:
        return PLL_CLKSEL_SIE2_IO_PXCLK_MASK;

    case PLL_CLKSEL_SIE4_IO_PXCLK:
        return PLL_CLKSEL_SIE4_IO_PXCLK_MASK;

    case PLL_CLKSEL_SIE5_IO_PXCLK:
        return PLL_CLKSEL_SIE5_IO_PXCLK_MASK;

    case PLL_CLKSEL_SIE7_IO_PXCLK:
        return PLL_CLKSEL_SIE7_IO_PXCLK_MASK;

    case PLL_CLKSEL_SIE8_IO_PXCLK:
        return PLL_CLKSEL_SIE8_IO_PXCLK_MASK;

    case PLL_CLKSEL_LPD0_LPCLK:
		return PLL_CLKSEL_CSILPD0_MASK;

    case PLL_CLKSEL_LPD1_LPCLK:
		return PLL_CLKSEL_CSILPD1_MASK;

    case PLL_CLKSEL_LPD2_LPCLK:
		return PLL_CLKSEL_CSILPD2_MASK;

    case PLL_CLKSEL_LPD3_LPCLK:
		return PLL_CLKSEL_CSILPD3_MASK;

    case PLL_CLKSEL_LPD4_LPCLK:
		return PLL_CLKSEL_CSILPD4_MASK;

    case PLL_CLKSEL_LPD5_LPCLK:
		return PLL_CLKSEL_CSILPD5_MASK;

    case PLL_CLKSEL_LPD6_LPCLK:
		return PLL_CLKSEL_CSILPD6_MASK;

    case PLL_CLKSEL_LPD7_LPCLK:
		return PLL_CLKSEL_CSILPD7_MASK;

	case PLL_CLKSEL_CANBUS:
		return PLL_CLKSEL_CANBUS_MASK;

    default:
        DBG_ERR("Non-supported Clk Mask ID! (0x%x)\r\n", uiNum);
        return 0;
    }
}

/*
    Check module clock limitation

    Check input clock mux/select.
    If outputted frequency is out of spec, dump warning messsage to UART.

    @param[in] uiNum            Clock mux to check
    @param[in] uiValue          Clock rate of uiNum

    @return void
*/
static void pll_chkClkLimitation(PLL_CLKSEL uiNum, UINT32 uiValue)
{
    UINT32 uiFreq;

    switch(uiNum)
    {
    case PLL_CLKSEL_CPU:
        if (uiValue == PLL_CLKSEL_CPU_PLL8)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_8);
        }
        else
        {
            return;
        }
        if(uiFreq > 420000000)
        {
            DBG_WRN("CPU Clock frequency exceed 420MHz!\r\n");
        }
        return;

    /*case PLL_CLKSEL_OCP:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_CPU);

            switch(uiFreq)
            {
            case PLL_CLKSEL_CPU_PLL8:
                {
                    uiFreq = pll_getPLLFreq(PLL_ID_8);
                    if (uiValue == PLL_CLKSEL_OCP_CPUCLK_D2)
                    {
                        uiFreq /= 2;
                    }
                    if(uiFreq > 420000000)
                    {
                        DBG_WRN("OCP clock must not exceed 420Mhz!\r\n");
                    }
                }
                return;
            default:
                return;
            }
        }
        return;*/

    /*case PLL_CLKSEL_PRE:
        if (uiValue == PLL_CLKSEL_PRE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("PRE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;*/

    case PLL_CLKSEL_IPE:
        if (uiValue == PLL_CLKSEL_IPE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("IPE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_DIS:
        if (uiValue == PLL_CLKSEL_DIS_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("DIS Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_IME:
        if (uiValue == PLL_CLKSEL_IME_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("IME Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_FDE:
        if (uiValue == PLL_CLKSEL_FDE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("FDE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_ISE:
        if (uiValue == PLL_CLKSEL_FDE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("ISE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_DCE:
        if (uiValue == PLL_CLKSEL_DCE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("DCE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_IFE:
        if (uiValue == PLL_CLKSEL_IFE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("IFE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_IFE2:
        if (uiValue == PLL_CLKSEL_IFE2_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("IFE2 Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_RDE:
        if (uiValue == PLL_CLKSEL_RDE_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 300000000)
            {
                DBG_WRN("RDE Clock frequency exceed 300MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_H26X:
        if (uiValue == PLL_CLKSEL_H26X_PLL6)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
            if(uiFreq > 250000000)
            {
                DBG_WRN("H.264 Clock frequency exceed 250MHz!\r\n");
            }
        }
        else if (uiValue == PLL_CLKSEL_H26X_PLL13)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_13);
            if(uiFreq > 250000000)
            {
                DBG_WRN("H.264 Clock frequency exceed 250MHz!\r\n");
            }
        }
        return;

    /*case PLL_CLKSEL_NAND:
        if (uiValue == PLL_CLKSEL_NAND_PLL4)
        {
            uiFreq = pll_getPLLFreq(PLL_ID_6);
        }
        else
        {
            return;
        }
        if(uiFreq > 60000000)
        {
            DBG_WRN("NAND Clock frequency exceed 60MHz!\r\n");
        }
        return;*/

    case PLL_CLKSEL_SIE_MCLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SIE_MCLKSRC);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SIE_MCLKSRC_PLL5:
                uiFreq = pll_getPLLFreq(PLL_ID_5);
                break;

            default:
                return;

            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_SIE_MCLKDIV - PLL_CLKSEL_R8_OFFSET)) +1);
            if(uiFreq > 108000000)
            {
                DBG_WRN("SIE-Mclk clock must not exceed 108MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SIE_MCLK2DIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SIE_MCLK2SRC);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SIE_MCLK2SRC_PLL5:
                uiFreq = pll_getPLLFreq(PLL_ID_5);
                break;

            default:
                return;

            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_SIE_MCLK2DIV - PLL_CLKSEL_R8_OFFSET)) +1);
            if(uiFreq > 108000000)
            {
                DBG_WRN("SIE-Mclk2 clock must not exceed 108MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SIE_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SIE_CLKSRC);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SIE_CLKSRC_480:
                uiFreq = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_SIE_CLKSRC_PLL6:
                uiFreq = pll_getPLLFreq(PLL_ID_6);
                break;
            case PLL_CLKSEL_SIE_CLKSRC_PLL5:
                uiFreq = pll_getPLLFreq(PLL_ID_5);
                break;
            /*case PLL_CLKSEL_SIE_CLKSRC_PLL7:
                uiFreq = pll_getPLLFreq(PLL_ID_7);
                break;*/

            default:
                return;
            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_SIE_CLKDIV - PLL_CLKSEL_R8_OFFSET)) +1);
            if(uiFreq > 297000000)
            {
                DBG_WRN("SIE-clk clock must not exceed 297MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SIE2_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SIE2_CLKSRC);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SIE2_CLKSRC_480:
                uiFreq = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_SIE2_CLKSRC_PLL6:
                uiFreq = pll_getPLLFreq(PLL_ID_6);
                break;
            case PLL_CLKSEL_SIE2_CLKSRC_PLL5:
                uiFreq = pll_getPLLFreq(PLL_ID_5);
                break;
            /*case PLL_CLKSEL_SIE2_CLKSRC_PLL7:
                uiFreq = pll_getPLLFreq(PLL_ID_7);
                break;*/

            default:
                return;
            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_SIE2_CLKDIV - PLL_CLKSEL_R8_OFFSET)) +1);
            if(uiFreq > 297000000)
            {
                DBG_WRN("SIE2-clk clock must not exceed 297MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_IDE_CLKDIV:
    case PLL_CLKSEL_IDE_OUTIF_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_IDE_CLKSRC);

            switch(uiFreq)
            {
            case PLL_CLKSEL_IDE_CLKSRC_480:
                uiFreq = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_IDE_CLKSRC_PLL2:
                uiFreq = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_IDE_CLKSRC_PLL4:
                uiFreq = pll_getPLLFreq(PLL_ID_4);
                break;
            /*case PLL_CLKSEL_IDE_CLKSRC_PLL7:
                uiFreq = pll_getPLLFreq(PLL_ID_7);
                break;*/

            default:
                return;
            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_IDE_CLKDIV - PLL_CLKSEL_R23_OFFSET)) +1);
            if(uiFreq > 297000000)
            {
                DBG_WRN("IDE-1 clock must not exceed 297Mhz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_IDE2_CLKDIV:
    case PLL_CLKSEL_IDE2_OUTIF_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_IDE2_CLKSRC);

            switch(uiFreq)
            {
            case PLL_CLKSEL_IDE2_CLKSRC_480:
                uiFreq = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_IDE2_CLKSRC_PLL2:
                uiFreq = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_IDE2_CLKSRC_PLL4:
                uiFreq = pll_getPLLFreq(PLL_ID_4);
                break;
            /*case PLL_CLKSEL_IDE2_CLKSRC_PLL7:
                uiFreq = pll_getPLLFreq(PLL_ID_7);
                break;*/

            default:
                return;
            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_IDE2_CLKDIV - PLL_CLKSEL_R23_OFFSET)) +1);
            if(uiFreq > 297000000)
            {
                DBG_WRN("IDE-2 clock must not exceed 297Mhz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_MI_CLKDIV:
        {
#if _FPGA_EMULATION_
            uiFreq = PLL_FPGA_480SRC / ( (uiValue>>(PLL_CLKSEL_MI_CLKDIV - PLL_CLKSEL_R10_OFFSET)) +1);
#else
            uiFreq = 240000000 / ( (uiValue>>(PLL_CLKSEL_MI_CLKDIV - PLL_CLKSEL_R10_OFFSET)) +1);
#endif
            if(uiFreq > 80000000)
            {
                DBG_WRN("MI clock must not exceed 80Mhz!\r\n");
            }
        }
        return;

    /*case PLL_CLKSEL_DSI_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_DSI_CLKSRC);
            switch (uiFreq)
            {
            case PLL_CLKSEL_DSI_CLKSRC_480:
                uiFreq = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_DSI_CLKSRC_PLL2:
                uiFreq = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_DSI_CLKSRC_PLL4:
                uiFreq = pll_getPLLFreq(PLL_ID_4);
                break;
            case PLL_CLKSEL_DSI_CLKSRC_PLL7:
                uiFreq = pll_getPLLFreq(PLL_ID_7);
                break;

            default:
                return;
            }

            uiFreq = uiFreq / ( (uiValue>>(PLL_CLKSEL_DSI_CLKDIV - PLL_CLKSEL_R23_OFFSET)) +1);  // PHY freq
            uiFreq /= 8;    // module freq
            if(uiFreq > 80000000)
            {
                DBG_WRN("DSI clock must not exceed 60Mhz!\r\n");
            }
        }
        return;*/

    case PLL_CLKSEL_SP_CLKDIV:
        {
#if _FPGA_EMULATION_
            uiFreq = PLL_FPGA_480SRC / ( (uiValue>>(PLL_CLKSEL_SP_CLKDIV - PLL_CLKSEL_R11_OFFSET)) +1);
#else
            uiFreq = 240000000/ ( (uiValue>>(PLL_CLKSEL_SP_CLKDIV - PLL_CLKSEL_R11_OFFSET)) +1);
#endif
            if(uiFreq > 80000000)
            {
                DBG_WRN("SP clock must not exceed 80MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_ADO_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_7) / ( (uiValue>>(PLL_CLKSEL_ADO_CLKDIV - PLL_CLKSEL_R11_OFFSET)) +1);
            if(uiFreq > 25000000)
            {
                DBG_WRN("ADO clock must not exceed 25MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_HDMI_ADO_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_7) / ( (uiValue>>(PLL_CLKSEL_HDMI_ADO_CLKDIV - PLL_CLKSEL_R11_OFFSET)) +1);
            if(uiFreq > 25000000)
            {
                DBG_WRN("HDMI-ADO clock must not exceed 25MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SDIO_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SDIO);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SDIO_192:
                uiFreq = 192000000;
                break;
            case PLL_CLKSEL_SDIO_480:
                uiFreq = 480000000;
                break;
            case PLL_CLKSEL_SDIO_PLL2:
                uiFreq = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SDIO_PLL4:
                uiFreq = pll_getPLLFreq(PLL_ID_4);
                break;
            default:
                return;
            }

            uiFreq = uiFreq / ( (uiValue>>(PLL_CLKSEL_SDIO_CLKDIV - PLL_CLKSEL_R12_OFFSET)) +1);
            if(uiFreq > 208000000)
            {
                DBG_WRN("SDIO clock must not exceed 208MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SDIO2_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SDIO2);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SDIO2_192:
                uiFreq = 192000000;
                break;
            case PLL_CLKSEL_SDIO2_480:
                uiFreq = 480000000;
                break;
            case PLL_CLKSEL_SDIO2_PLL2:
                uiFreq = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SDIO2_PLL4:
                uiFreq = pll_getPLLFreq(PLL_ID_4);
                break;

            default:
                return;
            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_SDIO2_CLKDIV - PLL_CLKSEL_R12_OFFSET)) +1);
            if(uiFreq > 208000000)
            {
                DBG_WRN("SDIO2 clock must not exceed 208MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SDIO3_CLKDIV:
        {
            uiFreq = pll_getClockRate(PLL_CLKSEL_SDIO3);

            switch(uiFreq)
            {
            case PLL_CLKSEL_SDIO3_192:
                uiFreq = 192000000;
                break;
            case PLL_CLKSEL_SDIO3_480:
                uiFreq = 480000000;
                break;
            case PLL_CLKSEL_SDIO3_PLL2:
                uiFreq = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SDIO3_PLL4:
                uiFreq = pll_getPLLFreq(PLL_ID_4);
                break;

            default:
                return;
            }

            uiFreq = uiFreq /( (uiValue>>(PLL_CLKSEL_SDIO3_CLKDIV - PLL_CLKSEL_R13_OFFSET)) +1);
            if(uiFreq > 208000000)
            {
                DBG_WRN("SDIO3 clock must not exceed 208MHz!\r\n");
            }
        }
        return;


    case PLL_CLKSEL_SPI_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_1) / ( (uiValue>>(PLL_CLKSEL_SPI_CLKDIV - PLL_CLKSEL_R14_OFFSET)) +1);
            if(uiFreq > 96000000)
            {
                DBG_WRN("SPI clock must not exceed 96MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SPI2_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_1) / ( (uiValue>>(PLL_CLKSEL_SPI2_CLKDIV - PLL_CLKSEL_R14_OFFSET)) +1);
            if(uiFreq > 96000000)
            {
                DBG_WRN("SPI2 clock must not exceed 96MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SPI3_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_1) / ( (uiValue>>(PLL_CLKSEL_SPI3_CLKDIV - PLL_CLKSEL_R15_OFFSET)) +1);
            if(uiFreq > 96000000)
            {
                DBG_WRN("SPI3 clock must not exceed 96MHz!\r\n");
            }
        }
        return;

    case PLL_CLKSEL_SPI4_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_1) / ( (uiValue>>(PLL_CLKSEL_SPI4_CLKDIV - PLL_CLKSEL_R15_OFFSET)) +1);
            if(uiFreq > 96000000)
            {
                DBG_WRN("SPI3 clock must not exceed 96MHz!\r\n");
            }
        }
        return;
    /*case PLL_CLKSEL_SPI5_CLKDIV:
        {
            uiFreq = pll_getPLLFreq(PLL_ID_1) / ( (uiValue>>(PLL_CLKSEL_SPI5_CLKDIV - PLL_CLKSEL_R16_OFFSET)) +1);
            if(uiFreq > 96000000)
            {
                DBG_WRN("SPI3 clock must not exceed 96MHz!\r\n");
            }
        }
        return;*/

    default:
        return;
    }



}


/**
    Set module clock rate

    Set module clock rate, one module at a time.

    @param[in] uiNum      Module ID(PLL_CLKSEL_*), one module at a time.
                          Please refer to pll.h
    @param[in] uiValue    Moudle clock rate(PLL_CLKSEL_*_*), please refer to pll.h

    @return void
*/
void pll_setClockRate(PLL_CLKSEL uiNum, UINT32 uiValue)
{
    REGVALUE RegData;
    UINT32 uiMask,uiRegOffset;

    if((uiNum == PLL_CLKSEL_SIE_MCLKSRC) && (uiValue == PLL_CLKSEL_SIE_MCLKSRC_PLL2_D2))
    {
        DBG_ERR("SIE MCLKSRC not support PLL2/2!\r\n");
        return;
    }

    // Check if uiNum/uiValue exceeds limitation of NT96650
    pll_chkClkLimitation(uiNum, uiValue);

    uiMask = pll_getClockMask(uiNum);
    uiRegOffset = (uiNum >> 5) << 2;

    //race condition protect. enter critical section
    loc_multiCores();

    RegData = PLL_GETREG(PLL_SYS_CR_REG_OFS + uiRegOffset);
    RegData &= ~uiMask;
    RegData |= uiValue;
    PLL_SETREG(PLL_SYS_CR_REG_OFS + uiRegOffset, RegData);

    //race condition protect. leave critical section
    unl_multiCores();
}

/**
    Get module clock rate

    Get module clock rate, one module at a time.

    @param[in] uiNum      Module ID(PLL_CLKSEL_*), one module at a time.
                          Please refer to pll.h

    @return Moudle clock rate(PLL_CLKSEL_*_*), please refer to pll.h
*/
UINT32 pll_getClockRate(PLL_CLKSEL uiNum)
{
    UINT32      uiMask, uiRegOffset;
    REGVALUE    RegData;

    uiMask = pll_getClockMask(uiNum);
    uiRegOffset = (uiNum >> 5) << 2;

    RegData = PLL_GETREG(PLL_SYS_CR_REG_OFS + uiRegOffset);
    RegData &= uiMask;

    return (UINT32)RegData;
}

/**
    Set PWM Clock Rate.

    Set PWM Clock Rate.

    @param[in] uiPWMNumber    PWM ID (vaid value: 0x00 ~ 0x13)
    @param[in] uiDiv          Divisor number (valid value: 0x0003 ~ 0x3FFF)

    @return void
*/
void pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv)
{
    REGVALUE    RegData;
    UINT32      uiBitShift;
    UINT32      uiRegOffset;

    if(uiPWMNumber > 0x13)
    {
        return;
    }

    if (uiPWMNumber < 12)
    {
        // PWM0~3, PWM4~7, PWM8~11
        uiPWMNumber /= 4;
    }
    else
    {
        // PWM12~ has standalone id
        uiPWMNumber -= 12 - 3;
    }

    if (uiDiv < 3)
    {
        DBG_WRN("div should >=3, but %d\r\n", uiDiv);
        uiDiv = 3;
    }

    uiRegOffset = (uiPWMNumber >> 1)<<2;
    uiBitShift  = (uiPWMNumber & 0x01) << 4;

    //race condition protect. enter critical section
    loc_multiCores();

    RegData = PLL_GETREG(PLL_PWMCR0_REG_OFS + uiRegOffset);
    RegData &= ~(0x3FFF << uiBitShift);
    RegData |= uiDiv << uiBitShift;

    PLL_SETREG(PLL_PWMCR0_REG_OFS + uiRegOffset, RegData);

    //race condition protect. leave critical section
    unl_multiCores();
}

/*
    Get PWM Clock Rate.

    Get PWM Clock Rate. (for Emulation)

    @param[in] uiPWMNumber      PWM ID (vaid value: 0x00 ~ 0x13)
    @param[out] puiDiv          Clock divider of specific PWM ID

    @return void
*/
void pll_getPWMClockRate(UINT32 uiPWMNumber, UINT32 *puiDiv)
{
    REGVALUE    RegData;
    UINT32      uiBitShift;
    UINT32      uiRegOffset;

    if(uiPWMNumber > 0x13)
        return;

    if (uiPWMNumber < 12)
    {
        // PWM0~3, PWM4~7, PWM8~11
        uiPWMNumber /= 4;
    }
    else
    {
        // PWM12~ has standalond id
        uiPWMNumber -= 12 - 3;
    }

    uiRegOffset = (uiPWMNumber >> 1)<<2;
    uiBitShift  = (uiPWMNumber & 0x01) << 4;

    RegData = PLL_GETREG(PLL_PWMCR0_REG_OFS + uiRegOffset);
    RegData >>= uiBitShift;
    RegData &= 0x3FFF;

    *puiDiv= RegData;
}
#endif
/**
    Enable module clock

    Enable module clock, module clock must be enabled that it could be work correctly
    @param[in] Num  Module enable ID, one module at a time

    @return void
*/
void pll_enableClock(CG_EN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    /*if (Num >= CPU2_CLK)
    {
        uiRegOffset = ((Num-CPU2_CLK) >> 5) << 2;

        //race condition protect. enter critical section
        loc_multiCores();

        RegData  = PLL_GETREG(PLL_CPU2_CLKEN_REG_OFS + uiRegOffset);

        RegData |= 1 << (Num & 0x1F);

        PLL_SETREG(PLL_CPU2_CLKEN_REG_OFS + uiRegOffset, RegData);

        //race condition protect. leave critical section
        unl_multiCores();
    }
    else*/
    {
        uiRegOffset = (Num >> 5) << 2;

        //race condition protect. enter critical section
        loc_multiCores();

        RegData  = PLL_GETREG(PLL_CLKEN0_REG_OFS + uiRegOffset);

        RegData |= 1 << (Num & 0x1F);

        PLL_SETREG(PLL_CLKEN0_REG_OFS + uiRegOffset, RegData);

        //race condition protect. leave critical section
        unl_multiCores();
    }
}
#if 0
/**
    Disable module clock

    Disable module clock

    @param[in] Num  Module enable ID, one module at a time

    @return void
*/
void pll_disableClock(CG_EN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    /*if (Num >= CPU2_CLK)
    {
        uiRegOffset = ((Num-CPU2_CLK) >> 5) << 2;

        //race condition protect. enter critical section
        loc_multiCores();

        RegData = PLL_GETREG(PLL_CPU2_CLKEN_REG_OFS + uiRegOffset);

        RegData &= ~(1 << (Num & 0x1F));

        PLL_SETREG(PLL_CPU2_CLKEN_REG_OFS + uiRegOffset, RegData);

        //race condition protect. leave critical section
        unl_multiCores();
    }
    else*/
    {
        uiRegOffset = (Num >> 5) << 2;

        //race condition protect. enter critical section
        loc_multiCores();

        RegData = PLL_GETREG(PLL_CLKEN0_REG_OFS + uiRegOffset);

        RegData &= ~(1 << (Num & 0x1F));

        PLL_SETREG(PLL_CLKEN0_REG_OFS + uiRegOffset, RegData);

        //race condition protect. leave critical section
        unl_multiCores();
    }
}

/**
    Check module clock

    Check module clock

    @param[in] Num    Module ID, one module at a time

    @return
        - @b TRUE:    Clock is enabled
        - @b FALSE:   Clock is disabled

*/
BOOL pll_isClockEnabled(CG_EN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    uiRegOffset = (Num >> 5) << 2;

    RegData = PLL_GETREG(PLL_CLKEN0_REG_OFS + uiRegOffset);
    RegData &= 1 << (Num & 0x1F);

    return (RegData != 0);
}

/**
    Set the module clock frequency

    This api setup the module clock frequency by chnaging module clock divider.
    If the module has multiple source clock choices, user must set the correct
    source clock before calling this API.
\n  If the target frequency can not well divided from source frequency,this api
    would output warning message.

    @param[in] ClkID    Module select ID, refer to structure PLL_CLKFREQ.
    @param[in] uiFreq   Target clock frequency. Unit in Hertz.

    @return
     - @b E_ID:     ClkID is not support in this API.
     - @b E_PAR:    Target frequency can not be divided with no remainder.
     - @b E_OK:     Done and success.
*/
ER pll_setClockFreq(PLL_CLKFREQ ClkID, UINT32 uiFreq)
{
    UINT32 SrcClk,Divider,ClkSrc;

    if(ClkID >= PLL_CLKFREQ_MAXNUM)
    {
        return E_ID;
    }

    // Get Src Clock Frequency
    switch(ClkID)
    {
    case SIEMCLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_MCLKSRC);
            if (ClkSrc == PLL_CLKSEL_SIE_MCLKSRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if (ClkSrc == PLL_CLKSEL_SIE_MCLKSRC_PLL5)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_5);
            }
            else
            {
                return E_PAR;
            }
        }
        break;
    case SIEMCLK2_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_MCLK2SRC);
            if (ClkSrc == PLL_CLKSEL_SIE_MCLK2SRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if (ClkSrc == PLL_CLKSEL_SIE_MCLK2SRC_PLL5)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_5);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case SIECLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_CLKSRC);
            switch (ClkSrc)
            {
            case PLL_CLKSEL_SIE_CLKSRC_480:
                SrcClk = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_SIE_CLKSRC_PLL2:
                SrcClk = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SIE_CLKSRC_PLL5:
                SrcClk = pll_getPLLFreq(PLL_ID_5);
                break;
            case PLL_CLKSEL_SIE_CLKSRC_PLL14:
                SrcClk = pll_getPLLFreq(PLL_ID_14);
                break;
            default:
                return E_PAR;
            }
        }
        break;

    case SIE2CLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE2_CLKSRC);
            switch (ClkSrc)
            {
            case PLL_CLKSEL_SIE2_CLKSRC_480:
                SrcClk = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_SIE2_CLKSRC_PLL2:
                SrcClk = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SIE2_CLKSRC_PLL5:
                SrcClk = pll_getPLLFreq(PLL_ID_5);
                break;
            case PLL_CLKSEL_SIE2_CLKSRC_PLL14:
                SrcClk = pll_getPLLFreq(PLL_ID_14);
                break;
            default:
                return E_PAR;
            }
        }
        break;

    case SIE3CLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE3_CLKSRC);
            switch (ClkSrc)
            {
            case PLL_CLKSEL_SIE3_CLKSRC_480:
                SrcClk = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_SIE3_CLKSRC_PLL2:
                SrcClk = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SIE3_CLKSRC_PLL5:
                SrcClk = pll_getPLLFreq(PLL_ID_5);
                break;
            case PLL_CLKSEL_SIE3_CLKSRC_PLL14:
                SrcClk = pll_getPLLFreq(PLL_ID_14);
                break;
            default:
                return E_PAR;
            }
        }
        break;

    case SIE4CLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE4_CLKSRC);
            switch (ClkSrc)
            {
            case PLL_CLKSEL_SIE4_CLKSRC_480:
                SrcClk = pll_getPLLFreq(PLL_ID_1);
                break;
            case PLL_CLKSEL_SIE4_CLKSRC_PLL2:
                SrcClk = pll_getPLLFreq(PLL_ID_2);
                break;
            case PLL_CLKSEL_SIE4_CLKSRC_PLL5:
                SrcClk = pll_getPLLFreq(PLL_ID_5);
                break;
            case PLL_CLKSEL_SIE4_CLKSRC_PLL14:
                SrcClk = pll_getPLLFreq(PLL_ID_14);
                break;
            default:
                return E_PAR;
            }
        }
        break;

    case SIE5CLK_FREQ:
    case SIE6CLK_FREQ:
    case SIE7CLK_FREQ:
    case SIE8CLK_FREQ:
        {
            SrcClk = pll_getPLLFreq(PLL_ID_1);
        }
        break;


    case IDECLK_FREQ:
    case IDEOUTIFCLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_IDE_CLKSRC);
            if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_PLL12)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_12);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case IDE2CLK_FREQ:
    case IDE2OUTIFCLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_IDE2_CLKSRC);
            if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_PLL12)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_12);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case MICLK_FREQ:
        {
#if _FPGA_EMULATION_
            SrcClk = PLL_FPGA_480SRC;
#else
            SrcClk = 240000000;
#endif
        }
        break;

    case DSICLK_FREQ:
        {
            /*ClkSrc = pll_getClockRate(PLL_CLKSEL_DSI_CLKSRC);
            if(ClkSrc == PLL_CLKSEL_DSI_CLKSRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if(ClkSrc == PLL_CLKSEL_DSI_CLKSRC_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if(ClkSrc == PLL_CLKSEL_DSI_CLKSRC_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else if(ClkSrc == PLL_CLKSEL_DSI_CLKSRC_PLL7)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_7);
            }
            else
            {
                return E_PAR;
            }*/
#if _FPGA_EMULATION_
            SrcClk = PLL_FPGA_480SRC;
#else
            SrcClk = pll_getPLLFreq(PLL_ID_11);;
#endif
        }
        break;

    case SPCLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SP);
            if (ClkSrc == PLL_CLKSEL_SP_240)
            {
#if _FPGA_EMULATION_
                SrcClk = PLL_FPGA_480SRC;
#else
                SrcClk = 240000000;
#endif
            }
            else if (ClkSrc == PLL_CLKSEL_SP_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if (ClkSrc == PLL_CLKSEL_SP_PLL5)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_5);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case SPCLK2_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SP2);
            if (ClkSrc == PLL_CLKSEL_SP2_240)
            {
#if _FPGA_EMULATION_
                SrcClk = PLL_FPGA_480SRC;
#else
                SrcClk = 240000000;
#endif
            }
            else if (ClkSrc == PLL_CLKSEL_SP2_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if (ClkSrc == PLL_CLKSEL_SP2_PLL5)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_5);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case CECCLK_FREQ:
        {
#if _FPGA_EMULATION_
            // HDMI CEC has different clk divider in FPGA phase
            SrcClk = (PLL_FPGA_480SRC*2) / 12;
#else
            SrcClk = 3000000;
#endif
        }
        break;

    case ADOCLK_FREQ:
    case HDMIADOCLK_FREQ:
        {
            SrcClk = pll_getPLLFreq(PLL_ID_7);
        }
        break;

    case SDIOCLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SDIO);
            if(ClkSrc == PLL_CLKSEL_SDIO_60)
            {
#if _FPGA_EMULATION_
                SrcClk = (PLL_FPGA_480SRC*2) / 8;
#else
                SrcClk = 60000000;
#endif
            }
            else if(ClkSrc == PLL_CLKSEL_SDIO_80)
            {
#if _FPGA_EMULATION_
                SrcClk = (PLL_FPGA_480SRC*2) / 6;
#else
                SrcClk = 80000000;
#endif
            }
            else if(ClkSrc == PLL_CLKSEL_SDIO_96)
            {
#if _FPGA_EMULATION_
                SrcClk = (PLL_FPGA_480SRC*2) / 5;
#else
                SrcClk = 96000000;
#endif
            }
            else if(ClkSrc == PLL_CLKSEL_SDIO_192) {
#if _FPGA_EMULATION_
                //#NT#2016/07/04#Steven Wang -begin
                //#NT#FPGA  | Real chip
                //  pll2*2  |   CLK480
                //  pll1/2  |   CLK192
                SrcClk = PLL_FPGA_192SRC;
                DBG_DUMP("SDIO src = 192 freq = %8d Hz\r\n", ClkSrc);
#else
                SrcClk = 192000000;
#endif
            } else if(ClkSrc == PLL_CLKSEL_SDIO_480) {
#if _FPGA_EMULATION_
                //#NT#2016/07/04#Steven Wang -begin
                //#NT#FPGA  | Real chip
                //  pll2*2  |   CLK480
                //  pll1/2  |   CLK192

                SrcClk = (PLL_FPGA_480SRC*2);
#else
                SrcClk = 480000000;
#endif
            }

            else if(ClkSrc == PLL_CLKSEL_SDIO_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case SDIO2CLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_SDIO2);
            if(ClkSrc == PLL_CLKSEL_SDIO2_60)
            {
#if _FPGA_EMULATION_
                SrcClk = (PLL_FPGA_480SRC*2) / 8;
#else
                SrcClk = 60000000;
#endif
            }
            else if(ClkSrc == PLL_CLKSEL_SDIO2_80)
            {
#if _FPGA_EMULATION_
                SrcClk = (PLL_FPGA_480SRC*2) / 6;
#else
                SrcClk = 80000000;
#endif
            }
            else if(ClkSrc == PLL_CLKSEL_SDIO2_96)
            {
#if _FPGA_EMULATION_
                SrcClk = (PLL_FPGA_480SRC*2) / 5;
#else
                SrcClk = 96000000;
#endif
            }
            else if(ClkSrc == PLL_CLKSEL_SDIO2_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case SPICLK_FREQ:
    case SPI2CLK_FREQ:
    case SPI3CLK_FREQ:
    case SPI4CLK_FREQ:
    case SPI5CLK_FREQ:
        SrcClk = pll_getPLLFreq(PLL_ID_1);
        break;

    default:
        break;
        //return E_PAR;
    }


    // Calculate the clock divider value
    Divider = (SrcClk + uiFreq-1) / uiFreq;

    // prevent error case
    if(Divider == 0)
    {
        Divider = 1;
    }
     // Set Clock Divider
    switch(ClkID)
    {
    case SIEMCLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(Divider-1));
        break;
    case SIEMCLK2_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE_MCLK2DIV, PLL_SIE_MCLK2DIV(Divider-1));
        break;
    case SIECLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE_CLKDIV, PLL_SIE_CLKDIV(Divider-1));
        break;
    case SIE2CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE2_CLKDIV, PLL_SIE2_CLKDIV(Divider-1));
        break;
    case SIE3CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE3_CLKDIV, PLL_SIE3_CLKDIV(Divider-1));
        break;
    case SIE4CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE4_CLKDIV, PLL_SIE4_CLKDIV(Divider-1));
        break;
    case SIE5CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE5_CLKDIV, PLL_SIE5_CLKDIV(Divider-1));
        break;
    case SIE6CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE6_CLKDIV, PLL_SIE6_CLKDIV(Divider-1));
        break;
    case SIE7CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE7_CLKDIV, PLL_SIE7_CLKDIV(Divider-1));
        break;
    case SIE8CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SIE8_CLKDIV, PLL_SIE8_CLKDIV(Divider-1));
        break;
    case IDECLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(Divider-1));
        break;
    case IDEOUTIFCLK_FREQ:
        DBG_MSG("IDEOUTIFCLK_FREQ\r\n");
        pll_setClockRate(PLL_CLKSEL_IDE_OUTIF_CLKDIV, PLL_IDE_OUTIF_CLKDIV(Divider-1));
        break;
    case IDE2CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_IDE2_CLKDIV, PLL_IDE2_CLKDIV(Divider-1));
        break;
    case IDE2OUTIFCLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_IDE2_OUTIF_CLKDIV, PLL_IDE2_OUTIF_CLKDIV(Divider-1));
        break;
    case MICLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Divider-1));
        break;
    case DSICLK_FREQ:
        //??pll_setClockRate(PLL_CLKSEL_DSI_CLKDIV, PLL_DSI_CLKDIV(Divider-1));
        break;
    case SPCLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SP_CLKDIV, PLL_SP_CLKDIV(Divider-1));
        break;
    case SPCLK2_FREQ:
        pll_setClockRate(PLL_CLKSEL_SP2_CLKDIV, PLL_SP2_CLKDIV(Divider-1));
        break;
    case CECCLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_CEC_CLKDIV, PLL_CEC_CLKDIV(Divider-1));
        break;
    case ADOCLK_FREQ:
        if (pll_getClockRate(PLL_CLKSEL_ADO_CLKSEL) == PLL_CLKSEL_ADO_CLKSEL_CLKDIV)
        {
            #if !_FPGA_EMULATION_
            if (Divider == 1)
            {
                // ADO CLKDIV register should >= 1
                Divider = 2;
            }
            #endif
            pll_setClockRate(PLL_CLKSEL_ADO_CLKDIV, PLL_ADO_CLKDIV(Divider-1));
        }
        else
        {
            /*switch (uiFreq)
            {
            case 24576000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_24P576);
                break;
            case 12288000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_12P288);
                break;
            case 8192000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_8P192);
                break;
            case 6144000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_6P144);
                break;
            case 4096000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_4P096);
                break;
            case 3072000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_3P072);
                break;
            case 2048000:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_2P048);
                break;
            case 11289600:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_11P2896);
                break;
            case 5644800:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_5P6448);
                break;
            case 2822400:
            default:
                pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, PLL_CLKSEL_ADO_CLKMUX_2P8224);
                break;
            }*/
            return E_OK;
        }
        break;
    case HDMIADOCLK_FREQ:

        #if !_FPGA_EMULATION_
        if (Divider == 1)
        {
            // ADO CLKDIV register should >= 1
            Divider = 2;
        }
        #endif
        pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKDIV, PLL_HDMI_ADO_CLKDIV(Divider-1));

        switch (uiFreq)
        {
        case 24576000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_24P576);
            break;
        case 12288000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_12P288);
            break;
        case 8192000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_8P192);
            break;
        case 6144000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_6P144);
            break;
        case 4096000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_4P096);
            break;
        case 3072000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_3P072);
            break;
        case 2048000:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_2P048);
            break;
        case 11289600:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_11P2896);
            break;
        case 5644800:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_5P6448);
            break;
        case 2822400:
        default:
            pll_setClockRate(PLL_CLKSEL_HDMI_ADO_CLKMUX, PLL_CLKSEL_HDMI_ADO_CLKMUX_2P8224);
            break;
        }
        break;
    case SDIOCLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SDIO_CLKDIV, PLL_SDIO_CLKDIV(Divider-1));
        break;
    case SDIO2CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SDIO2_CLKDIV, PLL_SDIO2_CLKDIV(Divider-1));
        break;
    case SPICLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SPI_CLKDIV, PLL_SPI_CLKDIV(Divider-1));
        break;
    case SPI2CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SPI2_CLKDIV, PLL_SPI2_CLKDIV(Divider-1));
        break;
    case SPI3CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SPI3_CLKDIV, PLL_SPI3_CLKDIV(Divider-1));
        break;
    case SPI4CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SPI4_CLKDIV, PLL_SPI4_CLKDIV(Divider-1));
        break;
    /*case SPI5CLK_FREQ:
        pll_setClockRate(PLL_CLKSEL_SPI5_CLKDIV, PLL_SPI5_CLKDIV(Divider-1));
        break;*/

    default:
        return E_PAR;
    }

    // Output warning msg if target freq can not well divided from source freq
    if (SrcClk % uiFreq)
    {
        UINT32 uiRealFreq = SrcClk / Divider;

        // Truncate inaccuray under 1000 Hz
        uiRealFreq = (uiRealFreq + 50) / 1000;
        uiFreq /= 1000;
        if (uiFreq != uiRealFreq)
        {
            DBG_WRN("Target(%d) freq can not be divided with no remainder! Result is %dHz.\r\n",ClkID,(SrcClk/Divider));
            return E_PAR;
        }
    }

    return E_OK;
}

/**
    Get the module clock frequency

    This api get the module clock frequency.

    @param[in] ClkID    Module select ID, refer to structure PLL_CLKFREQ.
    @param[out] pFreq   Return clock frequency. Unit in Hertz.

    @return
     - @b E_ID:     ClkID is not support in this API.
     - @b E_PAR:    Target frequency can not be divided with no remainder.
     - @b E_OK:     Done and success.
*/
ER pll_getClockFreq(PLL_CLKFREQ ClkID, UINT32* pFreq)
{
    UINT32 SrcClk=0,Divider=0,ClkSrc=0;

    if(ClkID >= PLL_CLKFREQ_MAXNUM)
    {
        return E_ID;
    }

    if (pFreq == NULL)
    {
        DBG_ERR("input pFreq is NULL\r\n");
        return E_PAR;
    }

    // Get Src Clock Frequency
    switch(ClkID)
    {
    case SPICLK_FREQ:
    case SPI2CLK_FREQ:
    case SPI3CLK_FREQ:
    case SPI4CLK_FREQ:
    case SPI5CLK_FREQ:
        SrcClk = pll_getPLLFreq(PLL_ID_1);
        break;
    case SIEMCLK_FREQ:
        ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_MCLKSRC);
        if (ClkSrc == PLL_CLKSEL_SIE_MCLKSRC_480)
        {
            SrcClk = pll_getPLLFreq(PLL_ID_1);
        }
        else if (ClkSrc == PLL_CLKSEL_SIE_MCLKSRC_PLL5)
        {
            SrcClk = pll_getPLLFreq(PLL_ID_5);
        }
        else
        {
            return E_PAR;
        }
        break;
    case SIEMCLK2_FREQ:
        ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_MCLK2SRC);
        if (ClkSrc == PLL_CLKSEL_SIE_MCLK2SRC_480)
        {
            SrcClk = pll_getPLLFreq(PLL_ID_1);
        }
        else if (ClkSrc == PLL_CLKSEL_SIE_MCLK2SRC_PLL5)
        {
            SrcClk = pll_getPLLFreq(PLL_ID_5);
        }
        else
        {
            return E_PAR;
        }
        break;
    case SIECLK_FREQ:
        ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_CLKSRC);
        switch (ClkSrc)
        {
        case PLL_CLKSEL_SIE_CLKSRC_480:
            SrcClk = pll_getPLLFreq(PLL_ID_1);
            break;
        case PLL_CLKSEL_SIE_CLKSRC_PLL6:
            SrcClk = pll_getPLLFreq(PLL_ID_6);
            break;
        case PLL_CLKSEL_SIE_CLKSRC_PLL5:
            SrcClk = pll_getPLLFreq(PLL_ID_5);
            break;
        /*case PLL_CLKSEL_SIE_CLKSRC_PLL7:
            SrcClk = pll_getPLLFreq(PLL_ID_7);
            break;*/
        default:
            return E_PAR;
        }
        break;

    case SIE2CLK_FREQ:
        ClkSrc = pll_getClockRate(PLL_CLKSEL_SIE2_CLKSRC);
        switch (ClkSrc)
        {
        case PLL_CLKSEL_SIE2_CLKSRC_480:
            SrcClk = pll_getPLLFreq(PLL_ID_1);
            break;
        case PLL_CLKSEL_SIE2_CLKSRC_PLL6:
            SrcClk = pll_getPLLFreq(PLL_ID_6);
            break;
        case PLL_CLKSEL_SIE2_CLKSRC_PLL5:
            SrcClk = pll_getPLLFreq(PLL_ID_5);
            break;
        /*case PLL_CLKSEL_SIE2_CLKSRC_PLL7:
            SrcClk = pll_getPLLFreq(PLL_ID_7);
            break;*/
        default:
            return E_PAR;
        }
        break;

    case IDECLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_IDE_CLKSRC);
            if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else if (ClkSrc == PLL_CLKSEL_IDE_CLKSRC_PLL7)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_7);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    case IDE2CLK_FREQ:
        {
            ClkSrc = pll_getClockRate(PLL_CLKSEL_IDE2_CLKSRC);
            if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_480)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_1);
            }
            else if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_PLL2)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_2);
            }
            else if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_PLL4)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_4);
            }
            else if(ClkSrc == PLL_CLKSEL_IDE2_CLKSRC_PLL7)
            {
                SrcClk = pll_getPLLFreq(PLL_ID_7);
            }
            else
            {
                return E_PAR;
            }
        }
        break;

    default:
        break;
        //return E_PAR;
    }

    // Get Clock Divider
    switch(ClkID)
    {
    case SPICLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SPI_CLKDIV) >> (PLL_CLKSEL_SPI_CLKDIV - PLL_CLKSEL_R14_OFFSET);
        break;
    case SPI2CLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SPI2_CLKDIV) >> (PLL_CLKSEL_SPI2_CLKDIV - PLL_CLKSEL_R14_OFFSET);
        break;
    case SPI3CLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SPI3_CLKDIV) >> (PLL_CLKSEL_SPI3_CLKDIV - PLL_CLKSEL_R15_OFFSET);
        break;
    case SPI4CLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SPI4_CLKDIV) >> (PLL_CLKSEL_SPI4_CLKDIV - PLL_CLKSEL_R15_OFFSET);
        break;
    /*case SPI5CLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SPI5_CLKDIV) >> (PLL_CLKSEL_SPI5_CLKDIV - PLL_CLKSEL_R16_OFFSET);
        break;*/
    case SIEMCLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SIE_MCLKDIV) >> (PLL_CLKSEL_SIE_MCLKDIV - PLL_CLKSEL_R8_OFFSET);
        break;
    case SIEMCLK2_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SIE_MCLK2DIV) >> (PLL_CLKSEL_SIE_MCLK2DIV - PLL_CLKSEL_R8_OFFSET);
        break;
    case SIECLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SIE_CLKDIV) >> (PLL_CLKSEL_SIE_CLKDIV - PLL_CLKSEL_R8_OFFSET);
        break;
    case SIE2CLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_SIE2_CLKDIV) >> (PLL_CLKSEL_SIE2_CLKDIV - PLL_CLKSEL_R8_OFFSET);
        break;
    case IDECLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_IDE_CLKDIV) >> (PLL_CLKSEL_IDE_CLKDIV - PLL_CLKSEL_R23_OFFSET);
        break;
    case IDE2CLK_FREQ:
        Divider = pll_getClockRate(PLL_CLKSEL_IDE2_CLKDIV) >> (PLL_CLKSEL_IDE2_CLKDIV - PLL_CLKSEL_R23_OFFSET);
        break;

    default:
        return E_PAR;
    }

    *pFreq = SrcClk / (Divider + 1);

    return E_OK;
}

/**
    Module (hardware) reset on.

    This function will reset module.

    @param[in] Num  Reset bit number of type CG_RSTN, only one at a time

    @return void
*/
void pll_enableSystemReset(CG_RSTN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    uiRegOffset = (Num >> 5) << 2;

    //race condition protect. enter critical section
    loc_multiCores();

    RegData = PLL_GETREG(PLL_SYS_RST0_REG_OFS + uiRegOffset);
    RegData &= ~(1 << (Num & 0x1F));

    PLL_SETREG(PLL_SYS_RST0_REG_OFS + uiRegOffset, RegData);

    //race condition protect. leave critical section
    unl_multiCores();
}
#endif
/**
    Module (hardware) reset off.

    This fulction will enable module.

    @param[in] Num  Reset bit number of type CG_RSTN, only one at a time

    @return void
*/
void pll_disableSystemReset(CG_RSTN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    uiRegOffset = (Num >> 5) << 2;

    //race condition protect. enter critical section
    loc_multiCores();

    RegData = PLL_GETREG(PLL_SYS_RST0_REG_OFS + uiRegOffset);
    RegData |= 1 << (Num & 0x1F);

    PLL_SETREG(PLL_SYS_RST0_REG_OFS + uiRegOffset, RegData);

    //race condition protect. leave critical section
    unl_multiCores();
}
#if 0
/*
    Module (hardware) reset on for multiple modules.

    This function will reset multiple module.

    @param[in] uiGroup      reset register group.
    @param[in] uiMask       bit mask of module to be reset

    @return void
*/
void pll_writeSystemResetMulti(UINT32 uiGroup, UINT32 uiMask)
{
    UINT32      uiRegOffset;

    if (uiGroup == 0)
    {
        uiRegOffset = PLL_SYS_RST0_REG_OFS;
    }
    else if (uiGroup == 1)
    {
        uiRegOffset = PLL_SYS_RST1_REG_OFS;
    }
    else if (uiGroup == 2)
    {
        uiRegOffset = PLL_SYS_RST2_REG_OFS;
    }
    else if (uiGroup == 3)
    {
        uiRegOffset = PLL_SYS_RST3_REG_OFS;
    }
    else if (uiGroup == 4)
    {
        uiRegOffset = PLL_SYS_RST4_REG_OFS;
    }
    else
    {
        uiRegOffset = PLL_SYS_RST5_REG_OFS;
    }

    PLL_SETREG(uiRegOffset, uiMask);
}

/*
    Read Module (hardware) reset for multiple modules.

    This function will read multiple module reset status.

    @param[in] uiGroup      reset register group.

    @return bit mask of reset module
*/
UINT32 pll_readSystemResetMulti(UINT32 uiGroup)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    if (uiGroup == 0)
    {
        uiRegOffset = PLL_SYS_RST0_REG_OFS;
    }
    else if (uiGroup == 1)
    {
        uiRegOffset = PLL_SYS_RST1_REG_OFS;
    }
    else if (uiGroup == 2)
    {
        uiRegOffset = PLL_SYS_RST2_REG_OFS;
    }
    else if (uiGroup == 3)
    {
        uiRegOffset = PLL_SYS_RST3_REG_OFS;
    }
    else if (uiGroup == 4)
    {
        uiRegOffset = PLL_SYS_RST4_REG_OFS;
    }
    else
    {
        uiRegOffset = PLL_SYS_RST5_REG_OFS;
    }

    RegData = PLL_GETREG(uiRegOffset);

    return RegData;
}

/**
    Module (hardware) SRAM shut down.

    This function will shut down  module's SRAM.

    @param[in] Num  Shut down bit number of type CG_RSTN, only one at a time

    @return void
*/
void pll_enableSramShutDown(CG_RSTN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    uiRegOffset = (Num >> 5) << 2;

    //race condition protect. enter critical section
    loc_multiCores();

    RegData = PLL_GETREG(PLL_SYS_SRAMSD0_REG_OFS + uiRegOffset);
    RegData |= 1 << (Num & 0x1F);

    PLL_SETREG(PLL_SYS_SRAMSD0_REG_OFS + uiRegOffset, RegData);

    //race condition protect. leave critical section
    unl_multiCores();
}

/**
    Module (hardware) SRAM turn on.

    This fulction will enable module's SRAM

    @param[in] Num  Shut down bit number of type CG_RSTN, only one at a time

    @return void
*/
void pll_disableSramShutDown(CG_RSTN Num)
{
    REGVALUE    RegData;
    UINT32      uiRegOffset;

    uiRegOffset = (Num >> 5) << 2;

    //race condition protect. enter critical section
    loc_multiCores();

    RegData = PLL_GETREG(PLL_SYS_SRAMSD0_REG_OFS + uiRegOffset);
    RegData &= ~(1 << (Num & 0x1F));

    PLL_SETREG(PLL_SYS_SRAMSD0_REG_OFS + uiRegOffset, RegData);

    //race condition protect. leave critical section
    unl_multiCores();
}

/**
    Hardware Module Clock Auto Gating Enable

    This api is used to enable the module clock auto-gating to save power.

    @param[in] ClkSel  Module Gated Clock Select ID, refer to structure GATECLK.

    @return void

*/
void pll_setClkAutoGating(GATECLK ClkSel)
{
    T_PLL_CLKGATE1_REG  RegGateClk;
    UINT32              ofs,bit;

    switch (ClkSel)
    {
    case SIE_GCLK:
    case PRE_GCLK:
    case DAI_GCLK:
    case EAC_GCLK:
    case I2C_GCLK:
    case I2C2_GCLK:
    case SIE2_GCLK:
    //case ISE_GCLK:
    //case IME_GCLK:
    //case DIS_GCLK:
    //case SIF_GCLK:
        return;
    default:
        if ((ClkSel >= UART_GCLK) && (ClkSel <= ADC_GCLK))
            return;
        if ((ClkSel >= IDE_GCLK) && (ClkSel <= HDMI_GCLK))
            return;
        if ((ClkSel >= TMR_GCLK) && (ClkSel <= MIPI_DSI_GCLK))
            return;
        if ((ClkSel >= UART3_GCLK) && (ClkSel <= UART4_GCLK))
            return;
        if (ClkSel > SPI5_GCLK)
            return;
    }

    ofs = (ClkSel>>5)<<2;
    bit = (0x01 << (ClkSel & 0x1F));

    //race condition protect. enter critical section
    loc_multiCores();

    RegGateClk.Reg = PLL_GETREG(PLL_CLKGATE0_REG_OFS+ofs);
    RegGateClk.Reg |= bit;
    PLL_SETREG(PLL_CLKGATE0_REG_OFS+ofs, RegGateClk.Reg);

    //race condition protect. leave critical section
    unl_multiCores();
}

/**
    Hardware Module Clock Auto Gating Disable

    This api is used to disable the module clock auto-gating.

    @param[in] ClkSel  Module Gated Clock Select ID, refer to structure GATECLK.

    @return void

*/
void pll_clearClkAutoGating(GATECLK ClkSel)
{
    T_PLL_CLKGATE1_REG  RegGateClk;
    UINT32              ofs,bit;

    switch (ClkSel)
    {
    case SIE_GCLK:
    case PRE_GCLK:
    case DAI_GCLK:
    case EAC_GCLK:
    case I2C_GCLK:
    case I2C2_GCLK:
    case SIE2_GCLK:
    //case ISE_GCLK:
    //case IME_GCLK:
    //case DIS_GCLK:
    //case SIF_GCLK:
        return;
    default:
        if ((ClkSel >= UART_GCLK) && (ClkSel <= ADC_GCLK))
            return;
        if ((ClkSel >= IDE_GCLK) && (ClkSel <= HDMI_GCLK))
            return;
        if ((ClkSel >= TMR_GCLK) && (ClkSel <= MIPI_DSI_GCLK))
            return;
        if ((ClkSel >= UART3_GCLK) && (ClkSel <= UART4_GCLK))
            return;
        if (ClkSel > SPI5_GCLK)
            return;
    }

    ofs = (ClkSel>>5)<<2;
    bit = (0x01 << (ClkSel & 0x1F));

    //race condition protect. enter critical section
    loc_multiCores();

    RegGateClk.Reg = PLL_GETREG(PLL_CLKGATE0_REG_OFS+ofs);
    RegGateClk.Reg &= ~bit;
    PLL_SETREG(PLL_CLKGATE0_REG_OFS+ofs, RegGateClk.Reg);

    //race condition protect. leave critical section
    unl_multiCores();
}

/**
    Get Hardware Module Clock Auto Gating Enable/Disable

    This api is used to get the module clock auto-gating status.

    @param[in] ClkSel  Module Gated Clock Select ID, refer to structure GATECLK.

    @return
     - @b TRUE:  Auto-Gating is ENABLE.
     - @b FALSE: Auto-Gating is DISABLE.

*/
BOOL pll_getClkAutoGating(GATECLK ClkSel)
{
    T_PLL_CLKGATE1_REG  RegGateClk;
    UINT32              ofs,bit;

    switch (ClkSel)
    {
    case SIE_GCLK:
    case PRE_GCLK:
    case DAI_GCLK:
    case EAC_GCLK:
    case I2C_GCLK:
    case I2C2_GCLK:
    case SIE2_GCLK:
    //case ISE_GCLK:
    //case IME_GCLK:
    //case DIS_GCLK:
    //case SIF_GCLK:
        return FALSE;
    default:
        if ((ClkSel >= UART_GCLK) && (ClkSel <= ADC_GCLK))
            return FALSE;
        if ((ClkSel >= IDE_GCLK) && (ClkSel <= HDMI_GCLK))
            return FALSE;
        if ((ClkSel >= TMR_GCLK) && (ClkSel <= MIPI_DSI_GCLK))
            return FALSE;
        if ((ClkSel >= UART3_GCLK) && (ClkSel <= UART4_GCLK))
            return FALSE;
        if (ClkSel > SPI5_GCLK)
            return FALSE;
    }

    ofs = (ClkSel>>5)<<2;
    bit = (0x01 << (ClkSel & 0x1F));

    RegGateClk.Reg = PLL_GETREG(PLL_CLKGATE0_REG_OFS+ofs);
    return (RegGateClk.Reg & bit) > 0;
}

/*
    Set Hardware Module Clock Auto Gating Configuration

    This api is used to configure the module clock auto-gating to save power.

    @param[in] uiClkSel1  Module Gated Clock Select ID Register 1
    @param[in] uiClkSel2  Module Gated Clock Select ID Register 2
    @param[in] uiClkSel3  Module Gated Clock Select ID Register 3

    @return void
*/
void pll_configClkAutoGating(UINT32 uiClkSel1, UINT32 uiClkSel2, UINT32 uiClkSel3)
{
    PLL_SETREG(PLL_CLKGATE0_REG_OFS, uiClkSel1);
    PLL_SETREG(PLL_CLKGATE1_REG_OFS, uiClkSel2);
    PLL_SETREG(PLL_CLKGATE2_REG_OFS, uiClkSel3);
}

/**
    Hardware PCLK Clock Auto Gating Enable

    This api is used to enable the module PCLK auto-gating to save power.

    @param[in] ClkSel  Module Gated Clock Select ID, refer to structure GATECLK.

    @return void
*/
void pll_setPclkAutoGating(GATECLK ClkSel)
{
    T_PLL_PCLKGATE1_REG  RegGateClk;
    UINT32               ofs,bit;

    if (ClkSel >= PCLKGAT_MAXNUM)
        return;
    if (ClkSel == PWMCCNT_GCLK)
        return;

    ofs = (ClkSel>>5)<<2;
    bit = (0x01 << (ClkSel & 0x1F));

    //race condition protect. enter critical section
    loc_multiCores();

    RegGateClk.Reg = PLL_GETREG(PLL_PCLKGATE0_REG_OFS+ofs);
    RegGateClk.Reg |= bit;
    PLL_SETREG(PLL_PCLKGATE0_REG_OFS+ofs, RegGateClk.Reg);

    //race condition protect. leave critical section
    unl_multiCores();
}

/**
    Hardware PCLK Clock Auto Gating Disable

    This api is used to disable the module PCLK auto-gating.

    @param[in] ClkSel  Module Gated Clock Select ID, refer to structure GATECLK.

    @return void
*/
void pll_clearPclkAutoGating(GATECLK ClkSel)
{
    T_PLL_PCLKGATE1_REG  RegGateClk;
    UINT32               ofs,bit;

    if (ClkSel >= PCLKGAT_MAXNUM)
        return;
    if (ClkSel == PWMCCNT_GCLK)
        return;

    ofs = (ClkSel>>5)<<2;
    bit = (0x01 << (ClkSel & 0x1F));

    //race condition protect. enter critical section
    loc_multiCores();

    RegGateClk.Reg = PLL_GETREG(PLL_PCLKGATE0_REG_OFS+ofs);
    RegGateClk.Reg &= ~bit;
    PLL_SETREG(PLL_PCLKGATE0_REG_OFS+ofs, RegGateClk.Reg);

    //race condition protect. leave critical section
    unl_multiCores();
}




/**
    Get Hardware PCLK Clock Auto Gating Enable/Disable

    This api is used to get the module PCLK auto-gating status.

    @param[in] ClkSel  Module Gated Clock Select ID, refer to structure GATECLK.

    @return
     - @b TRUE:  Auto-Gating is ENABLE.
     - @b FALSE: Auto-Gating is DISABLE.
*/
BOOL pll_getPclkAutoGating(GATECLK ClkSel)
{
    T_PLL_PCLKGATE1_REG  RegGateClk;
    UINT32               ofs,bit;

    if (ClkSel >= PCLKGAT_MAXNUM)
        return FALSE;
    if (ClkSel == PWMCCNT_GCLK)
        return FALSE;

    ofs = (ClkSel>>5)<<2;
    bit = (0x01 << (ClkSel & 0x1F));

    RegGateClk.Reg = PLL_GETREG(PLL_PCLKGATE0_REG_OFS+ofs);
    return (RegGateClk.Reg & bit) > 0;

}

/*
    Set Hardware APB-Clock Auto Gating Configuration

    This api is used to configure the module clock auto-gating to save power.

    @param[in] uiClkSel1  Module APB-Clock Gated Clock Select ID Register 1
    @param[in] uiClkSel2  Module APB-Clock Gated Clock Select ID Register 2
    @param[in] uiClkSel3  Module APB-Clock Gated Clock Select ID Register 3

    @return void
*/
void pll_configPclkAutoGating(UINT32 uiClkSel1, UINT32 uiClkSel2, UINT32 uiClkSel3)
{
    PLL_SETREG(PLL_PCLKGATE0_REG_OFS, uiClkSel1);
    PLL_SETREG(PLL_PCLKGATE1_REG_OFS, uiClkSel2);
    PLL_SETREG(PLL_PCLKGATE2_REG_OFS, uiClkSel3);
}

/**
    Set clock frequency

    @param[in] clkID    clock frequency to be changed
    @param[in] uiFreq   target frequency (unit: Hz)

    @return
        - @b E_OK: success
        - @b E_NOSPT: clkID not support
*/
ER pll_setClkFreq(PLL_CLK clkID, UINT32 uiFreq)
{
    switch (clkID)
    {
    case PLL_CLK_SIEMCLK:
        pll_setClockFreq(SIEMCLK_FREQ, uiFreq);
        break;
    case PLL_CLK_SIEMCLK2:
        pll_setClockFreq(SIEMCLK2_FREQ, uiFreq);
        break;
    case PLL_CLK_SPCLK:
        pll_setClockFreq(SPCLK_FREQ, uiFreq);
        break;
    case PLL_CLK_SPCLK2:
        pll_setClockFreq(SPCLK2_FREQ, uiFreq);
        break;
    default:
        DBG_ERR("clkID %d not supported\r\n", clkID);
        return E_NOSPT;
    }

    return E_OK;
}

/**
    Get clock frequency

    @param[in] clkID    clock frequency to be read
    @param[out] puiFreq returned frequency (unit: Hz)

    @return
        - @b E_OK: success
        - @b E_NOSPT: clkID not support
*/
ER pll_getClkFreq(PLL_CLK clkID, UINT32* puiFreq)
{
    switch (clkID)
    {
    case PLL_CLK_SIEMCLK:
        pll_getClockFreq(SIEMCLK_FREQ, puiFreq);
        break;
    case PLL_CLK_SIEMCLK2:
        pll_getClockFreq(SIEMCLK2_FREQ, puiFreq);
        break;
    case PLL_CLK_SPCLK:
        pll_getClockFreq(SPCLK_FREQ, puiFreq);
        break;
    case PLL_CLK_SPCLK2:
        pll_getClockFreq(SPCLK2_FREQ, puiFreq);
        break;
    default:
        DBG_ERR("clkID %d not supported\r\n", clkID);
        return E_NOSPT;
    }

    return E_OK;
}

/**
    Select clock source

    @param[in] clkID    clock source to be selected
    @param[in] clkSrc   selected source

    @return
        - @b E_OK: success
        - @b E_NOSPT: clkID or clkSrc not support
*/
ER pll_selectClkSrc(PLL_CLK clkID, PLL_CLKSRC clkSrc)
{
    switch (clkID)
    {
    case PLL_CLK_SIEMCLK:
        if (clkSrc == PLL_CLKSRC_480)
        {
            pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_480);
        }
        else if (clkSrc == PLL_CLKSRC_PLL5)
        {
            pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL5);
        }
        else
        {
            DBG_ERR("(0x%lx, 0x%lx) not supported\r\n", clkID, clkSrc);
        }
        break;
    case PLL_CLK_SIEMCLK2:
        if (clkSrc == PLL_CLKSRC_480)
        {
            pll_setClockRate(PLL_CLKSEL_SIE_MCLK2SRC, PLL_CLKSEL_SIE_MCLK2SRC_480);
        }
        else if (clkSrc == PLL_CLKSRC_PLL5)
        {
            pll_setClockRate(PLL_CLKSEL_SIE_MCLK2SRC, PLL_CLKSEL_SIE_MCLK2SRC_PLL5);
        }
        else
        {
            DBG_ERR("(0x%lx, 0x%lx) not supported\r\n", clkID, clkSrc);
        }
        break;
    case PLL_CLK_SPCLK:
        if (clkSrc == PLL_CLKSRC_240)
        {
            pll_setClockRate(PLL_CLKSEL_SP, PLL_CLKSEL_SP_240);
        }
        else if (clkSrc == PLL_CLKSRC_PLL2)
        {
            pll_setClockRate(PLL_CLKSEL_SP, PLL_CLKSEL_SP_PLL2);
        }
        else if (clkSrc == PLL_CLKSRC_PLL5)
        {
            pll_setClockRate(PLL_CLKSEL_SP, PLL_CLKSEL_SP_PLL5);
        }
        else
        {
            DBG_ERR("(0x%lx, 0x%lx) not supported\r\n", clkID, clkSrc);
        }
        break;
    case PLL_CLK_SPCLK2:
        if (clkSrc == PLL_CLKSRC_240)
        {
            pll_setClockRate(PLL_CLKSEL_SP2, PLL_CLKSEL_SP2_240);
        }
        else if (clkSrc == PLL_CLKSRC_PLL2)
        {
            pll_setClockRate(PLL_CLKSEL_SP2, PLL_CLKSEL_SP2_PLL2);
        }
        else if (clkSrc == PLL_CLKSRC_PLL5)
        {
            pll_setClockRate(PLL_CLKSEL_SP2, PLL_CLKSEL_SP2_PLL5);
        }
        else
        {
            DBG_ERR("(0x%lx, 0x%lx) not supported\r\n", clkID, clkSrc);
        }
        break;
    default:
        DBG_ERR("clkID %d not supported\r\n", clkID);
        return E_NOSPT;
    }

    return E_OK;
}

/**
    Get clock source

    @param[in] clkID    clock source to be read
    @param[out] pClkSrc target clock source

    @return
        - @b E_OK: success
        - @b E_NOSPT: clkID or clkSrc not support
*/
ER pll_getClkSrc(PLL_CLK clkID, PLL_CLKSRC* pClkSrc)
{
    UINT32 uiClkSrc;

    switch (clkID)
    {
    case PLL_CLK_SIEMCLK:
        uiClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_MCLKSRC);
        if (uiClkSrc == PLL_CLKSEL_SIE_MCLKSRC_480)
        {
            *pClkSrc = PLL_CLKSRC_480;
        }
        else if (uiClkSrc == PLL_CLKSEL_SIE_MCLKSRC_PLL5)
        {
            *pClkSrc = PLL_CLKSRC_PLL5;
        }
        else
        {
            *pClkSrc = PLL_CLKSRC_UNKNOWN;
            DBG_ERR("clkID %d get unknown source 0x%lx\r\n", clkID, uiClkSrc);
        }
        break;
    case PLL_CLK_SIEMCLK2:
        uiClkSrc = pll_getClockRate(PLL_CLKSEL_SIE_MCLK2SRC);
        if (uiClkSrc == PLL_CLKSEL_SIE_MCLK2SRC_480)
        {
            *pClkSrc = PLL_CLKSRC_480;
        }
        else if (uiClkSrc == PLL_CLKSEL_SIE_MCLK2SRC_PLL5)
        {
            *pClkSrc = PLL_CLKSRC_PLL5;
        }
        else
        {
            *pClkSrc = PLL_CLKSRC_UNKNOWN;
            DBG_ERR("clkID %d get unknown source 0x%lx\r\n", clkID, uiClkSrc);
        }
        break;
    case PLL_CLK_SPCLK:
        uiClkSrc = pll_getClockRate(PLL_CLKSEL_SP);
        if (uiClkSrc == PLL_CLKSEL_SP_240)
        {
            *pClkSrc = PLL_CLKSRC_240;
        }
        else if (uiClkSrc == PLL_CLKSEL_SP_PLL2)
        {
            *pClkSrc = PLL_CLKSRC_PLL2;
        }
        else if (uiClkSrc == PLL_CLKSEL_SP_PLL5)
        {
            *pClkSrc = PLL_CLKSRC_PLL5;
        }
        break;
    case PLL_CLK_SPCLK2:
        uiClkSrc = pll_getClockRate(PLL_CLKSEL_SP2);
        if (uiClkSrc == PLL_CLKSEL_SP2_240)
        {
            *pClkSrc = PLL_CLKSRC_240;
        }
        else if (uiClkSrc == PLL_CLKSEL_SP2_PLL2)
        {
            *pClkSrc = PLL_CLKSRC_PLL2;
        }
        else if (uiClkSrc == PLL_CLKSEL_SP2_PLL5)
        {
            *pClkSrc = PLL_CLKSRC_PLL5;
        }
        break;
    default:
        DBG_ERR("clkID %d not supported\r\n", clkID);
        return E_NOSPT;
    }

    return E_OK;
}

/**
    Set clock enable

    @param[in] clkID    Clock ID to be enabled
    @param[in] bEnable  enable/disable clkID
                        - @b TRUE: enable clock
                        - @b FALSE: disable clock

    @return
        - @b E_OK: success
        - @b E_NOSPT: clkID not support
*/
ER pll_setClkEn(PLL_CLK clkID, BOOL bEnable)
{
    switch (clkID)
    {
    case PLL_CLK_SIEMCLK:
        if (bEnable)
        {
            pll_enableClock(SIE_MCLK);
        }
        else
        {
            pll_disableClock(SIE_MCLK);
        }
        break;
    case PLL_CLK_SIEMCLK2:
        if (bEnable)
        {
            pll_enableClock(SIE_MCLK2);
        }
        else
        {
            pll_disableClock(SIE_MCLK2);
        }
        break;
    case PLL_CLK_SPCLK:
        if (bEnable)
        {
            pll_enableClock(SP_CLK);
        }
        else
        {
            pll_disableClock(SP_CLK);
        }
        break;
    case PLL_CLK_SPCLK2:
        if (bEnable)
        {
            pll_enableClock(SP2_CLK);
        }
        else
        {
            pll_disableClock(SP2_CLK);
        }
        break;
    default:
        DBG_ERR("clkID %d not supported\r\n", clkID);
        return E_NOSPT;
    }

    return E_OK;
}

/**
    Get clock enable

    @param[in] clkID    Clock ID to be enabled
    @param[out] pEnable return enable or disable

    @return
        - @b E_OK: success
        - @b E_NOSPT: clkID not support
*/
ER pll_getClkEn(PLL_CLK clkID, BOOL* pEnable)
{
    switch (clkID)
    {
    case PLL_CLK_SIEMCLK:
        *pEnable = pll_isClockEnabled(SIE_MCLK);
        break;
    case PLL_CLK_SIEMCLK2:
        *pEnable = pll_isClockEnabled(SIE_MCLK2);
        break;
    case PLL_CLK_SPCLK:
        *pEnable = pll_isClockEnabled(SP_CLK);
        break;
    case PLL_CLK_SPCLK2:
        *pEnable = pll_isClockEnabled(SP2_CLK);
        break;
    default:
        DBG_ERR("clkID %d not supported\r\n", clkID);
        return E_NOSPT;
    }

    return E_OK;
}
#endif
//@}
