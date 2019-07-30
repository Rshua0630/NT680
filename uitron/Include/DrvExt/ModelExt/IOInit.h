/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOInit.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of IO
                This file is the header file of IO init API

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _IOINIT_H
#define _IOINIT_H

//--------------------------------------------------------------------
// Definitions for APIs
//--------------------------------------------------------------------
extern void IO_InitPinmux(void);
extern void IO_InitIntDir(void);
extern void IO_InitGPIO(void);
extern void IO_InitADC(void);
extern void IO_InitNet(void);

#endif //IOINIT
//@}

