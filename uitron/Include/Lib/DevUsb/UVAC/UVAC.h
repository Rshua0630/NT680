/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       UVAC.h
    @ingroup    mISYSUVAC

    @brief      USB PCC device class API header.
                USB PCC device class API header.

*/

#ifndef _UVAC_H
#define _UVAC_H

/*-----------------------------------
    Includes
-----------------------------------*/
#include "Type.h"


#define UVAC_VID_RESO_MAX_CNT               12
#define UVAC_VID_RESO_INTERNAL_TBL_CNT      5
#define UVAC_VID_RESO_FPS_MAX_CNT           3

#define UVAC_AUD_SAMPLE_RATE_MAX_CNT        3

//customer's winusb requests
#define SET_CUR                             0x01
#define GET_CUR                             0x81
#define GET_MIN                             0x82
#define GET_MAX                             0x83
#define GET_RES                             0x84
#define GET_LEN                             0x85
#define GET_INFO                            0x86
#define GET_DEF                             0x87

/**
    @addtogroup mISYSUVAC
*/
//@{

/**
    Video format for UVC, UV packed only.
*/
typedef enum _UVAC_VIDEO_FORMAT {
	UVAC_VIDEO_FORMAT_H264,                ///< H264 format with YCbCr 420, UV packed.
	UVAC_VIDEO_FORMAT_MJPG,                ///< MJPEG format with YCbCr 422, UV packed.
	ENUM_DUMMY4WORD(UVAC_VIDEO_FORMAT)
} UVAC_VIDEO_FORMAT;


/**
    Video Resolution.

    @note For video resolution of UVAC_VID_RESO_ARY
*/
typedef struct _UVAC_VID_RESO_ {
	UINT32 width;                            ///< Width.
	UINT32 height;                           ///< Height.
	UINT32 fpsCnt;                           ///< The number of fps .
	UINT32 fps[UVAC_VID_RESO_FPS_MAX_CNT];   ///< FPS array.
} UVAC_VID_RESO, *PUVAC_VID_RESO;

/**
    Video Resolution array.

    This structure is used in UVAC_ConfigVidReso(..) to set supported video resolution, and
    it must be set before UVAC_Open() is called.
*/
typedef struct _UVAC_VID_RESO_ARY_ {
	UINT32 aryCnt;                          ///< The number of video resolution array.
	UVAC_VID_RESO *pVidResAry;              ///< Video resolution.
} UVAC_VID_RESO_ARY, *PUVAC_VID_RESO_ARY;

/**
    Audio sample array.

    This structure is used for the configuration of UVAC_CONFIG_AUD_SAMPLERATE to set supported audio resolution, and
    it must be set before UVAC_Open() is called.
*/
typedef struct _UVAC_AUD_SAMPLERATE_ARY_ {
	UINT32 aryCnt;                          ///< The number of audio resolution array.
	UINT32 *pAudSampleRateAry;              ///< Audio resolution.
} UVAC_AUD_SAMPLERATE_ARY, *PUVAC_AUD_SAMPLERATE_ARY;

/**
    Structure used for UVAC Device Vendor Request Callback API.

    This structure is used in the callback prototype UVAC_VENDOR_REQ_CB.
*/
typedef struct _UVAC_VENDOR_PARAM {
	BOOL      bHostToDevice;      ///< Data direction.
	///< - @b TRUE: USB Host sent data to USB Device.
	///< - @b FALSE: USB Host will get data from USB Device.
	UINT8     uiReguest;          ///< The bRequest field in USB Device Requests.
	UINT16    uiValue;            ///< The wValue field in USB Device Requests.
	UINT16    uiIndex;            ///< The wIndex field in USB Device Requests.
	UINT32    uiDataAddr;         ///< Data address.
	UINT32    uiDataSize;         ///< Data size, in byte, maximun is 64. If there is no data to transfer, this should be zero.
} UVAC_VENDOR_PARAM, *PUVAC_VENDOR_PARAM;

/**
    COM ID for CDC.

    It supports dual COM port.
*/
typedef enum _CDC_COM_ID {
	CDC_COM_1ST = 0,                       ///< The first COM ID.
	CDC_COM_2ND,                           ///< The second COM ID.
	CDC_COM_MAX_NUM,
	ENUM_DUMMY4WORD(CDC_COM_ID)
} CDC_COM_ID;

/**
    Callback function prototype for PSTN subclass request.
*/
typedef BOOL (*UVAC_CDC_PSTN_REQUEST_CB)(CDC_COM_ID ComID, UINT8 PstnRequestCode, UINT8 *pData, UINT32 *pDataLen);


