/**
    Header file of media fileformat writing library

    Exported header file of media fileformat writing library.

    @file       MediaWriteLib.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.01.006
    @date       2011/06/23

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MEDIAWRITELIB_H
#define _MEDIAWRITELIB_H

#include "Type.h"
#include "AudFilterAPI.h"
#include "media_def.h"

/**
    @addtogroup mIMEDIAWRITE
*/
//@{


#define AVI_USERDATA_MAXLEN 128//2011/11/22 Meg adds ///< user data max length for avi file

//#NT#2012/06/26#Hideo Lin -begin
//#NT#To support MP4 file type
#define MEDIA_FTYP_MOV      0       ///< mov file
#define MEDIA_FTYP_MP4      1       ///< mp4 file, audio must be AAC
//#NT#2012/06/26#Hideo Lin -end

#define MEDIARECVY_VERSION         0x101//ver 1.01 //2012/09/11 Meg Lin
#define SMEDIARECVY_VERSION        0x301//ver 3.01 //2014/04/23 Meg Lin matches to media 3.0
#define SMEDIARECVY_CO64_VERSION   0x302//ver 3.02
#define SM2015_RCVY_VERSION        0x315//ver 3.15 //2015/12/3 new 2015 for FileTruncate
#define SM2015_RCVY_CO64_VERSION   0x316//ver 3.16

#define CARDV_NEWROOTLEN  16

#define  AVIWRITER_COUNT 2

#define NAMERULE_ATTRIB_NORMAL          0x00
#define NAMERULE_ATTRIB_OR_READONLY     0x01
#define NAMERULE_ATTRIB_CLR_READONLY    0x02

/**
    Get information from media fileformat writing library.
    format: ER (*GetInfo)(MEDIAWRITE_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
*/
typedef enum {
	MEDIAWRITE_GETINFO_HEADERSIZE       = 0x01,   ///< p1: id(in), p2: headersize per second(out)
	MEDIAWRITE_GETINFO_VALID_VFNUM      = 0x02,   ///< p1: id(in), p2: valid video frame numbers(out)//2010/04/20 Meg Lin
	MEDIAWRITE_GETINFO_SIZE_ONEENTRY    = 0x03,   ///< p1: size of one entry (out)
	MEDIAWRITE_GETINFO_NEWIDX1_BUF      = 0x04,   ///< p1: new back header addr (out), p2: size (out), for file-recovery
	MEDIAWRITE_GETINFO_BACK1STHEAD_BUF  = 0x05,   ///< p1: new front header addr (out), p2: size (out), for file-recovery
	MEDIAWRITE_GETINFO_ROUGHHDRSIZE     = 0x06,   ///< p1: video frame rate (in), p2: aud frame rate(in), p3:headersize per second(out)
	MEDIAWRITE_GETINFO_ROUGH_VVFNUM     = 0x07,   ///< p1: MP_ESTIMATE_VNUM(in), p2:rough video frame number(out)
	MEDIAWRITE_GETINFO_FINALFILESIZE    = 0x08,   ///< p1: id(in), p2:filesize(out)
	MEDIAWRITE_GETINFO_VFR_TRANS        = 0x09,   ///< p1: id(in),
	MEDIAWRITE_GETINFO_BACKHDR_SIZE     = 0x0A,   ///< p1: id(in), p2: backhdr size(out)
	MEDIAWRITE_GETINFO_VIDEOENTRY_ADDR  = 0x81,   ///< p1: id(in), p2:video frame number(in), p3: MEDIAREC_ENTRY_INFO(out)
	MEDIAWRITE_GETINFO_AUDIOENTRY_ADDR  = 0x82,   ///< p1: id(in), p2:audio frame number(in), p3: MEDIAREC_ENTRY_INFO(out)
	MEDIAWRITE_GETINFO_SM_AUDENTRY_ADDR = 0x83,   ///< p1: id(in), p2:SM_AUD_NIDXINFO(in), p3: MEDIAREC_ENTRY_INFO(out)
	MEDIAWRITE_GETINFO_SM_VIDENTRY_ADDR = 0x84,   ///< p1: id(in), p2:SM_VID_NIDXINFO(in), p3: MEDIAREC_ENTRY_INFO(out)
	MEDIAWRITE_GETINFO_SM_VID_POS       = 0x85,   ///< p1: id(in), p2:vfn(in), p3: file offset(out)
	MEDIAWRITE_GETINFO_SM_AUD_POS       = 0x86,   ///< p1: id(in), p2:vfn(in), p3: file offset(out)
	MEDIAWRITE_GETINFO_FRAMESIZE        = 0x87,   ///< p1: vfr(in), p2: samplerate(in), p3: sec(in)
	MEDIAWRITE_GETINFO_TRUNCATESIZE     = 0x88,   ///< p1: new truncate size(in)
	MEDIAWRITE_GETINFO_TRUNCATESIZE_H   = 0x89,   ///< p1: new truncate size(in) HIGH 32bit

	MEDIAWRITE_GETINFO_USERDATA         = 0xB1,   ///< p1: userdata name(out), p2: userdata model(out)
	ENUM_DUMMY4WORD(MEDIAWRITE_GETINFO_TYPE)
} MEDIAWRITE_GETINFO_TYPE;


