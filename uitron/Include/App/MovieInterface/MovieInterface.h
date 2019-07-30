/**
    Header file of movie interface

    movie interface

    @file       MovieInterface.h
    @ingroup    mIAPPMOVIEINTERFACE

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MOVIEINTERFACE_H
#define _MOVIEINTERFACE_H

#include "Type.h"
#include "FileSysTsk.h"
#include "SysKer.h"


#include "movieinterface_def.h"

//#include "MediaRecAPI.h"
//#NT#2013/04/19#Calvin Chang#Add customer's Maker/Model name to MOV/MP4 file format by Nvtsystem -begin
#include "SMediaRecAPI.h"
#include "SMediaRawEncAPI.h"


/**
    Open movrec recorder task.

    Open movrec recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open Smedia recorder task successfully
        - @b E_SYS: Smedia recorder task is already opened
*/
extern ER   MovRec_Open(MEDIAREC_OBJ *pObj);

/**
    Close movrec recorder task.

    Close movrec recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: Smedia recorder task is not opened
*/
extern ER   MovRec_Close(void);

/**
    Change movrec recorder parameters.

    Change movrec recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER   MovRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Start movrec recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: Smedia recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER   MovRec_Record(void);

/**
    Stop movrec recording.

    @param[in] waitType waiting type as media recording stop, MOVREC_NO_WAIT or MOVREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER   MovRec_Stop(MOVREC_STOP_TYPE ucWait);
extern ER   MovRec_ResetMemoryValue(UINT32 addr, UINT32 size);//2011/06/15 Meg Lin
extern ER   MovRec_StopFlash(UINT32 pathid);

/**
    Re-allocate buffer to movrec recorder.

    Re-allocate buffer for movrec video and audio recording usage.

    @note The buffer should be re-allocated before media recording.

    @param[in] memStart buffer starting address
    @param[in] memSize  buffer size

    @return
        - @b E_OK:  re-allocate buffer successfully
        - @b E_SYS: re-allocate buffer failed (memory is not enough)
*/
extern ER   MovRec_ReAllocBuf2VA(UINT32 addr, UINT32 size);
extern void MovRec_CB(UINT32 uiEventID, UINT32 uiSeconds);

extern UINT32   MovRec_Disk2Second(void);
extern void     MovRec_MinusFilesize(UINT32 size);

/**
    Get movrec recorder parameter settings.

    Get movrec recorder parameter settings, only support getting width and height information currently.

    @note

    @param[in]  type    parameter type, only support MOVREC_GETINFO_WIDTH_HEIGHT currently
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value
    @param[out] p3      parameter3 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER       MovRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     MovRec_SetUserData(UINT32 addr, UINT32 size);

/**
    Estimate remaining time for media recording.

    Estimate remaining time in seconds according to storage device space with different counting type.

    @note This function should be called before Smedia recording.

    @param[in] type counting type, MOVREC_COUNTTYPE_FS (according to storage free space),
                    MOVREC_COUNTTYPE_4G (FAT file size limitation), or MOVREC_COUNTTYPE_USER
                    (user defined space size, for special purpose)
    @param[in] size user defined space size, just for MOVREC_COUNTTYPE_USER type

    @return Estimated remaining time in seconds.
*/
extern UINT32   MovRec_Disk2SecondWithType(UINT32 type, UINT32 size);

/**
    Estimate remaining time for media recording.

    @param[in] first: 1 to add TBR[0]
    @param[in] second: 1 to add TBR[1], if multi-recording
    @return Estimated remaining time in seconds.
*/
extern UINT32 MovRec_GetFreeRecSecByPath(UINT32 first, UINT32 second);

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for Smedia recording.

    @note

    @param[in] type     restriction type, MOVREC_RESTR_INDEXBUF or MOVREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     MovRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     MovRec_SetFre1Data(UINT32 addr, UINT32 size);

