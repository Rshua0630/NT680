/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/

#ifndef _PRJCFG_H_
#define _PRJCFG_H_

#include "Type.h"
#include "Debug.h"


#define DEBUG_LEVEL_NONE                    0   ///< there is no debug message will show beside using debug_msg
#define DEBUG_LEVEL_ERROR                   1   ///< only debug_err() message will show
#define DEBUG_LEVEL_WARNING                 2   ///< only debug_wrn() and debug_err() message will show
#define DEBUG_LEVEL_INDICATION              3   ///< debug_ind(), debug_wrn() and debug_err() message will show

#define DEBUG_LEVEL                         DEBUG_LEVEL_ERROR   ///< debug level there are DEBUG_LEVEL_NONE/DEBUG_LEVEL_ERROR/DEBUG_LEVEL_WARNING/DEBUG_LEVEL_INDICATION can be select

#if (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define debug_err(msg)                      do{debug_msg("ERR: ");debug_msg msg ;}while(0)
#else
#define debug_err(msg)
#endif

#if (DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)
#define debug_wrn(msg)                      do{debug_msg("WRN: ");debug_msg msg ;}while(0)
#else
#define debug_wrn(msg)
#endif

#if (DEBUG_LEVEL >= DEBUG_LEVEL_INDICATION)
#define debug_ind(msg)                      do{debug_msg("IND: ");debug_msg msg ;}while(0)
#else
#define debug_ind(msg)
#endif
extern BOOL g_bDbgTest;
#define DBG_TEST(fmtstr, args...) do{ if(g_bDbgTest) debug_msg("DBGT: " fmtstr, ##args);  }while(0)

/*
#if (defined(_CPU1_UITRON_) && defined(_CPU2_NONE_))
#if defined(_NETWORK_)
#warning Not support _NETWORK_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_ECOS_) && defined(_CPU2_NONE_))
#if (defined(_NETWORK_) && !defined(_NETWORK_ON_CPU1_) && defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU1_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_UITRON_) && defined(_CPU2_ECOS_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_ECOS_) && defined(_CPU2_ECOS_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_UITRON_) && defined(_CPU2_LINUX_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_ECOS_) && defined(_CPU2_LINUX_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#endif

#if defined(_NETWORK_ERROR_)
#warning Please check your "NETWORK" config in "MakeCommon\MakeOption.txt"
#warning Please check your "CPU1", "CPU2" config in "ModelConfig_[MODEL].txt"
#error (see above)
#endif
*/

#include "PrjCfg_Option.h"

#if (_FPGA_EMULATION_ == ENABLE)
#include "PrjCfg_FPGA.h"
#elif defined(_MODEL_IPCAM1_EVB_)
#include "PrjCfg_IPCam.h"
#elif defined(_MODEL_IPCAM_256M_EVB_)
#include "PrjCfg_IPCam_256M.h"
#elif defined(_MODEL_DVCAM1_EVB_) || \
	  defined(_MODEL_DVCAM2_EVB_) || \
	  defined(_MODEL_PURE_LINUX_) || \
	  defined(_MODEL_EMMC_EVB_) || \
	  defined(_MODEL_ECOS_EVB_)
#include "PrjCfg_DVCam.h"
#elif defined(_MODEL_S5PV2_)
#include "PrjCfg_S5PV2.h"
#elif defined(_MODEL_WiFiSDV_)
#include "PrjCfg_WiFiSDV.h"
#elif defined(_MODEL_ARTOSYN_DVCAM_EVB_)
#include "PrjCfg_ARTOSYN_DVCam.h"
#elif defined(_MODEL_ARTOSYN_IPCAM_EVB_)
#include "PrjCfg_ARTOSYN_IPCam.h"
#elif defined(_MODEL_FIREFLY_)
#include "PrjCfg_FIREFLY.h"
#elif defined(_MODEL_USB_DISK_EVB_)
#include "PrjCfg_USB_DISK.h"
#elif defined(_MODEL_MIO_HT43_)
#include "PrjCfg_MIO_HT43.h"
#elif defined(_MODEL_B5_)
#include "PrjCfg_B5.h"
#elif defined(_MODEL_XF4000_)
#include "PrjCfg_XF4000.h"
#elif defined(_MODEL_CarDV_)
#include "PrjCfg_CarDV.h"
#else //Unknown MODEL
#warning Unknown MODEL?
#warning Please assign your "MODEL" in include "ModelConfig.txt"
#error (see above)
#endif

#include "PrjCfg_Default.h"

#endif //_PRJCFG_H_
