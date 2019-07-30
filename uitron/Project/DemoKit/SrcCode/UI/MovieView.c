#include "PrjCfg.h"
#include "UIDisplay.h"
//#include "MovieView.h"

#include "UIAppMoviePlay.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MVVIEW
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


//extern void Movie_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);
//extern void FlowMovie_OnChangeSize(ISIZE *pSize, ISIZE *pOldSize);

//VDO1
BOOL View_Movie(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	/*
	    if(cmd == SHOWCMD_BEGINDRAW)
	    {
	        DBG_MSG("^Gbegin VDO1 src=MOVIE\r\n");
	        GxDisplay_SetSwapEffect(LAYER_VDO1, SWAPEFFECT_DISCARD); //disable double buffer
	        return TRUE;
	    }
	    if(cmd == SHOWCMD_ENDDRAW)
	    {
	        DBG_MSG("^Gend VDO1 src=MOVIE\r\n");
	        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COMPARE, COMPARE_KEY_NEVER);
	        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COLORKEY, 0);
	        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1ALPHA, OSD_OPAQUE_100);
	        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1BLEND, BLEND_DISABLE);
	        return TRUE;
	    }
	*/
	if (cmd == SHOWCMD_SETSIZE) {
		//ISIZE *pSize = (ISIZE*)param2;
		//Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
		//FlowMovie_OnChangeSize(pSize, 0);
		return TRUE;
	}
	return FALSE;
}


