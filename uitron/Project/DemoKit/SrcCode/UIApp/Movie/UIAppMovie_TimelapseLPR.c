////////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "UIAppMovie.h"
#include "UIInfo.h"
#include "UIFlowMovieFuncs.h"
#include "time.h"
#include "rtc.h"
#include "HwPower.h"
////////////////////////////////////////////////////////////////////////////////

//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MOVIE0_FOLDER       "Movie"
#define MOVIE1_FOLDER       "Movie"
#define TIMELAPSE0_FOLDER   "Timelapse"         // Max 15 characters
#define TIMELAPSE1_FOLDER   "Timelapse"         // Max 15 characters
// Notice: If TmpFile name changed, the filename in UIAppMovie_CB.c should also be changed too.
#define TIMELAPSE0_RECFILE  "Timelapse0_Recording"      // Need to append .MP4 or .TS depends on recording file type
#define TIMELAPSE1_RECFILE  "Timelapse1_Recording"      // Need to append .MP4 or .TS depends on recording file type
#define TIMELAPSE0_TMPFILE  "Timelapse0_Tmp"            // Need to append .MP4 or .TS depends on recording file type
#define TIMELAPSE1_TMPFILE  "Timelapse1_Tmp"            // Need to append .MP4 or .TS depends on recording file type
#define FILENAME_LEN        80
#define STSS_GAP            15          // GAP to insert STSS sample
#define PRE_BOOT_TIME       4
#define PRE_PWRDOWN_TIME    2

#define MP4_TIME_BASE       0x784CE00   // MP4 time base is from 1904/1/1 but rtc time base is from 1900/1/1. The difference is 86400 x 365 x 4
#define YEAR1970BASE        0x83AA7E80  // The seconds from 1900/1/1 0:0:0 to 1970/1/1 0:0:0 due to new LibC time base is 1970/1/1 0:0:0

typedef struct {
	UINT32 Loc;
	UINT32 Size;
} AtomInfo;

typedef struct {
	UINT32      FileSize;
	AtomInfo    mdat;
	AtomInfo    moov;
	AtomInfo    trak;
	AtomInfo    mdia;
	AtomInfo    minf;
	AtomInfo    stbl;
	AtomInfo    stss;
	AtomInfo    LastFrame;
	UINT32      FrameCnt;
	UINT32      KeyFrameCnt;
	UINT32      LastKeyFrame;
	UINT32      TimeScale;
	UINT32      Duration;
	UINT32      CreationTime;
	UINT32      ModificationTime;
} MP4Info;

typedef enum {
	TSPKT_PAT,      ///<
	TSPKT_PMT,      ///<
	TSPKT_PCR,      ///<
	TSPKT_VID,      ///<
} TSPKT;

static ER _MovieTLLPR_FilePostProcess(void);
static ER _MovieTLLPR_CloseFile(FST_FILE *MovFile1, FST_FILE *MovFile2);
static ER _MovieTLLPR_ConvertTime2DateTime(UINT32 Time, struct tm *DateTime);
static ER _MovieTLLPR_DumpFileInfo(MP4Info *FileInfo);
static ER _MovieTLLPR_ParseFileInfo(FST_FILE *MovFile, MP4Info *FileInfo);
static ER _MovieTLLPR_AppendMdat(FST_FILE *MovFile, MP4Info *FileInfo, MP4Info *NewFileInfo, UINT32 Addr, UINT32 Size, UINT32 MTime);
static ER _MovieTLLPR_AppendMoov(FST_FILE *MovFile, MP4Info *FileInfo, MP4Info *NewFileInfo, UINT32 MoovBuf, UINT32 Size);
static ER _MovieTLLPR_CheckRecordingFile(char *Filename, UINT32 Mode, struct tm *CTime);
static ER _MovieTLLPR_CheckBothRecordingFile(char *Filename0, char *Filename1);
static ER _MovieTLLPR_RenameFile(void);
static ER _MovieTLLPR_OneShotCapture(void);
static ER _MovieTLLPR_SetNextHWRT(UINT32 Sec);
static ER _MovieTLLPR_StartHWRT(void);
static ER _MovieTLLPR_StopHWRT(void);
static ER _MovieTLLPR_WaitTriggerTime(void);
static UINT32 _MovieTLLPR_GetTimeInSec(UINT32 TimeID);
static UINT32 _MovieTLLPR_CheckCLibVersion(void);
// Follows functions are for TS format
static UINT32 _MovieTLLPR_CheckTSFileSize(FST_FILE *MovFile);
static UINT32 _MovieTLLPR_GetTSPktCnt(FST_FILE *MovFile, TSPKT Type);
static UINT32 _MovieTLLPR_UpdateTSPktCnt(TSPKT Type, UINT8 *TSPktBuf);
static UINT32 _MovieTLLPR_WriteTSPkt(FST_FILE *MovFile, TSPKT Type);
static UINT32 _MovieTLLPR_CopyVideo(FST_FILE *DestFile, FST_FILE *SrcFile);

static UINT32 TimelapseTime[MOVIE_TIMELAPSEREC_ID_MAX] = {
	0,      // MOVIE_TIMELAPSEREC_OFF,
	1,      // MOVIE_TIMELAPSEREC_1SEC,
	5,      // MOVIE_TIMELAPSEREC_5SEC,
	10,     // MOVIE_TIMELAPSEREC_10SEC,
	30,     // MOVIE_TIMELAPSEREC_30SEC,
	60,     // MOVIE_TIMELAPSEREC_1MIN,
	300,    // MOVIE_TIMELAPSEREC_5MIN,
	600,    // MOVIE_TIMELAPSEREC_10MIN,
	1800,   // MOVIE_TIMELAPSEREC_30MIN,
	3600,   // MOVIE_TIMELAPSEREC_1HOUR,
	7200,   // MOVIE_TIMELAPSEREC_2HOUR,
	10800,  // MOVIE_TIMELAPSEREC_3HOUR,
	86400,  // MOVIE_TIMELAPSEREC_1DAY,
};

static UINT32 gMutexKey = 0;
static UINT32 gTLLPRStatus = 0;
static UINT32 TmpMdatAddr = 0;
static UINT32 TmpMdatSize = 0;
static UINT32 UseNewLibC = 0;
static UINT32 gLPRFileType = MEDIA_FILEFORMAT_MP4;
// Follow variables are for TS
static UINT8 *g_pPATBuf = NULL, *g_pPMTBuf = NULL, *g_pPCRBuf = NULL, *g_pVideoBuf = NULL;
static UINT8 g_CntPAT = 0, g_CntPMT = 0, g_CntPCR = 0, g_CntVideo = 0;
static UINT64 g_PCR = 0UL;

// For OSD display test
struct tm gStartTime = {0};
char gStrStartTitle[15] = "Start Time:";
char gStrStartDateTime[30] = {0};
struct tm gNextAlarmTime = {0};
char gStrNextTitle[15] = "Next Time:";
char gStrNextDateTime[30] = {0};

void MovieTLLPR_SetMem(UINT32 uiAddr, UINT32 uiSize)
{
	TmpMdatAddr = uiAddr;
	TmpMdatSize = uiSize;
}

