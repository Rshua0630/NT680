
#include <common.h>

#include <asm/addrspace.h>
#include <asm/io.h>

phys_size_t initdram(int board_type)
{
	return CONFIG_SYS_MEM_SIZE;
}

int checkboard(void)
{
	return 0;
}
