/**
    Header file for CC (Core Communicator) module.

    This file is the header file that define the API and data type
    for CC module.

    @file       CC.h
    @ingroup    mIDrvSys_CC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _CC_H
#define _CC_H

#if defined(__UITRON)
#include "Type.h"
#else
#include "nvt_type.h"
#endif

/**
    @addtogroup mIDrvSys_CC
*/
//@{

/**
    core communication core ID number

    @note For cc_hwLockResource(), cc_hwUnlockResource(), cc_getHwResGrant(),cc_getHwResReqFlag(), cc_startCore(), cc_stopCore(), cc_configCoreOutstanging()
*/
typedef enum {
	CC_CORE_CA53_CORE1 = 0x0,   ///< 1st Core CA53 core1
	CC_CORE_CA53_CORE2,         ///< 2nd Core CA53 core2

	CC_CORE_CA53_CORE_NUM,
	CC_CORE_DSP = 0x2,          ///< 1st Core DSP XM4_1
	CC_CORE_DSP2,               ///< 1st Core DSP XM4_2

	CC_CORE_DSP_NUM = 0x2,

	ENUM_DUMMY4WORD(CC_CORE_ID)

} CC_CORE_ID;

/**
    core communication event ID number

    @note For cc_waitCoreEntrySleep()
*/
typedef enum {
	CC_EVT_FROM_CPU1        = 0x00000001,           ///< Event receive from CPU1
	CC_EVT_FROM_CPU2        = 0x00000002,           ///< Event receive from CPU2
	CC_EVT_FROM_DSP         = 0x00000004,           ///< Event receive from DSP
	CC_EVT_FROM_DSP2        = 0x00000008,           ///< Event receive from DSP2

	CC_ACK_FROM_CPU1        = 0x00000010,           ///< CPU1 ack request
	CC_ACK_FROM_CPU2        = 0x00000020,           ///< CPU2 ack request
	CC_ACK_FROM_DSP         = 0x00000040,           ///< DSP ack request
	CC_ACK_FROM_DSP2        = 0x00000080,           ///< DSP ack request


	CC_CPU2_GOES_SLEEP      = 0x00000100,           ///< CPU2 entry sleep mode
	///< @note  call __asm__("wait")instruction
	CC_DSP_GOES_LIGHT_SLEEP = 0x00000200,           ///< DSP entry light sleep mode
	CC_DSP_GOES_STANDBY     = 0x00000400,           ///< DSP entry standbymode


	CC_EVT_ERR              = 0xFFFFFFFF,
	ENUM_DUMMY4WORD(CC_EVENT)

} CC_EVENT;

/*
    core request configuration 1-4
*/
typedef enum {
	CC_CORE_REQ_1 = 0x0,
	CC_CORE_REQ_2,
	CC_CORE_REQ_3,
	CC_CORE_REQ_4,

	CC_CORE_REQ_MAX,
	ENUM_DUMMY4WORD(CC_CORE_REQ_NUM)
} CC_CORE_REQ_NUM;


