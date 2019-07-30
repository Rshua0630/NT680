#ifndef _IPL_INI_H_
#define _IPL_INI_H_
/**
    data ini module

    ini data management on Dram

    @file       ipl_ini.h
    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "Type.h"

#define IPL_INI_STS_OK          (0)
#define IPL_INI_STS_ERR         (1)
#define IPL_INI_STS_HTAG_ERR    (2)
#define IPL_INI_STS_HSTS_ERR    (3)
#define IPL_INI_STS_HFULL_ERR   (4)
#define IPL_INI_STS_MEMFULL_ERR (5)
#define IPL_INI_STS_NULL_SECTION (6)
#define IPL_INI_STS_NULL_KEY 	(7)


typedef struct _IPL_INI_KEY_LIST {
	UINT32 key_addr;
	UINT32 key_size;
	UINT32 val_addr;
	UINT32 val_size;
	UINT32 rev[2];
	const struct _IPL_INI_KEY_LIST *pnext;
} IPL_INI_KEY_LIST;

/**
    ipl ini open

    @note open rw control flow

    @param[in] work_buf_addr    working buffer address
    @param[in] work_buf_size    working buffer size(header size + org_data_size + extra buf size)
                                header size:    get from ipl_ini_get_header_buf_size()
                                org_data_size:  old data need to be modify.
                                                put old data at work_buf_addr
                                extra buf size: append data buffer
    @param[in] org_data_size    original data size (without original data, org_data_size set to 0)

    @return return control handle.
    - @b 0: fail
*/
UINT32 ipl_ini_open(UINT32 work_buf_addr, UINT32 work_buf_size, UINT32 org_data_size);

/**
    ipl ini get data

    @note get data buffer

    @param[in] handle   rw contorl handle
    @param[out] addr    data buffer address
    @param[out] size    data buffer size

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header)
*/
UINT32 ipl_ini_get_data(UINT32 handle, UINT32 *addr, UINT32 *size);

/**
    ipl ini get key

    @note get key value

    @param[in] handle       rw contorl handle
    @param[in] sec_name     section name
    @param[in] key_name     key name
    @param[out] val_adr     key value addr
    @param[out] length      key value length

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header)
*/
UINT32 ipl_ini_get_key(UINT32 handle, CHAR *sec_name, CHAR *key_name, const CHAR **val_adr, UINT32 *length);

/**
    ipl ini get key list

    @note get key list

    @param[in] handle       rw contorl handle
    @param[in] sec_name     section name
    @param[out] key_list     key list

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header)
*/
UINT32 ipl_ini_get_key_list(UINT32 handle, CHAR *sec_name, const IPL_INI_KEY_LIST **key_list);
/**
    ipl ini set key

    @note set key value, key is exist => over write old value, else add one

    @param[in] handle       rw contorl handle
    @param[in] sec_name     section name
    @param[in] key_name     key name
    @param[in] value        key value

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header or section/key does not exist)
*/
UINT32 ipl_ini_set_key(UINT32 handle, CHAR *sec_name, CHAR *key_name, CHAR *value);

/**
    ipl ini del key

    @note del key

    @param[in] handle       rw contorl handle
    @param[in] sec_name     section name
    @param[in] key_name     key name

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header or section/key does not exist)
*/
UINT32 ipl_ini_del_key(UINT32 handle, CHAR *sec_name, CHAR *key_name);

/**
    ipl ini get section

    @note get section

    @param[in] handle       rw contorl handle
    @param[in] sec_name     section name
    @param[out] val_adr     section value addr
    @param[out] length      section value length

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header or section does not exist)
*/
UINT32 ipl_ini_get_section(UINT32 handle, CHAR *sec_name, const CHAR **val_adr, UINT32 *length);

/**
    ipl ini del section

    @note del section

    @param[in] handle       rw contorl handle
    @param[in] sec_name     section name

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header or section does not exist)
*/
UINT32 ipl_ini_del_section(UINT32 handle, CHAR *sec_name);

/**
    ipl ini get header buffer size

    @note get header buffer size

    @return return header buffer size.
*/
UINT32 ipl_ini_get_header_buf_size(void);
/**
    ipl ini close

    @note close rw control flow

    @param[in] handle   rw contorl handle

    @return return status.
    - @b IPL_INI_STS_OK: OK
    - @b IPL_INI_STS_ERR: NG (invalid header)
    - @b IPL_INI_STS_HTAG_ERR: NG(check header tag fail)
*/
UINT32 ipl_ini_close(UINT32 handle);



/**
    ipl ini dump information

    @note dump handle information

    @param[in] handle rw contorl handle

*/
void ipl_ini_dump(UINT32 handle);
#endif
