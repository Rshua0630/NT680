#include <common.h>
#include <malloc.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
//#include <linux/mtd/compat.h>

#include <asm/errno.h>
#include <nand.h>

#include "nvt_plat.h"
#include "nand_flash.h"

//#include "cache.h"

struct NTK_NF_prv {
	struct mtd_info mtd;
	struct nand_chip chip;
	unsigned int nand_data_buf_idx;
	unsigned int nand_write_page;
	unsigned int nand_previous_page;
	char nand_first_read;
	unsigned char nand_uncorrectable;
	//unsigned char nand_data_buf[4096]  __attribute__((aligned(64)));
	unsigned char nand_data_buf[8192]  __attribute__((aligned(64)));
	//unsigned char *nand_data_buf;
	int              spareonly;	// 0 : main data 1 : spare data
	//unsigned char nand_ecc_buf[224] __attribute__((aligned(64)));
	unsigned char nand_ecc_buf[436] __attribute__((aligned(64)));
	//unsigned char *nand_ecc_buf;
	int		nand_ecc_mode;	// 0 : disable 1 : enable
};

struct NTK_NF_prv NFDevice;
//unsigned char OOB_Temp[224] __attribute__((aligned(64)));
unsigned char OOB_Temp[436] __attribute__((aligned(64)));

unsigned int NAND_Page_Size;
unsigned int NAND_Oob_Size;
unsigned int NAND_Block_Size;
unsigned int NAND_PPB;	//Page_Per_Block
unsigned int NAND_SubPage_Size; 
unsigned int NAND_SubOob_Size;
unsigned int NAND_SubOob_SW = 12;


/* These really don't belong here, as they are specific to the NAND Model */
static uint8_t scan_ff_pattern[] = { 0xff, 0xff };

static struct nand_bbt_descr NTK_NF_bbt_descr = {
	.options = NAND_BBT_SCAN2NDPAGE,
	.offs = 0,
	.len = 1,
	.pattern = scan_ff_pattern
};


#if 0	//for BCH, temp solution for new nand
static struct nand_ecclayout NTK_NF_ecclayout = {
	.eccbytes = 42,
	.eccpos = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
		         22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
		         32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 
		         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 
		         52, 53,
		         },
	.oobfree = {
		{.offset = 2,
		 .length = 10},
		{.offset = 54,
		 .length = 10}
		 }
};

#else	//for BCH

static struct nand_ecclayout NTK_NF_ecclayout = {
	.eccbytes = 168,
	.eccpos = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
			22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
		         32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 
		         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 
		         52, 53, 54, 55, 
		         68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 
		         78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 
		         88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 
		         98, 99, 100, 101, 102, 103 , 104, 105, 106, 107, 
		         108, 109, 110, 111, 
		         122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 
		         132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 
		         142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 
		         152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 
		         162, 163, 164, 165, 
		         176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 
		         186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 
		         196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 
		         206, 207, 208, 209, 210, 211
		         },
	.oobfree = {
		{.offset = 2,
		 .length = 10},
		{.offset = 56,
		 .length = 12},
		{.offset = 112,
		 .length = 12},
		{.offset = 166,
		 .length = 12}
		 }
};
#endif

#define DRV_NAME		"NTK_NF"
#define NTKNAND_PRINTK	//printk

#define FPGA	1

#if FPGA
#define CPU_SPEED_MHZ	                (384) /* 500MHz for real ROM code */
#else
#define CPU_SPEED_MHZ	                (CONFIG_CPU_MHZ)
#endif

//********************************************************************************
//
// Private Function
//
//********************************************************************************

static u32 ns2clk(u32 hclk_mhz, u32 ns)
{
	u32 clk;
	
	clk = ((hclk_mhz * ns + 1000 - 1) / 1000);
	
	return clk;
}

static void delay_us(u32 count)
{
#if 0
	volatile unsigned long *freq = (volatile unsigned long *)(0xbfb00140);
	unsigned long cpu_freq;
	u32 i;
	
	if (freq[0])
		cpu_freq = (freq[0] / 1000000) + 1;
	else
		cpu_freq = CONFIG_CPU_MHZ;

	for (i = 0; i < (count * (cpu_freq>>6)); i++) // make sure CPU_SPEED_MHZ > 64
	{
	    // 64 "nop"s for CPU
	    __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
	    __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
	    __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
	    __asm__ __volatile__ ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");
	}
#endif
	udelay(count);
}

//********************************************************************************
// Function Name	: NFCHIP_Init
// Author			    : Ken Yu
// Description		: init nand flash controller
//********************************************************************************
static int NFCHIP_Init( void )
{
	u8 tWH, tWP, tRH, tRP, tWW, tAR, tCLS_tCS_tALS_tALH;
	u32 hclk , bootval;
	u32 temp;
	
	ntk_writel(0x1F, REG_NFC_SW_RESET);
	while(ntk_readl(REG_NFC_SW_RESET) & 0x01);

	//Ken, force to default, FPGA bootup is 0xff000020 
#if FPGA
	//ntk_writel(0x03000000, REG_NFC_SYSCTRL);
#endif
	
	// clear interrupt status
	ntk_writel(0x00, REG_NFC_INT_ENABLE);
	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);

	// Get Bootstrap value to determine what type of NAND Flash using in this system
	bootval = ntk_readl(REG_STBC_BOOTSTRAP);
	switch(((bootval >> 10) & 0x03)){
		case 0:	// 2k
			NAND_Page_Size = 0x800;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_PAGE_2048;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_2048;
			break;
		case 1:	// 8k
			NAND_Page_Size = 0x2000;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_PAGE_8192;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_8192;
			break;
		case 2:	// 4k
			NAND_Page_Size = 0x1000;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_PAGE_4096;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_4096;
			break;
	}

	switch(((bootval >> 12) & 0x03)){
		case 0:	// 128k
			NAND_Block_Size = 0x20000;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_128K;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_128K;
			break;
		case 1:	// 512k
			NAND_Block_Size = 0x80000;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_512K;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_512K;
			break;
		case 2:	// 256k 
			NAND_Block_Size = 0x40000;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_256K;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_256K;
			break;
		case 3:	// 1024k
			NAND_Block_Size = 0x100000;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_1M;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_1M;
			break;
	}

	switch(((bootval >> 14) & 0x01)){
		case 0:	// 512+16
			NAND_SubPage_Size = 512;
			NAND_SubOob_Size = 16;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_EXTRA_SIZE(16) | 
				NFC_SYS_CTRL_ECC_RS | NFC_SYS_CTRL_SUBPAGE_512;
			ntk_writel(temp, REG_NFC_SYSCTRL);
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_EXTRA_SIZE(16) | NFC_SYS_CTRL_ECC_RS | NFC_SYS_CTRL_SUBPAGE_512;
			break;
		case 1:	// 1024+54
			NAND_SubPage_Size = 1024;
			NAND_SubOob_Size = 54;
			temp = ntk_readl(REG_NFC_SYSCTRL) | NFC_SYS_CTRL_EXTRA_SIZE(54) | 
				NFC_SYS_CTRL_ECC_BCH | NFC_SYS_CTRL_SUBPAGE_1024;
			ntk_writel(temp, REG_NFC_SYSCTRL);			
			//REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_EXTRA_SIZE(54) | NFC_SYS_CTRL_ECC_BCH | NFC_SYS_CTRL_SUBPAGE_1024;
			break;
	}

