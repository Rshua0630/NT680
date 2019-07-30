/**
    IPH module (DCE type define)

    @file       ipl_hal_dce.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_DCE_H
#define _IPL_Hal_DCE_H
#include "ipl_hal_dce_info.h"

/** \addtogroup mILibIPH */
//@{

typedef enum _IPH_DCE_QUE {
	ENUM_DUMMY4WORD(IPH_DCE_QUE)
} IPH_DCE_QUE;

/**
     enum of DCE update selection
*/
typedef enum _DCE_UPDATE {
	DCE_D2D_SET        = 0x00000001,    ///< update D2D Information.
	DCE_IO_SIZE        = 0x00000002,    ///< update in size/lineoffset
	DCE_CROP_SIZE      = 0x00000004,    ///< update in size/lineoffset
	DCE_STRIPE         = 0x00000008,    ///< update Stripe Increment/decrement pixel number/stripe overlap selection/stripe table
	DCE_CENTER         = 0x00000010,    ///< update distortion center
	DCE_DIST           = 0x00000020,    ///< update H/V distance factor/ratio selection
	DCE_RADIUS         = 0x00000040,    ///< update radius norm term
	DCE_CAB            = 0x00000080,    ///< update color aberration factor
	DCE_LUT            = 0x00000100,    ///< update LUT table address
	DCE_FOV            = 0x00000200,    ///< update FOV
	DCE_ENH            = 0x00000400,    ///< update enhancement
	DCE_H_FILT         = 0X00001000,    ///< update h direction filter enable/disable(UV only)
	DCE_DISTOR         = 0x00002000,    ///< update distortion parameter set (must be include DCE_ENH, DCE_FOV, DCE_LUT, DCE_CAB, DCE_CENTER)
	DCE_CFAINFO        = 0x00004000,    ///< update cfa interpolation
	DCE_2DLUT          = 0x00008000,    ///< update 2D LUT information.
	DCE_INIT           = 0x00010000,    ///< init DCE
	DCE_SUBFEN         = 0x00020000,    ///< update function switch
	DCE_CALSTRP        = 0x00040000,    ///< only valid in _DCE_Load flow, to notify calculate dce stripe
	DCE_ST             = 0x00080000,    ///< SST/MST/2ST  (only used for runtime change, otherwise DCE_INIT)
	DCE_CFAPAT         = 0x00100000,    ///< update cfa pattern
	ENUM_DUMMY4WORD(DCE_UPDATE)
} DCE_UPDATE;

/**
     enum of DCE read selection

     read from inner table, not from hw register.
*/
typedef enum _DCE_READ {
	DCE_R_IO        = 0x00000001,   ///< read in/out info. exclude DCE_SUBFUNC,             Data type: DCE
	DCE_R_SUBFEN    = 0x00000002,   ///< read func enable/disable,                          Data type: DceFuncEn
	DCE_R_CENTER    = 0X00000003,   ///< read distortion center,                            Data type: Coordinate
	DCE_R_DIST      = 0x00000004,   ///< read H/V distance factor/ratio selection,          Data type: DIST
	DCE_R_RADIOUS   = 0X00000005,   ///< read radius norm term,                             Data type: Fact_Norm
	DCE_R_CAB       = 0x00000006,   ///< read color aberration factor,                      Data type: Aberation
	DCE_R_LUT       = 0X00000007,   ///< read LUT table address,                            Data type: UINT32
	DCE_R_FOV       = 0X00000008,   ///< read FOV,                                          Data type: FOV
	DCE_R_ENH       = 0X00000009,   ///< read enhancement,                                  Data type: EnH
	DCE_R_INTEEN    = 0x0000000A,   ///< read interruppt enabe bit                          Data type: DceInteEn
	DCE_R_LUTTYPE   = 0x0000000B,   ///< read dce lut table type(inc/dec, none)             Data type: DCE_LutType
	DCE_R_CFA       = 0x0000000C,   ///< read CFA parameters,                               Data type: DCE_CFAPARAM
	DCE_R_D2D       = 0x0000000D,   ///< read D2D information                               Data type: DCE
	DCE_R_STRP      = 0x0000000E,   ///< read stripe information                            Data type: DCE_Stripe
	DCE_R_STRPNUM   = 0x0000000F,   ///< read stripe number(1/2/4/m strp)                   Data type: UINT32
	ENUM_DUMMY4WORD(DCE_READ)
} DCE_READ;

