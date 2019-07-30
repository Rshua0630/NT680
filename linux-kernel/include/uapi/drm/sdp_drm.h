/*
 * Copyright (C) 2013 Samsung Electronics Co.Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _UAPI_SDP_DRM_H_
#define _UAPI_SDP_DRM_H_

#include <linux/videodev2.h> /* fixme: how to remove denpendency ?? */
#include <drm/drm.h>

/************************
* DRM ADDED PROPERTIES  *
*************************/
/* plane properties */
#define DRM_PROP_PLANE_TYPE_STR			"plane_type"
#define DRM_PROP_VBLANK_TYPE_STR		"vblank_type"
#define DRM_PROP_FLIP_STR			"flip_mode"
#define DRM_PROP_STEREOSCOPIC_STR		"stereo-scopic_mode"
#define DRM_PROP_GFX_ALPHA_STR			"gfxp_alpha"
#define DRM_PROP_GFX_ORDER_STR			"gfxp_order"
#define DRM_PROP_GAMEMODE_STR			"game_mode"
#define DRM_PROP_DP_OUT_PATH_STR		"dp_out_path"
#define DRM_PROP_DP_FRAME_DOUBLE_STR		"dp_frame_doubling"
#define DRM_PROP_DP_PIVOT_STR			"dp_pivot"
#define DRM_PROP_GEOMETRY_STR			"plane_geometry"
#define DRM_PROP_ROTATION_STR			"plane_rotation"
#define DRM_PROP_RESOLUTION_STR			"plane_res_info"
#define DRM_PROP_HW_CSC_STR			"plane_hw_csc"
#define DRM_PROP_LBD_MODE_STR			"plane_lbd_mode"
#define DRM_PROP_LBD_CONFIG_STR			"plane_lbd_config"

/* crtc properties */
#define DRM_PROP_CRTC_DP_PC_SHARPNESS_STR	"dp_pc_sharpness"
#define DRM_PROP_CRTC_DP_ORDER_STR		"dp_order"
#define DRM_PROP_CRTC_LAYER_STR			"layer_order"
#define DRM_PROP_CRTC_OUTPUT_RES_STR		"output_res"
#define DRM_PROP_CRTC_FRC_INFO_STR		"frc_info"
#define DRM_PROP_CRTC_POST_PATTERN_STR		"post_pattern"
#define DRM_PROP_CRTC_LVDS_FORMAT_STR  		"lvds_format"
#define DRM_PROP_CRTC_VIDEO_CONFIG_STR		"video_config"
#define DRM_PROP_CRTC_PIXEL_SHIFT_STR		"pixel_shift"

/* gfx crtc properties */
#define DRM_PROP_CRTC_GFX_OUTPUT_RES_STR	"gfx_output_res"


/* connector properties : this is crtc properties, temporarily */
#if 0
#define DRM_PROP_CONNECTOR_TYPE_STR		"conn_output_type"
#define DRM_PROP_CONNECTOR_COLOR_SYS_STR	"conn_color_system"
#endif
#define DRM_PROP_CRTC_CVBSOUT_DPMS_STR		"CVBS_DPMS"
#define DRM_PROP_CRTC_COLOR_SYS_STR		"cvbsout_color_system"

#define DRM_PROP_3D_BUF_MODE_SHIFT	8
#define DRM_PROP_3D_MODE_MASK		0xFF
#define DRM_PROP_3D_BUF_MODE_MASK	0xF00

/* plane type */
enum sdp_drm_plane_type {
	DRM_SDP_PLANE_OSDP,
	DRM_SDP_PLANE_GP,
	DRM_SDP_PLANE_SGP,
	DRM_SDP_PLANE_CURSOR1,
	DRM_SDP_PLANE_CURSOR2,
	DRM_SDP_PLANE_DP_MAIN,
	DRM_SDP_PLANE_DP_SUB,
	DRM_SDP_PLANE_DP_SUB2,
	DRM_SDP_PLANE_DP_BG,
	DRM_SDP_PLANE_MAX
};

/* vblank type */
enum sdp_drm_vblank {
	DRM_SDP_VBLANK_GFX = 0,
	DRM_SDP_VBLANK_DP = _DRM_VBLANK_SECONDARY,
};

/* flip type */
enum sdp_drm_flip {
	DRM_SDP_NO_FLIP,
	DRM_SDP_H_FLIP,
	DRM_SDP_V_FLIP,
	DRM_SDP_HV_FLIP,
	DRM_SDP_FLIP_MAX,
};

/* flip type */
enum sdp_drm_pivot {
	DRM_SDP_PIVOT_NORMAL,
	DRM_SDP_PIVOT_H_INV,
	DRM_SDP_PIVOT_V_INV,
	DRM_SDP_PIVOT_HV_INV,
	DRM_SDP_PIVOT_MAX,
};

enum sdp_drm_dp_plane_order {
	DRM_SDP_DP_PLANE_SMB,
	DRM_SDP_DP_PLANE_SBM,
	DRM_SDP_DP_PLANE_MSB,
	DRM_SDP_DP_PLANE_MBS,
	DRM_SDP_DP_PLANE_BSM,
	DRM_SDP_DP_PLANE_BMS,
	DRM_SDP_DP_PLANE_RESERVED,
	DRM_SDP_DP_PLANE_MB,
};

enum sdp_drm_output_type {
	DRM_SDP_OUTPUT_CVBS,
	DRM_SDP_OUTPUT_MAX
};

enum sdp_drm_color_system {
	DRM_SDP_ANAVENC_PALM,	/**< PAL-M	*/
	DRM_SDP_ANAVENC_PALN,	/**< PAL-N	*/
	DRM_SDP_ANAVENC_PALBG,	/**< PAL-B,G	*/
	DRM_SDP_ANAVENC_PAL60,	/**< PAL60	*/
	DRM_SDP_ANAVENC_NTSCM,	/**< NTSC-M	*/
	DRM_SDP_ANAVENC_SECAM,	/**< SECAM	*/
	DRM_SDP_ANAVENC_MAX
};

enum sdp_drm_rotation {
	DRM_SDP_DEGREE_CCW_0,	/** No Rotation */
	DRM_SDP_DEGREE_CCW_90,	/** Counter Clock Wise 90 degree Rotation */
	DRM_SDP_DEGREE_CCW_180,	/** Counter Clock Wise 180 degree Rotation */
	DRM_SDP_DEGREE_CCW_270,	/** Counter Clock Wise 270 degree Rotation */
};

enum sdp_drm_lbd_mode {
	DRM_SDP_LBD_MODE_OFF,	/** LetterBoxDetect mode off*/
	DRM_SDP_LBD_MODE_1080I,	/** 1080i mode */
	DRM_SDP_LBD_MODE_1080P,	/** 1080p mode */
	DRM_SDP_LBD_MODE_2160P,	/** 2160p mode */
	DRM_SDP_LBD_MODE_720P,	/** 720p  mode */
	DRM_SDP_LBD_MODE_MAX
};

