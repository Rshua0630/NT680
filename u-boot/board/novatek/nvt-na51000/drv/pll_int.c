/*
    PLL Internal module

    @file       pll_int.c
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>
#include "pll.h"
#include "pll_reg.h"
#include "pll_int.h"
#include "clock.h"
#include "Debug.h"

#if 0
/*
    Calculate clock divider

    Target Frequency = Source Frequency / clock divider

    @param[in] uiTargetF        Target (expected) frequency (unit: Hz)
    @param[in] uiSrcF           Source frequency (unit: Hz)

    @return clock divider
*/
static UINT32 pll_genClkDiv(UINT32 uiTargetF, UINT32 uiSrcF)
{
    UINT32 uiDiv;

    uiDiv = (uiSrcF + uiTargetF-1) / uiTargetF;
    if (uiDiv == 0) uiDiv = 1;

    return uiDiv;
}

/*
    Set CPU Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setCpuFreq(UINT32 configContext)
{
    T_PLL_SYS_CR_REG sysCrReg;

    sysCrReg.Reg = PLL_GETREG(PLL_SYS_CR_REG_OFS);

    if (configContext == PLL_SRC_ID_PLL8)
    {
        sysCrReg.Bit.CPU_CLKSEL = CPU_CLK_ENUM_PLL8;
    }
    else if (configContext < 240)
    {
        sysCrReg.Bit.CPU_CLKSEL = CPU_CLK_ENUM_80MHZ;
    }
    else if (configContext < 480)
    {
        sysCrReg.Bit.CPU_CLKSEL = CPU_CLK_ENUM_240MHZ;
    }
    else
    {
        sysCrReg.Bit.CPU_CLKSEL = CPU_CLK_ENUM_480MHZ;
    }

    PLL_SETREG(PLL_SYS_CR_REG_OFS, sysCrReg.Reg);

    return E_OK;
}

/*
    Set OCP Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setOcpFreq(UINT32 configContext)
{
    T_PLL_SYS_CR_REG sysCrReg;

    sysCrReg.Reg = PLL_GETREG(PLL_SYS_CR_REG_OFS);

    if (configContext == PLL_OCP_SRC_CPU)
    {
        sysCrReg.Bit.OCP_CLKSEL = OCP_CLK_ENUM_CPU;
    }
    else
    {
        sysCrReg.Bit.OCP_CLKSEL = OCP_CLK_ENUM_CPUD2;
    }

    PLL_SETREG(PLL_SYS_CR_REG_OFS, sysCrReg.Reg);

    return E_OK;
}

/*
    Set APB Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setApbFreq(UINT32 configContext)
{
    T_PLL_SYS_CR_REG sysCrReg;

    sysCrReg.Reg = PLL_GETREG(PLL_SYS_CR_REG_OFS);

    if (configContext < 60)
    {
        sysCrReg.Bit.APB_CLKSEL = APB_CLK_ENUM_48;
    }
    else if (configContext < 80)
    {
        sysCrReg.Bit.APB_CLKSEL = APB_CLK_ENUM_60;
    }
    else if (configContext < 120)
    {
        sysCrReg.Bit.APB_CLKSEL = APB_CLK_ENUM_80;
    }
    else
    {
        sysCrReg.Bit.APB_CLKSEL = APB_CLK_ENUM_120;
    }

    PLL_SETREG(PLL_SYS_CR_REG_OFS, sysCrReg.Reg);

    return E_OK;
}

/*
    Set IPP Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setIppFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 wordOffset, bitOffset;

    wordOffset = ((configID - PLL_CONFIG_ID_PRE_FREQ) / 8) * 4;
    bitOffset = ((configID - PLL_CONFIG_ID_PRE_FREQ) % 8) * 4;

    if (configContext == PLL_SRC_ID_PLL6)
    {
        uiValue = IPP_CLK_ENUM_PLL6;
    }
    else if (configContext < 60)
    {
        uiValue = IPP_CLK_ENUM_48;
    }
    else if (configContext < 80)
    {
        uiValue = IPP_CLK_ENUM_60;
    }
    else if (configContext < 120)
    {
        uiValue = IPP_CLK_ENUM_80;
    }
    else if (configContext < 160)
    {
        uiValue = IPP_CLK_ENUM_120;
    }
    else if (configContext < 192)
    {
        uiValue = IPP_CLK_ENUM_160;
    }
    else if (configContext < 240)
    {
        uiValue = IPP_CLK_ENUM_192;
    }
    else
    {
        uiValue = IPP_CLK_ENUM_240;
    }

    uiReg = PLL_GETREG(PLL_IPP_CR0_REG_OFS+wordOffset);
    uiReg &= ~(0x07 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_IPP_CR0_REG_OFS+wordOffset, uiReg);

    return E_OK;
}

/*
    Set SIEx MCLK Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_PAR: real frequency is not equal to expected frequency.
*/
ER pll_setSieMclkFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq, uiDiv;

    if (pll_getSieMclkSrc(configID) == PLL_SRC_ID_PLL5)
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_5);
    }
    else
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
    }

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_MCLK_FREQ) * 8;

    uiReg = PLL_GETREG(PLL_IPP_CLKDIV_REG_OFS);
    uiReg &= ~(0xFF << bitOffset);
    uiReg |= uiDiv << bitOffset;
    PLL_SETREG(PLL_IPP_CLKDIV_REG_OFS, uiReg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set SIEx PLL Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_PAR: real frequency is not equal to expected frequency.
*/
ER pll_setSiePllFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq, uiDiv;

    if (pll_getSiePllSrc(configID) == PLL_SRC_ID_PLL6)
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_6);
    }
    else
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
    }

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_PLLFREQ) * 8 + 16;

    uiReg = PLL_GETREG(PLL_IPP_CLKDIV_REG_OFS);
    uiReg &= ~(0xFF << bitOffset);
    uiReg |= uiDiv << bitOffset;
    PLL_SETREG(PLL_IPP_CLKDIV_REG_OFS, uiReg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set JPEG Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setJpgFreq(UINT32 configContext)
{
    T_PLL_CODEC_CR_REG codecCrReg;

    codecCrReg.Reg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);

    if (configContext < 120)
    {
        codecCrReg.Bit.JPEG_CLKSEL = JPG_CLK_ENUM_80;
    }
    else if (configContext < 160)
    {
        codecCrReg.Bit.JPEG_CLKSEL = JPG_CLK_ENUM_120;
    }
    else if (configContext < 192)
    {
        codecCrReg.Bit.JPEG_CLKSEL = JPG_CLK_ENUM_160;
    }
    else
    {
        codecCrReg.Bit.JPEG_CLKSEL = JPG_CLK_ENUM_192;
    }

    PLL_SETREG(PLL_CODEC_CR_REG_OFS, codecCrReg.Reg);

    return E_OK;
}

