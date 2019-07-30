/**
    Module debug message

    Use this debug message suit for show debug message with module tag. User can
    operate filter to select message you want.

    @file       DebugModule.h
    @ingroup    mISYSDebug
    @note

    Usage:
    @code
    [module.c]
    #define __MODULE__    MyLib
    #define __DBGLVL__    2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
    #define __DBGFLT__    "*" // *=All, [mark1]=CustomClass
    #include "DebugModule.h"

    func()
    {
        DBG_ERR("My msg\r\n");
        DBG_MSG("My msg\r\n");
    }

    __MODULE__ = your module name (C Language Symbol)


    __DBGLVL__ = 0~7 : select level and filter in compile time
    __DBGLVL__ = 8 : select level and filter in runtime by these global varable:

                     UINT32 _DBGLVL_MyLib = 2;
                     char _DBGFLT_MyLib[] = "*";

    __DBGFLT__ = "*" : see all messages
    __DBGFLT__ = "[mark1]" : see only CustomClass [mark1] messages
    __DBGFLT__ = "*[mark1]" : see general  messages & CustomClass [mark1] messages



    NOTE:
    -----------------------------------------------------------
    API               effective         __DBGLVL__
    -----------------------------------------------------------
    DBG_FATAL()           v                 >= 0
    -----------------------------------------------------------
    DBG_ERR()             v                 >= 1
    -----------------------------------------------------------
    DBG_WRN()             v                 >= 2
    -----------------------------------------------------------
    DBG_UNIT_BEGIN()      v(*)              >= 3
    DBG_UNIT()
    DBG_UNIT_END()
    -----------------------------------------------------------
    DBG_FUNC_BEGIN()      v(*)              >= 4
    DBG_FUNC()
    DBG_FUNC_END()
    -----------------------------------------------------------
    DBG_IND()             v(*)              >= 5
    -----------------------------------------------------------
    DBG_MSG()             v(*)              >= 6
    -----------------------------------------------------------
    DBG_D()               v                 >= 7
    DBG_X()
    DBG_S()
    DBG_P()
    DBG_EOL()
    -----------------------------------------------------------
     v : effective
    (*): support filtering by __DBGFLT__ settings
    FATAL,ERR,WRN,UNIT,FUNC,IND: output additional FILE, FUNC, LINE info
    UNIT: output additional caller TASK, caller ADDRESS

    //TM - TIMEMARK: record to global timing record
    Usage:

    [module.c]
    #define __MODULE__    MyLib
    #define __PERFTM__    1 //1:Enable TIMEMARK,0:Disable TIMEMARK
    #include "DebugModule.h"

    func()
    {
        TM_BEGIN(name);
        TimerDelay(100);
        TM_END(name);

        TM_DUMP(MyLib); //dump all TM result of MyLib
    }

    Output:
    name    11104510
    name    12110270

    //BM - BENCHMARK: record to local benchmark record
    Usage:

    [module.h]
    BM_VAL_DECLARE(name);

    [module.c]
    #define __MODULE__    MyLib
    #define __PERFBM__    1 //1:Enable BENCHMARK,0:Disable BENCHMARK
    #include "DebugModule.h"
    BM_VAL_DEFINE(name);

    func()
    {
        BM_BEGIN(name);
        TimerDelay(100);
        BM_END(name);
        BM_BEGIN(name2);
        TimerDelay(200);
        BM_END(name2);

        BM_DUMP_HEADER();
        BM_DUMP(name); //dump BM:name result of MyLib
        BM_DUMP(name2); //dump BM:name result of MyLib
    }

    Output:
    >Total-Time,   Count,  Avg-Time, [Module::Func]
    >------------------------------------------------------------
    >  1005.040,       1,   100.040, [MyLib::name]
    >  1005.038,       1,   100.038, [MyLib::name2]
    @endcode

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef __DEBUG_MODULE__
#define __DEBUG_MODULE__

#include "SysKer.h"
#include "Debug.h"
#include "Perf.h"
#include <string.h>
#include <stdlib.h>

#define DBG_SETTINGS  1

#ifndef __MODULE__
#define __MODULE__ __SHORT_FILE__
#endif

#ifndef __DBGLVL__
#undef DBG_SETTINGS
#define DBG_SETTINGS  0
#endif

#ifndef __DBGFLT__
#undef DBG_SETTINGS
#define DBG_SETTINGS  0
#endif

#define __DBGLVL__FATAL     0
#define __DBGLVL__ERR       1
#define __DBGLVL__WRN       2
#define __DBGLVL__UNIT      3
#define __DBGLVL__FUNC      4
#define __DBGLVL__IND       5
#define __DBGLVL__MSG       6
#define __DBGLVL__VALUE     7
#define __DBGLVL__USER      8

#if (DBG_SETTINGS==0)
#warning Please define your __MODULE__,__DBGLVL__ and __DBGFLT__ before include "DebugModule.h"
#warning Ex:
#warning #define __MODULE__    MyLib
#warning #define __DBGLVL__    2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#warning #define __DBGFLT__    "*" // *=All, [mark1]=CustomClass
#warning include "DebugModule.h"
#warning
#warning if level = 8, please provide global varable as "dynamic level and filter":
#warning UINT32 _DBGLVL_MyLib = 2;
#warning char _DBGFLT_MyLib[] = "*";
#warning
#error (see above)
#endif

#define _TOKEN_STR(a)  # a
#define TOKEN_STR(a)  _TOKEN_STR(a)
#define _TOKEN_BIND(a, b)  a ## b
#define TOKEN_BIND(a, b)  _TOKEN_BIND(a, b)

#ifdef __DBGMONO__
#define DBG_COLOR_GRAY ""
#define DBG_COLOR_RED ""
#define DBG_COLOR_YELLOW ""
#define DBG_COLOR_CYAN ""
#define DBG_COLOR_GREEN ""
#define DBG_COLOR_MAGENTA ""
#define DBG_COLOR_BLUE ""
#define DBG_COLOR_WHITE ""
#define DBG_COLOR_HI_GRAY ""
#define DBG_COLOR_HI_RED ""
#define DBG_COLOR_HI_YELLOW ""
#define DBG_COLOR_HI_CYAN ""
#define DBG_COLOR_HI_GREEN ""
#define DBG_COLOR_HI_MAGENTA ""
#define DBG_COLOR_HI_BLUE ""
#define DBG_COLOR_HI_WHITE ""
#else
#define DBG_COLOR_GRAY "^N"
#define DBG_COLOR_RED "^R"
#define DBG_COLOR_YELLOW "^Y"
#define DBG_COLOR_CYAN "^C"
#define DBG_COLOR_GREEN "^G"
#define DBG_COLOR_MAGENTA "^M"
#define DBG_COLOR_BLUE "^B"
#define DBG_COLOR_WHITE "^W"
#define DBG_COLOR_HI_GRAY "~N"
#define DBG_COLOR_HI_RED "~R"
#define DBG_COLOR_HI_YELLOW "~Y"
#define DBG_COLOR_HI_CYAN "~C"
#define DBG_COLOR_HI_GREEN "~G"
#define DBG_COLOR_HI_MAGENTA "~M"
#define DBG_COLOR_HI_BLUE "~B"
#define DBG_COLOR_HI_WHITE "~W"
#endif

#ifdef __DBGMONO__
#define DBG_COLOR_FATAL ""
#define DBG_COLOR_ERR ""
#define DBG_COLOR_WRN ""
#define DBG_COLOR_UNIT ""
#define DBG_COLOR_FUNC ""
#define DBG_COLOR_IND ""
#define DBG_COLOR_MSG ""
#define DBG_COLOR_VALUE ""
#else
#define DBG_COLOR_FATAL "~R"
#define DBG_COLOR_ERR "~R"
#define DBG_COLOR_WRN "~Y"
#define DBG_COLOR_UNIT "^G"
#define DBG_COLOR_FUNC "^C"
#define DBG_COLOR_IND ""
#define DBG_COLOR_MSG ""
#define DBG_COLOR_VALUE ""
#endif


/**
    @def DBG_FATAL(fmtstr)
    @brief Show error message

    @def DBG_ERR(fmtstr)
    @brief Show function begin

    @def DBG_WRN(fmtstr)
    @brief Show warning message

    @def DBG_UNIT_BEGIN(fmtstr)
    @brief Show module important function begin

    @def DBG_UINT_END(fmtstr)
    @brief Show module important function end

    @def DBG_FUNC_BEGIN(fmtstr)
    @brief Show function begin

    @def DBG_FUNC_END(fmtstr)
    @brief Show function end

    @def DBG_IND(fmtstr)
    @brief Show indiction

    @def DBG_MSG(fmtstr)
    @brief Show message

    @def DBG_D(fmtstr)
    @brief Show a variable decimal value

    @def DBG_X(fmtstr)
    @brief Show a variable hex value

    @def DBG_S(fmtstr)
    @brief Show a variable string value

    @def DBG_P(fmtstr)
    @brief Show a variable hex value with 4 chars format

    @def DBG_EOL(fmtstr)
    @brief Show a change line tag

*/

