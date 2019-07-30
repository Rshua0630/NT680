/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       EngineerModeTbl.c
    @ingroup    mIPRJAPTest

    @brief      Engineer Mode item table
                Engineer Mode item table

    @note       Nothing.

    @date       2007/12/18
*/

/** \addtogroup mIPRJAPTest */
//@{
#include <string.h>
#include "SysCommon.h"
#include "AppCommon.h"
#include "UICommon.h"
#include "CalibrationWnd.h"
#include "DynamicMenu.h"
#include "FileSysTsk.h"
#include "SysCfg.h"
//#include "CalCIPA.h"
//#include "CalMITF.h"
//#include "CalBurnIn.h"
//#include "CalSerNum.h"
//#include "CalEmptyVolt.h"
//#include "CalRtc.h"
//#include "CalLangSet.h"
//#include "CalMisc.h"
//#include "CalGyro.h"
#include "PStore.h"
//#include "ae_task.h"
//#include "awb_task.h"
//#include "Lens.h"
//#include "af_api.h"
#include "EngineerMode.h"
#if (CALIBRATION_FUNC == ENABLE)
//#include "CalRunScript.h"
#include "calibration_item.h"
#endif
//extern void IQ_GetAdjustRst(CalIQInfo *CalData);
#include "UserCmd.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          EngTbl
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

UIDMenuInfo UIFlowInfoEngMode =
{
    0,
    0,
    0,
    ITEMS_PER_PAGE
};
UIDMenuInfo *pEngModeDMenuInfo = &UIFlowInfoEngMode;
volatile BOOL   g_bCalbirationOpened = FALSE;
CHAR*           g_pExt;
UINT32          g_uiResult;
volatile BOOL   g_bCalibrationModeChange = FALSE;
BOOL            g_bIsEngineerMode = FALSE;

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
/*
static UINT32 ChkSelfTestStatus(void)
{
    return  (gCalSysData.RTC_Test_Status == _RTC_TEST_STATUS);
}

static UINT32 ChkBatEmptyVolt(void)
{
    return (gCalSysData.EmptyVolt_Test_Status == _EMPTYVOLT_STATUS);
}

static UINT32 ChkBurnInSts(void)
{
    return (gCalSysData.Burnin_Status== _BURNIN_STATUS);
}

static UINT32 ChkLangSts(void)
{
    return (gCalSysData.Language_Status == _LANGUAGE_STATUS);
}

static UINT32 ChkSNTestSts(void)
{
    return (gCalSysData.SerialNumber_Test_Status == _SERNUM_STATUS);
}

static UINT32 ChkGryoZeroAdjust(void)
{
    return (gCalSysData.Gyro_Zero_Adjust_Status == _GYRO_ZERO_ADJUST_STATUS);
}

static UINT32 ChkOrientAdjust(void)
{
    return (gCalSysData.Orient_Adjust_Status == _ORIENT_ADJUST_STATUS);
}


static UINT32 ChkDNPCalSts(void)
{
    return (DNP_Status_chk(NULL));
}

static UINT32 ChkISOCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.ISO_STATUS == _CAL_OK);
}

static UINT32 ChkAWBCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.AWB_STATUS == _CAL_OK);
}

static UINT32 ChkDPCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.DP_STATUS == _CAL_OK);
}

static UINT32 ChkDP_Dark_CalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return ((CalIQData.Cal_Status.DP_STATUS == _CAL_UnderFlow) || (CalIQData.Cal_Status.DP_STATUS == _CAL_OK));
}

static UINT32 ChkDP_Bright_CalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.DP_STATUS == _CAL_OK);
}

static UINT32 ChkECSCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.ECS_STATUS == _CAL_OK);
}

static UINT32 ChkAECHKCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.AECHK_STATUS == _CAL_OK);
}
*/

void Self_Test(void)
{
    CHAR     StringBuf[30];
    //URECT    tempRECT={20,60,300,40};
    snprintf(StringBuf,sizeof(StringBuf)-1,"Test OK.");
    //Cal_ShowString(StringBuf, &tempRECT, TRUE);
    //Cal_SetStringColor(_OSD_INDEX_WHITE);
}
void Cal_AudioTest(void)
{
    DBG_DUMP("^C Cal_AudioTest!\r\n");
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_AUDIOTEST);
}

