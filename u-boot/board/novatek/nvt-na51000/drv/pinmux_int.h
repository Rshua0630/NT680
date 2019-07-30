/*
	Pinmux module internal header file

	Pinmux module internal header file

	@file		pinmux_int.h
	@ingroup
	@note		Nothing

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _PINMUX_INT_H
#define _PINMUX_INT_H

#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>
#include "nvt_type.h"

#define TOP_REG_ADDR(ofs)       (IOADDR_TOP_REG_BASE+(ofs))
#define TOP_GETREG(ofs)         INW(TOP_REG_ADDR(ofs))
#define TOP_SETREG(ofs,value)   OUTW(TOP_REG_ADDR(ofs), (value))

#define TGE_REG_ADDR(ofs)       (IOADDR_TGE_REG_BASE+(ofs))
#define TGE_GETREG(ofs)         INW(TGE_REG_ADDR(ofs))
#define TGE_SETREG(ofs,value)   OUTW(TGE_REG_ADDR(ofs), (value))
#define TGE_CONTROL_OFS 0x0

typedef enum
{
    BOOT_SRC_NAND_RS,                   //< Boot from NAND (Reed Solomon)
    BOOT_SRC_SDIO,                      //< Boot from SDIO
    BOOT_SRC_EMMC,                      //< Boot from eMMC (SDIO3 2nd pinmux)
    BOOT_SRC_USB_FS,                    //< Boot from USB (Full Speed)
    BOOT_SRC_SPI,                       //< Boot from SPI
    BOOT_SRC_USB_HS,                    //< Boot from USB (High Speed)
    BOOT_SRC_SPI_NAND,                  //< Boot from SPI_NAND
    BOOT_SRC_BMC,                       //< Boot from BMC
    BOOT_SRC_ETH,

    ENUM_DUMMY4WORD(BOOT_SRC_ENUM)
} BOOT_SRC_ENUM;


/*
    EJTAG_ENUM
*/
typedef enum
{
    EJTAG_GPIO,                         //< Disable EJTAG (ICE interface)
    EJTAG_EN,                           //< Enable EJTAG

    ENUM_DUMMY4WORD(EJTAG_ENUM)
} EJTAG_ENUM;

/*
    MPLL_CLK_ENUM
*/
typedef enum
{
    MPLL_CLK_NORMAL,                    //< PLL2~13 from internal MPLL
    MPLL_CLK_BYPASS,                    //< Bypass MPLL (From external pad)

    ENUM_DUMMY4WORD(MPLL_CLK_ENUM)
} MPLL_CLK_ENUM;

/*
    NAND_ACT_ENUM
*/
typedef enum
{
    NAND_ACT_DIS,                       //< De-activate NAND ACT
    NAND_ACT_EN,                        //< Activate NAND ACT

    ENUM_DUMMY4WORD(NAND_ACT_ENUM)
} NAND_ACT_ENUM;

/*
    FSPI_ACT_ENUM
*/
typedef enum
{
    FSPI_ACT_DIS,                        //< De-activate FSPI ACT
    FSPI_ACT_EN,                         //< Activate FSPI ACT

    ENUM_DUMMY4WORD(FSPI_ACT_ENUM)
} FSPI_ACT_ENUM;

/*
    SDIO2_ACT_ENUM
*/
typedef enum
{
    SDIO2_ACT_DIS,                      //< De-activate SDIO2 ACT
    SDIO2_ACT_EN,                       //< Activate SDIO2 ACT

    ENUM_DUMMY4WORD(SDIO2_ACT_ENUM)
} SDIO2_ACT_ENUM;

/*
    NAND_EXIST_ENUM
*/
typedef enum
{
    NAND_EXIST_DIS,                     //< De-activate NAND EXIST
    NAND_EXIST_EN,                      //< Activate NAND EXIST

    ENUM_DUMMY4WORD(NAND_EXIST_ENUM)
} NAND_EXIST_ENUM;

/*
    FSPI_EXIST_ENUM
*/
typedef enum
{
    FSPI_EXIST_DIS,                      //< De-activate FSPI EXIST
    FSPI_EXIST_EN,                       //< Activate FSPI EXIST

    ENUM_DUMMY4WORD(FSPI_EXIST_ENUM)
} FSPI_EXIST_ENUM;

/*
    SDIO_EXIST_ENUM
*/
typedef enum
{
    SDIO_EXIST_DIS,                     //< De-activate SDIO EXIST
    SDIO_EXIST_EN,                      //< Activate SDIO EXIST

    ENUM_DUMMY4WORD(SDIO_EXIST_ENUM)
} SDIO_EXIST_ENUM;

/*
    SDIO2_EXIST_ENUM
*/
typedef enum
{
    SDIO2_EXIST_DIS,                    //< De-activate SDIO2 EXIST
    SDIO2_EXIST_EN,                     //< Activate SDIO2 EXIST

    ENUM_DUMMY4WORD(SDIO2_EXIST_ENUM)
} SDIO2_EXIST_ENUM;

/*
    SDIO3_EXIST_ENUM
*/
typedef enum
{
    SDIO3_EXIST_DIS,                    //< De-activate SDIO3 EXIST
    SDIO3_EXIST_EN,                     //< Activate SDIO3 EXIST

    ENUM_DUMMY4WORD(SDIO3_EXIST_ENUM)
} SDIO3_EXIST_ENUM;

/*
    EXTROM_EXIST_ENUM
*/
typedef enum
{
    EXTROM_EXIST_DIS,                   //< De-activate EXTROM ACT
    EXTROM_EXIST_EN,                    //< Activate EXTROM ACT

    ENUM_DUMMY4WORD(EXTROM_EXIST_ENUM)
} EXTROM_EXIST_ENUM;

/*
    SDIO3_MUX_ENUM
*/
typedef enum
{
    SDIO3_MUX_1ST,                      //< 1st pinmux pad
    SDIO3_MUX_2ND,                      //< 2nd pinmux pad

    ENUM_DUMMY4WORD(SDIO3_MUX_ENUM)
} SDIO3_MUX_ENUM;

/*
    SPI_MUX_ENUM
*/
typedef enum
{
    SPI_MUX_GPIO,                       //< GPIO
    SPI_MUX_1ST,                        //< 1st pinmux pad
    SPI_MUX_2ND,                        //< 2nd pinmux pad
    SPI_MUX_3RD,                        //< 3rd pinmux pad

    ENUM_DUMMY4WORD(SPI_MUX_ENUM)
} SPI_MUX_ENUM;

/*
    ROM_SRC_ENUM
*/
typedef enum
{
    ROM_SRC_INTERNAL,                   //< Internal ROM
    ROM_SRC_EXTERNAL,                   //< External ROM

    ENUM_DUMMY4WORD(ROM_SRC_ENUM)
} ROM_SRC_ENUM;

/*
    NAND_CS_NUM_ENUM
*/
typedef enum
{
    NAND_CS_NUM_1CS,                    //< 1 CS Pin
    NAND_CS_NUM_2CS,                    //< 2 CS Pin

    ENUM_DUMMY4WORD(NAND_CS_NUM_ENUM)
} NAND_CS_NUM_ENUM;

/*
    SPI_BUS_WIDTH_ENUM
*/
typedef enum
{
    SPI_BUS_WIDTH_2BITS,                //< SPI is 2 bits
    SPI_BUS_WIDTH_4BITS,                //< SPI is 4 bits

    ENUM_DUMMY4WORD(SPI_BUS_WIDTH_ENUM)
} SPI_BUS_WIDTH_ENUM;

/*
    SDIO2_BUS_WIDTH_ENUM
*/
typedef enum
{
    SDIO2_BUS_WIDTH_4BITS,              //< SDIO2 is 4 bits
    SDIO2_BUS_WIDTH_8BITS,              //< SDIO2 is 8 bits (no use, Backward compatible)

    ENUM_DUMMY4WORD(SDIO2_BUS_WIDTH_ENUM)
} SDIO2_BUS_WIDTH_ENUM;

/*
    SDIO3_BUS_WIDTH_ENUM
*/
typedef enum
{
    SDIO3_BUS_WIDTH_4BITS,              //< SDIO3 is 4 bits
    SDIO3_BUS_WIDTH_8BITS,              //< SDIO3 is 8 bits

    ENUM_DUMMY4WORD(SDIO3_BUS_WIDTH_ENUM)
} SDIO3_BUS_WIDTH_ENUM;


/*
    LCDTYPE_ENUM
*/
typedef enum
{
    LCDTYPE_ENUM_GPIO,                  //< GPIO
    LCDTYPE_ENUM_CCIR656,               //< CCIR656
    LCDTYPE_ENUM_CCIR601,               //< CCIR601
    LCDTYPE_ENUM_PARALLEL_LCD,          //< Parallel LCD (RGB565, RGB666, RGB888)
    LCDTYPE_ENUM_SerialRGB_8BITS,       //< Serial RGB 8 bits
    LCDTYPE_ENUM_SerialRGB_6BITS,       //< Serial RGB 6 bits
    LCDTYPE_ENUM_SerialYCbCr_8BITS,     //< Serial YCbCr 8 bits
    LCDTYPE_ENUM_RGB_16BITS,            //< RGB 16 bits
    LCDTYPE_ENUM_MIPI_DSI,              //< MIPI DSI

    ENUM_DUMMY4WORD(LCDTYPE_ENUM)
} LCDTYPE_ENUM;

/*
    PLCD_DATA_WIDTH_ENUM
*/
typedef enum
{
    PLCD_DATA_WIDTH_RGB888,             //< RGB888
    PLCD_DATA_WIDTH_RGB666,             //< RGB666
    PLCD_DATA_WIDTH_RGB565,             //< RGB565

    ENUM_DUMMY4WORD(PLCD_DATA_WIDTH_ENUM)
} PLCD_DATA_WIDTH_ENUM;

