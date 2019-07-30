#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <fcntl.h>
#include "nvtcodec.h"

#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H

//#define DEBUG
#define VQA_TEST 1

#define DBG_WRN(fmtstr, args...) printf("\033[33mWRN:%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) printf("\033[31mERR:%s(): \033[0m" fmtstr,__func__, ##args)

#ifdef DEBUG
#define DBG_IND(fmtstr, args...) printf(fmtstr, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif

#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)

#define VIDEO_FILE_H264     "./video%luH264.raw"
#define VIDEO_FILE_MJPEG    "./video%luMJPEG.raw"
#define VIDEO_FILE_H265	    "./video%luH265.raw"
#define VIDEO_FILE_YUV        "./video%luYUV.raw"
#define SNAPSHOT_FILE       "./snapshot%lu_%u.jpg"
#define AUDIO_FILE_AAC      "./audio%luAAC.raw"
#define AUDIO_FILE_ULAW     "./audio%luULAW.raw"
#define AUDIO_FILE_ALAW     "./audio%luALAW.raw"
#define AUDIO_FILE_PCM      "./audio%luPCM.raw"
#define AUDIO_FILE_PPCM     "./audio%luPPCM.raw"

#define DEFAULT_SAVE_VIDEO_FRAME_NUM 300
#define DEFAULT_SAVE_AUDIO_FRAME_NUM 100
#define WIDTH   256
#define HEIGHT  50

typedef enum {
    CODEC_TEST_VIDEO_ENCODER,
    CODEC_TEST_VIDEO_AQINFO,
    CODEC_TEST_VIDEO_ENCODER_2CH,
    CODEC_TEST_VIDEO_SNAPSHOT,
    CODEC_TEST_VIDEO_ENCODER_ROI,
    CODEC_TEST_AUDIO_INPUT,
    CODEC_TEST_AUDIO_OUTPUT,
    CODEC_TEST_FACE_DETECTION,
    CODEC_TEST_VIDEO_OUTPUT,
    //CODEC_TEST_MOTION_DETECTION,
   // CODEC_TEST_MOTION_CONFIG,
   // CODEC_TEST_MOTION_CONFIG_GET,
   // CODEC_TEST_FACE_TRACKING,
   // CODEC_TEST_FACE_TRACKING_GET_SNAPSHOT,
   // CODEC_TEST_FACE_TRACKING_REMOVE_SNAPSHOT,
   // CODEC_TEST_BLENDING,
    CODEC_TEST_RECORD,
    CODEC_TEST_QUICK_VIDEO_CONFIG,
    CODEC_TEST_TAMPERING_DETECTION,
    CODEC_TEST_BC_SETTING,
    CODEC_TEST_BC_STATUS,
    CODEC_TEST_BC_STATUS_BLK,
    CODEC_TEST_VIDEO_QUALITY_ANALYSIS,
	CODEC_TEST_VIDEO_DATA,
   // CODEC_TEST_OSD_CONFIG_SET,
   // CODEC_TEST_OSD_CONFIG_MASK,
    CODEC_TEST_MAX,
} CodecTestItem;

BOOL bCopyBuf=NVT_TRUE;
const unsigned char * m_pStart;
unsigned short m_nLength;
int m_nCurrentBit;

unsigned char			image[HEIGHT][WIDTH];
unsigned char			alpha[HEIGHT][WIDTH];

static pthread_t vencThread0, vencThread1;
#if 0
static int init_ftype(FT_Library stLibrary, char *pcFontFile, FT_Face *pstFace, int *font_size, FT_Matrix *pstMatrix, double dAngle);
static int test_freetype_init (char* pcFontFile, int iEnable);
static void test_osd_cfg_set (char* pcFontFile, int iChannel, int iWidth, int iHeight, int iEnable);
static void draw_bitmap( FT_Bitmap*  bitmap, FT_Int x, FT_Int y);
#endif
static void show_help(void)
{
    DBG_DUMP("usage:\n"
             "  nvtcodec_test [test item] [test channel]\n");
    DBG_DUMP("  [test item] : assign test item\n");
    DBG_DUMP("    %d : video encoder test\n", CODEC_TEST_VIDEO_ENCODER);
    DBG_DUMP("    %d : video encoder adaptive quantization test\n", CODEC_TEST_VIDEO_AQINFO);
    DBG_DUMP("    %d : video encoder 2 ch test\n", CODEC_TEST_VIDEO_ENCODER_2CH);
    DBG_DUMP("    %d : video snapshot test\n", CODEC_TEST_VIDEO_SNAPSHOT);
	DBG_DUMP("    %d : video encoder ROI test\n", CODEC_TEST_VIDEO_ENCODER_ROI);
    DBG_DUMP("    %d : audio input test\n", CODEC_TEST_AUDIO_INPUT);
    DBG_DUMP("    %d : audio output test\n", CODEC_TEST_AUDIO_OUTPUT);
    DBG_DUMP("    %d : face detection test\n", CODEC_TEST_FACE_DETECTION);
    DBG_DUMP("    %d : video output test\n", CODEC_TEST_VIDEO_OUTPUT);
   // DBG_DUMP("    %d : motion status test\n", CODEC_TEST_MOTION_DETECTION);
   // DBG_DUMP("    %d : motion config set test\n", CODEC_TEST_MOTION_CONFIG);
   // DBG_DUMP("    %d : motion config get test\n", CODEC_TEST_MOTION_CONFIG_GET);
	//DBG_DUMP("    %d : face tracking test\n", CODEC_TEST_FACE_TRACKING);
   // DBG_DUMP("    %d : face tracking get snapshot test\n", CODEC_TEST_FACE_TRACKING_GET_SNAPSHOT);
   // DBG_DUMP("    %d : face tracking remove snapshot test\n", CODEC_TEST_FACE_TRACKING_REMOVE_SNAPSHOT);
   // DBG_DUMP("    %d : blending test\n", CODEC_TEST_BLENDING);
    DBG_DUMP("    %d : record start/stop\r\n", CODEC_TEST_RECORD);
    DBG_DUMP("    %d : quick set video config\r\n", CODEC_TEST_QUICK_VIDEO_CONFIG);
    DBG_DUMP("    %d : tampering detection test\n", CODEC_TEST_TAMPERING_DETECTION);
    DBG_DUMP("    %d : background constructure setting test\n", CODEC_TEST_BC_SETTING);
    DBG_DUMP("    %d : background constructure status test\n", CODEC_TEST_BC_STATUS);
    DBG_DUMP("    %d : background constructure block status test\n", CODEC_TEST_BC_STATUS_BLK);
    DBG_DUMP("    %d : video quality analysis test\n", CODEC_TEST_VIDEO_QUALITY_ANALYSIS);   
	 DBG_DUMP("    %d : save current video data\n", CODEC_TEST_VIDEO_DATA);
   // DBG_DUMP("  [test channel] : assign test video/audio channel\n\n");
   // DBG_DUMP("  OSD test\n");
   // DBG_DUMP("    nvtcodec_test %d [enable] [font file]\n", CODEC_TEST_OSD_CONFIG_SET);
   // DBG_DUMP("      [enable] : 0 is disable, 1 is enable\n");
   // DBG_DUMP("      [font file] : font file path\n");
   // DBG_DUMP("  Mask test\n");
   // DBG_DUMP("    nvtcodec_test %d [enable]\n", CODEC_TEST_OSD_CONFIG_MASK);
   // DBG_DUMP("      [enable] : 0 is disable, 1 is enable\n");
}

unsigned int read_bit(void)
{
    assert(m_nCurrentBit <= m_nLength * 8);
    int nIndex = m_nCurrentBit / 8;
    int nOffset = m_nCurrentBit % 8 + 1;

    m_nCurrentBit++;
    return (m_pStart[nIndex] >> (8-nOffset)) & 0x01;
}

unsigned int read_bits(int n)
{
    int r = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        r |= ( read_bit() << ( n - i - 1 ) );
    }
    return r;
}


INT32 test_td_rsult(INT8 td_status){

    DBG_DUMP("td status == %d\n",td_status);
    return 0;
}

unsigned int read_exponential_golomb_code(void)
{
    int r = 0;
    int i = 0;

    while( (read_bit() == 0) && (i < 32) )
    {
        i++;
    }

    r = read_bits(i);
    r += (1 << i) - 1;
    return r;
}

unsigned int read_se(void)
{
    int r = read_exponential_golomb_code();
    if (r & 0x01)
    {
        r = (r+1)/2;
    }
    else
    {
        r = -(r/2);
    }
    return r;
}

#ifdef DEBUG
void H264_SPS_parse(const unsigned char * pStart, unsigned short nLen)
{
    m_pStart = pStart;
    m_nLength = nLen;
    m_nCurrentBit = 0;
    int width=0, height=0;
    int frame_crop_left_offset=0;
    int frame_crop_right_offset=0;
    int frame_crop_top_offset=0;
    int frame_crop_bottom_offset=0;

    int profile_idc = read_bits(8);
    //int constraint_set0_flag = read_bit();
    read_bit();
    //int constraint_set1_flag = read_bit();
    read_bit();
    //int constraint_set2_flag = read_bit();
    read_bit();
    //int constraint_set3_flag = read_bit();
    read_bit();
    //int constraint_set4_flag = read_bit();
    read_bit();
    //int constraint_set5_flag = read_bit();
    read_bit();
    //int reserved_zero_2bits  = read_bits(2);
    read_bits(2);
    //int level_idc = read_bits(8);
    read_bits(8);
    //int seq_parameter_set_id = read_exponential_golomb_code();
    read_exponential_golomb_code();


    if( profile_idc == 100 || profile_idc == 110 ||
        profile_idc == 122 || profile_idc == 244 ||
        profile_idc == 44 || profile_idc == 83 ||
        profile_idc == 86 || profile_idc == 118 )
    {
        int chroma_format_idc = read_exponential_golomb_code();

        if( chroma_format_idc == 3 )
        {
            //int residual_colour_transform_flag = read_bit();
            read_bit();
        }
        //int bit_depth_luma_minus8 = read_exponential_golomb_code();
        read_exponential_golomb_code();
        //int bit_depth_chroma_minus8 = read_exponential_golomb_code();
        read_exponential_golomb_code();
        //int qpprime_y_zero_transform_bypass_flag = read_bit();
        read_bit();
        int seq_scaling_matrix_present_flag = read_bit();

        if (seq_scaling_matrix_present_flag)
        {
            int i=0;
            for ( i = 0; i < 8; i++)
            {
                int seq_scaling_list_present_flag = read_bit();
                if (seq_scaling_list_present_flag)
                {
                    int sizeOfScalingList = (i < 6) ? 16 : 64;
                    int lastScale = 8;
                    int nextScale = 8;
                    int j=0;
                    for ( j = 0; j < sizeOfScalingList; j++)
                    {
                        if (nextScale != 0)
                        {
                            int delta_scale = read_se();
                            nextScale = (lastScale + delta_scale + 256) % 256;
                        }
                        lastScale = (nextScale == 0) ? lastScale : nextScale;
                    }
                }
            }
        }
    }

    //int log2_max_frame_num_minus4 = read_exponential_golomb_code();
    read_exponential_golomb_code();
    int pic_order_cnt_type = read_exponential_golomb_code();
    if( pic_order_cnt_type == 0 )
    {
        //int log2_max_pic_order_cnt_lsb_minus4 = read_exponential_golomb_code();
        read_exponential_golomb_code();
    }
    else if( pic_order_cnt_type == 1 )
    {
        //int delta_pic_order_always_zero_flag = read_bit();
        read_bit();
        //int offset_for_non_ref_pic = read_se();
        read_se();
        //int offset_for_top_to_bottom_field = read_se();
        read_se();
        int num_ref_frames_in_pic_order_cnt_cycle = read_exponential_golomb_code();
        int i;
        for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
        {
            read_se();
            //sps->offset_for_ref_frame[ i ] = read_se();
        }
    }
    //int max_num_ref_frames = read_exponential_golomb_code();
    read_exponential_golomb_code();
    //int gaps_in_frame_num_value_allowed_flag = read_bit();
    read_bit();

    int pic_width_in_mbs_minus1 = read_exponential_golomb_code();
    int pic_height_in_map_units_minus1 = read_exponential_golomb_code();
    int frame_mbs_only_flag = read_bit();
    if( !frame_mbs_only_flag )
    {
        //int mb_adaptive_frame_field_flag = read_bit();
        read_bit();
    }
    //int direct_8x8_inference_flag = read_bit();
    read_bit();
    int frame_cropping_flag = read_bit();
    if( frame_cropping_flag )
    {
        frame_crop_left_offset = read_exponential_golomb_code();
        frame_crop_right_offset = read_exponential_golomb_code();
        frame_crop_top_offset = read_exponential_golomb_code();
        frame_crop_bottom_offset = read_exponential_golomb_code();
    }
    //int vui_parameters_present_flag = read_bit();
    read_bit();

    width = ((pic_width_in_mbs_minus1 +1)*16) - (frame_crop_right_offset + frame_crop_left_offset) * 2;
    height = ((2 - frame_mbs_only_flag)* (pic_height_in_map_units_minus1 +1) * 16) - (frame_crop_bottom_offset + frame_crop_top_offset) * 2;

    DBG_IND("Width = %d, Height = %d\n\n", width, height);
}
#endif

