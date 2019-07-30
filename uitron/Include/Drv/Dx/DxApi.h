/**
    Device Object Operation

    Device Object Operation APIs

    @file       DxApi.h
    @ingroup    mDX

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DXAPI_H
#define _DXAPI_H

#include "Type.h"
#include "DxType.h"

/**
    @addtogroup mDX
*/
//@{

typedef void (*DX_CB)(UINT32 EventID, UINT32 Param1, UINT32 Param2);  ///< General Events type

/**
     Get Capability

     For operating object to get capability. the DxObject need to ref the object their own header.

     @param[in] DxObject object handle
     @param[in] CapID capability like STORAGE_CAPS_ or DISPLAY_CAPS_
     @param[in] Param1 the input value depends on CapID
     @return error code
*/
UINT32 Dx_Getcaps(DX_HANDLE DxObject, UINT32 CapID, UINT32 Param1);

/**
     Set Config Setting

     For operating object to set configuration. the CfgID need to ref the object their own header.

     @param[in] DxObject object handle
     @param[in] CfgID capability like STORAGE_CFG_ or DISPLAY_CFG_
     @param[in] Param1 the input value depends on CfgID
     @return error code
*/
UINT32 Dx_Setconfig(DX_HANDLE DxObject, UINT32 CfgID, UINT32 Param1);

/**
     Set Init Parameters

     For operating object to initialize. the pInitParam and pfCallback need to ref the object their own header.

     @param[in] DxObject object handle
     @param[in] pInitParam is an input structure pointer specific by object header.
     @param[in] pfCallback is an callback pointer specific by object header.
     @param[in] CurrVer current version to check it's right.
     @return error code
*/
UINT32 Dx_Init(DX_HANDLE DxObject, void *pInitParam, DX_CB pfCallback, UINT32 CurrVer);

/**
     Common Constructor

     For operating object to open start.

     @param[in] DxObject object handle
     @return error code
*/
UINT32 Dx_Open(DX_HANDLE DxObject);

/**
     Common Destructor

     For operating object to close.

     @param[in] DxObject object handle
     @return error code
*/
UINT32 Dx_Close(DX_HANDLE DxObject);

/**
     General Get Properties

     For operating object to get state. the StateID need to ref the object their own header.

     @param[in] DxObject object handle
     @param[in] StateID capability like STORAGE_STATE_ or DRVDISP_CFG_
     @param[out] pOut state value
     @return error code
*/
UINT32 Dx_GetState(DX_HANDLE DxObject, UINT32 StateID, UINT32 *pOut);

/**
     General Set Properties

     For operating object to set state. the StateID need to ref the object their own header.

     @param[in] DxObject object handle
     @param[in] StateID capability like STORAGE_STATE_ or DRVDISP_CFG_
     @param[in] Value state value
     @return state value
*/
UINT32 Dx_SetState(DX_HANDLE DxObject, UINT32 StateID, UINT32 Value);

/**
     General Control

     For operating object to control something. the CmdID need to ref the object their own header.

     @param[in] DxObject object handle
     @param[in] CmdID capability like STORAGE_CTRL_ or DRVDISP_CTRL_
     @param[in] Param1 is specific by CmdID
     @param[in] Param2 is specific by CmdID
     @return error code
*/
UINT32 Dx_Control(DX_HANDLE DxObject, UINT32 CmdID, UINT32 Param1, UINT32 Param2);

/**
     General Command Console

     For operating object to send a shell command string. the pcCmdStr need to
     ref the object their own header if there's support.

     @param[in] DxObject object handle
     @param[in] pcCmdStr command string
     @return error code
*/
BOOL Dx_Command(DX_HANDLE DxObject, CHAR *pcCmdStr);

//@}
#endif //_DXAPI_H


