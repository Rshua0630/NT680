#include <stdio.h>
#include "UIFlow.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------

int MenuCustom_Movie(UINT32 uiMessage, UINT32 uiParam)
{
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE) {
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
	} else {
		Ux_CloseWindow(&MenuCommonItemCtrl, 0);
	}
	return TMF_PROCESSED;
}
int MenuCustom_Photo(UINT32 uiMessage, UINT32 uiParam)
{
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PHOTO) {
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
	} else {
		Ux_CloseWindow(&MenuCommonItemCtrl, 0);
	}

	return TMF_PROCESSED;
}
int MenuCustom_Playback(UINT32 uiMessage, UINT32 uiParam)
{
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PLAYBACK) {
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
	} else {
		Ux_CloseWindow(&MenuCommonItemCtrl, 0);
	}

	return TMF_PROCESSED;
}

// Movie Menu Items
TMDEF_BEGIN_ITEMS(MODE)
TMDEF_ITEM_CUSTOM(MODE_MOVIE, MenuCustom_Movie)
TMDEF_ITEM_CUSTOM(MODE_PHOTO, MenuCustom_Photo)
TMDEF_ITEM_CUSTOM(MODE_PLAYBACK, MenuCustom_Playback)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Common Menu Pages
TMDEF_BEGIN_PAGES(MODE)
TMDEF_PAGE_TEXT_ICON(MODE)
TMDEF_END_PAGES()

TMDEF_EMNU(gModeMenu, MODE, Mode_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Mode_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
#if 0
	UINT16  uwItemId;
	UINT16  uwOption;

	if (uiMessage == TMM_CONFIRM_OPTION) {
		uwItemId = LO_WORD(uiParam);
		uwOption = HI_WORD(uiParam);
	}
#endif

	return TMF_PROCESSED;
}
