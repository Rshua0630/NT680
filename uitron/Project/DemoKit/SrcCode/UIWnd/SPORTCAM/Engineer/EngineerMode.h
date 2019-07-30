#ifndef _ENGINEERMODE_H
#define _ENGINEERMODE_H

#define ENG_MODE_FILE  "A:\\engmode"

extern volatile BOOL  g_bCalbirationOpened;

extern void Engineer_InstallID(void) _SECTION(".kercfg_text");
extern BOOL EngineerMode_CheckEng(void);
extern void EngineerMode_Open(void);
extern BOOL IsEngineerModeOpened(void);
extern void EngineerMode_WndOpen(void);
extern void EngineerMode_WndClose(void);
extern BOOL IsEngineerModeChange(void);
extern void SetEngineerModeChange(BOOL b);
extern BOOL IsEngineerMode(void);
#endif
