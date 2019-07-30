/**
    Device/Driver HAL

    A HAL to get object handle like storage or display

    @file       Dx.h
    @ingroup    mDX

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _DX_H
#define _DX_H

#include "Type.h"
#include "DxType.h"

/**
     @name Error code
*/
//@{
#define DX_OK                       0x0000 ///< Success
#define DX_NULL_POINTER             0x0001 ///< Object is NULL
#define DX_VER_NOTMATCH             0x0002 ///< version not match
#define DX_NOT_SUPPORT              0x0003 ///< the id (CapID, CfgID..) not support
#define DX_LACKOF_FUNC              0x0004 ///< the object has null function pointer
#define DX_ALREADY_OPEN             0x0005 ///< Dx_Open is already called
#define DX_NOT_OPEN                 0x0006 ///< Dx_Open is not called
#define DX_PARAM_ERROR              0x0007 ///< input parameter has wrong.
//@}

/**
     Dx information

     Using Dx_GetInfo with DX_INFO, to get Dx Object information about type or name.
*/
typedef enum _DX_INFO {
	DX_INFO_CLASS,      ///< class type, pOut is a UINT32 pointer
	DX_INFO_NAME,       ///< string name, pOut is a char double pointer
	ENUM_DUMMY4WORD(DX_INFO)
} DX_INFO;


/**
    @addtogroup mDX
*/
//@{

/**
     Get Object

     Get object handle like storage or display

     @param[in] DxClassType       input DX_CLASS_ and DX_TYPE_
     @return object handle

     @code
     {
        Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
        Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_TVOUT);
        Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM0);
        Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM0);
        Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM1);
        Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM3);
        Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
     }
     @endcode
*/
extern DX_HANDLE Dx_GetObject(UINT32 DxClassType);  // Query device object

/**
     Get Object information

     In project layer, user cannot access DX_OBJECT structure. Dx_GetInfo helps
     user access member with DX_HANDLE.

     @param[in] DxObject object handle
     @param[in] InfoID refer to DX_INFO
     @param[in] pOut the returned information
     @return error code
*/
UINT32 Dx_GetInfo(DX_HANDLE DxObject, DX_INFO InfoID, void *pOut);

//@}
#endif //_DX_H

