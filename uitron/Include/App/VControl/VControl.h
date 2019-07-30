/**
    @file       VControl.h
    @ingroup    mIVControl

    @brief      Declare control object.

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef NVT_VCONTROL_H
#define NVT_VCONTROL_H

#include "NVTEvent.h"
#include "NVTReturn.h"

/**
    @addtogroup mIVControl
*/

//@{


#define CTRL_TYPE_MIN   ((UINT32)0x100)       ///<definition of UI control type minmum valuse
#define APP_TYPE_MIN    ((UINT32)0x200)       ///<definition of application control type minmum valuse

/**
    common flag for redraw dirty flag.
*/
typedef INT32 COMMONFLAG;

/**
    Result of executed event.
*/

typedef enum {
	NVTEVT_PASS = 0,                 ///<event pass to next flow
	NVTEVT_CONSUME,                  ///<event is executed,not pass to next flow
	NVTEVT_FLOW_MAX                  ///<maxmun event flow value
} NVTEVT_FLOW;

/**
    @brief Indicate a control dispaly area
*/
typedef struct _Ux_RECT {
	INT32  x1;                      ///<The left point
	INT32  y1;                      ///<The top point
	INT32  x2;                      ///<The right point
	INT32  y2;                      ///<The bottom point
} Ux_RECT, *PUx_RECT;


/**
    @brief VControl Object Prototype

    VControl defines the behavior of a visual component
*/
typedef struct _VControl {
	char         *Name;                             ///< Name of this object. Used for debug and identification.
	UINT32       wType;                             ///< Control type
	struct _VControl    **ControlList;              ///< An array of (VControl *)
	struct _VControl     *pParent;                  ///< Pointer to control parent
	void                 *usrData;                  ///< user extended data
	//------------------------------------------------------------------------------------------------------------
	struct _EVENT_ENTRY  *EventList;                ///< An array of EVENT_ENTRY arrays (msg handles)
	//------------------------------------------------------------------------------------------------------------
	void                 *CtrlData;                 ///< Depend on control type,there is different data structure
	//------------------------------------------------------------------------------------------------------------
	struct _ITEM_BASE   **ShowList;                 ///< Tool would generate show obj in list
	Ux_RECT rect;                                   ///< The original point of the control (usually 0,0)
	COMMONFLAG flag;                                ///< if dirty -> post NVTEVT_REDRAW
	//------------------------------------------------------------------------------------------------------------
} VControl;


/**
    @brief Event handler function prototype
*/
typedef INT32(* NVTEVT_EVENT_FUNC_PTR)(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

/**
    @brief Command mapping table entry
*/
typedef struct _EVENT_ENTRY {
	NVTEVT event;                                ///< The handled event
	NVTEVT_EVENT_FUNC_PTR pfn;                   ///< The event handler of the event
} EVENT_ENTRY;

/**
    @brief Command mapping table
*/
typedef EVENT_ENTRY *EVENT_TABLE;

/**
    create application control.
*/
#define CREATE_APP(Name,Type)        \
	VControl Name##Ctrl={ \
						  #Name,            \
						  Type,             \
						  0,   \
						  0,           \
						  0,                \
						  Name##CmdMap,     \
						  0,             \
						  0,  \
						  {0,0,0,0},    \
						  0                 \
						};


/**
    Send event to control.

    Send event to contorl.This API would loop up the control event table to find match event.
    Then execute event function.
    @note This API would not change task,it use the caller task.

    @param[in] pCtrl The pointer of the control
    @param[in] evt   Event.
    @param[in] paramNum How many parameters given to the control. Acceptable value:0~3
    @param ... Variable number arguments according to paramNum

    @return
        - @b NVTEVT_CONSUME:         event is be recesived
        - @b NVTEVT_PASS:            pass to next flow.
*/

extern INT32 Ux_SendEvent(VControl *pCtrl, NVTEVT evt,  UINT32 paramNum, ...);

/**
    Execute default event behavior.

    Execute default event behavior of the control.Different control has different default event function.
    If the contorl has no this event function,it would return NVTEVT_PASS.
    @note This API would not change task,it use the caller task.

    @param[in] pCtrl The pointer of the control
    @param[in] evt   Event.
    @param[in] paramNum How many parameters given to the control. Acceptable value:0~3
    @param[in] paramArray   param array.

    @return
        - @b NVTEVT_CONSUME:         event is be recesived
        - @b NVTEVT_PASS:            pass to next flow.
*/
extern INT32 Ux_DefaultEvent(VControl *pCtrl, NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);

/**
    Get control base type control.\n
    Each control has its own type.Each type as base control.
    This API would get the base type control of the given control.

    @param[in] pCtrl The pointer of control.
    @return The pointer of base type control.
*/
extern VControl *Ux_GetBaseType(VControl *pCtrl);

/**
    Set UI type control list.

    @note must be UI type in control list.

    @param[in] pCtrlTable The pointer of UI control list.
    @param[in] size       How many control in list.

    @return void.
*/
extern void Ux_SetUITypeList(VControl **pCtrlTable, INT32 size);

/**
    Get UI type control list.

    @return The pointer of UI control list.
*/
extern VControl **Ux_GetUITypeList(void);

/**
    Check if control is UI type control.

    @param[in] pCtrl The pointer of the control.

    @return
        - @b TRUE The control is UI type control
        - @b FALSE The control is not UI type control
*/
extern BOOL Ux_IsUIControl(VControl *pCtrl);

/**
    Set application type control list.
    @note must be application type in control list.

    @param[in] pCtrlTable The pointer of application control list.
    @param[in] size       How many control in list.

    @return void.
*/
extern void Ux_SetAppTypeList(VControl **pCtrlTable, INT32 size);

/**
    Get application type control list.

    @return The pointer of application control list.
*/
extern VControl **Ux_GetAppTypeList(void);

/**
    Check if control is application type control.

    @param[in] pCtrl The pointer of the control.

    @return
        - @b TRUE The control is application type control
        - @b FALSE The control is not application type control
*/
extern BOOL Ux_IsAppControl(VControl *pCtrl);

/**
    Set active application control.

    If user doesn't assign control in Ux_SendEvent,SendEvent would get active
    application control as user control.
    @note must be application type control

    @param[in] pApp The pointer of the application control.

    @return void.
*/
extern void Ux_SetActiveApp(VControl *pApp);

/**
    Get active application control.

    @return The pointer of active application control.
*/
extern VControl *Ux_GetActiveApp(void);

//@}

#endif //NVT_VCONTROL_H
