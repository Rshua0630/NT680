/**
    DPS (PictBridge) Functions for DSC system.

    @file       DPSFuncs.h
    @ingroup    mIUSIDC

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

*/

#ifndef _DPSFUNCS_H
#define _DPSFUNCS_H

#include "Type.h"
#include "SysKer.h"
#include "Debug.h"
#include "USBSIDC2.h"

/**
    @addtogroup mIUSIDC
*/
//@{

/**
     @name CROPPING function
     Indicate the cropping function is enabled or not.
*/
#define _DPS_CROPPING_              1

/**
     @name Layout Support
     1: support all 253 layouts in pictb-spec.\n
     0: support normal 14 layouts in pictb-spec.
*/
//@{
#define _DPS_LAYOUT_TBL_            0
//Support 1UP-250UP layout
#if _DPS_LAYOUT_TBL_
#define  _DPS_LAYOUT_SUPP250UP_         253  //support all 253 layouts in pictb-spec.
#else
#define  _DPS_LAYOUT_SUPP250UP_         0    //support normal 14 layouts in pictb-spec.
#endif
//@}

/**
     @name PicBridge flag
*/
//@{
#define FLGDPS_ABORTJOB_GO      0x00010000      // get datePrints
#define FLGDPS_NEWJOBOK         0x00020000      // get croppings
#define FLGDPS_FIRSTPTPCMD      0x00040000
#define FLGDPS_PRINTOK          0x00080000      // print complete
#define FLGDPS_INITOK           0x00200000      // init complete
#define FLGDPS_INITFAIL         0x00400000      // init fail ! (not PTP)
#define FLGDPS_FAIL             0x01000000      // get error
#define FLGDPS_ZEROHANDLE       0x02000000      // get error
#define FLGDPS_CAPTUREOK        0x04000000      // capture OK!
#define FLGDPS_EP3START         0x10000000      // start to send ep3 data
#define FLGDPS_EP3END           0x20000000
#define FLGDPS_PRINTFAIL        0x80000000      // printer error
#define FLGDPS_QUALITY          0x00000001      // get quality
#define FLGDPS_PAPERSIZE        0x00000002      // get papersize
#define FLGDPS_PAPERTYPE        0x00000004
#define FLGDPS_FILETYPE         0x00000008
#define FLGDPS_DATEPRINT        0x00000010
#define FLGDPS_NAMEPRINT        0x00000020
#define FLGDPS_LAYOUT           0x00000040
#define FLGDPS_FIXEDSIZE        0x00000080
#define FLGDPS_STARTCANCEL      0x00000100
#define FLGDPS_RSPOK            0x00000200
#define FLGDPS_STATUS_OK        0x00000400
#define FLGDPS_STATUS_FAIL      0x00000800
#define FLGDPS_CALLBACK         0x00001000
#define FLGDPS_CANCELOK         0x00002000
#define FLGDPS_STARTJOB         0x00004000
#define FLGDPS_STOPJOB          0x00008000
#define FLGDPS_1SECOND          0x40000000
#define FLGDPS_IMGOPT           0x00100000
#if _DPS_CROPPING_
#define FLGDPS_CROPPING         0x08000000  //cropping
#endif
//@}

/**
     @name SIDC Host Type
*/
//@{
#define SIDC_UNKNOWN            0   ///< Unknown host.
#define SIDC_PTR                1   ///< Host is a printer.
#define SIDC_PC                 2   ///< Host is a PC.
//@}

/**
     @name SIDC callback event for USIDCPTR_CB
*/
//@{
#define USIDC_PRINTIMG_OK       0x01
#define USIDC_PRINTIMG_FAIL     0x02
#define USIDC_PRINTIMG_WARNING  0x03

#define USIDC_PRINTIMG_CONTINUE 0x04
#define USIDC_PRINTERR_PAPER    0x31
#define USIDC_PRINTERR_INK      0x32
#define USIDC_PRINTERR_HW       0x33
#define USIDC_PRINTERR_FILE     0x34
#define USIDC_PTP_FW_UPDATE_UPDATE_CODE_START      0x41
#define USIDC_PTP_FW_UPDATE_UPDATE_CODE_PROGRESS   0x42
#define USIDC_PTP_FW_UPDATE_UPDATE_CODE_DONE       0x43
#define USIDC_PTP_FW_UPDATE_ERASING_FSYS_START     0x44
#define USIDC_PTP_FW_UPDATE_ERASING_FSYS_DONE      0x45
#define USIDC_PTP_FW_UPDATE_VERIFYING_NEW_FW_START 0x46
#define USIDC_PTP_FW_UPDATE_VERIFYING_NEW_FW_DONE  0x47
#define USIDC_PTP_FW_UPDATE_FORMATTING_FSYS_START  0x48
#define USIDC_PTP_FW_UPDATE_FORMATTING_FSYS_DONE   0x49
#define USIDC_PTP_FW_UPDATE_COMPLETE               0x4A
#define USIDC_PTP_FW_UPDATE_FAIL                   0x4B

#define USIDC_REQUEST_STATUS_RSP    0x70
#define USIDC_REQUEST_NSTATUS_RSP   0x71
#define USIDC_REQUEST_NOSUPPORT     0x72
#define USIDC_REQUEST_NORECOGNIZE   0x75
#define USIDC_PRINT_NOTIFYJOB       0x76
#define USIDC_REQUEST_1WRONG        0x79
#define USIDC_REQUEST_JSTATUS       0x7E  //job status
#define USIDC_REQUEST_DSTATUS       0x7F  // device status
#define USIDC_CANCEL_OK             0x90
#define USIDC_START_CANCEL          0x91

#define USIDC_CONT_JOB_OK           0x92  //ContinueJob call back constant
#define USIDC_CONT_JOB_FAIL         0x93  //ContinueJob call back constant

#define USIDC_DISCONN_ENABLE        0x94
#define USIDC_CAP_CHANGED           0x95

#define USIDC_PRINTIMG_FATALFAIL    0xFD  //Not execute printing
#define USIDC_PRINTING_NOW          0xFC
#define USIDC_CANCEL_FAIL           0x96

#define USIDC_WARNING_PAPER_ERROR   0xB0 //00010000
#define USIDC_ERROR_PAPER_ERROR     0xB1
#define USIDC_WARNING_PAPER_OUT     0xB2 //00010100
#define USIDC_ERROR_PAPER_OUT       0xB3
#define USIDC_WARNING_PAPER_LOAD    0xB4 //00010200
#define USIDC_ERROR_PAPER_LOAD      0xB5
#define USIDC_WARNING_PAPER_JAM     0xB6 //00010300, 00010400, 00010500
#define USIDC_ERROR_PAPER_JAM       0xB7
#define USIDC_WARNING_PAPER_NOMSG   0xB8 //00010600
#define USIDC_ERROR_PAPER_NOMSG     0xB9
#define USIDC_WARNING_PAPER_NOSUP   0xBA //00010700
#define USIDC_ERROR_PAPER_NOSUP     0xBB
#define USIDC_WARNING_INK_ERROR     0xBC //00020000
#define USIDC_ERROR_INK_ERROR       0xBD
#define USIDC_WARNING_INK_OUT       0xBE //00020100
#define USIDC_ERROR_INK_OUT         0xBF
#define USIDC_WARNING_INK_LOW       0xC0 //00020200
#define USIDC_ERROR_INK_LOW         0xC1
#define USIDC_WARNING_PTR_ERROR     0xC2 //00030000, 00030100, 00030200, 00030500
#define USIDC_ERROR_PTR_ERROR       0xC3
#define USIDC_WARNING_PTR_UC        0xC4 //00030300, 00030400
#define USIDC_ERROR_PTR_UC          0xC5
#define USIDC_WARNING_PTR_OPEN      0xC6 //00030600
#define USIDC_ERROR_PTR_OPEN        0xC7
#define USIDC_WARNING_PTR_HEAD      0xC8 //00030700
#define USIDC_ERROR_PTR_HEAD        0xC9
#define USIDC_WRANING_PTR_INK       0xCA //00030800
#define USIDC_ERROR_PTR_INK         0xCB
#define USIDC_WARNING_PTR_CART      0xCC //00030900
#define USIDC_ERROR_PTR_CART        0xCD
#define USIDC_WARNING_FILE_ERROR    0xCE //00040000
#define USIDC_ERROR_FILE_ERROR      0xCF