/**
    Stop movrec recording and set read-only.

    Stop movrec recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       MovRec_StopAndReadOnly(void);

/**
    To get movrec recorder status.

    @return
        - @b MOVREC_STATUS_NOT_OPENED:        movrec recorder is not opened
        - @b MOVREC_STATUS_OPENED_NOT_RECORD: movrec recorder is opened but not recording
        - @b MOVREC_STATUS_RECORDING:         movrec recorder is recording
*/
extern MOVREC_STATUS_TYPE   MovRec_GetStatus(void);

/**
    Set audio volume for movrec Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER       MovRec_SetAudioVolume(UINT32 percent);

/**
    Set ready buffer for movrec recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     MovRec_PutReadyBuf(MP_VDOENC_YUV_SRC *pInfo);
extern void     MovRec_GetReadyBuf(MP_VDOENC_YUV_SRC *pInfo);
extern void     MovRec_SetReadyBuf(MP_VDOENC_YUV_SRC *pInfo);
extern ER       MovRec_PutReadyBufWithID(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);
extern void     MovRec_GetReadyBufWithID(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);
extern void     MovRec_SetReadyBufWithID(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);
extern void     MovRec_SetReadyBuf4RawEnc(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);
extern void     MovRec_Pause(void);
extern void     MovRec_Resume(void);

/**
    Set GPS data.

    Set GPS data for movrec container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     MovRec_SetGPSData(UINT32 addr, UINT32 size);

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     MovRec_SetCrash(void);

/**
    Set lastfile as read-only and move to RO folder.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    last has been RO already
*/
extern UINT32 MovRec_SetCrash_Lastfile(void);

/**
    Set nextfile as read-only and move to RO folder.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            next file will be read-only
*/
extern UINT32 MovRec_SetCrash_Nextfile(void);

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     MovRec_CancelCrash(void);

/**
    Set event.

    Set current recording file to be renamed and read-only.

    @param[in] pathid 0 or 1
    @param[in] eventid MEDIAREC_CRASHTYPE_EV1 or others

    @return void
*/
extern void MovRec_SetEvent(UINT32 pathid, UINT32 eventid);

/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     MovRec_SetTheSameCrashDid(UINT8 on);

/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     MovRec_SetCrashFolderName(char *pChar);

/**
    Set naming rule for movrec Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER       MovRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl);
extern void     MovRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
extern void     MovRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB);
extern void     MovRec_VideoDoEnc(void);
extern void     MovRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid);
extern UINT32   MovRec_GetNowVideoFrames(void);

/**
    Open movrec recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void     MovRec_OpenErrMsg(UINT8 on);

/**
    Reset movrec recorder containerMaker.

    @return void
*/
extern void     MovRec_ResetConMaker(void);

/**
    Open movrec recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void     MovRec_OpenWrnMsg(UINT8 on);

/**
    Open movrec recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void     MovRec_OpenCutMsg(UINT8 on);

/**
    Check movrec recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL     MovRec_IsRecording(void);

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void     MovRec_SetGiveYUVBuf(UINT32 addr, UINT32 size);

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void     MovRec_GiveYUV(MP_VDOENC_YUV_SRC *ptr);

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER       MovRec_SetExtendCrash(void);

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32   MovRec_DumpDataStatus(UINT32 *pData);

/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32   MovRec_GetLastV_BS(MEDIAREC_VIDBS_TYPE vidnum, MEDIAREC_MEM_RANGE *frameBuf);

/**
    Force movrec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void     MovRec_CheckFSDiskInfo(void);

/**
    Open movrec recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void     MovRec_OpenFSOBJMsg(UINT8 on);

/**
    Open movrec recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void     MovRec_OpenFileMsg(UINT8 on);

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void     MovRec_SetPath2WriteCard(UINT32 on);

/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void     MovRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void     MovRec_MinusFilesize(UINT32 size);
extern void     MovRec_StopPreRecordStartWriting(void);

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
extern void     MovRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize);
extern void     MovRec_InstallID(void);
/**
    Set MediaRec version.

    @param[in] ver MEDIAREC_VER_2_0/MEDIAREC_VER_3_0
    @return void
*/
extern void     MovRec_SetVersion(UINT32 ver);

/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32   MovRec_GetLiveViewMinMemSize(void);

