/*
    Display HDMI device object

    @file       dispdev_hdmi.c
    @ingroup    mIDISPHDMITx
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include "kernel.h"
#include "Debug.h"
#include "dispdev_hdmi.h"
#include "hdmitx.h"
#include "pll.h"
#include "top.h"
#include "ide.h"
#include "display.h"
#include "dispdev_ioctrl.h"
#include "Audio.h"

static DISPDEV_IOCTRL   pHdmiIoControl = NULL;

static ER               dispdev_openHDMI(void);
static ER               dispdev_closeHDMI(void);
static void             dispdev_setHdmiIOCTRL(FP pIoCtrlFunc);
static ER               dispdev_getHdmiSize(DISPDEV_GET_PRESIZE *tSize);

static DISPDEV_OBJ DispDev_HdmiObj = { dispdev_openHDMI, dispdev_closeHDMI, NULL, NULL, NULL, dispdev_getHdmiSize, dispdev_setHdmiIOCTRL, {NULL, NULL}};

#define     VIDMAX      (59+10)
const static DISPDEV_HDMI_TIMING  dispVideoTiming[VIDMAX] = {
	/* Video Format timing structure defined in CEA-861-D */
	/*           VID                 HS       VS       HVLD      VVLD      CLK       DE  PROGRESSIVE HSYNC  HTOT      HSTART       HEND            VSYNC    VTOT       VOSTART     VOEND       VESTART       VEEND      WINX,   WINY */
	/* 1*/{HDMI_640X480P60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     96, (160 + 640), (96 + 48), (96 + 48 + 640),        2, (480 + 45), (2 + 33), (2 + 33 + 480), (2 + 33), (2 + 33 + 480),   16,     12   },
	/* 2*/{HDMI_720X480P60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138 + 720), (62 + 60), (62 + 60 + 720),        6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   20,     12   },
	/* 3*/{HDMI_720X480P60_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138 + 720), (62 + 60), (62 + 60 + 720),        6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   20,     12   },
	/* 4*/{HDMI_1280X720P60,      ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     40, (370 + 1280), (40 + 220), (40 + 220 + 1280),      5, (720 + 30), (5 + 20), (5 + 20 + 720), (5 + 20), (5 + 20 + 720),   30,     20   },
	/* 5*/{HDMI_1920X1080I60,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE,    44, (280 + 1920), (44 + 148), (44 + 148 + 1920),     10, (1080 + 45), (10 + 15 * 2), (10 + 30 + 1080), (10 + 30 + 1), (10 + 30 + 1080 + 1),   45,     30   },
	/* 6*/{HDMI_720X480I60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 6, (480 + 45), (6 + 15 * 2), (6 + 30 + 480), (6 + 15 * 2 + 1), (6 + 30 + 480 + 1),   16,     12   },
	/* 7*/{HDMI_720X480I60_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 6, (480 + 45), (6 + 15 * 2), (6 + 30 + 480), (6 + 15 * 2 + 1), (6 + 30 + 480 + 1),   16,     12   },
	/* 8*/{HDMI_720X240P60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 3, (240 + 22), (3 + 15), (3 + 15 + 240), (3 + 15), (3 + 15 + 240),   16,      6   },
	/* 9*/{HDMI_720X240P60_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 3, (240 + 22), (3 + 15), (3 + 15 + 240), (3 + 15), (3 + 15 + 240),   16,      6   },
	/*10*/{HDMI_1440X480I60,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 248 / 2, (552 + 2880) / 2, (248 + 228) / 2, (248 + 228 + 2880) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30 + 1), (6 + 30 + 480 + 1),   32,     12   },
	/*11*/{HDMI_1440X480I60_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 248 / 2, (552 + 2880) / 2, (248 + 228) / 2, (248 + 228 + 2880) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30 + 1), (6 + 30 + 480 + 1),   32,     12   },
	/*12*/{HDMI_1440X240P60,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248 / 2, (552 + 2880) / 2, (248 + 228) / 2, (248 + 228 + 2880) / 2, 3, (240 + 22), (3 + 15), (3 + 15 + 240), (3 + 15), (3 + 15 + 240),   32,      6   },
	/*13*/{HDMI_1440X240P60_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248 / 2, (552 + 2880) / 2, (248 + 228) / 2, (248 + 228 + 2880) / 2, 3, (240 + 22), (3 + 15), (3 + 15 + 240), (3 + 15), (3 + 15 + 240),   32,      6   },
	/*14*/{HDMI_1440X480P60,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    124, (276 + 1440), (124 + 120), (124 + 120 + 1440),     6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   32,     12   },
	/*15*/{HDMI_1440X480P60_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    124, (276 + 1440), (124 + 120), (124 + 120 + 1440),     6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   32,     12   },
	/*16*/{HDMI_1920X1080P60,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     44, (280 + 1920), (44 + 148), (44 + 148 + 1920),      5, (1080 + 45), (5 + 36), (5 + 36 + 1080), (5 + 36), (5 + 36 + 1080),   45,     30   },
	/*17*/{HDMI_720X576P50,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144 + 720), (64 + 68), (64 + 68 + 720),        5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   20,     16   },
	/*18*/{HDMI_720X576P50_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144 + 720), (64 + 68), (64 + 68 + 720),        5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   20,     16   },
	/*19*/{HDMI_1280X720P50,      ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     40, (700 + 1280), (40 + 220), (40 + 220 + 1280),      5, (720 + 30), (5 + 20), (5 + 20 + 720), (5 + 20), (5 + 20 + 720),   30,     20   },
	/*20*/{HDMI_1920X1080I50,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE,    44, (720 + 1920), (44 + 148), (44 + 148 + 1920),     10, (1080 + 45), (10 + 15 * 2), (10 + 30 + 1080), (10 + 30 + 1), (10 + 30 + 1080 + 1),   45,     30   },
	/*21*/{HDMI_720X576I50,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1),   16,     16   },
	/*22*/{HDMI_720X576I50_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1),   16,     16   },
	/* Warning: VID 23/24 's V-BP in CEA-861D is 18 lines but in Astro VA-1809 is 19 lines. */
	/*23*/{HDMI_720X288P50,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 3, (288 + 24), (3 + 19), (3 + 19 + 288), (3 + 19), (3 + 19 + 288),   16,      8   },
	/*24*/{HDMI_720X288P50_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 3, (288 + 24), (3 + 19), (3 + 19 + 288), (3 + 19), (3 + 19 + 288),   16,      8   },
	/*25*/{HDMI_1440X576I50,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 252 / 2, (576 + 2880) / 2, (252 + 276) / 2, (252 + 276 + 2880) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1),   32,     16   },
	/*26*/{HDMI_1440X576I50_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 252 / 2, (576 + 2880) / 2, (252 + 276) / 2, (252 + 276 + 2880) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1),   32,     16   },
	/*27*/{HDMI_1440X288P50,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  252 / 2, (576 + 2880) / 2, (252 + 276) / 2, (252 + 276 + 2880) / 2, 3, (288 + 24), (3 + 19), (3 + 19 + 288), (3 + 19), (3 + 19 + 288),   32,      8   },
	/*28*/{HDMI_1440X288P50_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  252 / 2, (576 + 2880) / 2, (252 + 276) / 2, (252 + 276 + 2880) / 2, 3, (288 + 24), (3 + 19), (3 + 19 + 288), (3 + 19), (3 + 19 + 288),   32,      8   },
	/*29*/{HDMI_1440X576P50,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    128, (288 + 1440), (128 + 136), (128 + 136 + 1440),     5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   32,     16   },
	/*30*/{HDMI_1440X576P50_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    128, (288 + 1440), (128 + 136), (128 + 136 + 1440),     5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   32,     16   },
	/*31*/{HDMI_1920X1080P50,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     44, (720 + 1920), (44 + 148), (44 + 148 + 1920),      5, (1080 + 45), (5 + 36), (5 + 36 + 1080), (5 + 36), (5 + 36 + 1080),   45,     30   },
	/*32*/{HDMI_1920X1080P24,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     44, (830 + 1920), (44 + 148), (44 + 148 + 1920),      5, (1080 + 45), (5 + 36), (5 + 36 + 1080), (5 + 36), (5 + 36 + 1080),   45,     30   },
	/*33*/{HDMI_1920X1080P25,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     44, (720 + 1920), (44 + 148), (44 + 148 + 1920),      5, (1080 + 45), (5 + 36), (5 + 36 + 1080), (5 + 36), (5 + 36 + 1080),   45,     30   },
	/*34*/{HDMI_1920X1080P30,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     44, (280 + 1920), (44 + 148), (44 + 148 + 1920),      5, (1080 + 45), (5 + 36), (5 + 36 + 1080), (5 + 36), (5 + 36 + 1080),   45,     30   },
	//35~38 Width is 2880  IDE Un-Supported
	/*35*/{35,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248 / 2, (552 + 2880) / 2, (248 + 240) / 2, (248 + 240 + 2880) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   64,     12   },
	/*36*/{36,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248 / 2, (552 + 2880) / 2, (248 + 240) / 2, (248 + 240 + 2880) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   64,     12   },
	/*37*/{37,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  256 / 2, (576 + 2880) / 2, (256 + 272) / 2, (256 + 272 + 2880) / 2, 5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   64,     16   },
	/*38*/{38,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  256 / 2, (576 + 2880) / 2, (256 + 272) / 2, (256 + 272 + 2880) / 2, 5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   64,     16   },
	/*39*/{HDMI_1920X1080I50_VT1250, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE,   168, (384 + 1920), (168 + 184), (168 + 184 + 1920),    10, (1080 + 170), (10 + 57 * 2), (10 + 114 + 1080), (10 + 57 * 2 + 1), (10 + 114 + 1080 + 1), 45,     30   },
	/*40*/{HDMI_1920X1080I100,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE,    44, (720 + 1920), (44 + 148), (44 + 148 + 1920),     10, (1080 + 45), (10 + 15 * 2), (10 + 30 + 1080), (10 + 30 + 1), (10 + 30 + 1080 + 1),  45,     30   },
	/*41*/{HDMI_1280X720P100,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     40, (700 + 1280), (40 + 220), (40 + 220 + 1280),      5, (720 + 30), (5 + 20), (5 + 20 + 720), (5 + 20), (5 + 20 + 720),   32,     20   },
	/*42*/{HDMI_720X576P100,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144 + 720), (64 + 68), (64 + 68 + 720),        5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   20,     16   },
	/*43*/{HDMI_720X576P100_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144 + 720), (64 + 68), (64 + 68 + 720),        5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   20,     16   },
	/*44*/{HDMI_720X576I100,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1), 16,     16   },
	/*45*/{HDMI_720X576I100_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1), 16,     16   },
	/*46*/{HDMI_1920X1080I120,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE,    44, (280 + 1920), (44 + 148), (44 + 148 + 1920),     10, (1080 + 45), (10 + 15 * 2), (10 + 30 + 1080), (10 + 30 + 1), (10 + 30 + 1080 + 1),  45,     30   },
	/*47*/{HDMI_1280X720P120,     ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     40, (370 + 1280), (40 + 220), (40 + 220 + 1280),      5, (720 + 30), (5 + 20), (5 + 20 + 720), (5 + 20), (5 + 20 + 720),   30,     20   },
	/*48*/{HDMI_720X480P120,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138 + 720), (62 + 60), (62 + 60 + 720),        6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   20,     12   },
	/*49*/{HDMI_720X480P120_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138 + 720), (62 + 60), (62 + 60 + 720),        6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   20,     12   },
	/*50*/{HDMI_720X480I120,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30 + 1), (6 + 30 + 480 + 1),   16,     12   },
	/*51*/{HDMI_720X480I120_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30 + 1), (6 + 30 + 480 + 1),   16,     12   },
	/*52*/{HDMI_720X576P200,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144 + 720), (64 + 68), (64 + 68 + 720),        5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   20,     16   },
	/*53*/{HDMI_720X576P200_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144 + 720), (64 + 68), (64 + 68 + 720),        5, (576 + 49), (5 + 39), (5 + 39 + 576), (5 + 39), (5 + 39 + 576),   20,     16   },
	/*54*/{HDMI_720X576I200,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1), 16,     16   },
	/*55*/{HDMI_720X576I200_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126 / 2, (288 + 1440) / 2, (126 + 138) / 2, (126 + 138 + 1440) / 2, 6, (576 + 49), (6 + 19 * 2), (6 + 38 + 576), (6 + 38 + 1), (6 + 38 + 576 + 1), 16,     16   },
	/*56*/{HDMI_720X480P240,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138 + 720), (62 + 60), (62 + 60 + 720),        6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   20,     12   },
	/*57*/{HDMI_720X480P240_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138 + 720), (62 + 60), (62 + 60 + 720),        6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30), (6 + 30 + 480),   20,     12   },
	/*58*/{HDMI_720X480I240,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30 + 1), (6 + 30 + 480 + 1),   16,     12   },
	/*59*/{HDMI_720X480I240_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124 / 2, (276 + 1440) / 2, (124 + 114) / 2, (124 + 114 + 1440) / 2, 6, (480 + 45), (6 + 30), (6 + 30 + 480), (6 + 30 + 1), (6 + 30 + 480 + 1),   16,     12   },

	/*93*/ {HDMI_3840X2160P24,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1660 + 3840), (88 + 296), (88 + 296 + 3840),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
	/*94*/ {HDMI_3840X2160P25,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1440 + 3840), (88 + 296), (88 + 296 + 3840),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
	/*95*/ {HDMI_3840X2160P30,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (560 + 3840), (88 + 296), (88 + 296 + 3840),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
	/*98*/ {HDMI_4096X2160P24,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1404 + 4096), (88 + 296), (88 + 296 + 4096),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
	/*99*/ {HDMI_4096X2160P25,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1184 + 4096), (88 + 128), (88 + 128 + 4096),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
#if !_FPGA_EMULATION_
	/*100*/{HDMI_4096X2160P30,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (304 + 4096), (88 + 128), (88 + 128 + 4096),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
#else
// This is fpga test size to output width 4096 to SC monitor. because VA1809 would be clamped to 4095.
	/*100*/{HDMI_4096X2160P30,    ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (304 + 4096), (88 + 128), (88 + 128 + 4096),      5, (272 + 15), (5 + 5), (5 + 5 + 272), (5 + 5), (5 + 5 + 272), 90,     8   },
#endif

 /*128+95*/{HDMI14B_3840X2160P30, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (560 + 3840), (88 + 296), (88 + 296 + 3840),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
 /*128+94*/{HDMI14B_3840X2160P25, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1440 + 3840),(88 + 296), (88 + 296 + 3840),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
 /*128+93*/{HDMI14B_3840X2160P24, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1660 + 3840),(88 + 296), (88 + 296 + 3840),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   },
 /*128+98*/{HDMI14B_4096X2160P24, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     88, (1404 + 4096),(88 + 296), (88 + 296 + 4096),      10, (2160 + 90), (10 + 72), (10 + 72 + 2160), (10 + 72), (10 + 72 + 2160), 90,     60   }

	/*            VID                 HS       VS       HVLD      VVLD      CLK       DE  PROGRESSIVE HSYNC  HTOT      HSTART       HEND            VSYNC    VTOT       VOSTART     VOEND       VESTART       VEEND      WINX,   WINY */
};

#if !_FPGA_EMULATION_
static UINT32           gDispDev_SrcFreq = 0;
#endif

static HDMI_MODE        gDispDev_HdmiMode;
static HDMI_VIDEOID     gDispDev_HdmiVid = HDMI_720X480P60;
static HDMI_AUDIOFMT    gDispDev_AudFmt  = HDMI_AUDIO48KHZ;

static UINT32           gDispDev_WinHoziOfs;
static UINT32           gDispDev_WinVertOfs;
static UINT32           gDispDev_HdmiIndex;

static BOOL dispdev_initHdmiVidFormat(HDMI_VIDEOID VID_Code)
{
	UINT8                   i, Valid = 0;
	DISPDEV_IOCTRL_PARAM    DevIoCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;

	disphdmi_debug(("DEVHDMI: initHdmiVidFormat (%d)\r\n", VID_Code));

	//
	//Search the mapping Video ID index
	//
	for (i = 0; i < VIDMAX; i++) {
		if (dispVideoTiming[i].VID == VID_Code) {
			gDispDev_HdmiIndex   = i;
			Valid           = 1;
			break;
		}
	}

	if (Valid == 0) {
		DBG_ERR("No support Video Format!\r\n");
		return FALSE;
	}


	//
	//  Handle the TV overscan/underscan capability.
	//
	if ((!hdmitx_getConfig(HDMI_CONFIG_ID_FORCE_UNDERSCAN)) && ((hdmitx_getEdidInfo()&HDMITX_TV_UNDERSCAN) == 0x0) && (hdmitx_getEdidInfo() != 0x0)) {
		// Overscan
		DevIoCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX = dispVideoTiming[gDispDev_HdmiIndex].WINX_OFS;
		DevIoCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY = dispVideoTiming[gDispDev_HdmiIndex].WINY_OFS;

		gDispDev_WinHoziOfs = dispVideoTiming[gDispDev_HdmiIndex].WINX_OFS << 1;
		gDispDev_WinVertOfs = dispVideoTiming[gDispDev_HdmiIndex].WINY_OFS << 1;
	} else {
		// underscan or EDID Parsing fail
		DevIoCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX = 0;
		DevIoCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY = 0;

		gDispDev_WinHoziOfs = 0x0;
		gDispDev_WinVertOfs = 0x0;
	}
	pDispDevControl(DISPDEV_IOCTRL_SET_CONST_OFS, &DevIoCtrl);


	if ((!hdmitx_getConfig(HDMI_CONFIG_ID_FORCE_RGB)) && (hdmitx_getEdidInfo()&HDMITX_TV_YCBCR444)) {
		gDispDev_HdmiMode = HDMI_MODE_YCBCR444;
	} else if ((!hdmitx_getConfig(HDMI_CONFIG_ID_FORCE_RGB)) && (hdmitx_getEdidInfo()&HDMITX_TV_YCBCR422)) {
		gDispDev_HdmiMode = HDMI_MODE_YCBCR422;
	} else {
		gDispDev_HdmiMode = HDMI_MODE_RGB444;
	}

	return TRUE;
}


static void dispdev_initHdmiDisplay(void)
{
	DISPDEV_IOCTRL_PARAM    DevIoCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;

	disphdmi_debug(("DEVHDMI: initHdmiDisplay\r\n"));

	DevIoCtrl.SEL.SET_DITHER_EN.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_DITHER_EN, &DevIoCtrl);

	if (gDispDev_HdmiMode == HDMI_MODE_YCBCR422) {
		/* 16bits */
		DevIoCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_INF_HDMI_16BIT;
	} else {
		/* 24bits */
		DevIoCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_EMBD_HDMI;
	}
	pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIoCtrl);

