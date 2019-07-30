/**
    Public header file for capture information

    This file is the header file that define the API and data type for capture information.

    @file       ImgCapInfor.h
    @ingroup    mIAppCap
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IMG_CAPINFOR_H_
#define _IMG_CAPINFOR_H_

#include "Type.h"
#include "ErrorNo.h"


#define IMG_CAP_INFOR_ERR 0xffffffff    ///< error code(ImgCap_GetUIInfo())
#define IMG_CAP_TAG       0x12340000

/** \addtogroup mIAppCap */
//@{

/**
    capture informatin selection index(ImgCap_SetUIInfo() or ImgCap_GetUIInfo())
*/

/*****************tem*******************************/
typedef enum _CAP_SEL_ITEM_ {
	/************* CAP_SEL_ITEM start *************/
	CAP_SEL_ACTFLAG = IMG_CAP_TAG, ///<Pipeline ID configure Flag
	CAP_SEL_QUALITY,               ///<jpeg encode quality, data type depend on JPEG (0 ~ 100)
	CAP_SEL_FILEFMT,               ///<save file format, data type #SEL_FILEFMT.
	CAP_SEL_REENCTYPE,             ///<jpeg bit rate contorl type, data type #SEL_REENCTYPE.
	CAP_SEL_IMG_RATIO,             ///<image ratio(16:9, 4:3,....), data type #IPL_SENCROPRATIO
	CAP_SEL_JPG_IMG_H_SIZE,        ///<jpeg pixel size H(%16 == 0)
	CAP_SEL_JPG_IMG_V_SIZE,        ///<jpeg pixel size V(% 8 == 0)
	CAP_SEL_JPGFMT,                ///<jpeg format, data type #SEL_JPGFMT
	CAL_SEL_DATASTAMP,             ///<capture data stamp on/off
	CAP_SEL_SCREEN_IMG,            ///<screenail image add or not
	CAP_SEL_SCREEN_IMG_H_SIZE,     ///<screenail image pixel size H
	CAP_SEL_SCREEN_IMG_V_SIZE,     ///<screenail image pixel size V
	CAP_SEL_SCREEN_FMT,            ///<screenail format, data type #IPL_YUV_IMG_TYPE
	CAP_SEL_QV_IMG,                ///<quick view image on/off
	CAP_SEL_QV_IMG_H_SIZE,         ///<quick view image pixel size H
	CAP_SEL_QV_IMG_V_SIZE,         ///<quick view image pixel size V
	CAP_SEL_QV_FMT,                ///<quick view format, data type #IPL_YUV_IMG_TYPE
	CAP_SEL_PRI_FMT,               ///<main image format, data type #IPL_YUV_IMG_TYPE
	CAP_SEL_ENCRATIO,              ///<raw encode ratio, 50% ~ 80%, disable 100%
	CAP_SEL_SCREENBUFSIZE,         ///<screenail bit stream buffer size(byte)
	CAP_SEL_RHO_INITQF,            ///<bit rate contorl:RHO initial Q
	CAP_SEL_RHO_TARGETSIZE,        ///<bit rate contorl:RHO target bit stream size(byte)
	CAP_SEL_RHO_LBOUNDSIZE,        ///<bit rate contorl:RHO min bit stream size < target bit stream size(byte)
	CAP_SEL_RHO_HBOUNDSIZE,        ///<bit rate contorl:RHO max bit stream size > target bit stream size(byte)
	CAP_SEL_RHO_RETRYCNT,          ///<bit rate contorl:RHO retry number
	/************* CAP_SEL_ITEM end *************/

	/************* CAP_SEL_ITEM_INT start *************/
	/*************      DO NOT MODIFY     *************/
	CAP_SEL_DF_EN,                                  //dark frame mode enable/disable
	CAP_SEL_CH3_SUB_EN,                             //sie out channel 3 enable/disable
	CAL_SEL_CH3_SUB_H_SIZE,                         //sie out channel 3 size H
	CAL_SEL_CH3_SUB_V_SIZE,                         //sie out channel 3 size V
	CAP_SEL_RAWENC_EN,                              //sie out rawenc enable/disable
	/*************      DO NOT MODIFY     *************/
	CAP_SEL_LCA_EN,                                //MSNR process enable/disable
	CAP_SEL_LCA_H_SIZE,                             //LCA image pixel size H
	CAP_SEL_LCA_V_SIZE,                             //LCA image pixel size V
	CAP_SEL_LCA_FMT,                                //LCA input format
	CAP_SEL_MSNR_EN,                                //MSNR process enable/disable
	CAP_SEL_FLIP_DIRECTION,                         //FLIP direction
	CAP_SEL_ROTATION_DIRECTION,                     //FLIP direction
	CAP_SEL_PRIVACYMASK_EN,                         //privacy mask enable/disable
	CAP_SEL_PRIVACYMASK_H_SIZE,                     //privacy mask image pixel size H
	CAP_SEL_PRIVACYMASK_V_SIZE,                     //privacy mask image pixel size V
	CAP_SEL_PRIVACYMASK_FMT,                        //privacy mask input format
	CAP_SEL_DEFOG_EN,                               //DEFOG enable/disable
	CAP_SEL_SHDR_EN,                                //SHDR enable/disable
	CAP_SEL_PHDR_EN,                                //PHDR enable/disable
	CAP_SEL_PHDR_RHDR_X_WINNUM,                     //PHDR WINDOW NUMBER
	CAP_SEL_PHDR_RHDR_Y_WINNUM,                     //PHDR WINDOW NUMBER
	CAP_SEL_FRM_CTRL,                               // frame rate ctrl(if value=0,no use)
	/************* CAP_SEL_ITEM_INT end ***************/

	/************* DO NOT SEPARATE BY ID *************/
	CAP_SEL_DRIVEMODE,             ///<drive mode, data type #SEL_DRIVEMODE.
	CAP_SEL_PICNUM,                ///<picutre number, data type NULL.
	CAP_SEL_FILEBUFMODE,           ///<jpeg buffer mode, data type #SEL_FILEBUFMODE
	CAP_SEL_FILEBUFSIZE,           ///<jpeg buffer size(byte), only valid in #SEL_FILEBUFMODE_MANUAL mode
	CAP_SEL_RAW_BUF_NUM,           ///<raw data buffer number
	CAP_SEL_JPG_BUF_NUM,           ///<jpeg buffer number
	CAP_SEL_YUV_BUF_NUM,           ///<yuv buffer number
	CAP_SEL_MSGBUFSIZE,            ///<debug message buffer size(byte)
	CAP_SEL_THUMB_IMG,             ///<thumb nail image add or not
	CAP_SEL_THUMB_BUF_H_SIZE,      ///<thumb buffer pixel size H
	CAP_SEL_THUMB_BUF_V_SIZE,      ///<thumb buffer pixel size V
	CAP_SEL_THUMB_IMG_H_SIZE,      ///<thumb image pixel size H
	CAP_SEL_THUMB_IMG_V_SIZE,      ///<thumb image pixel size V
	CAP_SEL_THUMB_FMT,             ///<thumb format, data type #IPL_YUV_IMG_TYPE
	CAP_SEL_DISPTYPE,              ///<quick view display type, data type #SEL_DISPTYPE
	CAP_SEL_PRVPIPE,               //ipl return flow
	CAP_SEL_MSNR_P2_H_SIZE,        //MSNR image pixel size H
	CAP_SEL_MSNR_P2_V_SIZE,        //MSNR image pixel size V
	CAP_SEL_MSNR_P2_FMT,           //MSNR format

	CAP_SEL_MAX_CNT,
	ENUM_DUMMY4WORD(CAP_SEL_ITEM)
} CAP_SEL_ITEM;

