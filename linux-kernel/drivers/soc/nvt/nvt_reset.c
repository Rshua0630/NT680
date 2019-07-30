/*
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 *
 */

#include <asm/io.h>
#include <nvt_stbc_regs.h>
#include <nvt_stbc.h>

void nvt_machine_restart(char str, const char *cmd);
void nvt_machine_power_off(void);

void nvt_machine_restart(char str, const char *cmd)
{
	printk("Kernel restart stbc!\n");

	/*nvt_soc_stbc_cmd(KER_STBC_IPC_CMD_RESET_STBC);*/

	/*fix to normal reset temporally,
	 * due to wdt reset (0xC4) has some issue*/
	nvt_soc_stbc_cmd(KER_STBC_IPC_CMD_RESET);
}

void nvt_machine_power_off(void)
{
	printk("Kernel power off stbc!\n");

	nvt_soc_stbc_cmd(KER_STBC_IPC_CMD_POWER_OFF);
}
