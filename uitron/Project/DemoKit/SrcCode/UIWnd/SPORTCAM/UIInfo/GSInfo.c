////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "GSInfo.h"
#include "PStore.h"

//------------------------------------------------------------
typedef struct {
	UINT32  GSCalData[8];
	UINT32  uiVerifyCode;
} GSCALI_INFO;

#define PS_GS_CALI              "GSCA" //MAKE_PS_SECNAME_ID('G', 'S', 'C', 'A')
#define  GSCALI_VERIFICATION    0x47534341 //"GSCA"

GSCALI_INFO     GSCaliInfo;
BOOL bGSReady = 0;

#if (GSENSOR_FUNCTION == ENABLE)
void Save_GSInfo(void)
{
	PPSTORE_SECTION_HANDLE  pSection;

	debug_msg("[Init_MenuInfo][PStore write GSCali]\r\n");
	//save data to PStore
	if ((pSection = PStore_OpenSection(PS_GS_CALI, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
		GSCaliInfo.uiVerifyCode = GSCALI_VERIFICATION;
		PStore_WriteSection((UINT8 *)&GSCaliInfo, 0, sizeof(GSCaliInfo), pSection);
		PStore_CloseSection(pSection);
	} else {
		debug_err(("[TPCalibrate][PStore Write GS calibration param fail]\r\n"));
	}
}

void Load_GSInfo(void)
{
	PPSTORE_SECTION_HANDLE  pSection;

	debug_msg("[Init_MenuInfo][PStore read GSCali]\r\n");
	memset((UINT8 *)&GSCaliInfo, 0, sizeof(GSCaliInfo));

	if ((pSection = PStore_OpenSection(PS_GS_CALI, PS_RDWR)) != E_PS_SECHDLER) {
		PStore_ReadSection((UINT8 *)&GSCaliInfo, 0, sizeof(GSCaliInfo), pSection);
		PStore_CloseSection(pSection);
		bGSReady = 1;
	} else {
		debug_err(("^R Pstore of G Sensor open to read FAIL\r\n"));
	}

}

void Init_GSInfo(void)
{
	debug_msg("[Init_MenuInfo][PStore update GSCali]\r\n");
	if (bGSReady) {
		if (GSCaliInfo.uiVerifyCode == GSCALI_VERIFICATION) {
			//GSSensor_ReadCalData(GSCaliInfo.GSCalData);
		} else {
			debug_err(("^R Calbration data of G Sensor is invalid, Using default value\r\n"));
		}
	}
}

void Update_GSInfo(UINT32 *pGSCalData)
{
	int i;
	for (i = 0; i < 8; i++) {
		GSCaliInfo.GSCalData[i] = pGSCalData[i];
	}
}


#endif

