#include "SysCfg.h"
#include "UMSDC.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCb.h"
#include "SxCmd.h"
#include "NvtIpcAPI.h"
#include "SysCommon.h"
#if defined(_BSP_NA51000_)
#include "usb3dev.h"
#endif

#if(MSDCVENDOR_NVT==ENABLE)

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          MsdcNvtCb
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//------------------------------------------------------------------------------
// Multi Module Selection
//------------------------------------------------------------------------------
#define CFG_MSDC_NVT_CB_PHOTO   PHOTO_MODE  //!< Preview / Capture
#define CFG_MSDC_NVT_CB_DISP    ENABLE  //!< Display Engine Adjustment (IDE Gamma)
#define CFG_MSDC_NVT_CB_FILE    ENABLE  //!< Remote File System
#define CFG_MSDC_NVT_CB_ADJ     ENABLE  //!< Universal Adjustment
#define CFG_MSDC_NVT_CB_UPDFW   ENABLE  //!< Update Fw
#if defined(_MODEL_IPCAM1_EVB_) || \
	defined(_MODEL_DVCAM1_EVB_) || \
	defined(_MODEL_ECOS_EVB_)
#define CFG_MSDC_NVT_CB_EXAM    MSDCVENDOR_NVT_EXAM
#else
#define CFG_MSDC_NVT_CB_EXAM    DISABLE
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Single Module Selection
//------------------------------------------------------------------------------
#define SI_MODULE_NONE          0  //!< No Single Module
#define SI_MODULE_CUSTOM_SI     1  //!< Customer Develop Module
#define SI_MODULE_IQSIM         2
#define SI_MODULE_AE            3
#define SI_MODULE_TIME_SI       4
#define SI_MODULE_CALUSB        5
#define SI_MODULE_FRNN          6
//Configure your Single Module. Module can be SI_MODULE_NONE / SI_MODULE_CUSTOM_SI / SI_MODULE_IQSIM..etc
#define CFG_SI_MODULE SI_MODULE_IQSIM
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Cache Memory Selection
//------------------------------------------------------------------------------
#define CACHE_USE_LOCAL             0
#define CACHE_USE_MEM_POOL          1

//Configure your Cache use Local Array or Memory Pool
#define CFG_CACHE_USE               CACHE_USE_MEM_POOL //!< CACHE_USE_MEM_POOL / CACHE_USE_LOCAL
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Config Multi Module Include Headers
//------------------------------------------------------------------------------
#if(CFG_MSDC_NVT_CB_PHOTO==ENABLE)
#include "MsdcNvtCb_Photo.h"
#endif
#if(CFG_MSDC_NVT_CB_DISP==ENABLE)
#include "MsdcNvtCb_Disp.h"
#endif
#if(CFG_MSDC_NVT_CB_FILE==ENABLE)
#include "MsdcNvtCb_File.h"
#endif
#if(CFG_MSDC_NVT_CB_ADJ==ENABLE)
#include "MsdcNvtCb_Adj.h"
#endif
#if(CFG_MSDC_NVT_CB_UPDFW==ENABLE)
#include "MsdcNvtCb_UpdFw.h"
#endif
#if(CFG_MSDC_NVT_CB_EXAM==ENABLE)
#include "MsdcNvtCb_Exam.h"
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Config Single Module Include Headers
//------------------------------------------------------------------------------
#if(CFG_SI_MODULE==SI_MODULE_CUSTOM_SI)
#include "MsdcNvtCb_CustomSi.h"
#elif (CFG_SI_MODULE==SI_MODULE_IQSIM)
#include "MsdcNvtCb_IQSim.h"
#elif (CFG_SI_MODULE==SI_MODULE_AE)
#include "ae_alg.h"
#elif (CFG_SI_MODULE==SI_MODULE_TIME_SI)
#include "MsdcNvtCb_TimeSi.h"
#elif (CFG_SI_MODULE==SI_MODULE_FRNN)
#include "MsdcNvtCb_FRNN.h"
#endif

_ALIGNED(4) const static UINT8 gMSDCManuStrDesc[] = {
	USB_VENDER_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_VENDER_DESC_STRING
};

_ALIGNED(4) const static UINT8 gMSDCProdStrDesc[] = {
	USB_PRODUCT_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_PRODUCT_DESC_STRING
};
_ALIGNED(16) const static UINT8 gMSDCInquiryData[36] = {
	0x00, 0x80, 0x05, 0x02, 0x20, 0x00, 0x00, 0x00,
	//Vendor identification,
	USB_VENDER_STRING,/*'\0',*/
	USB_PRODUCT_STRING, '\0',
	USB_PRODUCT_REVISION
};

static BOOL m_bInit = FALSE;
static GXUSB_HDL_ID m_ConnectedID = GXUSB_HDL_ID_INVALID;