/**
    Set information to media fileformat writing library for making file header.
    format: ER (*SetInfo)(MEDIAWRITE_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum {
	MEDIAWRITE_SETINFO_WID_HEI          = 0x01, ///< p1:width(in), param2: height(in), p3:id (in)
	MEDIAWRITE_SETINFO_VID_FRAME        = 0x02, ///< p1:total video frame (in), p3:id (in)
	MEDIAWRITE_SETINFO_AUD_FRAME        = 0x03, ///< p1:total audio frame (in), p3:id (in)
	MEDIAWRITE_SETINFO_REC_DURATION     = 0x04, ///< p1:total duration microsecond(in), p3:id (in)
	MEDIAWRITE_SETINFO_FRAMEINFOBUF     = 0x05, ///< p1:addr(in), param2:size(in), p3:id (in)
	MEDIAWRITE_SETINFO_VID_FR           = 0x06, ///< p1:video frame rate(in), p3:id (in)
	MEDIAWRITE_SETINFO_AUD_SAMPLERATE   = 0x07, ///< p1:audio sample rate(in), p3:id (in)
	MEDIAWRITE_SETINFO_USERDATA         = 0x08, ///< p1:addr(in), param2:size(in), p3:id(in)
	MEDIAWRITE_SETINFO_AUD_CHS          = 0x09, ///< p1:channels (in), p3:id(in)
	MEDIAWRITE_SETINFO_AUD_BITRATE      = 0x0A, ///< p1:audio bit rate (in), for AAC, p3:id (in)
	MEDIAWRITE_SETINFO_FRE1DATA         = 0x0B, ///< p1:addr(in), param2:size(in), p3:id(in)
	MEDIAWRITE_SETINFO_RESET            = 0x0C, ///< p3:id (in)
	MEDIAWRITE_SETINFO_VIDEOTYPE        = 0x0D, ///< p1:MEDIAVIDENC_MJPG or others, p3:id (in)
	MEDIAWRITE_SETINFO_CHECKHDR         = 0x0E, ///< p2: outlen (out)
	MEDIAWRITE_SETINFO_CLUSTERSIZE      = 0x0F, ///< p1: cluster size (in), p3:id (in)
	MEDIAWRITE_SETINFO_UPDATEHDR        = 0x10, ///< no parameters
	MEDIAWRITE_SETINFO_CUTENTRY         = 0x11, ///< p1:(MEDIAREC_FLASHINFO *)(in), p3:id (in)
	MEDIAWRITE_SETINFO_FILETYPE         = 0x12, ///< p1: file type, MEDIA_FTYP_MOV/MEDIA_FTYP_MP4 (to distinguish MOV/MP4: same plug-in but different type), p3:id (in)
	MEDIAWRITE_SETINFO_SAVE_VF          = 0x13, ///< p1: frame num for video once writing to card, p3:id(in)
	//#NT#2012/08/15#Hideo Lin -begin
	//#NT#Add some information
	MEDIAWRITE_SETINFO_AUD_BITS         = 0x14, ///< p1: audio sample bits (in), p3:id(in)
	MEDIAWRITE_SETINFO_AUD_ENABLE       = 0x15, ///< p1: audio enable/disable (in), p3:id(in)
	//#NT#2012/08/15#Hideo Lin -end
	MEDIAWRITE_SETINFO_RECVRY_UPDATEHDR = 0x16, ///< p1: update 1st cluster addr
	MEDIAWRITE_SETINFO_TEMPIDX1ADDR     = 0x17, ///< p1: addr to write back header (in), p3:id (in)
	//MEDIAWRITE_SETINFO_MAKEIDX1DATA     = 0x18, ///< p1: (MEDIAREC_MAKEAVIIDX1 *)(in) make back header, output size
	MEDIAWRITE_SETINFO_RECVRY_FILESIZE  = 0x19, ///< p1: filesize before recovery (in)
	MEDIAWRITE_SETINFO_RECVRY_MEMBUF    = 0x1A, ///< p1: addr(in) for recovery, p2: size(in)
	MEDIAWRITE_SETINFO_THUMB_DATA       = 0x20, ///< p1: thumbnail data addr (in), p2: thumbnail data size (in), p3:id(in)
	MEDIAWRITE_SETINFO_HEADER_SIZE      = 0x21, ///< p1: header size (in) (usually times of cluster size), p3:id(in)
	MEDIAWRITE_SETINFO_CUSTOM_UDATA     = 0x22, ///< p1: customized user data TRUE/FALSE (in), p3:id(in)
	MEDIAWRITE_SETINFO_DAR              = 0x23, ///< p1: video display aspect ratio
	MEDIAWRITE_SETINFO_CUSTOMDATA       = 0x24, ///< p1: addr(in), param2:size(in), p3:id(in)
	MEDIAWRITE_SETINFO_GPSBUFFER        = 0x25, ///< p1: addr(in), param2:size(in), p3:id(in)
	MEDIAWRITE_SETINFO_GPSTAGBUFFER     = 0x26, ///< p1:addr(in), param2:size(in), p3:id(in)
	MEDIAWRITE_SETINFO_MEDIAVERSION     = 0x27, ///< p1:addr(in) length<=8

	MEDIAWRITE_SETINFO_MOVVIDEOENTRY    = 0x81, ///< mov: p1:num(in), p2:(MOV_Ientry *)(in) entry info, p3: id
	MEDIAWRITE_SETINFO_MOVAUDIOENTRY    = 0x82, ///< mov: p1:num(in), p2:(MOV_Ientry *)(in) entry info, p3: id
	MEDIAWRITE_SETINFO_MOVMDATSIZE      = 0x83, ///< mov: p1:mdatsize(in), p3:id (in)
	MEDIAWRITE_SETINFO_TEMPMOOVADDR     = 0x84, ///< mov: p1:(UINT8 *)(in) addr to put temp moov, p3:id (in)
	MEDIAWRITE_SETINFO_AUDIOSIZE        = 0x85, ///< mov: p1:total audio data size(in), p3:id(in)
	MEDIAWRITE_SETINTO_MOVGPSENTRY      = 0x86, ///< mov: p1:num(in), p2:(MOV_Ientry *)(in) entry info, p3: id
	MEDIAWRITE_SETINTO_MOV_264DESC      = 0x87, ///< mov: p1:264 video description addr(in), p2: len (in), p3:id(in)
	MEDIAWRITE_SETINFO_AUDIOTYPE        = 0x89, ///< mov: p1:audio codec type, MOVAUDENC_PCM or others (in), p3:id (in)
	MEDIAWRITE_SETINFO_UPDATEVIDPOS     = 0x8A, ///< mov: p1:num(in), p2:(MOV_Ientry *)(in). entry info to update, p3:id(in)
	MEDIAWRITE_SETINFO_UPDATEAUDPOS     = 0x8B, ///< mov: p1:num(in), p2:(MOV_Ientry *)(in). entry info to update, p3:id(in)
	MEDIAWRITE_SETINFO_H264GOPTYPE      = 0x8C, ///< mov: p1:GOP type, MEDIAREC_GOP_IPONLY or others (in), p3:id(in)
	MEDIAWRITE_SETINFO_MOVHDR_REVSIZE   = 0x8D, ///< mov: p1:mov header reserved size (in) = MEDIAREC_AVI_HDR////2012/10/22 Meg
	MEDIAWRITE_SETINFO_MOV_ROTATEINFO   = 0x8E, ///< mov: p1:rotation matrix information(in), p3:id(in) ////2013/04/17 Calvin
	MEDIAWRITE_SETINFO_SCREEN_DATA      = 0x8F, ///< mov: screennail data addr (in), p2: screennail data size (in)
	MEDIAWRITE_SETINFO_AVIVIDPOS        = 0x91, ///< avi: p1:num(in), p2:(AVI_Entry *)(in), p3:id(in)
	MEDIAWRITE_SETINFO_AVIAUDPOS        = 0x92, ///< avi: p1:num(in), p2:(AVI_Entry *)(in), p3:id(in)
	MEDIAWRITE_SETINFO_NOWPOS           = 0x94, ///< avi: p1:now fileposition(in), p3:id (in)
	MEDIAWRITE_SETINFO_H264DESC         = 0x95, ///< avi: p1:h264desc(in), p2: h264descSize(in), p3:id(in)
	MEDIAWRITE_SETINFO_PADAVISIZE       = 0x96, ///< avi: p1:frame num(in), param2:size(in), param3:id(in)
	MEDIAWRITE_SETINFO_NIDXDATA         = 0x97, ///< p1:nidx info (MEDIAREC_NIDXINFO *)(in), p3: id(in)
	MEDIAWRITE_SETINFO_GPSDATA          = 0x99, ///< avi: p1:gps info (MEDIAREC_GPSINFO *)(in), p3:id(in)
	MEDIAWRITE_SETINFO_SM_NIDXDATA      = 0x9A, ///< p1:nidx info (MEDIAREC_NIDXINFO *)(in), p3: id(in)
	MEDIAWRITE_SETINFO_2015_NIDXDATA    = 0x9B, ///< p1:nidx info (MEDIAREC_NIDXINFO *)(in), p3: id(in)
	MEDIAWRITE_SETINFO_TIMESPECIALID    = 0x9C, ///< p1:timeid(in), p3: id(in)
	MEDIAWRITE_SETINFO_EN_CO64          = 0x9D, ///< p1:enable or not (in)

	MEDIAWRITE_SETINFO_MP4VIDEOENTRY    = 0xA1, ///< mp4: p1:num(in), p2:(MOV_Ientry *)(in)
	MEDIAWRITE_SETINFO_MP4AUDIOENTRY    = 0xA2, ///< mp4: p1:num(in), p2:(MOV_Ientry *)(in)
	MEDIAWRITE_SETINFO_MP4MDATSIZE      = 0xA3, ///< mp4: p1:mdatsize(in)
	MEDIAWRITE_SETINFO_MP4TEMPMOOVADDR  = 0xA4, ///< mp4: p1:(UINT8 *)(in) addr to put temp moov
	MEDIAWRITE_SETINFO_MP4AUDIOSIZE     = 0xA5, ///< mp4: p1:total audio data size(in)
	MEDIAWRITE_SETINFO_MP4_TEMPHDR      = 0xA6, ///< mov: p1:temp hdr addr (in), p3: id (in)
	MEDIAWRITE_SETINFO_H265DESC         = 0xA7, ///< mp4: p1:adr(in), p2:size(in), p3:id(in)
    MEDIAWRITE_SETINFO_MOOV_LAYER1DATA  = 0xA8, ///< mp4: p1:layer1 adr(in) p2: layer1 size (in), p3: id
    MEDIAWRITE_SETINFO_COPYTEMPHDR      = 0xA9, ///< mp4: p1:layer1 adr(in) p2: layer1 size (in), p3: id

	MEDIAWRITE_SETINFO_PESHEADERADDR    = 0xB1, ///< ts:  p1:pes header src addr (in),p2: pes header payload size (in)
	MEDIAWRITE_SETINFO_MAKEPESHEADER    = 0xB2, ///< ts:  p1:id type (in)
	MEDIAWRITE_SETINFO_RESETPTS         = 0xB3,
	MEDIAWRITE_SETINFO_VIDEOFRAMERATE   = 0xB4, ///< ts:  p1:video frame rate (in)
	ENUM_DUMMY4WORD(MEDIAWRITE_SETINFO_TYPE)
} MEDIAWRITE_SETINFO_TYPE;

//#NT#2012/08/15#Calvin Chang#Support Media File Recovery Function - begin
/**
    Customize function flag definition
    ER (*CustomizeFunc)(UINT32 type, void *pobj);
*/
#define MEDIAWRITELIB_CUSTOM_FUNC_IDXTLB_RECOVERY     0x01//param: UINT32 id (in)
//#NT#2012/08/15#Calvin Chang -end

