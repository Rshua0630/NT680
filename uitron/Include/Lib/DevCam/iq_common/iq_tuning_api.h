#ifndef _IQ_TUNING_API_H_
#define _IQ_TUNING_API_H_

#define ISP_MAX_ISONUM      IQS_ISOMAPTAB_NUM
#define IQ_FEATURE_COMMON           0x00000000
#define IQ_FEATURE_HDR              0x00000001
#define IQ_FEATURE_WDR              0x00000002
#define IQ_FEATURE_DEFOG            0x00000004

/**
    Typedef struct for IQ Information.
*/
typedef struct{
	UINT32 id;
    UINT32 cur_sen_mode;
    UINT32 iq_feature;
	char sensor_name[32];
	char iq_setting_name[32];
    char ae_setting_name[32];
    char awb_setting_name[32];

	UINT32 iqs_mode_table[SENSOR_MODE_Num];
} iqs_isp_info;

typedef struct{
	UINT32 id;
	UINT32 size;
	char isp_name[32];
	iqs_isp_info iqs_info[Total_SENIDNum];
} sISP_isp_info;

typedef struct _sISP_SIE_PAUSE {
	UINT32 Id;
	UINT32 size;

    IPL_PAUSEDMA_INFO sie_pause;
} sISP_SIE_PAUSE;

typedef struct _sISP_SIE_RESUME {
	UINT32 Id;
	UINT32 size;

    IPL_RESUMEDMA_INFO sie_resume;
} sISP_SIE_RESUME;

typedef struct _sISP_RAW_Info {
	UINT32 Id;
	UINT32 size;

    HEADER_RAW_DATA raw_info;
} sISP_RAW_Info;

typedef struct _sISP_IME_INFO {
	UINT32 Id;
	UINT32 size;

    IPL_IME_INFOR ime_info;
} sISP_IME_INFO;

typedef struct{
	UINT32 id;
	UINT32 size;

	UINT32 wdr_strength;
    UINT32 hdr_strength;
    UINT32 reserved_2;
    UINT32 reserved_3;
    UINT32 reserved_4;
    UINT32 reserved_5;
} sISP_IQ_STATUS;

typedef struct _sISP_RAW_Buffer_Info {
	UINT32 Id;
	UINT32 size;

	UINT32 width;           ///< image width(pixel)
	UINT32 height;          ///< image height(pixel)
	UINT32 line_offset;      ///< image line offset(byte)
	IPL_RAW_BIT bit;        ///< bits /pixel
	IPL_RAW_PIX st_pix;      ///< image start pixel
} sISP_RAW_Buffer_Info;

typedef struct _sISP_RAW_Buffer_Addr {
	UINT32 Id;
	UINT32 size;

    UINT32 raw_addr;
} sISP_RAW_Buffer_Addr;

typedef struct _sISP_YUV_Buffer_Info {
	UINT32 Id;
	UINT32 size;

	UINT32 plane_addr[3];
} sISP_YUV_Buffer_Info;
/************************************************************/

