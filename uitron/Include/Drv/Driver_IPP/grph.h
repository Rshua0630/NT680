/**
    Graphic module driver

    @file       grph.h
    @ingroup    mIDrvIPP_Graph
    @note
\n              -graph_open() to start this module.
\n              -graph_close() to close this module.
\n
\n              This module will have to enable and wait the system interrupt.
\n
\n              -graph_request. set parameters of images.
\n
\n
\n              Usage example:
\n
\n              (1)graph_open(GRPH_ID_1);
\n              (2)Fill request;
\n                 graph_request(GRPH_ID_1, &request);
\n                      :
\n              (3)graph_close(GRPH_ID_1);

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef   _GRPH_H
#define   _GRPH_H

#include "Driver.h"

/**
    @addtogroup mIDrvIPP_Graph
*/
//@{

/**
    Graphic channel ID

*/
typedef enum {
	GRPH_ID_1,                          ///< Graphic Controller
	GRPH_ID_2,                          ///< Graphic Controller 2

	ENUM_DUMMY4WORD(GRPH_ID)
} GRPH_ID;

/**
    Graphic configuration ID

    @note For graph_setConfig()
*/
typedef enum {
	GRPH_CONFIG_ID_FREQ,                ///< Graphic operating frequency (unit: MHz). Context can be:
	///< - @b 240: 240 MHz
	///< - @b 380: 380 MHz (PLL6, need confirm when realchip)
	///< - @b 420: 420 MHz (PLL13, need confirm when realchip)
	///< - @b 480: 480 MHz

	ENUM_DUMMY4WORD(GRPH_CONFIG_ID)
} GRPH_CONFIG_ID;
#define GRPH_CONFIG_FREQUENCY           (GRPH_CONFIG_ID_FREQ)

/**
    Geometric operation mode

    Select which Geometric operation mode for graphic engine operation.

*/
typedef enum {
	GRPH_GOP_VCOV = 8,                  ///< Video Cover

	ENUM_DUMMY4WORD(GRPH_GOP_MODE)
} GRPH_GOP_MODE;

// deprecated in NT96680
__attribute__((deprecated("NT96680 graphic does not support ROT_90, use rotate engine instead"))) const static GRPH_GOP_MODE GRPH_GOP_ROT_90 = GRPH_GOP_VCOV - 8;
__attribute__((deprecated)) const static GRPH_GOP_MODE GRPH_GOP_ROT_270 = GRPH_GOP_VCOV - 7;
__attribute__((deprecated)) const static GRPH_GOP_MODE GRPH_GOP_ROT_180 = GRPH_GOP_VCOV - 6;
__attribute__((deprecated)) const static GRPH_GOP_MODE GRPH_GOP_HRZ_FLIP = GRPH_GOP_VCOV - 5;
__attribute__((deprecated)) const static GRPH_GOP_MODE GRPH_GOP_VTC_FLIP = GRPH_GOP_VCOV - 4;
__attribute__((deprecated)) const static GRPH_GOP_MODE GRPH_GOP_HRZ_CFLT = GRPH_GOP_VCOV - 3;
__attribute__((deprecated)) const static GRPH_GOP_MODE GRPH_GOP_VTC_CFLT = GRPH_GOP_VCOV - 2;

/*
    Arithmetric operation control

    Select which Arithmetric operation control mode for graphic engine operation.

*/
typedef enum {
	GRPH_AOP_A_COPY,            //0x00        //< A -> destination
	GRPH_AOP_PLUS_SHF,          //0x01        //< (A+(B>>SHF[1:0])) -> destination
	GRPH_AOP_MINUS_SHF,         //0x02        //< (A-(B>>SHF[1:0])) -> destination
	GRPH_AOP_COLOR_EQ,          //0x03        //< A -> destination with color key (=)
	GRPH_AOP_COLOR_LE,          //0x04        //< A -> destination with color key (<)
	GRPH_AOP_A_AND_B,           //0x05        //< A & B -> destination
	GRPH_AOP_A_OR_B,            //0x06        //< A | B -> destination
	GRPH_AOP_A_XOR_B,           //0x07        //< A ^ B -> destination
	GRPH_AOP_TEXT_COPY,         //0x08        //< text -> destination
	GRPH_AOP_TEXT_AND_A,        //0x09        //< text & A -> destination
	GRPH_AOP_TEXT_OR_A,         //0x0A        //< text | A -> destination
	GRPH_AOP_TEXT_XOR_A,        //0x0B        //< text ^ A -> destination
	GRPH_AOP_TEXT_AND_AB,       //0x0C        //< (text & A) | (~text & B) -> destination
	GRPH_AOP_BLENDING,          //0x0D        //< ((A * WA) + (B * WB) + 128  >> 8 )-> destination
	GRPH_AOP_ACC,               //0x0E        //< Acc(A): pixel accumulation
	GRPH_AOP_MULTIPLY_DIV,      //0x0F        //< (A * B + (1<<DIV[2:0]-1))>>DIV[2:0] -> destination
	GRPH_AOP_PACKING,           //0x10        //< Pack(A,B)->C
	GRPH_AOP_DEPACKING,         //0x11        //< Unpack(A) ->(B,C)
	GRPH_AOP_TEXT_MUL,          //0x12        //< (A*text+(1<<SHF-1)) >> SHF -> destination
	GRPH_AOP_PLANE_BLENDING,    //0x13        //< (A*B + C*(256-B) + 128) >> 8 -> destination
	GRPH_AOP_1D_LUT = 0x16,     //0x16        //< 1D Look up table OP -> destinaion
	GRPH_AOP_2D_LUT,            //0x17        //< 2D Look up table OP -> destinaion
	GRPH_AOP_RGBYUV_BLEND,      //0x18        //< RGB blending on YUV
	GRPH_AOP_RGBYUV_COLORKEY,   //0x19        //< RGB color key on YUV


	GRPH_AOP_MINUS_SHF_ABS = 0x80, //0x80        //< abs(A-(B>>SHF[1:0])) -> destination
	GRPH_AOP_COLOR_MR,          //0x81        //< A -> destination with color key (>)

	ENUM_DUMMY4WORD(GRPH_AOP_MODE)
} GRPH_AOP_MODE;

