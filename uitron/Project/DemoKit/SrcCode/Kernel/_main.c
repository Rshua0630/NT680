/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       _main.c
    @ingroup

    @brief      first C entry function (low-level function to start Kernel)
                first C entry function( low-level function to start Kernel)

    @note       Nothing.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"
#include "bin_info.h"
#include "modelext_info.h"
#include <math.h>
#include "PrjCfg.h"
#include "Driver.h"
#include "DxSys.h"
#include "DrvExt.h"
#include "GxSystem.h"
#include "dram_partition_info.h"
#include "dma.h"
#include "SysMain.h"
#include "clock.h"
#include "HwPower.h"
#include "wdt.h"
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/02/18#Lincy Lin -end

#define _MAIN_BREAK         DISABLE

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          os_main
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
extern void user_exceptionCB(UINT32 *tbl);
extern void user_stackoverflowCB(UINT32 tcb);
EXP_HANDLER user_exp_handler = {
	user_exceptionCB,
	user_stackoverflowCB
};
#if(_MAIN_BREAK == ENABLE)
void _main_break(void);
#endif
void _fill_draminfo(void);
void _clear_stack(void);

TM_BOOT_ARRAY(128);
static UINT32 LdLoadTime = 0;
#if (LOGFILE_FUNC==ENABLE)
static CHAR gLogFile_Buff[LOGFILE_BUFFER_SIZE]= {0};
#endif

/* Important !! uitron addr, size and resv size should be 2MB alignment
*/
STATIC_ASSERT((_BOARD_UITRON_ADDR_ & 0x1FFFFF) ==0);
STATIC_ASSERT((_BOARD_UITRON_SIZE_ & 0x1FFFFF) ==0);
STATIC_ASSERT((_BOARD_UITRON_RESV_SIZE_ & 0x1FFFFF) ==0);


void OS_Main(void)
{
	MODELEXT_HEADER *header;
	DRAM_PARTITION *p_dram_partition = (DRAM_PARTITION *)Dx_GetModelExtCfg(MODELEXT_TYPE_DRAM_PARTITION, &header);
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);

	if (!p_dram_partition || !header || !pBinInfo) {
		DBG_FATAL("MODELEXT_TYPE_DRAM_PARTITION cannot be null\n");
	}

	TM_BOOT_INIT(128);

	LdLoadTime = pBinInfo->ld.LdLoadTime;

	TM_BOOT_BEGIN("Ldr", 0);
	TM_BOOT_END("Ldr", (char *)(&LdLoadTime)); //save "loader loadtime" => TM nand_load
#if(_MAIN_BREAK == ENABLE)
	_main_break(); //enable this line for trace boot flow
#endif
	TM_BOOT_BEGIN("os", "init");
	//#NT#2016/02/18#Lincy Lin -begin
	//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
	{
		LOGFILE_CFG   cfg = {0};


		#if 1
		// only store system error log
		cfg.ConType = LOGFILE_CON_UART|LOGFILE_CON_MEM;
		#else
		// store normal log and system error log
		cfg.ConType = LOGFILE_CON_UART|LOGFILE_CON_STORE;
		#endif
		cfg.TimeType = LOGFILE_TIME_TYPE_COUNTER;
		cfg.LogBuffAddr = (UINT32)gLogFile_Buff;
		cfg.LogBuffSize = sizeof(gLogFile_Buff);
		LogFile_Config(&cfg);
	}
#else
	DxSys_Entry();
#endif
	//#NT#2016/02/18#Lincy Lin -end

	DBG_DUMP("^W\r\nF/W %s-%s Start ...\r\n", _BSP_NAME_, _CHIP_NAME_);

	//#NT#2016/09/12#Lincy Lin -begin
	//#NT#Porting Dual OS flow, the interrupt setting will be configured in loader
#if defined(_CPU2_LINUX_)
	DBG_DUMP("CPU2 Linux\r\n");
