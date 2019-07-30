//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCfg.h"
#include "SysCommon.h"

#include "AppLib.h"

#if (UI_FUNC == ENABLE)
#include "UIInfo.h"
#include "UISetup.h"
#endif

#include "UIFramework.h"
#include "NVTUserCommand.h"

#if (UI_FUNC == ENABLE)
#include "UIAppMovie.h"  // Isiah, implement YUV merge mode of recording func.
#include "UIAppMovie_Param.h"
#include "UIMovieMapping.h"

#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "UIPhotoMapping.h"
#endif

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysVdoCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern DX_HANDLE gDevLCDObj;
#if (TV_FUNC == ENABLE)
extern DX_HANDLE gDevTVObj;
#endif
#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif

static BOOL Cmd_sys_disp(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1)); //fix for CID 44997

	if (strncmp(str1, "off", 31) == 0) {
		DBG_DUMP("(OFF)!\r\n");
		//TBD
	}
	
#if (LCD1_FUNC == ENABLE)
	if (strncmp(str1, "lcd", 31) == 0) {
#if (DISPLAY_FAST_SWITCH == DISABLE)
		DBG_DUMP("LCD!\r\n");
#if (UI_FUNC == ENABLE)
		Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0); //disable dual display
#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
		SwTimer_DelayMs(500); //wait for insert/remove detect
		SwTimer_DelayMs(500); //wait for insert/remove detect
#else
		UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
		DBG_DUMP("LCD!\r\n");
		Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0); //disable dual display
		if (GxVideo_GetDevice(DOUT1) != device_def) {
			Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
		}
#if (DISPLAY2_FUNC == ENABLE)
		if (GxVideo_GetDevice(DOUT2) != 0) {
			Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
		}
#endif
#endif
	}
#endif

#if (TV_FUNC == ENABLE)
	if (strncmp(str1, "tv", 31) == 0) {
#if (DISPLAY_FAST_SWITCH == DISABLE)
		DBG_DUMP("TV!\r\n");
#if (UI_FUNC == ENABLE)
		Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0); //disable dual display
#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
		SwTimer_DelayMs(500); //wait for insert/remove detect
		SxCmd_DoCommand("dx tv vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
#else
		UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
		UINT32 device = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT);
		DBG_DUMP("TV!\r\n");
#if (UI_FUNC == ENABLE)
		Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0); //disable dual display
#endif
		if (GxVideo_GetDevice(DOUT1) != device) {
			if (GxVideo_GetDevice(DOUT1) != device_def) {
				Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
			}
			Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, device);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if (GxVideo_GetDevice(DOUT2) != 0) {
			Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
		}
#endif
#endif
	}
#endif

#if (HDMI_FUNC == ENABLE)
	if (strncmp(str1, "hd", 31) == 0) {
	#if (DISPLAY_FAST_SWITCH == DISABLE)
	    DBG_DUMP("HDMI!\r\n");
	#if (UI_FUNC == ENABLE)
	    Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0); //disable dual display
	#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
		SwTimer_DelayMs(500); //wait for insert/remove detect
		SxCmd_DoCommand("dx hdmi vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
#else
		UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
		UINT32 device = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT);
		DBG_DUMP("HDMI!\r\n");
#if (UI_FUNC == ENABLE)
		Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0); //disable dual display
#endif
		if (GxVideo_GetDevice(DOUT1) != device) {
			if (GxVideo_GetDevice(DOUT1) != device_def) {
				Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
			}
			Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, device);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if (GxVideo_GetDevice(DOUT2) != 0) {
			Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
		}
#endif
	#endif
	}
#endif

#if (LCD2_FUNC == ENABLE)
	if (strncmp(str1, "lcd2", 31) == 0) {
	#if (DISPLAY_FAST_SWITCH == DISABLE)
	    DBG_DUMP("LCD2!\r\n");
	#if (UI_FUNC == ENABLE)
	    Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1); //enable dual display
	#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
	    SwTimer_DelayMs(500); //wait for insert/remove detect
	    SxCmd_DoCommand("dx lcd2 vdet 1");
	    SwTimer_DelayMs(500); //wait for insert/remove detect
	#else
	    UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
	    UINT32 device2 = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD2);
	    DBG_DUMP("LCD2!\r\n");
	#if (UI_FUNC == ENABLE)
	    Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1); //enable dual display
	#endif
	    if (GxVideo_GetDevice(DOUT1) != device_def) {
	        Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
	    }
#if (DISPLAY2_FUNC == ENABLE)
		if (GxVideo_GetDevice(DOUT2) != device2) {
			if (GxVideo_GetDevice(DOUT2) != 0) {
				Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
			}
			Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, device2);
		}
#endif
	#endif
	}
#endif