/*
    Set H.264 Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setH264Freq(UINT32 configContext)
{
    T_PLL_CODEC_CR_REG codecCrReg;

    codecCrReg.Reg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);

    if (configContext == PLL_SRC_ID_PLL6)
    {
        codecCrReg.Bit.H264_CLKSEL = H264_CLK_ENUM_PLL6;
    }
    else if (configContext < 160)
    {
        codecCrReg.Bit.H264_CLKSEL = H264_CLK_ENUM_120;
    }
    else if (configContext < 192)
    {
        codecCrReg.Bit.H264_CLKSEL = H264_CLK_ENUM_160;
    }
    else
    {
        codecCrReg.Bit.H264_CLKSEL = H264_CLK_ENUM_192;
    }

    PLL_SETREG(PLL_CODEC_CR_REG_OFS, codecCrReg.Reg);

    return E_OK;
}

/*
    Set Graphic/Graphic2/Affine Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setGrphFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_GRPH_FREQ) * 4 + 8;

    if (configContext < 120)
    {
        uiValue = GRPH_CLK_ENUM_80;
    }
    else if (configContext < 160)
    {
        uiValue = GRPH_CLK_ENUM_120;
    }
    else if (configContext < 192)
    {
        uiValue = GRPH_CLK_ENUM_160;
    }
    else
    {
        if ((configID == PLL_CONFIG_ID_GRPH_FREQ) || (configContext < 240))
        {
            uiValue = GRPH_CLK_ENUM_192;
        }
        else
        {
            uiValue = GRPH_CLK_ENUM_240;
        }
    }

    uiReg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);
    uiReg &= ~(0x07 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_CODEC_CR_REG_OFS, uiReg);

    return E_OK;
}

/*
    Set Crypto Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setCryptoFreq(UINT32 configContext)
{
    T_PLL_CODEC_CR_REG codecCrReg;

    codecCrReg.Reg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);

    if (configContext < 160)
    {
        codecCrReg.Bit.CRYPTO_CLKSEL = CRYPTO_CLK_ENUM_80;
    }
    else
    {
        codecCrReg.Bit.CRYPTO_CLKSEL = CRYPTO_CLK_ENUM_160;
    }

    PLL_SETREG(PLL_CODEC_CR_REG_OFS, codecCrReg.Reg);

    return E_OK;
}

/*
    Set NAND Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setNandFreq(UINT32 configContext)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);

    if (configContext == PLL_SRC_ID_PLL4)
    {
        periCrReg.Bit.NAND_CLKEL = NAND_CLK_ENUM_PLL4;
    }
    else if (configContext == PLL_SRC_ID_PLL4D2)
    {
        periCrReg.Bit.NAND_CLKEL = NAND_CLK_ENUM_PLL4D2;
    }
    else if (configContext < 60)
    {
        periCrReg.Bit.NAND_CLKEL = NAND_CLK_ENUM_48;
    }
    else if (configContext < 96)
    {
        periCrReg.Bit.NAND_CLKEL = NAND_CLK_ENUM_60;
    }
    else
    {
        periCrReg.Bit.NAND_CLKEL = NAND_CLK_ENUM_96;
    }

    PLL_SETREG(PLL_PERI_CR_REG_OFS, periCrReg.Reg);

    return E_OK;
}

/*
    Set SDIO Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_PAR: real frequency is not equal to expected frequency.
*/
ER pll_setSdioFreq(UINT32 configContext)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_SDIO_CLKDIV_REG sdioDivReg;

    switch (pll_getSdioSrc())
    {
    case 48:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 10;
#else
        uiSrcFreq = 48000000;
#endif
        break;
    case 80:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 6;
#else
        uiSrcFreq = 80000000;
#endif
        break;
    case 96:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 5;
#else
        uiSrcFreq = 96000000;
#endif
        break;
    case PLL_SRC_ID_PLL4:
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    }

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0x1FFF;

    sdioDivReg.Reg = PLL_GETREG(PLL_SDIO_CLKDIV_REG_OFS);
    sdioDivReg.Bit.SDIO_CLKDIV = uiDiv;
    PLL_SETREG(PLL_SDIO_CLKDIV_REG_OFS, sdioDivReg.Reg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set SDIO2 Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_PAR: real frequency is not equal to expected frequency.
*/
ER pll_setSdio2Freq(UINT32 configContext)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_SDIO_CLKDIV_REG sdioDivReg;

    switch (pll_getSdio2Src())
    {
    case 40:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 12;
#else
        uiSrcFreq = 40000000;
#endif
        break;
    case 48:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 10;
#else
        uiSrcFreq = 48000000;
#endif
        break;
    case PLL_SRC_ID_PLL4:
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    }

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0x1FFF;

    sdioDivReg.Reg = PLL_GETREG(PLL_SDIO_CLKDIV_REG_OFS);
    sdioDivReg.Bit.SDIO2_CLKDIV = uiDiv;
    PLL_SETREG(PLL_SDIO_CLKDIV_REG_OFS, sdioDivReg.Reg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set MIPI/LVDS Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setMipiLvdsFreq(UINT32 configContext)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);

    if (configContext < 80)
    {
        periCrReg.Bit.MIPI_LVDS_CLKSEL = MIPI_CLK_ENUM_60;
    }
    else
    {
        periCrReg.Bit.MIPI_LVDS_CLKSEL = MIPI_CLK_ENUM_80;
    }

    PLL_SETREG(PLL_PERI_CR_REG_OFS, periCrReg.Reg);

    return E_OK;
}