static void test_venc_aqinfo(UINT32 vencID){


    CodecVencSetting sVencSetting = {0};
    NVTRET ret;
    if ((ret = NvtCodec_GetVencSetting(vencID, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d setting fail! (ret=%d)\n", vencID, ret);
        return;

    }
    printf("current AQ information ===========\r\n");
    printf("ChannelID        =%d\n",vencID);
    printf("AQ enable        =%d\n", sVencSetting.sAQSetting.aq_enable);
    printf("AQ strength      =%u\n", sVencSetting.sAQSetting.aq_str);
    printf("initial i_ASlog2 =%d\n", sVencSetting.sAQSetting.sraq_init_aslog2);
    printf("max ssim aq      =%d\n", sVencSetting.sAQSetting.max_sraq);
    printf("min ssim aq      =%d\n", sVencSetting.sAQSetting.min_sraq);
    printf("fix aslog2       =%d\n",sVencSetting.sAQSetting.sraq_const_aslog2);


    printf(" AQ enable option ( value :0,1): ");
	if ((ret = scanf("%ld",&sVencSetting.sAQSetting.aq_enable)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    printf(" AQ strength option (value :1~8): ");
	if ((ret = scanf("%lu",&sVencSetting.sAQSetting.aq_str)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    printf(" initial i_ASlog2 option (value: 0~63): ");
	if ((ret = scanf("%ld",&sVencSetting.sAQSetting.sraq_init_aslog2)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    printf(" max ssim aq (value: 0~8): ");
	if ((ret = scanf("%ld", &sVencSetting.sAQSetting.max_sraq)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    printf(" min ssim aq (value: 0~-8): ");
	if ((ret = scanf("%ld",&sVencSetting.sAQSetting.min_sraq)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    printf(" fix aslog2 (0:update aslog2 by each frame, 1:use sraq_init_aslog2 for each frame): ");
	if ((ret = scanf("%ld",&sVencSetting.sAQSetting.sraq_const_aslog2)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    if ((ret = NvtCodec_SetVencSetting(vencID, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Set venc %d setting fail! (ret=%d)\n", vencID, ret);
    }


    return;
}

static void test_venc(UINT32 vencID, int saveFrameNum)
{
    FILE *pfVideo=NULL;//, *pfSnap=NULL;
    CodecVencSetting sVencSetting = {0};
    CodecVencStream sVencStream = {0};
    CodecVencParamSet sVencParamSet = {0};
    CodecVencStatus sVencStatus = {0};
    BOOL getFirstIFrame=NVT_FALSE;
    unsigned int frameCounter=0, i, selectItem;
   // unsigned int frameCounter=0, i;
    char fileName[64];//, snapFilename[64];
    NVTRET ret;
    DBG_IND("Test venc channel = %d\n", vencID);
    // Get current video encoder setting
    if ((ret = NvtCodec_GetVencSetting(vencID, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d setting fail! (ret=%d)\n", vencID, ret);
        goto TEST_VENC_END0;
    }
    DBG_DUMP("source ID           = %d\n", sVencSetting.eVencInputSource);
    DBG_DUMP("Encoded Format       = %d\n", sVencSetting.eVencFormat);
    DBG_DUMP("Resolution           = %dx%d\n", sVencSetting.uiTargetWidth, sVencSetting.uiTargetHeight);
    DBG_DUMP("NR Enable           = %d\n",sVencSetting.sNRSetting.bEnable);
    DBG_DUMP("Rotate Type          = %d\n",sVencSetting.eVencRotateType);
    DBG_DUMP("SVC-t                = %d\n", sVencSetting.sSVCSetting.eSVCTemporalLayer);
    DBG_DUMP("long-term reference = \n");
    DBG_DUMP("  LTRInterval      = %d\n",sVencSetting.sLTRInfo.uiLTRInterval);
    DBG_DUMP("  LTRPreRef        = %d\n",sVencSetting.sLTRInfo.uiLTRPreRef);
    DBG_DUMP("H264 Profile       = %d\n",sVencSetting.eVencProfile);
	DBG_DUMP("SEI setting		 = %d\n",sVencSetting.sSEISetting.bEnable);
    DBG_DUMP("Bitrate Control Mode = \n");
     DBG_DUMP("H264 Profile       = %d\n",sVencSetting.eVencProfile);
    if( sVencSetting.eBRCType == CODEC_VENC_BRC_CBR){
        DBG_DUMP("  CBR mode==\r\n");
        DBG_DUMP("    uiEnable=%d\r\n",sVencSetting.sVencCBRConfig.uiEnable);
        DBG_DUMP("    uiStaticTime=%d\r\n",sVencSetting.sVencCBRConfig.uiStaticTime);
        DBG_DUMP("    uiByteRate=%d\r\n",sVencSetting.sVencCBRConfig.uiByteRate);
        DBG_DUMP("    uiFrameRate=%d\r\n",sVencSetting.sVencCBRConfig.uiFrameRate);
        DBG_DUMP("    uiGOP=%d\r\n",sVencSetting.sVencCBRConfig.uiGOP);
        DBG_DUMP("    uiInitIQp=%d\r\n",sVencSetting.sVencCBRConfig.uiInitIQp);
        DBG_DUMP("    uiMinIQp=%d\r\n",sVencSetting.sVencCBRConfig.uiMinIQp);
        DBG_DUMP("    uiMaxIQp=%d\r\n",sVencSetting.sVencCBRConfig.uiMaxIQp);
        DBG_DUMP("    uiInitPQp=%d\r\n",sVencSetting.sVencCBRConfig.uiInitPQp);
        DBG_DUMP("    uiMinPQp=%d\r\n",sVencSetting.sVencCBRConfig.uiMinPQp);
        DBG_DUMP("    uiMaxPQp=%d\r\n",sVencSetting.sVencCBRConfig.uiMaxPQp);
        DBG_DUMP("    iIPWeight=%d\r\n",sVencSetting.sVencCBRConfig.iIPWeight);
        DBG_DUMP("    uiRowRcEnalbe=%d\r\n",sVencSetting.sVencCBRConfig.uiRowRcEnalbe);
        DBG_DUMP("    uiRowRcQpRange=%d\r\n",sVencSetting.sVencCBRConfig.uiRowRcQpRange);
        DBG_DUMP("    uiRowRcQpStep=%d\r\n",sVencSetting.sVencCBRConfig.uiRowRcQpStep);
    }
    else if(sVencSetting.eBRCType == CODEC_VENC_BRC_VBR){
        DBG_DUMP("  VBR mode==\r\n");
        DBG_DUMP("    uiEnable=%d\r\n",sVencSetting.sVencVBRConfig.uiEnable);
        DBG_DUMP("    uiStaticTime=%d\r\n",sVencSetting.sVencVBRConfig.uiStaticTime);
        DBG_DUMP("    uiByteRate=%d\r\n",sVencSetting.sVencVBRConfig.uiByteRate);
        DBG_DUMP("    uiFrameRate=%d\r\n",sVencSetting.sVencVBRConfig.uiFrameRate);
        DBG_DUMP("    uiGOP=%d\r\n",sVencSetting.sVencVBRConfig.uiGOP);
        DBG_DUMP("    uiInitIQp=%d\r\n",sVencSetting.sVencVBRConfig.uiInitIQp);
        DBG_DUMP("    uiMinIQp=%d\r\n",sVencSetting.sVencVBRConfig.uiMinIQp);
        DBG_DUMP("    uiMaxIQp=%d\r\n",sVencSetting.sVencVBRConfig.uiMaxIQp);
        DBG_DUMP("    uiInitPQp=%d\r\n",sVencSetting.sVencVBRConfig.uiInitPQp);
        DBG_DUMP("    uiMinPQp=%d\r\n",sVencSetting.sVencVBRConfig.uiMinPQp);
        DBG_DUMP("    uiMaxPQp=%d\r\n",sVencSetting.sVencVBRConfig.uiMaxPQp);
        DBG_DUMP("    iIPWeight=%d\r\n",sVencSetting.sVencVBRConfig.iIPWeight);
        DBG_DUMP("    uiChangePos=%d\r\n",sVencSetting.sVencVBRConfig.uiChangePos);
        DBG_DUMP("    uiRowRcEnalbe=%d\r\n",sVencSetting.sVencVBRConfig.uiRowRcEnalbe);
        DBG_DUMP("    uiRowRcQpRange=%d\r\n",sVencSetting.sVencVBRConfig.uiRowRcQpRange);
        DBG_DUMP("    uiRowRcQpStep=%d\r\n",sVencSetting.sVencVBRConfig.uiRowRcQpStep);
    }
    else if(sVencSetting.eBRCType == CODEC_VENC_BRC_EVBR){
        DBG_DUMP("  EVBR mode==\r\n");
        DBG_DUMP("    uiEnable=%d\r\n",sVencSetting.sVencEVBRConfig.uiEnable);
        DBG_DUMP("    uiStaticTime=%d\r\n",sVencSetting.sVencEVBRConfig.uiStaticTime);
        DBG_DUMP("    uiByteRate=%d\r\n",sVencSetting.sVencEVBRConfig.uiByteRate);
        DBG_DUMP("    uiFrameRate=%d\r\n",sVencSetting.sVencEVBRConfig.uiFrameRate);
        DBG_DUMP("    uiGOP=%d\r\n",sVencSetting.sVencEVBRConfig.uiGOP);
        DBG_DUMP("    uiKeyPPeriod=%d\r\n",sVencSetting.sVencEVBRConfig.uiKeyPPeriod);
        DBG_DUMP("    uiInitIQP=%d\r\n",sVencSetting.sVencEVBRConfig.uiInitIQp);
        DBG_DUMP("    uiMinIQp=%d\r\n",sVencSetting.sVencEVBRConfig.uiMinIQp);
        DBG_DUMP("    uiMaxIQp=%d\r\n",sVencSetting.sVencEVBRConfig.uiMaxIQp);
        DBG_DUMP("    uiInitPQP=%d\r\n",sVencSetting.sVencEVBRConfig.uiInitPQp);
        DBG_DUMP("    uiMinPQp=%d\r\n",sVencSetting.sVencEVBRConfig.uiMinPQp);
        DBG_DUMP("    uiMaxPQp=%d\r\n",sVencSetting.sVencEVBRConfig.uiMaxPQp);
        DBG_DUMP("    iIPWeight=%d\r\n",sVencSetting.sVencEVBRConfig.iIPWeight);
        DBG_DUMP("    iKeyPWeight=%d\r\n",sVencSetting.sVencEVBRConfig.iKeyPWeight);
        DBG_DUMP("    iMotionAQStrength=%d\r\n",sVencSetting.sVencEVBRConfig.iMotionAQStrength);
        DBG_DUMP("    uiRowRcEnalbe=%d\r\n",sVencSetting.sVencEVBRConfig.uiRowRcEnalbe);
        DBG_DUMP("    uiRowRcQpRange=%d\r\n",sVencSetting.sVencEVBRConfig.uiRowRcQpRange);
        DBG_DUMP("    uiRowRcQpStep=%d\r\n",sVencSetting.sVencEVBRConfig.uiRowRcQpStep);
        DBG_DUMP("    uiStillFrameCnd=%d\r\n",sVencSetting.sVencEVBRConfig.uiStillFrameCnd);
        DBG_DUMP("    uiMotionRatioThd=%d\r\n",sVencSetting.sVencEVBRConfig.uiMotionRatioThd);
        DBG_DUMP("    uiIPsnrCnd=%d\r\n",sVencSetting.sVencEVBRConfig.uiIPsnrCnd);
        DBG_DUMP("    uiPPsnrCnd=%d\r\n",sVencSetting.sVencEVBRConfig.uiPPsnrCnd);
        DBG_DUMP("    uiKeyPPsnrCnd=%d\r\n",sVencSetting.sVencEVBRConfig.uiKeyPPsnrCnd);


    }
    else if(sVencSetting.eBRCType == CODEC_VENC_BRC_FIXQP){
        DBG_DUMP("  Fix QP mode==\r\n");
        DBG_DUMP("    uiEnable=%d\r\n",sVencSetting.sVencFixQPConfig.uiEnable);
        DBG_DUMP("    uiByteRate=%d\r\n",sVencSetting.sVencFixQPConfig.uiByteRate);
        DBG_DUMP("    uiFrameRate=%d\r\n",sVencSetting.sVencFixQPConfig.uiFrameRate);
        DBG_DUMP("    uiGOP=%d\r\n",sVencSetting.sVencFixQPConfig.uiGOP);
        DBG_DUMP("    uiIFixQP=%d\r\n",sVencSetting.sVencFixQPConfig.uiIFixQP);
        DBG_DUMP("    uiPFixQP=%d\r\n",sVencSetting.sVencFixQPConfig.uiPFixQP);




    }
    else{
        DBG_DUMP("  BRC NONE mode==\r\n");
        DBG_DUMP("   FPS=    %d\r\n",sVencSetting.sVencBRCNoneInfo.uiFrameRate);
        DBG_DUMP("   GOP=    %d\r\n",sVencSetting.sVencBRCNoneInfo.uiGOP);
        DBG_DUMP("   uiByteRate=%d\r\n",sVencSetting.sVencBRCNoneInfo.uiByteRate);

    }

#if 0
    NvtCodec_GetVencStatus(vencID, &sVencStatus);
    DBG_IND("Venc Channel %u %s\n", vencID, (sVencStatus.bStart == NVT_TRUE) ? "start" : "stop");
	DBG_IND("  Max Resolution is %ux%u\n", sVencStatus.uiMaxWidth, sVencStatus.uiMaxHeight);
    DBG_DUMP("Multi Sensor Option(channel:%u)\n",vencID);
    DBG_DUMP("if Single Sensor Please set 0\n");
    DBG_DUMP("0) Sensor 0\n");
    DBG_DUMP("1) Sensor 1\n");
	DBG_DUMP("Please enter your select: ");
    if ((ret = scanf("%u", &selectItem)) < 0) {
        printf("scanf error(%d)\n", ret);
        exit(1);
    }

    sVencSetting.eVencInputSource = selectItem;
#endif
    DBG_DUMP("Encoded Format Option (current value:%d)\n",sVencSetting.eVencFormat);
    DBG_DUMP("0) MJPEG\n");
    DBG_DUMP("1) H.264\n");
    DBG_DUMP("2) H.265\n");
    DBG_DUMP("3) YUV\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    switch (selectItem)
    {
        case 3:
            sVencSetting.eVencFormat = CODEC_VENC_FORMAT_YUV;
            break;
        case 2:
            sVencSetting.eVencFormat = CODEC_VENC_FORMAT_H265;
            break;
        case 1:
            sVencSetting.eVencFormat = CODEC_VENC_FORMAT_H264;
            break;
        case 0:
            sVencSetting.eVencFormat = CODEC_VENC_FORMAT_MJPEG;
            break;
        default:
            DBG_ERR("format error! (%d)\r\n",selectItem);
    }

    /* Resolution */
    DBG_DUMP("Resolution Option (current value:%dx%d)\n",sVencSetting.uiTargetWidth,sVencSetting.uiTargetHeight);
    DBG_DUMP("0) 3840x2160\n");
    DBG_DUMP("1) 2048x1536\n");
    DBG_DUMP("2) 1920x1080\n");
    DBG_DUMP("3) 1280x720\n");
    DBG_DUMP("4) 848x480\n");
    DBG_DUMP("5) 640x480\n");
    DBG_DUMP("6) 640x360\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}


    switch (selectItem)
    {
        case 0:
            sVencSetting.uiTargetWidth = 3840;
            sVencSetting.uiTargetHeight = 2160;
            break;
        case 1:
            sVencSetting.uiTargetWidth = 2048;
            sVencSetting.uiTargetHeight = 1536;
            break;
        case 2:
            sVencSetting.uiTargetWidth = 1920;
            sVencSetting.uiTargetHeight = 1080;
            break;
        case 3:
            sVencSetting.uiTargetWidth = 1280;
            sVencSetting.uiTargetHeight = 720;
            break;
        case 4:
            sVencSetting.uiTargetWidth = 848;
            sVencSetting.uiTargetHeight = 480;
            break;
        case 5:
            sVencSetting.uiTargetWidth = 640;
            sVencSetting.uiTargetHeight = 480;
            break;
        case 6:
            sVencSetting.uiTargetWidth = 640;
            sVencSetting.uiTargetHeight = 360;
            break;
        default:
            sVencSetting.uiTargetWidth = 640;
            sVencSetting.uiTargetHeight = 360;
            break;
    }

    if(sVencSetting.eVencFormat == CODEC_VENC_FORMAT_MJPEG){
        sVencSetting.eBRCType = CODEC_VENC_BRC_NONE;
        DBG_DUMP("Bitrate Option\n");
        DBG_DUMP("0) 30720Kbps\n");
        DBG_DUMP("1) 20480Kbps\n");
        DBG_DUMP("2) 15360Kbps\n");
        DBG_DUMP("3) 8192Kbps\n");
        DBG_DUMP("4) 4096Kbps\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        switch (selectItem)
        {
            case 0:
                sVencSetting.sVencBRCNoneInfo.uiByteRate = 30720*1024/8;
                break;
            case 1:
                sVencSetting.sVencBRCNoneInfo.uiByteRate = 20480*1024/8;
                break;
            case 2:
                sVencSetting.sVencBRCNoneInfo.uiByteRate = 15360*1024/8;
                break;
            case 3:
                sVencSetting.sVencBRCNoneInfo.uiByteRate = 8192*1024/8;
                break;
            case 4:
            default:
                sVencSetting.sVencBRCNoneInfo.uiByteRate = 4096*1024/8;
                break;
        }
        DBG_DUMP("GOP Option\n");
        DBG_DUMP("Please enter your select(1~60): ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        sVencSetting.sVencBRCNoneInfo.uiGOP = selectItem;

        DBG_DUMP("FrameRate Option\n");
        DBG_DUMP("Please enter your select(1~30): ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        sVencSetting.sVencBRCNoneInfo.uiFrameRate = selectItem;

    }
    else if(sVencSetting.eVencFormat == CODEC_VENC_FORMAT_YUV){

        goto VENC_SAVE_DATA;
    }
    else{

        if(sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264){
            DBG_DUMP("H264 Profile Option(current value:%d)\n",sVencSetting.eVencProfile);
            DBG_DUMP("0) Baseline\n");
            DBG_DUMP("1) Main\n");
            DBG_DUMP("2) High\n");
            DBG_DUMP("Please enter your select: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            switch(selectItem){
                case 0:
                    sVencSetting.eVencProfile = CODEC_VENC_PROFILE_BASELINE;
                    break;
                case 1:
                    sVencSetting.eVencProfile = CODEC_VENC_PROFILE_MAIN;
                    break;
                case 2:
                    sVencSetting.eVencProfile = CODEC_VENC_PROFILE_HIGH;
                    break;
                default:
                    sVencSetting.eVencProfile = CODEC_VENC_PROFILE_HIGH;
            }
        }

        DBG_DUMP("Bitrate Type Option(current value:%d)\n",sVencSetting.eBRCType);
        DBG_DUMP("1) CBR\n");
        DBG_DUMP("2) VBR\n");
        DBG_DUMP("3) FixQP\n");
        DBG_DUMP("4) EVBR\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        switch(selectItem){
            case 4:
                sVencSetting.eBRCType = CODEC_VENC_BRC_EVBR;
               break;
            case 3:
                sVencSetting.eBRCType = CODEC_VENC_BRC_FIXQP;
                break;
            case 2:
                sVencSetting.eBRCType = CODEC_VENC_BRC_VBR;
                break;
            case 1:
                sVencSetting.eBRCType = CODEC_VENC_BRC_CBR;
                break;
            default:
                sVencSetting.eBRCType = CODEC_VENC_BRC_CBR;
        }


        if(sVencSetting.eBRCType == CODEC_VENC_BRC_CBR){


            DBG_DUMP("StaticTime Option (current value:%u)\n",sVencSetting.sVencCBRConfig.uiStaticTime);
            DBG_DUMP("Please enter your select(0~20): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiStaticTime = selectItem;

            DBG_DUMP("Target ByteRate Option (current value:%u)\n",sVencSetting.sVencCBRConfig.uiByteRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiByteRate = selectItem;

            DBG_DUMP("FrameRate Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiFrameRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiFrameRate = selectItem;


            DBG_DUMP("GOP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiGOP);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiGOP = selectItem;

            DBG_DUMP("InitIQP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiInitIQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiInitIQp = selectItem;

            DBG_DUMP("MinIQP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiMinIQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiMinIQp = selectItem;

            DBG_DUMP("MaxIQP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiMaxIQp);
            DBG_DUMP("Please enter your value(1~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiMaxIQp = selectItem;

            DBG_DUMP("InitPQP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiInitPQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiInitPQp = selectItem;

            DBG_DUMP("MinPQP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiMinPQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiMinPQp = selectItem;

            DBG_DUMP("MaxPQP Option (current value:%u)\n", sVencSetting.sVencCBRConfig.uiMaxPQp);
            DBG_DUMP("Please enter your value(1~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiMaxPQp = selectItem;

            DBG_DUMP("QP weight between I frame and P frame Option (current value:%d)\n", sVencSetting.sVencCBRConfig.iIPWeight);
            DBG_DUMP("Please enter your value(-100~100): ");
			if ((ret = scanf("%d",(int *) &sVencSetting.sVencCBRConfig.iIPWeight)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}

            DBG_DUMP("Row Level Rate Control Option (current value:%u)\n",  sVencSetting.sVencCBRConfig.uiRowRcEnalbe);
            DBG_DUMP("Please enter your value(0:Disable  1:Enable): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiRowRcEnalbe = selectItem;

            DBG_DUMP("uiRowRcQpRange Option (current value:%u)\n",  sVencSetting.sVencCBRConfig.uiRowRcQpRange);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiRowRcQpRange = selectItem;

            DBG_DUMP("uiRowRcQpStep Option (current value:%u)\n",sVencSetting.sVencCBRConfig.uiRowRcQpStep);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencCBRConfig.uiRowRcQpStep = selectItem;

        }
        else if(sVencSetting.eBRCType == CODEC_VENC_BRC_VBR){


            DBG_DUMP("StaticTime Option (current value:%u)\n", sVencSetting.sVencVBRConfig.uiStaticTime);
            DBG_DUMP("Please enter your select(0~20): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiStaticTime = selectItem;

            DBG_DUMP("Target ByteRate Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiByteRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiByteRate = selectItem;

            DBG_DUMP("FrameRate Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiFrameRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiFrameRate = selectItem;


            DBG_DUMP("GOP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiGOP);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiGOP = selectItem;

            DBG_DUMP("InitIQP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiInitIQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiInitIQp = selectItem;

            DBG_DUMP("MinIQP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiMinIQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiMinIQp = selectItem;

            DBG_DUMP("MaxIQP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiMaxIQp);
            DBG_DUMP("Please enter your value(1~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiMaxIQp = selectItem;

            DBG_DUMP("InitPQP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiInitPQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiInitPQp = selectItem;

            DBG_DUMP("MinPQP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiMinPQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiMinPQp = selectItem;

            DBG_DUMP("MaxPQP Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiMaxPQp);
            DBG_DUMP("Please enter your value(1~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiMaxPQp = selectItem;

            DBG_DUMP("IPWeight (current value:%d)\n", sVencSetting.sVencVBRConfig.iIPWeight);
            DBG_DUMP("Please enter your value(-100~100): ");
			if ((ret = scanf("%d",(int *) &sVencSetting.sVencVBRConfig.iIPWeight)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}

            DBG_DUMP("ChangePos Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiChangePos);
            DBG_DUMP("Please enter your value(0~100): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiChangePos = selectItem;

            DBG_DUMP("Row Level Rate Control Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiRowRcEnalbe);
            DBG_DUMP("Please enter your value(0:Disable  1:Enable): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiRowRcEnalbe = selectItem;

            DBG_DUMP("uiRowRcQpRange Option (current value:%u)\n",sVencSetting.sVencVBRConfig.uiRowRcQpRange);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiRowRcQpRange = selectItem;

            DBG_DUMP("uiRowRcQpStep (current value:%u)\n",sVencSetting.sVencVBRConfig.uiRowRcQpStep);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencVBRConfig.uiRowRcQpStep = selectItem;


        }
        else if(sVencSetting.eBRCType == CODEC_VENC_BRC_EVBR){


            DBG_DUMP("StaticTime Option (current value:%u)\n", sVencSetting.sVencEVBRConfig.uiStaticTime);
            DBG_DUMP("Please enter your select(0~20): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiStaticTime = selectItem;

            DBG_DUMP("Target ByteRate Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiByteRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiByteRate = selectItem;

            DBG_DUMP("FrameRate Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiFrameRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiFrameRate = selectItem;


            DBG_DUMP("GOP Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiGOP);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiGOP = selectItem;

            DBG_DUMP("uiKeyPPeriod Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiKeyPPeriod);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiKeyPPeriod = selectItem;

            DBG_DUMP("InitIQP Option (current value:%u)\n", sVencSetting.sVencEVBRConfig.uiInitIQp);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiInitIQp = selectItem;

            DBG_DUMP("MinIQP Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiMinIQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiMinIQp = selectItem;

            DBG_DUMP("MaxIQP Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiMaxIQp);
            DBG_DUMP("Please enter your value(1~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiMaxIQp = selectItem;

            DBG_DUMP("InitPQP Option (current value:%u)\n", sVencSetting.sVencEVBRConfig.uiInitPQp);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiInitPQp = selectItem;

            DBG_DUMP("MinPQP Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiMinPQp);
            DBG_DUMP("Please enter your value(0~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiMinPQp = selectItem;

            DBG_DUMP("MaxPQP Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiMaxPQp);
            DBG_DUMP("Please enter your value(1~51): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiMaxPQp = selectItem;

            DBG_DUMP("IPWeight (current value:%d)\n", sVencSetting.sVencEVBRConfig.iIPWeight);
            DBG_DUMP("Please enter your value(-100~100): ");
			if ((ret = scanf("%d",(int *) &sVencSetting.sVencEVBRConfig.iIPWeight)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}

            DBG_DUMP("iKeyPWeight Option (current value:%d)\n",sVencSetting.sVencEVBRConfig.iKeyPWeight);
            DBG_DUMP("Please enter your value(-100~100): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.iKeyPWeight = selectItem;

            DBG_DUMP("iMotionAQStrength Option (current value:%d)\n",sVencSetting.sVencEVBRConfig.iMotionAQStrength);
            DBG_DUMP("Please enter your value(-20~20): ");
			if ((ret = scanf("%d", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.iMotionAQStrength = selectItem;

            DBG_DUMP("Row Level Rate Control Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiRowRcEnalbe);
            DBG_DUMP("Please enter your value(0:Disable  1:Enable): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiRowRcEnalbe = selectItem;

            DBG_DUMP("uiRowRcQpRange Option (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiRowRcQpRange);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiRowRcQpRange = selectItem;

            DBG_DUMP("uiRowRcQpStep (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiRowRcQpStep);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiRowRcQpStep = selectItem;

            DBG_DUMP("uiStillFrameCnd (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiStillFrameCnd);
            DBG_DUMP("Please enter your value(1~Unlimited): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiStillFrameCnd = selectItem;

            DBG_DUMP("uiMotionRatioThd (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiMotionRatioThd);
            DBG_DUMP("Please enter your value(1~100): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiMotionRatioThd = selectItem;

            DBG_DUMP("uiIPsnrCnd (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiIPsnrCnd);
            DBG_DUMP("Please enter your value(0~10000): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiIPsnrCnd = selectItem;

            DBG_DUMP("uiPPsnrCnd (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiPPsnrCnd);
            DBG_DUMP("Please enter your value(0~10000): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiPPsnrCnd = selectItem;

            DBG_DUMP("uiKeyPPsnrCnd (current value:%u)\n",sVencSetting.sVencEVBRConfig.uiKeyPPsnrCnd);
            DBG_DUMP("Please enter your value(0~10000): ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencEVBRConfig.uiKeyPPsnrCnd = selectItem;
        }
        else if(sVencSetting.eBRCType == CODEC_VENC_BRC_FIXQP){


            DBG_DUMP("Target ByteRate Option (current value:%u)\n",sVencSetting.sVencFixQPConfig.uiByteRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencFixQPConfig.uiByteRate = selectItem;

            DBG_DUMP("GOP Option (current value:%u)\n",sVencSetting.sVencFixQPConfig.uiGOP);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencFixQPConfig.uiGOP = selectItem;

            DBG_DUMP("FrameRate Option (current value:%u)\n",sVencSetting.sVencFixQPConfig.uiFrameRate);
            DBG_DUMP("Please enter your value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencFixQPConfig.uiFrameRate = selectItem;

            DBG_DUMP("Fix I frame QP Option (current value:%u)\n",sVencSetting.sVencFixQPConfig.uiIFixQP);
            DBG_DUMP("Please enter yout value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencFixQPConfig.uiIFixQP = selectItem;

            DBG_DUMP("Fix P frame QP Option (current value:%u)\n",sVencSetting.sVencFixQPConfig.uiPFixQP);
            DBG_DUMP("Please enter yout value: ");
			if ((ret = scanf("%u", &selectItem)) < 0) {
				printf("scanf error(%d)\n", ret);
				exit(1);
			}
            sVencSetting.sVencFixQPConfig.uiPFixQP = selectItem;

        }


    }
    DBG_DUMP("encode NR: (current value:%d)\n",sVencSetting.sNRSetting.bEnable);
    DBG_DUMP("0) Disable\n");
    DBG_DUMP("1) Enable\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    sVencSetting.sNRSetting.bEnable = (int)(selectItem);
    DBG_DUMP("Rotate type: (current value:%d)\n",sVencSetting.eVencRotateType);
    DBG_DUMP("0) Disable\n");
    DBG_DUMP("1) BuildIn 90  degrees (Clockwise)\n");
    DBG_DUMP("2) BuildIn 270 degrees (Clockwise)\n");
    DBG_DUMP("3) EXTEND  90  degrees (Clockwise)\n");
    DBG_DUMP("4) EXTEND  180 degrees (Clockwise)\n");
    DBG_DUMP("5) EXTEND  270 degrees (Clockwise)\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    sVencSetting.eVencRotateType = selectItem;


    /* SVC-T */
    if (sVencSetting.eVencFormat != CODEC_VENC_FORMAT_MJPEG)
    {
        DBG_DUMP("SVC-T Option\n");
        DBG_DUMP("0) Disable\n");
        DBG_DUMP("1) Layer 1\n");
        DBG_DUMP("2) Layer 2\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        switch (selectItem)
        {
            case 1:
                sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_LAYER_1;
                break;
            case 2:
                if(sVencSetting.eBRCType == CODEC_VENC_BRC_CBR){
                    if((sVencSetting.sVencCBRConfig.uiGOP%2)!=0 ){
                        printf("SVC layer 2 not support this GOP value:%d\r\n",sVencSetting.sVencCBRConfig.uiGOP);
                        return;
                    }
                }
                else if(sVencSetting.eBRCType == CODEC_VENC_BRC_VBR){
                    if((sVencSetting.sVencVBRConfig.uiGOP%2)!=0 ){
                        printf("SVC layer 2 not support this GOP value:%d\r\n",sVencSetting.sVencVBRConfig.uiGOP);
                        return;
                    }
                }
                else if(sVencSetting.eBRCType == CODEC_VENC_BRC_EVBR){
                    if((sVencSetting.sVencEVBRConfig.uiGOP%2)!=0 ){
                        printf("SVC layer 2 not support this GOP value:%d\r\n",sVencSetting.sVencEVBRConfig.uiGOP);
                        return;
                    }
                }
                else if(sVencSetting.eBRCType == CODEC_VENC_BRC_FIXQP){
                    if((sVencSetting.sVencFixQPConfig.uiGOP%2)!=0 ){
                        printf("SVC layer 2 not support this GOP value:%d\r\n",sVencSetting.sVencFixQPConfig.uiGOP);
                        return;
                    }
                }
                sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_LAYER_2;

                break;
            case 0:
            default:
                sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_DISABLE;
                break;
        }
    }


    //set LTR
    if (sVencSetting.eVencFormat != CODEC_VENC_FORMAT_MJPEG){
        DBG_DUMP("Long-term reference Option: (current value:%u)\n",sVencSetting.sLTRInfo.uiLTRPreRef);
        DBG_DUMP("0) all long-term reference to IDR frame\n");
        DBG_DUMP("1) reference latest long-term reference frame\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        if(selectItem > 0){
            sVencSetting.sLTRInfo.uiLTRPreRef = 1;
        }
        else{
            sVencSetting.sLTRInfo.uiLTRPreRef = 0;
        }


        DBG_DUMP("Long-term reference Interval Option: (current value: %u)\n",sVencSetting.sLTRInfo.uiLTRInterval);
        DBG_DUMP("Please enter your value(0 ~ 4095): ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        sVencSetting.sLTRInfo.uiLTRInterval = selectItem;



    }

    //set SEI
    if ((sVencSetting.eVencFormat != CODEC_VENC_FORMAT_MJPEG) && (sVencSetting.eVencFormat != CODEC_VENC_FORMAT_YUV))
	{
        DBG_DUMP("SEI option: (current value:%u)\n",sVencSetting.sSEISetting.bEnable);
        DBG_DUMP("0) disable SEI \n");
        DBG_DUMP("1) enable SEI\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        if(selectItem > 0){
            sVencSetting.sSEISetting.bEnable = TRUE;
        }
        else{
            sVencSetting.sSEISetting.bEnable = FALSE;
        }
    }
VENC_SAVE_DATA:
    // Set new video encoder setting
    if ((ret = NvtCodec_SetVencSetting(vencID, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Set venc %d setting fail! (ret=%d)\n", vencID, ret);
        goto TEST_VENC_END0;
    }

    DBG_DUMP("Get and store video stream to file?\n");
    DBG_DUMP("0) No\n");
    DBG_DUMP("1) Yes\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    if (selectItem == 0)
        goto TEST_VENC_END0;

    switch (sVencSetting.eVencFormat)
    {
        case CODEC_VENC_FORMAT_MJPEG:
            sprintf(fileName, VIDEO_FILE_MJPEG, vencID);
            break;
        case CODEC_VENC_FORMAT_H264:
            sprintf(fileName, VIDEO_FILE_H264, vencID);
            break;
        case CODEC_VENC_FORMAT_H265:
            sprintf(fileName, VIDEO_FILE_H265, vencID);
            break;
        case CODEC_VENC_FORMAT_YUV:
            sprintf(fileName, VIDEO_FILE_YUV, vencID);
            break;
        default:
            sprintf(fileName, VIDEO_FILE_H264, vencID);
    }
	//sprintf(fileName, VIDEO_FILE_H264, vencID);
    if ((pfVideo = fopen(fileName, "wb")) == NULL)
        DBG_WRN("%s open fail, can't save encoded data\n", fileName);


    // Start video encoder
    if ((ret = NvtCodec_StartVenc(vencID)) != ER_SUCCESS)
    {
        DBG_ERR("Venc %d start fail! (ret=%d)\n", vencID, ret);
        goto TEST_VENC_END0;
    }

    NvtCodec_GetVencStatus(vencID, &sVencStatus);
    DBG_IND("Venc channel %d %s\n", vencID, (sVencStatus.bStart == NVT_TRUE) ? "start" : "stop");
    //NvtCodec_RequestVencIFrame(vencID);

    if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264 ||
		sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H265)
    {
        // Get encoder parameter set
        if ((ret = NvtCodec_RequestVencParamSet(vencID, &sVencParamSet)) != ER_SUCCESS)
        {
            DBG_ERR("Get venc parameter set fail! (ret=%d)\n", vencID, ret);
            goto TEST_VENC_END1;
        }

		DBG_IND("VPS : ");
        for (i = 0; i < sVencParamSet.VPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.VPS[i]);

		DBG_IND("\n");

        DBG_IND("SPS : ");
        for (i = 0; i < sVencParamSet.SPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.SPS[i]);

        DBG_IND("\n");

        DBG_IND("PPS : ");
        for (i = 0; i < sVencParamSet.PPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.PPS[i]);

        DBG_IND("\n");
#ifdef DEBUG
		if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264)
	        H264_SPS_parse(sVencParamSet.SPS+5, sVencParamSet.SPSLength);
#endif
    }

    DBG_DUMP("Save video encoded data to %s ...\n", fileName);

	if (saveFrameNum < 0)
		saveFrameNum = DEFAULT_SAVE_VIDEO_FRAME_NUM;

    while (frameCounter <= (unsigned int)saveFrameNum)
    {
        // Receive video encoded data
        if ((ret = NvtCodec_GetVencStream(vencID, &sVencStream, 3000, bCopyBuf)) != ER_SUCCESS)
        {
            DBG_ERR("Get venc stream fail! (ret=%d)\n", vencID, ret);
            goto TEST_VENC_END1;
        }

        if (sVencStream.eVencFormat == CODEC_VENC_FORMAT_H264 ||
			sVencStream.eVencFormat == CODEC_VENC_FORMAT_H265)
        {
            DBG_IND("Encode Format=%d, Sequence Number=%u, SVC-T ID=%u FrameType:%u\n",
                        sVencStream.eVencFormat,
                        sVencStream.uiSeqNum,
                        sVencStream.uiSVCTemporalLayerID,
                        sVencStream.uiFrameType);
        }
        else
        {
            DBG_IND("Encode Format=%d, Sequence Number=%u, \n",
                        sVencStream.eVencFormat,
                        sVencStream.uiSeqNum);
        }

        for (i = 0; i < sVencStream.uiPackNum; i++)
        {
            switch (sVencStream.eVencFormat)
            {
                case CODEC_VENC_FORMAT_YUV:
                   DBG_IND("Data Length=%d, isLastPack=%d %x %x %x\n",
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack,
                        *((char *)sVencStream.sVencPack[i].puiStreamPayloadData),
                        *((char *)(sVencStream.sVencPack[i].puiStreamPayloadData+1)),
                        *((char *)(sVencStream.sVencPack[i].puiStreamPayloadData+2))
                        );
                    if (getFirstIFrame == NVT_FALSE)
                        getFirstIFrame = NVT_TRUE;
                    break;
                case CODEC_VENC_FORMAT_MJPEG:
                    DBG_IND("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eJPEGType,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE)
                        getFirstIFrame = NVT_TRUE;
                    break;
				case CODEC_VENC_FORMAT_H265:
                    DBG_IND("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eH264Type,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE &&
                        sVencStream.sVencPack[i].uPackType.eH265Type == H265_NALU_TYPE_IDR)
                        getFirstIFrame = NVT_TRUE;
					break;
                case CODEC_VENC_FORMAT_H264:
                default:
                    DBG_IND("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eH264Type,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE &&
                        sVencStream.sVencPack[i].uPackType.eH264Type == H264_NALU_TYPE_IDR)
                        getFirstIFrame = NVT_TRUE;
                    break;
            }
        }
        // Save video encoded data from first I-frame
        if (getFirstIFrame == NVT_TRUE)
        {
            for (i = 0; i < sVencStream.uiPackNum; i++)
                fwrite(sVencStream.sVencPack[i].puiStreamPayloadData, sVencStream.sVencPack[i].uiDataLength, 1, pfVideo);

            frameCounter++;
        }

        // Release video encoded data
        NvtCodec_ReleaseVencStream(vencID, &sVencStream);
    }

TEST_VENC_END1:
    // Stop video encoder
    NvtCodec_StopVenc(vencID);

TEST_VENC_END0:
    if (pfVideo)
    {
        fflush(pfVideo);
        fclose(pfVideo);
    }
}

static void test_venc_direct(UINT32 vencID)
{
#if 0
    FILE *pfVideo=NULL, *pfSnap=NULL;
    CodecVencSetting sVencSetting;
    CodecVencStream sVencStream;
    CodecVencSnapshot sVencSnapshot;
    CodecVencParamSet sVencParamSet;
    CodecVencStatus sVencStatus;
    BOOL getFirstIFrame=NVT_FALSE;
    unsigned int frameCounter=0, i;
    char fileName[64], snapFilename[64];
    NVTRET ret;

    DBG_IND("Test venc channel = %d\n", vencID);

    // Get current video encoder setting
    if ((ret = NvtCodec_GetVencSetting(vencID, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d setting fail! (ret=%d)\n", vencID, ret);
        goto TEST_VENC_END0;
    }

    DBG_IND("Encoded Format       = %s\n", (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_MJPEG) ? "MJPEG" : "H.264");
    DBG_IND("Resolution           = %dx%d\n", sVencSetting.uiTargetWidth, sVencSetting.uiTargetHeight);
    DBG_IND("FPS                  = %d\n", sVencSetting.uiTargetFrameRate);
    DBG_IND("GOP                  = %d\n", sVencSetting.sVencBitRateCtrl.uiGOP);
    DBG_IND("SVC-t                = %d\n", sVencSetting.sSVCSetting.eSVCTemporalLayer);
 //   DBG_IND("Crop                 = %s\n", sVencSetting.sVencCrop.bCropEnable == NVT_TRUE ? "On" : "Off");
 //   DBG_IND("  Position           = (%u, %u)\n", sVencSetting.sVencCrop.uiCoordinateX, sVencSetting.sVencCrop.uiCoordinateY);
#if 0
    DBG_IND("Bitrate Control Mode = ");
    if (sVencSetting.sVencBitRateCtrl.eBitrateMode == CODEC_VENC_BIT_RATE_CTRL_CBR)
    {
        DBG_IND("CBR\n");
        DBG_IND("  Target Bitrate   = %u Kbps\n", sVencSetting.sVencBitRateCtrl.uiTargetBitrate);
        DBG_IND("  Statistics Time  = %f second\n", sVencSetting.sVencBitRateCtrl.fStatisticsTime);
        DBG_IND("  QP Delta         = %u\n", sVencSetting.sVencBitRateCtrl.uiQPDelta);
        DBG_IND("  QP Frame Rate    = %u\n", sVencSetting.sVencBitRateCtrl.uiQPFrameRate);
        DBG_IND("  IP Weight        = %u\n", sVencSetting.sVencBitRateCtrl.uiIPWeight);
		DBG_IND("  Bitrate Tolerance= %u\n", sVencSetting.sVencBitRateCtrl.uiBitrateTolerance);
        DBG_IND("  Max QP           = %u\n", sVencSetting.sVencBitRateCtrl.uiMaxQP);
    }
    else
    {
        DBG_IND("VBR\n");
        DBG_IND("  Max QP           = %u\n", sVencSetting.sVencBitRateCtrl.uiMaxQP);
        DBG_IND("  Min QP           = %u\n", sVencSetting.sVencBitRateCtrl.uiMinQP);
        DBG_IND("  Max Bitrate      = %u Kbps\n", sVencSetting.sVencBitRateCtrl.uiMaxBitrate);
		DBG_IND("  Bitrate Tolerance= %u\n", sVencSetting.sVencBitRateCtrl.uiBitrateTolerance);
        DBG_IND("  QP GOP Delta     = %u\n", sVencSetting.sVencBitRateCtrl.uiQPGOPDelta);
    }
#endif
    NvtCodec_GetVencStatus(vencID, &sVencStatus);
    DBG_IND("Venc channel %d %s\n", vencID, (sVencStatus.bStart == NVT_TRUE) ? "start" : "stop");

    switch (sVencSetting.eVencFormat)
    {
        case CODEC_VENC_FORMAT_MJPEG:
            sprintf(fileName, VIDEO_FILE_MJPEG, vencID);
            break;
        case CODEC_VENC_FORMAT_H264:
        default:
            sprintf(fileName, VIDEO_FILE_H264, vencID);
            break;
    }

    if ((pfVideo = fopen(fileName, "wb")) == NULL)
        DBG_WRN("%s open fail, can't save encoded data\n", fileName);


    // Start video encoder
    if ((ret = NvtCodec_StartVenc(vencID)) != ER_SUCCESS)
    {
        DBG_ERR("Venc %d start fail! (ret=%d)\n", vencID, ret);
        goto TEST_VENC_END0;
    }

    NvtCodec_GetVencStatus(vencID, &sVencStatus);
    DBG_IND("Venc channel %d %s\n", vencID, (sVencStatus.bStart == NVT_TRUE) ? "start" : "stop");


    if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264 ||
		sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H265)
    {
        // Get encoder parameter set
        if ((ret = NvtCodec_RequestVencParamSet(vencID, &sVencParamSet)) != ER_SUCCESS)
        {
            DBG_ERR("Get venc parameter set fail! (ret=%d)\n", vencID, ret);
            goto TEST_VENC_END1;
        }

		DBG_IND("VPS : ");
        for (i = 0; i < sVencParamSet.VPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.VPS[i]);

        DBG_IND("SPS : ");
        for (i = 0; i < sVencParamSet.SPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.SPS[i]);

        DBG_IND("\n");

        DBG_IND("PPS : ");
        for (i = 0; i < sVencParamSet.PPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.PPS[i]);

        DBG_IND("\n");
#ifdef DEBUG
		if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264)
	        H264_SPS_parse(sVencParamSet.SPS+5, sVencParamSet.SPSLength);
#endif
    }

    DBG_DUMP("Save video encoded data to %s ...\n", fileName);

    while (frameCounter < DEFAULT_SAVE_VIDEO_FRAME_NUM)
    {
        // Receive video encoded data
        if ((ret = NvtCodec_GetVencStream(vencID, &sVencStream, 3000, bCopyBuf)) != ER_SUCCESS)
        {
            DBG_ERR("Get venc stream fail! (ret=%d)\n", vencID, ret);
            goto TEST_VENC_END1;
        }

        if (sVencStream.eVencFormat == CODEC_VENC_FORMAT_H264)
        {
            DBG_IND("Encode Format=%d, Sequence Number=%u, SVC-T ID=%u\n",
                        sVencStream.eVencFormat,
                        sVencStream.uiSeqNum,
                        sVencStream.uiSVCTemporalLayerID);
        }
        else
        {
            DBG_IND("Encode Format=%d, Sequence Number=%u, \n",
                        sVencStream.eVencFormat,
                        sVencStream.uiSeqNum);
        }

        for (i = 0; i < sVencStream.uiPackNum; i++)
        {
            switch (sVencStream.eVencFormat)
            {
                case CODEC_VENC_FORMAT_MJPEG:
                    DBG_IND("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eJPEGType,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE)
                        getFirstIFrame = NVT_TRUE;
                    break;
                case CODEC_VENC_FORMAT_H264:
                default:
                    DBG_IND("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eH264Type,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE &&
                        sVencStream.sVencPack[i].uPackType.eH264Type == H264_NALU_TYPE_IDR)
                        getFirstIFrame = NVT_TRUE;
                    break;
            }
        }

        // Get one snapshot every 64 frames
#if 0
        if (getFirstIFrame == NVT_TRUE && (frameCounter & 0x3F) == 0)
        {
            sprintf(snapFilename, SNAPSHOT_FILE, vencID, frameCounter >> 6);
            if ((pfSnap = fopen(snapFilename, "wb")) == NULL)
                DBG_WRN("%s open fail, can't save encoded data\n", snapFilename);
            else
            {
                if ((ret = NvtCodec_RequestVencJPEG(vencID, &sVencSnapshot)) != ER_SUCCESS)
                    DBG_ERR("Get snapshot fail! (ret=%d)\n", vencID, ret);
                else
                {
                    DBG_DUMP("Save sanpshot to %s ...\n", snapFilename);
                    fwrite(sVencSnapshot.uiSnapshotData, sVencSnapshot.uiDatatLength, 1, pfSnap);
                }

                fflush(pfSnap);
                fclose(pfSnap);
            }
        }
#endif
        // Save video encoded data from first I-frame
        if (getFirstIFrame == NVT_TRUE)
        {
            for (i = 0; i < sVencStream.uiPackNum; i++)
                fwrite(sVencStream.sVencPack[i].puiStreamPayloadData, sVencStream.sVencPack[i].uiDataLength, 1, pfVideo);

            frameCounter++;
        }

        // Release video encoded data
        NvtCodec_ReleaseVencStream(vencID, &sVencStream);
    }

TEST_VENC_END1:
    // Stop video encoder
    NvtCodec_StopVenc(vencID);

TEST_VENC_END0:
    if (pfVideo)
    {
        fflush(pfVideo);
        fclose(pfVideo);
    }
#endif
}

static void vencThreadFunc(void *argv)
{
    test_venc_direct(*(UINT32 *)argv);
    return;
}

static void test_venc_2ch(void)
{
    UINT32 vencID0=0, vencID1=1;
	int ret = 0;
    ret = pthread_create(&vencThread0, NULL, (void *)vencThreadFunc, (void*)(&vencID0));
	if(ret < 0 )
	{	
		printf("pthread create fail (%d)\n",ret);
		return;
	}
    ret = pthread_create(&vencThread1, NULL, (void *)vencThreadFunc, (void*)(&vencID1));
	if(ret < 0 )
    {
        printf("pthread create fail (%d)\n",ret);
        return;
    }
    pthread_join(vencThread0, NULL);
    pthread_join(vencThread1, NULL);
}
static void test_venc_roi(UINT32 vencID)
{
    CodecVencROISetting sVencROISetting={0};
    unsigned int selectItem;
    NVTRET ret;

    DBG_DUMP("Test venc channel = %d\n", vencID);


    // Get current video encoder ROI setting
    if ((ret = NvtCodec_GetVencROISetting(vencID,&sVencROISetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d ROI setting fail! (ret=%d)\n", vencID, ret);
        return;
    }
    int i=0;

    DBG_DUMP("=========ROI INFO====\n");
    DBG_DUMP("DeltaQp     = %u\r\n",sVencROISetting.uiDeltaQp);
    DBG_DUMP("SmartROI     = %u\r\n",sVencROISetting.bSmartROI);

    for(i=0;i< 10;i++){

        DBG_DUMP("===Normal ROI index=%d\n",i);
        DBG_DUMP("ROI enable   = %s\n", sVencROISetting.sROIConfig[i].bROIEnable == NVT_TRUE ? "On" : "Off");
        DBG_DUMP("Position     = (%u, %u)\n", sVencROISetting.sROIConfig[i].uiX, sVencROISetting.sROIConfig[i].uiY);
        DBG_DUMP("Size         = (%u, %u)\n", sVencROISetting.sROIConfig[i].uiWidth, sVencROISetting.sROIConfig[i].uiHeight);
        DBG_DUMP("QP        = %d\n", sVencROISetting.sROIConfig[i].iQP);


    }


    DBG_DUMP("SmartROI Option\n");
    DBG_DUMP("0) Disable Smart ROI\n");
    DBG_DUMP("1) Enable  Smart ROI (Please Disable Ch:%d all Normal ROI  first)\n",vencID);
    if((ret = scanf("%d",&selectItem)) < 0)
	{
		printf("scanf error(%d)\n", ret);
        exit(1);
	}
    sVencROISetting.bSmartROI = selectItem;

    if(sVencROISetting.bSmartROI == 1){
        goto ROI_SET;
    }

    /*
     If you want to change DeltaQp Option, you need close all window first, and then set DeltaQp and open window again
    */
    DBG_DUMP("ROI DeltaQp Option\n");
    DBG_DUMP("0) All fixed QP\n");
    DBG_DUMP("1) All delta QP\n");
	if((ret = scanf("%d",&selectItem)) < 0)
	{
		printf("scanf error(%d)\n", ret);
        exit(1);
	}
    sVencROISetting.uiDeltaQp = selectItem;

    DBG_DUMP("ROI index Option (value:0~9) ");
	if ((ret = scanf("%d",&i)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
	if(i<0)
		i = 0;
	else if(i > 9)
		i = 9;

    DBG_DUMP("ROI Option\n");
    DBG_DUMP("0) Disable\n");
    DBG_DUMP("1) Enable\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    switch (selectItem)
    {
        case 1:
            sVencROISetting.sROIConfig[i].bROIEnable = NVT_TRUE;
            break;
        case 0:
        default:
            sVencROISetting.sROIConfig[i].bROIEnable = NVT_FALSE;
            break;
    }

    if (sVencROISetting.sROIConfig[i].bROIEnable == NVT_TRUE)
    {
        DBG_DUMP("ROI X Position\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%lu", &(sVencROISetting.sROIConfig[i].uiX))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("ROI Y Position\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%lu", &(sVencROISetting.sROIConfig[i].uiY))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("ROI Width\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%lu", &(sVencROISetting.sROIConfig[i].uiWidth))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("ROI Height\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%lu", &(sVencROISetting.sROIConfig[i].uiHeight))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

		DBG_DUMP("ROI QP (if DeltaQp=0, 1~51 ; if DeltaQp=1, -32~31)\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%lu", &(sVencROISetting.sROIConfig[i].iQP))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
    }

ROI_SET:
    // Set new video encoder ROI setting
    if ((ret = NvtCodec_SetVencROISetting(vencID, &sVencROISetting)) != ER_SUCCESS)
    {
        DBG_ERR("Set venc %d ROI setting fail! (ret=%d)\n", vencID, ret);
        return;
    }

    return;
}
static void test_ainput(UINT32 ainID, int saveFrameNum)
{
    FILE *pfAudio=NULL;
    CodecAinSetting sAinSetting = {0};
    CodecAencStream sAencStream = {0};
    unsigned int frameCounter=0, selectItem;
    char fileName[64];
    NVTRET ret;

    DBG_IND("Test audio input channel = %d\n", ainID);

    // Get current audio input setting
    if ((ret = NvtCodec_GetAinSetting(ainID, &sAinSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get ain %d setting fail! (ret=%d)\n", ainID, ret);
        goto TEST_AIN_END0;
    }

    DBG_DUMP("Codec       = ");
    switch (sAinSetting.eAinFormat)
    {
        case CODEC_AENC_FORMAT_AAC:
            DBG_DUMP("AAC\n");
            break;
        case CODEC_AENC_FORMAT_ULAW:
            DBG_DUMP("ulaw\n");
            break;
        case CODEC_AENC_FORMAT_ALAW:
            DBG_DUMP("alaw\n");
            break;
        case CODEC_AENC_FORMAT_PCM:
        default:
            DBG_DUMP("PCM\n");
            break;
    }

    DBG_DUMP("Sample Rate = ");
    switch (sAinSetting.eAinSampleRate)
    {
        case CODEC_AIO_SAMPLE_RATE_8000:
            DBG_DUMP("8KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_11025:
            DBG_DUMP("11.025KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_12000:
            DBG_DUMP("12KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_16000:
            DBG_DUMP("16KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_22050:
            DBG_DUMP("22.05KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_24000:
            DBG_DUMP("24KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_32000:
            DBG_DUMP("32KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_44100:
            DBG_DUMP("44.1KHz\n");
            break;
        case CODEC_AIO_SAMPLE_RATE_48000:
        default:
            DBG_DUMP("48KHz\n");
            break;
    }

    DBG_DUMP("Sound Mode  = ");
    switch (sAinSetting.eAinSoundMode)
    {
        case CODEC_AIO_SOUND_MODE_STEREO:
            DBG_DUMP("stereo\n");
            break;
        case CODEC_AIO_SOUND_MODE_LEFT:
            DBG_DUMP("left\n");
            break;
        case CODEC_AIO_SOUND_MODE_RIGHT:
        default:
            DBG_DUMP("right\n");
            break;
    }

    DBG_DUMP("Duplex Mode = ");
    switch (sAinSetting.eAinDuplexMode)
    {
        case CODEC_AIO_DUPLEX_MODE_HALF:
            DBG_DUMP("half\n");
            break;
        case CODEC_AIO_DUPLEX_MODE_FULL:
        default:
            DBG_DUMP("full\n");
            break;
    }

    DBG_DUMP("Volume      = %lu\n", sAinSetting.uiAinVolume);
    DBG_DUMP("Audio Noise Reduction enable:%s\n",sAinSetting.sAinNRSetting.bNREnable==0?"off":"on");
    DBG_DUMP("Audio Noise Reduction maximum suppression level of noise:%ld\n",sAinSetting.sAinNRSetting.iNRdB);
    DBG_DUMP("Audio Noise Reduction cut-off frequency of HPF pre-filtering:%ld\n",sAinSetting.sAinNRSetting.iHPFCutOffFreq);
    DBG_DUMP("Audio Noise Reduction bias sensitive:%ld\n",sAinSetting.sAinNRSetting.iBiasSensitive);
	DBG_DUMP("Audio Filter enable::%s\n",sAinSetting.sAinAudFiltSetting.bFiltEnable==0?"off":"on");


    DBG_DUMP("Audio Noise Redunction Option\n");
    DBG_DUMP("0) Disable\n");
    DBG_DUMP("1) Enable\n");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    switch(selectItem){

        case 0:
            sAinSetting.sAinNRSetting.bNREnable = NVT_FALSE;
            break;
        case 1:
        default:
           sAinSetting.sAinNRSetting.bNREnable = NVT_TRUE;
           break;

    }

    if(sAinSetting.sAinNRSetting.bNREnable == NVT_TRUE){
        DBG_DUMP("ANR maximum suppression level of noise\n");
        DBG_DUMP("Please enter your select(3~35): ");
		if ((ret = scanf("%ld", &sAinSetting.sAinNRSetting.iNRdB)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("ANR cut-off frequency of HPF pre-filtering\n");
        DBG_DUMP("Please enter your select(Hz): ");
		if ((ret = scanf("%ld", &sAinSetting.sAinNRSetting.iHPFCutOffFreq)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("ANR bias sensitive\n");
        DBG_DUMP("Please enter your select(1~9): ");
		if ((ret = scanf("%ld", &sAinSetting.sAinNRSetting.iBiasSensitive)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
    }

    DBG_DUMP("Format Option\n");
    DBG_DUMP("0) AAC\n");
    DBG_DUMP("1) PCM\n");
    DBG_DUMP("2) alaw\n");
    DBG_DUMP("3) ulaw\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    switch (selectItem)
    {
        case 0:
            sprintf(fileName, AUDIO_FILE_AAC, ainID);
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_AAC;
            break;
        case 1:
            sprintf(fileName, AUDIO_FILE_PCM, ainID);
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_PCM;
            break;
        case 2:
            sprintf(fileName, AUDIO_FILE_ALAW, ainID);
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_ALAW;
            break;
        case 3:
            sprintf(fileName, AUDIO_FILE_ULAW, ainID);
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_ULAW;
            break;
        default:
            sprintf(fileName, AUDIO_FILE_PCM, ainID);
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_PCM;
            break;
    }

    DBG_DUMP("Sample Rate Option\n");
    DBG_DUMP("0) 8KHz\n");
    DBG_DUMP("1) 11.025KHz\n");
    DBG_DUMP("2) 12KHz\n");
    DBG_DUMP("3) 16KHz\n");
    DBG_DUMP("4) 22.05KHz\n");
    DBG_DUMP("5) 24KHz\n");
    DBG_DUMP("6) 32KHz\n");
    DBG_DUMP("7) 44.1KHz\n");
    DBG_DUMP("8) 48KHz\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    switch (selectItem)
    {
        case 0:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_8000;
            break;
        case 1:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_11025;
            break;
        case 2:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_12000;
            break;
        case 3:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_16000;
            break;
        case 4:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_22050;
            break;
        case 5:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_24000;
            break;
        case 6:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_32000;
            break;
        case 7:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_44100;
            break;
        case 8:
        default:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_48000;
            break;
    }

    DBG_DUMP("Sound Mode Option\n");
    DBG_DUMP("0) stereo\n");
    DBG_DUMP("1) left\n");
    DBG_DUMP("2) right\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    switch (selectItem)
    {
        case 0:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_STEREO;
            break;
        case 1:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
            break;
        case 2:
        default:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_RIGHT;
            break;
    }

    DBG_DUMP("Duplex Mode Option\n");
    DBG_DUMP("0) half\n");
    DBG_DUMP("1) full\n");
    DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &(sAinSetting.eAinDuplexMode))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

    DBG_DUMP("Volume Option\n");
    DBG_DUMP("Please enter your select(0~8): ");
		if ((ret = scanf("%lu", &(sAinSetting.uiAinVolume))) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

	DBG_DUMP("Audio Filter Option\n");
	DBG_DUMP("0) Disable\n");
	DBG_DUMP("1) Enable\n");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
	switch(selectItem){
		case 0:
			sAinSetting.sAinAudFiltSetting.bFiltEnable = NVT_FALSE;
			break;
		case 1:
		default:
			sAinSetting.sAinAudFiltSetting.bFiltEnable = NVT_TRUE;
			break;
	}

    // Set new audio input setting
    if ((ret = NvtCodec_SetAinSetting(ainID, &sAinSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Set ain %d setting fail! (ret=%d)\n", ainID, ret);
        goto TEST_AIN_END0;
    }

    DBG_DUMP("Get and store audio stream to file?\n");
    DBG_DUMP("0) No\n");
    DBG_DUMP("1) Yes\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    if (selectItem == 0)
        goto TEST_AIN_END0;

    if ((pfAudio = fopen(fileName, "wb")) == NULL)
        DBG_WRN("%s open fail, can't save encoded data\n", fileName);

    // Start audio input
    if ((ret = NvtCodec_StartAin(ainID)) != ER_SUCCESS)
    {
        DBG_ERR("Ain %d start fail! (ret=%d)\n", ainID, ret);
        goto TEST_AIN_END0;
    }

    DBG_DUMP("Save audio encoded data to %s ...\n", fileName);

	if (saveFrameNum <= 0)
		saveFrameNum = DEFAULT_SAVE_AUDIO_FRAME_NUM;

    while (frameCounter < (unsigned int)saveFrameNum)
    {
        // Receive audio encoded data
        if ((ret = NvtCodec_GetAinStream(ainID, &sAencStream, 3000, bCopyBuf)) != ER_SUCCESS)
        {
            DBG_ERR("Get ain stream fail! (ret=%d)\n", ainID, ret);
            goto TEST_AIN_END1;
        }

        DBG_IND("Encode Format=%d, Sequence Number=%d, Data Length=%d\n",
                    sAencStream.eAinFormat,
                    sAencStream.uiSeqNum,
                    sAencStream.uiDataLength);

        fwrite(sAencStream.puiStreamPayloadData, sAencStream.uiDataLength, 1, pfAudio);

        // Release audio encoded data
        NvtCodec_ReleaseAinStream(ainID, &sAencStream);
        frameCounter++;
    }

TEST_AIN_END1:
    // Stop audio input
    NvtCodec_StopAin(ainID);

TEST_AIN_END0:
    if (pfAudio)
    {
        fflush(pfAudio);
        fclose(pfAudio);
    }
}

static void test_aout(UINT32 aoutID)
{
    CodecAoutSetting sAoutSetting={0};
    CodecAinSetting sAinSetting = {0};
    //CodecAoutStatus sAoutStatus={0};
    unsigned int selectItem, sampleRate;
    unsigned long sleepUs;
    char testFilePath[128]={0};
    int fd=-1, isWAVFormat=0;
    NVTRET ret;

    DBG_IND("Test aoutID channel = %d\n", aoutID);

    // Get current audio output setting
    if ((ret = NvtCodec_GetAoutSetting(aoutID, &sAoutSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get aout %d setting fail! (ret=%d)\n", aoutID, ret);
        return;
    }

    //DBG_IND("Audio Output Enable = %s\n", (sAoutStatus.bStart == NVT_TRUE) ? "On" : "Off");
    DBG_DUMP("Output Target = ");
    switch (sAoutSetting.eAoutTarget)
    {
        case CODEC_AOUT_TARGET_LINEOUT:
            DBG_DUMP("Line Out\n");
            break;
        case CODEC_AOUT_TARGET_SPEAKER:
        default:
            DBG_DUMP("Speaker\n");
            break;
    }

    DBG_DUMP("Sound Mode    = ");
    switch (sAoutSetting.eAoutSoundMode)
    {
        case CODEC_AIO_SOUND_MODE_STEREO:
            DBG_DUMP("stereo\n");
            break;
        case CODEC_AIO_SOUND_MODE_LEFT:
            DBG_DUMP("left\n");
            break;
        case CODEC_AIO_SOUND_MODE_RIGHT:
        default:
            DBG_DUMP("right\n");
            break;
    }

    DBG_DUMP("Codec    = %d\r\n",sAoutSetting.eAoutFormat);

    DBG_DUMP("Volume        = %lu\n", sAoutSetting.uiAoutVolume);
    DBG_DUMP("AEC Mode          = %s\n", sAoutSetting.sAoutAECSetting.bAECEnable == NVT_TRUE ? "On" : "Off");
    DBG_DUMP("AEC LeakEstEnable = %d\n", sAoutSetting.sAoutAECSetting.bLeakEstEnable);
    DBG_DUMP("AEC LeakEstValue  = %d\n", sAoutSetting.sAoutAECSetting.iLeakEstValue);
    DBG_DUMP("AEC NoiseCancelLevel  = %d\n", sAoutSetting.sAoutAECSetting.iNoiseCancelLevel);
    DBG_DUMP("AEC EchoCancelLevel   = %d\n", sAoutSetting.sAoutAECSetting.iEchoCancelLevel);
    DBG_DUMP("AEC FilterLen         = %d\n", sAoutSetting.sAoutAECSetting.uiFilterLen);
    DBG_DUMP("AEC uiFrameSize       = %d\n", sAoutSetting.sAoutAECSetting.uiFrameSize);
    DBG_DUMP("AEC uiNotchRadius     = %d\n", sAoutSetting.sAoutAECSetting.uiNotchRadius);



    DBG_DUMP("Audio Output Codec\n");
    DBG_DUMP("0) PCM\r\n");
    DBG_DUMP("2) uLaw\r\n");
    DBG_DUMP("3) aLaw\r\n");
    DBG_DUMP("Please enter your select: ");

	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    switch (selectItem)
    {
        case 0:
        case 2:
        case 3:
            sAoutSetting.eAoutFormat = selectItem;
            break;
        default:
            sAoutSetting.eAoutFormat = 1;
            break;
    }

    DBG_DUMP("Audio Output Target Option\n");
    DBG_DUMP("0) Line Out\n");
    DBG_DUMP("1) Speaker\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &(sAoutSetting.eAoutTarget))) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    DBG_DUMP("Sound Mode Option\n");
    DBG_DUMP("0) stereo\n");
    DBG_DUMP("1) left\n");
    DBG_DUMP("2) right\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    switch (selectItem)
    {
        case 0:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_STEREO;
            break;
        case 1:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
            break;
        case 2:
        default:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_RIGHT;
            break;
    }

    DBG_DUMP("Volume Option\n");
    DBG_DUMP("Please enter your select(0~64): ");
	if ((ret = scanf("%lu", &(sAoutSetting.uiAoutVolume))) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    DBG_DUMP("AEC Mode Option\n");
    DBG_DUMP("0) Disable\n");
    DBG_DUMP("1) Enable\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    switch (selectItem)
    {
        case 1:
            sAoutSetting.sAoutAECSetting.bAECEnable = NVT_TRUE;
            break;
        case 0:
        default:
            sAoutSetting.sAoutAECSetting.bAECEnable = NVT_FALSE;
            break;
    }


    if(sAoutSetting.sAoutAECSetting.bAECEnable == NVT_TRUE){
        DBG_DUMP("AEC LeakEstEnable\n");
        DBG_DUMP("Please enter your select:\n");
        DBG_DUMP("0) Disable\n");
        DBG_DUMP("1) Enable\n");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        switch(selectItem){
            case 0:
                sAoutSetting.sAoutAECSetting.bLeakEstEnable = NVT_FALSE;
                break;
            case 1:
            default:
                sAoutSetting.sAoutAECSetting.bLeakEstEnable = NVT_TRUE;
                break;
        }

        DBG_DUMP("AEC LeakEstValue\n");
        DBG_DUMP("Please enter your select(25~99): ");
		if ((ret = scanf("%ld", &sAoutSetting.sAoutAECSetting.iLeakEstValue)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("AEC NoiseCancelLevel\n");
        DBG_DUMP("Please enter your select(-3~-40): ");
		if ((ret = scanf("%ld", &sAoutSetting.sAoutAECSetting.iNoiseCancelLevel)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("AEC EchoCancelLevel\n");
        DBG_DUMP("Please enter your select(-30~-60): ");
		if ((ret = scanf("%ld", &sAoutSetting.sAoutAECSetting.iEchoCancelLevel)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("AEC FilterLen\n");
        DBG_DUMP("Please enter your select( Set to 0 is using default value): ");
		if ((ret = scanf("%lu", &sAoutSetting.sAoutAECSetting.uiFilterLen)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("AEC FrameSize\n");
        DBG_DUMP("Please enter your select( Set to 0 is using default value): ");
		if ((ret = scanf("%lu", &sAoutSetting.sAoutAECSetting.uiFrameSize)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

        DBG_DUMP("AEC NotchRadius\n");
        DBG_DUMP("Please enter your select( 0~1000): ");
		if ((ret = scanf("%lu", &sAoutSetting.sAoutAECSetting.uiNotchRadius)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}

    }


    if ((ret = NvtCodec_SetAoutSetting(aoutID, &sAoutSetting)) != ER_SUCCESS)
        DBG_ERR("Set aout %d setting fail! (ret=%d)\n", aoutID, ret);

    DBG_DUMP("Input Audio Test File: \n");
    DBG_DUMP("0) No Test File\n");
    DBG_DUMP("1) WAV Test File\n");
    DBG_DUMP("2) PCM RAW Test File\n");
    DBG_DUMP("Please enter your select: ");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    if (selectItem == 0)
        return;

    switch (selectItem)
    {
        case 1:
            isWAVFormat = 1;
            DBG_DUMP("Please enter file path: ");
		if ((ret = scanf("%127s", testFilePath)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
            break;
        case 2:
            isWAVFormat = 0;
            DBG_DUMP("Please enter file path: ");
		if ((ret = scanf("%127s", testFilePath)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
            break;
        //case 0:
        default:
            break;
    }

    // Get current audio input setting
    if ((ret = NvtCodec_GetAinSetting(aoutID, &sAinSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get ain %d setting fail! (ret=%d)\n", aoutID, ret);
        sampleRate = 32000;
    }
    else
    {
        switch (sAinSetting.eAinSampleRate)
        {
            case CODEC_AIO_SAMPLE_RATE_8000:
                sampleRate = 8000;
                break;
            case CODEC_AIO_SAMPLE_RATE_11025:
                sampleRate = 11025;
                break;
            case CODEC_AIO_SAMPLE_RATE_12000:
                sampleRate = 12000;
                break;
            case CODEC_AIO_SAMPLE_RATE_16000:
                sampleRate = 16000;
                break;
            case CODEC_AIO_SAMPLE_RATE_22050:
                sampleRate = 22050;
                break;
            case CODEC_AIO_SAMPLE_RATE_24000:
                sampleRate = 24000;
                break;
            case CODEC_AIO_SAMPLE_RATE_44100:
                sampleRate = 44100;
                break;
            case CODEC_AIO_SAMPLE_RATE_48000:
                sampleRate = 48000;
                break;
            case CODEC_AIO_SAMPLE_RATE_32000:
            default:
                sampleRate = 32000;
                break;
        }
    }

    // Test audio output
    if ((fd = open(testFilePath, O_RDONLY)) >= 0)
    {
        UINT8 audioBuffer[1600];
        int audioDataSize=0;
        CodecAoutStream sAoutStream;
        UINT32 sendSize;
		off_t ret_offset;

        // Calculate audio data play speed
        sleepUs = (((unsigned long)sizeof(audioBuffer) * 1000000) / (sampleRate * 2)) - 50;
        if (sAoutSetting.eAoutSoundMode == CODEC_AIO_SOUND_MODE_MONO ||
            sAoutSetting.eAoutSoundMode == CODEC_AIO_SOUND_MODE_STEREO)
            sleepUs = sleepUs >> 1;
        DBG_DUMP("sleep us = %lu\n", sleepUs);

        if (isWAVFormat)
		{
        	ret_offset = lseek(fd, 44, SEEK_SET);
			if(ret_offset < 0)
			{
				DBG_ERR("lseek fail \r\n");
				goto TEST_AOUT_END;
			}
		}
        // Start audio output
        if ((ret = NvtCodec_StartAout(aoutID)) != ER_SUCCESS)
        {
            DBG_ERR("Aout %d start fail! (ret=%d)\n", aoutID, ret);
            goto TEST_AOUT_END;
        }

        // Send audio stream
        while ((audioDataSize = read(fd, audioBuffer, sizeof(audioBuffer))) > 0)
        {
            sAoutStream.uiDataLength = (UINT32)audioDataSize;
            sAoutStream.puiStreamPayloadData = audioBuffer;
            sendSize = 0;

            while (sAoutStream.uiDataLength > 0)
            {
                if ((ret = NvtCodec_SendAoutStream(aoutID, &sAoutStream, &sendSize)) != ER_SUCCESS)
                {
                    DBG_ERR("Aout %d send stream fail! (ret=%d)\n", aoutID, ret);
                    break;
                }

                // Audio buffer is full
                if (sAoutStream.uiDataLength != sendSize)
                {
                    if (sendSize == 0)
                    {
                        usleep(sleepUs*2);
                    }
                    else
                    {
                        sAoutStream.uiDataLength -= sendSize;
                        sAoutStream.puiStreamPayloadData = audioBuffer + sendSize;
                        usleep((sleepUs*3)/2);
                    }
                }
                else
                {
                    usleep(sleepUs);
                    break;
                }
            }

            if (ret != ER_SUCCESS)
                break;
        }

        if (audioDataSize < 0)
            DBG_ERR("read %s fail, error is %s\n", testFilePath, strerror(errno));

        // Stop audio output
        if ((ret = NvtCodec_StopAout(aoutID)) != ER_SUCCESS)
            DBG_ERR("Aout %d stop fail! (ret=%d)\n", aoutID, ret);

TEST_AOUT_END:
        close(fd);
    }
    else
        DBG_ERR("open %s fail, error is %s\n", testFilePath, strerror(errno));
}
static void test_fd(void)
{


    UINT32 i,uiFDIdx = 0, testloop = 30;
    CodecFDSetting  fdSetting={0};
    char *fdData = NULL;//malloc(10240);
    CodecFDData *fdOut;
    unsigned int selectItem;
    NVTRET ret;
    // Get fd setting
    NvtCodec_GetFDSetting(uiFDIdx, &fdSetting);
    DBG_DUMP("Current face detection setting:\n");
    DBG_DUMP("Max Face Number    = %lu\n", fdSetting.uiMaxFaceNum);
    DBG_DUMP("Face Scale         = %lu\n", fdSetting.uiFaceScale);
    DBG_DUMP("Process Rate       = %lu\n", fdSetting.uiProcessRate);
    DBG_DUMP("\n");
	
	fdData = malloc(10240);
	if(fdData == NULL)
	{
		printf("malloc fdData is null\r\n");
		exit(1);
	}

    // Set fd setting
    DBG_DUMP("Use current setting? (Yes:1/No:0):");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    if (selectItem)
        goto skipSetting;

    DBG_DUMP("New Max Face Number(1~36):");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    fdSetting.uiMaxFaceNum = selectItem;

    DBG_DUMP("New Face Scale(4~48):");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    fdSetting.uiFaceScale = selectItem;

    DBG_DUMP("New Process Rate(>1):");
	if ((ret = scanf("%u", &selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}
    fdSetting.uiProcessRate = selectItem;

    DBG_DUMP("New face detection setting:\n");
    DBG_DUMP("Max Face Number = %lu\n", fdSetting.uiMaxFaceNum);
    DBG_DUMP("Face Scale      = %lu\n", fdSetting.uiFaceScale);
    DBG_DUMP("Process Rate    = %lu\n", fdSetting.uiProcessRate);

    if (NvtCodec_SetFDSetting(uiFDIdx, &fdSetting) != ER_SUCCESS)
    {
        DBG_ERR("Face detection parameters are invalid\n");
        return;
    }


skipSetting:

    // Start FD
    NvtCodec_StartFD(uiFDIdx);
    DBG_DUMP("Face detection start\n");

    // Get FD result
    while (testloop--)
    {
        NvtCodec_GetFDData(fdData, 3000);
        fdOut = (CodecFDData *)fdData;
        DBG_DUMP("uiFaceNum  =%lu\n",fdOut->uiFaceNumber);

        for (i=0;i<fdOut->uiFaceNumber;i++)
        {
            DBG_DUMP("face %02d ",i);
            DBG_DUMP("x=%04d, y=%04d, w=%04d, h=%04d\n", fdOut->info[i].x, fdOut->info[i].y, fdOut->info[i].w, fdOut->info[i].h);
            fdOut++;
        }
    }
    // Stop FD
    NvtCodec_ExitFd();
    NvtCodec_StopFD(uiFDIdx);
	free(fdData);
    DBG_DUMP("Face detection test terminated!\n");

}

static void test_td(void)
{
#if 1
    CodecTDSetting tdSetting={0};
    // Get TD setting
    NvtCodec_GetTDSetting(&tdSetting);
    DBG_DUMP("Current tampering detection setting:\n");
    DBG_DUMP("TD Percetage    = %lu\n", tdSetting.uiPCT);
    DBG_DUMP("FG FrameNum     = %lu\n", tdSetting.uiFrameNum);

    // Set TD setting

    tdSetting.uiPCT = 70;
    tdSetting.uiFrameNum = 3;

    NvtCodec_SetTDSetting(&tdSetting);

    NvtCodec_GetTDSetting(&tdSetting);
    DBG_DUMP("Test New tampering detection setting:\n");
    DBG_DUMP("TD Percetage    = %lu\n", tdSetting.uiPCT);
    DBG_DUMP("FG FrameNum     = %lu\n", tdSetting.uiFrameNum);



    // Start TD
    NvtCodec_TDStart(1);
    int timeout = 40;
    CodecTDStatus status={0};
    DBG_DUMP("start test!!\r\n");
    while(timeout > 0){


        NvtCodec_GetTDStatus(&status);

        printf("result: trigger:%d frame:%d percent:%d\r\n",status.bStatus,status.uiFrameNum_Cur, status.uiPCT_Cur);
        usleep(250000);
        timeout --;
    }



    // stop TD
    DBG_DUMP("stop test!!\r\n");
    NvtCodec_TDStart(0);
    DBG_DUMP("Tampering detection test terminated!\n");
#endif
}
#if 0
static void test_ftg_remove_snapshot(void)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    UINT32 id;
    DBG_DUMP("please enter id you want remove:\r\n");
    scanf(" %lu",&id);
    NvtCodec_FTGSnapshotRemove(id);
#endif
}
static void test_ftg_get_snapshot(void)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    UINT32 id;
    UINT32 output_addr;
    DBG_DUMP("please enter id you want get:\r\n");
    scanf(" %lu",&id);
    int ret=0;
    ret = NvtCodec_FTGSnapshotGet(id, &output_addr);
    if(ret < 0){
        DBG_DUMP("get snapshot error!\r\n");
        return ;
    }
    DBG_DUMP("get output_addr=%x\r\n",output_addr);
    CodecFTG_SNAPSHOT *ftg_snapshot= (CodecFTG_SNAPSHOT *)output_addr;
    DBG_DUMP("get ftg snapshot=======\r\n");
    DBG_DUMP("ftg_snapshot.id=%d\r\n",ftg_snapshot->id);
    DBG_DUMP("ftg_snapshot.pic_num=%d\r\n",ftg_snapshot->pic_num);
    UINT32 i;
    for(i=0; i < ftg_snapshot->pic_num; i++){
        DBG_DUMP("pic [%d]=======\r\n",i);
	DBG_DUMP("socre =%d\r\n",ftg_snapshot->pic_list[i].score);
        DBG_DUMP("overrall =%x %x %x %x %x %x\r\n",
	ftg_snapshot->pic_list[i].overrall_pic[0],
        ftg_snapshot->pic_list[i].overrall_pic[1],
        ftg_snapshot->pic_list[i].overrall_pic[2],
        ftg_snapshot->pic_list[i].overrall_pic[3],
        ftg_snapshot->pic_list[i].overrall_pic[4],
        ftg_snapshot->pic_list[i].overrall_pic[5]
        );
	DBG_DUMP("overrall size=%d\r\n",ftg_snapshot->pic_list[i].overrall_size);
        DBG_DUMP("face =%x %x %x %x %x %x\r\n",
	ftg_snapshot->pic_list[i].face_pic[0],
        ftg_snapshot->pic_list[i].face_pic[1],
        ftg_snapshot->pic_list[i].face_pic[2],
        ftg_snapshot->pic_list[i].face_pic[3],
        ftg_snapshot->pic_list[i].face_pic[4],
        ftg_snapshot->pic_list[i].face_pic[5]
        );

	DBG_DUMP("face size=%d\r\n",ftg_snapshot->pic_list[i].face_size);

        ////save pic
        FILE *fp= NULL;
        char path[128]={0};
        sprintf(path,"/tmp/overrall_%lu_%lu.jpg",ftg_snapshot->id,i);
        fp = fopen(path,"w");
        fwrite(ftg_snapshot->pic_list[i].overrall_pic,1,ftg_snapshot->pic_list[i].overrall_size,fp);
        fclose(fp);
        sprintf(path,"/tmp/face_%lu_%lu.jpg",ftg_snapshot->id,i);
        fp = fopen(path,"w");
        fwrite(ftg_snapshot->pic_list[i].face_pic,1,ftg_snapshot->pic_list[i].face_size,fp);
        fclose(fp);
    }
#endif
}

static void test_ftg(void)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    UINT32 i,uiFTGIdx = 0, testloop = 20;
    UINT32 draw_rate=0, allinfo=0, snapshot_flag=0;
    CodecFTGSetting  ftgSetting={0};
    CodecFTGData     ftgData={0};
    CodecFTGObjData *pFace;
    CodecIAObjData   targetCoord={0,0,1920,1080};
	BOOL             bUpdated;

    // Get FTG setting
    NvtCodec_GetFTGSetting(uiFTGIdx, &ftgSetting);
    DBG_DUMP("Current face tracking setting:\n");
    DBG_DUMP("Max Face Number = %lu\n", ftgSetting.uiMaxFaceNum);
    DBG_DUMP("Face Scale      = %lu\n", ftgSetting.uiFaceScale);
    DBG_DUMP("Process Rate    = %lu\n", ftgSetting.uiProcessRate);
    DBG_DUMP("Min Face Score  = %lu\n", ftgSetting.uiMinFaceScore);
    DBG_DUMP("Draw Rate       = %lu\n", ftgSetting.uiDrawRate);
    DBG_DUMP("all info        = %lu\n", ftgSetting.uiAllInfo);
    DBG_DUMP("\n");

    // Set FTG setting
    ftgSetting.uiMaxFaceNum = 20;
    ftgSetting.uiFaceScale = 15;
    ftgSetting.uiProcessRate = 3;
    ftgSetting.uiMinFaceScore = 33554432;/////recommand value

    DBG_DUMP("set draw rectangle rate  (1~30):\n");
    scanf(" %lu",&draw_rate);
    DBG_DUMP("set all info or only location info (0: only location, 1: loc+ clarity 2:all info):\n");
    scanf(" %lu",&allinfo);
    DBG_DUMP("snapshot test (0: disable, 1: enable)\r\n");
    scanf(" %lu",&snapshot_flag);
    DBG_DUMP("sensitivity (1: high hit, high false hit 2: low hit, low false hit 3: include glass, hat )\n");
    scanf(" %lu",&ftgSetting.uiSensitivity);


    DBG_DUMP("test times(1~60)\r\n");
    scanf(" %lu",&testloop);
    ftgSetting.uiDrawRate = draw_rate;
    ftgSetting.uiAllInfo = allinfo;


    NvtCodec_SetFTGSetting(uiFTGIdx, &ftgSetting);

    DBG_DUMP("New face tracking setting:\n");
    DBG_DUMP("Max Face Number = %lu\n", ftgSetting.uiMaxFaceNum);
    DBG_DUMP("Face Scale      = %lu\n", ftgSetting.uiFaceScale);
    DBG_DUMP("Process Rate    = %lu\n", ftgSetting.uiProcessRate);
    DBG_DUMP("Min Face Score  = %lu\n", ftgSetting.uiMinFaceScore);
    DBG_DUMP("Draw Rate       = %lu\n", ftgSetting.uiDrawRate);
    DBG_DUMP("all info        = %lu\n", ftgSetting.uiAllInfo);
    DBG_DUMP("sensitivity     = %lu\n", ftgSetting.uiSensitivity);
    // Start FTG
    NvtCodec_StartFTG(uiFTGIdx);

    UINT32 fake_score=0;
    // Get FTG result
    while (testloop--)
    {
        sleep(1);
        memset(&ftgData, 0, sizeof(ftgData));
        NvtCodec_GetFTGData(uiFTGIdx, &ftgData,&targetCoord, &bUpdated);
        pFace = ftgData.psFTGObjData;
        DBG_DUMP("uiFaceNum  =%lu, bUpdated =%lu\n",ftgData.uiFaceNum, bUpdated);
        for (i=0;i<ftgData.uiFaceNum;i++)
        {
            DBG_DUMP("face %d , x=%d, y=%d, w=%d, h=%d, id=%d\n"
                     "type=%d , clarity=%d luma=%d\n"
                     "angle_hori=%d  angle_ver=%d  angle_roll=%d  score=%d\n",
            i,pFace->uiCoordinateX,pFace->uiCoordinateY,pFace->uiWidth,pFace->uiHeight,pFace->id,
            pFace->type,
            pFace->clarity,
            pFace->luma,
            pFace->angle_horizontal,
            pFace->angle_vertical,
            pFace->angle_roll,
            pFace->score
            );
            if(snapshot_flag == 1){
                NvtCodec_FTGSnapshotNotify(pFace->id,fake_score);
            }
            pFace++;
            fake_score ++;

        }
        NvtCodec_ReleaseFTGData(uiFTGIdx, &ftgData);
    }
    // Stop FTG
    NvtCodec_StopFTG(uiFTGIdx);

    DBG_DUMP("Face tracking test terminated!\n");
#endif
}
#endif
static void test_vout(UINT32 voutID)
{
    CodecVoutSetting sVoutSetting={0};
    int selectItem;
    NVTRET ret;

    DBG_IND("Test voutID channel = %d\n", voutID);

    // Get current video encoder setting
    if ((ret = NvtCodec_GetVoutSetting(voutID, &sVoutSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get vout %d setting fail! (ret=%d)\n", voutID, ret);
        return;
    }

    DBG_DUMP("Output Interface    = ");
    switch (sVoutSetting.eVoutInterface)
    {
        case CODEC_VOUT_INTERFACE_CVBS_NTSC:
            DBG_DUMP("CVBS(NTSC)\n");
            break;
        case CODEC_VOUT_INTERFACE_CVBS_PAL:
            DBG_DUMP("CVBS(PAL)\n");
            break;
        case CODEC_VOUT_INTERFACE_LCD:
            DBG_DUMP("LCD\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_1280x720P30:
            DBG_DUMP("HDMI(1280x720P30)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_1920x1080P30:
            DBG_DUMP("HDMI(1920x1080P30)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_1920x1080I60:
            DBG_DUMP("HDMI(1920x1080I60)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_3840x2160P30:
            DBG_DUMP("HDMI(3840x2160P30)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_3840x2160P25:
            DBG_DUMP("HDMI(3840x2160P25)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_3840x2160P24:
            DBG_DUMP("HDMI(3840x2160P24)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_4096x2160P30:
            DBG_DUMP("HDMI(4096x2160P30)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_4096x2160P25:
            DBG_DUMP("HDMI(4096x2160P25)\n");
            break;
        case CODEC_VOUT_INTERFACE_HDMI_4096x2160P24:
            DBG_DUMP("HDMI(4096x2160P24)\n");
            break;
        case CODEC_VOUT_INTERFACE_AUTO:
            DBG_DUMP("AUTO MODE\n");
            break;
        default:
            DBG_DUMP("Unknown Interface %u\n", sVoutSetting.eVoutInterface);
            break;
    }
    DBG_DUMP("Display Path (-1: no display path) =%d\r\n",sVoutSetting.iChannel);
    DBG_DUMP("================\n");

    DBG_DUMP("\r\n");
    DBG_DUMP("Please Enter Option\r\n");
    DBG_DUMP("0) Disable/Enable Option\r\n");
    DBG_DUMP("1) DisPlay Setting\n");
	if ((ret = scanf("%d",&selectItem)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    if(selectItem == 0){
        DBG_DUMP("Display Enable Option\r\n");
        DBG_DUMP("0) Disable\r\n");
        DBG_DUMP("1) Enable\r\n");
		if ((ret = scanf("%d",&selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        NvtCodec_SetVoutEnable(selectItem);

    }
    else if(selectItem == 1){

        DBG_DUMP("Display Path Option\n");
        DBG_DUMP("Value(-1 ~15)  -1: no display path\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%d", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        sVoutSetting.iChannel = selectItem;

        DBG_DUMP("Video Output Interface Option\n");
        DBG_DUMP("0) CVBS(NTSC)\n");
        DBG_DUMP("1) CVBS(PAL)\n");
        DBG_DUMP("2) HDMI(1280x720P30)\n");
        DBG_DUMP("3) HDMI(1920x1080P30)\n");
        DBG_DUMP("4) HDMI(1920x1080I60)\n");
        DBG_DUMP("5) HDMI(3840x2160P30)\n");
        DBG_DUMP("6) HDMI(3840x2160P25)\n");
        DBG_DUMP("7) HDMI(3840x2160P24)\n");
        DBG_DUMP("8) HDMI(4096x2160P30)\n");
        DBG_DUMP("9) HDMI(4096x2160P25)\n");
        DBG_DUMP("10) HDMI(4096x2160P24)\n");
        DBG_DUMP("11) HDMI(AUTO)\n");
        DBG_DUMP("12) LCD\n");
        DBG_DUMP("Please enter your select: ");
		if ((ret = scanf("%u", &selectItem)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        switch (selectItem)
        {
            case 0:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_NTSC;
                break;
            case 1:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_PAL;
                break;
            case 2:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_1280x720P30;
                break;
            case 3:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_1920x1080P30;
                break;
            case 4:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_1920x1080I60;
                break;
            case 5:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_3840x2160P30;
                break;
            case 6:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_3840x2160P25;
                break;
            case 7:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_3840x2160P24;
                break;
            case 8:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_4096x2160P30;
                break;
            case 9:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_4096x2160P25;
                break;
            case 10:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_4096x2160P24;
                break;
            case 11:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_AUTO;
                break;
            case 12:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_LCD;
                break;
            default:
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_PAL;
                break;
        }

        if ((ret = NvtCodec_SetVoutSetting(voutID, &sVoutSetting)) != ER_SUCCESS)
             DBG_ERR("Set vout %d setting fail! (ret=%d)\n", voutID, ret);

    }
    else{
        DBG_ERR("error option (%d)\r\n",selectItem);
    }





}

#if 0
INT32 motion_CB(INT8 motion_status)
{
    DBG_DUMP("motion status == %d\n",motion_status);
    return 0;
}

static void test_motion(void)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    NvtCodec_StartMD(0);
  //  printf("enter test time (sec): 0~60\r\n");
   // int test_time=0;
 //   scanf(" %d",&test_time);
    NvtCodec_RegisterNotifyMotion(&motion_CB);
    DBG_DUMP("please trigger motion!!!\n");
    sleep(20);
    DBG_DUMP("test motion finish!!!!\n");
    NvtCodec_UnregisterNotifyMotion();
    NvtCodec_StopMD(0);
#endif
}

static void test_motion_cfg_get(void)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    CHAR level[16]={0};
    CHAR wnd[256]={0};


    NvtCOdec_GetMotionCfg(0,level,wnd);

    printf("===========\n");
    printf("level=%s\n",level);
    printf("wnd=%s\n",wnd);
    INT32 i=0,j=0;
    INT32 md_wnd[12][16]={0};
    printf("motion windows\n");
    CHAR tmp_wnd[256]={0};
    strcpy(tmp_wnd, wnd);
    INT32 wnd_index=0;
    INT32 wnd_index2=0;
    CHAR *token=NULL;
    for(i=0; i< 12 ; i++){
        if(i==0){
            token = strtok(tmp_wnd, ",");
        }
        else{
            token = strtok(NULL, ",");
        }

        wnd_index = atoi(token);
        for(j=0 ; j<16 ; j++){
            wnd_index2 = (1<<j);

            if( (wnd_index2& wnd_index)==wnd_index2){
                md_wnd[i][j]=1;
            }
            printf("%d ",md_wnd[i][j]);
        }
        printf("\n");
    }
#endif
}

static void test_motion_cfg_set(void)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    CHAR level[16]={0};
    CHAR wnd[256]={0};
    printf("please set level(High,9,8,7,6,5,4,3,2,1,Low):\n");
    scanf(" %s",level);
    printf("plesae set motion window grids{\n"
           "windows is 12x16 as follow\n"
             "[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             " 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ]\n"
           "now we using 12 values to set 12 rows, value = 0~65535\n"
             "if value = 65535, the row is \n"
             "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1\n"
             "if value = 0, the row is\n"
             "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             "if value = 1 the row is \n"
             "1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             "if value = 2,the row is\n"
             "0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
             "if value  = 5,the row is \n"
             "1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
              "......\n"
           "please set 12 rows:\n"
             "ex:\n"
             "65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535\n"
             "this means all grid is set to 1\n"
             "ex: \n"
             "0,0,0,0,0,0,0,0,0,0,0,0\n"
             "this means all grid is set to 0}\n");
    scanf(" %s",wnd);

    printf("========\n");
    printf("level=%s\n",level);
    printf("wnd=%s\n",wnd);

    NvtCodec_SetMotionCfg(0, level, wnd);
#endif
}
#endif
#if 0
static int init_ftype(FT_Library stLibrary, char *pcFontFile, FT_Face *pstFace, int *font_size, FT_Matrix *pstMatrix, double dAngle)
{
	FT_Error	error;
	int n;

	if(!pcFontFile || !pstMatrix){
		printf("freetyped : invalid ftype parameter\n");
		return -1;
	}

	error = FT_Init_FreeType( &stLibrary );              /* initialize library */

	for (n = 0 ; n < MEDIA_PROFILE_MAX_NUM ; ++n)
	{
		error = FT_New_Face( stLibrary, pcFontFile, 0, &pstFace[n] ); /* create face object */
	}

	/* use 50pt at 100dpi */
	for(n = 0 ; n < MEDIA_PROFILE_MAX_NUM ; ++n)
	{
		error = FT_Set_Char_Size( pstFace[n], font_size[n] * 64, 0, 100, 0 );                /* set character size */
	}

	/* set up matrix */
	pstMatrix->xx = (FT_Fixed)( cos( dAngle ) * 0x10000L );
	pstMatrix->xy = (FT_Fixed)(-sin( dAngle ) * 0x10000L );
	pstMatrix->yx = (FT_Fixed)( sin( dAngle ) * 0x10000L );
	pstMatrix->yy = (FT_Fixed)( cos( dAngle ) * 0x10000L );

    printf("error=%d\n", error);
	return 0;
}

/* Replace this function with something useful. */
static void draw_bitmap( FT_Bitmap*  bitmap, FT_Int x, FT_Int y)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for ( i = x, p = 0; i < x_max; i++, p++ )
	{
		for ( j = y, q = 0; j < y_max; j++, q++ )
		{
			if ( i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT )
				continue;

			if(bitmap->buffer[q * bitmap->width + p]){
				image[j][i] = 0xFF;
				alpha[j][i] = 255;
			}
			else{
				image[j][i] = 0x00;
				alpha[j][i] = 0x00;
			}
		}
	}
}

static int test_freetype_init (char* pcFontFile, int iEnable)
{
	int iRet = -1;
	int iLen = 0, iIdx = 0, iProfileIdx = 0;
	int old_font_size[MEDIA_PROFILE_MAX_NUM] = { 0 };
	FT_Library	stLibrary = { 0 };
	FT_Face		face[MEDIA_PROFILE_MAX_NUM];
	FT_Matrix 	matrix;
	FT_Vector	pen;
	FT_Error         enError;
	FT_GlyphSlot	slot;

	int			iTarget_height = HEIGHT;//, skip_width  = 0;
	double		angle = 0;
	char* pcOSDString =  "DebugOSD";

	for (iProfileIdx=0; iProfileIdx<MEDIA_PROFILE_MAX_NUM; iProfileIdx++)
	{
		old_font_size[iProfileIdx] = 25;
	}

	if (!iEnable)
	{
		printf("=============Disable ===============\n");
		for (iProfileIdx=0; iProfileIdx<MEDIA_PROFILE_MAX_NUM; iProfileIdx++)
		{
			test_osd_cfg_set (NULL, iProfileIdx, WIDTH, HEIGHT, iEnable);
		}
		iRet = 0;
		NvtCodec_ClearAddedOSDImg();
		NvtCodec_ApplyAddedOSDImg();
		goto END_INIT;
	}

	if ( init_ftype(stLibrary, pcFontFile, face, old_font_size, &matrix, angle))
	{
		goto END_INIT;
	}

	for (iProfileIdx=0; iProfileIdx<MEDIA_PROFILE_MAX_NUM; iProfileIdx++)
	{
		printf("drawing %dth stream\n", iProfileIdx);
		slot = face[iProfileIdx]->glyph;
		iLen = strlen(pcOSDString);

		memset(image, 0, sizeof(image));
		memset(alpha, 0, sizeof(alpha));

		pen.x = 0;
		pen.y = ( iTarget_height - (HEIGHT) + 10) * 64;

		for ( iIdx = 0; iIdx < iLen; iIdx++ )
		{
			FT_Set_Transform( face[iProfileIdx], &matrix, &pen );

			enError = FT_Load_Char( face[iProfileIdx], pcOSDString[iIdx], FT_LOAD_RENDER );
			if ( enError ){
				printf("FT_Load_Char(%c) fail with %d\n", pcOSDString[iIdx], enError);
				continue;
			}

			draw_bitmap( &slot->bitmap, slot->bitmap_left, iTarget_height - slot->bitmap_top );

			/* increment pen position */
			pen.x += slot->advance.x;
			pen.y += slot->advance.y;
		}

		printf("test_osd_cfg_set(%d)\n", iProfileIdx);
		test_osd_cfg_set (pcFontFile, iProfileIdx, WIDTH, HEIGHT, iEnable);
		printf("test_osd_cfg_set(%d) done\n", iProfileIdx);
	}
	iRet = 0;

	//printf("=============Apply cause ? ===============\n");
	//sleep(2);
	NvtCodec_ApplyAddedOSDImg();
END_INIT:
	return iRet;
}

static void test_osd_cfg_set (char* pcFontFile, int iChannel, int iWidth, int iHeight, int iEnable)
{
	CodecOSDIMAGE ipc_image;

	//draw two bitmap images
	//1st bitmap
	ipc_image.channel = iChannel;//draw to streaming 0
	ipc_image.id  = iChannel;
	ipc_image.bmp_ptr = (unsigned long)image;
	ipc_image.alpha_ptr = (unsigned long)alpha;
	ipc_image.on = iEnable;
	ipc_image.x = 0;//draw to upper-left corner
	ipc_image.y = 0;
	ipc_image.width = iWidth;
	ipc_image.height = iHeight;

	if (NvtCodec_UpdateImage(&ipc_image))
	{
		NvtCodec_AddImage(&ipc_image);
	}
}
#endif
#if 0
static void test_osd_display (char* pcFontFile, int iEnable)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
	if (NvtCodec_StartOsd())
	{
		goto END_SET;
	}

	test_freetype_init (pcFontFile, iEnable);

	NvtCodec_StopOsd();

END_SET:
	return ;
#endif
}

static void test_osd_cfg_mask (int iEnableMask)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
	int iRet;
	int iProfileIdx = 0;

	printf("test_osd_cfg_mask(%d)\n", iEnableMask);
	//if (NvtCodec_InitOsd())
	if (NvtCodec_StartOsd())
	{
		printf("NvtCodec_InitOsd() fail\n");
		goto END_SET;
	}
	printf("NvtCodec_StartOsd() OK\n");

	for (iProfileIdx=0; iProfileIdx<MEDIA_PROFILE_MAX_NUM; iProfileIdx++)
	{
		if (NvtCodec_UpdateMask (iProfileIdx<<1, iProfileIdx, iEnableMask, 0xff00, 2500, 2500, 1000, 1000))
		{
			iRet = NvtCodec_AddMask (iProfileIdx << 1, iProfileIdx, iEnableMask, 0xff00, 2500, 2500, 1000, 1000);
			printf("NvtCodec_AddMask() return %d\n", iRet);
		}
	}
	iRet = NvtCodec_ApplyAddedOSDImg();
	printf("NvtCodec_ApplyAddedOSDImg() return %d\n", iRet);

	NvtCodec_StopOsd();

END_SET:
	return ;
#endif
}
#endif
#if 0
static void test_blending(int testChannel)
{
	DBG_WRN("This function is not ready!\n");
	return;
#if 0
    CodecBlendSetting sBlendSetting={0};

    printf("blending test===\r\n");
    NVTRET ret=0;
    ret = NvtCodec_GetBlendSetting(testChannel, &sBlendSetting);
    if(ret != ER_SUCCESS){
        printf("get blend setting error!!(%d)\r\n",ret);
        return;
    }
    printf("  Enable		  : %d\r\n", sBlendSetting.bEnable);
    printf("  Alpha 		  : %d\r\n", sBlendSetting.uiAlpha);
    printf("  Source video ch	  : %d\r\n", sBlendSetting.uiSrcVideoID);
    printf("  destination video ch: %d\r\n", sBlendSetting.uiDesVideoID);


    sBlendSetting.uiDesVideoID = testChannel;
    printf("Please Set Enable (0: disable, 1: enable): ");
    scanf("%lu", &sBlendSetting.bEnable);
    printf("Please Set Alpha (0~255): ");
    scanf("%lu", &sBlendSetting.uiAlpha);
    printf("Please Set source Video channel: ");
    scanf("%lu",&sBlendSetting.uiSrcVideoID);

    ret = NvtCodec_SetBlendSetting ((UINT32) testChannel, &sBlendSetting);
    if(ret != ER_SUCCESS){

        printf("set blend setting error!!(%d)\r\n",ret);

    }
    return;
#endif
}
#endif
static void test_snapshot(int testChannel){

    CodecVencSnapshot sVencSnapshot = {0};
    FILE *pfSnap = NULL;
    unsigned int  frameCounter=0;
    char snapFilename[64]={0};
    int ret=0;
    UINT32 uiScaleW, uiScaleH;
    CodecVencSetting sVencSetting = {0};
    CodecVencSnapshotConfig SnapshotConfig = {0};
    while(frameCounter < 5){


    if ((ret = NvtCodec_GetVencSetting(testChannel, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d setting fail! (ret=%d)\n", testChannel, ret);
        return;
    }
        DBG_DUMP("source ID                = %d\n", sVencSetting.eVencInputSource);
        DBG_DUMP("current Resolution       = %dx%d\n", sVencSetting.uiTargetWidth, sVencSetting.uiTargetHeight);
        DBG_DUMP("Please enter Scaling Width: ");
		if ((ret = scanf("%u",&uiScaleW)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        DBG_DUMP("Please enter Scaling Height: ");
		if ((ret = scanf("%u",&uiScaleH)) < 0) {
			printf("scanf error(%d)\n", ret);
			exit(1);
		}
        SnapshotConfig.uiVencChannelID = testChannel;
        SnapshotConfig.uiWidth = uiScaleW;
        SnapshotConfig.uiHeight = uiScaleH;

        sprintf(snapFilename, SNAPSHOT_FILE, (UINT32)testChannel, frameCounter);
        if ((pfSnap = fopen(snapFilename, "wb")) == NULL){
            DBG_WRN("%s open fail, can't save encoded data\n", snapFilename);
        }
        else
        {
            if ((ret = NvtCodec_RequestVencJPEG(&SnapshotConfig, &sVencSnapshot)) != ER_SUCCESS){
                DBG_ERR("Get snapshot fail! (ret=%d)\n", testChannel, ret);
            }
            else
            {
                DBG_DUMP("snapshot addr=%x size=%d\r\n",sVencSnapshot.uiSnapshotData,sVencSnapshot.uiDatatLength);
                DBG_DUMP("Save sanpshot to %s ...\n", snapFilename);
                fwrite((char *)sVencSnapshot.uiSnapshotData, sVencSnapshot.uiDatatLength, 1, pfSnap);
            }

            fflush(pfSnap);
            fclose(pfSnap);
            NvtCodec_ReleaseVencJPEG();
        }
        frameCounter++;
    }
}

void test_record(void){

    int uiSelect=0;
	NVTRET ret;
    printf("record option:\n");
    printf(" 0) stop\n");
    printf(" 1) start\n");

	if ((ret = scanf("%d",&uiSelect)) < 0) {
		printf("scanf error(%d)\n", ret);
		exit(1);
	}

    if(uiSelect==0){

        system("killall nvtrecordManagerd");
    }
    else if(uiSelect==1){

        system("nvtrecordManagerd &");
    }

    return;
}
static int test_quick_set_cbr(int argc, char *argv[], CodecVencCBR* cbr_config){

    cbr_config->uiEnable = 1;

    if(argc>=12){
       cbr_config->uiStaticTime = atoi(argv[11]);
    }else{
        printf("argc:%d error, can not get cbr statictime\n",argc);
        return -1;
    }

    if(argc>=13){
       cbr_config->uiByteRate = atoi(argv[12]);
    }else{
        printf("argc:%d error, can not get byterate\n",argc);
        return -1;
    }

    if(argc>=14){
       cbr_config->uiFrameRate = atoi(argv[13]);
    }else{
        printf("argc:%d error, can not get uiFrameRate\n",argc);
        return -1;
    }


    if(argc>=15){
       cbr_config->uiGOP = atoi(argv[14]);
    }else{
        printf("argc:%d error, can not get uiGOP\n",argc);
        return -1;
    }

    if(argc>=16){
       cbr_config->uiInitIQp = atoi(argv[15]);
    }else{
        printf("argc:%d error, can not get uiInitIQp\n",argc);
        return -1;
    }

    if(argc>=17){
        cbr_config->uiMinIQp = atoi(argv[16]);
    }else{
        printf("argc:%d error, can not get uiMinIQp\n",argc);
        return -1;
    }

    if(argc>=18){
        cbr_config->uiMaxIQp = atoi(argv[17]);
    }else{
        printf("argc:%d error, can not get uiMaxIQp\n",argc);
        return -1;
    }

    if(argc>=19){
       cbr_config->uiInitPQp = atoi(argv[18]);
    }else{
        printf("argc:%d error, can not get uiInitPQp\n",argc);
        return -1;
    }

    if(argc>=20){
        cbr_config->uiMinPQp = atoi(argv[19]);
    }else{
        printf("argc:%d error, can not get uiMinPQp\n",argc);
        return -1;
    }

    if(argc>=21){
        cbr_config->uiMaxPQp = atoi(argv[20]);
    }else{
        printf("argc:%d error, can not get uiMaxPQp\n",argc);
        return -1;
    }

    if(argc>=22){
        cbr_config->iIPWeight = atoi(argv[21]);
    }else{
        printf("argc:%d error, can not get iIPWeight\n",argc);
        return -1;
    }

    if(argc>=23){
        cbr_config->uiRowRcEnalbe = atoi(argv[22]);
    }else{
        printf("argc:%d error, can not get uiRowRcEnalbe\n",argc);
        return -1;
    }

    if(argc>=24){
        cbr_config->uiRowRcQpRange = atoi(argv[23]);
    }else{
        printf("argc:%d error, can not get uiRowRcQpRange\n",argc);
        return -1;
    }


    if(argc>=25){
        cbr_config->uiRowRcQpStep = atoi(argv[24]);
    }else{
        printf("argc:%d error, can not get uiRowRcQpStep\n",argc);
        return -1;
    }

    return 0;
}

static int test_quick_set_vbr(int argc, char *argv[], CodecVencVBR* vbr_config){

    vbr_config->uiEnable = 1;

    if(argc>=12){
       vbr_config->uiStaticTime = atoi(argv[11]);
    }else{
        printf("argc:%d error, can not get statictime\n",argc);
        return -1;
    }

    if(argc>=13){
       vbr_config->uiByteRate = atoi(argv[12]);
    }else{
        printf("argc:%d error, can not get byterate\n",argc);
        return -1;
    }

    if(argc>=14){
       vbr_config->uiFrameRate = atoi(argv[13]);
    }else{
        printf("argc:%d error, can not get uiFrameRate\n",argc);
        return -1;
    }


    if(argc>=15){
       vbr_config->uiGOP = atoi(argv[14]);
    }else{
        printf("argc:%d error, can not get uiGOP\n",argc);
        return -1;
    }

    if(argc>=16){
       vbr_config->uiInitIQp = atoi(argv[15]);
    }else{
        printf("argc:%d error, can not get uiInitIQp\n",argc);
        return -1;
    }

    if(argc>=17){
        vbr_config->uiMinIQp = atoi(argv[16]);
    }else{
        printf("argc:%d error, can not get uiMinIQp\n",argc);
        return -1;
    }

    if(argc>=18){
        vbr_config->uiMaxIQp = atoi(argv[17]);
    }else{
        printf("argc:%d error, can not get uiMaxIQp\n",argc);
        return -1;
    }

    if(argc>=19){
       vbr_config->uiInitPQp = atoi(argv[18]);
    }else{
        printf("argc:%d error, can not get uiInitPQp\n",argc);
        return -1;
    }

    if(argc>=20){
        vbr_config->uiMinPQp = atoi(argv[19]);
    }else{
        printf("argc:%d error, can not get uiMinPQp\n",argc);
        return -1;
    }

    if(argc>=21){
        vbr_config->uiMaxPQp = atoi(argv[20]);
    }else{
        printf("argc:%d error, can not get uiMaxPQp\n",argc);
        return -1;
    }

    if(argc>=22){
        vbr_config->iIPWeight = atoi(argv[21]);
    }else{
        printf("argc:%d error, can not get iIPWeight\n",argc);
        return -1;
    }

    if(argc>=23){
        vbr_config->uiChangePos = atoi(argv[22]);
    }else{
        printf("argc:%d error, can not get uiChangePos\n",argc);
        return -1;
    }

    if(argc>=24){
        vbr_config->uiRowRcEnalbe = atoi(argv[23]);
    }else{
        printf("argc:%d error, can not get uiRowRcEnalbe\n",argc);
        return -1;
    }


    if(argc>=25){
        vbr_config->uiRowRcQpRange = atoi(argv[24]);
    }else{
        printf("argc:%d error, can not get uiRowRcQpRange\n",argc);
        return -1;
    }


    if(argc>=26){
        vbr_config->uiRowRcQpStep = atoi(argv[25]);
    }else{
        printf("argc:%d error, can not get uiRowRcQpStep\n",argc);
        return -1;
    }
    return 0;
}

static int test_quick_set_NoneBRC(int argc, char *argv[], CodecVencBRCNoneInfo* NoneBRC_config){


    if(argc>=12){
        NoneBRC_config->uiFrameRate = atoi(argv[11]);
    }
    else{
        printf("argc:%d error, can not get frame rate\n",argc);
        return -1;
    }

    if(argc>=13){
        NoneBRC_config->uiGOP = atoi(argv[12]);
    }
    else{
        printf("argc:%d error, can not get GOP\n",argc);
        return -1;
    }

    if(argc>=14){
        NoneBRC_config->uiByteRate = atoi(argv[13]);
    }
    else{
        printf("argc:%d error, can not get Byte rate\n",argc);
        return -1;
    }
    return 0;
}

static int test_quick_set_fixedQP(int argc, char *argv[], CodecVencFIXQP* fix_config){

    fix_config->uiEnable = 1;

    if(argc>=12){
       fix_config->uiByteRate = atoi(argv[11]);
    }else{
        printf("argc:%d error, can not get uiByteRate\n",argc);
        return -1;
    }

    if(argc>=13){
       fix_config->uiFrameRate = atoi(argv[12]);
    }else{
        printf("argc:%d error, can not get uiFrameRate\n",argc);
        return -1;
    }

    if(argc>=14){
       fix_config->uiGOP = atoi(argv[13]);
    }else{
        printf("argc:%d error, can not get uiGOP\n",argc);
        return -1;
    }


    if(argc>=15){
       fix_config->uiIFixQP = atoi(argv[14]);
    }else{
        printf("argc:%d error, can not get uiIFixQP\n",argc);
        return -1;
    }

    if(argc>=16){
        fix_config->uiPFixQP = atoi(argv[15]);
    }else{
        printf("argc:%d error, can not get uiPFixQP\n",argc);
        return -1;
    }

    return 0;
}

void test_quick_usage(void){

    printf("CBR set====\r\n");
    printf("./nvtcodec_test %d [channel] [width] [height] [codec] [rotate] "
           "[codec_3dnr] [bitrate control type-->1] [LTR enable] [LTR Interval] [StaticTime] [ByteRate] "
           "[FrameRate] [GOP] [InitIQP] [MinIQp] [MaxIQp] [InitPQP] [MinPQp] [MAXPQp] "
           "[IPWeight] [RowRcEnable] [RowRcQpRange] [RowRcQpStep]\r\n"
           ,CODEC_TEST_QUICK_VIDEO_CONFIG);
    printf("\r\n");

    printf("VBR set====\r\n");
    printf("./nvtcodec_test %d [channel] [width] [height] [codec] [rotate] "
           "[codec_3dnr] [bitrate control type-->2] [LTR enable] [LTR Interval] [StaticTime] [ByteRate] "
           "[FrameRate] [GOP] [InitIQP] [MinIQp] [MaxIQp] [InitPQP] [MinPQp] [MAXPQp] "
           "[IPWeight] [ChangePos] [RowRcEnable] [RowRcQpRange] [RowRcQpStep]\r\n"
           ,CODEC_TEST_QUICK_VIDEO_CONFIG);

    printf("\r\n");
    printf("Fixed QP set====\r\n");
    printf("./nvtcodec_test %d [channel] [width] [height] [codec] [rotate] "
           "[codec_3dnr] [bitrate control type-->3] [LTR enable: set 0] [LTR Interval: set 0] [ByteRate] "
           "[FrameRate] [GOP] [IFixQP] [PFixQP]\r\n"
           ,CODEC_TEST_QUICK_VIDEO_CONFIG);

    printf("\r\n");
    printf("None Bitrate Control Set (for MJPEG)=====\n");
    printf("./nvtcodec_test %d [channel] [width] [height] [codec] [rotate] [codec_3dnr] [bitate control type --> 0]"
           " [LTR enable: set 0] [LTR Interval: set 0] [FrameRate] [GOP] [ByteRate]\r\n",CODEC_TEST_QUICK_VIDEO_CONFIG);


}

void test_quick_set_video_config(int argc, char *argv[]){

#if 1

    UINT32 channel = 0;
    UINT32 width   = 0;
    UINT32 height  = 0;
    UINT32 codec   = 0;
    UINT32 rotate  = 0;
    UINT32 uiLTRPreRef = 0;
    UINT32 uiLTRInterval=0;
    INT32 codec_3dnr = 0;
    UINT32 brc_type = 0;
    CodecVencVBR vbr_config = {0};
    CodecVencCBR cbr_config = {0};
    CodecVencFIXQP fixQP_config = {0};
    CodecVencBRCNoneInfo NoneBRC_config = {0};
    CodecVencSetting sVencSetting = {0};

    if(argc >=3){
        channel = atoi(argv[2]);
    }
    else{
        printf("argc:%d error, can not get channel id\n",argc);
        test_quick_usage();
        return;
    }

    if(argc >=4){
        width = atoi(argv[3]);
    }
    else{
        printf("argc:%d error, can not get width\n",argc);
        return;
    }

    if(argc >=5){
        height = atoi(argv[4]);
    }
    else{
        printf("argc:%d error, can not get height\n",argc);
        return;
    }

    if(argc>=6){
        codec = atoi(argv[5]);
    }
    else{
        printf("argc:%d error, can not get codec\n",argc);
        return;
    }

    if(argc>=7){
        rotate = atoi(argv[6]);
    }
    else{
        printf("argc:%d error, can not get rotate\n",argc);
        return;
    }

    if(argc>=8){
       codec_3dnr = atoi(argv[7]);
    }
    else{
        printf("argc:%d error, can not get codec 3dnr\n",argc);
        return;
    }

    if(argc>=9){
       brc_type = atoi(argv[8]);
    }
    else{
        printf("argc:%d error, can not get brc type\n",argc);
        return;
    }

    if(argc>=10){
       uiLTRPreRef = atoi(argv[9]);
    }
    else{
        printf("argc:%d error, can not get uiLTRPreRef\n",argc);
        return;
    }

    if(argc>=11){
       uiLTRInterval = atoi(argv[10]);
    }
    else{
        printf("argc:%d error, can not get uiLTRInterval\n",argc);
        return;
    }

    if(brc_type == CODEC_VENC_BRC_CBR){
        test_quick_set_cbr( argc, argv, &cbr_config);
    }
    else if(brc_type == CODEC_VENC_BRC_VBR){
        test_quick_set_vbr( argc, argv, &vbr_config);
    }
    else if(brc_type == CODEC_VENC_BRC_FIXQP){
        test_quick_set_fixedQP(argc, argv, &fixQP_config);
    }
    else if(brc_type == CODEC_VENC_BRC_NONE){

        test_quick_set_NoneBRC(argc, argv, &NoneBRC_config);
    }
    else{
        printf("brc_type:%d error\n",brc_type);
        return;
    }


    NVTRET ret;
    if ((ret = NvtCodec_GetVencSetting(channel, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d setting fail! (ret=%d)\n", channel, ret);
        return;
    }

    sVencSetting.eVencFormat = codec;
    sVencSetting.uiTargetWidth = width;
    sVencSetting.uiTargetHeight = height;
    sVencSetting.eBRCType      = brc_type;
    sVencSetting.sNRSetting.bEnable    = codec_3dnr;
    sVencSetting.eVencRotateType = rotate;
    sVencSetting.sLTRInfo.uiLTRInterval = uiLTRInterval;
    sVencSetting.sLTRInfo.uiLTRPreRef = uiLTRPreRef;

    printf("set video config=====\r\n");
    printf("  channel:      %d\r\n",channel);
    printf("  video format: %d\r\n",sVencSetting.eVencFormat);
    printf("  resolution: %d x %d\r\n",sVencSetting.uiTargetWidth, sVencSetting.uiTargetHeight);
    printf("  codec NR: %s\r\n",(sVencSetting.sNRSetting.bEnable == 0) ? "off":"on");
    printf("  rotate    : %d\r\n", sVencSetting.eVencRotateType);
    printf("  uiLTRPreRef: %d\r\n",sVencSetting.sLTRInfo.uiLTRPreRef);
    printf("  uiLTRInterval: %d\r\n",sVencSetting.sLTRInfo.uiLTRInterval);

    if(brc_type == CODEC_VENC_BRC_CBR){
        memcpy(&sVencSetting.sVencCBRConfig, &cbr_config, sizeof(CodecVencCBR));
        printf("  CBR mode===\r\n");
        printf("  StaticTime:  %d\r\n",sVencSetting.sVencCBRConfig.uiStaticTime);
        printf("  ByteRate:    %d\r\n",sVencSetting.sVencCBRConfig.uiByteRate);
        printf("  uiFrameRate: %d\r\n",sVencSetting.sVencCBRConfig.uiFrameRate);
        printf("  uiGOP      : %d\r\n",sVencSetting.sVencCBRConfig.uiGOP);
        printf("  uiInitIQp  : %d\r\n",sVencSetting.sVencCBRConfig.uiInitIQp);
        printf("  uiMinIQp   : %d\r\n",sVencSetting.sVencCBRConfig.uiMinIQp);
        printf("  uiMaxIQp   : %d\r\n",sVencSetting.sVencCBRConfig.uiMaxIQp);
        printf("  uiInitPQp  : %d\r\n",sVencSetting.sVencCBRConfig.uiInitPQp);
        printf("  uiMinPQp   : %d\r\n",sVencSetting.sVencCBRConfig.uiMinPQp);
        printf("  uiMaxPQp   : %d\r\n",sVencSetting.sVencCBRConfig.uiMaxPQp);
        printf("  iIPWeight  : %d\r\n",sVencSetting.sVencCBRConfig.iIPWeight);
        printf("  uiRowRcEnalbe: %d\r\n",sVencSetting.sVencCBRConfig.uiRowRcEnalbe);
        printf("  uiRowRcQpRange: %d\r\n",sVencSetting.sVencCBRConfig.uiRowRcQpRange);
        printf("  uiRowRcQpStep: %d\r\n",sVencSetting.sVencCBRConfig.uiRowRcQpStep);


    }
    else if(brc_type == CODEC_VENC_BRC_VBR){
        memcpy(&sVencSetting.sVencVBRConfig, &vbr_config, sizeof(CodecVencVBR));
        printf("  VBR mode===\r\n");
        printf("  StaticTime:  %d\r\n",sVencSetting.sVencVBRConfig.uiStaticTime);
        printf("  ByteRate:    %d\r\n",sVencSetting.sVencVBRConfig.uiByteRate);
        printf("  uiFrameRate: %d\r\n",sVencSetting.sVencVBRConfig.uiFrameRate);
        printf("  uiGOP      : %d\r\n",sVencSetting.sVencVBRConfig.uiGOP);
        printf("  uiInitIQp  : %d\r\n",sVencSetting.sVencVBRConfig.uiInitIQp);
        printf("  uiMinIQp   : %d\r\n",sVencSetting.sVencVBRConfig.uiMinIQp);
        printf("  uiMaxIQp   : %d\r\n",sVencSetting.sVencVBRConfig.uiMaxIQp);
        printf("  uiInitPQp  : %d\r\n",sVencSetting.sVencVBRConfig.uiInitPQp);
        printf("  uiMinPQp   : %d\r\n",sVencSetting.sVencVBRConfig.uiMinPQp);
        printf("  uiMaxPQp   : %d\r\n",sVencSetting.sVencVBRConfig.uiMaxPQp);
        printf("  iIPWeight  : %d\r\n",sVencSetting.sVencVBRConfig.iIPWeight);
        printf("  uiChangePos: %d\r\n",sVencSetting.sVencVBRConfig.uiChangePos);
        printf("  uiRowRcEnalbe: %d\r\n",sVencSetting.sVencVBRConfig.uiRowRcEnalbe);
        printf("  uiRowRcQpRange: %d\r\n",sVencSetting.sVencVBRConfig.uiRowRcQpRange);
        printf("  uiRowRcQpStep: %d\r\n",sVencSetting.sVencVBRConfig.uiRowRcQpStep);
    }
    else if(brc_type == CODEC_VENC_BRC_FIXQP){
        memcpy(&sVencSetting.sVencFixQPConfig, &fixQP_config, sizeof(CodecVencFIXQP));
        printf("  FixQP mode===\r\n");
        printf("  uiByteRate:  %d\r\n",sVencSetting.sVencFixQPConfig.uiByteRate);
        printf("  uiFrameRate: %d\r\n",sVencSetting.sVencFixQPConfig.uiFrameRate);
        printf("  uiGOP     :  %d\r\n",sVencSetting.sVencFixQPConfig.uiGOP);
        printf("  uiIFixQP  :  %d\r\n",sVencSetting.sVencFixQPConfig.uiIFixQP);
        printf("  uiPFixQP  :  %d\r\n",sVencSetting.sVencFixQPConfig.uiPFixQP);

    }
    else if(brc_type == CODEC_VENC_BRC_NONE){

        memcpy(&sVencSetting.sVencBRCNoneInfo, &NoneBRC_config, sizeof(CodecVencBRCNoneInfo));
        printf("  None BRC mode===\r\n");
        printf("  uiFrameRate:  %d\r\n",sVencSetting.sVencBRCNoneInfo.uiFrameRate);
        printf("  uiGOP      :  %d\r\n",sVencSetting.sVencBRCNoneInfo.uiGOP);
        printf("  uiByteRate :  %d\r\n",sVencSetting.sVencBRCNoneInfo.uiByteRate);
    }
    else{
        printf("brc_type:%d error\n",brc_type);
        return;
    }

    // Set new video encoder setting
    if ((ret = NvtCodec_SetVencSetting(channel, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Set venc %d setting fail! (ret=%d)\n", channel, ret);
        return;
    }

    return;
#else
    printf("not support now!\r\n");
#endif

}

static int test_bc_status_blk(void)
{
    CodecBCSetting BCConfig ={0};
 	CodecBCBLKData *BCresult = NULL;//{0};
	BCresult = malloc(sizeof(CodecBCBLKData));
	if(BCresult == NULL)
	{
		DBG_ERR("malloc fail\n");
		return -1;
	}
    NvtCodec_ResetBC();
    //get info
    NvtCodec_GetBCSetting(&BCConfig);

    printf("current BC config====\r\n");
    printf("uiUpdateProb           =%d\r\n",BCConfig.uiUpdateProb);
    printf("bUpdateNeighborEn      =%d\r\n",BCConfig.bUpdateNeighborEn);
    printf("bDeghostEn             =%d\r\n",BCConfig.bDeghostEn);
    printf("uiMaxFGFrameNum        =%d\r\n",BCConfig.uiMaxFGFrameNum);
    printf("uiMinOBJAreaPCT        =%d\r\n",BCConfig.uiMinOBJAreaPCT);
    printf("uiTHCrossFrameNum      =%d\r\n",BCConfig.uiTHCrossFrameNum);
    printf("uiTHFeature            =%d\r\n",BCConfig.uiTHFeature);
    printf("uiSensi                =%d\r\n",BCConfig.uiSensi);
    printf("uiBlkNumH              =%d\r\n",BCConfig.uiBlkNumH);
    printf("uiBlkNumW              =%d\r\n",BCConfig.uiBlkNumW);
    printf("MinOBJBlkNum           =%d\r\n",BCConfig.MinOBJBlkNum);
    printf("FGPCTperBlk            =%d\r\n",BCConfig.FGPCTperBlk);

    int timeout = 40;
    UINT32 i=0;
    NvtCodec_BCStart(1);
    CodecObjData TargetCoord ={0};

    TargetCoord.uiCoordinateX =0;
    TargetCoord.uiCoordinateY =0;
    TargetCoord.uiWidth = 1920;
    TargetCoord.uiHeight =1080;
    printf("start test!!!\r\n");
    while(timeout > 0){

		if(memset(BCresult,0,sizeof(CodecBCBLKData)) == NULL)
		{
			DBG_ERR("memset of BCresult fail\n");
			NvtCodec_BCStart(0);
			return -1;
		}
        
		NvtCodec_GetBCBLKResult(BCresult, &TargetCoord);

        for(i=0; i < BCresult->uiObjectNum ;i++){
            printf("%d:(%lu,%lu,%lu,%lu)\r\n",i,
            BCresult->sObjectData[i].uiCoordinateY,
            BCresult->sObjectData[i].uiCoordinateY,
            BCresult->sObjectData[i].uiWidth,
            BCresult->sObjectData[i].uiHeight);
        }

        usleep(250000);
        timeout--;
    }
    printf("finish test!!\r\n");
    NvtCodec_BCStart(0);
	free(BCresult);
    return 0;
}
static int test_bc_status(void){


    CodecBCSetting BCConfig ={0};

    NvtCodec_ResetBC();
    //get info
    NvtCodec_GetBCSetting(&BCConfig);

    printf("current BC config====\r\n");
    printf("uiUpdateProb           =%d\r\n",BCConfig.uiUpdateProb);
    printf("bUpdateNeighborEn      =%d\r\n",BCConfig.bUpdateNeighborEn);
    printf("bDeghostEn             =%d\r\n",BCConfig.bDeghostEn);
    printf("uiMaxFGFrameNum        =%d\r\n",BCConfig.uiMaxFGFrameNum);
    printf("uiMinOBJAreaPCT        =%d\r\n",BCConfig.uiMinOBJAreaPCT);
    printf("uiTHCrossFrameNum      =%d\r\n",BCConfig.uiTHCrossFrameNum);
    printf("uiTHFeature            =%d\r\n",BCConfig.uiTHFeature);
    printf("uiSensi                =%d\r\n",BCConfig.uiSensi);
    printf("uiBlkNumH              =%d\r\n",BCConfig.uiBlkNumH);
    printf("uiBlkNumW              =%d\r\n",BCConfig.uiBlkNumW);
    printf("MinOBJBlkNum           =%d\r\n",BCConfig.MinOBJBlkNum);
    printf("FGPCTperBlk            =%d\r\n",BCConfig.FGPCTperBlk);

    int timeout = 40;
    UINT32 i=0;
    NvtCodec_BCStart(1);
    CodecObjData TargetCoord ={0};

    TargetCoord.uiCoordinateX =0;
    TargetCoord.uiCoordinateY =0;
    TargetCoord.uiWidth = 1920;
    TargetCoord.uiHeight =1080;
    printf("start test!!!\r\n");

    int first_fg_get =0;
    int first_bg_get =0;


    while(timeout > 0){

        CodecBCData BCresult ={0};
        NvtCodec_GetBCResult(&BCresult, &TargetCoord);

        for(i=0; i < BCresult.uiObjectNum ;i++){
            printf("%d:  (x,y,w,h)==>(%lu,%lu,%lu,%lu)\r\n",i,
            BCresult.sObjectData[i].uiCoordinateY,
            BCresult.sObjectData[i].uiCoordinateY,
            BCresult.sObjectData[i].uiWidth,
            BCresult.sObjectData[i].uiHeight);

            if(first_fg_get == 0){
                 CodecImageBuf fgBuf={0};
                 NvtCodec_GetBCForeground(&fgBuf);
                 printf("fg img test\r\n");
                 printf("width:%d height:%d format:%d\r\n",fgBuf.uiWidth,fgBuf.uiHeight,fgBuf.ePxlFmt);
                 printf("addr Y:%x    U:%x   V:%x\r\n",fgBuf.uiPxlAddr[0],fgBuf.uiPxlAddr[1],fgBuf.uiPxlAddr[2]);
                 first_fg_get++;
                 NvtCodec_ReleaseBCForeground();

            }


            if(first_bg_get == 0){
                 CodecImageBuf bgBuf={0};
                 NvtCodec_GetBCBackground(&bgBuf);
                 printf("bg img test\r\n");
                 printf("width:%d height:%d format:%d\r\n",bgBuf.uiWidth,bgBuf.uiHeight,bgBuf.ePxlFmt);
                 printf("addr Y:%x    U:%x   V:%x\r\n",bgBuf.uiPxlAddr[0],bgBuf.uiPxlAddr[1],bgBuf.uiPxlAddr[2]);
                 first_bg_get++;
                 NvtCodec_ReleaseBCBackground();
            }


        }

        usleep(250000);
        timeout--;
    }
    printf("finish test!!\r\n");
    NvtCodec_BCStart(0);

    return 0;
}

static int test_bc_setting(void){


    CodecBCSetting BCConfig ={0};

    NvtCodec_ResetBC();
    //get info
    NvtCodec_GetBCSetting(&BCConfig);

    printf("current BC config====\r\n");
    printf("uiUpdateProb           =%d\r\n",BCConfig.uiUpdateProb);
    printf("bUpdateNeighborEn      =%d\r\n",BCConfig.bUpdateNeighborEn);
    printf("bDeghostEn             =%d\r\n",BCConfig.bDeghostEn);
    printf("uiMaxFGFrameNum        =%d\r\n",BCConfig.uiMaxFGFrameNum);
    printf("uiMinOBJAreaPCT        =%d\r\n",BCConfig.uiMinOBJAreaPCT);
    printf("uiTHCrossFrameNum      =%d\r\n",BCConfig.uiTHCrossFrameNum);
    printf("uiTHFeature            =%d\r\n",BCConfig.uiTHFeature);
    printf("uiSensi                =%d\r\n",BCConfig.uiSensi);
    printf("uiBlkNumH              =%d\r\n",BCConfig.uiBlkNumH);
    printf("uiBlkNumW              =%d\r\n",BCConfig.uiBlkNumW);
    printf("MinOBJBlkNum           =%d\r\n",BCConfig.MinOBJBlkNum);
    printf("FGPCTperBlk            =%d\r\n",BCConfig.FGPCTperBlk);


   //set test info
    BCConfig.uiUpdateProb = 1;
    BCConfig.bUpdateNeighborEn = 1;
    BCConfig.bDeghostEn = 1;
    BCConfig.uiMaxFGFrameNum = 3;
    BCConfig.uiMinOBJAreaPCT = 30;
    BCConfig.uiTHCrossFrameNum = 40;
    BCConfig.uiTHFeature = 20;
    BCConfig.uiSensi  = 33;
    BCConfig.uiBlkNumH = 20;
    BCConfig.uiBlkNumW = 21;
    BCConfig.MinOBJBlkNum = 60;
    BCConfig.FGPCTperBlk = 54;

    NvtCodec_SetBCSetting(&BCConfig);

    NvtCodec_GetBCSetting(&BCConfig);

    printf("after set test BC config====\r\n");
    printf("uiUpdateProb           =%d\r\n",BCConfig.uiUpdateProb);
    printf("bUpdateNeighborEn      =%d\r\n",BCConfig.bUpdateNeighborEn);
    printf("bDeghostEn             =%d\r\n",BCConfig.bDeghostEn);
    printf("uiMaxFGFrameNum        =%d\r\n",BCConfig.uiMaxFGFrameNum);
    printf("uiMinOBJAreaPCT        =%d\r\n",BCConfig.uiMinOBJAreaPCT);
    printf("uiTHCrossFrameNum      =%d\r\n",BCConfig.uiTHCrossFrameNum);
    printf("uiTHFeature            =%d\r\n",BCConfig.uiTHFeature);
    printf("uiSensi                =%d\r\n",BCConfig.uiSensi);
    printf("uiBlkNumH              =%d\r\n",BCConfig.uiBlkNumH);
    printf("uiBlkNumW              =%d\r\n",BCConfig.uiBlkNumW);
    printf("MinOBJBlkNum           =%d\r\n",BCConfig.MinOBJBlkNum);
    printf("FGPCTperBlk            =%d\r\n",BCConfig.FGPCTperBlk);



    return 0;
}


#if VQA_TEST
#include <pthread.h>
int vqa_exit = -1;   // Notify program exit

pthread_t vqa_thread_id;
static void init_vqa_param(void)
{
    int ret =0;
    IVSVQAParam sIVSVQAParam;

    memset(&sIVSVQAParam, 0, sizeof(IVSVQAParam));
    sIVSVQAParam.fps = 5;
    //enable
    sIVSVQAParam.enable_param.en_too_light = 1;
    sIVSVQAParam.enable_param.en_too_dark = 1;
    sIVSVQAParam.enable_param.en_blur = 1;
    sIVSVQAParam.enable_param.en_auto_adj_param = 1;
    sIVSVQAParam.enable_param.en_ref_md = 1;
    //global
    sIVSVQAParam.global_param.auto_adj_period = 5;
    sIVSVQAParam.global_param.g_alarm_frame_num = 20;
	 //overexposure
    sIVSVQAParam.light_param.too_light_strength_th = 215;
    sIVSVQAParam.light_param.too_light_cover_th = 15;
    sIVSVQAParam.light_param.too_light_alarm_times = 5;
    //underexposure
    sIVSVQAParam.light_param.too_dark_strength_th = 50;
    sIVSVQAParam.light_param.too_dark_cover_th = 75;
    sIVSVQAParam.light_param.too_dark_alarm_times = 5;
	//blur
    sIVSVQAParam.contrast_param.blur_strength = 8;
    sIVSVQAParam.contrast_param.blur_cover_th = 85;
    sIVSVQAParam.contrast_param.blur_alarm_times = 5;
    
	ret = NvtCodec_SetVqaParam(&sIVSVQAParam);
    if (ret < 0) {
        printf("Error to set VQA parameters!! ret=%d\n",ret);
        //return ER_GET_DATA_FAIL;
    }
    sleep(1);
    //return ER_SUCCESS;
}

static void *vqa_thread(void *arg)
{
    int ret;
    IPCAM_VQA_RESULT vqa_res = {0};

    while(vqa_exit == 0) {
        printf("Enter \"q\" to stop vqa\n");
        ret = NvtCodec_GetVqaResult(&vqa_res, 3000);
        if(ret < 0) { //-1:error, 0:sucess
            printf("Error to get vqa result(%d)\n", ret);
            continue;
        }
        if (vqa_res.TooLight == 1) {
            printf("vqa detect: too light\n");
        }
        if (vqa_res.TooDark == 1) {
            printf("vqa detect: too dark\n");
        }
        if (vqa_res.Blur == 1) {
           printf("vqa detect: blur\n");
        }
    }
    return 0;
}

static void start_vqa(void)
{
    int ret;
    init_vqa_param();
    if (vqa_exit == 0){
        printf("VQA is running...\n");
        return;
    }else{
        printf("Start VQA test...(%d)\n",vqa_exit);
        vqa_exit= 0;

    }

    ret = pthread_create(&vqa_thread_id, NULL, vqa_thread, (void *)0);
    if(ret < 0) {
        printf("create vqa thread failed");
        return;
    }
}


static void stop_vqa(void)
{
    int ret;
    IVSVQAParam sIVSVQAParam;
    if (vqa_exit == 1){
        printf("VQA is not running...)\n");
        return;
    }else{
        printf("Stop VQA test...(%d)\n",vqa_exit);
        vqa_exit = 1;

    }
    sIVSVQAParam.fps = 0;
   ret = NvtCodec_SetVqaParam(&sIVSVQAParam);
    if (ret < 0) {
        printf("Error to set VQA parameters!! ret=%d\n",ret);
        //return ER_GET_DATA_FAIL;
        return;
    }
    sleep(1);
    pthread_join(vqa_thread_id, NULL);
}

static void test_vqa(void)
{
    int key;
    start_vqa();

    printf("Enter \"q\" to stop vqa\n");
    while(1) {
        key = getchar();
        if(key == 'q') {
            break;
        }
    }

    stop_vqa();
    return;

}
#endif
void test_video_data(int vencID){


    unsigned int  frameCounter=0;
    CodecVencParamSet sVencParamSet = {0};
    CodecVencSetting sVencSetting = {0};
    NVTRET ret;
    int i=0;
    FILE *pfVideo = NULL;
    if ((ret = NvtCodec_GetVencSetting(vencID, &sVencSetting)) != ER_SUCCESS)
    {
        DBG_ERR("Get venc %d setting fail! (ret=%d)\n", vencID, ret);
        return;

    }


	//sprintf(fileName, VIDEO_FILE_H264, vencID);
    if ((pfVideo = fopen("/mnt/sd/test", "wb")) == NULL){
        DBG_WRN("open fail, can't save encoded data\n");
    }

    // Start video encoder
    if ((ret = NvtCodec_StartVenc(vencID)) != ER_SUCCESS)
    {
        DBG_ERR("Venc %d start fail! (ret=%d)\n", vencID, ret);
        goto TEST_VENC_END0_TEST;
    }

    if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264 ||
		sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H265)
    {
        // Get encoder parameter set
        if ((ret = NvtCodec_RequestVencParamSet(vencID, &sVencParamSet)) != ER_SUCCESS)
        {
            DBG_ERR("Get venc parameter set fail! (ret=%d)\n", vencID, ret);
            goto TEST_VENC_END1_TEST;
        }

		DBG_IND("VPS : ");
        for (i = 0; i < sVencParamSet.VPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.VPS[i]);

		DBG_IND("\n");

        DBG_IND("SPS : ");
        for (i = 0; i < sVencParamSet.SPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.SPS[i]);

        DBG_IND("\n");

        DBG_IND("PPS : ");
        for (i = 0; i < sVencParamSet.PPSLength; i++)
            DBG_IND("0x%02X ", sVencParamSet.PPS[i]);

        DBG_IND("\n");
#ifdef DEBUG
		if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264)
	        H264_SPS_parse(sVencParamSet.SPS+5, sVencParamSet.SPSLength);
#endif
    }
    DBG_DUMP("Save video encoded data  ...\n");

    unsigned int saveFrameNum =  DEFAULT_SAVE_VIDEO_FRAME_NUM;
    BOOL getFirstIFrame=NVT_FALSE;
    CodecVencStream sVencStream;
    bCopyBuf = 0;
    while (frameCounter <= saveFrameNum)
    {
        // Receive video encoded data
        if ((ret = NvtCodec_GetVencStream(vencID, &sVencStream, 3000, bCopyBuf)) != ER_SUCCESS)
        {
            DBG_ERR("Get venc stream fail! (ret=%d)\n", vencID, ret);
            goto TEST_VENC_END1_TEST;
        }

        if (sVencStream.eVencFormat == CODEC_VENC_FORMAT_H264 ||
			sVencStream.eVencFormat == CODEC_VENC_FORMAT_H265)
        {
            DBG_DUMP("Encode Format=%d, Sequence Number=%u, SVC-T ID=%u FrameType:%u\n",
                        sVencStream.eVencFormat,
                        sVencStream.uiSeqNum,
                        sVencStream.uiSVCTemporalLayerID,
                        sVencStream.uiFrameType);
        }
        else
        {
            DBG_DUMP("Encode Format=%d, Sequence Number=%u, \n",
                        sVencStream.eVencFormat,
                        sVencStream.uiSeqNum);
        }

        for (i = 0; i < (int)sVencStream.uiPackNum; i++)
        {
            switch (sVencStream.eVencFormat)
            {
                case CODEC_VENC_FORMAT_YUV:
                   DBG_DUMP("Data Length=%d, isLastPack=%d %x %x %x\n",
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack,
                        *((char *)sVencStream.sVencPack[i].puiStreamPayloadData),
                        *((char *)(sVencStream.sVencPack[i].puiStreamPayloadData+1)),
                        *((char *)(sVencStream.sVencPack[i].puiStreamPayloadData+2))
                        );
                    if (getFirstIFrame == NVT_FALSE)
                        getFirstIFrame = NVT_TRUE;
                    break;
                case CODEC_VENC_FORMAT_MJPEG:
                    DBG_DUMP("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eJPEGType,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE)
                        getFirstIFrame = NVT_TRUE;
                    break;
				case CODEC_VENC_FORMAT_H265:
                    DBG_DUMP("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eH264Type,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE &&
                        sVencStream.sVencPack[i].uPackType.eH265Type == H265_NALU_TYPE_IDR)
                        getFirstIFrame = NVT_TRUE;
					break;
                case CODEC_VENC_FORMAT_H264:
                default:
                    DBG_DUMP("  Pack Type=%d, Data Length=%d, isLastPack=%d\n",
                        sVencStream.sVencPack[i].uPackType.eH264Type,
                        sVencStream.sVencPack[i].uiDataLength,
                        sVencStream.sVencPack[i].bLastPack);

                    if (getFirstIFrame == NVT_FALSE &&
                        sVencStream.sVencPack[i].uPackType.eH264Type == H264_NALU_TYPE_IDR)
                        getFirstIFrame = NVT_TRUE;
                    break;
            }
        }
        // Save video encoded data from first I-frame
        if (getFirstIFrame == NVT_TRUE)
        {
            for (i = 0; i < (int)sVencStream.uiPackNum; i++)
                fwrite(sVencStream.sVencPack[i].puiStreamPayloadData, sVencStream.sVencPack[i].uiDataLength, 1, pfVideo);

            frameCounter++;
        }

        // Release video encoded data
        NvtCodec_ReleaseVencStream(vencID, &sVencStream);
    }

TEST_VENC_END1_TEST:
    // Stop video encoder
    NvtCodec_StopVenc(vencID);

TEST_VENC_END0_TEST:
    if (pfVideo)
    {
        fflush(pfVideo);
        fclose(pfVideo);
    }


}


int main(int argc, char *argv[])
{
    int testItem=CODEC_TEST_MAX, testChannel=0, testParam0=-1;

    if ((argc < 2) || (testItem = atoi(argv[1])) >= CODEC_TEST_MAX)
    {
        show_help();
        return 0;
    }

    if (argc >= 3)
        testChannel = atoi(argv[2]);

    if (argc >= 4)
        testParam0 = atoi(argv[3]);

	NvtCodec_EventHandler_Init();

    switch (testItem)
    {
        case CODEC_TEST_VIDEO_ENCODER:
            test_venc(testChannel, testParam0);
            break;
        case CODEC_TEST_VIDEO_AQINFO:
            test_venc_aqinfo(testChannel);
            break;
        case CODEC_TEST_VIDEO_ENCODER_2CH:
            test_venc_2ch();
            break;
	case CODEC_TEST_VIDEO_SNAPSHOT:
            test_snapshot(testChannel);
            break;
        case CODEC_TEST_VIDEO_ENCODER_ROI:
            test_venc_roi(testChannel);
            break;
        case CODEC_TEST_AUDIO_INPUT:
            test_ainput(testChannel, testParam0);
            break;
        case CODEC_TEST_AUDIO_OUTPUT:
            test_aout(testChannel);
            break;
        case CODEC_TEST_FACE_DETECTION:
            test_fd();
            break;
#if VQA_TEST
        case CODEC_TEST_VIDEO_QUALITY_ANALYSIS:
            test_vqa();
            break;
#endif
        case CODEC_TEST_VIDEO_OUTPUT:
            test_vout(testChannel);
            break;
#if 0
        case CODEC_TEST_MOTION_DETECTION:
            test_motion();
            break;
        case CODEC_TEST_MOTION_CONFIG:
            test_motion_cfg_set();
            break;
        case CODEC_TEST_MOTION_CONFIG_GET:
            test_motion_cfg_get();
            break;
        case CODEC_TEST_FACE_TRACKING:
            test_ftg();
            break;
        case CODEC_TEST_FACE_TRACKING_GET_SNAPSHOT:
            test_ftg_get_snapshot();
            break;
        case CODEC_TEST_FACE_TRACKING_REMOVE_SNAPSHOT:
            test_ftg_remove_snapshot();
            break;
        case CODEC_TEST_OSD_CONFIG_SET:
        {
		//printf("[%s]CODEC_TEST_OSD_CONFIG_SET \n", __FUNCTION__);

		if (argc == 4)
		{
			test_osd_display((char*)argv[3], atoi((char*)argv[2])); //xxx.ttf
		}
		else
		{
			test_osd_display((char*)NULL, atoi((char*)argv[2]));
		}
    		break;
        }
        case CODEC_TEST_OSD_CONFIG_MASK:
        {
		//printf("[%s]CODEC_TEST_OSD_CONFIG_MASK \n", __FUNCTION__);
    		test_osd_cfg_mask(atoi((char*)argv[2])); // 1 => enable, 0 => disable
    		break;
        }
        case CODEC_TEST_BLENDING:
        {
            test_blending(testChannel);
            break;
        }
#endif
        case CODEC_TEST_RECORD:
        {
            test_record();
            break;
        }
        case CODEC_TEST_QUICK_VIDEO_CONFIG:
        {
            test_quick_set_video_config(argc, argv);
            break;
        }
        case CODEC_TEST_TAMPERING_DETECTION:
            test_td();
            break;
        case CODEC_TEST_BC_SETTING:
            test_bc_setting();
            break;
        case CODEC_TEST_BC_STATUS:
            test_bc_status();
            break;
        case CODEC_TEST_BC_STATUS_BLK:
            test_bc_status_blk();
            break;
        case CODEC_TEST_VIDEO_DATA:
            test_video_data(testChannel);
            break;
        default:
            printf("error cmd=%d\r\n",testItem);
            break;
    }
	NvtCodec_EventHandler_Exit();
    return 0;
}


