/**
    NvtNN library functions.

    Definitions of NvtNN Neural Network functions.

    @file       nvtnn.h
    @ingroup    NvtNN
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NVTNN_H
#define _NVTNN_H

#include "nvt_type.h"

// Common types ----------------------------------------------------------------
/**
    Return status.
*/
typedef enum {
	NVTNN_STA_OK            = 0,            ///< the status is OK
	NVTNN_STA_ERROR         = -1,           ///< some error occurred
	NVTNN_STA_INVALID_FUN   = -2,           ///< invalid function enum
	ENUM_DUMMY4WORD(NVTNN_STA)
} NVTNN_STA;

/**
    Data dimensions.
*/
typedef struct {
	INT32 num;                              ///< Caffe blob: batch size
	INT32 channels;                         ///< Caffe blob: number of channels
	INT32 height;                           ///< Caffe blob: height
	INT32 width;                            ///< Caffe blob: width
	INT32 lineoffset;                       ///< distance between two lines (rows); unit: byte
} NVTNN_SHAPE;

// Postprocessing: RPN ---------------------------------------------------------
typedef struct {
	INT16 *input_box_delta;                 ///< address of input data
	INT16 *input_score;
	INT16 *input_box;
	INT16 *output;                          ///< address of output data
	INT16 *work_buf;

	INT32 img_height;
	INT32 img_width;
	INT32 input_box_num;

	INT32 per_nms_top_n;
	INT32 min_score;
	INT32 nms_overlap_thres;
	INT32 max_output_box_num;
	INT32 *output_box_num;
} NVTNN_RPN_POSTPROC_PRMS;




//-----XM4_defines.h------
#define INIT_PSH_VAL    0
#define NUM_FILTER      6
#define SRC_OFFSET      8
#define COEFF_OFFSET    16
#define STEP            21
#define PATTERN_OFFSET  24
#define SW_CONFIG(init_psh,num_filter,src_offset,coeff_offset,step,pattern)     (((init_psh) & 0x3f) << INIT_PSH_VAL | ((num_filter) & 0x7) << NUM_FILTER     | ((src_offset) & 0x3f) << SRC_OFFSET | ((coeff_offset) & 0x1f) << COEFF_OFFSET | ((step) & 0x7) << STEP | ((pattern) & 0xff) << PATTERN_OFFSET )


#undef MAX
#define MAX(a,b) ((a)>(b) ? (a) : (b))

#undef MIN
#define MIN(a,b) ((a)<(b) ? (a) : (b))


#define ROUND_UP_2(a)   ((((a) + 1  )>>1)<<1)
#define ROUND_UP_4(a)   ((((a) + 3  )>>2)<<2)
#define ROUND_UP_8(a)   ((((a) + 7  )>>3)<<3)
#define ROUND_UP_16(a)  ((((a) + 15 )>>4)<<4)
#define ROUND_UP_32(a)  ((((a) + 31 )>>5)<<5)
#define ROUND_UP_64(a)  ((((a) + 63 )>>6)<<6)
#define ROUND_UP_128(a) ((((a) + 127)>>7)<<7)
//-----XM4_defines.h------

//-----Type.h------
/**
    @name Align macros

    Align floor, align round, and align ceil

    @note base must be 2^n, where n = 1, 2, 3, ...
*/
//@{
#define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))                   ///< Align Floor
#define ALIGN_ROUND(value, base)  ALIGN_FLOOR((value) + ((base)/2), base)   ///< Align Round
#define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)   ///< Align Ceil
//@}
//-----Type.h------


#define MOD(value, base)          ((value) & ((base)-1))
#define DIV_CEIL(value, base)     (((value) + ((base)-1)) / base)

#define CHAR_SIZE           sizeof(char)
#define SHORT_SIZE          sizeof(short)
#define INT_SIZE            sizeof(int)

#define NIBBLES_PER_BYTE    2
#define NIBBLES_PER_INT     8

#define BITS_PER_NIBBLE     4
#define BITS_PER_BYTE       8
#define BITS_PER_INT        32

#define ALIGN_BYTE_FROM_NIBBLE(value)   (DIV_CEIL(value, NIBBLES_PER_BYTE))
#define ALIGN_INT_FROM_NIBBLE(value)    (ALIGN_CEIL(value, NIBBLES_PER_INT) / NIBBLES_PER_BYTE)
#define ALIGN_INT(value)                (ALIGN_CEIL(value, INT_SIZE))


INT32     nvtnn_rpn_get_buf_size(INT32 box_num);
NVTNN_STA nvtnn_rpn_postproc(NVTNN_RPN_POSTPROC_PRMS *p_prms);

#endif  //_NVTNN_H
