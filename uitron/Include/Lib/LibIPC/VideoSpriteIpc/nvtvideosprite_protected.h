/*
    Copyright (c) 2017  Novatek Microelectronics Corporation

    @file VideoSpriteIpc.h

    @version

    @date
*/

#ifndef _VIDEOSPRITECODEC_H
#define _VIDEOSPRITECODEC_H

extern ER nvtvideosprite_Init(unsigned int uiMaxSize);

extern ER nvtvideosprite_Uninit(void);

extern ER nvtvideosprite_AddMask(CodecOSDIMAGE *image);

extern ER nvtvideosprite_AddOsd(CodecOSDIMAGE *image);

extern ER nvtvideosprite_AllocOsdBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

extern ER nvtvideosprite_SwapExtMask(int vid);

extern ER nvtvideosprite_SwapPsMask(int vid);

extern ER nvtvideosprite_SwapBtnExtOsd(int vid);

extern ER nvtvideosprite_SwapPsOsd(int vid);

extern ER nvtvideosprite_Clear(void);

extern ER nvtvideosprite_FreeBuffer(int release);

extern ER nvtvideosprite_DelOsd(VDS_PHASE phase, int id);

extern ER nvtvideosprite_GetImages(VDS_PHASE phase, int mask, CodecOSDIMAGE *image, int *num);

extern ER nvtvideosprite_GetLockPingPong(VDS_PHASE phase, int id, unsigned int *addr, unsigned int *size);

extern ER nvtvideosprite_UnlockPingPong(void);

extern ER nvtvideosprite_AllocPingPongBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

extern ER nvtvideosprite_AllocScaleBuf(VDS_PHASE phase, int id, unsigned int PreW, unsigned int PreH, unsigned int PostW, unsigned int PostH);

extern ER nvtvideosprite_AllocSingleBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

extern ER nvtvideosprite_AddScaleOsd(CodecOSDIMAGE *image, int ratio);

extern ER nvtvideosprite_InitMosaicBuf(unsigned int uiMaxSize);

extern ER nvtvideosprite_AllocMosaicBuf(VDS_PHASE phase, int vid, unsigned int width, unsigned int height);

extern ER nvtvideosprite_FreeMosaicBuffer(int release);

extern ER nvtvideosprite_SetVdsMosaicBlksz(VDS_PHASE phase, int vid, unsigned int blksz);

#endif /* _VIDEOSPRITECODEC_H */