UINT32 MovieTLLPR_Process(INT32 CmdFrom)
{
	ER ret = E_SYS;
	UINT32 Flow = TL_FLOW_MOVIE;
	UINT32 State = TL_STATE_BUSY;
	UINT32 TimelapseMode = 0;
	char MovieFolderName[2][16] = {0};
	NMC_FOLDER_INFO DestFolder[2] = {0};
	char RecFileName[2][FILENAME_LEN] = {0};
	UINT32 HWRTStatus = TL_HWRT_BOOT_NORMAL;
	UINT32 CurrTime = 0;
	UINT32 AlarmTime = 0;

	// Step: Check LibC version
	// Notice:
	//    The old LibC (1.00.006 or below) use 1900/1/1 0:0:0 as timebase
	//    However the starnd CLib use 1970/1/1 0:0:0 as standard timebase
	//    From 1.00.007 LibC, the timebase is changed.
	//    In order to backward compatible, so we should check the LibC version.
	UseNewLibC = _MovieTLLPR_CheckCLibVersion();

	// Step: Check current file type (this is a temporary method. Should use regular one when project layer done.)
	extern BOOL g_bTsFileFormat;
	gLPRFileType = g_bTsFileFormat ? MEDIA_FILEFORMAT_TS : MEDIA_FILEFORMAT_MP4;

	// Step: Set flow flag
	TimelapseMode = UI_GetData(FL_MOVIE_TIMELAPSE_REC);
	if (TimelapseMode == MOVIE_TIMELAPSEREC_OFF) {
		Flow = TL_FLOW_MOVIE;
	} else if (TimelapseMode < TL_LPR_TIME_MIN_PERIOD) {
		Flow = TL_FLOW_ORG;
	} else {
		Flow = TL_FLOW_LPR;
	}

	// Step: If re-entry, return
	if (gMutexKey) {
		return ((Flow | TL_STATE_BUSY));
	}

	gMutexKey = 1;

	DestFolder[0].pathid = 0;
	DestFolder[0].pMovie = MovieFolderName[0];
	DestFolder[1].pathid = 1;
	DestFolder[1].pMovie = MovieFolderName[1];

	if (TimelapseMode == MOVIE_TIMELAPSEREC_OFF) {
		strncpy(MovieFolderName[0], MOVIE0_FOLDER, 15);
		NH_Custom_SetFolderPath(&(DestFolder[0]));
		strncpy(MovieFolderName[1], MOVIE1_FOLDER, 15);
		NH_Custom_SetFolderPath(&(DestFolder[1]));
		ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_AUTO);
		State = TL_STATE_DONTCARE;
	} else if (TimelapseMode < TL_LPR_TIME_MIN_PERIOD) {
		strncpy(MovieFolderName[0], TIMELAPSE0_FOLDER, 15);
		NH_Custom_SetFolderPath(&(DestFolder[0]));
		strncpy(MovieFolderName[1], TIMELAPSE1_FOLDER, 15);
		NH_Custom_SetFolderPath(&(DestFolder[1]));
		ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_AUTO);
		State = TL_STATE_DONTCARE;
	} else {
		// Todo: Need to add system check here. Ex. card full, no card, .....
		//
		//
		strncpy(MovieFolderName[0], TIMELAPSE0_FOLDER, 15);
		NH_Custom_SetFolderPath(&(DestFolder[0]));
		strncpy(MovieFolderName[1], TIMELAPSE1_FOLDER, 15);
		NH_Custom_SetFolderPath(&(DestFolder[1]));
		ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
		snprintf(RecFileName[0], FILENAME_LEN, "A:\\%s\\%s\\%s", FILEDB_CARDV_ROOT, TIMELAPSE0_FOLDER, TIMELAPSE0_RECFILE);
		snprintf(RecFileName[1], FILENAME_LEN, "A:\\%s\\%s\\%s", FILEDB_CARDV_ROOT, TIMELAPSE1_FOLDER, TIMELAPSE1_RECFILE);
		strncat(RecFileName[0], (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);
		strncat(RecFileName[1], (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);

		if (CmdFrom == TIMELAPSE_FROM_UI) {
			// If not find recording file, means start timelapse record. Record the 1st file. Start HWRT.
			// else means stop timelapse rec and just rename the file to normal name. Stop HWRT.
			ret = _MovieTLLPR_CheckBothRecordingFile(RecFileName[0], RecFileName[1]);
			if (ret == E_SYS) {
				DBG_IND("[TLLPR] Start capture by REC key\r\n");
				_MovieTLLPR_SetNextHWRT(TimelapseTime[TimelapseMode]);
				_MovieTLLPR_OneShotCapture();
				State = TL_STATE_RECORD;
			} else {
				DBG_IND("[TLLPR] Stop capture by REC key\r\n");
				_MovieTLLPR_RenameFile();
				_MovieTLLPR_StopHWRT();
				State = TL_STATE_STOP;
			}
		} else if (CmdFrom == TIMELAPSE_FROM_TIMER) {
			HWRTStatus = MovieTLLPR_CheckHWRTStatus();
			CurrTime = _MovieTLLPR_GetTimeInSec(TIME_ID_CURRENT);
			AlarmTime = _MovieTLLPR_GetTimeInSec(TIME_ID_HWRT);

			if (HWRTStatus == TL_HWRT_BOOT_NORMAL) {
				ret = _MovieTLLPR_CheckBothRecordingFile(RecFileName[0], RecFileName[1]);
				if (ret == E_SYS) {
					// Not start record, do nothing.
					State = TL_STATE_STOP;
				} else {
					if (AlarmTime > CurrTime) {
						if ((AlarmTime - CurrTime) <= TimelapseTime[TimelapseMode]) {
							// Restart HWRT
							DBG_IND("[TLLPR] Restart HWRT since reasonable alarm time\r\n");
							_MovieTLLPR_StartHWRT();
							State = TL_STATE_RECORD;
						} else {
							// Unreasonable alarm time, terminate this recording.
							DBG_IND("[TLLPR] Stop capture since unreasonable alarm time case 1\r\n");
							_MovieTLLPR_RenameFile();
							_MovieTLLPR_StopHWRT();
							State = TL_STATE_STOP;
						}
					} else {
						if ((CurrTime - AlarmTime) < 10) {
							// Do record
							DBG_IND("[TLLPR] Alarm missed case 1, capture immediately\r\n");
							_MovieTLLPR_SetNextHWRT(TimelapseTime[TimelapseMode]);
							_MovieTLLPR_OneShotCapture();
							State = TL_STATE_RECORD;
						} else {
							// If Alarm time expired more than 10 second, terminate this recording.
							DBG_IND("[TLLPR] Stop capture since Alarm time expired (case 1)\r\n");
							_MovieTLLPR_RenameFile();
							_MovieTLLPR_StopHWRT();
							State = TL_STATE_STOP;
						}
					}
				}
			} else if (HWRTStatus == TL_HWRT_BOOT_ALARM) {
				DBG_IND("[TLLPR] HWRTStatus = TL_HWRT_BOOT_ALARM\r\n");
				/// Manual power hit alarm?
			} else if (HWRTStatus == TL_HWRT_WORKING) {
				ret = _MovieTLLPR_CheckBothRecordingFile(RecFileName[0], RecFileName[1]);
				if (ret == E_SYS) {
					// No file, stop HWRT
					DBG_IND("[TLLPR] Stop capture (invalid state since no file)\r\n");
					_MovieTLLPR_StopHWRT();
					State = TL_STATE_STOP;
				} else {
					if (AlarmTime < CurrTime) {
						if ((CurrTime - AlarmTime) < 10) {
							// Do record
							DBG_IND("[TLLPR] Alarm missed case 2, capture immediately\r\n");
							_MovieTLLPR_SetNextHWRT(TimelapseTime[TimelapseMode]);
							_MovieTLLPR_OneShotCapture();
							State = TL_STATE_RECORD;
						} else {
							// If Alarm time expired more than 10 second, terminate this recording.
							DBG_IND("[TLLPR] Stop capture since Alarm time expired (case 2)\r\n");
							_MovieTLLPR_RenameFile();
							_MovieTLLPR_StopHWRT();
							State = TL_STATE_STOP;
						}
					} else {
						if (AlarmTime - CurrTime > TimelapseTime[TimelapseMode]) {
							// Unreasonable alarm time, terminate this recording.
							DBG_IND("[TLLPR] Stop capture since unreasonable alarm time case 2\r\n");
							_MovieTLLPR_RenameFile();
							_MovieTLLPR_StopHWRT();
							State = TL_STATE_STOP;
						} else {
							State = TL_STATE_RECORD;

							// For OSD display test
							if (strlen(gStrStartDateTime) == 0) {
								_MovieTLLPR_CheckRecordingFile(RecFileName[0], 1, &gStartTime);
								snprintf(gStrStartDateTime, 30, "%04u/%02u/%02u  %02u:%02u:%02u", gStartTime.tm_year, gStartTime.tm_mon, gStartTime.tm_mday, gStartTime.tm_hour, gStartTime.tm_min, gStartTime.tm_sec);

								//gNextAlarmTime = HwClock_GetTime(TIME_ID_HWRT);
								AlarmTime = _MovieTLLPR_GetTimeInSec(TIME_ID_HWRT);
								//struct tm TimeTmp = {0};
								AlarmTime += PRE_BOOT_TIME;
								//gNextAlarmTime = TimeUtil_ValueAddDiff(gNextAlarmTime, TimeTmp);
								_MovieTLLPR_ConvertTime2DateTime((AlarmTime - MP4_TIME_BASE), &gNextAlarmTime);
								snprintf(gStrNextDateTime, 30, "%04u/%02u/%02u  %02u:%02u:%02u", gNextAlarmTime.tm_year, gNextAlarmTime.tm_mon, gNextAlarmTime.tm_mday, gNextAlarmTime.tm_hour, gNextAlarmTime.tm_min, gNextAlarmTime.tm_sec);
							}
						}
					}
				}
			} else if (HWRTStatus == TL_HWRT_TIMEUP) {
				if (CurrTime >= AlarmTime + PRE_BOOT_TIME) {
					DBG_IND("[TLLPR] Capture by HWRT timeup\r\n");
					_MovieTLLPR_SetNextHWRT(TimelapseTime[TimelapseMode]);
					_MovieTLLPR_OneShotCapture();
				}
				State = TL_STATE_RECORD;
			}
		} else if (CmdFrom == TIMELAPSE_FROM_HWRT) {
			CurrTime = _MovieTLLPR_GetTimeInSec(TIME_ID_CURRENT);
			AlarmTime = _MovieTLLPR_GetTimeInSec(TIME_ID_HWRT);

			if ((CurrTime > AlarmTime) && (CurrTime - AlarmTime) > 10) {
				// If Alarm time expired more than 10 second, terminate this recording.
				// Possible case: Remove SD card during LPR recording, when reboot system manually, the state will be TIMELAPSE_FROM_HWRT
				DBG_IND("[TLLPR] Stop capture since Alarm time expired (case 3)\r\n");
				_MovieTLLPR_RenameFile();
				_MovieTLLPR_StopHWRT();
				State = TL_STATE_STOP;
				Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
			} else {
				DBG_IND("[TLLPR] Capture by HWRT reboot\r\n");
				_MovieTLLPR_WaitTriggerTime();
				_MovieTLLPR_SetNextHWRT(TimelapseTime[TimelapseMode]);
				_MovieTLLPR_OneShotCapture();
				State = TL_STATE_RECORD;
				Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
			}
		} else if (CmdFrom == TIMELAPSE_FROM_PWRKEY) {
			HWRTStatus = MovieTLLPR_CheckHWRTStatus();

			if (HWRTStatus == TL_HWRT_BOOT_NORMAL) {
				State = TL_STATE_DONTCARE;
			} else {
				CurrTime = _MovieTLLPR_GetTimeInSec(TIME_ID_CURRENT);
				AlarmTime = _MovieTLLPR_GetTimeInSec(TIME_ID_HWRT);

				if ((AlarmTime > CurrTime) && (AlarmTime - CurrTime) > PRE_PWRDOWN_TIME) {
					// Powerdown directly
					State = TL_STATE_RECORD;
				} else {
					// Show mesaage, wait to capture next one and then powerdown
					DBG_IND("[TLLPR] Capture by powerdown delay\r\n");
					Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 1, FLOWWRNMSG_ISSUE_PLEASE_WAIT);
					Ux_Redraw();
					_MovieTLLPR_WaitTriggerTime();
					_MovieTLLPR_SetNextHWRT(TimelapseTime[TimelapseMode]);
					_MovieTLLPR_OneShotCapture();
					State = TL_STATE_RECORD;
				}
			}
		}
	}
	gTLLPRStatus = (Flow | State);


	if (Flow == TL_FLOW_LPR) {
		if (State == TL_STATE_RECORD) {
			gMovData.State = MOV_ST_REC;
		} else {
			gMovData.State = MOV_ST_VIEW;
		}
	}

	gMutexKey = 0;
	return ((Flow | State));
}

