#ifndef _NVTVIDEOSPRITE_H
#define _NVTVIDEOSPRITE_H
/*
    Copyright (c) 2017~  Novatek Microelectronics Corporation

    @file nvtvideosprite.h

    @version

    @date
*/
#include <stdio.h>
#include <nvt_type.h>

#define MAX_BTN_OSD_NUM   (32)
#define MAX_EXT_OSD_NUM   (16)
#define MAX_EXT_MASK_NUM  (16)
#define MAX_PS_OSD_NUM    (4)
#define MAX_PS_MASK_NUM   (8)

#define ER_VDS_SUCCESS    (0)
#define ER_VDS_NOT_INIT   (-1)
#define ER_VDS_PARAMETER  (-2)
#define ER_VDS_SIZE       (-3)
#define ER_VDS_IPC        (-4)
#define ER_VDS_REINIT     (-5)
#define ER_VDS_NOT_EXIST  (-6)

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

extern ER nvtvideosprite_Init(unsigned int uiMaxSize);

extern ER nvtvideosprite_Uninit(void);

extern ER nvtvideosprite_AddMask(CodecOSDIMAGE *image);

extern ER nvtvideosprite_AddOsd(CodecOSDIMAGE *image);

extern ER nvtvideosprite_AllocOsdBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

extern ER nvtvideosprite_SwapExtMask(int vid);

extern ER nvtvideosprite_SwapPsMask(int vid);

extern ER nvtvideosprite_SwapBtnExtOsd(int vid);

extern ER nvtvideosprite_SwapPsOsd(int vid);

extern ER nvtvideosprite_Clear(void);

extern ER nvtvideosprite_FreeBuffer(int release);

extern ER nvtvideosprite_Defragment(void);

extern ER nvtvideosprite_DelOsd(VDS_PHASE phase, int id);

extern ER nvtvideosprite_GetImages(VDS_PHASE phase, int mask, CodecOSDIMAGE *image, int *num);

extern ER nvtvideosprite_GetLockPingPong(VDS_PHASE phase, int id, unsigned int *addr, unsigned int *size);

extern ER nvtvideosprite_UnlockPingPong(void);

extern ER nvtvideosprite_AllocPingPongBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

extern ER nvtvideosprite_AllocScaleBuf(VDS_PHASE phase, int id, unsigned int PreW, unsigned int PreH, unsigned int PostW, unsigned int PostH);

extern ER nvtvideosprite_AllocSingleBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

extern ER nvtvideosprite_AddScaleOsd(CodecOSDIMAGE *image, int ratio);

extern ER nvtvideosprite_InitMosaicBuf(unsigned int uiMaxSize);

extern ER nvtvideosprite_AllocMosaicBuf(VDS_PHASE phase, int vid, unsigned int width, unsigned int height);

extern ER nvtvideosprite_FreeMosaicBuffer(int release);

extern ER nvtvideosprite_SetVdsMosaicBlksz(VDS_PHASE phase, int vid, unsigned int blksz);

#endif /* _NVTVIDEOSPRITE_H  */
