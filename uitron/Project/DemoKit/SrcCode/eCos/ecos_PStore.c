//eCos header
#include <cyg/fs/pstorefs.h>
#include <dirent.h> //for listdir
#include <fcntl.h>

//uItron header
#include "ecos_PStore.h"
#include "PStore.h"

//debug header and flag
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          eCos_PStore
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


//private function prototype
static char*   cyg_PStore_GetVerInfo(void);
static char*   cyg_PStore_GetBuildDate(void);

static int     cyg_PStore_OpenSection( const char *pSecName, int oflag, int* iSecSize);//open
static int     cyg_PStore_CloseSection( int fd );//close
static int     cyg_PStore_ReadSection( int fd, void *buf, ssize_t len, int offset );//read
static int     cyg_PStore_WriteSection( int fd, const void *buf, ssize_t len, int offset );//write
static int     cyg_PStore_StatSection( int fd, struct stat *pStat );//fstat
static int     cyg_PStore_DeleteSection( const char *pSecName);//unlink

static int     cyg_PStore_Stat( const char *pSecName, struct stat *pStat);//stat

static bool    cyg_PStore_SearchSectionOpen(void);//opendir
static bool    cyg_PStore_SearchSection( char *pName, ssize_t len);//readdir
static bool    cyg_PStore_SearchSectionRewind(void);//rewinddir
static int     cyg_PStore_SearchSectionClose(void);//closedir


//global variable
static PStore_CBS g_PStore_CBS = {  &cyg_PStore_GetVerInfo,
                                    &cyg_PStore_GetBuildDate,
                                    &cyg_PStore_OpenSection,
                                    &cyg_PStore_CloseSection,
                                    &cyg_PStore_ReadSection,
                                    &cyg_PStore_WriteSection,
                                    &cyg_PStore_StatSection,
                                    &cyg_PStore_DeleteSection,
                                    &cyg_PStore_Stat,
                                    &cyg_PStore_SearchSectionOpen,
                                    &cyg_PStore_SearchSection,
                                    &cyg_PStore_SearchSectionRewind,
                                    &cyg_PStore_SearchSectionClose,
                                   };

//function definition
void listsection( char *name)
{
    int err;
    DIR *dirp;
    int num=0;

    DBG_DUMP("^M<INFO>: reading directory %s\r\n",name);

    dirp = opendir( name );
    if( dirp == NULL ) DBG_ERR("dirp == NULL\r\n");

    for(;;)
    {
        struct dirent *entry = readdir( dirp );

        if( entry == NULL )
            break;
        num++;
        DBG_DUMP("^M<INFO>: entry %14s\r\n",entry->d_name);
    }

    err = closedir( dirp );
    if( err < 0 ) DBG_ERR("closedir failed\r\n");
}

void cyg_PStore_RegisterCB(void)
{
    PStoreECOS_RegisterCB(&g_PStore_CBS);
}

static char* cyg_PStore_GetVerInfo(void)
{
    return NULL;
}

static char* cyg_PStore_GetBuildDate(void)
{
    return NULL;
}

//bIsRoot is used to tell whether the path is relative path or absolute path
static int cyg_PStore_OpenSection( const char *pSecName, int oflag, int* pSecSize)
{
    PSTORE_SECTION_HANDLE* hSection;

    if(oflag & O_CREAT)
    {
        DBG_IND("transfer eCos O_CREAT to PStore PS_CREATE\r\n");
        oflag = (oflag & (~O_CREAT)) | PS_CREATE;
    }

    DBG_IND("pSecName = %s, oflag = %d\r\n", pSecName, oflag);

    hSection = PStore_OpenSection((char *)pSecName, oflag);

    if(E_PS_SECHDLER != (INT32)hSection)//success
    {
        DBG_IND("hSection = 0x%X\r\n", hSection);//handle
        *pSecSize = hSection->uiSectionSize;
    }
    else//fail
    {
        DBG_ERR("hSection = E_PS_SECHDLER\r\n");//NULL handle
        *pSecSize = 0;
    }

    return (int)hSection;
}

