/**
    Buf sample.

    @file       ipl_ext_buffer.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "ipl_ext_common.h"

CHAR *IPL_PrvBufNameTab[IPLBUF_PRV_MAX_CNT] = {
	"SIE_H_1",
	"SIE_H_2",
	"SIE_H_3",
	"SIE_H_4",
	"SIE_H_5",
	"SIE_H_6",
	"SIE_H_7",
	"SIE_H_8",
	"",
	"SIE_CH0_1",
	"SIE_CH0_2",
	"SIE_CH0_3",
	"SIE_CH0_4",
	"SIE_CH0_5",
	"SIE_CH0_6",
	"SIE_CH0_7",
	"SIE_CH0_8",
	"",
	"SIE_CH1_1",
	"SIE_CH1_2",
	"SIE_CH1_3",
	"SIE_CH1_4",
	"SIE_CH1_5",
	"SIE_CH1_6",
	"SIE_CH1_7",
	"SIE_CH1_8",
	"",
	"SIE_CH2_1",
	"SIE_CH2_2",
	"SIE_CH2_3",
	"SIE_CH2_4",
	"SIE_CH2_5",
	"SIE_CH2_6",
	"SIE_CH2_7",
	"SIE_CH2_8",
	"",
	"SIE_CH3_1",
	"SIE_CH3_2",
	"SIE_CH3_3",
	"SIE_CH3_4",
	"SIE_CH3_5",
	"SIE_CH3_6",
	"SIE_CH3_7",
	"SIE_CH3_8",
	"",
	"SIE_CH4_1",
	"SIE_CH4_2",
	"SIE_CH4_3",
	"",
	"SIE_CH5_1",
	"SIE_CH5_2",
	"SIE_CH5_3",
	"SIE_CH5_4",
	"SIE_CH5_5",
	"SIE_CH5_6",
	"SIE_CH5_7",
	"SIE_CH5_8",
	"",
	"RHE_DEFOG_SUB_OUT_1",
	"RHE_DEFOG_SUB_OUT_2",
	"",
	"IPE_ETH_1",
	"IPE_ETH_2",
	"",
	"IME_SUB_OUT_1",
	"IME_SUB_OUT_2",
	"",
	"IME_PRI_MASK_1",
	"IME_PRI_MASK_2",
	"",
	"IMEP1_SQUARE_MV_1",
	"IMEP1_SQUARE_MV_2",
	"",
	"IMEP1_SQUARE_MO_1",
	"IMEP1_SQUARE_MO_1",
};


CHAR *IPL_CapBufNameTab[IPLBUF_CAP_MAX_CNT] = {
	"DRE_DBGBUF"
};

#define IPPBUF_WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 4)
#define IPPBUF_4WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 16)
#define IPPBUF_8WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 32)

extern IPLBUF_RST IPL_BufAssignFlowStream(IPLBUF_ENG2DRAMINFO *data);
extern IPLBUF_RST IPL_BufAssignFlowCap(IPLBUF_ENG2DRAMINFO *data);


FPIPPBUFASSIGNER IPL_BufAssignObj[IPLBUF_MODE_MAX_CNT] = {
	NULL,
	IPL_BufAssignFlowStream,
	IPL_BufAssignFlowCap,
};

IPLBUF_RST IPL_BufAssign(IPLBUF_MODE ModeIdx, IPLBUF_ENG2DRAMINFO *data)
{
	IPLBUF_RST rt_rst;

	rt_rst.er_code = E_SYS;
	rt_rst.buf_size = 0;
	if (ModeIdx >= IPLBUF_MODE_MAX_CNT) {
		DBG_ERR("ModeIdx = %d overflow\r\n", ModeIdx);
		return rt_rst;

	}

	if (IPL_BufAssignObj[ModeIdx] != NULL) {
		rt_rst = IPL_BufAssignObj[ModeIdx](data);
	}
	return rt_rst;
}

IPLBUF_RST IPL_BufAssignFlowStream(IPLBUF_ENG2DRAMINFO *data)
{
	IPLBUF_RST rt_rst;
	UINT32 buf_st_addr;
	UINT32 buf_size;
	UINT32 Size = 0, head_size = 0, addr, i;
	UINT32 y_size, uv_size;

	IPL_BufGetResInfo(data->id, &buf_st_addr, &buf_size);
	addr = buf_st_addr;

	if (IPL_CtrlGetInfor(data->id, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
		if (data->sie_ch0_en == ENABLE && data->sie_ch1_en == ENABLE) {
			if ((data->sie_out_ch0.buf_num == data->sie_out_ch1.buf_num) && (data->sie_out_ch0.buf_num > 0) && (data->sie_out_ch0.buf_num <= (IPLBUF_SIE_CH0_MAX - IPLBUF_SIE_CH0_1))) {

				head_size = IPL_HeaderRawGetBufSize();
				y_size = IPL_UTI_RAW_BUF_SIZE(data->sie_out_ch0.width, data->sie_out_ch0.line_ofs, data->sie_out_ch0.height);
				y_size = IPPBUF_WORDALIGN(y_size);

				uv_size = IPL_UTI_RAW_BUF_SIZE(data->sie_out_ch1.width, data->sie_out_ch1.line_ofs, data->sie_out_ch1.height);
				uv_size = IPPBUF_WORDALIGN(uv_size);
				for (i = 0; i < data->sie_out_ch0.buf_num; i ++) {

					IPL_BufSet(data->id, (IPLBUF_SIE_H_1 + i), addr, head_size, IPL_PrvBufNameTab[(IPLBUF_SIE_H_1 + i)]);
					addr += head_size;

					IPL_BufSet(data->id, (IPLBUF_SIE_CH0_1 + i), addr, y_size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH0_1 + i)]);
					addr += y_size;

					IPL_BufSet(data->id, (IPLBUF_SIE_CH1_1 + i), addr, uv_size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH1_1 + i)]);
					addr += uv_size;
				}
			} else {
				DBG_ERR("Id: %d, SIE Ch0/Ch1 out Buffer number not match (CH0 buf num: %d, CH1 buf num: %d)\r\n", data->id, data->sie_out_ch0.buf_num, data->sie_out_ch1.buf_num);
			}
		} else {
			DBG_ERR("Id: %d, CCIR Flow need enable SIE Ch0/Ch1 output(CH0 out: %d, CH1 out: %d)\r\n", data->id, data->sie_ch0_en, data->sie_ch1_en);
		}
	} else {
		if (data->sie_ch0_en == ENABLE) {
			if (((data->sie_out_ch0.buf_num > 0) && (data->sie_out_ch0.buf_num <= (IPLBUF_SIE_CH0_MAX - IPLBUF_SIE_CH0_1))) &&
				((IPLBUF_SIE_CH0_MAX - IPLBUF_SIE_CH0_1) == (IPLBUF_SIE_H_MAX - IPLBUF_SIE_H_1))
			   ) {
				Size = IPL_UTI_RAW_BUF_SIZE(data->sie_out_ch0.width, data->sie_out_ch0.line_ofs, data->sie_out_ch0.height);
				Size = IPPBUF_WORDALIGN(Size);
				head_size = IPL_HeaderRawGetBufSize();

				if (data->sie_out_ch0.buf_num == 1) {
					//minimum header buffer number = 2
					//head --> head --> raw
					for (i = 0; i < 2; i ++) {
						IPL_BufSet(data->id, (IPLBUF_SIE_H_1 + i), addr, head_size, IPL_PrvBufNameTab[(IPLBUF_SIE_H_1 + i)]);
						addr += head_size;
					}
					IPL_BufSet(data->id, (IPLBUF_SIE_CH0_1), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH0_1)]);
					addr += Size;
				} else {
					for (i = 0; i < data->sie_out_ch0.buf_num; i ++) {
						IPL_BufSet(data->id, (IPLBUF_SIE_H_1 + i), addr, head_size, IPL_PrvBufNameTab[(IPLBUF_SIE_H_1 + i)]);
						addr += head_size;
						IPL_BufSet(data->id, (IPLBUF_SIE_CH0_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH0_1 + i)]);
						addr += Size;
					}
				}
			} else {
				DBG_ERR("Id: %d, SIE Ch0 out Buffer number error (%d)\r\n", data->id, data->sie_out_ch0.buf_num);
			}
		}
	}

	if (data->sie_ca_en == ENABLE) {
		if ((data->sie_ca_buf_num > 0) && (data->sie_ca_buf_num <= (IPLBUF_SIE_CH1_MAX - IPLBUF_SIE_CH1_1))) {
			Size = IPPBUF_WORDALIGN(data->sie_ca_buf_size);
			for (i = 0; i < data->sie_ca_buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_SIE_CH1_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH1_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, SIE CA out Buffer number error (%d)\r\n", data->id, data->sie_ca_buf_num);
		}
	}

	if (data->sie_la_en == ENABLE) {
		if ((data->sie_la_buf_num > 0) && (data->sie_la_buf_num <= (IPLBUF_SIE_CH2_MAX - IPLBUF_SIE_CH2_1))) {
			Size = IPPBUF_WORDALIGN(data->sie_la_buf_size);
			for (i = 0; i < data->sie_la_buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_SIE_CH2_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH2_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, SIE LA out Buffer number error (%d)\r\n", data->id, data->sie_la_buf_num);
		}
	}

	if (data->sie_ch3_en == ENABLE) {
		if (data->sie_ch3_src == CH3_YOUT_ACC) {
			Size = IPL_UTI_RAW_BUF_SIZE(data->sie_out_ch3.width, data->sie_out_ch3.line_ofs, data->sie_out_ch3.height);
			Size = IPPBUF_WORDALIGN(Size);
		} else if (data->sie_ch3_src == CH3_VACC) {
			Size = IPPBUF_WORDALIGN(data->sie_ch3_va_buf_size);
		}

		if ((data->sie_out_ch3.buf_num > 0) && (data->sie_out_ch3.buf_num <= (IPLBUF_SIE_CH3_MAX - IPLBUF_SIE_CH3_1))) {
			for (i = 0; i < data->sie_out_ch3.buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_SIE_CH3_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH3_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, SIE Ch3 out Buffer number error (%d)\r\n", data->id, data->sie_out_ch3.buf_num);
		}
	}

	if (data->sie_ch4_en == ENABLE) {
		Size = IPPBUF_WORDALIGN(data->sie_ch4_buf_size);
		if ((data->sie_ch4_buf_num > 0) && (data->sie_ch4_buf_num <= (IPLBUF_SIE_CH4_MAX - IPLBUF_SIE_CH4_1))) {
			for (i = 0; i < data->sie_ch4_buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_SIE_CH4_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH4_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, SIE Ch4 out Buffer number error (%d)\r\n", data->id, data->sie_ch4_buf_num);
		}
	}

	if (data->sie_ch5_en == ENABLE) {
		if ((data->sie_ch5_buf_num > 0) && (data->sie_ch5_buf_num <= (IPLBUF_SIE_CH5_MAX - IPLBUF_SIE_CH5_1))) {
			Size = IPPBUF_WORDALIGN(data->sie_ch5_buf_size);
			for (i = 0; i < data->sie_ch5_buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_SIE_CH5_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIE_CH5_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, SIE Ch5 out Buffer number error (%d)\r\n", data->id, data->sie_ch5_buf_num);
		}
	}

	if (data->rhe_defog_sub_out_en == ENABLE) {
		Size = IPPBUF_WORDALIGN(data->rhe_defog_sub_out_size);
		IPL_BufSet(data->id, IPLBUF_RHESUB_1, addr, Size, IPL_PrvBufNameTab[IPLBUF_RHESUB_1]);
		addr += Size;
		IPL_BufSet(data->id, IPLBUF_RHESUB_2, addr, Size, IPL_PrvBufNameTab[IPLBUF_RHESUB_2]);
		addr += Size;
	}

	if (data->ipe_eth_en == ENABLE) {
		Size = IPPBUF_WORDALIGN(data->ipe_eth_size);
		IPL_BufSet(data->id, IPLBUF_IPEETH_1, addr, Size, IPL_PrvBufNameTab[IPLBUF_IPEETH_1]);
		addr += Size;
		IPL_BufSet(data->id, IPLBUF_IPEETH_2, addr, Size, IPL_PrvBufNameTab[IPLBUF_IPEETH_2]);
		addr += Size;
	}

	if (data->ife2_lca_en == ENABLE) {
		if ((data->ime_sub_out_buf_num > 0) && (data->ime_sub_out_buf_num <= (IPLBUF_IME_SUB_OUT_MAX - IPLBUF_IME_SUB_OUT_1))) {
			Size = data->ime_sub_out_size;
			for (i = 0 ; i < data->ime_sub_out_buf_num ; i ++) {
				IPL_BufSet(data->id, (IPLBUF_IME_SUB_OUT_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_IME_SUB_OUT_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, IME out Buffer number error (%d)\r\n", data->id, data->ime_sub_out_buf_num);
		}
	}

	if (data->ime_primacy_mask_en == ENABLE) {
		Size = data->ime_primacy_mask_buf_size;
		IPL_BufSet(data->id, IPLBUF_IME_PRI_MASK_1, addr, Size, IPL_PrvBufNameTab[IPLBUF_IME_PRI_MASK_1]);
		addr += Size;
		IPL_BufSet(data->id, IPLBUF_IME_PRI_MASK_2, addr, Size, IPL_PrvBufNameTab[IPLBUF_IME_PRI_MASK_2]);
		addr += Size;
	}

	if (data->ime_path1_square_en == ENABLE) {
		Size = data->ime_path1_square_mv_buf_size;
		addr = IPPBUF_8WORDALIGN(addr);	//MV address need 8word align
		Size = IPPBUF_8WORDALIGN(Size);	//MV address need 8word align
		IPL_BufSet(data->id, IPLBUF_IMEP1_SQUARE_MV_1, addr, Size, IPL_PrvBufNameTab[IPLBUF_IMEP1_SQUARE_MV_1]);
		addr += Size;
		IPL_BufSet(data->id, IPLBUF_IMEP1_SQUARE_MV_2, addr, Size, IPL_PrvBufNameTab[IPLBUF_IMEP1_SQUARE_MV_2]);
		addr += Size;

		Size = data->ime_path1_square_mo_buf_size;
		IPL_BufSet(data->id, IPLBUF_IMEP1_SQUARE_MO_1, addr, Size, IPL_PrvBufNameTab[IPLBUF_IMEP1_SQUARE_MO_1]);
		addr += Size;
		IPL_BufSet(data->id, IPLBUF_IMEP1_SQUARE_MO_2, addr, Size, IPL_PrvBufNameTab[IPLBUF_IMEP1_SQUARE_MO_2]);
		addr += Size;
	}
#if 0
	if (data->ime_path4.width != 0 && data->ime_path4.height != 0) {
		if ((data->ime_path4.buf_num > 0) && (data->ime_path4.buf_num <= (IPLBUF_IMEP4_MAX - IPLBUF_IMEP4_1))) {
			for (i = 0; i < data->ime_path4.buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_IMEP4_H_1 + i), addr, head_size, IPL_PrvBufNameTab[(IPLBUF_IMEP4_H_1 + i)]);
				addr += head_size;
			}
#if 0   //Jarkko tmp, wait adas lib ready
			if (IPL_CtrlGetInfor(data->id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE) {
				// arrange Y, Histo, EdgeMap buffer
				Size = IPL_YUV_SIZE(IPL_IMG_Y, data->ime_path4.line_ofs, data->ime_path4.height);
				Size = IPPBUF_WORDALIGN(Size);
				IPL_CtrlSetInfor(data->id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS, Size);
				Size +=  ADAS_getFcwsEdgeMapSz();
				Size = IPPBUF_WORDALIGN(Size);
				IPL_CtrlSetInfor(data->id, IPLCTRL_IME_P4_HISTO_BUFOFS, Size);
				Size +=  ADAS_getHistSz();
			}
#endif
			Size = IPPBUF_WORDALIGN(Size);
			for (i = 0; i < data->ime_path4.buf_num; i ++) {
				IPL_BufSet(data->id, (IPLBUF_IMEP4_1 + i), addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP4_1 + i)]);
				addr += Size;
			}
		} else {
			DBG_ERR("Id: %d, IME P4 out Buffer number error (%d)\r\n", data->id, data->ime_path4.buf_num);
		}
	}
#endif
	//Check Total Buffer
	rt_rst.er_code = E_OK;
	rt_rst.buf_size = (addr - buf_st_addr);
	if (rt_rst.buf_size > buf_size) {
		rt_rst.er_code = E_NOMEM;
		DBG_ERR("Id: %d, Buf. not enough 0x%x<0x%x\r\n", data->id, buf_size, rt_rst.buf_size);
	}
	return rt_rst;
}

IPLBUF_RST IPL_BufAssignFlowCap(IPLBUF_ENG2DRAMINFO *data)
{
	IPLBUF_RST rt_rst;
	UINT32 buf_st_addr = 0, buf_size = 0, addr = 0;

	IPL_BufGetResInfo(data->id, &buf_st_addr, &buf_size);
	addr = buf_st_addr;

	rt_rst.er_code = E_OK;
	rt_rst.buf_size = (addr - buf_st_addr);
	if (rt_rst.buf_size > buf_size) {
		rt_rst.er_code = E_NOMEM;
		DBG_ERR("Id: %d, Buf. not enough 0x%x<0x%x\r\n", data->id, buf_size, rt_rst.buf_size);
	}
	return rt_rst;
}