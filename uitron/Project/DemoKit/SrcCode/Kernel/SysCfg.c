/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       ModuleInstall.c
    @ingroup

    @brief      Install Task,Sem,Flag for module.
                Install Task,Sem,Flag for module.

    @note       Nothing.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"
#include "EXTERNS.h"
#include "SysKer.h"

//===========================================================================
#include "DxSys.h"

//System Service
#include "SxCmd.h"
#include "GxSystem.h"
#include "DbgUtApi.h"
#include "Utility.h"

//Device Service
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#if (PST_FUNC == ENABLE)
#include "PStore.h"
#endif
#include "FileSysTsk.h"
#include "GxTimer.h"
#include "GxImage.h"
#include "vdodisp_api.h"

//Lib
#include "DCF.h"
#include "Exif.h"
#include "FileDB.h"
#include "GxSound.h"
#include "UMSDC.h"
#include "UVAC.h"
//#include "USBSIDC2.h"
#include "Audio.h"
#include "AudFilterAPI.h"

#if (LENS_FUNCTION == ENABLE)
#include "Lens.h"
#endif
#include "GxFlash.h"
#include "DhcpNvt.h"
#include "GxDisplay.h"
#if !defined(_Gyro_None_)
#include "rsc_task.h"
#endif
#include "rsc_api.h"
//charlie test
#include "WavStudioTsk.h"
#include "StreamReceiverAPI.h"
//end charlie test
#include "VideoCodec_H264.h"
#if (HTTP_LIVEVIEW_FUNC)
#include "LviewNvtAPI.h"
#endif

//App Playback
#include "PlaybackTsk.h"

//App Photo
#include "ae_task.h"
#include "awb_task.h"
#include "af_task.h"
#include "ipl_cmd.h"
#include "imgcaptureapi.h"
#include "sensor.h"
#include "photo_task.h"
#include "ipl_cal_api.h"

//App Usb
#include "MsdcNvtApi.h"
#include "MsdcNvtCb_Exam.h"
#include "IspSudoApi.h"

//App Media
#include "SMediaPlayAPI.h"
#include "NMediaRecAPI.h"
#include "NMediaPlayAPI.h"
#include "NvtIpcAPI.h"
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
#include "WiFiIpcAPI.h"
#endif
#include "FsIpcAPI.h"
#if (HFS_FUNC == ENABLE)
#include "HfsNvtAPI.h"
#endif
//#include "P2pIpcAPI.h"
#if(WIFI_AP_FUNC==ENABLE || IPCAM_FUNC==ENABLE)
#include "RtspNvtApi.h"
#include "RtspCliNvtApi.h"
#include "UsockIpcAPI.h"
#if (USOCKET_CLIENT==ENABLE)
#include "UsockCliIpcAPI.h"
#endif
#endif
#if (IOT_P2P_FUNC == ENABLE)
#include "P2pIpcAPI.h"
#endif


#if(IPCAM_DIS_MV_FUNC == ENABLE && IPCAM_FUNC==ENABLE)
#include "alg_IPCam_PhotoFuncInfo.h"
#endif

//User
#include "NvtUser.h"
#include "NvtBack.h"
#if(UI_FUNC==ENABLE)
#include "UIGraphics.h"
#include "UIControl.h"
#endif
#if(ENG_MODE==ENABLE)
#include "UserCmd.h"
#endif

#if(STAMP_FUNC==ENABLE)
#include "MovieStamp.h"
#include "MovieStampAPI.h"
#endif

#include "spiflash.h"

#include "iq_task.h"
//#include "IPL_Stitch_Id.h"

#if (GPS_FUNCTION == ENABLE)
#include "GPS.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif

#include "ImageStream.h"
#if(UCTRL_FUNC==ENABLE)
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "UCtrlAppPhoto.h"
#include "UCtrlAppOnvif.h"
#include "UCtrlAppSystem.h"
#endif
#if(CALIBRATION_FUNC==ENABLE)
#include "EngineerMode.h"
//#NT#2016/07/20#XL_Wei -begin
//#NT#Support web calibration
//#include "IPL_CalWEB_Task.h" // [cal-mark]
//#NT#2016/07/20#XL_Wei -end
//#NT#2016/07/19#Silvia Wu -begin
//#NT# support calibration usb type
//#include "IPL_CalUSB_Task.h" // [cal-mark]
//#NT#2016/07/19#Silvia Wu -end
#endif
#include "SysMain.h"
#include "PstoreIpcAPI.h"
#if (BT_FUNC==ENABLE)
#include "BT_API.h"
#endif
#include "videosprite.h"
#if (VIDEOSPRITEIPC==ENABLE)
#include "VideoSpriteIpc.h"
#endif
#if(FACE_GRADE_TEST==ENABLE)
#include "FaceGIpcAPI.h"
#endif
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/02/18#Lincy Lin -end
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
#include "CurlNvtAPI.h"
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/04/27#Lincy Lin -begin
//#NT#Support userlog function
#if (USERLOG_FUNC == ENABLE)
#include "userlog.h"
#endif
//#NT#2016/04/27#Lincy Lin -end
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
#include "UsbCmdAPI.h"
#endif
//#NT#2016/05/31#Ben Wang -end
#if (IPCAM_FUNC==DISABLE)
#include "AppDisp_MoviePlayView.h"
#endif

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif

