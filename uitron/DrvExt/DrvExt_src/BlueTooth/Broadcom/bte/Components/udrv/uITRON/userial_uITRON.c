/*
 * $ Copyright Broadcom Corporation $
 */
/*******************************************************************************
 **  Name:       userial_bby.c
 **
 **  Description:
 **
 **  This file contains the universal driver wrapper for the BTE-QC serial
 **  drivers
 **
 **  Copyright (c) 2001-2004, WIDCOMM Inc., All Rights Reserved.
 **  WIDCOMM Bluetooth Core. Proprietary and confidential.
 *******************************************************************************/
#include "kernel.h"
#include <string.h>
#include "bt_target.h"
#include "gki.h"
#include "userial.h"
#include "wiced.h"
#include "hcidefs.h"
#include "btm_cfg.h"
#include "rtos_type.h"
#include "uart.h"
#include "utron_utils.h"
#include "SysKer.h"
#include "SwTimer.h"
#include "clock.h"

#define READ_THREAD_PRI  12

void configure_hostwake(int level);

/* Verify if WICED Platform API returns success.
 * Otherwise, returns the error code immediately.
 * Assert in DEBUG build.
 */
#define VERIFY_RETVAL( x ) \
do \
{ \
    wiced_result_t verify_result = (x); \
    if ( verify_result != WICED_SUCCESS ) \
    { \
        wiced_assert( "bus error", 0!=0 ); \
        return verify_result; \
    } \
} while( 0 )

/* Macro for checking of bus is initialised */
#define IS_BUS_INITIALISED( ) \
do \
{ \
    if ( bus_initialised == WICED_FALSE ) \
    { \
        wiced_assert( "bus uninitialised", 0!=0 ); \
        return WICED_ERROR; \
    } \
}while ( 0 )

/* Macro for checking if bus is ready */
#define BT_BUS_IS_READY( ) \
do \
{ \
    if ( bt_bus_is_ready( ) == WICED_FALSE ) \
    { \
        wiced_assert( "bus not ready", 0!=0 ); \
        return WICED_ERROR; \
    } \
}while ( 0 )

/* Macro for waiting until bus is ready */
#define BT_BUS_WAIT_UNTIL_READY( ) \
do \
{ \
    while ( bt_bus_is_ready( ) == WICED_FALSE ) \
    { \
        wiced_rtos_delay_milliseconds( 10 ); \
    } \
} while ( 0 )

/* TODO: bring in bt_bus code to remove BTE dependency on wiced bluetooth library */
extern wiced_result_t bt_bus_init( void );
extern wiced_result_t bt_bus_deinit( void );
extern wiced_result_t bt_bus_transmit( const uint8_t* data_out, uint32_t size );
extern wiced_result_t bt_bus_receive( uint8_t* data_in, uint32_t size, uint32_t timeout_ms );
extern wiced_result_t bt_bus_uart_reset( void );
extern wiced_result_t bt_bus_uart_reconifig_baud(uint32_t baud);
extern wiced_bool_t bt_bus_is_ready( void );
extern void utron_setup_gpio(void);
#if HCILP_INCLUDED
void start_idle_timer(void);
//static void stop_idle_timer(void);
static void set_rts_enable(int enable);
#endif
static void novatek_uart_init(UART_BAUDRATE baudrate);
void  enable_lpm_interrupt(void);
void  change_host_baudrate(UART_BAUDRATE v);

static UINT32 userial_baud_tbl[] = {
    300, /* USERIAL_BAUD_300          0 */
    600, /* USERIAL_BAUD_600          1 */
    1200, /* USERIAL_BAUD_1200         2 */
    2400, /* USERIAL_BAUD_2400         3 */
    9600, /* USERIAL_BAUD_9600         4 */
    19200, /* USERIAL_BAUD_19200        5 */
    57600, /* USERIAL_BAUD_57600        6 */
    115200, /* USERIAL_BAUD_115200       7 */
    230400, /* USERIAL_BAUD_230400       8 */
    460800, /* USERIAL_BAUD_460800       9 */
    921600, /* USERIAL_BAUD_921600       10 */
    1000000, /* USERIAL_BAUD_1M           11 */
    1500000, /* USERIAL_BAUD_1_5M         12 */
    2000000, /* USERIAL_BAUD_2M           13 */
    3000000, /* USERIAL_BAUD_3M           14 */
    4000000 /* USERIAL_BAUD_4M           15 */
};

#define UART0_RX_FIFO_SIZE 256
#define UART0_TX_FIFO_SIZE 256

