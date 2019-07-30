/**
    Audio Filtering library Driver header file.

    This file is the driver of Audio Filtering library.

    @file       audFilt.h
    @ingroup    mIAudFilt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _AUDFILT_H
#define _AUDFILT_H

#define IM_DBG  ENABLE
#if 0//!IM_DBG
// For Reference only
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
typedef unsigned long           UINT32;     // Unsigned 32 bits data type
typedef signed long             INT32;      // Signed 32 bits data type
typedef unsigned int            BOOL;
/*
#ifndef TRUE
#define TRUE (0 == 0)
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif
*/
#else
#include "Type.h"
#endif
#include "Audio.h"


/**
    @addtogroup mIAudFilt
*/
//@{

/**
    Audio Filter Maximum Section Number

    This value multiplied by 2 would also become the maximum filter orders.
*/
//@{
#define AUDFILT_MAX_SECNO  5
//@}

/**
    The Audio Filter Channel Number Select

    This field is used in the structure AUDFILT_INIT in AudFilt_Open() API.
*/
typedef enum {
	AUDFILT_CH_MONO   = 1,  ///< Audio Filter uses MONO in/out
	AUDFILT_CH_STEREO,      ///< Audio Filter uses Stereo in/out

	ENUM_DUMMY4WORD(AUDFILT_CHANNEL)
} AUDFILT_CHANNEL;


/**
    The sturcture for Audio Filter initialization

    This field is used in APIs: AudFilt_Open().
*/
typedef struct {
	AUDFILT_CHANNEL     FiltCH;         ///< The Number of Audio channel.
	BOOL                bSmoothEn;      ///< Enable/Disable for the Smooth function during the transition of filter Enable/Disable.

} AUDFILT_INIT, *PAUDFILT_INIT;


/**
    Audio Filter Selection Identification.

    Audio Filter Selection Identification.
    This field is used in APIs: AudFilt_SetConfig() / AudFilt_EnableFilt().
*/
typedef enum {
	AUDFILT_SEL_FILT0,                      ///< 1st IIR Filter Selection ID
	AUDFILT_SEL_FILT1,                      ///< 2nd IIR Filter Selection ID
	AUDFILT_SEL_FILT2,                      ///< 3rd IIR Filter Selection ID
	AUDFILT_SEL_FILT3,                      ///< 4th IIR Filter Selection ID
	AUDFILT_SEL_FILT4,                      ///< 5th IIR Filter Selection ID
	AUDFILT_SEL_FILT5,                      ///< 6th IIR Filter Selection ID
	AUDFILT_SEL_FILT6,                      ///< 7th IIR Filter Selection ID
	AUDFILT_SEL_FILT7,                      ///< 8th IIR Filter Selection ID

	AUDFILT_SEL_TOTAL,                      // Total Number of the filter Selection

	AUDFILT_SEL_NOTCH1 = AUDFILT_SEL_FILT0, ///< 1st Notch Filter Selection ID
	AUDFILT_SEL_NOTCH2,                     ///< 2nd Notch Filter Selection ID
	AUDFILT_SEL_NOTCH3,                     ///< 3rd Notch Filter Selection ID
	AUDFILT_SEL_NOTCH4,                     ///< 4th Notch Filter Selection ID
	AUDFILT_SEL_NOTCH5,                     ///< 5th Notch Filter Selection ID
	AUDFILT_SEL_HIGHPASS1,                  ///< 1st Highpass Filter Selection ID
	AUDFILT_SEL_BANDSTOP1,                  ///< 1st BandStop Filter Selection ID
	AUDFILT_SEL_LOWPASS1,                   ///< 1st Lowpass Filter Selection ID

	ENUM_DUMMY4WORD(AUDFILT_SELECTION)
} AUDFILT_SELECTION;

/**
    Audio Filter Coefficient Remap

    These definitions are used in AudFilt_SetConfig() to remap the filer coefficients into designed format.
*/
//@{
#define AUDFILT_ORDER(x)    (((x)+1)&0xE)                             ///< The filter order can only use 2/4/6/8/10 in current design.
#define AUDFILT_GAIN(x)     ((INT32)((FLOAT)((FLOAT)(x))*65536.0))    ///< Remap floating point input coefficients into fixed point value. The input value allowed range is from -32768.0 to 32767.999999.(Q15.16)
#define AUDFILT_COEF(x)     ((INT32)((FLOAT)((FLOAT)(x))*134217728.0))///< Remap floating point input coefficients into fixed point value. The input value allowed range is from -16.0 to 15.999999.(Q4.27)
//@}

