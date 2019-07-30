#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../..

MAKE_COMMON_DIR = $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/MakeOption.txt
include ModelConfig.txt
include ModelConfig_$(MODEL).txt
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = DemoKit

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
    $(ROOT_DIR)/Include \
    $(ROOT_DIR)/Alg/PlugIn

#----------------------------------------------------------------------
# Add/delete the project C source and assembly files here
#----------------------------------------------------------------------
ifeq "$(USE_ECOS_KERNEL)" "ON"
CPP_SRC = \
	SrcCode/eCos/hello_world.cpp
endif

ASM = \
    SrcCode/Kernel/CodeInfo.S

SRC = \
    SrcCode/Kernel/_main.c \
    SrcCode/Kernel/BinInfo.c \
    SrcCode/Kernel/SysCfg.c

ifeq "$(USE_ECOS_KERNEL)" "ON"
SRC += \
    SrcCode/eCos/eCos_main.c \
    SrcCode/eCos/ecos_FileSys.c \
    SrcCode/eCos/test_ping.c \
    SrcCode/eCos/cmd_tftp.c \
    SrcCode/eCos/test_eth.c \
    SrcCode/eCos/ecos_PStore.c
endif

SRC += \
    SrcCode/System/main.c \
    SrcCode/System/SysMain.c \
    SrcCode/System/SysMain_Exe.c \
    SrcCode/System/SysMain_Flow_Init.c \
    SrcCode/System/SysMain_Flow_Exit.c \
    SrcCode/System/SysMain_Flow_Mode.c \
    SrcCode/System/SysMain_Flow_Sleep.c \
    SrcCode/System/SysMain_State.c \
    SrcCode/System/SysMain_cmd.c \
    SrcCode/System/SysLock.c \
    SrcCode/System/SysVideo_Exe.c \
    SrcCode/System/SysVideo_CB.c \
    SrcCode/System/SysVideo_cmd.c \
    SrcCode/System/SysAudio_Exe.c \
    SrcCode/System/SysAudio_CB.c \
    SrcCode/System/SysStrg_Exe.c \
    SrcCode/System/SysStrg_CB.c \
    SrcCode/System/SysStrg_cmd.c \
    SrcCode/System/SysSensor_Exe.c \
    SrcCode/System/SysSensor_CB.c \
    SrcCode/System/SysSensor_cmd.c \
    SrcCode/System/SysLens_Exe.c  \
    SrcCode/System/SysLens_CB.c  \
    SrcCode/System/SysIpc_Exe.c \
    SrcCode/System/SysUctrl_Exe.c \
    SrcCode/System/SysBT.c \
    SrcCode/ProjectInfo.c \
    SrcCode/UIApp/ExifVendor.c  \
    SrcCode/UIApp/MovieUdtaVendor.c

#----------------------------------------------------------------------
# For disconnect the linking of MsdcNvtCb, Using MSDCVENDOR_NVT DISABLE is OK
#----------------------------------------------------------------------
SRC += \
    SrcCode/UIApp/UsbMsdcCb/IspSudoCb.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_Photo.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_Adj.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_UpdFw.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_CustomSi.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_IQSim.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_TimeSi.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_FRNN.c \
    SrcCode/System/SysExam_cmd.c

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/System/SysInput_Exe.c \
    SrcCode/System/SysInput_CB.c \
    SrcCode/System/SysInput_cmd.c \
    SrcCode/System/SysOutput_Exe.c \
    SrcCode/System/SysOutput_cmd.c \
    SrcCode/System/SysPower_Exe.c \
    SrcCode/System/SysPower_CB.c \
    SrcCode/System/SysPower_cmd.c \
    SrcCode/System/SysUsb_Exe.c \
    SrcCode/System/SysUsb_CB.c \
    SrcCode/System/SysUsb_cmd.c

