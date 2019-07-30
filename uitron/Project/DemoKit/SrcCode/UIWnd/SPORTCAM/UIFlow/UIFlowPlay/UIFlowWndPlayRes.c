//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDPLAYRES_H
#define UIFLOWWNDPLAYRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_ITEM_RECT(CMD_Rectangle, Skin1Rect, 0, 0, 319, 239, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_BODY(Skin1Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin1, 0, 0, 319, 239)

SHOW_ITEM_RECT(CMD_Rectangle, Skin2Rect, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Skin2Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin2, 0, 0, 1, 1)

SHOW_ITEM_RECT(CMD_Rectangle, Skin4Rect, 0, 0, 49, 49, 0, 0, 18, 18, 0, 0, 0)
SHOW_MAP_BEGIN(Skin4)
SHOW_MAP_BODY(Skin4Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin4, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon5, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal3)
SHOW_MAP_BODY(Skin4)
SHOW_MAP_BODY(Icon5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal3, 140, 102, 189, 151)

SHOW_ITEM_RECT(CMD_Rectangle, Skin7Rect, 0, 0, 49, 49, 0, 0, 2, 2, 0, 0, 0)
SHOW_MAP_BEGIN(Skin7)
SHOW_MAP_BODY(Skin7Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin7, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon8, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused6)
SHOW_MAP_BODY(Skin7)
SHOW_MAP_BODY(Icon8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused6, 140, 102, 189, 151)

SHOW_MAP_BEGIN(Skin10)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin10, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon11, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal_Disable9)
SHOW_MAP_BODY(Skin10)
SHOW_MAP_BODY(Icon11)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal_Disable9, 140, 102, 189, 151)

SHOW_MAP_BEGIN(Skin13)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin13, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon14, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused_Disable12)
SHOW_MAP_BODY(Skin13)
SHOW_MAP_BODY(Icon14)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused_Disable12, 140, 102, 189, 151)

SHOW_ITEM_RECT(CMD_Rectangle, Skin16Rect, 0, 0, 49, 49, 0, 0, 1, 1, 0, 0, 0)
SHOW_MAP_BEGIN(Skin16)
SHOW_MAP_BODY(Skin16Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin16, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon17, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Pressed15)
SHOW_MAP_BODY(Skin16)
SHOW_MAP_BODY(Icon17)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Pressed15, 140, 102, 189, 151)

CREATE_BUTTON_ITEM_DATA(ICON_PLAY_PLAY18, STRID_NULL, ICON_PLAY_PLAY, STATUS_ENABLE, NULL)
SHOW_MAP_BEGIN(ButtonPlay)
SHOW_MAP_BODY(Normal3)
SHOW_MAP_BODY(Focused6)
SHOW_MAP_BODY(Normal_Disable9)
SHOW_MAP_BODY(Focused_Disable12)
SHOW_MAP_BODY(Pressed15)
SHOW_MAP_END

DECLARE_EVENT(ButtonPlay)
BUTTON_LIST_BEGIN(ButtonPlay)
BUTTON_LIST_ITEM(ICON_PLAY_PLAY18)
BUTTON_LIST_END

CREATE_BUTTON_DATA(ButtonPlay, BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(ButtonPlay, ButtonPlay, 0, 57, 58, 106, 107)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin20Rect, 0, 0, 49, 49, 0, 0, 18, 18, 0, 0, 0)
SHOW_MAP_BEGIN(Skin20)
SHOW_MAP_BODY(Skin20Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin20, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon21, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal19)
SHOW_MAP_BODY(Skin20)
SHOW_MAP_BODY(Icon21)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal19, 85, 102, 134, 151)

SHOW_ITEM_RECT(CMD_Rectangle, Skin23Rect, 0, 0, 49, 49, 0, 0, 2, 2, 0, 0, 0)
SHOW_MAP_BEGIN(Skin23)
SHOW_MAP_BODY(Skin23Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin23, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon24, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused22)
SHOW_MAP_BODY(Skin23)
SHOW_MAP_BODY(Icon24)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused22, 85, 102, 134, 151)

SHOW_MAP_BEGIN(Skin26)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin26, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon27, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal_Disable25)
SHOW_MAP_BODY(Skin26)
SHOW_MAP_BODY(Icon27)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal_Disable25, 85, 102, 134, 151)

