/*
    Crypto driver header file

    Crypto driver header file.

    @file       crypto.h
    @ingroup    mIDrvMisc_Crypto
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _CRYPTO_H
#define _CRYPTO_H

#include "Type.h"

/**
    @addtogroup mIDrvMisc_Crypto
*/
//@{

/**
    Crypto engine crypto mode
*/
typedef enum {
	CRYPTO_DES = 0x00,                  ///< Select Crypto mode DES
	CRYPTO_3DES,                        ///< Select Crypto mode 3DES
	CRYPTO_AES,                         ///< Select Crypto mode AES
	CRYPTO_AES256,                      ///< Select Crypto mode AES-256
	CRYPTO_MODE_NUM,
	ENUM_DUMMY4WORD(CRYPTO_MODE)
} CRYPTO_MODE;


/**
    Crypto engine crypto Operating mode
*/
typedef enum {
	CRYPTO_EBC = 0x00,                  ///< Select Crypto opmode EBC
	CRYPTO_CBC,                         ///< Select Crypto opmode CBC
	CRYPTO_CFB,                         ///< Select Crypto opmode CFB
	CRYPTO_OFB,                         ///< Select Crypto opmode OFB
	CRYPTO_CTR,                         ///< Select Crypto opmode CTR
	CRYPTO_OPMODE_NUM,
	ENUM_DUMMY4WORD(CRYPTO_OPMODE)
} CRYPTO_OPMODE;


/**
    Crypto engine encrypt or decrypt
*/
typedef enum {
	CRYPTO_ENCRYPT = 0x00,              ///< Select Crypto engine encrypt
	CRYPTO_DECRYPT,                     ///< Select Crypto engine decrypt
	CRYPTO_TYPE_NUM,

	ENUM_DUMMY4WORD(CRYPTO_TYPE)
} CRYPTO_TYPE;


/**
    Crypto engine endian selection
*/
typedef enum {
	CRYPTO_BYTE_LITTLE_ENDIAN = 0x00,   ///< Select Crypto engine little endian
	CRYPTO_BYTE_BIG_ENDIAN,             ///< Select Crypto engine big endian

	CRYPTO_BYTE_ENDIAN_NUM,
	ENUM_DUMMY4WORD(CRYPTO_BYTE_ENDIAN)
} CRYPTO_BYTE_ENDIAN;


/**
    Crypto engine bit order selection
*/
typedef enum {
	CRYPTO_B_0_7 = 0x00,                ///< Select Crypto engine bit order 0 to 7
	CRYPTO_B_7_0,                       ///< Select Crypto engine bit order 7 to 0

	CRYPTO_BIT_ORDER_NUM,
	ENUM_DUMMY4WORD(CRYPTO_BIT_ORDER)
} CRYPTO_BIT_ORDER;


/**
    Crypto engine Access Mode selection
*/
typedef enum {
	CRYPTO_PIO = 0x00,                ///< Select Crypto PIO Mode
	CRYPTO_DMA,                       ///< Select Crypto DMA Mode

	CRYPTO_ACCMOD_NUM,
	ENUM_DUMMY4WORD(CRYPTO_ACCMOD)
} CRYPTO_ACCMOD;


/**
    Crypto engine clock rate
*/
typedef enum {
	CRYPTO_CLOCK_80MHz = 0x00,          ///< Select Crypto clock rate 80 MHz
	CRYPTO_CLOCK_160MHz,                ///< Select Crypto clock rate 160 MHz
	CRYPTO_CLOCK_240MHz,                ///< Select Crypto clock rate 240 MHz

	CRYPTO_CLOCK_RATE_NUM,
	ENUM_DUMMY4WORD(CRYPTO_CLOCK_RATE)
} CRYPTO_CLOCK_RATE;

/**
    Crypto configuration identifier

    @note For crypto_setConfig()
*/
typedef enum {
	CRYPTO_CONFIG_ID_CLOCK_RATE,        ///< Select Crypto engine internal clock rate usage
	///< Context can be any of:
	///< - @b CRYPTO_CLOCK_80MHz:   Internal clock rate 80MHz
	///< - @b CRYPTO_CLOCK_160MHz:  Internal clock rate 160MHz
	///< - @b CRYPTO_CLOCK_240MHz:  Internal clock rate 160MHz
	ENUM_DUMMY4WORD(CRYPTO_CONFIG_ID)
} CRYPTO_CONFIG_ID;



typedef struct _CRYPT_SET {
	CRYPTO_MODE        uiMode;
	CRYPTO_OPMODE      uiOPMode;
	CRYPTO_TYPE        uiType;
	CRYPTO_BYTE_ENDIAN uiByteEndian;
	CRYPTO_BIT_ORDER   uiBitOrder;
	UINT8              ucInput[16];
	UINT8              ucKey[32];
	UINT8              ucOutput[16];
	UINT8              ucIV[16];
	UINT8              ucCounter[16];
	CRYPTO_ACCMOD      uiACCMOD;
} CRYPT_SET, *pCRYPT_SET;

typedef struct _CRYPT_OP {
    CRYPTO_OPMODE   op_mode;            ///< Operation Mode (now support ECB only)
    CRYPTO_TYPE     en_de_crypt;        ///< Encrypt or decrypt (CRYPTO_ENCRYPT or CRYPTO_DECRYPT)
    UINT32          src_addr;           ///< Source address
    UINT32          dst_addr;           ///< Destination address
    UINT32          length;             ///< length
} CRYPT_OP;

typedef enum {
	CRYPTO_1ST_KEY_SET_FIELD = 0x0,            // This if for secure boot
	CRYPTO_2ND_KEY_SET_FIELD,
	CRYPTO_3RD_KEY_SET_FIELD,
	CRYPTO_4TH_KEY_SET_FIELD,
	CRYPTO_TOTAL_KEY_SET_FIELD,

	ENUM_DUMMY4WORD(KEY_SET_FROM_OTP_FIELD)
} KEY_SET_FROM_OTP_FIELD;

STATIC_ASSERT(CRYPTO_TOTAL_KEY_SET_FIELD <= 4);

extern void crypto_swReset(void);
extern BOOL checkey(UINT32 addr);
extern void crypto_setMode(UINT32 uiMode, UINT32 uiOPMode, UINT32 uiType, UINT32 uiACCMOD);
extern void crypto_setKey(UINT8 *ucKey);
extern void crypto_setInput(UINT8 *Input);
extern void crypto_getOutput(UINT8 *Output);
extern void crypto_setVector(UINT8 *ucVector);
extern void crypto_setCounter(UINT8 *ucCounter);
extern ER   crypto_setConfig(CRYPTO_CONFIG_ID configID, UINT32 configContext);
extern ER   crypto_open(void);
extern ER   crypto_close(void);
extern BOOL crypto_isBusy(void);
extern ER   crypto_data_operation(KEY_SET_FROM_OTP_FIELD key_set, CRYPT_OP crypt_op_param);
extern ER   crypto_data_operation_by_key(UINT8 * key, CRYPT_OP crypt_op_param);


//@}

#endif // _CRYPTO_H



