/**
    Header file for DMA module.

    This file is the header file that define the API for DMA module.

    @file       DMA.h
    @ingroup    mIDrvDMA_DMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _DMA_H
#define _DMA_H

//#include "Type.h"

/**
    @addtogroup mIDrvDMA_DMA
*/

/*
    @name DMA channel encoding

    NT96680 DMA channel setting

    @note Used in dma_setChannelPriority(), dma_getChannelPriority(),
                  dma_enableChannel(), dma_disableChannel()
*/
typedef enum
{
    DMA_CH_RSV = 0x00,
    DMA_CH_FIRST_CHANNEL,
    DMA_CH_CPU = DMA_CH_FIRST_CHANNEL,
    DMA_CH_CPU2,
    DMA_CH_DSP_0,
    DMA_CH_DSP_1,
    DMA_CH_DSP2_0,
    DMA_CH_DSP2_1,
    DMA_CH_USB30,
    DMA_CH_Ethernet,        // Ethernet
    DMA_CH_IDE_a0,          // IDE V1 Y
    DMA_CH_IDE_b0,          // IDE V1 C
    DMA_CH_IDE_a1,          // IDE V2 Y
    DMA_CH_IDE_b1,          // IDE V2 C
    DMA_CH_IDE_6,           // IDE O1 PAL/A
    DMA_CH_IDE_7,           // IDE O1 RGB
    DMA_CH_IDE2_a0,         // IDE2 V Y
    // Ctrl 0

    DMA_CH_IDE2_b0,         // IDE2 V C
    DMA_CH_IDE2_6,          // IDE2 O1 PAL/A
    DMA_CH_IDE2_7,          // IDE2 O2 PAL
    DMA_CH_TSMUX,           // TS Mux
    DMA_CH_CRYPTO,          // Crypto
    DMA_CH_LARB,            // Local Arbit for SIF/BMC/I2C/UART
    DMA_CH_NAND,            // NAND
    DMA_CH_SDIO,            // SDIO
    DMA_CH_SDIO2,           // SDIO2
    DMA_CH_SDIO3,           // SDIO3
    DMA_CH_SPI,             // SPI
    DMA_CH_DAI,             // DAI
    DMA_CH_USB,
    DMA_CH_HWCOPY_0,
    DMA_CH_HWCOPY_1,
    DMA_CH_ROTATE_0,
    // Ctrl 1

    DMA_CH_ROTATE_1,
    DMA_CH_TSDEMUX_0,
    DMA_CH_TSDEMUX_1,
    DMA_CH_ISE2_0,          // ISE2 input
    DMA_CH_ISE2_1,          // ISE2 output
    DMA_CH_DIS0,            // DIS input/output
    DMA_CH_DIS1,            // DIS input
    DMA_CH_FSD0,            // FDE input image
    DMA_CH_FSD1,            // FDE output image
    DMA_CH_FSD2,            // FDE output result
    DMA_CH_ISE_a0,          // ISE input
    DMA_CH_ISE_a1,          // ISE output
    DMA_CH_SIE_0,           // SIE output
    DMA_CH_SIE_1,           // SIE output
    DMA_CH_SIE_2,           // SIE output
    DMA_CH_SIE_3,           // SIE output
    // Ctrl 2

    DMA_CH_SIE_4,           // SIE output
    DMA_CH_SIE_5,           // SIE output
    DMA_CH_SIE_6,           // SIE input
    DMA_CH_SIE_7,           // SIE input
    DMA_CH_SIE_8,           // SIE input
    DMA_CH_SIE2_0,          // SIE2 output
    DMA_CH_SIE2_1,          // SIE2 output
    DMA_CH_SIE2_2,          // SIE2 output
    DMA_CH_SIE2_3,          // SIE2 output
    DMA_CH_SIE2_4,          // SIE2 output
    DMA_CH_SIE2_5,          // SIE2 input
    DMA_CH_SIE2_6,          // SIE2 input
    DMA_CH_SIE3_0,          // SIE3 output
    DMA_CH_SIE3_1,          // SIE3 output
    DMA_CH_SIE3_2,          // SIE3 output
    DMA_CH_SIE3_3,          // SIE3 output
    // Ctrl 3

    DMA_CH_SIE3_4,          // SIE3 output
    DMA_CH_SIE3_5,          // SIE3 input
    DMA_CH_SIE3_6,          // SIE3 input
    DMA_CH_SIE4_0,          // SIE4 output
    DMA_CH_SIE4_1,          // SIE4 output
    DMA_CH_SIE4_2,          // SIE4 output
    DMA_CH_SIE4_3,          // SIE4 output
    DMA_CH_SIE4_4,          // SIE4 output
    DMA_CH_SIE4_5,          // SIE4 input
    DMA_CH_SIE4_6,          // SIE4 input
    DMA_CH_SIE5_0,          // SIE5 output
    DMA_CH_SIE5_1,          // SIE5 output
    DMA_CH_SIE5_2,          // SIE5 input
    DMA_CH_SIE6_0,          // SIE6 output
    DMA_CH_SIE6_1,          // SIE6 output
    DMA_CH_SIE6_2,          // SIE6 input
    // Ctrl 4

    DMA_CH_SIE7_0,          // SIE7 output
    DMA_CH_SIE7_1,          // SIE7 output
    DMA_CH_SIE7_2,          // SIE7 input
    DMA_CH_SIE8_0,          // SIE8 output
    DMA_CH_SIE8_1,          // SIE8 output
    DMA_CH_SIE8_2,          // SIE8 input
    DMA_CH_GRA_0,           // Graphic Input 0
    DMA_CH_GRA_1,           // Graphic Input 1
    DMA_CH_GRA_2,           // Graphic Input 2
    DMA_CH_GRA_3,           // Graphic output
    DMA_CH_GRA_4,           // Graphic output
    DMA_CH_GRA2_0,          // Graphic2 input
    DMA_CH_GRA2_1,          // Graphic2 input
    DMA_CH_GRA2_2,          // Graphic2 output
    DMA_CH_Affine_0,        // Affine input
    DMA_CH_Affine_1,        // Affine output
    // Ctrl 5

    DMA_CH_JPG0,            // JPG IMG
    DMA_CH_JPG1,            // JPG BS
    DMA_CH_JPG2,            // JPG Encode mode DC output
    DMA_CH_DCE_0,           // DCE input
    DMA_CH_DCE_1,           // DCE input
    DMA_CH_DCE_2,           // DCE output
    DMA_CH_DCE_3,           // DCE output
    DMA_CH_DCE_4,           // DCE input
    DMA_CH_DRE_0,           // DRE input
    DMA_CH_DRE_1,           // DRE input
    DMA_CH_DRE_2,           // DRE input
    DMA_CH_DRE_3,           // DRE input
    DMA_CH_DRE_4,           // DRE input
    DMA_CH_DRE_5,           // DRE output
    DMA_CH_DRE_6,           // DRE output
    DMA_CH_DRE_7,           // DRE output
    // Ctrl 6

    DMA_CH_IFE_0,           // IFE input
    DMA_CH_IFE_1,           // IFE output
    DMA_CH_RHE_0,           // RHE input
    DMA_CH_RHE_1,           // RHE input
    DMA_CH_RHE_2,           // RHE input
    DMA_CH_RHE_3,           // RHE input
    DMA_CH_RHE_4,           // RHE input
    DMA_CH_RHE_5,           // RHE output
    DMA_CH_RHE_6,           // RHE input
    DMA_CH_RHE_7,           // RHE output
    DMA_CH_H264_0,          // H.264 (TBD)
    DMA_CH_H264_1,          // H.264 (TBD)
    DMA_CH_H264_2,          // H.264 (TBD)
    DMA_CH_H264_3,          // H.264 (TBD)
    DMA_CH_H264_4,          // H.264 (TBD)
    DMA_CH_H264_5,          // H.264 (TBD)
    // Ctrl 7

    DMA_CH_H264_6,          // H.264 (TBD)
    DMA_CH_H264_7,          // H.264 (TBD)
    DMA_CH_H264_8,          // H.264 (TBD)
    DMA_CH_COE_0,           // COE input
    DMA_CH_COE_1,           // COE input
    DMA_CH_IPE0,            // IPE input
    DMA_CH_IPE1,            // IPE input
    DMA_CH_IPE2,            // IPE output
    DMA_CH_IPE3,            // IPE output
    DMA_CH_IME_0,           // IME input
    DMA_CH_IME_1,           // IME input
    DMA_CH_IME_2,           // IME input
    DMA_CH_IME_3,           // IME output
    DMA_CH_IME_4,           // IME output
    DMA_CH_IME_5,           // IME output
    DMA_CH_IME_6,           // IME output
    // Ctrl 8

    DMA_CH_IME_7,           // IME output
    DMA_CH_IME_8,           // IME output
    DMA_CH_IME_9,           // IME output
    DMA_CH_IME_A,           // IME output
    DMA_CH_IME_B,           // IME output
    DMA_CH_IME_C,           // IME output
    DMA_CH_IME_D,           // IME output
    DMA_CH_IME_E,           // IME input
    DMA_CH_IME_F,           // IME output
    DMA_CH_IME_10,          // IME input
    DMA_CH_IME_11,          // IME input/output
    DMA_CH_IME_12,          // IME input
    DMA_CH_IME_13,          // IME input
    DMA_CH_IME_14,          // IME input
    DMA_CH_IME_15,          // IME output
    DMA_CH_IME_16,          // IME input
    // Ctrl 9

    DMA_CH_IME_17,          // IME output
    DMA_CH_IFE2_0,          // IFE2 input
    DMA_CH_IFE2_1,          // IFE2 output
    DMA_CH_SVM_0,           // SVM input
    DMA_CH_SVM_1,           // SVM output
    DMA_CH_SDE_0,           // SDE input
    DMA_CH_SDE_1,           // SDE input
    DMA_CH_SDE_2,           // SDE input
    DMA_CH_SDE_3,           // SDE output
    DMA_CH_IVE_0,           // IVE input
    DMA_CH_IVE_1,           // IVE output
    DMA_CH_CNN_0,           // CNN input
    DMA_CH_CNN_1,           // CNN output
    // Ctrl 10

    DMA_CH_COUNT,
    DMA_CH_ALL = 0x80000000,
    ENUM_DUMMY4WORD(DMA_CH)
} DMA_CH;

