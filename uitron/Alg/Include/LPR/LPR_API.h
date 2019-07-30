/**
    LPR library functions.

    Definitions of License Plate Recognition Interface.

    @file       LPR_API.h
    @ingroup    LPR
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/
#ifndef _LPR_API_H
#define _LPR_API_H

#include "Type.h"
#include "GxImage.h"

#define LPR_VERSION_MAJOR 1
#define LPR_VERSION_MINOR 0
#define LPR_VERSION_BUILD 3



/**
    DSP core to run LPR.
*/

/**
    Return status.
*/
typedef enum {
        LPR_STA_OK =  0,                    ///< The status is OK
        LPR_STA_ERR = -1,                   ///< Some error occurred
        ENUM_DUMMY4WORD(LPR_STA)
} LPR_STA;

/**
    Car plate type
*/
typedef enum LPR_PLATE_TYPE {
        TW_TYPE = 1,                        ///< Car plate type
        CN_TYPE = 2,                        ///< Car plate type
}LPR_PLATE_TYPE;


// Structures ------------------------------------------------------------------

/**
    Version parameter
*/
typedef struct {
        UINT32 major;                       ///< The number of major in version, only get not set
        UINT32 minor;                       ///< The number of minor in version, only get not set
        UINT32 build;                       ///< The number of build in version, only get not set
}LPR_VERS;

/**
    System parameter
*/
typedef struct {
        LPR_VERS ver;                       ///< The version of system
}LPR_SYS_PRMS;


/**
    Result
*/
typedef struct LPR_RSLT_T {

        UINT8 lp_name[15];                  ///< The name of license plate
        INT32 diff;                         ///< The score of difference
        INT32 ch_num;                       ///< The number of character
}LPR_RSLT_T;

typedef struct {
        UINT32 max_rslt_num;                ///< The max candidates
        LPR_RSLT_T *rslt;                   ///< The structure of result
}LPR_RESULT;

/**
    Init parameters.
*/
typedef struct {
        LPR_PLATE_TYPE cp_type;             ///< The type of car plate
        UINT32 src_w;                       ///< Source width (sensor input)
        UINT32 src_h;                       ///< Source height (sensor input)
        UINT32 crop_x;                      ///< The x of source crop
        UINT32 crop_y;                      ///< The y of source crop
        UINT32 crop_w;                      ///< The width of source crop
        UINT32 crop_h;                      ///< The height of source crop
        UINT32 detect_w;                    ///< The scale width of image for detection of LPR 
        UINT32 detect_h;                    ///< The scale height of image for detection of LPR
        UINT32 reg_w;                       ///< The scale width of image for recognition of LPR
        UINT32 reg_h;                       ///< The scale height of image for recognition of LPR
        char   cp_model_path[256];          ///< The model path of car plate type
        MEM_RANGE *buf;                     ///< Memory range of working buffer
        MEM_RANGE *cache_buf;               ///< Memory range of working cache buffer
} LPR_INIT_PRMS;

/**
    Sensor input processing parameters.
*/
typedef struct {
        IMG_BUF *data;                      ///< Input sensor data (yuv420)
} LPR_SENSOR_PRMS;

/**
    File input processing parameters.
*/
typedef struct {
        CHAR *filename;                     ///< Input file name (jpeg)
} LPR_FILE_PRMS;

/**
    User parameter
*/
typedef enum
{
        LPR_ROI_LEFT_E,                     ///< The left distance of roi, unit:pixel
        LPR_ROI_RIGHT_E,                    ///< The right distance of roi, unit:pixel
        LPR_ROI_TOP_E,                      ///< The top distance of roi, unit:pixel
        LPR_ROI_BOTTOM_E,                   ///< The bottom distance of roi, unit:pixel
        LPR_FEA_MAX_WIDTH_E,                ///< The max width of car plate, unit: pixel
        LPR_FEA_MIN_WIDTH_E,                ///< The min width of car plate, unit: pixel
        LPR_FEA_MAX_HEIGHT_E,               ///< The max height of car plate, unit: pixel
        LPR_FEA_MIN_HEIGHT_E,               ///< The min height of car plate, unit: pixel
        LPR_REG_THRESHOLD_E,                ///< The threshold of recognition, the bigger value would cause more false result.
        ENUM_DUMMY4WORD(LPR_USR_PRMS)
}LPR_USR_PRMS;


/**
    Install required system resource.
*/
extern VOID LPR_InstallID(VOID)  _SECTION(".kercfg_text");

BOOL lpr_ap_init(LPR_INIT_PRMS *prms);
LPR_SYS_PRMS lpr_getsystem(void);
VOID lpr_setprms(LPR_USR_PRMS name, UINT32 val);
UINT32 lpr_getprms(LPR_USR_PRMS name);
BOOL lpr_proc_file(LPR_FILE_PRMS *prms);
BOOL lpr_proc_sensor(LPR_SENSOR_PRMS *prms);
BOOL lpr_get_result(LPR_RESULT *lpr_result);
char lpr_calibration_init(IPOINT *lp_corner_loc, int scale_deg);
UINT32 LPR_CalcBuffSize(void);

#endif  //_LPR_API_H
