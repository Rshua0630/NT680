#ifndef _NMEDIARECFILEOUT_H
#define _NMEDIARECFILEOUT_H

#include "NvtMediaInterface.h"
#include "MediaWriteLib.h"

enum {
	//Basic Settings
	NMI_FILEOUT_PARAM_MAX_FILE,//
	NMI_FILEOUT_PARAM_REG_CB,
	NMI_FILEOUT_PARAM_ENDTYPE,
	NMI_FILEOUT_PARAM_EN_POINTTHM,
};

typedef enum {
	NMI_FILEOUT_ACTION_START,
	NMI_FILEOUT_ACTION_STOP,
	NMI_FILEOUT_ACTION_MAX,
} NMI_FILEOUT_ACTION;

typedef struct {
	UINT32 Addr;    ///< data to write
	UINT32 Size;    ///< size to write
} FILEOUT_DATA, *PFILEOUT_DATA;

typedef struct {
	UINT32 dataAddr;
	UINT32 dataSize;
	UINT32 uiAudio; //is audio or not
	UINT32 padSize;
	UINT32 ifUpdateFlashQ; //if update flash Q
	UINT32 ifNidx;
	UINT32 givenYes;//give a specific fileoffset to write! 0 means continue
	UINT32 rev;
	UINT64 givenPos;//pos if giveYes =1
} NMR_FSQ_BLOCK_INFO;

typedef enum {
	NMI_FILEOUT_GETNAME_EMRPATH,
	NMI_FILEOUT_GETNAME_PATH0,
	NMI_FILEOUT_WRITE_ERR,    //p1: id
	NMI_FILEOUT_FILE_COMPLETE, //p1: id
	NMI_FILEOUT_CLOSE_RESULT = 0x100,    //p1: pointer of FILEOUT_CALLBACK_INFO
	ENUM_DUMMY4WORD(NMI_FILEOUT_CB_EVENT)
} NMI_FILEOUT_CB_EVENT;

typedef struct {
	UINT32 mdatid;         //now id
	UINT32 filetype;   //MEDIA_FILEFORMAT_MP4 or others

} FILEOUT_CALLBACK_GETNAME_INFO;

#define FILEOUT_RESULT_NORMAL			0
#define FILEOUT_RESULT_WRITEERR			1
#define FILEOUT_RESULT_LOOPREC_FULL 	2
#define FILEOUT_RESULT_SLOW				3
#define FILEOUT_RESULT_CARDFULL         4


typedef struct {
	UINT32 id;         //now id
	UINT32 close_result;   //close result, FILEOUT_RESULT_NORMAL or others
} FILEOUT_CALLBACK_INFO;



typedef void (NMR_FILEOUT_CB)(UINT32 event, UINT32 param);
typedef void (MediaRecCallbackType)(UINT32 event_id, UINT32 Seconds);

extern ER       NMR_FileOut_SetFSNamingHandler_byid(MEDIANAMINGRULE *pHdl, UINT32 nowid);

typedef struct {
	NMR_FILEOUT_CB          *CallBackFunc;      ///< event inform callback
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
	UINT32                  filetype;       ///< filetype, MEDIA_FILEFORMAT_MP4 or others
} NMI_FILEOUT_OBJ;

extern UINT32 	NMR_GetEmrMdatid(void);

extern void 	NMR_FileOut_AddUnit(void);
//extern void     NMR_FSUti_PauseAudioAndStop(UINT32 result);
extern UINT64   NMI_FileOut_GetRemain(void);
extern void 	NMR_FSObj_ClearNidxVAfn(UINT32 fstskID);
extern void     NMediaRec_Enable_PointThmfile(UINT32 en);
extern void     NMediaRec_SetCustomDataMaxSize_Fs(UINT32 value);
extern void     NMI_FS_CheckInit(void);

/**
    Set nextfile as read-only and move.

    @param[in] nextcrash:0 to cancel, 1 to set, fstskid: fileid
    @return void
*/
extern void 	NMR_FileOut_SetNextCrash(UINT32 nextcrash, UINT32 fstskid);

/**
    Set previous file as read-only and move.

    @param[in] fstskid: fileid
    @return
       - @b MOVREC_SETCRASH_OK: previous file is RO and moved.
       - @b MOVREC_SETCRASH_NOLASTFILE:  no last file.
       - @b MOVREC_SETCRASH_LASTFILERO:  previous file has been RO.

*/
extern UINT32   NMR_FileOut_SetPrevCrash(UINT32 fstskID);

/**
    Set now crash, when end file, set to read-only and move.

    @param[in] crash:0 to cancel, 1 to set, fstskid: fileid
    @return void
*/
extern void     NMR_FileOut_SetCrash(UINT32 crash, UINT32 fstskid);


#endif //_NMEDIARECFILEOUT_H

