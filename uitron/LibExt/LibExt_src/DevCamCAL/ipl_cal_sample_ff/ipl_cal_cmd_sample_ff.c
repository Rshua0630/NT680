// system
#include "string.h"
#include "stdio.h"
#include "DxSensor.h"
#include "sensor.h"
#include "FileSysTsk.h"
// calibration
#include "ipl_calibration.h"
#include "ipl_cal_3a.h"
#include "ipl_cal_alg.h"
#include "ipl_cal_api.h"
#include "ipl_cal_buf.h"
#include "ipl_cal_ctrl.h"
#include "ipl_cal_rw.h"
#include "ipl_cal_cmd_sample_ff_int.h"
#include "ipl_cal_ctrl_sample_ff_int.h"
#include "ipl_cal_setting_sample_ff_int.h"
// ipl
#include "ae_api.h"
#include "awb_api.h"
#include "iq_api.h"
#include "pq_obj.h"

// for debug use
static BOOL b_ipl_cal_dbg_cmd_en_getrst = FALSE;
static BOOL b_ipl_cal_dbg_saveraw = FALSE;
// for internal use
static UINT8 y_value[65][65] = {0};
IPLC_BASEINFO g_base_info_addr = {0};
IPLC_EXT g_ext_info_addr = {0};
IPL_ISR_OBJ_INFO g_isr_info_addr = {0};
static ER ipl_cal_ext_get_data_buf_addr(CAL_DATA_BUF_INFO *buf_info, UINT32 *addr);

static ER initial_iniheader(IPL_PROC_BIT_ID id_bit, CAL_ITEM_BIT cal_item_bit, CAL_DATA_BUF_SEL buf_sel)
{
	ER rt;
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_BUF_ID buf_id_ini_header = CAL_BUF_ID_INIH_1, buf_id_data;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	MEM_RANGE header_mem;
	CAL_SETTING_INFO *setting_info;
	UINT32 cal_key_num = 0, cal_header_buf;
	UINT32 header_size, header_addr, ini_work_buf = ipl_ini_get_header_buf_size();
	UINT32 id_cnt, item_cnt, mode_cnt;
	UINT32 key_length = cal_rw()->fp_get_key_length(), val_length = cal_rw()->fp_get_val_length();

	if (cal_buf()->fp_isset_buf(buf_id_ini_header)) {
		//DBG_WRN("bset\r\n");
		//return E_OK;
		buf_id_data = cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info);
		cal_buf()->fp_release_buf(buf_id_data);
		cal_buf()->fp_release_buf(buf_id_ini_header);
	}

	/*
	    read ori storage header data to dram
	*/
	// get ori header size
	header_size = cal_rw()->fp_get_storage_header_size();
	// calc cal need header size
	switch (buf_sel) {
	case CAL_DATA_BUF_SEL_ITEM_ALL:
		for (id_cnt = 0; id_cnt < IPL_ID_MAX_NUM; id_cnt++) {
			if (!CAL_CHK_ID(id_bit, id_cnt)) {
				continue;
			}
			for (item_cnt = 0; item_cnt < CAL_ITEM_MAX_NUM; item_cnt++) {
				if (!CAL_CHK_ITEM(cal_item_bit, item_cnt)) {
					continue;
				}
				setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(item_cnt);
				if (setting_info == NULL) {
					DBG_ERR("%s get cal setting info error\r\n", cal_dbg_str_calitem(item_cnt));
				} else {
					for (mode_cnt = 0; mode_cnt < SENSOR_MODE_Num - 1; mode_cnt++) {
						if (setting_info->sen_mode[mode_cnt] == SENSOR_MODE_UNKNOWN) {
							break;
						}
						cal_key_num++;
					}
				}
			}
		}
		break;
	case CAL_DATA_BUF_SEL_STORAGE_ONLY:
		break;
//	case CAL_DATA_BUF_SEL_ITEM_MAX:
	default:
		DBG_ERR("buf_sel\r\n");
		break;
	}

	// set header buffer
	cal_header_buf = cal_key_num * (key_length + val_length) * 1.2;
	if (cal_buf()->fp_set_buf(buf_id_ini_header, ALIGN_CEIL_4(header_size + ini_work_buf + cal_header_buf)) != E_OK) {
		DBG_ERR("header\r\n");
		return E_SYS;
	} else {
		// read header from storage to dram
		if (cal_buf()->fp_get_buf(buf_id_ini_header, &header_addr, &header_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		}
		header_mem.Addr = header_addr;
		header_mem.Size = header_size;
		rt = cal_rw()->fp_header_open(header_mem);
		if ((rt != E_OK) && (rt != E_OBJ)) {
			cal_rt |= CAL_ER_RW;
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("initial_iniheader 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER chk_dp_storage_data(CAL_RW_HEADER_INFO *rw_header_info, UINT32 cur_data_buf_addr)
{
	ER rt;
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	UINT32 size;
	IPL_PROC_ID id;
	CAL_RW_DATA_INFO rw_data_info = {0};

	if (rw_header_info == NULL) {
		DBG_ERR("\r\n");
		return E_SYS;
	}

	id = rw_header_info->key.id;
	rw_data_info.sec = rw_header_info->sec;
	rw_data_info.key = rw_header_info->key;

	rt = cal_rw()->fp_header_read(rw_header_info, &size);
	if (rt == E_OBJ) {
		DBG_IND("null key or section\r\n");
		return E_OK;
	} else if (rt != E_OK) {
		cal_rt |= CAL_ER_RW;
	}

	switch (ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG)) {
	case CAL_ITEM_DP_TAG_BRIGHT:
		switch (rw_header_info->value.cal_status) {
		case CAL_STATUS_DP_D_OK:
			// get ori dp dark data from storage to dram
			if (cal_rw()->fp_storage_read(&rw_data_info, cur_data_buf_addr, rw_header_info->value.read_data_size) != E_OK) {
				cal_rt |= CAL_ER_RW;
			}
			break;
		case CAL_STATUS_OK:
		case CAL_STATUS_DP_B_OK:
		case CAL_STATUS_OVERFLOW:
		case CAL_STATUS_NONE:
		case CAL_STATUS_AE_TIMEOUT:
			break;
		default:
			//DBG_ERR("cal_status error 0x%x\r\n", rw_header_info->value.cal_status);
			//cal_rt |= CAL_ER_PAR;
			break;
		}
		break;
	case CAL_ITEM_DP_TAG_DARK:
		switch (rw_header_info->value.cal_status) {
		case CAL_STATUS_DP_B_OK:
			// get ori dp bright data from storage to dram
			if (cal_rw()->fp_storage_read(&rw_data_info, cur_data_buf_addr, rw_header_info->value.read_data_size) != E_OK) {
				cal_rt |= CAL_ER_RW;
			}
			break;
		case CAL_STATUS_OK:
		case CAL_STATUS_DP_D_OK:
		case CAL_STATUS_OVERFLOW:
		case CAL_STATUS_NONE:
		case CAL_STATUS_AE_TIMEOUT:
			break;
		default:
			//DBG_ERR("cal_status error 0x%x\r\n", rw_header_info->value.cal_status);
			//cal_rt |= CAL_ER_PAR;
			break;
		}
		break;
	case CAL_ITEM_DP_TAG_BRIGHT_ONLY:
		switch (rw_header_info->value.cal_status) {
		case CAL_STATUS_OK:
		case CAL_STATUS_DP_B_OK:
		case CAL_STATUS_OVERFLOW:
		case CAL_STATUS_DP_D_OK:
		case CAL_STATUS_NONE:
		case CAL_STATUS_AE_TIMEOUT:
			break;
		default:
			//DBG_ERR("cal_status error 0x%x\r\n", rw_header_info->value.cal_status);
			//cal_rt |= CAL_ER_PAR;
			break;
		}
		break;
	default:
		DBG_ERR("cal_item_dp_tag error 0x%x\r\n", ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG));
		cal_rt |= CAL_ER_PAR;
		break;
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}


static CAL_ER_TAG chk_dp_ori_header(IPL_PROC_ID id, UINT32 *dp_param_cnt, DP_PARAM_CHGFMT *dp_param_format)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_RW_HEADER_INFO rw_header_info = {0};
	UINT32 size;
	ER rt;

	// read INI header
	rw_header_info.sec.cal_item = CAL_ITEM_DP;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rt = cal_rw()->fp_header_read(&rw_header_info, &size);
	if (rt == E_OBJ) {
		DBG_IND("null key or section\r\n");
		return E_OK;
	} else if (rt != E_OK) {
		cal_rt |= CAL_ER_RW;
	}

	switch (ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG)) {
	case CAL_ITEM_DP_TAG_BRIGHT:
		switch (rw_header_info.value.cal_status) {
		case CAL_STATUS_OK:
		case CAL_STATUS_DP_B_OK:
		case CAL_STATUS_OVERFLOW:
		case CAL_STATUS_NONE:
		case CAL_STATUS_AE_TIMEOUT:
			*dp_param_cnt = 0;
			*dp_param_format = DP_PARAM_CHGFMT_SKIP;
			break;
		case CAL_STATUS_DP_D_OK:
			*dp_param_cnt = rw_header_info.value.data[0];
			*dp_param_format = DP_PARAM_CHGFMT_AUTO;
			break;
		default:
			DBG_ERR("cal_status error 0x%x\r\n", rw_header_info.value.cal_status);
			cal_rt |= CAL_ER_PAR;
			break;
		}
		break;
	case CAL_ITEM_DP_TAG_DARK:
		switch (rw_header_info.value.cal_status) {
		case CAL_STATUS_OK:
		case CAL_STATUS_DP_D_OK:
		case CAL_STATUS_OVERFLOW:
		case CAL_STATUS_NONE:
		case CAL_STATUS_AE_TIMEOUT:
			*dp_param_cnt = 0;
			*dp_param_format = DP_PARAM_CHGFMT_SKIP;
			break;
		case CAL_STATUS_DP_B_OK:
			*dp_param_cnt = rw_header_info.value.data[0];
			*dp_param_format = DP_PARAM_CHGFMT_AUTO;
			break;
		default:
			DBG_ERR("cal_status error 0x%x\r\n", rw_header_info.value.cal_status);
			cal_rt |= CAL_ER_PAR;
			break;
		}
		break;
	case CAL_ITEM_DP_TAG_BRIGHT_ONLY:
		switch (rw_header_info.value.cal_status) {
		case CAL_STATUS_OK:
		case CAL_STATUS_DP_B_OK:
		case CAL_STATUS_OVERFLOW:
		case CAL_STATUS_DP_D_OK:
		case CAL_STATUS_NONE:
		case CAL_STATUS_AE_TIMEOUT:
			*dp_param_cnt = 0;
			*dp_param_format = DP_PARAM_CHGFMT_AUTO;
			break;
			break;
		default:
			DBG_ERR("cal_status error 0x%x\r\n", rw_header_info.value.cal_status);
			cal_rt |= CAL_ER_PAR;
			break;
		}
		break;
	default:
		DBG_ERR("cal_item_dp_tag error 0x%x\r\n", ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG));
		cal_rt |= CAL_ER_PAR;
		break;
	}

	return cal_rt;
}