SRC += \
    SrcCode/UIApp/MovieStamp/MovieStamp.c \
    SrcCode/UIApp/MovieStamp/MovieStampID.c \
    SrcCode/UIApp/MovieStamp/MovieStampTsk.c \
    SrcCode/UIApp/MovieStamp/DateStampFont10x16.c \
    SrcCode/UIApp/MovieStamp/DateStampFont12x20.c \
    SrcCode/UIApp/MovieStamp/DateStampFont18x30.c \
    SrcCode/UIApp/MovieStamp/DateStampFont20x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFont26x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl10x16.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl12x20.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl18x30.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl20x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl26x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl36x60.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl42x72.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl56x96.c \
	SrcCode/UIApp/MovieStamp/DateStampFont_WautoH44.c 

SRC += \
    SrcCode/UI/SysUICB.c \
    SrcCode/UI/UIDisplay.c \
    SrcCode/UI/UIGraphics.c \
    SrcCode/UI/UIGraphicsID.c \
    SrcCode/UI/UIView.c \
    SrcCode/UI/PlaybackView.c \
    SrcCode/UI/PhotoView.c \
    SrcCode/UI/MovieView.c \
    SrcCode/UI/UIBuffer.c \
    SrcCode/UIApp/MovieStamp/UniFont.c \
    SrcCode/UIApp/UserCmd/UserCmd.c \
    SrcCode/UIApp/UserCmd/UserCmdID.c \
    SrcCode/UIApp/AppDisp_PipView.c \
    SrcCode/UIApp/AppDisp_SbsView.c \
    SrcCode/UIApp/AppDisp_MoviePlayView.c \
    SrcCode/UIApp/AppDisp_MoviePlayViewID.c \
    SrcCode/UIApp/AppDisp_3DNRView.c \
    SrcCode/UIApp/AppDisp_PBView.c \
    SrcCode/UIApp/AppDisp_View.c
endif
endif

ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/AppDisp_IPCamView.c \
    SrcCode/UIApp/MovieStamp/MovieStamp.c \
    SrcCode/UIApp/MovieStamp/MovieStampID.c \
    SrcCode/UIApp/MovieStamp/MovieStampTsk.c \
    SrcCode/UIApp/MovieStamp/DateStampFont10x16.c \
    SrcCode/UIApp/MovieStamp/DateStampFont12x20.c \
    SrcCode/UIApp/MovieStamp/DateStampFont18x30.c \
    SrcCode/UIApp/MovieStamp/DateStampFont20x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFont26x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl10x16.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl12x20.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl18x30.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl20x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl26x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl36x60.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl42x72.c \
    SrcCode/UIApp/MovieStamp/DateStampFontTbl56x96.c \
    SrcCode/UIApp/MovieStamp/UniFont.c

endif

################### UIMode ######################
SRC += \
    SrcCode/Mode/UIMode.c \
    SrcCode/Mode/UIMode_cmd.c \
    SrcCode/Mode/UIModeMain.c  \
    SrcCode/Mode/UIModeSleep.c

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModeWifi.c
endif
endif


################### UIWnd ######################
ifeq "$(UI_STYLE)" "UI_STYLE_CARDV"
SRC += \
    SrcCode/UIWnd/CARDV/FlowCommon.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuCommon/MenuCommonItem.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuCommon/MenuCommonOption.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuCommon/TabMenu.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuMovie/MenuMovie.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/MenuPhoto.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/MenuPlayback.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/MenuSetup.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowPhotoParams.c   \
    SrcCode/UIWnd/CARDV/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/CARDV/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/CARDV/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/CARDV/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/CARDV/Engineer/CalSensor/cal_header.c \
    SrcCode/UIWnd/CARDV/Engineer/CalSensor/calibration_item.c \
    SrcCode/UIWnd/CARDV/Engineer/CalSensor/cal_ui_sample/cal_ui_sample.c

else ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
SRC += \
    SrcCode/UIWnd/SPORTCAM/FlowCommon.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommonItem.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommonOption.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommonConfirm.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/TabMenu.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommon.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuMode.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuMovie/MenuMovie.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPhoto/MenuPhoto.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPlay/MenuPlayback.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/MenuSetup.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/MenuExtSetup.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowPhotoParams.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/BurnInWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/KeyTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/MovieRecTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/MoviePlayTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/LCDTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/LEDTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/AudioTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalSensor/cal_header.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalSensor/calibration_item.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalSensor/cal_ui_sample/cal_ui_sample.c

