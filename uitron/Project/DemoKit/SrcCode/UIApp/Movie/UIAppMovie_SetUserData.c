////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppMovie.h"
#include "MovieUdtaVendor.h"
#if _TODO
#include "IQS_Utility.h"
#endif
#include "ImageApp_Movie.h"
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
#include "ImageApp_MovieMulti.h"
#endif
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern GPSDATA    gpsdata;

void Movie_SetUserData(void)
{
#if _TODO
	UINT32  uiUserDataAddr, uiUserDataSize = 0;

	// Disable end-char 'A' of file name.
	MovRec_EnableEndChar4SingleRec(FALSE);
	MovRec_EnableEndChar4MultiRec(TRUE);

	//#NT#2013/05/15#Calvin Chang#Add customer's user data to MOV/MP4 file format -begin
	if (MovieUdta_MakeVendorUserData(&uiUserDataAddr, &uiUserDataSize)) {
		// Enable custom user data
		MovRec_ChangeParameter(MOVREC_RECPARAM_EN_CUSTOMUDATA, TRUE, 0, 0);
		// Set user data
		MovRec_SetUserData((UINT32)uiUserDataAddr, uiUserDataSize);
	}
	//#NT#2013/05/15#Calvin Chang -end
#endif

	MEM_RANGE log_buf;
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	log_buf.Size = mp4log_GetTotalLogSize() * 1024;
	log_buf.Size = log_buf.Size ? (log_buf.Size - 16) : (1024 - 16);
	if (mp4log_ToggleBuf(&(log_buf.Addr), NULL) == E_OK) {
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		// since all file share same gps data, only config once
		ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_GPS_DATA, (UINT32)&log_buf);
#else
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_GPS_DATA, (UINT32)&log_buf);
#endif
	} else {
		DBG_ERR("No GPS buffer!!!");
	}
#else   // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	log_buf.Addr = &gpsdata;
	log_buf.Size = sizeof(GPSDATA);
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	// since all file share same gps data, only config once
	ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_GPS_DATA, (UINT32)&log_buf);
#else
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_GPS_DATA, (UINT32)&log_buf);
#endif
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