#define MEDIAWRITE_H265_STSD_MAXLEN                   0x80
//#NT#2013/02/26#Meg Lin -begin
/**
    Set information to media naming handler library.
    format: ER (*SetInfo)(MEDIANAMING_SETINFO_TYPE type, UINT32 p1, UINT32 p2, UINT32 p3);
*/
typedef enum {
	MEDIANAMING_SETINFO_SAMECRASHFOLDER = 0x01, ///< p1: yesOrNo(in)
	MEDIANAMING_SETINFO_SET1STCRASH     = 0x02, ///< no param
	MEDIANAMING_SETINFO_SETCRASHNAME    = 0x03, ///< p1: (char *)CrashName (in)
	MEDIANAMING_SETINFO_WORKINGHDL      = 0x81, ///< p1: (FILEDB_HANDLE) fileDBhandle (in)
	//MEDIANAMING_SETINFO_ROOTFOLDERNAME  = 0x82, ///< p1: (char *)root folderName (in)
	MEDIANAMING_SETINFO_PATH_ENDCHAR    = 0x83, ///< p1: char 'A'
	MEDIANAMING_SETINFO_OPENDEBUGMSG    = 0x84,
	MEDIANAMING_SETINFO_SETFILETYPE     = 0x85,
	MEDIANAMING_SETINFO_SETNOWPATHID    = 0x86, ///< p1: pathid, p2:emr!!
	MEDIANAMING_SETINFO_SETEMRPATHID    = 0x87, ///< p1: emr pathid
	MEDIANAMING_SETINFO_SETEMRSAVETYPE  = 0x88, ///< p1: emr savetype, defautlt SAVE1
	MEDIANAMING_SETINFO_SET2NDSAVETYPE  = 0x89, ///< p1: 2nd savetype, defautlt SAVE1
	ENUM_DUMMY4WORD(MEDIANAMING_SETINFO_TYPE)
} MEDIANAMING_SETINFO_TYPE;
//#NT#2013/02/26#Meg Lin -end

