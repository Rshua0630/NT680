/**
    Header file for PMC module

    This file is the header file that define the API and data type for PMC module.

    @file       pmc.h
    @ingroup    mIDrvSys_PMC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PMC_H
#define _PMC_H

//#include "Driver.h"

#define PMC_MODULE_PD1      0   ///< Power domain 1 starting ID
#define PMC_MODULE_PD2      32  ///< Power domain 2 starting ID
#define PMC_MODULE_ON       128 ///< Power domain always on ID

/**
    @addtogroup mIDrvSys_PMC
*/
//@{

/**
    PMC module ID

    PMC module ID.
*/
typedef enum
{
    PMC_MODULE_DSP         = PMC_MODULE_PD1,    ///< DSP

    PMC_MODULE_DSP2        = PMC_MODULE_PD2,    ///< DSP2

    PMC_MODULE_NONE        = PMC_MODULE_ON,     ///< NONE

    ENUM_DUMMY4WORD(PMC_MODULE)
} PMC_MODULE, *PPMC_MODULE;

#define PMC_MODULE_CPU2      PMC_MODULE_NONE
#define PMC_MODULE_SIE       PMC_MODULE_NONE
#define PMC_MODULE_SIE2      PMC_MODULE_NONE
#define PMC_MODULE_TGE       PMC_MODULE_NONE
#define PMC_MODULE_IPE       PMC_MODULE_NONE
#define PMC_MODULE_DIS       PMC_MODULE_NONE
#define PMC_MODULE_IME       PMC_MODULE_NONE
#define PMC_MODULE_FDE       PMC_MODULE_NONE
#define PMC_MODULE_ISE       PMC_MODULE_NONE
#define PMC_MODULE_IFE       PMC_MODULE_NONE
#define PMC_MODULE_IFE2      PMC_MODULE_NONE
#define PMC_MODULE_DCE       PMC_MODULE_NONE
#define PMC_MODULE_TV        PMC_MODULE_NONE
#define PMC_MODULE_HDMI      PMC_MODULE_NONE
#define PMC_MODULE_AFFINE    PMC_MODULE_NONE
#define PMC_MODULE_JPEG      PMC_MODULE_NONE
#define PMC_MODULE_GRAPHIC   PMC_MODULE_NONE
#define PMC_MODULE_GRAPHIC2  PMC_MODULE_NONE
#define PMC_MODULE_I2C3      PMC_MODULE_NONE
#define PMC_MODULE_MIPI_CSI  PMC_MODULE_NONE
#define PMC_MODULE_MIPI_CSI2 PMC_MODULE_NONE
#define PMC_MODULE_RDE       PMC_MODULE_NONE
#define PMC_MODULE_DRE       PMC_MODULE_NONE
#define PMC_MODULE_VX1       PMC_MODULE_NONE
#define PMC_MODULE_SIE3      PMC_MODULE_NONE
#define PMC_MODULE_SIE4      PMC_MODULE_NONE
#define PMC_MODULE_RHE       PMC_MODULE_NONE
#define PMC_MODULE_SL        PMC_MODULE_NONE
#define PMC_MODULE_SL_REG    PMC_MODULE_NONE
#define PMC_MODULE_H26X      PMC_MODULE_NONE
#define PMC_MODULE_SVM       PMC_MODULE_NONE

#define PMC_MODULE_LVDS      PMC_MODULE_MIPI_CSI   ///< LVDS / HiSPi
#define PMC_MODULE_LVDS2     PMC_MODULE_MIPI_CSI2  ///< LVDS2 / HiSPi2



/**
    PMC module status

    PMC module status.
*/
typedef enum
{
    PMC_PWRSTS_OFF,         ///< Power is OFF
    PMC_PWRSTS_ON,          ///< Power is ON

    ENUM_DUMMY4WORD(PMC_PWRSTS)
} PMC_PWRSTS, *PPMC_PWRSTS;

// PMC public APIs
extern PMC_PWRSTS  pmc_getPWRStatus(PMC_MODULE Module);
extern void pmc_turnonPower(PMC_MODULE Module);
//@}

#endif