/**
    @def DBG_DUMP(fmtstr)
    @brief Show dumpt message (anytime)
*/


#if (__DBGLVL__>=8)  //LEVEL = USER

#define _DBGLVL_ TOKEN_BIND(_DBGLVL_,__MODULE__)
#define _DBGFLT_ TOKEN_BIND(_DBGFLT_,__MODULE__)

extern UINT32 _DBGLVL_;
extern char _DBGFLT_[];

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) if(_DBGLVL_>=1){ debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_WRN(fmtstr, args...) if(_DBGLVL_>=2){ debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_UNIT_BEGIN(fmtstr, args...) if(_DBGLVL_>=3){ debug_msg_filter(_DBGFLT_, DBG_COLOR_UNIT "%s():begin {%s/0x%08x} " fmtstr, \
			__func__, __TASK__, __CALL__, ##args); } while(0)

#define DBG_UNIT(fmtstr, args...) if(_DBGLVL_>=3){ debug_msg_filter(_DBGFLT_, DBG_COLOR_UNIT "%s(): {%s/0x%08x} " fmtstr, \
			__func__, __TASK__, __CALL__, ##args); } while(0)

#define DBG_UNIT_END(fmtstr, args...) if(_DBGLVL_>=3){ debug_msg_filter(_DBGFLT_, DBG_COLOR_UNIT "%s():end " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_FUNC_BEGIN(fmtstr, args...) if(_DBGLVL_>=4){ debug_msg_filter(_DBGFLT_, DBG_COLOR_FUNC "%s():begin " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_FUNC(fmtstr, args...) if(_DBGLVL_>=4){ debug_msg_filter(_DBGFLT_, DBG_COLOR_FUNC "%s(): " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_FUNC_END(fmtstr, args...) if(_DBGLVL_>=4){ debug_msg_filter(_DBGFLT_, DBG_COLOR_FUNC "%s():end " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_IND(fmtstr, args...) if(_DBGLVL_>=5){ debug_msg_filter(_DBGFLT_, DBG_COLOR_IND "%s(): " fmtstr, \
			__func__, ##args); } while(0)

#define DBG_MSG(fmtstr, args...) if(_DBGLVL_>=6){ debug_msg_filter(_DBGFLT_, fmtstr, ##args); } while(0)

#define DBG_D(var) if(_DBGLVL_>=7){ debug_msg("%s=%d ", #var, (int)(var)); } while(0)
#define DBG_X(var) if(_DBGLVL_>=7){ debug_msg("%s=%x ", #var, (int)(var)); } while(0)
#define DBG_S(var) if(_DBGLVL_>=7){ debug_msg("%s=%s ", #var, (char*)(var)); } while(0)
#define DBG_P(var) if(_DBGLVL_>=7){ debug_msg("%s=%08x ", #var, (void*)(var)); } while(0)
#define DBG_EOL() if(_DBGLVL_>=7){ debug_msg("\r\n"); } while(0)

#elif (__DBGLVL__==7)  //LEVEL = VALUE

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
		__func__, ##args)

#define DBG_UNIT_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():begin {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s(): {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():begin " fmtstr, \
		__func__, ##args)

#define DBG_FUNC(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_IND(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_IND "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_MSG(fmtstr, args...) debug_msg_filter(__DBGFLT__, fmtstr, ##args)

#define DBG_D(var) debug_msg("%s=%d ", #var, (int)(var))
#define DBG_X(var) debug_msg("%s=%x ", #var, (int)(var))
#define DBG_S(var) debug_msg("%s=%s ", #var, (char*)(var))
#define DBG_P(var) debug_msg("%s=%08x ", #var, (void*)(var))
#define DBG_EOL() debug_msg("\r\n")

#elif (__DBGLVL__==6)  //LEVEL = MSG

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
		__func__, ##args)

#define DBG_UNIT_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():begin {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s(): {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():begin " fmtstr, \
		__func__, ##args)

#define DBG_FUNC(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_IND(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_IND "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_MSG(fmtstr, args...) debug_msg_filter(__DBGFLT__, fmtstr, ##args)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#elif (__DBGLVL__==5)  //LEVEL = IND

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
		__func__, ##args)

#define DBG_UNIT_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():begin {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s(): {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():begin " fmtstr, \
		__func__, ##args)

#define DBG_FUNC(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_IND(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_IND "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#elif (__DBGLVL__==4)  //LEVEL = FUNC

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
		__func__, ##args)

#define DBG_UNIT_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():begin {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s(): {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():begin " fmtstr, \
		__func__, ##args)

#define DBG_FUNC(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s(): " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_FUNC "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#elif (__DBGLVL__==3)  //LEVEL = UNIT

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
		__func__, ##args)

#define DBG_UNIT_BEGIN(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():begin {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s(): {%s/0x%08x} " fmtstr, \
		__func__, __TASK__, __CALL__, ##args)

#define DBG_UNIT_END(fmtstr, args...) debug_msg_filter(__DBGFLT__, DBG_COLOR_UNIT "%s():end " fmtstr, \
		__func__, ##args)

#define DBG_FUNC_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_FUNC(fmtstr, args...) do{}while(0)

#define DBG_FUNC_END(fmtstr, args...) do{}while(0)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#elif (__DBGLVL__==2)  //LEVEL = WRN

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) debug_msg(DBG_COLOR_WRN "WRN:%s() " fmtstr, \
		__func__, ##args)

#define DBG_UNIT_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_UNIT(fmtstr, args...) do{}while(0)

#define DBG_UNIT_END(fmtstr, args...) do{}while(0)

#define DBG_FUNC_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_FUNC(fmtstr, args...) do{}while(0)

#define DBG_FUNC_END(fmtstr, args...) do{}while(0)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#elif (__DBGLVL__==1)  //LEVEL = ERR

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) debug_msg(DBG_COLOR_ERR "ERR:%s() " fmtstr, \
		__func__, ##args)

#define DBG_WRN(fmtstr, args...) do{}while(0)

#define DBG_UNIT_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_UNIT(fmtstr, args...) do{}while(0)

#define DBG_UNIT_END(fmtstr, args...) do{}while(0)

#define DBG_FUNC_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_FUNC(fmtstr, args...) do{}while(0)

#define DBG_FUNC_END(fmtstr, args...) do{}while(0)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#else //(__DBGLVL__==0)  //LEVEL = FATAL

#define DBG_FATAL(fmtstr, args...) do{ debug_msg(DBG_COLOR_FATAL "FATAL:%s() " fmtstr, \
			__func__, ##args); exit(EXIT_FAILURE); } while(0)

#define DBG_ERR(fmtstr, args...) do{}while(0)

#define DBG_WRN(fmtstr, args...) do{}while(0)

#define DBG_UNIT_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_UNIT(fmtstr, args...) do{}while(0)

#define DBG_UNIT_END(fmtstr, args...) do{}while(0)

#define DBG_FUNC_BEGIN(fmtstr, args...) do{}while(0)

#define DBG_FUNC(fmtstr, args...) do{}while(0)

#define DBG_FUNC_END(fmtstr, args...) do{}while(0)

#define DBG_IND(fmtstr, args...) do{}while(0)

#define DBG_MSG(fmtstr, args...) do{}while(0)

#define DBG_D(var) do{}while(0)
#define DBG_X(var) do{}while(0)
#define DBG_S(var) do{}while(0)
#define DBG_P(var) do{}while(0)
#define DBG_EOL() do{}while(0)

#endif


#define DBG_DUMP(fmtstr, args...) debug_msg(fmtstr, ##args)



#ifndef NDEBUG //build debug

#define __assert2(e,func) \
	((void)debug_msg(DBG_COLOR_RED "ASSERT FAIL:%s() %s\r\n", func,e), abort(), 0)
#define ASSERT(e) \
	((void) ((e) ? 0 : __assert2 (#e, __func__)))

#else  //build release

#define ASSERT(ignore) ((void)0)

#endif



#ifdef __PERFTM__
#if (__PERFTM__>=1)  //LEVEL = ENABLE
#define TM_BEGIN(name)  \
	TM_Begin(TOKEN_STR(__MODULE__), TOKEN_STR(name))

#define TM_END(name) \
	TM_End(TOKEN_STR(__MODULE__), TOKEN_STR(name))

#define TM_CLEAR() \
	TM_Clear()

#define TM_DUMP(module) \
	TM_Dump(TOKEN_STR(module))

#else //(__PERFTM__==0)  //LEVEL = DISABLE

#define TM_BEGIN(name)
#define TM_END(name)
#define TM_CLEAR()
#define TM_DUMP(module)

#endif
#else

#define TM_BEGIN(name)
#define TM_END(name)
#define TM_CLEAR()
#define TM_DUMP(module)

#endif

#ifdef __PERFBM__
#if (__PERFBM__>=1)  //LEVEL = ENABLE
#define BM_VAL_DEFINE(name) \
	BENCHMARK TOKEN_BIND( BM_, name) = { TOKEN_STR(__MODULE__), TOKEN_STR(name), 0, 0, 0}

#define BM_BEGIN(name) \
	BM_Begin( & TOKEN_BIND( BM_, name))

#define BM_END(name) \
	BM_End( & TOKEN_BIND( BM_, name))

#define BM_CLEAR(name) \
	BM_Clear( & TOKEN_BIND( BM_, name))

#define BM_VAL_DECLARE(name) \
	extern BENCHMARK TOKEN_BIND( BM_, name)

#define BM_DUMP_HEADER() \
	BM_Dump_Header()

#define BM_DUMP(name) \
	BM_Dump( & TOKEN_BIND( BM_, name))

#else //(__PERFBM__==0)  //LEVEL = DISABLE

#define BM_VAL_DEFINE(name)
#define BM_BEGIN(name)
#define BM_END(name)
#define BM_CLEAR(name)
#define BM_VAL_DECLARE(name)
#define BM_DUMP_HEADER()
#define BM_DUMP(name)

#endif
#else

#define BM_VAL_DEFINE(name)
#define BM_BEGIN(name)
#define BM_END(name)
#define BM_CLEAR(name)
#define BM_VAL_DECLARE(name)
#define BM_DUMP_HEADER()
#define BM_DUMP(name)

#endif

#endif //__DEBUG_MODULE__

