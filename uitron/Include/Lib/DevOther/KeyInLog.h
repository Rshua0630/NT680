/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyInLog.h
    @ingroup

    @brief      Log keyIn, modedial, ..
                keyIn modedial, ..Logging

    @note       Nothing.

    @date       2009/03/18
*/

#ifndef _KEYINLOG_H
#define _KEYINLOG_H

//#define KEYINLOG_FUNCTION               ENABLE
#define KEYINLOG_FUNCTION               DISABLE

#define Keyscan_FUNCMASK                0xFF000000
#define Keyscan_FUNCMASK_CNT            0x00FFFFFF

#define KeyScan_DetKey_FUNC             0x01000000 /*0xaa_bbbbbb ==> aa:function index, bbbbbb=count*/
#define KeyScan_DetModedial_FUNC        0x02000000
#if (KEYINLOG_FUNCTION == ENABLE)
//------------------------------------------------------------
//As follow function for KeyIn Log Init(Keyin system init)
extern void Keylog_Init(UINT32 *pKeyScanTimerCounter);

//------------------------------------------------------------
//As follow function for KeyIn Log Playing
extern BOOL Keylog_IsRuningKeyLog(void);
extern void Keylog_EnableRuningKeyLog(void);
extern void Keylog_DisableRuningKeyLog(void);
extern void Keylog_InitKeylogRecordingFile(CHAR *filename);
extern BOOL Keylog_ScanRecordEnd(void);

//------------------------------------------------------------
// As follow functions for Keys and ModeDial
//
//(A)
extern BOOL Keylog_IsRecordingKeyLog(void);
extern void Keylog_EnableRecordingKeyLog(void);
extern void Keylog_DisableRecordingKeyLog(void);
//(B)
extern UINT32 Keylog_Play_DetKey(void);
extern void   Keylog_Record_DetKey(UINT32 uiCurKeyStatus);
//(C)
extern UINT32 Keylog_Play_DetModedial(void);
extern void   Keylog_Record_DetModedial(UINT32 uiModedial);

//------------------------------------------------------------
// As follow functions for TV
//

//------------------------------------------------------------
// As follow functions for Battery(ADC)
//
#endif //#if (KEYINLOG_FUNCTION == ENABLE)

#endif //_KEYINLOG_H