#ifdef  __S3_POWER_DOWN
	if (pBinInfo->ld.LdCtrl2 & LDCF_S3_BOOT) {
		// get saved apb buf info
		MODELEXT_HEADER *header = NULL;
		MEM_RANGE Mem;
		BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
		if (!pBinInfo || !header) {
			DBG_FATAL("bininfo is null\n");
			return;
		}
		Mem.Addr = pBinInfo->comm.Resv[COMM_RESV_IDX_S3_APB_ADDR];
		Mem.Size = pBinInfo->comm.Resv[COMM_RESV_IDX_S3_APB_LEN];
		DBG_DUMP("Restore address 0x%08x, size = 0x%08x\r\n", Mem.Addr, Mem.Size);
		clk_reload_apb_register(Mem.Addr, Mem.Size);

	} else {
#endif
		drv_set_init_int(FALSE);
#ifdef  __S3_POWER_DOWN
	}
#endif
#else
	DBG_DUMP("CPU2 None\r\n");
#endif
	//#NT#2016/09/12#Lincy Lin -end
	DBG_IND("(OS_Main begin)\r\n");

	DBG_IND("(reg exp handler)\r\n");

	DxSys_RegExpHandler(&user_exp_handler);

#if !defined(_MODELEXT_BUILT_IN_ON_)
	{
		HEADINFO   *pHeadInfo;
		pHeadInfo = (HEADINFO *)(_BOARD_UITRON_ADDR_ + BIN_INFO_OFFSET_UITRON);

		if (pHeadInfo->ModelextAddr == 0) {
			DBG_FATAL("MODELEXT should built in\r\n");
		}
	}
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	DBG_IND("(kernel init)\r\n");
	DxSys_Validate(p_dram_partition->dram_size); //(check DRAM size)
	//Config OS (Config Memory Layout)
#if defined(_BSP_NA51000_)
	{
		OS_INIT_S init_parm = {0};

		init_parm.ddr1_addr = dma_getDramBaseAddr(DMA_ID_1);
		init_parm.ddr1_size = dma_getDramCapacity(DMA_ID_1);
		init_parm.uitron_addr = p_dram_partition->uitron_addr;
		init_parm.uitron_size = p_dram_partition->uitron_size - _BOARD_UITRON_RESV_SIZE_;
		init_parm.ddr2_addr = dma_getDramBaseAddr(DMA_ID_2);
		init_parm.ddr2_size = dma_getDramCapacity(DMA_ID_2);
		OS_Init(&init_parm);
	}
#endif
#if defined(_BSP_NA51023_)
	OS_Init(p_dram_partition->dram_addr, p_dram_partition->dram_size, p_dram_partition->uitron_addr, p_dram_partition->uitron_size);
#endif
	OS_BeginCode();
	{
		MEM_RANGE *pCodeSection = (MEM_RANGE *)(p_dram_partition->uitron_addr + CODE_SECTION_OFFSET);
		MEM_RANGE *pZISection = (MEM_RANGE *)(p_dram_partition->uitron_addr + ZI_SECTION_OFFSET);
		DBG_IND("(Install code section)\r\n");
		//Config code sections for partial-load
		OS_CONFIG_CODESECTION(pCodeSection);
		DBG_IND("(Install code zi)\r\n");
		//Config ZI section
		OS_CONFIG_ZISECTION(pZISection);
	}
	OS_EndCode();
	OS_BeginStack();
	{
		//Install driver OS resource (Config Task,Flag,Semaphore)
		//  Note: stack is config by Task
		DBG_IND("(Install Drv ID)\r\n");
		Install_Drv();
		//Install driver ext OS resource (Config Task,Flag,Semaphore)
		//  Note: stack is config by Task
		DBG_IND("(Install DrvExt ID)\r\n");
		Install_DrvExt();
		//Install OS resource (Config Task,Flag,Semaphore) for user
		//  Note: stack is config by Task
		DBG_IND("(Install User ID)\r\n");
		Install_User();
	}
	OS_EndStack();
	OS_BeginHeap();
	{
		//Install OS resource (Config MemPool) for user
		//  Note: heap is config by MemPool
		DBG_IND("(Install User ID for mempool)\r\n");
		Install_UserMEM();
	}
	OS_EndHeap();
	_fill_draminfo();
	_clear_stack();
	if (__DBGLVL__ > 2) {
		OS_DumpMemRange();
		OS_DumpKernelResCnt();
	}
	/////////////////////////////////////////////////////////////////////////////////////
	DBG_IND("(OS_Main end)\r\n");
	TM_BOOT_END("os", "init");
}

