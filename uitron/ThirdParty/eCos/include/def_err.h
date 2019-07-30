/***************************************************************************
	(C)Copyright 2013, NOVATEK MICROELECTRONICS CORP.
 
  This document is the CONFIDENTIAL property of Novatek Microelectronics Corp.
	Sources	 : def_err.h
	Customer : Novatek STB
***************************************************************************/
/*
 * Brief: Error code definition.
 */
#ifndef __DEF_ERROR
#define __DEF_ERROR
#define ERROR_BASE				0
#define ERROR_BASE_COMMON		(( 0<<8)+ERROR_BASE )
#define ERROR_BASE_OS			(( 1<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_AUDIO	(( 2<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_CARD		(( 3<<8)+ERROR_BASE )  /* card reader */
#define ERROR_BASE_DRV_CI		(( 4<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_LAYER	(( 5<<8)+ERROR_BASE )  /* layer */
#define ERROR_BASE_DRV_DMA		(( 6<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_E2P		(( 7<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_ETH		(( 8<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_GPIO		((9<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_2WS		((10<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_IR		((11<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_IRQ		((12<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_KEYPAD	((13<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_NAND		((14<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_NIM		((15<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_BLIT		((16<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_PWM		((17<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_SMC		((18<<8)+ERROR_BASE )  /* smart card */
#define ERROR_BASE_DRV_TSPU		((19<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_TVE		((20<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_UART		((21<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_USB		((22<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_VDEC 	((23<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_SYS		((24<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_FLASH	((25<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_VMIX		((26<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_EVENT	((27<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_DBG		((28<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_STC		((29<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_VTG		((30<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_VEP  	((31<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_HDMI		((32<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_FCR		((33<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_CEC		((34<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_STBC		((35<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_ADC		((36<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_RO		((37<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_CRYPTO		((38<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_RAMCTRL		((39<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_CAPROC		((40<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_IPC		((41<<8)+ERROR_BASE )
#define ERROR_BASE_DRV_RTC		((42<<8)+ERROR_BASE )

enum
{
	ERROR_NO_ERROR=ERROR_BASE_COMMON,		/*!< no error */
	ERROR_NOT_SUPPORTED,	/*!< The function is not supported. */
	ERROR_INVALID_HANDLE, 	/*!< Handle is not valid. */
	ERROR_TIMEOUT, 			/*!< Timeout */
	ERROR_HARDWARE_FAIL,	/*!< The hardware is fail. */
	ERROR_BAD_PARAMETER,	/*!< Bad parameter passed */
	ERROR_INTERRUPT,		/*!< Interrupt install or uninstall error */
	ERROR_NO_FREE_HANDLE,	/*!< No free handle for the device */
	ERROR_NO_MEMORY,		/*!< The memory allocate is fail */
	ERROR_APU_NOT_READY,	/*!< Proc2 not start yet */
	ERROR_INITIALISED,		/*!< The driver is not initialized or already initialized. */
	ERROR_NO_INDEX,
};

typedef unsigned long ErrorCode;

#endif