/**
    Callback function prototype for UVAC Device Vendor Request.
    @return
     - @UINT32 0:  Ok.
     - @UINT32 1,2,...: TBD.
*/
typedef UINT32(*UVAC_VENDOR_REQ_CB)(PUVAC_VENDOR_PARAM pParam);

/**
    Callback function prototype for WINUSB.
*/
typedef UINT8(*UVAC_WINUSBCLSREQCB)(UINT32 ControlCode, UINT8 CS, UINT8 *pData, UINT32 *pDataLen);

/**
    String Descriptor
*/
typedef  struct _UVAC_STRING_DESC {
	UINT8       bLength;                ///< size of String Descriptor
	UINT8       bDescriptorType;        ///< String Descriptor type
	UINT8       bString[254];           ///< UNICODE stirng
} UVAC_STRING_DESC;

/**
    Device Descriptor
*/
typedef struct _UVAC_VEND_DEV_DESC {
	UINT16              VID;                 ///< Customized USB Vendor ID
	UINT16              PID;                 ///< Customized USB Product ID
	UVAC_STRING_DESC    *pManuStringDesc;    ///< USB Manufacturer String Descriptor ptr
	UVAC_STRING_DESC    *pProdStringDesc;    ///< USB Product String Descriptor ptr
	UVAC_STRING_DESC    *pSerialStringDesc;  ///< USB Serial Number String Descriptor ptr
	UVAC_VENDOR_REQ_CB  fpVendReqCB;         ///< Obsolete.
	UVAC_VENDOR_REQ_CB  fpIQVendReqCB;       ///< Obsolete.
} UVAC_VEND_DEV_DESC, *PUVAC_VEND_DEV_DESC;

/**
    UVAC channel.

    @note For UVAC_INFO
*/
typedef enum {
	UVAC_CHANNEL_1V1A = 1,      ///< One video and one audio channel.
	UVAC_CHANNEL_2V2A,          ///< Two video and two audio channel.
	UVAC_CHANNEL_MAX,
} UVAC_CHANNEL;

/**
    Audio format

    @note For UVAC_CONFIG_AUD_FMT_TYPE.
*/
typedef enum _UVAC_AUD_FMT_TYPE {
	UVAC_AUD_FMT_TYPE_PCM,      ///< Audio type is PCM.
	UVAC_AUD_FMT_TYPE_AAC,      ///< Audio type is AAC.
	UVAC_AUD_FMT_TYPE_MAX,
} UVAC_AUD_FMT_TYPE;

/**
    Stream path.
*/
typedef enum _UVAC_STRM_PATH {
	UVAC_STRM_VID,   ///< 1st video bitstream
	UVAC_STRM_AUD,   ///< 1st audio bitstream
	UVAC_STRM_VID2,  ///< 2nd video bitstream
	UVAC_STRM_AUD2,  ///< 2nd audio bitstream
	ENUM_DUMMY4WORD(UVAC_STRM_PATH)
} UVAC_STRM_PATH, *PUVAC_STRM_PATH;

/**
    Stream format.

    @note For UVAC_SetEachStrmInfo.
*/
typedef struct _UVAC_STRM_FRM {
	UINT32              addr;       ///< Address of stream.
	UINT32              size;       ///< Size of stream.
	UVAC_STRM_PATH      path;       ///< Stream path.
	UINT8               *pStrmHdr;  ///< VS header address.
	UINT32              strmHdrSize;///< VS header size.
} UVAC_STRM_FRM, *PUVAC_STRM_FRM;

/**
    Stream information.

    Request from USB Host.
*/
typedef struct _UVAC_STRM_INFO {
	UINT8               *pStrmHdr;      ///< H264 header address.
	UINT32              strmHdrSize;    ///< H264 header size.
	UVAC_STRM_PATH      strmPath;       ///< Stream path.
	UINT32              strmWidth;      ///< Stream width.
	UINT32              strmHeight;     ///< Stream height.
	UINT32              strmFps;        ///< Frame rate.
	UINT32              strmTBR;        ///< Target bitrate (Byte/Sec).
	UVAC_VIDEO_FORMAT   strmCodec;      ///< Stream format.
	UINT32              strmResoIdx;    ///< Obsolete.
	BOOL                isStrmOn;       ///< Indicate if the stream is active.
} UVAC_STRM_INFO, *PUVAC_STRM_INFO;

