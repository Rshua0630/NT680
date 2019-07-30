/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationWnd.c
    @ingroup    mIPRJAPTest

    @brief      Calibration Window
                Calibration Window

    @note       Nothing.

    @date       2007/12/10
*/

/** \addtogroup mIPRJAPTest */
//@{

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#if (CALIBRATION_FUNC == ENABLE)
#include "CalibrationWndRes.c"
#include "CalibrationWnd.h"
//#include "FlowMode.h"
#include "UIMode.h"
#include "UIDisplay.h"
#include "DynamicMenu.h"
#include "EngineerMode.h"
// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
#include "UIControlWnd.h"
#include "AppControl.h"
#include "UICommon.h"
#include "GxTimer.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          CalWnd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//UINT32  g_IsautoMTF = FALSE;

#define CALIBRATE_STRING_AREA_NUMBER      11
#define TIMER_HALF_SEC                   500
#define TIMER_ONE_SEC                   1000

extern void Cal_ItemClearScreen(void);
extern UINT8 FocusOffsetAdjust(void);
extern void Cal_ItemCountDown(UINT8 uiSec);

//CAL_AREA_PARAM gCalibrationAreaParam = {0};
CAL_AREA_PARAM gCalibrationMultiAreaParam[CALIBRATE_STRING_AREA_NUMBER] = {0};
static BOOL gCalibrationClrOSD = TRUE;
static UINT32 guiDMenuSelectNum = 0;


//---------------------CalibrationWndCtrl Control List---------------------------
CTRL_LIST_BEGIN(CalibrationWnd)
CTRL_LIST_ITEM(CalibrationMenu)
CTRL_LIST_ITEM(CalibrationStringArea)
CTRL_LIST_END

//----------------------CalibrationWndCtrl Event---------------------------
INT32 CalibrationWnd_OnOpen(VControl *, UINT32, UINT32 *);
INT32 CalibrationWnd_OnClose(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(CalibrationWnd)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,CalibrationWnd_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,CalibrationWnd_OnClose)
EVENT_END

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
static void Cal_DrawShowText(UIScreen ScreenObj,ITEM_TEXT* pShowObj)
{
    IRECT OrgPos ={0};
    IPOINT CurPoint ={0};
    UIDMenuInfo *dMenuInfo = CalibrationMenuCtrl.usrData;
    UINT32 i;
    CHAR *pStr = (CHAR *)pShowObj->Content;

    pShowObj->uiTextColor = _OSD_INDEX_RED;

    if(pStr)
    {
        if((UINT32)pStr & TEXT_POINTER)
        {
            pStr = (CHAR *)((UINT32)pStr & ~TEXT_POINTER);
        }

        for (i= 0 ; i< dMenuInfo->uiMaxItem;i++)
        {
            if(!strcmp(dMenuInfo->menuItem[i].pItemName, pStr))
            {
                if(dMenuInfo->menuItem[i].ChkStatusFunc)
                {
                    pShowObj->uiTextColor = dMenuInfo->menuItem[i].ChkStatusFunc() ? _OSD_INDEX_GREEN : _OSD_INDEX_RED;
                }
            }
        }
        //DBG_IND("^GStr=%s, Clr:%d\r\n", pStr, pShowObj->uiTextColor);
    }
    else
    {
        DBG_WRN("NULL str\r\n");
    }

    GxGfx_SetTextColor(pShowObj->uiTextColor,pShowObj->uiShadowColor,pShowObj->uiLineColor);
    GxGfx_SetTextStroke(Ux_GetFontTable((UINT32)pShowObj->uiFontTable), pShowObj->uiFontStyle, 0);
    GxGfx_SetTextLayout(pShowObj->uiLayout,pShowObj->uiAlignment,pShowObj->uiLineHeight, pShowObj->uiLetterSpace, pShowObj->uiIndentSpace);
    if(pShowObj->uiCLipping & LAYOUT_CLIPPING)
    {
        OrgPos = GxGfx_GetWindow(((DC**)ScreenObj)[GxGfx_OSD]);
        CurPoint = GxGfx_GetOrigin(((DC**)ScreenObj)[GxGfx_OSD]);
            GxGfx_SetWindow(((DC**)ScreenObj)[GxGfx_OSD],
        CurPoint.x+pShowObj->x1,CurPoint.y+pShowObj->y1,CurPoint.x+pShowObj->x2,CurPoint.y+pShowObj->y2); //clipping text by text rect
        DBG_IND("clipping %d %d %d %d  \r\n",CurPoint.x+pShowObj->x1,CurPoint.y+pShowObj->y1,CurPoint.x+pShowObj->x2,CurPoint.y+pShowObj->y2);

    }
    if(pShowObj->Content&TEXT_POINTER)
    {
        pShowObj->Content  =(pShowObj->Content)&~TEXT_POINTER;

        GxGfx_TextPrintInRect(((DC**)ScreenObj)[GxGfx_OSD],  pShowObj->x1,pShowObj->y1,pShowObj->x2,pShowObj->y2 ,"%s" ,pShowObj->Content);
    }
    else
    {
        GxGfx_TextInRect(((DC**)ScreenObj)[GxGfx_OSD],  pShowObj->x1,pShowObj->y1,pShowObj->x2,pShowObj->y2 , GxGfx_GetStringID((UINT16)pShowObj->Content));
    }

    if(pShowObj->uiCLipping & LAYOUT_CLIPPING)
    {
                GxGfx_SetWindow(((DC**)ScreenObj)[GxGfx_OSD],
        OrgPos.x,OrgPos.y,OrgPos.w-1,OrgPos.h-1); //clipping text by original pos
    }


}

