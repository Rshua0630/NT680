#ifndef _NVTIVS_H
#define _NVTIVS_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtivs.h

    @version

    @date
*/
#include <stdio.h>
#include <nvt_type.h>
#include <nvtcfg_define.h>
#ifdef __cplusplus
extern "C" {
#endif


#define MOTION_SHM_KEY 9991
#define TAMPER_SHM_KEY 9992

/* Motion Vector */
// Define in uItron dis_lib.h
#define BLKHMAX                     4
#define BLKVMAX                     32
#define MDSNUMMAX                   12
#define MVNUMMAX                    (BLKHMAX*BLKVMAX*MDSNUMMAX)

typedef struct {
	UINT32 uiMaxFaceNum;
	UINT32 uiFaceScale;
	UINT32 uiProcessRate;
} IVSFDSetting;

typedef struct {
	UINT32 uiFGPercentage;		/**< Foreground area threshold in tampering detection.
								 *   50 means that tampering detection timer is start if foreground is over 50%.
								 *   Range: 1~100 (unit is percentage)
								 */
	UINT32 uiFGDuration;		/**< Foreground keep time threshold in tampering detection.
								 *   30 means tampering is detected if foreground area is over uiFGPercentage and keep over 30 video detected frames.
								 *   Range: 0~65535 (unit is detected frame)
								 */
	UINT32 uiFGLifeTime;		/**< Foreground life time threshold. 50 means one part of foreground will be set as background forcibly
								 *   if it keeps over 50 video detected frames
								 *   Range: 1~65535 (unit is processed frame)
								 *   (uiFGLifeTime should be larger than uiFGDuration, or tampering will never be detected)
								 */
	UINT32 uiBGUpdateSpeed;		/**< Background update speed. The larger value means the faster speed. Range: 0~2
								 */
	UINT32 uiProcessRate;		/**< Process rate of tampering detection, 10 means executing tampering detection every 10 frames.
								 *   Range: 1~(maximum frames rate)
								 *   (Recommended minimum value is 10 because of CPU performance)
								 */
	UINT32 uiFGThreshold;		/**< Threshold of FG detection, the littler value means the litter difference will be detected.
								 *   Range: 25~200
								 */
} IVSTDSetting;

typedef struct {
	UINT16 uiCoordinateX;
	UINT16 uiCoordinateY;
	UINT16 uiWidth;
	UINT16 uiHeight;
} IVSIAObjData;

typedef struct {
	UINT32 uiFaceNum;
	IVSIAObjData *psFDObjData;
	UINT32 uiTimeStamp;
} IVSFDData;

typedef struct {
	UINT32 uiMaxFaceNum;        ///< Maximum number of tracking faces. Range: 1~36
	UINT32 uiFaceScale;         ///< The ratio of minimum face size to maximum image size. If the value is 15, face size is 80*80. Range: 0~15
	UINT32 uiProcessRate;       ///< Process rate of face tracking, 5 means executing face tracking every 5 frames. Range: 1~(max frames rate) (Recommended minimum value is 3 because of CPU performance)
	UINT32 uiMinFaceScore;      ///< The face score filter. If the face score is under the value, the face info will not be shown in NvtCodec_GetFTGData. The recommend value is  33554432Range: 0 ~ UINT32
	UINT32 uiDrawRate;          ///< How many frame to update the rectangle line with face location in streaming. Range: 0~ UINT32 , 0 means disable draw function
	UINT32 uiAllInfo;           ///< Contol the NvtCodec_GetFTGData function information. 0: only show location information. 1: location information and clarity value. 2: all information. Performance: 0 > 1 > 2
	UINT32 uiSensitivity;       ///< The face tracking sensitivity. 1: high face hit rate. 2: low face hit rate. 3: very high face hit rate, can detect glass and hat. precision: 2 > 1 > 3
} IVSFTGSetting;

typedef struct {
	UINT16 uiCoordinateX;       ///< X coordinate of the top-left point of the detected face
	UINT16 uiCoordinateY;       ///< Y coordinate of the top-left point of the detected face
	UINT16 uiWidth;             ///< Width of the detected face
	UINT16 uiHeight;            ///< Height of the detected face
	UINT32 id;                  ///< The face id which be detected
	UINT16 type;                ///< result type: 1--first appearance, other--not first.
	UINT16 clarity;             ///< result clarity
	UINT16 luma;                ///< result luma
	INT32  angle_horizontal;    ///< result face horizontal angle
	INT32  angle_vertical;      ///< result face vertical angle
	INT32  angle_roll;          ///< result face roll angle
	INT32  score;               ///< result face quality reference score
} IVSFTGObjData;


typedef struct {
	UINT32 uiFaceNum;               ///< Number of detected face
	IVSFTGObjData *psFTGObjData;    ///< Pointer to object data of tracking face
	UINT32 uiTimeStamp;             ///< Time stamp of tracking face (This parameter is not supported.)
} IVSFTGData;

// follow with uITRON
#define FTG_OVERRALL_MAX_BUF_SIZE      250*1024
#define FTG_FACE_MAX_BUF_SIZE      100*1024
#define FTG_MAX_FACE_NUM      3

typedef struct {
	UINT32 score;                   ///< The face score in this picture
	char overrall_pic[FTG_OVERRALL_MAX_BUF_SIZE];   ///< The actually overall picture memory address
	UINT32 overrall_size;           ///< The overall picture size
	char face_pic[FTG_FACE_MAX_BUF_SIZE];           ///< The actually face picture memory address
	UINT32 face_size;               ///< The face picture size
} IVSFTG_PIC_DATA;

typedef struct {
	UINT32 id;                      ///< The face id
	UINT32 pic_num;                 ///< The number of pictures which are saved. The max number is 3
	IVSFTG_PIC_DATA pic_list[FTG_MAX_FACE_NUM]; ///< Picture list. Include max 3 overall pictures and 3 face pictures
} IVSFTG_SNAPSHOT;

typedef struct {
	BOOL bGlobalMotionEnable;       ///< Global motion calculation enable. If don’t need DIS(digital image stabilization), please set this item with NVT_FALSE to save CPU usage.
	BOOL bROIMotionEnable;          ///< ROI motion calculation enable
	UINT8 uiROIStartX;              ///< Start X coordinate of ROI. Range: 0~100 (Unit: percentage)
	UINT8 uiROIStartY;              ///< Start Y coordinate of ROI. Range: 0~100 (Unit: percentage)
	UINT8 uiROISizeX;               ///< Width of ROI. Range: 0~100 (Unit: percentage)
	UINT8 uiROISizeY;               ///< Height of ROI. Range: 0~100 (Unit: percentage)
	UINT32 uiInputSizeH;            ///< Input image horizontal pixel number. This parameter is read only.
	UINT32 uiInputSizeV;            ///< Input image vertical pixel number. This parameter is read only.
} IVSMVSetting;

typedef struct {
	INT32   iX;                     ///< X component of motion vector. Range: -32 ~ 31
	INT32   iY;                     ///< Y component of motion vector. Range: -32 ~ 31
	UINT32  uiSad;                  ///< sum of absolute difference (The score of the motion vector block. Basically, a higher score represents better confidence, but it’s not absolutely relative.)
	UINT32  uiCnt;                  ///< The number of edge pixels in the block. Maximal value is 64x48=3072. It can be used to decide if the block has enough features to calculate reliable motion.
	UINT32  uiIdx;                  ///< Block HW ID, not in use. It’s for HW debug.
	BOOL    bValid;                 ///< bValid is to indicate if block motion vector is reliable. If a block has too many features(>70%) or too less features(<10%), the value is set to NVT_FALSE as it’s not reliable.
} IVSMVInfor;

typedef struct {
	IVSMVInfor sIVSMVROIResult[MVNUMMAX];   ///< ROI motion vector result data
	UINT32 uiNumH;                          ///< The horizontal number of motion vector block
	UINT32 uiNumV;                          ///< The vertical number of motion vector block
} IVSMVData;

/* Background Construction */
// Define in uItron BC_lib.h
#define BC_MAX_OBJ_NUM           64
#define BC_MAX_BLK_NUM_H         32
#define BC_MAX_BLK_NUM_W         32
#define BC_MAX_BLK_NUM           (BC_MAX_BLK_NUM_H * BC_MAX_BLK_NUM_W)

// Define in uItron GxImage.h
#define MAX_PLANE_NUM            3            ///<  3 if for YUV planner

typedef struct {
	UINT32 uiMinObjectAreaPCT;      ///< Minimum object area percentage of the whole image. Range: 0~10000, default value is 16
	UINT32 uiMaxFGFrame;            ///< Maximum foreground life time, foreground will be update to background after uiMaxFGFrame frames. Range: 0~32767, default value is 300
	BOOL   bNHUpdateEnable;         ///< Neighborhood update enable. NVT_TRUE means that background update will also handle neighbor area. It can improve accuracy, but foreground will be updated to background gradually. Default value is NVT_TRUE
	UINT32 uiBGUpdateSpeed;         ///< Background update speed. 0 is slow, 1 is middle and 2 is fast. Default value is 1
	UINT32 uiBLKNumW;               ///< Block number in horizontal direction. Range: 1~32, default value is 32
	UINT32 uiBLKNumH;               ///< Block number in vertical direction. Range: 1~32, default value is 32
	UINT32 uiMinObjectBLKNum;       ///< Minimum block number of the detected object. Range: 0~32, default value is 1
	UINT32 uiFGPerBlkPCT;           ///< Foreground threshold. If the percentage of foreground pixels in one block is larger than uiFGPerBlkPCT %, this block is considered as foreground. Range: 0~100, default value is 50
	UINT32 uiSensitivity;           ///< Foreground sensitivity. The smaller value means the more sensitive detection. Range: 25~200, default value is 100
} IVSBCSetting;

typedef struct {
	UINT32 uiCoordinateX;           ///< X coordinate of the top-left point of the foreground object
	UINT32 uiCoordinateY;           ///< Y coordinate of the top-left point of the foreground object
	UINT32 uiWidth;                 ///< Width of the foreground object
	UINT32 uiHeight;                ///< Height of the foreground object
} IVSObjData;

typedef struct {
	BOOL       bUpdated;            ///< Data updated flag, NVT_TRUE means this data is new.
	UINT32     uiObjectNum;         ///< Number of foreground objects
	IVSObjData sObjectData[BC_MAX_OBJ_NUM]; ///< Information of foreground object
} IVSBCData;

typedef struct {
	BOOL       bUpdated;            ///< Data updated flag, NVT_TRUE means this data is new.
	UINT32     uiObjectNum;         ///< Number of foreground objects
	IVSObjData sObjectData[BC_MAX_BLK_NUM]; ///< Information of foreground object
} IVSBCBLKData;

typedef enum {
	IVS_PIXEL_FMT_YUV422_PLANAR  = 0x00,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h), and V(w/2,h)
	IVS_PIXEL_FMT_YUV420_PLANAR  = 0x01,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h/2), and V(w/2,h/2)
	IVS_PIXEL_FMT_YUV422_PACKED  = 0x02,    ///< 2 color plane, pixel=Y(w,h), UV(w,h)
	IVS_PIXEL_FMT_YUV420_PACKED  = 0x03,    ///< 2 color plane, pixel=Y(w,h), UV(w,h/2)
	IVS_PIXEL_FMT_Y_ONLY         = 0x04,    ///< 1 color plane, pixel=Y(w,h),
	IVS_PIXEL_FMT_ARGB565        = 0x05,    ///< 2 color plane, pixel=A8(w,h),RGB565(2w,h)
	IVS_PIXEL_FMT_RGB888_PLANAR  = 0x06,    ///< 3 color plane, pixel=R(w,h), G(w,h), B(w,h)
	IVS_PIXEL_FMT_ARGB888_PACKED = 0x07,    ///< 1 color plane, pixel=ARGB(4w,h)
	IVS_PIXEL_FMT_MAX,
} IVSPixelFormat;