#if FPGA
	hclk = 12;
#else
	// check HCLK selection & calculate NAND timing setting; 
	hclk_sel = (ntk_readl(REG_SYS_PROGRAM_OPTION) & 0x07000000)>>24;
	if (hclk_sel == 0){ // divider = 6 
		hclk = CPU_SPEED_MHZ / 6;
	}else{ // hclk_sel == 1, divider = 4
		hclk = CPU_SPEED_MHZ / 4;
	}
#endif
	
	//hclk = 4;
	// modify timing setting here: (in ns)
	tWH = ns2clk(hclk, 30);
	tWH -= tWH ? 1 : 0;
	
	tWP = ns2clk(hclk, 34);
	tWP -= tWP ? 1 : 0;
	
	tRH = ns2clk(hclk, 30);
	tRH -= tRH ? 1 : 0;

	tRP = ns2clk(hclk, 34);
	tRP -= tRP ? 1 : 0;

	tWW = ns2clk(hclk, 200); // t3: (n+1)*4clks+3
	tWW -= (tWW >= 3) ? 3 : 0;
	tWW = tWW/4;
	tWW -= tWW ? 1 : 0;

	tAR = (ns2clk(hclk, 20)/4);
	tAR -= tAR ? 1 : 0;

	// notice that tCLS tCS definiiton is different between samsung and micron, our ip use samsung's define
	tCLS_tCS_tALS_tALH = ns2clk(hclk, (48 - 34)); // micron: tCS - tWP
	tCLS_tCS_tALS_tALH -= tCLS_tCS_tALS_tALH ? 1 : 0;

	tCLS_tCS_tALS_tALH = 2;

	ntk_writel(0x00, REG_NFC_CFG0);
	// fill tWH, tWL, tRH, tRL, tWW, tAR, tCLS_tCS_tALS_tALH, and fix to 8 bit bus width
	temp = NFC_CFG0_TWH(tWH) | NFC_CFG0_TWL(tWP) | NFC_CFG0_TRH(tRH) | NFC_CFG0_TRL(tRP) |
		NFC_CFG0_TWW(tWW) | NFC_CFG0_TAR(tAR) | NFC_CFG0_TCLS_TCS_TALS_TALH(tCLS_tCS_tALS_tALH) |
		NFC_CFG0_ROW_ADDR_3CYCLES | NFC_CFG0_COL_ADDR_2CYCLES;
	ntk_writel(temp, REG_NFC_CFG0);	

	if(NAND_SubPage_Size == 1024){
		ntk_writel(0x00, REG_NFC_CFG1);
		temp = NFC_CFG1_READY_TO_BUSY_TIMEOUT(-1) | NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_LITTLE_ENDIAN | 
		           NFC_CFG1_BCH_ECC_ENABLE | NFC_CFG1_BUSY_TO_READY_TIMEOUT(-1);
		ntk_writel(temp, REG_NFC_CFG1);
	}else{
		temp = NFC_CFG1_READY_TO_BUSY_TIMEOUT(-1) | NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_LITTLE_ENDIAN | 
		           NFC_CFG1_RS_ECC_ENABLE | NFC_CFG1_BUSY_TO_READY_TIMEOUT(-1);
		ntk_writel(temp, REG_NFC_CFG1);
	}

	return E_OK;
}


//********************************************************************************
//
// Function Name	: NFCHIP_Reset
//
// Author			    : Mars Chen
//
// Description		: reset nand flash
//
//********************************************************************************
static int NFCHIP_Reset(struct mtd_info *mtd , u8 bChipNumber)
{   // 0xff, busy
	u8   dwCE = bChipNumber;
	u32   ret;
	struct nand_chip *nand = mtd->priv;
	struct NTK_NF_prv *prv = nand->priv;
	u32 temp;
	
	prv->nand_first_read = 1;
	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
	
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_END_WAIT_BUSY_TO_RDY | 
		NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0xff);
	ntk_writel(temp, REG_NFC_CMD);
	while( !(ntk_readl(REG_NFC_INT_STAT)&(NFC_INT_ERR | NFC_INT_CMD_COMPLETE)) );  // wait for command complete

	ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR);
	if (ret)
	{
		return E_OK;
	}
	else
	{
		return E_SYS;
	}
}

//********************************************************************************
//
// Function Name	: NFCHIP_ReadStatus
//
// Author			    : Mars Chen
//
// Description		: read NAND Flash Status
//
//********************************************************************************
static u8 NFCHIP_ReadStatus(u8 bChipNumber)
{   // 0x70, data(1)
	u8   dwCE = bChipNumber;
	//u32   ret , cmd;
	u32 ret;
	u32 temp;
	
	temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_BLK_SIZE(1); // transfer 1 byte (status)
	ntk_writel(temp, REG_NFC_TRAN_MODE);
	
	temp = NFC_INT_CMD_COMPLETE | NFC_INT_DATA_TRAN_COMPLETE;   // clear command complete & transfer complete status
	ntk_writel(temp, REG_NFC_INT_STAT);
	
	//cmd = ntk_readl(REG_NFC_TRAN_MODE);
	
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_CYCLE_ONE | 
		NFC_CMD_CODE0(0x70);
	ntk_writel(temp, REG_NFC_CMD);
		
	while( !(ntk_readl(REG_NFC_INT_STAT)&(NFC_INT_ERR | NFC_INT_CMD_COMPLETE)) );  // wait for command complete
	while( !(ntk_readl(REG_NFC_INT_STAT)&(NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE)) );  // wait for transfer complete

	if (ntk_readl(REG_NFC_CFG1) & NFC_CFG1_LITTLE_ENDIAN){
	// little endian
		ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR) ? (ntk_readl(REG_NFC_DATA_PORT) & 0xff) : 0;
	}else{ 
	// big endian
		ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR) ? ntk_readl(REG_NFC_DATA_PORT) >> 24 : 0;
	}
	
	//ret |= 0x80;
	return ret;
}

//********************************************************************************
//
// Function Name	: NFCHIP_ReadPageOOB
//
// Author			    : Mars Chen
//
// Description		: read OOB
//
//********************************************************************************
static int NFCHIP_ReadPageOOB(u8 bChipNumber, u32 dwBuffer, u32 dwPageAddr , u8 SamePage)
{   // 0x00, col, row, 0x30, busy, data(16)
 	//u32   SubPagesPerPage = NAND_Page_Size / 512; // subpage size 512 is NAND IP limitation
	u8    dwCE = bChipNumber;
	//u32   ret , cmd;
	u32 ret;
	u32 temp;

	//flush_dcache_range(dwBuffer, 64);
	//dma_cache_sync(NULL, dwBuffer, 64, DMA_FROM_DEVICE);
	
	ntk_writel(dwBuffer & 0x1fffffff, REG_NFC_DMA_ADDR);
	
	temp = NFC_DMA_CTRL_READ | NFC_DMA_CTRL_TRAN_BYTE_COUNT(64);
	ntk_writel(temp, REG_NFC_DMA_CTRL);
	
	ntk_writel(dwPageAddr, REG_NFC_ROW_ADDR);
	ntk_writel(NAND_Page_Size, REG_NFC_COL_ADDR);
	
	if(SamePage){
		temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_BLK_SIZE(64) | 
			NFC_TRAN_MODE_DATA_SEL_DMA;
		ntk_writel(temp, REG_NFC_TRAN_MODE);
		ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
		temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL | 
			NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0xe0) | NFC_CMD_CODE0(0x05);  
		ntk_writel(temp, REG_NFC_CMD);
	}else{
		temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_BLK_SIZE(64) | 
			NFC_TRAN_MODE_DATA_SEL_DMA | NFC_TRAN_MODE_START_WAIT_RDY;
		ntk_writel(temp, REG_NFC_TRAN_MODE);
		ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
		temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL_ROW | 
			NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | 
			NFC_CMD_CODE0(0x00);  
		ntk_writel(temp, REG_NFC_CMD);
	}
	
	while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));  // wait for command complete
	while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE)));    // wait for transfer complete
	while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_MEM_TRAN_COMPLETE))); // wait for system memory transfer complete
	
	invalidate_dcache_range(dwBuffer, dwBuffer+64);
	ret = !(REG_NFC_INT_STAT & NFC_INT_ERR);
	if (ret)
	{
		return 0;
	}
	else
	{
		//printk(KERN_ERR DRV_NAME ": Fail(REG_NFC_INT_STAT %X)\n" , REG_NFC_INT_STAT);
		return -1;
	}
}


