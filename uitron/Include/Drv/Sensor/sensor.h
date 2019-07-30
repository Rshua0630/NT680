/**
    Public header file for Sensor driver

    This file is the header file that define the API and data type for Sensor driver.

    @file       sensor.h
    @ingroup    mIDrvSensor
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _SENSOR_H
#define _SENSOR_H

//----------Header file include-------------------------------------------------
#include "Type.h"
#include "ErrorNo.h"
#include "tge_lib.h"
#include "vx1.h"
#include "slvsec.h"
#include "lvds.h"
#include "csi.h"
#include "sie_lib.h"

#define IPL_PRESET_READY ENABLE

#define CSI_BUG ENABLE

/** \addtogroup mISensor */
//@{
#define SENDRVEXT_PWR_CTRL DISABLE
#define SEN_IGNORE  0xffffffff  ///< sensor ignore information
#define SEN_MFRAME_MAX_NUM  4   ///< dps support max frame number
#define SEN_CSI_MAX_DATALANE  4   ///< csi support max datalane number
#define SEN_CSI_MAX_DATALANE_ID  8   ///< csi support max datalane id
#define SEN_SER_MAX_DATALANE  8   ///< lvds/csi/slvsec serial interface support max datalane number
#define SEN_480MHz 480000000
#define SEN_240MHz 240000000
#define SIE_MAX_SUPPORT_CLK 400000000 ///< HW limit
#define PLL5_MAX_SUPPORT_CLK 399000000 ///< HW limit

#define SEN_RATIO(w, h) (((UINT32)(UINT16)(w) << 16) | (UINT32)(UINT16)(h))
#define SEN_FPS(src, div) (((UINT32)(UINT16)(src) << 16) | (UINT32)(UINT16)(div))
#define CMD_DELAY       0xffffffff // delay
#define CMD_SETAE       0xfffffffe // preset ae parameter
#if CSI_BUG
#define CMD_CSIEN       0xfffffffd // csi enable
#endif
#define CMD_SETVD       0xfffffffc // set vd period
#define CMD_SETEXP      0xfffffffb // set exp
#define CMD_SETGAIN     0xfffffffa // set gain
#define SEN_FPS_DFT     0xff000000 // sensor driver default frame rate

#define SEN_FPS_FMT_CONV2_REAL(sen_fps_fmt) sensor_fps_fmt_conv2_real(sen_fps_fmt) ///< x1000
#define SEN_FPS_EXPT_CONV2_FMT(expt, sen_fps_fmt) sensor_fps_expt_conv2_fmt(expt, sen_fps_fmt)
#define SEN_FPS_CONV2_DIV(fps, src, div) \
	{\
		*src = (fps&0xffff0000)>>16;\
		*div = fps&0x0000ffff;\
	}
#define SEN_FPS_CONV_PARAM(dft_param, cur_param, dft_fps, cur_fps) \
	{\
		*cur_param = ((((UINT32)dft_fps&0xffff0000)>>16)*((UINT32)cur_fps&0x0000ffff)*dft_param)/(((UINT32)dft_fps&0x0000ffff)*(((UINT32)cur_fps&0xffff0000)>>16));\
	}


/**
    Sensor ID
*/
typedef enum {
	SENSOR_ID_1 = 0,                ///< sensor id 1
	SENSOR_ID_2,                    ///< sensor id 2
	SENSOR_ID_3,                    ///< sensor id 3
	SENSOR_ID_4,                    ///< sensor id 4
	SENSOR_ID_5,                    ///< sensor id 5
	SENSOR_ID_6,                    ///< sensor id 6
	SENSOR_ID_7,                    ///< sensor id 7
	SENSOR_ID_8,                    ///< sensor id 8
	SENSOR_ID_MAX_NUM,
	SENSOR_ID_NONE = 0xffffffff,    ///< sensor id null
	ENUM_DUMMY4WORD(SENSOR_ID)
} SENSOR_ID;

/**
    sensor multi-frame ID
*/
typedef enum {
	SENSOR_MULTI_ID_NONE = 0x00,  ///< sensor multi-frame id null
	SENSOR_MULTI_ID_1    = 0x01,  ///< sensor multi-frame id 1
	SENSOR_MULTI_ID_2    = 0x02,  ///< sensor multi-frame id 2
	SENSOR_MULTI_ID_3    = 0x04,  ///< sensor multi-frame id 3
	SENSOR_MULTI_ID_4    = 0x08,  ///< sensor multi-frame id 4
	SENSOR_MULTI_ID_5    = 0x10,  ///< sensor multi-frame id 5
	SENSOR_MULTI_ID_6    = 0x20,  ///< sensor multi-frame id 6
	SENSOR_MULTI_ID_7    = 0x40,  ///< sensor multi-frame id 7
	SENSOR_MULTI_ID_8    = 0x80,  ///< sensor multi-frame id 8
	SENSOR_MULTI_ID_MAX_NUM = 0xffffffff,
	ENUM_DUMMY4WORD(SENSOR_MULTI_ID)
} SENSOR_MULTI_ID;


/**
    sensor signal type
*/
typedef enum {
	SENSOR_SIGNAL_MASTER = 0,       ///< sensor output HD and VD signal
	SENSOR_SIGNAL_SLAVE,            ///< dsp output HD and VD signal
	SENSOR_SIGNAL_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_SIGNAL_TYPE)
} SENSOR_SIGNAL_TYPE;

/**
    sensor type
*/
typedef enum {
	SENSOR_TYPE_CMOS = 0,           ///< CMOS
	SENSOR_TYPE_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_TYPE)
} SENSOR_TYPE;

/**
    sensor mode type
*/
typedef enum {
	SENSOR_MODE_TYPE_UNKNOWN = 0,

	//single frame
	SENSOR_MODE_LINEAR = 1,             ///< linear raw
	SENSOR_MODE_BUILTIN_HDR,        ///< bulid in HDR
	SENSOR_MODE_CCIR,               ///< DVI
	SENSOR_MODE_CCIR_INTERLACE,     ///< DVI INTERLACE

	//multiple frames
	SENSOR_MODE_STAGGER_HDR = 5,        ///< stagger HDR
	SENSOR_MODE_PDAF,               ///< PDAF

	SENSOR_MODE_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_MODE_TYPE)
} SENSOR_MODE_TYPE;

/**
    sensor output data type
*/
typedef enum {
	SENSOR_DATA_PARALLEL = 0,   ///< parallel
	SENSOR_DATA_LVDS     = 1,   ///< LVDS
	SENSOR_DATA_MIPI     = 2,   ///< MIPI
	SENSOR_DATA_SLVSEC   = 3,   ///< SLVS-EC
	SENSOR_DATA_DUMMY    = 4,
	SENSOR_DATA_SIEPATGEN = 5,  ///< sie patten gen
	SENSOR_DATA_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_DATA_TYPE)
} SENSOR_DATA_TYPE;

/**
    sie signal receive mode
*/
typedef enum  {
	SENSOR_SIGNAL_MODE_PARA_MSTR_SNR = 0,   ///< Parallel Master Sensor
	SENSOR_SIGNAL_MODE_PARA_SLAV_SNR,       ///< Parallel Slave Sensor
	SENSOR_SIGNAL_MODE_SELF_PATGEN,         ///< Self Pattern-Generator
	SENSOR_SIGNAL_MODE_VX1_IF0_SNR,         ///< Vx1 Sensor
	SENSOR_SIGNAL_MODE_CSI_1,               ///< Serial Sensor from CSI-1 or LVDS-1
	SENSOR_SIGNAL_MODE_CSI_2,               ///< Serial Sensor from CSI-2 or LVDS-2
	SENSOR_SIGNAL_MODE_CSI_3,               ///< Serial Sensor from CSI-3 or LVDS-3
	SENSOR_SIGNAL_MODE_CSI_4,               ///< Serial Sensor from CSI-4 or LVDS-4
	SENSOR_SIGNAL_MODE_CSI_5,               ///< Serial Sensor from CSI-5 or LVDS-5
	SENSOR_SIGNAL_MODE_CSI_6,               ///< Serial Sensor from CSI-6 or LVDS-6
	SENSOR_SIGNAL_MODE_CSI_7,               ///< Serial Sensor from CSI-7 or LVDS-7
	SENSOR_SIGNAL_MODE_CSI_8,               ///< Serial Sensor from CSI-8 or LVDS-8
	SENSOR_SIGNAL_MODE_DRAM,                ///< Dram Input (channel 0)
	SENSOR_SIGNAL_MODE_AHD,
	SENSOR_SIGNAL_MODE_2PPATGEN,            ///< ??????
	SENSOR_SIGNAL_MODE_SLVS_EC,             ///< Serial Sensor from SLVS-EC
	SENSOR_SIGNAL_MODE_VX1_IF1_SNR,         ///< Vx1 Sensor, interface 1
	ENUM_DUMMY4WORD(SENSOR_SIGNAL_MODE)
} SENSOR_SIGNAL_MODE;