/**
    set (storage and cal_item) data buffer
    cal_item_bit: which cal item you want to do
                  not set specified "id_bit & cal_item_bit" for storage
                  set specified "id_bit & cal_item_bit" data buffer
    buffer: (ori storage data size, except specified "id_bit & cal_item_bit" )
          + (specified "id_bit & cal_item_bit"  (all, only max) largest used size)
*/
static ER initial_data_buf(IPL_PROC_BIT_ID id_bit, CAL_ITEM_BIT cal_item_bit, CAL_DATA_BUF_SEL buf_sel)
{
	ER rt = E_OK;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	CAL_BUF_ID buf_id_data;
	UINT32 id_cnt, item_cnt, ori_data_size = 0, cur_data_max_size = 0, data_addr, data_size, mode_cnt, data_buf_size;
	HEADER_SEC sec = {0};
	CAL_RW_DATA_INFO rw_data_info = {0};
	CAL_RW_HEADER_INFO rw_header_info = {0};
	CAL_SETTING_INFO *setting_info;

	buf_id_data = cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info);

	/*
	    get storage data, except cal_item_bit
	*/
	if (cal_buf()->fp_isset_buf(buf_id_data)) {
		cal_buf()->fp_release_buf(buf_id_data);
		//return E_OK;
	}

	// get ori storage data size
	switch (buf_sel) {
	case CAL_DATA_BUF_SEL_STORAGE_ONLY:
		ori_data_size = 0;
		break;
//	case CAL_DATA_BUF_SEL_ITEM_MAX:
	case CAL_DATA_BUF_SEL_ITEM_ALL:
		for (id_cnt = 0; id_cnt < IPL_ID_MAX_NUM; id_cnt++) {
			for (item_cnt = 0; item_cnt < CAL_ITEM_MAX_NUM; item_cnt++) {
				sec.cal_item = (CAL_ITEM)item_cnt;
				if ((CAL_CHK_ITEM(cal_item_bit, item_cnt)) && (CAL_CHK_ID(id_bit, id_cnt))) {
					continue;
				}
				ori_data_size += cal_rw()->fp_get_storage_data_size(id_cnt, sec);
			}
		}
		break;
	default:
		ori_data_size = 0;
		DBG_ERR("buf_sel\r\n");
		break;
	}

	// get data buffer addr for dp storage data
	rt = cal_buf()->fp_set_buf(buf_id_data, ori_data_size);
	rt |= cal_buf()->fp_get_buf(buf_id_data, &data_addr, &data_size);
	rt |= cal_buf()->fp_release_buf(buf_id_data);
	UINT32 cur_data_start_addr = data_addr + data_size;

	// set data buf
	switch (buf_sel) {
	case CAL_DATA_BUF_SEL_STORAGE_ONLY:
		rt = cal_buf()->fp_set_buf(buf_id_data, ori_data_size);
		rt |= cal_buf()->fp_get_buf(buf_id_data, &data_addr, &data_size);
		rt |= cal_buf()->fp_set_buf_item(buf_id_data, CAL_BUF_ID_DATA_ITEM_STORAGE, data_addr, ori_data_size);
		return rt;
	case CAL_DATA_BUF_SEL_ITEM_ALL:
		cur_data_max_size = 0;
		for (id_cnt = 0; id_cnt < IPL_ID_MAX_NUM; id_cnt++) {
			if (!CAL_CHK_ID(id_bit, id_cnt)) {
				continue;
			}
			for (item_cnt = 0; item_cnt < CAL_ITEM_MAX_NUM; item_cnt++) {
				setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(item_cnt);
				if (setting_info == NULL) {
					DBG_ERR("%s get cal setting info error\r\n", cal_dbg_str_calitem(item_cnt));
					continue;
				}
				if (!CAL_CHK_ITEM(cal_item_bit, item_cnt)) {
					continue;
				}
				for (mode_cnt = 0; mode_cnt < SENSOR_MODE_Num - 1; mode_cnt++) {
					if (setting_info->sen_mode[mode_cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					}
					rw_data_info.sec.cal_item = item_cnt;
					rw_data_info.key.id = id_cnt;
					rw_data_info.key.sensor_mode = setting_info->sen_mode[mode_cnt];
					rw_data_info.key.cal_cond = CAL_COND_DFT;
					data_buf_size = ipl_cal_ext_ctrl_sample()->fp_get_inidata_buf(&rw_data_info);
					if (data_buf_size != 0) {
						if (rw_data_info.sec.cal_item == CAL_ITEM_DP) {
							rw_header_info.sec = rw_data_info.sec;
							rw_header_info.key = rw_data_info.key;
							rt |= chk_dp_storage_data(&rw_header_info, cur_data_start_addr);
							cur_data_start_addr += rw_header_info.value.read_data_size;
						}
						rt |= cal_rw()->fp_header_set_dram_sft(&rw_data_info, ori_data_size + cur_data_max_size);
						cur_data_max_size += data_buf_size;
					}
				}
			}
		}
		rt = cal_buf()->fp_set_buf(buf_id_data, ori_data_size + cur_data_max_size);
		rt |= cal_buf()->fp_get_buf(buf_id_data, &data_addr, &data_size);
		rt |= cal_buf()->fp_set_buf_item(buf_id_data, CAL_BUF_ID_DATA_ITEM_STORAGE, data_addr, ori_data_size);
		rt |= cal_buf()->fp_set_buf_item(buf_id_data, CAL_BUF_ID_DATA_ITEM_CUR, data_addr + ori_data_size, cur_data_max_size);
		break;
//	case CAL_DATA_BUF_SEL_ITEM_MAX:
	default:
		DBG_ERR("buf_sel\r\n");
		break;
	}

	if (rt != E_OK) {
		DBG_ERR("cfg data buf error rt %d\r\n", rt);
	}

	return rt;
}

/**
    get data from storage to dram
    except specified "id_bit & cal_item_bit"
    specified "id_bit & cal_item_bit" wiil be delete
*/
static ER initial_data(IPL_PROC_BIT_ID id_bit, CAL_ITEM_BIT cal_item_bit)
{
	ER rt = E_OK;
	UINT32 item_cnt, data_buf_addr, data_buf_size, sft_data_addr = 0, header_size;
	BOOL b_next;
	HEADER_SEC sec = {0};
	HEADER_KEY key = {0};
	HEADER_VAL val = {0};
	CAL_RW_DATA_INFO data_info = {0};
	CAL_RW_HEADER_INFO header_info = {0};
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	CAL_BUF_ID buf_id_data;

	buf_id_data = cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info);
	rt = cal_buf()->fp_get_buf_item(buf_id_data, CAL_BUF_ID_DATA_ITEM_STORAGE, &data_buf_addr, &data_buf_size);

	for (item_cnt = 0; item_cnt < CAL_ITEM_MAX_NUM; item_cnt++) {
		sec.cal_item = item_cnt;
		b_next = cal_rw()->fp_search_sec_init(sec);
		while (b_next) {
			b_next = cal_rw()->fp_search_sec(&key, &val);
			header_info.sec = sec;
			header_info.key = key;
			if ((CAL_CHK_ID(id_bit, key.id)) && (CAL_CHK_ITEM(cal_item_bit, item_cnt))) {
					if (val.cal_status == CAL_STATUS_OK) {
						header_info.value.cal_status = CAL_STATUS_NONE;
						cal_rw()->fp_header_write(&header_info);	// avoid get raw sie get error dram data
					}
				continue;
			}
			// get data from storage to dram
			if (val.read_data_size != 0) {
				data_info.sec = sec;
				data_info.key = key;
				rt |= cal_rw()->fp_storage_read(&data_info, data_buf_addr + sft_data_addr, val.read_data_size);

				// updata header data sft info
				rt |= cal_rw()->fp_header_read(&header_info, &header_size);
				rt |= cal_rw()->fp_header_set_dram_sft(&data_info, sft_data_addr);

				sft_data_addr += val.read_data_size;
			}
		}
	}
	if (rt != E_OK) {
		DBG_ERR("rt %d\r\n", rt);
	}
	return rt;
}