void Cal_BurnIn(void)
{
    DBG_DUMP("^C(9) Cal_BurnIn!\r\n");
    Cal_ForceExitWnd();
    UserCmd_RunCmd(USERCMD_BURNIN);
}

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
DMENU_ITEM g_EngineerItem[]    =
{
    //Main Items, Intergrated Items
    {" 0. Self Test",                   Self_Test                ,NULL,     NULL},
#if CALIBRATION_FUNC
    {" 1. cal - dp bright",             cal_item_dpb				 ,NULL,     NULL},
    {" 2. cal - dp dark",               cal_item_dpd				 ,NULL,     NULL},
    {" 3. cal - awb",                 	cal_item_awb              ,NULL,     NULL},
    {" 4. cal - ecs",                 	cal_item_ecs              ,NULL,     NULL},
    {" 5. cal - iso",                 	cal_item_iso              ,NULL,     NULL},
    {" 6. cal - lenscen",             	cal_item_lenscen          ,NULL,     NULL},
    {" 7. cal - focus",                 cal_item_focus              ,NULL,     NULL},
    {" 8. cal - test",                  cal_item_test			,NULL,     NULL},
    {" 9. cal - DNP",             cal_item_dnp				 ,NULL,     NULL},
#endif
    {"10. Burn-In",                     Cal_BurnIn               ,NULL,     NULL},
    {"11. Audio Test",                  Cal_AudioTest            ,NULL,     NULL},

    {"12. Exit",                        NULL                     ,NULL,     NULL},
};

UINT32 g_EngineerMaxItemNum      = sizeof(g_EngineerItem) / sizeof(DMENU_ITEM) - 1;
UINT32 g_EngineerMaxItemNumOri  = sizeof(g_EngineerItem) / sizeof(DMENU_ITEM) - 1; // original max item


