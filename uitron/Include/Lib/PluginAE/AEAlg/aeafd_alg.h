/**
    Anti Flicker Detection Algorithm.

    @file       aeafd_alg.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AEAFD_ALG_H_
#define _AEAFD_ALG_H_

/**
     @name AEAFD Alg debug option
     @note
*/
//@{
#define AEAFD_FLG_DEBUG_LOG       0x00000001
#define AEAFD_FLG_ADJUST          0x00000002

#define AEAFD_DETECT_CYCLE                            6
//@}

/**
     AEAFDALG status code.

     @note
*/
typedef enum AEAFD_STATUS {
	AEAFD_STATUS_STOP,
	AEAFD_STATUS_READY,
	AEAFD_STATUS_START,
	AEAFD_STATUS_PROCESSING,
	AEAFD_STATUS_NUM
} AEAFD_STATUS;

/**
     AEAFDALG fps index.

     @note
*/
typedef enum {
	AEAFD_FRAME_60FPS = 0,
	AEAFD_FRAME_30FPS,
	AEAFD_FRAME_15FPS,
	AEAFD_FRAME_MAX_NUM,
	ENUM_DUMMY4WORD(AEAFD_FRAME)
} AEAFD_FRAME;

/**
     AEAFDALG flicker index.

     @note
*/
typedef enum AEAFD_FLICKER {
	AEAFD_FLICKER_50Hz,
	AEAFD_FLICKER_60Hz,
	AEAFD_FLICKER_UNKNOW,
	AEAFD_FLICKER_NUM
} AEAFD_FLICKER;



typedef struct _AEAFD_INFO {
	UINT32 VTotal;
	UINT32 HTotal;
	UINT32 CAWinSizeX;
	UINT32 CAWinSizeY;
	UINT32 CAWinStartX;
	UINT32 CAWinStartY;
	UINT32 CABuffer;
	UINT32 CABlkWidth;
	UINT32 CABlkHeight;
	UINT32 DebugFlag;

	UINT32 ScalingFactor;
	UINT32 YLowerLimit;
	UINT32 YUpperLimit;
	UINT32 FOMiThreshold;
	UINT32 FOMsThreshold;
	UINT32 DetectUpperBound;
	UINT32 DetectLowerBound;

} AEAFD_INFO;


/**

     AEAfdAlg status function

     Set aeafd alg status

     @note

     @param[in] AEAFD_STATUS    Current status.
     @return
*/
void AEAFD_SetStatus(AEAFD_STATUS flag);

/**

     AEAfdAlg status function

     Get aeafd alg status

     @note

     @param[in]
     @return
         - @b AEAFD_STATUS    Current status.
*/
AEAFD_STATUS AEAFD_GetStatus(void);

/**

     AEAfdAlg exp&time function

     Get expotime & fps from aeafd alg

     @note

     @param[in] AEAFD_FLICKER current flicker
     @param[out] AfdExpTime, exposure time
     @param[out] AfdFrame, frame rate
     @return
*/
void AEAFD_GetAESetting(AEAFD_FLICKER CurrentFreq, UINT32 *AfdExpTime, AEAFD_FRAME *AfdFrame);

/**

     AEAfdAlg Init

     @note

     @param[in] VTotal, Vtotal length
     @param[in] CABuffer, CA Buffer address
     @param[in] flag, debug option
     @param[in] CAWinNumX, CA window X size
     @param[in] CAWinNumY, CA window Y size
     @param[in] CABlkSize, CA window size
     @return
*/

void AEAFD_Init(AEAFD_INFO *Info);

/**

     AEAfdAlg process

     @note

     @param[in]
     @return
         - @b AEAFD_FLICKER    current flicker index
*/
AEAFD_FLICKER AEAFD_process(void);

/**

     get AEAfdAlg result

     @note

     @param[in]
     @return
         - @b AEAFD_FLICKER    current flicker index
*/
AEAFD_FLICKER AEAFD_GetRslt(void);

/**

     The function to set AEAfdAlg sensitivity

     @note

     @param[in] DetBlockUpper, threshold to change flicker
     @param[in] DetBlockLower, threshold to keep flicker
     @return
*/
void AEAFD_SetDetParameter(UINT32 DetBlockUpper, UINT32 DetBlockLower);
#endif //_AEAFD_ALG_H_