INT32 CalibrationWnd_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^C(7) CalibrationWnd_OnOpen\r\n");
    UIDMenuInfo *pDMenuInfo=0;

    if ((EngineerMode_CheckEng()) && (CALIBRATION_FUNC == ENABLE)) {
		Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);
    }

    Input_SetKeyMask(KEY_PRESS, FLGKEY_UP|FLGKEY_DOWN|FLGKEY_SHUTTER2);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_UP|FLGKEY_DOWN|FLGKEY_SHUTTER2);
    Input_SetKeyMask(KEY_CONTINUE, 0);

    // Disable auto power off
    GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN,FALSE);
    // Disable sleep mode
    GxPower_SetControl(GXPWR_CTRL_SLEEP_EN,FALSE);

    if(paramNum ==1)
    {
        pDMenuInfo = (UIDMenuInfo *)paramArray[0];
    }

    // Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
    Ux_SetShowObjFunc(CMD_Text, (SHOWOBJ_FUNC_PTR)Cal_DrawShowText);

    UxCtrl_SetShow(&CalibrationMenuCtrl, TRUE);

    /*
    if (g_IsautoMTF == TRUE)
    {
        Cal_ItemClearScreen();
        UxCtrl_SetShow(&CalibrationStringAreaCtrl, TRUE);
        FocusOffsetAdjust();
        Cal_ItemCountDown(10);
        g_IsautoMTF = FALSE;
    }
    */
    if (pDMenuInfo->uiSelectNum != NO_DEFAULT_RUN)
    {
        if (pDMenuInfo->uiSelectNum)
            pDMenuInfo->uiSelectNum --;
        DBG_IND("uiSelectNum = %d \r\n",pDMenuInfo->uiSelectNum);
        //init menu item
        DMenu_InitItem(&CalibrationMenuCtrl,pDMenuInfo);

        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 2, NVTEVT_KEY_PRESS,pDMenuInfo->uiSelectNum);
    }
    else
    {
        pDMenuInfo->uiSelectNum = guiDMenuSelectNum;
        UxCtrl_SetShow(&CalibrationStringAreaCtrl, FALSE);
        //init menu item
        DMenu_InitItem(&CalibrationMenuCtrl,pDMenuInfo);

    }

    if (IsEngineerModeChange())
    {
        SetEngineerModeChange(FALSE);
        // excute test when mode change done.
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2 , 1, NVTEVT_KEY_PRESS);
    }

    USB_SetForceMsdcNvt(TRUE);

    return NVTEVT_CONSUME;
}