#define USIDC_PC_XFER               0xA0
#define USIDC_PC_XFEROK             0xA4

#define USIDC_PC_OPEN               0xF5
#define USIDC_PC_CLOSE              0xFA

#define USIDC_CLOSE_SESSION         0xFE
//@}

/**
     @name Capability Items
*/
//@{
#define DPS_CAPITEM_QUALITY         1//50
#define DPS_CAPITEM_PAPERSIZE       2//51
#define DPS_CAPITEM_PAPERTYPE       3//52
#define DPS_CAPITEM_FILETYPES       4//53
#define DPS_CAPITEM_DATEPRINTS      5//54
#define DPS_CAPITEM_NAMEPRINTS      6//55
#define DPS_CAPITEM_CROPPINGS       7//59
#define DPS_CAPITEM_LAYOUTS         8//57
#define DPS_CAPITEM_IMGOPT          10//56
#define DPS_CAPITEM_FIXSIZE         9//58
#define DPS_CAPITEM_PAPERSIZE_MORE    11
//°Ï¤À1L,2L¤Î100RO,210RO
#define DPS_CAPITEM_LAYOUTS_MORE      12
#define DPS_CAPITEM_LAYOUTS_250UP           13
#define DPS_CAPITEM_LAYOUTS_MORE_250UP      14
//@}

/**
     @name Parameters for capability qualities
*/
//@{
//50000000
#define DPS_CAP_QUALITY_DEF         0x01 //default
#define DPS_CAP_QUALITY_NOR         0x02 //normal
#define DPS_CAP_QUALITY_DRA         0x04 //draft
#define DPS_CAP_QUALITY_FIN         0x08 //fine
//@}
/**
     @name Parameters for capability paperSizes
*/
//@{
//51000000
#define DPS_CAP_PAPERSIZE_TOTAL     34 // total number of paper size
#define DPS_CAP_PAPERSIZE_DEF       0x00000001 //default
#define DPS_CAP_PAPERSIZE_01L       0x00000002 // L 89mmx127mm
#define DPS_CAP_PAPERSIZE_02L       0x00000004 // 2L 127mmx178mm
#define DPS_CAP_PAPERSIZE_POS       0x00000008 // postcard 100x148mm
#define DPS_CAP_PAPERSIZE_CARD      0x00000010 // card 54x85.6mm
#define DPS_CAP_PAPERSIZE_150       0x00000020 // 100x150mm
#define DPS_CAP_PAPERSIZE_4x6       0x00000040 // 4x6 101x152mm
#define DPS_CAP_PAPERSIZE_8x10      0x00000080 // 8x10 203x254mm
#define DPS_CAP_PAPERSIZE_LET       0x00000100 // letter 216x279mm
#define DPS_CAP_PAPERSIZE_11x17     0x00000200 // 11x17 279x431mm
#define DPS_CAP_PAPERSIZE_A0        0x00000400
#define DPS_CAP_PAPERSIZE_A1        0x00000800
#define DPS_CAP_PAPERSIZE_A2        0x00001000
#define DPS_CAP_PAPERSIZE_A3        0x00002000
#define DPS_CAP_PAPERSIZE_A4        0x00004000
#define DPS_CAP_PAPERSIZE_A5        0x00008000
#define DPS_CAP_PAPERSIZE_A6        0x00010000
#define DPS_CAP_PAPERSIZE_A7        0x00020000
#define DPS_CAP_PAPERSIZE_A8        0x00040000
#define DPS_CAP_PAPERSIZE_A9        0x00080000
#define DPS_CAP_PAPERSIZE_B0        0x00100000
#define DPS_CAP_PAPERSIZE_B1        0x00200000
#define DPS_CAP_PAPERSIZE_B2        0x00400000
#define DPS_CAP_PAPERSIZE_B3        0x00800000
#define DPS_CAP_PAPERSIZE_B4        0x01000000
#define DPS_CAP_PAPERSIZE_B5        0x02000000
#define DPS_CAP_PAPERSIZE_B6        0x04000000
#define DPS_CAP_PAPERSIZE_B7        0x08000000
#define DPS_CAP_PAPERSIZE_B8        0x10000000
#define DPS_CAP_PAPERSIZE_B9        0x20000000
#define DPS_CAP_PAPERSIZE_89RO      0x40000000 //89mm roll
#define DPS_CAP_PAPERSIZE_127RO     0x80000000 //127mm roll

//paperSizeMore, over 32 paper size
#define DPS_CAP_PAPERSIZE_100RO     0x00000002 //100mm roll
#define DPS_CAP_PAPERSIZE_210RO     0x00000004 //210mm roll
//@}
/**
     @name Parameters for capability paperTypes
*/
//@{
//52000000
#define DPS_CAP_PAPERTYPE_DEF       0x01//default
#define DPS_CAP_PAPERTYPE_PLA       0x02//plain paper
#define DPS_CAP_PAPERTYPE_PHO       0x04//photo paper
#define DPS_CAP_PAPERTYPE_FPHO      0x08//fast photo
//@}
/**
     @name Parameters for capability fileTypes
*/
//@{
//53000000
#define DPS_CAP_FILETYPES_DEF       0x01
#define DPS_CAP_FILETYPES_EXIFJ     0x02 //exif/jpeg
#define DPS_CAP_FILETYPES_EXIF      0x04 //exif
#define DPS_CAP_FILETYPES_JPEG      0x08 //jpeg
#define DPS_CAP_FILETYPES_JFIF      0x10 // jfif
#define DPS_CAP_FILETYPES_ASSO      0x20 // association
#define DPS_CAP_FILETYPES_DPOF      0x40 // DPOF
//@}
/**
     @name Parameters for capability datePrints
*/
//@{
//54000000
#define DPS_CAP_DATEPRINT_DEF       0x01 // default
#define DPS_CAP_DATEPRINT_OFF       0x02 // off
#define DPS_CAP_DATEPRINT_ON        0x04 // on
//@}
/**
     @name Parameters for capability fileNamePrints
*/
//@{
//55000000
#define DPS_CAP_NAMEPRINT_DEF       0x01
#define DPS_CAP_NAMEPRINT_OFF       0x02
#define DPS_CAP_NAMEPRINT_ON        0x04
//@}
/**
     @name Parameters for capability imageOptimizes
*/
//@{
//56000000
#define DPS_CAP_IMGOPT_DEF          0x01
#define DPS_CAP_IMGOPT_OFF          0x02
#define DPS_CAP_IMGOPT_ON           0x04
//@}
/**
     @name Parameters for capability layouts
*/
//@{
//57000000
#define DPS_CAP_LAYOUT_DEF          0x0001 //default
#define DPS_CAP_LAYOUT_01           0x0002 ///1-up
#define DPS_CAP_LAYOUT_02           0x0004 //2-up
#define DPS_CAP_LAYOUT_03           0x0008 //3-up
#define DPS_CAP_LAYOUT_04           0x0010 //4-up
#define DPS_CAP_LAYOUT_05           0x0020 //5-up
#define DPS_CAP_LAYOUT_06           0x0040 //6-up
#define DPS_CAP_LAYOUT_07           0x0080 //7-up
#define DPS_CAP_LAYOUT_08           0x0100 //8-up
#define DPS_CAP_LAYOUT_09           0x0200 //9-up
#define DPS_CAP_LAYOUT_10           0x0400 //16-up
#define DPS_CAP_LAYOUT_250UP        0x0800 //250-up
#define DPS_CAP_LAYOUT_IDX          0x1000 //index print
#define DPS_CAP_LAYOUT_NOB          0x2000 //borderless

