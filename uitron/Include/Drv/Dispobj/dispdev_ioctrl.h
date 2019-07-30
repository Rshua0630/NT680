/**
    @file       dispdev_ioctrl.h
    @ingroup    mISYSDisp

    @brief      Header file for Display device interface between display object
                and the display device.
                This file is the header file that define the API and data type
                for Display control object
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/** \addtogroup  mISYSDisp*/
//@{

#ifndef _DISPDEV_IOCTRL_H
#define _DISPDEV_IOCTRL_H

#include "display.h"


/**
    Display device control interface between display object and the display device
*/
typedef enum {
	/* SET control group */
	DISPDEV_IOCTRL_SET_ENABLE = DISPCMDBASE_IOCTRL, ///< Set display engine's global enable/disable.
	DISPDEV_IOCTRL_SET_DEVICE,                    ///< Select the display device for display engine.
	DISPDEV_IOCTRL_SET_SRGB_OUTORDER,             ///< Configure display RGB-Serial-pixels output order.
	DISPDEV_IOCTRL_SET_SYNC_INVERT,               ///< Configure Output interface singals'inversion: HSYNC / VSYNC / PIXCLK.
	DISPDEV_IOCTRL_SET_VLD_INVERT,                ///< Configure Output interface singals'inversion: HVLD  / VVLD  / FIELD / DE.
	DISPDEV_IOCTRL_SET_SUBPIXEL,                  ///< Set display subpixel function configurations.
	DISPDEV_IOCTRL_SET_WINDOW_H_TIMING,           ///< Set display global output window horizontal timings.
	DISPDEV_IOCTRL_SET_WINDOW_V_TIMING,           ///< Set display global output window vertical timings.
	DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE,           ///< Set display global output window interlaced/progressive, start-field select
	DISPDEV_IOCTRL_SET_SYNCDELAY,                 ///< Set display HSYNC / VSYNC delay clock cycles.
	DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE,          ///< Set display digital video data code timing for CCIR656
	DISPDEV_IOCTRL_SET_GAMMA_EN,                  ///< Set display Gamma function enable/disable. Also config the Gamma coefficients if function enabled.
	DISPDEV_IOCTRL_SET_CSB_EN,                    ///< Set display Contrast / Saturation / Brightness performances and also the function enable/disable.
	DISPDEV_IOCTRL_SET_YC_EXCHG,                  ///< Set display Y/C exchange and Cb/Cr exchange.
	DISPDEV_IOCTRL_SET_CLAMP,                     ///< Set display Y/C Clamp
	DISPDEV_IOCTRL_SET_DITHER_EN,                 ///< Set display Dithering functin enable/disable and the dither bits selection.
	DISPDEV_IOCTRL_SET_OUT_COMPONENT,             ///< Set display ouput component swap if RGB/YUV parallel is selected.
	DISPDEV_IOCTRL_SET_ICST_EN,                   ///< Set display Inverse-Color-Space-Transform function enable/disable.
	DISPDEV_IOCTRL_SET_HLPF_EN,                   ///< Set display horizontal low pass filter enable/disable.
	DISPDEV_IOCTRL_SET_OUT_LIMIT,                 ///< Set display output limitation values.
	DISPDEV_IOCTRL_SET_CONST_OFS,                 ///< Set display window constant offset X/Y. This feature is used for handling the overscan/underscan hdmi tv.
	DISPDEV_IOCTRL_SET_DISPSIZE,                  ///< Set current active display standard buffer/window size.
	DISPDEV_IOCTRL_SET_CLK1_2,                    ///< Set display clock1/2

	/* GET control group */
	DISPDEV_IOCTRL_GET_ENABLE,                    ///< Get display engine's global enable/disable.
	DISPDEV_IOCTRL_GET_DEVICE,                    ///< Get display device for display engine.
	DISPDEV_IOCTRL_GET_SRCCLK,                    ///< Get display operating source clock
	DISPDEV_IOCTRL_GET_REG_IF,                    ///< Get display Device(panel) Register Access Interface.
	DISPDEV_IOCTRL_GET_HDMIMODE,                  ///< Get the HDMI Audio/Video Mode
	DISPDEV_IOCTRL_GET_ACT_DEVICE,                ///< Get current active device. (PANEL/TV/HDMI)
	DISPDEV_IOCTRL_GET_PANEL_ADJUST,              ///< Get the panel adjustment callback.
	DISPDEV_IOCTRL_GET_TVADJUST,                  ///< Get the current TV adjustment configurations.
	DISPDEV_IOCTRL_GET_GAMMA_EN,                  ///< Get display Gamma function enable/disable
	DISPDEV_IOCTRL_GET_CSB_EN,                    ///< Get display Contrast / Saturation / Brightness performances and also the function enable/disable.
	DISPDEV_IOCTRL_GET_TVPAR,                     ///< Get TV paramter of user definition.
	DISPDEV_IOCTRL_GET_TVFULL,                    ///< Get TV full screen.

	/* Others */
	DISPDEV_IOCTRL_SET_LOAD,                      ///< Set Display Configuration Activation
	DISPDEV_IOCTRL_WAIT_FRAMEEND,                 ///< Wait Display Frame End
	DISPDEV_IOCTRL_WAIT_DMA_DONE,                 ///< Wait YUV output Dram done


	DISPDEV_IOCTRL_SET_CLK_FREQ,                  ///< Set IDEx clock frequency
	DISPDEV_IOCTRL_SET_CLK_EN,                    ///< Enable IDEx clock

	ENUM_DUMMY4WORD(DISPDEV_IOCTRL_OP)
} DISPDEV_IOCTRL_OP;




