/**
    ipl calibration public header file for extend cal lib

    This file is the header file that define the API and data type for extend lib.

    @file       ipl_cal_api.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_API_H
#define _IPL_CAL_API_H

#include "ipl_utility.h"
#include "ipl_calibration.h"
#include "ipl_cal_ctrl.h"
#include "ipl_cal_buf.h"
#include "ipl_cal_rw.h"
#include "ipl_cal_obj.h"
/**
    ipl calibration extern API for project
*/

// common
extern UINT32 ipl_cal_chk_rt(ER *rt, UINT32 cnt);
// for calibration libext initial
extern ER ipl_cal_init(CAL_INIT_DATA *init_data);
extern ER ipl_cal_uninit(void);
extern ER ipl_cal_ext_init(IPL_PROC_ID id);
extern ER ipl_cal_ext_uninit(IPL_PROC_ID id);
// for calibration project (flow)
extern ER ipl_cal_open(IPL_PROC_ID id, CAL_OPEN_PARAM *open_param);
extern ER ipl_cal_close(IPL_PROC_ID id);
extern ER ipl_cal_prv(CAL_PRV_DATA *prv_data);
extern ER ipl_cal_getraw(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_RST *getraw_rst);
extern ER ipl_cal_alg(IPL_PROC_ID id, CAL_ALG_PARAM *alg_param, void *alg_rst);
// for calibration project (get calibration libext setting)
extern pCAL_EXT_CTRL_OBJ ipl_cal_ctrl(IPL_PROC_ID id);
extern pCAL_EXT_SETTING_OBJ ipl_cal_setting(IPL_PROC_ID id);

extern ER ipl_cal_get_data_buf_addr(CAL_DATA_BUF_INFO *buf_info, UINT32 *addr);

// for ipl get calibration rst from dram
extern ER ipl_cal_getcalrst(IPL_PROC_ID id, CAL_RW_HEADER_INFO *rw_header_info, UINT32 *data_addr);

extern void ipl_cal_install_id(void) _SECTION(".kercfg_text");

//@}
#endif