#if 0//_FPGA_EMULATION_
	hdmitx_setPg1(0, dispVideoTiming[gDispDev_HdmiIndex].H_TOTAL);
	hdmitx_setPg2(dispVideoTiming[gDispDev_HdmiIndex].H_SYNCTIME, dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME);
	hdmitx_setPg3((dispVideoTiming[gDispDev_HdmiIndex].H_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME), (dispVideoTiming[gDispDev_HdmiIndex].V_TOTAL));
	hdmitx_setPg4((dispVideoTiming[gDispDev_HdmiIndex].V_SYNCTIME), dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME);
	hdmitx_setPg5((dispVideoTiming[gDispDev_HdmiIndex].V_ODD_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME));
	hdmitx_setPgPattern(0x0A);
#endif

#if 0
	DevIoCtrl.SEL.SET_SYNC_INVERT.bHsInv  = dispVideoTiming[gDispDev_HdmiIndex].HS;
	DevIoCtrl.SEL.SET_SYNC_INVERT.bVsInv  = dispVideoTiming[gDispDev_HdmiIndex].VS;
	DevIoCtrl.SEL.SET_SYNC_INVERT.bClkInv = dispVideoTiming[gDispDev_HdmiIndex].CLK;
	pDispDevControl(DISPDEV_IOCTRL_SET_SYNC_INVERT, &DevIoCtrl);