/**
    Supported video format.

    @note For UVAC_CONFIG_VIDEO_FORMAT.
*/
typedef enum _UVAC_VIDEO_FORMAT_TYPE {
	UVAC_VIDEO_FORMAT_H264_MJPEG,          ///< Support H264 and MJPEG.
	UVAC_VIDEO_FORMAT_H264_ONLY,           ///< Support H264 only.
	UVAC_VIDEO_FORMAT_MJPEG_ONLY,          ///< Support MJPEG only.
	ENUM_DUMMY4WORD(UVAC_VIDEO_FORMAT_TYPE)
} UVAC_VIDEO_FORMAT_TYPE;


/**
    UVAC configuration identifier.

    This definition is used in UVAC_SetConfig() to assgin new configuration to specified ID function.
*/
typedef enum _UVAC_CONFIG_ID {
	UVAC_CONFIG_VEND_DEV_DESC,              ///< Pointer to the vendor device descriptor, refering to #_UVAC_VEND_DEV_DESC.
	UVAC_CONFIG_AUD_SAMPLERATE,             ///< Pointer to audio sample array, refering to #_UVAC_AUD_SAMPLERATE_ARY_.
	UVAC_CONFIG_AUD_FMT_TYPE,               ///< Audio format, refering to #_UVAC_AUD_FMT_TYPE
	UVAC_CONFIG_H264_TARGET_SIZE,           ///< H.264 stream target size(Byte/Sec). Default value is 0x100000.
	UVAC_CONFIG_MJPG_TARGET_SIZE,           ///< Motion JPEG stream target size(Byte/Sec). Default value is 0x300000.
	UVAC_CONFIG_VENDOR_CALLBACK,            ///< The callback function for USB Vendor Request
	UVAC_CONFIG_VENDOR_IQ_CALLBACK,         ///< The callback function for USB Vendor IQ Request
	UVAC_CONFIG_MFK_REC2LIVEVIEW,           ///< Suspend sending stream to USB host.
	UVAC_CONFIG_MFK_LIVEVIEW2REC,           ///< Resume  sending stream to USB host.
	UVAC_CONFIG_UVAC_CAP_M3,                ///< Enable UVC capture method 3.
	UVAC_CONFIG_EU_VENDCMDCB_START,         ///< The callback function for USB Vendor Command of Extention Unit, idx0 no use
	UVAC_CONFIG_EU_VENDCMDCB_ID01,          ///< Extension unit callback with CS=1.
	UVAC_CONFIG_EU_VENDCMDCB_ID02,          ///< Extension unit callback with CS=2.
	UVAC_CONFIG_EU_VENDCMDCB_ID03,          ///< Extension unit callback with CS=3.
	UVAC_CONFIG_EU_VENDCMDCB_ID04,          ///< Extension unit callback with CS=4.
	UVAC_CONFIG_EU_VENDCMDCB_ID05,          ///< Extension unit callback with CS=5.
	UVAC_CONFIG_EU_VENDCMDCB_ID06,          ///< Extension unit callback with CS=6.
	UVAC_CONFIG_EU_VENDCMDCB_ID07,          ///< Extension unit callback with CS=7.
	UVAC_CONFIG_EU_VENDCMDCB_ID08,          ///< Extension unit callback with CS=8.
	UVAC_CONFIG_EU_VENDCMDCB_END,
	UVAC_CONFIG_WINUSB_ENABLE,              ///< Enable WINUSB.
	UVAC_CONFIG_WINUSB_CB,                  ///< The callback function for WINUSB
	UVAC_CONFIG_VIDEO_FORMAT_TYPE,          ///< Supported video format by UVC device.
	UVAC_CONFIG_CDC_ENABLE,                 ///< Enable CDC ADM. It'll emulate dual COM port at USB host.
	UVAC_CONFIG_CDC_PSTN_REQUEST_CB,        ///< The callback function for CDC PSTN subclass specific request, REFERING TO #UVAC_CDC_PSTN_REQUEST_CB.
	UVAC_CONFIG_MTP_ENABLE,                 ///< Enable MTP.
	UVAC_CONFIG_MTP_DEVICE_FREIEDLYNAME,    ///< MTP device friendly name.
	UVAC_CONFIG_MTP_VENDSPECI_DEVINFO,      ///< MTP vender specific device info.
	UVAC_CONFIG_MTP_DATATX_CB,              ///< MTP data transmit callback function.
	UVAC_CONFIG_PTP_DEVDATETIME_CB,         ///< PTP device date time callback function.
	UVAC_CONFIG_PTP_BATTERY_LEVEL,          ///< PTP battery level.
	UVAC_CONFIG_PTP_STORAGETYPE_CB,         ///< PTP storage type callback. Judge storage type by project-code.
	UVAC_CONFIG_PTP_STORAGEID,              ///< PTP storage ID.
	UVAC_CONFIG_XU_CTRL,                    ///< UVC extension unit capability.
	UVAC_CONFIG_MAX_FRAME_SIZE,				///< Max video frame size, the default is 800KB.
    UVAC_CONFIG_AUD_CHANNEL_NUM,			///< Audio channel number.
	ENUM_DUMMY4WORD(UVAC_CONFIG_ID)
} UVAC_CONFIG_ID;


