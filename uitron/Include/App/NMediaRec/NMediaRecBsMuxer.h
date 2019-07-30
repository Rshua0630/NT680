#ifndef _NMEDIARECBSMUXER_H
#define _NMEDIARECBSMUXER_H

#include "NvtMediaInterface.h"
#include "MOVLib.h"
#define NEW_STATE   		 ENABLE
#define BSMUX_NEW_FILEOUT    ENABLE

#define BXMUER_CALCSEC_UNKNOWN_FSSIZE (UINT64)0xFFFFFFFFFFFFFFFF

typedef struct {
	UINT64     nowFSFreeSpace;      //in: free space
	UINT32     vidfps;              //in: video frame per second
	UINT32     vidTBR;              //in: now video target bit rate
	UINT32     audSampleRate;       //in: audio sample rate
	UINT32     audChs;              //in: ahdio channels
	UINT32     gpson;               //in: if gps on
	UINT32     nidxon;              //in: if nidx on
} NMEDIAREC_CALCSEC_SETTING;

typedef struct {
	UINT64    givenSpace;  //in: free space
	NMEDIAREC_CALCSEC_SETTING info[8];
} NMEDIAREC_CALCSEC_SETTING2;
/**
    Media BSQ type definition

    Media BSQ type definition
*/
typedef enum {
	NMEDIA_REC_BSQTYPE_VIDEO,           ///< video bitstream data
	NMEDIA_REC_BSQTYPE_AUDIO,           ///< audio bitstream data
	NMEDIA_REC_BSQTYPE_NIDX,            ///< nidx bitstream data
	NMEDIA_REC_BSQTYPE_GPS,             ///< gps bitstream data
	NMEDIA_REC_BSQTYPE_USER,            ///< user data
	NMEDIA_REC_BSQTYPE_THUMB,           ///< user data
	ENUM_DUMMY4WORD(NMEDIA_REC_BSQ_TYPE)
} NMEDIA_REC_BSQ_TYPE;

enum {
	//Basic Settings
	BSMUXER_PARAM_ALLOC_SIZE,//get min buffer
	BSMUXER_PARAM_MEM_RANGE, //set buffer
	BSMUXER_PARAM_MAX_FILE,  //set max files at the same time

	BSMUXER_PARAM_GPS_ON,       //GPS on/off
	BSMUXER_PARAM_GPS_ADDR,     //gps data addr
	BSMUXER_PARAM_GPS_SIZE,     //gps data size

	BSMUXER_PARAM_THUMB_ADDR,   //thumb addr
	BSMUXER_PARAM_THUMB_SIZE,   //thumb size

	BSMUXER_PARAM_VID_WIDTH,    //vid wid
	BSMUXER_PARAM_VID_HEIGHT,   //vid height
	BSMUXER_PARAM_VID_VFR,      //vid frame rate
	BSMUXER_PARAM_VID_TBR,      //vid target bitrate
	BSMUXER_PARAM_VID_CODECTYPE,    //vid codectype
	BSMUXER_PARAM_VID_DESCADDR,     //vid desc addr
	BSMUXER_PARAM_VID_DESCSIZE,     //vid desc size
	BSMUXER_PARAM_VID_DAR,     //vid display aspect ratio, MP_VDOENC_DAR_DEFAULT

	BSMUXER_PARAM_AUD_CODECTYPE,//MOVAUDENC_AAC or others
	BSMUXER_PARAM_AUD_SR,       //audio sample rate
	BSMUXER_PARAM_AUD_CHS,      //audio channels
	BSMUXER_PARAM_AUD_ON,       //audio on/off

	BSMUXER_PARAM_FILE_EMRON,   //emr on
	BSMUXER_PARAM_FILE_EMRLOOP,   //emrloop on
	BSMUXER_PARAM_FILE_STRGID,  //storage id (0 or 1)
	BSMUXER_PARAM_FILE_SEAMLESSSEC,     //seamlessSec
	BSMUXER_PARAM_FILE_ROLLBACKSEC,     //rollbacksec (default:1)
	BSMUXER_PARAM_FILE_KEEPSEC, //keepsec (only for EMR)
	BSMUXER_PARAM_FILE_ENDTYPE, //endtype, MOVREC_ENDTYPE_CUTOVERLAP or others
	BSMUXER_PARAM_FILE_FILETYPE,//filetype, MEDIA_FILEFORMAT_MP4 or others
	BSMUXER_PARAM_FILE_RECFORMAT,//recformat, MEDIAREC_AUD_VID_BOTH or others
    BSMUXER_PARAM_FILE_PLAYFRAMERATE, //default as 30
	BSMUXER_PARAM_FILE_BUFRESSEC,    //buffer reserved sec, default 5 (5~30)

