/*****************************************************************************
 **
 **  Name    upio_bby.c
 **
 **  Description
 **  This file contains the universal driver wrapper for the BTE-QC pio
 **  drivers
 **
 **  Copyright (c) 2001-2004, WIDCOMM Inc., All Rights Reserved.
 **  WIDCOMM Bluetooth Core. Proprietary and confidential.
 *****************************************************************************/
#include "bt_target.h"
#include "gki.h"
#include "upio.h"
#include "gpio.h"
#include "clock.h"
/*
#include "wiced.h"
#include "wiced_rtos.h"
#include "wiced_utilities.h"
#include "bt_bus.h"
#include "wiced_bt_platform.h"
*/
const int  HCILP_BT_WAKE_POLARITY = 1;
const int HCILP_HOST_WAKE_POLARITY = 1;

/******************************************************
 *               Variable Definitions
 ******************************************************/

//static volatile wiced_bool_t bus_initialised = WICED_FALSE;
//static volatile wiced_bool_t device_powered = WICED_FALSE;

/*******************************************************************************
 **  UPIO Driver functions
 *******************************************************************************/

/*****************************************************************************
 **
 ** Function         UPIO_Init
 **
 ** Description
 **      Initialize the GPIO service.
 **      This function is typically called once upon system startup.
 **
 ** Returns          nothing
 **
 *****************************************************************************/
UDRV_API void UPIO_Init( void *p_cfg )
{
    DRV_TRACE_DEBUG0("UPIO_Init\r\n");
}

/*****************************************************************************
 **
 ** Function         UPIO_Set
 **
 ** Description
 **      This function sets one or more GPIO devices to the given state.
 **      Multiple GPIOs of the same type can be masked together to set more
 **      than one GPIO. This function can only be used on types UPIO_LED and
 **      UPIO_GENERAL.
 **
 ** Input Parameters:
 **      type    The type of device.
 **      pio     Indicates the particular GPIOs.
 **      state   The desired state.
 **
 ** Output Parameter:
 **      None.
 **
 ** Returns:
 **      None.
 **
 *****************************************************************************/
UDRV_API void UPIO_Set( tUPIO_TYPE type, tUPIO pio, tUPIO_STATE state )
{
    //DRV_TRACE_DEBUG2("UPIO_Set %d, %s\r\n", pio, UPIO_OFF == state ? "UPIO_OFF" : "UPIO_ON");
#if HCILP_INCLUDED
    if(UPIO_OFF == state)
    {
        gpio_clearPin(GPIO_BT_WAKE);
    }else
    {
        gpio_setPin(GPIO_BT_WAKE);
    }

    if( HCILP_BT_WAKE_POLARITY !=  state)
    {
        void start_idle_timer(void);
        start_idle_timer();
    }
#endif
}

/*****************************************************************************
 **
 ** Function         UPIO_Read
 **
 ** Description
 **      Read the state of a GPIO. This function can be used for any type of
 **      device. Parameter pio can only indicate a single GPIO; multiple GPIOs
 **      cannot be masked together.
 **
 ** Input Parameters:
 **      Type:  The type of device.
 **      pio:    Indicates the particular GUPIO.
 **
 ** Output Parameter:
 **      None.
 **
 ** Returns:
 **      State of GPIO (UPIO_ON or UPIO_OFF).
 **
 *****************************************************************************/
UDRV_API tUPIO_STATE UPIO_Read( tUPIO_TYPE type, tUPIO pio )
{
    DRV_TRACE_DEBUG1("UPIO_Read %d\r\n", pio);
#if HCILP_INCLUDED
    return (tUPIO_STATE)!!gpio_getPin(GPIO_HOST_WAKE);

#else
    return UPIO_OFF;
#endif

}

/*****************************************************************************
 **
 ** Function         UPIO_Config
 **
 ** Description      - Configure GPIOs of type UPIO_GENERAL as inputs or outputs
 **                  - Configure GPIOs to be polled or interrupt driven
 **
 **
 ** Output Parameter:
 **      None.
 **
 ** Returns:
 **      None.
 **
 *****************************************************************************/
UDRV_API void UPIO_Config( tUPIO_TYPE type, tUPIO pio, tUPIO_CONFIG config, tUPIO_CBACK *cback )
{
    DRV_TRACE_DEBUG0("UPIO_Config");
}

