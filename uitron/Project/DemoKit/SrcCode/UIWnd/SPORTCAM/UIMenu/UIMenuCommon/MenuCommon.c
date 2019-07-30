#include <stdio.h>
#include "UIFlow.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// Menu Common Options
TMDEF_BEGIN_OPTIONS(COMMON_MODE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_MENU)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_LOCAL)
TMDEF_OPTION_TEXT_S(COMMON_LOCAL_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(COMMON_LOCAL_ON, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_CLOUD)
TMDEF_OPTION_TEXT_S(COMMON_CLOUD_OFF, TM_OPTION_ENABLE)
TMDEF_OPTION_TEXT_S(COMMON_CLOUD_ON, TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_SETUP)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_EXT_SETUP)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
// Movie Menu Items
TMDEF_BEGIN_ITEMS(COMMON)
TMDEF_ITEM_TEXTID(COMMON_MODE)
TMDEF_ITEM_TEXTID(COMMON_MENU)
#if !defined(_NETWORK_NONE_)
TMDEF_ITEM_TEXTID(COMMON_LOCAL)
#endif
TMDEF_ITEM_TEXTID(COMMON_SETUP)
#if (EXTRA_MENU==ENABLE)
TMDEF_ITEM_TEXTID(COMMON_EXT_SETUP)
#endif
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Common Menu Pages
TMDEF_BEGIN_PAGES(COMMON)
TMDEF_PAGE_TEXT_ICON(COMMON)
TMDEF_END_PAGES()

TMDEF_EMNU(gCommonMenu, COMMON, Common_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Common_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
	UINT16  uwItemId;
#if(WIFI_AP_FUNC==ENABLE)
	UINT16  uwOption;
#endif

	if (uiMessage == TMM_CONFIRM_OPTION) {
		uwItemId = LO_WORD(uiParam);
#if(WIFI_AP_FUNC==ENABLE)
		uwOption = HI_WORD(uiParam);
#endif

		switch (uwItemId) {
		case IDM_COMMON_MODE:

			break;

		case IDM_COMMON_MENU:

			break;
#if(WIFI_AP_FUNC==ENABLE)
		case IDM_COMMON_LOCAL:
			if (uwOption == WIFI_ON) {
#if 0
				UI_SetData(FL_NetWorkMode, NET_AP_MODE); // flag for TUTK on in
				Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
#else
				//#NT#2016/03/23#Isiah Chang -begin
				//#NT#add new Wi-Fi UI flow.
				if (UI_GetData(FL_WIFI_LINK) == WIFI_LINK_NG) {
					if (CheckWiFiMapTbl() != E_OK) {
						UI_SetData(FL_WIFI_LINK, WIFI_LINK_NG);
						return TMF_PROCESSED;
					}

					UI_SetData(FL_WIFI_LINK, WIFI_LINK_LINKING);
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
					Ux_OpenWindow(&UIMenuWndWiFiWaitCtrl, 0);
#endif
					BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
				}
				//#NT#2016/03/23#Isiah Chang -end
#endif
			} else {
				//#NT#2016/03/23#Isiah Chang -begin
				//#NT#add new Wi-Fi UI flow.
				if (UI_GetData(FL_WIFI_LINK) == WIFI_LINK_OK) {
					UI_SetData(FL_WIFI_LINK, WIFI_LINK_DISCONNECTING);
					Ux_SendEvent(0, NVTEVT_EXE_WIFI_STOP, 0);
				}
				//#NT#2016/03/23#Isiah Chang -end
				Ux_CloseWindow(&MenuCommonItemCtrl, 0);
			}
			break;

		case IDM_COMMON_CLOUD:

			break;
#endif
		case IDM_COMMON_SETUP:

			break;
		}
	}

	return TMF_PROCESSED;
}
