#ifndef _NVTMPP_H
#define _NVTMPP_H
/*
    Copyright (c) 2017~  Novatek Microelectronics Corporation

    @file nvtmpp.h

    @version

    @date
*/
#include <stdio.h>
#include <Type.h>
#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef signed long         NVTMPP_VB_POOL;     ///< Signed 32 bits data type
typedef signed long         NVTMPP_VB_BLK;      ///< Signed 32 bits data type
typedef unsigned long long  NVTMPP_MODULE;      ///< Unsigned 64 bits data type


#define MAKE_NVTMPP_MODULE(ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7)  \
	((UINT64)(UINT8)(ch0) | ((UINT64)(UINT8)(ch1) << 8) |   \
	 ((UINT64)(UINT8)(ch2) << 16) | ((UINT64)(UINT8)(ch3) << 24 ) |\
	 ((UINT64)(UINT8)(ch4) << 32) | ((UINT64)(UINT8)(ch5) << 40 ) |\
	 ((UINT64)(UINT8)(ch6) << 48) | ((UINT64)(UINT8)(ch7) << 56 )  \
	)

#define NVTMPP_DDR_MAX_NUM          3
#define NVTMPP_VB_MAX_POOLS         64
#define NVTMPP_VB_MAX_COMM_POOLS    16
#define NVTMPP_VB_MAX_BLK_EACH_POOL 64
#define NVTMPP_VB_MAX_POOL_NAME     23

#define NVTMPP_VB_INVALID_POOL     -1
#define NVTMPP_VB_INVALID_BLK      -1
#define NVTMPP_VB_BLK_SIZE_MAX     -1

#define NVTMPP_TEMP_POOL_NAME      "NVTMPP_TEMP"

/**
   Error Code.

   Any error code occurred will display on uart or return.
*/
typedef enum _NVTMPP_ER {
	NVTMPP_ER_OK                      =   0, ///< no error
	NVTMPP_ER_PARM                    =  -1, ///< has some error of input parameter
	NVTMPP_ER_POOL_UNEXIST            =  -2, ///< pool not exist
	NVTMPP_ER_NOBUF                   =  -3, ///< not enough buffer to allocate
	NVTMPP_ER_2MPOOLS                 =  -4, ///< create too many pools
	NVTMPP_ER_UNCONFIG                =  -5, ///< vb not config
	NVTMPP_ER_UNINIT                  =  -6, ///< vb not init
	NVTMPP_ER_INIT_ALREADY            =  -7, ///< vb already inited
	NVTMPP_ER_BLK_UNEXIST             =  -8, ///< blk not exist, the blk ID is invalid
	NVTMPP_ER_BLK_UNLOCK_REF          =  -9, ///< the block total refere count is 0 and want to unlock
	NVTMPP_ER_BLK_UNLOCK_MODULE_REF   = -10, ///< the block module's refere count is 0 and want to unlock
	NVTMPP_ER_BLK_ALREADY_FREE        = -11, ///< the block is already freed and want to reference
	NVTMPP_ER_UNKNOWW_MODULE          = -12, ///< the module is not in module list
	NVTMPP_ER_2MODULES                = -13, ///< Add too manys modules
	NVTMPP_ER_RESID_NOT_INSTALL       = -14, ///< Resource ID is not install
} NVTMPP_ER;

/**
   DDR ID.

*/
typedef enum {
	NVTMPP_DDR_1                    =   0, ///< DDR1
	NVTMPP_DDR_2                    =   1, ///< DDR2
	NVTMPP_DDR_1_PLUG               =   2, ///< DDR1 plug
	NVTMPP_DDR_MAX = NVTMPP_DDR_1_PLUG,
	ENUM_DUMMY4WORD(NVTMPP_DDR)
} NVTMPP_DDR;

typedef struct {
	UINT32     blk_size;                ///< vb block size
	UINT32     blk_cnt;                 ///< vb block count
	NVTMPP_DDR ddr;                     ///< ddr ID
} NVT_VB_CPOOL_S;

/**
   Video buffer pools configure structure.

*/
typedef struct {
	MEM_RANGE      ddr_mem[NVTMPP_DDR_MAX_NUM];           ///< ddr memory range
	UINT32         max_pool_cnt;                          ///< max count of pools, (0,NVTMPP_VB_MAX_POOLS]
	NVT_VB_CPOOL_S common_pool[NVTMPP_VB_MAX_COMM_POOLS]; ///< common pools
	BOOL           use_reserved_mem;
} NVTMPP_VB_CONF_S;