enum sdp_drm_layer {
	DRM_SDP_LAYER_TOP_VIDEO, /** Videos move to higher than graphics */
	DRM_SDP_LAYER_BOTTOM_VIDEO, /** Videos move to lower than graphics */
	DRM_SDP_LAYER_MIDDLE_VIDEO, /** VIdeos move between graphics (OSD-Video-GP-SP) */
};

enum sdp_drm_dp_out {
	DRM_SDP_WINDOW,		/* screen plane */
	DRM_SDP_ENCODING,	/* encoding path - encoder inst. is 0 */
	DRM_SDP_SCART_OUT_CVBS,	/* scart out - CVBS */
	DRM_SDP_SCART_OUT_BT656,/* scart out - BT656 */
	DRM_SDP_ENCODING_1,	/* encoding path - encoder inst. is 1 */
	DRM_SDP_OUT_MAX
};

enum sdp_drm_3d_mode {
	DRM_SDP_3D_2D,
	DRM_SDP_3D_FRAMEPACKING,
	DRM_SDP_3D_FRAMESEQ,
	DRM_SDP_3D_TOPBOTTOM,
	DRM_SDP_3D_SIDEBYSIDE,
	DRM_SDP_3D_MAX
};

enum sdp_drm_src_3dmode {
	DRM_SDP_SRC3D_2D, 
	DRM_SDP_SRC3D_SBS, 
	DRM_SDP_SRC3D_TNB, 
	DRM_SDP_SRC3D_FRAME_SEQ,
	DRM_SDP_SRC3D_FRAME_PACK,
	DRM_SDP_SRC3D_KR3D, 
	DRM_SDP_SRC3D_MVC_B,
	DRM_SDP_SRC3D_MFC_D, 
	DRM_SDP_SRC3D_SVAF,
	DRM_SDP_SRC3D_MVC,
	DRM_SDP_SRC3D_MAX
};

enum sdp_drm_3d_buf_mode {
	DRM_SDP_3D_BUF_SINGLE,
	DRM_SDP_3D_BUF_DOUBLE,
	DRM_SDP_3D_BUF_MAX
};

enum sdp_drm_lvds_format {  
	DRM_SDP_LVDS_PDP,	/* lvds format : PDP */
	DRM_SDP_LVDS_JEIDA,	/* lvds format : JEIDA */
	DRM_SDP_LVDS_VESA,	/* lvds format : VESA */
	DRM_SDP_LVDS_MAX
};

enum sdp_drm_internal_pattern {
	DRM_SDP_INTERNAL_PATTERN_OFF,
	DRM_SDP_INTERNAL_PATTERN_CHESS,
	DRM_SDP_INTERNAL_PATTERN_MAIN_LINE_VISION1,
	DRM_SDP_INTERNAL_PATTERN_MAIN_LINE_VISION2,
	DRM_SDP_INTERNAL_PATTERN_8COLOR_BAR,
	DRM_SDP_INTERNAL_PATTERN_RGB_RAMP,
	DRM_SDP_INTERNAL_PATTERN_FULL_WHITE,
	DRM_SDP_INTERNAL_PATTERN_FULL_RED,
	DRM_SDP_INTERNAL_PATTERN_FULL_GREEN,
	DRM_SDP_INTERNAL_PATTERN_FULL_BLUE,
	DRM_SDP_INTERNAL_PATTERN_16GRAY,
	DRM_SDP_INTERNAL_PATTERN_32GRAY,
	DRM_SDP_INTERNAL_PATTERN_64GRAY,
	DRM_SDP_INTERNAL_PATTERN_FULL_BLACK,
	DRM_SDP_INTERNAL_PATTERN_GRAY_RAMP,
	DRM_SDP_INTERNAL_PATTERN_GRAY_RAMP_BMP,
	DRM_SDP_INTERNAL_PATTERN_WB, 
	DRM_SDP_INTERNAL_PATTERN_WB1,
	DRM_SDP_INTERNAL_PATTERN_WB2,
	DRM_SDP_INTERNAL_PATTERN_3D,
	DRM_SDP_INTERNAL_PATTERN_RGB_COMPOSITE,
	DRM_SDP_INTERNAL_PATTERN_GAMMA1,
	DRM_SDP_INTERNAL_PATTERN_GAMMA2,
	DRM_SDP_INTERNAL_PATTERN_RED_100_IRE,
	DRM_SDP_INTERNAL_PATTERN_RED_70_IRE,
	DRM_SDP_INTERNAL_PATTERN_RED_40_IRE,
	DRM_SDP_INTERNAL_PATTERN_RED_20_IRE,
	DRM_SDP_INTERNAL_PATTERN_RED_10_IRE,
	DRM_SDP_INTERNAL_PATTERN_GREEN_100_IRE,
	DRM_SDP_INTERNAL_PATTERN_GREEN_70_IRE,
	DRM_SDP_INTERNAL_PATTERN_GREEN_40_IRE,
	DRM_SDP_INTERNAL_PATTERN_GREEN_20_IRE,
	DRM_SDP_INTERNAL_PATTERN_GREEN_10_IRE,
	DRM_SDP_INTERNAL_PATTERN_BLUE_100_IRE,
	DRM_SDP_INTERNAL_PATTERN_BLUE_70_IRE,
	DRM_SDP_INTERNAL_PATTERN_BLUE_40_IRE,
	DRM_SDP_INTERNAL_PATTERN_BLUE_20_IRE,
	DRM_SDP_INTERNAL_PATTERN_BLUE_10_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_100_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_90_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_80_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_70_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_60_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_50_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_40_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_30_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_20_IRE,
	DRM_SDP_INTERNAL_PATTERN_WHITE_10_IRE,
	DRM_SDP_INTERNAL_PATTERN_OFF_L400_R1000,
	DRM_SDP_INTERNAL_PATTERN_OFF_L200_R1000,
	DRM_SDP_INTERNAL_PATTERN_OFF_L1000_R200,
	DRM_SDP_INTERNAL_PATTERN_OFF_L100_R1K,
	DRM_SDP_INTERNAL_PATTERN_OFF_L1K_R100,
	DRM_SDP_INTERNAL_PATTERN_OFF_L1K_R10K,
	DRM_SDP_INTERNAL_PATTERN_OFF_L10K_R1K,
	DRM_SDP_INTERNAL_PATTERN_OFF_L100_R10K,
	DRM_SDP_INTERNAL_PATTERN_OFF_L10K_R100,
	DRM_SDP_INTERNAL_PATTERN_OFF_L1K,
	DRM_SDP_INTERNAL_PATTERN_OFF_R1K,
	DRM_SDP_INTERNAL_PATTERN_OFF_L10K,
	DRM_SDP_INTERNAL_PATTERN_OFF_R10K,
	DRM_SDP_INTERNAL_PATTERN_OFF_L100,
	DRM_SDP_INTERNAL_PATTERN_OFF_R100,
	DRM_SDP_INTERNAL_PATTERN_OFF_L600,
	DRM_SDP_INTERNAL_PATTERN_OFF_R600,
	DRM_SDP_INTERNAL_PATTERN_45D_RAMP,
	DRM_SDP_INTERNAL_PATTERN_WHITE_BOX,
	DRM_SDP_INTERNAL_PATTERN_VERTICAL_RAMP,
	DRM_SDP_INTERNAL_PATTERN_HV_RAMP,
	DRM_SDP_INTERNAL_PATTERN_RGB_BOX,
	DRM_SDP_INTERNAL_PATTERN_HATCH,
	DRM_SDP_INTERNAL_PATTERN_V_RAMP,
	DRM_SDP_INTERNAL_PATTERN_H_RAMP,
	DRM_SDP_INTERNAL_PATTERN_DOT,
	DRM_SDP_INTERNAL_PATTERN_MAX
};

