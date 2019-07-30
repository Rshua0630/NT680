/**
    Header file for Display library

    Header file for Display library.

    @file       ide2.h
    @ingroup    mIDrvDisp_IDE
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _IDE2_H
#define _IDE2_H

#include "Type.h"
#include "ide.h"
#if 0
/**
    @addtogroup mIDrvDisp_IDE
*/
//@{

/**
    IDE channel ID

    @note For ide_setConfig()
*/
typedef enum {
	IDE_ID_1,                          ///< IDE Controller
	IDE_ID_2,                          ///< IDE2 Controller

	ENUM_DUMMY4WORD(IDE_ID)
} IDE_ID;

typedef enum {
	IDE_YCCCLAMP_NOCLAMP,
	IDE_YCCCLAMP_1_254,
	IDE_YCCCLAMP_16_235,

	ENUM_DUMMY4WORD(IDE_YCCCLAMP)
} IDE_YCCCLAMP;


/**
    IDE configuration ID

    @note For IDE_setConfig()
*/
typedef enum {
	//
	IDE_CONFIG_DITHER_EN,              ///<
	IDE_CONFIG_DITHER_FREERUN,         ///<
	IDE_CONFIG_DISPDEV,
	IDE_CONFIG_PDIR,
	IDE_CONFIG_LCDL0,
	IDE_CONFIG_LCDL1,
	IDE_CONFIG_HSINV,
	IDE_CONFIG_VSINV,
	IDE_CONFIG_HVLDINV,
	IDE_CONFIG_VVLDINV,
	IDE_CONFIG_CLKINV,
	IDE_CONFIG_FLDINV,
	IDE_CONFIG_RGBDSEL,
	IDE_CONFIG_DEINV,
	IDE_CONFIG_OUTDDR,
	IDE_CONFIG_THROUGHSEL,

	//
	IDE_CONFIG_IDEEN,
	IDE_CONFIG_LAYEREN,

	//TG
	IDE_CONFIG_TG_HSYNC,
	IDE_CONFIG_TG_HTOTAL,
	IDE_CONFIG_TG_HSYNCDLY,
	IDE_CONFIG_TG_HVALIDST,
	IDE_CONFIG_TG_HVALIDED,
	IDE_CONFIG_TG_VSYNC,
	IDE_CONFIG_TG_VTOTAL,
	IDE_CONFIG_TG_VSYNCDLY,
	IDE_CONFIG_TG_FLD0_VVALIDST,
	IDE_CONFIG_TG_FLD0_VVALIDED,
	IDE_CONFIG_TG_FLD1_VVALIDST,
	IDE_CONFIG_TG_FLD1_VVALIDED,
	IDE_CONFIG_TG_INTERLACE_EN,
	IDE_CONFIG_TG_FLD0_CBLKST,
	IDE_CONFIG_TG_FLD0_CBLKED,
	IDE_CONFIG_TG_FLD1_CBLKST,
	IDE_CONFIG_TG_FLD1_CBLKED,
	IDE_CONFIG_TG_CFIDST,
	IDE_CONFIG_TG_CFIDED,


	//

	ENUM_DUMMY4WORD(IDE_CONFIG_ID)
} IDE_CONFIG_ID;



//new API
extern ER    idec_open(IDE_ID id);
extern BOOL  idec_isOpened(IDE_ID id);
extern ER    idec_close(IDE_ID id);
extern ER    idec_setConfig(IDE_ID IDEID, IDE_CONFIG_ID configID, UINT32 configContext);



//-------------------------------------------------
// IDE register programm APIs
//-------------------------------------------------

