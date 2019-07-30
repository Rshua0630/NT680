/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALIBRATIONITEM_H
#define _CALIBRATIONITEM_H

#include "Type.h"
#include "ipl_calibration.h"

extern CAL_RW_STORE_OP cal_rw_store_op;
extern void cal_item_dnp(void);
extern void cal_item_awb(void);
extern void cal_item_dpb(void);
extern void cal_item_dpd(void);
extern void cal_item_iso(void);
extern void cal_item_ecs(void);
extern void cal_item_lenscen(void);
extern void cal_item_focus(void);
extern void cal_item_test(void);

void cal_system_init(void);
void cal_system_uninit(void);

#endif

//@}
