//#NT#2016/06/13#Niven Cho -begin
//#NT#TIME_SYNC
#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCb_CustomSi.h"
#include "SysCfg.h"
#include "HwClock.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) |   \
	 ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

#define FOURCC_TIME MAKEFOURCC('T','I','M','E')

typedef struct _MSDCNVT_TIME {
	UINT32 uiFourCC; //Key for 'TIME'
	UINT32 uiYear;
	UINT32 uiMonth;
	UINT32 uiDay;
	UINT32 uiHour;
	UINT32 uiMin;
	UINT32 uiSec;
} MSDCNVT_TIME, *PMSDCNVT_TIME;

//Step 1: Declare custom functions
//Declare Gets Functions
static void GetTime(void);
//Declare Sets Functions
static void SetTime(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void) = {
	GetTime,
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void) = {
	SetTime,
};

//Step 4: Provide API for Register Single Direction Functions
BOOL MsdcNvtRegSi_TimeSi(void)
{
	return MsdcNvt_AddCallback_Si(MSDCCustomGet, sizeof(MSDCCustomGet) / sizeof(MSDCCustomGet[0]), MSDCCustomSet, sizeof(MSDCCustomSet) / sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
static void GetTime(void)
{
	struct tm ct = {0};
	MSDCNVT_TIME *pData    = (MSDCNVT_TIME *)MsdcNvt_GetDataBufferAddress();
	UINT32   uiLength = MsdcNvt_GetTransSize();

	if (uiLength != sizeof(MSDCNVT_TIME)) { //invalid data
		return;
	}

	ct = HwClock_GetTime(TIME_ID_CURRENT);
	pData->uiFourCC = FOURCC_TIME;
	pData->uiYear = ct.tm_year;
	pData->uiMonth = ct.tm_mon;
	pData->uiDay = ct.tm_mday;
	pData->uiHour = ct.tm_hour;
	pData->uiMin = ct.tm_min;
	pData->uiSec = ct.tm_sec;
}

static void SetTime(void)
{
	struct tm ct = {0};
	MSDCNVT_TIME *pData    = (MSDCNVT_TIME *)MsdcNvt_GetDataBufferAddress();
	UINT32  uiLength = MsdcNvt_GetTransSize();

	if (uiLength != sizeof(MSDCNVT_TIME)) { //invalid data
		return;
	}

	if (pData->uiFourCC != FOURCC_TIME) {
		return;
	}

	ct.tm_year = pData->uiYear;
	ct.tm_mon = pData->uiMonth;
	ct.tm_mday = pData->uiDay;
	ct.tm_hour = pData->uiHour;
	ct.tm_min = pData->uiMin;
	ct.tm_sec = pData->uiSec;
	HwClock_SetTime(TIME_ID_CURRENT, ct, 0);
}
//#NT#2016/06/13#Niven Cho -end
