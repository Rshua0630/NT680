/*
    Register offset and bit definition for DSP module

    Register offset and bit definition for DSP module.

    @file       dsp_reg.h
    @ingroup    mIDrvSys_DSP
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DSP_REG_H
#define _DSP_REG_H
#include <asm/arch/rcw_macro.h>

//#include "DrvCommon.h"

/*
    @addtogroup mIDrvSys_DSP
*/
//@{

// DSP Boot Control Register
#define DSP_BOOTCTR_REG_OFS          0x04
REGDEF_BEGIN(DSP_BOOTCTR_REG)
    REGDEF_BIT(DSP_BOOTKEY       , 12)
    REGDEF_BIT(DSP_BOOTVECT      , 20)
REGDEF_END(DSP_BOOTCTR_REG)

// DSP Boot Control Register
#define DSP_DEST0_REG_OFS            0x08
REGDEF_BEGIN(DSP_DEST0_REG)
    REGDEF_BIT(DSP_TO_CPU1       , 1)
    REGDEF_BIT(DSP_TO_CPU2       , 1)
    REGDEF_BIT(DSP_TO_DSP1       , 1)
    REGDEF_BIT(DSP_TO_DSP2       , 1)
    REGDEF_BIT(                  , 28)
REGDEF_END(DSP_DEST0_REG)

// DSP Interupt Status CPU1 Register
#define DSP_CPU1_INT_STS0_REG_OFS    0x10
REGDEF_BEGIN(DSP_CPU1_INT_STS0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_CPU1_INT_STS0_REG)

// DSP Interupt Status Register
#define DSP_CPU1_INT_STS1_REG_OFS    0x14
REGDEF_BEGIN(DSP_CPU1_INT_STS1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_CPU1_INT_STS1_REG)


// DSP Interupt Status0 CPU2 Register
#define DSP_CPU2_INT_STS0_REG_OFS    0x20
REGDEF_BEGIN(DSP_CPU2_INT_STS0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_CPU2_INT_STS0_REG)

// DSP Interupt Status1 CPU2 Register
#define DSP_CPU2_INT_STS1_REG_OFS    0x24
REGDEF_BEGIN(DSP_CPU2_INT_STS1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_CPU2_INT_STS1_REG)

// DSP Interupt Status0 DSP1 Register
#define DSP_DSP1_INT_STS0_REG_OFS    0x30
REGDEF_BEGIN(DSP_DSP1_INT_STS0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_DSP1_INT_STS0_REG)

// DSP Interupt Status1 DSP1 Register
#define DSP_DSP1_INT_STS1_REG_OFS    0x34
REGDEF_BEGIN(DSP_DSP1_INT_STS1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_DSP1_INT_STS1_REG)


// DSP Interupt Status0 DSP2 Register
#define DSP_DSP2_INT_STS0_REG_OFS    0x40
REGDEF_BEGIN(DSP_DSP2_INT_STS0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_DSP2_INT_STS0_REG)

// DSP Interupt Status1 DSP2 Register
#define DSP_DSP2_INT_STS1_REG_OFS    0x44
REGDEF_BEGIN(DSP_DSP2_INT_STS1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_DSP2_INT_STS1_REG)


// DSP Interupt enable CPU1 Register
#define DSP_CPU1_INT_EN0_REG_OFS     0x60
REGDEF_BEGIN(DSP_CPU1_INT_EN0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_CPU1_INT_EN0_REG)

// DSP Interupt enable Register
#define DSP_CPU1_INT_EN1_REG_OFS     0x64
REGDEF_BEGIN(DSP_CPU1_INT_EN1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_CPU1_INT_EN1_REG)

// DSP Interupt enable CPU2 Register
#define DSP_CPU2_INT_EN0_REG_OFS     0x70
REGDEF_BEGIN(DSP_CPU2_INT_EN0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_CPU2_INT_EN0_REG)

// DSP Interupt enable Register
#define DSP_CPU2_INT_EN1_REG_OFS     0x74
REGDEF_BEGIN(DSP_CPU2_INT_EN1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_CPU2_INT_EN1_REG)

// DSP Interupt enable DSP1 Register
#define DSP_DSP1_INT_EN0_REG_OFS     0x80
REGDEF_BEGIN(DSP_DSP1_INT_EN0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_DSP1_INT_EN0_REG)

// DSP Interupt enable Register
#define DSP_DSP1_INT_EN1_REG_OFS     0x84
REGDEF_BEGIN(DSP_DSP1_INT_EN1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_DSP1_INT_EN1_REG)

// DSP Interupt enable CPU2 Register
#define DSP_DSP2_INT_EN0_REG_OFS     0x90
REGDEF_BEGIN(DSP_DSP2_INT_EN0_REG)
    REGDEF_BIT(DSP_GOES_STANDBY  , 1)
    REGDEF_BIT(DSP_ERROR         , 1)
    REGDEF_BIT(DSP_DDMA_DBG_MATCH, 1)
    REGDEF_BIT(DSP_PI            , 1)
    REGDEF_BIT(DSP_PSU_MAPV      , 1)
    REGDEF_BIT(DSP_OCEM_DBG      , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_DSP2_INT_EN0_REG)

// DSP Interupt enable Register
#define DSP_DSP2_INT_EN1_REG_OFS     0x94
REGDEF_BEGIN(DSP_DSP2_INT_EN1_REG)
    REGDEF_BIT(DSP_RD_IND        , 32)
REGDEF_END(DSP_DSP2_INT_EN1_REG)