//#NT#2018/04/03#Niven Cho -begin
//#NT#PARTIAL_COMPRESS
#if defined(_FW_TYPE_PARTIAL_COMPRESS_)
#include "PartLoad.h"
#endif
//#NT#2018/04/03#Niven Cho -end

#include "NMediaPlayAPI.h"

//#NT#2016/07/27#Charlie Chang -begin
//#NT#support iqtool ipc
#if _TODO
#include "IqtoolIpcAPI.h"
#endif
//#NT#2016/07/27#Charlie Chang -end
#include "ImageApp_MovieMulti.h"
#include "ImageUnit_ImgTrans.h"
#include "ImageUnit_UserProc.h"
#include "ImageUnit_ImagePProc.h"
#include "ImageUnit_StreamSender.h"
#include "ImageUnit_VdoDec.h"
#include "ImageUnit_AudDec.h"
#include "ImageUnit_StreamReceiver.h"
#include "nvtmpp.h"
#include "UIDateImprint.h"
#include "ImageUnit_FileOut.h"
#if (LINUX_MSDC == ENABLE)
#include "usbipc_api.h"
#endif

#ifdef  __S3_POWER_DOWN
#include "UIModeSleep.h"
#endif

#include "alg_id.h"

#if (IPCAM_AEC_FUNC == ENABLE)
#include "nvtaec_api.h"
#endif

//===========================================================================
//static OS resource (Config Task,Flag,Semaphore & MemPool)

DEFINE_OS_RESOURCE
(
	MAX_TASK_COUNT,
	MAX_SEMAPHORE_COUNT,
	MAX_FLAG_COUNT,
	BUF_NUM,
	MAX_MAILBOX_COUNT,
	MAX_MEMPOOL_COUNT + MAX_MEMPOOL2_COUNT,
	ARBIT2_START_ID
);

//===========================================================================
//Install user OS resource (Config Task,Flag,Semaphore & MemPool)
//  Note: stack is config by Task, heap is config by MemPool

//for sdio driver
UINT32 POOL_ID_STORAGE_SDIO = 0;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
UINT32 POOL_ID_STORAGE_SDIO2 = 0;
#endif
//for nand driver
UINT32 POOL_ID_STORAGE_NAND = 0;
//for ramdisk
UINT32 POOL_ID_STORAGE_RAMDISK = 0;
#if (FWS_FUNC == ENABLE)
//for FWStore
UINT32 POOL_ID_FWS_BUFFER = 0;
#endif
//for FileSys
UINT32 POOL_ID_FS_BUFFER = 0;
//for FsIpc
UINT32 POOL_ID_FSIPC = 0;
//for DCF
#if (USE_DCF == ENABLE)
UINT32 POOL_ID_DCF_BUFFER = 0;
#endif
#if (PST_FUNC == ENABLE)
//for PStore
UINT32 POOL_ID_PS_BUFFER = 0;
#endif
#if (WIFI_FUNC == ENABLE)
//for WiFiIpc
UINT32 POOL_ID_WIFIIPC = 0;
#endif
//for P2pIpc
#if (IOT_P2P_FUNC == ENABLE)
UINT32 POOL_ID_P2PIPC = 0;
#endif
//for UsockIpc
UINT32 POOL_ID_USOCKIPC = 0;
//for DHCPIpc
UINT32 POOL_ID_DHCPIPC = 0;
//for WiFi cmd XML
UINT32 POOL_ID_TEMP = 0;
#if(FACE_GRADE_TEST==ENABLE)
UINT32 POOL_ID_FACEGIPC = 0;
#endif
//for Gfx
UINT32 POOL_ID_GFX_IMAGE = 0;
UINT32 POOL_ID_GFX_STRING = 0;
UINT32 POOL_ID_GFX_TEMP = 0;
//for Exif
UINT32 POOL_ID_EXIF = 0;
//for Date Imprint
UINT32 POOL_ID_DATEIMPRINT = 0;
#if (USE_FILEDB==ENABLE)
//for fileDB
UINT32 POOL_ID_FILEDB = 0;
#endif
//for Display
UINT32 POOL_ID_DISP_OSD2 = 0;
UINT32 POOL_ID_DISP_OSD2TEMP = 0;
UINT32 POOL_ID_DISP_OSD1 = 0;
UINT32 POOL_ID_DISP_OSD1TEMP = 0;
UINT32 POOL_ID_DISP_VDO2 = 0;
UINT32 POOL_ID_DISP_VDO2TEMP = 0;
UINT32 POOL_ID_DISP_VDO1 = 0;
UINT32 POOL_ID_DISP_VDO1TEMP = 0;
//UINT32 POOL_ID_DISP_VDO1TEMP2 = 0;
//UINT32 POOL_ID_DISP_VDO1TEMP3 = 0;
//free
UINT32 POOL_ID_APP = 0;
UINT32 POOL_ID_APP_ARBIT2   = 0;
//for Multi-core
UINT32 POOL_ID_CPU2 = 0;
UINT32 POOL_ID_DSP = 0;
#if (HFS_FUNC == ENABLE)
UINT32 POOL_ID_HFSIPC = 0;
#endif
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
UINT32 POOL_ID_USBCMD = 0;
#endif
//#NT#2016/05/31#Ben Wang -end
UINT32 POOL_ID_NVTIPC = 0;
UINT32 POOL_ID_NVTUCTRL = 0;
UINT32 POOL_ID_CAL = 0;
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
UINT32 POOL_ID_CURLNVT = 0;
#endif
//#NT#2016/04/21#Lincy Lin -end