#define CAP_SEL_MAX_ITEM  (CAP_SEL_MAX_CNT - IMG_CAP_TAG)
#define CAP_UI_FLAG_ITEM  (CAP_SEL_MAX_CNT - CAP_SEL_DRIVEMODE)

/*****************tem*******************************/


/**
    capture drive mode
*/
typedef enum _SEL_DRIVEMODE {
	SEL_DRIVEMODE_SINGLE = 0,               ///<invalid parameter: #CAP_SEL_PICNUM,
	SEL_DRIVEMODE_BURST,
	SEL_DRIVEMODE_INFBURST,                 ///<invalid parameter: #CAP_SEL_PICNUM,
	SEL_DRIVEMODE_EBURST,
	SEL_DRIVEMODE_EINFBURST,                ///<invalid parameter: #CAP_SEL_PICNUM,
	SEL_DRIVEMODE_SMALLBURST16,             ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
	SEL_DRIVEMODE_AEBREAKETING,
	SEL_DRIVEMODE_AWBBREAKETING,
	SEL_DRIVEMODE_DIS,                      ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
	SEL_DRIVEMODE_IMGEFFECT,                ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
	SEL_DRIVEMODE_HDR,                      ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
	SEL_DRIVEMODE_MULFMHDR,                 ///<invalid parameter: #CAP_SEL_PICNUM (>1), #CAP_SEL_RAW_BUF_NUM
	SEL_DRIVEMODE_MULFMRHDR,                ///<invalid parameter: #CAP_SEL_PICNUM (>1), #CAP_SEL_RAW_BUF_NUM
	SEL_DRIVEMODE_INVID,                    ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_JPG_BUF_NUM
	SEL_DRIVEMODE_SIM,                      ///<invalid parameter: #CAP_SEL_PICNUM, #CAP_SEL_RAW_BUF_NUM, #CAP_SEL_JPG_BUF_NUM
	SEL_DRIVEMODE_MULFMANTISHAKE,           ///<invalid parameter: #CAP_SEL_PICNUM (>1), #CAP_SEL_RAW_BUF_NUM
	SEL_DRIVEMODE_MULSENSOR,
	SEL_DRIVEMODE_MULINFBURST,
	SEL_DRIVEMODE_DUALSBS,
	SEL_DRIVEMODE_CUSTOMER1,
	SEL_DRIVEMODE_CUSTOMER2,
	SEL_DRIVEMODE_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_DRIVEMODE)
} SEL_DRIVEMODE;