/**
    Audio Filter Section Coefficients Select

    These definitions are used in AudFilt_SetConfig() to select the IIR coefficients B0/B1/B2/A0/A1/A2 for member piFiltCoef.
*/
typedef enum {
	AUDFILT_COEFSEL_B0,     ///< IIR Section Coefficients B0
	AUDFILT_COEFSEL_B1,     ///< IIR Section Coefficients B1
	AUDFILT_COEFSEL_B2,     ///< IIR Section Coefficients B2
	AUDFILT_COEFSEL_A0,     ///< IIR Section Coefficients A0
	AUDFILT_COEFSEL_A1,     ///< IIR Section Coefficients A1
	AUDFILT_COEFSEL_A2,     ///< IIR Section Coefficients A2

	AUDFILT_COEFSEL_TOTAL,  ///< Coefficient Total Number of the Filter Section
	ENUM_DUMMY4WORD(AUDFILT_COEFSEL)
} AUDFILT_COEFSEL;


/**
    Structure for Filter configurations

    This Structure is used for Filter configurations, such as filter-order and Filter-Coeficients.
    The filter order can support for 2/ 4/ 6/ 8/ 10 only in current implementation.
*/
typedef struct {
	UINT32      uiFiltOrder;                        ///< Available Filter Order in current Design is 2/ 4/ 6/ 8/ 10.
	INT32       iTotalGain;                         ///< This is the section output total-Gain and Format is "Q15.16".

	INT32       piFiltCoef[AUDFILT_MAX_SECNO][AUDFILT_COEFSEL_TOTAL];   ///< One Section is 2 order of the IIR Filter.
													///< The 6 Parameter in order are b0,b1,b2,a0,a1,a2 in filter design.
													///< Each Parameter's format is "Q4.27".
	INT32       piSectionGain[AUDFILT_MAX_SECNO];   ///< The Section Input Gain Format is "Q15.16".
} AUDFILT_CONFIG, *PAUDFILT_CONFIG;

/**
    Structure for audio filtering input BitStream information

    Structure for audio filtering input BitStream information, such as BS length and BS buffer address.
*/
typedef struct {
	UINT32      uiBsAddrIn;     ///< The Audio input BitStream buffer Address.
	UINT32      uiBsAddrOut;    ///< The Audio output BitStream buffer Address.

	UINT32      uiBsLength;     ///< The Audio input BitStream Length in Byte count.
								///< This size must be multiples of 256 Samples.(MONO:512Bytes.  STEREO:1024Bytes)
} AUDFILT_BITSTREAM, *PAUDFILT_BITSTREAM;



//
//	EQ related definitions
//

/**
	EQ Band Design Selection ID

	This EQ band selection is used at AudFilt_DesignEQ()
	to select to design EQ band filter.
*/
typedef enum
{
	AUDFILT_EQBAND_01,	///< EQ band 01
	AUDFILT_EQBAND_02,	///< EQ band 02
	AUDFILT_EQBAND_03,	///< EQ band 03
	AUDFILT_EQBAND_04,	///< EQ band 04
	AUDFILT_EQBAND_05,	///< EQ band 05
	AUDFILT_EQBAND_06,	///< EQ band 06
	AUDFILT_EQBAND_07,	///< EQ band 07
	AUDFILT_EQBAND_08,	///< EQ band 08
	AUDFILT_EQBAND_09,	///< EQ band 09
	AUDFILT_EQBAND_10,	///< EQ band 10
	AUDFILT_EQBAND_11,	///< EQ band 11
	AUDFILT_EQBAND_12,	///< EQ band 12

	AUDFILT_EQBAND_MAX, // Max bands number
	ENUM_DUMMY4WORD(AUDFILT_EQBAND)
}AUDFILT_EQBAND;


