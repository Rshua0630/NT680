/**
    Header file of vdodec library

    Exported header file of vdodec library.

    @file       dal_vdodec.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DAL_VDODEC_H
#define _DAL_VDODEC_H

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"

#define DAL_VDODEC_SHOW_MSG								1
#define DAL_VDODEC_PREDEC_FRMNUM						3

/**
    Video Decoder ID
*/
typedef enum {
	DAL_VDODEC_ID_1,                			        ///< video decoder ID 1
	DAL_VDODEC_ID_MAX,              			        ///< video decoder ID maximum
	ENUM_DUMMY4WORD(DAL_VDODEC_ID)
} DAL_VDODEC_ID;

/**
    Type of getting information from video decoding codec library.
*/
typedef enum {
	ENUM_DUMMY4WORD(DAL_VDODEC_GET_ITEM)
} DAL_VDODEC_GET_ITEM;

/**
    Type of setting information to video decoding codec library.
*/
typedef enum {
	ENUM_DUMMY4WORD(DAL_VDODEC_SET_ITEM)
} DAL_VDODEC_SET_ITEM;

/**
    Init structure
*/
typedef struct {
	UINT32 uiWidth;										///<[in] decode image width
	UINT32 uiHeight;									///<[in] decode image heght
	UINT32 uiDescAddr;									///<[in] description addr
	UINT32 uiDescSize;									///<[in] description size
	UINT32 uiDecBufAddr;								///<[in] decoder internal buffer starting address
	UINT32 uiDecBufSize;								///<[in] decoder internal buffer size
	UINT32 uiDisplayWidth;								///<[in] display source width, e.g. 720 x 480, uiWidth = 736, uiDisplayWidth = 720
} DAL_VDODEC_INIT;

/**
    Decode structure
*/
typedef struct {
	UINT32 uiBsAddr;									///<[in] bit-stream address
	UINT32 uiBsSize;									///<[in] bit-stream size
	UINT32 uiRawYAddr;									///<[in] raw y address
	UINT32 uiRawUVAddr;									///<[in] raw uv address
} DAL_VDODEC_PARAM;

#endif
