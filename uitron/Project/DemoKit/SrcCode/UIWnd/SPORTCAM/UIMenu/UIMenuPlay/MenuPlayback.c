#include <stdio.h>
#include "UIFlow.h"
#include "Utility.h"
#include "PlaybackTsk.h"
#include "DCF.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------

TMDEF_BEGIN_OPTIONS(VOLUME)
TMDEF_OPTION_TEXT_S(VOL0, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL1, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL2, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL3, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL4, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL5, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL6, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL7, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL8, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL9, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(VOL10, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------

static BOOL MenuPlay_IsNoFile(void)
{
	UINT32 uiStatus;
	BOOL   bRet;

	uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
	if (uiStatus & PB_STA_NOIMAGE) {
		bRet = TRUE;
	} else {
		bRet = FALSE;
	}

	return bRet;
}

static int MenuCustom_Protect(UINT32 uiMessage, UINT32 uiParam)
{
	if (MenuPlay_IsNoFile() == TRUE) {
		Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
	} else {
		Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	}
	return TMF_PROCESSED;
}

// Playback Menu Items
TMDEF_BEGIN_ITEMS(PLAYBACK)
TMDEF_ITEM_CUSTOM(PROTECT, MenuCustom_Protect)
TMDEF_ITEM_TEXTID(VOLUME)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Playback Menu Pages
TMDEF_BEGIN_PAGES(PLAYBACK)
TMDEF_PAGE_TEXT_ICON(PLAYBACK)
TMDEF_END_PAGES()

TMDEF_EMNU(gPlaybackMenu, PLAYBACK, Playback_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Playback_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
	UINT16  uwItemId;
	UINT16  uwOption;

	if (uiMessage == TMM_CONFIRM_OPTION) {
		uwItemId = LO_WORD(uiParam);
		uwOption = HI_WORD(uiParam);

		switch (uwItemId) {
		case IDM_VOLUME:
			Ux_SendEvent(&CustomMoviePlayObjCtrl, NVTEVT_EXE_MOVIEAUDPLAYVOLUME, 1, uwOption);
			break;
		}
	}

	return TMF_PROCESSED;
}

