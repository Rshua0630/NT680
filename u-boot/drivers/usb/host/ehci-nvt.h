/*-
 * Copyright (c) 2007-2008, Juniper Networks, Inc.
 * Copyright (c) 2008, Michael Trimarchi <trimarchimichael@yahoo.it>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef USB_EHCI_NVT_H
#define USB_EHCI_NVT_H

//#if !defined(CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS)
//#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	1
//#endif

#define NVT_USB0_APB_BASE  (0xFD170000)
#define NVT_USB1_APB_BASE  (0xFD178000)
#define NVT_USB2_APB_BASE  (0xFD180000)
#define NVT_USB3_APB_BASE  (0xFD184000)
#define clear(add,wValue)    writel(~(wValue)&readl(add),add)
#define set(add,wValue)      writel(wValue|readl(add),add)

#define setMode 		(1<<4|1<<5|1<<10|1<<8|1<<9|1<<11|1<<12)
#define clearMode 		(1|1<<5|1<<10|1<<8|1<<9)

void usb_power_on(void);
void usb_power_off(void);

#endif /* USB_EHCI_NVT_H */
