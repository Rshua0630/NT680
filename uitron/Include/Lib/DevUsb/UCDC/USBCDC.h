/**
    UCDC, APIs declare.

    @file       USBCDC.h
    @ingroup    mUCDC
    @note       --

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _UCDCAPI_H
#define _UCDCAPI_H

#include "usb.h"
#include "usb3dev.h"

#define UCDC_API_VERSION 0x11012008U


typedef struct _USB_CDC_INFO {
	UINT16              VID;                ///< Customized USB Vendor ID
	UINT16              PID;                ///< Customized USB Product ID
	USB_STRING_DESC     *pManuStringDesc;   ///< USB Manufacturer String Descriptor pointer.
	USB_STRING_DESC     *pProdStringDesc;   ///< USB Product String Descriptor pointer.
	USB_STRING_DESC     *pSerialStringDesc; ///< USB Serial Number String Descriptor pointer.
	UINT32              uiUcdcBufAddr;
	UINT32              uiUcdcBufSize;
} USB_CDC_INFO;


/**
    Supported CDC PSTN request codes.

    This definition is used for CDC_PSTN_REQUEST_CB.
*/
typedef enum _CDC_PSTN_REQUEST {
	REQ_SET_LINE_CODING         =    0x20,
	REQ_GET_LINE_CODING         =    0x21,
	REQ_SET_CONTROL_LINE_STATE  =    0x22,
	REQ_SEND_BREAK              =    0x23,
	ENUM_DUMMY4WORD(CDC_PSTN_REQUEST)
} CDC_PSTN_REQUEST;

/**
     Line coding structure.
*/
typedef _PACKED_BEGIN struct {
	UINT32   uiBaudRateBPS; ///< Data terminal rate, in bits per second.
	UINT8    uiCharFormat;  ///< Stop bits.
	UINT8    uiParityType;  ///< Parity.
	UINT8    uiDataBits;    ///< Data bits (5, 6, 7, 8 or 16).
} _PACKED_END CDCLineCoding;

/**
    Callback function prototype for PSTN subclass request.
*/
typedef BOOL (*CDC_PSTN_REQUEST_CB)(CDC_PSTN_REQUEST Code, UINT8 *pData, UINT32 *pDataLen);


/**
    CDC configuration identifier.

    This definition is used in CDC_SetConfig() to assgin new configuration to specified ID function.
*/
typedef enum _CDC_CONFIG_ID {
	CDC_CONFIG_PSTN_REQUEST_CB,          ///> callback function for PSTN subclass specific request
	CDC_CONFIG_ASYNC_WRITE_MODE,         ///> asynchronous write mode
	ENUM_DUMMY4WORD(CDC_CONFIG_ID)
} CDC_CONFIG_ID;

/**
    CDC asynchronous write mode.

    This definition is used to config CDC_CONFIG_ASYNC_WRITE_MODE.
*/
typedef enum _CDC_ASYNC_WRITE_MODE {
	CDC_ASYNC_WRITE_DATA_MODE,              ///> write data mode
	CDC_ASYNC_WRITE_DEBUG_MODE,             ///> debug message mode
	ENUM_DUMMY4WORD(CDC_ASYNC_WRITE_MODE)
} CDC_ASYNC_WRITE_MODE;

/**
     Install UCDC task,flag and semaphore id

     This is default install API. User can create own task id, semphore id,
     flag id to adjust own stack size and task priority as following:

     @code
     {
        #define PRI_UCDC             20
        #define STKSIZE_UCDC         2048

        UINT32 UCDC_TSK_ID = 0;
        UINT32 UCDC_FLG_ID = 0;
        UINT32 UCDC_SEM_ID = 0;

        void UCDC_InstallID(void)
        {
            OS_CONFIG_TASK(UCDC_TSK_ID,PRI_UCDC,STKSIZE_UCDC,UCDCTsk);
            OS_CONFIG_FLAG(UCDC_FLG_ID);
            OS_CONFIG_SEMPHORE(UCDC_SEM_ID,0,1,1);
        }
     }
     @endcode
*/
extern void CDC_InstallID(void) _SECTION(".kercfg_text");


/**
    Set CDC configuration

    Assign new configuration of the specified ConfigID.

    @param[in] ConfigID         Configuration identifier
    @param[in] Value            Configuration context for ConfigID
*/
extern void CDC_SetConfig(CDC_CONFIG_ID ConfigID, UINT32 Value);

/**
    Open USB CDC module.

    @param[in] pClassInfo Pointer to the structure of USB CDC class information: #_USB_CDC_INFO.
    @return
        - @b E_OK:  Success.
        - @b E_SYS: Failed
*/
extern ER CDC_Open(USB_CDC_INFO *pClassInfo);

/**
    Close USB CDC module.
*/
extern ER CDC_Close(void);

/**
    Get data count in byte.
    This API could check if there is any data coming from USB host.

    @return data count in byte.
*/
extern UINT32 CDC_GetDataCount(void);

/**
    Read data from USB host.
    This API doesn't return until there is any data coming from USB host or CDC_AbortRead() is invoked.

    @param[in] pBuf   Buffer pointer.
    @param[in,out] pBufSize  Input length to read, output actual transfered length.

    @return
        - @b E_OK:  Success.
        - @b E_SYS: Failed
        - @b E_RSATR: Invoked by another task.
*/
extern ER CDC_ReadData(UINT8 *pBuf, UINT32 *pBufSize);

/**
    Abort the function of CDC_ReadData().
*/
extern void CDC_AbortRead(void);

/**
    Write data to USB host.

    This function is a non-blocking API and will return immediately.
    Used with CDC_AsyncWaitWriteDone() to wait for writing done.

    @param[in] pBuf  Buffer pointer
    @param[in] BufSize  Data length to be written. Valid length is 0x1 to 0x7fffff.

    @return
        - @b E_OK: Start to transfer.
        - @b E_SYS: Failed.
*/
extern ER CDC_AsyncWriteData(UINT8 *pBuf, UINT32 BufSize);

/**
    Wait for writing done.

    This function is a blocking API and will return after data is sent to USB host.

    @return
        - @b Actual transfered length.
*/
extern UINT32 CDC_AsyncWaitWriteDone(void);

/**
    Write data to USB host.

    This function is a blocking API and will return after data is sent to USB host.

    @param[in] pBuf  Buffer pointer
    @param[in] pBufSize Data length to be written. Valid length is 0x1 to 0x7fffff.

    @return
        - @b Actual transfered length.
*/
extern UINT32 CDC_WriteData(UINT8 *pBuf, UINT32 *pBufSize);
#endif