enum sdp_drm_hwcsc_mode {
	DRM_SDP_CSC_BYPASS,
	DRM_SDP_CSC_YC_TO_RGB,
	DRM_SDP_CSC_RGB_TO_YC,
	DRM_SDP_CSC_RESERVED0,
	DRM_SDP_CSC_RESERVED1,
	DRM_SDP_CSC_MAX
};

enum sdp_drm_color_spec {
	DRM_SDP_CS_ITU_R_BT601,
	DRM_SDP_CS_ITU_R_BT709,
	DRM_SDP_CS_ITU_R_BT2020,
	DRM_SDP_CS_RESERVED0,
	DRM_SDP_CS_RESERVED1,
	DRM_SDP_CS_RESERVED2,
	DRM_SDP_CS_RESERVED3,
	DRM_SDP_CS_MAX
};

struct sdp_drm_gfx_output_res {
	unsigned short h_res;
	unsigned short v_res;
	unsigned short reserved[2];
};

struct sdp_drm_output_res {
	unsigned short h_res;
	unsigned short v_res;
	unsigned short framerate;
	unsigned short reserved;
};

struct sdp_drm_frc_info {
	unsigned char b_is_60hz;
	unsigned char reserved[7];	
};

struct sdp_drm_geometry {
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

struct sdp_drm_post_pattern {		/* structure for setting pattern*/
	unsigned char onoff;		/* on / off */
	unsigned char pattern_num;	/* pattern type num */
	unsigned char chip_num;
	unsigned char reserved[5];	/* reserved */
};

struct sdp_drm_res {
	unsigned short hres;		/* h-resolution */
	unsigned short vres;		/* v-resolution */
	unsigned short framerate;	/* frame-rate */
	unsigned char progressive;	/* 1~: progressive, 0 : interlace */
	unsigned char reserved;		/* reserved */
};

struct sdp_drm_hw_csc{
	enum sdp_drm_hwcsc_mode mode : 8;	
	enum sdp_drm_color_spec spec : 8;
	unsigned char hdmi_color_lev;	/* 0 : Low, 1 : Normal */
	unsigned char reserved;		/* resever 1 byte Padding*/
	unsigned int hdmi_color_fmt; 	/* DRM_FORMAT_RGB888, DRM_FORMAT_NV42, DRM_FORMAT_NV61, DRM_FORMAT_NV21 */
};

struct sdp_drm_video_config {
	unsigned char is_panel_sync_mode :1;	/* DE(0)/SYNC(1) Mode */
	unsigned char is_pdp		:1;	/* LCD(0)/PDP[1] */
	unsigned char frc_info		:4;	/* FRC Chip */
	unsigned char isdb_info		:1;	/* ISDB-T Information*/
	unsigned char is_panel_60Hz	:1;	/* 50/60 Hz*/
	unsigned char output_format;		/* Display Output */
	unsigned char reserved[6];		/* reserved 6 Bytes*/
};

struct sdp_drm_lbd_config {
	unsigned char on_detect_time;	/* lbd ON detect time */
	unsigned char off_detect_time;	/* lbd OFF detect time */
	unsigned char reserved[6];	/* reserved */ 
};

struct sdp_drm_pixel_shift {
	unsigned char pixel_shift_on;	/* to enable/disable pixel shift */
	char h_pixel_shift;		/* horizontal pixel shift (Range :  -4 to + 4 pixel) */ 	
	char v_pixel_shift;		/* vertical pixel shift (Range :  -4 to + 4 pixel) */
	unsigned char reserved[5];	/* reserved 5 bytes */
};

union sdp_drm_prop_data {
	struct sdp_drm_post_pattern 	p_patt;		/* post-pattern */
	struct sdp_drm_geometry 	geometry;	/* geometry */
	struct sdp_drm_res 		resolution;	/* geometry */
	struct sdp_drm_frc_info 	frc;		/* frc info. */
	struct sdp_drm_output_res 	output_res;	/* output's res. */
	struct sdp_drm_gfx_output_res	gfx_output_res;	/* gfx output's res. */
	struct sdp_drm_hw_csc		hw_csc;		/* hw csc info */
	struct sdp_drm_video_config	video_config;	/* video config */
	struct sdp_drm_lbd_config	lbd_config;	/* letter box config */
	struct sdp_drm_pixel_shift	pixel_shift;	/* pixel shift */
};

union sdp_drm_prop_val {		/* for changing prop_val */
	unsigned long long value;	/* setting prop_val */
	union sdp_drm_prop_data data;	/* setting data */
};

/***********don't use ************/
#define DRM_PROP_CRTC_MAIN_OUTPUT_RES_STR "main_output_res"
union sdp_drm_output_res_val {
	unsigned long long prop_val;
	struct sdp_drm_output_res data;
	struct sdp_drm_gfx_output_res gfx_data;
};

union sdp_drm_frc_info_val {
	unsigned long long prop_val;
	struct sdp_drm_frc_info data;
};

union sdp_drm_geometry_val {
	unsigned long long prop_val;
	struct sdp_drm_geometry data;
};

/******** SDP GA **************************/
enum sdp_drm_ga_bpp_mode {
	BPP8  = 1,
	BPP16 = 2,
	BPP32 = 4
};

enum sdp_drm_ga_op_type {
	SDP_GA_SOLID_FILL,
	SDP_GA_COPY, /* BITBLT*/
	SDP_GA_SCALE,
	SDP_GA_ROP /* BLEND */
};

enum sdp_drm_ga_color_mode {
	GFX_GA_FORMAT_32BPP_aRGB,
	GFX_GA_FORMAT_32BPP_RGBa,
	GFX_GA_FORMAT_16BPP,
	GFX_GA_FORMAT_8BPP,
	GFX_FORMAT_MAX
};

enum sdp_drm_ga_mode {
	GA_BITBLT_MODE_NORMAL,
	GA_BITBLT_MODE_PREALPHA,
	GA_BITBLT_MODE_CA,
	GA_BITBLT_MODE_SRC,
	GA_BLOCKFILL_MODE,
	GA_MHP_ROP_CA_VALUE_MODE,
	GA_MHP_FILLED_ROP_CA_VALUE_MODE,
	GA_SCALE_MODE,
	GA_SCALEDROP_MODE,
	GA_YCBCR_PACK_MODE,/*spec out*/
	GA_MHP_ROP_CA_MODE,	/*spec out*/
	GA_MODE_END
};

struct sdp_drm_ga_src_info {
	enum sdp_drm_ga_color_mode color_mode;
	unsigned int src_hbyte_size;
	unsigned int base_addr;
	unsigned int startx;
	unsigned int starty;
	unsigned int width;
	unsigned int height;
	enum sdp_drm_ga_bpp_mode bit_depth;
};

struct sdp_drm_ga_rect {
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
};

enum sdp_drm_ga_mhp_rop {
	S_ROP_COPY,
	S_ROP_ALPHA,
	S_ROP_TRANSPARENT,
	S_ROP_DVB_SRC,
	S_ROP_DVB_SRC_OVER,
	S_ROP_DVB_DST_OVER,
	S_ROP_DVB_SRC_IN,
	S_ROP_DVB_DST_IN,
	S_ROP_DVB_SRC_OUT,
	S_ROP_DVB_DST_OUT,
	S_ROP_OVERLAY,
	S_ROP_XOR,
	S_ROP_SRC_ATOP,
	S_ROP_DST_ATOP,
	S_ROP_DVB_CLEAR,
	S_ROP_8BPP_BLENDING
};

enum sdp_drm_ga_key_mode {
	GA_KEYMODE_OFF,
	GA_KEYMODE_ROP_TRANSPARENT,
	GA_KEYMODE_8BPP
};

enum sdp_drm_ga_filled_rop_mode {
	GA_SRC1_FILLCOLOR_SRC2_IMAGE,
	GA_SRC1_IMAGE_SRC2_FILLCOLOR
};

enum sdp_drm_ga_pre_mul_alpha_mode {
	GA_PREMULTIPY_ALPHA_OFF_SHADOW = 1,
	GA_PREMULTIPY_ALPHA_ON_SHADOW = 3
};

struct sdp_drm_ga_pixmap_info {
	enum sdp_drm_ga_bpp_mode bit_depth;
	unsigned int width;
	unsigned int height;
};

struct sdp_drm_ga_solid_fill {
	enum sdp_drm_ga_color_mode color_mode;
	struct sdp_drm_ga_pixmap_info  pixmap;
	void *paddr;
	unsigned int handle;
	unsigned int hbyte_size;
	unsigned int h_start;
	unsigned int v_start;
	unsigned int h_size;
	unsigned int v_size;
	unsigned int color;
	unsigned int stride;
};

struct sdp_drm_ga_scale {
	enum sdp_drm_ga_color_mode color_mode;