/**
    Video device count.

    @note For UVAC_STARTVIDEOCB and UVAC_STOPVIDEOCB.
*/
typedef enum {
	UVAC_VID_DEV_CNT_1 = 0,
	UVAC_VID_DEV_CNT_2,
	UVAC_VID_DEV_CNT_MAX
} UVAC_VID_DEV_CNT;

typedef enum _UVAC_TYPE {
	UVAC_TYPE_U3 = 0,
	UVAC_TYPE_U2,
	ENUM_DUMMY4WORD(UVAC_TYPE)
} UVAC_TYPE;


/**
    Callback function prototype for the fpStartVideoCB of UVAC_INFO.
*/
typedef UINT32(*UVAC_STARTVIDEOCB)(UVAC_VID_DEV_CNT vidDevIdx, UVAC_STRM_INFO *pStrmInfo);

/**
    Callback function prototype for the fpGetH264HeaderCB of UVAC_INFO.
*/
typedef UINT32(*UVAC_GETH264HEADERCB)(UVAC_STRM_INFO *pStrmInfo);  ///< Callback for starting video.

/**
    Callback function prototype for the fpGetH264HeaderCB of UVAC_INFO.
*/
typedef void (*UVAC_STOPVIDEOCB)(UVAC_VID_DEV_CNT vidDevIdx);

#define _UVAC_TEST_VID_FIX_PATT_   DISABLE  //ENABLE  DISABLE

#if _UVAC_TEST_VID_FIX_PATT_
typedef void (*UVAC_GETSTRMCB)(UVAC_STRM_PATH strmPath);
#endif

/**
     @name Can mount sxdc card, but can not access if this is set to be 1.
*/
//@{
#define _UVAC_SDXC_MTP_MOUNT_NOTACCESS_          0
//@}
#if _UVAC_SDXC_MTP_MOUNT_NOTACCESS_
#include "sdio.h"

/**
    SD card type call-back function
*/
typedef PSDIO_MISC_INFORMATION(*UVAC_USIDC_GET_CARDTYPE_FUNC)(void);
#endif

/**
    Callback function prototype for the fpSetVolCB of UVAC_INFO.
*/
typedef UINT32(*UVAC_SETVOLCB)(UINT32 volume);  ///< Callback for starting video.

/**
    Information needed for UVAC module.

    This structure is used in UVAC_Open() to specify the UVAC task needed information for normal working
    and user customization callback.
*/
typedef struct _UVAC_INFO_ {
	UINT32              UvacMemAdr;                      ///< Buffer address for USB operation
	UINT32              UvacMemSize;                     ///< Buffer size for USB operation
	UINT32              hwPayload[UVAC_VID_DEV_CNT_MAX]; ///< auto-payload by hardware if it is set to 1.
	UVAC_CHANNEL        channel;                         ///< The number of UVAC channel, refering to UVAC_CHANNEL.
	UVAC_STRM_INFO      strmInfo;                        ///< Stream information.
	UVAC_STARTVIDEOCB   fpStartVideoCB;                  ///< Callback function to start video
	UVAC_GETH264HEADERCB   fpGetH264HeaderCB;            ///< Callback function to get h264's SPS
	UVAC_STOPVIDEOCB    fpStopVideoCB;                   ///< Callback function to stop video buffer
#if _UVAC_TEST_VID_FIX_PATT_
	UVAC_GETSTRMCB      fpGetStrmCB;
#endif
	UINT16              uiStrgCardLock;     ///< CardLock should be set when usidc_open()
#if _UVAC_SDXC_MTP_MOUNT_NOTACCESS_
	UVAC_USIDC_GET_CARDTYPE_FUNC pGetCardTypeFunc;///< sd card type call-back function
#endif
	UVAC_SETVOLCB		fpSetVolCB;						 ///< Callback function to audio record volume
} UVAC_INFO;

