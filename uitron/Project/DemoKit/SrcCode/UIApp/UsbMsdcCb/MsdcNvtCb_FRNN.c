#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCb_FRNN.h"
#include "SysCfg.h"

//Step 1: Declare custom functions
//Declare Gets Functions
static void GetRegisterResult(void);
//Declare Sets Functions
static void SetRegisterStart(void);
static void SetRegisterStop(void);
static void SetRegister(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void) = {
	GetRegisterResult,                   // 0
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void) = {
	SetRegisterStart,
	SetRegisterStop,
	SetRegister,
};

//Step 4: Provide API for Register Single Direction Functions
BOOL MsdcNvtRegSi_FRNN(void)
{
	return MsdcNvt_AddCallback_Si(MSDCCustomGet, sizeof(MSDCCustomGet) / sizeof(MSDCCustomGet[0]), MSDCCustomSet, sizeof(MSDCCustomSet) / sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
static void GetRegisterResult(void)
{
	BOOL  *pData    = (BOOL *)MsdcNvt_GetDataBufferAddress();
	UINT32   uiLength = MsdcNvt_GetTransSize();

	if (uiLength != sizeof(BOOL)) {
		return;
	}

	*pData = TRUE;
	CHKPNT;
}

static void SetRegisterStart(void)
{
	CHKPNT;
}

static void SetRegisterStop(void)
{
	CHKPNT;
}

static void SetRegister(void)
{
	CHAR *pData = (CHAR *)MsdcNvt_GetDataBufferAddress();
	debug_msg("Got: %s\r\n", pData);
}