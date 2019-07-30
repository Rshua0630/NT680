/**
    Debug Utility Provided SxCmd

    @file       DbgUt_SxCmd.h
    @ingroup    mDBGUT

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DBGUT_CMD_H
#define _DBGUT_CMD_H
#include "SxCmd.h"

/**
     Initial

     this SxCmd have to initial before using
*/
extern BOOL SysDbgUtCmd_Init(void);

/**
     Dma Write Protection of SxCmd Entry
*/
extern SX_CMD_ENTRY *DmaWp_GetCmdEntry(void);

/**
     CPU Usage of SxCmd Entry
*/
extern SX_CMD_ENTRY *Usage_GetCmdEntry(void);

#endif