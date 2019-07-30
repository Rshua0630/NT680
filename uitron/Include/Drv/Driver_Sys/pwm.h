/**
    @file       pwm.h
    @ingroup    mIDrvIO_PWM

    @brief      Header file for PWM module
                This file is the header file that define the API for PWM module

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

*/
#ifndef _PWM_H
#define _PWM_H

#include "Type.h"


/**
    @addtogroup mIDrvIO_PWM
*/
//@{
/**
    PWM micro step direction

    Micro step direction configuration

    @note for pwm_mstepConfig() pwm_mstepConfigSet()
*/
typedef enum {
	MS_DIR_INCREASE = 0x0,          ///< CounterClockwise
	MS_DIR_DECREASE,                ///< Clockwise
	MS_DIR_CNT,
	ENUM_DUMMY4WORD(PWM_MS_DIR)
} PWM_MS_DIR;


/**
    PWM type (PWM or Micro step)
*/
typedef enum {
	PWM_TYPE_PWM = 0x0,             ///< PWM
	PWM_TYPE_MICROSTEP,             ///< Micro step
	PWM_TYPE_CCNT,                  ///< CCNT
	PWM_TYPE_CNT,
	ENUM_DUMMY4WORD(PWM_TYPE)
} PWM_TYPE;


/**
    PWM clock divid

    @note for pwm_pwmConfigClockDiv()
*/
typedef enum {
	PWM0_3_CLKDIV   = 0x0,          ///< PWM0~PWM3 clock divid
	PWM4_7_CLKDIV,                  ///< PWM4~PWM7 clock divid
	PWM8_11_CLKDIV,                 ///< PWM8~PWM11 clock divid
	PWM12_CLKDIV    = 12,           ///< PWM12 clock divid
	PWM13_CLKDIV,                   ///< PWM13 clock divid
	PWM14_CLKDIV,                   ///< PWM14 clock divid
	PWM15_CLKDIV,                   ///< PWM15 clock divid
	PWM16_CLKDIV,                   ///< PWM16 clock divid
	PWM17_CLKDIV,                   ///< PWM17 clock divid
	PWM18_CLKDIV,                   ///< PWM18 clock divid
	PWM19_CLKDIV,                   ///< PWM19 clock divid
	ENUM_DUMMY4WORD(PWM_CLOCK_DIV)
} PWM_CLOCK_DIV;


/**
    PWM micro step phase type (1-2 or 2-2 phase)

    @note for pwm_mstepConfig() pwm_mstepConfigSet()
*/
typedef enum {
	PWM_MS_1_2_PHASE_TYPE = 0x0,    ///< 1 unit each operation
	PWM_MS_2_2_PHASE_TYPE,          ///< 2 unit each operation
	PWM_MS_PHASE_TYPE_CNT,
	ENUM_DUMMY4WORD(PWM_MS_PHASE_TYPE)
} PWM_MS_PHASE_TYPE;


/**
    PWM Micro step step per phase

    PWM Micro step step per phase configuration

    @note for pwm_mstepConfig() pwm_mstepConfigSet()
*/
typedef enum {
	TOTAL_08_STEP_PER_PHASE = 8,    ///< 8  steps each phase
	TOTAL_16_STEP_PER_PHASE = 16,   ///< 16 steps each phase
	TOTAL_32_STEP_PER_PHASE = 32,   ///< 32 steps each phase
	TOTAL_64_STEP_PER_PHASE = 64,   ///< 64 steps each phase
	ENUM_DUMMY4WORD(PWM_MS_STEP_PER_PHASE)
} PWM_MS_STEP_PER_PHASE;


/**
    PWM Micro step channel level

    PWM Micro step channel level configuation
*/
typedef enum {
	PWM_MS_CHANNEL_LEVEL_LOW = 0x0, ///< PWM MS channel level low
	PWM_MS_CHANNEL_LEVEL_HIGH,      ///< PWM MS channel level high

	ENUM_DUMMY4WORD(PWM_MS_CHANNEL_LEVEL)
} PWM_MS_CHANNEL_LEVEL;



