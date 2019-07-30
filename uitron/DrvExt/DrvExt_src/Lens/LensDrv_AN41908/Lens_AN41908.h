/**
 * Novatek PANASONIC AN41908 driver header.
 * 
 * Reference AN41908 driver header.
 * 
 * @file Lens_AN41908.h
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _LENS_AN41908_H_
#define _LENS_AN41908_H_

#include "Type.h"
#include "DxLens.h"
//#include "timer.h"
//#include "Utility.h"
#include "Lens_Common.h"


#define __MODULE__          LENSDRV_AN41908
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


//=============================================================================
// compile switch
//=============================================================================
#define FZ_PLS_PIN_EXIST           0    // *
#define ISR_RISING_EDGE_TRIGGER    1


//=============================================================================
// constant define
//=============================================================================
#define LENS_DRV_NAME               "PANA-AN41908"
#define LENS_DRV_ID                 "nvt-ver0.99"     // *


//=============================================================================
// HW pin configuration
//=============================================================================
#define MOTOR_AN41908_SPI_CH        SPI_ID_2          // *


//=============================================================================
// lens & motor parameters
//=============================================================================
#define LENS_FOCAL_LENGTH_WIDE      2800     // * 2.8 mm
#define LENS_FOCAL_LENGTH_TELE      12000    // * 12 mm

#define NEAR_BOUND                  0        // *
#define INF_BOUND                   1095     // *
#define WIDE_BOUND                  0        // *
#define TELE_BOUND                  920      // *
#define INIT_FOCUS_POS              10       // *
#define INIT_ZOOM_POS               120      // *

#define FOCUS_BACKLASH_CORR         10       // * correction for focus backlash
#define ZOOM_BACKLASH_CORR          10       // * correction for zoom backlash

#if !FZ_PLS_PIN_EXIST
#define FOCUS_PSUM_DELAY            28       // * unit: msec (12*AN41908_PULSE1_STEP_CYCLE/27MHz)*255=26 msec
#define ZOOM_PSUM_DELAY             56       // * unit: msec (12*AN41908_PULSE2_STEP_CYCLE/27MHz)*255=53 msec
#else
#define FOCUS_PSUM_DELAY            1
#define ZOOM_PSUM_DELAY             1
#endif

#define STANDBY_PEAK_PULSE_WIDTH    0x3232    // set to 30%  (vary with PWMMOD=21) 0.3*21*8
#define GOHOME_FPEAK_PULSE_WIDTH    0xFCFC    // set to 150% (vary with PWMMOD=21) 1.5*21*8
#define GOHOME_ZPEAK_PULSE_WIDTH    0xFCFC    // set to 150% (vary with PWMMOD=21) 1.5*21*8


//=============================================================================
// register definition
//=============================================================================
#define LENS_REG_DEF_BEGIN(name)     \
typedef union                        \
{                                    \
    UINT16 Reg;                      \
    struct                           \
    {                                \

#define LENS_REG_DEF_BIT(field, bits)\
        unsigned field : bits;

#define LENS_REG_DEF_END(name)       \
    } Bit;                           \
} T_##name;


// REG_00
LENS_REG_DEF_BEGIN(IRIS_TARGET_00)
    LENS_REG_DEF_BIT(IRS_TGT, 10)
LENS_REG_DEF_END(IRIS_TARGET_00)

// REG_01
LENS_REG_DEF_BEGIN(IRIS_LPF_FC_CTL_01)
    LENS_REG_DEF_BIT(OVER_LPF_FC_1ST, 2)
    LENS_REG_DEF_BIT(OVER_LPF_FC_2ND, 2)
    LENS_REG_DEF_BIT(DEC_AVE,         1)
    LENS_REG_DEF_BIT(AS_FLT_OFF,      1)
    LENS_REG_DEF_BIT(ASOUND_LPF_FC,   3)
    LENS_REG_DEF_BIT(DGAIN,           7)
LENS_REG_DEF_END(IRIS_LPF_FC_CTL_01)

// REG_02
LENS_REG_DEF_BEGIN(IRIS_POLE_ZERO_CTL_02)
    LENS_REG_DEF_BIT(IRIS_CALC_NR, 4)
    LENS_REG_DEF_BIT(IRIS_ROUND,   4)
    LENS_REG_DEF_BIT(PID_ZERO,     4)
    LENS_REG_DEF_BIT(PID_POLE,     4)
LENS_REG_DEF_END(IRIS_POLE_ZERO_CTL_02)

// REG_03
LENS_REG_DEF_BEGIN(IRIS_PWM_CTL_03)
    LENS_REG_DEF_BIT(ARW,         4)
    LENS_REG_DEF_BIT(LMT_ENB,     1)
    LENS_REG_DEF_BIT(PWM_FLT_OFF, 1)
    LENS_REG_DEF_BIT(PWM_LPF_FC,  3)
    LENS_REG_DEF_BIT(PWM_IRIS,    3)
    LENS_REG_DEF_BIT(DT_ADJ_IRIS, 2)
LENS_REG_DEF_END(IRIS_PWM_CTL_03)

// REG_04
LENS_REG_DEF_BEGIN(IRIS_HALL_SENSOR_04)
    LENS_REG_DEF_BIT(HALL_BIAS_DAC,   8)
    LENS_REG_DEF_BIT(HALL_OFFSET_DAC, 8)
LENS_REG_DEF_END(IRIS_HALL_SENSOR_04)

// REG_05
LENS_REG_DEF_BEGIN(IRIS_HALL_SENSOR_GAIN_05)
    LENS_REG_DEF_BIT(TGT_LPF_FC,  4)
    LENS_REG_DEF_BIT(TGT_FLT_OFF, 1)
    LENS_REG_DEF_BIT(PID_INV,     1)
    LENS_REG_DEF_BIT(RESERVED,    2)
    LENS_REG_DEF_BIT(HALL_GAIN,   4)
    LENS_REG_DEF_BIT(AAF_FC,      1)
LENS_REG_DEF_END(IRIS_HALL_SENSOR_GAIN_05)

// REG_06
LENS_REG_DEF_BEGIN(PULSE1_START_TIME_06)
    LENS_REG_DEF_BIT(START1_TIME, 10)
LENS_REG_DEF_END(PULSE1_START_TIME_06)

// REG_07
LENS_REG_DEF_BEGIN(PULSE1_WIDTH_07)
    LENS_REG_DEF_BIT(WIDTH1,   12)
    LENS_REG_DEF_BIT(RESERVED, 3)
    LENS_REG_DEF_BIT(P1EN,     1)
LENS_REG_DEF_END(PULSE1_WIDTH_07)

// REG_08
LENS_REG_DEF_BEGIN(PULSE2_START_TIME_08)
    LENS_REG_DEF_BIT(START2_TIME, 10)
LENS_REG_DEF_END(PULSE2_START_TIME_08)

// REG_09
LENS_REG_DEF_BEGIN(PULSE2_WIDTH_09)
    LENS_REG_DEF_BIT(WIDTH2,   6)
    LENS_REG_DEF_BIT(RESERVED, 9)
    LENS_REG_DEF_BIT(P2EN,     1)
LENS_REG_DEF_END(PULSE2_WIDTH_09)

// REG_0A
LENS_REG_DEF_BEGIN(IRIS_TEST_0A)
    LENS_REG_DEF_BIT(TGT_IN_TEST, 10)
    LENS_REG_DEF_BIT(DUTY_TEST,   1)
LENS_REG_DEF_END(IRIS_TEST_0A)

// REG_0B
LENS_REG_DEF_BEGIN(VD_FZ_POLARITY_0B)
    LENS_REG_DEF_BIT(RESERVED0,    3)
    LENS_REG_DEF_BIT(ASWMODE,      2)
    LENS_REG_DEF_BIT(RESERVED1,    2)
    LENS_REG_DEF_BIT(TESTEN1,      1)
    LENS_REG_DEF_BIT(MODESEL_IRIS, 1)
    LENS_REG_DEF_BIT(MODESEL_FZ,   1)
    LENS_REG_DEF_BIT(PDWNB,        1)
    LENS_REG_DEF_BIT(ADC_TEST,     1)
    LENS_REG_DEF_BIT(PID_CLIP,     4)
LENS_REG_DEF_END(VD_FZ_POLARITY_0B)

// REG_0C
LENS_REG_DEF_BEGIN(IRIS_ADC_OUTPUT_0C)
    LENS_REG_DEF_BIT(IRSAD, 10)
LENS_REG_DEF_END(IRIS_ADC_OUTPUT_0C)

// REG_0E
LENS_REG_DEF_BEGIN(IRIS_TARGET_AVG_SPEED_0E)
    LENS_REG_DEF_BIT(TGT_UPDATE, 8)
    LENS_REG_DEF_BIT(AVE_SPEED,  5)
LENS_REG_DEF_END(IRIS_TARGET_AVG_SPEED_0E)

// REG_20
LENS_REG_DEF_BEGIN(PULSE_FREQUENCY_20)
    LENS_REG_DEF_BIT(DT1,     8)
    LENS_REG_DEF_BIT(PWMMODE, 5)
    LENS_REG_DEF_BIT(PWMRES,  2)
LENS_REG_DEF_END(PULSE_FREQUENCY_20)

// REG_21
LENS_REG_DEF_BEGIN(PLS_OUTPUT_21)
    LENS_REG_DEF_BIT(FZTEST,   5)
    LENS_REG_DEF_BIT(RESERVED, 2)
    LENS_REG_DEF_BIT(TESTEN2,  1)
LENS_REG_DEF_END(PLS_OUTPUT_21)

// REG_22
LENS_REG_DEF_BEGIN(PULSE1_PHASE_CORR_22)
    LENS_REG_DEF_BIT(DT2A,    8)
    LENS_REG_DEF_BIT(PHMODAB, 6)
LENS_REG_DEF_END(PULSE1_PHASE_CORR_22)

// REG_23
LENS_REG_DEF_BEGIN(PULSE1_MAX_DUTY_WIDTH_23)
    LENS_REG_DEF_BIT(PPWA, 8)
    LENS_REG_DEF_BIT(PPWB, 8)
LENS_REG_DEF_END(PULSE1_MAX_DUTY_WIDTH_23)

// REG_24
LENS_REG_DEF_BEGIN(PULSE1_MOVE_STEP_24)
    LENS_REG_DEF_BIT(PSUMAB,  8)
    LENS_REG_DEF_BIT(CCWCWAB, 1)
    LENS_REG_DEF_BIT(BRAKEAB, 1)
    LENS_REG_DEF_BIT(ENDISAB, 1)
    LENS_REG_DEF_BIT(LEDB,    1)
    LENS_REG_DEF_BIT(MICROAB, 2)
LENS_REG_DEF_END(PULSE1_MOVE_STEP_24)

// REG_25
LENS_REG_DEF_BEGIN(PULSE1_STEP_CYCLE_25)
    LENS_REG_DEF_BIT(INTCTAB, 16)
LENS_REG_DEF_END(PULSE1_STEP_CYCLE_25)

// REG_27
LENS_REG_DEF_BEGIN(PULSE2_PHASE_CORR_27)
    LENS_REG_DEF_BIT(DT2B,    8)
    LENS_REG_DEF_BIT(PHMODCD, 6)
LENS_REG_DEF_END(PULSE2_PHASE_CORR_27)

// REG_28
LENS_REG_DEF_BEGIN(PULSE2_MAX_DUTY_WIDTH_28)
    LENS_REG_DEF_BIT(PPWC, 8)
    LENS_REG_DEF_BIT(PPWD, 8)
LENS_REG_DEF_END(PULSE2_MAX_DUTY_WIDTH_28)

// REG_29
LENS_REG_DEF_BEGIN(PULSE2_MOVE_STEP_29)
    LENS_REG_DEF_BIT(PSUMCD,  8)
    LENS_REG_DEF_BIT(CCWCWCD, 1)
    LENS_REG_DEF_BIT(BRAKECD, 1)
    LENS_REG_DEF_BIT(ENDISCD, 1)
    LENS_REG_DEF_BIT(LEDA,    1)
    LENS_REG_DEF_BIT(MICROCD, 2)
LENS_REG_DEF_END(PULSE2_MOVE_STEP_29)

// REG_2A
LENS_REG_DEF_BEGIN(PULSE2_STEP_CYCLE_2A)
    LENS_REG_DEF_BIT(INTCTCD, 16)
LENS_REG_DEF_END(PULSE2_STEP_CYCLE_2A)

// REG_2C
LENS_REG_DEF_BEGIN(IRCUT_CONTROL_2C)
    LENS_REG_DEF_BIT(IR2, 1)
    LENS_REG_DEF_BIT(IR1, 1)
    LENS_REG_DEF_BIT(IRSWICH, 1)
LENS_REG_DEF_END(IRCUT_CONTROL_2C)

// REG
LENS_REG_DEF_BEGIN(AN41908_REG)
    LENS_REG_DEF_BIT(BIT0_15, 16)
LENS_REG_DEF_END(AN41908_REG)


typedef struct
{
    //T_IRIS_TARGET_00           R00;// 0
    //T_IRIS_LPF_FC_CTL_01       R01;// 1
    //T_IRIS_POLE_ZERO_CTL_02    R02;// 2
    //T_IRIS_PWM_CTL_03          R03;// 3
    //T_IRIS_HALL_SENSOR_04      R04;// 4
    //T_IRIS_HALL_SENSOR_GAIN_05 R05;// 5
    //T_PULSE1_START_TIME_06     R06;// 6
    //T_PULSE1_WIDTH_07          R07;// 7
    //T_PULSE2_START_TIME_08     R08;// 8
    //T_PULSE2_WIDTH_09          R09;// 9
    //T_IRIS_TEST_0A             R0A;// 10
    //T_VD_FZ_POLARITY_0B        R0B;// 11
    //T_IRIS_ADC_OUTPUT_0C       R0C;// 12
    //T_IRIS_TARGET_AVG_SPEED_0E R0E;// 13
    //T_PULSE_FREQUENCY_20       R20;// 14
    //T_PLS_OUTPUT_21            R21;// 15
    //T_PULSE1_PHASE_CORR_22     R22;// 16
    //T_PULSE1_MAX_DUTY_WIDTH_23 R23;// 17
    //T_PULSE1_MOVE_STEP_24      R24;// 18
    //T_PULSE1_STEP_CYCLE_25     R25;// 19
    //T_PULSE2_PHASE_CORR_27     R27;// 20
    //T_PULSE2_MAX_DUTY_WIDTH_28 R28;// 21
    //T_PULSE2_MOVE_STEP_29      R29;// 22
    //T_PULSE2_STEP_CYCLE_2A     R2A;// 23

    T_AN41908_REG REG[24];
} AN41908_REGISTER;


//=============================================================================
// struct & flag definition
//=============================================================================
typedef enum
{
    AN41908_IRIS_TARGET            = 0x00,
    AN41908_IRIS_LPF_FC_CTL        = 0x01,
    AN41908_IRIS_POLE_ZERO_CTL     = 0x02,
    AN41908_IRIS_PWM_CTL           = 0x03,
    AN41908_IRIS_HALL_SENSOR       = 0x04,
    AN41908_IRIS_HALL_SENSOR_GAIN  = 0x05,
    AN41908_PULSE1_START_TIME      = 0x06,
    AN41908_PULSE1_WIDTH           = 0x07,
    AN41908_PULSE2_START_TIME      = 0x08,
    AN41908_PULSE2_WIDTH           = 0x09,
    AN41908_IRIS_TEST              = 0x0A,
    AN41908_VD_FZ_POLARITY         = 0x0B,
    AN41908_IRIS_ADC_OUTPUT        = 0x0C,
    AN41908_IRIS_TARGET_AVG_SPEED  = 0x0E,
    AN41908_PULSE_FREQUENCY        = 0x20,
    AN41908_PLS_OUTPUT             = 0x21,
    AN41908_PULSE1_PHASE_CORR      = 0x22,
    AN41908_PULSE1_MAX_DUTY_WIDTH  = 0x23,
    AN41908_PULSE1_MOVE_STEP       = 0x24,
    AN41908_PULSE1_STEP_CYCLE      = 0x25,
    AN41908_PULSE2_PHASE_CORR      = 0x27,
    AN41908_PULSE2_MAX_DUTY_WIDTH  = 0x28,
    AN41908_PULSE2_MOVE_STEP       = 0x29,
    AN41908_PULSE2_STEP_CYCLE      = 0x2A,
    AN41908_IRCUT_CTL              = 0x2C,
    AN41908_REG_UNKNOWN            = 0x3F
} AN41908_REG_CMD;

typedef enum
{
    AN41908_CHNEL1 = 0,
    AN41908_CHNEL3,
    ENUM_DUMMY4WORD(AN41908_CHNEL_NUM)
} AN41908_CHNEL_NUM;

typedef enum
{
    AN41908_MODE_64MACRO  = 16,    // depending on gaer ratio
    AN41908_MODE_128MACRO = 32,
    AN41908_MODE_256MACRO = 64,
    AN41908_MODE_UNKNOWN  = 0xFF
} AN41908_MACROSTEP_MODE;

typedef enum
{
    AN41908_MOTOR_SPD_QUARTER = 1,
    AN41908_MOTOR_SPD_HALF    = 2,
    AN41908_MOTOR_SPD_1X      = 4,
    AN41908_MOTOR_SPD_2X      = 8,
    AN41908_MOTOR_SPD_4X      = 16,
    ENUM_DUMMY4WORD(AN41908_MOTOR_SPEED)
} AN41908_MOTOR_SPEED;

typedef enum
{
    AN41908_SET_MOTOR_CFG = 0,
    AN41908_SET_MOTOR_SPD,
    AN41908_SET_MOTOR_STEP,
    ENUM_DUMMY4WORD(AN41908_CMD_TYPE)
} AN41908_CMD_TYPE;


typedef struct
{
    AN41908_CHNEL_NUM eChnelSel;
    AN41908_MACROSTEP_MODE eMicroStepMode;    // micro step mode
    INT32 iExciteEnable;
    INT32 iDefPositiveDir;
    UINT32 uiPwmFreq;
    volatile INT32 iWorkStatus;
    volatile INT32 iMotorStatus;
    INT32 iMaxStepSize;
    BOOL bMove2Home;
    volatile INT32 iCurrPosition;
    BOOL bSetFinalStep;
    INT32 iMoveDir;
    AN41908_MOTOR_SPEED eMoveSpeed;
    INT32 iBacklashValue;
    UINT16 wMaxPulseWidth;
    INT32 iDlyTime;
    BOOL bSetMove2Stop;
    INT32 iInitPosBias;
} MOTOR_CONFIG;


typedef struct
{
    MOTOR_TAB LensMotor;

    UINT32 uiPinIn[LENS_IO_MAX];    // GPIO related variable

    AN41908_REGISTER LensMotorReg;

    MOTOR_CONFIG LensMotorConfig[2];    // for focus & zoom

    LENS_ZOOM_FOCUS_TAB ZoomFocusTbl[ZOOM_SECTION_NUM];

    LENS_FOCUS_DISTANCE_TAB FocusDistanceTbl[ZOOM_SECTION_NUM];
} LENSMOTOR_INFO;


//=============================================================================
// external functions
//=============================================================================
extern LENSMOTOR_INFO* AN41908_GetMotorInfo(void);
extern INT32 AN41908_TransmitCmd(AN41908_CHNEL_NUM chnel, AN41908_CMD_TYPE cmd_type, INT32 cmd_arg);


#endif    // _LENS_AN41908_H_