SHOW_MAP_BEGIN(Skin29)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin29, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon30, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused_Disable28)
SHOW_MAP_BODY(Skin29)
SHOW_MAP_BODY(Icon30)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused_Disable28, 85, 102, 134, 151)

SHOW_ITEM_RECT(CMD_Rectangle, Skin32Rect, 0, 0, 49, 49, 0, 0, 1, 1, 0, 0, 0)
SHOW_MAP_BEGIN(Skin32)
SHOW_MAP_BODY(Skin32Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin32, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon33, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Pressed31)
SHOW_MAP_BODY(Skin32)
SHOW_MAP_BODY(Icon33)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Pressed31, 85, 102, 134, 151)

CREATE_BUTTON_ITEM_DATA(ICON_DIR_LEFT34, STRID_NULL, ICON_DIR_LEFT, STATUS_ENABLE, NULL)
SHOW_MAP_BEGIN(ButtonLeft)
SHOW_MAP_BODY(Normal19)
SHOW_MAP_BODY(Focused22)
SHOW_MAP_BODY(Normal_Disable25)
SHOW_MAP_BODY(Focused_Disable28)
SHOW_MAP_BODY(Pressed31)
SHOW_MAP_END

DECLARE_EVENT(ButtonLeft)
BUTTON_LIST_BEGIN(ButtonLeft)
BUTTON_LIST_ITEM(ICON_DIR_LEFT34)
BUTTON_LIST_END

CREATE_BUTTON_DATA(ButtonLeft, BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(ButtonLeft, ButtonLeft, 0, 2, 58, 51, 107)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin36Rect, 0, 0, 49, 49, 0, 0, 18, 18, 0, 0, 0)
SHOW_MAP_BEGIN(Skin36)
SHOW_MAP_BODY(Skin36Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin36, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon37, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal35)
SHOW_MAP_BODY(Skin36)
SHOW_MAP_BODY(Icon37)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal35, 195, 102, 244, 151)

SHOW_ITEM_RECT(CMD_Rectangle, Skin39Rect, 0, 0, 49, 49, 0, 0, 2, 2, 0, 0, 0)
SHOW_MAP_BEGIN(Skin39)
SHOW_MAP_BODY(Skin39Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin39, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon40, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused38)
SHOW_MAP_BODY(Skin39)
SHOW_MAP_BODY(Icon40)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused38, 195, 102, 244, 151)

SHOW_MAP_BEGIN(Skin42)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin42, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon43, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal_Disable41)
SHOW_MAP_BODY(Skin42)
SHOW_MAP_BODY(Icon43)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal_Disable41, 195, 102, 244, 151)

SHOW_MAP_BEGIN(Skin45)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin45, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon46, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused_Disable44)
SHOW_MAP_BODY(Skin45)
SHOW_MAP_BODY(Icon46)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused_Disable44, 195, 102, 244, 151)

SHOW_ITEM_RECT(CMD_Rectangle, Skin48Rect, 0, 0, 49, 49, 0, 0, 1, 1, 0, 0, 0)
SHOW_MAP_BEGIN(Skin48)
SHOW_MAP_BODY(Skin48Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin48, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon49, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Pressed47)
SHOW_MAP_BODY(Skin48)
SHOW_MAP_BODY(Icon49)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Pressed47, 195, 102, 244, 151)

CREATE_BUTTON_ITEM_DATA(ICON_DIR_RIGHT50, STRID_NULL, ICON_DIR_RIGHT, STATUS_ENABLE, NULL)
SHOW_MAP_BEGIN(ButtonRight)
SHOW_MAP_BODY(Normal35)
SHOW_MAP_BODY(Focused38)
SHOW_MAP_BODY(Normal_Disable41)
SHOW_MAP_BODY(Focused_Disable44)
SHOW_MAP_BODY(Pressed47)
SHOW_MAP_END

DECLARE_EVENT(ButtonRight)
BUTTON_LIST_BEGIN(ButtonRight)
BUTTON_LIST_ITEM(ICON_DIR_RIGHT50)
BUTTON_LIST_END

CREATE_BUTTON_DATA(ButtonRight, BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(ButtonRight, ButtonRight, 0, 112, 58, 161, 107)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin52Rect, 0, 0, 49, 49, 0, 0, 18, 18, 0, 0, 0)
SHOW_MAP_BEGIN(Skin52)
SHOW_MAP_BODY(Skin52Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin52, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon53, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal51)
SHOW_MAP_BODY(Skin52)
SHOW_MAP_BODY(Icon53)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal51, 140, 47, 189, 96)