static void CalRoot_ScanCB(FIND_DATA *findData, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
{
    #define LONG_NAME_MAX   30
    #define EXT_NAME_SIZE   4

    CHAR    cFileName[LONG_NAME_MAX]; // long file name, include extension
    CHAR    ch;
    UINT32  i, j;
    UINT32  uiShift = 0, uiCompareSize = 0;

    if (!M_IsDirectory(findData->attrib))
    {
        if (!strncmp(findData->FATExtName, g_pExt, 3))
        {
// For linux file system
#if 1
			if (findData->filename[0])
			{
				uiCompareSize = strlen(findData->filename) - EXT_NAME_SIZE;
				memcpy(cFileName, findData->filename, uiCompareSize);

			}
#else
            if (cLongname[0])
            {
                memset(cFileName, 0, LONG_NAME_MAX);
                for (i = 0; i < LONG_NAME_MAX; i++)
                {
                    if (cLongname[i] == 0)
                    {
						//#NT#2016/11/30#KCHong -begin
						//#NT# coverity 14073 [Out-of-bounds access]
						//uiCompareSize = i - EXT_NAME_SIZE;
						uiCompareSize = (i < EXT_NAME_SIZE)?0:(i - EXT_NAME_SIZE);
						//#NT#2016/11/30#KCHong -end
                        break;
                    }

                    // only support ASCII
                    if (cLongname[i] < 128)
                    {
                        cFileName[i] = (CHAR)cLongname[i];
                    }
                    else
                    {
                        break;
                    }
                }
            }
#endif
            if (uiCompareSize)
            {
                for (i = 0; i <= g_EngineerMaxItemNumOri; i++)
                {
                    for (j = 0; j < LONG_NAME_MAX; j++)
                    {
                        ch = g_EngineerItem[i].pItemName[j];
                        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
                        {
                            uiShift = j;
                            break;
                        }
                    }
					//DBG_DUMP("ItemName = %s, i=%d, Shift=%d\r\n", &g_EngineerItem[i].pItemName[uiShift], i, uiShift);
					//DBG_DUMP("cFileName = %s\r\n", cFileName);
                    if (memcmp(&g_EngineerItem[i].pItemName[uiShift], cFileName, uiCompareSize) == 0)
                    {
                        g_uiResult = i + 1;
                        break;
                    }
                }
            }
        }
    }
}

/**
  Find file that filename with extension in root folder

  Find file that filename with extension in root folder and return the main
  filename (convert to integer of UINT32)
  [Calibration internal API]

  @param INT8 *pExt: extension
  @return UINT32: Filename of integer
*/

UINT32 Cal_GetDefaultInRoot(CHAR * pExt)
{
    g_uiResult = NO_DEFAULT_RUN;
    g_pExt = pExt;
#if 0
    if(FileSys_ScanDir("A:\\", CalRoot_ScanCB, FALSE, 0)!=FST_STA_OK)
#else // for long file name test
    if(FileSys_ScanDir("A:\\", CalRoot_ScanCB, TRUE, 0)!=FST_STA_OK)
#endif
    {
        DBG_ERR("Failed to FileSys_ScanDir.\r\n");
    }
    return g_uiResult;
}

BOOL EngineerMode_CheckEng(void)
{
    BOOL ret = FALSE;
    FST_FILE filehdl = NULL;

    if (System_GetState(SYS_STATE_CARD) == CARD_INSERTED)
    {
        filehdl = FileSys_OpenFile(ENG_MODE_FILE,FST_OPEN_READ);
        if (filehdl!=NULL)
        {
           FileSys_CloseFile(filehdl);
           ret = TRUE;
        }
    }
    return ret;
}

void EngineerMode_Open(void)
{
	DBG_DUMP("^C(5) EngineerMode_Open\r\n");
    UINT32  i;

    if (g_bCalbirationOpened == TRUE)
    {
        return;
    }
    g_bCalbirationOpened = TRUE;

	//#NT#2016/10/14#KCHong -begin
	//#NT#Forced use 320x240 UI for engineer mode
	// Toggle UI_Show twice to trigger UIDisplay_Init()
	UI_Show(UI_SHOW_WINDOW, FALSE);
	UI_Show(UI_SHOW_WINDOW, TRUE);
	// Forced set UI layout size to 320x240
	UI_SetLayoutSize(320, 240);
	//#NT#2016/10/14#KCHong -end

    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_DEFAULT);

    for (i = 0; i <= g_EngineerMaxItemNum; i++)
    {
        if (g_EngineerItem[i].ItemFunction == NULL)
        {
            DBG_DUMP("^Y max active cal item = %d\r\n", i);
            break;
        }
    }
    g_EngineerMaxItemNum = i;



    //Ux_OpenWindow((VControl *)(&EngModeWndCtrl), 3, g_EngineerItem,g_EngineerMaxItemNum,NO_DEFAULT_RUN);
    pEngModeDMenuInfo->menuItem = g_EngineerItem;
    pEngModeDMenuInfo->uiMaxItem = g_EngineerMaxItemNum;
    //pEngModeDMenuInfo->uiSelectNum = NO_DEFAULT_RUN;
    pEngModeDMenuInfo->uiSelectNum = Cal_GetDefaultInRoot("CAL");
    //temp use CalibrationWndCtrl UI
    Ux_OpenWindow((VControl *)(&CalibrationWndCtrl), 1, pEngModeDMenuInfo);
}

BOOL IsEngineerModeOpened(void)
{
    return g_bCalbirationOpened;
}

void EngineerMode_WndOpen(void)
{
	DBG_DUMP("^C(6) EngineerMode_WndOpen\r\n");
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_DEFAULT);

    //Ux_OpenWindow((VControl *)(&EngModeWndCtrl), 3, g_EngineerItem,g_EngineerMaxItemNum,NO_DEFAULT_RUN);
    pEngModeDMenuInfo->menuItem = g_EngineerItem;
    pEngModeDMenuInfo->uiMaxItem = g_EngineerMaxItemNum;
    //pEngModeDMenuInfo->uiSelectNum = NO_DEFAULT_RUN;
    pEngModeDMenuInfo->uiSelectNum = Cal_GetDefaultInRoot("CAL");
    //temp use CalibrationWndCtrl UI
    Ux_OpenWindow((VControl *)(&CalibrationWndCtrl), 1, pEngModeDMenuInfo);
}

void EngineerMode_WndClose(void)
{
    Ux_CloseWindow((VControl *)(&CalibrationWndCtrl), 0);
}

BOOL IsEngineerModeChange(void)
{
    return g_bCalibrationModeChange;
}

void SetEngineerModeChange(BOOL b)
{
    g_bCalibrationModeChange = b;
}

BOOL IsEngineerMode(void)
{
    return g_bIsEngineerMode;
}

//@}