/**
    sensor command interface
*/
typedef enum {
	SENSOR_CMD_UNKNOWN = 0,
	SENSOR_CMD_Vx1     = 1, ///< Vx1
	SENSOR_CMD_SIF     = 2, ///< serial
	SENSOR_CMD_I2C     = 3, ///< I2C
	SENSOR_CMD_IO      = 4, ///< IO
	SENSOR_CMD_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_CMD_TYPE)
} SENSOR_CMD_TYPE;

/**
    sensor support property
*/
typedef enum {
	SENSOR_SUPPORT_NONE     = 0x00000000,
	SENSOR_SUPPORT_MIRROR   = 0x00000001,
	SENSOR_SUPPORT_FLIP     = 0x00000002,

	SENSOR_SUPPORT_ALL      = 0x00000003,

	ENUM_DUMMY4WORD(SENSOR_SUPPORT_PROPERTY)
} SENSOR_SUPPORT_PROPERTY;

/**
    Sensor mode
*/
typedef enum {
	SENSOR_MODE_UNKNOWN = 0,    ///< Unknow Mode
	SENSOR_MODE_1       = 1,    ///< Sensor mode 1
	SENSOR_MODE_2       = 2,    ///< Sensor mode 2
	SENSOR_MODE_3       = 3,    ///< Sensor mode 3
	SENSOR_MODE_4       = 4,    ///< Sensor mode 4
	SENSOR_MODE_5       = 5,    ///< Sensor mode 5
	SENSOR_MODE_6       = 6,    ///< Sensor mode 6
	SENSOR_MODE_7       = 7,    ///< Sensor mode 7
	SENSOR_MODE_8       = 8,    ///< Sensor mode 8
	SENSOR_MODE_9       = 9,    ///< Sensor mode 9
	SENSOR_MODE_10      = 10,   ///< Sensor mode 10
	SENSOR_MODE_11      = 11,   ///< Sensor mode 11
	SENSOR_MODE_12      = 12,   ///< Sensor mode 12
	SENSOR_MODE_13      = 13,   ///< Sensor mode 13
	SENSOR_MODE_14      = 14,   ///< Sensor mode 14
	SENSOR_MODE_15      = 15,   ///< Sensor mode 15
	SENSOR_MODE_Num,
	SENSOR_MODE_CUR     = 0xFF, ///< current sensor mode
	ENUM_DUMMY4WORD(SENSOR_MODE)
} SENSOR_MODE, *PSENSOR_MODE;

/**
    Sensor data start pixel or pattern
*/
typedef enum {
	// RGB start pixel
	SENSOR_STPIX_R = 0,         ///< start pixel R
	SENSOR_STPIX_GR = 1,        ///< start pixel GR
	SENSOR_STPIX_GB = 2,        ///< start pixel GB
	SENSOR_STPIX_B = 3,         ///< start pixel B

	// RGBIR data format
	SENSOR_STPIX_RGBIR_RIR = 100,       ///< pattern RIR
	SENSOR_STPIX_RGBIR_RG = 101,        ///< pattern RG

	// RCCB start pixel
	SENSOR_STPIX_RCCB_RC = 200,         ///< start pixel RC
	SENSOR_STPIX_RCCB_CR = 201,         ///< start pixel CR
	SENSOR_STPIX_RCCB_CB = 202,         ///< start pixel CB
	SENSOR_STPIX_RCCB_BC = 203,         ///< start pixel BC

	// Y only
	SENSOR_STPIX_Y_ONLY = 301,                  ///< y pixel only

	// None
	SENSOR_STPIX_NONE = 0xFFFF,

	SENSOR_STPIX_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_STPIX)
} SENSOR_STPIX;

/**
    Sensor data format
*/
typedef enum {
	SENSOR_FMT_POGRESSIVE = 0,      ///< pogressive mode
	SENOSR_FMT_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_FMT)
} SENSOR_FMT;

/**
    Sensor Act Select(for CCIR656 only)
*/
typedef enum {
	SENSOR_DVI_SEL_EAV = 0,         ///< EAV code recognition
	SENSOR_DVI_SEL_ACTWIN,          ///< Active window size setting
	SENOSR_DVI_SEL_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_DVI_SEL)
} SENSOR_DVI_SEL;

/**
    Sensor DVI MODE
*/
typedef enum {
	SENSOR_DVI_MODE_SD = 0,         ///< DVI sd mode (8 bit)
	SENSOR_DVI_MODE_HD,             ///< DVI hd mode (16 bit)
	SENSOR_DVI_MODE_HD_INV,         ///< DVI hd mode (16 bit) with Byte Inverse
	SENOSR_DVI_MODE_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_DVI_MODE)
} SENSOR_DVI_MODE;


/**
    Sensor DVI format
*/
typedef enum {
	SENSOR_DVI_CCIR601 = 0,         ///< DVI CCIR 601
	SENSOR_DVI_CCIR656_EAV,         ///< DVI CCIR 656 EAV
	SENSOR_DVI_CCIR656_ACT,         ///< DVI CCIR 656 ACT
	SENOSR_DVI_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_DVI_FMT)
} SENSOR_DVI_FMT;

/**
    Sensor DVI IN format
*/
typedef enum {
	SENSOR_DVI_YUYV = 0,
	SENSOR_DVI_YVYU,
	SENSOR_DVI_UYVY,
	SENSOR_DVI_VYUY,
	ENUM_DUMMY4WORD(SENSOR_DVI_IN_FMT)
} SENSOR_DVI_IN_FMT;
/**
    Sensor Option
*/
typedef enum {
	SENSOR_OPTION_NONE           = 0x00000000,    ///< none
	SENSOR_OPTION_MIRROR         = 0x00000001,    ///< Sensor mirror
	SENSOR_OPTION_FLIP           = 0x00000002,    ///< Sensor flip
	SENSOR_OPTION_USER           = 0x00000004,    ///< User option

	SENSOR_OPTION_ALL            = 0x00000007,
	ENUM_DUMMY4WORD(SENSOR_OPTION)
} SENSOR_OPTION, *PSENSOR_OPTION;

/**
    Sensor interface mapping
    SENSOR (x) <-> interface (mapping) <-> SIE (x) <-> IPL (x)
*/
typedef enum {
	/*
	    Default:
	    LVDSMIPI:
	        SENSOR_1_MAP_IF_LVDSMIPI_1/SENSOR_2_MAP_IF_LVDSMIPI_2/SENSOR_3_MAP_IF_LVDSMIPI_3/SENSOR_4_MAP_IF_LVDSMIPI_4/
	        SENSOR_5_MAP_IF_LVDSMIPI_5/SENSOR_6_MAP_IF_LVDSMIPI_6/SENSOR_7_MAP_IF_LVDSMIPI_7/SENSOR_8_MAP_IF_LVDSMIPI_8/
	    SLVSEC:
	        SENSOR_1_MAP_IF_SLVSEC_1
	    VX1_TXMODULE_THCV241:
	        SENSOR_1_MAP_IF_Vx1_1/SENSOR_2_MAP_IF_Vx1_1/SENSOR_3_MAP_IF_Vx1_1/SENSOR_4_MAP_IF_Vx1_1/SENSOR_5_MAP_IF_Vx1_1
	    VX1_TXMODULE_THCV231/VX1_TXMODULE_THCV235:
	        SENSOR_3_MAP_IF_Vx1_1/SENSOR_5_MAP_IF_Vx1_1/SENSOR_4_MAP_IF_Vx1_2/SENSOR_6_MAP_IF_Vx1_2
	*/
	SENSOR_MAP_IF_DEFAULT        = 0,

	// LVDS & MIPI
	SENSOR_2_MAP_IF_LVDSMIPI_1  = 101,
	SENSOR_3_MAP_IF_LVDSMIPI_1  = 102,
	SENSOR_4_MAP_IF_LVDSMIPI_1  = 103,
	SENSOR_4_MAP_IF_LVDSMIPI_2  = 104,

	// SLVSEC
	SENSOR_2_MAP_IF_SLVSEC_1    = 201,

	SENSOR_IF_MAP_MAX_NUM = 0xFFFF,
	ENUM_DUMMY4WORD(SENSOR_MAP_IF)
} SENSOR_MAP_IF, *PSENSOR_MAP_IF;


