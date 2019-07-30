/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg_Default.h
    @ingroup

    @brief

    @note       This config genernate default config where isn't set in PrjCfg_MODEL.h
*/

#ifndef _PRJCFG_DEFAULT_H_
#define _PRJCFG_DEFAULT_H_

#ifndef GPROF_FUNC
#define GPROF_FUNC                      DISABLE
#endif

#ifndef NVT_DEPTH_FUNC
#define NVT_DEPTH_FUNC                  DISABLE
#endif

#ifndef NVT_DEPTH_VIEW
#define NVT_DEPTH_VIEW                  DISABLE
#endif

#ifndef DEMO_SENSOR6_CCIR
#define DEMO_SENSOR6_CCIR 				DISABLE
#endif

#ifndef PST_VER
#define PST_VER				1
#endif

#ifndef IPCAM_AUDIO_IN_FUNC
#define IPCAM_AUDIO_IN_FUNC                     DISABLE
#endif

#ifndef IPCAM_AUDIO_OUT_FUNC
#define IPCAM_AUDIO_OUT_FUNC                     DISABLE
#endif

#ifndef IPCAM_AEC_FUNC
#define IPCAM_AEC_FUNC  				DISABLE
#endif

#ifndef IPCAM_ANR_FUNC
#define IPCAM_ANR_FUNC  				DISABLE
#endif

#ifndef IPCAM_AUDFILT_FUNC
#define IPCAM_AUDFILT_FUNC              DISABLE
#endif

#ifndef AUDIO_FUNC_PCM
#define AUDIO_FUNC_PCM               	ENABLE
#endif

#ifndef AUDIO_FUNC_AAC
#define AUDIO_FUNC_AAC                  ENABLE
#endif

#ifndef AUDIO_FUNC_PPCM
#define AUDIO_FUNC_PPCM               	DISABLE
#endif

#ifndef AUDIO_FUNC_ULAW
#define AUDIO_FUNC_ULAW                 DISABLE
#endif

#ifndef AUDIO_FUNC_ALAW
#define AUDIO_FUNC_ALAW                 DISABLE
#endif

#ifndef USB_OTG_FUNC
#define USB_OTG_FUNC		DISABLE
#endif

#ifndef IPCAM_CDNN_FUNC
#define IPCAM_CDNN_FUNC                 DISABLE
#define IPCAM_CDNN2_FUNC                DISABLE
#endif

#ifndef IPCAM_FD_DRAW_VIDEO
#define IPCAM_FD_DRAW_VIDEO                     DISABLE
#endif

#ifndef IPCAM_OD_FUNC
#define IPCAM_OD_FUNC                     DISABLE
#endif

#ifndef IPCAM_OD_DRAW_VIDEO
#define IPCAM_OD_DRAW_VIDEO                     DISABLE
#endif

#ifndef TV_POWER_SAVE_MODE
#define TV_POWER_SAVE_MODE  			DISABLE
#endif

#ifndef IPCAM_WDR_FUNC
#define IPCAM_WDR_FUNC  			DISABLE
#endif

#ifndef MOVIE_STRM_CODEC_3DNR_ENABLE
#define MOVIE_STRM_CODEC_3DNR_ENABLE		DISABLE
#endif

#ifndef MOVIE_STRM_PATH_USERPROC
#define MOVIE_STRM_PATH_USERPROC		DISABLE
#endif

#ifndef IPCAM_BC_FUNC
#define IPCAM_BC_FUNC		DISABLE
#endif

#ifndef IPCAM_USER_PULL_DATA
#define IPCAM_USER_PULL_DATA		DISABLE
#endif

#ifndef VIDEOSPRITE_MOSAIC
#define VIDEOSPRITE_MOSAIC              DISABLE
#endif

#ifndef VIDEOSPRITE_STAMP_FUNC
#define VIDEOSPRITE_STAMP_FUNC 			DISABLE
#endif

#ifndef MOVIE_FDCNN_FUNC
#define MOVIE_FDCNN_FUNC                DISABLE
#endif

#ifndef MOVIE_FDCNN_DRAW_VIDEO
#define MOVIE_FDCNN_DRAW_VIDEO          DISABLE
#endif

#ifndef MOVIE_FDCNN_USE_ODT
#define MOVIE_FDCNN_USE_ODT             DISABLE
#endif


#ifndef IPCAM_DEPTH_FUNC
#define IPCAM_DEPTH_FUNC		DISABLE
#endif