void write_oob_to_reg(u8 *w_oobbuf)
{
	int i;
	unsigned int reg_oob;
	
	for (i=0; i<3; i++){
		reg_oob = w_oobbuf[0+4*i] | (w_oobbuf[1+4*i] << 8) | (w_oobbuf[2+4*i] << 16) | (w_oobbuf[3+4*i] << 24);
		ntk_writel( reg_oob, REG_NFC_XTRA_DATA0+i*4);
	}
}


//********************************************************************************
// Function Name	: NFCHIP_WritePage
// Author			    : Mars Chen
// Description		: read NAND Flash Status
//********************************************************************************
static int NFCHIP_WritePage(struct mtd_info *mtd, u8 bChipNumber, u32 dwBuffer, u32 dwExtraBuffer, 
	u32 dwPageAddr)
{   // 0x80, col, row, data(512), 0x85, col, data(16),  (SubPage 0)
    // 0x85, col,      data(512), 0x85, col, data(16),  (SubPage 1)
    // 0x85, col,      data(512), 0x85, col, data(16),  (SubPage 2)
    //          ...                                     (SubPage 3 ... n)
    // 0x10, busy, check status
	
	u32   SubPagesPerPage = NAND_Page_Size/NAND_SubPage_Size;
	u32   i;
	u8    dwCE = bChipNumber;
	//u32   value , cmd;
	u32 temp;
	u32 ret;
	//Write is not Free-Run mode, so have to -1
	u32 NAND_SubPage_Size_W = NAND_SubPage_Size - 1;	

	// Reset Buffer - Need to check with Designer
	ntk_writel(0x1F, REG_NFC_SW_RESET);
	while(ntk_readl(REG_NFC_SW_RESET) != 0);
	
#if 1
	flush_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	flush_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	wb_dcache_range((u32)dwBuffer, NAND_Page_Size);
	wb_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif

	for(i = 0; i < SubPagesPerPage; ++i){
		ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);

		if(NAND_SubPage_Size == 1024){
			ntk_writel( ntk_readl(REG_NFC_CFG1)|(0x01<<23), REG_NFC_CFG1);
			ntk_writel( ntk_readl(REG_NFC_CFG1)&0xFF7FFFFF, REG_NFC_CFG1);
		}
		
		ntk_writel((dwBuffer + NAND_SubPage_Size * i ) & 0x1fffffff, REG_NFC_DMA_ADDR);

		//write oob area to registers
		write_oob_to_reg((u8 *)dwExtraBuffer + i*NAND_SubOob_SW);

		temp = NFC_DMA_CTRL_TRAN_BYTE_COUNT(NAND_SubPage_Size_W);
        		ntk_writel(temp, REG_NFC_DMA_CTRL);
		
		ntk_writel( NAND_SubPage_Size * i, REG_NFC_COL_ADDR);
		
		temp = NFC_RAND_ACC_CMD_CODE0(0x85) | 
            		NFC_RAND_ACC_CMD_COL_ADDR(NAND_Page_Size + NAND_SubOob_Size * i); 
		ntk_writel(temp, REG_NFC_RAND_ACC_CMD);
		
		temp = NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE |
			NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | 
			NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET | NFC_TRAN_MODE_DATA_SEL_DMA | 
			NFC_TRAN_MODE_WRITE;
		ntk_writel(temp, REG_NFC_TRAN_MODE);

		if(i == 0){
			// SubPage 0:           0x80, col, row, data(512), 0x85, col, data(16)
			ntk_writel(dwPageAddr, REG_NFC_ROW_ADDR);
			
			temp = NFC_TRAN_MODE_KEEP_CE | NFC_CMD_WP_NEG|NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | 
				NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x80);
			ntk_writel(temp, REG_NFC_CMD);
        		}else{
			// SubPage 1, 2, ... n: 0x85, col,      data(512), 0x85, col, data(16)
	            	temp = NFC_TRAN_MODE_KEEP_CE | NFC_CMD_WP_NEG | NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | 
	            		NFC_CMD_ADDR_CYCLE_COL | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x85);
	                	ntk_writel(temp, REG_NFC_CMD);
		}

		while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_DATA_TRAN_COMPLETE));    // wait for transfer complete

		if(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW){
        			printk("[%s] SubPages = %d INT ERR: %#lx\n", __FUNCTION__, i, 
        				ntk_readl(REG_NFC_INT_STAT));  
            		return -1;
        		}
	}

	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
			
	//2nd write command
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_ONE | 
		NFC_CMD_CODE0(0x10);
    	ntk_writel(temp, REG_NFC_CMD);

	while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_CMD_COMPLETE));  // wait for command complete

#if 1
	invalidate_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	invalidate_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	inv_dcache_range((u32)dwBuffer, NAND_Page_Size);
	inv_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif

	ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW);
	if (ret){
		u8 bStatus = NFCHIP_ReadStatus(bChipNumber);
		if(bStatus & 0x40){	//Status bit I/O<6>
			// ready
			if(bStatus & 0x01){   // fail, Status bit I/O<0>
				printk("[%s] IO6 ok, Write page=%d FAILS\n" , __FUNCTION__, dwPageAddr);
				return -1;
			}
		}
		return 0;
	}else{
		printk("[%s] Write page=%d FAILS\n" , __FUNCTION__, dwPageAddr);
		return -1;
	}
}


