/*
    DSP module driver

    DSP module driver.

    @file       dsp.c
    @ingroup    mIDrvSys_DSP
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#include <common.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include <asm/arch/nvt_common.h>
#include <asm/arch/IOAddress.h>
#include "dsp.h"
#include "dsp_reg.h"
#include "../drv/pll.h"
#include "../drv/dma.h"
#include "../drv/pmc.h"

extern ER pinmux_activeDSPPERI(uint32_t pinmux);
extern ER pinmux_activeDSPIOP(uint32_t pinmux);
extern ER pinmux_activeDSP2PERI(uint32_t pinmux);
extern ER pinmux_activeDSP2IOP(uint32_t pinmux);
//#include "pll_protected.h"
//#include "Utility.h"
//#include "dma.h"

#define DSP_SETREG(ofs,value)      OUTW(IOADDR_DSP_REG_BASE+(ofs),value)
#define DSP_GETREG(ofs)            INW(IOADDR_DSP_REG_BASE+(ofs))

#define DSP2_SETREG(ofs,value)     OUTW(IOADDR_DSP2_REG_BASE+(ofs),value)
#define DSP2_GETREG(ofs)           INW(IOADDR_DSP2_REG_BASE+(ofs))

#define Delay_DelayUsPolling(x) 	udelay(x)

//static UINT8            vdsp_Opened[] = {FALSE, FALSE};
//static ID               vdsp_LockStatus[] = {NO_TASK_LOCKED, NO_TASK_LOCKED};

//static ER dsp_lock(DSP_ID id);
//static ER dsp_unlock(DSP_ID id);
//static ID dsp_getLockStatus(DSP_ID id);


REGVALUE dsp_getReg(UINT32 id, UINT32 offset);
static void dsp_setReg(UINT32 id, UINT32 offset, REGVALUE value);


#if 0
void dsp_isr(void)
{
     T_DSP_CPU1_INT_STS0_REG sts0Reg;
     T_DSP_CPU1_INT_STS1_REG sts1Reg;

     sts0Reg.Reg = dsp_getReg(DSP_ID_1, DSP_CPU1_INT_STS0_REG_OFS);
     sts1Reg.Reg = dsp_getReg(DSP_ID_1, DSP_CPU1_INT_STS1_REG_OFS);

     dsp_setReg(DSP_ID_1, DSP_CPU1_INT_STS0_REG_OFS, sts0Reg.Reg);
     dsp_setReg(DSP_ID_1, DSP_CPU1_INT_STS1_REG_OFS, sts1Reg.Reg);


     DBG_DUMP("\r\nDSP ISR sts0 = 0x%x, sts1 = 0x%x!!\r\n", sts0Reg, sts1Reg.Reg);

	 //for bus monitor
	 /*if (sts0Reg.Bit.DSP_GOES_STANDBY)
	 {
	      dma_disableChannel(DMA_CH_DSP_0);
		  dma_disableChannel(DMA_CH_DSP_1);
	      DBG_DUMP("\r\nDisable DSP dma channel\r\n");
	 }*/
}

void dsp2_isr(void)
{
	 T_DSP_CPU1_INT_STS0_REG sts0Reg;
	 T_DSP_CPU1_INT_STS1_REG sts1Reg;

	 sts0Reg.Reg = dsp_getReg(DSP_ID_2, DSP_CPU1_INT_STS0_REG_OFS);
	 sts1Reg.Reg = dsp_getReg(DSP_ID_2, DSP_CPU1_INT_STS1_REG_OFS);

	 dsp_setReg(DSP_ID_2, DSP_CPU1_INT_STS0_REG_OFS, sts0Reg.Reg);
	 dsp_setReg(DSP_ID_2, DSP_CPU1_INT_STS1_REG_OFS, sts1Reg.Reg);

	 DBG_DUMP("\r\nDSP2 ISR sts0 = 0x%x, sts1 = 0x%x!!\r\n", sts0Reg.Reg, sts1Reg.Reg);

	//for bus monitor
	/*if (sts0Reg.Bit.DSP_GOES_STANDBY)
	{
		 dma_disableChannel(DMA_CH_DSP2_0);
		 dma_disableChannel(DMA_CH_DSP2_1);
		 DBG_DUMP("\r\nDisable DSP2 dma channel\r\n");
	}*/
}
#endif