/**
    I2C physical channel
*/
typedef enum {
	SENSOR_I2C1_1ST = 0,          ///< I2C channel 1
	SENSOR_I2C1_2ND,              ///< I2C channel 1_2

	SENSOR_I2C2_1ST = 2,          ///< I2C channel 2
	SENSOR_I2C2_2ND,              ///< I2C channel 2_2

	SENSOR_I2C3_1ST = 4,          ///< I2C channel 3
	SENSOR_I2C3_2ND,              ///< I2C channel 3_2
	SENSOR_I2C3_3RD,              ///< I2C channel 3_3

	SENSOR_I2C4_1ST = 7,          ///< I2C channel 4
	SENSOR_I2C4_2ND,              ///< I2C channel 4_2
	SENSOR_I2C4_3RD,              ///< I2C channel 4_3
	SENSOR_I2C4_4TH,              ///< I2C channel 4_4

	SENSOR_I2C5_1ST = 11,         ///< I2C channel 5
	SENSOR_I2C5_2ND,              ///< I2C channel 5_2

	SENSOR_I2C_VX1 = 13,          ///< I2C channel vx1

	SENSOR_I2C_MAX_NUM,
	ENUM_DUMMY4WORD(SENSOR_I2C)
} SENSOR_I2C;

/**
    Sensor signal

    @note relative falling edge
*/
typedef struct {
	UINT32 sync;            ///< sync
	UINT32 period;          ///< period
	UINT32 data_start;      ///< valid data start pos
	UINT32 data_size;       ///< valid data size
} SENSOR_SIGNAL;

/**
    Sensor OB position
    @note relative falling edge
*/
typedef struct {
	UINT32 top_win[4];          ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
	UINT32 bottom_win[4];       ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
	UINT32 left_win[4];         ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
	UINT32 right_win[4];        ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
} SENSOR_OB;

/**
    Sensor img ratio information

    // crop_ratio setting rule
    condition1: (full size w) -> (crop size w_c == valid_width)
    setting rule: crop_ratio_h = (w_c/w)*100

    condition2: (full size w) -> (scale size w_s == valid_width)
    setting rule: crop_ratio_h = 100

    condition3: (full size w) -> (crop size w_c) -> (scale size w_s == valid_width)
    setting rule: crop_ratio_h = (w_c/w)*1000

    condition4: (full size w) -> (scale size w_s) -> (crop size w_c == valid_width)
    setting rule: crop_ratio_h = (w_c/w_s)*1000

    crop_ratio_v setting rule same as crop_ratio_h
*/
typedef struct {
	UINT32 ratio_h_v;         ///< hv ratio (H:bit[31:16], V:bit[15:0])
	UINT32 ratio_h;          ///< ratio h (1x = 1000) (relative 1x image size) ((sensor output width x 1000) / (1x width))
	UINT32 ratio_v;          ///< ratio v (1x = 1000) (relative 1x image size) ((sensor output height x 1000) / (1x height))
	UINT32 crop_ratio_h;    ///< crop ratio h (1x = 1000) sensor mode width view angle relative to the ratio of sensor full size width view angle
	UINT32 crop_ratio_v;    ///< crop ratio v (1x = 1000) sensor mode height view angle relative to the ratio of sensor full size height view angle
} SENSOR_IMG_RATIO;

/**
    multi-frame information
*/
#define SEN_BIT_OFS_NONE (0)
#define SEN_BIT_OFS_0 (1<<0)
#define SEN_BIT_OFS_1 (1<<1)
#define SEN_BIT_OFS_2 (1<<2)
#define SEN_BIT_OFS_3 (1<<3)
#define SEN_BIT_OFS_4 (1<<4)
#define SEN_BIT_OFS_5 (1<<5)
#define SEN_BIT_OFS_6 (1<<6)
#define SEN_BIT_OFS_7 (1<<7)
#define SEN_BIT_OFS_8 (1<<8)
#define SEN_BIT_OFS_9 (1<<9)
#define SEN_BIT_OFS_10 (1<<10)
#define SEN_BIT_OFS_11 (1<<11)
#define SEN_BIT_OFS_12 (1<<12)
#define SEN_BIT_OFS_13 (1<<13)
#define SEN_BIT_OFS_14 (1<<14)
#define SEN_BIT_OFS_15 (1<<15)
typedef struct {
	UINT32 total_frm;                          ///< total frame number (range:1~4)
	UINT32 sel_frm_id[SEN_MFRAME_MAX_NUM];     ///< select frame id (LVDS range:0~3, SLVS-EC range:0x0~0xF)
	UINT16 sel_bit_ofs;                         ///< use SEN_BIT_OFS_X, select frame bit(LVDS range:4bits,MIPI range:4bits, SLVS-EC not used)
} SENSOR_MULTI_FRM_INFO;

/**
    RSC information
*/
typedef struct {
	UINT32 str_ln_t;            ///< length from VD start to 1st active line(including OB), uint:us
	UINT32 end_ln_t;            ///< length from VD start to last active line(including OB), uint:us
	UINT32 trans_delay_t;       ///< length from exposure end to start of data transmission, uint:us
} SENSOR_RSC_INFO;

/**
    Sensor lvds ctrl pattern
    bit[63~48]: virtual sensor 3
    bit[47~32]: virtual sensor 2
    bit[31~16]: virtual sensor 1
    bit[15~0]:  virtual sensor 0
*/
typedef struct {
	UINT64 ctrl_hd;          ///< lvds ctrl words
	UINT64 mask_hd;          ///< lvds mask
	UINT64 ctrl_vd;          ///< lvds ctrl words
	UINT64 mask_vd;          ///< lvds mask
} SENSOR_LVDS_CTRLPTN;

typedef struct {
	UINT32  sensor_pin_order;               ///< sensor pin order, range: 0 ~ SEN_SER_MAX_DATALANE
	SENSOR_LVDS_CTRLPTN ctrl_ptn;          ///< lvds ctrl pattern
} SENSOR_LVDS_CTRLPTN_INFO;


/**
    Sensor lvds sync code pattern
*/
typedef struct {
	UINT32 num;             ///< sync code total number
	UINT32 code[7];         ///< sync code
} SENSOR_LVDS_SYNC_CODE_PTN;

/**
    Sensor mode lvds information
*/
typedef struct {
	SENSOR_SIGNAL xhs;  ///< HD length for sony sensor only. (only use the Sync, Period)
	SENSOR_SIGNAL xvs;  ///< VD length for sony sensor only. (only use the Sync, Period)
	UINT32 width;       ///< data output width
	UINT32 height;      ///< data output height
	UINT32 clk_lane_num;///< clock lanes number (use LVDS_CLKLANE_NO)
	UINT32 lane_num;    ///< data lanes number (use LVDS_DATLANE_NO)
	UINT32 bit_depth;   ///< data bits
	UINT32 data_align;  ///< data MSB/LSB
	UINT32 vsync_gen;   ///< refer to LVDS_VSYNC_GEN (use LVDS_VSYNC_GEN)
	UINT32 output_pixel_order[LVDS_DATLANE_ID_MAX];   ///< output data pixel order
	UINT32 fset_bit;    ///< only for SONY HDR Type Sensor, set to LVDS_CONFIG_ID_FSET_BIT
} SENSOR_MODE_LVDS;

/**
    Sensor mode mipi information
*/
typedef enum {
	SEN_MIPI_PIXDEPTH_8BIT,         ///< Pixel depth is  8 bits per pixel.
	SEN_MIPI_PIXDEPTH_10BIT,        ///< Pixel depth is 10 bits per pixel.
	SEN_MIPI_PIXDEPTH_12BIT,        ///< Pixel depth is 12 bits per pixel.
	SEN_MIPI_PIXDEPTH_14BIT,        ///< Pixel depth is 14 bits per pixel.

	ENUM_DUMMY4WORD(SEN_MIPI_PIXDEPTH)
} SEN_MIPI_PIXDEPTH;