#define DPS_CAP_LAYOUT_14           0x100000 //20-up
//@}
/**
     @name Parameters for capability fixedSized
*/
//@{
#define DPS_CAP_FIXSIZE_DEF         0x00000001//5800
#define DPS_CAP_FIXSIZE_2x3         0x00000002//5801
#define DPS_CAP_FIXSIZE_3x5         0x00000004//5802
#define DPS_CAP_FIXSIZE_4x6         0x00000008//5803
#define DPS_CAP_FIXSIZE_5x7         0x00000010//5804
#define DPS_CAP_FIXSIZE_8x10        0x00000020//5805
#define DPS_CAP_FIXSIZE_254         0x00000040//5806
#define DPS_CAP_FIXSIZE_110         0x00000080//5807
#define DPS_CAP_FIXSIZE_89          0x00000100//5808
#define DPS_CAP_FIXSIZE_100         0x00000200//5809
#define DPS_CAP_FIXSIZE_6_8         0x00000400//580A
#define DPS_CAP_FIXSIZE_7_10        0x00000800//580B
#define DPS_CAP_FIXSIZE_9_13        0x00001000//580C
#define DPS_CAP_FIXSIZE_10_15       0x00002000//580D
#define DPS_CAP_FIXSIZE_13_18       0x00004000//580E
#define DPS_CAP_FIXSIZE_15_21       0x00008000//580F
#define DPS_CAP_FIXSIZE_18_24       0x00010000//5810
#define DPS_CAP_FIXSIZE_A4          0x00020000//5811
#define DPS_CAP_FIXSIZE_LETTER      0x00040000//5812

#define DPS_CAP_FIXSIZE_HP          0x00080000//58A08000

//@}
/**
     @name Parameters for capability croppings
*/
//@{
//59000000
#define DPS_CAP_CROPPINGS_DEF       0x01
#define DPS_CAP_CROPPINGS_OFF       0x02
#define DPS_CAP_CROPPINGS_ON        0x04
//@}

/**
    DPS Init Parameters.
*/
typedef struct _DPS_INIT_PARAMS {
	UINT32          qualities;       ///< all kinds of supported quality levels of a printer
	UINT32          paperSizes;      ///< all kinds of supported paper sizes of a printer
	UINT32          paperSizesMore;  ///< paper size extension
	UINT32          paperTypes;      ///< all kinds of supported paper types with default paper size
	UINT32          fileTypes;       ///< all kinds of supported file types of a printer
	UINT32          datePrints;      ///< whether a printer supports printing dates adjacent to images
	UINT32          fileNamePrints;  ///< whether a printer supports printing file names adjacent to images
	UINT32          layouts;         ///< all kinds of supported layouts with default paper size
	UINT32          croppings;       ///< whether a printer supports cropping
	UINT32          fixedSizes;      ///< all kinds of supported fixed sizes of a printer
	UINT32          imgOpti;         ///< whether a printer can enhance or optimize images
} DPS_INIT_PARAMS;


/**
    DPS Print Data.
*/
typedef struct _PB_PRINT_DATA {
	UINT32          handle;         ///< the image file handle hold by the File System.
	UINT16          dirID;          ///< ID of a directory where the image file is in
	UINT16          fileID;         ///< file id of the image file
	UINT16          quantity;       ///< copies of the image to be print
	UINT8           zeroSize;       ///< 1 means the image size is zero then it won't be printed
	UINT8           datePrints;     ///< print date or not
	UINT8           layouts;        ///< layout
	UINT8           papersize;      ///< paper size
	UINT8           fixedsize;      ///< fixed size
	UINT8           quality;        ///< quality
	UINT8           rev;            ///< reserved
	UINT16          date;           ///< date
	UINT8           filenamePrints; ///< print file name or not
	UINT8           rev2;           ///< reserved

#if _DPS_CROPPING_
	UINT16          cropX;          ///< the coordinate x of cropping
	UINT16          cropY;          ///< the coordinate y of cropping
	UINT16          cropW;          ///< the width of cropping area
	UINT16          cropH;          ///< the height of cropping area
#endif

} PB_PRINT_DATA, *PPB_PRINT_DATA;

/**
    DPS Print Information
*/
typedef struct {
	UINT16          total_images; ///< number of print image
	UINT16          total_prints; ///< number of paper print
	UINT8           total_fPrints;///< number of file name print
	UINT8           total_dPrints;///< number of date print
	UINT8           total_layouts;///< number of layout
	UINT8           rev1;         ///< reserved
} PB_PRINT_INFO, *PPB_PRINT_INFO;

/**
    Set capability of printer.

    Set a specific value to the specified capability.

    @param[in] capItem The item of capability.
    @param[in] number The value to be set to the specified capability.
    @return Return void

    Example:
    @code
    {
        PictB_SetPTRCapability(DPS_CAPITEM_DATEPRINTS, DPS_CAP_DATEPRINT_ON);
    }
    @endcode
*/
extern void     PictB_SetPTRCapability(UINT8 capItem, UINT32 number);

/**
    Get data of "print all".

    @param[out] startaddr Starting address.
    @param[out] totalImage Total images.
    @return Return void
*/
extern void     Sidc_GetPrintAllData(UINT32 *startaddr, UINT32 *totalImage);

/**
    Get PTP type

    @param[out] sidcType Sidc connection type (#SIDC_UNKNOWN/#SIDC_PTR/#SIDC_PC).
    @return Return void
*/
extern void     Sidc_GetPTPType(UINT8 *sidcType);

/**
    This function will print all images.

    @param[in] pPrintData Pointer to print data.
    @param[in] totalImages Total images could be printed, which is from the result of #Sidc_GetPrintAllData().
    @param[in] copies Copies of an image.
    @return none.
*/
extern void     PictB_PrintAllImages(PB_PRINT_DATA *pPrintData, UINT16 totalImages, UINT16 copies);

/**
    This function will print the specified image.

    @param[in] dirID DIR ID.
    @param[in] fileID File ID.
    @param[in] qua Quantity.
    @return none.
*/
extern void     PictB_PrintOneImage(UINT16 dirID, UINT16 fileID, UINT8 qua);

/**
    This function will print one or more pictures.

    Set date-print and file-name-print for each individual picture.

    @param[in] pPrintData Quantities of each image.
    @param[in] totalImages Total images.
    @return none.
*/
extern void     PictB_PrintMoreImagesEachSetting(PB_PRINT_DATA *pPrintData, UINT16 totalImages);

/**
    Get printer capability items.

    @param[out] pDPSGetCap Pointer to DPS Capability: #_DPS_INIT_PARAMS.
    @return Return void.
*/
extern void     PictB_GetCapabilityItem(DPS_INIT_PARAMS *pDPSGetCap);

/**
    Print Images defined in DPOF file

    @return Return void.
*/
extern void     PictB_PrintDPOF(void);

/**
    Abort Print Image.
    @return Return void.
*/
extern void     PictB_PrintImageAbort(void);

/**
    Register a callback funtion.

    @param[in] CB callback function.
*/
extern void     PictB_RegCBFunction(USIDCPTR_CB CB);

/**
    Get current Job status.

    @param[out] nowP current progress
    @param[out] totalP total images
    @return Return void
*/
extern void     PictB_GetPTRnotifyJob(UINT16 *nowP, UINT16 *totalP);