static void ipl_cal_saveraw(UINT32 addr, UINT32 width, UINT32 height)
{
	static UINT32 picidx = 0;
	char    filename1[64];
	UINT32 size;
	FST_FILE    pFileHdl;

	size = width * height;

	snprintf(filename1, sizeof(filename1), "A:\\CAL_Mem%d_Addr_%x_W%d_H%d.RAW", picidx, addr, width, height);
	debug_msg("^BA:\\CAL_Mem%d_Addr_%x_W%d_H%d.RAW\r\n", picidx, addr, width, height);
	pFileHdl = FileSys_OpenFile(filename1, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
	FileSys_WriteFile(pFileHdl, (UINT8 *)addr, &size, 0, NULL);
	FileSys_CloseFile(pFileHdl);
	picidx ++;
}

static ER cal_ae_timeout_write_rst(IPL_PROC_ID id, CAL_ITEM cal_item)
{
	ER rt = E_OK;
	UINT32 i;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	/*
	    write alg result to dram (CAL_AE)
	*/
	rw_header_info.sec.cal_item = cal_item;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = CAL_STATUS_AE_TIMEOUT;
	for (i = 0; i < HEADER_VAL_MAX_DATA_NUM; i++) {
		rw_header_info.value.data[i] = 0;
	}
	rw_header_info.value.read_data_size = 0;
	rt = cal_rw()->fp_header_write(&rw_header_info);
	if (rt != E_OK) {
		DBG_ERR("write header error\r\n");
	}
	return rt;
}

static ER cal_getraw_fail_write_rst(IPL_PROC_ID id, CAL_ITEM cal_item)
{
	ER rt = E_OK;
	UINT32 i;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	/*
	    write alg result to dram
	*/
	rw_header_info.sec.cal_item = cal_item;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = CAL_STATUS_NONE;
	for (i = 0; i < HEADER_VAL_MAX_DATA_NUM; i++) {
		rw_header_info.value.data[i] = 0;
	}
	rw_header_info.value.read_data_size = 0;
	rt = cal_rw()->fp_header_write(&rw_header_info);
	if (rt != E_OK) {
		DBG_ERR("write header error\r\n");
	}
	return rt;
}

static CAL_ER_TAG cal_getraw_cfg_buf(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_RST *getraw_rst)
{
	ER rt = E_OK;
	UINT32 sie_buf_addr = 0, sie_buf_size = 0;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;
	CAL_GETRAW_BUF_SIZE getraw_buf_size = {0};

	buf_id_info.id = id;
	if (!cal_buf()->fp_isset_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info))) {
		// set raw buf
		rt = ipl_cal_ext_ctrl_sample()->fp_get_getraw_buf_size(id, getraw_param, &getraw_buf_size);
		rt |= cal_buf()->fp_set_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), getraw_buf_size.total_size);
		rt |= cal_buf()->fp_get_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), &sie_buf_addr, &sie_buf_size);
		// set raw buf item
		ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();
		rt |= ipl_cal_ext_ctrl_sample()->fp_set_getraw_buf_item(buf_id_info, ctrl_setting_info, sie_buf_addr, getraw_buf_size);
	} else {
		rt = cal_buf()->fp_get_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), &sie_buf_addr, &sie_buf_size);
	}

	getraw_rst->raw_info->pixel_addr = sie_buf_addr + IPL_HeaderRawGetBufSize();

	if (rt != E_OK) {
		DBG_ERR("id %d cal_getraw_cfg_buf error %d\r\n", id, rt);
		return CAL_ER_BUF;
	} else {
		return CAL_ER_NONE;
	}
}


static CAL_ER_TAG cal_getraw_cfg_param(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_CTRL_PARAM *getraw_ctrl_param, CAL_GETRAW_RST *getraw_rst, USIZE *raw_size)
{
	ER rt = E_OK;
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	IPLC_BASEINFO *base_info = &g_base_info_addr;
	IPLC_EXT *ext_info = &g_ext_info_addr;
	IPL_ISR_OBJ_INFO *isr_info = &g_isr_info_addr;

	// get getraw ctrl information
	rt = ipl_cal_set_info(id, CAL_SEL_GETRAW_SENMODE, getraw_param->sen_mode);
	rt |= ipl_cal_set_info(id, CAL_SEL_GETRAW_CALCOND, CAL_COND_DFT);
	if (rt != E_OK) {
		cal_rt |= CAL_ER_INFOR;
	}

	getraw_ctrl_param->p_base_info = base_info;
	getraw_ctrl_param->p_ext_info = ext_info;
	getraw_ctrl_param->p_isr_info = isr_info;
	getraw_ctrl_param->p_adapt_setting = NULL;//fp_adapt_setting;
	if (ipl_cal_ext_ctrl_sample()->fp_get_getraw_ctrl_param(id, getraw_param, getraw_ctrl_param) != E_OK) {
		cal_rt |= CAL_ER_PAR;
	}

	getraw_rst->raw_info->img_info = getraw_ctrl_param->p_base_info->SIE_Ch0_Img;

	raw_size->w = getraw_ctrl_param->p_base_info->SIE_Ch0_Img.width;
	raw_size->h = getraw_ctrl_param->p_base_info->SIE_Ch0_Img.height;

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	}

	return cal_rt;
}

static CAL_ER_TAG cal_getraw_group_cfg_buf(IPL_PROC_ID id, CAL_CTRL_SETTING_INFO *ctrl_setting_info, USIZE src_size, MEM_RANGE *group_mem)
{
	ER rt = E_OK;

	group_mem->Size = src_size.w * src_size.h * ctrl_setting_info->group->block.x * ctrl_setting_info->group->block.y;

	if (!cal_buf()->fp_isset_buf(CAL_BUF_ID_GETRAW_GROUP_1)) {
		rt = cal_buf()->fp_set_buf(CAL_BUF_ID_GETRAW_GROUP_1, group_mem->Size);
		rt |= cal_buf()->fp_get_buf(CAL_BUF_ID_GETRAW_GROUP_1, &group_mem->Addr, &group_mem->Size);
		memset((void *)group_mem->Addr, 0, (size_t)group_mem->Size);
	} else {
		rt = cal_buf()->fp_get_buf(CAL_BUF_ID_GETRAW_GROUP_1, &group_mem->Addr, &group_mem->Size);
	}

	if (rt != E_OK) {
		DBG_ERR("id %d cal_getraw_group_cfg_buf error %d\r\n", id, rt);
		return CAL_ER_BUF;
	} else {
		return CAL_ER_NONE;
	}
}

static CAL_ER_TAG cal_getraw_group(IPL_PROC_ID id, CAL_CTRL_SETTING_INFO *ctrl_setting_info, MEM_RANGE group_mem, UINT32 src_addr, USIZE src_size)
{
	UINT32 cnt, dist_buf_addr = 0;


	switch (id) {
	case IPL_ID_1:
		dist_buf_addr = group_mem.Addr;
		break;
	case IPL_ID_2:
		dist_buf_addr = group_mem.Addr + src_size.w;
		break;
	case IPL_ID_3:
		dist_buf_addr = group_mem.Addr + src_size.w * ctrl_setting_info->group->block.x * src_size.h;
		break;
	case IPL_ID_4:
		dist_buf_addr = group_mem.Addr + src_size.w * ctrl_setting_info->group->block.x * src_size.h + src_size.w;
		break;
	default:
		dist_buf_addr = group_mem.Addr;
		break;
	}

	for (cnt = 0; cnt < src_size.h; cnt++) {
		memcpy((void *)(dist_buf_addr + src_size.w * ctrl_setting_info->group->block.x * cnt), (void *)(src_addr + src_size.w * cnt), src_size.w);
	}


	return CAL_ER_NONE;
}

static CAL_ER_TAG cal_getraw_group_update_rst(CAL_CTRL_SETTING_INFO *ctrl_setting_info, UINT32 group_addr, CAL_GETRAW_RST *getraw_rst)
{
	getraw_rst->raw_info->pixel_addr = group_addr;
	getraw_rst->raw_info->img_info.line_offset *= ctrl_setting_info->group->block.x;
	getraw_rst->raw_info->img_info.width *= ctrl_setting_info->group->block.x;
	getraw_rst->raw_info->img_info.height *= ctrl_setting_info->group->block.y;

	return CAL_ER_NONE;
}

static CAL_ER_TAG cal_getraw_start(IPL_PROC_ID id, CAL_GETRAW_CTRL_PARAM *getraw_ctrl_param)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;

	// ipl ctrl run
	if (cal_ctrl()->fp_getraw(id, getraw_ctrl_param, TRUE) != E_OK) {
		cal_rt |= CAL_ER_IPL;
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
	}

	return cal_rt;
}

