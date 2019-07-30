#include "kernel.h"
#include <string.h>
#include <stdio.h>
#include "SysKer.h"
#include "SysCfg.h"
#include "SxCmd.h"
#include "SysCommon.h"
#include "AppCommon.h"
#include "UserCmd.h"
#include "UserCmdInt.h"
#include "UserCmdID.h"
#include "FileSysTsk.h"
#include "FileDB.h"
#include "WavStudioTsk.h"
#include "dma.h"
#include "UIFlow.h"
#if defined(_UI_STYLE_SPORTCAM_)
#include "BurnInWnd.h"
#include "AudioTestWnd.h"
#endif
//#include "WavTestWnd.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          USERCMD
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MAX_CMD_SCRIPT_FILE_LEN     128
#define SCRIPT_FILE_LOOP_RESET_IDX  0xFFFFFFFF
#define SCRIPT_FILE_LOOP_INIT_IDX   0xFFFFFFFE
#define SCRIPT_FILE_MAX_LOOP_DEPTH  4

static BOOL g_bUserCmdTskOpened = FALSE;
static UINT32 g_uiUserCmdId = USERCMD_MAX;

USERCMD_ITEM g_UserCmdItem[] = {
#if defined(_UI_STYLE_SPORTCAM_)
	{USERCMD_BURNIN,        UserCmd_BurnIn},
	{USERCMD_RUNSCRIPT,     UserCmd_RunScript},
	{USERCMD_AUDIOTEST,     UserCmd_AudioTest},
	//{USERCMD_WAVTEST,       UserCmd_WavTest},
	{USERCMD_PCC,           UserCmd_PCC},
	//{USERCMD_MAX,           NULL},
#endif
};
#if defined(_UI_STYLE_SPORTCAM_)
//-------------------------------------------------------------------------------------------------
void UserCmd_BurnIn(void)
{
#if 0
	UINT32 i;

	Delay_DelayMs(3000);
	for (i = 0; i < 1000; i++) {
		DBG_DUMP("^C Burn in loop: %d\r\n", i + 1);
		Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
		Delay_DelayMs(120 * 1000); // record 2 min
		Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
		Delay_DelayMs(3000);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
		Delay_DelayMs(3000);
		Ux_PostEvent(NVTEVT_KEY_SELECT, 1, NVTEVT_KEY_PRESS);
		Delay_DelayMs(60 * 1000); // play 1 min
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
		Delay_DelayMs(3000);
	}
#else
	Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
	Ux_OpenWindow((VControl *)(&BurnInWndCtrl), 0);
#endif
}