/**
    Callback function type for media recording. Event types are defined in MediaRecAPI.h.
*/

/**
    Media Recorder event callback
*/
typedef void (MediaRecCallbackType)(UINT32 event_id, UINT32 Seconds);

/**
    Media Recorder drawing string callback
*/
typedef void (MediaRecDrawStr)(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineoftY, UINT32 drawWidth);//2010/09/07 Meg Lin

/**
    Media Recorder IPL change mode callback
*/
typedef void (MediaRecIPLChangeCB)(UINT32 mode, UINT32 param);//2012/08/30 Meg Lin

/**
    Media Recorder IPL get ready buffer callback
*/
typedef void (MediaRecIPLGetReadyCB)(UINT32 mode, UINT32 param);//2012/08/30 Meg Lin

/**
    Media Recorder IPL trigger callback
*/
typedef void (MediaRecIPLTriggerCB)(UINT32 uiVidEncId, UINT32 param);

/**
    Media Recorder immediately process callback
*/
typedef void (MediaRecImmediatelyProc)(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);//2012/09/12 Meg Lin

/**
    Callback function type for media recording. Event types are defined in MediaRecAPI.h.
*/
typedef void (RawEncCallbackType)(UINT32 uiEventID, UINT32 uiPathID);

/**
    Callback function type for RawEncode
*/
typedef INT32(*FSWRITECB)(UINT32 Addr, UINT32 Size, UINT32 Fmt, UINT32 uiPathID);


