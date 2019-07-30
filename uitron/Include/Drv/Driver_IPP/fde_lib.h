/*
    Public header file for FDE module.

    This file is the header file that define the API and data type for FDE
    module.

    @file       fde_lib.h
    @ingroup    mIDrvIPP_FDE
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _FDE_LIB_H
#define _FDE_LIB_H

/**
    @addtogroup mIDrvIPP_FDE
*/
//@{

/**
    Define FDE interrput enable.
*/
//@{
#define FDE_INTE_ALLSIZE                0x00000001  ///< all sizes are done
#define FDE_INTE_SIZE                   0x00000002  ///< one size done
//#define FDE_INTE_16                     0x00000004  ///< 16 faces detected
#define FDE_INTE_OFAIL                  0x00000008  ///< overflow flag
#define FDE_INTE_ALL                    0x0000000B  ///< all interrupts

//@}

/**
    Define FDE interrput status.
*/
//@{
#define FDE_INT_ALL                     0x00000001  ///< all interrupt
#define FDE_INT_SIZE                    0x00000002  ///< one size done
//#define FDE_INT_16                      0x00000004  ///< 16 faces detected
#define FDE_INT_OFAIL                   0x00000008  ///< overflow flag
//@}

/**
    Define FDE parameters.
*/
//@{
#define FACE_NUM_MAX                    96 //64 //36          ///< max detection number
//@}

/**
    FDE FD Function Selection.

    Select FD functions.
\n  Used for fde_setFDMode()
*/
//@{
typedef enum {
	FDE_FD_FACEDETECTION    = 0,    ///< face detection
	FDE_FD_EDGE_EXTRACTION  = 1,    ///< edge extraction
	FDE_FD_SCALING          = 2,    ///< scaling down
	FDE_USERDEFINE          = 3,    ///< all manual mode
	ENUM_DUMMY4WORD(FDE_FDFUNC)
} FDE_FDFUNC;
//@}

/**
    FDE FD OPeration Mode Selection.

    Select FDE operation mode.
*/
//@{
typedef enum {
	FDE_OPMODE_FD       = 0,        ///< normal face detection mode
	FDE_OPMODE_LUMIN    = 1,        ///< output luminance plane
	FDE_OPMODE_EDGH     = 2,        ///< output horizontal edge plane
	FDE_OPMODE_EDGV     = 3,        ///< output vertical edge plane
	FDE_OPMODE_EDGHV0   = 4,        ///< output abs(H) + abs(V) edge plane
	FDE_OPMODE_EDGHV1   = 5,        ///< output abs(H+V) edge plane
	FDE_OPMODE_SCALEDOWN = 6,       ///< output scaled down plane
	ENUM_DUMMY4WORD(FDE_FDOPMODE)
} FDE_FDOPMODE;
//@}

#if 0
/**
    FDE FD Scaling Direction.

    Select which direction of scaling factor
*/
//@{
typedef enum {
	FDE_HORIZONTAL  = 0,            ///< horizontal scaling factor
	FDE_VERTICAL    = 1,            ///< vertical scaling factor
	ENUM_DUMMY4WORD(FDE_FDDIRECTION)
} FDE_FDDIRECTION;
//@}
#endif

/**
    FDE OPP En/Disable Selection.

    Select output buffer
*/
//@{
typedef enum {
	FDE_OPPDISABLE   = 0,           ///< one ping-pong buffer
	FDE_OPPENABLE    = 1,           ///< two ping-pong buffer
	FDE_OPPSEQUEN    = 2,           ///< sequential output scaled data
	ENUM_DUMMY4WORD(FDE_FDOPP)
} FDE_FDOPP;
//@}

/**
    FDE Result Format Selection.

    Select result format
\n  FD_RSLT_ORI:  {Target[30..27] Size[26..24] Size[23..18] Sty[17..9] Stx[8..0]}
\n  FD_RSLT_SCORE:   {Target[30..28] Size[21..16] Score2[15..8] Score1[7..0] }
\n  FD_RSLT_COR1:   {Target[30..28] corr2[26..18] corr1[17..9] corr0[8..0]}
\n  FD_RSLT_COR2:   {Target[30..28] corr5[26..18] corr4[17..9] corr3[8..0]}
\n  Stx = actual_stx >> 2;
\n  Sty = actual_sty >> 2;

*/
//@{
typedef enum {
	//FD_RSLT_ORI     = 0,        ///< original result format
	//FD_RSLT_MOD     = 1,        ///< modified result format
	//FD_RSLT_DBG     = 2,        ///< debug result format
	FD_RSLT_ORI     = 0,        ///< original result format
	FD_RSLT_SCORE   = 1,        ///< score format
	FD_RSLT_COR1    = 2,        ///< correlation 0~2 format
	FD_RSLT_COR2    = 3,        ///< correlation 3~5 format
	ENUM_DUMMY4WORD(FD_RSLT_SEL)
} FDE_RSLT_SEL;
//@}

