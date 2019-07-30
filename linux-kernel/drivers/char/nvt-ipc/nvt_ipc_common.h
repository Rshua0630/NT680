#ifndef _NVT_IPC_COMMON_H
#define _NVT_IPC_COMMON_H

#define NVTIPC_INTERFACE_VER        0x17032410
#define NVTIPC_SHARED_MEM_ADDR      0x00080000  // the ipc shared memory is in DRAM 512KB.
#define NVTIPC_SHARED_MEM_SIZE      1024*4
#define NVTIPC_SHARED_PM_ADDR       NVTIPC_SHARED_MEM_ADDR + NVTIPC_SHARED_MEM_SIZE
#define NVTIPC_SHARED_PM_SIZE       1024*4


#define NVTIPC_SEM_NUM                   8
#define NVTIPC_MSG_QUEUE_NUM             16 ///<  queue number
#define NVTIPC_MSG_ELEMENT_SIZE          24 ///<  queue element size
#define NVTIPC_CORE_NUM                   4
#define NVTIPC_MSG_SYS_CMD_MAX_LEN      256
#define NVTIPC_SHM_HEADER_SIZE            4 ///<  each queue reserved share memory size
#define NVTIPC_SHM_SIZE                  12 ///<  each queue reserved share memory size


/**
   CPU status.

*/
typedef enum {
	NVTIPC_CPUSTS_POWEROFF = 0,
	NVTIPC_CPUSTS_POWERON_READY,
	NVTIPC_CPUSTS_SUSPEND,
	NVTIPC_CPUSTS_SUSPEND_ING,
	NVTIPC_CPUSTS_REBOOT_ING,
	NVTIPC_CPUSTS_POWEROFF_ING,
	ENUM_DUMMY4WORD(NVTIPC_CPUSTS)
} NVTIPC_CPUSTS;


typedef struct _NVTIPC_MSG {
	UINT32 size;
	UINT32 data[(NVTIPC_MSG_ELEMENT_SIZE + 3) / 4];
} NVTIPC_MSG;

typedef struct _NVTIPC_SEND_MSG {
	UINT32           sendTo;
	UINT32           OpCmd;
	UINT32           msgData[(NVTIPC_MSG_ELEMENT_SIZE + 3) / 4];
} NVTIPC_SEND_MSG;

typedef struct _NVTIPC_CORE_CMD_BUF {
	NVTIPC_SEND_MSG                buf[NVTIPC_CORE_NUM][2];
	char                           SysCmd[NVTIPC_MSG_SYS_CMD_MAX_LEN];
} NVTIPC_CORE_CMD_BUF;

typedef struct {
	UINT16                         key;
	UINT16                         ref_count;
	UINT32                         buf[NVTIPC_SHM_SIZE / 4];
} NVTIPC_SHM_BUF;


// the variables in this structure in shared in CPU1 & CPU2 ,
// so need to use volatile for each variable
typedef struct _NVTIPC_SHM_CTRL {
	volatile unsigned int          InterfaceVer;   // this should be first one
	volatile unsigned int          SemBits;        // this should be second one
	// the InterfaceVer and SemBits can not move to other position

	// -----------------------------------
	volatile unsigned int          Cpu1AckBits;
	volatile unsigned int          Cpu2AckBits;
	volatile unsigned int          Cpu1SemWaitCnt[NVTIPC_SEM_NUM];
	volatile unsigned int          Cpu2SemWaitCnt[NVTIPC_SEM_NUM];
	volatile unsigned int          Cpu1SemLockTime;
	volatile unsigned int          Cpu1SemUnLockTime;
	volatile unsigned int          Cpu2SemLockTime;
	volatile unsigned int          Cpu2SemUnLockTime;
	volatile int                    Cpu1Cpu2Err;
	volatile int                    Cpu1Cpu3Err;
	volatile int                    Cpu1Cpu4Err;
	volatile int                    Cpu2Cpu1Err;
	volatile int                    Cpu2Cpu3Err;
	volatile int                    Cpu2Cpu4Err;
	volatile int                    Cpu3Cpu1Err;
	volatile int                    Cpu3Cpu2Err;
	volatile int                    Cpu3Cpu4Err;
	volatile int                    Cpu4Cpu1Err;
	volatile int                    Cpu4Cpu2Err;
	volatile int                    Cpu4Cpu3Err;
	volatile unsigned int          Cpu1Cpu2Key;
	volatile unsigned int          Cpu1Cpu3Key;
	volatile unsigned int          Cpu1Cpu4Key;
	volatile unsigned int          Cpu2Cpu1Key;
	volatile unsigned int          Cpu2Cpu3Key;
	volatile unsigned int          Cpu2Cpu4Key;
	volatile unsigned int          Cpu3Cpu1Key;
	volatile unsigned int          Cpu3Cpu2Key;
	volatile unsigned int          Cpu3Cpu4Key;
	volatile unsigned int          Cpu4Cpu1Key;
	volatile unsigned int          Cpu4Cpu2Key;
	volatile unsigned int          Cpu4Cpu3Key;
	volatile unsigned int          ubootCmd[2];
	volatile unsigned int          Cpu1Sts;
	volatile unsigned int          Cpu2Sts;
	volatile unsigned int          Dsp1Sts;
	volatile unsigned int          Dsp2Sts;
	volatile unsigned int          ddr1_addr;          ///< ddr1 starting address.
	volatile unsigned int          ddr1_size;          ///< ddr1 size.
	volatile unsigned int          uitron_addr;        ///< uitron starting address.
	volatile unsigned int          uitron_size;        ///< uitron size.
	volatile unsigned int          ddr2_addr;          ///< ddr2 starting address.
	volatile unsigned int          ddr2_size;          ///< ddr2 size.
	NVTIPC_CORE_CMD_BUF            core_cmd_buf[NVTIPC_CORE_NUM];
	NVTIPC_SHM_BUF                 shmbuf[NVTIPC_MSG_QUEUE_NUM];
	unsigned int                   reserved[4];
} NVTIPC_SHM_CTRL;

// the variables in this structure in shared in CPU1 & CPU2 ,
// so need to use volatile for each variable
typedef struct _NVTIPC_SHM_PM_INFO {
       volatile unsigned int          s3_suspend_after_reg_0;   // Linux mem region after suspend #1 cksum
       volatile unsigned int          s3_resume_before_reg_0;   // Linux mem region before resume #1 cksum
       volatile unsigned int          s3_suspend_after_reg_1;   // Linux mem region after suspend #2 cksum
       volatile unsigned int          s3_resume_before_reg_1;   // Linux mem region before resume #2 cksum
} NVTIPC_SHM_PM_INFO;

typedef enum _NVTIPC_SEM_CMD_ {
	NVTIPC_SEM_CMD_INIT = 0,
	NVTIPC_SEM_CMD_UNLOCK,
	NVTIPC_SEM_CMD_UNLOCK_ACK,
	ENUM_DUMMY4WORD(NVTIPC_SEM_CMD)
} NVTIPC_SEM_CMD;

typedef struct _NVTIPC_SEM_MSG {
	NVTIPC_SEM_CMD   CmdID;              ///< command ID.
	unsigned int     semid;              ///< semaphore id
} NVTIPC_SEM_MSG;


#endif //_NVTIPC_INT_H
