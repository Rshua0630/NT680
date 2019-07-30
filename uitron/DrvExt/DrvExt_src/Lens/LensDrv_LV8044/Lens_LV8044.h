/**
 * Novatek SANYO LV8044 driver header.
 * 
 * Reference LV8044 driver header.
 * 
 * @file Lens_LV8044.h
 * @note Nothing.
 *
 * Copyright (C) 2018 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _LENS_LV8044_H_
#define _LENS_LV8044_H_

#include "Type.h"
#include "DxLens.h"
#include "timer.h"
#include "Utility.h"
#include "Lens_Common.h"


#define __MODULE__          LENSDRV_LV8044
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


//=============================================================================
// compile switch
//=============================================================================


//=============================================================================
// constant define
//=============================================================================
#define LENS_DRV_NAME               "SANYO-LV8044"
#define LENS_DRV_ID                 "nvt-ver0.99"     // *


//=============================================================================
// HW pin configuration
//=============================================================================
#if 0
#define MOTOR_LV8044_SIF_CH         SIF_CH2           // *
#else
#define MOTOR_LV8044_SPI_CH         SPI_ID_3          // *
#endif


//=============================================================================
// lens & motor parameters
//=============================================================================
#define LENS_FOCAL_LENGTH_WIDE      2700     // * 2.7 mm
#define LENS_FOCAL_LENGTH_TELE      12000    // * 12 mm

#define NEAR_BOUND                  0        // *
#define INF_BOUND                   1110     // * 3060/2=1530
#define WIDE_BOUND                  0        // *
#define TELE_BOUND                  680      // * 2360/2=1180
#define INIT_FOCUS_POS              400      // *
#define INIT_ZOOM_POS               0        // *

#define FOCUS_BACKLASH_CORR         6        // * correction for focus backlash
#define ZOOM_BACKLASH_CORR          6        // * correction for zoom backlash


//=============================================================================
// register definition
//=============================================================================
#define LENS_REG_DEF_BEGIN(name)     \
typedef union                        \
{                                    \
    UINT8 Reg;                       \
    struct                           \
    {                                \

#define LENS_REG_DEF_BIT(field, bits)\
        unsigned field : bits;

#define LENS_REG_DEF_END(name)       \
    } Bit;                           \
} T_##name;


// REG_00
LENS_REG_DEF_BEGIN(CHNEL12_EXCITE_MODE_00)
    LENS_REG_DEF_BIT(ADDR00,              3)
    LENS_REG_DEF_BIT(CHNEL12_EXCITE_MODE, 2)
    LENS_REG_DEF_BIT(CHNEL12_REF_VOLT,    2)
LENS_REG_DEF_END(CHNEL12_EXCITE_MODE_00)

// REG_01
LENS_REG_DEF_BEGIN(CHNEL12_MOVE_CTRL_01)
    LENS_REG_DEF_BIT(ADDR01,   3)
    LENS_REG_DEF_BIT(CCWCWAB,  1)
    LENS_REG_DEF_BIT(HOLDAB,   1)
    LENS_REG_DEF_BIT(CNTRSTAB, 1)
    LENS_REG_DEF_BIT(ENDISAB,  1)
LENS_REG_DEF_END(CHNEL12_MOVE_CTRL_01)

// REG_02
LENS_REG_DEF_BEGIN(CHNEL34_EXCITE_MODE_02)
    LENS_REG_DEF_BIT(ADDR02,              3)
    LENS_REG_DEF_BIT(CHNEL34_EXCITE_MODE, 2)
    LENS_REG_DEF_BIT(CHNEL34_REF_VOLT,    2)
    LENS_REG_DEF_BIT(CHNEL34_OUT_MODE,    1)
LENS_REG_DEF_END(CHNEL34_EXCITE_MODE_02)

// REG_03
LENS_REG_DEF_BEGIN(CHNEL34_MOVE_CTRL_03)
    LENS_REG_DEF_BIT(ADDR03,   3)
    LENS_REG_DEF_BIT(CCWCWCD,  1)
    LENS_REG_DEF_BIT(HOLDCD,   1)
    LENS_REG_DEF_BIT(CNTRSTCD, 1)
    LENS_REG_DEF_BIT(ENDISCD,  1)
LENS_REG_DEF_END(CHNEL34_MOVE_CTRL_03)

// REG_04
LENS_REG_DEF_BEGIN(CHNEL56_ENERGIZE_04)
    LENS_REG_DEF_BIT(ADDR04,    3)
    LENS_REG_DEF_BIT(ENERG5DIR, 2)
    LENS_REG_DEF_BIT(ENERG6DIR, 2)
LENS_REG_DEF_END(CHNEL56_ENERGIZE_04)

// REG_05
LENS_REG_DEF_BEGIN(CHNEL56_REF_VOLT_05)
    LENS_REG_DEF_BIT(ADDR05,   3)
    LENS_REG_DEF_BIT(REFCHNEL, 1)
    LENS_REG_DEF_BIT(REFVOLT,  4)
LENS_REG_DEF_END(CHNEL56_REF_VOLT_05)

// REG_06
LENS_REG_DEF_BEGIN(PHOTO_SENSOR_06)
    LENS_REG_DEF_BIT(ADDR06,   3)
    LENS_REG_DEF_BIT(PHOTO1EN, 1)
    LENS_REG_DEF_BIT(PHOTO2EN, 1)
    LENS_REG_DEF_BIT(PHOTO3EN, 1)
LENS_REG_DEF_END(PHOTO_SENSOR_06)

// REG_07
LENS_REG_DEF_BEGIN(CHOP_FREQ_07)
    LENS_REG_DEF_BIT(ADDR07,    3)
    LENS_REG_DEF_BIT(PI3MO,     1)
    LENS_REG_DEF_BIT(MOCHNEL,   1)
    LENS_REG_DEF_BIT(MOPOS,     1)
    LENS_REG_DEF_BIT(CHOP_FREQ, 2)
LENS_REG_DEF_END(CHOP_FREQ_07)

// REG
LENS_REG_DEF_BEGIN(LV8044_REG)
    LENS_REG_DEF_BIT(BIT0_7, 8)
LENS_REG_DEF_END(LV8044_REG)


typedef struct
{
    //T_CHNEL12_EXCITE_MODE_00 R00;
    //T_CHNEL12_MOVE_CTRL_01   R01;
    //T_CHNEL34_EXCITE_MODE_02 R02;
    //T_CHNEL34_MOVE_CTRL_03   R03;
    //T_CHNEL56_ENERGIZE_04    R04;
    //T_CHNEL56_REF_VOLT_05    R05;
    //T_PHOTO_SENSOR_06        R06;
    //T_CHOP_FREQ_07           R07;

    T_LV8044_REG REG[8];
} LV8044_REGISTER;


//=============================================================================
// struct & flag definition
//=============================================================================
typedef enum
{
    LV8044_CHNEL12_EXCITE_MODE = 0x00,
    LV8044_CHNEL12_MOVE_CTRL   = 0x01,
    LV8044_CHNEL34_EXCITE_MODE = 0x02,
    LV8044_CHNEL34_MOVE_CTRL   = 0x03,
    LV8044_CHNEL56_ENERGIZE    = 0x04,
    LV8044_CHNEL56_REF_VOLT    = 0x05,
    LV8044_PHOTO_SENSOR        = 0x06,
    LV8044_CHOP_FREQ           = 0x07,
    LV8044_REG_UNKNOWN         = 0x07
} LV8044_REG_CMD;

typedef enum
{
    LV8044_CHNEL1 = 0,
    LV8044_CHNEL3,
    ENUM_DUMMY4WORD(LV8044_CHNEL_NUM)
} LV8044_CHNEL_NUM;

typedef enum
{
    LV8044_MODE_2PHASE       = 1,
    LV8044_MODE_12PHASE_FULL = 2,
    LV8044_MODE_12PHASE      = 3,
    LV8044_MODE_4W12PHASE    = 16,    // 64 microstep
    LV8044_MODE_PWM,
    LV8044_MODE_UNKNOWN
} LV8044_MACROSTEP_MODE;

typedef enum
{
    LV8044_MOTOR_SPD_QUARTER = 1,
    LV8044_MOTOR_SPD_HALF    = 2,
    LV8044_MOTOR_SPD_1X      = 4,
    LV8044_MOTOR_SPD_2X      = 8,
    LV8044_MOTOR_SPD_4X      = 16,
    ENUM_DUMMY4WORD(LV8044_MOTOR_SPEED)
} LV8044_MOTOR_SPEED;

typedef enum
{
    LV8044_CMD_SET_MODE,
    LV8044_CMD_SET_CURR,
    LV8044_CMD_DIRECTION,
    LV8044_CMD_RESET,
    LV8044_CMD_ENABLE,
    LV8044_CMD_UNKNOWN,
    ENUM_DUMMY4WORD(LV8044_CMD_TYPE)
} LV8044_CMD_TYPE;


typedef struct
{
    LV8044_CHNEL_NUM eChnelSel;
    LV8044_MACROSTEP_MODE eMicroStepMode;    // micro step mode
    UINT32 uiPwmOutputPin;
    SWTIMER_ID eSwTimerID;
    INT32 iDefPositiveDir;
    UINT32 uiPwmFreq;
    volatile INT32 iWorkStatus;
    volatile INT32 iMotorStatus;
    INT32 iMaxStepSize;
    BOOL bMove2Home;
    volatile INT32 iCurrPosition;
    BOOL bSetFinalStep;
    INT32 iMoveDir;
    LV8044_MOTOR_SPEED eMoveSpeed;
    UINT32 uiPwmBaseNum;
    INT32 iBacklashValue;
    BOOL bSetMove2Stop;
    INT32 iInitPosBias;
} MOTOR_CONFIG;


typedef struct
{
    MOTOR_TAB LensMotor;

    UINT32 uiPinIn[LENS_IO_MAX];    // GPIO related variable

    LV8044_REGISTER LensMotorReg;

    MOTOR_CONFIG LensMotorConfig[2];    // for focus & zoom

    LENS_ZOOM_FOCUS_TAB ZoomFocusTbl[ZOOM_SECTION_NUM];

    LENS_FOCUS_DISTANCE_TAB FocusDistanceTbl[ZOOM_SECTION_NUM];
} LENSMOTOR_INFO;


//=============================================================================
// external functions
//=============================================================================
extern LENSMOTOR_INFO* LV8044_GetMotorInfo(void);
extern INT32 LV8044_TransmitCmd(LV8044_CHNEL_NUM chnel, LV8044_CMD_TYPE cmd_type, INT32 cmd_arg);


#endif    // _LENS_LV8044_H_


