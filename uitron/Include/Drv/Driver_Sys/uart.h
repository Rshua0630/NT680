/**
    Header file for UART/UART2

    This file is the header file for UART/UART2 driver
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       uart.h
    @ingroup    mIDrvIO_UART
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/**
    @addtogroup mIDrvIO_UART
*/
//@{

#ifndef _UART_H
#define _UART_H

#include "Type.h"



/**
    UART/UART2 Controller General Configuration Selection

    This is used in uart_setConfig()/uart2_setConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	UART_CONFIG_ID_BAUDRATE,        ///< Set configuring of the UART BaudRate. Please use enumeration UART_BAUDRATE as input parameter.
	UART_CONFIG_ID_CHAR_LEN,        ///< Set configuring of the UART receiving format. Please use enumeration UART_LENGTH as input parameter.
	UART_CONFIG_ID_PARITY,          ///< Set configuring of the UART parity format. Please use enumeration UART_PARITY as input parameter.
	UART_CONFIG_ID_FLOWCTRL_MODE,   ///< Set configuring of the UART flow control. Please use enumeration UART_FC_MODE as input parameter.
	UART_CONFIG_ID_FLOWCTRL_CTS_PIN,///< If the flow control mode is set to UART_FC_MODE_SW_ON,  this is used to configure the software
	///< flow control CTS pin location. Please use *_GPIO_* as input parameter.
	UART_CONFIG_ID_FLOWCTRL_RTS_PIN,///< If the flow control mode is set to UART_FC_MODE_SW_ON,  this is used to configure the software
	///< flow control RTS pin location. Please use *_GPIO_* as input parameter.
	UART_CONFIG_ID_TRANSFER_MODE,   ///< Set configuring of the UART transfer mode. Please use enumeration UART_DATA_TRAN_MODE as input parameter.
	UART_CONFIG_ID_BREAK,           ///< Set TRUE to BREAK would force the uart outout low to make the bus suspend. Set FALSE would release the BREAK.
	UART_CONFIG_ID_AUTOPINMUX,      ///< This setting is used to notify the driver to auto switch ON/OFF the UART pinmux during uart open/close.

	UART_CONFIG_ID_ENDDET_CHAR_NO,  ///< Set UART Receive Auto End-Detection Characters Numbers.(Valid for PIO mode only)
	///< Valid settings is 0 ~ 4. Set this valud to 0 means disabling the end-detection(default).
	///< After Enabled, the API uart_read()/uart2_read() PIO receiving would auto detect end-character as return signal.

	UART_CONFIG_ID_ENDDET_MODE,     ///< Select UART_ENDDET_MODE_OR or UART_ENDDET_MODE_AND
	UART_CONFIG_ID_ENDDET_CHAR0,    ///< End Detection Character 0 value
	UART_CONFIG_ID_ENDDET_CHAR1,    ///< End Detection Character 1 value
	UART_CONFIG_ID_ENDDET_CHAR2,    ///< End Detection Character 2 value
	UART_CONFIG_ID_ENDDET_CHAR3,    ///< End Detection Character 3 value
	UART_CONFIG_ID_ENDDET_BACKSPACE_EN,///< Enable/Disable End Detection Character Processing BackSpace.

	UART_CONFIG_ID_PUTSTR_POLLING,  ///< Enable/Disable the UART putString force using polling or allow task switch.
	///< Default is ENABLE to force using polling.

	ENUM_DUMMY4WORD(UART_CONFIG_ID)
} UART_CONFIG_ID;

/**
    UART END Detect Mode

    This is used in uart_setConfig(UART_CONFIG_ID_ENDDET_MODE) / uart2_setConfig(UART_CONFIG_ID_ENDDET_MODE)
    as input selection.
*/
typedef enum {
	UART_ENDDET_MODE_OR,            ///< End-Detect mode is One of Character matching would be regarded as END detected.
	UART_ENDDET_MODE_AND,           ///< End-Detect mode is ALL of Characters sequential matching would be regarded as END detected.

	ENUM_DUMMY4WORD(UART_ENDDET_MODE)
} UART_ENDDET_MODE;

/**
    UART Baud Rate value

    This definition is used in uart_setConfig(UART_CONFIG_ID_BAUDRATE)/uart2_setConfig(UART_CONFIG_ID_BAUDRATE)
    to specify the uart bus transmitting/receiving speed.
*/
typedef enum {
	UART_BAUDRATE_2400    =    2400,///< Baud rate    2400 bits per second.
	UART_BAUDRATE_4800    =    4800,///< Baud rate    4800 bits per second.
	UART_BAUDRATE_9600    =    9600,///< Baud rate    9600 bits per second.
	UART_BAUDRATE_19200   =   19200,///< Baud rate   19200 bits per second.
	UART_BAUDRATE_38400   =   38400,///< Baud rate   38400 bits per second.
	UART_BAUDRATE_57600   =   57600,///< Baud rate   57600 bits per second.
	UART_BAUDRATE_115200  =  115200,///< Baud rate  115200 bits per second.
	UART_BAUDRATE_1500000 = 1500000,///< Baud rate 1500000 bits per second.
	UART_BAUDRATE_3000000 = 3000000,///< Baud rate 3000000 bits per second. Only Support for UART2

	ENUM_DUMMY4WORD(UART_BAUDRATE)
} UART_BAUDRATE;