/**
    core communication configurated ID number

    @note For cc_setConfig()
*/
typedef enum {
	CC_CONFIG_ID_CORE2_BASE_ADDRESS = 1,            ///< Configured CPU exception base address
	///< @note  represent code start point running @ Core2
	///< Context is :
	///< - @b UINT32 : address of base address
	CC_CONFIG_ID_DSP_BASE_ADDRESS,                  ///< Configured CPU exception base address
	///< @note  represent code start point running @ DSP
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_CTRL_SRST,                         ///< CC controller will reset to default value except MIPS2 & DSP base address
	///< - @b NULL

	CC_CONFIG_ID_SET_MIPS1_REQ,                     ///< Set MIPS1 request slot number of each process cycle
	///< Context is :
	///< - @b CC_CORE_REQ_NUM : slot number of each process cycle(1-4)

	CC_CONFIG_ID_SET_MIPS2_REQ,                     ///< Set MIPS2 request slot number of each process cycle
	///< Context is :
	///< - @b CC_CORE_REQ_NUM : slot number of each process cycle(1-4)

	CC_CONFIG_ID_SET_DSP_EPP_REQ,                   ///< Set DSP EPP request slot number of each process cycle
	///< Context is :
	///< - @b CC_CORE_REQ_NUM : slot number of each process cycle(1-4)

	CC_CONFIG_ID_SET_DSP_EDP_REQ,                   ///< Set DSP EDP request slot number of each process cycle
	///< Context is :
	///< - @b CC_CORE_REQ_NUM : slot number of each process cycle(1-4)

	CC_CONFIG_ID_SET_DSP_PINT_BASE_ADDRESS,         ///< Set DSP Program_Int source data base address
	///< Context is :
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_SET_DSP_ISDM_BASE_ADDRESS,         ///< Set DSP Scaler memory source data base address
	///< Context is :
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_SET_DSP_IVDM_BASE_ADDRESS,         ///< Set DSP Vector memory source data base address
	///< Context is :
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_SET_DSP2_DSP_BASE1_ADDRESS,        ///< Set DSP DSP2 base1 address
	///< Context is :
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_SET_DSP2_DSP_BASE2_ADDRESS,        ///< Set DSP DSP2 base1 address
	///< Context is :
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_SET_DSP2_DSP_BASE3_ADDRESS,        ///< Set DSP DSP2 base1 address
	///< Context is :
	///< - @b UINT32 : address of base address

	CC_CONFIG_ID_SET_ACK_INTEN,                     ///< Set CoreX ack signal inten
	///< Context is :
	///< - @b UINT32 : point of PCC_ACK_INTEN




	ENUM_DUMMY4WORD(CC_CONFIG_ID)
} CC_CONFIG_ID;

//@}
// Core Communicator handler
typedef void (*CC_HANDLER)(UINT32 uiEvent);

typedef struct {
	UINT32      uiCMDID;        ///< Command ID, a serial number from 1 to 0xFFFF
	///< @note      For cc_getCoreXCMD() only, generated by CC driver.
	UINT32      uiCMDOperation; ///< Command operation, from 0 to 0xFFFF
	UINT32     *pData;          ///< Command data
	///< @note      Not the address of data that will be processed by this command
	UINT32      uiDataSize;     ///< Command data size in bytes
	///< @note      Not the size of data that will be processed by this command
} CC_CMD, *PCC_CMD;

typedef struct {
	UINT32      CoreID;         ///< core ID
	///< @note      For cc_getCoreXCMD() only, generated by CC driver.
	UINT32      uiCCEvt;        ///< CC event

	BOOL        bEn;            ///< enable or disable
	///< @note      Not the size of data that will be processed by this command
} CC_ACK_INTEN, *PCC_ACK_INTEN;

extern ER       cc_open(void);
extern ER       cc_close(void);

extern void     cc_registerCore2Handler(CC_HANDLER Hdl);
extern void     cc_registerCore3Handler(CC_HANDLER Hdl);
extern void     cc_registerCore4Handler(CC_HANDLER Hdl);

extern ER       cc_sendCore2CMD(PCC_CMD pCMD);
extern ER       cc_sendCore3CMD(PCC_CMD pCMD);
extern ER       cc_sendCore4CMD(PCC_CMD pCMD);

extern void     cc_getCore2CMD(PCC_CMD pCMD);
extern void     cc_getCore3CMD(PCC_CMD pCMD);
extern void     cc_getCore4CMD(PCC_CMD pCMD);

extern void     cc_ackCore2CMD(void);
extern void     cc_ackCore3CMD(void);
extern void     cc_ackCore4CMD(void);

extern void     cc_setCore2Base(UINT32 uiBaseAddr);

extern void     cc_setCore3BootVector(UINT32 uiBVAddr);

extern UINT32   cc_getCore2Base(void);
extern UINT32   cc_getCore3BootVector(void);
extern void     cc_stopCore(CC_CORE_ID uiCoreID);
extern ER       cc_startCore(CC_CORE_ID uiCoreID);
extern void     cc_setConfig(CC_CONFIG_ID ConfigID, UINT32 uiConfig);


extern INT32    cc_getHwResGrant(CC_CORE_ID uiCoreID);
extern INT32    cc_getHwResReqFlag(CC_CORE_ID uiCoreID);
extern CC_EVENT cc_waitCoreEntrySleep(CC_EVENT uiEvt);
extern void     cc_configCoreOutstanging(CC_CORE_ID uiCoreID, BOOL bArbEn, BOOL bApbEn);


#endif
