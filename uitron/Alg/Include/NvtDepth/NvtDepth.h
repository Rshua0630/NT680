#ifndef __NVT_DEPTH__H__
#define __NVT_DEPTH__H__

#include "ive_lib.h"
#include "sde_lib.h"
#include "ise_lib.h"
#include "GxImage.h"

#define __Depth_FLOW__        1// 0: testKit 1: demokit ?

/**
     @name Depth return status
*/
//@{
#define Depth_STA_OK                  (0)                   ///<  the status is ok
#define Depth_STA_ERROR               (-1)                  ///<  some error occurred
//@}


/**
     Stereo Depth disparity mode
*/
typedef enum {
	MAX_DISP_D32 = 0,                                       ///<  max disparity = 32
	MAX_DISP_D128 = 1,                                      ///<  max disparity = 128
	ENUM_DUMMY4WORD(DISP_MODE)
} DISP_MODE;


/**
     Stereo Depth display mode
*/
typedef enum {
	RIGHT_IMAGE = 0,                                       ///<  show right image
	DEPTH = 1,                                             ///<  show depth image
	ENUM_DUMMY4WORD(DISPLAY_MODE)
} DISPLAY_MODE;

/**
     Stereo Depth disparity output information
*/
typedef struct _STEREO_DISP_RESULT {
	IMG_BUF disp_out;                                        ///<  disparity output image buf
	DISP_MODE disp_mode;                                     ///<  disparity mode
	DISPLAY_MODE show_case;                                  ///<  display case

} STEREO_DISP_RESULT;

/**
     Stereo Depth init parameters
*/
typedef struct _DEPTH_INIT_PARAM {
	MEM_RANGE buf;                                          ///<  image buffer
	MEM_RANGE cachebuf;                                     ///<  cache image buffer
	DISP_MODE disp_mode;                                    ///<  disparity mode
	UINT32 ntr_on;											///<  non-texture removal enable
	UINT32 inval_show;										///<  set invalid as white or black

} DEPTH_INIT_PARAM;


typedef struct _DEPTH_PARAM {
	UINT32 ntr_on;											///<  non-texture removal enable
	UINT32 inval_show;										///<  set invalid as white or black
	DISP_MODE disp_mode;

} DEPTH_PARAM;

extern void Depth_PrintVersion(void);

extern void Depth_GetDispResult(STEREO_DISP_RESULT *Result);

extern void Depth_DumpDispResult(UINT32 frame_count);

extern UINT32 Depth_CalcCacheBuffSize(UINT32 width, UINT32 height, DISP_MODE dispMode);

extern UINT32 Depth_CalcBuffSize(UINT32 width, UINT32 height, DISP_MODE dispMode);

extern UINT32 Depth_GetBuffSize(void);

extern UINT32 Depth_Init(DEPTH_INIT_PARAM *initObj);

extern UINT32 Depth_ChangeParam(DEPTH_PARAM *ParamObj);

#if __Depth_FLOW__
extern void Depth_Process(IMG_BUF *pLeftInputImg, IMG_BUF *pRightInputImg);
#else
extern void Depth_Process(void);
#endif

#endif
