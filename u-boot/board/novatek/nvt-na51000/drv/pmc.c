/*
    PMC module driver

    PMC module driver.

    @file       pmc.c
    @ingroup    mIDrvSys_PMC
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#include <common.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include <asm/arch/nvt_common.h>
#include <asm/arch/IOAddress.h>

#include "pmc.h"
#include "pmc_reg.h"
#include "pll.h"
//#include "pll_protected.h"
//#include "Utility.h"

#define Delay_DelayUsPolling(x) 	udelay(x)

// Get power domain number from module
#define pmc_getDomain(m)        ((m) >> 5)

// Power domain number
#define PMC_POWER_DOMIAN_1      0
#define PMC_POWER_DOMIAN_2      1
//#define PMC_POWER_DOMIAN_3      2
#define PMC_POWER_DOMAIN_NONE   4

// Set register base address
REGDEF_SETIOBASE(IOADDR_PMC_REG_BASE);

// Bitwise status of power domain 1's module ON status
static UINT32   uiPMCPD1OnSts;
// Bitwise status of power domain 2's module ON status
static UINT32   uiPMCPD2OnSts;
// Bitwise status of power domain 2's module ON status
//static UINT32   uiPMCPD3OnSts;

/**
    @addtogroup mIDrvSys_PMC
*/
//@{

/**
    Query module's power status

    This function will return module's power status.

    @param[in] Module   Module that you want to query

    @return Power status
        - @b PMC_PWRSTS_OFF : Power is OFF
        - @b PMC_PWRSTS_ON  : Power is ON
*/
PMC_PWRSTS pmc_getPWRStatus(PMC_MODULE Module)
{
    RCW_DEF(PMC_PWROFF_REG);

    RCW_LD(PMC_PWROFF_REG);
    switch (pmc_getDomain(Module))
    {
    case PMC_POWER_DOMIAN_1:
        if (RCW_OF(PMC_PWROFF_REG).PwrOff_PD1 == 0)
        {
            return PMC_PWRSTS_ON;
        }
        return PMC_PWRSTS_OFF;

    case PMC_POWER_DOMIAN_2:
        if (RCW_OF(PMC_PWROFF_REG).PwrOff_PD2 == 0)
        {
            return PMC_PWRSTS_ON;
        }
        return PMC_PWRSTS_OFF;

    /*case PMC_POWER_DOMIAN_3:
        if (RCW_OF(PMC_PWROFF_REG).PwrOff_PD3 == 0)
        {
            return PMC_PWRSTS_ON;
        }
        return PMC_PWRSTS_OFF;*/

    default:
        return PMC_PWRSTS_ON;
    }
}

//@}


// Power domain 1 module reset register group4
#define PMC_PD1_RST_G4     (                                  \
                            (1 << ((DSP_RSTN) - 128))       | \
                            0)

#define PMC_PD2_RST_G4     (                                  \
                            (1 << ((DSP2_RSTN) - 128))      | \
                            0)

