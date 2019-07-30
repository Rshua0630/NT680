#ifndef IPL_EXT_BUF_H_
#define IPL_EXT_BUF_H_

/**
    IPLBUF_MODE
    Mode selection for IPP-Buffer-Allocation
    Add ones if necessary
*/
typedef enum {
	IPLBUF_MODE_NULL,           ///<
	IPLBUF_MODE_FLOW_STREAM,    ///< for streaming mode
	IPLBUF_MODE_CAP,            ///< for capture mode(only working buffer)
	IPLBUF_MODE_MAX_CNT,
	ENUM_DUMMY4WORD(IPLBUF_MODE)
} IPLBUF_MODE;


/**
    IPLBUF_PRV_IDX for IPLBUF_MODE_PRV
    Manage all the buffers for Image-Pipeline
    Add ones if necessary
*/
typedef enum {
	IPLBUF_SIE_H_1 = 0,  ///< for SIE output raw image header
	IPLBUF_SIE_H_2,      ///< for SIE output raw image header
	IPLBUF_SIE_H_3,      ///< for SIE output raw image header
	IPLBUF_SIE_H_4,      ///< for SIE output raw image header
	IPLBUF_SIE_H_5,      ///< for SIE output raw image header
	IPLBUF_SIE_H_6,      ///< for SIE output raw image header
	IPLBUF_SIE_H_7,      ///< for SIE output raw image header
	IPLBUF_SIE_H_8,      ///< for SIE output raw image header
	IPLBUF_SIE_H_MAX,    ///< for SIE output raw image header

	IPLBUF_SIE_CH0_1,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_2,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_3,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_4,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_5,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_6,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_7,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_8,        ///< for SIE output raw/Y image
	IPLBUF_SIE_CH0_MAX,

	IPLBUF_SIE_CH1_1,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_2,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_3,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_4,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_5,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_6,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_7,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_8,        ///< for SIE output UV image/CA
	IPLBUF_SIE_CH1_MAX,

	IPLBUF_SIE_CH2_1,        ///< LA
	IPLBUF_SIE_CH2_2,        ///< LA
	IPLBUF_SIE_CH2_3,        ///< LA
	IPLBUF_SIE_CH2_4,        ///< LA
	IPLBUF_SIE_CH2_5,        ///< LA
	IPLBUF_SIE_CH2_6,        ///< LA
	IPLBUF_SIE_CH2_7,        ///< LA
	IPLBUF_SIE_CH2_8,        ///< LA
	IPLBUF_SIE_CH2_MAX,

	IPLBUF_SIE_CH3_1,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_2,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_3,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_4,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_5,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_6,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_7,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_8,        ///< for SIE sub image/Y out/VA output
	IPLBUF_SIE_CH3_MAX,

	IPLBUF_SIE_CH4_1,    ///< for SIE CH4 output
	IPLBUF_SIE_CH4_2,    ///< for SIE CH4 output
	IPLBUF_SIE_CH4_3,    ///< for SIE CH4 output
	IPLBUF_SIE_CH4_MAX,

	IPLBUF_SIE_CH5_1,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_2,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_3,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_4,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_5,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_6,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_7,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_8,        ///< for SIE ch5 bcc sub output
	IPLBUF_SIE_CH5_MAX,

	IPLBUF_RHESUB_1,         ///< for RHE defog sub out
	IPLBUF_RHESUB_2,         ///< for RHE defog sub out
	IPLBUF_RHESUB_MAX,

	IPLBUF_IPEETH_1,        ///< for IPE ETH output
	IPLBUF_IPEETH_2,        ///< for IPE ETH output
	IPLBUF_SIEETH_MAX,

	IPLBUF_IME_SUB_OUT_1,         ///< for IME sub output
	IPLBUF_IME_SUB_OUT_2,         ///< for IME sub output
	IPLBUF_IME_SUB_OUT_MAX,

	IPLBUF_IME_PRI_MASK_1,      ///< for privacy mask
	IPLBUF_IME_PRI_MASK_2,      ///< for privacy mask
	IPLBUF_PRI_MASK_MAX,

	IPLBUF_IMEP1_SQUARE_MV_1,   ///< for path1 square mv out
	IPLBUF_IMEP1_SQUARE_MV_2,   ///< for path1 square mv out
	IPLBUF_IMEP1_SQUARE_MV_MAX,

	IPLBUF_IMEP1_SQUARE_MO_1,   ///< for path1 square mo out
	IPLBUF_IMEP1_SQUARE_MO_2,   ///< for path1 square mo out
	IPLBUF_IMEP1_SQUARE_MO_MAX,

	IPLBUF_PRV_MAX_CNT,
	ENUM_DUMMY4WORD(IPLBUF_PRV_IDX)
} IPLBUF_PRV_IDX;