/**
    Struct of All IQ Parameter Pointers.
*/
typedef struct {
	ISOMAP *ISOIDX_MAP;

    ENGINE_CTRL *Engine_Ctrl;

	//***********
	//**  SIE  **
	//***********
	//SIE-OB
	UINT32 *OB;             // variable

	//SIE-ECS
	UINT32 *ECS_tab;        // array, size=4225, NONISO
	SIE_EmbeddedCS *ECS;    // NONISO

	//***********
	//**  RHE  **
	//***********
	//RHE-WDR
	UINT8 *WDR_strength;                // NONISO
	IQS_WDR_LCE_OFFSET *WDR_LCE_offset; // NONISO

	//RHE-LCE
    IQS_LCE *LCE_strength;    // variable

	//RHE-GCE
    SR_WDR_GCE_PARAM *GCE_strength;

	//***********
	//**  IFE  **
	//***********
	//IFE-Outlier
	UINT32 *Outl_Bri;       // array, size=5
	UINT32 *Outl_Dark;      // array, size=5
	UINT32 *Outl_Cnt;       // array, size=2
	IFE_OUTL *Outl;

	//IFE-Spatial
	UINT32 *Spatial_W;      // array, size=10
	Spatial_Filter *Spatial;// NONISO

	//IFE-NLM
	UINT8 *KER_Radious;     // array, size=8
	IFE_HAL_NLM_KER *Ker;
	UINT8 *MWTH;            // array, size=2
	UINT8 *BWA;             // array, size=8
	UINT8 *BWB;             // array, size=4
	UINT8 *BWC;             // array, size=7
	UINT8 *BWBL;            // array, size=8
	UINT8 *BWBM;            // array, size=8
	UINT8 *BWBH;            // array, size=8

	//IFE-Range
	UINT32 *RangeA_Thr;     // array, size=6
	UINT32 *RangeA_Lut;     // array, size=17
	UINT32 *RangeB_Thr;     // array, size=6
	UINT32 *RangeB_Lut;     // array, size=17
	RangeCFGB *RangeB;

	//IFE-Clamp
	RANGE_FILT_CLAMP_PARAM *CLAMP;

	//IFE-Vig
	IQS_VIG_CENTER_RATIO *Vig_Center;   // NONISO
	IQS_VIG_SET *Vig;                   // NONISO
	IQS_VIG_ISO_TH *Vig_ISO_TH;         // NONISO

	//IFE-Gbal
	UINT8  *Gbal_Stab;      // array, size=16
	UINT8  *Gbal_Dtab;      // array, size=16
	UINT32 *Gbal_IrSub;     // array, size=4
	IFE_GBAL *Gbal;

	//***********
	//**  DCE  **
	//***********
	//DCE-DIST
	DIST *DIST;             // NONISO

	//DCE-RADIOUS
	Fact_Norm  *Radious;    // NONISO

	//DCE-CFA
	DCE_CFAPARAM *CFA;      // NONISO

	//DCE-GDC
	FOV    *FOV;            // NONISO
	UINT32 *LDC_tab;        // array, size=65, NONISO
	INT32  *LDC_Rtab;       // array, size=65, NONISO
	INT32  *LDC_Btab;       // array, size=65, NONISO
	EnH    *LDC_Enh;        // NONISO

	//DCE-CAC
	Aberation *CAC;         // NONISO

    //DCE-2D
    _2DLUT_PARAM    *DCE2DPARAM;    // size=2, NONISO
    UINT32          *DCE2DLUT00;    // array, size=65, NONISO
    UINT32          *DCE2DLUT01;    // array, size=65, NONISO
    UINT32          *DCE2DLUT02;    // array, size=65, NONISO
    UINT32          *DCE2DLUT03;    // array, size=65, NONISO

	//DCE-IPL STRIPE INFO
	IPL_STRP_INFO *DCE_IPL_strp_info;

	//***********
	//**  IPE  **
	//***********
	//IPE-Edge Ext
	INT16 *Edge_Ker;        // array, size=12, NONISO
	UINT8 *EDIR_Tab;        // array, size=8, NONISO
	IPE_DEE *EDIR_Ext;
	IPE_DirScorCal *DIR_SCORE_TAB;      // NONISO
	IPE_DirScorCalTh *SCORETH;
	IPE_DEConParam *DECon;  // NONISO
	IPE_KAEdgeW *EWA;
	IPE_KBEdgeW *EWB;       // NONISO
	IPE_EdgeW *EWparam;

	//IPE-Edge Enhance
	IPE_EDGEENH *ED_Enh;

	//IPE-Edge Map
	EdgeMap  *ED_map;
	EdgeMapS *ES_map;
	UINT8  *ED_tab;         // array, size=16
	UINT8  *ES_tab;         // array, size=16

	//IPE-LPF
	LPF *LPF;

	//IPE-CC
	IQS_COLOR_TEMPERATURE_TH       *CC_TH;          // NONISO
	IQS_COLOR_TEMPERATURE_PARAM    *Color[3];
	UINT8  *FS_tab;         // array, size=16
	UINT8  *FD_tab;         // array, size=16
	IPE_CC *CC;             // NONISO

	//IPE-Y curve
	UINT32 *Y_curve;        // array, size=129, NONISO

	//IPE-Gamma
	IQS_GAMMA_LV_TH *GammaLut_TH;   // NONISO
	UINT32 *GammaLut_Day;   // array, size=129, NONISO
	UINT32 *GammaLut_Night; // array, size=129, NONISO

	//IPE-3DCC
	UINT32 *Color3D_Lut;        // array, size=900, NONISO
	IPE_3DCCRound  *Color3D_Round; // NONISO

	//IPE-CCtrl
	UINT8  *edg_tab;        // array, size=24, NONISO
	UINT8  *dds_tab;        // array, size=8, NONISO

	INT8   *saturation;     // variable
	INT8   *contrast;       // variable
	IQS_CCTRL_INFO   *cctrl_info;

	//IPE-CST
	INT16  *CST_Coef;       // array, size=9, NONISO
	IPE_CST *CST;           // NONISO

	//IPE-YCCFIX
	IPE_YCCFIX *YCC_Fix;    // NONISO

	//IPE-NoiseParam
	IPE_RAND_NR *Noise_Param;   // NONISO

	//IPE-GammaRand
	IPE_GamRand *Gamma_Rand;    // NONISO

	//***********
	//** IFE2  **
	//***********
	//IFE2-ReferenceCenter
	UINT32 *RefCenter_YTh;      // array, size=3
	UINT32 *RefCenter_YWt;      // array, size=4
	IFE2_REFCENT_Set *RefCenter_Y;
	UINT32 *RefCenter_UVTh;     // array, size=3
	UINT32 *RefCenter_UVWt;     // array, size=4
	IFE2_REFCENT_Set *RefCenter_UV;

	//IFE2-Filter
	UINT32 *IFE2Filter_YTh;     // array, size=5
	UINT32 *IFE2Filter_YWt;     // array, size=6
	UINT32 *IFE2Filter_UVTh;    // array, size=5
	UINT32 *IFE2Filter_UVWt;    // array, size=6
	IQS_IFE2_FILTER *IFE2Filter;

	//IFE2-Gray
	IFE2_GRAY_Statal *GrayStatical;     // NONISO

	//***********
	//**  IME  **
	//***********
	//IME-LCA
	IME_CHRA_ADAP *ChromaAdapt;
	IME_CHRA_CADAP *ChromaAdapt_Color;
	IME_CHRA_LADAP *ChromaAdapt_Lum;

	//IME-ChromaSuppression
	UINT32 *ChromaSupp_WeightY; // array, size=16, NONISO
	UINT32 *ChromaSupp_WeightC; // array, size=16, NONISO
	IME_ChromaSupp *ChromaSupp; // NONISO

	//IME-SuperResolution
	IME_SuperRes *SuperRes;     // NONISO

	//IME-SQUARE
	IME_HAL_3DNR_LUMA_PARAM **SQ_3NR_LUMA_Config;
	IME_HAL_3DNR_CHROMA_PARAM **SQ_3NR_CHROMA_Config;
    IME_HAL_3DNR_Y_PARAM **SQ_3NR_CHROMA_Y_Config;

	//***********
	//** Codec **
	//***********
	//Codec
	INT8   *TNR_level;      // variable
	UINT32  TNR_level_num;  // variable, NONISO
	H26XEncTnr **TNR_Config;

	//***********
	//**  HDR  **
	//***********
	//HDR
    SHDR_IQ_PARA *hdr_config;

	//***********
	//** Defog **
	//***********
	//defog
    S_DEFOG_LIB_INIT_PARA *defog_init_config;
	UINT32 *defog_wet;
} sISP_PTR;