/**
    UART Character Length and StopBit format

    This definition is used in uart_setConfig(UART_CONFIG_ID_CHAR_LEN)/uart2_setConfig(UART_CONFIG_ID_CHAR_LEN)
    to specify the uart bus transmitting/receiving character format.
*/
typedef enum {
	UART_LEN_L5_S1,         ///< Character length 5, stop bits 1
	UART_LEN_L6_S1,         ///< Character length 6, stop bits 1
	UART_LEN_L7_S1,         ///< Character length 7, stop bits 1
	UART_LEN_L8_S1,         ///< Character length 8, stop bits 1
	UART_LEN_L5_S1P5,       ///< Character length 5, stop bits 1.5
	UART_LEN_L6_S2,         ///< Character length 6, stop bits 2
	UART_LEN_L7_S2,         ///< Character length 7, stop bits 2
	UART_LEN_L8_S2,         ///< Character length 8, stop bits 2

	ENUM_DUMMY4WORD(UART_LENGTH)
} UART_LENGTH;

/**
    UART Character Parity Definition

    This definition is used in uart_setConfig(UART_CONFIG_ID_PARITY)/uart2_setConfig(UART_CONFIG_ID_PARITY)
    to specify the uart bus transmitting/receiving character parity.
*/
typedef enum {
	UART_PARITY_NONE = 0,   ///< No parity checking
	UART_PARITY_ODD  = 1,   ///< Odd parity
	UART_PARITY_EVEN = 3,   ///< Even parity
	UART_PARITY_ONE  = 5,   ///< One parity
	UART_PARITY_ZERO = 7,   ///< Zero parity

	ENUM_DUMMY4WORD(UART_PARITY)
} UART_PARITY;

/**
    UART Flow Control Mode

    This definition is used in uart_setConfig(UART_CONFIG_ID_FLOWCTRL_MODE)/uart2_setConfig(UART_CONFIG_ID_FLOWCTRL_MODE)
    to specify the uart flow control mode.
*/
typedef enum {
	UART_FC_MODE_NONE,      ///< UART doesn't support any flow control.
	UART_FC_MODE_HW_ON,     ///< UART support hardware flow control, and turn it on.
	UART_FC_MODE_SW_ON,     ///< UART support software flow control (GPIO simulation), and turn it on.

	ENUM_DUMMY4WORD(UART_FC_MODE)
} UART_FC_MODE;

/**
    UART data transfer mode.

    This definition is used in uart_setConfig(UART_CONFIG_ID_TRANSFER_MODE)/uart2_setConfig(UART_CONFIG_ID_TRANSFER_MODE)
    to specify the uart transmission in PIO/DMA mode. The DMA mode only support for UART2 only.
    The driver can judge to use PIO or DMA mode automatically if the user sets mode to UART_DATA_TRANS_PIODMA.
*/
typedef enum {
	UART_DATA_TRANS_PIO,    ///< Uart Force PIO mode transfer. Valid for UART and UART2.
	UART_DATA_TRANS_DMA,    ///< Uart Force DMA mode transfer. Valid for UART2 only.
	UART_DATA_TRANS_PIODMA, ///< Uart PIO/DMA Transfer Mode depends on driver usage. Valid for both UART/UART2.

	ENUM_DUMMY4WORD(UART_DATA_TRAN_MODE)
} UART_DATA_TRAN_MODE;

/**
    UART Data Buffer Information

    This structure is used in the api uart_operateData() / uart2_operateData() to specify the input/output buffer information.
*/
typedef struct {
	volatile UINT32 uiSize;     ///< Input/Output operation size, it must be larger than 0. Support for Byte alignment size.
	UINT8          *pBuffer;    ///< Buffer address pointer. Support for Byte alignment address.
	///< it is used to get data from RX-FIFO to buffer or
	///< put data from buffer to TX-FIFO.
} UART_BUFFER_INFO;

/**
    UART Software Flow Control Pin Location

    This is used in uart_initHW() to specify the software flow control pin location.
    This is valid only if the flow control set to UART_FC_MODE_SW_ON.
*/
typedef struct {
	UINT32      uiRts;          ///< RTS pin number. Please use *_GPIO_* as input parameter.
	UINT32      uiCts;          ///< CTS pin number. Please use *_GPIO_* as input parameter.
} UART_FC_PIN_NUM;