uint8_t g_RxFifoBuffer[UART0_RX_FIFO_SIZE];
uint8_t g_TxFifoBuffer[UART0_TX_FIFO_SIZE];


ID _SECTION(".kercfg_data") read_thread_id = 7;
#define READ_THREAD_STACK_SIZE      0x1000
#define READ_THREAD_NAME            ((INT8 *) "READ_THREAD")
//#define READ_THREAD_PRI             5
//#define READ_THREAD_PRI             8   /* test: lower than player task */
// #define READ_LIMIT               (GKI_BUF3_SIZE-BT_HDR_SIZE)
// #define HCISU_EVT                   EVENT_MASK(APPL_EVENT_0)

#define HCI_TYPE_COMMAND            1
#define HCI_TYPE_ACL_DATA           2
#define HCI_TYPE_SCO_DATA           3
#define HCI_TYPE_EVENT              4

BUFFER_Q Userial_in_q;
static BT_HDR *pbuf_USERIAL_Read = NULL;

void userial_read_thread( void ) _SECTION(".kercfg_text");
UINT8 _SECTION(".kercfg_data") g_readThreadAlive = 1;

/* USERIAL control block */
typedef struct
{
    tUSERIAL_CBACK *p_cback;
} tUSERIAL_CB;
tUSERIAL_CB userial_cb;



/******************************************************
 *                   Enumerations
 ******************************************************/

/* HCI Transport Layer Packet Type */
typedef enum
{
    HCI_UNINITIALIZED = 0x00, // Uninitialized
    HCI_COMMAND_PACKET = 0x01, // HCI Command packet from Host to Controller
    HCI_ACL_DATA_PACKET = 0x02, // Bidirectional Asynchronous Connection-Less Link (ACL) data packet
    HCI_SCO_DATA_PACKET = 0x03, // Bidirectional Synchronous Connection-Oriented (SCO) link data packet
    HCI_EVENT_PACKET = 0x04, // Events
    HCI_LOOPBACK_MODE = 0xFF,
// Loopback mode
// HCI Event packet from Controller to Host
} hci_packet_type_t;

#pragma pack(1)

typedef struct
{
    hci_packet_type_t packet_type; /* This is transport layer packet type. Not transmitted if transport bus is USB */
    uint8_t event_code;
    uint8_t content_length;
} hci_event_header_t;

typedef struct
{
    hci_packet_type_t packet_type; /* This is transport layer packet type. Not transmitted if transport bus is USB */
    uint16_t hci_handle;
    uint16_t content_length;
} hci_acl_packet_header_t;

#pragma pack()
/*****************************************************************************
 * Platform UART interface, taken from
 * ../Library/bluetooth/internal/bus/UART/bt_bus.c
 * (audio/2.4.x-bluetooth branch)
 *****************************************************************************/
//#include "wiced_bt_platform.h"

#ifndef USERIAL_RX_FIFO_SIZE
#define USERIAL_RX_FIFO_SIZE (3000)
#endif

static volatile wiced_bool_t bus_initialised = WICED_FALSE;
static volatile wiced_bool_t device_powered  = WICED_FALSE;

/* RX ring buffer. Bluetooth chip UART receive can be asynchronous, therefore a ring buffer is required */
//static volatile wiced_ring_buffer_t rx_ring_buffer;
//static volatile uint8_t             rx_data[USERIAL_RX_FIFO_SIZE];

wiced_result_t bt_bus_init( void )
{
    if ( bus_initialised == WICED_FALSE )
    {
        utron_setup_gpio();
		
		DRV_TRACE_DEBUG0("bt delay 500ms between gpio setup and uart init\r\n");
		Delay_DelayMs(500);
		
        novatek_uart_init(UART_BAUDRATE_115200);
        bus_initialised = WICED_TRUE;
    }

    return WICED_SUCCESS;
}

wiced_result_t bt_bus_deinit( void )
{
    if ( bus_initialised == WICED_TRUE )
    {
        uart3_close();
        return WICED_SUCCESS;
    }
    else
    {
        return WICED_ERROR;
    }
}

wiced_result_t bt_bus_transmit( const uint8_t* data_out, uint32_t size )
{
    uint32_t c = uart3_write((uint8_t*)data_out, size);

    #if 0
    DRV_TRACE_DEBUG2("bt_bus_transmit c=0x%x, size=%d\r\n", c,size);
    #endif

    return (c == size ) ? WICED_SUCCESS : WICED_ERROR;
}

