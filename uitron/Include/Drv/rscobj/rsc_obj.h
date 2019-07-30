/**
    Public header file for RSC obj

    This file is the header file that define the API and data type for RSC ctrl obj.

    @file       RSC_obj.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _RSC_OBJ_H
#define _RSC_OBJ_H

#include "Type.h"
#include "sensor.h"


#define RSC_SURRPORT_ID 3 // equal to IPL_ID_4

/**
    extend library common interface
*/
typedef enum {
	SEL_RSC_IDLE = 0,
	SEL_RSC_CHANGEMODE,
	SEL_RSC_RUNTIME,
	ENUM_DUMMY4WORD(SEL_RSC_MODECHG)
} SEL_RSC_MODECHG;
/**
    Rolling Shutter Correction On/Off
*/
typedef enum {
	SEL_RSC_OFF = 0,            ///<-
	SEL_RSC_ON,                 ///<-
	SEL_RSC_MAX,
	ENUM_DUMMY4WORD(SEL_RSC_STATUS)
} SEL_RSC_STATUS;

typedef enum {
	SEL_RSC_O_RATIO = 0,         ///<- ORIGINAL RSC RATIO
	SEL_EIS_O_RATIO,             ///<- ORIGINAL EIS RATIO
	SEL_RSC_RATIO ,              ///<- RSC RATIO FOR IPL
	SEL_EIS_RATIO,				 ///<- EIS RATIO FOR IPL
	ENUM_DUMMY4WORD(SEL_RSC_GETRATIO)
} SEL_RSC_GETRATIO;

typedef struct {
	//Common
	void (*fp_set_switch)(UINT32 Id, SEL_RSC_MODECHG Value, SEL_RSC_STATUS Value2);
	SEL_RSC_STATUS (*fp_rsc_enable)(UINT32 Id);
	UINT32 (*fp_get_eisraio)(UINT32 Id,SEL_RSC_GETRATIO ratio_type,SENSOR_MODE NextSensoeMode);
} RSC_EXT_LIB_TAB;

typedef struct {
	RSC_EXT_LIB_TAB *ext_lib_tab;
} RSC_HANDLE;

extern BOOL rsc_ext_lib_init(UINT32 id, RSC_HANDLE *handle);

//@}
#endif