static void MsdcNvtCb_MakerInit(USB_MSDC_INFO *pUSBMSDCInfo)
{
	pUSBMSDCInfo->pManuStringDesc = (USB_STRING_DESC *)gMSDCManuStrDesc;
	pUSBMSDCInfo->pProdStringDesc = (USB_STRING_DESC *)gMSDCProdStrDesc;

	//USBMakerInit_SerialNumber();
	pUSBMSDCInfo->pSerialStringDesc = NULL;

	pUSBMSDCInfo->VID = USB_VID;
	pUSBMSDCInfo->PID = USB_PID_MSDC;

	pUSBMSDCInfo->pInquiryData = (UINT8 *)&gMSDCInquiryData[0];
}

#if(CFG_CACHE_USE==CACHE_USE_LOCAL)
_ALIGNED(4) static UINT8 m_MsdcNvtCache[MSDCNVT_REQUIRE_MIN_SIZE];
#if(CFG_MSDC_NVT_CB_EXAM==ENABLE)
_ALIGNED(4) static UINT8 m_MsdcNvtExam[MSDCNVTCBEXAM_MSG_SIZE];
#endif
#endif

SX_CMD_ENTRY msdcnvt[];

static void xMsdcNvtCb_Init(void)
{
	UINT8              *pMemPool = NULL;
	UINT32              MemSize = 0;
	MSDCNVT_INIT        MsdcNvtInit = {0};

	//Due to PC Limit Max Packet Size is 64k Bytes, so MsdcVendor work need to cache buffer with 0x24000 bytes
#if(CFG_CACHE_USE==CACHE_USE_LOCAL)
	pMemPool    = m_MsdcNvtCache;
	MemSize     = sizeof(m_MsdcNvtCache);
#elif(CFG_CACHE_USE==CACHE_USE_MEM_POOL)
	pMemPool    = (UINT8 *)OS_GetMempoolAddr(POOL_ID_MSDCNVT);
	MemSize     = OS_GetMempoolSize(POOL_ID_MSDCNVT);
#else
#error "You have to select a cache source!"
#endif

	if (m_bInit == FALSE) {
		MsdcNvtInit.uiApiVer         = MSDCNVT_API_VERSION;
		MsdcNvtInit.pMemCache        = (UINT8 *)pMemPool;
		MsdcNvtInit.uiSizeCache      = MemSize;
		MsdcNvtInit.bHookDbgMsg      = TRUE;
		MsdcNvtInit.uiUartIdx        = 0;
		MsdcNvtInit.fpEvent          = NULL;
		MsdcNvtInit.pMsdc            = NULL;
		if (!MsdcNvt_Init(&MsdcNvtInit)) {
			DBG_ERR("MsdcNvt_Init() Failed!\r\n");
			return;
		}

		//Register Bi - Direction Functions
#if(CFG_MSDC_NVT_CB_PHOTO==ENABLE)
		MsdcNvtRegBi_Photo();
#endif
#if(CFG_MSDC_NVT_CB_DISP==ENABLE)
		MsdcNvtRegBi_Disp();
#endif
#if(CFG_MSDC_NVT_CB_FILE==ENABLE)
		{
			MSDCNVTCBFILE_INIT Init = {0};
			Init.uiWorkingAddr = OS_GetMempoolAddr(POOL_ID_APP);
			Init.uiWorkingSize = POOL_SIZE_APP;
			MsdcNvtCbFile_Init(&Init);
			MsdcNvtRegBi_File();
		}
#endif
#if(CFG_MSDC_NVT_CB_ADJ==ENABLE)
		MsdcNvtRegBi_Adj();
#endif
#if(CFG_MSDC_NVT_CB_UPDFW==ENABLE)
		MsdcNvtRegBi_UpdFw();
#endif
#if(CFG_MSDC_NVT_CB_EXAM==ENABLE)
		MSDCNVTCBEXAM_INIT ExamInit = {0};
#if(CFG_CACHE_USE==CACHE_USE_LOCAL)
		ExamInit.uiWorkingAddr = (UINT32)m_MsdcNvtExam;
		ExamInit.uiWorkingSize = sizeof(m_MsdcNvtExam);
#elif(CFG_CACHE_USE==CACHE_USE_MEM_POOL)
		ExamInit.uiWorkingAddr = OS_GetMempoolAddr(POOL_ID_MSDCNVT_EXAM);
		ExamInit.uiWorkingSize = OS_GetMempoolSize(POOL_ID_MSDCNVT_EXAM);
#endif
		MsdcNvtCbExam_Init(&ExamInit);
		//set it as "MONITOR" for auto-test server
		MsdcNvtCb_ExamSetName("TARGET");
		MsdcNvtRegBi_Exam();
		//set key map (ks, ki, ...)
		extern BOOL User_CommandFilter(CHAR * pcCmdStr, int len);
		MsdcNvtCb_RegCmdFilterCB(User_CommandFilter);
		//#NT#2016/12/13#Niven Cho -end
#endif
		//#NT#2016/11/10#Niven Cho -end

		//Register Single Direction Functions
#if(CFG_SI_MODULE==SI_MODULE_CUSTOM_SI)
		MsdcNvtRegSi_CustomSi();
#elif (CFG_SI_MODULE==SI_MODULE_IQSIM)
		MsdcNvtRegSi_IQSim();
#elif (CFG_SI_MODULE==SI_MODULE_AE)
		AEAlg2_MsdcCB();
#elif (CFG_SI_MODULE==SI_MODULE_TIME_SI)
		MsdcNvtRegSi_TimeSi();
#elif (CFG_SI_MODULE==SI_MODULE_CALUSB)
		CalUSB_MsdcCB();
#elif (CFG_SI_MODULE==SI_MODULE_FRNN)
		MsdcNvtRegSi_FRNN();
#endif
		m_bInit = TRUE;
	}
}