/**
    Display Device Parameter
*/
typedef struct {
	union {
		/* SET control group */

		/**
		    Set Display Engine Enable/Disable

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_ENABLE"
		    to set the display enable/disable. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ENABLE.bEn     = TRUE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_ENABLE, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_ENABLE
			BOOL            bEn;            ///< Display Enable/Disable
		} SET_ENABLE;

		/**
		    Set Display Engine outout format type

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_DEVICE"
		    to set the display output format type. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_DEVICE.DispDevType     = DISPDEV_IOCTRL_TYPE_RGB_SERIAL;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_DEVICE, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_DEVICE
			DISPDEV_TYPE    DispDevType;    ///< Select display device type
		} SET_DEVICE;

		/**
		    Set Display Engine PIXEL output order for Serial-RGB/RGBD/RGB-Through

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_SRGB_OUTORDER"
		    to set the display pixels output order for Serial-RGB/RGBD/RGB-Through. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_SRGB_OUTORDER.PixOrder     = IDE_PDIR_RGB;
		\n  DispCtrl.SEL.SET_SRGB_OUTORDER.OddStart     = IDE_LCD_R;
		\n  DispCtrl.SEL.SET_SRGB_OUTORDER.EvenStart    = IDE_LCD_G;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_SRGB_OUTORDER, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_SRGB_OUTORDER
			IDE_PDIR        PixOrder;       ///< RGB-Serial-pixels output order is RGB or RBG.
			IDE_PORDER      OddStart;       ///< Odd lines RGB-Serial-pixels start output from R or G or B.
			IDE_PORDER      EvenStart;      ///< Even lines RGB-Serial-pixels start output from R or G or B.
		} SET_SRGB_OUTORDER;

		/**
		    Set Display Engine HSYNC/VSYNC/PIXCLK output invert

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_SYNC_INVERT"
		    to set the display signals HSYNC/VSYNC/PIXCLK output invert. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_SYNC_INVERT.bHsInv     = TRUE;
		\n  DispCtrl.SEL.SET_SYNC_INVERT.bVsInv     = TRUE;
		\n  DispCtrl.SEL.SET_SYNC_INVERT.bClkInv    = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_SYNC_INVERT, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_SYNC_INVERT
			BOOL            bHsInv;         ///< FALSE: Active High, TRUE: Active Low. Active low means that the HSYNC changes at clk negative edge.
			BOOL            bVsInv;         ///< FALSE: Active High, TRUE: Active Low. Active low means that the HSYNC changes at clk negative edge.
			BOOL            bClkInv;        ///< FALSE: Not invert,  TRUE: Invert.
		} SET_SYNC_INVERT;

		/**
		    Set Display Engine HSYNC/VSYNC/PIXCLK output invert

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_SYNC_INVERT"
		    to set the display signals HSYNC/VSYNC/PIXCLK output invert. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_SYNC_INVERT.bHsInv     = TRUE;
		\n  DispCtrl.SEL.SET_SYNC_INVERT.bVsInv     = TRUE;
		\n  DispCtrl.SEL.SET_SYNC_INVERT.bClkInv    = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_SYNC_INVERT, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_VLD_INVERT
			BOOL            bHvldInv;       ///< FALSE: Active High, TRUE: Active Low. Active low means that the HSYNC changes at clk negative edge.
			BOOL            bVvldInv;       ///< FALSE: Active High, TRUE: Active Low. Active low means that the HSYNC changes at clk negative edge.
			BOOL            bFieldInv;      ///< FALSE: Not invert,  TRUE: Invert.
			BOOL            bDeInv;         ///< FALSE: Not invert,  TRUE: Invert.
		} SET_VLD_INVERT;

		/**
		    Set Display Engine SubPixel Funtion

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_SUBPIXEL"
		    to set the display subpixel funtion enable/disable for Odd/Even lines. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_SUBPIXEL.bOddR     = TRUE;
		\n  DispCtrl.SEL.SET_SUBPIXEL.bOddG     = TRUE;
		\n  DispCtrl.SEL.SET_SUBPIXEL.bOddB     = TRUE;
		\n  DispCtrl.SEL.SET_SUBPIXEL.bEvenR    = FALSE;
		\n  DispCtrl.SEL.SET_SUBPIXEL.bEvenG    = FALSE;
		\n  DispCtrl.SEL.SET_SUBPIXEL.bEvenB    = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_SUBPIXEL, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_SUBPIXEL
			BOOL            bOddR;          ///< Enable low pass filter for R compoent for Odd lines.
			BOOL            bOddG;          ///< Enable low pass filter for G compoent for Odd lines.
			BOOL            bOddB;          ///< Enable low pass filter for B compoent for Odd lines.

			BOOL            bEvenR;         ///< Enable low pass filter for R compoent for Even lines.
			BOOL            bEvenG;         ///< Enable low pass filter for G compoent for Even lines.
			BOOL            bEvenB;         ///< Enable low pass filter for B compoent for Even lines.
		} SET_SUBPIXEL;

		/**
		    Set Display Engine GLOBAL output window Horizontal timing

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_WINDOW_H_TIMING"
		    to set the display global output Horizontal window. This controls the horizontal timing generator in bus interface.
		    This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync     = 1;
		\n  DispCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal    = 1560;
		\n  DispCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart = 32;
		\n  DispCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd   = 1311;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_WINDOW_H_TIMING
			UINT32          uiHsync;        ///< HSYNC active pulse width in pixel clock cycles.
			UINT32          uiHtotal;       ///< The Horizontal total width would be (uiHtotal+1) pixclk cycles, including blanking pixels.
			UINT32          uiHvldStart;    ///< Horizontal valid start position.
			UINT32          uiHvldEnd;      ///< Horizontal valid end position.
		} SET_WINDOW_H_TIMING;

		/**
		    Set Display Engine GLOBAL output window Vertical timing

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_WINDOW_V_TIMING"
		    to set the display global output Vertical window. This controls the Vertical timing generator in bus interface.
		    This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = 1;
		\n  DispCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = 524;
		\n  DispCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 16;
		\n  DispCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = 495;
		\n  DispCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart= 17;
		\n  DispCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 496;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_WINDOW_V_TIMING
			UINT32          uiVsync;            ///< VSYNC active pulse width in pixel clock cycles or lines.
			UINT32          uiVtotal;           ///< The vertical total lines would be (uiVtotal+1) lines, including blanking lines.
			UINT32          uiVvldOddStart;     ///< Vertical Odd field start lines.
			UINT32          uiVvldOddEnd;       ///< Vertical Odd field end lines.
			UINT32          uiVvldEvenStart;    ///< Vertical Even field start lines.
			UINT32          uiVvldEvenEnd;      ///< Vertical Even field end lines.
		} SET_WINDOW_V_TIMING;

		/**
		    Set Display Engine Global output window type

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE"
		    to set the display Global output window type such as Interlaced/progressive. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_WINDOW_OUT_TYPE.bInterlaced     = TRUE;
		\n  DispCtrl.SEL.SET_WINDOW_OUT_TYPE.bFieldOddSt     = TRUE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE
			BOOL            bInterlaced;        ///< TRUE: Interlaced format.   FALSE: Progressive format.
			BOOL            bFieldOddSt;        ///< TRUE: First field output is ODD.  FALSE: First field output is EVEN.
		} SET_WINDOW_OUT_TYPE;

		/**
		    Set Display Engine Global output window SYNC signal delay

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_SYNCDELAY"
		    to set the display Global output window SYNC signal delay. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_SYNCDELAY.uiHsyncDly     = 0x0;
		\n  DispCtrl.SEL.SET_SYNCDELAY.uiHsyncDly     = 0x0;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_SYNCDELAY, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_SYNCDELAY
			UINT32          uiHsyncDly;     ///< Add delay count to HSYNC signal. Unit in PIXCLK cycles.
			UINT32          uiVsyncDly;     ///< Add delay count to VSYNC signal. Unit in PIXCLK cycles.
		} SET_SYNCDELAY;

		/**
		    Set Display Engine CCIR656 SYNC Code position

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE"
		    to set the display CCIR656 SYNC Code position. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x20C;
		\n  DispCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x16;
		\n  DispCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x106;
		\n  DispCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x11C;
		\n  DispCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = 0x04;
		\n  DispCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = 0x10A;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE
			UINT32          uiOddStart;         ///< CCIR656 Digital SYNC code odd start position
			UINT32          uiOddEnd;           ///< CCIR656 Digital SYNC code odd end position
			UINT32          uiEvenStart;        ///< CCIR656 Digital SYNC code Even start position
			UINT32          uiEvenEnd;          ///< CCIR656 Digital SYNC code Even end position
			UINT32          uiFieldStart;       ///< CCIR656 Digital SYNC code FIELD start position
			UINT32          uiFieldEnd;         ///< CCIR656 Digital SYNC code FIELD end position
		} SET_CCIR656_SYNCCODE;

		/**
		    Set Display Engine Gamma Function

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_GAMMA_EN"
		    to set the display gamma function enable/disable and also the gamma table. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_GAMMA_EN.bEn           = TRUE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_GAMMA_EN, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_GAMMA_EN
			BOOL            bEn;           ///< Enable/Disable the Gamma function for component Y
			//UINT8           *pGammaTabY;    ///< The 17 UINT8 entries gamma table for component Y. This field is useless if bEnY=FALSE.
		} SET_GAMMA_EN;

		/**
		    Set Display Engine Contrast/Saturation/Brightness configurations

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CSB_EN"
		    to set the display Contrast/Saturation/Brightness configurations. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_GAMMA_EN.bEn          = TRUE;
		\n  DispCtrl.SEL.SET_GAMMA_EN.uiContrast   = 0x40;
		\n  DispCtrl.SEL.SET_GAMMA_EN.uiSaturation = 0x40;
		\n  DispCtrl.SEL.SET_GAMMA_EN.uiBrightness = 0x00;
		\n  dispDevCtrl(DDISPDEV_IOCTRL_SET_CSB_EN, &DispCtrl);
		\n  Or Using like this:
		\n  DispCtrl.SEL.SET_GAMMA_EN.bEn          = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CSB_EN, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_CSB_EN
			BOOL            bEn;            ///< Set Contrast/Saturation/Brightness adjustment function enable/disable

			UINT32          uiContrast;     ///< Contrast value, valid range from 0x0 to 0xFF. default value is 0x40.
			UINT32          uiSaturation;   ///< Saturation value, valid range from 0x0 to 0x1FF. default value is 0x40.
			UINT32          uiBrightness;   ///< The brightness values, Valid range between -64 to 63. 2's complement value.
		} SET_CSB_EN;

		/**
		    Set Display Engine YUV output Format Y/Cb/Cr component exchange

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_YC_EXCHG"
		    to set the display YUV output Format Y/Cb/Cr component exchange. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_YC_EXCHG.bCbCrExchg    = FALSE;
		\n  DispCtrl.SEL.SET_YC_EXCHG.bYCExchg      = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_YC_EXCHG, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_YC_EXCHG
			BOOL            bCbCrExchg;     ///< FALSE: CB first.  TRUE: CR first. This field is also valid for YUV parallel 16bits out.
			BOOL            bYCExchg;       ///< FALSE: C first.   TRUE: Y first. This field is also valid for YUV parallel 16bits out.
		} SET_YC_EXCHG;

		/**
		    Set Display Engine YUV output Format Y/C Clamp

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CLAMP"
		    to set the display YUV output Format Y/Cb/Cr component exchange. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CLAMP.uiClamp    =  IDE_YCCCLAMP_NOCLAMP;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CLAMP, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_CLAMP
			IDE_YCCCLAMP    uiClamp;     ///< Clamp value
		} SET_CLAMP;


		/**
		    Set Display Engine Dithering Function

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_DITHER_EN"
		    to set the display Dithering Function. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_DITHER_EN.bEn          = TRUE;
		\n  DispCtrl.SEL.SET_DITHER_EN.bFreeRun     = FALSE;
		\n  DispCtrl.SEL.SET_DITHER_EN.Rbits        = IDE_DITHER_5BITS;
		\n  DispCtrl.SEL.SET_DITHER_EN.Gbits        = IDE_DITHER_6BITS;
		\n  DispCtrl.SEL.SET_DITHER_EN.Bbits        = IDE_DITHER_5BITS;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_DITHER_EN, &DispCtrl);
		\n  Or Using like this:
		\n  DispCtrl.SEL.SET_DITHER_EN.bEn          = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_DITHER_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_DITHER_EN
			BOOL            bEn;            ///< Select dithering function enable/disable. This function is designed for RGB color-domain.
			BOOL            bFreeRun;       ///< Select dithering at freeRun mode enable/disable. If bEn=FALSE, this setting can be ignored.

			IDE_DITHER_VBITS Rbits;         ///< Select dithering bits for R color. If bEn=FALSE, this setting can be ignored.
			IDE_DITHER_VBITS Gbits;         ///< Select dithering bits for G color. If bEn=FALSE, this setting can be ignored.
			IDE_DITHER_VBITS Bbits;         ///< Select dithering bits for B color. If bEn=FALSE, this setting can be ignored.
		} SET_DITHER_EN;

		/**
		    Set Display Engine Output R/G/B (Y/Cb/Cr) Component Swap and bit Order Swap

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_OUT_COMPONENT"
		    to set the display TG Output R/G/B (Y/Cb/Cr) Component Swap and bit Order Swap. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_OUT_COMPONENT.Comp0     = IDE_COMPONENT_R;
		\n  DispCtrl.SEL.SET_OUT_COMPONENT.Comp1     = IDE_COMPONENT_G;
		\n  DispCtrl.SEL.SET_OUT_COMPONENT.Comp2     = IDE_COMPONENT_B;
		\n  DispCtrl.SEL.SET_OUT_COMPONENT.bBitSwap  = FALSE;
		\n  DispCtrl.SEL.SET_OUT_COMPONENT.bLength   = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_OUT_COMPONENT, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_OUT_COMPONENT
			IDE_OUT_COMP    Comp0;          ///< Select RGB/YUV parallel output component-0 as R/G/B or Y/U/V.
			IDE_OUT_COMP    Comp1;          ///< Select RGB/YUV parallel output component-1 as R/G/B or Y/U/V.
			IDE_OUT_COMP    Comp2;          ///< Select RGB/YUV parallel output component-2 as R/G/B or Y/U/V.

			BOOL            bBitSwap;       ///< Select RGB/YUV parallel output bit order swap enable/disable.
			BOOL            bLength;        ///< Select RGB/YUV parallel output bit length. FALSE is 8bitsfor each component. TRUE is 6bits for each component.
		} SET_OUT_COMPONENT;

		/**
		    Set Display Engine Inverse Color Space Transform

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_ICST_EN"
		    to set the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ICST_EN.bEn        = TRUE;
		\n  DispCtrl.SEL.SET_ICST_EN.Select     = CST_YCBCR2RGB;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_ICST_EN, &DispCtrl);
		\n  Or Using like this:
		\n  DispCtrl.SEL.SET_ICST_EN.bEn        = FALSE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_ICST_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_ICST_EN
			BOOL            bEn;            ///< The Inverse color space transform function enable/disable.
			CST_SEL         Select;         ///< The color space transform of RGB2YCBCR or YCBCR2RGB.
		} SET_ICST_EN;

		/**
		    Set Display Engine Horizontal Low Pass Filter Enable/Disable

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_HLPF_EN"
		    to set the display Horizontal Low Pass Filter Enable/Disable. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_HLPF_EN.bEn     = TRUE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_HLPF_EN, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_HLPF_EN
			BOOL            bEn;            ///< TRUE: Enable horizontal LPF.  FALSE: Disable horizontal LPF.
		} SET_HLPF_EN;

		/**
		    Set Display Engine Bus Output Value Limiter

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_OUT_LIMIT"
		    to set the displayBus Output Value Limiter. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_OUT_LIMIT.uiYLow     = 0x00;
		\n  DispCtrl.SEL.SET_OUT_LIMIT.uiYUp      = 0xFF;
		\n  DispCtrl.SEL.SET_OUT_LIMIT.uiCbLow    = 0x00;
		\n  DispCtrl.SEL.SET_OUT_LIMIT.uiCbUp     = 0xFF;
		\n  DispCtrl.SEL.SET_OUT_LIMIT.uiCrLow    = 0x00;
		\n  DispCtrl.SEL.SET_OUT_LIMIT.uiCrUp     = 0xFF;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_OUT_LIMIT, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_OUT_LIMIT
			UINT8           uiYLow;         ///<  Y Value Output Lower Bound
			UINT8           uiYUp;          ///<  Y Value Output Upper Bound
			UINT8           uiCbLow;        ///< Cb Value Output Lower Bound
			UINT8           uiCbUp;         ///< Cb Value Output Upper Bound
			UINT8           uiCrLow;        ///< Cr Value Output Lower Bound
			UINT8           uiCrUp;         ///< Cr Value Output Upper Bound
		} SET_OUT_LIMIT;

		/**
		    Set Display Engine Constant Window Offset

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CONST_OFS"
		    to set the display  Constant Window Offset. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX    = 0x0;
		\n  DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY    = 0x0;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CONST_OFS, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPDEV_IOCTRL_SET_CONST_OFS
			UINT32          uiWinConstOfsX;    ///< The display Video/OSD layer output window constant offset X
			UINT32          uiWinConstOfsY;    ///< The display Video/OSD layer output window constant offset Y
		} SET_CONST_OFS;

		/**
		    Set Display Device optimal buffer/window size

		    This structure is used for display device control operation code "DISPDEV_IOCTRL_SET_DISPSIZE"
		    to set the device optimal size. This operation is normally used in the panel device open,
		    and the user can use the "DISPDEV_IOCTRL_GET_DISPSIZE: to get the optimally buffer window size for this device.
		    This structure shuld be used like this:
		\n  DispDev.SEL.SET_DISPSIZE.uiBufWidth    = 320;
		\n  DispDev.SEL.SET_DISPSIZE.uiBufHeight   = 240;
		\n  DispDev.SEL.SET_DISPSIZE.uiWinWidth    = 320;
		\n  DispDev.SEL.SET_DISPSIZE.uiWinHeight   = 240;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_DISPSIZE, &DispDev);
		*/
		struct { // DISPDEV_IOCTRL_SET_DISPSIZE
			UINT32              uiBufWidth;     ///< Suggest Buffer width. Unit in pixels.
			UINT32              uiBufHeight;    ///< Suggest Buffer Height. Unit in pixels.

			UINT32              uiWinWidth;     ///< Suggest Window width. Unit in pixels.
			UINT32              uiWinHeight;    ///< Suggest Window Height. Unit in pixels.
		} SET_DISPSIZE;


		/**
		    Set Display Engine Clock1/2

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CLK1_2"
		    to set the display clock 1/2. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CLK1_2.bClk1_2    =  TRUE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CLK1_2, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_CLK1_2
			BOOL               bClk1_2;     ///< Clock 1/2
		} SET_CLK1_2;


		/* GET control group */

		/**
		    Get Display Engine Enable/Disable

		    This structure is used for display control operation code "DISPDEV_IOCTRL_GET_ENABLE"
		    to get the display enable/disable. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_ENABLE, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ENABLE.bEn"
		    is the display enable/disable information.
		*/
		struct { // DISPDEV_IOCTRL_GET_ENABLE
			BOOL            bEn;          ///< Get display Enable/Disable
		} GET_ENABLE;

		/**
		    Get Display Engine outout format type

		    This structure is used for display control operation code "DISPDEV_IOCTRL_GET_DEVICE"
		    to get the display output format type. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_DEVICE, &DispCtrl);
		\n  "DispCtrl.SEL.GET_DEVICE.DispDevType"
		    is the display Engine outout format type information
		*/
		struct { // DISPDEV_IOCTRL_GET_ENABLE
			DISPDEV_TYPE    DispDevType;    ///< Get display device type
		} GET_DEVICE;

		/**
		    Get Display Engine Clock Source

		    This structure is used for display control operation code "DISPDEV_IOCTRL_GET_SRCCLK"
		    to get the display Clock Source. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_SRCCLK, &DispCtrl);
		\n  "DispCtrl.SEL.GET_SRCCLK.SrcClk"
		    is the display Clock Source information.
		*/
		struct { // DISPDEV_IOCTRL_GET_SRCCLK
			DISPCTRL_SRCCLK    SrcClk;   ///< Get display source clock
		} GET_SRCCLK;

		/**
		    Get the user assigned LCD Control interface configurations

		    This structure is used for display device control operation code "DISPDEV_IOCTRL_GET_REG_IF"
		    to get the user assigned LCD Control interface configurations. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_TVADJUST, &DispDev);
		\n  "DispDev.SEL.GET_REG_IF.LcdCtrl"/"DispDev.SEL.GET_REG_IF.uiSifCh"/"DispDev.SEL.GET_REG_IF.uiGpioSen"/ ..
		    are the user assigned LCD Control interface configurations.
		*/
		struct { // DISPDEV_IOCTRL_GET_REG_IF
			DISPDEV_LCDCTRL LcdCtrl;        ///< Get LCD Control Interface Select

			SIF_CH          uiSifCh;        ///< If interface is SIF, this field is the assigned SIF CH number.
			UINT32          uiGpioSen;      ///< If interface is GPIO, this field is the assigned GPIO Pin ID for SIF SEN.
			UINT32          uiGpioClk;      ///< If interface is GPIO, this field is the assigned GPIO Pin ID for SIF CLK.
			UINT32          uiGpioData;     ///< If interface is GPIO, this field is the assigned GPIO Pin ID for SIF DATA.
		} GET_REG_IF;

		/**
		    Get HDMI Display VIDEO/AUDIO Mode

		    This structure is used for display device control operation code "DISPDEV_IOCTRL_GET_HDMIMODE"
		    to get the HDMI VIDEO/AUDIO Mode. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_HDMIMODE, &DispDev);
		\n  "DispDev.SEL.GET_HDMIMODE.VideoID"/"DispDev.SEL.GET_HDMIMODE.AudioID"
		    are the HDMI VIDEO/AUDIO mode selection.
		*/
		struct { // DISPDEV_IOCTRL_GET_HDMIMODE
			HDMI_VIDEOID    VideoID;        ///< HDMI VIDEO ID select
			HDMI_AUDIOFMT   AudioID;        ///< HDMI AUDIO ID select
		} GET_HDMIMODE;


		/**
		    Get the current active Display Device ID

		    This structure is used for display device control operation code "DISPDEV_IOCTRL_GET_ACT_DEVICE"
		    to get the current active display device of the display object. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_ACT_DEVICE, &DispDev);
		\n  "DispDev.SEL.GET_ACT_DEVICE.DevID"
		    is the current active display device ID.
		*/
		struct { // DISPDEV_IOCTRL_GET_ACT_DEVICE
			DISPDEV_ID  DevID;              ///< Current Active display device ID.
		} GET_ACT_DEVICE;

		/**
		    Get the user defined PANEL adjustment callback

		    This structure is used for display device control operation code "DISPDEV_IOCTRL_GET_PANEL_ADJUST"
		    to get the user defined PANEL adjustment callback. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_PANEL_ADJUST, &DispDev);
		\n  "DispDev.SEL.GET_PANEL_ADJUST.pfpAdjust"
		    is the user defined PANEL adjustment callback.
		*/
		struct { // DISPDEV_IOCTRL_GET_PANEL_ADJUST
			FP              pfpAdjust;      ///< Get the panel adjustment callback function
		} GET_PANEL_ADJUST;

		/**
		    Get the user assigned TV adjustment resistor value

		    This structure is used for display device control operation code "DISPDEV_IOCTRL_GET_TVADJUST"
		    to get the user assigned TV adjustment resistor value. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_TVADJUST, &DispDev);
		\n  "DispDev.SEL.GET_TVADJUST.TvAdjust"
		    is the user assigned TV adjustment resistor value.
		*/
		struct { // DISPDEV_IOCTRL_GET_TVADJUST
			DISPDEV_TVADJUST    TvAdjust;   ///< Get the user assigned TV adjustment resistor value
		} GET_TVADJUST;

		/**
		    Get Display Engine Gamma Function

		    This structure is used for display control operation code "DISPDEV_IOCTRL_GET_GAMMA_EN"
		    to get the display gamma function enable/disable and also the gamma table. This structure shuld be used like this:
		\n  dispDevCtrl(DISPDEV_IOCTRL_GET_GAMMA_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_GAMMA_EN.bEn "
		    is the Display Engine Gamma Function Disable/Enable
		*/
		struct { // DISPDEV_IOCTRL_GET_GAMMA_EN
			BOOL            bEn;           ///< Enable/Disable the Gamma function for component Y/RGB
		} GET_GAMMA_EN;

		/**
		    Get Display Engine Contrast/Saturation/Brightness configurations

		    This structure is used for display control operation code "DISPDEV_IOCTRL_GET_CSB_EN"
		    to get the display Contrast/Saturation/Brightness configurations. This structure shuld be used like this:
		\n  dispDevCtrl(DDISPDEV_IOCTRL_GET_CSB_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CSB_EN.bEn"
		\n  "DispCtrl.SEL.GET_CSB_EN.uiContrast"
		\n  "DispCtrl.SEL.GET_CSB_EN.uiSaturation"
		\n  "DispCtrl.SEL.GET_CSB_EN.uiBrightness"
		   is the Display Engine Contrast/Saturation/Brightness configurations.
		*/
		struct { // DISPDEV_IOCTRL_GET_CSB_EN
			BOOL            bEn;            ///< Set Contrast/Saturation/Brightness adjustment function enable/disable

			UINT32          uiContrast;     ///< Contrast value, valid range from 0x0 to 0xFF. default value is 0x40.
			UINT32          uiSaturation;   ///< Saturation value, valid range from 0x0 to 0x1FF. default value is 0x40.
			UINT32          uiBrightness;   ///< The brightness values, Valid range between -64 to 63. 2's complement value.
		} GET_CSB_EN;

		/**
		    Get TV parameter

		    This structure is used for display device control operation code "DISPDEV_GET_IOCTRL_TVPAR"
		    to get the TV's parameters.
		    This structure shuld be used like this:
		\n  pDispObj->devCtrl(DISPDEV_IOCTRL_GET_TVPAR, &DispDev);
		\n  "DispDev.SEL.GET_TVPAR.bEnUser" is Enable/Disable user define value
		\n  "DispDev.SEL.GET_TVPAR.uiBLL" is the TV's blank level
		\n  "DispDev.SEL.GET_TVPAR.uiBRL" is the TV's burst level
		\n  "DispDev.SEL.GET_TVPAR.uiSetup" is the TV's setup level
		\n  "DispDev.SEL.GET_TVPAR.uiYScale" is the TV's y scale level
		\n  "DispDev.SEL.GET_TVPAR.uiCbScale" is the TV's cb scale level
		\n  "DispDev.SEL.GET_TVPAR.uiCrScale" is the TV's cr scale level
		*/
		struct { // DISPDEV_IOCTRL_GET_TVPAR
			BOOL        bEnUser;               ///< Enable/Disable user's setting value
			UINT32      uiNTSC_BLL;            ///< TV NTSC Blank Level(valid value:0~0xFF)
			UINT32      uiNTSC_BRL;            ///< TV NTSC Burst Level(valid value:0~0xFF)
			UINT32      uiNTSC_Setup;          ///< TV NTSC setupt(valid value:0~0xFF)
			UINT32      uiNTSC_YScaling;       ///< TV NTSC Y Scale(valid value:0~0xFF)
			UINT32      uiNTSC_CbScaling;      ///< TV NTSC Cb Scale(valid value:0~0xFF)
			UINT32      uiNTSC_CrScaling;      ///< TV NTSC Cr Scale(valid value:0~0xFF)
			UINT32      uiPAL_BLL;             ///< TV PAL Blank Level(valid value:0~0xFF)
			UINT32      uiPAL_BRL;             ///< TV PAL Burst Level(valid value:0~0xFF)
			UINT32      uiPAL_Setup;           ///< TV PAL setupt(valid value:0~0xFF)
			UINT32      uiPAL_YScaling;        ///< TV PAL Y Scale(valid value:0~0xFF)
			UINT32      uiPAL_CbScaling;       ///< TV PAL Cb Scale(valid value:0~0xFF)
			UINT32      uiPAL_CrScaling;       ///< TV PAL Cr Scale(valid value:0~0xFF)
		} GET_TVPAR;


		/**
		    Get TV Full screen

		    This structure is used for display device control operation code "DISPDEV_GET_IOCTRL_TVFULL"
		    to get the TV's parameters.
		    This structure shuld be used like this:
		\n  pDispObj->devCtrl(DISPDEV_IOCTRL_GET_TVFULL, &DispDev);
		*/
		struct { // DISPDEV_IOCTRL_GET_TVFULL
			BOOL        bEnFull;               ///< Enable/Disable tv full screen
		} GET_TVFULL;



		/**
		    Set Display Engine clock frequency

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CLK_FREQ"
		    to set the display clock frequency. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CLK_FREQ.bUse2x    =  FALSE;
		\n  DispCtrl.SEL.SET_CLK_FREQ.uiFreq    =  10000000;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CLK_FREQ, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_CLK_FREQ
			BOOL               bYCC8bit;        ///< YCbCr serial 8 bit (use clock2x), Format:YCbCr/CCIR601/CCIR656 8bit set to TRUE
			///< others set to FALSE
			UINT32             uiFreq;          ///< clock freq: unit: Hz
		} SET_CLK_FREQ;

		/**
		    Set Display Engine clock Enable/Disable

		    This structure is used for display control operation code "DISPDEV_IOCTRL_SET_CLK_EN"
		    to set the display clock frequency. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CLK_EN.bClkEn    =  TRUE;
		\n  dispDevCtrl(DISPDEV_IOCTRL_SET_CLK_EN, &DispCtrl);
		*/
		struct { // DISPDEV_IOCTRL_SET_CLK_EN
			BOOL               bClkEn;          ///< clock enable/disable
		} SET_CLK_EN;


	} SEL;
} DISPDEV_IOCTRL_PARAM, *PDISPDEV_IOCTRL_PARAM;


typedef ER(*DISPDEV_IOCTRL)(DISPDEV_IOCTRL_OP, PDISPDEV_IOCTRL_PARAM);


#endif
//@}