/*
    CCIR_DATA_WIDTH_ENUM
*/
typedef enum
{
    CCIR_DATA_WIDTH_8BITS,              //< CCIR 8 bits
    CCIR_DATA_WIDTH_16BITS,             //< CCIR 16 bits

    ENUM_DUMMY4WORD(CCIR_DATA_WIDTH_ENUM)
} CCIR_DATA_WIDTH_ENUM;

/*
    MEMIF_TYPE_ENUM
*/
typedef enum
{
    MEMIF_TYPE_GPIO,                    //< GPIO
    MEMIF_TYPE_SERIAL,                  //< Select serial MI
    MEMIF_TYPE_PARALLEL,                //< Select parallel MI

    ENUM_DUMMY4WORD(MEMIF_TYPE_ENUM)
} MEMIF_TYPE_ENUM;

/*
    PMEM_SEL_ENUM
*/
typedef enum
{
    PMEM_SEL_ENUM_1ST_PINMUX,           //< Primary Parallel MI pinmux (up to 18 bits)
    PMEM_SEL_ENUM_2ND_PINMUX,           //< Secondary Parallel MI pinmux (up to 9 bits)

    ENUM_DUMMY4WORD(PMEM_SEL_ENUM)
} PMEM_SEL_ENUM;

/*
    PMEMIF_DATA_WIDTH_ENUM
*/
typedef enum
{
    PMEMIF_DATA_WIDTH_8BITS,            //< Parallel MI bus width is 8 bits
    PMEMIF_DATA_WIDTH_9BITS,            //< Parallel MI bus width is 9 bits
    PMEMIF_DATA_WIDTH_16BITS,           //< Parallel MI bus width is 16 bits
    PMEMIF_DATA_WIDTH_18BITS,           //< Parallel MI bus width is 18 bits

    ENUM_DUMMY4WORD(PMEMIF_DATA_WIDTH_ENUM)
} PMEMIF_DATA_WIDTH_ENUM;

/*
    SMEMIF_DATA_WIDTH_ENUM
*/
typedef enum
{
    SMEMIF_DATA_WIDTH_1BITS,            //< Serial MI bus width is 1 bit
    SMEMIF_DATA_WIDTH_2BITS,            //< Serial MI bus width is 2 bits

    ENUM_DUMMY4WORD(SMEMIF_DATA_WIDTH_ENUM)
} SMEMIF_DATA_WIDTH_ENUM;

/*
    PLCD_DE_ENUM
*/
typedef enum
{
    PLCD_DE_ENUM_GPIO,                  //< GPIO
    PLCD_DE_ENUM_DE,                    //< DE

    ENUM_DUMMY4WORD(PLCD_DE_ENUM)
} PLCD_DE_ENUM;

/*
    CCIR_HVLD_VVLD_ENUM
*/
typedef enum
{
    CCIR_HVLD_VVLD_GPIO,                //< GPIO
    CCIR_HVLD_VVLD_EN,                  //< HVLD/VVLD enable

    ENUM_DUMMY4WORD(CCIR_HVLD_VVLD_ENUM)
} CCIR_HVLD_VVLD_ENUM;

/*
    CCIR_FIELD_ENUM
*/
typedef enum
{
    CCIR_FIELD_GPIO,                    //< GPIO
    CCIR_FIELD_EN,                      //< FIELD enable

    ENUM_DUMMY4WORD(CCIR_FIELD_ENUM)
} CCIR_FIELD_ENUM;

/*
    TE_SEL_ENUM
*/
typedef enum
{
    TE_SEL_GPIO,                        //< GPIO
    TE_SEL_EN,                          //< TE enable

    ENUM_DUMMY4WORD(TE_SEL_ENUM)
} TE_SEL_ENUM;

/*
    LCDTYPE2_ENUM
*/
typedef enum
{
    LCDTYPE2_ENUM_GPIO,                 //< GPIO
    LCDTYPE2_ENUM_CCIR656,              //< CCIR656
    LCDTYPE2_ENUM_CCIR601,              //< CCIR601
    LCDTYPE2_ENUM_SerialRGB_8BITS=4,    //< Serial RGB 8 bits
    LCDTYPE2_ENUM_SerialRGB_6BITS,      //< Serial RGB 6 bits
    LCDTYPE2_ENUM_SerialYCbCr_8BITS,    //< Serial YCbCr 8 bits
    LCDTYPE2_ENUM_ParallelMI,           //< Parallel MI
    LCDTYPE2_ENUM_SerialMI,             //< Serial MI

    ENUM_DUMMY4WORD(LCDTYPE2_ENUM)
} LCDTYPE2_ENUM;

/*
    PMEMIF2_DATA_WIDTH_ENUM
*/
typedef enum
{
    PMEMIF2_DATA_WIDTH_8BITS,           //< Parallel MI 2 bus width is 8 bits
    PMEMIF2_DATA_WIDTH_9BITS,           //< Parallel MI 2 bus width is 9 bits

    ENUM_DUMMY4WORD(PMEMIF2_DATA_WIDTH_ENUM)
} PMEMIF2_DATA_WIDTH_ENUM;

/*
    SMEMIF2_DATA_WIDTH_ENUM
*/
typedef enum
{
    SMEMIF2_DATA_WIDTH_1BITS,           //< Serial MI 2 bus width is 1 bit
    SMEMIF2_DATA_WIDTH_2BITS,           //< Serial MI 2 bus width is 2 bits

    ENUM_DUMMY4WORD(SMEMIF2_DATA_WIDTH_ENUM)
} SMEMIF2_DATA_WIDTH_ENUM;

/*
    CEC_ENUM
*/
typedef enum
{
    CEC_ENUM_GPIO,                      //< GPIO
    CEC_ENUM_CEC,                       //< CEC

    ENUM_DUMMY4WORD(CEC_ENUM)
} CEC_ENUM;

/*
    DDC_ENUM
*/
typedef enum
{
    DDC_ENUM_GPIO,                      //< GPIO
    DDC_ENUM_DDC,                       //< DDC

    ENUM_DUMMY4WORD(DDC_ENUM)
} DDC_ENUM;

/*
    HOTPLUG_ENUM
*/
typedef enum
{
    HOTPLUG_ENUM_GPIO,                  //< GPIO
    HOTPLUG_HOTPLUG,                    //< HOTPLUG

    ENUM_DUMMY4WORD(HOTPLUG_ENUM)
} HOTPLUG_ENUM;

/*
    SENSOR_ENUM
*/
typedef enum
{
    SENSOR_ENUM_GPIO,                   //< GPIO
    SENSOR_ENUM_8BITS,                  //< 8 bits sensor
    SENSOR_ENUM_10BITS,                 //< 10 bits sensor
    SENSOR_ENUM_12BITS,                 //< 12 bits sensor
    SENSOR_ENUM_16BITS,                 //< 16 bits sensor
//    SENSOR_ENUM_CCIR8BITS,              //< CCIR 8 bits sensor
//    SENSOR_ENUM_CCIR16BITS,             //< CCIR 16 bits sensor

    ENUM_DUMMY4WORD(SENSOR_ENUM)
} SENSOR_ENUM;

/*
    SENSOR2_ENUM
*/
typedef enum
{
    SENSOR2_ENUM_GPIO,                   //< GPIO
    SENSOR2_ENUM_CCIR8BITS,              //< CCIR 8 bits sensor
    SENSOR2_ENUM_10BITS,                 //< 10 bit sensor
    SENSOR2_ENUM_CCIR16BITS,             //< CCIR 16 bits sensor

    ENUM_DUMMY4WORD(SENSOR2_ENUM)
} SENSOR2_ENUM;

/*
    SENSOR3_ENUM
*/
typedef enum
{
    SENSOR3_ENUM_GPIO,                   //< GPIO
    SENSOR3_ENUM_8BITS,                  //< 8 bits sensor

    ENUM_DUMMY4WORD(SENSOR3_ENUM)
} SENSOR3_ENUM;

/*
    SENSOR4_ENUM
*/
typedef enum
{
    SENSOR4_ENUM_GPIO,                   //< GPIO
    SENSOR4_ENUM_8BITS,                  //< 8 bits sensor

    ENUM_DUMMY4WORD(SENSOR4_ENUM)
} SENSOR4_ENUM;

/*
    SENSOR6_ENUM
*/
typedef enum
{
    SENSOR6_ENUM_GPIO,                   //< GPIO
    SENSOR6_ENUM_8BITS,                  //< 8 bits sensor
    SENSOR6_ENUM_16BITS,                 //< 16 bits sensor

    ENUM_DUMMY4WORD(SENSOR6_ENUM)
} SENSOR6_ENUM;

/*
    SENSOR7_ENUM
*/
typedef enum
{
    SENSOR7_ENUM_GPIO,                   //< GPIO
    SENSOR7_ENUM_8BITS,                  //< 8 bits sensor

    ENUM_DUMMY4WORD(SENSOR7_ENUM)
} SENSOR7_ENUM;

/*
    SENSOR8_ENUM
*/
typedef enum
{
    SENSOR8_ENUM_GPIO,                   //< GPIO
    SENSOR8_ENUM_8BITS,                  //< 8 bits sensor

    ENUM_DUMMY4WORD(SENSOR8_ENUM)
} SENSOR8_ENUM;

/*
    PXCLK_SEL_ENUM
*/
typedef enum
{
    PXCLK_SEL_ENUM_GPIO,                //< Select as GPIO
    PXCLK_SEL_ENUM_PXCLK,               //< Select as PXCLK

    ENUM_DUMMY4WORD(PXCLK_SEL_ENUM)
} PXCLK_SEL_ENUM;

