/**
    PStore driver export variables and function prototypes.

    PStore is novatek original simple file system.
    For access permanent storage in embeded system.

    @file       PStore.h
    @ingroup    mIPStore

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#ifndef _PSTORE_H
#define _PSTORE_H

#include "Type.h"
#include "DxType.h"
/**
     @addtogroup mIPStore
*/

/**
    PStore register kernel resource API;user would intall before open PStore
*/
extern void PStore_InstallID(void) _SECTION(".kercfg_text");

/**
    Register PStore cmd table to system.

    @return void.
*/
extern void PStore_InstallCmd(void);


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
    @name PStore header identification.
    Used for check section return value or restore header parameter

*/
//@{
#define PS_HEAD1    0x1     ///<PStore header1(restore header1 or header1 error)
#define PS_HEAD2    0x2     ///<PStore header2(restore header2 or header2 error)
//@}

/**
    @name PStore total section range.
*/
//@{
#define PS_MIN_SEC_NUM      4      ///<Minimum section number
#define PS_MAX_SEC_NUM      256     ///<Maximum section number
//@}

/**
    @name The range of PStore block number per section.
*/
//@{
#define PS_MIN_BLK_NUM_PER_SEC      128     ///<Minimum block per section
#define PS_MAX_BLK_NUM_PER_SEC      512     ///<Minimum block per section
//@}

#define SEC_NAME_LEN                12      ///<Setcion name maximun length.

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
#define E_PS_INIT           (-121)          ///< PStore init fail
//@}


/**
    PStore header structure.
*/
typedef struct _PSTORE
{
    UINT32 uiSignature;             ///<Just for identification.
    UINT32 uiPSVersion;             ///<Version of PStore library.
    UINT32 uiSectionNum;            ///<How many sections the PStore have.
    UINT32 uiMaxBlkPerSec;          ///<Max block per section.
    UINT32 uiPStoreSize;            ///<PStore size in byte unit (for NAND flash device, please make sure the PStoreSize that a multiple of block size).
    UINT32 uiPSDrvVersion;          ///<Version of stroage driver.
    UINT32 uiCheckSum;              ///<Please keep check sum item is the last element of PS structure.
} PSTORE, *PPSTORE;


/**
    PStore format structure.User can decide total section number and how many block per section
*/
typedef struct _PSFMT
{
    UINT32 uiMaxBlkPerSec;       ///<block number per section.This value would decice Maximun size of one section.
    UINT32 uiMaxSec;             ///<total section number
} PSFMT, *PPSFMTE;

/**
    PStore init pramater
*/
typedef struct _PSTORE_INIT_PARAM
{
    UINT8 *pBuf;                ///<PStoer working buffer address
    UINT32 uiBufSize;           ///<PStoer working buffer size.PStore_CaculateBuffer() can get minimum buffer size
} PSTORE_INIT_PARAM, *PPSTORE_INIT_PARAM;

/**
    Section handle structure.
*/
typedef struct _PSTORE_SECTION_HANDLE
{
    char pSectionName[SEC_NAME_LEN];   ///<uiSectionName =0, means a free section handle.
    UINT32 uiSectionID;                 ///<Section sequence in header.
    UINT32 uiRWOperation;               ///<Bit flag of PS_RDONLY, PS_WRONLY, PS_RDWR, PS_CREATE ,PS_UPDATE
    UINT32 uiSectionUpdate;             ///<Flag of update header or not.
    UINT32 uiSectionSize;               ///<Section size.Byte unit.(not block alignment)
    UINT32 uiHandleID;                  ///<Handle identification.
} PSTORE_SECTION_HANDLE, *PPSTORE_SECTION_HANDLE;


/**
    Found section handle structure.
*/
typedef struct _PSTORE_SEARCH_HANDLE
{
    char pSectionName[SEC_NAME_LEN];    ///<Section name ID
    UINT32 uiSectionSize;               ///<Section size
    UINT32 uiReadOnly;                  ///<Section read only
    UINT32 uiDescCheckSum;              ///<check sum of descriptor
} PSTORE_SEARCH_HANDLE, *PPSTORE_SEARCH_HANDLE;

/**
    The parameter of PStore_GetInfo()
*/
typedef enum _PS_INFO_ID_
{
    PS_INFO_MAX_SEC = 0,    ///<PStore max section ,set from PStore_Format
    PS_INFO_MAX_SEC_SIZE,   ///<A section max size(block size * max block per section),set from PStore_Format
    PS_INFO_TOT_SIZE,       ///<PStore size,get from storage partitioned physical size
    PS_INFO_FREE_SPACE,     ///<PStore free space(block alignment)
    PS_INFO_BLK_SIZE,       ///<Storage block size
    ENUM_DUMMY4WORD(PS_INFO_ID)
} PS_INFO_ID;

