/**
    This system for ISP pseudo driver on Linux command dispatch and execution.

    @file       IspSudoApi.h

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/

#ifndef _ISPSUDO_API_H
#define _ISPSUDO_API_H

#include "SysKer.h"


/**
     API Version
*/
#define ISPSUDO_API_VERSION    0x16080511U

/**
     The minimun stack size requirement for IspSudoTsk
*/
#define ISPSUDO_REQUIRE_MIN_SIZE    0x020100    // 0x40 + 0xB8 + 0x010000 + 0x010000


/**
    Event

    IspSudo reports some events
*/
typedef enum _ISPSUDO_EVENT {
	ISPSUDO_EVENT_HOOK_DBG_MSG_ON,      ///< turn on usb debug message
	ISPSUDO_EVENT_HOOK_DBG_MSG_OFF,     ///< turn off usb debug message
	ENUM_DUMMY4WORD(ISPSUDO_EVENT)
} ISPSUDO_EVENT;

typedef void (*ISPSUDO_CB_EVENT)(ISPSUDO_EVENT event);

/**
    Handler type

    IspSudo allows to register following handler
*/
typedef enum _ISPSUDO_HANDLER_TYPE {
	ISPSUDO_HANDLER_TYPE_UNKNOWN,              ///< reversed
	ISPSUDO_HANDLER_TYPE_VIRTUAL_MEM_MAP,      ///< ISPSUDO_CB_VIRTUAL_MEM_READ, Handler that PC read memory 0xF0000000~0xFFFFFFFF via registered handler
	ENUM_DUMMY4WORD(ISPSUDO_HANDLER_TYPE)
} ISPSUDO_HANDLER_TYPE;

typedef UINT32(*ISPSUDO_CB_VIRTUAL_MEM_READ)(UINT32 MemOffset, UINT32 MemSize);

/**
     IspSudo configuration strucutre

     Set working buffer, semaphore, task id for Ipd vendor system using via IspSudo_Init() before calling Ipd_Open()
*/
typedef struct _ISPSUDO_INIT {
	UINT32 uiApiVer;                                ///< just assign to ISPSUDO_API_VERSION
	UINT8 *pMemCache;                               ///< cache-able Memory (ISPSUDO_REQUIRE_MIN_SIZE Bytes Requirement) Front-End:0x10000,Back-End:0x10000,Msg:0x4000,CBW:0x40
	UINT32 uiSizeCache;                             ///< n Bytes of cache memory
	ISPSUDO_CB_EVENT fpEvent;                       ///< export IspSudo events
} ISPSUDO_INIT, *PISPSUDO_INIT;

//Bi-Direction Call Function Unit
/**
     Bi Unit

     ISP pseudo Bi-Direction Call Function Unit
*/
typedef struct _ISPSUDO_BI_CALL_UNIT {
	char *szName;                                     ///< function name
	void (*fp)(void *pData);                          ///< handler callback
} ISPSUDO_BI_CALL_UNIT;

/**
    Structure used for MSDC Vendor Command Callback API

    This structure is used in the callback prototype MSDC_Vendor_CB to specify the needed parameters for vendor command processing.
*/
typedef struct {
	UINT32    OutDataBufLen;                                        ///< Data Buffer Length for "Host to MSDC-task" transaction.

	UINT32    InDataBuf;                                            ///< Data Buffer Address for "MSDC-task to Host" transaction. MSDC-task would prepare this buffer for callback function.
	UINT32    InDataBufLen;                                         ///< Host assigned "MSDC-task to Host" transaction length in byte count.

	UINT32    VendorCmdBuf;                                         ///< Vendor CBW buffer that device will receive from host for CBW buffer. Fixed 31 bytes.
	UINT32    VendorCSWBuf;                                         ///< Vendor CSW buffer that device will return to host for CSW buffer, MSDC task should prepare this buffer for callback function, Fixed 13 bytes.
} ISPSUDOVendorParam, *PISPSUDOVendorParam;

/**
     Install task, flag, and semaphore id
*/
extern void IspSudo_InstallID(void) _SECTION(".kercfg_text");

/**
     ISP pseudo vendor configuration

     Configure ISP pseudo for system initial

     @param[in] pInit       Configuration strucutre
     @return
         - @b TRUE:   configuration success.
         - @b FALSE:  configuration failed.
*/
extern BOOL IspSudo_Init(ISPSUDO_INIT *pInit);

/**
     ISP pseudo vendor exit

     Quit ISP pseudo

     @return
         - @b TRUE:   exit success.
         - @b FALSE:  failed to exit.
*/
extern BOOL IspSudo_Exit(void);

/**
     ISP pseudo verify callback

     A callback for plug-in USB_IPD_INFO.ipd_check_cb
*/
extern BOOL IspSudo_Verify_Cb(UINT32 pCmdBuf, UINT32 *pDataBuf);

/**
     ISP pseudo verify callback
*/
extern void IspSudo_Vendor_Cb(PISPSUDOVendorParam pBuf);

/**
     Get host-device common data buffer address

     Get data address for single directtion callback function

     @return
         - @b NULL:     failed to get buffer address.
         - @b NON-NULL: buffer address.
*/
extern UINT8 *IspSudo_GetDataBufferAddress(void);

/**
     Get host-device common data buffer size (byte unit)

     Get data size for single directtion callback function

     @return
         @return
         - @b 0:        failed to get buffer size.
         - @b Non-Zero: buffer size.
*/
extern UINT32 IspSudo_GetDataBufferSize(void);

/**
     Register single-direction type callback function

     Plugin vendor function(single-direction,from PC-Get and PC-Set)

     @param[in] fpTblGet function table for host get data
     @param[in] nGets    number of functions in fpTblGet table
     @param[in] fpTblSet function table for host set data
     @param[in] nSets    number of functions in fpTblSet table
     @return
         - @b TRUE:    success to register.
         - @b FALSE:   failed to register callback function.
*/
extern BOOL IspSudo_AddCallback_Si(FP *fpTblGet, UINT8 nGets, FP *fpTblSet, UINT8 nSets);

/**
     Get transmitted data size

     For IPDVendorNVT_AddCallback_Si functions, get pc how many data is transmitted

     @return data size (byte unit).
*/
extern UINT32 IspSudo_GetTransSize(void);

/**
     ISP pseudo vendor main task

     This task is used for do background command, currently.
*/
extern void IspSudoTsk(void);

/**
     ISP pseudo Turn ON / OFF Net Service

     After initiating, with CPU2 started, call it on for NET supporting.
     before CPU2 closed, call it off for next NET supporting.

     @param[in] bTurnOn Turn it ON / OFF
     @return
         - @b TRUE:   success.
         - @b FALSE:  failed.
*/
extern BOOL IspSudo_Net(BOOL bTurnOn);


//------------------------------------------------------------
// Utility Functions
//------------------------------------------------------------
/**
     Handler to read memory 0xF0000000 ~ 0xFFFFFFFF

     @return
         - @b TRUE:   success.
         - @b FALSE:  failed.
*/
extern BOOL IspSudo_RegHandler(ISPSUDO_HANDLER_TYPE Type, void *pCb);

#endif