	BSMUXER_PARAM_OVERLAP_ON,    //overlap on/off, default OFF
	BSMUXER_PARAM_PRECALC_BUFFER,    //buffer to calculat min bsmuxer second
	BSMUXER_PARAM_EN_AUD_ADTS,    //audio BS ADTS shift bytes. default =0
	BSMUXER_PARAM_EN_POINTTHM,
	BSMUXER_PARAM_MAX_SIZE,

};

typedef enum {
	BSMUXER_ACTION_START,
	BSMUXER_ACTION_STOP,
	BSMUXER_ACTION_MAX,
} BSMUXER_ACTION;


#define BSMUXER_RESULT_NORMAL 			0
#define BSMUXER_RESULT_SLOWCARD 		1
#define BSMUXER_RESULT_LOOPREC_FULL  	2
#define BSMUXER_RESULT_CARD_FULL  		3

/**
     @name BsMux file event definitions
*/
#define BSMUXER_FEVENT_NORMAL		0
#define BSMUXER_FEVENT_EMR		    1


/**
     @name BsMux file operation definitions (bitwise)
*/
#define BSMUXER_FOP_NONE       0x00000000 ///< Do nothing. Used for event only
#define BSMUXER_FOP_CREATE     0x00000001 ///< Create a new file (if old one exists, it will be truncated)
#define BSMUXER_FOP_CLOSE      0x00000002 ///< Close the file
#define BSMUXER_FOP_CONT_WRITE 0x00000004 ///< Write continously (from current position)
#define BSMUXER_FOP_SEEK_WRITE 0x00000008 ///< Write after seeking
#define BSMUXER_FOP_FLUSH      0x00000010 ///< Flush the data right away after writing
#define BSMUXER_FOP_DISCARD    0x00000100 ///< Discard operations which not processed yet

typedef struct _BSMUXER_FILE_BUF {
	UINT32 				pathID;					   ///< keep path id
	UINT32              event;                     ///< BSMux event
	UINT32              fileop;                    ///< bitwise: open/close/conti_write or seek_write/flush/none(event only)/discard
	UINT32              addr;                      ///< write data address
	UINT64              size;                      ///< write data size
	UINT64              pos;                       ///< only valid if seek_write
	UINT32              type;                      ///< file type, MP4, TS, JPG, THM
} BSMUXER_FILE_BUF, *PBSMUXER_FILE_BUF;


typedef enum {
	BSMUXER_CBEVENT_BSDONE, //p1: bufID, p2: type
	BSMUXER_CBEVENT_MDATREADY, //p1: addr, p2: size, p3: portid
	BSMUXER_CBEVENT_KICKTHUMB, //p1: mdatid
	BSMUXER_CBEVENT_SLOWCARD, //p1: mdatid
	BSMUXER_CBEVENT_LOOPREC_FULL, //p1: mdatid
	BSMUXER_CBEVENT_CUT_COMPLETE, //p1: mdatid
	BSMUXER_CBEVENT_OVERTIME, //p1: mdatid
	BSMUXER_CBEVENT_MAXSIZE,	// for FAT,max file size is 4G
	BSMUXER_CBEVENT_CLOSE_RESULT = 0x100, //p1: pointer of BSMUXER_CALLBACK_INFO

} BSMUXER_CALLBACKEVENT;

typedef struct {
	UINT32  bSMemAddr;    // in: bitstream address saving in queue
	UINT32  bSSize;       // in: bitstream size
	UINT32  type;         // in: NMEDIA_REC_BSQTYPE_VIDEO or others
	UINT32  isKey;        // in: video is key frame or not
	UINT32  isIDR2Cut;    // in: video is idr to cut here
	UINT32  bufid;        // in: buffer id
} BSMUXER_MDATDATA, *PBSMUXER_MDATDATA;

typedef struct {
	UINT32 Addr;    ///< Memory buffer starting address
	UINT32 Size;    ///< Memory buffer size
} BSMUXER_MEM_RANGE, *PBSMUXER_MEM_RANGE;