SHOW_ITEM_RECT(CMD_Rectangle, Skin55Rect, 0, 0, 49, 49, 0, 0, 2, 2, 0, 0, 0)
SHOW_MAP_BEGIN(Skin55)
SHOW_MAP_BODY(Skin55Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin55, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon56, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused54)
SHOW_MAP_BODY(Skin55)
SHOW_MAP_BODY(Icon56)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused54, 140, 47, 189, 96)

SHOW_MAP_BEGIN(Skin58)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin58, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon59, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal_Disable57)
SHOW_MAP_BODY(Skin58)
SHOW_MAP_BODY(Icon59)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal_Disable57, 140, 47, 189, 96)

SHOW_MAP_BEGIN(Skin61)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin61, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon62, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused_Disable60)
SHOW_MAP_BODY(Skin61)
SHOW_MAP_BODY(Icon62)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused_Disable60, 140, 47, 189, 96)

SHOW_ITEM_RECT(CMD_Rectangle, Skin64Rect, 0, 0, 49, 49, 0, 0, 1, 1, 0, 0, 0)
SHOW_MAP_BEGIN(Skin64)
SHOW_MAP_BODY(Skin64Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin64, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon65, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Pressed63)
SHOW_MAP_BODY(Skin64)
SHOW_MAP_BODY(Icon65)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Pressed63, 140, 47, 189, 96)

CREATE_BUTTON_ITEM_DATA(ICON_DELETE66, STRID_NULL, ICON_DELETE, STATUS_ENABLE, NULL)
SHOW_MAP_BEGIN(ButtonUp)
SHOW_MAP_BODY(Normal51)
SHOW_MAP_BODY(Focused54)
SHOW_MAP_BODY(Normal_Disable57)
SHOW_MAP_BODY(Focused_Disable60)
SHOW_MAP_BODY(Pressed63)
SHOW_MAP_END

DECLARE_EVENT(ButtonUp)
BUTTON_LIST_BEGIN(ButtonUp)
BUTTON_LIST_ITEM(ICON_DELETE66)
BUTTON_LIST_END

CREATE_BUTTON_DATA(ButtonUp, BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(ButtonUp, ButtonUp, 0, 57, 3, 106, 52)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin68Rect, 0, 0, 49, 49, 0, 0, 18, 18, 0, 0, 0)
SHOW_MAP_BEGIN(Skin68)
SHOW_MAP_BODY(Skin68Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin68, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon69, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal67)
SHOW_MAP_BODY(Skin68)
SHOW_MAP_BODY(Icon69)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal67, 140, 157, 189, 206)

SHOW_ITEM_RECT(CMD_Rectangle, Skin71Rect, 0, 0, 49, 49, 0, 0, 2, 2, 0, 0, 0)
SHOW_MAP_BEGIN(Skin71)
SHOW_MAP_BODY(Skin71Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin71, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon72, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused70)
SHOW_MAP_BODY(Skin71)
SHOW_MAP_BODY(Icon72)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused70, 140, 157, 189, 206)

SHOW_MAP_BEGIN(Skin74)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin74, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon75, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal_Disable73)
SHOW_MAP_BODY(Skin74)
SHOW_MAP_BODY(Icon75)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal_Disable73, 140, 157, 189, 206)

SHOW_MAP_BEGIN(Skin77)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin77, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon78, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Focused_Disable76)
SHOW_MAP_BODY(Skin77)
SHOW_MAP_BODY(Icon78)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Focused_Disable76, 140, 157, 189, 206)

SHOW_ITEM_RECT(CMD_Rectangle, Skin80Rect, 0, 0, 49, 49, 0, 0, 1, 1, 0, 0, 0)
SHOW_MAP_BEGIN(Skin80)
SHOW_MAP_BODY(Skin80Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin80, 0, 0, 49, 49)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon81, 11, 11, 38, 38, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Pressed79)
SHOW_MAP_BODY(Skin80)
SHOW_MAP_BODY(Icon81)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Pressed79, 140, 157, 189, 206)

CREATE_BUTTON_ITEM_DATA(ICON_MENU_RETURN82, STRID_NULL, ICON_MENU_RETURN, STATUS_ENABLE, NULL)
SHOW_MAP_BEGIN(ButtonDown)
SHOW_MAP_BODY(Normal67)
SHOW_MAP_BODY(Focused70)
SHOW_MAP_BODY(Normal_Disable73)
SHOW_MAP_BODY(Focused_Disable76)
SHOW_MAP_BODY(Pressed79)
SHOW_MAP_END

