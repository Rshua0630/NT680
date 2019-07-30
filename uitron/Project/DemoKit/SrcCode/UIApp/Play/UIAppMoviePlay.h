#ifndef _UIVIEWMOVIEPLAY_H_
#define _UIVIEWMOVIEPLAY_H_

#include "UIFramework.h"
#include "PrjCfg.h"
#include "UIAppCommon.h"
#include "UIAppPlay.h"

#define UIPLAYAVI_SPEED_X1_IDX          0
#define UIPLAYAVI_SPEED_X2_IDX          1
#define UIPLAYAVI_SPEED_X4_IDX          2
#define UIPLAYAVI_SPEED_X8_IDX          3

//----- Play -----
#define MOVPLAY_EVENT_FINISH                        1            //1   ///< Finish
#define MOVPLAY_EVENT_ONE_SECOND                    2        //2   ///< One second arrives
#define MOVPLAY_EVENT_ONE_VIDEOFRAME                3    //3   ///< Decode one video-frame
#define MOVPLAY_EVENT_START                         4           //6   //AVIPLAY_EVENT_START
#define MOVPLAY_EVENT_INIT_PROCESSING               5   //5
#define MOVPLAY_EVENT_INIT_OK                       6           //6
#define MOVPLAY_EVENT_ERROR                         7
#define MOVPLAY_EVENT_TEST                          8
#define MOVPLAY_EVENT_AUD_OPENED                    9

typedef enum {
	PLAYMOVIE_DIRID,
	PLAYMOVIE_FILEID,
	PLAYMOVIE_TOTAL_FRAME,
	PLAYMOVIE_TOTAL_SECOND,
	PLAYMOVIE_EV,
	PLAYMOVIE_WIDTH,
	PLAYMOVIE_CURR_FRAME,
	PLAYMOVIE_CURR_SECOND,
	PLAYMOVIE_LOCKSTATUS,
	PLAYMOVIE_FILENUM,
	PLAYMOVIE_OPENED,
	PLAYMOVIE_GET_DATATYPE_MAX
} PLAYMOVIE_GET_DATATYPE;


typedef enum {
	PLAYMOVIE_SPEEDL1,
	PLAYMOVIE_SPEEDL2,
	PLAYMOVIE_SPEEDL4,
	PLAYMOVIE_SPEEDL8,
	PLAYMOVIE_AUD_VOLUME,
	PLAYMOVIE_SET_DATATYPE_MAX
} PLAYMOVIE_SET_DATATYPE;


typedef enum {
	RECMOVIE_WIDTH,
	RECMOVIE_HEIGHT,
	RECMOVIE_FRAMERATE,
	RECMOVIE_TARGETBITRATE,
	RECMOVIE_QUALITY,
	RECMOVIE_RECORDTIME,
	RECMOVIE_AYSYNCCNT,
	RECMOVIE_AUD_VOLUME,
	RECMOVIE_SET_DATATYPE_MAX
} RECMOVIE_SET_DATATYPE;

extern VControl CustomMoviePlayObjCtrl;

extern UINT32   MoviePlayExe_GetMovieSizeWidth(UINT32 ubIndex);
extern UINT32   MoviePlayExe_GetMovieSizeHeight(UINT32 ubIndex);
extern UINT32   MoviePlayExe_GetMovieFrameRate(UINT32 ubIndex);
extern UINT32   MoviePlayExe_GetMovieQualityValue(UINT16 ubIndex);
extern UINT32   MoviePlayExe_GetMovieTargetBitrate(UINT16 ubIndex);
extern UINT32   MoviePlayExe_GetDataPlay(PLAYMOVIE_GET_DATATYPE dataType);
extern void     MoviePlayExe_SetDataPlay(PLAYMOVIE_SET_DATATYPE dataType, UINT32 data);

#endif
