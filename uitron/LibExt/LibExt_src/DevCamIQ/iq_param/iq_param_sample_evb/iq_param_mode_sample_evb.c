#ifndef _IQ_PARAM_MODE_SAMPLE_EVB_C
#define _IQ_PARAM_MODE_SAMPLE_EVB_C

#include "iq_param_common_sample_evb.h"

static SENSOR_MODE_PARAM mode_table[IQS_PARAM_TABLE_MAX]= {
    NULL,
	iq_param_sample_evb_table1_init, //IQS_PARAM_TABLE1
    iq_param_sample_evb_table2_init, //IQS_PARAM_TABLE2
    NULL,

};


static iq_param_table mode2tbl[SENSOR_MODE_Num] ={
	IQS_PARAM_TABLE_UNKONWN, // SENSOR_MODE_UNKNOWN
	IQS_PARAM_TABLE_1,  //SENSOR_MODE_1
	IQS_PARAM_TABLE_2,  //SENSOR_MODE_2
	IQS_PARAM_TABLE_1,  //SENSOR_MODE_3
	IQS_PARAM_TABLE_1,  //SENSOR_MODE_4
	IQS_PARAM_TABLE_1,  //SENSOR_MODE_5
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_6
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_7
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_8
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_9
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_10
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_11
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_12
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_13
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_14
	IQS_PARAM_TABLE_UNKONWN, //SENSOR_MODE_15


};

void iq_param_init_sample_evb(UINT32 Id);

void iq_param_init_sample_evb(UINT32 Id)
{
    UINT32 SenId_Value = Id; // Used before IPL start, error if =IQS_GetSensorIdx(Id)
	UINT32 idx;

	for (idx = IQS_PARAM_TABLE_1; mode_table[idx].init_func!=NULL; idx++)
		mode_table[idx].init_func(SenId_Value,idx);

	iq_set_mode2tbl(Id, mode2tbl);
	//DBG_ERR("%s %s\r\n",_IQS1_,_SENSOR1_);
}
#endif