wiced_result_t bt_bus_receive( uint8_t* data_in, uint32_t size, uint32_t timeout_ms )
{
    int32_t c = uart3_read(data_in, size);

    #if 0
    if (size==1)
    {
       DRV_TRACE_DEBUG2("bt_bus_receive size=%d, data=0x%x\r\n", c,*data_in);
    }
    else if (size==2)
    {
       DRV_TRACE_DEBUG3("bt_bus_receive size=%d, data=0x%x,0x%x\r\n", c,*data_in,*(data_in+1));
    }
    else if (size==4)
    {
       DRV_TRACE_DEBUG5("bt_bus_receive size=%d, data=0x%x,0x%x,0x%x,0x%x\r\n", c,*data_in,*(data_in+1),*(data_in+2),*(data_in+3));
    }
    #endif

    return (c >= 0) ? WICED_SUCCESS: WICED_ERROR;
}

wiced_bool_t bt_bus_is_ready( void )
{
    return WICED_TRUE;
}

static void platform_USART_Config(uint32_t baud)
{
   uart_setConfig(UART_CONFIG_ID_BAUDRATE, baud);
}

wiced_result_t bt_bus_uart_reconifig_baud(uint32_t baud)
{
   if ( bus_initialised == WICED_TRUE )
    {
        platform_USART_Config(baud);

        return WICED_SUCCESS;
    }
    else
    {
        return WICED_ERROR;
    }
}
static void novatek_uart_init(UART_BAUDRATE baudrate)
{

    UART_INIT_PARA UartParaBasic;

    uart3_open();
    /*== Initialize Uart configurations ==*/
    UartParaBasic.BaudRate = baudrate; // BaudRate 115200 bps
    UartParaBasic.Length = UART_LEN_L8_S1; // Character 8bits ; Stop 1 bit
    UartParaBasic.Parity = UART_PARITY_NONE; // No Parity Bit
    UartParaBasic.FlowCtrlMode = UART_FC_MODE_HW_ON; // with Flow Control function
    UartParaBasic.DataTransMode = UART_DATA_TRANS_PIO;// Use PIO Transfer Mode
    uart3_initHW(&UartParaBasic);
    /*== Start UART Tx/Rx ==*/

}

/*******************************************************************************
 **
 ** Function           USERIAL_GetLineSpeed
 **
 ** Description        This function convert USERIAL baud to line speed.
 **
 ** Output Parameter   None
 **
 ** Returns            line speed
 **
 *******************************************************************************/
UINT32 USERIAL_GetLineSpeed( UINT8 baud )
{
    if ( baud <= USERIAL_BAUD_4M )
        return ( userial_baud_tbl[baud - USERIAL_BAUD_300] );
    else
        return 0;
}

/*******************************************************************************
 **
 ** Function           USERIAL_GetBaud
 **
 ** Description        This function convert line speed to USERIAL baud.
 **
 ** Output Parameter   None
 **
 ** Returns            line speed
 **
 *******************************************************************************/
