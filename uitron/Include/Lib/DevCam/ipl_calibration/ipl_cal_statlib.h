/**
    IPL calibration statistics lib.

    @file       ipl_cal_statlib.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_STATLIB_H_
#define _IPL_CAL_STATLIB_H_
#include "Type.h"
#include "ErrorNo.h"
#include "ipl_calibration.h"

#define CAL_MAX_CA_WIN (65*65)
#define CAL_MAX_ECS_WINH 65
#define CAL_MAX_ECS_WINV 65

typedef enum {
	CAL_ALG_RGB_CH_R    = 0,
	CAL_ALG_RGB_CH_G    = 1,
	CAL_ALG_RGB_CH_B    = 2,
	CAL_ALG_RGB_CH_MAX  = 3,
	ENUM_DUMMY4WORD(CAL_ALG_RGB_CH)
} CAL_ALG_RGB_CH;

typedef enum {
	CAL_STAT_TYPE_PLANAR = 0,
	CAL_STAT_TYPE_BAYER_PROGRESSIVE,
	ENUM_DUMMY4WORD(CAL_STAT_TYPE)
} CAL_STAT_TYPE;

typedef enum {
	CAL_STAT_BIT_8 = 0,
	CAL_STAT_BIT_10,
	CAL_STAT_BIT_12,
	CAL_STAT_BIT_16,
	ENUM_DUMMY4WORD(CAL_STAT_BIT)
} CAL_STAT_BIT;

typedef enum {
	CAL_STAT_STPIX_R = 0,
	CAL_STAT_STPIX_GR = 1,
	CAL_STAT_STPIX_GB = 2,
	CAL_STAT_STPIX_B = 3,

	CAL_STAT_STPIX_RGBIR_RIR = 100,       ///< pattern RIR
	CAL_STAT_STPIX_RGBIR_RG = 101,        ///< pattern RG

	// RCCB start pixel
	CAL_STAT_STPIX_RCCB_RC = 200,         ///< start pixel RC
	CAL_STAT_STPIX_RCCB_CR = 201,         ///< start pixel CR
	CAL_STAT_STPIX_RCCB_CB = 202,         ///< start pixel CB
	CAL_STAT_STPIX_RCCB_BC = 203,         ///< start pixel BC

	ENUM_DUMMY4WORD(CAL_STAT_STPIX)
} CAL_STAT_STPIX;

typedef struct {
	CAL_STAT_TYPE type;      ///< image type
	UINT32 addr;            ///< image addr
	UINT32 h_size;           ///< image H Size
	UINT32 v_size;           ///< image V Size
	UINT32 line_ofs;         ///< image line offset
	CAL_STAT_STPIX stpix;    ///< start pixel, only used on bayer format
	CAL_STAT_BIT bit;        ///< bit number, only used on bayer format
	UINT32 sub_rate;         ///< sub sample rate
	UINT32 win_h_num;         ///< CA window H number
	UINT32 win_v_num;         ///< CA window V number
	UINT32 bin_num;          ///< histogram bin number
} CAL_STAT_CA_INFO_IN;

typedef struct {
	UINT32 *r_data;          ///< the array pointer, index must be equal WinHNum x WinVNum
	UINT32 *g_data;          ///< the array pointer, index must be equal WinHNum x WinVNum
	UINT32 *b_data;          ///< the array pointer, index must be equal WinHNum x WinVNum
} CAL_STAT_CA_INFO_OUT;

typedef struct {
	UINT32 addr;            ///< image addr
	UINT32 h_start;           ///< image H start
	UINT32 v_start;           ///< image V start
	UINT32 h_size;           ///< image H Size
	UINT32 v_size;           ///< image V Size
	UINT32 line_ofs;         ///< image line offset
	CAL_STAT_STPIX stpix;    ///< start pixel, only used on bayer format
	UINT32 sub_rate;         ///< sub sample rate
} CAL_STAT_RGB_INFO_IN;

typedef struct {
	UINT32 *r_data;          ///< the array pointer
	UINT32 *g_data;          ///< the array pointer
	UINT32 *b_data;          ///< the array pointer
} CAL_STAT_RGB_INFO_OUT;

typedef struct {
	UINT32 *y_data;          ///< the array pointer, index must be equal WinHNum x WinVNum
} CAL_STAT_Y_INFO;

typedef struct {
	UINT32 *hist_data;       ///< the array pointer, index must be equal BinNum
} CAL_STAT_HISTO_INFO;

typedef struct {
	void(*fp_raw2ca)(CAL_STAT_CA_INFO_IN *in, CAL_STAT_CA_INFO_OUT *ca_out, CAL_STAT_HISTO_INFO *hist_out);
	void(*fp_raw2rgb)(CAL_STAT_RGB_INFO_IN *in, CAL_STAT_RGB_INFO_OUT *rgb_out);
	CAL_STAT_BIT(*fp_conv_bit_ipl2stat)(IPL_RAW_BIT ipl_raw_bit);
	CAL_STAT_STPIX(*fp_conv_stpix_ipl2stat)(IPL_RAW_PIX ipl_raw_pix);
} CAL_STATLIB_OBJ, *pCAL_STATLIB_OBJ;

extern pCAL_STATLIB_OBJ cal_statlib(void);
//@}
#endif