#else
	hdmitx_setConfig(HDMI_CONFIG_ID_VSYNC_INVERT, !dispVideoTiming[gDispDev_HdmiIndex].VS);
	hdmitx_setConfig(HDMI_CONFIG_ID_HSYNC_INVERT, !dispVideoTiming[gDispDev_HdmiIndex].HS);
#endif

	DevIoCtrl.SEL.SET_VLD_INVERT.bHvldInv    = dispVideoTiming[gDispDev_HdmiIndex].HVLD;
	DevIoCtrl.SEL.SET_VLD_INVERT.bVvldInv    = dispVideoTiming[gDispDev_HdmiIndex].VVLD;
	DevIoCtrl.SEL.SET_VLD_INVERT.bFieldInv   = FALSE;
	DevIoCtrl.SEL.SET_VLD_INVERT.bDeInv      = dispVideoTiming[gDispDev_HdmiIndex].DE;
	pDispDevControl(DISPDEV_IOCTRL_SET_VLD_INVERT, &DevIoCtrl);

	DevIoCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = dispVideoTiming[gDispDev_HdmiIndex].H_SYNCTIME - 1;
	DevIoCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = dispVideoTiming[gDispDev_HdmiIndex].H_TOTAL - 1;
	DevIoCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME;
	DevIoCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = dispVideoTiming[gDispDev_HdmiIndex].H_ENDTIME - 1;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIoCtrl);

	DevIoCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = dispVideoTiming[gDispDev_HdmiIndex].V_SYNCTIME - 1;
	DevIoCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = dispVideoTiming[gDispDev_HdmiIndex].V_TOTAL - 1;
	DevIoCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME;
	DevIoCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = dispVideoTiming[gDispDev_HdmiIndex].V_ODD_ENDTIME - 1;
	DevIoCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart = dispVideoTiming[gDispDev_HdmiIndex].V_EVEN_STARTTIME;
	DevIoCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = dispVideoTiming[gDispDev_HdmiIndex].V_EVEN_ENDTIME - 1;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIoCtrl);

	DevIoCtrl.SEL.SET_CSB_EN.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CSB_EN, &DevIoCtrl);

	DevIoCtrl.SEL.SET_YC_EXCHG.bCbCrExchg = FALSE;
	DevIoCtrl.SEL.SET_YC_EXCHG.bYCExchg = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_YC_EXCHG, &DevIoCtrl);

	DevIoCtrl.SEL.SET_SUBPIXEL.bOddR     = FALSE;
	DevIoCtrl.SEL.SET_SUBPIXEL.bOddG     = FALSE;
	DevIoCtrl.SEL.SET_SUBPIXEL.bOddB     = FALSE;
	DevIoCtrl.SEL.SET_SUBPIXEL.bEvenR    = FALSE;
	DevIoCtrl.SEL.SET_SUBPIXEL.bEvenG    = FALSE;
	DevIoCtrl.SEL.SET_SUBPIXEL.bEvenB    = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_SUBPIXEL, &DevIoCtrl);

	DevIoCtrl.SEL.SET_CLK1_2.bClk1_2     = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK1_2, &DevIoCtrl);

	ide_setTvPowerDown(TRUE);

	if (gDispDev_HdmiMode == HDMI_MODE_RGB444) { /* output=RGB for 444 mode */
		/* Output=RGB for 444 mode */
		DevIoCtrl.SEL.SET_ICST_EN.bEn    = TRUE;
		DevIoCtrl.SEL.SET_ICST_EN.Select = CST_YCBCR2RGB;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIoCtrl);

		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT0, 0);
		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT1, 1);
		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT2, 2);
	} else if (gDispDev_HdmiMode == HDMI_MODE_YCBCR444) {
		/* output=YCbCr for 444 mode */
		DevIoCtrl.SEL.SET_ICST_EN.bEn    = FALSE;
		DevIoCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIoCtrl);

		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT0, 1);
		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT1, 2);
		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT2, 0);
	} else {
		/* output=YCbCr for 422 mode */
		DevIoCtrl.SEL.SET_ICST_EN.bEn = FALSE;
		DevIoCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIoCtrl);

		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT0, 0);
		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT1, 2);
		hdmitx_setConfig(HDMI_CONFIG_ID_COMPONENT2, 1);
	}

	DevIoCtrl.SEL.SET_WINDOW_OUT_TYPE.bInterlaced = !(dispVideoTiming[gDispDev_HdmiIndex].PROGRESSIVE);
	DevIoCtrl.SEL.SET_WINDOW_OUT_TYPE.bFieldOddSt = TRUE;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE, &DevIoCtrl);

	DevIoCtrl.SEL.SET_OUT_LIMIT.uiYLow   = 0x00;
	DevIoCtrl.SEL.SET_OUT_LIMIT.uiYUp    = 0xFF;
	DevIoCtrl.SEL.SET_OUT_LIMIT.uiCbLow  = 0x00;
	DevIoCtrl.SEL.SET_OUT_LIMIT.uiCbUp   = 0xFF;
	DevIoCtrl.SEL.SET_OUT_LIMIT.uiCrLow  = 0x00;
	DevIoCtrl.SEL.SET_OUT_LIMIT.uiCrUp   = 0xFF;
	pDispDevControl(DISPDEV_IOCTRL_SET_OUT_LIMIT, &DevIoCtrl);

}

