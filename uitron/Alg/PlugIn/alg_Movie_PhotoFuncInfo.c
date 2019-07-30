#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "UIPhotoFuncInfo.h"
#include "UIAppMovie.h"
#include "alg_MovieExe.h"
#include "riff.h"
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
#include "ImageApp_MovieMulti.h"
#endif

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

// Common header files for pull-mode video buffer
#include "ImageUnit_UserProc.h"

#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#define MOVIE_FD_FACESCALE	15
#endif
#if _TSR_FUNC_
#include "UIAppPhoto.h"
#include "UIControlWnd.h"
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#include "UIAppMovie.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#include "dsp.h"
#if TSR_READ_TSD_MODEL_FROM_SDCARD
#define TSR_TSD_MODEL_NAME "tsd_model.bin"
#endif
#define TSR_OCR_MODEL_NAME "ocr_model.bin"
#endif
#if TSR_MODEL_IN_PSTORE
#include "PStore.h"
#define PS_TSR_MODEL1_DATA            "TSRMDL1_DAT"
#define PS_TSR_MODEL1_SIZE            "TSRMDL1_SIZ"
#define PS_TSR_MODEL2_DATA            "TSRMDL2_DAT"
#define PS_TSR_MODEL2_SIZE            "TSRMDL2_SIZ"
#endif

// Global variable
extern URECT gMovieOsdDispCord;

#if (_ADAS_FUNC_ == ENABLE)
#include "adas_Apps.h"
#include "ImageApp_Movie.h"
#endif  // #if (_ADAS_FUNC_ == ENABLE)

//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if (GPS_FUNCTION == ENABLE)
extern FLOAT g_CurSpeed;
extern BOOL g_GPSLinked;
extern BOOL g_GPSStatus;
#endif
//#NT#2016/09/29#KCHong -end

// #NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
#include "CvHost_lib.h"
#include "UIAppMovie_CvHostEx.h"

static BOOL g_CvHostExDone = FALSE;
#endif
// #NT#2015/09/17#JS Liu - end
//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
#include "NvtCv_lib.h"
#include "UIAppMovie_NvtCvEx.h"

static BOOL g_NvtCvExDone = FALSE;
#endif
//#NT#2016/05/31#Omega Yu -end
//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
#include "NvtOt.h"
#include "ImageUnit_ImagePipe.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
#include "dis_alg_dsp.h"
#include "ImageUnit_ImagePipe.h"
#endif
//#NT#2016/05/20#Yang Jin -end

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#include "DDD_lib.h"
#include "UIAppMovie.h"
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#include "crypto.h"
#define DDD_MODEL_NAME "ciphertext.bin"
#define DDD_MAX_DB_FILE_SIZE    (12 * 1024 * 1024)
//#NT#2016/05/11#Isiah Chang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

//#NT#2016/05/23#David Tsai -begin
//#NT# Add tampering detection and background calculation function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_SCD_FUNC || MOVIE_BC_MD
#include "bc_lib.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#define DVCAM_BC_USE_ODT 	DISABLE
#endif

#if MOVIE_TWD_FUNC
#include "TWD_lib.h"
#endif
#if MOVIE_TZD_FUNC
#include "TZD_lib.h"
#endif
//#NT#2016/10/14#Yuzhe Bian -end
//#NT#2016/05/23#David Tsai -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
#include "SCD_lib.h"
#endif
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoMapping.h"
#include "ftg_fd_lib.h"
#endif
//#NT#2016/10/17#Bin Xiao -end

#if MOVIE_CDNN_FUNC
#include "alg_CDNN_PhotoFuncInfo.h"
#include "NvtCDNNDev.h"
#define CDNN_DEV    DISABLE
#endif


//#NT#2017/11/3#Ming Yang -begin
//#NT# Object detection
#if (MOVIE_OD_FUNC == ENABLE)
//#include "NvtCNN.h"
#include "NvtNNFlow.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "GxStrg.h"
#include "HwClock.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2017/11/3#Ming Yang -end


//#NT#2017/09/27#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if (MOVIE_ODT_FUNC == ENABLE)
#include "odt_lib.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "HwClock.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2017/09/27#Bin Xiao -end


//#NT#2018/03/12#Dan Guo -begin
//#NT# Face recognition
#if (MOVIE_FR_FUNC == ENABLE)
#include "FR_lib.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "HwClock.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2018/03/12#Dan Guo -end


//#NT#2018/01/29#Bin Xiao -begin
//#NT# Face Detection by CNN
#if (MOVIE_FDCNN_FUNC == ENABLE)
#include "fdcnn_lib.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "HwClock.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2018/01/29#Bin Xiao -end

//#NT#2017/12/04#WY -begin
//#NT#
#if (MOVIE_ODP_FUNC == ENABLE) || (MOVIE_ODP_TEST_FUNC == ENABLE)
#include "NvtNNFlow.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "GxStrg.h"
#include "HwClock.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2017/12/04#WY -end

//#NT#2018/03/19#Wayne Wei -begin
//#NT#Support License Plate Recognition function
#if MOVIE_LPR_FUNC
#include "ImageUnit_ImagePipe.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "FileSysTsk.h"
#include "LPR_API.h"
#include "UIControlWnd.h"
#endif
//#NT#2018/01/23#Wayne Wei -end

#include "DxSensor.h"
#if DDD_MODEL_IN_PSTORE
#include "PStore.h"
#define PS_DDD_MODEL1_DATA            "DDDMDL1_DAT"
#define PS_DDD_MODEL1_SIZE             "DDDMDL1_SIZ"
#endif

#include "ImageUnit_UserProc.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppMovie_PhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_BC_MD
static BOOL gBCLock = TRUE;
static UINT32 gBCCounter = 0, gBCProcessRate = 4; // 3;
static UINT32 gBCUpdateCounter = 0;
#endif

#if MOVIE_FTG_FUNC
static BOOL gFTGLock = TRUE;
#endif
extern UINT32 System_GetEnableSensor(void);

#if (MOVIE_TSR_FUNC_ && _TSR_FUNC_)
static BOOL gInited = FALSE;

void Movie_TSRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	TSR_Init(buf, cachebuf, 1);
}

void Movie_TSRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    ISF_DATA         isf_data;
    ISF_UNIT         *pUnit;
    ISF_PORT         *pSrc;
    IMG_BUF*         IMG_TEMP;
	BOOL			 buf_got = FALSE;

#if 0
    {
        pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
		pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif
        if(pSrc)
        {
            if (!ImageUnit_IsAllowPull(pSrc))
            {
                ImageUnit_Begin(pUnit, 0);
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
                ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
#else
                ImageUnit_SetParam(ISF_OUT15, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
#endif
                ImageUnit_End();
            }
            ImageUnit_PullData(pSrc, (ISF_DATA*)&isf_data, 0);
            IMG_TEMP = (IMG_BUF *)&isf_data.Desc[0];
            debug_msg("ISF_UserProc ISF_OUT15 has output, size is %d, %d\r\n",
                      IMG_TEMP->Width, IMG_TEMP->Height);

            ImageUnit_ReleaseData((ISF_DATA*)&isf_data);
	//return;

        }
        else
            debug_msg("ISF_UserProc ISF_OUT15 failed!\r\n");

	}
#endif

	if(TSR_ValidForProcess() == FALSE)
		return;
    pUnit = &ISF_UserProc;
#if 0
    ImageUnit_Begin(pUnit, 0);
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
    ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
#else
    ImageUnit_SetParam(ISF_OUT15, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
#endif
    ImageUnit_End();
#endif
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, &isf_data, 0) == ISF_OK) {
        IMG_TEMP = (IMG_BUF *)&isf_data.Desc[0];
        TSR_PreProcess(IMG_TEMP, TSR_IMAGERATIO_16_9, buf->Addr, cachebuf->Addr);
		ImageUnit_ReleaseData(&isf_data);
		buf_got = TRUE;
    }

	if(buf_got)	{
		TSR_Process();
    }
}

