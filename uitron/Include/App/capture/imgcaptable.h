/**
    header file for capture event table

    This file is the header file that define the API and data type for capture event table.

    @file       imgcaptable.h
    @ingroup    mIAppCap
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IMG_CAP_TABLE_H_
#define _IMG_CAP_TABLE_H_

#include "Type.h"
#include "imgcaptureapi.h"
#include "imgcapinfor.h"
#include "imgcommon.h"

/** \addtogroup mIAppCap */
//@{

/**
    captsk: assign buffer event prototype
*/
typedef IMGCAP_EVTMSG(*IMGCAP_ASSIGNBUF)(UINT32 BufAddr, UINT32 BufSize);

/**
    captsk: enter event prototype
*/
typedef IMGCAP_EVTMSG(*IMGCAP_ENTER)(UINT32 BufAddr, UINT32 BufSize, UINT32 *UsedSize);

/**
    captsk: process event prototype
*/
typedef IMGCAP_EVTMSG(*IMGCAP_PROC)(UINT32 BurstNum, IMG_CAP_S2DET_FP S2DetFP);

/**
    captsk: exit event prototype
*/
typedef IMGCAP_EVTMSG(*IMGCAP_EXIT)(UINT32 BurstNum);

/**
    captsk: get extra flag event prototype
*/
typedef FLGPTN(*IMGCAP_GETEVTFLAG)(void);

/**
    the event object for capture task.

    event trigger timing:\n
    FP_GetEvtFlag -> FP_EvtEnter -> FP_EvtAssignBuf -> FP_EvtProc -> FP_EvtProc...... -> FP_EvtExit
*/
typedef struct _ImgCapTskEvent {
	IMGCAP_ENTER FP_EvtEnter;               ///< enter event function pointer
	IMGCAP_ASSIGNBUF FP_EvtAssignBuf;       ///< assign buffer event function pointer
	IMGCAP_PROC FP_EvtProc;                 ///< process event function pointer
	IMGCAP_EXIT FP_EvtExit;                 ///< exit event function pointer
	IMGCAP_GETEVTFLAG FP_GetEvtFlag;        ///< get flag event function pointer for extra flag
	CHAR *EvtName;                          ///< cap mode name for debug
} ImgCapTskEvent;

/**
    proctsk: event prototype
*/
typedef IMGCAP_EVTMSG(*IMGPROC)(UINT32 BurstNum);

/**
    proctsk: get ext. flag event prototype
*/
typedef FLGPTN(*IMGPROC_GETEVTFLAG)(void);

/**
    the event object for process task.

    event trigger timing:\n
    FP_GetEvtFlag -> FP_EvtEnter -> FP_EvtProc -> FP_EvtProc...... -> FP_EvtExit
*/
typedef struct _ImgProcTskEvent {
	IMGPROC FP_EvtEnter;                ///< enter event function pointer
	IMGPROC FP_EvtProc;                 ///< process event function pointer
	IMGPROC FP_EvtExit;                 ///< exit event function pointer
	IMGPROC_GETEVTFLAG FP_GetEvtFlag;   ///< get flag event function pointer for extra flag
	CHAR *EvtName;                      ///< cap mode name for debug
} ImgProcTskEvent;


/**
    proc2tsk: event prototype
*/
typedef IMGCAP_EVTMSG(*IMGPROC2)(UINT32 BurstNum);

/**
    proc2tsk: get ext. flag event prototype
*/
typedef FLGPTN(*IMGPROC2_GETEVTFLAG)(void);

/**
    the event object for process2 task.

    event trigger timing:\n
    FP_GetEvtFlag -> FP_EvtEnter -> FP_EvtProc -> FP_EvtProc...... -> FP_EvtExit
*/
typedef struct _ImgProc2TskEvent {
	IMGPROC2 FP_EvtEnter;                ///< enter event function pointer
	IMGPROC2 FP_EvtProc;                 ///< process event function pointer
	IMGPROC2 FP_EvtExit;                 ///< exit event function pointer
	IMGPROC2_GETEVTFLAG FP_GetEvtFlag;   ///< get flag event function pointer for extra flag
	CHAR *EvtName;                      ///< cap mode name for debug
} ImgProc2TskEvent;


/**
    enctsk: event prototype
*/
typedef IMGCAP_EVTMSG(*IMGENC)(UINT32 BurstNum);

/**
    enctsk: get extra flag event prototype
*/
typedef FLGPTN(*IMGENC_GETEVTFLAG)(void);

/**
    the event object for encode task.

    event trigger timing:\n
    FP_GetEvtFlag -> FP_EvtEnter -> FP_EvtProc -> FP_EvtProc...... -> FP_EvtExit
*/
typedef struct _ImgEncTskEvent {
	IMGENC FP_EvtEnter;                ///< enter event function pointer
	IMGENC FP_EvtProc;                 ///< process event function pointer
	IMGENC FP_EvtExit;                 ///< exit event function pointer
	IMGENC_GETEVTFLAG FP_GetEvtFlag;   ///< get flag event function pointer
	CHAR *EvtName;                     ///< cap mode name for debug
} ImgEncTskEvent;