/**
    @addtogroup mIDrvSys_DSP
*/
//@{
#if 0
/*
    Lock DSP Module.

    Lock DSP Module.

    @return Lock result
        - @b E_OK       : Success
        - @b E_ID        : Outside semaphore ID number range
        - @b E_NOEXS  : Semaphore does not yet exist
*/
static ER dsp_lock(DSP_ID id)
{
    ER erReturn;

    if (id > DSP_ID_2)
    {
        return E_NOSPT;
    }

    erReturn = wai_sem(SEMID_DSP);

    if (erReturn != E_OK)
        return erReturn;

    vdsp_LockStatus[id] = TASK_LOCKED;

    return E_OK;
}

/*
    Unlock DSP Module.

    Unlock DSP Module.

    @return Unlock result
        - @b E_OK   : Success
        - @b E_ID    : Outside semaphore ID number range
        - @b E_NOEXS: Semaphore does not yet exist
        - @b E_QOVR : Semaphore's counter error, maximum counter < counter
*/
static ER dsp_unlock(DSP_ID id)
{
    if (id > DSP_ID_2)
    {
        return E_NOSPT;
    }

    vdsp_LockStatus[id] = NO_TASK_LOCKED;

    return sig_sem(SEMID_DSP);
}

/**
    Get DSP Lock Status.

    Get DSP Lock Status.

    @return
        - @b NO_TASK_LOCKED: COE is NOT locked by any task
        - @b TASK_LOCKED: COE is locked some task
*/
static ID dsp_getLockStatus(DSP_ID id)
{
    if (id > DSP_ID_2)
    {
        return E_NOSPT;
    }

    return vdsp_LockStatus[id];
}
#endif

/*
    Get DSP controller register.

    @param[in] id           DSP channel ID
                            - @b DSP_ID_1: DSP1
                            - @b DSP_ID_2: DSP2
    @param[in] offset       register offset in DSP controller (word alignment)

    @return register value
*/

REGVALUE dsp_getReg(UINT32 id, UINT32 offset)
{
    if (id == DSP_ID_1)
    {
        return DSP_GETREG(offset);
    }
    else if (id == DSP_ID_2)
    {
        return DSP2_GETREG(offset);
    }
    else
    {
        return 0;
    }
}
/*
    Set DSP controller register.

    @param[in] id           DSP channel ID
                            - @b DSP_ID_1: DSP1
                            - @b DSP_ID_2: DSP2
    @param[in] offset       register offset in DSP controller (word alignment)
    @param[in] value        register value

    @return void
*/
static void dsp_setReg(UINT32 id, UINT32 offset, REGVALUE value)
{
    if (id == DSP_ID_1)
    {
        DSP_SETREG(offset, value);
    }
    else if (id == DSP_ID_2)
    {
        DSP2_SETREG(offset, value);
    }
}



