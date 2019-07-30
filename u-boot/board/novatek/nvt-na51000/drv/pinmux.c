/*
	Pinmux module driver.

	This file is the driver of Piumux module.

	@file		pinmux.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include <common.h>
#include <asm/io.h>
#include "pinmux_int.h"
#include <modelext/top.h>

#define loc_multi_cores() {}
#define unl_multi_cores() {}

static uint32_t mipi_lvds_sensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
typedef ER (*PINMUX_CONFIG_HDL)(uint32_t);
static uint32_t mi_display = 0;

static ER pinmux_config_sdio(uint32_t config);
static ER pinmux_config_sdio2(uint32_t config);
static ER pinmux_config_sdio3(uint32_t config);
static ER pinmux_config_nand(uint32_t config);
static ER pinmux_config_sensor(uint32_t config);
static ER pinmux_config_sensor2(uint32_t config);
static ER pinmux_config_sensor3(uint32_t config);
static ER pinmux_config_sensor4(uint32_t config);
static ER pinmux_config_sensor5(uint32_t config);
static ER pinmux_config_sensor6(uint32_t config);
static ER pinmux_config_sensor7(uint32_t config);
static ER pinmux_config_sensor8(uint32_t config);
static ER pinmux_config_mipi_lvds(uint32_t config);
static ER pinmux_config_i2c(uint32_t config);
static ER pinmux_config_sif(uint32_t config);
static ER pinmux_config_uart(uint32_t config);
static ER pinmux_config_spi(uint32_t config);
static ER pinmux_config_remote(uint32_t config);
static ER pinmux_config_pwm(uint32_t config);
static ER pinmux_config_audio(uint32_t config);
static ER pinmux_config_lcd(uint32_t config);
static ER pinmux_config_lcd2(uint32_t config);
static ER pinmux_config_tv(uint32_t config);
static ER pinmux_config_hdmi(uint32_t config);
static ER pinmux_config_eth(uint32_t config);
static ER pinmux_config_canbus(uint32_t config);

union T_TOP_REG0 top_reg0;
union T_TOP_REG1 top_reg1;
union T_TOP_REG2 top_reg2;
union T_TOP_REG3 top_reg3;
union T_TOP_REG4 top_reg4;
union T_TOP_REG5 top_reg5;
union T_TOP_REG6 top_reg6;
union T_TOP_REG7 top_reg7;
union T_TOP_REG8 top_reg8;
union T_TOP_REG9 top_reg9;
union T_TOP_REG10 top_reg10;
union T_TOP_REG11 top_reg11;
union T_TOP_REG12 top_reg12;
union T_TOP_REGCGPIO0 top_reg_cgpio0;
union T_TOP_REGCGPIO1 top_reg_cgpio1;
union T_TOP_REGPGPIO0 top_reg_pgpio0;
union T_TOP_REGPGPIO1 top_reg_pgpio1;
union T_TOP_REGSGPIO0 top_reg_sgpio0;
union T_TOP_REGLGPIO0 top_reg_lgpio0;
union T_TOP_REGLGPIO1 top_reg_lgpio1;
union T_TOP_REGDGPIO0 top_reg_dgpio0;
union T_TOP_REGHGPI0  top_reg_hgpi0;

static uint32_t disp_pinmux_config[] =
{
    PINMUX_DISPMUX_SEL_NONE,            // LCD
    PINMUX_DISPMUX_SEL_NONE,            // LCD2
    PINMUX_TV_HDMI_CFG_NORMAL,          // TV
    PINMUX_TV_HDMI_CFG_NORMAL           // HDMI
};

static PINMUX_CONFIG_HDL pinmux_config_hdl[] =
{
    pinmux_config_sdio,
    pinmux_config_sdio2,
    pinmux_config_sdio3,
    pinmux_config_nand,
    pinmux_config_sensor,
    pinmux_config_sensor2,
    pinmux_config_sensor3,
    pinmux_config_sensor4,
    pinmux_config_sensor5,
    pinmux_config_sensor6,
    pinmux_config_sensor7,
    pinmux_config_sensor8,
    pinmux_config_mipi_lvds,
    pinmux_config_i2c,
    pinmux_config_sif,
    pinmux_config_uart,
    pinmux_config_spi,
    pinmux_config_remote,
    pinmux_config_pwm,
    pinmux_config_audio,
    pinmux_config_lcd,
    pinmux_config_lcd2,
    pinmux_config_tv,
    pinmux_config_hdmi,
    pinmux_config_eth,
    pinmux_config_canbus
};

/**
	Configure pinmux controller

	Configure pinmux controller by upper layer

	@param[in] top_config	Configuration table
	@return void
*/

ER pinmux_init(PIN_GROUP_CONFIG *top_config)
{
	uint32_t i, tge_reg;
	ER err;

	// Assume all PINMUX is GPIO
	top_reg1.Reg = 0;
	top_reg2.Reg = 0;
	top_reg3.Reg = 0;
	top_reg4.Reg = 0;
	top_reg5.Reg = 0;
	top_reg6.Reg = 0;
	top_reg7.Reg = 0;
	top_reg8.Reg = 0x02;
	top_reg9.Reg = 0;
	top_reg_cgpio0.Reg = 0xFFFFFFFF;
	top_reg_cgpio1.Reg = 0xFFFFFFFF;
	top_reg_pgpio0.Reg = 0xFFFFFFFF;
	top_reg_pgpio1.Reg = 0xFFFFFFFF;
	top_reg_sgpio0.Reg = 0xFFFFFFFF;
	top_reg_lgpio0.Reg = 0xFFFFFFFF;
	top_reg_lgpio1.Reg = 0xFFFFFFFF;
	top_reg_hgpi0.Reg = 0xFFFFFFFF;
	top_reg0.Reg = TOP_GETREG(TOP_REG0_OFS);
	if (top_reg0.Bit.EJTAG_SEL)
		top_reg_dgpio0.Reg = 0x0000183F;
	else
		top_reg_dgpio0.Reg = 0x00001FFF;

	mi_display = 0;
	mipi_lvds_sensor[0] = mipi_lvds_sensor[1] = 0;

	for (i=0; i<=PIN_FUNC_ETH; i++) {
		if (top_config[i].pinFunction != i) {
			printf("top_config[%d].pinFunction context error\r\n", i);
			return E_CTX;
		}

		err = pinmux_config_hdl[i](top_config[i].config);
		if (err != E_OK) {
			printf("top_config[%d].config config error\r\n", i);
			return err;
		}
	}

	// keep ram/rom mapping
	top_reg12.Reg = TOP_GETREG(TOP_REG12_OFS);
	top_reg12.Bit.RAM_SRC_SELECT = top_reg12.Bit.RAM_SRC_SELECT;
	top_reg12.Bit.ROM_SRC_SELECT = top_reg12.Bit.ROM_SRC_SELECT;


	TOP_SETREG(TOP_REG1_OFS, top_reg1.Reg);
	TOP_SETREG(TOP_REG2_OFS, top_reg2.Reg);
	TOP_SETREG(TOP_REG3_OFS, top_reg3.Reg);
	TOP_SETREG(TOP_REG4_OFS, top_reg4.Reg);
	TOP_SETREG(TOP_REG5_OFS, top_reg5.Reg);
	TOP_SETREG(TOP_REG6_OFS, top_reg6.Reg);
	TOP_SETREG(TOP_REG7_OFS, top_reg7.Reg);
	TOP_SETREG(TOP_REG8_OFS, top_reg8.Reg);
	TOP_SETREG(TOP_REG9_OFS, top_reg9.Reg);
	TOP_SETREG(TOP_REG10_OFS,top_reg10.Reg);

	TOP_SETREG(TOP_REGCGPIO0_OFS, top_reg_cgpio0.Reg);
	TOP_SETREG(TOP_REGCGPIO1_OFS, top_reg_cgpio1.Reg);
	TOP_SETREG(TOP_REGPGPIO0_OFS, top_reg_pgpio0.Reg);
	TOP_SETREG(TOP_REGPGPIO1_OFS, top_reg_pgpio1.Reg);
	TOP_SETREG(TOP_REGSGPIO0_OFS, top_reg_sgpio0.Reg);
	TOP_SETREG(TOP_REGLGPIO0_OFS, top_reg_lgpio0.Reg);
	TOP_SETREG(TOP_REGLGPIO1_OFS, top_reg_lgpio1.Reg);
	TOP_SETREG(TOP_REGDGPIO0_OFS, top_reg_dgpio0.Reg);
	TOP_SETREG(TOP_REGHGPI0_OFS,  top_reg_hgpi0.Reg);

	/*Patch TGE pin default output high => change to default low*/
	tge_reg = TGE_GETREG(TGE_CONTROL_OFS);
	tge_reg &= 0xFFFFFF;
	TGE_SETREG(TGE_CONTROL_OFS, tge_reg);

#if defined(_NVT_SDIO2_PAD_POWER_1800_)
	*(u32*) 0xF029001C |= 0x10;
#else
	*(u32*) 0xF029001C &= ~0x10;
#endif

	return E_OK;
}


/*-----------------------------------------------------------------------------*/
/* PINMUX Interface Functions							*/
/*-----------------------------------------------------------------------------*/
static ER pinmux_config_sdio(uint32_t config)
{
    if (config == PIN_SDIO_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SDIO_CFG_2ND_PINMUX)
        {
            printf("SDIO does NOT have 2nd pinmux pad\r\n");
            return E_PAR;
        }

        if (config & PIN_SDIO_CFG_8BITS)
        {
            printf("SDIO does NOT support 8 bits\r\n");
            return E_PAR;
        }

        top_reg1.Bit.SDIO_EXIST = SDIO_EXIST_EN;    // SDIO exist

        top_reg_cgpio0.Bit.CGPIO_16 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_17 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_18 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_19 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_20 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_21 = GPIO_ID_EMUM_FUNC;
    }

    return E_OK;
}

static ER pinmux_config_sdio2(uint32_t config)
{
    if (config == PIN_SDIO_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SDIO_CFG_8BITS)
        {
            //top_reg1.Bit.SDIO2_BUS_WIDTH = SDIO2_BUS_WIDTH_8BITS;
             printf("SDIO2 does NOT support 8 bits\r\n");
            return E_PAR;
        }

        if (config & PIN_SDIO_CFG_2ND_PINMUX)
        {
            //top_reg1.Bit.SDIO2_MUX_SEL = SDIO2_MUX_2ND;
            printf("SDIO2 does NOT have 2nd pinmux pad\r\n");
            return E_PAR;
        }
        //else
        //{
            // SDIO2 2nd pinmux can be always active
            //top_reg1.Bit.SDIO2_ACT = SDIO2_ACT_EN;      // SD active
        //}

        top_reg1.Bit.SDIO2_EXIST = SDIO2_EXIST_EN;  // SD CLK exist

        top_reg_cgpio0.Bit.CGPIO_22 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_23 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_24 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_25 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_26 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_27 = GPIO_ID_EMUM_FUNC;

    }

    return E_OK;
}

static ER pinmux_config_sdio3(uint32_t config)
{
    if (config == PIN_SDIO_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SDIO_CFG_2ND_PINMUX)
        {
            if (config & PIN_SDIO_CFG_8BITS)
            {
                top_reg1.Bit.SDIO3_BUS_WIDTH = SDIO3_BUS_WIDTH_8BITS;

                top_reg_cgpio0.Bit.CGPIO_4 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_5 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_6 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_7 = GPIO_ID_EMUM_FUNC;

            }
            top_reg1.Bit.SDIO3_MUX_SEL = SDIO3_MUX_2ND;

            top_reg_cgpio0.Bit.CGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_1 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_3 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_11 = GPIO_ID_EMUM_FUNC;
        }
        else
        {
            if (config & PIN_SDIO_CFG_8BITS)
            {
                //top_reg1.Bit.SDIO2_BUS_WIDTH = SDIO2_BUS_WIDTH_8BITS;
                printf("SDIO3 does NOT support 8 bits when PIN_SDIO_CFG_1ST_PINMUX \r\n");
                return E_PAR;
            }

            top_reg_cgpio0.Bit.CGPIO_28 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_29 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_30 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_31 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio1.Bit.CGPIO_32 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio1.Bit.CGPIO_33 = GPIO_ID_EMUM_FUNC;
        }

        top_reg1.Bit.SDIO3_EXIST = SDIO3_EXIST_EN;  // SD3 pinmux enable

    }

    return E_OK;
}

static uint32_t nand_get_nandtype(void)
{
	return *(uint32_t *) 0xF0400000 & 0x80000;
}