typedef struct {
	UINT32 mdatid;         //now id
	UINT32 close_result;   //close result

} BSMUXER_CALLBACK_INFO;

typedef UINT32(*BSMUXER_CB)(UINT32 event_id, UINT32 va1, UINT32 va2, UINT32 va3);

typedef struct {
	BSMUXER_CB     callBackFunc;      ///< event inform callback
	UINT32         version;            ///< mediaRec version MEDIAREC_VER_2_0
} BSMUXER_OBJ, *PBSMUXER_OBJ;


#define NMR_MAX_TOTALMDAT  8

typedef struct {
	UINT32 gpson;
	UINT32 gpsdataadr;
	UINT32 gpsdatasize;

} MDAT_GPSINFO;

typedef struct {
	UINT32 thumbadr;
	UINT32 thumbsize;

} MDAT_THUMBINFO;


typedef struct {
	UINT32 vidpathid;
	UINT32 vidcodec;//MEDIAVIDENC_H264
	UINT32 vfr; //round
	UINT32 width;
	UINT32 height;
	UINT32 descAddr;
	UINT32 descSize;
	UINT32 tbr;//target bitrate Bytes/sec
	UINT32 DAR;//MP_VDOENC_DAR_DEFAULT or others
} MDAT_VIDEOINFO;

typedef struct {
	UINT32 codectype;//MOVAUDENC_AAC or others
	UINT32 chs;//channels
	UINT32 asr;//audio samplerate
	UINT32 aud_en;//audio enable
} MDAT_AUDIOINFO;

typedef struct {
	MDAT_VIDEOINFO vid;
	MDAT_AUDIOINFO aud;
	UINT32 fstskid;
	UINT32 emron;
	UINT32 emrloop;
	UINT32 strgid;
	UINT32 seamlessSec;
	UINT32 rollbacksec;//video rollback sec
	UINT32 keepsec;//emr keep sec
	UINT32 endtype;//MOVREC_ENDTYPE_CUTOVERLAP
	UINT32 filetype;//MEDIA_FILEFORMAT_MP4
	UINT32 recformat;//MEDIAREC_AUD_VID_BOTH as default
	UINT32 playvfr;  //30 as default, only works in MEDIAREC_TIMELAPSE/MEDIAREC_GOLFSHOT
} MDAT_FILEINFO;

typedef struct {
	UINT32 frontheader;
	UINT32 backheader;
	UINT32 backsize;
	UINT32 framebuf;
	UINT32 framesize;
	UINT32 nidxaddr;
	UINT32 gpsaddr;//gps data in mdat
	UINT32 gpsbuffer;//gps entry
	UINT32 gpsbufsize;
	UINT32 gpstagaddr;//gps atom in moov
	UINT32 gpstagsize;
	UINT32 frontheader_2;
} MDAT_HDRINFO;

typedef struct {
	UINT32 mdatstart;
	UINT32 mdatend;
	UINT32 totalstart;
	UINT32 totalsize;
} MDAT_MEMINFO;

typedef struct {
	UINT32 nidxok_vfn;
	UINT32 nidxok_afn;
} MDAT_NIDXINFO;

typedef struct {
	MDAT_FILEINFO file;
	MDAT_GPSINFO  gps;
	MDAT_THUMBINFO  thumb;
	MDAT_HDRINFO  hdr;
	MDAT_MEMINFO  mem;
	MDAT_NIDXINFO nidxinfo;
	UINT32        mdatid;//out, mdatid
	UINT32        freasize;//out freasize
	UINT32        start;//starting
	UINT32        precalc_buf;
} MDAT_ONEINFO, *PMDAT_ONEINFO;
typedef struct {
	UINT32                  max_mdat_num;     ///< max file number
	UINT32                  totalbufadr;
	UINT32                  totalbufsize;
	MDAT_ONEINFO            mdatinfo[NMR_MAX_TOTALMDAT];
} MDAT_MDATOBJ, *PMDAT_MAINOBJ;

typedef struct {
	UINT32              mdatid;                      ///< MAKEFOURCC('F','O','U','T')
	UINT32              event;                     ///< defined by the input unit
	UINT32              fileop;                    ///< bitwise: open/close/conti_write or seek_write/flush/none(event only)/discard
	UINT32              addr;                      ///< write data address
	UINT64              size;                      ///< write data size
	UINT64              pos;                       ///< only valid if seek_write
	UINT32              type;                      ///< file type, MP4, TS, JPG, THM
} BSMUXER_FILEOUT_BUF;