//********************************************************************************
//
// Function Name	: NFCHIP_ReadPage
//
// Author			    : Mars Chen
//
// Description		: read NAND Flash Status
//
//********************************************************************************
static int NFCHIP_ReadPage(struct mtd_info *mtd, u8 bChipNumber, u32 dwBuffer, u32 dwExtraBuffer, 
	u32 dwPageAddr , u8 SamePage)
{
	int   bResult = 0;
	u32   SubPagesPerPage = NAND_Page_Size / NAND_SubPage_Size;
	u8    dwCE = bChipNumber;    
	int i , j;  
	u32 dwErrCount , BytePos , BitPos;
	u32 dwErrorAddr , ret;
	u32 temp;
	
	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
	
	for(i = 0; i < SubPagesPerPage; ++i){
		ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
		if(NAND_SubPage_Size == 1024){
			ntk_writel(ntk_readl(REG_NFC_CFG1)|(0x01<<22), REG_NFC_CFG1);
			ntk_writel(ntk_readl(REG_NFC_CFG1)&0xFFBFFFFF, REG_NFC_CFG1);
		}
		
		ntk_writel((NAND_SubPage_Size * i + dwBuffer) & 0x1fffffff, REG_NFC_DMA_ADDR);
		
		temp = NFC_DMA_CTRL_READ | NFC_DMA_CTRL_TRAN_BYTE_COUNT(NAND_SubPage_Size);
		ntk_writel(temp, REG_NFC_DMA_CTRL);
		
		ntk_writel(NAND_SubPage_Size * i, REG_NFC_COL_ADDR);
		
		temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO | 
			NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_ECC_CHK | 
			NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | NFC_TRAN_MODE_ECC_ENABLE | 
			NFC_TRAN_MODE_ECC_RESET | NFC_TRAN_MODE_DATA_SEL_DMA;    
		ntk_writel(temp, REG_NFC_TRAN_MODE);
		
		temp = NFC_RAND_ACC_CMD_CODE1(0xe0) | NFC_RAND_ACC_CMD_CODE0(0x05) |            
			NFC_RAND_ACC_CMD_COL_ADDR(NAND_SubOob_Size * i + NAND_Page_Size); 
		ntk_writel(temp, REG_NFC_RAND_ACC_CMD);
	
		if(i == 0){   
			// SubPage 0:           0x00, col, row, 0x30, busy, data(512), 0x05, col, 0xe0, data(16)            
			ntk_writel(dwPageAddr, REG_NFC_ROW_ADDR);
  			ntk_writel(ntk_readl(REG_NFC_TRAN_MODE)|NFC_TRAN_MODE_START_WAIT_RDY, REG_NFC_TRAN_MODE);
			temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_END_WAIT_BUSY_TO_RDY | 
				          NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | NFC_CMD_CODE0(0x00);
			ntk_writel(temp, REG_NFC_CMD);
		}else{        
			// SubPage 1, 2, ... n: 0x05, col,      0xe0,       data(512), 0x05, col, 0xe0, data(16)            
			temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL |                
			              NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0xe0) | NFC_CMD_CODE0(0x05);
			ntk_writel(temp, REG_NFC_CMD);
		}        
			
		while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));  // wait for command complete        
		while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE)));    // wait for transfer complete       
		while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_MEM_TRAN_COMPLETE))); // wait for system memory transfer complete        // process RSECC        
		while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_ECC_DEC_COMPLETE)));   // wait for RSECC decoding complete        
		
		if(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR)){
			ret = (ntk_readl(REG_NFC_STAT) & NFC_STAT_RS_ECC_ERR_MASK);
			if(NAND_SubPage_Size == 1024)	{	
				if(ret != NFC_STAT_RS_ECC_ERR_NONE){   
					// process BCH-ECC error
					dwErrCount = 0;
					dwErrCount = ntk_readl(REG_NFC_STAT);
					dwErrCount = dwErrCount>>14;
					dwErrCount = dwErrCount & 0x1F;
				
					for(j = 0 ; j < dwErrCount ; j++){
						if(j < 8){
							dwErrorAddr = REG_NFC_BCH_ERR_SET1(j/2);
							if(j % 2 == 0){
								dwErrorAddr = dwErrorAddr&0xffff;
							}else{
								dwErrorAddr = (dwErrorAddr>>16)&0xffff;
							}
							BytePos = (1077 - (dwErrorAddr>>3));
							BitPos = 7 - ((8623 - dwErrorAddr) % 8);
							((u8 *)(dwBuffer))[i*NAND_SubPage_Size + BytePos] ^= (0x01<<BitPos);
						}else{
							dwErrorAddr = REG_NFC_BCH_ERR_SET2((j-8)/2);
							if(j % 2 == 0){
								dwErrorAddr = dwErrorAddr&0xffff;
							}else{
								dwErrorAddr = (dwErrorAddr>>16)&0xffff;
							}
							BytePos = (1077 - (dwErrorAddr>>3));
							BitPos = 7 - ((8623 - dwErrorAddr) % 8);

							((u8 *)(dwBuffer))[i*NAND_SubPage_Size + BytePos] ^= (0x01<<BitPos);
						}
					}
					
					ret = (ntk_readl(REG_NFC_STAT) & NFC_STAT_RS_ECC_ERR_MASK);
					if(ret == NFC_STAT_RS_ECC_ERR_NONCORRECTABLE){
						printk("[%s] BCH ECC NON-CORRECTABLE Error\n", __FUNCTION__);
						return -1;
					}
				}
			}else	{
				if(ntk_readl(REG_NFC_STAT) != NFC_STAT_RS_ECC_ERR_NONE){   
					// process RSECC error                   
					for(j = 0; j < 4; ++j){                    
						//dwErrorAddr = (REG_NFC_RS_ECC(j) >> 16) & 0x3ff;
						u32 dwErrorAddr = (ntk_readl(REG_NFC_RS_ECC0+4*j) >> 16) & 0x3ff;
						if(dwErrorAddr < 512 + 16){   
							// within range of main/extra data                        
							if(dwErrorAddr < 512){   
								// correct main data                            
								((u8 *)(dwBuffer))[dwErrorAddr+i* NAND_SubPage_Size] ^=  (ntk_readl(REG_NFC_RS_ECC0+4*j) & 0xff);
							}else{   
								// correct extra data                            
								((u8 *)&ntk_readl(REG_NFC_XTRA_DATA0))[dwErrorAddr - 512] ^= (ntk_readl(REG_NFC_RS_ECC0+4*j)& 0xff);
							}                    
						}
					}
					
					if((REG_NFC_STAT & NFC_STAT_RS_ECC_ERR_MASK) == NFC_STAT_RS_ECC_ERR_NONCORRECTABLE){                    
						printk("[%s] RS ECC NON-CORRECTABLE Error\n", __FUNCTION__);
						bResult = -1;                    
					}
				}  
			}
		}else{
			printk("[%s] REG_NFC_INT_STAT Error, Force Break~~\n", __FUNCTION__);
			// io error            
			break;        
		}
	}
	//invalidate_dcache_range(dwBuffer, 2048);
	//invalidate_dcache_range(dwExtraBuffer , dwExtraBuffer+64);	// Mars @ 20100514 Suwon
	if(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR)){
		return 0;
	}else{
		printk("[%s] REG_NFC_INT_STAT Error\n", __FUNCTION__);
		return -1;
	}
}