/**
    Structure definition for Media recording flash info.
    (for MEDIAREC_OBJ)
*/
typedef struct {
	UINT32 fstVideoPos;     ///< [in] first video offset
	UINT32 fstAudioPos;     ///< [in] first audio offset
	UINT32 startSec;        ///< [in] real start second
	UINT32 flashSec;        ///< [in] flash second
	UINT32 frameRate;       ///< [in] video frame rate
} MEDIAREC_FLASHINFO;

/**
    Structure definition for Media recording quick info.
    (for MEDIAREC_OBJ)
*/
typedef struct {
	UINT32              uiQVMemAddr;      ///< [in]  quick view output addr
	UINT32              uiQVActualSize;   ///< [out] quick view picutre size
	UINT8               ucQVEnable;       ///< [in]  quick view enable or not
	UINT8               rev;
	UINT16              rev2;
} MEDIAREC_QVINFO;

/**
    Structure definition for Media recording user data.
    (for MEDIAREC_OBJ)
*/
typedef struct {
	UINT16              uiDisplayWidth;   ///< display width
	UINT16              uiDisplayHeight;  ///< display height
	UINT16              uiThumbWidth;     ///< thumbnail width
	UINT16              uiThumbHeight;    ///< thumbnail height
	UINT32              uiThumbLen;       ///< [out] thumbnail length
	UINT32              uiUserDataBuf;    ///< user data buffer
	UINT32              uiUserDataSize;   ///< size of user data buffer
	UINT8               ucUserDataEmbed;  ///< encode userdata embeded or not
	UINT8               rev;
	UINT16              rev2;
} MEDIAREC_USERDATA;

/**
    Structure definition for Media recording rate control.
    (for MEDIAREC_OBJ)
*/
typedef struct {
	UINT32                  uiTargetBitrate;    ///< video data bytes per second
	UINT8                   ucEnableBRCtrl;     ///< enable rate control or not
	UINT8                   rev8;
	UINT16                  rev16;
} MEDIAREC_BRCTRL;

/**
    Structure definition for Media recording file limit.
    (for MEDIAREC_OBJ)
*/
typedef struct {
	UINT32                  uiMaxRecSecs;       ///< if not zero, stop when recording time is enough
	UINT32                  uiMaxRecFileSize;   ///< if not zero, stop when recording size is enough
} MEDIAREC_FILELIMIT;

/**
    Structure definition for Media nidx info.
    Nidx info is used for power-off protection.
*/
typedef struct {
	UINT32 dataoutAddr;         ///< [in] address to put nidx data
	//UINT32 thisVideoStartPos;   ///< [in] this second video data starting fileoffset
	//UINT32 thisVideoSize;       ///< [in] this second video data size
	UINT32 thisVideoStartFN;    ///< [in] this second video starting frame number
	UINT32 thisVideoFN;         ///< [in] this second video frame number
	UINT32 thisVideoFrameData;  ///< [in] this second 1st video frame entry addr
	UINT32 thisVideoFrmDataLen; ///< [in] this second video frame info len
	UINT32 thisAudioStartPos;   ///< [in] this second audio data starting fileoffset
	UINT32 thisAudioSize;       ///< [in] this second audio data size
	UINT32 lastNidxdataPos;     ///< [in] last nidx fileoffset
	//for mov
	UINT32 videoWid;            ///< [in] video width
	UINT32 videoHei;            ///< [in] video height
	UINT32 videoCodec;          ///< [in] MEDIAVIDENC_MJPG or MEDIAVIDENC_H264
	UINT32 audioSampleRate;     ///< [in] audio sample rate
	UINT32 audioChannel;        ///< [in] channels
	UINT32 h264descLen;         ///< [in] h264 desc length
	UINT32 h264descData;        ///< [in] h264 desc data

	UINT32 headerSize;          ///< [in] header size, avi:including movi, mov: including mdat
	UINT32 versionCode;         ///< [in] versionCode, for recover-identification
	UINT32 clusterSize;         ///< [in] clustersize, if move file to other card, recovery must ok

} MEDIAREC_NIDXINFO;//order must be the same with MEDIAREC_NIDXAVI