//#NT#2016/07/27#Charlie Chang -begin
//#NT# support iqtool ipc
#if(IQTOOL_FUNC == ENABLE)
UINT32 POOL_ID_IQTOOLIPC = 0;
#endif
//#NT#2016/07/27#Charlie Chang -end

//#NT#2016/08/19#Lincy Lin -begin
//#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
UINT32 POOL_ID_CVHOST = 0;
#endif
//#NT#2016/08/19#Lincy Lin -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
UINT32 POOL_ID_FTG_SNAPSHOT = 0;
#endif
//#NT#2016/11/11#Charlie Chang -end

#if (UCTRL_FUNC==ENABLE)
UINT32 POOL_ID_UCTRL_NVT_STATUS = 0;
#endif

#if (MSDCVENDOR_NVT == ENABLE)
UINT32 POOL_ID_MSDCNVT = 0, POOL_ID_ISPSUDO = 0;
#endif

#if (MSDCVENDOR_NVT_EXAM == ENABLE)
UINT32 POOL_ID_MSDCNVT_EXAM = 0;
#endif

#if (LINUX_MSDC == ENABLE)
UINT32 POOL_ID_USBIPC = 0;
#endif

#if (VIDEOSPRITEIPC != ENABLE)
UINT32 POOL_ID_VDS_BK = 0;
#endif

#if (LOGFILE_FUNC==ENABLE)
UINT32 POOL_ID_LOGFILE = 0;
#endif