/**
    writefiletsk: event prototype
*/
typedef IMGCAP_EVTMSG(*IMGWRITEFILE)(UINT32 BurstNum);

/**
    writefiletsk: get extra flag event prototype
*/
typedef FLGPTN(*IMGWRITEFILE_GETEVTFLAG)(void);

/**
    the event object for write file task.

    event trigger timing:\n
    FP_GetEvtFlag -> FP_EvtEnter -> FP_EvtProc -> FP_EvtProc...... -> FP_EvtExit
*/
typedef struct _ImgWriteFileTskEvent {
	IMGWRITEFILE FP_EvtEnter;               ///< enter event function pointer
	IMGWRITEFILE FP_EvtProc;                ///< process event function pointer
	IMGWRITEFILE FP_EvtExit;                ///< exit event function pointer
	IMGWRITEFILE_GETEVTFLAG FP_GetEvtFlag;  ///< get flag event function pointer
	CHAR *EvtName;                          ///< cap mode name for debug
} ImgWriteFileTskEvent;


/**
    Register capture task event.

    @param[in] Mode     register drive mode
    @param[in] Event    event function pointer. Each function pointer can be set NULL.
    @return
        - @b E_OK:   Success.
        - @b E_SYS: Fail.

    Example:
    @code
    {
        ImgCapTskEvent Event;
        CapEvent.FP_EvtEnter = NULL;
        CapEvent.FP_EvtAssignBuf =ImgCap_PlugInAssignBuf;
        CapEvent.FP_EvtProc = ImgCap_PlugIn;
        CapEvent.FP_EvtExit = ImgCap_PlugInExit;
        CapEvent.FP_GetEvtFlag = ImgCap_PlugInGetEvtFlag;
        CapEvent.EvtName = "CUSTOMER1";
        ImgCap_RegCapEvent(SEL_DRIVEMODE_CUSTOMER1, &CapEvent);
    }
    @endcode
*/
ER ImgCap_RegCapEvent(SEL_DRIVEMODE Mode, ImgCapTskEvent *Event);

/**
    Register process task event.

    @param[in] Mode     register drive mode
    @param[in] Event    event function pointer. Each function pointer can be set NULL.
    @return
        - @b E_OK:   Success.
        - @b E_SYS: Fail.

    Example:
    @code
    {
        ImgProcTskEvent ProcEvent;
        ProcEvent.FP_EvtEnter = ImgProc_PlugInEnter;
        ProcEvent.FP_EvtProc = ImgProc_PlugIn;
        ProcEvent.FP_EvtExit = ImgProc_PlugInExit;
        ProcEvent.FP_GetEvtFlag = ImgProc_PlugInGetEvtFlag;
        ProcEvent.EvtName = "CUSTOMER1";
        ImgCap_RegProcEvent(SEL_DRIVEMODE_CUSTOMER1, &ProcEvent);
    }
    @endcode
*/
ER ImgCap_RegProcEvent(SEL_DRIVEMODE Mode, ImgProcTskEvent *Event);

/**
    Register encode task event.

    @param[in] Mode     register drive mode
    @param[in] Event    event function pointer. Each function pointer can be set NULL.
    @return
        - @b E_OK:   Success.
        - @b E_SYS: Fail.

    Example:
    @code
    {
        ImgEncTskEvent EncEvent;
        EncEvent.FP_EvtEnter = NULL;
        EncEvent.FP_EvtProc = ImgEnc_PlugIn;
        EncEvent.FP_EvtExit = NULL;
        EncEvent.FP_GetEvtFlag = NULL;
        EncEvent.EvtName = "CUSTOMER1";
        ImgCap_RegEncEvent(SEL_DRIVEMODE_CUSTOMER1, &EncEvent);
    }
    @endcode
*/
ER ImgCap_RegEncEvent(SEL_DRIVEMODE Mode, ImgEncTskEvent *Event);

/**
    Write file task event.

    @param[in] Mode     register drive mode
    @param[in] Event    event function pointer. it can be set NULL.
    @return
        - @b E_OK:   Success.
        - @b E_SYS: Fail.

    Example:
    @code
    {
        ImgWriteFileTskEvent FSEvent;
        FSEvent.FP_EvtEnter = NULL;
        FSEvent.FP_EvtProc = NULL;
        FSEvent.FP_EvtExit = NULL;
        FSEvent.FP_GetEvtFlag = NULL;
        FSEvent.EvtName = "CUSTOMER1";
        ImgCap_RegEncEvent(SEL_DRIVEMODE_CUSTOMER1, &FSEvent);
    }
    @endcode
*/
ER ImgCap_RegWriteFileEvent(SEL_DRIVEMODE Mode, ImgWriteFileTskEvent *Event);


//@}
#endif //_IMG_CAP_TABLE_H_
