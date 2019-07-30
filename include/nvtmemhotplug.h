#ifndef _NVTMEMHOTPLUG_H
#define _NVTMEMHOTPLUG_H

/*
    Copyright (c) 2017~  Novatek Microelectronics Corporation

    @file nvtmemhotplug.h

    @version

    @date
*/

#include <stdio.h>
#include <nvt_type.h>

#ifdef __cplusplus
extern "C" {
#endif



#define PRINT_OUTFILE stderr

#define CHKPNT                   fprintf(PRINT_OUTFILE, "\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__) ///< Show a color sting of line count and function name in your insert codes
#define DBG_WRN(fmtstr, args...) fprintf(PRINT_OUTFILE, "\033[33m%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) fprintf(PRINT_OUTFILE, "\033[31m%s(): \033[0m" fmtstr,__func__, ##args)


#ifdef DEBUG
#define DBG_IND(fmtstr, args...) fprintf(PRINT_OUTFILE, "%s(): " fmtstr, \
    __func__, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif

#define DBG_DUMP(fmtstr, args...) fprintf(PRINT_OUTFILE, fmtstr, ##args)



typedef struct{

    UINT32 uiAddr;
    UINT32 uiSize;

}HotPlugInfo;

extern INT32 MemHotPlug(void);


#ifdef __cplusplus
}
#endif
#endif
