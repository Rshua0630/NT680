/**
    Object Detection and Tracking library.

    @file       odt_lib.h
    @ingroup    mILibODT

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef ODT_LIB_H
#define ODT_LIB_H



#include "dis_lib.h"
#include "dis_alg_fw.h"
#include "dis_alg_hw.h"
#include "Type.h"
#include "GxImage.h"


#define ODT_NUM_MAX                 64  ///< max obj number in ODT
#define ODT_MAX_DISRSLTBUF_NUM 		30
#define ODT_PROCESS_NUM_MAX         10  ///< max ODT process number

#define ODT_OFFLINE_TEST            DISABLE
#define ODT_TEST					DISABLE



/**
     @name ODT return status
*/
//@{
#define ODT_STA_OK                 (0)                   ///<  the status is ok
#define ODT_STA_ERROR             (-1)                  ///<  some error occurred
//@}


typedef struct _odt_rslt_loc
{
	UINT32 timestamp_sec;
	UINT32 timestamp_usec;
}odt_rslt_loc;

typedef struct _odt_od_result
{
    UINT32	class;
    INT32 	score;
    UINT32 	x;
    UINT32 	y;
    UINT32 	w;
    UINT32 	h;
}odt_od_result;


typedef struct _odt_obj
{
    URECT		obj_rect;          ///< result start x, start y, width, height
    UINT32		id;                ///< result id
	INT32  		score;             ///< result obj quality reference score
    INT32  		class;             ///< result obj class
    INT32  		type;              ///< result type: 0--the current detected target, other--estimated value of previous target .     
    INT32  		life1;
	INT32  		life2;
    IMG_BUF *	p_frame;          ///< result obj corresponding frame image
    odt_rslt_loc timestamp;
} odt_obj;

typedef struct _odt_rslt_inf
{
	INT32	flag;
	UINT32 	num;
	INT32 	index_max[ODT_NUM_MAX];
	INT32 	index_shift_x[ODT_NUM_MAX];
	INT32 	index_shift_y[ODT_NUM_MAX];
	odt_obj	odt_obj_inf[ODT_NUM_MAX];
}odt_rslt_inf;


typedef enum
{
	odt_low = 0,
	odt_normal,
	odt_high
} odt_value;

typedef struct
{
	odt_value sensitivity;
	float sensitivity_w;
	float sensitivity_b;
	UINT32 uiNumH;
	UINT32 uiNumV;
	DIS_BLKSZ blksz;
}odt_track_param;


/**
     Config ID.

*/
typedef enum
{
    odt_cfg_max_obj_life1,       ///< config max obj life1
	odt_cfg_max_obj_life2,       ///< config max obj life2
	odt_cfg_outobj_context,      ///< config output obj context range, can be set to 0~100, e.g. set 15 means output obj box with 1.5 times the original size and context range.
	odt_cfg_scale_for_match,     ///< config scale factor for match
	odt_cfg_sensitivity,         ///< config odt sensitivity
	ENUM_DUMMY4WORD(odt_cfg)
} odt_cfg;


typedef struct _ODT_MOTION_INFOR {
	INT32   iX;         ///< x component
	INT32   iY;         ///< y component
	BOOL    bValid;     ///< reliability of MV
} ODT_MOTION_INFOR;


/**
     DIS results buffer.
*/
typedef struct _odt_dis_rsltbuf
{
	ODT_MOTION_INFOR odt_dis_mvrslt[MVNUMMAX];
	UINT64           odt_dis_timestamp;
}odt_dis_rsltbuf;

//typedef struct _odt_dis_rsltbuf
//{
//	MOTION_INFOR odt_dis_mvrslt[MVNUMMAX];
//}odt_dis_rsltbuf;


typedef struct _odt_process_buf_param
{
	UINT32	process_id;
	UINT32 	detect_rslt_addr;
	UINT32 	odt_od_rslt_buffer_addr;
	UINT32 	odt_rslt_buffer_addr;
}odt_process_buf_param;

typedef struct _odt_process_param
{
	INT32	flag;
	UINT32 	process_id;
	
	UINT32 	detect_loc;
	UINT32 	detect_loc_pre;
	UINT32 	detect_num;
	UINT32 	detect_rslt_addr;

	UINT32 	odt_od_rslt_loc;
	UINT32 	odt_od_rslt_loc_pre;
	UINT32 	odt_od_rslt_buffer_addr;

	UINT32  odt_rslt_loc_after_match;
	
	UINT32 	odt_rslt_loc;
	UINT32 	odt_rslt_buffer_addr;

	UINT32 	odt_getdisrsltloc;

	UINT32 	odt_obj_id;
	INT32 	odt_max_objlife1;
	INT32 	odt_max_objlife2;
	INT32 	odt_outobjcontext;
    INT32	odt_scale_for_match;
	odt_track_param	odt_ot_param;

}odt_process_param;



/**
    Install required system resource.
*/
extern void odt_installid(void) _SECTION(".kercfg_text");


extern INT32 odt_regist(odt_process_buf_param *param);
extern INT32 odt_logoff(UINT32 process_id);



/**
     Config some settings of ODT.

     @param[in] config_id: configure ID.
     @param[in] value: setting value.

     Example:
     @code
     {
         odt_config(odt_cfg_max_obj_life1, 15);
     }
     @endcode
*/
extern void odt_config(odt_cfg config_id, INT32 value, UINT32 process_id);


