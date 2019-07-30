/**
    HwCopy module driver

    @file       hwcopy.h
    @ingroup    mIDrvIPP_Hwcopy
    @note
\n              -hwcopy_open() to start this module.
\n              -hwcopy_close() to close this module.
\n
\n              This module will have to enable and wait the system interrupt.
\n
\n              -hwcopy_request. set parameters of images.
\n
\n
\n              Usage example:
\n
\n              (1)hwcopy_open(HWCOPY_ID_1);
\n              (2)Fill request;
\n                 hwcopy_request(HWCOPY_ID_1, &request);
\n                      :
\n              (3)hwcopy_close(HWCOPY_ID_1);

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef   _HWCOPY_H
#define   _HWCOPY_H

#include "Driver.h"

/**
    @addtogroup mIDrvIPP_Hwcopy
*/
//@{

/**
    HwCopy channel ID

    HwCopy channel ID
*/
typedef enum {
	HWCOPY_ID_1,                          ///< Hwcopy Controller
	ENUM_DUMMY4WORD(HWCOPY_ID)
} HWCOPY_ID;

/**
    Hwcopy configuration ID

    Hwcopy configuration ID
    @note For hwcopy_setConfig()
*/
typedef enum {
	HWCOPY_CONFIG_ID_FREQ,              ///< HwCopy operating frequency (unit: MHz). Context can be:
	///< - @b 240: 120 MHz
	///< - @b 380: 380 MHz
	///< - @b 420: 420 MHz
	///< - @b 480: 480 MHz

	ENUM_DUMMY4WORD(HWCOPY_CONFIG_ID)
} HWCOPY_CONFIG_ID;
#define HWCOPY_CONFIG_FREQUENCY           (HWCOPY_CONFIG_ID_FREQ)


/**
    HwCopy operation command

    Select which HwCopy operation command for HwCopy engine operation.

*/
typedef enum {
	HWCOPY_LINEAR_SET,                  ///< linear set constant to addressed memory
	HWCOPY_BLOCK_SET,                   ///< block set constant to addressed memory
	HWCOPY_LINEAR_COPY,                 ///< linear copy to addressed memory
	HWCOPY_BLOCK_COPY,                  ///< block copy to addressed memory


	ENUM_DUMMY4WORD(HWCOPY_CMD)
} HWCOPY_CMD;


/**
    HwCopy Memory identifier

    Select Memory A/B.

    @note For HWCOPY_MEM
*/
typedef enum {
	HWCOPY_MEM_ID_A,                          ///< Memory A
	HWCOPY_MEM_ID_B,                          ///< Memory B

	ENUM_DUMMY4WORD(HWCOPY_MEM_ID)
} HWCOPY_MEM_ID;


/**
    HwCopy Memory structure

    Description of HwCopy memory

    @note For HWCOPY_REQUEST
*/
typedef struct HWCOPY_MEM {
	HWCOPY_MEM_ID memID;            ///< Memory identifier
	UINT32 uiAddress;               ///< Memory DRAM address (unit: byte)
	UINT32 uiLineoffset;            ///< Memory lineoffset (unit: byte) (Must be 4 byte alignment)
	UINT32 uiWidth;                 ///< Memory width (unit: byte)
	UINT32 uiHeight;                ///< Memory height (unit: line)

	struct HWCOPY_MEM *pNext;         ///< Link to next Memory. (Fill NULL if this is last item)
} HWCOPY_MEM, *PHWCOPY_MEM;

/**
    HwCopy constant text description

    Description for each constant text

    @note For HWCOPY_REQUEST
*/
typedef struct HWCOPY_CTEX {
	UINT32 ctex;                    ///<  context
	UINT32 datalength;               ///< Data length of linear set and copy (unit: byte)

} HWCOPY_CTEX, *PHWCOPY_CTEX;


/**
    HwCopy request structure

    Description of hwcopy request

    @note For hwcopy_request()
*/
typedef struct {
	DRV_VER_INFO verInfo;           ///< Driver version
	PHWCOPY_MEM pMemoryDescript;       ///< Descriptions of Memories for command
	PHWCOPY_CTEX pCtexDescript;
	HWCOPY_CMD command;

} HWCOPY_REQUEST, *PHWCOPY_REQUEST;





// Functions exported from HwCopy driver
extern ER       hwcopy_open(HWCOPY_ID id);
extern BOOL     hwcopy_isOpened(HWCOPY_ID id);
extern ER       hwcopy_close(HWCOPY_ID id);

extern ER       hwcopy_setConfig(HWCOPY_ID hwcopyID, HWCOPY_CONFIG_ID configID, UINT32 configContext);
extern ER       hwcopy_getConfig(HWCOPY_ID hwcopyID, HWCOPY_CONFIG_ID configID, UINT32 *pConfigContext);

extern ER       hwcopy_request(HWCOPY_ID id, PHWCOPY_REQUEST pRequest);

//@}

#endif