DECLARE_EVENT(ButtonDown)
BUTTON_LIST_BEGIN(ButtonDown)
BUTTON_LIST_ITEM(ICON_MENU_RETURN82)
BUTTON_LIST_END

CREATE_BUTTON_DATA(ButtonDown, BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(ButtonDown, ButtonDown, 0, 57, 113, 106, 162)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndPlay_TabNavi)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_TabNavi)
CTRL_LIST_BEGIN(UIFlowWndPlay_TabNavi)
CTRL_LIST_ITEM(ButtonPlay)
CTRL_LIST_ITEM(ButtonLeft)
CTRL_LIST_ITEM(ButtonRight)
CTRL_LIST_ITEM(ButtonUp)
CTRL_LIST_ITEM(ButtonDown)
CTRL_LIST_END

CREATE_TAB_DATA(UIFlowWndPlay_TabNavi, 0)

CREATE_TAB_CTRL(UIFlowWndPlay_TabNavi, UIFlowWndPlay_TabNavi, 0, 83, 44, 84, 45)
SHOW_ITEM_RECT(CMD_Rectangle, Skin84Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin84)
SHOW_MAP_BODY(Skin84Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin84, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon85, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal83)
SHOW_MAP_BODY(Skin84)
SHOW_MAP_BODY(Icon85)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal83, 12, 5, 39, 32)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticICN_DSCMode)
SHOW_MAP_BODY(Normal83)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticICN_DSCMode)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticICN_DSCMode, ICON_MODE_PLAYBACK)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticICN_DSCMode, 0, 12, 5, 39, 32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin88Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin88)
SHOW_MAP_BODY(Skin88Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin88, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon89, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal87)
SHOW_MAP_BODY(Skin88)
SHOW_MAP_BODY(Icon89)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal87, 12, 104, 39, 131)

CREATE_STATE_ITEM_DATA(ICON_FLASH_AUTO90, STRID_NULL, ICON_FLASH_AUTO, NULL)
CREATE_STATE_ITEM_DATA(ICON_FLASH_ON91, STRID_NULL, ICON_FLASH_ON, NULL)
CREATE_STATE_ITEM_DATA(ICON_FLASH_RED_EYE92, STRID_NULL, ICON_FLASH_RED_EYE, NULL)
CREATE_STATE_ITEM_DATA(ICON_FLASH_OFF93, STRID_NULL, ICON_FLASH_OFF, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_Flash)
SHOW_MAP_BODY(Normal87)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_Flash)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_Flash)
STATE_LIST_ITEM(ICON_FLASH_AUTO90)
STATE_LIST_ITEM(ICON_FLASH_ON91)
STATE_LIST_ITEM(ICON_FLASH_RED_EYE92)
STATE_LIST_ITEM(ICON_FLASH_OFF93)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_Flash, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_Flash, 0, 12, 104, 39, 131)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin95Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin95)
SHOW_MAP_BODY(Skin95Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin95, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon96, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal94)
SHOW_MAP_BODY(Skin95)
SHOW_MAP_BODY(Icon96)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal94, 12, 38, 39, 65)