/**
    DCE stripe configuration
*/
typedef struct _DCE_Stripe {
	UINT32  *stripeTbl;             ///< stripe table for dce register, tab num: 16, Data type: UINT32
	UINT32  IncNum;                 ///< max input horizontal stripe Increment pixel number
	UINT32  DecNum;                 ///< max input horizontal stripe Decrement pixel number
	enum {
		First_STP_POS_OFS   = 0,    ///< Input starts from DCE calculated position
		First_STP_POS_NONOFS = 1,   ///< Input starts from 0
		ENUM_DUMMY4WORD(StrPos)
	} StrPos;                       ///< Input first stripe start position selection

	enum {
		IPE_H_OVERLAP_8     = 0,        ///< overlap  8  pixels, default
		IPE_H_OVERLAP_16    = 1,        ///< overlap  16 pixels, if ipe eth enabled
		ENUM_DUMMY4WORD(IPEOut_STP_Overlap)
	} IPEOut_STP_Overlap;              ///< Output stripe overlap selection

	enum {
		IME_H_OVERLAP_16    = 0,        ///< overlap 16 pixels
		IME_H_OVERLAP_24    = 1,        ///< -
		IME_H_OVERLAP_32    = 2,        ///< -
		IME_H_OVERLAP_USER  = 3,        ///< user define value(used when 3dnr+encode+decode)
		ENUM_DUMMY4WORD(IMEOut_STP_Overlap)
	} IMEOut_STP_Overlap;               ///< Output stripe overlap selection
	UINT32 ImeOverlapVal;               ///< user-defined overlap value

	UINT32 ImeSsrEn;                 ///< extend information for calculate STRIPE
	UINT32 ImeIsdEn;                 ///< extend information for calculate STRIPE
	UINT32 ImeEncEn;                 ///< extend information for calculate STRIPE
	UINT32 ImeDecEn;                 ///< extend information for calculate STRIPE
	UINT32 Ime3dnrEn;                ///< extend information for calculate STRIPE

	UINT32 *stripe_size;            ///< read only stripe size real stripe size,  tab num: 16, Data type: UINT32
	UINT32  stripe_num;             ///< read only stripe number
} DCE_Stripe;

/**
    DCE distortion correction selection

    Select distortion correction mode
\n Used for dce_setDistortCorrSel().
*/
typedef enum {
	DCSEL_GDC_ONLY              = 0,    ///< GDC/CAC only
	DCSEL_2DLUT_ONLY            = 1,    ///< 2D lut only
	DCSEL_GDC_2DLUT_DIFFSUM     = 2,    ///< GDC/CAC, 2D grid LUT difference addition
	DCSEL_GDC_2DLUT_CASCADE     = 3,    ///< 2D grid LUT cascade with GDC/CAC
	ENUM_DUMMY4WORD(DC_SEL)
} DC_SEL;

/**
    DCE structure - DCE D2D parameters.
*/
//@{
typedef struct {
	enum {
		_D2D_YUV422IN_YUV422OUT    = 0,    ///< In/Out YUV 422 8 bit packed plane
		_D2D_YUV420IN_YUV420OUT    = 1,    ///< In/Out YUV 420 8 bit packed plane
		_D2D_BAYERIN_YUV422OUT     = 2,    ///< In Bayer 8 bit plane/Out YUV 422 8 bit packed plane
		ENUM_DUMMY4WORD(DCE_D2DFMT)
	} DCE_D2DFMT;
	struct {
		UINT32  Addr_Y;             ///< -
		UINT32  Addr_UV;            ///< -
		UINT32  Lineoffset_Y;       ///< -
		UINT32  Lineoffset_UV;      ///< -
	} Dram_In;                      ///< source which input from dram
	struct {
		UINT32  Addr_Y;             ///< -
		UINT32  Addr_UV;            ///< -
		UINT32  Lineoffset_Y;       ///< -
		UINT32  Lineoffset_UV;      ///< -
	} Dram_Out;                     ///< output data to dram
} DCE_D2D_SETTING;
/**
    @name DCE struct - Output cropping
*/
//@{
typedef struct {
	UINT32    uiSizeH;    ///< Horizontal image size, unit : pix
	UINT32    uiSizeV;    ///< Vertical image size, uint : line
	UINT32    uiStartH;    ///< Horizontal image size, unit : pix
	UINT32    uiStartV;    ///< Vertical image size, uint : line
} OUT_CROP;
//@}
/**
     DCE configuration

     all DCE configuration including all DCE sub function
*/

typedef struct _DCE_Manager {
	DCE_SRCMODE DCEMode;

	enum {
		LBUF_BAYER   = 0,                 ///< DCE Bayer buffer mode
		LBUF_RGB     = 1,                 ///< DCE RGB buffer mode
		ENUM_DUMMY4WORD(DCE_LBUF)
	} DCE_LBUF;
	enum {
		STPMODE_SST     = 0,              ///< DCE single stripe mode
		STPMODE_MST     = 1,              ///< DCE horizontal multiple stripe mode
		STPMODE_2ST     = 2,              ///< DCE horizontal 2 stripe mode
		STPMODE_4ST     = 3,              ///< DCE horizontal 4 stripe mode
		STPMODE_UST     = 4,              ///< DCE user defined horizontal stripe mode
		ENUM_DUMMY4WORD(DCE_STP)
	} DCE_STP;
	USIZE       Size;                   ///< in size
	OUT_CROP    Out_Crop_Size;          ///< Out crop size
	DCE_Stripe  Stripe;                 ///< stripe configuration
	DCE_CFA     Cfa_pat;                ///< DCE input Bayer data start CFA selection
	DC_SEL      Distortion_Sel;         ///< Distortion correction combination selection
	DCE_D2D_SETTING D2d_setting;        ///< D2D parameters
	DCE_SUBFUNC *pSubFunc;              ///< -
} DCE_Manager, *pDCE_Manager;

/**
     DCE configuration
*/
typedef DCE_Manager DCE;

//@}
#endif