/**
    Get printer Job status.

    @return
        - @b 0: OK.
        - @b 1: Fail.
*/
extern UINT8    PictB_GetPTRJobStatus(void);

/**
    Continue Print Image.

    @return Return void.
*/
extern void     PictB_PrintImageContinue(void);

/**
    Get current printer status.

    @return
        - @b 0: OK.
        - @b 1: Fail.
*/
extern UINT8    PictB_GetPTRDeviceStatus(void);

/**
    Get layouts parameter for specified paper size.

    @param[in] number Specified paper size.
    @return Layout capability.

    Example:
    @code
    {
        PaperSize = DPS_CAP_PAPERSIZE_01L;
        PictB_SetPTRCapability(DPS_CAPITEM_PAPERSIZE, PaperSize);
        DBG_MSG("Layout for this paper size =0x%X\r\n",PictB_GetLayoutsSpecific(PaperSize));
    }
    @endcode
*/
extern UINT32   PictB_GetLayoutsSpecific(UINT32 number);

/**
    This function will print 1 or maxnum 999 pictures.

    Set date-print and file-name-print are for each individual picture.

    @param[in] pPrintData Pointer to print data.
    @param[in] totalImages Total images.
*/
extern void     PictB_PrintUpTo999Images(PB_PRINT_DATA *pPrintData, UINT16 totalImages);

/**
    Get Layouts parameter for specified paper size for 1L, 2L, 100RO and 210RO.

    @param[in] number Specified paper size.
    @return Layout capability.
*/
extern UINT32   PictB_GetLayoutsSpecificMore(UINT32 number);

/**
    Get paperTypes parameter for specific paper size.

    @param[in] number Specified paper size.
    @return Paper type.
*/
extern UINT32   PictB_GetPaperTypesSpecific(UINT32 number);

/**
    Get the max images limitation in a StartJob.

    @return The max images limitation.
*/
extern UINT32 PictB_GetPTRMaxImgCnt(void);

/**
    Printer device status
*/
typedef struct _PTR_DEV_STS_ {
	UINT32 prtServSts;    ///< Current print service status, refer to DPS_PRT_SERV_STS.
	UINT32 jobEndReason;  ///< Job end reason, refer to DPS_JOB_END_REASON.
	UINT32 errSts;        ///< Error status, refer to DPS_ERR_STS.
	UINT32 errReason;     ///< Error reason, refer to DPS_ERR_REASON.
	UINT32 disConnEn;     ///< Disconnect enable, refer to DPS_DISCONN_EN.
	UINT32 capChg;        ///< Capability changed, refer to DPS_CAP_CHG.
	UINT32 newJobOk;      ///< Is new job OK, refer to DPS_NEW_JOB_OK.
} PTR_DEV_STS, *PPTR_DEV_STS;

/**
    Current print service status
*/
typedef enum _DPS_PRT_SERV_STS {
	DPS_PRT_SERV_STS_PRINTING = 0, ///< Printing
	DPS_PRT_SERV_STS_IDLE,         ///< Idle
	DPS_PRT_SERV_STS_PAUSED        ///< Paused
} DPS_PRT_SERV_STS;

/**
    DPS job end reason.
*/
typedef enum _DPS_JOB_END_REASON {
	DPS_JOB_NOT_END = 0,    ///< Job not ended.
	DPS_JOB_END_NORMALLY,   ///< Job ended normally.
	DPS_JOB_END_ABORT1,     ///< Job ended due to DPS_AbortJob with "abortstyle"=900000000
	DPS_JOB_END_ABORT2,     ///< Job ended due to DPS_AbortJob with "abortstyle"=900100000
	DPS_JOB_END_OTHER       ///< Job ended due to other reason.
} DPS_JOB_END_REASON;

/**
    DPS error status
*/
typedef enum _DPS_ERR_STS {
	DPS_NO_ERROR = 0,       ///< No error.
	DPS_RECOVERABLE_ERROR,  ///< Warning (recoverable error).
	DPS_FATAL_ERROR         ///< Fatal error (unrecoverable error).
} DPS_ERR_STS;

/**
    DPS error reason
*/
typedef enum _DPS_ERR_REASON {
	DPS_NO_REASON = 0,  ///< No reason.
	DPS_PAPER_ERROR,    ///< Paper-related error.
	DPS_INK_ERROR,      ///< Ink-related error.
	DPS_HARDWARE_ERROR, ///< Hardware-related error.
	DPS_FILE_ERROR      ///< File-related error.
} DPS_ERR_REASON;

/**
    DPS disconnection enable.
*/
typedef enum _DPS_DISCONN_EN {
	DPS_DISCONN_FALSE = 0, ///< False: Cannont be disconnected.
	DPS_DISCONN_TRUE       ///< True: Can be disconnected.
} DPS_DISCONN_EN;

/**
    DPS capability change.
*/
typedef enum _DPS_CAP_CHG {
	DPS_CAPCHG_FALSE = 0, ///< False: Printer capability is unchanged.
	DPS_CAPCHG_TRUE       ///< True: Printer capability has changed.
} DPS_CAP_CHG;

/**
    DPS new job OK.
*/
typedef enum _DPS_NEW_JOB_OK {
	DPS_NEW_JOBOK_FALSE = 0, ///< False: Printer cannot accept a DPS_StartJob request now.
	DPS_NEW_JOBOK_TRUE       ///< TRUE: Printer can accept a DPS_StartJob request now.
} DPS_NEW_JOB_OK;

/**
    Get printer device status.
    @return Pointer to the structure of device status (#_PTR_DEV_STS_).
*/
extern PPTR_DEV_STS PictB_GetPtrDevSts(void);

/**
    Get the capability of specified item.

    @param[in] capItem Capability item.
    @return The capability of specified item.
*/
extern UINT32 PictB_GetPTREachCap(UINT8 capItem);

/**
    Get printer device status after disconnecting enabled.

    @return Pointer to the structure of device status (#_PTR_DEV_STS_).
*/
extern PPTR_DEV_STS PictB_GetPtrDevStsDisConnEn(void);


#if _DPS_LAYOUT_SUPP250UP_

/**
    DPS layout 250UP.
*/
typedef struct _DPS_LAYOUT_250UP_ {
	UINT32 layDef_Lay31;                      ///< default, 1up ~ 31up
	UINT32 lay32_Lay63;                       ///< 32up ~ 63up
	UINT32 lay64_Lay95;                       ///< 64up ~ 95up
	UINT32 lay96_Lay127;                      ///< 96up ~ 127up
	UINT32 lay128_Lay159;                     ///< 128up ~ 159up
	UINT32 lay160_Lay191;                     ///< 160up ~ 191up
	UINT32 lay192_Lay223;                     ///< 192up ~ 223up
	UINT32 lay224_LayBorderless;              ///< 224up ~ 250up, N/A, N/A, N/A, index,borderless
} DPS_LAYOUT_250UP, *PDPS_LAYOUT_250UP;