else ifeq "$(UI_STYLE)" "UI_STYLE_TOUCH"
SRC += \
    SrcCode/UIWnd/TOUCHUI/FlowCommon.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommonItem.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommonOption.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommonConfirm.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/TabMenu.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommon.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuMode.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuMovie/MenuMovie.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuPhoto/MenuPhoto.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuPlay/MenuPlayback.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuSetup/MenuSetup.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuSetup/MenuExtSetup.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowPhotoParams.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalSensor/cal_header.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalSensor/calibration_item.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalSensor/cal_ui_sample/cal_ui_sample.c

else

endif

################### UIInfo ######################
ifeq "$(UI_STYLE)" "UI_STYLE_CARDV"
SRC += \
    SrcCode/UIWnd/CARDV/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/CARDV/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/CARDV/UIInfo/UIInfo.c \
    SrcCode/UIWnd/CARDV/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/CARDV/UIInfo/GSInfo.c

else ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
    SRC += \
    SrcCode/UIWnd/SPORTCAM/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/UIInfo.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/GSInfo.c

else ifeq "$(UI_STYLE)" "UI_STYLE_TOUCH"
    SRC += \
    SrcCode/UIWnd/TOUCHUI/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/UIInfo.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/GSInfo.c

else

endif
################### UIApp ######################
SRC += \
    SrcCode/UIApp/UIAppObj.c \

################### Network ######################

SRC += \
    SrcCode/UIApp/Network/UIAppNetwork.c \
    SrcCode/UIApp/Network/UIAppNetwork_cmd.c

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UIApp/Network/WifiAppXML.c \
    SrcCode/UIApp/Network/WifiAppCmd.c \
	SrcCode/UIApp/Network/UIAppWiFiCmd_Exe.c \
	SrcCode/UIApp/Network/UIAppWiFiCmdMovie_Exe.c \
	SrcCode/UIApp/Network/UIAppWiFiCmdPhoto_Exe.c \
    SrcCode/UIApp/Network/WifiAppCmdMapping.c \
    SrcCode/UIApp/Network/NetFs.c
endif
endif

################### Movie ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModeMovie.c \
    SrcCode/UIApp/Movie/UIAppMovie_Exe.c \
    SrcCode/UIApp/Movie/UIAppMovie_CB.c \
    SrcCode/UIApp/Movie/UIAppMovie_cmd.c \
    SrcCode/UIApp/Movie/UIAppMovie_GetFreeRecSec.c \
    SrcCode/UIApp/Movie/UIAppMovie_SetUserData.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Zoom.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Focus.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Iris.c \
    SrcCode/UIApp/Movie/UIAppMovie_CvHostEx.c \
    SrcCode/UIApp/Movie/UIAppMovie_NvtCvEx.c \
    SrcCode/UIApp/Movie/UIAppMovie_Param.c \
    SrcCode/UIApp/Movie/UIAppMovie_CommPoolTbl.c \
    SrcCode/UIApp/Movie/UIAppMovie_RecSetting.c
endif
endif

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UIApp/Movie/UIAppMovie_TimelapseLPR.c

endif
endif
################### Photo ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModePhoto.c \
    SrcCode/UIApp/Photo/UIAppPhoto_Exe.c \
    SrcCode/UIApp/Photo/UIAppPhoto_CB.c \
    SrcCode/UIApp/Photo/UIAppPhoto_cmd.c \
    SrcCode/UIApp/Photo/UIAppPhoto_Param.c \
    SrcCode/UIApp/Photo/UIDateImprint.c \
    SrcCode/UIApp/Photo/UIDateImprintID.c \
    SrcCode/UIApp/Photo/UIStorageCheck.c \
    SrcCode/UIApp/Photo/UIPhotoFuncInfo.c \

endif
endif

ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/Photo/UIPhotoFuncInfo.c