/************************************************************/

/**
    All IQ Items for NOTE Page.
*/
typedef enum  {
	ISP_ISOINDEX,
	ISP_ISOMAP,
	ISP_ENGINE_CTRL,
	ISP_ALL_ISO,
	ISP_ALL_NONISO,
	ISP_OB,
	ISP_ECS,
	ISP_WDR,
    ISP_CE,
	ISP_OUTLIER,
	ISP_SPATIAL,
	ISP_RANGE,
	ISP_VIG,
	ISP_GBAL,
	ISP_LDC,
	ISP_EDGE,
	ISP_RGBLPF,
	ISP_CC,
	ISP_Y_CURVE,
	ISP_GAMMA,
	ISP_3DCC,
	ISP_CCTRL,
	ISP_CST,
	ISP_IFE2FILTER,
	ISP_LCA,
	ISP_CHROMA_SUPP,
	ISP_SUPER_RES,
	ISP_SQ_3DNR_CONFIG,
	ISP_TNR,
	ISP_TNR_CONFIG,
	ISP_DEFOG_CONFIG,
	ISP_FRAME_NUM,
	ISP_LDC_EXT,
	ISP_HDR_CONFIG,
	ISP_MAX
} ISP_IQ_ITEM;
/************************************************************/

/**
    All Structs for NOTE Page.
*/
// Struct of Index Information
typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 num; //fixed, cannot be changed
	UINT32 curIdx;
} sISP_ISOINDEX;

