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

#if IPCAM_CDNN2_FUNC

// Definitions -----------------------------------------------------------------

#define CDNN2_PATH                      "A:\\CNNLib\\"
#define CDNN2_INPUT_PATH                CDNN2_PATH "DI\\CDNN_flow\\"
#define CDNN2_OUTPUT_PATH               CDNN2_PATH "DO\\CDNN_flow\\"
#define CDNN2_NET_PATH                  CDNN2_INPUT_PATH "para\\"
#define CDNN2_IMG_PATH                  CDNN2_PATH "DI\\imgData\\Alexnet\\"

#define CDNN2_GOOGLENET_FILE            CDNN2_NET_PATH "deploy1001_v400f_googlenet.cdnnQdata"
#define CDNN2_FRCNN_RESNET50_FILE       CDNN2_NET_PATH "deploy1001_v400f_frcnn_resnet50.cdnnQdata"
#define CDNN2_FRCNN_INPUT2_FILE         CDNN2_NET_PATH "proposal_2.mat"
#define CDNN2_FRCNN_PROPOSAL_SCORE_FILE CDNN2_OUTPUT_PATH "proposal_score.bin"
#define CDNN2_FRCNN_BBOX_FILE           CDNN2_OUTPUT_PATH "bbox.bin"
#define CDNN2_FRCNN_CLS_FILE            CDNN2_OUTPUT_PATH "cls.bin"
#define CDNN2_FRCNN_PROPOSAL_ROIS_FILE  CDNN2_OUTPUT_PATH "proposal_rois.bin"


#define CDNN2_NET_SEL                   CDNN_GOOGLENET
#define CDNN2_NET_FILE                  CDNN2_GOOGLENET_FILE
#if (CDNN2_NET_SEL == CDNN_FRCNN_RESNET50)
#define CDNN2_INPUT2_FILE               CDNN2_FRCNN_INPUT2_FILE
#else
#define CDNN2_INPUT2_FILE               NULL
#endif

#define CDNN2_DSP_SEL                   CDNN_DSP2
#define CDNN2_SENSOR_FLOW               1 // 1: Sensor mode, 0: File mode


#define __CDNN2_PERF__                  DISABLE
#define __CDNN2_DUMP_INPUT_IMG__        DISABLE

#if IPCAM_CDNN2_FUNC
#define CDNN2_ISF_OUT_PORT              ISF_OUT16
#else
#define CDNN2_ISF_OUT_PORT              ISF_OUT15
#endif

// Variables -------------------------------------------------------------------

#if (CDNN2_SENSOR_FLOW == 0)

#define CDNN2_MAX_FILE_PATH             100

static UINT32 g_cdnn2_test_idx = 0;
static CHAR g_cdnn2_test_file[][CDNN2_MAX_FILE_PATH] = {
#if (CDNN2_NET_SEL == CDNN_FRCNN_RESNET50)
	CDNN2_IMG_PATH "bus.jpg",
#else
	CDNN2_IMG_PATH "dog.jpg",
#endif
#if (CDNN2_NET_SEL != CDNN_FRCNN_RESNET50)
	CDNN2_IMG_PATH "bicycle.jpg",
	CDNN2_IMG_PATH "bird.jpg",
	CDNN2_IMG_PATH "boat.jpg",
	CDNN2_IMG_PATH "car.jpg",
	CDNN2_IMG_PATH "cat.jpg",
	CDNN2_IMG_PATH "girl.jpg",
	CDNN2_IMG_PATH "man.jpg",
	CDNN2_IMG_PATH "peacock.jpg",
	CDNN2_IMG_PATH "sheep.jpg",
	CDNN2_IMG_PATH "train.jpg",
	CDNN2_IMG_PATH "turtle.jpg",
#endif
};

#endif // (CDNN2_SENSOR_FLOW == 0)

// Functions -------------------------------------------------------------------

VOID alg_CDNN2ShowResult(CDNN_OUTPUT_CLASSES *out)
{
	UINT8 idx;

	DBG_DUMP("CDNN2 Results:\r\n");
	for (idx = 0; idx < out->class_num; idx++) {
		DBG_DUMP("no=%d, label=%s, score=%d\r\n", idx + 1, out->classes[idx].label, out->classes[idx].score);
	}
	DBG_DUMP("\r\n");

	return;
}

VOID alg_CDNN2ShowResult_FRCNN(CDNN_OUTPUT_FRCNN *out)
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

	filehdl = FileSys_OpenFile(CDNN2_FRCNN_PROPOSAL_SCORE_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->proposal_score_addr, &proposal_score_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN2_FRCNN_BBOX_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->bbox_addr, &bbox_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN2_FRCNN_CLS_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->cls_addr, &cls_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);

	filehdl = FileSys_OpenFile(CDNN2_FRCNN_PROPOSAL_ROIS_FILE, FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)out->proposal_rois_addr, &proposal_rois_output_size, 0, NULL);
	FileSys_CloseFile(filehdl);
}

void alg_CDNN2Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init after file system is ready in alg_CDNN2Process()
	return;
}

void alg_CDNN2Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	MEM_RANGE           buf2;
	CDNN_DSP            dsp = CDNN2_DSP_SEL;
	CDNN_BUF_PRMS       buf_prms = {0};
	CDNN_INIT_PRMS      init_prms = {0};
	CDNN_STA            status = CDNN_STA_OK;
	ISF_DATA            isf_data;
	ISF_UNIT            *pUnit;
	ISF_PORT            *pSrc;
	static UINT32       check_fs_status = 0;