typedef enum {
	SEN_MIPI_MANUAL_8BIT,         ///< mipi manual Pixel depth is  8 bits per pixel.
	SEN_MIPI_MANUAL_10BIT,        ///< mipi manual Pixel depth is 10 bits per pixel.
	SEN_MIPI_MANUAL_12BIT,        ///< mipi manual Pixel depth is 12 bits per pixel.
	SEN_MIPI_MANUAL_14BIT,        ///< mipi manual Pixel depth is 14 bits per pixel.
	SEN_MIPI_MANUAL_YUV422,        ///< mipi manual Pixel depth is YUV422.

	SEN_MIPI_MANUAL_NONE = 0xF,        ///< mipi auto.

	ENUM_DUMMY4WORD(SEN_MIPI_MANUAL_BIT)
} SEN_MIPI_MANUAL_BIT;

#define SEN_MIPI_PIXEL_DATA 0xffff

typedef struct {
	SEN_MIPI_MANUAL_BIT bit;  ///< manaul depack format (CSI_CONFIG_ID_MANUAL_FORMAT/..)
	UINT32 data_id;           ///< receiving data id, range: 0x00~0xFF (CSI_CONFIG_ID_MANUAL_DATA_ID/..) or SEN_MIPI_PIXEL_DATA
} SEN_MIPI_MANUAL_INFO;

typedef struct {
	UINT32 lane_num;                ///< csi data lanes number (use CSI_DATLANE_NUM)
	SEN_MIPI_PIXDEPTH bit_depth;    ///< data bits
	SEN_MIPI_MANUAL_INFO manual_info[3];
	BOOL save_pwr;                  ///< only support MIPI data rate < 1Gbps
} SENSOR_MODE_MIPI;

/**
    tge mode information
*/
typedef struct {
	SENSOR_SIGNAL xhs;  ///< HD period
	SENSOR_SIGNAL xvs;  ///< VD period
} SENSOR_MODE_TGE;

/**
    Sensor command structure
*/
typedef struct {
	UINT32 ui_addr;                      ///< address
	UINT32 ui_data_len;                   ///< data length(bytes)
	UINT32 ui_data[2];                   ///< data idx1(LSB) -> idx2(MSB)
} SENSOR_CMD;

/**
    Sensor DVI structure

    ==Data period from parallel interface==
    The SENSOR_DVI_COM_NUM(for PARAL_DATA_PERIOD) and SENSOR_DVI_SEL_IDX(PARAL_DATA_INDEX) are used to separate data.
    SENSOR_DVI_COM_NUM: SIE accepts a sequence which combined by 1/2/3/4 separate input from sensor.
    SENSOR_DVI_SEL_IDX: index of selected pixel for SIE.
*/
typedef enum {
	SENSOR_DVI_COM_NUM_1 = 0,
	SENSOR_DVI_COM_NUM_2 = 1,
	SENSOR_DVI_COM_NUM_3 = 2,
	SENSOR_DVI_COM_NUM_4 = 3,

	SENSOR_DVI_COM_MAX_NUM = 4,
	ENUM_DUMMY4WORD(SENSOR_DVI_COM_NUM)
} SENSOR_DVI_COM_NUM;

typedef enum {
	SENSOR_DVI_SEL_IDX_1 = 0,
	SENSOR_DVI_SEL_IDX_2 = 1,
	SENSOR_DVI_SEL_IDX_3 = 2,
	SENSOR_DVI_SEL_IDX_4 = 3,

	SENSOR_DVI_SEL_IDX_MAX_NUM = 4,
	ENUM_DUMMY4WORD(SENSOR_DVI_SEL_IDX)
} SENSOR_DVI_SEL_IDX;

typedef struct {
	BOOL auto_align;
	SENSOR_DVI_COM_NUM combine_num; ///< SIE accepts a sequence which combined by 1/2/3/4 separate input from sensor.
	SENSOR_DVI_SEL_IDX select_id;   ///< index of selected pixel for SIE.
} SENSOR_DVI_AHD_INFO;

typedef struct {
	BOOL vd_mode;   ///< CCIR656_VD_MODE (FALSE: Interlaced system, TRUE: progressive system)
} SENSOR_DVI_CCIR656_INFO;

typedef struct {
	SENSOR_DVI_FMT fmt;                 ///< dvi format
	SENSOR_DVI_MODE mode;               ///< dvi mode
	SENSOR_DVI_IN_FMT data_fmt;          ///< dvi in data format
	BOOL fld_en;                         ///< start input from field with internal field signal=0
	BOOL fld_inv_en;                      ///< inverse the polarity of internal field signal
	BOOL out_split_en;                    ///< output Split en (YUYV -> YYYYYYY UVUVUV)
	SENSOR_DVI_AHD_INFO ahd_info;
	SENSOR_DVI_CCIR656_INFO ccir656_info;
} SENSOR_MODE_DVI;

/**
    Sensor SLVSEC structure
*/
typedef struct {
	SENSOR_SIGNAL xhs;  ///< HD length for sony sensor only. (only use the Sync, Period)
	SENSOR_SIGNAL xvs;  ///< VD length for sony sensor only. (only use the Sync, Period)
	UINT32 width;       ///< data output width
	UINT32 height;      ///< data output height
	UINT32 lane_num;    ///< data lanes number (use SLVSEC_DATLANE_NO)
	UINT32 bit_depth;   ///< data bits (use SLVSEC_PIXDEPTH)
	UINT32 output_pixel_order[SLVSEC_PADPIN_MAX];   ///< output data pixel order
} SENSOR_MODE_SLVSEC;


/**
    Sensor GAIN structure
*/
typedef struct {
	UINT32 min;                     ///< min gain (1X = 1 x 1000 = 1000)
	UINT32 max;                     ///< max gain (1X = 1 x 1000 = 1000)
} SENSOR_MODE_GAIN;

/**
    Sensor mode information
*/
typedef struct {
	SENSOR_MODE_TYPE mode_type; ///< sensor mode type(HDR or .....)
	SENSOR_STPIX stpix;         ///< Sensor start pixel or data format
	SENSOR_FMT fmt;             ///< Sensor data type
	UINT32 valid_width;         ///< sensor output pixel number
	UINT32 valid_height;        ///< sensor output pixel number
	SENSOR_IMG_RATIO ratio;     ///< Sensor ratio information
	UINT32 frame_rate;          ///< frame rate (Dividend:bit[31:16], Divisor:bit[15:0])
	UINT32 pclk;                ///< pixel clock Hz
	UINT32 data_rate;           ///< data rate (pixel/s)
	UINT32 bining_ratio;        ///< binning ratio X 100
	SENSOR_MODE_GAIN gain;      ///< sensor gain X 1000
	SENSOR_RSC_INFO rsc;        ///< RSC information
	SENSOR_OB ob;               ///< Sensor OB

	SENSOR_MULTI_FRM_INFO *multi_frame_info;    ///< sensor select frame information

	SENSOR_SIGNAL *trans_hd[SEN_MFRAME_MAX_NUM]; ///< transfer HD signal ([1]~[3] for Sensor multi-frame)
	SENSOR_SIGNAL *trans_vd[SEN_MFRAME_MAX_NUM]; ///< transfer VD signal ([1]~[3] for Sensor multi-frame)
	SENSOR_SIGNAL *sen_hd[SEN_MFRAME_MAX_NUM];   ///< Sensor HD signal ([1]~[3] for Sensor multi-frame)
	SENSOR_SIGNAL *sen_vd[SEN_MFRAME_MAX_NUM];   ///< Sensor VD signal ([1]~[3] for Sensor multi-frame)

	SENSOR_MODE_LVDS    *lvds;      ///< lvds information
	SENSOR_MODE_MIPI    *mipi;      ///< mipi information
	SENSOR_MODE_TGE     *tge;       ///< tge information
	SENSOR_MODE_DVI     *dvi;       ///< dvi information
	SENSOR_MODE_SLVSEC  *slvsec;    ///< slvs-ec information
} SENSOR_MODE_INFO;

/**
    tge information
*/

typedef struct {
	TGE_DATA_PHASE_SEL tge_vd_phase;   ///< tge VD phase
	TGE_DATA_PHASE_SEL tge_hd_phase;   ///< tge HD phase
	BOOL tge_vd_inv;   ///< tge VD inverse
	BOOL tge_hd_inv;   ///< tge HD inverse
} SENSOR_TGE;

