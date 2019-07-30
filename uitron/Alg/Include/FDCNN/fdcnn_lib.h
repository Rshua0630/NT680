#ifndef FDCNN_LIB_H
#define FDCNN_LIB_H

#include    "Type.h"
#include    "GxImage.h"

#include    "FileSysTsk.h"
#include    "SxCmd.h"
#include    "SysKer.h"
#include    "string.h"
#include    "stdio.h"
#include    "NvtCv_lib.h"
#include    "NvtCvFuns.h"
#include    "cnn_lib.h"
#include 	"grph.h"
#include 	"NvtSvm.h"
#include 	"Perf.h"


#define FDCNN_STA_OK			1
#define FDCNN_STA_ERR			-1

#define FDCNN_SENSITIVITY_MAX 	128

#define FDCNN_MAX_OUTNUM 		150

typedef struct _fdcnn_result
{
	INT32 	class;
	INT32 	score;
	INT32 	x;
	INT32 	y;
	INT32 	w;
	INT32 	h;
	IPOINT 	landmark_points[5];//left_eye, right_eye, nose, left_mouth, right_mouth
}fdcnn_result;

typedef enum //FDCNN_NETWORK
{
    FDCNN_METHOD_1 = 0,
} fdcnn_network;

/**
     Config ID.

*/
typedef enum
{
    FDCNN_CFG_SENSITIVITY = 0,       ///< config FDCNN sensitivity, 0~FDCNN_SENSITIVITY_MAX, precision is improved but recall is reduced from 0 to 128.
	FDCNN_CFG_OUTNUM,                ///< config FDCNN output object number, 0~FDCNN_MAX_OUTNUM.
	ENUM_DUMMY4WORD(fdcnn_cfg)
} fdcnn_cfg;


UINT32 	fdcnn_init(MEM_RANGE *buf, MEM_RANGE *cachebuf);
void 	fdcnn_uninit(void);
UINT32 	fdcnn_calccachebuffsize(void);
UINT32 	fdcnn_calcbuffsize(void);
void    fdcnn_lock(BOOL block);
void 	fdcnn_config(fdcnn_cfg config_id, UINT32 value);
UINT32 	fdcnn_getconfig(fdcnn_cfg config_id);

void 	fdcnn_process(IMG_BUF *p_inputimg);

UINT32 	fdcnn_getresults(fdcnn_result *results, URECT* targetcoord);

void 	fdcnn_netselect(UINT32 sel);
UINT32 	fdcnn_loadmodel(void);



#endif

