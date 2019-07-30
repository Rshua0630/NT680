
#ifndef _MENUSETUP_H
#define _MENUSETUP_H

extern TM_MENU gSetupMenu;
extern TM_ITEM gTM_ITEMS_SETUP[];

extern TM_OPTION gTM_OPTIONS_LANGUAGE[LANG_ID_MAX];
extern int Setup_MenuCallback(UINT32 uiItemId, UINT32 uiOptionId);

#endif
