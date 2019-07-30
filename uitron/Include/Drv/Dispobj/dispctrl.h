/**
    @file       dispctrl.h
    @ingroup    mISYSDisp

    @brief      Header file for Display Engine control
                This file is the header file that define the API and data type
                for Display control object
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/** \addtogroup  mISYSDisp*/
//@{

#ifndef _DISPCTRL_H
#define _DISPCTRL_H

#include "dispcomn.h"
#include "ide.h"
//#include "ide2.h"



/**
    @name Display Control Definition Group

    These definitions are used for display control API (DISP_OBJ.dispCtrl) as the display control portal.
    The display general functionalities such as subpixel/HLPF/Contrast/Saturation/Brightness (...etc) are controlled by this API.
    The user must know that the API would not set load to activate the settings.
    The User would programs all the settings needed and then use DISP_OBJ.load() to activate all the settings.
*/
//@{

/**
    Display output format type definition

    This definition is used for the OP Code: "DISPCTRL_SET_DEVICE" in the dispCtrl API to specify the display engine output format type.
    Such as RGB/YUV-Serial,RGB/YUV-Parallel,CCIR-601/656, ...etc.
*/
typedef enum {
	DISPDEV_TYPE_RGB_SERIAL,                ///< Display MODE is RGB Serial or UPS051
	DISPDEV_TYPE_RGB_PARALL,                ///< Display MODE is RGB Parallel
	DISPDEV_TYPE_RGBD,                      ///< Display MODE is RGB Dummy
	DISPDEV_TYPE_RGB_THROUGH,               ///< Display MODE is RGB through mode
	DISPDEV_TYPE_YUV,                       ///< Display MODE is YUV
	DISPDEV_TYPE_CCIR601_8BIT,              ///< Display MODE is CCIR601 8bits bus
	DISPDEV_TYPE_CCIR656_8BIT,              ///< Display MODE is CCIR656 8bits bus
	DISPDEV_TYPE_CCIR601_16BIT,             ///< Display MODE is CCIR601 16bits bus
	DISPDEV_TYPE_CCIR656_16BIT,             ///< Display MODE is CCIR656 16bits bus
	DISPDEV_TYPE_RGBDELTA_16BIT,            ///< Display MODE is RGB Delta 16bits bus

	DISPDEV_TYPE_INF_HDMI_8BIT_DDR,         ///< Display MODE is outsourcing HDMI 8bits with double-data-rate(DDR) bus
	DISPDEV_TYPE_INF_HDMI_16BIT,            ///< Display MODE is outsourcing HDMI 16bits bus
	DISPDEV_TYPE_INF_HDMI_24BIT,            ///< Display MODE is outsourcing HDMI 24bits bus

	DISPDEV_TYPE_MI,                        ///< Display MODE is Memory interface
	DISPDEV_TYPE_EMBD_TV,                   ///< Display MODE is Embedded TV Encoder
	DISPDEV_TYPE_EMBD_HDMI,                 ///< Display MODE is Embedded HDMI transmitter
	DISPDEV_TYPE_EMBD_MIPIDSI,              ///< Display MODE is Embedded MIPI DSI tx.

	DISPDEV_TYPE_OUTPUT_DRAM,               ///< Display MODE is output to dram via VDO2 channel

	ENUM_DUMMY4WORD(DISPDEV_TYPE)
} DISPDEV_TYPE;

/**
    Display Engine Source clock select definition

    This definition is used for the OP Code: "DISPCTRL_SET_SRCCLK" in the dispCtrl API to specify the display engine clock source.
*/
typedef enum {
	DISPCTRL_SRCCLK_PLL1,                   ///< The display source clock from 480MHz
	DISPCTRL_SRCCLK_PLL2,                   ///< The display source clock from PLL2
	DISPCTRL_SRCCLK_PLL4,                   ///< The display source clock from PLL4
//  DISPCTRL_SRCCLK_PLL7,
//  DISPCTRL_SRCCLK_PLL12,
	DISPCTRL_SRCCLK_PLL14,                  ///< The display source clock from PLL14

	DISPCTRL_SRCCLK_IDE2_PLL1,
	DISPCTRL_SRCCLK_IDE2_PLL2,
	DISPCTRL_SRCCLK_IDE2_PLL4,
//  DISPCTRL_SRCCLK_IDE2_PLL7,
//  DISPCTRL_SRCCLK_IDE2_PLL12,
	DISPCTRL_SRCCLK_IDE2_PLL14,
	//DISPCTRL_SRCCLK_MI_ENGINE,              ///< The display source clock from Fixed MI Engine Clock Source 240MHz

	DISPCTRL_SRCCLK_DEFAULT = DISPCTRL_SRCCLK_PLL2,///< Default display source clock selection for realchip
	DISPCTRL_SRCCLK_IDE2_DEFAULT = DISPCTRL_SRCCLK_IDE2_PLL2,///< Default display source clock selection for realchip IDE2
	ENUM_DUMMY4WORD(DISPCTRL_SRCCLK)
} DISPCTRL_SRCCLK;

