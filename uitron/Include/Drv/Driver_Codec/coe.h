/**
    Header file for COE module

    This file is the header file that define the API for COE module.

    @file       coe.h
    @ingroup    mIDrvCodec_COE
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _COE_H
#define _COE_H

#include "Type.h"

/**
    @addtogroup mIDrvCodec_COE
*/
//@{

/**
    COE OSD layer

    COE OSD layer
    @note There are two layer osd in coe funtion
*/
typedef enum {
	OSD_LAYER1,     ///< osd layer 1
	OSD_LAYER2,     ///< osd layer 2
	ENUM_DUMMY4WORD(COE_OSD_LAYER)
} COE_OSD_LAYER;

/**
    COE OSD Region

    COE OSD Region
    @note There are 16 region in a osd layer
*/
typedef enum {
	COE_REGION1,
	COE_REGION2,
	COE_REGION3,
	COE_REGION4,
	COE_REGION5,
	COE_REGION6,
	COE_REGION7,
	COE_REGION8,
	COE_REGION9,
	COE_REGION10,
	COE_REGION11,
	COE_REGION12,
	COE_REGION13,
	COE_REGION14,
	COE_REGION15,
	COE_REGION16,

	ENUM_DUMMY4WORD(OSD_REGION)
} OSD_REGION;

/**
    COE OSD format

    COE OSD format
    @note COE support 3 kinds of format: RGB565, ARGB1555, and ARGB4444
*/
typedef enum {
	COE_RGB565,
	COE_ARGB1555,
	COE_ARGB4444,
	ENUM_DUMMY4WORD(COE_OSD_FMT)
} COE_OSD_FMT;

/**
    COE parameter mode

    COE parameter mode
    @note
*/
typedef enum {
	COE_CST_AUTO_MODE = 0,    ///< Auto parameter mode: use default CST coefficient
	COE_CST_USER_MODE = 1,    ///< User parameter mode: user define own CST coefficient
	ENUM_DUMMY4WORD(COE_CST_MODE)
} COE_CST_MODE;

/**
    COE configuration identifier

    COE configuration identifier
    @note
*/
typedef struct {
	UINT16              coe_src_w;          ///< Input video source width
	UINT16              coe_src_h;          ///< Input video source height
} COE_CONFIG, *PCOE_CONFIG;


/**
    CST coefficient

    CST coefficient
    @note
*/
typedef struct {
	COE_CST_MODE        mode;               ///< COE CST coefficient mode(0: default, 1: user define)
	UINT8               coe_cst_coef;       ///< COE CST ceofficient 0
	UINT8               coe_cst_coef2;      ///< COE CST ceofficient 1
	UINT8               coe_cst_coef3;      ///< COE CST ceofficient 2
	UINT8               coe_cst_coef4;      ///< COE CST ceofficient 3
} COE_CST, *PCOE_CST;

/**
    QP

    Quantization parameters for each osd region
    @note
*/
typedef struct {
	BOOL                en;                 ///< OSD QP enable
	BOOL                fix;                ///< OSD QP fixd value(1: fixed QP
	///<                   0: QP will change by rate control)

	INT8                val;                ///< OSD QP value(6 bits)(if fix = 1, val = 0 ~ 51
	///<                         fix = 0, val = -25 ~ 25

} COE_OSD_QP, *PCOE_OSD_QP;

/**
    colorkey

    colorkey for each osd region
    @note
*/
typedef struct {
	BOOL                en;                 ///< OSD colorkey enable
	UINT16              val;                ///< OSD colorkey value(16 bits)
} COE_OSD_CKEY, *PCOE_OSD_CKEY;



/**
    COE OSD configuration identifier

    COE OSD configuration identifier
    @note
    There is a alpha table in COE
        - @b alpha table: Alpha_Table[16] = {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255}

        - @b RGB565: For RGB565, ALPHA is 8 bit alpha.
        - @b ARGB1555: For ARGB1555, if A = 0, ALPHA = Alpha_Table[alpha[3..0]]
                       if A = 1, ALPHA = Alpha_Table[alpha[7..4]]
        - @b ARGB4444: For ARGB4444, ALPHA = Alpha_Table[A]
*/
typedef struct {
	COE_OSD_FMT         format;             ///< OSD format
	UINT32              addr;               ///< OSD start address(31 bits, word alignment)
	UINT16              x;                  ///< OSD x position(13 bits)
	UINT16              y;                  ///< OSD y position(13 bits
	UINT16              w;                  ///< OSD width(13 bits)
	UINT16              h;                  ///< OSD height(13 bits)
	UINT16              lofs;               ///< OSD line offset(15 bits, unit:word)
	UINT8               alpha;              ///< OSD alpha(8 bits)
	COE_OSD_CKEY        colorkey;           ///< OSD colorkey
	COE_OSD_QP          qp;                 ///< OSD qp
} COE_OSD_CONFIG, *PCOE_OSD_CONFIG;

/**
     YUV

     YUV
*/
typedef enum {
	COE_Y  = 0,
	COE_U  = 1,
	COE_V  = 2,
	ENUM_DUMMY4WORD(COE_YUV)
} COE_YUV;





// -------------------------------------------------
// The API for the COE codec driver
// -------------------------------------------------
extern ER       coe_open(void);
extern ER       coe_close(void);
extern void     coe_trigger(void);
extern BOOL     coe_isOpened(void);
extern ID       coe_getLockStatus(void);
extern UINT32   coe_checkViolation(void);

extern ER       coe_setCOEConfig(COE_CONFIG uiConfig);
extern ER       coe_setCSTConfig(PCOE_CST pCst);
extern ER       coe_setOSDx(COE_OSD_LAYER layer, OSD_REGION region, PCOE_OSD_CONFIG pConfig);
extern ER       coe_setCOEOvercount(UINT16 overtime);
extern void     coe_getCOEConfig(PCOE_CONFIG pConfig);
extern ER       coe_getCSTConfig(PCOE_CST pCst);
extern void     coe_getOSDx(COE_OSD_LAYER layer, OSD_REGION region, PCOE_OSD_CONFIG pConfig);
extern UINT16   coe_getCOEOvercount(void);
extern void     coe_enableOSD(COE_OSD_LAYER layer, OSD_REGION region);
extern void     coe_disableOSD(COE_OSD_LAYER layer, OSD_REGION region);

//@}
#endif

