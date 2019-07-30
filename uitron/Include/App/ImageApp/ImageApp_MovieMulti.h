/*
    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.

    @file       ImageApp_MovieMulti.h
    @ingroup    mIImageApp

    @note       This header file is updated by ImageApp_MovieMulti 0.00.000

    @date       2018/05/15
*/

#ifndef IMAGEAPP_MOVIEMULTI_H
#define IMAGEAPP_MOVIEMULTI_H


#include "ImageStream.h"
#include "ImageApp_MovieCommon.h"
#include "ipl_alg_infor.h"
#include "ImageUnit_BsMux.h"

/**
    @addtogroup mIImageApp
*/
//@{

enum {
	MOVIEMULTI_PARAM_START = 0x0000ED000,
	// ipl group
	MOVIEMULTI_PARAM_IPL_GROUP_BEGIN = MOVIEMULTI_PARAM_START,
	MOVIEMULTI_PARAM_IPL_INFO = MOVIEMULTI_PARAM_IPL_GROUP_BEGIN,                   /// imagepipe
	MOVIEMULTI_PARAM_IPL_GROUP_END,
	// image group
	MOVIEMULTI_PARAM_IMG_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0100,
	MOVIEMULTI_PARAM_SIZE = MOVIEMULTI_PARAM_IMG_GROUP_BEGIN,                       /// vdoenc + bsmux
	MOVIEMULTI_PARAM_FRAMERATE,                                                     /// vdoenc + bsmux
	MOVIEMULTI_PARAM_DAR,                                                           /// vdoenc + bsmux
	MOVIEMULTI_PARAM_IMG_GROUP_END,
	// video codec group
	MOVIEMULTI_PARAM_CODEC_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0200,
	MOVIEMULTI_PARAM_VDO_ENCODER_OBJ = MOVIEMULTI_PARAM_CODEC_GROUP_BEGIN,          /// vdoenc
	MOVIEMULTI_PARAM_CODEC,                                                         /// vdoenc + bsmux
	MOVIEMULTI_PARAM_PROFILE,                                                       /// vdoenc
	MOVIEMULTI_PARAM_TARGETRATE,                                                    /// vdoenc + bsmux
	MOVIEMULTI_PARAM_INITQP,                                                        /// vdoenc
	MOVIEMULTI_PARAM_MINQP,                                                         /// vdoenc
	MOVIEMULTI_PARAM_MAXQP,                                                         /// vdoenc
	MOVIEMULTI_PARAM_AQ_INFO,                                                       /// vdoenc
	MOVIEMULTI_PARAM_CBR_INFO,                                                      /// vdoenc
	MOVIEMULTI_PARAM_ENCBUF_MS,                                                     /// vdoenc
	MOVIEMULTI_PARAM_ENCBUF_RESERVED_MS,                                            /// vdoenc
	MOVIEMULTI_PARAM_TIMELAPSE_TIME,                                                /// vdoenc
	MOVIEMULTI_PARAM_SKIP_FRAME,                                                    /// vdoenc
	MOVIEMULTI_PARAM_CODEC_GROUP_END,
	// imgcap group
	MOVIEMULTI_PARAM_IMGCAP_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0300,
	MOVIEMULTI_PARAM_IMGCAP_MAXSIZE = MOVIEMULTI_PARAM_IMGCAP_GROUP_BEGIN,          /// var
	MOVIEMULTI_PARAM_IMGCAP_SIZE,                                                /// var
	MOVIEMULTI_PARAM_IMGCAP_GROUP_END,
	// audio codec group
	MOVIEMULTI_PARAM_AUD_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0400,
	MOVIEMULTI_PARAM_AUD_ENCODER_OBJ = MOVIEMULTI_PARAM_AUD_GROUP_BEGIN,            /// audenc
	MOVIEMULTI_PARAM_AUD_SAMPLERATE,                                                /// audenc
	MOVIEMULTI_PARAM_AUD_SAMPLERATE_FILEINFO,                                       /// bsmux
	MOVIEMULTI_PARAM_AUD_CHS,                                                       /// audin + audenc
	MOVIEMULTI_PARAM_AUD_CHS_FILEINFO,                                              /// bsmux
	MOVIEMULTI_PARAM_AUD_CODEC,                                                     /// audenc
	MOVIEMULTI_PARAM_AUD_CODEC_FILEINFO,                                            /// bsmux
	MOVIEMULTI_PARAM_AUD_ADTS_HEADER,                                               /// audenc
	MOVIEMULTI_PARAM_AUD_FILETYPE,                                                  /// audenc
	MOVIEMULTI_PARAM_AUD_GROUP_END,
	// file group
	MOVIEMULTI_PARAM_FILE_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0500,
	MOVIEMULTI_PARAM_FILE_FORMAT = MOVIEMULTI_PARAM_FILE_GROUP_BEGIN,               /// audenc + vdoenc + bsmux
	MOVIEMULTI_PARAM_REC_FORMAT,                                                    /// vdoenc + bsmux
	MOVIEMULTI_PARAM_FILE_STRGID,                                                   /// bsmux
	MOVIEMULTI_PARAM_FILE_SEAMLESSSEC,                                              /// vdoenc + bsmux
	MOVIEMULTI_PARAM_FILE_ROLLBACKSEC,                                              /// bsmux
	MOVIEMULTI_PARAM_FILE_KEEPSEC,                                                  /// bsmux
	MOVIEMULTI_PARAM_FILE_ENDTYPE,                                                  /// bsmux
	MOVIEMULTI_PARAM_FILE_OVERLAP,                                                  /// bsmux
	MOVIEMULTI_PARAM_MAX_FILE,                                                      /// bsmux
	MOVIEMULTI_PARAM_FILE_EMRON,                                                    /// bsmux
	MOVIEMULTI_PARAM_FILE_PLAYFRAMERATE,                                            /// bsmux
	MOVIEMULTI_PARAM_FILE_BUFRESSEC,                                                /// bsmux
	MOVIEMULTI_PARAM_PRECALC_BUFFER,                                                /// bsmux
	MOVIEMULTI_PARAM_GPS_DATA,                                                      /// bsmux
	MOVIEMULTI_PARAM_FILE_TRIGEMR,                                                  /// bsmux
	MOVIEMULTI_PARAM_FILE_GROUP_END,
	// display group
	MOVIEMULTI_PARAM_DISP_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0600,
	MOVIEMULTI_PARAM_DISP_REG_CB = MOVIEMULTI_PARAM_DISP_GROUP_BEGIN,
	MOVIEMULTI_PARAM_DISP_DIR,
	MOVIEMULTI_PARAM_DISP_GROUP_END,
	// wifi group
	MOVIEMULTI_PARAM_WIFI_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0700,
	MOVIEMULTI_PARAM_WIFI_REG_CB = MOVIEMULTI_PARAM_WIFI_GROUP_BEGIN,
	MOVIEMULTI_PARAM_WIFI_GROUP_END,
	// misc group
	MOVIEMULTI_PARAM_MISC_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0900,
	MOVIEMULTI_PARAM_USER_CB = MOVIEMULTI_PARAM_MISC_GROUP_BEGIN,                   /// var
	MOVIEMULTI_PARAM_MISC_GROUP_END,
/*
	MOVIE_PARAM_YUV_COMPRESS,
	MOVIE_PARAM_3DNRLEVEL,
	MOVIE_PARAM_3DNR_CB,
	MOVIE_PARAM_IMM_CB,

	MOVIE_PARAM_PIPVIEW_PUSH_CB,
	MOVIE_PARAM_PIPVIEW_PROCESS_CB,

	MOVIE_PARAM_DUAL_DISP_EN,
	MOVIE_PARAM_DUAL_DISP_IPL_SIZE,
	MOVIE_PARAM_DUAL_DISP_AR,
	MOVIE_PARAM_MULTI_TEMPORARY_LAYER,
	MOVIE_PARAM_TV_RANGE,
	MOVIE_PARAM_IMGCAP_JPG_BUFNUM,
*/
};

