#ifndef UI_PLAYINFO_H
#define UI_PLAYINFO_H

#include "UIInfo.h"

typedef enum {
	SLIDE_3SEC = 0,
	SLIDE_5SEC,
	SLIDE_10SEC,
	SLIDE_SETTING_MAX
} SLIDE_SETTING;

typedef enum {
	VM_ON = 0,
	VM_OFF,
	VM_SETTING_MAX
} VM_SETTING;

typedef enum {
	PB_EFFECT_NORMAL = 0,
	PB_EFFECT_VIVID,
	PB_EFFECT_MAX
} PB_EFFECT_SETTING;

extern void Set_SlideIndex(UINT32 uhSelect);
extern UINT32 Get_SlideIndex(void);
extern void Set_VoiceMemoIndex(UINT32 ubIndex);
extern UINT32 Get_VoiceMemoIndex(void);

extern void Set_DPOFPrtIndex(UINT32 ubIndex);
extern UINT32 Get_DPOFPrtIndex(void);
extern void Set_DPOFDateOnIndex(UINT32 ubIndex);
extern UINT32 Get_DPOFDateOnIndex(void);
extern UINT32 Get_SlideValue(UINT32 uhIndex);

extern void Set_MenuItem_Copy_Status(UINT32 uiStatus);
extern void Set_MenuItem_Slide_Status(UINT32 uiStatus);
extern void Set_MenuItem_DPOF_Status(UINT32 uiStatus);
//#NT#2007/07/23@Steven Wang disabled playback list function begin
extern void Set_MenuItem_Delete_Status(UINT32 uiStatus);
extern void Set_MenuItem_Protect_Status(UINT32 uiStatus);
extern void Set_MenuItem_Thumbnail_Status(UINT32 uiStatus);
//#NT#2007/07/23@Steven Wang disabled playback list function end
/*
extern DDX_INDEX ddx_Slide ;
extern DDX_INDEX ddx_VoiceMemo ;
*/
#endif //UI_PLAYINFO_H
