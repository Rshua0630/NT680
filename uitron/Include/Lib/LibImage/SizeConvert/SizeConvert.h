/**
    Size convert module.

    Input/output size calculation accroding to desired width/height ratio.

    @file       SizeConvert.h

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _SIZECONVERT_H
#define _SIZECONVERT_H


typedef enum {
	SIZECONVERT_ALIGN_FLOOR_0   = 0xFFFFFFFF,   ///< no alignment
	SIZECONVERT_ALIGN_FLOOR_2   = 0xFFFFFFFE,   ///< align down to 2
	SIZECONVERT_ALIGN_FLOOR_4   = 0xFFFFFFFC,   ///< align down to 4
	SIZECONVERT_ALIGN_FLOOR_8   = 0xFFFFFFF8,   ///< align down to 8
	SIZECONVERT_ALIGN_FLOOR_16  = 0xFFFFFFF0,   ///< align down to 16
	SIZECONVERT_ALIGN_FLOOR_32  = 0xFFFFFFE0,   ///< align down to 32
	SIZECONVERT_ALIGN_FLOOR_64  = 0xFFFFFFC0,   ///< align down to 64
	ENUM_DUMMY4WORD(SIZECONVERT_ALIGH_TYPE)
} SIZECONVERT_ALIGH_TYPE;

/**
    Structure definition for size convert usage.
*/
typedef struct {
	UINT32 uiSrcWidth;      ///< [in] src width (ex. 1280x720)
	UINT32 uiSrcHeight;     ///< [in] src height
	UINT32 uiDstWidth;      ///< [in] dst width (ex. 640x240)
	UINT32 uiDstHeight;     ///< [in] dst height
	UINT32 uiDstWRatio;     ///< [in] dst wratio (ex. 4:3)
	UINT32 uiDstHRatio;     ///< [in] dst hratio
	SIZECONVERT_ALIGH_TYPE alignType; ///< [in] alignment type, SIZECONVERT_ALIGN_FLOOR_32 or others

	UINT32 uiOutWidth;      ///< [out] out width (ex. 640)
	UINT32 uiOutHeight;     ///< [out] out height (ex. 180)
	UINT32 uiOutX;          ///< [out] x offset 0
	UINT32 uiOutY;          ///< [out] y offset (240-180)/2 = 30
} SIZECONVERT_INFO;

extern void DisplaySizeConvert(SIZECONVERT_INFO *pScinfo);
extern void DisplaySizeDeform(SIZECONVERT_INFO *pScinfo);

#endif
