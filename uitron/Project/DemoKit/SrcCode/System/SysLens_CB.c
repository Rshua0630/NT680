////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "Lens.h"
#include "GxFlash.h"
#include "ImageApp_Photo.h" //for UIMovie_UpdateLensState
#include "UIAppMovie.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysLensExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

BOOL    bIsShuterPseudoopen = FALSE;

void Photo_LensCB(UINT32 MessageID, UINT32 *Parameter)
{
#if (LENS_FUNCTION == ENABLE)
	switch (MessageID) {
	case LENS_CB_CHGOZOOMSTEP:
		///W Ux_PostEvent(NVTEVT_CB_ZOOM, 0);
		break;
	case LENS_CB_SHUTTER_PSEUDO_OPEN:
		DBG_IND("===LENS_CB_SHUTTER_PSEUDO_OPEN===\r\n");
		bIsShuterPseudoopen = TRUE;
		break;
	case LENS_CB_SHUTTER_PSEUDO_CLOSE:
		DBG_IND("===LENS_CB_SHUTTER_PSEUDO_CLOSE===\r\n");
		bIsShuterPseudoopen = FALSE;
		break;
	}
#endif
}

void LensAPICB(UINT32 MessageID, UINT32 *Parameter)
{
#if (LENS_FUNCTION == ENABLE)
	DBG_IND("MessageID=%d\r\n", MessageID);

	switch (MessageID) {
	case LENS_CB_INITZOOM_START:
	case LENS_CB_INITFOCUS_START:
	case LENS_CB_INITAPERTURE_START:
	case LENS_CB_ZOOM_START:
	case LENS_CB_FOCUS_START:
	case LENS_CB_APERTURE_START:
	case LENS_CB_SHUTTER_START:
	case LENS_CB_IRCUT_START:
		///W UIMovie_UpdateLensState(MessageID);    // API function for Lens state updating
		break;
	case LENS_CB_INITZOOM_END:
	case LENS_CB_INITFOCUS_END:
	case LENS_CB_INITAPERTURE_END:
	case LENS_CB_ZOOM_END:
	case LENS_CB_FOCUS_END:
	case LENS_CB_APERTURE_END:
	case LENS_CB_SHUTTER_END:
	case LENS_CB_IRCUT_END:
		///W UIMovie_UpdateLensState(MessageID);    // API function for Lens state updating
		break;
	case LENS_CB_RESET_START:
		break;
	case LENS_CB_RESET_END:
		break;
	default:
		DBG_ERR("Parameter error in LensAPICB() (%d)\r\n", MessageID);
		break;
	}
#endif
}



