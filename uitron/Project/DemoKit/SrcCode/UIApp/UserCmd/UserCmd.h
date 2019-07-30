#ifndef _USERCMD_H
#define _USERCMD_H

#define USERCMD_RUNSCRIPT_FILE      "A:\\ntscript.txt"

enum {
	USERCMD_BURNIN,
	USERCMD_RUNSCRIPT,
	USERCMD_AUDIOTEST,
	USERCMD_WAVTEST,
	USERCMD_PCC,
	USERCMD_MAX
};

typedef struct {
	UINT32  Id;
	FP      Function;
} USERCMD_ITEM;

extern void UserCmd_InstallID(void) _SECTION(".kercfg_text");
extern ER UserCmd_Open(void);
extern ER UserCmd_Close(void);
extern UINT32 UserCmd_RunCmd(UINT32 cmd);

#endif