/**
    vx1 information
*/
typedef enum {
	SEN_VX1_1LANE = 1,	///< 1 lane.
	SEN_VX1_2LANE = 2,	///< 2 lane.

	ENUM_DUMMY4WORD(SEN_VX1_LANE)
} SEN_VX1_LANE;

typedef struct {
	UINT8 PLL0;
	UINT8 PLL1;
	UINT8 PLL2;
	UINT8 PLL3;
	UINT8 PLL4;
	UINT8 PLL5;
	UINT8 PLL6;
	UINT8 PLL7;
} VX1_PLL_241;

typedef enum {
	SEN_VX1_DATAMUX_3BYTE_RAW,	///< Vx1 is operating at 3BYTE Muxing Mode with RAW10/RAW12 sensor in use.
								///< 1T D[23:0] to 2T "D[23:12]"+"D[11:0]".
								///< This shall be used for THCV241 with RAW sensor.
	SEN_VX1_DATAMUX_4BYTE_RAW,	///< Vx1 is operating at 4BYTE Muxing Mode with RAW10/RAW12 sensor in use.(default)
								///< 1T D[31:0] to 2T "D[27:16]"+"D[11:0]".
								///< This shall be used for THCV231/THCV235 with RAW sensor.
	SEN_VX1_DATAMUX_YUV,		///< YUV sensor
	SEN_VX1_DATAMUX_NONE,		///< D[11:0] to D[11:0]. Debugging usages.

	ENUM_DUMMY4WORD(SEN_VX1_DATAMUX)
} SEN_VX1_DATAMUX;


typedef enum {
	SEN_VX1_IF0, // support (parallel sensor) and (mipi sensor linear mode)
	SEN_VX1_IF1, // support mipi sensor linear and hdr mode

	ENUM_DUMMY4WORD(SEN_VX1_IF)
} SEN_VX1_IF;

/*
	if b_manual_vx1_id = FALSE, auto select:
	SENSOR_ID_1/SENSOR_ID_2/SENSOR_ID_3/SENSOR_ID_4/SENSOR_ID_5 + VX1_TXMODULE_THCV241: VX1_ID_1
	SENSOR_ID_3/SENSOR_ID_5 + VX1_TXMODULE_THCV235/VX1_TXMODULE_THCV231: VX1_ID_1
	SENSOR_ID_4/SENSOR_ID_6 + VX1_TXMODULE_THCV235/VX1_TXMODULE_THCV231: VX1_ID_2
	other: not support

	if b_manual_vx1_if = FALSE, auto select:
	SENSOR_ID_1/SENSOR_ID_2/SENSOR_ID_4 + VX1_TXMODULE_THCV241 + VX1_ID_1: SEN_VX1_IF1
	SENSOR_ID_3/SENSOR_ID_5/SENSOR_ID_6/SENSOR_ID_7/SENSOR_ID_8 + VX1_TXMODULE_THCV241 + VX1_ID_1: SEN_VX1_IF0
	SENSOR_ID_1/SENSOR_ID_2/SENSOR_ID_3/SENSOR_ID_4/SENSOR_ID_5/SENSOR_ID_6/SENSOR_ID_7/SENSOR_ID_8 + VX1_TXMODULE_THCV241 + VX1_ID_2: IF0
	SENSOR_ID_1/SENSOR_ID_2/SENSOR_ID_3/SENSOR_ID_4/SENSOR_ID_5/SENSOR_ID_6/SENSOR_ID_7/SENSOR_ID_8 + VX1_TXMODULE_THCV235/VX1_TXMODULE_THCV231 + VX1_ID_1/VX1_ID_2: IF0

*/
typedef struct {
//	VX1_TXINIT tx_init;                       ///< Vx1 Transmitter operation mode select
	VX1_TXMODULE tx_module;     ///< vx1 transmitter Module
	SEN_VX1_LANE lane_num;          ///< vx1 lane number
	SEN_VX1_DATAMUX data_mux;
	ER(*fp_tx_gpio)(SENSOR_ID id, VX1_TXMODULE tx_module);

	BOOL b_manual_vx1_id;
	VX1_ID manual_vx1_id;

	BOOL b_manual_vx1_if;
	SEN_VX1_IF manual_vx1_if;

	/* only VX1_TXMODULE_THCV241 need to implement */
	BOOL clk_speed_bps;///< When using THCV241 is PLL auto calculation path.
					   ///< The user must assign the sensor clock speed in bits per second to make calculation tool working.
					   ///< For example, if the sensor clock speed is 445Mbps, please set 445000000 (bps) to this field.
					   ///< set 0 for manual mode (need to set VX1_PLL_241 pll_241[SENSOR_MODE_Num])
	VX1_PLL_241 pll_241[SENSOR_MODE_Num];    ///< only clk_speed_bps ==  0 need to set. refer to setThcv241PLL.
	BOOL cko_pin;   ///< refer to VX1SBL_CONFIG_ID_CKO_OUTOUT.
} SEN_Vx1;

typedef struct {
	VX1_TXINIT tx_init;         ///< Vx1 Transmitter operation mode
} SEN_Vx1_INFO;

/**
    sensor interface (if) information
*/
typedef struct {
	SENSOR_LVDS_SYNC_CODE_PTN *(*fp_get_sync_code)(LVDS_PIXDEPTH pixel_depth);
	SENSOR_LVDS_CTRLPTN *(*fp_get_ctrl_ptn)(UINT32 idx, LVDS_PIXDEPTH pixel_depth, SENSOR_MODE_TYPE mode_type);
} SEN_LVDS_INFO;

typedef struct {
	SEN_LVDS_INFO lvds_info;
} SEN_IF_INFO;

/**
    sensor cmd information
    if sensor_cmd_get_txrx_ctrl_obj()->init, set SEN_RXMODULE_THCV236:
    	SEN_I2C_W_ADDR_DFT: #define THCV236_WRITE_ID_DFT (0x0B << 1) // AIN1 = 0, AIN0 = 0
		SEN_I2C_W_ADDR_OPTION1: #define THCV236_WRITE_ID_1 (0x34 << 1) // AIN1 = 0, AIN0 = 1
		SEN_I2C_W_ADDR_OPTION2: #define THCV236_WRITE_ID_2 (0x77 << 1) // AIN1 = 1, AIN0 = 0
		SEN_I2C_W_ADDR_OPTION3: #define THCV236_WRITE_ID_3 (0x65 << 1) // AIN1 = 1, AIN0 = 1
*/
typedef enum {
	SEN_I2C_W_ADDR_DFT = 0,
	SEN_I2C_W_ADDR_OPTION1 = 1,
	SEN_I2C_W_ADDR_OPTION2 = 2,
	SEN_I2C_W_ADDR_OPTION3 = 3,
	SEN_I2C_W_ADDR_OPTION4 = 4,
	SEN_I2C_W_ADDR_OPTION5 = 5,

	SEN_I2C_W_ADDR_SEL_MAX_NUM,
	ENUM_DUMMY4WORD(SEN_I2C_W_ADDR_SEL)
} SEN_I2C_W_ADDR_SEL;

typedef struct {
	SEN_I2C_W_ADDR_SEL w_addr_sel;
	UINT8 w_addr;
} SEN_I2C_INFO;

typedef struct {
	SEN_I2C_INFO i2c_info[SEN_I2C_W_ADDR_SEL_MAX_NUM];
} SEN_CMD_INFO;

typedef struct {
	BOOL hdr_dol;
} SENSOR_DATA_CFG;

typedef struct {
	BOOL support_chg;
} SENSOR_FPS_INFO;

typedef struct {
	UINT32 mclk;        ///< Hz
} SENSOR_CLK_INFO;

typedef enum {
	SENSOR_PHASE_RISING,  ///< rising edge latch/trigger
	SENSOR_PHASE_FALLING, ///< falling edge latch/trigger
	ENUM_DUMMY4WORD(SENSOR_PHASE_SEL)
} SENSOR_PHASE_SEL;

typedef enum {
	SENSOR_ACTIVE_HIGH = 0,  ///< sensor active high, SIE non-inverse
	SENSOR_ACTIVE_LOW = 1,  ///< sensor active low, SIE inverse
	ENUM_DUMMY4WORD(SENSOR_ACTIVE_SEL)
} SENSOR_ACTIVE_SEL;