/**
    The parameter of PStore_EnableUpdateHeader()
*/
typedef enum _PS_HEAD_UPDATE_
{
    PS_UPDATE_ALLHEAD = 1,   ///<head 1 and head 2 would update when create and write section
    PS_UPDATE_ONEHEAD,       ///<only update header 1,header 2 is read only
    PS_UPDATE_ZEROHEAD,      ///<header 1 and header 2 are read only
    ENUM_DUMMY4WORD(PS_HEAD_UPDATE)
} PS_HEAD_UPDATE;


typedef enum _PS_TYPE
{
    PS_TYPE_EMBEDED = 0,
    PS_TYPE_FILESYS,
    ENUM_DUMMY4WORD(PS_TYPE)
} PS_TYPE;


/**
    Calculate PStore buffer size

    The fomula as below, \n
    1.one block for read/write not block alignment access \n
    2.keep tmp 2 section block table + 1 cTempHeadBuf  = 3 PS_SECTION_HEADER_SIZE \n
    3.all section descption \n
    4.buffer check byte:4 \n

    @param[in] pFmtStruct       Format structure for caculate block table size.
    @param[in] blkSize          Storage block size.

    @return    Buffer size.
*/
extern UINT32 PStore_CalBuffer(PSFMT *pFmtStruct,UINT32 blkSize);

/**
    init PStore type

    init PStore type,user can choose PS_TYPE type
    @param[in] PsType       PStore supported type (embeded or file system ).

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail

*/
extern ER PStore_Init(PS_TYPE PsType,CHAR Drive);

/**
    Open PStore.

    This API would initialize storage driver,working buffer and global parmater.
    Need to open PStore before access PStore.

    @param[in] handle      The pointer of storage handle (contain storage object).

    @param[in] pInitParam  The pointer of working buffer address and size.

    @return
        - @b E_PS_OK if success
        - @b E_PS_PAR if the input parameters are wrong
        - @b E_PS_SYS if some system settings did not be set, such as PStore_InstallID
        - @b E_PS_CHKER if checksum error
        - @b E_PS_EMPTY if no PStore header.All haeder data is 0xFF
        - @b E_PS_VERFAIL if library or driver version is different from verion in storage
        - @b E_PS_DRVFAIL if init storge driver fail
        - @b E_PS_GETRIGHTFAIL if get access right fail.(last time use PStore_GetAccessRight but not release)
        - @b E_PS_HEADFAIL if found PStore header error
        - @b E_PS_SEC_NUM if section number out of range (PS_MIN_SEC_NUM ~ PS_MAX_SEC_NUM)
        - @b E_PS_PARTITION if NAND partition error
*/
extern ER     PStore_Open(DX_HANDLE handle,PSTORE_INIT_PARAM* pInitParam);

/**
    Close PStore.

    This API will check whether there is an opened handle ,so close the section and release handle

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
        - @b E_PS_CHKER if check sum error
*/
extern ER     PStore_Close(void);

/**
    Open a PStore section.

    The API opens the section that is specified by section name.

    @note
    A valid section name consist of four characters or less, but can not be NULL or 0xFFFFFFFF.

    @param[in]      pSecName           Section name.
    @param[in]      RWOperation         Read / write operation.
                                                            PS_RDONLY Read only
                                                            PS_WRONLY Write only
                                                            PS_RDWD   Read and write
                                                            PS_CREATE Create a new section when the section not existence.
                                                            PS_UPDATE update exist section data,not change section size and header table

    @return      Section Handle      The pointer of section handle.
                                     - @b E_PS_SECHDLER for open section fail.
                                     (ex:Section not found,operation error,check name error,section full,check sum error.)
                                     If section opened,This API would wait until the section closed.

*/
extern PSTORE_SECTION_HANDLE* PStore_OpenSection(char *pSecName, UINT32 RWOperation);

/**
    Close a PStore section.

    The API closes opened handle

    @param[in]      pSectionHandle  The Pointer to section handle.

    @return
        - @b E_PS_OK if success
        - @b E_PS_PAR if wrong parameter
        - @b E_PS_CHKER if check sum error
        - @b E_PS_SYS if write block error
*/
extern ER     PStore_CloseSection(PSTORE_SECTION_HANDLE* pSectionHandle);