UDRV_API
extern UINT8 USERIAL_GetBaud( UINT32 line_speed )
{
    UINT8 i;
    for ( i = USERIAL_BAUD_300; i <= USERIAL_BAUD_4M; i++ )
    {
        if ( userial_baud_tbl[i - USERIAL_BAUD_300] == line_speed )
            return i;
    }

    return USERIAL_BAUD_AUTO;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Init
 **
 ** Description        This function initializes the  serial driver.
 **
 ** Output Parameter   None
 **
 ** Returns            Nothing
 **
 *******************************************************************************/

UDRV_API
void USERIAL_Init( void *p_cfg )
{
    DRV_TRACE_DEBUG0("USERIAL_Init\r\n");

    memset(&userial_cb, 0, sizeof(userial_cb));
    GKI_init_q( &Userial_in_q );

    return;
}

//char read_thread_stack[1024];
static void USERIAL_Open_ReadThread( void )
{
    sta_tsk(read_thread_id, 0);
    return;
}

void USERIAL_ReadThread_create( void )
{
    g_readThreadAlive = 1;
    OS_CONFIG_TASK(read_thread_id, READ_THREAD_PRI, 1024*2, userial_read_thread);
    //debug_trace("create userial read thread, read_thread_id %d\r\n", read_thread_id);
}

static void USERIAL_Close_ReadThread( void )
{
    // Close our read thread
    g_readThreadAlive = 0;
    ter_tsk( read_thread_id );

    // Flush the queue
    do
    {
        pbuf_USERIAL_Read = (BT_HDR *) GKI_dequeue( &Userial_in_q );
        if ( pbuf_USERIAL_Read )
        {
            GKI_freebuf( pbuf_USERIAL_Read );
        }

    } while ( pbuf_USERIAL_Read );

}

/*******************************************************************************
 **
 ** Function           USERIAL_Open
 **
 ** Description        Open the indicated serial port with the given configuration
 **
 ** Output Parameter   None
 **
 ** Returns            Nothing
 **
 *******************************************************************************/

UDRV_API
void USERIAL_Open( tUSERIAL_PORT port, tUSERIAL_OPEN_CFG *p_cfg, tUSERIAL_CBACK *p_cback )
{
    wiced_result_t result;

    DRV_TRACE_DEBUG0("USERIAL_Open\r\n");

    result = bt_bus_init( );
    if ( result != WICED_SUCCESS )
    {
        DRV_TRACE_DEBUG0("USERIAL_Open failed\r\n");
        return;
    }

    //novatek_uart_init(UART_BAUDRATE_115200);


    USERIAL_Open_ReadThread( );

    userial_cb.p_cback = p_cback;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Read
 **
 ** Description        Read data from a serial port using byte buffers.
 **
 ** Output Parameter   None
 **
 ** Returns            Number of bytes actually read from the serial port and
 **                    copied into p_data.  This may be less than len.
 **
 *******************************************************************************/

UDRV_API
UINT16 USERIAL_Read( tUSERIAL_PORT port, UINT8 *p_data, UINT16 len )
{
    UINT16 total_len = 0;
    UINT16 copy_len = 0;
    UINT8 * current_packet = NULL;

    do
    {
        if ( pbuf_USERIAL_Read != NULL )
        {
            current_packet = ( (UINT8 *) ( pbuf_USERIAL_Read + 1 ) ) + ( pbuf_USERIAL_Read->offset );

            if ( ( pbuf_USERIAL_Read->len ) <= ( len - total_len ) )
                copy_len = pbuf_USERIAL_Read->len;
            else
                copy_len = ( len - total_len );

            memcpy( ( p_data + total_len ), current_packet, copy_len );

            total_len += copy_len;

            pbuf_USERIAL_Read->offset += copy_len;
            pbuf_USERIAL_Read->len -= copy_len;

            if ( pbuf_USERIAL_Read->len == 0 )
            {
                GKI_freebuf( pbuf_USERIAL_Read );
                pbuf_USERIAL_Read = NULL;
            }
        }

        if ( pbuf_USERIAL_Read == NULL )
        {
            pbuf_USERIAL_Read = (BT_HDR *) GKI_dequeue( &Userial_in_q );
            //DRV_TRACE_DEBUG1("USERIAL_Read dequeue %08x", pbuf_USERIAL_Read);
        }
    } while ( ( pbuf_USERIAL_Read != NULL ) && ( total_len < len ) );

//    DRV_TRACE_DEBUG1("USERIAL_Read %i bytes", total_len);

    return total_len;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Readbuf
 **
 ** Description        Read data from a serial port using GKI buffers.
 **
 ** Output Parameter   Pointer to a GKI buffer which contains the data.
 **
 ** Returns            Nothing
 **
 ** Comments           The caller of this function is responsible for freeing the
 **                    GKI buffer when it is finished with the data.  If there is
 **                    no data to be read, the value of the returned pointer is
 **                    NULL.
 **
 *******************************************************************************/
UDRV_API
void USERIAL_ReadBuf( tUSERIAL_PORT port, BT_HDR **p_buf )
{

}

/*******************************************************************************
 **
 ** Function           USERIAL_WriteBuf
 **
 ** Description        Write data to a serial port using a GKI buffer.
 **
 ** Output Parameter   None
 **
 ** Returns            TRUE  if buffer accepted for write.
 **                    FALSE if there is already a buffer being processed.
 **
 ** Comments           The buffer will be freed by the serial driver.  Therefore,
 **                    the application calling this function must not free the
 **                    buffer.
 **
 *******************************************************************************/
UDRV_API
BOOLEAN USERIAL_WriteBuf( tUSERIAL_PORT port, BT_HDR *p_buf )
{
    return FALSE;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Write
 **
 ** Description        Write data to a serial port using a byte buffer.
 **
 ** Output Parameter   None
 **
 ** Returns            Number of bytes actually written to the serial port.  This
 **                    may be less than len.
 **
 *******************************************************************************/
UDRV_API
UINT16 USERIAL_Write( tUSERIAL_PORT port, UINT8 *p_data, UINT16 len )
{
    wiced_result_t result;

//    DRV_TRACE_DEBUG1("USERIAL_Write len=%d", len);
    result = bt_bus_transmit( p_data, len );
    if ( result != WICED_SUCCESS )
    {
        DRV_TRACE_DEBUG0("USERIAL_Write failed");
        return 0;
    }
//    DRV_TRACE_DEBUG0("USERIAL_Write success");
    return len;
}

void  change_host_baudrate(UART_BAUDRATE v)
{
    if(v)
        uart3_setConfig(UART_CONFIG_ID_BAUDRATE, v);
}
/*******************************************************************************
 **
 ** Function           USERIAL_Ioctl
 **
 ** Description        Perform an operation on a serial port.
 **
 ** Output Parameter   The p_data parameter is either an input or output depending
 **                    on the operation.
 **
 ** Returns            Nothing
 **
 *******************************************************************************/

UDRV_API
void USERIAL_Ioctl( tUSERIAL_PORT port, tUSERIAL_OP op, tUSERIAL_IOCTL_DATA *p_data )
{

    switch ( op )
    {
        case USERIAL_OP_FLUSH:
            break;
        case USERIAL_OP_FLUSH_RX:
            break;
        case USERIAL_OP_FLUSH_TX:
            break;
        case USERIAL_OP_BAUD_RD:
            break;
        case USERIAL_OP_BAUD_WR:
            change_host_baudrate(USERIAL_GetLineSpeed(p_data->baud));
            break;
        default:
            break;
    }

    return;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Close
 **
 ** Description        Close a serial port
 **
 ** Output Parameter   None
 **
 ** Returns            Nothing
 **
 *******************************************************************************/
UDRV_API
void USERIAL_Close( tUSERIAL_PORT port )
{
    wiced_result_t result;
    DRV_TRACE_DEBUG0("USERIAL_Close");

    USERIAL_Close_ReadThread( );

    result = bt_bus_deinit( );
    if ( result != WICED_SUCCESS )
    {
        DRV_TRACE_DEBUG0("USERIAL_Close failed");
    }

    DRV_TRACE_DEBUG0("USERIAL_Close--");
}

/*******************************************************************************
 **
 ** Function           USERIAL_Feature
 **
 ** Description        Check whether a feature of the serial API is supported.
 **
 ** Output Parameter   None
 **
 ** Returns            TRUE  if the feature is supported
 **                    FALSE if the feature is not supported
 **
 *******************************************************************************/
UDRV_API BOOLEAN USERIAL_Feature( tUSERIAL_FEATURE feature )
{
    return FALSE;
}

static wiced_result_t bt_hci_transport_driver_bus_read_handler( BT_HDR* packet )
{
    hci_packet_type_t packet_type = HCI_UNINITIALIZED;
    UINT8 *current_packet;

    if ( packet == NULL )
    {
        return WICED_ERROR;
    }

    packet->offset = 0;
    packet->layer_specific = 0;
    current_packet = (UINT8 *) ( packet + 1 );

    // Read 1 byte:
    //    packet_type
    // Use a timeout here so we can shutdown the thread
    if ( bt_bus_receive( (uint8_t*) &packet_type, 1, 0 ) != WICED_SUCCESS)
    {
        debug_trace("bt_bus error reading pkt_type last error ");

        return WICED_ERROR;
    }

    //DRV_TRACE_DEBUG1("read packet_type=0x%x\n", packet_type);

    switch ( packet_type )
    {
        case HCI_LOOPBACK_MODE:
            *current_packet++ = packet_type;

            // Read 1 byte:
            //    content_length
            if ( bt_bus_receive( current_packet, 1, 0 ) != WICED_SUCCESS )
            {
                return WICED_ERROR;
            }

            packet->len = *current_packet++;
            DRV_TRACE_DEBUG1("HCI_LOOPBACK_MODE packet->len=0x%x", packet->len);

            // Read payload
            if ( bt_bus_receive( current_packet, packet->len, 0 ) != WICED_SUCCESS )
            {
                DRV_TRACE_DEBUG0("bt_bus_receive FAIL in bt_hci_transport_driver_bus_read_handler");
                return WICED_ERROR;
            }

            packet->len = packet->len + 2; // +2 for packet type + read length

            break;

        case HCI_EVENT_PACKET:
        {
            hci_event_header_t header={0};

            // Read 2 bytes:
            //    event_code
            //    content_length
            if ( bt_bus_receive( (uint8_t*) &header.event_code, sizeof( header ) - sizeof( packet_type ), 0 ) != WICED_SUCCESS )
            {
                return WICED_ERROR;
            }

            header.packet_type = packet_type;
            *current_packet++ = packet_type;
            *current_packet++ = header.event_code;
            *current_packet++ = header.content_length;
            packet->len = header.content_length + 3; // +3 to include sizeof: packet_type=1 event_code=1 content_length=1

            // Read payload
            if ( bt_bus_receive( (uint8_t*) current_packet, (uint32_t) header.content_length, 0 ) != WICED_SUCCESS )
            {
                return WICED_ERROR;
            }
            //dump_array("payload - \n", current_packet, header.content_length);
            break;
        }

        case HCI_ACL_DATA_PACKET:
        {
            hci_acl_packet_header_t header={0};

            // Read 4 bytes:
            //    hci_handle (2 bytes)
            //    content_length (2 bytes)
            if ( bt_bus_receive( (uint8_t*) &header.hci_handle, HCI_DATA_PREAMBLE_SIZE, 0 ) != WICED_SUCCESS )
            {
                DRV_TRACE_ERROR0("bt_bus error acl header");
                return WICED_ERROR;
            }

            header.packet_type = packet_type;
            UINT8_TO_STREAM(current_packet, packet_type);
            UINT16_TO_STREAM(current_packet, header.hci_handle);
            UINT16_TO_STREAM(current_packet, header.content_length);

            packet->len = header.content_length + 5; // +5 to include sizeof: packet_type=1 hci_handle=2 content_length=2

#ifdef L2CAP_REASSEMBLE

            /* Check for segmented packets. If this is a continuation packet, then   */
            /* current rcv buffer will be freed, and we will continue appending data */
            /* to the original rcv buffer.                                           */
            if ((packet = l2cap_link_chk_pkt_start (packet)) == NULL)
            {
                /* If a NULL is returned, then we have a problem. Ignore remaining data in this packet */
                p_cb->rcv_len = msg_len;
                if (msg_len == 0)
                {
                    p_cb->rcv_state = HCISU_H4_MSGTYPE_ST;  /* Wait for next message */
                }
                else
                {
                    HCI_TRACE_ERROR0("HCIS: Ignoring....");

                    p_cb->rcv_state = HCISU_H4_IGNORE_ST;   /* Ignore rest of the packet */
                }

                break;
            }
#endif

            if (header.content_length > (HCI_ACL_POOL_BUF_SIZE - BT_HDR_SIZE))
            {
                DRV_TRACE_ERROR1("bt_bus error invalid acl len %i", header.content_length);
                return WICED_ERROR;
            }
            // Read payload
            if ( bt_bus_receive( (uint8_t*) current_packet, (uint32_t) header.content_length, 0 ) != WICED_SUCCESS )
            {
                return WICED_ERROR;
            }

            break;
        }

        case HCI_COMMAND_PACKET: /* Fall-through */
        default:
            return WICED_ERROR;
    }

    return WICED_SUCCESS;
}

/*******************************************************************************
 **
 ** Function           userial_read_thread
 **
 ** Description        userial_read_thread
 **
 ** Output Parameter   None
 **
 ** Returns            None
 **
 *******************************************************************************/
void userial_read_thread( )
{
    BT_HDR *p_buf;
    wiced_result_t ret;

    while ( g_readThreadAlive )
    {
        //DRV_TRACE_DEBUG0("userial_read_thread wait data...\n");

        // FIXME:TODO: Decide on the correct buffer size
        if ( ( p_buf = (BT_HDR *) GKI_getbuf( HCI_ACL_POOL_BUF_SIZE) ) != NULL )
        {
            ret = bt_hci_transport_driver_bus_read_handler( p_buf );
            if ( ret == WICED_ERROR )
            {
                GKI_freebuf( p_buf );
                continue;
            }

            //DRV_TRACE_DEBUG1("USERIAL_Read enqueue %08x", p_buf);

#if 0
            p = (p_buf+1);
            if (p[0] == HCI_ACL_DATA_PACKET)
            {
                /* Skip over packet type */
                p_buf->offset = 1;
                p_buf->len -= 1;
                p_buf->layer_specific = 0;
                p_buf->event = BT_EVT_TO_BTU_HCI_ACL;
                GKI_send_msg(BTU_TASK, 0, p_buf);
                continue;
            }
#endif
            GKI_enqueue( &Userial_in_q, p_buf );


            if (userial_cb.p_cback)
            {
                (userial_cb.p_cback)(0, USERIAL_RX_READY_EVT, NULL);
            }
        }
        else
        {
            GKI_delay(2000);
        }
    }

    DRV_TRACE_DEBUG0("Shutdown userial_read_thread")
    ext_tsk();
}

#if HCILP_INCLUDED

typedef struct {
    SWTIMER_ID timer_id;
    int        timer_is_valid;
}lpm_cb_t;

lpm_cb_t lmp_cb = {0, 0};


#define UART_IDLE_TIMEOUT_INTERVAL 5000

static void idle_timeout_handler( UINT32 uiEvent )
{
    //DRV_TRACE_DEBUG0("enter system sleep...\r\n");
    if(uiEvent == lmp_cb.timer_id)
    {
        set_rts_enable(0);
        //clk_powerdown(CLK_PDN_MODE_SLEEP1);
    }
}


static void init_idle_timer(void)
{
    if (SwTimer_Open(&lmp_cb.timer_id, idle_timeout_handler) == E_OK)
    {
        SwTimer_Cfg(lmp_cb.timer_id, UART_IDLE_TIMEOUT_INTERVAL,SWTIMER_MODE_ONE_SHOT);
        debug_msg("start uart idle timer id = %d\r\n", lmp_cb.timer_id);
    }
}

void start_idle_timer(void)
{
    if(lmp_cb.timer_is_valid)
    {
        SwTimer_Stop(lmp_cb.timer_id);
    }
    debug_msg("reset timer id [%d]\r\n", lmp_cb.timer_id);
    SwTimer_Start(lmp_cb.timer_id);
    lmp_cb.timer_is_valid = 1;
}

#if 0
static void stop_idle_timer(void)
{
    SwTimer_Stop(lmp_cb.timer_id);
    lmp_cb.timer_is_valid = 0;
}
#endif

void enable_lpm_interrupt(void)
{
    init_idle_timer();
    gpio_enableInt(GPIO_INT_08);
}

#if 0
static void uart3_lockRts(void)
{
    UINT32 uiReg;
    gpio_setDir(P_GPIO_33, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_33);
    uiReg = *(UINT32 *)(0xC0010000 + 0xAC);
    uiReg |= 0x00000002;
    *(UINT32 *)(0xC0010000 + 0xAC) = uiReg;
}

static void uart3_unlockRts(void)
{
    UINT32 uiReg;
    uiReg = *(UINT32 *)(0xC0010000 + 0xAC);
    uiReg &= ~(0x00000002);
    *(UINT32 *)(0xC0010000 + 0xAC) = uiReg;
    gpio_clearPin(P_GPIO_33);
}
#endif


static void set_rts_enable(int enable)
{
    #if 0
    static int rts_state = 1;
    if(rts_state == !!enable)
    {
        return;
    }

    enable ? uart3_unlockRts() : uart3_lockRts();
    rts_state = !!enable;
    #endif
}

#if 0
static void host_wake_isr(UINT32 uiEvent)
{
    debug_msg("host wake isr event %08x\r\n", uiEvent);
    if(gpio_getPin(GPIO_HOST_WAKE) == HCILP_HOST_WAKE_POLARITY)
    {
        stop_idle_timer();
        set_rts_enable(1);
    }else
    {
        start_idle_timer();
    }
}
#endif
#endif


#if 0
#define GPIO_BT_WAKE        P_GPIO_5
#define GPIO_HOST_WAKE      P_GPIO_3
#define GPIO_BT_REG         P_GPIO_40
#define GPIO_BT_POWER       P_GPIO_6

void test_btuart_hw(void)
{
    UART_INIT_PARA UartParaBasic;

    CHKPNT;
#if 0    
    //BT power
    gpio_setDir(GPIO_BT_POWER, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_BT_POWER);
    Delay_DelayMs(20);

    gpio_setDir(GPIO_BT_WAKE, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_BT_WAKE);
    Delay_DelayMs(20);

    gpio_setDir(GPIO_BT_REG, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_BT_REG);
    Delay_DelayMs(100);
    gpio_clearPin(GPIO_BT_REG);
    /*== Open Uart Driver ==*/
    uart3_open();

    /*== Initialize Uart configurations ==*/
    UartParaBasic.BaudRate = UART_BAUDRATE_115200; // BaudRate 115200 bps
    UartParaBasic.Length = UART_LEN_L8_S1; // Character 8bits ; Stop 1 bit
    UartParaBasic.Parity = UART_PARITY_NONE; // No Parity Bit
    //UartParaBasic.FlowCtrlMode = UART_FC_MODE_NONE; // No Flow Control function
    UartParaBasic.FlowCtrlMode = UART_FC_MODE_HW_ON; // with Flow Control function
    UartParaBasic.DataTransMode = UART_DATA_TRANS_PIO;// Use PIO Transfer Mode
    uart3_initHW(&UartParaBasic);
    /*== Start UART Tx/Rx ==*/
    
#else
    utron_setup_gpio();
    novatek_uart_init(UART_BAUDRATE_115200);
    GKI_delay(100);
#endif    
  

    uint8_t cmd_reset[] = {0x01, 0x03,0x0c, 0x00};
    uint8_t evt_reset[10] = {0x00};

    debug_trace("-> hci reset cmd\r\n");
    UINT32 r = uart3_write(cmd_reset, sizeof(cmd_reset));
    debug_trace("status %d\r\n", r);

    GKI_delay(1000);

    debug_trace("<- hci reset evt\r\n");
    //r = uart3_read(evt_reset, 7);
    r = uart3_read(evt_reset, 1);
    r = uart3_read(evt_reset + 1, 2);
    r = uart3_read(evt_reset + 3, 4);
    debug_trace("status %d\r\n", r);

    debug_dump_hex(evt_reset, sizeof(evt_reset));
    debug_trace("uart hw is ok\r\n");

    uart3_close();

}
#endif

#define BTRF_TEST
#ifdef BTRF_TEST
static unsigned char signal_test_cmds[4][10] = {

        {4, 0x01, 0x03, 0x0c, 0x00},
        {7, 0x01, 0x05, 0x0C, 0x03, 0x02, 0x00, 0x02},
        {5, 0x01, 0x1A, 0x0C, 0x01, 0x03},
        {4, 0x01, 0x03, 0x18, 0x00}
};

static void send_hci_one_cmd(unsigned char * cmd, int length, int delay)
{    
    unsigned char evt_reset[10];
  
    memset(evt_reset, 0, sizeof(evt_reset));
    debug_trace("->hci commond");    
    debug_dump_hex(cmd + 1, length);
    
    UINT32 r = uart3_write(cmd + 1, length); 
     debug_trace("status %d\r\n", r);
    
    GKI_delay(delay);   
    r = uart3_read(evt_reset, 1);       debug_trace("read status %d\r\n", r);
    r = uart3_read(evt_reset + 1, 2);   debug_trace("read status %d\r\n", r);
    length = 0x0ff & evt_reset[2];
    r = uart3_read(evt_reset + 3, length);    
    
    debug_trace("<-HCI event %d", r);
    debug_dump_hex(evt_reset, length + 3);
}
void download_hcd(void);
void signal_test(void);
void signal_test(void)
{
  int i;
  unsigned char hci_reset[] = {4, 0x01, 0x03, 0x0c, 0x00};
  unsigned char hci_download_minidriver[] = { 4, 0x01, 0x2e, 0xfc, 0x00 };

  debug_trace("Bluetooth signal test begin");
  
  //bt_bus_init();  
  utron_setup_gpio();
  novatek_uart_init(UART_BAUDRATE_115200);
  GKI_delay(200);
  send_hci_one_cmd(hci_reset, hci_reset[0] & 0x0ff, 2000);
  send_hci_one_cmd(hci_download_minidriver, hci_download_minidriver[0] & 0x0ff, 200);
  
  GKI_delay(200);

  download_hcd();

  GKI_delay(1000);

  for(i = 0; i < 4; i++)
  {
    send_hci_one_cmd(signal_test_cmds[i], signal_test_cmds[i][0] & 0x0ff, 1000);
  }

  debug_trace("go ahead for testing...");
}

void get_one_command(unsigned char * out, int offset);
UINT32 get_hcd_content_length(void);

void download_hcd(void)
{
    UINT32 totallength = get_hcd_content_length();
    unsigned char buf[512];
    int cmdlen = 0;

    int offset = 0;
    do{
        get_one_command(buf+2, offset);
        cmdlen = (buf[4] & 0x00ff) + 3;
        offset += cmdlen;   // header(2 bytes) + length(1 byte)
        buf[0] = cmdlen + 1;  // +0x01, this is useless here, some command may have length 0xff already, overflow may ocurr
        buf[1] = 0x01;
        send_hci_one_cmd(buf, cmdlen + 1, 100);
    } while ((UINT32)offset < totallength - 1);
}

#endif
