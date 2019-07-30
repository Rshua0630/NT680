/**
    Header file of Clock Generator DAL library

    Exported header file of Clock Generator DAL library.

    @file       dal_ckgen.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/
#ifndef _DAL_CKGEN_H
#define _DAL_CKGEN_H

#include "Type.h"

/**
    CKG IDentification

    This is used at dal_ckg_get_object() to retrieve the ckg driver object.
*/
typedef enum {
	DAL_CKG_ID_CKG = 0x00,      ///< DAL of CKG Interface.

	DAL_CKG_ID_MAX,
	ENUM_DUMMY4WORD(DAL_CKG_ID)
} DAL_CKG_ID;

/**
    CKG SIEX Module Clock Source Selection.
    This is used at .set_module_clk_src() and .get_module_clk_src().
*/
typedef enum {

	DAL_CKG_ID_MODULE_SIEX_SRC_FIXED        = 0x00, ///< Set SIEX clock source to FIXED clock source
	DAL_CKG_ID_MODULE_SIEX_SRC_ADJ          = 0x01, ///< Set SIEX clock source to ADJUSTIBLE clock source

	DAL_CKG_ID_MODULE_SIEX_SRC_MAX,
	ENUM_DUMMY4WORD(DAL_CKG_ID_MODULE_SIEX_SRC)
} DAL_CKG_ID_MODULE_SIEX_SRC;

/**
    CKG SIEX Module PXCLK Source Selection.
    This is used at .set_module_clk_src() and .get_module_clk_src().
*/
typedef enum {

	DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX = 0x00, ///< Set SIEX PXCLK from PXCLK PADX
	DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX      = 0x01, ///< Set SIEX PXCLK from MCLKX (SIE from MCLK, SIE2~SIE8 from MCLK2)
	DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X     = 0x02, ///< Set SIEX PXCLK from VX1 1X clock
	DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_2X     = 0x03, ///< Set SIEX PXCLK from VX1 2X clock

	DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MAX,
	ENUM_DUMMY4WORD(DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC)
} DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC;