typedef struct {
	UINT32 Id;
	UINT32 size;
	ISOMAP ISOIDX_MAP[IQS_ISOMAPTAB_NUM];
} sISP_ISOMAP;

typedef struct {
	UINT32 Id;
	UINT32 size;
	ENGINE_CTRL e_ctrl;
} sISP_ENGINE_CTRL;

// Struct of all parameters which WILL change by ISO Index.
typedef struct {
	UINT32 Id;
	UINT16 ISO_index;

	//***********
	//**  SIE  **
	//***********
	//SIE-OB
	UINT32 OB[4];

	//***********
	//**  RHE  **
	//***********
    //RHE-LCE
    IQS_LCE LCE_strength;    // variable

    //RHE-GCE
    SR_WDR_GCE_PARAM GCE_strength;

	//***********
	//**  IFE  **
	//***********
	//IFE-Outlier
	UINT32 Outl_Bri[5];
	UINT32 Outl_Dark[5];
	UINT32 Outl_Cnt[2];
	IFE_OUTL Outl;

	//IFE-Spatial
	UINT32 Spatial_W[10];
	Spatial_Filter Spatial;

	//IFE-NLM
	UINT8 Ker_Radious[8];
	IFE_HAL_NLM_KER Ker;
	UINT8 MWTH[2];
	UINT8 BWA[8];
	UINT8 BWB[4];
	UINT8 BWC[7];
	UINT8 BWBL[8];
	UINT8 BWBM[8];
	UINT8 BWBH[8];

	//IFE-Range
	UINT32 RangeA_Thr[6];
	UINT32 RangeA_Lut[17];
	UINT32 RangeB_Thr[6];
	UINT32 RangeB_Lut[17];
	RangeCFGB RangeB;

	//IFE-CLAMP
	RANGE_FILT_CLAMP_PARAM CLAMP;

	//IFE-Gbal
	UINT8    Gbal_Stab[17];
	UINT8    Gbal_Dtab[17];
	UINT32   Gbal_IrSub[4];
	IFE_GBAL Gbal;

	//***********
	//**  DCE  **
	//***********

	//***********
	//**  IPE  **
	//***********
	//IPE-Edge Ext
	//IPE-Edge EXT
	INT16           Edge_Ker[12];
	UINT8           EDIR_Tab[8];
	IPE_DEE         EDIR_Ext;
	IPE_DirScorCal  DIR_SCORE_TAB;
	IPE_DirScorCalTh SCORETH;
	IPE_DEConParam  DECon;
	IPE_KAEdgeW     EWA;
	IPE_KBEdgeW     EWB;
	IPE_EdgeW       EWparam;

	//IPE-Edge Enhance
	IPE_EDGEENH ED_Enh;

	//IPE-Edge Map
	EdgeMap  ED_map;
	EdgeMapS ES_map;
	UINT8  ED_tab[16];
	UINT8  ES_tab[16];

	//IPE-LPF
	LPF LPF;

	//IPE-CC
	UINT8  FS_tab[16];
	UINT8  FD_tab[16];

	//IPE-CCtrl
	INT8   saturation;
	INT8   contrast;
    IQS_CCTRL_INFO   cctrl_info;

	//***********
	//** IFE2  **
	//***********
	//IFE2-ReferenceCenter
	UINT32 RefCenter_YTh[3];
	UINT32 RefCenter_YWt[4];
	IFE2_REFCENT_Set RefCenter_Y;
	UINT32 RefCenter_UVTh[3];
	UINT32 RefCenter_UVWt[4];
	IFE2_REFCENT_Set RefCenter_UV;

	//IFE2-Filter
	UINT32 IFE2Filter_YTh[5];
	UINT32 IFE2Filter_YWt[6];
	UINT32 IFE2Filter_UVTh[5];
	UINT32 IFE2Filter_UVWt[6];
	IQS_IFE2_FILTER IFE2Filter;

	//IFE2-Gray
	IFE2_GRAY_Statal GrayStatical;

	//***********
	//**  IME  **
	//***********
	//IME-LCA
	IME_CHRA_ADAP ChromaAdapt;
	IME_CHRA_CADAP ChromaAdapt_Color;
	IME_CHRA_LADAP ChromaAdapt_Lum;

	//IME-ChromaSuppression
	UINT32 ChromaSupp_WeightY[16];
	UINT32 ChromaSupp_WeightC[16];
	IME_ChromaSupp ChromaSupp;

	//IME-SQUARE
	UINT32 sq_luma_detail_penalty[6];
	UINT32 sq_luma_sad_penalty[8];
	UINT32 sq_luma_periodic_sad_similar[4];
	UINT32 sq_luma_candiate_pos_x[7];
	UINT32 sq_luma_candiate_pos_y[7];

	UINT32 sq_luma_motion_tensor_valmap[4];
	UINT32 sq_luma_motion_map[5];

	UINT32 sq_luma_snr_ratio[6];
	UINT32 sq_luma_tnr_ratio[6];
	UINT32 sq_luma_luma_gain[8];
	UINT32 sq_luma_freq[7];
	UINT32 sq_luma_snr_freq[4];

	IME_HAL_3DNR_LUMA_PARAM SQ_3NR_LUMA_Config;

	UINT32 sq_chroma_info_Luma_Gain[8];
	UINT32 sq_chroma_info_motion_ctrl[16];
	UINT32 sq_chroma_info_map2d_weight[4];
	UINT32 sq_chroma_info_map3D_weight[4];
	UINT32 sq_chroma_info_MixTh[4];
	UINT32 sq_chroma_info_LevelMap[16];

	IME_HAL_3DNR_CHROMA_PARAM SQ_3NR_CHROMA_Config;

	UINT32 sq_chroma_y_info_map2d_weight[4];
	UINT32 sq_chroma_y_info_map3D_weight[4];
    IME_HAL_3DNR_Y_PARAM SQ_3NR_CHROMA_Y_Config;

	//***********
	//** Codec **
	//***********
	//Codec
	UINT16 TNR_level;
	H26XEncTnr TNR_Config;
} sISP_ALL_ISO;