// IDE control
void idec_setDithering(IDE_ID id, BOOL b_en, BOOL b_freerun);
void idec_setDevice(IDE_ID id, IDE_DEVICE_TYPE uidevice);
void idec_setPdir(IDE_ID id, IDE_PDIR b_pdir);
void idec_setOdd(IDE_ID id, IDE_PORDER uiODD);
void idec_setEven(IDE_ID id, IDE_PORDER uiEVEN);
void idec_setHsInv(IDE_ID id, BOOL b_inv);
void idec_setVsInv(IDE_ID id, BOOL b_inv);
void idec_setHvldInv(IDE_ID id, BOOL b_inv);
void idec_setVvldInv(IDE_ID id, BOOL b_inv);
void idec_setClkInv(IDE_ID id, BOOL b_inv);
void idec_setFldInv(IDE_ID id, BOOL b_inv);
void idec_setDeInv(IDE_ID id, BOOL b_inv);
void idec_setRgbd(IDE_ID id, BOOL b_en);
void idec_setThrough(IDE_ID id, BOOL b_en);
void idec_setHdmiDdr(IDE_ID id, BOOL b_en);
void idec_setReset(IDE_ID id);
void idec_setEn(IDE_ID id, BOOL b_en);
void idec_setLoad(IDE_ID id);
void idec_setDevNo(IDE_ID id, BOOL b_no);
void idec_setClk1_2(IDE_ID id, BOOL b_en);
void idec_setHLPFEn(IDE_ID id, BOOL b_en);
BOOL idec_getHLPFEn(IDE_ID id);

BOOL idec_getEn(IDE_ID id);
BOOL idec_getLoad(IDE_ID id);
BOOL idec_getThrough(IDE_ID id);
BOOL idec_getRgbd(IDE_ID id);
IDE_DEVICE_TYPE idec_getDevice(IDE_ID id);

UINT32 idec_getWindowEn(IDE_ID id);
void idec_setAllWindowDis(IDE_ID id);
void idec_setAllWindowEn(IDE_ID id, UINT32 uiWins);

// Scaling control
//void idec_setDimCvert(void);
//void idec_setCvertFactor(UINT32 uiHFT, UINT32 uiVFT);