/**
    DMA controller ID

*/
typedef enum
{
    DMA_ID_1,                           ///< DMA Controller
    DMA_ID_2,                           ///< DMA Controller 2

    DMA_ID_COUNT,                       //< DMA controller count

    ENUM_DUMMY4WORD(DMA_ID)
} DMA_ID;

/*
    DMA AXI channel
*/
typedef enum
{
    DMA_AXI_CH0,                                //< AXI channel 0 (CPU0)
    DMA_AXI_CH1,                                //< AXI channel 1 (CPU1)
    DMA_AXI_CH2,                                //< AXI channel 2 (DSP EDP)
    DMA_AXI_CH3,                                //< AXI channel 3 (DSP EPP)
    DMA_AXI_CH4,                                //< AXI channel 4 (DSP2 EDP)
    DMA_AXI_CH5,                                //< AXI channel 5 (DSP2 EPP)
    DMA_AXI_CH6,                                //< AXI channel 6 (USB 3.0)
    DMA_AXI_CH7,                                //< AXI channel 7 (ethernet)


    ENUM_DUMMY4WORD(DMA_AXI_CH)
} DMA_AXI_CH;
extern ER dma_setAXIRequestCount(DMA_AXI_CH channel, UINT32 count);
extern ER dma_enableChannel(DMA_CH Channel);