/**
    save file format
*/
typedef enum _SEL_FILEFMT {
	SEL_FILEFMT_NONE = 0,                   ///< save none
	SEL_FILEFMT_RAW,                        ///< save raw(bayer)
	SEL_FILEFMT_JPG,                        ///< save jpg
	SEL_FILEFMT_RAW_JPG,                    ///< save raw(bayer) + jpg
	SEL_FILEFMT_MPO,
	SEL_FILEFMT_RAW_MPO,
	SEL_FILEFMT_RAW_LJPG,
	SEL_FILEFMT_RAW_RJPG,
	SEL_FILEFMT_RAW_LJPG_MPO,
	SEL_FILEFMT_RAW_RJPG_MPO,
	SEL_FILEFMT_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_FILEFMT)
} SEL_FILEFMT;

/**
    bit rate control method
*/
typedef enum _SEL_REENCTYPE {
	SEL_REENCTYPE_FIXQ = 0,                 ///< depend on #CAP_SEL_QUALITY, encode jpeg file
	SEL_REENCTYPE_RHO,                      ///< depend on CAP_SEL_RHO_xxxxx parameters, encode jpeg file
	SEL_REENCTYPE_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_REENCTYPE)
} SEL_REENCTYPE;

/**
    allocate buffer method for Jpeg file
*/
typedef enum _SEL_FILEBUFMODE {
	SEL_FILEBUFMODE_AUTO = 0,               ///< depend on remain memory, to assign jpeg buffer size
	SEL_FILEBUFMODE_MANUAL,                 ///< depend on #CAP_SEL_FILEBUFSIZE value, to assign jpeg buffer size
	SEL_FILEBUFMODE_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_FILEBUFMODE)
} SEL_FILEBUFMODE;

/**
    jpeg sampling mode
*/
typedef enum _SEL_JPGFMT {
	SEL_JPGFMT_444 = 0,                 ///< jpeg format 444
	SEL_JPGFMT_422,                     ///< jpeg format 422
	SEL_JPGFMT_420,                     ///< jpeg format 420
	SEL_JPGFMT_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_JPGFMT)
} SEL_JPGFMT;

/**
    quick view display type
*/
typedef enum _SEL_DISPTYPE {
	SEL_DISP_SINGLE = 0,                ///<-
	SEL_DISP_DUAL,                      ///<-
	SEL_DISP_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_DISPTYPE)
} SEL_DISPTYPE;

/**
    datastamp switch(on/off)
*/
typedef enum _SEL_DATASTAMP {
	SEL_DATASTAMP_ON = 0,               ///< cap datastamp on
	SEL_DATASTAMP_OFF,                  ///< cap datastamp off
	SEL_DATASTAMP_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_DATASTAMP)
} SEL_DATASTAMP;

/**
    capture picture nubmer
*/
typedef enum _SEL_PICNUM {
	SEL_PICNUM_INF = 0xfffffffe,        ///<-
	SEL_PICNUM_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_PICNUM)
} SEL_PICNUM;


/**
    screennail switch(on/off)
*/

typedef enum _SEL_SCREEN_IMG {
	SEL_SCREEN_IMG_ON = 0,          ///<-
	SEL_SCREEN_IMG_OFF,             ///<-
	SEL_SCREEN_IMG_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_SCREEN_IMG)
} SEL_SCREEN_IMG;

/**
    thumbnail switch(on/off)
*/

typedef enum _SEL_THUMB_IMG {
	SEL_THUMB_IMG_ON = 0,          ///<-
	SEL_THUMB_IMG_OFF,             ///<-
	SEL_THUMB_IMG_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_THUMB_IMG)
} SEL_THUMB_IMG;

/**
    quick view switch(on/off)
*/

typedef enum _SEL_QV_IMG {
	SEL_QV_IMG_ON = 0,          ///<-
	SEL_QV_IMG_OFF,             ///<-
	SEL_QV_IMG_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_QV_IMG)
} SEL_QV_IMG;


