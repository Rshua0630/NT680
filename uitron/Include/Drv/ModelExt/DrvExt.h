#ifndef _DRIVEREXT_H_
#define _DRIVEREXT_H_

#include "Type.h"
#include "ErrorNo.h"
#include "Debug.h"
#include "Utility.h"
#include "modelext_info.h"

// Init IO for external device
extern void Dx_InitIO(void);
// Uninit IO for external device
extern void Dx_UninitIO(void);
// Get modelext resource
extern UINT8 *Dx_GetModelExtCfg(MODELEXT_TYPE type, MODELEXT_HEADER **header);

// command
extern BOOL Dx_OnCommand(CHAR *pcCmdStr);

// Install driver ext OS resource
extern void Install_DrvExt(void);

/////////////////////////////////////////
// DriverExt

#include "DxStorage.h"
#include "DxDisplay.h"

#endif //_DRIVEREXT_H_
