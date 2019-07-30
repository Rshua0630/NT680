////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "PStore.h"
#include "UIInfo.h"
#include "DateTimeInfo.h"
#include "HwClock.h"

//-----------------------------------------------------------------------------
// Date Time
//-----------------------------------------------------------------------------

#define DbgMsg_UISetDate    debug_ind
#define RANGE_YEAR  (MAX_YEAR-MIN_YEAR+1)
//struct tm gUIDateTime = {0, 0, 0, DEF_DAY, DEF_MONTH, DEF_YEAR, 0, 0, 0};

INT32  gUIDateYear = DEF_YEAR;
INT32  gUIDateMonth = DEF_MONTH;
INT32  gUIDateDay = DEF_DAY;
INT32  gUITimeHour = 0;
INT32  gUITimeMinute = 0;
INT32  gUITimeSecond = 0;

INT32  gUIDateSetFmt = DATEFMT_YMD;
INT32  gUITimeSetFmt = TIMEFMT_24HR;

void Validate_Date(INT32 *pYear, INT32 *pMonth, INT32 *pDay,
				   BOOL bClampY, BOOL bClampM, BOOL bClampD);
void Validate_Time(INT32 *pHour, INT32 *pMinute,
				   BOOL bClampH, BOOL bClampM);

void DateTime_Init(void)
{
	gUIDateYear = DEF_YEAR;
	gUIDateMonth = DEF_MONTH;
	gUIDateDay = DEF_DAY;
	gUITimeHour = gUITimeMinute = gUITimeSecond = 0;
	gUIDateSetFmt = DATEFMT_YMD;
	gUITimeSetFmt = TIMEFMT_24HR;
}

void DateTime_Load(void)
{
	struct tm cUIDateTime;

	//load date-time
	cUIDateTime = HwClock_GetTime(TIME_ID_CURRENT);
	gUIDateYear = cUIDateTime.tm_year;
	gUIDateMonth = cUIDateTime.tm_mon;
	gUIDateDay = cUIDateTime.tm_mday;
	gUITimeHour = cUIDateTime.tm_hour;
	gUITimeMinute = cUIDateTime.tm_min;
	gUITimeSecond = cUIDateTime.tm_sec;
	DbgMsg_UISetDate(("Curr Date:Y=%d,M=%d,D=%d\r\n", gUIDateYear, gUIDateMonth, gUIDateDay));
	DbgMsg_UISetDate(("Curr Time:H=%d,M=%d,S=%d\r\n", gUITimeHour, gUITimeMinute, gUITimeSecond));

	//load format
	gUIDateSetFmt = UI_GetData(FL_DateFormatIndex);
	gUITimeSetFmt = UI_GetData(FL_TimeFormatIndex);
	DbgMsg_UISetDate(("Curr DateFmt:%d\r\n", gUIDateSetFmt));
	DbgMsg_UISetDate(("Curr TimeFmt:%d\r\n", gUITimeSetFmt));

	//validate date
	{
		INT32 y, m, d;
		y = gUIDateYear;
		m = gUIDateMonth;
		d = gUIDateDay;
		Validate_Date(&y, &m, &d, 1, 1, 1);
		if ((y != gUIDateYear) && (m != gUIDateMonth) && (d != gUIDateDay)) {
			debug_err(("Date is invalidate=%d,M=%d,D=%d\r\n", gUIDateYear, gUIDateMonth, gUIDateDay));
			debug_err(("Date is clamped=%d,M=%d,D=%d\r\n", y, m, d));
			gUIDateYear = y;
			gUIDateMonth = m;
			gUIDateDay = d;;
			DateTime_Save();
			debug_err(("Date is saved\r\n"));
		}
	}
}

