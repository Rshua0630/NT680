#ifndef _DATETIMEINFO_H_
#define _DATETIMEINFO_H_

#include "GxSystem.h"
#include "HwClock.h"

//-----------------------------------------------------------------------------
// Date Time
//-----------------------------------------------------------------------------
#define DATETIME_YEAR       0
#define DATETIME_MONTH      1
#define DATETIME_DAY        2
#define DATETIME_HOUR       3
#define DATETIME_MINUTE     4
#define DATETIME_SECOND     5
#define DATETIME_DATEFMT    6
#define DATETIME_TIMEFMT    7

#define DATEFMT_YMD         0
#define DATEFMT_MDY         1
#define DATEFMT_DMY         2

#define TIMEFMT_24HR        0
#define TIMEFMT_12HR        1

//DateTime Info
extern void     DateTime_Init(void);
extern void     DateTime_Load(void); //Load from RTC
extern void     DateTime_Save(void); //Save to RTC
extern void     DateTime_SaveToDefault(void); //Save to RTC
extern INT32    DateTime_Get(int element); //Get current element
extern void     DateTime_Edit(int element, int bias); //Edit current element (auto validate)

extern char    *DateTime_MakeYMD(void);
extern char    *DateTime_MakeYMDHM(void);
extern char    *DateTime_MakeYMDHM_Edit(void);
extern char    *DateTime_MakeYMDHMS(void);
extern char    *User_MakeYMDHM(UINT32 Year, UINT32 Month, UINT32 Day, UINT32 Hour, UINT32 Minute);
extern char    *User_MakeYMDHM_short(UINT32 Year, UINT32 Month, UINT32 Day, UINT32 Hour, UINT32 Minute);

//-----------------------------------------------------------------------------
// World Time Zone
//-----------------------------------------------------------------------------

typedef struct _TIME_RECORD {
	UINT32 TimeZone;
	UINT32 new_TimeZone;
	struct tm Curr_DateTime;
}
TIME_RECORD;

#define TIME_RECORD_CNT     2

UINT32 Get_ActiveTimeRecord(void);
void Set_ActiveTimeRecord(INT32 iTid);

void TimeRecord_BeginChange(INT32 iOldTid);
void TimeRecord_SetZone(INT32 iTid, INT32 v);
void TimeRecord_SetNewZone(INT32 iTid, INT32 v);
void TimeRecord_EndChange(INT32 iNewTid);



#endif //_DATETIMEINFO_H_