// Struct of all parameters which WILL NOT change by ISO Index.
typedef struct {
	UINT32 Id;

	ISOMAP ISOIDX_MAP[IQS_ISOMAPTAB_NUM];

	ENGINE_CTRL e_ctrl;

	//***********
	//**  SIE  **
	//***********
	//SIE-ECS
	UINT32 ECS_tab[4225];
	SIE_EmbeddedCS ECS;

	//***********
	//**  RHE  **
	//***********
	//RHE-WDR
	UINT8 WDR_auto_strength[21];
    IQS_WDR_LCE_OFFSET WDR_LCE_offset;

	//***********
	//**  IFE  **
	//***********
	//IFE-Vig
	IQS_VIG_CENTER_RATIO Vig_Center;
	IQS_VIG_SET Vig;
    IQS_VIG_ISO_TH Vig_ISO_TH;

	//***********
	//**  DCE  **
	//***********
	//DCE-DIST
	DIST DIST;

	//DCE-RADIOUS
	Fact_Norm Radious;

	//DCE-CFA
	DCE_CFAPARAM CFA;

	//DCE-GDC
	FOV FOV;
	UINT32 LDC_tab[65];
	INT32 LDC_Rtab[65];
	INT32 LDC_Btab[65];
	EnH LDC_Enh;

	//DCE-CAC
	Aberation CAC;

	//DCE-2D
	_2DLUT_PARAM DCE2DPARAM[2];
    UINT32 DCE2DLUT00[4225];
    UINT32 DCE2DLUT01[4225];

	//DCE-IPL STRIPE INFO
	IPL_STRP_INFO DCE_IPL_strp_info[2];

	//DCE-2D EXT
	_2DLUT_PARAM DCE2DPARAM_EXT[2];
	UINT32 DCE2DLUT02[4225];
	UINT32 DCE2DLUT03[4225];

	//DCE-IPL STRIPE INFO EXT
	IPL_STRP_INFO DCE_IPL_strp_info_EXT[2];

	//***********
	//**  IPE  **
	//***********
	//IPE-CC
	IQS_COLOR_TEMPERATURE_TH CC_TH;
	IQS_COLOR_TEMPERATURE_PARAM Color[3];
	IPE_CC CC;

	//IPE-Y curve
	UINT32 Y_curve[129];

	//IPE-Gamma
	IQS_GAMMA_LV_TH GammaLut_TH;
	UINT32 GammaLut_Day[129];
	UINT32 GammaLut_Night[129];

	//IPE-3DCC
	UINT32 Color3D_Lut[900];
	IPE_3DCCRound  Color3D_Round;

	//IPE-CCtrl
	UINT8  edg_tab[24];
	UINT8  dds_tab[8];

	//IPE-CST
	INT16  CST_Coef[9];
	IPE_CST CST;

	//IPE-YCCFIX
	IPE_YCCFIX YCC_Fix;

	//IPE-NoiseParam
	IPE_RAND_NR Noise_Param;

	//IPE-GammaRand
	IPE_GamRand Gamma_Rand;

	//***********
	//** IFE2  **
	//***********

	//***********
	//**  IME  **
	//***********
	//IME-SuperResolution
	IME_SuperRes SuperRes;

	//***********
	//** Codec **
	//***********
	//Codec
	UINT32  TNR_level_num;

	//***********
	//**  HDR  **
	//***********
	//HDR
    SHDR_IQ_PARA hdr_config;

	//***********
	//** Defog **
	//***********
	//Defog
    S_DEFOG_LIB_INIT_PARA defog_init_config;
	UINT32 defog_wet[17];
} sISP_ALL_NONISO;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//SIE-OB
	UINT32 OB[4];
} sISP_OB;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//SIE-ECS
	UINT32 ECS_tab[4225];
	SIE_EmbeddedCS ECS;
} sISP_ECS;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//RHE-WDR
	UINT32 WDR_mode;
    UINT32 WDR_manual_level;    // range:0~128
	UINT8 WDR_auto_strength[21];//size 24byte(alignment 4byte), range:0~128
    IQS_WDR_LCE_OFFSET WDR_LCE_offset;// variable
} sISP_WDR;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 ISO_index;

    //RHE-LCE
    IQS_LCE LCE_strength;    // variable

    //RHE-GCE
    SR_WDR_GCE_PARAM GCE_strength;
} sISP_CE;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Outlier
	UINT32 Outl_Bri[5];
	UINT32 Outl_Dark[5];
	UINT32 Outl_Cnt[2];
	IFE_OUTL Outl;
} sISP_OUTLIER;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Spatial
	UINT32 Spatial_W[10];
	Spatial_Filter Spatial;
} sISP_SPATIAL;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-NLM
	UINT8 Ker_Radious[8];
	IFE_HAL_NLM_KER Ker;
	UINT8 MWTH[2];
	UINT8 BWA[8];
	UINT8 BWB[4];
	UINT8 BWC[7];
	UINT8 BWBL[8];
	UINT8 BWBM[8];
	UINT8 BWBH[8];

	//IFE-Range
	UINT32 RangeA_Thr[6];
	UINT32 RangeA_Lut[17];
	UINT32 RangeB_Thr[6];
	UINT32 RangeB_Lut[17];
	RangeCFGB RangeB;

	//IFE-CLAMP
	RANGE_FILT_CLAMP_PARAM CLAMP;
} sISP_RANGE;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IFE-Vig
	IQS_VIG_CENTER_RATIO Vig_Center;
	IQS_VIG_SET Vig;
    IQS_VIG_ISO_TH Vig_ISO_TH;
} sISP_VIG;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE-Gbal
	UINT8    Gbal_Stab[17];
	UINT8    Gbal_Dtab[17];
	UINT32   Gbal_IrSub[4];
	IFE_GBAL Gbal;
} sISP_GBAL;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//DCE-DIST
	DIST DIST;

	//DCE-RADIOUS
	Fact_Norm Radious;

	//DCE-CFA
	DCE_CFAPARAM CFA;

	//DCE-GDC
	FOV FOV;
	UINT32 LDC_tab[65];
	INT32 LDC_Rtab[65];
	INT32 LDC_Btab[65];
	EnH LDC_Enh;

	//DCE-CAC
	Aberation CAC;

	//DCE-2D
	_2DLUT_PARAM DCE2DPARAM[2];
    UINT32 DCE2DLUT00[4225];
    UINT32 DCE2DLUT01[4225];

	//DCE-IPL STRIPE INFO
	IPL_STRP_INFO DCE_IPL_strp_info[2];
} sISP_LDC;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//DCE-2D EXT
	_2DLUT_PARAM DCE2DPARAM_EXT[2];
	UINT32 DCE2DLUT02[4225];
	UINT32 DCE2DLUT03[4225];

	//DCE-IPL STRIPE INFO EXT
	IPL_STRP_INFO DCE_IPL_strp_info_EXT[2];
} sISP_LDC_EXT;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IPE-Edge Ext
	INT16 Edge_Ker[12];
	UINT8 EDIR_Tab[8];
	IPE_DEE EDIR_Ext;
	IPE_DirScorCal DIR_SCORE_TAB;
	IPE_DirScorCalTh SCORETH;
	IPE_DEConParam DECon;
	IPE_KAEdgeW EWA;
	IPE_KBEdgeW EWB;
	IPE_EdgeW EWparam;

	//IPE-Edge Enhance
	IPE_EDGEENH ED_Enh;

	//IPE-Edge Map
	EdgeMap  ED_map;
	EdgeMapS ES_map;
	UINT8  ED_tab[16];
	UINT8  ES_tab[16];
} sISP_EDGE;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IPE-LPF
	LPF LPF;
} sISP_RGBLPF;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-CC
	IQS_COLOR_TEMPERATURE_TH CC_TH;
	IQS_COLOR_TEMPERATURE_PARAM Color[3];
	IPE_CC CC;

	//IPE-CCtrl
	UINT8  edg_tab[24];
	UINT8  dds_tab[8];
} sISP_CC;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-Y curve
	UINT32 Y_curve[129];
} sISP_Y_CURVE;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-Gamma
	IQS_GAMMA_LV_TH GammaLut_TH;
	UINT32 GammaLut_Day[129];
	UINT32 GammaLut_Night[129];
} sISP_GAMMA;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-3DCC
	UINT32 Color3D_Lut[900];
	IPE_3DCCRound  Color3D_Round;
} sISP_3DCC;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IPE-CC
	UINT8  FS_tab[16];
	UINT8  FD_tab[16];

	//IPE-CCtrl
	INT8   saturation;
	INT8   contrast;
    IQS_CCTRL_INFO   cctrl_info;
} sISP_CCTRL;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IPE-CST
	INT16  CST_Coef[9];
	IPE_CST CST;

	//IPE-YCCFIX
	IPE_YCCFIX YCC_Fix;

	//IPE-NoiseParam
	IPE_RAND_NR Noise_Param;

	//IPE-GammaRand
	IPE_GamRand Gamma_Rand;
} sISP_CST;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IFE2-ReferenceCenter
	UINT32 RefCenter_YTh[3];
	UINT32 RefCenter_YWt[4];
	IFE2_REFCENT_Set RefCenter_Y;
	UINT32 RefCenter_UVTh[3];
	UINT32 RefCenter_UVWt[4];
	IFE2_REFCENT_Set RefCenter_UV;

	//IFE2-Filter
	UINT32 IFE2Filter_YTh[5];
	UINT32 IFE2Filter_YWt[6];
	UINT32 IFE2Filter_UVTh[5];
	UINT32 IFE2Filter_UVWt[6];
	IQS_IFE2_FILTER IFE2Filter;

	//IFE2-Gray
	IFE2_GRAY_Statal GrayStatical;
} sISP_IFE2FILTER;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IME-LCA
	IME_CHRA_ADAP ChromaAdapt;
	IME_CHRA_CADAP ChromaAdapt_Color;
	IME_CHRA_LADAP ChromaAdapt_Lum;
} sISP_LCA;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//IME-ChromaSuppression
	UINT32 ChromaSupp_WeightY[16];
	UINT32 ChromaSupp_WeightC[16];
	IME_ChromaSupp ChromaSupp;
} sISP_CHROMA_SUPP;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//IME-SuperResolution
	IME_SuperRes SuperRes;
} sISP_SUPER_RES;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 ISO_index;

	//IME-SQUARE
	UINT32 sq_luma_detail_penalty[6];
	UINT32 sq_luma_sad_penalty[8];
	UINT32 sq_luma_periodic_sad_similar[4];
	UINT32 sq_luma_candiate_pos_x[7];
	UINT32 sq_luma_candiate_pos_y[7];

	UINT32 sq_luma_motion_tensor_valmap[4];
	UINT32 sq_luma_motion_map[5];

	UINT32 sq_luma_snr_ratio[6];
	UINT32 sq_luma_tnr_ratio[6];
	UINT32 sq_luma_luma_gain[8];
	UINT32 sq_luma_freq[7];
	UINT32 sq_luma_snr_freq[4];

	IME_HAL_3DNR_LUMA_PARAM SQ_3NR_LUMA_Config;

	UINT32 sq_chroma_info_Luma_Gain[8];
	UINT32 sq_chroma_info_motion_ctrl[16];
	UINT32 sq_chroma_info_map2d_weight[4];
	UINT32 sq_chroma_info_map3D_weight[4];
	UINT32 sq_chroma_info_MixTh[4];
	UINT32 sq_chroma_info_LevelMap[16];

	IME_HAL_3DNR_CHROMA_PARAM SQ_3NR_CHROMA_Config;

	UINT32 sq_chroma_y_info_map2d_weight[4];
	UINT32 sq_chroma_y_info_map3D_weight[4];
    IME_HAL_3DNR_Y_PARAM SQ_3NR_CHROMA_Y_Config;
} sISP_SQ_3DNR_CFG;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT16 ISO_index;

	//Codec
	UINT16 TNR_level;
	UINT32  TNR_level_num;
} sISP_TNR;