/*
    Set IDE/IDE2 Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setIdeFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq, uiDiv;

    switch (pll_getDsiIdeSrc(configID))
    {
    case PLL_SRC_ID_PLL2:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_2);
        break;
    case PLL_SRC_ID_PLL4:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    case PLL_SRC_ID_PLL7:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_7);
        break;
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
        break;
    }

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    bitOffset = (configID - PLL_CONFIG_ID_IDE_FREQ) * 8;

    uiReg = PLL_GETREG(PLL_VDO_CLKDIV_REG_OFS);
    uiReg &= ~(0xFF << bitOffset);
    uiReg |= uiDiv << bitOffset;
    PLL_SETREG(PLL_VDO_CLKDIV_REG_OFS, uiReg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set MI Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setMiFreq(UINT32 configContext)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_VDO_CLKDIV_REG vdoDivReg;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 2;
#else
    uiSrcFreq = 240000000;
#endif

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    vdoDivReg.Reg = PLL_GETREG(PLL_VDO_CLKDIV_REG_OFS);
    vdoDivReg.Bit.MI_CLKDIV = uiDiv;
    PLL_SETREG(PLL_VDO_CLKDIV_REG_OFS, vdoDivReg.Reg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set DSI PHY Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setDsiFreq(UINT32 configContext)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_VDO_CLKDIV_REG vdoDivReg;

    switch (pll_getDsiIdeSrc(PLL_CONFIG_ID_DSI_CLK_SRC))
    {
    case PLL_SRC_ID_PLL2:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_2);
        break;
    case PLL_SRC_ID_PLL4:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    case PLL_SRC_ID_PLL7:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_7);
        break;
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
        break;
    }

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0x3;

    vdoDivReg.Reg = PLL_GETREG(PLL_VDO_CLKDIV_REG_OFS);
    vdoDivReg.Bit.DSI_CLKDIV = uiDiv;
    PLL_SETREG(PLL_VDO_CLKDIV_REG_OFS, vdoDivReg.Reg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set SP_CLK Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSpClkFreq(UINT32 configContext)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_PERI_CLKDIV_REG periDivReg;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 2;
#else
    uiSrcFreq = 240000000;
#endif

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    periDivReg.Reg = PLL_GETREG(PLL_PERI_CLKDIV_REG_OFS);
    periDivReg.Bit.SP_CLKDIV = uiDiv;
    PLL_SETREG(PLL_PERI_CLKDIV_REG_OFS, periDivReg.Reg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set HDMI CEC Frequency

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setHdmiCecFreq(UINT32 configContext)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_PERI_CLKDIV_REG periDivReg;

#if _FPGA_EMULATION_
    uiSrcFreq = (24*2) / 12;
#else
    uiSrcFreq = 3000000;
#endif

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    periDivReg.Reg = PLL_GETREG(PLL_PERI_CLKDIV_REG_OFS);
    periDivReg.Bit.CEC_CLKDIV = uiDiv;
    PLL_SETREG(PLL_PERI_CLKDIV_REG_OFS, periDivReg.Reg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set Audio Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setAudioFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq, uiDiv;

    uiSrcFreq = pll_getPLLFreq(PLL_ID_7);

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    bitOffset = (configID - PLL_CONFIG_ID_ADO_PLL_FREQ) * 8 + 16;

    uiReg = PLL_GETREG(PLL_PERI_CLKDIV_REG_OFS);
    uiReg &= ~(0xFF << bitOffset);
    uiReg |= uiDiv << bitOffset;
    PLL_SETREG(PLL_PERI_CLKDIV_REG_OFS, uiReg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set SPI Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSpiFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq, uiDiv;

    uiSrcFreq = pll_getPLLFreq(PLL_ID_1);

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0xFF;

    bitOffset = (configID - PLL_CONFIG_ID_SPI_FREQ) * 8 + 16;

    uiReg = PLL_GETREG(PLL_SPI_CLKDIV_REG_OFS);
    uiReg &= ~(0xFF << bitOffset);
    uiReg |= uiDiv << bitOffset;
    PLL_SETREG(PLL_SPI_CLKDIV_REG_OFS, uiReg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set PWM Frequency

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setPwmFreq(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg;;
    UINT32 wordOffset, bitOffset;
    UINT32 uiSrcFreq, uiDiv;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 4;
#else
    uiSrcFreq = 120000000;
#endif

    uiDiv = pll_genClkDiv(configContext, uiSrcFreq);
    uiDiv = (uiDiv - 1) & 0x3FFF;

    wordOffset = ((configID - PLL_CONFIG_ID_PWM0_3_FREQ) / 2) * 4;
    bitOffset = ((configID - PLL_CONFIG_ID_PWM0_3_FREQ) % 2) * 16;

    uiReg = PLL_GETREG(PLL_PWMCR0_REG_OFS+wordOffset);
    uiReg &= ~(0x3FFF << bitOffset);
    uiReg |= uiDiv << bitOffset;
    PLL_SETREG(PLL_PWMCR0_REG_OFS+wordOffset, uiReg);

    if ((uiSrcFreq / (uiDiv+1)) != configContext)
    {
        debug_err(("^R%s: real freq will be %d Hz, not expected %d Hz\r\n", __func__, uiSrcFreq / (uiDiv+1), configContext));
        return E_PAR;
    }

    return E_OK;
}

/*
    Set SIEx MCLK source

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSieMclkSrc(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_MCLK_SRC) * 2 + 8;

    if (configContext == PLL_SRC_ID_PLL5)
    {
        uiValue = SIE_MCLK_SRC_ENUM_PLL5;
    }
    else
    {
        uiValue = SIE_MCLK_SRC_ENUM_480;
    }

    uiReg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    uiReg &= ~(0x01 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_IPP_CR1_REG_OFS, uiReg);

    return E_OK;
}

/*
    Set SIEx PLL source

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSiePllSrc(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_PLLCLK_SRC) * 2 + 12;

    if (configContext == PLL_SRC_ID_PLL6)
    {
        uiValue = SIE_PLL_SRC_ENUM_PLL6;
    }
    else
    {
        uiValue = SIE_PLL_SRC_ENUM_480;
    }

    uiReg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    uiReg &= ~(0x01 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_IPP_CR1_REG_OFS, uiReg);

    return E_OK;
}

/*
    Set SIEx source

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSieSrc(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_CLK_SRC) + 16;

    if (configContext == PLL_SIE_SRC_PXCLK_PAD)
    {
        uiValue = SIE_SRC_ENUM_PAD;
    }
    else
    {
        uiValue = SIE_SRC_ENUM_PLL;
    }

    uiReg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    uiReg &= ~(0x01 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_IPP_CR1_REG_OFS, uiReg);

    return E_OK;
}

/*
    Set SDIO source

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSdioSrc(UINT32 configContext)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);

    if (configContext == PLL_SRC_ID_PLL4)
    {
        periCrReg.Bit.SDIO_CLKEL = SDIO_CLK_ENUM_PLL4;
    }
    else if (configContext < 80)
    {
        periCrReg.Bit.SDIO_CLKEL = SDIO_CLK_ENUM_48;
    }
    else if (configContext < 96)
    {
        periCrReg.Bit.SDIO_CLKEL = SDIO_CLK_ENUM_80;
    }
    else
    {
        periCrReg.Bit.SDIO_CLKEL = SDIO_CLK_ENUM_96;
    }

    PLL_SETREG(PLL_PERI_CR_REG_OFS, periCrReg.Reg);

    return E_OK;
}

/*
    Set SDIO2 source

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSdio2Src(UINT32 configContext)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);

    if (configContext == PLL_SRC_ID_PLL4)
    {
        periCrReg.Bit.SDIO2_CLKEL = SDIO2_CLK_ENUM_PLL4;
    }
    else if (configContext < 48)
    {
        periCrReg.Bit.SDIO2_CLKEL = SDIO2_CLK_ENUM_40;
    }
    else
    {
        periCrReg.Bit.SDIO2_CLKEL = SDIO2_CLK_ENUM_48;
    }

    PLL_SETREG(PLL_PERI_CR_REG_OFS, periCrReg.Reg);

    return E_OK;
}

/*
    Set DSI/IDE/IDE2 source

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setDsiIdeSrc(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_DSI_CLK_SRC) * 2 + 14;
    switch (configContext)
    {
    case PLL_SRC_ID_PLL2:
        uiValue = IDE_SRC_ENUM_PLL2;
        break;
    case PLL_SRC_ID_PLL4:
        uiValue = IDE_SRC_ENUM_PLL4;
        break;
    case PLL_SRC_ID_PLL7:
        uiValue = IDE_SRC_ENUM_PLL7;
        break;
    default:
        uiValue = IDE_SRC_ENUM_480;
        break;
    }

    uiReg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    uiReg &= ~(0x03 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_PERI_CR_REG_OFS, uiReg);

    return E_OK;
}

/*
    Set Audio/HdmiAudio Mux source

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setAudioMuxSrc(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_ADO_CLKMUX_SRC) * 4 + 20;
    switch (configContext)
    {
    case 2048000:
        uiValue = ADO_CLKMUX_ENUM_2P048;
        break;
    case 2822400:
        uiValue = ADO_CLKMUX_ENUM_2P8224;
        break;
    case 3072000:
        uiValue = ADO_CLKMUX_ENUM_3P072;
        break;
    case 4096000:
        uiValue = ADO_CLKMUX_ENUM_4P096;
        break;
    case 5644800:
        uiValue = ADO_CLKMUX_ENUM_5P6448;
        break;
    case 6144000:
        uiValue = ADO_CLKMUX_ENUM_6P144;
        break;
    case 8192000:
        uiValue = ADO_CLKMUX_ENUM_8P192;
        break;
    case 11289600:
        uiValue = ADO_CLKMUX_ENUM_11P2896;
        break;
    case 12288000:
        uiValue = ADO_CLKMUX_ENUM_12P288;
        break;
    case 24576000:
        uiValue = ADO_CLKMUX_ENUM_24P576;
        break;
    default:
        debug_err(("^R%s: input audio frequency is NOT supported\r\n", __func__, configContext));
        return E_NOSPT;
    }

    uiReg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    uiReg &= ~(0x0F << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_PERI_CR_REG_OFS, uiReg);

    return E_OK;
}

/*
    Set Audio/HdmiAudio CLK source

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setAudioClkSrc(PLL_CONFIG_ID configID, UINT32 configContext)
{
    UINT32 uiReg, uiValue;;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_ADO_CLK_SRC) + 28;
    if (configContext == PLL_ADO_SRC_CLKDIV)
    {
        uiValue = ADO_CLKSRC_ENUM_PLLDIV;
    }
    else
    {
        uiValue = ADO_CLKSRC_ENUM_CLKMUX;
    }

    uiReg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    uiReg &= ~(0x01 << bitOffset);
    uiReg |= uiValue << bitOffset;
    PLL_SETREG(PLL_PERI_CR_REG_OFS, uiReg);

    return E_OK;
}


/*
    Set SIE MCLK phase

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setSieMclkPhase(UINT32 configContext)
{
    T_PLL_IPP_CR1_REG ippCr1Reg;

    ippCr1Reg.Reg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);

    if (configContext == TRUE)
    {
        ippCr1Reg.Bit.SIE_MCLK_INV = SIE_MCLK_PHASE_ENUM_INVERT;
    }
    else
    {
        ippCr1Reg.Bit.SIE_MCLK_INV = SIE_MCLK_PHASE_ENUM_NORMAL;
    }

    PLL_SETREG(PLL_IPP_CR1_REG_OFS, ippCr1Reg.Reg);

    return E_OK;
}

/*
    Set LVDS CLK phase

    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
        - @b E_CTX:
*/
ER pll_setLvdsClkPhase(UINT32 configContext)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);

    if (configContext == TRUE)
    {
        periCrReg.Bit.LVDS_CLK_PHASE = LVDS_PHASE_ENUM_INVERT;
    }
    else
    {
        periCrReg.Bit.LVDS_CLK_PHASE = LVDS_PHASE_ENUM_NORMAL;
    }

    PLL_SETREG(PLL_PERI_CR_REG_OFS, periCrReg.Reg);

    return E_OK;
}