typedef struct {
	IPL_SEL_ITEM   item;
	UINT32  value;
} MOVIEMULTI_RECODE_IPL_INFO;

typedef struct {
	UINT32 aud_rate;
	UINT32 aud_ch;
} MOVIEMULTI_AUDIO_INFO;

typedef NMEDIAREC_CALCSEC_SETTING2 MOVIEMULTI_CALCSEC_SETTING2;

typedef void (*MOVIE_DISP_PIP_PROCESS_CB)(void);
typedef void (*MOVIE_WIFI_PIP_PROCESS_CB)(void);

typedef struct {
	MOVIE_CFG_REC_ID        rec_id;
	CHAR*   movie;
	CHAR*   emr;
	CHAR*   snapshot;
	BOOL   file_naming;
} MOVIEMULTI_RECODE_FOLDER_NAMING;

extern void ImageApp_MovieMulti_InstallID(void);

extern ISF_RV ImageApp_MovieMulti_Open(void);
extern ISF_RV ImageApp_MovieMulti_Close(void);

extern ISF_RV ImageApp_MovieMulti_Config(UINT32 config_id, UINT32 value);

extern ISF_RV ImageApp_MovieMulti_FileOption(MOVIE_RECODE_FILE_OPTION *prec_option);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForDisp(MOVIE_CFG_REC_ID id, UINT32 action, BOOL allow_pull);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForStreaming(MOVIE_CFG_REC_ID id, UINT32 action, BOOL allow_pull);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForAlg(MOVIE_CFG_REC_ID id, MOVIE_CFG_ALG_PATH path, UINT32 action, BOOL allow_pull);