//-------------------------------------------------------------------------------------------------
void UserCmd_RunScript(void)
{
	FST_FILE pFile;
	UINT32 lSize, uiReadSize, uiFileIdx = 0, uiCmdIdxStart = 0, uiCmdIdxEnd = 0, uiLoopIdxStart[SCRIPT_FILE_MAX_LOOP_DEPTH] = {SCRIPT_FILE_LOOP_RESET_IDX};
	char *buffer = NULL;
	char szCmdBuf[MAX_CMD_SCRIPT_FILE_LEN] = {0}, *pChar;
	size_t result;
	BOOL bInsideLoop = 0;
	UINT32 uiLoopCount[SCRIPT_FILE_MAX_LOOP_DEPTH] = {0}, uiLoopDepth = 0;

	pFile = FileSys_OpenFile(USERCMD_RUNSCRIPT_FILE, FST_OPEN_READ);
	if (pFile == NULL) {
		DBG_ERR("File error\r\n");
		return;
	}

	// obtain file size:
	// Seek the end of file to get the file size (position)
	FileSys_SeekFile(pFile, 0, FST_SEEK_END);
	uiReadSize = lSize = FileSys_TellFile(pFile);
	FileSys_SeekFile(pFile, 0, FST_SEEK_SET); // set to file start

	if (lSize) {
		// allocate memory to contain the whole file (use RAM disk buffer temporarily)
		if (lSize < POOL_SIZE_STORAGE_RAMDISK) {
			buffer = (char *)OS_GetMempoolAddr(POOL_ID_STORAGE_RAMDISK);
		} else {
			DBG_ERR("Memory not enough!\r\n");
		}

		if (buffer == NULL) {
			DBG_ERR("Memory error\r\n");
		}

		// copy the file into the buffer:
		result = FileSys_ReadFile(pFile, (UINT8 *)buffer, &uiReadSize, 0, 0);
		if (result != FST_STA_OK) {
			DBG_ERR("Reading error\r\n");
		}

		/* the whole file is now loaded in the memory buffer. */
	}

	FileSys_CloseFile(pFile);

	if (lSize == 0 || !buffer || (uiReadSize != lSize)) {
		DBG_ERR("FileSize = 0 - Program End\r\n");
	} else {
		while (uiFileIdx < lSize) {
			// Find out the head of command.
			while (uiFileIdx < lSize) {
				if (buffer[uiFileIdx] != '\r' && buffer[uiFileIdx] != '\n'  && buffer[uiFileIdx] != '\t' && \
					buffer[uiFileIdx] != ' ') {
					uiCmdIdxStart =  uiFileIdx;

					uiFileIdx++;
					break;
				} else {
					uiFileIdx++;
				}
			}

			if (uiFileIdx >= lSize) {
				DBG_DUMP("^C FileIdx > Size - Program End\r\n");
				break;
			}

			// Find out the tail of command.
			while (uiFileIdx < lSize) {
				if (buffer[uiFileIdx] == '\r' || buffer[uiFileIdx] == '\n') {
					uiCmdIdxEnd = uiFileIdx - 1;
					break;
				} else if (uiFileIdx == lSize - 1) { // End of file
					uiCmdIdxEnd = uiFileIdx;
					break;
				} else {
					uiFileIdx++;
				}
			}

			// Extract command for the file buffer.
			if (uiCmdIdxEnd > uiCmdIdxStart) {
				memset(szCmdBuf, 0, sizeof(szCmdBuf));
				if (buffer[uiCmdIdxStart] != ';') {
					//#NT#2016/10/06#Brain Yen -begin
					//#NT# Coverity CID 67993, 68893
					if ((uiCmdIdxEnd >= uiCmdIdxStart) && ((uiCmdIdxEnd - uiCmdIdxStart + 1) <= MAX_CMD_SCRIPT_FILE_LEN)) {
						memcpy(szCmdBuf, &buffer[uiCmdIdxStart], uiCmdIdxEnd - uiCmdIdxStart + 1);
					} else {
						DBG_ERR("Command lenth exceed the max length - %d\r\n", MAX_CMD_SCRIPT_FILE_LEN);
					}
					//#NT#2016/10/06#Brain Yen -end
				}

				// Check if loop start or end.
				pChar = strstr(szCmdBuf, "loop begin");
				if (pChar) {
					if (uiLoopDepth > SCRIPT_FILE_MAX_LOOP_DEPTH) {
						DBG_ERR("Illegal loop cmd!\r\n");
					} else {
						bInsideLoop = 1;

						// Keep track of the lst line in the loop.
						if (uiLoopDepth && uiLoopIdxStart[uiLoopDepth - 1] == SCRIPT_FILE_LOOP_INIT_IDX) {
							uiLoopIdxStart[uiLoopDepth - 1] = uiCmdIdxStart;
						}

						// Keep track of the loop counter for each level of loop.
						if (++uiLoopDepth <= SCRIPT_FILE_MAX_LOOP_DEPTH) {
							sscanf(&pChar[11], "%d", &uiLoopCount[uiLoopDepth - 1]);
							uiLoopIdxStart[uiLoopDepth - 1] = SCRIPT_FILE_LOOP_INIT_IDX;
						}
					}
				} else if (strstr(szCmdBuf, "loop end")) {
					//#NT#2016/10/06#Brain Yen -begin
					//#NT#Coverity CID 67993, 67994, 67995
					if (uiLoopDepth >= 1) {
						if (uiLoopCount[uiLoopDepth - 1]) {
							if (--uiLoopCount[uiLoopDepth - 1]) {
								uiFileIdx = uiLoopIdxStart[uiLoopDepth - 1];
							} else {
								bInsideLoop = 0;
								uiLoopIdxStart[uiLoopDepth - 1] = SCRIPT_FILE_LOOP_RESET_IDX;
								uiLoopDepth--;
							}
						} else {
							bInsideLoop = 0;
							uiLoopIdxStart[uiLoopDepth - 1] = SCRIPT_FILE_LOOP_RESET_IDX;
							uiLoopDepth--;
						}
					}
					//#NT#2016/10/06#Brain Yen -end
				} else if (szCmdBuf[0]) { // Ignore comments.
					// Keep tract of the lst line in the loop.
					if (bInsideLoop == 1 && uiLoopIdxStart[uiLoopDepth - 1] == SCRIPT_FILE_LOOP_INIT_IDX) {
						uiLoopIdxStart[uiLoopDepth - 1] = uiCmdIdxStart;
					}

					// Parse and run this command indeed!
					DBG_DUMP("^C Cmd:%s, lend:%d\r\n", szCmdBuf, strlen(szCmdBuf));
					SxCmd_DoCommand(szCmdBuf);
				}

				//szCmdBuf[strlen(szCmdBuf)] = '\r';
				uiCmdIdxStart = uiCmdIdxEnd;
			}
		}
	}

	DBG_DUMP("^C Finished!\r\n");
}