void Movie_TSRProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvtsr = {
	{
		PHOTO_PRIORITY_SH,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TSRProcess",
		Movie_TSRInit,          ///< init tsr, only execute at first process
		Movie_TSRProcess,       ///< process tsr
		Movie_TSRProcessEnd,     ///< process end tsr
		NULL,                      ///< exit fp, only execute at photo close
		TSR_CalcBuffSize,       ///< get working buffer tsr
		TSR_CalcCacheBuffSize,  ///< get working cache buffer tsr
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
BOOL Movie_TSRDSPReadModel(CHAR *filePath, UINT32 mdlAddr, UINT32 mdlSize)
{
    FST_FILE filehdl;
    filehdl = FileSys_OpenFile(filePath, FST_OPEN_READ);
    if(!filehdl)
    {
        DBG_ERR("Error: can not open file %s\r\n", filePath);
        return FALSE;
    }

    FileSys_ReadFile(filehdl, (UINT8*)mdlAddr, &mdlSize, 0, NULL);
    FileSys_CloseFile( filehdl );
    return TRUE;
}
#if TSR_MODEL_IN_PSTORE
UINT32 Movie_TSRCheckPSFormat(void)
{
#define SETTING_SIZE 30
#define FORMAT_CHK_STRING  "format="
    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;
    pFile = FileSys_OpenFile("A:\\tsr_psformat", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, FORMAT_CHK_STRING);
            sscanf(pCh + sizeof(FORMAT_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_TSRCheckPSFormat FormatChk = %d, size %d\r\n", uiUpdateChk, sizeof(FORMAT_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
    }
    return 0;
}
UINT32 Movie_TSRCheckDBUpdate(void)
{
#define SETTING_SIZE 30
#define UPDATE_CHK_STRING  "update="
    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;
    pFile = FileSys_OpenFile("A:\\tsr_update", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, UPDATE_CHK_STRING);
            sscanf(pCh + sizeof(UPDATE_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_TSRCheckDBUpdate UpdateChk = %d, size %d\r\n", uiUpdateChk, sizeof(UPDATE_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
    }
    return 0;
}
#endif
#if TSR_READ_TSD_MODEL_FROM_SDCARD
UINT32 Movie_TSRGetDBFileSizeFrCard_TSD(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        DBG_DUMP("TSR load tsd model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}
UINT32 Movie_TSRGetDBFileSize_TSD(void)
{
    UINT32 ui32FileSize = 0;
#if TSR_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL1_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER
                    || Movie_TSRCheckDBUpdate())
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n", result, ui32FileSize);
        if (ui32FileSize == 0)
        {
            DBG_WRN("PStore Save default TSD Model Size\r\n");
            ui32FileSize = Movie_TSRGetDBFileSizeFrCard_TSD();
            DBG_WRN("ui32FileSize=%d\r\n", ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load TSD Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load TSD Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_TSRGetDBFileSizeFrCard_TSD();
#endif
    return ui32FileSize;
}
void Movie_TSRGetDBFile_TSD(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    FST_FILE filehdl = NULL;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        DBG_DUMP("TSR load tsd model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                         uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        return;
    }
}
#endif
UINT32 Movie_TSRGetDBFileSizeFrCard_OCR(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        DBG_DUMP("TSR load ocr model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}
UINT32 Movie_TSRGetDBFileSize_OCR(void)
{
    UINT32 ui32FileSize = 0;
#if TSR_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL2_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER
                    || Movie_TSRCheckDBUpdate())
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n",result,ui32FileSize);
        if (ui32FileSize==0)
        {
            DBG_WRN("PStore Save default OCR Model Size\r\n");
            ui32FileSize=Movie_TSRGetDBFileSizeFrCard_OCR();
            DBG_WRN("ui32FileSize=%d\r\n",ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize,0,sizeof(UINT32),pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load OCR Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load OCR Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_TSRGetDBFileSizeFrCard_OCR();
#endif
    return ui32FileSize;
}
void Movie_TSRGetDBFile_OCR(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    FST_FILE filehdl = NULL;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        DBG_DUMP("TSR load ocr model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                         uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        return;
    }
}

INT32 Movie_TSRGetDBFileAndInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if(!gInited)
	{
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
		UINT32 uiDBFileSize_tsd;
		UINT32 uiBuflen_tsd;
		UINT32 tsdModelAddr;
	#endif
    	UINT32 uiDBFileSize_ocr;
    	UINT32 uiBuflen_ocr;
    	UINT32 ocrModelAddr;
    	UINT32 tsdDspBufAddr;

		if(FileSys_WaitFinish() == FST_STA_ERROR)
		{
			DBG_WRN("TSR_Init Failed: file system is not ready!\r\n");
			return 0;
		}

		tsdDspBufAddr = buf->Addr;

	#if TSR_READ_TSD_MODEL_FROM_SDCARD
		uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
    	uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
    	uiBuflen_tsd = ALIGN_CEIL_16(uiDBFileSize_tsd);
    	uiBuflen_ocr = ALIGN_CEIL_16(uiDBFileSize_ocr);
    	tsdModelAddr = ALIGN_CEIL_4(tsdDspBufAddr + NvtCv_TSDDSP_CalcBuffSize());
    	ocrModelAddr = ALIGN_CEIL_4(tsdModelAddr + uiBuflen_tsd);
    #else
		uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
		uiBuflen_ocr = ALIGN_CEIL_16(uiDBFileSize_ocr);
		ocrModelAddr = ALIGN_CEIL_4(tsdDspBufAddr + NvtCv_TSDDSP_CalcBuffSize());
	#endif

	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	if(uiDBFileSize_tsd > NvtCv_TSDDSP_TSDMaxModelSize())
    	{
        	DBG_ERR("TSD DB file size is too large!\r\n");
        	return 0;
    	}
	#endif
    	if(uiDBFileSize_ocr > NvtCv_TSDDSP_OCRMaxModelSize())
    	{
        	DBG_ERR("OCR DB file size is too large!\r\n");
        	return 0;
    	}
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	if(buf->Size < NvtCv_TSDDSP_CalcBuffSize() + uiBuflen_tsd + uiBuflen_ocr + 4)
    	{
        	DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
			return 0;
    	}
	#else
	    if(buf->Size < NvtCv_TSDDSP_CalcBuffSize() + uiBuflen_ocr + 4)
    	{
        	DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
			return 0;
    	}
    #endif
#if (TSR_MODEL_IN_PSTORE==DISABLE)
    	{//load Tsr Model
        	FST_FILE filehdl = NULL;
		#if TSR_READ_TSD_MODEL_FROM_SDCARD
        	CHAR* pBinaryFileName_tsd = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        	filehdl = FileSys_OpenFile(pBinaryFileName_tsd, FST_OPEN_READ);
        	if(!filehdl)
        	{
            	CHAR* pBinaryFileName2_tsd = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
            	DBG_DUMP("TSR load tsd model from SD-card!\r\n");
            	filehdl = FileSys_OpenFile(pBinaryFileName2_tsd, FST_OPEN_READ);
        	}
        	if(filehdl)
        	{
            	FileSys_ReadFile(filehdl, (UINT8*)tsdModelAddr, &uiBuflen_tsd, 0, NULL);
            	FileSys_CloseFile(filehdl);
        	}
        	else
        	{
            	DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
            	return 0;
        	}
		#endif

        	CHAR* pBinaryFileName_ocr = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        	filehdl = FileSys_OpenFile(pBinaryFileName_ocr, FST_OPEN_READ);
        	if(!filehdl)
        	{
            	CHAR* pBinaryFileName2_ocr = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
            	DBG_DUMP("TSR load ocr model from SD-card!\r\n");
            	filehdl = FileSys_OpenFile(pBinaryFileName2_ocr, FST_OPEN_READ);
        	}

        	if(filehdl)
        	{
            	FileSys_ReadFile(filehdl, (UINT8*)ocrModelAddr, &uiBuflen_ocr, 0, NULL);
            	FileSys_CloseFile(filehdl);
        	}
        	else
        	{
            	DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
            	return 0;
        	}
    	}
#else
    	PPSTORE_SECTION_HANDLE  pSection;
    	UINT32  result;
    	UINT32  uiUpdateDBChk = Movie_TSRCheckDBUpdate();

    	if(Movie_TSRCheckPSFormat() == 1)
    	{
        	DBG_WRN("PStore format!\r\n");
        	System_PS_Format();
		#if TSR_READ_TSD_MODEL_FROM_SDCARD
        	uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
		#endif
        	uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
    	}
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	uiBuflen_tsd = uiDBFileSize_tsd;
	#endif
    	uiBuflen_ocr = uiDBFileSize_ocr;
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	if ((pSection = PStore_OpenSection(PS_TSR_MODEL1_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    	{
        	result = PStore_ReadSection((UINT8 *)tsdModelAddr, 0, uiBuflen_tsd, pSection);
        	if (result != E_PS_OK || uiUpdateDBChk)
        	{
            	Movie_TSRGetDBFile_TSD((UINT8 *)tsdModelAddr, &uiBuflen_tsd);
            	DBG_WRN("Save TSD Model Data in PStore [size:%d]\r\n", uiBuflen_tsd);
            	PStore_WriteSection((UINT8 *)tsdModelAddr, 0, uiBuflen_tsd, pSection);
            	PStore_CloseSection(pSection);
        	}
        	else
        	{
            	DBG_DUMP("PStore Load TSD Model Data OK\r\n");
            	PStore_CloseSection(pSection);
        	}
    	}
    	else
    	{
        	DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        	return 0;
    	}
	#endif

    	if((pSection = PStore_OpenSection(PS_TSR_MODEL2_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    	{
        	result = PStore_ReadSection((UINT8 *)ocrModelAddr, 0, uiBuflen_ocr, pSection);
        	if (result != E_PS_OK || uiUpdateDBChk)
        	{
            	Movie_TSRGetDBFile_OCR((UINT8 *)ocrModelAddr, &uiBuflen_ocr);
            	DBG_WRN("Save OCR Model Data in PStore [size=%d]\r\n", uiBuflen_ocr);
            	PStore_WriteSection((UINT8 *)ocrModelAddr, 0, uiBuflen_ocr, pSection);
            	PStore_CloseSection(pSection);
        	}
        	else
        	{
            	DBG_DUMP("PStore Load OCR Model Data OK\r\n");
            	PStore_CloseSection(pSection);
        	}
    	}
    	else
    	{
        	DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        	return 0;
    	}
#endif
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	NvtCv_TSDDSP_Init(tsdDspBufAddr, cachebuf->Addr, tsdModelAddr, ocrModelAddr);
    #else
    	NvtCv_TSDDSP_Init(tsdDspBufAddr, cachebuf->Addr, ocrModelAddr);
    #endif
		gInited = TRUE;
	}
	return 1;
}

void Movie_TSRDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	gInited = FALSE;
    if(!dsp_isOpened(DSP_ID_1))
    {
        DBG_WRN("DSP is not start, app open DSP automatic!\r\n");
        SxCmd_DoCommand("dsp start 1");
    }
}

void Movie_TSRDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//debug_msg("Movie_TSRDSPProcess start\r\n");
	if(!Movie_TSRGetDBFileAndInit(buf, cachebuf))
	{
		return;
	}
	NvtCv_TSDDSP_Process();
	//debug_msg("Movie_TSRDSPProcess end\r\n");
}

void Movie_TSRDSPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if 1
#if 0
    VControl *pCurrnetWnd;
    Ux_GetFocusedWindow(&pCurrnetWnd);
    Ux_SendEvent(pCurrnetWnd, NVTEVT_UPDATE_INFO, 1, UIAPPPHOTO_CB_ALGEND);
#else
	  //#NT#2018/06/06#B_B Jiang -start
    if(!TSR_GetLockStatus())
	  //#NT#2018/06/06#B_B Jiang -end
        Ux_PostEvent(NVTEVT_UPDATE_INFO, 1, UIAPPPHOTO_CB_ALGEND);
#endif
#else
    ISF_DATA		 isf_data;
    ISF_UNIT		 *pUnit;
    ISF_PORT		 *pSrc;
    pUnit = &ISF_UserProc;
    ImageUnit_Begin(pUnit, 0);
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
    ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
#else
    ImageUnit_SetParam(ISF_OUT15, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
#endif
    ImageUnit_End();
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA*)&isf_data, 0) == ISF_OK)
    {
        if(((IMG_BUF *)&isf_data.Desc[0])->PxlAddr[0]==0)
        { return; }
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

        //	logSize = TSR_GetLog(&logBuf);
        //	  if(logSize!=0)
        //	  { mp4log_AddLog(DbgLogTsrID, (char*)logBuf, logSize); }
        ImageUnit_ReleaseData((ISF_DATA*)&isf_data);
        DBG_DUMP("tsd dsp end 4\r\n");
	}
#endif
}

UINT32 Movie_TSRDSP_CalcBufWithModel(VOID)
{
#if POWERON_WAIT_FS_READY
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    UINT32 uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
	#endif
    UINT32 uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    if(uiDBFileSize_tsd==0)
        uiDBFileSize_tsd=Movie_TSRGetDBFileSizeFrCard_TSD();
    DBG_DUMP("^Ytsd filesize:%d\r\n", uiDBFileSize_tsd);
	#endif
    if(uiDBFileSize_ocr==0)
        uiDBFileSize_ocr=Movie_TSRGetDBFileSizeFrCard_OCR();
    DBG_DUMP("^Yocr filesize:%d\r\n", uiDBFileSize_ocr);
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    if(uiDBFileSize_tsd > NvtCv_TSDDSP_TSDMaxModelSize())
    {
        DBG_ERR("TSD DB file size is too large!\r\n");
        return 0;
    }
	#endif
    if(uiDBFileSize_ocr > NvtCv_TSDDSP_OCRMaxModelSize())
    {
        DBG_ERR("OCR DB file size is too large!\r\n");
        return 0;
    }
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(uiDBFileSize_tsd) + ALIGN_CEIL_16(uiDBFileSize_ocr) + 4;
#else
    return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(uiDBFileSize_ocr) + 4;
	#endif
#else
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
	return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(NvtCv_TSDDSP_TSDMaxModelSize()) + ALIGN_CEIL_16(NvtCv_TSDDSP_OCRMaxModelSize()) + 4;
	#else
    return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(NvtCv_TSDDSP_OCRMaxModelSize()) + 4;
	#endif
#endif
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtsrdsp = {
	{
		PHOTO_PRIORITY_SH,              ///< function hook process Id.
		IPL_PATH_1,                     ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"Movie_TSRDSPProcess",
		Movie_TSRDSPInit,               ///< init tsr, only execute at first process
		Movie_TSRDSPProcess,            ///< process tsr
		Movie_TSRDSPProcessEnd,         ///< process end tsr
		NULL,                      ///< exit fp, only execute at photo close
		Movie_TSRDSP_CalcBufWithModel,  ///< get working buffer tsr
		NvtCv_TSDDSP_CalcCacheBuffSize, ///< get working cache buffer tsr
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

//#NT#2017/11/3#Ming Yang -begin
//#NT# Object detection
#define DEBUG_DUMP_IMG           DISABLE
#if (MOVIE_OD_FUNC == ENABLE)
void Movie_ODInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	NN_NetSelect(2);
	NN_Init(buf, cachebuf, 1);
}

void Movie_ODProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
#if (DEBUG_DUMP_IMG == ENABLE)
	static UINT32    frame_cnt = 0;
	char             filename[50];
#endif

    while(((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 0))
    {
        //debug_msg("FileSys_Init not ready\r\n");
    }
    static UINT32 is_first_load_model = 1;
	if ((is_first_load_model == 1) && ((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 1)) {
        is_first_load_model = 0;
		NN_NetSelect(2);
		NN_LoadModel();
	}

	#if (MOVIE_OD_USE_ODT && MOVIE_ODT_FUNC)
    odt_rslt_loc                    TimeStamp = {0};
	static INT32					regist_falg = ODT_STA_ERROR;
	static UINT32					od_rslt_loc;
	static UINT32					od_num;
	static NN_Result				od_rslts[MAX_DETECTED_OBJECT];
	static odt_od_result 			od_odt_rslts[MAX_DETECTED_OBJECT];

	static odt_process_buf_param 	od_odt_buf_param;
	static odt_rslt_inf				od_buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				od_buf2;
	static odt_od_result 			od_buf3[MAX_DETECTED_OBJECT];

	od_odt_buf_param.detect_rslt_addr = (UINT32)od_buf3;
	od_odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&od_buf2);
	od_odt_buf_param.odt_rslt_buffer_addr = (UINT32)od_buf1;

	if (regist_falg == ODT_STA_ERROR) {
		regist_falg = odt_regist(&od_odt_buf_param);
		if (regist_falg == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 15, od_odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 1, od_odt_buf_param.process_id);
		}
	}
	#endif

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15); //ISF_OUT15  KC_old_version:ISF_OUT16
#endif

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		#if (MOVIE_OD_USE_ODT && MOVIE_ODT_FUNC)
        TimeStamp.timestamp_sec  = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		TimeStamp.timestamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
		od_rslt_loc = (UINT32)&TimeStamp;
		//od_rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif

		NN_Process((IMG_BUF *)&isf_data.Desc[0], 1);
#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	#if (MOVIE_OD_USE_ODT && MOVIE_ODT_FUNC)
	UINT32 i;
	URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
	od_num = NN_GetResult(od_rslts, &targetcoord);
	for(i = 0; i < od_num; i++)
	{
		od_odt_rslts[i].x = od_rslts[i].x *3;
		od_odt_rslts[i].y = od_rslts[i].y *3;
		od_odt_rslts[i].w = od_rslts[i].w *3;
		od_odt_rslts[i].h = od_rslts[i].h *3;
		od_odt_rslts[i].score = od_rslts[i].score;
		od_odt_rslts[i].class = od_rslts[i].class;
	}
    odt_setodrslt_timestamp(od_num, od_odt_rslts, od_rslt_loc, od_odt_buf_param.process_id);
	//odt_setodrslt(od_num, od_odt_rslts, od_rslt_loc, od_odt_buf_param.process_id);
	#else
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
	#endif

}
/*
#define OD_DRAW_RESULT    ENABLE
#define OD_PUSH_RESULT    ENABLE

void Movie_ODProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	UINT32 ODNumber = 0;
	OD_Result od_result[MAX_DETECTED_OBJECT] = {0};
    #if 0  /// For CNN demo!
	URECT DispCord = {0, 0, 960, 240};
    #else
    URECT DispCord = {0, 0, 1920, 1080};
    #endif
    DISPLAYER_PARAM   DispLyr = {0} ;

    if (!g_hdmi_mode) {
            gDevHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
            if (gDevHDMIObj) {
                g_hdmi_mode = Dx_Getcaps(gDevHDMIObj, DISPLAY_CAPS_VIDMODE, 0);
            }
    }

    switch(g_hdmi_mode)
    {
        case HDMI_640X480P60:
            DispCord.w = 640;
            DispCord.h = 480;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 7;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 7;
            break;
        case HDMI_3840X2160P30:
            DispCord.w = 960;
            DispCord.h = 240;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 4;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 4;
            break;
        case HDMI_1920X1080I60:
            DispCord.w = 1920;
            DispCord.h = 1080;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 10;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 10;
            break;
        default:
            debug_msg("xExamOD_odgetresult Error: Unknown HDMI mode (%d)\r\n", g_hdmi_mode);
            break;
    }

	ODNumber = OD_GetResult(od_result, &DispCord);

#if (OD_PUSH_RESULT == ENABLE)
	{
		ISF_PORT *pDest= ImageUnit_In(&ISF_VdoEnc, ISF_IN1);
		if(ImageUnit_IsAllowPush(pDest) == ISF_OK) {
			ISF_DATA isf_data = {0};
			ISF_DATA* pData = &isf_data;
			UINT32 addr = 0;
			//new data
	    		pData->TimeStamp = HwClock_GetLongCounter();
			addr = ImageUnit_NewData(sizeof(RIFF_CHUNK)+sizeof(VIDENC_SMART_ROI_INFO), pData);
			if(addr != 0) {
				//assign data to "metadata"
				ISF_METADATA_BUF *pMetadata = (ISF_METADATA_BUF *)(pData->Desc);
				pMetadata->flag = MAKEFOURCC('M','E','T','A');
				pMetadata->addr = addr;

				//fill riff chunk header
				RIFF_CHUNK* pChunk = (RIFF_CHUNK*)pMetadata->addr;
				pChunk->fourcc = MAKEFOURCC('O','D','R','L');
				pChunk->size = sizeof(RIFF_CHUNK)+sizeof(VIDENC_SMART_ROI_INFO);
				//fill riff chunk data
				VIDENC_SMART_ROI_INFO* pInfo = (VIDENC_SMART_ROI_INFO*)(((UINT8*)pChunk)+(pChunk->size));
				pInfo->uiRoiNum = ODNumber;
				pInfo->uiRange = 64;
				memcpy(pInfo->RoiInfo, &DispCord, sizeof(OD_Result)*MAX_DETECTED_OBJECT);
		    		pInfo->TimeStamp = pData->TimeStamp;

		    		//push data
				ImageUnit_PushData(pDest, pData, 0);
			}
		}
	}
#endif
#if (OD_DRAW_RESULT == ENABLE)
	UINT32            i = 0;
	PDISP_OBJ         pDispObj = NULL;

	idec_setFDAllEn(0, 1);
	pDispObj = disp_getDisplayObject(DISP_1);
	if (ODNumber > 16) {
		ODNumber = 16;
	}
	for (i = 0; i < 16; i++) {
		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = FALSE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}

	for (i = 0; i < ODNumber; i++) {
		//DBG_DUMP("Object %d = [x=%d,y=%d,w=%d,h=%d]\r\n",i,od_result[i].x,od_result[i].y,od_result[i].w,od_result[i].h);
		DispLyr.SEL.SET_FDSIZE.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDSIZE.uiFDX = od_result[i].x;
		DispLyr.SEL.SET_FDSIZE.uiFDY = od_result[i].y;
		DispLyr.SEL.SET_FDSIZE.uiFDW = od_result[i].w;
		DispLyr.SEL.SET_FDSIZE.uiFDH = od_result[i].h;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDSIZE, &DispLyr);

		DispLyr.SEL.SET_FDCOLOR.FD_NUM = DISPFD_NUM0 << i;

		// Blue   : 0x1d/0xff/0x6b
		// White  : 0xff/0x80/0x80
		// Red    : 0x4c/0x54/0xff
		// Yellow : 0xff/0x00/0x94
		switch (od_result[i].class) {
		case 0:     // Bus (blue)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0x1d;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x6b;
			break;
		case 1:     // Car (white)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x80;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x80;
			break;
		case 2:     // Human (red)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0x4c;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x54;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0xff;
			break;
		default:    // Others (yellow)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x00;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x94;
			break;
		}
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDCOLOR, &DispLyr);

		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = TRUE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	pDispObj->load(TRUE);
#endif
}
*/
PHOTO_FUNC_INFO PhotoFuncInfo_mvod = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODProcess",
		Movie_ODInit,                       ///< init OD, only execute at first process
		Movie_ODProcess,                    ///< process OD
		NULL,                               ///< process end OD
		NULL,                               ///< exit fp, only execute at photo close
		OD_CalcBuffSize,                    ///< get working buffer OD
		NN_CalcCacheBuffSize,               ///< get working cache buffer OD
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2017/11/3#Ming Yang -end


//#NT#2017/09/27#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if (MOVIE_ODT_FUNC == ENABLE)

//MEM_RANGE odt_buf;
//MEM_RANGE odt_cachebuf;

void Movie_ODTInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	odt_init(buf, cachebuf);
	odt_lock(FALSE);
}



void Movie_ODTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	if (!odt_getinitstate()) {
		return;
	}

	odt_ot_process();

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}


void Movie_ODTODProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	if (!odt_getinitstate()) {
		return;
	}

	odt_od_process();


}



PHOTO_FUNC_INFO PhotoFuncInfo_mvodt = {
	{
		PHOTO_PRIORITY_H,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODTProcess",
		Movie_ODTInit,                       ///< init ODT, only execute at first process
		Movie_ODTProcess,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		odt_calcbuffsize,                    ///< get working buffer ODT
		odt_calccachebuffsize,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

PHOTO_FUNC_INFO PhotoFuncInfo_mvodtod = {
	{
		PHOTO_PRIORITY_H,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODTODProcess",
		NULL,                       ///< init ODT, only execute at first process
		Movie_ODTODProcess,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		NULL,                    ///< get working buffer ODT
		NULL,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2017/09/27#Bin Xiao -end


//#NT#2018/03/12#Dan Guo -begin
//#NT#Face recognition
#if (MOVIE_FR_FUNC == ENABLE)
//#if (MOVIE_FR_REG_FLOW == ENABLE)
MEM_RANGE g_FRInitBuf;
MEM_RANGE g_FRInitCacheBuf;
static BOOL g_bFRRegFlowEnable = FALSE;

static BOOL CmdFRReg_Flow(CHAR* strCmd)
{
	UINT32  bFRRegFlow = 0;
	sscanf_s(strCmd, "%d", &bFRRegFlow);
     if(bFRRegFlow) {
		g_bFRRegFlowEnable = TRUE;
		DBG_DUMP("FR reg flow start.\r\n");
		FR_reg_set(1);
		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

	}else {

		//re-init FR
		FR_Init(&g_FRInitBuf, &g_FRInitCacheBuf, 1,1);
		g_bFRRegFlowEnable = FALSE;
		FR_reg_set(0);
		DBG_DUMP("FR reg flow stop.\r\n");

	}
	return TRUE;
}
static BOOL CmdFRReg_Cap(CHAR* strCmd)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	static char name[32] = {0};
    UINT32 register_status = 0;
	if(FALSE == g_bFRRegFlowEnable) {
		DBG_ERR("run \"fr regflow 1\" first!\r\n");
		return TRUE;
	}
	sscanf_s(strCmd, "%s", name, sizeof(name));
	if(0 == strlen(name)) {
		DBG_ERR("no name!\r\n");
		return TRUE;
	}
	DBG_DUMP("FR reg name = %s\r\n",name);
	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0//dump register YUV
		char filename[64];
		IMG_BUF *pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
		snprintf(filename, sizeof(filename), "A:\\%s.yuv", name);
		if(GX_IMAGE_PIXEL_FMT_YUV422_PACKED == pImgBuf->PxlFmt) {
			DBG_DUMP("Save YUV422 to %s, %dx%d \r\n", filename,pImgBuf->Width,pImgBuf->Height);
		}else {
			DBG_DUMP("Save YUV420 to %s, %dx%d \r\n", filename,pImgBuf->Width,pImgBuf->Height);
		}
		GxImg_DumpBuf(filename, pImgBuf, GX_IMAGE_DUMP_ALL);
		#endif
		//FR_Register((IMG_BUF *)&isf_data.Desc[0], name);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

		register_status = FR_Register((IMG_BUF *)&isf_data.Desc[0], name);
		if(register_status == 0)
		{
			DBG_DUMP("FR_Register_OK\r\n");
		}
		else if(register_status == 1)
		{
			DBG_DUMP("no face\r\n");
		}
		else if(register_status == 2)
		{
			DBG_DUMP("more than one face ...\r\n");
		}


	}
	return TRUE;
}
SXCMD_BEGIN(fr, "FR command")
SXCMD_ITEM("regflow %", CmdFRReg_Flow, "FR register flow enable/disable.")
SXCMD_ITEM("regcap %", CmdFRReg_Cap, "FR register capture.")
SXCMD_END()
//#endif

void Movie_FRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//DBG_ERR("FR position 1 ###########\r\n");
   // debug_msg("####FR init\r\n");
    FR_Init(buf, cachebuf, 1,1);

//#if (MOVIE_FR_REG_FLOW == ENABLE)
		SxCmd_AddTable(fr);
		g_FRInitBuf.Addr = buf->Addr;
		g_FRInitBuf.Size = buf->Size;
		g_FRInitCacheBuf.Addr = cachebuf->Addr;
		g_FRInitCacheBuf.Size = cachebuf->Size;
		//DBG_DUMP("buf=0x%X,0x%X	cachebuf=0x%X,0x%X\r\n",g_FRInitBuf.Addr,g_FRInitBuf.Size,g_FRInitCacheBuf.Addr,g_FRInitCacheBuf.Size);
//#endif

}
void Movie_FRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

    ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;

//#if (MOVIE_FR_REG_FLOW == ENABLE)
	if(g_bFRRegFlowEnable) {
		return;
	}
//#endif

    static UINT32 is_first = 1;
	if ((is_first == 1) && ((INT32)FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0) == 1)) {
        is_first = 0;

	    fdcnn_netselect(FDCNN_METHOD_1);
		fdcnn_loadmodel();

		FR_NetSelect(0);
        FR_LoadModel(1);


	}

	if(is_first == 1) {
		return;
	}

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

    //debug_msg("####ready\r\n");

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		FR_recognition((IMG_BUF *)&isf_data.Desc[0]);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvfr = {
	{
		PHOTO_PRIORITY_L,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_FRProcess",
		Movie_FRInit,                       ///< init FR, only execute at first process
		Movie_FRProcess,                    ///< process FR
		NULL,                 ///< process end FR
		NULL,                      ///< exit fp, only execute at photo close
		FR_CalcBuffSize,                    ///< get working buffer FR
		FR_CalcCacheBuffSize,               ///< get working cache buffer FR
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2018/03/12#Dan Guo -end



//#NT#2018/01/29#Bin Xiao -begin
//#NT# Face Detection by CNN
#if (MOVIE_FDCNN_FUNC == ENABLE)
void Movie_FDCNNInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	fdcnn_lock(FALSE);
	fdcnn_init(buf, cachebuf);

}


void Movie_FDCNNProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;


    static UINT32 is_first = 1;
	if ((is_first == 1) && ((INT32)FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0) == 1)) {
        is_first = 0;

		fdcnn_netselect(FDCNN_METHOD_1);
		fdcnn_loadmodel();

	}

	if(is_first == 1) {
		return;
	}

	#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
	static INT32					regist_falg = ODT_STA_ERROR;
	static UINT32					fdcnn_rslt_loc;
	static UINT32					fdcnn_num;
	static fdcnn_result				fdcnn_rslts[FDCNN_MAX_OUTNUM];
	static odt_od_result 			fdcnn_odt_rslts[FDCNN_MAX_OUTNUM];

	static odt_process_buf_param 	fdcnn_odt_buf_param;
	static odt_rslt_inf				fdcnn_buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				fdcnn_buf2;
	static odt_od_result 			fdcnn_buf3[FDCNN_MAX_OUTNUM];

	fdcnn_odt_buf_param.detect_rslt_addr = (UINT32)fdcnn_buf3;
	fdcnn_odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&fdcnn_buf2);
	fdcnn_odt_buf_param.odt_rslt_buffer_addr = (UINT32)fdcnn_buf1;

	if (regist_falg == ODT_STA_ERROR) {
		regist_falg = odt_regist(&fdcnn_odt_buf_param);
		if (regist_falg == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 15, fdcnn_odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 1, fdcnn_odt_buf_param.process_id);
		}
	}
	#endif

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
		fdcnn_rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif

		#if 0
		UINT32 t1 = 0, t2 = 0;
		Perf_Open();
		t1 = Perf_GetCurrent();

		fdcnn_process((IMG_BUF *)&isf_data.Desc[0]);

		t2 = Perf_GetCurrent();
		Perf_Close();

		debug_msg("%d\r\n", t2-t1);
		#else
		fdcnn_process((IMG_BUF *)&isf_data.Desc[0]);
		#endif

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
	UINT32 i;
	URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
	fdcnn_num = fdcnn_getresults(fdcnn_rslts, &targetcoord);
	for(i = 0; i < fdcnn_num; i++)
	{
		fdcnn_odt_rslts[i].x = fdcnn_rslts[i].x;
		fdcnn_odt_rslts[i].y = fdcnn_rslts[i].y;
		fdcnn_odt_rslts[i].w = fdcnn_rslts[i].w;
		fdcnn_odt_rslts[i].h = fdcnn_rslts[i].h;
		fdcnn_odt_rslts[i].score = fdcnn_rslts[i].score;
		fdcnn_odt_rslts[i].class = fdcnn_rslts[i].class;
	}
	odt_setodrslt(fdcnn_num, fdcnn_odt_rslts, fdcnn_rslt_loc, fdcnn_odt_buf_param.process_id);
	#else
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
	#endif

}

PHOTO_FUNC_INFO PhotoFuncInfo_mvfdcnn = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_FDCNNProcess",
		Movie_FDCNNInit,                       ///< init FDCNN, only execute at first process
		Movie_FDCNNProcess,                    ///< process FDCNN
		NULL,                 ///< process end FDCNN
		NULL,                      ///< exit fp, only execute at photo close
		fdcnn_calcbuffsize,                    ///< get working buffer FDCNN
		fdcnn_calccachebuffsize,               ///< get working cache buffer FDCNN
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2018/01/29#Bin Xiao -end

//#NT#2017/12/04#WY -begin
//#NT#
#if (MOVIE_ODP_FUNC == ENABLE)
void Movie_ODPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    NN_NetSelect(CNN_NODNETPIPE);
	NN_Init(buf, cachebuf, 1);
}

void Movie_ODPNet1Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;

    while(((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 0))
    {
        //debug_msg("FileSys_Init not ready\r\n");
    }
    static UINT32 is_first_time = 1;
    if ((is_first_time == 1) && ((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 1)) {
        is_first_time = 0;
        debug_msg("Load model\r\n");
        NN_LoadModel();
	}

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15); //ISF_OUT15  KC_old_version:ISF_OUT16
#endif

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
        NN_Process((IMG_BUF *)&isf_data.Desc[0], 1);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}


void Movie_ODPNet2Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    static UINT32 is_first_time2 = 1;
	if ((is_first_time2 == 1) && ((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 1)) {
        is_first_time2 = 0;
		NN_NetSelect(CNN_NODNETPIPE);
	}
    while(((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 0))
    {
        //debug_msg("FileSys_Init not ready\r\n");
    }
	NN_Process(NULL, 2);

    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvodpnet1 = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODPNet1Process",
		Movie_ODPInit,                       ///< init ODT, only execute at first process
		Movie_ODPNet1Process,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		ODP_CalcBuffSize,                    ///< get working buffer ODT
		NN_CalcCacheBuffSize,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

PHOTO_FUNC_INFO PhotoFuncInfo_mvodpnet2 = {
	{
		PHOTO_PRIORITY_L,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODPNet2Process",
		NULL,                       ///< init ODT, only execute at first process
		Movie_ODPNet2Process,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		NULL,                    ///< get working buffer ODT
		NULL,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2017/12/04#WY -end

//#NT#2018/06/08#WY -begin
//#NT# NDONet pipe test flow
#if (MOVIE_ODP_TEST_FUNC == ENABLE)
void Movie_ODPTestInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    NN_NetSelect(CNN_NODNETPIPE_TEST);
	NN_Init(buf, cachebuf, 1);
}

void Movie_ODPTestProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    while(((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 0))
    {
        //debug_msg("FileSys_Init not ready\r\n");
    }
    static UINT32 is_first_time = 1;
	if ((is_first_time == 1) && ((INT32)GxStrg_GetDeviceCtrl(0, CARD_INSERT) == 1)) {
        is_first_time = 0;
		NN_NetSelect(CNN_NODNETPIPE_TEST);
        debug_msg("Load model\r\n");
        NN_LoadModel();
        NN_Process(NULL, 2);
	}

    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvodptest = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODPTestProcess",
		Movie_ODPTestInit,                       ///< init ODT, only execute at first process
		Movie_ODPTestProcess,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		ODP_CalcBuffSize,                    ///< get working buffer ODT
		NN_CalcCacheBuffSize,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2018/06/08#WY -end


#if MOVIE_FD_FUNC_
void Movie_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//#if (_FPGA_EMULATION_ == ENABLE)
	FD_Config(FD_CFG_SCALE_ENG, FD_SCALE_ENG2);
	//#endif
	// Init FD buffer, set max face number to 10, and process rate as 3
	FD_Init(buf, cachebuf, MAX_FDNUM, MOVIE_FD_FACESCALE, 3);
}

void Movie_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	//UINT32           ImageRatioIdx;

	//ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	//ImageRatioIdx = IMAGERATIO_16_9;
	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0
		{
			static UINT32 isdump = 0;
			PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data.Desc[0];
			if (!isdump)
			{
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				GxImg_DumpBuf("A:\\fd.yuv", pImgBuf, GX_IMAGE_DUMP_ALL);
				isdump = 1;
			}
		}
		#endif
		//FD_Process((IMG_BUF *)&isf_data.Desc[0], Get_FDImageRatioValue(ImageRatioIdx));
		FD_Process((IMG_BUF *)&isf_data.Desc[0], FD_IMAGERATIO_16_9);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}

UINT32 Movie_FDCalcBuffSize(void)
{
	FD_Config(FD_CFG_FACE_SCALE, MOVIE_FD_FACESCALE);
	return FD_CalcBuffSize();
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvfd = {
	{
		PHOTO_PRIORITY_M,       ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_FDProcess",
		Movie_FDInit,           ///< init fp, only execute at first process
		Movie_FDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Movie_FDCalcBuffSize,   ///< get working buffer fp
		FD_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif



// #NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
void Movie_CvHostExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (g_CvHostExDone != TRUE) {
		debug_msg("\n\r************************\n");
		debug_msg("\r*  Run CvHost Example  *\n");
		debug_msg("\r************************\n");
#if _CVHOST_EX_ENDSP_
		SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
#endif

		UIAppMovie_CvHostEx_Opening(buf);
		UIAppMovie_CvHostEx_Sobel3x3(buf);
		UIAppMovie_CvHostEx_Gaussian3X3(buf);

		g_CvHostExDone = TRUE;

#if _CVHOST_EX_ENDSP_
		SxCmd_DoCommand("dsp close");
#endif
		debug_msg("\r** CvHost Example End **\n");
	}
}

UINT32 Movie_CvHostExCalcBufSize(void)
{
	UINT32 ImgRowsMax = 1080;
	UINT32 ImgColsMax = 1920;
	UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
	UINT32 CvHostBufSize = CvHost_GetWorkMemSize();

	return ImgSizeMax * 3 + CvHostBufSize;  // input gray, output gray and CvHost working buffer
}

PHOTO_FUNC_INFO MovieFuncInfo_CvHostEx = {
	{
		PHOTO_PRIORITY_M,                     ///< function hook process Id.
		IPL_PATH_1,                       ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"Movie_CvHostExProcess",
		NULL,
		Movie_CvHostExProcess,
		NULL,
		NULL,                      ///< exit fp, only execute at photo close
		Movie_CvHostExCalcBufSize,
		NULL,
	},
	NULL
};
#endif
// #NT#2015/09/17#JS Liu - end


//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
void Movie_NvtCvExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (g_NvtCvExDone != TRUE) {
		debug_msg("\n\r************************\n");
		debug_msg("\r*  Run NvtCv Example   *\n");
		debug_msg("\r************************\n");
#if _NVTCV_EX_ENDSP_
		SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
#endif

		UIAppMovie_NvtCvEx_test(buf);

		g_NvtCvExDone = TRUE;

#if _NVTCV_EX_ENDSP_
		SxCmd_DoCommand("dsp close");
#endif
		debug_msg("\r** NvtCv Example End **\n");
	}
}

UINT32 Movie_NvtCvExCalcBufSize(void)
{
	UINT32 ImgRowsMax = 1080;
	UINT32 ImgColsMax = 1920;
	UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
	UINT32 NvtCvBufSize = NvtCv_GetWrkMemSize();

	return ImgSizeMax * 3 + NvtCvBufSize;  // input gray, output gray and NvtCv working buffer
}

PHOTO_FUNC_INFO MovieFuncInfo_NvtCvEx = {
	{
		PHOTO_PRIORITY_M,                     ///< function hook process Id.
		IPL_PATH_1,                       ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"Movie_NvtCvExProcess",
		NULL,
		Movie_NvtCvExProcess,
		NULL,
		NULL,                      ///< exit fp, only execute at photo close
		Movie_NvtCvExCalcBufSize,
		NULL,
	},
	NULL
};
#endif // _NVTCV_EX_
//#NT#2016/05/31#Omega Yu -end

#if (_ADAS_FUNC_ == ENABLE)
void Photo_ADASInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_MidPrioAppsInit(buf, cachebuf, 1, 0);
}

void Photo_ADASProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA isf_data;
	ISF_UNIT *pUnit;
	ISF_PORT *pSrc;

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH4));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);
#endif

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0   // For dump file test
		static UINT32 frame_cnt = 0;
		char filename[50];
		FST_FILE pFile = NULL;
		UINT32 addr = 0, size = 0;
		IMG_BUF *pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			DBG_DUMP("pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
			DBG_DUMP("pImgBuf PxlAddr = 0x%08X 0x%08X 0x%08X, LineOffset = %lu %lu %lu, metadata addr = 0x%08X, size = 0x%08X\r\n",
			pImgBuf->PxlAddr[0], pImgBuf->PxlAddr[1], pImgBuf->PxlAddr[2], pImgBuf->LineOffset[0], pImgBuf->LineOffset[1], pImgBuf->LineOffset[2], pImgBuf->Metadata.Addr, pImgBuf->Metadata.Size);

			pFile = FileSys_OpenFile(filename, FST_OPEN_WRITE|FST_OPEN_ALWAYS);
			if (pFile) {
				addr = pImgBuf->PxlAddr[0];
				size = pImgBuf->Width * pImgBuf->Height * 2;
				FileSys_WriteFile(pFile, (UINT8 *)addr, &size, 0, NULL);
				FileSys_CloseFile(pFile);
				DBG_DUMP("Save file %s\r\n", filename);
			}
		}
		#endif  // #if 0   // For dump file test

		// Copy data first
		ADAS_MidPrioAppsCopySrcDataInt((IMG_BUF *)&isf_data.Desc[0]);

		// Relese data buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

		// Process data
		ADAS_MidPrioAppsProcess();
	}
}