#if !_FPGA_EMULATION_
static void dispdev_disableIDE(void)
{
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	pDispDevControl(DISPDEV_IOCTRL_GET_ENABLE, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_ENABLE.bEn == TRUE) {
		disphdmi_debug(("DEVHDMI: dispdev_disableIDE\r\n"));

		DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
		pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE,    &DevIOCtrl);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

		// Close IDE clock
		DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
		pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
	}
}
#endif

static BOOL dispdev_chgHdmiClkFreq(BOOL bNative)
{
	UINT32                  uiTargetFreq = 27000000;
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;
	DISPDEV_IOCTRL_PARAM    DevIoCtrl;
#if !_FPGA_EMULATION_
	UINT32                  CurFreq;
	PLL_ID                  SrcPLL;
	disphdmi_debug(("DEVHDMI: chgHdmiClkFreq (%d)\r\n", bNative));

	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIoCtrl);

	switch (DevIoCtrl.SEL.GET_SRCCLK.SrcClk) {
	case DISPCTRL_SRCCLK_PLL1:
	case DISPCTRL_SRCCLK_IDE2_PLL1: {
			SrcPLL =                                    PLL_ID_1;
		}
		break;

	case DISPCTRL_SRCCLK_PLL2:
	case DISPCTRL_SRCCLK_IDE2_PLL2: {
			SrcPLL =                                    PLL_ID_2;
		}
		break;

	case DISPCTRL_SRCCLK_PLL4:
	case DISPCTRL_SRCCLK_IDE2_PLL4: {
			SrcPLL =                                    PLL_ID_4;
		}
		break;

	case DISPCTRL_SRCCLK_PLL14:
	case DISPCTRL_SRCCLK_IDE2_PLL14: {
			SrcPLL =                                    PLL_ID_14;
		}
		break;

	default: {
			return TRUE;
		}

	}

	CurFreq              = pll_getPLLFreq(SrcPLL) / 1000000;
	disphdmi_debug(("DEVHDMI: Original SrcClk(%d)Mhz\r\n", CurFreq));

	if (bNative) {
		gDispDev_SrcFreq = CurFreq;
	}

	// Calculate PLL Ratio
	CurFreq = CurFreq * 131072 / 12;

	switch (gDispDev_HdmiVid) {

	/*
	    Change PLL2 to 216Mhz
	*/
	// Target 54MHz
	case HDMI_1440X480P60:          /* VID14 */
	case HDMI_1440X480P60_16X9:     /* VID15 */
	case HDMI_1440X576P50:          /* VID29 */
	case HDMI_1440X576P50_16X9:     /* VID30 */
	case HDMI_720X576P100:          /* VID42 */
	case HDMI_720X576P100_16X9:     /* VID43 */
	case HDMI_720X480P120:          /* VID48 */
	case HDMI_720X480P120_16X9:     /* VID49 */
	case HDMI_720X576I200:          /* VID54 */
	case HDMI_720X576I200_16X9:     /* VID55 */
	case HDMI_720X480I240:          /* VID58 */
	case HDMI_720X480I240_16X9:     /* VID59 */
	// Target 108MHz
	case HDMI_720X576P200:          /* VID52 */
	case HDMI_720X576P200_16X9:     /* VID53 */
	case HDMI_720X480P240:          /* VID56 */
	case HDMI_720X480P240_16X9: {   /* VID57 */
			if ((CurFreq != HDMI_PLL216) && (SrcPLL != PLL_ID_1)) {
				dispdev_disableIDE();

				pll_setPLLEn(SrcPLL,    FALSE);
				pll_setPLL(SrcPLL,      HDMI_PLL216);
				pll_setPLLEn(SrcPLL,    TRUE);
				disphdmi_debug(("DEVHDMI: Chg PLL%d to(%d)Mhz\r\n", (SrcPLL + 1), pll_getPLLFreq(SrcPLL) / 1000000));
			}
		}
		break;

	/*
	    Change PLL2 to 252Mhz
	*/
	// Target 25.2MHz
	case HDMI_640X480P60: {         /* VID1 */
			if ((CurFreq != HDMI_PLL252) && (SrcPLL != PLL_ID_1)) {
				dispdev_disableIDE();

				pll_setPLLEn(SrcPLL,    FALSE);
				pll_setPLL(SrcPLL,      HDMI_PLL252);
				pll_setPLLEn(SrcPLL,    TRUE);
				disphdmi_debug(("DEVHDMI: Chg PLL%d to(%d)Mhz\r\n", (SrcPLL + 1), pll_getPLLFreq(SrcPLL) / 1000000));
			}
		}
		break;

	/*
	    Change PLL2 to 297Mhz
	*/
	// Target 13.5MHz
	case HDMI_720X480I60:           /* VID6 */
	case HDMI_720X480I60_16X9:      /* VID7 */
	case HDMI_720X240P60:           /* VID8 */
	case HDMI_720X240P60_16X9:      /* VID9 */
	case HDMI_720X576I50:           /* VID21 */
	case HDMI_720X576I50_16X9:      /* VID22 */
	case HDMI_720X288P50:           /* VID23 */
	case HDMI_720X288P50_16X9:      /* VID24 */
	// Target 27MHz
	case HDMI_720X480P60:           /* VID2 */
	case HDMI_720X480P60_16X9:      /* VID3 */
	case HDMI_1440X480I60:          /* VID10 */
	case HDMI_1440X480I60_16X9:     /* VID11 */
	case HDMI_1440X240P60:          /* VID12 */
	case HDMI_1440X240P60_16X9:     /* VID13 */
	case HDMI_720X576P50:           /* VID17 */
	case HDMI_720X576P50_16X9:      /* VID18 */
	case HDMI_1440X576I50:          /* VID25 */
	case HDMI_1440X576I50_16X9:     /* VID26 */
	case HDMI_1440X288P50:          /* VID27 */
	case HDMI_1440X288P50_16X9:     /* VID28 */
	case HDMI_720X576I100:          /* VID44 */
	case HDMI_720X576I100_16X9:     /* VID45 */
	case HDMI_720X480I120:          /* VID50 */
	case HDMI_720X480I120_16X9:     /* VID51 */
	// Target 74.25MHz
	case HDMI_1280X720P60:          /* VID4 */
	case HDMI_1920X1080I60:         /* VID5 */
	case HDMI_1280X720P50:          /* VID19 */
	case HDMI_1920X1080I50:         /* VID20 */
	case HDMI_1920X1080P24:         /* VID32 */
	case HDMI_1920X1080P25:         /* VID33 */
	case HDMI_1920X1080P30:         /* VID34 */
	// Target 148.5MHz
	case HDMI_1920X1080P60:         /* VID16 */
	case HDMI_1920X1080P50:         /* VID31 */
	case HDMI_1920X1080I100:        /* VID40 */
	case HDMI_1280X720P100:         /* VID41 */
	case HDMI_1920X1080I120:        /* VID46 */
	case HDMI_1280X720P120:         /* VID47 */
	// Target 297MHz
	case HDMI_3840X2160P24:         /* VID93 */
	case HDMI_3840X2160P25:         /* VID94 */
	case HDMI_3840X2160P30:         /* VID95 */
	case HDMI_4096X2160P24:         /* VID98 */
	case HDMI14B_3840X2160P24:
	case HDMI14B_3840X2160P25:
	case HDMI14B_3840X2160P30:
	case HDMI14B_4096X2160P24:
	case HDMI_4096X2160P25:         /* VID99 */
	case HDMI_4096X2160P30: {       /* VID100*/
			if ((CurFreq != HDMI_PLL297) && (SrcPLL != PLL_ID_1)) {
				dispdev_disableIDE();

				pll_setPLLEn(SrcPLL,    FALSE);
				pll_setPLL(SrcPLL,      HDMI_PLL297);
				pll_setPLLEn(SrcPLL,    TRUE);
				disphdmi_debug(("DEVHDMI: Chg PLL%d to(%d)Mhz\r\n", (SrcPLL + 1), pll_getPLLFreq(SrcPLL) / 1000000));
			}
		}
		break;

	case HDMI_1920X1080I50_VT1250:  /* VID39 */
	default:
		DBG_ERR("No support HDMI mode(%d)\r\n",gDispDev_HdmiVid);
		return TRUE;
	}


