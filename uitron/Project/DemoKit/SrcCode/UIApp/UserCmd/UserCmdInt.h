#ifndef _USERCMD_INT_H
#define _USERCMD_INT_H

#include "SysKer.h"

// Flag
#define FLGUSERCMD_IDLE         FLGPTN_BIT(0)
#define FLGUSERCMD_RUNCMD       FLGPTN_BIT(1)
#define FLGUSERCMD_ALL          0xFFFFFFFF

extern void UserCmd_BurnIn(void);
extern void UserCmd_RunScript(void);
extern void UserCmd_AudioTest(void);
extern void UserCmd_WavTest(void);
extern void UserCmd_PCC(void);
extern void UserCmdTsk(void);

#endif