void Photo_ADASProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	URECT DispCord = {0, 0, OSD_W, OSD_H};
	ADAS_APPS_RESULT_INFO *pAdasRlt = alg_MovieExe_GetAdasRltOSD();

	if (ADAS_GetAppsStatus() == FALSE) {
		return;
	}

#if (_AUTOVP_FUNC_ == ENABLE)
	URECT ADASCropWin = {0};
	// Get crop window setting
	if (ADAS_GetNewCropWindow(&ADASCropWin) == TRUE) {
		Ux_PostEvent(NVTEVT_CB_ADAS_SETCROPWIN, 2, ((ADASCropWin.x << 16) | ADASCropWin.y), ((ADASCropWin.w << 16) | ADASCropWin.h));
	}
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)

	// Get FC/LD Result
	DispCord = gMovieOsdDispCord;
	if (ADAS_GetLdFcRslt(pAdasRlt, &DispCord) == ADAS_STA_OK) {
#if (GPS_FUNCTION == ENABLE)
		if (((g_GPSLinked == FALSE) || (g_GPSStatus == FALSE) || (g_CurSpeed > 50)) && (ADAS_GetStopDectRslt() == STOPDECT_STOP_FALSE))
#else   // #if (GPS_FUNCTION == ENABLE)
		if (ADAS_GetStopDectRslt() == STOPDECT_STOP_FALSE)
#endif  // #if (GPS_FUNCTION == ENABLE)
		{
			if ((pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) &&
				((pAdasRlt->LdwsRsltInfo.DepartureDirSound == LDWS_DEPARTURE_LEFT) || (pAdasRlt->LdwsRsltInfo.DepartureDirSound == LDWS_DEPARTURE_RIGHT))) {
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_LD);
			} else if ((pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) && (pAdasRlt->FcwsRsltInfo.FCSoundWarning == TRUE)) {
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_FC);
			}
		}
	}