///////////////////////////////////////////////////////////////////////

/*
    Get CPU Frequency

    @return
        - @b PLL_SRC_ID_PLL8: PLL8
        - @b 80: 80 MHz
        - @b 240: 240 MHz
        - @b 480: 480 MHz
*/
UINT32 pll_getCpuFreq(void)
{
    T_PLL_SYS_CR_REG sysCrReg;

    sysCrReg.Reg = PLL_GETREG(PLL_SYS_CR_REG_OFS);
    switch (sysCrReg.Bit.CPU_CLKSEL)
    {
    case CPU_CLK_ENUM_PLL8:
        return PLL_SRC_ID_PLL8;
    case CPU_CLK_ENUM_80MHZ:
        return 80;
    case CPU_CLK_ENUM_240MHZ:
        return 240;
    case CPU_CLK_ENUM_480MHZ:
    default:
        return 480;
    }
}

/*
    Get OCP Frequency

    @return
        - @b PLL_OCP_SRC_CPU: OCP = CPU
        - @b PLL_OCP_SRC_CPU_D2: OCP = CPU / 2
*/
UINT32 pll_getOcpFreq(void)
{
    T_PLL_SYS_CR_REG sysCrReg;

    sysCrReg.Reg = PLL_GETREG(PLL_SYS_CR_REG_OFS);
    if (sysCrReg.Bit.OCP_CLKSEL == OCP_CLK_ENUM_CPU)
    {
        return PLL_OCP_SRC_CPU;
    }
    else
    {
        return PLL_OCP_SRC_CPU_D2;
    }
}

