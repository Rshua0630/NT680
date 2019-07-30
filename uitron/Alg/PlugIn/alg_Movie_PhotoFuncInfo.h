#ifndef _ALG_MOVIE_PHOTOFUNCINFO_H
#define _ALG_MOVIE_PHOTOFUNCINFO_H

extern PHOTO_FUNC_INFO PhotoFuncInfo_dis;
extern void Movie_BCLock(BOOL bLock);
extern void Movie_BCProcessRateSet(UINT32 ProcessRate);
extern UINT32 Movie_BCProcessRateGet(void);
extern UINT32 Movie_BCGetBlkTarget(URECT* target, URECT* targetcoord, UINT32 maxnum);
extern void alg_Movie_InitPhotoFunc(PHOTO_FUNC_INFO *phead);

#endif