#if (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
	MOVIE_RECODE_INFO *pRecInfo = (MOVIE_RECODE_INFO *)ImageApp_Movie_GetConfig(_CFG_REC_ID_1);
	if (pRecInfo) {
		DispCord.x = 0;
		DispCord.y = 0;
		DispCord.w = pRecInfo->size.w;
		DispCord.h = pRecInfo->size.h;
		pAdasRlt = alg_MovieExe_GetAdasRltVideo();
		ADAS_GetLdFcRslt(pAdasRlt, &DispCord);
	}
#endif

	// Get Log Data
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	UINT32 LogSize = 0;
	UINT32 LogBuf = 0;
	LogSize = ADAS_GetLdFcLog(&LogBuf);
	if (LogSize) {
		mp4log_AddLog(DbgLogFCLDID, (char *)LogBuf, LogSize);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

#if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
	if (1)
#else   // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
	if (ADAS_IsAdasDebug())
#endif  // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
	{
		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_adas = {
	{
		PHOTO_PRIORITY_M,                           ///< function hook process Id.
		IPL_PATH_1,                                 ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,                      ///< isr tigger event
		"Photo_ADASProcess",
		Photo_ADASInit,                             ///< init fp, only execute at first process
		Photo_ADASProcess,                          ///< process fp
		Photo_ADASProcessEnd,                       ///< process end fp
		NULL,                                       ///< exit fp, only execute at photo close
		ADAS_CalcMidPrioAppsBuffSize,               ///< get working buffer fp
		NULL,                                       ///< get working cache buffer fp
	},
	NULL                                            ///< point next Function Obj, last Function must be set NULL
};

#if (_AUTOVP_FUNC_ == ENABLE)
void Photo_AutoVPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_LowPrioAppsInit(buf, cachebuf, 1, 1 * 1000); // 1s
}

void Photo_AutoVPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA isf_data;
	ISF_UNIT *pUnit;
	ISF_PORT *pSrc;

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0   // For dump file test
		static UINT32    frame_cnt = 0;
		char             filename[50];
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
		#endif  // #if 0   // For dump file test

        //ADAS_AppsSetAutoVPEanble(TRUE);

		// Copy data first
		ADAS_LowPrioAppsCopySrcDataInt((IMG_BUF *)&isf_data.Desc[0]);

		// Relese data buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

		// Process data
		ADAS_LowPrioAppsProcess();
	}
}

void Photo_AutoVPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (ADAS_GetAppsStatus() == FALSE) {
		return;
	}
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	UINT32 LogSize = 0;
	UINT32 LogBuf = 0;
	LogSize = ADAS_GetAutoVPLog(&LogBuf);
	if (LogSize) {
		mp4log_AddLog(DbgLogAutoVPID, (char *)LogBuf, LogSize);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

PHOTO_FUNC_INFO PhotoFuncInfo_autoVP = {
	{
		PHOTO_PRIORITY_L,                           ///< function hook process Id.
		IPL_PATH_1,                                 ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,                      ///< isr tigger event
		"Photo_AutoVPProcess",
		Photo_AutoVPInit,                           ///< init fp, only execute at first process
		Photo_AutoVPProcess,                        ///< process fp
		Photo_AutoVPProcessEnd,                     ///< process end fp
		NULL,                                       ///< exit fp, only execute at photo close
		ADAS_CalcLowPrioAppsBuffSize,               ///< get working buffer fp
		ADAS_CalcLowPrioAppsCacheBuffSize,          ///< get working cache buffer fp
	},
	NULL                                            ///< point next Function Obj, last Function must be set NULL
};
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)

#if _TODO   // [ADAS_TODO] (_SNG_FUNC_ == ENABLE)
void Photo_SnGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_HighPrioAppsInit(buf, cachebuf, 1, 66); // 66ms
}

void Photo_SnGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_HighPrioAppsProcess();
}

void Photo_SnGProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	URECT DispCord = {0, 0, OSD_W, OSD_H};
	ADAS_APPS_RESULT_INFO *pAdasRlt = alg_MovieExe_GetAdasRltOSD();

	if (ADAS_GetAppsStatus() == FALSE) {
		return;
	}

	// Get SnG Result
	DispCord = gMovieOsdDispCord;
	if (ADAS_GetSnGRslt(pAdasRlt, &DispCord) == ADAS_STA_OK) {

#if (GPS_FUNCTION == ENABLE)
		if ((g_GPSLinked == FALSE) || (g_GPSStatus == FALSE) || (g_CurSpeed <= 50)) {
#endif  // #if (GPS_FUNCTION == ENABLE)
			if (pAdasRlt->SnGInfo.uiGoStatus == 1) {
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_GO);
			}
			if (pAdasRlt->SnGInfo.uiStopStatus == 3) {
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_STOP);
			}
#if (GPS_FUNCTION == ENABLE)
		}
#endif  // #if (GPS_FUNCTION == ENABLE)
	}

#if _TODO   // [ADAS_TODO] (_DRAW_SNG_ROI_ == ENABLE)
	ADAS_SNG_ROI_INFO *pSnGROI = NULL;

	pSnGROI = alg_MovieExe_GetSnGROIOSD();
	ADAS_GetSnGRoi(pSnGROI, &DispCord);

#if _TODO   // [ADAS_TODO] (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
	IMG_BUF InputImg = {0};
	ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT1), (ISF_DATA *)&InputImg, 0);
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = InputImg.Width;
	DispCord.h = (InputImg.Height == 1088) ? 1080 : (InputImg.Height);

	pAdasRlt = alg_MovieExe_GetAdasRltVideo();
	ADAS_GetSnGRslt(pAdasRlt, &DispCord);

	pSnGROI = alg_MovieExe_GetSnGROIVideo();
	ADAS_GetSnGRoi(pSnGROI, &DispCord);
#endif
#endif

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	UINT32 LogSize = 0;
	UINT32 LogBuf = 0;
	LogSize = ADAS_GetSnGLog(&LogBuf);
	if (LogSize) {
		mp4log_AddLog(DbgLogSnGID, (char *)LogBuf, LogSize);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

PHOTO_FUNC_INFO PhotoFuncInfo_SnG = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Photo_SnGProcess",
		Photo_SnGInit,           ///< init fp, only execute at first process
		Photo_SnGProcess,        ///< process fp
		Photo_SnGProcessEnd,     ///< process end fp
		NULL,                    ///< exit fp, only execute at photo close
		ADAS_CalcHighPrioAppsBuffSize,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif  // #if (_SNG_FUNC_ == ENABLE)
#endif  // #if (_ADAS_FUNC_ == ENABLE)


#if MOVIE_DIS_MV_FUNC || MOVIE_FTG_FUNC || MOVIE_SCD_FUNC || MOVIE_ODT_FUNC   // [ADAS_TODO] || _ADAS_FUNC_
//#if MOVIE_DIS || MOVIE_FTG_FUNC || MOVIE_SCD_FUNC || MOVIE_ODT_FUNC   // [ADAS_TODO] || _ADAS_FUNC_
#include "af_api.h"
#include "ipl_header_infor_raw.h"
#include "dis_alg.h"
#include "ipl_cmd.h"
#include "dis_api.h"

void Photo_DIS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	dis_initialize(buf, cachebuf);

    //dis_setDisViewRatio(900); --> It should be set befor auto-gen dzoom table

    DIS_InstallCmd();

#if _TODO   // [ADAS_TODO] (_ADAS_FUNC_ == ENABLE)
	//Disable FW/ROI dis to decrease CPU usage.
	DISALG_CFGINFO info;
	info.iCfgVal = FALSE;
	dis_SetConfig(DISALG_CFG_GLOBALMOTEN, &info);
	dis_SetConfig(DISALG_CFG_ROIMOTEN, &info);
#endif

	//AF_VA_Setup(0);
}

//#NT#2017/10/02#Harry_Tsai -begin
//#NT rearrange DIS structure
void Photo_DIS(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

#if (IPCAM_DIS_COMP_FUNC == ENABLE)
	dis_compensation(buf,cachebuf,FALSE);
#else
    dis_compensation(buf,cachebuf,TRUE);
#endif

#if (MOVIE_FTG_FUNC == ENABLE)
    //dis_compensation(buf,cachebuf,TRUE);
    if (!gFTGLock) {
        ftg_savedisrslt();
    }
#elif (MOVIE_ODT_FUNC == ENABLE)
	//dis_compensation(buf,cachebuf,TRUE);
	odt_savedisrslt();
#else
    //dis_compensation(buf,cachebuf,FALSE);
#endif
}
//#NT#2017/10/02#Harry_Tsai -end

