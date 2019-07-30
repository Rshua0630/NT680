#ifndef _WIFIAPPXML_H_
#define _WIFIAPPXML_H_
#include "hfs.h"

void XML_SetTempMem(UINT32 uiAddr, UINT32 uiSize);
UINT32 XML_snprintf(char **buf, UINT32 *size, const char *fmt, ...);
int XML_GetModeData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_PBGetData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_QueryCmd(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetThumbnail(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetRawEncJpg(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetVersion(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetLiveViewFmt(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_FileList(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
void XML_DefaultFormat(UINT32 cmd, UINT32 ret, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType);
int XML_QueryCmd_CurSts(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetHeartBeat(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetFreePictureNum(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetMovieRecStatus(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetMaxRecordTime(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetDiskFreeSpace(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_DeleteOnePicture(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_DeleteAllPicture(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetPictureEnd(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetBattery(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_HWCapability(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
INT32 WifiCmd_getQueryData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_AutoTestCmdDone(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
int XML_APP_STARTUP(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
//#NT#2016/03/23#Isiah Chang -end
int XML_GetCardStatus(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetDownloadURL(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetUpdateFWPath(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetMovieFileInfo(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetSSID_passphrase(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetMovieSizeCapability(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
int XML_GetMenuItem(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
//upload command xml
int XML_UploadFile(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 bufSize, HFS_U32 segmentCount, HFS_PUT_STATUS putStatus);
int XML_UploadAudio(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 bufSize, HFS_U32 segmentCount, HFS_PUT_STATUS putStatus);
//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
int XML_GetCntModeStatus(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
//#NT#2016/06/01#Isiah Chang -end
//#NT#2016/06/08#Charlie Chang -begin
//#NT#support search wifi ap
int XML_GetWifiAP(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);
//#NT#2016/06/08#Charlie Chang -end
#endif