//check IPLBUF_PRV_IDX range
STATIC_ASSERT((IPLBUF_PRV_MAX_CNT <= IPL_BUF_ELEMENT_MAX));

typedef enum {
	IPLBUF_DRE_DBGBUF,

	IPLBUF_CAP_MAX_CNT,
	ENUM_DUMMY4WORD(IPLBUF_CAP_IDX)
} IPLBUF_CAP_IDX;

STATIC_ASSERT((IPLBUF_CAP_MAX_CNT <= IPL_BUF_ELEMENT_MAX));

/**
     IPLBUF Assigner result

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPBUF_RST {
	ER     er_code;         ///< Description
	UINT32 buf_size;         ///< required buffer size
} IPLBUF_RST, *PIPLBUF_RST;

typedef struct {
	UINT32 win_num_h;
	UINT32 win_num_v;
	UINT32 pos_h;
	UINT32 pos_v;
} IPLBUF_IMEOUTARRANGE;

typedef struct {
	UINT32 total_win_num_h;
	UINT32 total_win_num_v;
	UINT32 pos_h;
	UINT32 pos_v;
} IPLBUF_IMEOUTGROUP;


typedef struct {
	UINT32 buf_num;
	UINT32 width;
	UINT32 height;
	UINT32 line_ofs;
	UINT32 bit;
} IPLBUF_RAWINFO;

typedef struct {
	UINT32 buf_num;
	UINT32 width;
	UINT32 height;
	UINT32 line_ofs;
	IPL_YUV_IMG_TYPE fmt;
} IPLBUF_YCCINFO;

typedef struct {
	UINT32 buf_num;
	UINT32 width;
	UINT32 height;
	UINT32 line_ofs;
	UINT32 hist_buf_ofs;
	UINT32 edge_map_buf_ofs;
} IPLBUF_IMEP4INFO;

typedef struct {
	IPL_PROC_ID id;

	BOOL sie_ch0_en;
	IPLBUF_RAWINFO sie_out_ch0;

	//ch1
	BOOL sie_ch1_en;  //output Y when ccir
	IPLBUF_YCCINFO sie_out_ch1;

	BOOL sie_ca_en;
	UINT32 sie_ca_buf_num;
	UINT32 sie_ca_buf_size;

	//ch2 - la output
	BOOL sie_la_en;
	UINT32 sie_la_buf_num;
	UINT32 sie_la_buf_size;

	//ch3
	BOOL sie_ch3_en;  //output sub-img when cap
	UINT32 sie_ch3_src;
	IPLBUF_RAWINFO sie_out_ch3;
	UINT32 sie_ch3_va_buf_size;

	//ch4
	BOOL sie_ch4_en;
	UINT32 sie_ch4_buf_num;
	UINT32 sie_ch4_buf_size;

	//ch5
	BOOL sie_ch5_en;
	UINT32 sie_ch5_buf_num;
	UINT32 sie_ch5_buf_size;

	//rhe defog sub out
	BOOL rhe_defog_sub_out_en;
	UINT32 rhe_defog_sub_out_size;

	//ipe eth out
	BOOL ipe_eth_en;
	UINT32 ipe_eth_size;

	//primacy mask
	BOOL ime_primacy_mask_en;
	UINT32 ime_primacy_mask_buf_size;

	//square out
	BOOL ime_path1_square_en;
	UINT32 ime_path1_square_mo_buf_size;
	UINT32 ime_path1_square_mv_buf_size;

	//ime sub out
	BOOL ife2_lca_en;
	UINT32 ime_sub_out_buf_num;
	UINT32 ime_sub_out_size;

	BOOL ime_path4_en;
	IPLBUF_IMEP4INFO ime_path4;

	IPLBUF_IMEOUTARRANGE ime_out_arrange;
	IPLBUF_IMEOUTGROUP ime_out_group;
} IPLBUF_ENG2DRAMINFO;

typedef IPLBUF_RST(*FPIPPBUFASSIGNER)(IPLBUF_ENG2DRAMINFO *Data);


extern IPLBUF_RST IPL_BufAssign(IPLBUF_MODE ModeIdx, IPLBUF_ENG2DRAMINFO *Data);

#endif //IPL_EXT_BUF_H_
