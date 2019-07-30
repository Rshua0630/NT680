/**
    Public header file for image setting information

    This file is the header file that define the API and data type for image information.

    @file       IPL_AlgInfor.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_ALGINFOR_H_
#define _IPL_ALGINFOR_H_

#include "Type.h"
#include "ipl_utility.h"
#include "ErrorNo.h"

/** \addtogroup mIIPL */
//@{

#define IPL_INFOR_ERR   0xffffffff    ///< error code(IPL_AlgGetUIInfo())

#define SEL_NO_CHANG    0xF0


#define IPL_SEL_TAG   0x43210000

/**
    IPL informatin selection index(IPL_AlgSetUIInfo() or IPL_AlgGetUIInfo())
*/
typedef enum _IPL_SEL_ITEM {
	IPL_SEL_RAW_BITDEPTH_PRV = IPL_SEL_TAG, ///<prv mode raw bits, data type #SEL_RAW_BITDEPTH
	IPL_SEL_RAW_BITDEPTH_CAP,           	///<cap mode raw bits, data type #SEL_RAW_BITDEPTH
	IPL_SEL_IMAGEFLIP_IMM,              	///<image flip mode, data type #SEL_IMAGEFLIP
	IPL_SEL_PRVSENMODE,                 	///<prv sensor mode
	IPL_SEL_CAPSENMODE,                 	///<cap sensor mode
	IPL_SEL_VIDEO_FPS,                  	///<Video FPS [31:16]/[15:0](src:bit[31:16], div:bit[15:0])
	IPL_SEL_VIDEO_SIZE,                 	///<Video SIZE (H_SIZE:bit[31:16], V_SIZE:bit[15:0])
//ALG
	IPL_SEL_DISCOMP_IMM,					///<prv DIS on/off, data type #SEL_DISCOMP
	IPL_SEL_SHDR,                       	///<SHDR mode on/off, data type #SEL_SHDR
	IPL_SEL_SHDR_LEVEL_IMM,                	///<SHDR mode on/off, level range: 0~255
	IPL_SEL_SHDR_FRAME_NUM,                	///<SHDR frame number, data type #SEL_SHDR_FRM_NUM
	IPL_SEL_WDR_IMM,                    	///<WDR mode on/off, data type #SEL_WDR
	IPL_SEL_WDR_M_LEVEL_IMM,              	///<WDR Level, 0~128
	IPL_SEL_DEFOG_IMM,                     	///<DEFOG mode on/off, data type #SEL_DEFOG
	IPL_SEL_DEFOG_LEVEL_IMM,               	///<DEFOG Level, 0~255
	IPL_SEL_FD_IMM,                     	///<Face detection on/off, data type #SEL_FD
	IPL_SEL_DEWARPING_IMM,                  ///<IPL use multi-stripe to do wall-mount dewarping by GDC's 2D lut
//Other
	IPL_USER_DEFINE1,
	IPL_USER_DEFINE2,
	IPL_SEL_SQUARE_REF_PATH,				///<square reference path select, data type #SEL_SQUARE_REF_PATH
	IPL_SEL_MAX_CNT,
	ENUM_DUMMY4WORD(IPL_SEL_ITEM)
} IPL_SEL_ITEM;

#define IPL_SEL_MAX_ITEM  (IPL_SEL_MAX_CNT - IPL_SEL_TAG)

/**
    bayer bit number
*/
typedef enum _SEL_RAW_BITDEPTH {
	SEL_RAW_BITDEPTH_8 = 8,		///<-
	SEL_RAW_BITDEPTH_10 = 10,   ///<-
	SEL_RAW_BITDEPTH_12 = 12,   ///<-
	SEL_RAW_BITDEPTH_16 = 16,   ///<-
	SEL_RAW_BITDEPTH_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_RAW_BITDEPTH)
} SEL_RAW_BITDEPTH;

/**
    image flip and mirror
*/
typedef enum _SEL_IMAGEFLIP {
	SEL_IMAGEFLIP_OFF = 0,
	SEL_IMAGEFLIP_H,
	SEL_IMAGEFLIP_V,
	SEL_IMAGEFLIP_H_V,
	SEL_IMAGEFLIP_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_IMAGEFLIP)
} SEL_IMAGEFLIP;

/**
    dis on/off
*/
typedef enum _SEL_DISCOMP {
	SEL_DISCOMP_OFF = 0,    ///<-
	SEL_DISCOMP_ON,         ///<-
	SEL_DISCOMP_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_DISCOMP)
} SEL_DISCOMP;