// Palette Control
void idec_setPalEntry(IDE_ID id, UINT32 uiEntry, UINT8 uiColorY, UINT8 uiColorCB, UINT8 uiColorCR, UINT8 uiAlpha);
void idec_getPalEntry(IDE_ID id, UINT32 uiEntry, UINT8 *uiColorY, UINT8 *uiColorCB, UINT8 *uiColorCR, UINT8 *uiAlpha);
void idec_setPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_getPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_setPaletteGroupACrCbY(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_getPaletteGroupACrCbY(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_setPal(IDE_ID id, UINT32 uiEntry, UINT32 uiPalette);
void idec_getPal(IDE_ID id, UINT32 uiEntry, UINT32 *uiPalette);

// Misc
void idec_setBackground(IDE_ID id, UINT8 uiColorY, UINT8 uiColorCB, UINT8 uiColorCR);
void idec_getBackground(IDE_ID id, UINT8 *uiColorY, UINT8 *uiColorCB, UINT8 *uiColorCR);



// Timing
void idec_setHorTiming(IDE_ID id, UINT32 uiHsynct, UINT32 uiHtotal, UINT32 uiHvalidst, UINT32 uiHvalided);
void idec_setVerTiming(IDE_ID id, UINT32 uiVsynct, UINT32 uiVtotal, UINT32 uiODDVvalidst, UINT32 uiODDVvalided, UINT32 uiEVENVvalidst, UINT32 uiEVNEVvalided);
void idec_getHorTiming(IDE_ID id, UINT32 *puiHsynct, UINT32 *puiHtotal, UINT32 *puiHvalidst, UINT32 *puiHvalided);
void idec_getVerTiming(IDE_ID id, UINT32 *puiVsynct, UINT32 *puiVtotal, UINT32 *puiODDVvalidst, UINT32 *puiODDVvalided, UINT32 *puiEVENVvalidst, UINT32 *puiEVNEVvalided);
void idec_setInterLoadMode(IDE_ID id, BOOL b2field);

void idec_setSyncDelay(IDE_ID id, UINT8 uiHsDealy, UINT8 uiVsDelay);
void idec_setInterlace(IDE_ID id, BOOL b_inter);
void idec_setStartField(IDE_ID id, BOOL b_oddst);
BOOL idec_getInterlace(IDE_ID id);
BOOL idec_getStartField(IDE_ID id);
BOOL idec_getCurField(IDE_ID id);
void idec_setDigitalTiming(IDE_ID id, UINT32 uiCoddBlkSt, UINT32 uiCoddBlkEd, UINT32 uiCevenBlkSt, UINT32 uiCevenBlkEd, UINT32 uiCfidSt, UINT32 uiCfidEd);

// CSB
void idec_fillGamma(IDE_ID id, UINT8 *uipgamma);
void idec_getGamma(IDE_ID id, UINT8 *uipgamma);
void idec_fillRGBGamma(IDE_ID id, UINT8 *uiprgamma, UINT8 *uipggamma, UINT8 *uipbgamma);
void idec_getRGBGamma(IDE_ID id, UINT8 *uiprgamma, UINT8 *uipggamma, UINT8 *uipbgamma);
void idec_setCtrst(IDE_ID id, UINT32 uiCTRST);
UINT32 idec_getCtrst(IDE_ID id);
void idec_setBrt(IDE_ID id, UINT32 uiBRT);
INT8 idec_getBrt(IDE_ID id);
void idec_setCmults(IDE_ID id, UINT32 uiCMULTS);
UINT32 idec_getCmults(IDE_ID id);

void idec_setClamp(IDE_ID id,  IDE_YCCCLAMP clamp);
IDE_YCCCLAMP idec_getClamp(IDE_ID id);
void idec_setCex(IDE_ID id, BOOL b_cex);
BOOL idec_getCex(IDE_ID id);
void idec_setYCex(IDE_ID id, BOOL b_ycex);
BOOL idec_getYCex(IDE_ID id);

void idec_setIcst0(IDE_ID id, BOOL b_en);
BOOL idec_getIcst0(IDE_ID id);
void idec_setIcst(IDE_ID id, BOOL b_en);
BOOL idec_getIcst(IDE_ID id);
void idec_setCst1(IDE_ID id, BOOL b_en);
BOOL idec_getCst1(IDE_ID id);


// color control
void idec_setColorCtrlEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCtrlEn(IDE_ID id);
void idec_setColorCompAdjEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCompAdjEn(IDE_ID id);
void idec_setColorCtrlHueAdjEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCtrlHueAdjEn(IDE_ID id);
void idec_setColorCompYConEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCompYConEn(IDE_ID id);
void idec_setColorCompCConEn(IDE_ID id, BOOL b_en);
BOOL idec_getColorCompCConEn(IDE_ID id);
void idec_setColorCtrlHue(IDE_ID id, UINT8 *hue24tbl);
void idec_getColorCtrlHue(IDE_ID id, UINT8 *hue24tbl);
void idec_setColorCtrlInt(IDE_ID id, INT8 *intensity24tbl);
void idec_getColorCtrlInt(IDE_ID id, INT8 *intensity24tbl);
void idec_setColorCtrlSat(IDE_ID id, INT8 *sat24tbl);
void idec_getColorCtrlSat(IDE_ID id, INT8 *sat24tbl);
void idec_setColorCtrlDds(IDE_ID id, UINT8 *dds8tbl);
void idec_getColorCtrlDds(IDE_ID id, UINT8 *dds8tbl);
void idec_setColorCtrlIntSatOfs(IDE_ID id, INT8 int_ofs, INT8 sat_ofs);
void idec_getColorCtrlIntSatOfs(IDE_ID id, INT8 *int_ofs, INT8 *sat_ofs);
void idec_setColorCompYCon(IDE_ID id, UINT8 cony);
void idec_getColorCompYCon(IDE_ID id, UINT8 *cony);
void idec_setColorCompCCon(IDE_ID id, UINT8 conc);
void idec_getColorCompCCon(IDE_ID id, UINT8 *conc);
void idec_setColorCompYOfs(IDE_ID id, INT8 yofs);
void idec_getColorCompYOfs(IDE_ID id, INT8 *yofs);
void idec_setColorCompCOfs(IDE_ID id, UINT8 cbofs, UINT8 crofs);
void idec_getColorCompCOfs(IDE_ID id, UINT8 *cbofs, UINT8 *crofs);

void idec_setCsbEn(IDE_ID id, BOOL b_en);
BOOL idec_getCsbEn(IDE_ID id);
void idec_setGammaEn(IDE_ID id, BOOL b_en);
BOOL idec_getGammaEn(IDE_ID id);
void idec_setDitherVbits(IDE_ID id, IDE_DITHER_VBITS b_rsel, IDE_DITHER_VBITS b_gsel, IDE_DITHER_VBITS b_bsel);
void idec_setOutComp(IDE_ID id, IDE_OUT_COMP uiComp0, IDE_OUT_COMP uiComp1, IDE_OUT_COMP uiComp2, BOOL b_BitSwp, BOOL b_Len);
void idec_getOutComp(IDE_ID id, IDE_OUT_COMP *uiComp0, IDE_OUT_COMP *uiComp1, IDE_OUT_COMP *uiComp2, BOOL *b_BitSwp, BOOL *b_Len);

// Interrupt
UINT32 idec_getInterruptStatus(IDE_ID id);
void idec_clearInterruptStatus(IDE_ID id, UINT32 uiINTstatus);
void idec_setInterruptEn(IDE_ID id, UINT32 uiINTen);
void idec_clrInterruptEn(IDE_ID id, UINT32 uiINT);
UINT32 idec_getInterruptEn(IDE_ID id);

// ICST0
void idec_setIcst0PreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void idec_getIcst0PreOffset(IDE_ID id, INT32 *uiY, INT32 *uiCb, INT32 *uiCr);
void idec_setIcst0Coef(IDE_ID id, UINT32 *uipCoef);
void idec_getIcst0Coef(IDE_ID id, INT32 *uipCoef);
void idec_setOutOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
void idec_getOutOffset(IDE_ID id, INT32 *uiY, INT32 *uiCb, INT32 *uiCr);
void idec_setOutLimit(IDE_ID id, UINT8 uiYLow, UINT8 uiYUp, UINT8 uiCbLow, UINT8 uiCbUp, UINT8 uiCrLow, UINT8 uiCrUp);

// ICST
void idec_setIcstCoef(IDE_ID id, UINT32 *uipCoef);
void idec_getIcstCoef(IDE_ID id, INT32 *uipCoef);


// Alpha Blending
void idec_setAlphaBlending(IDE_ID id, IDE_BLEND_LAYER uiLayer, IDE_ALPHA_TYPE uiAlphaType, UINT8 uiGlobalAlpha);

// Shawdow register
void idec_setO1PalShwEn(IDE_ID id, BOOL bEn);
void idec_setLBReadEn(IDE_ID id, BOOL bEn);
void idec_setFDExpEn(IDE_ID id, BOOL bEn);



// Sub-Pixel interpolation
void idec_setSubpixel(IDE_ID id, BOOL bOddLine, BOOL bR, BOOL bG, BOOL bB);
void idec_getSubpixel(IDE_ID id, BOOL bOddLine, BOOL *bR, BOOL *bG, BOOL *bB);

// FD
UINT32 idec_getFDAllEn(IDE_ID id);
void idec_setFDEn(IDE_ID id, IDE_FD_NUM uiNum);
void idec_setFDDis(IDE_ID id, IDE_FD_NUM uiNum);
void idec_setFDWinPos(IDE_ID id, IDE_FD_NUM uiNum, UINT32 uiX, UINT32 uiY);
void idec_getFDWinPos(IDE_ID id, IDE_FD_NUM uiNum, UINT32 *uiX, UINT32 *uiY);
void idec_setFDWinDim(IDE_ID id, IDE_FD_NUM uiNum, UINT32 uiWinW, UINT32 uiWinH);
void idec_getFDWinDim(IDE_ID id, IDE_FD_NUM uiNum, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_setFDWinBord(IDE_ID id, IDE_FD_NUM uiNum, UINT32 uiBorH, UINT32 uiBorV);
void idec_getFDWinBord(IDE_ID id, IDE_FD_NUM uiNum, UINT32 *uiBorH, UINT32 *uiBorV);
void idec_setFDColor(IDE_ID id, IDE_FD_NUM uiNum, UINT8 uiY, UINT8 uiCB, UINT8 uiCR);
void idec_getFDColor(IDE_ID id, IDE_FD_NUM uiNum, UINT8 *uiY, UINT8 *uiCB, UINT8 *uiCR);

//-------------------------------------------------
// IDE Video register programm APIs
//-------------------------------------------------

// Video control
void idec_setV1En(IDE_ID id, BOOL b_en);
void idec_setV2En(IDE_ID id, BOOL b_en);
BOOL idec_getV1En(IDE_ID id);
BOOL idec_getV2En(IDE_ID id);

// Video buffer
void idec_setV1Buf0Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV1Buf1Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV1Buf2Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV1Buf0Odd(IDE_ID id, BOOL b_odd);
void idec_setV1Buf1Odd(IDE_ID id, BOOL b_odd);
void idec_setV1Buf2Odd(IDE_ID id, BOOL b_odd);
void idec_setV1BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setV1ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
void idec_setV1BufOp(IDE_ID id, IDE_BJMODE uiBJMODE, IDE_OP_BUF uiOPTBUF, IDE_BUF_NUM uiBUFNUM);
void idec_chV1Buf(IDE_ID id, IDE_OP_BUF uiOPTBUF);
//void idec_setV1Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);
void idec_setV1Src(IDE_ID id, IDE_SYNC_SRC b_sel);

void idec_setV2Buf0Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV2Buf1Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV2Buf2Addr(IDE_ID id, UINT32 uiYaddr, UINT32 uiCBaddr, UINT32 uiCRaddr);
void idec_setV2Buf0Odd(IDE_ID id, BOOL b_odd);
void idec_setV2Buf1Odd(IDE_ID id, BOOL b_odd);
void idec_setV2Buf2Odd(IDE_ID id, BOOL b_odd);
void idec_setV2BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setV2ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
void idec_setV2BufOp(IDE_ID id, IDE_BJMODE uiBJMODE, IDE_OP_BUF uiOPTBUF, IDE_BUF_NUM uiBUFNUM);
void idec_chV2Buf(IDE_ID id, IDE_OP_BUF uiOPTBUF);
//void idec_setV2Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);
void idec_setV2Src(IDE_ID id, IDE_SYNC_SRC b_sel);

void idec_getV1Buf0Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV1Buf1Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV1Buf2Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV1Buf0Odd(IDE_ID id, BOOL *b_odd);
void idec_getV1Buf1Odd(IDE_ID id, BOOL *b_odd);
void idec_getV1Buf2Odd(IDE_ID id, BOOL *b_odd);
void idec_getV1BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getV1BufOp(IDE_ID id, IDE_BJMODE *puiBJMODE, IDE_OP_BUF *puiOPTBUF, IDE_BUF_NUM *puiBUFNUM);
void idec_getV1ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

void idec_getV2Buf0Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV2Buf1Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV2Buf2Addr(IDE_ID id, UINT32 *puiYaddr, UINT32 *puiCBaddr, UINT32 *puiCRaddr);
void idec_getV2Buf0Odd(IDE_ID id, BOOL *b_odd);
void idec_getV2Buf1Odd(IDE_ID id, BOOL *b_odd);
void idec_getV2Buf2Odd(IDE_ID id, BOOL *b_odd);
void idec_getV2BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getV2BufOp(IDE_ID id, IDE_BJMODE *puiBJMODE, IDE_OP_BUF *puiOPTBUF, IDE_BUF_NUM *puiBUFNUM);
void idec_getV2ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

// Video Window
void idec_setV1ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setV1ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setV1VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setV1WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setV1WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setV1Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);
void idec_setV2Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);

void idec_setV2ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setV2ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setV2VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setV1Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);
void idec_setV2WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setV2WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setV2Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);