static CAL_ER_TAG cal_getraw_ae_convert(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_RST *getraw_rst)
{
	ER rt = E_OK;
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_SETTING_INFO *setting_info;
	CAL_AE_PARAM ae_convert_input = {0}, ae_convert_ouput = {0};
	CAL_AE_CONVERGE_PARAM ae_converge_param = {0}, ae_converge_param_setting = {0};
	CAL_AE_CONVERGE_RST ae_converge_rst = {0};

	// get cal ae parameter
	setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(getraw_param->cal_item);
	if (setting_info == NULL) {
		DBG_ERR("get setting info error\r\n");
		return E_SYS;
	}

	if (setting_info->ae_converge_param != NULL) {
		ae_converge_param_setting = *setting_info->ae_converge_param;
		if (ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG) == CAL_ITEM_DP_TAG_DARK) {
			if (setting_info->ae_converge_param->pnext != NULL) {
				ae_converge_param_setting = *setting_info->ae_converge_param->pnext;
			}
		}
		ae_convert_input.gain_ratio = ae_converge_param_setting.gain_ratio;
		ae_convert_input.exp_time = ae_converge_param_setting.exp_time;
		ae_convert_input.lum = ae_converge_param_setting.target_lum;

		// cal ae converge
		rt = cal_3a()->fp_ae()->fp_open(id, NULL);
		rt |= cal_3a()->fp_ae()->fp_convert_param(id, &ae_convert_input, &ae_convert_ouput, ae_converge_param_setting.adjust, getraw_param->cal_item);
		ae_converge_param.gain_ratio = ae_convert_ouput.gain_ratio;
		ae_converge_param.exp_time = ae_convert_ouput.exp_time;
		ae_converge_param.target_lum = ae_converge_param_setting.target_lum;
		ae_converge_param.tolerance = ae_converge_param_setting.tolerance;
		ae_converge_param.times = ae_converge_param_setting.times;
		ae_converge_param.adjust = ae_converge_param_setting.adjust;
		ae_converge_param.coverge_step = ae_converge_param_setting.coverge_step;

		/*
			set ipl timeout
			#define FLOW_TIMEOUT_MS     500  //ms
			500ms / 3 = 166666 us
		*/
		UINT32 ipl_timeout;
		ipl_timeout = (ae_converge_param.exp_time > 166666) ? ae_converge_param.exp_time : 166666;
		IPL_Ctrl_Set_Timeout_MS(ipl_timeout / 200);	// * 5(VD) / 1000

		rt |= cal_3a()->fp_ae()->fp_wait_converge(id, &ae_converge_param, &ae_converge_rst);
		rt |= cal_3a()->fp_ae()->fp_close(id);
		if (rt != E_OK) {
			cal_rt |= CAL_ER_3A;
		}
	} else {
		DBG_ERR("cal_item %d ae_converge_param is NULL!! (skip ae converge)\r\n", getraw_param->cal_item);
		cal_rt |= CAL_ER_3A;
		ae_converge_rst.b_converge = FALSE;
		ae_converge_rst.converge_lum = 0;
		ae_converge_rst.gain_ratio[0] = 0;
		ae_converge_rst.exp_time[0] = 0;
	}
	getraw_rst->ae_converge_rst = ae_converge_rst;

	rt = IPL_Wait_Vd((1 << id), TRUE, ae_converge_param.exp_time);
	rt |= IPL_Wait_Vd((1 << id), TRUE, ae_converge_param.exp_time);
	rt |= IPL_Wait_Vd((1 << id), TRUE, ae_converge_param.exp_time);
	if (rt != E_OK) {
		cal_rt |= CAL_ER_IPL;
	}

	return cal_rt;
}

static CAL_ER_TAG cal_getraw_stop(IPL_PROC_ID id, CAL_GETRAW_CTRL_PARAM *getraw_ctrl_param)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	if (cal_ctrl()->fp_getraw(id, getraw_ctrl_param, FALSE) != E_OK) {
		cal_rt |= CAL_ER_IPL;
	}
	return cal_rt;
}

static CAL_ER_TAG cal_alg_dp_get_setting(IPL_PROC_ID id, CAL_ALG_DP_SETTING dp_setting, DP_SETTING *dp_param)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	switch (ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG)) {
	case CAL_ITEM_DP_TAG_BRIGHT:
	case CAL_ITEM_DP_TAG_BRIGHT_ONLY:
		*dp_param = dp_setting.bright_set;
		break;
	case CAL_ITEM_DP_TAG_DARK:
		*dp_param = dp_setting.dark_set;
		break;
	default:
		DBG_ERR("cal_item_dp_tag error 0x%x, force to %d\r\n", ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG), CAL_ITEM_DP_TAG_BRIGHT);
		*dp_param = dp_setting.bright_set;
		cal_rt |= CAL_ER_PAR;
		break;
	}

	return cal_rt;
}

static CAL_ER_TAG cal_alg_dp_get_maxdpcnt(IPL_PROC_ID id, SENSOR_MODE sensor_mode, CAL_SETTING_INFO *setting_info, CAL_ALG_DP_SETTING dp_setting, UINT32 *max_dp_cnt)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	UINT32 sen_mode_cnt, senmode_size;
	SEN_STATUS_PARAM sen_status = {0};

	for (sen_mode_cnt = 0; sen_mode_cnt < SENSOR_MODE_Num; sen_mode_cnt++) {
		if (setting_info->sen_mode[sen_mode_cnt] == sensor_mode) {
			sen_status.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
			if (sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status) != E_OK) {
				DBG_ERR("get sensor status error\r\n");
				cal_rt |= CAL_ER_SEN;
			} else {
				if (sen_status.sensor_info.mode == NULL) {
					DBG_ERR("get sensor status error\r\n");
					cal_rt |= CAL_ER_SEN;
				} else if ((sen_status.sensor_info.mode->trans_vd[0] == NULL) || (sen_status.sensor_info.mode->trans_hd[0] == NULL)) {
					DBG_ERR("get sensor status error\r\n");
					cal_rt |= CAL_ER_SEN;
				} else {
					senmode_size = (sen_status.sensor_info.mode->trans_vd[0]->data_size * sen_status.sensor_info.mode->trans_hd[0]->data_size);
					*max_dp_cnt = (senmode_size / 10000) * dp_setting.max_cnt_ratio[sen_mode_cnt];
				}
			}
			break;
		}
		if (setting_info->sen_mode[sen_mode_cnt] == SENSOR_MODE_UNKNOWN) {
			DBG_ERR("sensor mode %d get setting infor error\r\n", ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE));
			cal_rt |= CAL_ER_SETTING;
			break;
		}
	}
	return cal_rt;
}

static CAL_ER_TAG cal_alg_awb_write_header(IPL_PROC_ID id, CAL_SETTING_INFO *setting_info, CAL_ALG_AWB_PARAM awb_param, CAL_ALG_AWB_RST *awb_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	// write ini header
	rw_header_info.sec.cal_item = CAL_ITEM_AWB;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = ipl_cal_ext_ctrl_sample()->fp_chk_alg_rst(id
									  , rw_header_info.key.sensor_mode, CAL_ITEM_AWB
									  , (void *)awb_rst, setting_info->alg);
	rw_header_info.value.data[0] = awb_rst->rgain;
	rw_header_info.value.data[1] = awb_rst->bgain;
	rw_header_info.value.read_data_size = 0;
	if (cal_rw()->fp_header_write(&rw_header_info)) {
		cal_rt |= CAL_ER_RW;
	}
	return cal_rt;
}

static CAL_ER_TAG cal_alg_dp_write_header(IPL_PROC_ID id, CAL_SETTING_INFO *setting_info, CAL_ALG_DP_PARAM dp_param, CAL_ALG_DP_RST *dp_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	// write ini header
	rw_header_info.sec.cal_item = CAL_ITEM_DP;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = ipl_cal_ext_ctrl_sample()->fp_chk_alg_rst(id
									  , rw_header_info.key.sensor_mode, CAL_ITEM_DP
									  , (void *)dp_rst, setting_info->alg);

	if (dp_param.b_chg_dp_format == DP_PARAM_CHGFMT_SKIP) {
		if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
			switch (ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG)) {
			case CAL_ITEM_DP_TAG_BRIGHT:
				rw_header_info.value.cal_status = CAL_STATUS_DP_B_OK;
				break;
			case CAL_ITEM_DP_TAG_DARK:
				rw_header_info.value.cal_status = CAL_STATUS_DP_D_OK;
				break;
			case CAL_ITEM_DP_TAG_BRIGHT_ONLY:
				rw_header_info.value.cal_status = CAL_STATUS_OK;
				break;
			default:
				DBG_ERR("cal_item_dp_tag error 0x%x\r\n", ipl_cal_get_info(id, CAL_SEL_ALG_DP_TAG));
				cal_rt |= CAL_ER_PAR;
				break;
			}

		}
	}

	rw_header_info.value.data[0] = dp_rst->pixel_cnt;
	if (dp_rst->pixel_cnt == 0) {
		rw_header_info.value.read_data_size = 0;
	} else {
		rw_header_info.value.read_data_size = dp_rst->data_length;
	}

	if (cal_rw()->fp_header_write(&rw_header_info) != E_OK) {
		cal_rt |= CAL_ER_RW;
	}
	return cal_rt;
}

static CAL_ER_TAG cal_alg_ecs_write_header(IPL_PROC_ID id, CAL_SETTING_INFO *setting_info, CAL_ALG_ECS_PARAM ecs_param, CAL_ALG_ECS_RST *ecs_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_RW_HEADER_INFO rw_header_info = {0};
	UINT32 max_tbl_h = 65, max_tbl_v = 65;

	// write ini header
	rw_header_info.sec.cal_item = CAL_ITEM_ECS;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = ipl_cal_ext_ctrl_sample()->fp_chk_alg_rst(id
									  , rw_header_info.key.sensor_mode, CAL_ITEM_ECS
									  , (void *)ecs_rst, setting_info->alg);
	rw_header_info.value.data[0] = (UINT32)ecs_rst->ecs_over_spec;
	ECS_MAP_SIZE(ecs_param.setting.map_size, &max_tbl_h);
	ECS_MAP_SIZE(ecs_param.setting.map_size, &max_tbl_v);
	rw_header_info.value.read_data_size = sizeof(UINT32) * max_tbl_h * max_tbl_v;
	if (cal_rw()->fp_header_write(&rw_header_info)) {
		cal_rt |= CAL_ER_RW;
	}
	return cal_rt;
}