/**
    Display control operation ID definition

    This field is used as the first operand in the dispCtrl API of the display object to specify
    which of the display control operation would be performed.
*/
typedef enum {
	/* SET control group */
	DISPCTRL_SET_ENABLE = DISPCMDBASE_DISPCTRL, ///< Set display engine's global enable/disable.
	DISPCTRL_SET_ALL_LYR_EN,                ///< Set ALL selected display layers Enable/Disable.
	DISPCTRL_SET_BACKGROUND,                ///< Set Display background color.
	DISPCTRL_SET_GAMMA_EN,                  ///< Set display Gamma function enable/disable. Also config the Gamma coefficients if function enabled.
	DISPCTRL_SET_ICST_EN,                   ///< Set display Inverse-Color-Space-Transform (ICST1) function enable/disable.
	DISPCTRL_SET_ICST_COEF,                 ///< Set display Inverse-Color-Space-Transform (ICST1) function coefficients.
	DISPCTRL_SET_HLPF_EN,                   ///< Set display horizontal low pass filter enable/disable.
	DISPCTRL_SET_CONST_OFS,                 ///< Set display window constant offset X/Y. This feature is used for handling the overscan/underscan hdmi tv.
	DISPCTRL_SET_SRCCLK,                    ///< Set display operating source clock
	DISPCTRL_SET_GAMMA_Y,                   ///< Set display gamma table Y
	DISPCTRL_SET_GAMMA_RGB,                 ///< Set display gamma table RGB
	DISPCTRL_SET_ICST0_EN,                  ///< Set display ICST0 enable/disable
	DISPCTRL_SET_ICST0_COEF,                ///< Set display Inverse-Color-Space-Transform (ICST0) function coefficients.
	DISPCTRL_SET_CST_EN,                    ///< Set display CST1 enable/disable
	DISPCTRL_SET_CC_EN,                     ///< Set display color control enable/disable
	DISPCTRL_SET_CCA_EN,                    ///< Set display color component adjustment enable/disable
	DISPCTRL_SET_CCA_HUE_EN,                ///< Set display color component Hue adjustment enable/disable
	DISPCTRL_SET_CCA_YCON_EN,               ///< Set display color component Y contrast adjustment enable/disable
	DISPCTRL_SET_CCA_CCON_EN,               ///< Set display color component C contrast adjustment enable/disable
	DISPCTRL_SET_CC_HUE,                    ///< Set display color control hue table
	DISPCTRL_SET_CC_INT,                    ///< Set display color control intensity table
	DISPCTRL_SET_CC_SAT,                    ///< Set display color control saturation table
	DISPCTRL_SET_CC_DDS,                    ///< Set display color control dds table
	DISPCTRL_SET_CC_INT_OFS,                ///< Set display color control intensity offset
	DISPCTRL_SET_CC_SAT_OFS,                ///< Set display color control saturation offset
	DISPCTRL_SET_CCA_YCON,                  ///< Set display color component adjustment Y contrast
	DISPCTRL_SET_CCA_CCON,                  ///< Set display color component adjustment C contrast
	DISPCTRL_SET_CCA_YOFS,                  ///< Set display color component adjustment Y offset
	DISPCTRL_SET_CCA_COFS,                  ///< Set display color component adjustment C offset


	/* GET control group */
	DISPCTRL_GET_ENABLE,                    ///< Get display engine's global enable/disable.
	DISPCTRL_GET_ALL_LYR_EN,                ///< Get ALL selected display layers Enable/Disable.
	DISPCTRL_GET_BACKGROUND,                ///< Get Display background color.
	DISPCTRL_GET_GAMMA_EN,                  ///< Get display Gamma function enable/disable. Also config the Gamma coefficients if function enabled.
	DISPCTRL_GET_ICST_EN,                   ///< Get display Inverse-Color-Space-Transform(ICST1) function enable/disable.
	DISPCTRL_GET_ICST_COEF,                 ///< Get display Inverse-Color-Space-Transform(ICST1) function coefficients.
	DISPCTRL_GET_HLPF_EN,                   ///< Get display horizontal low pass filter enable/disable.
	DISPCTRL_GET_CONST_OFS,                 ///< Get display window constant offset X/Y. This feature is used for handling the overscan/underscan hdmi tv.
	DISPCTRL_GET_SRCCLK,                    ///< Get display operating source clock
	DISPCTRL_GET_GAMMA_Y,                   ///< Get display gamma table Y
	DISPCTRL_GET_GAMMA_RGB,                 ///< Get display gamma table RGB
	DISPCTRL_GET_ICST0_EN,                  ///< Get display ICST0 enable/disable
	DISPCTRL_GET_ICST0_COEF,                ///< Get display Inverse-Color-Space-Transform (ICST0) function coefficients.
	DISPCTRL_GET_CST_EN,                    ///< Get display CST1 enable/disable
	DISPCTRL_GET_CC_EN,                     ///< Get display color control enable/disable
	DISPCTRL_GET_CCA_EN,                    ///< Get display color component adjustment enable/disable
	DISPCTRL_GET_CCA_HUE_EN,                ///< Get display color component Hue adjustment enable/disable
	DISPCTRL_GET_CCA_YCON_EN,               ///< Get display color component Y contrast adjustment enable/disable
	DISPCTRL_GET_CCA_CCON_EN,               ///< Get display color component C contrast adjustment enable/disable
	DISPCTRL_GET_CC_HUE,                    ///< Get display color control hue table
	DISPCTRL_GET_CC_INT,                    ///< Get display color control intensity table
	DISPCTRL_GET_CC_SAT,                    ///< Get display color control saturation table
	DISPCTRL_GET_CC_DDS,                    ///< Get display color control dds table
	DISPCTRL_GET_CC_INT_OFS,                ///< Get display color control intensity offset
	DISPCTRL_GET_CC_SAT_OFS,                ///< Set display color control saturation offset
	DISPCTRL_GET_CCA_YCON,                  ///< Get display color component adjustment Y contrast
	DISPCTRL_GET_CCA_CCON,                  ///< Get display color component adjustment C contrast
	DISPCTRL_GET_CCA_YOFS,                  ///< Get display color component adjustment Y offset
	DISPCTRL_GET_CCA_COFS,                  ///< Get display color component adjustment C offset


	ENUM_DUMMY4WORD(DISPCTRL_OP)
} DISPCTRL_OP;