void Install_User(void)
{
	/*
	NOTE: Some special limitations and tips about implement Install ID in OS_Main():
	1.  Before Install ID, we needs to confirm ZI section is already clear ok, make sure 1st code section has been load finished, and note that other code sections may not be ready yet.
	2.  While Install ID, all function codes have to be include in 1st code section, and all prototype declarations have to add into the _SECTION(".kercfg_text")
	3.  While Install ID, all global variables have to be include in 1st code section or ZI section. So prototype declarations have to add into the _SECTION(".kercfg_text")
	4.  While Install ID, the stack is only 1K bytes for kernel, do not call functions too deep.
	5.  While Install ID, kernel is not start yet, make sure only use OS_CONFIG_ APIs and debug message function, do not call any other kernel functions.
	6.  While Install ID, driver and I/O is also not start yet, do not call any driver APIs or i/o control APIs.
	*/

//System Service
	SwTimer_InstallID();
	SxCmd_InstallID();
	GxSystem_InstallID();
	UserMainProc_InstallID();

//Device Ext
	spiflash_InstallID();//SPI FLASH

//Device Service
	DbgUt_InstallID(); //CPU,DMA
	//Timer
	GxTimer_InstallID(); //TIMER
	//Storage
#if (FWS_FUNC == ENABLE)
	FwSrv_InstallID(); //NAND
#endif
#if defined(_CPU2_LINUX_)
	FileSys_InstallID(FileSys_GetOPS_Linux());
#else
	FileSys_InstallID(FileSys_GetOPS_uITRON());
#endif

#if (PST_FUNC == ENABLE)
	PStore_InstallID(); //NAND
#endif
	//Display
#if _TODO
	DispSrv_InstallID(); //DISPLAY
#endif
	vdodisp_install_id();

//Lib
#if (USE_DCF == ENABLE)
	DCF_InstallID();
#endif
#if (USE_FILEDB == ENABLE)
	FileDB_InstallID();
#endif
#if (PLAY_MODE==ENABLE)
#endif
	EXIF_InstallID();
#if (MSDCVENDOR_NVT == ENABLE)
	Msdc_InstallID();
	U2Msdc_InstallID();
#endif
#if (USB_MODE==ENABLE || IPCAM_UVC_FUNC==ENABLE)
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection
	#if defined(_USB2_DEVICE_)
		U2UVAC_InstallID();
	#endif
	#if defined(_USB1_DEVICE_)
		U3UVAC_InstallID();
	#endif
//#NT#2018/01/12#Ben Wang -end
#endif
#if (HTTP_LIVEVIEW_FUNC)
	LviewNvt_InstallID();
#endif
	GxSound_InstallID();
	AudFilter_InstallID();
#if (LENS_FUNCTION == ENABLE)
	LensCtrl_InstallID();
#endif

//App Playback
#if (PLAY_MODE==ENABLE)
	PB_InstallID();
#endif

//App Photo
	sensor_install_id();
	IPL_InstallID();
	IQS_InstallID();
	//IPL_Stitch_InstallID();
#if (PHOTO_MODE==ENABLE)
	Cap_InstallID();
#endif
#if !defined(_Gyro_None_)
	RSC_InstallID();
#endif
	Install_RSCAPIID();
	Photo_InstallID();
	AE_InstallID();
	AWB_InstallID();
	AF_InstallID();
	//RSC_InstallID();
	ipl_cal_install_id();
	//IqtoolIpc_InstallID();
//App Usb
#if (MSDCVENDOR_NVT == ENABLE)
	MsdcNvt_InstallID();
	IspSudo_InstallID();
#endif

#if (MSDCVENDOR_NVT_EXAM == ENABLE)
	MsdcNvtCb_ExamInstallID();
#endif

//App Media
	//SMediaRec_InstallID();
#if _TODO
	MovRec_InstallID();
#endif

#if (IPCAM_FUNC==ENABLE)
	NMR_ImgCap_InstallID();
	NMR_VdoEnc_InstallID();
	ISF_VdoEnc_InstallID();
	NMR_AudEnc_InstallID();
	NMP_VdoDec_InstallID();
	NMP_AudDec_InstallID();
	ISF_AudEnc_InstallID();
#else
	NMR_ImgCap_InstallID();
	NMR_VdoEnc_InstallID();
	NMR_AudEnc_InstallID();
	NMP_VdoDec_InstallID();
	NMP_AudDec_InstallID();
	NMR_BsMux_InstallID();
	NMR_FileOut_InstallID();
	ISF_VdoEnc_InstallID();
	ISF_AudEnc_InstallID();
	ISF_VdoDec_InstallID();
	ImageApp_MovieMulti_InstallID();     // Only for MOVIEMULTI test!!
#endif

    // FileOut
    ISF_FileOut_InstallID();

    // NMediaPlay
    NMP_BsDemux_InstallID();
	ISF_AudDec_InstallID();

#if (PLAY_MODE == ENABLE)
	SMediaPlay_InstallID();
	AppDisp_MoviePlayView_InstallID();
#endif
	NvtIPC_InstallID();
#if _TODO
	FsIpc_InstallID();
#endif
#if (HFS_FUNC == ENABLE)
	HfsNvt_InstallID();
#endif
#if (IOT_P2P_FUNC == ENABLE)
	P2pIpc_InstallID();
#endif
#if _TODO
	Dhcp_InstallID();
#endif
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
	WiFiIpc_InstallID();
#endif
#if(WIFI_AP_FUNC==ENABLE)
	RtspNvt_InstallID();
	RtspCliNvt_InstallID();
	UsockIpc_InstallID();
#if (USOCKET_CLIENT==ENABLE)
	UsockCliIpc_InstallID();
#endif
#endif
#if(IPCAM_FUNC==ENABLE && FORCE_UNPLUG_RTSP!=ENABLE)
	RtspNvt_InstallID();
#endif
#if (IPCAM_FUNC==ENABLE)
	Videosprite_InstallID();
	#if(IPCAM_DIS_MV_FUNC == ENABLE)
		IPCam_DIS_InstallID();
	#endif
#endif
#if (VIDEOSPRITEIPC==ENABLE)
       VideoSpriteIpc_InstallID();
#endif
#if(FACE_GRADE_TEST==ENABLE)
	FaceGIpc_InstallID();
#endif

//User
#if (GPS_FUNCTION == ENABLE)
	GPS_InstallID();
#endif
	NvtUser_InstallID();
	NvtBack_InstallID();
#if(UI_FUNC==ENABLE)
	UIControl_InstallID();
	UI_InstallID();
#endif
#if(STAMP_FUNC==ENABLE)
	MovieStamp_InstallID();
#endif
#if(ENG_MODE==ENABLE)
	UserCmd_InstallID();
#endif
#if(WIFI_AP_FUNC==ENABLE)
	WifiCmd_InstallID();
#endif
	//charlie test


	WavStudio_InstallID();

	StreamReceiver_InstallID();
	ISF_StreamReceiver_InstallID();
	//end charlie test

	ImageStream_InstallID();
#if(UCTRL_FUNC==ENABLE)
	NvtUctrl_InstallID();
	UCtrlMain_InstallID();
#if (PHOTO_MODE==ENABLE)
	UCtrlAppPhoto_InstallID();
#endif
	UCtrlAppOnvif_InstallID();
	UCtrlAppSys_InstallID();
#endif

#if(CALIBRATION_FUNC==ENABLE)
	Engineer_InstallID();
//#NT#2016/07/20#XL_Wei -begin
//#NT#Support web calibration
	//Cal_WEBInstallID(); // [cal-mark]
//#NT#2016/07/20#XL_Wei -end
//#NT#2016/07/19#Silvia Wu -begin
//#NT# support calibration usb type
	//Cal_USBInstallID(); // [cal-mark]
//#NT#2016/07/19#Silvia Wu -end
#endif
#if defined(_CPU2_LINUX_)
	PstoreIpc_InstallID();
#endif
#if (BT_FUNC==ENABLE)
	BT_InstallID();
#endif
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
	LogFile_InstallID();
#endif
//#NT#2016/02/18#Lincy Lin -end
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
	CurlNvt_InstallID();
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/04/27#Lincy Lin -begin
//#NT#Support userlog function
#if (USERLOG_FUNC == ENABLE)
	userlog_InstallID();
#endif
//#NT#2016/04/27#Lincy Lin -end
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
	UsbCmd_InstallID();
#endif
//#NT#2016/05/31#Ben Wang -end



#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	mp4log_InstallID();
#endif

	ISF_ImgTrans_InstallID();
	ISF_UserProc_InstallID();
	ISF_ImagePProc_InstallID();
	ISF_StreamSender_InstallID();
	nvtmpp_install_id();
#if (PHOTO_MODE==ENABLE)
	UiDateImprint_InstallID();
#endif

#if (LINUX_MSDC == ENABLE)
	usbipc_install_id();
#endif

#ifdef  __S3_POWER_DOWN
	ModeSleep_InstallID();
#endif

    alg_InstallID();

//#NT#2018/04/03#Niven Cho -begin
//#NT#PARTIAL_COMPRESS
#if defined(_FW_TYPE_PARTIAL_COMPRESS_)
	PartLoad_InstallID();
#endif
//#NT#2018/04/03#Niven Cho -end

#if (IPCAM_AEC_FUNC == ENABLE)
	NvtAec_InstallID();
#endif
}