#endif

	//
	//  Configure IDE Clock frequency according to HDMI Video ID
	//
	switch (gDispDev_HdmiVid) {

	/* Target frequency 13.5Mhz */
	case HDMI_720X480I60:          /* VID6 */
	case HDMI_720X480I60_16X9:     /* VID7 */
	case HDMI_720X240P60:          /* VID8 */
	case HDMI_720X240P60_16X9:     /* VID9 */
	case HDMI_720X576I50:          /* VID21 */
	case HDMI_720X576I50_16X9:     /* VID22 */
	case HDMI_720X288P50:          /* VID23 */
	case HDMI_720X288P50_16X9: {   /* VID24 */
			uiTargetFreq = 13500000;
		}
		break;

	/* Target frequency 25.2Mhz */
	case HDMI_640X480P60: {         /* VID1 */
#if _FPGA_EMULATION_
			uiTargetFreq = 27000000;
#else
			uiTargetFreq = 25200000;
#endif
		}
		break;

	/* Target frequency 27Mhz */
	case HDMI_720X480P60:           /* VID2 */
	case HDMI_720X480P60_16X9:      /* VID3 */
	case HDMI_1440X480I60:          /* VID10 */
	case HDMI_1440X480I60_16X9:     /* VID11 */
	case HDMI_1440X240P60:          /* VID12 */
	case HDMI_1440X240P60_16X9:     /* VID13 */
	case HDMI_720X576P50:           /* VID17 */
	case HDMI_720X576P50_16X9:      /* VID18 */
	case HDMI_1440X576I50:          /* VID25 */
	case HDMI_1440X576I50_16X9:     /* VID26 */
	case HDMI_1440X288P50:          /* VID27 */
	case HDMI_1440X288P50_16X9:     /* VID28 */
	case HDMI_720X576I100:          /* VID44 */
	case HDMI_720X576I100_16X9:     /* VID45 */
	case HDMI_720X480I120:          /* VID50 */
	case HDMI_720X480I120_16X9: {   /* VID51 */
			uiTargetFreq = 27000000;
		}
		break;

	/* Target frequency 54Mhz */
	case HDMI_1440X480P60:          /* VID14 */
	case HDMI_1440X480P60_16X9:     /* VID15 */
	case HDMI_1440X576P50:          /* VID29 */
	case HDMI_1440X576P50_16X9:     /* VID30 */
	case HDMI_720X576P100:          /* VID42 */
	case HDMI_720X576P100_16X9:     /* VID43 */
	case HDMI_720X480P120:          /* VID48 */
	case HDMI_720X480P120_16X9:     /* VID49 */
	case HDMI_720X576I200:          /* VID54 */
	case HDMI_720X576I200_16X9:     /* VID55 */
	case HDMI_720X480I240:          /* VID58 */
	case HDMI_720X480I240_16X9: {   /* VID59 */
			uiTargetFreq = 54000000;
		}
		break;

	/* Target frequency 74.25Mhz */
	case HDMI_1280X720P60:          /* VID4 */
	case HDMI_1920X1080I60:         /* VID5 */
	case HDMI_1280X720P50:          /* VID19 */
	case HDMI_1920X1080I50:         /* VID20 */
	case HDMI_1920X1080P24:         /* VID32 */
	case HDMI_1920X1080P25:         /* VID33 */
	case HDMI_1920X1080P30: {       /* VID34 */
			uiTargetFreq = 74250000;
		}
		break;

	/* Target frequency 108Mhz */
	case HDMI_720X576P200:          /* VID52 */
	case HDMI_720X576P200_16X9:     /* VID53 */
	case HDMI_720X480P240:          /* VID56 */
	case HDMI_720X480P240_16X9: {   /* VID57 */
			uiTargetFreq = 108000000;
		}
		break;

	/* Target frequency 148.5Mhz */
	case HDMI_1920X1080P60:         /* VID16 */
	case HDMI_1920X1080P50:         /* VID31 */
	case HDMI_1920X1080I100:        /* VID40 */
	case HDMI_1280X720P100:         /* VID41 */
	case HDMI_1920X1080I120:        /* VID46 */
	case HDMI_1280X720P120: {       /* VID47 */
			uiTargetFreq = 148500000;
		}
		break;

	case HDMI_3840X2160P24:         /* VID93 */
	case HDMI_3840X2160P25:         /* VID94 */
	case HDMI_3840X2160P30:         /* VID95 */
	case HDMI_4096X2160P24:         /* VID98 */
	case HDMI14B_3840X2160P24:
	case HDMI14B_3840X2160P25:
	case HDMI14B_3840X2160P30:
	case HDMI14B_4096X2160P24:
	case HDMI_4096X2160P25:         /* VID99 */
	case HDMI_4096X2160P30: {       /* VID100*/
			uiTargetFreq = 297000000;
		}
		break;

	default: {
			return TRUE;
		}

	}

	// set clock rate
	DevIoCtrl.SEL.SET_CLK_FREQ.uiFreq   = uiTargetFreq;