/**
    CKG Module Clock Source Seleciton ID.
    This is used at .set_module_clk_src() and .get_module_clk_src().
*/
typedef enum {

	// SIEX Module MCLK Clock Source Group
	DAL_CKG_MODULE_CLKSRC_ID_SIEX_MCLK      = 0x1010,    ///< Set Clock Source of MCLK, Use DAL_CKG_ID_MODULE_SIEX_SRC_FIXED or DAL_CKG_ID_MODULE_SIEX_SRC_ADJ
	DAL_CKG_MODULE_CLKSRC_ID_SIEX_MCLK2,                ///< Set Clock Source of MCLK2, Use DAL_CKG_ID_MODULE_SIEX_SRC_FIXED or DAL_CKG_ID_MODULE_SIEX_SRC_ADJ

	// SIEX Module Clock Source Group
	DAL_CKG_MODULE_CLKSRC_ID_SIE            = 0x1020,     ///< Set Clock Source of SIE, Use DAL_CKG_ID_MODULE_SIEX_SRC_FIXED or DAL_CKG_ID_MODULE_SIEX_SRC_ADJ
	DAL_CKG_MODULE_CLKSRC_ID_SIE2,                      ///< Set Clock Source of SIE2, Use DAL_CKG_ID_MODULE_SIEX_SRC_FIXED or DAL_CKG_ID_MODULE_SIEX_SRC_ADJ
	DAL_CKG_MODULE_CLKSRC_ID_SIE3,                      ///< Set Clock Source of SIE3, Use DAL_CKG_ID_MODULE_SIEX_SRC_FIXED or DAL_CKG_ID_MODULE_SIEX_SRC_ADJ
	DAL_CKG_MODULE_CLKSRC_ID_SIE4,                      ///< Set Clock Source of SIE4, Use DAL_CKG_ID_MODULE_SIEX_SRC_FIXED or DAL_CKG_ID_MODULE_SIEX_SRC_ADJ
	//DAL_CKG_MODULE_CLKSRC_ID_SIE5,                    ///< Set Clock Source of SIE5, It only support DAL_CKG_ID_MODULE_SIEX_SRC_FIXED
	//DAL_CKG_MODULE_CLKSRC_ID_SIE6,                    ///< Set Clock Source of SIE6, It only support DAL_CKG_ID_MODULE_SIEX_SRC_FIXED
	//DAL_CKG_MODULE_CLKSRC_ID_SIE7,                    ///< Set Clock Source of SIE7, It only support DAL_CKG_ID_MODULE_SIEX_SRC_FIXED
	//DAL_CKG_MODULE_CLKSRC_ID_SIE8,                    ///< Set Clock Source of SIE8, It only support DAL_CKG_ID_MODULE_SIEX_SRC_FIXED

	// SIEX Module PXCLK Group
	DAL_CKG_MODULE_CLKSRC_ID_SIE_PXCLK      = 0x1030,     ///< Set Clock Source of SIE PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	DAL_CKG_MODULE_CLKSRC_ID_SIE2_PXCLK,                ///< Set Clock Source of SIE2 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	DAL_CKG_MODULE_CLKSRC_ID_SIE3_PXCLK,                ///< Set Clock Source of SIE3 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	DAL_CKG_MODULE_CLKSRC_ID_SIE4_PXCLK,                ///< Set Clock Source of SIE4 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	DAL_CKG_MODULE_CLKSRC_ID_SIE5_PXCLK,                ///< Set Clock Source of SIE5 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	DAL_CKG_MODULE_CLKSRC_ID_SIE6_PXCLK,                ///< Set Clock Source of SIE6 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_VX1_1X
	DAL_CKG_MODULE_CLKSRC_ID_SIE7_PXCLK,                ///< Set Clock Source of SIE7 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX
	DAL_CKG_MODULE_CLKSRC_ID_SIE8_PXCLK,                ///< Set Clock Source of SIE8 PXCLK
	///< Use DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_PXCLK_PADX
	///<  or DAL_CKG_ID_MODULE_SIEX_PXCLK_SRC_MCLKX

	DAL_CKG_MODULE_CLKSRC_ID_MAX,

	ENUM_DUMMY4WORD(DAL_CKG_MODULE_CLKSRC_CFGID)
} DAL_CKG_MODULE_CLKSRC_CFGID;

/**
    CKG Module Clock Enable/Disable Selection.
    This is used at .set_module_clk_en() and .get_module_clk_en().
*/
typedef enum {

	// SIEX Module MCLK/PXCLK Clock Group
	DAL_CKG_MODULE_CLKEN_ID_SIEX_MCLK       = 0x1110,     ///< Set Enable/Disable of MCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIEX_MCLK2,                 ///< Set Enable/Disable of MCLK2, FALSE: Disable, TRUE: Enable

	// SIEX Module Clock Group
	DAL_CKG_MODULE_CLKEN_ID_SIE             = 0x1120,     ///< Set Enable/Disable of SIE, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE2,                       ///< Set Enable/Disable of SIE2, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE3,                       ///< Set Enable/Disable of SIE3, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE4,                       ///< Set Enable/Disable of SIE4, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE5,                       ///< Set Enable/Disable of SIE5, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE6,                       ///< Set Enable/Disable of SIE6, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE7,                       ///< Set Enable/Disable of SIE7, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE8,                       ///< Set Enable/Disable of SIE8, FALSE: Disable, TRUE: Enable

	DAL_CKG_MODULE_CLKEN_ID_SIE_PXCLK       = 0x1130,     ///< Set Enable/Disable of SIE PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE2_PXCLK,                 ///< Set Enable/Disable of SIE2 PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE3_PXCLK,                 ///< Set Enable/Disable of SIE3 PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE4_PXCLK,                 ///< Set Enable/Disable of SIE4 PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE5_PXCLK,                 ///< Set Enable/Disable of SIE5 PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE6_PXCLK,                 ///< Set Enable/Disable of SIE6 PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE7_PXCLK,                 ///< Set Enable/Disable of SIE7 PXCLK, FALSE: Disable, TRUE: Enable
	DAL_CKG_MODULE_CLKEN_ID_SIE8_PXCLK,                 ///< Set Enable/Disable of SIE8 PXCLK, FALSE: Disable, TRUE: Enable

	DAL_CKG_MODULE_CLKEN_ID_MAX,

	ENUM_DUMMY4WORD(DAL_CKG_MODULE_CLKEN_CFGID)
} DAL_CKG_MODULE_CLKEN_CFGID;