typedef struct {
	SENSOR_ACTIVE_SEL vd_inv;
	SENSOR_ACTIVE_SEL hd_inv;
	SENSOR_PHASE_SEL vd_phase;
	SENSOR_PHASE_SEL hd_phase;
	SENSOR_PHASE_SEL data_phase;
} SENSOR_SIGNAL_INFO;

/**
    Sensor information
*/
typedef struct {
	SENSOR_TYPE sen_type;       ///< sensor type
	SENSOR_SIGNAL_MODE sig_mode;///< sie signal receive mode
	SENSOR_SIGNAL_INFO sig_info;///< sie signal information
	SENSOR_DATA_TYPE data_type; ///< transfer type
	SENSOR_CMD_TYPE cmd_type;   ///< protocol type
	SENSOR_SUPPORT_PROPERTY property;   ///< sensor support property
	UINT32 cell_width;          ///< cell width mm * 1000
	UINT32 cell_height;         ///< cell height mm * 1000
	UINT32 width_1x;            ///< width (full scan pixel size)
	UINT32 height_1x;           ///< height(full scan pixel size)
	UINT32 ae_effective_time;   ///< AE effective time
	UINT32 sync_timing;         ///< sync timing for Exposure time & gain(VD)
	SEN_Vx1_INFO vx1;           ///< Vx1 information
	SENSOR_MODE_INFO *mode;     ///< sensor current mode
} SENSOR_INFO;

typedef struct {
	SENSOR_MODE sensor_mode;    ///< sensor mode
	SENSOR_INFO sensor_info;    ///< sensor information
} SEN_STATUS_PARAM;

/**
    command information
*/
/**
    SIF information
*/
typedef struct {
	UINT32      channel;     ///< sif channel (use enum SIF_CH)
	UINT32      bus_clk;     ///< unit:HZ
	UINT32      sen_d_s;    ///< map to sif duration, ref:SIF_CONFIG_ID_SENS,SIF_CONFIG_ID_SEND
	UINT32      sen_h;      ///< map to sif hold time,ref:SIF_CONFIG_ID_SENH
	BOOL        dma_en;     ///< dma mode
} SEN_SIF;

/**
    I2C information
*/
typedef struct {
	SENSOR_I2C i2c;                 ///< I2C channel
	UINT32  s_clk;                   ///< I2C clock
	SEN_I2C_W_ADDR_SEL w_addr_sel;
} SEN_I2C;

/**
    IO information
*/
typedef struct {
	ER(*fp_io)(SENSOR_ID id, UINT32 ui_data_0);                    ///< GPIO function point
} SEN_IO;

/**
    Sensor command information
*/

typedef struct {
	SENSOR_CMD_TYPE cmd_type;        ///< command type
	union {
		SEN_SIF sif;                ///< SIF information
		SEN_I2C i2c;                ///< I2C information
		SEN_IO  io;                 ///< IO  information
	} info;
	SEN_Vx1  vx1;                 ///< vx1  information
} SENSOR_CMD_INFO;

/**
    TGE information
*/
typedef struct {
	BOOL b_enable;                       ///< set tge enable
	UINT32 interrupt;                   ///< set tge interrupt enable (TGE_INT_XX)
	TGE_CLKSRC_SEL tge_clk_src;           ///< select tge clock source
	void (*fp_tge_isr_cb)(UINT32 status);  ///< isr callback function
	TGE_OUT_SWAP_SEL swap_info;          ///< set tge swap information
	BOOL b_chg_vd_to_sie_src;                ///< set tge send which VD/HD to SIE (FALSE:VDHD->SIE VD5HD5->SIE3, TRUE:VD3HD3->SIE VD7HD7->SIE3)
} SEN_TGE;

/**
    CSI information
*/
typedef struct {
	void(*fp_hw_rst)(SENSOR_ID id);
} SEN_CSI;

/**
    Sensor interface information
*/
typedef struct {
	SEN_TGE tge;    ///< TGE information
	SEN_CSI csi;    ///< CSI information
} SENSOR_IF_INFO;

/**
    Sensor sie mclk information
*/
typedef enum {
	SEN_SIEMCLK_SRC_DFT = 0,          ///< id 0: PLL_CLK_SIEMCLK, id 1~7: PLL_CLK_SIEMCLK2
	SEN_SIEMCLK_SRC_MCLK = 1,          ///< PLL_CLK_SIEMCLK
	SEN_SIEMCLK_SRC_MCLK2 = 2,          ///< PLL_CLK_SIEMCLK2

	SEN_SIEMCLK_SRC_MAX = 3,
	ENUM_DUMMY4WORD(SEN_SIEMCLK_SRC)
} SEN_SIEMCLK_SRC;

typedef struct {
	UINT32 chg_mclk_en;         ///< change mclk depend on sensor mode
	SEN_SIEMCLK_SRC mclk_src;   ///< change sie mclk source
	BOOL b_chg_mclk_freq; ///< if TRUE, chg_mclk_en need to set TRUE, need to set mclk_freq
	UINT32 mclk_freq;   ///< change sie mclk freq
} SENSOR_SIEMCLK_INFO;

/**
    Sensor sie signal information
*/
typedef struct {
	BOOL b_ext;
	SENSOR_ACTIVE_SEL vd_inv;
	SENSOR_ACTIVE_SEL hd_inv;
	SENSOR_PHASE_SEL vd_phase;
	SENSOR_PHASE_SEL hd_phase;
	SENSOR_PHASE_SEL data_phase;
} SENSOR_SIGNAL_EXT_INFO;

/**
    sensor initial object
*/
typedef struct {
	SENSOR_CMD_INFO cmd_info;        ///< command information
	SENSOR_IF_INFO  interface_info;  ///< interface information
	UINT32 sen_2_serial_pin_map[SEN_SER_MAX_DATALANE];    ///< Serial (lvds/csi/slvsec) pin 2 hw map
	SENSOR_SIEMCLK_INFO sie_mclk_info;
	SENSOR_OPTION option;           ///< Special Command
	SENSOR_MAP_IF sen_map_if;     ///< sensor id map sensor interface (lvds/csi/slvsec/vx1/...)
	BOOL serial_clane_switch;     ///< serial (lvds/csi/slvsec) interface clock lane switch (reference to CSI_CONFIG_ID_CLANE_SWITCH / LVDS_CONFIG_ID_CLANE_SWITCH)
	UINT32 time_out;                ///< wait fmd timeout, Unit in ms. 0 = no timeout, only support CSI and LVDS sensor
	SENSOR_SIGNAL_EXT_INFO signal_info;
} SENSOR_INIT_OBJ;


/**
    sensor chgmode information
    * sensor frame rate
      - input: user input/SEN_FPS_DFT
      - user input: (Dividend:bit[31:16], Divisor:bit[15:0])
      - user input: must be less than sensor driver default value
      - user input result: Dividend/Divisor
*/
typedef struct {
	SENSOR_MULTI_ID multi_id;   ///< sensor multi id
	SENSOR_MODE mode;           ///< sensor mode
	UINT32 frame_rate;          ///< sensor frame rate
} SEN_CHGMODE_DATA;

/**
    sensor chgmode fps information
    * sensor frame rate
      - input: user input/SEN_FPS_DFT/SEN_FPS_PRE
      - user input: (Dividend:bit[31:16], Divisor:bit[15:0])
      - user input: must be less than sensor driver default value
      - user input result: Dividend/Divisor
*/
typedef struct {
	SENSOR_MODE mode;           ///< sensor mode
	UINT32 frame_rate;          ///< sensor frame rate
} SEN_CHGFPS_DATA;

/**
    sensor get information item
*/

typedef struct {
	SENSOR_MODE sen_mode;
	SENSOR_MULTI_ID multi_id;
	SENSOR_MODE preset_sen_mode;
} SENSOR_CUR_INFO;