// deprecated in NT96680
__attribute__((deprecated)) const static GRPH_AOP_MODE GRPH_AOP_ALPHA_SWITCH = GRPH_AOP_PLANE_BLENDING + 1;
__attribute__((deprecated)) const static GRPH_AOP_MODE GRPH_AOP_HWORD_TO_BYTE = GRPH_AOP_PLANE_BLENDING + 2;

/**
    Graphic operation command

    Select which operation for graphic engine operation.

    @note Used for GRPH_REQUEST.
*/
typedef enum {
	// GOP
	GRPH_CMD_VCOV = 8,                      ///< Video covering
	///< - (GRPH_ID_2)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS
	///< - Source image: GRPH_IMG_ID_A
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_GOPMAX,                        //< Max GOP count

	// AOP
	GRPH_CMD_A_COPY = 0x10000,              ///< A -> destination
	///< - (GRPH_ID_1, GRPH_ID_2)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_32BITS_ARGB8888_A, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB1555_A, GRPH_FORMAT_16BITS_ARGB4444_RGB, GRPH_FORMAT_16BITS_ARGB4444_A
	///< - GRPH_ID_1: GRPH_FORMAT_8BITS, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_32BITS_ARGB8888_A, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB1555_A, GRPH_FORMAT_16BITS_ARGB4444_RGB, GRPH_FORMAT_16BITS_ARGB4444_A
	///< - GRPH_ID_2: GRPH_FORMAT_8BITS
	///< - Source image: GRPH_IMG_ID_A
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_PLUS_SHF,                      ///< (A+(B>>SHF[1:0])) -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_PLUS_PROPTY()
	GRPH_CMD_MINUS_SHF,                     ///< (A-(B>>SHF[1:0])) -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_SUB_PROPTY()
	GRPH_CMD_COLOR_EQ,                      ///< (B == KEY) ? A:B -> destination with color key (=)
	///< - GRPH_ID_1: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB4444_RGB
	///< - GRPH_ID_2: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_32BITS_ARGB8888_RGB
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_COLOR_KEY_PROPTY()
	GRPH_CMD_COLOR_LE,                      ///< (B < KEY) ? A:B -> destination with color key (<)
	///< - (GRPH_ID_1, GRPH_ID_2)
	///< - GRPH_ID_1: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - GRPH_ID_2: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_COLOR_KEY_PROPTY()
	GRPH_CMD_A_AND_B,                       ///< A & B -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_A_OR_B,                        ///< A | B -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_A_XOR_B,                       ///< A ^ B -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_TEXT_COPY,                     ///< text -> destination
	///< - (GRPH_ID_1, GRPH_ID_2)
	///< - Format:
	///< - GRPH_ID_1: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_32BITS_ARGB8888_A, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB1555_A, GRPH_FORMAT_16BITS_ARGB4444_RGB, GRPH_FORMAT_16BITS_ARGB4444_A
	///< - GRPH_ID_2: GRPH_FORMAT_8BITS
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_TEXT_AND_A,                    ///< text & A -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_TEXT_OR_A,                     ///< text | A -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_TEXT_XOR_A,                    ///< text ^ A -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_TEXT_AND_AB,                   ///< (text & A) | (~text & B) -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_BLENDING,                      ///< ((A * WA) + (B * WB) + 128) >> 8 -> destination
	///< - (GRPH_ID_1, GRPH_ID_2)
	///< - Format:
	///< - GRPH_ID_1: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_16BITS_RGB565, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB4444_RGB
	///< - GRPH_ID_2: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_16BITS_RGB565, GRPH_FORMAT_32BITS_ARGB8888_RGB
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_BLD_WA_WB_THR()
	GRPH_CMD_ACC,                           ///< Acc(A): pixel accumulation
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS
	///< - Source image A: GRPH_IMG_ID_A
	///< - Property: Not required
	GRPH_CMD_MULTIPLY_DIV,                  ///< (A * B + 1<<(DIV[2:0]-1))>>DIV[2:0] -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_MULT_PROPTY()
	GRPH_CMD_PACKING,                       ///< Pack(A,B)->C
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_DEPACKING,                     ///< Unpack(A) ->(B,C)
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_16BITS
	///< - Source image A: GRPH_IMG_ID_A
	///< - Destination image B: GRPH_IMG_ID_B
	///< - Destination image C: GRPH_IMG_ID_C
	GRPH_CMD_TEXT_MUL,                      ///< (A*text + 1<<(SHF-1)) >> SHF -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_32BITS_ARGB8888_A, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB4444_RGB, GRPH_FORMAT_16BITS_ARGB4444_A
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_TEXT_MULT_PROPTY()
	GRPH_CMD_PLANE_BLENDING,                ///< (A*B + C*(256-B) + 128) >> 8 -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V, GRPH_FORMAT_16BITS_RGB565, GRPH_FORMAT_32BITS_ARGB8888_RGB, GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB4444_RGB
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Source image C: GRPH_IMG_ID_C
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_ALPHA_CONST()\n
	///<            GRPH_COLOR_CONST() if GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V\n
	///<            GRPH_ENABLE_ALPHA_FROM_A or GRPH_ENABLE_ALPHA_FROM_C if GRPH_FORMAT_32BITS_ARGB8888_RGB
	GRPH_CMD_1D_LUT = GRPH_CMD_PLANE_BLENDING + 3,  ///< 1D Look up table OP -> destinaion
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_2D_LUT,                        ///< 2D Look up table OP -> destinaion
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	GRPH_CMD_RGBYUV_BLEND,                  ///< RGB blending on YUV
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_16BITS_ARGB1555_RGB, GRPH_FORMAT_16BITS_ARGB4444_RGB, GRPH_FORMAT_16BITS_RGB565
	///< - Source image A: GRPH_IMG_ID_A (ARGB)
	///< - Source/Destination image B: GRPH_IMG_ID_B (Y)
	///< - Source/Destination image: GRPH_IMG_ID_C (UV pack)
	GRPH_CMD_RGBYUV_COLORKEY,               ///< RGB color key on YUV
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_16BITS_RGB565
	///< - Source image A: GRPH_IMG_ID_A (RGB)
	///< - Source/Destination image B: GRPH_IMG_ID_B (Y)
	///< - Source/Destination image: GRPH_IMG_ID_C (UV pack)
	GRPH_CMD_AOPMAX,                        //< Max AOP count


	GRPH_CMD_MINUS_SHF_ABS = 0x10000 + 0x80, ///< abs(A-(B>>SHF[1:0])) -> destination
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_SUB_PROPTY()
	GRPH_CMD_COLOR_MR,                      ///< (B >= KEY) ? A:B -> destination with color key (>)
	///< - GRPH_ID_1: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - GRPH_ID_2: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK, GRPH_FORMAT_16BITS_UVPACK_U, GRPH_FORMAT_16BITS_UVPACK_V
	///< - Source image A: GRPH_IMG_ID_A
	///< - Source image B: GRPH_IMG_ID_B
	///< - Destination image: GRPH_IMG_ID_C
	///< - Property: GRPH_COLOR_KEY_PROPTY()
	GRPH_CMD_COLOR_FILTER,                  ///< 3 channel color key filter
	///< - (GRPH_ID_1 only)
	///< - Format: GRPH_FORMAT_8BITS, GRPH_FORMAT_16BITS_UVPACK
	///< - When format is GRPH_FORMAT_8BITS, Source GRPH_IMG_ID_A/GRPH_IMG_ID_B/GRPH_IMG_ID_C, Destination: GRPH_IMG_ID_A/GRPH_IMG_ID_B/GRPH_IMG_ID_C.
	///< - When format is GRPH_FORMAT_16BITS_UVPACK, Source GRPH_IMG_ID_A/GRPH_IMG_ID_B, Destination: GRPH_IMG_ID_A/GRPH_IMG_ID_B.
	///< - Property: GRPH_COLOR_FLT_PROPTY()

	ENUM_DUMMY4WORD(GRPH_CMD)
} GRPH_CMD;