//
PHOTO_FUNC_INFO PhotoFuncInfo_dis = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
//#NT#2016/05/04#Harry_Tsai -begin
//#NT# Change to VD
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"Photo_DIS",
//#NT#2016/05/04#Harry_Tsai -end
		Photo_DIS_Init,                   ///< init fp, only execute at first process
		Photo_DIS,           ///< process fp
		NULL,                   ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		dis_GetPrvMaxBuffer,        ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#else
PHOTO_FUNC_INFO PhotoFuncInfo_dis = {0};
#endif

//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
void Movie_OTInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
//#NT#2017/06/16#MB Wang -begin
//#NT#Set Alg Version and Feature Type for OT
    OT_SetConfig(OT_CFG_ALG_VERSION,1);  //0: single scale, 1: multiscale.
    OT_SetConfig(OT_CFG_FEATURE_TYPE,0);  // feature type can be set 0 or 1.
//#NT#2017/06/16#MB Wang -end
	NvtOt_Init(buf);
}
void Movie_OTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	UINT32           ImageRatioIdx;
	OT_UIMGSIZE      InputImgSize;
	IMG_BUF*          IMG_TEMP;

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

	if (ImageUnit_IsAllowPull(pSrc) && (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK)) {

		IMG_TEMP = (IMG_BUF *)&isf_data.Desc[0];

		InputImgSize.width = IMG_TEMP->Width;
		InputImgSize.height = IMG_TEMP->Height;
		InputImgSize.lineoffset = IMG_TEMP->LineOffset[0];
		InputImgSize.addr = IMG_TEMP->PxlAddr[0];
		InputImgSize.imgRatio = ImageRatioIdx;

		if(!OT_PROCESS_STA()){

			//OT_SetDSP(NVTCV_SENDTO_DSP1);

			OT_SetImgSize( &InputImgSize);  //set should be done for every frame,addr maybe differ.

			NvtCv_ot_process();
		}

		ImageUnit_ReleaseData((ISF_DATA*)&isf_data);

		//#NT#2016/06/08#Lincy Lin -begin
		//#NT#Implement generic OSD and video drawing mechanism for ALG function
		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
		//#NT#2016/06/08#Lincy Lin -end
	}
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvot = {
	{
		PHOTO_PRIORITY_SH,          ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_OTProcess",
		Movie_OTInit,           ///< init fp, only execute at first process
		Movie_OTProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		OT_CalcBuffSize,        ///< get working buffer fp
		NULL,//OT_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
void Movie_DISDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	DisDsp_Init(buf);
}

void Movie_DISDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	IMG_BUF   InputImg = {0};
	ISF_PORT *pSrc = ImageUnit_Out(ISF_IPL(0), ISF_OUT3);
	DIS_DSP_ROIGMV_OUT roiOut;
	UIMGSIZE InputImgSize = {0};

	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT3), (ISF_DATA *)&InputImg, 0);

		InputImgSize.width = InputImg.Width;
		InputImgSize.height = InputImg.Height;
		InputImgSize.lineoffset = InputImg.LineOffset[0];
		InputImgSize.addr = InputImg.PxlAddr[0];

		DisDsp_ImgSize_SetInputInfo(&InputImgSize);
		NvtCv_disdsp_process();

		roiOut = DisDsp_Roi_GetResult();
		DBG_ERR("x = %d\t y = %d\t valid = %d\n\r", roiOut.vector.iX, roiOut.vector.iY, roiOut.bValidVec);
	}
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvdisdsp = {
	{
		PHOTO_PRIORITY_SH,          ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_DISDSPProcess",
		Movie_DISDSPInit,       ///< init fp, only execute at first process
		Movie_DISDSPProcess,    ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		DisDsp_CalcBuffSize,    ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2016/05/20#Yang Jin -end



//#NT#2016/11/16#ML Cui -begin
//#NT# The FD for Driver Drowsiness Detection(DDDFD)
#if MOVIE_DDD_FD_FUNC
#include "DDD_fd.h"

void Movie_DDDFdInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//GET_EXTERN_STU(FD_MODEL_NAME).pfInit(buf, cachebuf, 3);
	debug_msg("DDDFD Init\r\n");
	FD_FUNC(DDDFd)->pfInit(buf, cachebuf, 1);
}

void Movie_DDDFdProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF InputImg = {0};
	UINT32 myDualCam;
	ISF_PORT *pSrc;
	ISF_UNIT *pUnit;

	static UINT32 index = 0;

	if (index++ % 3 != 0) {
		return;
	}

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == SENSOR_1) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#elif (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == SENSOR_1 || System_GetEnableSensor() == SENSOR_2) {
		//only enable one sensor
		if (System_GetEnableSensor() == SENSOR_1) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		//enable dual sensors
		myDualCam = DUALCAM_BEHIND;
	}
#else
#error "Undefined case"
#endif

	if (myDualCam == DUALCAM_FRONT) {
		pUnit = ISF_IPL[0];
	} else {
		pUnit = ISF_IPL[1];
	}

	pSrc = ImageUnit_Out(pUnit, ISF_OUT4);




	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		//        ImageUnit_SetParam(pUnit, IMAGEPIPE_PARAM_PULLBUF_OUT4, (UINT32)&InputImg);

		ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT4), (ISF_DATA *)&InputImg, 0);

		FD_FUNC(DDDFd)->pfProcess(&InputImg);

	}


}

static UINT32 Movie_DDDFdGetBuf(VOID)
{
	return FD_FUNC(DDDFd)->pfCalcBuffSize();
}

static UINT32 Movie_DDDFdGetCache(VOID)
{
	return FD_FUNC(DDDFd)->pfCalcCacheBuffSize();
}

static VOID Movie_DDDFdEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvdddfd = {
	{
		PHOTO_PRIORITY_M,                 ///< function hook process Id.
		IPL_PATH_1,                   ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,      ///< isr tigger event
		"Movie_DDDFdProcess",
		Movie_DDDFdInit,           ///< init tsr, only execute at first process
		Movie_DDDFdProcess,        ///< process tsr
		Movie_DDDFdEnd,            ///< process end tsr
		NULL,                      ///< exit fp, only execute at photo close
		Movie_DDDFdGetBuf,       ///< get working buffer tsr
		Movie_DDDFdGetCache,  ///< get working cache buffer tsr
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/11/16#ML Cui -end


//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#define DDD_USING_FD_OR_NOT DISABLE
#if DDD_USING_FD_OR_NOT
#include "NvtFd.h"
#include "fde_lib.h"
static UINT32 g_pFdHandleBuf = 0;
#endif
#if !(MOVIE_FD_FUNC_ || MOVIE_DDD_FD_FUNC)
#error "DDD need enable MOVIE_FD_FUNC_ or MOVIE_DDD_FD_FUNC"
#endif

#include "DDD_fd.h"

static INT32 Movie_DDDGetFdRslt(const IMG_BUF *pImgBuf, RECT *pFdRect)
{
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
	INT32 iFaceNum;
#if DDD_USING_FD_OR_NOT
	if (pFdRect->width != 0 && g_pFdHandleBuf) {
		FACE face = {0};

		face.uiStx = pFdRect->x;
		face.uiSty = pFdRect->y;
		face.uiWidth = pFdRect->width;
		face.uiHeight = pFdRect->height;
		face.uiTrgIdx = FDE_R000_P000;

		UINT32 t1 = Perf_GetCurrent();
		iFaceNum = NvtFD_DetectFaceOrNot(pImgBuf, &face, 1, g_pFdHandleBuf);

		if (iFaceNum) {
//            debug_msg("Front time : %d\r\n", Perf_GetCurrent() - t1);
			return iFaceNum;
		} else {
			INT32 iShake = DDD_GetPoseAngleShake();
			//The FD's Left and Right is opposite to DDD's
			if (iShake > 200) {
				face.uiTrgIdx = FDE_R000_PR90;
			} else if (iShake < -200) {
				face.uiTrgIdx = FDE_R000_PL90;
			}
			iFaceNum = NvtFD_DetectFaceOrNot(pImgBuf, &face, 1, g_pFdHandleBuf);
			if (iFaceNum) {

				debug_msg("Profile %d time : %d\r\n", face.uiTrgIdx, Perf_GetCurrent() - t1);
				return iFaceNum;
			}

		}
		debug_msg("recall FD wast time : %d \r\n");
	}

#endif

#if MOVIE_DDD_FD_FUNC
	URECT faceRslt;
	iFaceNum = FD_FUNC(DDDFd)->pfGetFDResult(pImgBuf, &faceRslt);

	if (iFaceNum > 0) {
		pFdRect->x = faceRslt.x;
		pFdRect->y = faceRslt.y;
		pFdRect->width = faceRslt.w;
		pFdRect->height = faceRslt.h;
	}
#else
	iFaceNum = FD_GetRsltFaceNum();
	if (iFaceNum >= 0) {
		static FD_FACE faceRslt[FD_NUM_MAX] = {0};
#if 1
		URECT DispCord;
		DispCord.x = 0;
		DispCord.y = 0;
		DispCord.w = pImgBuf->Width;
		DispCord.h = pImgBuf->Height;
		FD_GetRsltFace(faceRslt, &DispCord);
		pFdRect->height = pFdRect->width = MAX(faceRslt[0].w, faceRslt[0].h);
		pFdRect->x = faceRslt[0].x + faceRslt[0].w / 2 - pFdRect->width / 2;
		pFdRect->y = faceRslt[0].y + faceRslt[0].h / 2 - pFdRect->height / 2;
#else

		FD_GetRsltFace(faceRslt, 0);
		pFdRect->x = faceRslt[0].x * pImgBuf->Width / 640;
		pFdRect->y = faceRslt[0].y * pImgBuf->Height / 360;
		pFdRect->width = MAX(faceRslt[0].w * pImgBuf->Width / 640,
							 faceRslt[0].h * pImgBuf->Height / 360);
		pFdRect->height = pFdRect->width;
#endif
	}
#endif



	return iFaceNum;
}
//#NT#2016/11/10#Brain Yen -begin
//#NT#Add for save DDD MODEL in pstore
#if DDD_MODEL_IN_PSTORE
UINT32 Movie_DDDCheckPSFormat(void)
{
#define SETTING_SIZE 30
#define FORMAT_CHK_STRING  "format="

	UINT32      uiReadSize = SETTING_SIZE;
	UINT32      uiUpdateChk;
	CHAR        cDataBuf[SETTING_SIZE] = {0}, *pCh;
	FST_FILE    pFile;

	pFile = FileSys_OpenFile("A:\\ddd_psformat", FST_OPEN_READ);
	if (pFile) {
		FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
		FileSys_CloseFile(pFile);
		//DBG_DUMP("Read size %d\r\n", uiReadSize);
		if (uiReadSize) {
			pCh = strstr(cDataBuf, FORMAT_CHK_STRING);
			sscanf(pCh + sizeof(FORMAT_CHK_STRING) - 1, "%d", &uiUpdateChk);
			DBG_DUMP("Movie_DDDCheckPSFormat FormatChk = %d, size %d\r\n", uiUpdateChk, sizeof(FORMAT_CHK_STRING));
			return uiUpdateChk;
		}
	} else {
		//DBG_ERR("Open ddd_psformat check file error!\r\n");
	}
	return 0;

}
UINT32 Movie_DDDCheckDBUpdate(void)
{
#define SETTING_SIZE 30
#define UPDATE_CHK_STRING  "update="

	UINT32      uiReadSize = SETTING_SIZE;
	UINT32      uiUpdateChk;
	CHAR        cDataBuf[SETTING_SIZE] = {0}, *pCh;
	FST_FILE    pFile;

	pFile = FileSys_OpenFile("A:\\ddd_update", FST_OPEN_READ);
	if (pFile) {
		FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
		FileSys_CloseFile(pFile);
		//DBG_DUMP("Read size %d\r\n", uiReadSize);
		if (uiReadSize) {
			pCh = strstr(cDataBuf, UPDATE_CHK_STRING);
			sscanf(pCh + sizeof(UPDATE_CHK_STRING) - 1, "%d", &uiUpdateChk);
			DBG_DUMP("Movie_DDDCheckDBUpdate UpdateChk = %d, size %d\r\n", uiUpdateChk, sizeof(UPDATE_CHK_STRING));
			return uiUpdateChk;
		}
	} else {
		//DBG_ERR("Open ddd_update check file error!\r\n");
	}
	return 0;

}
#endif
//#NT#2016/11/10#Brain Yen -end
UINT32 Movie_DDDGetDBFileSizeFrCard(void)
{
	FST_FILE filehdl = NULL;
	UINT32 ui32FileSize = 0;

	CHAR *pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
	filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
	if (!filehdl) {
		CHAR *pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
		DBG_DUMP("DDD load model from SD-card!\r\n");
		filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
	}

	if (filehdl) {
		// Get file size info
		FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
		ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
		//DBG_DUMP("ui32FileSize=0x%x\r\n",ui32FileSize);
		FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);

		FileSys_CloseFile(filehdl);
	} else {
		DBG_ERR("DDD_Init Failed: load model failed!\r\n");
		return 0;
	}
	return ui32FileSize;
}
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
UINT32 Movie_DDDGetDBFileSize(void)
{
	UINT32 ui32FileSize = 0;
#if DDD_MODEL_IN_PSTORE
	PPSTORE_SECTION_HANDLE  pSection;
	UINT32  result;
	if ((pSection = PStore_OpenSection(PS_DDD_MODEL1_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
		result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
		DBG_IND("result=%d, ui32FileSize=0x%x\r\n", result, ui32FileSize);
		if (ui32FileSize == 0) {
			DBG_ERR("PStore Save default DDD Model Size\r\n");
			ui32FileSize = Movie_DDDGetDBFileSizeFrCard();
			DBG_ERR("ui32FileSize=%d\r\n", ui32FileSize);
			PStore_WriteSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
			PStore_CloseSection(pSection);
		} else {
			DBG_DUMP("PStore Load DDD Model Size OK\r\n");
			PStore_CloseSection(pSection);
		}
	} else {
		DBG_ERR("PStore Load DDD Model Size Fail!\r\n");
		return 0;
	}
#else
	ui32FileSize = Movie_DDDGetDBFileSizeFrCard();
#endif
	return ui32FileSize;
}
//#NT#2016/05/11#Isiah Chang -end
void Movie_DDDGetDBFile(UINT8 *pBinaryMap, UINT32 *uiBuflen)
{
	//load Landmark Model
	FST_FILE filehdl = NULL;

	CHAR *pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
	filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
	if (!filehdl) {
		CHAR *pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
		DBG_DUMP("DDD load model from SD-card!\r\n");
		filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
	}

	if (filehdl) {
		FileSys_ReadFile(filehdl, (UINT8 *)pBinaryMap,
						 uiBuflen, 0, NULL);
		FileSys_CloseFile(filehdl);
	} else {
		DBG_ERR("DDD_Init Failed: load model failed!\r\n");
		return;
	}
}
void Movie_DDDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
//#NT#2016/05/04#ML Cui -begin
//#NT# DDD will load model from an addr that generate by system.

#if 1
#if (DDD_MODEL_IN_PSTORE)
	if (Movie_DDDCheckPSFormat() == 1) {
		DBG_ERR("PStore format!\r\n");
		System_PS_Format();
	}
#endif
	ER bRetval;
	UINT32 pBinaryMap = buf->Addr;
	UINT32 uiDBFileSize = Movie_DDDGetDBFileSize();
	UINT32 uiBuflen = ALIGN_CEIL_16(uiDBFileSize);
#if (DDD_MODEL_IN_PSTORE)
	UINT32 uiUpdateDBChk = Movie_DDDCheckDBUpdate();
#endif
	if (uiDBFileSize > DDD_MAX_DB_FILE_SIZE) {
		DBG_ERR("DDD DB file size is too large!\r\n");
		return;
	}

	pBinaryMap = ALIGN_CEIL_4(pBinaryMap);//Input address must algin words
	if (buf->Size < uiBuflen + 4) {
		DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
	}
//#NT#2016/11/10#Brain Yen -begin
//#NT#Add for save DDD MODEL in pstore
#if (DDD_MODEL_IN_PSTORE==DISABLE)
	//init Model
	{
		//load Landmark Model
		FST_FILE filehdl = NULL;

		CHAR *pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
		filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
		if (!filehdl) {
			CHAR *pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
			DBG_DUMP("DDD load model from SD-card!\r\n");
			filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
		}

		if (filehdl) {
			FileSys_ReadFile(filehdl, (UINT8 *)pBinaryMap,
							 &uiBuflen, 0, NULL);
			FileSys_CloseFile(filehdl);
		} else {
			DBG_ERR("DDD_Init Failed: load model failed!\r\n");
			return;
		}
	}
#else
	PPSTORE_SECTION_HANDLE  pSection;
	UINT32  result;

	uiBuflen = ALIGN_CEIL_16(uiDBFileSize);
	if ((pSection = PStore_OpenSection(PS_DDD_MODEL1_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
		result = PStore_ReadSection((UINT8 *)pBinaryMap, 0, uiBuflen, pSection);
		if (result != E_PS_OK || uiUpdateDBChk) {
			DBG_ERR("Save DDD Model Data in PStore\r\n");
			//load Landmark Model
			Movie_DDDGetDBFile((UINT8 *)pBinaryMap, &uiBuflen);
			PStore_WriteSection((UINT8 *)pBinaryMap, 0, uiBuflen, pSection);
			PStore_CloseSection(pSection);
		} else {
			DBG_DUMP("PStore Load DDD Model Data OK\r\n");
			PStore_CloseSection(pSection);
		}
	} else {
		DBG_ERR("3.DDD_Init Failed: load model failed!\r\n");
		return;
	}
#endif
//#NT#2016/11/10#Brain Yen -end
	// Decrypt DDD database.
	buf->Size -= uiBuflen + 4;
	buf->Addr += uiBuflen + 4;
	crypto_open();
	bRetval = crypto_decryption(pBinaryMap, pBinaryMap, uiBuflen);
	crypto_close();
#if DDD_USING_FD_OR_NOT
	g_pFdHandleBuf = buf->Addr;
	buf->Size -= NvtFD_GetBuffSize();
	buf->Addr += NvtFD_GetBuffSize();
#endif

	if (bRetval != E_OK) {
		DBG_ERR("DDD decrypt failed!\r\n");
		return;
	}
#else
	UINT32 pBinaryMap = DDD_GetBinaryMap(&tmpBuf);//this func will be replaced.
#endif

	if (DDD_Init(buf, cachebuf, pBinaryMap,
				 Movie_DDDGetFdRslt) == DDD_STA_OK) {
		DBG_DUMP("DDD init success\r\n");
		if (DDD_LoadParam("A:\\DDD_config.txt")) {
			DBG_DUMP("DDD load param from SD card\r\n");
		} else {
			DBG_DUMP("DDD using default param\r\n");
		}

		if (DDD_LoadUserParam("A:\\ddd_user_config.txt")) {
			DBG_DUMP("DDD load user param from SD card\r\n");
		} else {
			DBG_DUMP("DDD using default user param\r\n");
		}
	} else {
		DBG_ERR("DDD init failed!\r\n");
	}
//#NT#2016/05/04#ML Cui -end
//#NT#2016/05/11#Isiah Chang -end
}

void Movie_DDDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF InputImg = {0};
	UINT32 myDualCam;
	ISF_PORT *pSrc;
	ISF_UNIT *pUnit;
	static BOOL bNeedClean = FALSE;
//CHKPNT;
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == SENSOR_1) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#elif (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == SENSOR_1 || System_GetEnableSensor() == SENSOR_2) {
		//only enable one sensor
		if (System_GetEnableSensor() == SENSOR_1) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		//enable dual sensors
		myDualCam = DUALCAM_BEHIND;
	}
#else
#error "Undefined case"
#endif

	if (myDualCam == DUALCAM_FRONT) {
		pUnit = ISF_IPL[0];
	} else {
		pUnit = ISF_IPL[1];
	}
	pSrc = ImageUnit_Out(pUnit, ISF_OUT4);



	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT4), (ISF_DATA *)&InputImg, 0);
		DDD_Process(&InputImg);
		if (!bNeedClean) {
			bNeedClean = TRUE;
		}
	} else {
		//we should clean the DDD result here to prevent OSD keep redrawing the landmark
		if (bNeedClean) {
			DDD_ClearRsltMsg();
			bNeedClean = FALSE;
		}
	}
}



