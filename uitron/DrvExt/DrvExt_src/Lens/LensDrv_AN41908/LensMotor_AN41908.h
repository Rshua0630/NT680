/**
 * Novatek PANASONIC AN41908 motor driver header.
 * 
 * Reference AN41908 motor driver header.
 * 
 * @file LensMotor_AN41908.h
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _LENSMOTOR_AN41908_H_
#define _LENSMOTOR_AN41908_H_

#include "Type.h"
#include "Lens_AN41908.h"


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
            UINT8 A0:1;
            UINT8 A1:1;
            UINT8 A2:1;
            UINT8 A3:1;
            UINT8 A4:1;
            UINT8 A5:1;
        } A;

        UINT8 addr;        // register address
    };

    union {
        struct {
            UINT8 RW:1;    // 0: write / 1: read
            UINT8 C1:1;    // un-used
        } M;

        UINT8 rw;
    };

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
            UINT8 b8:1;
            UINT8 b9:1;
            UINT8 b10:1;
            UINT8 b11:1;
            UINT8 b12:1;
            UINT8 b13:1;
            UINT8 b14:1;
            UINT8 b15:1;
        } B;

        UINT16 data;       // written data
    };
} SPI_DATA_FMT;


//=============================================================================
// motor driver parameters
//=============================================================================
const UINT8 g_ucRegAddressOfst[] = 
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0E, 0x20, 0x21,
    0x22, 0x23, 0x24, 0x25, 0x27, 0x28, 0x29, 0x2A
};


static LENSMOTOR_INFO g_LensMotorInfo =
{
    .uiPinIn = {0},

    .LensMotorReg.REG[0]  = 0x0000,
    .LensMotorReg.REG[1]  = 0x2000,
    .LensMotorReg.REG[2]  = 0x0000,
    .LensMotorReg.REG[3]  = 0x0000,
    .LensMotorReg.REG[4]  = 0x0000,
    .LensMotorReg.REG[5]  = 0x0000,
    .LensMotorReg.REG[6]  = 0x0000,
    .LensMotorReg.REG[7]  = 0x0000,
    .LensMotorReg.REG[8]  = 0x0000,
    .LensMotorReg.REG[9]  = 0x0000,
    .LensMotorReg.REG[10] = 0x0000,
    .LensMotorReg.REG[11] = 0x0480,      // enable IRIS function block (bit10)
    .LensMotorReg.REG[12] = 0x0000,
    .LensMotorReg.REG[13] = 0x0100,
    .LensMotorReg.REG[14] = 0x5503,
    .LensMotorReg.REG[15] = 0x0087,
    .LensMotorReg.REG[16] = 0x1602,
    .LensMotorReg.REG[17] = STANDBY_PEAK_PULSE_WIDTH,
    .LensMotorReg.REG[18] = 0x0000,
    .LensMotorReg.REG[19] = 0x009D,
    .LensMotorReg.REG[20] = 0x1602,
    .LensMotorReg.REG[21] = STANDBY_PEAK_PULSE_WIDTH,
    .LensMotorReg.REG[22] = 0x0000,
    .LensMotorReg.REG[23] = 0x0139,

    .LensMotorConfig[0].eChnelSel       = AN41908_CHNEL1,               // *
    .LensMotorConfig[0].eMicroStepMode  = AN41908_MODE_64MACRO,
    .LensMotorConfig[0].iExciteEnable   = 0x0400,
    .LensMotorConfig[0].iDefPositiveDir = 1,                            // *
    .LensMotorConfig[0].uiPwmFreq       = 4500,    // must be the same with zoom's one
    .LensMotorConfig[0].iWorkStatus     = MOTOR_STATUS_FREE,
    .LensMotorConfig[0].iMotorStatus    = MOTOR_STATUS_FREE,
    .LensMotorConfig[0].iMaxStepSize    = 255/AN41908_MODE_64MACRO,
    .LensMotorConfig[0].bMove2Home      = FALSE,
    .LensMotorConfig[0].iCurrPosition   = NEAR_BOUND,
    .LensMotorConfig[0].bSetFinalStep   = FALSE,
    .LensMotorConfig[0].iMoveDir        = MOVE_DIR_INIT,
    .LensMotorConfig[0].eMoveSpeed      = AN41908_MOTOR_SPD_2X,         // *
    .LensMotorConfig[0].iBacklashValue  = FOCUS_BACKLASH_CORR,
    .LensMotorConfig[0].wMaxPulseWidth  = STANDBY_PEAK_PULSE_WIDTH,
    .LensMotorConfig[0].iDlyTime        = FOCUS_PSUM_DELAY,
    .LensMotorConfig[0].bSetMove2Stop   = FALSE,
    .LensMotorConfig[0].iInitPosBias    = 0,

    .LensMotorConfig[1].eChnelSel       = AN41908_CHNEL3,               // *
    .LensMotorConfig[1].eMicroStepMode  = AN41908_MODE_64MACRO,
    .LensMotorConfig[1].iExciteEnable   = 0x0400,
    .LensMotorConfig[1].iDefPositiveDir = 1,                            // *
    .LensMotorConfig[1].uiPwmFreq       = 4500,    // must be the same with focus's one
    .LensMotorConfig[1].iWorkStatus     = MOTOR_STATUS_FREE,
    .LensMotorConfig[1].iMotorStatus    = MOTOR_STATUS_FREE,
    .LensMotorConfig[1].iMaxStepSize    = 255/AN41908_MODE_64MACRO,
    .LensMotorConfig[1].bMove2Home      = FALSE,
    .LensMotorConfig[1].iCurrPosition   = WIDE_BOUND,
    .LensMotorConfig[1].bSetFinalStep   = FALSE,
    .LensMotorConfig[1].iMoveDir        = MOVE_DIR_INIT,
    .LensMotorConfig[1].eMoveSpeed      = AN41908_MOTOR_SPD_1X,         // *
    .LensMotorConfig[1].iBacklashValue  = ZOOM_BACKLASH_CORR,
    .LensMotorConfig[1].wMaxPulseWidth  = STANDBY_PEAK_PULSE_WIDTH,
    .LensMotorConfig[1].iDlyTime        = ZOOM_PSUM_DELAY,
    .LensMotorConfig[1].bSetMove2Stop   = FALSE,
    .LensMotorConfig[1].iInitPosBias    = 0,

                       // zoom_pos  focus_min  focus_max
    .ZoomFocusTbl[0]  = {   0,           0,    0 },
    .ZoomFocusTbl[1]  = { WIDE_BOUND,    0,  120 },        // * 1
    .ZoomFocusTbl[2]  = {  63,         178,  298 },        // * 2
    .ZoomFocusTbl[3]  = { 142,         353,  473 },        // * 3
    .ZoomFocusTbl[4]  = { 226,         501,  621 },        // * 4
    .ZoomFocusTbl[5]  = { 310,         619,  739 },        // * 5
    .ZoomFocusTbl[6]  = { 405,         725,  845 },        // * 6
    .ZoomFocusTbl[7]  = { 499,         803,  923 },        // * 7
    .ZoomFocusTbl[8]  = { 599,         865,  985 },        // * 8
    .ZoomFocusTbl[9]  = { 699,         908, 1028 },        // * 9
    .ZoomFocusTbl[10] = { 810,         949, 1069 },        // * 10
    .ZoomFocusTbl[11] = { TELE_BOUND,  974, INF_BOUND },   // * 11

                          // 0.05   0.1  0.15   0.2   0.3   0.5   0.7   1.0   1.4   2.0   3.0  infinite (unit: meter)
    .FocusDistanceTbl[0]  = {   0,    0,    7,   13,   18,   21,   23,   25,   27,   29,   30,   33 },
    .FocusDistanceTbl[1]  = {   0,    0,    7,   13,   18,   21,   23,   25,   27,   29,   30,   33 },    // * 1
    .FocusDistanceTbl[2]  = { 120,  120,  127,  133,  138,  141,  143,  145,  147,  149,  150,  153 },    // * 2
    .FocusDistanceTbl[3]  = { 230,  230,  237,  243,  248,  251,  253,  255,  257,  259,  260,  263 },    // * 3
    .FocusDistanceTbl[4]  = { 335,  335,  342,  348,  353,  356,  358,  360,  362,  364,  365,  368 },    // * 4
    .FocusDistanceTbl[5]  = { 435,  435,  442,  448,  453,  456,  458,  460,  462,  464,  465,  468 },    // * 5
    .FocusDistanceTbl[6]  = { 530,  530,  537,  543,  548,  551,  553,  555,  557,  559,  560,  563 },    // * 6
    .FocusDistanceTbl[7]  = { 620,  620,  627,  633,  638,  641,  643,  645,  647,  649,  650,  653 },    // * 7
    .FocusDistanceTbl[8]  = { 705,  705,  712,  718,  723,  726,  728,  730,  732,  734,  735,  738 },    // * 8
    .FocusDistanceTbl[9]  = { 795,  795,  802,  808,  813,  816,  818,  820,  822,  824,  825,  828 },    // * 9
    .FocusDistanceTbl[10] = { 900,  900,  907,  913,  918,  921,  923,  925,  927,  929,  930,  933 },    // * 10
    .FocusDistanceTbl[11] = { 900,  900,  907,  913,  918,  921,  923,  925,  927,  929,  930,  933 },    // * 11
};


// following table is based on 27MHz source clock to AN41908
const UINT32 g_uiPwmFreqTbl[][12] = {{ 27200,  30100,  40200,  54400,  64900,  76700, 108900, 129800, 160700, 210900, 306800, 562500},    // unit: Hz
                                     {0x5F00, 0x5C00, 0x5500, 0x3F00, 0x3A00, 0x3600, 0x1F00, 0x1A00, 0x1500, 0x1000, 0x0B00, 0x0600}};

#define MAX_PWM_TAB_SIZE    sizeof(g_uiPwmFreqTbl)/sizeof(UINT32)/2

const UINT32 g_uiApertaure2PwmDuty[] = {0x00BB, 0x00AA, 0x0099, 0x0088, 0x0077, 0x0066, 0x0055};    // *


#endif    // _LENSMOTOR_AN41908_H_