	/* Scale Src */
	void *src_paddr;
	unsigned int src_handle;
	unsigned int src_hbyte_size;
	struct sdp_drm_ga_rect	src_rect;

	/* Scale Dst */
	void		*dst_paddr;
	unsigned int dst_handle;
	unsigned int dst_hbyte_size;
	struct sdp_drm_ga_rect dst_rect;

	/* ROP */
	enum sdp_drm_ga_mhp_rop	rop_mode;
	enum sdp_drm_ga_pre_mul_alpha_mode pre_mul_alpha;
	unsigned int rop_ca_value;
	unsigned int src_key;
	unsigned int rop_on_off;
};

struct sdp_drm_ga_bitblt {
	enum sdp_drm_ga_color_mode color_mode;
	enum sdp_drm_ga_mode ga_mode;

	void *src1_paddr;
	unsigned int src1_handle;
	unsigned int src1_byte_size;
	struct sdp_drm_ga_rect src1_rect;

	void *dst_paddr;
	unsigned int dst_handle;
	unsigned int dst_byte_size;
	unsigned int dst_x;
	unsigned int dst_y;

	unsigned int ca_value;
};

struct sdp_drm_ga_rop {
	enum sdp_drm_ga_mode ga_mode;
	enum sdp_drm_ga_color_mode color_mode;
	enum sdp_drm_ga_pre_mul_alpha_mode pre_mul_alpha;

	void		*src1_paddr;
	unsigned int src1_handle;
	unsigned int src1_byte_size;
	struct sdp_drm_ga_rect	src1_rect;

	void		*dst_paddr;
	unsigned int dst_handle;
	unsigned int dst_byte_size;
	unsigned int dst_x;
	unsigned int dst_y;

	unsigned int fill_color;

	union {
		struct mhp_const_t {
			enum sdp_drm_ga_mhp_rop rop_mode;
			unsigned int color_key;
			unsigned int ca_value;
		} mhp_const;
	} rop_mode;

	enum sdp_drm_ga_filled_rop_mode filled_rop_mode;
};

struct sdp_drm_ga_exec {
	enum sdp_drm_ga_op_type ga_op_type;