CREATE_STATE_ITEM_DATA(ICON_EV_M2P097, STRID_NULL, ICON_EV_M2P0, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_M1P698, STRID_NULL, ICON_EV_M1P6, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_M1P399, STRID_NULL, ICON_EV_M1P3, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_M1P0100, STRID_NULL, ICON_EV_M1P0, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_M0P6101, STRID_NULL, ICON_EV_M0P6, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_M0P3102, STRID_NULL, ICON_EV_M0P3, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P0P0103, STRID_NULL, ICON_EV_P0P0, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P0P3104, STRID_NULL, ICON_EV_P0P3, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P0P6105, STRID_NULL, ICON_EV_P0P6, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P1P0106, STRID_NULL, ICON_EV_P1P0, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P1P3107, STRID_NULL, ICON_EV_P1P3, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P1P6108, STRID_NULL, ICON_EV_P1P6, NULL)
CREATE_STATE_ITEM_DATA(ICON_EV_P2P0109, STRID_NULL, ICON_EV_P2P0, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_EV)
SHOW_MAP_BODY(Normal94)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_EV)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_EV)
STATE_LIST_ITEM(ICON_EV_M2P097)
STATE_LIST_ITEM(ICON_EV_M1P698)
STATE_LIST_ITEM(ICON_EV_M1P399)
STATE_LIST_ITEM(ICON_EV_M1P0100)
STATE_LIST_ITEM(ICON_EV_M0P6101)
STATE_LIST_ITEM(ICON_EV_M0P3102)
STATE_LIST_ITEM(ICON_EV_P0P0103)
STATE_LIST_ITEM(ICON_EV_P0P3104)
STATE_LIST_ITEM(ICON_EV_P0P6105)
STATE_LIST_ITEM(ICON_EV_P1P0106)
STATE_LIST_ITEM(ICON_EV_P1P3107)
STATE_LIST_ITEM(ICON_EV_P1P6108)
STATE_LIST_ITEM(ICON_EV_P2P0109)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_EV, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_EV, 0, 12, 38, 39, 65)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin111Rect, 0, 0, 107, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin111)
SHOW_MAP_BODY(Skin111Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin111, 0, 0, 107, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text113, 1, 0, 106, 27, STRID_NULL, 0, 1280, 0, 0, 2, 1, 0, 0, 0, 18, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal110)
SHOW_MAP_BODY(Skin111)
SHOW_MAP_BODY(Text113)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal110, 201, 32, 308, 59)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_Size)
SHOW_MAP_BODY(Normal110)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_Size)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_Size, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_Size, 0, 201, 32, 308, 59)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin116Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin116)
SHOW_MAP_BODY(Skin116Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin116, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon117, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal115)
SHOW_MAP_BODY(Skin116)
SHOW_MAP_BODY(Icon117)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal115, 12, 71, 39, 98)

CREATE_STATE_ITEM_DATA(ICON_WB_AUTO118, STRID_NULL, ICON_WB_AUTO, NULL)
CREATE_STATE_ITEM_DATA(ICON_WB_DAYLIGHT119, STRID_NULL, ICON_WB_DAYLIGHT, NULL)
CREATE_STATE_ITEM_DATA(ICON_WB_CLOUDY120, STRID_NULL, ICON_WB_CLOUDY, NULL)
CREATE_STATE_ITEM_DATA(ICON_WB_TUNGSTEN121, STRID_NULL, ICON_WB_TUNGSTEN, NULL)
CREATE_STATE_ITEM_DATA(ICON_WB_FLUORESCENT122, STRID_NULL, ICON_WB_FLUORESCENT, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_WB)
SHOW_MAP_BODY(Normal115)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_WB)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_WB)
STATE_LIST_ITEM(ICON_WB_AUTO118)
STATE_LIST_ITEM(ICON_WB_DAYLIGHT119)
STATE_LIST_ITEM(ICON_WB_CLOUDY120)
STATE_LIST_ITEM(ICON_WB_TUNGSTEN121)
STATE_LIST_ITEM(ICON_WB_FLUORESCENT122)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_WB, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_WB, 0, 12, 71, 39, 98)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin124Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin124)
SHOW_MAP_BODY(Skin124Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin124, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon125, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal123)
SHOW_MAP_BODY(Skin124)
SHOW_MAP_BODY(Icon125)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal123, 280, 60, 307, 87)

CREATE_STATE_ITEM_DATA(ICON_QUALITY_FINE126, STRID_NULL, ICON_QUALITY_FINE, NULL)
CREATE_STATE_ITEM_DATA(ICON_QUALITY_NORMAL127, STRID_NULL, ICON_QUALITY_NORMAL, NULL)
CREATE_STATE_ITEM_DATA(ICON_QUALITY_BASIC128, STRID_NULL, ICON_QUALITY_BASIC, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_Quality)
SHOW_MAP_BODY(Normal123)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_Quality)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_Quality)
STATE_LIST_ITEM(ICON_QUALITY_FINE126)
STATE_LIST_ITEM(ICON_QUALITY_NORMAL127)
STATE_LIST_ITEM(ICON_QUALITY_BASIC128)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_Quality, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_Quality, 0, 280, 60, 307, 87)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin130Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin130)
SHOW_MAP_BODY(Skin130Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin130, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon131, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal129)
SHOW_MAP_BODY(Skin130)
SHOW_MAP_BODY(Icon131)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal129, 280, 92, 307, 119)

