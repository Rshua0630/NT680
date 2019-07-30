/**
    Execute command line function

    User can plug-in a serial commands into shell system.

    @file       SxCmd.h
    @ingroup    mISYSDebug
    @code
    Syntax:

    SX_CMD_BEGIN(module)
    SX_CMD_ITEM("<action>", func1)      //no parameter
    SX_CMD_ITEM("<action> %", func2)    //have parameters => add " %" at tail
    SX_CMD_END()

    extern SX_CMD_ENTRY Cmd_module[];

    SxCmd_AddTable(Cmd_module);
    @endcode

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _GXCMD_H
#define _GXCMD_H

#include "Type.h"
#include "Debug.h"

/**
     Command service install API.

     Befor use the shell command system, user need to install this function at OS_Main().
     @return Description of data returned.
         - @b E_OK:  Success
*/
extern void SxCmd_InstallID(void) _SECTION(".kercfg_text");


typedef BOOL (*SX_CMD)(CHAR *strCmd); ///< Command handler function prototype


#define SX_MODE_MAX_CNT                 8

/**
    Command interface of different cmd mode

    SxCmd could provide many command systems (cmd mode).
    User can registered new command system by this API.

    @param[in] id Mode id, default value = 0, maximumvalue = SX_MODE_MAX_CNT-1.
    @param[in] strModeName Mode name, maximum length is 8.
    @param[in] cPrompt Prompt character for change mode.
    @param[in] fpCmdCB Cmd handler function callback.
*/
extern void SxCmd_RegCmdMode(UINT32 id, CHAR *strModeName, CHAR cPrompt, SX_CMD fpCmdCB);


/**
     Command entry

     The command system registering structure. After the entry is resgistered, the
     module command could be used.

     @note Do not declare variable by this structure directly. User have to use SX_CMD_???
     macros to create user own command entry.
*/
typedef struct _SX_CMD_ENTRY {
	CHAR    *pName; ///< command's module name
	SX_CMD  pfFunc; ///< command table
	CHAR    *pDesc;
}
SX_CMD_ENTRY;

/**
     @name Create a command table
     @code
        static BOOL Cmd_A(CHAR* strCmd)
        {
            //do something
            return TRUE;
        }

        static BOOL Cmd_B(CHAR* strCmd)
        {
            //do something,
            //if use % add tail at SX_CMD_ITEM, user can parse strCmd to get parameters
            sscanf(strCmd,"%d %d\r\n",&par1,&par2);
            return TRUE;
        }

        SXCMD_BEGIN(module_name,"module description")
        SXCMD_ITEM("a", Cmd_A, "explain command a")
        SXCMD_ITEM("b %", Cmd_B, "explain command b")
        SXCMD_END()

        SX_CMD_ENTRY* Cmd_GetCmdEntry(void)
        {
            return (SX_CMD_ENTRY*)module_name;
        }
     @endcode
     then user can use below to register this table.
     @code
        SxCmd_AddTable(Cmd_GetCmdEntry());
     @endcode
*/
//@{
#define SXCMD_BEGIN(tbl,desc) _SECTION(".kercfg_data") SX_CMD_ENTRY (tbl)[]={{#tbl,0,desc}, ///< begin a command table
#define SXCMD_ITEM(cmd,func,desc)  {(cmd), (func), (desc)}, ///< insert a command item in command table
#define SXCMD_END()    {0,0,NULL}}; ///< end a command table
//@}

/**
     Start command service

     Use this API to start the service
     @return
         - @b E_OK:  Success
         - @b E_SYS:  SxCmd is already open
*/
extern ER SxCmd_Open(void);

/**
     Close command service

     Use this API to close the service
     @return
         - @b E_OK:  Success
         - @b E_SYS:  SxCmd is already close
*/
extern ER SxCmd_Close(void);

/**
     Add command table

     Use this API to plug-in a command entry

     @note this table must place at global
     @param[in] pTable command entry.
     @return
         - @b E_OK:  Success
         - @b E_SYS:  SxCmd is already close
*/
extern INT32 SxCmd_AddTable(SX_CMD_ENTRY *pTable);

/**
     Run one command

     After SxCmd opened and registered table, Command Task can use this API to run
     a command. Also, user can run this API to do something directly in his
     function implementing for debug.

     @param[in] pcCmdStr command string.

     @return
         - @b TRUE:  handled this command.
         - @b FALSE: may not handled command
*/
extern BOOL SxCmd_DoCommand(CHAR *pcCmdStr);

/**
    Register a get temp memory

    Some command may need use a temp buffer, user can use the function to register
    a function pointer the command get temp memory by.

    @param[in] pfuncGetTempBuffer get temp buffer address and buffer size function
    pointer.
*/
extern void SxCmd_RegTempMemFunc(UINT32(*pfuncGetTempBuffer)(UINT32 uiSize));

/**
    Register a release temp memory

    Some command may need use a temp buffer, user can use the function to register
    a function pointer the command release temp memory by.

    @param[in] pfFuncRelTempBuffer release temp buffer address
    pointer.
*/
extern void SxCmd_RegRelTempMemFunc(UINT32(*pfFuncRelTempBuffer)(UINT32 addr));

/**
    Release temp memory

    Some command implementation may need use a temp buffer, user use this API to
    release temp buffer.

    @note The project users need to register their own get memory body code, and
    set it to SxCmd service via SxCmd_RegRelTempMemFunc.

    @param[in] addr the address you want to release.
*/
extern UINT32 SxCmd_RelTempMem(UINT32 addr);

#define TEMPBUF_GET_MAXSIZE 0xffffffff

/**
    Get temp memory

    Some command implementation may need use a temp buffer, user use this API to
    get temp buffer.

    @note The project users need to register their own get memory body code, and
    set it to SxCmd service via SxCmd_RegTempMemFunc.

    @param[in] uiSize the buffer size you need.
    pointer.
*/
extern UINT32 SxCmd_GetTempMem(UINT32 uiSize);

/**
    Command hook

    If hook function is registered, any command will call this function first before
    execute the command. User can modify command string in hook function, and then the
    string will be pass to next.

    @param[in] fpCmdFilterCB function pointer of user implemented hook function.
*/
extern void SxCmd_RegCmdFilterCB(BOOL (*fpCmdFilterCB)(CHAR *pcCmdStr, int len));  //filter inside SxCmd_DoCommand()


#endif //_GXCMD_H