endif

ifeq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UIApp/Photo/UIPhotoFuncInfo.c

endif

################### Play ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModePlayback.c \
    SrcCode/UIApp/Play/UIAppPlay_Exe.c \
    SrcCode/UIApp/Play/UIAppPlay_CB.c \
    SrcCode/UIApp/Play/UIAppPlay_cmd.c \
    SrcCode/UIApp/Play/UIAppMoviePlay_Exe.c \
    SrcCode/UIApp/Play/UIPlaySlide.c \
    SrcCode/UIApp/Play/UIPlayWallpaper.c \
    SrcCode/UIApp/Play/UIPlayComm.c
endif
endif

##################IPCam ######################

ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeIPCam.c \
    SrcCode/Mode/UIModeIPCamPB.c \
    SrcCode/UIApp/IPCam/UIAppIPCam_Exe.c \
    SrcCode/UIApp/IPCamPB/UIAppIPCamPB_Exe.c \
    SrcCode/UIApp/IPCam/UIAppIPCam_CB.c \
    SrcCode/UIApp/IPCam/UIAppIPCam_cmd.c \
    SrcCode/UIApp/IPCamPB/UIAppIPCamPB_cmd.c \
    SrcCode/UIApp/IPCam/UIAppIPCam_Param.c \
	SrcCode/UIApp/IPCamPB/UIAppIPCamPB_Param.c
endif

################### UpdFw ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUpdFw.c
endif


################### Setup ######################
SRC += \
    SrcCode/UIApp/Setup/UISetup_Exe.c

################### Background ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UIApp/Background/UIBackgroundObj.c
endif
endif

################### UsbCam ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModeUsbCam.c \
    SrcCode/UIApp/UsbCam/UIAppUsbCam_Exe.c

endif
endif
################### UsbCommand ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UIApp/UsbCommand/UsbCmd.c \
    SrcCode/UIApp/UsbCommand/UsbCmdID.c \
    SrcCode/UIApp/UsbCommand/UsbCmdTsk.c

endif
endif
################### UsbDisk ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUsbDisk.c \
    SrcCode/UIApp/UsbDisk/UIAppUsbDisk_Exe.c

endif

################### UsbPrint ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UIApp/UsbPrint/UIAppUsbPrint_Exe.c

endif
endif
################### UsbPTP ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModeUsbPTP.c

endif
endif
################### UsbCharge ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModeUsbCharge.c \
    SrcCode/UIApp/UsbCharge/UIAppUsbCharge_Exe.c

endif
endif
################### UsbMenu ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/Mode/UIModeUsbMenu.c

endif
endif
################### UCtrlApp ######################
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UCtrlApp/Main/UCtrlMain.c \
    SrcCode/UCtrlApp/Main/UCtrlMainID.c \
    SrcCode/UCtrlApp/Mode/UCtrlMode.c \
    SrcCode/UCtrlApp/Setup/UCtrlAppSetup.c \
    SrcCode/UCtrlApp/System/UCtrlAppSystem.c \
    SrcCode/UCtrlApp/Movie/UCtrlAppMovie.c \
    SrcCode/UCtrlApp/Onvif/UCtrlAppOnvif.c \
    SrcCode/UCtrlApp/Onvif/UCtrlAppOnvifID.c\
    SrcCode/UCtrlApp/Image/UCtrlAppImage.c \
    SrcCode/UCtrlApp/nvt_status/uctrlapp_nvt_status.c \
    SrcCode/UCtrlApp/rtsp/uctrlapp_rtsp.c
endif

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
ifneq "$(UI_STYLE)" "UI_STYLE_USB_DISK"
SRC += \
    SrcCode/UCtrlApp/Photo/UCtrlAppPhoto.c \
    SrcCode/UCtrlApp/Photo/UCtrlAppPhotoID.c

endif
endif