/**
	Filter Design Type

	This is used at AUDFILT_EQPARAM to specify the filter design type.
*/
typedef enum {
	AUDFILT_DESIGNTYPE_BYPASS,      ///< Bypass type.(Bypass section band)

	AUDFILT_DESIGNTYPE_LOWPASS,     ///< Low Pass Filter,   Gain Value range: -60 ~ 12 dB
	AUDFILT_DESIGNTYPE_HIGHPASS,    ///< High Pass Filter,  Gain Value range: -60 ~ 12 dB
	AUDFILT_DESIGNTYPE_PEAKING,     ///< Peaking Filter,    Gain Value range:   0 ~ 12 dB
	AUDFILT_DESIGNTYPE_HIGHSHELF,   ///< High Shelf Filter, Gain Value range: -60 ~ 12 dB
	AUDFILT_DESIGNTYPE_LOWSHELF,    ///< Low Shelf Filter,  Gain Value range: -60 ~ 12 dB
	AUDFILT_DESIGNTYPE_NOTCH,       ///< Notching Filter,   Gain Value range: -60 ~  0 dB

	AUDFILT_DESIGNTYPE_NUM, 		// Total Number of the filter type
	ENUM_DUMMY4WORD(AUDFILT_DESIGNTYPE)
} AUDFILT_DESIGNTYPE;


/**
	Audio filter design parameter structure

	This structure is used at AudFilt_DesignEQ() and AudFilt_DesignFilter()
	to specify the filter target design settings.
*/
typedef struct {
	AUDFILT_DESIGNTYPE  FiltType;		///< Filter Design Type
	AUDIO_SR            SampleRate;		///< Audio Sample rate
	FLOAT               fGainDB;        ///< Accoding to FiltType: Gain Value range [-60,12] dB.
										///< This setting is useless for Lowpass/Highpass filter type.
	FLOAT               fFrequency;     ///< Center Frequency for filter design.
	FLOAT               Q;              ///< Q value for filter design.
   	  								 	///< [0.03125,4] for LowPass and HighPass.
										///< [0.03125,10] for other Filter Type
} AUDFILT_EQPARAM, *PAUDFILT_EQPARAM;


/**
	Audio Filter Design Control

	This is used at AudFilt_DesignFilter() to control filter output order.
*/
typedef enum {
	AUDFILT_DESGIN_CTRL_DEFAULT = 1,	///< Designed Filter order would be 2.
	AUDFILT_DESGIN_CTRL_2X		= 2,	///< Designed Filter order would be 4.

	AUDFILT_DESGIN_CTRL_MAX		= 5,
	ENUM_DUMMY4WORD(AUDFILT_DESGIN_CTRL)
}AUDFILT_DESGIN_CTRL;



//
//  Export APIs
//

/**
    Open the audio filter library.

    This API must be called to initialize the audio filter basic settings.

    @param[in] pFiltInit    The audio filter initial parameters. Please refer to structure AUDFILT_INIT for reference.
    @return
     - @b TRUE:  Open Done and success.
     - @b FALSE: Open Operation fail.
*/
extern BOOL     AudFilt_Open(PAUDFILT_INIT pFiltInit);

/**
    Check if the audio filter library is opened or not.

    Check if the audio filter library is opened or not.

    @return
     - @b TRUE:  Already Opened.
     - @b FALSE: Have not opened.
*/
extern BOOL     AudFilt_IsOpened(void);

/**
    Close the audio filter library.

    Close the audio filter library.
    After closing the audio filter library, any accessing for the audio filter APIs are forbidden except AudFilt_Open().

    @return
     - @b TRUE:  Close Done and success.
     - @b FALSE: Close Operation fail.
*/
extern BOOL     AudFilt_Close(void);

/**
    Initialize the audio filtering before recording start

    This API would reset the filtering internal temp buffer.
    The user must call this before any new filtering BitStream.

    @return
     - @b TRUE:  Done and success.
     - @b FALSE: Operation fail.
*/
extern BOOL     AudFilt_Init(void);