// Deprecated in NT96680
const static GRPH_CMD GRPH_CMD_ROT_90 = GRPH_CMD_VCOV - 8;
const static GRPH_CMD GRPH_CMD_ROT_270 = GRPH_CMD_VCOV - 7;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_ROT_180 = GRPH_CMD_VCOV - 6;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_HRZ_FLIP = GRPH_CMD_VCOV - 5;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_VTC_FLIP = GRPH_CMD_VCOV - 4;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_HRZ_CFLT = GRPH_CMD_VCOV - 3;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_VTC_CFLT = GRPH_CMD_VCOV - 2;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_ALPHA_SWITCH = GRPH_CMD_PLANE_BLENDING + 1;
__attribute__((deprecated)) const static GRPH_CMD GRPH_CMD_HWORD_TO_BYTE = GRPH_CMD_PLANE_BLENDING + 2;

/**
    Graphic data format

    Select data format of graphic operation.

    @note For GRPH_REQUEST
*/
typedef enum {
	GRPH_FORMAT_4BITS,                      ///< Normal 4 bits data
	GRPH_FORMAT_8BITS,                      ///< Normal 8 bits data
	GRPH_FORMAT_16BITS,                     ///< Normal 16 bits data
	GRPH_FORMAT_16BITS_UVPACK,              ///< 16 bits U/V pack data and apply to both plane
	GRPH_FORMAT_16BITS_UVPACK_U,            ///< 16 bits U/V pack data and apply to U plane only
	GRPH_FORMAT_16BITS_UVPACK_V,            ///< 16 bits U/V pack data and apply to V plane only
	GRPH_FORMAT_16BITS_RGB565,              ///< 16 bits RGB565 pack data and apply to all plane
	GRPH_FORMAT_32BITS,                     ///< Normal 32 bits data
	GRPH_FORMAT_32BITS_ARGB8888_RGB,        ///< 32 bits ARGB8888 pack data and apply to RGB plane only
	GRPH_FORMAT_32BITS_ARGB8888_A,          ///< 32 bits ARGB8888 pack data and apply to A plane only
	GRPH_FORMAT_16BITS_ARGB1555_RGB,        ///< 16 bits ARGB1555 pack data and apply to RGB only
	GRPH_FORMAT_16BITS_ARGB1555_A,          ///< 16 bits ARGB1555 pack data and apply to A(alpha) only
	GRPH_FORMAT_16BITS_ARGB4444_RGB,        ///< 16 bits ARGB4444 pack data and apply to RGB only
	GRPH_FORMAT_16BITS_ARGB4444_A,          ///< 16 bits ARGB4444 pack data and apply to A(alpha) only

	ENUM_DUMMY4WORD(GRPH_FORMAT)
} GRPH_FORMAT;