/*
    VD_HD_SEL_ENUM
*/
typedef enum
{
    VD_HD_SEL_ENUM_GPIO,                //< Select as GPIO
    VD_HD_SEL_ENUM_SIE_VDHD,            //< Mux VD/HD to SIE
    VD_HD_SEL_ENUM_LVDS_VDHD,           //< Mux VD/HD to LVDS

    ENUM_DUMMY4WORD(VD_HD_SEL_ENUM)
} VD_HD_SEL_ENUM;

/*
    XVS_XHS_ENUM
*/
typedef enum {
	XVS_XHS_SEL_ENUM_GPIO,                //< Select as GPIO
	XVS_XHS_SEL_ENUM_XVS_XHS,             //< Select as XVS_XHS

	ENUM_DUMMY4WORD(XVS_XHS_SEL_ENUM)
} XVS_XHS_SEL_ENUM;

/*
    XVS_XHS_PINMUX
*/
typedef enum
{
    XVS_XHS_1ST_PINMUX,                 //< Select 1st pinmux pad
    XVS_XHS_2ND_PINMUX,                 //< Select 2nd pinmux pad

    ENUM_DUMMY4WORD(XVS_XHS_PINMUX)
} XVS_XHS_PINMUX;

/*
    SENMCLK_SEL_ENUM
*/
typedef enum
{
    SENMCLK_SEL_ENUM_GPIO,              //< Select as GPIO
    SENMCLK_SEL_ENUM_MCLK,              //< Select as MCLK

    ENUM_DUMMY4WORD(SENMCLK_SEL_ENUM)
} SENMCLK_SEL_ENUM;

/*
    MES_SEL_ENUM
*/
typedef enum
{
    MES_SEL_ENUM_GPIO,                  //< Select as GPIO
    MES_SEL_ENUM_1ST_PINMUX,            //< Select as MES at 1st pinmux pad
    MES_SEL_ENUM_2ND_PINMUX,            //< Select as MES at 2nd pinmux pad

    ENUM_DUMMY4WORD(MES_SEL_ENUM)
} MES_SEL_ENUM;

/*
    FLCTR_SEL_ENUM
*/
typedef enum
{
    FLCTR_SEL_ENUM_GPIO,                //< Select as GPIO
    FLCTR_SEL_ENUM_FLCTR,               //< Select as FLCTR

    ENUM_DUMMY4WORD(FLCTR_SEL_ENUM)
} FLCTR_SEL_ENUM;

/*
    SP_CLK_SEL_ENUM
*/
typedef enum
{
    SP_CLK_SEL_ENUM_GPIO,               //< Select as GPIO
    SP_CLK_SEL_ENUM_SPCLK,              //< Select as SP_CLK
    SP_CLK_SEL_ENUM_SP_2_CLK,           //< Select as SP_2_CLK
    SP_CLK_SEL_ENUM_SP_3_CLK,           //< Select as SP_3_CLK

    ENUM_DUMMY4WORD(SP_CLK_SEL_ENUM)
} SP_CLK_SEL_ENUM;

/*
    SP2_CLK_SEL_ENUM
*/
typedef enum
{
    SP2_CLK_SEL_ENUM_GPIO,              //< Select as GPIO
    SP2_CLK_SEL_ENUM_SP2CLK,            //< Select as SP2_CLK
    SP2_CLK_SEL_ENUM_SP2_2_CLK,         //< Select as SP2_2_CLK

    ENUM_DUMMY4WORD(SP2_CLK_SEL_ENUM)
} SP2_CLK_SEL_ENUM;

/*
    SHUTTER_ENUM
*/
typedef enum
{
    SHUTTER_ENUM_GPIO,                  //< Select as GPIO
    SHUTTER_ENUM_SHUTTER,               //< Select as SHUTTER

    ENUM_DUMMY4WORD(SHUTTER_ENUM)
} SHUTTER_ENUM;

/*
    STROBE_ENUM
*/
typedef enum
{
    STROBE_ENUM_GPIO,                   //< Select as GPIO
    STROBE_ENUM_STROBE,                 //< Select as STROBE

    ENUM_DUMMY4WORD(STROBE_ENUM)
} STROBE_ENUM;

/*
    MIPI_LVDS_CLK_ENUM
*/
typedef enum
{
    MIPI_LVDS_CLK_ENUM_GPI,             //< Select as GPI
    MIPI_LVDS_CLK_ENUM_CLK,             //< Select as Clock Lane

    ENUM_DUMMY4WORD(MIPI_LVDS_CLK_ENUM)
} MIPI_LVDS_CLK_ENUM;

/*
    MIPI_LVDS_DATA_ENUM
*/
typedef enum
{
    MIPI_LVDS_DATA_ENUM_GPI,            //< Select as GPI
    MIPI_LVDS_DATA_ENUM_DAT,            //< Select as Data Lane

    ENUM_DUMMY4WORD(MIPI_LVDS_DATA_ENUM)
} MIPI_LVDS_DATA_ENUM;

/*
    I2C_ENUM
*/
typedef enum
{
    I2C_ENUM_GPIO,                      //< Select as GPIO
    I2C_ENUM_I2C,                       //< Select as I2C

    ENUM_DUMMY4WORD(I2C_ENUM)
} I2C_ENUM;

/*
    I2C_2_ENUM
*/
typedef enum
{
    I2C_2_ENUM_GPIO,                    //< Select as GPIO
    I2C_2_ENUM_I2C,                     //< Select as I2C

    ENUM_DUMMY4WORD(I2C_2_ENUM)
} I2C_2_ENUM;

/*
    SPI_ENUM
*/
typedef enum
{
    SPI_DISABLE,                       //< Select as GPIO
    SPI_1ST_PINMUX,                    //< Select SPI on 1st pinmux pad
    SPI_2ND_PINMUX,                    //< Select SPI on 2nd pinmux pad

    ENUM_DUMMY4WORD(SPI_ENUM)
} SPI_ENUM;


/*
    SPI2_ENUM
*/
typedef enum
{
    SPI2_DISABLE,                       //< Select as GPIO
    SPI2_1ST_PINMUX,                    //< Select SPI2 on 1st pinmux pad
    SPI2_2ND_PINMUX,                    //< Select SPI2 on 2nd pinmux pad
    SPI2_3RD_PINMUX,                    //< Select SPI2 on 3rd pinmux pad

    ENUM_DUMMY4WORD(SPI2_ENUM)
} SPI2_ENUM;

/*
    SPI3_ENUM
*/
typedef enum
{
    SPI3_DISABLE,                       //< Select as GPIO
    SPI3_1ST_PINMUX,                    //< Select SPI3 on 1st pinmux pad
    SPI3_2ND_PINMUX,                    //< Select SPI3 on 2nd pinmux pad
    SPI3_3RD_PINMUX,                    //< Select SPI3 on 3rd pinmux pad

    ENUM_DUMMY4WORD(SPI3_ENUM)
} SPI3_ENUM;

/*
    SPI4_ENUM
*/
typedef enum
{
    SPI4_DISABLE,                       //< Select as GPIO
    SPI4_1ST_PINMUX,                    //< Select SPI4 on 1st pinmux pad
    SPI4_2ND_PINMUX,                    //< Select SPI4 on 2nd pinmux pad
    SPI4_3RD_PINMUX,                    //< Select SPI4 on 3rd pinmux pad

    ENUM_DUMMY4WORD(SPI4_ENUM)
} SPI4_ENUM;

/*
    SPI_RDY_ENUM
*/
typedef enum
{
    SPI_RDY_DISABLE,                   //< Select as GPIO
    SPI_RDY_1ST_PINMUX,                //< Select SPI on 1st pinmux pad
    SPI_RDY_2ND_PINMUX,                //< Select SPI on 2nd pinmux pad

    ENUM_DUMMY4WORD(SPI_RDY_ENUM)
} SPI_RDY_ENUM;

/*
    SPI5_ENUM
*/
typedef enum
{
    SPI5_DISABLE,                       //< Select as GPIO
    SPI5_1ST_PINMUX,                    //< Select SPI5 on 1st pinmux pad
    SPI5_2ND_PINMUX,                    //< Select SPI5 on 2nd pinmux pad

    ENUM_DUMMY4WORD(SPI5_ENUM)
} SPI5_ENUM;

/*
    SPI_DAT_ENUM
*/
typedef enum
{
    SPI_DAT_ENUM_1Bit,                  //< Select SPI data is 1 bit
    SPI_DAT_ENUM_2Bit,                  //< Select SPI data is 2 bits
    SPI_DAT_ENUM_4Bit,                  //< Select SPI data is 4 bits

    ENUM_DUMMY4WORD(SPI_DAT_ENUM)
} SPI_DAT_ENUM;

/*
    UART_ENUM
*/
typedef enum
{
    UART_ENUM_GPIO,                     //< Select as GPIO
    UART_ENUM_UART,                     //< Select as UART

    ENUM_DUMMY4WORD(UART_ENUM)
} UART_ENUM;

/*
    UART2_ENUM

    @note For UART2
*/
typedef enum
{
    UART2_ENUM_GPIO,                    //< Select as GPIO
    UART2_ENUM_1ST_PINMUX,              //< Select as UART2_1
    UART2_ENUM_2ND_PINMUX,              //< Select as UART2_2

    ENUM_DUMMY4WORD(UART2_ENUM)
} UART2_ENUM;

/*
    UART3_ENUM

    @note For UART3
*/
typedef enum
{
    UART3_ENUM_GPIO,                    //< Select as GPIO
    UART3_ENUM_1ST_PINMUX,              //< Select as UART3 1ST
    UART3_ENUM_2ND_PINMUX,              //< Select as UART3 2ND

    ENUM_DUMMY4WORD(UART3_ENUM)
} UART3_ENUM;