//===========================================================================
#define DRAMINFO_TOTAL_START    (pBinInfo->dram.DramInfo[0])
#define DRAMINFO_TOTAL_SIZE     (pBinInfo->dram.DramInfo[1])
#define DRAMINFO_CODE_START     (pBinInfo->dram.DramInfo[2])
#define DRAMINFO_CODE_SIZE      (pBinInfo->dram.DramInfo[3])
#define DRAMINFO_STACK_START    (pBinInfo->dram.DramInfo[4])
#define DRAMINFO_STACK_SIZE     (pBinInfo->dram.DramInfo[5])
#define DRAMINFO_HEAP_START     (pBinInfo->dram.DramInfo[6])
#define DRAMINFO_HEAP_SIZE      (pBinInfo->dram.DramInfo[7])
#define DRAMINFO_RESV_START     (pBinInfo->dram.DramInfo[8])
#define DRAMINFO_RESV_SIZE      (pBinInfo->dram.DramInfo[9])

void _fill_draminfo(void)
{
	//Fill memory range to BinInfo
	MODELEXT_HEADER *header;
	BININFO *pBinInfo;

	pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	if (!pBinInfo || !header) {
		DBG_FATAL("Modelext: bininfo is null\n");
		return;
	}

	//Total start
	DRAMINFO_TOTAL_START = OS_GetMemAddr(MEM_TOTAL);
	//Total end
	DRAMINFO_TOTAL_SIZE = OS_GetMemSize(MEM_TOTAL);
	//Code start
	DRAMINFO_CODE_START = OS_GetMemAddr(MEM_CODE);
	//Code size
	DRAMINFO_CODE_SIZE = OS_GetMemSize(MEM_CODE);
	//Stack start
	DRAMINFO_STACK_START = OS_GetMemAddr(MEM_STACK);
	//Stack size
	DRAMINFO_STACK_SIZE = OS_GetMemSize(MEM_STACK);
	//Heap start
	DRAMINFO_HEAP_START = OS_GetMemAddr(MEM_HEAP);
	//Heap size
	DRAMINFO_HEAP_SIZE = OS_GetMemSize(MEM_HEAP);
	//Resv start
	DRAMINFO_RESV_START = OS_GetMemAddr(MEM_RESV);
	//Resv size
	DRAMINFO_RESV_SIZE = OS_GetMemSize(MEM_RESV);
}

void _clear_stack(void)
{
	memset((void *)OS_GetMemAddr(MEM_STACK), 0x00, OS_GetMemSize(MEM_STACK));
}

#if(_MAIN_BREAK == ENABLE)
void _main_break(void)
{
	static int a = 0;
	while (!a) {
		a = a + a;
	}
	debug_msg("^N(go)\r\n");
}
#endif

//===========================================================================
//Config assert handler

#ifdef __ECOS
#include <cyg/infra/cyg_type.h>        // base types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
void __assert_func(const char *fileName, int line, const char *funcName, const char *condition) __attribute__((__noreturn__));
externC void
cyg_assert_fail(const char *psz_func, const char *psz_file,
				cyg_uint32 linenum, const char *psz_msg) __THROW {
	__assert_func(psz_file, linenum, psz_func, psz_msg); //call to LibC assert func
}
#endif
void __assert_func(const char *fileName, int line, const char *funcName, const char *condition)
{
	debug_msg("^RAssert:'%s' is failed! (%s:%u)\r\n", condition, fileName, line);
#ifdef __ECOS
	debug_msg("(halt)\r\n");
	// coverity[no_escape]
	while (1);
#else
	abort();
#endif
}

//===========================================================================
//Config float math exception handler

//for math lib linking
int __errno = 0;
#ifndef __ECOS
//for math lib linking
int matherr(struct exception *e)
{
	debug_msg("^R*** Math error: type=%d name=\"%s\" arg1=%f arg2=%f retval=%f err=%d\r\n",
			  e->type, e->name, e->arg1, e->arg2, e->retval, e->err);
	return 0;
}
#endif

