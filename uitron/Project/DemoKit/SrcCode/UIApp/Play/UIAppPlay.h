#ifndef _UIVIEWPLAY_H_
#define _UIVIEWPLAY_H_

#include "NVTEvent.h"

#include "AppLib.h"
#include "UIAppCommon.h"

#include "UIPlayInfo.h"


/**
    Movie play system event.
*/
typedef enum {
	NVTEVT_PLAYBACK_EVT_START   = APPUSER_PLAYBACK_BASE, ///< Min value = 0x14003000
	//Event for single view
	/* INSERT NEW EVENT HRER */
	//Event for thumbnial view
	/* INSERT NEW EVENT HRER */
	//Event for Photo zoom view
	/* INSERT NEW EVENT HRER */
	//Event for Photo edit view
	/* INSERT NEW EVENT HRER */
	//Event for Movie playback view
	NVTEVT_EXE_OPENPLAY         = 0x14003800,
	NVTEVT_EXE_CLOSEPLAY        = 0x14003801,
	NVTEVT_EXE_PAUSEPLAY        = 0x14003802,
	NVTEVT_EXE_STARTPLAY        = 0x14003803,
	NVTEVT_EXE_FWDPLAY          = 0x14003804,
	NVTEVT_EXE_BWDPLAY          = 0x14003805,
	NVTEVT_EXE_FWDSTEPPLAY      = 0x14003806,
	NVTEVT_EXE_BWDSTEPPLAY      = 0x14003807,
	NVTEVT_EXE_SWITCH_FIRSTFRAME = 0x14003808,
	NVTEVT_EXE_SWITCH_LASTFRAME = 0x14003809,
	NVTEVT_EXE_CHANGESIZE       = 0x1400380a,
	NVTEVT_EXE_MOVIEAUDPLAYVOLUME = 0x1400380b,
	NVTEVT_CB_MOVIE_START       = 0x140038f0, ///< Movie start.
	NVTEVT_CB_MOVIE_ONE_SEC     = 0x140038f1, ///< Posted every one second on movie recoding.
	NVTEVT_CB_MOVIE_FINISH      = 0x140038f2, ///< Movie finished.
	NVTEVT_CB_MOVIE_ONE_VIDEOFRAME = 0x140038f3, ///< The first video frame done.
	/* INSERT NEW EVENT HRER */
	NVTEVT_PLAYBACK_EVT_END     = APPUSER_PLAYBACK_BASE + 0x1000 - 1, ///< Max value = 0x14003000
} NVT_MOVIEPLAY_EVENT;

//Play Init
extern INT32 PlayExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PlayExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//Play Exe
extern INT32 PlayExe_OnSlide(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PlayExe_OnVoiceMemo(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

extern void UIAppPlay_InstallCmd(void);

extern VControl CustomPlayObjCtrl;

/**
    Register callback function for MediaPlay_Open().

    Register callback function for the events from Media Play Object

    @param[in] event_id  define Media Play Event ID

    @return void
*/
extern void Play_MovieCB(UINT32 event_id);

extern void UIAppPlay_MoviePlayCB(UINT32 uiEventID);

#endif //_UIVIEWPLAY_H_
