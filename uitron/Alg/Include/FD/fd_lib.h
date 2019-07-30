/**
    Face detection library.

    @file       fd_lib.h
    @ingroup    mILibFD

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef FD_H
#define FD_H

#include    "GxImage.h"
#include    "fde_lib.h"

/**
    @addtogroup mILibFD
*/

#define FD_NUM_MAX                    36       ///< max face detection number

/**
     @name FD return status
*/
//@{
#define FD_STA_OK                  (0)                   ///<  the status is ok
#define FD_STA_ERROR               (-1)                  ///<  some error occurred
//@}


/**
     Face detection input image ratio.

*/
typedef enum _FD_IMAGERATIO {
	FD_IMAGERATIO_4_3 = 0,                     ///< input image ratio is 4:3
	FD_IMAGERATIO_3_2,                         ///< input image ratio is 3:2
	FD_IMAGERATIO_16_9,                        ///< input image ratio is 16:9
	FD_IMAGERATIO_MAX_CNT,                     ///< total image ratio count
	ENUM_DUMMY4WORD(FD_IMAGERATIO)
} FD_IMAGERATIO;

/**
    Face Orientation of detected
*/
typedef enum _FACE_ORIENT {
	FD_ORIENT_DEGREE_0    = 0,                      ///< face orient is degree 0
	FD_ORIENT_DEGREE_90   = 1,                      ///< face orient is degree 90
	FD_ORIENT_DEGREE_270  = 2,                      ///< face orient is degree 270
	FD_ORIENT_DEGREE_180  = 3,                      ///< face orient is degree 180
	ENUM_DUMMY4WORD(FACE_ORIENT)
} FACE_ORIENT;


/**
     @name Face Orientation of supported
*/
//@{
#define FD_SUPPORT_ORIENT_0        0x0001                   ///<  support face orient degree 0
#define FD_SUPPORT_ORIENT_90       0x0002                   ///<  support face orient degree 90
#define FD_SUPPORT_ORIENT_270      0x0004                   ///<  support face orient degree 270
#define FD_SUPPORT_ORIENT_180      0x0008                   ///<  support face orient degree 180
//@}


typedef struct _FD_FACE {
	UINT16  x;               ///< result start x
	UINT16  y;               ///< result start y
	UINT16  w;               ///< result width
	UINT16  h;               ///< result height
	UINT8   trgIdx;           ///< trigger index
} FD_FACE, *P_FD_FACE;

/**
     Face detection result info.

*/
typedef struct _FDMSG_INFO {
	UINT32      FaceDetectNum;                   ///<Detected total face number
	FD_FACE     FaceDetectRect[FD_NUM_MAX];      ///<Detected face rectangle
} FDMSG_INFO, *P_FDMSG_INFO;

/**
     Face detection callback message.

*/
typedef enum {
	FD_CBMSG_DETEND = 0,        ///< user will receive this notification when face detection process is end.
	FD_CBMSG_MAX_CNT,           ///< total callback message count
	ENUM_DUMMY4WORD(FD_CBMSG)
} FD_CBMSG;

/**
     Rotation engine ID.

*/
typedef enum {
	FD_ROTATE_ENG1 = 0,         ///< Rotation Engine 1
	FD_ROTATE_ENG2,             ///< Rotation Engine 2
	ENUM_DUMMY4WORD(FD_ROTATE_ENG)
} FD_ROTATE_ENG;

/**
     Copy Engine ID.
*/
typedef enum {
	FD_COPY_ENG1 = 0,           ///< Copy Engine 1
	FD_COPY_ENG2,               ///< Copy Engine 2
	ENUM_DUMMY4WORD(FD_COPY_ENG)
} FD_COPY_ENG;

/**
     Scale engine ID.

*/
typedef enum {
	FD_SCALE_ENG1 = 0,          ///< Scale Engine 1 (scale by FD)
	FD_SCALE_ENG2,              ///< Scale Engine 2 (scale by ISE)
	ENUM_DUMMY4WORD(FD_SCALE_ENG)
} FD_SCALE_ENG;


/**
     Config ID.

*/
typedef enum {
	FD_CFG_ROTATE_ENG = 0,      	///< config rotate engine
	FD_CFG_COPY_ENG,            	///< config copy engine
	FD_CFG_SCALE_ENG,           	///< config scale engine
	FD_CFG_MAX_FACE,            	///< config maximum face number
	FD_CFG_FACE_SCALE,          	///< config the minimal face to detect releative to the maximum of image width and weight
	FD_CFG_PROCESS_RATE,        	///< config the process rate of frame, set 3 means 3 frames detect FD 1 time.
	FD_CFG_DEBUG_CLASSIFIER_SEL,	///< config the debug type of classifier *** add by YZ 20170927 only for debug test ***
    FD_CFG_CLASSIFIER_SPEED,		///< config the speed level of method2 *** add by YZ 20170927 not support this version ***
	ENUM_DUMMY4WORD(FD_CFG)
} FD_CFG;


