#include "UIFramework.h"

#include "Debug.h"
#include "NVTUserCommand.h"
#include "UIAppPlay.h"

void FlowPB_SliderCBFunc(void)
{
#if 0
	Ux_FlushEventByRange(NVTEVT_CB_SLIDE_KEEP, NVTEVT_CB_SLIDE_KEEP);
	Ux_PostEvent(NVTEVT_CB_SLIDE_KEEP, 1, NVTEVT_CB_SLIDE_KEEP);
#endif
}