/**
    Structure definition for SMedia nidx info.
    Nidx info is used for power-off protection.
*/
typedef struct {
	UINT32 dataoutAddr;         ///< [in] address to put nidx data
	UINT32 thisVideoStartFN;    ///< [in] this second video starting frame number
	UINT32 thisVideoFN;         ///< [in] this second video frame number
	UINT32 thisVideoFrameData;  ///< [in] this second 1st video frame entry addr
	UINT32 thisVideoFrmDataLen; ///< [in] this second video frame info len
	UINT32 thisAudioFrameData;  ///< [in] this second audio data starting fileoffset
	UINT32 thisAudioFrmDataLen; ///< [in] this second audio frame info len
	UINT64 lastNidxdataPos64;     ///< [in] last nidx fileoffset
	UINT32 thisAudioStartFN;    ///< [in] this second audio starting frame number
	UINT32 thisAudioFN;         ///< [in] last second audio starting frame number
	UINT32 audioCodec;          ///< [in] MOVAUDENC_AAC or MOVAUDENC_PCM

	//for mov
	UINT32 videoVfr;            ///< [in] video frame rate (round)
	UINT32 videoWid;            ///< [in] video width
	UINT32 videoHei;            ///< [in] video height
	UINT32 videoCodec;          ///< [in] MEDIAVIDENC_MJPG or MEDIAVIDENC_H264
	UINT32 audioSampleRate;     ///< [in] audio sample rate
	UINT32 audioChannel;        ///< [in] channels
	UINT32 h264descLen;         ///< [in] h264 desc length
	UINT32 h264descData;        ///< [in] h264 desc data

	UINT32 headerSize;          ///< [in] header size, avi:including movi, mov: including mdat
	UINT32 versionCode;         ///< [in] versionCode, for recover-identification
	UINT32 clusterSize;         ///< [in] NIDX blocksize, SMEDIAREC_MAX_NIDX_BLK(0x2000)
	UINT32 en_co64;             ///< [in] enable co64 (MOVENTRY=16), if not (MOVENTRY= 12)

} SM_REC_CO64_NIDXINFO;//order must be the same with MEDIAREC_NIDXAVI

/**
    Structure definition for SMedia nidx info.
    Nidx info is used for power-off protection.
*/
typedef struct {
	UINT32 dataoutAddr;         ///< [in] address to put nidx data
	UINT32 thisVideoStartFN;    ///< [in] this second video starting frame number
	UINT32 thisVideoFN;         ///< [in] this second video frame number
	UINT32 thisVideoFrameData;  ///< [in] this second 1st video frame entry addr
	UINT32 thisVideoFrmDataLen; ///< [in] this second video frame info len
	UINT32 thisAudioFrameData;  ///< [in] this second audio data starting fileoffset
	UINT32 thisAudioFrmDataLen; ///< [in] this second audio frame info len
	UINT32 lastNidxdataPos;     ///< [in] last nidx fileoffset
	UINT32 thisAudioStartFN;    ///< [in] this second audio starting frame number
	UINT32 thisAudioFN;         ///< [in] last second audio starting frame number
	UINT32 audioCodec;          ///< [in] MOVAUDENC_AAC or MOVAUDENC_PCM

	//for mov
	UINT32 videoVfr;            ///< [in] video frame rate (round)
	UINT32 videoWid;            ///< [in] video width
	UINT32 videoHei;            ///< [in] video height
	UINT32 videoCodec;          ///< [in] MEDIAVIDENC_MJPG or MEDIAVIDENC_H264
	UINT32 audioSampleRate;     ///< [in] audio sample rate
	UINT32 audioChannel;        ///< [in] channels
	UINT32 h264descLen;         ///< [in] h264 desc length
	UINT32 h264descData;        ///< [in] h264 desc data

	UINT32 headerSize;          ///< [in] header size, avi:including movi, mov: including mdat
	UINT32 versionCode;         ///< [in] versionCode, for recover-identification
	UINT32 clusterSize;         ///< [in] NIDX blocksize, SMEDIAREC_MAX_NIDX_BLK(0x2000)

} SM_REC_NIDXINFO;//order must be the same with MEDIAREC_NIDXAVI

//#NT#2012/09/11#Meg Lin -end

/**
    Structure definition for Media nidx output data.
*/
typedef struct {
	UINT32 outsize;             ///< [out] output back header size

} MEDIAREC_MAKEAVIIDX1;

