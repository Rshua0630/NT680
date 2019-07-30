#ifndef _SYS_SENSOR
#define _SYS_SENSOR

#include "Type.h"
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_ImagePipe.h"

extern INT32     uiSensorMapping[VDOIN_MAX_NUM];

#define IPL_PATH_1                      SENSOR_MAP_1
#define IPL_PATH_2                      SENSOR_MAP_2
#define IPL_PATH_3                      SENSOR_MAP_3
#define IPL_PATH_4                      SENSOR_MAP_4
#define IPL_PATH_5                      SENSOR_MAP_5
#define IPL_PATH_6                      SENSOR_MAP_6
#define IPL_PATH_7                      SENSOR_MAP_7
#define IPL_PATH_8                      SENSOR_MAP_8

#define IPL_PATH(id) uiSensorMapping[id]

#define SENSOR_MAX_NUM					4 // max 4 sensors

extern void System_InstallSensorcmd(void);

extern ER   System_ChangeSensorName(SENSOR_ID id, CHAR *repalce_name);

#endif