/**
    PWM Micro step channel set

    PWM Micro step channel set definition

    @note for pwm_openSet, pwm_closeSet, pwm_waitSet(), pwm_configClockDivSet(), pwm_mstepConfigSet(),\n
              pwm_pwmEnableSet(), pwm_pwmDisableSet(), pwm_mstepEnableSet(), pwm_mstepDisableSet()
*/
typedef enum {
	PWM_MS_SET_0 = 0x0,             ///< PWM MS channel set 0, including PWM0-3
	PWM_MS_SET_1,                   ///< PWM MS channel set 1, including PWM4-7
	PWM_MS_SET_2,                   ///< PWM MS channel set 2, including PWM8-11
	PWM_MS_SET_3,                   ///< PWM MS channel set 3, including PWM12-15
	PWM_MS_SET_TOTAL,

	ENUM_DUMMY4WORD(PWM_MS_CHANNEL_SET)
} PWM_MS_CHANNEL_SET;

/**
    Micro step target count set(s)

    @note for pwm_mstepConfigTargetCountEnable()
*/
typedef enum {
	PWM_00_03_TGT_CNT   = 0x0,          ///< PWM0~PWM3 target count
	PWM_04_07_TGT_CNT,                  ///< PWM4~PWM7 target count
	PWM_08_11_TGT_CNT,                  ///< PWM8~PWM11 target count

	PWM_TGT_CNT_NUM,

	ENUM_DUMMY4WORD(PWM_TGT_COUNT)
} PWM_TGT_COUNT;

/**
    PWM CCNT count down policy

    @note   for pwm_ccntConfig()
*/
typedef enum {
	PWM_CCNT_COUNT_INCREASE = 0x0,  ///< PWM cycle count increase
	PWM_CCNT_COUNT_DECREASE,        ///< PWM cycle count decrease
	PWM_CCNT_COUNT_POLICY_CNT,
	ENUM_DUMMY4WORD(PWM_CCNT_COUNTDOWN)
} PWM_CCNT_COUNTDOWN;


/**
    PWM CCNT signal source

    @note   for pwm_ccntConfig()
*/
typedef enum {
	PWM_CCNT_SIGNAL_GPIO = 0x0,                 ///< Signal from GPIO (default value)
	PWM_CCNT_SIGNAL_ADC,                        ///< Signal from ADC channel( CCNT0 & 2 from ADC ch1 / CCNT1 & 3 from ADC ch2)
	PWM_CCNT_SIGNAL_CNT,
	ENUM_DUMMY4WORD(PWM_CCNT_SIGNAL_SOURCE)
} PWM_CCNT_SIGNAL_SOURCE;

/**
    PWM CCNT interrupt trigger event

    @note   for pwm_ccntConfig()
*/
typedef enum {
	PWM_CCNT_EDGE_TRIG_INTEN            = 0x1,  ///< CCNT target value arrived interrupt
	PWM_CCNT_TAGT_TRIG_INTEN            = 0x2,  ///< CCNT edge triggerd interrupt\n
	///< (depend on uiCountMode set PWM_CCNT_MODE_PULSE or PWM_CCNT_MODE_EDGE)
	///< @note PWM_CCNT_MODE_PULSE, PWM_CCNT_MODE_EDGE
	PWM_CCNT_TRIG_INTEN_CNT,
	ENUM_DUMMY4WORD(PWM_CCNT_TRIG_INTERRUPT)
} PWM_CCNT_TRIG_INTERRUPT;

/**
    PWM CCNT interrupt trigger event

    @note   for pwm_ccntConfig()
*/
typedef enum {
	PWM_CCNT_EDGE_TRIG                  = 0x1,  ///< CCNT target value arrived trigger
	PWM_CCNT_TAGT_TRIG                  = 0x2,  ///< CCNT edge trigger
	///< @note PWM_CCNT_MODE_PULSE,PWM_CCNT_MODE_EDGE
	PWM_CCNT_TOUT_TRIG                  = 0x4,  ///< CCNT time out trigger
	PWM_CCNT_TRIG_CNT,
	ENUM_DUMMY4WORD(PWM_CCNT_TRIG_TYPE)
} PWM_CCNT_TRIG_TYPE;