/********************************************************************************
 Function Name	: NFCHIP_ReadSubPage_FreeRun
 Author			: Ken Yu
 Description	: read NAND Flash use Free-Run that only work
			on BCH 	
********************************************************************************/
static int NFCHIP_ReadSubPage_FreeRun(struct mtd_info *mtd, u8 bChipNumber, u32 dwBuffer, u32 dwExtraBuffer, 
	u32 dwSubPageAddr)
{
	int    bResult = 0;
	u32 SubPagesPerPage = NAND_Page_Size / NAND_SubPage_Size;
	u8   dwCE = bChipNumber;    
	//int     i;
	u32 temp;
	
#if 1
	flush_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	flush_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	wb_dcache_range((u32)dwBuffer, NAND_Page_Size);
	wb_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif
		
	//reset ecc encode	
	if(NAND_SubPage_Size == 1024){
		ntk_writel( ntk_readl(REG_NFC_CFG1)|NFC_CFG1_ECC_DECODE_RESET, REG_NFC_CFG1);
		ntk_writel( ntk_readl(REG_NFC_CFG1)& ~NFC_CFG1_ECC_DECODE_RESET, REG_NFC_CFG1);
	}

	ntk_writel( 0xFFFFFFFF, REG_NFC_INT_STAT);
	ntk_writel( 0x04, REG_NFC_SYSCTRL1);
	
	//enable bch codes pipeline
	if(NAND_SubPage_Size == 1024){
		ntk_writel( ntk_readl(REG_NFC_CFG1)|NFC_CFG1_BCH_ENABLE_PL, REG_NFC_CFG1 );
	}

 	ntk_writel(dwSubPageAddr / SubPagesPerPage, REG_NFC_ROW_ADDR);
 	ntk_writel((dwSubPageAddr % SubPagesPerPage) * NAND_SubPage_Size, REG_NFC_COL_ADDR);

	// NT72558 Need Modify
	// NFC_TRAN_MODE_XTRA_DATA_COUNT_16 : This config will not use when BCH is ready
	temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO | 
		NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET | 
		NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | 
		NFC_TRAN_MODE_START_WAIT_RDY | NFC_TRAN_MODE_DATA_SEL_DMA;  
	ntk_writel( temp, REG_NFC_TRAN_MODE);
	
	// Set random access command
	temp = NFC_RAND_ACC_CMD_CODE1(0xe0) | NFC_RAND_ACC_CMD_CODE0(0x05) |            
		NFC_RAND_ACC_CMD_COL_ADDR((dwSubPageAddr % SubPagesPerPage) * NAND_SubOob_Size + NAND_Page_Size);
	ntk_writel( temp, REG_NFC_RAND_ACC_CMD);
	
	// Set data and oob DMA address
	ntk_writel( (dwBuffer) & 0x1fffffff, REG_NFC_DMA_ADDR);
	ntk_writel( (dwExtraBuffer) & 0x1fffffff, REG_NFC_XTRA_ADDR);

	// Set DMA Control
	ntk_writel( NFC_DMA_CTRL_READ | NAND_SubPage_Size, REG_NFC_DMA_CTRL);
		
	// Set command
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_FREERUN | 
		NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_END_WAIT_BUSY_TO_RDY | 
		NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | NFC_CMD_CODE0(0x00);   
	ntk_writel( temp, REG_NFC_CMD);

	while( !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_MEM_TRAN_COMPLETE) ){
		if ( ntk_readl(REG_NFC_INT_STAT) & NFC_INT_FR_UNCORECTABLE ){
			printk("BCH Un-Correctable Error, INT_STAT=%#lx\n" , ntk_readl(REG_NFC_INT_STAT));
			return -1;
		}
	}; // wait for system memory transfer complete 

#if 1
	invalidate_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	invalidate_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	inv_dcache_range((u32)dwBuffer, NAND_Page_Size);
	inv_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif
		
	bResult = ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW;

	//disable BCH and Free-Run
	if(NAND_SubPage_Size == 1024){
		ntk_writel( ntk_readl(REG_NFC_CFG1)&~NFC_CFG1_BCH_ENABLE_PL , REG_NFC_CFG1);
		ntk_writel( 0x00, REG_NFC_SYSCTRL1);
	}
	
	if (bResult){
		printk("[%s] READ Subpage=%d FAILS\n", __FUNCTION__, dwSubPageAddr);
		return -1;
	}
		
	return 0;
}


/********************************************************************************
 Function Name	: NFCHIP_ReadPage_FreeRun
 Author			: Ken Yu
 Description	: read NAND Flash use Free-Run that only work
			on BCH 	
********************************************************************************/
static int NFCHIP_ReadPage_FreeRun(struct mtd_info *mtd, u8 bChipNumber, u32 dwBuffer, u32 dwExtraBuffer, 
	u32 dwPageAddr)
{
	int    bResult = 0;    
	u8   dwCE = bChipNumber;    
	//int     i;
	u32 temp;
	
#if 1
	flush_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	flush_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	wb_dcache_range((u32)dwBuffer, NAND_Page_Size);
	wb_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif
		
	//reset ecc encode	
	if(NAND_SubPage_Size == 1024){
		ntk_writel( ntk_readl(REG_NFC_CFG1)|NFC_CFG1_ECC_DECODE_RESET, REG_NFC_CFG1);
		ntk_writel( ntk_readl(REG_NFC_CFG1)& ~NFC_CFG1_ECC_DECODE_RESET, REG_NFC_CFG1);
	}

	ntk_writel( 0xFFFFFFFF, REG_NFC_INT_STAT);
	ntk_writel( 0x04, REG_NFC_SYSCTRL1);
	
	//enable bch codes pipeline
	if(NAND_SubPage_Size == 1024){
		ntk_writel( ntk_readl(REG_NFC_CFG1)|NFC_CFG1_BCH_ENABLE_PL, REG_NFC_CFG1 );
	}

	// Set Row Address
	ntk_writel( dwPageAddr, REG_NFC_ROW_ADDR);
	// Set Col Address to subpage0 (0)
	ntk_writel( 0x00, REG_NFC_COL_ADDR);

	temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO | 
		NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET | 
		NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_Page_Size) | 
		NFC_TRAN_MODE_START_WAIT_RDY | NFC_TRAN_MODE_DATA_SEL_DMA;  
	ntk_writel( temp, REG_NFC_TRAN_MODE);
	
	// Set random access command
	temp = NFC_RAND_ACC_CMD_CODE1(0xe0) | NFC_RAND_ACC_CMD_CODE0(0x05) |            
		NFC_RAND_ACC_CMD_COL_ADDR(NAND_Page_Size);
	ntk_writel( temp, REG_NFC_RAND_ACC_CMD);
	
	// Set data and oob DMA address
	ntk_writel( (dwBuffer) & 0x1fffffff, REG_NFC_DMA_ADDR);
	ntk_writel( (dwExtraBuffer) & 0x1fffffff, REG_NFC_XTRA_ADDR);

	// Set DMA Control
	ntk_writel( NFC_DMA_CTRL_READ | NAND_Page_Size, REG_NFC_DMA_CTRL);
		
	// Set command
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_FREERUN | 
		NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_END_WAIT_BUSY_TO_RDY | 
		NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | NFC_CMD_CODE0(0x00);   
	ntk_writel( temp, REG_NFC_CMD);

	while( !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_MEM_TRAN_COMPLETE) ){
		if ( ntk_readl(REG_NFC_INT_STAT) & NFC_INT_FR_UNCORECTABLE ){
			printk("BCH Un-Corectable Error, INT_STAT=%#lx\n" , ntk_readl(REG_NFC_INT_STAT));
			return -1;
		}
	}; // wait for system memory transfer complete 