#ifndef MOVIE_BC_MD
#define MOVIE_BC_MD	                    ENABLE
#endif

#ifndef VDOENC_MULTI_TEMPORARY_LAYER
#define VDOENC_MULTI_TEMPORARY_LAYER 	ENABLE
#endif

#ifndef YUV_TV_RANGE
#define YUV_TV_RANGE 					DISABLE
#endif

#ifndef VIDEO_FUNC_MJPG
#define VIDEO_FUNC_MJPG               	ENABLE
#endif

#ifndef VIDEO_FUNC_H264
#define VIDEO_FUNC_H264               	ENABLE
#endif

#ifndef VIDEO_FUNC_H265
#define VIDEO_FUNC_H265                 ENABLE
#endif

//#NT#2018/03/12#Dan Guo -begin
//#NT# Face recognition
#ifndef MOVIE_FR_FUNC
#define MOVIE_FR_FUNC                   DISABLE
#endif

#ifndef MOVIE_FR_DRAW_VIDEO
#define MOVIE_FR_DRAW_VIDEO             DISABLE
#endif
//#NT#2018/03/12#Dan Guo -end

#ifndef STAMP_PRIVATE_POOL
#define STAMP_PRIVATE_POOL				DISABLE
#endif

#ifndef IPCAM_LPR_FUNC
#define IPCAM_LPR_FUNC                  DISABLE
#endif

#ifndef SENSOR_GP_FUNC
#define SENSOR_GP_FUNC                  DISABLE
#endif

#ifndef IPCAM_DEFOG_FUNC
#define IPCAM_DEFOG_FUNC                  DISABLE
#endif

#ifndef IPCAM_SHDR_FUNC
#define IPCAM_SHDR_FUNC                  DISABLE
#endif

#ifndef IPCAM_SQUARE_3DNR
#define IPCAM_SQUARE_3DNR                  DISABLE
#endif

#ifndef IPCAM_LCE
#define IPCAM_LCE                  DISABLE
#endif

#ifndef IPCAMPB_MODE
#define IPCAMPB_MODE                  DISABLE
#endif

#ifndef IPCAM_RAW_DATA
#define IPCAM_RAW_DATA                  DISABLE
#endif

#ifndef DEWARP_FUNC
#define DEWARP_FUNC            		    DISABLE
#endif

#ifndef MOVIE_MAPPING_MULTIREC
#if (IPCAM_FUNC == ENABLE)
#define MOVIE_MAPPING_MULTIREC			DISABLE
#else
#define MOVIE_MAPPING_MULTIREC			ENABLE
#endif
#endif

#ifndef STOP_REC_BK
#define STOP_REC_BK  DISABLE
#endif

#ifndef UVAC_VID_DISPLAY
#define UVAC_VID_DISPLAY                DISABLE
#endif

#ifndef NMEDIAPLAY_FUNC
#define NMEDIAPLAY_FUNC                 DISABLE
#endif

#ifndef FS_SWITCH_STRG_FUNC
#define FS_SWITCH_STRG_FUNC             DISABLE
#endif

#ifndef USE_IPC_RESERVED_MEM
#define USE_IPC_RESERVED_MEM            DISABLE
#endif

#ifndef IPCAM_STEAMSENDER_A_SLOT
#define IPCAM_STEAMSENDER_A_SLOT				30
#endif

#ifndef IPCAM_STEAMSENDER_V_SLOT
#define IPCAM_STEAMSENDER_V_SLOT				30
#endif


#ifndef IPCAM_STEAMSENDER_AUTO_REL
#define IPCAM_STEAMSENDER_AUTO_REL				1
#endif

#if (FS_SWITCH_STRG_FUNC && FS_MULTI_STRG_FUNC)
#error "FS_SWITCH_STRG_FUNC and FS_MULTI_STRG_FUNC cannot be both enabled."
#endif
#if (FS_SWITCH_STRG_FUNC && SDHOTPLUG_FUNCTION)
#error "FS_SWITCH_STRG_FUNC and SDHOTPLUG_FUNCTION cannot be both enabled."
#endif
/**
================================================================================
The following is for checking rule.
================================================================================
*/

#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
#if FS_MULTI_STRG_FUNC == DISABLE
"FS_MULTI_STRG_FUNC should be ENABLE while EMBMEM = EMBMEM_EMMC"
#endif
#endif

#endif //_PRJCFG_DEFAULT_H_