/**
    PWM configuration identifier

    @note For pwm_setConfig()
*/
typedef enum {
	PWM_CONFIG_ID_AUTOPINMUX,                   ///< Context can be one of the following:
	///< - @b TRUE  : disable pinmux when pwm driver close
	///< - @b FALSE : not disable pinmux when pwm driver close

	ENUM_DUMMY4WORD(PWM_CONFIG_ID)
} PWM_CONFIG_ID;


/**
    PWM configuration structure

    @note for pwm_set()
*/
typedef struct {
#if 0
	UINT32  uiDiv;                              ///< Clock divider, PWM clock = 120MHZ / (uiDiv + 1), uiDiv = 3 ~ 16383, min value = 3
	///< @note 1. uiDiv MUST >= 3, if uiDiv < 3, driver will force set as 3
	///< \n2. PWM0-3, PWM4-7 and PWM8-11 are use same clock source respectively
	///< \n3. Others(PWM12-19) use dedicate clock source respectively
#endif
	UINT32  uiPrd;                              ///< Base period, how many PWM clock per period, 2 ~ 255
	///< @note uiRise <= uiFall <= uiPrd
	UINT32  uiRise;                             ///< Rising at which clock
	///< @note uiRise <= uiFall <= uiPrd
	UINT32  uiFall;                             ///< Falling at which clock
	///< @note uiRise <= uiFall <= uiPrd
	UINT32  uiOnCycle;                          ///< Output cycle, 0 ~ 65535
	///< - @b PWM_FREE_RUN: Free Run
	///< - @b Others: How many cycles (PWM will stop after output the cycle counts)
	UINT32  uiInv;                              ///< Invert PWM output signal or not
	///< - @b PWM_SIGNAL_NORMAL: Don't invert PWM output signal
	///< - @b PWM_SIGNAL_INVERT: Invert PWM output signal
} PWM_CFG, *PPWM_CFG;


/**
    PWM CCNT configuration structure
*/
typedef struct {
	UINT32  uiStartValue;                       ///< CCNT start count value, 0 ~ 0xFFFF (Starting count value)
	UINT32  uiTriggerValue;                     ///< CCNT target trigger value, 0 ~ 0xFFFF (trigger count value)
	UINT32  uiFilter;                           ///<  Filter glitch of input signal, value is 0 ~ 255. If the glitch width is smaller than (CCNTx_FILTER * 8) + 1 cycles, then the glitch will be filtered out.
	UINT32  uiInv;                              ///< Invert CCNT input signal or not
	///< - @b PWM_CCNT_SIGNAL_NORMAL: Don't invert CCNT input signal
	///< - @b PWM_CCNT_SIGNAL_INVERT: Invert CCNT input signal
	UINT32  uiMode;                             ///< CCNT mode
	///< - @b PWM_CCNT_MODE_PULSE: Counting pulse only
	///< - @b PWM_CCNT_MODE_EDGE: Counting edge, both rising and falling edges
	PWM_CCNT_SIGNAL_SOURCE  uiSigSrc;           ///< CCNT signal source
	///< - @b PWM_CCNT_SIGNAL_GPIO  : Signal from GPIO (default value)
	///< - @b PWM_CCNT_SIGNAL_ADC   : Signal from ADC channel( CCNT0 & 3 from ADC ch1 / CCNT1 from ADC ch2 / CCNT2 from ADC ch6)
	PWM_CCNT_COUNTDOWN      uiCountMode;        ///< CCNT count mode
	///< - @b PWM_CCNT_COUNT_INCREASE: The value of CCNT0_CURRENT_VAL will be increased by one when detecting one pulse or edge( by CCNT0_MODE)
	///< - @b PWM_CCNT_COUNT_DECREASE: The value of CCNT0_CURRENT_VAL will be decreased by one when detecting one pulse or edge( by CCNT0_MODE)
	PWM_CCNT_TRIG_INTERRUPT uiTrigIntEn;        ///< CCNT interrupt issue condition (can occurred mutiple condition at the same time)
	///<   @note PWM_CCNT_TRIG_INTERRUPT
	///< - @b TRUE  : Trigger at each edge(rising or both rasing & faling depend on uiMode configuration
	///< - @b FALSE : Trigger at target value arrived.
} PWM_CCNT_CFG, *PPWM_CCNT_CFG;


