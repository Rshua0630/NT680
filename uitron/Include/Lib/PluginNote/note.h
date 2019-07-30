/**
    NOTE.

    The common function for NOTE control.

    @file       NOTEe.h
    @ingroup    Lib\PluginNote
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NOTE_H_
#define _NOTE_H_
#ifdef WIN32
#define ENUM_DUMMY4WORD(a)
#endif
#include "Type.h"
//#include "NvtVerInfo.h"
#include <stdio.h>
#include "Delay.h"

#include "iq_param_ctrl.h"
#include "iq_tuning_api.h"

#include "ae_task.h"
#include "ae_alg.h"
#include "ae_api.h"

#include "awb_task.h"
#include "awb_alg.h"

extern void isp_ae_init(UINT32 Id, void *ae_status_info_ptr, void *ae_param_info_ptr, void *ae_info_ptr);
extern void isp_ae_process(UINT32 Id); // iyen note mark

extern void isp_awb_init(UINT32 Id);
extern void isp_awb_process(UINT32 Id);

extern void isp_iq_init(UINT32 id, UINT32 iqs_mode_idx);
#endif
