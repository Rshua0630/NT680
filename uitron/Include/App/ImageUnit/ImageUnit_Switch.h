/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Switch.h
    @ingroup    mSwitch

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_SWITCH_H
#define IMAGEUNIT_SWITCH_H

/*
Support these port config API(s):

============================================================
port
------------------------------------------------------------
============================================================
*/

#include "Type.h"
#include "ImageStream.h"

#if defined(_BSP_NA51000_)
#define SWITCH_MAX_NUM	8
#endif
#if defined(_BSP_NA51023_)
#define SWITCH_MAX_NUM	2
#endif

enum {
	SWITCH_PARAM_START = 0x00008000,

	///< support these ISF_IN parameters
	SWITCH_PARAM_CONNECTTYPE = SWITCH_PARAM_START,//assign connect-type of INPUT port, default is ISF_CONNECT_PUSH
	SWITCH_PARAM_SYNC_FROM_OPORT,                //auto set "size. fmt, aspect" of INPUT port from "given" OUTPUT port
	SWITCH_PARAM_AUTO_BEGIN_IMM,		//ISF_CTRL: auto-connect  begin (clear all output & input)
	SWITCH_PARAM_AUTO_END_IMM,			//ISF_CTRL: auto-connect end (summary to all input, and auto connect work path)
	SWITCH_PARAM_AUTO_IMGSIZE_IMM,		//ISF_OUT/ISF_IN: get/set size
	SWITCH_PARAM_AUTO_IMGFMT_IMM,		//ISF_OUT/ISF_IN: get/set format
	SWITCH_PARAM_AUTO_FRC_IMM,			//ISF_OUT/ISF_IN: get/set frc
	SWITCH_PARAM_AUTO_RESULT_IMM,		//ISF_CTRL: get auto-connect result count (RO)
	SWITCH_PARAM_AUTO_RANGE_IMM,		//ISF_CTRL: set auto-connect mapping range
	SWITCH_PARAM_AUTO_CONNECT_IMM,		//ISF_OUT: get auto-connect mapping (RO)
	SWITCH_PARAM_MANUAL_BEGIN_IMM,		//ISF_CTRL: manual-connect begin
	SWITCH_PARAM_MANUAL_END_IMM,		//ISF_CTRL: manual-connect end
	SWITCH_PARAM_MANUAL_CONNECT_IMM,	//ISF_OUT: set manual-connect
};

#define SWITCH_IN_MAX		16
#define SWITCH_OUT_MAX	32