INT32 CalibrationWnd_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    // Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
    if ((EngineerMode_CheckEng()) && (CALIBRATION_FUNC == ENABLE)) {
		Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);
    }
    Ux_SetShowObjFunc(CMD_Text, (SHOWOBJ_FUNC_PTR)0);

    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);

    USB_SetForceMsdcNvt(FALSE);
    return NVTEVT_CONSUME;
}
//----------------------CalibrationMenuCtrl Event---------------------------
INT32 CalibrationMenu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 CalibrationMenu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 CalibrationMenu_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 CalibrationMenu_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 CalibrationMenu_OnKeyShutter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(CalibrationMenu)
EVENT_ITEM(NVTEVT_KEY_PREV,CalibrationMenu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_NEXT,CalibrationMenu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_SELECT,CalibrationMenu_OnKeyEnter)
#if (MODEKEY_FUNCTION == ENABLE)
EVENT_ITEM(NVTEVT_KEY_MODE,CalibrationMenu_OnKeyMode)
#endif
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,CalibrationMenu_OnKeyShutter)

EVENT_END

INT32 CalibrationMenu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiEvtKeyAct = 0;
    UIDMenuInfo *dMenuInfo = pCtrl->usrData;

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
             DMenu_PreItem(pCtrl,paramNum,paramArray);
             guiDMenuSelectNum = dMenuInfo->uiSelectNum;
             break;
        case NVTEVT_KEY_RELEASE:
        default:
            break;

    }
    return NVTEVT_CONSUME;
}
INT32 CalibrationMenu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiEvtKeyAct = 0;
    UIDMenuInfo *dMenuInfo = pCtrl->usrData;

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
             DMenu_NextItem(pCtrl,paramNum,paramArray);
             guiDMenuSelectNum = dMenuInfo->uiSelectNum;
             break;
        case NVTEVT_KEY_RELEASE:
        default:
            break;

    }
    return NVTEVT_CONSUME;
}

INT32 CalibrationMenu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIDMenuInfo *dMenuInfo = pCtrl->usrData;

    UINT32 uiEvtKeyAct = 0;

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
            if (UxCtrl_IsShow(&CalibrationMenuCtrl))
            {
                UxCtrl_SetShow(&CalibrationMenuCtrl, FALSE);
                Ux_Redraw();
                // Run test process
                if( dMenuInfo->menuItem[dMenuInfo->uiSelectNum].ItemFunction != NULL)
                {
                    dMenuInfo->menuItem[dMenuInfo->uiSelectNum].ItemFunction();
                    UxCtrl_SetShow(&CalibrationMenuCtrl, TRUE);
                    //UxCtrl_SetShow(&CalibrationStringAreaCtrl, FALSE);
                    Ux_Redraw();
                }
                else
                {
                    // Enable auto power off
                    SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
                    // Enable sleep mode
                    SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID,TRUE);

                    //#NT#2010/02/26#Lincy Lin -begin
                    g_bCalbirationOpened = FALSE;
                    //#NT#2010/02/26#Lincy Lin -end

                    guiDMenuSelectNum = 0;

                    //Ux_CloseWindow(pCtrl,0);
                    Ux_CloseWindow((VControl *)(&CalibrationWndCtrl), 0);

                    // Return to photo task
                    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetBootFirstMode());
                }
            }
            else
            {
                UxCtrl_SetShow(&CalibrationMenuCtrl, TRUE);
                UxCtrl_SetShow(&CalibrationStringAreaCtrl, FALSE);
                //Ux_Redraw();
            }

             break;
        case NVTEVT_KEY_RELEASE:
        default:
            break;

    }
    return NVTEVT_CONSUME;
}

INT32 CalibrationMenu_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //Ux_OpenWindow((VControl *)(&FlowModeCtrl),0);
    return NVTEVT_CONSUME;
}

