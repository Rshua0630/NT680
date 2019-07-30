/*
 *  board/nvt/evb668/nt72668_spi_fwupdate.h
 *
 *  Author:	Alvin lin
 *  Created:	Dec 16, 2011
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NT72668_SPI_FIRMWARE_UPDATE_H__
#define __NT72668_SPI_FIRMWARE_UPDATE_H__

#include "nt72668_spi_partition.h"

int nt72668_spi_burn_bin(SPI_PARTITION partition,u8* buf,u32 len);

#endif