#if _FPGA_EMULATION_
	DevIoCtrl.SEL.SET_CLK_FREQ.bYCC8bit = TRUE;
#else
	DevIoCtrl.SEL.SET_CLK_FREQ.bYCC8bit = FALSE;
#endif
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_FREQ, &DevIoCtrl);

	disphdmi_debug(("HDMI Target Frequency = %d Hertz\r\n", uiTargetFreq));

	return FALSE;

}

static void dispdev_checkSampleRate(BOOL bEnable)
{
	BOOL                    bAudOpened;
	AUDIO_SR                AudSR;

	// Audio must be opened first and can apply set Parameter/Feature.
	bAudOpened = aud_getLockStatus();
	if (bAudOpened == FALSE) {
		if (aud_open() != E_OK) {
			disphdmi_debug(("aud op err\r\n"));
		}

	}

	aud_setFeature(AUDIO_FEATURE_CHECK_PLAY_SAMPLING_RATE, bEnable);

	if (bEnable) {
		// Config Audio Check Sampling Rate
		if (gDispDev_AudFmt == HDMI_AUDIO32KHZ) {
			AudSR =  AUDIO_SR_32000;
		} else if (gDispDev_AudFmt == HDMI_AUDIO44_1KHZ) {
			AudSR =  AUDIO_SR_44100;
		} else {
			AudSR =  AUDIO_SR_48000;
		}
		aud_setParameter(AUDIO_PARAMETER_CHECKED_PLAY_SAMPLING_RATE, AudSR);
	}

	// Restore the original aud open status
	if (bAudOpened == FALSE) {
		aud_close();
	}

}