static CAL_ER_TAG cal_alg_iso_write_header(IPL_PROC_ID id, CAL_SETTING_INFO *setting_info, CAL_ALG_ISO_PARAM iso_param, CAL_ALG_ISO_RST *iso_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	// write ini header
	rw_header_info.sec.cal_item = CAL_ITEM_ISO;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = ipl_cal_ext_ctrl_sample()->fp_chk_alg_rst(id
									  , rw_header_info.key.sensor_mode, CAL_ITEM_ISO
									  , (void *)iso_rst, setting_info->alg);
	rw_header_info.value.data[0] = iso_rst->gain_ratio;
	rw_header_info.value.read_data_size = 0;
	if (cal_rw()->fp_header_write(&rw_header_info)) {
		cal_rt |= CAL_ER_RW;
	}
	return cal_rt;
}

static CAL_ER_TAG cal_alg_lenscen_write_header(IPL_PROC_ID id, CAL_SETTING_INFO *setting_info, CAL_ALG_LENSCEN_PARAM lenscen_param, CAL_ALG_LENSCEN_RST *lenscen_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	// write ini header
	rw_header_info.sec.cal_item = CAL_ITEM_LENSCEN;
	rw_header_info.key.id = id;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rw_header_info.key.cal_cond = ipl_cal_get_info(id, CAL_SEL_GETRAW_CALCOND);
	rw_header_info.value.cal_status = ipl_cal_ext_ctrl_sample()->fp_chk_alg_rst(id
									  , rw_header_info.key.sensor_mode, CAL_ITEM_LENSCEN
									  , (void *)lenscen_rst, setting_info->alg);
	rw_header_info.value.data[0] = lenscen_rst->reliability;
	rw_header_info.value.data[1] = lenscen_rst->center_sft.x;
	rw_header_info.value.data[2] = lenscen_rst->center_sft.y;
	rw_header_info.value.read_data_size = 0;
	if (cal_rw()->fp_header_write(&rw_header_info)) {
		cal_rt |= CAL_ER_RW;
	}
	return cal_rt;
}

static ER cal_alg_awb_sample(IPL_PROC_ID id, IPL_RAW_IMG *raw_info, CAL_SETTING_INFO *setting_info, void *alg_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_ALG_AWB_PARAM awb_param = {0};
	CAL_ALG_AWB_RST *awb_rst;
	UINT32 header_addr = 0, header_size = 0;

	/*
	    get awb parameter & awb alg
	*/
	awb_param.raw_info = raw_info;
	awb_param.setting = *(CAL_ALG_AWB_SETTING *)(setting_info->alg);
	awb_rst = (CAL_ALG_AWB_RST *)alg_rst;
	if (cal_alg()->fp_awb_alg(&awb_param, awb_rst) != E_OK) {
		cal_rt |= CAL_ER_ALG;
	}

	/*
	    write awb alg result
	*/
	if (!(cal_rt & CAL_ER_ALG)) {
		if (cal_buf()->fp_get_buf(CAL_BUF_ID_INIH_1, &header_addr, &header_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			cal_rt |= cal_alg_awb_write_header(id, setting_info, awb_param, awb_rst);
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}
static ER cal_alg_dp_sample(IPL_PROC_ID id, IPL_RAW_IMG *raw_info, CAL_SETTING_INFO *setting_info, void *alg_rst, UINT32 data_addr)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_ALG_DP_PARAM dp_param = {0};
	CAL_ALG_DP_RST *dp_rst;
	UINT32 header_addr = 0, header_size = 0;
	CAL_ALG_DP_SETTING dp_setting = {0};

	/*
	    check ori data
	*/
	cal_rt |= chk_dp_ori_header(id, &dp_param.ori_dp_cnt, &dp_param.b_chg_dp_format);

	/*
	    get dp parameter & dp alg
	*/
	dp_param.raw_info = raw_info;
	dp_setting = *(CAL_ALG_DP_SETTING *)(setting_info->alg);
	cal_rt |= cal_alg_dp_get_setting(id, dp_setting, &dp_param.setting);
	cal_rt |= cal_alg_dp_get_maxdpcnt(id, ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE), setting_info, dp_setting, &dp_param.max_dp_cnt);

	dp_param.dp_pool_addr = data_addr;
	dp_param.sie_act_str.x = ipl_cal_get_info(id, CAL_SEL_GETRAW_SIE_ACT_STR_X);
	dp_param.sie_act_str.y = ipl_cal_get_info(id, CAL_SEL_GETRAW_SIE_ACT_STR_Y);
	dp_param.sie_crp_str.x = ipl_cal_get_info(id, CAL_SEL_GETRAW_SIE_CRP_STR_X);
	dp_param.sie_crp_str.y = ipl_cal_get_info(id, CAL_SEL_GETRAW_SIE_CRP_STR_Y);
	dp_rst = (CAL_ALG_DP_RST *)alg_rst;
	if (cal_alg()->fp_dp_alg(&dp_param, dp_rst) != E_OK) {
		cal_rt |= CAL_ER_ALG;
	}

	/*
	    write dp alg result
	*/
	if (!(cal_rt & CAL_ER_ALG)) {
		if (cal_buf()->fp_get_buf(CAL_BUF_ID_INIH_1, &header_addr, &header_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			cal_rt |= cal_alg_dp_write_header(id, setting_info, dp_param, dp_rst);
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER cal_alg_ecs_sample(IPL_PROC_ID id, IPL_RAW_IMG *raw_info, CAL_SETTING_INFO *setting_info, void *alg_rst, UINT32 data_addr)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_ALG_ECS_PARAM ecs_param = {0};
	CAL_ALG_ECS_RST *ecs_rst;
	UINT32 header_addr = 0, header_size = 0;

	/*
	    get ecs parameter & ecs alg
	*/
	ecs_param.raw_info = raw_info;
	ecs_param.setting = *(CAL_ALG_ECS_SETTING *)(setting_info->alg);
	ecs_param.ecs_tbl_addr = data_addr;
	ecs_param.y_tbl_addr = (UINT32)(&y_value[0][0]);
	ecs_rst = (CAL_ALG_ECS_RST *)alg_rst;
	if (cal_alg()->fp_ecs_alg(&ecs_param, ecs_rst) != E_OK) {
		cal_rt |= CAL_ER_ALG;
	}

	/*
	    write ecs alg result
	*/
	if (!(cal_rt & CAL_ER_ALG)) {
		if (cal_buf()->fp_get_buf(CAL_BUF_ID_INIH_1, &header_addr, &header_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			cal_rt |= cal_alg_ecs_write_header(id, setting_info, ecs_param, ecs_rst);
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER cal_alg_iso_sample(IPL_PROC_ID id, CAL_GETRAW_RST *getraw_rst, CAL_SETTING_INFO *setting_info, void *alg_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_ALG_ISO_PARAM iso_param = {0};
	CAL_ALG_ISO_RST *iso_rst;
	UINT32 header_addr = 0, header_size = 0;

	/*
	    get iso parameter & iso alg
	*/
	iso_param.raw_info = getraw_rst->raw_info;
	iso_param.setting = *(CAL_ALG_ISO_SETTING *)(setting_info->alg);
	iso_param.ae_converge_rst = getraw_rst->ae_converge_rst;
	iso_rst = (CAL_ALG_ISO_RST *)alg_rst;
	if (cal_alg()->fp_iso_alg(&iso_param, iso_rst) != E_OK) {
		cal_rt |= CAL_ER_ALG;
	}

	/*
	    write iso alg result
	*/
	if (!(cal_rt & CAL_ER_ALG)) {
		if (cal_buf()->fp_get_buf(CAL_BUF_ID_INIH_1, &header_addr, &header_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			cal_rt |= cal_alg_iso_write_header(id, setting_info, iso_param, iso_rst);
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER cal_alg_lenscen_sample(IPL_PROC_ID id, IPL_RAW_IMG *raw_info, CAL_SETTING_INFO *setting_info, void *alg_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_ALG_LENSCEN_PARAM lenscen_param = {0};
	CAL_ALG_LENSCEN_RST *lenscen_rst;
	UINT32 header_addr = 0, header_size = 0;

	/*
	    get lenscen parameter & lenscen alg
	*/
	lenscen_param.raw_info = raw_info;
	lenscen_param.setting = *(CAL_ALG_LENSCEN_SETTING *)(setting_info->alg);
	lenscen_rst = (CAL_ALG_LENSCEN_RST *)alg_rst;
	if (cal_alg()->fp_lenscen_alg(&lenscen_param, lenscen_rst) != E_OK) {
		cal_rt |= CAL_ER_ALG;
	}

	/*
	    write lenscen alg result
	*/
	if (!(cal_rt & CAL_ER_ALG)) {
		if (cal_buf()->fp_get_buf(CAL_BUF_ID_INIH_1, &header_addr, &header_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			cal_rt |= cal_alg_lenscen_write_header(id, setting_info, lenscen_param, lenscen_rst);
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER ipl_cal_ext_open(IPL_PROC_ID id, CAL_OPEN_PARAM *open_param)
{
	ER rt = E_OK;
	SENSOR_HANDLE sen_handle = {0};
	IQS_HANDLE iqs_handle = {0};

	/*
	    open sensor
	*/
	if (!sensor_is_open(IPL_UTI_CONV2_SEN_ID(id))) {
		drv_sensor_power_turn_on(IPL_UTI_CONV2_SEN_ID(id));
		drv_sensor_init(IPL_UTI_CONV2_SEN_ID(id), &sen_handle);
		if (sen_handle.drv_tab != NULL) {
			rt = sensor_open(IPL_UTI_CONV2_SEN_ID(id), &sen_handle);
			if (rt != E_OK) {
				DBG_ERR("sensor_open id %d error %d\r\n", IPL_UTI_CONV2_SEN_ID(id), rt);
				return rt;
			}
		} else {
			DBG_ERR("get sensor drv_tab error\r\n");
			return E_SYS;
		}
	} else {
		DBG_IND("sensor id %d is open\r\n", IPL_UTI_CONV2_SEN_ID(id));
	}

	/*
	    initial ae & awb & iq
	*/
	if(pqs_iq_init(id, &iqs_handle)){
		if (iqs_handle._iqs_param_init != NULL) {
			iqs_handle._iqs_param_init(id);
		} else {
			DBG_ERR("init _iqs_param_init error\r\n");
			return E_SYS;
		}
	}else{
		DBG_ERR("init _iqs_param_init error\r\n");
		return E_SYS;
	}

	{
		UINT32 awb_init = 0;
		UINT32 ae_init = 0;
		pqs_awb_init(id, &awb_init);
		awb_param_init(id, &awb_init);
		pqs_ae_init(id, &ae_init);
		ae_param_init(id, &ae_init);
	}

	/*
	    initial ini header & ini data buffer & get storage data
	*/
	rt = initial_iniheader(CAL_IPL_ID2BIT(id), open_param->cal_item_bit, CAL_DATA_BUF_SEL_ITEM_ALL);
	rt |= initial_data_buf(CAL_IPL_ID2BIT(id), open_param->cal_item_bit, CAL_DATA_BUF_SEL_ITEM_ALL);
	rt |= initial_data(CAL_IPL_ID2BIT(id), open_param->cal_item_bit);

	// open ipl cal ctrl
	rt |= cal_ctrl()->fp_open(id);
	if (rt != E_OK) {
		return rt;
	}

	return rt;
}

static ER ipl_cal_ext_close(IPL_PROC_ID id)
{
	ER rt[32];
	UINT32 rt_cnt = 0, cnt;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};

	// close ipl cal ctrl
	rt[rt_cnt++] = cal_ctrl()->fp_close(id);


	// close sensor
	if (sensor_is_open(IPL_UTI_CONV2_SEN_ID(id))) {
		rt[rt_cnt++] = sensor_close(IPL_UTI_CONV2_SEN_ID(id));
	} else {
		DBG_IND("sensor id %d is close\r\n", IPL_UTI_CONV2_SEN_ID(id));
	}
	drv_sensor_power_turn_off(IPL_UTI_CONV2_SEN_ID(id));

	// release prv buf
	buf_id_info.id = id;
	if (cal_buf()->fp_isset_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info))) {
		ipl_cal_ext_ctrl_sample()->fp_display_close(id);
		rt[rt_cnt++] = cal_buf()->fp_release_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info));
	}

	/*
	    write data to storage
	*/
	UINT32 data_addr, data_size;
	rt[rt_cnt++] = cal_buf()->fp_get_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info), &data_addr, &data_size);
	rt[rt_cnt++] = cal_rw()->fp_storege_update_dram_data(data_addr);

	// header close (write ini header to storage)
	rt[rt_cnt++] = cal_rw()->fp_header_close();
	for (cnt = CAL_BUF_ID_INID_1; cnt < CAL_BUF_ID_INID_MAX; cnt++) {
		if (cal_buf()->fp_isset_buf(cnt)) {
			cal_buf()->fp_release_buf(cnt);
		}
	}

	// release ini header & data buf
	for (cnt = CAL_BUF_ID_INIH_1; cnt < CAL_BUF_ID_INIH_MAX; cnt++) {
		if (cal_buf()->fp_isset_buf(cnt)) {
			cal_buf()->fp_release_buf(cnt);
		}
	}

	if (ipl_cal_chk_rt(&rt[0], rt_cnt) != 0) {
		DBG_ERR("ipl_cal_ext_close error 0x%.8x\r\n", ipl_cal_chk_rt(&rt[0], rt_cnt));
		return E_SYS;
	}

	return E_OK;
}

static ER ipl_cal_ext_prv(IPL_PROC_ID id, CAL_PRV_PARAM *prv_param)
{
	ER rt = E_OK;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	IPL_MODE_INFOR info = {0};
	UINT32 ipl_buf_size, ime_buf_size, cal_prv_buf_size, cal_prv_buf_addr;// cal_prv_buf_size = ipl_buf_size +  ime_buf_size

	display_id = id;

	info.id = id;
	IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&info);
	if (info.mode == IPL_MODE_PREVIEW) {
		return E_OK;
	}

	// set buffer information
	if (ipl_cal_ext_ctrl_sample()->fp_get_prv_buf_size(id, prv_param, &ipl_buf_size, &ime_buf_size) != E_OK) {
		return E_SYS;
	}

	buf_id_info.id = id;
	if (!cal_buf()->fp_isset_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info))) {
		rt = cal_buf()->fp_set_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info), ipl_buf_size + ime_buf_size);
		rt |= cal_buf()->fp_get_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info), &cal_prv_buf_addr, &cal_prv_buf_size);
		rt |= cal_buf()->fp_set_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info), CAL_BUF_ID_PRV_ITEM_IPL_1, cal_prv_buf_addr, ipl_buf_size);
		rt |= cal_buf()->fp_set_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info), CAL_BUF_ID_PRV_ITEM_IME2_1, cal_prv_buf_addr + ipl_buf_size, ime_buf_size);
	} else {
		rt = cal_buf()->fp_get_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info), &cal_prv_buf_addr, &cal_prv_buf_size);
	}
	if (rt != E_OK) {
		return E_SYS;
	}