	/* One of the GA Operation*/
	union op {
		struct sdp_drm_ga_solid_fill solid_fill;
		struct sdp_drm_ga_bitblt bitblt;
		struct sdp_drm_ga_scale scale;
		struct sdp_drm_ga_rop rop;
		/* Add other GA Ops here */
	} ga_op;
};

/* indicate cache units. */
enum sdp_drm_gem_cache_sel_type {
	SDP_DRM_L1_CACHE = 1 << 0,
	SDP_DRM_L2_CACHE = 1 << 1,
	SDP_DRM_ALL_CORES = 1 << 2,
	SDP_DRM_ALL_CACHES = SDP_DRM_L1_CACHE | SDP_DRM_L2_CACHE,
	SDP_DRM_ALL_CACHES_CORES = SDP_DRM_L1_CACHE 
				| SDP_DRM_L2_CACHE 
				| SDP_DRM_ALL_CORES,
	SDP_DRM_CACHE_SEL_MASK	= SDP_DRM_ALL_CACHES_CORES
};

/* indicate cache operation types. */
enum sdp_drm_gem_cache_op_type {
	SDP_DRM_CACHE_INV_ALL	= 1 << 3,
	SDP_DRM_CACHE_INV_RANGE	= 1 << 4,
	SDP_DRM_CACHE_CLN_ALL	= 1 << 5,
	SDP_DRM_CACHE_CLN_RANGE	= 1 << 6,
	SDP_DRM_CACHE_FSH_ALL	= SDP_DRM_CACHE_INV_ALL 
				| SDP_DRM_CACHE_CLN_ALL,
	SDP_DRM_CACHE_FSH_RANGE	= SDP_DRM_CACHE_INV_RANGE 
				| SDP_DRM_CACHE_CLN_RANGE,
	SDP_DRM_CACHE_OP_MASK	= SDP_DRM_CACHE_FSH_ALL 
				| SDP_DRM_CACHE_FSH_RANGE
};
/**
 * A structure for cache operation.
 *
 * @usr_addr: user space address.
 *	P.S. it SHOULD BE user space.
 * @size: buffer size for cache operation.
 * @flags: select cache unit and cache operation.
 */
struct sdp_drm_gem_cache_op {
	const void* usr_addr;
	unsigned int size;
	unsigned int flags;
	unsigned int gem_handle;
};
/***************************************************************/

/* memory type */
#define SDP_DRM_GEM_TYPE(x)	((x) & 0x7)
#define SDP_DRM_GEM_CONTIG	(0x0 << 0)
#define SDP_DRM_GEM_NONCONTIG	(0x1 << 0)
#define SDP_DRM_GEM_MP		(0x2 << 0)
#define SDP_DRM_GEM_HW		(0x3 << 0)
#define SDP_DRM_GEM_GA		(0x4 << 0)

/* memory type : dp memory */
enum sdp_drm_gem_hw_mem_flag {
	SDP_DRM_GEM_DP_FB_Y,
	SDP_DRM_GEM_DP_FB_C,
/* ---- will remove */	
	SDP_DRM_GEM_DP_RM_CAPT_Y,
	SDP_DRM_GEM_DP_RM_CAPT_C,
	SDP_DRM_GEM_DP_MAIN_CAPT_Y,
	SDP_DRM_GEM_DP_MAIN_CAPT_C,
	SDP_DRM_GEM_DP_SUB_CAPT_Y,
	SDP_DRM_GEM_DP_SUB_CAPT_C,
	SDP_DRM_GEM_DP_FB_CR,
	SDP_DRM_GEM_DP_SUB2_CAPT_Y,
	SDP_DRM_GEM_DP_SUB2_CAPT_C,
	SDP_DRM_GEM_DP_ENCIN_CAPT,
	SDP_DRM_GEM_DP_INPUT_CAPT,
/************************/	
	SDP_DRM_GEM_DP_MEM_MAX
};

/* SDP-GEM's bit-count for moving */
#define DP_FB_PLANE_BIT_MOVE	3
#define DRM_INST_BIT_MOVE	DP_FB_PLANE_BIT_MOVE /* replace PLANE_BIT_MOVE*/
#define DRM_BUF_BIT_MOVE	7

/* SDP-GEM's bit-mask */
#define DP_FB_IDX_MASK		0x7		/* use 3 bit */
#define DP_FB_MASK		0xF		/* use 4 bit */
#define DRM_FB_IDX_MASK		DP_FB_IDX_MASK	/* it will replace DP_FB_IDX_MASK */
#define DRM_INST_MASK		DP_FB_MASK	/* it will replace DP_FB_MASK */
#define DRM_BUF_MASK		0xF		/* use 4 bit */

/* SDP-GEM flag */
#define DP_FB_MAIN		(0x0<<DRM_INST_BIT_MOVE)
#define DP_FB_SUB		(0x1<<DRM_INST_BIT_MOVE)
#define DP_FB_SUB2		(0x2<<DRM_INST_BIT_MOVE)
#define DP_FB_BGP		(0x3<<DRM_INST_BIT_MOVE)

#define DRM_DPINST_MAIN		0x0
#define DRM_DPINST_SUB		0x1
#define DRM_DPINST_SUB2		0x2
#define DRM_DPINST_BGP		0x3
#define DRM_DPMAIN_INFO		(DRM_DPINST_MAIN<<DRM_INST_BIT_MOVE)
#define DRM_DPSUB_INFO		(DRM_DPINST_SUB<<DRM_INST_BIT_MOVE)
#define DRM_DPSUB2_INFO		(DRM_DPINST_SUB2<<DRM_INST_BIT_MOVE)
#define DRM_DPBGP_INFO		(DRM_DPINST_BGP<<DRM_INST_BIT_MOVE)

#define DRM_ENCINST(x)		(x)
#define DRM_ENCINST_INFO(x)	(DRM_ENCINST((x))<<DRM_INST_BIT_MOVE)

#define DRM_BUF_Y		0x0
#define DRM_BUF_C_CB		0x1
#define DRM_BUF_CR		0x2
#define DRM_BUF_Y_INFO		(DRM_BUF_Y<<DRM_BUF_BIT_MOVE)
#define DRM_BUF_C_CB_INFO	(DRM_BUF_C_CB<<DRM_BUF_BIT_MOVE)
#define DRM_BUF_CR_INFO		(DRM_BUF_CR<<DRM_BUF_BIT_MOVE)

/**
 * A structure for buffer creation
 *
 * @size: user-desired memory allocation size
 * @flags: for memory type
 * @handle: returned a handle to created gem object
 */
struct sdp_drm_gem_create {
	unsigned long long size;
	unsigned int flags;
	unsigned int handle;
};

/**
 * A structure for buffer creation
 *
 * @size: user-desired memory allocation size
 * @flags: for memory type
 * @handle: returned a handle to created gem object for HW
 */
struct sdp_drm_gem_hw_create {
	enum sdp_drm_gem_hw_mem_flag e_flag;
	unsigned int userinfo;
	unsigned int handle;
	unsigned long long ret_size;
};

/**
 * A structure for mapping buffer
 *
 * @handle: a handle to gem object created
 * @pad: for just padding 64bit aligned
 * @size: memory size to be mapped
 * @mapped: having user virtual address mmaped
 */
struct sdp_drm_gem_mmap {
	unsigned int handle;
	unsigned int pad;
	unsigned long long size;
	unsigned long long mapped;
};

enum sdp_drm_onoff_func {
	DRM_SDP_FUNC_SYNC_ONOFF,
	DRM_SDP_FUNC_MUTE_ONOFF,
	DRM_SDP_FUNC_STILL_ONOFF,
	DRM_SDP_FUNC_LETTER_BOX_DETECT_ONOFF,
	DRM_SDP_FUNC_GAMEMODE_ONOFF,
	DRM_SDP_FUNC_DP_PC_SHARPNESS_ONOFF,
	DRM_SDP_FUNC_MAX
};

struct sdp_drm_onoff_param {
	unsigned int plane_id;
	enum sdp_drm_onoff_func	set_func;

	unsigned int onoff_flag;
};

/* SDP DRM BUF. */
enum sdp_drm_dp_bufmode {
	DRM_SDP_DP_BUF_Y_CBCR,
	DRM_SDP_DP_BUF_Y_CB_CR,
	DRM_SDP_DP_BUF_ARGB,
	DRM_SDP_DP_BUF_A_R_G_B,
	DRM_SDP_DP_BUF_MAX
};

/** will remove **/
struct sdp_drm_buf_addr {
	unsigned int addr[4];
	unsigned int size[4];
	enum sdp_drm_dp_bufmode buf_mod;
};

struct sdp_drm_buf {
	struct sdp_drm_buf_addr addr_info;
	unsigned int u_width;
	unsigned int u_height;
};

struct sdp_drm_dp_fb_addr {
	struct sdp_drm_buf_addr *info;
	unsigned int u_cur_color_fmt;
	unsigned int u_cur_width;
	unsigned int u_cur_height;
	unsigned int u_cur_y_bps;
	unsigned int u_cur_c_bps;
};

struct sdp_drm_dp_fb_c_size {
	unsigned int u_color_format;
	unsigned int u_c_byteperline;
	unsigned int u_c_right_buf_offset;
};

struct sdp_drm_dp_fb_info {
	unsigned int u_max_width;
	unsigned int u_max_height;
	unsigned int b_use_stereoscopic;	/* using flag 3D */
	unsigned int u_num_buffer;

