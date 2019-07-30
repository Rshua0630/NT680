//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
#include "SxCmd.h"
#include "UIInfo.h"
#include "NvtUctrlAPI.h"
#include "stdio.h"
#include "HwClock.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "SysExam_cmd.h"

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysExamCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (UI_FUNC == ENABLE)
static BOOL cmd_recsize(CHAR* strCmd)
{
	if (strncmp(strCmd,"MOVIE_SIZE_FRONT_3840x2160P30",strlen("MOVIE_SIZE_FRONT_3840x2160P30")) == 0) {
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_3840x2160P30);
	} else if (strncmp(strCmd,"MOVIE_SIZE_FRONT_2560x1440P60",strlen("MOVIE_SIZE_FRONT_2560x1440P60")) == 0) {
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_2560x1440P60);
	} else if (strncmp(strCmd,"MOVIE_SIZE_FRONT_1280x720P60",strlen("MOVIE_SIZE_FRONT_1280x720P60")) == 0) {
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1280x720P60);
	} else {
		exam_msg("unknown size");
	}
	return TRUE;
}
#endif

static UINT32 m_tm_preview_rdy = 0;
static UINT32 m_tm_record_rdy = 0;
void SysExam_SetTimeTag(SYSEXAM_TMTAG tag)
{
	switch(tag) {
	case SYSEXAM_TMTAG_PREVIEW_RDY:
		if (m_tm_preview_rdy == 0) {
			m_tm_preview_rdy = HwClock_GetCounter();
		}
		break;
	case SYSEXAM_TMTAG_RECORD_RDY:
		if (m_tm_record_rdy == 0 ) {
			m_tm_record_rdy = HwClock_GetCounter();
		}
		break;
	default:
		DBG_ERR("unsupported value: %d\r\n", tag);
		break;
	}
}

static BOOL cmd_uctrl_notfiy(CHAR* strCmd)
{
    NvtUctrl_Notify(strCmd);
	return TRUE;
}
static BOOL cmd_post_event(CHAR* strCmd)
{
    UINT32 evt,paramNum,param1,param2,param3;

    DBG_DUMP("%s\r\n",strCmd);

	sscanf_s(strCmd, "%d %d %d %d %d", &evt,&paramNum,&param1,&param2,&param3);

    switch(paramNum) {
        case 0:
            Ux_PostEvent(evt,paramNum);
        break;
        case 1:
            Ux_PostEvent(evt,paramNum,param1);
        break;
        case 2:
            Ux_PostEvent(evt,paramNum,param1,param2);
        break;
        case 3:
            Ux_PostEvent(evt,paramNum,param1,param2,param3);
        break;
        default:
            DBG_ERR("parm err %s\r\n",strCmd);
            break;

    }
	return TRUE;
}
static BOOL cmd_bkg_event(CHAR* strCmd)
{
    UINT32 evt;

    sscanf_s(strCmd, "%d",&evt);

    DBG_DUMP("%s\r\n",strCmd);

    BKG_PostEvent(evt);

    return TRUE;
}
static BOOL cmd_ui_redraw(CHAR* strCmd)
{
    VControl *pFocusedWnd = 0;;

    NVTRET result = Ux_GetFocusedWindow(&pFocusedWnd);

    if((result==NVTRET_OK)&&(pFocusedWnd))
    {
        DBG_DUMP("%s\r\n",pFocusedWnd->Name);

        UxCtrl_SetDirty(pFocusedWnd,TRUE);
        Ux_Redraw();
    }
    return TRUE;
}
static BOOL cmd_rm_firmware(CHAR* strCmd)
{
	//Delete Firmware in Cards
	if (FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN") != FST_STA_OK) {
		exam_msg("RM_FILE_NG");
	} else {
		exam_msg("RM_FILE_OK");
	}
	return TRUE;
}
static BOOL cmd_dump_tm(CHAR* strCmd)
{
	exam_msg("PREVIEW_RDY: %d\r\n", m_tm_preview_rdy);
	exam_msg("RECORD_RDY: %d\r\n", m_tm_record_rdy);
	return TRUE;
}


SXCMD_BEGIN(autotest, "auto test command")
#if (UI_FUNC == ENABLE)
SXCMD_ITEM("recsize %", cmd_recsize, "set rec size")
#endif
SXCMD_ITEM("notify %", cmd_uctrl_notfiy, "uctrl notify")
SXCMD_ITEM("post %", cmd_post_event, "post event")
SXCMD_ITEM("bkg %", cmd_bkg_event, "background event")
SXCMD_ITEM("redraw", cmd_ui_redraw, "redraw focus wind")
SXCMD_ITEM("rmfw", cmd_rm_firmware, "remove firmware")
SXCMD_ITEM("dumptm", cmd_dump_tm, "dump time tag")
SXCMD_END()