#if 1

/*
    Display Device API of open HDMI Transmitter

    Display Device API of open HDMI Transmitter

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_NOSPT:  Desired HDMI format no support.
     - @b E_OK:     HDMI Open done and success.
*/
static ER dispdev_openHDMI(void)
{
	PINMUX_LCDINIT          LcdInit;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	pDispDevControl(DISPDEV_IOCTRL_GET_HDMIMODE, &DevIOCtrl);
	gDispDev_HdmiVid = DevIOCtrl.SEL.GET_HDMIMODE.VideoID;
	gDispDev_AudFmt  = DevIOCtrl.SEL.GET_HDMIMODE.AudioID;
	disphdmi_debug(("DEVHDMI: open (%d)\r\n", gDispDev_HdmiVid));


	LcdInit             = pinmux_getDispMode(PINMUX_FUNC_ID_HDMI);
	if (hdmitx_isOpened() == FALSE) {
		pDispDevControl(DISPDEV_IOCTRL_GET_ENABLE, &DevIOCtrl);
		if (DevIOCtrl.SEL.GET_ENABLE.bEn == TRUE) {
			DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
			pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE,    &DevIOCtrl);
			pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
		}

		// Open HDMI-Tx & Read EDID
		hdmitx_open();

		if (hdmitx_getConfig(HDMI_CONFIG_ID_VERIFY_VIDEOID)) {
			hdmitx_verifyVideoID(&gDispDev_HdmiVid);
		}

		// Check & Change IDE Source frequency
		if (dispdev_chgHdmiClkFreq(TRUE)) {
			return E_NOSPT;
		}

		// Enable IDE clock
		DevIOCtrl.SEL.SET_CLK_EN.bClkEn = TRUE;
		pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);


		pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
		if ((DevIOCtrl.SEL.GET_SRCCLK.SrcClk < DISPCTRL_SRCCLK_IDE2_PLL1)) {
			if ((LcdInit & PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_NORMAL) {
				pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_GPIO);
			}
		} else {
			if ((LcdInit & PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_NORMAL) {
				pinmux_setPinmux(PINMUX_FUNC_ID_LCD2, PINMUX_LCD_SEL_GPIO);
			}
		}


		hdmitx_setConfig(HDMI_CONFIG_ID_VIDEO, gDispDev_HdmiVid);
		hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO, gDispDev_AudFmt);

		// Configure VID content
		if (dispdev_initHdmiVidFormat(gDispDev_HdmiVid) == FALSE) {
			return E_NOSPT;
		}

		// Configure IDE
		dispdev_initHdmiDisplay();

		DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = (dispVideoTiming[gDispDev_HdmiIndex].H_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME);
		DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = (dispVideoTiming[gDispDev_HdmiIndex].V_ODD_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME);
		DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = (dispVideoTiming[gDispDev_HdmiIndex].H_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME) - gDispDev_WinHoziOfs;
		DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = (dispVideoTiming[gDispDev_HdmiIndex].V_ODD_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME) - gDispDev_WinVertOfs;
		pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

		DevIOCtrl.SEL.SET_ENABLE.bEn = TRUE;
		pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

		hdmitx_checkVideoStable();
		hdmitx_setConfig(HDMI_CONFIG_ID_OE,             TRUE);
		hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO_STREAM_EN, TRUE);

		hdmitx_setConfig(HDMI_CONFIG_ID_SCDC_ENABLE,    ENABLE);

	} else {
		//
		// HDMI is already opened, Just changing HDMI format
		//
		hdmitx_setConfig(HDMI_CONFIG_ID_AV_MUTE, TRUE);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

		hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO_STREAM_EN, FALSE);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

		if (hdmitx_getConfig(HDMI_CONFIG_ID_VERIFY_VIDEOID)) {
			hdmitx_verifyVideoID(&gDispDev_HdmiVid);
		}

		// Check & Change IDE Source frequency
		if (dispdev_chgHdmiClkFreq(FALSE)) {
			return E_NOSPT;
		}

		// Enable IDE clock
		DevIOCtrl.SEL.SET_CLK_EN.bClkEn = TRUE;
		pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);

		// Configure VID content
		if (dispdev_initHdmiVidFormat(gDispDev_HdmiVid) == FALSE) {
			return E_NOSPT;
		}

		// Configure IDE
		dispdev_initHdmiDisplay();

		DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = (dispVideoTiming[gDispDev_HdmiIndex].H_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME);
		DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = (dispVideoTiming[gDispDev_HdmiIndex].V_ODD_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME);
		DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = (dispVideoTiming[gDispDev_HdmiIndex].H_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].H_STARTTIME) - gDispDev_WinHoziOfs;
		DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = (dispVideoTiming[gDispDev_HdmiIndex].V_ODD_ENDTIME - dispVideoTiming[gDispDev_HdmiIndex].V_ODD_STARTTIME) - gDispDev_WinVertOfs;
		pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

		// Open HDMI-Tx
		hdmitx_setConfig(HDMI_CONFIG_ID_VIDEO, gDispDev_HdmiVid);
		hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO, gDispDev_AudFmt);

		pDispDevControl(DISPDEV_IOCTRL_SET_LOAD,      NULL);

		DevIOCtrl.SEL.SET_ENABLE.bEn = TRUE;
		pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

		hdmitx_checkVideoStable();
		hdmitx_setConfig(HDMI_CONFIG_ID_OE,             TRUE);
		hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO_STREAM_EN, TRUE);

		hdmitx_setConfig(HDMI_CONFIG_ID_SCDC_ENABLE,    ENABLE);

		// Wait TV Video Stable and then clear AV-Mute
		hdmitx_setConfig(HDMI_CONFIG_ID_AV_MUTE, FALSE);
	}

	dispdev_checkSampleRate(ENABLE);

	return E_OK;
}