/**
    PWM CCNT time out configuration structure

    @note for pwm_ccntEnable()
*/
typedef struct {
	BOOL    ubToutEN;                           ///< CCNT timeout enable
	UINT32  uiToutValue;                        ///< CCNT timeout value
} PWM_CCNT_TOUT_CFG, *PPWM_CCNT_TOUT_CFG;


/**
    MSTP configuration structure

    @note for pwm_mstepConfig()
*/
typedef struct {
	UINT32  uiPh;                               ///< Micro step starting phase, phase 0 ~ 7
	UINT32  uiDir;                              ///< Micro step moving direction ,
	///< - @b MS_DIR_INCREASE
	///< - @b MS_DIR_DECREASE
	UINT32  uiOnCycle;                          ///< Number of phase for Micro step mode
	UINT32  uiStepPerPhase;                     ///< Step per phase
	///< - @b TOTAL_08_STEP_PER_PHASE (8 steps each phase)
	///< - @b TOTAL_16_STEP_PER_PHASE (16 steps each phase)
	///< - @b TOTAL_32_STEP_PER_PHASE (32 steps each phase)
	///< - @b TOTAL_64_STEP_PER_PHASE (64 steps each phase)
	UINT32  uiPhaseType;                        ///< 1-2 or 2-2 phase
	///< - @b PWM_MS_1_2_PHASE_TYPE (1 unit each operation)
	///< - @b PWM_MS_2_2_PHASE_TYPE (2 unit each operation)
	BOOL    uiThresholdEn;                      ///< Enable threshold filter(always set as FALSE), TRUE for test only
	BOOL    uiThreshold;                        ///< Max 0x63
    BOOL    is_square_wave;                  ///< square wave or micro stepping
	///< - @b TRUE (square)
	///< - @b FALSE (micro-stepping)
} MSTP_CFG, *PMSTP_CFG;


/**
    Micro step channels (within a set) phase configuration

    @note for pwm_mstepConfigSet()
*/
typedef struct {
	UINT32  uiCH0_Phase;                       ///< Specific MS set channel 0 start phase
	UINT32  uiCH1_Phase;                       ///< Specific MS set channel 1 start phase
	UINT32  uiCH2_Phase;                       ///< Specific MS set channel 2 start phase
	UINT32  uiCH3_Phase;                       ///< Specific MS set channel 3 start phase

} MS_CH_PHASE_CFG, *PMS_CH_PHASE_CFG;

/**
    Micro step channels (within a set) level configuration

    @note for PWM_MS_CHANNEL_LEVEL_LOW or PWM_MS_CHANNEL_LEVEL_HIGH,
*/
typedef struct {
	UINT32  uiCH0_Level;                       ///< Specific MS channel 0 level
	UINT32  uiCH1_Level;                       ///< Specific MS channel 1 level
	UINT32  uiCH2_Level;                       ///< Specific MS channel 2 level
	UINT32  uiCH3_Level;                       ///< Specific MS channel 3 level

} MS_CH_LEVEL_CFG, *PMS_CH_LEVEL_CFG;


