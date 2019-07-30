#ifndef _NVTAECAPI_H
#define _NVTAECAPI_H

#include "Type.h"
#include "Audio.h"

extern void NvtAec_InstallID(void) _SECTION(".kercfg_text");
extern BOOL NvtAec_ChkModInstalled(void);

typedef enum {
	NVTAEC_CONFIG_ID_LEAK_ESTIMATE_EN,      ///< Enable/Disable the AEC leak estimate.
	NVTAEC_CONFIG_ID_LEAK_ESTIMATE_VAL,     ///< Initial Condition of the leak estimate. Value range 25 ~ 99.
	NVTAEC_CONFIG_ID_NOISE_CANCEL_LVL,      ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.
	NVTAEC_CONFIG_ID_ECHO_CANCEL_LVL,       ///< Defualt is -50dB. Suggest value range -30 ~ -60. Unit in dB.
	NVTAEC_CONFIG_ID_FILTER_LEN,            ///< Set AEC internal Filter Length. Set to 0 is using default value. Default is 0.
	NVTAEC_CONFIG_ID_FRAME_SIZE,            ///< Set AEC internal Frame Size. Set to 0 is using default value. Default is 0.
	NVTAEC_CONFIG_ID_NOTCH_RADIUS,			///< Set the Notch filter Radius. Value range 0 ~ 1000.
	NVTAEC_CONFIG_ID_PRELOAD_EN,			///< Set preload coefficient enable.
	NVTAEC_CONFIG_ID_RECORD_CH_NO,
	NVTAEC_CONFIG_ID_PLAYBACK_CH_NO,
	NVTAEC_CONFIG_ID_SAMPLERATE,
	ENUM_DUMMY4WORD(NVTAEC_CONFIG_ID)
} NVTAEC_CONFIG_ID;

typedef enum {
	NVTAEC_MEM_ID_INTENAL,
	NVTAEC_MEM_ID_FORE,
	NVTAEC_MEM_ID_BACK,
	ENUM_DUMMY4WORD(NVTAEC_MEM_ID)
} NVTAEC_MEM_ID;

/*
    Open NvtAec library.

    Open NvtAec library.

    @return
     - @b E_OK:     Initial successfully.
     - @b E_OACV:   Open AEC failed.
     - @b E_SYS:    Initial AEC failed.
*/
extern ER NvtAec_Open(void);

/*
    Close NvtAec library.

    Close NvtAec library.

    @return
     - @b E_OK:     Close successfully.
     - @b E_SYS:    AEC is not opened.
*/
extern ER NvtAec_Close(void);

/*
    Start NvtAec.

    Start NvtAec when record task is started.

    @param[in] iSampleRate  audio sample rate.
    @param[in] iMicCH       microphone channel number.
    @param[in] iSpkCH       speaker channel number.

    @return
     - @b E_OK:     Start successfully.
     - @b E_SYS:    Start failed.
*/
extern ER NvtAec_Start(INT32 iSampleRate, INT32 iMicCH, INT32 iSpkCH);

/*
    Stop NvtAec.

    Stop NvtAec when record task is stopped.

    @return
     - @b E_OK:     Stop successfully.
*/
extern ER NvtAec_Stop(void);

/*
    Run AEC.

    Run AEC.

    @param[in] addrRec      recorded audio buffer address.
    @param[in] addrPlay     played audio buffer address.
    @param[in] addrOut      out audio buffer address.
    @param[in] size         recorded audio buffer size.

    @return
     - @b E_OK:     AEC done successfully.
     - @b E_SYS:    AEC done failed.
*/
extern ER NvtAec_Apply(UINT32 addrRec, UINT32 addrPlay, UINT32 addrOut, UINT32 size);

/*
    Set NvtAec configuration.

    Set NvtAec configuration.

    @param[in]  NvtAecSel       Configuration type.
    @param[in]  iNvtAecConfig   Configuration value. (According to the confiuration type)
*/
extern void NvtAec_SetConfig(NVTAEC_CONFIG_ID NvtAecSel, INT32 iNvtAecConfig);

/*
    Get NvtAec configuration.

    Get NvtAec configuration.

    @param[in]  NvtAecSel       Configuration type.
*/
extern INT32 NvtAec_GetConfig(NVTAEC_CONFIG_ID NvtAecSel);

/*
    Enable NvtAec processing.

    Enable NvtAec processing.

    @param[in]  bEn       Eable configuration. (TRUE: enable, FALSE: disable)
*/
extern void NvtAec_Enable(BOOL bEn);

/*
    Get NvtAec required memory size.

    Get NvtAec required memory size.

    @param[in]  BufID       Buffer ID.

    Example:
    @code
    {
        NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_INTENAL);
        NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_FORE);
        NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_BACK);
    }
    @endcode
*/
extern UINT32 NvtAec_GetRequireBufSize(NVTAEC_MEM_ID BufID);

/*
    Set NvtAec required memory buffer.

    Set NvtAec required memory buffer.

    @param[in]  BufID       Buffer ID.
    @param[in]  addr        Buffer address.
    @param[in]  size        Buffer size.

*/
extern void NvtAec_SetRequireBuffer(NVTAEC_MEM_ID id, UINT32 addr, UINT32 size);


#endif