	/* Y : byte per line*/
	unsigned int u_y_byteperline;
	unsigned int u_y_right_buf_offset;

	/* C : byte per line*/
	struct sdp_drm_dp_fb_c_size s_c_linesize[4];
};

struct sdp_drm_dp_fb_info_param {
	unsigned int plane_id;
	struct sdp_drm_dp_fb_info s_ret_info;
};
/*******************************************/

/*** IOCTL : Capture ---- will remove ***/
struct sdp_drm_capture_info {
	unsigned int CpWrHSize;	/* width */
	unsigned int CpWrVSize;	/* height */
	unsigned int CpWrYLineSize;	/* line size with padding 24 */
	unsigned int CpWrCLineSize;	/* line size with padding 24 */
	unsigned int tColorFormat;	/* captured data's format */
	unsigned int u_is_flipped;	/* if flipped 1 otherwise 0 */
	enum sdp_drm_dp_bufmode bufmode;   /* capture buffer mode */
};

enum sdp_drm_capture_type {
	DRM_SDP_CAPTURE_SCREEN,
	DRM_SDP_CAPTURE_VIDEO_ONLY,
	DRM_SDP_CAPTURE_VIDEO_0,
	DRM_SDP_CAPTURE_VIDEO_1,
	DRM_SDP_CAPTURE_VIDEO_2,
	DRM_SDP_CAPTURE_ENCODER_INPUT,
	DRM_SDP_CAPTURE_VIDEO_INPUT,
	DRM_SDP_CAPTURE_MAX
};

/* req_width - requested width : different with result's width */
/* req_height - requested height : different with result's height */
union sdp_drm_capt_instance {
	enum sdp_drm_plane_type dp;
	uint32_t enc;
};

struct sdp_drm_capture {
	enum sdp_drm_capture_type type;
	struct sdp_drm_capture_info ret_data;
	unsigned int req_width;	
	unsigned int req_height;
	union sdp_drm_capt_instance inst;
	unsigned int reserved;
};
/**********************************/

/*** IOCTL : resume data ***/
enum sdp_drm_resume {
	DRM_SDP_RESUME_GAMMA,
	DRM_SDP_RESUME_MAX
};

struct sdp_drm_resume_param {
	enum sdp_drm_resume e_data;
	unsigned int data_size;
	void *data;
};
/****************************/

/*** IOCTL : send output's data ***/
enum sdp_drm_outdata_type {
	DRM_SDP_OUTDATA_VBI_CC,/** Closed Caption */
	DRM_SDP_OUTDATA_VBI_TTX,/** Analog SdUserData is Teletext  */
	DRM_SDP_OUTDATA_VBI_WSS,/** Analog SdUserData is WSS  */
	DRM_SDP_OUTDATA_VBI_VPS,/** Analog SdUserData is VPS  */
	DRM_SDP_OUTDATA_VBI_APS,/** Analog Protection System  */
	DRM_SDP_OUTDATA_VBI_CGMS,/** Copy Generation Management System  */
	DRM_SDP_OUTDATA_VBI_CGMS_B,/** Copy Generation Management System (Type B)  */
	DRM_SDP_OUTDATA_MAX
};

struct sdp_drm_out_send_data {
	enum sdp_drm_output_type out_type;
	enum sdp_drm_outdata_type data_type;
	unsigned int data_size;
	void *data;
};
/**********************************/

/*** IOCTL : Get-status about HW ***/
/* AUTO-VIEW PATTERN DECTECTION */
struct sdp_drm_status_apd_info {
	unsigned int avg_ce_histogram;
	unsigned int threshold;
	unsigned int pattlevel;
};

/* Frame Buffer Info. */
struct sdp_drm_status_fb_param {
	unsigned int plane_id;
};

struct sdp_drm_status_fb_info {
	/* buffer width, height */
	unsigned int max_width;
	unsigned int max_height;

	/* number of buffer for 2D */
	unsigned int num_buffer;
	
	/* 1~ : support 3d and mean number of buffer for 3D, 0 : not support */
	unsigned int num_3d_buffer;

