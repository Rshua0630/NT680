/*
    Display object object Golden Sample for driving Memory interface panel

    @file       dispdev_gsinfmi.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DISPDEV_GSINFMI_H
#define _DISPDEV_GSINFMI_H

#include "sif.h"
#include "dispdev_panlcomn.h"
#include "mi.h"
#include "pll.h"

// Default debug level
#ifndef __DBGLVL__
#define __DBGLVL__  2       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
#define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"

#define CMDDELAY_MS 0x80000000
#define CMDDELAY_US 0x40000000

#define MICMD_CMD   0x01000000
#define MICMD_DATA  0x02000000




typedef struct {
	MI_DATAFMT      DataFmt;
	FLOAT           fMiSrcClk;
} tMI_CONFIG;





tLCD_PARAM *dispdev_getConfigModeMI(UINT32 *ModeNumber);
tPANEL_CMD *dispdev_getStandbyCmdMI(UINT32 *CmdNumber);

void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue);

tLCD_ROT *dispdev_getLcdRotateMiCmd(UINT32 *ModeNumber);
void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig);
void      dispdev_setMiAutoCmd(void);

#endif