/*
    Get APB Frequency

    @return
        - @b 48: 48 MHz
        - @b 60: 60 MHz
        - @b 80: 80 MHz
        - @b 120: 120 MHz
*/
UINT32 pll_getApbFreq(void)
{
    T_PLL_SYS_CR_REG sysCrReg;

    sysCrReg.Reg = PLL_GETREG(PLL_SYS_CR_REG_OFS);
    switch (sysCrReg.Bit.APB_CLKSEL)
    {
    case APB_CLK_ENUM_48:
        return 48;
    case APB_CLK_ENUM_60:
        return 60;
    case APB_CLK_ENUM_80:
        return 80;
    case APB_CLK_ENUM_120:
    default:
        return 120;
    }
}

/*
    Get IPP Frequency

    @param[in] configID         configuration identifier

    @return
        - @b PLL_SRC_ID_PLL6: PLL6
        - @b 48: 48 MHz
        - @b 60: 60 MHz
        - @b 80: 80 MHz
        - @b 120: 120 MHz
        - @b 160: 160 MHz
        - @b 192: 192 MHz
        - @b 240: 240 MHz
*/
UINT32 pll_getIppFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 wordOffset, bitOffset;

    wordOffset = ((configID - PLL_CONFIG_ID_PRE_FREQ) / 8) * 4;
    bitOffset = ((configID - PLL_CONFIG_ID_PRE_FREQ) % 8) * 4;

    uiReg = PLL_GETREG(PLL_IPP_CR0_REG_OFS+wordOffset);
    uiReg >>= bitOffset;
    uiReg &= 0x07;

    switch (uiReg)
    {
    case IPP_CLK_ENUM_48:
        return 48;
    case IPP_CLK_ENUM_60:
        return 60;
    case IPP_CLK_ENUM_80:
        return 80;
    case IPP_CLK_ENUM_120:
        return 120;
    case IPP_CLK_ENUM_160:
        return 160;
    case IPP_CLK_ENUM_192:
        return 192;
    case IPP_CLK_ENUM_240:
        return 240;
    case IPP_CLK_ENUM_PLL6:
    default:
        return PLL_SRC_ID_PLL6;
    }
}

/*
    Get SIEx MCLK Frequency

    @param[in] configID         configuration identifier

    @return SIEx MCLK Frequency (unit: Hz)
*/
UINT32 pll_getSieMclkFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq;

    if (pll_getSieMclkSrc(configID) == PLL_SRC_ID_PLL5)
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_5);
    }
    else
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
    }

    bitOffset = (configID - PLL_CONFIG_ID_SIE_MCLK_FREQ) * 8;

    uiReg = PLL_GETREG(PLL_IPP_CLKDIV_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0xFF;

    return uiSrcFreq / (uiReg+1);
}

/*
    Get SIEx PLL Frequency

    @param[in] configID         configuration identifier

    @return SIEx PLL frequency (unit: Hz)
*/
UINT32 pll_getSiePllFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq;

    if (pll_getSiePllSrc(configID) == PLL_SRC_ID_PLL6)
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_6);
    }
    else
    {
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
    }

    bitOffset = (configID - PLL_CONFIG_ID_SIE_PLLFREQ) * 8 + 16;

    uiReg = PLL_GETREG(PLL_IPP_CLKDIV_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0xFF;

    return uiSrcFreq / (uiReg+1);
}

/*
    Get JPEG Frequency

    @return
        - @b 80: 80 MHz
        - @b 120: 120 MHz
        - @b 160: 160 MHz
        - @b 192: 192 MHz
*/
UINT32 pll_getJpgFreq(void)
{
    T_PLL_CODEC_CR_REG codecCrReg;

    codecCrReg.Reg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);
    switch (codecCrReg.Bit.JPEG_CLKSEL)
    {
    case JPG_CLK_ENUM_80:
        return 80;
    case JPG_CLK_ENUM_120:
        return 120;
    case JPG_CLK_ENUM_160:
        return 160;
    case JPG_CLK_ENUM_192:
    default:
        return 192;
    }
}