/**
    Set DSP configuration

    Set DSP configuration.

    @param[in] uiConfig     configuration context

    @return
        - @b E_OK       : Set configuration success
*/
ER dsp_setConfig(DSP_ID id, DSP_CONFIG_ID ConfigID, UINT32 uiCfgValue)
{
    union T_DSP_BOOTCTR_REG  dspbootReg;
    union T_DSP_DBG_SIG_REG  dspdbgReg;
    union T_DSP_CTL0_REG     dspctl0Reg;
    union T_DSP_CTL3_REG     dspctl3Reg;
	union T_DSP_AXIBRG_REG   dspaxibrgReg;


    loc_cpu();
    switch (ConfigID)
    {
        case DSP_CONFIG_ID_BOOTVECTOR:
        {
            dspbootReg.Reg = dsp_getReg(id, DSP_BOOTCTR_REG_OFS);
            dspbootReg.Bit.DSP_BOOTVECT = (uiCfgValue >> 12);
            dspbootReg.Bit.DSP_BOOTKEY = 0x5A9;
            dsp_setReg(id, DSP_BOOTCTR_REG_OFS, dspbootReg.Reg);
        }
        break;

        case DSP_CONFIG_ID_BOOTMODE:
        {
            dspctl0Reg.Reg = dsp_getReg(id, DSP_CTL0_REG_OFS);
            dspctl0Reg.Bit.DSP_RESET_MODE = uiCfgValue;
            dsp_setReg(id, DSP_CTL0_REG_OFS, dspctl0Reg.Reg);
        }
        break;

        case DSP_CONFIG_ID_DERESET:
        {
            dspctl3Reg.Reg = dsp_getReg(id, DSP_CTL3_REG_OFS);
            dspctl3Reg.Bit.DSP_CORE_RSTN = (uiCfgValue & 0x01);
            dspctl3Reg.Bit.DSP_SYS_RSTN = (uiCfgValue & 0x01);
            dspctl3Reg.Bit.DSP_OCEM_RSTN = (uiCfgValue & 0x01);
            dspctl3Reg.Bit.DSP_GLOBAL_RSTN = (uiCfgValue & 0x01);
            dsp_setReg(id, DSP_CTL3_REG_OFS, dspctl3Reg.Reg);
        }
        break;

        case DSP_CONFIG_ID_DEBUGPORTEN:
        {
            dspdbgReg.Reg = dsp_getReg(id, DSP_DBG_SIG_REG_OFS);
            dspdbgReg.Bit.DBG_EN = (uiCfgValue & 0x01);
            dsp_setReg(id, DSP_DBG_SIG_REG_OFS, dspdbgReg.Reg);
        }
        break;

        case DSP_CONFIG_ID_DEBUGPORTSEL:
        {
            dspdbgReg.Reg = dsp_getReg(id, DSP_DBG_SIG_REG_OFS);
            dspdbgReg.Bit.DBG_SEL = (uiCfgValue & 0x0F);
            dsp_setReg(id, DSP_DBG_SIG_REG_OFS, dspdbgReg.Reg);
        }
        break;

        case DSP_CONFIG_ID_EPP_POSTD:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EPP_POSTD = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EPP_PAWBA_MODE:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EPP_PAWBA_MODE = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EPP_AWBA_SET:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EPP_AWBA_SET = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EPP_PPRE_RD_OFF:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EPP_PPRE_RD_OFF = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EDP_POSTD:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EDP_POSTD = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EDP_PAWBA_MODE:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EDP_PAWBA_MODE = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EDP_AWBA_SET:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EDP_AWBA_SET = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;

        case DSP_CONFIG_ID_EDP_PPRE_RD_OFF:
		{
			dspaxibrgReg.Reg = dsp_getReg(id, DSP_AXIBRG_REG_OFS);
			dspaxibrgReg.Bit.DSP_EDP_PPRE_RD_OFF = (uiCfgValue & 0x01);
			dsp_setReg(id, DSP_AXIBRG_REG_OFS, dspaxibrgReg.Reg);
		}
        break;


        default:
        break;
    }
    unl_cpu();
    return E_OK;
}