/**
    Check DRAM address is cacheable or not

    Check DRAM address is cacheable or not.

    @param[in] addr     DRAM address

    @return Cacheable or not
        - @b TRUE   : Cacheable
        - @b FALSE  : Non-cacheable
*/
#define dma_isCacheAddr(addr)           (((UINT32)(addr) >= 0x60000000UL) ? FALSE : TRUE)

/**
    Translate DRAM address to cacheable address.

    Translate DRAM address to cacheable address.

    @param[in] addr     DRAM address

    @return Cacheable DRAM address
*/
#define dma_getCacheAddr(addr)          ((((UINT32)(addr))>=0x60000000UL)?((UINT32)(addr)-0x60000000UL):(UINT32)(addr))


/**
    DRAM type encoding

    DRAM type encoding returned by dma_getDramType()
*/
typedef enum
{
    DDR_TYPE_DDR2,          ///< DDR2 DRAM
    DDR_TYPE_DDR3,          ///< DDR3 DRAM
    ENUM_DUMMY4WORD(DMA_DDR_TYPE)
} DMA_DDR_TYPE;

extern DMA_DDR_TYPE     dma_getDramType(void);
extern UINT32           dma_getDramBaseAddr(DMA_ID id);

/*
    Get DRAM capacity

    Get DRAM capacity of DMA controller configuration

    @return DRAM capacity
*/
extern UINT32   dma_getDramCapacity(DMA_ID id);

//@}

#endif