/**
    CKG Module Clock Frequency Selection.
    This is used at .set_module_clk_freq() and .get_module_clk_freq().
*/
typedef enum {

	// Adjustable clock source Group
	DAL_CKG_MODULE_CLKFREQ_ID_ADJ           = 0x1200,     ///< Set Clock Freq. of Adjustable Source, Unit in Hertz. (Note: This source if for SIE relative clock)

	// SIEX Module MCLK Clock Group
	DAL_CKG_MODULE_CLKFREQ_ID_SIEX_MCLK     = 0x1210,     ///< Set Clock Freq. of MCLK, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIEX_MCLK2,               ///< Set Clock Freq. of MCLK2, Unit in Hertz.

	// SIEX Module Clock Group
	DAL_CKG_MODULE_CLKFREQ_ID_SIE           = 0x1220,     ///< Set Clock Freq. of SIE, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE2,                     ///< Set Clock Freq. of SIE2, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE3,                     ///< Set Clock Freq. of SIE3, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE4,                     ///< Set Clock Freq. of SIE4, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE5,                     ///< Set Clock Freq. of SIE5, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE6,                     ///< Set Clock Freq. of SIE6, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE7,                     ///< Set Clock Freq. of SIE7, Unit in Hertz.
	DAL_CKG_MODULE_CLKFREQ_ID_SIE8,                     ///< Set Clock Freq. of SIE8, Unit in Hertz.

	DAL_CKG_MODULE_CLKFREQ_ID_MAX,

	ENUM_DUMMY4WORD(DAL_CKG_MODULE_CLKFREQ_CFGID)
} DAL_CKG_MODULE_CLKFREQ_CFGID;


/**
    DAL of Serial Sensor Interface Object Definotions

    This is used by the DAL of Serial Sensor Interface Get Object API dal_ssenif_get_object() to get the specific module control object.
*/
typedef struct {

	/* Object Common items */

	/* Module's Source Clock Enable API */
	DAL_CKG_ID  ckg_id;                 ///< CKG Object ID.
	CHAR        name[16];               ///< CKG Object name.

	/* Module's Clock Source Selection API */
	INT32(*set_module_clk_src)(DAL_CKG_MODULE_CLKSRC_CFGID config_id, UINT32 value);    ///< Set mododule clock source configuration.
	INT32(*get_module_clk_src)(DAL_CKG_MODULE_CLKSRC_CFGID config_id);  ///< Get mododule clock source configuration.

	/* Module's Clock Frequency */
	INT32(*set_module_clk_freq)(DAL_CKG_MODULE_CLKFREQ_CFGID config_id, UINT32 value);  ///< Set mododule clock frequency.
	UINT32(*get_module_clk_freq)(DAL_CKG_MODULE_CLKFREQ_CFGID config_id);   ///< Get mododule clock frequency.

	/* Module's Clock Enable API */
	INT32(*set_module_clk_en)(DAL_CKG_MODULE_CLKEN_CFGID config_id, UINT32 value);  ///< Set mododule clock enable.
	BOOL(*get_module_clk_en)(DAL_CKG_MODULE_CLKEN_CFGID config_id);    ///< Get mododule clock enable.

} DAL_CKGOBJ, *PDAL_CKGOBJ;


/*
    Exporting API
*/

/**
    Get CKG driver object

    Get CKG driver object

    @return
     - NULL:   The CKG driver object not existed.
     - Others: The CKG driver object.
*/
extern PDAL_CKGOBJ dal_ckg_get_object(DAL_CKG_ID ckg_id);

#endif