/**
    Callback functions prototype that UVAC task used.

    These definitions are used in the structure UVAC_INFO and would be used during UVAC_Open() to specify the
    UVAC_Task needed information.
*/
//@{
typedef UINT8(*UVAC_EUVENDCMDCB)(UINT32 ControlCode, UINT8 CS, UINT8 *pData, UINT32 *pDataLen);
//@}


/**
    Close UVAC module.
*/
extern void UVAC_Close(void);

/**
    Get needed buffer size for UVAC module.
*/
extern UINT32 UVAC_GetNeedMemSize(void);

/**
    Set UVAC configuration

    Assign new configuration of the specified ConfigID.

    @param[in] ConfigID         Configuration identifier
    @param[in] Value            Configuration context for ConfigID
*/
extern void UVAC_SetConfig(UVAC_CONFIG_ID ConfigID, UINT32 Value);

/**
    Set the stream of a frame information to UVAC lib.
*/
extern void UVAC_SetEachStrmInfo(PUVAC_STRM_FRM pStrmFrm);

/**
    For test only.
*/
extern void UVC_SetTestImg(UINT32 imgAddr, UINT32 imgSize);

/**
    Set video resolution.
*/
extern ER UVAC_ConfigVidReso(PUVAC_VID_RESO pVidReso, UINT32 cnt);

/**
    Open UVAC(USB VIDEO/AUDIO-Device-Class) module.

    Open UVAC(USB VIDEO/AUDIO-Device-Class) module. Before using this API to open UVAC task, the user must use UVAC_InstallID()
    to retrieve the system task ID and the flag ID for UVAC Task usage to keep the task normal working.

    @param[in] pClassInfo Information needed for opening UVAC Task. The user must prepare all the information needed.

    @return
     - @b E_OK:  The UVAC Task open done and success.
     - @b E_SYS: Buffer allocation failed.
     - @b E_PAR: Parameter error.
*/
extern UINT32   UVAC_Open(UVAC_INFO *pClassInfo);


/**
     Install UVAC task,flag and semaphore id
*/

extern void U2UVAC_InstallID(void) _SECTION(".kercfg_text");

extern void U3UVAC_InstallID(void) _SECTION(".kercfg_text");

/**
    Read data from USB host via CDC class.
    This API doesn't return until there is any data coming from USB host or CDC_AbortRead() is invoked.

    @param[in] ComID   COM ID.
    @param[in] pBuf   Buffer pointer.
    @param[in,out] pBufSize  Input length to read, output actual transfered length.

    @return
        - @b E_OK:  Success.
        - @b E_SYS: Failed
        - @b E_RSATR: UVAC_AbortCdcRead() is invoked by another task.
*/
extern ER UVAC_ReadCdcData(CDC_COM_ID ComID, UINT8 *pBuf, UINT32 *pBufSize);

/**
    Abort the function of UVAC_ReadCdcData().

    @param[in] ComID   COM ID.
*/
extern void UVAC_AbortCdcRead(CDC_COM_ID ComID);

/**
    Write data to USB host via CDC class.

    This function is a blocking API and will return after data is sent to USB host.

    @param[in] ComID   COM ID.
    @param[in] pBuf  Buffer pointer
    @param[in,out] pBufSize  Input length to read, output actual transfered length. Valid length is 0x1 to 0x7fffff.

    @return
        - @b E_OK: Start to transfer.
        - @b E_SYS: Failed.
*/
extern ER UVAC_WriteCdcData(CDC_COM_ID ComID, UINT8 *pBuf, UINT32 *pBufSize);

/**
    Set UVAC USB type.

    This function sets UVAC USB type.

    @param[in] type   USB type.

    @return
        - @b E_OK: Start to transfer.
        - @b E_SYS: Failed.
*/
extern void UVAC_SetObject(UVAC_TYPE type);

/**
    Set device icon.

    @param[in] pIcon Pointer to icon data.
    @param[in] len The length of icon data.
    @return 0: succeed; 1: DeviceIcon is too big.
*/
extern UINT8 UVAC_MTP_SetDeviceIcon(const UINT8 *pIcon, UINT32 len) __attribute__((weak));

/**
    Get device icon.

    @param[in] pLen Icon data length.
    @return The pointer of icon data.
*/
extern UINT8 *UVAC_MTP_GetDeviceIcon(UINT32 *pLen) __attribute__((weak));

/**
    Add new file to object list by full path.

    @param[in] FilePath full file path. If object is folder, must include "\\" at last.
    @return
        - @b E_OK: Successfully added.
        - @b E_SYS: Failed.
*/
extern ER UVAC_SidcAddObjectByPath(char *FilePath) __attribute__((weak));

//@}

#endif  // _UVAC_H

