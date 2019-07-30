#ifndef IPL_EXT_CTRL_INT_H_
#define IPL_EXT_CTRL_INT_H_

/**
    ipl_ext_ctrl_int.h


    @file       ipl_ctrl_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/**
    information for Image-Pipeline
    Add ones if necessary
*/
typedef enum {
	IPLCTRL_MAX_CNT = IPLCTRL_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(IPLCTRL_IDX)
} IPLCTRL_IDX;

//check IPLCTRL_IDX range
STATIC_ASSERT((IPLCTRL_MAX_CNT <= IPL_CTRL_ELEMENT_MAX));

/**
    set information for Image-Pipeline
    Add ones if necessary
*/
typedef enum {
	IPLCTRL_CAP_FLOW_NONE = 0,
	IPLCTRL_CAP_FLOW_NORMAL,
	IPLCTRL_CAP_FLOW_HDR,
	IPLCTRL_CAP_FLOW_3D,
	IPLCTRL_CAP_FLOW_HS,
	IPLCTRL_CAP_FLOW_EBURST,
	IPLCTRL_CAP_FLOW_INVID,
	IPLCTRL_CAP_FLOW_AEB,
	IPLCTRL_CAP_FLOW_AWB,
	IPLCTRL_CAP_FLOW_SIM,
	IPLCTRL_CAP_FLOW_MFHDR,
	IPLCTRL_CAP_FLOW_MFANTISHAKE,
	IPLCTRL_CAP_FLOW_DUALSBS,
	IPLCTRL_CAP_FLOW_RHDR,
	IPLCTRL_CAP_FLOW_MAX,
	ENUM_DUMMY4WORD(IPLCTRL_CAP_FLOW)
} IPLCTRL_CAP_FLOW;

typedef enum {
	IPLCTRL_CA_TYPE_32BIT = 0,
	IPLCTRL_CA_TYPE_64BIT,
	IPLCTRL_CA_TYPE_MAX,
	ENUM_DUMMY4WORD(IPLCTRL_CA_TYPE)
} IPLCTRL_CA_TYPE;

typedef enum {
	IPL_OB_FIX = 0,
	IPL_OB_AUTO,
	ENUM_DUMMY4WORD(IPL_OB_TYPE)
} IPL_OB_TYPE;

typedef struct {
	SENSOR_INFO *info;
	IPL_MODE_DATA *chg_mode;
} IPLCTRL_INFO;

typedef struct {
	UINT32 param[10];
	UINT32 frm_ctrl;
} IPLCTRL_CAP_EXT_INFO;

void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *cap_info, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_CtrlInitPrvIpcInfo(UINT32 id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_MODE_DATA *ChgMode);

void IPL_CtrlInitPrvbuf_info(IPL_MODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *buf_info);
void IPL_CtrlInitCapbuf_info(UINT32 id, IPL_MODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *buf_info);
void IPL_CtrlPrvInit(UINT32 id, IPLCTRL_INFO *CtrlInfo);
void IPL_CtrlCapInit(UINT32 id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo);

INT32 IPL_Ctrl_cal_input_offset(UINT32 src_size, UINT32 dest_size, INT32 start_ofs, UINT32 *start_pos);
UINT32 IPL_CtrlGetSIEVAbuf_info(void);

void IPL_CtrlPrvSetDZoomInfo(UINT32 id);
void IPL_CtrlPrvGetDZoomInfo(UINT32 id, IPC_Dzoom_Info *Info);
void IPL_Setime_path_Info(UINT32 id, IPL_IME_PATH path_id, IPL_IME_PATH_INFO *Info);
void IPL_Getime_path_Info(UINT32 id, IPL_IME_PATH path_id, IPL_IME_PATH_INFO *Info);
void IPL_GetDceStrpInfo(UINT32 id, IPL_STRP_INFO *strp_info);

//sie flow callback
void IPL_SIESetOB_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub);
void IPL_SIESetCA_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath);
void IPL_SIESetLA_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_LACC *lacc, SIE_STATS_PATH_INFO *pPath);
void IPL_SIESetEthOut_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_ETH *eth, SIE_VACC *va, SIE_EDGETHRE *eththr);
void IPL_SIESetBP_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub);
void IPL_SIESetDMAOutSrc_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update);
void IPL_SIESetDMAOutCh3_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_HAL_YOUT_INFO *yinfo);
void IPL_SIESetDP_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *subs);
void IPL_SIEPatGen_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, PAT_GEN_INFO *PatGen);
void IPL_SIESetRawEnc_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, SIE_RAWENC *RawEnc);
void IPL_SIECCIR_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, SIE_DVI *pDvi);

