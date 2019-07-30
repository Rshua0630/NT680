/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       gpio_info.h
    @ingroup    mMODELEXT

    @brief      Header file of IO config
                This file is the header file of IO config

    @note       Nothing.
    @date       2016/06/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#ifndef _GPIO_INFO_H
#define _GPIO_INFO_H

#if defined(__UITRON)
#include "Type.h"
#else
#include "nvt_type.h"
#endif

#define GPIO_INFO_VER           0x16062414 ///< YYYY/MM/DD HH


//--------------------------------------------------------------------
// GPIO common
//--------------------------------------------------------------------
typedef struct _GPIO_INIT_OBJ {
	UINT32  GpioPin;
	UINT32  GpioDir;
	UINT32  PadDir;
	UINT32  PadPin;
} GPIO_INIT_OBJ, *PGPIO_INIT_OBJ;

#endif

//@}