/**
    Display Control Parameter Definition

    This definition is used as the second operand in the dispCtrl API to specify the display control parameters
    for each of the display control opeation.
*/
typedef struct {
	union {
		/* SET control group */

		/**
		    Set Display Engine Enable/Disable

		    This structure is used for display control operation code "DISPCTRL_SET_ENABLE"
		    to set the display enable/disable. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ENABLE.bEn     = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ENABLE, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_ENABLE
			BOOL            bEn;            ///< Display Enable/Disable
		} SET_ENABLE;

		/**
		    Set Display Engine layers enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_ALL_LYR_EN"
		    to set the display layers enable/disable. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = TRUE;
		\n  DispCtrl.SEL.SET_ALL_LYR_EN.DispLyr = DISPLAYER_VDO1|DISPLAYER_OSD1;    // Enable the specified layers.
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
		\n  Or Using like this:
		\n  DispCtrl.SEL.SET_ALL_LYR_EN.bEn     = FALSE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ALL_LYR_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { //DISPCTRL_SET_ALL_LYR_EN
			BOOL            bEn;            ///< For ALL display layers ON/OFF. TRUE: Enable. FALSE:Disable.
			DISPLAYER       DispLyr;        ///< If bEn = TRUE, the specified display layers would be enabled.
			///< If bEn = FALSE, "ALL" the display layers would be disabled. This parameter would be useless.
		} SET_ALL_LYR_EN;

		/**
		    Set the Display default background color

		    This structure is used for display control operation code "DISPCTRL_SET_BACKGROUND"
		    to set the display default background color.
		    If none of the display layer is enabled, the default color on the display screen is the background color.
		    This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_BACKGROUND.uiColorY     = 0x00;
		\n  DispCtrl.SEL.SET_BACKGROUND.uiColorCB    = 0x80;
		\n  DispCtrl.SEL.SET_BACKGROUND.uiColorCR    = 0x80;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_BACKGROUND, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_BACKGROUND
			UINT32          uiColorY;       ///< Background color  Y value. Must be within 0~0xFF.
			UINT32          uiColorCB;      ///< Background color CB value. Must be within 0~0xFF.
			UINT32          uiColorCR;      ///< Background color CR value. Must be within 0~0xFF.
		} SET_BACKGROUND;

		/**
		    Set Display Engine Gamma Function

		    This structure is used for display control operation code "DISPCTRL_SET_GAMMA_EN"
		    to set the display gamma function enable/disable. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_GAMMA_EN.bEn          = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_GAMMA_EN, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_GAMMA_EN
			BOOL            bEn;           ///< Enable/Disable the Gamma function
			//UINT8           *pGammaTabY;    ///< The 17 UINT8 entries gamma table for component Y. This field is useless if bEnY=FALSE.
		} SET_GAMMA_EN;

		/**
		    Set Display Engine Inverse Color Space Transform

		    This structure is used for display control operation code "DISPCTRL_SET_ICST_EN"
		    to set the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ICST_EN.bEn        = TRUE;
		\n  DispCtrl.SEL.SET_ICST_EN.Select     = CST_YCBCR2RGB;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ICST_EN, &DispCtrl);
		\n  Or Using like this:
		\n  DispCtrl.SEL.SET_ICST_EN.bEn        = FALSE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ICST_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_ICST_EN
			BOOL            bEn;            ///< The Inverse color space transform function enable/disable.
			CST_SEL         Select;         ///< The color space transform of RGB2YCBCR or YCBCR2RGB.
		} SET_ICST_EN;

		/**
		    Set Display Engine ICST Coeficients

		    This structure is used for display control operation code "DISPCTRL_SET_ICST_COEF"
		    to set the display Inverse Color Space Transform Coeficients. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ICST_COEF.piCoef     = (Address of the ICST Coeficients table);
		\n  DispCtrl.SEL.SET_ICST_COEF.iPreOfsY   = 0;
		\n  DispCtrl.SEL.SET_ICST_COEF.iPreOfsCb  = -128;
		\n  DispCtrl.SEL.SET_ICST_COEF.iPreOfsCr  = -128;
		\n  DispCtrl.SEL.SET_ICST_COEF.iPostOfsY  = 0;
		\n  DispCtrl.SEL.SET_ICST_COEF.iPostOfsCb = 0;
		\n  DispCtrl.SEL.SET_ICST_COEF.iPostOfsCr = 0;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ICST_COEF, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_ICST_COEF
			INT32          *piCoef;         ///< The 3X3 ICST Matrix coefficients. The coefficient format is (Q3.8). Valid range from -2048 to 2047.
			///< The coefficients in order are: Ry, Ru, Rv, Gy, Gu, Gv, By, Bu, Bv.

			INT16           iPreOfsY;       ///< Inverse color space transform Pre-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPreOfsCb;      ///< Inverse color space transform Pre-Offset value for Cb. Valid range from -128 to 128.
			INT16           iPreOfsCr;      ///< Inverse color space transform Pre-Offset value for Cr. Valid range from -128 to 128.

			INT16           iPostOfsY;      ///< Inverse color space transform Post-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPostOfsCb;     ///< Inverse color space transform Post-Offset value for Cb. Valid range from -128 to 128.
			INT16           iPostOfsCr;     ///< Inverse color space transform Post-Offset value for Cr. Valid range from -128 to 128.
		} SET_ICST_COEF;

		/**
		    Set Display Engine Horizontal Low Pass Filter Enable/Disable

		    This structure is used for display control operation code "DISPCTRL_SET_HLPF_EN"
		    to set the display Horizontal Low Pass Filter Enable/Disable. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_HLPF_EN.bEn     = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_HLPF_EN, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_HLPF_EN
			BOOL            bEn;            ///< TRUE: Enable horizontal LPF.  FALSE: Disable horizontal LPF.
		} SET_HLPF_EN;

		/**
		    Set Display Engine Constant Window Offset

		    This structure is used for display control operation code "DISPCTRL_SET_CONST_OFS"
		    to set the display  Constant Window Offset. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX    = 0x0;
		\n  DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY    = 0x0;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CONST_OFS, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CONST_OFS
			UINT32          uiWinConstOfsX;    ///< The display Video/OSD layer output window constant offset X
			UINT32          uiWinConstOfsY;    ///< The display Video/OSD layer output window constant offset Y
		} SET_CONST_OFS;

		/**
		    Set Display Engine Clock Source

		    This structure is used for display control operation code "DISPCTRL_SET_SRCCLK"
		    to set the display Clock Source. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_SRCCLK.SrcClk    = DISPCTRL_SRCCLK_PLL2;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_SRCCLK, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_SRCCLK
			DISPCTRL_SRCCLK    SrcClk;         ///< Select display source clock
		} SET_SRCCLK;


		/**
		    Set Display Engine Gamma Table Y

		    This structure is used for display control operation code "DISPCTRL_SET_GAMMA_Y"
		    to set the display gamma table Y. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_GAMMA_Y.pGammaTabY    = (Address of the Gamman Table);
		\n  pDispObj->dispCtrl(DISPCTRL_SET_GAMMA_Y, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_GAMMA_Y
			UINT8           *pGammaTabY;    ///< The UINT8 entries gamma table for component Y.(for 220:17 entries)
		} SET_GAMMA_Y;

		/**
		    Set Display Engine Gamma Table RGB

		    This structure is used for display control operation code "DISPCTRL_SET_GAMMA_RGB"
		    to set the display gamma table R. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_GAMMA_RGB.pGammaTabR    = (Address of the Gamman Table R);
		 \n  DispCtrl.SEL.SET_GAMMA_RGB.pGammaTabG    = (Address of the Gamman Table G);
		  \n  DispCtrl.SEL.SET_GAMMA_RGB.pGammaTabB    = (Address of the Gamman Table B);
		\n  pDispObj->dispCtrl(DISPCTRL_SET_GAMMA_RGB, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_GAMMA_RGB
			UINT8           *pGammaTabR;    ///< The UINT8 entries gamma table for component R.(for 650:33 entries)
			UINT8           *pGammaTabG;    ///< The UINT8 entries gamma table for component G.(for 650:33 entries)
			UINT8           *pGammaTabB;    ///< The UINT8 entries gamma table for component B.(for 650:33 entries)
		} SET_GAMMA_RGB;

		/**
		    Set Display Engine ICST0

		    This structure is used for display control operation code "DISPCTRL_SET_ICST0_EN"
		    to set the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ICST0_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ICST0_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_ICST0_EN
			BOOL            bEn;            ///< The ICST0 enable/disable.
		} SET_ICST0_EN;

		/**
		    Set Display Engine ICST0 Coeficients

		    This structure is used for display control operation code "DISPCTRL_SET_ICST0_COEF"
		    to set the display Inverse Color Space Transform Coeficients. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_ICST0_COEF.piCoef     = (Address of the ICST0 Coeficients table);
		\n  DispCtrl.SEL.SET_ICST0_COEF.iPreOfsY   = 0;
		\n  DispCtrl.SEL.SET_ICST0_COEF.iPreOfsCb  = -128;
		\n  DispCtrl.SEL.SET_ICST0_COEF.iPreOfsCr  = -128;
		\n  DispCtrl.SEL.SET_ICST0_COEF.iPostOfsY  = 0;
		\n  DispCtrl.SEL.SET_ICST0_COEF.iPostOfsCb = 0;
		\n  DispCtrl.SEL.SET_ICST0_COEF.iPostOfsCr = 0;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_ICST0_COEF, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_ICST0_COEF
			INT32          *piCoef;         ///< The 3X3 ICST Matrix coefficients. The coefficient format is (Q3.8). Valid range from -2048 to 2047.
			///< The coefficients in order are: Ry, Ru, Rv, Gy, Gu, Gv, By, Bu, Bv.

			INT16           iPreOfsY;       ///< Inverse color space transform Pre-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPreOfsCb;      ///< Inverse color space transform Pre-Offset value for Cb. Valid range from -128 to 128.
			INT16           iPreOfsCr;      ///< Inverse color space transform Pre-Offset value for Cr. Valid range from -128 to 128.

			INT16           iPostOfsY;      ///< Inverse color space transform Post-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPostOfsCb;     ///< Inverse color space transform Post-Offset value for Cb. Valid range from -128 to 128.
			INT16           iPostOfsCr;     ///< Inverse color space transform Post-Offset value for Cr. Valid range from -128 to 128.
		} SET_ICST0_COEF;


		/**
		    Set Display Engine CST

		    This structure is used for display control operation code "DISPCTRL_SET_CST_EN"
		    to set the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CST_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CST_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CST_EN
			BOOL            bEn;            ///< The CST  enable/disable.
		} SET_CST_EN;

		/**
		    Set Display Engine Color control enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CC_EN"
		    to set the display Color control adjustment. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CC_EN
			BOOL            bEn;            ///< The CC  enable/disable.
		} SET_CC_EN;

		/**
		    Set Display Engine Color control adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_EN"
		    to set the display Color control adjustment. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CCA_EN
			BOOL            bEn;            ///< The CCA  enable/disable.
		} SET_CCA_EN;


		/**
		    Set Display Engine Color control Hue adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_HUE_EN"
		    to set the display Color control Hue adjustment . This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_HUE_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_HUE_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CCA_HUE_EN
			BOOL            bEn;            ///< The CCA HUE  enable/disable.
		} SET_CCA_HUE_EN;

		/**
		    Set Display Engine Color control Y contrast adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_YCON_EN"
		    to set the display Color control Y contrast adjustment . This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_YCON_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_YCON_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CCA_YCON_EN
			BOOL            bEn;            ///< The CCA YCON  enable/disable.
		} SET_CCA_YCON_EN;

		/**
		    Set Display Engine Color control C contrast adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_CCON_EN"
		    to set the display Color control C contrast adjustment . This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_CCON_EN.bEn        = TRUE;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_CCON_EN, &DispCtrl);
		    @note This control operation is VD Latached, so the user must set pDispObj->load to validate the configurations.
		*/
		struct { // DISPCTRL_SET_CCA_CCON_EN
			BOOL            bEn;            ///< The CCA CCON  enable/disable.
		} SET_CCA_CCON_EN;

		/**
		    Set Display Engine Color control Hue table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_HUE"
		    to set the display Color control  Hue table. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_HUE.pHueTab       = (Address of the HUE table);
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_HUE, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CC_HUE
			UINT8           *pHueTab;    ///< The UINT8 entries hue table.(for 650:24 entries)
		} SET_CC_HUE;


		/**
		    Set Display Engine Color control Intensity table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_INT"
		    to set the display Color control  Intensity table. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_INT.pIntTab       = (Address of the INT table);
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_INT, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CC_INT
			INT8            *pIntTab;    ///< The INT8 entries intensity table.(for 650:24 entries)
		} SET_CC_INT;

		/**
		    Set Display Engine Color control Saturation table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_SAT"
		    to set the display Color control  Saturation table. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_SAT.pSatTab       = (Address of the SAT table);
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_SAT, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CC_SAT
			INT8            *pSatTab;    ///< The INT8 entries saturation table.(for 650:24 entries)
		} SET_CC_SAT;

		/**
		    Set Display Engine Color control DDS table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_DDS"
		    to set the display Color control  DDS table. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_DDS.pDdsTab       = (Address of the DDS table);
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_DDS, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CC_DDS
			UINT8           *pDdsTab;    ///< The INT8 entries DDS table.(for 650:8 entries)
		} SET_CC_DDS;


		/**
		    Set Display Engine Color control Intensity offset

		    This structure is used for display control operation code "DISPCTRL_SET_CC_INT_OFS"
		    to set the display Color control   Intensity offset. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_INT_OFS.iintofs  = 0x80;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_INT_OFS, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CC_INT_OFS
			INT8             iintofs;    ///< The intensity offset, INT8(-128~127)
		} SET_CC_INT_OFS;

		/**
		    Set Display Engine Color control Saturation offset

		    This structure is used for display control operation code "DISPCTRL_SET_CC_SAT_OFS"
		    to set the display Color control   Saturation offset. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CC_SAT_OFS.isatofs  = 0x80;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CC_SAT_OFS, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CC_SAT_OFS
			INT8             isatofs;    ///< The Saturation offset, INT8(-128~127)
		} SET_CC_SAT_OFS;

		/**
		   Set Display Engine Color component adjustment Y contrast

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_YCON"
		   to set the display Color component adjustment Y contrast. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_YCON.uiycon  = 0x80;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_YCON, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CCA_YCON
			UINT8            uiycon;    ///< The Y contrast (0~255)
		} SET_CCA_YCON;

		/**
		   Set Display Engine Color component adjustment Cb/Cr contrast

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_CCON"
		   to set the display Color component adjustment Y contrast. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_CCON.uiccon  = 0x80;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_CCON, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CCA_CCON
			UINT8            uiccon;    ///< The C contrast (0~255)
		} SET_CCA_CCON;

		/**
		   Set Display Engine Color component adjustment Y offset

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_YOFS"
		   to set the display Color component adjustment Y offset. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_YOFS.iyofs  = 0x00;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_YOFS, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CCA_YOFS
			INT8              iyofs;    ///< The Y Ofs (-128~127)
		} SET_CCA_YOFS;

		/**
		   Set Display Engine Color component adjustment Cb/Cr offset

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_COFS"
		   to set the display Color component adjustment Y contrast. This structure shuld be used like this:
		\n  DispCtrl.SEL.SET_CCA_COFS.uicbofs  = 0x80;
		\n  DispCtrl.SEL.SET_CCA_COFS.uicrofs  = 0x80;
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CCA_COFS, &DispCtrl);
		*/
		struct { // DISPCTRL_SET_CCA_COFS
			UINT8            uicbofs;    ///< The Cb offset (0~255)
			UINT8            uicrofs;    ///< The Cr offset (0~255)
		} SET_CCA_COFS;


		/* GET control group */

		/**
		    Get Display Engine Enable/Disable

		    This structure is used for display control operation code "DISPCTRL_GET_ENABLE"
		    to get the display enable/disable. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_ENABLE, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ENABLE.bEn"
		    is the display enable/disable information.
		*/
		struct { // DISPCTRL_GET_ENABLE
			BOOL            bEn;          ///< Get display Enable/Disable
		} GET_ENABLE;

		/**
		    Get Display Engine layers enable information

		    This structure is used for display control operation code "DISPCTRL_GET_ALL_LYR_EN"
		    to get the display layers enable information. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_ALL_LYR_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ALL_LYR_EN.DispLyr"
		    is the display layers enable information.
		*/
		struct { // DISPCTRL_GET_ALL_LYR_EN
			DISPLAYER       DispLyr;      ///< Get the information of which the display layers are enabled.
		} GET_ALL_LYR_EN;


		/**
		    Get the Display default background color

		    This structure is used for display control operation code "DISPCTRL_GET_BACKGROUND"
		    to get the display default background color.
		    If none of the display layer is enabled, the default color on the display screen is the background color.
		    This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_BACKGROUND, &DispCtrl);
		\n  "DispCtrl.SEL.GET_BACKGROUND.uiColorY"
		\n  "DispCtrl.SEL.GET_BACKGROUND.uiColorCB"
		\n  "DispCtrl.SEL.GET_BACKGROUND.uiColorCR"
		     is the Display default background color
		*/
		struct { // DISPCTRL_GET_BACKGROUND
			UINT32          uiColorY;       ///< Background color  Y value. Must be within 0~0xFF.
			UINT32          uiColorCB;      ///< Background color CB value. Must be within 0~0xFF.
			UINT32          uiColorCR;      ///< Background color CR value. Must be within 0~0xFF.
		} GET_BACKGROUND;

		/**
		    Get Display Engine Gamma Function

		    This structure is used for display control operation code "DISPCTRL_GET_GAMMA_EN"
		    to set the display gamma function enable/disable and also the gamma table. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_GAMMA_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_GAMMA_EN.bEn "
		    is the Display Engine Gamma Function Enable/Disable
		*/
		struct { // DISPCTRL_GET_GAMMA_EN
			BOOL            bEn;            ///< Enable/Disable the Gamma function for component Y
		} GET_GAMMA_EN;

		/**
		    Get Display Engine Inverse Color Space Transform

		    This structure is used for display control operation code "DISPCTRL_GET_ICST_EN"
		    to get the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_ICST_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ICST_EN.bEn"
		\n  "DispCtrl.SEL.GET_ICST_EN.Select"
		    is the Display Engine Inverse Color Space Transform
		*/
		struct { // DISPCTRL_GET_ICST_EN
			BOOL            bEn;            ///< The Inverse color space transform function enable/disable.
			CST_SEL         Select;         ///< The color space transform of RGB2YCBCR or YCBCR2RGB.
		} GET_ICST_EN;

		/**
		    Get Display Engine ICST Coeficients

		    This structure is used for display control operation code "DISPCTRL_GET_ICST_COEF"
		    to get the display Inverse Color Space Transform Coeficients. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_ICST_COEF, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ICST_COEF.piCoef "
		\n  "DispCtrl.SEL.GET_ICST_COEF.iPreOfsY"
		\n  "DispCtrl.SEL.GET_ICST_COEF.iPreOfsCb"
		\n  "DispCtrl.SEL.GET_ICST_COEF.iPreOfsCr"
		\n  "DispCtrl.SEL.GET_ICST_COEF.iPostOfsY"
		\n  "DispCtrl.SEL.GET_ICST_COEF.iPostOfsCb"
		\n  "DispCtrl.SEL.GET_ICST_COEF.iPostOfsCr"
		    is the Display Engine ICST Coeficients.
		*/
		struct { // DISPCTRL_GET_ICST_COEF
			INT32          *piCoef;         ///< The 3X3 ICST Matrix coefficients. The coefficient format is (Q3.8). Valid range from -2048 to 2047.
			///< The coefficients in order are: Ry, Ru, Rv, Gy, Gu, Gv, By, Bu, Bv.

			INT16           iPreOfsY;       ///< Inverse color space transform Pre-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPreOfsCb;      ///< Inverse color space transform Pre-Offset value for Cb. Valid range from -128 to 127.
			INT16           iPreOfsCr;      ///< Inverse color space transform Pre-Offset value for Cr. Valid range from -128 to 127.

			INT16           iPostOfsY;      ///< Inverse color space transform Post-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPostOfsCb;     ///< Inverse color space transform Post-Offset value for Cb. Valid range from -128 to 127.
			INT16           iPostOfsCr;     ///< Inverse color space transform Post-Offset value for Cr. Valid range from -128 to 127.
		} GET_ICST_COEF;

		/**
		    Get Display Engine Horizontal Low Pass Filter Enable/Disable

		    This structure is used for display control operation code "DISPCTRL_GET_HLPF_EN"
		    to get the display Horizontal Low Pass Filter Enable/Disable. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_HLPF_EN, &DispCtrl);
		\n  "DispCtrl.SEL.SET_HLPF_EN.bEn"
		    is the Display Engine Horizontal Low Pass Filter Enable/Disable.
		*/
		struct { // DISPCTRL_GET_HLPF_EN
			BOOL            bEn;            ///< TRUE: Enable horizontal LPF.  FALSE: Disable horizontal LPF.
		} GET_HLPF_EN;

		/**
		    Get Display Engine Constant Window Offset

		    This structure is used for display control operation code "DISPCTRL_GET_CONST_OFS"
		    to get the display  Constant Window Offset. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_SET_CONST_OFS, &DispCtrl);
		\n  "DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX"
		\n  "DispCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY"
		    is the Display Engine Constant Window Offset.
		*/
		struct { // DISPCTRL_GET_CONST_OFS
			UINT32          uiWinConstOfsX;    ///< The display Video/OSD layer output window constant offset X
			UINT32          uiWinConstOfsY;    ///< The display Video/OSD layer output window constant offset Y
		} GET_CONST_OFS;

		/**
		    Get Display Engine Clock Source

		    This structure is used for display control operation code "DISPCTRL_GET_SRCCLK"
		    to get the display Clock Source. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_SRCCLK, &DispCtrl);
		\n  "DispCtrl.SEL.GET_SRCCLK.SrcClk"
		    is the display Clock Source information.
		*/
		struct { // DISPCTRL_GET_SRCCLK
			DISPCTRL_SRCCLK    SrcClk;   ///< Get display source clock
		} GET_SRCCLK;

		/**
		    Get Display Engine Gamma Table Y

		    This structure is used for display control operation code "DISPCTRL_GET_GAMMA_Y"
		    to get the display gamma table Y. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_GAMMA_Y, &DispCtrl);
		\n  "DispCtrl.SEL.GET_GAMMA_Y.pGammaTabY"
		    is the Display Engine Gamma Table Y
		*/
		struct { // DISPCTRL_GET_GAMMA_Y
			UINT8           *pGammaTabY;    ///< The UINT8 entries gamma table for component Y.(for 220:17 entries)
		} GET_GAMMA_Y;

		/**
		    Get Display Engine Gamma Table RGB

		    This structure is used for display control operation code "DISPCTRL_GET_GAMMA_RGB"
		    to get the display gamma table RGB. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_GAMMA_RGB, &DispCtrl);
		\n  "DispCtrl.SEL.GET_GAMMA_RGB.pGammaTabR"
		\n  "DispCtrl.SEL.GET_GAMMA_RGB.pGammaTabG"
		\n  "DispCtrl.SEL.GET_GAMMA_RGB.pGammaTabB"
		    is the Display Engine Gamma Table R
		*/
		struct { // DISPCTRL_GET_GAMMA_RGB
			UINT8           *pGammaTabR;    ///< The UINT8 entries gamma table for component R.(for 650:33 entries)
			UINT8           *pGammaTabG;    ///< The UINT8 entries gamma table for component G.(for 650:33 entries)
			UINT8           *pGammaTabB;    ///< The UINT8 entries gamma table for component B.(for 650:33 entries)
		} GET_GAMMA_RGB;


		/**
		    Get Display Engine ICST0 enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_ICST0_EN"
		    to get the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_ICST0_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ICST0_EN.bEn "
		   is the Display Engine ICST0
		*/
		struct { // DISPCTRL_GET_ICST0_EN
			BOOL            bEn;            ///< The ICST0 enable/disable.
		} GET_ICST0_EN;


		/**
		    Get Display Engine ICST0 Coeficients

		    This structure is used for display control operation code "DISPCTRL_GET_ICST0_COEF"
		    to get the display Inverse Color Space Transform Coeficients. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_ICST0_COEF, &DispCtrl);
		\n  "DispCtrl.SEL.GET_ICST0_COEF.piCoef "
		\n  "DispCtrl.SEL.GET_ICST0_COEF.iPreOfsY"
		\n  "DispCtrl.SEL.GET_ICST0_COEF.iPreOfsCb"
		\n  "DispCtrl.SEL.GET_ICST0_COEF.iPreOfsCr"
		\n  "DispCtrl.SEL.GET_ICST0_COEF.iPostOfsY"
		\n  "DispCtrl.SEL.GET_ICST0_COEF.iPostOfsCb"
		\n  "DispCtrl.SEL.GET_ICST0_COEF.iPostOfsCr"
		    is the Display Engine ICST0 Coeficients.
		*/
		struct { // DISPCTRL_GET_ICST0_COEF
			INT32          *piCoef;         ///< The 3X3 ICST Matrix coefficients. The coefficient format is (Q3.8). Valid range from -2048 to 2047.
			///< The coefficients in order are: Ry, Ru, Rv, Gy, Gu, Gv, By, Bu, Bv.

			INT16           iPreOfsY;       ///< Inverse color space transform Pre-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPreOfsCb;      ///< Inverse color space transform Pre-Offset value for Cb. Valid range from -128 to 127.
			INT16           iPreOfsCr;      ///< Inverse color space transform Pre-Offset value for Cr. Valid range from -128 to 127.

			INT16           iPostOfsY;      ///< Inverse color space transform Post-Offset value for Y.  Valid range from -128 to 127.
			INT16           iPostOfsCb;     ///< Inverse color space transform Post-Offset value for Cb. Valid range from -128 to 127.
			INT16           iPostOfsCr;     ///< Inverse color space transform Post-Offset value for Cr. Valid range from -128 to 127.
		} GET_ICST0_COEF;


		/**
		    Get Display Engine CST

		    This structure is used for display control operation code "DISPCTRL_SET_CST_EN"
		    to get the display Inverse Color Space Transform configurations. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CST_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CST_EN.bEn"
		   is the Display Engine CST
		*/
		struct { // DISPCTRL_GET_CST_EN
			BOOL            bEn;            ///< The CST  enable/disable.
		} GET_CST_EN;

		/**
		    Get Display Engine Color control enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CC_EN"
		    to get the display Color control adjustment. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_EN.bEn"
		   is the Display Engine Color control enable/disable
		*/
		struct { // DISPCTRL_GET_CC_EN
			BOOL            bEn;            ///< The CC  enable/disable.
		} GET_CC_EN;

		/**
		    Get Display Engine Color control adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_EN"
		    to get the display Color control adjustment. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_EN.bEn"
		    is the Color control adjustment enable/disable
		*/
		struct { // DISPCTRL_GET_CCA_EN
			BOOL            bEn;            ///< The CCA  enable/disable.
		} GET_CCA_EN;


		/**
		    Get Display Engine Color control Hue adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_HUE_EN"
		    to get the display Color control Hue adjustment . This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_HUE_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_HUE_EN.bEn"
		    is the Display Engine Color control Hue adjustment enable/disable
		*/
		struct { // DISPCTRL_GET_CCA_HUE_EN
			BOOL            bEn;            ///< The CCA HUE  enable/disable.
		} GET_CCA_HUE_EN;

		/**
		    Get Display Engine Color control Y contrast adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_YCON_EN"
		    to get the display Color control Y contrast adjustment . This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_YCON_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_YCON_EN.bEn"
		    is the Display Engine Color control Y contrast adjustment enable/disable
		*/
		struct { // DISPCTRL_GET_CCA_YCON_EN
			BOOL            bEn;            ///< The CCA YCON  enable/disable.
		} GET_CCA_YCON_EN;

		/**
		    Get Display Engine Color control C contrast adjustment enable/disable

		    This structure is used for display control operation code "DISPCTRL_SET_CCA_CCON_EN"
		    to get the display Color control C contrast adjustment . This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_CCON_EN, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_CCON_EN.bEn"
		    is the Display Engine Color control C contrast adjustment enable/disable
		*/
		struct { // DISPCTRL_GET_CCA_CCON_EN
			BOOL            bEn;            ///< The CCA CCON  enable/disable.
		} GET_CCA_CCON_EN;

		/**
		    Get Display Engine Color control Hue table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_HUE"
		    to get the display Color control  Hue table. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_HUE, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_HUE.pHueTab"
		    is the Display Engine Color control Hue table
		*/
		struct { // DISPCTRL_GET_CC_HUE
			UINT8           *pHueTab;    ///< The UINT8 entries hue table.(for 650:24 entries)
		} GET_CC_HUE;


		/**
		    Get Display Engine Color control Intensity table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_INT"
		    to get the display Color control  Intensity table. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_INT, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_INT.pIntTab"
		    is the Display Engine Color control Intensity table
		*/
		struct { // DISPCTRL_GET_CC_INT
			INT8            *pIntTab;    ///< The INT8 entries intensity table.(for 650:24 entries)
		} GET_CC_INT;

		/**
		    Get Display Engine Color control Saturation table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_SAT"
		    to get the display Color control  Saturation table. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_SAT, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_SAT.pSatTab"
		    is the Display Engine Color control Saturation table
		*/
		struct { // DISPCTRL_GET_CC_SAT
			INT8            *pSatTab;    ///< The INT8 entries saturation table.(for 650:24 entries)
		} GET_CC_SAT;

		/**
		    Get Display Engine Color control DDS table

		    This structure is used for display control operation code "DISPCTRL_SET_CC_DDS"
		    to get the display Color control  DDS table. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_DDS, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_DDS.pDdsTab"
		    is the Display Engine Color control DDS table
		*/
		struct { // DISPCTRL_GET_CC_DDS
			UINT8           *pDdsTab;    ///< The INT8 entries DDS table.(for 650:8 entries)
		} GET_CC_DDS;


		/**
		    Get Display Engine Color control Intensity offset

		    This structure is used for display control operation code "DISPCTRL_SET_CC_INT_OFS"
		    to get the display Color control   Intensity offset. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_INT_OFS, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_INT_OFS.iintofs"
		    is the Display Engine Color control Intensity offset
		*/
		struct { // DISPCTRL_GET_CC_INT_OFS
			INT8             iintofs;    ///< The intensity offset, INT8(-128~127)
		} GET_CC_INT_OFS;

		/**
		    Get Display Engine Color control Saturation offset

		    This structure is used for display control operation code "DISPCTRL_SET_CC_SAT_OFS"
		    to get the display Color control   Saturation offset. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CC_SAT_OFS, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CC_SAT_OFS.isatofs"
		    is the Display Engine Color control Saturation offset
		*/
		struct { // DISPCTRL_GET_CC_SAT_OFS
			INT8             isatofs;    ///< The Saturation offset, INT8(-128~127)
		} GET_CC_SAT_OFS;

		/**
		   Get Display Engine Color component adjustment Y contrast

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_YCON"
		   to get the display Color component adjustment Y contrast. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_YCON, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_YCON.uiycon"
		   is the Display Engine Color component adjustment Y contrast
		*/
		struct { // DISPCTRL_GET_CCA_YCON
			UINT8            uiycon;    ///< The Y contrast (0~255)
		} GET_CCA_YCON;

		/**
		   Get Display Engine Color component adjustment Cb/Cr contrast

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_CCON"
		   to get the display Color component adjustment Y contrast. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_CCON, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_CCON.uiccon"
		   is the Display Engine Color component adjustment Cb/Cr contrast
		*/
		struct { // DISPCTRL_GET_CCA_CCON
			UINT8            uiccon;    ///< The C contrast (0~255)
		} GET_CCA_CCON;

		/**
		   Get Display Engine Color component adjustment Y offset

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_YOFS"
		   to get the display Color component adjustment Y offset. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_YOFS, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_YOFS.iyofs"
		   is the Display Engine Color component adjustment Y offset
		*/
		struct { // DISPCTRL_GET_CCA_YOFS
			INT8              iyofs;    ///< The Y Ofs (-128~127)
		} GET_CCA_YOFS;

		/**
		   Get Display Engine Color component adjustment Cb/Cr offset

		   This structure is used for display control operation code "DISPCTRL_SET_CCA_COFS"
		   to get the display Color component adjustment Y contrast. This structure shuld be used like this:
		\n  pDispObj->dispCtrl(DISPCTRL_GET_CCA_COFS, &DispCtrl);
		\n  "DispCtrl.SEL.GET_CCA_COFS.uicbofs"
		\n  "DispCtrl.SEL.GET_CCA_COFS.uicrofs"
		    is the Display Engine Color component adjustment Cb/Cr offset
		*/
		struct { // DISPCTRL_GET_CCA_COFS
			UINT8            uicbofs;    ///< The Cb offset (0~255)
			UINT8            uicrofs;    ///< The Cr offset (0~255)
		} GET_CCA_COFS;


	} SEL;
} DISPCTRL_PARAM, *PDISPCTRL_PARAM;
//@}



typedef ER(*DISPCTRL)(DISPCTRL_OP, PDISPCTRL_PARAM);


#endif
//@}