typedef struct {
	UINT32 Id;
	UINT32 size;
	UINT32 ISO_index;

	//Codec
	H26XEncTnr TNR_Config;
} sISP_TNR_CONFIG;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//RHE-Defog
	UINT32 defog_mode;
    UINT32 defog_level; // range:0~255, 1X=128
	S_DEFOG_LIB_INIT_PARA defog_init_config;
	UINT32 defog_wet[17];
}sISP_DEFOG_CONFIG;

typedef struct {
	UINT32 Id;
	UINT32 size;

	//HDR
	UINT32 hdr_mode;
    UINT32 hdr_level;   // range:0~255, 1X=128
    SHDR_TEST_PARA test_param;
    SHDR_IQ_PARA hdr_config;
}sISP_HDR_CONFIG;

extern UINT32 iqt_get_param(ISP_IQ_ITEM item, UINT32 addr, UINT32 size);
extern UINT32 iqt_get_param_update(UINT32 id, ISP_IQ_ITEM item);
extern UINT32 iqt_set_param(ISP_IQ_ITEM item, UINT32 addr, UINT32 size);
extern void iqt_set_param_update(UINT32 id, ISP_IQ_ITEM item);
extern void iqt_reset_param_update(UINT32 id, ISP_IQ_ITEM item);

extern void iqt_init_param(UINT32 Id, UINT32 iso_idx, sISP_PTR *info);
extern void iqt_init_iso_idx(UINT32 Id, sISP_ISOINDEX *addr);

extern void iq_set_sim_status(BOOL sim_status);
extern BOOL iq_get_sim_status(void);
extern void iq_set_sim_raw_addr(UINT32 addr);
extern void iq_get_sim_raw_addr(UINT32* addr);
extern void iq_set_sim_raw_info(sISP_RAW_Buffer_Info* info);
extern void iq_get_sim_raw_info(sISP_RAW_Buffer_Info* info);
extern void iq_set_sim_yuv_info(sISP_YUV_Buffer_Info* info);
extern void iq_get_sim_yuv_info(sISP_YUV_Buffer_Info* info);
extern void iq_set_raw_cb(IPL_FRONTEND_IMG img, UINT32 temp1, UINT32 temp2);

#endif