extern ISF_RV ImageApp_MovieMulti_RecStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_RecStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_TriggerSnapshot(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_SetParam(MOVIE_CFG_REC_ID id, UINT32 param, UINT32 value);

extern ISF_RV ImageApp_MovieMulti_StreamingStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_StreamingStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_DispStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_DispStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_UvacStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_UvacStop(MOVIE_CFG_REC_ID id);

extern UINT32 ImageApp_MovieMulti_GetDispDataPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetStreamingDataPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetAlgDataPort(MOVIE_CFG_REC_ID id, MOVIE_CFG_ALG_PATH path);
extern UINT32 ImageApp_MovieMulti_GetVdoEncPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetDispImgTransPort(MOVIE_CFG_REC_ID id);

extern UINT32 ImageApp_MovieMulti_IsStreamRunning(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetFreeRec(void);
extern UINT32 ImageApp_MovieMulti_GetFreeRec2(MOVIEMULTI_CALCSEC_SETTING2 *pSetting);
extern UINT32 ImageApp_MovieMulti_GetRemainSize(CHAR drive, UINT64 *p_size);
extern UINT32 ImageApp_MovieMulti_SetReservedSize(CHAR drive, UINT64 size, UINT32 is_loop);

extern INT32 ImageApp_MovieMulti_FMOpen(CHAR drive);
extern INT32 ImageApp_MovieMulti_FMClose(CHAR drive);
extern INT32 ImageApp_MovieMulti_FMConfig(MOVIE_CFG_FDB_INFO *p_cfg);
/**
    set crash the assigned rec id to backup files

    set crash the assigned rec id to backup files

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_MovieMulti_SetCrash(MOVIE_CFG_REC_ID rec_id, UINT32 is_set);

extern void ImageApp_MovieMulti_Root_Path(CHAR * prootpath, UINT32 rec_id);
extern ISF_RV ImageApp_MovieMulti_Folder_Naming(MOVIEMULTI_RECODE_FOLDER_NAMING *pfolder_naming);
extern BOOL ImageApp_MovieMulti_isFileNaming(MOVIE_CFG_REC_ID rec_id);
extern ISF_RV ImageApp_MovieMulti_RegUserCB(MovieUserEventCb *fncb);

//@}
#endif//IMAGEAPP_MOVIEMULTI_H

