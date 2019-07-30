/*
    Copyright (c) 2017  Novatek Microelectronics Corporation

    @file VideoSpriteIpc.h

    @version

    @date
*/

#ifndef _VIDEOSPRITEIPC_H
#define _VIDEOSPRITEIPC_H

#include "Type.h"

#define NVTVDSIPC_RET_OK                       0
#define NVTVDSIPC_RET_ERR                      (-1)

extern void VideoSpriteIpc_InstallID(void) _SECTION(".kercfg_text"); 

extern ER VideoSpriteIpc_Open(void);

extern ER VideoSpriteIpc_Close(void);

#endif /* _VIDEOSPRITEIPC_H  */
