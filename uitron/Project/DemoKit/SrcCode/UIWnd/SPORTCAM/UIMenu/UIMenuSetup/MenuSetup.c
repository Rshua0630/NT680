#include <stdio.h>
#include "UIFlow.h"


// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// Power Saving Options
TMDEF_BEGIN_OPTIONS(AUTO_POWER_OFF)
TMDEF_OPTION_TEXT_S(OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(1MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(2MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(3MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(5MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(10MIN, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()


// Beep Options
TMDEF_BEGIN_OPTIONS(BEEP)
TMDEF_OPTION_TEXT_S(BEEP_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(BEEP_ON, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()


// Language Options
TMDEF_BEGIN_OPTIONS(LANGUAGE)
TMDEF_OPTION_TEXT_S(LANG_EN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_FR, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_ES, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_PO, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_DE, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_IT, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_SC, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_TC, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_RU, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(LANG_JP, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

// TV Mode Options
TMDEF_BEGIN_OPTIONS(TV_MODE)
TMDEF_OPTION_TEXT_S(TV_NTSC, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(TV_PAL, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()


// Frequency Options
TMDEF_BEGIN_OPTIONS(FREQUENCY)
TMDEF_OPTION_TEXT_S(50HZ, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(60HZ, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

// Screen Saver
TMDEF_BEGIN_OPTIONS(SCREEN_SAVE)
TMDEF_OPTION_TEXT_S(AUTO_OFF_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(AUTO_OFF_1MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(AUTO_OFF_3MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(AUTO_OFF_5MIN, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(AUTO_OFF_10MIN, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
int MenuCustom_DateTime(UINT32 uiMessage, UINT32 uiParam)
{
	Ux_OpenWindow(&UIMenuWndSetupDateTimeCtrl, 0);
	return TMF_PROCESSED;
}

int MenuCustom_Format(UINT32 uiMessage, UINT32 uiParam)
{
	Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	return TMF_PROCESSED;
}

int MenuCustom_Default(UINT32 uiMessage, UINT32 uiParam)
{
	Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	return TMF_PROCESSED;
}

int MenuCustom_Version(UINT32 uiMessage, UINT32 uiParam)
{
	//Ux_OpenWindow(&UIMenuWndSetupVersionCtrl, 0);
	return TMF_PROCESSED;
}
int MenuCustom_FwUpdate(UINT32 uiMessage, UINT32 uiParam)
{
	Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	return TMF_PROCESSED;
}
int MenuCustom_DeleteAll(UINT32 uiMessage, UINT32 uiParam)
{
	if (SetupExe_IsNoFile() == TRUE) {
		Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
	} else {
		Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	}
	return TMF_PROCESSED;
}
int MenuCustom_ProtectAll(UINT32 uiMessage, UINT32 uiParam)
{
	if (SetupExe_IsNoFile() == TRUE) {
		Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
	} else {
		Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	}
	return TMF_PROCESSED;
}
int MenuCustom_UnProtectAll(UINT32 uiMessage, UINT32 uiParam)
{
	if (SetupExe_IsNoFile() == TRUE) {
		Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_NO_IMAGE, FLOWWRNMSG_TIMER_1SEC);
	} else {
		Ux_OpenWindow(&MenuCommonConfirmCtrl, 1, uiParam);
	}
	return TMF_PROCESSED;
}
// Setup Menu Items
TMDEF_BEGIN_ITEMS(SETUP)
TMDEF_ITEM_CUSTOM(DATE_TIME, MenuCustom_DateTime)
TMDEF_ITEM_TEXTID(AUTO_POWER_OFF)
TMDEF_ITEM_TEXTID(BEEP)
TMDEF_ITEM_TEXTID(LANGUAGE)
TMDEF_ITEM_TEXTID(FREQUENCY)
#if TV_FUNC
TMDEF_ITEM_TEXTID(TV_MODE)
#endif
TMDEF_ITEM_CUSTOM(FORMAT, MenuCustom_Format)
TMDEF_ITEM_CUSTOM(DEFAULT, MenuCustom_Default)
TMDEF_ITEM_CUSTOM(VERSION, MenuCustom_Version)
TMDEF_ITEM_TEXTID(SCREEN_SAVE)
TMDEF_ITEM_CUSTOM(FW_UPDATE, MenuCustom_FwUpdate)
TMDEF_ITEM_CUSTOM(DELETE_ALL, MenuCustom_DeleteAll)
TMDEF_ITEM_CUSTOM(PROTECT_ALL, MenuCustom_ProtectAll)
TMDEF_ITEM_CUSTOM(UNPROTECT_ALL, MenuCustom_UnProtectAll)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Setup Menu Pages
TMDEF_BEGIN_PAGES(SETUP)
TMDEF_PAGE_TEXT_ICON(SETUP)
TMDEF_END_PAGES()

TMDEF_EMNU(gSetupMenu, SETUP, Setup_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Setup_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
	UINT16  uwItemId;
	UINT16  uwOption;

	if (uiMessage == TMM_CONFIRM_OPTION) {
		uwItemId = LO_WORD(uiParam);
		uwOption = HI_WORD(uiParam);

		switch (uwItemId) {
		case IDM_AUTO_POWER_OFF:
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_POWEROFF, 1, uwOption);
			break;
		case IDM_SCREEN_SAVE:
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_LCDOFF, 1, uwOption);
			break;

		case IDM_BEEP:
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_BEEPKEY, 1, uwOption);
			break;

		case IDM_LANGUAGE:
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_LANGUAGE, 1, uwOption);
			break;

		case IDM_TV_MODE:
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_TVFORMAT, 1, uwOption);
			break;

		case IDM_FREQUENCY:
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_FREQ, 1, uwOption);
			break;
		default:
			debug_err(("err id %d\r\n", uwItemId));
			break;
		}
	}

	return TMF_PROCESSED;
}