typedef struct {
	UINT32              uiWidth;                     ///< Width of image buffer
	UINT32              uiHeight;                    ///< Height of image buffer
	IVSPixelFormat      ePxlFmt;                     ///< Pixel format
	UINT32              uiPxlAddr[MAX_PLANE_NUM];    ///< Image data address
	UINT32              uiLineOffset[MAX_PLANE_NUM]; ///< Pixel number of one line, this value usually is equal to uiWidth, but sometimes is larger than uiWidth due to hardware limitation.
} IVSImageBuf;


typedef struct {
	UINT32              uiAddr;                    ///< buffer address
	UINT32              uiSize;                    ///< buffer size
} IVSBuf;

// motion detection
/**
    Register callback function, the function will call when motion status is changed

    @param[in] pfNotifyCB   It is callback function, the function intput parameter is motion status, 1=trigger, 0=no trigger

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_RegisterNotifyMotion(INT32(*pfNotifyCB)(INT8));

/**
    Unregister callback function

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_UnregisterNotifyMotion(void);

/**
    Get motion detection status

    @param[out] iMotionStatus   Motion status, 1=trigger, 0 =no trigger

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetMotionStatus(INT8 *iMotionStatus);

/**
    Set motion level and motion window

    @param[in] iID      The motion window id, now only support id=0
    @param[in] cLevel   Motion sensitive value=High,9,8,7,6,5,4,3,2,1,Low
    @param[in] cWnd     Motion window, the window grids are 12x16.                                  \n
                        This value is component of 12 values, and every value is 0~65535            \n
                        Ex:65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535  \n
                        Each value is control the one row grids, each bit of value control a grid.  \n
                        Ex:                                                                         \n
                        Wnd=1,2,4,8,16,32,64,128,256,512,1024,2048  \n
                        Means the motion window is like             \n
                        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0             \n
                        0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0             \n
                        0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0             \n
                        0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0             \n
                        0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0             \n
                        0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0             \n
                        0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0             \n
                        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0             \n
                        0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0             \n
                        0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0             \n
                        0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0             \n
                        0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_SetMotionCfg(INT8 iID, CHAR *cLevel, CHAR *cWnd);

/**
    Set motion level and motion window

    @param[in] iID      The motion window id, now only support id=0
    @param[out] cLevel  Motion sensitive value=High,9,8,7,6,5,4,3,2,1,Low
    @param[out] cWnd    Motion window, the window grids are 12x16.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetMotionCfg(INT8 iID, CHAR *cLevel, CHAR *cWnd);

/**
    Start motion detection

    @param[in] iID      The motion window id, now only support id=0

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StartMD(INT8 iID);  //now iID please set 0

/**
    Stop motion detection

    @param[in] iID      The motion window id, now only support id=0

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StopMD(INT8 iID); // now iID please set 0

/**
    Check the status of motion detection register function (which can be registered by NvtIVS_RegisterNotifyMotion)

    @param[in] iID      The motion window id, now only support id=0

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern BOOL NvtIVS_CheckMDRegFunc(void);

// tampering detection
/**
    Get current tampering detection setting

    @param[in] uiTDIdx      Index of tampering detection window (Only support one detection window)
    @param[out] psTDSetting Pointer to the data structure of tampering detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetTDSetting(UINT32 uiTDIdx, IVSTDSetting *psTDSetting);

/**
    Set new tampering detection setting

    @param[in] uiTDIdx      Index of tampering detection window (Only support one detection window)
    @param[in] psTDSetting  Pointer to the data structure of tampering detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_SetTDSetting(UINT32 uiTDIdx, IVSTDSetting *psTDSetting);

/**
    Start tampering detection

    @param[in] uiTDIdx      Index of tampering detection window (Only support one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StartTD(UINT32 uiTDIdx);

/**
    Stop tampering detection

    @param[in] uiTDIdx      Index of tampering detection window (Only support one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StopTD(UINT32 uiTDIdx);

/**
    Get tampering detection result

    @note
    -# This function must be used after face detection start.

    @param[in] uiTDIdx      Index of tampering detection window (Only support one detection window)
    @param[out] pbDetected  Pointer to the boolean data structure of tampering detection result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetTDResult(UINT32 uiTDIdx, BOOL *pbDetected);

/**
    Unregister callback function

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_UnregisterNotifyTD(void);

/**
    Register callback function to get notify status

    @param[in] pfNotifyCB   Pointer to callback function which is getting notify status

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_RegisterNotifyTD(INT32(*pfNotifyCB)(INT8));

// face detection
/**
    Get current face detection setting

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)
    @param[in] psFDSetting  Pointer to the data structure of face detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetFDSetting(UINT32 uiFDIdx, IVSFDSetting *psFDSetting);

/**
    Set new face detection setting

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)
    @param[in] psFDSetting  Pointer to the data structure of face detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_SetFDSetting(UINT32 uiFDIdx, IVSFDSetting *psFDSetting);

/**
    Start face detection

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StartFD(UINT32 uiFDIdx);

/**
    Stop face detection

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StopFD(UINT32 uiFDIdx);

/**
    Get face detection result

    @note
    -# This function must be used after face detection start.

    @param[in] uiFDIdx          Index of face detection window (Only supports one detection window)
    @param[in] psFDData         Pointer to the data structure of face detection result
    @param[in] psTargetCoord    Mapping coordinate of face detection result
    @param[in] bUpdated         Data update flag. NVT_TRUE means this data is new

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetFDData(UINT32 uiFDIdx, IVSFDData *psFDData, IVSIAObjData *psTargetCoord, BOOL *bUpdated);

/**
    Release face detection result

    @note
    -# This function must be used after face detection start

    @param[in] uiFDIdx          Index of face detection window (Only supports one detection window)
    @param[in] psFDData         Pointer to the data structure of face detection result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_ReleaseFDData(UINT32 uiFDIdx, IVSFDData *psFDData);

// FTG (Face Tracking Grading)
/**
    Get current face tracking setting

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGSetting     Pointer to the data structure of face tracking setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetFTGSetting(UINT32 uiFTGIdx, IVSFTGSetting *psFTGSetting);

/**
    Set new face tracking setting

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGSetting     Pointer to the data structure of face tracking setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_SetFTGSetting(UINT32 uiFTGIdx, IVSFTGSetting *psFTGSetting);

/**
    Start face tracking grading

    @param[in] uiFTGIdx         Index of face tracking frame.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StartFTG(UINT32 uiFTGIdx);

/**
    Stop face tracking grading

    @param[in] uiFTGIdx         Index of face tracking frame.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StopFTG(UINT32 uiFTGIdx);

/**
    Get face tracking grading result

    @note
    -# This function must be used after face tracking grading start

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGData        Pointer to the data structure of face tracking result
    @param[in] psTargetCoord    Mapping coordinate of face tracking result
    @param[in] bUpdated         Data update flag. NVT_TRUE means this data is new.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetFTGData(UINT32 uiFTGIdx, IVSFTGData *psFTGData, IVSIAObjData *psTargetCoord, BOOL *bUpdated);

/**
    Release face tracking grading result

    @note
    -# This function must be used after face tracking grading start

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGData        Pointer to the data structure of face tracking result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_ReleaseFTGData(UINT32 uiFTGIdx, IVSFTGData *psFTGData);

/**
    Notify uITRON to save the frame in the buffer

    This function can notify uITRON to save the frame in the buffer, \n
    the frame reference to with NvtCodec_GetFTGData Infomation which you call, \n
    uITRON will save the frames which the score are top of three

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] score            The score with face ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_FTGSnapshotNotify(UINT32 uiFTGIdx, UINT32 score);

/**
    Get the snapshot which triggered by NvtIVS_FTGSnapshotNotify

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[out] output_addr     Output address to put data. Please cast to (IVSFTG_SNAPSHOT*) to parse

    @return NvtErrorCode (Refer to nvt_type.h)

    Example:
    @code
    {
        if(0 > NvtCodec_FTGSnapshotGet(id, &output_addr)){
            DBG_ERR("get snapshot error!\r\n");
            return;
        }
        DBG_DUMP("output_addr=%x\r\n",output_addr);
        CodecFTG_SNAPSHOT *ftg_snapshot= (IVSFTG_SNAPSHOT *)output_addr;
    }
    @endcode
*/
extern NVTRET NvtIVS_FTGSnapshotGet(UINT32 uiFTGIdx, UINT32 *output_addr);