/*
    UART4_ENUM

    @note For UART4
*/
typedef enum
{
    UART4_ENUM_GPIO,                    //< Select as GPIO
    UART4_ENUM_1ST_PINMUX,              //< Select as UART4 1ST
    UART4_ENUM_2ND_PINMUX,              //< Select as UART4 2ND

    ENUM_DUMMY4WORD(UART4_ENUM)
} UART4_ENUM;


/*
    UART_CTSRTS_ENUM

    @note for UART2~4
*/
typedef enum
{
    UART_CTSRTS_GPIO,                   //< Select as UART without flow control
    UART_CTSRTS_PINMUX,                 //< Select as UART with flow control

    ENUM_DUMMY4WORD(UART_CTSRTS_ENUM)
} UART_CTSRTS_ENUM;

/*
    SIFCH_ENUM

    @note For SIF CH0, 1, 2~5
*/
typedef enum
{
    SIFCH_ENUM_GPIO,                    //< Select as GPIO
    SIFCH_ENUM_SIF,                     //< Select as SIF
    SIFCH_ENUM_SIF_2ND_PINMUX,          //< Select as SIF 2nd
    SIFCH_ENUM_SIF_3RD_PINMUX,          //< Select as SIF 3rd

    ENUM_DUMMY4WORD(SIFCH_ENUM)
} SIFCH_ENUM;

/*
    AUDIO_ENUM
*/
typedef enum
{
    AUDIO_ENUM_GPIO,                    //< Select as GPIO
    AUDIO_ENUM_I2S,                     //< Select as I2S (on UART2_TX/UART2_RX/UART2_RTS/UART2_CTS)
    AUDIO_ENUM_I2S_2ND_PINMUX,          //< Backward compatible

    ENUM_DUMMY4WORD(AUDIO_ENUM)
} AUDIO_ENUM;

/*
    AUDIO_MCLK_ENUM
*/
typedef enum
{
    AUDIO_MCLK_GPIO,                    //< Select as GPIO
    AUDIO_MCLK_MCLK,                    //< Select as audio MCLK
    AUDIO_MCLK_MCLK_2ND_PINMUX,         //< Backward compatible

    ENUM_DUMMY4WORD(AUDIO_MCLK_ENUM)
} AUDIO_MCLK_ENUM;

/*
    REMOTE_ENUM
*/
typedef enum
{
    REMOTE_ENUM_GPIO,                   //< Select as GPIO
    REMOTE_ENUM_REMOTE,                 //< Select as REMOTE

    ENUM_DUMMY4WORD(REMOTE_ENUM)
} REMOTE_ENUM;


/*
    EJTAG_SEL_ENUM
*/
typedef enum
{
    EJTAG_SEL_ENUM_EJTAG_CPU1,          //< Select as EJTAG only CPU1
    EJTAG_SEL_ENUM_EJTAG_DAISYCHAIN,    //< Select as EJTAG Daisy chain

    ENUM_DUMMY4WORD(EJTAG_SEL_ENUM)
} EJTAG_SEL_ENUM;

/*
    EJTAG_DSP_SEL_ENUM
*/
typedef enum
{
    EJTAG_DSP_SEL_ENUM_1ST_PINMUX,     //< Select as DSP EJTAG as 1'st pinmux
    EJTAG_DSP_SEL_ENUM_2ND_PINMUX,     //< Select as DSP EJTAG as 2'nd pinmux

    ENUM_DUMMY4WORD(EJTAG_DSP_SEL_ENUM)
} EJTAG_DSP_SEL_ENUM;


/*
    PWM_ENUM
*/
typedef enum
{
    PWM_ENUM_GPIO,                      //< Select as GPIO
    PWM_ENUM_PWM,                       //< Select as PWM

    ENUM_DUMMY4WORD(PWM_ENUM)
} PWM_ENUM;

/*
    PWM_SEL_ENUM
*/
typedef enum
{
    PWM_SEL_ENUM_1ST_PINMUX,            //< Select as PWMx_1
    PWM_SEL_ENUM_2ND_PINMUX,            //< Select as PWMx_2

    ENUM_DUMMY4WORD(PWM_SEL_ENUM)
} PWM_SEL_ENUM;

/*
    PICNT_ENUM
*/
typedef enum
{
    PICNT_ENUM_GPIO,                    //< Select as GPIO
    PICNT_ENUM_PICNT,                   //< Select as PI_CNT
    PICNT_ENUM_PICNT2,                  //< Select as PI_CNT2

    ENUM_DUMMY4WORD(PICNT_ENUM)
} PICNT_ENUM;

/*
    DAC_CLK_ENUM
*/
typedef enum
{
    DAC_CLK_ENUM_GPIO,                  //< Select as GPIO
    DAC_CLK_ENUM_DAC_CLK,               //< Select as DAC_CLK

    ENUM_DUMMY4WORD(DAC_CLK_ENUM)
} DAC_CLK_ENUM;

/*
    USB_VBUSI_ENUM
*/
typedef enum
{
    USB_VBUSI_ENUM_INACTIVE,            //< Inactive (VBUSI is low)
    USB_VBUSI_ENUM_ACTIVE,              //< Active (VBUSI is high)

    ENUM_DUMMY4WORD(USB_VBUSI_ENUM)
} USB_VBUSI_ENUM;

/*
    USB_ID_ENUM
*/
typedef enum
{
    USB_ID_ENUM_HOST,                   //< VBUS ID is host
    USB_ID_ENUM_DEVICE,                 //< VBUS ID is device

    ENUM_DUMMY4WORD(USB_ID_ENUM)
} USB_ID_ENUM;

/*
    ETH_ID_ENUM
*/
typedef enum
{
   ETH_ID_ENUM_GPIO,
   ETH_ID_ENUM_MII,
   ETH_ID_ENUM_RMII,
   ETH_ID_ENUM_GMII,
   ETH_ID_ENUM_RGMII,
   ETH_ID_ENUM_REVMII_10_100,
   ETH_ID_ENUM_REVMII_10_1000,
   ENUM_DUMMY4WORD(ETH_ID_ENUM)
} ETH_ID_ENUM;

/*
    LOCKN_ENUM
*/
typedef enum
{
    LOCKN_ENUM_GPIO,                   //< GPIO
    LOCKN_ENUM_LOCKN,                  //< LOCKN

    ENUM_DUMMY4WORD(LOCKN_ENUM)
} LOCKN_ENUM;

/*
    CANBUS_ENUM
*/
typedef enum
{
  CANBUS_ENUM_GPIO,                   //< GPIO
  CANBUS_ENUM_CANBUS,                 //< CANBUS

  ENUM_DUMMY4WORD(CANBUS_ENUM)
} CANBUS_ENUM;

/*
   TV_TEST_CLK_ENUM
*/
typedef enum
{
  TV_TEST_CLK_GPIO,                   //< GPIO
  TV_TEST_CLK_TV_CLK,                 //< TV TEST CLK

  ENUM_DUMMY4WORD(TV_TEST_CLK_ENUM)
} TV_TEST_CLK_ENUM;

/*
   TRACE
*/
typedef enum
{
  TRACE_ENUM_GPIO,                    //< GPIO
  TRACE_ENUM_TRACE,                   //< TRACE

  ENUM_DUMMY4WORD(TRACE_ENUM)
} TRACE_ENUM;

/*
    GPIO_ID_ENUM
*/
typedef enum
{
    GPIO_ID_EMUM_FUNC,                  //< pinmux is mapping to function
    GPIO_ID_EMUM_GPIO,                  //< pinmux is mapping to gpio

    ENUM_DUMMY4WORD(GPIO_ID_ENUM)
} GPIO_ID_ENUM;