/*
    Get H.264 Frequency

    @return
        - @b PLL_SRC_ID_PLL6: PLL6
        - @b 120: 120 MHz
        - @b 160: 160 MHz
        - @b 192: 192 MHz
*/
UINT32 pll_getH264Freq(void)
{
    T_PLL_CODEC_CR_REG codecCrReg;

    codecCrReg.Reg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);
    switch (codecCrReg.Bit.H264_CLKSEL)
    {
    case H264_CLK_ENUM_120:
        return 120;
    case H264_CLK_ENUM_160:
        return 160;
    case H264_CLK_ENUM_192:
        return 192;
    case H264_CLK_ENUM_PLL6:
    default:
        return PLL_SRC_ID_PLL6;
    }
}

/*
    Get Graphic/Graphic2/Affine Frequency

    @param[in] configID         configuration identifier

    @return
        - @b 80: 80 MHz
        - @b 120: 120 MHz
        - @b 160: 160 MHz
        - @b 192: 192 MHz
        - @b 240: 240 MHz
*/
UINT32 pll_getGrphFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_GRPH_FREQ) * 4 + 8;

    uiReg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0x07;

    switch (uiReg)
    {
    case GRPH_CLK_ENUM_80:
        return 80;
    case GRPH_CLK_ENUM_120:
        return 120;
    case GRPH_CLK_ENUM_160:
        return 160;
    case GRPH_CLK_ENUM_192:
        return 192;
    case GRPH_CLK_ENUM_240:
    default:
        return 240;
    }
}

/*
    Get Crypto Frequency

    @return
        - @b 80: 80 MHz
        - @b 160: 160 MHz
*/
UINT32 pll_getCryptoFreq(void)
{
    T_PLL_CODEC_CR_REG codecCrReg;

    codecCrReg.Reg = PLL_GETREG(PLL_CODEC_CR_REG_OFS);
    if (codecCrReg.Bit.CRYPTO_CLKSEL == CRYPTO_CLK_ENUM_80)
    {
        return 80;
    }
    else
    {
        return 160;
    }
}

/*
    Get NAND Frequency

    @return
        - @b 48: 48 MHz
        - @b 60: 60 MHz
        - @b 96: 96 MHz
        - @b PLL_SRC_ID_PLL4: PLL4
        - @b PLL_SRC_ID_PLL4D2: PLL4 / 2
*/
UINT32 pll_getNandFreq(void)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    switch (periCrReg.Bit.NAND_CLKEL)
    {
    case NAND_CLK_ENUM_48:
        return 48;
    case NAND_CLK_ENUM_60:
        return 60;
    case NAND_CLK_ENUM_96:
        return 96;
    case NAND_CLK_ENUM_PLL4:
        return PLL_SRC_ID_PLL4;
    case NAND_CLK_ENUM_PLL4D2:
    default:
        return PLL_SRC_ID_PLL4D2;
    }
}

/*
    Get SDIO Frequency

    @return SDIO frequency (unit: Hz)
*/
UINT32 pll_getSdioFreq(void)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_SDIO_CLKDIV_REG sdioDivReg;

    switch (pll_getSdioSrc())
    {
    case 48:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 10;
#else
        uiSrcFreq = 48000000;
#endif
        break;
    case 80:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 6;
#else
        uiSrcFreq = 80000000;
#endif
        break;
    case 96:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 5;
#else
        uiSrcFreq = 96000000;
#endif
        break;
    case PLL_SRC_ID_PLL4:
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    }

    sdioDivReg.Reg = PLL_GETREG(PLL_SDIO_CLKDIV_REG_OFS);
    uiDiv = sdioDivReg.Bit.SDIO_CLKDIV;

    return uiSrcFreq / (uiDiv+1);
}

/*
    Get SDIO2 Frequency

    @return SDIO2 frequency (unit: Hz)
*/
UINT32 pll_getSdio2Freq(void)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_SDIO_CLKDIV_REG sdioDivReg;

    switch (pll_getSdio2Src())
    {
    case 40:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 12;
#else
        uiSrcFreq = 40000000;
#endif
        break;
    case 48:
#if _FPGA_EMULATION_
        uiSrcFreq = (_FPGA_PLL_OSC_*2) / 10;
#else
        uiSrcFreq = 48000000;
#endif
        break;
    case PLL_SRC_ID_PLL4:
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    }

    sdioDivReg.Reg = PLL_GETREG(PLL_SDIO_CLKDIV_REG_OFS);
    uiDiv = sdioDivReg.Bit.SDIO2_CLKDIV;

    return uiSrcFreq / (uiDiv+1);
}

/*
    Get MIPI/LVDS Frequency

    @return
        - @b 60 MHz
        - @b 80 MHz
*/
UINT32 pll_getMipiLvdsFreq(void)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    if (periCrReg.Bit.MIPI_LVDS_CLKSEL == MIPI_CLK_ENUM_60)
    {
        return 60;
    }
    else
    {
        return 80;
    }
}

/*
    Get IDE/IDE2 Frequency

    @param[in] configID         configuration identifier

    @return IDEx frequency (unit: Hz)
*/
UINT32 pll_getIdeFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq;

    switch (pll_getDsiIdeSrc(configID))
    {
    case PLL_SRC_ID_PLL2:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_2);
        break;
    case PLL_SRC_ID_PLL4:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    case PLL_SRC_ID_PLL7:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_7);
        break;
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
        break;
    }

    bitOffset = (configID - PLL_CONFIG_ID_IDE_FREQ) * 8;

    uiReg = PLL_GETREG(PLL_VDO_CLKDIV_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0xFF;

    return uiSrcFreq / (uiReg+1);
}

/*
    Get MI Frequency

    @return MI frequency (unit: Hz)
*/
UINT32 pll_getMiFreq(void)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_VDO_CLKDIV_REG vdoDivReg;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 2;
#else
    uiSrcFreq = 240000000;
