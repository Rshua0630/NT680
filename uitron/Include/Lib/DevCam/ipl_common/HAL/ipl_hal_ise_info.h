/**
    IPH module (ISE sub function type define)

    @file       IPL_Hal_ISE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_ISE_Info_H
#define _IPL_Hal_ISE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    ise engine supported clk
*/
typedef enum _IPH_ISECLK {
	IPH_ISECLK0 = 240,  ///< 240 mhz
	IPH_ISECLK1 = 480,  ///< 480 mhz
	ENUM_DUMMY4WORD(IPH_ISECLK)
} IPH_ISECLK;


/**
    ise interrupt enable
*/
typedef enum _ISE_INTE_EN {
	ISE__INTE_CLR       =       0x00000000,  ///< disable interrupt
	ISE__INTE_FRMEND    =       0x80000000,  ///< frame-end interrupt enable
	ISE__INTE_ALL       =       0xffffffff,  ///< enable interrupt: all
} ISE_INTE_EN;

/**
    @name ISE enum - Input and output packing format selection
*/
typedef enum _ISE_IO_PACK {
	ISE_SEL_Y8 = 0,         ///< Y-8bit packing
	ISE_SEL_Y4 = 1,         ///< Y-4bit packing
	ISE_SEL_Y1 = 2,         ///< Y-1bit packing
	ISE_SEL_UVP = 3,        ///< UV packing
	ISE_SEL_RGB565 = 4,     ///< RGB565 packing
	ISE_SEL_ARGB8888 = 5,   ///< ARGB8888
	ISE_SEL_ARGB1555 = 6,   ///< ARGB1555
	ISE_SEL_ARGB4444 = 7,   ///< ARGB4444
	ISE_SEL_YUVP = 8,       ///< YUVYUV
	ENUM_DUMMY4WORD(ISE_IO_PACK)
} ISE_IO_PACK;

/**
    @name ISE enum - ARGB8888 output format
*/
typedef enum _ISE_ARGB8888_OUT_FMT {
	ISE_FMT_ARGB8888 = 0,                 ///< Output ARGB8888
	ISE_FMT_RGB8888 = 1,                  ///< Output RGB888
	ISE_FMT_A8 = 2,                       ///< Output A8
	ISE_FMT_NONE = 0xfffffffe,            ///< ignore
	ENUM_DUMMY4WORD(ISE_ARGB8888_OUT_FMT)
} ISE_ARGB8888_OUT_FMT;

/**
    @name ISE enum - Image scaling method selection
*/
typedef enum _ISE_SEL_SCALE_METHOD {
	ISE_SEL_BILINEAR        = 1,            ///< bilinear interpolation
	ISE_SEL_NEAREST         = 2,            ///< nearest interpolation
	ISE_SEL_INTERGRATION    = 3,            ///< intergration interpolation(only support scale down)
	ENUM_DUMMY4WORD(ISE_SCALE_METHOD_SEL)
} ISE_SCALE_METHOD_SEL;

/**
    @name ISE enum - Burst length selection
*/
typedef enum _ISE_BURST {
	ISE_SEL_BURST_32WORD = 32,  ///< burst 32 word
	ISE_SEL_BURST_16WORD = 16,  ///< burst 16 word
	ENUM_DUMMY4WORD(ISE_BURST)
} ISE_BURST;

/**
    @name ISE enum - Scale factor mode
*/
typedef enum _ISE_SCLFTR {
	ISE_SEL_SCLFTR_AUTO = 0,      ///< Scale factor auto mode, handled by driver
	ISE_SEL_SCLFTR_USER = 1,      ///< Scale factor auto mode, handled by user
	ENUM_DUMMY4WORD(ISE_SCLFTR)
} ISE_SCLFTR;


/**
     ISE sub function configuration.
*/
/**
    @name ISE enum - Get engine information selection
*/
typedef enum _ISE_GET_INFO {
	ISE_SEL_GET_IN_H_SIZE       = 0,   ///< get input width size
	ISE_SEL_GET_IN_V_SIZE       = 1,   ///< get input height size
	ISE_SEL_GET_IN_OFS          = 2,   ///< get input lineoffset size
	ISE_SEL_GET_IN_ADDR         = 3,   ///< get input buffer address
	ISE_SEL_GET_OUT_H_SIZE      = 4,   ///< get output width size
	ISE_SEL_GET_OUT_V_SIZE      = 5,   ///< get output height size
	ISE_SEL_GET_OUT_OFS         = 6,   ///< get output lineoffset
	ISE_SEL_GET_OUT_ADDR        = 7,   ///< get output buffer address
	ISE_SEL_GET_INTERRUPT       = 8,   ///< get interrupt status
	ISE_SEL_GET_INTERRUPT_EN    = 9,   ///< get interrupt enable
	ENUM_DUMMY4WORD(ISE_GET_INFO)
} ISE_GET_INFO;

/**
    @name ISE enum - Clear Flag
*/
typedef enum _ISE_FLAG_CLEAR {
	ISE_SEL_FLAG_NO_CLEAR = 0,        ///< No clear flag
	ISE_SEL_FLAG_CLEAR    = 1,        ///< Clear flag
	ENUM_DUMMY4WORD(ISE_CLEAR_FLAG)
} ISE_CLEAR_FLAG;

/**
    ISE structure - ISE image information
*/
typedef struct _ISE_IN {
	UINT32 uiWidth;                    ///< image width
	UINT32 uiHeight;                   ///< image height
	UINT32 uiLineoffset;               ///< lineoffset
	UINT32 uiAddr;                     ///< buffer address
} ISE_IN;

/**
    ISE structure - ISE image information
*/
typedef ISE_IN ISE_OUT;

/**
    ISE structure - ISE image pack format
*/
typedef struct _ISE_PACK_FMT {
	ISE_IO_PACK pack_fmt;               ///< input and output packing format
	ISE_ARGB8888_OUT_FMT ARGB_fmt;       ///< if packing format == ARGB8888, user can select output ARGB, A, RGB
} ISE_PACK_FMT;


/**
    ISE structure - Overlap size setting structure
*/
typedef struct _ISE_SUBFUNC {
	ISE_INTE_EN InteEn;     ///< interrupt enable

} ISE_SUBFUNC;

//@}
#endif