STATIC_ASSERT(NVTMPP_DDR_MAX_NUM == NVTMPP_DDR_MAX+1);


extern void    nvtmpp_install_id(void) _SECTION(".kercfg_text");

/**
    Set the video buffer pools configuration.

    @param p_vb_conf: the configure parameters.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {

        NVTMPP_ER        ret;
        NVTMPP_VB_CONF_S st_conf;

        memset(&st_conf,0,sizeof(NVTMPP_VB_CONF_S));
        st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
        st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);
        st_conf.max_pool_cnt = 64;
        st_conf.common_pool[0].blk_size = 1920*1080*3/2 + 1024;
        st_conf.common_pool[0].blk_cnt = 16;
        st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
        st_conf.common_pool[1].blk_size = 640*480*3/2 + 1024;
        st_conf.common_pool[1].blk_cnt = 4;
        st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
        ret = nvtmpp_vb_set_conf(&st_conf);
        if (NVTMPP_ER_OK != ret) {
            DBG_ERR("nvtmpp set vb config err: %d\r\n", ret);
            return -1;
        }
    }
    @endcode

*/
NVTMPP_ER      nvtmpp_vb_set_conf(const NVTMPP_VB_CONF_S *p_vb_conf);

/**
    Get the video buffer pools configuration.

    @param p_vb_conf: the configure parameters.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_ER        ret;
        NVTMPP_VB_CONF_S st_conf;

        memset(&st_conf,0,sizeof(NVTMPP_VB_CONF_S));
        ret = nvtmpp_vb_get_conf(&st_conf);
        if (NVTMPP_ER_OK != ret) {
            DBG_ERR("nvtmpp get vb config err: %d\r\n", ret);
            return -1;
        }
    }
    @endcode

*/
NVTMPP_ER      nvtmpp_vb_get_conf(NVTMPP_VB_CONF_S *p_vb_conf);


/**
    Initialize video buffer common pools.

    Initialize video buffer common pools that set by nvtmpp_vb_set_conf().

    @note before call nvtmpp_vb_init(), need to call nvtmpp_vb_set_conf()
          to set the configurations firstly.

    @param void

    @return NVTMPP_ER_OK for success, < 0 when some error happened,
            the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        ret = nvtmpp_vb_init();
        if (NVTMPP_ER_OK != ret) {
            DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
            return FALSE;
        }
    }
    @endcode

*/
NVTMPP_ER      nvtmpp_vb_init(void);

/**
    Un-initialize video buffer pools.

    @param void

    @return NVTMPP_ER_OK for success, < 0 when some error happened,
            the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_ER        ret;
        ret = nvtmpp_vb_exit();
        if (NVTMPP_ER_OK != ret) {
            DBG_ERR("nvtmpp exit vb err: %d\r\n", ret);
            return -1;
        }
    }
    @endcode

*/
NVTMPP_ER      nvtmpp_vb_exit(void);


/**
    Create a private pool.

    @param pool_name: the pool name that want to create.
    @param blk_size:  each block size of this pool.
    @param blk_cnt:   the block count of this pool.
    @param ddr:       create pool in which DDR.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_VB_POOL pool;
        NVTMPP_VB_BLK  blk;
        UINT32         blk_size = 320*240*2;
        UINT32         blk_cnt =  5;
        NVTMPP_ER      ret;

        pool = nvtmpp_vb_create_pool("module1",blk_size,blk_cnt,NVTMPP_DDR_1);
        if ( NVTMPP_VB_INVALID_POOL == pool ) {
            DBG_ERR("create private pool err\r\n");
            return FALSE;
        }
    }
    @endcode
*/
NVTMPP_VB_POOL nvtmpp_vb_create_pool(CHAR *pool_name, UINT32 blk_size, UINT32 blk_cnt, NVTMPP_DDR ddr);

