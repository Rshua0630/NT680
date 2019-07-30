/**
    AVI utility functions

    AVI utility functions

    @file       MP_AVI.h
    @ingroup    mIAVAVI
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _AVILIB_H
#define _AVILIB_H

#include "MediaReadLib.h"
#include "MediaWriteLib.h"
#include "AVFile_ParserAvi.h"
#include "AVFile_MakerAvi.h"

#define AVI_H264_GOPFN     15//H264 GOP frame number = i+14p = 15
#define AVI_H264_GOPFN20   20//H264 GOP frame number = i+19p = 15//2011/11/21 Meg

#define AVI_PARSEOK             0
#define AVI_ERRHEADER           (-1)
#define AVI_ERRDECODE           (-2)
#define AVI_ERRREAD             (-3)
#define AVI_ERRBIGFILE          (-4)

#define AVIWRITELIB_CHECKID  0x72696666//riff

#define AVI_Probe_RIFF  0x46464952

// AVI file 'four CC' literals

#define FOURCC_RIFF 0x46464952  // 'RIFF'
#define FOURCC_LIST 0x5453494c  // 'LIST'
#define FOURCC_AVI  0x20495641  // 'AVI '
#define FOURCC_HDRL 0x6c726468  // 'hdrl'
#define FOURCC_AVIH 0x68697661  // 'avih'
#define FOURCC_STRL 0x6c727473  // 'strl'
#define FOURCC_STRH 0x68727473  // 'strh'
#define FOURCC_STRF 0x66727473  // 'strf"
#define FOURCC_INFO 0x4f464e49  // 'INFO'
#define FOURCC_ISFT 0x54465349  // 'ISFT'
#define FOURCC_MOVI 0x69766f6d  // 'movi'
#define FOURCC_JUNK 0x4b4e554a  // 'JUNK'
#define FOURCC_IDX1 0x31786469  // 'idx1'
#define FOURCC_VIDS 0x73646976  // 'vids'
#define FOURCC_AUDS 0x73647561  // 'auds'
#define FOURCC_mjpg 0x67706a6d  // 'mjpg'
#define FOURCC_MJPG 0x47504a4d  // 'MJPG'
#define FOURCC_00DC 0x63643030  // '00dc'
#define FOURCC_01WB 0x62773130  // '01wb'
#define FOURCC_WB01 0x31306277  // 'wb01'
#define FOURCC_01WB 0x62773130  // '01wb'
#define FOURCC_WAVE 0x45564157  // 'MOVE'
#define FOURCC_DATA 0x61746164  // 'data'
#define FOURCC_WAVE 0x45564157  // 'WAVE'
#define FOURCC_FMT  0x20746d66  // 'fmt '
#define FOURCC_IDIT 0x54494449  // 'IDIT'       //DateTimeOriginal
#define FOURCC_H264 0x34363248  // 'H264'
#define FOURCC_NIDX 0x7864696e  // 'nidx'       //2011/02/18 Meg Lin
#define FOURCC_GPS  0x20535047  // 'GPS '       //2011/06/23 Meg Lin
//#NT#2012/08/22#Hideo Lin -begin
//#NT#For thumbnail and user data
#define FOURCC_THUM 0x6d756874  // 'thum'
#define FOURCC_USER 0x72657375  // 'user'
//#NT#2012/08/22#Hideo Lin -end
#define FOURCC_VER101 0x31303176 // 'v101'     //2012/09/11 Meg Lin
#define FOURCC_CLUR   0x72756c63 // 'clur'     //2012/09/12 Meg

typedef struct {
	UINT64 pos;
	UINT32 size;
} AVI_Entry;
#define AVIENTRY_SIZE 8//8 bytes//2012/09/11 Meg Lin

//
// here define some default value of record avi file
//
#define     AVIDEFAULT_VIDEO_FR         30// 15 fps
#define     AVIDEFAULT_WIDTH            320
#define     AVIDEFAULT_HEIGHT           240
#define     AVIDEFAULT_AUDIO_RATE       8000     //AUDIO_SR_8000    // 8000 //8064 //11025     // AUDIO Simple RATE
#define     AVIDEFAULT_AUDIO_SIZE       16000    // AVIDEFAULT_AUDIO_CHANNEL * (AVIDEFAULT_AUDIO_BITS / 8) * AVIDEFAULT_AUDIO_RATE
#define     AVIDEFAULT_AUDIO_SAMP_BIT   16       // 16 bits per sample

//Functions
extern ER MP_AVIReadLib_RegisterObjCB(void *pobj);
extern ER MP_AviWriteLib_Initialize(UINT32 id);
extern ER MP_AviWriteLib_SetMemBuf(UINT32 id, UINT32 startAddr, UINT32 size);
extern ER MP_AviWriteLib_RegisterObjCB(void *pobj);
extern ER MP_AviWriteLib_MakeHeader(UINT32 hdrAddr, UINT32 *pSize, MEDIAREC_HDR_OBJ *pobj);
extern ER MP_AviWriteLib_GetInfo(MEDIAWRITE_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
extern ER MP_AviWriteLib_SetInfo(MEDIAWRITE_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
extern ER MP_AviWriteLib_UpdateHeader(UINT32 type, void *pobj);
extern ER MP_AviWriteLib_CustomizeFunc(UINT32 type, void *pobj);
extern void MP_AVIUti_ShowAllSize(void);

#endif

