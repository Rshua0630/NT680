/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include "../../../drivers/mmc/nt72668_mmc.h"


DECLARE_GLOBAL_DATA_PTR;

const char *boardinfo[] = {
	"Board: NT72656 DTV Turnkey board\n"
};

/**
 * @brief board_init
 *
 * @return 0
 */
int board_init(void)
{
	usb_power_on();
	return 0;
}

int board_mmc_init(bd_t *bis)
{
	nt72668_mmc_init();
	return 0;
}

#if 0 
int board_nand_init(bd_t *bis)
{
	return 0;
}
#endif
int board_eth_init(bd_t *bis)
{
	return 0;
}

/*
* Routine: get_board_revision
* Description: Detect if we are running on a NT72656 Turnkey board.
*/
int get_board_revision(void)
{
	int board_id;
	return board_id;
}

/**
 * @brief misc_init_r - Configure Panda board specific configurations
 * such as power configurations, ethernet initialization as phase2 of
 * boot sequence
 *
 * @return 0
 */
int misc_init_r(void)
{
	get_board_revision();
	return 0;
}

/*
 * get_board_rev() - get board revision
 */
u32 get_board_rev(void)
{
	return 0;
}