static int cyg_PStore_CloseSection( int fd )
{
    INT32 iRet;

    DBG_IND("Close handle = 0x%X\r\n", fd);

    iRet = PStore_CloseSection((PSTORE_SECTION_HANDLE *)fd);

    if(E_PS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_PStore_ReadSection( int fd, void *buf, ssize_t len, int offset )
{
    INT32 iRet;

    DBG_IND("fd = 0x%X, buf = 0x%X, len = %d, offset = %d\r\n", fd, buf, len, offset);

    iRet = PStore_ReadSection((UINT8 *)buf, offset, len, (PSTORE_SECTION_HANDLE *)fd);

    if(E_PS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_PStore_WriteSection( int fd, const void *buf, ssize_t len, int offset )
{
    INT32 iRet;

    DBG_IND("fd = 0x%X, buf = 0x%X, len = %d, offset = %d\r\n", fd, buf, len, offset);

    iRet = PStore_WriteSection((UINT8 *)buf, offset, len, (PSTORE_SECTION_HANDLE *)fd);

    if(E_PS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_PStore_StatSection( int fd, struct stat *pStat)
{
    PSTORE_SECTION_HANDLE* hSection;

    DBG_IND("fd = 0x%X, pStat = 0x%X\r\n", fd, pStat);

    if(0 == fd)
        return E_PS_PAR;

    hSection = (PSTORE_SECTION_HANDLE *)fd;

    pStat->st_mode  = __stat_mode_REG;//a regular file
    pStat->st_size  = hSection->uiSectionSize;
    pStat->st_atime = 0;
    pStat->st_mtime = 0;
    pStat->st_ctime = 0;

    return E_PS_OK;
}

static int cyg_PStore_DeleteSection( const char *pSecName)
{
    INT32   iRet;

    DBG_IND("pSecName = %s\r\n", pSecName);

    iRet = PStore_DeleteSection((char *)pSecName);

    if(E_PS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static int cyg_PStore_Stat( const char *pSecName, struct stat *pStat)
{
    PSTORE_SECTION_HANDLE* hSection;
    INT32           iRet;

    DBG_IND("pSecName = %s, pStat = 0x%X\r\n", pSecName, pStat);

    hSection = PStore_OpenSection((char *)pSecName, PS_RDONLY);
    if(E_PS_SECHDLER == (INT32)hSection)
    {
        DBG_ERR("Section not opened\r\n");
        return E_PS_SECNOTOPEN;
    }

    pStat->st_mode  = __stat_mode_REG;//a regular file
    pStat->st_size  = hSection->uiSectionSize;
    pStat->st_atime = 0;
    pStat->st_mtime = 0;
    pStat->st_ctime = 0;

    iRet = PStore_CloseSection(hSection);
    if(E_PS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}

static bool cyg_PStore_SearchSectionOpen(void)
{
    PSTORE_SEARCH_HANDLE SearchHandle;
    BOOL    bIsOK;

    //call PStore_SearchSectionOpen() but return nothing.
    //this operation will cause PStore to lock in search operation
    bIsOK = PStore_SearchSectionOpen(&SearchHandle);
    PStore_SearchSectionRewind();

    if(bIsOK)//success
        DBG_IND("bIsOK = %d\r\n", bIsOK);
    else//fail
        DBG_ERR("bIsOK = %d\r\n", bIsOK);

    return bIsOK;
}

static bool cyg_PStore_SearchSection( char *pName, ssize_t req_len)
{
    PSTORE_SEARCH_HANDLE SearchHandle = {0};
    UINT32  got_len;
    BOOL    bIsOK;

    bIsOK = PStore_SearchSection(&SearchHandle);

    if(bIsOK)//success
    {
        DBG_IND("bIsOK = %d\r\n", bIsOK);
    }
    else//fail
    {
        DBG_IND("bIsOK = %d\r\n", bIsOK);
        *(char *)pName = '\0';
        return FALSE;
    }

    if (req_len < 2) { //at least one character and one NULL byte
        DBG_ERR("req_len < 2\r\n");
        *(char *)pName = '\0';
        return FALSE;
    }

    got_len = strlen(SearchHandle.pSectionName);
    DBG_IND("pSectionName = %s, got_len = %d\r\n", SearchHandle.pSectionName, got_len);

    if(got_len > (UINT32)(req_len-1))
    {
        DBG_WRN("got_len (%d) > req_len (%d) - 1\r\n", got_len, req_len);
        SearchHandle.pSectionName[req_len-1] = '\0';//set to the smaller requested len
    }

    strcpy((char*)pName, SearchHandle.pSectionName);

    return bIsOK;
}

static bool cyg_PStore_SearchSectionRewind(void)
{
    BOOL    bIsOK;

    bIsOK = PStore_SearchSectionRewind();

    if(bIsOK)//success
        DBG_IND("bIsOK = %d\r\n", bIsOK);
    else//fail
        DBG_ERR("bIsOK = %d\r\n", bIsOK);

    return bIsOK;
}

static int cyg_PStore_SearchSectionClose(void)
{
    INT32 iRet;

    iRet = PStore_SearchSectionClose();

    if(E_PS_OK == iRet)//success
        DBG_IND("iRet = %d\r\n", iRet);
    else//fail
        DBG_ERR("iRet = %d\r\n", iRet);

    return iRet;
}
