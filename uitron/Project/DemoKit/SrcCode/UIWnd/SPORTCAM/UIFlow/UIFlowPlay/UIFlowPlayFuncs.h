
#ifndef _UIFLOWPLAYFUNCS_H
#define _UIFLOWPLAYFUNCS_H

#include "Type.h"

enum _PLB_TASK_STATE {
	PLB_ST_IDLE = 0,        // idle mode
	PLB_ST_MENU,            // menu mode
	PLB_ST_FULL,            // full image mode
	PLB_ST_MAGNIFY,         // Magnify mode
	PLB_ST_THUMB,           // thumbnail mode
	PLB_ST_PLAY_MOV,        // play movie
	PLB_ST_PAUSE_MOV,       // pause movie
	PLB_ST_FWD_MOV,         // forward movie
	PLB_ST_BWD_MOV,         // backward movie
	PLB_ST_SLOW_MOV,        // play movie with slow motion
	PLB_ST_PLAY_WAV,        // play wav
	PLB_ST_PAUSE_WAV,       // pause wav
	PLB_ST_FWD_WAV,         // forward wav
	PLB_ST_BWD_WAV,         // backward wav
	PLB_ST_ZOOM,            // zoom mode
	PLB_ST_ZOOM_MOVE,            // zoom mode
	PLB_ST_ERASE,           // erase menu mode
	PLB_ST_SLIDE_SHOW,       // slide show mode
	PLB_ST_QDMENU, //quick delete mode
	PLB_ST_MENU_AVI,        //AVI menu mode
	PLB_ST_MENUVIVILINK  // vivilink menu state
};

enum _PLB_VIDEO_SPEEDUP {
	PLB_BWD_MOV_8x = 0x10,     // 8x backward play speed
	PLB_BWD_MOV_4x,     // 4x backward play speed
	PLB_BWD_MOV_2x,     // 2x backward play speed
	PLB_BWD_MOV_1x,     // 1x backward play spped
	PLB_FWD_MOV_1x,     // normal play speed
	PLB_FWD_MOV_2x,     // 2x forward play speed
	PLB_FWD_MOV_4x,     // 4x forward play speed
	PLB_FWD_MOV_8x,     // 8x forward play speed
};

typedef struct _PLB_TASK_DATA {
	UINT8   State;          // current state
	UINT32  err;            // error status
	UINT16  TimerCount;     // timer count
	UINT16  VolCount;       // volume bar display count
	BOOL    bVolDraw;       // volume bar draw flag
	INT16  VideoPBSpeed;   // video file play speed
} PLB_TASK_DATA;


//#NT#2012/8/13#Philex - begin
//Add Thumbnail play data structure
typedef struct _THUMBNAIL_CONFIG {
	UINT16  wThumbPerLine;          // Number of thumbnails per line
	UINT16  wThumbPerColumn;        // Number of thumbnails per Column
	UINT16  wFirstHGap;             // The Horizontal gap size to the first thumbnail
	UINT16  wHGap;                  // The Horizontal gap size between thumbnails
	UINT16  wLastHGap;              // The Horizontal gap size after the last thumbnail
	UINT16  wFirstVGap;             // The Vertical gap size to the first thumbnail
	UINT16  wVGap;                  // The Vertical gap between thumbnails
	UINT16  wLastVGap;              // The Vertical gap size after the last thumbnail
	UINT16  wFrColor;               // Color of select frame
	UINT16  wBgColor;               // Color of background
} THUMBNAIL_CONFIG;

#define FLOWPB_THUMB_MODE_3_3           0
#define FLOWPB_THUMB_MODE_3_2           1
#define FLOWPB_THUMB_MODE_5_5           2
#define FLOWPB_THUMB_MODE               FLOWPB_THUMB_MODE_3_3

#if (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_3)
#define FLOWPB_THUMB_H_NUM2             3
#define FLOWPB_THUMB_V_NUM2             3
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_2)
#define FLOWPB_THUMB_H_NUM2             3
#define FLOWPB_THUMB_V_NUM2             2
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_5_5)
#define FLOWPB_THUMB_H_NUM2             5
#define FLOWPB_THUMB_V_NUM2             5
#endif
//#NT#2012/8/13#Philex - end


extern PLB_TASK_DATA g_PlbData;

extern void FlowPB_SliderCBFunc(void);
extern void UIFlowMoviePlayCBFunc(UINT32 event_id);

extern UINT8 FlowPB_GetPlbDataState(void);
extern void FlowPB_PlayVideo(void);
extern void FlowPB_PauseVideoPlaying(void);
extern void FlowPB_ResumeVideoPlaying(void);
extern void FlowPB_StopVideoPlaying(void);

#endif //_UIFLOWPLAYFUNCS_H
