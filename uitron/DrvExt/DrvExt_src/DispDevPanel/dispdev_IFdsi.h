/*
    Display object object Golden Sample for driving Memory interface panel

    @file       dispdev_gsinfmi.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DISPDEV_GSINFDSI_H
#define _DISPDEV_GSINFDSI_H

#include "dispdev_panlcomn.h"
#include "dsi.h"
#include "pll.h"

#define CMDDELAY_MS         0x80000000
#define CMDDELAY_US         0x40000000
#define CMDTEON             0x20000000
#define CMDTEEXTPIN         0x10000000
#define CMDTEONEACHFRAME    0x04000000
#define CMDBTA              0x08000000
#define CMDTEONEACHFRAMERTY 0x00800000
#define CMDBTAONLY               0x00400000
#define CMDTEONEACHFRAMERTYTWICE 0x00200000

#define DSICMD_CMD          0x01000000
#define DSICMD_DATA         0x02000000




typedef struct {
	DSI_PIXEL_FORMATSEL     pixelFmt;
} tDSI_CONFIG;





tLCD_PARAM *dispdev_getConfigModeDSI(UINT32 *ModeNumber);
tPANEL_CMD *dispdev_getStandbyCmdDSI(UINT32 *CmdNumber);


tLCD_ROT *dispdev_getLcdRotateDSICmd(UINT32 *ModeNumber);
void      dispdev_setDSIConfig(tDSI_CONFIG *pDsiConfig);

#endif