/**
     Debug classifier type. *** add by YZ 20170927 only for debug test ***

*/
typedef enum
{
    FD_CLASSIFIER_METHOD1 = 0,		///< using method1 classifier for FD
    FD_CLASSIFIER_METHOD2,			///< using method2 classifier for FD
    ENUM_DUMMY4WORD(FD_DEBUG_CLASSIFIER_SEL)
} FD_DEBUG_CLASSIFIER_SEL;

/**
     Classifier speed. *** add by YZ 20170927 not support this version ***

*/
typedef enum
{
    FD_CLASSIFIER_NORM = 0,		///< using the high recall version, the speed maybe slow
    FD_CLASSIFIER_FAST,			///< using the high speed version, the recall maybe low
	FD_CLASSIFIER_SLOW,			///< using the higher recall version, the speed maybe slow
    ENUM_DUMMY4WORD(FD_CLASSIFIER_SPEED)
} FD_CLASSIFIER_SPEED;

/**
     FDE target parameters for enrolling new target.

*/
typedef struct
{
    UINT32          *ptarget;   //target
    FDE_TGTSTAT     *ptgtstat;  //tgtstat
    FDE_TGTPARAM    *ptgtparam; //tgtparam
    UINT32          pyh_id;     //the physical position(0~4) 
    UINT32          map_id;     //the directional label(the uiTrgIdx of struct FACE)
}FD_FDE_TARGET;


/**
    FD callback funcion proto type
*/
typedef void (*FD_CBMSG_FP)(FD_CBMSG uiMsgID, void *Data);



/**
     Config some settings of FD.

     @param[in] config_id: configure ID.
     @param[in] value: setting value.

     Example:
     @code
     {
         FD_Config(FD_CFG_SCALE_ENG, FD_SCALE_ENG2);
     }
     @endcode
*/
extern void FD_Config(FD_CFG config_id, UINT32 value);


/**
     Get some configurations of FD.

     @param[in] config_id: configure ID.

     @return the setting value.

     Example:
     @code
     {
         UINT32 value;
         value  = FD_GetConfig(FD_CFG_SCALE_ENG);
     }
     @endcode
*/
extern UINT32 FD_GetConfig(FD_CFG config_id);
/**
     Init Face detection function.

     This function is used to initialize face detection buffer and set some initial setting.

     @param[in] buf: the work buffer for face detection process. the required buffer size can be get by FD_CalcBuffSize().
     @param[in] cachebuf: the cache work buffer for face detection process. the required cache buffer size can be get by FD_CalcCacheBuffSize().
     @param[in] maxFace: the maximum number of detected faces.
     @param[in] faceScale: the value defining the minimal face to detect releative to the maximum of image width and weight.
     @param[in] ProcessRate: the process rate of FD_Process(), if set value 3, means calling the function FD_Process() 3 times will run the face detection once.
     @return
         - @b FD_STA_OK:       Success.
         - @b FD_STA_ERROR:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf, cachebuf;

         buf.Addr = pbuf;
         buf.Size = FD_CalcBuffSize();
         cachebuf.Addr = pcachebuf;
         cachebuf.Size = FD_CalcCacheBuffSize();

         // Init FD buffer , and set max face number 10, faceScale 15, process rate as 3
         FD_Init(buf,cachebuf,10, 15, 3);
     }
     @endcode


*/
extern INT32 FD_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 maxFace, UINT32 faceScale, UINT32 ProcessRate);

/**
     UnInit face detection function.

     The API will reset the face detection result number and release buffer.
     @return
         - @b FD_STA_OK:       Success.
         - @b FD_STA_ERROR:    Some error occurred.
*/
extern INT32 FD_UnInit(void);
/**
     Process the face detection function.

     This API will scale the input image to temp buffer by image ratio.\n
     The temp image buffer will be the input image of HW face detection engine FDE.\n
     After FDE process, if returned any candidate faces, each face will be verified by Arcsoft FD lib.\n
     If Arcsoft FD lib return TRUE, then we treat this candidate face as a real face, and the face number count add 1.\n
     This API also uses the face tracking to enhance the stability of face detection.

     @note The process function will really run the face detection when FD is unlocked. It will just returned when FD is locked.\n
           User can lock/unlock the FD function by calling FD_Lock().\n

     @param[in] pInputImg: the face detection input image.
     @param[in] imgRatio: the input image w/h ratio.

     @return
         - @b >=0:       The number of detected faces
         - @b <0:        Some error occurred.

     Example:
     @code
     {
        IMG_BUF          InputImg;
        IPL_IME_BUF_ADDR CurInfo;
        UINT32           srcW,srcH,PxlFmt;
        UINT32           PxlAddr[3];
        UINT32           LineOff[3];
        INT32            FaceNumber;

        CurInfo.Id = IPL_ID_1;
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);
        if (CurInfo.ImeP2.type == IPL_IMG_Y_PACK_UV422)
        {
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        }
        else
        {
            PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        }
        srcW = CurInfo.ImeP2.Ch[0].Width;
        srcH = CurInfo.ImeP2.Ch[0].Height;
        LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
        LineOff[1] = CurInfo.ImeP2.Ch[1].LineOfs;
        LineOff[2] = CurInfo.ImeP2.Ch[2].LineOfs;
        PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];
        PxlAddr[1] = CurInfo.ImeP2.PixelAddr[1];
        PxlAddr[2] = CurInfo.ImeP2.PixelAddr[2];
        GxImg_InitBufEx(&InputImg, srcW, srcH, PxlFmt, LineOff, PxlAddr);
        FaceNumber = FD_Process(&InputImg, FD_IMAGERATIO_4_3);
     }
     @endcode
*/
extern INT32 FD_Process(IMG_BUF *pInputImg, FD_IMAGERATIO  imgRatio);


