/**
    Msdc-Nvt Vendor

    This system for PC control device(DSC) via MSDC. User can register callbacks
    to respond PC command.

    @file       MsdcNvtApi.h
    @ingroup    mMSDCNVT
    @note       Suggest Task Stack Size is 8192. Suggest Task Priority is 10.
                This library depends on Msdc and Strg lib.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _MSDCNVTAPI_H
#define _MSDCNVTAPI_H

#include "SysKer.h"
#include "Dx.h"
#include "UMSDC.h"

/**
    @addtogroup mMSDCNVT
*/
//@{

/**
     API Version
*/
#define MSDCNVT_API_VERSION 0x16080511U

/**
     The minimun stack size requirement for MsdcNvtTsk
*/
#if (MSDC_MIN_BUFFER_SIZE>0x240BC)
#define MSDCNVT_REQUIRE_MIN_SIZE MSDC_MIN_BUFFER_SIZE
#else
#define MSDCNVT_REQUIRE_MIN_SIZE 0x240BC
#endif

/**
     @name Macro Tools
*/
//@{
#define MSDCNVT_CAST(type,p_data) (type*)MsdcNvt_ChkParameters(p_data,sizeof(type)) ///< Check Begin Parameters from Host. Used in Bi-Direction functions.
//@}

/**
     @name BiDirection function register
*/
//@{
#define MSDCNVT_REG_BI_BEGIN(tbl) static MSDCNVT_BI_CALL_UNIT (tbl)[]={ ///< declare bi-direction function table begin
#define MSDCNVT_REG_BI_ITEM(x)  {#x,(x)},                               ///< append a bi-direction function
#define MSDCNVT_REG_BI_END() {NULL,NULL}};                              ///< bi-direction function table end
//@}

/**
    Event

    MsdcNvt reports some events
*/
typedef enum _MSDCNVT_EVENT {
	MSDCNVT_EVENT_HOOK_DBG_MSG_ON,      ///< turn on usb debug message
	MSDCNVT_EVENT_HOOK_DBG_MSG_OFF,     ///< turn off usb debug message
	ENUM_DUMMY4WORD(MSDCNVT_EVENT)
} MSDCNVT_EVENT;

typedef void (*MSDCNVT_CB_EVENT)(MSDCNVT_EVENT event);

/**
    Handler type

    MsdcNvt allows to register following handler
*/
typedef enum _MSDCNVT_HANDLER_TYPE {
	MSDCNVT_HANDLER_TYPE_UNKNOWN,               ///< Reversed.
	MSDCNVT_HANDLER_TYPE_VIRTUAL_MEM_MAP,      ///< MSDCNVT_CB_VIRTUAL_MEM_READ, Handler that PC read memory 0xF0000000~0xFFFFFFFF via registered handler
	ENUM_DUMMY4WORD(MSDCNVT_HANDLER_TYPE)
} MSDCNVT_HANDLER_TYPE;

typedef UINT32(*MSDCNVT_CB_VIRTUAL_MEM_READ)(UINT32 MemOffset, UINT32 MemSize);

/**
     MsdcNvt configuration strucutre

     Set working buffer, semaphore, task id for Msdc vendor system using via MsdcNvt_Init() before calling Msdc_Open()
*/
typedef struct _MSDCNVT_INIT {
	UINT32 uiApiVer;                                ///< just assign to MSDCNVT_API_VERSION
	UINT8 *pMemCache;                               ///< Cache-able Memory (MSDCNVT_REQUIRE_MIN_SIZE Bytes Requirement) Front-End:0x10000,Back-End:0x10000,Msg:0x4000,CBW:0x40
	UINT32 uiSizeCache;                             ///< n Bytes of Cache Memory
	BOOL   bHookDbgMsg;                             ///< Indicate use USB to seeing message and sending command
	UINT32 uiUartIdx;                               ///< Select to 0:uart_putString or 1:uart2_putString to real uart output
	MSDCNVT_CB_EVENT fpEvent;                       ///< export MsdcNvt events
#if defined(_BSP_NA51000_)
	MSDC_OBJ *pMsdc;								///< via USB 2.0 or USB 3.0
#endif
	//net supporting
	UINT32 uiPort;                                  ///< TCP port (set 0 for default 1968)
	UINT32 uiTos;                                   ///< TCP tos value
	UINT32 uiSndBufSize;                            ///< Send Socket Buffer Size (0: system default)
	UINT32 uiRecvBufSize;                           ///< Recv Socket Buffer Size (0: system default)
} MSDCNVT_INIT, *PMSDCNVT_INIT;

/**
     Null Lun

     Msdc-Nvt provide a null lun for easy to setting msdc structure
*/
typedef struct _MSDCNVT_LUN {
	MSDC_TYPE Type;                                 ///< MsdcNvt provide suggest msdc type
	DX_HANDLE hStrg;                                ///< MsdcNvt provide suggest dx handle of storage
	MSDC_StorageDet_CB fpStrgDetCb;                 ///< MsdcNvt provide suggest card detection callback
	MSDC_StrgLockDet_CB fpStrgLockCb;               ///< MsdcNvt provide suggest card lock detection callback
	UINT32 uiLUNs;                                  ///< MsdcNvt provide suggest the number of lun.
} MSDCNVT_LUN, *PMSDCNVT_LUN;

