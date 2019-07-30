#!/usr/bin/python

import sys, os
import math

W = 1000
SP = 128 * 1024
SL = 124 * 1024
BB = 0
O = SP - SL

# UBI uses some amount of flash space for its own purposes, thus reducing the amount of flash space available for UBI users. Namely
# 2 PEBs are used to store the volume table;
# 1 PEB is reserved for wear-leveling purposes;
# 1 PEB is reserved for the atomic LEB change operation;
# some amount of PEBs is reserved for bad PEB handling; this is applicable for NAND flash, but not for NOR flash; the amount of reserved PEBs is configurable and is equal to 20 blocks per 1024 blocks by default;
# UBI stores the EC and VID headers at the beginning of each PEB; the amount of bytes used for these purposes depends on the flash type and is explained below.
# 
# W	- total number of physical eraseblocks on the flash chip (NB: the entire chip, not the MTD partition);
# P	- total number of physical eraseblocks on the MTD partition);
# SP	- physical eraseblock size;
# SL	- logical eraseblock size;
# BB	- number of bad blocks on the MTD partition;
# BR	- number of PEBs reserved for bad PEB handling. it is 20 * W/1024 for NAND by default, and 0 for NOR and other flash types which do not have bad PEBs;
# B	- MAX(BR,BB);
# O	- the overhead related to storing EC and VID headers in bytes, i.e. O = SP - SL.

# in case of NOR flash which has 1 byte minimum input/output unit, O is 128 bytes;
# in case of NAND flash which does not have sub-pages (e.g., MLC NAND), O is 2 NAND pages, i.e. 4KiB in case of 2KiB NAND page and 1KiB in case of 512 bytes NAND page;
# in case of NAND flash which has sub-pages, UBI optimizes its on-flash layout and puts the EC and VID headers at the same NAND page, but different sub-pages; in this case O is only one NAND page;
# for other flashes the overhead should be 2 min. I/O units if the min. I/O unit size is greater or equivalent to 64 bytes, and 2 times 64 bytes aligned to the min. I/O unit size if the min. I/O unit size is less than 64 bytes.

# N.B.: the formula above counts bad blocks as a UBI overhead. The real UBI overhead is: (B - BB + 4) * SP + O * (P - B - 4).


def usage():
        print "Usage: %s PartitionSize(Bytes)" % sys.argv[0]

def get_ubifs_max_leb(partition_size):		#Unit: Bytes
	P = partition_size/SP
	BR = math.ceil(30 * W/float(1024))	# Sync with kernel
	B = max(BB, BR)

	ubi_overhead = (( B - BB + 4 ) * SP + O * ( P - B - 4 ))/float(SP)
	ubi_overhead = math.ceil(ubi_overhead)
	ubi_overhead = math.floor(((P - ubi_overhead) * SP) / float(SL))
	return ubi_overhead * 0.98

def main():
	PTS = int(sys.argv[1])
	print "The MAX LEB is %d" %(get_ubifs_max_leb(PTS))
	

if __name__ == "__main__":
	if (len(sys.argv) < 2)  :
		usage()
		sys.exit(0)

        main()
