#ifndef WIFICMDMOVIE_H
#define WIFICMDMOVIE_H

#include "UIFramework.h"

typedef enum {
	WIFI_MOV_ST_IDLE = 0,
	WIFI_MOV_ST_LVIEW,
	WIFI_MOV_ST_RECORD,
	WIFI_MOV_ST_MAX
} WIFI_MOV_ST_SETTING;

extern INT32 WiFiCmd_GetStatus(void);
extern UINT32  UIFlowWiFiMovie_GetMaxRecTime(void);

extern INT32 WiFiCmd_OnExeMovieRec(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeSetMovieRecSize(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeMovieHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
//#NT#2016/06/03#Charlie Chang -begin
//#NT#add contrast, audioIn, flip_mirror , quality set
extern INT32 WiFiCmd_OnExeSetMovieContrast(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieAudioIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieAudioInSR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieFlipMirror(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieQualitySet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
//#NT#2016/06/03#Charlie Chang -end
extern INT32 WiFiCmd_OnExeSetMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 WiFiCmd_OnExeSetMovieLiveviewSize(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeSetMovieGSesnorSensitivity(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeSetAutoRecording(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeSetMovieRecBitRate(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeSetMovieLiveviewBitRate(VControl *, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeMovieLiveviewStart(VControl *pCtrl, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnExeTriggerMovieRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//#NT#2017/01/03#Isiah Chang -begin
//#NT#Add WiFiCmd Bitrate control.
extern INT32 WiFiCmd_OnExeMovieBRCAdjust(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern BOOL WiFiCmd_BRC_AutoRunStateGet(void);
extern INT32 WiFiCmd_BRCAdjust(INT32 iLevel);
//#NT#2017/01/03#Isiah Chang -end
extern INT32 WiFiCmd_OnMovieFull(VControl *pCtrl, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnMovieWrError(VControl *pCtrl, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnStorageSlow(VControl *pCtrl, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnMovieOneSec(VControl *pCtrl, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnMovieRawEncJpgOKCB(VControl *pCtrl, UINT32, UINT32 *);
extern INT32 WiFiCmd_OnMovieRawEncErr(VControl *pCtrl, UINT32, UINT32 *);
extern void WiFiCmd_MotionDetectStart(void);
extern void WiFiCmd_MotionDetectStop(void);
extern void WiFiCmd_OnMotionDetect(void);
extern void WiFiCmd_HBStart(void);
extern void WiFiCmd_HBStop(void);
extern void WiFiCmd_HBOneSec(void);
extern INT32 WiFiCmd_OnExeMovieVedioReady(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

#endif