/**
    NAND configuration identifier

    @note For nand_setConfig()
*/
typedef enum
{
    NAND_CONFIG_ID_FREQ,                    ///< NAND module clock (Unit: MHz), MUST config before storage object open, active after open
                                            ///< Context can be one of the following:
                                            ///< - @b 48    : 48 MHz (Default value)
                                            ///< - @b 60    : 60 MHz
                                            ///< - @b 96    : 96 MHz
    NAND_CONFIG_ID_TIMING0,                 ///< NAND controller timing0 register, MUST config before storage object open and need config timing1 at the same time
                                            ///< Context is UINT32 value
    NAND_CONFIG_ID_TIMING1,                 ///< NAND controller timing1 register, MUST config before storage object open and need config timing0 at the same time
                                            ///< Context is UINT32 value

    NAND_CONFIG_ID_DELAY_LATCH,             ///< Context can be one of the following:
                                            ///< - @b TRUE  : 0.5T delay latch
                                            ///< - @b FALSE : 0.0T delay latch
                                            ///<   @note: TRUE equal to 0.5 tRP

    NAND_CONFIG_ID_SET_INTEN,               ///< Context can be one of the following:
                                            ///< - @b TRUE  : Enable interrupt mode
                                            ///< - @b FALSE : Disable interrupt mode
                                            ///<   @note: Need config before any access of storage object hook on NAND device open\n
                                            ///<          such as STRG_SET_PARTITION_SIZE, STRG_SET_PARTITION_RSV_SIZE, \n
                                            ///<          STRG_GET_BEST_ACCESS_SIZE, STRG_GET_SECTOR_SIZE, STRG_GET_DEVICE_PHY_SIZE,\n
                                            ///<          STRG_GET_MEMORY_SIZE_REQ
                                            ///< @code
                                            ///     if(nand_setConfig(NAND_CONFIG_ID_SET_INTEN, TRUE) != E_OK)
                                            ///     {
                                            ///         ERROR;
                                            ///     }
                                            ///     //Enable Interrupt mode
                                            //      if(nand_setConfig(NAND_CONFIG_ID_SET_INTEN, FALSE) != E_OK)
                                            ///     {
                                            ///         ERROR;
                                            ///     }
                                            ///     //Disable Interrupt mode
                                            ///  }
                                            ///  @endcode
    NAND_CONFIG_ID_AUTOPINMUX,              ///< Context can be one of the following:
                                            ///< - @b TRUE  : disable pinmux when NAND driver close
                                            ///< - @b FALSE : not disable pinmux when NAND driver close

    NAND_CONFIG_ID_NAND_TYPE,               ///< Context can be one of the following:
                                            ///< - @b NANDCTRL_ONFI_NAND_TYPE   : ONFI NAND via NAND controller
                                            ///< - @b NANDCTRL_SPI_NAND_TYPE    :  SPI NAND via NAND controller
                                            ///< - @b NANDCTRL_SPI_NOR_TYPE     :  SPI NOR  via NAND controller
                                            ///< - @b SPICTRL_SPI_NAND_TYPE     :  SPI NAND via  SPI controller

    NAND_CONFIG_ID_SPI_ECC_TYPE,            ///< Context can be one of the following(only available when device is SPI NAND flash):
                                            ///< - @b NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC : SPI NAND use nand controller reedsolomon ecc
                                            ///< - @b NANDCTRL_SPIFLASH_USE_ONDIE_ECC       : SPI NAND use nand on die ecc(depend on each spi nand flash)

    NAND_CONFIG_ID_SPI_SEC_ECC_EN,          ///< Context can be one of the following(only available when device is SPI NAND flash):
                                            ///< - @b TRUE  : Secondary ECC enable(only available when NAND_CONFIG_ID_SPI_ECC_TYPE = NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC)
                                            ///< - @b FALSE : Secondary ECC disable

    NAND_CONFIG_ID_SPI_OPERATION_BIT_MODE,  ///< Context can be one of the following(only available when device is SPI NAND flash):
                                            ///< - @b NANDCTRL_SPIFLASH_USE_1_BIT : SPI NAND 1 bit operation mode
                                            ///< - @b NANDCTRL_SPIFLASH_USE_2_BIT : SPI NAND 2 bit operation mode(dual mode)
                                            ///< - @b NANDCTRL_SPIFLASH_USE_4_BIT : SPI NAND 4 bit operation mode(quad mode)

    ENUM_DUMMY4WORD(NAND_CONFIG_ID)
} NAND_CONFIG_ID;

typedef enum
{
    NANDCTRL_ONFI_NAND_TYPE = 0x0,
    NANDCTRL_SPI_NAND_TYPE,
    NANDCTRL_SPI_NOR_TYPE,
    SPICTRL_SPI_NAND_TYPE,

    NAND_TYPE_NUM,
    ENUM_DUMMY4WORD(NAND_TYPE_SEL)
} NAND_TYPE_SEL;

//
//0x00 TOP Control Register 0
//
#define TOP_REG0_OFS                        0x00
REGDEF_BEGIN(TOP_REG0)
    REGDEF_BIT(BOOT_SRC         ,4)     // Boot Source Selection
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(EJTAG_SEL        ,1)     // EJTAG select
    REGDEF_BIT(MPLL_CLKSELL     ,1)     // PLL1 clock output mux
    REGDEF_BIT(EMMC_WIDTH       ,1)     // eMMC bus width
    REGDEF_BIT(ETH_MODE_SEL     ,2)
    REGDEF_BIT(DEBUG_MODE_SEL   ,2)
    REGDEF_BIT(                 ,20)
REGDEF_END(TOP_REG0)

//
//0x04 TOP Control Register 1
//
#define TOP_REG1_OFS                        0x04
REGDEF_BEGIN(TOP_REG1)
    REGDEF_BIT(                 ,1)     // Active NAND bus
    REGDEF_BIT(                 ,1)     // Active SPI bus
    REGDEF_BIT(                 ,1)     // Active SDIO2 bus
    REGDEF_BIT(                 ,9)
    REGDEF_BIT(NAND_EXIST       ,1)     // NAND exist enable control
    REGDEF_BIT(FSPI_EXIST       ,1)     // FSPI exist enable control
    REGDEF_BIT(SDIO_EXIST       ,1)     // SDIO exist enable control
    REGDEF_BIT(SDIO2_EXIST      ,1)     // SDIO2 exist enable control
    REGDEF_BIT(EXTROM_EXIST     ,1)     // EXT ROM exist enable control
    REGDEF_BIT(SDIO3_EXIST      ,1)     // SDIO3 exist enable control
    REGDEF_BIT(                 ,6)
    REGDEF_BIT(SDIO3_MUX_SEL    ,1)     // SDIO3 pinmux pad select
    REGDEF_BIT(                 ,1)     // SPI pinmux pad select
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)     //
    REGDEF_BIT(                 ,1)     //
    REGDEF_BIT(NAND_CS_NUM      ,1)     // Select NAND CS Pin number
    REGDEF_BIT(                 ,1)     // Select SPI bus width
    REGDEF_BIT(SDIO3_BUS_WIDTH  ,1)     // Select SDIO3 bus width
REGDEF_END(TOP_REG1)

//
//0x08 TOP Control Register 2
//
#define TOP_REG2_OFS                        0x08
REGDEF_BEGIN(TOP_REG2)
    REGDEF_BIT(LCD_TYPE         ,4)     // Pinmux of LCD interface
    REGDEF_BIT(PLCD_DATA_WIDTH  ,2)     // Parallel LCD data width
    REGDEF_BIT(PLCD_DE          ,1)     // Pinmux of DE (for parallel LCD)
    REGDEF_BIT(CCIR_DATA_WIDTH  ,1)     // CCIR data width
    REGDEF_BIT(CCIR_HVLD_VVLD   ,1)     // CCIR VVLD, HVLD select
    REGDEF_BIT(CCIR_FIELD       ,1)     // CCIR FIELD select
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(LCD2_TYPE        ,4)     // Pinmux of LCD2 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(PLCD2_DE         ,1)     // Pinmux of LCD2 DE (for parallel LCD)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(MEMIF_TYPE       ,2)     // MI display type
    REGDEF_BIT(PMEM_SEL         ,1)     // Parallel MI pinmux pad select
    REGDEF_BIT(SMEMF_DATA_WIDTH ,1)     // Serial Memory Interface data width
    REGDEF_BIT(PMEMIF_DATA_WIDTH,2)     // Parallel Memory Interface data width
    REGDEF_BIT(TE_SEL           ,1)     // Memory Interface TE select
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)     // Pinmux of HDMI CEC
    REGDEF_BIT(                 ,1)     // Pinmux of HDMI HOTPLUG
    REGDEF_BIT(TV_TEST_CLK      ,1)     // TV test clock enable
    REGDEF_BIT(                 ,1)
REGDEF_END(TOP_REG2)

//
//0x0c TOP Control Register 3
//
#define TOP_REG3_OFS                        0x0C
REGDEF_BEGIN(TOP_REG3)
    REGDEF_BIT(SENSOR           ,3)     // Pinmux of sensor interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,3)     // Pinmux of sensor2 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SENSOR3          ,3)     // Pinmux of sensor3 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,3)     // Pinmux of sensor4 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,3)     // Pinmux of sensor5 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SENSOR6          ,3)     // Pinmux of sensor6 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SENSOR7          ,3)     // Pinmux of sensor7 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SENSOR8          ,3)     // Pinmux of sensor8 interface
    REGDEF_BIT(                 ,1)
REGDEF_END(TOP_REG3)

//
//0x10 TOP Control Register 4
//
#define TOP_REG4_OFS                        0x10
REGDEF_BEGIN(TOP_REG4)
    REGDEF_BIT(FLCTR            ,1)     // Pinmux of sensor FLCTR
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(STROBE_SEL       ,1)     // Pinmux of strobe
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SHUTTER_SEL      ,1)     // Pinmux of shutter
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(MES_SH0_SEL      ,2)     // MES pinmux pad select (MES_SEL)
    REGDEF_BIT(MES_SH1_SEL      ,2)     // MES2 pinmux pad select (MES2_SEL)
    REGDEF_BIT(MES2_SH0_SEL     ,2)     // MES_2 pinmux pad select (MES_2_SEL)
    REGDEF_BIT(MES2_SH1_SEL     ,2)     // MES_2_2 pinmux pad select(MES_2_2_SEL)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(XVS_XHS_EN       ,1)     // XVS/XHS enable
    REGDEF_BIT(XVS_XHS2_EN      ,1)     // XVS/XHS2 enable
    REGDEF_BIT(XVS_XHS3_EN      ,1)     // XVS/XHS3 enable
    REGDEF_BIT(XVS_XHS4_EN      ,1)     // XVS/XHS4 enable
    REGDEF_BIT(XVS_XHS5_EN      ,1)     // XVS/XHS5 enable
    REGDEF_BIT(XVS_XHS6_EN      ,1)     // XVS/XHS6 enable
    REGDEF_BIT(XVS_XHS7_EN      ,1)     // XVS/XHS7 enable
    REGDEF_BIT(XVS_XHS8_EN      ,1)     // XVS/XHS8 enable
    REGDEF_BIT(XVS_XHS3_SEL     ,1)     // XVS/XHS3 pinmux seleciton
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(LOCKN            ,1)     // Pinmux of LOCKN
    REGDEF_BIT(LOCKN2           ,1)     // Pinmux of LOCKN
    REGDEF_BIT(                 ,2)
REGDEF_END(TOP_REG4)