//Warning frequency will be more and more frequently then bState is always true
#define DDD_SET_WRN_STATE(bFlag, bState, maxFreq, minFreq, rate) \
	static UINT32 ui##bFlag##Frame = 0;   \
	static UINT32 ui##bFlag##Freq = minFreq;  \
	static BOOL   bFlag = FALSE;    \
	Movie_DDDSetWarnForStatistic(bState, &ui##bFlag##Frame, &ui##bFlag##Freq, \
								 &bFlag, maxFreq, minFreq, rate)

static void Movie_DDDSetWarnForStatistic(BOOL bState,
		UINT32 *uiWarnFrame, UINT32 *uiWarnFreq, BOOL *bWarn,
		UINT32 uiMaxFreq, UINT32 uiMinFreq, FLOAT rate)
{

	//debug_msg("uiWarnFrame:%d\r\n", *uiWarnFrame);
	if (bState) {

		(*uiWarnFrame)++;
		if (*uiWarnFrame == 1) {
			*bWarn = TRUE;
		} else if ((*uiWarnFrame) % (*uiWarnFreq) == 0) {
			*bWarn = TRUE;
			*uiWarnFreq /= rate;
			if (*uiWarnFreq < uiMinFreq) {
				*uiWarnFreq = uiMinFreq;
			}
		}
	} else {
		*uiWarnFrame = 0;
		*uiWarnFreq = uiMaxFreq;
		*bWarn = FALSE;
	}

}

//#NT#2016/07/20#Brain Yen -begin
//#NT#Add for DDD ararm
void Movie_DDDProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	RECT trackRect;
	DDD_WARN dowsyState;
	UINT32 WarnEvent = 0;
	static UINT32 PreWarnEvent = DDD_UWRN_NORM;
	static UINT16 EventCnt = 0;
	if (DDD_GetTrackingRect(&trackRect)) {

		dowsyState = DDD_GetUserWarning();

		DDD_SET_WRN_STATE(bYawnWarn, (dowsyState & (DDD_UWRN_YAWN | DDD_UWRN_YAWN_DURA)), 500, 50, 2.0);
		DDD_SET_WRN_STATE(bPerWarn, (dowsyState & (DDD_UWRN_BLINK | DDD_UWRN_EYE)), 150, 30, 1.2);
		DDD_SET_WRN_STATE(bNodeWarn, (dowsyState & (DDD_UWRN_NOD | DDD_UWRN_NOD_DURA)),  500, 50, 2.0);
		DDD_SET_WRN_STATE(bDisWarn, (dowsyState & (DDD_UWRN_DIS | DDD_UWRN_DIS_DURA)),  500, 50, 2.0);


		if (dowsyState) {
			if (dowsyState & DDD_UWRN_NOFACE) {
				EventCnt = 0;
				PreWarnEvent = DDD_UWRN_NOFACE;
			} else {
				if (bDisWarn) {
					DBG_IND("Serious Warning! Driver distraction!\r\n");
					WarnEvent = DDD_ALARM_DIS;
					bPerWarn = FALSE;
				} else if ((dowsyState & DDD_UWRN_EYE_DURA)) {
					DBG_IND("Serious Warning! Driver's eye closed too long!\r\n");
					WarnEvent = DDD_ALARM_EYE;
				} else if (bPerWarn) {
					DBG_IND("Warning! Driver drossy!!\r\n");
					WarnEvent = DDD_ALARM_PERCLOS;
					bPerWarn = FALSE;
				} else if (bYawnWarn) {
					DBG_IND("Remind~ driver yawn too frequently\r\n");
					WarnEvent = DDD_ALARM_YAWN;
					bYawnWarn = FALSE;
				} else if (bNodeWarn) {
					DBG_IND("Annotation! Driver is a little tired!\r\n");
					WarnEvent = DDD_ALARM_NODE;
					bNodeWarn = FALSE;
				}
				if (PreWarnEvent == WarnEvent) {
					EventCnt++;
				} else {
					EventCnt = 0;
				}
				if ((EventCnt % 20) == 0) {
					Ux_PostEvent(NVTEVT_CB_DDD_SHOWALARM, 1, WarnEvent);
				}
				PreWarnEvent = WarnEvent;
			}
		} else {
			EventCnt = 0;
			PreWarnEvent = DDD_UWRN_NORM;
		}
	}

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#if (MOVIE_DDD_SAVE_LOG == ENABLE)

	{
		UINT32 uiLogSize;
		UINT32 uiLogAddr;
		uiLogSize = DDD_GetLog(&uiLogAddr);
//        debug_msg("uiLogSize: %d\r\n", uiLogSize);
		if (uiLogSize) {
			mp4log_AddLog(DbgLogDDDID, (CHAR *)uiLogAddr, uiLogSize);
		}
	}
#endif
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}
#undef DDD_SET_WRN_STATE
//#NT#2016/07/20#Brain Yen -end
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
UINT32 Movie_DDDCalcBufWithBinaryMap(VOID)
{
	UINT32 uiDBFileSize = Movie_DDDGetDBFileSize();
	if (uiDBFileSize == 0) {
		uiDBFileSize = Movie_DDDGetDBFileSizeFrCard();
	}
	DBG_DUMP("^Yddd filesize:%d\r\n", uiDBFileSize);
	if (uiDBFileSize > DDD_MAX_DB_FILE_SIZE) {
		DBG_ERR("DDD DB file size is too large!\r\n");
		return 0;
	}
#if DDD_USING_FD_OR_NOT
	return NvtFD_GetBuffSize() + DDD_CalcBuffSize() + uiDBFileSize + 4;
#else
	return DDD_CalcBuffSize() + uiDBFileSize + 4;
#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvddd = {
	{
		PHOTO_PRIORITY_SH,          ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_DDDProcess",
		Movie_DDDInit,          ///< init fp, only execute at first process
		Movie_DDDProcess,       ///< process fp
		Movie_DDDProcessEnd,    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Movie_DDDCalcBufWithBinaryMap,///< get working buffer fp
		DDD_CalcCacheBuffSize,  ///< get working cache buffer fp
	},
	NULL                    ///< point next Function Obj, last Function must be set NULL
};
//#NT#2016/05/11#Isiah Chang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

//#NT#2017/02/07#XQ Liu -begin
//#NT#Support Background Construction/Tamper Detection
#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_BC_MD
void Movie_BCLock(BOOL bLock)
{
	gBCLock = bLock;
}

void Movie_BCProcessRateSet(UINT32 ProcessRate)
{
	gBCProcessRate = ProcessRate;
}

UINT32 Movie_BCProcessRateGet(void)
{
	return gBCProcessRate;
}

UINT32 Movie_BCGetBlkTarget(URECT* target, URECT* targetcoord, UINT32 maxnum)
{
	return bc_getblktarget(target, targetcoord, maxnum);
}

void Movie_BCInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	bc_init(buf->Addr, cachebuf->Addr);

#if (MOVIE_BC_MD == ENABLE)
	bc_setprms(BC_MAX_FG_FRM_NUM, 50);
	bc_setblkprms(BC_BLK_NUM_W, 10);
	bc_setblkprms(BC_BLK_NUM_H, 10);
#endif
}

void Movie_BCProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA		 isf_data;
	ISF_UNIT		 *pUnit;
	ISF_PORT		 *pSrc;
	IMG_BUF          *inputimg;

	if (gBCLock == TRUE) {
		return;
	}

	gBCCounter++;
	if (gBCCounter >= gBCProcessRate) {
		gBCCounter = 0;
		gBCUpdateCounter++;
	} else {
		return;
	}

	#if (MOVIE_ODT_FUNC && DVCAM_BC_USE_ODT)
	static INT32					regist_flag = ODT_STA_ERROR;
	static UINT32					rslt_loc;
	static UINT32					num;
	static URECT					rslts[32];
	static odt_od_result 			odt_rslts[32];

	static odt_process_buf_param 	odt_buf_param;
	static odt_rslt_inf				buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				buf2;
	static odt_od_result 			buf3[32];

	odt_buf_param.detect_rslt_addr = (UINT32)buf3;
	odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&buf2);
	odt_buf_param.odt_rslt_buffer_addr = (UINT32)buf1;

	if (regist_flag == ODT_STA_ERROR) {
		regist_flag = odt_regist(&odt_buf_param);
		if (regist_flag == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 5, odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 0, odt_buf_param.process_id);
		}
	}
	#endif

	pUnit = &ISF_UserProc;

#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

	if (ImageUnit_IsAllowPull(pSrc) && (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK)) {

		#if (MOVIE_ODT_FUNC && DVCAM_BC_USE_ODT)
		rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif

		inputimg = (IMG_BUF *)&isf_data.Desc[0];
		bc_setimg(inputimg);
		ImageUnit_ReleaseData((ISF_DATA*)&isf_data);

		bc_process();
		#if (MOVIE_ODT_FUNC && DVCAM_BC_USE_ODT)
		UINT32 i;
		URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
		num = bc_gettarget(rslts, &targetcoord, 32);
		for(i = 0; i < num; i++)
		{
			odt_rslts[i].x = rslts[i].x;
			odt_rslts[i].y = rslts[i].y;
			odt_rslts[i].w = rslts[i].w;
			odt_rslts[i].h = rslts[i].h;
			odt_rslts[i].score = 0;
			odt_rslts[i].class = 1008;
		}
		odt_setodrslt(num, odt_rslts, rslt_loc, odt_buf_param.process_id);
		#endif

		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
	}
}

UINT32 Movie_BCGetBufSize(VOID)
{
	return bc_getbufsize();
}

