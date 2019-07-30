#ifndef __NVT_NAND_ECC_LAYOUT_H_
#define __NVT_NAND_ECC_LAYOUT_H_

#ifdef __NVT_NAND_UBOOT__
#include <linux/mtd/mtd.h>
#endif

#ifdef __NVT_NAND_KERNEL__
#include <linux/mtd/mtd.h>
#endif


enum {
    NAND_ECC_HAMMING = 0,
	NAND_ECC_RS_512_16B,
    NAND_ECC_BCH8_512_16B,
	NAND_ECC_BCH8_512_32B,
	NAND_ECC_BCH8_512_64B,
    NAND_ECC_BCH13_512_16B,
    NAND_ECC_BCH24_1024_54B_HW,
    NAND_ECC_BCH24_1024_54B_SW
};

#if (defined (__NVT_NAND_KERNEL__) || defined (__NVT_NAND_UBOOT__))
static struct nand_ecclayout hw_512B_page_ecclayout_RS = {
    .eccbytes = 10,
    .eccpos = {8, 9, 10, 11, 12, 13 },
    .oobfree = { {2, 6} }
};

static struct nand_ecclayout hw_2KB_page_ecclayout_RS = {
        .eccbytes = 40,
        .eccpos = {
                 6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
                22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
                54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
        .oobfree = { {2, 4},  {16, 6}, {32, 6}, {48, 6} }
};

static struct nand_ecclayout hw_4KB_page_ecclayout_BCH24_1024_54 = {
        .eccbytes = 168,  /*42 * 4*/
        .eccpos = {
                12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
				25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
				38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
                51, 52, 53, 

                66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
				79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
                92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104,
                105, 106, 107,

                120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
				133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145,
                146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
                159, 160, 161,

                174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
				187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
                200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
                213, 214, 215},

        .oobfree = { {2, 10},  {54, 12}, {108, 12}, {162, 12} }
};
#endif
#endif
