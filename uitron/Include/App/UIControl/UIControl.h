/**
    UI Control related API.

    UI Control export variables and function prototypes.

    @file       UIControl.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#ifndef UICONTROL_H
#define UICONTROL_H

#include "Type.h"
#include "NVTReturn.h"
#include "UIControlEvent.h"
#include "VControl.h"

extern void UIControl_InstallID(void) _SECTION(".kercfg_text");


/**
    @addtogroup mIUIControl
*/
//@{

/**
    Set child control by index

    @param[in] pParent The pointer of the parent control instance
    @param[in] index The index of the  control list
    @param[in] pCtrl The pointer of the child control instance

*/
extern void UxCtrl_SetChild(VControl *pParent, UINT32 index, VControl *pCtrl);

/**
    Get child control by index

    @param[in] pCtrl The pointer of the control instance
    @param[in] index The index of the control list

    @return The pointer of child control instance

*/
extern VControl *UxCtrl_GetChild(VControl *pCtrl, UINT32 index);

/**
    Get the leaf focus control

    @param[in] pCtrl The pointer of the ancestor control instance

    @return The pointer of leaf control instance

*/
extern VControl *UxCtrl_GetLeafFocus(VControl *pCtrl);

/**
    Set control user data

    @param[in] pCtrl The pointer of the control instance
    @param[in] userData The pointer of the user data
*/
extern void UxCtrl_SetUsrData(VControl *pCtrl, void *userData);

/**
    Get control user data

    @param[in] pCtrl The pointer of the control instance

    @return The pointer of the user data
*/
extern void *UxCtrl_GetUsrData(VControl *pCtrl);

/**
    Set control data table

    @param[in] pCtrl The pointer of the control instance
    @param[in] DataTable The pointer of the data table
*/
extern void UxCtrl_SetDataTable(VControl *pCtrl, void *DataTable);

/**
    Get control data table

    @param[in] pCtrl The pointer of the control instance

    @return The pointer of the data table
*/
extern void *UxCtrl_GetDataTable(VControl *pCtrl);

/**
    Set control show table

    @param[in] pCtrl The pointer of the control instance
    @param[in] showTable The pointer of the show table
*/
extern void UxCtrl_SetShowTable(VControl *pCtrl, struct _ITEM_BASE   **showTable);

/**
    Get control show table

    @param[in] pCtrl The pointer of the control instance

    @return The pointer of the show table
*/
extern struct _ITEM_BASE   **UxCtrl_GetShowTable(VControl *pCtrl);

/**
    Set show status of the control

    Ux_Redraw() function will check the show status of this control to determine
    whether shows or hides this control

    @note if set hide,this API would set parent of control dirty,Ux_Redraw would
    redraw parent for clean this control

    @param[in] pCtrl The pointer of the control instance
    @param[in] bShow TRUE show; FALSE hide
*/
extern void UxCtrl_SetShow(VControl *pCtrl, BOOL bShow);

/**
    Get the show status of the control

    @param[in] pCtrl The pointer of the control instance

    @return
        - @b TRUE The control is shown \n
        - @b FALSE The control is hidden
*/
extern BOOL UxCtrl_IsShow(VControl *pCtrl);

/**
    Set show status of the all child control

    Ux_Redraw() function will check the show status of this control to determine
    whether shows or hides the child control

    @param[in] pCtrl The pointer of the parent control instance
    @param[in] bShow TRUE show; FALSE hide
*/
extern void UxCtrl_SetAllChildShow(VControl *pCtrl, BOOL bShow);

/**
    Set control position

    @param[in] pCtrl The pointer of the control instance
    @param[in] rect The rectangle of the control position
*/
extern void UxCtrl_SetPos(VControl *pCtrl, Ux_RECT rect);

/**
    Get control position

    @param[in] pCtrl The pointer of the control instance
    @param[out] pRect The pointer of the control position

    @return
        - @b NVTRET_OK get position success.
        - @b NVTRET_ERROR pCtrl or pRect is null pointer.
*/
extern INT32 UxCtrl_GetPos(VControl *pCtrl, Ux_RECT *pRect);

/**
    Set dirty status of the control

    Ux_Redraw() function will check the dirty status of this control to determine
    whether issue NVTEVT_REDRAW to the control or not.

    @param[in] pCtrl The pointer of the control instance
    @param[in] bDirty TRUE set dirty; FALSE clear dirty
*/
extern void UxCtrl_SetDirty(VControl *pCtrl, BOOL bDirty);

/**
    Get the dirty status of the control

    @param[in] pCtrl The pointer of the control instance

    @return
        - @b TRUE The control is set dirty \n
        - @b FALSE The control is set not dirty
*/
extern BOOL UxCtrl_IsDirty(VControl *pCtrl);

/**
    Set dirty status of the child control

    Ux_Redraw() function will check the dirty status of the child control to determine
    whether issue NVTEVT_REDRAW to the child control or not.

    @param[in] pCtrl The pointer of the control instance
    @param[in] nChildIndex The index of the control in Control list
    @param[in] bDirty TRUE set dirty; FALSE clear dirty
*/
extern void UxCtrl_SetChildDirty(VControl *pCtrl, INT32 nChildIndex, BOOL bDirty);

/**
    Set lock status of the control

    Dispatch event would check if any control locked.
    If there is locked control,event would be only sent to locked control.
    This API is useful for control in STATUS_FOCUS_PRESS status.

    @note Most only one control be locked

    @param[in] pCtrl The pointer of the control instance
    @param[in] bLock TRUE set lock; FALSE clear lock
*/
extern void UxCtrl_SetLock(VControl *pCtrl, BOOL bLock);

/**
    Get lock status of the control

    Dispatch event would check if any control locked.
    If there is locked control,event would be only sent to locked control.

    @note Most only one control be locked

    @return The locked control
*/
extern VControl *UxCtrl_GetLock(void);

//@}
#endif //UICONTROL_H

