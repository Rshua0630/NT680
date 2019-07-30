#ifndef _ALG_IPCAM_PHOTOFUNCINFO_H
#define _ALG_IPCAM_PHOTOFUNCINFO_H

#include "photo_task.h"
#include "fd_lib.h"
#include "bc_lib.h"
#include "vqa_lib.h"

#define IPCAM_EVENT_FD		1
#define IPCAM_EVENT_VQA		2
#define IPCAM_EVENT_BCMD	3

#define BC_MAX_NUM	1024
#define SET_FLAG(flag, val)     flag|=(1<<(val))
#define CLEAR_FLAG(flag, val)   flag&=(~(1<<(val)))
#define IS_FLAG(flag, val)      (((flag)&(1<<(val)))!=0)
#define CUSTOM_BC_USR_MAX_EVT_SIZE 120*90

typedef struct
{
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* microseconds */
} TIME_VAL ;

typedef struct {
	UINT32  x;
	UINT32  y;
	UINT32  w;
	UINT32  h;
} EVENT_URECT, *EVENT_PURECT;

typedef struct {
	char Event;
	UINT32 uiFaceNumber;
	URECT info[FD_NUM_MAX];
} IPCAM_FD_INFO;

//NEED MOVE TO BC_LIB.H
//BCMD TODO
typedef struct {
	UINT32 fps; //fps=0 means turn off
	UINT32 bc_mode;
	BC_USR_PARAM param_info;
	UINT32	reserved[6];
} BCParam;

typedef struct {
	UINT32 targetnum;
	EVENT_URECT target[BC_MAX_NUM];
	UINT32 evt_width;
	UINT32 evt_height;
	UINT8 evt_map_addr[CUSTOM_BC_USR_MAX_EVT_SIZE];

} BC_RESULT;
///////////////
typedef struct {
	char event;
	TIME_VAL TimeStamp;
	BC_RESULT result;
} IPCAM_BC_RESULT;



typedef struct {
	char Event;
	TIME_VAL TimeStamp;
	BOOL TooLight;
	BOOL TooDark;
	BOOL Blur;
} IPCAM_VQA_RESULT;


extern void Movie_SetBcParam(BCParam *param);
extern void Movie_VqaSetFps(UINT32 fps);
extern vqa_res_t Movie_VqaGetResult(void);
extern void Photo_InstallCmd(void);
//END
extern void IPCam_InitPhotoFunc(void);

/*
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion;

#if IPCAM_FD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamfd;

#endif

#if IPCAM_DIS
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamdis;
#endif

#if IPCAM_OD_FUNC
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamod;
#endif
*/

#if IPCAM_CDNN_FUNC || IPCAM_CDNN2_FUNC
#include "alg_CDNN_PhotoFuncInfo.h"
#endif

#if IPCAM_BC_FUNC
//extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcambc;
extern void IPCam_BCLock(BOOL enable);
#endif


/*
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull1;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull2;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull3;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull4;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull5;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull6;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull7;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull8;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull9;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull10;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull11;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull12;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull13;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull14;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull15;
extern PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull16;
*/

void Movie_MDLock(BOOL enable);

//#if IPCAM_IN_DIS_FUNC || MOVIE_ODT_FUNC
#if 1
//#if IPCAM_DIS_MV_FUNC
extern void IPCam_DIS_SigSem(void);
extern void IPCam_DIS_WaitSem(void);
extern void IPCam_DIS_InstallID(void);
#endif
#endif