void Install_DrvMEM(void)
{
//for nand driver
	OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_NAND,  POOL_SIZE_STORAGE_NAND,    POOL_CNT_STORAGE_NAND);      // nand driver working buffer
#if defined(_EMBMEM_SPI_NOR_)
//for ramdisk driver
	OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_RAMDISK,  POOL_SIZE_STORAGE_RAMDISK, POOL_CNT_STORAGE_RAMDISK); // ramdisk driver working buffer
#endif
//for sdio driver
	OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_SDIO,  POOL_SIZE_STORAGE_SDIO,    POOL_CNT_STORAGE_SDIO);      // sdio driver working buffer
#if(COPYCARD2CARD_FUNCTION == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_SDIO2, POOL_SIZE_STORAGE_SDIO,    POOL_CNT_STORAGE_SDIO);      // sdio2 driver working buffer
#endif
//for NvtIPC driver
	OS_CONFIG_MEMPOOL(POOL_ID_NVTIPC,        POOL_SIZE_NVTIPC,          POOL_CNT_NVTIPC);
}

void Install_DevEmbMEM(void)
{
#if (FWS_FUNC == ENABLE)
//for FWStore
	OS_CONFIG_MEMPOOL(POOL_ID_FWS_BUFFER,    POOL_SIZE_FWS_BUFFER,      POOL_CNT_FWS_BUFFER);        // FWStore working buffer
#endif
#if (PST_FUNC == ENABLE)
//for PStore
	OS_CONFIG_MEMPOOL(POOL_ID_PS_BUFFER,     POOL_SIZE_PS_BUFFER,       POOL_CNT_PS_BUFFER);         // PStore working buffer
#endif
}

void Install_DevStrgMEM(void)
{
//for FsIpc
#if(POOL_SIZE_FSIPC)
	OS_CONFIG_MEMPOOL(POOL_ID_FSIPC,         POOL_SIZE_FSIPC,           POOL_CNT_FSIPC);             // FsIpc working buffer
#endif
//for FileSys
	OS_CONFIG_MEMPOOL(POOL_ID_FS_BUFFER,     POOL_SIZE_FS_BUFFER,       POOL_CNT_FS_BUFFER);         // FileSys working buffer
#if (USE_DCF == ENABLE)
//for DCF
	OS_CONFIG_MEMPOOL(POOL_ID_DCF_BUFFER,    POOL_SIZE_DCF_BUFFER,      POOL_CNT_DCF_BUFFER);        // DCF  working buffer
#endif
#if (USE_FILEDB==ENABLE)
//for fileDB
	OS_CONFIG_MEMPOOL(POOL_ID_FILEDB,        POOL_SIZE_FILEDB,          POOL_CNT_FILEDB);            // FileDB working buffer
#endif

#if (LOGFILE_FUNC==ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_LOGFILE,       POOL_SIZE_LOGFILE,         POOL_CNT_LOGFILE);           // logfile working buffer
#endif
}

