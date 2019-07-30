#include "modelext_info.h"

#define __mkstring(_x) #_x
#define __mkxstring(_x) __mkstring(_x)

MODELEXT_INFO ind_modelext_info_data __attribute__((section("modelext_data.modelext_info"))) = {
	.name = {'M', 'O', 'D', 'E', 'L', 'E', 'X', 'T'},
	.chip_name = _CHIP_NAME_,
	.version = "00000001",
	.date = __mkxstring(_BUILD_DATE_),
};

MODELEXT_HEADER ind_modelext_info_header __attribute__((section("modelext_header.modelext_info"))) = {
	.size = sizeof(ind_modelext_info_data) + sizeof(MODELEXT_HEADER),
	.type = MODELEXT_TYPE_INFO,
	.number = 1,
	.version = MODELEXT_INFO_VER,
};