/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void     MovRec_UpdateSetting2CaluSec(void);

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void     MovRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size);

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void MovRec_SetROPercent(UINT32 percent);

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void MovRec_EnableEndChar4SingleRec(UINT8 value);

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void MovRec_EnableEndChar4MultiRec(UINT8 value);

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void MovRec_GetLastA_BS(MEDIAREC_MEM_RANGE *frameBuf);

/**
    Get lastest audio PCM raw data.

    @param[in] frameBuf output audio PCM raw data addr/size
    @return void
*/
extern void MovRec_GetLastA_PCM(MEDIAREC_MEM_RANGE *frameBuf);

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void MovRec_GetLastFilePath(char *pPath);

/**
    Get lastest EMR movie filepath. (callback event:MOVREC_EVENT_EMR_FILECOMPLETE)

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void MovRec_GetLastEMRFilePath(char *pPath);


/**
    Get lastest del movie filepath. (Immoc:MOVREC_IMMPROC_DELETE_FILE)

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void MovRec_GetLastDelFilePath(char *pPath);

/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void MovRec_EnableCheckReadonly(UINT8 value);

/**
    Set max customdata size. Card capacity will minus this size when start recording.

    @param[in] fileID
    @param[in] maxSize
    @return void
*/
extern void MovRec_SetCustomDataMaxSize(UINT32 fileid, UINT32 maxSize);
extern void MovRec_SetCustomDataWithID(UINT32 id, UINT32 tag, UINT32 addr, UINT32 size);

/**
    Get H.264 encoding path after some settings (such as resolution, frame rate, ...).

    @param[in] uiVidEncId   Video encode ID.
    @return void
*/
extern MP_VDOENC_PATH MovRec_GetEncPath(UINT32 uiVidEncId);

/**
    Set raw encode JPEG width and height.

    @param[in] uiPathId     Raw encode path ID, 1st path or 2nd path
    @param[in] uiWidth      JPEG width
    @param[in] uiHeight     JPEG height
    @return void
*/
extern void MovRec_SetRawEncJpgSize(UINT32 uiPathId, UINT32 uiWidth, UINT32 uiHeight);

/**
    Set raw encode source image width and height.

    @param[in] uiPathId     Raw encode path ID, 1st path or 2nd path
    @param[in] uiWidth      source image width
    @param[in] uiHeight     source image height
    @return void
*/
extern void MovRec_SetRawEncSrcSize(UINT32 uiPathId, UINT32 uiWidth, UINT32 uiHeight);

/**
    Trigger raw encoding.

    @param[in] uiPathType   Raw encode path type, all path or one path
    @param[in] uiPathId     Raw encode path ID, 1st path or 2nd path
    @return
        - @b SMRAWENC_OK:                   raw encoding successfully
        - @b SMRAWENC_ERR_NOT_OPEN:         raw encode task is not opened
        - @b SMRAWENC_ERR_BUSY:             raw encode task is busy
        - @b SMRAWENC_ERR_IMG_NOT_READY:    YUV raw image is not ready
        - @b SMRAWENC_ERR_UNSAFE_TO_WRITE:  memory may be not enough for writing JPEG file
*/
extern INT32 MovRec_TriggerRawEncode(UINT32 uiPathType, UINT32 uiPathId);

/**
    Enable/disable card writing.

    @param[in] pathid   0 or 1
    @param[in] on       0: not save, 1: as default
    @return void
*/
extern void MovRec_SetWriteCard(UINT32 pathid, UINT32 on);

/**
    Enable/disable EmergencyRecording for path0/1
    (If EMR is enable, it needs more memory!!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @param[in] on       0: disable 1: enable
    @return void
*/
extern void MovRec_SetEmergencyRecording(UINT32 pathid, UINT32 on);

/**
    Enable/disable EMR loop recording for path0/1
    (1st file rollback, but others just overlap 1 sec)
    (of course, EMR should be enable)

    @param[in] pathid   0 or 1(if path1 is enable)
    @param[in] on       0: disable 1: enable
    @return void
*/
extern void MovRec_SetEMR_loop(UINT32 pathid, UINT32 on);

/**
    Set H.264 Max Qp level.

    @param[in] level VIDENC_MAX_QP_LEVEL_0 as default
    @return void
*/
extern void MovRec_SetH264MaxQp(UINT32 pathid, UINT32 level);

