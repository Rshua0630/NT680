#ifndef MOVIE_MAPPING_H
#define MOVIE_MAPPING_H


typedef enum _MOVIE_REC_TYPE {
	MOVIE_REC_TYPE_UNKNOWN,
	MOVIE_REC_TYPE_FRONT,
	MOVIE_REC_TYPE_BEHIND,
	MOVIE_REC_TYPE_DUAL,
	MOVIE_REC_TYPE_CLONE,
	MOVIE_REC_TYPE_TRI,
	MOVIE_REC_TYPE_QUAD,
} MOVIE_REC_TYPE;


extern UINT32   GetMovieSizeWidth(UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight(UINT32 uiIndex);
extern UINT32   GetMovieFrameRate(UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate(UINT32 uiSize, UINT32 uiQuality);
extern void     SetMovieTargetBitrate(UINT32 uiType, UINT32 uiSize, UINT32 uiBitRate);
extern void     SetMovieTargetFrameRate(UINT32 uiType, UINT32 uiIndex, UINT32 uiFramRate);
#if (UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE)
extern void     SetMovieLiveViewFrameRate(UINT32 uiType, UINT32 uiIndex, UINT32 uiFrameRate);
#endif
extern UINT32   GetMovieDispAspectRatio(UINT32 uiIndex);
extern UINT32   GetMovieSizeRatio(UINT32 ubIndex);
extern UINT32   GetMovieAudioVolumeValue(UINT32 uiIndex);
extern UINT32   Get_MovieTimeLapseValue(UINT32 uhIndex);
//#NT#2015/03/18#Lily Kao -begin
//#NT#Add for UVAC to set parameters for MFK
extern UINT32   GetMovieTBRAspRatio(UINT32 width, UINT32 height, UINT32 fps, UINT32 *pTBR, UINT32 *pAspRatio);
//#NT#2015/03/18#Lily Kao -end

extern UINT32   GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex);
extern MOVIE_REC_TYPE GetMovieRecType_2p(UINT32 uiIndex);
extern UINT32   GetMovieVidEncFmt_2p(UINT32 uiPath);
extern UINT32   CheckWiFiMapTbl(void);
extern void GetMovieAqInfo(UINT32 uiIndex, UINT32 Value);
extern void GetMovieCbrInfo(UINT32 uiIndex, UINT32 Value);
extern void GetMovieAqInfo_2p(UINT32 uiIndex, UINT32 Value);
extern void GetMovieCbrInfo_2p(UINT32 uiIndex, UINT32 Value);
//#NT#2018/04/02#IsiahChang -begin
//#NT#To support Wi-Fi command 2025 BRC.
extern void GetMovieStreamInfo(UINT32 uiIndex, UINT32 Value);
//#NT#2018/04/02#IsiahChang -end

#if MOVIE_MAPPING_MULTIREC
// main movie
extern UINT32 MovieMapping_GetRecType(UINT32 uiSizeIdx);
extern UINT32 MovieMapping_GetWidth(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetHeight(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetFrameRate(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetTargetBitrate(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetDispAspectRatio(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetImageRatio(UINT32 uiSizeIdx, UINT32 uiIplId);
extern void MovieMapping_GetAqInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value);
extern void MovieMapping_GetCbrInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value);
// clone movie
extern UINT32 MovieMapping_GetCloneWidth(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetCloneHeight(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetCloneFrameRate(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetCloneTargetBitrate(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetCloneDispAspectRatio(UINT32 uiSizeIdx, UINT32 uiIplId);
extern UINT32 MovieMapping_GetCloneImageRatio(UINT32 uiSizeIdx, UINT32 uiIplId);
extern void MovieMapping_GetCloneAqInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value);
extern void MovieMapping_GetCloneCbrInfo(UINT32 uiSizeIdx, UINT32 uiIplId, UINT32 Value);
// WiFi/UVC streaming
extern void MovieMapping_GetStreamInfo(UINT32 uiSizeIdx, UINT32 Value);
#endif
#endif
