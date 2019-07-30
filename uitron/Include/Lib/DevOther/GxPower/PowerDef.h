/**
    Power battery level definition.

    @file       PowerDef.h
    @ingroup    mILibPowerCtrl

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _POWERDEF_H
#define _POWERDEF_H

/*
    @addtogroup mILibPowerCtrl
*/

//-----------------------------------------------------------------------------
// Power global definitions
//-----------------------------------------------------------------------------
/**
     @name Battery level definition
*/
//@{
#define VOLDET_BATTERY_LVL_UNKNOWN      0xFFFFFFFF         ///<  battery level is unknown, the initial value before battery detection start
#define VOLDET_BATTERY_LVL_EMPTY        5                  ///<  battery is empty
#define VOLDET_BATTERY_LVL_0            0                  ///<  battery level 0
#define VOLDET_BATTERY_LVL_1            1                  ///<  battery level 1
#define VOLDET_BATTERY_LVL_2            2                  ///<  battery level 2
#define VOLDET_BATTERY_LVL_3            3                  ///<  battery level 3
#define VOLDET_BATTERY_LVL_4            4                  ///<  battery level 4
//@}

#endif //_POWERDEF_H