/**
    Graphic Image identifier

    Select Image A/B/C.

    @note For GRPH_IMG
*/
typedef enum {
	GRPH_IMG_ID_A,                          ///< Image A
	GRPH_IMG_ID_B,                          ///< Image B
	GRPH_IMG_ID_C,                          ///< Image C

	ENUM_DUMMY4WORD(GRPH_IMG_ID)
} GRPH_IMG_ID;

/**
    Graphic Image YUV format

    Select YUV422 or YUV411

    @note For GRPH_PROPERTY_ID_YUVFMT
*/
typedef enum {
	GRPH_YUV_422,                           ///< YUV 422
	GRPH_YUV_411,                           ///< YUV 411

	ENUM_DUMMY4WORD(GRPH_YUV_FMT)
} GRPH_YUV_FMT;

/**
    Graphic command property identifier

    Select property for which plane.

    @note For GRPH_PROPERTY
*/
typedef enum {
	GRPH_PROPERTY_ID_NORMAL,                ///< property for normal plane
	GRPH_PROPERTY_ID_U,                     ///< property for U plane
	GRPH_PROPERTY_ID_V,                     ///< property for V plane
	GRPH_PROPERTY_ID_R,                     ///< property for R plane
	GRPH_PROPERTY_ID_G,                     ///< property for G plane
	GRPH_PROPERTY_ID_B,                     ///< property for B plane
	GRPH_PROPERTY_ID_A,                     ///< property for A(alpha) plane

	GRPH_PROPERTY_ID_ACC_SKIPCTRL,          ///< (OBSOLETE) Skip control for GRPH_CMD_ACC command. uiProperty can be any of GRPH_ACC_SKIP_CTL.
	GRPH_PROPERTY_ID_ACC_FULL_FLAG,         ///< (OBSOLETE) full flag for GRPH_CMD_ACC command. uiProperty will  return full flag of accumulation operation.
	GRPH_PROPERTY_ID_PIXEL_CNT,             ///< (OBSOLETE) pixel count for GRPH_CMD_ACC command. uiProperty will return pixel count of accumulation operation
	GRPH_PROPERTY_ID_VALID_PIXEL_CNT,       ///< (OBSOLETE) valid pixel count for GRPH_CMD_ACC command
	GRPH_PROPERTY_ID_ACC_RESULT,            ///< acc result for GRPH_CMD_ACC command

	GRPH_PROPERTY_ID_LUT_BUF,               ///< Buffer stores LUT table referenced by GRPH_CMD_1D_LUT and GRPH_CMD_2D_LUT (word alignment).

	GRPH_PROPERTY_ID_YUVFMT,                ///< YUV format (for RGB + YUV operations)
	GRPH_PROPERTY_ID_ALPHA0_INDEX,          ///< For GRPH_CMD_RGBYUV_BLEND. Index (0~15) value when alpha (of ARGB1555) is 0
	GRPH_PROPERTY_ID_ALPHA1_INDEX,          ///< For GRPH_CMD_RGBYUV_BLEND. Index (0~15) value when alpha (of ARGB1555) is 1

	GRPH_PROPERTY_ID_QUAD_PTR,              ///< This property is a pointer to quadrilateral descriptor (GRPH_QUAD_DESC)

	ENUM_DUMMY4WORD(GRPH_PROPERTY_ID)
} GRPH_PROPERTY_ID;

/**
    Graphic IN/OUT operation identifier

    Select In/OUT operation and select operated plane.

    @note For GRPH_INOUTOP
*/
typedef enum {
	GRPH_INOUT_ID_IN_A,                     ///< IN operation on image A
	GRPH_INOUT_ID_IN_A_U,                   ///< IN operation on U plane of UV packed image A
	GRPH_INOUT_ID_IN_A_V,                   ///< IN operation on V plane of UV packed image A

	GRPH_INOUT_ID_IN_B,                     ///< IN operation on image B
	GRPH_INOUT_ID_IN_B_U,                   ///< IN operation on U plane of UV packed image B
	GRPH_INOUT_ID_IN_B_V,                   ///< IN operation on V plane of UV packed image B

	GRPH_INOUT_ID_OUT_C,                    ///< OUT operation on image C
	GRPH_INOUT_ID_OUT_C_U,                  ///< OUT operation on U plane of UV packed image C
	GRPH_INOUT_ID_OUT_C_V,                  ///< OUT operation on V plane of UV packed image C

	ENUM_DUMMY4WORD(GRPH_INOUT_ID)
} GRPH_INOUT_ID;

