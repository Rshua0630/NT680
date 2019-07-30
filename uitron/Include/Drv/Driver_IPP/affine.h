/**
    Affine module driver


    @file       affine.h
    @ingroup    mIDrvIPP_Affine
    @note

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef   _AFFINE_H
#define   _AFFINE_H

#include "Driver.h"

/**
    @addtogroup mIDrvIPP_Affine
*/
//@{

/**
    Affine configuration ID

    @note For affine_setConfig()
*/
typedef enum {
	AFFINE_CONFIG_ID_FREQ,              ///< Affine operating frequency (unit: MHz). Context can be:
	///< - @b 120: 120 MHz
	///< - @b 160: 160 MHz
	///< - @b 192: 192 MHz
	///< - @b 240: 240 MHz

	// For emulation:
	AFFINE_CONFIG_ID_COEFF_A,           //< Affine coefficient A. Context can be UINT32 stores Q1.15
	AFFINE_CONFIG_ID_COEFF_B,           //< Affine coefficient B. Context can be UINT32 stores Q1.15
	AFFINE_CONFIG_ID_COEFF_C,           //< Affine coefficient C. Context can be UINT32 stores Q14.15
	AFFINE_CONFIG_ID_COEFF_D,           //< Affine coefficient D. Context can be UINT32 stores Q1.15
	AFFINE_CONFIG_ID_COEFF_E,           //< Affine coefficient E. Context can be UINT32 stores Q1.15
	AFFINE_CONFIG_ID_COEFF_F,           //< Affine coefficient F. Context can be UINT32 stores Q14.15
	AFFINE_CONFIG_ID_SUBBLK1,           //< Affine SubBlock1 Register
	AFFINE_CONFIG_ID_SUBBLK2,           //< Affine SubBlock2 Register
	AFFINE_CONFIG_ID_DIRECT_COEFF,      //< Direct affine coefficient path. Context can be:
	//< - @b TRUE: affine_request() will use coefficients set by AFFINE_CONFIG_ID_COEFF_A ~ AFFINE_CONFIG_ID_COEFF_F
	//< - @b FALSE: affine_request() will use coefficients passed through PAFFINE_REQUEST (default value)

	ENUM_DUMMY4WORD(AFFINE_CONFIG_ID)
} AFFINE_CONFIG_ID;

/**
    Affine data format

    @note For AFFINE_REQUEST
*/
typedef enum {
	AFFINE_FORMAT_FORMAT_8BITS,             ///< Normal 8 bits data
	AFFINE_FORMAT_FORMAT_16BITS_UVPACK,     ///< 16 bits U/V pack data and apply to both plane

	ENUM_DUMMY4WORD(AFFINE_FORMAT)
} AFFINE_FORMAT;

/**
    Affine coefficient structure

    @note For AFFINE_REQUEST
*/
typedef struct {
	FLOAT fCoeffA;                  ///< Coefficient A of affine matrix
	FLOAT fCoeffB;                  ///< Coefficient B of affine matrix
	FLOAT fCoeffC;                  ///< Coefficient C of affine matrix
	FLOAT fCoeffD;                  ///< Coefficient D of affine matrix
	FLOAT fCoeffE;                  ///< Coefficient E of affine matrix
	FLOAT fCoeffF;                  ///< Coefficient F of affine matrix
} AFFINE_COEFF, *PAFFINE_COEFF;

/**
    Affine image descriptor

    @note For AFFINE_REQUEST
*/
typedef struct {
	UINT32 uiImageAddr;             ///< DRAM address of image (word alignment)
	UINT32 uiLineOffset;            ///< Lineoffset of image (word alignment)
} AFFINE_IMAGE, *PAFFINE_IMAGE;

/**
    Affine request structure

    @note For affine_request()
*/
typedef struct {
	DRV_VER_INFO verInfo;           ///< Driver version
	AFFINE_FORMAT format;           ///< format of operated images
	UINT32 uiWidth;                 ///< Image Width (unit: byte) (alignment: 16 bytes) (max: 16384)
	UINT32 uiHeight;                ///< Image Height (unit: line) (alignment: 8 lines) (max: 8192)
	PAFFINE_IMAGE pSrcImg;          ///< Source Image
	PAFFINE_IMAGE pDstImg;          ///< Destinaiton Image
	PAFFINE_COEFF pCoefficient;     ///< Affine matrix coefficients
} AFFINE_REQUEST, *PAFFINE_REQUEST;

//
// Functions exported from affine
//
extern ER affine_open(void);
extern ER affine_close(void);
extern ER affine_setConfig(AFFINE_CONFIG_ID configID, UINT32 configContext);
extern ER affine_getConfig(AFFINE_CONFIG_ID configID, UINT32 *pConfigContext);
extern ER affine_trial(PAFFINE_REQUEST pRequest);
extern ER affine_request(PAFFINE_REQUEST pRequest);

//@}

#endif

