/**
    @file       display.h
    @ingroup    mISYSDisp

    @brief      Header file for Display control object
                This file is the header file that define the API and data type
                for Display control object
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/** \addtogroup  mISYSDisp*/
//@{

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "dispcomn.h"
#include "displyr.h"
#include "dispctrl.h"
#include "dispdevctrl.h"

/**
    Display Control Object

    The display control object which provides unified interface for display device/engine control.
    The user can get the display object by API: disp_getDisplayObject() first, and then access the display engine through the
    display object. The dispCtrl/dispLyrCtrl/devCtrl would not set load to activate the configurations, the user must set load
    after all the settings is done.
*/
typedef struct {
	const DISP_ID   dispID;                     ///< DISPAY object ID: DISP_1 or DISP_2.
	const PDISP_CAP(*getDispCap)(void);         ///< Display object capabilities.

	ER(*open)(void);                            ///< Open display object.
	ER(*close)(void);                           ///< Close display object.
	BOOL (*isOpened)(void);                     ///< Check if display object is opened or not.

	DISPCTRL        dispCtrl;                   ///< Control interface for common functions of physical display engine.
	DISPLYRCTRL     dispLyrCtrl;                ///< Control interface for layer function control of physical display engine.
	DISPDEVCTRL     devCtrl;                    ///< Display device control such as device open/close/register_access and hook DISPDEV_OBJ.

	void (*load)(BOOL bWaitDone);               ///< Set Load to activate the display configurations.
	void (*waitFrmEnd)(void);                   ///< Wait the display device frame end.
	void (*waitOutDramDone)(void);              ///< Wait output dram done

} DISP_OBJ, *PDISP_OBJ;



//
//  Export Display Object API
//
extern PDISP_OBJ disp_getDisplayObject(DISP_ID DispID);

//@}

#endif