/**
    Set specified audio filter configuration.

    This API is used to set the specified audio filter configuration
    such as Filter-Order / Filter-Coeficients.
    The basic implementaion of the filter is the IIR section, so the filter parameters for one section is 6 entries.
    The format is like H(z) = (Section-Gain) x (B0 + B1*Z1 + B2*Z2) / (A0 + A1*Z1 + A2*Z2).
    The six entries are (B0,B1,B2,A0,A1,A2) in order. And the format for the six entries are Q4.27, which means the allowed
    floating point coefficient value range is from -16.0 to 15.99999.
    The format for the Section input Gain and the Total output Gain is Q15.16, which means the allowed
    floating point coefficient value range is from -32768.0 to 32767.99999.

    @param[in] FiltSel      Select which of the filter is selected for configuring.
    @param[in] pFiltConfig  The configurations of the specified audio filter.

    @return
     - @b TRUE:  setConfig Done and success.
     - @b FALSE: setConfig Operation fail.
*/
extern BOOL     AudFilt_SetConfig(AUDFILT_SELECTION FiltSel, PAUDFILT_CONFIG pFiltConfig);

/**
    Set specified audio filter enable/disable.

    Set specified audio filter enable/disable. If the smooth function is enabled, the audio sample would be
    fade-in/out in the next audio 256 samples after the transition of the filter enable/disable.

    @param[in] FiltSel      Select which of the filter is selected for configuring.
    @param[in] bEnable
     - @b TRUE:  Enable the specified auido filter.
     - @b FALSE: Disable the specified auido filter.

    @return
     - @b TRUE:  setEnable Done and success.
     - @b FALSE: setEnable Operation fail.
*/
extern BOOL     AudFilt_EnableFilt(AUDFILT_SELECTION FiltSel, BOOL bEnable);

/**
	Audio Filtering Parameters Design

	Design Filter paramters. This API supports HighPass/Lowpass/Peaking/HighShelf/LowShelf/Notch filter type design.
	The user needs only input the sample-rate/filter-type/filter-gain/Q/frequency to retrive the filter paramter settings.
	The default designed filter order is fixed order 2, and using DesignCtrl can making designed order become 4.

	@param[in] 	pEqParam		Target design filter settings.
	@param[out] pFiltConfig		Designed Output filter settings for AudFilt_SetConfig() usages.
	@param[in]  DesignCtrl		Design control. This can control the designed filter order and making filter response more sharped.

	@return
     - @b TRUE:  DesignFilter Done and success.
     - @b FALSE: DesignFilter Operation fail.
*/
extern BOOL 	AudFilt_DesignFilter(PAUDFILT_EQPARAM pEqParam, PAUDFILT_CONFIG pFiltConfig, AUDFILT_DESGIN_CTRL DesignCtrl);

/**
    Apply audio filtering to the input audio bit stream

    Apply specified audio filtering to the input audio bit stream.
    If the smooth function is enabled and one of the audio filter is enabled/disabled, the first 256 samples
    in the audio bit stream would be fade in/out to the incoming audio stream.
    In the current implementation, the input bit stream length must be multiple of 512 Bytes for MONO channel
    and multiple of 1024 Bytes for Stereo channel.
    The input BitStream format support for PCM-16Bits per sample only.

    @param[in] pAudBitStream    The input BitStream information.

    @return
     - @b TRUE:  applyFiltering Done and success.
     - @b FALSE: applyFiltering Operation fail.
*/
extern BOOL     AudFilt_ApplyFiltering(PAUDFILT_BITSTREAM  pAudBitStream);

/**
	ENABLE/DISABLE Audio EQ

	ENABLE/DISABLE Audio EQ with specified band number during AudFilt_ApplyFiltering().
	The EQ band design configurations are using AudFilt_DesignEQ() API to implement.

	@param[in] bEnable		EQ function ENABLE/DISABLE.
	@param[in] uiBandNum	EQ band numbers. Valid value is 1~12.

	@return void
*/
extern void     AudFilt_EnableEQ(BOOL bEnable, UINT32 uiBandNum);

/**
	Audio EQ Filter Design

	Design EQ Filter paramters. This API supports HighPass/Lowpass/Peaking/HighShelf/LowShelf/Notch filter type design.
	The user needs only input the sample-rate/filter-type/filter-gain/Q/frequency to retrive the filter paramter settings.

	@param[in]  BandIndex		Designed EQ band index.
	@param[in] 	pEqParam		Target EQ band filter settings.

	@return
     - @b TRUE:  DesignEQ band Done and success.
     - @b FALSE: DesignEQ band Operation fail.
*/
extern BOOL     AudFilt_DesignEQ(AUDFILT_EQBAND BandIndex, PAUDFILT_EQPARAM pEqParam);

//@}

#endif