CREATE_STATE_ITEM_DATA(ICON_SHARPNESS_HIGH132, STRID_NULL, ICON_SHARPNESS_HIGH, NULL)
CREATE_STATE_ITEM_DATA(ICON_SHARPNESS_MED133, STRID_NULL, ICON_SHARPNESS_MED, NULL)
CREATE_STATE_ITEM_DATA(ICON_SHARPNESS_LOW134, STRID_NULL, ICON_SHARPNESS_LOW, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_Sharpness)
SHOW_MAP_BODY(Normal129)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_Sharpness)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_Sharpness)
STATE_LIST_ITEM(ICON_SHARPNESS_HIGH132)
STATE_LIST_ITEM(ICON_SHARPNESS_MED133)
STATE_LIST_ITEM(ICON_SHARPNESS_LOW134)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_Sharpness, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_Sharpness, 0, 280, 92, 307, 119)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin136Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin136)
SHOW_MAP_BODY(Skin136Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin136, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon137, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal135)
SHOW_MAP_BODY(Skin136)
SHOW_MAP_BODY(Icon137)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal135, 280, 125, 307, 152)

CREATE_STATE_ITEM_DATA(ICON_INTERNAL_FLASH138, STRID_NULL, ICON_INTERNAL_FLASH, NULL)
CREATE_STATE_ITEM_DATA(ICON_SD_CARD139, STRID_NULL, ICON_SD_CARD, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_Storage)
SHOW_MAP_BODY(Normal135)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_Storage)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_Storage)
STATE_LIST_ITEM(ICON_INTERNAL_FLASH138)
STATE_LIST_ITEM(ICON_SD_CARD139)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_Storage, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_Storage, 0, 280, 125, 307, 152)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin141Rect, 0, 0, 31, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin141)
SHOW_MAP_BODY(Skin141Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin141, 0, 0, 31, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon142, 0, 0, 31, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal140)
SHOW_MAP_BODY(Skin141)
SHOW_MAP_BODY(Icon142)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal140, 276, 207, 307, 234)

CREATE_STATE_ITEM_DATA(ICON_BATTERY_FULL143, STRID_NULL, ICON_BATTERY_FULL, NULL)
CREATE_STATE_ITEM_DATA(ICON_BATTERY_MED144, STRID_NULL, ICON_BATTERY_MED, NULL)
CREATE_STATE_ITEM_DATA(ICON_BATTERY_LOW145, STRID_NULL, ICON_BATTERY_LOW, NULL)
CREATE_STATE_ITEM_DATA(ICON_BATTERY_EMPTY146, STRID_NULL, ICON_BATTERY_EMPTY, NULL)
CREATE_STATE_ITEM_DATA(ICON_BATTERY_ZERO147, STRID_NULL, ICON_BATTERY_ZERO, NULL)
CREATE_STATE_ITEM_DATA(ICON_BATTERY_CHARGE148, STRID_NULL, ICON_BATTERY_CHARGE, NULL)
SHOW_MAP_BEGIN(UIFlowWndPlay_StatusICN_Battery)
SHOW_MAP_BODY(Normal140)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StatusICN_Battery)
STATE_LIST_BEGIN(UIFlowWndPlay_StatusICN_Battery)
STATE_LIST_ITEM(ICON_BATTERY_FULL143)
STATE_LIST_ITEM(ICON_BATTERY_MED144)
STATE_LIST_ITEM(ICON_BATTERY_LOW145)
STATE_LIST_ITEM(ICON_BATTERY_EMPTY146)
STATE_LIST_ITEM(ICON_BATTERY_ZERO147)
STATE_LIST_ITEM(ICON_BATTERY_CHARGE148)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPlay_StatusICN_Battery, 0)
CREATE_STATE_CTRL(UIFlowWndPlay_StatusICN_Battery, 0, 276, 207, 307, 234)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin150Rect, 0, 0, 107, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin150)
SHOW_MAP_BODY(Skin150Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin150, 0, 0, 107, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text152, 0, 0, 107, 27, STRID_NULL, 0, 1280, 0, 0, 2, 1, 1, 0, 0, 18, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal149)
SHOW_MAP_BODY(Skin150)
SHOW_MAP_BODY(Text152)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal149, 201, 5, 308, 32)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_Filename)
SHOW_MAP_BODY(Normal149)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_Filename)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_Filename, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_Filename, 0, 201, 5, 308, 32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin155Rect, 0, 0, 27, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin155)
SHOW_MAP_BODY(Skin155Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin155, 0, 0, 27, 27)

