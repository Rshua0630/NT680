/*
    Copyright   Novatek Microelectronics Corp. 2017~2026.  All rights reserved.

    @file       ImageUnit_Dummy.h
    @note       Nothing.

    @date       2017/03/31
*/

#ifndef IMAGEUNIT_DUMMY_H
#define IMAGEUNIT_DUMMY_H

/*
Support these input port(s): IN1

Support these output port(s): OUT1


*/


#include "Type.h"
#include "ImageStream.h"



enum {
	DUMMY_PARAM_UNIT_TYPE_IMM = 0,
};


typedef enum {
	DUMMY_UNIT_TYPE_LEAF = 0,
	DUMMY_UNIT_TYPE_ROOT,
	ENUM_DUMMY4WORD(DUMMY_UNIT_TYPE)
} DUMMY_UNIT_TYPE;



extern ISF_UNIT ISF_Dummy;

#endif //IMAGEUNIT_DUMMY_H

