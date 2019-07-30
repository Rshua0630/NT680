/*********************************************************************
**
** File Name: ker_ci_syscalls.h
**
** Description: DVBT Common Interface
**
** Project:
**
** Creation Date:
**
***********************************************************************/

#ifndef _KER_CE_SYSCALLS_H
#define _KER_CE_SYSCALLS_H


#define CIPINSTATUS_CD1            1    /* CD1T# */
#define CIPINSTATUS_CD2            2    /* CD2# */
#define CIPINSTATUS_READY_IRQ      4    /* IRQ#/RDY */
#define CIPINSTATUS_READY_IRQ2     8    /* IRQ#/RDY */
#define CIPINSTATUS_WAIT          16    /* WAIT# */

/*---------------------------------------------------------------------------*/
/* Types Declarations                                                        */
/*---------------------------------------------------------------------------*/

/* General purpose commands of ioctl system call */
/* system... */
enum {
	NTDEV_IOCTLID_AV = 0xB2,
	NTDEV_IOCTLID_FR2IP,
	NTDEV_IOCTLID_FRE,
	NTDEV_IOCTLID_VOD,
	NTDEV_IOCTLID_IMG,
	NTDEV_IOCTLID_OSD,
	NTDEV_IOCTLID_FP,
	NTDEV_IOCTLID_MEM,
	NTDEV_IOCTLID_PQ,
	NTDEV_IOCTLID_TVENC,
	NTDEV_IOCTLID_VBI,
	NTDEV_IOCTLID_CI,
	NTDEV_IOCTLID_CRYPTO,
	NTDEV_IOCTLID_CRYPTO2,
	NTDEV_IOCTLID_PTI,
	NTDEV_IOCTLID_TUN,
	NTDEV_IOCTLID_REG,
	NTDEV_IOCTLID_CPE,
	NTDEV_IOCTLID_SCS,
	NTDEV_IOCTLID_DIRECTIO,
	NTDEV_IOCTLID_SB,
	NTDEV_IOCTLID_HDMI,
	NTDEV_IOCTLID_TSMUX,
	NTDEV_IOCTLID_VENC,
	NTDEV_IOCTLID_DEMURA,

	NTDEV_IOCTLID_MAX

};

enum {
	IOWR_NUM_IOCTL_NONE = 0,
	IOWR_NUM_IOCTL_INIT_REG,
	IOWR_NUM_IOCTL_DETECT_TIMEOUT,
	IOWR_NUM_IOCTL_INTSTATUS,
	IOWR_NUM_IOCTL_RESET,
	IOWR_NUM_IOCTL_GETSTATUS,
	IOWR_NUM_IOCTL_ATTR_READ,
	IOWR_NUM_IOCTL_ATTR_WRITE,
	IOWR_NUM_IOCTL_IO_READ,
	IOWR_NUM_IOCTL_IO_WRITE,
	IOWR_NUM_IOCTL_SET_ROUTE,
	IOWR_NUM_IOCTL_GET_ROUTE,
	IOWR_NUM_IOCTL_GET_ENCRYPT_MEM,
	IOWR_NUM_IOCTL_EN50221_RELEASE,
	IOWR_NUM_IOCTL_CARDINT_EN,

	IOWR_NUM_IOCTL_TERM = 15,

} IOWR_NUM_t;

#define	IOCTL_CI_TERM				_IO(NTDEV_IOCTLID_CI, 15)


struct CI_Ioctl_Get_Encrypt_Mem_t {
	u32 physAddr;
	u32 size;
	u32 virtuAddr;
	u32 ecRetVal;
};
#define IOCTL_CI_GET_ENCRYPT_MEM  \
	_IOWR(NTDEV_IOCTLID_CI, \
	IOWR_NUM_IOCTL_GET_ENCRYPT_MEM, \
	struct CI_Ioctl_Get_Encrypt_Mem_t)

struct CI_Ioctl_EN50221_Release_t {
	u32 rlflag;
	u32 ecRetVal;
};
#define IOCTL_CI_EN50221_RELEASE \
	_IOWR(NTDEV_IOCTLID_CI, \
	IOWR_NUM_IOCTL_EN50221_RELEASE, \
	struct CI_Ioctl_EN50221_Release_t)

#endif /*#ifndef _KER_CE_SYSCALLS_H*/