//
//0x14 TOP Control Register 5
//
#define TOP_REG5_OFS                        0x14
REGDEF_BEGIN(TOP_REG5)
    REGDEF_BIT(SPI              ,2)     // SPI  pinmux select
    REGDEF_BIT(SPI2             ,2)     // SPI2 pinmux select
    REGDEF_BIT(SPI3             ,2)     // SPI3 pinmux select
    REGDEF_BIT(SPI4             ,2)     // SPI4 pinmux select
    REGDEF_BIT(SPI_DAT          ,1)     // SPI  data select
    REGDEF_BIT(SPI2_DAT         ,1)     // SPI2 data select
    REGDEF_BIT(SPI3_DAT         ,1)     // SPI3 data select
    REGDEF_BIT(SPI4_DAT         ,1)     // SPI4 data select
    REGDEF_BIT(SPI3_RDY         ,2)     // SPI3_RDY seletion
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(SIFCH0           ,2)     // Pinmux of SIF channel 0
    REGDEF_BIT(SIFCH1           ,2)     // Pinmux of SIF channel 1
    REGDEF_BIT(SIFCH2           ,2)     // Pinmux of SIF channel 2
    REGDEF_BIT(SIFCH3           ,2)     // Pinmux of SIF channel 3
    REGDEF_BIT(SIFCH4           ,2)     // Pinmux of SIF channel 4
    REGDEF_BIT(SIFCH5           ,2)     // Pinmux of SIF channel 5
    REGDEF_BIT(SIFCH6           ,2)     // Pinmux of SIF channel 6
    REGDEF_BIT(SIFCH7           ,2)     // Pinmux of SIF channel 7
REGDEF_END(TOP_REG5)

//
//0x18 TOP Control Register 6
//
#define TOP_REG6_OFS                        0x18
REGDEF_BEGIN(TOP_REG6)
    REGDEF_BIT(AUDIO            ,2)     // Pinmux of external AUDIO interface
    REGDEF_BIT(AUDIO_MCLK       ,2)     // AUDIO MCLK pinmux pad select
    REGDEF_BIT(                 ,4)
    REGDEF_BIT(SP_CLK           ,2)     // Pinmux of Special Clock
    REGDEF_BIT(SP2_CLK          ,2)     // Pinmux of Special Clock 2
    REGDEF_BIT(REMOTE           ,1)     // Pinmux of REMOTE
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(ETH              ,3)     // Pinmux of ETH
    REGDEF_BIT(CANBUS           ,1)     // Pinmux of Canbus
    REGDEF_BIT(                 ,12)
REGDEF_END(TOP_REG6)

//
//0x1C TOP Control Register 7
//
#define TOP_REG7_OFS                        0x1C
REGDEF_BEGIN(TOP_REG7)
    REGDEF_BIT(PWM0             ,1)     // Pinmux of PWM0
    REGDEF_BIT(PWM1             ,1)     // Pinmux of PWM1
    REGDEF_BIT(PWM2             ,1)     // Pinmux of PWM2
    REGDEF_BIT(PWM3             ,1)     // Pinmux of PWM3
    REGDEF_BIT(PWM4             ,1)     // Pinmux of PWM4
    REGDEF_BIT(PWM5             ,1)     // Pinmux of PWM5
    REGDEF_BIT(PWM6             ,1)     // Pinmux of PWM6
    REGDEF_BIT(PWM7             ,1)     // Pinmux of PWM7
    REGDEF_BIT(PWM8             ,1)     // Pinmux of PWM8
    REGDEF_BIT(PWM9             ,1)     // Pinmux of PWM9
    REGDEF_BIT(PWM10            ,1)     // Pinmux of PWM10
    REGDEF_BIT(PWM11            ,1)     // Pinmux of PWM11
    REGDEF_BIT(PWM12            ,1)     // Pinmux of PWM12
    REGDEF_BIT(PWM13            ,1)     // Pinmux of PWM13
    REGDEF_BIT(PWM14            ,1)     // Pinmux of PWM14
    REGDEF_BIT(PWM15            ,1)     // Pinmux of PWM15
    REGDEF_BIT(PWM16            ,1)     // Pinmux of PWM16
    REGDEF_BIT(PWM17            ,1)     // Pinmux of PWM17
    REGDEF_BIT(PWM18            ,1)     // Pinmux of PWM18
    REGDEF_BIT(PWM19            ,1)     // Pinmux of PWM19
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(PWM0_SEL         ,1)     // Pinmux of PWM0 pinmux
    REGDEF_BIT(PI_CNT           ,2)     // Pinmux of PI_CNT
    REGDEF_BIT(PI_CNT2          ,2)     // Pinmux of PI_CNT2
    REGDEF_BIT(PI_CNT3          ,2)     // Pinmux of PI_CNT3
    REGDEF_BIT(PI_CNT4          ,2)     // Pinmux of PI_CNT4
REGDEF_END(TOP_REG7)

//
//0x20 TOP Control Register 8
//
#define TOP_REG8_OFS                        0x20
REGDEF_BEGIN(TOP_REG8)
    REGDEF_BIT(USB_VBUSI        ,1)     // VBUSI control
    REGDEF_BIT(USB_ID           ,1)     // USB ID control
    REGDEF_BIT(USB2_VBUSI       ,1)     // USB2 VBUSI control
    REGDEF_BIT(USB2_ID          ,1)     // USB2 ID control
    REGDEF_BIT(                 ,28)
REGDEF_END(TOP_REG8)

//
//0x24 TOP Control Register 9
//
#define TOP_REG9_OFS                        0x24
REGDEF_BEGIN(TOP_REG9)
    REGDEF_BIT(UART             ,1)     // Pinmux of UART
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(UART2            ,2)     // Pinmux of UART2
    REGDEF_BIT(UART3            ,2)     // Pinmux of UART3
    REGDEF_BIT(UART4            ,2)     // Pinmux of UART4
    REGDEF_BIT(UART2_CTSRTS     ,1)     // Pinmux of UART2 CTS/RTS
    REGDEF_BIT(UART3_CTSRTS     ,1)     // Pinmux of UART3 CTS/RTS
    REGDEF_BIT(UART4_CTSRTS     ,1)     // Pinmux of UART4 CTS/RTS
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(                 ,18)
REGDEF_END(TOP_REG9)

//
//0x28 TOP Control Register 10
//
#define TOP_REG10_OFS                        0x28
REGDEF_BEGIN(TOP_REG10)
    REGDEF_BIT(I2C              ,1)     // Pinmux of I2C interface
    REGDEF_BIT(I2C_2            ,1)     // Pinmux of I2C_2 interface
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(I2C2             ,1)     // Pinmux of I2C2 interface
    REGDEF_BIT(I2C2_2           ,1)     // Pinmux of I2C2_2 interface
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(I2C3             ,1)     // Pinmux of I2C3 interface
    REGDEF_BIT(I2C3_2           ,1)     // Pinmux of I2C3_2 interface
    REGDEF_BIT(I2C3_3           ,1)     // Pinmux of I2C3_3 interface
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(I2C4             ,1)     // Pinmux of I2C4 interface
    REGDEF_BIT(I2C4_2           ,1)     // Pinmux of I2C4_2 interface
    REGDEF_BIT(I2C4_3           ,1)     // Pinmux of I2C4_3 interface
    REGDEF_BIT(I2C4_4           ,1)     // Pinmux of I2C4_4 interface
    REGDEF_BIT(I2C5             ,1)     // Pinmux of I2C5 interface
    REGDEF_BIT(I2C5_2           ,1)     // Pinmux of I2C5_2 interface
    REGDEF_BIT(                 ,14)
REGDEF_END(TOP_REG10)


//
//0x2C TOP Control Register 11
//
#define TOP_REG11_OFS                        0x2C
REGDEF_BEGIN(TOP_REG11)
    REGDEF_BIT(CPU_EJTAG_EN     ,1)     // Pinmux of EJTAG
    REGDEF_BIT(DSP_EJTAG_EN     ,1)     // Pinmux of DSP EJTAG
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(DSP_EJTAG_SEL    ,1)     // Pinmux of DSP EJTAG select
    REGDEF_BIT(DSP_EJTAG_RTCKSEL,1)     // Pinmux of DSP EJTAG RTCK select
    REGDEF_BIT(DSP_EJTAG_DC     ,2)     // Daisy-chain selection of DSP EJTAG
    REGDEF_BIT(TRACE_EN         ,2)
    REGDEF_BIT(                 ,6)
    REGDEF_BIT(DSP_PERI_EN      ,1)
    REGDEF_BIT(DSP_IOP_EN       ,1)
    REGDEF_BIT(DSP2_PERI_EN     ,1)
    REGDEF_BIT(DSP2_IOP_EN      ,1)
    REGDEF_BIT(                 ,12)
REGDEF_END(TOP_REG11)

//
//0x30 TOP Control Register 12
//
#define TOP_REG12_OFS                        0x30
REGDEF_BEGIN(TOP_REG12)
    REGDEF_BIT(RAM_SRC_SELECT   ,1)
    REGDEF_BIT(ROM_SRC_SELECT   ,1)
    REGDEF_BIT(                 ,30)
REGDEF_END(TOP_REG12)