typedef enum {
	SEN_GET_CUR_MODE = 0,       ///< data type: SENSOR_MODE, sensor current mode
	SEN_GET_STATUS,             ///< data type: SEN_STATUS_PARAM, sensor and sensor mode information
	SEN_GET_IF_MAP,             ///< data type: SENSOR_MAP_IF, sensor interface mapping
	SEN_GET_CMD_INFO,           ///< data type: SEN_CMD_INFO, sensor cmd information
	SEN_GET_FPS,                ///< data type: FPS_PARAM, sensor frame rate information
	SEN_GET_MAX_MODE = 5,       ///< data type: UINT32, sensor max support mode
	SEN_GET_NAME,               ///< data type: void, sensor name
	SEN_GET_EXPO_SETTING,       ///< data type: EXPOSURE_PARAM, sensor exposure setting
	SEN_GET_GAIN_SETTING,       ///< data type: GAIN_PARAM, sensor gain setting
	SEN_GET_USER_OPTION,        ///< data type: UINT32 (SENSOR_OPTION), sensor user option
	SEN_GET_HDR_STATUS = 10,    ///< data type: void, hdr status
	SEN_GET_TEMP,               ///< data type: UINT32, sensor temperature
	SEN_GET_GAIN_INFO,          ///< data type: GAIN_INFO, sensor gain infomation
	SEN_GET_USER_DEFINE1,       ///< user define 1 by sensor
	SEN_GET_USER_DEFINE2,       ///< user define 2 by sensor
	SEN_GET_USER_DEFINE3,       ///< user define 3 by sensor

	SEN_GET_MAX_NUM,
	ENUM_DUMMY4WORD(SEN_GET_ITEM)
} SEN_GET_ITEM;

/**
    sensor set information item
*/

typedef enum {
	SEN_SET_EXPT = 0,           ///< EXPOSURE_SETTING, sensor exposure setting
	SEN_SET_GAIN,               ///< GAIN_SETTING, sensor gain setting
	SEN_SET_FPS,                ///< UINT32 (frame rate), set frame rate
	SEN_SET_USER_OPTION,        ///< UINT32 (SENSOR_OPTION), sensor user option
	SEN_SET_GRS,                ///< void, GRS mode parameters
	SEN_SET_HDR_INFO = 5,           ///< HDR_SET_INFO, HDR set infomation
	SEN_SET_TRIGGER_EVENT,  ///< SEN_TRIGGER_EVENT_INFO, trigger event
	SEN_SET_USER_DEFINE1,       ///< user define 1 by sensor
	SEN_SET_USER_DEFINE2,       ///< user define 2 by sensor
	SEN_SET_USER_DEFINE3,       ///< user define 3 by sensor

	SEN_SET_MAX_NUM,
	ENUM_DUMMY4WORD(SEN_SET_ITEM)
} SEN_SET_ITEM;

/**
    sensor eshutter info
*/
typedef struct {
	UINT32 ui_frame[SEN_MFRAME_MAX_NUM];         ///< exposure frame
	UINT32 ui_line[SEN_MFRAME_MAX_NUM];          ///< exposure line
	UINT32 ui_pixel[SEN_MFRAME_MAX_NUM];         ///< exposure pixel
	UINT32 ui_line_ratio;                        ///< exposure line ratio (long expt line * 100 / short expt line)
} EXPOSURE_SETTING;

typedef struct {
	SENSOR_MODE sensor_mode;                    ///< sensor mode
	UINT32 exp_time[SEN_MFRAME_MAX_NUM];        ///< exposure time
	EXPOSURE_SETTING exp_setting;               ///< exposure setting
	UINT32 compensate_ratio[SEN_MFRAME_MAX_NUM];///< compensate ratio
} EXPOSURE_PARAM;

/**
    sensor gain info
*/
typedef struct {
	UINT32 data1[SEN_MFRAME_MAX_NUM];         ///< depend on sensor register
	UINT32 data2[SEN_MFRAME_MAX_NUM];         ///< depend on sensor register
	UINT32 data3[SEN_MFRAME_MAX_NUM];         ///< depend on sensor register
	UINT32 rev[SEN_MFRAME_MAX_NUM];           ///< internal information(do not modify)
} GAIN_SETTING;

typedef struct {
	SENSOR_MODE sensor_mode;                ///< sensor mode
	UINT32 gain_ratio[SEN_MFRAME_MAX_NUM];  ///< gain ratio (1.5x = 1.5 x 1000 = 1500)
	GAIN_SETTING gain_setting;              ///< gain setting
} GAIN_PARAM;

typedef struct {
	SENSOR_MODE sensor_mode;        ///< sensor mode
	SENSOR_MODE_GAIN gain;          ///< gain information
} GAIN_INFO;

/**
    sensor HDR info
*/
typedef struct {
	SENSOR_MODE sensor_mode;        ///< sensor mode
	UINT32 expt_ratio;      ///< sensor hdr exptime ratio
	UINT32 gain_ratio;      ///< sensor hdr gain ratio
	UINT32 user_option[10]; ///< define by sensor
} HDR_SET_INFO;

/**
    sensor trigger event information
*/
typedef struct {
	UINT32 info[10];
} SEN_TRIGGER_EVENT_INFO;

/**
    sensor preset information
*/
typedef struct {
	SENSOR_MODE mode;
	UINT32 frame_rate;
} SEN_PRESET_MODE;

typedef struct {
	BOOL lock;
} SEN_PRESET_LOCK;

/**
    sensor fps info
*/
typedef struct {
	SENSOR_MODE sensor_mode;///< sensor mode
	UINT32 cur_fps;         ///< current frame rate (Dividend:bit[31:16], Divisor:bit[15:0])
	UINT32 chg_fps;         ///< chgmode frame rate (Dividend:bit[31:16], Divisor:bit[15:0])
	UINT32 dft_fps;         ///< sensor driver default (max) support frame rate (Dividend:bit[31:16], Divisor:bit[15:0])
} FPS_PARAM;

/**
    sensor common info
*/
typedef struct {
	UINT32 cur_sensor_mode;                   ///< current sensor mode(do not modify)
} SEN_COM_INFO;

/**
    sensor driver common interface
*/

typedef struct {
	SENSOR_TYPE sen_type;       ///< sensor type
	SENSOR_SIGNAL_TYPE sig_type;///< sensor master or slave
	SENSOR_DATA_TYPE data_type; ///< transfer type
	SENSOR_DATA_CFG data_cfg;   ///< sensor data config information
	SENSOR_SIGNAL_INFO sig_info;///< sie or tge signal information
	SENSOR_FPS_INFO fps_info;       ///< fps information
	SENSOR_CLK_INFO clk_info;       ///< mclk information
	SENSOR_SUPPORT_PROPERTY property;   ///< sensor support property
	USIZE cell_size;            ///< cell size mm * 1000
	USIZE  pixel_size;          ///< full scan pixel size
	UINT32 ae_effective_time;   ///< AE effective time
	UINT32 sync_timing;         ///< sync timing for Exposure time & gain(VD)
} SENSOR_DRV_INFO;

typedef struct {
	SENSOR_OPTION option;           ///< Special Command
} SEN_DRV_INIT_OBJ;

typedef enum {
	SEN_GET_DRV_FPS,                ///< data type: FPS_PARAM, sensor frame rate information
	SEN_GET_DRV_MAX_MODE,           ///< data type: UINT32, sensor max support mode
	SEN_GET_DRV_NAME,               ///< data type: void, sensor name
	SEN_GET_DRV_EXPO_SETTING,       ///< data type: EXPOSURE_PARAM, sensor exposure setting
	SEN_GET_DRV_GAIN_SETTING,       ///< data type: GAIN_PARAM, sensor gain setting
	SEN_GET_DRV_USER_OPTION,        ///< data type: UINT32 (SENSOR_OPTION), sensor user option
	SEN_GET_DRV_HDR_STATUS,         ///< data type: void, hdr status
	SEN_GET_DRV_TEMP,               ///< data type: UINT32, sensor temperature
	SEN_GET_DRV_GAIN_INFO,          ///< data type: GAIN_INFO, sensor gain infomation
	SEN_GET_DRV_USER_DEFINE1,       ///< user define 1 by sensor
	SEN_GET_DRV_USER_DEFINE2,       ///< user define 2 by sensor
	SEN_GET_DRV_USER_DEFINE3,       ///< user define 3 by sensor

	SEN_GET_DRV_MAX_NUM,
	ENUM_DUMMY4WORD(SEN_GET_DRV_ITEM)
} SEN_GET_DRV_ITEM;

/**
    sensor set information item
*/

