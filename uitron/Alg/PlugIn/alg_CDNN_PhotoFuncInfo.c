/**
    NVT CDNN application.

    Application of NVT CDNN (CEVA Deep Neural Network) functions.

    @file       alg_CDNN_PhotoFuncInfo.c
    @ingroup    NvtCDNN

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/

// Include ---------------------------------------------------------------------

#include "SysCommon.h"
#include "Perf.h"
#include "GxImage.h"
#include "GxStrg.h"
#include "ImageUnit_UserProc.h"
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
#include "ImageApp_MovieMulti.h"
#endif
#include "NvtCDNN.h"
#include "alg_CDNN_PhotoFuncInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
#define THIS_DBGLVL         2   // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          alg_CDNN
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" // *=All, [mark1]=CustomClass
#include "DebugModule.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if MOVIE_CDNN_FUNC || IPCAM_CDNN_FUNC

// Definitions -----------------------------------------------------------------

#define CDNN_PATH                       "A:\\CNNLib\\"
#define CDNN_INPUT_PATH                 CDNN_PATH "DI\\CDNN_flow\\"
#define CDNN_OUTPUT_PATH                CDNN_PATH "DO\\CDNN_flow\\"
#define CDNN_NET_PATH                   CDNN_INPUT_PATH "para\\"
#define CDNN_IMG_PATH                   CDNN_PATH "DI\\imgData\\Alexnet\\"

#define CDNN_GOOGLENET_FILE             CDNN_NET_PATH "deploy1001_v400f_googlenet.cdnnQdata"
#define CDNN_FRCNN_RESNET50_FILE        CDNN_NET_PATH "deploy1001_v400f_frcnn_resnet50.cdnnQdata"
#define CDNN_FRCNN_INPUT2_FILE          CDNN_NET_PATH "proposal_2.mat"
#define CDNN_FRCNN_PROPOSAL_SCORE_FILE  CDNN_OUTPUT_PATH "proposal_score.bin"
#define CDNN_FRCNN_BBOX_FILE            CDNN_OUTPUT_PATH "bbox.bin"
#define CDNN_FRCNN_CLS_FILE             CDNN_OUTPUT_PATH "cls.bin"
#define CDNN_FRCNN_PROPOSAL_ROIS_FILE   CDNN_OUTPUT_PATH "proposal_rois.bin"
#define CDNN_MOBILENET_SSD_FILE         CDNN_NET_PATH "deploy1001_v400f_mobilenet_ssd.cdnnQdata"
#define CDNN_MOBILENET_SSD_CLASS_ID_FILE        CDNN_OUTPUT_PATH "class_id.bin"
#define CDNN_MOBILENET_SSD_PROB_ROI_INFO_FILE   CDNN_OUTPUT_PATH "prob_roi_info.bin"


#define CDNN_NET_SEL                    CDNN_GOOGLENET //CDNN_FRCNN_RESNET50 //CDNN_MOBILENET_SSD
#define CDNN_NET_FILE                   CDNN_GOOGLENET_FILE //CDNN_FRCNN_RESNET50_FILE //CDNN_MOBILENET_SSD_FILE
#if (CDNN_NET_SEL == CDNN_FRCNN_RESNET50)
#define CDNN_INPUT2_FILE                CDNN_FRCNN_INPUT2_FILE
#else
#define CDNN_INPUT2_FILE                NULL
#endif

#define CDNN_DSP_SEL                    CDNN_DSP1
#define CDNN_SENSOR_FLOW                1 // 1: Sensor mode, 0: File mode


#define __CDNN_PERF__                   DISABLE
#define __CDNN_DUMP_INPUT_IMG__         DISABLE

#if IPCAM_CDNN_FUNC
#define CDNN_ISF_OUT_PORT               ISF_OUT16
#else
#define CDNN_ISF_OUT_PORT               ISF_OUT15
#endif

// Variables -------------------------------------------------------------------

#if (CDNN_SENSOR_FLOW == 0)

#define CDNN_MAX_FILE_PATH              100

static UINT32 g_cdnn_test_idx = 0;
static CHAR g_cdnn_test_file[][CDNN_MAX_FILE_PATH] = {
#if (CDNN_NET_SEL == CDNN_FRCNN_RESNET50)
	CDNN_IMG_PATH "bus.jpg",
#elif (CDNN_NET_SEL == CDNN_MOBILENET_SSD)
	CDNN_IMG_PATH "bus_300x300.jpg",
#else
	CDNN_IMG_PATH "dog.jpg",
#endif
#if (CDNN_NET_SEL != CDNN_FRCNN_RESNET50 && CDNN_NET_SEL != CDNN_MOBILENET_SSD)
	CDNN_IMG_PATH "bicycle.jpg",
	CDNN_IMG_PATH "bird.jpg",
	CDNN_IMG_PATH "boat.jpg",
	CDNN_IMG_PATH "car.jpg",
	CDNN_IMG_PATH "cat.jpg",
	CDNN_IMG_PATH "girl.jpg",
	CDNN_IMG_PATH "man.jpg",
	CDNN_IMG_PATH "peacock.jpg",
	CDNN_IMG_PATH "sheep.jpg",
	CDNN_IMG_PATH "train.jpg",
	CDNN_IMG_PATH "turtle.jpg",
#endif
};

#endif // (CDNN_SENSOR_FLOW == 0)

// Functions -------------------------------------------------------------------

VOID alg_CDNNShowResult(CDNN_OUTPUT_CLASSES *out)
{
	UINT8 idx;

	DBG_DUMP("CDNN Results:\r\n");
	for (idx = 0; idx < out->class_num; idx++) {
		DBG_DUMP("no=%d, label=%s, score=%d\r\n", idx + 1, out->classes[idx].label, out->classes[idx].score);
	}
	DBG_DUMP("\r\n");

	return;
}

VOID alg_CDNNShowResult_FRCNN(CDNN_OUTPUT_FRCNN *out)
{
	FST_FILE filehdl;
	UINT32 bbox_output_size = out->output_size*4;
	UINT32 cls_output_size = out->output_size;
	UINT32 proposal_score_output_size = 40;
	UINT32 proposal_rois_output_size = out->proposal_rois_output_size;

	DBG_DUMP("proposal_score_addr: 0x%X\r\n"    , out->proposal_score_addr);
	DBG_DUMP("bbox_addr: 0x%X\r\n"              , out->bbox_addr);
	DBG_DUMP("cls_addr: 0x%X\r\n"               , out->cls_addr);
	DBG_DUMP("proposal_rois_addr: 0x%X\r\n"     , out->proposal_rois_addr);
	DBG_DUMP("output_size: %d\r\n"              , out->output_size);
	DBG_DUMP("proposal_rois_output_size: %d\r\n", out->proposal_rois_output_size);

	filehdl = FileSys_OpenFile(CDNN_FRCNN_PROPOSAL_SCORE_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->proposal_score_addr, &proposal_score_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN_FRCNN_BBOX_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->bbox_addr, &bbox_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN_FRCNN_CLS_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->cls_addr, &cls_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN_FRCNN_PROPOSAL_ROIS_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->proposal_rois_addr, &proposal_rois_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);
}

VOID alg_CDNNShowResult_MOBILENET_SSD(CDNN_OUTPUT_MOBILENET_SSD *out)
{
	FST_FILE filehdl;
	UINT32 class_id_size = out->class_id_size;
	UINT32 prob_roi_info_size = out->prob_roi_info_size;

	DBG_DUMP("out->class_id_size = %d\r\n", out->class_id_size);
	DBG_DUMP("out->prob_roi_info_size = %d\r\n", out->prob_roi_info_size);

	filehdl = FileSys_OpenFile(CDNN_MOBILENET_SSD_CLASS_ID_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->class_id_addr, &class_id_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN_MOBILENET_SSD_PROB_ROI_INFO_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->prob_roi_info_addr, &prob_roi_info_size, 0, NULL);
	FileSys_CloseFile(filehdl);
}

VOID alg_CDNNInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init after file system is ready in alg_CDNNProcess()
	return;
}

VOID alg_CDNNProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	MEM_RANGE           buf2;
	CDNN_DSP            dsp = CDNN_DSP_SEL;
	CDNN_BUF_PRMS       buf_prms = {0};
	CDNN_INIT_PRMS      init_prms = {0};
	CDNN_STA            status = CDNN_STA_OK;
	ISF_DATA            isf_data;
	ISF_UNIT            *pUnit;
	ISF_PORT            *pSrc;
	static UINT32       check_fs_status = 0;
#if (CDNN_SENSOR_FLOW == 1)
	CDNN_SENSOR_PRMS    sensor_prms = {0};
#else
	CDNN_FILE_PRMS      file_prms = {0};
#endif

	pUnit = &ISF_UserProc;
#if (MOVIE_CDNN_FUNC && MOVIE_MAPPING_MULTIREC)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, CDNN_ISF_OUT_PORT);
#endif

	if (check_fs_status == 0) {
		check_fs_status = GxStrg_GetDeviceCtrl(0, CARD_INSERT);

		if (check_fs_status == 0) {
			return;
		}

		buf_prms.dsp  = CDNN_DSP_SEL;
		buf_prms.net  = CDNN_NET_SEL;
		buf_prms.dram = CDNN_DRAM2;
		buf2.Size = cdnn_get_buf_size(&buf_prms);
		buf2.Addr = CDNN_DRAM2_ADDR;

		init_prms.net             = CDNN_NET_SEL;
		init_prms.filename        = CDNN_NET_FILE;
		init_prms.input2_filename = CDNN_INPUT2_FILE;
		init_prms.width           = CDNN_INPUT_WIDTH;
		init_prms.height          = CDNN_INPUT_HEIGHT;
		init_prms.buf             = buf;
		init_prms.buf2            = &buf2;
		init_prms.cache_buf       = cachebuf;
		init_prms.dsp             = dsp;
		status |= cdnn_init(&init_prms);
		if (status != CDNN_STA_OK) {
			check_fs_status = 0;
			return;
		}
	}

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//IMG_BUF *pISF_buf = NULL;
		//pISF_buf = (IMG_BUF *)&isf_data.Desc[0];
		//DBG_IND("send DSP YUV w=%d h=%d\r\n", pISF_buf->Width, pISF_buf->Height);

#if __CDNN_DUMP_INPUT_IMG__
		{
			static UINT32 frame_cnt = 0;
			char filename[50];

			frame_cnt++;
			if (!(frame_cnt % 100)) {
				snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
				DBG_DUMP("Save YUV file %s\r\n", filename);
				GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
			}
		}
#endif

		DBG_DUMP("######sensor inputting...\r\n");

#if (CDNN_SENSOR_FLOW == 1)

	#if __CDNN_PERF__
		if (Perf_IsOpened() == FALSE) {
			Perf_Open();
		}
		Perf_Mark();
	#endif

		sensor_prms.data = (IMG_BUF *)&isf_data.Desc[0];
		sensor_prms.dsp  = dsp;
		status |= cdnn_proc_sensor(&sensor_prms);

	#if __CDNN_PERF__
		UINT32 tick = Perf_GetDuration();
		Perf_Close();
		DBG_DUMP("CDNN Time [uItron+DSP]: %d us\r\n", tick);
	#endif


		if (status == CDNN_STA_OK) {
	#if (CDNN_NET_SEL != CDNN_FRCNN_RESNET50 && CDNN_NET_SEL != CDNN_MOBILENET_SSD)
			VOID *out = cdnn_get_output(dsp);
			alg_CDNNShowResult((CDNN_OUTPUT_CLASSES *)out);
	#endif
		}

#endif // (CDNN_SENSOR_FLOW == 1)

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}


#if (CDNN_SENSOR_FLOW == 0)
	{
	#if (CDNN_NET_SEL != CDNN_GOOGLENET)
		static UINT8 is_first_frame = 1;
	#endif

		file_prms.filename = g_cdnn_test_file[g_cdnn_test_idx];
		file_prms.dsp  = dsp;
		status |= cdnn_proc_file(&file_prms);

		DBG_DUMP("CDNN Input File: %s\r\n", file_prms.filename);
		if (status == CDNN_STA_OK) {
			VOID *out = cdnn_get_output(dsp);
	#if (CDNN_NET_SEL == CDNN_FRCNN_RESNET50)
			if (is_first_frame)
				alg_CDNNShowResult_FRCNN((CDNN_OUTPUT_FRCNN *)out);
	#elif (CDNN_NET_SEL == CDNN_MOBILENET_SSD)
			if (is_first_frame)
				alg_CDNNShowResult_MOBILENET_SSD((CDNN_OUTPUT_MOBILENET_SSD *)out);
	#else
			alg_CDNNShowResult((CDNN_OUTPUT_CLASSES *)out);
	#endif
		}

	#if (CDNN_NET_SEL != CDNN_GOOGLENET)
		is_first_frame = 0;
	#endif

		g_cdnn_test_idx++;
		if (g_cdnn_test_idx >= (sizeof(g_cdnn_test_file) / CDNN_MAX_FILE_PATH)) {
			g_cdnn_test_idx = 0;
		}
	}
#endif // (CDNN_SENSOR_FLOW == 0)
}

UINT32 alg_CDNNCalcBufSize(VOID)
{
	CDNN_CREATE_PRMS create_prms = {0};
	CDNN_BUF_PRMS buf_prms = {0};

	// creating here because alg_CDNNCalcBufSize() is called before alg_CDNNInit()
	create_prms.dsp       = CDNN_DSP_SEL;
#if (CDNN_SENSOR_FLOW == 0)
	create_prms.input_src = CDNN_FILE_INPUT;
#else
	create_prms.input_src = CDNN_SENSOR_INPUT;
#endif
	create_prms.use_dram2 = CDNN_USE_DRAM2;
	cdnn_create(&create_prms);

	buf_prms.dsp  = CDNN_DSP_SEL;
	buf_prms.net  = CDNN_NET_SEL;
	buf_prms.dram = CDNN_DRAM1;
	return cdnn_get_buf_size(&buf_prms);
}

UINT32 alg_CDNNCalcCacheBufSize(VOID)
{
	CDNN_BUF_PRMS buf_prms = {0};

	buf_prms.dsp  = CDNN_DSP_SEL;
	buf_prms.net  = CDNN_NET_SEL;
	buf_prms.dram = CDNN_DRAM1;
	return cdnn_get_cache_buf_size(&buf_prms);
}

#endif // IPCAM_CDNN_FUNC || MOVIE_CDNN_FUNC