/**
    Color key filter select

    @note For GRPH_COLOR_FLT_PROPTY
*/
typedef enum {
	GRPH_COLORKEY_FLTSEL_0,                     ///< filter by FKEY1 <  Pixel_Value <  FKEY2
	GRPH_COLORKEY_FLTSEL_1,                     ///< filter by FKEY1 <= Pixel_Value <  FKEY2
	GRPH_COLORKEY_FLTSEL_2,                     ///< filter by FKEY1 <  Pixel_Value <= FKEY2

	ENUM_DUMMY4WORD(GRPH_COLORKEY_FLTSEL)
} GRPH_COLORKEY_FLTSEL;

/**
    Color key filter mode

    @note For GRPH_COLOR_FLT_PROPTY
*/
typedef enum {
	GRPH_COLORKEY_FLTMODE_BOTH,                 ///< Y1/U/V and Y2/U/V both match
	GRPH_COLORKEY_FLTMODE_OR,                   ///< Y1/U/V or Y2/U/V match
	GRPH_COLORKEY_FLTMODE_1ST,                  ///< Y1/U/V match
	GRPH_COLORKEY_FLTMODE_2ND,                  ///< Y2/U/V match

	ENUM_DUMMY4WORD(GRPH_COLORKEY_FLTMODE)
} GRPH_COLORKEY_FLTMODE;

/**
    @name PLUS Operation Property

    Generate property value for plus operation (GRPH_CMD_PLUS_SHF).

    @param[in] SHF      Right shift value after plus. valid value: 0~3

    @note For property parameter work with aopMode = GRPH_CMD_PLUS_SHF. \n
            Only valid with 16 bits (non-pack) operation.
*/
//@{
#define GRPH_PLUS_PROPTY(SHF)   ((SHF)&0x3)         ///< GRPH_CMD_PLUS_SHF property
//@}

/**
    @name SUB Operation Property

    Generate property value for sub operation (GRPH_CMD_MINUS_SHF/GRPH_CMD_MINUS_SHF_ABS).

    @param[in] SHF      Right shift value after plus. valid value: 0~3

    @note For property parameter work with aopMode = GRPH_CMD_MINUS_SHF or GRPH_CMD_MINUS_SHF_ABS. \n
            Only valid with 16 bits (non-pack) operation.
*/
//@{
#define GRPH_SUB_PROPTY(SHF)    ((SHF)&0x3)         ///< GRPH_CMD_MINUS_SHF/GRPH_CMD_MINUS_SHF_ABS property
//@}

/**
    @name Color Key Operation Property

    Generate property value for color key operation (GRPH_CMD_COLOR_EQ/GRPH_CMD_COLOR_LE/GRPH_CMD_COLOR_MR).

    @param[in] COLOR    Color key value. valid value: 0~FF

    @note For property parameter work with aopMode = GRPH_CMD_COLOR_EQ or GRPH_CMD_COLOR_LE or GRPH_CMD_COLOR_MR. \n
*/
//@{
#define GRPH_COLOR_KEY_PROPTY(COLOR) ((COLOR)&0xFF)                  ///< GRPH_CMD_COLOR_EQ/GRPH_CMD_COLOR_LE/GRPH_CMD_COLOR_MR property
//@}

/**
    @name Color Key Filter Operation Property

    Generate property value for color key filter operation (GRPH_CMD_COLOR_FILTER).
    This property MACRO should be used when you need to apply color key filter on YUV image simultaneously.

    @param[in] FLT_SEL  Filter select
                        - @b GRPH_COLORKEY_FLTSEL_0: filter by FKEY1 < Pixel_Value < FKEY2
                        - @b GRPH_COLORKEY_FLTSEL_1: filter by FKEY1 <= Pixel_Value < FKEY2
                        - @b GRPH_COLORKEY_FLTSEL_2: filter by FKEY1 < Pixel_Value <= FKEY2
    @param[in] FLT_MODE Filter mode
                        - @b GRPH_COLORKEY_FLTMODE_BOTH: Y1/U/V and Y2/U/V both match
                        - @b GRPH_COLORKEY_FLTMODE_OR: Y1/U/V or Y2/U/V match
                        - @b GRPH_COLORKEY_FLTMODE_1ST: Y1/U/V match
                        - @b GRPH_COLORKEY_FLTMODE_2ND: Y2/U/V match
    @param[in] COLORKEY Color key value. From LSB to MSB is Y, U, V

    @note For property parameter work with aopMode = GRPH_CMD_COLOR_FILTER.
*/
//@{
#define GRPH_COLOR_FLT_PROPTY(FLT_SEL, FLT_MODE, COLORKEY) ((((FLT_SEL)&0x3)<<30) | 0x20000000 | (((FLT_MODE)&0x3)<<26) | ((COLORKEY)&0xFFFFFF))    ///< GRPH_CMD_COLOR_LE/GRPH_CMD_COLOR_MR property
//@}