/**
    FDE Result Selection.

    Select original FD coordinate results or remove overlap ones
*/
//@{
typedef enum {
	FDE_RSLT_NONOVERLAP   = 0,      ///< numbers of overlap FDs are removed
	FDE_RSLT_ORIGINAL     = 1,      ///< numbers of original FD engine output
	ENUM_DUMMY4WORD(FDE_RSLT_OVLP)
} FDE_RSLT_OVLP;
//@}

/**
    FDE Face Orientation.

    Select which orientations to detect
*/
//@{
typedef enum {
	FDE_R000_P000       = 0,    ///< frontal face
	FDE_R030_P000       = 1,    ///< rotate positive 30 degree
	FDE_R330_P000       = 2,    ///< rotate negative 30 degree
	FDE_R000_PR90       = 3,    ///< right side face
	FDE_R030_PR90       = 4,    ///< right side face rotate positive 30 degree
	FDE_R330_PR90       = 5,    ///< right side face rotate negative 30 degree
	FDE_R000_PL90       = 6,    ///< left side face
	FDE_R030_PL90       = 7,    ///< left side face rotate positive 30 degree
	FDE_R330_PL90       = 8,    ///< left side face rotate negative 30 degree
	FDE_ORIENT_DISABLE  = 9,    ///< disable this target
	FDE_ORIENT_NUMMAX   = 10,   ///< max number of orientation
	ENUM_DUMMY4WORD(FDE_ORIENT)
} FDE_ORIENT;
//@}