#endif

    vdoDivReg.Reg = PLL_GETREG(PLL_VDO_CLKDIV_REG_OFS);
    uiDiv = vdoDivReg.Bit.MI_CLKDIV;

    return uiSrcFreq / (uiDiv+1);
}

/*
    Get DSI PHY Frequency

    @return DSI PHY frequency (unit: Hz)
*/
UINT32 pll_getDsiFreq(void)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_VDO_CLKDIV_REG vdoDivReg;

    switch (pll_getDsiIdeSrc(PLL_CONFIG_ID_DSI_CLK_SRC))
    {
    case PLL_SRC_ID_PLL2:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_2);
        break;
    case PLL_SRC_ID_PLL4:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_4);
        break;
    case PLL_SRC_ID_PLL7:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_7);
        break;
    default:
        uiSrcFreq = pll_getPLLFreq(PLL_ID_1);
        break;
    }

    vdoDivReg.Reg = PLL_GETREG(PLL_VDO_CLKDIV_REG_OFS);
    uiDiv = vdoDivReg.Bit.DSI_CLKDIV ;

    return uiSrcFreq / (uiDiv+1);
}

/*
    Get SP_CLK Frequency

    @return SPI_CLK frequency (unit: Hz)
*/
UINT32 pll_getSpClkFreq(void)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_PERI_CLKDIV_REG periDivReg;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 2;
#else
    uiSrcFreq = 240000000;
#endif

    periDivReg.Reg = PLL_GETREG(PLL_PERI_CLKDIV_REG_OFS);
    uiDiv = periDivReg.Bit.SP_CLKDIV;

    return uiSrcFreq / (uiDiv+1);
}

/*
    Get HDMI CEC Frequency

    @return CEC frequency (unit: Hz)
*/
UINT32 pll_getHdmiCecFreq(void)
{
    UINT32 uiSrcFreq, uiDiv;
    T_PLL_PERI_CLKDIV_REG periDivReg;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 160;
#else
    uiSrcFreq = 3000000;
#endif

    periDivReg.Reg = PLL_GETREG(PLL_PERI_CLKDIV_REG_OFS);
    uiDiv = periDivReg.Bit.CEC_CLKDIV;

    return uiSrcFreq / (uiDiv+1);
}

/*
    Get Audio Frequency

    @param[in] configID         configuration identifier

    @return audio input frequency (unit: Hz)
*/
UINT32 pll_getAudioFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq;

    uiSrcFreq = pll_getPLLFreq(PLL_ID_7);

    bitOffset = (configID - PLL_CONFIG_ID_ADO_PLL_FREQ) * 8 + 16;

    uiReg = PLL_GETREG(PLL_PERI_CLKDIV_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0xFF;

    return uiSrcFreq / (uiReg+1);
}

/*
    Get SPI Frequency

    @param[in] configID         configuration identifier

    @return SPIx frequency (unit: Hz)
*/
UINT32 pll_getSpiFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;;
    UINT32 bitOffset;
    UINT32 uiSrcFreq;

    uiSrcFreq = pll_getPLLFreq(PLL_ID_1);

    bitOffset = (configID - PLL_CONFIG_ID_SPI_FREQ) * 8 + 16;

    uiReg = PLL_GETREG(PLL_SPI_CLKDIV_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0xFF;

    return uiSrcFreq / (uiReg+1);
}

/*
    Get PWM Frequency

    @param[in] configID         configuration identifier

    @return PWM frequency (unit: Hz)
*/
UINT32 pll_getPwmFreq(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 wordOffset, bitOffset;
    UINT32 uiSrcFreq;

#if _FPGA_EMULATION_
    uiSrcFreq = (_FPGA_PLL_OSC_*2) / 4;
#else
    uiSrcFreq = 120000000;
#endif

    wordOffset = ((configID - PLL_CONFIG_ID_PWM0_3_FREQ) / 2) * 4;
    bitOffset = ((configID - PLL_CONFIG_ID_PWM0_3_FREQ) % 2) * 16;

    uiReg = PLL_GETREG(PLL_PWMCR0_REG_OFS+wordOffset);
    uiReg >>= bitOffset;
    uiReg &= 0x3FFF;

    return uiSrcFreq / (uiReg+1);
}

/*
    Get SIEx MCLK source

    @param[in] configID         configuration identifier

    @return
        - @b PLL_SRC_ID_PLL5: source is PLL5
        - @b 480: 480 MHz
*/
UINT32 pll_getSieMclkSrc(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_MCLK_SRC) * 2 + 8;

    uiReg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    uiReg = (uiReg >> bitOffset) & 0x01;
    if (uiReg == SIE_MCLK_SRC_ENUM_PLL5)
    {
        return PLL_SRC_ID_PLL5;
    }
    else
    {
        return 480;
    }
}

/*
    Get SIEx PLL source

    @param[in] configID         configuration identifier

    @return
        - @b PLL_SRC_ID_PLL6: source is PLL6
        - @b 480: 480 MHz
*/
UINT32 pll_getSiePllSrc(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_PLLCLK_SRC) * 2 + 12;

    uiReg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    uiReg = (uiReg >> bitOffset) & 0x01;
    if (uiReg == SIE_PLL_SRC_ENUM_PLL6)
    {
        return PLL_SRC_ID_PLL6;
    }
    else
    {
        return 480;
    }
}

/*
    Get SIEx source

    @param[in] configID         configuration identifier

    @return
        - @b PLL_SIE_SRC_PXCLK_PAD: SIE clock from SIE PXCLK PAD
        - @b PLL_SIE_SRC_PLL_CLK: SIE clock from PLL_CONFIG_ID_SIE_PLLFREQ or PLL_CONFIG_ID_SIE2_PLLFREQ
*/
UINT32 pll_getSieSrc(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_SIE_CLK_SRC) + 16;

    uiReg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0x01;

    if (uiReg == SIE_SRC_ENUM_PAD)
    {
        return PLL_SIE_SRC_PXCLK_PAD;
    }
    else
    {
        return PLL_SIE_SRC_PLL_CLK;
    }
}