// DSP control Register0
#define DSP_CTL0_REG_OFS             0x100
REGDEF_BEGIN(DSP_CTL0_REG)
    REGDEF_BIT(DSP_RESET_MODE    , 1)
    REGDEF_BIT(                  , 7)
    REGDEF_BIT(DSP_CACHE_INVALID , 1)
    REGDEF_BIT(                  , 1)
    REGDEF_BIT(                  , 6)
    REGDEF_BIT(DSP_STS_REG       , 2)
    REGDEF_BIT(DSP_OM_REG        , 2)
    REGDEF_BIT(                  , 12)
REGDEF_END(DSP_CTL0_REG)

// DSP control Register1
#define DSP_CTL1_REG_OFS             0x104
REGDEF_BEGIN(DSP_CTL1_REG)
    REGDEF_BIT(DSP_DDMA_MATCH_ACK, 1)
    REGDEF_BIT(DSP_ACU_LOCK      , 1)
    REGDEF_BIT(DSP_SLV_ACC       , 1)
    REGDEF_BIT(                  , 29)
REGDEF_END(DSP_CTL1_REG)

// DSP control Register2
#define DSP_CTL2_REG_OFS             0x108
REGDEF_BEGIN(DSP_CTL2_REG)
    REGDEF_BIT(DSP_JP_AP_KEY     , 12)
    REGDEF_BIT(DSP_JP_AP         , 1)
    REGDEF_BIT(DSP_EXT_WAIT      , 1)
    REGDEF_BIT(DSP_TCLK_INV      , 1)
    REGDEF_BIT(DSP_EPP_RDATA_ZERO, 1)
    REGDEF_BIT(                  , 16)
REGDEF_END(DSP_CTL2_REG)

// DSP control Register3
#define DSP_CTL3_REG_OFS             0x10C
REGDEF_BEGIN(DSP_CTL3_REG)
    REGDEF_BIT(DSP_CORE_RSTN     , 1)
    REGDEF_BIT(DSP_SYS_RSTN      , 1)
    REGDEF_BIT(DSP_OCEM_RSTN     , 1)
    REGDEF_BIT(DSP_GLOBAL_RSTN   , 1)
    REGDEF_BIT(DSP_OCEM_APB_BRG_RSTN, 1)
    REGDEF_BIT(                  , 27)
REGDEF_END(DSP_CTL3_REG)

// DSP Error Status
#define DSP_ERR_REG_OFS              0x110
REGDEF_BEGIN(DSP_ERR_REG)
    REGDEF_BIT(DSP_ERR_WDOG_EPP  , 1)
    REGDEF_BIT(DSP_ERR_WDOG_EDP  , 1)
    REGDEF_BIT(                  , 1)
    REGDEF_BIT(DSP_ERR_WDOG_IOP  , 1)
    REGDEF_BIT(DSP_ERR_UOP       , 1)
    REGDEF_BIT(DSP_ERR_GVI       , 1)
    REGDEF_BIT(                  , 26)
REGDEF_END(DSP_ERR_REG)

// DSP DBG Control
#define DSP_DBG_CTR_REG_OFS          0x114
REGDEF_BEGIN(DSP_DBG_CTR_REG)
    REGDEF_BIT(DSP2_DSP_DEBUG_EN  , 1)
    REGDEF_BIT(CPU_DSP_DEBUG_EN   , 1)
    REGDEF_BIT(CPU2_DSP_DEBUG_EN  , 1)
    REGDEF_BIT(                   , 29)
REGDEF_END(DSP_DBG_CTR_REG)

// DSP AXI BRG
#define DSP_AXIBRG_REG_OFS          0x118
REGDEF_BEGIN(DSP_AXIBRG_REG)
    REGDEF_BIT(DSP_EPP_POSTD       , 1)
    REGDEF_BIT(DSP_EPP_PAWBA_MODE  , 1)
    REGDEF_BIT(DSP_EPP_AWBA_SET    , 1)
    REGDEF_BIT(DSP_EPP_PPRE_RD_OFF , 1)
    REGDEF_BIT(                    , 4)
    REGDEF_BIT(DSP_EDP_POSTD       , 1)
    REGDEF_BIT(DSP_EDP_PAWBA_MODE  , 1)
    REGDEF_BIT(DSP_EDP_AWBA_SET    , 1)
    REGDEF_BIT(DSP_EDP_PPRE_RD_OFF , 1)
    REGDEF_BIT(DSP_OCEM_APB_BRG_WR_BUF_EN ,1)
    REGDEF_BIT(                    , 19)
REGDEF_END(DSP_AXIBRG_REG)

// DSP DBG signal Control
#define DSP_DBG_SIG_REG_OFS         0x11C
REGDEF_BEGIN(DSP_DBG_SIG_REG)
    REGDEF_BIT(DBG_SEL            , 4)
    REGDEF_BIT(                   , 4)
    REGDEF_BIT(DBG_EN             , 1)
    REGDEF_BIT(                   , 23)
REGDEF_END(DSP_DBG_SIG_REG)

// DSP PC Register
#define DSP_PC_REG_OFS              0x120
REGDEF_BEGIN(DSP_PC_REG)
    REGDEF_BIT(DSP_PC             , 32)
REGDEF_END(DSP_PC_REG)

// DSP DUMMY Register
#define DSP_DUMMY_REG_OFS           0x124
REGDEF_BEGIN(DSP_DUMMY_REG)
    REGDEF_BIT(DSP_DUMMY           , 32)
REGDEF_END(DSP_DUMMY_REG)


//@}

#endif
