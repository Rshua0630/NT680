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
#include "Delay.h"

#define GPIO_BT_WAKE        P_GPIO_0
//#define GPIO_HOST_WAKE      P_GPIO_0
#define GPIO_BT_REG         P_GPIO_1
//#define GPIO_BT_POWER       P_GPIO_6

const int  HCILP_BT_WAKE_POLARITY = 1;
const int HCILP_HOST_WAKE_POLARITY = 1;

void utron_setup_gpio(void);

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
    /*if(UPIO_OFF == state)
    {
        gpio_clearPin(BT_WAKE);
    }else
    {
        gpio_setPin(BT_WAKE);
    }*/
	//DRV_TRACE_DEBUG0("BT_WAKE not configured\r\n");

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
    //return (tUPIO_STATE)!!gpio_getPin(GPIO_HOST_WAKE);
    DRV_TRACE_DEBUG0("GPIO_HOST_WAKE not configured\r\n");
    return UPIO_OFF;
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


void utron_setup_gpio(void)
{
    DRV_TRACE_DEBUG0("utron_setup_gpio\r\n");
    gpio_setDir(GPIO_BT_WAKE, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_BT_WAKE);
    
    //gpio_setDir(GPIO_HOST_WAKE, GPIO_DIR_OUTPUT);
    //gpio_setPin(GPIO_HOST_WAKE);

    gpio_setDir(GPIO_BT_REG, GPIO_DIR_OUTPUT);
    //gpio_setDir(GPIO_BT_POWER, GPIO_DIR_OUTPUT);

    //gpio_clearPin(GPIO_BT_POWER);
    gpio_clearPin(GPIO_BT_REG);
    Delay_DelayMs(100);
    //gpio_setPin(GPIO_BT_POWER);
    gpio_setPin(GPIO_BT_REG);


#if HCILP_INCLUDED
    gpio_setDir(GPIO_BT_WAKE, GPIO_DIR_OUTPUT);
    #if 1
    gpio_setPin(GPIO_BT_REG);
    #else
    gpio_clearPin(GPIO_BT_REG);
    #endif


#endif
}