UINT32 MovieTLLPR_CheckHWRTStatus(void)
{
	UINT32 ret = TL_HWRT_BOOT_NORMAL;
	UINT32 HWRTEnabled = 0;
	UINT32 HWRTPowerSrc = 0;

	HWRTEnabled = HwPower_GetPowerKey(POWER_ID_HWRT);
	HWRTPowerSrc = HwPower_GetPowerOnState(POWER_STATE_SRC);

	if (HWRTEnabled == 0 && HWRTPowerSrc != RTC_PWRON_SRC_PWR_ALM) {
		ret = TL_HWRT_BOOT_NORMAL;
	} else if (HWRTEnabled == 0 && HWRTPowerSrc == RTC_PWRON_SRC_PWR_ALM) {
		ret = TL_HWRT_BOOT_ALARM;
	} else if (HWRTEnabled == 1 && HWRTPowerSrc != RTC_PWRON_SRC_PWR_ALM) {
		ret = TL_HWRT_WORKING;
	} else if (HWRTEnabled == 1 && HWRTPowerSrc == RTC_PWRON_SRC_PWR_ALM) {
		ret = TL_HWRT_TIMEUP;
	}
	return ret;
}

// For OSD display test
UINT32 MovieTLLPR_GetStatus(void)
{
	return gTLLPRStatus;
}