#if 0	// avoid ipl wait ime fmd timeout error
	GAIN_PARAM gain_param = {0};
	EXPOSURE_PARAM exp_param = {0};

	/*
		set expusue setting & gain
		avoid get raw long expt, causing prv timeout
	*/
	exp_param.sensor_mode = SENSOR_MODE_CUR;
	exp_param.exp_time[0] = 33333;
	rt |= sensor_get_info((SENSOR_ID)id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
	gain_param.sensor_mode = SENSOR_MODE_CUR;
	gain_param.gain_ratio[0] = 200;
	rt |= sensor_get_info((SENSOR_ID)id, SEN_GET_GAIN_SETTING, (void *)&gain_param);

	rt |= sensor_set_info((SENSOR_ID)id, SEN_SET_EXPT, (void *)&exp_param.exp_setting);
	rt |= sensor_set_info((SENSOR_ID)id, SEN_SET_GAIN, (void *)&gain_param.gain_setting);

	Delay_DelayMs(1000);	// wait sensor output, need to set last get raw expt

#endif

	// set preview information
	prv_param->buf_addr = cal_prv_buf_addr;
	prv_param->buf_size = cal_prv_buf_size - ime_buf_size;
	prv_param->isr_cb_fp[IPL_ISR_IME] = ipl_cal_ext_ctrl_sample()->fp_get_prv_isr_event_fp(IPL_ISR_IME);
	prv_param->out_buf_cfg_cb_fp = ipl_cal_ext_ctrl_sample()->fp_get_prv_output_buf_cfg_fp(id, IPL_HAL_IME);
	if (cal_ctrl()->fp_prv(id, prv_param) != E_OK) {
		return E_SYS;
	}

	return rt;
}

static ER ipl_cal_ext_getraw(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_RST *getraw_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_GETRAW_CTRL_PARAM getraw_ctrl_param = {0};
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;
	MEM_RANGE group_mem = {0};
	USIZE raw_size = {0};
	UINT32 i;

	ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();
	if (ctrl_setting_info->group->bit_id == IPL_BIT_ID_NONE) {
		// cfg getraw param & buffer
		cal_rt |= cal_getraw_cfg_buf(id, getraw_param, getraw_rst);
		cal_rt |= cal_getraw_cfg_param(id, getraw_param, &getraw_ctrl_param, getraw_rst, &raw_size);
		// start getraw
		cal_rt |= cal_getraw_start(id, &getraw_ctrl_param);
		cal_rt |= cal_getraw_ae_convert(id, getraw_param, getraw_rst);
		cal_rt |= cal_getraw_stop(id, &getraw_ctrl_param);
	} else {
		for (i = IPL_ID_1; i < IPL_ID_MAX_NUM; i++) {
			if (CAL_IPL_ID2BIT(i) & ctrl_setting_info->group->bit_id) {
				// cfg getraw param & buffer
				cal_rt |= cal_getraw_cfg_buf(i, getraw_param, getraw_rst);
				cal_rt |= cal_getraw_cfg_param(i, getraw_param, &getraw_ctrl_param, getraw_rst, &raw_size);
				cal_rt |= cal_getraw_group_cfg_buf(i, ctrl_setting_info, raw_size, &group_mem);

				// start getraw
				cal_rt |= cal_getraw_start(i, &getraw_ctrl_param);
				cal_rt |= cal_getraw_ae_convert(i, getraw_param, getraw_rst);
				cal_rt |= cal_getraw_stop(i, &getraw_ctrl_param);

				cal_rt |= cal_getraw_group(i, ctrl_setting_info, group_mem, getraw_rst->raw_info->pixel_addr, raw_size);
			}
		}
		cal_rt |= cal_getraw_group_update_rst(ctrl_setting_info, group_mem.Addr, getraw_rst);
	}

	/* update last output dram addr */
	if (getraw_ctrl_param.out.dram0_addr != 0) {
		getraw_rst->raw_info->pixel_addr = getraw_ctrl_param.out.dram0_addr;
	} else {
		DBG_ERR("getraw_ctrl_param.out.dram0_addr error\r\n");
	}

	// save raw in SD card for debug
	if (b_ipl_cal_dbg_saveraw) {
		ipl_cal_saveraw(getraw_rst->raw_info->pixel_addr, getraw_rst->raw_info->img_info.width, getraw_rst->raw_info->img_info.height);
	}

	getraw_rst->rt = cal_rt;
	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		if (cal_rt == CAL_ER_IPL) {	// need get raw to release raw buf
			return E_OK;
		} else {
			return E_SYS;
		}
	} else {
		return E_OK;
	}
}


