#ifndef BT_SMART_SERVICE_H
#define BT_SMART_SERVICE_H

#include <stdio.h>
#include "Type.h"

#define CHAR_VALUE_LENGTH           20
#define START_PACKAGE_HEADER_SIZE   3
#define MAX_CMD_LENGTH              256
#define MAX_RESPONSE_LENGTH         20

#define BYTE_EQUAL(a, b) ((a & 0x0ff) == (b & 0x0ff))

typedef enum{
    RCV_IDLE = 0,
    RCV_START,
    RCV_CONTINUE,
    RCV_INVALID
}rcv_state_t;


typedef enum{
    INVALID_TYPE
}packaget_type_t;

// return 0 on success
typedef int (*incoming_data_handle_t)(UINT8 * data, int length);

// align to 1 byte
#pragma pack(1)

typedef struct{
    UINT8 SN;
    UINT8 CmdID;
    UINT8 length;
}start_package_header_t, *start_package_header_p;


// response to command
typedef struct{
    UINT8 mSN;
    UINT8 mlength;
    UINT8 mpayload[MAX_RESPONSE_LENGTH];
}cmd_resp_t, *cmd_resp_p;

#pragma pack()




typedef int (*incoming_command_handle_p)(UINT8 * cmd);
void register_incoming_command_handler(incoming_command_handle_p h);
int handle_incoming_data(UINT8 * data, int length);
void clean_buff(UINT8* data, int len);
int bt_BleSndNotify_data(UINT8 cmdID, UINT8* data, int length);

#endif
