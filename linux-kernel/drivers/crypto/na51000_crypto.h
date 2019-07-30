#ifndef _NA51000_CRYPTO_H_
#define _NA51000_CRYPTO_H_

typedef enum {
    NA51000_CRYPTO_MODE_DES = 0,            ///< Block Size => 64  bits, Key Size => 64  bits, Single Block Cipher
    NA51000_CRYPTO_MODE_3DES,               ///< Block Size => 64  bits, Key Size => 192 bits, Single Block Cipher
    NA51000_CRYPTO_MODE_AES_128,            ///< Block Size => 128 bits, Key Size => 128 bits, Single Block Cipher
    NA51000_CRYPTO_MODE_AES_256,            ///< Block Size => 128 bits, Key Size => 256 bits, Single Block Cipher
    NA51000_CRYPTO_MODE_MAX
} NA51000_CRYPTO_MODE_T;

typedef enum {
    NA51000_CRYPTO_OPMODE_ECB = 0,
    NA51000_CRYPTO_OPMODE_CBC,
    NA51000_CRYPTO_OPMODE_CFB,
    NA51000_CRYPTO_OPMODE_OFB,
    NA51000_CRYPTO_OPMODE_CTR,
    NA51000_CRYPTO_OPMODE_MAX
} NA51000_CRYPTO_OPMODE_T;

typedef enum {
    NA51000_CRYPTO_TYPE_ENCRYPT = 0,
    NA51000_CRYPTO_TYPE_DECRYPT,
    NA51000_CRYPTO_TYPE_MAX
} NA51000_CRYPTO_TYPE_T;

typedef enum {
    NA51000_CRYPTO_BYTE_ENDIAN_LIT = 0,         ///< if store 0x11223344 in memory => 0x44 0x33 0x22 0x11
    NA51000_CRYPTO_BYTE_ENDIAN_BIG,             ///< if store 0x11223344 in memory => 0x11 0x22 0x33 0x44
    NA51000_CRYPTO_BYTE_ENDIAN_MAX
} NA51000_CRYPTO_BYTE_ENDIAN_T;

typedef enum {
    NA51000_CRYPTO_BIT_ORDER_0_7 = 0,           ///< Bit order => 01234567
    NA51000_CRYPTO_BIT_ORDER_7_0,               ///< Bit order => 76543210
    NA51000_CRYPTO_BIT_ORDER_MAX
} NA51000_CRYPTO_BIT_ORDER_T;

typedef enum {
    NA51000_CRYPTO_ACCMODE_PIO = 0,
    NA51000_CRYPTO_ACCMODE_DMA,
    NA51000_CRYPTO_ACCMODE_MAX
} NA51000_CRYPTO_ACCMODE_T;

#define NA51000_CRYPTO_TIMEOUT           0x50000
#define NA51000_CRYPTO_MAX_IV_SIZE       16         ///< 16  Bytes
#define NA51000_CRYPTO_MAX_KEY_SIZE      32         ///< 32  Bytes, 256 bits
#define NA51000_CRYPTO_CIV_WORK_SIZE     128        ///< 128 Bytes
#define NA51000_CRYPTO_CTR_MAX_BLOCKS    256        ///< 256 Blocks

#define NA51000_CRYPTO_CTRL_REG          0x00       ///< crypto control
#define NA51000_CRYPTO_IN3_REG           0x10       ///< input data 96  ~ 127 bit
#define NA51000_CRYPTO_IN2_REG           0x14       ///< input data 64  ~ 95  bit
#define NA51000_CRYPTO_IN1_REG           0x18       ///< input data 32  ~ 63  bit
#define NA51000_CRYPTO_IN0_REG           0x1C       ///< input data  0  ~ 31  bit
#define NA51000_CRYPTO_IV0_REG           0x20       ///< initial vector  0  ~ 31  bit
#define NA51000_CRYPTO_IV1_REG           0x24       ///< initial vector 32  ~ 63  bit
#define NA51000_CRYPTO_IV2_REG           0x28       ///< initial vector 64  ~ 95  bit
#define NA51000_CRYPTO_IV3_REG           0x2C       ///< initial vector 96  ~ 127 bit
#define NA51000_CRYPTO_KEY0_REG          0x30       ///< key 0   ~ 31  bit
#define NA51000_CRYPTO_KEY1_REG          0x34       ///< key 32  ~ 63  bit
#define NA51000_CRYPTO_KEY2_REG          0x38       ///< key 64  ~ 95  bit
#define NA51000_CRYPTO_KEY3_REG          0x3C       ///< key 96  ~ 127 bit
#define NA51000_CRYPTO_KEY4_REG          0x40       ///< key 128 ~ 159 bit
#define NA51000_CRYPTO_KEY5_REG          0x44       ///< key 160 ~ 191 bit
#define NA51000_CRYPTO_KEY6_REG          0x48       ///< key 192 ~ 223 bit
#define NA51000_CRYPTO_KEY7_REG          0x4C       ///< key 224 ~ 255 bit
#define NA51000_CRYPTO_CNT0_REG          0x50       ///< counter value  0  ~ 31  bit, CTR mode only
#define NA51000_CRYPTO_CNT1_REG          0x54       ///< counter value 32  ~ 63  bit, CTR mode only
#define NA51000_CRYPTO_CNT2_REG          0x58       ///< counter value 64  ~ 95  bit, CTR mode only
#define NA51000_CRYPTO_CNT3_REG          0x5C       ///< counter value 96  ~ 127 bit, CTR mode only
#define NA51000_CRYPTO_OUT3_REG          0x60       ///< output data 96  ~ 127 bit
#define NA51000_CRYPTO_OUT2_REG          0x64       ///< output data 64  ~ 95  bit
#define NA51000_CRYPTO_OUT1_REG          0x68       ///< output data 32  ~ 63  bit
#define NA51000_CRYPTO_OUT0_REG          0x6C       ///< output data  0  ~ 31  bit
#define NA51000_CRYPTO_CURR_IV0_REG      0x70       ///< running vector  0  ~ 31  bit
#define NA51000_CRYPTO_CURR_IV1_REG      0x74       ///< running vector 32  ~ 63  bit
#define NA51000_CRYPTO_CURR_IV2_REG      0x78       ///< running vector 64  ~ 95  bit
#define NA51000_CRYPTO_CURR_IV3_REG      0x7C       ///< running vector 96  ~ 127 bit
#define NA51000_CRYPTO_FLAG_REG          0x80       ///< crypto idle or busy
#define NA51000_CRYPTO_DMA_CTRL_REG      0x88       ///< DMA transfer enable/disable
#define NA51000_CRYPTO_DMA_CFG_REG       0x90       ///< DMA interrupt enable/disable
#define NA51000_CRYPTO_DMA_STS_REG       0x94       ///< DMA transfer status
#define NA51000_CRYPTO_DMA_TX_SIZE_REG   0x98       ///< buffer size in byte uint, must word alignment
#define NA51000_CRYPTO_DMA_TX_CNT_REG    0x9c       ///< show how many bytes of DMA data already transferred
#define NA51000_CRYPTO_DMA_SRC_REG       0xA0       ///< source buffer address, must be word alignment
#define NA51000_CRYPTO_DMA_DST_REG       0xA4       ///< destination buffer address, must be word alignment

#define NA51000_CRYPTO_FLAG_BUSY_BIT     0x1

#endif  /* _NA51000_CRYPTO_H_ */