#if 1
	invalidate_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	invalidate_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	inv_dcache_range((u32)dwBuffer, NAND_Page_Size);
	inv_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif
		
	bResult = ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW;

	//disable BCH and Free-Run
	if(NAND_SubPage_Size == 1024){
		//ntk_writel( ntk_readl(REG_NFC_CFG1)&0xFFFDFFFF, REG_NFC_CFG1);
		ntk_writel( ntk_readl(REG_NFC_CFG1)&~NFC_CFG1_BCH_ENABLE_PL , REG_NFC_CFG1);
		ntk_writel( 0x00, REG_NFC_SYSCTRL1);
	}
	
	if (bResult){
		printk("[%s] READ page=%d FAILS\n", __FUNCTION__, dwPageAddr);
		return -1;
	}
		
	return 0;
}


//********************************************************************************
//
// Function Name	: NFCHIP_WriteSubPage
//
// Author			    : Mars Chen
//
// Description		: read NAND Flash Status
//
//********************************************************************************
static int NFCHIP_WriteSubPage(struct mtd_info *mtd, u8 bChipNumber, u32 dwBuffer, u32 dwExtraBuffer,
	u32 dwSubPageAddr)
{   // 0x80, col, row, data(512), 0x85, col, data(16), 0x10, busy
	u32 SubPagesPerPage = NAND_Page_Size / NAND_SubPage_Size;
	u8   dwCE = bChipNumber;
	//u32   ret , value;
	u32 ret;
	u32 temp; 

	//Write is not Free-Run mode, so have to -1
	u32 NAND_SubPage_Size_W = NAND_SubPage_Size - 1;	
	
	// Reset Buffer - Need to check with Designer
	ntk_writel(0x1F, REG_NFC_SW_RESET);
	while(ntk_readl(REG_NFC_SW_RESET) != 0);
		
#if 1
	flush_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	flush_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	wb_dcache_range((u32)dwBuffer, NAND_Page_Size);
	wb_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif
 	
 	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);

	if(NAND_SubPage_Size == 1024){
		ntk_writel(ntk_readl(REG_NFC_CFG1)|(0x01<<23), REG_NFC_CFG1);
		ntk_writel(ntk_readl(REG_NFC_CFG1)&0xFF7FFFFF, REG_NFC_CFG1);
	}

	ntk_writel(dwBuffer & 0x1fffffff, REG_NFC_DMA_ADDR);
	
	//write oob area to registers
	write_oob_to_reg((u8 *)dwExtraBuffer);
	
	temp = NFC_DMA_CTRL_TRAN_BYTE_COUNT(NAND_SubPage_Size_W);
	ntk_writel(temp, REG_NFC_DMA_CTRL);

 	ntk_writel(dwSubPageAddr / SubPagesPerPage, REG_NFC_ROW_ADDR);
 	ntk_writel((dwSubPageAddr % SubPagesPerPage) * NAND_SubPage_Size, REG_NFC_COL_ADDR);
	
	temp = NFC_RAND_ACC_CMD_CODE0(0x85) |
		NFC_RAND_ACC_CMD_COL_ADDR((dwSubPageAddr % SubPagesPerPage) * NAND_SubOob_Size + NAND_Page_Size);
	ntk_writel(temp, REG_NFC_RAND_ACC_CMD);

	temp = NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE |
		NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_BLK_SIZE(NAND_SubPage_Size) | 
		NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET | NFC_TRAN_MODE_DATA_SEL_DMA | 
		NFC_TRAN_MODE_WRITE;
	ntk_writel(temp, REG_NFC_TRAN_MODE);

	temp = NFC_TRAN_MODE_KEEP_CE | NFC_CMD_WP_NEG | NFC_CMD_CE_IDX(dwCE) |
		NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL_ROW |
		NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x80);
	ntk_writel(temp, REG_NFC_CMD);
	 		
	while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_DATA_TRAN_COMPLETE));    // wait for transfer complete

	if(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW){
    		printk("[%s] SubPagesPerPage INT ERR\n", __FUNCTION__);  
        		return -1;
    	}
	
	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);    // clear command complete
	
	//2nd write command
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_ONE | 
		NFC_CMD_CODE0(0x10);
	ntk_writel(temp, REG_NFC_CMD);
	
	while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_CMD_COMPLETE));  // wait for command complete

#if 1
	invalidate_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);
	invalidate_dcache_range((u32)dwExtraBuffer, (u32)dwExtraBuffer+NAND_Oob_Size);
#else
	inv_dcache_range((u32)dwBuffer, NAND_Page_Size);
	inv_dcache_range((u32)dwExtraBuffer, NAND_Oob_Size);
#endif
	//invalidate_dcache_range(dwBuffer, 512);	
	ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR_RW);
	if (ret){
		u8 bStatus = NFCHIP_ReadStatus(bChipNumber);
		if(bStatus & 0x40){	//Status bit I/O<6>
			// ready
			if(bStatus & 0x01){   // fail, Status bit I/O<0>
				printk("[%s] IO6 ok, Write SubPage=%d FAILS\n" , __FUNCTION__, dwSubPageAddr);
				return -1;
			}
		}
		return 0;
	}else{
		printk("[%s] Write SubPage=%d FAILS\n" , __FUNCTION__, dwSubPageAddr);
		return -1;
	}
}

//********************************************************************************
// Function Name	: NFCHIP_EraseBlock
// Author			    : Mars Chen
// Description		: read NAND Flash Status
//********************************************************************************
static int NFCHIP_EraseBlock(struct mtd_info *mtd, u8 bChipNumber, u32 dwBlockAddr)
{   // 0x60, row, 0xd0, busy
	u8   dwCE = bChipNumber;
	//u32   ret , cmd;
	u32 ret;
	u32 temp;
	
	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);
	
	ntk_writel(dwBlockAddr * (NAND_Block_Size/NAND_Page_Size), REG_NFC_ROW_ADDR);
	ntk_writel(0x00, REG_NFC_COL_ADDR);
	
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_WP_NEG | NFC_CMD_ADDR_CYCLE_ROW | 
		NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0xd0) | 
		NFC_CMD_CODE0(0x60);
	ntk_writel(temp, REG_NFC_CMD);
	
	while(!(ntk_readl(REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));  // wait for command complete
	while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR)){
		// check status
		u8 bStatus = NFCHIP_ReadStatus(bChipNumber);
		unsigned int reg_num;
		printk("%s: Erase Status %02x\n" , __FUNCTION__, bStatus);
		if(bStatus & 0x40){   // ready
			if(bStatus & 0x01){   // fail
				// mark block as bad
				u8 bBuf[NAND_SubPage_Size] __attribute__((aligned(8)));
				u8 bExtraBuf[NAND_SubOob_Size] __attribute__((aligned(8)));
				u32 dw1stSubPageAddrInBlock = dwBlockAddr *  (NAND_Block_Size/NAND_Page_Size) * (NAND_Page_Size/NAND_SubPage_Size);
				u32 i;
				
				NFCHIP_Reset(mtd, bChipNumber);

				memset(bBuf, 0, sizeof(bBuf));
				memset(bExtraBuf, 0 , sizeof(bExtraBuf));
				//clear all oob data
				ntk_writel(0x00, REG_NFC_XTRA_DATA0);
				ntk_writel(0x00, REG_NFC_XTRA_DATA1);
				ntk_writel(0x00, REG_NFC_XTRA_DATA2);
				ntk_writel(0x00, REG_NFC_XTRA_DATA3);
				for ( i=0; i<24; i++){
					reg_num = REG_NFC_XTRA_DATA4 + i*4;
					ntk_writel(0x00, reg_num);
				}
			
				/* mark 2 pages in this block to denote bad block, here we mark 1st & 2nd page */
				NFCHIP_WriteSubPage(mtd, bChipNumber, (u32)bBuf, (u32)bExtraBuf, dw1stSubPageAddrInBlock);
				NFCHIP_WriteSubPage(mtd, bChipNumber, (u32)bBuf, (u32)bExtraBuf, dw1stSubPageAddrInBlock + (1 * (NAND_Page_Size/NAND_SubPage_Size)));

				NFCHIP_Reset(mtd, bChipNumber);
				ntk_writel(0xFFFFFFFF, REG_NFC_XTRA_DATA0);
				ntk_writel(0xFFFFFFFF, REG_NFC_XTRA_DATA1);
				ntk_writel(0xFFFFFFFF, REG_NFC_XTRA_DATA2);
				ntk_writel(0xFFFFFFFF, REG_NFC_XTRA_DATA3);
				for ( i=0; i<24; i++){
					reg_num = REG_NFC_XTRA_DATA4 + i*4;
					ntk_writel(0xFFFFFFFF, reg_num);
				}
				printk("[%s] Warning: Erase FAILs at %d\n" , __FUNCTION__, dwBlockAddr);
				//delay_us(5);
				return -1;
			} // end of if(bStatus & 1)
			break;
            
		} // end of if(bStatus & 0x40)
	}

	ret = !(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR);
	if (ret){
		return 0;
	}else{
		printk("%s: NFC_INT_ERR, REG_NFC_INT_STAT=%#lx\n" , __FUNCTION__, ntk_readl(REG_NFC_INT_STAT));
		return -1;
	}
}