void DateTime_Save(void)
{
	struct tm cUIDateTime = {0};

	DbgMsg_UISetDate(("Save Date:Y=%d,M=%d,D=%d\r\n", gUIDateYear, gUIDateMonth, gUIDateDay));
	DbgMsg_UISetDate(("Save Time:H=%d,M=%d\r\n", gUITimeHour, gUITimeMinute));
	//save date-time
	cUIDateTime.tm_year = gUIDateYear;
	cUIDateTime.tm_mon = gUIDateMonth;
	cUIDateTime.tm_mday = gUIDateDay;
	cUIDateTime.tm_hour = gUITimeHour;
	cUIDateTime.tm_min = gUITimeMinute;
	cUIDateTime.tm_sec = gUITimeSecond;
	HwClock_SetTime(TIME_ID_CURRENT, cUIDateTime, 0);

	DbgMsg_UISetDate(("Save DateFmt:%d\r\n", gUIDateSetFmt));
	DbgMsg_UISetDate(("Save TimeFmt:%d\r\n", gUITimeSetFmt));
	//save format
	UI_SetData(FL_DateFormatIndex, gUIDateSetFmt);
	UI_SetData(FL_TimeFormatIndex, gUITimeSetFmt);

}

void DateTime_SaveToDefault(void)
{
	DateTime_Init();
	DateTime_Save();
	DateTime_Load();
}

void Validate_Date(INT32 *pYear, INT32 *pMonth, INT32 *pDay,
				   BOOL bClampY, BOOL bClampM, BOOL bClampD)
{
#define LOC_Y   (*pYear)
#define LOC_M   (*pMonth)
#define LOC_D   (*pDay)

	INT32 maxDay = 0;

	//clamp/loop year to MIN_YEAR ~ MAX_YEAR
	if (bClampY) {
		LOC_Y = RANGE_Clamp(MIN_YEAR, MAX_YEAR, LOC_Y);
	} else {
		LOC_Y = ((LOC_Y - MIN_YEAR + RANGE_YEAR) % RANGE_YEAR) + MIN_YEAR;
	}

	//clamp/loop month to 1~12
	if (bClampM) {
		LOC_M = RANGE_Clamp(1, 12, LOC_M);
	} else {
		LOC_M = ((LOC_M - 1 + 12) % 12) + 1;
	}

	//clamp/loop day to 1~maxday
	maxDay = TimeUtil_CalcMonthDays(LOC_Y, LOC_M);
	if (bClampD) {
		LOC_D = RANGE_Clamp(1, maxDay, LOC_D);
	} else {
		LOC_D = ((LOC_D - 1 + maxDay) % maxDay) + 1;
	}
}

void Validate_Time(INT32 *pHour, INT32 *pMinute,
				   BOOL bClampH, BOOL bClampM)
{
#define LOC_h   (*pHour)
#define LOC_m   (*pMinute)

	//clamp/loop hour to 0~23
	if (bClampH) {
		LOC_h = RANGE_Clamp(0, 23, LOC_h);
	} else {
		LOC_h = (LOC_h + 24) % 24;
	}

	//clamp/loop minute to 0~59
	if (bClampM) {
		LOC_m = RANGE_Clamp(0, 59, LOC_m);
	} else {
		LOC_m = (LOC_m + 60) % 60;
	}
}

INT32 DateTime_Get(int element)
{
	INT32 rvalue;
	switch (element) {
	case DATETIME_YEAR:
		rvalue = gUIDateYear;
		break;
	case DATETIME_MONTH:
		rvalue = gUIDateYear;
		break;
	case DATETIME_DAY:
		rvalue = gUIDateDay;
		break;
	case DATETIME_HOUR:
		rvalue = gUITimeHour;
		break;
	case DATETIME_MINUTE:
		rvalue = gUITimeMinute;
		break;
	case DATETIME_DATEFMT:
		rvalue = gUIDateSetFmt;
		break;
	case DATETIME_TIMEFMT:
		rvalue = gUITimeSetFmt;
		break;
	default:
		rvalue = 0;
		break;
	}
	return rvalue;
}

