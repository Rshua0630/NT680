#ifndef UIFLOW_PHOTO_PARAMS_H
#define UIFLOW_PHOTO_PARAMS_H


#include "Type.h"
#include "KeyDef.h"



//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
// Photo mode setting params list
#define TOTAL_PHOTO_PARAMS_SETS             3
#define PHOTOPARAMS_USER_DEFINED            0xFFFF


// Photo mode type key mask
#define PHOTO_AUTO_KEY_PRESS_MASK           FLGKEY_KEY_MASK_DEFAULT
#define PHOTO_AUTO_KEY_RELEASE_MASK         FLGKEY_KEY_MASK_DEFAULT

#define PHOTO_PORTRAIT_KEY_PRESS_MASK       FLGKEY_KEY_MASK_DEFAULT
#define PHOTO_PORTRAIT_KEY_RELEASE_MASK     FLGKEY_KEY_MASK_DEFAULT

#define SCN_LANDSCAPE_KEY_PRESS_MASK        FLGKEY_KEY_MASK_DEFAULT
#define SCN_LANDSCAPE_KEY_RELEASE_MASK      FLGKEY_KEY_MASK_DEFAULT


extern UINT32 gResetAll;

//-----------------------------------------------------------------------------
// Photo Setting Params Info
//-----------------------------------------------------------------------------
typedef struct _UIPhotoSettingParamsInfo {
	/*--- General ---*/
	CHAR        *pNameString;
	UINT32      uiSceneType;
	UINT32      uiInitMaskKeyPress;
	UINT32      uiInitMaskKeyRelease;

	/*--- Menu ---*/
	UINT16      uhIdxFlash;
	UINT16      uhIdxMacro;
	UINT16      uhIdxEV;
	UINT16      uhIdxSelftimer;
	UINT16      uhIdxAFWindow;
	UINT16      uhIdxAFBeam;
	UINT16      uhIdxMetering;
	UINT16      uhIdxColor;
	UINT16      uhIdxSize;
	UINT16      uhIdxQuality;
	UINT16      uhIdxISO;
	UINT16      uhIdxWB;

	/*--- Hidden ---*/
	UINT16      uhIdxSaturation;
	UINT16      uhIdxSharpness;
	UINT16      uhIdxContrast;
} UIPhotoSettingParamsInfo;


extern UIPhotoSettingParamsInfo *GetPhotoParamsInfo(void);
extern void SetPhotoParamsInfo(UIPhotoSettingParamsInfo *pInfo);
extern void BackupPhotoParamsInfo(void);
extern void RestorePhotoParamsInfo(void);
extern /*const */UIPhotoSettingParamsInfo UIPhotoParams[TOTAL_PHOTO_PARAMS_SETS];

#endif