/**
    UART Initialization Parameters

    This is used in uart_initHW() to specify all the uart initialization parameters.
    This api would also reset the Tx/Rx FIFO.
*/
typedef struct {
	UART_BAUDRATE           BaudRate;       ///< Specify the UART operating BaudRate.
	UART_LENGTH             Length;         ///< Specify character length and stop bits.
	UART_PARITY             Parity;         ///< Specify the character Parity.
	UART_FC_MODE            FlowCtrlMode;   ///< Specify the Flow Control Mode.
	UART_FC_PIN_NUM         FcPinNum;       ///< If is SW Flow control, this is used to specify the pin location.
	UART_DATA_TRAN_MODE     DataTransMode;  ///< Specify Data transfer mode in UART data transmission and receiving.
} UART_INIT_PARA;

/**
    UART operating data action

    This definition is used in uart_waitData() / uart_stopData() to specify to wait/stop which of the uart behavior,
    or used in uart_operateData() tp specify the uart operation.
*/
typedef enum {
	UART_ACT_PUT = 0x1,                     ///< Put data to uart.(Transmit)
	UART_ACT_GET,                           ///< Gte data from uart.(Receive)
	ENUM_DUMMY4WORD(UART_ACTION)
} UART_ACTION;

/**
    UART operate data parameter

    This structure is used in uart_operateData() to specify the uart operation and the buffer information.
*/
typedef struct {
	UART_ACTION         Action;             ///< Get or Put. (Receive or Transmit)
	UART_BUFFER_INFO    BufferInfo;         ///< Buffer information.
} UART_DATA_PARA;


/**
    @name   UART Line status

    Uart Line status identification for uart_checkIntStatus().
*/
//@{
#define UART_INT_STATUS_DATA_READY          0x00000001      ///< Received data ready
#define UART_INT_STATUS_OVERRUN_ERROR       0x00000002      ///< Overrun
#define UART_INT_STATUS_PARITY_ERROR        0x00000004      ///< Parity error
#define UART_INT_STATUS_FRAMING_ERROR       0x00000008      ///< Do not have valid stop bit
#define UART_INT_STATUS_BREAK_INTERRUPT     0x00000010      ///< Receiver's line input RX held at zero for a character time
#define UART_INT_STATUS_THR_EMPTY           0x00000020      ///< Non-FIFO: THR is empty, FIFO: FIFO is empty
#define UART_INT_STATUS_TX_EMPTY            0x00000040      ///< Both THR (or FIFO) and TSR are empty
#define UART_INT_STATUS_RX_FIFO_ERROR       0x00000080      ///< RX FIFO error
//@}


#define UART_ESC                            27
#define UART_ENTER                          13


//
//  Extern UART APIs
//