void idec_getV1ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getV1WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getV1WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getV1Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);

void idec_getV2ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getV2WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getV2WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getV2Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);

// Video Color key and Blending
void idec_setVideoColorKey(IDE_ID id, UINT8 uiCkY, UINT8 uiCkCb, UINT8 uiCkCr);
//void ide_setVideoBlendOp(IDE_VIDEO_BLEND_OP uiOp);
void idec_setVideoColorKeySel(IDE_ID id, IDE_VIDEO_COLORKEY_SEL b_sel);
void idec_setVideoColorKeyOp(IDE_ID id, IDE_VIDEO_COLORKEY_OP uiCkOp);

void idec_getVideoColorKey(IDE_ID id, UINT8 *uiCkY, UINT8 *uiCkCb, UINT8 *uiCkCr);
//void ide_getVideoBlendOp(IDE_ID id, IDE_VIDEO_BLEND_OP *uiOp);
void idec_getVideoColorKeySel(IDE_ID id, IDE_VIDEO_COLORKEY_SEL *b_sel);
void idec_getVideoColorKeyOp(IDE_ID id, IDE_VIDEO_COLORKEY_OP *uiCkOp);

// Video Color Space Transform
//void idec_setV1CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV1CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV1CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void idec_setV2CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV2CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setV2CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);


