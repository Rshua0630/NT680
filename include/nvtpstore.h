/**

    This header file of nvtpstore APIs.

    This header file of nvtpstore APIs for user space applications using.
   Applications can use the APIs to open/close/read/write pstore partition defined in CORE1.

    @file       nvtipc.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

*/

#ifndef _PSTORE_H
#define _PSTORE_H
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#define PS_SEC_NAME_MAX_LEN        12      ///<Pstore setcion name maximun length.

/**
    @name PStore error code.
*/
//@{
#define E_PS_OK             (0)             ///< PStore operation correct
#define E_PS_PAR            (-33)           ///< Input parameter error (E_PAR)
#define E_PS_SECHDLER       ((INT32)NULL)   ///< Section handle error,NULL
#define E_PS_SYS            (-5)            ///< System error (E_SYS)
#define E_PS_CHKER          (-100)          ///< Header of PStore checksum error
#define E_PS_EMPTY          (-101)          ///< PStore is empty.All data is 0xFF in storage
#define E_PS_VERFAIL        (-102)          ///< The version mismatch
#define E_PS_SIZE           (-103)          ///< Input size error
#define E_PS_DRVFAIL        (-104)          ///< Device driver init fail
#define E_PS_PSNOTOPEN      (-105)          ///< PStore has not opened yet
#define E_PS_PSOPEN         (-106)          ///< PStore has opened
#define E_PS_SECNOTOPEN     (-107)          ///< Section has not opened yet
#define E_PS_RDONLY         (-108)          ///< PStore is opened as read only
#define E_PS_WRONLY         (-109)          ///< PStore is opened as write only
#define E_PS_SECNOTFOUND    (-110)          ///< Section not found
#define E_PS_SECNOTCLOSE    (-111)          ///< Section not close
#define E_PS_GETRIGHTFAIL   (-112)          ///< Get access right fail
#define E_PS_NOSTRGOBJ      (-113)          ///< Storage object not found
#define E_PS_NAMEFAIL       (-114)          ///< PStore Name Error(out of 'A'~'Z')
#define E_PS_HEADFAIL       (-115)          ///< PStore header error
#define E_PS_OPFAIL         (-116)          ///< PStore operation fail
#define E_PS_BUF            (-117)          ///< PStore buffer error
#define E_PS_SEC_NUM        (-118)          ///< PStore section number error,out of range (PS_MIN_SEC_NUM ~ PS_MAX_SEC_NUM)
#define E_PS_TBL            (-119)          ///< PStore block table not correct
#define E_PS_PARTITION      (-120)          ///< Nand partition error,maybe change partition
//@}

/**
    @name PStore read / write operation attribute.
    These bits can be OR'd.
*/
//@{
#define PS_RDONLY           0x1                       ///<Read only
#define PS_WRONLY           0x2                       ///<Write only
#define PS_RDWR             (PS_RDONLY|PS_WRONLY)     ///<Read and write
#define PS_CREATE           0x4                       ///<Create a new section when the section not existence.  Can be OR'd with read and write flag.
#define PS_UPDATE           0x8                       ///<Update exist section data,not change section size and header table
//@}



/**
    Open a PStore section.

    The API opens the section that is specified by section name.

    @note
    A valid section name consist of four characters or less, but can not be NULL or 0xFFFFFFFF.

    @param[in]      pSecName           Section name.
    @param[in]      rwMode             Read / write mode.
                                                            PS_RDONLY Read only
                                                            PS_WRONLY Write only
                                                            PS_RDWD   Read and write
                                                            PS_CREATE Create a new section when the section not existence.
                                                            PS_UPDATE update exist section data,not change section size and header table

    @return      Section Handle      The pstore section handle.
                                     - @b <=0 for open section fail.
                                     (ex:Section not found,operation error,check name error,section full,check sum error.)



    Example:
    @code
    {
        int  psHandle;
        char dataW[11]= "1234567890";
        char dataR[11];
        int  datalen = 10;
        psHandle=nvtpstore_open("ps1",PS_CREATE|PS_RDWR);
        if (psHandle <=0)
        {
            DBG_ERR("open pstore error\n");
            return;
        }
        lseek(psHandle, 0, SEEK_SET);
        write(psHandle,dataW,datalen);
        lseek(psHandle, 0, SEEK_SET);
        read(psHandle,dataR,datalen);
        nvtpstore_close(psHandle);

    }
    @endcode

*/
extern int nvtpstore_open(char *pSecName,int rwMode);

/**
    Close a PStore section.

    The API closes opened handle.

    @param[in]      psHdl  The section handle.

    @return
        - @b E_PS_OK if success
        - @b E_PS_PAR if wrong parameter
        - @b E_PS_CHKER if check sum error
        - @b E_PS_SYS if write block error
*/
extern int nvtpstore_close(int psHdl);

/**
    Remove a PStore section.

    Remove a PStore section.

    @param[in]      pSecName           Section name.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if delete fail
        - @b E_PS_SECNOTCLOSE if section opened,please close section first.
        - @b E_PS_SECNOTFOUND if assigned section name not found
        - @b E_PS_PAR if wrong parameter

    Example:
    @code
    {
        nvtpstore_remove("ps1");
    }
    @endcode
*/
extern int nvtpstore_remove(char *pSecName);
#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _PSTORE_H  */