//********************************************************************************
//
// Function Name	: NFCHIP_mass_erase
//
// Author			    : Mars Chen
//
// Description		: read NAND Flash Status
//
//********************************************************************************
static int NFCHIP_mass_erase (struct mtd_info *mtd, u8 bChipNumber, u32 nand_offset, u32 erase_size)
{
	u32 block_size, block_size_mask;
	u32 block_start, block_count;	
	u32 i;
	u32 retry_count=0;
	int status = E_OK;
	
	block_size = NAND_Page_Size * NAND_PPB;
	block_size_mask = block_size - 1; // make sure block size = 2^n
	
	if (nand_offset & block_size_mask)
	{ // start address not align to blcok boundary
		return E_SYS;
	}
	block_start = (nand_offset / block_size);
	
	if (erase_size & block_size_mask)
	{ // data size not align to block boundary
		return E_SYS;
	}
	block_count = (erase_size / block_size);

	for (i = 0; i < block_count; i++)
	{
		while( NFCHIP_EraseBlock(mtd, bChipNumber, (block_start + i)) != E_OK )
		{
			retry_count++;
			if (retry_count >=3)
			{
				status = E_SYS;
				break;
			}
			delay_us(1);
		}
		if (status == E_SYS)
		{
			break;
		}
		else
		{
			delay_us(1);
		}
	}
	
	return status;	
}

//********************************************************************************
// Function Name	: NFCHIP_ReadID
// Author			    : Mars Chen
// Description		: read NAND Flash ID
//********************************************************************************
static u32 NFCHIP_ReadID(struct mtd_info *mtd , u8 bChipNumber)
{	// 0x90, dummy, data(6)
	//H27UAG8T2A ID: AD D5 94 25 44 41
	//K9G8G08U0C ID: EC D3 84 72 50 42
	u8   dwCE = bChipNumber;
	u32   device_id=0x00, device_id_ext=0x00;
	struct nand_chip *nand = mtd->priv;
	struct NTK_NF_prv *prv = nand->priv;
	u32 temp;
	
	ntk_writel(0x00, REG_NFC_TRAN_MODE);
	temp = NFC_TRAN_MODE_CE_IDX(dwCE) | NFC_TRAN_MODE_BLK_SIZE(6); // new nand:transfer 6 bytes (ID)
	ntk_writel(temp, REG_NFC_TRAN_MODE);

	ntk_writel(0xFFFFFFFF, REG_NFC_INT_STAT);   // clear interrupt status
	
	ntk_writel(0x00, REG_NFC_CMD);
	temp = NFC_CMD_CE_IDX(dwCE) | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_DUMMY |
		NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x90);
	ntk_writel(temp, REG_NFC_CMD);
	
	// wait for command complete
	while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_CMD_COMPLETE));
	// wait for transfer complete
	while(!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_DATA_TRAN_COMPLETE));

	if (!(ntk_readl(REG_NFC_INT_STAT) & NFC_INT_ERR)){
		device_id = ntk_readl(REG_NFC_DATA_PORT);
		device_id_ext = ntk_readl(REG_NFC_DATA_PORT);
		//printk ("[%s] device_id=%#08x, device_id_ext=%#08x\n",  __FUNCTION__, device_id, device_id_ext);
		
		memcpy(prv->nand_data_buf , &device_id , sizeof(device_id));
		memcpy(prv->nand_data_buf+sizeof(device_id), &device_id_ext , sizeof(device_id_ext));
		printk ("%s: Manufacture ID=0x%02x, Chip ID=0x%02x, "
			"3thID=0x%02x, 4thID=0x%02x, 5thID=0x%02x 6thID=0x%02x\n",
			mtd->name, prv->nand_data_buf[0] , prv->nand_data_buf[1] , prv->nand_data_buf[2] , 
			prv->nand_data_buf[3] , prv->nand_data_buf[4], prv->nand_data_buf[5]);
		return device_id;
  	 }else{
  	 	printk("[%s] Warning: Read ID FAILS\n", __FUNCTION__);
		return 0;
  	 }
}


//********************************************************************************
//
// Public Interface Function
//
//********************************************************************************

/* Copy data from/to NAND main and spare buffers */
static void NTK_NF_buf_copy(struct mtd_info *mtd, u_char * buf, int len, int wr)
{
	struct nand_chip *nand = mtd->priv;
	struct NTK_NF_prv *prv = nand->priv;
	
	/* Handle spare area access */
	if (prv->spareonly || prv->nand_data_buf_idx >= NAND_Page_Size) {
		/* Calculate offset from beginning of spare area */
		if (!wr)
			memcpy(buf , prv->nand_ecc_buf , len);
		else
			memcpy(prv->nand_ecc_buf , buf, len);
		return;
	}
	
	/* Handle data area access */
	if(!wr){
		memcpy(buf , &(prv->nand_data_buf[prv->nand_data_buf_idx]) , len);
		prv->nand_data_buf_idx += len;
	}else{
		memcpy(&(prv->nand_data_buf[prv->nand_data_buf_idx]) , buf , len);
		prv->nand_data_buf_idx += len;
	}
}

/* Read data from NAND buffers */
static void NTK_NF_read_buf(struct mtd_info *mtd, u_char * buf, int len)
{
	NTK_NF_buf_copy(mtd, buf, len, 0);
}

/* Read byte from NAND buffers */
static u8 NTK_NF_read_byte(struct mtd_info *mtd)
{
	u8 tmp;
	
	NTK_NF_read_buf(mtd, &tmp, sizeof(tmp));

	return tmp;
}

