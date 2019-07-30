/**
    Public header file for capture API

    This file is the header file that define the API and data type for capture API.

    @file       ImgCaptureAPI.h
    @ingroup    mIAppCap
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IMG_CAPTURE_API_H
#define _IMG_CAPTURE_API_H
#include "Type.h"
#include "imgcapcbmsg.h"

/** \addtogroup mIAppCap */
//@{

/**
    S2 detection prototype
*/
//@{
typedef BOOL (*IMG_CAP_S2DET_FP)(void);
//@}

/**
    write file prototype
*/
//@{
typedef INT32(*IMG_CAP_FSYS_FP)(UINT32 Addr, UINT32 Size, UINT32 Fmt, INT32 SensorID);
//@}

/**
    cap open parameter
*/
typedef struct _CAP_OBJ_INFO {
	UINT32  uiBufAddr;                      ///< input buffer address
	UINT32  uiBufSize;                      ///< input buffer size
	IMG_CAP_CBMSG_FP CBMsgFP;               ///< UI flow callback function pointer
	IMG_CAP_S2DET_FP S2DetFP;               ///< S2 key detection function pointer
	IMG_CAP_FSYS_FP WriteFileFP;            ///< write file function pointer
} CAP_OBJ_INFO;

/**
     cap open
     initial cap module

     @param[in] CapObj    input object

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL

    Example:
    @code
    {
        CAP_OBJ_INFO CapObj;

        CapObj.uiBufAddr = 0x500000;
        CapObj.uiBufSize = 0xF00000;
        CapObj.S2DetFP = NULL;
        CapObj.CBMsgFP = NULL;
        CapObj.WriteFileFP = xExamCap_WriteFile;
        ImgCap_Open(&CapObj);
    }
    @endcode

*/
ER ImgCap_Open(CAP_OBJ_INFO *CapObj);

/**
     cap close
     close cap module

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
ER ImgCap_Close(void);

/**
     trigger start capture

     @note without wait command finish

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
ER ImgCap_StartCapture(void);

/**
     trigger stop capture

     @note in SEL_DRIVEMODE_INVID mode, it trigger by start to process dram data.
           in other mode, if set picture number > 1, it can stop capture process.

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
ER ImgCap_StopCapture(void);

/**
     wait capture finish

     @note wait capture end(include write file)

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
ER ImgCap_WaitFinish(void);

/**
     get current jpeg information

     @note after ImgCap_WaitFinish, user can use this function to get current jpeg information

     @param[out] Addr    jpeg buffer address
     @param[out] Size    jpeg buffer size
*/
void ImgCap_GetCurJpgInfo(UINT32 *Addr, UINT32 *Size);


/**
     get current QuickView information (IQSim only)

     @note after ImgCap_WaitFinish, user can use this function to get current QuickView information

     @param[out] Addr    QuickView buffer address
     @param[out] Size    QuickView buffer size
     @param[out] Fmt     QuickView format

*/
void ImgCap_GetCurQVInfo(UINT32 *Addr, UINT32 *Size, UINT32 *Fmt);


/**
     get current Primary Image information (IQSim only)

     @note after ImgCap_WaitFinish, user can use this function to get current Primary Image information

     @param[out] Addr    Primary Image buffer address
     @param[out] Size    Primary Image buffer size
     @param[out] Fmt     Primary Image format

*/
void ImgCap_GetCurPRIInfo(UINT32 *Addr, UINT32 *Size, UINT32 *Fmt);



/**
     install cap control id
*/
extern void Cap_InstallID(void) _SECTION(".kercfg_text");
//@}
/**
     get current buffer information
*/
extern void ImgCap_DumpBuf(void (*dump)(char *fmt, ...));
#endif