void Install_DevNetMEM(void)
{
#if (WIFI_FUNC == ENABLE)
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
//for WiFiIpc
	OS_CONFIG_MEMPOOL(POOL_ID_WIFIIPC,       POOL_SIZE_WIFIIPC,         POOL_CNT_WIFIIPC);           // WiFiIPc working buffer
#endif
#endif
#if(WIFI_AP_FUNC==ENABLE)
//for UsockIpc
	OS_CONFIG_MEMPOOL(POOL_ID_USOCKIPC,     POOL_SIZE_USOCKIPC,        POOL_CNT_USOCKIPC);          // UsockIPc working buffer
//for DHCPIpc
	OS_CONFIG_MEMPOOL(POOL_ID_DHCPIPC,       POOL_SIZE_DHCPIPC,         POOL_CNT_DHCPIPC);           // WiFiIPc working buffer
//for WiFi cmd XML
	OS_CONFIG_MEMPOOL(POOL_ID_TEMP,       POOL_SIZE_TEMP,         POOL_CNT_TEMP);           // WiFiCmd XML working buffer
#endif
#if(UCTRL_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_NVTUCTRL,      ALIGN_CEIL_64(NVTUCTRL_PARAM_BUF_SIZE),        POOL_CNT_NVTUCTRL);
#endif
#if (HFS_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_HFSIPC,        POOL_SIZE_HFSIPC,          POOL_CNT_HFSIPC);
#endif
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_CURLNVT,       POOL_SIZE_CURLNVT,           POOL_CNT_CURLNVT);
#endif
//#NT#2016/04/21#Lincy Lin -end

#if (UCTRL_FUNC==ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_UCTRL_NVT_STATUS, POOL_SIZE_UCTRL_NVT_STATUS, POOL_CNT_UCTRL_NVT_STATUS);
#endif
}

void UI_GetOSDBuf(UINT32 *pAddr, UINT32 *pSize)
{
	UINT32    uiPoolAddr, uiPoolSize;
	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD2);
	uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_OSD2)
				 + OS_GetMempoolSize(POOL_ID_DISP_OSD2TEMP)
				 + OS_GetMempoolSize(POOL_ID_DISP_OSD1)
				 + OS_GetMempoolSize(POOL_ID_DISP_OSD1TEMP);
	*pAddr = uiPoolAddr;
	*pSize = uiPoolSize;
}

void Install_DevOsdMEM(void)
{
#if(UI_FUNC==ENABLE)
//for Display
	UINT32 osd2_size = 0;
	UINT32 osd1_size = 0;
	//UI - manage by UIDisplay, buffer is fixed memory after OS start
#if (OSD2_FUNC == ENABLE)
#if (DISPLAY_OSD2_FMT == DISP_PXLFMT_INDEX8)
	osd2_size =
		ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W, OSD_H, TYPE_FB, PXLFMT_INDEX8));
#else
	debug_msg("ERR: invalid OSD2 fmt!\r\n");
#endif
#else // OSD2_FUNC disable, minimux OSD2 buffer
	osd2_size = 0x10;
#endif
	//UI - manage by UIDisplay, buffer is fixed memory after OS start
#if (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8888_PK)
	osd1_size =
		ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W, OSD_H, TYPE_FB, PXLFMT_RGBA8888_PK));
#elif (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8565_PK)
	osd1_size =
		ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W, OSD_H, TYPE_FB, PXLFMT_RGBA5658_PK));
#elif (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
	osd1_size =
		ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W, OSD_H, TYPE_FB, PXLFMT_INDEX8));
#else
	debug_msg("ERR: invalid OSD1 fmt!\r\n");
#endif

#if ((DUALDISP_FUNC == ENABLE) && (OSD_USE_ROTATE_BUFFER == ENABLE))
	//prepare 2 show DC and 1 paint DC
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2,     osd2_size,       POOL_CNT_DISP_OSD2);     // Display OSD2 buffer
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2TEMP, osd2_size * 2,     POOL_CNT_DISP_OSD2TEMP);   // Display OSD2 buffer 2
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1,     osd1_size,       POOL_CNT_DISP_OSD1);     // Display OSD1 buffer
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1TEMP, osd1_size * 2,     POOL_CNT_DISP_OSD1TEMP);   // Display OSD1 buffer 2
#else
	//prepare 1 show DC and 1 paint DC
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2,     osd2_size,       POOL_CNT_DISP_OSD2);     // Display OSD2 buffer
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2TEMP, osd2_size,       POOL_CNT_DISP_OSD2TEMP);     // Display OSD2 buffer 2
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1,     osd1_size,       POOL_CNT_DISP_OSD1);     // Display OSD1 buffer
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1TEMP, osd1_size,       POOL_CNT_DISP_OSD1TEMP);     // Display OSD1 buffer 2
#endif
	//for Gfx
	OS_CONFIG_MEMPOOL(POOL_ID_GFX_TEMP,      POOL_SIZE_GFX_TEMP,        POOL_CNT_GFX_TEMP);          // Gfx working + JPG decode buffer
#endif
}