/*
    Display Device API of Close HDMI Transmitter

    Display Device API of Close HDMI Transmitter

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_OK:     HDMI Close done and success.
*/
static ER dispdev_closeHDMI(void)
{
	DISPDEV_IOCTRL_PARAM    DevIoCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;
#if !_FPGA_EMULATION_
	UINT32                  CurFreq;
	PLL_ID                  SrcPLL = PLL_ID_2;
#endif

	if (hdmitx_isOpened() == FALSE) {
		return E_OK;
	}

	disphdmi_debug(("DEVHDMI: close\r\n"));

	hdmitx_setConfig(HDMI_CONFIG_ID_OE, FALSE);
	// Close HDMI
	hdmitx_close();

	// Disable IDE
	DevIoCtrl.SEL.SET_ENABLE.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIoCtrl);
	pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

	DevIoCtrl.SEL.SET_OUT_COMPONENT.Comp0    = IDE_COMPONENT_R;
	DevIoCtrl.SEL.SET_OUT_COMPONENT.Comp1    = IDE_COMPONENT_G;
	DevIoCtrl.SEL.SET_OUT_COMPONENT.Comp2    = IDE_COMPONENT_B;
	DevIoCtrl.SEL.SET_OUT_COMPONENT.bBitSwap = FALSE;
	DevIoCtrl.SEL.SET_OUT_COMPONENT.bLength  = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_OUT_COMPONENT, &DevIoCtrl);

	DevIoCtrl.SEL.SET_CONST_OFS.uiWinConstOfsX = 0;
	DevIoCtrl.SEL.SET_CONST_OFS.uiWinConstOfsY = 0;
	pDispDevControl(DISPDEV_IOCTRL_SET_CONST_OFS, &DevIoCtrl);

	// Close IDE clock
	DevIoCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIoCtrl);

#if !_FPGA_EMULATION_
	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIoCtrl);
	switch (DevIoCtrl.SEL.GET_SRCCLK.SrcClk) {
	case DISPCTRL_SRCCLK_PLL1:
	case DISPCTRL_SRCCLK_IDE2_PLL1: {
			SrcPLL = PLL_ID_1;
		}
		break;

	case DISPCTRL_SRCCLK_PLL2:
	case DISPCTRL_SRCCLK_IDE2_PLL2: {
			SrcPLL = PLL_ID_2;
		}
		break;

	case DISPCTRL_SRCCLK_PLL4:
	case DISPCTRL_SRCCLK_IDE2_PLL4: {
			SrcPLL = PLL_ID_4;
		}
		break;

	case DISPCTRL_SRCCLK_PLL14:
	case DISPCTRL_SRCCLK_IDE2_PLL14: {
			SrcPLL = PLL_ID_14;
		}
		break;

	default:
		break;
	}

	CurFreq = pll_getPLLFreq(SrcPLL) / 1000000;
	if ((CurFreq != gDispDev_SrcFreq) && (SrcPLL != PLL_ID_1)) {
		CurFreq = gDispDev_SrcFreq * 131072 / 12;

		pll_setPLLEn(SrcPLL,    FALSE);
		pll_setPLL(SrcPLL,      CurFreq);
		pll_setPLLEn(SrcPLL,    TRUE);
	}
#endif

	dispdev_checkSampleRate(DISABLE);

	return E_OK;
}

/*
    Set HDMI Display Device IOCTRL function pointer
*/
static void dispdev_setHdmiIOCTRL(FP pIoCtrlFunc)
{
	pHdmiIoControl = (DISPDEV_IOCTRL)pIoCtrlFunc;
}

/*
    PreGet HDMI size

    PreGet HDMI size

    @return void
*/
static ER dispdev_getHdmiSize(DISPDEV_GET_PRESIZE *tSize)
{
	UINT32                  idx;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pHdmiIoControl;


	disphdmi_debug(("getHDMISize START\r\n"));

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	//
	//Search the mapping Video ID index
	//
	pDispDevControl(DISPDEV_IOCTRL_GET_HDMIMODE, &DevIOCtrl);
	for (idx = 0; idx < VIDMAX; idx++) {
		if (dispVideoTiming[idx].VID == DevIOCtrl.SEL.GET_HDMIMODE.VideoID) {
			break;
		}
	}

	if (idx < VIDMAX) {
		tSize->uiBufWidth   = (dispVideoTiming[idx].H_ENDTIME - dispVideoTiming[idx].H_STARTTIME);
		tSize->uiBufHeight  = (dispVideoTiming[idx].V_ODD_ENDTIME - dispVideoTiming[idx].V_ODD_STARTTIME);
	} else {
		tSize->uiBufWidth   = 0;
		tSize->uiBufHeight  = 0;
	}


	return E_OK;
}

#endif

/**
    Get HDMI Display Device Object

    Get HDMI Display Device Object. This Object is used to control the HDMI Transmitter.

    @return The pointer of HDMI Display Device object.
*/
PDISPDEV_OBJ dispdev_getHdmiDevObj(void)
{
	return &DispDev_HdmiObj;
}


