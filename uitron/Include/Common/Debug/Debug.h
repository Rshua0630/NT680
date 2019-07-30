/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       Debug.h
    @ingroup    mISYSDebug

    @brief      Header file of debug module
                Header file of debug module

*/

/** \addtogroup mISYSDebug */
//@{

#ifndef _DEBUG_H
#define _DEBUG_H

#include "Type.h"
#include <stdarg.h>

/**
     Console object

     console object is an object to plug-in a console input and output
*/
typedef struct _console {
	void (*hook)(void); ///< start this object
	void (*unhook)(void); ///< terminal this object
	void (*puts)(const char *s); ///< console input funciton pointer
	int (*gets)(char *s, int len); ///< console output function point
} console;

/**
     @name Colsole ID
     @note User use debug_???_console to operate the console
*/
//@{
#define CON_0   0 ///< NULL console (built-in)
#define CON_1   1 ///< UART console (built-in)
#define CON_2   2 ///< USB or file console
#define CON_MAX 3 ///< Total ID counts.
//@}

/**
     Console object register

     User use this function to register a console object providing by novatek or implementing by userself

     @param[in] id       console id you want to register.
     @param[out] pConsole    console object
*/
void debug_reg_console(int id, const console *pConsole);

/**
     Console object switcher

     User use this function to register a console object providing by novatek or implementing by userself

     @param[in] id       console id you want to switch
*/
void debug_set_console(int id);

/**
     get current console ID

     Get current console ID in use.

     @return a console ID mapping to CON_? number
*/
int debug_get_console(void);

/**
    Output a debug string

    send it a string appear on console
*/
extern void debug_putstring(const char *str);

/**
    Get an input string from console

    recevie a string from console

    @return real length of return string
*/
extern int debug_getstring(char *str, int maxlen);

extern void debug_msg(char *fmtstr, ...);   ///< show a debug message without any debug level limition
extern int debug_vprintf(const char *fmtstr, va_list marker);  ///< show a debug message with a va_list, without any debug level limition.

//filter
extern void debug_msg_filter(const char *filter, const char *fmtstr, ...); ///< reversed function only used in DebugMoudle.h

extern void debug_dumpmem(UINT32 addr, UINT32 length); ///< dump memory region to console output
extern void debug_dumpmem2file(UINT32 addr, UINT32 length, CHAR *filename); ///< dump memory region to file output


/**
    Show the check point, variable values

    This Macro is only for Debuging,In release Mode shoulde remove it.
    Usage: This Macro must be put in the preceding procedure code

    Example: (Optional)
    @code
    {
    void TestFunction(void)
    {
        INT32 iCmd = 30;

        DBGTAG(0);

        iCmd++;
        DBGD(iCmd);
        CHKPNT;
        DBGH(iCmd);

        DBGTAG(1);

    }
    }
    @endcode
    Get Result as Following:

    TestFunction,T:41 -begin             //!< DBGTAG(0);
    iCmd=31                              //!< DBGD(iCmd);
    CHK: 216, TestFunction               //!< CHKPNT;
    iCmd=0x0000001F                      //!< DBGH(iCmd);
    TestFunction,T:41 -end               //!< DBGTAG(1);
*/
//@{
#define CHKPNT    debug_msg("\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__) ///< Show a color sting of line count and function name in your insert codes
#define DBGD(x)   debug_msg("\033[0;35m%s=%d\033[0m\r\n",#x,x)                  ///< Show a color sting of variable name and variable deciaml value
#define DBGH(x)   debug_msg("\033[0;35m%s=0x%08X\033[0m\r\n",#x,x)              ///< Show a color sting of variable name and variable hexadecimal value
#define DBGTAG(x) debug_msg("%s,T:%02d %s\r\n",__func__,__TASKID__,(x)?"-end":"-begin");///< Show function caller task with begin 'DBGTAG(0)' and end 'DBGTAG(1)'
//@}

/**
    Show the function caller address

    This Macro is only for Debuging,In release Mode shoulde remove it.
    Usage: This Macro must be put in the preceding procedure code

    @code
    {
    INT32 AVIPlayParseIndexTbl(USHORT *pdata, UINT32 aviStartAddr)
    {
         ---------------------------
           variables declartion area
         ---------------------------

         -->DBGCALLER();<--

         --------------------------
         procedure code body
         --------------------------
    }
    }
    @endcode
    According to the 0x00076BD4 to find the which is called at 00076BD4 inner function(caller)
    then get the caller against Project\DemoKit\DemoKit_Data\Release\MIPS\dasm.txt
*/
#define DBGCALLER() debug_msg("^Y%s() is called @0x%08X in %s\r\n",__func__,__CALL__-8,__TASK__);


//////////////////////////////////// for dump data to file ///////////////////////////
typedef void (*File_CB)(UINT32 MessageID, UINT32 Param1, UINT32 Param2, UINT32 Param3); ///< file system callback. refre to Filesys's FileSys_CB

/**
    File dump object

    For NAND storage or other else, there are existing something dumping content.
    Those have to use a file mechanism, so in project layer the user has to register this tab
    to dump successfly.
*/
typedef struct {
	UINT32 *(*OpenFile)(char *filename, UINT32 flag);   ///< file system open file
	INT32(*CloseFile)(UINT32 *pFile);   ///< file system close file
	INT32(*WriteFile)(UINT32 *pFile, UINT8 *pBuf, UINT32 *pBufSize, UINT32 Flag, File_CB CB); ///< file system write file
	UINT32 WriteFlag;   ///< file system write flag
} FILEOP_TAB, *PFILEOP_TAB;

/**
    Register file dump object

    If user wants to dump file success. File dump object must be registered.

    @param[in] pTab       file dump object refers to @ref FILEOP_TAB
*/
void debug_reg_fileFunc(FILEOP_TAB *pTab);

void debug_dump_open(char *pFilename); ///< Novatek internal using for starting dump file
void debug_dump_write(UINT32 uiData, UINT32 uiSize); ///< Novatek internal using for writing data
void debug_dump_close(void); ///< Novatek internal using for end file


// === profile ===
extern void profile_msg(char *fmtstr, ...); //cls="\x1B[2J\x1B[H"

#define PROFILE_MODE    0
#define PROFILE_DMA     1
#define PROFILE_CPU     2
#define PROFILE_TASK    3
extern void profile_cmd(UINT32 cmd, UINT32 param1, UINT32 param2);
extern void profile_get(UINT32 state, UINT32 *pvalue);
extern void profile_set(UINT32 state, UINT32 value);
extern void profile_task_busy(void);
extern void profile_task_idle(void);

#ifndef __ECOS
#define PROFILE_TASK_BUSY() profile_task_busy()
#define PROFILE_TASK_IDLE() profile_task_idle()
#else
#define PROFILE_TASK_BUSY()
#define PROFILE_TASK_IDLE()
#endif

// === autotest exam ===
/**
     write msg to the function registered by exam_msg_register
*/
extern int exam_msg(const char *fmtstr, ...);
/**
     register a callback (normally is autotest) for exam_msg.
*/
extern int exam_msg_register(int (*fp)(const char *fmtstr, va_list marker));

#endif
//@}
