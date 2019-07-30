/**
    IPL dzoom table sim.

    @file       ipl_dzoom_tab_sim_ff.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "ipl_sim_ff_int.h"

static IPL_DZOOM_SEL_INFO sensor_mode_dzoom_table[] = {
//special case for sensor auto gen
	//{SENSOR_MODE_1,61,8,0,0,IPL_CROP},
};
const static UINT32 VDOZOOM_INFOR_MODE_1_TABLE[20][DZOOM_ITEM_MAX] = {
	//{2056, 1544, 2056, 1544, 2056, 1544, 2056, 1544}, //1
	//{1920, 1440, 640, 480, 640, 480, 640, 480},       //1
	{1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
	//{2688, 1508, 2112, 1188, 2112, 1188, 2688, 1508}, //1
	//{3840, 2160, 3840, 2160, 3840, 2160, 3840, 2160}, //1
	//{3840, 2160, 1920, 1080, 1920, 1080, 1920, 1080}, //1
	//{2560,  720, 1280,  720, 1280,  720, 2560,  720}, //1 for AHD

	{2608, 1472, 2112, 1188, 2112, 1188, 2608, 1472}, //2
	{2544, 1424, 2112, 1188, 2112, 1188, 2544, 1424}, //3
	{2480, 1392, 2112, 1188, 2112, 1188, 2480, 1392}, //4
	{2416, 1360, 2112, 1188, 2112, 1188, 2416, 1360}, //5
	{2352, 1312, 2112, 1188, 2112, 1188, 2352, 1312}, //6
	{2272, 1280, 2112, 1188, 2112, 1188, 2272, 1280}, //7
	{2208, 1232, 2112, 1188, 2112, 1188, 2208, 1232}, //8
	{2144, 1200, 2112, 1188, 2112, 1188, 2144, 1200}, //9
	{2112, 1188, 2112, 1188, 2112, 1188, 2080, 1168}, //10
	{2112, 1188, 2112, 1188, 2016, 1120, 2016, 1120}, //11
	{2112, 1188, 2112, 1188, 1936, 1088, 1936, 1088}, //12
	{2112, 1188, 2112, 1188, 1872, 1056, 1872, 1056}, //13
	{2112, 1188, 2112, 1188, 1808, 1008, 1808, 1008}, //14
	{2112, 1188, 2112, 1188, 1744,  976, 1744,  976}, //15
	{2112, 1188, 2112, 1188, 1680,  944, 1680,  944}, //16
	{2112, 1188, 2112, 1188, 1600,  896, 1600,  896}, //17
	{2112, 1188, 2112, 1188, 1536,  864, 1536,  864}, //18
	{2112, 1188, 2112, 1188, 1472,  816, 1472,  816}, //19
	{2112, 1188, 2112, 1188, 1408,  784, 1408,  784}, //20
};

UINT32 *SenMode2TblFp_sim_ff(UINT32 id, UINT32 *DzMaxidx)
{
	UINT32 *Ptr = NULL;
	UINT32 tbl_size,i;
	ER rt = E_OK;
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 sen_mode;

	sen_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return NULL;
	}

	//special case for manual select dzoom table
	switch (sen_mode) {
	case SENSOR_MODE_1:
	case SENSOR_MODE_2:
		Ptr = NULL;
		*DzMaxidx = 0;
//		Ptr = (UINT32 *)&VDOZOOM_INFOR_MODE_1_TABLE[0][0];
//		*DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
		break;

	default:
		Ptr = NULL;
		*DzMaxidx = 0;
		break;
	}

	tbl_size = sizeof(sensor_mode_dzoom_table) / sizeof(IPL_DZOOM_SEL_INFO);
	//auto gen dzoom table
	if(tbl_size == 0){
		//use default setting
		Ptr = ChkSenModeTbl(id, &sen_status.sensor_info, Ptr, DzMaxidx, NULL);
	} else {
		for(i=0;i<tbl_size;i++){
			if(sensor_mode_dzoom_table[i].mode == sen_mode){
				Ptr = ChkSenModeTbl(id, &sen_status.sensor_info, Ptr, DzMaxidx, &sensor_mode_dzoom_table[i]);
			} else {
				//use default setting
				Ptr = ChkSenModeTbl(id, &sen_status.sensor_info, Ptr, DzMaxidx, NULL);
			}
		}
	}
	return Ptr;
}