/* Write data to NFC buffers */
static void NTK_NF_write_buf(struct mtd_info *mtd,
				  const u_char * buf, int len)
{
	//printk(KERN_ERR DRV_NAME ": NTK_NF_write_buf len %d\n" , len);
	NTK_NF_buf_copy(mtd, (u_char *) buf, len, 1);
}

static void NTK_NF_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	return;
}

static void NTK_NF_hweccenable(struct mtd_info *mtd, int mode)
{
	switch (mode) {
	case NAND_ECC_READ:
		//printk(KERN_ERR DRV_NAME ": NAND_ECC_READ \n");
		break;
	case NAND_ECC_WRITE:
		//printk(KERN_ERR DRV_NAME ": NAND_ECC_WRITE \n");
		break;
	default:
		break;
	}
	
}

static int NTK_NF_correct_data(struct mtd_info *mtd, u_char *dat,
				     u_char *read_ecc, u_char *calc_ecc)
{
	// we correct the data in NTK_ReadSubPage function , so we return the correction result here
	struct nand_chip *nand = mtd->priv;
	struct NTK_NF_prv *prv = nand->priv;

	if(prv->nand_uncorrectable)
		return -1;
	//printk(KERN_ERR DRV_NAME ": NTK_NF_correct_data \n");
	return 0;
}

static int NTK_NF_calculate_ecc(struct mtd_info *mtd, const u_char *dat,
				      u_char *ecc_code)
{
	// 
	//printk(KERN_ERR DRV_NAME ": NTK_NF_calculate_ecc \n");
	return 0;
}


/* Write command to NAND flash */
static void NTK_NF_command(struct mtd_info *mtd, unsigned command,
				int column, int page)
{
	//debug_nand("---------[%s]----------\n", __FUNCTION__);
	//u32 pReadBuf;
	struct nand_chip *chip = mtd->priv;
	struct NTK_NF_prv *prv = chip->priv;
	unsigned char SamePage;

	prv->spareonly = 0;
	SamePage = 0;

	//printk(KERN_ERR DRV_NAME ": NAND CMD Start\n");
	switch(command){
		case NAND_CMD_RESET:
			prv->nand_data_buf_idx = 0;
			NFCHIP_Reset(mtd , 0);
			break;
		case NAND_CMD_READID:
			prv->nand_data_buf_idx = 0;
			NFCHIP_ReadID(mtd , 0);
			break;
		case NAND_CMD_ERASE1:
			prv->nand_data_buf_idx = 0;
			prv->nand_first_read = 1;
			//printk(KERN_ERR DRV_NAME ": NAND_CMD_ERASE : Col : %d  Page : %d\n" , column , page);
			if(NFCHIP_mass_erase (mtd, 0 , page * NAND_Page_Size , NAND_Block_Size) != E_OK){
				printk("[%s] Erase Command Fail\n", __FUNCTION__);  
			}
			//printk(KERN_ERR DRV_NAME ": NAND_CMD_ERASE : Finish\n");
			break;
		case NAND_CMD_ERASE2:
			break;
		case NAND_CMD_READ0:
		case NAND_CMD_READ1:
			prv->nand_data_buf_idx = 0;

			if(NFCHIP_ReadPage_FreeRun(mtd , 0 , (u32)prv->nand_data_buf, (u32)prv->nand_ecc_buf , page ) == -1){
				printk("[%s] Read_FreeRun Command Fail\n", __FUNCTION__);  
			}else{
				//memcpy(prv->nand_data_buf , (void *)pReadBuf , NAND_Page_Size);
			}
			break;
		case NAND_CMD_SEQIN:	// 0x80
			prv->nand_data_buf_idx = 0;
			//printk(KERN_ERR DRV_NAME ": NAND_CMD_SEQIN : Col : %d  Page : %d\n" , column , page);
			prv->nand_ecc_buf[0] = 0xff;
			prv->nand_ecc_buf[1] = 0xff;
			prv->nand_write_page = page;
			break;
		case NAND_CMD_PAGEPROG:	// 0x10
			//printk(KERN_ERR DRV_NAME ": NAND_CMD_PAGEPROG : Col : %d  Page : %d\n" , column , prv->nand_write_page);
			prv->nand_first_read = 1;
			if(NFCHIP_WritePage(mtd , 0 , (u32)prv->nand_data_buf, (u32)prv->nand_ecc_buf , (prv->nand_write_page)) == -1){
				printk("[%s] Write Command Fail\n", __FUNCTION__);  
			}
			
			break;
		case NAND_CMD_READOOB:
			prv->spareonly = 1;

			if(NFCHIP_ReadPage_FreeRun(mtd , 0, (u32)prv->nand_data_buf, (u32)prv->nand_ecc_buf , page ) == -1){
				printk("[%s] Read_FreeRun Command Fail\n", __FUNCTION__);  
			}else{
				//memcpy(prv->nand_data_buf , (void *)pReadBuf , NAND_Page_Size);
			}
			break;
		case NAND_CMD_STATUS:
			prv->nand_data_buf_idx = 0;
			prv->nand_data_buf[0] = NFCHIP_ReadStatus(0);
			//flush_dcache_range(prv->nand_data_buf, prv->nand_data_buf+32);	// Mars @ 20100514 Suwon
				//wbflush_ahb();															// Mars @ 20100514 Suwon
			break;
		default:
			printk("Novatek NFC does not support this command = %#x\n", command);
	}
}

static int NTK_NF_ready(struct mtd_info *mtd)
{
	return (~((ntk_readl(REG_NFC_STAT >> 31)) & 0x01));
}

int board_nand_init(struct nand_chip *chip)
{
	struct NTK_NF_prv *prv;
	struct mtd_info *mtd;

	static int chip_nr = 0;

	prv = (struct NTK_NF_prv *)&NFDevice;
	printf("%s\n",__FUNCTION__);
	/* Init hardware */
	if(NFCHIP_Init() == E_SYS){
		return E_SYS;
	}
	
	mtd = &nand_info[chip_nr++];
	mtd->priv = chip;
	chip->priv = prv;

	prv->nand_first_read = 1;
	/* initialize nand_chip data structure */
	chip->cmdfunc = NTK_NF_command;
	chip->read_byte = NTK_NF_read_byte;
	chip->read_buf = NTK_NF_read_buf;
	chip->write_buf = NTK_NF_write_buf;
	chip->dev_ready = NTK_NF_ready;
	chip->cmd_ctrl = NTK_NF_hwcontrol;
	
	chip->ecc.hwctl = NTK_NF_hweccenable;
	chip->ecc.calculate = NTK_NF_calculate_ecc;
	chip->ecc.correct = NTK_NF_correct_data;
	chip->ecc.mode = NAND_ECC_HW;

	chip->ecc.size		= NAND_Page_Size;	//should be page size
	chip->ecc.bytes		= 168;	
	chip->ecc.layout = &NTK_NF_ecclayout;
	chip->badblock_pattern = &NTK_NF_bbt_descr;
	
	//Ken, 2010.09.09
	mtd->name = "ntk_nand";
	mtd->type	 = MTD_NANDFLASH;
	mtd->flags = MTD_CAP_NANDFLASH;
	mtd->oobsize = NAND_Oob_Size;
	mtd->writesize = NAND_Page_Size;
	mtd->erasesize = NAND_Block_Size;
	mtd->ecclayout = &NTK_NF_ecclayout;
	
	return 0;
}