/**
    Destroy a private pool.

    @param pool: the pool handle.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_VB_POOL pool = g_pool;
        NVTMPP_ER      ret;

        ret = nvtmpp_vb_destroy_pool(pool);
        if (NVTMPP_ER_OK != ret) {
            DBG_ERR("destory pool 0x%x\r\n",pool);
        }
    }
    @endcode
*/
NVTMPP_ER      nvtmpp_vb_destroy_pool(NVTMPP_VB_POOL pool);

/**
    Get one new block from video buffer pools.

    Get one new block from video buffer common pools or private pools.
    When the parameter pool input value is NVTMPP_VB_INVALID_POOL means want to
    get block from common pools, else want to get block from one specific private pool.


    @param module: the module id.
    @param pool: the pool handle.
    @param blk_size: the pool handle.
    @param ddr: DDR1 or DDR2.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_MODULE  module = g_module;
		NVTMPP_VB_POOL pool = NVTMPP_VB_INVALID_POOL;
		UINT32         blk_size = 1920 * 1080 * 2;
		NVTMPP_DDR     ddr = NVTMPP_DDR_1;
		NVTMPP_VB_BLK  blk;

        blk = nvtmpp_vb_get_block(module, pool, blk_size, ddr);
		if (NVTMPP_VB_INVALID_BLK == blk) {
			DBG_ERR("ge blk fail\r\n");
			return FALSE;
		}
    }
    @endcode
*/
NVTMPP_VB_BLK  nvtmpp_vb_get_block(NVTMPP_MODULE module, NVTMPP_VB_POOL pool, UINT32 blk_size, NVTMPP_DDR ddr); // default reference count �� 1�A�s�W�@�� reference blk ����

/**
    Translate video buffer block handle to buffer address.

    @param blk: the block handle.

    @return the block buffer address, return 0 when some error happened,

    Example:
    @code
    {
        UINT32             addr;
		NVTMPP_VB_BLK      blk = g_blk;

		addr = nvtmpp_vb_block2addr(blk);
		if (addr == 0) {
			DBG_ERR("block2addr fail, blk = 0x%x\r\n", blk);
			return FALSE;
		}
    }
    @endcode
*/
UINT32         nvtmpp_vb_block2addr(NVTMPP_VB_BLK blk);

/**
    Translate video buffer address to block handle.

    @param blk: the block address.

    @return the block handle, return NVTMPP_VB_INVALID_BLK when some error happened,

    Example:
    @code
    {
        UINT32             addr = g_addr;
		NVTMPP_VB_BLK      blk;

		blk = nvtmpp_vb_addr2block(addr);
		if (NVTMPP_VB_INVALID_BLK == blk) {
			DBG_ERR("addr2block fail, addr = 0x%x\r\n", addr);
			return FALSE;
		}
    }
    @endcode
*/
NVTMPP_VB_BLK  nvtmpp_vb_addr2block(UINT32 blk_addr);

/**
    Translate video buffer address to pool handle.

    @param blk: the block address.

    @return the pool handle, return NVTMPP_VB_INVALID_POOL when some error happened,

    Example:
    @code
    {
        UINT32             addr = g_addr;
		NVTMPP_VB_POOL     pool;

		pool = nvtmpp_vb_addr2pool(addr);
		if (NVTMPP_VB_INVALID_POOL == pool) {
			DBG_ERR("addr2pool fail, addr = 0x%x\r\n", addr);
			return FALSE;
		}
    }
    @endcode
*/
NVTMPP_VB_POOL nvtmpp_vb_addr2pool(UINT32 blk_addr);

/**
    Lock the video buffer block.

    This API will cause video buffer block reference count+1.

    @param module: the module that want to lock this block.
    @param blk: the block handle.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_ER          ret;
		NVTMPP_MODULE      module = g_module;

		ret = nvtmpp_vb_lock_block(module, g_blk);
		if (NVTMPP_ER_OK != ret) {
			DBG_ERR("lock blk fail %d\r\n", ret);
			return FALSE;
		}
    }
    @endcode
*/
NVTMPP_ER      nvtmpp_vb_lock_block(NVTMPP_MODULE module, NVTMPP_VB_BLK blk);


