#include "GSensor.h"

#define __MODULE__          GSensor_None
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"


BOOL GSensor_open(void)
{
	DBG_IND("\r\n");
	return TRUE;
}

BOOL GSensor_close(void)
{
	return TRUE;
}



BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
	DBG_IND("\r\n");
	return FALSE;
}

BOOL GSensor_ParkingMode(void)
{
	return FALSE;
}

BOOL GSensor_CrashMode(void)
{
	return FALSE;
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{

}

void GSensor_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
}

