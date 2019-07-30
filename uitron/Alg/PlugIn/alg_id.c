#include "SysKer.h"
#include "SysCommon.h"
#include "alg_id.h"
#include "NvtCv_lib.h"
#include "nvtnn_lib.h"
#include "cvuser_lib.h"
#include "LPR_API.h"
#include "odt_lib.h"

#if ((MOVIE_CDNN_FUNC == ENABLE) || (IPCAM_CDNN_FUNC == ENABLE) || (IPCAM_CDNN2_FUNC == ENABLE))
#include "NvtCDNN.h"
#endif
#if (_ADAS_FUNC_ == ENABLE)
#include "adas_Apps.h"
#endif
#if (_DDD_FUNC_ == ENABLE)
#include "DDD_lib.h"
#endif

#if (_TSR_FUNC_ == ENABLE)
#include "tsr_lib.h"
#endif

#if (_ODP_FUNC_ == ENABLE)
#include "NvtNNFlow.h"
#endif

#if ((MOVIE_LPR_FUNC == ENABLE) || (IPCAM_LPR_FUNC == ENABLE))
#include "LPR_API.h"
#endif


void alg_InstallID(void)
{
#if (_ADAS_FUNC_ == ENABLE)
	ADAS_InstallID();
#endif

#if (_DDD_FUNC_ == ENABLE)
	DDD_InstallID();
#endif

#if (_TSR_FUNC_ == ENABLE)
	TSR_InstallID();
#endif
#if (_ODP_FUNC_ == ENABLE)
	ODP_InstallID();
#endif

#if ((MOVIE_CDNN_FUNC == ENABLE) || (IPCAM_CDNN_FUNC == ENABLE) || (IPCAM_CDNN2_FUNC == ENABLE))
	cdnn_install_id();
#endif

#if (!(defined(_DSP1_NONE_) && defined(_DSP2_NONE_)))
	NvtCv_InstallID();
	nvtnn_install_id();
	cvuser_install_id();
#endif

#if ((MOVIE_LPR_FUNC == ENABLE) || (IPCAM_LPR_FUNC == ENABLE))
	LPR_InstallID();
#endif

#if (MOVIE_ODT_FUNC == ENABLE)
	odt_installid();
#endif

}

