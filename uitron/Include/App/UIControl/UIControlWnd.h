/**
    UI Control window header file

    All window related export variables and function prototypes.

    @file       UIControlWnd.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICONTROLWND_H
#define UICONTROLWND_H

#include "Type.h"
#include "NVTReturn.h"
#include "UIControlEvent.h"

/**
     @addtogroup mIUIControl
*/
//@{



/**
    General Control type
*/
typedef enum {
	CTRL_BASE       = CTRL_TYPE_MIN, ///< Base control type,Ux_DefaultEvent would loop up the control's base object
	CTRL_WND,                        ///< Window control,for all Ux_xxxWindow APIs
	CTRL_TYPE_MAX                    ///< Max control type value.
} CONTROL_TYPE;


/**
    @name Speicial window index for Ux_GetWindowByIndex
*/
//@{
#define UX_ROOT_WND_IDX         0   ///< Root window index
#define UX_2NDLAYER_WND_IDX     1   ///< Second windwo index
#define UX_3RDLAYER_WND_IDX     2   ///< Third window index
//@}

/**
    The parameter index of NVTEVT_REDRAW event.
    The first NVTEVT_REDRAW parameter always screen obj.
    Example:
    @code
    INT32 Wnd_OnDraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
    {
        DC** pDCList = (DC**)paramArray[ONEVENT_PARAM_INDEX_SCREEN];
    }
    @endcode
*/
#define ONEVENT_PARAM_INDEX_SCREEN    0

/**
    Create a window

    After the window is created, the window is the focused window.
    All events is dispathed to this window.

    @param[in] pCtrl The pointer of the window instance
    @param[in] paramNum How many parameters given to the created window. Acceptable value:0~3
    @param ... Variable number arguments according to paramNum

    @return
        - @b NVTRET_OK open success
        - @b NVTRET_ERROR open fail

    @code
    Ux_OpenWindow((VControl *)(&UIFlowInfo), 0);
    @endcode

    @code
    Ux_OpenWindow((VControl *)(&UIFlowInfo), 1, value1);
    @endcode

    @code
    Ux_OpenWindow((VControl *)(&UIFlowInfo), 2, value1, value2);
    @endcode

    @code
    Ux_OpenWindow((VControl *)(&UIFlowInfo), 3, value1, value2, value3);
    @endcode
*/
extern NVTRET Ux_OpenWindow(VControl *pCtrl, UINT32 paramNum, ...);

/**
    Close a window

    If input window is the last window, this window is closed.
    If input window is not the last window, the windows opened after input window are
    closed, includeing the input window.

    @param[in] pCtrl The pointer of the window instance
    @param[in] paramNum How many parameters given to the created window. Acceptable value:0~3
    @param ... Variable number arguments according to paramNum

    @return
        - @b NVTRET_OK close success
        - @b NVTRET_ERROR close fail
*/
extern NVTRET Ux_CloseWindow(VControl *pCtrl, UINT32 paramNum, ...);

/**
    Close a window with clear the screen

    The action of this function is mush the same as Ux_CloseWindow().
    Besides, it calls the _pUIReader->pfn_ClearScreen() to clear the screen.
    The actul clearing process is defined in the function pointed by pfn_ClearScreen.

    @param[in] pCtrl The pointer of the window instance
    @param[in] paramNum How many parameters given to the created window. Acceptable value:0~3
    @param ... Variable number arguments according to paramNum

    @return
        - @b NVTRET_OK close success
        - @b NVTRET_ERROR close fail

*/
extern NVTRET Ux_CloseWindowClear(VControl *pCtrl, UINT32 paramNum, ...);

/**
    Get root window

    Root window means the first created window.

    @param[out] pRootCtrl The root window pointer by reference

    @return
        - @b NVTRET_OK get success
        - @b NVTRET_ERROR get fail

*/
extern NVTRET Ux_GetRootWindow(VControl **pRootCtrl);

/**
    Get parent window

    Parent window means the previous created window of the input window.

    @param[in] pCtrl The pointer of the window instance
    @param[out] pParentWnd The parent window pointer by reference

    @return
        - @b NVTRET_OK get success
        - @b NVTRET_ERROR get fail

*/
extern NVTRET Ux_GetParentWindow(VControl *pCtrl, VControl **pParentWnd);

/**
    Get current focused window

    @param[out] ppFocusWnd The focused window pointer by reference

    @return
        - @b NVTRET_OK get success
        - @b NVTRET_ERROR get fail

*/
extern NVTRET Ux_GetFocusedWindow(VControl **ppFocusWnd);

/**
    Get window by index

    @param[out] ppWnd The index window pointer by reference
    @param[in] index Control index,root index is 0

    @return
        - @b NVTRET_OK get success
        - @b NVTRET_ERROR get fail

*/
extern NVTRET Ux_GetWindowByIndex(VControl **ppWnd, UINT8 index);

/**

    Get this control is force closed by leaf control or not.
    In child close,user can get this close is from child or
    direct force close to root window.

    @return
        - @b TRUE The control is force closed
        - @b FALSE The control is not force closed
*/
extern BOOL Ux_IsForceCloseWindow(void);

/**
    define UIScreen type
*/
typedef UINT32 UIScreen;

/**
    Render function pointer
*/
typedef struct _UIRender {
	UIScreen(*pfn_BeginScreen)(void);                                                   ///< Begin screen function, need to return Screen Object.
	void (*pfn_EndScreen)(UIScreen ScreenObj);                                          ///< End screen function
	void (*pfn_BeginWindow)(VControl *pCtrl, UIScreen ScreenObj);                       ///< Begin window function
	void (*pfn_EndWindow)(VControl *pCtrl, UIScreen ScreenObj);                         ///< End window function
	void (*pfn_BeginControl)(VControl *pCtrl, VControl *pControl, UIScreen ScreenObj);  ///< Begin control function
	void (*pfn_EndControl)(VControl *pCtrl, VControl *pControl, UIScreen ScreenObj);    ///< End control function
	void (*pfn_ClearScreen)(UIScreen ScreenObj);                                        ///< Clear screen function. May called by CloseWindowClear().
}
UIRender;

/**
    Set UIRender instance to UI framework

    @param[in] pUIRender The pointer of UIRender instance
*/
extern void Ux_SetRender(UIRender *pUIRender);

/**
    Ux_Redraw the control

    This function checks the dirty and show status of the focused control include its child control
    to determine what Ux_Redraw actions should be done.

    @return
        - @b NVTRET_OK Redraw success
        - @b NVTRET_ERROR Redraw fail
*/
extern NVTRET Ux_Redraw(void);

/**
    Ux_Redraw the all control from root window

    This function would redraw all windows from root.It is usefull if the child window is not full seceen
    and need to update parent screen.

    @return
        - @b NVTRET_OK Redraw success
        - @b NVTRET_ERROR Redraw fail
*/
extern NVTRET Ux_RedrawAllWind(void);

/**
    Dispatch message

    This API would according to input event dispatch to corresponding function to
    accomplish job.

    @param[in] evt window control event
    @param[in] paramNum How many parameters given. Acceptable value:0~3
    @param[in] paramArray number arguments according to paramNum

    @return
        - @b NVTRET_OK Redraw success
        - @b NVTRET_ERROR Redraw fail
*/
extern INT32 Ux_WndDispatchMessage(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);


/**
    Dump window information.(focus window and rott window).
*/
extern void Ux_DumpStatus(void);

//@}
#endif //UICONTROLWND_H

