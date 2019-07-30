#include <stdio.h>
#include "UIFlow.h"


// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// MCTF Options
TMDEF_BEGIN_OPTIONS(MCTF)
TMDEF_OPTION_TEXT_S(MCTF_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(MCTF_ON, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()
// EDGE Options
TMDEF_BEGIN_OPTIONS(EDGE)
TMDEF_OPTION_TEXT_S(EDGE_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(EDGE_ON, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()
// NR Options
TMDEF_BEGIN_OPTIONS(NR)
TMDEF_OPTION_TEXT_S(NR_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(NR_ON, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
// extra Setup Menu Items
TMDEF_BEGIN_ITEMS(EXT_SETUP)
TMDEF_ITEM_TEXTID(MCTF)
TMDEF_ITEM_TEXTID(EDGE)
TMDEF_ITEM_TEXTID(NR)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Setup Menu Pages
TMDEF_BEGIN_PAGES(EXT_SETUP)
TMDEF_PAGE_TEXT_ICON(EXT_SETUP)
TMDEF_END_PAGES()

TMDEF_EMNU(gExtSetupMenu, EXT_SETUP, ExtSetup_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int ExtSetup_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
	UINT16  uwItemId;
	UINT16  uwOption;

	if (uiMessage == TMM_CONFIRM_OPTION) {
		uwItemId = LO_WORD(uiParam);
		uwOption = HI_WORD(uiParam);

		switch (uwItemId) {
		case IDM_MCTF:
			Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_MCTF, 1, uwOption);
			break;
		case IDM_EDGE:
			Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_EDGE, 1, uwOption);
			break;
		case IDM_NR:
			Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_NR, 1, uwOption);
			break;
		default:
			debug_err(("err id %d\r\n", uwItemId));
			break;
		}
	}

	return TMF_PROCESSED;
}
