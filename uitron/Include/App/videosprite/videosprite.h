/**
    Image operation module.

    This module support put mask and icon on video frame.

    @file       videosprite.h
    @ingroup    mIAppVideosprite

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _VIDEO_SPRITE_H_
#define _VIDEO_SPRITE_H_

#include "Type.h"
#include "GxImage.h"
#include "ime_lib.h"
#include "ipl_hal_ime_info.h"

#define VDS_SHARE_ADDR    (0)
#define VDS_REUSE_ADDR    (1)

typedef enum {
	VDS_FMT_NULL,
	VDS_FMT_MASK,            ///< mask for graph, ime
	VDS_FMT_MOSAIC,            ///< mosaic for graph, ime
	VDS_FMT_PICTURE_RGB565,      ///< rgb565 osd for coe, graph, ime
	VDS_FMT_PICTURE_ARGB1555,        ///< argb1555 osd for coe, graph, ime
	VDS_FMT_PICTURE_ARGB4444,        ///< argb4444 osdfor coe, graph, ime
	VDS_FMT_MAX,
	ENUM_DUMMY4WORD(VDS_FMT)
} VDS_FMT;

typedef enum {
	VDS_PHASE_NULL,
	VDS_PHASE_BTN,             ///< Draw by encoder
	VDS_PHASE_EXT,             ///< Draw by gximage
	VDS_PHASE_PS,              ///< Draw by image pipeline
	VDS_PHASE_MAX,
	ENUM_DUMMY4WORD(VDS_PHASE)
} VDS_PHASE;

typedef enum{
    VDS_BUF_TYPE_NULL,
    VDS_BUF_TYPE_PING_PONG,
    VDS_BUF_TYPE_SHARE,
    VDS_BUF_TYPE_SCALE,
    VDS_BUF_TYPE_SINGLE,
    VDS_BUF_TYPE_MAX,
    ENUM_DUMMY4WORD(VDS_BUF_TYPE)
}VDS_BUF_TYPE;

typedef struct _VDS_BTN_DATA {
	UINT32  vid;     ///< stream id
	UINT32  en;      ///< display:1 un-display:0
	UINT32  layer;       ///< 1, 2
	UINT32  region;      ///< 1 ~ 16
	VDS_FMT fmt;     ///< VDS_FMT_PICTURE_RGB565  VDS_FMT_PICTURE_ARGB1555 VDS_FMT_PICTURE_ARGB4444
	UINT32  addr;        ///< rgb buffer
	UINT32  bid;
	UINT32  x;       ///< x in pixel
	UINT32  y;       ///< y in pixel
	UINT32  w;       ///< width in pixel
	UINT32  h;       ///< height in pixel
	UINT16  ckey_en;     ///< color key enable
	UINT16  ckey_val;    ///< color key value
	UINT32  qp_en;       ///< Optional. Default 0. Refer to coe document for detailed information
	UINT32  qp_fix;      ///< Optional. Default 0. Refer to coe document for detailed information
	UINT8   qp_val;      ///< Optional. Default 0. Refer to coe document for detailed information
	UINT8   alpha;       ///< Optional. Default 0. Refer to coe document for detailed information
} VDS_BTN_DATA;

typedef struct _VDS_EXT_DATA {
	UINT32  vid;     ///< stream id
	VDS_FMT fmt;     ///< VDS_FMT_MASK VDS_FMT_PICTURE_RGB565  VDS_FMT_PICTURE_ARGB1555 VDS_FMT_PICTURE_ARGB4444
	UINT32  en;      ///< display:1 un-display:0
	UINT32  addr;        ///< rgb buffer. VDS_FMT_MASK doesn't use this field
	UINT32  bid;
	UINT32  x;       ///< x in pixel
	UINT32  y;       ///< y in pixel
	UINT32  w;       ///< width in pixel
	UINT32  h;       ///< height in pixel
	UINT32  color;       ///< color for VDS_FMT_MASK
	UINT32  alpha;       ///< Optional. Default 0. Refer to graph document for detailed information
} VDS_EXT_DATA;

typedef struct _VDS_PS_DATA {
	UINT32  vid;     ///< stream id
	VDS_FMT fmt;     ///< VDS_FMT_MASK VDS_FMT_PICTURE_RGB565  VDS_FMT_PICTURE_ARGB1555 VDS_FMT_PICTURE_ARGB4444
	UINT32  en;      ///< display:1 un-display:0
	UINT32  addr;        ///< rgb buffer. VDS_FMT_MASK doesn't use this field
	UINT32  bid;
	UINT32  x[4];        ///< [0] for osd ; [0] ~ [3] for FT_MASK
	UINT32  y[4];        ///< [0] for osd ; [0] ~ [3] for FT_MASK
	UINT32  w;       ///< width in pixel. valid for osd
	UINT32  h;       ///< height in pixel. valid for osd
	UINT32  color;       ///< valid for VDS_FMT_MASK
	UINT32  aweight;     ///< Optional. Default 0. Refer to ime document for detailed information
	UINT32  ckey_en;     ///< Optional. Default 0. Refer to ime document for detailed information
	UINT32  ckey_val;    ///< Optional. Default 0. Refer to ime document for detailed information
	UINT16  bweight0;    ///< Optional. Default 0. Refer to ime document for detailed information
	UINT16  bweight1;    ///< Optional. Default 0. Refer to ime document for detailed information
} VDS_PS_DATA;

typedef struct _CodecOSDIMAGE {
	unsigned int  id;        ///< serial number.
	VDS_PHASE     phase;     ///< VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS

	union {
		VDS_BTN_DATA   btn;
		VDS_EXT_DATA   ext;
		VDS_PS_DATA    ps;
	} data;             ///< configuration data

} CodecOSDIMAGE;

#ifdef __cplusplus
extern "C" {
#endif

extern UINT32 _SECTION(".kercfg_data") VIDEOSPRITE_TSK_ID;
extern UINT32 _SECTION(".kercfg_data") VIDEOSPRITE_SEM;

extern void Videosprite_InstallID(void);
extern int  videosprite_open(void);
extern int  videosprite_get_osd_list(VDS_PHASE phase, unsigned int *addr, unsigned int *len, unsigned int *num);
extern int  videosprite_query_size(VDS_PHASE phase, VDS_BUF_TYPE type, unsigned int width, unsigned int height);
extern int  videosprite_config_ping_pong_buf(VDS_PHASE phase, int id, unsigned int addr, unsigned int size);
extern int  videosprite_config_scale_buf(VDS_PHASE phase, int id, unsigned int addr1, unsigned int size1, unsigned int addr2, unsigned int size2);
extern int  videosprite_config_single_buf(VDS_PHASE phase, int id, unsigned int addr, unsigned int size);
extern int  videosprite_query_mosaic_size(VDS_PHASE phase, unsigned int width, unsigned int height);
extern int  videosprite_config_mosaic_buf(VDS_PHASE phase, int vid, unsigned int addr, unsigned int size);
extern int videosprite_query_bookkeep_size(void);
extern int videosprite_set_bookkeep_buf(unsigned int addr, int size);

extern int  videomask_add(CodecOSDIMAGE *image);
extern int  videoimage_add(CodecOSDIMAGE *image);
extern void videosprite_lock(void);
extern void videosprite_release(void);
extern void videomask_swap_ext(int vid);
extern void videoimage_swap_btn_ext(int vid);
extern void videomask_swap_ps(int vid);
extern void videoimage_swap_ps(int vid);
extern int  videoimage_delete(VDS_PHASE phase, int id);
extern int  videoimage_get_free_ping_pong_buffer(VDS_PHASE phase, int id, unsigned int *addr, unsigned int *size);

extern int  videosprite_reset(void);

extern void videosprite_draw_btn_ext(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth, UINT32 imgHeight);
extern void videosprite_draw_ext(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth, UINT32 imgHeight);
extern void videosprite_draw_ps_mask(IME_PRI_MASK_INFO* maskinfo, UINT32 ipl_id, UINT32 w, UINT32 h, UINT32 resv);
extern void videosprite_draw_ps_image(IMESTAMP_INFO* osdinfo, UINT32 ipl_id, UINT32 w, UINT32 h, UINT32 resv);

extern void         videosprite_suspend(VDS_PHASE phase, int id);
extern void         videosprite_resume(VDS_PHASE phase, int id);
extern unsigned int videosprite_is_suspend(void);
extern int          videosprite_defragment(void);
extern int          videosprite_config_scale_ratio(VDS_PHASE phase, int id, int ratio);
extern int          videosprite_config_mosaic_blksz(VDS_PHASE phase, int vid, int blksz);

extern void xVideoSprite_InstallCmd(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_VIDEO_SPRITE_H_