typedef enum {
	SEN_SET_DRV_EXPT,           ///< EXPOSURE_SETTING, sensor exposure setting
	SEN_SET_DRV_GAIN,           ///< GAIN_SETTING, sensor gain setting
	SEN_SET_DRV_USER_OPTION,    ///< UINT32 (SENSOR_OPTION), sensor user option
	SEN_SET_DRV_GRS,            ///< void, GRS mode parameters
	SEN_SET_DRV_HDR_INFO,       ///< HDR_SET_INFO, HDR set infomation
	SEN_SET_DRV_TRIGGER_EVENT,  ///< SEN_TRIGGER_EVENT_INFO, trigger event infomation
	SEN_SET_DRV_PRESET_MODE,	///< SEN_PRESET_MODE, preset mode infomation
	SEN_SET_DRV_PRESET_LOCK,	///< SEN_PRESET_LOCK, preset lock infomation
	SEN_SET_DRV_USER_DEFINE1,   ///< user define 1 by sensor
	SEN_SET_DRV_USER_DEFINE2,   ///< user define 2 by sensor
	SEN_SET_DRV_USER_DEFINE3,   ///< user define 3 by sensor

	SEN_SET_DRV_MAX_NUM,
	ENUM_DUMMY4WORD(SEN_SET_DRV_ITEM)
} SEN_SET_DRV_ITEM;

typedef struct {
	//Common
	ER(*fp_init)(SENSOR_ID id, SEN_DRV_INIT_OBJ *drv_init_obj);         ///< initial sensor flow
	ER(*fp_uninit)(SENSOR_ID id);                                   ///< un-initial sensor flow
	ER(*fp_sleep)(SENSOR_ID id);                                    ///< enter sensor sleep mode
	ER(*fp_wakeup)(SENSOR_ID id);                                   ///< exit sensor sleep mode
	ER(*fp_write)(SENSOR_ID id, SENSOR_CMD *cmd);                   ///< write command
	ER(*fp_read)(SENSOR_ID id, SENSOR_CMD *cmd);                    ///< read command
	ER(*fp_chg_mode)(SENSOR_ID id, SEN_CHGMODE_DATA chg_mode_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt);   ///< change mode flow
	ER(*fp_chg_fps)(SENSOR_ID id, SEN_CHGFPS_DATA chg_fps_data, GAIN_SETTING *preset_gain, EXPOSURE_SETTING *preset_expt);   ///< change mode flow
	ER(*fp_set_info)(SENSOR_ID id, SEN_SET_DRV_ITEM set_drv_item, void *data, SEN_COM_INFO com_info);  ///< set sensor information
	ER(*fp_get_info)(SENSOR_ID id, SEN_GET_DRV_ITEM get_drv_item, void *data);  ///< get sensor information

	SENSOR_DRV_INFO *(*fp_get_sensor_drv_info)(void);
	SENSOR_MODE_INFO *(*fp_get_sensor_mode_info)(SENSOR_MODE mode);
	SEN_CMD_INFO *(*fp_get_cmd_info)(void);
	SEN_IF_INFO *(*fp_get_if_info)(void);

#if SENDRVEXT_PWR_CTRL
	ER(*fp_pwron)(SENSOR_ID id);                                    ///< power on flow
	ER(*fp_pwroff)(SENSOR_ID id);                                   ///< power off flow
#endif
} SENSOR_DRV_TAB;

typedef struct {
	SENSOR_INIT_OBJ ini_obj;
	SENSOR_DRV_TAB *drv_tab;
} SENSOR_HANDLE;


/**
    debug information
*/
typedef enum {
	SENSOR_DEBUG_NONE       = 0x00000000,   ///< dump ori error msg
	SENSOR_DEBUG_API        = 0x00000001,   ///< sensor internal debug
	SENSOR_DEBUG_API_AE     = 0x00000002,   ///< sensor internal debug
	SENSOR_DEBUG_IF         = 0x00000010,   ///< sensor internal debug
	SENSOR_DEBUG_CMD_IF     = 0x00000100,   ///< sensor internal debug
	SENSOR_DEBUG_DRVEXT     = 0x00001000,   ///< for .\DrvExt\DrvExt_src\sensor
	SENSOR_DEBUG_MODELEXT   = 0x00010000,   ///< for .\DrvExt\DrvExt_src\ModelExt

	ENUM_DUMMY4WORD(SENSOR_DEBUG_ITEM)
} SENSOR_DEBUG_ITEM;
extern SENSOR_DEBUG_ITEM sensor_debug_item[SENSOR_ID_MAX_NUM];

/**
    General Sensor APIs
*/
extern UINT32 sensor_fps_fmt_conv2_real(UINT32 sen_fps_fmt);
extern void sensor_fps_expt_conv2_fmt(UINT32 expt, UINT32 *sen_fps_fmt);
extern ER sensor_cfg_sie_mclk(SENSOR_ID id, SENSOR_INIT_OBJ *sen_init_obj, SENSOR_DRV_TAB *sen_drv_tab);
extern ER sensor_cfg_sie_max_clk(SENSOR_ID id, SENSOR_INIT_OBJ *sen_init_obj, SENSOR_DRV_TAB *sen_drv_tab);
extern ER sensor_cfg_pll_clk(void);
extern void sensor_set_group_info(SENSOR_MULTI_ID* sensor_gp_id, SENSOR_MULTI_ID* sensor_subgp_id);
extern BOOL sensor_chk_power_on(SENSOR_ID id);
extern BOOL sensor_chk_power_off(SENSOR_ID id);
extern BOOL sensor_chk_power_save_on(SENSOR_ID id);
extern BOOL sensor_chk_power_save_off(SENSOR_ID id);

/**
    open sensor

    @param[in] id       sensor id
    @param[in] init_obj  sensor initial object
    @param[in] DrvTab   sensor driver table
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER sensor_open(SENSOR_ID id, SENSOR_HANDLE *handle);

/**
    close sensor

    @param[in] id sensor id
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER sensor_close(SENSOR_ID id);

/**
    check sensor is open or not

    @param[in] id sensor id
    @return
        - @b FALSE close, TRUE open
 */
extern BOOL sensor_is_open(SENSOR_ID id);

/**
    Turn on sensor step by step

    @param[in] id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_pwron(SENSOR_ID id);

/**
    Turn off sensor step by step

    @param[in] id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_pwroff(SENSOR_ID id);

/**
    Set sensor to standby state

    @param[in] id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_sleep(SENSOR_ID id);

/**
    Resume the sensor from the standby

    @param[in] id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_wakeup(SENSOR_ID id);


/**
    configure sensor register

    @param[in] id    sensor id
    @param[in] cmd   sensor cmd
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_write_reg(SENSOR_ID id, SENSOR_CMD *cmd);

/**
    read sensor register

    @param[in] id    sensor id
    @param[in] cmd   sensor cmd
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_read_reg(SENSOR_ID id, SENSOR_CMD *cmd);

/**
    Set the current operation mode

    @param[in] id               sensor id
    @param[in] chg_mode_data    sensor multi-id/sensor mode/frame rate
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_chg_mode(SENSOR_ID id, SEN_CHGMODE_DATA chg_mode_data);

/**
    reset tge signal to sync multi-id

    @param[in] id       sensor id
    @param[in] multi_id  sensor multi-id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_chg_mode_rst(SENSOR_ID id, SENSOR_MULTI_ID multi_id);

/**
    Set sensor information

    @param[in] id           sensor id
    @param[in] set_item    set information item
    @param[out] data        sensor information
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_set_info(SENSOR_ID id, SEN_SET_ITEM set_item, void *data);

/**
    Get sensor information

    @param[in] id           sensor id
    @param[in] get_item    get information item
    @param[out] data        sensor information
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER sensor_get_info(SENSOR_ID id, SEN_GET_ITEM get_item, void *data);


/**
    preset sensor mode, (sensor_preset_lock(1) -> sensor_preset_mode -> sensor_set_gain or ...... ->sensor_preset_lock(0))

    @param[in] SENSOR_ID    sensor id
    @param[in] mode         sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
*/
#if IPL_PRESET_READY
extern ER sensor_preset_mode(SENSOR_ID id, SENSOR_MODE mode, UINT32 frame_rate);
#else
extern ER sensor_preset_mode(SENSOR_ID id, SENSOR_MODE mode);
#endif

/**
    preset lock

    @param[in] SENSOR_ID    sensor id
    @param[in] mode         sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER sensor_preset_lock(SENSOR_ID id, UINT32 flag);
extern void sensor_dump_info(void (*dump)(char *fmt, ...));
extern ER sensor_dump_ctrl_info(void (*dump)(char *fmt, ...));

extern void sensor_install_id(void) _SECTION(".kercfg_text");

//@}
#endif