/**
    Delete a PStore section.

    Delete a PStore section.

    @param[in]      pSecName           Section name.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if delete fail
        - @b E_PS_SECNOTCLOSE if section opened,please close section first.
        - @b E_PS_SECNOTFOUND if assigned section name not found
        - @b E_PS_PAR if wrong parameter
*/
extern ER     PStore_DeleteSection(char *pSecName);

/**
    Read data from storage of PStore area.

    Read data from storage of PStore area to buffer.

    @param[out] pcBuffer        The read-in data.
    @param[in]  ulStartAddress  Data to read from.
    @param[in]  ulDataLength    How many bytes to read.
    @param[in]  pSectionHandle  The pointer of section handle

    @return
        - @b E_PS_OK if success
        - @b E_PS_PSNOTOPEN if PStore has not opened yet.
        - @b E_PS_PAR if wrong parameter
        - @b E_PS_SECNOTFOUND if section not found.
        - @b E_PS_SECNOTOPEN if the section has not opened yet.
        - @b E_PS_WRONLY if write only
        - @b E_PS_SYS if system error


    Example:
    @code
    {
        if ((pSection = PStore_OpenSection(PS_SEC_NAME, PS_RDWR)) != E_PS_SECHDLER)
        {
            PStore_ReadSection((UINT8 *)pBuf, 0, secSize, pSection);
            PStore_CloseSection(pSection);
        }
    }
    @endcode
*/
extern ER     PStore_ReadSection( UINT8* pcBuffer,UINT32 ulStartAddress, UINT32 ulDataLength, PSTORE_SECTION_HANDLE* pSectionHandle);

/**
    Write data to storage of PStore area.

    Write data to storage of PStore area.

    @param[in] pcBuffer        The pointer to data to be written.
    @param[in] ulStartAddress  Start position of data.
    @param[in] ulDataLength    How many bytes to write.
    @param[in] pSectionHandle  The pointer of section handle.

    @return
        - @b E_PS_OK if success
        - @b E_PS_PSNOTOPEN if PStore has not opened yet.
        - @b E_PS_PAR if wrong parameter
        - @b E_PS_SECNOTFOUND if section not found.
        - @b E_PS_SECNOTOPEN if the section has not opened yet.
        - @b E_PS_RDONLY if read only
        - @b E_PS_SYS if system error

    Example:
    @code
    {
        if ((pSection = PStore_OpenSection(PS_SEC_NAME, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
        {
            PStore_WriteSection((UINT8 *)pBuf, 0, secSize, pSection);
            PStore_CloseSection(pSection);
        }
    }
    @endcode
*/
extern ER     PStore_WriteSection(UINT8* pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, PSTORE_SECTION_HANDLE* pSectionHandle);

/**
    Format PStore.

    Erase all data in PStore area,then fill PStore header parameter to PStore area.

    @param[in] pFmtStruct    Format structure.Assign block number per section and total block.

    @return
        - @b E_PS_OK if success.
        - @b E_PS_SYS if failure.
        - @b E_PS_PAR if parameter error.
        - @b E_PS_SIZE if buffer size error.


*/
extern ER     PStore_Format(PSFMT *pFmtStruct);

/**
    Get PStore information.

    Get PStore information.

    @param[in] infoId    To specify which information ID.

    @return               Information data.
*/
extern UINT32 PStore_GetInfo(PS_INFO_ID infoId);

/**
    Open for search PStore section.

    Open for search PStore section.

    @param[out] pFindData    return found section handle.

    @return
        - @b TRUE continue to search next by PStore_SearchSection()
        - @b FALSE search to end.Use PStore_SearchSectionClose() to close search

    Example:
    @code
    {
        UINT32 bNext = FALSE;
        PSTORE_SEARCH_HANDLE SearchHandle;

        bNext = PStore_SearchSectionOpen(&SearchHandle);
        while(bNext)
        {
            DBG_DUMP("%15s 0x%08x  %d  0x%08x\r\n", SearchHandle.pSectionName,SearchHandle.uiSectionSize,SearchHandle.uiReadOnly,SearchHandle.uiDescCheckSum);
            bNext = PStore_SearchSection(&SearchHandle);
        }

        PStore_SearchSectionClose();

        return TRUE;
    }
    @endcode
*/
extern BOOL PStore_SearchSectionOpen(PSTORE_SEARCH_HANDLE *pFindData);

