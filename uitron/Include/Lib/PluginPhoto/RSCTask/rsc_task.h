#ifndef _RSC_TASK_H_
#define _RSC_TASK_H_

#include "stdarg.h"
#include "Type.h"
#include "ipl_utility.h"
#include "rsc_obj.h"
#define RSC_SURPPORT_NUM RSC_SURRPORT_ID  // SPI only surpport sie1~sie4(reference by spi proposal)
typedef void (*RSC_TSK_FP)(void);
typedef void (*FPRSC_Process)(IPL_PROC_ID Id, UINT32 Value);
typedef void (*RSC_End_TSK_FP)(IPL_PROC_ID Id);

typedef struct {
	RSC_TSK_FP fpEvent;
	UINT32     EventID;
} RSC_EVENT;

#define FLGPTN_RSC_IDLE      FLGPTN_BIT(0)
#define RSC_BASE             FLGPTN_BIT(0)
#define RSC_OP1              FLGPTN_BIT(1)
#define RSC_OP2              FLGPTN_BIT(2)
#define RSC_OP3              FLGPTN_BIT(3)
#define RSC_PROC_1           FLGPTN_BIT(4)
#define RSC_PROC_2           FLGPTN_BIT(5)
#define RSC_PROC_3           FLGPTN_BIT(6)
#define RSC_PROC_4           FLGPTN_BIT(7)
#define RSC_END_1            FLGPTN_BIT(8)
#define RSC_END_2            FLGPTN_BIT(9)
#define RSC_END_3            FLGPTN_BIT(10)
#define RSC_END_4            FLGPTN_BIT(11)
#define RSC_END_DONE_1       FLGPTN_BIT(12)
#define RSC_END_DONE_2       FLGPTN_BIT(13)
#define RSC_END_DONE_3       FLGPTN_BIT(14)
#define RSC_END_DONE_4       FLGPTN_BIT(15)

#define RSC_OP_MASK (RSC_OP1|RSC_OP2|RSC_OP3|RSC_PROC_1|RSC_PROC_2|RSC_PROC_3|RSC_PROC_4|RSC_END_1|RSC_END_2|RSC_END_3|RSC_END_4)
#define RSC_OP_MAX  15


#define FLGPTN_RSC_E_IDLE    FLGPTN_BIT(0)
#define RSC_E_BASE           FLGPTN_BIT(0)
#define RSC_E_START1         FLGPTN_BIT(1)
#define RSC_E_START2         FLGPTN_BIT(2)
#define RSC_E_START3         FLGPTN_BIT(3)
#define RSC_E_START4         FLGPTN_BIT(4)
#define RSC_E_DONE1          FLGPTN_BIT(5)
#define RSC_E_DONE2          FLGPTN_BIT(6)
#define RSC_E_DONE3          FLGPTN_BIT(7)
#define RSC_E_DONE4          FLGPTN_BIT(8)

#define RSC_OP_END_MASK (RSC_E_START1|RSC_E_START2|RSC_E_START3|RSC_E_START4)
#define RSC_OP_END_MAX  8

extern RSC_TSK_FP *RSC_GetCmdEventTbl(void);
extern void RSC_SetCmdEventTbl(IPL_PROC_ID Id, RSC_EVENT *pFpTbl, UINT32 TblLen, RSC_End_TSK_FP EndSet);
extern void RSC_SetCmdEvent2(FPRSC_Process fp);
extern ER RSC_SetCmd(UINT32 Cmd);
extern ER RSC_E_SetCmd(UINT32 Cmd);
extern ER RSC_TrigEvent(IPL_PROC_ID Id, UINT32 Cmd);
extern BOOL RSC_IsIdle(void);


/**
    install RSC control id
*/
extern void RSC_InstallID(void) _SECTION(".kercfg_text");

/////////////////////////////////////////////////
extern void RSC_Tsk(void);
extern void RSC_E_Tsk(void);
extern void RSC_OpenTsk(void);
extern void RSC_CloseTsk(void);
extern BOOL RSC_TskOpened(void);
extern void RSC_InstallCmd(void);
extern BOOL RSC_GetFlagStatus(void);

#endif
