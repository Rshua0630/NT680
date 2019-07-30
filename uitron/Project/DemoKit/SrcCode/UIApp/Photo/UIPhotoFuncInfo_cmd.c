//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#if(FACE_GRADE_TEST==ENABLE)
#include "FaceGIpcAPI.h"
#endif

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiPhotoFuncCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

BOOL Cmd_face_grade(CHAR *strCmd)
{
#if(FACE_GRADE_TEST==ENABLE)
	FaceGIpc_Open(OS_GetMempoolAddr(POOL_ID_FACEGIPC), POOL_SIZE_FACEGIPC);
#else
	DBG_ERR("FACE_GRADE_TEST is not turned on\r\n");
#endif
	return TRUE;
}

SXCMD_BEGIN(pfunc, "photofunc command")
SXCMD_ITEM("face_grade", Cmd_face_grade, "start face grade task")
SXCMD_END()
