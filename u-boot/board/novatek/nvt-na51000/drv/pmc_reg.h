/*
    Register offset and bit definition for PMC module

    Register offset and bit definition for PMC module.

    @file       pmc_reg.h
    @ingroup    mIDrvSys_PMC
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PMC_REG_H
#define _PMC_REG_H

//#include "DrvCommon.h"

/*
    @addtogroup mIDrvSys_PMC
*/
//@{

// PMC Signal Register
REGDEF_OFFSET(PMC_SIG_REG       , 0x00)
REGDEF_BEGIN(PMC_SIG_REG)               // --> Register "PMC_SIG_REG" begin ---
    REGDEF_BIT(SigEn_PD1        , 1)
    REGDEF_BIT(SigEn_PD2        , 1)
    REGDEF_BIT(                 , 1)
    REGDEF_BIT(                 , 29)
REGDEF_END(PMC_SIG_REG)                 // --- Register "PMC_SIG_REG" end   <--

// PMC Power OFF Register
REGDEF_OFFSET(PMC_PWROFF_REG    , 0x04)
REGDEF_BEGIN(PMC_PWROFF_REG)            // --> Register "PMC_PWROFF_REG" begin ---
    REGDEF_BIT(PwrOff_PD1       , 1)
    REGDEF_BIT(PwrOff_PD2       , 1)
    REGDEF_BIT(                 , 1)
    REGDEF_BIT(                 , 29)
REGDEF_END(PMC_PWROFF_REG)              // --- Register "PMC_PWROFF_REG" end   <--

// PMC Power OFF Status Register
REGDEF_OFFSET(PMC_PWRSTS_REG    , 0x08)
REGDEF_BEGIN(PMC_PWRSTS_REG)            // --> Register "PMC_PWRSTS_REG" begin ---
    REGDEF_BIT(PwrSts_PD1       , 1)
    REGDEF_BIT(PwrSts_PD2       , 1)
    REGDEF_BIT(                 , 1)
    REGDEF_BIT(                 , 29)
REGDEF_END(PMC_PWRSTS_REG)              // --- Register "PMC_PWRSTS_REG" end   <--

// PMC Power RST Register
REGDEF_OFFSET(PMC_PWRRST_REG    , 0x0C)
REGDEF_BEGIN(PMC_PWRRST_REG)            // --> Register "PMC_PWRRST_REG" begin ---
    REGDEF_BIT(PwrRst_PD1       , 1)
    REGDEF_BIT(PwrRst_PD2       , 1)
    REGDEF_BIT(                 , 1)
    REGDEF_BIT(                 , 29)
REGDEF_END(PMC_PWRRST_REG)              // --- Register "PMC_PWRRST_REG" end   <--

//@}

#endif