UINT32 Movie_BCGetCacheBufSize(VOID)
{
	return bc_getcachebufsize();
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvbc = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_BCProcess",
		Movie_BCInit,           ///< init fp, only execute at first process
		Movie_BCProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Movie_BCGetBufSize,        ///< get working buffer fp
		Movie_BCGetCacheBufSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2017/02/07#XQ Liu -end


//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TWD_FUNC
void Movie_TWDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	nvt_twd_init(buf->Addr, cachebuf->Addr);
}
void Movie_TWDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	static UINT32    TWDUpdateCounter = 0;
	if (gBCLock == TRUE) {
		return;
	}
	if (gBCUpdateCounter != TWDUpdateCounter) {
		TWDUpdateCounter = gBCUpdateCounter;
	} else {
		return;
	}
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL[0];
	} else {
		pUnit = ISF_IPL[1];
	}

	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);

	nvt_twd_process(&InputImg);

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtwd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TWDProcess",
		Movie_TWDInit,           ///< init fp, only execute at first process
		Movie_TWDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		nvt_twd_calcbuffsize,        ///< get working buffer fp
		nvt_twd_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-zone detection function
#if MOVIE_TZD_FUNC
void Movie_TZDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	nvt_tzd_init(buf->Addr, cachebuf->Addr);
}
void Movie_TZDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	static UINT32    TZDUpdateCounter = 0;
	if (gBCLock == TRUE) {
		return;
	}
	if (gBCUpdateCounter != TZDUpdateCounter) {
		TZDUpdateCounter = gBCUpdateCounter;
	} else {
		return;
	}
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL[0];
	} else {
		pUnit = ISF_IPL[1];
	}

	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);

	nvt_tzd_process(&InputImg);

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtzd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TZDProcess",
		Movie_TZDInit,           ///< init fp, only execute at first process
		Movie_TZDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		nvt_tzd_calcbuffsize,        ///< get working buffer fp
		nvt_tzd_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
void Movie_SCDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	SCD_Init();
}

void Movie_SCDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	HEADER_RAW_DATA rawHeaderInfo = {0};
	DISALG_VECTOR disOffset = {0};
	SCD_PROC_PRMS prms = {0};

	if (IPL_HeaderRawGetCurInfo_Dly1VD(IPL_PATH_1, &rawHeaderInfo) != E_OK) {
		DBG_ERR("DIS: Get header error!!!\r\n");
	}

	dis_getFrameCorrVecPxl(&disOffset, rawHeaderInfo.frame_cnt);
	//DBG_DUMP("dis: %d %d %d %d\n", disOffset.vector.iX, disOffset.vector.iY, disOffset.frame_cnt, disOffset.score);

	if (1 == disOffset.score) {
		prms.iX = disOffset.vector.iX;
		prms.iY = disOffset.vector.iY;
		SCD_Process(&prms);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvscd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_SCDProcess",
		Movie_SCDInit,          ///< init fp, only execute at first process
		Movie_SCDProcess,       ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		NULL,                   ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // MOVIE_SCD_FUNC
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
void Movie_FTGLock(BOOL bLock)
{
	gFTGLock = bLock;
	ftg_lock(bLock);
}

BOOL Movie_FTGGetLockSts(void)
{
	return gFTGLock;
}


void Movie_FTGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//URECT roi_rect = {300,300,400,300};
	//ftg_setfdroi(TRUE, &roi_rect);

	ftg_init(buf, cachebuf, 5, 15, 3);
}

void Movie_FTGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	ISF_PORT        *pSrc;


	if (gFTGLock == TRUE) {
		return;
	}

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL[0];
	} else {
		pUnit = ISF_IPL[1];
	}

	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(ISF_IPL[0], ISF_OUT1), (ISF_DATA *)&InputImg, 0);
	} else {

		ImageUnit_PullData(ImageUnit_Out(ISF_IPL[0], ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	}

#if 0
	static UINT32 ftg_num2 = 0;
	static UINT32 ftg_time2 = 0;
	static UINT32 ftg_max2 = 0;
	UINT32 t1 = Perf_GetCurrent();
	ftg_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	UINT32 t2 = Perf_GetCurrent();
	if (ftg_num2 < 1500 && ftg_num2 >= 1000) {
		if (ftg_max2 < t2 - t1) {
			ftg_max2 = t2 - t1;
		}
		ftg_time2 += t2 - t1;
		ftg_num2++;
	} else if (ftg_num2 >= 1000) {
		DBG_ERR("500 FTG uses %d us...max %d us\n\r", ftg_time2, ftg_max2);
	} else {
		ftg_num2++;
	}
#else
	ftg_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
#endif

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvftg = {
	{
		PHOTO_PRIORITY_L,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_FTGProcess",
		Movie_FTGInit,           ///< init fp, only execute at first process
		Movie_FTGProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		ftg_calcbuffsize,        ///< get working buffer fp
		ftg_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/17#Bin Xiao -end
//#NT# Support Face Tracking Grading(FTG)

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support FD used by Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
void Movie_FTGFDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ftg_config(ftg_fd_cfg_scale_eng, ftg_fd_scale_eng2);
	ftg_config(ftg_fd_cfg_sensitivity, 1);

	// Init FD buffer, set max face number to 10, and process rate as 3
	ftg_fd_init(buf, cachebuf, MAX_FDNUM, 15, 5);
}

void Movie_FTGFDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	ISF_PORT        *pSrc;

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL[0];
	} else {
		pUnit = ISF_IPL[1];
	}

	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(ISF_IPL[0], ISF_OUT1), (ISF_DATA *)&InputImg, 0);
	} else {

		ImageUnit_PullData(ImageUnit_Out(ISF_IPL[0], ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	}

#if 0
	static UINT32 ftg_num3 = 0;
	static UINT32 ftg_time3 = 0;
	static UINT32 ftg_max3 = 0;
	UINT32 t1 = Perf_GetCurrent();
	ftg_fd_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	UINT32 t2 = Perf_GetCurrent();
	if (ftg_num3 < 1500 && ftg_num3 >= 1000) {
		if (ftg_max3 < t2 - t1) {
			ftg_max3 = t2 - t1;
		}
		ftg_time3 += t2 - t1;
		ftg_num3++;
	} else if (ftg_num3 >= 1000) {
		DBG_ERR("500 FTG_FD uses %d us...max %d us\n\r", ftg_time3, ftg_max3);
	} else {
		ftg_num3++;
	}
#else
	ftg_fd_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
#endif

	//Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvftgfd = {
	{
		PHOTO_PRIORITY_M,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_FTGFDProcess",
		Movie_FTGFDInit,           ///< init fp, only execute at first process
		Movie_FTGFDProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                    ///< exit fp, only execute at photo close
		ftg_fd_calcbuffsize,        ///< get working buffer fp
		ftg_fd_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/17#Bin Xiao -end


//#NT#2017/08/29#Charlie Chang -begin
//#NT# Support CEVA Deep Neural Network (CDNN)
#if MOVIE_CDNN_FUNC
#if CDNN_DEV
VOID Movie_CDNNInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	cdnndev_app_init(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	cdnndev_app_init(ISF_OUT15);
#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvcdnn = {
	{
		PHOTO_PRIORITY_M,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"Movie_CDNNProcess",            ///< process name
		Movie_CDNNInit,                 ///< init fp, only execute at first process
		cdnndev_app_process,            ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		cdnndev_app_calc_buf_size,      ///< get working buffer fp
		cdnndev_app_calc_cache_buf_size,///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#else // CDNN_DEV
PHOTO_FUNC_INFO PhotoFuncInfo_mvcdnn = {
	{
		PHOTO_PRIORITY_M,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"Movie_CDNNProcess",            ///< process name
		alg_CDNNInit,                   ///< init fp, only execute at first process
		alg_CDNNProcess,                ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		alg_CDNNCalcBufSize,            ///< get working buffer fp
		alg_CDNNCalcCacheBufSize,       ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // CDNN_DEV
#endif // MOVIE_CDNN_FUNC
//#NT#2017/08/29#Charlie Chang -end

#if MOVIE_UVAC_FUNC
void Movie_UvcFunc(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init


	return;
}

void Movie_UvcProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_DATA         isf_data, isf_new_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	UINT32           img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;
	IMG_BUF          dst_img;

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT14);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		ImageUnit_GetVdoImgSize(pUnit, pSrc->oPort,&dst_w, &dst_h);
		dst_pxlfmt = pISF_buf->PxlFmt;
		img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);

		GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);

		//Do process here
		GxImg_CopyData(pISF_buf, REGION_MATCH_IMG, &dst_img , REGION_MATCH_IMG);

		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
		isf_new_data.Sign = ISF_SIGN_DATA;

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_new_data, 0);
		ImageUnit_ReleaseData(&isf_data);
		ImageUnit_ReleaseData(&isf_new_data);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvToUvcFunc = {
	{
		PHOTO_PRIORITY_M,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_SendYUVToUvc",
		Movie_UvcFunc,           ///< init fp, only execute at first process
		Movie_UvcProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		NULL,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

#if (MOVIE_STRM_PATH_USERPROC==ENABLE)

void Movie_StrmInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init


	return;
}


void Movie_StrmProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf){


	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT13);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//IMG_BUF 		*pISF_buf = NULL;
		//pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do waht you want~~~

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_mvToStrmFunc = {
	{
		PHOTO_PRIORITY_M,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_SendYUVToStrm",
		Movie_StrmInit,           ///< init fp, only execute at first process
		Movie_StrmProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		NULL,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif

//#NT#2018/03/19#Wayne Wei -begin
//#NT# Support License Plate Recognition (LPR)
#if MOVIE_LPR_FUNC
#define LPR_SENSOR_FLOW 1 //1: sensor mode, 0: file mode
#define LPR_CALI_ENABLE 0  //1: enable calibration, 0: disable

//#define _DEBUG_LPR_USER_
#define LPR_IMG_PATH "A:\\LPRLib\\DI\\imgData\\"
#define LPR_MAX_FILE_PATH              100
UINT32 lpr_test_idx = 0;
char filename[64];

//////////////////////////////////////////////////////////////////////////
// Voting function
//////////////////////////////////////////////////////////////////////////
#define MAX_VOTE_NUM   5
#define MAX_NAME_SIZE  15
char g_vote_name[MAX_VOTE_NUM][MAX_NAME_SIZE];

void Movie_LPRVote_init(void)
{
    int idx,i,j;

    for(i = 0; i < MAX_VOTE_NUM; i++) {
        for(j = 0; j < MAX_NAME_SIZE; j++) {
                g_vote_name[i][j] = 0;
        }
    }

    for(idx = 0; idx < MAX_VOTE_NUM; idx++) {
            snprintf(&g_vote_name[idx][0], MAX_NAME_SIZE, "...");
    }
}

int Movie_LPRVote_lp_name(char* input_lp_name)
{
    int      i, j, cmp;
    int      max_idx, max_value;
    int      vote_sum[MAX_VOTE_NUM] = {};
    // delete old lp name
    for(i = 0;i < MAX_VOTE_NUM - 1;i++) {
            snprintf(&g_vote_name[i][0], MAX_NAME_SIZE, "%s", &g_vote_name[i+1][0]);
    }
    // get new lp name
    snprintf(&g_vote_name[MAX_VOTE_NUM - 1][0], MAX_NAME_SIZE, "%s", input_lp_name);
    // voting
    for(i = 0;i < MAX_VOTE_NUM; i++) {
            for(j = 0;j < MAX_VOTE_NUM; j++) {
                    cmp = strcmp(&g_vote_name[i][0], &g_vote_name[j][0]);
                    if(cmp == 0){
                        vote_sum[i]++;
                    }
            }
    }
    max_value = -1;
    max_idx = 0;
    for(i = MAX_VOTE_NUM - 1; i >= 0; i--) {
            if(vote_sum[i] >= max_value) {
                max_idx = i;
                max_value = vote_sum[i];
        }
    }
    // output the result with max voting
    if(max_value != 0) {
        DBG_DUMP("[LPR]Vote: %s\r\n", &g_vote_name[max_idx][0]);
    }

    return 0;
}

INT32 Movie_LPRCheckVer(void)
{
    INT32 ret;
    LPR_SYS_PRMS sys;

    sys = lpr_getsystem();

    if ((sys.ver.major != LPR_VERSION_MAJOR) || (sys.ver.minor != LPR_VERSION_MINOR) ||
        (sys.ver.build != LPR_VERSION_BUILD)) {
            DBG_ERR("[LPR]Error, Not match version, Please check LPR_API.h(%d:%d:%d) and LPR.a(%d:%d:%d)\r\n",
                       LPR_VERSION_MAJOR, LPR_VERSION_MINOR, LPR_VERSION_BUILD,
                    sys.ver.major, sys.ver.minor, sys.ver.build);
            ret = LPR_STA_ERR;
    } else {
            DBG_DUMP("[LPR] Version Check OK:  LPR_API.h(%d:%d:%d) and LPR.a(%d:%d:%d)\r\n",
                    LPR_VERSION_MAJOR, LPR_VERSION_MINOR, LPR_VERSION_BUILD,
                    sys.ver.major, sys.ver.minor, sys.ver.build);
            ret = LPR_STA_OK;
    }

    return ret;
}

void Movie_LPRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init after file system is ready
	return;
}

void Movie_LPRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA            isf_data;
	ISF_UNIT            *pUnit;
	ISF_PORT            *pSrc = 0;
	static UINT32       check_fs_status = 0;
        IMG_BUF             *InputImg;
	LPR_RESULT          lpr_result;
	UINT32              p_idx;
        //UINT8               u8Status;
	#if (LPR_CALI_ENABLE == 1)
        IPOINT              lp_corner_loc[4];
	#endif

#if (DEBUG_DUMP_IMG == ENABLE)
	static UINT32   frame_cnt = 0;
	char            filename[50];
#endif
        if (check_fs_status == 0) {

		check_fs_status = FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0);
		if (check_fs_status == 0) {
			//DBG_DUMP("####FS is not ready####\r\n");
			return;
		} else {
			DBG_DUMP("####FS is ready####\r\n");
		}

		LPR_INIT_PRMS      init_prms = {0};
		UINT32 uiPool;
		UINT32 uiPoolSize;

		if (Movie_LPRCheckVer() != LPR_STA_OK) {
			DBG_ERR("[LPR]Error, Check version failed.\r\n");
			check_fs_status = 0;
			return;
		}
		Movie_LPRVote_init();

		uiPoolSize = LPR_CalcBuffSize();
		uiPool=SxCmd_GetTempMem(uiPoolSize);

		buf->Addr = uiPool;
		buf->Size = uiPoolSize;
		cachebuf->Addr = 0;
		cachebuf->Size = 0;

		init_prms.cp_type    = CN_TYPE;
		init_prms.src_w      = 720;
		init_prms.src_h      = 480;
		init_prms.crop_x     = 0;
		init_prms.crop_y     = 0;
		init_prms.crop_w     = 600;
		init_prms.crop_h     = 300;
		init_prms.detect_w   = 600;
		init_prms.detect_h   = 300;
		init_prms.reg_w      = 600;
		init_prms.reg_h      = 300;
		snprintf(init_prms.cp_model_path, sizeof(init_prms.cp_model_path), "A:\\LPRLib\\model");
		init_prms.buf        = buf;
		init_prms.cache_buf  = cachebuf;

		lpr_ap_init(&init_prms);
    	}

	pUnit = &ISF_UserProc;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
		pSrc = ImageUnit_Out(pUnit, ISF_OUT15);
#endif

    	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//IMG_BUF *pISF_buf = NULL;
		//pISF_buf = (IMG_BUF *)&isf_data.Desc[0];
		//DBG_IND("send DSP YUV w=%d h=%d\r\n", pISF_buf->Width, pISF_buf->Height);
                InputImg = (IMG_BUF *)&isf_data.Desc[0];

#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif

#if 0
		if (MovRec_IsRecording()) {
                	DBG_DUMP("####[Rec]Sensor inputting width=%d height=%d\r\n", InputImg->Width, InputImg->Height);
        	} else {
                	DBG_DUMP("####[Preview]Sensor inputting width=%d height=%d\r\n", InputImg->Width, InputImg->Height);
        	}
#endif

#if (LPR_SENSOR_FLOW == 1)
		static UINT32 is_first = 1;
		LPR_SENSOR_PRMS sensor_prms = {0};

		DBG_DUMP("[LPR]Sensor mode\r\n");
		sensor_prms.data = InputImg;

		if (is_first == 1) {
			//Run-time to set user parameter
			lpr_setprms(LPR_ROI_LEFT_E, 5);
			lpr_setprms(LPR_ROI_RIGHT_E, 5);
			lpr_setprms(LPR_ROI_TOP_E, 5);
			lpr_setprms(LPR_ROI_BOTTOM_E, 5);
			lpr_setprms(LPR_FEA_MAX_WIDTH_E, 650);
			lpr_setprms(LPR_FEA_MIN_WIDTH_E, 100);
			lpr_setprms(LPR_FEA_MAX_HEIGHT_E, 150);
			lpr_setprms(LPR_FEA_MIN_HEIGHT_E, 25);
			lpr_setprms(LPR_REG_THRESHOLD_E, 400);

			#if (LPR_CALI_ENABLE == 1)
                        lp_corner_loc[0].x = 924;
                        lp_corner_loc[0].y = 506;
                        lp_corner_loc[1].x = 1066;
                        lp_corner_loc[1].y = 513;
                        lp_corner_loc[2].x = 1062;
                        lp_corner_loc[2].y = 557;
			lp_corner_loc[3].x = 918;
                        lp_corner_loc[3].y = 549;

                        lpr_calibration_init(lp_corner_loc, 0);
			#endif

			is_first = 0;
		}

#ifdef _DEBUG_LPR_USER_
		DBG_DUMP("[LPR]Get parameter, roi:(%d %d %d %d) fea:(%d %d %d %d) reg:(%d)\r\n",
			lpr_getprms(LPR_ROI_LEFT_E), lpr_getprms(LPR_ROI_RIGHT_E),
			lpr_getprms(LPR_ROI_TOP_E), lpr_getprms(LPR_ROI_BOTTOM_E),
			lpr_getprms(LPR_FEA_MAX_WIDTH_E), lpr_getprms(LPR_FEA_MIN_WIDTH_E),
			lpr_getprms(LPR_FEA_MAX_HEIGHT_E), lpr_getprms(LPR_FEA_MIN_HEIGHT_E),
			lpr_getprms(LPR_REG_THRESHOLD_E));
#endif

		lpr_proc_sensor(&sensor_prms);
		lpr_get_result(&lpr_result);

		for(p_idx = 0;p_idx < lpr_result.max_rslt_num; p_idx++){
			DBG_DUMP("[LPR]LPName_Top%d: %s num=%d\r\n", p_idx+1, lpr_result.rslt[p_idx].lp_name, lpr_result.rslt[p_idx].ch_num);
		}
		Movie_LPRVote_lp_name((char *)lpr_result.rslt[0].lp_name);

#endif

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	} else {
		DBG_DUMP("[LPR] no sensor inputting\r\n");
	}

#if (LPR_SENSOR_FLOW == 0)
	//DBG_DUMP("####Sensor not inputting, please enter \"ks2\" to enable record.\r\n");
        DBG_DUMP("[LPR]File mode\r\n");
        LPR_FILE_PRMS file_prms = {0};
        UINT32 test_num;
        UINT32 test_max_num;

        static UINT32 is_first = 1;
	if (is_first == 1) {
		//Run-time to set user parameter
		lpr_setprms(LPR_ROI_LEFT_E, 5);
		lpr_setprms(LPR_ROI_RIGHT_E, 5);
		lpr_setprms(LPR_ROI_TOP_E, 5);
		lpr_setprms(LPR_ROI_BOTTOM_E, 5);
		lpr_setprms(LPR_FEA_MAX_WIDTH_E, 650);
		lpr_setprms(LPR_FEA_MIN_WIDTH_E, 100);
		lpr_setprms(LPR_FEA_MAX_HEIGHT_E,150);
		lpr_setprms(LPR_FEA_MIN_HEIGHT_E, 25);
		lpr_setprms(LPR_REG_THRESHOLD_E, 400);

		#if (LPR_CALI_ENABLE == 1)
		lp_corner_loc[0].x = 806;
                lp_corner_loc[0].y = 443;
                lp_corner_loc[1].x = 975;
                lp_corner_loc[1].y = 459;
                lp_corner_loc[2].x = 978;
                lp_corner_loc[2].y = 574;
                lp_corner_loc[3].x = 808;
                lp_corner_loc[3].y = 544;

                lpr_calibration_init(lp_corner_loc, 0);
		#endif

		is_first = 0;
	}

#ifdef _DEBUG_LPR_USER_
                DBG_DUMP("[LPR]Get parameter, roi:(%d %d %d %d) fea:(%d %d %d %d) reg:(%d)\r\n",
                        lpr_getprms(LPR_ROI_LEFT_E), lpr_getprms(LPR_ROI_RIGHT_E),
                        lpr_getprms(LPR_ROI_TOP_E), lpr_getprms(LPR_ROI_BOTTOM_E),
                        lpr_getprms(LPR_FEA_MAX_WIDTH_E), lpr_getprms(LPR_FEA_MIN_WIDTH_E),
                        lpr_getprms(LPR_FEA_MAX_HEIGHT_E), lpr_getprms(LPR_FEA_MIN_HEIGHT_E),
                        lpr_getprms(LPR_REG_THRESHOLD_E));
#endif

        test_num = 10;
        test_max_num = 10;
        DBG_DUMP("[LPR]test_num=%d\r\n", test_num);

        lpr_test_idx++;
        if (lpr_test_idx > test_max_num) {
                lpr_test_idx = 1;
        }

        sprintf((char *)filename, "%strain_o_000%02d.jpg", LPR_IMG_PATH, lpr_test_idx);

        file_prms.filename = filename;

        DBG_DUMP("[LPR][DET]filename=%s\r\n", file_prms.filename);

        lpr_proc_file(&file_prms);

        DBG_DUMP("[LPR]####test license plate idx=%d \r\n", lpr_test_idx);

        lpr_get_result(&lpr_result);

        for(p_idx = 0;p_idx < lpr_result.max_rslt_num; p_idx++){
                DBG_DUMP("[TestKit][LPR]LPName_Top%d: %s num=%d\r\n", p_idx+1, lpr_result.rslt[p_idx].lp_name, lpr_result.rslt[p_idx].ch_num);
        }
#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvlpr = {
	{
		PHOTO_PRIORITY_M,              ///< priority
		IPL_PATH_1,                    ///< IPL Id
		PHOTO_ISR_IME_FRM_END,         ///< isr tigger event
		"Movie_LPRProcess",            ///< process name
		Movie_LPRInit,                 ///< init fp, only execute at first process
		Movie_LPRProcess,              ///< process fp
		NULL,                          ///< process end fp
		NULL,                          ///< exit fp, only execute at photo close
		LPR_CalcBuffSize,              ///< get working buffer fp
		NULL,                          ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // MOVIE_LPR_FUNC
//#NT#2018/03/19#Wayne Wei -end



void alg_Movie_InitPhotoFunc(PHOTO_FUNC_INFO *phead)
{
#if (_ADAS_FUNC_ == ENABLE)
	//#NT#2016/03/25#KCHong -begin
	//#NT#New ADAS
	// --------------------------------------------------------------------------------------------------
	//    *** ADAS parameter setting ***
	//    User can set ADAS parameters (FSize/LD sensitivity/FC warning distance/ LD debounce time/FC debounce time)
	//    If not set, ADAS lib will use default settings.
	//    Important notice:
	//        ADAS_SetLdFcFsizeValueByPixel(), ADAS_SetLdwsLeftSensitivity(), ADAS_SetLdwsRightSensitivity() MUST be set before ADAS_Init()
	//        Only ADAS_SetFcwsUIWarningDist() / ADAS_SetFcwsDebounceTime / ADAS_SetLdwsDebounceTime can be changed during ADAS detection.
	//        Ex. Different car speed has different FC warning distance.
	//
	//        FSize          : Please refer to APNote to measure FSize
	//        LD Sensitivity : 0~50, default is 15. Bigger value means more sentive.
	//        FC distance    : 1~40, default is 15. The unit is meter.
	//   --------------------------------------------------------------------------------------------------
	// Set fsize value. This value CANNOT be changed on-the-fly.
	ADAS_SetLdFcFsizeValueByPixel(188);

	// Set fcws warning distance (unit: m). This value can be changed on-the-fly.
	ADAS_SetFcwsUIWarningDist(15);
	// Set fcws debounce time (unit: ms). This value can be changed on-the-fly.
	ADAS_SetFcwsDebounceTime(10 * 1000);
	// Set fcws output format
	ADAS_SetFcwsCarRsltFmt(ADAS_FCWS_RSLT_LANE_WIDTH);

	// Set ldws left side sensitivity. This value CANNOT be changed on-the-fly.
	ADAS_SetLdwsLeftSensitivity(15);
	// Set ldws right side sensitivity. This value CANNOT be changed on-the-fly.
	ADAS_SetLdwsRightSensitivity(15);
	// Set ldws debounce time (Unit: ms). This value can be changed on-the-fly.
	ADAS_SetLdwsDebounceTime(10 * 1000);
	// Set fc/ld IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selMidPrioAppsDataSrc(IPL_PATH_1);
	// Set autoVP IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selLowPrioAppsDataSrc(IPL_PATH_1);

	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_adas;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   // find last func
		}
		pcurr->pNext = pfunc;       // append this func
		pfunc->pNext = NULL;        // mark end
	}

	#if 0
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_SnG;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
	#endif

#if (_AUTOVP_FUNC_ == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_autoVP;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   //find last func
		}
		pcurr->pNext = pfunc;       //append this func
		pfunc->pNext = NULL;        //mark end
	}
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)
#endif  // #if (_ADAS_FUNC_ == ENABLE)