void Install_DevVideoMEM(void)
{
//for Display
	USIZE vdo1_lcd = Calc_VideoSize_LCD();
	USIZE vdo1_max = Calc_VideoSize_MAX();
#if 0 //(VDO_USE_COPY_BUFFER == ENABLE)
    #if (DUALDISP_FUNC == DISABLE)
	UINT32 vdo1_size_1 = 0x10;
    #else
	UINT32 vdo1_size_1 = 0x10;
	UINT32 vdo1_size_2 = 0x10;
    #endif
#endif
	UINT32 vdo2_size;

	vdo1_lcd.w = ALIGN_CEIL_16(vdo1_lcd.w);
	vdo1_max.w = ALIGN_CEIL_16(vdo1_max.w);

	//UI - manage by UIDisplay, buffer is fixed memory after OS start
	//#NT#2016/03/29#Lincy Lin -begin
	//#NT#Fix the photo quick view display error when image size is 16:9 with rotate panel
#if(VDO_USE_ROTATE_BUFFER==ENABLE)
	if (vdo1_lcd.w * vdo1_lcd.h > VDO_W * VDO_H) {
		vdo2_size = ALIGN_CEIL_32(GxImg_CalcRequireSize(vdo1_lcd.w, vdo1_lcd.h, GX_IMAGE_PIXEL_FMT_YUV422_PACKED, vdo1_lcd.w));
	} else
#endif
	{
		vdo2_size = ALIGN_CEIL_32(GxImg_CalcRequireSize(VDO_W, VDO_H, GX_IMAGE_PIXEL_FMT_YUV422_PACKED, VDO_W));
	}
	//#NT#2016/03/29#Lincy Lin -end
	//#NT#2016/06/27#Lincy Lin -begin
	//#NT#Reduce IPCAM used memory pool size
#if (POWERONLOGO_FUNCTION==DISABLE && VDO_USE_ROTATE_BUFFER==DISABLE)
	{
		vdo2_size = 32;
	}
#endif
	//#NT#2016/06/27#Lincy Lin -end
	//App - manage by DispSrv, buffer is fixed memory after OS start
#if 0//(VDO_USE_COPY_BUFFER == ENABLE)
    #if (DUALDISP_FUNC == DISABLE)
    vdo1_size_1 =
        ALIGN_CEIL_32(GxImg_CalcRequireSize(vdo1_max.w, vdo1_max.h, GX_IMAGE_PIXEL_FMT_YUV422_PACKED, vdo1_max.w))*1;
    #else
    vdo1_size_1 =
        ALIGN_CEIL_32(GxImg_CalcRequireSize(vdo1_lcd.w, vdo1_lcd.h, GX_IMAGE_PIXEL_FMT_YUV422_PACKED, vdo1_lcd.w))*1;
    vdo1_size_2 =
        ALIGN_CEIL_32(GxImg_CalcRequireSize(vdo1_max.w, vdo1_max.h, GX_IMAGE_PIXEL_FMT_YUV422_PACKED, vdo1_max.w))*1;
    #endif
	//debug_msg("[vdo-buf] lcd w=%d h=%d\r\n", vdo1_lcd.w, vdo1_lcd.h);
	//debug_msg("[vdo-buf] max w=%d h=%d\r\n", vdo1_max.w, vdo1_max.h);
	//debug_msg("[vdo-buf] %d %d %d\r\n", vdo1_size_1, vdo1_size_2, vdo2_size);
#endif
#if (VDO_USE_ROTATE_BUFFER == ENABLE)
	//debug_msg("[vdo-buf] %d\r\n", vdo1_size_r);
#endif
#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
	//debug_msg("[vdo-buf] %d\r\n", vdo1_size_f);
#endif

	OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2,     vdo2_size,       POOL_CNT_DISP_VDO2);     // Display VDO2 jpg buffer
	OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2TEMP, vdo2_size,       POOL_CNT_DISP_VDO2TEMP);     // Display VDO2 jpg buffer 2
#if 0//(VDO_USE_COPY_BUFFER == ENABLE)
    #if (DUALDISP_FUNC == DISABLE)
	    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1,     vdo1_size_1,     POOL_CNT_DISP_VDO1);     // App: VDO1 frame buffer
    #else
	    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1,     vdo1_size_1,     POOL_CNT_DISP_VDO1);     // App: VDO1 frame buffer
	    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP, vdo1_size_2,     POOL_CNT_DISP_VDO1TEMP);     // App: VDO1 frame buffer 2
    #endif
#endif
}

void Install_MultiCoreMEM(void)
{
#if(POOL_SIZE_CPU2)
	OS_CONFIG_MEMPOOL(POOL_ID_CPU2,          POOL_SIZE_CPU2,            POOL_CNT_CPU2);
#endif

#if(POOL_SIZE_DSP)
	OS_CONFIG_MEMPOOL(POOL_ID_DSP,           POOL_SIZE_DSP,             POOL_CNT_DSP);
#endif
}

void Install_DevVCodecMEM(void)
{
//for Exif
	OS_CONFIG_MEMPOOL(POOL_ID_EXIF,          POOL_SIZE_EXIF,            POOL_CNT_EXIF);              // Exif lib working buffer

#if(STAMP_FUNC==ENABLE && STAMP_PRIVATE_POOL == DISABLE)
	//for DateImprint
	OS_CONFIG_MEMPOOL(POOL_ID_DATEIMPRINT,   POOL_SIZE_DATEIMPRINT_TOTAL,     POOL_CNT_DATEIMPRINT);       // DATE IMPRINT working buffer
#endif
}