typedef enum _SEL_FLIP_DIR {
	SEL_FLIP_DIR_OFF = 0,          ///<-
	SEL_FLIP_DIR_H,             ///<-
	SEL_FLIP_DIR_V,
	SEL_FLIP_DIR_H_V,
	ENUM_DUMMY4WORD(SEL_FLIP_DIR)
} SEL_FLIP_DIR;

typedef enum _SEL_ROTATION_DIR {
	SEL_ROTATION_DIR_OFF = 0,          ///<-
	SEL_ROTATION_DIR_90,             ///<-
	SEL_ROTATION_DIR_270,
	ENUM_DUMMY4WORD(SEL_ROTATION_DIR)
} SEL_ROTATION_DIR;


typedef enum _SEL_RPI_MASK_TYPE {
	SEL_PRI_MASK_TYPE_YUV = 0,  ///< Using YUV color
	SEL_PRI_MASK_TYPE_PXL = 1,  ///< Using pixilation
	ENUM_DUMMY4WORD(SEL_RPI_MASK_TYPE)
} SEL_RPI_MASK_TYPE;

typedef struct _Cap_CoordinateRatio {
	UINT32  xratio;              ///< x point ratio
	UINT32  yratio;              ///< y point ratio
} Cap_CoordinateRatio;


typedef struct _CAP_UI_PRI_MASK_ITEM {
	BOOL                EnFlag;       ///< cap privacy mask function enable
	Cap_CoordinateRatio Coord[4];     ///< cap privacy mask 4 coordinates, must be Convex Hull
	SEL_RPI_MASK_TYPE   PmMskType;    ///< cap privacy mask type selection
	UINT32              PmColor[3];   ///< cap privacy mask using YUV color
	UINT32              AWeight;      ///< cap privacy mask alpha weight
} CAP_UI_PRI_MASK_ITEM;

typedef enum _CAP_PRI_PXL_BLK_SIZE {
	SEL_PRI_PIXELATION_08 = 0,  ///< 8x8
	SEL_PRI_PIXELATION_16 = 1,  ///< 16x16
	SEL_PRI_PIXELATION_32 = 2,  ///< 32x32
	SEL_PRI_PIXELATION_64 = 3,  ///< 64x64
	ENUM_DUMMY4WORD(CAP_PRI_PXL_BLK_SIZE)
} CAP_PRI_PXL_BLK_SIZE;


typedef struct _CAP_UI_PRI_MASK_INFO {
	CAP_UI_PRI_MASK_ITEM PmSet0;                    ///< cap privacy mask set0
	CAP_UI_PRI_MASK_ITEM PmSet1;                    ///< cap privacy mask set1
	CAP_UI_PRI_MASK_ITEM PmSet2;                    ///< cap privacy mask set2
	CAP_UI_PRI_MASK_ITEM PmSet3;                    ///< cap privacy mask set3
	CAP_UI_PRI_MASK_ITEM PmSet4;                    ///< cap privacy mask set4
	CAP_UI_PRI_MASK_ITEM PmSet5;                    ///< cap privacy mask set5
	CAP_UI_PRI_MASK_ITEM PmSet6;                    ///< cap privacy mask set6
	CAP_UI_PRI_MASK_ITEM PmSet7;                    ///< cap privacy mask set7

	CAP_PRI_PXL_BLK_SIZE PmPxlBlkSize;           ///< block size of pixelation
} CAP_UI_PRI_MASK_INFO;

/**
     Get Cap UI Information

     @param[in] SelIdx    UI Select item

     @return UINT32
         - @b select item information.

    Example:
    @code
    {
        DebugInfo.TotalSize = ImgCap_GetUIInfo(CAP_SEL_MSGBUFSIZE);
    }
    @endcode

*/
extern UINT32 ImgCap_GetUIInfo(UINT32 Id, CAP_SEL_ITEM SelIdx);

/**
     Set Cap UI Information

     @param[in] SelIdx    UI Select item
     @param[in] Value     item value
     @return ER
         - @b E_OK
         - @b E_SYS

    Example:
    @code
    {
        ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_HIGHSPEED);
    }
    @endcode

*/
extern ER ImgCap_SetUIInfo(UINT32 Id, CAP_SEL_ITEM SelIdx, UINT32 Value);

/**
     Set Cap Privacy Mask Information

     @param[in] Id        IPL Id
     @param[in] info      Privacy Mask Parameters
     @return ER
         - @b E_OK
         - @b E_SYS

    Example:
    @code
    {
        ImgCap_SetPMInfo(0, info);
    }
    @endcode

*/
extern ER ImgCap_SetPMInfo(UINT32 Id, CAP_UI_PRI_MASK_INFO *info);

//@}
#endif //_IMG_CAPINFOR_H_