#if (LOGFILE_FUNC==ENABLE)
static void watchdog_sw_reset(void)
{
	wdt_close();
	DBG_DUMP("+(watch-dog)\r\n");
	// Open
	wdt_open();
	// Config WDT to [uiTimeOut] ms, reset system
	wdt_setConfig(WDT_CONFIG_ID_MODE, WDT_MODE_RESET);
	wdt_setConfig(WDT_CONFIG_ID_TIMEOUT, 0);
	// Enable WDT
	wdt_enable();
	while (1)
		;
}
void LogFile_DumpMemAndSwReset(void)
{
	LogFile_DumpToMem(OS_GetMempoolAddr(POOL_ID_LOGFILE),POOL_SIZE_LOGFILE);
	watchdog_sw_reset();
}
#endif

//===========================================================================
//Config cpu exception handler

void user_exceptionCB(UINT32 *tbl)
{
	debug_msg("user_exceptionCB\r\n");
#if (LOGFILE_FUNC==ENABLE)
	LogFile_DumpMemAndSwReset();
#else
	exit(EXIT_FAILURE);
#endif

}

//===========================================================================
//Config stack-overflow exception handler

#ifdef __ECOS
// eCos kernel just generate assert fail for these kinds of stack corrupt:
// 1."Stack base corrupt"
// 2."Stack top corrupt"
// 3."Gap between stack limit and base corrupt"
// EX:
// Thread "t9":: Stack base corrupt - i: 0
// 80CA7EE0: 54 37 D0 80 20 C0 09 80  EA 5E C4 82 EB 5F C5 83  |T7.. ....^..._..|
// 80CA7EF0: EC 58 C2 84 ED 59 C3 85  4C 37 D0 80 00 00 95 80  |.X...Y..L7......|
// ASSERT FAIL: <C2>thread.inl[120]void Cyg_HardwareThread::check_stack() Stack base corrupt
// ASSERT FAIL: <C2>[120]void Cyg_HardwareThread::check_stack() Stack base corrupt
#endif
void user_stackoverflowCB(UINT32 tcb)
{
	debug_msg("user_stackoverflowCB\r\n");
#if (LOGFILE_FUNC==ENABLE)
	LogFile_DumpMemAndSwReset();
#else
	exit(EXIT_FAILURE);
#endif
}

//===========================================================================




//===========================================================================
#include "FwSrvApi.h"

extern UINT32 UserSection_Load[10];
extern UINT32 UserSection_Order_full[];
extern void UserSection_LoadCb(const UINT32 Idx);

//Config code sections for partial-load
//To assign content of each section,
//please edit the code info file: \Project\DemoKit\Kernel\CodeInfo.s
//please edit the linker script file: \Project\DemoKit\[Model].lds

//This array is sort by section id
UINT32 UserSection_Load[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//This array is sort by loading order
//#NT#2018/04/03#Niven Cho -begin
//#NT#PARTIAL_COMPRESS
UINT32 UserSection_Order_full[] = {
	CODE_SECTION_02, CODE_SECTION_03, CODE_SECTION_04, CODE_SECTION_05, CODE_SECTION_06,
	CODE_SECTION_07, CODE_SECTION_08, CODE_SECTION_09, CODE_SECTION_10, FWSRV_PL_BURST_END_TAG
};
//#NT#2018/04/03#Niven Cho -end

int order = 1;
void UserSection_LoadCb(const UINT32 Idx)
{
	DBG_MSG("^G   Section-%.2ld: Range[0x%08X~0x%08X] Size=0x%08X (LOAD) - %d\r\n",
			Idx + 1, OS_GetMemAddr(Idx), OS_GetMemAddr(Idx) + OS_GetMemSize(Idx), OS_GetMemSize(Idx), Perf_GetCurrent());

	UserSection_Load[Idx] = 1; //mark loaded
	order++;
#if (POWERON_FAST_BOOT == ENABLE)
	if (Idx == 0) { //Section-01
		INIT_SETFLAG(FLGINIT_LOADCODE1);
	}
	if (Idx == 1) { //Section-02
		INIT_SETFLAG(FLGINIT_LOADCODE2);
	}
	if (Idx == 9) { //Section-10
		INIT_SETFLAG(FLGINIT_LOADCODE3);
	}
#endif
}