void Install_DevImageMEM(void)
{
#if(POOL_SIZE_CAL)
	OS_CONFIG_MEMPOOL(POOL_ID_CAL,           POOL_SIZE_CAL,             POOL_CNT_CAL);               // Calibration data buffer
#endif
}

void Install_SystemMEM(void)
{
	//Driver
	Install_DrvMEM();
	//Embedded Storage
	Install_DevEmbMEM();
}

#if defined(_BSP_NA51000_)
#ifdef __ECOS
#else
static void Install_Arbit2MEM(void)
{
#if 0
	{
		UINT32 uiDDRTop2Cfg;
		uiDDRTop2Cfg = *(UINT32 *)0xF0100004;
		if ((uiDDRTop2Cfg & (0x1 << 5)) != (0x1 << 5))
		{
			return;
		}
	}
#endif
	if (dma_getDramCapacity(DMA_ID_2)) {
		OS_CONFIG_MEMPOOL_ARB2(POOL_ID_APP_ARBIT2,  POOL_SIZE_ARB2_FREE,    POOL_CNT_ARBIT2_APP);
	}
}
#endif
#endif


void Install_UserMEM(void)
{
	Install_SystemMEM();
	//Storage
	Install_DevStrgMEM();
	//Video
	Install_DevVideoMEM();
	//OSD
	Install_DevOsdMEM();
	//Imagepipe
	Install_DevImageMEM();
	//Video Codec
	Install_DevVCodecMEM();
	//Network
	Install_DevNetMEM();
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_USBCMD,      POOL_SIZE_USBCMD,          POOL_CNT_USBCMD);
#endif
//#NT#2016/05/31#Ben Wang -end
//free
#if(FACE_GRADE_TEST==ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_FACEGIPC,       POOL_SIZE_FACEGIPC,         POOL_CNT_FACEGIPC);
#endif
#if (IOT_P2P_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_P2PIPC,         POOL_SIZE_P2PIPC,           POOL_CNT_P2PIPC);
#endif
	//#NT#2016/07/27#Charlie Chang -begin
	//#NT# support iqtool ipc
#if(IQTOOL_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_IQTOOLIPC,         POOL_SIZE_IQTOOL_IPC,           POOL_CNT_IQTOOLIPC);
#endif
	//#NT#2016/07/27#Charlie Chang -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_FTG_SNAPSHOT, POOL_SIZE_FTG_SNAPSHOT, POOL_CNT_FTG_SNAPSHOT);

#endif
//#NT#2016/11/11#Charlie Chang -end

	//#NT#2016/08/19#Lincy Lin -begin
	//#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_CVHOST,         POOL_SIZE_CVHOST,           POOL_CNT_CVHOST);
#endif
	//#NT#2016/08/19#Lincy Lin -end

#if (MSDCVENDOR_NVT == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_MSDCNVT, ALIGN_CEIL_64(MSDCNVT_REQUIRE_MIN_SIZE), POOL_CNT_MSDCNVT);
	OS_CONFIG_MEMPOOL(POOL_ID_ISPSUDO, ALIGN_CEIL_64(ISPSUDO_REQUIRE_MIN_SIZE), POOL_CNT_ISPSUDO);
#endif

#if (MSDCVENDOR_NVT_EXAM == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_MSDCNVT_EXAM, ALIGN_CEIL_64(MSDCNVTCBEXAM_MSG_SIZE), POOL_CNT_MSDCNVT_EXAM);
#endif

#if (LINUX_MSDC == ENABLE)
	OS_CONFIG_MEMPOOL(POOL_ID_USBIPC,       POOL_SIZE_USBIPC,           POOL_CNT_USBIPC);
#endif

#if (VIDEOSPRITEIPC != ENABLE)
       OS_CONFIG_MEMPOOL(POOL_ID_VDS_BK,       POOL_SIZE_VDS_BK,           POOL_CNT_VDS_BK);
#endif


	OS_CONFIG_MEMPOOL(POOL_ID_APP,           POOL_SIZE_FREE - POOL_SIZE_CPU2 - POOL_SIZE_DSP, POOL_CNT_APP); // App: config all free memory for it

#if defined(_BSP_NA51000_)
#ifdef __ECOS
#else
	Install_Arbit2MEM();
#endif
#endif
	/// the end of Install_UserMEM
	Install_MultiCoreMEM(); /// IMPORTANT!! This call must be last line in Install_UserMEM();
}

#include "ImageStream.h"
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageUnit_NetHTTP.h"

void ImageStream_ConfigUserDump(void)
{
	/*
	    ImageUnit_CfgUserDump(&ISF_VdoIn1, Install_DevImageMEM);
	    ImageUnit_CfgUserDump(&ISF_Media, Install_DevVCodecMEM);
	    ImageUnit_CfgUserDump(&ISF_CamFile, Install_DevStrgMEM);
	    ImageUnit_CfgUserDump(&ISF_VdoOut1, Install_DevVideoMEM);
	    ImageUnit_CfgUserDump(&ISF_NetHTTP, Install_DevNetMEM);
	    ImageUnit_CfgUserDump(&ISF_NetRTSP, Install_DevNetMEM);
	*/
}