/**
    Structure definition for Media recovery data.
*/
typedef struct {
	UINT32 outAddr;                     ///< [in] output address
	UINT32 clustersize;                 ///< [in] cluster size
	UINT32 oldFilesize;                 ///< [in] old filesize

} MEDIAREC_RECVFNTHDR;//recover front header

//#NT#2011/06/23#Meg Lin -begin
typedef struct {
	UINT32 bufAddr;             ///< [in] address to put gps data
	UINT32 clusterSize;         ///< [in] this tag size
	UINT32 gpsDataAddr;         ///< [in] gpsData addr
	UINT32 gpsDataSize;         ///< [in] gpsData size

} MEDIAREC_GPSINFO;

typedef struct {
	UINT32 junkCC;      //oft: 0
	UINT32 junkSize;    //oft: 4
	UINT32 gpsCC;       //oft: 8
	UINT32 gpsSize;     //oft: c
	UINT32 gpsData;     //GPS data starting pointer
} MEDIAREC_GPSAVI;
//#NT#2011/06/23#Meg Lin -end


//#NT#2012/08/30#Meg Lin -begin
/**
    Structure definition for media recording operation.
    (for MediaRec_Open() )
*/
typedef struct {
	MediaRecCallbackType    *CallBackFunc;      ///< event inform callback
	MediaRecDrawStr         *CallbackDrawFunc;  ///< callback draw func (for D2D stamp)
	AUDFILTER_FILTER_CB     fpAudPostProcess;   ///< callback function for audio post process
	RawEncCallbackType      *RawCallBackFunc;   ///< event inform callback
	MediaRecIPLChangeCB     *IPLChangeCB;       ///< IPL change mode
	MediaRecIPLGetReadyCB   *IPLGetReadyCB;     ///< IPL get ready buffer callback
	MediaRecIPLGetReadyCB   *IPLGetReadyCB_2;   ///< IPL get ready buffer callback
	MediaRecIPLTriggerCB    *IPLTriggerCB;      ///< callback to trigger IPL direct path
	MediaRecImmediatelyProc *ImmProc;           ///< Immediately process when media recorder callback//2012/09/12 Meg
	FSWRITECB               JpgWriteFileFP;     ///< write file function pointer
	UINT32                  version;            ///< mediaRec version MEDIAREC_VER_2_0
} MEDIAREC_OBJ;

typedef struct {
	UINT32              uiVidCodec;         ///< recording video codec, MEDIAREC_ENC_JPG, MEDIAREC_ENC_H264
	UINT32              uiVidFrameRate;     ///< recording video frame rate, MEDIAREC_VRATE_30
	UINT32              uiWidth;            ///< now recording video width
	UINT32              uiHeight;           ///< now recording video height
	UINT32              uiAudSampleRate;    ///< recording audio sample rate, AUDIO_SR_8000
	UINT32              uiAudBits;          ///< audio setting, bits per sample, 8 or 16
	UINT32              uiAudChannels;      ///< audio channel number, 1 or 2
	UINT32              uiThumbAddr;        ///< thumbnail bit-stream starting address
	UINT32              uiThumbSize;        ///< thumbnail bit-stream size
	UINT32              uiFre1Addr;         ///< thumbnail bit-stream starting address
	UINT32              uiFre1Size;         ///< thumbnail bit-stream size
	UINT32              uiScreenAddr;        ///< screennail bit-stream starting address//2013/05/28 Meg
	UINT32              uiScreenSize;        ///< screennail bit-stream size//2013/05/28 Meg
	UINT32              uiFreaSize;         ///< [out] frea size for mov
	UINT32              uiHeaderSize;       ///< max header size (set from MediaRecorder)
	UINT32              uiUdataAddr;        ///< [AVI] user data starting address
	UINT32              uiUdataSize;        ///< [AVI] user data size
	UINT32              uiMakerID;          ///< [AVI] ID, which file
	UINT32              uiFileType;         ///< [MOV] MEDIA_FTYP_MP4 or MEDIA_FTYP_MOV
	UINT32              uiRotationInfo;     ///< rotation information in Mov/Mp4 file
	UINT32              ui1st;              ///< 1st
	BOOL                bAudioEn;           ///< audio enable, TRUE, FALSE
	BOOL                bCustomUdata;       ///< customized user data
	MEDIAREC_DAR     	uiDAR;              ///< video display aspect ratio
} MEDIAREC_HDR_OBJ;

typedef struct {
	UINT32  entryAddr;
	UINT32  entrySize;
} MEDIAREC_ENTRY_INFO;