extern ER       uart_open(void);
extern ER       uart_close(void);
extern BOOL     uart_isOpened(void);
extern ER       uart_putChar(CHAR cData);
extern ER       uart_getChar(CHAR *pcData);
extern ER       uart_putString(CHAR *pString);
extern ER       uart_getString(CHAR *pcString, UINT32 *pcBufferLen);
extern void     uart_setConfig(UART_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   uart_write(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart_read(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart_stopData(UART_ACTION Action);
extern void     uart_waitData(UART_ACTION Action);
extern ER       uart_chkChar(CHAR *pcData);
extern void     uart_abortGetChar(void);

extern ER       uart_initHW(UART_INIT_PARA  *pUartInitPara);
extern ER       uart_operateData(UART_DATA_PARA *pDataPara);
extern void     uart_putHex(UINT8 inData);
extern void     uart_putReg(UINT32 inReg);
extern UINT32   uart_checkIntStatus(void);

_INLINE void uart_init(UART_BAUDRATE BaudRate, UART_LENGTH Length, UART_PARITY Parity)
{
	uart_setConfig(UART_CONFIG_ID_BAUDRATE,         BaudRate);
	uart_setConfig(UART_CONFIG_ID_CHAR_LEN,         Length);
	uart_setConfig(UART_CONFIG_ID_PARITY,           Parity);
	uart_setConfig(UART_CONFIG_ID_FLOWCTRL_MODE,    UART_FC_MODE_NONE);
	uart_setConfig(UART_CONFIG_ID_TRANSFER_MODE,    UART_DATA_TRANS_PIO);
}



//
//  Extern UART2 APIs
//

extern ER       uart2_open(void);
extern ER       uart2_close(void);
extern BOOL     uart2_isOpened(void);
extern ER       uart2_putChar(CHAR cData);
extern ER       uart2_getChar(CHAR *pcData);
extern ER       uart2_putString(CHAR *pString);
extern ER       uart2_getString(CHAR *pcString, UINT32 *pcBufferLen);
extern void     uart2_setConfig(UART_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   uart2_write(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart2_read(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart2_stopData(UART_ACTION Action);
extern void     uart2_waitData(UART_ACTION Action);
extern ER       uart2_chkChar(CHAR *pcData);
extern void     uart2_abortGetChar(void);

extern ER       uart2_initHW(UART_INIT_PARA  *pUartInitPara);
extern ER       uart2_operateData(UART_DATA_PARA *pDataPara);
extern void     uart2_putHex(UINT8 inData);
extern void     uart2_putReg(UINT32 inReg);
extern UINT32   uart2_checkIntStatus(void);

_INLINE void uart2_init(UART_BAUDRATE BaudRate, UART_LENGTH Length, UART_PARITY Parity)
{
	uart2_setConfig(UART_CONFIG_ID_BAUDRATE,        BaudRate);
	uart2_setConfig(UART_CONFIG_ID_CHAR_LEN,        Length);
	uart2_setConfig(UART_CONFIG_ID_PARITY,          Parity);
	uart2_setConfig(UART_CONFIG_ID_FLOWCTRL_MODE,   UART_FC_MODE_NONE);
	uart2_setConfig(UART_CONFIG_ID_TRANSFER_MODE,   UART_DATA_TRANS_PIO);
}

//
//  Extern UART3 APIs
//

extern ER       uart3_open(void);
extern ER       uart3_close(void);
extern BOOL     uart3_isOpened(void);
extern ER       uart3_putChar(CHAR cData);
extern ER       uart3_getChar(CHAR *pcData);
extern ER       uart3_putString(CHAR *pString);
extern ER       uart3_getString(CHAR *pcString, UINT32 *pcBufferLen);
extern void     uart3_setConfig(UART_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   uart3_write(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart3_read(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart3_stopData(UART_ACTION Action);
extern void     uart3_waitData(UART_ACTION Action);
extern ER       uart3_chkChar(CHAR *pcData);
extern void     uart3_abortGetChar(void);

extern ER       uart3_initHW(UART_INIT_PARA  *pUartInitPara);
extern ER       uart3_operateData(UART_DATA_PARA *pDataPara);
extern void     uart3_putHex(UINT8 inData);
extern void     uart3_putReg(UINT32 inReg);
extern UINT32   uart3_checkIntStatus(void);

_INLINE void uart3_init(UART_BAUDRATE BaudRate, UART_LENGTH Length, UART_PARITY Parity)
{
	uart3_setConfig(UART_CONFIG_ID_BAUDRATE,        BaudRate);
	uart3_setConfig(UART_CONFIG_ID_CHAR_LEN,        Length);
	uart3_setConfig(UART_CONFIG_ID_PARITY,          Parity);
	uart3_setConfig(UART_CONFIG_ID_FLOWCTRL_MODE,   UART_FC_MODE_NONE);
	uart3_setConfig(UART_CONFIG_ID_TRANSFER_MODE,   UART_DATA_TRANS_PIO);
}

//
//  Extern UART4 APIs
//

extern ER       uart4_open(void);
extern ER       uart4_close(void);
extern BOOL     uart4_isOpened(void);
extern ER       uart4_putChar(CHAR cData);
extern ER       uart4_getChar(CHAR *pcData);
extern ER       uart4_putString(CHAR *pString);
extern ER       uart4_getString(CHAR *pcString, UINT32 *pcBufferLen);
extern void     uart4_setConfig(UART_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   uart4_write(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart4_read(UINT8 *pBuffer, UINT32 Length);
extern UINT32   uart4_stopData(UART_ACTION Action);
extern void     uart4_waitData(UART_ACTION Action);
extern ER       uart4_chkChar(CHAR *pcData);
extern void     uart4_abortGetChar(void);

extern ER       uart4_initHW(UART_INIT_PARA  *pUartInitPara);
extern ER       uart4_operateData(UART_DATA_PARA *pDataPara);
extern void     uart4_putHex(UINT8 inData);
extern void     uart4_putReg(UINT32 inReg);
extern UINT32   uart4_checkIntStatus(void);

_INLINE void uart4_init(UART_BAUDRATE BaudRate, UART_LENGTH Length, UART_PARITY Parity)
{
	uart4_setConfig(UART_CONFIG_ID_BAUDRATE,        BaudRate);
	uart4_setConfig(UART_CONFIG_ID_CHAR_LEN,        Length);
	uart4_setConfig(UART_CONFIG_ID_PARITY,          Parity);
	uart4_setConfig(UART_CONFIG_ID_FLOWCTRL_MODE,   UART_FC_MODE_NONE);
	uart4_setConfig(UART_CONFIG_ID_TRANSFER_MODE,   UART_DATA_TRANS_PIO);
}


//@}

#endif