/**
     Get some configurations of ODT.

     @param[in] config_id: configure ID.

     @return the setting value.

     Example:
     @code
     {
         UINT32 value;
         value  = odt_getconfig(odt_cfg_max_obj_life1);
     }
     @endcode
*/
extern UINT32 odt_getconfig(odt_cfg config_id, UINT32 process_id);
/**
     Init ODT function.

     This function is used to initialize ODT buffer and set some initial setting.

     @param[in] buf: the work buffer for ODT process. the required buffer size can be get by odt_calcbuffsize().
     @param[in] cachebuf: the cache work buffer for ODT process. the required cache buffer size can be get by odt_calccachebuffsize().
     @return
         - @b odt_sta_ok:       Success.
         - @b odt_sta_error:    Some error occurred.

     Example:
     @code
     {
         MEM_RANGE buf, cachebuf;

         buf.Addr = pbuf;
         buf.Size = odt_calcbuffsize();
         cachebuf.Addr = pcachebuf;
         cachebuf.Size = odt_calccachebuffsize();

         // Init ODT buffer
         odt_init(buf,cachebuf);
     }
     @endcode


*/
extern INT32 odt_init(MEM_RANGE *buf, MEM_RANGE *cachebuf);

/**
     Reset ODT function.

     The API will reset the ODT result number and params.
     @return
         - @b odt_sta_ok:       Success.
         - @b odt_sta_error:    Some error occurred.
*/
extern INT32 odt_reset(void);


/**
     UnInit ODT function.

     The API will reset the ODT result number and release buffer.
     @return
         - @b odt_sta_ok:       Success.
         - @b odt_sta_error:    Some error occurred.
*/
extern INT32 odt_uninit(void);


/**
     Process the ODT function.

     This API include obj detection and obj tracking

     Obj detection will scale the input image to temp buffer by image ratio.\n
     If Novatek OD lib return TRUE, then we treat this candidate obj as a real obj, and give it a id number.\n

     Obj tracking will tracking the reference real obj in present frame use DIS process.\n
     If DIS process return TRUE, then we treat this obj and the reference real obj having same id number.\n
     
*/
extern void odt_od_process(void);
extern void odt_ot_process(void);

/**
     Calculate ODT required buffer size.

     The working buffer is used for obj detection and tracking.
     
     @return the required buffer size.
*/
extern UINT32   odt_calcbuffsize(void);

/**
     Calculate ODT required cache buffer size.

     The working buffer is used for obj detection and tracking.

     @return the required cache buffer size.
*/
extern UINT32   odt_calccachebuffsize(void);


/**
     Lock/Unlock ODT.

     @param[in] bLock: lock or unlock.

     Example:
     @code
     {
         // lock the ODT
         odt_lock(TRUE);
         // doing something such as capture
         ...
         // end of capture
         // unlock the ODT
         odt_lock(FALSE);
     }
     @endcode
*/
extern void     odt_lock(BOOL block);

/**
     Get the result objs of ODT.

     This API will return the result objs by different coordinate.\n

     @param[out] Obj: the result obj info.
     @param[in] process_id: the detection process id in ODT.
     @param[in] TargetCoord: the target window coordinate.

     @return The number of objs in present frame.

     Example:
     @code
     {
        odt_obj   ODTInfo[ODT_NUM_MAX];
        URECT      ODTDispCoord={0,0,640,480};
        UINT32     ObjNumber;

        ObjNumber = odt_getrsltobj_for_draw(ODTInfo, 1, &ODTDispCoord);
     }
     @endcode
*/
extern UINT32   odt_getrsltobj_for_draw(odt_obj *obj, UINT32 process_id, URECT* targetcoord);
extern UINT32 	odt_getrsltobj_after_match(odt_obj *obj, UINT32 process_id, URECT* targetcoord, UINT32 *p_loc);

/**
     Map obj IDs to continuous IDs.

     @param[in] obj: ODT obj detection results.
     @param[in] objnumber: ODT obj number.

     @return void.

     Example:
     @code
     {
        ODT_OBJ   ODTInfo[ODT_NUM_MAX];
        URECT      ODTDispCoord={0,0,640,480};
        UINT32     ObjNumber;

        ObjNumber = odt_getrsltobj(ODTInfo, 1, &ODTDispCoord);
		odt_idmap(ODTInfo, ObjNumber);
     }
     @endcode

*/
extern void     odt_idmap(odt_obj* obj, UINT32 objnumber);



/**
     Get the number of buffer which used to save newest dis result.
*/
extern UINT32   odt_getsavedisrsltloc(void);



/**
     Save one DIS motion vecter result.
*/
extern void   odt_savedisrslt(void);
extern BOOL   odt_savedisrslt_offline(UINT32 edgeimgaddr);
extern INT32  odt_process_offline(UINT32 edgeimgaddr);

extern BOOL odt_getinitstate(void);

extern INT32 odt_setodrslt(UINT32 num, odt_od_result *rslt, UINT32 rslt_loc, UINT32 process_id);
extern INT32 odt_setodrslt_timestamp(UINT32 num, odt_od_result *rslt, UINT32 rslt_loc, UINT32 process_id);


#endif