// Video Auto Blinking
void idec_setV1Blink(IDE_ID id, BOOL b_ovr, BOOL b_und, BOOL b_sel);
void idec_setV1Count(IDE_ID id, UINT8 uiCnt);
void idec_setV1OvrExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV1OvrExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV1UndExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV1UndExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2Blink(IDE_ID id, BOOL b_ovr, BOOL b_und, BOOL b_sel);
void idec_setV2Count(IDE_ID id, UINT8 uiCnt);
void idec_setV2OvrExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2OvrExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2UndExpThreshold(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
void idec_setV2UndExpColor(IDE_ID id, UINT8 uiY, UINT8 uiCb, UINT8 uiCr);
UINT8 idec_getV1Count(IDE_ID id);
UINT8 idec_getV2Count(IDE_ID id);
void idec_getV1OvrExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2OvrExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV1UndExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2UndExpThreshold(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV1OvrExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2OvrExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV1UndExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);
void idec_getV2UndExpColor(IDE_ID id, UINT8 *uiY, UINT8 *uiCb, UINT8 *uiCr);

//-------------------------------------------------
// IDE OSD register programm APIs
//-------------------------------------------------

// OSD control
void idec_setO1En(IDE_ID id, BOOL b_en);
void idec_setO2En(IDE_ID id, BOOL b_en);
BOOL idec_getO1En(IDE_ID id);
BOOL idec_getO2En(IDE_ID id);

// OSD buffer
void idec_setO1BufAddr(IDE_ID id, UINT32 uiaddr);
void idec_setO1BufAlphaAddr(IDE_ID id, UINT32 uiaddr);
void idec_setO1BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setO1ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
//void ide_setO1Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);

void idec_setO2BufAddr(IDE_ID id, UINT32 uiaddr);
void idec_setO2BufDim(IDE_ID id, UINT32 uiBW, UINT32 uiBH, UINT32 uiLineoffset);
void idec_setO2ReadOrder(IDE_ID id, IDE_HOR_READ b_L2R, IDE_VER_READ b_T2B);
//void ide_setO2Cst(IDE_ID id, BOOL b_en, CST_SEL b_sel);

void idec_getO1BufAddr(IDE_ID id, UINT32 *uiaddr);
void idec_getO1AlphaBufAddr(IDE_ID id, UINT32 *uiaddr);
void idec_getO1BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getO1ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

void idec_getO2BufAddr(IDE_ID id, UINT32 *uiaddr);
void idec_getO2BufDim(IDE_ID id, UINT32 *puiBW, UINT32 *puiBH, UINT32 *puiLineoffset);
void idec_getO2ReadOrder(IDE_ID id, IDE_HOR_READ *pb_L2R, IDE_VER_READ *pb_T2B);

// OSD Window
void idec_setO1ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setO1ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setO1VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setO1Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);
void idec_setO1Vsm(IDE_ID id, IDE_SCALE_METHOD vsm);
void idec_setO1WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setO1WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setO1PaletteSel(IDE_ID id, IDE_PALETTE_SEL uiPsel);
void idec_setO1PaletteHighAddr(IDE_ID id, UINT8 uiHiAddr);
void idec_setO1Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);

