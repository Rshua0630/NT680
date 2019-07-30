/*
    IPL D2D Callback Function

    This file is the API of the D2D callback function.

    @file       ipl_ext_d2d_cb.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "ipl_ext_common.h"
_2DLUT_PARAM GET_Dce2DLut = {0};

static UINT32 IFE2_PPROC_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC *subF, IFE2_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;
	return Cnt;
}


static UINT32 IFE2_CAP_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC *subF, IFE2_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 5;

	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pIfe2_sub = subF;
	group.ife2Update = *Update;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);
	switch (eventID & (~IPL_FLOW_RESERVE)) {
	case (IPL_FLOW_IFE2D2D_LCA):
		Cnt = iq_get_ife2_lca_time(id, IQCB_D2D_IFE2_PRI);
		iq_set_flow_cb(id, IQCB_D2D_IFE2_PRI, &Info, &group);
		break;
	default:
		DBG_ERR("event fail\r\n");
		break;
	}
	*Update = group.ife2Update;

	return Cnt;
}

UINT32 IFE2_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC *subF, IFE2_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{

	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = IFE2_CAP_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = IFE2_PPROC_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}


#if 0
static UINT32 DCE_PPROC_D2D_CB(IPL_PROC_ID id, USIZE *pRef_1x, Coordinate *pRef_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}

static UINT32 DCE_CAP_D2D_CB(IPL_PROC_ID id, USIZE *pRef_1x, Coordinate *pRef_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}
#endif
UINT32 DCE_D2D_CB(IPL_PROC_ID id, USIZE *pRef_1x, Coordinate *pRef_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;
#if 0
	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = DCE_CAP_D2D_CB(id, pRef_1x, pRef_1xCen, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = DCE_PPROC_D2D_CB(id, pRef_1x, pRef_1xCen, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}
#endif
	return Cnt;
}

#if 0


static UINT32 IPE_PPROC_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}

static UINT32 IPE_CAP_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}
#endif
#if 0
UINT32 IPE_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = IPE_CAP_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = IPE_PPROC_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}
#endif
#if 0


static UINT32 IFE_PPROC_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}
#endif
#if 0
static UINT32 IFE_CAP_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 5;

	static UINT32 d2dCnt = 5;
	if (d2dCnt <= 1) {
		d2dCnt = Cnt;
	}
	d2dCnt--;
	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	return Cnt;
}
#endif
#if 0
UINT32 IFE_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = IFE_CAP_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = IFE_PPROC_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}
#endif

static UINT32 IME_PPROC_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}

static UINT32 IME_CAP_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	//IPL_FLOW_IMED2D_QV: Enable LCA
	IPL_HAL_GROUP group;
	group.imeUpdate = *Update;

	if ((eventID & (~IPL_FLOW_RESERVE)) == IPL_FLOW_IMED2D_SCAL) {
		group.imeUpdate = IME_SUBFEN;
		subF->imeFuncEn = 0;
	}

	*Update = group.imeUpdate;

	return Cnt;
}


UINT32 IME_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = IME_CAP_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = IME_PPROC_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}



static UINT32 DRE_PPROC_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}

static UINT32 DRE_CAP_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pDre_sub = subF;
	group.dreUpdate = *Update;

	iq_set_flow_cb(id, IQCB_D2D_DRE_PRI, &Info, &group);

	*Update = group.dreUpdate;
	return 1;
}

UINT32 DRE_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = DRE_CAP_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = DRE_PPROC_D2D_CB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}

#if 0
UINT32 SIE_D2D_CB(IPL_PROC_ID id, SIE_SUBFUNC *subF, SIE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;
	return Cnt;
}


#endif
static UINT32 RHEIME_PPROC_D2D_RHECB(IPL_PROC_ID id, RHE_SUBFUNC *subF, RHE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}


static UINT32 RHEIME_CAP_D2D_RHECB(IPL_PROC_ID id, RHE_SUBFUNC *subF, RHE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pRhe_sub = subF;
	group.rheUpdate = *Update;

	switch ((eventID & (~IPL_FLOW_RESERVE))) {
	case (IPL_FLOW_RAW2YCC_PRI):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_RHE_PRI, &Info, &group);
		break;
	case (IPL_FLOW_RAW2YCC_LCA):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_RHE_SUB, &Info, &group);
		break;

	default:
		DBG_ERR("event fail (%x)\r\n", eventID);
		break;
	}
	*Update = group.rheUpdate;
	return 1;
}


UINT32 RHEIME_D2D_RHECB(IPL_PROC_ID id, RHE_SUBFUNC *subF, RHE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = RHEIME_CAP_D2D_RHECB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = RHEIME_PPROC_D2D_RHECB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}

static UINT32 RHEIME_PPROC_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}


static UINT32 RHEIME_CAP_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pIfe_sub = subF;
	group.ifeUpdate = *Update;

	switch ((eventID & (~IPL_FLOW_RESERVE))) {
	case (IPL_FLOW_RAW2YCC_PRI):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_IFE_PRI, &Info, &group);
		break;
	case (IPL_FLOW_RAW2YCC_LCA):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_IFE_SUB, &Info, &group);
		break;

	default:
		DBG_ERR("event fail (%x)\r\n", eventID);
		break;
	}

	*Update = group.ifeUpdate;
	return 1;
}


UINT32 RHEIME_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC *subF, IFE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = RHEIME_CAP_D2D_IFECB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = RHEIME_PPROC_D2D_IFECB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}

static UINT32 RHEIME_PPROC_D2D_DCECB(IPL_PROC_ID id, USIZE *pRef_1x, Coordinate *pRef_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}


static UINT32 RHEIME_CAP_D2D_DCECB(IPL_PROC_ID id, USIZE *pRef_1x, Coordinate *pRef_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{

	IQS_REFIMG_INFO Refimg_info = {0};
	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pDce_sub = subF;
	group.dceUpdate = *Update;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);
	switch ((eventID & (~IPL_FLOW_RESERVE))) {
	case (IPL_FLOW_RAW2YCC_PRI):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_DCE_PRI, &Info, &group);
		break;
	case (IPL_FLOW_RAW2YCC_LCA):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_DCE_SUB, &Info, &group);
		break;
	default:
		DBG_ERR("event fail (%x)\r\n", eventID);
		break;
	}

	if (RSC_OnOffInfor(id) == SEL_RSC_ON) {
		UINT32 GET_Dce2DLut;

		GET_Dce2DLut = RSCCTRL_GETADR(id);
		Info.pDce_sub->_2DLutParam = (_2DLUT_PARAM *)GET_Dce2DLut;
		group.dceUpdate |= DCE_2DLUT;

	} else {
		Info.pDce_sub->_2DLutParam = &GET_Dce2DLut;
		Info.pDce_sub->_2DLutParam->_2DLutAddr = 0;
		Info.pDce_sub->_2DLutParam->_2DLutNum = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsFrac = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsInt = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsFrac = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsInt = 0;

		group.dceUpdate |= DCE_2DLUT;
	}

	iq_get_ref_image_info(id, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE), &Refimg_info);
	pRef_1x->w = Refimg_info.width;
	pRef_1x->h = Refimg_info.height;

	pRef_1xCen->x = pRef_1x->w / 2;
	pRef_1xCen->y = pRef_1x->h / 2;

	*Update = group.dceUpdate;

	return 1;
}


UINT32 RHEIME_D2D_DCECB(IPL_PROC_ID id, USIZE *pRef_1x, Coordinate *pRef_1xCen, DCE_SUBFUNC *subF, DCE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = RHEIME_CAP_D2D_DCECB(id, pRef_1x, pRef_1xCen, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = RHEIME_PPROC_D2D_DCECB(id, pRef_1x, pRef_1xCen, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}



static UINT32 RHEIME_PPROC_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}


static UINT32 RHEIME_CAP_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	DBG_IND("EventID = 0x%.8x\r\n", eventID);
	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pIpe_sub = subF;
	group.ipeUpdate = *Update;
	switch ((eventID & (~IPL_FLOW_RESERVE))) {
	case (IPL_FLOW_RAW2YCC_PRI):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_IPE_PRI, &Info, &group);
		break;
	case (IPL_FLOW_RAW2YCC_LCA):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_IPE_SUB, &Info, &group);
		break;

	default:
		DBG_ERR("event fail (%x)\r\n", eventID);
		break;
	}

	*Update = group.ipeUpdate;
	return 1;
}


UINT32 RHEIME_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC *subF, IPE_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = RHEIME_CAP_D2D_IPECB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = RHEIME_PPROC_D2D_IPECB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}

static UINT32 RHEIME_PPROC_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	return 1;
}

static UINT32 RHEIME_CAP_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pIme_sub = subF;
	group.imeUpdate = *Update;
	switch ((eventID & (~IPL_FLOW_RESERVE))) {
	case (IPL_FLOW_RAW2YCC_PRI):
		iq_set_flow_cb(id, IQCB_PREIME_D2D_IME_PRI, &Info, &group);
		break;
	case (IPL_FLOW_RAW2YCC_LCA):
		break;
	default:
		DBG_ERR("event fail (%x)\r\n", eventID);
		break;
	}
	*Update = group.imeUpdate;

	return 1;
}

UINT32 RHEIME_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC *subF, IME_UPDATE *Update, UINT32 eventID, fpRegDump *fpRegD)
{
	UINT32 Cnt = 1;

	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {
		Cnt = RHEIME_CAP_D2D_IMECB(id, subF, Update, eventID, fpRegD);
	} else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1))) {
		Cnt = RHEIME_PPROC_D2D_IMECB(id, subF, Update, eventID, fpRegD);
	} else {
		DBG_ERR("event fail\r\n");
	}

	return Cnt;
}
static UINT32 RHEIME_CAP_D2D_DEFOGCB(IPL_PROC_ID id, RHE_SUBFUNC *psubF, RHE_UPDATE *Update, void *extinfo)
{

	UINT32 rhe_sub_out_size;

	defog_lib_get_subimg_size(id, &rhe_sub_out_size);

	if(psubF->subout_info.uiSubOutAddr != 0) {
		psubF->subout_info.SubOutParam.SubOutParamMode = RHE_HAL_PARAM_AUTO_MODE;
		psubF->subout_info.SubOutSize.width = rhe_sub_out_size;
		psubF->subout_info.SubOutSize.height = rhe_sub_out_size;
		psubF->subout_info.uiSubOutOfs = rhe_sub_out_size*4;
	} else {
		// defog init
		S_DEFOG_LIB_RET defog_lib_ret;
		static UINT32 CAdjModLutA[9] = { 32,  64,  96, 128, 160, 192, 224, 256};
		static UINT32 CAdjModLutB[9] = {512,1024,1536,2048,2560,3072,3584,4096};
		static UINT32 out_wet[17]= {0};
		UINT32* out_wet_addr = iq_get_defog_wet(id);
		S_DEFOG_LIB_INIT_PARA defog_init_para;

		iq_get_defog_param(id, &defog_init_para);
		if (defog_lib_init(id, &defog_init_para) != E_OK) {
			DBG_ERR("Id %u, Init Defog Lib fail\r\n", id);
		}
		// defog process
	    S_DEFOG_LIB_PARA defog_lib_param;
	    S_DEFOG_LIB_INIT_PARA defog_para;

		iq_get_defog_param(id, &defog_para);
	    chg_defog_percentages(defog_para.percentages, id); //only for auto mode
	    chg_defog_lut(defog_para.auto_defog_lut, id); //only for auto mode

	    defog_lib_param.p_subImg        = (UINT32 *)extinfo;
		defog_lib_param.user_strength   = IPL_AlgGetUIInfo(id, IPL_SEL_DEFOG_LEVEL_IMM); //only for auto mode
		defog_lib_param.blending_factor = 255; // cap need to be 255, preview need to be 128
		defog_lib_proc(id, &defog_lib_param);
		// get parameters
		memcpy(out_wet, out_wet_addr, sizeof(UINT32)*17);
		defog_lib_get_para(id, &defog_lib_ret);

		psubF->contrast_adj_info.CAdjSubImgSize.height = rhe_sub_out_size;
		psubF->contrast_adj_info.CAdjSubImgSize.width =rhe_sub_out_size;
		psubF->contrast_adj_info.uiCAdjImgOfs = rhe_sub_out_size*4;
		psubF->contrast_adj_info.CAdjParam.CAdjMin.uiDiv = defog_lib_ret.RHE_MIN_DIV;
		psubF->contrast_adj_info.CAdjParam.CAdjMin.uiClamp = defog_lib_ret.RHE_MIN_CLAMP;

		psubF->contrast_adj_info.CAdjParam.CAdjMax.uiDiv = defog_lib_ret.RHE_MAX_DIV;
		psubF->contrast_adj_info.CAdjParam.CAdjMax.uiClamp = defog_lib_ret.RHE_MAX_CLAMP;

		psubF->contrast_adj_info.CAdjParam.CAdjEnh.uiCAdjRto = defog_lib_ret.REH_CTADJ_RTO;
		psubF->contrast_adj_info.CAdjParam.CAdjEnh.uiCAdjMinDif = defog_lib_ret.RHE_CTADJ_MIN_DIF;
		psubF->contrast_adj_info.CAdjParam.CAdjEnh.uiCAdjGainRto = defog_lib_ret.RHE_CTADJ_GRATIO;
		psubF->contrast_adj_info.CAdjParam.CAdjEnh.CAdjOpt = RHE_HAL_CADJ_OPT_B;

		psubF->contrast_adj_info.CAdjParam.CAdjBld.puiCAdjBldRto = &defog_lib_ret.BLDRTO[0];
		psubF->contrast_adj_info.CAdjParam.CAdjBld.CadjBldRef = RHE_HAL_CADJ_BLD_REF_GAMMA;

		psubF->contrast_adj_info.CAdjParam.CAdjOutCoefs.puiCAdjOutputWet = &out_wet[0];
		psubF->contrast_adj_info.CAdjParam.CAdjOutCoefs.CAdjOutWetRef = RHE_HAL_CADJ_OUT_AADJ;
		psubF->contrast_adj_info.CAdjParam.CAdjOutCoefs.CAdjRefGmaEn = DISABLE;

		psubF->contrast_adj_info.CAdjParam.CAdjParamMode = RHE_HAL_PARAM_USER_MODE;
		psubF->contrast_adj_info.CAdjParam.puiCAdjModLutA = &CAdjModLutA[0];
		psubF->contrast_adj_info.CAdjParam.puiCAdjModLutB = &CAdjModLutB[0];
		psubF->contrast_adj_info.CAdjParam.puiCAdjGamma = &defog_lib_ret.RHE_YGAMMA[0];

		psubF->contrast_adj_info.CAdjFilter.FtrParamMode = RHE_HAL_PARAM_AUTO_MODE;
		psubF->contrast_adj_info.CAdjSclFtr.CAdjParamMode = RHE_HAL_PARAM_AUTO_MODE;

		*Update |= RHE_SUBF_UPDATE|RHE_COLOR_CONTRAST;

		// defog uninit
		if (defog_lib_close(id) != E_OK) {
			DBG_ERR("Id %u, UnInit Defog Lib fail\r\n", id);
		}
	}
	return E_OK;
}
static UINT32 RHEIME_CAP_D2D_PHDRCB(IPL_PROC_ID id, RHE_SUBFUNC *psubF, RHE_UPDATE *Update,void *extinfo )
{
	ER rt;
	SR_WDR_OPEN_INFO pOpenInfo;
	IPLC_D2D_YOUT_INFO* yout_info = (IPLC_D2D_YOUT_INFO*)extinfo;
	//wdr init
	rt = SceneRender_WDR_Init(IPL_UTI_CONV2_WDR_ID(id), &pOpenInfo);
	if (rt != E_OK) {
		DBG_ERR("Init WDR Lib fail\r\n");
		return E_SYS;
	}
	//wdr process
	SR_WDR_CAL_INFO wdr_info = {0};
	SR_WDR_YOUT_INFO wdr_yout_info = {0};
	SR_WDR_CHG_PARAMS wdr_chg_par = {0};

	switch (IPL_AlgGetUIInfo(id, IPL_SEL_WDR_IMM)) {
			case SEL_WDR_OFF:
				wdr_info.WdrProcMode = SR_WDR_OFF;
				break;

			case SEL_WDR_AUTO:
				wdr_info.WdrProcMode = SR_WDR_AUTO;
				break;

			case SEL_WDR_MANUAL:
				wdr_info.WdrProcMode = SR_WDR_MANUAL;
				break;

			default:
				wdr_info.WdrProcMode = SR_WDR_OFF;
				break;
			}
	wdr_info.pYoutInfo = &wdr_yout_info;
	wdr_info.pYoutInfo->uiYoutAddr = yout_info->yout_addr;
	wdr_info.pYoutInfo->uiYoutLofs = yout_info->yout_lofs;
	wdr_info.pYoutInfo->uiYoutSizeH = yout_info->yout_sizeh;
	wdr_info.pYoutInfo->uiYoutSizeV = yout_info->yout_sizev;
	wdr_info.puiGGammaLut = iq_get_gamma_lut(IPL_UTI_CONV2_IQS_ID(id));
	wdr_info.uiWDRLevel = IPL_AlgGetUIInfo(id, IPL_SEL_WDR_M_LEVEL_IMM);

	wdr_chg_par.uiChgSel = SR_WDR_CHG_AE_TAG_Y;
	wdr_chg_par.uiChgAeTagY = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(id), AE_SEL_CURR_EXPY);
	rt |= SceneRender_WDR_ChgParams(IPL_UTI_CONV2_WDR_ID(id), &wdr_chg_par);
	rt |= SceneRender_WDR_Proc(IPL_UTI_CONV2_WDR_ID(id), &wdr_info);
	if (rt != E_OK) {
		DBG_ERR("WDR Proc fail\r\n");
		return E_SYS;
	}

    //wdr get parameters
    SR_WDR_PARAM wdr_param;
    SR_WDR_PATH_CURVE_PARAM	GetPathParam; ///< path parameters
    SR_WDR_BLEND_PARAM GetBlendParam; ///< blend parameters
    SR_WDR_LCE_PARAM GetLceParam; ///< local contrast enhancement parameters
    SR_WDR_GTC_PARAM GetGtcParam; ///< global tone curve parameters

	wdr_param.pGetPathParam = &GetPathParam;
	wdr_param.pGetBlendParam = &GetBlendParam;
	wdr_param.pGetLceParam = &GetLceParam;
	wdr_param.pGetGtcParam = &GetGtcParam;
	SceneRender_WDR_GetParam(IPL_UTI_CONV2_WDR_ID(id), &wdr_param);

	//wdr set parameters
	psubF->path_curve_info.PathCurveParamMode = RHE_HAL_PARAM_USER_MODE;
	psubF->path_curve_info.puiCurveA = wdr_param.pGetPathParam->puiCurveA;
	psubF->path_curve_info.puiCurveB = wdr_param.pGetPathParam->puiCurveB;
	psubF->path_curve_info.puiCurveC = wdr_param.pGetPathParam->puiCurveC;
	psubF->path_curve_info.puiCurveD = wdr_param.pGetPathParam->puiCurveD;
	psubF->path_curve_info.puiCurveE = wdr_param.pGetPathParam->puiCurveE;

	//BlendInfo
	psubF->blending_info.BldParamMode = RHE_HAL_PARAM_USER_MODE;
	psubF->blending_info.uiRefWet = wdr_param.pGetBlendParam->uiBlendRefWet;
	psubF->blending_info.puiBldCurve = wdr_param.pGetBlendParam->puiBlendCurve;

	//GblTnInfo
	psubF->tone_curve_info.TcMode = RHE_HAL_TC_RW_MODE_DRAM;
	psubF->tone_curve_info.TcDataMode = wdr_param.pGetGtcParam->GtcProcSel;
	psubF->tone_curve_info.puiTcCurve = wdr_param.pGetGtcParam->puiGtcCurve;

	psubF->subimg_filter_info.FtrParamMode = RHE_HAL_PARAM_AUTO_MODE;

	psubF->rhe_func_enable |= (SubImgFilterEn|ImgBlendingEn|ToneCurveEn);

	*Update |= RHE_SUBF_UPDATE|RHE_PATH_CURVE|RHE_BLEND|RHE_TONE_CURVE|RHE_SUBIMG_FILTER;

    rt |= iq_set_flow_cb(id, IQS_WDR_END, NULL, NULL);
	if(rt != E_OK){
		DBG_ERR("iq set wdr_end fail\r\n");
		return E_SYS;
	}

	return E_OK;
}

static void RHEIME_CAP_D2D_SETUI_SHDRCB(IPL_PROC_ID id)
{
	SHDR_UI_INFO shdr_info;
	switch (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR)) {
    case SEL_SHDR_WDR:
        DBG_WRN("Id: %d, Can't select SEL_SHDR_WDR in cap HDR mode! Force use SHDR_HDR_MODE\r\n", id);
	case SEL_SHDR_ON:
		shdr_info.shdrMode = SHDR_HDR_MODE;
		break;

	case SEL_SHDR_OFF:
		shdr_info.shdrMode = SHDR_LINEAR_MODE;
		break;

	default:
		DBG_ERR("Id: %d, Not Support SHDR Mode %d\r\n", id, IPL_AlgGetUIInfo(id, IPL_SEL_SHDR));
		shdr_info.shdrMode = SHDR_LINEAR_MODE;
		break;
	}

	shdr_info.uiHDRLevel = IPL_AlgGetUIInfo(id, IPL_SEL_SHDR_LEVEL_IMM);
	SensorHDR_SetUI(&shdr_info);

	return ;
}

static UINT32 RHEIME_CAP_D2D_SHDRCB(IPL_PROC_ID id, RHE_SUBFUNC *psubF, RHE_UPDATE *Update)
{
	SHDR_OPEN_INFO HdrOpenInfo;
	SHDR_CAL_INFO HdrCalInfo;
	static SHDR_PARAM HdrParamQ;
	ER rt;

	//SHDR Init
	HdrOpenInfo.uiHDRframeNo = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_TOTAL_FRM);
	HdrOpenInfo.evRatioExpect = ae_get_shdr_ev_ratio(id);
    HdrOpenInfo.shdrIQpara.uiRefMapWeight = IPL_AlgGetUIInfo(id, IPL_SEL_SHDR_LEVEL_IMM);
	RHEIME_CAP_D2D_SETUI_SHDRCB(id);
	rt = SensorHDR_Init(&HdrOpenInfo);
	if (rt == E_SYS) {
		DBG_ERR("SHDR Init fail %d\r\n", id);
		return E_SYS;
	}

	//SHDR Process
	HdrCalInfo.evRatioR = ae_get_shdr_ev_ratio(id);
	HdrCalInfo.uiAELV = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(id), AE_SEL_CAP_LV);
	HdrCalInfo.uiAEExpectLumRaw = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(id), AE_SEL_CURR_EXPY);
	HdrCalInfo.uiAECurrentLumRaw = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(id), AE_SEL_CURR_Y);
    HdrCalInfo.shdrIQpara.uiRefMapWeight = IPL_AlgGetUIInfo(id, IPL_SEL_SHDR_LEVEL_IMM);
	RHEIME_CAP_D2D_SETUI_SHDRCB(id);  //ask max if need?????????
	rt = SensorHDR_Proc(&HdrCalInfo);
	if (rt == E_SYS) {
		DBG_ERR("SHDR process fail %d\r\n", id);
		return E_SYS;
	}
	SensorHDR_GetParam(&HdrParamQ);
	//Set parameters

	//RefLaLpfInfo
	psubF->subimg_filter_info.FtrParamMode = RHE_HAL_PARAM_USER_MODE;
	psubF->subimg_filter_info.FtrSize = HdrParamQ.RefLaLpfInfo.uiLpfSize;
	psubF->subimg_filter_info.puiLpfCoefs = &HdrParamQ.RefLaLpfInfo.uiLpf[0];
	psubF->subimg_filter_info.uiLpfNormM = HdrParamQ.RefLaLpfInfo.uiLpfNormM;
	psubF->subimg_filter_info.uiLpfNormS = HdrParamQ.RefLaLpfInfo.uiLpfNormS;

	//ext
	psubF->path_curve_info.PathCurveParamMode = RHE_HAL_PARAM_USER_MODE;
	psubF->path_curve_info.puiCurveA = &HdrParamQ.PathCurveInfo.Curve[0].uiPathCvTbl[0];
	psubF->path_curve_info.puiCurveB = &HdrParamQ.PathCurveInfo.Curve[1].uiPathCvTbl[0];
	psubF->path_curve_info.puiCurveC = &HdrParamQ.PathCurveInfo.Curve[2].uiPathCvTbl[0];
	psubF->path_curve_info.puiCurveD = &HdrParamQ.PathCurveInfo.Curve[3].uiPathCvTbl[0];
	psubF->path_curve_info.puiCurveE = &HdrParamQ.PathCurveInfo.Curve[4].uiPathCvTbl[0];

	psubF->path_curve_info.puiWetA = &HdrParamQ.PathCurveInfo.Weight[0].uiPathWeiTbl[0];
	psubF->path_curve_info.puiWetB = &HdrParamQ.PathCurveInfo.Weight[1].uiPathWeiTbl[0];
	psubF->path_curve_info.puiWetC = &HdrParamQ.PathCurveInfo.Weight[2].uiPathWeiTbl[0];

	//GblTnInfo
	psubF->tone_curve_info.TcMode = RHE_HAL_TC_RW_MODE_DRAM;
	psubF->tone_curve_info.TcDataMode = RHE_HAL_TC_DATA_BAYER;
	psubF->tone_curve_info.puiTcCurve = &HdrParamQ.GblTnInfo.uiGblTnTbl[0];
	psubF->tone_curve_info.uiTcAddr = (UINT32) &HdrParamQ.GblTnInfo.uiGblTnTbl[0];

	//BlendInfo
	psubF->blending_info.BldParamMode = RHE_HAL_PARAM_USER_MODE;
	psubF->blending_info.uiRefWet = HdrParamQ.BlendInfo.uiRefMapWeight;
	psubF->blending_info.puiBldCurve = &HdrParamQ.BlendInfo.BlendCurve.uiBldCvTbl[0];

	psubF->rheFuncEn |= (SubImgFilterEn | ImgBlendingEn|ToneCurveEn);
	*Update |= (RHE_SUBIMG_FILTER |RHE_BLEND |RHE_TONE_CURVE|RHE_PATH_CURVE_WEIGHT);

    rt |= iq_set_flow_cb(id, IQS_SHDR_END, NULL, NULL);
	if(rt != E_OK){
		DBG_ERR("iq set wdr_end fail\r\n");
		return E_SYS;
	}

	return E_OK;
}

UINT32 RHEIME_D2D_EXTFUNC_RHE_CB(IPL_PROC_ID id, RHE_SUBFUNC *psubF, RHE_UPDATE *Update, UINT32 eventID, void *extinfo)
{
	DBG_IND("EventID = 0x%.8x\r\n", eventID);

	if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1))) {

		switch (eventID) {
		case IPL_FLOW_RAW2YCC_PHDR:
			RHEIME_CAP_D2D_PHDRCB(id, psubF, Update,extinfo);
			break;
		case IPL_FLOW_RAW2YCC_HDR:
			RHEIME_CAP_D2D_SHDRCB(id, psubF, Update);
			break;
		case IPL_FLOW_RAW2YCC_DEFOG:
			RHEIME_CAP_D2D_DEFOGCB(id, psubF, Update,extinfo);
			break;
		default:
			DBG_ERR("event fail (%x)\r\n", eventID);
			break;
		}
	} else {
		DBG_ERR("event fail\r\n");
	}

	return E_OK;

}

void _STRIPE_CB(IPL_PROC_ID id, USIZE *Ref_1x, Coordinate *Ref_1xCen,
				USIZE *SieInSize, USIZE *SieOutSize, UINT32 *SenHRatio, UINT32 *SenVRatio,
				DCE_SUBFUNC *subF, IPL_HAL_GROUP *Update, UINT32 eventID)
{
	ENG_SUBFUNC Info = {0};
	Info.pDce_sub = subF;
	IPL_HAL_GROUP group;
	group = *Update ;
	IQS_REFIMG_INFO Refimg_info = {0};
	IQS_FLOW_MSG IQSevent = IQCB_PREIME_D2D_DCE_PRI;

	if (eventID == IPL_FLOW_CAL_STRIPE) {
		IPL_DzoomGetSIECropSize(id, 0, TRUE, &(SieInSize->w), &(SieInSize->h));
		IPL_DzoomGetSIEOutSize(id, IPL_DzoomGetIndex(id), &(SieOutSize->w), &(SieOutSize->h));
		iq_get_ref_image_info(id, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE), &Refimg_info);
		Ref_1x->w = Refimg_info.width;
		Ref_1x->h = Refimg_info.height;
		Ref_1xCen->x = Ref_1x->w / 2;
		Ref_1xCen->y = Ref_1x->h / 2;
		*SenHRatio = Refimg_info.hratio;
		*SenVRatio = Refimg_info.vratio;

		iq_set_flow_cb(id, IQSevent, &Info, &group);
	} else {
		DBG_ERR("event fail\r\n");
	}

	if (rsc_ext_getenable(id) == SEL_RSC_ON) {

		UINT32 GET_Dce2DLut;

		GET_Dce2DLut = RSCCTRL_GETADR(id);

		Info.pDce_sub->_2DLutParam = (_2DLUT_PARAM *)GET_Dce2DLut;
		group.dceUpdate |= DCE_2DLUT;

	} else {
		Info.pDce_sub->_2DLutParam = &GET_Dce2DLut;
		Info.pDce_sub->_2DLutParam->_2DLutAddr = 0;
		Info.pDce_sub->_2DLutParam->_2DLutNum = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsFrac = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsInt = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsFrac = 0;
		Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsInt = 0;

		group.dceUpdate |= DCE_2DLUT;
	}

	*Update = group ;
}



