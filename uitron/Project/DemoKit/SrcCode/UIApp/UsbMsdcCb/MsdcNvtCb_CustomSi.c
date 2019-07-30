#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCb_CustomSi.h"
#include "SysCfg.h"

//Step 1: Declare custom functions
//Declare Gets Functions
static void GetSayHi(void);
static void GetSayHello(void);
//Declare Sets Functions
static void SetSayHi(void);
static void SetSayHello(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void) = {
	GetSayHi,                   // 0, #define ID_GET_SAY_HI    0 //in AP Side
	GetSayHello,                // 1, #define ID_GET_SAY_HELLO 1 //in AP Side
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void) = {
	SetSayHi,                   // 0, #define ID_SET_SAY_HI    0 //in AP Side
	SetSayHello,                // 1, #define ID_SET_SAY_HELLO 1 //in AP Side
};

//Step 4: Provide API for Register Single Direction Functions
BOOL MsdcNvtRegSi_CustomSi(void)
{
	return MsdcNvt_AddCallback_Si(MSDCCustomGet, sizeof(MSDCCustomGet) / sizeof(MSDCCustomGet[0]), MSDCCustomSet, sizeof(MSDCCustomSet) / sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
static void GetSayHi(void)
{
	UINT32  *pData    = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32   uiLength = MsdcNvt_GetTransSize();

	//1.  pData could be a structure, just the PC side and Firmware side use the
	//    same structure define. such as
	//    STRUCT_DATA *pData =  (STRUCT_DATA*)MsdcNvt_GetDataBufferAddress();
	//    then you can check size as if(uiLength!=sizeof(STRUCT_DATA))
	//2.  pData could be a string. if the string is not fixed length, you have
	//    to remove check size if condition for invalid data. and the string
	//    length have to smaller than uiLength-1

	//uiLength have to equal to PC data size
	//if PC only send 4 bytes data, here uiLength have to be 4
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	debug_msg("@@@@@@ Get Say Hi @@@@@@\r\n");
	*pData = 12345;
}

static void GetSayHello(void)
{
	UINT32  *pData    = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32   uiLength = MsdcNvt_GetTransSize();

	debug_msg("uiLength=%d\r\n", uiLength);

	//1.  pData could be a structure, just the PC side and Firmware side use the
	//    same structure define. such as
	//    STRUCT_DATA *pData =  (STRUCT_DATA*)MsdcNvt_GetDataBufferAddress();
	//    then you can check size as if(uiLength!=sizeof(STRUCT_DATA))
	//2.  pData could be a string. if the string is not fixed length, you have
	//    to remove check size if condition for invalid data. and the string
	//    length have to < uiLength

	//uiLength have to equal to PC data size
	//if PC only send 4 bytes data, here uiLength have to be 4
	if (uiLength != sizeof(UINT32)) {
		return;
	}

	debug_msg("@@@@@@ Get Say Hello @@@@@@\r\n");
	*pData = 67890;
}

static void SetSayHi(void)
{
	UINT32  i;
	UINT32 *pData    = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32  uiLength = MsdcNvt_GetTransSize();

	debug_msg("###### Set Say Hi ###### \r\n");

	//1.  pData could be a structure, just the PC side and Firmware side use the
	//    same structure define. such as
	//    STRUCT_DATA *pData =  (STRUCT_DATA*)MsdcNvt_GetDataBufferAddress();
	//    then you can check size as if(uiLength!=sizeof(STRUCT_DATA))
	//2.  pData could be a string.
	if (uiLength) {
		for (i = 0; i < uiLength; i += 4) {
			debug_err(("Data[%d]=%d\r\n", i, *pData));
			pData++;
		}
	}
}

static void SetSayHello(void)
{
	UINT32  i;
	UINT32 *pData    = (UINT32 *)MsdcNvt_GetDataBufferAddress();
	UINT32  uiLength = MsdcNvt_GetTransSize();

	debug_msg("###### Set Say Hello ###### \r\n");

	//1.  pData could be a structure, just the PC side and Firmware side use the
	//    same structure define. such as
	//    STRUCT_DATA *pData =  (STRUCT_DATA*)MsdcNvt_GetDataBufferAddress();
	//    then you can check size as if(uiLength!=sizeof(STRUCT_DATA))
	//2.  pData could be a string.
	if (uiLength) {
		for (i = 0; i < uiLength; i += 4) {
			debug_err(("Data[%d]=%d\r\n", i, *pData));
			pData++;
		}
	}
}