static CAL_ER_TAG cal_release_getraw_buf(void)
{
	//ER rt = E_OK;
	const UINT32 tbl_size = 2;
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	UINT32 i, j, addr, size, buf_id_map, max_buf_id_map = 0, min_buf_id_map = CAL_BUF_ID_MAX_NUM, release_cnt = 0;
	UINT32 buf_id_map_tbl[30][tbl_size];

	// search release buffer order
	for (j = CAL_BUF_ID_GETRAW_1; j < CAL_BUF_ID_GETRAW_MAX; j++) {
		if (cal_buf()->fp_isset_buf(j)) {
			cal_buf()->fp_cal_get_buf_info(j, &addr, &size, &buf_id_map);
			if (buf_id_map > max_buf_id_map) {
				max_buf_id_map = buf_id_map;
			}
			if (buf_id_map < min_buf_id_map) {
				min_buf_id_map = buf_id_map;
			}
			buf_id_map_tbl[release_cnt][0] = j; // CAL_BUF_ID
			buf_id_map_tbl[release_cnt][1] = buf_id_map; // buf_id_map

			release_cnt++;
		}
	}

	for (j = CAL_BUF_ID_GETRAW_GROUP_1; j < CAL_BUF_ID_GETRAW_GROUP_MAX; j++) {
		if (cal_buf()->fp_isset_buf(j)) {
			cal_buf()->fp_cal_get_buf_info(j, &addr, &size, &buf_id_map);
			if (buf_id_map > max_buf_id_map) {
				max_buf_id_map = buf_id_map;
			}
			if (buf_id_map < min_buf_id_map) {
				min_buf_id_map = buf_id_map;
			}
			buf_id_map_tbl[release_cnt][0] = j; // CAL_BUF_ID
			buf_id_map_tbl[release_cnt][1] = buf_id_map; // buf_id_map

			release_cnt++;
		}
	}

	// release buffer
	for (j = 0; j < release_cnt; j++) {
		for (i = 0; i < (sizeof(buf_id_map_tbl) / (sizeof(UINT32)*tbl_size)); i++) {
			if (buf_id_map_tbl[i][1] == max_buf_id_map) {
				cal_buf()->fp_release_buf(buf_id_map_tbl[i][0]);
				max_buf_id_map--;
				break;
			}
		}
	}
	/*if (rt != E_OK) {
	    cal_rt |= CAL_ER_BUF;
	}*/

	return cal_rt;
}

static ER ipl_cal_ext_alg(IPL_PROC_ID id, CAL_ALG_PARAM *alg_param, void *alg_rst)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_SETTING_INFO *setting_info;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	BOOL raw_input = FALSE;
	UINT32 data_addr;

	if (cal_ctrl()->fp_alg(id) != E_OK) {
		cal_rt |= CAL_ER_IPL;
	}

	if (alg_param->getraw_rst != NULL) {
		raw_input = TRUE;
	}

	/* check getraw rst */
	if (raw_input) {
		if (alg_param->getraw_rst->rt != CAL_ER_NONE) {
			DBG_ERR("get raw fail, skip alg\r\n");
			// release getraw buf
			buf_id_info.id = id;
			if (cal_buf()->fp_isset_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info))) {
				cal_buf()->fp_release_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info));
			}
			return cal_getraw_fail_write_rst(id, alg_param->cal_item);
		}
	}

	/* check ae rst */
	if (raw_input) {
		if (alg_param->getraw_rst->ae_converge_rst.b_converge != TRUE) {
			DBG_ERR("this RAW AE does not converge (lum %d gain ratio %d expt %d)\r\n"
					, alg_param->getraw_rst->ae_converge_rst.converge_lum
					, alg_param->getraw_rst->ae_converge_rst.gain_ratio[0]
					, alg_param->getraw_rst->ae_converge_rst.exp_time[0]);
			// release getraw buf
			buf_id_info.id = id;
			if (cal_buf()->fp_isset_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info))) {
				cal_buf()->fp_release_buf(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info));
			}
			return cal_ae_timeout_write_rst(id, alg_param->cal_item);
		}
	}

	setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(alg_param->cal_item);
	if (setting_info == NULL) {
		DBG_ERR("cal item %d get setting infor error %d\r\n", alg_param->cal_item);
		return E_SYS;
	}

	switch (alg_param->cal_item) {
	case CAL_ITEM_AWB:
		if (raw_input) {
			if (cal_alg_awb_sample(id, alg_param->getraw_rst->raw_info, setting_info, alg_rst) != E_OK) {
				cal_rt |= CAL_ER_ALG;
			}
		} else {
			DBG_ERR("please input raw information for %s\r\n", cal_dbg_str_calitem(alg_param->cal_item));
			cal_rt |= CAL_ER_PAR;
		}
		break;
	case CAL_ITEM_DP:
		if (raw_input) {
			data_addr = alg_param->res[0];
			if (cal_alg_dp_sample(id, alg_param->getraw_rst->raw_info, setting_info, alg_rst, data_addr) != E_OK) {
				cal_rt |= CAL_ER_ALG;
			}
		} else {
			DBG_ERR("please input raw information for %s\r\n", cal_dbg_str_calitem(alg_param->cal_item));
			cal_rt |= CAL_ER_PAR;
		}
		break;
	case CAL_ITEM_ECS:
		if (raw_input) {
			data_addr = alg_param->res[0];
			if (cal_alg_ecs_sample(id, alg_param->getraw_rst->raw_info, setting_info, alg_rst, data_addr) != E_OK) {
				cal_rt |= CAL_ER_ALG;
			}
		} else {
			DBG_ERR("please input raw information for %s\r\n", cal_dbg_str_calitem(alg_param->cal_item));
			cal_rt |= CAL_ER_PAR;
		}
		break;
	case CAL_ITEM_ISO:
		if (raw_input) {
			if (cal_alg_iso_sample(id, alg_param->getraw_rst, setting_info, alg_rst) != E_OK) {
				cal_rt |= CAL_ER_ALG;
			}
		} else {
			DBG_ERR("please input raw information for %s\r\n", cal_dbg_str_calitem(alg_param->cal_item));
			cal_rt |= CAL_ER_PAR;
		}
		break;
	case CAL_ITEM_LENSCEN:
		if (raw_input) {
			if (cal_alg_lenscen_sample(id, alg_param->getraw_rst->raw_info, setting_info, alg_rst) != E_OK) {
				cal_rt |= CAL_ER_ALG;
			}
		} else {
			DBG_ERR("please input raw information for %s\r\n", cal_dbg_str_calitem(alg_param->cal_item));
			cal_rt |= CAL_ER_PAR;
		}
		break;
	case CAL_ITEM_USER1:
	case CAL_ITEM_USER2:
	case CAL_ITEM_USER3:
	case CAL_ITEM_USER4:
	case CAL_ITEM_USER5:
		DBG_ERR("%s error (if you want to use user define, please add cal alg)\r\n", cal_dbg_str_calitem(alg_param->cal_item));
		cal_rt |= CAL_ER_PAR;
		break;
	default:
		DBG_ERR("error %s\r\n", cal_dbg_str_calitem(alg_param->cal_item));
		cal_rt |= CAL_ER_PAR;
		break;
	}

	// release getraw buf
	cal_rt |= cal_release_getraw_buf();
	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER ipl_cal_ext_get_data_buf_addr(CAL_DATA_BUF_INFO *buf_info, UINT32 *addr)
{
	ER rt = E_OK;
	UINT32 sft, data_addr, data_size;
	CAL_RW_DATA_INFO data_info = {0};
	CAL_GET_BUF_ID_INFO buf_id_info = {0};

	rt = cal_buf()->fp_get_buf(
			 cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info)
			 , &data_addr, &data_size);

	if (rt != E_OK) {
		DBG_ERR("buf\r\n");
		*addr = 0;
		return rt;
	}

	data_info.sec.cal_item = buf_info->cal_item;
	data_info.key.id = buf_info->id;
	data_info.key.sensor_mode = buf_info->sen_mode;
	data_info.key.cal_cond = buf_info->cal_cond;
	rt = cal_rw()->fp_header_get_dram_sft(&data_info, &sft);

	if (rt == E_OK) {
		*addr = data_addr + sft;
	} else if (rt == E_OBJ) {
		*addr = 0;
		DBG_IND("not assign buf\r\n");
	} else {
		*addr = 0;
		DBG_ERR("data_addr 0x%.8x sft %d\r\n", data_addr, sft);
	}
	return rt;
}

UINT32 cal_data_buf_used_size = 0;
static void ipl_cal_set_data_buf_used_size(UINT32 size)
{
	cal_data_buf_used_size += size;
}

static UINT32 ipl_cal_get_data_buf_used_size(void)
{
	return cal_data_buf_used_size;
}

typedef struct {
	BOOL b_set;
	IPL_PROC_ID id;
	CAL_ITEM cal_item;
	UINT32 sft;
} CAL_DATA_BUF_SFT;
#define CAL_DATA_BUF_CNT 20
static CAL_DATA_BUF_SFT cal_data_buf_sft[CAL_DATA_BUF_CNT] = {0};