/**
    MSTP configuration structure

    @note for pwm_mstepConfigSet()
*/
typedef struct {
	UINT32  uiDir;                              ///< Micro step moving direction ,
	///< - @b MS_DIR_INCREASE
	///< - @b MS_DIR_DECREASE
	UINT32  uiOnCycle;                          ///< Number of phase for Micro step mode
	UINT32  uiStepPerPhase;                     ///< Step per phase
	///< - @b TOTAL_08_STEP_PER_PHASE (8 steps each phase)
	///< - @b TOTAL_16_STEP_PER_PHASE (16 steps each phase)
	///< - @b TOTAL_32_STEP_PER_PHASE (32 steps each phase)
	///< - @b TOTAL_64_STEP_PER_PHASE (64 steps each phase)
	UINT32  uiPhaseType;                        ///< 1-2 or 2-2 phase
	///< - @b PWM_MS_1_2_PHASE_TYPE (1 unit each operation)
	///< - @b PWM_MS_2_2_PHASE_TYPE (2 unit each operation)
	BOOL    uiThresholdEn;                      ///< Enable threshold filter
	BOOL    uiThreshold;                        ///< Max 0x63
    BOOL    is_square_wave;                  ///< square wave or micro stepping
	///< - @b TRUE (square)
	///< - @b FALSE (micro-stepping)
} MSCOMMON_CFG, *PMSCOMMON_CFG;


/**
     @name PWM cycle type free run configuratopn
     @note pwm_set(), pwm_ms_set()
*/
//@{
#define PWM_FREE_RUN            0               ///< For pwm_set() API, output cycle is free run
//@}

/**
    @name PWM output signal mode

    Invert PWM output signal or not for pwm_set() API
*/
//@{
#define PWM_SIGNAL_NORMAL       0               ///< Don't invert PWM output signal
#define PWM_SIGNAL_INVERT       1               ///< Invert PWM output signal
//@}

/**
    @name CCNT input signal mode

    Invert CCNT input signal or not for pwm_setCCNT() API
*/
//@{
#define PWM_CCNT_SIGNAL_NORMAL  0               ///< Don't invert CCNT input signal
#define PWM_CCNT_SIGNAL_INVERT  1               ///< Invert CCNT input signal
//@}

/**
    @name PWM CCNT mode

    PWM CCNT mode for pwm_ccntConfig() API
*/
//@{
#define PWM_CCNT_MODE_PULSE     0               ///< Counting pulse only
///< Example:
///< @code
///  {
///        ____
///     ___|  |___
///     ==> count only raising edge => result is 1
///  }
///  @endcode
#define PWM_CCNT_MODE_EDGE      1               ///< Counting edge, both rising and falling edges
///< Example:
///< @code
///  {
///        ____
///     ___|  |___
///     ==> count both raising & faling edge => result is 2
///  }
///  @endcode
//@}

/**
    @name PWM ID

    PWM ID for PWM driver API

    @note for pwm_open(), pwm_set(), pwm_setCCNT(), pwm_setCCNTToutEN(), pwm_wait(), pwm_stop(), pwm_en(), pwm_reload(),\n
              pwm_ms_set(), pwm_ms_stop(), pwm_ms_en(), pwm_getCycleNumber(), pwm_ccntGetCurrentVal(), pwm_ccntGetCurrentVal(),\n
              pwm_ccntEnable()
*/
//@{
#define PWMID_0                 0x00000001  ///< PWM ID 0
#define PWMID_1                 0x00000002  ///< PWM ID 1
#define PWMID_2                 0x00000004  ///< PWM ID 2
#define PWMID_3                 0x00000008  ///< PWM ID 3
#define PWMID_4                 0x00000010  ///< PWM ID 4
#define PWMID_5                 0x00000020  ///< PWM ID 5
#define PWMID_6                 0x00000040  ///< PWM ID 6
#define PWMID_7                 0x00000080  ///< PWM ID 7
#define PWMID_8                 0x00000100  ///< PWM ID 8
#define PWMID_9                 0x00000200  ///< PWM ID 9
#define PWMID_10                0x00000400  ///< PWM ID 10
#define PWMID_11                0x00000800  ///< PWM ID 11
#define PWMID_12                0x00001000  ///< PWM ID 12
#define PWMID_13                0x00002000  ///< PWM ID 13
#define PWMID_14                0x00004000  ///< PWM ID 14
#define PWMID_15                0x00008000  ///< PWM ID 15
#define PWMID_16                0x00010000  ///< PWM ID 16
#define PWMID_17                0x00020000  ///< PWM ID 17
#define PWMID_18                0x00040000  ///< PWM ID 18
#define PWMID_19                0x00080000  ///< PWM ID 19

