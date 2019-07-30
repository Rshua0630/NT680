
#include "PrjCfg.h"
#include "UIPhotoInfo.h"
#include "UIFlowPhotoParams.h"


static UIPhotoSettingParamsInfo *pPhotoParamsInfo = NULL;

//-----------------------------------------------------------------------------
// Get/Set Current Photo Mode Setting Object
//-----------------------------------------------------------------------------
UIPhotoSettingParamsInfo *GetPhotoParamsInfo(void)
{
	return pPhotoParamsInfo;
}
void SetPhotoParamsInfo(UIPhotoSettingParamsInfo *pInfo)
{
	pPhotoParamsInfo = pInfo;
}

static UIPhotoSettingParamsInfo tmpUIPhotoParams[TOTAL_PHOTO_PARAMS_SETS] = {0};


//-----------------------------------------------------------------------------
// Photo Mode Setting Params Data
//-----------------------------------------------------------------------------
/*const */UIPhotoSettingParamsInfo UIPhotoParams[TOTAL_PHOTO_PARAMS_SETS] = {
	//SCENE_AUTO
	{
		/*--- General ---*/
		"PHOTO AUTO PARAMS",                    // Params Set Name
		SCENE_AUTO,                             // Scene Type
		PHOTO_AUTO_KEY_PRESS_MASK,              // Initial Mask KeyPress
		PHOTO_AUTO_KEY_RELEASE_MASK,            // Initial Mask KeyRelease

		/*--- Menu ---*/
		PHOTOPARAMS_USER_DEFINED,               // Idx Flash
		PHOTOPARAMS_USER_DEFINED,               // Idx Macro
		PHOTOPARAMS_USER_DEFINED,               // Idx EV
		PHOTOPARAMS_USER_DEFINED,               // Idx Selftimer
		PHOTOPARAMS_USER_DEFINED,               // Idx AFWindow
		PHOTOPARAMS_USER_DEFINED,               // Idx AFBeam
		PHOTOPARAMS_USER_DEFINED,               // Idx Metering
		PHOTOPARAMS_USER_DEFINED,               // Idx Color
		PHOTOPARAMS_USER_DEFINED,               // Idx Size
		PHOTOPARAMS_USER_DEFINED,               // Idx Quality
		PHOTOPARAMS_USER_DEFINED,               // Idx ISO
		PHOTOPARAMS_USER_DEFINED,               // Idx WB

		/*--- Hidden ---*/
		SATURATION_NORMAL,                      // Idx Saturation
		PHOTOPARAMS_USER_DEFINED,               // Idx Sharpness
		CONTRAST_NORMAL,                        // Idx Contrast
	},


	//SCENE_PORTRAIT
	{
		/*--- General ---*/
		"PHOTO PORTRAIT PARAMS",                // Params Set Name
		SCENE_PORTRAIT,                         // Scene Type
		PHOTO_PORTRAIT_KEY_PRESS_MASK,          // Initial Mask KeyPress
		PHOTO_PORTRAIT_KEY_RELEASE_MASK,        // Initial Mask KeyRelease

		/*--- Menu ---*/
		FLASH_RED_EYE,                          // Idx Flash
		MACRO_OFF,                              // Idx Macro
		EV_P13,                                 // Idx EV
		SELFTIMER_OFF,                          // Idx Selftimer
		AFWINDOW_SINGLE,                        // Idx AFWindow
		PHOTOPARAMS_USER_DEFINED,               // Idx AFBeam
		METERING_CENTER,                        // Idx Metering
		COLOR_STANDARD,                         // Idx Color
		PHOTOPARAMS_USER_DEFINED,               // Idx Size
		PHOTOPARAMS_USER_DEFINED,               // Idx Quality
		ISO_AUTO,                               // Idx ISO
		WB_AUTO,                                // Idx WB

		/*--- Hidden ---*/
		SATURATION_NORMAL,                      // Idx Saturation
		SHARPNESS_SOFT,                         // Idx Sharpness
		CONTRAST_N3,                            // Idx Contrast

	},

	// SCENE_LANDSCAPE
	{
		/*--- General ---*/
		"SCENE LANDSCAPE PARAMS",               // Params Set Name
		SCENE_LANDSCAPE,                        // Scene Type
		SCN_LANDSCAPE_KEY_PRESS_MASK,           // Initial Mask KeyPress
		SCN_LANDSCAPE_KEY_RELEASE_MASK,         // Initial Mask KeyRelease

		/*--- Menu ---*/
		FLASH_OFF,                              // Idx Flash
		MACRO_OFF,                              // Idx Macro
		EV_00,                                  // Idx EV
		SELFTIMER_OFF,                          // Idx Selftimer
		AFWINDOW_SINGLE,                        // Idx AFWindow
		AFBEAM_OFF,                             // Idx AFBeam
		METERING_CENTER,                        // Idx Metering
		COLOR_STANDARD,                         // Idx Color
		PHOTOPARAMS_USER_DEFINED,               // Idx Size
		PHOTOPARAMS_USER_DEFINED,               // Idx Quality
		ISO_AUTO,                               // Idx ISO
		WB_AUTO,                                // Idx WB

		/*--- Hidden ---*/
		SATURATION_P1,                          // Idx Saturation
		SHARPNESS_SHARP,                        // Idx Sharpness
		CONTRAST_P2,                            // Idx Contrast
	},
};


void BackupPhotoParamsInfo(void)
{
	memcpy(&tmpUIPhotoParams[0], &UIPhotoParams[0], TOTAL_PHOTO_PARAMS_SETS * sizeof(UIPhotoSettingParamsInfo));
}

void RestorePhotoParamsInfo(void)
{
	memcpy(&UIPhotoParams[0], &tmpUIPhotoParams[0], TOTAL_PHOTO_PARAMS_SETS * sizeof(UIPhotoSettingParamsInfo));
}