void idec_setO2ScaleCtrl(IDE_ID id, BOOL b_Hscaleup, BOOL b_Vscaleup);
void idec_setO2ScaleFactor(IDE_ID id, UINT32 uiHSF, BOOL b_sub, UINT32 uiVSF, BOOL b_vsub);
void idec_setO2VsfInit(IDE_ID id, UINT32 uiInit0, UINT32 uiInit1);
void idec_setO2Hsm(IDE_ID id, IDE_SCALE_METHOD hsm);
void idec_setO2WinDim(IDE_ID id, UINT32 uiWinW, UINT32 uiWinH);
void idec_setO2WinPos(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_setO2PaletteSel(IDE_ID id, IDE_PALETTE_SEL uiPsel);
void idec_setO2PaletteHighAddr(IDE_ID id, UINT8 uiHiAddr);
void idec_setO2Fmt(IDE_ID id, IDE_COLOR_FORMAT uiFmt);

void idec_getO1ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getO1WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getO1WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getO1PaletteSel(IDE_ID id, IDE_PALETTE_SEL *uiPsel);
void idec_getO1PaletteHighAddr(IDE_ID id, UINT8 *uiHiAddr);
void idec_getO1Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);

void idec_getO2ScaleFactor(IDE_ID id, UINT32 *uiHSF, BOOL *b_sub, UINT32 *uiVSF, BOOL *b_vsub);
void idec_getO2WinDim(IDE_ID id, UINT32 *uiWinW, UINT32 *uiWinH);
void idec_getO2WinPos(IDE_ID id, UINT32 *uiX, UINT32 *uiY);
void idec_getO2PaletteSel(IDE_ID id, IDE_PALETTE_SEL *uiPsel);
void idec_getO2PaletteHighAddr(IDE_ID id, UINT8 *uiHiAddr);
void idec_getO2Fmt(IDE_ID id, IDE_COLOR_FORMAT *uiFmt);