//-------------------------------------------------------------------------------------------------
void UserCmd_AudioTest(void)
{
	Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
	Ux_OpenWindow((VControl *)(&AudioTestWndCtrl), 0);
}

//-------------------------------------------------------------------------------------------------
void UserCmd_PCC(void)
{
	Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);

	{
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_USBPCC;
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
}
#endif
//-------------------------------------------------------------------------------------------------
UINT32 UserCmd_RunCmd(UINT32 cmd)
{
	g_uiUserCmdId = cmd;
	UserCmd_Open();
	set_flg(FLG_ID_USERCMD, FLGUSERCMD_RUNCMD);
	return NVTRET_OK;
}

ER UserCmd_Open(void)
{
	if (g_bUserCmdTskOpened == TRUE) {
		return E_SYS;
	}
	clr_flg(FLG_ID_USERCMD, 0xFFFFFFFF);
	g_bUserCmdTskOpened = TRUE;

	sta_tsk(USERCMD_TSK_ID, 0);

	return E_OK;
}

ER UserCmd_Close(void)
{
	if (g_bUserCmdTskOpened == FALSE) {
		return E_SYS;
	}
	g_bUserCmdTskOpened = FALSE;

	ter_tsk(USERCMD_TSK_ID);

	return E_OK;
}

void UserCmdTsk(void)
{
	UINT32  i, uiCmdMax;
	FLGPTN  FlgPtn;

	kent_tsk();

	uiCmdMax = sizeof(g_UserCmdItem) / sizeof(USERCMD_ITEM);
//#NT#2016/10/06#Brain Yen -begin
//#NT# Coverity CID 67992
	//while(1)
	while (USERCMD_TSK_ID)
//#NT#2016/10/06#Brain Yen -end
	{
		set_flg(FLG_ID_USERCMD, FLGUSERCMD_IDLE);
		wai_flg(&FlgPtn, FLG_ID_USERCMD, FLGUSERCMD_RUNCMD, TWF_ORW | TWF_CLR);
		clr_flg(FLG_ID_USERCMD, FLGUSERCMD_IDLE);

		if (FlgPtn & FLGUSERCMD_RUNCMD) {
			for (i = 0; i < uiCmdMax; i++) {
				if (g_UserCmdItem[i].Id == g_uiUserCmdId) {
					break;
				}
			}

			if (i < uiCmdMax) {
				if (g_UserCmdItem[i].Function) {
					g_UserCmdItem[i].Function();
				}
			}
		}
	}
}

#if 0
BOOL Cal_CheckRunScriptFile(void)
{
	BOOL ret = FALSE;
	FST_FILE filehdl = NULL;

	filehdl = FileSys_OpenFile(CAL_RUNSCRIPT_FILE, FST_OPEN_READ);
	if (filehdl != NULL) {
		FileSys_CloseFile(filehdl);
		ret = TRUE;
	}
	return ret;
}
#endif