static void ipl_cal_set_data_buf_sft(IPL_PROC_ID id, CAL_ITEM cal_item, UINT32 size)
{
	UINT32 i;

	for (i = 0; i < CAL_DATA_BUF_CNT; i++) {
		if ((cal_data_buf_sft[i].b_set) && (cal_data_buf_sft[i].id == id) && (cal_data_buf_sft[i].cal_item == cal_item)) {
			return;
		}
		if (!cal_data_buf_sft[i].b_set) {
			cal_data_buf_sft[i].b_set = TRUE;
			cal_data_buf_sft[i].id = id;
			cal_data_buf_sft[i].cal_item = cal_item;
			cal_data_buf_sft[i].sft = ipl_cal_get_data_buf_used_size();
			ipl_cal_set_data_buf_used_size(size);
			return;
		}
	}
	DBG_ERR("fail\r\n");
}

static UINT32 ipl_cal_get_data_buf_sft(IPL_PROC_ID id, CAL_ITEM cal_item)
{
	UINT32 i;

	for (i = 0; i < CAL_DATA_BUF_CNT; i++) {
		if ((cal_data_buf_sft[i].b_set) && (cal_data_buf_sft[i].id == id) && (cal_data_buf_sft[i].cal_item == cal_item)) {
			return cal_data_buf_sft[i].sft;
		}
	}
	DBG_ERR("fail\r\n");
	return 0;
}

static ER ipl_cal_getcalrst_sample(CAL_RW_HEADER_INFO *rw_header_info, UINT32 *data_addr)
{
	ER rt = E_OK;
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 size = 0, ini_data_addr = 0, ini_data_item_size = 0;
	CAL_RW_DATA_INFO rw_data_info = {0};

	/*
	    initial ini header & set ini data buffer
	*/
	rt = initial_iniheader(CAL_IPL_ID2BIT(rw_header_info->key.id), CAL_ITEM_BIT_NONE, CAL_DATA_BUF_SEL_STORAGE_ONLY);
	rt |= initial_data_buf(CAL_IPL_ID2BIT(rw_header_info->key.id), CAL_ITEM_BIT_NONE, CAL_DATA_BUF_SEL_STORAGE_ONLY);
	if (rt != E_OK) {
		DBG_ERR("initial error, skip get cal rst\r\n");
		return E_SYS;
	}

	/*
	    read INI header from dram
	*/
	rt = cal_rw()->fp_header_read(rw_header_info, &size);
	if (rt == E_OK) {
	} else if (rt == E_OBJ) {
		DBG_IND("NULL\r\n");
		rw_header_info->value.cal_status = CAL_STATUS_NONE;
		if (cal_rw()->fp_header_close() != E_OK) {
			cal_rt |= CAL_ER_RW;
		}
		return E_OK;
	} else {
		cal_rt |= CAL_ER_RW;
	}
	if (b_ipl_cal_dbg_cmd_en_getrst) {
		debug_msg("^M%s:%s %s id%d mode%d cond%d data(%d,%d,%d,%d,%d) size%d\r\n", __func__
				  , cal_dbg_str_calitem(rw_header_info->sec.cal_item)
				  , cal_dbg_str_calstatus(rw_header_info->value.cal_status)
				  , rw_header_info->key.id, rw_header_info->key.sensor_mode, rw_header_info->key.cal_cond
				  , rw_header_info->value.data[0], rw_header_info->value.data[1], rw_header_info->value.data[2], rw_header_info->value.data[3], rw_header_info->value.data[4]
				  , rw_header_info->value.read_data_size);
	}

	/*
	    read INI data from storage to dram, if exist
	*/
	if ((rw_header_info->value.read_data_size != 0) && ((cal_rt & CAL_ER_BUF) == 0)) {
		buf_id_info.id = rw_header_info->key.id;
		if (cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info)
									   , CAL_BUF_ID_DATA_ITEM_STORAGE, &ini_data_addr, &ini_data_item_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			rw_data_info.sec.cal_item = rw_header_info->sec.cal_item;
			rw_data_info.key.id = rw_header_info->key.id;
			rw_data_info.key.sensor_mode = rw_header_info->key.sensor_mode;
			rw_data_info.key.cal_cond = rw_header_info->key.cal_cond;

			ipl_cal_set_data_buf_sft(rw_data_info.key.id, rw_data_info.sec.cal_item, ipl_cal_ext_ctrl_sample()->fp_get_inidata_buf(&rw_data_info));
			ini_data_addr += ipl_cal_get_data_buf_sft(rw_data_info.key.id, rw_data_info.sec.cal_item);

			if (cal_rw()->fp_storage_read(&rw_data_info, ini_data_addr, rw_header_info->value.read_data_size) != E_OK) {
				cal_rt |= CAL_ER_RW;
			}
			if (b_ipl_cal_dbg_cmd_en_getrst) {
				debug_msg("^M%s:data addr 0x%x = 0x%x\r\n", __func__, ini_data_addr, *(UINT32 *)ini_data_addr);
			}
			if (data_addr != NULL) {
				*data_addr = ini_data_addr;
			}/* else {
                DBG_ERR("data size %d != 0, but data_addr is NULL\r\n", rw_header_info->value.read_data_size);
            }*/
		}
	}

	if (cal_rw()->fp_header_close() != E_OK) {
		cal_rt |= CAL_ER_RW;
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER ipl_cal_getcalrst_sample_incal(CAL_RW_HEADER_INFO *rw_header_info, UINT32 *data_addr)
{
	ER rt;
	CAL_RW_DATA_INFO rw_data_info = {0};
	UINT32 sft, buf_addr, buf_size, tmp_size, ini_data_addr = 0;

	// get header info from dram
	rt = cal_rw()->fp_header_read(rw_header_info, &tmp_size);
	if (rt == E_OBJ) {
		DBG_IND("null key or section\r\n");
		return E_OK;
	} else if (rt == E_OK) {
		if (rw_header_info->value.cal_status == CAL_STATUS_OK) {
			// get data info from dram
			rw_data_info.sec = rw_header_info->sec;
			rw_data_info.key = rw_header_info->key;
			rt = cal_rw()->fp_header_get_dram_sft(&rw_data_info, &sft);
			if (rt == E_OK) {
				CAL_GET_BUF_ID_INFO buf_id_info = {0};
				rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info)
												 , CAL_BUF_ID_DATA_ITEM_STORAGE, &buf_addr, &buf_size);
				ini_data_addr = buf_addr + sft;
			}
		}
	}

	if (data_addr != NULL) {
		*data_addr = ini_data_addr;
	}

	return rt;
}

static ER ipl_cal_get_ext_open(UINT32 id, void *open_param)
{
	return ipl_cal_ext_open((IPL_PROC_ID)id, (CAL_OPEN_PARAM *)open_param);
}

static ER ipl_cal_get_ext_close(UINT32 id)
{
	return ipl_cal_ext_close((IPL_PROC_ID)id);
}
static ER ipl_cal_get_ext_prv(UINT32 id, void *prv_param)
{
	return ipl_cal_ext_prv((IPL_PROC_ID)id, (CAL_PRV_PARAM *)prv_param);
}
static ER ipl_cal_get_ext_getraw(UINT32 id, void *getraw_param, void *getraw_rst)
{
	return ipl_cal_ext_getraw((IPL_PROC_ID)id, (CAL_GETRAW_PARAM *)getraw_param, (CAL_GETRAW_RST *)getraw_rst);
}
static ER ipl_cal_get_ext_alg(UINT32 id, void *alg_param, void *alg_rst)
{
	return ipl_cal_ext_alg((IPL_PROC_ID)id, (CAL_ALG_PARAM *)alg_param, alg_rst);
}
static ER ipl_cal_get_ext_get_data_buf_addr(void *buf_info, UINT32 *addr)
{
	return ipl_cal_ext_get_data_buf_addr((CAL_DATA_BUF_INFO *)buf_info, addr);
}
static ER ipl_cal_get_ext_getcalrst(void *rw_header_info, UINT32 *data_addr)
{
	return ipl_cal_getcalrst_sample((CAL_RW_HEADER_INFO *)rw_header_info, data_addr);
}
static ER ipl_cal_get_ext_getcalrst_incal(void *rw_header_info, UINT32 *data_addr)
{
	return ipl_cal_getcalrst_sample_incal((CAL_RW_HEADER_INFO *)rw_header_info, data_addr);
}
static void *ipl_cal_get_ext_ctrl(void)
{
	return (void *)ipl_cal_ext_ctrl_sample();
}
static void *ipl_cal_get_ext_setting(void)
{
	return (void *)ipl_cal_ext_setting_sample();
}

static IPL_CAL_EXT_LIB_TAB g_p_ipl_cal_lib_sample = {
	ipl_cal_get_ext_open,
	ipl_cal_get_ext_close,
	ipl_cal_get_ext_prv,
	ipl_cal_get_ext_getraw,
	ipl_cal_get_ext_alg,
	ipl_cal_get_ext_get_data_buf_addr,
	ipl_cal_get_ext_getcalrst,
	ipl_cal_get_ext_getcalrst_incal,
	ipl_cal_get_ext_ctrl,
	ipl_cal_get_ext_setting,
};

#if 0
- extern
#endif
// naming need to match ModelConfig_XX.txt
IPL_CAL_EXT_LIB_TAB *IPL_CAL_SAMPLE_FF_EXT_LIB_TAB(void)
{
	return &g_p_ipl_cal_lib_sample;
}
