#ifndef __NVT_NAND_656UBOOT_H__
#define __NVT_NAND_656UBOOT_H__


#define printk                         printf
//#define NFC_DBG                        printf
#define NFC_DBG(_fmt, args...)                       
#define KERN_ERR

#define WREG(info, off, val)	(*((volatile unsigned int*)((unsigned char*)(info)->pControllerBaseAddr + (off))) = val)
#define RREG(info, off)	        (*(volatile unsigned int*)((unsigned char*)(info)->pControllerBaseAddr + (off)))

#define WREG_ADDR(Addr, val)	(*((volatile unsigned int*)(Addr)) = val)
#define RREG_ADDR(Addr)	        (*(volatile unsigned int*)(Addr))

#define __iomem
#define ioremap_nocache(x, y)   ((void*)x)
#define iounmap(x) 


#endif