/**
     @name Layout index 250UP
*/
//@{
#define DPS_LAYOUT_IDX_DEF                                           0
#define DPS_LAYOUT_IDX_1UP                  DPS_LAYOUT_IDX_DEF    +  1
#define DPS_LAYOUT_IDX_2UP                  DPS_LAYOUT_IDX_1UP    +  1
#define DPS_LAYOUT_IDX_3UP                  DPS_LAYOUT_IDX_2UP    +  1
#define DPS_LAYOUT_IDX_4UP                  DPS_LAYOUT_IDX_3UP    +  1
#define DPS_LAYOUT_IDX_5UP                  DPS_LAYOUT_IDX_4UP    +  1
#define DPS_LAYOUT_IDX_6UP                  DPS_LAYOUT_IDX_5UP    +  1
#define DPS_LAYOUT_IDX_7UP                  DPS_LAYOUT_IDX_6UP    +  1
#define DPS_LAYOUT_IDX_8UP                  DPS_LAYOUT_IDX_7UP    +  1
#define DPS_LAYOUT_IDX_9UP                  DPS_LAYOUT_IDX_8UP    +  1
#define DPS_LAYOUT_IDX_10UP                 DPS_LAYOUT_IDX_9UP    +  1
#define DPS_LAYOUT_IDX_11UP                 DPS_LAYOUT_IDX_10UP   +  1
#define DPS_LAYOUT_IDX_12UP                 DPS_LAYOUT_IDX_11UP   +  1
#define DPS_LAYOUT_IDX_13UP                 DPS_LAYOUT_IDX_12UP   +  1
#define DPS_LAYOUT_IDX_14UP                 DPS_LAYOUT_IDX_13UP   +  1
#define DPS_LAYOUT_IDX_15UP                 DPS_LAYOUT_IDX_14UP   +  1
#define DPS_LAYOUT_IDX_16UP                 DPS_LAYOUT_IDX_15UP   +  1
#define DPS_LAYOUT_IDX_17UP                 DPS_LAYOUT_IDX_16UP   +  1
#define DPS_LAYOUT_IDX_18UP                 DPS_LAYOUT_IDX_17UP   +  1
#define DPS_LAYOUT_IDX_19UP                 DPS_LAYOUT_IDX_18UP   +  1
#define DPS_LAYOUT_IDX_20UP                 DPS_LAYOUT_IDX_19UP   +  1
#define DPS_LAYOUT_IDX_21UP                 DPS_LAYOUT_IDX_20UP   +  1
#define DPS_LAYOUT_IDX_22UP                 DPS_LAYOUT_IDX_21UP   +  1
#define DPS_LAYOUT_IDX_23UP                 DPS_LAYOUT_IDX_22UP   +  1
#define DPS_LAYOUT_IDX_24UP                 DPS_LAYOUT_IDX_23UP   +  1
#define DPS_LAYOUT_IDX_25UP                 DPS_LAYOUT_IDX_24UP   +  1
#define DPS_LAYOUT_IDX_26UP                 DPS_LAYOUT_IDX_25UP   +  1
#define DPS_LAYOUT_IDX_27UP                 DPS_LAYOUT_IDX_26UP   +  1
#define DPS_LAYOUT_IDX_28UP                 DPS_LAYOUT_IDX_27UP   +  1
#define DPS_LAYOUT_IDX_29UP                 DPS_LAYOUT_IDX_28UP   +  1
#define DPS_LAYOUT_IDX_30UP                 DPS_LAYOUT_IDX_29UP   +  1
#define DPS_LAYOUT_IDX_31UP                 DPS_LAYOUT_IDX_30UP   +  1
#define DPS_LAYOUT_IDX_32UP                 DPS_LAYOUT_IDX_31UP   +  1
#define DPS_LAYOUT_IDX_33UP                 DPS_LAYOUT_IDX_32UP   +  1
#define DPS_LAYOUT_IDX_34UP                 DPS_LAYOUT_IDX_33UP   +  1
#define DPS_LAYOUT_IDX_35UP                 DPS_LAYOUT_IDX_34UP   +  1
#define DPS_LAYOUT_IDX_36UP                 DPS_LAYOUT_IDX_35UP   +  1
#define DPS_LAYOUT_IDX_37UP                 DPS_LAYOUT_IDX_36UP   +  1
#define DPS_LAYOUT_IDX_38UP                 DPS_LAYOUT_IDX_37UP   +  1
#define DPS_LAYOUT_IDX_39UP                 DPS_LAYOUT_IDX_38UP   +  1
#define DPS_LAYOUT_IDX_40UP                 DPS_LAYOUT_IDX_39UP   +  1
#define DPS_LAYOUT_IDX_41UP                 DPS_LAYOUT_IDX_40UP   +  1
#define DPS_LAYOUT_IDX_42UP                 DPS_LAYOUT_IDX_41UP   +  1
#define DPS_LAYOUT_IDX_43UP                 DPS_LAYOUT_IDX_42UP   +  1
#define DPS_LAYOUT_IDX_44UP                 DPS_LAYOUT_IDX_43UP   +  1
#define DPS_LAYOUT_IDX_45UP                 DPS_LAYOUT_IDX_44UP   +  1
#define DPS_LAYOUT_IDX_46UP                 DPS_LAYOUT_IDX_45UP   +  1
#define DPS_LAYOUT_IDX_47UP                 DPS_LAYOUT_IDX_46UP   +  1
#define DPS_LAYOUT_IDX_48UP                 DPS_LAYOUT_IDX_47UP   +  1
#define DPS_LAYOUT_IDX_49UP                 DPS_LAYOUT_IDX_48UP   +  1
#define DPS_LAYOUT_IDX_50UP                 DPS_LAYOUT_IDX_49UP   +  1
#define DPS_LAYOUT_IDX_51UP                 DPS_LAYOUT_IDX_50UP   +  1
#define DPS_LAYOUT_IDX_52UP                 DPS_LAYOUT_IDX_51UP   +  1
#define DPS_LAYOUT_IDX_53UP                 DPS_LAYOUT_IDX_52UP   +  1
#define DPS_LAYOUT_IDX_54UP                 DPS_LAYOUT_IDX_53UP   +  1
#define DPS_LAYOUT_IDX_55UP                 DPS_LAYOUT_IDX_54UP   +  1
#define DPS_LAYOUT_IDX_56UP                 DPS_LAYOUT_IDX_55UP   +  1
#define DPS_LAYOUT_IDX_57UP                 DPS_LAYOUT_IDX_56UP   +  1
#define DPS_LAYOUT_IDX_58UP                 DPS_LAYOUT_IDX_57UP   +  1
#define DPS_LAYOUT_IDX_59UP                 DPS_LAYOUT_IDX_58UP   +  1
#define DPS_LAYOUT_IDX_60UP                 DPS_LAYOUT_IDX_59UP   +  1
#define DPS_LAYOUT_IDX_61UP                 DPS_LAYOUT_IDX_60UP   +  1
#define DPS_LAYOUT_IDX_62UP                 DPS_LAYOUT_IDX_61UP   +  1
#define DPS_LAYOUT_IDX_63UP                 DPS_LAYOUT_IDX_62UP   +  1
#define DPS_LAYOUT_IDX_64UP                 DPS_LAYOUT_IDX_63UP   +  1
#define DPS_LAYOUT_IDX_65UP                 DPS_LAYOUT_IDX_64UP   +  1
#define DPS_LAYOUT_IDX_66UP                 DPS_LAYOUT_IDX_65UP   +  1
#define DPS_LAYOUT_IDX_67UP                 DPS_LAYOUT_IDX_66UP   +  1
#define DPS_LAYOUT_IDX_68UP                 DPS_LAYOUT_IDX_67UP   +  1
#define DPS_LAYOUT_IDX_69UP                 DPS_LAYOUT_IDX_68UP   +  1
#define DPS_LAYOUT_IDX_70UP                 DPS_LAYOUT_IDX_69UP   +  1
#define DPS_LAYOUT_IDX_71UP                 DPS_LAYOUT_IDX_70UP   +  1
#define DPS_LAYOUT_IDX_72UP                 DPS_LAYOUT_IDX_71UP   +  1
#define DPS_LAYOUT_IDX_73UP                 DPS_LAYOUT_IDX_72UP   +  1
#define DPS_LAYOUT_IDX_74UP                 DPS_LAYOUT_IDX_73UP   +  1
#define DPS_LAYOUT_IDX_75UP                 DPS_LAYOUT_IDX_74UP   +  1
#define DPS_LAYOUT_IDX_76UP                 DPS_LAYOUT_IDX_75UP   +  1
#define DPS_LAYOUT_IDX_77UP                 DPS_LAYOUT_IDX_76UP   +  1
#define DPS_LAYOUT_IDX_78UP                 DPS_LAYOUT_IDX_77UP   +  1
#define DPS_LAYOUT_IDX_79UP                 DPS_LAYOUT_IDX_78UP   +  1
#define DPS_LAYOUT_IDX_80UP                 DPS_LAYOUT_IDX_79UP   +  1
#define DPS_LAYOUT_IDX_81UP                 DPS_LAYOUT_IDX_80UP   +  1
#define DPS_LAYOUT_IDX_82UP                 DPS_LAYOUT_IDX_81UP   +  1
#define DPS_LAYOUT_IDX_83UP                 DPS_LAYOUT_IDX_82UP   +  1
#define DPS_LAYOUT_IDX_84UP                 DPS_LAYOUT_IDX_83UP   +  1
#define DPS_LAYOUT_IDX_85UP                 DPS_LAYOUT_IDX_84UP   +  1
#define DPS_LAYOUT_IDX_86UP                 DPS_LAYOUT_IDX_85UP   +  1
#define DPS_LAYOUT_IDX_87UP                 DPS_LAYOUT_IDX_86UP   +  1
#define DPS_LAYOUT_IDX_88UP                 DPS_LAYOUT_IDX_87UP   +  1
#define DPS_LAYOUT_IDX_89UP                 DPS_LAYOUT_IDX_88UP   +  1
#define DPS_LAYOUT_IDX_90UP                 DPS_LAYOUT_IDX_89UP   +  1
#define DPS_LAYOUT_IDX_91UP                 DPS_LAYOUT_IDX_90UP   +  1
#define DPS_LAYOUT_IDX_92UP                 DPS_LAYOUT_IDX_91UP   +  1
#define DPS_LAYOUT_IDX_93UP                 DPS_LAYOUT_IDX_92UP   +  1
#define DPS_LAYOUT_IDX_94UP                 DPS_LAYOUT_IDX_93UP   +  1
#define DPS_LAYOUT_IDX_95UP                 DPS_LAYOUT_IDX_94UP   +  1
#define DPS_LAYOUT_IDX_96UP                 DPS_LAYOUT_IDX_95UP   +  1
#define DPS_LAYOUT_IDX_97UP                 DPS_LAYOUT_IDX_96UP   +  1
#define DPS_LAYOUT_IDX_98UP                 DPS_LAYOUT_IDX_97UP   +  1
#define DPS_LAYOUT_IDX_99UP                 DPS_LAYOUT_IDX_98UP   +  1
#define DPS_LAYOUT_IDX_100UP                DPS_LAYOUT_IDX_99UP   +  1
#define DPS_LAYOUT_IDX_101UP                DPS_LAYOUT_IDX_100UP  +  1
#define DPS_LAYOUT_IDX_102UP                DPS_LAYOUT_IDX_101UP  +  1
#define DPS_LAYOUT_IDX_103UP                DPS_LAYOUT_IDX_102UP  +  1
#define DPS_LAYOUT_IDX_104UP                DPS_LAYOUT_IDX_103UP  +  1
#define DPS_LAYOUT_IDX_105UP                DPS_LAYOUT_IDX_104UP  +  1
#define DPS_LAYOUT_IDX_106UP                DPS_LAYOUT_IDX_105UP  +  1
#define DPS_LAYOUT_IDX_107UP                DPS_LAYOUT_IDX_106UP  +  1
#define DPS_LAYOUT_IDX_108UP                DPS_LAYOUT_IDX_107UP  +  1
#define DPS_LAYOUT_IDX_109UP                DPS_LAYOUT_IDX_108UP  +  1
#define DPS_LAYOUT_IDX_110UP                DPS_LAYOUT_IDX_109UP  +  1
#define DPS_LAYOUT_IDX_111UP                DPS_LAYOUT_IDX_110UP  +  1
#define DPS_LAYOUT_IDX_112UP                DPS_LAYOUT_IDX_111UP  +  1
#define DPS_LAYOUT_IDX_113UP                DPS_LAYOUT_IDX_112UP  +  1
#define DPS_LAYOUT_IDX_114UP                DPS_LAYOUT_IDX_113UP  +  1
#define DPS_LAYOUT_IDX_115UP                DPS_LAYOUT_IDX_114UP  +  1
#define DPS_LAYOUT_IDX_116UP                DPS_LAYOUT_IDX_115UP  +  1
#define DPS_LAYOUT_IDX_117UP                DPS_LAYOUT_IDX_116UP  +  1
#define DPS_LAYOUT_IDX_118UP                DPS_LAYOUT_IDX_117UP  +  1
#define DPS_LAYOUT_IDX_119UP                DPS_LAYOUT_IDX_118UP  +  1
#define DPS_LAYOUT_IDX_120UP                DPS_LAYOUT_IDX_119UP  +  1
#define DPS_LAYOUT_IDX_121UP                DPS_LAYOUT_IDX_120UP  +  1
#define DPS_LAYOUT_IDX_122UP                DPS_LAYOUT_IDX_121UP  +  1
#define DPS_LAYOUT_IDX_123UP                DPS_LAYOUT_IDX_122UP  +  1
#define DPS_LAYOUT_IDX_124UP                DPS_LAYOUT_IDX_123UP  +  1
#define DPS_LAYOUT_IDX_125UP                DPS_LAYOUT_IDX_124UP  +  1
#define DPS_LAYOUT_IDX_126UP                DPS_LAYOUT_IDX_125UP  +  1
#define DPS_LAYOUT_IDX_127UP                DPS_LAYOUT_IDX_126UP  +  1
#define DPS_LAYOUT_IDX_128UP                DPS_LAYOUT_IDX_127UP  +  1
#define DPS_LAYOUT_IDX_129UP                DPS_LAYOUT_IDX_128UP  +  1
#define DPS_LAYOUT_IDX_130UP                DPS_LAYOUT_IDX_129UP  +  1
#define DPS_LAYOUT_IDX_131UP                DPS_LAYOUT_IDX_130UP  +  1
#define DPS_LAYOUT_IDX_132UP                DPS_LAYOUT_IDX_131UP  +  1
#define DPS_LAYOUT_IDX_133UP                DPS_LAYOUT_IDX_132UP  +  1
#define DPS_LAYOUT_IDX_134UP                DPS_LAYOUT_IDX_133UP  +  1
#define DPS_LAYOUT_IDX_135UP                DPS_LAYOUT_IDX_134UP  +  1
#define DPS_LAYOUT_IDX_136UP                DPS_LAYOUT_IDX_135UP  +  1
#define DPS_LAYOUT_IDX_137UP                DPS_LAYOUT_IDX_136UP  +  1
#define DPS_LAYOUT_IDX_138UP                DPS_LAYOUT_IDX_137UP  +  1
#define DPS_LAYOUT_IDX_139UP                DPS_LAYOUT_IDX_138UP  +  1
#define DPS_LAYOUT_IDX_140UP                DPS_LAYOUT_IDX_139UP  +  1
#define DPS_LAYOUT_IDX_141UP                DPS_LAYOUT_IDX_140UP  +  1
#define DPS_LAYOUT_IDX_142UP                DPS_LAYOUT_IDX_141UP  +  1
#define DPS_LAYOUT_IDX_143UP                DPS_LAYOUT_IDX_142UP  +  1
#define DPS_LAYOUT_IDX_144UP                DPS_LAYOUT_IDX_143UP  +  1
#define DPS_LAYOUT_IDX_145UP                DPS_LAYOUT_IDX_144UP  +  1
#define DPS_LAYOUT_IDX_146UP                DPS_LAYOUT_IDX_145UP  +  1
#define DPS_LAYOUT_IDX_147UP                DPS_LAYOUT_IDX_146UP  +  1
#define DPS_LAYOUT_IDX_148UP                DPS_LAYOUT_IDX_147UP  +  1
#define DPS_LAYOUT_IDX_149UP                DPS_LAYOUT_IDX_148UP  +  1
#define DPS_LAYOUT_IDX_150UP                DPS_LAYOUT_IDX_149UP  +  1
#define DPS_LAYOUT_IDX_151UP                DPS_LAYOUT_IDX_150UP  +  1
#define DPS_LAYOUT_IDX_152UP                DPS_LAYOUT_IDX_151UP  +  1
#define DPS_LAYOUT_IDX_153UP                DPS_LAYOUT_IDX_152UP  +  1
#define DPS_LAYOUT_IDX_154UP                DPS_LAYOUT_IDX_153UP  +  1
#define DPS_LAYOUT_IDX_155UP                DPS_LAYOUT_IDX_154UP  +  1
#define DPS_LAYOUT_IDX_156UP                DPS_LAYOUT_IDX_155UP  +  1
#define DPS_LAYOUT_IDX_157UP                DPS_LAYOUT_IDX_156UP  +  1
#define DPS_LAYOUT_IDX_158UP                DPS_LAYOUT_IDX_157UP  +  1
#define DPS_LAYOUT_IDX_159UP                DPS_LAYOUT_IDX_158UP  +  1
#define DPS_LAYOUT_IDX_160UP                DPS_LAYOUT_IDX_159UP  +  1
#define DPS_LAYOUT_IDX_161UP                DPS_LAYOUT_IDX_160UP  +  1
#define DPS_LAYOUT_IDX_162UP                DPS_LAYOUT_IDX_161UP  +  1
#define DPS_LAYOUT_IDX_163UP                DPS_LAYOUT_IDX_162UP  +  1
#define DPS_LAYOUT_IDX_164UP                DPS_LAYOUT_IDX_163UP  +  1
#define DPS_LAYOUT_IDX_165UP                DPS_LAYOUT_IDX_164UP  +  1
#define DPS_LAYOUT_IDX_166UP                DPS_LAYOUT_IDX_165UP  +  1
#define DPS_LAYOUT_IDX_167UP                DPS_LAYOUT_IDX_166UP  +  1
#define DPS_LAYOUT_IDX_168UP                DPS_LAYOUT_IDX_167UP  +  1
#define DPS_LAYOUT_IDX_169UP                DPS_LAYOUT_IDX_168UP  +  1
#define DPS_LAYOUT_IDX_170UP                DPS_LAYOUT_IDX_169UP  +  1
#define DPS_LAYOUT_IDX_171UP                DPS_LAYOUT_IDX_170UP  +  1
#define DPS_LAYOUT_IDX_172UP                DPS_LAYOUT_IDX_171UP  +  1
#define DPS_LAYOUT_IDX_173UP                DPS_LAYOUT_IDX_172UP  +  1
#define DPS_LAYOUT_IDX_174UP                DPS_LAYOUT_IDX_173UP  +  1
#define DPS_LAYOUT_IDX_175UP                DPS_LAYOUT_IDX_174UP  +  1
#define DPS_LAYOUT_IDX_176UP                DPS_LAYOUT_IDX_175UP  +  1
#define DPS_LAYOUT_IDX_177UP                DPS_LAYOUT_IDX_176UP  +  1
#define DPS_LAYOUT_IDX_178UP                DPS_LAYOUT_IDX_177UP  +  1
#define DPS_LAYOUT_IDX_179UP                DPS_LAYOUT_IDX_178UP  +  1
#define DPS_LAYOUT_IDX_180UP                DPS_LAYOUT_IDX_179UP  +  1
#define DPS_LAYOUT_IDX_181UP                DPS_LAYOUT_IDX_180UP  +  1
#define DPS_LAYOUT_IDX_182UP                DPS_LAYOUT_IDX_181UP  +  1
#define DPS_LAYOUT_IDX_183UP                DPS_LAYOUT_IDX_182UP  +  1
#define DPS_LAYOUT_IDX_184UP                DPS_LAYOUT_IDX_183UP  +  1
#define DPS_LAYOUT_IDX_185UP                DPS_LAYOUT_IDX_184UP  +  1
#define DPS_LAYOUT_IDX_186UP                DPS_LAYOUT_IDX_185UP  +  1
#define DPS_LAYOUT_IDX_187UP                DPS_LAYOUT_IDX_186UP  +  1
#define DPS_LAYOUT_IDX_188UP                DPS_LAYOUT_IDX_187UP  +  1
#define DPS_LAYOUT_IDX_189UP                DPS_LAYOUT_IDX_188UP  +  1
#define DPS_LAYOUT_IDX_190UP                DPS_LAYOUT_IDX_189UP  +  1
#define DPS_LAYOUT_IDX_191UP                DPS_LAYOUT_IDX_190UP  +  1
#define DPS_LAYOUT_IDX_192UP                DPS_LAYOUT_IDX_191UP  +  1
#define DPS_LAYOUT_IDX_193UP                DPS_LAYOUT_IDX_192UP  +  1
#define DPS_LAYOUT_IDX_194UP                DPS_LAYOUT_IDX_193UP  +  1
#define DPS_LAYOUT_IDX_195UP                DPS_LAYOUT_IDX_194UP  +  1
#define DPS_LAYOUT_IDX_196UP                DPS_LAYOUT_IDX_195UP  +  1
#define DPS_LAYOUT_IDX_197UP                DPS_LAYOUT_IDX_196UP  +  1
#define DPS_LAYOUT_IDX_198UP                DPS_LAYOUT_IDX_197UP  +  1
#define DPS_LAYOUT_IDX_199UP                DPS_LAYOUT_IDX_198UP  +  1
#define DPS_LAYOUT_IDX_200UP                DPS_LAYOUT_IDX_199UP  +  1
#define DPS_LAYOUT_IDX_201UP                DPS_LAYOUT_IDX_200UP  +  1
#define DPS_LAYOUT_IDX_202UP                DPS_LAYOUT_IDX_201UP  +  1
#define DPS_LAYOUT_IDX_203UP                DPS_LAYOUT_IDX_202UP  +  1
#define DPS_LAYOUT_IDX_204UP                DPS_LAYOUT_IDX_203UP  +  1
#define DPS_LAYOUT_IDX_205UP                DPS_LAYOUT_IDX_204UP  +  1
#define DPS_LAYOUT_IDX_206UP                DPS_LAYOUT_IDX_205UP  +  1
#define DPS_LAYOUT_IDX_207UP                DPS_LAYOUT_IDX_206UP  +  1
#define DPS_LAYOUT_IDX_208UP                DPS_LAYOUT_IDX_207UP  +  1
#define DPS_LAYOUT_IDX_209UP                DPS_LAYOUT_IDX_208UP  +  1
#define DPS_LAYOUT_IDX_210UP                DPS_LAYOUT_IDX_209UP  +  1
#define DPS_LAYOUT_IDX_211UP                DPS_LAYOUT_IDX_210UP  +  1
#define DPS_LAYOUT_IDX_212UP                DPS_LAYOUT_IDX_211UP  +  1
#define DPS_LAYOUT_IDX_213UP                DPS_LAYOUT_IDX_212UP  +  1
#define DPS_LAYOUT_IDX_214UP                DPS_LAYOUT_IDX_213UP  +  1
#define DPS_LAYOUT_IDX_215UP                DPS_LAYOUT_IDX_214UP  +  1
#define DPS_LAYOUT_IDX_216UP                DPS_LAYOUT_IDX_215UP  +  1
#define DPS_LAYOUT_IDX_217UP                DPS_LAYOUT_IDX_216UP  +  1
#define DPS_LAYOUT_IDX_218UP                DPS_LAYOUT_IDX_217UP  +  1
#define DPS_LAYOUT_IDX_219UP                DPS_LAYOUT_IDX_218UP  +  1
#define DPS_LAYOUT_IDX_220UP                DPS_LAYOUT_IDX_219UP  +  1
#define DPS_LAYOUT_IDX_221UP                DPS_LAYOUT_IDX_220UP  +  1
#define DPS_LAYOUT_IDX_222UP                DPS_LAYOUT_IDX_221UP  +  1
#define DPS_LAYOUT_IDX_223UP                DPS_LAYOUT_IDX_222UP  +  1
#define DPS_LAYOUT_IDX_224UP                DPS_LAYOUT_IDX_223UP  +  1
#define DPS_LAYOUT_IDX_225UP                DPS_LAYOUT_IDX_224UP  +  1
#define DPS_LAYOUT_IDX_226UP                DPS_LAYOUT_IDX_225UP  +  1
#define DPS_LAYOUT_IDX_227UP                DPS_LAYOUT_IDX_226UP  +  1
#define DPS_LAYOUT_IDX_228UP                DPS_LAYOUT_IDX_227UP  +  1
#define DPS_LAYOUT_IDX_229UP                DPS_LAYOUT_IDX_228UP  +  1
#define DPS_LAYOUT_IDX_230UP                DPS_LAYOUT_IDX_229UP  +  1
#define DPS_LAYOUT_IDX_231UP                DPS_LAYOUT_IDX_230UP  +  1
#define DPS_LAYOUT_IDX_232UP                DPS_LAYOUT_IDX_231UP  +  1
#define DPS_LAYOUT_IDX_233UP                DPS_LAYOUT_IDX_232UP  +  1
#define DPS_LAYOUT_IDX_234UP                DPS_LAYOUT_IDX_233UP  +  1
#define DPS_LAYOUT_IDX_235UP                DPS_LAYOUT_IDX_234UP  +  1
#define DPS_LAYOUT_IDX_236UP                DPS_LAYOUT_IDX_235UP  +  1
#define DPS_LAYOUT_IDX_237UP                DPS_LAYOUT_IDX_236UP  +  1
#define DPS_LAYOUT_IDX_238UP                DPS_LAYOUT_IDX_237UP  +  1
#define DPS_LAYOUT_IDX_239UP                DPS_LAYOUT_IDX_238UP  +  1
#define DPS_LAYOUT_IDX_240UP                DPS_LAYOUT_IDX_239UP  +  1
#define DPS_LAYOUT_IDX_241UP                DPS_LAYOUT_IDX_240UP  +  1
#define DPS_LAYOUT_IDX_242UP                DPS_LAYOUT_IDX_241UP  +  1
#define DPS_LAYOUT_IDX_243UP                DPS_LAYOUT_IDX_242UP  +  1
#define DPS_LAYOUT_IDX_244UP                DPS_LAYOUT_IDX_243UP  +  1
#define DPS_LAYOUT_IDX_245UP                DPS_LAYOUT_IDX_244UP  +  1
#define DPS_LAYOUT_IDX_246UP                DPS_LAYOUT_IDX_245UP  +  1
#define DPS_LAYOUT_IDX_247UP                DPS_LAYOUT_IDX_246UP  +  1
#define DPS_LAYOUT_IDX_248UP                DPS_LAYOUT_IDX_247UP  +  1
#define DPS_LAYOUT_IDX_249UP                DPS_LAYOUT_IDX_248UP  +  1
#define DPS_LAYOUT_IDX_250UP                DPS_LAYOUT_IDX_249UP  +  1              //57FA0000
#define DPS_LAYOUT_IDX_251UP                DPS_LAYOUT_IDX_250UP  +  1
#define DPS_LAYOUT_IDX_252UP                DPS_LAYOUT_IDX_251UP  +  1
#define DPS_LAYOUT_IDX_253UP                DPS_LAYOUT_IDX_252UP  +  1
#define DPS_LAYOUT_IDX_INDEX                DPS_LAYOUT_IDX_253UP  +  1              //57FE0000
#define DPS_LAYOUT_IDX_BRDLESS              DPS_LAYOUT_IDX_INDEX  +  1              //57FF0000
//@}