/**
    Struct FDE open object.

    Structure for fde_open()
*/
//@{
typedef struct {
	VOID (*FP_FDEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 uiFdeClockSel; ///< support 220/192/160/120/80/60/48 Mhz
} FDE_OPENOBJ;
//@}

#if 0
/**
    Struct FDE Result Lowpass.
    @brief none.
*/
//@{
typedef struct _FDE_LPF_PRAM {
	BOOL    bEnable;          ///< enable result lowpass
	UINT32  uiShift;          ///< result window moving speed
	UINT32  uiOverdue;        ///< result window overdue
} FDE_LPF_PRAM;
//@}
#endif

/**
    FDE Score Selection.

    Select weighting or AND of the two scores
*/
//@{
typedef enum {
	FDE_SCR_WEIGHTING   = 0,      ///< ((score1*(16-scrwt) + score2* scrwt)>>4) > scrthr0
	FDE_SCR_AND         = 1,      ///< (score1>scrthr0) & (score2>scrthr1)
	ENUM_DUMMY4WORD(FDE_SCORE_SEL)
} FDE_SCORE_SEL;
//@}


/**
    FDE Power Saving Mode

    Select power-saving mode or full functions.
*/
//@{
typedef enum {
	FDE_PWRSAV       = 0,      ///< disable correlation2 to save power. Results can be the same as 650
	FDE_FULL         = 1,      ///< Enable both correlation 1 & 2
	ENUM_DUMMY4WORD(FDE_POWERSAVING)
} FDE_POWERSAVING;
//@}



/**
    Struct FDE target Parameters.

    Set FDE targets Parameters.
*/
//@{
typedef struct {
	UINT32  uiWeight0;      ///< channel0 weighting
	UINT32  uiWeight1;      ///< channel1 weighting
	UINT32  uiWeight2;      ///< channel2 weighting
	UINT32  uiScoreShf0;    ///< score shift
	UINT32  uiScoreTh0;     ///< score threshold
	UINT32  uiStdTh;        ///< std threshold
	UINT32  uiCThEn0;       ///< channel threshold enable
	UINT32  uiCTh0;         ///< channel0 threshold
	UINT32  uiCTh1;         ///< channel1 threshold
	UINT32  uiCTh2;         ///< channel2 threshold

	UINT32  uiWeight3;      ///< channel0 weighting of cor2
	UINT32  uiWeight4;      ///< channel1 weighting of cor2
	UINT32  uiWeight5;      ///< channel2 weighting of cor2
	UINT32  uiScoreShf1;    ///< score shift of cor2
	UINT32  uiScoreTh1;     ///< score threshold of cor2
	UINT32  uiCThEn1;       ///< channel threshold enable of cor2
	UINT32  uiCTh3;         ///< channel0 threshold of cor2
	UINT32  uiCTh4;         ///< channel1 threshold of cor2
	UINT32  uiCTh5;         ///< channel2 threshold of cor2
	UINT32  uiC2FTh0;       ///< channel0 feature threshold of cor2
	UINT32  uiC2FTh1;       ///< channel1 feature threshold of cor2
	UINT32  uiC2FTh2;       ///< channel2 feature threshold of cor2
	UINT32  uiC2Shf;        ///< cor2 shift
} FDE_TGTPARAM;
//@}

/**
    Struct FDE target statistics.

    Set FDE target statistics.
*/
//@{
typedef struct {
	UINT32  uiDenY0;        ///< channel0 denominator
	UINT32  uiCntY0;        ///< channel0 count
	UINT32  uiSumY0;        ///< channel0 sum
	UINT32  uiDenY1;        ///< channel1 denominator
	UINT32  uiCntY1;        ///< channel1 count
	UINT32  uiSumY1;        ///< channel1 sum
	UINT32  uiDenY2;        ///< channel2 denominator
	UINT32  uiCntY2;        ///< channel2 count
	UINT32  uiSumY2;        ///< channel2 sum
} FDE_TGTSTAT;
//@}

/**
    Struct FDE target .

    Set FDE target.
*/
//@{
typedef struct {
	UINT32  *pTarget0;      ///< target 0
	UINT32  *pTarget1;      ///< target 1
	UINT32  *pTarget2;      ///< target 2
	UINT32  *pTarget3;      ///< target 3
	UINT32  *pTarget4;      ///< target 4
} FDE_TARTET;
//@}


/**
    Struct FDE target threshold.

    Set FDE targets threshold.

\n  edge kernal = [eextdb   eextv   eextda
\n                 eexth    eextc   eexth
\n                 eextda   eextv   eextdb] >> eextdiv
*/
//@{
typedef struct {
	UINT8  uiEdgExtDiv;     ///< edge kernal division
	UINT8  uiNSym;          ///< edge kernel symmetric signs
	UINT8  uiEdgExtC;       ///< edge kernal center coefficient
	UINT8  uiEdgExtH;       ///< edge kernal horizontal coefficient
	UINT8  uiEdgExtV;       ///< edge kernal vertical coefficient
	UINT8  uiEdgExtDa;      ///< edge kernal diagonalA coefficient
	UINT8  uiEdgExtDb;      ///< edge kernal diagonalB coefficient
} FDE_FDE_EDGPARM;
//@}

/**
    Struct FDE scaling factor

    FDE scaling factor information
*/
//@{
typedef struct {
	UINT16  uiHIntFac;        ///< horizontal integer factor
	UINT16  uiHFrFac;         ///< horizontal fractional factor
	UINT16  uiVIntFac;        ///< vertical integer factor
	UINT16  uiVFrFac;         ///< vertical fractional factor
} FDE_SCLFAC;
//@}


/**
    Struct FDE Information.

    Set FDE information to execute.
*/
//@{
typedef struct {
	UINT32  uiInAddr;           ///< input starting address
	UINT32  uiOutAddr0;         ///< output starting address 0
	UINT32  uiOutAddr1;         ///< output starting address 1
	UINT32  uiOutAddr2;         ///< output starting address 2
	UINT32  uiWidth;            ///< image width
	UINT32  uiHeight;           ///< image height
	UINT32  uiInOfs;            ///< image input lineoffset
	UINT32  uiOutOfs;           ///< image output lineoffset
	FDE_FDOPMODE  OpMode;       ///< operation mode
	UINT32  uiSclNum;           ///< scaling number
	UINT32  uiShf0;             ///< channel 0 shift
	UINT32  uiShf1;             ///< channel 1 shift
	UINT32  uiShf2;             ///< channel 2 shift
	//UINT32  uiHFact;            ///< horizontal scaling factor
	//UINT32  uiVFact;            ///< vertical scaling factor
	FDE_SCLFAC  *pSclFac;       ///< scaling factors
	//FDE_FDE_THPRAM *pTrgTh;     ///< target 0 threshold
	//FDE_FDE_THPRAM *pTrgTh2;    ///< target 1 threshold
	//FDE_FDE_THPRAM *pTrgTh3;    ///< REMOVED in NT96460
	//UINT32  *puiTarget;         ///< face target 0
	//UINT32  *puiTarget2;        ///< face target 1
	//UINT32  *puiTarget3;        ///< REMOVED in NT96460
	FDE_TGTPARAM *pTrgTh0;      ///< target 0 parameters
	FDE_TGTPARAM *pTrgTh1;      ///< target 1 parameters
	FDE_TGTPARAM *pTrgTh2;      ///< target 2 parameters
	FDE_TGTPARAM *pTrgTh3;      ///< target 3 parameters
	FDE_TGTPARAM *pTrgTh4;      ///< target 4 parameters
	FDE_TARTET *pTarget;      ///< targets
	FDE_TGTSTAT *pTgtStat0;   ///< target 0 statistics
	FDE_TGTSTAT *pTgtStat1;   ///< target 1 statistics
	FDE_TGTSTAT *pTgtStat2;   ///< target 2 statistics
	FDE_TGTSTAT *pTgtStat3;   ///< target 3 statistics
	FDE_TGTSTAT *pTgtStat4;   ///< target 4 statistics
	FDE_ORIENT Orient0;         ///< face orient 0
	FDE_ORIENT Orient1;         ///< face orient 1
	FDE_ORIENT Orient2;         ///<  face orient 2
	FDE_ORIENT Orient3;         ///<  face orient 3
	FDE_ORIENT Orient4;         ///<  face orient 4
	FDE_FDE_EDGPARM *pEdgKer;   ///< edge kernal 0
	FDE_FDE_EDGPARM *pEdgKer2;  ///< edge kernal 1
	FDE_RSLT_SEL RsltSel;       ///< result format
	FDE_FDOPP   Opp;            ///< output ping-pong buffer enable
	//UINT8   uiOrientPri;        ///< orientation piority
	//FDE_LPF_PRAM RsltLpf;       ///< result low pass parameter
	FDE_SCORE_SEL ScrSel;     ///< select score calculation
	UINT32 uiScrWt;             ///< score weighting
	FDE_POWERSAVING Cor2En;     ///< correlation2 enable
} FDE_FDE_PRAM;
//@}

/**
    Struct FDE Location and Score

    Information of face result
*/
//@{
typedef struct {
	UINT16  uiStx;            ///< result start x
	UINT16  uiSty;            ///< result start y
	UINT16  uiWidth;          ///< result width
	UINT16  uiHeight;         ///< result height
	UINT8   uiScore;          ///< result score
	UINT8   uiTrgIdx;         ///< target index
	UINT32  uiOverlapCount;   ///< number of overlapped faces
	UINT32  uiTotalScore;     ///< total score
} FACE;
//@}

/**
    Struct FDE Location, Score and Correlation

    All information of face results
*/
//@{
typedef struct {
	UINT16  uiStx;            ///< result start x
	UINT16  uiSty;            ///< result start y
	UINT16  uiSclnum;         ///< result sclnum
	UINT8   uiScore;          ///< result score
	UINT8   uiScore1;         ///< result score1
	UINT8   uiScore2;         ///< result score2
	UINT8   uiTrgIdx;         ///< target index
	UINT8   uiCor0;           ///< result cor0
	UINT8   uiCor1;           ///< result cor1
	UINT8   uiCor2;           ///< result cor2
	UINT8   uiCor3;           ///< result cor3
	UINT8   uiCor4;           ///< result cor4
	UINT8   uiCor5;           ///< result cor5
} FACEINFO;
//@}

/**
    Struct FDE input size infor

    FDE input size information
*/
//@{
typedef struct {
	UINT16  uiWidth;          ///< input width
	UINT16  uiHeight;         ///< input height
	UINT16  uiInOfs;          ///< input lineoffset
	UINT16  uiOutOfs;         ///< output lineoffset
} FACESIZEINFOR;
//@}


extern ER       fde_start(VOID);
extern ER       fde_setFDMode(FDE_FDFUNC Mode, FDE_FDE_PRAM *pFdeInfo);
extern ID       fde_getLockStatus(VOID);
extern ER       fde_open(FDE_OPENOBJ *pObjCB);
extern BOOL     fde_isOpened(VOID);
extern ER       fde_close(VOID);
extern ER       fde_enableInt(UINT32 uiIntr);
extern UINT32   fde_getIntEnable(VOID);
extern UINT32   fde_getIntStatus(VOID);
extern ER       fde_clearInt(UINT32 uiIntr);
extern ER       fde_setRsltNum(UINT32  uiNum);
extern UINT32   fde_getDmaInAddr(VOID);
extern UINT32   fde_getDmaOutAddr0(VOID);
extern UINT32   fde_getDmaOutAddr1(VOID);
extern UINT32   fde_getDmaOutAddrRslt(VOID);
extern UINT32   fde_getRsltNum(FDE_RSLT_OVLP Overlap);
extern ER       fde_getInSizeInfor(FACESIZEINFOR *pSizeInfor);
extern ER       fde_getScaleFactor(FDE_SCLFAC *pFactor);
extern UINT32   fde_getScaleNum(VOID);
extern FDE_FDOPP fde_getOPPMode(VOID);
//extern ER fde_modParam(FDE_ORIENT Orient0, UINT32 uiIndex, UINT32 uiValue);
extern FDE_FDOPMODE fde_getOpMode(VOID);
extern UINT32 fde_getClockRate(VOID);
extern ER fde_getFaceRslt(FACEINFO *pFaceInfo, UINT32 uiAddr, FDE_RSLT_SEL Sel);
#endif

//@}