/**
    Unlock the video buffer block.

    This API will cause video buffer block reference count-1.
    If reference count reaches zero, then the block will be released and return back to pools.

    @param module: the module that want to unlock this block.
    @param blk: the block handle.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_ER          ret;
		NVTMPP_MODULE      module = g_module;

		ret = nvtmpp_vb_unlock_block(module, g_blk);
		if (NVTMPP_ER_OK != ret) {
			DBG_ERR("unlock blk fail %d\r\n", ret);
			return FALSE;
		}
    }
    @endcode
*/
NVTMPP_ER      nvtmpp_vb_unlock_block(NVTMPP_MODULE module, NVTMPP_VB_BLK blk);

/**
    Add a new module to module list.

    @param module: the module that want to add.

    @return NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
        NVTMPP_ER          ret;
		NVTMPP_MODULE      module = MAKE_NVTMPP_MODULE('V', 'd', 'o', 'E', 'n', 'c', 0, 0);

		ret = nvtmpp_vb_add_module(module);
		if (NVTMPP_ER_OK != ret) {
			DBG_ERR("add module err %d\r\n", ret);
		}
    }
    @endcode
*/
NVTMPP_ER      nvtmpp_vb_add_module(NVTMPP_MODULE module);

/**
    Translate module to a string.

    @param module: the module handle.
    @param buf: the buffer to store the module string.
    @param buflen: the buffer length.

    @return  NVTMPP_ER_OK for success, < 0 when some error happened, the error code can reference NVTMPP_ER.

    Example:
    @code
    {
	    CHAR           module_str[sizeof(NVTMPP_MODULE) + 1];
		NVTMPP_ER      ret;

		ret = nvtmpp_vb_module_to_string(g_module, module_str, sizeof(module_str));
		if (NVTMPP_ER_OK != ret) {
			DBG_ERR("get module string err\r\n");
			return FALSE;
		}
		DBG_DUMP("module_str = %s\r\n",module_str);
	}
    @endcode
*/

NVTMPP_ER      nvtmpp_vb_module_to_string(NVTMPP_MODULE module, char *buf, UINT32 buflen);


/**
    Get block last reference module.

    This API is for debug using, it will return the block last reference module.
    It can help debug when nvtmpp_vb_unlock_block() return some error code.

    @param blk: the block handle.

    @return the module handle, return 0 when some error happened.

    Example:
    @code
    {
	    NVTMPP_MODULE  module;
		NVTMPP_VB_BLK  blk = g_blk;
	    CHAR           module_str[sizeof(NVTMPP_MODULE) + 1];

		sscanf_s(strCmd, "%x", &blk);
		module = nvtmpp_vb_get_block_lastref_module(blk);
		if (0 == module) {
			DBG_ERR("get lastref_module err, blk=0x%x\r\n", blk);
		}
		nvtmpp_vb_module_to_string(module, module_str, sizeof(module_str));
		DBG_DUMP("module_str = %s\r\n",module_str);
		return TRUE;
    }
    @endcode

*/
NVTMPP_MODULE  nvtmpp_vb_get_block_lastref_module(NVTMPP_VB_BLK blk);


/**
    Get remain maximum free block size.

    @param ddr: DDR1 or DDR2.

    @return the remain maximum free block size.

    Example:
    @code
    {
	    static NVTMPP_VB_POOL g_vb_pool;
	    NVTMPP_VB_BLK  blk;
		UINT32         blk_size = nvtmpp_vb_get_max_free_size(NVTMPP_DDR_1);

		g_vb_pool = nvtmpp_vb_create_pool("Cap", blk_size, 1, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_POOL == g_vb_pool) {
			DBG_ERR("create private pool err\r\n");
			return;
		}
		blk = nvtmpp_vb_get_block(0, g_vb_pool, blk_size, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_BLK == blk) {
			DBG_ERR("get vb block err\r\n");
			return;
		}
	}
    @endcode
*/

UINT32         nvtmpp_vb_get_max_free_size(NVTMPP_DDR ddr);

/**
    Dump pools and blocks status to uart output.

    @param dump: the dump function name.

    @return void

    Example:
    @code
    {
		nvtmpp_dump_status(debug_msg);
    }
    @endcode
*/
void           nvtmpp_dump_status(void (*dump)(char *fmt, ...));

/**
    Dump all the memory blocks by sequence.

    @param dump: the dump function name.

    @return void

    Example:
    @code
    {
		nvtmpp_dump_mem_range(debug_msg);
    }
    @endcode
*/
void           nvtmpp_dump_mem_range(void (*dump)(char *fmt, ...));


#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTIPC_H  */
