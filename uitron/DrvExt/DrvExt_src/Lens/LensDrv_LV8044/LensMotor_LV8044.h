/**
 * Novatek SANYO LV8044 motor driver header.
 * 
 * Reference LV8044 motor driver header.
 * 
 * @file LensMotor_LV8044.h
 * @note Nothing.
 *
 * Copyright (C) 2018 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _LENSMOTOR_LV8044_H_
#define _LENSMOTOR_LV8044_H_

#include "Type.h"
#include "pwm.h"
#include "Lens_LV8044.h"


//=============================================================================
// constant define
//=============================================================================


//=============================================================================
// struct & flag definition
//=============================================================================
typedef enum
{
    MOTOR_FOCUS = 0,
    MOTOR_ZOOM,
    MOTOR_IRIS,
    MOTOR_IRCUT,
    ENUM_DUMMY4WORD(MOTOR_SEL)
} MOTOR_SEL;


typedef struct
{
    union {
        struct {
            UINT8 b0:1;
            UINT8 b1:1;
            UINT8 b2:1;
            UINT8 b3:1;
            UINT8 b4:1;
            UINT8 b5:1;
            UINT8 b6:1;
            UINT8 b7:1;
        } B;

        UINT8 data;       // written data
    };
} SPI_DATA_FMT;


//=============================================================================
// motor driver parameters
//=============================================================================
static LENSMOTOR_INFO g_LensMotorInfo =
{
    .uiPinIn = {0},

    .LensMotorReg.REG[0] = 0x78 | 0x00,
    .LensMotorReg.REG[1] = 0x00 | 0x01,
    .LensMotorReg.REG[2] = 0xF8 | 0x02,
    .LensMotorReg.REG[3] = 0x00 | 0x03,
    .LensMotorReg.REG[4] = 0x00 | 0x04,
    .LensMotorReg.REG[5] = 0xF0 | 0x05,
    .LensMotorReg.REG[6] = 0x00 | 0x06,
    .LensMotorReg.REG[7] = 0x00 | 0x07,

    .LensMotorConfig[0].eChnelSel       = LV8044_CHNEL1,               // *
    .LensMotorConfig[0].eMicroStepMode  = LV8044_MODE_4W12PHASE,
    .LensMotorConfig[0].uiPwmOutputPin  = PWMID_0,                     // * PWM0
    .LensMotorConfig[0].eSwTimerID      = 0,
    .LensMotorConfig[0].iDefPositiveDir = 1,                           // *
    .LensMotorConfig[0].uiPwmFreq       = 520,    // must be the same with zoom's one
    .LensMotorConfig[0].iWorkStatus     = MOTOR_STATUS_FREE,
    .LensMotorConfig[0].iMotorStatus    = MOTOR_STATUS_FREE,
    .LensMotorConfig[0].iMaxStepSize    = 8,
    .LensMotorConfig[0].bMove2Home      = FALSE,
    .LensMotorConfig[0].iCurrPosition   = NEAR_BOUND,
    .LensMotorConfig[0].bSetFinalStep   = FALSE,
    .LensMotorConfig[0].iMoveDir        = MOVE_DIR_INIT,
    .LensMotorConfig[0].eMoveSpeed      = LV8044_MOTOR_SPD_2X,         // *
    .LensMotorConfig[0].uiPwmBaseNum    = 30,                          // *
    .LensMotorConfig[0].iBacklashValue  = FOCUS_BACKLASH_CORR,
    .LensMotorConfig[0].bSetMove2Stop   = FALSE,
    .LensMotorConfig[0].iInitPosBias    = 0,

    .LensMotorConfig[1].eChnelSel       = LV8044_CHNEL3,               // *
    .LensMotorConfig[1].eMicroStepMode  = LV8044_MODE_4W12PHASE,
    .LensMotorConfig[1].uiPwmOutputPin  = PWMID_1,                     // * PWM1
    .LensMotorConfig[1].eSwTimerID      = 0,
    .LensMotorConfig[1].iDefPositiveDir = 0,                           // *
    .LensMotorConfig[1].uiPwmFreq       = 520,    // must be the same with focus's one
    .LensMotorConfig[1].iWorkStatus     = MOTOR_STATUS_FREE,
    .LensMotorConfig[1].iMotorStatus    = MOTOR_STATUS_FREE,
    .LensMotorConfig[1].iMaxStepSize    = 8,
    .LensMotorConfig[1].bMove2Home      = FALSE,
    .LensMotorConfig[1].iCurrPosition   = WIDE_BOUND,
    .LensMotorConfig[1].bSetFinalStep   = FALSE,
    .LensMotorConfig[1].iMoveDir        = MOVE_DIR_INIT,
    .LensMotorConfig[1].eMoveSpeed      = LV8044_MOTOR_SPD_1X,         // *
    .LensMotorConfig[1].uiPwmBaseNum    = 60,                          // *
    .LensMotorConfig[1].iBacklashValue  = ZOOM_BACKLASH_CORR,
    .LensMotorConfig[1].bSetMove2Stop   = FALSE,
    .LensMotorConfig[1].iInitPosBias    = 0,

                       // zoom_pos  focus_min  focus_max
    .ZoomFocusTbl[0]  = {   0,           0,    0 },
    .ZoomFocusTbl[1]  = { WIDE_BOUND,    0,  120 },        // * 1
    .ZoomFocusTbl[2]  = {  52,         149,  269 },        // * 2
    .ZoomFocusTbl[3]  = { 100,         290,  410 },        // * 3
    .ZoomFocusTbl[4]  = { 160,         440,  560 },        // * 4
    .ZoomFocusTbl[5]  = { 220,         565,  685 },        // * 5
    .ZoomFocusTbl[6]  = { 292,         691,  811 },        // * 6
    .ZoomFocusTbl[7]  = { 360,         783,  903 },        // * 7
    .ZoomFocusTbl[8]  = { 440,         869,  989 },        // * 8
    .ZoomFocusTbl[9]  = { 528,         937, 1057 },        // * 9
    .ZoomFocusTbl[10] = { 612,         985, 1105 },        // * 10
    .ZoomFocusTbl[11] = { TELE_BOUND, 1020, INF_BOUND },   // * 11

                          // 0.05   0.1  0.15   0.2   0.3   0.5   0.7   1.0   1.4   2.0   3.0  infinite (unit: meter)
    .FocusDistanceTbl[0]  = {   60,   60,   60,   60,   60,   60,   61,   63,   65,   67,   69,   70 },
    .FocusDistanceTbl[1]  = {   60,   60,   60,   60,   60,   60,   61,   63,   65,   67,   69,   70 },    // * 1
    .FocusDistanceTbl[2]  = {  189,  189,  189,  189,  189,  189,  190,  192,  194,  196,  198,  199 },    // * 2
    .FocusDistanceTbl[3]  = {  350,  350,  350,  350,  350,  350,  351,  353,  355,  357,  359,  360 },    // * 3
    .FocusDistanceTbl[4]  = {  500,  500,  500,  500,  500,  500,  501,  503,  505,  507,  509,  510 },    // * 4
    .FocusDistanceTbl[5]  = {  625,  625,  625,  625,  625,  625,  626,  628,  630,  632,  634,  635 },    // * 5
    .FocusDistanceTbl[6]  = {  751,  751,  751,  751,  751,  751,  752,  754,  756,  758,  760,  761 },    // * 6
    .FocusDistanceTbl[7]  = {  843,  843,  843,  843,  843,  843,  844,  846,  848,  850,  852,  853 },    // * 7
    .FocusDistanceTbl[8]  = {  929,  929,  929,  929,  929,  929,  930,  932,  934,  936,  938,  939 },    // * 8
    .FocusDistanceTbl[9]  = {  997,  997,  997,  997,  997,  997,  998, 1000, 1002, 1004, 1006, 1007 },    // * 9
    .FocusDistanceTbl[10] = { 1045, 1045, 1045, 1045, 1045, 1045, 1046, 1048, 1050, 1052, 1054, 1055 },    // * 10
    .FocusDistanceTbl[11] = { 1080, 1080, 1080, 1080, 1080, 1080, 1081, 1083, 1085, 1087, 1089, 1090 },    // * 11
};


#endif    // _LENSMOTOR_LV8044_H_


