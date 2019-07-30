/*
    Copyright (c) 2014  Novatek Microelectronics Corporation

    @file usbipc_api.h

    @version

    @date
*/

#ifndef _USBIPCAPI_H
#define _USBIPCAPI_H

#include "Type.h"

//-------------------------------------------------------------------------
//IPC command id and parameters
typedef enum {
	USBIPC_CMDID_MSDC,
	USBIPC_CMDID_MODE,
	USBIPC_CMDID_MAX,
} USBIPC_CMDID;

//-------------------------------------------------------------------------
//USBIPC definition

typedef struct {
	UINT32                  sharedMemAddr;  // shared memory address
	UINT32                  sharedMemSize;  // shared memory size
} USBIPC_OPEN;

typedef struct {
	int  luns;
	int  id_vendor;
	int  id_product;
	char manufacturer[254];
	char product[254];
	char serial[254];
} USBIPC_MSDC_INFO;

typedef enum {
	USBIPC_CONFIG_MSDC_INFO,
	USBIPC_CONFIG_MAX,
} USBIPC_CONFIG;

typedef enum {
	USB_MODE_HOST,
	USB_MODE_DEVICE,
	USB_MODE_MAX,
} USB_MODE_TYPE;

/**
    Install usbipc flag and semaphore id.

    @return void.
*/
extern void usbipc_install_id(void) _SECTION(".kercfg_text");

/**
    Open usbipc task.

    This is usbipc initialization function.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER usbipc_open(USBIPC_OPEN *pOpen);

/**
    Close usbipc task.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER usbipc_close(void);

/**
    Send USB IPC command

    @param[in] cmd        USB IPC command.
    @param[in] param1     parameter 1.
    @param[in] param2     parameter 2.

    @return
        - @b E_SYS: Send IPC failed.
        - @b E_OK:  Success.
*/
extern ER usbipc_send_cmd(USBIPC_CMDID cmd, UINT32 param1, UINT32 param2);

/**
    Set USB IPC configurations

    @param[in] cfg        USB IPC configuration.
    @param[in] param1     parameter 1.
    @param[in] param2     parameter 2.

    @return
        - @b E_PAR: Parameter error.
        - @b E_OK:  Success.
*/
extern ER usbipc_setconfig(USBIPC_CONFIG cfg, UINT32 param1, UINT32 param2);

#endif /* _USBIPCAPI_H  */
