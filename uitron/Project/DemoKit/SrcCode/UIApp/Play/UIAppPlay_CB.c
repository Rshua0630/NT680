
#include "SysKer.h"
#include "PlaybackTsk.h"
#include "UIFramework.h"
#include "UIAppPlay.h"
#include "SMediaPlayAPI.h"
#include "AppDisp_MoviePlayView.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MDSCPLAYBK
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MoviePlay_GetMediaInfo(x)                SMediaPlay_GetMediaInfo(x)
#define MoviePlay_GetNowDisplayFrame(x,y,z)      SMediaPlay_GetNowDisplayFrame(x,y,z)
#define MoviePlay_GetVideoFrameRate(x)           SMediaPlay_GetVideoFrameRate(x)
#define MoviePlay_GetVideoFrameData(x)           SMediaPlay_GetVideoFrameData((SMEDIAPLAY_FARMEPOS *)x)
#define MoviePlay_GetAudioFrameData(x)           SMediaPlay_GetAudioFrameData((SMEDIAPLAY_FARMEPOS *)x)

static UINT32   gMoviePlay_DisplayCnt = 1;

/**
	Movie Play Callback for NMediaPlay framework
*/
void UIAppPlay_MoviePlayCB(UINT32 uiEventID)
{
	DBG_IND("MoviePlayCB event=%d\r\n", uiEventID);

	switch (uiEventID) {
	case MOVIEPLAY_EVENT_MEDIAINFO_READY:
	{
		MEDIA_FIRST_INFO   *pMediaInfo = NULL;
		USIZE               DispAR;

		pMediaInfo = (MEDIA_FIRST_INFO *)ImageApp_MoviePlay_GetParam(MOVIEPLAY_NM_PARAM_MEDIA_INFO);

		// Get Display AR
		if (!pMediaInfo->tkwid || !pMediaInfo->tkhei) {
			DBG_ERR("tkwid=%d, tkhei=%d\r\n", pMediaInfo->tkwid, pMediaInfo->tkhei);
			break;
		}
		DispAR.w = pMediaInfo->tkwid;
		DispAR.h = pMediaInfo->tkhei;

		// Init for MoviePlayView
		AppDisp_MoviePlayView_Init(&DispAR);

		// Start MoviePlayView_tsk
		AppDisp_MoviePlayView_Open();
		break;
	}
	case MOVIEPLAY_EVENT_ONE_DISPLAYFRAME:
		AppDisp_MoviePlayView_OnDraw();
		break;
	case MOVIEPLAY_EVENT_STOP:
		AppDisp_MoviePlayView_Close(); // Release ISF_Data
		break;
	default:
		DBG_ERR("Invalid eventID=0x%x\r\n");
		break;
	}
}

void Play_MovieCB(UINT32 uiEventID)
{
	DBG_IND("+PlayCB=%d\r\n", uiEventID);

	switch (uiEventID) {
	case SMEDIAPLAY_EVENT_MEDIAINFO_READY: {
			MEDIA_FIRST_INFO   *pMediaInfo = NULL;
			USIZE               DispAR;

			pMediaInfo = MoviePlay_GetMediaInfo();

			// Get Display AR
			DispAR.w = pMediaInfo->tkwid;
			DispAR.h = pMediaInfo->tkhei;

			// Init for MoviePlayView
			AppDisp_MoviePlayView_Init(&DispAR);

			// Start MoviePlayView_tsk
			AppDisp_MoviePlayView_Open();
		}
		break;

	case SMEDIAPLAY_EVENT_ONE_DISPLAYFRAME: {
			AppDisp_MoviePlayView_OnDraw();
			break;
		}

	case SMEDIAPLAY_EVENT_FINISH:
			Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0); // UI close stream & free Buffer
		break;

	case SMEDIAPLAY_EVENT_STOP:
			AppDisp_MoviePlayView_Close(); // Release ISF_Data
		break;

	case SMEDIAPLAY_EVENT_ONE_SECOND:
			Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 0);
		break;

	case SMEDIAPLAY_EVENT_START: {
			gMoviePlay_DisplayCnt = 0;
			Ux_PostEvent(NVTEVT_CB_MOVIE_START, 0);
		}
		break;

	case SMEDIAPLAY_EVENT_ONE_VIDEOFRAME: {
// This callback is used for playback progressive bar, however NT96660 doesn't implement it, so close this block now.  ( reduce unnecessary event post )
#if 0
			UINT32 uPollCnt = 2;

			if (MoviePlay_GetVideoFrameRate() > 120) {      // for    240 fps to fix post event table overflow issue
				uPollCnt = 8;
			} else if (MoviePlay_GetVideoFrameRate() >= 60) { // for 60/120 fps to fix post event table overflow issue
				uPollCnt = 4;
			}
			gMoviePlay_DisplayCnt++;
			if (gMoviePlay_DisplayCnt == uPollCnt) {
				Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_VIDEOFRAME, 0);
				gMoviePlay_DisplayCnt = 0;
			}
#endif
		}
		break;

	case SMEDIAPLAY_EVENT_CURR_VIDFRAME: {
			SMEDIAPLAY_FARMEPOS VFrmPos;
			MoviePlay_GetVideoFrameData(&VFrmPos);
			DBG_IND("V:a=0x%X, s=%d\r\n", VFrmPos.Addr, VFrmPos.Size);
		}
		break;

	case SMEDIAPLAY_EVENT_CURR_AUDFRAME: {
			SMEDIAPLAY_FARMEPOS AFrmPos;
			MoviePlay_GetAudioFrameData(&AFrmPos);
			DBG_IND("A:a=0x%X, s=%d\r\n", AFrmPos.Addr, AFrmPos.Size);
		}
		break;

	default:
		break;
	}

}