// OSD Color key and Blending
void idec_setOsdColorKeyEn(IDE_ID id, BOOL b_en);
void idec_setOsdColorKey(IDE_ID id, UINT8 uiCk);
//void ide_setOsdBlendOp(IDE_ID id, IDE_OSD_BLEND_OP uiOp);

void idec_getOsdColorKey(IDE_ID id, UINT8 *uiCk);
void idec_getOsdColorKeyEn(IDE_ID id, BOOL *b_en);
//void idec_getOsdBlendOp(IDE_ID id, IDE_OSD_BLEND_OP *uiOp);

void idec_setOsd1ColorKeyEn(IDE_ID id, BOOL b_en);
void idec_getOsd1ColorKeyEn(IDE_ID id, BOOL *b_en);
void idec_setOsd1ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP ck_op);
void idec_getOsd1ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP *ck_op);
void idec_setOsd1ColorKey(IDE_ID id, UINT8 uiKey_y, UINT8 uiKey_cb, UINT8 uiKey_cr);
void idec_getOsd1ColorKey(IDE_ID id, UINT8 *uiKey_y, UINT8 *uiKey_cb, UINT8 *uiKey_cr);
void idec_setOsd2ColorKeyEn(IDE_ID id, BOOL b_en);
void idec_getOsd2ColorKeyEn(IDE_ID id, BOOL *b_en);
void idec_setOsd2ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP ck_op);
void idec_getOsd2ColorKeyOp(IDE_ID id, IDE_OSD_COLORKEY_OP *ck_op);
void idec_setOsd2ColorKey(IDE_ID id, UINT8 uiKey_y, UINT8 uiKey_cb, UINT8 uiKey_cr);
void idec_getOsd2ColorKey(IDE_ID id, UINT8 *uiKey_y, UINT8 *uiKey_cb, UINT8 *uiKey_cr);

// OSD Color Space Transform
//void idec_setO1CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO1CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO1CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);

//void idec_setO2CstPreOffset(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO2CstPostOffst(IDE_ID id, UINT32 uiY, UINT32 uiCb, UINT32 uiCr);
//void idec_setO2CstCoef(IDE_ID id, ACCRCY_SEL b_accuracy, UINT32 *uipCoef);



//-------------------------------------------------
// IDE Public Functions
//-------------------------------------------------