void MsdcNvtCb_Open(MSDCNVTCB_OPEN *pOpen)
{
	UINT8              *pMemPool = NULL;
	UINT32              MemSize = 0;
	USB_MSDC_INFO       MSDCInfo = {0};
	MSDCNVT_LUN        *pLun = MsdcNvt_GetNullLun();
	MSDC_OBJ           *pMsdc = NULL;

	if (m_ConnectedID != GXUSB_HDL_ID_INVALID) {
		DBG_ERR("has connected by: %d\r\n", m_ConnectedID);
		return;
	}

	//Due to PC Limit Max Packet Size is 64k Bytes, so MsdcVendor work need to cache buffer with 0x24000 bytes
#if(CFG_CACHE_USE==CACHE_USE_LOCAL)
	pMemPool    = m_MsdcNvtCache;
	MemSize     = sizeof(m_MsdcNvtCache);
#elif(CFG_CACHE_USE==CACHE_USE_MEM_POOL)
	pMemPool    = (UINT8 *)OS_GetMempoolAddr(POOL_ID_MSDCNVT);
	MemSize     = OS_GetMempoolSize(POOL_ID_MSDCNVT);
#else
#error "You have to select a cache source!"
#endif

	xMsdcNvtCb_Init();

	MSDCInfo.uiMsdcBufAddr = (UINT32)pMemPool;
	MSDCInfo.uiMsdcBufSize = (UINT32)MemSize;
	if (pOpen->fpUSBMakerInit == NULL) {
		MsdcNvtCb_MakerInit(&MSDCInfo);
	} else {
		pOpen->fpUSBMakerInit(&MSDCInfo);
	}

	m_ConnectedID = pOpen->GxUsbHdlID;
	pMsdc = USB_GetMsdcObj(m_ConnectedID);

	if (pMsdc == NULL) {
		DBG_ERR("NULL Msdc Object\r\n");
		return;
	}

	MsdcNvt_SetMsdcObj(pMsdc);

	//The callback functions for the MSDC Vendor command.
	//If project doesn't need the MSDC Vendor command, set this callback function as NULL.
	MSDCInfo.msdc_check_cb = MsdcNvt_Verify_Cb;
	MSDCInfo.msdc_vendor_cb = MsdcNvt_Vendor_Cb;

	//Assign a Null Lun
	MSDCInfo.pStrgHandle[0] = pLun->hStrg;
	MSDCInfo.msdc_type[0] = pLun->Type;
	MSDCInfo.msdc_storage_detCB[0] = pLun->fpStrgDetCb;
	MSDCInfo.msdc_strgLock_detCB[0] = pLun->fpStrgLockCb;
	MSDCInfo.LUNs = pLun->uiLUNs;
	// Open MSDC task
	pMsdc->SetConfig(USBMSDC_CONFIG_ID_RC_DISABLESTALL, TRUE);
	// auto detect speed
	//Msdc_SetConfig(USBMSDC_CONFIG_ID_SPEED, U3DEV_SPEED_HS);
	if (pMsdc->Open(&MSDCInfo) != E_OK) {
		DBG_ERR("Error open USB MSDC task\r\n");
	}

	SxCmd_AddTable(msdcnvt);
}

void MsdcNvtCb_Close(void)
{
	MSDC_OBJ *pMsdc = USB_GetMsdcObj(m_ConnectedID);

	m_ConnectedID = GXUSB_HDL_ID_INVALID;

	if (pMsdc != NULL) {
		pMsdc->Close();
	}
}

void MsdcNvtCb_Attach(MSDCNVTCB_ATTACH *pAttach)
{
	xMsdcNvtCb_Init();

	pAttach->pInfo->msdc_check_cb = MsdcNvt_Verify_Cb;
	pAttach->pInfo->msdc_vendor_cb = MsdcNvt_Vendor_Cb;

	SxCmd_AddTable(msdcnvt);
}

void MsdcNvtCb_Net(BOOL bTurnOn)
{
	xMsdcNvtCb_Init();
	MsdcNvt_Net(bTurnOn);
}

GXUSB_HDL_ID MsdcNvtCb_GetConnectedID(void)
{
	return m_ConnectedID;
}

static BOOL MsdcNvtCb_SxCmd_Off(CHAR *strCmd)
{
	MsdcNvtCb_Close();
	return TRUE;
}

SXCMD_BEGIN(msdcnvt, "MsdcNvt Utility")
SXCMD_ITEM("off", MsdcNvtCb_SxCmd_Off, "force unplug.")
SXCMD_END()
#endif //(MSDCVENDOR_NVT==ENABLE)
