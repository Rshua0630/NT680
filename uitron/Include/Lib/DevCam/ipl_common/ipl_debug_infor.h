#ifndef _IPH_DEBUG_H_
#define _IPH_DEBUG_H_

#include "Type.h"

extern void ipl_dump_info(void (*dump)(char *fmt, ...));
extern void ipl_dump_status(void (*dump)(char *fmt, ...));

extern void ipl_ctrl_dump_engstatus(void (*dump)(char *fmt, ...));
#endif //_IPH_DEBUG_H_