/**
    Set printer layout for specific 250UP.

    @param[in] layoutIdx Layout index, refer to Layout index 250UP.
*/
extern void PictB_SetPTRCapLayout250UP(UINT8 layoutIdx);

/**
    Get printer layout for specific 250UP.

    @param[in] paperSize Paper size.
    @param[out] layoutIdx Layout index, refer to Layout index 250UP.
*/
extern void PictB_GetLayoutsSpecific250UP(UINT32 paperSize, DPS_LAYOUT_250UP *pLayout250UP);
extern void PictB_GetLayoutsSpecificMore250UP(UINT32 paperSize, DPS_LAYOUT_250UP *pLayout250UP);

#endif

/**
    Make vendor specific version for PictBridge Compliance Test.

    @param[in] strLen The length of bytes in the vendor specific string.
    @param[in] pUsbStr Pointer to a vendor specific string.
    @return
        - @b 0: Success.
        - @b 1: String length is longer than DPS_MAX_SPECVERS_LEN(8).
*/
extern unsigned char PictB_SetVendSpecVer(UINT8 strLen, UINT8 *pUsbStr);

/**
    Make vendor name for PictBridge Compliance Test.

    @param[in] strLen The length of bytes in the vendor name unicode string.
    @param[in] pUsbStr Pointer to a vendor name unicode string.
    @return
        - @b 0: Success.
        - @b 1: String length is longer than DPS_MAX_VENDNAME_LEN(64).
*/
extern unsigned char PictB_SetVendName(UINT8 strLen, UINT8 *pUsbStr);

