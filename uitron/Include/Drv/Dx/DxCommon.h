/**
    Dx Object

    Dx Object body definded here.

    @file       DxCommon.h
    @ingroup    mDX
    @note

    UINT32 (*pfGetcap)(UINT32 CapID); It can be run before DxOpen or after DxOpen.
    UINT32 (*pfInit)(void* pInitParam); It have to be run before DxOpen. This one checks DxObject version
    UINT32 (*pfOpen)(void); cannot be NULL.
    UINT32 (*pfClose)(void); cannot be NULL.
    UINT32 (*pfState)(UINT32 StateID, UINT32 Value); It have to be run after DxOpen.
    UINT32 (*pfControl)(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  It have to be run after DxOpen.
    UINT32 (*pfDumpinfo)(void); cannot be NULL. It have to be run after DxOpen.
    UINT32 (*pfCallback)(UINT32 EventID, UINT32 Param1, UINT32 Param2);  It have to be run after DxOpen.

    Dx APIs check DXFLAG_SIGN and DxObject address value. The implemented body don't need to check it.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _DXCOMM_H
#define _DXCOMM_H

#include "Type.h"
#include "string.h"
/**
     @name Signature value
*/
//@{
#define DXFLAG_SIGN                 0x00004458 ///< a signature is "DX"
#define DXFLAG_SIGN_MASK            0x0000FFFF ///< a mask to get Dx signature
#define DXFLAG_OPEN                 0x00010000 ///< a bit indicates if DxObject is open.
//@}

/**
     @name General Properties value
*/
//@{
#define DXGET                       0x10000000 ///< used in pfState' StateID indicates the current requirement is get
#define DXSET                       0x20000000 ///< used in pfState' StateID indicates the current requirement is set
//@}

/**
     DX OBJECT's Body

     Mapping to the DX_HANDLE, used in ModelExt.
*/
typedef struct _DX_OBJECT {
	UINT32 sign;                                        ///< Signature. This have to equal to DXFLAG_SIGN
	UINT32 type;                                        ///< Device Type
	UINT32 ver;                                         ///< Interface Version
	CHAR *name;                                         ///< Object Name
	UINT32 resv[4];                                     ///< Reserved data
	UINT32(*pfGetcaps)(UINT32 CapID, UINT32 Param1);    ///< Get Capability Flag (Base on interface version)
	UINT32(*pfSetcfgs)(UINT32 CfgID, UINT32 Param1);    ///< Set Config Setting
	UINT32(*pfInit)(void *pInitParam);                  ///< Set Init Parameters
	UINT32(*pfOpen)(void);                              ///< Common Constructor
	UINT32(*pfClose)(void);                             ///< Common Destructor
	UINT32(*pfState)(UINT32 StateID, UINT32 Value);     ///< General Properties
	UINT32(*pfControl)(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);    ///< General Methods
	UINT32(*pfCommand)(CHAR *pcCmdStr);                                 ///< General Command
	void (*pfCallback)(UINT32 EventID, UINT32 Param1, UINT32 Param2);   ///< General Events
}
DX_OBJECT;

#endif //_DXCOMM_H