/*
    Release the snapshot resource of uITRON

    @note
    -# If too many snapshots are stored in uITRON, the buffer will full and uITRON can not save snapshot anymore.

    @param[in] uiFTGIdx         Index of face tracking frame.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_FTGSnapshotRemove(UINT32 uiFTGIdx);

// Motion Vector
/*
    Start motion vector handle

    @param[in] uiMVIdx          Motion vector channel ID. This item is always 0 if only have one sensor.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StartMV(UINT32 uiMVIdx);

/*
    Stop motion vector handle

    @param[in] uiMVIdx          Motion vector channel ID. This item is always 0 if only have one sensor.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StopMV(UINT32 uiMVIdx);

/*
    Get current motion vector setting

    @param[in] uiMVIdx          Motion vector channel ID. This item is always 0 if only have one sensor.
    @param[out] psMVSetting     Pointer to the data structure of motion vector setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetMVSetting(UINT32 uiMVIdx, IVSMVSetting *psMVSetting);

/*
    Set new motion vector setting

    @param[in] uiMVIdx          Motion vector channel ID. This item is always 0 if only have one sensor.
    @param[in] psMVSetting      Pointer to the data structure of motion vector setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_SetMVSetting(UINT32 uiMVIdx, IVSMVSetting *psMVSetting);

/*
    Get motion vector data

    @param[in] uiMVIdx          Motion vector channel ID. This item is always 0 if only have one sensor.
    @param[in] psMVData         Pointer to the data structure of motion vector result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetMVData(UINT32 uiMVIdx, IVSMVData *psMVData);

// Background Construction
/*
    Start background construction

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StartBC(UINT32 uiBCIdx);

/*
    Stop background construction

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_StopBC(UINT32 uiBCIdx);

/*
    Get current background construction setting

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor
    @param[out] psBCSetting     Pointer to the data structure of background construction setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetBCSetting(UINT32 uiBCIdx, IVSBCSetting *psBCSetting);

/*
    Set new background construction setting

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor
    @param[in] psBCSetting      Pointer to the data structure of background construction setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_SetBCSetting(UINT32 uiBCIdx, IVSBCSetting *psBCSetting);

/*
    Reset background construction

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_ResetBC(UINT32 uiBCIdx);

/*
    Get foreground object result

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor
    @param[out] psBCData        Pointer to the data structure of foreground object data
    @param[in] psTargetCoord    Mapping coordinate of background construction result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetBCResult(UINT32 uiBCIdx, IVSBCData *psBCData, IVSObjData *psTargetCoord);

/*
    Get foreground block result

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor
    @param[out] psBCBLKData     Pointer to the data structure of foreground block data
    @param[in] psTargetCoord    Mapping coordinate of background construction result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetBCBLKResult(UINT32 uiBCIdx, IVSBCBLKData *psBCBLKData, IVSObjData *psTargetCoord);

/*
    Get foreground image

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor
    @param[out] psImageBuf      Pointer to the data structure of image buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetBCForeground(UINT32 uiBCIdx, IVSImageBuf *psImageBuf);

/*
    Get background image

    @param[in] uiBCIdx          Background construction channel ID, this item is always 0 if only have one sensor
    @param[out] psImageBuf      Pointer to the data structure of image buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetBCBackground(UINT32 uiBCIdx, IVSImageBuf *psImageBuf);


/*
    Get DSP image buffer information.

    @param[in] psBuf            Pointer to the data structure of DSP image buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetDSPBuf(IVSBuf *psBuf);

/*
    Get current image and can copy to another image buffer directly.

    @param[in] uiChannelID          Video input channel ID, get current image from indicated channel.
    @param[in] bIsCopytoTempBuf     Image buffer copy flag, NVT_TRUE means this function will copy image buffer to psTempBuf automatically. Copy image buffer will occupy some DMA bandwidth, but it can guarantee image buffer not to overlap by new image data from sensor.
    @param[in] psTempBuf            Temporal image buffer
    @param[in] psCurrentImageBuf    Current image buffer, this memory address is passed from image pipe line. So it’s content will be overlapped with new image data from sensor.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_GetCurrentImage(UINT32 uiChannelID, BOOL bIsCopytoTempBuf, IVSBuf *psTempBuf, IVSImageBuf *psCurrentImageBuf);

/*
    Dump image buffer into assigned file path.

    @param[in] filepath         The file path to save image buffer
    @param[in] pImgBuf          The image buffer to be saved to file

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtIVS_DumpImgBuf(char *filepath, IVSImageBuf *pImgBuf);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTIVS_H  */
