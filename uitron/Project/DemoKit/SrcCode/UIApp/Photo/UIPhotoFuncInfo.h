#ifndef _UI_PHOTOFUNCINFO_H
#define _UI_PHOTOFUNCINFO_H

#include "PrjCfg.h"
#include "photo_task.h"

#if(IPCAM_MODE==ENABLE)
#include "alg_IPCam_PhotoFuncInfo.h"
#endif

#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"
#include "alg_Photo_PhotoFuncInfo.h"
#endif

#if(MOVIE_MODE==ENABLE)
#include "alg_Movie_PhotoFuncInfo.h"
#endif


extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;

extern void    UIPhotoFunc_Open(PHOTO_FUNC_INFO *phead);
extern void    UIPhotoFunc_Close(void);
extern UINT32  Photo_getDualCamSetting(void);


#endif