/**
    shdr on/off
*/
typedef enum _SEL_SHDR {
	SEL_SHDR_OFF = 0,   ///<- Linear in SHDR Mode, show long expo frame only
	SEL_SHDR_ON,        ///<- Staggered Mode
	SEL_SHDR_WDR,       ///<- Staggered Mode, only use long expo frame, plus WDR post-process (for fast switch between HDR & WDR mode)
	SEL_SHDR_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_SHDR)
} SEL_SHDR;

/**
    shdr frame number
*/
typedef enum _SEL_SHDR_FRM_NUM {
	SEL_SHDR_FRAME_NONE = 1,    ///<-
	SEL_SHDR_FRAME_2 = 2,    ///<-
	SEL_SHDR_FRAME_3 = 3,    ///<-
	SEL_SHDR_FRAME_4 = 4,    ///<-
	SEL_SHDR_FRAME_MAX_CNT = SEL_SHDR_FRAME_4,
	ENUM_DUMMY4WORD(SEL_SHDR_FRM_NUM)
} SEL_SHDR_FRM_NUM;

/**
    wdr on/off
*/
typedef enum _SEL_WDR {
	SEL_WDR_OFF = 0,    ///<-wdr linear mode
	SEL_WDR_AUTO,       ///<-wdr auto mode
	SEL_WDR_MANUAL,     ///<-wdr manual mode, adjust wdr by IPL_SEL_WDR_M_LEVEL_IMM
	SEL_WDR_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_WDR)
} SEL_WDR;

/**
    defog on/off
*/
typedef enum _SEL_DEFOG {
	SEL_DEFOG_OFF = 0,  ///<-defog disable
	SEL_DEFOG_ON,       ///<-defoe enable
	SEL_DEFOG_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_DEFOG)
} SEL_DEFOG;

/**
   Face Detection on/off
*/
typedef enum {
	SEL_FD_OFF = 0,            ///<-
	SEL_FD_ON,                 ///<-
	SEL_FD_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_FD)
} SEL_FD;

/**
   tmnr reference path select
*/
typedef enum {
	SEL_SQUARE_REF_PATH_1 = 0,	///<-
	SEL_SQUARE_REF_PATH_2,		///<-
	SEL_SQUARE_REF_PATH_3,		///<-
	SEL_SQUARE_REF_PATH_4,		///<-
	SEL_SQUARE_REF_PATH_5,		///<-
	SEL_SQUARE_REF_PATH_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_SQUARE_REF_PATH)
} SEL_SQUARE_REF_PATH;

/**
    dewarping select
*/
typedef enum _SEL_DEWARPING
{
    SEL_DEWARPING_OFF = 0,     ///<-normal case, dewarping off
    SEL_DEWARPING_ON_TYPE_FULL,      ///<-dewarping on
    SEL_DEWARPING_ON_TYPE_HORI,          ///<-dewarping on
    SEL_DEWARPING_ON_TYPE_VERT,          ///<-dewarping on
    SEL_DEWARPING_ON_TYPE_USER1,          ///<-dewarping on
    SEL_DEWARPING_ON_TYPE_USER2,          ///<-dewarping on
    SEL_DEWARPING_ON_TYPE_USER3,          ///<-dewarping on
    SEL_DEWARPING_ON_TYPE_USER4,          ///<-dewarping on
    SEL_DEWARPING_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DEWARPING)
} SEL_DEWARPING;

/**
     IPL Get UI Information

     @param[in] Id    IPL process id
     @param[in] SelIdx    UI Select item

     @return
        -b UINT32: select item information.
*/
extern UINT32 IPL_AlgGetUIInfo(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx);

/**
     IPL Set UI Infomation

     @param[in] Id    IPL process id
     @param[in] SelIdx    UI Select item
     @param[in] Value     information for select item

     @return Description of data returned.
         - @b E_OK
         - @b E_SYS
*/
extern ER IPL_AlgSetUIInfo(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx, UINT32 Value);


/**
     register event callback function

     @note when Using IPL_AlgSetUIInfo to change Alg UI information, the callback function can be executed.

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
typedef void (*IPL_INFOR_EVENT_FP)(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value);
extern ER IPL_AlgSetEventFP(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx, IPL_INFOR_EVENT_FP fp);


/**
     initial Alg UI information

     @note reset all array information.
           when user want to re-assign buffer, it can be executed at change mode flow.

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_AlgUIInfoInit(void);

/**
     un-initial Alg UI information

     @note unload Alg UI information. it can be executed at mode close.

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER IPL_AlgUIInfoUnInit(void);

//@}
#endif //_IPL_ALGINFOR_H_