void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 id, SIE_UPDATE *Update, SIE_ACT_MODE *ActMode);
void IPL_CtrlInitIpc_SIEBurstLength_Cap(UINT32 Id, SIE_UPDATE *Update, SENSOR_INFO *SenInfo, SIE_BURSTLENGTH_SET *Length);
void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 id, SIE_UPDATE *Update, IPP_SIE_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out, SIE_PACKBUS *Bit);
void IPL_CtrlInitIpc_SIECh1PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out);
void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out);
void IPL_CtrlInitIpc_SIECh3PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out, SIE_PACKBUS *Bit);
void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 id, SIE_UPDATE *Update, SIE_WINDOW *Win, BAYER_TYPE *bayer_type, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 id, SIE_UPDATE *Update1, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub);
//void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 Id,  SIE_UPDATE *Update1, SieFuncEn *func_en, SIE_GAMMA **pGamma);    //Jarkko tmp
void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 id,  SIE_UPDATE *Update1, SieFuncEn *func_en, SIE_RAWENC *RawEnc);
void IPL_CtrlInitIpc_SIECASet_Cap(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath, UINT32 CAEnable);
void IPL_CtrlInitIpc_SIECCIR_Cap(UINT32 id, SIE *sie, FLIP_TYPE flip_type);
void IPL_CtrlInitIpc_SIECH3Info_Cap(UINT32 id, SIE *psie, SIE_UPDATE *Update);

void IPL_CtrlInitIpc_SIEAct(UINT32 id, SENSOR_INFO *SenInfo, SIE_ACT_MODE *ActMode);
void IPL_CtrlInitIpc_SIEIn(UINT32 id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win, BAYER_TYPE *bayer_type);
void IPL_CtrlInitIpc_SIESignal(UINT32 id, IPP_SIE_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIECh0PPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, UINT32 *buf_num, UINT32 *addr);
void IPL_CtrlInitIpc_SIECh1PPB(UINT32 id, IPL_RAW_IMG_INFO *dma_out, UINT32 *addr);
void IPL_CtrlInitIpc_SIECh2PPB(UINT32 id, UINT32 *addr);
void IPL_CtrlInitIpc_SIECh3PPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, UINT32 *addr, UINT32 *buf_num, CH3_SRC *out_src);
void IPL_CtrlInitIpc_SIECh4PPB(UINT32 id, UINT32 *addr, CH4_SRC *out_src);
void IPL_CtrlInitIpc_SIECh5PPB(UINT32 id, UINT32 *addr);
void IPL_CtrlInitIpc_IPLInPPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, Coordinate *st_pos, RHE_HAL_FUNC_MODE *rhe_mode);
void IPL_CtrlInitIpc_IPLStrpInfo(UINT32 id, IPL_STRP_INFO *strp_info);
void IPL_CtrlInitIpc_RHESubOutPPB(UINT32 id, UINT32 *Addr, UINT32 *width, UINT32 *height);
void IPL_CtrlInitIpc_DCEIn(UINT32 id, SENSOR_INFO *SenInfo, USIZE *pRef_1x, Coordinate *pCenter_1x, iCoordinate *pCenter_1xOFS, UINT32 *HRatio, UINT32 *VRatio);
void IPL_CtrlInitIpc_VR360Info(UINT32 id, VR360_DATA *VR360Data);
void IPL_CtrlInitIpc_DCEDeWarpInfo(UINT32 id, DCE_Stripe *StripeInfo, _2DLUT_PARAM *Dce2DInfo);
void IPL_CtrlInitIpc_IPEEthPPB(UINT32 id, UINT32 *Addr, UINT32 *LineOfs);
void IPL_CtrlInitIpc_IMESubFuncPPB(UINT32 id, UINT32 buf_idx, UINT32 *PP0Addr, UINT32 *PP1Addr);
void IPL_CtrlInitIpc_IMEPxOutInfo(UINT32 id, UINT32 path_en_idx, IPLC_IME_PATH_INFO *path_info);
void IPL_CtrlInitIpc_IMEPxOutInfo_Stitch(UINT32 id, UINT32 path_en_idx, IPL_YCC_IMG_INFO *path_info, STITCH_IMG *stitch_img);

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 id);
IPLBUF_MODE IPL_MapICF2BufMode(IPLCTRL_FLOW flow);
void IPL_Ctrl_Out_Buf_Cfg(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path[]);
void IPL_YUVBufStsDump(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path, IPL_IMG_OUT_INFO *img_out_info);
#endif //IPL_EXT_CTRL_INT_H_