/**
    Set H.264 GOP number.

    @param[in] gopnum 15 as default. Max: 120
    @return void
*/
extern void MovRec_SetH264GopNum(UINT32 gopnum);

/**
    Set H.264 GOP number by pathid.

    @param[in] gopnum 15 as default. Max: 120
    @param[in] pathID 0 as 1st path
    @return void
*/
extern void MovRec_SetH264GopNumByPath(UINT32 gopnum, UINT32 pathID);

/**
    Get movie recording parameters.

    @param[in] type MOVREC_GETRECPARAM_TARGETBR or others

    @return ER
        - @b E_OK:  OK
        - @b E_SYS: params error
*/
extern ER MovRec_GetParam(UINT32 type, UINT32 *pParam1, UINT32 *pParam2, UINT32 *pParam3);

/**
    Get last rawencoding JPG data.

    @param[in] puiAddr output JPG addr
    @param[in] puiSize output JPG size
    @param[in] fileID 0:path0, 1:path1
*/
extern void MovRec_RawEncGetJpgData(UINT32 *puiAddr, UINT32 *puiSize, UINT32 fileid);

/**
    Set Force no write card

    @param[in] value 1:no write

    @return void
*/
extern void MovRec_ForceNoWriteCard(UINT32 value);

/**
    Set frameinfo size. Default: 8MB.

    @param[in] size
    @return void
*/
extern void MovRec_SetFrameInfoSize(UINT32 size);

/**
    Set max stopflash rollback sec

    @param[in] value 1~30 is valid, default: 30

    @return void
*/
extern void MovRec_SetMaxStopFlashSec(UINT32 value);

/**
    Set max stopflash rollback Keep sec

    @param[in] value 3~30 is valid, default: 30

    @return void
*/
extern void MovRec_SetMaxKeepSec(UINT32 value);


/**
    Enable rollback or not

    @param[in] enable: 0 no overlap, 1 overlap 1sec

    @return void
*/
extern void MovRec_EnableOverlap(UINT32 enable);

/**
    Get information by path, similar to MovRec_GetInfo()

    @param[in]  type    parameter type, only support MOVREC_GETINFO_WIDTH_HEIGHT currently
    @param[in]  pathid  video pathid, 0 or more
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER MovRec_GetInfoByPath(UINT32 type, UINT32 pathid, UINT32 *p1, UINT32 *p2);

/**
    Enable/disable ExtendCrash

    @param[in] enable: 0 no extend crash

    @return void
*/
extern void MovRec_Enable_ExtendCrash(UINT32 value);

/**
    Enable/disable Emergency recording with one shot.

    @param[in] enable: 0 no rawenc (0 as default)

    @return void
*/
extern void MovRec_Enable_EMRwithRawEnc(UINT32 value);

/**
    Set NextSec to call StopFlash.

    @param[in] value 5 is default (smallest : 5)

    @return void
*/
extern void MovRec_SetStopFlash_NextSec(UINT32 value);

/**
    Change h264 target bitrate

    @param[in] vsID  (start from 0)
    @param[in] value (1 as default)

    @return void
*/
extern void MovRec_ChangeVideoTBR(UINT32 vsID, UINT32 value);

/**
    Set RawEnc write JPG file or not.

    @param[in] value (default: 1)

    @return void
*/
extern void MovRec_RawEncEnableWriteCard(UINT32 enable);

/**
    Add pure streaming for media 4.0

    @param[in] pinfo information to add

    @return ER E_OK:success
*/
extern ER   MovRec_AddPureStreaming(MOVREC_ADD_INFO *pinfo);

extern void     MovRec_SetvidBs_RevSec(UINT32 value);
extern void     MovRec_SetvidBs_NumInBs(UINT32 value);

/**
    Check if RawEnc ready to capture

    @param[in] void

    @return ER E_OK:success
               E_SYS: RawEnc is not open
               E_PAR: not ready
*/
extern ER   MovRec_SMRawEnc_IsReadyToCapture(void);