#if MOVIE_TSR_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtsr;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtsr.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtsr.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, TSR IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvtsr.Info.IPLId);

		TSR_Lock(FALSE);
	}
#endif

#if MOVIE_TSR_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtsrdsp;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtsrdsp.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtsrdsp.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, TSR IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvtsrdsp.Info.IPLId);
	}
#endif

	//#NT#2017/11/3#Ming Yang -begin
    //#NT# Object detection
#if (MOVIE_OD_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvod;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		PhotoFuncInfo_mvod.Info.IPLId = IPL_PATH_1;
	}
#endif
	//#NT#2017/11/3#Ming Yang -end

//#NT#2017/09/27#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if (MOVIE_ODT_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvodt;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		PhotoFuncInfo_mvodt.Info.IPLId = IPL_PATH_1;
	}

	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvodtod;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 /// find last func
		}
		pcurr->pNext = pfunc;											/// append this func
		pfunc->pNext = NULL;											/// mark end
		PhotoFuncInfo_mvodtod.Info.IPLId = IPL_PATH_1;
	}

#endif
//#NT#2017/09/27#Bin Xiao -end



//#NT#2018/03/12#Dan Guo -begin
//#NT# Face recogniton
#if (MOVIE_FR_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvfr;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		PhotoFuncInfo_mvfr.Info.IPLId = IPL_PATH_1;
	}

#endif
//#NT#2018/03/12#Dan Guo -end


//#NT#2018/01/29#Bin Xiao -begin
//#NT# Face Detection by CNN
#if (MOVIE_FDCNN_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvfdcnn;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		PhotoFuncInfo_mvfdcnn.Info.IPLId = IPL_PATH_1;
	}
#endif
//#NT#2018/01/29#Bin Xiao -end

//#NT#2017/12/01#WY -begin
//#NT# OD pipe for NODNet & NODNetlite
#if (MOVIE_ODP_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvodpnet1;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		PhotoFuncInfo_mvodpnet1.Info.IPLId = IPL_PATH_1;
	}

	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvodpnet2;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 /// find last func
		}
		pcurr->pNext = pfunc;											/// append this func
		pfunc->pNext = NULL;											/// mark end
		PhotoFuncInfo_mvodpnet2.Info.IPLId = IPL_PATH_1;
	}

#endif
//#NT#2017/12/01#WY -end


#if _CVHOST_EX_
	{
		PHOTO_FUNC_INFO *pfunc = &MovieFuncInfo_CvHostEx;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if _NVTCV_EX_
	{
		PHOTO_FUNC_INFO *pfunc = &MovieFuncInfo_NvtCvEx;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if MOVIE_DIS_MV_FUNC
	{
		// DIS function

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_dis;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if MOVIE_CDNN_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvcdnn;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if MOVIE_FD_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvfd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvfd.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FD IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvfd.Info.IPLId);
	}
#endif

	//Support object tracking function
#if MOVIE_OT_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvot;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_1;

	}
#endif
	//Support Dis dsp function

#if MOVIE_DISDSP_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvdisdsp;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvdisdsp.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvdisdsp.Info.IPLId = IPL_PATH_2;
		}
	}
#endif

	//#NT#2016/05/20#Yang Jin -end


	//Support DDD's Face Detection(DDDFD)
#if MOVIE_DDD_FD_FUNC
	{
		UINT32      myDualCam;

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvdddfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvdddfd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvdddfd.Info.IPLId = IPL_PATH_2;
		}

	}
#endif
	//#NT#2016/11/15#ML Cui -end

	//Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
	{
		UINT32      myDualCam;

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvddd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvddd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvddd.Info.IPLId = IPL_PATH_2;
		}
	}

	DDD_Lock(FALSE);
#endif

#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_BC_MD
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvbc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		PhotoFuncInfo_mvbc.Info.IPLId = IPL_PATH_1;
	}
#endif


	// Support trip-wire detection & trip-zone detection function
#if MOVIE_TWD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtwd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtwd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtwd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif

#if MOVIE_TZD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtzd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtzd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtzd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif
	//#NT#2016/10/14#Yuzhe Bian -end

	//#NT#2016/10/27#Omega Yu -begin
	//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvscd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif
	//#NT#2016/10/27#Omega Yu -end

	//#NT#2016/10/17#Bin Xiao -begin
	//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvftg;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvftg.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvftg.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FTG IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvftg.Info.IPLId);
	}
#endif

#if MOVIE_FTG_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvftgfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvftgfd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvftgfd.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FTGFD IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvftgfd.Info.IPLId);
	}
#endif

#if MOVIE_UVAC_FUNC
	{
		// Send YUV to DSP function
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvToUvcFunc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}

#endif


#if (MOVIE_STRM_PATH_USERPROC==ENABLE)
	{

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvToStrmFunc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}

#endif

#if MOVIE_LPR_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvlpr;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif


}