/**
    @name Advanced blending control

    Select which blending control weighting & blending threshold for graphic engine operation.

    @param[in] WA       Blending weight of image A. valid value: 0~255
    @param[in] WB       Blending weight of image B. valid value: 0~255
    @param[in] THR      (OBSOLETE) Don't care by graphic driver

    @note For property parameter in graph_request() work with aopMode = GRPH_CMD_BLENDING.
*/
//@{
#define GRPH_BLD_WA_WB_THR(WA,WB,THR)   (((WA)&0xFF)|(((WB)&0xFF)<<8)|(((THR)&0xFF)<<16))  ///< Weighting is (WA/256) & (WB/256). THR is don't care field
//@}

/**
    @name Accumulation Operation Property (OBSOLETE)

    Generate property value for accumulation operation (GRPH_CMD_ACC).

    @note NT96660 only support checksum mode (i.e. chksumMode = TRUE) . This property is not required.

    @param[in] chksumMode   Checksum mode select
                            - @b FALSE: Disable checksum mode. i.e. accumulation mode
                            - @b TRUE: Enable checksum mode
    @param[in] THR          Threshold value for accumulation mode. (Valid when chksumMode==FALSE)
                            Accumulation is only applied when pixel value >= THR.
                            Valid value: 0~0xFFF

    @note For property parameter work with aopMode = GRPH_CMD_ACC.
*/
//@{
#define GRPH_ACC_PROPTY(chksumMode, THR) ((((chksumMode)&0x1)<<31) | ((THR)&0xFFF))     ///< GRPH_CMD_ACC property
//@}

/**
    @name Multiplication Operation Property

    Generate property value for multiplication operation (GRPH_CMD_MULTIPLY_DIV).

    @param[in] ditherEn     Select dithering enable
                            - @b FALSE: Disable dithering after multiplication
                            - @b TRUE: Enable dithering after multiplication
    @param[in] SQ1          SQ1 for dithering (valid when ditherEn==TRUE). Valid value: 0x0~0xF
    @param[in] SQ2          SQ2 for dithering (valid when ditherEn==TRUE). Valid value: 0x0~0x7FFF
    @param[in] SHF          Right shift value after multiplication. Valid value: 0x0~0xF

    @note For property parameter work with aopMode = GRPH_CMD_MULTIPLY_DIV.
*/
//@{
#define GRPH_MULT_PROPTY(ditherEn, SQ1, SQ2, SHF)   ((((ditherEn)&0x1)<<31) | (((SQ1)&0xF)<<24) | (((SQ2)&0x7FFF)<<8) | ((SHF)&0xF))    ///< GRPH_CMD_MULTIPLY_DIV property
//@}

/**
    @name Text MUL control

    Text MUL control for GRPH_CMD_TEXT_MUL

    - Byte Mode: C[7..0] = A*Text[11..4] >> SHF[3..0]
                -# A and C support IN/OUT operation respectively.
                -# Text supports signed/unsigned value.
    - (OBSOLETE) HWORD Mode: C[15..0] = (A*Text[11..4] >> SHF[3..0]) + B[15..0]
                -# A support IN operation.
                -# C[15..0] & B[15..0] are signed value.
                -# Text supports signed/unsigned value.
*/
//@{
#define GRPH_TEXTMUL_BYTE         0x00000000    ///< Byte Mode. (Exclusive use with GRPH_TEXTMUL_HWORD)
#define GRPH_TEXTMUL_HWORD        0x80000000    ///< (OBSOLETE) Half-word Mode. (Exclusive use with GRPH_TEXTMUL_BYTE)

#define GRPH_TEXTMUL_SIGNED       0x40000000    ///< Signed Multiply (Exclusive use with GRPH_TEXTMUL_UNSIGNED)
#define GRPH_TEXTMUL_UNSIGNED     0x00000000    ///< Un-signed Multiply (Exclusive use with GRPH_TEXTMUL_SIGNED)
//@}

/**
    @name Text Multiplication Operation Property

    Generate property value for text multiplication operation (GRPH_CMD_TEXT_MUL).

    @param[in] outPrecision Select output precision (Only support byte mode)
                            - @b GRPH_TEXTMUL_BYTE: Output 8 bits data (Byte mode)
    @param[in] signedOp     Select signed of CTEX
                            - @b GRPH_TEXTMUL_SIGNED: CTEX is signed value
                            - @b GRPH_TEXTMUL_UNSIGNED: CTEX is unsigned value
    @param[in] CTEX         Constant value to multiply with pixels. Valid value: 0x00~0xFF
    @param[in] SHF          Right shift value after multiplication. Valid value: 0x0~0xF

    @note For property parameter work with aopMode = GRPH_CMD_TEXT_MUL.
*/
//@{
#define GRPH_TEXT_MULT_PROPTY(outPrecision, signedOp, CTEX, SHF)  (((outPrecision)&GRPH_TEXTMUL_HWORD) | ((signedOp)&GRPH_TEXTMUL_SIGNED) | (((CTEX)&0xFF)<<4) | ((SHF)&0xF))    ///< GRPH_CMD_TEXT_MUL property
//@}

