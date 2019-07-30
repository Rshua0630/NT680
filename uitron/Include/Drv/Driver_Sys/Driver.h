/**
    Driver common part header file

    @file       Driver.h
    @ingroup    mIDriver
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _DRIVER_H
#define _DRIVER_H

/**
    @addtogroup mIDriver
*/
//@{

#include "Type.h"

typedef enum {
	DRV_VER_96650,
	DRV_VER_96660,
	DRV_VER_96680,

	ENUM_DUMMY4WORD(DRV_VER_INFO)
} DRV_VER_INFO;

/**
    Driver callback function

    uiEvent is bitwise event, please refer to each module's document.

*/
typedef void (*DRV_CB)(UINT32 uiEvent);

//#NT#2016/09/12#Lincy Lin -begin
//#NT#Porting Dual OS flow, the interrupt setting will be configured in loader
extern void     drv_set_init_int(BOOL enable);
//#NT#2016/09/12#Lincy Lin -end
extern void     drv_init(void);
extern void     drv_dumpInfo(void);
extern void     drv_dump_clk_gating_Info(void);
extern void     drv_dump_dsp_clk_Info(void);
extern void 	drv_dump_portable_clock_Info(void);
extern void     drv_dump_mmu_info(void);
extern void 	idec_dumpInfo(UINT32 id);
extern void     pwm_dumpInfo(void);

//@}

#endif
