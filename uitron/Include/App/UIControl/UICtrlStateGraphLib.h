/**
    UI state graph control related API.

    UI  state graph control export variables and function prototypes.

    @file       UICtrlStateGraphLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef UICTRLSTATEGRAPHLIB_H
#define UICTRLSTATEGRAPHLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     State graph control structure
*/
typedef struct _CTRL_STATEGRAPH_DATA {
	UINT32 StatusFlag;                          ///< State graph current status
	UINT32 pExeEvent;                           ///< State graph event
	UINT32 totalChangeStatus;                   ///< State graph total status
} CTRL_STATEGRAPH_DATA;

/**
     State graph control attribute index
*/
typedef enum {
	STATEGRAPH_STATUS = (CTRL_STATEGRAPH << 8), ///< Current state graph
	STATEGRAPH_EVENT,                           ///< State graph event
	STATEGRAPH_TOTSTA,                          ///< Total state
	STATEGRAPH_MAX
} STATEGRAPH_DATA_SET;

/**
    Set state graph data
    @param[in] pCtrl The pointer of the state graph control
    @param[in] attribute state graph attribute (STATEGRAPH_DATA_SET)
    @param[in] value the state graph value would be changed
*/
extern void UxStateGraph_SetData(VControl *pCtrl, STATEGRAPH_DATA_SET attribute, UINT32 value);

/**
    Get state graph data
    @param[in] pCtrl The pointer of the state graph control
    @param[in] attribute State graph attribute (STATEGRAPH_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxStateGraph_GetData(VControl *pCtrl, STATEGRAPH_DATA_SET attribute);
//@}
#endif