	/* stereo-scopic : 3D Jpeg Buffer mode */
	enum sdp_drm_3d_mode out_3dmode;
};

/* Frame Buffer Size Info. */
struct sdp_drm_status_fb_sz_param {
	unsigned int plane_id;
	unsigned int color_fmt;
};

struct sdp_drm_status_fb_sz_info {
	enum sdp_drm_dp_bufmode buf_mod;
	unsigned int buf_size[4];
	unsigned int buf_byteperline[4];
	unsigned int buf_r_offset[4];
};

/* HW Info about plane */
struct sdp_drm_status_plane_param {
	unsigned int plane_id;
};

struct sdp_drm_status_plane_info {
	unsigned char mute_stat;	/* mute on : 1, mute off : 0 */
	unsigned char sync_stat;	/* sync on : 1, sync off : 0 */
};

/* LETTER-BOX DECTECTION INFO */
struct sdp_drm_status_lbd_param {
	unsigned int plane_id;
};

struct sdp_drm_status_lbd_info {
	unsigned char detect_usr_on;	/* user setting on/off */
	unsigned char letterbox_detect;	/* letter-box existence detection(1,0) */
	unsigned char logo_detect;	/* logo existence detection(1,0) */
	unsigned char caption_detect;	/* caption existence detection(1,0) */
	unsigned short blank_line_cnt;	/* blank line count */
	unsigned short lower_line_cnt;	/* lower line count */
};

/* Delay value */
struct sdp_drm_status_delay_param {
	unsigned int plane_id;		/* drm plane id */
	unsigned int hwdec_info;	/* hw_decoder info from v4l2 */
	unsigned int src_hres;		/* src - h-resolution */
	unsigned int src_vres;		/* src - v-resolution */
	unsigned short src_framerate;	/* src - frame rate * 1000 */
	unsigned char src_progressive;	/* src - progressive[1]/interlace[0] */
	enum sdp_drm_src_3dmode src_3d;	/* src - 3D(Stereo-scopic) mode */
	unsigned char is_total_sum;	/* is getting Total Delay??? */
};

struct sdp_drm_status_delay_info {
	unsigned long long dalay_val;	/* display : dalay */
};

/* status's data pointer about HW */
union sdp_drm_status_param {
	struct sdp_drm_status_fb_param		*fb;
	struct sdp_drm_status_fb_sz_param	*fb_sz;
	struct sdp_drm_status_plane_param	*plane;
	struct sdp_drm_status_lbd_param		*lbd;
	struct sdp_drm_status_delay_param	*delay;
};

union sdp_drm_status_data {	
	struct sdp_drm_status_apd_info		*apd;
	struct sdp_drm_status_fb_info		*fb;
	struct sdp_drm_status_fb_sz_info	*fb_sz;
	struct sdp_drm_status_plane_info	*plane;
	struct sdp_drm_status_lbd_info		*lbd;
	struct sdp_drm_status_delay_info	*delay;
};

enum sdp_drm_status {
	DRM_SDP_STATUS_AUTO_PATT_DETECT_INFO,
	DRM_SDP_STATUS_FB_INFO,
	DRM_SDP_STATUS_FB_SIZE,
	DRM_SDP_STATUS_PLANE,
	DRM_SDP_STATUS_LETTERBOX_DETECT_INFO,
	DRM_SDP_STATUS_DELAY_INFO,
	DRM_SDP_STATUS_MAX
};

struct sdp_drm_get_status {
	enum sdp_drm_status type;
	union sdp_drm_status_param param;
	union sdp_drm_status_data data;
};
/***************************/

/* QPI Function */
enum sdp_drm_qpi_fn {
	DRM_SDP_QPI_FN_CHK_VX1_LOCK,		/* no param */
	DRM_SDP_QPI_FN_PRE_WORK,
	DRM_SDP_QPI_FN_POST_WORK,
	DRM_SDP_QPI_FN_GET_VX1_STRENGTH,
	DRM_SDP_QPI_FN_SET_VX1_STRENGTH,
	DRM_SDP_QPI_FN_GET_FLVDS_CNT_PEN_HSTRENGTH,	/* param : chip_num */
	DRM_SDP_QPI_FN_SET_FLVDS_STRENGTH,		
	DRM_SDP_QPI_FN_CHK_OSC,				/* param : num_chk */
	DRM_SDP_QPI_FN_GET_TCON_STATUS,
	DRM_SDP_QPI_FN_MAX
};

enum sdp_drm_qpi_pre_post_fn {
	DRM_SDP_QPI_PPFN_CHK_DDR,
	DRM_SDP_QPI_PPFN_CHK_VX1,
	DRM_SDP_QPI_PPFN_CHK_DPSYSTEM_FB,
	DRM_SDP_QPI_PPFN_CHK_DPSYSTEM_POST,
	DRM_SDP_QPI_PPFN_CHK_FLVDS_TX,
	DRM_SDP_QPI_PPFN_CHK_FLVDS_RX,
	DRM_SDP_QPI_PPFN_MAX
};

struct sdp_drm_get_vx1_strength_param {
	unsigned int chip_num;
	unsigned int lane;
};

struct sdp_drm_set_vx1_strength_param {
	unsigned int chip_num;
	unsigned int lane;
	unsigned int strength;
};

struct sdp_drm_set_flvds_strength_param {
	unsigned int chip_num;
	unsigned int strength;
};

union sdp_drm_qpi_param {
	unsigned int chip_num;		/* 0 : normal, 1 : special(ex)US-Chip) */
	unsigned int num_chk;
	enum sdp_drm_qpi_pre_post_fn 		pre_post_fn;
	struct sdp_drm_set_vx1_strength_param	*set_vx1;
	struct sdp_drm_get_vx1_strength_param	*get_vx1;
	struct sdp_drm_set_flvds_strength_param *set_flvds;
};

union sdp_drm_qpi_value {
	unsigned int err_value;
	unsigned int value;
};

struct sdp_drm_qpi {
	enum sdp_drm_qpi_fn	fn;
	union sdp_drm_qpi_param param;
	union sdp_drm_qpi_value ret;
	unsigned int reserved[2];
};

/*** IOCTL : QPI CRC ***/
#define MAX_CRC_SIZE		30
#define MAX_POST_CRC		6
#define MAX_INPUT_CRC		4

struct sdp_drm_qpi_out_crc {
	unsigned int a_out_1_r[MAX_CRC_SIZE];
	unsigned int a_out_1_g[MAX_CRC_SIZE];
	unsigned int a_out_1_b[MAX_CRC_SIZE];
	unsigned int a_out_2_r[MAX_CRC_SIZE];
	unsigned int a_out_2_g[MAX_CRC_SIZE];
	unsigned int a_out_2_b[MAX_CRC_SIZE];
};

struct sdp_drm_qpi_dst_crc {
	unsigned int a_luma_top[MAX_CRC_SIZE];
	unsigned int a_chrome_top[MAX_CRC_SIZE];
};

struct sdp_drm_qpi_post_crc {
	unsigned int a_post[MAX_POST_CRC][MAX_CRC_SIZE];
};

struct sdp_drm_qpi_input_crc {
	unsigned int a_input[MAX_INPUT_CRC][MAX_CRC_SIZE];
};

struct sdp_drm_qpi_crc {
	unsigned int u_param_cnt;
	union {
		struct out_crc_param {
			unsigned int u_reserved;
		} out;

		struct dst_crc_param {
			unsigned int u_test_nrfcmode;
			/* Test NRFC : Do Not[0], Do [1~] */
		} dst;

		struct post_crc_param {
			unsigned char chip_num;
			unsigned char reserved[3];
		} post;
		
		struct input_crc_param {
			unsigned char chip_num;
			unsigned char reserved[3];
		} input;
	} param;
	union {
		struct sdp_drm_qpi_out_crc s_out;
		struct sdp_drm_qpi_dst_crc s_dst;
		struct sdp_drm_qpi_input_crc input;
		struct sdp_drm_qpi_post_crc post;
	} rslt_crc;
};

struct sdp_drm_qpi_input_pattern {
	enum sdp_drm_plane_type plane_type;
	unsigned int onoff_flag;
	unsigned int pattern_type;
	unsigned int chip_num;	/* normal : 0, specially : 1~ (ex)US - 1 */
};

struct sdp_drm_qpi_incapt_clock_sel {
	enum sdp_drm_plane_type plane_type;
	unsigned int clksel;
	unsigned int invert;
	unsigned int delay;
};

struct sdp_drm_qpi_gfx_crc {
    unsigned int crc_a_val;
    unsigned int crc_r_val;
    unsigned int crc_g_val;
    unsigned int crc_b_val;
};
/**********************************/

/********************************
* DRM ADDED IOCTL = MAX :: 0x60 *
*********************************/
#define DRM_SDP_GEM_CREATE		0x00
#define DRM_SDP_GEM_CREATE_HWMEM	0x01
#define DRM_SDP_SET_DP_SOURCE		0x11
#define DRM_SDP_CAPTURE			0x14
#define DRM_SDP_SET_ONOFF		0x15
#define DRM_SDP_GET_FB_INFO		0x16
#define DRM_SDP_SET_RESUME_DATA		0x17
#define DRM_SDP_OUTPUT_SEND_DATA	0x18
#define DRM_SDP_GET_STATUS		0x19
#define DRM_SDP_GA_EXEC			0x40
#define DRM_SDP_GEM_CACHE_OP            0x41

#define DRM_IOCTL_SDP_GEM_CREATE	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GEM_CREATE,struct sdp_drm_gem_create)
#define DRM_IOCTL_SDP_GEM_CREATE_HWMEM	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GEM_CREATE_HWMEM, struct sdp_drm_gem_hw_create)
#define DRM_IOCTL_SDP_SET_DP_SOURCE	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_SET_DP_SOURCE,struct v4l2_drm)
/* ---- will remove */		
#define DRM_IOCTL_SDP_CAPTURE		DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_CAPTURE, struct sdp_drm_capture)
			
#define DRM_IOCTL_SDP_SET_ONOFF		DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_SET_ONOFF,struct sdp_drm_onoff_param)
#define DRM_IOCTL_SDP_GET_FB_INFO	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GET_FB_INFO, struct sdp_drm_dp_fb_info_param)
#define DRM_IOCTL_SDP_SET_RESUME_DATA	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_SET_RESUME_DATA,struct sdp_drm_resume_param)
#define DRM_IOCTL_SDP_OUTPUT_SEND_DATA	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_OUTPUT_SEND_DATA, struct sdp_drm_out_send_data)
#define DRM_IOCTL_SDP_GET_STATUS	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GET_STATUS, struct sdp_drm_get_status)
#define DRM_IOCTL_SDP_GA_EXEC		DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GA_EXEC, struct sdp_drm_ga_exec)
#define DRM_IOCTL_SDP_GEM_CACHE_OP DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GEM_CACHE_OP,struct sdp_drm_gem_cache_op)

/* QPI : for Testing in Factory */
#define DRM_SDP_QPI_GFX_CRC_PF_TEST		0x48
#define DRM_SDP_QPI_GET_GFX_CRC			0x49
#define DRM_SDP_QPI_GET_DST_CRC			0x50
#define DRM_SDP_QPI_GET_OUT_CRC			0x52
#define DRM_SDP_QPI_SET_INPUT_TEST_PATTERN	0x53
#define DRM_SDP_QPI_SET_INCAPT_CLOCK_SEL	0x54
#define DRM_SDP_QPI_SET_READY_GET_OUT_CRC	0x55
#define DRM_SDP_QPI_SET_GP_SYNCONOFF		0x56
#define DRM_SDP_QPI_GET_INPUT_CRC		0x57
#define DRM_SDP_QPI_GET_POST_CRC		0x58
#define DRM_SDP_QPI_FUNC			0x59

#define DRM_IOCTL_SDP_QPI_GET_DST_CRC	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_GET_DST_CRC,\
				struct sdp_drm_qpi_crc)