#if (DISPLAY2_FUNC == ENABLE)
#if (TV_FUNC == ENABLE)
	if (strncmp(str1, "lcdtv", 31) == 0) {
	    DBG_DUMP("LCD+TV!\r\n");
	#if (UI_FUNC == ENABLE)
	    Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1); //enable dual display
	#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
		SwTimer_DelayMs(500); //wait for insert/remove detect
		SxCmd_DoCommand("dx tv vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if (strncmp(str1, "lcdhd", 31) == 0) {
	    DBG_DUMP("LCD+HDMI!\r\n");
	#if (UI_FUNC == ENABLE)
	    Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1); //enable dual display
	#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
		SwTimer_DelayMs(500); //wait for insert/remove detect
		SxCmd_DoCommand("dx hdmi vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
#endif
#if (LCD2_FUNC == ENABLE)
	if (strncmp(str1, "lcdlcd2", 31) == 0) {
	    DBG_DUMP("LCD+LCD2!\r\n");
	#if (UI_FUNC == ENABLE)
	    Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1); //enable dual display
	#endif
	#if (TV_FUNC == ENABLE)
		SxCmd_DoCommand("dx tv vdet 0");
	#endif
	#if (HDMI_FUNC == ENABLE)
		SxCmd_DoCommand("dx hdmi vdet 0");
	#endif
	#if (LCD2_FUNC == ENABLE)
		SxCmd_DoCommand("dx lcd2 vdet 0");
	#endif
		SwTimer_DelayMs(500); //wait for insert/remove detect
		SxCmd_DoCommand("dx lcd2 vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
#endif
#endif

#if (TV_FUNC == ENABLE)
	if (strncmp(str1, "tvf1", 31) == 0) {
		DBG_DUMP("TV FMT = NTSC\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_NTSC);
	}
	if (strncmp(str1, "tvf2", 31) == 0) {
		DBG_DUMP("TV FMT = PAL\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_PAL);
	}
	if (strncmp(str1, "tvf3", 31) == 0) {
		DBG_DUMP("TV FMT = NTSC\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_NTSC_SD);
	}
	if (strncmp(str1, "tvf4", 31) == 0) {
		DBG_DUMP("TV FMT = PAL\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_PAL_SD);
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if (strncmp(str1, "hdf0", 31) == 0) {
		DBG_DUMP("HDMI FMT = (AUTO)\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_MAX); //change to max resoluition
	}
	if (strncmp(str1, "hdf1", 31) == 0) {
		DBG_DUMP("HDMI FMT = 1280x720p60\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1280X720P60);
	}
	if (strncmp(str1, "hdf2", 31) == 0) {
		DBG_DUMP("HDMI FMT = 1920x1080p30\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1920X1080P30);
	}
	if (strncmp(str1, "hdf3", 31) == 0) {
		DBG_DUMP("HDMI FMT = 1920x1080i60\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1920X1080I60);
	}
	if (strncmp(str1, "hdf4", 31) == 0) {
		DBG_DUMP("HDMI FMT = 3840x2160p30\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P30);
	}
	if (strncmp(str1, "hdf5", 31) == 0) {
		DBG_DUMP("HDMI FMT = 3840x2160p25\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P25);
	}
	if (strncmp(str1, "hdf6", 31) == 0) {
		DBG_DUMP("HDMI FMT = 3840x2160p24\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P24);
	}
	if (strncmp(str1, "hdf7", 31) == 0) {
		DBG_DUMP("HDMI FMT = 4096x2160p30\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P30);
	}
	if (strncmp(str1, "hdf8", 31) == 0) {
		DBG_DUMP("HDMI FMT = 4096x2160p25\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P25);
	}
	if (strncmp(str1, "hdf9", 31) == 0) {
		DBG_DUMP("HDMI FMT = 4096x2160p24\r\n");
		Ux_SendEvent(0, NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P24);
	}
#endif

#if (MOVIE_MODE==ENABLE)
	if (strncmp(str1, "mvs1", 31) == 0) {
		UINT32 uiSelect = MOVIE_SIZE_FRONT_1920x1080P30;
		DBG_DUMP("MOVIE SIZE = 1920x1080p30 (16:9)\r\n");
		UI_SetData(FL_MOVIE_SIZE, uiSelect);
		Ux_SendEvent(0, NVTEVT_EXE_MOVIESIZE, 1, uiSelect);
		//Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(uiSelect));
	}
	if (strncmp(str1, "mvs2", 31) == 0) {
		UINT32 uiSelect = MOVIE_SIZE_FRONT_640x480P30;
		DBG_DUMP("MOVIE SIZE = 640x480p30 (4:3)\r\n");
		UI_SetData(FL_MOVIE_SIZE, uiSelect);
		Ux_SendEvent(0, NVTEVT_EXE_MOVIESIZE, 1, uiSelect);
		//Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(uiSelect));
	}
#endif
#if (PHOTO_MODE==ENABLE)
	if (strncmp(str1, "pts1", 31) == 0) {
		UINT32 uiSelect = PHOTO_SIZE_2MHD;
		DBG_DUMP("PHOTO SIZE = 1920x1080 (16:9)\r\n");
		UI_SetData(FL_PHOTO_SIZE, uiSelect);
		Ux_SendEvent(0, NVTEVT_EXE_CAPTURE_SIZE, 1, uiSelect);
		Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetPhotoSizeRatio(uiSelect));
	}
	if (strncmp(str1, "pts2", 31) == 0) {
		UINT32 uiSelect = PHOTO_SIZE_VGA;
		DBG_DUMP("PHOTO SIZE = 640x480 (4:3)\r\n");
		UI_SetData(FL_PHOTO_SIZE, uiSelect);
		Ux_SendEvent(0, NVTEVT_EXE_CAPTURE_SIZE, 1, uiSelect);
		Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetPhotoSizeRatio(uiSelect));
	}
	if (strncmp(str1, "ptzi", 31) == 0) {
		UINT32 bStop = FALSE;
		PhotoExe_DZoomInBK(&bStop);
	}
	if (strncmp(str1, "ptzo", 31) == 0) {
		UINT32 bStop = FALSE;
		PhotoExe_DZoomOutBK(&bStop);
	}
#endif
	return TRUE;
}

SXCMD_BEGIN(sysvideo, "system command")
SXCMD_ITEM("disp %", Cmd_sys_disp, "change disp setting")
SXCMD_END()