INT32 CalibrationMenu_OnKeyShutter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^C(8) CalibrationMenu_OnKeyShutter\r\n");
    UIDMenuInfo *dMenuInfo = pCtrl->usrData;

    UINT32 uiEvtKeyAct = 0;

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
            if (UxCtrl_IsShow(&CalibrationMenuCtrl))
            {
                UxCtrl_SetShow(&CalibrationMenuCtrl, FALSE);
                Ux_Redraw();
                // Run test process
                if(paramNum>1)
                    dMenuInfo->uiSelectNum=paramArray[1];
                if( dMenuInfo->menuItem[dMenuInfo->uiSelectNum].ItemFunction != NULL)
                {
                    dMenuInfo->menuItem[dMenuInfo->uiSelectNum].ItemFunction();
                    //UxCtrl_SetShow(&CalibrationMenuCtrl, TRUE);
                    //UxCtrl_SetShow(&CalibrationStringAreaCtrl, FALSE);
                    //Ux_Redraw();

                    // flush key event to prevent incorrect key pressing during calibrating
                    Ux_FlushEventByRange(NVTEVT_KEY_EVT_START, NVTEVT_KEY_EVT_END);
                }
                else
                {
                    // Enable auto power off
                    SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
                    // Enable sleep mode
                    SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID,TRUE);

                    //#NT#2010/02/26#Lincy Lin -begin
                    g_bCalbirationOpened = FALSE;
                    //#NT#2010/02/26#Lincy Lin -end

                    guiDMenuSelectNum = 0;

                    //Ux_CloseWindow(pCtrl,0);
                    Ux_CloseWindow((VControl *)(&CalibrationWndCtrl), 0);

                    // Return to photo task
                    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetBootFirstMode());
                }
            }
            else
            {
                UxCtrl_SetShow(&CalibrationMenuCtrl, TRUE);
                UxCtrl_SetShow(&CalibrationStringAreaCtrl, FALSE);
                Ux_Redraw();
            }

             break;
        case NVTEVT_KEY_RELEASE:
        default:
            break;

    }
    return NVTEVT_CONSUME;
}

#if 0
{
    UINT32 uiEvtKeyAct = 0;

    DBG_IND("\r\n");

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
            // Enable auto power off
            SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
            // Enable sleep mode
            SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID,TRUE);

            Ux_CloseWindow(pCtrl,0);

            //#NT#2010/02/26#Lincy Lin -begin
            g_bCalbirationOpened = FALSE;
            //#NT#2010/02/26#Lincy Lin -end

            // Return to photo task
            UI_Switch_DscMode(DSC_MODE_MOVIE,DSCMODE_SWITCH_FORCE,DSCMODE_SWITCHDIR_DONT_CARE);

             break;
        case NVTEVT_KEY_RELEASE:
        default:
            break;

    }
    return NVTEVT_CONSUME;
}
#endif


//----------------------CalibrationStringAreaCtrl Event---------------------------
INT32 CalibrationStringArea_OnDraw(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(CalibrationStringArea)
EVENT_ITEM(NVTEVT_REDRAW,CalibrationStringArea_OnDraw)
EVENT_END

INT32 CalibrationStringArea_OnDraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DC** pDCList = (DC**)paramArray[ONEVENT_PARAM_INDEX_SCREEN];

    //GxGfx_SetTextStroke((const FONT*)&gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
    GxGfx_SetTextStroke((const FONT*)&gDemoKit_Font, FONTEFFECT_OUTLINE, SCALE_1X);

    GxGfx_SetShapeColor(CLRID_IDX_TRANSPART, CLRID_IDX_TRANSPART, 0);

    if (gCalibrationClrOSD == TRUE)
    {
        GxGfx_FillRect(pDCList[GxGfx_OSD],0,0,320,240);
    }

    // Multi-string
    {
        UINT i;

        for (i = 0; i < CALIBRATE_STRING_AREA_NUMBER; i++)
        {
            if (gCalibrationMultiAreaParam[i].bClearStr)
                GxGfx_SetTextColor(CLRID_IDX_TRANSPART, CLRID_IDX_TRANSPART, 0);

            if(gCalibrationMultiAreaParam[i].uiTextColor)
                GxGfx_SetTextColor(gCalibrationMultiAreaParam[i].uiTextColor, CLRID_IDX_BLACK50, 0);
            else
                GxGfx_SetTextColor(CLRID_IDX_RED, CLRID_IDX_BLACK, 0);

            GxGfx_Text(pDCList[GxGfx_OSD], gCalibrationMultiAreaParam[i].rect.x, gCalibrationMultiAreaParam[i].rect.y, gCalibrationMultiAreaParam[i].Str);

            if (gCalibrationMultiAreaParam[i].bShowRect)
            {
                GxGfx_SetShapeStroke(LINEBRUSH_SQUARE|LINEWEIGHT(1), FILLSTYLE_FILL);
                GxGfx_SetShapeColor(gCalibrationMultiAreaParam[i].uiTextColor, CLRID_IDX_BLACK50, NULL);
                GxGfx_FrameRect(pDCList[GxGfx_OSD], gCalibrationMultiAreaParam[i].rect.x,gCalibrationMultiAreaParam[i].rect.y,gCalibrationMultiAreaParam[i].rect.x+gCalibrationMultiAreaParam[i].rect.w,gCalibrationMultiAreaParam[i].rect.y+gCalibrationMultiAreaParam[i].rect.h);
            }
        }
    }

    return NVTEVT_CONSUME;
}