static ER pinmux_config_nand(uint32_t config)
{
    if (config == PIN_NAND_CFG_NONE)
    {
    }
    //else if (config & (PIN_NAND_CFG_1CS|PIN_NAND_CFG_2CS))
    else if (config & (PIN_NAND_CFG_1CS|PIN_NAND_CFG_2CS|PIN_NAND_CFG_SPI_NAND|PIN_NAND_CFG_SPI_NOR))
    {
        if ((top_reg1.Bit.FSPI_EXIST == FSPI_EXIST_EN))
        {
            printf("NAND conflict with FSPI\r\n");
            return E_OBJ;
        }

        if ((top_reg1.Bit.SDIO3_MUX_SEL == SDIO3_MUX_2ND) && (top_reg1.Bit.SDIO3_EXIST == SDIO3_EXIST_EN))
        {
            printf("NAND conflict with SDIO3_2\r\n");
            return E_OBJ;
        }

        if ((top_reg9.Bit.UART4 == UART4_ENUM_2ND_PINMUX))
        {
            printf("NAND conflict with UART4_2\r\n");
            return E_OBJ;
        }

        if ((top_reg10.Bit.I2C_2 == I2C_ENUM_I2C))
        {
            printf("NAND conflict with I2C_2\r\n");
            return E_OBJ;
        }

        if ((top_reg10.Bit.I2C5_2 == I2C_ENUM_I2C))
        {
            printf("NAND conflict with I2C5_2\r\n");
            return E_OBJ;
        }
        if ((top_reg11.Bit.TRACE_EN == TRACE_ENUM_TRACE))
        {
            printf("NAND conflict with TRACE\r\n");
            return E_OBJ;
        }


        if (config & PIN_NAND_CFG_2CS)
        {
            top_reg1.Bit.NAND_CS_NUM = NAND_CS_NUM_2CS;
            top_reg_cgpio0.Bit.CGPIO_9 = GPIO_ID_EMUM_FUNC;
        }



        if (config & PIN_NAND_CFG_SPI_NAND)
        {
            if(config & PIN_NAND_CFG_SPI_NOR)
            {
                printf("conflict with SPI NOR\r\n");
                return E_OBJ;
            }
            else
            {
                top_reg1.Bit.NAND_EXIST = NAND_EXIST_DIS;
                top_reg1.Bit.FSPI_EXIST = FSPI_EXIST_EN;
            }
        }
        else if (config & PIN_NAND_CFG_SPI_NOR)
        {
            if(config & PIN_NAND_CFG_SPI_NAND)
            {
                printf("conflict with SPI NAND\r\n");
                return E_OBJ;
            }
            else
            {
                top_reg1.Bit.NAND_EXIST = NAND_EXIST_DIS;
                top_reg1.Bit.FSPI_EXIST = FSPI_EXIST_EN;
            }
        }
        else
        {
            top_reg1.Bit.NAND_EXIST = NAND_EXIST_EN;
            top_reg1.Bit.FSPI_EXIST = FSPI_EXIST_DIS;
        }


        //top_reg1.Bit.NAND_ACT = NAND_ACT_EN;
        // Only if ONFI NAND type MC5/6/7/10~15 will chnage to function
        if(nand_get_nandtype() == NANDCTRL_ONFI_NAND_TYPE)
        {
            top_reg_cgpio0.Bit.CGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_11 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_4 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_5 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_6 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_7 = GPIO_ID_EMUM_FUNC;

        }
        // Only if SPI NAND type MC9 chnage to function
        else
        {
            top_reg_cgpio0.Bit.CGPIO_9 = GPIO_ID_EMUM_FUNC;
        }

        top_reg_cgpio0.Bit.CGPIO_0 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_1 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_2 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_3 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_8 = GPIO_ID_EMUM_FUNC;
    }
    else
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }
    return E_OK;
}

