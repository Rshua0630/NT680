/**
    Public header file for ipl obj

    This file is the header file that define the API and data type for ipl ctrl obj.

    @file       ipl_obj.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPL_OBJ_H
#define _IPL_OBJ_H

#include "Type.h"
#include "sensor.h"

#define IPL_MAX 8

/**
    vdo out information
*/
typedef struct {
	UINT32 ImgSize;     ///< Video SIZE (H_SIZE:bit[31:16], V_SIZE:bit[15:0])
	UINT32 ImgFps;		///< Video FPS [31:16]/[15:0](src:bit[31:16], div:bit[15:0])
} VDO_OUT_INFO;

/**
    extend library common interface
*/
typedef struct {
	//Common
	ER (*fp_get_sensor_info)(UINT32 id, UINT32 ipl_mode, SENSOR_INFO *sen_info);
	UINT32 *(*fp_chgmode)(UINT32 cur_ipl_mode, UINT32 next_ipl_mode);
	UINT32 *(*fp_senmode2tbl)(UINT32 id, UINT32 *dz_max_idx);
} IPL_EXT_LIB_TAB;

typedef struct {
	IPL_EXT_LIB_TAB *ext_lib_tab;
} IPL_HANDLE;

extern BOOL ipl_ext_lib_repalce_lib(UINT32 id, CHAR *specified_name);
extern BOOL ipl_ext_lib_init(UINT32 id, IPL_HANDLE *handle);
extern CHAR* ipl_ext_lib_get_name(UINT32 id);
//@}
#endif