//Bi-Direction Call Function Unit
/**
     Bi Unit

     Msdc-Nvt Bi-Direction Call Function Unit
*/
typedef struct _MSDCNVT_BI_CALL_UNIT {
	char   *szName;                                     ///< Function Name
	void (*fp)(void *pData);                            ///< Handler Callback
} MSDCNVT_BI_CALL_UNIT;

/**
     Install task,flag and semaphore id
*/
extern void MsdcNvt_InstallID(void) _SECTION(".kercfg_text");

/**
     Msdc-Nvt vendor configuration

     Configure Msdc-Nvt for system initial

     @param[in] pInit       Configuration strucutre
     @return
         - @b TRUE:   configuration success.
         - @b FALSE:  configuration failed.
*/
extern BOOL MsdcNvt_Init(MSDCNVT_INIT *pInit);

/**
     Msdc-Nvt vendor exit

     Quit Msdc-Nvt

     @return
         - @b TRUE:   exit success.
         - @b FALSE:  failed to exit.
*/
extern BOOL MsdcNvt_Exit(void);

/**
     Msdc verify callback

     A callback for plug-in USB_MSDC_INFO.msdc_check_cb
*/
extern BOOL MsdcNvt_Verify_Cb(UINT32 pCmdBuf, UINT32 *pDataBuf);

/**
     Msdc verify callback

     A callback for plug-in USB_MSDC_INFO.msdc_vendor_cb
*/
extern void MsdcNvt_Vendor_Cb(PMSDCVendorParam pBuf);

/**
     Get host-device common data buffer address

     Get data address for single directtion callback function

     @return
         - @b NULL:     failed to get buffer address.
         - @b NON-NULL: buffer address.
*/
extern UINT8 *MsdcNvt_GetDataBufferAddress(void);

/**
     Get host-device common data buffer size (byte unit)

     Get data size for single directtion callback function

     @return
         @return
         - @b 0:        failed to get buffer size.
         - @b Non-Zero: buffer size.
*/
extern UINT32 MsdcNvt_GetDataBufferSize(void);

/**
     Register bi-direction type callback function

     PlugIn vendor function(bi-direction, form PC set then get automatic)

     @param[in] pUnit the structure pointer that bi-direction needs
     @return
         - @b TRUE:    success to register.
         - @b FALSE:   failed to register callback function.
*/
extern BOOL MsdcNvt_AddCallback_Bi(MSDCNVT_BI_CALL_UNIT *pUnit);

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
extern BOOL MsdcNvt_AddCallback_Si(FP *fpTblGet, UINT8 nGets, FP *fpTblSet, UINT8 nSets);

/**
     Get transmitted data size

     For MSDCVendorNVT_AddCallback_Si functions, get pc how many data is transmitted

     @return data size (byte unit).
*/
extern UINT32 MsdcNvt_GetTransSize(void);

/**
     Msdc-Nvt vendor main task

     This task is used for do background command, currently.
*/
extern void MsdcNvtTsk(void);

/**
     Msdc-Nvt Turn ON / OFF Net Service

     After initiating, with CPU2 started, call it on for NET supporting.
     before CPU2 closed, call it off for next NET supporting.

     @param[in] bTurnOn Turn it ON / OFF
     @return
         - @b TRUE:   success.
         - @b FALSE:  failed.
*/
extern BOOL MsdcNvt_Net(BOOL bTurnOn);


//------------------------------------------------------------
// Utility Functions
//------------------------------------------------------------
/**
     Check Parameters

     for all registered callback as MsdcNvtCb_xxxxx, to check data valid.


     @param[in] pData the implemented callback input include tMSDCEXT_PARENT.
     @param[in] uiSize the size of data that you except
     @return
         - @b Non-Zero: valid data
         - @b NULL: invalid data
*/
void   *MsdcNvt_ChkParameters(void *pData, UINT32 uiSize);

/**
     NULL Lun

     Due to attach MSDC application need plug-in a stroage, here provide a virtual
     stroage for user plug-in.

     @return
         - @b virtual stroage information
*/
MSDCNVT_LUN *MsdcNvt_GetNullLun(void);

/**
     Handler to read memory 0xF0000000 ~ 0xFFFFFFFF

     @return
         - @b TRUE:   success.
         - @b FALSE:  failed.
*/
extern BOOL MsdcNvt_RegHandler(MSDCNVT_HANDLER_TYPE Type, void *pCb);

/**
     Set Msdc Object for current use

     @param[in] pMsdc Msdc Object
     @return
         - @b TRUE:   success.
         - @b FALSE:  failed.
*/
extern BOOL MsdcNvt_SetMsdcObj(MSDC_OBJ *pMsdc);

/**
     Get current Msdc Object

     @param[in] pMsdc Msdc Object
     @return
         - @b Non-Zero: pointer to Msdc Object.
         - @b NULL:  failed.
*/
extern MSDC_OBJ *MsdcNvt_GetMsdcObj(void);
//@}
#endif
