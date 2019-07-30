/**
    UI zoom control related API.

    UI  zoom control export variables and function prototypes.

    @file       UICtrlZoomLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICTRLZOOMLIB_H
#define UICTRLZOOMLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
    Zoom position attribute
*/
typedef enum {
	ZOOM_EXTERNAL_RECT = 1,     ///< Zoom external rectangle(Big rectangle)
	ZOOM_INTERNAL_RECT          ///< Zoom internal rectangle(small rectangle)
} ZOOM_POS_ATTRIBUTE_SET;


/**
     Zoom control data structure
*/
typedef struct _CTRL_ZOOM_DATA {
	UINT32 value;                ///< Zoom value
} CTRL_ZOOM_DATA;

/**
    Zoom control data attribute index
*/
typedef enum {
	ZOOM_VALUE = (CTRL_ZOOM << 8),
	ZOOM_MAX
} ZOOM_DATA_SET;

/**
    Set zoom data
    @param[in] pCtrl The pointer of the zoom control
    @param[in] attribute Zoom attribute(ZOOM_DATA_SET)
    @param[in] value The static value would be changed
*/
extern void UxZoom_SetData(VControl *pCtrl, ZOOM_DATA_SET attribute, UINT32 value);

/**
    Get zoom data
    @param[in] pCtrl The pointer of the zoom control
    @param[in] attribute Zoom attribute(ZOOM_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxZoom_GetData(VControl *pCtrl, ZOOM_DATA_SET attribute);

/**
    Get different zoom attribute rectangle
    @param[in] pCtrl The pointer of the zoom control
    @param[in] attribute Zoom position attribute(ZOOM_POS_ATTRIBUTE_SET)
    @param[out] pRect The pointer of zoom attribute rectangle
*/
extern void UxZoom_SetPosition(VControl *pCtrl, ZOOM_POS_ATTRIBUTE_SET attribute, Ux_RECT *pRect);
//@}
#endif