#define PWMID_CCNT0             0x00100000  ///< PWM ID CCNT0
#define PWMID_CCNT1             0x00200000  ///< PWM ID CCNT1
#define PWMID_CCNT2             0x00400000  ///< PWM ID CCNT2
#define PWMID_CCNT3             0x00800000  ///< PWM ID CCNT3
//@}

// PWM Driver API
extern ER       pwm_open(UINT32 uiPWMId);
extern ER       pwm_openSet(PWM_MS_CHANNEL_SET uiMSSet);
extern UINT32   pwm_pwmGetCycleNumber(UINT32 uiPWMId);
extern ER       pwm_close(UINT32 uiPWMId, BOOL bWaitAutoDisableDone);
extern ER       pwm_closeSet(PWM_MS_CHANNEL_SET uiMSSet, BOOL bWaitAutoDisableDone);
extern ER       pwm_wait(UINT32 uiPWMId, PWM_TYPE uiPwmType);
extern ER       pwm_waitSet(PWM_MS_CHANNEL_SET uiMSSet);

extern ER       pwm_pwmReload(UINT32 uiPWMId);
extern ER       pwm_pwmEnable(UINT32 uiPWMId);
extern ER       pwm_pwmDisable(UINT32 uiPWMId);
extern ER       pwm_pwmEnableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern ER       pwm_pwmDisableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern ER       pwm_pwmConfig(UINT32 uiPWMId, PPWM_CFG pPWMCfg);
extern ER       pwm_pwmConfigClockDiv(PWM_CLOCK_DIV uiClkSrc, UINT32 uiDiv);
extern ER       pwm_pwmReloadConfig(UINT32 uiPWMId, INT32 iRise, INT32 iFall, INT32 iBasePeriod);

extern ER       pwm_mstepEnable(UINT32 uiPWMId);
extern ER       pwm_mstepDisable(UINT32 uiPWMId, BOOL bWait);
extern ER       pwm_mstepEnableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern ER       pwm_mstepDisableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern ER       pwm_mstepClockDivReload(PWM_CLOCK_DIV uiPWMClkDiv, BOOL  bWaitDone);
extern ER       pwm_mstepConfig(UINT32 uiPWMId, PMSTP_CFG pMSTPCfg);
extern ER       pwm_mstepTargetCountWaitDone(PWM_TGT_COUNT uiTargetCnt);
extern ER       pwm_mstepClkDivWaitLoadDone(PWM_CLOCK_DIV uiPWMClkDivSrc);
extern ER       pwm_mstepConfigClockDiv(PWM_MS_CHANNEL_SET uiMSSet, UINT32 uiDiv);
extern ER       pwm_mstepConfigTargetCountEnable(PWM_TGT_COUNT uiTargetCntSet, UINT32 uiTargetCnt, BOOL bEnableTargetCnt);
extern ER       pwm_mstepConfigSet(PWM_MS_CHANNEL_SET uiMSSet, PMS_CH_PHASE_CFG pMSSetChPhCfg, PMSCOMMON_CFG pMSCommonCfg);


extern ER       pwm_ccntEnable(UINT32 uiPWMId);
extern ER       pwm_ccntReload(UINT32 uiPWMId);
extern UINT32   pwm_ccntGetCurrentVal(UINT32 uiPWMId);
extern UINT32   pwm_ccntGetStartValue(UINT32 uiPWMId);
extern UINT32   pwm_ccntGetEdgeInterval(UINT32 uiPWMId);
extern ER       pwm_ccntConfig(UINT32 uiPWMId, PPWM_CCNT_CFG pCCNTCfg);
extern ER       pwm_setConfig(PWM_CONFIG_ID ConfigID, UINT32 uiConfig);
extern ER       pwm_ccntConfigTimeoutEnable(UINT32 uiPWMId, PPWM_CCNT_TOUT_CFG pCCNTToutCfg);

//@}

#endif