#define DRM_IOCTL_SDP_QPI_GET_OUT_CRC	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_GET_OUT_CRC,\
				struct sdp_drm_qpi_crc)
#define DRM_IOCTL_SDP_QPI_SET_INPUT_TEST_PATTERN	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_SET_INPUT_TEST_PATTERN,\
				struct sdp_drm_qpi_input_pattern)
#define DRM_IOCTL_SDP_QPI_SET_INCAPT_CLOCK_SEL	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_SET_INCAPT_CLOCK_SEL,\
				struct sdp_drm_qpi_incapt_clock_sel)
#define DRM_IOCTL_SDP_QPI_SET_READY_GET_OUT_CRC	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_SET_READY_GET_OUT_CRC,\
				unsigned int)
#define DRM_IOCTL_SDP_QPI_SET_GP_SYNCONOFF	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_SET_GP_SYNCONOFF,\
				unsigned int)
#define DRM_IOCTL_SDP_QPI_GET_INPUT_CRC		\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_GET_INPUT_CRC,\
				struct sdp_drm_qpi_crc)
#define DRM_IOCTL_SDP_QPI_GET_POST_CRC		\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_GET_POST_CRC,\
				struct sdp_drm_qpi_crc)
#define DRM_IOCTL_SDP_QPI_FUNC			\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_FUNC,\
				struct sdp_drm_qpi)

#define DRM_IOCTL_SDP_QPI_GFX_CRC_PF_TEST	\
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_GFX_CRC_PF_TEST,\
				unsigned int)
#define DRM_IOCTL_SDP_QPI_GET_GFX_CRC	    \
		DRM_IOWR(DRM_COMMAND_BASE + DRM_SDP_QPI_GET_GFX_CRC,\
				struct sdp_drm_qpi_gfx_crc)
/**************************************************************/
/** DRM Capture                                              **/
/**************************************************************/
/**** Capture *****************************/
/*** IOCTL ***/
enum sdp_drm_capture_pos {
	DRM_SDP_CAPTURE_POST,
	DRM_SDP_CAPTURE_INPUT,
	DRM_SDP_CAPTURE_ENCODER,
	DRM_SDP_CAPTURE_POS_MAX
};

union sdp_drm_capture_inst {
	enum sdp_drm_plane_type drm;	/* drm : drm's plane type */
	uint32_t enc;			/* end : encoder instance number */
};

struct sdp_drm_hw_capture {
	/* param */
	/**
	* @type - capture-type
	* @req_width - requested width : different with result's width
	* @req_height - requested height : different with result's height
	* @inst - instance id
	* @is_only_video - set mix-point about capture POST : 0 is screen,1 is only-video 
	*/
	enum sdp_drm_capture_pos pos;
	union sdp_drm_capture_inst inst;
	unsigned int req_width;	
	unsigned int req_height;
	unsigned int is_only_video;
	unsigned int gem_hndl[4];	/* temp : destination's gem handle */ 

	/* return's value */
	unsigned int ret_width;
 	unsigned int ret_height;
 	unsigned int ret_colorfmt;	/* captured data's format */
	unsigned int ret_pitches[4];	/* line size about buffer */
 	unsigned int ret_is_flipped;	/* if flipped 1 otherwise 0 */
	unsigned int ret_protected_on;	/* 1:set protection for buf., 0:not */
	enum sdp_drm_3d_mode ret_3dmod; /* plane or output 3d mode */
	unsigned int ret_reserved[2];	
};

/**
 * A GEM structure for capture's buffer
 */
struct sdp_drm_gem_capture {
	/* param. */
	enum sdp_drm_capture_pos pos;		
	union sdp_drm_capture_inst inst;
	unsigned char buf_info;
	unsigned int reserved;
	
	/* return's value */
	unsigned int ret_handle;
	unsigned long long ret_size;
	unsigned int ret_reserved[3];
 };

/**
 * A sync. structure for capture's buffer
 *
 * @pos: post type for capture's sync
 * @inst: instance type for capture's sync
 * @is_lock: value, 1 is lock, 0 is unlock(release)
 */
struct sdp_drm_hw_capture_sync {
	enum sdp_drm_capture_pos pos;
	union sdp_drm_capture_inst inst;
	unsigned int is_lock;
};

/*******************************************/

#define DRM_SDP_GEM_CREATE_CAPTURE	0x04
#define DRM_SDP_HW_CAPTURE		0x12
#define DRM_SDP_HW_CAPTURE_SYNC		0x20

#define DRM_IOCTL_SDP_GEM_CREATE_CAPTURE DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_GEM_CREATE_CAPTURE,struct sdp_drm_gem_capture)
#define DRM_IOCTL_SDP_HW_CAPTURE	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_HW_CAPTURE, struct sdp_drm_hw_capture)
#define DRM_IOCTL_SDP_HW_CAPTURE_SYNC	DRM_IOWR(DRM_COMMAND_BASE+\
			DRM_SDP_HW_CAPTURE_SYNC,struct sdp_drm_hw_capture_sync)

#endif /* _UAPI_SDP_DRM_H_ */
