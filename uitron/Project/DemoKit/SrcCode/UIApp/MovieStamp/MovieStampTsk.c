#include "SysKer.h"
#include "MovieStampAPI.h"
#include "MovieStampInt.h"

#define __MODULE__          MovieStampTask
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

static BOOL     g_bMovieStampTskOpened = FALSE;
static MOVIESTAMP_UPDATE_CB g_MovieStampUpdateCb = NULL;

ER MovieStampTsk_Open(void)
{
	if (g_bMovieStampTskOpened) {
		return E_SYS;
	}

	clr_flg(FLG_ID_MOVIESTAMP, FLGMOVIESTAMP_ALL);
	g_bMovieStampTskOpened = TRUE;
	sta_tsk(MOVIESTAMPTSK_ID, 0);

	return E_OK;
}

ER MovieStampTsk_Close(void)
{
	FLGPTN  FlgPtn;

	if (!g_bMovieStampTskOpened) {
		return E_SYS;
	}

	wai_flg(&FlgPtn, FLG_ID_MOVIESTAMP, FLGMOVIESTAMP_IDLE, TWF_ORW);
	ter_tsk(MOVIESTAMPTSK_ID);
	g_bMovieStampTskOpened = FALSE;

	return E_OK;
}

void MovieStampTsk_TrigUpdate(void)
{
	set_flg(FLG_ID_MOVIESTAMP, FLGMOVIESTAMP_UPDATE);
}

void MovieStampTsk_RegUpdateCB(MOVIESTAMP_UPDATE_CB fpMovieStampUpdate)
{
	g_MovieStampUpdateCb = fpMovieStampUpdate;
}
BOOL MovieStampTsk_IsOpen(void)
{
    return g_bMovieStampTskOpened;
}
void MovieStampTsk(void)
{
	FLGPTN  FlgPtn;

	kent_tsk();

	while (MOVIESTAMPTSK_ID) {
		set_flg(FLG_ID_MOVIESTAMP, FLGMOVIESTAMP_IDLE);
		PROFILE_TASK_IDLE();
		wai_flg(&FlgPtn, FLG_ID_MOVIESTAMP, FLGMOVIESTAMP_UPDATE, TWF_ORW | TWF_CLR);
		PROFILE_TASK_BUSY();
		clr_flg(FLG_ID_MOVIESTAMP, FLGMOVIESTAMP_IDLE);

		if (FlgPtn & FLGMOVIESTAMP_UPDATE) {
			if (g_MovieStampUpdateCb) {
				g_MovieStampUpdateCb();
			}
		}
	}
}
//@}
