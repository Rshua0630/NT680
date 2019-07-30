#include "i2c.h"


typedef enum {
	TEMPIC_I2C_REGISTER_1BYTE = 1,
	TEMPIC_I2C_REGISTER_2BYTE = 2,
	ENUM_DUMMY4WORD(TEMPIC_I2C_REGISTER_BYTES)
} TEMPIC_I2C_REGISTER_BYTES, *PTEMPIC_I2C_REGISTER_BYTES;

typedef struct {
	I2C_SESSION I2C_Channel;
	TEMPIC_I2C_REGISTER_BYTES  I2C_RegBytes;
	I2C_PINMUX      I2C_PinMux;
	I2C_BUS_CLOCK   I2C_BusClock;
	UINT32          I2C_Slave_WAddr;
	UINT32          I2C_Slave_RAddr;
} TEMPIC_INFO, *PTEMPIC_INFO;

//extern BOOL TempIC_Open(void);
//extern BOOL TempIC_Close(void);

extern UINT32 TempIC_GetTempValue(void);
extern BOOL TempIC_I2C_Init(void);