SHOW_ITEM_IMAGE(CMD_Image | CMD_ITEM, Icon156, 0, 0, 27, 27, ICONID_NULL, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal154)
SHOW_MAP_BODY(Skin155)
SHOW_MAP_BODY(Icon156)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal154, 50, 5, 77, 32)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticICN_Protect)
SHOW_MAP_BODY(Normal154)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticICN_Protect)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticICN_Protect, ICON_LOCK)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticICN_Protect, 0, 50, 5, 77, 32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin159Rect, 0, 0, 107, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin159)
SHOW_MAP_BODY(Skin159Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin159, 0, 0, 107, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text161, 0, 0, 107, 27, STRID_NULL, 0, 1280, 0, 0, 2, 1, 1, 0, 0, 18, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal158)
SHOW_MAP_BODY(Skin159)
SHOW_MAP_BODY(Text161)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal158, 200, 154, 307, 181)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_Date)
SHOW_MAP_BODY(Normal158)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_Date)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_Date, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_Date, 0, 200, 154, 307, 181)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin164Rect, 0, 0, 107, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin164)
SHOW_MAP_BODY(Skin164Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin164, 0, 0, 107, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text166, 0, 0, 107, 27, STRID_NULL, 0, 1280, 0, 0, 2, 1, 1, 0, 0, 18, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal163)
SHOW_MAP_BODY(Skin164)
SHOW_MAP_BODY(Text166)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal163, 200, 181, 307, 208)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_Time)
SHOW_MAP_BODY(Normal163)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_Time)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_Time, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_Time, 0, 200, 181, 307, 208)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin169Rect, 0, 0, 81, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin169)
SHOW_MAP_BODY(Skin169Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin169, 0, 0, 81, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text171, 1, 0, 80, 27, STRID_NULL, 0, 1280, 0, 0, 2, 1, 1, 0, 0, 16, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal168)
SHOW_MAP_BODY(Skin169)
SHOW_MAP_BODY(Text171)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal168, 227, 32, 308, 59)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_MovPlayTime)
SHOW_MAP_BODY(Normal168)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_MovPlayTime)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_MovPlayTime, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_MovPlayTime, 0, 227, 32, 308, 59)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin174Rect, 0, 0, 155, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin174)
SHOW_MAP_BODY(Skin174Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin174, 0, 0, 155, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text176, 28, -2, 152, 26, STRID_NULL, 0, 1280, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0)
SHOW_ITEM_IMAGE(CMD_Image, Image177, 0, 0, 28, 24, ICON_DIR_LEFT, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal173)
SHOW_MAP_BODY(Skin174)
SHOW_MAP_BODY(Text176)
SHOW_MAP_BODY(Image177)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal173, 5, 211, 160, 238)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_LeftBtn)
SHOW_MAP_BODY(Normal173)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_LeftBtn)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_LeftBtn, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_LeftBtn, 0, 5, 211, 160, 238)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle, Skin180Rect, 0, 0, 145, 27, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Skin180)
SHOW_MAP_BODY(Skin180Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Skin180, 0, 0, 145, 27)

SHOW_ITEM_TEXT(CMD_Text | CMD_ITEM, Text182, 0, -2, 115, 25, STRID_NULL, 0, 1280, 0, 0, 2, 1, 1, 0, 0, 2, 0, 0, 0, 0)
SHOW_ITEM_IMAGE(CMD_Image, Image183, 119, 0, 147, 24, ICON_DIR_RIGHT, 0, 0, 0, 0, 0, 0, 0, 0)
SHOW_MAP_BEGIN(Normal179)
SHOW_MAP_BODY(Skin180)
SHOW_MAP_BODY(Text182)
SHOW_MAP_BODY(Image183)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group, Normal179, 169, 211, 314, 238)

SHOW_MAP_BEGIN(UIFlowWndPlay_StaticTXT_RightBtn)
SHOW_MAP_BODY(Normal179)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay_StaticTXT_RightBtn)
CREATE_STATIC_DATA(UIFlowWndPlay_StaticTXT_RightBtn, STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPlay_StaticTXT_RightBtn, 0, 169, 211, 314, 238)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndPlay)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPlay)
DECLARE_CTRL_LIST(UIFlowWndPlay)
CREATE_CTRL(UIFlowWndPlay, UIFlowWndPlay, CTRL_WND, NULL, 0, 0, 0, 319, 239)
#endif