#if 0
void Cal_ShowString(CHAR *pStr, PURECT pRect, BOOL bClear)
{
    UxCtrl_SetShow(&CalibrationStringAreaCtrl, TRUE);

    memcpy(gCalibrationAreaParam.Str, pStr, 128);
    gCalibrationAreaParam.rect.x = pRect->x;
    gCalibrationAreaParam.rect.y = pRect->y;
    gCalibrationAreaParam.rect.w = pRect->w;
    gCalibrationAreaParam.rect.h= pRect->h;
    gCalibrationAreaParam.bClearStr = bClear;
    gCalibrationAreaParam.bClearOSD = FALSE;

    Ux_Redraw();
}
#endif

void Cal_ShowMultiString(UINT32 Idx, CHAR *pStr, PURECT pRect, BOOL bClear, BOOL bShowRect)
{
    if (Idx >= CALIBRATE_STRING_AREA_NUMBER)
    {
        DBG_WRN("Area index is over 4!!\r\n");
        return;
    }

    UxCtrl_SetShow(&CalibrationStringAreaCtrl, TRUE);

    memcpy(gCalibrationMultiAreaParam[Idx].Str, pStr, 128);
    gCalibrationMultiAreaParam[Idx].rect.x = pRect->x;
    gCalibrationMultiAreaParam[Idx].rect.y = pRect->y;
    gCalibrationMultiAreaParam[Idx].rect.w = pRect->w;
    gCalibrationMultiAreaParam[Idx].rect.h= pRect->h;
    gCalibrationMultiAreaParam[Idx].bClearStr = bClear;
    gCalibrationMultiAreaParam[Idx].bClearOSD = FALSE;
    gCalibrationMultiAreaParam[Idx].bShowRect = bShowRect;

    //Ux_Redraw();
}


void Cal_SetClrOSD(BOOL flag)
{
    gCalibrationClrOSD = flag;
}

void  Cal_SetDispColor(UINT32 Idx, UINT32 uiTextColor)
{
    //gCalibrationAreaParam.uiTextColor = uiTextColor;
    gCalibrationMultiAreaParam[Idx].uiTextColor = uiTextColor;
}

void  Cal_ForceExitWnd(void)
{
    // Enable auto power off
    SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
    // Enable sleep mode
    SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID,TRUE);

    //#NT#2010/02/26#Lincy Lin -begin
    g_bCalbirationOpened = FALSE;
    //#NT#2010/02/26#Lincy Lin -end

    guiDMenuSelectNum = 0;

    //Ux_CloseWindow(pCtrl,0);
    Ux_CloseWindow((VControl *)(&CalibrationWndCtrl), 0);

    // Return to photo task
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetBootFirstMode());
}

#endif

//@}