################### Resource ######################
ifeq "$(UI_STYLE)" "UI_STYLE_CARDV"
SRC += \
    SrcCode/UIWnd/CARDV/Resource/UIResource.c \
    SrcCode/UIWnd/CARDV/Resource/BG_Images.c \
    SrcCode/UIWnd/CARDV/Resource/BG_Opening.c \
    SrcCode/UIWnd/CARDV/Resource/BG_Watermark.c \
    SrcCode/UIWnd/CARDV/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/CARDV/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_Image.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_EN.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_DE.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_ES.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_PO.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_IT.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_FR.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_SC.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_TC.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_JP.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_RU.c \
    SrcCode/UIWnd/CARDV/Resource/SoundData.c

else ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
SRC += \
    SrcCode/UIWnd/SPORTCAM/Resource/UIResource.c \
    SrcCode/UIWnd/SPORTCAM/Resource/BG_Images.c \
    SrcCode/UIWnd/SPORTCAM/Resource/BG_Opening.c \
    SrcCode/UIWnd/SPORTCAM/Resource/BG_Watermark.c \
    SrcCode/UIWnd/SPORTCAM/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/SPORTCAM/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_Image.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_PaletteOption.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_PaletteConfirm.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_EN.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_DE.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_ES.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_PO.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_IT.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_FR.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_SC.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_TC.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_JP.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_RU.c \
    SrcCode/UIWnd/SPORTCAM/Resource/SoundData.c

else ifeq "$(UI_STYLE)" "UI_STYLE_TOUCH"
SRC += \
    SrcCode/UIWnd/TOUCHUI/Resource/UIResource.c \
    SrcCode/UIWnd/TOUCHUI/Resource/UIResource2.c \
    SrcCode/UIWnd/TOUCHUI/Resource/BG_Images.c \
    SrcCode/UIWnd/TOUCHUI/Resource/BG_Opening.c \
    SrcCode/UIWnd/TOUCHUI/Resource/BG_Watermark.c \
    SrcCode/UIWnd/TOUCHUI/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/TOUCHUI/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_Image.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_PaletteOption.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_PaletteConfirm.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_EN.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_DE.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_ES.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_PO.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_IT.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_FR.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_SC.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_TC.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_JP.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_RU.c \
    SrcCode/UIWnd/TOUCHUI/Resource/SoundData.c

else

endif

################### Alg ######################
SRC += \
    ../../Alg/PlugIn/alg_id.c \
    ../../Alg/PlugIn/alg_VdoOut.c \
    ../../Alg/PlugIn/alg_CDNN_PhotoFuncInfo.c \
    ../../Alg/PlugIn/alg_CDNN2_PhotoFuncInfo.c

ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
SRC += \
    ../../Alg/PlugIn/alg_MovieStamp.c \
    ../../Alg/PlugIn/alg_Photo_PhotoFuncInfo.c \
    ../../Alg/PlugIn/alg_Movie_PhotoFuncInfo.c \
    ../../Alg/PlugIn/alg_UIFlowWndMovie.c \
    ../../Alg/PlugIn/alg_MovieExe.c
endif

ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    ../../Alg/PlugIn/alg_MovieStamp.c \
    ../../Alg/PlugIn/alg_IPCam_PhotoFuncInfo.c
endif

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
ARC_BASE_DIR = $(ROOT_DIR)/ARC

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS_D = $(C_DEFINE_EXT)

ASMFLAGS_D = \
    $(CHIP_PARAM_ASM)   \
    $(FWLOAD_PARAM_ASM)

CFLAGS_R = $(C_DEFINE_EXT)

ASMFLAGS_R = \
    $(CHIP_PARAM_ASM)   \
    $(FWLOAD_PARAM_ASM)

GCCFLAGS = $(C_DEFINE_EXT)

ifeq ($(OS),Linux)
	GIT_REVISION = $(shell git log --pretty=format:'%h' -1 2>>/dev/null)
	ifneq "$(GIT_REVISION)" ""
		CFLAGS_R += -D_GIT_REVISION_=0x$(GIT_REVISION)
		CFLAGS_D += -D_GIT_REVISION_=0x$(GIT_REVISION)
	endif
endif

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
include $(MAKE_COMMON_DIR)/OutputImg.txt