//
// 0xA0 CGPIO Control Register
//
#define TOP_REGCGPIO0_OFS                    0xA0
REGDEF_BEGIN(TOP_REGCGPIO0)
    REGDEF_BIT(CGPIO_0          ,1)     // pinmux of CGPIO 0
    REGDEF_BIT(CGPIO_1          ,1)     // pinmux of CGPIO 1
    REGDEF_BIT(CGPIO_2          ,1)     // pinmux of CGPIO 2
    REGDEF_BIT(CGPIO_3          ,1)     // pinmux of CGPIO 3
    REGDEF_BIT(CGPIO_4          ,1)     // pinmux of CGPIO 4
    REGDEF_BIT(CGPIO_5          ,1)     // pinmux of CGPIO 5
    REGDEF_BIT(CGPIO_6          ,1)     // pinmux of CGPIO 6
    REGDEF_BIT(CGPIO_7          ,1)     // pinmux of CGPIO 7
    REGDEF_BIT(CGPIO_8          ,1)     // pinmux of CGPIO 8
    REGDEF_BIT(CGPIO_9          ,1)     // pinmux of CGPIO 9
    REGDEF_BIT(CGPIO_10         ,1)     // pinmux of CGPIO 10
    REGDEF_BIT(CGPIO_11         ,1)     // pinmux of CGPIO 11
    REGDEF_BIT(CGPIO_12         ,1)     // pinmux of CGPIO 12
    REGDEF_BIT(CGPIO_13         ,1)     // pinmux of CGPIO 13
    REGDEF_BIT(CGPIO_14         ,1)     // pinmux of CGPIO 14
    REGDEF_BIT(CGPIO_15         ,1)     // pinmux of CGPIO 15
    REGDEF_BIT(CGPIO_16         ,1)     // pinmux of CGPIO 16
    REGDEF_BIT(CGPIO_17         ,1)     // pinmux of CGPIO 17
    REGDEF_BIT(CGPIO_18         ,1)     // pinmux of CGPIO 18
    REGDEF_BIT(CGPIO_19         ,1)     // pinmux of CGPIO 19
    REGDEF_BIT(CGPIO_20         ,1)     // pinmux of CGPIO 20
    REGDEF_BIT(CGPIO_21         ,1)     // pinmux of CGPIO 21
    REGDEF_BIT(CGPIO_22         ,1)     // pinmux of CGPIO 22
    REGDEF_BIT(CGPIO_23         ,1)     // pinmux of CGPIO 23
    REGDEF_BIT(CGPIO_24         ,1)     // pinmux of CGPIO 24
    REGDEF_BIT(CGPIO_25         ,1)     // pinmux of CGPIO 25
    REGDEF_BIT(CGPIO_26         ,1)     // pinmux of CGPIO 26
    REGDEF_BIT(CGPIO_27         ,1)     // pinmux of CGPIO 27
    REGDEF_BIT(CGPIO_28         ,1)     // pinmux of CGPIO 28
    REGDEF_BIT(CGPIO_29         ,1)     // pinmux of CGPIO 29
    REGDEF_BIT(CGPIO_30         ,1)     // pinmux of CGPIO 30
    REGDEF_BIT(CGPIO_31         ,1)     // pinmux of CGPIO 31
REGDEF_END(TOP_REGCGPIO0)

//
// 0xA4 CGPIO Control Register 1
//
#define TOP_REGCGPIO1_OFS                    0xA4
REGDEF_BEGIN(TOP_REGCGPIO1)
    REGDEF_BIT(CGPIO_32         ,1)     // pinmux of CGPIO 32
    REGDEF_BIT(CGPIO_33         ,1)     // pinmux of CGPIO 33
    REGDEF_BIT(                 ,30)
REGDEF_END(TOP_REGCGPIO1)

//
// 0xA8 PGPIO Control Register
//
#define TOP_REGPGPIO0_OFS                    0xA8
REGDEF_BEGIN(TOP_REGPGPIO0)
    REGDEF_BIT(PGPIO_0          ,1)     // pinmux of PGPIO 0
    REGDEF_BIT(PGPIO_1          ,1)     // pinmux of PGPIO 1
    REGDEF_BIT(PGPIO_2          ,1)     // pinmux of PGPIO 2
    REGDEF_BIT(PGPIO_3          ,1)     // pinmux of PGPIO 3
    REGDEF_BIT(PGPIO_4          ,1)     // pinmux of PGPIO 4
    REGDEF_BIT(PGPIO_5          ,1)     // pinmux of PGPIO 5
    REGDEF_BIT(PGPIO_6          ,1)     // pinmux of PGPIO 6
    REGDEF_BIT(PGPIO_7          ,1)     // pinmux of PGPIO 7
    REGDEF_BIT(PGPIO_8          ,1)     // pinmux of PGPIO 8
    REGDEF_BIT(PGPIO_9          ,1)     // pinmux of PGPIO 9
    REGDEF_BIT(PGPIO_10         ,1)     // pinmux of PGPIO 10
    REGDEF_BIT(PGPIO_11         ,1)     // pinmux of PGPIO 11
    REGDEF_BIT(PGPIO_12         ,1)     // pinmux of PGPIO 12
    REGDEF_BIT(PGPIO_13         ,1)     // pinmux of PGPIO 13
    REGDEF_BIT(PGPIO_14         ,1)     // pinmux of PGPIO 14
    REGDEF_BIT(PGPIO_15         ,1)     // pinmux of PGPIO 15
    REGDEF_BIT(PGPIO_16         ,1)     // pinmux of PGPIO 16
    REGDEF_BIT(PGPIO_17         ,1)     // pinmux of PGPIO 17
    REGDEF_BIT(PGPIO_18         ,1)     // pinmux of PGPIO 18
    REGDEF_BIT(PGPIO_19         ,1)     // pinmux of PGPIO 19
    REGDEF_BIT(PGPIO_20         ,1)     // pinmux of PGPIO 20
    REGDEF_BIT(PGPIO_21         ,1)     // pinmux of PGPIO 21
    REGDEF_BIT(PGPIO_22         ,1)     // pinmux of PGPIO 22
    REGDEF_BIT(PGPIO_23         ,1)     // pinmux of PGPIO 23
    REGDEF_BIT(PGPIO_24         ,1)     // pinmux of PGPIO 24
    REGDEF_BIT(PGPIO_25         ,1)     // pinmux of PGPIO 25
    REGDEF_BIT(PGPIO_26         ,1)     // pinmux of PGPIO 26
    REGDEF_BIT(PGPIO_27         ,1)     // pinmux of PGPIO 27
    REGDEF_BIT(PGPIO_28         ,1)     // pinmux of PGPIO 28
    REGDEF_BIT(PGPIO_29         ,1)     // pinmux of PGPIO 29
    REGDEF_BIT(PGPIO_30         ,1)     // pinmux of PGPIO 30
    REGDEF_BIT(PGPIO_31         ,1)     // pinmux of PGPIO 31
REGDEF_END(TOP_REGPGPIO0)

//
// 0xAC PGPIO Control Register
//
#define TOP_REGPGPIO1_OFS                    0xAC
REGDEF_BEGIN(TOP_REGPGPIO1)
    REGDEF_BIT(PGPIO_32         ,1)     // pinmux of PGPIO 32
    REGDEF_BIT(PGPIO_33         ,1)     // pinmux of PGPIO 33
    REGDEF_BIT(PGPIO_34         ,1)     // pinmux of PGPIO 34
    REGDEF_BIT(PGPIO_35         ,1)     // pinmux of PGPIO 35
    REGDEF_BIT(PGPIO_36         ,1)     // pinmux of PGPIO 36
    REGDEF_BIT(PGPIO_37         ,1)     // pinmux of PGPIO 37
    REGDEF_BIT(PGPIO_38         ,1)     // pinmux of PGPIO 38
    REGDEF_BIT(PGPIO_39         ,1)     // pinmux of PGPIO 39
    REGDEF_BIT(PGPIO_40         ,1)     // pinmux of PGPIO 40
    REGDEF_BIT(PGPIO_41         ,1)     // pinmux of PGPIO 41
    REGDEF_BIT(PGPIO_42         ,1)     // pinmux of PGPIO 42
    REGDEF_BIT(PGPIO_43         ,1)     // pinmux of PGPIO 43
    REGDEF_BIT(PGPIO_44         ,1)     // pinmux of PGPIO 44
    REGDEF_BIT(PGPIO_45         ,1)     // pinmux of PGPIO 45
    REGDEF_BIT(PGPIO_46         ,1)     // pinmux of PGPIO 46
    REGDEF_BIT(PGPIO_47         ,1)     // pinmux of PGPIO 47
    REGDEF_BIT(                 ,16)
REGDEF_END(TOP_REGPGPIO1)

//
// 0xB0 SGPIO Control Register
//
#define TOP_REGSGPIO0_OFS                    0xB0
REGDEF_BEGIN(TOP_REGSGPIO0)
    REGDEF_BIT(SGPIO_0          ,1)     // pinmux of SGPIO 0
    REGDEF_BIT(SGPIO_1          ,1)     // pinmux of SGPIO 1
    REGDEF_BIT(SGPIO_2          ,1)     // pinmux of SGPIO 2
    REGDEF_BIT(SGPIO_3          ,1)     // pinmux of SGPIO 3
    REGDEF_BIT(SGPIO_4          ,1)     // pinmux of SGPIO 4
    REGDEF_BIT(SGPIO_5          ,1)     // pinmux of SGPIO 5
    REGDEF_BIT(SGPIO_6          ,1)     // pinmux of SGPIO 6
    REGDEF_BIT(SGPIO_7          ,1)     // pinmux of SGPIO 7
    REGDEF_BIT(SGPIO_8          ,1)     // pinmux of SGPIO 8
    REGDEF_BIT(SGPIO_9          ,1)     // pinmux of SGPIO 9
    REGDEF_BIT(SGPIO_10         ,1)     // pinmux of SGPIO 10
    REGDEF_BIT(SGPIO_11         ,1)     // pinmux of SGPIO 11
    REGDEF_BIT(                 ,20)
REGDEF_END(TOP_REGSGPIO0)


