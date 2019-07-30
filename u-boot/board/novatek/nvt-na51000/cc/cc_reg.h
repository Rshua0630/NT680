/**

    Register offset and bit definition for CC module

    Register offset and bit definition for CC module.

    @file       cc_reg.h
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef _CC_REG_H
#define _CC_REG_H

// DSP2 Action Register
REGDEF_OFFSET(CC_DSP2ACT_REG         , 0x00)
REGDEF_BEGIN(CC_DSP2ACT_REG)             // --> Register "CC_DSPACT_REG" begin ---
    REGDEF_BIT(TriggerCPU1          , 1)
    REGDEF_BIT(TriggerCPU2          , 1)
    REGDEF_BIT(TriggerDSP           , 1)
    REGDEF_BIT(TriggerDSP2          , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckCPU1              , 1)
    REGDEF_BIT(AckCPU2              , 1)
    REGDEF_BIT(AckDSP               , 1)
    REGDEF_BIT(AckDSP2              , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_DSP2ACT_REG)               // --- Register "CC_DSPACT_REG" end   <--

// DSP2 Status Register
REGDEF_OFFSET(CC_DSP2STS_REG        , 0x04)
REGDEF_BEGIN(CC_DSP2STS_REG)             // --> Register "CC_DSPSTS_REG" begin ---
    REGDEF_BIT(EventFromCPU1        , 1)
    REGDEF_BIT(EventFromCPU2        , 1)
    REGDEF_BIT(EventFromDSP         , 1)
    REGDEF_BIT(EventFromDSP2        , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1          , 1)
    REGDEF_BIT(AckFromCPU2          , 1)
    REGDEF_BIT(AckFromDSP           , 1)
    REGDEF_BIT(AckFromDSP2          , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_DSP2STS_REG)               // --- Register "CC_DSPSTS_REG" end   <--

// DSP2 Interrupt Enable Register
REGDEF_OFFSET(CC_DSP2INTEN_REG      , 0x08)
REGDEF_BEGIN(CC_DSP2INTEN_REG)           // --> Register "CC_DSPINTEN_REG" begin ---
    REGDEF_BIT(EventFromCPU1_IntEn  , 1)
    REGDEF_BIT(EventFromCPU2_IntEn  , 1)
    REGDEF_BIT(EventFromDSP_IntEn   , 1)
    REGDEF_BIT(EventFromDSP2_IntEn  , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1_IntEn    , 1)
    REGDEF_BIT(AckFromCPU2_IntEn    , 1)
    REGDEF_BIT(AckFromDSP_IntEn     , 1)
    REGDEF_BIT(AckFromDSP2_IntEn    , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_DSP2INTEN_REG)             // --- Register "CC_DSPINTEN_REG" end   <--

// Reserved (0x0C)

// CPU1 Action Register
REGDEF_OFFSET(CC_CPU1ACT_REG        , 0x10)
REGDEF_BEGIN(CC_CPU1ACT_REG)            // --> Register "CC_CPU1ACT_REG" begin ---
    REGDEF_BIT(TriggerCPU1          , 1)
    REGDEF_BIT(TriggerCPU2          , 1)
    REGDEF_BIT(TriggerDSP           , 1)
    REGDEF_BIT(TriggerDSP2          , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckCPU1              , 1)
    REGDEF_BIT(AckCPU2              , 1)
    REGDEF_BIT(AckDSP               , 1)
    REGDEF_BIT(AckDSP2              , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_CPU1ACT_REG)              // --- Register "CC_CPU1ACT_REG" end   <--

// CPU1 Status Register
REGDEF_OFFSET(CC_CPU1STS_REG        , 0x14)
REGDEF_BEGIN(CC_CPU1STS_REG)            // --> Register "CC_CPU1STS_REG" begin ---
    REGDEF_BIT(EventFromCPU1        , 1)
    REGDEF_BIT(EventFromCPU2        , 1)
    REGDEF_BIT(EventFromDSP         , 1)
    REGDEF_BIT(EventFromDSP2        , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1          , 1)
    REGDEF_BIT(AckFromCPU2          , 1)
    REGDEF_BIT(AckFromDSP           , 1)
    REGDEF_BIT(AckFromDSP2          , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_CPU1STS_REG)              // --- Register "CC_CPU1STS_REG" end   <--

// CPU1 Interrupt Enable Register
REGDEF_OFFSET(CC_CPU1INTEN_REG      , 0x18)
REGDEF_BEGIN(CC_CPU1INTEN_REG)          // --> Register "CC_CPU1INTEN_REG" begin ---
    REGDEF_BIT(EventFromCPU1_IntEn  , 1)
    REGDEF_BIT(EventFromCPU2_IntEn  , 1)
    REGDEF_BIT(EventFromDSP_IntEn   , 1)
    REGDEF_BIT(EventFromDSP2_IntEn  , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1_IntEn    , 1)
    REGDEF_BIT(AckFromCPU2_IntEn    , 1)
    REGDEF_BIT(AckFromDSP_IntEn     , 1)
    REGDEF_BIT(AckFromDSP2_IntEn    , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_CPU1INTEN_REG)            // --- Register "CC_CPU1INTEN_REG" end   <--

// Reserved (0x1C)


// CPU2 Action Register
REGDEF_OFFSET(CC_CPU2ACT_REG        , 0x20)
REGDEF_BEGIN(CC_CPU2ACT_REG)            // --> Register "CC_CPU2ACT_REG" begin ---
    REGDEF_BIT(TriggerCPU1          , 1)
    REGDEF_BIT(TriggerCPU2          , 1)
    REGDEF_BIT(TriggerDSP           , 1)
    REGDEF_BIT(TriggerDSP2          , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckCPU1              , 1)
    REGDEF_BIT(AckCPU2              , 1)
    REGDEF_BIT(AckDSP               , 1)
    REGDEF_BIT(AckDSP2              , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_CPU2ACT_REG)              // --- Register "CC_CPU2ACT_REG" end   <--

// CPU2 Status Register
REGDEF_OFFSET(CC_CPU2STS_REG        , 0x24)
REGDEF_BEGIN(CC_CPU2STS_REG)            // --> Register "CC_CPU2STS_REG" begin ---
    REGDEF_BIT(EventFromCPU1        , 1)
    REGDEF_BIT(EventFromCPU2        , 1)
    REGDEF_BIT(EventFromDSP         , 1)
    REGDEF_BIT(EventFromDSP2        , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1          , 1)
    REGDEF_BIT(AckFromCPU2          , 1)
    REGDEF_BIT(AckFromDSP           , 1)
    REGDEF_BIT(AckFromDSP2          , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_CPU2STS_REG)              // --- Register "CC_CPU2STS_REG" end   <--

// CPU1 Interrupt Enable Register
REGDEF_OFFSET(CC_CPU2INTEN_REG      , 0x28)
REGDEF_BEGIN(CC_CPU2INTEN_REG)          // --> Register "CC_CPU2INTEN_REG" begin ---
    REGDEF_BIT(EventFromCPU1_IntEn  , 1)
    REGDEF_BIT(EventFromCPU2_IntEn  , 1)
    REGDEF_BIT(EventFromDSP_IntEn   , 1)
    REGDEF_BIT(EventFromDSP2_IntEn   , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1_IntEn    , 1)
    REGDEF_BIT(AckFromCPU2_IntEn    , 1)
    REGDEF_BIT(AckFromDSP_IntEn     , 1)
    REGDEF_BIT(AckFromDSP2_IntEn     , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_CPU2INTEN_REG)            // --- Register "CC_CPU2INTEN_REG" end   <--

// Reserved (0x2C)

// DSP Action Register
REGDEF_OFFSET(CC_DSPACT_REG         , 0x30)
REGDEF_BEGIN(CC_DSPACT_REG)             // --> Register "CC_DSPACT_REG" begin ---
    REGDEF_BIT(TriggerCPU1          , 1)
    REGDEF_BIT(TriggerCPU2          , 1)
    REGDEF_BIT(TriggerDSP           , 1)
    REGDEF_BIT(TriggerDSP2          , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckCPU1              , 1)
    REGDEF_BIT(AckCPU2              , 1)
    REGDEF_BIT(AckDSP               , 1)
    REGDEF_BIT(AckDSP2              , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_DSPACT_REG)               // --- Register "CC_DSPACT_REG" end   <--

// DSP Status Register
REGDEF_OFFSET(CC_DSPSTS_REG        , 0x34)
REGDEF_BEGIN(CC_DSPSTS_REG)             // --> Register "CC_DSPSTS_REG" begin ---
    REGDEF_BIT(EventFromCPU1        , 1)
    REGDEF_BIT(EventFromCPU2        , 1)
    REGDEF_BIT(EventFromDSP         , 1)
    REGDEF_BIT(EventFromDSP2        , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1          , 1)
    REGDEF_BIT(AckFromCPU2          , 1)
    REGDEF_BIT(AckFromDSP           , 1)
    REGDEF_BIT(AckFromDSP2          , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_DSPSTS_REG)               // --- Register "CC_DSPSTS_REG" end   <--

// DSP Interrupt Enable Register
REGDEF_OFFSET(CC_DSPINTEN_REG      , 0x38)
REGDEF_BEGIN(CC_DSPINTEN_REG)           // --> Register "CC_DSPINTEN_REG" begin ---
    REGDEF_BIT(EventFromCPU1_IntEn  , 1)
    REGDEF_BIT(EventFromCPU2_IntEn  , 1)
    REGDEF_BIT(EventFromDSP_IntEn   , 1)
    REGDEF_BIT(EventFromDSP2_IntEn  , 1)
    REGDEF_BIT(                     , 12)
    REGDEF_BIT(AckFromCPU1_IntEn    , 1)
    REGDEF_BIT(AckFromCPU2_IntEn    , 1)
    REGDEF_BIT(AckFromDSP_IntEn     , 1)
    REGDEF_BIT(AckFromDSP2_IntEn    , 1)
    REGDEF_BIT(                     , 12)
REGDEF_END(CC_DSPINTEN_REG)             // --- Register "CC_DSPINTEN_REG" end   <--

// Reserved (0x3C)

// CPU1 Request HW Resource Register
REGDEF_OFFSET(CC_CPU1REQ_REG        , 0x40)
REGDEF_BEGIN(CC_CPU1REQ_REG)            // --> Register "CC_CPU1REQ_REG" begin ---
    REGDEF_BIT(Request              , 32)
REGDEF_END(CC_CPU1REQ_REG)            // --- Register "CC_CPU1REQ_REG" end   <--

// CPU1 Release HW Resource Register
REGDEF_OFFSET(CC_CPU1REL_REG        , 0x44)
REGDEF_BEGIN(CC_CPU1REL_REG)            // --> Register "CC_CPU1REL_REG" begin ---
    REGDEF_BIT(Release              , 32)
REGDEF_END(CC_CPU1REL_REG)              // --- Register "CC_CPU1REL_REG" end   <--

// CPU1 Grant HW Resource Register
REGDEF_OFFSET(CC_CPU1GRANT_REG      , 0x48)
REGDEF_BEGIN(CC_CPU1GRANT_REG)          // --> Register "CC_CPU1GRANT_REG" begin ---
    REGDEF_BIT(Grant                , 32)
REGDEF_END(CC_CPU1GRANT_REG)            // --- Register "CC_CPU1GRANT_REG" end   <--

// CPU1 HW Resource Flag Register
REGDEF_OFFSET(CC_CPU1FLG_REG        , 0x4C)
REGDEF_BEGIN(CC_CPU1FLG_REG)            // --> Register "CC_CPU1FLG_REG" begin ---
    REGDEF_BIT(Flag                 , 32)
REGDEF_END(CC_CPU1FLG_REG)              // --- Register "CC_CPU1FLG_REG" end   <--

// CPU2 Request HW Resource Register
REGDEF_OFFSET(CC_CPU2REQ_REG        , 0x50)
REGDEF_BEGIN(CC_CPU2REQ_REG)            // --> Register "CC_CPU2REQ_REG" begin ---
    REGDEF_BIT(Request              , 32)
REGDEF_END(CC_CPU2REQ_REG)            // --- Register "CC_CPU2REQ_REG" end   <--

// CPU2 Release HW Resource Register
REGDEF_OFFSET(CC_CPU2REL_REG        , 0x54)
REGDEF_BEGIN(CC_CPU2REL_REG)            // --> Register "CC_CPU2REL_REG" begin ---
    REGDEF_BIT(Release              , 32)
REGDEF_END(CC_CPU2REL_REG)              // --- Register "CC_CPU2REL_REG" end   <--

// CPU2 Grant HW Resource Register
REGDEF_OFFSET(CC_CPU2GRANT_REG      , 0x58)
REGDEF_BEGIN(CC_CPU2GRANT_REG)          // --> Register "CC_CPU2GRANT_REG" begin ---
    REGDEF_BIT(Grant                , 32)
REGDEF_END(CC_CPU2GRANT_REG)            // --- Register "CC_CPU2GRANT_REG" end   <--

// CPU2 HW Resource Flag Register
REGDEF_OFFSET(CC_CPU2FLG_REG        , 0x5C)
REGDEF_BEGIN(CC_CPU2FLG_REG)            // --> Register "CC_CPU2FLG_REG" begin ---
    REGDEF_BIT(Flag                 , 32)
REGDEF_END(CC_CPU2FLG_REG)              // --- Register "CC_CPU2FLG_REG" end   <--

// DSP Request HW Resource Register
REGDEF_OFFSET(CC_DSPREQ_REG         , 0x60)
REGDEF_BEGIN(CC_DSPREQ_REG)             // --> Register "CC_DSPREQ_REG" begin ---
    REGDEF_BIT(Request              , 32)
REGDEF_END(CC_DSPREQ_REG)               // --- Register "CC_DSPREQ_REG" end   <--

// DSP Release HW Resource Register
REGDEF_OFFSET(CC_DSPREL_REG         , 0x64)
REGDEF_BEGIN(CC_DSPREL_REG)             // --> Register "CC_DSPREL_REG" begin ---
    REGDEF_BIT(Release              , 32)
REGDEF_END(CC_DSPREL_REG)               // --- Register "CC_DSPREL_REG" end   <--

// DSP Grant HW Resource Register
REGDEF_OFFSET(CC_DSPGRANT_REG      , 0x68)
REGDEF_BEGIN(CC_DSPGRANT_REG)           // --> Register "CC_DSPGRANT_REG" begin ---
    REGDEF_BIT(Grant                , 32)
REGDEF_END(CC_DSPGRANT_REG)             // --- Register "CC_DSPGRANT_REG" end   <--

// DSP HW Resource Flag Register
REGDEF_OFFSET(CC_DSPFLG_REG        , 0x6C)
REGDEF_BEGIN(CC_DSPFLG_REG)             // --> Register "CC_DSPFLG_REG" begin ---
    REGDEF_BIT(Flag                 , 32)
REGDEF_END(CC_DSPFLG_REG)               // --- Register "CC_DSPFLG_REG" end   <--

// DSP2 Request HW Resource Register
REGDEF_OFFSET(CC_DSP2REQ_REG        , 0x70)
REGDEF_BEGIN(CC_DSP2REQ_REG)             // --> Register "CC_DSPREQ_REG" begin ---
    REGDEF_BIT(Request              , 32)
REGDEF_END(CC_DSP2REQ_REG)               // --- Register "CC_DSPREQ_REG" end   <--

// DSP2 Release HW Resource Register
REGDEF_OFFSET(CC_DSP2REL_REG        , 0x74)
REGDEF_BEGIN(CC_DSP2REL_REG)            // --> Register "CC_DSPREL_REG" begin ---
    REGDEF_BIT(Release              , 32)
REGDEF_END(CC_DSP2REL_REG)              // --- Register "CC_DSPREL_REG" end   <--

// DSP2 Grant HW Resource Register
REGDEF_OFFSET(CC_DSP2GRANT_REG     , 0x78)
REGDEF_BEGIN(CC_DSP2GRANT_REG)           // --> Register "CC_DSPGRANT_REG" begin ---
    REGDEF_BIT(Grant                , 32)
REGDEF_END(CC_DSP2GRANT_REG)             // --- Register "CC_DSPGRANT_REG" end   <--

// DSP2 HW Resource Flag Register
REGDEF_OFFSET(CC_DSP2FLG_REG        , 0x7C)
REGDEF_BEGIN(CC_DSP2FLG_REG)             // --> Register "CC_DSPFLG_REG" begin ---
    REGDEF_BIT(Flag                 , 32)
REGDEF_END(CC_DSP2FLG_REG)               // --- Register "CC_DSPFLG_REG" end   <--

// Software Reset Register
REGDEF_OFFSET(CC_SWRST_REG          , 0x80)
REGDEF_BEGIN(CC_SWRST_REG)              // --> Register "CC_SWRST_REG" begin ---
    REGDEF_BIT(SWReset              , 1)
    REGDEF_BIT(                     , 31)
REGDEF_END(CC_SWRST_REG)                // --- Register "CC_SWRST_REG" end   <--

// Reserved (0x84 ~ 0x8C)

// MIPS Control Register
REGDEF_OFFSET(CC_MIPSCTRL_REG       , 0x90)
REGDEF_BEGIN(CC_MIPSCTRL_REG)           // --> Register "CC_MIPSCTRL_REG" begin ---
    REGDEF_BIT(Reorder              , 1)
    REGDEF_BIT(MultiAPBCMD          , 1)
    REGDEF_BIT(MultiDMACMD          , 1)
    REGDEF_BIT(                     , 29)
REGDEF_END(CC_MIPSCTRL_REG)             // --- Register "CC_MIPSCTRL_REG" end   <--

// MIPS2 Control Register
REGDEF_OFFSET(CC_MIPS2CTRL_REG      , 0x94)
REGDEF_BEGIN(CC_MIPS2CTRL_REG)          // --> Register "CC_MIPS2CTRL_REG" begin ---
    REGDEF_BIT(Reorder              , 1)
    REGDEF_BIT(MultiAPBCMD          , 1)
    REGDEF_BIT(MultiDMACMD          , 1)
    REGDEF_BIT(                     , 29)
REGDEF_END(CC_MIPS2CTRL_REG)            // --- Register "CC_MIPS2CTRL_REG" end   <--

// DSP Control and Status Register
REGDEF_OFFSET(CC_DSPCTRL_REG        , 0x98)
REGDEF_BEGIN(CC_DSPCTRL_REG)            // --> Register "CC_DSPCTRL_REG" begin ---
    REGDEF_BIT(ResetMode            , 1)
    REGDEF_BIT(MultiEPPCMD          , 1)
    REGDEF_BIT(MultiEDPCMD          , 1)
    REGDEF_BIT(                     , 13)
    REGDEF_BIT(DSPStatus            , 2)
    REGDEF_BIT(CPU2Sleep            , 1)
    REGDEF_BIT(                     , 13)
REGDEF_END(CC_DSPCTRL_REG)              // --- Register "CC_DSPCTRL_REG" end   <--

// DSP Error Status Register
REGDEF_OFFSET(CC_DSPERR_REG        , 0x9C)
REGDEF_BEGIN(CC_DSPERR_REG)             // --> Register "CC_DSPERR_REG" begin ---
    REGDEF_BIT(EPPInvalidAddr       , 1)
    REGDEF_BIT(EDPInvalidAddr       , 1)
    REGDEF_BIT(                     , 30)
REGDEF_END(CC_DSPERR_REG)               // --- Register "CC_DSPERR_REG" end   <--

// Reserved (0xA0 ~ 0xDC)

// Debug (0xE0 ~ 0xEC)

// Reserved (0xF0 ~ 0xFC)

// Command Buffer Register (0x100 ~ 0x144)
REGDEF_OFFSET(CC_CPU1_CPU2_CMDBUF_REG1  , 0x100)
REGDEF_BEGIN(CC_CPU1_CPU2_CMDBUF_REG1)             // --> Register "CC_CPU1_CPU2_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_CPU1_CPU2_CMDBUF_REG1)               // --- Register "CC_CPU1_CPU2_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_CPU1_CPU2_CMDBUF_REG2  , 0x104)
REGDEF_BEGIN(CC_CPU1_CPU2_CMDBUF_REG2)             // --> Register "CC_CPU1_CPU2_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_CPU1_CPU2_CMDBUF_REG2)               // --- Register "CC_CPU1_CPU2_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_CPU1_CPU2_CMDBUF_REG3  , 0x108)
REGDEF_BEGIN(CC_CPU1_CPU2_CMDBUF_REG3)             // --> Register "CC_CPU1_CPU2_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_CPU1_CPU2_CMDBUF_REG3)               // --- Register "CC_CPU1_CPU2_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_CPU1_DSP_CMDBUF_REG1  , 0x10C)
REGDEF_BEGIN(CC_CPU1_DSP_CMDBUF_REG1)             // --> Register "CC_CPU1_DSP_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_CPU1_DSP_CMDBUF_REG1)               // --- Register "CC_CPU1_DSP_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_CPU1_DSP_CMDBUF_REG2  , 0x110)
REGDEF_BEGIN(CC_CPU1_DSP_CMDBUF_REG2)             // --> Register "CC_CPU1_DSP_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_CPU1_DSP_CMDBUF_REG2)               // --- Register "CC_CPU1_DSP_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_CPU1_DSP_CMDBUF_REG3  , 0x114)
REGDEF_BEGIN(CC_CPU1_DSP_CMDBUF_REG3)             // --> Register "CC_CPU1_DSP_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_CPU1_DSP_CMDBUF_REG3)               // --- Register "CC_CPU1_DSP_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_CPU2_CPU1_CMDBUF_REG1  , 0x118)
REGDEF_BEGIN(CC_CPU2_CPU1_CMDBUF_REG1)             // --> Register "CC_CPU2_CPU1_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_CPU2_CPU1_CMDBUF_REG1)               // --- Register "CC_CPU2_CPU1_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_CPU2_CPU1_CMDBUF_REG2  , 0x11C)
REGDEF_BEGIN(CC_CPU2_CPU1_CMDBUF_REG2)             // --> Register "CC_CPU2_CPU1_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_CPU2_CPU1_CMDBUF_REG2)               // --- Register "CC_CPU2_CPU1_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_CPU2_CPU1_CMDBUF_REG3  , 0x120)
REGDEF_BEGIN(CC_CPU2_CPU1_CMDBUF_REG3)             // --> Register "CC_CPU2_CPU1_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_CPU2_CPU1_CMDBUF_REG3)               // --- Register "CC_CPU2_CPU1_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_CPU2_DSP_CMDBUF_REG1  , 0x124)
REGDEF_BEGIN(CC_CPU2_DSP_CMDBUF_REG1)             // --> Register "CC_CPU2_DSP_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_CPU2_DSP_CMDBUF_REG1)               // --- Register "CC_CPU2_DSP_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_CPU2_DSP_CMDBUF_REG2  , 0x128)
REGDEF_BEGIN(CC_CPU2_DSP_CMDBUF_REG2)             // --> Register "CC_CPU2_DSP_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_CPU2_DSP_CMDBUF_REG2)               // --- Register "CC_CPU2_DSP_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_CPU2_DSP_CMDBUF_REG3  , 0x12C)
REGDEF_BEGIN(CC_CPU2_DSP_CMDBUF_REG3)             // --> Register "CC_CPU2_DSP_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_CPU2_DSP_CMDBUF_REG3)               // --- Register "CC_CPU2_DSP_CMDBUF_REG3" end   <--


REGDEF_OFFSET(CC_DSP_CPU1_CMDBUF_REG1  , 0x130)
REGDEF_BEGIN(CC_DSP_CPU1_CMDBUF_REG1)             // --> Register "CC_DSP_CPU1_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_DSP_CPU1_CMDBUF_REG1)               // --- Register "CC_DSP_CPU1_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_DSP_CPU1_CMDBUF_REG2  , 0x134)
REGDEF_BEGIN(CC_DSP_CPU1_CMDBUF_REG2)             // --> Register "CC_DSP_CPU1_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_DSP_CPU1_CMDBUF_REG2)               // --- Register "CC_DSP_CPU1_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_DSP_CPU1_CMDBUF_REG3  , 0x138)
REGDEF_BEGIN(CC_DSP_CPU1_CMDBUF_REG3)             // --> Register "CC_DSP_CPU1_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_DSP_CPU1_CMDBUF_REG3)               // --- Register "CC_DSP_CPU1_CMDBUF_REG3" end   <--


REGDEF_OFFSET(CC_DSP_CPU2_CMDBUF_REG1  , 0x13C)
REGDEF_BEGIN(CC_DSP_CPU2_CMDBUF_REG1)             // --> Register "CC_DSP_CPU2_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_DSP_CPU2_CMDBUF_REG1)               // --- Register "CC_DSP_CPU2_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_DSP_CPU2_CMDBUF_REG2  , 0x140)
REGDEF_BEGIN(CC_DSP_CPU2_CMDBUF_REG2)             // --> Register "CC_DSP_CPU2_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_DSP_CPU2_CMDBUF_REG2)               // --- Register "CC_DSP_CPU2_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_DSP_CPU2_CMDBUF_REG3  , 0x144)
REGDEF_BEGIN(CC_DSP_CPU2_CMDBUF_REG3)             // --> Register "CC_DSP_CPU2_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_DSP_CPU2_CMDBUF_REG3)               // --- Register "CC_DSP_CPU2_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_CPU1_DSP2_CMDBUF_REG1  , 0x148)
REGDEF_BEGIN(CC_CPU1_DSP2_CMDBUF_REG1)             // --> Register "CC_CPU1_DSP2_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_CPU1_DSP2_CMDBUF_REG1)               // --- Register "CC_CPU1_DSP2_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_CPU1_DSP2_CMDBUF_REG2  , 0x14C)
REGDEF_BEGIN(CC_CPU1_DSP2_CMDBUF_REG2)             // --> Register "CC_CPU1_DSP2_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_CPU1_DSP2_CMDBUF_REG2)               // --- Register "CC_CPU1_DSP2_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_CPU1_DSP2_CMDBUF_REG3  , 0x150)
REGDEF_BEGIN(CC_CPU1_DSP2_CMDBUF_REG3)             // --> Register "CC_CPU1_DSP2_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_CPU1_DSP2_CMDBUF_REG3)               // --- Register "CC_CPU1_DSP2_CMDBUF_REG3" end   <--


REGDEF_OFFSET(CC_CPU2_DSP2_CMDBUF_REG1  , 0x154)
REGDEF_BEGIN(CC_CPU2_DSP2_CMDBUF_REG1)             // --> Register "CC_CPU2_DSP2_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_CPU2_DSP2_CMDBUF_REG1)               // --- Register "CC_CPU2_DSP2_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_CPU2_DSP2_CMDBUF_REG2  , 0x158)
REGDEF_BEGIN(CC_CPU2_DSP2_CMDBUF_REG2)             // --> Register "CC_CPU2_DSP2_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_CPU2_DSP2_CMDBUF_REG2)               // --- Register "CC_CPU2_DSP2_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_CPU2_DSP2_CMDBUF_REG3  , 0x15C)
REGDEF_BEGIN(CC_CPU2_DSP2_CMDBUF_REG3)             // --> Register "CC_CPU2_DSP2_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_CPU2_DSP2_CMDBUF_REG3)               // --- Register "CC_CPU2_DSP2_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_DSP_DSP2_CMDBUF_REG1  , 0x160)
REGDEF_BEGIN(CC_DSP_DSP2_CMDBUF_REG1)             // --> Register "CC_DSP_DSP2_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_DSP_DSP2_CMDBUF_REG1)               // --- Register "CC_DSP_DSP2_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_DSP_DSP2_CMDBUF_REG2  , 0x164)
REGDEF_BEGIN(CC_DSP_DSP2_CMDBUF_REG2)             // --> Register "CC_DSP_DSP2_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_DSP_DSP2_CMDBUF_REG2)               // --- Register "CC_DSP_DSP2_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_DSP_DSP2_CMDBUF_REG3  , 0x168)
REGDEF_BEGIN(CC_DSP_DSP2_CMDBUF_REG3)             // --> Register "CC_DSP_DSP2_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_DSP_DSP2_CMDBUF_REG3)               // --- Register "CC_DSP_DSP2_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_DSP2_CPU1_CMDBUF_REG1  , 0x16C)
REGDEF_BEGIN(CC_DSP2_CPU1_CMDBUF_REG1)             // --> Register "CC_DSP2_CPU1_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_DSP2_CPU1_CMDBUF_REG1)               // --- Register "CC_DSP2_CPU1_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_DSP2_CPU1_CMDBUF_REG2  , 0x170)
REGDEF_BEGIN(CC_DSP2_CPU1_CMDBUF_REG2)             // --> Register "CC_DSP2_CPU1_CMDBUF_REG2" begin ---
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_DSP2_CPU1_CMDBUF_REG2)               // --- Register "CC_DSP2_CPU1_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_DSP2_CPU1_CMDBUF_REG3  , 0x174)
REGDEF_BEGIN(CC_DSP2_CPU1_CMDBUF_REG3)             // --> Register "CC_DSP2_CPU1_CMDBUF_REG3" begin ---
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_DSP2_CPU1_CMDBUF_REG3)               // --- Register "CC_DSP2_CPU1_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_DSP2_CPU2_CMDBUF_REG1  , 0x178)
REGDEF_BEGIN(CC_DSP2_CPU2_CMDBUF_REG1)             // --> Register "CC_DSP2_CPU2_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_DSP2_CPU2_CMDBUF_REG1)               // --- Register "CC_DSP2_CPU2_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_DSP2_CPU2_CMDBUF_REG2  , 0x17C)   // --> Register "CC_DSP2_CPU2_CMDBUF_REG2" begin ---
REGDEF_BEGIN(CC_DSP2_CPU2_CMDBUF_REG2)
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_DSP2_CPU2_CMDBUF_REG2)               // --- Register "CC_DSP2_CPU2_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_DSP2_CPU2_CMDBUF_REG3  , 0x180)   // --> Register "CC_DSP2_CPU2_CMDBUF_REG3" begin ---
REGDEF_BEGIN(CC_DSP2_CPU2_CMDBUF_REG3)
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_DSP2_CPU2_CMDBUF_REG3)               // --- Register "CC_DSP2_CPU2_CMDBUF_REG3" end   <--

REGDEF_OFFSET(CC_DSP2_DSP_CMDBUF_REG1  , 0x184)
REGDEF_BEGIN(CC_DSP2_DSP_CMDBUF_REG1)             // --> Register "CC_DSP2_DSP_CMDBUF_REG1" begin ---
    REGDEF_BIT(CoreID             , 4)
    REGDEF_BIT(SerialID           , 12)
    REGDEF_BIT(OPCode             , 16)
REGDEF_END(CC_DSP2_DSP_CMDBUF_REG1)               // --- Register "CC_DSP2_DSP_CMDBUF_REG1" end   <--

REGDEF_OFFSET(CC_DSP2_DSP_CMDBUF_REG2  , 0x188)   // --> Register "CC_DSP2_DSP_CMDBUF_REG2" begin ---
REGDEF_BEGIN(CC_DSP2_DSP_CMDBUF_REG2)
    REGDEF_BIT(DataAddress        , 32)
REGDEF_END(CC_DSP2_DSP_CMDBUF_REG2)               // --- Register "CC_DSP2_DSP_CMDBUF_REG2" end   <--

REGDEF_OFFSET(CC_DSP2_DSP_CMDBUF_REG3  , 0x18C)   // --> Register "CC_DSP2_DSP_CMDBUF_REG3" begin ---
REGDEF_BEGIN(CC_DSP2_DSP_CMDBUF_REG3)
    REGDEF_BIT(DataSize           , 32)
REGDEF_END(CC_DSP2_DSP_CMDBUF_REG3)               // --- Register "CC_DSP2_DSP_CMDBUF_REG3" end   <--



REGDEF_OFFSET(CC_DEBUG_PORT  , 0x1FC)
REGDEF_BEGIN(CC_DEBUG_PORT)                       // -->
    REGDEF_BIT(sel           , 2)
    REGDEF_BIT(           , 30)
REGDEF_END(CC_DEBUG_PORT)                         //

#endif /* _CC_REG_H */