/*

[MANUAL-CONNECT]

	UINT32 i;

	ImageUnit_Begin(&ISF_Switch1, 0);
		ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_MANUAL_BEGIN_IMM, 0);
		for(i=0; i<32; i++) {
			ImageUnit_SetParam(ISF_OUT1+i, SWITCH_PARAM_MANUAL_CONNECT_IMM, iport);
		}
		ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_MANUAL_END_IMM, 0);
	ImageUnit_End();



[AUTO-CONNECT]

	UINT32 i, j, in_max;
	UINT32 in_imgsize, in_imgfmt, in_frc;
	UINT32 iport;

	//add oport spec of all possiable dest ports
	ImageUnit_Begin(&ISF_Switch1, 0);
		ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_AUTO_BEGIN_IMM, 0);
		for(i=0; i<32; i++) {
			ImageUnit_SetParam(ISF_OUT1+i, SWITCH_PARAM_AUTO_IMGSIZE_IMM, ISF_VDO_SIZE(w,h));
			ImageUnit_SetParam(ISF_OUT1+i, SWITCH_PARAM_AUTO_IMGFMT_IMM, ISF_VDO_FMT_YUV420);
			ImageUnit_SetParam(ISF_OUT1+i, SWITCH_PARAM_AUTO_FRC_IMM, 30);
			DBG_DUMP("SWITCH: out[%d] %d x %d . %d. %d\r\n", i, ISF_GET_HI(ipcam_out_table1[i]), ISF_GET_LO(ipcam_out_table1[i]), ipcam_out_table2[i], ipcam_out_table3[i]);
		}
		ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_AUTO_END_IMM, 0);
	ImageUnit_End();

	in_max = ImageUnit_GetParam(&ISF_Switch1, ISF_CTRL, SWITCH_PARAM_AUTO_RESULT_IMM);
	DBG_DUMP("SWITCH: total %d result\r\n", in_max);
	if(in_max > (SWITCH_IN_MAX/2)) {
		DBG_ERR("result is > %d, cannot support!\r\n", SWITCH_IN_MAX/2);
		return;
	}

	//get result of iport spec, and set spec to its src port
	ImageUnit_Begin(&ISF_Switch1, 0);
	for(j=0; j<in_max; j++) {
		//get result of 1st in group (1~8)
		in_imgsize = ImageUnit_GetParam(&ISF_Switch1, ISF_IN1+j, SWITCH_PARAM_AUTO_IMGSIZE_IMM);  //in_imgsize=ISF_VDO_SIZE(w,h)
		in_imgfmt = ImageUnit_GetParam(&ISF_Switch1, ISF_IN1+j, SWITCH_PARAM_AUTO_IMGFMT_IMM); //in_imgfmt=ISF_VDO_FMT_YUV420
		in_frc = ImageUnit_GetParam(&ISF_Switch1, ISF_IN1+j, SWITCH_PARAM_AUTO_FRC_IMM); //in_frc=30
		DBG_DUMP("SWITCH: in[%d] %d x %d . %d. %d\r\n", j, ISF_GET_HI(in_imgsize), ISF_GET_LO(in_imgsize), in_imgfmt, in_frc);
		//copy result to 2nd in group (9~16)
		ImageUnit_SetParam(ISF_IN9+j, SWITCH_PARAM_AUTO_IMGSIZE_IMM, in_imgsize);
		ImageUnit_SetParam(ISF_IN9+j, SWITCH_PARAM_AUTO_IMGFMT_IMM, in_imgfmt);
		ImageUnit_SetParam(ISF_IN9+j, SWITCH_PARAM_AUTO_FRC_IMM, in_frc);
		        :
	     (set these param(s) to its src port)
		        :
	}
	ImageUnit_End();

	//auto mapping and set iport connect (work path)
	ImageUnit_Begin(&ISF_Switch1, 0);
		ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_MANUAL_BEGIN_IMM, 0);
		for(i=0; i<16; i++) {
			ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_AUTO_RANGE_IMM, ISF_VDO_SIZE(ISF_IN1, ISF_IN8));
			iport = ImageUnit_GetParam(&ISF_Switch1, ISF_OUT1+i, SWITCH_PARAM_AUTO_CONNECT_IMM);
			if(iport != ISF_CTRL) {
				DBG_DUMP("SWITCH: connect in %d to out %d\r\n", iport-ISF_IN_BASE, i);
				ImageUnit_SetParam(ISF_OUT1+i, SWITCH_PARAM_MANUAL_CONNECT_IMM, iport);
			}
		}
		for(i=16+0; i<16+16; i++) {
			ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_AUTO_RANGE_IMM, ISF_VDO_SIZE(ISF_IN9, ISF_IN16));
			iport = ImageUnit_GetParam(&ISF_Switch1, ISF_OUT1+i, SWITCH_PARAM_AUTO_CONNECT_IMM);
			if(iport != ISF_CTRL) {
				DBG_DUMP("SWITCH: connect in %d to out %d\r\n", iport-ISF_IN_BASE, i);
				ImageUnit_SetParam(ISF_OUT1+i, SWITCH_PARAM_MANUAL_CONNECT_IMM, iport);
			}
		}
		ImageUnit_SetParam(ISF_CTRL, SWITCH_PARAM_MANUAL_END_IMM, 0);
	ImageUnit_End();

	
*/

#if (SWITCH_MAX_NUM >= 1)
extern ISF_UNIT ISF_Switch1;
#endif
#if (SWITCH_MAX_NUM >= 2)
extern ISF_UNIT ISF_Switch2;
#endif
#if (SWITCH_MAX_NUM >= 3)
extern ISF_UNIT ISF_Switch3;
#endif
#if (SWITCH_MAX_NUM >= 4)
extern ISF_UNIT ISF_Switch4;
#endif
#if (SWITCH_MAX_NUM >= 5)
extern ISF_UNIT ISF_Switch5;
#endif
#if (SWITCH_MAX_NUM >= 6)
extern ISF_UNIT ISF_Switch6;
#endif
#if (SWITCH_MAX_NUM >= 7)
extern ISF_UNIT ISF_Switch7;
#endif
#if (SWITCH_MAX_NUM >= 8)
extern ISF_UNIT ISF_Switch8;
#endif

#endif //IMAGEUNIT_SWITCH_H

