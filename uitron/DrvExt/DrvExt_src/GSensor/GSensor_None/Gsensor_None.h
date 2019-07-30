
#ifndef _GSENSOR_NONE_H
#define _GSENSOR_NONE_H
#include "Type.h"
#include "IOInit.h"
#include "pad.h"
#include "top.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"

#define STATUS_REG          0x00
#define OUT_X_MSB           0x01
#define OUT_X_LSB           0x02
#define OUT_Y_MSB           0x03
#define OUT_Y_LSB           0x04
#define OUT_Z_MSB           0x05
#define OUT_Z_LSB           0x06
#define SYSMOD_REG          0x0B
#define INT_SOURCE          0x0C
#define WHO_AM_I            0x0D
#define TRANSIENT_SRC       0x1E
#define OUT_X_REG           0x41
#define OUT_Y_REG           0x42
#define OUT_Z_REG           0x43
#define CTRL_REG1           0x44
#define CTRL_REG2           0x45
#define CTRL_REG3           0x46
#define CTRL_REG4           0x47
#define CTRL_REG5           0x48
#define XYZOR_REG           0x49
#define CTRL_REG6           0x4A
#define CTRL_REG7           0x4C
#define CTRL_REG8           0x4D

#define SW_RST              0x53

#define DRAMD07_WRITE_ID    0x38
#define DRAMD07_READ_ID     0x39
#define DRAMD07_DRV_ID      0x07

#endif