/**
    Make product name for PictBridge Compliance Test.

    @param[in] strLen The length of bytes in the product name unicode string.
    @param[in] pUsbStr Pointer to a product name unicode string.
    @return
        - @b 0: Success.
        - @b 1: String length is longer than DPS_MAX_PROCNAME_LEN(64).
*/
extern unsigned char PictB_SetProcName(UINT8 strLen, UINT8 *pUsbStr);

/** @fn unsigned char PictB_SetSerialNum(UINT8 strLen, UINT8 *pUsbStr)

    Make serial number for PictBridge Compliance Test.
    @param[in] strLen The length of bytes in the serial number unicode string.
    @param[in] pUsbStr Pointer to a serial number unicode string.
    @return
        - @b 0: Success.
        - @b 1: String length is longer than DPS_MAX_SERIALNO_LEN(64).
*/
extern unsigned char PictB_SetSerialNum(UINT8 strLen, UINT8 *pUsbStr);

/**
    Clear flag for PicBridge module.
    @param[in] clrptn Clear bit pattern.
*/
extern void PictB_ClrFlag(FLGPTN clrptn);

/**
    Wait eventflag for PicBridge module.

    @param[out] p_flgptn The result of waited pattern.
    @param[in] waiptn Wait bit pattern.
    @param[in] wfmode Wait mode.
    @return Error code, refer to ER.
*/
extern ER PictB_WaiFlg(PFLGPTN p_flgptn, FLGPTN waiptn, UINT32 wfmode);

/**
    Check flag for PicBridge module.

    @param[in] chkptn Check bit pattern.
    @return The flag pattern of PicBridge in the range of chkptn.
*/
extern FLGPTN PictB_ChkFlg(FLGPTN chkptn);


//@}
#endif    // _DPSFUNCS_H