/*
    Get SDIO source

    @return
        - @b PLL_SRC_ID_PLL4: source is PLL4
        - @b 48: 48 MHz
        - @b 80: 80 MHz
        - @b 96: 96 MHz
*/
UINT32 pll_getSdioSrc(void)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    switch (periCrReg.Bit.SDIO_CLKEL)
    {
    case SDIO_CLK_ENUM_48:
        return 48;
    case SDIO_CLK_ENUM_80:
        return 80;
    case SDIO_CLK_ENUM_96:
        return 96;
    case SDIO_CLK_ENUM_PLL4:
    default:
        return PLL_SRC_ID_PLL4;
    }
}

/*
    Get SDIO2 source

    @return
        - @b PLL_SRC_ID_PLL4: source is PLL4
        - @b 40: 40 MHz
        - @b 48: 48 MHz
*/
UINT32 pll_getSdio2Src(void)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    switch (periCrReg.Bit.SDIO2_CLKEL)
    {
    case SDIO2_CLK_ENUM_40:
        return 40;
    case SDIO2_CLK_ENUM_48:
        return 48;
    case SDIO2_CLK_ENUM_PLL4:
    default:
        return PLL_SRC_ID_PLL4;
    }
}

/*
    Get DSI/IDE/IDE2 source

    @param[in] configID         configuration identifier

    @return
        - @b PLL_SRC_ID_PLL2: source is PLL2
        - @b PLL_SRC_ID_PLL4: source is PLL4
        - @b PLL_SRC_ID_PLL7: source is PLL7
        - @b 480: 480 MHz
*/
UINT32 pll_getDsiIdeSrc(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_DSI_CLK_SRC) * 2 + 14;

    uiReg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0x03;
    switch (uiReg)
    {
    case IDE_SRC_ENUM_PLL2:
        return PLL_SRC_ID_PLL2;
    case IDE_SRC_ENUM_PLL4:
        return PLL_SRC_ID_PLL4;
    case IDE_SRC_ENUM_PLL7:
        return PLL_SRC_ID_PLL7;
    default:
        return 480;
    }
}

/*
    Get Audio/HdmiAudio Mux source

    @param[in] configID         configuration identifier

    @return
        - @b 2048000: 2.048 MHz
        - @b 2822400: 2.8224 MHz
        - @b 3072000: 3.072 MHz
        - @b 4096000: 4.096 MHz
        - @b 5644800: 5.6448 MHz
        - @b 6144000: 6.144 MHz
        - @b 8192000: 8.192 MHz
        - @b 11289600: 11.2896 MHz
        - @b 12288000: 12.288 MHz
        - @b 24576000: 24.576 MHz
        - @b 0: unkown state
*/
UINT32 pll_getAudioMuxSrc(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_ADO_CLKMUX_SRC) * 4 + 20;

    uiReg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0x0F;

    switch (uiReg)
    {
    case ADO_CLKMUX_ENUM_2P048:
        return 2048000;
    case ADO_CLKMUX_ENUM_2P8224:
        return 2822400;
    case ADO_CLKMUX_ENUM_3P072:
        return 3072000;
    case ADO_CLKMUX_ENUM_4P096:
        return 4096000;
    case ADO_CLKMUX_ENUM_5P6448:
        return 5644800;
    case ADO_CLKMUX_ENUM_6P144:
        return 6144000;
    case ADO_CLKMUX_ENUM_8P192:
        return 8192000;
    case ADO_CLKMUX_ENUM_11P2896:
        return 11289600;
    case ADO_CLKMUX_ENUM_12P288:
        return 12288000;
    case ADO_CLKMUX_ENUM_24P576:
        return 24576000;
    default:
        return 0;
    }
}

/*
    Get Audio/HdmiAudio CLK source

    @param[in] configID         configuration identifier

    @return
        - @b PLL_ADO_SRC_CLKDIV: clock from audio divider (PLL_CONFIG_ID_ADO_PLL_FREQ or PLL_CONFIG_ID_HDMI_ADO_PLL_FREQ)
        - @b PLL_ADO_SRC_CLKMUX: clock from audio mux (PLL_CONFIG_ID_ADO_CLKMUX_SRC or PLL_CONFIG_ID_HDMI_ADO_CLKMUX_SRC)
        - @b E_CTX:
*/
UINT32 pll_getAudioClkSrc(PLL_CONFIG_ID configID)
{
    UINT32 uiReg;
    UINT32 bitOffset;

    bitOffset = (configID - PLL_CONFIG_ID_ADO_CLK_SRC) + 28;

    uiReg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    uiReg >>= bitOffset;
    uiReg &= 0x01;

    if (uiReg == ADO_CLKSRC_ENUM_PLLDIV)
    {
        return PLL_ADO_SRC_CLKDIV;
    }
    else
    {
        return PLL_ADO_SRC_CLKMUX;
    }
}

/*
    Get SIE MCLK phase

    @return
        - @b TRUE: SIE MCLK is inverted
        - @b FALSE: Normal SIE MCLK phase
*/
UINT32 pll_getSieMclkPhase(void)
{
    T_PLL_IPP_CR1_REG ippCr1Reg;

    ippCr1Reg.Reg = PLL_GETREG(PLL_IPP_CR1_REG_OFS);
    if (ippCr1Reg.Bit.SIE_MCLK_INV == SIE_MCLK_PHASE_ENUM_INVERT)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
    Get LVDS CLK phase

    @return
        - @b TRUE: LVDS CLK is inverted
        - @b FALSE: Normal LVDS CLK phase
        - @b E_CTX:
*/
UINT32 pll_getLvdsClkPhase(void)
{
    T_PLL_PERI_CR_REG periCrReg;

    periCrReg.Reg = PLL_GETREG(PLL_PERI_CR_REG_OFS);
    if (periCrReg.Bit.LVDS_CLK_PHASE == LVDS_PHASE_ENUM_INVERT)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

