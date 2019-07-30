#ifndef _NT72_NAND_KERNEL_H_
#define _NT72_NAND_KERNEL_H_

int __init nt72_nand_probe(struct platform_device *pdev) __init;
int nt72_nand_remove(struct platform_device *pdev);

#endif /*  _NT72_NAND_KERNEL_H_ */