/**
    Structure definition for media fileformat writing container.
    (for MediaRecCreateContainerObj()) For example, we can get mov recording container by
    mov_getContainerMaker(), and set to gMediaRecMaker.
*/
typedef struct {
	ER(*Initialize)(UINT32 idnum);                           ///< initializing fileformat container
	ER(*SetMemBuf)(UINT32 idnum, UINT32 startAddr, UINT32 size);   ///< set memory to fileformat container
	ER(*MakeHeader)(UINT32 hdrAddr, UINT32 *pSize, MEDIAREC_HDR_OBJ *pobj);  ///< make front-header using fileformat maker
	ER(*UpdateHeader)(UINT32 idnum, void *pobj);      ///< make end-header using fileformat maker
	ER(*GetInfo)(MEDIAWRITE_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);  ///< get information from media fileformat container.
	ER(*SetInfo)(MEDIAWRITE_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);        ///< set information to media fileformat container.
	ER(*CustomizeFunc)(UINT32 type, void *pobj);     ///< custimized function if neeeded
	ER(*cbWriteFile)(UINT8 *type, UINT64 pos, UINT32 size, UINT32 addr);  ///< a file-writing function for fileformat maker
	ER(*cbReadFile)(UINT8 *type, UINT64 pos, UINT32 size, UINT32 addr);   ///< a file-reading function for fileformat maker
	void (*cbShowErrMsg)(char *fmtstr, ...);         ///< a debug function for fileformat maker
	UINT32 checkID;                                  ///< checkID for fileformat maker to check valid type
	UINT32 id;
} CONTAINERMAKER, *PCONTAINERMAKER;

/**
    Structure definition for media estimating valid video number.
    To calculate valid recording second.
*/
typedef struct {
	UINT32 vfr; //video frame rate
	UINT32 afr; //audio frame rate, 1 if PCM
	UINT32 size;//valid buffer size
} MP_ESTIMATE_VNUM;


//#NT#2013/02/26#Meg Lin -begin
/**
    Structure definition for media naming handler.
    (for MediaRecFS_CreateNamingHandler()) For example, we can get DCF naming handler by
    mediadcf_getNamingHandler(), and set to gMediaRecNamingHdl.
*/
typedef struct {
	ER(*CreateAndGetPath)(UINT32 filetype, CHAR *pPath);
	ER(*AddPath)(CHAR *pPath, UINT32 attrib);
	ER(*DelPath)(CHAR *pPath);
	ER(*GetPathBySeq)(UINT32 uiSequ, CHAR *pPath);
	ER(*CalcNextID)(void);
	ER(*GetLastPath)(CHAR *pPath);
	ER(*GetNewCrashPath)(CHAR *pPath, UINT8 setActive);
	ER(*CustomFunc)(UINT32 filetype, UINT32 p1, UINT32 p2, UINT32 p3);
	void (*SetInfo)(MEDIANAMING_SETINFO_TYPE type, UINT32 p1, UINT32 p2, UINT32 p3);
	UINT32 checkID;                                  ///< checkID for fileformat maker to check valid type
} MEDIANAMINGRULE, *PMEDIANAMINGRULE;
//#NT#2013/02/26#Meg Lin -end

typedef struct {
	UINT32 addr;
	UINT32 size;
	UINT32 tag;

} CUSTOMDATA, *PCUSTOMDATA;


typedef struct {
	UINT32 nowAfn;  //now afn
	UINT32 thisAfn; //this second Afn
} SM_AUD_NIDXINFO;

typedef struct {
	UINT32 nowVfn;  //now vfn
	UINT32 thisVfn; //this second Vfn
} SM_VID_NIDXINFO;


/**
    @name type for CustomizeFunc() in MEDIANAMINGRULE

    type for CustomizeFunc() in MEDIANAMINGRULE.
*/
//@{
#define MEDIANR_CUSTOM_CHANGECRASH      0x01 ///< p1: CHAR *pPath (in/out)
#define MEDIANR_CUSTOM_UPDATECRASH      0x02 ///< p1: void
#define MEDIANR_CUSTOM_GETFILES         0x03 ///< p1: UINT32 *num
#define MEDIANR_CUSTOM_GETFILESIZE_BYSEQ 0x04 ///< p1: seq, p2: UINT32 *filesize (out), p3: UINT8 *attrib (out)
#define MEDIANR_CUSTOM_SET_FILESIZE     0x05 ///< p1: CHAR *pPath (in), p2: UINT32 filesize (in), p3: filesize_H (in)
#define MEDIANR_CUSTOM_GET_FILESIZE     0x06 ///< p1: CHAR *pPath (in), p2: UINT32 *filesize (out), p3: filesize_H (out)
#define MEDIANR_CUSTOM_GET_ATTRIB       0x07 ///< p1: CHAR *pPath (in), p2: UINT32 *attrib (out)
#define MEDIANR_CUSTOM_UPDATE_ATTRIB    0x08 ///< p1: CHAR *pPath (in), p2: UINT32 attrib (in)
#define MEDIANR_CUSTOM_CHANGE_TO_EMR    0x09 ///< p1: CHAR *pPath (in/out)
#define MEDIANR_CUSTOM_CHANGE_TO_EV     0x0A ///< p1: CHAR *pPath (in/out)
//@}

#endif//_MEDIAWRITELIB_H

//@}