/*
    Turn on power

    Turn on power of specific module. To turn on module, please follow the flow:\n
    pmc_turnonPower()\n
    pll_enableClock()

    @note       Non-public function, only for driver usage.

    @param[in] Module   Module that you want to turn on power

    @return void
*/
void pmc_turnonPower(PMC_MODULE Module)
{
    RCW_DEF(PMC_PWROFF_REG);
    RCW_DEF(PMC_SIG_REG);
    RCW_DEF(PMC_PWRRST_REG);
    //UINT32 uiRstReg0, uiRstReg1, uiRstReg2, uiRstReg3;
    //UINT32 uiRstReg4;

    // Enter critical section
    loc_cpu();

    RCW_LD(PMC_SIG_REG);
    RCW_LD(PMC_PWROFF_REG);
    RCW_LD(PMC_PWRRST_REG);

    switch (pmc_getDomain(Module))
    {
    // Domain 1
    case PMC_POWER_DOMIAN_1:
        // There is no module in power domain 1 is power on, we need to turn on power
        if (uiPMCPD1OnSts == 0)
        {
            // Turn on power domain 2's power
            RCW_OF(PMC_PWROFF_REG).PwrOff_PD1 = 0;
            RCW_ST(PMC_PWROFF_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

            RCW_OF(PMC_PWRRST_REG).PwrRst_PD1 = 1;
            RCW_ST(PMC_PWRRST_REG);

            // Modules leave reset state
            // Note: These code must be checked with CG -begin
            // pll_writeSystemResetMulti(3, uiRstReg3 | PMC_PD1_RST_G3);
            // Note: These code must be checked with CG -end

            // Wait for 2 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

            // Enable normal signal output
            RCW_OF(PMC_SIG_REG).SigEn_PD1 = 1;
            RCW_ST(PMC_SIG_REG);

            // Wait for 2 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

        }

        // Set power on status of this module
        uiPMCPD1OnSts |= (1 << (Module - PMC_MODULE_PD1));
        break;

    // Domain 2
    case PMC_POWER_DOMIAN_2:

        // There is no module in power domain 1 is power on, we need to turn on power
        if (uiPMCPD2OnSts == 0)
        {
            // Turn on power domain 2's power
            RCW_OF(PMC_PWROFF_REG).PwrOff_PD2 = 0;
            RCW_ST(PMC_PWROFF_REG);

            // Wait for 2 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

            RCW_OF(PMC_PWRRST_REG).PwrRst_PD2 = 1;
            RCW_ST(PMC_PWRRST_REG);

            // Modules leave reset state
            // Note: These code must be checked with CG -begin
            // pll_writeSystemResetMulti(3, uiRstReg3 | PMC_PD1_RST_G3);
            // Note: These code must be checked with CG -end

            // Wait for 2 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

            // Enable normal signal output
            RCW_OF(PMC_SIG_REG).SigEn_PD2 = 1;
            RCW_ST(PMC_SIG_REG);

            // Wait for 2 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);
        }

        // Set power on status of this module
        uiPMCPD2OnSts |= (1 << (Module - PMC_MODULE_PD2));
        break;

    // Domain none
    case PMC_POWER_DOMAIN_NONE:
    default:
        break;
    }

    // Leave critical section
    unl_cpu();
}

/*
    Turn off power

    Turn off power of specific module. To turn off module, please follow the flow:\n
    pll_disableClock()\n
    pmc_turnoffPower()

    @note       Non-public function, only for driver usage.

    @param[in] Module   Module that you want to turn off power

    @return void
*/
void pmc_turnoffPower(PMC_MODULE Module)
{
    RCW_DEF(PMC_PWROFF_REG);
    RCW_DEF(PMC_SIG_REG);
    RCW_DEF(PMC_PWRRST_REG);

    // Enter critical section
    loc_cpu();

    RCW_LD(PMC_SIG_REG);
    RCW_LD(PMC_PWROFF_REG);
    RCW_LD(PMC_PWRRST_REG);

    switch (pmc_getDomain(Module))
    {
    // Domain 1
    case PMC_POWER_DOMIAN_1:
#if 0
        // Clear power on status of this module
        uiPMCPD1OnSts &= ~(1 << (Module - PMC_MODULE_PD1));

        pll_enableSystemReset(DSP_RSTN);

        //pll_disableSystemReset(DSPOCP_RSTN);

        // disable clock
        pll_disableClock(DSP_CLK);

        Delay_DelayUsPolling(2);
#endif

        // There is no module in power domain 1 is power on, we need to turn off power
        if (uiPMCPD1OnSts == 0)
        {
            // Isolate module's signal output
            RCW_OF(PMC_SIG_REG).SigEn_PD1 = 0;
            RCW_ST(PMC_SIG_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

            // Modules enter reset state
            // Note: These code must be checked with CG -begin
            // Get module reset status
            // uiRstReg3 = pll_readSystemResetMulti(3);

            // pll_writeSystemResetMulti(3, uiRstReg3 & ~PMC_PD1_RST_G3);
            // Note: These code must be checked with CG -end

            // PMC enter reset state
            RCW_OF(PMC_PWRRST_REG).PwrRst_PD1 = 0;
            RCW_ST(PMC_PWRRST_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
             Delay_DelayUsPolling(2);

            // Turn off power domain 2's power
            RCW_OF(PMC_PWROFF_REG).PwrOff_PD1   = 1;
            RCW_ST(PMC_PWROFF_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);
        }
        break;

    // Domain 2
    case PMC_POWER_DOMIAN_2:
#if 0
        // Clear power on status of this module
        uiPMCPD2OnSts &= ~(1 << (Module - PMC_MODULE_PD2));

        pll_enableSystemReset(DSP2_RSTN);

        //pll_disableSystemReset(DSPOCP_RSTN);

        // disable clock
        pll_disableClock(DSP2_CLK);

        Delay_DelayUsPolling(2);
#endif
        // There is no module in power domain 2 is power on, we need to turn off power
        if (uiPMCPD2OnSts == 0)
        {
            // Isolate module's signal output
            RCW_OF(PMC_SIG_REG).SigEn_PD2 = 0;
            RCW_ST(PMC_SIG_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

            // Modules enter reset state
            // Note: These code must be checked with CG -begin
            // Get module reset status
            // uiRstReg3 = pll_readSystemResetMulti(3);

            // pll_writeSystemResetMulti(3, uiRstReg3 & ~PMC_PD1_RST_G3);
            // Note: These code must be checked with CG -end

            // PMC enter reset state
            RCW_OF(PMC_PWRRST_REG).PwrRst_PD2 = 0;
            RCW_ST(PMC_PWRRST_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
             Delay_DelayUsPolling(2);

            // Turn off power domain 2's power
            RCW_OF(PMC_PWROFF_REG).PwrOff_PD2   = 1;
            RCW_ST(PMC_PWROFF_REG);

            // Wait for 1 us
            // Note: We already disable interrupt via loc_cpu()
            Delay_DelayUsPolling(2);

        }
        break;

    // Domain none
    case PMC_POWER_DOMAIN_NONE:
    default:
        break;
    }

    // Leave critical section
    unl_cpu();
}