/**
    @name Plane-Blending

    Plane-Blending for GRPH_CMD_PLANE_BLENDING

    Select the operation mode for plane blending operation.
*/
//@{
#define GRPH_ENABLE_COLOR_CONST     0x10000     ///< Enable constant color source instead of image A.
///< If constant color is enabled, GRPH_COLOR_CONST() will decide color source.
#define GRPH_ENABLE_ALPHA_CONST     0x20000     ///< Enable constant alpha source instead of image B.
///< If constant alpha is enabled, GRPH_ALPHA_CONST() will decide color source.
#define GRPH_ENABLE_ALPHA_SRC       0x40000     ///< Select alpha from image C (for GRPH_FORMAT_32BITS_ARGB8888_RGB)
///< Else default will select alpha from image A
#define GRPH_ENABLE_ALPHA_FROM_C    0x40000     ///< Select alpha from image C (for GRPH_FORMAT_32BITS_ARGB8888_RGB)
#define GRPH_ENABLE_ALPHA_FROM_A    0x00000     ///< Select alpha from image A (for GRPH_FORMAT_32BITS_ARGB8888_RGB)
#define GRPH_COLOR_ALPHA_MASK       (GRPH_ENABLE_COLOR_CONST|GRPH_ENABLE_ALPHA_CONST)   ///< Bit mask to filter GRPH_ENABLE_COLOR_CONST and GRPH_ENABLE_ALPHA_CONST

#define GRPH_COLOR_CONST(x)         ((((x) & 0xFF) <<  0) | GRPH_ENABLE_COLOR_CONST)///< Constant color when GRPH_ENABLE_COLOR_CONST is enabled
#define GRPH_ALPHA_CONST(x)         ((((x) & 0xFF) <<  8) | GRPH_ENABLE_ALPHA_CONST)///< Constant alpha when GRPH_ENABLE_ALPHA_CONST is enabled
#define GRPH_CONST_MASK             0x0000FFFF          ///< Bit mask to filter constant color/alpha
//@}

/**
    @name (OBSOLETE) Half-Word to Byte control

    Half-Word to Byte control for GRPH_CMD_HWORD_TO_BYTE

    Select the operation mode of signed/unsigned control for HWORD-to-BYTE control operation.

    @note Related operation is obsoleted
*/
//@{
#define GRPH_HWORD_TO_BYTE_SIGNED       0x00000000      ///< Signed half-word to unsigned byte
#define GRPH_HWORD_TO_BYTE_UNSIGNED     0x80000000      ///< Unsigned half-word to unsigned byte
//@}

/**
    @name (OBSOLETE) Half-Word to Byte Operation Property

    Generate property value for half-word to byte operation (GRPH_CMD_HWORD_TO_BYTE).

    @param[in] signedOp     Select signed of CTEX
                            - @b GRPH_HWORD_TO_BYTE_SIGNED: input image is signed
                            - @b GRPH_HWORD_TO_BYTE_UNSIGNED: input image is unsigned
    @param[in] SHF          Right shift value before hword to byte translation. Valid value: 0x0~0xF

    @note Related operation is obsoleted
*/
//@{
#define GRPH_HWORD2BYTE_PROPTY(signedOp, SHF)  (((signedOp)&GRPH_HWORD_TO_BYTE_UNSIGNED) | ((SHF)&0xF))     ///< GRPH_CMD_HWORD_TO_BYTE property
//@}

/**
    Accumulation control

    (OBSOLETE)

    Accumulation skip control

    @note used for GRPH_PROPERTY_ID_ACC_SKIPCTRL.
*/
typedef enum {
	GRPH_ACC_SKIP_NONE,                 ///< Accumulation none skip.
	GRPH_ACC_SKIP_ODD,                  ///< Accumulation odd skip.
	GRPH_ACC_SKIP_EVEN,                 ///< Accumulation even skip.

	ENUM_DUMMY4WORD(GRPH_ACC_SKIP_CTL)
} GRPH_ACC_SKIP_CTL __attribute__((deprecated));



/**
    INPUT operation control

    Definition for the INPUT operation control.

    @note For GRPH_INOUTOP.
*/
typedef enum {
	GRPH_INOP_NONE,         //0x00      ///< None:              A -> A
	GRPH_INOP_INVERT,       //0x01      ///< Invert:            A -> ~A
	GRPH_INOP_SHIFTR_ADD,   //0x02      ///< Shift right & Add: A -> ((A>>SHF[3..0]) + Constant)
	GRPH_INOP_SHIFTL_ADD,   //0x03      ///< Shift left & Add:  A -> ((A<<SHF[3..0]) + Constant)
	GRPH_INOP_SHIFTL_SUB,   //0x04      ///< Shift left & Sub:  A -> ((A<<SHF[3..0]) - Constant)

	ENUM_DUMMY4WORD(GRPH_INOPCTL)
} GRPH_INOPCTL;

/**
    OUTPUT operation control

    Definition for the OUTPUT operation control.

    @note For GRPH_INOUTOP.
*/
typedef enum {
	GRPH_OUTOP_SHF,                 ///< Shift:         C = (C >> SHF[3..0])
	GRPH_OUTOP_INVERT,              ///< Invert:        C = ~C
	GRPH_OUTOP_ADD,                 ///< Add:           C = C + Constant
	GRPH_OUTOP_SUB,                 ///< Sub:           C = C - Constant
	GRPH_OUTOP_ABS,                 ///< Absolute       C = ABS(C)

	ENUM_DUMMY4WORD(GRPH_OUTOPCTL)
} GRPH_OUTOPCTL;