extern void     mdat_configobj(MDAT_MDATOBJ *pObj, UINT32 id);
extern void     mdat_test(UINT32 id);
extern UINT32   NMR_BsMuxer_GetValidNum(void);
extern UINT32   NMR_BsMuxer_GetFsTskID_byid(UINT32 mdatid);
extern UINT32   NMR_BsMuxer_CheckEMRon_byid(UINT32 mdatid);
extern void     NM_Media_GetBSFile(UINT32 useBuf, UINT32 bufsize);
extern void     Cmd_nmrec_mdat_give(UINT32 order1);
extern UINT32   Cmd_nmrec_mdat_readtest(UINT32 order2, UINT32 *poutAdr, UINT32 *poutSize);
extern void     Cmd_nmrec_mdat_wrfs(UINT32 mdatid);
extern UINT32           gNMR_MdatFileStart[];
extern UINT32           gNMR_MdatFileSize[];
extern ER               NMR_BsMuxer_MovUpdateHeader(UINT32 fstskID);
extern PMDAT_ONEINFO    NMR_BsMuxer_GetMdatInfo(UINT32 mdatid);
extern PMDAT_ONEINFO    NMR_BsMuxer_GetMdatInfo2(UINT32 mdatid);
extern UINT32   NMR_BsMuxer_GetNowFrames(UINT32 type, UINT32 mdatid);
extern UINT32   NMR_BsMuxer_GetSeamlessSec(UINT32 mdatid);
extern UINT32   NMR_BsMuxer_GetTBR(UINT32 mdatid);
extern UINT32   NMR_BsMuxer_GetEMR(UINT32 mdatid);
//extern void 	NMR_BsMuxer_StopFromFileout(UINT32 event, UINT32 mdatid);
extern ER		NMR_BsMuxer_ReleaseBuf(BSMUXER_FILE_BUF *pBuf);

/**
    Set GPS data.

    Set GPS data for Smedia container.

    @note It's only valid for mp4 file currently. The GPS data is put in 'GPS ' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     NMR_BsMuxer_SetGPSData(UINT32 addr, UINT32 size);


/**
    Trigger to make a EMR file (old name NMediaRec_StopFlash)
    (If EMR is disable, it doesn't work!)

    @param[in]  0 or 1
    @return void
*/
extern ER       NMediaRec_TrigEMR(UINT32 ID);

/**
    Set Media max keep sec

    @param[in] value 3~30 is valid, default: 5

    @return void
*/
extern void 	NMR_BsMuxer_SetMaxKeepSec(UINT32 value);

extern void NMediaRec_SetCutsize(UINT32 value);

extern void NMR_BsMuxer_AddUnit(void);

/**
    Calculate recording seconds by setting.

    @param[in]  pSetting setting
    @return seconds to record
*/
extern UINT32 NMediaRec_CalcSecBySetting(NMEDIAREC_CALCSEC_SETTING *pSetting);

/**
    Set reserved size in storage, whatever endtype is.

    @param[in]  value. Default=zero, max: 512MB
    @return void
*/
extern void NMediaRec_SetCustomDataMaxSize(UINT32 value);

/**
    Set moov layer1 atom.

    @param[in]  adr address of atom
    @param[in]  size size of atom, max=1K
    @return void
*/
extern void NMediaRec_SetMoovLayer1Atom(UINT32 adr, UINT32 size);

/**
    Cut now to end last file, start new file.

    @param[in] mdatid

    @return ER E_OK  cut ok.
               E_SYS cannot be cut now.
*/
extern ER       NMediaRec_SetCutNOW(UINT32 mdatid);
extern void     NMediaRec_BsMuxer_Enable_PointThmfile(UINT32 en);
extern UINT32   NMR_BsMuxer_UserMakerModelData(MOV_USER_MAKERINFO *pMaker);
extern UINT32   NMediaRec_CalcSecBySettingPath(NMEDIAREC_CALCSEC_SETTING2 *pSetting);
extern void     NMR_Mdat_ShowBufMsg(void);

#endif //_NMEDIARECFILEOUT_H