/**
     Detect face of one picture.

     This API will detect the face of one input image buffer.

     @param[in] pImgBuf: the input image buffer.
     @param[in] FDBuffAddr: the work buffer for face detection process. the buffer size can be get by FD_CalcBuffSize().
     @param[in] FDCacheBuffAddr: the cache buffer for face detection process. the cache buffer size can be get by FD_CalcCacheBuffSize().
     @param[out] Face_rslt: the result face rectangle, the array size should be FD_NUM_MAX.

     @return the face number detected.

     Example:
     @code
     {
         UINT32  NumFaces, WorkMem, CacheMem;
         IMG_BUF g_TmpImgBuf;
         FACE    Face_rslt[FD_NUM_MAX];


         // prepare input image buffer
         g_TmpImgBuf = ...
         // assign work buffer
         WorkMem = ...
         // assing cache buffer
         CacheMem = ...
         NumFaces = FD_DetectOnePic(&g_TmpImgBuf, WorkMem, CacheMem, Face_rslt);
     }
     @endcode

*/
extern UINT32 FD_DetectOnePic(PIMG_BUF pImgBuf, UINT32 FDBuffAddr, UINT32 FDCacheBuffAddr, FD_FACE *Face_rslt);

/**
     Calculate face detection required buffer size.

     The working buffer is used for scaling temp buffer and HW engine FDE output data.

     @return the required buffer size.
*/
extern UINT32   FD_CalcBuffSize(void);

/**
     Calculate face detection required cache buffer size.

     The cache buffer size is for arcsoft lib using.

     @return the required cache buffer size.
*/
extern UINT32   FD_CalcCacheBuffSize(void);

/**
     Register callback function.

     @note User must register callback funcion then can receive the callback message notification when detected faces.

     @param[in] CB: the callback function pointer.

     Example:
     @code
     {
        void Photo_FdCB(FD_CBMSG uiMsgID, void *Data)
        {
            switch (uiMsgID)
            {
                case FD_CBMSG_DETEND:
                   DBG_IND("===FDEND===\r\n");
                   Ux_PostEvent(NVTEVT_ALGMSG_FDEND,1, NVTEVT_ALGMSG_FDEND);
                   break;
                default:
                   break;
            }
        }
        FD_RegisterCB(Photo_FdCB);
     }
     @endcode
*/
extern void     FD_RegisterCB(FD_CBMSG_FP CB);

/**
     Lock/Unlock face detction.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the FD
         FD_Lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the FD
         FD_Lock(FALSE);
     }
     @endcode
*/
extern void     FD_Lock(BOOL bLock);


//  fd info

/**
     Get input image of face detection.

     @param[out] Img: the input image of face detection.
*/
extern void     FD_GetInputImg(IMG_BUF *Img);

/**
     Clear the face number.

     This API will clear the result face number of face detection.

*/
extern void     FD_ClrRsltFaceNum(void);

/**
     Get the face number.

     This API will return the result face number of face detection.

     @return The number of detected faces.
*/
extern UINT32   FD_GetRsltFaceNum(void);


/**
     Get the result faces of different coordinate.

     This API will return the result faces by different coordinate.\n

     @param[out] Face: the result face info.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of detected faces.

     Example:
     @code
     {
        FD_FACE    FdInfo[FD_NUM_MAX];
        URECT      FdDispCoord={0,0,640,480};
        UINT32     FaceNumber;

        FaceNumber = FD_GetRsltFace(FdInfo, &FdDispCoord);
     }
     @endcode
*/
extern UINT32   FD_GetRsltFace(FD_FACE *Face, URECT *TargetCoord);


/**
     Get the face orientation.

     @return The face orientation.
*/
extern FACE_ORIENT   FD_GetFaceOrient(void);


/**
     Set supported face orientations.

     The API will set the supported face orientation of FD engine.\n
     The orientations that FD engine supported are 0, 90, 270, 180, user can configure which orientation to support.\n
     The default setting of supported orientations are 0, 90, 270.

     @return void

     Example:
     @code
     {
         UINT32  SupportOrient = FD_SUPPORT_ORIENT_0|FD_SUPPORT_ORIENT_90|FD_SUPPORT_ORIENT_270;

         FD_SetSupportFaceOrient(SupportOrient);
     }
     @endcode
*/
extern void   FD_SetSupportFaceOrient(UINT32   faceOrient);

/**
     Get the face detection result flag.

     Get the face detection result flag.
     If the face detection result has been updated, this flag will be set true.
     When API FD_GetRsltFace() is called, the flag is auto cleared.

     @return The face detection result flag.
*/
extern BOOL   FD_GetRsltFlag(void);

#endif