static ER _MovieTLLPR_FilePostProcess(void)
{
	UINT32 ChannelID = 0;
	ER ret1 = E_OK;
	ER ret2 = E_OK;
	FST_FILE TmpFile = NULL;
	FST_FILE RecFile = NULL;
	MP4Info TmpFileInfo = {0};
	MP4Info RecFileInfo = {0};
	MP4Info NewRecFileInfo = {0};
	UINT32 RecMoovAddr = 0;
	UINT32 ReadSize = 0;
	char RecFileName[FILENAME_LEN] = {0};
	char TmpFileName[FILENAME_LEN] = {0};
	// Following variables are for TS
	char FileNameBuf[FILENAME_LEN] = {0};
	UINT32 FileCTime[6] = {0}, FileMTime[6] = {0};

	for (ChannelID = 0; ChannelID < 2; ChannelID ++) {
		// Step: Check whether signal or dual channel
		if ((ChannelID == 1) && (UI_GetData(FL_MOVIE_SIZE) < MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30)) {    // Signal channel
			break;
		}

		// Step: Reset variables
		if (TmpMdatAddr == 0) {
			DBG_ERR("[TLLPR] Working buffer is not assigned!!!\r\n");
			return E_SYS;
		}

		if (gLPRFileType == MEDIA_FILEFORMAT_MP4) {
			memset(&TmpFileInfo, 0, sizeof(MP4Info));
			memset(&RecFileInfo, 0, sizeof(MP4Info));
			memset(&NewRecFileInfo, 0, sizeof(MP4Info));
		} else { // gLPRFileType == MEDIA_FILEFORMAT_TS
			g_pPATBuf = (UINT8 *)(TmpMdatAddr + 0x00BC);        // 188
			g_pPMTBuf = (UINT8 *)(TmpMdatAddr + 0x0178);        // 376
			g_pPCRBuf = (UINT8 *)(TmpMdatAddr + 0x0234);        // 564
			g_pVideoBuf = (UINT8 *)(TmpMdatAddr + 0x002F0);     // 752
		}

		// Step: Open tmp file
		snprintf(TmpFileName, FILENAME_LEN, "A:\\%s\\%s\\%s", FILEDB_CARDV_ROOT, ChannelID ? TIMELAPSE1_FOLDER : TIMELAPSE0_FOLDER, ChannelID ? TIMELAPSE1_TMPFILE : TIMELAPSE0_TMPFILE);
		strncat(TmpFileName, (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);

		if (System_GetState(SYS_STATE_CARD)  != CARD_INSERTED) {
			return E_SYS;
		}

		TmpFile = FileSys_OpenFile(TmpFileName, FST_OPEN_EXISTING | FST_OPEN_WRITE);

		if (TmpFile == NULL) {     // No new file find, just rename the temp file to new file.
			DBG_ERR("[TLLPR] No tmp file found!!!\r\n");
			return E_SYS;
		}

		// Step: Open recording file in read mode to check whether file existed
		snprintf(RecFileName, FILENAME_LEN, "A:\\%s\\%s\\%s", FILEDB_CARDV_ROOT, ChannelID ? TIMELAPSE1_FOLDER : TIMELAPSE0_FOLDER, ChannelID ? TIMELAPSE1_RECFILE : TIMELAPSE0_RECFILE);
		strncat(RecFileName, (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);
		RecFile = FileSys_OpenFile(RecFileName, FST_OPEN_EXISTING | FST_OPEN_READ);
		if (RecFile == NULL) {     // No new file find, just rename the temp file to new file.
			if (gLPRFileType == MEDIA_FILEFORMAT_MP4) {
				ret1 = _MovieTLLPR_ParseFileInfo(&TmpFile, &TmpFileInfo);               // In order to get CTime info
				_MovieTLLPR_CloseFile(&RecFile, &TmpFile);                              // Close file first and then check return code
				_MovieTLLPR_ConvertTime2DateTime(TmpFileInfo.CreationTime, &gStartTime);
			} else { // gLPRFileType == MEDIA_FILEFORMAT_TS)                             // Append next PMT/PCR.
				ret1 = _MovieTLLPR_GetTSPktCnt(&TmpFile, TSPKT_PMT);
				ret1 = _MovieTLLPR_GetTSPktCnt(&TmpFile, TSPKT_PCR);
				ret1 = _MovieTLLPR_UpdateTSPktCnt(TSPKT_PMT, g_pPMTBuf);
				ret1 = _MovieTLLPR_UpdateTSPktCnt(TSPKT_PCR, g_pPCRBuf);
				ret1 = _MovieTLLPR_WriteTSPkt(&TmpFile, TSPKT_PMT);
				ret1 = _MovieTLLPR_WriteTSPkt(&TmpFile, TSPKT_PCR);
				_MovieTLLPR_CloseFile(&RecFile, &TmpFile);                              // Close file first and then check return code
				FileSys_GetDateTime(TmpFileName, FileCTime, FileMTime);                 // Get file CTime
				gStartTime.tm_year = FileCTime[0];
				gStartTime.tm_mon = FileCTime[1];
				gStartTime.tm_mday = FileCTime[2];
				gStartTime.tm_hour = FileCTime[3];
				gStartTime.tm_min = FileCTime[4];
				gStartTime.tm_sec = FileCTime[5];
			}
			if (ret1 == E_SYS) {
				return E_SYS;
			}
			strncpy(FileNameBuf, ChannelID ? TIMELAPSE1_RECFILE : TIMELAPSE0_RECFILE, FILENAME_LEN);
			strncat(FileNameBuf, (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);
			FileSys_RenameFile(FileNameBuf, TmpFileName, FALSE);
			DBG_IND("[TLLPR] Rename tmp file to recording file!!!\r\n");

			// For OSD display test
			snprintf(gStrStartDateTime, 30, "%04u/%02u/%02u  %02u:%02u:%02u", gStartTime.tm_year, gStartTime.tm_mon, gStartTime.tm_mday, gStartTime.tm_hour, gStartTime.tm_min, gStartTime.tm_sec);
			continue;
		} else {
			_MovieTLLPR_CloseFile(&RecFile, 0);
		}

		// Step: Re-open recording file in write mode to merge file
		RecFile = FileSys_OpenFile(RecFileName, FST_OPEN_EXISTING | FST_OPEN_WRITE);

		if (gLPRFileType == MEDIA_FILEFORMAT_MP4) {
			// Step: Parse file info
			ret1 = _MovieTLLPR_ParseFileInfo(&TmpFile, &TmpFileInfo);
			ret2 = _MovieTLLPR_ParseFileInfo(&RecFile, &RecFileInfo);
			if (ret1 != E_OK || ret2 != E_OK) {
				_MovieTLLPR_CloseFile(&RecFile, &TmpFile);
				return E_SYS;
			}

			// Step: Backup data
			ReadSize = ALIGN_CEIL_32(TmpFileInfo.LastFrame.Size) + ALIGN_CEIL_32(RecFileInfo.moov.Size);
			if (ReadSize > TmpMdatSize) {
				_MovieTLLPR_CloseFile(&RecFile, &TmpFile);
				return E_SYS;
			}

			RecMoovAddr = TmpMdatAddr + ALIGN_CEIL_32(TmpFileInfo.LastFrame.Size);

			FileSys_SeekFile(TmpFile, TmpFileInfo.LastFrame.Loc, FST_SEEK_SET);
			ReadSize = TmpFileInfo.LastFrame.Size;
			ret1 = FileSys_ReadFile(TmpFile, (UINT8 *)TmpMdatAddr, &ReadSize, FALSE, NULL);

			FileSys_SeekFile(RecFile, RecFileInfo.moov.Loc, FST_SEEK_SET);
			ReadSize = RecFileInfo.moov.Size;
			ret1 = FileSys_ReadFile(RecFile, (UINT8 *)RecMoovAddr, &ReadSize, FALSE, NULL);

			// Step: Delete tmp file
			_MovieTLLPR_CloseFile(&TmpFile, NULL);
			FileSys_DeleteFile(TmpFileName);

			// Step: Append mdat & moov
			_MovieTLLPR_AppendMdat(&RecFile, &RecFileInfo, &NewRecFileInfo, TmpMdatAddr, TmpFileInfo.LastFrame.Size, TmpFileInfo.ModificationTime);
			_MovieTLLPR_AppendMoov(&RecFile, &RecFileInfo, &NewRecFileInfo, RecMoovAddr, RecFileInfo.moov.Size);

			// Step : for test only, dump file info
			DBG_DUMP("%s\r\n", RecFileName);
			_MovieTLLPR_ParseFileInfo(&RecFile, &RecFileInfo);
			_MovieTLLPR_DumpFileInfo(&RecFileInfo);
			_MovieTLLPR_ConvertTime2DateTime(RecFileInfo.CreationTime, &gStartTime);
			_MovieTLLPR_CloseFile(&RecFile, &TmpFile);
		} else { // gLPRFileType == MEDIA_FILEFORMAT_TS)
			// Step: Read PAT/PMT/PCR/VID data and counter
			_MovieTLLPR_GetTSPktCnt(&RecFile, TSPKT_PAT);
			_MovieTLLPR_GetTSPktCnt(&RecFile, TSPKT_PMT);
			_MovieTLLPR_GetTSPktCnt(&RecFile, TSPKT_PCR);
			_MovieTLLPR_GetTSPktCnt(&RecFile, TSPKT_VID);
			_MovieTLLPR_UpdateTSPktCnt(TSPKT_PAT, g_pPATBuf);
			_MovieTLLPR_UpdateTSPktCnt(TSPKT_PMT, g_pPMTBuf);
			_MovieTLLPR_UpdateTSPktCnt(TSPKT_PCR, g_pPCRBuf);
			_MovieTLLPR_CopyVideo(&RecFile, &TmpFile);

			// Step: Delete tmp file
			_MovieTLLPR_CloseFile(&TmpFile, NULL);
			FileSys_DeleteFile(TmpFileName);

			_MovieTLLPR_CloseFile(&RecFile, &TmpFile);
			FileSys_GetDateTime(RecFileName, FileCTime, FileMTime);                 // Get file CTime
			gStartTime.tm_year = FileCTime[0];
			gStartTime.tm_mon = FileCTime[1];
			gStartTime.tm_mday = FileCTime[2];
			gStartTime.tm_hour = FileCTime[3];
			gStartTime.tm_min = FileCTime[4];
			gStartTime.tm_sec = FileCTime[5];
		}

		// For OSD display test
		snprintf(gStrStartDateTime, 30, "%04u/%02u/%02u  %02u:%02u:%02u", gStartTime.tm_year, gStartTime.tm_mon, gStartTime.tm_mday, gStartTime.tm_hour, gStartTime.tm_min, gStartTime.tm_sec);
	}

	return E_OK;
}

static ER _MovieTLLPR_CloseFile(FST_FILE *MovFile1, FST_FILE *MovFile2)
{
	if (MovFile1 && *MovFile1) {
		FileSys_CloseFile(*MovFile1);
		*MovFile1 = NULL;
	}
	if (MovFile2 && *MovFile2) {
		FileSys_CloseFile(*MovFile2);
		*MovFile2 = NULL;
	}
	return E_OK;
}

static ER _MovieTLLPR_ConvertTime2DateTime(UINT32 Time, struct tm *DateTime)
{
	UINT32 uiDays = 0;
	UINT32 uiHour = 0;
	UINT32 uiMin = 0;
	UINT32 uiSec = 0;

	// MP4 time tick is from 1904/1/1
	Time += MP4_TIME_BASE;

	// Get date
	uiDays = Time / (24 * 60 * 60);
	memset(DateTime, 0, sizeof(struct tm));
	*DateTime = TimeUtil_Days2Value(uiDays);

	// Get time
	Time -= uiDays * (24 * 60 * 60);
	uiHour = Time / (60 * 60);
	Time -= uiHour * (60 * 60);
	uiMin = Time / 60;
	Time -= uiMin * 60;
	uiSec = Time;
	DateTime->tm_sec = uiSec;
	DateTime->tm_min = uiMin;
	DateTime->tm_hour = uiHour;

	return E_OK;
}

static ER _MovieTLLPR_DumpFileInfo(MP4Info *FileInfo)
{
	struct tm TimeInfo = {0};

	if (gLPRFileType != MEDIA_FILEFORMAT_MP4) {
		return E_SYS;
	}

	DBG_DUMP("FileInfo dump:\r\n");
	DBG_DUMP("FileSize : %d\r\n", FileInfo->FileSize);
	DBG_DUMP("mdat     : 0x%x (size = 0x%x)\r\n", FileInfo->mdat.Loc, FileInfo->mdat.Size);
	DBG_DUMP("moov     : 0x%x (size = 0x%x)\r\n", FileInfo->moov.Loc, FileInfo->moov.Size);
	DBG_DUMP("trak     : 0x%x (size = 0x%x)\r\n", FileInfo->trak.Loc, FileInfo->trak.Size);
	DBG_DUMP("mdia     : 0x%x (size = 0x%x)\r\n", FileInfo->mdia.Loc, FileInfo->mdia.Size);
	DBG_DUMP("minf     : 0x%x (size = 0x%x)\r\n", FileInfo->minf.Loc, FileInfo->minf.Size);
	DBG_DUMP("stbl     : 0x%x (size = 0x%x)\r\n", FileInfo->stbl.Loc, FileInfo->stbl.Size);
	DBG_DUMP("stss     : 0x%x (size = 0x%x)\r\n", FileInfo->stss.Loc, FileInfo->stss.Size);
	DBG_DUMP("LastFrm  : 0x%x (size = 0x%x)\r\n", FileInfo->LastFrame.Loc, FileInfo->LastFrame.Size);
	DBG_DUMP("FamCnt   : %d\r\n", FileInfo->FrameCnt);
	DBG_DUMP("LastKFrm : %d\r\n", FileInfo->LastKeyFrame);
	DBG_DUMP("KFrmCnt  : %d\r\n", FileInfo->KeyFrameCnt);
	DBG_DUMP("TScale   : 0x%x\r\n", FileInfo->TimeScale);
	DBG_DUMP("Duration : 0x%x\r\n", FileInfo->Duration);
	_MovieTLLPR_ConvertTime2DateTime(FileInfo->CreationTime, &TimeInfo);
	DBG_DUMP("CTime    : %04u/%02u/%02u %02u:%02u:%02u\r\n", TimeInfo.tm_year, TimeInfo.tm_mon, TimeInfo.tm_mday, TimeInfo.tm_hour, TimeInfo.tm_min, TimeInfo.tm_sec);
	_MovieTLLPR_ConvertTime2DateTime(FileInfo->ModificationTime, &TimeInfo);
	DBG_DUMP("MTime    : %04u/%02u/%02u %02u:%02u:%02u\r\n", TimeInfo.tm_year, TimeInfo.tm_mon, TimeInfo.tm_mday, TimeInfo.tm_hour, TimeInfo.tm_min, TimeInfo.tm_sec);

	return E_OK;
}

static ER _MovieTLLPR_ParseFileInfo(FST_FILE *MovFile, MP4Info *FileInfo)
{
	UINT32 atom[5] = {0};
	UINT32 ReadSize = 0;
	UINT32 ReadOffset = 0;
	INT32 ret = 0;
	UINT32 Cnt = 0;

	if (gLPRFileType != MEDIA_FILEFORMAT_MP4) {
		return E_SYS;
	}

	if (*MovFile == NULL) {
		return E_SYS;
	}

	// Step: Get file size info
	FileSys_SeekFile(*MovFile, 0, FST_SEEK_END);
	FileInfo->FileSize = FileSys_TellFile(*MovFile);
	FileSys_SeekFile(*MovFile, 0, FST_SEEK_SET);

	// Step: If file size < 8, return error
	if (FileInfo->FileSize < 8) {
		return E_SYS;
	}

	// Step: Check first atom, it must be "ftyp"
	ReadSize = sizeof(UINT32) * 2;
	FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
	if (atom[1] != 0x70797466) {                                                        // ftyp
		return E_NOSPT;
	}

	// Step: Start parsing atoms
	while (ReadOffset < FileInfo->FileSize) {
		// Set file position
		FileSys_SeekFile(*MovFile, ReadOffset, FST_SEEK_SET);
		ReadSize = sizeof(UINT32) * 2;
		FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);

		atom[0] = UINT32_SWAP(atom[0]);
		if (atom[0] == 0) {
			DBG_ERR("[TLLPR] Atom size = 0!!!\r\n");
			return E_SYS;
		}
		//DBG_IND("[TLLPR] Found atom %c%c%c%c (size = %x)!!!\r\n", (char)(atom[1]&0xff), (char)((atom[1]>>8)&0xff), (char)((atom[1]>>16)&0xff), (char)((atom[1]>>24)&0xff), atom[0]);
		if (atom[1] == 0x7461646d) {                                                    // mdat
			FileInfo->mdat.Loc = ReadOffset;
			FileInfo->mdat.Size = atom[0];
			ReadOffset += atom[0];
		} else if (atom[1] == 0x766f6f6d) {                                             // moov
			FileInfo->moov.Loc = ReadOffset;
			FileInfo->moov.Size = atom[0];
			ReadOffset += 8;
		} else if (atom[1] == 0x6468766d) {                                             // mvhd
			ReadOffset += atom[0];
			ReadSize = sizeof(UINT32) * 5;
			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			if (atom[0] != 0) {
				DBG_ERR("[TLLPR] MP4 atom version 1!!!");
			}
			FileInfo->CreationTime = UINT32_SWAP(atom[1]);
			FileInfo->ModificationTime = UINT32_SWAP(atom[2]);
			FileInfo->TimeScale = UINT32_SWAP(atom[3]);
			FileInfo->Duration = UINT32_SWAP(atom[4]);
		} else if (atom[1] == 0x6b617274) {                                             // trak
			FileInfo->trak.Loc = ReadOffset;
			FileInfo->trak.Size = atom[0];
			ReadOffset += 8;
		} else if (atom[1] == 0x6169646d) {                                             // mdia
			FileInfo->mdia.Loc = ReadOffset;
			FileInfo->mdia.Size = atom[0];
			ReadOffset += 8;
		} else if (atom[1] == 0x666e696d) {                                             // minf
			FileInfo->minf.Loc = ReadOffset;
			FileInfo->minf.Size = atom[0];
			ReadOffset += 8;
		} else if (atom[1] == 0x6c627473) {                                             // stbl
			FileInfo->stbl.Loc = ReadOffset;
			FileInfo->stbl.Size = atom[0];
			ReadOffset += 8;
		} else if (atom[1] == 0x73747473) {                                             // stts
			ReadSize = sizeof(UINT32) * 4;
			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			FileInfo->FrameCnt = UINT32_SWAP(atom[2]);
			Cnt = UINT32_SWAP(atom[1]);
			ReadOffset += (8 + 16 + 8 * (Cnt - 1));
		} else if (atom[1] == 0x73737473) {                                             // stss
			FileInfo->stss.Loc = ReadOffset;
			FileInfo->stss.Size = atom[0];
			ReadSize = sizeof(UINT32) * 2;
			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			Cnt = UINT32_SWAP(atom[1]);
			FileInfo->KeyFrameCnt = Cnt;
			ReadOffset += (8 + 8 + 4 * (Cnt - 1));
			if (ReadOffset > FileInfo->FileSize) {
				return E_SYS;
			}
			FileSys_SeekFile(*MovFile, ReadOffset, FST_SEEK_SET);
			ReadSize = sizeof(UINT32) * 1;
			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			FileInfo->LastKeyFrame = UINT32_SWAP(atom[0]);
			ReadOffset += 4;
		} else if (atom[1] == 0x7a737473) {                                             // stsz
			ReadSize = sizeof(UINT32) * 3;

			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			Cnt = UINT32_SWAP(atom[2]);
			ReadOffset += (8 + 12 + 4 * (Cnt - 1));
			if (ReadOffset > FileInfo->FileSize) {
				return E_SYS;
			}
			FileSys_SeekFile(*MovFile, ReadOffset, FST_SEEK_SET);
			ReadSize = sizeof(UINT32) * 1;
			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			FileInfo->LastFrame.Size = UINT32_SWAP(atom[0]);
			ReadOffset += 4;
		} else if (atom[1] == 0x6f637473) {                                             // stco
			ReadSize = sizeof(UINT32) * 2;

			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			Cnt = UINT32_SWAP(atom[1]);
			ReadOffset += (8 + 8 + 4 * (Cnt - 1));
			if (ReadOffset > FileInfo->FileSize) {
				return E_SYS;
			}
			FileSys_SeekFile(*MovFile, ReadOffset, FST_SEEK_SET);
			ReadSize = sizeof(UINT32) * 1;
			ret = FileSys_ReadFile(*MovFile, (UINT8 *)atom, &ReadSize, FALSE, NULL);
			FileInfo->LastFrame.Loc = UINT32_SWAP(atom[0]);
			ReadOffset += 4;
		} else {
			ReadOffset += atom[0];
		}
	}

	return E_OK;
}

static ER _MovieTLLPR_AppendMdat(FST_FILE *MovFile, MP4Info *FileInfo, MP4Info *NewFileInfo, UINT32 Addr, UINT32 Size, UINT32 MTime)
{
	UINT32 SizeDiff = 0;
	UINT32 NewMdatSize = 0;
	UINT32 WriteSize = 0;

	if (gLPRFileType != MEDIA_FILEFORMAT_MP4) {
		return E_SYS;
	}

	if (*MovFile == NULL) {
		return E_SYS;
	}

	SizeDiff = (FileInfo->mdat.Loc + FileInfo->mdat.Size) - (FileInfo->LastFrame.Loc + FileInfo->LastFrame.Size);
	NewMdatSize = FileInfo->mdat.Size + Size - SizeDiff;

	FileInfo->moov.Loc = FileInfo->mdat.Loc + FileInfo->mdat.Size;
	NewFileInfo->mdat.Loc = FileInfo->mdat.Loc;
	NewFileInfo->mdat.Size = NewMdatSize;
	NewFileInfo->moov.Loc = NewFileInfo->mdat.Loc + NewFileInfo->mdat.Size;
	NewFileInfo->LastFrame.Loc = FileInfo->LastFrame.Loc + FileInfo->LastFrame.Size;
	NewFileInfo->LastFrame.Size = Size;
	NewFileInfo->FrameCnt = FileInfo->FrameCnt + 1;
	//NewFileInfo->KeyFrameCnt = FileInfo->KeyFrameCnt + 1;
	NewFileInfo->Duration = (FileInfo->Duration / FileInfo->FrameCnt) * NewFileInfo->FrameCnt;
	NewFileInfo->ModificationTime = MTime;

	FileSys_SeekFile(*MovFile, FileInfo->mdat.Loc, FST_SEEK_SET);
	NewMdatSize = UINT32_SWAP(NewMdatSize);
	WriteSize = sizeof(UINT32);
	FileSys_WriteFile(*MovFile, (UINT8 *)&NewMdatSize, &WriteSize, FALSE, NULL);

	FileSys_SeekFile(*MovFile, (FileInfo->LastFrame.Loc + FileInfo->LastFrame.Size), FST_SEEK_SET);
	WriteSize = Size;
	FileSys_WriteFile(*MovFile, (UINT8 *)Addr, &WriteSize, FALSE, NULL);

	return E_OK;
}

static ER _MovieTLLPR_AppendMoov(FST_FILE *MovFile, MP4Info *FileInfo, MP4Info *NewFileInfo, UINT32 Buf, UINT32 Size)
{
	UINT32 Data[6] = {0};
	UINT32 WriteSize = 0;
	UINT8 *MoovBuf = (UINT8 *)Buf;
	UINT32 Ptr = 0;
	UINT32 ExtendByte = 0;

	if (gLPRFileType != MEDIA_FILEFORMAT_MP4) {
		return E_SYS;
	}

	if (FileInfo->stss.Loc == 0 && FileInfo->KeyFrameCnt == 0 && FileInfo->FrameCnt == STSS_GAP) { // No stss existed
		ExtendByte = 24 +      // stss atom with 2 two samples
					 4 +      // a new stsz sample
					 4 ;      // a new stco sample
		NewFileInfo->KeyFrameCnt = 2;
	} else if (FileInfo->stss.Loc != 0 && ((FileInfo->FrameCnt) % STSS_GAP) == 0) {                // Add stss / stsz / stco sample
		ExtendByte =  4 +      // a new stss sample
					  4 +      // a new stsz sample
					  4 ;      // a new stco sample
		NewFileInfo->KeyFrameCnt = FileInfo->KeyFrameCnt + 1;
	} else {                                                            // Add stsz / stco sample
		ExtendByte =  0 +      // no new stss sample
					  4 +      // a new stsz sample
					  4 ;      // a new stco sample
		NewFileInfo->KeyFrameCnt = FileInfo->KeyFrameCnt;
	}

	FileSys_SeekFile(*MovFile, NewFileInfo->moov.Loc, FST_SEEK_SET);
	while (Ptr < Size) {
		if (MoovBuf[Ptr + 4] == 'm' && MoovBuf[Ptr + 5] == 'o' && MoovBuf[Ptr + 6] == 'o' && MoovBuf[Ptr + 7] == 'v') {     // moov
			WriteSize = sizeof(UINT8) * 8;
			NewFileInfo->moov.Size = FileInfo->moov.Size + ExtendByte;
			MoovBuf[Ptr + 0] = (UINT8)((NewFileInfo->moov.Size >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((NewFileInfo->moov.Size >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((NewFileInfo->moov.Size >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((NewFileInfo->moov.Size >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 'm' && MoovBuf[Ptr + 5] == 'v' && MoovBuf[Ptr + 6] == 'h' && MoovBuf[Ptr + 7] == 'd') { // mvhd
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			// Modification time
			MoovBuf[Ptr + 16] = (UINT8)((NewFileInfo->ModificationTime >> 24) & 0xff);
			MoovBuf[Ptr + 17] = (UINT8)((NewFileInfo->ModificationTime >> 16) & 0xff);
			MoovBuf[Ptr + 18] = (UINT8)((NewFileInfo->ModificationTime >>  8) & 0xff);
			MoovBuf[Ptr + 19] = (UINT8)((NewFileInfo->ModificationTime >>  0) & 0xff);
			// Duration
			MoovBuf[Ptr + 24] = (UINT8)((NewFileInfo->Duration >> 24) & 0xff);
			MoovBuf[Ptr + 25] = (UINT8)((NewFileInfo->Duration >> 16) & 0xff);
			MoovBuf[Ptr + 26] = (UINT8)((NewFileInfo->Duration >>  8) & 0xff);
			MoovBuf[Ptr + 27] = (UINT8)((NewFileInfo->Duration >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 't' && MoovBuf[Ptr + 5] == 'r' && MoovBuf[Ptr + 6] == 'a' && MoovBuf[Ptr + 7] == 'k') { // trak
			WriteSize = sizeof(UINT8) * 8;
			NewFileInfo->trak.Size = FileInfo->trak.Size + ExtendByte;
			MoovBuf[Ptr + 0] = (UINT8)((NewFileInfo->trak.Size >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((NewFileInfo->trak.Size >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((NewFileInfo->trak.Size >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((NewFileInfo->trak.Size >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 't' && MoovBuf[Ptr + 5] == 'k' && MoovBuf[Ptr + 6] == 'h' && MoovBuf[Ptr + 7] == 'd') { // tkhd
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			// Modification time
			MoovBuf[Ptr + 16] = (UINT8)((NewFileInfo->ModificationTime >> 24) & 0xff);
			MoovBuf[Ptr + 17] = (UINT8)((NewFileInfo->ModificationTime >> 16) & 0xff);
			MoovBuf[Ptr + 18] = (UINT8)((NewFileInfo->ModificationTime >>  8) & 0xff);
			MoovBuf[Ptr + 19] = (UINT8)((NewFileInfo->ModificationTime >>  0) & 0xff);
			// Duration
			MoovBuf[Ptr + 28] = (UINT8)((NewFileInfo->Duration >> 24) & 0xff);
			MoovBuf[Ptr + 29] = (UINT8)((NewFileInfo->Duration >> 16) & 0xff);
			MoovBuf[Ptr + 30] = (UINT8)((NewFileInfo->Duration >>  8) & 0xff);
			MoovBuf[Ptr + 31] = (UINT8)((NewFileInfo->Duration >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 'm' && MoovBuf[Ptr + 5] == 'd' && MoovBuf[Ptr + 6] == 'i' && MoovBuf[Ptr + 7] == 'a') { // mdia
			WriteSize = sizeof(UINT8) * 8;
			NewFileInfo->mdia.Size = FileInfo->mdia.Size + ExtendByte;
			MoovBuf[Ptr + 0] = (UINT8)((NewFileInfo->mdia.Size >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((NewFileInfo->mdia.Size >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((NewFileInfo->mdia.Size >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((NewFileInfo->mdia.Size >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 'm' && MoovBuf[Ptr + 5] == 'd' && MoovBuf[Ptr + 6] == 'h' && MoovBuf[Ptr + 7] == 'd') { // mdhd
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			// Modification time
			MoovBuf[Ptr + 16] = (UINT8)((NewFileInfo->ModificationTime >> 24) & 0xff);
			MoovBuf[Ptr + 17] = (UINT8)((NewFileInfo->ModificationTime >> 16) & 0xff);
			MoovBuf[Ptr + 18] = (UINT8)((NewFileInfo->ModificationTime >>  8) & 0xff);
			MoovBuf[Ptr + 19] = (UINT8)((NewFileInfo->ModificationTime >>  0) & 0xff);
			// Duration
			MoovBuf[Ptr + 24] = (UINT8)((NewFileInfo->Duration >> 24) & 0xff);
			MoovBuf[Ptr + 25] = (UINT8)((NewFileInfo->Duration >> 16) & 0xff);
			MoovBuf[Ptr + 26] = (UINT8)((NewFileInfo->Duration >>  8) & 0xff);
			MoovBuf[Ptr + 27] = (UINT8)((NewFileInfo->Duration >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 'm' && MoovBuf[Ptr + 5] == 'i' && MoovBuf[Ptr + 6] == 'n' && MoovBuf[Ptr + 7] == 'f') { // minf
			WriteSize = sizeof(UINT8) * 8;
			NewFileInfo->minf.Size = FileInfo->minf.Size + ExtendByte;
			MoovBuf[Ptr + 0] = (UINT8)((NewFileInfo->minf.Size >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((NewFileInfo->minf.Size >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((NewFileInfo->minf.Size >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((NewFileInfo->minf.Size >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 's' && MoovBuf[Ptr + 5] == 't' && MoovBuf[Ptr + 6] == 'b' && MoovBuf[Ptr + 7] == 'l') { // stbl
			WriteSize = sizeof(UINT8) * 8;
			NewFileInfo->stbl.Size = FileInfo->stbl.Size + ExtendByte;
			MoovBuf[Ptr + 0] = (UINT8)((NewFileInfo->stbl.Size >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((NewFileInfo->stbl.Size >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((NewFileInfo->stbl.Size >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((NewFileInfo->stbl.Size >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		} else if (MoovBuf[Ptr + 4] == 's' && MoovBuf[Ptr + 5] == 't' && MoovBuf[Ptr + 6] == 't' && MoovBuf[Ptr + 7] == 's') { // stts
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			MoovBuf[Ptr + 16] = (UINT8)((NewFileInfo->FrameCnt >> 24) & 0xff);
			MoovBuf[Ptr + 17] = (UINT8)((NewFileInfo->FrameCnt >> 16) & 0xff);
			MoovBuf[Ptr + 18] = (UINT8)((NewFileInfo->FrameCnt >>  8) & 0xff);
			MoovBuf[Ptr + 19] = (UINT8)((NewFileInfo->FrameCnt >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
			if (FileInfo->stss.Loc == 0 && FileInfo->KeyFrameCnt == 0 && FileInfo->FrameCnt == STSS_GAP) {                  // Need to add stss
				WriteSize = 24;
				Data[0] = UINT32_SWAP(WriteSize);
				Data[1] = 0x73737473;
				Data[2] = UINT32_SWAP(0);
				Data[3] = UINT32_SWAP(NewFileInfo->KeyFrameCnt);
				Data[4] = UINT32_SWAP(1);
				Data[5] = UINT32_SWAP(NewFileInfo->FrameCnt);
				FileSys_WriteFile(*MovFile, (UINT8 *)Data, &WriteSize, FALSE, NULL);
			}
		} else if (MoovBuf[Ptr + 4] == 's' && MoovBuf[Ptr + 5] == 't' && MoovBuf[Ptr + 6] == 's' && MoovBuf[Ptr + 7] == 's') { // stss
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			Data[0] = (((FileInfo->FrameCnt) % STSS_GAP) == 0) ? (WriteSize + 4) : WriteSize;
			MoovBuf[Ptr + 0] = (UINT8)((Data[0] >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((Data[0] >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((Data[0] >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((Data[0] >>  0) & 0xff);
			MoovBuf[Ptr + 12] = (UINT8)((NewFileInfo->KeyFrameCnt >> 24) & 0xff);
			MoovBuf[Ptr + 13] = (UINT8)((NewFileInfo->KeyFrameCnt >> 16) & 0xff);
			MoovBuf[Ptr + 14] = (UINT8)((NewFileInfo->KeyFrameCnt >>  8) & 0xff);
			MoovBuf[Ptr + 15] = (UINT8)((NewFileInfo->KeyFrameCnt >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
			if (((FileInfo->FrameCnt) % STSS_GAP) == 0) {
				WriteSize = 4;
				Data[0] = UINT32_SWAP(NewFileInfo->FrameCnt);
				FileSys_WriteFile(*MovFile, (UINT8 *)Data, &WriteSize, FALSE, NULL);
			}
		} else if (MoovBuf[Ptr + 4] == 's' && MoovBuf[Ptr + 5] == 't' && MoovBuf[Ptr + 6] == 's' && MoovBuf[Ptr + 7] == 'z') { // stsz
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			Data[0] = WriteSize + 4;
			MoovBuf[Ptr + 0] = (UINT8)((Data[0] >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((Data[0] >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((Data[0] >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((Data[0] >>  0) & 0xff);
			MoovBuf[Ptr + 16] = (UINT8)((NewFileInfo->FrameCnt >> 24) & 0xff);
			MoovBuf[Ptr + 17] = (UINT8)((NewFileInfo->FrameCnt >> 16) & 0xff);
			MoovBuf[Ptr + 18] = (UINT8)((NewFileInfo->FrameCnt >>  8) & 0xff);
			MoovBuf[Ptr + 19] = (UINT8)((NewFileInfo->FrameCnt >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
			WriteSize = 4;
			Data[0] = UINT32_SWAP(NewFileInfo->LastFrame.Size);
			FileSys_WriteFile(*MovFile, (UINT8 *)Data, &WriteSize, FALSE, NULL);
		} else if (MoovBuf[Ptr + 4] == 's' && MoovBuf[Ptr + 5] == 't' && MoovBuf[Ptr + 6] == 'c' && MoovBuf[Ptr + 7] == 'o') { // stco
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			Data[0] = WriteSize + 4;
			MoovBuf[Ptr + 0] = (UINT8)((Data[0] >> 24) & 0xff);
			MoovBuf[Ptr + 1] = (UINT8)((Data[0] >> 16) & 0xff);
			MoovBuf[Ptr + 2] = (UINT8)((Data[0] >>  8) & 0xff);
			MoovBuf[Ptr + 3] = (UINT8)((Data[0] >>  0) & 0xff);
			MoovBuf[Ptr + 12] = (UINT8)((NewFileInfo->FrameCnt >> 24) & 0xff);
			MoovBuf[Ptr + 13] = (UINT8)((NewFileInfo->FrameCnt >> 16) & 0xff);
			MoovBuf[Ptr + 14] = (UINT8)((NewFileInfo->FrameCnt >>  8) & 0xff);
			MoovBuf[Ptr + 15] = (UINT8)((NewFileInfo->FrameCnt >>  0) & 0xff);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
			WriteSize = 4;
			Data[0] = UINT32_SWAP(NewFileInfo->LastFrame.Loc);
			FileSys_WriteFile(*MovFile, (UINT8 *)Data, &WriteSize, FALSE, NULL);
		} else {
			WriteSize = (MoovBuf[Ptr + 0] << 24) + (MoovBuf[Ptr + 1] << 16) + (MoovBuf[Ptr + 2] << 8) + (MoovBuf[Ptr + 3] << 0);
			FileSys_WriteFile(*MovFile, (UINT8 *)(&(MoovBuf[Ptr])), &WriteSize, FALSE, NULL);
			Ptr += WriteSize;
		}
	}

	return E_OK;
}

static ER _MovieTLLPR_CheckRecordingFile(char *Filename, UINT32 Mode, struct tm *CTime)
{
	ER ret = E_SYS;
	FST_FILE RecFile = NULL;
	MP4Info RecFileInfo = {0};
	UINT32 FileCTime[6] = {0}, FileMTime[6] = {0};


	if (Filename == NULL) {
		return E_SYS;
	}

	if (System_GetState(SYS_STATE_CARD)  != CARD_INSERTED) {
		return E_SYS;
	}

	RecFile = FileSys_OpenFile(Filename, FST_OPEN_EXISTING | FST_OPEN_READ);
	if (RecFile == NULL) {
		return E_SYS;
	}

	if (Mode == 0) {    // Simple check whether file existed.
		_MovieTLLPR_CloseFile(&RecFile, NULL);
		return E_OK;
	}

	if (gLPRFileType == MEDIA_FILEFORMAT_MP4) {
		ret = _MovieTLLPR_ParseFileInfo(&RecFile, &RecFileInfo);
		if (ret == E_OK && CTime) {
			_MovieTLLPR_ConvertTime2DateTime(RecFileInfo.CreationTime, CTime);
		}
		_MovieTLLPR_CloseFile(&RecFile, NULL);
	} else { // gLPRFileType == MEDIA_FILEFORMAT_TS
		ret =  E_OK;
		_MovieTLLPR_CloseFile(&RecFile, NULL);
		if (ret == E_OK && CTime) {
			FileSys_GetDateTime(Filename, FileCTime, FileMTime);
			CTime->tm_year = FileCTime[0];
			CTime->tm_mon = FileCTime[1];
			CTime->tm_mday = FileCTime[2];
			CTime->tm_hour = FileCTime[3];
			CTime->tm_min = FileCTime[4];
			CTime->tm_sec = FileCTime[5];
		}
	}

	return ret;
}

static ER _MovieTLLPR_CheckBothRecordingFile(char *Filename0, char *Filename1)
{
	ER ret0 = E_SYS, ret1 = E_SYS, ret = E_SYS;

	ret0 = _MovieTLLPR_CheckRecordingFile(Filename0, 1, NULL);
	ret1 = _MovieTLLPR_CheckRecordingFile(Filename1, 1, NULL);
	if (UI_GetData(FL_MOVIE_SIZE) < MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30) {    // Signal channel
		ret = ret0;
	} else {                                                                        // Dual channel
		ret = ((ret0 == E_SYS) && (ret1 == E_SYS)) ? E_SYS : E_OK;
	}
	return ret;
}

static ER _MovieTLLPR_RenameFile(void)
{
	UINT32 ChannelID = 0;
	ER ret = E_SYS;
	struct tm CTime = {0};
	char SrcFileName[FILENAME_LEN] = {0};
	char DestFileName[FILENAME_LEN] = {0};
	char *NamePtr = NULL;
	UINT32 i = 0;
	char PostFix[2] = {'A', 'B'};

	for (ChannelID = 0; ChannelID < 2; ChannelID ++) {
		if ((ChannelID == 1) && (UI_GetData(FL_MOVIE_SIZE) < MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30)) {    // Signal channel
			break;
		}

		snprintf(SrcFileName, FILENAME_LEN, "A:\\%s\\%s\\%s", FILEDB_CARDV_ROOT, ChannelID ? TIMELAPSE1_FOLDER : TIMELAPSE0_FOLDER, ChannelID ? TIMELAPSE1_RECFILE : TIMELAPSE0_RECFILE);
		strncat(SrcFileName, (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);

		ret = _MovieTLLPR_CheckRecordingFile(SrcFileName, 1, &CTime);

		if (ret == E_SYS) {
			return E_SYS;
		} else if (ret == E_NOSPT) {
			FileSys_DeleteFile(SrcFileName);
		} else {
			snprintf(DestFileName, FILENAME_LEN, "A:\\%s\\%s\\", FILEDB_CARDV_ROOT, ChannelID ? TIMELAPSE1_FOLDER : TIMELAPSE0_FOLDER);
			NamePtr = &(DestFileName[strlen(DestFileName)]);
			for (i = 0; i < 1000; i ++) {
				snprintf(NamePtr, (FILENAME_LEN - strlen(DestFileName)), "%04u%02u%02u_%02u%02u%02u_%03u%c", CTime.tm_year, CTime.tm_mon, CTime.tm_mday, CTime.tm_hour, CTime.tm_min, CTime.tm_sec, i, PostFix[ChannelID]);
				strncat(NamePtr, (gLPRFileType == MEDIA_FILEFORMAT_MP4) ? ".MP4" : ".TS", 4);
				if (_MovieTLLPR_CheckRecordingFile(DestFileName, 1, NULL) == E_SYS) {
					DBG_DUMP("[TLLPR] Rename recording file to: %s\r\n", DestFileName);
					break;
				}
			}
			FileSys_RenameFile(NamePtr, SrcFileName, FALSE);
		}
	}
	return E_OK;
}

static ER _MovieTLLPR_OneShotCapture(void)
{
	FlowMovie_StartRec();
	// Todo: Now use a fix delay. But how to use a safe way to check encode is really start?
	//
	//
	Delay_DelayMs(50);
	FlowMovie_StopRec();
	// Todo: How to make sure the file is closed by SMedia?
	//
	//
	_MovieTLLPR_FilePostProcess();

	// For OSD display test
	snprintf(gStrNextDateTime, 30, "%04u/%02u/%02u  %02u:%02u:%02u", gNextAlarmTime.tm_year, gNextAlarmTime.tm_mon, gNextAlarmTime.tm_mday, gNextAlarmTime.tm_hour, gNextAlarmTime.tm_min, gNextAlarmTime.tm_sec);

	return E_OK;
}

static ER _MovieTLLPR_SetNextHWRT(UINT32 TimelapseSec)
{
	struct tm CurrTime = {0};
	struct tm AlarmTime = {0};
	UINT32 CurrTime1 = 0;
	UINT32 AlarmTime1 = 0;

	CurrTime1 = _MovieTLLPR_GetTimeInSec(TIME_ID_CURRENT);
	AlarmTime1 = CurrTime1 + TimelapseSec - PRE_BOOT_TIME;
	_MovieTLLPR_ConvertTime2DateTime((CurrTime1 - MP4_TIME_BASE), &CurrTime);
	_MovieTLLPR_ConvertTime2DateTime((AlarmTime1 - MP4_TIME_BASE), &AlarmTime);

	HwClock_SetTime(TIME_ID_HWRT, AlarmTime, 0);
	HwPower_SetPowerKey(POWER_ID_HWRT, FALSE);      // Disable->Enable to clear flags.
	HwPower_SetPowerKey(POWER_ID_HWRT, TRUE);

	DBG_DUMP("[TLLPR] Current Time: %04u/%02u/%02u %02u:%02u:%02u\r\n", CurrTime.tm_year, CurrTime.tm_mon, CurrTime.tm_mday, CurrTime.tm_hour, CurrTime.tm_min, CurrTime.tm_sec);
	DBG_DUMP("[TLLPR]  Next Alarm : %04u/%02u/%02u %02u:%02u:%02u\r\n", AlarmTime.tm_year, AlarmTime.tm_mon, AlarmTime.tm_mday, AlarmTime.tm_hour, AlarmTime.tm_min, AlarmTime.tm_sec);

	// For OSD display test
	_MovieTLLPR_ConvertTime2DateTime((CurrTime1 + TimelapseSec - MP4_TIME_BASE), &gNextAlarmTime);

	return E_OK;
}

static ER _MovieTLLPR_StartHWRT(void)
{
	HwPower_SetPowerKey(POWER_ID_HWRT, TRUE);
	return E_OK;
}

static ER _MovieTLLPR_StopHWRT(void)
{
	struct tm AlarmTime = {0};

	AlarmTime = HwClock_GetTime(TIME_ID_CURRENT);
	AlarmTime.tm_hour = 0;
	AlarmTime.tm_min = 0;
	AlarmTime.tm_sec = 0;
	HwClock_SetTime(TIME_ID_HWRT, AlarmTime, 0);        // Set Alarm Time to <current day> 00:00:00 since rtc driver can only accept same day.
	HwPower_SetPowerKey(POWER_ID_HWRT, FALSE);
	return E_OK;
}

static ER _MovieTLLPR_WaitTriggerTime(void)
{
	UINT32 CurrTime = 0;
	UINT32 AlarmTime = 0;

	CurrTime = _MovieTLLPR_GetTimeInSec(TIME_ID_CURRENT);
	AlarmTime = _MovieTLLPR_GetTimeInSec(TIME_ID_HWRT);

	AlarmTime += PRE_BOOT_TIME;

	while (AlarmTime > CurrTime) {
		Delay_DelayMs(200);
		CurrTime = _MovieTLLPR_GetTimeInSec(TIME_ID_CURRENT);
	}
	return E_OK;
}

static UINT32 _MovieTLLPR_GetTimeInSec(UINT32 TimeID)
{
	struct tm CurrTime = {0};
	struct tm AlarmTime = {0};
	UINT32 CurrTime1 = 0;
	UINT32 AlarmTime1 = 0;

	CurrTime = HwClock_GetTime(TIME_ID_CURRENT);

	if (UseNewLibC) {
		CurrTime.tm_year -= 1900;
		CurrTime.tm_mon--;
	}
	CurrTime1 = mktime(&CurrTime);

	if (UseNewLibC) {
		CurrTime1 += YEAR1970BASE;
	}

	if (TimeID == TIME_ID_CURRENT) {
		return CurrTime1;
	}

	if (TimeID == TIME_ID_HWRT) {
		AlarmTime = HwClock_GetTime(TIME_ID_HWRT);
		if (UseNewLibC) {
			AlarmTime.tm_year -= 1900;
			AlarmTime.tm_mon--;
		}
		AlarmTime1 = mktime(&AlarmTime);
		if (UseNewLibC) {
			AlarmTime1 += YEAR1970BASE;
		}

		// Workaround here, now fixed in rtc.c, but only for 660
		if ((AlarmTime1 > CurrTime1) && (AlarmTime1 - CurrTime1) > (86400 * 31)) {
			AlarmTime1 -= (86400 * 32);
		}
		return AlarmTime1;
	}
	return 0;
}

static UINT32 _MovieTLLPR_CheckCLibVersion(void)
{
	extern const char LibC_LIBRARY_VERSION_INFO[];
	char *pPtr = NULL;
	char StrBuf[40] = {0};
	UINT32 Ver = 0;

	if ((pPtr = strstr(LibC_LIBRARY_VERSION_INFO, "#")) != NULL) {
		strncpy(StrBuf, pPtr, 40);
		pPtr = StrBuf;
	} else {
		DBG_WRN("[TLLPR] Cannot recognize LibC version!\r\n");
		return 0;
	}

	// If version >= 1.00.007, the mktime() and localtime() are follow standard C lib
	if ((pPtr = strtok(pPtr, "#._")) != NULL) {
		Ver = atoi(pPtr);
		if (Ver > 1) {  // Major version > 1
			return 1;
		}
	} else {
		DBG_WRN("[TLLPR] Cannot find major version!\r\n");
		return 0;
	}

	if ((pPtr = strtok(NULL, "#._")) != NULL) {
		Ver = atoi(pPtr);
		if (Ver > 0) {  // Minor version > 0
			return 1;
		}
	} else {
		DBG_WRN("[TLLPR] Cannot find minor version!\r\n");
		return 0;
	}

	if ((pPtr = strtok(NULL, "#._")) != NULL) {
		Ver = atoi(pPtr);
		if (Ver > 6) {  // Bugfix version > 6
			return 1;
		}
	} else {
		DBG_WRN("[TLLPR] Cannot find bugfix version!\r\n");
		return 0;
	}
	return 0;
}

static UINT32 _MovieTLLPR_CheckTSFileSize(FST_FILE *MovFile)
{
	UINT32 FileLen = 0;

	if (gLPRFileType != MEDIA_FILEFORMAT_TS) {
		return 0;
	}

	FileSys_SeekFile(*MovFile, 0, FST_SEEK_END);
	FileLen = FileSys_TellFile(*MovFile);

	if (FileLen < 188 || (FileLen % 188)) {
		DBG_ERR("[TLLPR] File Invalid!!!");
		return 0;
	}
	return FileLen;
}

static UINT32 _MovieTLLPR_GetTSPktCnt(FST_FILE *MovFile, TSPKT Type)
{
	UINT32 FileLen = 0;
	UINT32 RWSize = 0;
	UINT8 *Cnt = NULL, *TSPktBuf = NULL;
	UINT32 HeadOffset = 0, TailOffset = 0;
	UINT8 Pid[2] = {0};
	ER Ret = E_SYS;
#if (__DBGLVL__> 1)
	char PacketName[4][4] = {"PAT", "PMT", "PCR", "VID"};
#endif

	if (gLPRFileType != MEDIA_FILEFORMAT_TS) {
		return E_SYS;
	}

	switch (Type) {
	case TSPKT_PAT:
		Cnt = &g_CntPAT;
		TSPktBuf = g_pPATBuf;
		HeadOffset = 0;
		TailOffset = 564;   // 188 * 3
		Pid[0] = 0x40;                                                  // Start indicator = 1, Pid = 0x00
		Pid[1] = 0x00;
		break;
	case TSPKT_PMT:
		Cnt = &g_CntPMT;
		TSPktBuf = g_pPMTBuf;
		HeadOffset = 188;
		TailOffset = 376;   // 188 * 2
		Pid[0] = 0x40;                                                  // Start indicator = 1, Pid = 0x40
		Pid[1] = 0x40;
		break;
	case TSPKT_PCR:
		Cnt = &g_CntPCR;
		TSPktBuf = g_pPCRBuf;
		HeadOffset = 376;   // 188 * 2
		TailOffset = 188;
		Pid[0] = 0x41;                                                  // Start indicator = 1, Pid = 0x100
		Pid[1] = 0x00;
		break;
	case TSPKT_VID:
		Cnt = &g_CntVideo;
		TSPktBuf = g_pVideoBuf;
		HeadOffset = 564;   // 188 * 3
		TailOffset = 752;   // 188 * 4
		Pid[0] = 0x02;                                                  // Start indicator = 0, Pid = 0x200
		Pid[1] = 0x00;
		break;
	}

	*Cnt = 0xF0;

	FileLen = _MovieTLLPR_CheckTSFileSize(MovFile);

	FileSys_SeekFile(*MovFile, (FileLen - TailOffset), FST_SEEK_SET);       // Assume last four TS packet is VID-PAT-PMT-PCR
	RWSize = 188;
	Ret = FileSys_ReadFile(*MovFile, TSPktBuf, &RWSize, FALSE, NULL);
	if (RWSize != 188 || TSPktBuf[0] != 0x47) {
		DBG_ERR("[TLLPR] Not a TS stream!!! %d %d %x\r\n", Type, RWSize, TSPktBuf[0]);
		return E_SYS;
	}
	if (TSPktBuf[1] != Pid[0] || TSPktBuf[2] != Pid[1]) {
		DBG_IND("[TLLPR] No %s in the tail, then searh from head!!!\r\n", PacketName[Type]);
		FileSys_SeekFile(*MovFile, HeadOffset, FST_SEEK_SET);
		RWSize = 188;
		Ret = FileSys_ReadFile(*MovFile, TSPktBuf, &RWSize, FALSE, NULL);
		if (RWSize != 188 || TSPktBuf[0] != 0x47 || TSPktBuf[1] != Pid[0] || TSPktBuf[2] != Pid[1]) {
			DBG_ERR("[TLLPR] Not a TS stream!!! %d %d %x %x %x / %x %x\r\n", Type, RWSize, TSPktBuf[0], TSPktBuf[1], TSPktBuf[2], Pid[0], Pid[1]);
			return E_SYS;
		}
	}
	*Cnt = TSPktBuf[3] & 0x0F;

	if (Type == TSPKT_PCR) {
		g_PCR = ((UINT64)TSPktBuf[6] << 25) | ((UINT64)TSPktBuf[7] << 17) |
				((UINT64)TSPktBuf[8] << 9) | ((UINT64)TSPktBuf[9] << 1) |
				((UINT64)TSPktBuf[10] >> 7);
	}

	return E_OK;
}

static UINT32 _MovieTLLPR_UpdateTSPktCnt(TSPKT Type, UINT8 *TSPktBuf)
{
	UINT8 *Cnt = NULL;

	switch (Type) {
	case TSPKT_PAT:
		Cnt = &g_CntPAT;
		break;
	case TSPKT_PMT:
		Cnt = &g_CntPMT;
		break;
	case TSPKT_PCR:
		Cnt = &g_CntPCR;
		break;
	case TSPKT_VID:
		Cnt = &g_CntVideo;
		break;
	}
	if ((*Cnt & 0xF0) == 0) {           // Valid Cnt
		*Cnt = (*Cnt + 1) & 0x0F;
		TSPktBuf[3] = (TSPktBuf[3] & 0xF0) | (*Cnt);

		if (Type == TSPKT_PCR) {
			g_PCR += 3000UL;

			TSPktBuf[6] = (g_PCR >> 25) & 0xFF;
			TSPktBuf[7] = (g_PCR >> 17) & 0xFF;
			TSPktBuf[8] = (g_PCR >> 9) & 0xFF;
			TSPktBuf[9] = (g_PCR >> 1) & 0xFF;
			TSPktBuf[10] = ((g_PCR << 7) & 0x80) | 0x7E;
		}
		return E_OK;
	}
	return E_SYS;
}

static UINT32 _MovieTLLPR_WriteTSPkt(FST_FILE *MovFile, TSPKT Type)
{
	UINT32 RWSize = 0;
	UINT8 *TSPktBuf = NULL;

	switch (Type) {
	case TSPKT_PAT:
		TSPktBuf = g_pPATBuf;
		break;
	case TSPKT_PMT:
		TSPktBuf = g_pPMTBuf;
		break;
	case TSPKT_PCR:
		TSPktBuf = g_pPCRBuf;
		break;
	case TSPKT_VID:
		TSPktBuf = g_pVideoBuf;
		break;
	}
	RWSize = 188;
	FileSys_SeekFile(*MovFile, 0, FST_SEEK_END);
	FileSys_WriteFile(*MovFile, TSPktBuf, &RWSize, FALSE, NULL);

	return E_OK;
}

static UINT32 _MovieTLLPR_CopyVideo(FST_FILE *DestFile, FST_FILE *SrcFile)
{
	UINT32 i = 0;
	UINT32 RWSize = 0;
	UINT32 FileLen = 0;
	UINT32 PktCnt = 0;
	UINT8 *TSPktBuf = NULL;
	ER Ret = E_SYS;

	FileLen = _MovieTLLPR_CheckTSFileSize(SrcFile);

	PktCnt = FileLen / 188 - 4;                         // Discard front PAT / PMT / PCR and rear PAT, total 4 packet.

	if (FileLen > TmpMdatSize - 188 * 6) {
		DBG_WRN("[TLLPR] Frame size is too large!");
	}

	FileSys_SeekFile(*SrcFile, 564, FST_SEEK_SET);      // Jump to 4th packet (video start)
	FileSys_SeekFile(*DestFile, 0, FST_SEEK_END);
	TSPktBuf = g_pVideoBuf;

	RWSize = PktCnt * 188;
	Ret = FileSys_ReadFile(*SrcFile, TSPktBuf, &RWSize, FALSE, NULL);

	for (i = 0; i < PktCnt; i ++) {
		if (TSPktBuf[i * 188] != 0x47) {
			DBG_ERR("[TLLPR] Src TS file invalid!!!\r\n");
			return E_SYS;
		}
		if (TSPktBuf[i * 188 + 1] == 0x42 && TSPktBuf[i * 188 + 2] == 0x00) {       // Video packet with start indicator, need to update PTS and counter
			// Update PTS, use next PCR as PTS
			TSPktBuf[i * 188 + 13] = 0x20 | ((g_PCR >> 29) & 0x0E) | 0x01;
			TSPktBuf[i * 188 + 14] = (g_PCR >> 22) & 0xFF;
			TSPktBuf[i * 188 + 15] = ((g_PCR >> 14) & 0xFE) | 0x01;
			TSPktBuf[i * 188 + 16] = (g_PCR >> 7) & 0xFF;
			TSPktBuf[i * 188 + 17] = ((g_PCR << 1) & 0xFE) | 0x01;
		}
		_MovieTLLPR_UpdateTSPktCnt(TSPKT_VID, &(TSPktBuf[i * 188]));
	}
	memcpy(&(TSPktBuf[PktCnt * 188]), g_pPATBuf, 188 * 3);
	RWSize = 188 * (PktCnt + 3);                                                    // Append video + PAT + PMT + PCR
	FileSys_WriteFile(*DestFile, TSPktBuf, &RWSize, FALSE, NULL);

	return E_OK;
}

#endif
//#NT#2016/03/07#KCHong -end