/**
    Graphic INPUT/OUTPUT operation descriptor

    Structure for the INPUT/OUTPUT operation.

    @note For GRPH_IMG.
*/
typedef struct GRPH_INOUTOP {
	GRPH_INOUT_ID   id;             ///< Identifier for input operation
	UINT32          opCtl;          ///< Input/Output Operation Control. Can be:
	///< - @b GRPH_INOPCTL: if id indicates an IN operation
	///< - @b GRPH_OUTOPCTL: if id indicates an OUT operation
	UINT32          uiSHF;          ///< Shift Value for Operation.
	UINT32          uiConst;        ///< Constant value for Operation

	struct GRPH_INOUTOP *pNext;     ///< Link to next IN/OUT OP. (Fill NULL if this is last item)
} GRPH_INOUTOP, *PGRPH_INOUTOP;


/**
    Graphic image structure

    Description of graphic image

    @note For GRPH_REQUEST
*/
typedef struct GRPH_IMG {
	GRPH_IMG_ID imgID;              ///< Image identifier
	UINT32 uiAddress;               ///< Image DRAM address (unit: byte)
	UINT32 uiLineoffset;            ///< Image lineoffset (unit: byte) (Must be 4 byte alignment)
	UINT32 uiWidth;                 ///< Image width (unit: byte)
	UINT32 uiHeight;                ///< Image height (unit: line)
	PGRPH_INOUTOP pInOutOP;         ///< List of IN/OUT operation for this image (NULL if this is not required)

	struct GRPH_IMG *pNext;         ///< Link to next image. (Fill NULL if this is last item)
} GRPH_IMG, *PGRPH_IMG;

/**
    Graphic command property description

    Description property value for each command

    @note For GRPH_REQUEST
*/
typedef struct GRPH_PROPERTY {
	GRPH_PROPERTY_ID id;            ///< Property identifier
	UINT32 uiProperty;              ///< Property value

	struct GRPH_PROPERTY *pNext;    ///< Link to next property description. (Fill NULL if this is last item)
} GRPH_PROPERTY, *PGRPH_PROPERTY;

/**
    ColorKey filter control

    Definition for the color key filter control.

    @note For GRPH_REQUEST
*/
typedef struct {
	UINT32   FKEY1;         ///< color key filter key 1
	UINT32   FKEY2;         ///< color key filter key 2
	UINT32   RKEY;          ///< color key filter replace key
} GRPH_CKEYFILTER, *PGRPH_CKEYFILTER;

/**
    Graphic request structure

    Description of graphic request

    @note For graph_request()
*/
typedef struct {
	DRV_VER_INFO verInfo;           ///< Driver version
	GRPH_CMD command;               ///< Graphic command
	GRPH_FORMAT format;             ///< format of operated images
	PGRPH_IMG pImageDescript;       ///< Descriptions of images for command
	PGRPH_PROPERTY pProperty;       ///< Descriptions of properties
	PGRPH_CKEYFILTER pCkeyFilter;   ///< ColorKey filter control info. Valid when command is GRPH_AOP_COLOR_LE or GRPH_AOP_COLOR_MR and uiPropertyU is GRPH_COLOR_FLT_PROPTY
} GRPH_REQUEST, *PGRPH_REQUEST;


/**
    Point Descriptor

    @note For GRPH_QUAD_DESC
*/
typedef struct {
	INT32   x;          ///< x coordinator
	INT32   y;          ///< y coordinator

	struct GRPH_POINT_DESC *pNext;
} GRPH_POINT_DESC, *PGRPH_POINT_DESC;


/**
    Quadrilateral Descriptor

    The described quadrilateral should be inside a 4k * 4K region

    If uiMosaicWidth and uiMosiacHeight are not zero, the region will be filled with mosaic effect.
    Else the region will be filled with uiConst blending with background image (image A)

    @note For GRPH_PROPERTY_ID_QUAD_PTR
*/
typedef struct {
	BOOL   bBlendEn;                ///< enable/disable blend with background color
	///< - @b TRUE: enable blending
	///< - @b FALSE: disable blending
	UINT32 uiAlpha;                 ///< alpha value for blending

	UINT32 uiMosaicWidth;           ///< mosaic width (unit: byte)
	UINT32 uiMosaicHeight;          ///< mosaic height (unit: line)

	GRPH_POINT_DESC pointTopLeft;   ///< point 0 (left/top)
	GRPH_POINT_DESC pointTopRight;  ///< point 1 (right/top)
	GRPH_POINT_DESC pointBottomRight;   ///< point 2 (right/bottom)
	GRPH_POINT_DESC pointBottomLeft;    ///< point 3 (left/bottom)

	struct GRPH_QUAD_DESC *pNext;
} GRPH_QUAD_DESC, *PGRPH_QUAD_DESC;



// Functions exported from graphic driver
extern ER       graph_open(GRPH_ID id);
extern BOOL     graph_isOpened(GRPH_ID id);
extern ER       graph_close(GRPH_ID id);

extern ER       graph_setConfig(GRPH_ID grphID, GRPH_CONFIG_ID configID, UINT32 configContext);
extern ER       graph_getConfig(GRPH_ID grphID, GRPH_CONFIG_ID configID, UINT32 *pConfigContext);

extern ER       graph_request(GRPH_ID id, PGRPH_REQUEST pRequest);

//@}

#endif
