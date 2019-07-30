
#include "SysCfg.h"
#include "IspSudoApi.h"
#include "MsdcNvtCb_IQSim.h"
#include "SysCommon.h"


#if (MSDCVENDOR_NVT == ENABLE)
#define THIS_DBGLVL         2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          IspSudoCb
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"


static BOOL m_bIspSudoInit = FALSE;

static void IspSudoCb_Init(void)
{
	UINT8 *pMemPool = NULL;
	UINT32 MemSize = 0;
	ISPSUDO_INIT IspSudoInit = {0};

	pMemPool = (UINT8 *)OS_GetMempoolAddr(POOL_ID_ISPSUDO);
	MemSize = OS_GetMempoolSize(POOL_ID_ISPSUDO);

	if (m_bIspSudoInit == FALSE) {
		IspSudoInit.uiApiVer    = ISPSUDO_API_VERSION;
		IspSudoInit.pMemCache   = (UINT8 *)pMemPool;
		IspSudoInit.uiSizeCache = MemSize;
		IspSudoInit.fpEvent     = NULL;

		if (!IspSudo_Init(&IspSudoInit)) {
			DBG_ERR("IspSudo_Init() failed!\r\n");
			return;
		}

		IspSudoRegSi_IQSim();    // register single direction functions

		m_bIspSudoInit = TRUE;
	}
}

void IspSudoCb_Net(BOOL bTurnOn)
{
	IspSudoCb_Init();

	IspSudo_Switch2IpcPath(bTurnOn);

	IspSudo_Net(bTurnOn);
}
#endif