/**
    Search PStore section.

    Search PStore section.

    @param[out] pFindData    return found section handle.

    @return
        - @b TRUE continue to search next by PStore_SearchSection()
        - @b FALSE search to end.Use PStore_SearchSectionClose() to close the search.

*/
extern BOOL PStore_SearchSection(PSTORE_SEARCH_HANDLE *pFindData);

/**
    Rewind PStore search section.

    Rewind the PStore search section to the beginning.

    @return
        - @b TRUE Rewind operation succeed.
        - @b FALSE Fail to rewind. Use PStore_SearchSectionClose() to close the search.
*/
extern BOOL PStore_SearchSectionRewind(void);

/**
    Close search PStore section.

    Close search PStore section.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
*/
extern ER PStore_SearchSectionClose(void);

/**
    Dump PSTORE structure and exist PSTORE_SECTION_DESC and PSTORE_SECTION_HANDLE
*/
extern void   PStore_DumpPStoreInfo(void);

/**
    Dump PStore data to file system.

    The dump data including header,block table and section data.

    @param[in] pBuf        Working buffer for dump.
    @param[in] uiBufSize   Working buffer size.

    @return void.

*/
extern void   PStore_Dump(UINT8 *pBuf,UINT32 uiBufSize);

/**
    Check assigned section block table.

    Check PStore block table first block number is empty (0xFF) or invalid value

    @param[in] pSecName           Secton name.
    @param[out] ErrorSection       Return Section Error.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
        - @b E_PS_HEADFAIL if check header error
*/
extern ER     PStore_CheckSection(char *pSecName,UINT32* ErrorSection);

/**
    Check total section block table.

    Check exist PStore block table first block number is empty (0xFF) or invalid value

    @param[out] ErrorSection        Return Section header error.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
        - @b E_PS_HEADFAIL if check header error
*/
extern ER     PStore_CheckTotalSection(UINT32* ErrorSection);

/**
    Enable update header.

    User can decide update which header.
    this API ,PStore_CheckTotalSection and PStore_RestoreHeader can use together.

    @param[in] Header       Update Header type(refer to PS_HEAD_UPDATE).

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail

    Example:
    @code
    {
        PStore_EnableUpdateHeader(PS_UPDATE_ONEHEAD); //only update header 1,header 2 would not be update
        PStore_CheckTotalSection(&ErrorSection);
        if(ErrorSection==PS_HEAD1)
            PStore_RestoreHeader((UINT8 *)pBuf,PS_HEAD1);
    }
    @endcode

*/
extern ER     PStore_EnableUpdateHeader(PS_HEAD_UPDATE Header);

/**
    Restore PStore header.

    Restore from good header to bad header.There are only 2 header,if choose PS_HEAD1,
    it would read header2 data and write to header1

    @param[in]  pBuf         Working buffer.
    @param[in]  RestoreType  Restore header1 or header2.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
        - @b E_PS_PAR if pamameter error

    Example:
    @code
    {
        PStore_CheckTotalSection(&ErrorSection);
        if(ErrorSection==PS_HEAD1)
            PStore_RestoreHeader((UINT8 *)pBuf,PS_HEAD1);
    }
    @endcode

*/
extern ER     PStore_RestoreHeader(UINT8 *pBuf , UINT32 RestoreType);

/**
    Set single section read only.

    Set single section read only.

    @note
    The API would update PStore header,and write to stoarge.
    The attribute would exist after the reboot.

    @param[in] pSecName         Section name.
    @param[in] Enable            TRUE or FALSE.

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
        - @b E_PS_PAR if pamameter error
*/
extern ER     PStore_ReadOnly(char *pSecName, BOOL Enable);

/**
    Set exist section read only

    Set exist section read only.

    @note
    The API would update PStore header,and write to stoarge.
    The attribute would exist after the reboot.

    @param[in] Enable        TRUE or FALSE

    @return
        - @b E_PS_OK if success
        - @b E_PS_SYS if fail
*/
extern ER     PStore_ReadOnlyAll(BOOL Enable);

/**
    Enable/Disable check data after write section.

    If enable,write section would read data to temp buffer and compare with write data evey time.

    @note
    The performance of write section would be affected.

    @param[in]    Enable enable/disable function.

    @return void
*/
extern void   PStore_EnableCheckData(BOOL Enable);

/**
    Check storage version to determine if format is needed.

    If Version is different from the value stored in storage PStore_Open() will fail and project should then call PStore_Format().

    @param[in]    Version project's specific version.

    @return void
*/
extern void   PStore_SetPrjVersion(UINT32 Version);

#endif // _PSTORE_H

//@}