void DateTime_Edit(int element, int bias)
{
	switch (element) {
	case DATETIME_YEAR:
		gUIDateYear += bias;
		Validate_Date(&gUIDateYear, &gUIDateMonth, &gUIDateDay, 0, 1, 1);
		break;
	case DATETIME_MONTH:
		gUIDateMonth += bias;
		Validate_Date(&gUIDateYear, &gUIDateMonth, &gUIDateDay, 0, 0, 1);
		break;
	case DATETIME_DAY:
		gUIDateDay += bias;
		Validate_Date(&gUIDateYear, &gUIDateMonth, &gUIDateDay, 0, 0, 0);
		break;
	case DATETIME_HOUR:
		gUITimeHour += bias;
		Validate_Time(&gUITimeHour, &gUITimeMinute, 0, 0);
		break;
	case DATETIME_MINUTE:
		gUITimeMinute += bias;
		Validate_Time(&gUITimeHour, &gUITimeMinute, 0, 0);
		break;
	case DATETIME_DATEFMT:
		gUIDateSetFmt += bias;
		gUIDateSetFmt = ((gUIDateSetFmt + 3) % 3);
		break;
	case DATETIME_TIMEFMT:
		gUITimeSetFmt += bias;
		gUITimeSetFmt = ((gUITimeSetFmt + 2) % 2);
		break;
	}
}


char gUIDateTime_StrBuf[60];

char TimeFmtNameTable[2][5] = {
	"24HR",
	"12HR",
};
char Time12HRNameTable[3][4] = {
	"",
	"AM",
	"PM"
};

//using by date-stamp
char *DateTime_MakeYMD(void)
{
	if (gUIDateSetFmt == DATEFMT_YMD) { //YMD
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%04ld/%02ld/%02ld",
				 gUIDateYear, gUIDateMonth, gUIDateDay);
	} else if (gUIDateSetFmt == DATEFMT_MDY) { //MDY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%04ld",
				 gUIDateMonth, gUIDateDay, gUIDateYear);
	} else { //if(gUIDateSetFmt == DATEFMT_DMY) //DMY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%04ld",
				 gUIDateDay, gUIDateMonth, gUIDateYear);
	}

	return gUIDateTime_StrBuf;
}

//using by setup menu
char *DateTime_MakeYMDHM(void)
{
	INT32 hr, ti;

	hr = gUITimeHour;
	ti = 0;
	if (gUITimeSetFmt == TIMEFMT_12HR) { //apply Time Format
		if (gUITimeHour < 12) {
			ti = 1;
		}
		if (gUITimeHour >= 12) {
			ti = 2;
		}
		hr = hr % 12;
		if (hr == 0) {
			hr = 12;
		}
	}

	if (gUIDateSetFmt == DATEFMT_YMD) { //YMD
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%04ld/%02ld/%02ld %02ld:%02ld%s",
				 gUIDateYear, gUIDateMonth, gUIDateDay, hr, gUITimeMinute, Time12HRNameTable[ti]);
	} else if (gUIDateSetFmt == DATEFMT_MDY) { //MDY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%04ld %02ld:%02ld%s",
				 gUIDateMonth, gUIDateDay, gUIDateYear, hr, gUITimeMinute, Time12HRNameTable[ti]);
	} else { //if(gUIDateSetFmt == DATEFMT_DMY) //DMY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%04ld %02ld:%02ld%s",
				 gUIDateDay, gUIDateMonth, gUIDateYear, hr, gUITimeMinute, Time12HRNameTable[ti]);
	}

	return gUIDateTime_StrBuf;
}


//using by date-time dialog
char *DateTime_MakeYMDHM_Edit(void)
{
	INT32 hr;

	hr = gUITimeHour;
	if (gUITimeSetFmt == TIMEFMT_12HR) { //apply Time Format
		hr = hr % 12;
		if (hr == 0) {
			hr = 12;
		}
	}

	snprintf((char *)gUIDateTime_StrBuf + 0, sizeof(gUIDateTime_StrBuf) - 1, "%04ld", gUIDateYear);
	snprintf((char *)gUIDateTime_StrBuf + 5, sizeof(gUIDateTime_StrBuf) - 6, "%02ld", gUIDateMonth);
	snprintf((char *)gUIDateTime_StrBuf + 8, sizeof(gUIDateTime_StrBuf) - 9, "%02ld", gUIDateDay);
	snprintf((char *)gUIDateTime_StrBuf + 11, sizeof(gUIDateTime_StrBuf) - 12, "%02ld", hr);
	snprintf((char *)gUIDateTime_StrBuf + 14, sizeof(gUIDateTime_StrBuf) - 15, "%02ld", gUITimeMinute);

	return gUIDateTime_StrBuf;
}