/**
    Set extendcrash unit (10/20/30 second)

    @param[in] value MOVREC_EXTENDCRASH_UNIT

    @return void
*/
extern void MovRec_SetExtendCrashUnit(MOVREC_EXTENDCRASH_UNIT value);

/**
    According to now setting, calculate suggested memory size.

    @param[in] void

    @return Size in bytes
*/
extern UINT32 MovRec_GetSuggestMem(void);

/**
    Get now video encoding Y address by path.

    @param[in] pathID: 0 = 1st path

    @return Y address
*/
extern UINT32 MovRec_GetNowVidYuvAddr(UINT32 pathID);

/**
    Input settings to calculate recording seconds.

    @param[in] pSetting input recording setting
    @return recording seconds
*/
extern UINT32 MovRec_CalcSecBySetting(MEDIAREC_CALCSEC_SETTING *pSetting);

/**
    Force size of all movie files align to a special size.
    Only 16M/32M is valid. (0x1000000  0x2000000)

    @param[in] size (0x1000000 or 0x2000000)

    @return void
*/
extern void MovRec_Enable_FileSizeAlign(UINT32 size);

/**
    Set GPS block size. Max:100KB, default: 32KB

    @param[in] void

    @return Size in bytes
*/
extern void MovRec_SetGpsBlkSize(UINT32 value);

/**
    Get if vidPath is writing to card

    @param[in] vidPathID 0 as 1st path, 1 as 2nd path

    @return 0: not write
*/
extern UINT32 MovRec_IsWriting(UINT32 vidPathID);

/**
    Cut two-path EMR recordings at the same time!
    @param[in] void
    @return
        - @b E_OK: both OK
        - @b E_SYS: other errors
*/
extern ER MovRec_MultiStopFlash(void);

/**
    Enable/disable overwrite YUV test. default off

    @param[in] on: 1 to open test

    @return void
*/
extern void MovRec_overwriteYUVtest(UINT32 on);

/**
    Register project 3DNR callback

    @param[in] CB: project 3DNR callback

    @return void
*/
extern void MovRec_Register3DNRCb(UINT32 pathid, MEDIAREC_3DNRCB CB);

/**
     Install Task, Sem, Flag ID for MovRec.

*/
extern void MovRec_InstallID(void) _SECTION(".kercfg_text");

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
/**
    Get Now Display Frame YUV Address

    @param  [in] pathid: Path ID
    @param  [in] py: Y plane address pointer
    @param  [in] pu: U plane address pointer
    @param  [in] pv: V plane address pointer

    @return void
*/
extern void MovRec_GetNowDisplayFrame(UINT32 pathid, UINT32 *py, UINT32 *pu, UINT32 *pv);
//#NT#2016/09/20#Bob Huang -end

/**
    Register project AudioStart

    @param[in] CB: project AudioStart

    @return void
*/
extern void MovRec_RegisterCB_Start(MediaRecCB_Start IPLCb);

/**
    Register project AudioGet

    @param[in] CB: project AudioGet

    @return void
*/
extern void MovRec_RegisterCB_Get(MediaRecCB_Get IPLCb);

/**
    Reset one path to I-frame (only for VS/VSWA)

    @param[in] vsID: path ID

    @return void
*/
extern void MovRec_ResetI(UINT32 vsID);

/**
    Set delete readonly files or not.

    @param[in] value: 0 means no delete RO (1 as default)

    @return void
*/
extern void MovRec_SetDeleteReadonly(UINT32 value);

/**
    Cut now to end last file, start new file.

    @param[in] void

    @return ER E_OK or others
*/
extern ER MovRec_SetCutNOW(void);

/**
    Set playing framerate for Golfshot recformat.

    @param[in] value: must < 30 (30 as default)

    @return void
*/
extern void MovRec_SetGolfshot_FrameRate(UINT32 value);

/**
    Set playing framerate for timelapse recformat.

    @param[in] value: must < 30 (30 as default)

    @return void
*/
extern void MovRec_SetTimelapse_FrameRate(UINT32 value);

/**
    Enable/disable unlimited video TBR.

    @param[in] en: 1 to enable (default: disable)

    @return void
*/
extern void MovRec_EnUnlimitedLowTBR(UINT32 en);

#endif