/**
    Open DSP Module

    Open DSP Module.

    @return Open result
        - @b E_OK          : Open DSP success
        - @b Else            : Close DSP fail
*/
ER dsp_open(DSP_ID id)
{
    //ER erReturn;
    //UINT32 uiReg;
    //union T_DSP_DEST0_REG des0Reg;
    //union T_DSP_CPU1_INT_EN0_REG inten0Reg;

    if (id > DSP_ID_2)
    {
        return E_NOSPT;
    }

    //if(vdsp_Opened[id])
    //    return E_OK;

    if (id == DSP_ID_1)
    {
        // Turn on power
        pmc_turnonPower(PMC_MODULE_DSP);

		// Enable clock
		pll_enableClock(DSP_CLK);

		pll_disableSystemReset(CHIPPAR2_RSTN);

		// Wait for 2 us
		// Note: We already disable interrupt via loc_cpu()
		Delay_DelayUsPolling(2);

        pll_disableSystemReset(DSP_AXIB_EPP_RSTN);
        pll_disableSystemReset(DSP_AXIB_EDP_RSTN);
        pll_disableSystemReset(DSP_APBTX_RSTN);

        //Delay_DelayUsPolling(2);

        pll_disableSystemReset(DSP_NIC_RSTN);

        pll_disableSystemReset(DSP_RSTN);

		dma_enableChannel(DMA_CH_DSP_0);
		dma_enableChannel(DMA_CH_DSP_1);

		// Enable top's channel
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP_PERI_CH_EN, PINMUX_DSP_CH_ACTIVE);
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP_IOP_CH_EN, PINMUX_DSP_CH_ACTIVE);
		pinmux_activeDSPPERI(1);
		pinmux_activeDSPIOP(1);


        //Enable PLL9
        pll_setPLLEn(PLL_ID_9, TRUE);

    }
    else
    {
        // Turn on power
        pmc_turnonPower(PMC_MODULE_DSP2);

        // Enable clock
        pll_enableClock(DSP2_CLK);

		pll_disableSystemReset(CHIPPAR8_RSTN);

		// Wait for 2 us
		// Note: We already disable interrupt via loc_cpu()
		Delay_DelayUsPolling(2);

        pll_disableSystemReset(DSP2_AXIB_EPP_RSTN);
        pll_disableSystemReset(DSP2_AXIB_EDP_RSTN);
        pll_disableSystemReset(DSP2_APBTX_RSTN);

		//Delay_DelayUsPolling(2);

        pll_disableSystemReset(DSP2_NIC_RSTN);

        pll_disableSystemReset(DSP2_RSTN);

		dma_enableChannel(DMA_CH_DSP2_0);
		dma_enableChannel(DMA_CH_DSP2_1);

		// Enable top's channel
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP2_PERI_CH_EN, PINMUX_DSP_CH_ACTIVE);
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP2_IOP_CH_EN, PINMUX_DSP_CH_ACTIVE);
		pinmux_activeDSP2PERI(1);
		pinmux_activeDSP2IOP(1);

        //Enable PLL10
        pll_setPLLEn(PLL_ID_10, TRUE);

    }
	#if 0
    // Clear Interrupt status
    uiReg = dsp_getReg(id, DSP_CPU1_INT_STS0_REG_OFS);

    dsp_setReg(id, DSP_CPU1_INT_STS0_REG_OFS, uiReg);

    uiReg = dsp_getReg(id, DSP_CPU1_INT_STS1_REG_OFS);

    dsp_setReg(id, DSP_CPU1_INT_STS1_REG_OFS, uiReg);

    // Enable Interrupt dest
    des0Reg.Reg = dsp_getReg(id, DSP_DEST0_REG_OFS);
    des0Reg.Bit.DSP_TO_CPU1 = 1;
    dsp_setReg(id, DSP_DEST0_REG_OFS, des0Reg.Reg);

    // Enable DSP's Interrupt
    inten0Reg.Reg = dsp_getReg(id, DSP_CPU1_INT_EN0_REG_OFS);
    inten0Reg.Bit.DSP_GOES_STANDBY = 1;
    inten0Reg.Bit.DSP_ERROR = 1;
    inten0Reg.Bit.DSP_DDMA_DBG_MATCH = 1;
    inten0Reg.Bit.DSP_PI = 1;
    inten0Reg.Bit.DSP_PSU_MAPV = 1;
    inten0Reg.Bit.DSP_OCEM_DBG = 1;
    dsp_setReg(id, DSP_CPU1_INT_EN0_REG_OFS, inten0Reg.Reg);

    //uiReg = dsp_getReg(id, DSP_CPU1_INT_EN1_REG_OFS);
    uiReg = 0xFFFFFFFF;
    dsp_setReg(id, DSP_CPU1_INT_EN1_REG_OFS, uiReg);

    if (id == DSP_ID_1)
    {
        // Enable interrupt
        drv_enableInt(DRV_INT_DSP);
		// Enable top's channel
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP_PERI_CH_EN, PINMUX_DSP_CH_ACTIVE);
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP_IOP_CH_EN, PINMUX_DSP_CH_ACTIVE);
    }
    else
    {
        // Enable interrupt
        drv_enableInt(DRV_INT_DSP2);
		// Enable top's channel
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP2_PERI_CH_EN, PINMUX_DSP_CH_ACTIVE);
		//pinmux_setPinmux(PINMUX_FUNC_ID_DSP2_IOP_CH_EN, PINMUX_DSP_CH_ACTIVE);
    }

    //vdsp_Opened[id] = TRUE;
	#endif
    return E_OK;
}
#if 0
/**
    Close DSP Module.

    Close DSP Module.

    @return Close result
        - @b E_OK          : Close DSP success
        - @b Else            : Close DSP fail
*/
ER dsp_close(DSP_ID id)
{
    //ER      erReturn;

    if (id > DSP_ID_2)
    {
        return E_NOSPT;
    }

    if (id == DSP_ID_1)
    {
        // Disable interrupt
        drv_disableInt(DRV_INT_DSP);

        // Disable dma channel
        dma_disableChannel(DMA_CH_DSP_0);
		dma_disableChannel(DMA_CH_DSP_1);

		// Disable top's channel
		pinmux_setPinmux(PINMUX_FUNC_ID_DSP_PERI_CH_EN, PINMUX_DSP_CH_INACTIVE);
		pinmux_setPinmux(PINMUX_FUNC_ID_DSP_IOP_CH_EN, PINMUX_DSP_CH_INACTIVE);

		// assert dsp reset
        dsp_setConfig(DSP_ID_1, DSP_CONFIG_ID_DERESET, FALSE);

		// assert nic reset
		pll_enableSystemReset(DSP_NIC_RSTN);

        pll_enableSystemReset(DSP_AXIB_EPP_RSTN);
        pll_enableSystemReset(DSP_AXIB_EDP_RSTN);
        pll_enableSystemReset(DSP_APBTX_RSTN);

		pll_enableSystemReset(DSP_RSTN);

		Delay_DelayUsPolling(2);

        // assert chip2
		pll_disableSystemReset(CHIPPAR2_RSTN);

		// Disable clock
		pll_disableClock(DSP_CLK);

		Delay_DelayUsPolling(2);


        pmc_turnoffPower(PMC_MODULE_DSP);
    }
    else
    {
        // Disable interrupt
        drv_disableInt(DRV_INT_DSP2);

        // Disable dma channel
        dma_disableChannel(DMA_CH_DSP2_0);
		dma_disableChannel(DMA_CH_DSP2_1);

		// Disable top's channel
		pinmux_setPinmux(PINMUX_FUNC_ID_DSP2_PERI_CH_EN, PINMUX_DSP_CH_INACTIVE);
		pinmux_setPinmux(PINMUX_FUNC_ID_DSP2_IOP_CH_EN, PINMUX_DSP_CH_INACTIVE);

		// assert dsp reset
        dsp_setConfig(DSP_ID_2, DSP_CONFIG_ID_DERESET, FALSE);

		// assert nic reset
		pll_enableSystemReset(DSP2_NIC_RSTN);

        pll_enableSystemReset(DSP2_AXIB_EPP_RSTN);
        pll_enableSystemReset(DSP2_AXIB_EDP_RSTN);
        pll_enableSystemReset(DSP2_APBTX_RSTN);

		pll_enableSystemReset(DSP2_RSTN);

		Delay_DelayUsPolling(2);

        // assert chip2
		pll_disableSystemReset(CHIPPAR8_RSTN);

		// Disable clock
		pll_disableClock(DSP2_CLK);

		Delay_DelayUsPolling(2);

        pmc_turnoffPower(PMC_MODULE_DSP2);
    }


    //vdsp_Opened[id] = FALSE;

    return E_OK;
}

/**
    Check if DSP Driver is opened for accessing.

    Check if DSP Driver is opened for accessing.

    @return Driver is opened or closed
        - @b TRUE   : DSP driver is opened
        - @b FALSE  : DSP driver is closed
 */
BOOL dsp_isOpened(DSP_ID id)
{
    return vdsp_Opened[id];
}
#endif

//@}

