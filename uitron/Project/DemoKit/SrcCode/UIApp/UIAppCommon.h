#include "ImageStream.h"

///////////////////////////////////////////////////////////////////////////////

/**
     Get buffer size of unit.

     Get buffer size of unit.
     @param[in] pUnit           ImagePipe[?] unit
     @param[out] pSize          pointer to buffer size
*/
#define ImageUnit_GetDisp_BufSize(pUnit, id, pSize) \
	{ \
		ISF_PORT* pPort = ImageUnit_In((pUnit), id); \
		ISF_IMG_INFO *pImgInfo = (ISF_IMG_INFO *)pPort->Info; \
		*(pSize) = pImgInfo->ImgSize; \
	}

/**
     Get cast window of unit.

     Get cast window of unit.
     @param[in] pUnit           ImagePipe[?] unit
     @param[out] pWindow        pointer to cast window
*/
#define ImageUnit_GetDisp_CastWindow(pUnit, id, pWindow) \
	{ \
		ISF_PORT* pPort = ImageUnit_In((pUnit), id); \
		*(pWindow) = pPort->Info.PreWindow; \
	}


///////////////////////////////////////////////////////////////////////////////


/**
     Get maximum zoom index of unit.

     Get maximum zoom index of unit.
     @param[in] pUnit           ImagePipe[?] unit
     @param[out] pMaxIdx        pointer to maximum index value
*/
#define ImageUnit_GetZoom_MaxIdx(pUnit, pMaxIdx) \
	{ \
		ISF_PORT* pPort = ImageUnit_In((pUnit), ISF_IN1); \
		ISF_IMG_INFO *pImgInfo = (ISF_IMG_INFO *)pPort->Info; \
		*(pMaxIdx) = pImgInfo->PreWindow.y; \
	}

/**
     Get current zoom index of unit.

     Get current zoom index of unit.
     @param[in] pUnit           ImagePipe[?] unit
     @param[out] pCurIdx        pointer to current index value
*/
#define ImageUnit_GetZoom_CurIdx(pUnit, pCurIdx) \
	{ \
		ISF_PORT* pPort = ImageUnit_In((pUnit), ISF_IN1); \
		ISF_IMG_INFO *pImgInfo = (ISF_IMG_INFO *)pPort->Info; \
		*(pCurIdx) = pImgInfo->PreWindow.w; \
	}

/**
     Get zoom ratio of unit.

     Get zoom ratio of unit.
     @param[in] pUnit           ImagePipe[?] unit
     @param[out] pRatio         pointer to zoom ratio value
*/
#define ImageUnit_GetZoom_Ratio(pUnit, pRatio) \
	{ \
		ISF_PORT* pPort = ImageUnit_In((pUnit), ISF_IN1); \
		ISF_IMG_INFO *pImgInfo = (ISF_IMG_INFO *)pPort->Info; \
		*(pRatio) = pImgInfo->PreWindow.h; \
	}

/**
     Set current zoom index of unit.

     Set current zoom index of unit.
     @param[in] pUnit           ImagePipe[?] unit
     @param[in] CurIdx          Current index value
*/
#define ImageUnit_SetZoom_CurIdx(pUnit, CurIdx) \
	{ \
		ImageUnit_Begin((pUnit), 0); \
		ImageUnit_SetVdoPreWindow(ISF_IN1, (CurIdx), ISF_VALUE_KEEP, ISF_VALUE_KEEP, ISF_VALUE_KEEP); \
		ImageUnit_End(); \
		ImageStream_UpdateAll(&ISF_Stream[0]); \
	}

//ImageStream_UpdateSize();



#define APPMEM_ALL  0xffffffff

extern void AppMem_Open(void);
extern MEM_RANGE AppMem_Alloc(char *name, UINT32 size);
extern void AppMem_Dump(void);
extern void AppMem_Close(void);

#define APPUSER_MOVIE_BASE              0x14001000
#define APPUSER_PHOTO_BASE              0x14002000
#define APPUSER_PLAYBACK_BASE           0x14003000
#define APPUSER_SLEEP_BASE              0x14004000

#define APPUSER_USBMOVIE_BASE           0x14011000
#define APPUSER_USBPHOTO_BASE           0x14012000
#define APPUSER_USBPLAYBACK_BASE        0x14013000
#define APPUSER_USBCHARGE_BASE          0x14014000
#define APPUSER_USBPRINT_BASE           0x14015000
#define APPUSER_USBMENU_BASE            0x14016000

#define APPUSER_WIFICMD_BASE            0x14020000
#define APPUSER_NETMOVIE_BASE           0x14021000
#define APPUSER_NETPHOTO_BASE           0x14022000
#define APPUSER_NETPLAYBACK_BASE        0x14023000

#define APPUSER_IPCAM_BASE              0x14031000
#define APPUSER_IPCAMPB_BASE			0x14032000
#define APPUSER_SETUP_BASE              0x14080000



