/**
    uToken to parse command string

    @file       uToken.h
    @ingroup    UTOKEN

    Usage:
    @code
    INT32 param_w(INT32 argc, char* argv[])
    {
        return 0;
    }

    INT32 param_h(INT32 argc, char* argv[])
    {
        return 0;
    }

    INT32 param_begin(char* str, INT32 len)
    {
        printf("get: %s\r\n",str);
        return 0;
    }

    INT32 param_end(UTOKEN_END_INFO* p_info)
    {
        return 0;
    }

    INT32 param_unknown(char* name)
    {
        printf("unknown param: %s\r\n",name);
        return 0;
    }

    UTOKEN_PARAM tbl_media_param[] = {
        {"w",param_w},
        {"h",param_h},
        {NULL,NULL}, //last tag, it must be
    };

    UTOKEN_MODULE module_mediarec = {
        "mediarec",
        tbl_media_param,
        param_begin,
        param_end,
        param_unknown,
        NULL //must be NULL
    };

    char s[] = "mediarec -w 320 -h 240 -a test -b 1 2 3";
    uToken_Register(&module_mediarec);
    UTOKEN_MODULE* pModule = uToken_GetModule(s);
    if(pModule!=NULL)
    {
        uToken_Parse(s,pModule);
    }

    param_w() gets argc=1, argv[0]="320",
    param_h() gets argc=1, argv[0]="240",
    param_begin() gets "mediarec -w 320 -h 240 -a test -b"
    param_unknown gets "a" then gets "b"
    @endcode

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _UTOKEN_H
#define _UTOKEN_H

#ifdef __LINUX
typedef char  UTOKEN_CHAR;
typedef unsigned int UTOKEN_INT32;
typedef void  UTOKEN_VOID;
#else
typedef char  UTOKEN_CHAR;
typedef INT32 UTOKEN_INT32;
typedef void  UTOKEN_VOID;
#endif

/**
    Token Unit

    each token get their argument count and argument vector
*/
typedef struct _UTOKEN_PARAM {
	UTOKEN_CHAR *name; ///< token name
	UTOKEN_INT32(*fp)(UTOKEN_INT32 argc, UTOKEN_CHAR *argv[]);  ///< token's argc and argv callback
} UTOKEN_PARAM, *PUTOKEN_PARAM;

/**
    Token End Info

    each token get their argument count and argument vector
*/
typedef struct _UTOKEN_END_INFO {
	UTOKEN_CHAR *failed_name; ///< if *fp return non-zero, it will be name of parameter, otherwise is NULL.
	UTOKEN_INT32 err_code;    ///< if *fp return non-zero, it will equal to the value, otherwise is zero.
} UTOKEN_END_INFO, *PUTOKEN_END_INFO;

/**
    Moudle Unit

    each moudle set their token unit table and event callback
*/
typedef struct _UTOKEN_MODULE {
	UTOKEN_CHAR *name;
	UTOKEN_PARAM *params;
	UTOKEN_INT32(*fp_begin)(UTOKEN_CHAR *str, UTOKEN_INT32 len);
	UTOKEN_INT32(*fp_end)(UTOKEN_END_INFO *p_info);
	UTOKEN_INT32(*fp_unknown)(UTOKEN_CHAR *name);
	UTOKEN_VOID *Reversed;
} UTOKEN_MODULE, *PUTOKEN_MODULE;

/**
    Register module into uToken manager.

    @param[in] p_module necessary module data.
    @return
        - @b 0: success.
        - @b others: failed.
*/
extern UTOKEN_INT32 uToken_Register(UTOKEN_MODULE *p_module);

/**
    Get corresponding module.


    @param[in] str command string.
    @return
        - @b NULL: failed.
        - @b others: success to get matched module.
*/
extern UTOKEN_MODULE *uToken_GetModule(char *str);

/**
    Command parser.


    @param[in] str command string.
    @param[in] p_module selected module.
    @return
        - @b 0: success.
        - @b others: failed.
*/
extern UTOKEN_INT32 uToken_Parse(char *str, UTOKEN_MODULE *p_module);

//@}
#endif

