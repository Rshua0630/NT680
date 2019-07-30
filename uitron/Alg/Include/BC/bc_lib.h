#ifndef _BC_LIB_H_
#define _BC_LIB_H_

#include "stdio.h"
#include "Type.h"
#include "GxImage.h"

typedef enum
{
	BC_RESET,
	BC_RSLT_NEW,
	BC_UPDATE_PROB,
	BC_UPDATE_NEIGHBOR_EN,
	BC_DEGHOST_EN,
	BC_MAX_FG_FRM_NUM,
	BC_MIN_OBJ_AREA_PCT,
	BC_TH_CROSS_FRM_NUM,
	BC_TH_FEATURE,
	BC_SENSI,
	BC_IMG_HEIGHT,
	BC_IMG_WIDTH,
	ENUM_DUMMY4WORD(BC_USR_PRMS)
}BC_USR_PRMS;

typedef enum
{
	BC_BLK_NUM_H,
	BC_BLK_NUM_W,
	BC_MIN_OBJ_BLK_NUM,
	BC_FG_PCT_PER_BLK,
	ENUM_DUMMY4WORD(BC_BLK_PRMS)
}BC_BLK_PRMS;

typedef enum
{
	BC_TAMPER_PCT,
	BC_TAMPER_FRM_NUM,
	BC_TD_STATUS,
	BC_TAMPER_PCT_CUR,
	BC_TAMPER_FRM_NUM_CUR,
	ENUM_DUMMY4WORD(BC_TD_PRMS)
}BC_TD_PRMS;

typedef struct {
	UINT32  bc_width;
	UINT32  bc_height;
	UINT8  bc_reset;
	UINT8  bc_rslt_new;
	UINT8  bc_update_prob;
	UINT8  bc_update_neighbor_en;
	UINT8  bc_deghost_en;
	UINT32 bc_max_fg_frm_num;
	UINT32 bc_min_obj_area_pct;
	UINT32 bc_th_cross_frm_num;
	UINT8  bc_th_feature;
	UINT8  bc_sensi;
	UINT32 reserved[6];
} BC_USR_PARAM;

VOID bc_dsp_enable(UINT32 enable);
VOID bc_init(UINT32 buf, UINT32 cachebuf);
VOID bc_setimg(IMG_BUF *img);
VOID bc_process(VOID);
UINT32 bc_getcachebufsize(VOID);
UINT32 bc_getbufsize(VOID);


VOID bc_setprms(BC_USR_PRMS name, UINT32 val);
UINT32 bc_getprms(BC_USR_PRMS name);
UINT32 bc_gettarget(URECT* target, URECT* targetcoord, UINT32 maxnum);

VOID bc_setblkprms(BC_BLK_PRMS name, UINT32 val);
UINT32 bc_getblkprms(BC_BLK_PRMS name);
UINT32 bc_getblktarget(URECT* target, URECT* targetcoord, UINT32 maxnum);

VOID bc_settdprms(BC_TD_PRMS name, UINT32 val);
UINT32 bc_gettdprms(BC_TD_PRMS name);

VOID bc_getforeground(IMG_BUF *fg);
VOID bc_getforeground_v2(UINT8 *fg, UINT32 fgw, UINT32 fgh, UINT8 fgval, UINT8 bgval);
VOID bc_getbackground(IMG_BUF *bg);



#endif