BOOL idec_setVideoBufAddr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ADDR *pV_Buf_Addr);
BOOL idec_getVideoBufAddr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ADDR *pV_Buf_Addr);
BOOL idec_setVideoBufAttr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ATTR *pV_Buf_attr);
BOOL idec_getVideoBufAttr(IDE_ID id, IDE_VIDEOID VideoID, VIDEO_BUF_ATTR *pV_Buf_attr);
BOOL idec_setVideoWinAttrEx(IDE_ID id, IDE_VIDEOID VideoID, VOSD_WINDOW_ATTR *pV_Win_attr, BOOL bLoad);
BOOL idec_setVideoWinAttr(IDE_ID id, IDE_VIDEOID VideoID, VOSD_WINDOW_ATTR *pV_Win_attr);
void idec_setVideoVsfInit(IDE_ID id, IDE_VIDEOID VideoID, UINT32 uiInit0, UINT32 uiInit1);

BOOL idec_setOsdBufAddr(IDE_ID id, IDE_OSDID OSDID, UINT32 uiOSD_Addr);
BOOL idec_getOsdBufAddr(IDE_ID id, IDE_OSDID OSDID, UINT32 *puiOSD_Addr);
BOOL idec_setOsdBufAttr(IDE_ID id, IDE_OSDID OSDID, OSD_BUF_ATTR *pOSD_Buf_attr);
BOOL idec_getOsdBufAttr(IDE_ID id, IDE_OSDID OSDID, OSD_BUF_ATTR *pOSD_Buf_attr);
BOOL idec_setOsdWinAttrEx(IDE_ID id, IDE_OSDID OSDID, VOSD_WINDOW_ATTR *pOSD_Win_attr, BOOL bLoad);
BOOL idec_setOsdWinAttr(IDE_ID id, IDE_OSDID OSDID, VOSD_WINDOW_ATTR *pOSD_Win_attr);
void idec_setOsdVsfInit(IDE_ID id, IDE_OSDID OSDID, UINT32 uiInit0, UINT32 uiInit1);

void idec_setPaletteEntry(IDE_ID id, PALETTE_ENTRY *pPalette_entry);
void idec_getPaletteEntry(IDE_ID id, PALETTE_ENTRY *pPalette_entry);
void idec_setPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);
void idec_getPaletteGroup(IDE_ID id, UINT32 uiStart, UINT32 uiNumber, UINT32 *pPalette_entry);

void idec_waitFrameEnd(IDE_ID id);

void idec_disableVideo(IDE_ID id, IDE_VIDEOID VideoID);
void idec_enableVideo(IDE_ID id, IDE_VIDEOID VideoID);
BOOL idec_getVideoEnable(IDE_ID id, IDE_VIDEOID VideoID);

void idec_disableOsd(IDE_ID id, IDE_OSDID OSDID);
void idec_enableOsd(IDE_ID id, IDE_OSDID OSDID);
BOOL idec_getOsdEnable(IDE_ID id, IDE_OSDID OSDID);
IDE_PAL_CAP idec_getPalCapability(IDE_ID id);

void idec_setVideoBufferContent(IDE_ID id, IDE_VIDEOID VIDEO_ID, IDE_BUFFERID BUFFER_ID, PYUVCOLOR pYCbCr);
void idec_convertRGB2YCbCr(IDE_ID id, PRGBCOLOR pRGB, PYUVCOLOR pYCbCr);

//void idec_configVideoCst(IDE_VIDEOID VideoID, BOOL b_en, CST_SEL sel);
//void idec_configOsdCst(IDE_OSDID OSDID, BOOL b_en, CST_SEL sel);
//void idec_configIcst(BOOL b_en, CST_SEL sel);
void idec_configIcst(IDE_ID id, BOOL b_en, CST_SEL sel);
void idec_getconfigIcst(IDE_ID id, BOOL *b_en, CST_SEL *sel);
void idec_configOutputLimit(IDE_ID id, BOOL b_sel);
void idec_setConstantWindowOffset(IDE_ID id, UINT32 uiX, UINT32 uiY);
void idec_getConstantWindowOffset(IDE_ID id, UINT32 *uiX, UINT32 *uiY);

//@}
#endif

#endif //_IDE_H