//using by date-stamp, show time.second
char *DateTime_MakeYMDHMS(void)
{
	INT32 hr, ti;

	hr = gUITimeHour;
	ti = 0;
	if (gUITimeSetFmt == TIMEFMT_12HR) { //apply Time Format
		if (gUITimeHour < 12) {
			ti = 1;
		}
		if (gUITimeHour >= 12) {
			ti = 2;
		}
		hr = hr % 12;
		if (hr == 0) {
			hr = 12;
		}
	}

	if (gUIDateSetFmt == DATEFMT_YMD) { //YMD
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%04ld/%02ld/%02ld %02ld:%02ld:%02ld%s",
				 gUIDateYear, gUIDateMonth, gUIDateDay, hr, gUITimeMinute, gUITimeSecond, Time12HRNameTable[ti]);
	} else if (gUIDateSetFmt == DATEFMT_MDY) { //MDY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%04ld %02ld:%02ld:%02ld%s",
				 gUIDateMonth, gUIDateDay, gUIDateYear, hr, gUITimeMinute, gUITimeSecond, Time12HRNameTable[ti]);
	} else { //if(gUIDateSetFmt == DATEFMT_DMY) //DMY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%04ld %02ld:%02ld:%02ld%s",
				 gUIDateDay, gUIDateMonth, gUIDateYear, hr, gUITimeMinute, gUITimeSecond, Time12HRNameTable[ti]);
	}

	return gUIDateTime_StrBuf;
}


//using by playback, using short year
char *User_MakeYMDHM(UINT32 Year, UINT32 Month, UINT32 Day, UINT32 Hour, UINT32 Minute)
{
	INT32 hr, ti;

	//load format
	gUIDateSetFmt = UI_GetData(FL_DateFormatIndex);
	gUITimeSetFmt = UI_GetData(FL_TimeFormatIndex);

	hr = Hour;
	ti = 0;
	if (gUITimeSetFmt == TIMEFMT_12HR) { //apply Time Format
		if (Hour < 12) {
			ti = 1;
		}
		if (Hour >= 12) {
			ti = 2;
		}
		hr = hr % 12;
		if (hr == 0) {
			hr = 12;
		}
	}
	if (Year >= 2000) {
		Year -= 2000;
	}

	if (gUIDateSetFmt == DATEFMT_YMD) { //YMD
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%02ld %02ld:%02ld%s",
				 Year, Month, Day, hr, Minute, Time12HRNameTable[ti]);
	} else if (gUIDateSetFmt == DATEFMT_MDY) { //MDY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%02ld %02ld:%02ld%s",
				 Month, Day, Year, hr, Minute, Time12HRNameTable[ti]);
	} else { //if(gUIDateSetFmt == DATEFMT_DMY) //DMY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%02ld %02ld:%02ld%s",
				 Day, Month, Year, hr, Minute, Time12HRNameTable[ti]);
	}

	return gUIDateTime_StrBuf;
}


//using by playback, using short year, ignore AM/PM!
char *User_MakeYMDHM_short(UINT32 Year, UINT32 Month, UINT32 Day, UINT32 Hour, UINT32 Minute)
{
	INT32 hr;

	//load format
	gUIDateSetFmt = UI_GetData(FL_DateFormatIndex);
	gUITimeSetFmt = UI_GetData(FL_TimeFormatIndex);

	hr = Hour;
	if (gUITimeSetFmt == TIMEFMT_12HR) { //apply Time Format
		hr = hr % 12;
		if (hr == 0) {
			hr = 12;
		}
	}
	if (Year >= 2000) {
		Year -= 2000;
	}

	if (gUIDateSetFmt == DATEFMT_YMD) { //YMD
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%02ld %02ld:%02ld",
				 Year, Month, Day, hr, Minute);
	} else if (gUIDateSetFmt == DATEFMT_MDY) { //MDY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%02ld %02ld:%02ld",
				 Month, Day, Year, hr, Minute);
	} else { //if(gUIDateSetFmt == DATEFMT_DMY) //DMY
		snprintf((char *)gUIDateTime_StrBuf, sizeof(gUIDateTime_StrBuf) - 1, "%02ld/%02ld/%02ld %02ld:%02ld",
				 Day, Month, Year, hr, Minute);
	}

	return gUIDateTime_StrBuf;
}





