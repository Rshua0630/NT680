#include "modelext_info.h"
#include "model_cfg.h"

char ind_model_cfg_data[] __attribute__((section("modelext_data.model_cfg"))) = _MODEL_CFG_STR_;

MODELEXT_HEADER ind_model_cfg_header __attribute__((section("modelext_header.model_cfg"))) = {
	.size = sizeof(ind_model_cfg_data) + sizeof(MODELEXT_HEADER),
	.type = MODELEXT_TYPE_MODEL_CFG,
	.number = sizeof(ind_model_cfg_data),
	.version = MODEL_CFG_VER,
};