//
// 0xB8 LGPIO Control Register
//
#define TOP_REGLGPIO0_OFS                    0xB8
REGDEF_BEGIN(TOP_REGLGPIO0)
    REGDEF_BIT(LGPIO_0          ,1)     // pinmux of LGPIO 0
    REGDEF_BIT(LGPIO_1          ,1)     // pinmux of LGPIO 1
    REGDEF_BIT(LGPIO_2          ,1)     // pinmux of LGPIO 2
    REGDEF_BIT(LGPIO_3          ,1)     // pinmux of LGPIO 3
    REGDEF_BIT(LGPIO_4          ,1)     // pinmux of LGPIO 4
    REGDEF_BIT(LGPIO_5          ,1)     // pinmux of LGPIO 5
    REGDEF_BIT(LGPIO_6          ,1)     // pinmux of LGPIO 6
    REGDEF_BIT(LGPIO_7          ,1)     // pinmux of LGPIO 7
    REGDEF_BIT(LGPIO_8          ,1)     // pinmux of LGPIO 8
    REGDEF_BIT(LGPIO_9          ,1)     // pinmux of LGPIO 9
    REGDEF_BIT(LGPIO_10         ,1)     // pinmux of LGPIO 10
    REGDEF_BIT(LGPIO_11         ,1)     // pinmux of LGPIO 11
    REGDEF_BIT(LGPIO_12         ,1)     // pinmux of LGPIO 12
    REGDEF_BIT(LGPIO_13         ,1)     // pinmux of LGPIO 13
    REGDEF_BIT(LGPIO_14         ,1)     // pinmux of LGPIO 14
    REGDEF_BIT(LGPIO_15         ,1)     // pinmux of LGPIO 15
    REGDEF_BIT(LGPIO_16         ,1)     // pinmux of LGPIO 16
    REGDEF_BIT(LGPIO_17         ,1)     // pinmux of LGPIO 17
    REGDEF_BIT(LGPIO_18         ,1)     // pinmux of LGPIO 18
    REGDEF_BIT(LGPIO_19         ,1)     // pinmux of LGPIO 19
    REGDEF_BIT(LGPIO_20         ,1)     // pinmux of LGPIO 20
    REGDEF_BIT(LGPIO_21         ,1)     // pinmux of LGPIO 21
    REGDEF_BIT(LGPIO_22         ,1)     // pinmux of LGPIO 22
    REGDEF_BIT(LGPIO_23         ,1)     // pinmux of LGPIO 23
    REGDEF_BIT(LGPIO_24         ,1)     // pinmux of LGPIO 24
    REGDEF_BIT(LGPIO_25         ,1)     // pinmux of LGPIO 25
    REGDEF_BIT(LGPIO_26         ,1)     // pinmux of LGPIO 26
    REGDEF_BIT(LGPIO_27         ,1)     // pinmux of LGPIO 27
    REGDEF_BIT(LGPIO_28         ,1)     // pinmux of LGPIO 28
    REGDEF_BIT(LGPIO_29         ,1)     // pinmux of LGPIO 29
    REGDEF_BIT(LGPIO_30         ,1)     // pinmux of LGPIO 30
    REGDEF_BIT(LGPIO_31         ,1)     // pinmux of LGPIO 31
REGDEF_END(TOP_REGLGPIO0)

//
// 0xBC LGPIO Control Register
//
#define TOP_REGLGPIO1_OFS                    0xBC
REGDEF_BEGIN(TOP_REGLGPIO1)
    REGDEF_BIT(LGPIO_32         ,1)     // pinmux of LGPIO 32
    REGDEF_BIT(                 ,31)
REGDEF_END(TOP_REGLGPIO1)

//
// 0xD0 DGPIO Control Register
//
#define TOP_REGDGPIO0_OFS                    0xD0
REGDEF_BEGIN(TOP_REGDGPIO0)
    REGDEF_BIT(DGPIO_0          ,1)     // pinmux of DGPIO 0
    REGDEF_BIT(DGPIO_1          ,1)     // pinmux of DGPIO 1
    REGDEF_BIT(DGPIO_2          ,1)     // pinmux of DGPIO 2
    REGDEF_BIT(DGPIO_3          ,1)     // pinmux of DGPIO 3
    REGDEF_BIT(DGPIO_4          ,1)     // pinmux of DGPIO 4
    REGDEF_BIT(DGPIO_5          ,1)     // pinmux of DGPIO 5
    REGDEF_BIT(DGPIO_6          ,1)     // pinmux of DGPIO 6
    REGDEF_BIT(DGPIO_7          ,1)     // pinmux of DGPIO 7
    REGDEF_BIT(DGPIO_8          ,1)     // pinmux of DGPIO 8
    REGDEF_BIT(DGPIO_9          ,1)     // pinmux of DGPIO 9
    REGDEF_BIT(DGPIO_10         ,1)     // pinmux of DGPIO 10
    REGDEF_BIT(DGPIO_11         ,1)     // pinmux of DGPIO 11
    REGDEF_BIT(DGPIO_12         ,1)     // pinmux of DGPIO 11
    REGDEF_BIT(                 ,19)
REGDEF_END(TOP_REGDGPIO0)


//
// 0xD8 HSIGPI Control Register
//
#define TOP_REGHGPI0_OFS                    0xD8
REGDEF_BEGIN(TOP_REGHGPI0)
    REGDEF_BIT(HSIGPI_0         ,1)     // pinmux of HSIGPI 0
    REGDEF_BIT(HSIGPI_1         ,1)     // pinmux of HSIGPI 1
    REGDEF_BIT(HSIGPI_2         ,1)     // pinmux of HSIGPI 2
    REGDEF_BIT(HSIGPI_3         ,1)     // pinmux of HSIGPI 3
    REGDEF_BIT(HSIGPI_4         ,1)     // pinmux of HSIGPI 4
    REGDEF_BIT(HSIGPI_5         ,1)     // pinmux of HSIGPI 5
    REGDEF_BIT(HSIGPI_6         ,1)     // pinmux of HSIGPI 6
    REGDEF_BIT(HSIGPI_7         ,1)     // pinmux of HSIGPI 7
    REGDEF_BIT(HSIGPI_8         ,1)     // pinmux of HSIGPI 8
    REGDEF_BIT(HSIGPI_9         ,1)     // pinmux of HSIGPI 9
    REGDEF_BIT(HSIGPI_10        ,1)     // pinmux of HSIGPI 10
    REGDEF_BIT(HSIGPI_11        ,1)     // pinmux of HSIGPI 11
    REGDEF_BIT(HSIGPI_12        ,1)     // pinmux of HSIGPI 12
    REGDEF_BIT(HSIGPI_13        ,1)     // pinmux of HSIGPI 13
    REGDEF_BIT(HSIGPI_14        ,1)     // pinmux of HSIGPI 14
    REGDEF_BIT(HSIGPI_15        ,1)     // pinmux of HSIGPI 15
    REGDEF_BIT(HSIGPI_16        ,1)     // pinmux of HSIGPI 16
    REGDEF_BIT(HSIGPI_17        ,1)     // pinmux of HSIGPI 17
    REGDEF_BIT(HSIGPI_18        ,1)     // pinmux of HSIGPI 18
    REGDEF_BIT(HSIGPI_19        ,1)     // pinmux of HSIGPI 19
    REGDEF_BIT(HSIGPI_20        ,1)     // pinmux of HSIGPI 20
    REGDEF_BIT(HSIGPI_21        ,1)     // pinmux of HSIGPI 21
    REGDEF_BIT(HSIGPI_22        ,1)     // pinmux of HSIGPI 22
    REGDEF_BIT(HSIGPI_23        ,1)     // pinmux of HSIGPI 23
    REGDEF_BIT(HSIGPI_24        ,1)     // pinmux of HSIGPI 24
    REGDEF_BIT(HSIGPI_25        ,1)     // pinmux of HSIGPI 25
    REGDEF_BIT(HSIGPI_26        ,1)     // pinmux of HSIGPI 26
    REGDEF_BIT(HSIGPI_27        ,1)     // pinmux of HSIGPI 27
    REGDEF_BIT(HSIGPI_28        ,1)     // pinmux of HSIGPI 28
    REGDEF_BIT(HSIGPI_29        ,1)     // pinmux of HSIGPI 29
    REGDEF_BIT(HSIGPI_30        ,1)     // pinmux of HSIGPI 30
    REGDEF_BIT(HSIGPI_31        ,1)     // pinmux of HSIGPI 31
REGDEF_END(TOP_REGHGPI0)

//
//0xF0 Version Code Register
//
#define TOP_VERSION_REG_OFS                 0xF0
REGDEF_BEGIN(TOP_VERSION_REG)
    REGDEF_BIT(                 ,8)
    REGDEF_BIT(ROM_CODE_VERSION ,4)  // ROM Code Version
    REGDEF_BIT(ECO_VERSION      ,4)  // ECO Version
    REGDEF_BIT(CHIP_ID          ,16) // Chip ID
REGDEF_END(TOP_VERSION_REG)

//
//0xF8 OCP bus debug port sel
//
#define TOP_REG_REG_OCP_DEBUG_OFS           0xF8
REGDEF_BEGIN(TOP_REG_REG_OCP_DEBUG)
    REGDEF_BIT(OCP_DEBUG_SEL    ,4)  // ROM Code Version
    REGDEF_BIT(                 ,28)
REGDEF_END(TOP_REG_REG_OCP_DEBUG)

//
//0xFC Debug Port Register
//
#define TOP_REG_DEBUG_OFS                   0xFC
REGDEF_BEGIN(TOP_REG_DEBUG)
    REGDEF_BIT(DEBUG_SEL        ,7)  // Debug Port Selection
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(DEBUG_OUTSEL     ,2)  // Debug Port output select
    REGDEF_BIT(                 ,22)
REGDEF_END(TOP_REG_DEBUG)

#endif