//-----------------------------------------------------------------------------
// Home/Traval Time Zone
//-----------------------------------------------------------------------------

TIME_RECORD gTimeRecord[TIME_RECORD_CNT]; //index 0 is Home
static INT32 g_iTid = 0;

static INT32 TimeZoneMapping[24] =
{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void _TimeRecord_Load(INT32 iTid)
{
	TIME_RECORD *ti = &(gTimeRecord[iTid]);
	//load date-time
	ti->Curr_DateTime = HwClock_GetTime(TIME_ID_CURRENT);
}

void _TimeRecord_Save(INT32 iTid)
{
	TIME_RECORD *ti = &(gTimeRecord[iTid]);
	//save date-time
	HwClock_SetTime(TIME_ID_CURRENT, ti->Curr_DateTime, 0);
}

INT32 _TimeRecord_CalcTimeDiff(INT32 iNewTid, INT32 iOldTid)
{
	return (TimeZoneMapping[iNewTid] - TimeZoneMapping[iOldTid]);
}

void _TimeRecord_AddTimeDiff(INT32 iTid, INT32 TimeDiff)
{
	TIME_RECORD *ti = &(gTimeRecord[iTid]);
	struct tm cUIDateTime = HwClock_GetTime(TIME_ID_CURRENT);
	DbgMsg_UISetDate(("Old Date:Y=%ld,M=%ld,D=%ld\r\n", cUIDateTime.tm_year, cUIDateTime.tm_mon, cUIDateTime.tm_mday));
	DbgMsg_UISetDate(("Old Time:H=%ld,M=%ld\r\n", cUIDateTime.tm_hour, cUIDateTime.tm_min));

	DbgMsg_UISetDate(("TimeDiff=%ld\r\n", TimeDiff));
	if (cUIDateTime.tm_hour + TimeDiff < 0) {
		INT32 TmpDays;
		if (cUIDateTime.tm_year >= 2000) {
			TmpDays = TimeUtil_Value2Days(cUIDateTime);
			//DbgMsg_UISetDate(("Days=%ld\r\n",TmpDays));
			TmpDays--;
			//DbgMsg_UISetDate(("Days-1=%ld\r\n",TmpDays));
			ti->Curr_DateTime = TimeUtil_Days2Value(TmpDays);
			//DbgMsg_UISetDate(("Convert Time:H=%ld,M=%ld\r\n",ti->Curr_DateTime.tm_hour,ti->Curr_DateTime.tm_min));
			ti->Curr_DateTime.tm_hour = cUIDateTime.tm_hour + TimeDiff + 24;
			//DbgMsg_UISetDate(("New H = %ld +%ld + 24 = %ld\r\n",cUIDateTime.tm_hour, TimeDiff, ti->Curr_DateTime.tm_hour));
		} else {
			debug_err(("^RZone Calc Error!! Date can not before 2000/1/1\r\n"));
		}
	} else if (cUIDateTime.tm_hour + TimeDiff > 23) {
		INT32 TmpDays;
		if (cUIDateTime.tm_year < 2099) {
			TmpDays = TimeUtil_Value2Days(cUIDateTime);
			//DbgMsg_UISetDate(("Days=%ld\r\n",TmpDays));
			TmpDays++;
			//DbgMsg_UISetDate(("Days+1=%ld\r\n",TmpDays));
			ti->Curr_DateTime = TimeUtil_Days2Value(TmpDays);
			//DbgMsg_UISetDate(("Convert Time:H=%ld,M=%ld\r\n",ti->Curr_DateTime.tm_hour,ti->Curr_DateTime.tm_min));
			ti->Curr_DateTime.tm_hour = cUIDateTime.tm_hour + TimeDiff - 24;
			//DbgMsg_UISetDate(("New H = %ld +%ld - 24 = %ld\r\n",cUIDateTime.tm_hour, TimeDiff, ti->Curr_DateTime.tm_hour));
		} else {
			debug_err(("^RZone Calc Error!! Date can not after 2099/1/1\r\n"));
		}
	} else {
		ti->Curr_DateTime.tm_hour = cUIDateTime.tm_hour + TimeDiff;
		//DbgMsg_UISetDate(("New H = %ld +%ld = %ld\r\n",cUIDateTime.tm_hour, TimeDiff, ti->Curr_DateTime.tm_hour));
	}

	ti->Curr_DateTime.tm_min = cUIDateTime.tm_min;
	ti->Curr_DateTime.tm_sec = cUIDateTime.tm_sec;
	DbgMsg_UISetDate(("New Date:Y=%ld,M=%ld,D=%ld\r\n", ti->Curr_DateTime.tm_year, ti->Curr_DateTime.tm_mon, ti->Curr_DateTime.tm_mday));
	DbgMsg_UISetDate(("New Time:H=%ld,M=%ld\r\n", ti->Curr_DateTime.tm_hour, ti->Curr_DateTime.tm_min));
}

void TimeRecord_BeginChange(INT32 iOldTid)
{
	int i;
	for (i = 0; i < TIME_RECORD_CNT; i++) {
		gTimeRecord[i].new_TimeZone = gTimeRecord[i].TimeZone;
		_TimeRecord_Load(i);
	}
	g_iTid = iOldTid;
}

void TimeRecord_SetZone(INT32 iTid, INT32 v)
{
	gTimeRecord[iTid].TimeZone = v;
}

void TimeRecord_SetNewZone(INT32 iTid, INT32 v)
{
	gTimeRecord[iTid].new_TimeZone = v;
}

void TimeRecord_EndChange(INT32 iNewTid)
{
	INT32 TimeDiff = 0;
	INT32 iOldTid = g_iTid;
	if (iNewTid != iOldTid) {
		DbgMsg_UISetDate(("(OLD) LOCALE=%s, ZONE=%ld\r\n", (iOldTid == 0) ? "HOME" : "TRAVAL", gTimeRecord[iOldTid].TimeZone));
		DbgMsg_UISetDate(("(NEW) LOCALE=%s, ZONE=%ld\r\n", (iNewTid == 0) ? "HOME" : "TRAVAL", gTimeRecord[iNewTid].new_TimeZone));
		TimeDiff = _TimeRecord_CalcTimeDiff
				   (gTimeRecord[iNewTid].new_TimeZone, gTimeRecord[iOldTid].TimeZone);
	} else { //if equal tid
		DbgMsg_UISetDate(("(OLD) LOCALE=%s, ZONE=%ld\r\n", (iNewTid == 0) ? "HOME" : "TRAVAL", gTimeRecord[iNewTid].TimeZone));
		DbgMsg_UISetDate(("(NEW) LOCALE=%s, ZONE=%ld\r\n", (iNewTid == 0) ? "HOME" : "TRAVAL", gTimeRecord[iNewTid].new_TimeZone));
		TimeDiff = _TimeRecord_CalcTimeDiff
				   (gTimeRecord[iNewTid].new_TimeZone, gTimeRecord[iNewTid].TimeZone);
		if (iNewTid == 0) { //when chage Home Zone, but not change Locale
			TimeDiff = 0;    //the time date should be not change
		}
	}

	DbgMsg_UISetDate((">>> DIFF=%ld\r\n", TimeDiff));
	_TimeRecord_AddTimeDiff(iNewTid, TimeDiff);

	_TimeRecord_Save(iNewTid);
}