static ER pinmux_config_sensor(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR_CFG_NONE)
    {
    }
    else
    {
        mipi_lvds_sensor[0] = 0;
        tmp = config & (PIN_SENSOR_CFG_8BITS|PIN_SENSOR_CFG_10BITS|PIN_SENSOR_CFG_12BITS|PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR_CFG_8BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_8BITS;
            top_reg_hgpi0.Bit.HSIGPI_18 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_19 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_20 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_21 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_24 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_25 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_28 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_29 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;
        case PIN_SENSOR_CFG_10BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_10BITS;
            top_reg_hgpi0.Bit.HSIGPI_16 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_17 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_18 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_19 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_20 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_21 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_24 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_25 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_28 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_29 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;
        case PIN_SENSOR_CFG_12BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_12BITS;
            top_reg_hgpi0.Bit.HSIGPI_16 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_17 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_18 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_19 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_20 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_21 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_22 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_23 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_24 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_25 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_28 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_29 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;
        case PIN_SENSOR_CFG_MIPI:
            mipi_lvds_sensor[0] = PIN_SENSOR_CFG_MIPI;
            break;
        case PIN_SENSOR_CFG_LVDS:
            mipi_lvds_sensor[0] = PIN_SENSOR_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            return E_PAR;
        }

        // Don't need check I2C2_2, because VD/HD is checked first
        // When sensor is LVDS/MIPI and LVDS_VDHD is selected
        if ((mipi_lvds_sensor[0] != 0) && (config & PIN_SENSOR_CFG_LVDS_VDHD))
        {
            // Assing LVDS VD/HD when project layer select this config
            //top_reg3.Bit.VD_HD_SEL = VD_HD_SEL_ENUM_LVDS_VDHD;
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }
        // When sensor is parallel sensor interface
        else if (mipi_lvds_sensor[0] == 0)
        {
            // Force SN_VD/SN_HD to SIE when parallel sensor interface is selected
            //top_reg3.Bit.VD_HD_SEL = VD_HD_SEL_ENUM_SIE_VDHD;
        }

        if (config & PIN_SENSOR_CFG_SHUTTER)
        {
            if (top_reg10.Bit.I2C3 == I2C_2_ENUM_I2C)
            {
                printf("SHUTTER conflict with I2C3\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH7 == SIFCH_ENUM_SIF)
            {
                printf("SHUTTER conflict with SIFCH7\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI_MUX_1ST)
            {
                printf("SHUTTER conflict with SPI4_1\r\n");
                return E_OBJ;
            }

            top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;
            top_reg4.Bit.SHUTTER_SEL = SHUTTER_ENUM_SHUTTER;
        }

        if (config & PIN_SENSOR_CFG_MCLK)
        {
            top_reg_sgpio0.Bit.SGPIO_0 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK = SENMCLK_SEL_ENUM_MCLK;
        }

        if (config & PIN_SENSOR_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

        if (config & (PIN_SENSOR_CFG_MES0|PIN_SENSOR_CFG_MES0_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES0_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES_SH0_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES_SH0_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR_CFG_MES1|PIN_SENSOR_CFG_MES1_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES1_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES_SH1_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES_SH1_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR_CFG_MES2|PIN_SENSOR_CFG_MES2_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES2_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH0_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH0_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR_CFG_MES3|PIN_SENSOR_CFG_MES3_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES3_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH1_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH1_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & PIN_SENSOR_CFG_FLCTR)
        {
            if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
            {
                printf("FLCTR conflict with SEOSOR6 16\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR7 == SENSOR7_ENUM_8BITS)
            {
                printf("FLCTR conflict with SEOSOR7\r\n");
                return E_OBJ;
            }
            top_reg_pgpio0.Bit.PGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg4.Bit.FLCTR = FLCTR_SEL_ENUM_FLCTR;
        }

        if (config & PIN_SENSOR_CFG_STROBE)
        {
            if (top_reg10.Bit.I2C3 == I2C_2_ENUM_I2C)
            {
                printf("STROBE conflict with I2C3\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH6 == SIFCH_ENUM_SIF)
            {
                printf("STROBE conflict with SIFCH6\r\n");
                return E_OBJ;
            }
            if ((top_reg5.Bit.SPI4 == SPI4_1ST_PINMUX) && (top_reg5.Bit.SPI4_DAT == SPI_DAT_ENUM_2Bit))
            {
                printf("STROBE conflict with SPI4_1\r\n");
                return E_OBJ;
            }
            top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg4.Bit.STROBE_SEL = STROBE_ENUM_STROBE;
        }

        if (config & (PIN_SENSOR_CFG_SPCLK|PIN_SENSOR_CFG_SPCLK_2ND))
        {
            if (config & PIN_SENSOR_CFG_SPCLK_2ND)
            {
                if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
                {
                    printf("SP_CLK_2 conflict with SPI2_1\r\n");
                    return E_OBJ;
                }
                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SP_CLK_2 conflict with UART3_1\r\n");
                    return E_OBJ;
                }
                if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)
                {
                    printf("SP_CLK_2 conflict with SIF2\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.AUDIO_MCLK == AUDIO_MCLK_MCLK)
                {
                    printf("SP_CLK_2 conflict with AUDIO MCLK\r\n");
                    return E_OBJ;
                }
                if (top_reg4.Bit.LOCKN == LOCKN_ENUM_LOCKN)
                {
                    printf("SP_CLK_2 conflict with LOCKN\r\n");
                    return E_OBJ;
                }
                top_reg_pgpio0.Bit.PGPIO_30 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.SP_CLK = SP_CLK_SEL_ENUM_SP_2_CLK;
            }
            else
            {
                //check PICNT4_1
                if (top_reg7.Bit.PI_CNT4 == PICNT_ENUM_PICNT)
                {
                    printf("SP_CLK conflict with PICNT4_1\r\n");
                    return E_OBJ;
                }

                top_reg_pgpio0.Bit.PGPIO_22 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.SP_CLK = SP_CLK_SEL_ENUM_SPCLK;
            }
        }

        if (config & (PIN_SENSOR_CFG_SP2CLK|PIN_SENSOR_CFG_SP2CLK_2ND))
        {
            if (config & PIN_SENSOR_CFG_SP2CLK_2ND)
            {
                if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
                {
                    printf("SP_CLK2_2 conflict with SPI2_1\r\n");
                    return E_OBJ;
                }
                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SP_CLK2_2 conflict with UART3_1\r\n");
                    return E_OBJ;
                }
                if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
                {
                    printf("SP_CLK2_2 conflict with SIF3\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.CANBUS== CANBUS_ENUM_CANBUS)
                {
                    printf("SP_CLK2_2 conflict with CANBUS\r\n");
                    return E_OBJ;
                }
                if (top_reg2.Bit.TV_TEST_CLK == TV_TEST_CLK_TV_CLK)
                {
                    printf("SP_CLK2_2 conflict with TV_TEST_CLK\r\n");
                    return E_OBJ;
                }
                if (top_reg4.Bit.LOCKN2 == LOCKN_ENUM_LOCKN)
                {
                    printf("SP_CLK2_2 conflict with LOCKN2\r\n");
                    return E_OBJ;
                }
                top_reg_pgpio0.Bit.PGPIO_31 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.SP2_CLK = SP2_CLK_SEL_ENUM_SP2_2_CLK;
            }
            else
            {
                if ((top_reg7.Bit.PWM0 == PWM_ENUM_PWM) && (top_reg7.Bit.PWM0_SEL == PWM_SEL_ENUM_2ND_PINMUX))
                {
                    printf("SP_CLK2_1 conflict with PWM0_2\r\n");
                    return E_OBJ;
                }

                top_reg_pgpio0.Bit.PGPIO_23 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.SP2_CLK = SP2_CLK_SEL_ENUM_SP2CLK;
            }
        }
        if (config & (PIN_SENSOR_CFG_LOCKN))
        {
            if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
            {
                printf("LOCKN conflict with SPI2_1\r\n");
                return E_OBJ;
            }
            if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
            {
                printf("LOCKN conflict with UART3_1\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)
            {
                printf("LOCKN conflict with SIF2\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.AUDIO_MCLK == AUDIO_MCLK_MCLK)
            {
                printf("LOCKN conflict with AUDIO MCLK\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
            {
                printf("LOCKN conflict with SP_CLK_2\r\n");
                return E_OBJ;
            }

            //top_reg_pgpio0.Bit.PGPIO_30 = GPIO_ID_EMUM_FUNC; //mark this line for LOCKN FW patch
            top_reg4.Bit.LOCKN = LOCKN_ENUM_LOCKN;
        }
        if (config & (PIN_SENSOR_CFG_LOCKN2))
        {
            if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
            {
                printf("LOCKN2 conflict with SPI2_1\r\n");
                return E_OBJ;
            }
            if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
            {
                printf("LOCKN2 conflict with UART3_1\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
            {
                printf("LOCKN2 conflict with SIF3\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.CANBUS== CANBUS_ENUM_CANBUS)
            {
                printf("LOCKN2 conflict with CANBUS\r\n");
                return E_OBJ;
            }
            if (top_reg2.Bit.TV_TEST_CLK == TV_TEST_CLK_TV_CLK)
            {
                printf("LOCKN2 conflict with TV_TEST_CLK\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.SP2_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
            {
                printf("LOCKN2 conflict with SP_CLK2_2\r\n");
                return E_OBJ;
            }

            //top_reg_pgpio0.Bit.PGPIO_31 = GPIO_ID_EMUM_FUNC; //mark this line for LOCKN FW patch
            top_reg4.Bit.LOCKN2 = LOCKN_ENUM_LOCKN;
        }

    }

    return E_OK;
}

static ER pinmux_config_sensor2(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR2_CFG_NONE)
    {
    }
    else
    {
        mipi_lvds_sensor[1] = 0;
        tmp = config & (PIN_SENSOR2_CFG_MIPI|PIN_SENSOR2_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR2_CFG_MIPI:
            mipi_lvds_sensor[1] = PIN_SENSOR2_CFG_MIPI;
            break;
        case PIN_SENSOR2_CFG_LVDS:
            mipi_lvds_sensor[1] = PIN_SENSOR2_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }

        if ((mipi_lvds_sensor[1] != 0) && (config & PIN_SENSOR2_CFG_LVDS_VDHD))
        {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS2_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }
        if (config & PIN_SENSOR2_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }


        if (config & (PIN_SENSOR2_CFG_MES2|PIN_SENSOR2_CFG_MES2_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR2_CFG_MES2_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH0_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH0_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR2_CFG_MES3|PIN_SENSOR2_CFG_MES3_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR2_CFG_MES3_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH1_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;
                top_reg4.Bit.MES2_SH1_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

    }

    return E_OK;
}

/*
    Sensor3 pinmux config

    Sensor3 pinmux config

    @param[in] config

    @return
        - @b E_OK: success
        - @b E_PAR: parameter config not correct
*/
static ER pinmux_config_sensor3(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR3_CFG_NONE)
    {
    }
    else if (config & (PIN_SENSOR3_CFG_8BITS|PIN_SENSOR3_CFG_10BITS|PIN_SENSOR3_CFG_12BITS|PIN_SENSOR3_CFG_16BITS|PIN_SENSOR3_CFG_MIPI|PIN_SENSOR3_CFG_LVDS))
    {
        mipi_lvds_sensor[2] = 0;
        tmp = config & (PIN_SENSOR3_CFG_8BITS|PIN_SENSOR3_CFG_10BITS|PIN_SENSOR3_CFG_12BITS|PIN_SENSOR3_CFG_16BITS|PIN_SENSOR3_CFG_MIPI|PIN_SENSOR3_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR3_CFG_8BITS:
            top_reg3.Bit.SENSOR3 = SENSOR_ENUM_8BITS;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_12 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_13 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;
        case PIN_SENSOR3_CFG_10BITS:
            top_reg3.Bit.SENSOR3 = SENSOR_ENUM_10BITS;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_12 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_13 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;
        case PIN_SENSOR3_CFG_12BITS:
            top_reg3.Bit.SENSOR3 = SENSOR_ENUM_12BITS;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_12 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_13 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;
        case PIN_SENSOR3_CFG_16BITS:
            top_reg3.Bit.SENSOR3 = SENSOR_ENUM_16BITS;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_12 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_13 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_18 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_19 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_20 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_21 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_24 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_25 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_28 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_29 = GPIO_ID_EMUM_GPIO;

            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC; //HD
            */
            break;

        case PIN_SENSOR3_CFG_MIPI:
            mipi_lvds_sensor[2] = PIN_SENSOR_CFG_MIPI;
            break;
        case PIN_SENSOR3_CFG_LVDS:
            mipi_lvds_sensor[2] = PIN_SENSOR_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }

        if ((mipi_lvds_sensor[2] != 0) && (config & PIN_SENSOR3_CFG_LVDS_VDHD))
        {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS3_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
            top_reg4.Bit.XVS_XHS3_SEL = XVS_XHS_1ST_PINMUX;
        }
        if ((mipi_lvds_sensor[2] != 0) && (config & PIN_SENSOR3_CFG_LVDS_VDHD_2ND))
        {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS3_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
            top_reg4.Bit.XVS_XHS3_SEL = XVS_XHS_2ND_PINMUX;
        }


        if (config & PIN_SENSOR3_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

        if (config & (PIN_SENSOR3_CFG_8BITS | PIN_SENSOR3_CFG_16BITS))
        {
            if (config & PIN_SENSOR3_CFG_NOFILED)
            {
                top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_FUNC;
            }
        }
    }

    return E_OK;
}

static ER pinmux_config_sensor4(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR4_CFG_NONE)
    {
    }
    else
    {
        mipi_lvds_sensor[3] = 0;
        tmp = config & (PIN_SENSOR4_CFG_MIPI|PIN_SENSOR4_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR4_CFG_MIPI:
            mipi_lvds_sensor[3] = PIN_SENSOR4_CFG_MIPI;
            break;
        case PIN_SENSOR4_CFG_LVDS:
            mipi_lvds_sensor[3] = PIN_SENSOR4_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }
        if ((mipi_lvds_sensor[3] != 0) && (config & PIN_SENSOR4_CFG_LVDS_VDHD)) {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS4_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }

        if (config & PIN_SENSOR4_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

    }
    return E_OK;
}

static ER pinmux_config_sensor5(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR5_CFG_NONE)
    {
    }
    else
    {
        mipi_lvds_sensor[4] = 0;
        tmp = config & (PIN_SENSOR5_CFG_MIPI|PIN_SENSOR5_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR5_CFG_MIPI:
            mipi_lvds_sensor[4] = PIN_SENSOR5_CFG_MIPI;
            break;
        case PIN_SENSOR5_CFG_LVDS:
            mipi_lvds_sensor[4] = PIN_SENSOR5_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }
        if ((mipi_lvds_sensor[4] != 0) && (config & PIN_SENSOR5_CFG_LVDS_VDHD)) {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS5_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }

        if (config & PIN_SENSOR5_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

    }
    return E_OK;
}

static ER pinmux_config_sensor6(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR6_CFG_NONE)
    {
    }
    else if (config & (PIN_SENSOR6_CFG_8BITS|PIN_SENSOR6_CFG_16BITS|PIN_SENSOR6_CFG_MIPI|PIN_SENSOR6_CFG_LVDS))
    {
        mipi_lvds_sensor[5] = 0;

        tmp = config & (PIN_SENSOR6_CFG_8BITS|PIN_SENSOR6_CFG_16BITS|PIN_SENSOR6_CFG_MIPI|PIN_SENSOR6_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR6_CFG_8BITS:
            top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC; //HD
            */
            //top_reg4.Bit.XVS_XHS6_EN = VD_HD_SEL_ENUM_SIE_VDHD;
            top_reg3.Bit.SENSOR6 = SENSOR6_ENUM_8BITS;
            break;

        case PIN_SENSOR6_CFG_16BITS:
            top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_10= GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC; //HD
            */
            //top_reg4.Bit.XVS_XHS6_EN = VD_HD_SEL_ENUM_SIE_VDHD;
            top_reg3.Bit.SENSOR6 = SENSOR6_ENUM_16BITS;
            break;

        case PIN_SENSOR6_CFG_MIPI:
            mipi_lvds_sensor[5] = PIN_SENSOR6_CFG_MIPI;
            break;
        case PIN_SENSOR6_CFG_LVDS:
            mipi_lvds_sensor[5] = PIN_SENSOR6_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }
        if ((mipi_lvds_sensor[5] != 0) && (config & PIN_SENSOR6_CFG_LVDS_VDHD)) {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS6_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }

        if (config & PIN_SENSOR6_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

    }
    return E_OK;
}


static ER pinmux_config_sensor7(uint32_t config)
{
    uint32_t tmp;
    if (config == PIN_SENSOR7_CFG_NONE)
    {
    }
    else if (config & (PIN_SENSOR7_CFG_8BITS|PIN_SENSOR7_CFG_MIPI|PIN_SENSOR7_CFG_LVDS))
    {
        mipi_lvds_sensor[6] = 0;
        tmp = config & (PIN_SENSOR7_CFG_8BITS|PIN_SENSOR7_CFG_MIPI|PIN_SENSOR7_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR7_CFG_8BITS:
            top_reg_pgpio0.Bit.PGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_pgpio0.Bit.PGPIO_12 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_pgpio0.Bit.PGPIO_13 = GPIO_ID_EMUM_FUNC; //HD
            */
            //top_reg4.Bit.XVS_XHS7_EN = VD_HD_SEL_ENUM_SIE_VDHD;
            top_reg3.Bit.SENSOR7 = SENSOR7_ENUM_8BITS;
            break;

        case PIN_SENSOR7_CFG_MIPI:
            mipi_lvds_sensor[6] = PIN_SENSOR7_CFG_MIPI;
            break;
        case PIN_SENSOR7_CFG_LVDS:
            mipi_lvds_sensor[6] = PIN_SENSOR7_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }
        if ((mipi_lvds_sensor[6] != 0) && (config & PIN_SENSOR7_CFG_LVDS_VDHD)) {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_pgpio0.Bit.PGPIO_12 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_pgpio0.Bit.PGPIO_13 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS7_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }

        if (config & PIN_SENSOR7_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

    }
    return E_OK;
}

static ER pinmux_config_sensor8(uint32_t config)
{
    uint32_t tmp;
    if (config == PIN_SENSOR8_CFG_NONE)
    {
    }
    else if (config & (PIN_SENSOR8_CFG_8BITS|PIN_SENSOR8_CFG_MIPI|PIN_SENSOR8_CFG_LVDS))
    {
        mipi_lvds_sensor[7] = 0;
        tmp = config & (PIN_SENSOR8_CFG_8BITS|PIN_SENSOR8_CFG_MIPI|PIN_SENSOR8_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR8_CFG_8BITS:
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC; //PXCLK
            /*top_reg_lgpio0.Bit.LGPIO_24 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_lgpio0.Bit.LGPIO_25 = GPIO_ID_EMUM_FUNC; //HD
            */
            //top_reg4.Bit.XVS_XHS8_EN = VD_HD_SEL_ENUM_SIE_VDHD;
            top_reg3.Bit.SENSOR8 = SENSOR8_ENUM_8BITS;
            break;

        case PIN_SENSOR8_CFG_MIPI:
            mipi_lvds_sensor[7] = PIN_SENSOR8_CFG_MIPI;
            break;
        case PIN_SENSOR8_CFG_LVDS:
            mipi_lvds_sensor[7] = PIN_SENSOR8_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }
        if ((mipi_lvds_sensor[7] != 0) && (config & PIN_SENSOR8_CFG_LVDS_VDHD)) {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_pgpio0.Bit.PGPIO_14 = GPIO_ID_EMUM_FUNC; //XVS
            top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC; //XHS
            top_reg4.Bit.XVS_XHS8_EN = XVS_XHS_SEL_ENUM_XVS_XHS;
        }

        if (config & PIN_SENSOR8_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            //top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

    }
    return E_OK;
}

static ER pinmux_config_mipi_lvds(uint32_t config)
{

    if (config == PIN_MIPI_LVDS_CFG_NONE)
    {
    }
    else
    {
        // check clk lane 0
        if (config & PIN_MIPI_LVDS_CFG_CLK0)
        {
            top_reg_hgpi0.Bit.HSIGPI_18 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_19 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 1
        if (config & PIN_MIPI_LVDS_CFG_CLK1)
        {
            top_reg_hgpi0.Bit.HSIGPI_22 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_23 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 2
        if (config & PIN_MIPI_LVDS_CFG_CLK2)
        {
            top_reg_hgpi0.Bit.HSIGPI_26 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_27 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 3
        if (config & PIN_MIPI_LVDS_CFG_CLK3)
        {
            top_reg_hgpi0.Bit.HSIGPI_30 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_31 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 4
        if (config & PIN_MIPI_LVDS_CFG_CLK4)
        {
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 5
        if (config & PIN_MIPI_LVDS_CFG_CLK5)
        {
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 6
        if (config & PIN_MIPI_LVDS_CFG_CLK6)
        {
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 7
        if (config & PIN_MIPI_LVDS_CFG_CLK7)
        {
            top_reg_hgpi0.Bit.HSIGPI_14 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_15 = GPIO_ID_EMUM_FUNC;
        }


        // check data lane 0
        if (config & PIN_MIPI_LVDS_CFG_DAT0)
        {
            top_reg_hgpi0.Bit.HSIGPI_16 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_17 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 1
        if (config & PIN_MIPI_LVDS_CFG_DAT1)
        {

            top_reg_hgpi0.Bit.HSIGPI_20 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_21 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 2
        if (config & PIN_MIPI_LVDS_CFG_DAT2)
        {

            top_reg_hgpi0.Bit.HSIGPI_24 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_25 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 3
        if (config & PIN_MIPI_LVDS_CFG_DAT3)
        {

            top_reg_hgpi0.Bit.HSIGPI_28 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_29 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 4
        if (config & PIN_MIPI_LVDS_CFG_DAT4)
        {

            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 5
        if (config & PIN_MIPI_LVDS_CFG_DAT5)
        {

            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 6
        if (config & PIN_MIPI_LVDS_CFG_DAT6)
        {

            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 7
        if (config & PIN_MIPI_LVDS_CFG_DAT7)
        {

            top_reg_hgpi0.Bit.HSIGPI_12 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_13 = GPIO_ID_EMUM_FUNC;
        }

    }

    return E_OK;
}

static ER pinmux_config_i2c(uint32_t config)
{
    if (config == PIN_I2C_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_I2C_CFG_CH1)
        {
            top_reg_pgpio0.Bit.PGPIO_28 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_29 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH1_2ND_PINMUX)
        {
            if (top_reg1.Bit.NAND_EXIST == NAND_EXIST_EN)
            {
                printf("I2C_2 conflict with NAND\r\n");
                return E_OBJ;
            }
            if ((top_reg9.Bit.UART4 == UART4_ENUM_2ND_PINMUX))
            {
                printf("I2C_2 conflict with UART4_2\r\n");
                return E_OBJ;
            }
            if ((top_reg11.Bit.TRACE_EN == TRACE_ENUM_TRACE))
            {
                printf("I2C_2 conflict with TRACE\r\n");
                return E_OBJ;
            }

            top_reg_cgpio0.Bit.CGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_13 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C_2 = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH2)
        {
            if (top_reg5.Bit.SIFCH6 == SIFCH_ENUM_SIF)
            {
                printf("I2C2 conflict with SIFCH6\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH7 == SIFCH_ENUM_SIF)
            {
                printf("I2C2 conflict with SIFCH7\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI4_1ST_PINMUX)
            {
                printf("I2C2 conflict with SPI4\r\n");
                return E_OBJ;
            }

            top_reg_sgpio0.Bit.SGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_9 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C2 = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH2_2ND_PINMUX)
        {

            if (top_reg7.Bit.PWM8 == PWM_ENUM_PWM)
            {
                printf("I2C2_2 conflict with to PWM8: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg7.Bit.PWM9 == PWM_ENUM_PWM)
            {
                printf("I2C2_2 conflict with to PWM9: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
            {
                printf("I2C2_2 conflict with to SPI3_2: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
            {
                printf("I2C2_2 conflict with to UART3_2: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
            {
                printf("I2C2_2 conflict with to SIFCH2_2: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
            {
                printf("I2C2_2 conflict with to SIFCH3_2: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
            {
                printf("I2C2_2 conflict with to SENSOR6: 0x%x\r\n", config);
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C2_2 = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH3)
        {
            if (top_reg5.Bit.SIFCH6 == SIFCH_ENUM_SIF)
            {
                printf("I2C3 conflict with to SIFCH6: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH7 == SIFCH_ENUM_SIF)
            {
                printf("I2C3 conflict with to SIFCH7: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI4_1ST_PINMUX)
            {
                printf("I2C3 conflict with to SPI4_1: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg4.Bit.STROBE_SEL == STROBE_ENUM_STROBE)
            {
                printf("I2C3 conflict with to STROBE: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg4.Bit.SHUTTER_SEL == SHUTTER_ENUM_SHUTTER)
            {
                printf("I2C3 conflict with to SHUTTER: 0x%x\r\n", config);
                return E_OBJ;
            }

            top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C3 = I2C_ENUM_I2C;

        }
        if (config & PIN_I2C_CFG_CH3_2ND_PINMUX)
        {
            if (top_reg7.Bit.PWM10 == PWM_ENUM_PWM)
            {
                printf("PWM10 conflict with to PWM8: 0x%x\r\n", config);
                return E_OBJ;
            }

            if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
            {
                printf("I2C3_2 conflict with SPI3_2\r\n");
                return E_OBJ;
            }
            if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
            {
                printf("I2C3_2 conflict with UART3_2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
            {
                printf("I2C3_2 conflict with SIFCH2_2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
            {
                printf("I2C3_2 conflict with SIFCH3_2\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
            {
                printf("I2C3_2 conflict with SENSOR6\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
            {
                printf("I2C3_2 conflict with SENSOR7\r\n");
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C3_2 = I2C_ENUM_I2C;
        }
        if (config & PIN_I2C_CFG_CH3_3RD_PINMUX)
        {
            if (top_reg5.Bit.SIFCH4 == SIFCH_ENUM_SIF)
            {
                printf("I2C3_3 conflict with SIFCH4\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH5 == SIFCH_ENUM_SIF)
            {
                printf("I2C3_3 conflict with SIFCH5\r\n");
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_26 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_27 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C3_3 = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH4)
        {
            if (top_reg7.Bit.PWM16 == PWM_ENUM_PWM)
            {
                printf("I2C4 conflict with to PWM16: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg7.Bit.PWM17 == PWM_ENUM_PWM)
            {
                printf("I2C4 conflict with to PWM17: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg4.Bit.MES_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
            {
                printf("I2C4 conflict with to MES_2_SH0: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg4.Bit.MES_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
            {
                printf("I2C4 conflict with to MES_2_SH1: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
            {
                printf("I2C4 conflict with SPI_2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX)
            {
                printf("I2C4 conflict with SIFCH0_2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_3RD_PINMUX)
            {
                printf("I2C4 conflict with SIFCH0_3\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
            {
                printf("I2C4 conflict with SENSOR6\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
            {
                printf("I2C4 conflict with SENSOR7\r\n");
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C4 = I2C_ENUM_I2C;

        }
        if (config & PIN_I2C_CFG_CH4_2ND_PINMUX)
        {
            if (top_reg5.Bit.SIFCH4 == SIFCH_ENUM_SIF)
            {
                printf("I2C4_2 conflict with SIFCH4\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH5 == SIFCH_ENUM_SIF)
            {
                printf("I2C4_2 conflict with SIFCH5\r\n");
                return E_OBJ;
            }
            top_reg_pgpio0.Bit.PGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_25 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C4_2 = I2C_ENUM_I2C;

        }
        if (config & PIN_I2C_CFG_CH4_3RD_PINMUX)
        {
            if (top_reg7.Bit.PWM18 == PWM_ENUM_PWM)
            {
                printf("I2C4_3 conflict with to PWM18: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg7.Bit.PWM19 == PWM_ENUM_PWM)
            {
                printf("I2C4_3 conflict with to PWM19: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg4.Bit.MES2_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
            {
                printf("I2C4_3 conflict with to MES2_2_SH0: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
            {
                printf("I2C4_3 conflict with to MES2_2_SH1: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
            {
                printf("I2C4_3 conflict with SPI_2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX)
            {
                printf("I2C4_3 conflict with SIFCH0_2\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
            {
                printf("I2C4 conflict with SENSOR6\r\n");
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
            {
                printf("I2C4 conflict with SENSOR7\r\n");
                return E_OBJ;
            }
            top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C4_3 = I2C_ENUM_I2C;


        }
        if (config & PIN_I2C_CFG_CH4_4TH_PINMUX)
        {
            if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF)
            {
                printf("I2C4_4 conflict with SIFCH0_1\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI4_3RD_PINMUX)
            {
                printf("I2C4_4 conflict with SPI4_3\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.ETH!=ETH_ID_ENUM_GPIO)
            {
                printf("I2C4_4 conflict with ETH\r\n");
                return E_OBJ;
            }
            top_reg_lgpio0.Bit.LGPIO_28 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_29 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C4_4 = I2C_ENUM_I2C;

        }

        if (config & PIN_I2C_CFG_CH5)
        {
            top_reg_pgpio1.Bit.PGPIO_45 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio1.Bit.PGPIO_46 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C5 = I2C_ENUM_I2C;
        }
        if (config & PIN_I2C_CFG_CH5_2ND_PINMUX)
        {
            if (top_reg1.Bit.NAND_EXIST == NAND_EXIST_EN)
            {
                printf("I2C5_2 conflict with NAND\r\n");
                return E_OBJ;
            }
            if ((top_reg9.Bit.UART4 == UART4_ENUM_2ND_PINMUX) && (top_reg9.Bit.UART4_CTSRTS == UART_CTSRTS_PINMUX))
            {
                printf("I2C5_2 conflict with UART4_2 CTSRTS\r\n");
                return E_OBJ;
            }
            if (top_reg11.Bit.TRACE_EN == TRACE_ENUM_TRACE)
            {
                printf("I2C5_2 conflict with TRACE\r\n");
                return E_OBJ;
            }
            top_reg_cgpio0.Bit.CGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_15 = GPIO_ID_EMUM_FUNC;

            top_reg10.Bit.I2C5_2 = I2C_ENUM_I2C;

        }

    }

    return E_OK;
}

static ER pinmux_config_sif(uint32_t config)
{
    if (config == PIN_SIF_CFG_NONE)
    {
    }
    else
    {
        if (config & (PIN_SIF_CFG_CH0|PIN_SIF_CFG_CH0_2ND_PINMUX|PIN_SIF_CFG_CH0_3RD_PINMUX))
        {
            if (config & PIN_SIF_CFG_CH0_2ND_PINMUX)
            {
               if (top_reg7.Bit.PWM16 == PWM_ENUM_PWM)
               {
                    printf("SIFCH0_2 conflict with PWM16\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM18 == PWM_ENUM_PWM)
               {
                    printf("SIFCH0_2 conflict with PWM18\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM19 == PWM_ENUM_PWM)
               {
                    printf("SIFCH0_2 conflict with PWM19\r\n");
                    return E_OBJ;
               }
               if (top_reg4.Bit.MES_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH0_2 conflict with MES_2_SHUT0\r\n");
                    return E_OBJ;
               }
               if (top_reg4.Bit.MES2_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH0_2 conflict with MES2_2_SHUT0\r\n");
                    return E_OBJ;
               }
               if (top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH0_2 conflict with MES2_2_SHUT1\r\n");
                    return E_OBJ;
               }
               if (top_reg5.Bit.SPI == SPI_MUX_2ND)
               {
                    printf("SIFCH0_2 conflict with SPI_2\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C4 == I2C_ENUM_I2C)
               {
                    printf("SIFCH0_2 conflict with I2C4\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C4_3 == I2C_ENUM_I2C)
               {
                    printf("SIFCH0_2 conflict with I2C4_3\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR7 == SENSOR7_ENUM_8BITS)
               {
                    printf("SIFCH0_2 conflict with SENSOR7\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
               {
                    printf("SIFCH0_2 conflict with SENSOR6 16 bits\r\n");
                    return E_OBJ;
               }

               top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

               top_reg5.Bit.SIFCH0 = SIFCH_ENUM_SIF_2ND_PINMUX;

            }
            else if (config & PIN_SIF_CFG_CH0_3RD_PINMUX)
            {
               if (top_reg7.Bit.PWM17 == PWM_ENUM_PWM)
               {
                    printf("SIFCH0_3 conflict with PWM17\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM18 == PWM_ENUM_PWM)
               {
                    printf("SIFCH0_3 conflict with PWM18\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM19 == PWM_ENUM_PWM)
               {
                    printf("SIFCH0_3 conflict with PWM19\r\n");
                    return E_OBJ;
               }
               if (top_reg4.Bit.MES_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH0_3 conflict with MES_2_SHUT1\r\n");
                    return E_OBJ;
               }
               if (top_reg4.Bit.MES2_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH0_3 conflict with MES2_2_SHUT0\r\n");
                    return E_OBJ;
               }
               if (top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH0_3 conflict with MES2_2_SHUT1\r\n");
                    return E_OBJ;
               }
               if (top_reg5.Bit.SPI == SPI_MUX_2ND)
               {
                    printf("SIFCH0_3 conflict with SPI_2\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C4 == I2C_ENUM_I2C)
               {
                    printf("SIFCH0_3 conflict with I2C4\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C4_3 == I2C_ENUM_I2C)
               {
                    printf("SIFCH0_3 conflict with I2C4_3\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR7 == SENSOR_ENUM_8BITS)
               {
                    printf("SIFCH0_3 conflict with SENSOR7\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
               {
                    printf("SIFCH0_3 conflict with SENSOR6 16 bits\r\n");
                    return E_OBJ;
               }

               top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

               top_reg5.Bit.SIFCH0 = SIFCH_ENUM_SIF_3RD_PINMUX;
            }
            else
            {
                if (top_reg2.Bit.LCD_TYPE == LCDTYPE_ENUM_PARALLEL_LCD)
                {
                    printf("SIFCH0_1 conflict with LCD Parallel\r\n");
                    return E_OBJ;
                }
                if (top_reg7.Bit.PI_CNT4 == PICNT_ENUM_PICNT2)
                {
                    printf("SIFCH0_1 conflict with PI_CNT4_2\r\n");
                    return E_OBJ;
                }
                /*if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF)
                {
                    printf("SIFCH0_1 conflict with SIFCH0_1\r\n");
                    return E_OBJ;
                }*/
                if (top_reg10.Bit.I2C4_4 == I2C_ENUM_I2C)
                {
                    printf("SIFCH0_1 conflict with I2C4_4\r\n");
                    return E_OBJ;
                }
                if (top_reg5.Bit.SPI4 == SPI_MUX_3RD)
                {
                    printf("SIFCH0_1 conflict with SPI4_3\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
                {
                    printf("SIFCH0_1 conflict with ETH\r\n");
                    return E_OBJ;
                }
                if (top_reg5.Bit.SPI4 == SPI4_3RD_PINMUX)
                {
                    printf("SIFCH0_1 conflict with SPI4_3\r\n");
                    return E_OBJ;
                }

                top_reg_lgpio0.Bit.LGPIO_27 = GPIO_ID_EMUM_FUNC;
                top_reg_lgpio0.Bit.LGPIO_28 = GPIO_ID_EMUM_FUNC;
                top_reg_lgpio0.Bit.LGPIO_29 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SIFCH0 = SIFCH_ENUM_SIF;
            }
        }

        if (config & (PIN_SIF_CFG_CH1|PIN_SIF_CFG_CH1_2ND_PINMUX))
        {
            if (config & PIN_SIF_CFG_CH1_2ND_PINMUX)
            {
                printf("SIF_CH1 no 2ND PINMUX\r\n");
                return E_OBJ;
            }
            else
            {
                top_reg_lgpio0.Bit.LGPIO_30 = GPIO_ID_EMUM_FUNC;
                top_reg_lgpio0.Bit.LGPIO_31 = GPIO_ID_EMUM_FUNC;
                top_reg_lgpio1.Bit.LGPIO_32 = GPIO_ID_EMUM_FUNC;
            }

            top_reg5.Bit.SIFCH1 = SIFCH_ENUM_SIF;
        }

        if (config & (PIN_SIF_CFG_CH2|PIN_SIF_CFG_CH2_2ND_PINMUX))
        {

            if (config & PIN_SIF_CFG_CH2_2ND_PINMUX)
            {
               if (top_reg7.Bit.PWM8 == PWM_ENUM_PWM)
               {
                    printf("SIFCH2_2 conflict with PWM8\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM10 == PWM_ENUM_PWM)
               {
                    printf("SIFCH2_2 conflict with PWM10\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM11 == PWM_ENUM_PWM)
               {
                    printf("SIFCH2_2 conflict with PWM11\r\n");
                    return E_OBJ;
               }
               if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
               {
                    printf("SIFCH2_2 conflict with SPI3_2\r\n");
                    return E_OBJ;
               }
               if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH2_2 conflict with UART3_2\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C2_2 == I2C_ENUM_I2C)
               {
                    printf("SIFCH2_2 conflict with I2C2_2\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C3_2 == I2C_ENUM_I2C)
               {
                    printf("SIFCH2_2 conflict with I2C3_2\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
               {
                    printf("SIFCH2_2 conflict with SENSOR6\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
               {
                    printf("SIFCH2_2 conflict with SENSOR7\r\n");
                    return E_OBJ;
               }

               top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

               top_reg5.Bit.SIFCH2 = SIFCH_ENUM_SIF_2ND_PINMUX;

            }
            else
            {
                if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
                {
                    printf("SIFCH2 conflict with SPI2_1\r\n");
                    return E_OBJ;
                }
                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SIFCH2 conflict with UART3_1\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.AUDIO_MCLK == AUDIO_MCLK_MCLK)
                {
                    printf("SIFCH2 conflict with AUDIO MCLK\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.CANBUS == CANBUS_ENUM_CANBUS)
                {
                    printf("SIFCH2 conflict with CANBUS\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
                {
                    printf("SIFCH2 conflict with SP_CLK_2\r\n");
                    return E_OBJ;
                }
                if (top_reg4.Bit.LOCKN == LOCKN_ENUM_LOCKN)
                {
                    printf("SIFCH2 conflict with LOCKN\r\n");
                    return E_OBJ;
                }

                top_reg_pgpio0.Bit.PGPIO_30 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_32 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_33 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SIFCH2 = SIFCH_ENUM_SIF;
            }
        }

        if (config & (PIN_SIF_CFG_CH3|PIN_SIF_CFG_CH3_2ND_PINMUX))
        {
            if (config & PIN_SIF_CFG_CH3_2ND_PINMUX)
            {
               if (top_reg7.Bit.PWM9 == PWM_ENUM_PWM)
               {
                    printf("SIFCH3_2 conflict with PWM9\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM10 == PWM_ENUM_PWM)
               {
                    printf("SIFCH3_2 conflict with PWM10\r\n");
                    return E_OBJ;
               }
               if (top_reg7.Bit.PWM11 == PWM_ENUM_PWM)
               {
                    printf("SIFCH3_2 conflict with PWM11\r\n");
                    return E_OBJ;
               }
               if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
               {
                    printf("SIFCH3_2 conflict with SPI3_2\r\n");
                    return E_OBJ;
               }
               if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
               {
                    printf("SIFCH3_2 conflict with UART3_2\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C2_2 == I2C_ENUM_I2C)
               {
                    printf("SIFCH3_2 conflict with I2C2_2\r\n");
                    return E_OBJ;
               }
               if (top_reg10.Bit.I2C3_2 == I2C_ENUM_I2C)
               {
                    printf("SIFCH3_2 conflict with I2C3_2\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
               {
                    printf("SIFCH3_2 conflict with SENSOR6\r\n");
                    return E_OBJ;
               }
               if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
               {
                    printf("SIFCH3_2 conflict with SENSOR7\r\n");
                    return E_OBJ;
               }
               top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
               top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

               top_reg5.Bit.SIFCH3 = SIFCH_ENUM_SIF_2ND_PINMUX;

            }
            else
            {
                if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
                {
                    printf("SIFCH3 conflict with SPI2_1\r\n");
                    return E_OBJ;
                }
                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SIFCH3 conflict with UART3_1\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
                {
                    printf("SIFCH3 conflict with ETH\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
                {
                    printf("SIFCH3 conflict with SPCLK_2_2\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.CANBUS == CANBUS_ENUM_CANBUS)
                {
                    printf("SIFCH3 conflict with CANBUS\r\n");
                    return E_OBJ;
                }
                if (top_reg4.Bit.LOCKN2 == LOCKN_ENUM_LOCKN)
                {
                    printf("SIFCH3 conflict with LOCKN2\r\n");
                    return E_OBJ;
                }
                if (top_reg2.Bit.TV_TEST_CLK == TV_TEST_CLK_TV_CLK)
                {
                    printf("SIFCH3 conflict with TV_TEST_CLK\r\n");
                    return E_OBJ;
                }

                top_reg_pgpio0.Bit.PGPIO_31 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_32 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_33 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SIFCH3 = SIFCH_ENUM_SIF;
            }
        }

        if (config & PIN_SIF_CFG_CH4)
        {

            if (top_reg10.Bit.I2C4_2 == I2C_ENUM_I2C)
            {
                printf("SIFCH4 conflict with I2C4_2\r\n");
                return E_OBJ;
            }
            if (top_reg10.Bit.I2C3_3 == I2C_ENUM_I2C)
            {
                printf("SIFCH4 conflict with I2C3_3\r\n");
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_26 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_27 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH4 = SIFCH_ENUM_SIF;
        }

        if (config & PIN_SIF_CFG_CH5)
        {
            if (top_reg10.Bit.I2C4_2 == I2C_ENUM_I2C)
            {
                printf("SIFCH4 conflict with I2C4_2\r\n");
                return E_OBJ;
            }
            if (top_reg10.Bit.I2C3_3 == I2C_ENUM_I2C)
            {
                printf("SIFCH4 conflict with I2C3_3\r\n");
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_26 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_27 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH5 = SIFCH_ENUM_SIF;
        }

        if (config & PIN_SIF_CFG_CH6)
        {
            if (top_reg10.Bit.I2C2 == I2C_ENUM_I2C)
            {
                printf("SIFCH6 conflict with I2C2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI_MUX_1ST)
            {
                printf("SIFCH6 conflict with SPI4_1\r\n");
                return E_OBJ;
            }
            if (top_reg4.Bit.STROBE_SEL == STROBE_ENUM_STROBE)
            {
                printf("SIFCH6 conflict with STROBE\r\n");
                return E_OBJ;
            }
            top_reg_sgpio0.Bit.SGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH6 = SIFCH_ENUM_SIF;
        }

        if (config & PIN_SIF_CFG_CH7)
        {
            if (top_reg10.Bit.I2C2 == I2C_ENUM_I2C)
            {
                printf("SIFCH7 conflict with I2C2\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI_MUX_1ST)
            {
                printf("SIFCH7 conflict with SPI4_1\r\n");
                return E_OBJ;
            }
            if (top_reg4.Bit.SHUTTER_SEL == SHUTTER_ENUM_SHUTTER)
            {
                printf("SIFCH7 conflict with SHUTTER\r\n");
                return E_OBJ;
            }
            top_reg_sgpio0.Bit.SGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH7 = SIFCH_ENUM_SIF;
        }


    }

    return E_OK;
}

static ER pinmux_config_uart(uint32_t config)
{
    if (config == PIN_UART_CFG_NONE)
    {
    }
    else if (config & (PIN_UART_CFG_CH1|PIN_UART_CFG_CH2|PIN_UART_CFG_CH3|PIN_UART_CFG_CH4))
    {
        if (config & (PIN_UART_CFG_CH1|PIN_UART_CFG_CH1_TX))
        {
            top_reg_pgpio1.Bit.PGPIO_42 = GPIO_ID_EMUM_FUNC;

            if (config & (PIN_UART_CFG_CH1))
            {
                top_reg_pgpio1.Bit.PGPIO_43 = GPIO_ID_EMUM_FUNC;
            }
            top_reg9.Bit.UART = UART_ENUM_UART;
        }

        if (config & PIN_UART_CFG_CH2)
        {
            if (config & PIN_UART_CFG_CH2_2ND)
            {
                if ((top_reg1.Bit.SDIO3_EXIST == SDIO3_EXIST_EN)  && (top_reg1.Bit.SDIO3_MUX_SEL == SDIO3_MUX_1ST))
                {
                    printf("UART2 conflict with to SDIO3_1: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg5.Bit.SPI3 == SPI3_1ST_PINMUX)
                {
                    printf("UART2 conflict with to SPI3_1: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH2_CTSRTS)
                {
                    top_reg_cgpio0.Bit.CGPIO_31 = GPIO_ID_EMUM_FUNC;
                    top_reg_cgpio1.Bit.CGPIO_32 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART2_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_cgpio0.Bit.CGPIO_29 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_30 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART2 = UART2_ENUM_2ND_PINMUX;
            }
            else
            {
                if (top_reg5.Bit.SPI4 == SPI4_2ND_PINMUX)
                {
                    printf("UART2 conflict with to SPI4_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
                {
                    printf("UART2 conflict with to AUDIO: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH2_CTSRTS)
                {
                    if ((top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)||(top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT))
                    {
                        printf("UART2 conflict with to PICNT_1 or PICNT2_1: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    top_reg_pgpio1.Bit.PGPIO_36 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio1.Bit.PGPIO_37 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART2_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_pgpio1.Bit.PGPIO_34 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_35 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART2 = UART2_ENUM_1ST_PINMUX;

            }
        }

        if (config & PIN_UART_CFG_CH3)
        {
            if (config & PIN_UART_CFG_CH3_2ND)
            {
                if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
                {
                    printf("UART3 conflict with to SPI3_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg10.Bit.I2C2_2 == I2C_ENUM_I2C)
                {
                    printf("UART3 conflict with to I2C2_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM8 == PWM_ENUM_PWM)
                {
                    printf("UART3 conflict with to PWM8: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM9 == PWM_ENUM_PWM)
                {
                    printf("UART3 conflict with to PWM9: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
                {
                    printf("UART3 conflict with to SIFCH2_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
                {
                    printf("UART3 conflict with to SIFCH3_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
                {
                    printf("UART3 conflict with to SENSOR6: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH3_CTSRTS)
                {
                    if (top_reg7.Bit.PWM10 == PWM_ENUM_PWM)
                    {
                        printf("UART3 conflict with to PWM10: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg7.Bit.PWM11 == PWM_ENUM_PWM)
                    {
                        printf("UART3 conflict with to PWM11: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg10.Bit.I2C3_2 == I2C_ENUM_I2C)
                    {
                        printf("UART3 conflict with to I2C3_2: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
                    {
                        printf("UART3 conflict with to SENSOR7: 0x%x\r\n", config);
                        return E_OBJ;
                    }

                    top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART3_CTSRTS = UART_CTSRTS_PINMUX;
                }
                top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART3 = UART3_ENUM_2ND_PINMUX;
            }
            else
            {
                if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
                {
                    printf("UART3 conflict with to SPI2_1: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if ((top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)||(top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF))
                {
                    printf("UART3 conflict with to SIF2 or SIF3: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg6.Bit.AUDIO_MCLK == AUDIO_MCLK_MCLK)
                {
                    printf("UART3 conflict with AUDIO MCLK\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.CANBUS== CANBUS_ENUM_CANBUS)
                {
                    printf("UART3 conflict with CANBUS\r\n");
                    return E_OBJ;
                }
                if (top_reg2.Bit.TV_TEST_CLK == TV_TEST_CLK_TV_CLK)
                {
                    printf("UART3 conflict with TV_TEST_CLK\r\n");
                    return E_OBJ;
                }
                if (top_reg4.Bit.LOCKN == LOCKN_ENUM_LOCKN)
                {
                    printf("UART3 conflict with LOCKN\r\n");
                    return E_OBJ;
                }
                if (top_reg4.Bit.LOCKN2 == LOCKN_ENUM_LOCKN)
                {
                    printf("UART3 conflict with LOCKN2\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
                {
                    printf("UART3 conflict with to SP_CLK_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
                {
                    printf("UART3 conflict with to SP_CLK2_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH3_CTSRTS)
                {
                    if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
                    {
                        printf("UART3 conflict with to ETH: 0x%x\r\n", config);
                        return E_OBJ;
                    }

                    top_reg_pgpio1.Bit.PGPIO_32 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio1.Bit.PGPIO_33 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART3_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_pgpio0.Bit.PGPIO_30 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_31 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART3 = UART3_ENUM_1ST_PINMUX;

            }
        }

        if (config & PIN_UART_CFG_CH4)
        {
            if (config & PIN_UART_CFG_CH4_2ND)
            {
                if (top_reg1.Bit.NAND_EXIST == NAND_EXIST_EN)
                {
                    printf("UART4 conflict with to NAND: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if ((top_reg10.Bit.I2C_2 == I2C_ENUM_I2C))
                {
                    printf("UART4 conflict with I2C_2\r\n");
                    return E_OBJ;
                }
                if ((top_reg10.Bit.I2C5_2 == I2C_ENUM_I2C))
                {
                    printf("UART4 conflict with I2C5_2\r\n");
                    return E_OBJ;
                }
                if ((top_reg11.Bit.TRACE_EN == TRACE_ENUM_TRACE))
                {
                    printf("UART4 conflict with TRACE\r\n");
                    return E_OBJ;
                }

                if (config & PIN_UART_CFG_CH4_CTSRTS)
                {
                    top_reg_cgpio0.Bit.CGPIO_14 = GPIO_ID_EMUM_FUNC;
                    top_reg_cgpio0.Bit.CGPIO_15 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART4_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_cgpio0.Bit.CGPIO_12 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_13 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART4 = UART4_ENUM_2ND_PINMUX;

            }
            else
            {
                if (top_reg7.Bit.PWM4 == PWM_ENUM_PWM)
                {
                    printf("UART4 conflict with to PWM4: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM5 == PWM_ENUM_PWM)
                {
                    printf("UART4 conflict with to PWM5: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
                {
                    printf("UART4 conflict with to SENSOR6: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg4.Bit.XVS_XHS4_EN == VD_HD_SEL_ENUM_SIE_VDHD)
                {
                    printf("UART4 conflict with to XVS_XHS4: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH4_CTSRTS)
                {
                    if (top_reg7.Bit.PWM6 == PWM_ENUM_PWM)
                    {
                        printf("UART4 conflict with to PWM6: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg7.Bit.PWM7 == PWM_ENUM_PWM)
                    {
                        printf("UART4 conflict with to PWM7: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg4.Bit.XVS_XHS5_EN == VD_HD_SEL_ENUM_SIE_VDHD)
                    {
                        printf("UART4 conflict with to XVS_XHS5: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART4_CTSRTS = UART_CTSRTS_PINMUX;
                }
                top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART4 = UART4_ENUM_1ST_PINMUX;
            }
        }
    }
    else
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    return E_OK;
}

static ER pinmux_config_spi(uint32_t config)
{
    if (config == PIN_SPI_CFG_NONE)
    {
    }
    else
    {
        if (config & (PIN_SPI_CFG_CH1|PIN_SPI_CFG_CH1_2ND_PINMUX|PIN_SPI_CFG_CH1_2BITS))
        {
            if (config & PIN_SPI_CFG_CH1_2ND_PINMUX)
            {
                if ((top_reg7.Bit.PWM16 == PWM_ENUM_PWM)||(top_reg4.Bit.MES_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)||
                    (top_reg10.Bit.I2C4 == I2C_ENUM_I2C)||(top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX))
                {
                    printf("SPI2_2 conflict with PWM16 or MES or I2C4 or SIF_2\r\n");
                    return E_OBJ;
                }
                else if ((top_reg7.Bit.PWM17 == PWM_ENUM_PWM)||(top_reg4.Bit.MES_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)||
                    (top_reg10.Bit.I2C4 == I2C_ENUM_I2C)||(top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_3RD_PINMUX))
                {
                    printf("SPI2_2 conflict with PWM17 or MES2 or I2C4 or SIF_3\r\n");
                    return E_OBJ;
                }
                else if ((top_reg7.Bit.PWM18 == PWM_ENUM_PWM)||(top_reg4.Bit.MES2_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)||
                    (top_reg10.Bit.I2C4_3 == I2C_ENUM_I2C)||(top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX))
                {
                    printf("SPI2_2 conflict with PWM18 or MES_2 or I2C4_3 or SIF_2\r\n");
                    return E_OBJ;
                }
                else if ((config & PIN_SPI_CFG_CH2_2BITS)&&
                        ((top_reg7.Bit.PWM19 == PWM_ENUM_PWM)||(top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)||
                          (top_reg10.Bit.I2C4_3 == I2C_ENUM_I2C)||(top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX)))
                {
                    printf("SPI2_2 conflict with PWM19 or MES_2_2 or I2C4_3 or SIF_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH1_2BITS)
                {
                    top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI = SPI_2ND_PINMUX;

            }
            else
            {
                if (config & PIN_SPI_CFG_CH1_2BITS)
                {
                    top_reg_pgpio1.Bit.PGPIO_41 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio1.Bit.PGPIO_38 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_39 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_40 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI = SPI_1ST_PINMUX;
            }
        }

        if (config & (PIN_SPI_CFG_CH2|PIN_SPI_CFG_CH2_2ND_PINMUX|PIN_SPI_CFG_CH2_3RD_PINMUX|PIN_SPI_CFG_CH2_2BITS))
        {
            if (config & PIN_SPI_CFG_CH2_3RD_PINMUX)
            {
                if (top_reg1.Bit.SDIO2_EXIST == SDIO2_EXIST_EN)
                {
                    printf("SPI2_3 conflict with SDIO2\r\n");
                    return E_OBJ;
                }
                else if (top_reg11.Bit.DSP_EJTAG_SEL)
                {
                    printf("SPI2_3 conflict with DSP_EJTAG\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    top_reg_cgpio0.Bit.CGPIO_24 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI2_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_cgpio0.Bit.CGPIO_22 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_23 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_25 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI2 = SPI2_3RD_PINMUX;

            }
            else if (config & PIN_SPI_CFG_CH2_2ND_PINMUX)
            {
                if ((top_reg7.Bit.PWM12 == PWM_ENUM_PWM) ||
                    (top_reg3.Bit.SENSOR7 == SENSOR7_ENUM_8BITS) )
                {
                    printf("SPI2_2 conflict with PWM12 or SIE7 8 bits\r\n");
                    return E_OBJ;
                }
                else if (top_reg7.Bit.PWM13 == PWM_ENUM_PWM)
                {
                    printf("SPI2_2 conflict with PWM13\r\n");
                    return E_OBJ;
                }
                else if ((top_reg7.Bit.PWM14 == PWM_ENUM_PWM) ||
                    (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS) )
                {
                    printf("SPI2_2 conflict with PWM14 or SIE6 16 bits\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    if (top_reg7.Bit.PWM15 == PWM_ENUM_PWM)
                    {
                        printf("SPI2_2 conflict with PWM15\r\n");
                        return E_OBJ;
                    }
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI2_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_12 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_13 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_14 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI2 = SPI2_2ND_PINMUX;
            }
            else
            {
                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SPI2_1 conflict with UART3 1st\r\n");
                    return E_OBJ;
                }
                else if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)
                {
                    printf("SPI2_1 conflict with SIF2\r\n");
                    return E_OBJ;
                }
                else if (top_reg6.Bit.AUDIO_MCLK == AUDIO_MCLK_MCLK)
                {
                    printf("SPI2_1 conflict with AUDIO MCLK\r\n");
                    return E_OBJ;
                }
                else if (top_reg6.Bit.CANBUS == CANBUS_ENUM_CANBUS)
                {
                    printf("SPI2_1 conflict with CANBUS\r\n");
                    return E_OBJ;
                }
                else if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
                {
                    printf("SPI2_1 conflict with SPCLK_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
                    {
                        printf("SPI2_1 conflict with SIF3\r\n");
                        return E_OBJ;
                    }
                    else if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
                    {
                        printf("SPI2_1 conflict with SP2_2_CLK\r\n");
                        return E_OBJ;
                    }
                    else if (top_reg6.Bit.CANBUS== CANBUS_ENUM_CANBUS)
                    {
                        printf("SPI2_1 conflict with CANBUS\r\n");
                        return E_OBJ;
                    }
                    else if (top_reg2.Bit.TV_TEST_CLK == TV_TEST_CLK_TV_CLK)
                    {
                        printf("SPI2_1 conflict with TV_TEST_CLK\r\n");
                        return E_OBJ;
                    }
                    else if (top_reg4.Bit.LOCKN2 == LOCKN_ENUM_LOCKN)
                    {
                        printf("SPI2_1 conflict with LOCKN2\r\n");
                        return E_OBJ;
                    }
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    top_reg_pgpio0.Bit.PGPIO_31 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI2_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_30 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_32 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_33 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI2 = SPI2_1ST_PINMUX;

            }
        }

        if (config & (PIN_SPI_CFG_CH3|PIN_SPI_CFG_CH3_2ND_PINMUX|PIN_SPI_CFG_CH3_2BITS|PIN_SPI_CFG_CH3_RDY|PIN_SPI_CFG_CH3_RDY_2ND_PINMUX))
        {
            if (config & PIN_SPI_CFG_CH3_2ND_PINMUX)
            {
                if ((top_reg7.Bit.PWM8 == PWM_ENUM_PWM) ||
                    (top_reg7.Bit.PWM10 == PWM_ENUM_PWM) ||
                    (top_reg7.Bit.PWM11 == PWM_ENUM_PWM) )
                {
                    printf("SPI3_2 conflict with PWM8/10/11\r\n");
                    return E_OBJ;
                }
                else if ((top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_8BITS) ||
                    (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS) ||
                    (top_reg3.Bit.SENSOR7 == SENSOR7_ENUM_8BITS) )
                {
                    printf("SPI3_2 conflict with SIE6 or SIE7\r\n");
                    return E_OBJ;
                }
                else if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
                {
                    printf("SPI3_2 conflict with UART3_2\r\n");
                    return E_OBJ;
                }
                else if (top_reg10.Bit.I2C2_2 == I2C_ENUM_I2C)
                {
                    printf("SPI3_2 conflict with I2C2_2\r\n");
                    return E_OBJ;
                }
                else if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
                {
                    printf("SPI3_2 conflict with SIFCH2_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH3_2BITS)
                {
                    if (top_reg7.Bit.PWM9 == PWM_ENUM_PWM)
                    {
                        printf("SPI3_2 conflict with PWM9\r\n");
                        return E_OBJ;
                    }
                    else if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
                    {
                        printf("SPI3_2 conflict with SIFCH3_2\r\n");
                        return E_OBJ;
                    }
                }
                if (config & PIN_SPI_CFG_CH3_2BITS)
                {
                    top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI3_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI3 = SPI3_2ND_PINMUX;
            }
            else
            {
                if (top_reg1.Bit.SDIO3_EXIST == SDIO3_EXIST_EN)
                {
                    printf("SPI3_1 conflict with SDIO3\r\n");
                    return E_OBJ;
                }
                else if (top_reg9.Bit.UART2 == UART2_ENUM_2ND_PINMUX)
                {
                    printf("SPI3_1 conflict with UART2_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH3_2BITS)
                {
                    top_reg_cgpio0.Bit.CGPIO_30 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI3_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_cgpio0.Bit.CGPIO_28 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_29 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_31 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI3 = SPI3_1ST_PINMUX;

            }

            if (config & PIN_SPI_CFG_CH3_RDY_2ND_PINMUX)
            {
                if (top_reg7.Bit.PWM3 == PWM_ENUM_PWM)
                {
                    printf("SPI3_RDY2 conflict with PWM3\r\n");
                    return E_OBJ;
                }
                else if (top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_1ST_PINMUX)
                {
                    printf("SPI3_RDY2 conflict with MES2_SHUT1\r\n");
                    return E_OBJ;
                }
                else if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
                {
                    printf("SPI3_RDY2 conflict with SENSOR6\r\n");
                    return E_OBJ;
                }
                else if ((top_reg4.Bit.XVS_XHS3_EN == VD_HD_SEL_ENUM_SIE_VDHD) &&
                    (top_reg4.Bit.XVS_XHS3_SEL == XVS_XHS_2ND_PINMUX) )
                {
                    printf("SPI3_RDY2 conflict with XVS_XHS3_2\r\n");
                    return E_OBJ;
                }

                top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;
                top_reg5.Bit.SPI3_RDY = SPI_RDY_2ND_PINMUX;
            }
            else if (config & PIN_SPI_CFG_CH3_RDY)
            {
                if (top_reg1.Bit.SDIO3_EXIST == SDIO3_EXIST_EN)
                {
                    printf("SPI3_RDY conflict with SDIO3\r\n");
                    return E_OBJ;
                }
                else if ( (top_reg9.Bit.UART2 == UART2_ENUM_2ND_PINMUX) &&
                    (top_reg9.Bit.UART2_CTSRTS = UART_CTSRTS_PINMUX) )
                {
                    printf("SPI3_RDY conflict with UART2_2 CTSRTS\r\n");
                    return E_OBJ;
                }

                top_reg_cgpio1.Bit.CGPIO_32 = GPIO_ID_EMUM_FUNC;
                top_reg5.Bit.SPI3_RDY = SPI_RDY_1ST_PINMUX;
            }
        }

        if (config & (PIN_SPI_CFG_CH4|PIN_SPI_CFG_CH4_2ND_PINMUX|PIN_SPI_CFG_CH4_3RD_PINMUX|PIN_SPI_CFG_CH4_2BITS))
        {
            if (config & PIN_SPI_CFG_CH4_3RD_PINMUX)
            {
                if (top_reg7.Bit.PI_CNT4 == PICNT_ENUM_PICNT2)
                {
                    printf("SPI4_3 conflict with PI_CNT4\r\n");
                    return E_OBJ;
                }
                else if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF)
                {
                    printf("SPI4_3 conflict with SIFCH0\r\n");
                    return E_OBJ;
                }
                else if (top_reg10.Bit.I2C4_4 == I2C_ENUM_I2C)
                {
                    printf("SPI4_3 conflict with I2C4_4\r\n");
                    return E_OBJ;
                }
                else if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
                {
                    printf("SPI4_3 conflict with ETH\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH4_2BITS)
                {
                    if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_3_CLK)
                    {
                        printf("SPI4_3 conflict with SP_CLK_3\r\n");
                        return E_OBJ;
                    }

                    top_reg_lgpio0.Bit.LGPIO_26 = GPIO_ID_EMUM_FUNC;
                    top_reg5.Bit.SPI4_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_lgpio0.Bit.LGPIO_27 = GPIO_ID_EMUM_FUNC;
                top_reg_lgpio0.Bit.LGPIO_28 = GPIO_ID_EMUM_FUNC;
                top_reg_lgpio0.Bit.LGPIO_29 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI4 = SPI4_3RD_PINMUX;
            }
            else if (config & PIN_SPI_CFG_CH4_2ND_PINMUX)
            {
                if (top_reg9.Bit.UART2 == UART2_ENUM_1ST_PINMUX)
                {
                    printf("SPI4_2 conflict with UART2_1\r\n");
                    return E_OBJ;
                }
                else if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
                {
                    printf("SPI4_2 conflict with I2S\r\n");
                    return E_OBJ;
                }
                else if (top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)
                {
                    printf("SPI4_2 conflict with PICNT_1\r\n");
                    return E_OBJ;
                }
                else if (config & PIN_SPI_CFG_CH4_2BITS)
                {
                    if (top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT)
                    {
                        printf("SPI4_2 conflict with PICNT2_1\r\n");
                        return E_OBJ;
                    }
                }

                if (config & PIN_SPI_CFG_CH4_2BITS)
                {
                    top_reg_pgpio1.Bit.PGPIO_37 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI4_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio1.Bit.PGPIO_34 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_35 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio1.Bit.PGPIO_36 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI4 = SPI4_2ND_PINMUX;
            }
            else
            {
                if ((top_reg10.Bit.I2C2 == I2C_ENUM_I2C) ||
                    (top_reg10.Bit.I2C3 == I2C_2_ENUM_I2C) )
                {
                    printf("SPI4_1 conflict with I2C2_1 or I2C3_1\r\n");
                    return E_OBJ;
                }
                else if (top_reg5.Bit.SIFCH7 == SIFCH_ENUM_SIF)
                {
                    printf("SPI4_1 conflict with SIF7\r\n");
                    return E_OBJ;
                }
                else if (top_reg4.Bit.SHUTTER_SEL == SHUTTER_ENUM_SHUTTER)
                {
                    printf("SPI4_1 conflict with SHUTTER\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH4_2BITS)
                {
                    if (top_reg5.Bit.SIFCH6 == SIFCH_ENUM_SIF)
                    {
                        printf("SPI4_1 conflict with SIF6\r\n");
                        return E_OBJ;
                    }
                    else if (top_reg4.Bit.STROBE_SEL == STROBE_ENUM_STROBE)
                    {
                        printf("SPI4_1 conflict with STROBE\r\n");
                        return E_OBJ;
                    }
                }

                if (config & PIN_SPI_CFG_CH4_2BITS)
                {
                    top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI4_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_sgpio0.Bit.SGPIO_8 = GPIO_ID_EMUM_FUNC;
                top_reg_sgpio0.Bit.SGPIO_9 = GPIO_ID_EMUM_FUNC;
                top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI4 = SPI4_1ST_PINMUX;
            }
        }
    }

    return E_OK;
}

static ER pinmux_config_remote(uint32_t config)
{
    if (config == PIN_REMOTE_CFG_NONE)
    {
    }
    else if (config & PIN_REMOTE_CFG_CH1)
    {
        if (top_reg7.Bit.PI_CNT3 == PICNT_ENUM_PICNT)
        {
            printf("REMOTE conflict with to CCNT3_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2CLK)
        {
            printf("REMOTE conflict with to SP_CLK2_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("REMOTE conflict with to SENSOR6 16Bits: 0x%x\r\n", config);
            return E_OBJ;
        }

        if (top_reg3.Bit.SENSOR7 == SENSOR7_ENUM_8BITS)
        {
            printf("REMOTE conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.REMOTE = REMOTE_ENUM_REMOTE;
    }
    else
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    return E_OK;
}

static ER pinmux_config_pwm(uint32_t config)
{
    if (config & PIN_PWM_CFG_PWM0)
    {
        if (top_reg4.Bit.MES_SH0_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM0 conflict with to MES_SHUT0: 0x%x\r\n", config);
            return E_OBJ;
        }
        /*if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM0 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }*/
        if (top_reg4.Bit.XVS_XHS6_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM0 conflict with to XVS_XHS6: 0x%x\r\n", config);
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM0 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM1)
    {
        if (top_reg4.Bit.MES_SH1_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM1 conflict with to MES_SHUT1: 0x%x\r\n", config);
            return E_OBJ;
        }
        /*if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM1 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }*/
        if (top_reg4.Bit.XVS_XHS6_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM1 conflict with to XVS_XHS6: 0x%x\r\n", config);
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM1 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM2)
    {
        if (top_reg4.Bit.MES2_SH0_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM2 conflict with to MES2_SHUT0: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM2 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }
        if ((top_reg4.Bit.XVS_XHS3_EN == VD_HD_SEL_ENUM_SIE_VDHD)&&(top_reg4.Bit.XVS_XHS3_SEL == XVS_XHS_2ND_PINMUX))
        {
            printf("PWM1 conflict with to XVS_XHS3_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM2 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM3)
    {
        if (top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM3 conflict with to MES2_SHUT1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI3_RDY == SPI_RDY_2ND_PINMUX)
        {
            printf("PWM3 conflict with to SPI3_2_RDY: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM3 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }
        if ((top_reg4.Bit.XVS_XHS3_EN == VD_HD_SEL_ENUM_SIE_VDHD)&&(top_reg4.Bit.XVS_XHS3_SEL == XVS_XHS_2ND_PINMUX))
        {
            printf("PWM3 conflict with to XVS_XHS3_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM3 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM4)
    {
        if (top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX)
        {
            printf("PWM4 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM4 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg4.Bit.XVS_XHS4_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM4 conflict with to XVS_XHS4: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM4 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM5)
    {
        if (top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX)
        {
            printf("PWM5 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM5 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg4.Bit.XVS_XHS4_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM5 conflict with to XVS_XHS4: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM5 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM6)
    {
        if (top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX)
        {
            printf("PWM6 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM6 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg4.Bit.XVS_XHS5_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM6 conflict with to XVS_XHS5: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM6 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM7)
    {
        if (top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX)
        {
            printf("PWM7 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM7 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg4.Bit.XVS_XHS5_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM7 conflict with to XVS_XHS5: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM7 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM8)
    {
        if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
        {
            printf("PWM8 conflict with to SPI3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
        {
            printf("PWM8 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C2_2 == I2C_ENUM_I2C)
        {
            printf("PWM8 conflict with I2C2_2\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM8 conflict with to SIFCH2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM8 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }


        top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM8 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM9)
    {
        if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
        {
            printf("PWM9 conflict with to SPI3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
        {
            printf("PWM9 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C2_2 == I2C_ENUM_I2C)
        {
            printf("PWM9 conflict with I2C2_2\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM9 conflict with to SIFCH3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM9 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM9 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM10)
    {
        if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
        {
            printf("PWM10 conflict with to SPI3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
        {
            printf("PWM10 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C3_2 == I2C_ENUM_I2C)
        {
            printf("PWM10 conflict with I2C3_2\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM10 conflict with to SIFCH3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM10 conflict with to SIFCH2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 != SENSOR6_ENUM_GPIO)
        {
            printf("PWM10 conflict with to SENSOR6: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM10 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM11)
    {
        if ((top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX) && (top_reg5.Bit.SPI3_DAT == SPI_DAT_ENUM_2Bit))
        {
            printf("PWM11 conflict with to SPI3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
        {
            printf("PWM11 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM11 conflict with to SIFCH3_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM11 conflict with to SIFCH2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM11 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM11 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM12)
    {
        if (top_reg5.Bit.SPI2 == SPI2_2ND_PINMUX)
        {
            printf("PWM12 conflict with to SPI2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        /*if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM12 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }*/
        if (top_reg4.Bit.XVS_XHS7_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM12 conflict with to XVS_XHS7: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_12 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM12 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM13)
    {
        if (top_reg5.Bit.SPI2 == SPI2_2ND_PINMUX)
        {
            printf("PWM13 conflict with to SPI2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        /*if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM13 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }*/
        if (top_reg4.Bit.XVS_XHS7_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM13 conflict with to XVS_XHS7: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_13 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM13 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM14)
    {
        if (top_reg5.Bit.SPI2 == SPI2_2ND_PINMUX)
        {
            printf("PWM14 conflict with to SPI2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM14 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("PWM14 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
            return E_OBJ;
        }

        if (top_reg4.Bit.XVS_XHS8_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM14 conflict with to XVS_XHS8: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_14 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM14 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM15)
    {
        if (top_reg5.Bit.SPI2 == SPI2_2ND_PINMUX)
        {
            printf("PWM15 conflict with to SPI2_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM15 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("PWM15 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg4.Bit.XVS_XHS8_EN == VD_HD_SEL_ENUM_SIE_VDHD)
        {
            printf("PWM15 conflict with to XVS_XHS8: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM15 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM16)
    {
        if (top_reg4.Bit.MES_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
        {
            printf("PWM16 conflict with to MES_2_SHUT0: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM16 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C4 == I2C_ENUM_I2C)
        {
            printf("PWM16 conflict with I2C4\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM16 conflict with SIFCH0_2\r\n");
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM16 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("PWM16 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM16 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM17)
    {
        if (top_reg4.Bit.MES_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
        {
            printf("PWM17 conflict with to MES_2_SHUT1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM17 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C4 == I2C_ENUM_I2C)
        {
            printf("PWM17 conflict with I2C4\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_3RD_PINMUX)
        {
            printf("PWM17 conflict with SIFCH0_3\r\n");
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM17 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("PWM17 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM17 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM18)
    {
        if (top_reg4.Bit.MES2_SH0_SEL == MES_SEL_ENUM_2ND_PINMUX)
        {
            printf("PWM18 conflict with to MES2_2_SHUT0: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM18 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C4_3 == I2C_ENUM_I2C)
        {
            printf("PWM18 conflict with I2C4_3\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM18 conflict with SIFCH0_3\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_3RD_PINMUX)
        {
            printf("PWM18 conflict with SIFCH0_3\r\n");
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM18 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("PWM18 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
            return E_OBJ;
        }


        top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM18 = PWM_ENUM_PWM;
    }
    if (config & PIN_PWM_CFG_PWM19)
    {
        if (top_reg4.Bit.MES2_SH1_SEL == MES_SEL_ENUM_2ND_PINMUX)
        {
            printf("PWM19 conflict with to MES2_2_SHUT1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM19 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C4_3 == I2C_ENUM_I2C)
        {
            printf("PWM19 conflict with I2C4_3\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_2ND_PINMUX)
        {
            printf("PWM19 conflict with SIFCH0_3\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF_3RD_PINMUX)
        {
            printf("PWM19 conflict with SIFCH0_3\r\n");
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
        {
            printf("PWM19 conflict with to SENSOR7: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
        {
            printf("PWM19 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM19 = PWM_ENUM_PWM;
    }

    if (config & (PIN_PWM_CFG_CCNT|PIN_PWM_CFG_CCNT_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT_2ND)
        {
            // without check LCD2?

            // without check LCD?

            // SIE8
            if (top_reg3.Bit.SENSOR8 != SENSOR8_ENUM_GPIO)
            {
                printf("PICNT conflict with to SENSOR8: 0x%x\r\n", config);
                return E_OBJ;
            }

            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT conflict with ETH\r\n");
                return E_OBJ;
            }

            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT = PICNT_ENUM_PICNT2;
        }
        else
        {
            if (top_reg5.Bit.SPI4 == SPI4_2ND_PINMUX)
            {
                printf("PICNT conflict with to SPI4_2: 0x%x\r\n", config);
                return E_OBJ;
            }
            if ((top_reg9.Bit.UART2 == UART2_ENUM_1ST_PINMUX) &&
                (top_reg9.Bit.UART2_CTSRTS == UART_CTSRTS_PINMUX))
            {
                printf("PICNT conflict with UART2_1\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
            {
                printf("PICNT conflict with AUDIO\r\n");
                return E_OBJ;
            }
            top_reg_pgpio1.Bit.PGPIO_36 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT = PICNT_ENUM_PICNT;
        }
    }
    if (config & (PIN_PWM_CFG_CCNT2|PIN_PWM_CFG_CCNT2_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT2_2ND)
        {
            // without check LCD2?

            // without check LCD?

            // SIE8
            if (top_reg3.Bit.SENSOR8 != SENSOR8_ENUM_GPIO)
            {
                printf("PICNT2 conflict with to SENSOR8: 0x%x\r\n", config);
                return E_OBJ;
            }

            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT2 conflict with ETH\r\n");
                return E_OBJ;
            }

            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT2 = PICNT_ENUM_PICNT2;
        }
        else
        {
            if ((top_reg5.Bit.SPI4 == SPI4_2ND_PINMUX) &&
                (top_reg5.Bit.SPI4_DAT == SPI_DAT_ENUM_2Bit))
            {
                printf("PICNT2 conflict with SPI5_2\r\n");
                return E_OBJ;
            }
            if ((top_reg9.Bit.UART2 == UART2_ENUM_1ST_PINMUX) &&
                (top_reg9.Bit.UART2_CTSRTS == UART_CTSRTS_PINMUX))
            {
                printf("PICNT2 conflict with UART2_1\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
            {
                printf("PICNT2 conflict with AUDIO\r\n");
                return E_OBJ;
            }
            top_reg_pgpio1.Bit.PGPIO_34 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT2 = PICNT_ENUM_PICNT;
        }
    }
    if (config & (PIN_PWM_CFG_CCNT3|PIN_PWM_CFG_CCNT3_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT3_2ND)
        {
            // without check LCD2?

            // without check LCD?

            // SIE8
            if (top_reg3.Bit.SENSOR8 != SENSOR8_ENUM_GPIO)
            {
                printf("PICNT3 conflict with to SENSOR8: 0x%x\r\n", config);
                return E_OBJ;
            }

            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT3 conflict with ETH\r\n");
                return E_OBJ;
            }

            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT3 = PICNT_ENUM_PICNT2;
        }
        else
        {
            if (top_reg6.Bit.REMOTE == REMOTE_ENUM_REMOTE)
            {
                printf("PICNT3 conflict with to REMOTE: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR7 != SENSOR7_ENUM_GPIO)
            {
                printf("PICNT3 conflict with to SENSOR7: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg3.Bit.SENSOR6 == SENSOR6_ENUM_16BITS)
            {
                printf("PICNT3 conflict with to SENSOR6 16 Bits: 0x%x\r\n", config);
                return E_OBJ;
            }
            top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT3 = PICNT_ENUM_PICNT;
        }
    }
    if (config & (PIN_PWM_CFG_CCNT4|PIN_PWM_CFG_CCNT4_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT4_2ND)
        {
            // without check LCD?

            if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF)
            {
                printf("PICNT4 conflict with to SIFCH0_1: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg5.Bit.SPI4 == SPI4_3RD_PINMUX)
            {
                printf("PICNT4 conflict with to SPI4_3: 0x%x\r\n", config);
                return E_OBJ;
            }
            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT4 conflict with ETH\r\n");
                return E_OBJ;
            }


            top_reg_lgpio0.Bit.LGPIO_27 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT4 = PICNT_ENUM_PICNT2;
        }
        else
        {
            if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SPCLK)
            {
                printf("PICNT4 conflict with to SP_CLK: 0x%x\r\n", config);
                return E_OBJ;
            }
            top_reg_pgpio0.Bit.PGPIO_22 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT4 = PICNT_ENUM_PICNT;
        }
    }

    return E_OK;
}

static ER pinmux_config_audio(uint32_t config)
{
    if (config & (PIN_AUDIO_CFG_I2S))
    {

        if (top_reg5.Bit.SPI4 == SPI4_2ND_PINMUX)
        {
            printf("AUDIO conflict with SPI4_2\r\n");
            return E_OBJ;
        }
        if (top_reg9.Bit.UART2 == UART2_ENUM_1ST_PINMUX)
        {
            printf("AUDIO conflict with UART2_1\r\n");
            return E_OBJ;
        }
        if (top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)
        {
            printf("AUDIO conflict with PICNT_1\r\n");
            return E_OBJ;
        }
        if (top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT)
        {
            printf("AUDIO conflict with PICNT2_1\r\n");
            return E_OBJ;
        }
        top_reg_pgpio1.Bit.PGPIO_34 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio1.Bit.PGPIO_35 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio1.Bit.PGPIO_36 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio1.Bit.PGPIO_37 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.AUDIO = AUDIO_ENUM_I2S;

    }

    if (config & (PIN_AUDIO_CFG_MCLK))
    {

        if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
        {
            printf("AUDIO MCLK conflict with SPI2_1\r\n");
            return E_OBJ;
        }
        if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
        {
            printf("AUDIO MCLK conflict with UART3_1\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)
        {
            printf("AUDIO MCLK conflict with to SIFCH2_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg6.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
        {
            printf("AUDIO MCLK conflict with SP_CLK_2\r\n");
            return E_OBJ;
        }
        if (top_reg4.Bit.LOCKN == LOCKN_ENUM_LOCKN)
        {
            printf("AUDIO MCLK conflict with LOCKN\r\n");
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_30 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.AUDIO_MCLK = AUDIO_MCLK_MCLK;

    }

    return E_OK;
}

static ER pinmux_config_lcd(uint32_t config)
{
    uint32_t tmp;

    tmp = config & PINMUX_DISPMUX_SEL_MASK;
    if (tmp == PINMUX_DISPMUX_SEL_MASK)
    {
        printf("invalid locate: 0x%x\r\n", config);
        return E_PAR;
    }

    disp_pinmux_config[PINMUX_FUNC_ID_LCD] = config;

    return E_OK;
}

static ER pinmux_config_lcd2(uint32_t config)
{
    uint32_t tmp;

    tmp = config & PINMUX_DISPMUX_SEL_MASK;
    if (tmp == PINMUX_DISPMUX_SEL_MASK)
    {
        printf("invalid locate: 0x%x\r\n", config);
        return E_PAR;
    }

    disp_pinmux_config[PINMUX_FUNC_ID_LCD2] = config;

    return E_OK;
}

static ER pinmux_config_tv(uint32_t config)
{
    uint32_t tmp;

    tmp = config & PINMUX_TV_HDMI_CFG_MASK;
    if ((tmp != PINMUX_TV_HDMI_CFG_NORMAL) &&
        (tmp != PINMUX_TV_HDMI_CFG_PINMUX_ON))
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    disp_pinmux_config[PINMUX_FUNC_ID_TV] = config;

    return E_OK;
}

static ER pinmux_config_hdmi(uint32_t config)
{
    uint32_t tmp;

    tmp = config & PINMUX_TV_HDMI_CFG_MASK;
    if ((tmp != PINMUX_TV_HDMI_CFG_NORMAL) &&
        (tmp != PINMUX_TV_HDMI_CFG_PINMUX_ON))
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    if (config & PINMUX_HDMI_CFG_HOTPLUG)
    {
        top_reg_pgpio1.Bit.PGPIO_47 = GPIO_ID_EMUM_FUNC;

        //top_reg2.Bit.HOTPLUG = HOTPLUG_HOTPLUG;
    }

    if (config & PINMUX_HDMI_CFG_CEC)
    {
        top_reg_pgpio1.Bit.PGPIO_44 = GPIO_ID_EMUM_FUNC;

        //top_reg2.Bit.CEC = CEC_ENUM_CEC;
    }

    disp_pinmux_config[PINMUX_FUNC_ID_HDMI] = config;

    return E_OK;
}

static ER pinmux_config_eth(uint32_t config)
{
    //uint32_t uiLCDMode;

    if (config == PIN_ETH_CFG_NONE)
    {
    }
    else if (config & (PIN_ETH_CFG_MII|PIN_ETH_CFG_RMII|PIN_ETH_CFG_GMII|PIN_ETH_CFG_RGMII|PIN_ETH_CFG_REVMII_10_100|PIN_ETH_CFG_REVMII_10_1000))
    {
        // check MDIO/MDC
        if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF)
        {
            printf("ETH conflict with SIF0\r\n");
            return E_OBJ;
        }
        if (top_reg10.Bit.I2C4_4 == I2C_ENUM_I2C)
        {
            printf("ETH conflict with I2C4_4\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI4 == SPI4_3RD_PINMUX)
        {
            printf("ETH conflict with SPI4_3\r\n");
            return E_OBJ;
        }
        top_reg_lgpio0.Bit.LGPIO_28 = GPIO_ID_EMUM_FUNC;
        top_reg_lgpio0.Bit.LGPIO_29 = GPIO_ID_EMUM_FUNC;

        //check lcd27 ??
        if (config & (PIN_ETH_CFG_GMII|PIN_ETH_CFG_RMII|PIN_ETH_CFG_RGMII|PIN_ETH_CFG_REVMII_10_1000))
        {
            top_reg_lgpio0.Bit.LGPIO_27 = GPIO_ID_EMUM_FUNC;
        }

        if (config & PIN_ETH_CFG_MII)
        {
            top_reg_lgpio0.Bit.LGPIO_11 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_26 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_MII;
        }
        else if (config & PIN_ETH_CFG_RMII)
        {
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_RMII;
        }
        else if (config & PIN_ETH_CFG_GMII)
        {
            top_reg_lgpio0.Bit.LGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_1 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_3 = GPIO_ID_EMUM_FUNC;

            top_reg_lgpio0.Bit.LGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_11 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_26 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_GMII;
        }
        else if (config & PIN_ETH_CFG_RGMII)
        {
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_26 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_RGMII;
        }
        else if (config & PIN_ETH_CFG_REVMII_10_100)
        {
            top_reg_lgpio0.Bit.LGPIO_11 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_26 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_REVMII_10_100;
        }
        else if (config & PIN_ETH_CFG_REVMII_10_1000)
        {
            top_reg_lgpio0.Bit.LGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_1 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_3 = GPIO_ID_EMUM_FUNC;

            top_reg_lgpio0.Bit.LGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_11 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_18 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_24 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_26 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_REVMII_10_1000;
        }
    }
    return E_OK;
}

static ER pinmux_config_canbus(uint32_t config)
{

    if (config & (PIN_CANBUS_CFG_CANBUS))
    {
        if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
        {
            printf("CANBUS conflict with SPI2_1\r\n");
            return E_OBJ;
        }
        if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
        {
            printf("CANBUS conflict with UART3 1st\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)
        {
            printf("CANBUS conflict with SIF2_1\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
        {
            printf("CANBUS conflict with SIF3_1\r\n");
            return E_OBJ;
        }
        if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
        {
            printf("CANBUS conflict with SP2_2_CLK\r\n");
            return E_OBJ;
        }
        if (top_reg2.Bit.TV_TEST_CLK == TV_TEST_CLK_TV_CLK)
        {
            printf("CANBUS conflict with TV_TEST_CLK\r\n");
            return E_OBJ;
        }
        if (top_reg4.Bit.LOCKN2 == LOCKN_ENUM_LOCKN)
        {
            printf("CANBUS conflict with LOCKN2\r\n");
            return E_OBJ;
        }

        top_reg6.Bit.CANBUS = CANBUS_ENUM_CANBUS;

        top_reg_pgpio0.Bit.PGPIO_31 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio1.Bit.PGPIO_32 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio1.Bit.PGPIO_33 = GPIO_ID_EMUM_FUNC;
    }
	return E_OK;
}

#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include "../cc/cc.h"
#include <asm/arch/nvt_common.h>

#define loc_multiCores()    {loc_cpu();cc_hwLockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_TOP);}
#define unl_multiCores()    {cc_hwUnlockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_TOP);unl_cpu();}

#define PINMUX_DSP_CH_INACTIVE	 0
#define PINMUX_DSP_CH_ACTIVE	 1
/*
    Active DSP PERI channel

    Active DSP PERI channel

    @param[in] pinmux   pinmux setting
                        - @b PINMUX_DSP_CH_ACTIVE
                        - @b PINMUX_DSP_CH_INACTIVE

    @return
        - @b E_OK: success
        - @b E_PAR: invalid pinmux
*/
ER pinmux_activeDSPPERI(uint32_t pinmux)
{
    loc_multiCores();
    top_reg11.Reg = TOP_GETREG(TOP_REG11_OFS);
    switch (pinmux)
    {
    case PINMUX_DSP_CH_ACTIVE:
        top_reg11.Bit.DSP_PERI_EN = 1;
        break;
    case PINMUX_DSP_CH_INACTIVE:
        top_reg11.Bit.DSP_PERI_EN = 0;
        break;
    default:
        unl_multiCores();
        printf("invalid pinmux: %d\r\n", pinmux);
        return E_PAR;
    }
    TOP_SETREG(TOP_REG11_OFS, top_reg11.Reg);
    unl_multiCores();

    return E_OK;
}

/*
    Active DSP IOP channel

    Active DSP IOP channel

    @param[in] pinmux   pinmux setting
                        - @b PINMUX_DSP_CH_ACTIVE
                        - @b PINMUX_DSP_CH_INACTIVE

    @return
        - @b E_OK: success
        - @b E_PAR: invalid pinmux
*/
ER pinmux_activeDSPIOP(uint32_t pinmux)
{
    loc_multiCores();
    top_reg11.Reg = TOP_GETREG(TOP_REG11_OFS);
    switch (pinmux)
    {
    case PINMUX_DSP_CH_ACTIVE:
        top_reg11.Bit.DSP_IOP_EN = 1;
        break;
    case PINMUX_DSP_CH_INACTIVE:
        top_reg11.Bit.DSP_IOP_EN = 0;
        break;
    default:
        unl_multiCores();
        printf("invalid pinmux: %d\r\n", pinmux);
        return E_PAR;
    }
    TOP_SETREG(TOP_REG11_OFS, top_reg11.Reg);
    unl_multiCores();

    return E_OK;
}

/*
    Active DSP2 PERI channel

    Active DSP2 PERI channel

    @param[in] pinmux   pinmux setting
                        - @b PINMUX_DSP_CH_ACTIVE
                        - @b PINMUX_DSP_CH_INACTIVE

    @return
        - @b E_OK: success
        - @b E_PAR: invalid pinmux
*/
ER pinmux_activeDSP2PERI(uint32_t pinmux)
{
    loc_multiCores();
    top_reg11.Reg = TOP_GETREG(TOP_REG11_OFS);
    switch (pinmux)
    {
    case PINMUX_DSP_CH_ACTIVE:
        top_reg11.Bit.DSP2_PERI_EN = 1;
        break;
    case PINMUX_DSP_CH_INACTIVE:
        top_reg11.Bit.DSP2_PERI_EN = 0;
        break;
    default:
        unl_multiCores();
        printf("invalid pinmux: %d\r\n", pinmux);
        return E_PAR;
    }
    TOP_SETREG(TOP_REG11_OFS, top_reg11.Reg);
    unl_multiCores();

    return E_OK;
}

/*
    Active DSP2 IOP channel

    Active DSP2 IOP channel

    @param[in] pinmux   pinmux setting
                        - @b PINMUX_DSP_CH_ACTIVE
                        - @b PINMUX_DSP_CH_INACTIVE

    @return
        - @b E_OK: success
        - @b E_PAR: invalid pinmux
*/
ER pinmux_activeDSP2IOP(uint32_t pinmux)
{

    loc_multiCores();
    top_reg11.Reg = TOP_GETREG(TOP_REG11_OFS);
    switch (pinmux)
    {
    case PINMUX_DSP_CH_ACTIVE:
        top_reg11.Bit.DSP2_IOP_EN = 1;
        break;
    case PINMUX_DSP_CH_INACTIVE:
        top_reg11.Bit.DSP2_IOP_EN = 0;
        break;
    default:
        unl_multiCores();
        printf("invalid pinmux: %d\r\n", pinmux);
        return E_PAR;
    }
    TOP_SETREG(TOP_REG11_OFS, top_reg11.Reg);
    unl_multiCores();

    return E_OK;
}
