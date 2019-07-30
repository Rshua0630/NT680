#include "Type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "DrvExt.h"

#include "Debug.h"
#include "sdio.h"
#include "pad.h"
#include "StrgDef.h"
#include <string.h>
#include "emb_partition_info.h"
#include "nand.h"

#if defined(_EMBMEM_NONE_)
#define EMB_GETSTRGOBJ(x) 0 //Always NULL
#elif defined(_EMBMEM_NAND_) || defined(_EMBMEM_SPI_NAND_)
#include "nand.h"
#define EMB_GETSTRGOBJ(x) nand_getStorageObject(x)
#elif defined(_EMBMEM_SPI_NOR_)
#include "spiflash.h"
#include "RamDisk.h"
#define EMB_GETSTRGOBJ(x) spiflash_getStorageObject(x)
#elif defined(_EMBMEM_EMMC_)
#include "sdio.h"
#define EMB_GETSTRGOBJ(x) sdio3_getStorageObject(x)
static BOOL xSdioDet(void)
{
	return TRUE;
}
static BOOL xSdioWp(void)
{
	return FALSE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//public func
UINT32 DrvEmbMemGetcaps0(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps1(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps2(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps3(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps4(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps5(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps6(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps7(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps8(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemInit(void *pInitParam); // Set Init Parameters
UINT32 DrvEmbMemOpen(void); // Common Constructor
UINT32 DrvEmbMemClose(void); // Common Destructor
UINT32 DrvEmbMemState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvEmbMemControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvEmbMemCommand(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevEmbMem0 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM0,
	STORAGE_VER,
	"LOADER",
	0, 0, 0, 0,
	DrvEmbMemGetcaps0,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem1 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM1,
	STORAGE_VER,
	"MODELEXT",
	0, 0, 0, 0,
	DrvEmbMemGetcaps1,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem2 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM2,
	STORAGE_VER,
	"UITRON",
	0, 0, 0, 0,
	DrvEmbMemGetcaps2,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem3 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM3,
	STORAGE_VER,
	"UBOOT",
	0, 0, 0, 0,
	DrvEmbMemGetcaps3,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem4 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM4,
	STORAGE_VER,
	"LINUX",
	0, 0, 0, 0,
	DrvEmbMemGetcaps4,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem5 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM5,
	STORAGE_VER,
	"DSP1",
	0, 0, 0, 0,
	DrvEmbMemGetcaps5,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem6 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM6,
	STORAGE_VER,
	"DSP2",
	0, 0, 0, 0,
	DrvEmbMemGetcaps6,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem7 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM7,
	STORAGE_VER,
	"PSTORE",
	0, 0, 0, 0,
	DrvEmbMemGetcaps7,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

DX_OBJECT gDevEmbMem8 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM8,
	STORAGE_VER,
	"FAT",
	0, 0, 0, 0,
	DrvEmbMemGetcaps8,
	0,
	DrvEmbMemInit,
	DrvEmbMemOpen,
	DrvEmbMemClose,
	DrvEmbMemState,
	DrvEmbMemControl,
	DrvEmbMemCommand,
	0,
};

static UINT32 xDrvEmbMemGetUsedSize(EMB_PARTITION *pPat, UINT32 nPat)
{
	UINT32 i;
	UINT32 uiOffset = 0;
	for (i = 0; i < nPat; i++) {
		if (pPat->EmbType != EMBTYPE_UNKNOWN) {
			if (pPat->PartitionOffset < uiOffset) {
				DBG_ERR("Partition[%d] overlaps previous one.\r\n");
				return 0;
			}
			uiOffset = pPat->PartitionOffset + pPat->PartitionSize;
		}

		pPat++;
	}
	return uiOffset;
}

static UINT32 xDrvEmbMemGetcaps(UINT32 DxType, UINT32 CapID, UINT32 Param1)
{
	UINT32 v = 0;
	if (CapID == STORAGE_CAPS_HANDLE) {
		const UINT32 Map[] = {
			STRG_OBJ_FW_RSV1,
			STRG_OBJ_FW_RSV2,
			STRG_OBJ_FW_RSV3,
			STRG_OBJ_FW_RSV4,
			STRG_OBJ_FW_RSV5,
			STRG_OBJ_FW_RSV6,
			STRG_OBJ_FW_RSV7,
			STRG_OBJ_PSTORE1,
			STRG_OBJ_FAT1
		};
		if (DxType >= sizeof(Map) / sizeof(Map[0])) {
			DBG_ERR("out of id: %d\r\n", DxType);
			return 0;
		}

#if defined(_EMBMEM_NAND_) || defined(_EMBMEM_SPI_NAND_)
        nand_init_fat_storage_object();
#endif
		v = (UINT32)EMB_GETSTRGOBJ(Map[DxType]);
		//speical case handle
#if defined(_EMBMEM_SPI_NOR_)
		if (DxType == DX_TYPE_EMBMEM8) { //FAT change to ramdisk
			v = (UINT32)ramdsk_getStorageObject();
		}
#endif
	}
	return v;
}

UINT32 DrvEmbMemGetcaps0(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM0, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps1(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM1, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps2(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM2, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps3(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM3, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps4(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM4, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps5(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM5, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps6(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM6, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps7(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM7, CapID, Param1);
}
UINT32 DrvEmbMemGetcaps8(UINT32 CapID, UINT32 Param1)
{
	return xDrvEmbMemGetcaps(DX_TYPE_EMBMEM8, CapID, Param1);
}

UINT32 DrvEmbMemInit(void *pInitParam) // Set Init Parameters
{
	DXSTRG_INIT *g_pInit = (DXSTRG_INIT *)pInitParam; //fat and pstore not complete

	if (!g_pInit) {
		return DX_PARAM_ERROR;
	}

	DBG_FUNC_BEGIN("\r\n");


#if defined(_EMBMEM_EMMC_)
	static BOOL bEmmcInitOnce = FALSE;
	if (!bEmmcInitOnce) {
		sdio3_setCallBack(SDIO_CALLBACK_CARD_DETECT, (SDIO_CALLBACK_HDL)xSdioDet);
		sdio3_setCallBack(SDIO_CALLBACK_WRITE_PROTECT, (SDIO_CALLBACK_HDL)xSdioWp);
#if (_FPGA_EMULATION_ == ENABLE)
		// Support default speed only
		sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS);
		// Source clock 96 MHz can run faster than default value
		sdio3_setConfig(SDIO_CONFIG_ID_SRCLK,                   SDIO_SRC_CLK_96MHZ);
#else
		// Support default speed + high speed
		sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS);
		// Default speed driving -> 5.0 mA
		sdio3_setConfig(SDIO_CONFIG_ID_DS_DRIVING,              50);
		// High speed driving    -> 10.0 mA
		sdio3_setConfig(SDIO_CONFIG_ID_HS_DRIVING,              100);
		// Adjust driving/sink according to operating mode
		sdio3_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,         TRUE);
#endif
		sdio3_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,               SD_HOST_BUS_4_BITS);
		sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID, (UINT32)"NOVATEK");
		sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE, (UINT32)"SD_VOL");
		sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,   TRUE);
		bEmmcInitOnce = TRUE;
	}
#elif defined(_EMBMEM_SPI_NAND_)
	static BOOL bSpiNandInitOnce = FALSE;
	if (!bSpiNandInitOnce) {
		nand_setConfig(NAND_CONFIG_ID_FREQ, 80000000);
		nand_setConfig(NAND_CONFIG_ID_NAND_TYPE, NANDCTRL_SPI_NAND_TYPE);
		bSpiNandInitOnce = TRUE;
	}
#elif defined(_EMBMEM_SPI_NOR_)
	static BOOL bSpiNandInitOnce = FALSE;
	if (!bSpiNandInitOnce) {
		nand_setConfig(NAND_CONFIG_ID_NAND_TYPE, NANDCTRL_SPI_NOR_TYPE);
		bSpiNandInitOnce = TRUE;
	}
#endif

	{
		DXSTRG_PARTITION *p = &(g_pInit->prt);
		DX_HANDLE DxNandDev = Dx_GetObject(p->uiDxClassType);
		STORAGE_OBJ *pStrg = (STORAGE_OBJ *)Dx_Getcaps(DxNandDev, STORAGE_CAPS_HANDLE, 0);
		UINT32 blksize = 0;

		pStrg->Lock();


		if (p->uiDxClassType == (DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM0)) {
			pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
			pStrg->SetParam(STRG_SET_PARTITION_SECTORS, p->uiPhyAddr, p->uiPhySize);
			pStrg->GetParam(STRG_GET_BEST_ACCESS_SIZE, (UINT32)&blksize, 0); //get block size
			if (p->uiPhyAddr != 0) {
				DBG_ERR("loader area have to be at phyical address 0.\r\n");
				return E_PAR;
			}
			if (blksize != _EMBMEM_BLK_SIZE_) {
				DBG_FATAL("EMBMEM_BLK_SIZE in ModelConfig set wrong, must be 0x%0X. Set it right and rebuild ModelExt and Project\r\n", blksize);
				return E_PAR;
			}

			MODELEXT_HEADER *header;
			EMB_PARTITION *pEmbPat = (EMB_PARTITION *)Dx_GetModelExtCfg(MODELEXT_TYPE_EMB_PARTITION, &header);

			if (!pEmbPat || !header) {
				DBG_ERR("MODELEXT_TYPE_EMB_PARTITION is null\n");
				return E_SYS;
			}

			UINT32 PhySize;
			UINT32 UsedSize = xDrvEmbMemGetUsedSize(pEmbPat, header->number);
			pStrg->GetParam(STRG_GET_DEVICE_PHY_SIZE, (UINT32)&PhySize, 0);

			if (UsedSize == 0) {
				return E_PAR;
			}

			if (UsedSize > PhySize) {
				DBG_ERR("UsedSize (%08X) > PhySize (%08X)\r\n", UsedSize, PhySize);
				return E_PAR;
			}

		} else {
			pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
			pStrg->SetParam(STRG_SET_PARTITION_RSV_SECTORS, p->uiResvSize, 0);
			pStrg->SetParam(STRG_SET_PARTITION_SECTORS, p->uiPhyAddr, p->uiPhySize);
		}

		pStrg->Unlock();
	}

	return DX_OK;
}

UINT32 DrvEmbMemOpen(void) // Common Constructor
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DrvEmbMemClose(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DrvEmbMemState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		DBG_IND("get %08x\r\n", StateID);
		switch (StateID) {
		default:
			DBG_ERR("state=0x%02X not support!\r\n", StateID);
			break;
		}
		return rvalue;
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DBG_IND("set %08x\r\n", StateID);
		switch (StateID) {
		default:
			DBG_ERR("state=0x%02X not support!\r\n", StateID);
			break;
		}
	}
	return DX_OK;
}

UINT32 DrvEmbMemControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");
	DBG_IND("ctrl %08x\r\n", CtrlID);

	switch (CtrlID) {
	default:
		DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
		break;
	}
	return DX_OK;
}

UINT32 DrvEmbMemCommand(CHAR *pcCmdStr) //General Command Console
{
	switch (*pcCmdStr) {
	case 'd':
		/*if (!strncmp(pcCmdStr, "Nand dump", 9))
		{
		    return TRUE;
		}*/
		break;
	}
	return FALSE;
}


