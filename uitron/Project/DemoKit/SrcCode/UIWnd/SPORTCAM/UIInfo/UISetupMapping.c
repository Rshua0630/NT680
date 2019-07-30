
////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////

#if 0
static UINT32 ubLanguageIndex = ENGLISH;
//#NT#2009/12/17#Photon Lin -begin
//static UINT32 ubPowerOffIndex = POWER_OFF;
//#NT#2009/12/17#Photon Lin -end
//#NT#2009/12/16#Photon Lin -begin
//static UINT32 ubBeepIndex = BEEP_ON;
//#NT#2009/12/16#Photon Lin -end
//#NT#2009/12/15#Lincy Lin -begin
//static UINT32 ubBrightIndex = BRIGHT_0;
static UINT32 ubBrightIndex = LCDBRT_LVL_05;
//#NT#2009/12/15#Lincy Lin -end
static UINT32 ubDateFormatIndex = 1;
static UINT32 ubTVFormatIndex = VIDEOOUT_NTSC;

UINT16 FilesysDirNumber = 100, FilesysFileNumber = 1;

/////////////////////////////////////////////////////
//#NT#2009/12/16#Photon Lin -begin
/*
void Set_BeepIndex(UINT32 ubIndex)
{
    ubBeepIndex = ubIndex;
}

UINT32 Get_BeepIndex(void)
{
    return ubBeepIndex;
}
*/
//#NT#2009/12/16#Photon Lin -end
/////////////////////////////////////////////////////

void Set_LanguageIndex(UINT32 ubIndex)
{
	ubLanguageIndex = ubIndex;
}

UINT32 Get_LanguageIndex(void)
{
	return ubLanguageIndex;
}

/////////////////////////////////////////////////////
//#NT#2009/12/16#Photon Lin -begin
/*
void Set_PowerOffIndex(UINT32 ubIndex)
{
    ubPowerOffIndex = ubIndex;
}

UINT32 Get_PowerOffIndex(void)
{
    return ubPowerOffIndex;
}
*/
//#NT#2009/12/16#Photon Lin -end
/////////////////////////////////////////////////////


void Set_BrightIndex(UINT32 ubIndex)
{
	ubBrightIndex = ubIndex;
}

UINT32 Get_BrightIndex(void)
{
	return ubBrightIndex;
}
void Set_TVFormatIndex(UINT32 ubIndex)
{
	ubTVFormatIndex = ubIndex;
}

UINT32 Get_TVFormatIndex(void)
{
	return ubTVFormatIndex;
}

/////////////////////////////////////////////////////



void Set_DateFormatIndex(UINT32 ubIndex)
{
	ubDateFormatIndex = ubIndex;
}

UINT32 Get_DateFormatIndex(void)
{
	return ubDateFormatIndex;
}


/////////////////////////////////////////////////////

#include "GxFile.h"
UINT64  gUIDiskTotal = 1;
UINT64  gUIDiskRemain = 0;
BOOL bDiskInfoModified = FALSE;

void Get_DiskInfo(void)
{
	gUIDiskTotal = FilesysGetDiskInfo(FST_INFO_DISK_SIZE);
	gUIDiskRemain = FilesysGetDiskInfo(FST_INFO_FREE_SPACE);
	gUIDiskTotal /= (1024 * 1024);
	gUIDiskRemain /= (1024 * 1024);
	bDiskInfoModified = TRUE;
}

void Set_FilesysDirId(UINT32 ubIndex)
{
	FilesysDirNumber = ubIndex;
}

UINT16 Get_FilesysDirId(void)
{
	return FilesysDirNumber;
}

void Set_FilesysFileId(UINT32 ubIndex)
{
	FilesysFileNumber = ubIndex;
}

UINT16 Get_FilesysFileId(void)
{
	return FilesysFileNumber;
}

//static UINT16 ubBatteryLevel     = 0;
static UINT16 ubIsCardInsert      = CARD_REMOVED;
static UINT16 ubIsFSStatus        = FS_NOT_INIT;
static UINT16  ExeResult           = EXE_OK;
void SetCardStatus(UINT16 flag)
{
	ubIsCardInsert = flag;
}

UINT16 GetCardStatus(void)
{
	return ubIsCardInsert;
}

void SetFSStatus(UINT16 status)
{
	ubIsFSStatus = status;
}

UINT16 GetFSStatus(void)
{
	return ubIsFSStatus;
}


/////////////////////////////////////////////////////

void SetExeResult(INT32 result)
{
	ExeResult = result;
}

UINT16 GetExeResult(void)
{
	return ExeResult;
}

/////////////////////////////////////////////////////
//#NT#2009/12/16#Photon Lin -begin
//DDX_INDEX ddx_Beep = {Set_BeepIndex, Get_BeepIndex};
//#NT#2009/12/16#Photon Lin -end
DDX_INDEX ddx_TVFormat = {Set_TVFormatIndex, Get_TVFormatIndex};
DDX_INDEX ddx_DateFormat = {Set_DateFormatIndex, Get_DateFormatIndex};
DDX_INDEX ddx_Bright = {Set_BrightIndex, Get_BrightIndex};
//#NT#2009/12/16#Photon Lin -begin
//DDX_INDEX ddx_PowerOff = {Set_PowerOffIndex, Get_PowerOffIndex};
//#NT#2009/12/16#Photon Lin -end
DDX_INDEX ddx_Language = {Set_LanguageIndex, Get_LanguageIndex};

#endif