#if (CDNN2_SENSOR_FLOW == 1)
	CDNN_SENSOR_PRMS    sensor_prms = {0};
#else
	CDNN_FILE_PRMS      file_prms = {0};
#endif

	pUnit = &ISF_UserProc;
#if (MOVIE_CDNN_FUNC && MOVIE_MAPPING_MULTIREC)
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
#else
	pSrc = ImageUnit_Out(pUnit, CDNN2_ISF_OUT_PORT);
#endif

	if (check_fs_status == 0) {
		check_fs_status = GxStrg_GetDeviceCtrl(0, CARD_INSERT);

		if (check_fs_status == 0) {
			return;
		}

		buf_prms.dsp  = CDNN2_DSP_SEL;
		buf_prms.net  = CDNN2_NET_SEL;
		buf_prms.dram = CDNN_DRAM2;
		buf2.Size = cdnn_get_buf_size(&buf_prms);
		buf2.Addr = CDNN2_DRAM2_ADDR;

		init_prms.net             = CDNN2_NET_SEL;
		init_prms.filename        = CDNN2_NET_FILE;
		init_prms.input2_filename = CDNN2_INPUT2_FILE;
		init_prms.width           = CDNN2_INPUT_WIDTH;
		init_prms.height          = CDNN2_INPUT_HEIGHT;
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

#if __CDNN2_DUMP_INPUT_IMG__
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

#if (CDNN2_SENSOR_FLOW == 1)

	#if __CDNN2_PERF__
		if (Perf_IsOpened() == FALSE) {
			Perf_Open();
		}
		Perf_Mark();
	#endif

		sensor_prms.data = (IMG_BUF *)&isf_data.Desc[0];
		sensor_prms.dsp  = dsp;
		status |= cdnn_proc_sensor(&sensor_prms);

	#if __CDNN2_PERF__
		UINT32 tick = Perf_GetDuration();
		Perf_Close();
		DBG_DUMP("CDNN2 Time [uItron+DSP]: %d us\r\n", tick);
	#endif

		if (status == CDNN_STA_OK) {
	#if (CDNN2_NET_SEL != CDNN_FRCNN_RESNET50)
			VOID *out = cdnn_get_output(dsp);
			alg_CDNN2ShowResult((CDNN_OUTPUT_CLASSES *)out);
	#endif
		}

#endif // (CDNN2_SENSOR_FLOW == 1)

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}


#if (CDNN2_SENSOR_FLOW == 0)
	{
	#if (CDNN2_NET_SEL != CDNN_GOOGLENET)
		static UINT8 is_first_frame = 1;
	#endif

		file_prms.filename = g_cdnn2_test_file[g_cdnn2_test_idx];
		file_prms.dsp  = dsp;
		status |= cdnn_proc_file(&file_prms);

		DBG_DUMP("CDNN2 Input File: %s\r\n", file_prms.filename);
		if (status == CDNN_STA_OK) {
			VOID *out = cdnn_get_output(dsp);
	#if (CDNN2_NET_SEL == CDNN_FRCNN_RESNET50)
			if (is_first_frame)
				alg_CDNN2ShowResult_FRCNN((CDNN_OUTPUT_FRCNN *)out);
	#else
			alg_CDNN2ShowResult((CDNN_OUTPUT_CLASSES *)out);
	#endif
		}

	#if (CDNN2_NET_SEL != CDNN_GOOGLENET)
		is_first_frame = 0;
	#endif

		g_cdnn2_test_idx++;
		if (g_cdnn2_test_idx >= (sizeof(g_cdnn2_test_file) / CDNN2_MAX_FILE_PATH)) {
			g_cdnn2_test_idx = 0;
		}
	}
#endif // (CDNN2_SENSOR_FLOW == 0)
}

UINT32 alg_CDNN2CalcBufSize(VOID)
{
	CDNN_CREATE_PRMS create_prms = {0};
	CDNN_BUF_PRMS buf_prms = {0};

	// creating here because alg_CDNN2CalcBufSize() is called before alg_CDNN2Init()
	create_prms.dsp       = CDNN2_DSP_SEL;
#if (CDNN2_SENSOR_FLOW == 0)
	create_prms.input_src = CDNN_FILE_INPUT;
#else
	create_prms.input_src = CDNN_SENSOR_INPUT;
#endif
	create_prms.use_dram2 = CDNN_USE_DRAM2;
	cdnn_create(&create_prms);

	buf_prms.dsp  = CDNN2_DSP_SEL;
	buf_prms.net  = CDNN2_NET_SEL;
	buf_prms.dram = CDNN_DRAM1;
	return cdnn_get_buf_size(&buf_prms);
}

UINT32 alg_CDNN2CalcCacheBufSize(VOID)
{
	CDNN_BUF_PRMS buf_prms = {0};

	buf_prms.dsp  = CDNN2_DSP_SEL;
	buf_prms.net  = CDNN2_NET_SEL;
	buf_prms.dram = CDNN_DRAM1;
	return cdnn_get_cache_buf_size(&buf_prms);
}

#endif // IPCAM_CDNN2_FUNC
